#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <libgen.h>

#define BUFFER_SIZE 1024
static int stop = 0;
static void handle_term( int sig )
{
	    stop = 1;
}

int main(int argc,char * argv[])

{
	if( argc <= 3 )
	{
		 printf( "usage: %s ip_address port_number backlog\n", basename( argv[0] ) );
		 return 1;
	}
	const char* ip = argv[1];
    int port = atoi( argv[2] );
    int backlog = atoi( argv[3]);
	
	int sock = socket( PF_INET, SOCK_STREAM, 0 );
	assert( sock >= 0 );

    struct sockaddr_in address;
    bzero( &address, sizeof( address ) );
	address.sin_family = AF_INET;
    inet_pton( AF_INET, ip, &address.sin_addr );
    address.sin_port = htons( port );

    int ret = bind( sock, ( struct sockaddr* )&address, sizeof( address ) );
	assert( ret != -1 );

    ret = listen( sock, backlog );
	struct sockaddr_in cilent;
	socklen_t cilent_addrlength = sizeof(cilent);
	int connfd = accept(sock,(struct sockaddr *)&cilent,&cilent_addrlength);
	
	if(connfd < 0)
	{
		printf("errno is : %d\n",errno);
	}else{
		char buffer[BUFFER_SIZE];
		memset(buffer,'\0',BUFFER_SIZE);
		while( recv(connfd,buffer,BUFFER_SIZE-1,0) >0)
			{
				printf("%s\n","one client has connneted");
			}
	}
	close( sock );
	return 0;
}
