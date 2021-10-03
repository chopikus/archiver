#include "min_priority_queue.h"
#include <iostream>

size_t MinPriorityQueue::Parent(size_t i) const {
    if (i == 0) {
        return 0;
    }
    return (i - 1) / 2;
}

void MinPriorityQueue::Heapify(size_t v) {
    while (true) {
        size_t left_child = v * 2 + 1;
        size_t right_child = v * 2 + 2;
        bool can_swap_left = false;
        bool can_swap_right = false;
        if (left_child < elements_.size() && elements_[v] > elements_[left_child]) {
            can_swap_left = true;
        }
        if (right_child < elements_.size() && elements_[v] > elements_[right_child]) {
            can_swap_right = true;
        }  
        if (!can_swap_left && !can_swap_right) {
            break;
        } else if (can_swap_left && (!can_swap_right || elements_[left_child] < elements_[right_child])) {
            std::swap(elements_[left_child], elements_[v]);
            v = left_child;
        } else {
            std::swap(elements_[right_child], elements_[v]);
            v = right_child;
        }
    }
}

void MinPriorityQueue::Push(uint16_t key, uint16_t priority) {
    elements_.push_back(KeyPriority{.key = key, .priority = priority});
    // reordering
    size_t i = elements_.size() - 1;
    while (elements_[Parent(i)] > elements_[i]) {
        std::swap(elements_[Parent(i)], elements_[i]);
        i = Parent(i);
    }
}

KeyPriority MinPriorityQueue::Top() const {
    return elements_[0];
}

KeyPriority MinPriorityQueue::Pop() {
    KeyPriority top = elements_[0];
    std::swap(elements_[0], elements_.back());
    elements_.pop_back();
    Heapify(0);
    return top;
}

bool MinPriorityQueue::Empty() const {
    return elements_.empty();
}
