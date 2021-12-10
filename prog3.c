/*
* Benjamin Chen
* CSE 101 Program 3
*  Nov 14, 2021
*/
#include "Dictionary.h"
#include "List.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
//This is used internally by the output function. You don't need to change this value
#define WORD_BUFFER_SIZE 50


/**
 * @brief Checks if a character is in the char array.
 * 
 * @param ch The character to check
 * @param charArr The ckaracter array to compare against
 * @return true ch is in charArr
 * @return false ch is not in charArr
 */
bool any_char(char ch, char *charArr) {
    while (*charArr != 0) {
        if (ch == *charArr++) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Gets the distance to the closest delimeter.
 * 
 * @param word The string to look through
 * @param delim The delimeter(s) to search for
 * @return uint8_t The number of characters in the word 
 */
uint8_t word_length(char *str, char *delims) {
    int i = 0;
    while (!any_char(str[i], delims) && str[i] != 0) {
        i++;
    }
    return i;
}

/**
 * @brief Prints the output to stdout. Accepts one line of output at a time.
 * 
 * @param wordFreqs A dictionary of words to format according to their frequency
 * @param wordLengths A dictionary of words to format according to their length
 * @param text One output line to format
 */
void printOutput(Dictionary *wordFreqs, Dictionary *wordLengths, char *text) {
    char wordBuffer[WORD_BUFFER_SIZE] = {0};

    while (*text != 0) {
        uint8_t length = word_length(text, " \n");
        assert(length < WORD_BUFFER_SIZE - 1);
        memcpy(wordBuffer, text, length);
        text = text + length;
        wordBuffer[length] = 0;
        KVPair *freq = dictionary_find(wordFreqs, wordBuffer);
        KVPair *len = dictionary_find(wordLengths, wordBuffer);
        if (freq == NULL && len == NULL) {
            printf("%s ", wordBuffer);
        } else {
            printf("<span style=\"");
            if (freq != NULL) {
                uint8_t r = (intptr_t)freq->value * 23;
                uint8_t g = (intptr_t)freq->value * 29;
                uint8_t b = (intptr_t)freq->value * 31;
                printf("color:#%02x%02x%02x;font-weight:bold;", r, g, b);
            }
            if (len != NULL) {
                printf("font-size:%ldpx;", 2 * (intptr_t)len->value);
            }
            printf("\">%s</span> ", wordBuffer);
        }

        if (*text != 0) { // If delimiter is not null byte, then skip over it. 
            text += 1;
        }
    }
    printf("<br/>\n");
}
/**
 * @brief frees the kvPair passed in
 * 
 * @param kvPair to be freed
 */
void freeData(KVPair *kvPair);

/**
 * @brief prints the kvPair passed in
 * 
 * @param kvPair to be printed
 */
void dataPrinter(KVPair* kv);

/**
 * @brief compares two KVPairs according to their value parameter, then alphabetical
 * @param kvPair to be compared
 * @param kvPair to be compared
 */
int dataCompares(KVPair *kv1, KVPair *kv2);

/**
 * @brief compares two KVPairs according to alphabetical order
 * @param kvPair to be compared
 * @param kvPair to be compared
 */
int dataCompares2(KVPair *kv1, KVPair *kv2);
int main(void) {
    char s[10000];
    //reads in input and stores in stopWords dictionary
    Dictionary* stopWords = dictionary_create(100,dataPrinter, freeData);
    while(scanf("%[^\n]%*c", s) != EOF){
        if(strcmp("====", s) == 0){ //checks for end
            break;
        }
        char *words = strtok(s, " ");
        while (words != NULL){
            KVPair* kv = (KVPair*)malloc(sizeof(KVPair));
            kv->key = malloc(sizeof(char) * 254);
            strcpy(kv->key, words); //stores input in kv
            kv->value = 0;
            dictionary_insert(stopWords, kv);//puts kv in stopWords dictionary
            words = strtok(NULL, " ");
        }
    }
    ListPtr *ls = list_create(dataCompares2, dataPrinter, freeData); //create new list to store input
    int integer = 0;
    Dictionary* wordFreq = dictionary_create(100, dataPrinter, freeData); //two new dictionary for word frequency and length.
    Dictionary* wordLengths = dictionary_create(100, dataPrinter, freeData);

    ListPtr* list1 = list_create(dataCompares2, dataPrinter, freeData);//create new list to store all inputs and sort them
    ListPtr* list2 = list_create(dataCompares, dataPrinter, freeData);//different dataCompares because different comparison needed for find element in list
    while(scanf("%[^\n]%*c", s) != EOF){
        KVPair *str = (KVPair*)malloc(sizeof(KVPair));
        str->key = malloc(sizeof(char) * 1000);
        strcpy(str->key, s);
        str->value = 0;
        list_insert(ls, integer, str); //store input as whole lines
        integer++;
        char *words = strtok(s, " ");
        while(words != NULL){
            KVPair *x = dictionary_find(stopWords, words);
            if(x == NULL){ //if word is not in stopWords
                KVPair* kv = (KVPair*)malloc(sizeof(KVPair));
                kv->key = malloc(sizeof(char) * 254);
                strcpy(kv->key, words);
                kv->value = 1;
                int y = list_find_element(list1, kv); //if kv is not a duplicate
                if(y == -1)
                    list_insert(list1, 0, kv); //store in list
                else{ //else increment value and free kv
                    KVPair *z = list_get_index(list1, y);
                    z->value++;
                    free(kv);
                }
                KVPair* temp = (KVPair*)malloc(sizeof(KVPair));
                temp->key = malloc(sizeof(char) * 254);
                strcpy(temp->key, words);
                temp-> value = strlen(words); //value for list2 is wordlength
                if(list_find_element(list2, temp) == -1){
                    list_insert(list2, 0, temp); //insert into list if not a duplicate
                }
                else{
                    free(temp);
                }
            }
            words = strtok(NULL, " ");
        }
    }
    ListPtr* list3 = list_create(dataCompares, dataPrinter, freeData); //create another list to sort 
    int i;
    for(i = 0; i < list_length(list1); i++){
        KVPair* kv = list_get_index(list1, i);//transfer list 1 to list 3
        list_insert(list3, i, kv);
    }
    list_sort(list2, false);//sort according to wordLength
    list_sort(list3, false);//sort according to frequency
    int counter = 0;
    while(list_get_index(list2, counter) != NULL){
        dictionary_insert(wordLengths, list_get_index(list2, counter)); //add elements to dictionary
        counter++;
    }
    while(list_get_index(list2, 20) != NULL){ //delete everything past 20th index for list and dic
        KVPair* kv = list_del_index(list2, 20);
        dictionary_delete(wordLengths,kv->key);
    }

    counter = 0;
    while(list_get_index(list3, counter) != NULL){ //add list to dictionary
        dictionary_insert(wordFreq, list_get_index(list3, counter));
        counter++;
    }
    while(list_get_index(list3, 25) != NULL){ //delete everything past 25th index.
        KVPair* kv = list_del_index(list3, 25);
        dictionary_delete(wordFreq, kv -> key);
    }

    counter = 0;
    while(list_get_index(ls, counter)!= NULL){ //call print output function with dictionaries
        KVPair *kv = list_get_index(ls, counter);
        printOutput(wordFreq, wordLengths,kv ->key);
        counter++;
    }
    printf("\n");
    dictionary_destroy(stopWords, true);//free all dictionaries and lists
    dictionary_destroy(wordFreq, true);
    dictionary_destroy(wordLengths, true);
    list_destroy(ls, true);
    list_destroy(list1, false);
    list_destroy(list2, false);
    list_destroy(list3, false);
}

void freeData(KVPair *kvPair){
    free(kvPair ->key);
    free(kvPair);
}
void dataPrinter(KVPair* kv){
    printf("%s %i\n", kv->key, (int)kv->value);
}

int dataCompares(KVPair *kv1, KVPair *kv2){
    if(kv1 -> value > kv2->value)
        return 1;
    if(kv1->value < kv2 -> value)
        return -1;
    return strcmp(kv1->key, kv2->key) * -1;
}
int dataCompares2(KVPair *kv1, KVPair *kv2){
    return strcmp(kv1->key, kv2->key);
}