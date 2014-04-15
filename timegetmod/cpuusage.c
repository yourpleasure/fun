#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#define __USE_GNU
#include <pthread.h>
#include <sched.h>

unsigned long long cmd = 0;
void *thread_fun(void *arg);

int main()
{
	int fd;
	int res;
	pthread_t *a_thread;
	int i;
	int cpunum;
	

	fd = open("/proc/timeget/my_cycles", O_WRONLY);
	res = write(fd, &cmd, sizeof(unsigned long long));
	printf("%Lu\n", cmd);

	cpunum = sysconf(_SC_NPROCESSORS_ONLN);
	a_thread = (pthread_t *)malloc(cpunum * sizeof(pthread_t));

	for(i = 0; i < cpunum; i++){
		res = pthread_create(a_thread + i, NULL, thread_fun, (void *)&i);
		if(res == -1) exit(1);
		sleep(1);
	}

	getchar();

	for(i = 0; i < cpunum; i++){
		res = pthread_cancel(a_thread[i]);
		if(res == -1) exit(2);
		sleep(1);
	}

	for(i = 0; i < cpunum; i++){
		res = pthread_join(a_thread[i],
