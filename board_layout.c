#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "character.h"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define MAX_ROWS 100

char** treasure = NULL;
char** place = NULL;
char** house = NULL;
char** income_tax = NULL;
char** airport = NULL;
char** company = NULL;
double* t_amt;
double* p_amt;
double* h_amt;
double* a_amt;
double* c_amt;

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
        company = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        t_amt = (double*)malloc(sizeof(double) * MAX_ROWS);
        p_amt = (double*)malloc(sizeof(double) * MAX_ROWS);
        h_amt = (double*)malloc(sizeof(double) * MAX_ROWS);
        a_amt = (double*)malloc(sizeof(double) * MAX_ROWS);
        c_amt = (double*)malloc(sizeof(double) * MAX_ROWS);

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
                    company[i_trav] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (company[i_trav] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(company[i_trav], value);
                } else if (column == 6) {
                    t_amt[i_trav] = atof(value);
                } else if (column == 7) {
                    p_amt[i_trav] = atof(value);
                } else if (column == 8) {
                    h_amt[i_trav] = atof(value);
                } else if (column == 9) {
                    a_amt[i_trav] = atof(value);
                } else if (column == 10) {
                    c_amt[i_trav] = atof(value);
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
    for (int i = 0; i < i_trav; i++) {
        printf("Row %d:\n", i + 1);
        printf("Treasure: %s\n", treasure[i]);
        printf("Place: %s\n", place[i]);
        printf("House: %s\n", house[i]);
        printf("Income Tax: %s\n", income_tax[i]);
        printf("Airport: %s\n", airport[i]);
        printf("Company: %s\n", company[i]);
        printf("Treasure Amt: %.2f\n", t_amt[i]);
        printf("Place Amt: %.2f\n", p_amt[i]);
        printf("House Amt: %.2f\n", h_amt[i]);
        printf("Airport Amt: %.2f\n", a_amt[i]);
        printf("Company Amt: %.2f\n", c_amt[i]);
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

void board_layout(){
    printf("Board Layout here \n");
    char **designs = design();

    if (!designs) {
        printf("Their was an issue while allocating memory \n");
        exit(1);  // Exit if there was an error in memory allocation
    }

    // Print each design
    for (int i = 0; i < 9; i++) {
        printf("%s\n", designs[i]);
    }

    //free_design(designs); need to take care of this later
}

char** tile= NULL;
char** action = NULL;
int row_b = 0;
int i_board = 0;
//need to edit this, kept as a placeholder ..modify this to load board details
void getBoardAttr() {
    FILE* fp = fopen("board.csv", "r");

    if (!fp) {
        printf("Can't open file\n");
        return;
    } else {
        // Allocate memory for each array to store column values for 100 rows
        tile = (char**)malloc(sizeof(char*) * MAX_ROWS); 
        action = (char**)malloc(sizeof(char*) * 500); 

        char buffer[1024]; // buffer to store each line of the CSV
        while (fgets(buffer, sizeof(buffer), fp)) {
            row_b++;
            if (row_b == 1) {
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
                    tile[i_board] = (char*)malloc((strlen(value) + 1) * sizeof(char));
                    if (tile[i_board] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(tile[i_board], value);
                } else if (column == 1) {
                    action[i_board] = (char*)malloc(500 * sizeof(char));
                    if (action[i_board] == NULL) {
                        printf("Memory allocation failed\n");
                        return;
                    }
                    strcpy(action[i_board], value);
                }
                // Move to the next token
                value = strtok(NULL, ",");
                column++;
            }

            // Move to the next row
            i_board++;  
        }
        fclose(fp);
    }
}

void attrProperty(int i,int randomIndex){
    strcat(action[i], " ");
    strcat(action[i],house[randomIndex]);
    printf("%s \n\n",action[i]);
}

void attrLuck(int i,int randomIndex){
    strcat(action[i], ": ");
    strcat(action[i],treasure[randomIndex]);
    printf("%s \n\n",action[i]);
}

void attrAirport(int i,int randomIndex){
    strcat(action[i], " ");
    strcat(action[i],airport[randomIndex]);
    printf("%s \n\n",action[i]);
}

void attrOffice(int i,int randomIndex){
    strcat(action[i], " ");
    strcat(action[i], company[randomIndex]);
    strcat(action[i], " Office");
    printf("%s \n\n",action[i]);
}

//0: house, 1: car, 2: person, 3: plane, 4: hotel, 5: cruise, 6: office, 7: lucky day, 8: prison, 9: start
void displayBoard() {
    getTilesAttr();
    char **designs = design();
    if (!designs) {
        printf("Their was an issue while allocating memory \n");
        exit(1);  // Exit if there was an error in memory allocation
    }
    //int arr_size = sizeof(treasure) / sizeof(treasure[0]);
    int arr_size = 4;
    int randomIndex;
    //printf("Array size %d \n", arr_size);
    srand(time(NULL));
    for (int i = 0; i < i_board; i++) {
        //printf("Tile: %s\n", tile[i]);
        if (i == 0){
            printf("%s %-30s \n\n", designs[9], action[i]);
        } else if (strstr(tile[i], "Property") != NULL) {
            randomIndex = rand() % arr_size;
            printf("%s", designs[0]);
            attrProperty(i,randomIndex);
        } else if (strstr(tile[i], "Airport") != NULL) {
            randomIndex = rand() % arr_size;
            printf("%s", designs[3]);
            attrAirport(i,randomIndex);
        } else if (strstr(tile[i], "Luck") != NULL) {
            randomIndex = rand() % arr_size;
            printf("%s", designs[7]);
            attrLuck(i,randomIndex);
        } else if (strstr(tile[i], "Prison") != NULL) {
            randomIndex = rand() % arr_size;
            printf("%s", designs[8]);
            printf("%s%s \n\n",action[i], ": Pay Rs 2500 as a bribe to get bail");
        } else if (strstr(tile[i], "Office") != NULL) {
            randomIndex = rand() % arr_size;
            printf("%s", designs[6]);
            attrOffice(i,randomIndex);
        } 
        //printf("\n");
    }
}