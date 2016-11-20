# Bebinca - Text Adventure 2016

The experience of a lifetime awaits! Bebinca offers you a chance to roam through vast regions from the world of Mother Goose to abroad.
Interact with different players by shouting words back and forth, or whispering them to a teammate nearby. Level up with the built-in combat system to have a chance battle your friends and foes, cast spells, swap bodies all to become the most powerful being to set foot in the universe.

## Getting Started

To get started, the player must first run the game server. After that has loaded, up to 8 players will be able to join the adventure
at once. The usage will be as follows:

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

iv) Run the game-server executable with the config.yml file

```
./game-server config.yaml
```

v) Run the game-client executable and specify the same port and IP as the server

```
./game-client localhost 4000
```


### Requirements

You MUST install the following software on your computer:

CMake (minimum version 3.5.1)
libboost-all-dev (Boost libraries)
libncurses-dev (ncurses library)

### 


## Authors

* **Brandon Chong**
* **Yui Hei Tsui**
* **Melissa Wang**
* **Aaryaman Girish**
* **Nihas Nebaskhan**
* **Zachary Cesaretti**
* **Paul Madeya**
* **Josh Arik Miguel Fernandez** 