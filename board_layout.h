#ifndef BOARD_LAYOUT_H
#define BOARD_LAYOUT_H

typedef struct {
    double amt;           // Amount to be increased
    double increase_percent; // Percentage increase
    int seconds_interval;  // Time interval in seconds
    int tenure;             //time period in which interest 
} thread_data_interest;

extern pthread_t interest_amt_thread;
extern double rent[8];
extern double interest_per[8];
extern int tenure[6];
extern thread_data_interest data;

#endif
void board_layout();
void getTilesAttr();
void displayTiles();
void freeTilesMemory();
void *getBoardAttr(void *args);
void attrProperty(int i,int randomIndex);
void attrOffice(int i,int randomIndex);
void attrAirport(int i,int randomIndex);
void attrLuck(int i,int randomIndex);
void attrPrison(int i,int randomIndex);
void update_file_player_details(const char *p_name, const char *mod_value, const char *mod_column);
void *displayBoard(void *args);
void make_negative_double(double *num);
double increase_amount(double amt, double percent);
void* increase_amt(void* arg);
int get_interest_amt(double amount, double increase_percent,int tenure);