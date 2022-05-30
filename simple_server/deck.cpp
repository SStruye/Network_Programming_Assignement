#include "deck.h"

Card::Card() {
}

Card::Card(const std::string s,const std::string num) : suit(s),number(num){
}

//-----------------------
// Func : Deck();
// Use  : Initiate a deck
// Param: None
//-----------------------
Deck::Deck() {
    deckOfCards = new std::vector<Card>();
    deckOfCards->reserve(52);
    for(int i = 0 ; i < 4 ; ++i)
        for(int j = 0 ; j < 13 ; ++j)
            deckOfCards->emplace_back(Card(SUITS[i],TYPE[j]));
}

//------------------------------------
// Func : draw();
// Use  : Draw card & delete from deck
// Param: None
//------------------------------------
std::string Deck::draw() {
    std::string showCard("");
    auto it = deckOfCards->back();
    showCard.append((it.number));
    showCard.append(">");
    showCard.append((it.suit));
    showCard.append(">");
    deckOfCards->pop_back();
    return showCard;
}

//--------------------
// Func : shuffle()
// Use  : Shuffle deck
// Param: None
//--------------------
void Deck::shuffle() {
    srand(time(NULL));
    std::random_shuffle(deckOfCards->begin(),deckOfCards->end());
}

//---------------------------
// Func : GetSize();
// Use  : return size of deck
// Param: None
//---------------------------
int Deck::GetSize() {
   return deckOfCards->size();
}

Deck::~Deck() {
    delete deckOfCards;
}

