#pragma once

class IObject;

enum Commands
{
	COMMAND_MOVE,
	COMMAND_ENTER,
	COMMAND_JUMP,
	COMMAND_CLICK,
	COMMAND_CANCEL
};

struct Command
{
	int command;
	Command* next;

	Command() : next(nullptr) {}
};
