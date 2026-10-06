# C++ Networking Examples

Two small C++ networking projects:

1. **WebSocket chat server** built with the [Crow](https://crowcpp.org/) web framework. It broadcasts messages to every connected browser and serves a few HTML pages.
2. **Raw TCP client/server** using POSIX sockets. The client sends one message, the server replies once, and both exit.

## Files

| File | Description |
|------|-------------|
| `crow_server.cpp` | Crow app: WebSocket broadcast endpoint plus HTTP routes (port 9095) |
| `server.cpp` | Single-connection TCP server (port 8080) |
| `client.cpp` | TCP client that connects to `127.0.0.1:8080` |
| `common.h` | Shared constants (`PORT`, `BUFFER_SIZE`) and helpers (`checkargs`, `pserror`) |

> Rename the files above to match whatever you saved them as.

## Requirements

- A C++11 (or newer) compiler: `g++` or `clang++`
- Linux or macOS (the code uses POSIX socket headers)
- For the Crow server only: `crow.h` and its dependency Asio (Boost.Asio or standalone Asio). See the [Crow docs](https://crowcpp.org/master/getting_started/setup/) for installation.

## 1. WebSocket chat server (`crow_server.cpp`)

### What it does

- Listens on **port 9095** (defined as `IPADDRESS`, though it is a port, not an address).
- Runs multithreaded; a mutex guards the shared connection map.
- On connect, assigns each client a default name of `User_<ip address>`.
- On message, broadcasts the text to every connected client as `username: message`. Binary messages are forwarded unchanged, without a username.
- On disconnect, removes the client from the map.

### Routes

| Route | Type | Description |
|-------|------|-------------|
| `/ws` | WebSocket | Chat endpoint |
| `/` | HTTP GET | Renders `ws.html` with the server's hostname as `servername` |
| `/htmx` | HTTP GET | Renders `htmx.html` |
| `/send-message` | HTTP GET | Returns a small static HTML page ("hello from server") |

### Templates

Crow's Mustache loader looks in a `templates/` folder relative to the directory the binary runs from. Create:

```
templates/
├── ws.html      # chat page; can use {{servername}}
└── htmx.html
```

### Build and run

```bash
g++ -std=c++17 crow_server.cpp -o crow_server -lpthread
./crow_server
```

Then open `http://localhost:9095/` in a browser. If your Crow install needs extra flags (for example Asio include paths or `-DASIO_STANDALONE`), add them to the command.

### Quick test with a WebSocket client

```bash
# npm install -g wscat
wscat -c ws://localhost:9095/ws
```

Open two terminals and type in one; the message appears in both.

## 2. TCP client and server (`server.cpp`, `client.cpp`, `common.h`)

### What it does

- **Server:** creates a TCP socket on port 8080, binds to all interfaces, accepts **one** client, prints that client's message, replies with `Hi I am a message from the server!`, then exits.
- **Client:** connects to `127.0.0.1:8080`, sends `CLIENT: I am sending a message!`, prints the server's reply, then exits.
- Both programs reject command-line arguments via `checkargs`.

### Build and run

```bash
g++ -std=c++11 server.cpp -o server
g++ -std=c++11 client.cpp -o client
```

Start the server first, then the client in a second terminal:

```bash
./server
./client
```

Expected output:

```
# server terminal
CLIENT: I am sending a message!
SERVER: I just sent a message to the client!

# client terminal
CLIENT: Message has been sent!
CLIENT: printing the buffer...
Hi I am a message from the server!
```

To use a different port, change `PORT` in `common.h` and recompile both programs.

## Known issues and limitations

**Crow server**
- **The listing is incomplete.** `main()` has no closing `}` after `.run();`. The stray `}` at the end of the pasted code is probably that brace.
- **IP lookup likely doesn't compile.** `inet_ntop(..., &conn.get_ip_address(), ...)` takes the address of a temporary and is not a documented Crow call. Crow's websocket connection exposes the remote IP as a string (`conn.get_remote_ip()`); use that directly for the username.
- **Missing includes.** Add `<unordered_map>` and `<string>`, plus `<unistd.h>` (for `gethostname`) and `<arpa/inet.h>` if you keep `inet_ntop`.
- **Usernames are not unique or changeable.** Users behind the same IP share a name, and there is no way to set one.
- **The mutex is held while sending to every client.** One slow client can stall all others.
- **Stray `;;`** after `std::mutex mtx`.

**TCP client/server**
- **Single use.** The server handles one client, one message, then exits. Wrap `accept`/`read`/`send` in a loop (and add threads or `select`/`poll`) for multiple clients.
- **Socket creation check.** `server.cpp` tests `== 0` after `socket()`; it should test `< 0`, since `-1` signals failure.
- **Buffer termination.** Both programs `read()` up to `BUFFER_SIZE` bytes into a 1024-byte buffer. A full-size message would leave no room for the null terminator. Read `BUFFER_SIZE - 1` bytes.
- **One read, one send.** TCP is a byte stream; larger messages may arrive in several pieces. Real protocols need message framing.
- **Client leaks the socket on early error.** The error paths `return -1` without calling `close()`.
- **`common.h` defines functions in the header.** That is fine while each program is a single `.cpp` file, but linking two files that include it would cause duplicate definitions. Mark them `inline`. Also add `<cstdlib>` for `exit`.
- **No encryption.** Traffic is plain text, so neither project is suitable for sensitive data without TLS.

## License

Add a license of your choice here.
