// C Program to illustrate how to create a dynamic array of
// strings
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LENGTH 20

int main()
{
    char** strings = NULL;
    int size = 5;
    strings = (char**)malloc(size * sizeof(char*));
    if (strings == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    // Allocate memory for each string and assign values
    for (int i = 0; i < size; i++) {
        // Allocate memory for each string
        strings[i] = (char*)malloc((MAX_STRING_LENGTH + 1)
                                   * sizeof(char));
        if (strings[i] == NULL) {
            fprintf(stderr, "Memory allocation failed\n");
            return 1;
        }
        // Assign values to each string
        sprintf(strings[i], "Student%d", i);
    }
    for (int i = 0; i < size; i++) {
        printf("%s\n", strings[i]);
    }
    for (int i = 0; i < size; i++) {
        free(strings[i]);
    }
    free(strings);

    return 0;
}
