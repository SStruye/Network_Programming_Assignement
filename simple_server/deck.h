#ifndef DECK_H
#define DECK_H

#include <iostream>
#include <vector>
#include <algorithm>

class Card {
    public :
        std::string suit;
        std::string number;

        Card();
        Card(std::string s, std::string num);

};

class Deck : public Card{
    std::vector<Card> *deckOfCards;
    std::string SUITS[4] = {"hearts","diamonds","spades","clubs"};
    std::string TYPE[13] = {"14","2","3","4","5","6","7","8","9","10","11","12","13"};
    public:
    Deck();
    std::string draw();
    void shuffle();
    int GetSize();
    bool CheckAnswer;
    ~Deck();
};
#endif // DECK_H
