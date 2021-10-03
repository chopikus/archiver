// Author: Evgenii Kazakov. Github: @evgenstf
#include "../min_priority_queue/min_priority_queue.h"
#include <gtest/gtest.h>
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

int main(int argc, char** argv) {
   ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

