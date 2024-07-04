# Project Go Game
### Overview
This project is an implementation of the classic board game Go in C++. The game allows two players to place stones on a board, capture opponent stones, and follows traditional Go rules such as the Ko rule. The project is structured with a main game file (`main.cpp`) and a stone list management file (`StoneList.cpp` and `StoneList.h`).

### Preview
![image](https://github.com/janekludwicki/ProjectGO/assets/132893147/a09a9c01-e502-4cbf-8ae0-717f2caff2aa)

![image](https://github.com/janekludwicki/ProjectGO/assets/132893147/23bd927e-d0f8-4e67-8651-0e9c157e2ec5)


### Key Features
1. **Board Creation and Management**:
   - Dynamically sized board with options for standard sizes (19x19, 13x13, 9x9) or custom sizes.
   - Board initialization with border characters and empty cell indicators.

2. **Player and Stone Management**:
   - Players represented by black ('B') and white ('W') stones.
   - Lists to keep track of placed stones and captured stones.

3. **Game Mechanics**:
   - Placing stones on the board.
   - Capturing opponent stones by surrounding them.
   - Implementing the Ko rule to prevent repetitive board states.
   - Checking for groups of stones and their liberties (breaths).
   - Switching turns between players and managing scores.

4. **User Interface**:
   - Text-based interface using `conio2` for screen manipulation and keyboard input.
   - Display legend for controls and current game status.

5. **File Operations**:
   - Save the current game state to a file.
   - Load a game state from a file.

### File Descriptions

#### `main.cpp`
This file contains the main game logic, including board creation, player turns, stone placement, and capturing mechanics. Key functions include:

- **Board Management**:
  - `boardCreate(Board board)`: Initializes the board with borders and empty cells.
  - `BoardWrite(Board board)`: Displays the current state of the board on the screen.

- **Game Mechanics**:
  - `placeTheStone(Board& board, Player& currentPlayer, Player& opponentPlayer)`: Handles stone placement and captures.
  - `CheckIfKillGroup(Board& board, Player currentPlayer, Player opponentPlayer)`: Checks if placing a stone results in any captures.
  - `KoRule(Player currentPlayer, Board& board)`: Implements the Ko rule to prevent repetitive captures.

- **User Interface**:
  - `Legend(Board board)`: Displays game controls and current position.
  - `SizeOfTheBoard(Board& board)`: Allows the user to select the board size.

- **File Operations**:
  - `Save(Board board, Player currentPlayer, Player opponentPlayer)`: Saves the current game state to a file.
  - `Load(Board& board, Player& currentPlayer, Player& opponentPlayer)`: Loads a game state from a file.

#### `StoneList.cpp` and `StoneList.h`
These files manage the linked lists of stones for each player. Key functions include:

- **List Management**:
  - `AddToList(StoneList*& head, int x, int y)`: Adds a stone to the player's list.
  - `ClearList(StoneList*& head)`: Clears a player's stone list.
  - `RemoveStone(StoneList*& head, int x, int y)`: Removes a stone from the player's list.

- **Utility Functions**:
  - `CheckIfExists(StoneList* head, int x, int y)`: Checks if a stone exists at a given position in the list.
  - `MakePlayerListsIdentical(Player& player)`: Copies the current stone list to the player's history list.
  - `CheckIfListsIdenectical(Player player)`: Checks if the current stone list is identical to the history list.

### Controls:
   - Arrow keys to move the cursor.
   - 'i' to place a stone.
   - 's' to save the game.
   - 'l' to load a game.
   - 'n' to start a new game.
   - 'q' to quit the game.
   - 'e' for game state editor (only at the start).
   - 'Enter' to confirm configuration (in game state editor).
   - 'ESC' to cancel the current action.

