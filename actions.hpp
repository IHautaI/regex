#include "regex.hpp"
#include "basic.hpp"
#include "compound.hpp"
using namespace re;
namespace cmp = re::compound;
namespace bas = re::basic;

typedef std::shared_ptr<regex> re_ptr;
typedef std::map<std::string, std::set<re_ptr>> Cache;
typedef std::function<void()> Response;
typedef std::pair<regex*, Response> Action;
typedef std::vector<Action> State;

const auto no_action = Action(zero, [](){});

template<class It>
auto derive(It start, It end, char& c, Cache& cache)-> State {
  auto res = State();
  std::transform(start, end, std::back_inserter(res), [&](Action& x){auto y = x.first->derivative(c, cache); return std::make_pair(y, x.second);});

  auto zero = basic::build_Zero();

  auto z = false;
  for(auto& x : res){
    z = z && x == zero;
  }

  if(z){
    return std::vector<Action>();
  } else {
    return res;
  }
}


template<class It>
auto get_final(It start, It end)-> Response {
  for(auto x = start; x != end; x++){
    if(x.first->nullable){
      return x.second;
    }
  }
  return no_action.second;
}


// State -> pair of vectors with char sets
// they use integer-set partition fxn here to clean things up
// I'm not implementing that
template<class It>
auto compute_chars(It start, It end)-> std::pair<std::vector<std::set<chars>>, std::vector<std::set<chars>>> {
  auto res = std::vector<std::pair<std::set<char>, std::set<char>>>();
  if(end - start <= 0){
    return res;
  }

  for(auto x = start; x != end; x++){
    auto chrs = std::set<char>();
    auto nots = std::set<char>();
    x->get_chars(chrs, nots);

    res.first.push_back(chrs);
    res.second.push_back(nots);
  }

  return res;
}
