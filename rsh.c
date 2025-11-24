#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};


void getCMD(const char* cmd, char* command){
       	strcpy(command, cmd);
	strtok(command, " ");

}


int isAllowed(const char*cmd) {
	char command[50];
        getCMD(cmd, command);	
	for(int x = 0; x < N; x++){
		if(strcmp(command, allowed[x]) == 0) return 1;
	}
	return 0;
}

int main() {

    // TODO
    // Add variables as needed

    char line[256];

    while (1) {

	fprintf(stderr,"rsh>");

	if (fgets(line,256,stdin)==NULL) continue;

	if (strcmp(line,"\n")==0) continue;

	line[strlen(line)-1]='\0';
	
	if(isAllowed(line) == 0){
		printf("NOT ALLOWED!\n");
	}
	else{
		char* command;
		command = malloc(sizeof(char)*50);
	       	strcpy(command, line);
		strtok(command, " ");

		if(strcmp(command, "cd") == 0){
			int argCount = 0;
			char dir[50];
			while(command != NULL){
				if(argCount == 1){
					strcpy(dir, command);
				}
				argCount++;
				command = strtok(NULL, " ");	
			}
			if(argCount > 2){
				printf("-rsh: cd: too many arguments\n");
			}
			else{
				chdir(dir);
			}
		}
		else if(strcmp(command, "exit") == 0){
			return 0;
		}
		else if(strcmp(command,"help") == 0){
			printf("The allowed commands are:\n");
			printf("1: cp\n");
			printf("2: touch\n");
			printf("3: mkdir\n");
			printf("4: ls\n");
			printf("5: pwd\n");
			printf("6: cat\n");
			printf("7: grep\n");
			printf("8: chmod\n");
			printf("9: diff\n");
			printf("10: cd\n");
			printf("11: exit\n");
			printf("12: help\n");
		}
		else{
			pid_t pid;
			int argLength = strlen(command) + 1;
			char* args;
			args = malloc(sizeof(int)*50);
		        strncpy(args, line + argLength, 20);  
			char *argv[21];
		       	argv[0] = command;

			strtok(args, " ");
			int index = 1;
			while(args != NULL){
				if(strcmp(args, "") != 0){
					argv[index++] = args;
					args = strtok(NULL, " ");
				}
				else break;
			}

			argv[index] = NULL;

			int status;
			posix_spawnattr_t attr;

			posix_spawnattr_init(&attr);

			if(posix_spawnp(&pid, command, NULL, &attr, argv, environ) != 0) {
				perror("spawn failed");
				exit(EXIT_FAILURE);
			}

			if(waitpid(pid, &status, 0) == -1){
				perror("waitpid failed");
				exit(EXIT_FAILURE);
			}
			/*
			if(WIFEXITED(status)){
				printf("Spawned process exited with status %d\n", WEXITSTATUS(status));
			}
			*/
			posix_spawnattr_destroy(&attr);

		}
	}
    	}
    return 0;
}
