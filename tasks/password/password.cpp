#include "password.h"

const char MIN_ASCII_CHARACTER = 33;
const char MAX_ASCII_CHARACTER = 126;
const size_t MIN_PASSWORD_SIZE = 8;
const size_t MAX_PASSWORD_SIZE = 14;
const int MIN_CHARACTER_CLASSES = 3;

bool ValidatePassword(const std::string& password) {
    if (password.size() < MIN_PASSWORD_SIZE || password.size() > MAX_PASSWORD_SIZE) {
      return false;
    }
    for (size_t i = 0; i < password.size(); i++) {
      if (password.at(i) < MIN_ASCII_CHARACTER || password.at(i) > MAX_ASCII_CHARACTER) {
        return false;      
      }
    }
    bool has_big_letter = false;
    bool has_small_letter = false;
    bool has_digit = false;
    bool has_another_character = false;
    for (size_t i = 0; i < password.size(); i++) {
      if (password.at(i) >= 'A' && password.at(i) <= 'Z')  {
        has_big_letter = true;
      } else if (password.at(i) >= 'a' && password.at(i) <= 'z') {
        has_small_letter = true;
      } else if (password.at(i) >= '0' && password.at(i) <= '9') {
        has_digit = true;
      } else {
        has_another_character = true;
      }
    }
    int character_classes = 0;
    character_classes += static_cast<int>(has_big_letter);
    character_classes += static_cast<int>(has_small_letter);
    character_classes += static_cast<int>(has_digit);
    character_classes += static_cast<int>(has_another_character);

    return character_classes >= MIN_CHARACTER_CLASSES;
}
