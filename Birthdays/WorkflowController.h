#pragma once
#include <exception>
#include <vector>
#include <Windows.h>

class BirthdayInfo;
class Parser;

class WorkflowController
  {
  public:
    WorkflowController(const wchar_t* path);
    ~WorkflowController();

    void Run();
    const std::vector<BirthdayInfo>& GetNearestBirthdays();

    class CanNotGetHandle : public std::exception {};
    class ErrorOnReadRequest : public std::exception {};

  private:
    void PrepareOverlapped();
    void CreateReadRequest();
    void OnChunkPrepared(size_t chunk_size);
    void OnReadComplete();
    static void WINAPI FileReadComplete(DWORD dwError, DWORD cbTransferred, LPOVERLAPPED lpo);

    static const size_t buffer_size = 20;
    char buffers_[2][buffer_size];
    OVERLAPPED overlapped_[2];
    Parser* parser_;
    char updated_buffer_index_; 
    size_t position_;
    HANDLE db_file_handle_;
    LARGE_INTEGER file_size_;
  };

