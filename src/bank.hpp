/**
 * @file bank.cpp
 * @author Paolo Galli
 * @brief Implementation of the Bank class
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022 Paolo Galli
 *
 */

#ifndef _BANK_H_
#define _BANK_H_

#include <cstdint>

#include "doubly_linked_list.hpp"
#include "hash_table.hpp"

/********************************************************************/

enum class RequestType
{
    OpenAccount,
    CloseAccount,
    Deposit,
    Withdraw,
    Loan,
    None
};

/********************************************************************/

class Loan
{
public:
    Loan(const std::uint32_t &key, const std::uint64_t &loan_budget = 0) : key{key},
                                                                           loan_budget{loan_budget} {};
    bool operator==(const Loan &a) const { return key == a.key; };

    std::uint32_t key;
    std::uint64_t loan_budget;
};

/********************************************************************/

class NewOperation
{
public:
    NewOperation(const std::uint32_t &key, const RequestType &new_request_type = RequestType::None,
                 const std::uint64_t &new_request_budget = 0) : key{key},
                                                                new_request_type{new_request_type},
                                                                new_request_budget{new_request_budget} {};
    bool operator==(const NewOperation &a) const { return key == a.key; };

    std::uint32_t key;
    RequestType new_request_type;
    std::uint64_t new_request_budget;
};

/********************************************************************/

class Bank
{
public:
    Bank(const std::uint64_t &initial_bank_budget, const double &loan_interest = 0.1);
    ~Bank();

    void request(const std::uint32_t &key, const RequestType &request_type, const std::uint64_t &request_budget = 0);
    bool pending_request(const std::uint32_t &key) const;
    bool pending_loans() const;
    bool pending_operations() const;
    bool update_accounts_loans();
    bool update_accounts_operations();

    const std::uint64_t &get_bank_budget() const;
    const std::uint64_t &get_account_budget(const std::uint32_t &key) const;
    const HashTable<std::uint64_t> &get_accounts() const;
    bool exist(const std::uint32_t &key) const;

    const std::uint32_t &n_accounts() const;

private:
    bool _are_safe_loans() const;
    bool _are_safe_operations() const;

    std::uint64_t _bank_budget;
    double _loan_interest;

    HashTable<std::uint64_t> _accounts_budget;
    DoublyLinkedList<Loan> _new_loans;
    DoublyLinkedList<NewOperation> _new_operations;
};

Bank::Bank(const std::uint64_t &initial_bank_budget, const double &loan_interest) : _bank_budget{initial_bank_budget}, _loan_interest{loan_interest}
{
    if (_loan_interest < 0)
        throw std::runtime_error{"loan_interest must be >= 0"};
}

Bank::~Bank()
{
}

void Bank::request(const std::uint32_t &key, const RequestType &request_type, const std::uint64_t &request_budget)
{
    switch (request_type)
    {
    case RequestType::OpenAccount:
        _bank_budget += request_budget;
        _accounts_budget.insert(key, request_budget);
        break;
    case RequestType::Deposit:
        _bank_budget += request_budget;
        _accounts_budget[key] += request_budget;
        break;
    case RequestType::CloseAccount:
    case RequestType::Withdraw:
        _new_operations.append({key, request_type, request_budget});
        break;
    case RequestType::Loan:
        _new_loans.append({key, request_budget});
        break;
    }
}

bool Bank::pending_request(const std::uint32_t &key) const
{
    return _new_loans.contains({key}) || _new_operations.contains({key});
}

bool Bank::pending_loans() const
{
    return _new_loans.length();
}

bool Bank::pending_operations() const
{
    return _new_operations.length();
}

bool Bank::update_accounts_loans()
{
    // no pending operations
    if (!_new_loans.length())
        return true;

    auto is_safe{_are_safe_loans()};
    if (is_safe)
        for (int i{}; i < _new_loans.length(); ++i)
            _bank_budget += (_new_loans[i].loan_budget * _loan_interest);
    _new_loans.clear();
    return is_safe;
}

bool Bank::update_accounts_operations()
{
    // no pending operations
    if (!_new_operations.length())
        return true;

    auto is_safe{_are_safe_operations()};
    if (is_safe)
    {
        for (int i{}; i < _new_operations.length(); ++i)
        {
            _bank_budget -= _new_operations[i].new_request_budget;
            switch (_new_operations[i].new_request_type)
            {
            case RequestType::CloseAccount:
                _accounts_budget.remove(_new_operations[i].key);
                break;
            case RequestType::Withdraw:
                _accounts_budget[_new_operations[i].key] -= _new_operations[i].new_request_budget;
                break;
            }
        }
    }
    _new_operations.clear();
    return is_safe;
}

const std::uint64_t &Bank::get_bank_budget() const
{
    return _bank_budget;
}

const std::uint64_t &Bank::get_account_budget(const std::uint32_t &key) const
{
    return _accounts_budget[key];
}

const HashTable<std::uint64_t> &Bank::get_accounts() const
{
    return _accounts_budget;
}

bool Bank::exist(const std::uint32_t &key) const
{
    return _accounts_budget.contains(key);
}

const std::uint32_t &Bank::n_accounts() const
{
    return _accounts_budget.length();
}

bool Bank::_are_safe_loans() const
{
    /* NEEDED BUDGET FOR EACH LOAN
     *   need = max - alloc = loan_budget
     * WHERE:
     *   - max = loan_budget*(1+_loan_interest)
     *   - alloc = _loan_interest
     */

    // mark all loans as pending
    auto dispensed{new bool[_new_loans.length()]{false}};
    // copy current bank budget
    auto budget{_bank_budget};

    // while all loans are not dispensed or system is not in safe state
    std::uint32_t count{};
    bool is_safe{true};
    while (count < _new_loans.length())
    {
        // find a loan which is not dispensed and whose needs can be satisfied with current budget
        bool found{false};
        for (std::uint32_t i{}; i < _new_loans.length(); ++i)
        {
            // check if already dispensed and if all needs of i can be satisfied
            if (!dispensed[i] && _new_loans[i].loan_budget <= budget)
            {
                // add the allocated budget (aka the interest) to the available budget
                budget += (_new_loans[i].loan_budget * _loan_interest);
                // mark as dispensed
                dispensed[i] = true;
                found = true;
                ++count;
            }
        }
        // if not found, the state is unsafe
        if (!found)
        {
            is_safe = false;
            break;
        }
    }

    // delete allocated memory and return
    delete[] dispensed;
    return is_safe;
}

bool Bank::_are_safe_operations() const
{
    std::uint64_t need{};
    for (std::uint32_t i{}; i < _new_operations.length(); ++i)
        need += _new_operations[i].new_request_budget;
    return need <= _bank_budget;
}

/********************************************************************/

#endif // _BANK_H_
