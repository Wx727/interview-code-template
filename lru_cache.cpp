#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>

template <typename K, typename V>
class LRUCache {
private:
    using CacheNode = std::pair<K, V>;
    using CacheIt = typename std::list<CacheNode>::iterator;

    size_t capacity_;
    std::list<CacheNode> lru_list_;
    std::unordered_map<K, CacheIt> key_to_iter_;

public:
    explicit LRUCache(size_t capacity) : capacity_(capacity) {}

    V* get(const K& key) {
        auto it = key_to_iter_.find(key);
        if (it == key_to_iter_.end()) {
            return nullptr;
        }

        lru_list_.splice(lru_list_.begin(), lru_list_, it->second);
        return &it->second->second;
    }

    void put(const K& key, const V& value) {
        if (capacity_ == 0) {
            return;
        }

        auto it = key_to_iter_.find(key);
        if (it != key_to_iter_.end()) {
            it->second->second = value;
            lru_list_.splice(lru_list_.begin(), lru_list_, it->second);
            return;
        }

        if (lru_list_.size() == capacity_) {
            K old_key = lru_list_.back().first;
            lru_list_.pop_back();
            key_to_iter_.erase(old_key);
        }

        lru_list_.push_front({key, value});
        key_to_iter_[key] = lru_list_.begin();
    }

    bool remove(const K& key) {
        auto it = key_to_iter_.find(key);
        if (it == key_to_iter_.end()) {
            return false;
        }

        lru_list_.erase(it->second);
        key_to_iter_.erase(it);
        return true;
    }

    void print() const {
        for (const auto& [key, value] : lru_list_) {
            std::cout << "(" << key << ", " << value << ") ";
        }
        std::cout << '\n';
    }
};

int main() {
    LRUCache<int, std::string> cache(2);

    cache.put(1, "one");
    cache.put(2, "two");
    cache.print();

    cache.get(1);
    cache.put(3, "three");
    cache.print();

    std::cout << (cache.get(2) == nullptr) << '\n';
    return 0;
}
