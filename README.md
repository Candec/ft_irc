# ft_irc

## Summary

Create an IRC server in C++.

Version: 8

## Contents

1. Introduction
2. General rules
3. Mandatory part
4. Bonus part
5. Submission and peer evaluation

## Chapter 1 - Introduction

IRC is a text-based communication protocol. It supports real-time public and private messages.

IRC clients connect to IRC servers. Servers connect to each other to form a network.

## Chapter 2 - General rules

- The program must not crash. It must not quit unexpectedly.
- Turn in a Makefile. It must not relink.
- The Makefile must have these rules: `$(NAME)`, `all`, `clean`, `fclean`, `re`.
- Compile with C++ and flags: `-Wall -Wextra -Werror`.
- Use C++ 98 standard.
- Prefer C++ features over C features.
- Do not use external libraries or Boost.

## Chapter 3 - Mandatory part

Program name: `ircserv`

Files to turn in:
- Makefile
- `*.{h, hpp, *.cpp, *.tpp, *.ipp}`
- Optional configuration file

Makefile rules:
- `NAME`
- `all`
- `clean`
- `fclean`
- `re`

Arguments:
- `port` - The listening port
- `password` - The connection password

Allowed functions:
- `socket`, `close`, `setsockopt`, `getsockname`
- `getprotobyname`, `gethostbyname`, `getaddrinfo`, `freeaddrinfo`
- `bind`, `connect`, `listen`, `accept`, `htons`, `htonl`
- `ntohs`, `ntohl`, `inet_addr`, `inet_ntoa`, `send`, `recv`
- `signal`, `sigaction`, `lseek`, `fstat`, `fcntl`, `poll`

Do not develop a client.
Do not handle server-to-server communication.

Run the program like this:

```bash
./ircserv <port> <password>
```

You may use `select()`, `kqueue()`, or `epoll()` instead of `poll()`.

### Requirements

- The server must handle multiple clients at the same time.
- Do not use `fork()`. All I/O operations must be non-blocking.
- Use only one `poll()` (or equivalent) for all operations.

You must use `poll()` (or equivalent) for all read and write operations. If you read or write without `poll()`, the server is blocking and you will get a grade of 0.

- Choose one IRC client as a reference.
- Your reference client must connect without errors.
- Use TCP/IP (v4 or v6).
- Implement these features:
  - Authentication, nickname, username, join channels
  - Send and receive private messages
  - Forward channel messages to all members
  - Support operators and regular users
  - Operator commands:
    - `KICK` - Remove a client from a channel
    - `INVITE` - Invite a client to a channel
    - `TOPIC` - Change or view the channel topic
    - `MODE` - Change channel modes:
      - `i` - Invite-only
      - `t` - Topic restriction
      - `k` - Channel password
      - `o` - Operator privilege
      - `l` - User limit

Write clean code.

### For MacOS

MacOS does not implement `write()` like other Unix systems. You may use `fcntl()` to set file descriptors to non-blocking mode.

Only use this command:

```c
fcntl(fd, F_SETFL, O_NONBLOCK);
```

Do not use other flags.

### Test example

Test all possible errors. You can test with `nc`:

```bash
$ nc 127.0.0.1 6667
com^Dman^Dd
$
```

Use `ctrl+D` to send parts of a command.

## Chapter 4 - Bonus part

Add these features:

- File transfer
- A bot

The bonus part is graded only if the mandatory part is perfect and works without errors.

## Chapter 5 - Submission and peer evaluation

Turn in your assignment in your Git repository. Only the work in the repository is evaluated.

You may create test programs. Tests are useful for the defense.

Your reference client will be used during evaluation.

SHA checksums:
- 16D85ACC441674FBA2DF65190663F432222F81AA0248081A7C1C1823F7A96F0B
- 15056E97427E5B22F07132659EC8D88B574BD62C94BB654D5835AAD889B014E
- 709F6E