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
- So your send topic will consist of the servers topic + + name +'>' + opponent +'>'
- To get responses from the server your client will have to subscribe to  "HigherLower>msg!>" + name +'>'+ opponent +'>'
- For the server to recognize you as a player send the send topic + '1>'. (i.e. connect to server)
  + this will add you to a player pool
- If you want to end a session, you can do this correctly by sending send topic + '2>' (i.e. disconnect from server)
  + this will delete you from that player pool
- Once your opponent connects, the game starts and you can pick 'Higher' by sending send topic + '3>' or lower by sending send topic + '4>' 
