#pragma once
#include <string>

#include "../editor/TextEditor.hpp"
#include "../meta.hpp"
#include "stringProc.hpp"

namespace Util {
    using std::string, std::to_string, TextEditorNS::globalTextEditor;

    //Resets `result`.
    inline void printContent(string& result) noexcept {
        result = globalTextEditor.fileName + '\n';
        size_t size = globalTextEditor.lines.size(), lineNumber = 1, places = 0;
        if (size == 0) {
            if (globalTextEditor.fileOpened) [[unlikely]] result += "文档为空。";
            else [[likely]] result = "未打开文档。";
            return;
        }
        while (size > 0) {
            places++;
            size /= 10;
        }
        auto* curr = globalTextEditor.lines.front();
        while (curr != nullptr) {
            string numberStr = to_string(lineNumber);
            Util::padStart(numberStr, ' ', places);
            result += numberStr;
            if (globalTextEditor.currentLineIndex + 1 == lineNumber) [[unlikely]] result += "-> ";
            else [[likely]] result += " | ";
            result += curr->data + '\n';
            curr = curr->next;
            lineNumber++;
        }
        result += "第 " + to_string(globalTextEditor.currentLineIndex + 1) + " 行，共 " + to_string(lineNumber - 1) + " 行";
    }

    inline void printHelp(string& result) noexcept {
        result = "\n"
        "R，r    - 读取文件\n"
        "W，w    - 写入文件\n"
        "I，i    - 插入新行\n"
        "D，d    - 删除行\n"
        "F，f    - 查找文本（首次出现）\n"
        "C，c    - 替换文本\n"
        "N，n    - 跳转至下一行\n"
        "P，p    - 跳转至上一行\n"
        "B，b    - 跳转至第一行\n"
        "E，e    - 跳转至最后一行\n"
        "G，g    - 跳转至指定行\n"
        "V，v    - 打印文档内容\n"
        "Z，z    - 撤销上一次缓冲区更改\n"
        "Q，q    - 退出\n"
        "H，h，? - 显示帮助信息\n"
        "\n"
        CTE_COPYRIGHT_NOTICE;
    }
}