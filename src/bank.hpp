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

/**
 * @brief Type of the required action to the bank.
 */
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

/**
 * @brief NewLoan class, to store all the informations about a loan request.
 */
class NewLoan
{
public:
    /**
     * @brief Construct a new NewLoan object.
     *
     * @param key Key of the account that require a loan.
     * @param loan_budget Value of the requested loan.
     */
    NewLoan(const std::uint32_t &key, const std::uint64_t &loan_budget = 0) : key{key},
                                                                              loan_budget{loan_budget} {};

    /**
     * @brief Destroy the NewLoan object.
     */
    ~NewLoan(){};

    /**
     * @brief Compare two NewLoan objects.
     *
     * @param a The second object.
     * @return `true` if the key is equal, `false` otherwise.
     */
    bool operator==(const NewLoan &a) const { return key == a.key; };

    /**
     * @brief Key of the account that require a loan.
     */
    std::uint32_t key;

    /**
     * @brief Value of the requested loan.
     */
    std::uint64_t loan_budget;
};

/********************************************************************/

/**
 * @brief NewOperation class, to store all the informations about an operation request.
 *
 */
class NewOperation
{
public:
    /**
     * @brief Construct a new NewOperation object.
     *
     * @param key Key of the account that requested an operation.
     * @param new_request_type Type of the requested operation.
     * @param new_request_budget Budget of the requested operation.
     */
    NewOperation(const std::uint32_t &key, const RequestType &new_request_type = RequestType::None,
                 const std::uint64_t &new_request_budget = 0) : key{key},
                                                                new_request_type{new_request_type},
                                                                new_request_budget{new_request_budget} {};

    /**
     * @brief Destroy the NewOperation object.
     */
    ~NewOperation(){};

    /**
     * @brief Compare two NewOperation objects.
     *
     * @param a The second object.
     * @return `true` if the key is equal, `false` otherwise.
     */
    bool operator==(const NewOperation &a) const { return key == a.key; };

    /**
     * @brief Key of the account that requested an operation.
     */
    std::uint32_t key;

    /**
     * @brief Type of the requested operation.
     */
    RequestType new_request_type;

    /**
     * @brief Budget of the requested operation.
     */
    std::uint64_t new_request_budget;
};

/********************************************************************/

/**
 * @brief
 *
 */
class Bank
{
public:
    /**
     * @brief Construct a new Bank object.
     *
     * @param initial_bank_budget Initial bank budget.
     * @param loan_interest Loan interest factor (in favor of the bank).
     * @param deposit_interest Deposit interest factor (in favor of the depositors).
     */
    Bank(const std::uint64_t &initial_bank_budget, const double &loan_interest = 0.1, const double &deposit_interest = 0.005);

    /**
     * @brief Destroy the Bank object.
     */
    ~Bank();

    /**
     * @brief Request a new action to the bank.
     *
     * @param key Key of the account that requested an action.
     * @param request_type Type of the requested action.
     * @param request_budget Budget of the requested action.
     */
    void request(const std::uint32_t &key, const RequestType &request_type, const std::uint64_t &request_budget = 0);

    /**
     * @brief Check if an account has any pending loan or operation.
     *
     * @param key Key of the account to check.
     * @return `true` if the account has any pending action, `false` otherwise.
     */
    bool pending_request(const std::uint32_t &key) const;

    /**
     * @brief Check for any pending loan.
     *
     * @return `true` if there is at least a pending loan, `false` otherwise.
     */
    bool pending_loans() const;

    /**
     * @brief Check for any pending operation.
     *
     * @return `true` if there is at least a pending operation, `false` otherwise.
     */
    bool pending_operations() const;

    /**
     * @brief Check if required loans are safe and, if yes, dispense them.
     *
     * @return `true` if all the loans are dispensed, `false` otherwise.
     */
    bool update_accounts_loans();

    /**
     * @brief Check if required operations are safe and, if yes, dispense them.
     *
     * @return `true` if all the operations are dispensed, `false` otherwise.
     */
    bool update_accounts_operations();

    /**
     * @brief Update accounts budget by adding the due interests.
     */
    void update_interests();

    /**
     * @brief Get the current bank budget.
     *
     * @return The current bank budget.
     */
    const std::uint64_t &get_bank_budget() const;

    /**
     * @brief Get the budget of the given account.
     *
     * @param key The key of the account to find.
     * @return The budget of the given account.
     */
    const std::uint64_t &get_account_budget(const std::uint32_t &key) const;

    /**
     * @brief Get the accounts budget table.
     *
     * @return The accounts budget table.
     */
    const HashTable<std::uint64_t> &get_accounts() const;

    /**
     * @brief Check if an account with the given key exist in the bank.
     *
     * @param key The key of the account to find.
     * @return `true` if there is such an element, otherwise `false`.
     */
    bool exist(const std::uint32_t &key) const;

    /**
     * @brief Get the number of accounts in the bank.
     *
     * @return The number of accounts.
     */
    const std::uint32_t &n_accounts() const;

private:
    /**
     * @brief Check if all new requested loans are safe.
     *
     * @return `true` if safe, otherwise `false`.
     */
    bool _are_safe_loans() const;

    /**
     * @brief Check if all new requested operations are safe.
     *
     * @return `true` if safe, otherwise `false`.
     */
    bool _are_safe_operations() const;

    /**
     * @brief Bank budget for all operations.
     */
    std::uint64_t _bank_budget;

    /**
     * @brief Loan interest factor (in favor of the bank).
     */
    double _loan_interest;

    /**
     * @brief Deposit interest factor (in favor of the depositors).
     */
    double _deposit_interest;

    /**
     * @brief Table of the accounts budgets.
     */
    HashTable<std::uint64_t> _accounts_budget;

    /**
     * @brief List of new loan requests.
     */
    DoublyLinkedList<NewLoan> _new_loans;

    /**
     * @brief List of new operations requests.
     */
    DoublyLinkedList<NewOperation> _new_operations;
};

Bank::Bank(const std::uint64_t &initial_bank_budget, const double &loan_interest,
           const double &deposit_interest) : _bank_budget{initial_bank_budget},
                                             _loan_interest{loan_interest},
                                             _deposit_interest{deposit_interest}
{
    // check if all arguments are valid
    if (_loan_interest < 0)
        throw std::invalid_argument{"loan_interest must be >= 0"};
    if (_deposit_interest < 0)
        throw std::invalid_argument{"_deposit_interest must be >= 0"};
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
    // check if pending operations
    if (!_new_loans.length())
        return true;
    // if yes, execute
    auto is_safe{_are_safe_loans()};
    if (is_safe)
        for (std::uint32_t i{}; i < _new_loans.length(); ++i)
            _bank_budget += (_new_loans[i].loan_budget * _loan_interest);
    _new_loans.clear();
    return is_safe;
}

bool Bank::update_accounts_operations()
{
    // check if pending operations
    if (!_new_operations.length())
        return true;
    // if yes, execute
    auto is_safe{_are_safe_operations()};
    if (is_safe)
    {
        for (std::uint32_t i{}; i < _new_operations.length(); ++i)
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

void Bank::update_interests()
{
    for (std::uint32_t i{}; i < _accounts_budget.length(); ++i)
        _accounts_budget[_accounts_budget.keys()[i]] *= (1 + _deposit_interest);
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
    while (count < _new_loans.length() && is_safe)
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
