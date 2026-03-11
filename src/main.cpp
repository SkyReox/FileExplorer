/*
** EPITECH PROJECT, 2026
** FileExplorer
** File description:
** main.cpp
*/

#include "FileExplorer.hpp"
#include <fstream>
#include <iostream>

bool isDisplayAvailable()
{
    const char* display = std::getenv("DISPLAY");
    if (display == nullptr || std::string(display).empty())
        return false;
    return true;
}

int main(void)
{
    if (!isDisplayAvailable()) {
        std::cerr << "'DISPLAY' isn't set in the environment" << std::endl;
        return FAILURE;
    }

    fe::FileExplorer fileExplorer("Test");
    std::ifstream file;

    try
    {
        file = fileExplorer.open();
    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return FAILURE;
    }

    if (!file.is_open())
        std::cout << "No file opened" << std::endl;
    else {
        std::cout << file.rdbuf();
        file.close();
    }

    return SUCCESS;
}
