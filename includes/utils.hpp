#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include "unistd.h"
#include "../src/Commands/Commands.hpp"
#include "../src/Client/Client.hpp"
#include "../src/Server/Server.hpp"


void errorMsg(const std::string &msg);
bool ValidateAndStoreArgs(char *argv[], int* port_, std::string& pass_);
void ClientHandler(std::string msg, Client *nc);

#endif