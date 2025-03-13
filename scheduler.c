//NEED to define the time as well as tenure, for example 30 days and 3 months of tenure i.e. = 30 seconds * 3 =>  3 months tenure
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>  

double increase_amount(double amt, double percent) {
    return amt * (1 + percent / 100);
}

int main() {
    double amount = 1000.0;  
    double increase_percent = 6.0;  // 6% increase
    int seconds_interval = 30;
    long long total_seconds = 6 * 30;

    long long intervals = total_seconds / seconds_interval;  // Number of intervals (30 sec each)

    for (long long i = 0; i < intervals; i++) {
        amount = increase_amount(amount, increase_percent);  // Increase amount by %
        printf("After %lld intervals, the amount is: ₹%.2f\n", i + 1, amount);
        sleep(seconds_interval);  // Sleep for 30 seconds before next increase
    }

    printf("\nFinal amount after 6 months: ₹%.2f\n", amount);
    return 0;
}
