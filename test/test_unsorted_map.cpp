#include "UnsortedMap.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include <gtest.h>

TEST(UnsortedMapTest, DefaultConstructorCreatesEmptyMap) {
  UnsortedMap<int, int> map;

  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0u);
}

TEST(UnsortedMapTest, ClearMakesMapEmpty) {
  UnsortedMap<int, int> map;
  map.insert(1, 1);
  map.insert(2, 1);
  map.insert(3, 1);

  EXPECT_FALSE(map.empty());
  map.clear();

  EXPECT_TRUE(map.empty());
  EXPECT_EQ(map.size(), 0u);
}

TEST(UnsortedMapTest, MapWorksWithDifferentDataTypes) {
  UnsortedMap<char, std::string> map;
  map.insert('a', "ski");
  map.insert('b', "bi");
  map.insert('c', "di");

  EXPECT_EQ(map['a'], "ski");
  EXPECT_EQ(map['b'], "bi");
  EXPECT_EQ(map['c'], "di");
}

TEST(UnsortedMapTest, AtMethodThrowsExceptionForMissingKey) {
  UnsortedMap<int, int> map;
  map.insert(1, 10);

  EXPECT_NO_THROW(map.at(1));
  EXPECT_ANY_THROW(map.at(2));
  EXPECT_ANY_THROW(map.at(10));
}

TEST(UnsortedMapTest, InsertReturnsIteratorToInsertedElement) {
  UnsortedMap<int, int> map;
  auto it1 = map.insert(1, 10);
  EXPECT_EQ(it1, map.find(1));
  auto it2 = map.insert(2, 20);
  EXPECT_EQ(it2, map.find(2));
}

TEST(UnsortedMapTest, EraseReturnsIteratorToNextElement) {
  UnsortedMap<int, int> map;
  map.insert(1, 10);
  map.insert(2, 20);
  map.insert(3, 30);
  map.insert(4, 40);

  auto it1 = map.erase(2);
  EXPECT_EQ(it1, map.find(3));
  auto it2 = map.erase(3);
  EXPECT_EQ(it2, map.find(4));
}

TEST(UnsortedMapTest, InsertExistingElementDoesNotReplaceValue) {
  UnsortedMap<int, int> map;
  map.insert(1, 10);
  EXPECT_EQ(map[1], 10);
  map.insert(1, 20);
  EXPECT_EQ(map[1], 10);
}

TEST(UnsortedMapTest, EraseMissingElementThrows) {
  UnsortedMap<int, int> map;
  EXPECT_ANY_THROW(map.erase(1));
}

TEST(UnsortedMapTest, SubscriptCreatesValueIfMissing) {
  UnsortedMap<int, int> map;
  map[1];
  EXPECT_NE(map.find(1), map.end());
}

TEST(UnsortedMapTest, FindReturnsCorrectIterator) {
  UnsortedMap<int, int> map;
  map.insert(1, 10);
  map.insert(3, 30);
  map.insert(5, 50);

  auto it = map.find(3);
  EXPECT_NE(it, map.end());
  EXPECT_EQ(it->first, 3);
  EXPECT_EQ(it->second, 30);
  EXPECT_EQ(map.find(2), map.end());
}

TEST(UnsortedMapTest, IteratorKeepsInsertionOrder) {
  UnsortedMap<int, int> map;
  map.insert(3, 30);
  map.insert(1, 10);
  map.insert(4, 40);
  map.insert(2, 20);

  std::vector<int> expectedKeys = {3, 1, 4, 2};
  int index = 0;
  for (auto it = map.begin(); it != map.end(); ++it) {
    EXPECT_EQ(it->first, expectedKeys[index]);
    ++index;
  }
  EXPECT_EQ(index, 4);
}

TEST(UnsortedMapTest, StressInsertAndFindShuffledKeys) {
  UnsortedMap<int, int> map;
  constexpr int kCount = 200;

  std::vector<int> keys(kCount);
  std::iota(keys.begin(), keys.end(), 0);
  std::mt19937 rng(42);
  std::shuffle(keys.begin(), keys.end(), rng);

  for (int key : keys) {
    map.insert(key, key * 2);
  }

  for (int key = 0; key < kCount; ++key) {
    auto it = map.find(key);
    ASSERT_NE(it, map.end());
    EXPECT_EQ(it->second, key * 2);
  }
}

TEST(UnsortedMapTest, StressAgainstUnorderedMapModel) {
  UnsortedMap<int, int> map;
  std::unordered_map<int, int> model;
  std::mt19937 rng(1337);
  std::uniform_int_distribution<int> keyDist(0, 4000);
  std::uniform_int_distribution<int> opDist(0, 2);

  constexpr int kOps = 300;
  for (int i = 0; i < kOps; ++i) {
    int key = keyDist(rng);
    int value = i;
    int op = opDist(rng);  // 0 = insert, 1 = erase, 2 = get/create

    if (op == 0) {
      auto mapResult = map.insert(key, value);
      auto modelResult = model.insert({key, value});
      EXPECT_EQ(mapResult != map.end(), modelResult.second);
    } else if (op == 1) {
      bool exists = model.find(key) != model.end();
      if (exists) {
        model.erase(key);
        EXPECT_NO_THROW(map.erase(key));
      } else {
        EXPECT_ANY_THROW(map.erase(key));
      }
    } else {
      int& mapRef = map[key];
      int& modelRef = model[key];
      EXPECT_EQ(mapRef, modelRef);
    }
  }

  EXPECT_EQ(map.size(), model.size());
  for (const auto& kv : model) {
    auto it = map.find(kv.first);
    ASSERT_NE(it, map.end());
    EXPECT_EQ(it->second, kv.second);
  }
}
