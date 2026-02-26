#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
/*
  Author:  James Yoho & Daniel Whaley
  Course:  COMP 340, Operating Systems
  Date:    25 Feburary 2026
  Description:   This file implements the
                 Shell program
  Compile with:  gcc -o shell shell.c
  Run with:      ./shell

*/

// This function completes the functionality of cd
int shell_change_dir(char *dir_path) {
	if(chdir(dir_path) !=0){
		printf("-bash: cd: %s: No such file or directory\n", dir_path);
		return -1;
	}
	return 0;
  // use the chdir() system call to change the current directory
}

// This function takes a file path and retuns -1 if the file does not exist and 0 if it does
int shell_file_exists(char *file_path) {
	struct stat info;
	if(stat(file_path, &info) == -1){
		return -1;
	}
	return 0;
  // use the stat() system call to check if a file exists
}



// This file takes the filename, filepath, and the size of the filepath
// It checks if the file exists. If it does it sets the filepath to the path of the file and returns 0, otherwise it returns -1
int shell_find_file(char *file_name, char *file_path, char file_path_size) {
	char* path_name = getenv("PATH");
	char* path_copy = strdup(path_name);

	char* orginal_copy = path_copy;

    char* dir;
	while(true){
        dir = strsep(&path_copy, ":");
        if(dir == NULL){
            free(orginal_copy);
            return -1;
        }

        snprintf(file_path, file_path_size, "%s/%s", dir, file_name);
        if(shell_file_exists(file_path)==0){
            free(orginal_copy);
            return 0;
        }
	}

  // traverse the PATH environment variable to find the absolute path of a file/command
}

// This function takes a file_path and an array of arguments and it executes the command/file given with the arguments provided.
int shell_execute(char *file_path, char **argv) {

	pid_t pid;
	pid = fork();
	if(pid == 0){
		execv(file_path, argv);
		exit(EXIT_FAILURE);
	}
	else{
		wait(NULL);
	}
	return 0;

  // execute the file with the command line arguments
  // use the fork() and exec() system call 
}
// This function acts as the shell, it runs commands like a shell until exit is entered

int main (int argc, char *argv[]) {
   //run the shell
   bool exit = false;
   
   while (!exit){
	//1. display prompt and wait for user input
	// generate some prompt 
	// e.g. username@hostname:~/david$ ping 
	char command[127];
	char cwd[127];
	char file_path[127];
		
	printf("username@hostname:%s$ ", getcwd(cwd, sizeof(cwd)));
	fgets(command, sizeof(command), stdin);
	fflush(stdout);
	//2. filter out whitespace command 
	command[strcspn(command, "\n")] = 0;
	

	//Take the command and split it into an array of strings
	char* words[256];
	int numWords = 0;
	char* word = strtok(command, " ");
	while(word != NULL){
		words[numWords] = word;
		numWords++;
		word = strtok(NULL, " ");
	}

	words[numWords] = NULL;

	//3. if command line contains non-whitespace characters
	if(numWords > 0){
		//Checks for exit
		if(strcmp(words[0], "exit") == 0){
			exit = true;
		} 

		// Runs our cd command if cd was passed. 
		else if (strcmp(words[0], "cd") == 0){
			if (numWords > 1){
				shell_change_dir(words[1]);
			}
			else{
				shell_change_dir(getenv("HOME"));
			}
		}
		// Check if the command is a direct path (contains a '/') and run the file/command if possible
		else if (strchr(words[0], '/')) {
			// absolute or relative path
			if (shell_file_exists(words[0]) == 0){
				shell_execute(words[0], words);
			}
			else{
				printf("Command not found\n");
			}
		}

		// Search for the command in the directories listed in the PATH environment variable
		else if (shell_find_file(words[0], file_path, (char)sizeof(file_path)) == 0) {
			// command found in PATH
			shell_execute(file_path, words);
		}

		else {
			printf("Invalid Command\n");
		}

	}
	// //4. parse command line
	// 	if the specified command is “exit”, terminate the program taking care to release any allocated resources.
	// 	if the specified command is “cd”
	// 		change the current working directory to the specified directory path using shell_change_dir()
	// 	if the command is specified using an absolute path (e.g. /usr/bin/ls), exists in the user’s PATH (e.g. ls) or exists in the current folder (e.g. ./hello_world)
	// 		execute the command using shell_execute()
	// 	else
	// 		report an error message
    }
   
}
