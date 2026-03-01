#include "SortedMap.h"

#include <chrono>
#include <iostream>

#include <gtest.h>

TEST(SortedMapTest, DefaultConstructorCreatesEmptyMap)
{
    SortedMap<int, int> sMap;

    EXPECT_TRUE(sMap.empty());
    EXPECT_EQ(sMap.size(), 0);
    //ADD_FAILURE();
}

TEST(SortedMapTest, ClearMakesMapEmpty)
{
    SortedMap<int, int> sMap;
    sMap.insert(1, 1);
    sMap.insert(2, 1);
    sMap.insert(3, 1);

    EXPECT_FALSE(sMap.empty());

    sMap.clear();

    EXPECT_TRUE(sMap.empty());
    EXPECT_EQ(sMap.size(), 0);
    //ADD_FAILURE();
}

TEST(SortedMapTest, MapWorksWithDifferentDataTypes)
{
    SortedMap<char, std::string> sMap;
    sMap.insert('a', "ski");
    sMap.insert('b', "bi");
    sMap.insert('c', "di");

    EXPECT_EQ(sMap['a'], "ski");
    EXPECT_EQ(sMap['b'], "bi");
    EXPECT_EQ(sMap['c'], "di");
    //ADD_FAILURE();
}

TEST(SortedMapTest, AtMethodThrowsExceptionForInvalidIndex)
{
    SortedMap<int, int> sMap;
    sMap.insert(1, 10);

    EXPECT_NO_THROW(sMap.at(1));
    EXPECT_ANY_THROW(sMap.at(2));
    EXPECT_ANY_THROW(sMap.at(10));
    //ADD_FAILURE();
}

TEST(SortedMapTest, ClearOnEmptyListDoesNothing)
{
    SortedMap<int, int> sMap;

    EXPECT_TRUE(sMap.empty());
    EXPECT_EQ(sMap.size(), 0);
    EXPECT_NO_THROW(sMap.clear());
    EXPECT_TRUE(sMap.empty());
    EXPECT_EQ(sMap.size(), 0);
    //ADD_FAILURE();
}

TEST(SortedMapTest, InsertReturnsIterToInsetredElement)
{
    SortedMap<int, int> sMap;
    auto it1 = sMap.insert(1, 10);
    EXPECT_EQ(it1, sMap.find(1));
    auto it2 = sMap.insert(2, 20);
    EXPECT_EQ(it2, sMap.find(2));
    //ADD_FAILURE();
}

TEST(SortedMapTest, EraseReturnsIterToNextElement)
{
    SortedMap<int, int> sMap;
    sMap.insert(1, 10);
    sMap.insert(2, 20);
    sMap.insert(3, 30);
    sMap.insert(4, 40);
    auto it1 = sMap.erase(2);
    EXPECT_EQ(it1, sMap.find(3));
    auto it2 = sMap.erase(3);
    EXPECT_EQ(it2, sMap.find(4));
    //ADD_FAILURE();
}

TEST(SortedMapTest, InsertToExistElemDontReplaceIt)
{
    SortedMap<int, int> sMap;
    sMap.insert(1, 10);
    EXPECT_EQ(sMap[1], 10);
    sMap.insert(1, 20);
    EXPECT_EQ(sMap[1], 10);
    //ADD_FAILURE();
}

TEST(SortedMapTest, EraseNonExistElemThrow)
{
    SortedMap<int, int> sMap;
    EXPECT_ANY_THROW(sMap.erase(1));
    //ADD_FAILURE();
}

TEST(SortedMapTest, OperatorReturnCorrectly)
{
    SortedMap<int, int> sMap;
    sMap.insert(1, 1);
    EXPECT_EQ(sMap[1], 1);
    //ADD_FAILURE();
}

TEST(SortedMapTest, OperatorInsertIfNotFound)
{
    SortedMap<int, int> sMap;
    sMap[1];
    EXPECT_NE(sMap.find(1), sMap.end());
    //ADD_FAILURE();
}

TEST(SortedMapTest, FindReturnsCorrectIterator)
{
    SortedMap<int, int> sMap;
    sMap.insert(1, 10);
    sMap.insert(3, 30);
    sMap.insert(5, 50);

    auto it = sMap.find(3);
    EXPECT_NE(it, sMap.end());
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 30);
    EXPECT_EQ(sMap.find(2), sMap.end());
    //ADD_FAILURE();
}

TEST(SortedMapTest, FindReturnsCorrectIterator)
{
    SortedMap<int, int> sMap;
    sMap.insert(1, 10);
    sMap.insert(3, 30);
    sMap.insert(5, 50);

    auto it = sMap.find(3);
    EXPECT_NE(it, sMap.end());
    EXPECT_EQ(it->first, 3);
    EXPECT_EQ(it->second, 30);
    EXPECT_EQ(sMap.find(2), sMap.end());
    //ADD_FAILURE();
}

TEST(SortedMapTest, IteratorWorksCorrectly1)
{
    SortedMap<int, int> sMap;
    sMap.insert(3, 3);
    sMap.insert(1, 1);
    sMap.insert(4, 4);
    sMap.insert(2, 2);
    std::vector<int> expectedKeys = {1, 2, 3, 4};
    std::vector<int> expectedValues = {1, 2, 2, 4};

    int index = 0;
    for (auto it = sMap.begin(); it != sMap.end(); ++it)
    {
        EXPECT_EQ(it->first, expectedKeys[index]);
        EXPECT_EQ(it->second, expectedValues[index]);
        index++;
    }
    EXPECT_EQ(index, 4);
    //ADD_FAILURE();
}

TEST(SortedMapTest, IteratorWorksCorrectly2)
{
    SortedMap<int, int> sMap;
    sMap.insert(3, 3);
    sMap.insert(1, 1);
    sMap.insert(4, 4);
    sMap.insert(2, 2);
    //->
    auto it = sMap.begin();
    EXPECT_EQ(it->first, 1);
    EXPECT_EQ(it->second, 1);
    //++
    it = sMap.begin();
    auto oldIt = it++;
    EXPECT_EQ(oldIt->first, 1);
    EXPECT_EQ(it->first, 2);
    //== & !=
    auto it1 = sMap.begin();
    auto it2 = sMap.begin();
    auto it3 = sMap.end();
    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 != it2);
    EXPECT_TRUE(it1 != it3);
    //*
    EXPECT_EQ((*it1).first, 1);
    EXPECT_EQ((*it1).second, 1);
    //ADD_FAILURE();
}

TEST(SortedMapTest, StressTest)
{
    SortedMap<int, int> sMap;
    for (size_t i = 0; i < 1e+7; i++)
    {
        sMap.insert(i, i);
    }
    auto start = std::chrono::high_resolution_clock::now();
    //
    for (size_t i = 0; i < 1e+7; i++)
    {
        sMap.find(i);
    }
    //
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(end - start);
    std::cout << "seconds:" << duration.count() << std::endl;
    //ADD_FAILURE();
}


