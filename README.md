# Bebinca - Text Adventure 2016

![Alt text](https://www.sfu.ca/~pmadeya/newsplash.gif "Project Intro")

The experience of a lifetime awaits! Bebinca offers you a chance to roam through vast regions from the world of Mother Goose to Midguard and abroad.Interact with different players by shouting words back and forth, or whispering them to a teammate nearby. Level up with the built-in combat system to have a chance battle your friends and foes, cast spells, swap bodies all to become the most powerful being to set foot in the universe.

If you are feeling adventurous, you also have the ability to build and create your very own custom worlds with a specialized world builder that will let you customize world to make your dreams become a reality.  

## Getting Started

To get started, the player must first run the game server. After it has fully loaded, all of your friends will now be able to join in on the adventure of a lifetime all within the same chatroom. The usage will be as follows:

i) Clone the repository

```
https://csil-git1.cs.surrey.sfu.ca/373-16-3-bebinca/adventure2016.git
```


ii) Create a new directory for building and change into it

```
mkdir build
cd build
```

iii) Run CMake with the path to the source and run make

```
cmake ../adventure2016
make
```

iv) Run the game-server executable along with the config.yml file

```
./game-server config.yaml
```

v) Run the game-client executable and specify the same port and IP as the server

```
./game-client localhost 4000
```

## Walkthrough

You will commence your journey in the center of Midguard... Travel around the world with the regular compass directions by moving North, East, West, or South. You will be meeting many characters along the way including the likes of Samylla and Xouwasi and many more! Battle your friends by casting spells on them and even body swap with other players in order to strategically become the most clever in all of the land! 

## Supported Commands

**shout - Command:[shout]**
**whisper - Command:[whisper,/w]**
Chat to a single nearby player
**status - Command:[status]**
List your current stats
**cast - Command:[cast,/c]**
Cast spells to friends and enemies
(Spell list will be shown by typing: /c)
**exits - Command:[exits]**
**help - Command:[help,/h]**
List supported games in-game
**look - Command:[look]**
**attack - Command:[attack,/a]**
**move - Command:[go,move,g]**
**players - Command:[players]**
List the players currently in the same area
**say - Command:[say]**


## Screenshots

Gameplay
![Alt text](https://www.sfu.ca/~pmadeya/Title%20screen.png "Title screen")

![Alt text](https://www.sfu.ca/~pmadeya/authentication.png "Authentication")

![Alt text](https://www.sfu.ca/~pmadeya/start_gameplay.png "First World")

![Alt text](https://www.sfu.ca/~pmadeya/Screen%20Shot%202016-11-30%20at%2010.46.35%20PM.png "Gameplay")

Combat system
![Alt text](https://www.sfu.ca/~pmadeya/combat_ui.png "Project Intro")


### Requirements

You MUST install the following software on your computer:

CMake (minimum version 3.5.1)
libboost-all-dev (Boost libraries)
libncurses-dev (ncurses library)

## Authors

* **Brandon Chong**
* **Yui Hei Tsui**
* **Melissa Wang**
* **Aaryaman Girish**
* **Nihas Nebaskhan**
* **Zachary Cesaretti**
* **Paul Madeya**
* **Josh Arik Miguel Fernandez** 