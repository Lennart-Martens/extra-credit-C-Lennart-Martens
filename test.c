#include <stdio.h>
#include <stdlib.h>
#include "hashmap.h"

void _add_words(struct hashmap* hm, char* document_id); // Adds all words from a document to a hashmap
void _print_hashmap(struct hashmap* hm); // Prints each word from a hashmap and its frequency

int main(void) {
    
    // Starter variables
    char * document_id;
    struct hashmap * hm = hm_create(16);
    
    // Add words from three files to hm
    document_id = "./D1.txt";
    _add_words(hm, document_id);
    document_id = "./D2.txt";
    _add_words(hm, document_id);
    document_id = "./D3.txt";
    _add_words(hm, document_id);
    
    // Check if num_occurrences is correct
    printf("The instance of 'homework' in D3: %d\n", hm_get(hm, "homework", "./D3.txt"));

    // Print each word from hm and its frequency
    _print_hashmap(hm);

    // Free hm
    hm_destroy(hm);
}

void _add_words(struct hashmap* hm, char* document_id) {
    FILE * document;
    char word[1024];

    if((document = fopen(document_id, "r")) == 0) {
        fprintf(stderr, "ERROR: %s not found\n", document_id);
        exit(1);
    }

    while(fscanf(document, "%s", word) != EOF) {
        hm_put(hm, word, document_id, hm_get(hm, word, document_id));
    }
    fclose(document);
}

void _print_hashmap(struct hashmap* hm) {
    printf("\n");
    printf("Hashmap of words and their frequencies:\n");

    for(int i = 0; i < hm->num_buckets; i ++) {
        if(i<10) printf(" ");
        printf("[%d]:", i);

        struct llnode * curr = hm->map[i];
        while(curr) {
            printf(" %s*%d", curr->word, curr->num_occurrences);
            curr = curr->next;
        }
        
        printf("\n");
    }
    printf("\n");
}