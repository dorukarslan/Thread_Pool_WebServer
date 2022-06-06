#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include <pthread.h>
#include <string.h>

char default_root[] = ".";

//
// ./wserver [-p <portnum>] [-t threads] [-b buffers]
//
// e.g.
// ./wserver -p 2022 -t 5 -b 10
//
int main(int argc, char *argv[])
{
	int c;
	char *root_dir = default_root;
	int port = 10000;
	int threads = 2;
	int buffer_size = 5;

	while ((c = getopt(argc, argv, "d:p:t:b:")) != -1)
		switch (c)
		{
		case 'd':
			root_dir = optarg;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case 't':
			threads = atoi(optarg);
			break;
		case 'b':
			buffer_size = atoi(optarg);
			break;
		default:
			fprintf(stderr, "usage: ./wserver [-p <portnum>] [-t threads] [-b buffers] \n");
			exit(1);
		}

	///

	printf("Server running on port: %d, threads: %d, buffer: %d\n", port, threads, buffer_size);

	// run out of this directory
	chdir_or_die(root_dir);

	// now, get to work
	int listen_fd = open_listen_fd_or_die(port);

	pid_t pid;
	
	while (1)
	{
			struct sockaddr_in client_addr;
			pid = fork();		
			int client_len = sizeof(client_addr);
			int conn_fd = accept_or_die(listen_fd, (sockaddr_t *)&client_addr, (socklen_t *)&client_len);
			if (pid == 0) {
    			close(listen_fd);
    			request_handle(conn_fd);
    			close(conn_fd);
    			exit(0);
  			} else {
  				//int child_status;
    			close(conn_fd);
  				} 

			//request_handle(conn_fd); // The request is handled by this function
	}
	close(listen_fd);
	return 0;
}


