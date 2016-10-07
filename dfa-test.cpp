#include "dfa.hpp"

using namespace re;

int main(){
  auto cache = make_cache();

  auto open = basic::build_CharSet("([{", cache);
  auto close = basic::build_CharSet(")]}", cache);
  auto UC_name = basic::build_Range('A','Z', cache);
  auto LC_name = basic::build_Range('a', 'z', cache);
  auto num = basic::build_Range('0', '9', cache);
  auto num_rep = basic::build_Rep(num, 0, 5, cache);

  auto UC_second = compound::build_Cat(UC_name, close, cache);
  auto UC_first = compound::build_Cat(open, UC_second, cache);

  auto LC_second = compound::build_Cat(LC_name, close, cache);
  auto LC_first = compound::build_Cat(open, LC_second, cache);

  auto first = compound::build_Or(LC_first, UC_first, cache);

  auto full = compound::build_Cat(first, num_rep, cache);

  std::cout << full << "\n";

  auto start = State();
  start.insert(std::make_pair(full, Response([](){})));

  std::cout << "state constructed\n";

  auto dfa =  dfa::build_dfa(start, cache);
  std::cout << dfa;
}
