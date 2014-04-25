#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>

#define BUF_SIZE 1024
#define SERVER_PORT 8008
#define SERVER_HOST "127.0.0.1"
#define EPOLL_RUN_TIMEOUT -1
#define EPOLL_SIZE 10000
#define MAX_NAME_LEN 256

int set_non_blocking(int sockfd)
{
	int flag;

	flag = fcntl(sockfd, F_GETFL, 0);
	if(flag != -1){
		flag = fcntl(sockfd, F_SETFL, flag | O_NONBLOCK);
		if(flag != -1) return 0;
		else{
			perror("set_non_blocking");
			exit(-1);
		}
	}

	perror("set_non_blocking");
	exit(-1);
}

int inserthead(char *A)
{
	char datebuf[128];
	time_t result;
	int i;
	int j;
	j = i = 0;
	result = time(NULL);
	sprintf(datebuf, "%s", asctime(localtime(&result)));
	while(datebuf[i] != '\0'){
		if(datebuf[i] == ' ') j++;
		if(j == 4){
			datebuf[i] = '\0';
			break;
		}
		i++;
	}

	sprintf(A, "HTTP/1.1 200 OK\r\nServer:GaoLiang\r\nDate:%s\r\nContent-Type: text/html\r\nContent-Length: 6\r\nConnect: keep-alive\r\nTransfer-Encoding: chunked\r\nContent-Encoding: gzip\r\n", datebuf);
	printf("%s\n", A);
	return strlen(A);
}

void handle_request(int client)
{
	int len;
	char buf[BUF_SIZE];
	char sendbuf[4*BUF_SIZE];
	char filename[MAX_NAME_LEN];
	int i;
	int j;
	int fd;
	int lenfile;
	int pos;
	unsigned long filelen;
	struct stat statbuff;

	memset(buf, '\0', BUF_SIZE);
	len = recv(client, buf, BUF_SIZE, 0);
	if(len > 0){
		for(i = 0; i < len; i++){
			if(buf[i] == 'G'){
				i += 4;
				j = 0;
				while(buf[i] != ' '){
					filename[j++] = buf[i++];
				}
				break;
			}
		}
		filename[j] = '\0';
		if(stat(filename, &statbuff) < 0){
			filelen = -1;
			perror("stat");
			close(client);
			return;
		}
		else filelen = statbuff.st_size;
		fd = open(filename, O_RDONLY);
		if(fd == -1){
			perror("open");
			send(client, "No such file\n", 13, 0);
			close(client);
			return;
		}

		memset(sendbuf, '\0', 4*BUF_SIZE);
//		pos = inserthead(sendbuf);
//		sendbuf[pos] = ' ';
//		pos ++;
//		pos = 0;
		while(filelen > 4*BUF_SIZE){
			filelen -= 4 * BUF_SIZE;
			lenfile = read(fd, sendbuf, 4*BUF_SIZE);
			len = send(client, sendbuf, lenfile, 0);
			while(len != lenfile){
				len += send(client, sendbuf + len, lenfile-len, 0);
			}
		}
		lenfile = read(fd, sendbuf, filelen);
		len = send(client, sendbuf, lenfile, 0);
		if(len != lenfile){
			perror("send");
			printf("len = %d\n lenfile = %d\n", len, lenfile);
		}
		close(fd);
		close(client);
	}
}

int main()
{	
	int listener;
	struct sockaddr_in addr, their_addr;
	socklen_t socklen;
	static struct epoll_event ev, events[EPOLL_SIZE];

	int epfd;

	int client, res, epoll_events_count;
	int clientlist[EPOLL_SIZE];
	int listpoint;
	int i, j;
	int len;
	char buf[BUF_SIZE];
	int on = 1;

	listpoint = 0;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(SERVER_PORT);
	addr.sin_addr.s_addr = inet_addr(SERVER_HOST);
	socklen = sizeof(struct sockaddr_in);

	ev.events = EPOLLIN | EPOLLET;

	listener = socket(AF_INET, SOCK_STREAM, 0);
	if(listener == -1){
		perror("socket");
		exit(-1);
	}

	set_non_blocking(listener);

	res = setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if(res == -1){
		perror("setsockopt");
		exit(-1);
	}
	res = bind(listener, (const struct sockaddr *)&addr, sizeof(addr));
	if(res == -1){
		perror("bind");
		exit(-1);
	}

	res = listen(listener, 1);
	if(res == -1){
		perror("listen");
		exit(-1);
	}

	epfd = epoll_create(EPOLL_SIZE);
	if(epfd == -1){
		perror("epoll_create");
		exit(-1);
	}
	ev.data.fd = listener;

	res = epoll_ctl(epfd, EPOLL_CTL_ADD, listener, &ev);
	if(res == -1){
		perror("epoll_ctl");
		exit(-1);
	}

	while(1){
		epoll_events_count = epoll_wait(epfd, events, EPOLL_SIZE, EPOLL_RUN_TIMEOUT);
		if(epoll_events_count == -1){
			perror("epoll_wait");
			exit(-1);
		}

		for(i = 0; i < epoll_events_count; i++){
			if(events[i].data.fd == listener){
				client = accept(listener, (struct sockaddr *) & their_addr, &socklen);
				if(client == -1){
					printf("error!\n");
					continue;
				}
				set_non_blocking(client);
				ev.data.fd = client;
				res = epoll_ctl(epfd, EPOLL_CTL_ADD, client, &ev);
				if(res == -1){
					perror("epoll_ctl");
					exit(-1);
				}

				clientlist[listpoint++] = client;
				/*				memset(buf, '\0', BUF_SIZE);
								len = recv(client, buf, BUF_SIZE, 0);
								printf("len = %d\n%d\n", len, __LINE__);
								if(len > 0){
								for(j = 0; j < len; j++){
								if(isprint(buf[j])) putchar(buf[j]);
								else printf(" %d ", (int)buf[j]);
								}
								}*/
			}
			else{
				handle_request(events[i].data.fd);
			}
		}
	}

	close(listener);
	close(epfd);

	return 0;
}
