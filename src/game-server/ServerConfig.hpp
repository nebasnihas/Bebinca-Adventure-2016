#ifndef ADVENTURE2016_SERVERCONFIG_HPP
#define ADVENTURE2016_SERVERCONFIG_HPP

#include <string>
#include "CommandCreator.hpp"
#include <boost/optional.hpp>

class ServerConfig {
public:
    ServerConfig(const std::string& configFileName);

    unsigned short getPort() const;
    unsigned int getTicksPerSecond() const;
    const std::string& getMapFilePath() const; //only one file for now
    const std::string& getLogDirectory() const;
    CommandCreator& getCommandCreator();
private:
    void loadConfigFile();

    std::string configFileName;

    unsigned short port;
    unsigned int ticks;
    std::string mapFilePath;
    std::string logDir;
    boost::optional<CommandCreator> commandCreator;
};


#endif //ADVENTURE2016_SERVERCONFIG_HPP
