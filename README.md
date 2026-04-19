_This project has been created as part
of the 42 curriculum by tvandoor and gderez_

# IRC

### SUMMURY:
___

- Description
- Instruction
- Resources
- Bonus

## Description
___

The purpose of this group project is to develop an IRC (Internet Relay Chat) server using C++98.
Every server begins with socket initialization. A socket is an endpoint of a two-way communication link between two programs running over a network. Creating a socket is mandatory in order to obtain a unique file descriptor that identifies the connection.
Next, the socket and server parameters are configured to establish a connection using the TCP protocol, IPv4, and a designated port. Once the connection is established, the server listens for incoming data sent by clients, receives and parses it to determine whether valid commands have been issued, and then routes the corresponding responses to the appropriate clients.

## Instruction
___

To run this project:

- Open a terminal for the server and enter:
    ```./ircserv <port> <password>'```
- Open a second terminal and enter
    ```nc localhost <port>```
- Or connect with Hexchat.

Requirements of the subject:

- We must not develop an IRC client.
- We must not implement server-to-server communication.
- The server must be capable of handling multiple clients simultaneously without hanging.
- Forking is prohibited. All I/O operations must be non-blocking.
- Only one ```poll()``` (or equivalent) can be used to handle all operations (reading, writing, listening, etc.).
- Several IRC clients exist. We had to choose one as a reference. We chose HexChat, which will be used during the evaluation process.
- Communication between the client and the server must be done via TCP/IP (IPv4 or IPv6).
- The server must support authentication, nickname and username assignment, channel joining, and private messaging using the reference client.
- All messages sent by a client to a channel must be forwarded to every other client in that channel.
- The server must support both operators and regular users.
- The following commands specific to channel operators must be implemented:
    - KICK
    - INVITE
    - TOPIC
    - MODE:
        - ```-i```
        - ```-t```
        - ```-k```
        - ```-o```
        - ```-l```

## Resources
___

https://www.geeksforgeeks.org/cpp/socket-programming-in-cpp/
https://www.ibm.com/docs/en/zos/2.5.0?topic=otap-clientserver-socket-programs-blocking-nonblocking-asynchronous-socket-calls
http://manpagesfr.free.fr/man/man2/listen.2.html
https://www.frameip.com/c-mode-connecte/
https://www.csd.uoc.gr/~hy556/material/tutorials/cs556-3rd-tutorial.pdf
https://www.codequoi.com/programmation-reseau-via-socket-en-c/
https://www.figma.com/board/TEviHsREY663xd6BxfD1UQ/ft_irc?node-id=1-2
https://www.techbull.com/techbull/guide/internet/irccommande.html

## Bonus
___

- Handle file transfer.
- Create a bot.