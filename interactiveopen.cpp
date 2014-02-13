#include "interactiveopen.h"
/*
InteractiveOpen::InteractiveOpen(char *cmd)
{
    pipe(wpipes);
    pipe(rpipes);
    int pid = fork();
    if (pid == -1) {
        exit(1);
    }
    if (pid == 0) {//child
        close(wpipes[1]);
        close(rpipes[0]);
        dup2(wpipes[0], STDIN);
        dup2(rpipes[1], STDOUT);
        close(wpipes[0]);
        close(rpipes[1]);
        execl(cmd);

    } else { //parent
        close(wpipes[0]);
        close(rpipes[1]);
    }
}

void InteractiveOpen::giveInput(std::string input)
{
    write(wpipes[1]);
}

std::string InteractiveOpen::getOutput()
{
    
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
        if(fgets(buffer, 128, pipe) != NULL)
            result += buffer;
    }
    pclose(pipe);
    return result;
    read(wpipes[1]);
}
*/
