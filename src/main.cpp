#include <iostream>
#include <string>
#include <string_view>

#include "PathHandler.hpp"
#include "Hash.hpp"
#include "FileHandler.hpp"

int main()
{
  PathHandler pathHandler = PathHandler();
  std::string path = pathHandler.GetPath();
  path += "lockforge.dat";

  FileHandler fh(path);

  std::string username, password;

  // Add credentials
  fh.addCredential("reddit", "user123", "password123");

  // Find credentials
  fh.findCredential("linkedin", username, password);
  std::cout << "Username: " << username << ", Password: " << password << std::endl;

  // Try to find a credential that doesn't exist
  if (!fh.findCredential("linkedin", username, password))
  {
    std::cout << "Credential not found." << std::endl;
  }

  return 0;
}