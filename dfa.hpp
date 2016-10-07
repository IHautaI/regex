#ifndef DFA_H
#define DFA_H

#include "action.hpp"
#include <iostream>

namespace re {
  typedef std::shared_ptr<regex> re_ptr;
  typedef std::map<std::string, std::set<re_ptr>> Cache;
  typedef std::function<void(void)> Response;
  typedef std::pair<regex*, Response> Action;
  typedef std::set<Action, re::action::comp> State;
  
namespace dfa {

  auto state_comp(const State& x, const State& y)-> bool;

  class DFA {
    std::vector<std::tuple<std::set<char>, State, State>> transitions;
    State start;
    std::vector<std::pair<State, Response>> final_states;

    DFA(std::vector<std::tuple<std::set<char>, State, State>>& t, State& s, std::vector<std::pair<State, Response>>& f)
    : transitions(t)
    , start(s)
    , final_states(f)
    {}

  public:

    friend auto build_dfa(State& start, Cache& cache)-> DFA;

    friend std::ostream& operator<<(std::ostream& out, const DFA& d) {

      for(auto& x : d.transitions){
        for(auto& y : std::get<0>(x)){ // print characters
          out << y;
        }

        out << " ";

        for(auto& y : std::get<1>(x)){ // print the regexen in the state
          out << y.first << ",";
        }

        out << " ";

        for(auto& y : std::get<2>(x)){ // print out regexen in state to transition to
          out << y.first << ",";
        }

        out << "\n";
      }

      return out;
    }
  };

  auto build_dfa(State& start, Cache& cache)-> DFA;

}
}
#endif
