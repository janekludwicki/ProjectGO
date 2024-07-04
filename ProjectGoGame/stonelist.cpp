#include<stdio.h>
#include<string.h>
#include "StoneList.h"
#include "conio2.h"

void AddToList(StoneList*& head, int x, int y)
{
	if (head == nullptr)
	{
		head = new StoneList;
		head->x = x;
		head->y = y;
	}
	else
	{
		StoneList* temp = head;
		while (temp->next != nullptr)
		{
			temp = temp->next;
		}

		StoneList* newElement = new StoneList;
		newElement->x = x;
		newElement->y = y;
		temp->next = newElement;
	}
}

void ClearList(StoneList*& head) {
	if (head == nullptr)
		return;
	while (head != nullptr)
	{
		StoneList* temp = head;
		if (temp == nullptr)
			return;
		if (temp == head) {
			head = nullptr;
			delete temp;
			return;
		}
		while (temp->next != nullptr)
		{
			temp = temp->next;
		}
		StoneList* temp2 = head;
		while (temp2->next != temp)
		{
			temp2 = temp2->next;
		}
		temp2->next = nullptr;
		delete temp;
	}
}

void RemoveStone(StoneList*& head, int x, int y) {
	StoneList* temp = head;
	if (head == nullptr)
		return;
	while (temp->next != nullptr)
	{
		if ((temp->x == x) && (temp->y == y))
			break;
		temp = temp->next;
	}
	if (temp == head) {
		head = head->next;
		temp = nullptr;
		delete temp;
		return;
	}
	StoneList* temp2 = head;
	while (temp2->next != temp)
	{
		temp2 = temp2->next;
	}
	if (temp->next == nullptr) {
		temp = nullptr;
		delete temp;
		temp2->next = nullptr;
		return;
	}
	else
	{
		temp2->next = temp->next;
		temp = nullptr;
		delete temp;
	}
}

bool CheckIfExists(StoneList* head, int x, int y) {
	if (head == nullptr)
		return false;
	StoneList* temp = head;
	while (temp->next != nullptr)
	{
		if ((temp->x == x) && (temp->y == y))
			return true;
		temp = temp->next;
	}
	return false;
}

void MakePlayerListsIdentical(Player& player) {
	if (player.copyHead == nullptr)
		return;
	StoneList* temp = player.copyHead;
	while (temp->next != nullptr)
	{
		if (!CheckIfExists(player.stonesHead, temp->x, temp->y))
			RemoveStone(player.copyHead, temp->x, temp->y);
		temp = temp->next;
	}
	return;
}

bool CheckIfListsIdenectical(Player player) {
	if (player.copyHead == nullptr)
		return false;
	StoneList* temp2 = player.copyHead;
	while (temp2->next != nullptr)
	{
		if (!CheckIfExists(player.stonesHead, temp2->x, temp2->y))
			return false;
		temp2 = temp2->next;
	}
	return true;
}

void changePlayer(Player& currentPlayer, Player& secondPlayer) {
	Player temp = currentPlayer;
	currentPlayer = secondPlayer;
	secondPlayer = temp;
}
