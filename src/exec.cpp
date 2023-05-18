#include <vector>
#include <iostream>
#include <unistd.h>

#include "exec.hpp"

std::vector<std::string> split_str(const std::string& str, const std::string& delim) {
    std::vector<std::string> tokens;
    size_t start = 0, end = 0;
    while ((end = str.find(delim, start)) != std::string::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + delim.length();
    }
    tokens.push_back(str.substr(start));
    return tokens;
}

void process_output(std::string output) {
    std::cout << output << std::endl;
}

void exec_command(std::string command) {
    std::vector<std::string> args = split_str(command, " ");

    pid_t pid = fork();

    // convert args to char**
    char **args_vec = new char*[args.size() + 1];
    for (int i = 0; i < args.size(); i++) {
        args_vec[i] = (char*)args[i].c_str();
    }
    args_vec[args.size()] = NULL;

    if (pid == 0) {
        int status = execvp(args_vec[0], args_vec);
        if (status == -1) std::cout << "spring: command not found: " << args_vec[0] << std::endl;
        // kill child process
        exit(0);
    } else {
        wait(NULL);
    }
}
