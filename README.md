# Chat-App
#Summary:-
(1) Implemented a chat application using Socket programming in C to communicate between two local machine terminals.
(2) Developed using a UDP-based (IPv4) connection to achieve one-to-one Communication protocol.
(3) Pthreads handles different processes simultaneously.
(4)Major focus on memory management, used two single lists to store all the messages in one’s mailbox.

#Requirements:  

       (1) Linux or Mac terminal
       (2) Port Number of Both the local and remote machine (Could be decided by users)
       (3) Machine Name of both local and remote.

##Running the Chat-App:

       (1) Run the make file in the terminal. 
              Commands -
              (a). $make
              (b). $./s-talk (myPortNumber) (remoteMachineName) (RemotePortNumber)
       (2) Start Chatting.
       (3) To terminate the chat just Enter '!'.
       (4) Termination message will be sent to Remote Person.

##Working:

       (1) Pthreads has been used to communicate.
       (2) List C data structure used. 
       (3) 4 threads designed.
           (a) First Thread used to store our message into list.
           (b) Second Thread used to take out our message and send to remote person.
           (c) Third Thread used to store message of remote person into another list.
           (d) Forth Thread used to take out remote person message from list and print on my screen.
       (4) Socket establishing and binding used.

##Data Structures:

       List Data Structure for storing Messages.
       (1) Two arrays one for List headers and one for Nodes.
       (2) All List shares same memory.
