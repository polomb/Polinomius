#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>
#include <set>
#include <string>
#include <vector>

#include <gtest.h>

#include "AVLTree.h"

// ============================================================
//  HOW TO READ THIS FILE
// ============================================================
//  1) AVLBasicTest      - базовые операции: empty/size/find/insert/clear.
//  2) AVLRotationTest   - проверка всех 4 сценариев балансировки (LL/RR/LR/RL).
//  3) AVLDeleteTest     - сценарии удаления: лист, 1/2 ребёнка, несуществующий ключ.
//  4) AVLIteratorTest   - корректность in-order итерации и range-based for.
//  5) AVLStressTest     - большие объёмы данных и случайные последовательности операций.
//  6) AVLEdgeCaseTest   - граничные кейсы: 1-2 элемента, отрицательные, string-ключи.
//
//  Naming pattern:
//  <SuiteName>.<Scenario> => где SuiteName показывает "область",
//  а Scenario - конкретно "что именно проверяется".

template <typename Key>
std::vector<Key> toVector(const AVLTree<Key>& tree) {
  std::vector<Key> result;
  for (const auto& k : tree) {
    result.push_back(k);
  }
  return result;
}

template <typename T>
bool isSorted(const std::vector<T>& v) {
  return std::is_sorted(v.begin(), v.end());
}

class AVLBasicTest : public ::testing::Test {
 protected:
  AVLTree<int> tree;
};

// Пустое дерево сразу после конструктора.
TEST_F(AVLBasicTest, EmptyTree) {
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(tree.size(), 0);
  EXPECT_EQ(tree.height(), 0);
  EXPECT_FALSE(tree.find(42));
}

// Одна вставка: дерево перестаёт быть пустым, высота 1, ключ находится.
TEST_F(AVLBasicTest, InsertSingle) {
  tree.insert(10);
  EXPECT_FALSE(tree.empty());
  EXPECT_EQ(tree.size(), 1);
  EXPECT_TRUE(tree.find(10));
  EXPECT_FALSE(tree.find(99));
  EXPECT_EQ(tree.height(), 1);
  EXPECT_TRUE(tree.isValid());
}

// Набор вставок: проверяем, что все ожидаемые ключи есть, лишних нет.
TEST_F(AVLBasicTest, InsertMultiple) {
  for (int x : {5, 3, 7, 1, 4, 6, 9}) {
    tree.insert(x);
  }
  EXPECT_EQ(tree.size(), 7);
  for (int x : {1, 3, 4, 5, 6, 7, 9}) {
    EXPECT_TRUE(tree.find(x));
  }
  EXPECT_FALSE(tree.find(2));
  EXPECT_FALSE(tree.find(8));
  EXPECT_TRUE(tree.isValid());
}

// Дубликаты запрещены: insert должен бросать исключение и не менять size.
TEST_F(AVLBasicTest, DuplicateKeyThrows) {
  tree.insert(5);
  EXPECT_THROW(tree.insert(5), std::runtime_error);
  EXPECT_EQ(tree.size(), 1);
}

// clear полностью очищает дерево.
TEST_F(AVLBasicTest, Clear) {
  for (int x : {1, 2, 3, 4, 5}) {
    tree.insert(x);
  }
  tree.clear();
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(tree.size(), 0);
  EXPECT_FALSE(tree.find(1));
}

class AVLRotationTest : public ::testing::Test {
 protected:
  AVLTree<int> tree;
};

// LL-кейс (правый поворот).
TEST_F(AVLRotationTest, LL_Case_RightRotation) {
  tree.insert(30);
  tree.insert(20);
  tree.insert(10);

  EXPECT_TRUE(tree.isValid());
  EXPECT_EQ(tree.height(), 2);

  auto root = tree.getRoot();
  EXPECT_EQ(root->key, 20);
  EXPECT_EQ(root->left->key, 10);
  EXPECT_EQ(root->right->key, 30);
}

// RR-кейс (левый поворот).
TEST_F(AVLRotationTest, RR_Case_LeftRotation) {
  tree.insert(10);
  tree.insert(20);
  tree.insert(30);

  EXPECT_TRUE(tree.isValid());
  EXPECT_EQ(tree.height(), 2);

  auto root = tree.getRoot();
  EXPECT_EQ(root->key, 20);
  EXPECT_EQ(root->left->key, 10);
  EXPECT_EQ(root->right->key, 30);
}

// LR-кейс (левый + правый поворот).
TEST_F(AVLRotationTest, LR_Case_DoubleRotation) {
  tree.insert(30);
  tree.insert(10);
  tree.insert(20);

  EXPECT_TRUE(tree.isValid());
  EXPECT_EQ(tree.height(), 2);

  auto root = tree.getRoot();
  EXPECT_EQ(root->key, 20);
  EXPECT_EQ(root->left->key, 10);
  EXPECT_EQ(root->right->key, 30);
}

// RL-кейс (правый + левый поворот).
TEST_F(AVLRotationTest, RL_Case_DoubleRotation) {
  tree.insert(10);
  tree.insert(30);
  tree.insert(20);

  EXPECT_TRUE(tree.isValid());
  EXPECT_EQ(tree.height(), 2);

  auto root = tree.getRoot();
  EXPECT_EQ(root->key, 20);
  EXPECT_EQ(root->left->key, 10);
  EXPECT_EQ(root->right->key, 30);
}

// Рост высоты должен оставаться логарифмическим.
TEST_F(AVLRotationTest, HeightIsLogarithmic) {
  for (int i = 1; i <= 127; i++) {
    tree.insert(i);
  }
  EXPECT_LE(tree.height(), 8);
  EXPECT_TRUE(tree.isValid());
}

class AVLDeleteTest : public ::testing::Test {
 protected:
  AVLTree<int> tree;

  void SetUp() override {
    for (int x : {50, 30, 70, 20, 40, 60, 80}) {
      tree.insert(x);
    }
  }
};

// Удаление листа.
TEST_F(AVLDeleteTest, DeleteLeaf) {
  tree.erase(20);
  EXPECT_FALSE(tree.find(20));
  EXPECT_EQ(tree.size(), 6);
  EXPECT_TRUE(tree.isValid());
}

// Удаление узла с одним ребёнком.
TEST_F(AVLDeleteTest, DeleteNodeWithOneChild) {
  tree.erase(20);
  tree.erase(30);
  EXPECT_FALSE(tree.find(30));
  EXPECT_TRUE(tree.find(40));
  EXPECT_TRUE(tree.isValid());
}

// Удаление узла с двумя детьми (через in-order successor).
TEST_F(AVLDeleteTest, DeleteNodeWithTwoChildren) {
  tree.erase(50);
  EXPECT_FALSE(tree.find(50));
  EXPECT_TRUE(tree.find(60));
  EXPECT_EQ(tree.size(), 6);
  EXPECT_TRUE(tree.isValid());
  EXPECT_TRUE(isSorted(toVector(tree)));
}

// Полная очистка дерева удалениями.
TEST_F(AVLDeleteTest, DeleteAll) {
  std::vector<int> keys = {50, 30, 70, 20, 40, 60, 80};
  for (int k : keys) {
    tree.erase(k);
  }
  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(tree.size(), 0);
}

// erase несуществующего ключа не должен ломать структуру и size.
TEST_F(AVLDeleteTest, DeleteNonExistent) {
  int sizeBefore = tree.size();
  tree.erase(999);
  EXPECT_EQ(tree.size(), sizeBefore);
  EXPECT_TRUE(tree.isValid());
}

// После каждой операции удаления AVL-инвариант остаётся валиден.
TEST_F(AVLDeleteTest, AVLPropertyAfterDelete) {
  for (int x : {50, 30, 70, 20, 40, 60, 80}) {
    tree.erase(x);
    EXPECT_TRUE(tree.isValid());
  }
}

class AVLIteratorTest : public ::testing::Test {
 protected:
  AVLTree<int> tree;
};

// Итератор обязан выдавать ключи по возрастанию (in-order).
TEST_F(AVLIteratorTest, InOrderTraversal) {
  for (int x : {5, 3, 7, 1, 4, 6, 9, 2, 8}) {
    tree.insert(x);
  }
  auto v = toVector(tree);
  EXPECT_TRUE(isSorted(v));
  EXPECT_EQ(v.size(), 9u);
}

// begin == end на пустом дереве.
TEST_F(AVLIteratorTest, EmptyTreeIteration) {
  auto v = toVector(tree);
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(tree.begin(), tree.end());
}

// Итерация по одному элементу.
TEST_F(AVLIteratorTest, SingleElementIteration) {
  tree.insert(42);
  auto it = tree.begin();
  EXPECT_EQ(*it, 42);
  ++it;
  EXPECT_EQ(it, tree.end());
}

// Проверка range-based for.
TEST_F(AVLIteratorTest, RangeBasedFor) {
  for (int x : {10, 5, 15, 3, 7}) {
    tree.insert(x);
  }
  std::vector<int> result;
  for (const int& k : tree) {
    result.push_back(k);
  }
  EXPECT_EQ(result, std::vector<int>({3, 5, 7, 10, 15}));
}

// После вставки обход остаётся корректным.
TEST_F(AVLIteratorTest, IteratorAfterInsert) {
  tree.insert(10);
  tree.insert(5);
  auto v1 = toVector(tree);
  tree.insert(15);
  auto v2 = toVector(tree);
  EXPECT_EQ(v2.size(), v1.size() + 1);
  EXPECT_TRUE(isSorted(v2));
}

// После удаления обход остаётся корректным.
TEST_F(AVLIteratorTest, IteratorAfterDelete) {
  for (int x : {10, 5, 15}) {
    tree.insert(x);
  }
  tree.erase(5);
  auto v = toVector(tree);
  EXPECT_EQ(v, std::vector<int>({10, 15}));
}

class AVLStressTest : public ::testing::Test {};

// 10k случайных вставок + поиск каждого элемента.
TEST_F(AVLStressTest, RandomInsert10k) {
  AVLTree<int> tree;
  std::vector<int> data(10000);
  std::iota(data.begin(), data.end(), 0);
  std::mt19937 rng(42);
  std::shuffle(data.begin(), data.end(), rng);

  for (int x : data) {
    tree.insert(x);
  }

  EXPECT_EQ(tree.size(), 10000);
  EXPECT_TRUE(tree.isValid());
  EXPECT_LE(tree.height(), 20);

  for (int x : data) {
    EXPECT_TRUE(tree.find(x));
  }
}

// Вставка уже отсортированных данных (худший кейс для обычного BST).
TEST_F(AVLStressTest, SortedInsert10k) {
  AVLTree<int> tree;
  for (int i = 0; i < 10000; i++) {
    tree.insert(i);
  }

  EXPECT_EQ(tree.size(), 10000);
  EXPECT_TRUE(tree.isValid());
  EXPECT_LE(tree.height(), 20);
}

// Вставка данных в обратном порядке.
TEST_F(AVLStressTest, ReverseSortedInsert10k) {
  AVLTree<int> tree;
  for (int i = 9999; i >= 0; i--) {
    tree.insert(i);
  }

  EXPECT_EQ(tree.size(), 10000);
  EXPECT_TRUE(tree.isValid());
  EXPECT_LE(tree.height(), 20);
}

// Чередование insert/erase c эталоном std::set.
TEST_F(AVLStressTest, InterleavedInsertDelete) {
  AVLTree<int> tree;
  std::set<int> reference;
  std::mt19937 rng(123);
  std::uniform_int_distribution<int> dist(0, 1000);

  for (int i = 0; i < 5000; i++) {
    int key = dist(rng);
    if (reference.count(key) == 0) {
      tree.insert(key);
      reference.insert(key);
    } else {
      tree.erase(key);
      reference.erase(key);
    }

    if (i % 100 == 0) {
      ASSERT_TRUE(tree.isValid()) << "AVL broken after operation " << i;
      ASSERT_EQ(tree.size(), static_cast<int>(reference.size()));
    }
  }

  auto treeVec = toVector(tree);
  std::vector<int> refVec(reference.begin(), reference.end());
  EXPECT_EQ(treeVec, refVec);
}

// Удаление всех 5k ключей в случайном порядке.
TEST_F(AVLStressTest, DeleteAll5k) {
  AVLTree<int> tree;
  std::vector<int> data(5000);
  std::iota(data.begin(), data.end(), 0);
  for (int x : data) {
    tree.insert(x);
  }

  std::mt19937 rng(7);
  std::shuffle(data.begin(), data.end(), rng);

  for (int x : data) {
    tree.erase(x);
    ASSERT_TRUE(tree.isValid());
  }
  EXPECT_TRUE(tree.empty());
}

// Грубая проверка производительности на 100k insert/find/erase.
TEST_F(AVLStressTest, Performance100k) {
  AVLTree<int> tree;
  const int N = 100000;
  std::vector<int> data(N);
  std::iota(data.begin(), data.end(), 1);
  std::mt19937 rng(999);
  std::shuffle(data.begin(), data.end(), rng);

  auto t0 = std::chrono::high_resolution_clock::now();
  for (int x : data) {
    tree.insert(x);
  }
  auto t1 = std::chrono::high_resolution_clock::now();

  EXPECT_EQ(tree.size(), N);
  EXPECT_TRUE(tree.isValid());

  for (int x : data) {
    EXPECT_TRUE(tree.find(x));
  }
  auto t2 = std::chrono::high_resolution_clock::now();

  for (int x : data) {
    tree.erase(x);
  }
  auto t3 = std::chrono::high_resolution_clock::now();

  EXPECT_TRUE(tree.empty());

  auto insertMs =
      std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
  auto searchMs =
      std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
  auto deleteMs =
      std::chrono::duration_cast<std::chrono::milliseconds>(t3 - t2).count();

  std::cout << "\n[Performance 100k]\n"
            << "  Insert: " << insertMs << " ms\n"
            << "  Search: " << searchMs << " ms\n"
            << "  Delete: " << deleteMs << " ms\n";

  EXPECT_LT(insertMs, 5000);
  EXPECT_LT(searchMs, 5000);
  EXPECT_LT(deleteMs, 5000);
}

// Длительная рандомизированная сверка с поведением std::set.
TEST_F(AVLStressTest, CompareWithStdSet) {
  AVLTree<int> tree;
  std::set<int> stdSet;
  std::mt19937 rng(42);
  std::uniform_int_distribution<int> dist(0, 500);

  for (int i = 0; i < 10000; i++) {
    int key = dist(rng);
    int action = rng() % 3;

    if (action <= 1) {
      if (!stdSet.count(key)) {
        tree.insert(key);
        stdSet.insert(key);
      }
    } else {
      tree.erase(key);
      stdSet.erase(key);
    }
  }

  auto treeVec = toVector(tree);
  std::vector<int> setVec(stdSet.begin(), stdSet.end());
  EXPECT_EQ(treeVec, setVec);
  EXPECT_TRUE(tree.isValid());
}

// Массовое удаление по возрастанию (50k).
TEST_F(AVLStressTest, AscendingErase50k) {
  AVLTree<int> tree;
  constexpr int kN = 50000;
  for (int i = 0; i < kN; ++i) {
    tree.insert(i);
  }

  for (int i = 0; i < kN; ++i) {
    tree.erase(i);
    if (i % 2500 == 0) {
      ASSERT_TRUE(tree.isValid());
    }
  }

  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(tree.size(), 0);
}

// Массовое удаление по убыванию (50k).
TEST_F(AVLStressTest, DescendingErase50k) {
  AVLTree<int> tree;
  constexpr int kN = 50000;
  for (int i = 0; i < kN; ++i) {
    tree.insert(i);
  }

  for (int i = kN - 1; i >= 0; --i) {
    tree.erase(i);
    if (i % 2500 == 0) {
      ASSERT_TRUE(tree.isValid());
    }
  }

  EXPECT_TRUE(tree.empty());
  EXPECT_EQ(tree.size(), 0);
}

// 200k случайных операций, регулярная сверка find/size/содержимого со std::set.
TEST_F(AVLStressTest, RandomOps200kAgainstSet) {
  AVLTree<int> tree;
  std::set<int> ref;
  std::mt19937 rng(2026);
  std::uniform_int_distribution<int> keyDist(0, 2000000);
  std::uniform_int_distribution<int> opDist(0, 3);  // 0/1 insert, 2 erase, 3 find

  constexpr int kOps = 400000;
  for (int i = 1; i <= kOps; ++i) {
    int key = keyDist(rng);
    int op = opDist(rng);

    if (op <= 1) {
      bool exists = ref.count(key) > 0;
      if (!exists) {
        tree.insert(key);
        ref.insert(key);
      }
    } else if (op == 2) {
      tree.erase(key);
      ref.erase(key);
    } else {
      EXPECT_EQ(tree.find(key), ref.count(key) > 0);
    }

    if (i % 5000 == 0) {
      ASSERT_TRUE(tree.isValid()) << "invalid after op " << i;
      ASSERT_EQ(tree.size(), static_cast<int>(ref.size()));
    }
  }

  auto treeVec = toVector(tree);
  std::vector<int> refVec(ref.begin(), ref.end());
  EXPECT_EQ(treeVec, refVec);
}

// Проверка, что итерация корректна после больших пачек вставок и удалений.
TEST_F(AVLStressTest, IteratorStabilityAfterBulkMutations) {
  AVLTree<int> tree;
  std::vector<int> data(30000);
  std::iota(data.begin(), data.end(), 0);
  std::mt19937 rng(77);
  std::shuffle(data.begin(), data.end(), rng);

  for (int x : data) {
    tree.insert(x);
  }

  std::shuffle(data.begin(), data.end(), rng);
  for (int i = 0; i < 15000; ++i) {
    tree.erase(data[i]);
  }

  auto out = toVector(tree);
  EXPECT_TRUE(isSorted(out));
  EXPECT_EQ(out.size(), 15000u);
  EXPECT_TRUE(tree.isValid());
}

class AVLEdgeCaseTest : public ::testing::Test {};

// Базовый edge case на одном элементе.
TEST_F(AVLEdgeCaseTest, SingleElement) {
  AVLTree<int> tree;
  tree.insert(42);
  EXPECT_TRUE(tree.find(42));
  tree.erase(42);
  EXPECT_TRUE(tree.empty());
  EXPECT_TRUE(tree.isValid());
}

// Edge case на двух элементах.
TEST_F(AVLEdgeCaseTest, TwoElements) {
  AVLTree<int> tree;
  tree.insert(1);
  tree.insert(2);
  EXPECT_EQ(tree.size(), 2);
  tree.erase(1);
  EXPECT_EQ(tree.size(), 1);
  EXPECT_TRUE(tree.find(2));
  EXPECT_TRUE(tree.isValid());
}

// Отрицательные ключи.
TEST_F(AVLEdgeCaseTest, NegativeKeys) {
  AVLTree<int> tree;
  for (int x : {-5, -3, -7, -1, -9}) {
    tree.insert(x);
  }
  EXPECT_TRUE(tree.isValid());
  auto v = toVector(tree);
  EXPECT_TRUE(isSorted(v));
  EXPECT_EQ(v.front(), -9);
  EXPECT_EQ(v.back(), -1);
}

// Смешанные отрицательные/положительные ключи.
TEST_F(AVLEdgeCaseTest, MixedNegativePositive) {
  AVLTree<int> tree;
  for (int x : {-5, 0, 5, -10, 10}) {
    tree.insert(x);
  }
  EXPECT_TRUE(tree.isValid());
  auto v = toVector(tree);
  EXPECT_EQ(v, std::vector<int>({-10, -5, 0, 5, 10}));
}

// Проверка шаблона на string-ключах.
TEST_F(AVLEdgeCaseTest, StringKeys) {
  AVLTree<std::string> tree;
  for (const auto& s : {"banana", "apple", "cherry", "date"}) {
    tree.insert(s);
  }
  EXPECT_TRUE(tree.isValid());
  auto v = toVector(tree);
  EXPECT_TRUE(isSorted(v));
  EXPECT_EQ(v[0], "apple");
}
