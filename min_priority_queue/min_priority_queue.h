#include <cstddef>
#include <cstdint>
#include <vector>
#include <utility>

#pragma once

using Key = uint16_t;
using Priority = uint64_t; 

class MinPriorityQueue {
    public: 
        MinPriorityQueue() {};
        ~MinPriorityQueue() {};
        void Push(Key key, Priority priority);
        std::pair<Key, Priority> Top() const;
        std::pair<Key, Priority> Pop();
        bool Empty() const;
    private:
        struct KeyPriority {
            Key key;
            Priority priority;
            bool operator<(const KeyPriority& another) const {
                if (priority != another.priority)
                    return priority < another.priority;
                return key < another.key;
            }
            bool operator>(const KeyPriority& another) const {
                if (priority != another.priority)
                    return priority > another.priority;
                return key > another.key;
            }
            bool operator==(const KeyPriority another) const {
                return (key == another.key && priority == another.priority);
            }
        };

        std::vector<KeyPriority> elements_;
        void Heapify(size_t v); 
        size_t Parent(size_t i) const;
};
