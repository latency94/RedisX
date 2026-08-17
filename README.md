# RedisX

RedisX is a Redis-inspired in-memory key-value database built from scratch in **C++17**.

The project focuses on understanding how a networked database server works internally, including TCP socket programming, client handling, command parsing, concurrent connections, thread-safe data access, and the initial implementation of the Redis Serialization Protocol (RESP).

---

## 🚀 Features

### Networking

- TCP server built using POSIX sockets
- `socket()`, `bind()`, `listen()`, `accept()`
- Client communication using `recv()` and `send()`
- `SO_REUSEADDR` support
- Basic socket error handling
- Reliable response transmission using `send_all()`

### Client Handling

- Dedicated client handler
- Multiple simultaneous clients using `std::thread`
- Buffered TCP input handling
- Handles partial TCP reads
- Graceful client disconnection handling

### Database

- In-memory key-value storage
- Implemented using `std::unordered_map`
- Thread-safe access using `std::shared_mutex`
- Concurrent read operations using `std::shared_lock`
- Exclusive write operations using `std::unique_lock`

### Supported Commands

| Command | Description |
|---|---|
| `SET key value` | Stores a key-value pair |
| `GET key` | Retrieves the value associated with a key |
| `DEL key` | Deletes a key |
| `EXISTS key` | Checks whether a key exists |
| `PING` | Server health check |

### RESP Protocol

RedisX currently contains an **initial RESP implementation** supporting:

- RESP Arrays
- RESP Bulk Strings
- Simple Strings
- Integer responses
- Error responses
- Incremental buffered request parsing

> Complete Redis CLI compatibility is still under development.

---

# 🏗️ Architecture

```text
                         Client
                           │
                           │ TCP
                           ▼
                 ┌───────────────────┐
                 │    TCP Server     │
                 │                   │
                 │ socket()          │
                 │ bind()            │
                 │ listen()          │
                 │ accept()           │
                 └─────────┬─────────┘
                           │
                           │ std::thread
                           ▼
                 ┌───────────────────┐
                 │  Client Handler   │
                 │                   │
                 │ recv()            │
                 │ send_all()        │
                 │ buffering         │
                 └─────────┬─────────┘
                           │
                           ▼
                 ┌───────────────────┐
                 │   Parser / RESP   │
                 └─────────┬─────────┘
                           │
                           ▼
                 ┌───────────────────┐
                 │ Command Executor  │
                 │                   │
                 │ SET               │
                 │ GET               │
                 │ DEL               │
                 │ EXISTS            │
                 │ PING              │
                 └─────────┬─────────┘
                           │
                           ▼
                 ┌───────────────────┐
                 │     Database      │
                 │                   │
                 │ unordered_map     │
                 │ shared_mutex      │
                 └───────────────────┘


---

🔄 Request Flow

A typical client request follows this flow:

Client
  │
  │ TCP Request
  ▼
TCP Server
  │
  │ accept()
  ▼
Client Handler
  │
  │ recv()
  ▼
Input Buffer
  │
  ▼
Parser / RESP
  │
  ▼
Command Executor
  │
  ▼
Thread-Safe Database
  │
  ▼
Response
  │
  │ send_all()
  ▼
Client

---

🧵 Concurrency Model

RedisX creates a separate std::thread for each connected client.

                    RedisX Server
                         │
          ┌──────────────┼──────────────┐
          │              │              │
          ▼              ▼              ▼
      Client 1        Client 2       Client 3
      Thread 1        Thread 2       Thread 3
          │              │              │
          └──────────────┼──────────────┘
                         ▼
                  Shared Database
                         │
                  std::shared_mutex

The database uses:

std::shared_lock for read operations
std::unique_lock for write operations

---

📁 Project Structure
RedisX/
│
├── include/
│   ├── client.hpp
│   ├── command.hpp
│   ├── database.hpp
│   ├── parser.hpp
│   ├── resp.hpp
│   └── server.hpp
│
├── src/
│   ├── client.cpp
│   ├── command.cpp
│   ├── database.cpp
│   ├── main.cpp
│   ├── parser.cpp
│   ├── resp.cpp
│   └── server.cpp
│
├── tests/
├── docs/
│
├── CMakeLists.txt
├── README.md
└── .gitignore



Component Responsibilities
Component	Responsibility
server.*	TCP server and client connections
client.*	Client communication and request handling
parser.*	Parses command input into tokens
command.*	Executes supported commands
database.*	Thread-safe in-memory storage
resp.*	RESP parsing and response encoding
main.cpp	Application entry point


---

⚙️ Requirements

Make sure the following are installed:

C++17 compatible compiler
CMake
Make
Linux environment

Check your installation:

g++ --version
cmake --version
make --version


---


🛠️ Tech Stack
Language: C++17
Operating System: Linux
Networking: POSIX TCP Sockets
Build System: CMake
Compiler: GCC / G++
Concurrency: std::thread
Synchronization: std::shared_mutex
Containers: STL std::unordered_map
Protocol: RESP

---

⚙️ Requirements

Make sure the following are installed:

C++17 compatible compiler
CMake
Make
Linux environment

Check your installation:

g++ --version
cmake --version
make --version

---


🔨 Build

Clone the repository:

git clone https://github.com/latency94/RedisX.git
cd RedisX

Create a build directory:

mkdir build
cd build

Configure the project:

cmake ..

Build:

make

----

▶️ Run

Start the RedisX server:

./RedisX

The server listens on port:

6379

Expected output:

=============================
RedisX Server Started
Listening on port 6379
=============================
---

Usage

RedisX can be tested using a TCP client such as netcat.

Connect to the server:

nc localhost 6379
PING
PING

Response:

PONG
SET
SET name Gourav

Response:

OK
GET
GET name

Response:

Gourav
EXISTS
EXISTS name

Response:

1
DEL
DEL name

Response:

1

After deletion:

GET name

Response:

(nil)

----


📈 Current Status
Implemented
 TCP server
 Client handler
 Command parser
 Command executor
 Thread-safe database
 SET
 GET
 DEL
 EXISTS
 PING
 Multi-client support using std::thread
 Buffered TCP input handling
 Reliable response sending
 Initial RESP implementation
Planned
 Complete RESP integration
 EXPIRE / TTL
 Background key expiration
 Snapshot persistence
 LRU eviction
 Thread pool
 Benchmarking
 Automated tests
 Improved protocol compatibility

---


🔮 Future Direction

The project will gradually evolve toward a more production-oriented architecture.

Current
   │
   ├── TCP Server
   ├── Thread-per-client
   ├── Thread-safe Database
   └── Initial RESP
          │
          ▼
Future
   │
   ├── Complete RESP
   ├── TTL / Expiration
   ├── Persistence
   ├── LRU Eviction
   ├── Thread Pool
   ├── Benchmarking
   └── Automated Testing


👨‍💻 Author

Gourav Meena

GitHub:
https://github.com/latency94
