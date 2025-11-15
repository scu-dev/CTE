#pragma once
#include <string>

#include "../editor/TextEditor.hpp"
#include "../transform/transform.hpp"
#include "../util/output.hpp"

namespace Command {
    typedef uint64_t u64;
    using std::string, std::to_string, TextEditorNS::globalTextEditor, TextEditorNS::Mode, TransformNS::Transform;

    [[nodiscard]] inline bool replaceCommand(const string& input, string& result) noexcept {
        ASSERT_FILE_OPENED
        if (input == "a") [[likely]] globalTextEditor._findAll = true;
        else if (input == "c") [[likely]] globalTextEditor._findAll = false;
        else [[unlikely]] {
            result = "无效的范围选项。请输入 a（全文）或 c（仅当前行）。";
            return false;
        }
        result = "请输入要查找的目标文本。";
        globalTextEditor.mode = Mode::ReplaceAskForTarget;
        return true;
    }

    [[nodiscard]] inline bool replaceTargetCommand(const string& input, string& result) noexcept {
        ASSERT_FILE_OPENED
        result = "请输入要替换为的文本。";
        if (input.empty()) {
            result = "目标文本不能为空。";
            globalTextEditor.mode = Mode::Command;
            return false;
        }
        globalTextEditor._destFileNameOrTargetStr = input;
        globalTextEditor.mode = Mode::ReplaceAskForReplaceStr;
        return true;
    }

    [[nodiscard]] inline bool replaceStrCommand(const string& input, string& result) noexcept {
        ASSERT_FILE_OPENED
        u64 replaceCount = 0;
        size_t currentLineIndex = globalTextEditor._findAll ? 0 : globalTextEditor.currentLineIndex;
        auto* currentEntry = globalTextEditor.lines.at(currentLineIndex);
        while (currentEntry != nullptr) {
            bool found = false;
            string copy = currentEntry->data;
            size_t pos = currentEntry->data.find(globalTextEditor._destFileNameOrTargetStr);
            while (pos != string::npos) {
                found = true;
                currentEntry->data.replace(pos, globalTextEditor._destFileNameOrTargetStr.size(), input);
                pos = currentEntry->data.find(globalTextEditor._destFileNameOrTargetStr, pos + input.size());
                replaceCount++;
            }
            if (found) globalTextEditor.undoStack.push(Transform(Transform::Type::ModifyLine, currentLineIndex, copy));
            currentEntry = currentEntry->next;
            if (!globalTextEditor._findAll) [[unlikely]] break;
            currentLineIndex++;
        }
        Util::printContent(result);
        result += "\n共完成 " + to_string(replaceCount) + " 处替换。";
        globalTextEditor.mode = Mode::Command;
        return true;
    }
}