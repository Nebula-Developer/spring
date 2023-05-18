#include <iostream>
#include <unistd.h>
#include <fstream>
#include <termcap.h>
#include <termios.h>
#include <regex>
#include <sys/ioctl.h>
#include <vector>

#include "input.hpp"
#include "term.hpp"
#include "exec.hpp"

#define clearscr() printf("\033[H\033[J")

int main(int argc, char **argv) {
    std::string command;
    while (true) {
        command = std::string();
        get_input(command);

        if (command == "exit") {
            std::cout << "Exiting..." << std::endl;
            exit(0);
        }

        exec_command(command);
    }
    return 0;
}
