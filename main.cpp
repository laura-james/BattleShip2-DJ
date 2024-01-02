#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <unistd.h>
#include <vector>
bool areArraysNotEqual(const char arr1[], const char arr2[], int size) {
  for (int i = 0; i < size; i++) {
    if (arr1[i] != arr2[i]) {
      return true;
    }
  }
  return false;
}
void placeShip(char **board, int size, char type) {
  int randdir = rand() % 2;
  int randx;
  int randy;
  char check[5] = {};
  char clearArray[5] = {};

  while (areArraysNotEqual(check, clearArray, size)) {
    for (int i = 0; i < size; i++) {
      clearArray[i] = '-';
    }
    randx = rand() % (10 - size);
    randy = rand() % (10 - size);

    for (int i = 0; i < size; ++i) {
      if (randdir == 0) {
        check[i] = board[randx][randy + i];
      } else {
        check[i] = board[randx + i][randy];
      }
    }
  }

  for (int i = 0; i < size; ++i) {
    if (randdir == 0) {
      board[randx][randy + i] = type;
    } else {
      board[randx + i][randy] = type;
    }
  }
}

char** generateBoard() {
  // char board(10, char(10, '-')); //
  char **board = {};
  for (int i=0; i < 10; i++) {
    for (int j=0; j < 10; j++) {
      board[i][j] = '-';
    }
  }

  int numblocks = 5;
  char types[5] = {'A', 'B', 'D', 'S', 'P'}; 

  while (numblocks > 0) {
    placeShip(board, numblocks, types[5 - numblocks]);
    numblocks--;
  }

  return board;
}

void drawBoard(char** board, bool debugMode) {
  bool debug = debugMode;
  char letters[10] = {'A', 'B', 'C', 'D', 'E', 'F','G','H','I','J'};

  std::cout << "  1 2 3 4 5 6 7 8 9 10" << std::endl;

  for (int i = 0; i < 10; ++i) {
    char row[20] = "";

    for (int j=0;j<20;j+=2) { //increment by 2
      row[j] = board[i][j];
      row[j+1] = ' ';
    }

    if (!debug) {
        //blank out if not in debug mode
        for (int i = 0; row[i] != '\0'; i++) {
            if (row[i] == 'B' || row[i] == 'D' ||row[i] == 'S' ||row[i] == 'P'||row[i] == 'A' ) {
                row[i] = '-';
            }
        }
    }

    
    std::cout << letters[i] + ' ' + row << std::endl;
  }
}

char* getUserChoice() {
  char result[2];

  while (true) {
    char choice[4];
    std::cout << "Enter coords eg A 1: ";
    std::cin >> choice;

    try {
      char letter = choice[0];
      int num = choice[2]+choice[3];

      if (letter >= 'A' && letter <= 'J' && choice[1] == ' ' && num >= 1 &&
          num <= 10) {
        result[0] = letter;
        result[1] = (num - 1);
        break;
      } else {
        throw std::invalid_argument("not valid");
      }
    } catch (const std::invalid_argument &e) {
      std::cout << e.what() << std::endl;
    }
  }

  return result;
}

int countShips(char type, char** board) {
  int count = 0;
  for (int row = 0; row < 10; ++row) {
    for (int col = 0; col < 10; ++col) {
      if (board[row][col] == type) {
        count++;
      }
    }
  }
  // std::cout << "countships " << count << " " << type << std::endl;
  return count;
}

void play(bool debugMode) {
  char** board = generateBoard();

  drawBoard(board, debugMode);
  bool playing = true;
  int shots = 0;
  int shipssunk = 0;

  while (playing) {
    char guessArray[2] = {*getUserChoice()};
    shots++;

    if (board[guessArray[0]][guessArray[1]] == '-') {
      std::cout << "Miss!" << std::endl;
      board[guessArray[0]][guessArray[1]] = '.';
    } else if (board[guessArray[0]][guessArray[1]] == 'X') {
      std::cout << "you've already used this coordinate - wasted shot!"
                << std::endl;
    } else {
      std::cout << "hit! " << board[guessArray[0]][guessArray[1]] << std::endl;

      if (countShips(board[guessArray[0]][guessArray[1]], board) == 1) {
        std::cout << "---- You've sunk the "
                  << board[guessArray[0]][guessArray[1]] << "------- "
                  << std::endl;
        shipssunk++;
      }

      board[guessArray[0]][guessArray[1]] = 'X';
    }

    std::cout << "Shots used = " << shots << std::endl;
    std::cout << "Ships sunk = " << shipssunk << std::endl;
    drawBoard(board, debugMode);

    if (shots == 40) {
      std::cout << "GAME OVER" << std::endl;
      drawBoard(board, true);
      playing = false;
    }

    if (shipssunk == 5) {
      std::cout << "*****Congrats!! You've sunk all the ships!!******"
                << std::endl;
      playing = false;
    }
  }
}

void instructions() {
  std::cout << "This is a 2D Battleship game.\n";
  std::cout << "You will be asked to place ships on the board.\n";
  std::cout << "You will be asked to guess where the computers ships are.\n";
  std::cout << "The computer will hide ships of various sizes across the 10 x "
               "10 board.\n";
  std::cout << "On each turn, the player 'fires' into the grid, supplying an "
               "x-coordinate and a y-coordinate.\n";
  std::cout
      << "These coordinates will be supplied as a row and collumn number\n";
  std::cout << "The rows will be labelled with the letters A-J and the "
               "collumns will be labelled with numbers 1-10.\n";
  std::cout << "The computer will then respond with a hit or miss (This "
               "information will be displayed on the console).\n";
  std::cout
      << "Based on that, the player will then decide where to fire next.\n";
  std::cout << "The game will continue until either the player or the computer "
               "has sunk all of their opponents ships.\n";
  std::cout << "If all the players ammunition is used up without sinking all "
               "of their opponents ships, the players loses.\n";
  std::cout << "Ship Type Designators:\n";
  std::cout << "A - Airship Carrier - 5 units\n";
  std::cout << "B - Battleship - 4 units\n";
  std::cout << "D - Destroyer - 3 units\n";
  std::cout << "S - Submarine - 2 units\n";
  std::cout << "P - Patrol Boat - 1 unit\n";
}

void debug() {
  std::cout << "###### DEBUG MODE ######\n";
  play(true);
}

void menu() {
  while (true) {
    int choice = 0;

    while (choice < 1 || choice > 4) {
      std::cout
          << "Choose an option\n1: Play\n2: Instructions\n3: Debug\n4: Quit"
          << std::endl;
      std::cin >> choice;
    }

    if (choice == 1) {
      play(false);
    } else if (choice == 2) {
      instructions();
    } else if (choice == 3) {
      debug();
    } else if (choice == 4) {
      std::cout << "goodbye" << std::endl;
      break;
    }
  }
}

menu();
