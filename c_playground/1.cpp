#include <iostream>
#include <stdio.h>


struct Stu {
  std::string id;
  std::string name;
};

struct Stu get_s_l() {
  Stu stu1;
  Stu stu2;

  stu1.id = "id1";
  stu1.name = "name1";
  stu2.id = "id2";
  stu2.name = "name2";

  Stu stus[2] = {stu1, stu2};
  return stus[2];
}


int main() {
//   struct Stu* p = get_s_l();
//   std::cout << p->id;

  struct Stu res = get_s_l();
  std::cout << res.name;
  return 0;
}


