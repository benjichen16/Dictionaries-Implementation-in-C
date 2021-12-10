#include "List.h"
#include "HashTable.h"
#include "Dictionary.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Dictionary {
int		slots;				// total number of slots in hash table
int		size;				// number of elements currently in dictionary
ListPtr  *hash_table;	    // hash_table is array of ListPtrs
} Dictionary;			// dictionary is maintained as a hash table.

//comparision function that sorts by alphabetical order

int dataCompare(KVPair *value1, KVPair *value2){
        return strcmp(value1 ->key, value2 ->key);
}
//constructor for dictionary
Dictionary *dictionary_create(int hash_table_size, void (*dataPrinter)(void *data), void (*freeData)(KVPair *kvPair)){
    Dictionary *d = (Dictionary*) malloc(sizeof(Dictionary));
    d-> size = 0;
    d-> slots = hash_table_size;
    d->hash_table = malloc(hash_table_size* sizeof(ListPtr));
    int i;
    for(i = 0; i < hash_table_size; i++){ //malloc all lists with proper function
        d->hash_table[i] = list_create(dataCompare, dataPrinter, freeData);
    }
    return d;
}

//free all lists and dictionaries that are passed in via parameters

void dictionary_destroy(Dictionary *d, bool freeData){
    if(freeData == true){
        int i;
        for(i = 0; i < d-> slots; i++)
                list_destroy(d->hash_table[i], true);
    }
    free(d);
}

/**
 * @brief  inserts a KVPair into the dictionary
 * @param dictionary
 * @param KVPair that is going to be added
 */
bool dictionary_insert(Dictionary *D, KVPair *elem){
    int k = ht_hash(elem -> key, D->slots);
    bool x;
    if(list_find_element(D->hash_table[k], elem) == -1){ //checks for dups, if there is a dup, return false
        x = list_insert(D->hash_table[k], 0, elem);
        D->size++;
        return true;}
    else
        return false;
}

/**
 * @brief deletes a dictionary element according to the given key
 * @param dictionary that is going to have key deleted
 * @param key of KVPair to be deleted
 */
KVPair *dictionary_delete(Dictionary *D, char *key){
    int k = ht_hash(key, D->slots);
    KVPair *x = (KVPair*)malloc(sizeof(KVPair));
    x->key = key;
    x->value = 0;
    int index = list_find_element(D->hash_table[k], x); //finds index of KVPair in dictionary list
    if(index == -1){
        return NULL; //if there is no match, return null
    }
    KVPair *y = list_del_index(D->hash_table[k], index); //deletes KVPair in dictionary
    D->size--;
    free(x);
    return y;
}

/**
 * @brief returns KVPair that is found according to key, NULL if not in dic
 * @param Dictionary that key is in
 * @param key of kvPair to be found
 */
KVPair *dictionary_find(Dictionary *D, char *key){
    int k = ht_hash(key, D->slots);
    KVPair *x = (KVPair*)malloc(sizeof(KVPair));
    x->key = key;
    x->value = 0;
    int y = list_find_element(D->hash_table[k], x);//gets index of KVPair in dictionary list
    if(y == -1){
        return NULL;
    }
    free(x);
    return list_get_index(D->hash_table[k], y);
}

/**
 * @brief prints contents of Dictionary
 * @param Dictionary to be printed
 */
void dictionary_print(Dictionary *D){
    int i;
    for(i = 0; i < D->slots; i++){
        list_print(D->hash_table[i]);//pritns every list of dictionary
    }
}
/**
 * @brief returns dictionary size
 * @param Dictionary with a certain size
 */
int dictionary_size(Dictionary *D){
    return D->size;
}