//Program to demonstrate two children modifying the same variable through shared memory 
//and child process synchronized by named semaphores
//to compile - gcc fork.c -lrt -pthread

#include <unistd.h> //for fork(), exit(), pid_t
#include <stdlib.h> //for EXIT_SUCCESS, EXIT_FAILURE 
#include <stdio.h> //for printf()
#include <sys/wait.h> //for waitpid() 
#include <sys/mman.h> //for shared memory 
#include <fcntl.h> //for O_* constants
#include <string.h> //for memcpy
#include <semaphore.h>

#define NAME "/shared_memory"   //shared object of name shared_memory will be stored in /dev/shm/ 
#define SIZE (5 * sizeof(char)) //for "HELLO"/"CELLO"

#define SEM_NAME "/sema"  //named semaphore with name sema will be stored in /dev/shm/
#define SEM_INIT_VAL 1    //number of resources

void child_process(char * msg, int child_no){
    //open the semaphore and wait if the other process is in its critical section, otherwise decrement
    sem_t * semaphore = sem_open(SEM_NAME, O_RDWR);
    if (semaphore == SEM_FAILED) {
        printf("sem_open() error\n");
        exit(EXIT_FAILURE);
    }
            
    if( sem_wait(semaphore) == -1){
        printf("sem_wait error\n");
        exit(EXIT_FAILURE);
    }

    printf("Entered child %d critical section\n", child_no);
    //open the existing shared memory object
    int fd = shm_open(NAME, O_RDWR, 0);
    if (fd == -1){
        printf("shem_open failed !\n");
        exit(EXIT_FAILURE);
    }

    //map the shared memory object into this process's virtual address space
    char *addr =  mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED){
        printf("mmap failed !\n");
        exit(EXIT_FAILURE);
    }

    printf("Child %d reading from shared memory - \"%s\" \n", child_no, addr);
    printf("Child %d modifying \"%s\" to \"%s\" \n", child_no, addr, msg);
    memcpy(addr, msg, SIZE);
    printf("Copying done from child process %d to shared memory, exiting critical section \n", child_no);
    munmap(addr, SIZE);
    close(fd);

    //increment the semaphore and let the other process enter its critical section
    if (sem_post(semaphore) == -1){
        printf("sem_post error\n");
        exit(EXIT_FAILURE);
    }

    if (sem_close(semaphore) == -1) {
        printf("sem_close() failed\n");
    }
            
}

int main(){

    int status; //for storing the exit status of the children
    pid_t pid1, pid2; //process id of children

    //parent process creating named semaphore to synchronize the child processes 
    sem_t *semaphore = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0777, SEM_INIT_VAL);
    if (semaphore == SEM_FAILED) {
        printf("sem_open() error\n");
        exit(EXIT_FAILURE);
    }

    //we close the semaphore as we won't use it in the parent process 
    if (sem_close(semaphore) == -1) {
        printf("sem_close() failed\n");
        }

    
    //parent process creating a shared memory and initializing it with HELLO
    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR, 0777);
    if (fd == -1){
        printf("shem_open failed !\n");
        return EXIT_FAILURE;
    }

    //adjusting size of our shared memory object for our strings
    //if fd not in write mode then ftruncate fails
    if (ftruncate(fd, SIZE) == -1){
        printf("ftruncate failed !\n");
        return EXIT_FAILURE;
    }

    //map the shared memory object into this process's virtual address space
    void *addr =  mmap(NULL, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED){
        printf("mmap failed !\n");
        return EXIT_FAILURE;
    }

    //copy the string from the process to shared memory 
    memcpy(addr, "APPLE", SIZE);
    printf("Parent process writing \"APPLE\" to shared memory\n");
    //unmap the shared memory object from this process's virtual address space
    munmap(addr, SIZE);
    //close the file descriptor
    close(fd);
       
    //create 1st child
    switch(pid1 = fork()){

        //error creating 1st child
        case -1:
            printf("fork failed !\n");
            return EXIT_FAILURE;

        //child process 1
        case 0:
            //sleep(2); //uncomment to see different scheduling
            //write JELLO to shared memory
            child_process("LEMON", 1);
            break;

        //parent process
        default:
            //creating the 2nd child
            pid2 = fork();

            if(pid2 == -1){
                printf("fork failed !\n");
                return EXIT_FAILURE;
            }

            //child process 2
            else if(pid2 == 0){
                //write CELLO to shared memory
                child_process("MANGO", 2);    
                break;
                
            }

            //parent process
            else{
                //wait for all children to finish
                while(wait(&status)!= -1);
                
                //delete the shared memory object from /dev/shm/
                shm_unlink(NAME);

                //mark semaphore for deletion once all processes have stopped using it ,i.e, closed it      
                if (sem_unlink(SEM_NAME) == -1){
                    printf("sem_unlink() failed\n");
                }
            }

            break;

    }

    return EXIT_SUCCESS;
}