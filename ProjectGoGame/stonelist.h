#pragma once

struct StoneList {
	int x;
	int y;
	StoneList* next = nullptr;
};

struct Player {
	char sign;
	float score = 0;
	//current player's stone list with their coordinates on board
	StoneList* stonesHead = nullptr;
	//player's stones list as old as 2 turns ago(it resets when player places a new stone)
	StoneList* copyHead = nullptr;
};

void AddToList(StoneList*& head, int x, int y);

void ClearList(StoneList*& head);

void RemoveStone(StoneList*& head, int x, int y);

bool CheckIfExists(StoneList* head, int x, int y);

//Makes both players lists identical 
void MakePlayerListsIdentical(Player& player);

//Checks if players list are identitical
bool CheckIfListsIdenectical(Player player);

void changePlayer(Player& currentPlayer, Player& secondPlayer);