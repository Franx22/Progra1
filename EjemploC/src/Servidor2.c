
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h> //threading

//La funcion thread
void *connection_handler(void *);

/************************************************************************** MAIN *****************************************************************************************************/

int main(int argc , char *argv[])
{
	int numThreads;
	printf("hola");
	printf("Inserte el numero de threads: ");
	scanf("%d",&numThreads);

    int socket_desc , client_sock , c , *new_sock;
    struct sockaddr_in server , client;

/***************************** Crea el socket ***********************************************************************************************************/

    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("No se puede crear el socket");
    }
    puts("Socket creado");

/******************** Prepara la estructura Sockaddr ***********************************************************************************************************/

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );

/***************************** BIND ***********************************************************************************************************/

    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //Imprime el mensaje de error
        perror("bind fallo. Error");
        return 1;
    }
    puts("bind creado");

/***************************** LISTEN ***********************************************************************************************************/

    listen(socket_desc , 3);

/***************************** Espera conecciones de clientes ***********************************************************************************************************/

    puts("Esperando por conecciones...");
    c = sizeof(struct sockaddr_in);

/***************************** Espera conecciones de clientes ***********************************************************************************************************/

    puts("Esperando por conecciones...");
    c = sizeof(struct sockaddr_in);


    int num = 0;//contador para el Pre-Thread

/****** Aqui se crean los clientes con la cantidad "num" de threads ***********************************************************************************************************/

    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) && num <= numThreads )
    {
    	num = num + 1;
        puts("Coneccion aceptada");
        pthread_t sniffer_thread;
        new_sock = malloc(1);
        *new_sock = client_sock;

        if( pthread_create( &sniffer_thread , NULL ,  connection_handler , (void*) new_sock) < 0)
        {
            perror("no se puede crear el thread");
            return 1;
        }

        //Si no comento esto no sirve para todos los clientes
        //pthread_join( sniffer_thread , NULL);

        puts("Handler asignado");
    }

    if(num  ==  5 ){
    	printf("se paso la cantidad maxima de clientes");
    	return 0;
    }

    if (client_sock < 0)
    {
        perror("fallo accept");
        return 1;
    }

    return 0;
}

/***************************** Funcion para devolver lo que el cliente escribe ***********************************************************************************************************/
void *connection_handler(void *socket_desc)
{
    //Obtiene el socket
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000];
    puts("\n");

    //Envia mensaje al cliente
    message = "                     Soy tu handler\n";
    write(sock , message , strlen(message));

    message = "Escribe algo y lo repetire \n";
    write(sock , message , strlen(message));

    //Reenvia el mensaje al cliente
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
    //envia el mensaje al cliente
        write(sock , client_message , strlen(client_message));
    }

    if(read_size == 0)
    {
        puts("Cliente desconectado");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("no se recibio nada");
    }

    free(socket_desc);

    return 0;
}
