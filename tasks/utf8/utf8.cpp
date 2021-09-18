#include "utf8.h"
#include <cstddef>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>

size_t CountUtf8Symbols(const std::string& str) {
	const char* c_str = str.c_str();
	int symbols = 0;
	while ((*c_str) != '\0') {
		unsigned char byte = *c_str;
		symbols++;
		int ones_in_front = 0;
		for (int i = 7; i >= 0; i--)	{
			if ((byte >> i) & 1) {
				ones_in_front++;
			}
			else {
				break;
			}
		}
		c_str++;
		if (ones_in_front == 1 || ones_in_front > 4) {
			break;	// something has gone horribly wrong
		}
		while (ones_in_front>1 && (*c_str)!='\0')	{
			std::cout << "next " << (int) ((unsigned char) *c_str) << ' ' << std::endl;
			ones_in_front--;
			c_str++;
		}
	}	
	std::cout << "SYMBOLS " << symbols << std::endl;
  return symbols;
}

void EncodeUtf8(const std::vector<uint32_t>& codepoints, std::string& str) {
}

void DecodeUtf8(const std::string& str, std::vector<uint32_t>& codepoints) {
}
