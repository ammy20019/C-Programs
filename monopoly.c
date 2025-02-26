#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

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

char** treasure = NULL;
char** place = NULL;
char** house = NULL;
char** income_tax = NULL;
char** airport = NULL;
double* t_amt;
double* p_amt;
double* h_amt;
double* a_amt;

int row = 0;
int i_trav = 0;

void getTilesAttr() {
    FILE* fp = fopen("data.csv", "r");

    if (!fp) {
        printf("Can't open file\n");
        return;
    } else {
        // Allocate memory for each array to store column values for 100 rows
        treasure = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        place = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        house = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        income_tax = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        airport = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        t_amt = (double*)malloc(sizeof(double) * MAX_ROWS);
        p_amt = (double*)malloc(sizeof(double) * MAX_ROWS);
        h_amt = (double*)malloc(sizeof(double) * MAX_ROWS);
        a_amt = (double*)malloc(sizeof(double) * MAX_ROWS);

        char buffer[1024]; // buffer to store each line of the CSV
        while (fgets(buffer, sizeof(buffer), fp)) {
            row++;
            if (row == 1) {
                continue; // Skip the header row
            }

            // Tokenize each row
            char* value = strtok(buffer, ",");
            int column = 0;

            // Parse the columns and store values into appropriate arrays
            while (value) {
                // Trim whitespace if necessary
                value = value + strspn(value, " \t");
                for (int i = strlen(value) - 1; i >= 0; i--) {
                    if (value[i] == ' ' || value[i] == '\t') {
                        value[i] = '\0';  // Remove trailing spaces
                    } else {
                        break;
                    }
                }

                if (column == 0) {
                    treasure[i_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (treasure[i_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(treasure[i_trav], value);
                } else if (column == 1) {
                    place[i_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (place[i_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(place[i_trav], value);
                } else if (column == 2) {
                    house[i_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (house[i_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(house[i_trav], value);
                } else if (column == 3) {
                    income_tax[i_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (income_tax[i_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(income_tax[i_trav], value);
                } else if (column == 4) {
                    airport[i_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (airport[i_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(airport[i_trav], value);
                } else if (column == 5) {
                    t_amt[i_trav] = atof(value);
                } else if (column == 6) {
                    p_amt[i_trav] = atof(value);
                } else if (column == 7) {
                    h_amt[i_trav] = atof(value);
                } else if (column == 8) {
                    a_amt[i_trav] = atof(value);
                }
                // Move to the next token
                value = strtok(NULL, ",");
                column++;
            }

            // Move to the next row
            i_trav++;  
        }
        fclose(fp);
    }
}

void displayTiles() {
    printf("Treasure Data:\n");
    for (int i = 0; i < i_trav; i++) {
        printf("Row %d:\n", i + 1);
        printf("Treasure: %s\n", treasure[i]);
        printf("Place: %s\n", place[i]);
        printf("House: %s\n", house[i]);
        printf("Income Tax: %s\n", income_tax[i]);
        printf("Airport: %s\n", airport[i]);
        printf("Treasure Amt: %.2f\n", t_amt[i]);
        printf("Place Amt: %.2f\n", p_amt[i]);
        printf("House Amt: %.2f\n", h_amt[i]);
        printf("Airport Amt: %.2f\n", a_amt[i]);
        printf("\n");
    }
}

void freeTilesMemory(){
    for (int i = 0; i < i_trav; i++) {
        free(treasure[i]);
        free(place[i]);
        free(house[i]);
        free(income_tax[i]);
        free(airport[i]);
    }
    free(treasure);
    free(place);
    free(house);
    free(income_tax);
    free(airport);
    free(t_amt);
    free(p_amt);
    free(h_amt);
}

char** character = NULL;
char** name = NULL;
char** sp_ability = NULL;
char** backstory = NULL;
char** personality = NULL;
char** nick_name = NULL;
char* selected_player;
char* selected_personality;
char* selected_speciality;
double money = 10000.00;

int char_row = 0;
int char_trav = 0;

void get_Character_Details() {
    FILE* fp = fopen("characters.csv", "r");

    if (!fp) {
        printf("Can't open file\n");
        return;
    } else {
        // Allocate memory for each array to store column values for 100 rows
        character = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        name = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        sp_ability = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        backstory = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        personality = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        nick_name = (char**)malloc(sizeof(char*) * MAX_ROWS); 

        char buffer[1024]; // buffer to store each line of the CSV
        while (fgets(buffer, sizeof(buffer), fp)) {
            char_row++;
            if (char_row == 1) {
                continue; // Skip the header row
            }

            // Tokenize each row
            char* value = strtok(buffer, ",");
            int column = 0;

            // Parse the columns and store values into appropriate arrays
            while (value) {
                // Trim whitespace if necessary
                value = value + strspn(value, " \t");
                for (int i = strlen(value) - 1; i >= 0; i--) {
                    if (value[i] == ' ' || value[i] == '\t') {
                        value[i] = '\0';  // Remove trailing spaces
                    } else {
                        break;
                    }
                }

                if (column == 0) {
                    character[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (character[char_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(character[char_trav], value);
                } else if (column == 1) {
                    char name1[250], name2[250], full_name[500];
                    name[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    nick_name[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (name[char_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    sscanf(value, "%s %*s %s", name1, name2);
                    snprintf(full_name, sizeof(full_name), "%s %s", name1, name2);
                    strcpy(name[char_trav], full_name);
                    sscanf(value,  "%*s \"%[^\"]\"", nick_name[char_trav]); ///extracting nick name
                } else if (column == 2) {
                    sp_ability[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (sp_ability[char_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(sp_ability[char_trav], value);
                } else if (column == 3) {
                    backstory[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (backstory[char_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(backstory[char_trav], value);
                } else if (column == 4) {
                    personality[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (personality[char_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(personality[char_trav], value);
                }
                // Move to the next token
                value = strtok(NULL, ",");
                column++;
            }
            char_trav++;   //next row
        }
        fclose(fp);
    }
}

void display_Character_Details() {
    printf("[[[[[[[[[[[[[[[[[[[[[[[[[  CHARACTER LOBBY  ]]]]]]]]]]]]]]]]]]]]]]]]]]]\n");
    for (int i = 0; i < char_trav; i++) {
        char ch;
        printf(YELLOW "+----------------------------------------------------+\n");
        printf("| %-2d |CHARACTER: %-35s |\n" RESET, i+1, character[i]); 
        printf("+----------------------------------------------------+\n" RESET);
        printf("|NAME: %-18s [][][][] SHORT NAME: %-13s \n", name[i], nick_name[i]); 
        printf("+----------------------------------------------------+\n");
        printf("|PERSONA: %s \n", personality[i]);  
        printf("+----------------------------------------------------+\n");
        printf(GREEN "|SPECIAL ABILITY: %-60s \n" RESET, sp_ability[i]);  
        printf("+----------------------------------------------------+\n");
        printf("|BACKSTORY: %s \n", backstory[i]);  
        printf("+----------------------------------------------------+\n" );
        // printf("| %-26s |\n", sp_ability[i]);  // Description
        // printf("+----------------------------+\n");
        printf("\n");
        printf("PRESS ENTER TO VIEW OTHER CHARACTER \n");
        ch = getchar();
        if (ch == '\n') {
            printf("[][] Loading character details [][] \n");
            sleep(2);
            system("clear");
        } else {
            printf("You didn't press Enter!\n");
            sleep(10);
        }
    }
}

void Confirmed_character(char* nickname,char* personality,char* speciality){
    system("clear");
    printf(YELLOW "[_] Hey %s, Welcome to Switzerland! \n\n[_] I'm Mia, I will be assisting towards your journey in Switzerland \n\n[_] You have Rs %.2f with you in cash\n\n[_] Surviving and living your life the fullest is your goal\n\n[_] Each decision you make will either make you a millionare or bankrupt\n\n" RESET, nickname, money);
    selected_player = nickname;
    selected_personality = personality;
    selected_speciality = speciality;
}

void player_setup(int choice){
    printf("You have selected Character: %d \n", choice);
    printf(YELLOW "+----------------------------------------------------+\n");
    printf("| %-2d |CHARACTER: %-35s |\n" RESET, choice, character[choice-1]); 
    printf("+----------------------------------------------------+\n" RESET);
    printf("|NAME: %-18s [][][][] SHORT NAME: %-13s \n", name[choice-1], nick_name[choice-1]); 
    printf("+----------------------------------------------------+\n");
    printf("|PERSONA: %s \n", personality[choice-1]);  
    printf("+----------------------------------------------------+\n");
    printf(GREEN "|SPECIAL ABILITY: %-60s \n" RESET, sp_ability[choice-1]);  
    printf("+----------------------------------------------------+\n");
    printf("|BACKSTORY: %s \n", backstory[choice-1]);  
    printf("+----------------------------------------------------+\n" );
    printf("\n\n");
    char ch_i;
    printf("MOVE FORWARD WITH CURRENT SELECTION [Enter Y or N]? \n");
    while (1) {
        ch_i = getchar(); 
        if (ch_i == '\n') {
            //printf("You pressed Enter without Y or N. Please provide a valid answer.\n");
            continue;
        }
        if (tolower(ch_i) == 'y') {
            printf("MOVING FORWARD WITH THE SELECTION\n");
            sleep(3);
            Confirmed_character(nick_name[choice-1], personality[choice-1],sp_ability[choice-1]);
            break;
        } else if (tolower(ch_i) == 'n') {
            printf("[][] Loading character details [][] \n");
            sleep(1);
            system("clear");
            display_Character_Details();
            printf(YELLOW "\n CHOOSE YOUR CHARACTER \n" RESET);
            //printf(RED "Error: Operation Failed\n" RESET);
            scanf("%d", &choice);
            player_setup(choice);
            break;
        } else {
            printf("Invalid input! Please enter 'Y' or 'N'.\n");
        }
    }
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

    //function to have the player details
    get_Character_Details();
    display_Character_Details();
    //exit(0);
    printf(YELLOW "\n CHOOSE YOUR CHARACTER \n" RESET);
    //printf(RED "Error: Operation Failed\n" RESET);
    scanf("%d", &choice);
    player_setup(choice);


    exit(0);
    getTilesAttr();
    displayTiles();
    freeTilesMemory();
}

//cmd to execute script: gcc monopoly.c -o monopoly && ./monopoly