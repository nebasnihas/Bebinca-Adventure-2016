//
//
//

#include "ymlSerializer.hpp"


namespace {
    const std::string SAVE_LOCATION = "savefiles/";
    const std::string USERNAME_KEY = "username";
    const std::string PASSWORD_KEY = "password";
    const std::string ID = "id";
    const std::string NAME = "name";
    const std::string AREAID = "area_id";
    const std::string DAMAGE = "damage";
    const std::string ARMOR = "armor";
    const std::string GOLD = "gold";
    const std::string LEVEL = "level";
    const std::string CURRENTMANA = "current_mana";
    const std::string CURRENTHEALTH = "current_health";
    const std::string EXPERIENCE = "exp";
}


std::string ymlSerializer::get_saveloc(const std::string& username){
    std::string send_loc = SAVE_LOCATION + username + ".yml";
    return send_loc;
}


Character ymlSerializer::load_from_file(const std::string &username) {

    Character c;
    YAML::Node user_yaml_file = YAML::LoadFile(get_saveloc(username));

    auto area_id = user_yaml_file[AREAID].as<std::string>();
    auto armor = user_yaml_file[ARMOR].as<int>();
    auto current_health = user_yaml_file[CURRENTHEALTH].as<int>();
    auto current_mana = user_yaml_file[CURRENTMANA].as<int>();
    auto damage = user_yaml_file[DAMAGE].as<std::string>();
    auto exp = user_yaml_file[EXPERIENCE].as<int>();
    auto gold = user_yaml_file[GOLD].as<int>();
    auto id = user_yaml_file[ID].as<std::string>();
    auto level = user_yaml_file[LEVEL].as<int>();
    auto name = user_yaml_file[NAME].as<std::string>();

    c.setID(id);
    c.setAreaID(area_id);
    c.setArmor(armor);
    c.setCurrentHealth(current_health);
    c.setCurrentMana(current_mana);
    c.setDamage(damage);
    c.setLevel(level);
    c.setName(name);
    c.setGold(gold);

    return c;


}