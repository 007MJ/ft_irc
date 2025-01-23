#ifndef UTILS_HPP
#define UTILS_HPP
#include <iostream>
#include "unistd.h"
#include "../src/Commands/Commands.hpp"
#include "../src/Client/Client.hpp"
#include "../src/Server/Server.hpp"

void errorMsg(const std::string &msg);
bool ValidateAndStoreArgs(char *argv[], int* port_, std::string& pass_);
bool clean_recv1(int fd_,  std::string &line_);
bool clean_recv(int fd_, char *buffer);
bool clean_send(int fd_, const char *buff);
bool promptForUsername(int fd_, char *buff);
std::string getFullUsername(std::vector<std::string> splittedName_);


// function pour faire les commandes
bool ClientHandler(std::string msg, Client *nc, Server *irc);
// void ActionClient(Client *nc, Server *irc);
void RoomCheck(Client *nc, Server *irc);
int getRoomindex(std::string nameRoom, Server *irc);
unsigned int topic(Client *nc, Server *irc);
unsigned int invite(Client *nc, Server *irc);
int getUser(std::string usrName, Server *irc);
// void sendMsg(std::string msg, Client *nc, int codeError);
void sendMsg(std::string& msg, Client* nc, int codeError);
// void sendToChannel(Channel room, std::string msg);
void sendToChannel(Channel& room, std::string msg, int author);
void sendToUser(Client* author, std::string& nameClient, Server* irc, std::string& msg);
void privmsg(Client *nc, Server *irc);
// int isUserChannel(std::map<int, Client *> clientsChannel, std::string nickname);
// void displayRoom(Client *nc, Channel room);
// bool fillForm(Client *nc);
// void sendForm(Client *nc);
void kick(Client *nc, Server *irc);
void mode(Client *nc, Server *irc);
// void mode(client *nc, Server *irc);

#endif