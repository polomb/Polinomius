#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <cstddef>
#include <functional>
#include <stdexcept>
#include <utility>

#include "ForwardList.h"

template <typename Key, typename Value, typename Hash = std::hash<Key>>
class HashMap {
public:
    using Pair = std::pair<Key, Value>;

private:
    static constexpr std::size_t DEFAULT_BUCKET_COUNT = 16;//начальное количество бакетов
    static constexpr float MAX_LOAD_FACTOR = 0.75f;//степень загруженности

    ForwardListV<Pair>* buckets = nullptr;
    std::size_t bucketCount = 0;
    std::size_t elementCount = 0;
    Hash hasher;

    std::size_t bucketIndex(const Key& key, std::size_t count) const {
        return hasher(key) % count;
    }

    void rehash(std::size_t newCount) {
        ForwardListV<Pair>* newBuckets = new ForwardListV<Pair>[newCount];

        for (std::size_t i = 0; i < bucketCount; ++i) {
            for (auto it = buckets[i].begin(); it != buckets[i].end(); ++it) {
                std::size_t idx = hasher(it->first) % newCount;
                newBuckets[idx].push_front(*it);
            }
        }

        delete[] buckets;
        buckets = newBuckets;
        bucketCount = newCount;
    }

public:

    explicit HashMap(std::size_t count = DEFAULT_BUCKET_COUNT, const Hash& hash = Hash{})
        : buckets(new ForwardListV<Pair>[count]), bucketCount(count), elementCount(0), hasher(hash) {}

    HashMap(const HashMap& other)
        : buckets(new ForwardListV<Pair>[other.bucketCount]),
          bucketCount(other.bucketCount),
          elementCount(other.elementCount),
          hasher(other.hasher) {
        for (std::size_t i = 0; i < bucketCount; ++i) {
            buckets[i] = other.buckets[i];
        }
    }

    HashMap& operator=(const HashMap& other) {
        if (this == &other) return *this;
        delete[] buckets;
        bucketCount = other.bucketCount;
        elementCount = other.elementCount;
        hasher = other.hasher;
        buckets = new ForwardListV<Pair>[bucketCount];
        for (std::size_t i = 0; i < bucketCount; ++i) {
            buckets[i] = other.buckets[i];
        }
        return *this;
    }

    HashMap(HashMap&& other) noexcept
        : buckets(other.buckets),
          bucketCount(other.bucketCount),
          elementCount(other.elementCount),
          hasher(std::move(other.hasher)) {
        other.buckets = nullptr;
        other.bucketCount = 0;
        other.elementCount = 0;
    }

    HashMap& operator=(HashMap&& other) noexcept {
        if (this == &other) return *this;
        delete[] buckets;
        buckets = other.buckets;
        bucketCount = other.bucketCount;
        elementCount = other.elementCount;
        hasher = std::move(other.hasher);
        other.buckets = nullptr;
        other.bucketCount = 0;
        other.elementCount = 0;
        return *this;
    }

    ~HashMap() { delete[] buckets; }


    bool empty() const noexcept { return elementCount == 0; }
    std::size_t size() const noexcept { return elementCount; }
    std::size_t bucket_count() const noexcept { return bucketCount; }
    float load_factor() const noexcept {
        return bucketCount == 0 ? 0.f : static_cast<float>(elementCount) / bucketCount;
    }

    Value* find_val(const Key& key) const {
        if (bucketCount == 0) return nullptr;
        std::size_t idx = bucketIndex(key, bucketCount);
        for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it) {
            if (it->first == key) return &it->second;
        }
        return nullptr;
    }

    bool contains(const Key& key) const { return find_val(key) != nullptr; }

    Value& at(const Key& key) {
        Value* val = find_val(key);
        if (!val) throw std::out_of_range("HashMap::at — key not found");
        return *val;
    }

    const Value& at(const Key& key) const {
        const Value* val = find_val(key);
        if (!val) throw std::out_of_range("HashMap::at — key not found");
        return *val;
    }

    Value& operator[](const Key& key) {
        Value* val = find_val(key);
        if (val) return *val;
        insert(key, Value{});
        return *find_val(key);
    }

    bool insert(const Key& key, const Value& value) {
        if (load_factor() >= MAX_LOAD_FACTOR) {
            rehash(bucketCount * 2);
        }
        std::size_t idx = bucketIndex(key, bucketCount);
        for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it) {
            if (it->first == key) {
                it->second = value;
                return false;
            }
        }
        buckets[idx].push_front(Pair{key, value});
        ++elementCount;
        return true;
    }

    //принимает rvalue-ссылку и перемещает значение, избегает копирования
    bool insert(const Key& key, Value&& value) {
        if (load_factor() >= MAX_LOAD_FACTOR) {
            rehash(bucketCount * 2);
        }
        std::size_t idx = bucketIndex(key, bucketCount);
        for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it) {
            if (it->first == key) {
                it->second = std::move(value);
                return false;
            }
        }
        buckets[idx].push_front(Pair{key, std::move(value)});
        ++elementCount;
        return true;
    }

    bool erase(const Key& key) {
        if (bucketCount == 0) return false;
        std::size_t idx = bucketIndex(key, bucketCount);
        //проверяем 1 элемент в бакете отдельно, так как он может быть удалён через pop_front, а остальные — только через erase_after(удаляем элемент после итератора)
        if (!buckets[idx].empty() && buckets[idx].front().first == key) {
            buckets[idx].pop_front();
            --elementCount;
            return true;
        }
        // смотрим остальные элементы бакета
        auto prev = buckets[idx].begin();
        auto cur = prev;
        if (cur != buckets[idx].end()) ++cur;

        while (cur != buckets[idx].end()) {
            if (cur->first == key) {
                buckets[idx].erase_after(prev);
                --elementCount;
                return true;
            }
            prev = cur;
            ++cur;
        }
        return false;
    }

    void clear() {
        for (std::size_t i = 0; i < bucketCount; ++i) {
            buckets[i].clear();
        }
        elementCount = 0;
    }

    class Iterator {
    private:
        ForwardListV<Pair>* buckets;
        std::size_t bucketCount;
        std::size_t bucketIdx;
        typename ForwardListV<Pair>::Iterator listIt;

        //перемещается к следующему непустому бакету, пока текущий список не закончится
        void advance_to_valid() {
            while (bucketIdx < bucketCount &&
                   listIt == buckets[bucketIdx].end()) {
                ++bucketIdx;
                if (bucketIdx < bucketCount) {
                    listIt = buckets[bucketIdx].begin();
                }
            }
        }

    public:
        Iterator() : buckets(nullptr), bucketCount(0), bucketIdx(0) {}

        Iterator(ForwardListV<Pair>* buckets, std::size_t bucketCount,
                 std::size_t bucketIdx,
                 typename ForwardListV<Pair>::Iterator listIt)
            : buckets(buckets), bucketCount(bucketCount),
              bucketIdx(bucketIdx), listIt(listIt) {
            advance_to_valid();
        }

        Pair& operator*() const { return *listIt; }
        Pair* operator->() const { return &(*listIt); }

        Iterator& operator++() {
            ++listIt;
            advance_to_valid();
            return *this;
        }

        Iterator operator++(int) {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        bool operator==(const Iterator& other) const {
            if (bucketIdx >= bucketCount && other.bucketIdx >= other.bucketCount)
                return true;
            return bucketIdx == other.bucketIdx && listIt == other.listIt;
        }

        bool operator!=(const Iterator& other) const { return !(*this == other); }
    };

    Iterator begin() {
        if (bucketCount == 0) return end();
        return Iterator(buckets, bucketCount, 0, buckets[0].begin());
    }

    Iterator end() {
        return Iterator(buckets, bucketCount, bucketCount,
                        typename ForwardListV<Pair>::Iterator{});
    }

    Iterator find(const Key& key) {
        if (bucketCount == 0) return this->end();
        std::size_t idx = bucketIndex(key, bucketCount);
        for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it) {
            if (it->first == key) return this->begin();
        }
        return this->end();
    }
};

#endif  // HASH_MAP_H