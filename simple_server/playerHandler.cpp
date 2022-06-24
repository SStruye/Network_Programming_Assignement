#include "playerHandler.h"

playerHandler::playerHandler(){
}

//--------------------------------------------------------------------------------------------
// Func : connect(std::string compare, std::string insert)
// Use  : If msg = '1', add sender to player pool. If there is a match, add match to game pool
// Param: comapare = opponent + delim + sender ; insert = sender + delim + opponent
//--------------------------------------------------------------------------------------------
void playerHandler::connect(std::string compare, std::string insert){
    bool dupe = false;
    bool match = false;
    for(int i = 0; i < (int)players.size(); i++){       // Iterate player vector
        std::cout <<"comparing.." << std::endl;
        if(players.at(i).compare(insert)==0){           // Check if new player is duplicate
            std::cout <<"Already Connected" << std::endl;
            match = true;                               //match & dupe = true
            dupe = true;
        }
    }
    if(!dupe){
        for(int i = 0; i < (int)players.size(); i++){   // Iterate player vector
            if(players.at(i).compare(compare)==0){      // Check if new player's compare is already connected
                std::cout <<"Match made" << std::endl;
                std::cout <<"Players added to match pool" << std::endl;
                fillVector(gameVector, insert, compare);// If connected create new game with players
            }
        }
    }
    if(!match){                                         // If !match add player to player vector
        std::cout << "player added to player pool" << std::endl;
        players.push_back(insert);
    }
}

//-------------------------------------------------------------------------------------------------
// Func : fillVector(std::vector<game>&, std::string p1, std::string p2)
// use  : Add new game to game vector with player names & new game ID
// Param: vector<game>& = game vector ; p1 = name player1 ; p2 = name player2
//-------------------------------------------------------------------------------------------------
void playerHandler::fillVector(std::vector<game>&, std::string p1, std::string p2){
    currentID = (int)gameVector.size();                 // ID for new game
    game newGame(p1, p2, (int)gameVector.size());       // Init new game
    gameVector.push_back(newGame);                      // Add new game to game vector
    FillBuffer();                                       // Fill msg buffer & send
    full = true;
}

//-------------------------------------------------------------------------------------------------
// Func : disconnect(std::string insert)
// use  : If msg = '2', delete sender from game & player pool, destroy game instance and shift ID's
// Param: insert = sender + delim + opponent
//-------------------------------------------------------------------------------------------------
void playerHandler::disconnect(std::string insert){
    std::cout <<"Player disconnected" << std::endl;
    for(int i = 0; i<(int)gameVector.size();i++){                   // Search players game in game vector
        if(insert == gameVector.at(i).playerOne || insert == gameVector.at(i).playerTwo){
            gameVector.erase(gameVector.begin()+i);                 // Erase players game from game vector
            std::cout <<"Player deleted from matched pool" << std::endl;
            for(int n = i; n<(int)gameVector.size(); n++){          // Shift ID's from games with bigger ID's back by 1
                gameVector.at(n).gameID = (int)gameVector.at(n).gameID - 1;
            }
        }
    }
    for(int i = 0; i<(int)players.size();i++){                      // Search player in player vector
        if(insert == players.at(i)){
            players.erase(players.begin()+i);                       // Erase player from player vector
            std::cout <<"Player deleted from player pool" << std::endl;
        }
    }
}

//----------------------------------------------------------------------------
// Func : commandHandler(zmq::message_t *msg)
// Use  : Filter message from subscription and execute corresponding functions
// Param: *msg = message read by subscriber
//----------------------------------------------------------------------------
void playerHandler::commandHandler(zmq::message_t *msg){
    int cmd = 0;
    int bluf = 0;
    bool bluffed = false;
    std::string message( (char*) msg->data(), msg->size());         // Split incoming msg with delimiter & store in variables
    QString str = QString::fromStdString(message);
    QStringList list_recv = str.split(QLatin1Char('>'));
    std::string compare = list_recv[3].toStdString() + '>' + list_recv[2].toStdString();
    std::string insert = list_recv[2].toStdString() + '>' + list_recv[3].toStdString();

    cmd = stoi(list_recv[4].toStdString());                         // Take value from 5th item from list as command

    bluf = stoi(list_recv[5].toStdString());
    std::cout << "****BLUF = " << bluf << std::endl;
    if(bluf > 0){
        bluffed = true;
    }

    getID(insert);                                                  // Get ID from current game
    int id = currentID;

    switch(cmd){                                                    // Switch case handling command
    case 0:
        break;
    case 1: connect(compare, insert);                               // Case 1: Connect new player
        break;
    case 2: disconnect(insert);                                     // Case 2: Disconnect current player
        break;
    case 3: {int check = gameVector.at(id).checkGuess(cmd, insert, bluf);   // Case 3: Player picked higher -> check guess
                if(check != 2){                                             // If 2 = msg is from wrong player
                    gameVector.at(id).scoreHandler(check, bluf);                  // Calc score
                    gameVector.at(id).getGuess();                           // Get new guess
                    FillBuffer();                                           // Fill msg buffer with buffer from current game
                    full = true;                                            // Initiate sendHandler in main
                }
            } break;
    case 4: {int check = gameVector.at(id).checkGuess(cmd, insert, bluf); // Case 4: Player picked lower
                if(check != 2){                                     // Same principal as case 3
                    gameVector.at(id).scoreHandler(check, bluf);
                    gameVector.at(id).getGuess();
                    FillBuffer();
                    full = true;
                }
            } break;
    default: break;
    }
}

//------------------------------------------------------------------
// Func : playerHandler::getID(std::string insert)
// Use  : Get game ID from current game with the use of player names
// Param: insert = sender + delim + opponent
//------------------------------------------------------------------
void playerHandler::getID(std::string insert){
    for(int i = 0; i<(int)gameVector.size();i++){                                           // Iterate vector of games
        if(insert == gameVector.at(i).playerOne || insert == gameVector.at(i).playerTwo){   // If insert = player from game @ current iteration
            currentID = gameVector.at(i).gameID;                                            // Set current game ID
        }
    }
}

//----------------------------------------------------------
// Func : playerHandler::FillBuffer()
// Use  : Fill message buffer with message from current game
// Param: None
//----------------------------------------------------------
void playerHandler::FillBuffer(){
    int id = currentID;
    for(int i = 0; i<(int)gameVector.at(id).msgBuffer.size();i++){  // Iterate msgbuffer from current game
        msgBuffer.push_back(gameVector[id].msgBuffer[i]);           // Put elements in msg buffer from playerhandler
    }
    gameVector[id].msgBuffer.clear();                               // Clear msgbuffer from current game
}

//--------------------------------------------
// Func : playerHandler::sendHandler()
// Use  : Iterate buffer and return messages to sender
// Param: None
//--------------------------------------------
std::string playerHandler::sendHandler(){
        std::string temp = msgBuffer.at((int)msgBuffer.size()-1);   // Set temporary to last in msgbuffer
        msgBuffer.pop_back();                                       // Delete last in msgbuffer
        return temp;                                                // Return temporary
}

//------------------------------------------------
// Func : playerHandler::printstatus()
// Use  : Print current active games and connected players
// Param: None
//------------------------------------------------
void playerHandler::printstatus(){
    std::cout << "Matched: " << (int)gameVector.size() << "// players: " << (int)players.size() << std::endl;
}

playerHandler::~playerHandler(){
}
