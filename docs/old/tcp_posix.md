# tcp_posix - Bibliotheca TCP (Connexiones Rete)

## Overview

The TCP library provides a platform-agnostic API for TCP networking in C89. The public interface is declared in `include/tcp.h`, while the POSIX implementation lives in `lib/tcp_posix.c` (targeting macOS and Linux). The library supports both **client** connections (connecting to remote hosts) and **server** sockets (listening for incoming connections), with optional non-blocking I/O for integration with event loops.

All memory allocation is done through the `Piscina` arena allocator -- no malloc/free. Socket options (TCP_NODELAY, SO_KEEPALIVE, timeouts) are configurable via option structs with sensible defaults.

## Key Concepts

### Platform Abstraction

The header `tcp.h` defines the public API using opaque types (`TcpConnexio`, `TcpServus`). The internal struct definitions live only in `tcp_posix.c`, so a future Windows implementation could provide the same API with a different backing file (e.g., `tcp_win32.c`).

### Client vs. Server

- **Client** (`TcpConnexio`): Created via `tcp_connectere()`. Performs DNS resolution, iterates candidate addresses (IPv4/IPv6), and establishes a connection. Used for outbound TCP connections.
- **Server** (`TcpServus`): Created via `tcp_servus_creare()`. Binds to a port, listens for connections, and accepts incoming clients. Each accepted client becomes a `TcpConnexio`.

### Non-Blocking I/O

Server sockets default to non-blocking mode (`O_NONBLOCK`). Client connections can be switched between blocking and non-blocking via `tcp_ponere_non_blocans()`. Non-blocking mode allows integration with the `Reactor` event loop (via `tcp_obtinere_fd()` / `tcp_servus_obtinere_fd()`).

### Error Handling

All fallible operations return result structs (`TcpResultus`, `TcpServusResultus`) containing a `successus` flag, the result object, an error code (`TcpError` enum), and a human-readable `error_descriptio` (allocated as a `chorda` in the arena).

## Data Structures

### `TcpConnexio` (opaque)

Internal structure for a TCP connection. Fields (defined in `tcp_posix.c`):

| Field     | Type      | Description                          |
|-----------|-----------|--------------------------------------|
| `piscina` | `Piscina*`| Arena that owns this connection      |
| `fd`      | `integer` | POSIX socket file descriptor         |
| `clausa`  | `b32`     | Whether the connection has been closed|

### `TcpServus` (opaque)

Internal structure for a TCP server socket. Fields (defined in `tcp_posix.c`):

| Field         | Type         | Description                             |
|---------------|--------------|-----------------------------------------|
| `piscina`     | `Piscina*`   | Arena that owns this server             |
| `fd`          | `integer`    | Listening socket file descriptor        |
| `auscultans`  | `b32`        | Whether `listen()` has been called      |
| `non_blocans` | `b32`        | Whether socket is in non-blocking mode  |
| `ligatum`     | `TcpAddress` | Bound address and port                  |

### `TcpError` (enum)

| Value                | Description                        |
|----------------------|------------------------------------|
| `TCP_OK`             | No error                           |
| `TCP_ERROR_DNS`      | DNS resolution failed              |
| `TCP_ERROR_CONNEXIO` | Connection failed                  |
| `TCP_ERROR_TIMEOUT`  | Operation timed out                |
| `TCP_ERROR_CLAUSUM`  | Connection closed by remote host   |
| `TCP_ERROR_IO`       | General I/O error                  |

### `TcpResultus`

Result struct for client connection operations.

| Field               | Type           | Description                              |
|---------------------|----------------|------------------------------------------|
| `successus`         | `b32`          | `VERUM` if operation succeeded           |
| `connexio`          | `TcpConnexio*` | Connection handle (valid if `successus`) |
| `error`             | `TcpError`     | Error code (if failed)                   |
| `error_descriptio`  | `chorda`       | Human-readable error message             |

### `TcpServusResultus`

Result struct for server creation operations.

| Field               | Type         | Description                           |
|---------------------|--------------|---------------------------------------|
| `successus`         | `b32`        | `VERUM` if operation succeeded        |
| `servus`            | `TcpServus*` | Server handle (valid if `successus`)  |
| `error`             | `TcpError`   | Error code (if failed)                |
| `error_descriptio`  | `chorda`     | Human-readable error message          |

### `TcpOptiones`

Options for client connections.

| Field        | Type  | Default          | Description                              |
|--------------|-------|------------------|------------------------------------------|
| `timeout_ms` | `i32` | 30000 (30s)      | Send/receive timeout in milliseconds     |
| `nodelay`    | `b32` | `VERUM`          | `TCP_NODELAY` -- disable Nagle's algorithm|
| `keepalive`  | `b32` | `FALSUM`         | `SO_KEEPALIVE`                           |

### `TcpServusOptiones`

Options for server sockets.

| Field              | Type  | Default   | Description                                  |
|--------------------|-------|-----------|----------------------------------------------|
| `tergum`           | `i32` | 128       | `listen()` backlog size                      |
| `reuti_inscriptio` | `b32` | `VERUM`   | `SO_REUSEADDR` -- allow port reuse           |
| `non_blocans`      | `b32` | `VERUM`   | `O_NONBLOCK` on the listening socket         |

### `TcpAddress`

Address information struct.

| Field    | Type            | Description              |
|----------|-----------------|--------------------------|
| `hospes` | `character[256]`| Hostname or IP string    |
| `portus` | `i32`           | Port number              |

## API Reference

### Client -- Connection

#### `tcp_optiones_default()`

```c
TcpOptiones tcp_optiones_default(vacuum);
```

Returns a `TcpOptiones` struct with default values: 30s timeout, TCP_NODELAY enabled, keepalive disabled.

---

#### `tcp_connectere()`

```c
TcpResultus tcp_connectere(
    constans character* hospes,
    i32                 portus,
    Piscina*            piscina);
```

Connect to a remote host using default options. Performs DNS resolution via `getaddrinfo()`, tries all returned addresses (IPv4 and IPv6), and returns the first successful connection.

- **hospes**: DNS hostname or IP address string.
- **portus**: Port number (e.g., `LXXX` for 80).
- **piscina**: Arena for allocating the `TcpConnexio`.
- **Returns**: `TcpResultus` with `successus == VERUM` on success.

---

#### `tcp_connectere_cum_optionibus()`

```c
TcpResultus tcp_connectere_cum_optionibus(
    constans character*   hospes,
    i32                   portus,
    constans TcpOptiones* optiones,
    Piscina*              piscina);
```

Same as `tcp_connectere()` but with explicit options (timeout, nodelay, keepalive).

---

### Client -- I/O

#### `tcp_mittere()`

```c
i32 tcp_mittere(
    TcpConnexio*   connexio,
    constans i8*   data,
    i32            mensura);
```

Send data on a connection. May send fewer bytes than requested (partial send).

- **Returns**: Number of bytes sent, `0` if socket not ready (EAGAIN), `-1` on error.

---

#### `tcp_mittere_omnia()`

```c
b32 tcp_mittere_omnia(
    TcpConnexio*   connexio,
    constans i8*   data,
    i32            mensura);
```

Send all data, blocking until everything is sent. Retries partial sends with a 1ms sleep on EAGAIN.

- **Returns**: `VERUM` if all data sent, `FALSUM` on error.

---

#### `tcp_recipere()`

```c
i32 tcp_recipere(
    TcpConnexio* connexio,
    i8*          buffer,
    i32          capacitas);
```

Receive data from a connection.

- **Returns**: Number of bytes received, `0` if connection closed by remote (also sets `clausa = VERUM`), `-1` on error.

---

#### `tcp_claudere()`

```c
vacuum tcp_claudere(TcpConnexio* connexio);
```

Close a connection. Safe to call with `NIHIL`. Calls `close()` on the file descriptor and marks the connection as closed.

---

### Client -- Utility

#### `tcp_obtinere_fd()`

```c
i32 tcp_obtinere_fd(TcpConnexio* connexio);
```

Get the underlying file descriptor. Useful for registering with a `Reactor` or using `poll()`/`select()`.

- **Returns**: File descriptor, or `-1` if `NIHIL`.

---

#### `tcp_est_valida()`

```c
b32 tcp_est_valida(TcpConnexio* connexio);
```

Check if a connection is still valid (not null, not closed, fd >= 0).

---

#### `tcp_error_descriptio()`

```c
constans character* tcp_error_descriptio(TcpError error);
```

Get a human-readable Latin description for a `TcpError` code.

---

#### `tcp_ponere_non_blocans()`

```c
TcpError tcp_ponere_non_blocans(
    TcpConnexio* connexio,
    b32          non_blocans);
```

Switch a connection between blocking and non-blocking mode using `fcntl()`.

---

#### `tcp_obtinere_peer_address()`

```c
TcpAddress tcp_obtinere_peer_address(TcpConnexio* connexio);
```

Get the remote peer's address and port via `getpeername()`.

---

### Server -- Creation

#### `tcp_servus_optiones_default()`

```c
TcpServusOptiones tcp_servus_optiones_default(vacuum);
```

Returns default server options: backlog 128, SO_REUSEADDR enabled, non-blocking enabled.

---

#### `tcp_servus_creare()`

```c
TcpServusResultus tcp_servus_creare(
    i32      portus,
    Piscina* piscina);
```

Create a server socket bound to a port with default options. Pass `0` for auto-assigned port.

---

#### `tcp_servus_creare_cum_optionibus()`

```c
TcpServusResultus tcp_servus_creare_cum_optionibus(
    constans character*         hospes,
    i32                         portus,
    constans TcpServusOptiones* optiones,
    Piscina*                    piscina);
```

Create a server socket with explicit host binding and options.

- **hospes**: Interface to bind to (`NIHIL` = all interfaces / `INADDR_ANY`).
- **portus**: Port number (`0` = auto-select).

---

### Server -- Operations

#### `tcp_servus_auscultare()`

```c
TcpError tcp_servus_auscultare(
    TcpServus* servus,
    i32        tergum);
```

Start listening for incoming connections. Calls POSIX `listen()`. If `tergum <= 0`, defaults to 128. Idempotent -- calling again on an already-listening server returns `TCP_OK`.

---

#### `tcp_servus_accipere()`

```c
TcpResultus tcp_servus_accipere(
    TcpServus* servus,
    Piscina*   piscina);
```

Accept an incoming connection. Returns a `TcpResultus` with a new `TcpConnexio` for the accepted client. In non-blocking mode, returns an error immediately if no connections are pending. Accepted connections inherit the server's non-blocking mode.

---

#### `tcp_servus_obtinere_fd()`

```c
integer tcp_servus_obtinere_fd(TcpServus* servus);
```

Get the server's listening file descriptor. Used to register with a `Reactor` for event-driven accept.

---

#### `tcp_servus_obtinere_portum()`

```c
i32 tcp_servus_obtinere_portum(TcpServus* servus);
```

Get the actual bound port. Especially useful when port `0` was passed (OS auto-assigns).

---

#### `tcp_servus_obtinere_address()`

```c
TcpAddress tcp_servus_obtinere_address(TcpServus* servus);
```

Get the full bound address (host and port) as a `TcpAddress`.

---

#### `tcp_servus_claudere()`

```c
vacuum tcp_servus_claudere(TcpServus* servus);
```

Close the server socket. Safe to call with `NIHIL`.

---

## Usage Examples

### Basic Client Connection (from `probatio_tcp.c`)

```c
Piscina* piscina = piscina_generare_dynamicum("tcp_demo", LXIV * M);

/* Connect */
TcpResultus res = tcp_connectere("httpbin.org", LXXX, piscina);
si (res.successus) {
    /* Send HTTP request */
    constans character* petitio =
        "GET /get HTTP/1.1\r\n"
        "Host: httpbin.org\r\n"
        "Connection: close\r\n"
        "\r\n";
    tcp_mittere_omnia(res.connexio, (constans i8*)petitio, (i32)strlen(petitio));

    /* Receive response */
    i8 buffer[MMMMXCVI];
    i32 n = tcp_recipere(res.connexio, buffer, MMMMXCVI - I);

    /* Close */
    tcp_claudere(res.connexio);
}
```

### Server with Client-Server Communication (from `probatio_tcp_servus.c`)

```c
Piscina* piscina = piscina_generare_dynamicum("tcp_servus_demo", CXXVIII * M);

/* Create server on auto-assigned port, blocking mode */
TcpServusOptiones opt = tcp_servus_optiones_default();
opt.non_blocans = FALSUM;

TcpServusResultus servus_res = tcp_servus_creare_cum_optionibus(NIHIL, 0, &opt, piscina);
i32 portus = tcp_servus_obtinere_portum(servus_res.servus);

/* Listen */
tcp_servus_auscultare(servus_res.servus, V);

/* Client connects */
TcpResultus client_res = tcp_connectere("127.0.0.1", portus, piscina);

/* Server accepts */
TcpResultus accept_res = tcp_servus_accipere(servus_res.servus, piscina);

/* Bidirectional communication */
tcp_mittere(client_res.connexio, (constans i8*)"Salve Server!", XIII);

character buffer[CXXVIII];
i32 n = tcp_recipere(accept_res.connexio, (i8*)buffer, CXXVIII - I);

/* Cleanup */
tcp_claudere(client_res.connexio);
tcp_claudere(accept_res.connexio);
tcp_servus_claudere(servus_res.servus);
```

### HTTP Integration (from `probatio_tcp_servus.c`)

The server test file includes a full HTTP integration test combining TCP with the HTTP parser, router, and response serializer:

1. Create `TcpServus` and `Router`
2. Client connects and sends raw HTTP request via `tcp_mittere()`
3. Server accepts, receives raw bytes via `tcp_recipere()`
4. Parse with `http_petitio_parse()`
5. Route with `router_matching()`
6. Invoke handler, serialize response with `http_responsum_serialize()`
7. Send response back via `tcp_mittere()`

This demonstrates the TCP library as the transport layer in a full HTTP server pipeline.

## Relationship to Other Libraries

| Library        | Relationship                                                                                     |
|----------------|--------------------------------------------------------------------------------------------------|
| **piscina**    | All TCP allocations (connections, servers) use the arena allocator. No malloc/free.               |
| **chorda**     | Error descriptions are stored as `chorda` (length-prefixed, non-null-terminated strings).        |
| **tls**        | `tls.h` provides an encrypted layer on top of TCP. The TLS implementation internally uses a TCP connection for the underlying transport. The TLS API mirrors the TCP API (`tls_connectere`, `tls_mittere`, `tls_recipere`, `tls_claudere`). |
| **http**       | The HTTP library uses TCP (or TLS) connections as transport. On the server side, raw bytes received via `tcp_recipere()` are fed into `http_petitio_parse()`. On the client side, `http_exsequi()` internally creates TCP/TLS connections. |
| **reactor**    | The reactor event loop monitors file descriptors via `poll()`. Use `tcp_obtinere_fd()` or `tcp_servus_obtinere_fd()` to get FDs for registration with `reactor_adicere()`. This enables event-driven, non-blocking server architectures. |
| **actor**      | The actor message-passing system can be combined with TCP for network-aware actors, using the reactor as the glue layer between socket I/O events and actor mailboxes. |
| **router**     | URL routing for HTTP servers. Used after parsing an HTTP request received over TCP.               |

## Important Notes

- **IPv4 and IPv6**: The client connection code uses `AF_UNSPEC` with `getaddrinfo()`, so it tries both IPv4 and IPv6 addresses. The server currently uses `AF_INET` (IPv4 only).
- **No DNS for server bind**: If you pass a hostname to `tcp_servus_creare_cum_optionibus()`, it resolves it via `getaddrinfo()` to determine which interface to bind to.
- **Port 0 auto-assignment**: Passing port `0` to `tcp_servus_creare()` lets the OS assign an ephemeral port. Retrieve it with `tcp_servus_obtinere_portum()`. This is useful for tests.
- **Blocking vs. non-blocking accept**: The default server is non-blocking. If you call `tcp_servus_accipere()` with no pending connections, it returns immediately with an error. For reliable test scenarios (avoiding race conditions), create the server in blocking mode.
- **Accepted connections inherit mode**: Connections accepted by a non-blocking server are also set to non-blocking.
- **EAGAIN handling**: `tcp_mittere()` and `tcp_recipere()` return `0` (not an error) when the socket would block (`EAGAIN`/`EWOULDBLOCK`). The `tcp_mittere_omnia()` function handles this automatically with retry.
- **Connection closed detection**: When `tcp_recipere()` returns `0`, the connection's `clausa` flag is set to `VERUM`. Subsequent calls to `tcp_est_valida()` will return `FALSUM`.
- **Null safety**: All public functions handle `NIHIL` arguments gracefully -- they return error values or no-op without crashing.
- **Roman numeral constants**: The codebase uses Roman numeral macros from `latina.h` (e.g., `LXXX` = 80, `CXXVIII` = 128, `M` = 1000). These appear in port numbers, buffer sizes, and backlog values.
