#pragma once
#include <string>

#include "../editor/TextEditor.hpp"
#include "../transform/transform.hpp"
#include "../util/output.hpp"

namespace Command {
    typedef int64_t i64;
    using std::string, std::to_string, TextEditorNS::globalTextEditor, TextEditorNS::Mode, TransformNS::Transform;

    [[nodiscard]] inline bool insertLineCommand(const string& input, string& result) noexcept {
        ASSERT_FILE_OPENED
        i64 line = atoi(input.c_str());
        size_t size = globalTextEditor.lines.size();
        if (line == 0 && input != "0") {
            result = "无效的行号输入。";
            globalTextEditor.mode = Mode::Command;
            return false;
        }
        if (line < 0 || line > size) {
            result = "行号超出范围（0-" + to_string(size) + "）。";
            return false;
        }
        result = "输入插入的新行内容。";
        globalTextEditor.mode = Mode::InsertLineAskForStr;
        globalTextEditor._auxiliaryLineIndex = line - 1;
        return true;
    }

    [[nodiscard]] inline bool insertLineStrCommand(const string& input, string& result) noexcept {
        ASSERT_FILE_OPENED
        globalTextEditor.mode = Mode::Command;
        if(globalTextEditor._auxiliaryLineIndex == -1) [[unlikely]] globalTextEditor.lines.prepend(input);
        else [[likely]] globalTextEditor.lines.insert(input, globalTextEditor._auxiliaryLineIndex);
        globalTextEditor.undoStack.push(Transform(Transform::Type::AddLine, globalTextEditor._auxiliaryLineIndex + 1, input));
        Util::printContent(result);
        return true;
    }
}