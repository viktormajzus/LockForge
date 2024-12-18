# LockForge - Your Local-Only Secure Password Vault

LockForge is a simple, local-only password manager built with **C++**. Unlike mainstream password managers, **your data never leaves your machine**. LockForge is currently in **early development**, with only initial class headers written.

---

## 🚀 **Planned Features**

- **Local-Only Encryption**: No cloud, no sync. Everything stays on your machine.
- **Chunk-Based Encryption**: Only the requested account is decrypted, reducing risk if the system is compromised.
- **Argon2-Backed Master Password**: Secure password hashing with Argon2.
- **AES-256-CBC**: Data encryption for your credentials.
- **HMAC-SHA-256**: Integrity checks to detect file tampering.
- **Cross-Platform**: Works on **Windows, macOS, and Linux**.
- **Browser Extension API**: Planned API support for browser extension integration.

---

## 📋 **To-Do (How You Can Contribute)**

- **Implement Core Functions**: Build out the methods in `FileHandler` and `Crypt` classes.
- **Command Line Functionality**: Write the logic for commands like `add`, `get`, `remove`, `help`, `change`, and `delete`.
- **Error Handling**: Add proper error handling and user feedback.
- **File Encryption/Decryption**: Implement the AES-256-CBC encryption logic and file read/write logic.
- **API for Browser Extension**: Start development of an API to allow LockForge to integrate with browser extensions.
- **Tests**: Write unit tests for `FileHandler`, `Crypt`, and API functions.

If you'd like to contribute, see the **Contributing** section below.

---

## 🔐 **How LockForge Will Work**

LockForge will secure your credentials in an encrypted vault file. Each service and its corresponding username and password will be stored as independent, encrypted chunks. This structure ensures that **only the chunk you're accessing is decrypted**—not the entire vault.

### **File Structure**
```
+----------------------+-------------------------+
| Encrypted Service 1  | Encrypted Block 1       |
+----------------------+-------------------------+
| Encrypted Service 2  | Encrypted Block 2       |
+----------------------+-------------------------+
| Encrypted Service N  | Encrypted Block N       |
+----------------------+-------------------------+
```
---

## ⚙️ **Planned Commands**

| **Command**  | **Usage**                        | **Description**                         |
|--------------|----------------------------------|-----------------------------------------|
| `add`        | `add <service> <username> <password>` | Add a new credential to the vault.    |
| `remove`     | `remove <service>`               | Remove a credential from the vault.    |
| `get`        | `get <service>`                  | Retrieve the username & password.      |
| `change`     | `change <service> <new password>` | Change the master password.           |
| `delete`     | `delete`                         | **Deletes the entire vault file.**      |
| `help`       | `help`                           | Display command usage.                 |

**Example Usage (Planned):**
```bash
lockforge add github john_doe Hunter2
lockforge get github
lockforge remove github
lockforge change github NewPassword123
lockforge delete
```

---

## 🔧 **Current State**

- **Only class headers have been written**. The core functionality is still being implemented.
- No encryption or file handling logic is currently in place.
- **Next steps**: 
  - Implement core functions in the `FileHandler` and `Crypt` classes.
  - Add functionality for `add`, `get`, `remove`, `change`, and `delete` commands.
  - Build and test the command-line interface (CLI).
  - Start development on the **Browser Extension API**.

---

## 📦 **Building LockForge**

**Prerequisites:**
- **CMake** (for cross-platform builds)
- **OpenSSL** (for AES, HMAC, and Argon2 support)
- **A C++ compiler** (GCC, Clang, or MSVC)

**Build Instructions (Once Functional):**
```bash
git clone https://github.com/your-username/LockForge.git
cd LockForge
cmake -B build
cmake --build build
```
This will generate the LockForge binary. To test it, you will eventually run:
```bash
./build/LockForge help
```

---

## 🛠️ **Contributing**
If you'd like to contribute, here's how to get started:
1. Fork the repo.
2. Clone your forked version locally.
3. Create a new branch (`git checkout -b feature-name`).
4. Write your feature or bugfix.
5. Push your changes and create a pull request.

---

## ⚠️ **Current Limitations**
- **Only Class Headers Exist**: No working commands or core functionality are in place.
- **File Encryption Not Yet Implemented**: The encryption and decryption logic is still in progress.
- **No Error Handling**: Error messages and validation are not yet implemented.
- **API for Browser Extension**: The browser extension API is still in the planning phase.

---

## ⚖️ **License**
LockForge is licensed under the **MIT License**, so you're free to use, modify, and distribute it as you like. See [LICENSE](LICENSE) for details.

---

## ⭐ **Support**
If you'd like to support the project, give it a ⭐ on GitHub, submit an issue, or send feedback. We’re always looking for ways to improve LockForge.

**Built with 💪 C++ and a passion for privacy.**

