#include <iostream>
#include <utility>

template <typename T>
class UniquePtr {
public:
    explicit UniquePtr(T* p = nullptr) : ptr_(p) {}
    ~UniquePtr() { delete ptr_; }

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) { other.ptr_ = nullptr; }
    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            delete ptr_;
            ptr_ = other.ptr_;
            other.ptr_ = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }

    void reset(T* p = nullptr) {
        if (ptr_ != p) {
            delete ptr_;
            ptr_ = p;
        }
    }

    T* release() {
        T* p = ptr_;
        ptr_ = nullptr;
        return p;
    }

private:
    T* ptr_;
};

template <typename T>
class SharedPtr {
public:
    explicit SharedPtr(T* p = nullptr) : ptr_(p), count_(p ? new int(1) : nullptr) {}
    ~SharedPtr() { dec(); }

    SharedPtr(const SharedPtr& other) : ptr_(other.ptr_), count_(other.count_) {
        if (count_) ++(*count_);
    }

    SharedPtr& operator=(const SharedPtr& other) {
        if (this != &other) {
            dec();
            ptr_ = other.ptr_;
            count_ = other.count_;
            if (count_) ++(*count_);
        }
        return *this;
    }

    SharedPtr(SharedPtr&& other) noexcept : ptr_(other.ptr_), count_(other.count_) {
        other.ptr_ = nullptr;
        other.count_ = nullptr;
    }

    SharedPtr& operator=(SharedPtr&& other) noexcept {
        if (this != &other) {
            dec();
            ptr_ = other.ptr_;
            count_ = other.count_;
            other.ptr_ = nullptr;
            other.count_ = nullptr;
        }
        return *this;
    }

    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    T* get() const { return ptr_; }
    int use_count() const { return count_ ? *count_ : 0; }

    void reset(T* p = nullptr) {
        dec();
        ptr_ = p;
        count_ = p ? new int(1) : nullptr;
    }

private:
    T* ptr_;
    int* count_;

    void dec() {
        if (count_) {
            if (--(*count_) == 0) {
                delete ptr_;
                delete count_;
            }
            ptr_ = nullptr;
            count_ = nullptr;
        }
    }
};