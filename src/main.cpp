#include <iostream>
#include <unistd.h>
#include <fstream>
#include <termcap.h>
#include <termios.h>

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

void get_term_pos(int &x, int &y) {
    // save current terminal settings
    struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;

    // disable canonical mode and echo
    new_tio.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    // send query to get cursor position
    std::cout << "\033[6n";
    std::cout.flush();

    // read response from terminal
    char buf[32];
    int i = 0;
    char c;
    while (read(STDIN_FILENO, &c, 1) > 0) {
        if (c == 'R') break;
        buf[i++] = c;
    }
    buf[i] = '\0';

    // parse response to get cursor position
    sscanf(buf, "\033[%d;%dR", &y, &x);

    // restore original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

void set_term_pos(int x, int y) {
    std::cout << "\033[" << y << ";" << x << "H";
}

void get_input(std::string &command) {
    int x = 0;

    int posX, posY;
    get_term_pos(posX, posY);

    // Make it so getchar() doesn't wait for a newline
    // (we can use #include <termios.h> for this)
    struct termios old_tio, new_tio;
    tcgetattr(STDIN_FILENO, &old_tio);
    new_tio = old_tio;
    new_tio.c_lflag &= (~ICANON & ~ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_tio);

    while (true) {
        char c = getchar();
        if (c == '\n') break;
        
        command = command.insert(x, 1, c);
        x++;

        set_term_pos(posX, posY);
        std::cout << command;
    }

    std::cout << std::endl;

    tcsetattr(STDIN_FILENO, TCSANOW, &old_tio);
}

int main(int argc, char **argv) {
    std::string command;
    while (true) {
        command = "";
        std::cout << "spring> ";
        get_input(command);
        if (command == "exit") break;
        exec_command(command);
    }
    return 0;
}