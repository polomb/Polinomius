#include "Hash.h"

#include <chrono>
#include <iostream>

#include <gtest.h>

TEST(HashTest, DefaultConstructorCreatesEmptyHash)
{
    Hash<int, int> hash;

    EXPECT_TRUE(hash.empty());
    //ADD_FAILURE();
}

TEST(HashTest, ClearMakesHashEmpty)
{
    Hash<int, int> hash;
    hash.insert(1, 10);
    hash.insert(2, 20);
    hash.insert(3, 30);

    EXPECT_FALSE(hash.empty());

    hash.clear();

    EXPECT_TRUE(hash.empty());
    //ADD_FAILURE();
}

TEST(HashTest, OperatorWorksCorrectly)
{
    Hash<int, int> hash;
    hash.insert(1, 10);
    hash.insert(2, 20);
    hash.insert(3, 30);

    EXPECT_EQ(hash[1],10);
    //ADD_FAILURE();
}

TEST(HashTest, HashWorksWithDifferentDataTypes)
{
    Hash<char, std::string> hash;
    hash.insert('a', "ski");
    hash.insert('b', "bi");
    hash.insert('c', "di");

    EXPECT_EQ(hash.find('a')->second, "ski");
    EXPECT_EQ(hash.find('b')->second, "bi");
    EXPECT_EQ(hash.find('c')->second, "di");
    //ADD_FAILURE();
}

TEST(HashTest, ClearOnEmptyHashDoesNothing)
{
    Hash<int, int> hash;

    EXPECT_TRUE(hash.empty());
    EXPECT_NO_THROW(hash.clear());
    EXPECT_TRUE(hash.empty());
    //ADD_FAILURE();
}

TEST(HashTest, InsertReturnsIterToInsertedElement)
{
    Hash<int, int> hash;
    auto it1 = hash.insert(1, 10);
    EXPECT_EQ(it1, hash.find(1));
    auto it2 = hash.insert(2, 20);
    EXPECT_EQ(it2, hash.find(2));
    //ADD_FAILURE();
}

TEST(HashTest, InsertToExistElemReplaceIt)
{
    Hash<int, int> hash;
    hash.insert(1, 10);
    EXPECT_EQ(hash.find(1)->second, 10);
    hash.insert(1, 20);
    EXPECT_EQ(hash.find(1)->second, 20);
    //ADD_FAILURE();
}

TEST(HashTest, EraseNonExistElemGetsEnd)
{
    Hash<int, int> hash;
    EXPECT_EQ(hash.find(1), hash.end());
    //ADD_FAILURE();
}

TEST(HashTest, OperatorInsertIfNotFound)
{
    Hash<int, int> hash;
    hash.insert(1,1);
    EXPECT_NE(hash.find(1), hash.end());
    //ADD_FAILURE();
}

TEST(HashTest, FindReturnsCorrectIterator)
{
    Hash<int, int> hash;
    hash.insert(1, 10);
    hash.insert(3, 30);
    hash.insert(5, 50);

    auto it = hash.find(3);
    EXPECT_NE(it, hash.end());
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 30);
    EXPECT_EQ(hash.find(2), hash.end());
    //ADD_FAILURE();
}

TEST(HashTest, IteratorWorksCorrectlyWithMurMurHash2) // Works for MurMurHash2 only
{
    Hash<int, int> hash;
    hash.insert(3, 3);
    hash.insert(1, 1);
    hash.insert(4, 4);
    hash.insert(2, 2);
    //->
    auto it = hash.begin();
    EXPECT_EQ(it->first, 4);
    //++
    it = hash.begin();
    auto oldIt = it++;
    EXPECT_EQ(oldIt->first, 4);
    EXPECT_EQ(it->first, 1);
    //== & !=
    auto it1 = hash.begin();
    auto it2 = hash.begin();
    auto it3 = hash.end();
    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 != it2);
    EXPECT_TRUE(it1 != it3);
    //ADD_FAILURE();
}

TEST(HashTest, WorksWithString)
{
    Hash<std::string, int> hash;
    hash.insert("apple", 10);
    hash.insert("pen", 30);

    EXPECT_EQ(hash.find("apple")->second, 10);
    EXPECT_EQ(hash.find("pen")->second, 30);
    //ADD_FAILURE();
}

TEST(HashTest, StressTest)
{
    Hash<int, int> hash;
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 1e+7; i++)
    {
        hash.insert(i, i);
    }
    for (size_t i = 0; i < 1e+7; i++)
    {
        hash.erase(i);
    }
    //
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "seconds:" << duration.count() << std::endl;
    //ADD_FAILURE();
}

TEST(HashTest, CanBeBrokenTestForBasicHash) // Works for BasicHash only
{
    Hash<int, int> hash;
    auto start1 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 2 * 1e+4; i++)
    {
        hash.insert(i, i);
    }
    for (size_t i = 0; i < 2 * 1e+4; i++)
    {
        hash.erase(i);
    }
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = std::chrono::duration_cast<std::chrono::seconds>(end1 - start1);
    std::cout << "seconds:" << duration1.count() << std::endl;
    std::cout << hash.capacity() << '\n';
    auto start2 = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < 2 * 1e+4; i++)
    {
        hash.insert(i * hash.capacity(), 1);
    }
    for (size_t i = 0; i < 2 * 1e+4; i++)
    {
        hash.erase(i * hash.capacity());
    }
    auto end2 = std::chrono::high_resolution_clock::now();
    auto duration2 = std::chrono::duration_cast<std::chrono::seconds>(end2 - start2);
    std::cout << "seconds:" << duration2.count() << std::endl;
    //
    
    //ADD_FAILURE();
}

