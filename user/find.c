#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void
find(char *path, char *file)
{
	char buff[512], *p;
	int fd;
	struct dirent de;
	struct stat st;

	if((fd = open(path, 0)) < 0){
		fprintf(2, "find: can't open %s\n", path);
		return;
	}
	if(fstat(fd, &st) < 0){
		fprintf(2, "find: can't stat %s\n", path);
		close(fd);
		return;
	}
	if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buff){
		fprintf(2, "find: path too long\n");
		close(fd);
		return;
	}

	strcpy(buff, path);
	p = buff + strlen(buff);
	*p++ = '/';
	while(read(fd, &de, sizeof(de)) == sizeof(de)){
		if(de.inum == 0) //empty dir entry
			continue;
		if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
			continue;

		memmove(p, de.name, DIRSIZ);
		p[DIRSIZ] = 0;
		if(stat(buff, &st) < 0){
			printf("find: can't stat %s\n", buff);
			continue;
		}
		if(st.type == T_DIR){
			find(buff,file);
		} else if(st.type == T_FILE && strcmp(de.name, file) == 0){
			printf("%s\n", buff);
			continue;
		}
	}
	close(fd);
}

int
main(int argc, char *argv[])
{
  if(argc != 3){
	printf("usage: find [path] [file]\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
