// # include "main.hpp"
#include "../includes/commands.hpp"

const std::string cmdToString(int cmd)
{
	switch (cmd)
	{
	case Commands::PASS:
		return "PASS";
	case Commands::NICK:
		return "NICK";
	case Commands::USER:
		return "USER";
	case Commands::COLOR:
		return "COLOR";
	case Commands::JOIN:
		return "JOIN";
	case Commands::QUIT:
		return "QUIT";
	case Commands::CAP:
		return "CAP";
	case Commands::TOPIC:
		return "TOPIC";
	case Commands::PRIVMSG:
		return "PRIVMSG";
	case Commands::AWAY:
		return "AWAY";
	case Commands::PING:
		return "PING";
	case Commands::MODE:
		return "MODE";
	case Commands::KICK:
		return "KICK";
	case Commands::INVITE:
		return "INVITE";
	default:
		error("Unrecognized command: " + toString(cmd), CONTINUE);
		return "UNKNOWN";
	}
}

int cmdToEnum(const std::string &cmd)
{
	if (cmd == "PASS") {
		return Commands::PASS;
	} else if (cmd == "NICK") {
		return Commands::NICK;
	} else if (cmd == "USER") {
		return Commands::USER;
	} else if (cmd == "COLOR") {
		return Commands::COLOR;
	} else if (cmd == "JOIN") {
		return Commands::JOIN;
	} else if (cmd == "QUIT") {
		return Commands::QUIT;
	} else if (cmd == "CAP") {
		return Commands::CAP;
	} else if (cmd == "TOPIC") {
		return Commands::TOPIC;
	} else if (cmd == "PRIVMSG") {
		return Commands::PRIVMSG;
	} else if (cmd == "AWAY") {
		return Commands::AWAY;
	} else if (cmd == "PING") {
		return Commands::PING;
	} else if (cmd == "MODE") {
		return Commands::MODE;
	} else if (cmd == "KICK") {
		return Commands::KICK;
	} else if (cmd == "INVITE") {
		return Commands::INVITE;
	} else
		return Commands::UNKNOWN;
}
