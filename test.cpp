#include "regex.hpp"
#include "basic.hpp"
#include "compound.hpp"
using namespace re;
namespace cmp = compound;

#include <iostream>

typedef std::shared_ptr<regex> re_ptr;
typedef std::map<std::string, std::set<re_ptr>> Cache;

int main(){
  auto cache = Cache();
  cache["charset"] = std::set<re_ptr>();
  cache["and"] = std::set<re_ptr>();
  cache["or"] = std::set<re_ptr>();
  cache["cat"] = std::set<re_ptr>();
  cache["not"] = std::set<re_ptr>();

  auto chr = 'e';

  auto zero = basic::build_Zero();
  auto empty = basic::build_Empty();

  std::cout << zero << "\n";
  std::cout << zero->derivative(chr, cache) << "\n\n";

  std::cout << empty << "\n";
  std::cout << empty->derivative(chr, cache) << "\n\n";

  auto chars = basic::build_CharSet("abcd", cache);
  std::cout << chars << "\n";
  std::cout << chars->derivative(chr, cache) << "\n\n";

  auto chars2 = basic::build_CharSet("efgh", cache);
  std::cout << chars2 << "\n";
  std::cout << chars2->derivative(chr, cache) << "\n\n";

  auto nd = cmp::build_And({chars, chars2}, cache);
  std::cout << nd << "\n";
  std::cout << nd->derivative(chr, cache) << "\n\n";

  auto r = cmp::build_Or({nd, chars2}, cache);
  std::cout << r << "\n";
  std::cout << r->derivative(chr, cache) << "\n\n";

  auto c = cmp::build_Cat(r, nd, cache);
  std::cout << c << "\n";
  std::cout << c->derivative(chr, cache) << "\n\n";

  auto d = cmp::build_Cat(c, chars2, cache);
  std::cout << d << "\n";
  std::cout << d->derivative(chr, cache) << "\n\n";

  auto n = basic::build_Not(chars2, cache);
  std::cout << n << "\n";
  std::cout << n->derivative(chr, cache) << "\n\n";

  auto m = basic::build_Rep(n, 2, 5, cache);
  std::cout << m << "\n";
  std::cout << m->derivative(chr, cache) << "\n\n";

  auto e = cmp::build_Cat(m, c, cache);
  std::cout << e << "\n";
  std::cout << e->derivative(chr, cache) << "\n\n";
}
