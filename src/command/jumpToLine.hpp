#pragma once
#include <string>

#include "../editor/TextEditor.hpp"
#include "../util/output.hpp"

namespace Command {
    typedef int64_t i64;
    using std::string, std::to_string, TextEditorNS::globalTextEditor, TextEditorNS::Mode;

    [[nodiscard]] inline bool jumpToLineCommand(const string& input, string& result) noexcept {
        ASSERT_FILE_OPENED
        i64 line = atoi(input.c_str());
        size_t size = globalTextEditor.lines.size();
        if (line == 0 && input != "0") {
            result = "无效的行号输入。";
            globalTextEditor.mode = Mode::Command;
            return false;
        }
        if (line < 1 || line > size) {
            result = "行号超出范围（1-" + to_string(size) + "）。";
            return false;
        }
        globalTextEditor.currentLineIndex = line - 1;
        globalTextEditor.mode = Mode::Command;
        Util::printContent(result);
        return true;
    }
}