#include <gtest/gtest.h>
#include <game/protocols/Data.hpp>

using namespace protocols;

TEST(DataProtocolTests, BasicRequestResponse) {
    auto request = createDataRequest(DataType::AREA);
    ASSERT_TRUE(request.header == RequestHeader::DATA_REQUEST);
    ASSERT_TRUE(request.body);

    auto requestDataType = readDataRequest(request);
    ASSERT_TRUE(requestDataType == DataType::AREA);

    const std::string key = "key";
    const std::string value = "val";
    YAML::Node dataBody;
    dataBody[key] = value;
    GameData data{DataType::AREA, dataBody};
    auto response = createDataResponse(data);
    ASSERT_TRUE(response.header == ResponseHeader::DATA_RESPONSE);
    ASSERT_TRUE(response.body);

    auto gameData = readDataResponse(response);
    ASSERT_TRUE(gameData.type == DataType::AREA);
    ASSERT_TRUE(gameData.data);
    ASSERT_TRUE(gameData.data[key].as<std::string>() == value);
}