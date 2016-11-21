#include <game/protocols/Data.hpp>
#include <glog/logging.h>

namespace {
const std::string DATA_TYPE_KEY = "datatype";
const std::string DATA_BODY_KEY = "databody";

protocols::DataType getDataTypeFromYaml(const YAML::Node& node) {
    auto val = node[DATA_TYPE_KEY];
    CHECK(val) << "Invalid node for data type";

    return static_cast<protocols::DataType>(val.as<int>());
}
}

namespace protocols {

RequestMessage createDataRequest(DataType data) {
    YAML::Node node;
    node[DATA_TYPE_KEY] = static_cast<int>(data); //have to cast it explicitly or else it gives incompelte type error. havent checked why

    RequestMessage request{RequestHeader::DATA_REQUEST, node};
    return request;
}

DataType readDataRequest(const RequestMessage& requestMessage) {
    return getDataTypeFromYaml(requestMessage.body);
}

ResponseMessage createDataResponse(const GameData& data) {
    YAML::Node node;
    node[DATA_TYPE_KEY] = static_cast<int>(data.type);
    node[DATA_BODY_KEY] = data.data;

    ResponseMessage responseMessage{ResponseHeader::DATA_RESPONSE, node};
    return responseMessage;
}

GameData readDataResponse(const ResponseMessage& responseMessage) {
    auto dataType = getDataTypeFromYaml(responseMessage.body);
    auto dataBody = responseMessage.body[DATA_BODY_KEY];
    return GameData{dataType, dataBody};
}

}