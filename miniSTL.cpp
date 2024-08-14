#include <iostream>
#include <stdexcept>

template <typename Derived, typename T>
class Container {
public:
    class Iterator {
    private:
        T *ptr;
    public:
        Iterator(T* p) : ptr(p) {}

        T &operator*() const {
            return *ptr;
        }

        Iterator &operator++() {
            ++ptr;
            return *this;
        }

        bool operator!=(const Iterator &other) const {
            return ptr != other.ptr;
        }
    };

    Container() {}

    void push_back(const T &value) {
        static_cast<Derived*>(this)->push_back_impl(value);
    }

    T &operator[](size_t index) {
        return static_cast<Derived*>(this)->access_impl(index);
    }

    size_t size() const {
        return static_cast<const Derived*>(this)->size_impl();
    }

    bool empty() const {
        return static_cast<const Derived*>(this)->empty_impl();
    }

    Iterator begin() {
        return Iterator(static_cast<Derived*>(this)->data());
    }

    Iterator end() {
        return Iterator(static_cast<Derived*>(this)->data() + size());
    }
};




template <typename Derived, typename T>
class SequentialContainer : public Container<Derived, T> {
public:
    using Container<Derived, T>::Container;

    void push_back(const T &value) {
        static_cast<Derived*>(this)->push_back_impl(value);
    }

    T &operator[](size_t index) {
        return static_cast<Derived*>(this)->access_impl(index);
    }

    size_t size() const {
        return static_cast<const Derived*>(this)->size_impl();
    }

    bool empty() const {
        return static_cast<const Derived*>(this)->empty_impl();
    }
};

template <typename Derived, typename Key, typename Value>
class UnorderedContainer {
public:
    virtual void insert(const Key& key, const Value& value) = 0;
    virtual Value& operator[](const Key& key) = 0;
    virtual bool contains(const Key& key) const = 0;
    virtual void erase(const Key& key) = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
};

// 继承实现 UnorderedMap, UnorderedSet 等


template <typename Derived, typename T>
class AdapterContainer {
public:
    virtual void push(const T& value) = 0;
    virtual void pop() = 0;
    virtual T& top() = 0;
    virtual bool empty() const = 0;
    virtual size_t size() const = 0;
};

// 继承实现 Stack, Queue 等


template <typename Derived, typename Key, typename Value>
class AssociativeContainer {
public:
    virtual void insert(const Key& key, const Value& value) = 0;
    virtual Value& operator[](const Key& key) = 0;
    virtual bool contains(const Key& key) const = 0;
    virtual void erase(const Key& key) = 0;
    virtual size_t size() const = 0;
    virtual bool empty() const = 0;
};

// 继承实现 Map, Set 等



template <typename T>
class MyVector : public Container<MyVector<T>, T> {
public:
    MyVector() : Container<MyVector<T>, T>() {}

    ~MyVector() {
        delete[] this->data_;
    }

    void push_back_impl(const T& value) {
        if (this->size_ >= this->capacity_) {
            resize(this->capacity_ == 0 ? 1 : this->capacity_ * 2);
        }
        this->data_[this->size_++] = value;
    }

    T& access_impl(size_t index) {
        if (index >= this->size_) throw std::out_of_range("Index out of range");
        return this->data_[index];
    }

    size_t size_impl() const {
        return this->size_;
    }

    bool empty_impl() const {
        return this->size_ == 0;
    }

    T* data() {
        return this->data_;
    }

    const T* data() const {
        return this->data_;
    }

    void resize(size_t new_capacity) {
        if (new_capacity < this->size_) return;
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < this->size_; ++i) {
            new_data[i] = std::move(this->data_[i]);
        }
        delete[] this->data_;
        this->data_ = new_data;
        this->capacity_ = new_capacity;
    }
};

int main() {
    // Testing MyVector
    MyVector<int> vec;
    vec.push_back(10);
    vec.push_back(20);
    vec.push_back(30);

    std::cout << "MyVector elements: ";
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << "\n";

    // std::cout ·<< "\n";

    return 0;
}