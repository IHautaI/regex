#include "dfa.hpp"

namespace re {
namespace dfa {

  auto state_comp(const State* x, const State* y)-> bool {
    if(x->size() != y->size()){
      return false;
    }
    auto f = true;
    auto b = y->begin();
    for(auto a = x->begin(); a != x->end(); a++){
      f = f && (*b).first == (*a).first;
      b++;
    }
    return f;
  }

  auto build_dfa(State* start, Cache& cache)-> DFA {
    auto trans = std::vector<std::tuple<std::set<char>, State, State>>();

    auto old_states = std::vector<State*>{start};
    auto new_states = std::vector<State*>();
    auto all_states = std::vector<State*>{start};
    auto cs = re::action::compute_chars(start);

    while(!(old_states.empty() && new_states.empty())){

      if(old_states.empty()){
        cs = re::action::compute_chars(new_states.front());
        auto s = old_states;
        old_states = new_states;
        new_states = s;

      } else {

        if(cs.empty()){

          old_states.erase(old_states.begin());
          cs = re::action::compute_chars(old_states.front());

        } else {
          // add new state
          auto state = old_states.front();
          auto c = *(cs.begin());
          auto new_re = re::action::derive(state.begin(), state.end(), c, cache);

          if(!new_re.empty()){
            auto f = std::find_if(all_states.begin(), all_states.end(), [&](const State* x){return state_comp(x, new_re);});

            if(f == all_states.end()){
              all_states.push_back(new_re);
            }

            auto transition = std::make_tuple(std::set<char>{c}, new_re, old_states.front());
            trans.push_back(transition);

          } else {
              cs.erase(cs.begin());
          }
        }
      }
    }

    auto final = std::vector<std::pair<State, Response>>();
    for(auto& x : all_states){
      auto y = re::action::get_final(x.begin(), x.end());
      if(y.first){
        final.push_back(std::make_pair(x, y.second));
      }
    }

    return DFA(trans, start, final);
  }

}
}
