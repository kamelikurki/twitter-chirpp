#include "twitter_listener.hpp"
#include <iostream>

void printUsage()
{
    std::cout << "/**************************************/" << std::endl;
    std::cout << "twitter_listener command line interface." << std::endl;
    std::cout << "/**************************************/" << std::endl;
    std::cout << "Possible arguments" << std::endl;
    std::cout << "---" << std::endl;
    std::cout << "clearall \"apiKey\"                       // removes all rules" << std::endl;
    std::cout << "run      \"apiKey\"                       // runs the listener" << std::endl;
    std::cout << "print    \"apiKey\"                       // prints all current rules" << std::endl;
    std::cout << "remove   \"apiKey\"                       // removes all rules" << std::endl;
    std::cout << "add      \"rule\"   \"tag\"   \"apiKey\"      // Adds a new rule and adds a tag to it" << std::endl;
    std::cout << "remove   \"tag\"    \"apiKey\"              // removes a rule with the given tag" << std::endl;

}


void printMessage(std::string message)
{
    std::cout << message << std::endl;
}

int main(int argc, const char* argv[] )
{
    if(argc == 3)
    {       
        std::string command(argv[1]);
        twitter_listener tlist(argv[2]);

        if (command.compare("clearall") == 0)
        {
            tlist.removeAllRules();
        }
        else if(command.compare("run") == 0)
        {
            tlist.itsAMatch = [] (std::string msg) {printMessage(msg);};
            tlist.run();
        }
        else if(command.compare("print") == 0)
        {
            tlist.printRules();
        }
        else
        {
            printUsage();
        }       
    }
    else if(argc == 4)
    {
        std::string command(argv[1]);
        twitter_listener tlist(argv[3]);
        std::string tag(argv[2]);

        if(command.compare("remove") == 0)
        {
            tlist.removeRule(tag);
        }
        else
        {
            printUsage();
        }
    }
    else if(argc == 5)
    {
        std::string command(argv[1]);
        twitter_listener tlist(argv[4]);
        std::string value(argv[2]);
        std::string tag(argv[3]);

        if(command.compare("add") == 0)
        {
            tlist.addRule(value,tag);
        }
        else
        {
            printUsage();
        }
    }
    else
    {
        printUsage();
    }
}