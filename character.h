#ifndef CHARACTER_H
#define CHARACTER_H

extern char** character;
extern char** name;
extern char** sp_ability;
extern char** backstory;
extern char** personality;
extern char** nick_name;
extern char* selected_player;
extern char* selected_personality;
extern char* selected_speciality;
extern double money;
extern char* player_name;

#endif
void say_hello(void);
double transaction(const char* targetPlayer, double total_amt,double transaction_amt);
int check_if_already_registered();
int register_player();
void *get_Character_Details(void *args);
void *display_Character_Details(void *args);
void Confirmed_character(char* nickname,char* personality,char* speciality);
void player_setup(int choice);
void free_Character_Details();
char** design();
void free_design();