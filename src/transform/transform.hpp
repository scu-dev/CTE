#pragma once
#include <string>

namespace TransformNS {
    typedef uint8_t u8;
    using std::string, std::move;

    struct Transform {
        size_t lineIndex;
        string originalContent;
        enum struct Type : u8 {
            ModifyLine, AddLine, DeleteLine
        } type;

        [[nodiscard]] explicit Transform(Type type, size_t lineIndex, const string& originalContent) noexcept : type(type), lineIndex(lineIndex), originalContent(originalContent) {}
        Transform& operator=(const Transform& copy) noexcept {
            if (this != &copy) {
                lineIndex = copy.lineIndex;
                type = copy.type;
                originalContent = copy.originalContent;
            }
            return *this;
        }
        Transform& operator=(Transform&& _move) noexcept {
            if (this != &_move) {
                lineIndex = _move.lineIndex;
                type = _move.type;
                originalContent = move(_move.originalContent);
            }
            return *this;
        }
        [[nodiscard]] Transform(const Transform& copy) noexcept { operator=(copy); }
        [[nodiscard]] Transform(Transform&& _move) noexcept { operator=(move(_move)); }
        ~Transform() {
            originalContent.~string();
        }
    };
}