#include <stdio.h>
#include <string.h>

double get_asset_from_file(const char* targetPlayer) {
    FILE *file;
    char line[200];
    char playerName[100], character[100];
    double assets = 0.0; 
    double total_amt_from_file = 0.0;
    file = fopen("player_details.csv", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return -1;  
    }
    // Skip the first line if it contains headers (optional)
    fgets(line, sizeof(line), file);

    // Read through the file line by line
    while (fgets(line, sizeof(line), file)) {
        // Parse each line
        if (sscanf(line, "%99[^,],%99[^,],%lf", playerName, character, &assets) == 3) {
            // Compare the player name with the targetPlayer
            if (strcmp(playerName, targetPlayer) == 0) {
                printf("Checking current assets status: %.2f\n", assets);
                total_amt_from_file = assets;  // Assign the asset value to total_amt_from_file
                break;  // Exit the loop once the player is found
            }
        }
    }

    fclose(file);  
    printf("Total amount from file is %.2f\n", total_amt_from_file);
    return total_amt_from_file;
}

int main() {
    const char* targetPlayer = "Mario";  // Example target player name
    double asset = get_asset_from_file(targetPlayer);
    
    if (asset != -1) {
        printf("Asset for player %s: %.2f\n", targetPlayer, asset);
    } else {
        printf("Failed to retrieve asset.\n");
    }

    return 0;
}
