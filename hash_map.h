#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include <functional>

template <typename K, typename V, typename Hash = std::hash<K>>
class HashMap {
public:
    HashMap(size_t capacity = 8) : buckets(capacity < 1 ? 8 : capacity) {}

    void put(const K& key, const V& value) {
        size_t idx = hashKey(key);
        for (auto& kv : buckets[idx]) {
            if (kv.first == key) {
                kv.second = value;
                return;
            }
        }
        buckets[idx].push_back({key, value});
        ++sz;
        checkLoadFactor();
    }

    bool get(const K& key, V& value) const {
        size_t idx = hashKey(key);
        for (const auto& kv : buckets[idx]) {
            if (kv.first == key) {
                value = kv.second;
                return true;
            }
        }
        return false;
    }

    bool erase(const K& key) {
        size_t idx = hashKey(key);
        for (auto it = buckets[idx].begin(); it != buckets[idx].end(); ++it) {
            if (it->first == key) {
                buckets[idx].erase(it);
                --sz;
                return true;
            }
        }
        return false;
    }

private:
    using Entry = std::pair<K, V>;
    std::vector<std::list<Entry>> buckets;
    size_t sz = 0;
    double max_load_factor = 0.75;
    Hash hasher;

    size_t hashKey(const K& key) const {
        return hasher(key) % buckets.size();
    }

    void rehash(size_t new_bucket_count) {
        std::vector<std::list<Entry>> new_buckets(new_bucket_count);
        for (auto& bucket : buckets) {
            for (auto& kv : bucket) {
                size_t idx = hasher(kv.first) % new_bucket_count;
                new_buckets[idx].push_back(kv);
            }
        }
        buckets.swap(new_buckets);
    }

    void checkLoadFactor() {
        if ((double)sz / buckets.size() > max_load_factor) {
            rehash(buckets.size() * 2);
        }
    }
};