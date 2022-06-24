#include <iostream>
#include <zmq.hpp>
#include <string>
#include <cstring>
#include <string>
#include <QString>
#include <QList>
#include <thread>

#ifndef _WIN32
    #include <windows.h>
    #define sleep(n)    Sleep(n)
#else
    #include <unistd.h>
#endif

void receive( zmq::context_t *context, std::string tpc )
{
    try
    {
        zmq::context_t context(1);
        zmq::socket_t subscriber( context, ZMQ_SUB );
        std::string topic = tpc;//"HigherLower>msg!>";



        subscriber.connect( "tcp://benternet.pxl-ea-ict.be:24042" );

        subscriber.setsockopt( ZMQ_SUBSCRIBE, topic.c_str() ,topic.length());

        zmq::message_t * msg = new zmq::message_t();
        while( subscriber.connected() )
        {
            subscriber.recv( msg );
            std::string message( (char*) msg->data(), msg->size());
            //std::cout << message << std::endl;
            QString str = QString::fromStdString(message);
            QStringList list_recv = str.split(QLatin1Char('>'));
            //std::cout << list_recv[3].toStdString() << " drew: " << list_recv[4].toStdString() << std::endl;

            for(int i = 4; i<list_recv.size();i++){
                if(list_recv[i].toStdString() != "smg"){
                    std::cout << list_recv[i].toStdString() << std::endl;
                }
            }
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }
}

void send( zmq::context_t *context, std::string tpc )
{
    try
    {
        zmq::context_t context(1);
        std::string topic = tpc;//( "HigherLower>msg?>seppe>bob>" );
        std::string message;
        std::string bluf;

        zmq::socket_t publisher( context, ZMQ_PUSH );
        publisher.connect( "tcp://benternet.pxl-ea-ict.be:24041" );

        sleep(1);
        std::string send = topic + "1>" + "0>";
        system("CLS");
        publisher.send( send.c_str(),send.length() );

        while(publisher.connected())
        {
           std::cin >> message;
           std::cout << "bluf? (0 if no): " << std::endl;
           std::cin >> bluf;
           if(message == "h" || message == "l" || message == "2" || message == "1"){
               if(message == "h")message = '3';
               if(message == "l")message = '4';
               std::string send = topic + message + '>' + bluf + '>';
               //std::cout << "MSG: " << send << std::endl;
               publisher.send( send.c_str(),send.length());
           }
           else{
               std::cout << "non valid input, try again: " << std::endl;
           }
        }
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }
}

int main( int argc, char *argv[] )
{
    std::string topic_snd( "HigherLower>msg?>" );
    std::string topic_recv( "HigherLower>msg!>" );
    std::string topic;

    std::string temp;
    std::cout << "name: ";
    std::cin >> temp;
    topic = topic + temp +'>';
    std::cout << "corr: ";
    std::cin >> temp;
    topic = topic + temp +'>';


    std::cout << "topic: " << topic << std::endl;

    try
    {
        zmq::context_t context(1);
        std::thread recvThread( receive, &context, topic_recv + topic );
        std::thread sendThread( send, &context, topic_snd + topic );
        recvThread.join();
        sendThread.join();
    }
    catch( zmq::error_t & ex )
    {
        std::cerr << "Caught an exception : " << ex.what();
    }

    return 0;
}

//subscriber.connect( "tcp://localhost:5556" );         //If server is hosted locally

//zmq::socket_t publisher( context, ZMQ_PUB );
//publisher.connect( "tcp://localhost:24042" );
