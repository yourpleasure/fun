#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#define __USE_GNU
#include <pthread.h>
#include <sched.h>
#define MAXLEN 255

typedef unsigned long long TIC_t;

typedef struct CT_t{
	TIC_t u, n, s, i, w, x, y, z;
}CT_t;

typedef struct CPU_t{
	CT_t pre;
	CT_t last;
}CPU_t;


unsigned long long cmd = 0;
int flag = 1;
void *thread_fun(void *arg);

int main()
{
	int fd;
	int res;
	pthread_t *a_thread;
	int i;
	int cpunum;
	void **thread_result;
	

	fd = open("/proc/timeget/my_cycles", O_WRONLY);
	res = write(fd, &cmd, sizeof(unsigned long long));
	printf("%Lu\n", cmd);

	cpunum = sysconf(_SC_NPROCESSORS_ONLN);
	a_thread = (pthread_t *)malloc(cpunum * sizeof(pthread_t));
	thread_result = (void **)malloc(cpunum * sizeof(void *));

	for(i = 0; i < cpunum; i++){
		res = pthread_create(a_thread + i, NULL, thread_fun, (void *)&i);
		if(res == -1) exit(1);
		sleep(1);
	}

	getchar();

	flag = 0;

	for(i = 0; i < cpunum; i++){
		res = pthread_join(a_thread[i], thread_result + i);
		if(res != 0) exit(2);
		sleep(1);
	}

	exit(0);
}

void *thread_fun(void *arg)
{
	int number = *(int *)arg;
	cpu_set_t mask;
	int i;
	int res;
	FILE *fp;
	char buf[MAXLEN+1];
	CPU_t owncpu;
	CT_t result;
	TIC_t *x, *y, *z;
	TIC_t total;
	double rio;

	CPU_ZERO(&mask);
	CPU_SET(number, &mask);
	res = sched_setaffinity(0, sizeof(mask), &mask);
	if(res != 0) exit(4);
	
	fp = fopen("/proc/stat", "r");
	do{
		rewind(fp);
		fflush(fp);

		for(i = 0; i <= number + 1; i++){
			fgets(buf, MAXLEN, fp);
		}

		memcpy(&owncpu.pre, &owncpu.last, sizeof(struct CT_t));
		sscanf(buf, "cpu%*d %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu", &owncpu.last.u, &owncpu.last.n, &owncpu.last.s, &owncpu.last.i, &owncpu.last.w, &owncpu.last.x, &owncpu.last.y, &owncpu.last.z);
		total = 0;
		x = (TIC_t *)&result, y = (TIC_t *)&owncpu.last, z = (TIC_t *)&owncpu.pre;
		for(i = 0; i < 8; i++){
			*x = *y++ - *z++;
			total += *x++;
		}

		rio = (double) result.i / total;
		if(rio > 0.5){
			for(i = 0; i < cmd / 80 * (rio - 0.5); i++)
				;
		}
		else{
			usleep(100000 * (0.5-rio));
		}
	}while(flag);

	fclose(fp);
	pthread_exit(NULL);
}
