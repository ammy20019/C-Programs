#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>  
double increase_amount(double amt, double percent) {
    return amt * (1 + percent / 100);
}

void* increase_amt(void* arg) {
    double* amt = (double*) arg;
    double increase_percent = 6.0;  // 6% increase
    int seconds_interval = 30;  // 30 seconds interval
    long long total_seconds = 6 * 30;  // 6 months in seconds
    long long intervals = total_seconds / seconds_interval;  // Number of intervals (30 sec each)

    for (long long i = 0; i < intervals; i++) {
        *amt = increase_amount(*amt, increase_percent);  // Increase the amount by 6%
        printf("Updated amount: ₹%.2f\n", *amt);
        sleep(seconds_interval);  // Sleep for 30 seconds before next increase
    }
    return NULL;
}

// Function to launch threads for each amount in the array
void launch_threads(double* amounts, int num_amounts) {
    pthread_t threads[num_amounts];  // Create an array of threads based on the number of amounts

    // Create a thread for each amount in the array
    for (int i = 0; i < num_amounts; i++) {
        pthread_create(&threads[i], NULL, increase_amt, (void*)&amounts[i]);
    }

    // Wait for all threads to finish execution
    for (int i = 0; i < num_amounts; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    // Array of amounts to process
    double amounts[] = {1000.0, 6000.0, 50000.0};  // You can add more values to this array
    int num_amounts = sizeof(amounts) / sizeof(amounts[0]);  // Calculate the number of amounts

    // Launch threads for each amount
    launch_threads(amounts, num_amounts);

    // Final result
    printf("\nFinal amounts after 6 months:\n");
    for (int i = 0; i < num_amounts; i++) {
        printf("Amount %d: ₹%.2f\n", i + 1, amounts[i]);
    }

    return 0;
}
