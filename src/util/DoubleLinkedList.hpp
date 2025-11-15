#pragma once
#include <type_traits> // IWYU pragma: keep

namespace Util {
    using std::move;

    template <typename EntryType>
    struct DoubleLinkedList {
        struct Entry {
            EntryType data;
            Entry* prev{nullptr};
            Entry* next{nullptr};
        };
        Entry* head{nullptr};

        [[nodiscard]] explicit DoubleLinkedList() noexcept {}
        DoubleLinkedList& operator=(const DoubleLinkedList& copy) noexcept {
            if (this != &copy) {
                clear();
                Entry* curr = copy.head;
                while (curr != nullptr) {
                    push_back(curr->data);
                    curr = curr->next;
                }
            }
            return *this;
        }
        DoubleLinkedList& operator=(DoubleLinkedList&& _move) noexcept {
            if (this != &_move) {
                head = _move.head;
                _move.head = nullptr;
            }
            return *this;
        }
        [[nodiscard]] explicit DoubleLinkedList(const DoubleLinkedList& copy) noexcept { operator=(copy); }
        [[nodiscard]] explicit DoubleLinkedList(DoubleLinkedList&& _move) noexcept { operator=(move(_move)); }
        ~DoubleLinkedList() { clear(); }

        [[nodiscard]] bool empty() const noexcept { return head == nullptr; }

        [[nodiscard]] size_t size() const noexcept {
            size_t count = 0;
            Entry* curr = head;
            while (curr != nullptr) {
                count++;
                curr = curr->next;
            }
            return count;
        }

        [[nodiscard]] Entry* front() noexcept { return head; }
        [[nodiscard]] Entry* back() noexcept {
            Entry* curr = head;
            if (curr == nullptr) return nullptr;
            while(curr->next != nullptr) curr = curr->next;
            return curr;
        }
        [[nodiscard]] Entry* at(size_t index) noexcept {
            Entry* curr = head;
            size_t currIndex = 0;
            while (curr != nullptr) {
                if (currIndex == index) return curr;
                curr = curr->next;
                currIndex++;
            }
            return nullptr;
        }

        void push_back(const EntryType& data) noexcept {
            auto* newEntry = new Entry{data, nullptr, nullptr};
            if (head == nullptr) [[unlikely]] head = newEntry;
            else [[likely]] {
                Entry* tail = head;
                while(tail->next != nullptr) tail = tail->next;
                tail->next = newEntry;
                newEntry->prev = tail;
            }
        }
        void prepend(const EntryType& data) noexcept {
            auto* newEntry = new Entry{data, nullptr, head};
            if (head != nullptr) head->prev = newEntry;
            head = newEntry;
        }
        void insert(const EntryType& data, Entry* afterThis) noexcept {
            if (afterThis == nullptr) return;
            auto* newEntry = new Entry{data, afterThis, afterThis->next};
            if (afterThis->next != nullptr) afterThis->next->prev = newEntry;
            afterThis->next = newEntry;
        }
        void insert(const EntryType& data, size_t afterCurrentIndex) noexcept {
            if (head == nullptr && afterCurrentIndex == 0) {
                push_back(data);
                return;
            }
            Entry* curr = head;
            size_t index = 0;
            while (curr != nullptr && index < afterCurrentIndex) {
                curr = curr->next;
                index++;
            }
            if (curr != nullptr) insert(data, curr);
        }

        [[nodiscard]] Entry* pop_back() noexcept {
            if (head == nullptr) return nullptr;
            Entry* tail = head;
            while(tail->next != nullptr) tail = tail->next;
            if (tail->prev != nullptr) [[likely]] tail->prev->next = nullptr;
            else [[unlikely]] head = nullptr;
            return tail;
        }
        [[nodiscard]] Entry* remove(Entry* toRemove) noexcept {
            if (toRemove == nullptr) return nullptr;
            if (toRemove->prev != nullptr) [[likely]] toRemove->prev->next = toRemove->next;
            else [[unlikely]] head = toRemove->next;
            if (toRemove->next != nullptr) toRemove->next->prev = toRemove->prev;
            return toRemove;
        }
        [[nodiscard]] Entry* remove(size_t index) noexcept {
            Entry* curr = head;
            size_t currIndex = 0;
            while (curr != nullptr) {
                if (currIndex == index) {
                    if (curr->prev != nullptr) [[likely]] curr->prev->next = curr->next;
                    else [[unlikely]] head = curr->next;
                    if (curr->next != nullptr) curr->next->prev = curr->prev;
                    return curr;
                }
                curr = curr->next;
                currIndex++;
            }
            return nullptr;
        }

        void clear() noexcept {
            Entry* curr = head;
            while (curr != nullptr) {
                Entry* next = curr->next;
                delete curr;
                curr = next;
            }
            head = nullptr;
        }
    };
}