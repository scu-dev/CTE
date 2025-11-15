#include <atomic>
#include <string>
#include <iostream>

#include "editor/TextEditor.hpp"
#include "meta.hpp"
#include "util/consoleFix.hpp"
#include "util/platform.hpp"

using std::cout, std::cin, std::string, std::memory_order_acquire, TextEditorNS::globalTextEditor, TextEditorNS::getPrompt, Util::gotCtrlC;

int main() {
    #if _CTE_WINDOWS
        SetConsoleCP(CP_UTF8);
        SetConsoleOutputCP(CP_UTF8);
        SetConsoleCtrlHandler(Util::ConsoleCtrlHandler, TRUE);
    #endif
    string input, resultOrError;
    cout << "欢迎使用简单命令行文本编辑器。\n输入 H，h 或 ? 显示帮助。\n" << CTE_COPYRIGHT_NOTICE << '\n';
    while (true) {
        if (gotCtrlC.load(memory_order_acquire)) break;
        cout << "<" << getPrompt(globalTextEditor.mode) << "> ";
        if (!getline(cin, input)) {
            if (gotCtrlC.load(std::memory_order_acquire)) break;
            if (cin.eof()) break;
            cin.clear();
            continue;
        }
        if (input == "q" || input == "Q") break;
        if (globalTextEditor.processInput(input, resultOrError)) [[likely]] cout << resultOrError << '\n';
        else [[unlikely]] cout << "错误：" << resultOrError << '\n';
    }
    return 0;
}