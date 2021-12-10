#include "HashTable.h"

unsigned long ht_string2int(char *str) {
    unsigned long hash = 5381;
    int c;

    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash;
}

/**
 * @brief uses and returns hash function to get a hash for dictionary
 * @param key of KVPair to be hashed
 * @param number of slots dictionary has (100)
 */
unsigned int ht_hash(char *key, unsigned int slots) {
   return ht_string2int(key)%slots;
}