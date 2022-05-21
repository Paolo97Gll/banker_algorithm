#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdexcept>

using Tsize = unsigned long;
using Tk = unsigned long;
using Tv = int;

/********************************************************************/

class HashTableException : public std::exception
{
};

class NoSpaceException : public HashTableException
{
public:
    const char *what() const throw();
};

class KeyException : public HashTableException
{
public:
    KeyException(const Tk &key);
    const char *what() const throw();

private:
    Tk _key;
};

/********************************************************************/

struct HashTableItem
{
    Tk key;
    Tv value;
};

/********************************************************************/

class DoublyLinkedList
{
public:
    DoublyLinkedList(const Tk &key, const Tv &value, DoublyLinkedList *prev = nullptr, DoublyLinkedList *next = nullptr);
    ~DoublyLinkedList();

    HashTableItem item;
    DoublyLinkedList *prev;
    DoublyLinkedList *next;
};

/********************************************************************/

class HashTable
{
public:
    HashTable(const Tsize &bucket_capacity = 65536);
    ~HashTable();

    HashTableItem &operator[](const Tk &key);
    const HashTableItem &operator[](const Tk &key) const;

    void insert(const Tk &key, const Tv &value);
    void remove(const Tk &key);

    void print_table() const;
    void print_item(const Tk &key) const;

private:
    Tsize _hash_function(const Tk &key) const;

    HashTableItem &_get(const Tk &key);
    const HashTableItem &_get(const Tk &key) const;

    Tsize _bucket_capacity;
    DoublyLinkedList **_items_list;
};

/********************************************************************/

#endif // HASH_TABLE_H
