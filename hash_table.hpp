/**
 * @file hash_table.hpp
 * @author Paolo Galli
 * @brief Implementation of a simple hash table
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022 Paolo Galli
 *
 */

#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <cstdint>
#include <stdexcept>

#include "doubly_linked_list.hpp"

/********************************************************************/

template <typename T>
struct HashTableItem
{
    std::uint32_t key;
    T value;
};

/********************************************************************/

template <typename T>
class HashTable
{
public:
    HashTable(const std::uint32_t &bucket_capacity = 65536);
    ~HashTable();

    void insert(const std::uint32_t &key, const T &value);
    void remove(const std::uint32_t &key);
    void clear();

    T &operator[](const std::uint32_t &key);
    const T &operator[](const std::uint32_t &key) const;
    bool contains(const std::uint32_t &key) const;

    const std::uint32_t &length() const;
    const DoublyLinkedList<std::uint32_t> &keys() const;

private:
    std::uint32_t _hash_function(const std::uint32_t &key) const;

    std::uint32_t _bucket_capacity;
    std::uint32_t _count;
    DoublyLinkedList<HashTableItem<T>> *_items_list;
    DoublyLinkedList<std::uint32_t> _keys_list;
};

template <typename T>
HashTable<T>::HashTable(const std::uint32_t &bucket_capacity) : _bucket_capacity{bucket_capacity}, _count{0}
{
    _items_list = new DoublyLinkedList<HashTableItem<T>>[_bucket_capacity];
}

template <typename T>
HashTable<T>::~HashTable()
{
    clear();
    delete[] _items_list;
}

template <typename T>
void HashTable<T>::insert(const std::uint32_t &key, const T &value)
{
    auto &bucket{_items_list[_hash_function(key)]};
    for (std::uint32_t i{}; i < bucket.length(); ++i)
    {
        if (bucket[i].key == key)
        {
            bucket[i].value = value;
            return;
        }
    }
    bucket.append({key, value});
    _keys_list.append(key);
    ++_count;
}

template <typename T>
void HashTable<T>::remove(const std::uint32_t &key)
{
    auto &bucket{_items_list[_hash_function(key)]};
    for (std::uint32_t i{}; i < bucket.length(); ++i)
    {
        if (bucket[i].key == key)
        {
            bucket.remove_indexbased(i);
            _keys_list.remove_valuebased(key);
            --_count;
            return;
        }
    }
    throw std::out_of_range{"Key not found"};
}

template <typename T>
void HashTable<T>::clear()
{
    for (std::uint32_t i{}; i < _bucket_capacity; ++i)
        _items_list[i].clear();
    _keys_list.clear();
    _count = 0;
}

template <typename T>
T &HashTable<T>::operator[](const std::uint32_t &key)
{
    auto &bucket{_items_list[_hash_function(key)]};
    for (std::uint32_t i{}; i < bucket.length(); ++i)
        if (bucket[i].key == key)
            return bucket[i].value;
    throw std::out_of_range{"Key not found"};
}

template <typename T>
const T &HashTable<T>::operator[](const std::uint32_t &key) const
{
    auto &bucket{_items_list[_hash_function(key)]};
    for (std::uint32_t i{}; i < bucket.length(); ++i)
        if (bucket[i].key == key)
            return bucket[i].value;
    throw std::out_of_range{"Key not found"};
}

template <typename T>
bool HashTable<T>::contains(const std::uint32_t &key) const
{
    auto &bucket{_items_list[_hash_function(key)]};
    for (std::uint32_t i{}; i < bucket.length(); ++i)
        if (bucket[i].key == key)
            return true;
    return false;
}

template <typename T>
const std::uint32_t &HashTable<T>::length() const
{
    return _count;
}

template <typename T>
const DoublyLinkedList<std::uint32_t> &HashTable<T>::keys() const
{
    return _keys_list;
}

template <typename T>
std::uint32_t HashTable<T>::_hash_function(const std::uint32_t &key) const
{
    // Knuth multiplicative algorithm
    return (static_cast<std::uint32_t>(key) * UINT32_C(2654435761)) % _bucket_capacity;
}

/********************************************************************/

#endif // _HASH_TABLE_H_
