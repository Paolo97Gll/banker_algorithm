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

/**
 * @brief Hash table item, to store the key and the value of the element.
 *
 * @tparam T type of the value stored in the table
 */
template <typename T>
struct HashTableItem
{
    std::uint32_t key;
    T value;
};

/********************************************************************/

/**
 * @brief A simple hash table.
 *
 * @tparam T type of the value stored in the table
 */
template <typename T>
class HashTable
{
public:
    /**
     * @brief Construct a new HashTable object.
     */
    HashTable(const std::uint32_t &bucket_capacity = 65536);

    /**
     * @brief Destroy the HashTable object.
     */
    ~HashTable();

    /**
     * @brief Insert the given element in the table.
     *
     * @param key the key of the element to insert
     * @param value the value of the element to insert
     */
    void insert(const std::uint32_t &key, const T &value);

    /**
     * @brief Remove the element with the specified key from the table.
     *
     * @param key the key of the element to remove
     */
    void remove(const std::uint32_t &key);

    /**
     * @brief Erases all elements from the table.
     */
    void clear();

    /**
     * @brief Access or modify the specified element; no insertion.
     *
     * @param key the key of the element to find
     * @return the value of the element at the given key
     */
    T &operator[](const std::uint32_t &key);

    /**
     * @brief Access or modify the specified element; no insertion.
     *
     * @param key the key of the element to find
     * @return the value of the element at the given key
     */
    const T &operator[](const std::uint32_t &key) const;

    /**
     * @brief Checks if the table contains the specified key.
     *
     * @param key key of the element to search for
     * @return `true` if there is such an element, otherwise `false`
     */
    bool contains(const std::uint32_t &key) const;

    /**
     * @brief Get the number of elements in the table.
     *
     * @return the length of the table
     */
    const std::uint32_t &length() const;

    /**
     * @brief Get the keys list.
     *
     * @return the list of the keys contained in the table
     */
    const DoublyLinkedList<std::uint32_t> &keys() const;

private:
    /**
     * @brief Hash function, implemented using the Knuth multiplicative algorithm.
     *
     * @param key the input of the hash function, aka the element key
     * @return the hashed key
     */
    std::uint32_t _hash_function(const std::uint32_t &key) const;

    /**
     * @brief Length of the table bucket list.
     */
    std::uint32_t _bucket_capacity;

    /**
     * @brief Pointer to the bucket list.
     */
    DoublyLinkedList<HashTableItem<T>> *_items_list;

    /**
     * @brief List of the table keys (non-hashed).
     */
    DoublyLinkedList<std::uint32_t> _keys_list;

    /**
     * @brief Number of elements in the table.
     */
    std::uint32_t _count;
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
    return (static_cast<std::uint32_t>(key) * UINT32_C(2654435761)) % _bucket_capacity;
}

/********************************************************************/

#endif // _HASH_TABLE_H_
