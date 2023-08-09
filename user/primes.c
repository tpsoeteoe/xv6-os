#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
sieve(int fd)
{
	char prime;
	char buff;
	int pp[2];

	//print a prime or reach the end.
	int n = read(fd, &prime, 1);
	if (n==0)
		exit(0);
	else
		printf("prime %d\n", prime);

	//the recursion
	pipe(pp);
	if (fork() == 0) {
		close(pp[1]);
		sieve(pp[0]);
		close(pp[0]);
		exit(0);
	} else {
		close(pp[0]);
		n = read(fd, &buff, 1);
		while (n) { 
			//the sieve process
			if (buff % prime != 0) {
				write(pp[1], &buff, 1);
			}
			n = read(fd, &buff, 1);
		}
		close(pp[1]);
		wait((int *) 0);
	}

	exit(0);
}


int
main(int argc, char *argv[])
{
	int num = 34;
	char a[num];
	int p[2];

	//init the array
	for (int i = 0; i < num; i++) {
		a[i] = i + 2;
	}

	pipe(p);
	if (fork() == 0) {
		close(p[1]);
		sieve(p[0]);
		close(p[0]);
		exit(0);
	} else {
		close(p[0]);
		write(p[1], a, num);
		close(p[1]);
		//wait all children, grandchildren & c. exit
		wait((int *) 0);
	}

	exit(0);
}
