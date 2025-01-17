#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include "unistd.h"
#include "../src/Commands/Commands.hpp"
#include "../src/Client/Client.hpp"
#include "../src/Server/Server.hpp"


void errorMsg(const std::string &msg);
bool ValidateAndStoreArgs(char *argv[], int* port_, std::string& pass_);

// function pour faire les commandes
void ClientHandler(std::string msg, Client *nc, Server *irc);
// void ActionClient(Client *nc, Server *irc);
void RoomCheck(Client *nc, Server *irc);
int getRoom(std::string nameRoom, Server *irc);
unsigned int topic(Client *nc, Server *irc);
unsigned int invite(Client *nc, Server *irc);
int getUser(std::string usrName, Server *irc);
void sendMsg(Client *author, std::string msg, Client *nc, int codeError);
void sendToChannel(Client *nc, Channel room, std::string msg);
void sendToUser(Client *author, std::string nameClient, Server *irc, std::string msg);
unsigned int privmsg(Client *nc, Server *irc);
unsigned int isUserChannel(std::map<int, Client *> clientsChannel, std::string nickname);
void displayRoom(Channel room);
bool fillForm(Client *nc);
void sendForm(Client *nc);

#endif