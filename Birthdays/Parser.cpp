#include "Parser.h"

#include <iostream>
#include <string>


Parser::Parser()
  : state_(PARSING_DAY)
  , parsed_day_(0)
  , parsed_month_(0)
  {
  interval_.QuadPart = 2LLU * 24LLU * 3600LLU * 10000000LLU;
  GetLocalTime(&now_);
  now_.wHour = now_.wMinute = now_.wSecond = now_.wMilliseconds = 0;
  FILETIME t;
  SystemTimeToFileTime(&now_, &t);
  now_timestamp_.HighPart = t.dwHighDateTime;
  now_timestamp_.LowPart = t.dwLowDateTime;
  }


Parser::~Parser()
  {
  }


void Parser::ParseNextChunk(const char* data, size_t length) {
  for (size_t index = 0; index < length; ++index)
    {
    if (state_ == PARSING_DAY) {
      if (data[index] >= '0' && data[index] <= '9') {
        parsed_day_ = parsed_day_ * 10 + (data[index] - '0');
        continue;
        }
      if (data[index] == '.') {
        state_ = PARSING_MONTH;
        parsed_month_ = 0;
        continue;
        }
      }
    if (state_ == PARSING_MONTH) {
      if (data[index] >= '0' && data[index] <= '9') {
        parsed_month_ = parsed_month_ * 10 + (data[index] - '0');
        continue;
        }
      if (data[index] == '.') {
        SYSTEMTIME parsed_day_this_year(now_);
        parsed_day_this_year.wDay = parsed_day_;
        parsed_day_this_year.wMonth = parsed_month_;
        // We are always interested in dates in the future.
        // So, if parsed_date < now, we should add 1 year.
        if ((now_.wMonth > parsed_day_this_year.wMonth) ||
            ((now_.wMonth == parsed_day_this_year.wMonth) &&
              (now_.wDay > parsed_day_this_year.wDay))) {
          parsed_day_this_year.wYear += 1;
          }
        FILETIME t;
        SystemTimeToFileTime(&parsed_day_this_year, &t);
        ULARGE_INTEGER parsed_day_this_year_timestamp;
        parsed_day_this_year_timestamp.HighPart = t.dwHighDateTime;
        parsed_day_this_year_timestamp.LowPart = t.dwLowDateTime;
        // std::cout << "Remaining: " << parsed_day_this_year_timestamp.QuadPart - now_timestamp_.QuadPart
        //     << ". Interval = " << interval_.QuadPart << '\n';
        if (parsed_day_this_year_timestamp.QuadPart - now_timestamp_.QuadPart < interval_.QuadPart) {
          state_ = SKIPPING_WHITECHARACTERS;
          birthdays_list_.push_back(
            BirthdayInfo{ 
              (parsed_day_this_year_timestamp.QuadPart - now_timestamp_.QuadPart) / (24LLU * 3600LLU * 10000000LLU),
              {}
            });
          }
        else {
          state_ = SKIPPING_RECORD;
          }
        parsed_day_ = 0;
        continue;
        }
      }
    if (state_ == SKIPPING_WHITECHARACTERS) {
      if (data[index] == ' ') {
        continue;
        }
      else {
        birthdays_list_.back().name.push_back(data[index]);
        state_ = APPENDING_RECORD;
        continue;
        }
      }
    if (state_ == SKIPPING_RECORD) {
      if (data[index] == '\n') {
        state_ = PARSING_DAY;
        }
      continue;
      }
    if (state_ == APPENDING_RECORD) {
      if (data[index] == '\n') {
        state_ = PARSING_DAY;
        continue;
        }
      else {
        birthdays_list_.back().name.push_back(data[index]);
        continue;
        }
      }
    }
  }

void Parser::Finalize() {
  // std::cout << "Parsing complete\n";
  }
