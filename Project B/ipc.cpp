//Demonstrates interprocess communication using pipes
//Parent process creates a pipe and forks a child process

#include <iostream>
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>

//Producer process
void producer(int pipe_fd[2]){
    //Close read end of the pipe
    close(pipe_fd[0]);
    std::cout << "Producer: Writing message to the pipe\n";
    //Message pointer
    const char* message = "It's-a me, Mario!";
    //Write message to the pipe
    write(pipe_fd[1], message, strlen(message) + 1);
    //Close write end of the pipe
    close(pipe_fd[1]);
}

//Consumer process
void consumer(int pipe_fd[2]){
    //Close write end of the pipe
    close(pipe_fd[1]);
    std::cout << "Consumer: Reading message from the pipe\n";
    //Buffer to store the message
    char buffer[256];
    //Read message from the pipe
    read(pipe_fd[0], buffer, sizeof(buffer));
    //Print the message
    std::cout << "Consumer has recieved the message!: " << buffer << std::endl;
    //Close read end of the pipe
    close(pipe_fd[0]);
}

//Main function to show interprocess communication using pipes
int main(){
    int pipe_fd[2];

    //Create a pipe
    if(pipe(pipe_fd) == -1){ //Test for pipe creation failure
        std::cerr << "Pipe creation failed!\n";
        return 1;
    }

    pid_t pid = fork(); //Fork a child process

    if(pid == -1){ //Test for fork failure
        std::cerr << "Fork failed!\n";
        return 1;
    }

    if(pid == 0){ //Child process
        consumer(pipe_fd);
    }else{ //Parent process
        producer(pipe_fd);
        wait(NULL); //Wait for the child process to finish
    }

    return 0;
}