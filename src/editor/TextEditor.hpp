#pragma once
#include <stack>
#include <string>
#include <unordered_map>

#include "../transform/transform.hpp"
#include "../util/DoubleLinkedList.hpp"

constexpr const char* FILE_NOT_OPENED_MSG = "未打开文档。";

namespace TextEditorNS {
    typedef uint8_t u8;
    typedef int64_t i64;
    using std::move, std::unordered_map, std::string, Util::DoubleLinkedList, std::stack, TransformNS::Transform;

    enum struct Mode : u8 {
        Command, ReadFile,
        WriteFile, WriteFileConfirm,
        InsertLineAskForLine, InsertLineAskForStr,
        FindNextAskForRange, FindNextAskForTarget,
        ReplaceAskForRange, ReplaceAskForTarget, ReplaceAskForReplaceStr,
        JumpToLine
    };

    inline const unordered_map<Mode, const char*> promptMap{
        {Mode::Command, "命令"},
        {Mode::ReadFile, "读取文件"},
        {Mode::WriteFile, "写入文件"},
        {Mode::WriteFileConfirm, "确认覆盖"},
        {Mode::InsertLineAskForLine, "插入新行：行号"},
        {Mode::InsertLineAskForStr, "插入新行：内容"},
        {Mode::FindNextAskForRange, "查找：范围"},
        {Mode::FindNextAskForTarget, "查找：目标文本"},
        {Mode::ReplaceAskForRange, "替换：范围"},
        {Mode::ReplaceAskForTarget, "替换：目标文本"},
        {Mode::ReplaceAskForReplaceStr, "替换：新文本"},
        {Mode::JumpToLine, "跳转：行号"}
    };

    inline const char* getPrompt(Mode mode) noexcept { return promptMap.at(mode); }

    struct TextEditor {
        DoubleLinkedList<string> lines;
        stack<Transform> undoStack;
        Mode mode{Mode::Command};
        bool fileOpened{false}, _findAll;
        string fileName{}, _destFileNameOrTargetStr;
        size_t currentLineIndex{0};
        i64 _auxiliaryLineIndex;

        [[nodiscard]] bool processInput(const string& input, string& result) noexcept;
    };

    extern TextEditor globalTextEditor;

    #define ASSERT_FILE_OPENED if (!globalTextEditor.fileOpened) {\
        result = FILE_NOT_OPENED_MSG; \
        return false; \
    }
}