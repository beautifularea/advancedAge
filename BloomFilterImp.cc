//copy from : https://blog.csdn.net/better_jh/article/details/77187897

//字符转数字:BKDRHash
size_t BKDRHash(const char * str)
{
    unsigned int seed = 131; // 31 131 1313 13131 131313
    unsigned int hash = 0;
    while (*str)
    {
        hash = hash * seed + (*str++);
    }
    return (hash & 0x7FFFFFFF);
}
//字符转数字:SDBMHash
size_t SDBMHash(const char* str)
{
    register size_t hash = 0;
    while(size_t ch = (size_t)*str++)
    {
    hash = 65599*hash+ch;
    //hash = (size_t)ch+(hash<<6)+ (hash<<16)-hash;
    }
    return hash;
}
//字符转数字:RSHash
size_t RSHash(const char *str)
{
    register size_t hash = 0;
    size_t magic = 63689;
    while (size_t ch = (size_t)*str++)
    {
        hash = hash * magic + ch;
        magic *= 378551;
    }
    return hash;
}
//字符转数字:APHash
size_t APHash(const char* str)
{
    register size_t hash = 0;
    size_t ch;
    for (long i = 0; ch = (size_t)*str++; i++)
    {
        if (0 == (i&1))
        {
            hash ^= ((hash << 7) ^ (hash >> 3));
        }
        else
        {
            hash ^= (~((hash << 11) ^ ch ^ (hash >> 5)));
        }
    }
    return hash;
}
//字符转数字:JSHash
size_t JSHash(const char* str)
{
    if (!*str)
        return 0;
    register size_t hash = 1315423911;
    while (size_t ch = (size_t)*str++)
    {
        hash ^= ((hash << 5) + ch + (hash >> 2));
    }
    return hash;
}

template<typename K>
struct _Fun1
{
    size_t operator()(const K& key)
    {
        return BKDRHash(key.c_str());
    }
};

template<typename K>
struct _Fun2
{
    size_t operator()(const K& key)
    {
        return SDBMHash(key.c_str());
    }
};

template<typename K>
struct _Fun3
{
    size_t operator()(const K& key)
    {
        return RSHash(key.c_str());
    }
};

template<typename K>
struct _Fun4
{
    size_t operator()(const K& key)
    {
        return APHash(key.c_str());
    }
};

template<typename K>
struct _Fun5
{
    size_t operator()(const K& key)
    {
        return JSHash(key.c_str());
    }
};

//Bitmap
#include<iostream>
#include<vector>
using namespace std;
class BitMap
{
public:
    BitMap()
    {}
    BitMap(size_t size)
    {
        _table.resize((size >> 5) + 1);
    }
    void Set(int val)//将对应bit位置1--等同于插入一个元素
    {
        size_t byteNo = val >> 5;
        size_t bitNo = val % 32;
        _table[byteNo] |= (1 << bitNo);
    }
    void Reset(int val)//将对应bit位置0--等同于删除一个元素
    {
        size_t byteNo = val >> 5;
        size_t bitNo = val % 32;
        _table[byteNo] &= ~(1 << bitNo);
    }
    bool Test(int val)
    {
        size_t byteNo = val >> 5;
        size_t bitNo = val % 32;
        if ((1 << bitNo)&_table[byteNo])
        {
            return true;
        }
        else
        {
            return false;
        }
    }
private:
    vector<int> _table;
};

//bloom filter
#include "BitMap.hpp"
#include "common.h"
template<typename T,class Fun1 = _Fun1<T>,
                  class Fun2 = _Fun2<T>,
                  class Fun3 = _Fun3<T>,
                  class Fun4 = _Fun4<T>,
                  class Fun5 = _Fun5<T>>
class BloomFilter
{
public:
    BloomFilter(size_t size)
        : _bm(size)
        , _capacity(size)
    {}
    void Insert(const T& key)
    {
        size_t idx1 = Fun1()(key) % _capacity;
        _bm.Set(idx1);
        size_t idx2 = Fun2()(key) % _capacity;
        _bm.Set(idx2);
        size_t idx3 = Fun3()(key) % _capacity;
        _bm.Set(idx3);
        size_t idx4 = Fun4()(key) % _capacity;
        _bm.Set(idx4);
        size_t idx5 = Fun5()(key) % _capacity;
        _bm.Set(idx5);
        cout << idx1 << " " << idx2 << " " << idx3 << " " << idx4 << " " << idx5 << " " << endl;
    }
    bool Find(const T& key)
    {
        size_t idx1 = Fun1()(key) % _capacity;
        size_t idx2 = Fun2()(key) % _capacity;
        size_t idx3 = Fun3()(key) % _capacity;
        size_t idx4 = Fun4()(key) % _capacity;
        size_t idx5 = Fun5()(key) % _capacity;
        if (!_bm.Test(idx1))
            return false;
        if (!_bm.Test(idx2))
            return false;
        if (!_bm.Test(idx3))
            return false; 
        if (!_bm.Test(idx4))
            return false; 
        if (!_bm.Test(idx5))
            return false;
        cout << idx1 << " " << idx2 << " " << idx3 << " " << idx4 << " " << idx5 << " " << endl;
        return true;
    }
private:
    BitMap _bm;
    size_t _capacity;
};
void FunTest()
{
    BloomFilter<string> bf(100);
    bf.Insert("圆通");
    bf.Insert("韵达");
    bf.Insert("天天");
    bf.Insert("汇通");
    bf.Insert("中通");
    cout<<bf.Find("天天")<<endl;
    cout << bf.Find("邮政")<<endl;
}
