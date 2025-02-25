#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

typedef struct Node{
    int value;
    struct Node* next;
} Node;

typedef struct HashTable {
    Node* table[TABLE_SIZE]; 
} HashTable;

int hash(int key){
    return key % TABLE_SIZE;
}

HashTable* createTable(){
    HashTable* ht = (HashTable*)malloc(sizeof(HashTable));
    for(int i=0; i<TABLE_SIZE; i++){
        ht-> table[i] = NULL; //initalise each bucket as null -> empty linked list
    }
    return ht;
}

void insert(HashTable* ht, int key, int value){
    int index = hash(key);
    Node *newNode = (Node*)malloc(sizeof(Node));
    newNode->value = value;
    newNode->next = NULL;
    
    // Insert the new node at the beginning of the linked list at index
    if (ht->table[index] == NULL) {
        ht->table[index] = newNode;  // No list, just insert as first node
    } else {
        newNode->next = ht->table[index];  // Insert at the front of the list
        ht->table[index] = newNode;
    }


}

// Function to retrieve and print all values for a given key
void getValues(HashTable* ht, int key) {
    int index = hash(key);
    Node* current = ht->table[index];
    
    if (current == NULL) {
        printf("No values found for key %d\n", key);
        return;
    }
    
    printf("Values for key %d: ", key);
    while (current != NULL) {
        printf("%d ", current->value);
        current = current->next;
    }
    printf("\n");
}

int main(){
    HashTable* ht = createTable();

    insert(ht, 1, 10);
    insert(ht, 1, 20);
    insert(ht, 1, 30);
    insert(ht, 2, 40);
    insert(ht, 3, 50);
    insert(ht, 3, 60);
    insert(ht, 10, 378);
    insert(ht, 44, 26);

    // Retrieve values for keys
    getValues(ht, 1);  
    getValues(ht, 2);  
    getValues(ht, 3);  
    getValues(ht, 44);
    getValues(ht, 10);

    return 0;

}