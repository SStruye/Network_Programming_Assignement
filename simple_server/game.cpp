#include "game.h"

//------------------------------------------------------------------------------------------
// Func : game(std::string p1, std::string p2, int id);
// Use  : Init game with names, ID & new deck -> shuffle deck -> draw first card ->get guess
// Param: p1 = name player 1 ; p2 = name player 2 ; id = ID of game
//------------------------------------------------------------------------------------------
game::game(std::string p1, std::string p2, int id)
{
    playerOne = p1;                         // Set player names & ID
    playerTwo = p2;
    gameID = id;
    deckObject = new Deck;                  // Create new deck
    deckObject->shuffle();                  // Shuffle deck
    std::string card = deckObject->draw();  // Draw first card
    prevCard = card;
    getGuess();                             // Request guess
}

//-------------------------------------
// Func : game::getGuess();
// Use  : create msg requesting a guess
// Param: None
//-------------------------------------
void game::getGuess(){
    std::string guesser;
    std::string opponent;
    if(turn){                                                                       //Check whos turn it is -> create msgs accordingly
        guesser = playerOne + '>' + prevCard +  "Guess Higher or Lower (h/l): >";
        opponent = playerTwo + '>' + prevCard + "Opponent is guessing...>";
    }
    else{
        guesser = playerTwo + '>' + prevCard +  "Guess Higher or Lower (h/l): >";
        opponent = playerOne + '>' + prevCard + "Opponent is guessing...>";
    }
    msgBuffer.push_back(guesser);                                                   //Put msgs in msg buffer
    msgBuffer.push_back(opponent);
}

//--------------------------------------------------------------------------------------------------------------
// Func : checkGuess(int answer, std::string insert);
// Use  : draw new card and compare with previous, return 0: incorrect, 1: correct, 2: message from wrong player
// Param: answer = guess from guesser ; insert = sender + delim + opponent
//--------------------------------------------------------------------------------------------------------------
int game::checkGuess(int answer, std::string insert){
    int msg;
    if(((insert == playerOne) && turn) || ((insert == playerTwo) && !turn)){    // Check if it's sending players turn
        if(answer == 3){
            if(getNumber(prevCard)<=getNewCard()){ msg = 1; }                   // Check if higher is correct
            else { msg = 0; }
            prevCard = newCard;
        }
        else{
            if(getNumber(prevCard)>=getNewCard()){ msg = 1; }                   // Check if lower is correct
            else { msg = 0; }
            prevCard = newCard;
        }
    }
    else msg = 2;
    return msg;                                                                 // Return msg
}

//---------------------
// Func : getNewCard();
// Use  : Draw new card
// Param: None
//---------------------
int game::getNewCard(){
    newCard = deckObject->draw();   // Draw new card & put in newCard
    return getNumber(newCard);      // Return number of card as int
}

//-----------------------------------------------
// Func : scoreHandler(int answer);
// Use  : Create message for score
// Param: answer = return from check guess
//-----------------------------------------------
void game::scoreHandler(int answer){
    printf("in scoreHandler\n");
    std::string guesser;
    std::string opponent;
    if(answer){                                                                 // If answer = correct
        if(turn){                                                               // Check turn
            guesser = playerOne + '>' + "smg>" + "You guessed correct!>";       // create msgs accordingly
            opponent = playerTwo + '>' + "smg>" +"Opponent guessed correct!>";
        }
        else{
            guesser = playerTwo + '>' + "smg>" + "You guessed correct!>";
            opponent = playerOne + '>' + "smg>" +"Opponent guessed correct!>";
        }
    }
    if(!answer){                                                                // If answer = correct
        if(turn){                                                               // Check whos turn it is
            guesser = playerOne + '>' + "smg>" + "You guessed wrong!>";         // create msgs accordingly
            opponent = playerTwo + '>' + "smg>" +"Opponent guessed wrong!>";
        }
        else{
            guesser = playerTwo + '>' + "smg>" + "You guessed wrong!>";
            opponent = playerOne + '>' + "smg>" +"Opponent guessed wrong!>";
        }
    }
    msgBuffer.push_back(guesser);                                               //Put msgs in msg buffer
    msgBuffer.push_back(opponent);
    turn = !turn;                                                               //Set turn to opponent
}

//----------------------------------------
// Func : getNumber(std::string number);
// Use  : return number from string as int
// Param: number = card string
//----------------------------------------
int game::getNumber(std::string number){
    QString str = QString::fromStdString(number);           // Create list from string
    QStringList list_recv = str.split(QLatin1Char('>'));
    return stoi(list_recv[0].toStdString());                // Return conversion from string number
}
