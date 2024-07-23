#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

class FileRead{
    private:
        std::ifstream file;

    public:
        FileRead(std::string inputFile)
        {
            std::ifstream inputFileStream(inputFile);
            try
            {
                if(!inputFileStream.is_open())
                {
                    throw std::runtime_error("Error: could not open file" + inputFile);
                }
                else
                {
                    file = std::move(inputFileStream);
                }
            }
            catch(...) 
            {
                std::cout << "Error: could not open file" << std::endl;
            }
        }

        ~FileRead()
        {
            file.close();
        }

        void search(std::string pattern){
            std::string line;
            std::regex r(pattern);

            while(std::getline(file, line))
            {
                if(std::regex_search(line, r))
                {
                    std::cout <<"Found this in file: " << line << std::endl;
                }
            }
        }
};

int main () {
    
    std::string userInput;
    std::vector<std::vector<std::string>> commands; 
    std::string command;
    std::vector<std::string> command1;
    
    std::cout << "Enter the command: ";
    std::getline(std::cin, userInput);
    std::stringstream userCommand(userInput);

    while( std::getline(userCommand, command, ' ') ) 
    {
        if(command == "|") {
            commands.push_back(command1);
            command1.clear();
        }
        command1.push_back(command);
    } 

    commands.push_back(command1);

    for(auto commandVector : commands) {
        if(commandVector[1] == "-l") {
            FileRead file(commandVector[3]);
            file.search(commandVector[2]);
        }
    }
    return 0;
}