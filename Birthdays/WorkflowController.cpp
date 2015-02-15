#include "WorkflowController.h"

#include "Parser.h"


WorkflowController::WorkflowController(const wchar_t* path)
  : parser_(0)
  , updated_buffer_index_(0)
  , position_(0)
  {
  db_file_handle_ = CreateFile(
    path,
    GENERIC_READ,
    FILE_SHARE_READ,
    NULL,
    OPEN_EXISTING,
    FILE_FLAG_OVERLAPPED,
    0);
  if (db_file_handle_ == INVALID_HANDLE_VALUE)
    throw CanNotGetHandle();
  for (char i = 0; i < 2; ++i) {
    overlapped_[i].OffsetHigh = 0;
    overlapped_[i].hEvent = this;
    }
  file_size_.QuadPart = 0;
  }

void WorkflowController::PrepareOverlapped()
  {
  overlapped_[updated_buffer_index_].Offset = position_;
  }

void WorkflowController::CreateReadRequest()
  {
  PrepareOverlapped();
  const bool read = ReadFileEx(
    db_file_handle_,
    buffers_[updated_buffer_index_],
    buffer_size,
    &overlapped_[updated_buffer_index_],
    WorkflowController::FileReadComplete);
  const DWORD last_error = GetLastError();
  if (last_error != ERROR_SUCCESS) {
    throw ErrorOnReadRequest();
    }
  }

WorkflowController::~WorkflowController()
  {
  CloseHandle(db_file_handle_);
  delete parser_;
  }

void WorkflowController::Run()
  {
  CreateReadRequest();
  parser_ = new Parser();
  GetFileSizeEx(db_file_handle_, &file_size_);
  SleepEx(INFINITE, true);
  }

void WorkflowController::OnChunkPrepared(size_t chunk_size)
  {
  position_ += chunk_size;
  const char just_prepared_buffer = updated_buffer_index_;
  updated_buffer_index_ ^= 1;
  // Buffer is full, so we should continue reading.
  const bool should_continue_reading = position_ < file_size_.QuadPart;
  if (should_continue_reading)
    {
    CreateReadRequest();
    }
  parser_->ParseNextChunk(buffers_[just_prepared_buffer], chunk_size);
  if (should_continue_reading) {
    SleepEx(INFINITE, true);
    }
  else {
    OnReadComplete();
    }
  }

const std::vector<BirthdayInfo>& WorkflowController::GetNearestBirthdays() {
  return parser_->GetNearestBirthdays();
  }

void WorkflowController::OnReadComplete() {
  parser_->Finalize();
  }

// static
void WorkflowController::FileReadComplete(DWORD dwError, DWORD cbTransferred, LPOVERLAPPED lpo) {
  static_cast<WorkflowController*>(lpo->hEvent)->OnChunkPrepared(cbTransferred);
  }