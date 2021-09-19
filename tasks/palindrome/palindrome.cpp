#include "palindrome.h"

bool IsPalindrome(const std::string& str) {
    if (str.size() <= 1)
      return true;
    size_t p1 = 0;
    int64_t p2 = str.size() - 1;  // it can become -1!!
    while (p1 < str.size() && p2 >= 0) {
        while (p1 < str.size() && str[p1] == ' ') {
            p1++;
        }
        while (p2 >= 0 && str[p2] == ' ') {
            p2--;
        }
        if (p1 < str.size() && p2 >= 0 && str[p1] != str[p2]) {
            return false;  
        }
        p1++;
        p2--;
    }  
    return true;
}
