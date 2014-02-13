/*#ifndef INTERACTIVEOPEN_H
#define INTERACTIVEOPEN_H


#include <string>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


class InteractiveOpen
{
public:
    int wpipes[2];
    int rpipes[2];
    InteractiveOpen(char *cmd);
    void giveInput(std::string input);
    std::string getOutput();
};

#endif // INTERACTIVEOPEN_H
*/
