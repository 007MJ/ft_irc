#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include "unistd.h"


void errorMsg(const std::string &msg);
bool ValidateAndStoreArgs(char *argv[], int* port_, std::string& pass_);

#endif