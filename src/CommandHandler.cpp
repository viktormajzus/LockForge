#include <iostream>

#include "CommandHandler.hpp"

CommandHandler::CommandHandler(int argumentsCount, char** arguments)
{
  for (int index = 0; index < argumentsCount; ++index)
  {
    this->arguments.push_back(arguments[index]);
  }
}

void CommandHandler::handle()
{
  if (arguments.size() == 0)
  {
    std::cout << "Command is missing!\n";
  }
  else if (arguments[0] == "add")
  {
    handleAdd();
  }
  else if (arguments[0] == "remove")
  {
    handleRemove();
  }
  else if (arguments[0] == "get")
  {
    handleGet();
  }
  else if (arguments[0] == "change")
  {
    handleChange();
  }
  else if (arguments[0] == "delete")
  {
    handleDelete();
  }
  else if (arguments[0] == "help")
  {
    handleHelp();
  }
}

void CommandHandler::handleAdd()
{
  if (arguments.size() < 4)
  {
    std::cout << "Argument is missing... \'help\' command has more information!\n";
    return;
  }

  std::cout << "TODO: implement handling for 'add' command\n";
  // arguments[1] is <service>
  // arguments[2] is <username>
  // arguments[3] is <password>
}

void CommandHandler::handleRemove()
{
  if (arguments.size() < 2)
  {
    std::cout << "Argument is missing... \'help\' command has more information!\n";
    return;
  }

  std::cout << "TODO: implement handling for 'remove' command\n";
  // arguments[1] is <service>
}

void CommandHandler::handleGet()
{
  if (arguments.size() < 2)
  {
    std::cout << "Argument is missing... \'help\' command has more information!\n";
    return;
  }

  std::cout << "TODO: implement handling for 'get' command\n";
  // arguments[1] is <service>
}

void CommandHandler::handleChange()
{
  if (arguments.size() < 3)
  {
    std::cout << "Argument is missing... \'help\' command has more information!\n";
    return;
  }

  std::cout << "TODO: implement handling for 'change' command\n";
  // arguments[1] is <service>
  // arguments[2] is <new password>
}

void CommandHandler::handleDelete()
{
  std::cout << "TODO: implement handling for 'delete' command\n";
}

void CommandHandler::handleHelp()
{
  std::cout << "Commands list:\n"
            << "Add a new credential to the vault:  add <service> <username> <password>\n"
            << "Remove a credential from the vault: remove <service>\n"
            << "Retrieve the username & password:   get <service>\n"
            << "Change the master password:         change <service> <new password>\n"
            << "Deletes the entire vault file:      delete\n"
            << "Display command usage:              help\n";
}