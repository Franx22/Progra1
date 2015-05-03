#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>


/************************************************************************** MAIN *****************************************************************************************************/

int main(int argc , char *argv[])
{
	int numThreads;
	printf("Inserte el numero de threads: ");
	scanf("%d",&numThreads);
	int num;
    int sock;
    struct sockaddr_in server;
    char message[1000] , server_reply[2000];

/***************************** Crea el socket ***********************************************************************************************************/

    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1 || num == numThreads)
    {
    	num =  num +1 ;
        printf("No se puede crear el socket");
    }
    puts("Socket creado");

/************************* DirCliente, Puerto ************************************************************************************************************/

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 8888 );

/*************************** Conecta con el servidor *******************************************************************************************************/

    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("coneccion fallada. Error");
        return 1;
    }

    puts("Conectado\n");

/********************** Comunicacion con el servidor *****************************************************************************************************/

    while(1)
    {
        printf("Escribe el mensaje : ");
        scanf("%s" , message);

/********************** Envia un mensaje al servidor *****************************************************************************************************/

        if( send(sock , message , strlen(message) , 0) < 0)
        {
            puts("Envio fallado");
            return 1;
        }

/********************** Recibe el reply desde el servidor *****************************************************************************************************/

        if( recv(sock , server_reply , 2000 , 0) < 0)
        {
            puts("no se recibio");
            break;
        }
        puts("Server reply: ");
        puts(server_reply);
    }

    close(sock);
    return 0;
}
