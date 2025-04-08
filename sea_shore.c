#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h> 
#include <stdbool.h> 
#include <math.h>

#include "character.h"
#include "board_layout.h"
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define CYAN  "\033[36m"     
#define ORANGE "\033[48;5;214m"
bool get_watch1 = false;
bool get_watch2 = false;
void get_watch_quest(int own_property_counter){
    if (own_property_counter > 3 && get_watch1 == false) {
        double worth = 430500.80;
        char* watch = "⌚ Patek Philippe 7042/100G - Gondolo";
        printf(GREEN "Congratulations! 🌟 Received a %s of worth Rs %.2f from Mr. Ammy as a token of appreciation\n" RESET, watch, worth);
        get_watch1 = true;
        bought_property(worth, watch); 
    } else if (own_property_counter > 5 && get_watch2 == false) {
        double worth = 600000.00;
        char* watch = "⌚ Omega Seamaster ";
        printf(GREEN "Congratulations! 🌟 Received a %s of worth Rs %.2f from Mr. Ammy as a token of appreciation\n" RESET, watch, worth);
        get_watch2 = true;
        bought_property(worth, watch); 
}
}

void begin_cruise_journey(){
    travel_cost = 0;
}

void cruise_journey(){
    bool c1 = false;
    bool c2 = false;
    for (int i = 0; i < own_property_counter; i++){
        if (strstr(owned_properties[i], "Patek Philippe")) {
                c1 = true;
        } else if (strstr(owned_properties[i], "~") || strstr(owned_properties[i], "***")){
                c2 = true;
        }
    }
    if (c1 && c2) {
        printf("--------------------------------------------------------------------\n");
        printf("\n Congratulations! 🌟 You have been invited for the Cruise Trip \n");
        printf("--------------------------------------------------------------------\n");
        void begin_cruise_journey();
    }
}

int seashore_intro(){
    printf(GREEN "\n🌟 Congratulations! You have come a long way to the shores of Tauchplatz Rietli 🏖️\n" RESET);
    printf("🚢 A new journey of luxury is waiting for you! 🛳️\n");
    printf("There are a few criteria that are needed to join the LUXURY CRUISE Trip: \n");
    printf(" [] Owns a luxury watch ⌚\n");
    printf(" [] Has at least a luxury car worth ₹2L 🚗\n");
    printf(YELLOW "\n🌍 WOULD YOU LIKE TO GO ON THE CRUISE OR EXPLORE SWITZERLAND AGAIN FOR BETTER WEALTH? (Y/N) 🤔" RESET);
    char ch;
    scanf(" %c", &ch);
    if (ch == 'Y' || ch == 'y'){
        cruise_journey();
        return 0;
    } else if (ch == 'N' || ch == 'n'){
        return 1;
    }
    return seashore_intro();
}