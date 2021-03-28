#include "twitter_listener.hpp"
#include <iostream>
#include "nlohmann/json.hpp"

using json = nlohmann::json;


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

void printMessage(std::string message, long statuscode)
{
    if(statuscode == 200)
    {
        std::cout << message << std::endl;
    }
    else
    {
        std::cout << "Failed connection with code " << statuscode <<  std::endl;


        // Parse error from message 

        auto message_json = json::parse(message);
        std::cout << message_json["title"].dump() <<  std::endl;
        std::cout << message_json["detail"].dump() <<  std::endl;
    }
    
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
            tlist.itsAMatch = [] (std::string msg, long ec) {printMessage(msg, ec);};
            tlist.run(std::cout);
        }
        else if(command.compare("print") == 0)
        {
            tlist.printRules(std::cout);
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