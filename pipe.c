	#include <sys/types.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/wait.h>
	#include <stdio.h>

	void handler() 
	{ 
		int child_pid[1];  //to write child pid
		int readbuffer[60];	//for read and write through pipe
		int fd[2];	//file descriptor for pipe
				//fd[0] is read end, fd[1] is write end
		int pid; 
		
		int write_sequence[50]; //to write fibonacci sequence
		int fib1 = 0, fib2 = 1, fib3, count=0, limit; //for fibonacci 

		printf("Enter the limit to generate the Fibonacci Series \n");
			scanf("%d", &limit);
		
  
		if (pipe(fd) < 0)  //creating pipe	
       		exit(1); 
  
  		if((pid = fork()) == -1)
		{
			perror("fork");
			exit(1);
		}

		
		if (pid == 0) //child process
		{ 
			printf("Hello from Child!\n");
			child_pid[0] = getpid();
			close(fd[0]); //close the read end of the pipe
			write(fd[1], &child_pid[0], sizeof(int)); //send the child_pid to parent via pipe
			
			
			
			write_sequence[0] = fib1;
   			write(fd[1], &write_sequence[0], sizeof(int));
    			write_sequence[1] = fib2;
			write(fd[1], &write_sequence[1], sizeof(int));
			count = 2;
   			 while (count < limit)
    			{
        			fib3 = fib1 + fib2;
				write_sequence[count] = fib3;
				write(fd[1], &write_sequence[count], sizeof(int));
        			count++;
        			fib1 = fib2;
        			fib2 = fib3;
    			}
       			
		} 
  
		else	//parent process
		{ 	
			wait(NULL);
			printf("Hello from parent! \n");
			close(fd[1]);	//close the write end of the pipe
			read(fd[0], &readbuffer[0], sizeof(int)); //read the child pid and store in readbuffer
			printf("my child's process id: %i\n", readbuffer[0]); 

			printf("\nFibonacci Series is ...\n");
			count = 0;
			while(count < limit)
			{ 
				read(fd[0], &readbuffer[count], sizeof(int)); //read the fibonacci sequence and store in readbuffer
				printf("\n %d", readbuffer[count]);
				count++;
			}
			
			
			
		} 
    
	} 

	int main()
	{
		//signal(SIGINT, handler);
		handler();

		return 0;
	}