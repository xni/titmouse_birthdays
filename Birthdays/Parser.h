#pragma once
#include <vector>
#include <string>
#include <Windows.h>


struct BirthdayInfo {
  int days_remaining;
  std::string name;
  };


class Parser
  {
  public:
    Parser();
    ~Parser();

    void ParseNextChunk(const char* data, size_t length);
    void Finalize();
    const std::vector<BirthdayInfo>& GetNearestBirthdays() {
      return birthdays_list_;
      }

  private:
    enum ParserState {
      PARSING_DAY,
      PARSING_MONTH,
      SKIPPING_WHITECHARACTERS,
      SKIPPING_RECORD,
      APPENDING_RECORD
      };

    ULARGE_INTEGER interval_;
    SYSTEMTIME now_;
    ULARGE_INTEGER now_timestamp_;
    ParserState state_;
    char parsed_day_;
    char parsed_month_;
    std::vector<BirthdayInfo> birthdays_list_;
  };

