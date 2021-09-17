#include "sort_students.h"
#include <tuple> 

bool CompareNameFirst(const Student& first, const Student &second) {
  const Date& date_first = first.birth_date;
  const Date& date_second = second.birth_date;
  return std::tie(first.last_name, first.name, date_first.year, date_first.month, date_first.day) <
         std::tie(second.last_name, second.name, date_second.year, date_second.month, date_second.day); 
}

bool CompareDateFirst(const Student& first, const Student &second) {
  const Date& date_first = first.birth_date;
  const Date& date_second = second.birth_date;
  return std::tie(date_first.year, date_first.month, date_first.day, first.last_name, first.name) < 
         std::tie(date_second.year, date_second.month, date_second.day, second.last_name, second.name); 
}

void SortStudents(std::vector<Student>& students, SortKind sort_kind) {
  if (sort_kind == SortKind::Name)  {
    sort(students.begin(), students.end(), CompareNameFirst);    
  }
  else if (sort_kind == SortKind::Date) {
    sort(students.begin(), students.end(), CompareDateFirst);    
  }
}
