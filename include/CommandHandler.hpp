#pragma once
#include <vector>
#include <string>

/// @brief This class is used to handle the command-line input from the user.
/// @details This class performs actions based on the commands from the user, such as: add, remove, get, change, delete and help.
class CommandHandler
{
private:
  /// @brief Stores the command-line arguments.
  std::vector<std::string> arguments{};

public:
  /// @brief Converts the C-style command-line arguments to C++-style.
  CommandHandler(int argumentsCount, char** arguments);

  /// @brief Handles the command-line input from the user or prints a message if the format is not correct.
  void handle();

private:
  /// @brief Handles the add command. Not yet implemented.
  void handleAdd();

  /// @brief Handles the remove command. Not yet implemented.
  void handleRemove();

  /// @brief Handles the get command. Not yet implemented.
  void handleGet();

  /// @brief Handles the change command. Not yet implemented.
  void handleChange();

  /// @brief Handles the delete command. Not yet implemented.
  void handleDelete();

  /// @brief Handles the help command by printing to the terminal the list of supported commands.
  void handleHelp();
};