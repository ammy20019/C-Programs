// functions.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include "board_layout.h"
#include "audio.h"
#define MAX_ROWS 500
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
char* selected_player = NULL;
char* selected_personality = NULL;
char* selected_speciality = NULL;
double money=10000; //initial amount
char* player_name = NULL;
char name1[250], name2[250], full_name[500];

int char_row = 0;
int char_trav = 0;

double get_asset_from_file(const char* targetPlayer) {
    FILE *file;
    char line[200];
    char playerName[100], character[100];
    double assets = 0.0; 
    double total_amt_from_file = 0.0;
    file = fopen("player_details.csv", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return -1;  
    }
    // Skip the first line if it contains headers (optional)
    fgets(line, sizeof(line), file);

    // Read through the file line by line
    while (fgets(line, sizeof(line), file)) {
        // Parse each line
        if (sscanf(line, "%99[^,],%99[^,],%lf", playerName, character, &assets) == 3) {
            if (strcmp(playerName, targetPlayer) == 0) {
                printf("Checking current assets status: %.2f\n", assets);
                total_amt_from_file = assets; 
                break;  // Exit the loop once the player is found
            }
        }
    }

    fclose(file);  
    //printf("Total amount from file is %.2f\n", total_amt_from_file);
    return total_amt_from_file;
}

pthread_mutex_t counter_mutex;
double transaction(const char* targetPlayer, double total_amt, double transaction_amt){
    pthread_mutex_lock(&counter_mutex);
    if ((total_amt + transaction_amt < 0) ){
        double lower_amt_margin = total_amt / 2.5;
        double max_amt_margin = total_amt / 4.8;
        // Generate a random number between 0 and 1, then scale it to the range [lower_amt_margin, max_amt_margin]
        double random_amt = (rand() / (double)RAND_MAX) * (max_amt_margin - lower_amt_margin) + lower_amt_margin; // Generate a random amt between the margins
        printf("Insufficients funds for Transaction, Invalid request \n ");
        printf("Cheque bounced, pay a fine of $%.2f to Bank \n", random_amt);
        make_negative_double(&random_amt);
        return transaction(targetPlayer,total_amt, random_amt);
    } else if(total_amt == 0){
        printf("You have gone bankrupt! Exiting game...! \n");
        exit(0);
    }
    total_amt += transaction_amt;
    printf("-------------------------------------------------------\n");
    printf(YELLOW "Your updated balance is : %.2f \n" RESET, total_amt);
    printf("-------------------------------------------------------\n\n");
    pthread_mutex_unlock(&counter_mutex);
    money = total_amt;
    return total_amt;
}

pthread_mutex_t get_character_mutex = PTHREAD_MUTEX_INITIALIZER;
void *get_Character_Details(void *args) {
    pthread_mutex_lock(&get_character_mutex);
    FILE* fp_character = fopen("characters.csv", "r");

    if (!fp_character) {
        printf("Can't open file\n");
        exit(0);
        return NULL;
    } else {
        // Allocate memory for each array to store column values for 100 rows
        character = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        name = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        sp_ability = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        backstory = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        personality = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        nick_name = (char**)malloc(sizeof(char*) * MAX_ROWS); 

        char buffer[3072]; // buffer to store each line of the CSV
        while (fgets(buffer, sizeof(buffer), fp_character)) {
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
                        exit(0);
                        return NULL;
                    }
                    strcpy(character[char_trav], value);
                } else if (column == 1) {
                    name[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    nick_name[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (name[char_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        exit(0);
                        return NULL;
                    }
                    sscanf(value, "%s %*s %s", name1, name2);
                    snprintf(full_name, sizeof(full_name), "%s %s", name1, name2);
                    strcpy(name[char_trav], full_name);
                    sscanf(value,  "%*s \"%[^\"]\"", nick_name[char_trav]); ///extracting nick name
                } else if (column == 2) {
                    sp_ability[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (sp_ability[char_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        exit(0);
                        return NULL;
                    }
                    strcpy(sp_ability[char_trav], value);
                } else if (column == 3) {
                    backstory[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (backstory[char_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        exit(0);
                        return NULL;
                    }
                    strcpy(backstory[char_trav], value);
                } else if (column == 4) {
                    personality[char_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (personality[char_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        exit(0);
                        return NULL;
                    }
                    strcpy(personality[char_trav], value);
                }
                // Move to the next token
                value = strtok(NULL, ",");
                column++;
            }
            char_trav++;   //next row
        }
        fclose(fp_character);
        pthread_mutex_unlock(&get_character_mutex);
    }
    return NULL;
}

void *display_Character_Details(void *args) {
    system("clear");
    printf("[[[[[[[[[[[[[[[[[[[[[[[[[  CHARACTER LOBBY  ]]]]]]]]]]]]]]]]]]]]]]]]]]]\n");
    printf("Choose a character, that you would like to play in this game, each player has it's own trait \n\n");
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
        //while ((ch = getchar()) != '\n' && ch != EOF);
        ch = getchar();
        if (ch == '\n') {
            printf("[][] Loading character details [][] \n");
            //sleep(2);
            system("clear");
        } else {
            printf("You didn't press Enter!\n");
            sleep(5);
        }
    }
    return NULL;
}

void Confirmed_character(char* nickname,char* personality,char* speciality){
    char ch;
    system("clear");
    printf(GREEN "[👩‍💼] Mia [Your guide throughout journey] \n\n" RESET);
    printf(YELLOW "[💼] Welcome to Assetopia's Digital Realm! 🌍\n\n");
    printf("[🤖] I'm Mia, here to guide you on your path to fortune. 🛤️\n\n");
    printf("[💰] You will begin your journey with a balance of $10,000. 🏦\n\n");
    printf("[⚡] The choices you make will either pave the way to great riches or pull you into the grip of debt. 💳\n\n");
    printf("[💡] In this world, you can buy assets, earn profits, and watch your wealth grow, but remember, every ⏳ 30 seconds equals a full month of growth. Interest is earned on both your assets and their profits, so keep an eye on your investments.\n\n");
    printf("[🌟] Throughout your journey, you'll experience various stages of life, from taking on home loans to attending extravagant cruise parties. Your reputation will be shaped by the assets you accumulate. \n\n");
    printf("[🚀] Best of luck on your journey ahead! \n\n" RESET);    
    selected_player = nickname;
    selected_personality = personality;
    selected_speciality = speciality;
    printf("PRESS ENTER TO BEGIN YOUR JOURNEY! \n");
    getchar();  // This is to clear out any leftover input from previous operations
    ch = getchar();
    if (ch == '\n') {
        pthread_join(main_thread[5], NULL);
        system("clear");
    } else{
        Confirmed_character(nickname, personality, speciality);
    }
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
            sleep(2);
            pthread_create(&main_thread[5], NULL, mia_intro, NULL);
            Confirmed_character(nick_name[choice-1], personality[choice-1],sp_ability[choice-1]);
            break;
        } else if (tolower(ch_i) == 'n') {
            printf("[][] Loading character details [][] \n");
            sleep(1);
            system("clear");
            pthread_t thread; 
            pthread_create(&thread, NULL, display_Character_Details, NULL);
            pthread_join(thread, NULL);
            //display_Character_Details();
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
char **designs = NULL;
char** design() {
    // Dynamically allocate memory for the array of char pointers
    designs = (char**) malloc(11 * sizeof(char*)); //hardcode 11 element
    if (!designs) {
        printf("Memory allocation failed for designs array!\n");
        return NULL;
    }
    //0: house, 1: car, 2: person, 3: plane, 4: hotel, 5: cruise, 6: office, 7: lucky day, 8: prison, 9: start
    // Dynamically allocate memory for each design string and format the strings
    designs[0] = (char*) malloc(50 * sizeof(char));
    snprintf(designs[0], 50, " /```````\\ \n"
                            "`|``|`|``|`\n" 
                            " ^^HOUSE^^  ");

    designs[1] = (char*) malloc(50 * sizeof(char));
    snprintf(designs[1], 50, " ___\n"
                            "/   | \n"
                            "o``o`  ");

    designs[2] = (char*) malloc(20 * sizeof(char));
    snprintf(designs[2], 20, "  o   \n"
                            " /Q\\  \n"
                            " / \\  ");

    designs[3] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[3], 100, "  _____/\\\n"
                             "/[]`````|\n"
                             "````\\_\\``  ");

    designs[4] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[4], 100, " /====\\ \n"
                             "| HOTEL|\n"
                             "| -  - |\n"
                             "| -  - |\n"
                             "|_|^^|_|  ");

    designs[5] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[5], 100, "       \n"
                             "    ii_____n_ \n"
                             "   _|        |_\n"
                             "  /___CRUISE___\\\n"
                             "~~~~~~~~~~~~~~~~~~  ");

    designs[6] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[6], 100, " /````\\ \n"
                             "|OFFICE|\n"
                             "| -  - |````|\n"
                             "| -  - | -- |\n"
                             "|_|^^|_|_[]_|  ");

    designs[7] = (char*) malloc(40 * sizeof(char));
    snprintf(designs[7], 40, "  \\ o /\n"
                             "    Q  \n"
                             "   / \\ \n"
                             "LIFE EVENT ");

    designs[8] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[8], 100, "|---PRISON---|| \n"
                             "| | | | | | | | \n"
                             "| | | | | | | | \n"
                             "| | | | | | | | \n"
                             "``````````````` ");
    designs[9] = (char*) malloc(100 * sizeof(char));
    snprintf(designs[9], 100, " ____n____\n"
                              "|         | \n"
                              "|__START__|");
    designs[10] = (char*) malloc(200 * sizeof(char));
    snprintf(designs[10], 200, "      ________ \n"
                               "     |        | \n"
                               "     |  HOUSE | \n"
                               "     |  FOR   | \n"
                               "     |  SALE  | \n"
                               "     |________| \n"
                               " Best deals in town! ");
    return designs;
}

int check_if_already_registered(){
    char line[300];
    char player_name_from_file[300];
    char character[300];
    double assets;
    FILE *file_read = fopen("player_details.csv", "r");
    if (file_read == NULL) {
        printf("Could not open file.\n");
        return 0;
    }

    fgets(line, sizeof(line), file_read);

    // Read each line and check if the player exists
    while (fgets(line, sizeof(line), file_read) != NULL) {
        // Parse the line
        if (sscanf(line, "%[^,],%[^,],%lf", player_name_from_file, character, &assets) == 3) {
            // Check if the player name matches the input
            if (strcmp(player_name, player_name_from_file) == 0) {
                fclose(file_read);
                printf("Player already registered! Welcome back %s \n",player_name);
                return 1;
            }
        }
    }
    fclose(file_read);
    return 0;
}

int register_player(){
    char username[20];
    printf(GREEN "WELCOME TO ASSETOPIA \n" RESET);
    printf("Please register your profile, \n Enter your username : ");
    fgets(username, 20, stdin);
    username[strcspn(username, "\n")] = '\0';
    player_name = (char*)malloc(strlen(username) + 1); // +1 for null terminator
    if (player_name == NULL) {
        printf("Memory allocation failed while registering\n");
        return -1; 
    }
    strcpy(player_name, username);
    if (check_if_already_registered()){
        return 1;
    }
    FILE *file;
    file = fopen("player_details.csv", "a");  // Open in append mode
    if (file == NULL) {
        perror("Error Registering Your Data, Please check all resources are downloaded");
        return 0;
    }
    fprintf(file, "%s,%s,%.2f\n", username,selected_player, money);
    fclose(file);
    printf(GREEN "THANK YOU FOR REGISTERING WITH US \n\n\n" RESET);
    sleep(1);
    //update_file_player_details(player_name,"89999.09","Assets");
    return 1;
}

void free_design(){
    for (int i = 0; i < 10; i++) {
        free(designs[i]);  // Free each string
    }
    free(designs); 
}