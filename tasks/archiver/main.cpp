#include <iostream>

#include "ring_buffer.h"

using namespace std;

int main() {
  RingBuffer buffer(100);

  for (int i = 0; i < 10; ++i) {
    buffer.push(i);
  }

  while (buffer.size() > 0) {
    auto x = buffer.pop();
    cout << x << ' ';
  }
  cout << endl;

  return 0;
}
