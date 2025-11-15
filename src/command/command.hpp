#pragma once
#include <string>

#include "../editor/TextEditor.hpp"
#include "../transform/transform.hpp"
#include "../util/output.hpp"

namespace Command {
    using std::string, std::to_string, TextEditorNS::globalTextEditor, TextEditorNS::Mode, TransformNS::Transform;
    constexpr const char* UNKNOWN_COMMAND = "未知命令。输入 H，h 或 ? 获取帮助信息。";

    [[nodiscard]] inline bool command(const string& input, string& result) noexcept {
        if (input.size() != 1) {
            result = UNKNOWN_COMMAND;
            return false;
        }
        switch(input[0]) {
            case 'h': case 'H': case '?':
                Util::printHelp(result);
                return true;
            case 'r': case 'R':
                result = "请输入要读取的文件名。";
                globalTextEditor.mode = Mode::ReadFile;
                return true;
            case 'w': case 'W':
                ASSERT_FILE_OPENED
                result = "请输入要写入的文件名。";
                globalTextEditor.mode = Mode::WriteFile;
                return true;
            case 'i': case 'I':
                ASSERT_FILE_OPENED
                result = "请输入要在其后插入新行的行号。如果需要在最前插入，请输入 0。";
                globalTextEditor.mode = Mode::InsertLineAskForLine;
                return true;
            case 'd': case 'D': {
                ASSERT_FILE_OPENED
                auto* lineEntry = globalTextEditor.lines.at(globalTextEditor.currentLineIndex);
                globalTextEditor.undoStack.push(Transform(Transform::Type::DeleteLine, globalTextEditor.currentLineIndex, lineEntry->data));
                static_cast<void>(globalTextEditor.lines.remove(globalTextEditor.currentLineIndex));
                if(globalTextEditor.currentLineIndex >= globalTextEditor.lines.size() && globalTextEditor.currentLineIndex > 0) globalTextEditor.currentLineIndex--;
                Util::printContent(result);
                return true;
            }
            case 'f': case 'F':
                ASSERT_FILE_OPENED
                result = "请输入查找范围：全文(a) 或 从当前行开始(c)。";
                globalTextEditor.mode = Mode::FindNextAskForRange;
                return true;
            case 'c': case 'C':
                ASSERT_FILE_OPENED
                result = "请输入替换范围：全文(a) 或 仅当前行(c)。";
                globalTextEditor.mode = Mode::ReplaceAskForRange;
                return true;
            case 'n': case 'N':
                ASSERT_FILE_OPENED
                if (globalTextEditor.currentLineIndex + 1 < globalTextEditor.lines.size()) globalTextEditor.currentLineIndex++;
                Util::printContent(result);
                return true;
            case 'p': case 'P':
                ASSERT_FILE_OPENED
                if (globalTextEditor.currentLineIndex > 0) globalTextEditor.currentLineIndex--;
                Util::printContent(result);
                return true;
            case 'b': case 'B':
                ASSERT_FILE_OPENED
                globalTextEditor.currentLineIndex = 0;
                Util::printContent(result);
                return true;
            case 'e': case 'E': {
                ASSERT_FILE_OPENED
                size_t size = globalTextEditor.lines.size();
                if (size > 0) globalTextEditor.currentLineIndex = size - 1;
                Util::printContent(result);
                return true;
            }
            case 'g': case 'G':
                ASSERT_FILE_OPENED
                Util::printContent(result);
                result += "\n请输入要跳转到的行号。";
                globalTextEditor.mode = Mode::JumpToLine;
                return true;
            case 'v': case 'V':
                ASSERT_FILE_OPENED
                Util::printContent(result);
                return true;
            case 'z': case 'Z':
                ASSERT_FILE_OPENED
                if(globalTextEditor.undoStack.empty()) result = "没有可撤销的缓冲区操作。";
                else {
                    Transform lastTransform = globalTextEditor.undoStack.top();
                    globalTextEditor.undoStack.pop();
                    switch (lastTransform.type) {
                        case Transform::Type::ModifyLine: {
                            auto* lineEntry = globalTextEditor.lines.at(lastTransform.lineIndex);
                            lineEntry->data = lastTransform.originalContent;
                            break;
                        }
                        case Transform::Type::AddLine:
                            static_cast<void>(globalTextEditor.lines.remove(lastTransform.lineIndex));
                            if (globalTextEditor.currentLineIndex >= globalTextEditor.lines.size() && globalTextEditor.currentLineIndex > 0) globalTextEditor.currentLineIndex--;
                            break;
                        case Transform::Type::DeleteLine:
                            globalTextEditor.lines.insert(lastTransform.originalContent, lastTransform.lineIndex - 1);
                            break;
                        default:
                            break;
                    }
                    Util::printContent(result);
                    result += "\n已撤销上一次缓冲区操作。";
                }
                return true;
            default:
                result = UNKNOWN_COMMAND;
                return false;
        }
    }
}