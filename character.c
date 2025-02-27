// functions.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#define MAX_ROWS 100
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"

void say_hello(void) {
    printf("---------- Loading character resources from library ----------\n");
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

void free_Character_Details() {
    printf("Freeing character resources ");
    // Free all the dynamically allocated memory
    for (int i = 0; i < char_trav; i++) {
        if (character[i] != NULL) {
            free(character[i]);
            character[i] = NULL; // Set to NULL to avoid dangling pointer
        }
        if (name[i] != NULL) {
            free(name[i]);
            name[i] = NULL;
        }
        if (sp_ability[i] != NULL) {
            free(sp_ability[i]);
            sp_ability[i] = NULL;
        }
        if (backstory[i] != NULL) {
            free(backstory[i]);
            backstory[i] = NULL; 
        }
        if (personality[i] != NULL) {
            free(personality[i]);
            personality[i] = NULL;
        }
        if (nick_name[i] != NULL) {
            free(nick_name[i]);
            nick_name[i] = NULL; 
        }
    }

    // Free the arrays of pointers themselves
    free(character);
    free(name);
    free(sp_ability);
    free(backstory);
    free(personality);
    free(nick_name);

    // Set the pointers to NULL to avoid any dangling pointers
    character = NULL;
    name = NULL;
    sp_ability = NULL;
    backstory = NULL;
    personality = NULL;
    nick_name = NULL;
}

char** design() {
    // Dynamically allocate memory for the array of char pointers
    char **designs = (char**) malloc(9 * sizeof(char*));
    if (!designs) {
        printf("Memory allocation failed for designs array!\n");
        return NULL;
    }

    // Dynamically allocate memory for each design string and format the strings
    designs[0] = (char*) malloc(50 * sizeof(char));
    snprintf(designs[0], 50, " /```````\\ \n"
                            "`|``|`|``|`\n" 
                            " ^^HOUSE^^ \n");

    designs[1] = (char*) malloc(50 * sizeof(char));
    snprintf(designs[1], 50, " ___\n"
                            "/   | \n"
                            "o``o`\n");

    designs[2] = (char*) malloc(20 * sizeof(char));
    snprintf(designs[2], 20, "  o   \n"
                            " /Q\\  \n"
                            " / \\\n");

    designs[3] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[3], 100, "  _____/\\\n"
                             "/[]`````|\n"
                             "````\\_\\``\n");

    designs[4] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[4], 100, " /====\\ \n"
                             "| HOTEL|\n"
                             "| -  - |\n"
                             "| -  - |\n"
                             "|_|^^|_|\n");

    designs[5] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[5], 100, "       \n"
                             "    ii_____n_ \n"
                             "   _|        |_\n"
                             "  /___CRUISE___\\\n"
                             "~~~~~~~~~~~~~~~~~~\n");

    designs[6] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[6], 100, " /````\\ \n"
                             "|OFFICE|\n"
                             "| -  - |````|\n"
                             "| -  - | -- |\n"
                             "|_|^^|_|_[]_|\n");

    designs[7] = (char*) malloc(40 * sizeof(char));
    snprintf(designs[7], 40, "  \\ o /\n"
                             "    Q  \n"
                             "   / \\ \n"
                             "LUCKY DAY\n");

    designs[8] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[8], 100, "|---PRISON---|| \n"
                             "| | | | | | | | \n"
                             "| | | | | | | | \n"
                             "| | | | | | | | \n"
                             "``````````````` \n");
    designs[9] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[8], 100, " ____n____\n"
                              "|         | \n"
                              "|__START__|\n");

    return designs;
}

void free_design(char** designs){
    for (int i = 0; i < 10; i++) {
        free(designs[i]);  // Free each string
    }
    free(designs); 
}