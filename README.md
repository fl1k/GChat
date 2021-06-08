# GChat - Global Chat
GChat is a multi-client chat application with a login system. Packets are sent using a TCP connection. 
It supports 2 types of clients, a chat client and a SAMP (0.3.7 R1) client. Every message that starts with a dot '.' will be forwarded to the GChat Server.

It includes GNet library which is a wrapper around system's sockets.


## FEATURES
- Login system (local SQLite database)
- Privilege system (Admin(2), Moderator(1), Default(0))
- Commands based on privilege
  *) default commands: help, list, logout, login, disconnect
  *) staff commands: adduser, removeuser, kick
- Chat between gameservers, games, operating systems
- Chat competely private, only visible to those on the same GServer! Ideal for groups and clans, and communities spanning on different servers


## SERVER SETUP
- If LAN server go to step 2)
1) Open 9920 port for TCP connections
2) Run the server, it will prompt you to enter the first user (you) and it'll be given Admin privilege.
3) You may login using the crediantals you provided, using the /adduser command you may add new users


## SAMP CLIENT SETUP ( 0.3.7 R1 supported only )
1) Place .asi file in your SAMP directory. Once you run the game a gc-config.ini file will be created, in this config file you will put the ip, port, your username, your password, if the server is using a hostname (URL - google.com) you will change useHostname to true.
2) Run the game, it should say "GChat initialized" 
3) Use dot ('.') before your messages to forward them to the GServer you're connected on

https://www.youtube.com/watch?v=Ojak_z0-_9M
