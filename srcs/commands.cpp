// # include "main.hpp"
#include "commands.hpp"

const std::string	toString(e_Cmds cmd)
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
	default:
		error("Unrecognized command: " + cmd, CONTINUE);
		return "UNKNOWN";
	}
}

e_Cmds	toEnum(const std::string &cmd)
{
	if (!cmd.compare("PASS")) {
		return Commands::PASS;
	} else if (!cmd.compare("NICK")) {
		return Commands::NICK;
	} else if (!cmd.compare("USER")) {
		return Commands::USER;
	} else if (!cmd.compare("COLOR")) {
		return Commands::COLOR;
	} else if (!cmd.compare("JOIN")) {
		return Commands::JOIN;
	} else if (!cmd.compare("QUIT")) {
		return Commands::QUIT;
	} else if (!cmd.compare("CAP")) {
		return Commands::CAP;
	} else
		return Commands::UNKNOWN;
}
