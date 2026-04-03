#ifndef __Vector_Hash__
#define __Vector_Hash__

#include <vector>
#include <utility>
#include <optional>
#include <type_traits>
#include <random>
#include <string>
#include <functional>

#define ALPHA 0.8

enum class HashType
{
    MurmurHash2,
    BasicHash
};

template<typename Tkey, typename Tvalue>
class Hash
{
private:
    struct Cell
    {
        std::pair<Tkey, Tvalue> data;
        char state = 'e'; //e -> empty, d -> deleted, s -> something here
        Cell() : state('e') {}
        Cell(const Tkey& key, const Tvalue& value) : data(key, value), state('s') {}
    };
    std::vector<Cell> table;
    uint32_t elementsNum = 0;
    //
    uint32_t hash(const Tkey& key, HashType hashType = HashType::MurmurHash2) const
    {
        if (hashType == HashType::MurmurHash2)
        {
            if constexpr(std::is_same_v<Tkey, std::string>)
                return MurMurHash2String(key);
            else
                return MurMurHash2(key);
        }
        if (hashType == HashType::BasicHash)
            return BasicHash(key);
    }
    uint32_t MurMurHash2(const Tkey& key) const
    {
        const uint32_t m = 0x5bd1e995;
        const uint32_t seed = 0x9747b28c;
        const int r = 24;
        uint32_t len = sizeof(Tkey);
        uint32_t h = seed ^ len;
        const unsigned char* data = reinterpret_cast<const unsigned char*>(&key);

        while (len >= 4)
        {
            uint32_t k = data[0];
            k |= data[1] << 8;
            k |= data[2] << 16;
            k |= data[3] << 24;
            k *= m;
            k ^= k >> r;
            k *= m;
            h *= m;
            h ^= k;
            data += 4;
            len -= 4;
        }

        switch (len)
        {
        case 3:
            h ^= data[2] << 16;
        case 2:
            h ^= data[1] << 8;
        case 1:
            h ^= data[0];
            h *= m;
        };

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h % static_cast<uint32_t>(table.size());
    }
    uint32_t MurMurHash2String(const std::string& key) const
    {
        const uint32_t m = 0x5bd1e995;
        const uint32_t seed = 0x9747b28c;
        const int r = 24;
        uint32_t len = static_cast<uint32_t>(key.size());
        uint32_t h = seed ^ len;
        const unsigned char* data = reinterpret_cast<const unsigned char*>(key.data());

        while (len >= 4) {
            uint32_t k = data[0] | (data[1] << 8) | (data[2] << 16) | (data[3] << 24);
            k *= m;
            k ^= k >> r;
            k *= m;
            h *= m;
            h ^= k;
            data += 4;
            len -= 4;
        }

        switch (len) {
        case 3: h ^= data[2] << 16; [[fallthrough]];
        case 2: h ^= data[1] << 8; [[fallthrough]];
        case 1: h ^= data[0];
            h *= m;
        }

        h ^= h >> 13;
        h *= m;
        h ^= h >> 15;

        return h % static_cast<uint32_t>(table.size());
    }
    uint32_t BasicHash(const Tkey& key) const
    {
        if (table.size() == 0)
            return 0;
        return std::hash<Tkey>{}(key) % static_cast<uint32_t>(table.size());
    }
    bool needRehash() const
    {
        return static_cast<double>(elementsNum) / table.size() >= ALPHA;
    }
    void rehash()
    {
        uint32_t newSize = table.size() * 2;
        std::vector<Cell> oldTable = std::move(table);
        table.clear();
        table.resize(newSize);
        for (auto& cell : table)
        {
            cell.state = 'e';
        }
        elementsNum = 0;
        for (uint32_t i = 0; i < oldTable.size(); ++i)
        {
            if (oldTable[i].state == 's')
            {
                insert(oldTable[i].data.first, oldTable[i].data.second);
            }
        }
    }
public:
    Hash(uint32_t initial_size = 16) : table(initial_size) { }
    size_t size() const
    {
        return elementsNum;
    }
    bool empty() const
    {
        return elementsNum == 0;
    }
    void clear()
    {
        for (auto& cell : table)
        {
            cell.state = 'e';
        }
        elementsNum = 0;
    }
    size_t capacity() const
    {
        return table.size();
    }
    class Iterator
    {
    private:
        std::vector<Cell>*  tablePtr;
        size_t currentIndex;
        void findNextValid()
        {
            while (currentIndex < tablePtr->size() &&
                (*tablePtr)[currentIndex].state != 's')
            {
                ++currentIndex;
            }
        }
    public:
        Iterator() : tablePtr(nullptr), currentIndex(0) {}
        Iterator(std::vector<Cell>* const table, size_t index) : tablePtr(table), currentIndex(index)
        {
            if (tablePtr && currentIndex < tablePtr->size())
            {
                findNextValid();
            }
        }
        std::pair<const Tkey, Tvalue> operator*() const
        {
            if (!tablePtr || currentIndex >= tablePtr->size())
            {
                throw std::out_of_range("Iterator out of range");
            }
            const auto& cell = (*tablePtr)[currentIndex];
            return { cell.data.first, cell.data.second };
        }
        auto operator->() const
        {
            if (!tablePtr || currentIndex >= tablePtr->size())
            {
                throw std::out_of_range("Iterator out of range");
            }
            return &(*tablePtr)[currentIndex].data;
        }
        Iterator& operator++()
        {
            if (tablePtr && currentIndex < tablePtr->size())
            {
                ++currentIndex;
                findNextValid();
            }
            return *this;
        }
        Iterator operator++(int)
        {
            Iterator temp = *this;
            ++(*this);
            return temp;
        }
        bool operator==(const Iterator& other) const
        {
            return tablePtr == other.tablePtr && currentIndex == other.currentIndex;
        }
        bool operator!=(const Iterator& other) const
        {
            return !(*this == other);
        }
    };
    Iterator begin()
    {
        return Iterator(&table, 0);
    }
    Iterator end()
    {
        return Iterator(&table, table.size());
    }
    Iterator insert(const Tkey& key, const Tvalue& value)
    {
        if (needRehash())
        {
            rehash();
        }
        uint32_t start = hash(key);
        uint32_t current = start;
        uint32_t attempt = 0;
        while (attempt < table.size())
        {
            if (table[current].state == 'e' || table[current].state == 'd')
            {
                table[current].data = std::make_pair(key, value);
                table[current].state = 's';
                elementsNum++;
                return Iterator(&table, current);
            }
            else if (table[current].state == 's' && table[current].data.first == key)
            {
                table[current].data.second = value;
                return Iterator(&table, current);
            }
            attempt++;
            current = (start + attempt) % static_cast<uint32_t>(table.size());
        }
        return end();
    }
    Iterator find(const Tkey& key)
    {
        uint32_t start = hash(key);
        uint32_t current = start;
        uint32_t attempt = 0;

        while (attempt < table.size())
        {
            if (table[current].state == 'e')
            {
                return end();
            }
            else if (table[current].state == 's' && table[current].data.first == key)
            {
                return Iterator(&table, current);
            }
            attempt++;
            current = (start + attempt) % static_cast<uint32_t>(table.size());
        }
        return end();
    }
    Iterator erase(const Tkey& key)
    {
        uint32_t start = hash(key);
        uint32_t current = start;
        uint32_t attempt = 0;
        while (attempt < table.size())
        {
            if (table[current].state == 'e')
            {
                return end();
            }
            else if (table[current].state == 's' && table[current].data.first == key)
            {
                table[current].state = 'd';
                elementsNum--;
                Iterator It(&table, current);
                return It;
            }
            attempt++;
            current = (start + attempt) % static_cast<uint32_t>(table.size());
        }
        return end();
    }
    Tvalue& operator[](const Tkey& key)
    {
        Iterator it = find(key);
        if (it != end())
        {
            return it->second;
        }
        else
        {
            auto result = insert(key, Tvalue());
            it = find(key);
            return it->second;
        }
    }
    Tvalue& at(const Tkey& key)
    {
        Iterator it = find(key);
        if (it == end())
        {
            throw std::out_of_range("Error_AtNotFound");
        }
        return it->second;
    }
};

#endif

