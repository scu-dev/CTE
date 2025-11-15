#pragma once
#include <atomic>

#include "platform.hpp"

namespace Util {
    using std::atomic, std::memory_order_release;
    // Assuming the teacher will only use Windows :)
    #if _CTE_WINDOWS
        inline atomic<bool> gotCtrlC{false};

        inline BOOL WINAPI ConsoleCtrlHandler(DWORD dwCtrlType) {
            if (dwCtrlType == CTRL_C_EVENT) {
                const char crlf[] = "\r\n";
                DWORD written = 0;
                WriteFile(GetStdHandle(STD_OUTPUT_HANDLE), crlf, (DWORD)strlen(crlf), &written, nullptr);
                gotCtrlC.store(true, memory_order_release);
                return TRUE;
            }
            return FALSE;
        }
    #endif
}