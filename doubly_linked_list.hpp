/**
 * @file doubly_linked_list.hpp
 * @author Paolo Galli
 * @brief Implementation of a simple doubly linked list
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022 Paolo Galli
 *
 */

#ifndef _DOUBLY_LINKED_LIST_H_
#define _DOUBLY_LINKED_LIST_H_

#include <cstdint>
#include <stdexcept>

/********************************************************************/

template <typename T>
struct DoublyLinkedListItem
{
    T value;
    DoublyLinkedListItem *prev;
    DoublyLinkedListItem *next;
};

/********************************************************************/

template <typename T>
class DoublyLinkedList
{
public:
    DoublyLinkedList();
    ~DoublyLinkedList();

    void append(const T &value);
    void remove_indexbased(const std::uint32_t &index);
    void remove_valuebased(const T &value);
    void clear();

    T &operator[](const std::uint32_t &index);
    const T &operator[](const std::uint32_t &index) const;

    std::uint32_t length() const;

private:
    DoublyLinkedListItem<T> *_first_item;
    DoublyLinkedListItem<T> *_last_item;
    std::uint32_t _count;
};

template <typename T>
DoublyLinkedList<T>::DoublyLinkedList() : _first_item{nullptr}, _last_item{nullptr}, _count{0}
{
}

template <typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
    clear();
}

template <typename T>
void DoublyLinkedList<T>::append(const T &value)
{
    if (!_last_item)
    {
        _last_item = new DoublyLinkedListItem<T>{value, nullptr, nullptr};
        _first_item = _last_item;
    }
    else
    {
        DoublyLinkedListItem<T> *new_item = new DoublyLinkedListItem<T>{value, _last_item, nullptr};
        _last_item->next = new_item;
        _last_item = new_item;
    }
    ++_count;
}

template <typename T>
void DoublyLinkedList<T>::remove_indexbased(const std::uint32_t &index)
{
    DoublyLinkedListItem<T> *current_item{_first_item};
    for (std::uint32_t i{}; current_item && i < index; ++i)
        current_item = current_item->next;
    if (current_item)
    {
        if (!current_item->next && !current_item->prev)
        {
            delete current_item;
            _first_item = nullptr;
            _last_item = nullptr;
        }
        else if (!current_item->next)
        {
            current_item->prev->next = nullptr;
            _last_item = current_item->prev;
            delete current_item;
        }
        else if (!current_item->prev)
        {
            current_item->next->prev = nullptr;
            _first_item = current_item->next;
            delete current_item;
        }
        else
        {
            current_item->prev->next = current_item->next;
            current_item->next->prev = current_item->prev;
            delete current_item;
        }
        --_count;
        return;
    }
    throw std::out_of_range{};
}

template <typename T>
void DoublyLinkedList<T>::remove_valuebased(const T &value)
{
    DoublyLinkedListItem<T> *current_item{_first_item};
    while (current_item)
    {
        if (current_item->value == value)
        {
            if (!current_item->next && !current_item->prev)
            {
                delete current_item;
                _first_item = nullptr;
                _last_item = nullptr;
            }
            else if (!current_item->next)
            {
                current_item->prev->next = nullptr;
                _last_item = current_item->prev;
                delete current_item;
            }
            else if (!current_item->prev)
            {
                current_item->next->prev = nullptr;
                _first_item = current_item->next;
                delete current_item;
            }
            else
            {
                current_item->prev->next = current_item->next;
                current_item->next->prev = current_item->prev;
                delete current_item;
            }
            --_count;
            return;
        }
        else
        {
            current_item = current_item->next;
        }
    }
    throw std::out_of_range{};
}

template <typename T>
void DoublyLinkedList<T>::clear()
{
    DoublyLinkedListItem<T> *current_item{_first_item};
    DoublyLinkedListItem<T> *previous_item{};
    while (current_item)
    {
        previous_item = current_item;
        current_item = current_item->next;
        delete previous_item;
    }
    _first_item = nullptr;
    _last_item = nullptr;
    _count = 0;
}

template <typename T>
T &DoublyLinkedList<T>::operator[](const std::uint32_t &index)
{
    DoublyLinkedListItem<T> *current_item{_first_item};
    for (std::uint32_t i{}; current_item && i < index; ++i)
        current_item = current_item->next;
    if (current_item)
        return current_item->value;
    throw std::out_of_range{};
}

template <typename T>
const T &DoublyLinkedList<T>::operator[](const std::uint32_t &index) const
{
    DoublyLinkedListItem<T> *current_item{_first_item};
    for (std::uint32_t i{}; current_item && i < index; ++i)
        current_item = current_item->next;
    if (current_item)
        return current_item->value;
    throw std::out_of_range{};
}

template <typename T>
std::uint32_t DoublyLinkedList<T>::length() const
{
    return _count;
}

/********************************************************************/

#endif // _DOUBLY_LINKED_LIST_H_
