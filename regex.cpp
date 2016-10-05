#include "regex.hpp"

namespace re {
  typedef std::shared_ptr<regex> re_ptr;
  typedef std::map<std::string, std::set<re_ptr>> Cache;

  auto make_cache()-> Cache {
    auto cache = Cache();
    cache["charset"] = std::set<re_ptr>();
    cache["and"] = std::set<re_ptr>();
    cache["or"] = std::set<re_ptr>();
    cache["cat"] = std::set<re_ptr>();
    cache["not"] = std::set<re_ptr>();
    return cache;
  }

}
