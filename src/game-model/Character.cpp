#include <game/Character.hpp>

Character::Character() {}

Character::Character(const std::string& id,
                     const std::string& name,
                     std::string& hit,
                     std::string& damage,
                     int level,
                     int exp,
                     int armor,
                     int gold,
                     Inventory inventory,
                     std::string& areaID
                    )

                    :
                     id(id)
                    , name(name)
                    , hit(hit)
                    , damage(damage)
                    , level(level)
                    , exp(exp)
                    , armor(armor)
                    , gold(gold)
                    , areaID(areaID)

                    {
                       inventory = Inventory(inventory); //Unsure if this works/keeps consistency (Understatement of the year)
                    }


const std::string Character::defaultID = "0";
const std::string Character::defaultName = "noName";
const std::string Character::defaultHit = "2d7+98";
const std::string Character::defaultDamage = "1d7+2";

//Getters
std::string Character::getID() const {
    return id;
}

std::string Character::getName() const {
    return name;
}

std::string Character::getHit() const {
    return hit;
}

std::string Character::getDamage() const {
    return damage;
}

std::string Character::getAreaID() const {
    return areaID;
}

int Character::getLevel() const {
    return level;
}

int Character::getExp() const {
    return exp;
}

int Character::getMaxHealth() const {
    return this->maxHealth;
}

int Character::getCurrentHealth() const {
    return this->currentHealth;
}

int Character::getMaxMana() const {
    return this->maxMana;
}

int Character::getCurrentMana() const {
    return this->currentMana;
}

int Character::getGold() const{
    return gold;
}

Inventory& Character::getInventory() {
    return inventory;
}

std::vector<std::shared_ptr<StatusEffect>>& Character::getStatusEffects() {
    return statusEffects;
}

// Setters

CharacterState Character::getState() const {
    return state;
}

MessageBuffer Character::getOutputBuffer() {
	return outputBuffer;
}

void Character::setID(const std::string& id) {
    this->id = id;
}

void Character::setName(const std::string& name) {
    this->name = name;
}

void Character::setDamage(const std::string& damage) {
    this->damage = damage;
}

void Character::setArmor(int armor) {
    this->armor = armor;
}

void Character::setGold(const int gold) {
    this->gold = gold;
}

void Character::setLevel(int newLevel) {
    this->level = newLevel;
}

void Character::setMaxHealth(int maxHealth) {
    this->maxHealth = maxHealth;
}

void Character::setCurrentHealth(int currentHealth) {
    this->currentHealth = currentHealth;
}

void Character::setMaxMana(int maxMana) {
    this->maxMana = maxMana;
}

void Character::setCurrentMana(int currentMana) {
    this->currentMana = currentMana;
}

void Character::setInventory(const std::string& objectID) {
    Inventory inventoryNew = this->inventory;
    inventoryNew.addItem(objectID);
}

void Character::increaseLevel() {
    this->level++;
}

void Character::increaseExp(int expToAdd) {
    this->exp += expToAdd;
}

void Character::setState(CharacterState state) {
    this->state = state;
}
void Character::setAreaID(const std::string& newAreaID){
    this->areaID = newAreaID;
}

void Character::addStatusEffect(std::shared_ptr<StatusEffect> statusEffect) {
    statusEffects.push_back(statusEffect);
}
void Character::setOutputBuffer(MessageBuffer outputBuffer) {
	this->outputBuffer = std::move(outputBuffer);
}

void Character::pushToBuffer(const std::string message) {
	outputBuffer->push_back(message);
}

std::string Character::getStatus() {
	std::string status = name + "\n" +
						"Health: " + std::to_string(currentHealth) + "\n" +
						"Mana: " + std::to_string(currentMana) + "\n" +
						"Level: " + std::to_string(getLevel());
	return status;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////                                     NPC Subclass                                               ///////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

NPC::NPC(const std::string& id,
                   const std::string& name,
                   std::string& hit,
                   std::string& damage,
                   int level,
                   int exp,
                   int armor,
                   int gold,
                   Inventory inventory,
                   std::string& areaID,
                   int thac0,
                   const std::string& description,
                   const std::string& keywords,
                   const std::string& longDesc,
                   std::vector<NPCScripts>& scripts
                    )

                    : Character(id,
                        name, //Shortdesc is a name
                        hit,
                        damage,
                        level,
                        exp,
                        armor,
                        gold,
                        inventory,
                        areaID
                        )

                    , thac0(thac0)
                    , description(description)
                    , keywords(keywords)
                    , longDesc(longDesc)
                    , scripts(scripts)

                    {
                    }

const std::vector<std::string> NPC::defaultDescription = {"noDescription", " "};
const std::vector<std::string> NPC::defaultKeywords = {"noKeywords", " "};
const std::vector<std::string> NPC::defaultLongDescription = {"noDescription", " "};


//Getters
std::string NPC::getDescription() const {
    return description;
}

std::string NPC::getKeywords() const {
    return keywords;
}

std::string NPC::getlongDesc() const {
    return longDesc;
}

int NPC::getThac0() const {
    return thac0;
}

std::vector<NPCScripts> NPC::getScripts() const {
    return scripts;
}

int NPC::getCounter() const {
    return counter;
}

void NPC::increaseCounter() {
    this->counter++;
}

void NPC::setCounter(int newCount) {
    this->counter = newCount;
}
