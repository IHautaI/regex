#ifndef COMPOUND_H
#define COMPOUND_H

#include "regex.hpp"

namespace re {

namespace compound {

typedef std::shared_ptr<regex> re_ptr;
typedef std::map<std::string, std::set<re_ptr>> Cache;


//----------AND----------------------

auto build_And(std::set<regex*>& s, Cache& cache)-> regex*;

auto build_And(std::initializer_list<regex*> x, Cache& cache)-> regex*;

template<class It>
auto build_And(It first, It last, Cache& cache)-> regex*;


class And: public regex {
  std::set<regex*> members;

  And(std::set<regex*>& x)
  : regex(true)
  , members(x)
  {
    for(auto& x : this->members){
      this->nullable = this->nullable && x->nullable;
    }
  }

public:
  friend auto and_flatten(std::set<re_ptr>& ch, regex* z, std::set<regex*>& se) -> void;

  friend auto build_And(std::set<regex*>& x, Cache& cache)-> regex*;

  friend auto build_And(std::initializer_list<regex*> x, Cache& cache)-> regex*;

  template<class It>
  friend auto build_And(It first, It last, Cache& cache)-> regex*;

  auto derivative(char& c, Cache& cache)-> regex*;


  auto same_as(const std::set<regex*>& mem){
    return mem == this->members;
  }

  auto toString()-> std::string;
};


//------------OR------------------------

auto or_flatten(std::set<re_ptr>& ch, regex* r, std::set<regex*>& se, regex* z)-> void;

auto build_Or(regex* a, regex* b, Cache& cache)-> regex*;

auto build_Or(std::set<regex*>& x, Cache& cache)-> regex*;

auto build_Or(std::initializer_list<regex*> x, Cache& cache)-> regex*;

template<class It>
auto build_Or(It start, It end, Cache& cache)-> regex*;

class Or: public regex {
  std::set<regex*> members;

  Or(std::set<regex*>& x)
  : regex(false)
  , members(x)
  {
    for(auto& x : this->members){
      this->nullable = this->nullable || x->nullable;
    }
  }

public:
  friend auto or_flatten(std::set<re_ptr>& ch, regex* r, std::set<regex*>& se, regex* z)-> void;

  friend auto build_Or(regex* a, regex* b, Cache& cache)-> regex*;

  friend auto build_Or(std::set<regex*>& x, Cache& cache)-> regex*;

  friend auto build_Or(std::initializer_list<regex*> x, Cache& cache)-> regex*;

  template<class It>
  friend auto build_Or(It start, It end, Cache& cache)-> regex*;

  auto derivative(char& c, Cache& cache)-> regex*;

  auto same_as(std::set<regex*>& mem){
    return mem == this->members;
  }

  auto toString()-> std::string;
};

//-----------CAT------------------



auto build_Cat(regex* a, regex* b, Cache& cache)-> regex*;

class Cat: public regex {
  regex* left;
  regex* right;

  Cat(regex* a, regex* b)
  : regex(false)
  , left(a)
  , right(b)
  {
    this->nullable = a->nullable && b->nullable;
  }

public:
  friend auto build_Cat(regex* a, regex* b, Cache& cache)-> regex*;

  auto derivative(char& c, Cache& cache)-> regex*;

  auto same_as(regex* l, regex* r){
    return l == this->left && r == this->right;
  }

  auto toString()-> std::string;
};

}
}

#endif
