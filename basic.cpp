#include "basic.hpp"
#include "compound.hpp"
#include <algorithm>

namespace re {
namespace basic {
regex* Zero::z = 0;
regex* Empty::e = 0;

  //---------ZERO-----------------------------------

  auto build_Zero()-> regex* {
    if(!Zero::z){
      Zero::z = new Zero();
    }
    return Zero::z;
  }

  auto Zero::toString()-> std::string {
    return std::string("(NULL)");
  }

  //---------EMPTY----------------------------------

  auto build_Empty()-> regex* {
    if(!Empty::e){
      Empty::e = new Empty();
    }
    return Empty::e;
  }


  auto Empty::toString()-> std::string {
    return std::string("\'\'");
  }


  //---------CHARSET--------------------------------

  auto build_CharSet(const std::string& s, Cache& cache)-> regex* {
    auto mem = std::set<int>();

    for(auto& c : s){
      mem.insert(c); // implicit conv char -> int
    }

    for(auto& x : cache["charset"]){
      auto y = (CharSet*) x.get();
      if(y->same_as(mem)){
        return y;
      }
    }

    auto* ptr = new CharSet::CharSet(mem);
    re_ptr out = std::shared_ptr<regex>(ptr);
    cache["charset"].insert(out);
    return ptr;
  }


  auto build_CharSet(const char* str, Cache& cache)-> regex* {
    auto s = std::string(str);
    return build_CharSet(s, cache);
  }


  auto CharSet::toString()-> std::string {
    auto out = std::string("[");

    for(auto& x : members){
      out += (char) x;
    }
    out += "]";
    return out;
  }


  auto build_Range(char& start, char& end, Cache& cache)-> regex* {

    if(start <= end){
      auto mem = std::string("");
      for(auto& i = start; i <= end; i++){
        mem += i;
      }

      return build_CharSet(mem, cache);
    }
    else {
      return build_Zero();
    }
  }

  auto CharSet::derivative(char& c, Cache& cache)-> regex* {
    if(std::find(this->members.begin(), this->members.end(), (int) c) != this->members.end()){
      return build_Empty();
    }
    else {
      return build_Zero();
    }
  }


//---------------NOT---------------------

auto build_Not(regex* reg, Cache& cache)-> regex* {
  auto e = build_Empty();
  if(reg == e){
    return build_Zero();
  }

  for(auto& x : cache["not"]){
    auto y = (Not*) x.get();

    if(y->same_as(reg)){
      return x.get();
    }

    if(x.get() == reg){ // already a Not, return contents
      return ((Not*) x.get())->r;
    }
  }

  auto a = new Not(reg);
  auto an = std::shared_ptr<regex>(a);
  cache["not"].insert(an);
  return a;
}

auto Not::toString()-> std::string {
  auto out = std::string("(~:");
  out += this->r->toString();
  out += ")";
  return out;
}


auto Not::derivative(char& c, Cache& cache)-> regex* {
  auto d = this->r->derivative(c, cache);
  return build_Not(d, cache);
}


//-------------REP-------------------

auto build_Rep(regex* reg, size_t low, size_t high, Cache& cache)-> regex* {
  for(auto& x : cache["rep"]){
    auto y = (Rep*) x.get();
    if(y->same_as(reg) && low == y->low && high == y->high){
      return x.get();
    }
  }

  auto a = new Rep(reg, low, high);
  auto an = std::shared_ptr<regex>(a);
  cache["rep"].insert(an);
  return a;
}

auto Rep::toString()-> std::string {
  return "(" + this->r->toString() + "){" + std::to_string(this->low) + "," + std::to_string(this->high) + "}";
}

auto Rep::derivative(char& c, Cache& cache)-> regex* {
  auto d = this->r->derivative(c, cache);
  auto re = build_Rep(this->r, this->low - 1, ((this->high == SIZE_MAX)? SIZE_MAX : this->high - 1), cache);
  return compound::build_Cat(d, re, cache);
}


}
}
