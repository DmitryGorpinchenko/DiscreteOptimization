#ifndef BOUNDED_SET_H
#define BOUNDED_SET_H

#include <vector>

class BoundedSet {
public:
    BoundedSet(size_t n, bool insert_all = false)
        : data(n, insert_all)
        , count(insert_all ? n : 0)
    {
    }
    ~BoundedSet() = default;
    
    BoundedSet(const BoundedSet& other) = default;
    BoundedSet& operator =(const BoundedSet& other) = default;
    
    BoundedSet(BoundedSet&& other) = delete;
    BoundedSet& operator =(BoundedSet&& other) = delete;
    
    void swap(BoundedSet& other) {
        data.swap(other.data);
        std::swap(count, other.count);
    }

    size_t capacity() const { return data.size(); }
    size_t size() const { return count; }
    
    bool empty() const { return size() == 0; }
    bool full() const { return size() == capacity(); }

    bool contains(size_t e) const { return data[e]; }
    
    void insert(size_t e) {
        if (!data[e]) {
            data[e] = true;
            ++count;
        }
    }
    
    void insertAll() {
        data.assign(capacity(), true);
        count = capacity();
    }
    
    void erase(size_t e) {
        if (data[e]) {
            data[e] = false;
            --count;
        }
    }
    
    void clear() {
        data.assign(capacity(), false);
        count = 0;
    }

private:
    std::vector<bool> data;
    size_t count;
};

#endif
