
# **R-type**

Third year project part of CPP module at EPITECH
## **Table of Contents**
- [About](#about)
- [Documentation](#documentation)
- [Features](#features)
- [Requirements](#requirements)
- [Installation](#installation)
- [Build Instructions](#build-instructions)
- [Usage](#usage)
- [License](#license)
- [Authors](#authors)
- [Contact](#contact)

---

## **About**

- **Purpose**: Create an ECS and a server to make a video game in cpp
- **Goal**: Recreate R-Type with more features such as multiplayer

This project is a cross-platform R-Type game engine designed to provide a classic arcade experience with modern enhancements.

---

## **Documentation**

- **Link**: https://rtype-3.gitbook.io/rtype-doc/

---

## **Features**

- Multiplayer support
- Modular architecture
- Compatibility with Windows / Unix Systems
- Easy setup with cmake

---

## **Requirements**

- **Operating Systems**: Windows 10/11, Ubuntu 20.04+, macOS 11+
- **Build Tools**: CMake 3.22+, a C++17 compiler (GCC, Clang or Visual Studio 17 2022)
- **Languages**: C++17
- **Linux only**: the system packages SFML needs, e.g.
  ```bash
  sudo apt install build-essential cmake libx11-dev libxrandr-dev libxcursor-dev libxi-dev \
      libudev-dev libgl1-mesa-dev libfreetype6-dev libopenal-dev libvorbis-dev libflac-dev
  ```

---

## **Installation**

```bash
git clone https://github.com/Chapitoj/rtype.git r-type
cd r-type
```

---

## **Build Instructions**

### Linux / macOS
```bash
./bashbuild.sh
```
or by hand:
```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_POLICY_VERSION_MINIMUM=3.5
cmake --build . -j
```
`CMAKE_POLICY_VERSION_MINIMUM` is only needed with CMake 4 or newer (the pinned dependencies predate it).

### Windows
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```
Install OpenAL if it is missing.

The build produces `r-type_server` and `r-type_client` in the repository root, and the ECS shared library in `build/` (`libecs.so`, `libecs.dylib` or `ecs.dll`).

---

## **Usage**

Instructions on how to run or use the project:

### Linux / macOS:
#### Server:
```bash
./r-type_server -p <port>
```
##### Options:

- **`-p <port>`**
  Specify the port number to listen on.

#### Client:
```bash
./r-type_client -p <port> [-h <ip>] [-u <username>]
```
##### Options:
- **`-p <port>`**
  Specify the port number to connect to (required).

- **`-u <username>`**
  Specify the username (optional, 1-12 characters). Defaults to `RTyper` if not provided.

- **`-h <ip>`**
  Specify the server IP address (optional). Defaults to `127.0.0.1` if not provided.

### Windows:
#### Server:
```bash
.\r-type_server.exe -p <port>
```
##### Options:

- **`-p <port>`**
  Specify the port number to listen on.

#### Client:
```bash
.\r-type_client.exe -p <port> [-h <ip>] [-u <username>]
```
##### Options:
- **`-p <port>`**
  Specify the port number to connect to (required).

- **`-u <username>`**
  Specify the username (optional, 1-12 characters). Defaults to `RTyper` if not provided.

- **`-h <ip>`**
  Specify the server IP address (optional). Defaults to `127.0.0.1` if not provided.

> **Run both programs from the repository root.** They load `assets/`, `config/` and `build/libecs.*` relative to the current directory.

---

## **License**

This project is licensed under the Apache License 2.0. See the [LICENSE](./LICENSE) file for the full license text.

The license covers the source code only. The sprites and sound effects in `assets/` include material from the original R-Type game and other third-party sources; they are used here for educational purposes and are not covered by that license.


---

## **Authors**

- **Adrien JACQ** - *ECS* ([github](https://github.com/Xylliar))
- **Alexandre MOLINA** - *Network* ([github](https://github.com/MolinaAlexandre))
- **Amitoj NAYAR** - *Scrum Master / Network* ([github](https://github.com/Chapitoj))
- **Jules REYNAUD** - *Product Owner / Game design* ([github](https://github.com/julesreyn))
- **Tom FACON** - *ECS* ([github](https://github.com/txmfac))

---

## **Contact**

Reach the authors through their GitHub profiles listed above.
