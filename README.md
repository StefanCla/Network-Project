# Network-Project
This project aims to improve my skills on networking within games via a small bomberman-like game.
The project will make use of a server that will handle & validate data received by the clients before processing it.
Afterwards, it will inform all clients about the new state of the game.

# Setup
This project will be available on both Windows and Linux platforms.

### Windows
To setup the project on Windows, simply download or clone the project and run both the client & server.
As the game is unable to be played alone, the server must have at least 2 clients connected to the server.

### Linux
To setup the project on Linux, make sure to first install SFML via the following command `sudo apt-get install libsfml-dev` if you wish to use the client.
Afterwards, either download or clone the project and run both the client & server.
As the game is unable to be played alone, the server must have at least 2 clients connected to the server.

# Known bugs
#1 Sometimes a chat message will be send & parsed twice in a row.
