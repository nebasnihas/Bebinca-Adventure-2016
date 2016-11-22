#include <gtest/gtest.h>
#include <game/GameDataImporter.hpp>
#include <game/YamlAreaBuilder.hpp>
#include <game/Area.hpp>

class YamlAreaBuilderTest : public ::testing::Test{
public:
    const std::vector<std::string> desc = {"Lovely", "place"};
    const std::vector<std::string> extended_descriptions = {"Beauty", "Prime"};
    const std::string id = "2016";
    const std::string name = "bobby";


    const std::vector<std::string> doorDir = {"East", "North", "West"};
    const std::vector<std::string> doorTo = {"2037", "2090", "3923"};

protected:

    virtual void SetUp() {
        rootNode = YamlAreaBuilder::populateAreasNode(desc, extended_descriptions, id, name,
                                                      doorDir, doorTo);
    }
    YAML::Node rootNode;
};


TEST_F(YamlAreaBuilderTest, AreaParse){
    std::vector<Area> room = GameDataImporter::getRooms(rootNode);
    std::string ID = room[0].Area::getID();
    std::string name = room[0].Area::getTitle();

    std::unordered_map<std::string, std::string>* doors = room[0].Area::getConnectedAreas();

    for (const auto &door : *doors) {
        std::cout << door.first << std::endl;
        std::cout << door.second << std::endl;

    }



    EXPECT_EQ(doors->at("East"), "2037");
    EXPECT_EQ(doors->at("West"), "3923");
    EXPECT_EQ(room.size(), 1);
    //EXPECT_EQ(doorDir[0], "East");
    EXPECT_EQ(name, "bobby");
    EXPECT_EQ(ID, "2016");
}