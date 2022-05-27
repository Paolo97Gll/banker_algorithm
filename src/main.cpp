/**
 * @file main.cpp
 * @author Paolo Galli
 * @brief Simple bank simulator using the banker algorithm
 * @version 0.1
 * @date 2022-05-15
 *
 * @copyright Copyright (c) 2022 Paolo Galli
 *
 */

#include <chrono>
#include <iostream>
#include <random>
#include <thread>

#include "bank.hpp"

/********************************************************************/

Bank bank{0, 0.05};

/********************************************************************/

template <typename T>
void log_message(T t)
{
    std::clog << t << std::endl;
}

template <typename T, typename... Args>
void log_message(T t, Args... args)
{
    std::clog << t << " ";
    log_message(args...);
}

/********************************************************************/

void handle_accounts(const unsigned int &n_steps, const unsigned int &sleep_ms)
{
    // SETUP RANDOM NUMBER GENERATOR
    std::mt19937_64 rng{};
    rng.seed(1);

    // SETUP DISTRIBUTIONS
    // uniform (uniform real distribution between 0 and 1)
    std::uniform_real_distribution<double> rand_uniform{0, 1};
    // request type generator (uniform integer distribution between RequestType::Deposit, RequestType::Withdraw, RequestType::Loan)
    std::uniform_int_distribution<int> _request_type_generator{2, 4};
    auto request_type_generator{
        [&](std::mt19937_64 rng)
        {
            return static_cast<RequestType>(_request_type_generator(rng));
        }};
    // new key generator (uniform integer distribution between 0 and MAX_UINT32_T)
    std::uniform_int_distribution<std::uint32_t> new_account_key_generator{};
    // remove key generator (uniform integer distribution between 0 and bank.n_accounts() - 1)
    auto remove_account_key_generator{
        [&](std::mt19937_64 rng)
        {
            std::uniform_int_distribution<std::uint32_t> _key_extractor{0, bank.n_accounts() - 1};
            return _key_extractor(rng);
        }};
    // open and deposit budget generator (exponential distribution)
    std::exponential_distribution<double> _budget_generator{0.0001};
    auto budget_generator{
        [&](std::mt19937_64 rng)
        {
            return static_cast<std::uint64_t>(_budget_generator(rng));
        }};
    // loan budget generator (exponential distribution)
    std::exponential_distribution<double> _loan_budget_generator{0.00001};
    auto loan_budget_generator{
        [&](std::mt19937_64 rng)
        {
            return static_cast<std::uint64_t>(_loan_budget_generator(rng));
        }};
    // withdraw budget generator (uniform integer distribution between 0 and account_budget)
    auto withdraw_budget_generator{
        [&](std::mt19937_64 rng, std::uint32_t key)
        {
            std::uniform_int_distribution<std::uint64_t> _withdraw_budget_generator{0, bank.get_account_budget(key)};
            return _withdraw_budget_generator(rng);
        }};

    // START LOOP
    int l_a{}, l_r{}, o_a{}, o_r{};
    log_message("[handle_accounts]", "Starting loop");
    for (unsigned int i{1}; i <= n_steps; ++i)
    {
        log_message("\n[handle_accounts]", "[", i, "] Starting epoch", i, "with bank budget", bank.get_bank_budget());

        // GENERATE A NEW ACCOUNT
        if (rand_uniform(rng) < 0.15)
        {
            std::uint32_t key{};
            do
                key = new_account_key_generator(rng);
            while (bank.exist(key));
            const auto budget{budget_generator(rng)};
            bank.request(key, RequestType::OpenAccount, budget);
            log_message("[handle_accounts]", "[", i, "]", "Operation requested: new account with key", key, "and budget", budget);
        }

        // REMOVE AN EXISTING ACCOUNT
        if (bank.n_accounts() > 0 && rand_uniform(rng) < 0.1)
        {
            const auto key{bank.get_accounts().keys()[remove_account_key_generator(rng)]};
            const auto budget{bank.get_account_budget(key)};
            bank.request(key, RequestType::CloseAccount, budget);
            log_message("[handle_accounts]", "[", i, "]", "Operation requested: remove account with key", key, "and budget", budget);
        }

        // HANDLE EXISTING ACCOUNTS
        for (std::uint32_t j{}; j < bank.n_accounts(); ++j)
        {
            auto key{bank.get_accounts().keys()[j]};
            if (!bank.pending_request(key) && rand_uniform(rng) < 0.5)
            {
                switch (request_type_generator(rng))
                {
                case RequestType::Deposit:
                {
                    const auto budget{budget_generator(rng)};
                    bank.request(key, RequestType::Deposit, budget);
                    log_message("[handle_accounts]", "[", i, "]", "Operation requested: deposit with key", key, "and budget", budget);
                    break;
                }
                case RequestType::Withdraw:
                {
                    const auto budget{withdraw_budget_generator(rng, key)};
                    bank.request(key, RequestType::Withdraw, budget);
                    log_message("[handle_accounts]", "[", i, "]", "Operation requested: withdraw with key", key, "and budget", budget);
                    break;
                }
                case RequestType::Loan:
                {
                    const auto budget{loan_budget_generator(rng)};
                    bank.request(key, RequestType::Loan, budget);
                    log_message("[handle_accounts]", "[", i, "]", "Operation requested: loan with key", key, "and budget", budget);
                    break;
                }
                }
            }
        }

        // EXECUTE OPERATIONS
        if (bank.pending_loans())
        {
            bool result{bank.update_accounts_loans()};
            log_message("[handle_accounts]", "[", i, "]", "RESULT: loans", result ? "accepted" : "rejected");
            ++(result ? l_a : l_r);
        }
        if (bank.pending_operations())
        {
            bool result{bank.update_accounts_operations()};
            log_message("[handle_accounts]", "[", i, "]", "RESULT: operations", result ? "accepted" : "rejected");
            ++(result ? o_a : o_r);
        }

        // END (AND SLEEP)
        if (sleep_ms)
            std::this_thread::sleep_for(std::chrono::milliseconds{sleep_ms});
    }

    log_message("\nFINAL: loans (a r)", l_a, l_r, "| operations (a r)", o_a, o_r);
}

/********************************************************************/

int main(int argc, char const *argv[])
{
    std::thread thread_handle_accounts{handle_accounts, 1000, 0};
    thread_handle_accounts.join();

    return 0;
}