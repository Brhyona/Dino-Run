#include <iostream>
#include <cstdlib>
#include <string>
#include <math.h>
#include <ctime>

using namespace std;

bool gameLogic() {
    return false;
}

// Function to display the game over screen
void displayGameOver() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif

    // Shows the positive message to the user
    std::string messages[] = {
        "Better luck next time!"
    };

    // Ask user if they want to retry
    std::cout << "***** GAME OVER *****\n";
    std::cout << " Would you like to retry? (y/n): ";
}
// Retrying the game
bool retryGame(){
    char choice;
    std::cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        return true;
    } else {
        return false;
    }
}


void displayMessage() {
    bool playAgain = true;

    while (playAgain) {
     //   bool gameWon = gameLogic();

        // Need to have game logic

        // Game over
        displayGameOver();

        // Depending on the user, they can retry the game
        playAgain = retryGame();
    }

    std::cout << "Thanks for playing Dino-Run!\n";
}

// This will display the game over screen and start the game over loop
void startGameOver() {
}
