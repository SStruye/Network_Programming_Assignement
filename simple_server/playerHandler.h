#ifndef PLAYERHANDLER_H
#define PLAYERHANDLER_H

#include <iostream>
#include <vector>
#include "deck.h"
#include <string>
#include <cstring>
#include <QString>
#include <QList>
#include <zmq.hpp>
#include "game.h"

class playerHandler
{
public:
    playerHandler();
    std::vector<std::string> players;
    std::vector<std::string> msgBuffer;
    std::vector<game> gameVector;

    bool full = false;

    void connect(std::string compare, std::string insert);
    void fillVector(std::vector<game>&, std::string p1, std::string p2);
    void disconnect(std::string insert);
    void commandHandler(zmq::message_t *msg);
    void getID(std::string insert);
    void FillBuffer();
    std::string sendHandler();
    void printstatus();  
    int currentID;
    std::string prev;

    ~playerHandler();
};

#endif // PLAYERHANDLER_H
