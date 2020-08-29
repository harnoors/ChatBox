#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include"list.h"

//send
struct LIST* Squeue;
struct LIST* Rqueue;
struct sockaddr_in myport,hisport;

char myText[1024],hisText[1024];
int sockfd,port1,port2;
//Mutex
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int release_screen;

void* KeyThread()
 {  
 	
 	while(1)
 	{	//memset(myText,0,strlen(myText));
 		fgets(myText,1024,stdin);
		if(myText[0]=='!')
    	{
    		printf("\nxxx  THANKS FOR USING CHATBOX  xxx\n");
    		exit(0);
    	}

    	else if(myText!=NULL)
    	{	
   			pthread_mutex_lock(&mutex);
			if(ListAppend(Squeue,&myText)==-1)
			{	
				printf("failed to insert in the send list \n" );
				exit(0);
			}
			
			char* demo;
 	 		demo=(char *)ListFirst(Squeue);
 			pthread_mutex_unlock(&mutex);
    	}
    }
 }

void* S2()
 {     
 	while(1)
 	{
 	
 		pthread_mutex_lock(&mutex);
 	 	char* demo;
 	 	demo=(char *)ListFirst(Squeue);
        pthread_mutex_unlock(&mutex);
 	 	if(demo!=NULL)
 	    { 	
            sendto(sockfd, demo, 1024, 0, (struct sockaddr*)&hisport, sizeof(hisport));
 			pthread_mutex_lock(&mutex);
 			ListRemove(Squeue);
 			pthread_mutex_unlock(&mutex);
 		}
 	}
	
 }

void* ScreenThread()
 {  
 	while(1)
 	{	
 		while(release_screen!=1);
 		pthread_mutex_lock(&mutex);
 	 	char* demo;
 	 	demo=(char *)ListFirst(Rqueue);
 		ListRemove(Rqueue);
 		pthread_mutex_unlock(&mutex);
		printf(">>%s",demo);
		release_screen=0;
 	}
 }
 
void* R2()
 { 
 	while(1)
    {	
    	memset(hisText,0,strlen(hisText));
    	recvfrom(sockfd,(char*) hisText, 1024, 0, (struct sockaddr*)&hisport, sizeof(hisport));
    	if(hisText!=NULL)
    	{
    		pthread_mutex_lock(&mutex);
    		if(ListAppend(Rqueue,hisText)==-1)
    		{	
				printf("failed to insert in the recived list \n" );
				exit(0);
			}
    		pthread_mutex_unlock(&mutex);
    		release_screen=1;
    		while(release_screen!=0);
		}
	}
}



int main(int argc, char **argv)

{
    
    if(argc != 4)
    {
    printf("Usage: %s <port>\n", argv[0]);
    exit(0);
    }

    
    port1 = atoi(argv[1]); //myport
    port2= atoi(argv[3]);  //destination port
    
    struct hostent *he;
    //used from (http://beej.us/guide/bgnet/html/#getaddrinfoprepare-to-launch)
    if ((he=gethostbyname(argv[2])) == NULL) { 
        perror("gethostbyname");
        exit(1);
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    myport.sin_family = AF_INET;
    myport.sin_port = htons(port1);
    //change ip her for diffrent systems 
    //enter ip of this system here instead of 127.0.0.1
    myport.sin_addr.s_addr=inet_addr("127.0.0.1");
    
    bind(sockfd, (struct sockaddr*)&myport, sizeof(myport));

 
    //change ip her for diffrent systems 
    //enter ip of this system here
    hisport.sin_family = AF_INET;
    hisport.sin_port = htons(port2);
    //change ip her for diffrent systems 
    //enter ip of destination system here instead of 127.0.0.1
    hisport.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    Squeue=ListCreate();
    Rqueue=ListCreate();
	
	release_screen=0;

	printf("\n WELCOME TO THE CHATBOX \n");

    pthread_t send,recv,keybrd,screen;
  
   	if(pthread_create(&send, NULL, (void *) S2, NULL) != 0)
   	{
		printf("\n ERROR! send pthread\n");
		return 0;
	}

	if(pthread_create(&recv, NULL, (void *) R2, NULL) != 0)
	{
		printf("\n ERROR! recive pthread\n");
		return 0;
	}

	if(pthread_create(&keybrd, NULL, (void *)KeyThread, NULL) != 0)
	{
		printf("\n ERROR! Keyboard pthread\n");
		return 0;
	}


	if(pthread_create(&screen, NULL, (void *)ScreenThread, NULL) != 0)
	{
		printf("\nERROR Screen pthread\n");
		return 0;
	}

    pthread_exit(NULL);


	return 0; 
}