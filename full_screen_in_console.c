//    #include <windows.h>

//    int main() {
//        HWND consoleWindow = GetConsoleWindow();
//        ShowWindow(consoleWindow, SW_MAXIMIZE); // Maximize the console window


//        printf("IT SHOULD PRINT IN FULL SCREEN");
//        printf("\n\n AMIT HERE \n\n");
//        // Your game code here

//        return 0;
//    }
#include <stdio.h>
#include <stdlib.h>

int main() {
    // Use AppleScript to maximize the Terminal
    system("osascript -e 'tell application \"Terminal\" to set the bounds of the front window to {0, 0, 1920, 1080}'");

    // Example output
    printf("Terminal should now be full-screen.\n");
    printf("IT SHOULD PRINT IN FULL SCREEN\n");
    printf("\n\n AMIT HERE \n\n");
    
    // Your game code here

    return 0;
}
