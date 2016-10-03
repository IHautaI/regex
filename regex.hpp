#ifndef REGEX_H
#define REGEX_H

#include <string>
#include <memory>
#include <set>
#include <vector>
#include <map>

namespace re {

class regex {
public:
  bool nullable;

  regex(bool t)
  : nullable(t)
  {}

  ~regex(){}

  virtual auto derivative(char& c, std::map<std::string, std::set<std::shared_ptr<regex>>>& cache)-> regex* = 0;

  virtual auto toString()-> std::string = 0;

  friend std::ostream& operator<<(std::ostream& out, regex* x){
    out << x->toString();
    return out;
  }

};

}

#endif
