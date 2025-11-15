#pragma once
#include <filesystem>
#include <fstream>
#include <string>

#include "../editor/TextEditor.hpp"

namespace Command {
    using std::string, std::ofstream, std::filesystem::exists, std::filesystem::is_regular_file, std::filesystem::path, TextEditorNS::globalTextEditor, TextEditorNS::Mode;

    [[nodiscard]] inline bool writeFileCommand(const string& input, string& result) noexcept {
        ASSERT_FILE_OPENED
        if(exists(input) && is_regular_file(path(input))) {
            result = "文件已存在，是否覆盖？（y/n）";
            globalTextEditor._destFileNameOrTargetStr = input;
            globalTextEditor.mode = Mode::WriteFileConfirm;
            return true;
        }
        ofstream outFile(input);
        if (!outFile.is_open()) {
            result = "无法打开文件进行写入。";
            globalTextEditor.mode = Mode::Command;
            return false;
        }
        for (auto* curr = globalTextEditor.lines.head; curr != nullptr; curr = curr->next) outFile << curr->data << '\n';
        outFile.close();
        result = "已成功写入文件：" + input;
        globalTextEditor.mode = Mode::Command;
        return true;
    }

    [[nodiscard]] inline bool writeFileConfirmCommand(const string& input, string& result) noexcept {
        ASSERT_FILE_OPENED
        if (input != "y" && input != "n") {
            result = "请输入 y（是）或 n（否）。";
            return false;
        }
        if (input == "y") {
            ofstream outFile(globalTextEditor._destFileNameOrTargetStr);
            if (!outFile.is_open()) {
                result = "无法打开文件进行写入。";
                globalTextEditor.mode = Mode::Command;
                return false;
            }
            for (auto* curr = globalTextEditor.lines.head; curr != nullptr; curr = curr->next) outFile << curr->data << '\n';
            outFile.close();
            result = "已成功写入文件：" + globalTextEditor._destFileNameOrTargetStr;
        }
        if (input == "n") result = "写入操作已取消。";
        globalTextEditor.mode = Mode::Command;
        return true;
    }
}