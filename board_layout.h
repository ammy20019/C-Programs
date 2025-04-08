#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

typedef struct {
    double amt;           // Amount to be increased
    double increase_percent; // Percentage increase
    int seconds_interval;  // Time interval in seconds
    int tenure;             //time period in which interest 
} thread_data_interest;

extern pthread_t interest_amt_thread;
extern double travel_cost;
extern double rent[8];
extern double interest_per[8];
extern int tenure[8];
extern char* owned_properties[10];
extern thread_data_interest data;
extern int own_property_counter;
extern double own_properties_amt[10];
extern double quoted_price[10];

#endif
void board_layout();
void getTilesAttr();
void displayTiles();
void freeTilesMemory();
void *getBoardAttr(void *args);
void attrProperty(int i,int randomIndex);
void sell_property(double amt, char* property, int index, double sell_price);
int bought_property(double amt, char* property);
void attrOffice(int i,int randomIndex);
void attrAirport(int i,int randomIndex);
void attrLuck(int i,int randomIndex);
void attrPrison(int i);
void attrRealEstate(int i);
void update_file_player_details(const char *p_name, const char *mod_value, const char *mod_column);
void *displayBoard(void *args);
void make_negative_double(double *num);
void make_positive_double(double *num);
double increase_amount(double amt, double percent);
void* increase_amt(void* arg);
int get_interest_amt(double amount, double increase_percent,int tenure);
void attrAutomobile(int i);
int attrVacation(int i);
int seashore_intro();
void get_watch_quest(int own_property_counter);
void begin_cruise_journey();