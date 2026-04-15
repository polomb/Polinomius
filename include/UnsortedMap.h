#ifndef __UNSORTED_MAP__
#define __UNSORTED_MAP__

#include <stdexcept>
#include <utility>
#include <vector>

template <typename Tkey, typename Tvalue>
class UnsortedMap {
 private:
  std::vector<std::pair<Tkey, Tvalue>> data;

 public:
  class Iterator {
   public:
    typename std::vector<std::pair<Tkey, Tvalue>>::iterator iter;

    Iterator() = default;
    explicit Iterator(typename std::vector<std::pair<Tkey, Tvalue>>::iterator it)
        : iter(it) {}

    std::pair<Tkey, Tvalue>& operator*() const { return *iter; }
    std::pair<Tkey, Tvalue>* operator->() const { return iter.operator->(); }

    Iterator& operator++() {
      ++iter;
      return *this;
    }

    Iterator operator++(int) {
      Iterator temp = *this;
      ++iter;
      return temp;
    }

    bool operator==(const Iterator& other) const { return iter == other.iter; }
    bool operator!=(const Iterator& other) const { return iter != other.iter; }

    operator typename std::vector<std::pair<Tkey, Tvalue>>::iterator() const {
      return iter;
    }
  };

  UnsortedMap() = default;

  Iterator begin() { return Iterator(data.begin()); }
  Iterator end() { return Iterator(data.end()); }

  Iterator find(const Tkey& key) {
    for (auto it = data.begin(); it != data.end(); ++it) {
      if (it->first == key) {
        return Iterator(it);
      }
    }
    return end();
  }

  Tvalue& operator[](const Tkey& key) {
    Iterator it = find(key);
    if (it != end()) {
      return it->second;
    }

    data.push_back(std::make_pair(key, Tvalue()));
    return data.back().second;
  }

  Tvalue& at(const Tkey& key) {
    Iterator it = find(key);
    if (it != end()) {
      return it->second;
    }
    throw std::out_of_range("key was not found");
  }

  Iterator insert(const Tkey& key, const Tvalue& val) {
    Iterator it = find(key);
    if (it != end()) {
      return end();
    }

    data.push_back(std::make_pair(key, val));
    return Iterator(data.end() - 1);
  }

  Iterator erase(const Tkey& key) {
    Iterator it = find(key);
    if (it == end()) {
      throw std::out_of_range("key was not found");
    }

    auto next = data.erase(it.iter);
    return Iterator(next);
  }

  size_t size() const { return data.size(); }
  bool empty() const { return data.empty(); }
  void clear() { data.clear(); }
};

#endif
