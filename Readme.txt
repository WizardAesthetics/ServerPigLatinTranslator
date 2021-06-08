Client.c
Compile: gcc -o UDPEchoClient Client.c DieWithError.c
Run: ./UDPEchoClient 127.0.0.1 29643

INPUT

//This is case sencsitive Enter in one of the three options "toEnglish"  "toPigLatin"  "Quit"
Translation type [toPigLatin/toEnglish] or Quit:  

//Input a word or a phrase do not add "  " surounding your word or phrase
Word or phrase to translate:					  

OUTPUT

//Specific to your tanslation request if you see "Request ID does not match, Please close and restart program" the request ID you recived did not match the one you sent
Request ID:					

//Confermation of the word or phrase that was sent
Sent: 

//Your translated wor or phrase 
Received:



Server.c 
Compile: gcc -o UDPEchoServer Server.c DieWithError.c
Run: ./UDPEchoServer 29643

OUTPUT

//The IP adress of the sender
Handling client 127.0.0.1

//Confriming request ID
Request ID:

//Confirming received word or phrase
Received:

//Prosseccing
Translating...

//What was sent back to the client
Sent:


/////////////////////////////////////////////////////////////////////////////////////////////
Example run

CLIENT


[bj4390@emunix.emich.edu:/usr/users/students/bj4390/Project 1]# gcc -o UDPEchoClient Client.c DieWithError.c
[bj4390@emunix.emich.edu:/usr/users/students/bj4390/Project 1]# ./UDPEchoClient 127.0.0.1 29643

Translation type [toPigLatin/toEnglish] or Quit: toPigLatin
Word or phrase to translate: Hello how has your day been
Request ID: 1
Sent: Hello how has your day been
Received: elloHay owhay ashay ourYay ayday eenbay

Translation type [toPigLatin/toEnglish] or Quit: toEnglish
Word or phrase to translate: elloHay owhay ashay ourYay ayday eenbay
Request ID: 2
Sent: elloHay owhay ashay ourYay ayday eenbay
Received: Hello how has Your day been
Translation type [toPigLatin/toEnglish] or Quit: Quit


SERVER

[bj4390@emunix.emich.edu:/usr/users/students/bj4390/Project 1]# gcc -o UDPEchoServer Server.c DieWithError.c
[bj4390@emunix.emich.edu:/usr/users/students/bj4390/Project 1]# ./UDPEchoServer 29643
Handling client 127.0.0.1
Request ID: 1
Received: Hello how has your day been
Translating...
Sent: elloHay owhay ashay ourYay ayday eenbay

---------------------------------------------------------------
---------------------------------------------------------------

Handling client 127.0.0.1
Request ID: 2
Received: elloHay owhay ashay ourYay ayday eenbay
Translating...
Sent: Hello how has Your day been

---------------------------------------------------------------
---------------------------------------------------------------





