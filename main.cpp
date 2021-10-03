#include <iostream>
#include "min_priority_queue/min_priority_queue.h"

using namespace std;

int main() {
    MinPriorityQueue q;
    
    q.Push(3, 4);
    q.Push(1, 2);
    while (!q.Empty()) {
        KeyPriority kp = q.Top(); 
        cout << kp.priority << ' ' << kp.key << endl;
        q.Pop();
    }
    return 0;
}
