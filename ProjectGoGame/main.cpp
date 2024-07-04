#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include"conio2.h"
#include "StoneList.h"
#define BOARDX 35
#define BOARDY 1
#define EMPTYCELLSIGN '+'
#define BLACKPLAYERSIGN 'B'
#define WHITEPLAYERSIGN 'W'
#define LEGENDPOSX 2
#define LEGENDPOSY 1
#define LISTENDSIGN -1
#define BOTTOMTOPBORDERSIGN '_'
#define LEFTRIGHTBORDERSIGN '|'
#define ESCASCII 27

struct KoBlockedCell
{
    int x = NULL;
    int y = NULL;
};

struct Board {
    char** board;
    int x = BOARDX + 1;
    int y = BOARDY + 1;
    int coordinateX = x - BOARDX;
    int coordinateY = y - BOARDY;
    int sizeX;
    int sizeY;
    bool ThereWasKill = false;
    int Turn = 0;
    bool GameStateEditor = false;
    KoBlockedCell BlockedCell;
};

bool IsDigit(char c) {
    int digit = c - '0';
    if ((digit >= 0) && (digit <= 9))
        return true;
    return false;
}

//Adds stones taken into captivity in previous turn(if there are any)
void AddDeletedElementsToBoard(Board& Board, Player& opponentPlayer, Player& currentPlayer) {
    if (opponentPlayer.copyHead == nullptr)
        return;
    StoneList* temp = opponentPlayer.copyHead;
    while (temp->next != nullptr)
    {
        if (!CheckIfExists(opponentPlayer.stonesHead, temp->x, temp->y)) {
            AddToList(opponentPlayer.stonesHead, temp->x, temp->y);
            Board.board[temp->y][temp->x] = opponentPlayer.sign;
            currentPlayer.score--;
        }
        temp = temp->next;
    }
}

//Deletes most recently added stones to the player's list
void DeleteLastAddedElement(Board& Board, Player& player) {
    if (player.stonesHead == nullptr)
        return;
    StoneList* temp = player.stonesHead;
    while (temp->next != nullptr)
    {
        temp = temp->next;
    }
    Board.board[temp->y][temp->x] = EMPTYCELLSIGN;
    RemoveStone(player.stonesHead, temp->x, temp->y);
}

void boardCreate(Board Board) {
    for (int j = 0; j < Board.sizeY + 2; j++) {
        for (int k = 0; k < Board.sizeX + 2; k++) {
            if ((j == 0) || (j == Board.sizeY + 1))
                Board.board[j][k] = BOTTOMTOPBORDERSIGN;
            else if ((j != 0) && (j != Board.sizeY + 1) && ((k == 0) || (k == Board.sizeX + 1)))
                Board.board[j][k] = LEFTRIGHTBORDERSIGN;
            else
                Board.board[j][k] = EMPTYCELLSIGN;
        }
        Board.board[j][Board.sizeX + 2] = '\0';
    }
    return;
}

void SizeOfTheBoard(Board& board) {
    clrscr();
    textcolor(7);
    gotoxy(1, 1);
    cputs("Choose the size of the board (click the number on the keyboard):");
    gotoxy(1, 2);
    cputs("1. 19x19");
    gotoxy(1, 3);
    cputs("2. 13x13");
    gotoxy(1, 4);
    cputs("3. 9x9");
    gotoxy(1, 5);
    cputs("4. Custom size: ");
    char choice = getch();
    if (choice == '1') {
        board.sizeX = 19;
        board.sizeY = 19;
        return;
    }
    else if (choice == '2') {
        board.sizeX = 13;
        board.sizeY = 13;
        return;
    }
    else if (choice == '3') {
        board.sizeX = 9;
        board.sizeY = 9;
        return;
    }
    else if (choice == '4') {
        clrscr();
        struct text_info information;
        gettextinfo(&information);
        char y, x;
        int sizeX = 0, sizeY = 0;
        gotoxy(2, 2);
        cputs("X: ");
        gotoxy(5, 2);
        x = getche();
        while (IsDigit(x)) {
            sizeX = sizeX * 10 + x - '0';
            x = getche();
        }
        if (sizeX > information.screenwidth - 36)
        {
            clrscr();
            cputs("Too big width!");
            x = getch();
            SizeOfTheBoard(board);
        }
        clrscr();
        gotoxy(2, 2);
        cputs("Y: ");
        gotoxy(5, 2);
        y = getche();
        while (IsDigit(y)) {
            sizeY = sizeY * 10 + y - '0';
            y = getche();
        }
        if (sizeY > information.screenheight - 2)
        {
            clrscr();
            cputs("Too big height!");
            y = getch();
            SizeOfTheBoard(board);
        }
        board.sizeX = sizeX;
        board.sizeY = sizeY;
        return;
    }
    else
        SizeOfTheBoard(board);
    return;
}

void Legend(Board Board) {
    char txt[32];
    textcolor(LIGHTGRAY);
    textbackground(BROWN);
    gotoxy(LEGENDPOSX, LEGENDPOSY);
    cputs("Jan Ludwicki 188899");
    gotoxy(LEGENDPOSX, LEGENDPOSY + 1);
    cputs("cursors = moving");
    gotoxy(LEGENDPOSX, LEGENDPOSY + 2);
    cputs("q       = exit");
    gotoxy(LEGENDPOSX, LEGENDPOSY + 3);
    cputs("n       = new game");
    gotoxy(LEGENDPOSX, LEGENDPOSY + 4);
    cputs("s       = save game");
    gotoxy(LEGENDPOSX, LEGENDPOSY + 5);
    cputs("l       = load game");
    gotoxy(LEGENDPOSX, LEGENDPOSY + 6);
    cputs("esc     = cancel current action");
    gotoxy(LEGENDPOSX, LEGENDPOSY + 7);
    cputs("i       = place a stone");
    gotoxy(LEGENDPOSX, LEGENDPOSY + 8);
    sprintf(txt, "current position: X: %d, Y: %d", Board.coordinateX, Board.coordinateY);
    cputs(txt);
    gotoxy(LEGENDPOSX, LEGENDPOSY + 9);
    cputs("I did points abcdefghjk");
    if (Board.Turn == 0)
    {
        gotoxy(LEGENDPOSX, LEGENDPOSY + 11);
        cputs("e       = game state editor");
        gotoxy(LEGENDPOSX, LEGENDPOSY + 12);
        cputs("enter   = confirm config");
    }
    return;
}


void KoRule(Player currentPlayer, Board& Board) {
    if (Board.ThereWasKill == false)
        return;
    AddToList(currentPlayer.stonesHead, Board.coordinateX, Board.coordinateY);
    if (CheckIfListsIdenectical(currentPlayer)) {
        Board.ThereWasKill = false;
        Board.BlockedCell.x = Board.coordinateX;
        Board.BlockedCell.y = Board.coordinateY;
        return;
    }
    return;
}

//This function groups Stones that are next to each other into one list and determines if they as a whole has
//any breaths
void StoneGroup(StoneList*& groupHead, int x, int y, Board Board, bool& HasNoBreaths, Player player) {
    if (HasNoBreaths == false)
        return;
    if ((Board.board[y + 1][x] == EMPTYCELLSIGN) || (Board.board[y - 1][x] == EMPTYCELLSIGN)
        || (Board.board[y][x + 1] == EMPTYCELLSIGN) || (Board.board[y][x - 1] == EMPTYCELLSIGN)) {
        HasNoBreaths = false;
        return;
    }
    AddToList(groupHead, x, y);
    if ((Board.board[y + 1][x] == player.sign) && (!CheckIfExists(groupHead, x, y + 1)))
        StoneGroup(groupHead, x, y + 1, Board, HasNoBreaths, player);
    if ((Board.board[y - 1][x] == player.sign) && (!CheckIfExists(groupHead, x, y - 1)))
        StoneGroup(groupHead, x, y - 1, Board, HasNoBreaths, player);
    if ((Board.board[y][x + 1] == player.sign) && (!CheckIfExists(groupHead, x + 1, y)))
        StoneGroup(groupHead, x + 1, y, Board, HasNoBreaths, player);
    if ((Board.board[y][x - 1] == player.sign) && (!CheckIfExists(groupHead, x - 1, y)))
        StoneGroup(groupHead, x - 1, y, Board, HasNoBreaths, player);
}

//This function return if Stone/Group Stones(started by stone on coordinates x,y) has any breaths
bool GroupBreath(Board Board, Player player, int x, int y) {
    StoneList* groupHead = nullptr;
    bool HasNoBreaths = true;
    bool boardChanged = false;
    if (Board.board[y][x] == EMPTYCELLSIGN) {
        Board.board[y][x] = player.sign;
        boardChanged = true;
    }
    StoneGroup(groupHead, x, y, Board, HasNoBreaths, player);
    ClearList(groupHead);
    delete groupHead;
    if (boardChanged)
        Board.board[y][x] = EMPTYCELLSIGN;
    return HasNoBreaths;
}

bool CheckIfKillIfPlaced(Player currentPlayer, Player opponentPlayer, Board Board) {
    Board.board[Board.coordinateY][Board.coordinateX] = currentPlayer.sign;
    if ((Board.board[Board.coordinateY + 1][Board.coordinateX] == opponentPlayer.sign) && (GroupBreath(Board, opponentPlayer, Board.coordinateX, Board.coordinateY + 1))) return true;
    else if ((Board.board[Board.coordinateY - 1][Board.coordinateX] == opponentPlayer.sign) && (GroupBreath(Board, opponentPlayer, Board.coordinateX, Board.coordinateY - 1))) return true;
    else if ((Board.board[Board.coordinateY][Board.coordinateX + 1] == opponentPlayer.sign) && (GroupBreath(Board, opponentPlayer, Board.coordinateX + 1, Board.coordinateY))) return true;
    else if ((Board.board[Board.coordinateY][Board.coordinateX - 1] == opponentPlayer.sign) && (GroupBreath(Board, opponentPlayer, Board.coordinateX - 1, Board.coordinateY))) return true;
    Board.board[Board.coordinateY][Board.coordinateX] = EMPTYCELLSIGN;
    return false;
}

//This function gathers same player's stones next to each other into one group and takes them into opponent players' 
//captivity
void GroupKill(Board& Board, Player& opponentPlayer, Player& currentPlayer, int x, int y) {
    StoneList* groupHead = nullptr;
    bool Kill = true;
    StoneGroup(groupHead, x, y, Board, Kill, opponentPlayer);
    if ((Kill == true) && (groupHead != nullptr)) {
        StoneList* temp = groupHead;
        Board.ThereWasKill = true;
        while (temp->next != nullptr)
        {
            RemoveStone(opponentPlayer.stonesHead, temp->x, temp->y);
            Board.board[temp->y][temp->x] = EMPTYCELLSIGN;
            currentPlayer.score++;
            temp = temp->next;
        }
        RemoveStone(opponentPlayer.stonesHead, temp->x, temp->y);
        Board.board[temp->y][temp->x] = EMPTYCELLSIGN;
        currentPlayer.score++;
    }
    ClearList(groupHead);
    delete groupHead;
}

//After placing a stone this function checks if there are any opponent's stones next to it and then checks if 
//by placing a stone any of them got taken into captivity(alone or in a group)
void CheckIfKillGroup(Board& Board, Player currentPlayer, Player opponentPlayer) {
    if (Board.board[Board.coordinateY + 1][Board.coordinateX] == opponentPlayer.sign)
        GroupKill(Board, opponentPlayer, currentPlayer, Board.coordinateX, Board.coordinateY + 1);
    if (Board.board[Board.coordinateY - 1][Board.coordinateX] == opponentPlayer.sign)
        GroupKill(Board, opponentPlayer, currentPlayer, Board.coordinateX, Board.coordinateY - 1);
    if (Board.board[Board.coordinateY][Board.coordinateX + 1] == opponentPlayer.sign)
        GroupKill(Board, opponentPlayer, currentPlayer, Board.coordinateX + 1, Board.coordinateY);
    if (Board.board[Board.coordinateY][Board.coordinateX - 1] == opponentPlayer.sign)
        GroupKill(Board, opponentPlayer, currentPlayer, Board.coordinateX - 1, Board.coordinateY);
}

void ListsDeleter(Player& currentPlayer, Player& opponentPlayer) {
    ClearList(currentPlayer.stonesHead);
    delete currentPlayer.stonesHead;
    ClearList(opponentPlayer.stonesHead);
    delete opponentPlayer.stonesHead;
    ClearList(currentPlayer.copyHead);
    delete currentPlayer.copyHead;
    ClearList(opponentPlayer.copyHead);
    delete opponentPlayer.copyHead;
}

void SaveList(FILE*& save, StoneList* head) {
    StoneList* temp = head;
    while (temp->next != nullptr)
    {
        fprintf(save, "%d %d ", temp->x, temp->y);
        temp = temp->next;
    }
    fprintf(save, "%d %d ", temp->x, temp->y);
    fprintf(save, "%d\n", LISTENDSIGN);
}

void Save(Board Board, Player currentPlayer, Player opponentPlayer) {
    clrscr();
    gotoxy(LEGENDPOSX, LEGENDPOSY);
    cputs("How do you want to name the file?:\n");
    char filename[32];
    int length = 0;
    int c = getche();
    while (c != '\r')
    {
        filename[length] = c;
        length++;
        c = getche();
    }
    filename[length] = '.';
    filename[length + 1] = 't';
    filename[length + 2] = 'x';
    filename[length + 3] = 't';
    filename[length + 4] = '\n';
    filename[strcspn(filename, "\n")] = '\0';
    FILE* save = fopen(filename, "w");
    if (save == nullptr) {
        clrscr();
        cputs("The File did not open!!!");
        return;
    }
    fprintf(save, "%d\n%d\n%d\n%d %f\n%d %f\n%d %d\n%d\n", Board.sizeX, Board.sizeY, Board.Turn, currentPlayer.sign,
        currentPlayer.score, opponentPlayer.sign, opponentPlayer.score, Board.BlockedCell.x, Board.BlockedCell.y, Board.GameStateEditor);
    if (currentPlayer.stonesHead != nullptr) {
        SaveList(save, currentPlayer.stonesHead);
        SaveList(save, currentPlayer.copyHead);
    }
    if (opponentPlayer.stonesHead != nullptr) {
        SaveList(save, opponentPlayer.stonesHead);
        SaveList(save, opponentPlayer.copyHead);
    }
    fclose(save);
}

void LoadList(Player& player, FILE*& save, Board& Board, bool stonesHead) {
    int o;
    fscanf(save, "%d", &o);
    while (o != LISTENDSIGN)
    {
        int x = o, y;
        fscanf(save, " %d", &y);
        AddToList(player.stonesHead, x, y);
        if (stonesHead)
            Board.board[y][x] = player.sign;
        fscanf(save, " %d", &o);
    }
}

void Load(Board& Board, Player& currentPlayer, Player& opponentPlayer) {
    clrscr();
    gotoxy(LEGENDPOSX, LEGENDPOSY);
    cputs("What is the name of the file you want to load?:\n");
    char filename[32];
    int length = 0;
    int c = getche();
    while (c != '\r')
    {
        filename[length] = c;
        length++;
        c = getche();
    }
    filename[length] = '.';
    filename[length + 1] = 't';
    filename[length + 2] = 'x';
    filename[length + 3] = 't';
    filename[length + 4] = '\n';
    filename[strcspn(filename, "\n")] = '\0';
    FILE* save = fopen(filename, "r");
    if (save == nullptr) {
        clrscr();
        cputs("The File did not open!!!");
        return;
    }
    fscanf(save, "%d\n%d\n%d\n%d %f\n%d %f\n%d %d\n%d\n", &Board.sizeX, &Board.sizeY, &Board.Turn, &currentPlayer.sign,
        &currentPlayer.score, &opponentPlayer.sign, &opponentPlayer.score, &Board.BlockedCell.x, &Board.BlockedCell.y, &Board.GameStateEditor);
    Board.board = new char* [Board.sizeY + 2];
    for (int i = 0; i < Board.sizeY + 2; i++)
        Board.board[i] = new char[Board.sizeX + 3];
    boardCreate(Board);
    LoadList(currentPlayer, save, Board, true);
    LoadList(currentPlayer, save, Board, false);
    LoadList(opponentPlayer, save, Board, true);
    LoadList(opponentPlayer, save, Board, false);
    fclose(save);
}

void BoardWrite(Board Board) {
    for (int i = 0; i < Board.sizeY + 2; i++) {
        gotoxy(BOARDX, BOARDY + i);
        cputs(Board.board[i]);
        putchar('\n');
    }
    gotoxy(Board.x, Board.y);
}

void CursorMovement(int& zero, Board& Board, int zn) {
    zero = 1;
    zn = getch();
    if ((zn == 0x48) && (Board.y - 1 > BOARDY)) Board.y--, Board.coordinateY--;
    else if ((zn == 0x50) && (Board.y + 1 < BOARDY + Board.sizeY + 1)) Board.y++, Board.coordinateY++;
    else if ((zn == 0x4b) && (Board.x - 1 > BOARDX)) Board.x--, Board.coordinateX--;
    else if ((zn == 0x4d) && (Board.x + 1 < BOARDX + Board.sizeX + 1)) Board.x++, Board.coordinateX++;
}

void NewGame(Board& Board, Player& currentPlayer, Player& opponentPlayer) {
    Board.x = BOARDX + 1;
    Board.y = BOARDY + 1;
    Board.coordinateX = Board.x - BOARDX;
    Board.coordinateY = Board.y - BOARDY;
    Board.ThereWasKill = false;
    Board.GameStateEditor = false;
    opponentPlayer.score = 6.5;
    currentPlayer.score = 0;
    Board.Turn = 0;
    ClearList(currentPlayer.stonesHead);
    ClearList(currentPlayer.copyHead);
    ClearList(opponentPlayer.stonesHead);
    ClearList(opponentPlayer.copyHead);
    boardCreate(Board);
}

void placeTheStone(Board& Board, Player& currentPlayer, Player& opponentPlayer) {
    if (Board.GameStateEditor == true)
    {
        Board.board[Board.coordinateY][Board.coordinateX] = currentPlayer.sign;
        AddToList(currentPlayer.stonesHead, Board.coordinateX, Board.coordinateY);
        AddToList(currentPlayer.copyHead, Board.coordinateX, Board.coordinateY);
        return;
    }
    if ((Board.coordinateX != Board.BlockedCell.x) && (Board.coordinateY != Board.BlockedCell.y)
        && ((!GroupBreath(Board, currentPlayer, Board.coordinateX, Board.coordinateY)) || (CheckIfKillIfPlaced(currentPlayer, opponentPlayer, Board)))) {
        Board.board[Board.coordinateY][Board.coordinateX] = currentPlayer.sign;
        Board.ThereWasKill = false;
        AddToList(currentPlayer.stonesHead, Board.coordinateX, Board.coordinateY);
        AddToList(currentPlayer.copyHead, Board.coordinateX, Board.coordinateY);
        Board.BlockedCell.x = NULL;
        Board.BlockedCell.y = NULL;
        MakePlayerListsIdentical(currentPlayer);
        CheckIfKillGroup(Board, currentPlayer, opponentPlayer);
        changePlayer(currentPlayer, opponentPlayer);
        Board.Turn++;
    }
    else
        return;
}

int main() {
    int zn = 0, zero = 0;
    Player currentPlayer, opponentPlayer;
    currentPlayer.sign = BLACKPLAYERSIGN;
    opponentPlayer.sign = WHITEPLAYERSIGN;
    opponentPlayer.score = 6.5;
    Board Board;
#ifndef __cplusplus
    Conio2_Init();
#endif
    settitle("Jan Ludwicki, 188899");
    textbackground(BROWN);
    _setcursortype(_SOLIDCURSOR);
    SizeOfTheBoard(Board);
    _wscroll = 0;
    Board.board = new char* [Board.sizeY + 2];
    for (int i = 0; i < Board.sizeY + 2; i++)
        Board.board[i] = new char[Board.sizeX + 3];
    boardCreate(Board);
    do {
        clrscr();
        Legend(Board);
        textcolor(BLACK);
        BoardWrite(Board);
        KoRule(currentPlayer, Board);
        zero = 0;
        zn = getch();
        if (zn == 0) {
            CursorMovement(zero, Board, zn);
        }
        else if (zn == 'n') {
            NewGame(Board, currentPlayer, opponentPlayer);
        }
        else if ((zn == 'e') && (Board.Turn == 0))
        {
            Board.GameStateEditor = true;
            opponentPlayer.score += 0.5;
            continue;
        }
        else if ((zn == 'i') && (Board.board[Board.coordinateY][Board.coordinateX] == EMPTYCELLSIGN)) {
            placeTheStone(Board, currentPlayer, opponentPlayer);
        }
        else if ((zn == '\r') && (Board.GameStateEditor))
        {
            Board.GameStateEditor = false;
            Board.Turn++;
            changePlayer(currentPlayer, opponentPlayer);
        }
        else if (zn == 's') {
            Save(Board, currentPlayer, opponentPlayer);
        }
        else if (zn == 'l') {
            Load(Board, currentPlayer, opponentPlayer);
        }
        else if (zn == ESCASCII) {
            if ((Board.Turn == 0) && (!Board.GameStateEditor))
                continue;
            if (Board.GameStateEditor)
                DeleteLastAddedElement(Board, currentPlayer);
            else
            {
                DeleteLastAddedElement(Board, opponentPlayer);
                AddDeletedElementsToBoard(Board, currentPlayer, opponentPlayer);
                changePlayer(currentPlayer, opponentPlayer);
                Board.Turn--;
            }
        }
    } while (zn != 'q');
    _setcursortype(_NORMALCURSOR);
    for (int i = 0; i < Board.sizeY + 2; ++i) {
        delete[] Board.board[i];
    }
    delete[] Board.board;
    ListsDeleter(currentPlayer, opponentPlayer);
    return 0;
}