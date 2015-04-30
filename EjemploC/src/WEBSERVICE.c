/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
/*#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
//     if (argc < 2) {
//         fprintf(stderr,"ERROR, no port provided\n");
//         exit(1);
//     }
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(8080);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     clilen = sizeof(cli_addr);
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
          error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);
     n = write(newsockfd,"I got your message",18);
     if (n < 0) error("ERROR writing to socket");
     close(newsockfd);
     close(sockfd);
     return 0;
}
*/

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define PORT 7200

int openSocket();
void bindToPort(int,int);

int main(int argc, char **argv) {
	char *advice[]={"erterter1","sersdfsd2","sdfsdf3"};
	int listener=openSocket();
	if (listener==-1) {
		printf("ERROR en listener\n");
		return 0;
	}

	bindToPort(listener,PORT);
	if(listen(listener,10)==-1){
		printf("NO SE ESCUCHA\n");
		return 0;
	}

	printf("ESCUCHANDO\n");
	while(1){
		struct sockaddr_storage client;
		unsigned int addresSize=sizeof(client);
		printf("ESPERANDO CLIENTE\n");
		int connect=accept(listener,(struct sockaddr*)&client,&addresSize);
		if (connect==-1) {
			printf("NO SE PUEDE CONECTAR\n");
		}
		printf("ATENDIENDO\n");
		char *msg=advice[rand()%3];
		send(connect,msg,strlen(msg),0);
		msg=NULL;
		close(connect);
	}
	return 0;
}

int openSocket(){
	int s=socket(PF_INET,SOCK_STREAM,0);
	if (s==-1) {
		printf("ERROR EN SOCKET\n");
	}
	return s;
}

void bindToPort(int socket,int port){
	struct sockaddr_in name;
	name.sin_family=PF_INET;
	name.sin_port=(in_port_t)htons(port);
	name.sin_addr.s_addr=htonl(INADDR_ANY);

	int reuse=1;
	if (setsockopt(socket,SOL_SOCKET,SO_REUSEADDR,(char*)&reuse,sizeof(int))==-1) {
		printf("NO SE PUEDE REUSAR\n");
	}
	int c=bind(socket,(struct sockaddr*)&name,sizeof(name));
	if (c==-1) {
		perror("PUERTO EN USO\n");
	}
}
