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

/**
 * @brief Doubly linked list item, to store the value and the pointers
 * the the next and the previous item.
 *
 * @tparam T type of the value stored in the list
 */
template <typename T>
struct DoublyLinkedListItem
{
    T value;
    DoublyLinkedListItem<T> *prev;
    DoublyLinkedListItem<T> *next;
};

/********************************************************************/

/**
 * @brief A simple doubly linked list.
 *
 * @tparam T type of the value stored in the list
 */
template <typename T>
class DoublyLinkedList
{
public:
    /**
     * @brief Construct a new DoublyLinkedList object.
     */
    DoublyLinkedList();

    /**
     * @brief Destroy the DoublyLinkedList object.
     */
    ~DoublyLinkedList();

    /**
     * @brief Appends the given element value to the end of the list.
     *
     * @param value the value of the element to append
     */
    void append(const T &value);

    /**
     * @brief Remove the element at the specified index from the list.
     *
     * @param index the index of the element to remove
     */
    void remove_indexbased(const std::uint32_t &index);

    /**
     * @brief Remove the first element of the given value from the list.
     *
     * @param value the value of the element to remove
     */
    void remove_valuebased(const T &value);

    /**
     * @brief Erases all elements from the list.
     */
    void clear();

    /**
     * @brief Access or modify the specified element; no insertion.
     *
     * @param index the index of the element to find
     * @return the value of the element at the given index
     */
    T &operator[](const std::uint32_t &index);

    /**
     * @brief Access or modify the specified element; no insertion.
     *
     * @param index the index of the element to find
     * @return the value of the element at the given index
     */
    const T &operator[](const std::uint32_t &index) const;

    /**
     * @brief Checks if the list contains the specified element.
     *
     * @param value value of the element to search for
     * @return `true` if there is such an element, otherwise `false`
     */
    bool contains(const T &value) const;

    /**
     * @brief Get the number of elements in the list.
     *
     * @return the length of the list
     */
    const std::uint32_t &length() const;

private:
    /**
     * @brief Pointer to the first element of the list.
     */
    DoublyLinkedListItem<T> *_first_item;

    /**
     * @brief Pointer to the last item of the list.
     */
    DoublyLinkedListItem<T> *_last_item;

    /**
     * @brief Number of elements in the list.
     */
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
        DoublyLinkedListItem<T> *new_item{new DoublyLinkedListItem<T>{value, _last_item, nullptr}};
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
    throw std::out_of_range{"Index out of range"};
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
    throw std::out_of_range{"Index out of range"};
}

template <typename T>
void DoublyLinkedList<T>::clear()
{
    while (_first_item)
    {
        DoublyLinkedListItem<T> *previous_item{_first_item};
        _first_item = _first_item->next;
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
    throw std::out_of_range{"Index out of range"};
}

template <typename T>
const T &DoublyLinkedList<T>::operator[](const std::uint32_t &index) const
{
    DoublyLinkedListItem<T> *current_item{_first_item};
    for (std::uint32_t i{}; current_item && i < index; ++i)
        current_item = current_item->next;
    if (current_item)
        return current_item->value;
    throw std::out_of_range{"Index out of range"};
}

template <typename T>
bool DoublyLinkedList<T>::contains(const T &value) const
{
    for (std::uint32_t i{}; i < _count; ++i)
        if ((*this)[i] == value)
            return true;
    return false;
}

template <typename T>
const std::uint32_t &DoublyLinkedList<T>::length() const
{
    return _count;
}

/********************************************************************/

#endif // _DOUBLY_LINKED_LIST_H_
