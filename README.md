# Network-Project
This project aims to improve my skills on networking within games via a small bomberman-like game.
The project will make use of a server that will handle & validate data received by the clients before processing it.
Afterwards, it will inform all clients about the new state of the game.
The client & server can also communicate between eachother while running on different platforms.

# Setup
This project supports x86 & x64 for Windows as well as ARM & ARM64 for Linux.

### x86-x64 | Windows
To setup the project on Windows, simply download or clone the project and run both the client & server.
As the game is unable to be played alone, the server must have at least 2 clients connected to the server.

### ARM-ARM64 | Linux
To setup the project on ARMs, make sure a remote machine that supports ARM is connected to your Visual Studio.
Make sure to first install SFML on the remote machine via the following command `sudo apt-get install libsfml-dev` if you wish to use the **client**.
Afterwards, either download or clone the project and run both the client & server.
As the game is unable to be played alone, the server must have at least 2 clients connected to the server.

# Known bugs
#1 Sometimes a chat message will be send & parsed twice in a row.

#2 Debug | Win32 does not compile
