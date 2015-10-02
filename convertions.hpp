#ifndef CONVERTIONS_HPP
#define CONVERTIONS_HPP

#include <map>
#include <vector>

namespace utils {

template <class K,class T>
std::vector<T> toVector(const std::map<K, T>& input)
{
  std::vector<T> results;
  results.reserve(input.size());

  for(auto it = input.begin(); it != input.end(); ++it)
    results.push_back( it->second );

  return std::move(results);
}

}

#endif // CONVERTIONS_HPP

