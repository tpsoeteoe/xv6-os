#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  char buff[2];
  int p0[2];
  int p1[2];

  pipe(p0);
  pipe(p1);

  if (fork() == 0) {
    close(p0[1]);
    int n = read(p0[0], &buff[1], 1);
    close(p0[0]);

    if (n == 1) {
      printf("%d: received ping\n", getpid());
      close(p1[0]);
      write(p1[1], "a", 1);
      close(p1[1]);
      exit(0);
    } else{
      printf("%d: received ping error\n", getpid());
      exit(1);
    }
  } else {
    close(p0[0]);
    write(p0[1], "A", 1);
    close(p0[1]);

    close(p1[1]);
    int m = read(p1[0], &buff[2], 1);
    close(p1[0]);

    if (m == 1) {
      printf("%d: received pong\n", getpid());
      exit(0);
    } else {
      printf("%d: received pong error\n", getpid());
      exit(1);
    }
  }
}
