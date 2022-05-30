/*#include <iostream>
#include <stdio.h>
#include <string>
#include <cstring>
#include <zmq.hpp>
#include <QString>
#include <QList>

#ifndef _WIN32
    #include <unistd.h>
#else
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#endif

int main( void )
{
    std::string topic_recv( "HigherLower>msg?>" );
    std::string topic_send( "HigherLower>msg!>" );

    zmq::context_t context(1);
    zmq::socket_t pusher( context, ZMQ_PUSH );
    zmq::socket_t subscriber( context, ZMQ_SUB );

    pusher.connect( "tcp://localhost:5556" );
    //pusher.connect( "tcp://benternet.pxl-ea-ict.be:24041" );
    subscriber.connect( "tcp://localhost:24042" );
    //subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );
    subscriber.setsockopt( ZMQ_SUBSCRIBE, topic_recv.c_str(), topic_recv.length() );
    std::cout << "Connected to Benternet !" << std::endl;

    zmq::message_t * msg = new zmq::message_t();
    while( subscriber.connected() )
    {
        subscriber.recv( msg );
        //std::cout << "received : [" << std::string ( (char*) msg->data(), msg->size() ) << "]" << std::endl;

        std::string message( (char*) msg->data(), msg->size());
        QString str = QString::fromStdString(message);
        QStringList list_recv = str.split(QLatin1Char('>'));
        std::string msg_send = topic_send + list_recv[3].toStdString() + '>' + list_recv[2].toStdString() + '>' +list_recv[4].toStdString() + '>';
        //std::cout << "message send: " << msg_send << std::endl;
        pusher.send(msg_send.c_str(), msg_send.length());

        std::cout << "From " << list_recv[2].toStdString() << " To "<< list_recv[3].toStdString() << ": " << list_recv[4].toStdString() << std::endl;
    }

    std::cout << "Disconnected from Benternet !" << std::endl;
}*/
