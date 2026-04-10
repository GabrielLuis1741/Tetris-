#include <iostream>
#include "ui_TetrisPlusV2.h"
#include "../../GameEngine.h"

void MenuManager() {

	std::cout << "Welcome to TetrisPlus!" << std::endl << "Press 1 to play Endless Mode.\n Press 2 to play TimeTrial\n Press 0 to Quit Game.";
	
	int choice;
	std::cin >> choice;

	switch (choice) {
	case 0:
		// Handle quit game logic here
		break;
	// Add other cases as needed
	default:
		break;
	}
}


bool GameEngine::isGameOver() const {
    return gameOver;
}