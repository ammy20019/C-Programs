#include <stdio.h>
#include <pthread.h>

int cnt = 0;
void *print_message(void *arg) {
    for (int i = 0; i < 100000; i++) {
        cnt++;
    }
    return NULL;
}

int main() {
    pthread_t thread[5]; 
    pthread_create(&thread[1], NULL, print_message, NULL);
    pthread_create(&thread[0], NULL, print_message, NULL);

    pthread_join(thread[1], NULL);
    pthread_join(thread[0], NULL);

    printf("Shared counter value: %d\n", cnt);

    return 0;
}


//Here I observed that race condition is happening due to which the shared variable is changing inconsistently, to resolve this I searched in the internet and found that it is due to lack of synchronisation and mutex implementation.