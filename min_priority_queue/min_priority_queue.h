#include <cstddef>
#include <cstdint>
#include <vector>
#include <utility>

#pragma once

using Key = uint16_t;
using Priority = uint16_t; 

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

class MinPriorityQueue {
    // priority queue, element with minimal priority sitting on top.
    // classic implementation using heap.
    public: 
        MinPriorityQueue() {};
        ~MinPriorityQueue() {};
        void Push(Key key, Priority priority);
        KeyPriority Top() const; // returning key, then priority
        KeyPriority Pop();
        bool Empty() const;
    private:
        std::vector<KeyPriority> elements_;
        void Heapify(size_t v); 
        size_t Parent(size_t i) const;
};
