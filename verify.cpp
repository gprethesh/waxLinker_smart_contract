#include <eosio/eosio.hpp>
#include <eosio/print.hpp>
#include <string>
#include <eosio/crypto.hpp>
#include <eosio/transaction.hpp>

using namespace eosio;

class [[eosio::contract("verify")]] verify : public eosio::contract {
public:
    using contract::contract;

    [[eosio::action]]
    void addinfo(eosio::name user_id, std::string twitterId) {
        require_auth(get_self());

        check(!twitterId.empty(), "Error: Twitter Id is empty.");

        // Check if user_id exists in listtab table
        list_table lists(get_self(), get_self().value);
        auto wallet_entry = lists.find(user_id.value);
        check(wallet_entry != lists.end(), "Please verify your wallet id.");

        verify_table datas(get_self(), get_self().value);
        auto existing_entry = datas.find(user_id.value);

        // Get the index by hash value
        auto hashValue_index = datas.get_index<"hashvalue"_n>();
        uint64_t hash_value = hash_string(twitterId);
        auto hashValue_itr = hashValue_index.find(hash_value);
        check(hashValue_itr == hashValue_index.end(), "Error: Twitter Id already exists.");

        if (existing_entry == datas.end()) {
            datas.emplace(get_self(), [&](auto &row) {
                row.user_id = user_id;
                row.twitterId = twitterId;
                row.hashValue = hash_value;
            });
        } else {
            datas.modify(existing_entry, get_self(), [&](auto &row) {
                row.twitterId = twitterId;
                row.hashValue = hash_value;
            });
        }
    }

    
    
    
    [[eosio::action]]
    void addwallet(eosio::name user) {
        require_auth(user); // Ensuring the caller is authorized

        list_table lists(get_self(), get_self().value);
        auto existing_entry = lists.find(user.value);

        check(existing_entry == lists.end(), "Error: Wallet ID already exists.");

        lists.emplace(user, [&](auto &row) {
            row.wallet_id = user; // Storing the caller's account name as the wallet ID
        });
    }


    [[eosio::action]]
    void removeinfo(eosio::name user_id) {
        require_auth(get_self());

        verify_table datas(get_self(), get_self().value);
        auto existing_entry = datas.find(user_id.value);

        check(existing_entry != datas.end(), "Error: User not found.");
        datas.erase(existing_entry);
    }
    
    [[eosio::action]]
    void clearlist() {
        require_auth(get_self()); // Only the contract owner can call this action

        list_table lists(get_self(), get_self().value);

        // Iterate through the table and erase all entries
        auto itr = lists.begin();
        while (itr != lists.end()) {
            itr = lists.erase(itr);
        }
    }
    
    
    [[eosio::action]]
    void cleartab() {
        require_auth(get_self()); // Only the contract owner can call this action

        verify_table datas(get_self(), get_self().value);

        // Iterate through the table and erase all entries
        auto itr = datas.begin();
        while (itr != datas.end()) {
            itr = datas.erase(itr);
        }
    }



    [[eosio::action]]
    void hasentry(eosio::name user_id) {
        verify_table datas(get_self(), get_self().value);
        auto existing_entry = datas.find(user_id.value);

        if (existing_entry != datas.end() && !existing_entry->twitterId.empty()) {
            eosio::print("1");
        } else {
            eosio::print("0");
        }
    }

private:
    struct [[eosio::table]] verifytab {
        eosio::name user_id; // Changed to eosio::name
        std::string twitterId;
        uint64_t hashValue;
        uint64_t primary_key() const {
            return user_id.value;
        }
        uint64_t by_hashvalue() const {
            return hashValue;
        }
    };

    static uint64_t hash_string(const std::string& str) {
        auto hash_result = eosio::sha256(str.data(), str.size());
        uint64_t hash_value = 0;
        memcpy(&hash_value, hash_result.extract_as_byte_array().data(), 8);
        return hash_value;
    }


    using verify_table = eosio::multi_index<"verifytab"_n, verifytab,
                                            eosio::indexed_by<"hashvalue"_n, eosio::const_mem_fun<verifytab, uint64_t, &verifytab::by_hashvalue>>
                                           >;

    struct [[eosio::table]] listtab {
            eosio::name wallet_id;
            uint64_t primary_key() const {
                return wallet_id.value;
            }
        };

        using list_table = eosio::multi_index<"listtab"_n, listtab>;

        // Function to get the sender's account name
        eosio::name get_sender() {
            auto sender = get_first_receiver(); // This returns the sender's account name
            return sender;
        }
};


