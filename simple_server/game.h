#ifndef GAME_H
#define GAME_H

#include "deck.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <QString>
#include <QList>

class game
{
public:
    game(std::string p1, std::string p2, int id);

    int checkGuess(int answer, std::string insert);
    void getGuess();

    std::string prevCard;
    std::string newCard;

    int getNewCard();
    void scoreHandler(int answer);
    void getAnswer();
    int getNumber(std::string number);
    std::vector<std::string> msgBuffer;
    bool bufferFull = false;

    Deck *deckObject;

    bool turn = false;

    int gameID = 0;
    std::string playerOne;
    std::string playerTwo;
};

#endif // GAME_H
