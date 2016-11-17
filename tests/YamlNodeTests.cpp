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
    const std::vector<std::string> doorDesc {"Spooky", "room"};
    const std::vector<std::string> keywords {"Creaky", "wood"};
    const std::string doorDir = "North";
    const std::string doorTo = "2037";

protected:

    virtual void SetUp() {
        rootNode = YamlAreaBuilder::populateAreasNode(desc, extended_descriptions, id, name,
                                                        doorDesc, keywords, doorDir, doorTo);
    }
    YAML::Node rootNode;
};


TEST_F(YamlAreaBuilderTest, AreaParse){
    std::vector<Area> room = GameDataImporter::getRooms(rootNode);
    std::string ID = room[0].Area::getID();
    EXPECT_EQ(room.size(), 1);
    EXPECT_EQ(ID, "2016");
}