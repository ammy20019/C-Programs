#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h> 
#include <math.h>

#include "character.h"
#include "board_layout.h"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN  "\033[36m"     
#define ORANGE "\033[48;5;214m"
#define MAX_ROWS 100
#define MAX_LINE_LENGTH 1024

pthread_t main_thread[50];
double rent[8] = {250.0, 450.0, 400.0, 140.0, 620.0, 280.0, 300.0, 800.0};
double interest_per[8] = {7.3, 3, 4, 6.2, 5, 6, 7, 8};
int tenure[8] = {1,3,6,4,5,2,10,8};
char* owned_properties[10] = {NULL};
int own_property_counter = 0;
double own_properties_amt[10];
double quoted_price[10];
double travel_cost = 280;
char *vehicle_names[] = {
    "🏍️ YAMAHA LRX200 *         ",
    "🚗 HYUNDAI i40M **     ",
    "🚙 PORSCHE MACAN ***   ",
    "🚘 LAMBORGHINI URUS SE~",
    "🏎️ FERRARI SF90 STRADLE~~  "
};
double price[] = {36000.00, 132000.20, 205000.50, 450000.00, 760000.10};
int fuel[] = {30, 140, 180, 200, 250};

char** treasure = NULL;
char** place = NULL;
char** house = NULL;
char** income_tax = NULL;
char** airport = NULL;
char** company = NULL;
double* t_amt = NULL;
double* p_amt = NULL;
double* h_amt = NULL;
double* a_amt = NULL;
double* c_amt = NULL;

int row = 0;
int i_trav = 0;

void getTilesAttr() {
    FILE* fp_tiles = fopen("mix_data.csv", "r");

    if (!fp_tiles) {
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

        char buffer[2042]; // buffer to store each line of the CSV
        while (fgets(buffer, sizeof(buffer), fp_tiles)) {
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
        fclose(fp_tiles);
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
        exit(1); 
    }
    for (int i = 0; i < 9; i++) {
        printf("%s\n", designs[i]);
    }
    //free_design(designs); need to take care of this later
}

char** tile= NULL;
char** action = NULL;
int row_b = 0;
int i_board = 0;
pthread_mutex_t board_attr_mutex = PTHREAD_MUTEX_INITIALIZER;
void *getBoardAttr(void *args) {
    pthread_mutex_lock(&board_attr_mutex);
    FILE* fp = fopen("board.csv", "r");

    if (!fp) {
        printf("Can't open file\n");
        exit(0);
        return NULL;
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
                        exit(1);
                        return NULL;
                    }
                    strcpy(tile[i_board], value);
                } else if (column == 1) {
                    action[i_board] = (char*)malloc(500 * sizeof(char));
                    if (action[i_board] == NULL) {
                        printf("Memory allocation failed\n");
                        exit(1);
                        return NULL;
                    }
                    strcpy(action[i_board], value);
                }
                value = strtok(NULL, ",");
                column++;
            }
            i_board++;  
        }
        fclose(fp);
        pthread_mutex_unlock(&board_attr_mutex);
    }
    return NULL;
}

void update_file_player_details(const char *p_name, const char *mod_value, const char *mod_column) {
    FILE *file, *temp_file;
    char line[2000];
    char playerName[100], character[100];
    double assets;
    int found = 0;
    file = fopen("player_details.csv", "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }
    temp_file = fopen("temp_player_details.csv", "w");
    if (temp_file == NULL) {
        perror("Error opening temp file");
        fclose(file);
        return;
    }
    if (fgets(line, sizeof(line), file) != NULL) {
        fputs(line, temp_file); // Write the header to the temp file
    }
    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%99[^,],%99[^,],%lf", playerName, character, &assets) == 3) {
            // Trim any newlines or excess spaces
            playerName[strcspn(playerName, "\n")] = 0;  // Remove the newline character
            character[strcspn(character, "\n")] = 0;
            //printf("Parsed values: %s, %s, %.2f\n", playerName, character, assets);
            if (strcmp(playerName, p_name) == 0) {
                if (strcmp(mod_column, "Assets") == 0) {
                    assets = atof(mod_value);  // Convert the string value to a double
                }
                found = 1;
            }
            fprintf(temp_file, "%s,%s,%.2f\n", playerName, character, assets);
        }
    }
    fclose(file);
    fclose(temp_file);
    // If the player was found and modified, replace the original file with the temporary file
    if (found) {
        remove("player_details.csv");
        rename("temp_player_details.csv", "player_details.csv");
        printf("Player details updated successfully.\n");
    } else {
        printf("Player not found!\n");
        remove("temp_player_details.csv");
    }
}

//code for interest amt on property bought
// Function to calculate the increased amount after a percentage increase
double increase_amount(double amt, double percent) {
    return amt *  percent / 100;
}

// Thread function to handle the increase of the amount over time
void* increase_amt(void* arg) {
    thread_data_interest* data = (thread_data_interest*) arg;
    double* amt = &(data->amt);  
    double increase_percent = data->increase_percent;
    int seconds_interval = data->seconds_interval; 
    int tenure = data->tenure;
    long long total_seconds = tenure * 30; 
    long long intervals = total_seconds / seconds_interval;  // Number of intervals (30 sec each)
    double curr_bal = 0;

    for (long long i = 0; i < intervals; i++) {
        sleep(seconds_interval);  // Sleep for the given interval before next increase
        double interest_amt = increase_amount(*amt, increase_percent);  // Increase the amount by the given percentage
        printf("\n💼 Current Balance: $ %.2f 💻\n", money);
        printf(GREEN " Interest Accrued: $ %.2f 🌟\n" RESET, interest_amt);        
        //curr_bal = get_asset_from_file(selected_player);
        
        transaction(selected_player,money,interest_amt);
        *amt += interest_amt;
    }
    return NULL;
}

pthread_t interest_amt_thread;
thread_data_interest data;

int get_interest_amt(double amount, double increase_percent,int tenure ){
    printf("📈 You will earn %.2f%% interest on your asset for %d months\n", increase_percent, tenure);
    printf("🧭 Interest will be credited to your account every month until the tenure.\n\n");
    int seconds_interval = 30;  // 30 seconds interval, which is 1 month in this game
    data.amt = amount;
    data.increase_percent = increase_percent;
    data.seconds_interval = seconds_interval;
    data.tenure = tenure;

    pthread_create(&interest_amt_thread, NULL, increase_amt, (void*)&data);

    // // Wait for the thread to finish execution
    // pthread_join(interest_amt_thread, NULL); called in main function monopoly.c
    return 0;
}

void sell_property(double amt, char* property, int index, double sell_price){
    printf("Successfully Sold: %s for %.2f \n ", property, sell_price);
    transaction(selected_player,money,sell_price);
    for (int i = index; i < own_property_counter - 1; i++) {
        own_properties_amt[i] = own_properties_amt[i + 1];
        owned_properties[i] = owned_properties[i+1];
    }
    own_property_counter--;
    char ch;
    printf("DO YOU WISH TO SELL MORE? (Y/N)\n");
    scanf(" %c", &ch);  // here I have have space before %c to ignore any leftover newline character
    if (ch == 'Y' || ch == 'y') {
        bought_property(0,"Real Estate");
    }
    else{
        ;
    }
}

int bought_property(double amt, char* property){
    if ((amt == 0) || (strcmp(property, "Real Estate") == 0)){
        printf("Listing your all properties here \n");
        for(int i=0;i<own_property_counter;i++){
            double max_amt = own_properties_amt[i] * 1.6; // Calculate the upper bound (60% more than the property amount)
            double random_factor = ((double)rand() / RAND_MAX) * (max_amt - own_properties_amt[i]); // A value between 0 and (max_amt - amt) -> Generate a random number to decide whether to increase or decrease
            if (rand() % 2 == 0) { // Randomly decide to increase or decrease the value
                quoted_price[i] = own_properties_amt[i] + random_factor;
            } else {
                quoted_price[i] =  own_properties_amt[i] - random_factor;
            }
            printf(YELLOW "%d-[🏠] Owned: %s | Purchased Amt: $ %.2f | Quoted Price: $ %.2f 💰\n" RESET, i+1, owned_properties[i], own_properties_amt[i], quoted_price[i]);
        }
        if (own_property_counter > 0){
            printf(GREEN " DO YOU WISH TO SELL YOUR PROPERTY? (Enter the corresponding number to sell) or Enter '0' to skip \n" RESET);
            int choice;
            scanf("%d", &choice);
            if (choice == 0){
                ;
            } else{
                int s = choice - 1;
                sell_property(own_properties_amt[s],owned_properties[s],s,quoted_price[s]);
            }
        }
    } else{
        owned_properties[own_property_counter] = property;
        own_properties_amt[own_property_counter] = amt;
        own_property_counter++;
        get_watch_quest(own_property_counter);
    }
    return 0;
}

void make_negative_double(double *num) {
    if (*num > 0) {
        *num = -(*num);  // Multiply the number by -1 to make it negative
    }
}
void make_positive_double(double *num) {
    if (*num < 0) {
        *num = -(*num);  // Multiply the number by -1 to make it positive
    }
}
void attrProperty(int i,int randomIndex){
    int r_index = rand() % 8; //8 here because rent/tenure/interest are having 8 ele each
    strcat(action[i], " ");
    strcat(action[i],house[randomIndex]);
    printf("%s \n\n",action[i]);
    //will define the sequence once the player lands in property
    char ch;
    printf("Do you want to buy this property for $%.2f or pay rent of $%.2f ? (B/R) : ",fabs(h_amt[randomIndex]),fabs(rent[r_index]));
    scanf(" %c", &ch);  // here I have have space before %c to ignore any leftover newline character
    if (ch == 'B' || ch == 'b') {
        printf("You chose to buy this property. Proceeding...\n");
        make_negative_double(&h_amt[randomIndex]);
        double before_buying = money; // suppose 10rs and bought property for 1rs, remainig = 9rs in acct -> logic 10 - property = acct bal
        transaction(selected_player,money,h_amt[randomIndex]);
        make_positive_double(&h_amt[randomIndex]);
        if (before_buying - h_amt[randomIndex] == money){
            get_interest_amt(h_amt[randomIndex], interest_per[r_index], tenure[r_index]);
            bought_property(h_amt[randomIndex], house[randomIndex]); //purchased property history
        }
    } else if (ch == 'R' || ch == 'r') {
        printf("You chose to pay the rent for the stay. Proceeding...\n");
        make_negative_double(&rent[r_index]);
        transaction(selected_player,money,rent[r_index]);
    } else {
        printf("Invalid input. Please enter 'B' or 'R'.\n");
        attrProperty(i,randomIndex);
    }
    // printf("-------------------------------------------------------\n");
    // printf(YELLOW "Your updated balance is : %.2f \n" RESET, money);
    // printf("-------------------------------------------------------\n\n");
    // sleep(2);
}

void attrLuck(int i,int randomIndex){
    strcat(action[i], ": ");
    strcat(action[i],treasure[randomIndex]);
    printf("%s \n\n",action[i]);
    //defining flow for the luck attribute
    money = transaction(selected_player,money,t_amt[randomIndex]);
}

void attrAirport(int i,int randomIndex){
    int r_index = rand() % 8;
    strcat(action[i], " ");
    strcat(action[i],airport[randomIndex]);
    printf("%s \n\n",action[i]);
    //defining flow for airport
    char ch;
    double fee = -100.00;
    printf("Do you want to buy this Airport for $%.2f or parking fee of $%.2f ? (B/R) : ",fabs(a_amt[randomIndex]),fabs(fee));
    scanf(" %c", &ch);  // here I have have space before %c to ignore any leftover newline character
    if (ch == 'B' || ch == 'b') {
        printf("You chose to buy this property. Proceeding...\n");
        //bought_property(a_amt[randomIndex], airport[randomIndex]); //purchased property history
        make_negative_double(&a_amt[randomIndex]);
        double before_buying = money;
        transaction(selected_player,money,a_amt[randomIndex]);
        make_positive_double(&a_amt[randomIndex]);
        if (before_buying - a_amt[randomIndex] == money){
            //flow for interest on the property
            get_interest_amt(a_amt[randomIndex], interest_per[r_index], tenure[r_index]);
            bought_property(a_amt[randomIndex], airport[randomIndex]); //purchased property history
        }
    } else if (ch == 'R' || ch == 'r') {
        printf("You chose to pay the rent for parking. Proceeding...\n");
        money = transaction(selected_player,money,fee);
    } else {
        printf("Invalid input. Please enter 'B' or 'R'.\n");
        attrAirport(i,randomIndex);
    }
}

void attrOffice(int i,int randomIndex){
    int r_index = rand() % 8;
    strcat(action[i], " ");
    strcat(action[i], company[randomIndex]);
    strcat(action[i], " Office");
    printf("%s \n\n",action[i]);
    //flow for buying office
    char ch;
    printf("Do you want to buy this Company for $%.2f or visiting fee of $%.2f ? (B/R) : ",fabs(c_amt[randomIndex]),fabs(rent[r_index]));
    scanf(" %c", &ch);  // here I have have space before %c to ignore any leftover newline character
    if (ch == 'B' || ch == 'b') {
        printf("You chose to buy this Office. Proceeding...\n");
        //bought_property(c_amt[randomIndex], company[randomIndex]); //purchased property history
        make_negative_double(&c_amt[randomIndex]);
        double before_buying = money;
        transaction(selected_player,money,c_amt[randomIndex]);
        make_positive_double(&c_amt[randomIndex]);
        if (before_buying - c_amt[randomIndex] == money){
            //flow for interest on the property
            get_interest_amt(c_amt[randomIndex], interest_per[r_index], tenure[r_index]);
            bought_property(c_amt[randomIndex], company[randomIndex]); 
        }
    } else if (ch == 'R' || ch == 'r') {
        printf("You chose to pay the visiting fee in the Office. Proceeding...\n");
        make_negative_double(&rent[r_index]);
        money = transaction(selected_player,money,rent[r_index]);
    } else {
        printf("Invalid input. Please enter 'B' or 'R'.\n");
        attrProperty(i,randomIndex);
    }
    sleep(2);
}

void attrPrison(int i){
    int r_index = rand() % 8;
    double bail_amt = -3000;
    printf("%s. Pay $%.2f to get bail \n\n",action[i],fabs(bail_amt));
    money = transaction(selected_player,money,bail_amt);
}

void attrRealEstate(int i){
    int r_index = rand() % 8;
    printf("%s \n\n",action[i]);
    //flow for buying properties, here will give user option to sell properties
    bought_property(0,"Real Estate");
}

void attrAutomobile(int i){
    printf("%s \n\n",action[i]);
    printf(YELLOW "         []               MARCO AUTOMOBILES             [] \n");
    printf( "  |        VEHICLE             |  On-Road PRICE  | Cost Per Move \n" RESET);
    printf("  | -------------------------  - --------------  - ------------- |\n");

    for (int i = 0; i < 5; i++) {
        printf(CYAN "%d | %-28s | %14.2f | %14d |\n" RESET, i+1, vehicle_names[i], price[i], fuel[i]);
        printf("  - -------------------------  - ---------------- - --------------\n");
    }
    printf("WHICH VEHICLE WOULD YOU LIKE TO PURCHASE? (Enter No. corresponding to vehicle) or Enter '0' to skip \n");
    int ch;
    scanf("%d", &ch);
    if (ch == 0){
        ;
    } else{
        int s = ch - 1;
        bought_property(price[s],vehicle_names[s]);
        make_negative_double(&price[s]);
        transaction(selected_player,money,price[s]);
        make_positive_double(&price[s]);
        //sell_property(own_properties_amt[s],owned_properties[s],s,quoted_price[s]);
    }
}

int attrVacation(int i){
    int response = seashore_intro();
    return response; //0 or 1, 1 for no, 0 for yes
}

//0: house, 1: car, 2: person, 3: plane, 4: hotel, 5: cruise, 6: office, 7: life event, 8: prison, 9: start, 10: Real Estate Agent
void *displayBoard(void *args) {
    //getTilesAttr();
    char **designs = design();
    if (!designs) {
        printf("Their was an issue while allocating memory \n");
        exit(1);  // Exit if there was an error in memory allocation
    }
    //int arr_size = sizeof(treasure) / sizeof(treasure[0]);
    int arr_size = 29; //for 29 data rows in excel
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
        } else if (strstr(tile[i], "Life Event") != NULL) {
            randomIndex = rand() % arr_size;
            printf("%s", designs[7]);
            attrLuck(i,randomIndex);
        } else if (strstr(tile[i], "Prison") != NULL) {
            printf("%s", designs[8]);
            attrPrison(i);
        } else if (strstr(tile[i], "Office") != NULL) {
            randomIndex = rand() % arr_size;
            printf("%s", designs[6]);
            attrOffice(i,randomIndex);
        } else if(strstr(tile[i], "Dealership") != NULL) {
            printf("%s", designs[10]);
            attrRealEstate(i);
        } else if(strstr(tile[i], "Automobile") != NULL) {
            //printf("%s", designs[10]);
            attrAutomobile(i);
        } else if(strstr(tile[i], "Vacation") != NULL) {
            printf("%s", designs[5]);
            int go = attrVacation(i);
            if (go==1){
                i = 1;
            }
        }
        double travel_cost_r;
        if (own_property_counter>0){
            for (int i = 0; i < own_property_counter; i++) {
                if (strstr(owned_properties[i], "***")) { travel_cost_r = fuel[2]; }
                else if (strstr(owned_properties[i], "**")) { travel_cost_r = fuel[1]; }
                else if (strstr(owned_properties[i], "*")) { travel_cost_r = fuel[0]; }
                else if (strstr(owned_properties[i], "~~")) { travel_cost_r = fuel[4]; }
                else if (strstr(owned_properties[i], "~")) { travel_cost_r = fuel[3]; }
            }
        } else{
            travel_cost_r = travel_cost;
        }
        printf("Spent $ %.2f on travelling \n",fabs(travel_cost_r));
        make_negative_double(&travel_cost_r);
        transaction(selected_player,money,travel_cost_r);
        sleep(2);
        //printf("\n");
    }
    char money_str[20];
    sprintf(money_str, "%.2f", money);  //cconvert int to str first
    //printf("===Fetched player name is %s \n", player_name);
    update_file_player_details(player_name,money_str,"Assets");
    return NULL;
}