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

