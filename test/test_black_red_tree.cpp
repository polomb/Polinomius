#include "BlackRedTree.h"

#include <chrono>
#include <iostream>

#include <gtest.h>

TEST(BlackRedTree, DefaultConstructorCreatesEmptyTree)
{
    BlackRedTree<int, int> tree;

    EXPECT_TRUE(tree.empty());
    //ADD_FAILURE();
}

TEST(BlackRedTree, ClearMakesTreeEmpty)
{
    BlackRedTree<int, int> tree;
    tree.insert(1, 10);
    tree.insert(2, 20);
    tree.insert(3, 30);

    EXPECT_FALSE(tree.empty());

    tree.clear();

    EXPECT_TRUE(tree.empty());
    //ADD_FAILURE();
}

TEST(BlackRedTree, TreeWorksWithDifferentDataTypes)
{
    BlackRedTree<char, std::string> tree;
    tree.insert('a', "ski");
    tree.insert('b', "bi");
    tree.insert('c', "di");

    EXPECT_EQ(tree['a'], "ski");
    EXPECT_EQ(tree['b'], "bi");
    EXPECT_EQ(tree['c'], "di");
    //ADD_FAILURE();
}

TEST(BlackRedTree, AtMethodThrowsExceptionForInvalidIndex)
{
    BlackRedTree<int, int> tree;
    tree.insert(1, 10);

    EXPECT_NO_THROW(tree.at(1));
    EXPECT_ANY_THROW(tree.at(2));
    EXPECT_ANY_THROW(tree.at(10));
    //ADD_FAILURE();
}

TEST(BlackRedTree, ClearOnEmptyTreeDoesNothing)
{
    BlackRedTree<int, int> tree;

    EXPECT_TRUE(tree.empty());
    EXPECT_NO_THROW(tree.clear());
    EXPECT_TRUE(tree.empty());
    //ADD_FAILURE();
}

TEST(BlackRedTree, InsertReturnsIterToInsertedElement)
{
    BlackRedTree<int, int> tree;
    auto it1 = tree.insert(1, 10);
    EXPECT_EQ(it1, tree.find(1));
    auto it2 = tree.insert(2, 20);
    EXPECT_EQ(it2, tree.find(2));
    //ADD_FAILURE();
}

TEST(BlackRedTree, InsertToExistElemDontReplaceIt)
{
    BlackRedTree<int, int> tree;
    tree.insert(1, 10);
    EXPECT_EQ(tree[1], 10);
    tree.insert(1, 20);
    EXPECT_EQ(tree[1], 10);
    //ADD_FAILURE();
}

TEST(BlackRedTree, EraseNonExistElemThrow)
{
    BlackRedTree<int, int> tree;
    EXPECT_ANY_THROW(tree.erase(1));
    //ADD_FAILURE();
}

TEST(BlackRedTree, OperatorReturnCorrectly)
{
    BlackRedTree<int, int> tree;
    tree.insert(1, 1);
    EXPECT_EQ(tree[1], 1);
    //ADD_FAILURE();
}

TEST(BlackRedTree, OperatorInsertIfNotFound)
{
    BlackRedTree<int, int> tree;
    tree[1];
    EXPECT_NE(tree.find(1), tree.end());
    //ADD_FAILURE();
}

TEST(BlackRedTree, FindReturnsCorrectIterator)
{
    BlackRedTree<int, int> tree;
    tree.insert(1, 10);
    tree.insert(3, 30);
    tree.insert(5, 50);

    auto it = tree.find(3);
    EXPECT_NE(it, tree.end());
    EXPECT_EQ(it.iter->key, 3);
    EXPECT_EQ(it.iter->value, 30);
    EXPECT_EQ(tree.find(2), tree.end());
    //ADD_FAILURE();
}

TEST(BlackRedTree, IteratorWorksCorrectly)
{
    BlackRedTree<int, int> tree;
    tree.insert(3, 3);
    tree.insert(1, 1);
    tree.insert(4, 4);
    tree.insert(2, 2);
    //->
    auto it = tree.begin();
    EXPECT_EQ(it.iter->key, 1);
    EXPECT_EQ(it.iter->value, 1);
    //++
    it = tree.begin();
    auto oldIt = it++;
    EXPECT_EQ(oldIt.iter->key, 1);
    EXPECT_EQ(it.iter->value, 2);
    //== & !=
    auto it1 = tree.begin();
    auto it2 = tree.begin();
    auto it3 = tree.end();
    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 != it2);
    EXPECT_TRUE(it1 != it3);
    //ADD_FAILURE();
}

TEST(BlackRedTree, IteratorWorksCorrectl)
{
    BlackRedTree<int, int> tree;
    for (size_t i = 0; i < 10; i++)
    {
        tree[i] = i;
    }
    int j = 0;
    for (auto it = tree.begin(); it != tree.end(); it++, j++)
    {
        EXPECT_EQ(*it, j);
    }
    //ADD_FAILURE();
}

TEST(BlackRedTree, StressTest)
{
    BlackRedTree<int, int> tree;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 1e+7; i++)
    {
        tree.insert(i, i);
    }
    std::cout << tree.height();
    for (size_t i = 0; i < 1e+7; i++)
    {
        tree.erase(i);
    }
    tree.print();
    //
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "seconds:" << duration.count() << std::endl;
    //ADD_FAILURE();
}


