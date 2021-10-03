#include "../min_priority_queue/min_priority_queue.h"
#include "../trie/trie.h"
#include <gtest/gtest.h>
#include <string>
#include <vector>

TEST(MinPriorityQueue, PushAndPop1) {
    MinPriorityQueue q;
    q.Push(500, 5);
    q.Push(600, 5);
    q.Push(300, 3);
    q.Push(100, 1);     
    q.Push(200, 1);
    std::vector<KeyPriority> v;
    std::vector<KeyPriority> expected = {{100, 1}, {200, 1}, {300, 3}, {500, 5}, {600, 5}};
    while (!q.Empty()) {
        v.push_back(q.Top());
        KeyPriority kp1 = q.Top();
        KeyPriority kp2 = q.Pop();
        ASSERT_EQ(kp1, kp2);
    }
    ASSERT_EQ(v, expected);
}

TEST(MinPriorityQueue, PushAndPop2) {
    MinPriorityQueue q;
    q.Push(5, 5);
    q.Push(6, 4);
    q.Push(3, 3);
    q.Push(1, 2);     
    q.Push(2, 1);
    q.Push(228, 0);
    std::vector<KeyPriority> v;
    std::vector<KeyPriority> expected = {{228, 0}, {2, 1}, {1, 2}, {3, 3}, {6, 4}, {5, 5}};
    while (!q.Empty()) {
        v.push_back(q.Top());
        KeyPriority kp1 = q.Top();
        KeyPriority kp2 = q.Pop();
        ASSERT_EQ(kp1, kp2);
    }
    ASSERT_EQ(v, expected);
}

TEST(MinPriorityQueue, PushAndPopMAX) {
    MinPriorityQueue q;
    std::vector<KeyPriority> expected;
    for (uint16_t i = 0; i < 50000; ++i) {
        expected.push_back({i, i});
    }
    std::reverse(expected.begin(), expected.end());
    for (uint16_t i = 0; i < 50000; ++i) {
        q.Push(expected[i].key, expected[i].priority);
    }
    std::reverse(expected.begin(), expected.end());
    std::vector<KeyPriority> v;
    while (!q.Empty()) {
        v.push_back(q.Top());
        KeyPriority kp1 = q.Top();
        KeyPriority kp2 = q.Pop();
        ASSERT_EQ(kp1, kp2);
    }
    ASSERT_EQ(v, expected);
}

TEST(Trie, AddAndCheck1) {
    Trie trie(9);
    trie.AddChild(0, 1);
    trie.AddChild(0, 2);
    trie.AddChild(1, 3);
    trie.AddChild(1, 4);
    trie.AddChild(2, 7);
    trie.AddChild(7, 8);
    std::vector<std::pair<uint16_t, std::string> > expected = {{3, "00"}, {4, "01"}, {8, "100"}};
    ASSERT_EQ(expected, trie.LeavesFrom(0));
}

TEST(Trie, AddAndCheckMAX) {
    Trie trie(500);
    for (size_t i = 0; i < 1000; ++i) {
        trie.AddChild(i, i + 1);
    }
    std::vector<std::pair<uint16_t, std::string> > expected;
    std::string s;
    for (uint16_t j = 0; j < 499; ++j) {
        s += "0";
    }
    expected = {{499, s}};
    ASSERT_EQ(expected, trie.LeavesFrom(0));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

