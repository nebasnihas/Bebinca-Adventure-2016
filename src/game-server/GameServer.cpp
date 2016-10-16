/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include <boost/lexical_cast.hpp>
#include "glog/logging.h"
#include "ServerLoop.hpp"

void validateServerArgs(int argc, char* argv[], unsigned short& port, std::string& filename) {
    if (argc < 3) {
        std::cout << "Usage:\n" << argv[0] << " <port> <pathToMap>\ne.g. " << argv[0] << "4002 mgoose.yml\n" << std::endl;
        exit(1);
    }

    try {
        port = boost::lexical_cast<ushort>(argv[1]);
    } catch (const boost::bad_lexical_cast&) {
        std::cerr << "Invalid port number" << std::endl;
        exit(1);
    }

    filename = std::string(argv[2]);
}

int main(int argc, char *argv[]) {
    //TODO configuration using file
    FLAGS_log_dir = "./";
    google::InitGoogleLogging("GameServer");
    unsigned short port;
    std::string sourceFile;
    validateServerArgs(argc, argv, port, sourceFile);

    //start server
    ServerLoop loop{port, sourceFile};
    Looper looper{5}; //TODO add tick rate to config file
    looper.run(loop);

    return 0;
}

