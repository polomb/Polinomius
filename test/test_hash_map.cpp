#include <gtest.h>

#include <algorithm>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>

#include "HashMap.h"

// ═══════════════════════════════════════════════════════════════════════════
//  Helpers
// ═══════════════════════════════════════════════════════════════════════════

// Collect all key-value pairs from HashMap into a sorted vector for
// order-independent comparison.
template <typename K, typename V>
std::vector<std::pair<K, V>> to_sorted_vec(HashMap<K, V>& map) {
    std::vector<std::pair<K, V>> result;
    for (auto& [k, v] : map) result.emplace_back(k, v);
    std::sort(result.begin(), result.end());
    return result;
}

// ═══════════════════════════════════════════════════════════════════════════
//  1. Construction & empty state
// ═══════════════════════════════════════════════════════════════════════════

TEST(HashMapConstruct, DefaultEmpty) {
    HashMap<int, int> m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
    EXPECT_GT(m.bucket_count(), 0u);
}

TEST(HashMapConstruct, CopyConstructor) {
    HashMap<int, int> a;
    a.insert(1, 10);
    a.insert(2, 20);

    HashMap<int, int> b(a);
    EXPECT_EQ(b.size(), 2u);
    EXPECT_EQ(*b.find(1), 10);
    EXPECT_EQ(*b.find(2), 20);
}

TEST(HashMapConstruct, MoveConstructor) {
    HashMap<int, int> a;
    a.insert(1, 10);
    a.insert(2, 20);

    HashMap<int, int> b(std::move(a));
    EXPECT_EQ(b.size(), 2u);
    EXPECT_TRUE(a.empty());
}

TEST(HashMapConstruct, CopyAssignment) {
    HashMap<int, int> a;
    a.insert(5, 50);

    HashMap<int, int> b;
    b = a;
    EXPECT_EQ(b.size(), 1u);
    EXPECT_EQ(*b.find(5), 50);
}

TEST(HashMapConstruct, MoveAssignment) {
    HashMap<int, int> a;
    a.insert(7, 70);

    HashMap<int, int> b;
    b = std::move(a);
    EXPECT_EQ(b.size(), 1u);
    EXPECT_TRUE(a.empty());
}

TEST(HashMapConstruct, SelfCopyAssignment) {
    HashMap<int, int> m;
    m.insert(1, 1);
    m = m;  // must not crash / corrupt
    EXPECT_EQ(m.size(), 1u);
}

// ═══════════════════════════════════════════════════════════════════════════
//  2. insert
// ═══════════════════════════════════════════════════════════════════════════

TEST(HashMapInsert, ReturnsTrueOnNew) {
    HashMap<int, int> m;
    EXPECT_TRUE(m.insert(1, 100));
}

TEST(HashMapInsert, ReturnsFalseOnDuplicate) {
    HashMap<int, int> m;
    m.insert(1, 100);
    EXPECT_FALSE(m.insert(1, 200));
}

TEST(HashMapInsert, UpdatesValueOnDuplicate) {
    HashMap<int, int> m;
    m.insert(1, 100);
    m.insert(1, 200);
    EXPECT_EQ(*m.find(1), 200);
}

TEST(HashMapInsert, SizeGrowsOnNewKeys) {
    HashMap<int, int> m;
    for (int i = 0; i < 10; ++i) m.insert(i, i);
    EXPECT_EQ(m.size(), 10u);
}

TEST(HashMapInsert, SizeUnchangedOnDuplicate) {
    HashMap<int, int> m;
    m.insert(1, 10);
    m.insert(1, 20);
    EXPECT_EQ(m.size(), 1u);
}

TEST(HashMapInsert, ManyDistinctKeys) {
    HashMap<int, int> m;
    for (int i = 0; i < 200; ++i) m.insert(i, i * 2);
    EXPECT_EQ(m.size(), 200u);
    for (int i = 0; i < 200; ++i) EXPECT_EQ(*m.find(i), i * 2);
}

// ═══════════════════════════════════════════════════════════════════════════
//  3. find / contains / at
// ═══════════════════════════════════════════════════════════════════════════

TEST(HashMapLookup, FindReturnsNullptrOnMiss) {
    HashMap<int, int> m;
    EXPECT_EQ(m.find(42), nullptr);
}

TEST(HashMapLookup, FindReturnsPointerOnHit) {
    HashMap<int, int> m;
    m.insert(42, 99);
    ASSERT_NE(m.find(42), nullptr);
    EXPECT_EQ(*m.find(42), 99);
}

TEST(HashMapLookup, ContainsFalseOnEmpty) {
    HashMap<int, int> m;
    EXPECT_FALSE(m.contains(1));
}

TEST(HashMapLookup, ContainsTrueAfterInsert) {
    HashMap<int, int> m;
    m.insert(1, 10);
    EXPECT_TRUE(m.contains(1));
}

TEST(HashMapLookup, AtThrowsOnMiss) {
    HashMap<int, int> m;
    EXPECT_THROW(m.at(99), std::out_of_range);
}

TEST(HashMapLookup, AtReturnsValueOnHit) {
    HashMap<int, int> m;
    m.insert(3, 33);
    EXPECT_EQ(m.at(3), 33);
}

TEST(HashMapLookup, ConstFind) {
    HashMap<int, int> m;
    m.insert(5, 55);
    const HashMap<int, int>& cm = m;
    ASSERT_NE(cm.find(5), nullptr);
    EXPECT_EQ(*cm.find(5), 55);
}

TEST(HashMapLookup, ConstAt) {
    HashMap<int, int> m;
    m.insert(5, 55);
    const HashMap<int, int>& cm = m;
    EXPECT_EQ(cm.at(5), 55);
    EXPECT_THROW(cm.at(999), std::out_of_range);
}

// ═══════════════════════════════════════════════════════════════════════════
//  4. operator[]
// ═══════════════════════════════════════════════════════════════════════════

TEST(HashMapOperatorBracket, InsertsDefault) {
    HashMap<int, int> m;
    m[7];   // default int = 0
    EXPECT_TRUE(m.contains(7));
    EXPECT_EQ(m[7], 0);
}

TEST(HashMapOperatorBracket, AssignsValue) {
    HashMap<std::string, int> m;
    m["hello"] = 42;
    EXPECT_EQ(m.at("hello"), 42);
}

TEST(HashMapOperatorBracket, UpdatesExisting) {
    HashMap<int, int> m;
    m[1] = 10;
    m[1] = 20;
    EXPECT_EQ(m[1], 20);
    EXPECT_EQ(m.size(), 1u);
}

// ═══════════════════════════════════════════════════════════════════════════
//  5. erase
// ═══════════════════════════════════════════════════════════════════════════

TEST(HashMapErase, ReturnsFalseOnMiss) {
    HashMap<int, int> m;
    EXPECT_FALSE(m.erase(42));
}

TEST(HashMapErase, ReturnsTrueOnHit) {
    HashMap<int, int> m;
    m.insert(1, 10);
    EXPECT_TRUE(m.erase(1));
}

TEST(HashMapErase, SizeDecreases) {
    HashMap<int, int> m;
    m.insert(1, 10);
    m.insert(2, 20);
    m.erase(1);
    EXPECT_EQ(m.size(), 1u);
}

TEST(HashMapErase, ElementNotFoundAfterErase) {
    HashMap<int, int> m;
    m.insert(1, 10);
    m.erase(1);
    EXPECT_FALSE(m.contains(1));
    EXPECT_EQ(m.find(1), nullptr);
}

TEST(HashMapErase, EraseHead) {
    // The most recently push_front-ed element is at the head of its bucket.
    // Erase it explicitly to cover the head-erase branch.
    HashMap<int, int> m(1);  // 1 bucket → all keys collide
    m.insert(0, 0);
    m.insert(1, 1);   // same bucket, becomes head
    EXPECT_TRUE(m.erase(1));
    EXPECT_FALSE(m.contains(1));
    EXPECT_TRUE(m.contains(0));
}

TEST(HashMapErase, EraseNonHead) {
    HashMap<int, int> m(1);
    m.insert(0, 0);
    m.insert(1, 1);
    EXPECT_TRUE(m.erase(0));   // tail element
    EXPECT_FALSE(m.contains(0));
    EXPECT_TRUE(m.contains(1));
}

TEST(HashMapErase, EraseAllElements) {
    HashMap<int, int> m;
    for (int i = 0; i < 5; ++i) m.insert(i, i);
    for (int i = 0; i < 5; ++i) m.erase(i);
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
}

// ═══════════════════════════════════════════════════════════════════════════
//  6. clear
// ═══════════════════════════════════════════════════════════════════════════

TEST(HashMapClear, EmptyAfterClear) {
    HashMap<int, int> m;
    for (int i = 0; i < 20; ++i) m.insert(i, i);
    m.clear();
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.size(), 0u);
}

TEST(HashMapClear, InsertAfterClear) {
    HashMap<int, int> m;
    m.insert(1, 1);
    m.clear();
    m.insert(2, 2);
    EXPECT_FALSE(m.contains(1));
    EXPECT_TRUE(m.contains(2));
    EXPECT_EQ(m.size(), 1u);
}

// ═══════════════════════════════════════════════════════════════════════════
//  7. Iterator
// ═══════════════════════════════════════════════════════════════════════════

TEST(HashMapIterator, EmptyMapBeginEqualsEnd) {
    HashMap<int, int> m;
    EXPECT_EQ(m.begin(), m.end());
}

TEST(HashMapIterator, IteratesAllElements) {
    HashMap<int, int> m;
    for (int i = 0; i < 5; ++i) m.insert(i, i * 10);

    std::vector<int> keys;
    for (auto& [k, v] : m) keys.push_back(k);
    std::sort(keys.begin(), keys.end());

    ASSERT_EQ(keys.size(), 5u);
    for (int i = 0; i < 5; ++i) EXPECT_EQ(keys[i], i);
}

TEST(HashMapIterator, ValueAccessViaArrow) {
    HashMap<std::string, int> m;
    m.insert("x", 7);
    auto it = m.begin();
    EXPECT_EQ(it->second, 7);
}

TEST(HashMapIterator, PostfixIncrement) {
    HashMap<int, int> m;
    m.insert(1, 1);
    m.insert(2, 2);
    auto it = m.begin();
    auto prev = it++;
    EXPECT_NE(prev, it);
}

TEST(HashMapIterator, IteratorCountMatchesSize) {
    HashMap<int, int> m;
    for (int i = 0; i < 30; ++i) m.insert(i, i);

    std::size_t count = 0;
    for ([[maybe_unused]] auto& p : m) ++count;
    EXPECT_EQ(count, m.size());
}

TEST(HashMapIterator, NoElementsVisitedTwice) {
    HashMap<int, int> m;
    for (int i = 0; i < 20; ++i) m.insert(i, i);

    std::vector<int> seen;
    for (auto& [k, v] : m) seen.push_back(k);
    std::sort(seen.begin(), seen.end());
    auto unique_end = std::unique(seen.begin(), seen.end());
    EXPECT_EQ(unique_end, seen.end());
}

// ═══════════════════════════════════════════════════════════════════════════
//  8. Rehash / load factor
// ═══════════════════════════════════════════════════════════════════════════

TEST(HashMapRehash, DataIntactAfterRehash) {
    // Insert enough elements to trigger at least one rehash
    HashMap<int, int> m(4);  // tiny start
    for (int i = 0; i < 100; ++i) m.insert(i, i * 3);
    EXPECT_EQ(m.size(), 100u);
    for (int i = 0; i < 100; ++i) EXPECT_EQ(*m.find(i), i * 3);
}

TEST(HashMapRehash, LoadFactorStaysBelow1) {
    HashMap<int, int> m(4);
    for (int i = 0; i < 200; ++i) m.insert(i, i);
    EXPECT_LT(m.load_factor(), 1.0f);
}

// ═══════════════════════════════════════════════════════════════════════════
//  9. String keys
// ═══════════════════════════════════════════════════════════════════════════

TEST(HashMapStringKeys, InsertAndFind) {
    HashMap<std::string, std::string> m;
    m.insert("foo", "bar");
    m.insert("baz", "qux");
    EXPECT_EQ(*m.find("foo"), "bar");
    EXPECT_EQ(*m.find("baz"), "qux");
}

TEST(HashMapStringKeys, EraseByString) {
    HashMap<std::string, std::string> m;
    m.insert("key", "value");
    m.erase("key");
    EXPECT_FALSE(m.contains("key"));
}

// ═══════════════════════════════════════════════════════════════════════════
//  10. Collision correctness (force single bucket)
// ═══════════════════════════════════════════════════════════════════════════

TEST(HashMapCollision, AllKeysInOneBucket) {
    // bucket_count=1 → every key goes into bucket 0
    HashMap<int, int> m(1);
    for (int i = 0; i < 10; ++i) m.insert(i, i + 100);
    EXPECT_EQ(m.size(), 10u);
    for (int i = 0; i < 10; ++i) EXPECT_EQ(*m.find(i), i + 100);
}

TEST(HashMapCollision, EraseFromCollisionChain) {
    HashMap<int, int> m(1);
    for (int i = 0; i < 5; ++i) m.insert(i, i);
    m.erase(2);   // middle of chain
    EXPECT_FALSE(m.contains(2));
    for (int i : {0, 1, 3, 4}) EXPECT_TRUE(m.contains(i));
}

// ═══════════════════════════════════════════════════════════════════════════
//  11. Stress tests
// ═══════════════════════════════════════════════════════════════════════════

// Stress 1: large sequential insert + lookup
TEST(HashMapStress, LargeSequentialInsertLookup) {
    constexpr int N = 4000000;
    HashMap<int, int> m;
    for (int i = 0; i < N; ++i) m.insert(i, i * 7);
    EXPECT_EQ(m.size(), static_cast<std::size_t>(N));
    for (int i = 0; i < N; ++i) {
        ASSERT_NE(m.find(i), nullptr) << "missing key " << i;
        EXPECT_EQ(*m.find(i), i * 7);
    }
}

// Stress 2: random insert / erase / lookup — cross-checked against std::unordered_map
TEST(HashMapStress, RandomOpsMatchStdUnorderedMap) {
    constexpr int N = 5'000;
    std::mt19937 rng(42);
    std::uniform_int_distribution<int> keyDist(0, 500);
    std::uniform_int_distribution<int> valDist(0, 10'000);
    std::uniform_int_distribution<int> opDist(0, 2);  // 0=insert, 1=erase, 2=find

    HashMap<int, int> hm;
    std::unordered_map<int, int> ref;

    for (int i = 0; i < N; ++i) {
        int op  = opDist(rng);
        int key = keyDist(rng);
        int val = valDist(rng);

        if (op == 0) {
            bool hmInserted  = hm.insert(key, val);
            bool refInserted = ref.emplace(key, val).second;
            if (!refInserted) {
                // key already existed → both should update
                ref[key] = val;
                hm.insert(key, val);
            }
            (void)hmInserted;
        } else if (op == 1) {
            bool hmErased  = hm.erase(key);
            bool refErased = ref.erase(key) > 0;
            EXPECT_EQ(hmErased, refErased) << "erase mismatch for key " << key;
        } else {
            bool hmHas  = hm.contains(key);
            bool refHas = ref.count(key) > 0;
            EXPECT_EQ(hmHas, refHas) << "contains mismatch for key " << key;
            if (hmHas && refHas) {
                EXPECT_EQ(*hm.find(key), ref[key])
                    << "value mismatch for key " << key;
            }
        }
    }

    // Final size must match
    EXPECT_EQ(hm.size(), ref.size());
}

// Stress 3: repeated insert → erase → re-insert cycle
TEST(HashMapStress, InsertEraseCycle) {
    constexpr int ROUNDS = 20;
    constexpr int BATCH  = 200;
    HashMap<int, int> m;

    for (int r = 0; r < ROUNDS; ++r) {
        for (int i = 0; i < BATCH; ++i) m.insert(i, i + r);
        EXPECT_EQ(m.size(), static_cast<std::size_t>(BATCH));
        for (int i = 0; i < BATCH; ++i) m.erase(i);
        EXPECT_TRUE(m.empty());
    }
}

// Stress 4: iterator count always equals size after mixed ops
TEST(HashMapStress, IteratorCountAlwaysMatchesSize) {
    std::mt19937 rng(7);
    std::uniform_int_distribution<int> dist(0, 300);
    HashMap<int, int> m;

    for (int round = 0; round < 500; ++round) {
        int key = dist(rng);
        if (round % 3 == 0) {
            m.erase(key);
        } else {
            m.insert(key, key);
        }
        std::size_t count = 0;
        for ([[maybe_unused]] auto& p : m) ++count;
        EXPECT_EQ(count, m.size())
            << "iterator count != size after round " << round;
    }
}

// Stress 5: string keys, large volume
TEST(HashMapStress, LargeStringKeys) {
    constexpr int N = 2'000;
    HashMap<std::string, int> m;
    for (int i = 0; i < N; ++i) {
        m.insert("key_" + std::to_string(i), i);
    }
    EXPECT_EQ(m.size(), static_cast<std::size_t>(N));
    for (int i = 0; i < N; ++i) {
        ASSERT_NE(m.find("key_" + std::to_string(i)), nullptr);
        EXPECT_EQ(*m.find("key_" + std::to_string(i)), i);
    }
}