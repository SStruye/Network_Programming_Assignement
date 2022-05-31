#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <zmq.hpp>
#include <QString>
#include <QList>
#include "playerHandler.h"
#include "game.h"
#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

//-----------------------------------
// Func : int main(void);
// Use  : Execute Higher Lower server
// Param: None
//-----------------------------------
int main( void )
{

    std::string topic_recv( "HigherLower>msg?>" );      //Topic for send & receive
    std::string topic_send( "HigherLower>msg!>" );

    zmq::context_t context(1);                          //Connect zmq subrscriber & zmq pusher with corresponding topics
    zmq::socket_t pusher( context, ZMQ_PUSH );
    zmq::socket_t subscriber( context, ZMQ_SUB );
    pusher.connect( "tcp://benternet.pxl-ea-ict.be:24041" );
    subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );
    subscriber.setsockopt( ZMQ_SUBSCRIBE, topic_recv.c_str(), topic_recv.length() );
    zmq::message_t * msg = new zmq::message_t();
    std::cout << "Connected to Benternet !" << std::endl;

    playerHandler PlayerHandler;                        //Create playerHandler object

    while( subscriber.connected() )                     //Continious loop when subscriber = connected
    {
        PlayerHandler.printstatus();                    //Print current active games & connected players
        std::cout << "waiting for msg.." << std::endl;

        subscriber.recv( msg );                         //Receive incoming msg & send msg to commandHandler for further processing
        PlayerHandler.commandHandler(msg);

        if(PlayerHandler.full){                         //If 'bool full' from playerHandler = true, Iterate msg buffer and send elements
            while(PlayerHandler.msgBuffer.size()>0){
                std::string msg_send = topic_send + PlayerHandler.sendHandler();
                std::cout << "send = " << msg_send << std::endl;
                sleep(1);
                pusher.send(msg_send.c_str(), msg_send.length());
            }
            PlayerHandler.msgBuffer.clear();            //Clear msg buffer & set 'bool full' false
            PlayerHandler.full = false;
        }
    }
    std::cout << "Disconnected from Benternet !" << std::endl;
}

//pusher.connect( "tcp://localhost:5556" );             //Connect if hosting local
//subscriber.bind( "tcp://*:24042" );

