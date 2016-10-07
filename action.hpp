#ifndef ACTION_H
#define ACTION_H

#include "basic.hpp"
#include "compound.hpp"
#include <functional>
#include <algorithm>

namespace cmp = re::compound;
namespace bas = re::basic;

namespace re {
namespace action {

typedef std::shared_ptr<regex> re_ptr;
typedef std::map<std::string, std::set<re_ptr>> Cache;
typedef std::function<void(void)> Response;
typedef std::pair<regex*, Response> Action;

class comp {
public:
  bool operator()(const Action& x, const Action& y){return x.first < y.first;}
};

typedef std::set<Action, comp> State;


template<class It>
auto derive(It start, It end, char& c, Cache& cache)-> State {
  auto res = State();
  std::transform(start, end, std::inserter(res, res.begin()), [&](const Action& x){auto y = x.first->derivative(c, cache); return std::make_pair(y, x.second);});

  auto zero = basic::build_Zero();

  auto z = std::find_if(res.begin(), res.end(), [&](const Action& x){return x.first != zero;});

  if(z == res.end()){
    return State();
  } else {
    return res;
  }
}


template<class It>
auto get_final(It start, It end)-> std::pair<bool, Response> {
  for(auto x = start; x != end; x++){
    if(x->first->nullable){
      return std::make_pair(true, x->second);
    }
  }
  return std::make_pair(false, Response([](){}));
}

// State -> pair sets<char>
// they use integer-set partition fxn here to clean things up
// I'm not implementing that, just merging.
auto compute_chars(const State& s)-> std::set<char>;

}
}

#endif
