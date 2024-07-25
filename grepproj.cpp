#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>  
#include <filesystem>
class FileRead{
    public:
        std::ifstream file;
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
};

class GrepCommands{
    public:
        bool incaseSensitive(std::string inputCommand){
            return (inputCommand.find("i") != std::string::npos);
        }

        bool lineNumbers(std::string inputCommand){
            return (inputCommand.find("n") != std::string::npos);
        }

        void search(std::string inputFile, std::string pattern, bool caseInsensitive = false, bool lineNumbers = false){
            std::string line;
            std::regex r(pattern);
            FileRead fileToRead(inputFile);
            int count = 0;

            if(caseInsensitive)
            {
                 std::regex r(pattern, std::regex_constants::icase);
            }

            while(std::getline(fileToRead.file, line))
            {
                count++;
                if(std::regex_search(line, r))
                {
                    if(lineNumbers)
                    {
                        std::cout << "Found this in file(line Number " << count << ") :" << line << std::endl;
                    }
                    else
                    {
                        std::cout <<"Found this in file: " << line << std::endl;
                    }
                }
            }
        }

        void searchRecursively(std::string folder, std::string pattern, bool caseInsensitive = false, bool lineNumbers = false)
        {
            for (const auto & entry : std::filesystem::directory_iterator(folder))
            {
                if(entry.is_directory())
                {
                    std::string folder = entry.path().string();
                    searchRecursively(folder, pattern, caseInsensitive, lineNumbers);
                }
                else
                {
                    std::string file = entry.path().string();
                    search(file, pattern, caseInsensitive, lineNumbers);
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
        GrepCommands grep;
        if(commandVector[1].find("-l") != std::string::npos) {
            grep.search(commandVector[3], commandVector[2], grep.incaseSensitive(commandVector[1]), grep.lineNumbers(commandVector[1]));
        }
        if(commandVector[1].find("-r") != std::string::npos) {
            std::string folderToSearch = std::filesystem::current_path().string();
            grep.searchRecursively(folderToSearch, commandVector[2], grep.incaseSensitive(commandVector[1]), grep.lineNumbers(commandVector[1]));
        }
    }
    return 0;
}
