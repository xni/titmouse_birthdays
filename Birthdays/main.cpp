#include <iostream>
#include <Windows.h>

#include "WorkflowController.h"
#include "Parser.h"


int wmain(int argc, wchar_t* argv[], wchar_t* envp[]) {
  if (argc != 2) {
    std::cerr << "You should provide an argument PATH\n";
    return 1;
    }
  WorkflowController workflow_controller(argv[1]);
  workflow_controller.Run();

  const wchar_t* phrases[] = { L"Сегодня", L"Завтра" };

  for (const auto& birthday_info : workflow_controller.GetNearestBirthdays()) {
    wchar_t r[1000];
    int status = MultiByteToWideChar(
      CP_UTF8,
      0,
      birthday_info.name.c_str(),
      -1,
      r,
      1000);
    MessageBoxExW(NULL, r, phrases[birthday_info.days_remaining], MB_OK, 0);
    // std::cout << birthday_info.days_remaining << " " << birthday_info.name << '\n';
    }
  // std::cout << "Main. Complete\n";
  }