#include <string.h>
#include <stdlib.h>
#include "hashmap.h"

struct hashmap* hm_create(int num_buckets) {

    // Allocate hashmap
    struct hashmap * hm = calloc(1, sizeof(struct hashmap));

    // Set num_buckets
    hm->num_buckets = num_buckets;

    // Allocate map
    hm->map = calloc(num_buckets, sizeof(struct llnode*));

    // Return pointer
    return hm;
}

int hm_get(struct hashmap* hm, char* word, char* document_id) {

    // Find num_occurrences of word from document_id in hm if it exists
    struct llnode * curr = hm->map[hash(hm, word, document_id)];
    while(curr) {
        if(strcmp(curr->word, word) == 0 && strcmp(curr->document_id, document_id) == 0) {
            return curr->num_occurrences;
        }
        curr = curr->next;
    }

    // Else return -1
    return -1;
}

void hm_put(struct hashmap* hm, char* word, char* document_id, int num_occurrences) {

    // Find correct bucket
    int bucket = hash(hm, word, document_id);
    struct llnode * curr = hm->map[bucket];

    // Add word and document_id to hm or increment num_occurrences
    if(num_occurrences == -1) {
        struct llnode * new_node = malloc(sizeof(struct llnode));
        new_node->word = strdup(word);
        new_node->document_id = strdup(document_id);
        new_node->num_occurrences = 1;
        new_node-> next = hm->map[bucket];

        hm->map[bucket] = new_node;
        hm->num_elements++;
    } 
    
    else {
        while(curr) {
            if(strcmp(curr->word, word) == 0 && strcmp(curr->document_id, document_id) == 0) {
                curr->num_occurrences++;
                return;
            }
            curr = curr->next;
        }
    }
}

void hm_remove(struct hashmap* hm, char* word, char* document_id) {

    // Find the previous and target node
    struct llnode * curr = hm->map[hash(hm, word, document_id)];
    struct llnode * prev = 0;
    while(curr) {
        if(strcmp(curr->word, word) == 0 && strcmp(curr->document_id, document_id) == 0) {
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    // Cut target node out of the bucket
    if(prev) {
        prev->next = curr->next;
    } else {
        hm->map[hash(hm, word, document_id)] = curr->next;
    }

    // Deallocate target node and update hm->num_elements
    free(curr->word);
    free(curr->document_id);
    free(curr);
    hm->num_elements--;
}

void hm_destroy(struct hashmap* hm) {

    // Deallocate llnodes
    for(int i = 0; i < hm->num_buckets; i++) {
        struct llnode * curr = hm->map[i];
        struct llnode * prev;
        while(curr) {
            prev = curr;
            curr = curr->next;
            free(prev->word);
            free(prev->document_id);
            free(prev);
        }
    }

    // Deallocate map
    free(hm->map);

    // Deallocate hashmap
    free(hm);
}

int hash(struct hashmap* hm, char* word, char* document_id) {

    // Uses ASCII values of word & document_id to find a bucket
    int sum = 0;
    for(int i = 0; i < (int)strlen(word); i++) {
        sum += word[i];
    }
    for(int i = 0; i < (int)strlen(document_id); i++) {
        sum += document_id[i];
    }
    return sum % hm->num_buckets;
}