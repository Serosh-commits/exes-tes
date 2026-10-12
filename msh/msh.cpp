#include<iostream>
#include<string>
#include<vector>
#include<unistd.h>
#include<sstream>
#include<cerrno>
#include<cstring>

static std::vector<std::string> tokenize(const std::string &line)
{
    std::vector<std::string> tokens;
    std::istringstream stream(line);
    std::string token;

    while(stream >> token){
        tokens.push_back(token);
    }

    return tokens;
}

static bool isbuiltin(const std::string& cmd)
{
    return cmd == "exit" || cmd == "cd";
}

static bool runbuiltin(const std::vector<std::string>& args, bool &running)
{
    const std::string& cmd = args[0];

    if(cmd == "exit")
    {
        running = false;
        return true;
    }

    if(cmd == "cd")
    {
        const char* target = (args.size() > 1) ? args[1].c_str() : nullptr;
        if(chdir(target) != 0)
        {
            std::cerr <<"cd : "<<std::strerror(errno)<<std::endl;
        }
        return true;
    }

    return false;
}
int main()
{
    std::string line;
    bool running = true;

    while(running)
    {
        std::cout<<"msh> "<<std::endl;

        if(std::getline(std::cin, line))
        {
            std::cout<<"\n";
            break;
        }

        auto args = tokenize(line);
        if(args.empty())
        {
            continue;
        }

        if(isbuiltin(args[0])){
            runbuiltin(args);
        } else {
            runexternal(args);
        }
    }
    return 0;
}
