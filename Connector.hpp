#ifndef CONNECTOR_HPP
#define CONNECTOR_HPP

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <string>

void ConnectToEngine(const char* path);

std::string getNextMove(std::string &position);

void CloseConnection();

#endif CONNECTOR_H