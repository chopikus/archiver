#include "sort_students.h"

bool DatesNotEqual(const Date& d1, const Date& d2) {
  return (d1.day != d2.day || d1.month != d2.month || d1.year != d2.year);
}

bool CompareDates(const Date& d1, const Date& d2) {
  return std::tie(d1.year, d1.month, d1.day) < std::tie(d2.year, d2.month, d2.day);
}

bool CompareNameFirst(const Student& first, const Student &second) {
  if (first.last_name != second.last_name)
    return (first.last_name < second.last_name);
  if (first.name != second.name)
    return (first.name < second.name);
  if (DatesNotEqual(first.birth_date, second.birth_date))
    return CompareDates(first.birth_date, second.birth_date);
  return false;
}

bool CompareDateFirst(const Student& first, const Student &second) {
  if (DatesNotEqual(first.birth_date, second.birth_date))
    return CompareDates(first.birth_date, second.birth_date);
  if (first.last_name != second.last_name)
    return (first.last_name < second.last_name);
  if (first.name != second.name)
    return (first.name < second.name);
  return false;
}

void SortStudents(std::vector<Student>& students, SortKind sort_kind) {
  if (sort_kind == SortKind::Name)  {
    sort(students.begin(), students.end(), CompareNameFirst);    
  }
  else if (sort_kind == SortKind::Date) {
    sort(students.begin(), students.end(), CompareDateFirst);    
  }
}
