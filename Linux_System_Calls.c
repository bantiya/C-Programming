#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

/* Method to handle signal from the child process.
Whenever there will be a signal from the child process, this
method will be called to handle it. It will print the signal type.
*/
void childSignalHandler(int signum) {
   printf("\nCaught child signal: %d.\n", signum);
}

/*
Parent Process signal handler. It will finally exit the code.
*/
void parentSignalHandler(int signum){
    printf("\nCaught parent signal: %d. Exiting!\n", signum);
    exit(1);
}

/* User defined system function*/
int cs_system(char **argv, int argc){
    pid_t forkedProcess, waitSignal;
    int status;
    void (*istat)(int);

    //calling fork to create a copy(Child) of the current process(Parent).
    forkedProcess = fork();

    //If fork cannot create a process i.e. some error occurred,
    //it will return -1.
    if(forkedProcess == -1){
        perror("\nError! Cannot create the child process.");
        exit(EXIT_FAILURE);
    }

    //Fork returns a 0 when child process is successfully created.
    if(forkedProcess == 0){
        printf("\nThe child process, PID: %ld.\n",(long)getpid());

        //Sets the handler for the user interrupt.
        //When user will interrupt the code, childSignalHandler
        //will be called.
        signal(SIGINT, childSignalHandler);
        if(argc == 1){
            printf("\n1 command line argument, program will be paused!\n");
            //system will wait till a signal arrives.
            pause();
        }
        exit(atoi(argv[1]));
    }
    // If fork does not zero, then we are in the parent process.
    else{
		istat = signal(SIGINT, SIG_IGN);
		//
		do {
		    printf("\nParent Process, PID: %ld.\n",(long)getpid());
		    //will wait for child to terminate. If successfull, will return childs PID.
			waitSignal = wait(&status);
			printf("\nValue of waitSignal: %d and status: %d\n",waitSignal,status);
			printf("\nParent Process: after waiting on child process, PID: %d\n", getpid());
			//wait returns -1 if some error occurs.
			if (waitSignal == -1) {
				perror("\nError occurred!");
				exit(EXIT_FAILURE);
			}
			if (WIFEXITED(status)) {
				printf("\nexited, status=%d\n", WEXITSTATUS(status));
                //getting the original file descriptor.
				int desc = open("duplicateFile.txt", O_WRONLY | O_APPEND);
				if(desc < 0){
				    printf("Error opening the file\n");
				}
				//dup will create a copy of the file descriptor. Then we can use the copy of the descriptor
				//to make changes to original file.
				int copy_desc = dup(desc);
				write(copy_desc,"Adding into the file through duplicate descriptor\n",50);
				write(desc,"Adding into the file through original descriptor\n",51);
			} else if (WIFSIGNALED(status)) {
				printf("\nkilled by signal %d\n", WTERMSIG(status));
			} else if (WIFSTOPPED(status)) {
				printf("\nstopped by signal %d\n", WSTOPSIG(status));
			} else if (WIFCONTINUED(status)) {
				printf("\ncontinued\n");
				//usage of execlp
				execlp("echo","echo","CS","531",NULL);
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));

		signal(SIGINT, parentSignalHandler);

		while (1);

		exit(EXIT_SUCCESS);
    }
}

/* The main method. Entry point of the code.
It will call the system function. The system function accepts
the command line arguments.
*/
int main(int argc, char *argv[]){
    printf("Main started. Calling user defined system function cs531_system....!\n");
    cs_system(argv, argc);
    printf("\nExiting....!");
    return 0;
}
