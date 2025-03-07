#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include "character.h"
#include "board_layout.h"

#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

#define BOARD_SIZE 9
#define MAX_ROWS 100
#define MAX_STRING_LENGTH 100

typedef struct Node {
    int key;
    char* value;
    struct Node* next;
} Node;

typedef struct Board_HashTable {
    Node* table[BOARD_SIZE];
} Board_HashTable;

int hash(int key) {
    return key % BOARD_SIZE;
}

Board_HashTable* createTable() {
    Board_HashTable* ht = (Board_HashTable*)malloc(sizeof(Board_HashTable));
    if (ht == NULL) {
        fprintf(stderr, "Memory allocation failed for hash table\n");
        return NULL;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
        ht->table[i] = NULL;
    }
    return ht;
}

void insert(Board_HashTable* ht, int key, const char* value) {
    int index = hash(key);

    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed for new node\n");
        return;
    }

    newNode->value = (char*)malloc((strlen(value) + 1) * sizeof(char));
    if (newNode->value == NULL) {
        fprintf(stderr, "Memory allocation failed for value (string)\n");
        free(newNode);
        return;
    }
    strcpy(newNode->value, value);  // Copy the string into the allocated memory
    newNode->next = NULL;
    newNode->next = NULL;
    if (ht->table[index] == NULL) {
        ht->table[index] = newNode;
    } else {
        newNode->next = ht->table[index];
        ht->table[index] = newNode;  // Inserting at the front of the list
    }
}

void retrieveValues(Board_HashTable* ht, int key) {
    int index = hash(key);
    Node* current = ht->table[index];

    if (current == NULL) {
        printf("Couldn't find any value for key %d\n", key);
        return;
    }

    printf("Values for key %d: ", key);
    while (current != NULL) {
        printf("%s ", current->value);
        current = current->next;
    }
    printf("\n");
}

char** createTilesArr(int size) {
    char** strings = (char**)malloc(size * sizeof(char*));
    if (strings == NULL) {
        fprintf(stderr, "Memory allocation failed for string array\n");
        return NULL;
    }
    char* tiles[] = {"Treasure", "Place", "House", "Income Tax", "Airport", 
                      "Vacation", "Company", "Surprise", "Prison"};

    for (int i = 0; i < size; i++) {
        strings[i] = (char*)malloc((strlen(tiles[i]) + 1) * sizeof(char));
        if (strings[i] == NULL) {
            fprintf(stderr, "Memory allocation failed for string %d\n", i);
            return NULL; 
        }
        strcpy(strings[i], tiles[i]);  // Copy the string literal into the allocated memory
    }

    return strings;
}

void freeTilesArr(char** strings, int size) {
    for (int i = 0; i < size; i++) {
        free(strings[i]);  // Free each dynamically allocated string
    }
    free(strings);  // Free the array of string pointers
}



int main() {
    int choice;
    char** strings = createTilesArr(BOARD_SIZE);
    Board_HashTable* ht = createTable();
    if (strings == NULL) {
        return 1;
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
        insert(ht, i, strings[i]);
        //retrieveValues(ht, i);
    }
    if (ht == NULL) {
        return 1;  
    }
    //board_layout();
    pthread_t thread[6]; 
    pthread_create(&thread[0], NULL, get_Character_Details, NULL);
    pthread_create(&thread[1], NULL, getBoardAttr, NULL);
    pthread_create(&thread[1], NULL, displayBoard, NULL);
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_create(&thread[2], NULL, display_Character_Details, NULL);
    pthread_join(thread[2], NULL);
    printf(YELLOW "\n CHOOSE YOUR CHARACTER \n" RESET);
    scanf("%d", &choice);
    player_setup(choice);
    pthread_create(&thread[3], NULL, displayBoard, NULL);
    pthread_join(thread[3], NULL);

    exit(0);
    free_Character_Details();
    //getTilesAttr();
    //displayTiles();
    freeTilesMemory();
}

//cmd to execute script: gcc monopoly.c character.c bord_layout.c -o monopoly && ./monopoly