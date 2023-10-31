# Tests

## Mandatory

Your executable will be run as follows:

```bash
./ircserv <port> <password>
```

### Requirements

- No forking ✔️
- Only 1 poll() ✔️
- irssi must be able to connect to your server without encountering any error.
  - test also with netcat
- Communication has to be done via TCP ✔️
- Compare with official IRC servers ❌
  - Authenticate
  - Set Nickname ✔️
  - Set Username ✔️
  - Join Channel
  - Messages to channel must be sent to every other channel member ✔️
  - Operators and regular users ✔️
  - Channel Operators commands:
    - KICK
    - INVITE
    - TOPIC
    - MODE
      - i: Set/remove Invite-only channel
      - t: Set/remove the restrictions of the TOPIC command to channel operators
      - k: Set/remove the channel key (password)
      - o: Give/take channel operator privilege
      - l: Set/remove the user limit to channel

### For MacOS only

Since MacOS doesn’t implement write() the same way as other Unix OSes, you are allowed to use fcntl().
You must use file descriptors in non-blocking mode in order to get a behavior similar to the one of other Unix OSes.

However, you are allowed to use fcntl() only as follows:
fcntl(fd, F_SETFL, O_NONBLOCK);
Any other flag is forbidden.

### Test example

Verify absolutely every possible error and issue (receiving partial data, low bandwidth, and so forth).
To ensure that your server correctly processes everything that you send to it, the following simple test using nc can be done:

\$> nc 127.0.0.1 6667
com^Dman^Dd
\$>

Use ctrl+D to send the command in several parts: ’com’, then ’man’, then ’d\n’.
In order to process a command, you have to first aggregate the received packets in order to rebuild it.

## Bonus part

- Handle file transfer.
- A bot.
