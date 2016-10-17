#include "action.hpp"

namespace re {
namespace action {


// State -> pair sets<char>
// they use integer-set partition fxn here to clean things up
// I'm not implementing that, just merging.
auto compute_chars(const State* s)-> std::set<char> {
  auto chrs = std::set<char>();
  auto nots = std::set<char>();

  s->begin()->first->get_chars(chrs, nots);

  auto new_nots = std::set<char>();
  for(auto i = 0; i < 128; i++){
    if(std::find(nots.begin(), nots.end(), i) != nots.end()){
      chrs.insert(i);
    }
  }

  return chrs;
}

}
}
