#pragma once

#include <vector>
#include <string>
#include <tuple>

struct Date {
    int day = 0;
    int month = 0;
    int year = 0;
};

struct Student {
    std::string name;
    std::string last_name;
    Date birth_date;
};

enum class SortKind {
    Name,
    Date
};


bool DatesNotEqual(const Date& d1, const Date& d2);

bool CompareDates(const Date& d1, const Date& d2);

bool CompareNameFirst(const Student& first, const Student &second);

bool CompareDateFirst(const Student& first, const Student &second);

void SortStudents(std::vector<Student>& students, SortKind sortKind);

