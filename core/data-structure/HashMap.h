#pragma once
#include <vector>
#include <list>
#include <utility>
#include <functional>  // std::hash
#include <stdexcept>   // exceptions
#include <type_traits> // static_assert


template <
    typename Key,
    typename T,
    typename Hash = std::hash<Key>,
    typename Eq = std::equal_to<Key>
>
class HashMap {
private:
    static constexpr size_t DEFAULT_CAPACITY = 16;
    static constexpr double LOAD_FACTOR = 0.75;
    static constexpr size_t GROWTH_FACTOR = 2;

    struct Entry {
        Key key;
        T value;
    };

    std::vector<std::list<Entry>> buckets;
    size_t elementCount = 0;

    Hash hasher{};
    Eq equals{};

    size_t bucketIndex(const Key& key) const {
        return hasher(key) % buckets.size();
    }

    void resizeIfNeeded() {
        if (elementCount < static_cast<size_t>(buckets.size() * LOAD_FACTOR)) return;

        size_t newCapacity = buckets.size() * GROWTH_FACTOR;
        if (newCapacity == 0) newCapacity = 1;

        std::vector<std::list<Entry>> newBuckets(newCapacity);

        for (auto& bucket : buckets) {
            for (auto& entry : bucket) {
                size_t newIndex = hasher(entry.key) % newCapacity;
                newBuckets[newIndex].push_back(std::move(entry));
            }
        }

        buckets = std::move(newBuckets);
    }

public:
    explicit HashMap(size_t initialCapacity = DEFAULT_CAPACITY)
        : buckets(initialCapacity == 0 ? 1 : initialCapacity) {}

    bool put(const Key& key, const T& value) {
        resizeIfNeeded();
        size_t idx = bucketIndex(key);

        for (auto& entry : buckets[idx]) {
            if (equals(entry.key, key)) {
                entry.value = value;    
                return false;
            }
        }

        buckets[idx].push_back(Entry{ key, value });
        ++elementCount;
        return true;
    }

    T* getPtr(const Key& key) {
        size_t idx = bucketIndex(key);
        for (auto& entry : buckets[idx]) {
            if (equals(entry.key, key)) return &entry.value;
        }
        return nullptr;
    }

    const T* getPtr(const Key& key) const {
        size_t idx = bucketIndex(key);
        for (const auto& entry : buckets[idx]) {
            if (equals(entry.key, key)) return &entry.value;
        }
        return nullptr;
    }

    bool remove(const Key& key) {
        size_t idx = bucketIndex(key);
        auto& bucket = buckets[idx];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (equals(it->key, key)) {
                bucket.erase(it);
                --elementCount;
                return true;
            }
        }
        return false;
    }

    bool contains(const Key& key) const {
        return getPtr(key) != nullptr;
    }

    size_t size() const { return elementCount; }
    bool empty() const { return elementCount == 0; }

    std::vector<std::pair<Key, T>> entries() const {
        std::vector<std::pair<Key, T>> result;
        result.reserve(elementCount);

        for (const auto& bucket : buckets) {
            for (const auto& e : bucket) {
                result.emplace_back(e.key, e.value);
            }
        }
        return result;
    }

    std::vector<Key> keys() const {
        std::vector<Key> result;
        result.reserve(elementCount);

        for (const auto& bucket : buckets) {
            for (const auto& e : bucket) {
                result.push_back(e.key);
            }
        }
        return result;
    }

    std::vector<T> values() const {
        std::vector<T> result;
        result.reserve(elementCount);

        for (const auto& bucket : buckets) {
            for (const auto& e : bucket) {
                result.push_back(e.value);
            }
        }
        return result;
    }

    std::vector<const Key*> keysPtr() const {
        std::vector<const Key*> result;
        result.reserve(elementCount);

        for (const auto& bucket : buckets) {
            for (const auto& e : bucket) {
                result.push_back(&e.key);
            }
        }
        return result;
    }

    std::vector<const T*> valuesPtr() const {
        std::vector<const T*> result;
        result.reserve(elementCount);

        for (const auto& bucket : buckets) {
            for (const auto& e : bucket) {
                result.push_back(&e.value);
            }
        }
        return result;
    }
};
