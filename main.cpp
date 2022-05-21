#include "hash_table.hpp"

#include <iostream>

int main(int argc, char const *argv[])
{
    HashTable ht{10};
    ht.insert(1, 11);
    ht.insert(11, 13);
    ht.insert(21, 14);
    ht.insert(2, 12);
    ht.insert(2, 22);
    ht.print_table();
    ht.remove(1);
    ht.insert(1, 15);
    ht.print_table();
    std::cout << ht[1].key << " " <<ht[1].value;
    return 0;
}
