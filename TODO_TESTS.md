# Tests

## Mandatory

Your executable will be run as follows:

```bash
./ircserv <port> [password] [--log / --logfile]
```

### Requirements

- No forking ✔️
- Only 1 poll() ✔️
- irssi must be able to connect to your server without encountering any error. ✔️
  - test also with netcat ✔️
  - Server might accept client without a proper password if it send many connection requests ✔️
  - Server migh not accept client, even when all the information is correct. The welcome message is even sent, but after, the user is not able to interact with the server. The fd is created, and irssi even tries to reconnect, as it has the "reconnect after timeout" setting active by default ✔️
- Communication has to be done via TCP ✔️
- Compare with official IRC servers ❌
  - Authenticate ✔️
    - When creating a server without password, /NICK and /NAME complain the user has not provided a password ✔️
  - Set Nickname (NICK command )✔️
    - If nickname already in use, reply with *ERR_NICKNAMEINUSE* ✔️
    - If nickname not valid, reply with *ERR_ERRONEUSNICKNAME* ✔️
      - no leading # character or other character advertized in CHANTYPES ✔️
      - no leading colon (:) ✔️
      - no ASCII space ✔️
      - no ASCII simboles other than allowed ✔️
    - If nickname not received, reply with *ERR_NONICKNAMEGIVEN* ✔️
    - Send NICK message to client as acknowledgement ✔️
  - Set Username ✔️
  - Join Channel ✔️
  - Messages to channel must be sent to every other channel member ✔️
  - Operators and regular users ✔️
  - Channel Operators commands:
    - KICK `Parameters: [<channel>] <user> *( "," <user> ) [<comment>]` ✔️
      - If no comment is given, the server SHOULD use a default message instead ✔️
      - The server MUST NOT send KICK messages with multiple users to clients.
      - The only the last word of the reason is being sent to the kick user.
      - cmd [/kick maria Maria, que te vayas]
        -error: first word is not being added to the reason to kick
      - if the target user is not on the client the message is not properly handle, printed on console only, but not sent to the user sending the cmd
      - kicking a target while the user is at another channel throws a SEGV. Both clients are members of the channel the target is being kicked
      - kicking multiple users, all members of the same channel results in SEGV --> cmd [/kick fire candec,candec_]
    - INVITE `Parameters: <nickname> <channel>` ✔️
      - If \<channel> doesn't exist, reject with *ERR_NOSUCHCHANNEL* ✔️
      - If caller isn't \<channel> member, reject with *ERR_NOTONCHANNEL* ✔️
      - If caller isn't \<channel> operator, reject with *ERR_CHANOPRIVSNEEDED* ✔️
      - If \<user>'s already on \<channel>, reject with *ERR_USERONCHANNEL* ✔️
      - If INVITE is successful, send:
        - *RPL_INVITING* to caller ✔️
        - INVITE message (with caller as \<source>) to \<user> ✔️
        - Other \<channel> members SHOULD NOT be notified. ✔️
    - TOPIC `Parameters: <channel> [<topic>]` ✔️
      - Only the first word is being set as topic, even when the whole sentence is between quotes ("") ✔️
      - If no \<topic> is given, reply with *RPL_TOPIC* or *RPL_NOTOPIC* ✔️
      - If \<topic> is an empty string, the \<channel>'s topic is cleared ✔️
        - In irssi: TOPIC -delete \<channel>
      - ~~If caller isn't on \<channel>, server MAY reject with *ERR_NOTONCHANNEL*~~
      - If *RPL_TOPIC* is returned to caller, *RPL_TOPICWHOTIME* SHOULD also be sent to that client ✔️
      - If **protected topic** mode is set on \<channel> and caller isn't operator, reject with *ERR_CHANOPRIVSNEEDED* ✔️
      - If topic is set or cleared, every client in \<channel> will receive a TOPIC command with the new topic or an empty string as argument, depending on if the topic was set or cleared ✔️
    - MODE `Parameters: <target> [<modestring> [<mode arguments>...]]` ✔️
      - i: Set/remove Invite-only channel -- Invitations are not being properly registered ✔️
        - If this mode is set, users must have received an INVITE to be able to join. If they try joining without it, reject JOIN with *ERR_INVITEONLYCHAN* ✔️
      - t: Set/remove the restrictions of the TOPIC command to channel operators ✔️
        - If mode is set on channel, TOPIC caller isn't operator and is trying to set the topic, reject with *ERR_CHANOPRIVSNEEDED* ✔️
      - k: Set/remove the channel key (password) ✔️
        - If, when setting the key, it's invalid, reject with *ERR_INVALIDMODEPARAM*/*ERR_INVALIDKEY*
        - If this mode is set and a client doesn't supply the correct key, reject JOIN with *ERR_BADCHANNELKEY* ✔️
      - o: Give/take channel operator privilege ✔️
        - Users with this mode may:
          - Kick users
          - Apply channel modes ✔️
          - Set other users to operator (or lower) status ✔️
      - l: Set/remove the user limit to channel ✔️
    - NAME ✔️
      - When a user creates a channel, the message points there are 2 normal users and no operator (the creator of the channel should be op by default and there should be no normal users instead of 2) ✔️
      - When a second user joins, the second user sees all the people in the channel, but previous users do not see the new members added to the list ✔️
    - PART ✔️
      - The leaving message is being sent to the user leaving the channel instead of broadcasting it to the rest of users in the channel ✔️
      - When the owner leave the channel after a user has been in the channel (even when the second user had already leave the channel), errors are generated ✔️

### For MacOS only

Since MacOS doesn’t implement write() the same way as other Unix OSes, you are allowed to use fcntl().
You must use file descriptors in non-blocking mode in order to get a behavior similar to the one of other Unix OSes.

However, you are allowed to use fcntl() only as follows:
fcntl(fd, F_SETFL, O_NONBLOCK);
Any other flag is forbidden.

### Test example

Verify absolutely every possible error and issue (receiving partial data, low bandwidth, and so forth).
To ensure that your server correctly processes everything that you send to it, the following simple test using nc can be done: ✔️

\$> nc 127.0.0.1 6667
com^Dman^Dd
\$>

Use ctrl+D to send the command in several parts: ’com’, then ’man’, then ’d\n’.
In order to process a command, you have to first aggregate the received packets in order to rebuild it.

## Bonus part

- Handle file transfer.
- A bot.

## Correction

### Mandatory Part

#### Basic checks

- There is a Makefile, the project compiles correctly with the required options, is written in C++, and the executable is called as expected.
- Ask and check how many poll() (or equivalent) are present in the code. There must be only one. ✔️
- Verify that the poll() (or equivalent) is called every time before each accept, read/recv, write/send. After these calls, errno should not be used to trigger specific action (e.g. like reading again after errno == EAGAIN).
- Verify that each call to fcntl() is done as follows: fcntl(fd, F_SETFL, O_NONBLOCK); Any other use of fcntl() is forbidden. ✔️
- If any of these points is wrong, the evaluation ends now and the final mark is 0.

#### Networking

Check the following requirements:

- The server starts, and listens on all network interfaces on the port given from the command line. ✔️
- Using the 'nc' tool, you can connect to the server, send commands, and the server answers you back. ✔️
- Ask the team what is their reference IRC client.
- Using this IRC client, you can connect to the server. ✔️
- The server can handle multiple connections at the same time. The server should not block. It should be able to answer all demands. Do some test with the IRC client and nc at the same time. ✔️
- Join a channel thanks to the appropriate command. Ensure that all messages from one client on that channel are sent to all other clients that joined the channel. ✔️

#### Networking specials

Network communications can be disturbed by many strange situations.

- Just like in the subject, using nc, try to send partial commands. Check that the server answers correctly. With a partial command sent, ensure that other connections still run fine. ✔️
- Unexpectedly kill a client. Then check that the server is still operational for the other connections and for any new incoming client. 🟡
  - It crashes the server (sometimes), I think it would be best to check user status before each send. Ctrl-C'd netcat and one of the times it closed without error message when trying to send PART command to netcat's user but the other time it worked.
- Unexpectedly kill a nc with just half of a command sent. Check again that the server is not in an odd state or blocked.
- Stop a client (^-Z) connected on a channel. Then flood the channel using another client. The server should not hang. When the client is live again, all stored commands should be processed normally. Also, check for memory leaks during this operation.

#### Client Commands basic

- With both nc and the reference IRC client, check that you can authenticate, set a nickname, a username, join a channel. This should be fine (you should have already done this previously). ✔️
- Verify that private messages (PRIVMSG) are fully functional with different parameters.

#### Client Commands channel operator

- With both nc and the reference IRC client, check that a regular user does not have privileges to do channel operator actions. Then test with an operator. All the channel operation commands should be tested (remove one point for each feature that is not working).

### Bonus part

Evaluate the bonus part if, and only if, the mandatory part has been entirely and perfectly done, and the error management handles unexpected or bad usage. In case all the mandatory points were not passed during the defense, bonus points must be totally ignored.

- File transfer works with the reference IRC client.
- There's an IRC bot.
