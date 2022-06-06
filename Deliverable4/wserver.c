#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include "queue.h"
#include <pthread.h>
char default_root[] = ".";
//
// ./wserver [-p <portnum>] [-t threads] [-b buffers]
//
// e.g.
// ./wserver -p 2022 -t 5 -b 10
//

pthread_mutex_t mutex;
pthread_cond_t notFull;
pthread_cond_t notEmpty;
int buffer_size;
struct Queue *queue;
void threadRequest(int FD)
{

	request_handle(FD);
	close_or_die(FD);
	printf("%ld completed task %d \n", pthread_self(), FD);
}

void *threadTask()
{
	puts("Running thread");
	while (1)
	{
		pthread_mutex_lock(&mutex);

		if (isEmpty(queue))
		{
			pthread_cond_wait(&notEmpty, &mutex);
		}
		int currentFD;

		if (!isEmpty(queue))
		{
			currentFD = dequeue(queue);
		}
		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&notFull);

		printf("Thread handling %d \n", currentFD);

		threadRequest(currentFD);
	}
}

int main(int argc, char *argv[])
{
	int c;
	char *root_dir = default_root;
	int port = 10000;
	int threads = 2;
	buffer_size = 5;

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

	printf("Server running on port: %d, threads: %d, buffer: %d\n", port, threads, buffer_size);

	// run out of this directory
	chdir_or_die(root_dir);
	queue = createQueue(buffer_size);
	pthread_t workerThreads[threads];
	for (int i = 0; i < threads; i++)
	{
		pthread_create(&workerThreads[i], NULL, threadTask, NULL);
	}

	// now, get to work
	int listen_fd = open_listen_fd_or_die(port);
	while (1)
	{
		struct sockaddr_in client_addr;
		int client_len = sizeof(client_addr);
		int conn_fd = accept_or_die(listen_fd, (sockaddr_t *)&client_addr, (socklen_t *)&client_len);

		// Keep obtained Descriptor
		int *obtainedFD = &conn_fd;

		// Again, since the Queue is a shared source between the executing threads,
		// we need to protect this section with mutex.
		pthread_mutex_lock(&mutex);
		if (isFull(queue))
		{
			pthread_cond_wait(&notFull, &mutex);
		}
		enqueue(queue, *obtainedFD);
		pthread_cond_signal(&notEmpty); // Inform waiting threads that new request added to the queue
		pthread_mutex_unlock(&mutex);

	}
	return 0;
}
