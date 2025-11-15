#include <string>

#include "../command/index.hpp"
#include "TextEditor.hpp"

namespace TextEditorNS {
    using std::string;
    using namespace Command;

    TextEditor globalTextEditor;

    [[nodiscard]] bool TextEditor::processInput(const string& input, string& result) noexcept {
        switch (mode) {
            case Mode::Command:                 return command(input, result);
            case Mode::ReadFile:                return readFileCommand(input, result);
            case Mode::WriteFile:               return writeFileCommand(input, result);
            case Mode::WriteFileConfirm:        return writeFileConfirmCommand(input, result);
            case Mode::InsertLineAskForLine:    return insertLineCommand(input, result);
            case Mode::InsertLineAskForStr:     return insertLineStrCommand(input, result);
            case Mode::FindNextAskForRange:     return findNextCommand(input, result);
            case Mode::FindNextAskForTarget:    return findNextStrCommand(input, result);
            case Mode::ReplaceAskForRange:      return replaceCommand(input, result);
            case Mode::ReplaceAskForTarget:     return replaceTargetCommand(input, result);
            case Mode::ReplaceAskForReplaceStr: return replaceStrCommand(input, result);
            case Mode::JumpToLine:              return jumpToLineCommand(input, result);
            default:
                result = "出现问题：未实现的编辑器模式。";
                return false;
        }
    }
}