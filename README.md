# Higher Lower Game
## Introduction:
This project contains a zmq server that provides a 'Higher Lower' game and 2 clients.
One client is written in c++ and prints its output to command prompt. The other one is written in python and contains a qt GUI.
### Steps to use server with the provided clients:
- Run the c++ or python client
- Insert your name and the name of your opponent
- The game will start once your opponent connects with his client
### Steps to use server without the provided clients:
- The zmq server is subsribed to the topic "HigherLower>msg?>"
- To get responses from the server your client will have to subscribe to  "HigherLower>msg?>"
- For the server to recognize you as a player send the topic + '1>'. (i.e. connect to server)
  + this will add you to a player pool
- If you want to end a session, you can do this correctly by sending topic + '2>' (i.e. disconnect from server)
  + this will delete you from that player pool
- Once the game started you can pick 'Higher' by sending '3>' or lower by sending '4>' 
