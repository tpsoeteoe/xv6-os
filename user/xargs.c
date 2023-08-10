#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

int 
main(int argc, char *argv[])
{
	if(argc < 2){
		fprintf(2, "usage: xargs [command] [args ...]\n");
		exit(1);
	}

	char *cmd = argv[1];
	char *args[MAXARG];
	int arg_count = 0;

	//parse the command-line arguments into args array
	for(int i = 1; i < argc; i++){
		if(arg_count >= MAXARG){
			fprintf(2, "xargs: too many arguments\n");
			exit(1);
		}
		args[arg_count++] = argv[i];
	}


	#define MAX_ARG_LEN 128
	char line[MAX_ARG_LEN];
	char buff;
	int i = 0;

	//read lines from standard input & execute the command with args
	while(read(0, &buff, 1)){
		//get a line
		if(buff != '\n'){
			line[i++] = buff;
		} else{
			line[i] = '\0';
			//fork a child process to execute the command
			int pid = fork();
			if(pid < 0){
				fprintf(2, "xargs: fork failed\n");
				exit(1);
			}
			if(pid == 0){//child process
				args[arg_count] = line;
				args[arg_count+1] = 0;
				exec(cmd, args);
				fprintf(2, "xargs: exec failed\n");
				exit(1);
			} else{//parent process
				wait((int *)0);
			}

			i = 0; //next line start
		}
	}

	exit(0);
}
