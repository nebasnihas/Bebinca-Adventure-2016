#ifndef ADVENTURE2016_DATA_HPP
#define ADVENTURE2016_DATA_HPP

#include "RequestMessage.hpp"
#include "ResponseMessage.hpp"

namespace protocols {

enum class DataType {
    AREA,
};

struct GameData {
    DataType type;
    YAML::Node data;
};
//TODO maybe there needs to be extra information in the request
RequestMessage createDataRequest(DataType data);
DataType readDataRequest(const RequestMessage& requestMessage);

ResponseMessage createDataResponse(const GameData& data);
GameData readDataResponse(const ResponseMessage& responseMessage);

}

#endif //ADVENTURE2016_DATA_HPP_HPP