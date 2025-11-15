#pragma once
#include <filesystem>
#include <fstream>
#include <string>

#include "../editor/TextEditor.hpp"

namespace Command {
    using std::string, std::getline, std::ifstream, std::filesystem::exists, std::filesystem::is_regular_file, std::filesystem::path, std::filesystem::u8path, TextEditorNS::globalTextEditor, TextEditorNS::Mode;

    [[nodiscard]] inline bool readFileCommand(const string& input, string& result) noexcept {
        path filePath(reinterpret_cast<const char8_t*>(input.c_str()));
        if (!exists(filePath)) {
            result = "文件不存在：" + input;
            globalTextEditor.mode = Mode::Command;
            return false;
        }
        if (!is_regular_file(filePath)) {
            result = "不是合法文件：" + input;
            globalTextEditor.mode = Mode::Command;
            return false;
        }
        ifstream file(filePath, std::ios::in);
        if (!file.is_open()) {
            result = "无法打开文件：" + input;
            globalTextEditor.mode = Mode::Command;
            return false;
        }
        globalTextEditor.lines.clear();
        while (!file.eof()) {
            string line;
            getline(file, line);
            globalTextEditor.lines.push_back(line);
        }
        file.close();
        result = "已成功读取文件：" + input;
        globalTextEditor.mode = TextEditorNS::Mode::Command;
        globalTextEditor.fileName = input;
        globalTextEditor.fileOpened = true;
        return true;
    }
}