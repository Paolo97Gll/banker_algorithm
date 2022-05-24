/**
 * @file hash_table.hpp
 * @author Paolo Galli
 * @brief Implementation of a simple Hash Table
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022 Paolo Galli
 *
 */

#ifndef _HASH_TABLE_H_
#define _HASH_TABLE_H_

#include <inttypes.h>
#include <iostream>
#include <stdexcept>
#include <string>

/********************************************************************/

class HashTableException : public std::exception
{
};

class KeyException : public HashTableException
{
public:
    KeyException(const std::uint32_t &key);
    const char *what() const throw();

private:
    std::uint32_t _key;
};

KeyException::KeyException(const std::uint32_t &key) : _key{key}
{
}

const char *KeyException::what() const throw()
{
    return (std::string{"Key error: "} + std::to_string(_key)).c_str();
}

/********************************************************************/

template <typename T>
struct DoublyLinkedList
{
    T item;
    DoublyLinkedList *prev;
    DoublyLinkedList *next;
};

/********************************************************************/

template <typename TValue>
struct HashTableItem
{
    std::uint32_t key;
    TValue value;
};

/********************************************************************/

template <typename TValue>
class HashTable
{
public:
    HashTable(const std::uint32_t &bucket_capacity = 65536);
    ~HashTable();

    TValue &operator[](const std::uint32_t &key);
    const TValue &operator[](const std::uint32_t &key) const;

    void insert(const std::uint32_t &key, const TValue &value);
    void remove(const std::uint32_t &key);

    bool contains(const std::uint32_t &key) const;
    const DoublyLinkedList<std::uint32_t> *keys() const;
    const std::uint32_t &length() const;

    void print_table() const;
    void print_item(const std::uint32_t &key) const;

private:
    std::uint32_t _hash_function(const std::uint32_t &key) const;

    void _add_key_to_list(const std::uint32_t &key);
    void _remove_key_from_list(const std::uint32_t &key);

    TValue &_get(const std::uint32_t &key);
    const TValue &_get(const std::uint32_t &key) const;

    std::uint32_t _bucket_capacity;
    std::uint32_t _count;
    DoublyLinkedList<HashTableItem<TValue>> **_items_list;
    DoublyLinkedList<std::uint32_t> *_keys_list;
};

template <typename TValue>
HashTable<TValue>::HashTable(const std::uint32_t &bucket_capacity) : _bucket_capacity{bucket_capacity}, _count{0}, _keys_list{nullptr}
{
    _items_list = new DoublyLinkedList<HashTableItem<TValue>> *[_bucket_capacity];
    for (std::uint32_t i{}; i < _bucket_capacity; ++i)
        _items_list[i] = nullptr;
}

template <typename TValue>
HashTable<TValue>::~HashTable()
{
    for (std::uint32_t i{}; i < _bucket_capacity; ++i)
        delete _items_list[i];
    delete _items_list;
    delete _keys_list;
}

template <typename TValue>
TValue &HashTable<TValue>::operator[](const std::uint32_t &key)
{
    return _get(key);
}

template <typename TValue>
const TValue &HashTable<TValue>::operator[](const std::uint32_t &key) const
{
    return _get(key);
}

template <typename TValue>
void HashTable<TValue>::insert(const std::uint32_t &key, const TValue &value)
{
    const std::uint32_t key_hash{_hash_function(key)};
    if (!_items_list[key_hash])
    {
        _items_list[key_hash] = new DoublyLinkedList<HashTableItem<TValue>>{{key, value}, nullptr, nullptr};
        ++_count;
        _add_key_to_list(key);
    }
    else
    {
        bool found{false};
        DoublyLinkedList<HashTableItem<TValue>> *current_item{_items_list[key_hash]};
        DoublyLinkedList<HashTableItem<TValue>> *previous_item{};
        do
        {
            if (current_item->item.key == key)
            {
                current_item->item.value = value;
                found = true;
            }
            else
            {
                previous_item = current_item;
                current_item = current_item->next;
            }
        } while (current_item && !found);
        if (!found)
        {
            current_item = new DoublyLinkedList<HashTableItem<TValue>>{{key, value}, previous_item, nullptr};
            previous_item->next = current_item;
            ++_count;
            _add_key_to_list(key);
        }
    }
}

template <typename TValue>
void HashTable<TValue>::remove(const std::uint32_t &key)
{
    const std::uint32_t key_hash{_hash_function(key)};
    DoublyLinkedList<HashTableItem<TValue>> *current_item{_items_list[key_hash]};
    while (current_item)
    {
        if (current_item->item.key == key)
        {
            if (!current_item->next && !current_item->prev)
            {
                delete current_item;
                _items_list[key_hash] = nullptr;
            }
            else if (!current_item->next)
            {
                current_item->prev->next = nullptr;
                delete current_item;
            }
            else if (!current_item->prev)
            {
                current_item->next->prev = nullptr;
                _items_list[key_hash] = current_item->next;
                delete current_item;
            }
            else
            {
                current_item->prev->next = current_item->next;
                current_item->next->prev = current_item->prev;
                delete current_item;
            }
            --count;
            _remove_key_from_list(key);
            return;
        }
        else
        {
            current_item = current_item->next;
        }
    }
    throw KeyException{key};
}

template <typename TValue>
bool HashTable<TValue>::contains(const std::uint32_t &key) const
{
    DoublyLinkedList<HashTableItem<TValue>> *current_item = _items_list[_hash_function(key)];
    while (current_item)
    {
        if (current_item->item.key == key)
            return true;
        else
            current_item = current_item->next;
    }
    return false;
}

template <typename TValue>
const DoublyLinkedList<std::uint32_t> *HashTable<TValue>::keys() const
{
    return _keys_list;
}

template <typename TValue>
const std::uint32_t &HashTable<TValue>::length() const
{
    return _count;
}

template <typename TValue>
void HashTable<TValue>::print_table() const
{
    std::cout << "\nHash Table\n-------------------\n";
    for (std::uint32_t i{}; i < _bucket_capacity; ++i)
    {
        DoublyLinkedList<HashTableItem<TValue>> *current_item = _items_list[i];
        while (current_item)
        {
            std::cout << "Index: " << i << ", Key: " << current_item->item.key << ", Value: " << current_item->item.value << std::endl;
            current_item = current_item->next;
        }
    }
    std::cout << "-------------------\n\n";
}

template <typename TValue>
void HashTable<TValue>::print_item(const std::uint32_t &key) const
{
    try
    {
        const TValue value{_get(key).value};
        std::cout << "Key: " << key << ", Value: " << value << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

template <typename TValue>
std::uint32_t HashTable<TValue>::_hash_function(const std::uint32_t &key) const
{
    // Knuth multiplicative algorithm
    return (static_cast<std::uint32_t>(key) * UINT32_C(2654435761)) % _bucket_capacity;
}

template <typename TValue>
void HashTable<TValue>::_add_key_to_list(const std::uint32_t &key)
{
    if (!_keys_list)
    {
        _keys_list = new DoublyLinkedList<std::uint32_t>{key, nullptr, nullptr};
    }
    else
    {
        DoublyLinkedList<std::uint32_t> *current_key{_keys_list};
        DoublyLinkedList<std::uint32_t> *previous_key{};
        do
        {
            previous_key = current_key;
            current_key = current_key->next;
        } while (current_key);
        current_key = new DoublyLinkedList<std::uint32_t>{key, previous_key, nullptr};
        previous_key->next = current_key;
    }
}

template <typename TValue>
void HashTable<TValue>::_remove_key_from_list(const std::uint32_t &key)
{
    DoublyLinkedList<std::uint32_t> *current_key{_keys_list};
    while (current_key)
    {
        if (current_key->item == key)
        {
            if (!current_key->next && !current_key->prev)
            {
                delete current_key;
                _keys_list = nullptr;
            }
            else if (!current_key->next)
            {
                current_key->prev->next = nullptr;
                delete current_key;
            }
            else if (!current_key->prev)
            {
                current_key->next->prev = nullptr;
                _keys_list = current_key->next;
                delete current_key;
            }
            else
            {
                current_key->prev->next = current_key->next;
                current_key->next->prev = current_key->prev;
                delete current_key;
            }
            return;
        }
        else
        {
            current_key = current_key->next;
        }
    }
    throw KeyException{key};
}

template <typename TValue>
TValue &HashTable<TValue>::_get(const std::uint32_t &key)
{
    DoublyLinkedList<HashTableItem<TValue>> *current_item = _items_list[_hash_function(key)];
    while (current_item)
    {
        if (current_item->item.key == key)
            return current_item->item.value;
        else
            current_item = current_item->next;
    }
    throw KeyException{key};
}

template <typename TValue>
const TValue &HashTable<TValue>::_get(const std::uint32_t &key) const
{
    return _get(key);
}

/********************************************************************/

#endif // _HASH_TABLE_H_
