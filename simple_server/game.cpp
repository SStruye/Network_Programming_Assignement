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
    std::string score1;
    std::string score2;
    if(turn){                                                                       //Check whos turn it is -> create msgs accordingly
        guesser = playerOne + '>' + prevCard +  "Guess Higher or Lower (h/l): >";
        opponent = playerTwo + '>' + prevCard + "Opponent is guessing...>";
    }
    else{
        guesser = playerTwo + '>' + prevCard +  "Guess Higher or Lower (h/l): >";
        opponent = playerOne + '>' + prevCard + "Opponent is guessing...>";
    }
    score1 = playerOne + '>' + "SCORE: " + std::to_string(scoreP1) + " : " + std::to_string(scoreP2) + '>';
    score2 = playerTwo + '>' + "SCORE: " + std::to_string(scoreP2) + " : " + std::to_string(scoreP1) + '>';
    msgBuffer.push_back(guesser);                                                   //Put msgs in msg buffer
    msgBuffer.push_back(opponent);
    msgBuffer.push_back(score1);
    msgBuffer.push_back(score2);
    msgBuffer.push_back(guesBuf);                                                   //Put msgs in msg buffer
    msgBuffer.push_back(oppBuf);
}

//--------------------------------------------------------------------------------------------------------------
// Func : checkGuess(int answer, std::string insert);
// Use  : draw new card and compare with previous, return 0: incorrect, 1: correct, 2: message from wrong player
// Param: answer = guess from guesser ; insert = sender + delim + opponent
//--------------------------------------------------------------------------------------------------------------
int game::checkGuess(int answer, std::string insert, int bluf){
    int msg;
    if(((insert == playerOne) && turn) || ((insert == playerTwo) && !turn)){    // Check if it's sending players turn
        if(answer == 3){
            if((getNumber(prevCard) + bluf)<=getNewCard()){ msg = 1; }                   // Check if higher is correct
            else { msg = 0; }
            prevCard = newCard;
        }
        else{
            if((getNumber(prevCard) - bluf)>=getNewCard()){ msg = 1; }                   // Check if lower is correct
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
void game::scoreHandler(int answer, int bluf){
    printf("in scoreHandler\n");
    std::string guesser;
    std::string opponent;
    if(answer){                                                                 // If answer = correct
        if(turn){                                                               // Check turn
            if(bluf > 0){
                guesser = playerOne + '>' + "smg>" + "You BLUFFED correct!>";       // create msgs accordingly
                opponent = playerTwo + '>' + "smg>" +"Opponent BLUFFED correct!>";
                scoreP1 += bluf;
            }else{
                guesser = playerOne + '>' + "smg>" + "You guessed correct!>";       // create msgs accordingly
                opponent = playerTwo + '>' + "smg>" +"Opponent guessed correct!>";

            }
            scoreP1++;
        }
        else{
            if(bluf > 0){
                guesser = playerTwo + '>' + "smg>" + "You BLUFFED correct!>";
                opponent = playerOne + '>' + "smg>" +"Opponent BLUFFED correct!>";
                scoreP2 += bluf;
            }else{
                guesser = playerTwo + '>' + "smg>" + "You guessed correct!>";
                opponent = playerOne + '>' + "smg>" +"Opponent guessed correct!>";

            }
            scoreP2++;
        }
    }
    if(!answer){                                                                // If answer = correct
        if(turn){                                                               // Check turn
            if(bluf > 0){
                guesser = playerOne + '>' + "smg>" + "You BLUFFED wrong!>";       // create msgs accordingly
                opponent = playerTwo + '>' + "smg>" +"Opponent BLUFFED wrong!>";
            }else{
                guesser = playerOne + '>' + "smg>" + "You guessed wrong!>";       // create msgs accordingly
                opponent = playerTwo + '>' + "smg>" +"Opponent guessed wrong!>";
            }
        }
        else{
            if(bluf > 0){
                guesser = playerTwo + '>' + "smg>" + "You BLUFFED wrong!>";
                opponent = playerOne + '>' + "smg>" +"Opponent BLUFFED wrong!>";
            }else{
                guesser = playerTwo + '>' + "smg>" + "You guessed wrong!>";
                opponent = playerOne + '>' + "smg>" +"Opponent guessed wrong!>";
            }
        }
    }
    guesBuf = guesser;
    oppBuf = opponent;
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
