#ifndef EXEC_HPP
#define EXEC_HPP

#include <iostream>
#include <vector>

std::vector<std::string> split_str(const std::string& str, const std::string& delim);
void process_output(std::string output);
void exec_command(std::string command);

#endif
