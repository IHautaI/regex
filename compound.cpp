#include "basic.hpp"
#include "compound.hpp"
#include <algorithm>
namespace re {
namespace compound {

  //-------AND-------------------

  auto and_flatten(std::set<re_ptr>& ch, regex* z, std::set<regex*>& se)-> void {
    auto f = std::find_if(ch.begin(), ch.end(), [&](const re_ptr& p){return p.get() == z;});

    if(f != ch.end()){
      for(auto& k : ((And*) (*f).get())->members){
        se.insert(k);
      }
    }
    else {
      se.insert(z);
    }
  }


  auto build_And(std::set<regex*>& s, Cache& cache)-> regex* {
    auto se = std::set<regex*>();
    auto ze = basic::build_Zero();

    // if member of s is in cache["and"], replace with members of that regex
    for(auto& z : s){
      if(z == ze){
        return z;
      }

      and_flatten(cache["and"], z, se);
    }

    if(se.empty()){
      return basic::build_Not(ze, cache);
    }

    auto* a = new And(se);
    auto an = std::shared_ptr<regex>(a);
    cache["and"].insert(an);
    return a;
  }

  auto build_And(std::initializer_list<regex*> s, Cache& cache)-> regex* {
    auto se = std::set<regex*>();
    auto ze = basic::build_Zero();
    // if member of s is in cache["and"], replace with members of that regex
    for(auto& z : s){
      if(z == ze){
        return ze;
      }

      and_flatten(cache["and"], z, se);
    }

    if(se.empty()){
      return basic::build_Not(ze, cache);
    }

    for(auto& x : cache["and"]){
      auto y = (And*) x.get();
      if(y->same_as(s)){
        return x.get();
      }
    }

    auto* a = new And(se);
    auto an = std::shared_ptr<regex>(a);
    cache["and"].insert(an);
    return a;
  }


  template<class It>
  auto build_And(It start, It end, Cache& cache)-> regex* {
    auto se = std::set<regex*>();
    auto ze = basic::build_Zero();

    for(auto z = start; z != end; z++){
      if(*z == ze){
        return ze;
      }

      and_flatten(cache["and"], *z, se);
    }

    if(se.empty()){
      return basic::build_Not(ze, cache);
    }

    for(auto& x : cache["and"]){
      auto y = (And*) x.get();
      if(y->same_as(se)){
        return x.get();
      }
    }

    auto a = new And(se);
    auto an = std::shared_ptr<regex>(a);
    cache["and"].insert(an);
    return a;
  }

  auto And::toString()-> std::string {
    auto out = std::string("(&:");

    for(auto x = this->members.begin(); x != this->members.end(); x++){
      out += (*x)->toString();
    }
    out += ")";
    return out;
  }


  auto And::derivative(char& c, Cache& cache)-> regex* {
    auto s = std::set<regex*>();
    for(auto& x : this->members){
      s.insert(x->derivative(c, cache));
    }

    return build_And(s, cache);
  }

  //-------OR--------------------


  auto or_flatten(std::set<re_ptr>& ch, regex* r, std::set<regex*>& se, regex* z)-> void {
    auto f = std::find_if(ch.begin(), ch.end(), [&](const re_ptr& x){return x.get() == r;});

    if(f != ch.end()){
      for(auto& k : ((Or*) (*f).get())->members){
        se.insert(k);
      }
    }
    else {
      if(r != z){
        se.insert(r);
      }
    }
  }

  auto build_Or(std::set<regex*>& s, Cache& cache)-> regex* {
    auto se = std::set<regex*>();
    auto z = basic::build_Zero();
    auto nz = basic::build_Not(z, cache);

    for(auto& r : s){
      if(r == nz){
        return nz;
      }

      or_flatten(cache["or"], r, se, z);
    }

    if(se.empty()){ // empty -> Zero
      return z;
    }


    for(auto& x : cache["or"]){ // check if cached
      auto y = (Or*) x.get();
      if(y->same_as(se)){
        return x.get();
      }
    }

    if(se.size() == 1){ // one entry -> the entry
      return *(se.begin());
    }

    auto* a = new Or(se);
    auto an = std::shared_ptr<regex>(a);
    cache["or"].insert(an);
    return a;
  }

  auto build_Or(regex* a, regex* b, Cache& cache)-> regex* {
    auto se = std::set<regex*>{a, b};
    return build_Or(se, cache);
  }

  auto build_Or(std::initializer_list<regex*> s, Cache& cache)-> regex* {
    auto se = std::set<regex*>(s);
    return build_Or(se, cache);
  }

  template<class It>
  auto build_Or(It start, It end, Cache& cache)-> regex* {
    auto se = std::set<regex*>();
    for(auto x = start; x != end; x++){
      se.insert(x);
    }
    return build_Or(se, cache);
  }

  auto Or::toString()-> std::string {
    auto out = std::string("(|:");

    for(auto x = this->members.begin(); x != this->members.end(); x++){
      out += (*x)->toString();
    }
    out += ")";
    return out;
  }

  auto Or::derivative(char& c, Cache& cache)-> regex* {
    auto s = std::set<regex*>();
    for(auto& x : this->members){
      s.insert(x->derivative(c, cache));
    }

    return build_Or(s, cache);
  }

  //-------CAT-------------------

  auto build_Cat(regex* a, regex* b, Cache& cache)-> regex* {
    auto z = basic::build_Zero();
    auto e = basic::build_Empty();

    if(a == z || b == z){
      return z;
    }

    if(a == e){
      return b;
    }

    if(b == e){
      return a;
    }


    for(auto& x : cache["cat"]){
      auto y = (Cat*) x.get();
      if(y->same_as(a, b)){
        return y;
      }
    }

    auto c = new Cat(a, b);
    auto cat = std::shared_ptr<regex>(c);
    cache["cat"].insert(cat);
    return c;
  }

  auto Cat::toString()-> std::string {
    return this->left->toString() + this->right->toString();
  }


  auto Cat::derivative(char& c, Cache& cache)-> regex* {
    auto l = this->left->derivative(c, cache);
    auto d = build_Cat(l, this->right, cache);
    if(this->left->nullable){
      return build_Or(d, this->right, cache);
    }
    else {
      return d;
    }
  }

}
}
