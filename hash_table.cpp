#include "hash_table.hpp"

#include <iostream>
#include <string>

/********************************************************************/

const char *NoSpaceException::what() const throw()
{
    return "No space available in the map!";
}

KeyException::KeyException(const Tk &key) : _key{key}
{
}

const char *KeyException::what() const throw()
{
    return (std::string{"Key error: "} + std::to_string(_key)).c_str();
}

/********************************************************************/

DoublyLinkedList::DoublyLinkedList(
    const Tk &key,
    const Tv &value,
    DoublyLinkedList *prev,
    DoublyLinkedList *next) : item{key, value},
                              prev{prev},
                              next{next}
{
}

DoublyLinkedList::~DoublyLinkedList()
{
}

/********************************************************************/

HashTable::HashTable(const Tsize &bucket_capacity) : _bucket_capacity{bucket_capacity}
{
    _items_list = new DoublyLinkedList *[_bucket_capacity];
    for (Tsize i{}; i < _bucket_capacity; ++i)
        _items_list[i] = nullptr;
}

HashTable::~HashTable()
{
    for (Tsize i{}; i < _bucket_capacity; ++i)
        delete _items_list[i];
    delete _items_list;
}

HashTableItem &HashTable::operator[](const Tk &key)
{
    return _get(key);
}

const HashTableItem &HashTable::operator[](const Tk &key) const
{
    return _get(key);
}

void HashTable::insert(const Tk &key, const Tv &value)
{
    const Tsize key_hash{_hash_function(key)};
    if (!_items_list[key_hash])
    {
        _items_list[key_hash] = new DoublyLinkedList{key, value};
    }
    else
    {
        bool found{false};
        DoublyLinkedList *current_item{_items_list[key_hash]};
        DoublyLinkedList *previous_item{};
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
            current_item = new DoublyLinkedList{key, value, previous_item};
            previous_item->next = current_item;
        }
    }
}

void HashTable::remove(const Tk &key)
{
    const Tsize key_hash{_hash_function(key)};
    DoublyLinkedList *current_item = _items_list[key_hash];
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
            return;
        }
        else
        {
            current_item = current_item->next;
        }
    }
    throw KeyException{key};
}

void HashTable::print_table() const
{
    std::cout << "\nHash Table\n-------------------\n";
    for (Tsize i{}; i < _bucket_capacity; ++i)
    {
        DoublyLinkedList *current_item = _items_list[i];
        while (current_item)
        {
            std::cout << "Index: " << i << ", Key: " << current_item->item.key << ", Value: " << current_item->item.value << std::endl;
            current_item = current_item->next;
        }
    }
    std::cout << "-------------------\n\n";
}

void HashTable::print_item(const Tk &key) const
{
    try
    {
        const Tv value{_get(key).value};
        std::cout << "Key: " << key << ", Value: " << value << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

Tsize HashTable::_hash_function(const Tk &key) const
{
    // TODO change this hash function
    return key % _bucket_capacity;
}

HashTableItem &HashTable::_get(const Tk &key)
{
    DoublyLinkedList *current_item = _items_list[_hash_function(key)];
    while (current_item)
    {
        if (current_item->item.key == key)
            return current_item->item;
        else
            current_item = current_item->next;
    }
    throw KeyException{key};
}

const HashTableItem &HashTable::_get(const Tk &key) const
{
    return _get(key);
}
