// Author: Evgenii Kazakov. Github: @evgenstf
#include "../ring_buffer.h"
#include <gtest/gtest.h>


TEST(RingBuffer, Push10Elements) {
  ASSERT_EQ(1, 1);

  RingBuffer test(10);

  for (int i = 0; i < 10; ++i) {
    test.push(i);
  }

  for (int i = 0; i < 10; ++i) {
    int x = test.pop();
    ASSERT_EQ(x, i);
  }
}

int main(int argc, char** argv) {
   ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

