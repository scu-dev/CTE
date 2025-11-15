#pragma once
#include <string>

#include "../editor/TextEditor.hpp"

namespace Command {
    using std::string, std::to_string, TextEditorNS::globalTextEditor, TextEditorNS::Mode;

    [[nodiscard]] inline bool findNextCommand(const string& input, string& result) noexcept {
        ASSERT_FILE_OPENED
        if (input == "a") [[likely]] globalTextEditor._findAll = true;
        else if (input == "c") [[likely]] globalTextEditor._findAll = false;
        else [[unlikely]] {
            result = "无效的范围选项。请输入 a（全文）或 c（从当前行开始）。";
            return false;
        }
        result = "请输入要查找的目标文本。";
        globalTextEditor.mode = Mode::FindNextAskForTarget;
        return true;
    }

    [[nodiscard]] inline bool findNextStrCommand(const string& input, string& result) noexcept {
        ASSERT_FILE_OPENED
        if (input.empty()) {
            result = "目标文本不能为空。";
            globalTextEditor.mode = Mode::Command;
            return false;
        }
        size_t currentLineIndex = globalTextEditor._findAll ? 0 : globalTextEditor.currentLineIndex;
        auto* currentEntry = globalTextEditor.lines.at(currentLineIndex);
        while (currentEntry != nullptr) {
            if (currentEntry->data.find(input) != string::npos) {
                result = "找到匹配项，位于第 " + to_string(currentLineIndex + 1) + " 行：" + currentEntry->data;
                globalTextEditor.mode = Mode::Command;
                return true;
            }
            currentEntry = currentEntry->next;
            currentLineIndex++;
        }
        result = "未找到匹配项。";
        globalTextEditor.mode = Mode::Command;
        return true;
    }
}