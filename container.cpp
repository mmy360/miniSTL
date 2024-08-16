#include <iostream>
#include <stdexcept>


template <typename Derived, typename T>
class Contianer{
public:
    class Iterator{
    private:
        T *ptr;
    public:
        Iterator(T *p) : ptr(p){}

        T &operator*() const{
            return *ptr;
        }

        Iterator &operator++{
            ++ptr;
            return *this;
        }

        bool operator !=(const Iterator &other) const{
            return ptr != other.ptr;
        }
    };

    Contianer() {}

    woid push_back(const T &value){
        static_cast<Derived*>(this) -> push_back_impl(value);
    }

    T &operator[](size_t index){
        return static_cast<Derived*>(this) -> access_impl(index);
    }

    size_t size() const {
        return static_cast<const Derived*>(this) -> size_impl();
    }

    bool empty() const {
        return static_cast<const Derived*>(this)-> empty_impl();
    }

    Iterator begin() {
        return Iterator(static_cast<Derived*>(this)->data());
    }

    Iterator end(){
        return Iterator(static_cast<Derived*>(this)->data()+size());
    }
};


template<typename Derived, typename T>
class SequentialContainer : public ContainerBase<Derived, T> {
public:
    void push_back(const T& value) {
        static_cast<Derived*>(this)->push_back_impl(value);
    }

    T& operator[](size_t index) {
        return static_cast<Derived*>(this)->access_impl(index);
    }
};


template <typename Derived, typename Key, typename Value>
class AssociativeContainer : public ContainerBase<Derived, std::pair<Key, Value>> {
public:
    void insert(const Key& key, const Value& value) {
        static_cast<Derived*>(this)->insert_impl(key, value);
    }

    Value& operator[](const Key& key) {
        return static_cast<Derived*>(this)->access_impl(key);
    }

    bool contains(const Key& key) const {
        return static_cast<const Derived*>(this)->contains_impl(key);
    }

    void erase(const Key& key) {
        static_cast<Derived*>(this)->erase_impl(key);
    }
};



template <typename Derived, typename Key, typename Value>
class UnorderedContainer : public ContainerBase<Derived, std::pair<Key, Value>> {
public:
    void insert(const Key& key, const Value& value) {
        static_cast<Derived*>(this)->insert_impl(key, value);
    }

    Value& operator[](const Key& key) {
        return static_cast<Derived*>(this)->access_impl(key);
    }

    bool contains(const Key& key) const {
        return static_cast<const Derived*>(this)->contains_impl(key);
    }

    void erase(const Key& key) {
        static_cast<Derived*>(this)->erase_impl(key);
    }
};

template <typename T>
class Vector : public SequentialContainer<Vector<T>, T> {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

public:
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    ~Vector() {
        delete[] data_;
    }

    void push_back_impl(const T& value) {
        if (size_ >= capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    T& access_impl(size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    size_t size_impl() const {
        return size_;
    }

    bool empty_impl() const {
        return size_ == 0;
    }

    T* data() {
        return data_;
    }

    const T* data() const {
        return data_;
    }

    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }
};


template <typename T>
class List : public SequentialContainer<List<T>, T> {
private:
    struct Node {
        T value;
        Node* prev;
        Node* next;
        Node(const T& val) : value(val), prev(nullptr), next(nullptr) {}
    };

    Node* head_;
    Node* tail_;
    size_t size_;

public:
    List() : head_(nullptr), tail_(nullptr), size_(0) {}

    ~List() {
        while (head_) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
    }

    void push_back_impl(const T& value) {
        Node* new_node = new Node(value);
        if (tail_) {
            tail_->next = new_node;
            new_node->prev = tail_;
            tail_ = new_node;
        } else {
            head_ = tail_ = new_node;
        }
        ++size_;
    }

    T& access_impl(size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");
        Node* current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->value;
    }

    size_t size_impl() const {
        return size_;
    }

    bool empty_impl() const {
        return size_ == 0;
    }
};


template <typename T>
class ForwardList : public SequentialContainer<ForwardList<T>, T> {
private:
    struct Node {
        T value;
        Node* next;
        Node(const T& val) : value(val), next(nullptr) {}
    };

    Node* head_;
    size_t size_;

public:
    ForwardList() : head_(nullptr), size_(0) {}

    ~ForwardList() {
        while (head_) {
            Node* temp = head_;
            head_ = head_->next;
            delete temp;
        }
    }

    void push_back_impl(const T& value) {
        Node* new_node = new Node(value);
        if (!head_) {
            head_ = new_node;
        } else {
            Node* current = head_;
            while (current->next) {
                current = current->next;
            }
            current->next = new_node;
        }
        ++size_;
    }

    T& access_impl(size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");
        Node* current = head_;
        for (size_t i = 0; i < index; ++i) {
            current = current->next;
        }
        return current->value;
    }

    size_t size_impl() const {
        return size_;
    }

    bool empty_impl() const {
        return size_ == 0;
    }
};


#include <iostream>
#include <stdexcept>

template<typename T>
class MyDeque : public SequentialContainer<MyDeque<T>, T> {
private:
    T* data_;
    size_t size_;
    size_t capacity_;
    size_t front_;
    size_t back_;

    void resize(size_t new_capacity) {
        T* new_data = new T[new_capacity];
        size_t j = 0;
        for (size_t i = front_; i != back_; i = (i + 1) % capacity_) {
            new_data[j++] = std::move(data_[i]);
        }
        front_ = 0;
        back_ = size_;
        capacity_ = new_capacity;
        delete[] data_;
        data_ = new_data;
    }

public:
    MyDeque() : data_(nullptr), size_(0), capacity_(0), front_(0), back_(0) {}

    ~MyDeque() {
        delete[] data_;
    }

    void push_back_impl(const T& value) {
        if (size_ >= capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[back_] = value;
        back_ = (back_ + 1) % capacity_;
        ++size_;
    }

    void push_front_impl(const T& value) {
        if (size_ >= capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        front_ = (front_ == 0) ? capacity_ - 1 : front_ - 1;
        data_[front_] = value;
        ++size_;
    }

    T& access_impl(size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[(front_ + index) % capacity_];
    }

    size_t size_impl() const {
        return size_;
    }

    bool empty_impl() const {
        return size_ == 0;
    }
};

#include <iostream>
#include <stdexcept>

template<typename T, size_t N>
class MyArray : public SequentialContainer<MyArray<T, N>, T> {
private:
    T data_[N];

public:
    MyArray() {}

    void push_back_impl(const T& value) {
        throw std::out_of_range("Cannot push_back to a fixed-size array");
    }

    T& access_impl(size_t index) {
        if (index >= N) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    size_t size_impl() const {
        return N;
    }

    bool empty_impl() const {
        return N == 0;
    }
};

#include <iostream>
#include <cstring>
#include <stdexcept>

class MyString : public SequentialContainer<MyString, char> {
private:
    char* data_;
    size_t size_;
    size_t capacity_;

    void resize(size_t new_capacity) {
        char* new_data = new char[new_capacity];
        std::memcpy(new_data, data_, size_);
        delete[] data_;
        data_ = new_data;
        capacity_ = new_capacity;
    }

public:
    MyString() : data_(nullptr), size_(0), capacity_(0) {}

    ~MyString() {
        delete[] data_;
    }

    void push_back_impl(const char& value) {
        if (size_ >= capacity_) {
            resize(capacity_ == 0 ? 1 : capacity_ * 2);
        }
        data_[size_++] = value;
    }

    char& access_impl(size_t index) {
        if (index >= size_) throw std::out_of_range("Index out of range");
        return data_[index];
    }

    size_t size_impl() const {
        return size_;
    }

    bool empty_impl() const {
        return size_ == 0;
    }

    const char* c_str() const {
        return data_;
    }
};

