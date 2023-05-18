#include <iostream>
#include <unistd.h>
#include <fstream>

#define clearscr() printf("\033[H\033[J")

char **split_str(std::string str, std::string delim) {
    char **split = new char*[str.length()];
    int i = 0;
    for (char *p = strtok((char*)str.c_str(), delim.c_str()); p != NULL; p = strtok(NULL, delim.c_str())) {
        split[i] = p;
        i++;
    }
    return split;
}

void process_output(std::string output) {
    std::cout << output << std::endl;
}

void exec_command(std::string command) {
    char **args = split_str(command, " ");

    pid_t pid = fork();

    if (pid == 0) {
        int status = execvp(args[0], args);
        if (status == -1) std::cout << "spring: command not found: " << args[0] << std::endl;
    } else {
        wait(NULL);
    }
}

int main(int argc, char **argv) {
    std::string command;
    while (true) {
        std::cout << "spring> ";
        std::getline(std::cin, command);
        if (command == "exit") break;
        exec_command(command);
    }
    return 0;
}