#ifndef SPARSE_VECTOR_H
#define SPARSE_VECTOR_H

#include <vector>
#include <unordered_map>

class SparseVector {
    class IntProxy;
public:
    SparseVector(size_t _n);

    size_t size() const { return n; }

    IntProxy operator [](size_t index);
    const IntProxy operator [](size_t index) const;
    
    int dot(const std::vector<int>& rhs) const;
    std::vector<int> toStdVector() const;

private:
    size_t n;
    std::unordered_map<size_t, int> data;
};

//

class SparseVector::IntProxy {
public:
    IntProxy(SparseVector& _v, size_t _index);

    IntProxy& operator =(const IntProxy& rhs);
    IntProxy& operator =(int val);
    
    operator int() const;
private:
    SparseVector& v;
    size_t index;
};

inline SparseVector::IntProxy::IntProxy(SparseVector& _v, size_t _index)
    : v(_v)
    , index(_index)
{
}

inline SparseVector::IntProxy& SparseVector::IntProxy::operator =(const IntProxy& rhs) {
    return operator =(int(rhs));
}

inline SparseVector::IntProxy& SparseVector::IntProxy::operator =(int val) {
    if (val != 0) {
        v.data[index] = val;
    } else {
        v.data.erase(index);
    }
    return *this;
}

inline SparseVector::IntProxy::operator int() const {
    auto it = v.data.find(index);
    return it != v.data.end() ? (*it).second : 0;
}

//

inline SparseVector::SparseVector(size_t _n)
    : n(_n)
{
}

inline SparseVector::IntProxy SparseVector::operator [](size_t index) {
    return IntProxy(*this, index);
}

inline const SparseVector::IntProxy SparseVector::operator [](size_t index) const {
    return IntProxy(const_cast<SparseVector&>(*this), index);
}

inline int SparseVector::dot(const std::vector<int>& rhs) const {
    int res = 0;
    const auto& lhs = *this;
    for (size_t i = 0; i < rhs.size(); ++i) {
        res += lhs[i] * rhs[i];
    }
    return res;
}

inline std::vector<int> SparseVector::toStdVector() const {
    std::vector<int> res;
    res.reserve(n);
    const auto& v = *this;
    for (size_t i = 0; i < n; ++i) {
        res.push_back(v[i]);
    }
    return res;
}

#endif
