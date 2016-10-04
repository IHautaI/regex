#ifndef BASIC_H
#define BASIC_H

#include "regex.hpp"

namespace re {
namespace basic {

typedef std::shared_ptr<regex> re_ptr;
typedef std::map<std::string, std::set<re_ptr>> Cache;


auto build_Zero()-> regex*;

class Zero: public regex {
  static regex* z;

  Zero()
  : regex(false)
  {}

public:
  ~Zero(){}

  auto derivative(char& c, Cache& cache)-> regex* {
    return build_Zero();
  }

  auto toString()-> std::string;

  friend auto build_Zero()-> regex*;
};

auto build_Empty()-> regex*;

class Empty: public regex {
  static regex* e;

  Empty()
  : regex(true)
  {}

public:
  ~Empty(){}

  auto derivative(char& c, Cache& cache)-> regex* {
    return build_Zero();
  }

  auto toString()-> std::string;

  friend auto build_Empty()-> regex*;
};


auto build_CharSet(const std::string& s, Cache& cache)-> regex*;
auto build_CharSet(const char* str, Cache& cache)-> regex*;

class CharSet : public regex {
  std::set<int> members;

  CharSet(std::set<int>& x)
  : regex(false)
  , members(x)
  {}

public:
  ~CharSet(){}

  friend auto build_CharSet(const std::string& s, Cache& cache)-> regex*;

  auto derivative(char& c, Cache& cache)-> regex*;

  virtual auto toString()-> std::string;

  auto same_as(std::set<int>& mem)-> bool {
    return this->members == mem;
  }

};

auto make_range(char& start, char& end)-> regex*;


//-----------NOT---------------------

auto build_Not(regex* reg, Cache& cache)-> regex*;

class Not: public regex {
  regex* r;

  Not(regex* reg)
  : regex(!reg->nullable)
  , r(reg)
  {}

public:
  friend auto build_Not(regex* reg, Cache& cache)-> regex*;

  auto derivative(char& c, Cache& cache)-> regex*;


  auto toString()->std::string;

  auto same_as(regex* r){
    return this->r == r;
  }
};


//------------REP------------------------

auto build_Rep(regex* reg, size_t low, size_t high, Cache& cache)-> regex*;

class Rep: public regex {
  regex* r;
  size_t low;
  size_t high;

  Rep(regex* reg, size_t l, size_t h)
  : regex(true)
  , r(reg)
  , low(l)
  , high(h)
  {}

public:

  friend auto build_Rep(regex* reg, size_t low, size_t high, Cache& cache)-> regex*;
  friend auto build_Star(regex* reg, size_t low, Cache& cache)-> regex*;
  friend auto build_Star(regex* reg, Cache& cache)-> regex*;

  auto derivative(char& c, Cache& cache)-> regex*;

  auto toString()-> std::string;

  auto same_as(regex* r){
    return this->r == r;
  }

};

}
}
#endif
