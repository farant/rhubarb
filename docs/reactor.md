# Reactor - Event Loop Library

## Overview

The reactor library implements a single-threaded event loop using `poll()` for I/O multiplexing. It follows the **reactor pattern**: instead of blocking on a single I/O operation, the reactor monitors multiple file descriptors simultaneously and dispatches callbacks when events occur.

This enables handling many concurrent connections (up to 1000 FDs) in a single thread without requiring multi-threading, making it the foundation for event-driven network servers and asynchronous I/O in rhubarb.

The reactor also supports **timers** (up to 128), allowing scheduled callbacks that integrate seamlessly with the I/O event loop.

## Key Concepts

### The Reactor Pattern

The reactor pattern inverts the typical blocking I/O model. Instead of:

```
read(fd, buf, n);  /* blocks until data arrives */
```

You register interest and a callback:

```c
reactor_adicere(reactor, fd, REACTOR_LEGERE, on_data, user_data);
reactor_currere(reactor);  /* dispatches callbacks as events arrive */
```

The event loop calls `poll()` internally, which blocks until one or more registered FDs have events ready, then dispatches the appropriate callbacks.

### Single-Threaded

The reactor runs entirely in one thread. All callbacks execute sequentially -- there is no concurrency within the event loop. This eliminates the need for locks or synchronization, but means callbacks should not block for long periods (otherwise they stall the entire loop).

### Timer Integration

Timers are integrated into the poll cycle. The reactor calculates the minimum timeout across all pending timers and uses that as the `poll()` timeout, ensuring timers fire with reasonable precision even while waiting for I/O events.

## Data Structures

### `Reactor` (opaque)

The main event loop handle. Created with `reactor_creare()`, it holds:

- **`piscina`** -- Arena allocator used for the reactor's own memory
- **`fd_entries[REACTOR_FD_MAXIMA]`** -- Array of `ReactorFdEntry` tracking registered file descriptors, their events, callbacks, and user data
- **`poll_fds[REACTOR_FD_MAXIMA]`** -- Parallel array of `struct pollfd` used by the `poll()` system call
- **`fd_numerus`** -- Count of FD entries (including inactive slots)
- **`timers[REACTOR_TIMER_MAXIMA]`** -- Array of `ReactorTimerEntry` for scheduled callbacks
- **`timer_numerus`** -- High-water mark of timer slots used
- **`proximus_timer_id`** -- Monotonically increasing timer ID counter
- **`currens`** -- Whether the event loop is currently running
- **`sistere_petitum`** -- Flag set by `reactor_sistere()` to request loop exit

### `ReactorFdEntry` (internal)

Tracks a single registered file descriptor:

| Field       | Type              | Description                              |
|-------------|-------------------|------------------------------------------|
| `fd`        | `integer`         | The file descriptor                      |
| `eventus`   | `i32`             | Bitmask of events to monitor             |
| `callback`  | `ReactorCallback` | Function called when events occur        |
| `data`      | `vacuum*`         | User data passed to callback             |
| `activus`   | `b32`             | Whether this entry is active             |

### `ReactorTimerEntry` (internal)

Tracks a single timer:

| Field          | Type                   | Description                              |
|----------------|------------------------|------------------------------------------|
| `id`           | `ReactorTimerId`       | Unique timer identifier                  |
| `proximus_ms`  | `i64`                  | Next fire time (absolute, milliseconds)  |
| `interval_ms`  | `i32`                  | Timer interval in milliseconds           |
| `repetere`     | `b32`                  | Whether timer repeats after firing       |
| `callback`     | `ReactorTimerCallback` | Function called when timer fires         |
| `data`         | `vacuum*`              | User data passed to callback             |
| `activus`      | `b32`                  | Whether this timer is active             |

### Callback Types

```c
/* Called when FD events occur */
nomen vacuum (*ReactorCallback)(integer fd, i32 eventus, vacuum* data);

/* Called when a timer fires */
nomen vacuum (*ReactorTimerCallback)(vacuum* data);
```

### Event Bitmask Constants

| Constant            | Value   | Description                     |
|---------------------|---------|---------------------------------|
| `REACTOR_LEGERE`    | `0x01u` | FD is ready to read (POLLIN)    |
| `REACTOR_SCRIBERE`  | `0x02u` | FD is ready to write (POLLOUT)  |
| `REACTOR_ERROR`     | `0x04u` | Error condition (POLLERR)       |
| `REACTOR_CLAUSUM`   | `0x08u` | Connection closed (POLLHUP)     |

### Limits

| Constant                  | Value  | Description             |
|---------------------------|--------|-------------------------|
| `REACTOR_FD_MAXIMA`       | `M`    | 1000 FDs maximum        |
| `REACTOR_TIMER_MAXIMA`    | `CXXVIII` | 128 timers maximum   |

### `ReactorTimerId`

A `typedef` for `i32`. Timer IDs are assigned monotonically starting from 0. The sentinel value `REACTOR_TIMER_ERROR` (`0xFFFFFFFFu`) indicates failure.

## API Reference

### Lifecycle

#### `reactor_creare`

```c
Reactor* reactor_creare(Piscina* piscina);
```

Create a new reactor instance.

- **`piscina`** -- Arena allocator. The reactor struct itself is allocated from this arena.
- **Returns** -- `Reactor*` on success, `NIHIL` if `piscina` is `NIHIL` or allocation fails.

The reactor starts in a stopped state (`currens = FALSUM`) with zero FDs and zero timers.

#### `reactor_destruere`

```c
vacuum reactor_destruere(Reactor* reactor);
```

Destroy the reactor. Sets `currens` to `FALSUM` and resets counters. Safe to call with `NIHIL`.

Note: Since the reactor is arena-allocated, memory is not individually freed -- it will be reclaimed when the arena is destroyed.

---

### FD Management

#### `reactor_adicere`

```c
b32 reactor_adicere(
    Reactor*        reactor,
    integer         fd,
    i32             eventus,
    ReactorCallback callback,
    vacuum*         data);
```

Register a file descriptor with the reactor.

- **`reactor`** -- The reactor instance
- **`fd`** -- File descriptor to monitor (must be >= 0)
- **`eventus`** -- Bitmask of events: `REACTOR_LEGERE`, `REACTOR_SCRIBERE`, or both OR'd together
- **`callback`** -- Function to call when events occur (must not be `NIHIL`)
- **`data`** -- User data pointer passed to callback
- **Returns** -- `VERUM` on success, `FALSUM` on invalid args or capacity exceeded

If the FD is already registered, this **updates** the existing entry (events, callback, data) rather than adding a duplicate.

#### `reactor_modificare`

```c
b32 reactor_modificare(
    Reactor* reactor,
    integer  fd,
    i32      eventus);
```

Change which events to monitor for an already-registered FD.

- **`fd`** -- Must be a previously registered file descriptor
- **`eventus`** -- New event bitmask
- **Returns** -- `VERUM` on success, `FALSUM` if FD not found

#### `reactor_removere`

```c
b32 reactor_removere(
    Reactor* reactor,
    integer  fd);
```

Remove a file descriptor from the reactor. Marks the entry as inactive.

- **Returns** -- `VERUM` on success, `FALSUM` if FD not found

---

### Timer Management

#### `reactor_timer_adicere`

```c
ReactorTimerId reactor_timer_adicere(
    Reactor*             reactor,
    i32                  timeout_ms,
    b32                  repetere,
    ReactorTimerCallback callback,
    vacuum*              data);
```

Add a timer to the reactor.

- **`timeout_ms`** -- Time in milliseconds until the timer fires (and interval if repeating)
- **`repetere`** -- `VERUM` for a repeating timer, `FALSUM` for one-shot
- **`callback`** -- Function to call when timer fires (must not be `NIHIL`)
- **`data`** -- User data pointer passed to callback
- **Returns** -- Timer ID (>= 0) on success, `REACTOR_TIMER_ERROR` on failure

One-shot timers are automatically deactivated after firing. Repeating timers reschedule themselves relative to the current time when they fire.

#### `reactor_timer_cancellare`

```c
b32 reactor_timer_cancellare(
    Reactor*       reactor,
    ReactorTimerId id);
```

Cancel an active timer before it fires.

- **`id`** -- Timer ID returned from `reactor_timer_adicere`
- **Returns** -- `VERUM` on success, `FALSUM` if timer not found

---

### Event Loop

#### `reactor_poll`

```c
i32 reactor_poll(
    Reactor* reactor,
    i32      timeout_ms);
```

Poll once for events. This is the low-level building block of the event loop.

- **`timeout_ms`** -- Maximum time to wait in milliseconds. `-1` = wait indefinitely, `0` = non-blocking
- **Returns** -- Number of events processed (FD events + timer events)

The actual timeout used may be shorter than `timeout_ms` if a timer is due to fire sooner. After `poll()` returns, all ready FD callbacks are dispatched, then all expired timer callbacks are dispatched.

#### `reactor_currere`

```c
vacuum reactor_currere(Reactor* reactor);
```

Run the event loop continuously. Calls `reactor_poll()` in a loop with a 1000ms default timeout until `reactor_sistere()` is called.

Sets `currens` to `VERUM` while running, `FALSUM` after exit.

#### `reactor_sistere`

```c
vacuum reactor_sistere(Reactor* reactor);
```

Request the event loop to stop. The loop will exit after the current `poll()` iteration completes. Safe to call from within a callback.

#### `reactor_est_currens`

```c
b32 reactor_est_currens(Reactor* reactor);
```

Check if the reactor event loop is currently running.

- **Returns** -- `VERUM` if running, `FALSUM` otherwise

---

### Introspection

#### `reactor_numerus_fd`

```c
i32 reactor_numerus_fd(Reactor* reactor);
```

Get the count of currently active (registered) file descriptors.

#### `reactor_numerus_timer`

```c
i32 reactor_numerus_timer(Reactor* reactor);
```

Get the count of currently active timers.

---

## Usage Examples

### Basic FD Monitoring with Poll

From the test suite -- write to a pipe and poll for readability:

```c
Piscina* piscina = piscina_generare_dynamicum("exemplum", CXXVIII * M);
Reactor* reactor = reactor_creare(piscina);

integer pipe_fds[II];
pipe(pipe_fds);

/* Register read end for LEGERE events */
reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE, on_data, NIHIL);

/* Write data to trigger readability */
write(pipe_fds[I], "Salve", V);

/* Poll once with 100ms timeout */
i32 events = reactor_poll(reactor, C);
/* on_data callback was invoked with REACTOR_LEGERE */
```

### One-Shot Timer

```c
Reactor* reactor = reactor_creare(piscina);

/* Fire callback once after 50ms */
ReactorTimerId id = reactor_timer_adicere(reactor, L, FALSUM, on_timeout, NIHIL);

usleep(LX * M);  /* Wait 60ms */
reactor_poll(reactor, C);
/* on_timeout was called */
```

### Repeating Timer

```c
Reactor* reactor = reactor_creare(piscina);

i32 counter = 0;
reactor_timer_adicere(reactor, XX, VERUM, on_tick, &counter);

/* Poll multiple times */
per (i = 0; i < V; i++)
{
    usleep(XXV * M);  /* 25ms between polls */
    reactor_poll(reactor, L);
}
/* counter >= 3 -- timer fired at least 3 times */
```

### Self-Stopping Event Loop

A common pattern is to use a timer that calls `reactor_sistere()` from within its callback:

```c
interior vacuum
callback_sistere(vacuum* data)
{
    Reactor* reactor = (Reactor*)data;
    reactor_sistere(reactor);
}

/* ... */
Reactor* reactor = reactor_creare(piscina);

/* Stop the loop after 50ms */
reactor_timer_adicere(reactor, L, FALSUM, callback_sistere, reactor);

reactor_currere(reactor);
/* Returns after ~50ms when the timer fires and calls sistere */
```

### Combined FD + Timer

```c
Reactor* reactor = reactor_creare(piscina);

integer pipe_fds[II];
pipe(pipe_fds);

/* Monitor pipe for reads */
reactor_adicere(reactor, pipe_fds[0], REACTOR_LEGERE, on_data, NIHIL);

/* Write data so the FD callback fires */
write(pipe_fds[I], "Probatio", VIII);

/* Stop after 100ms */
reactor_timer_adicere(reactor, C, FALSUM, callback_sistere, reactor);

reactor_currere(reactor);
/* Both on_data and callback_sistere were called */
```

## Relationship to Other Libraries

### tcp.h (TCP Networking)

The TCP library's server API (`TcpServus`) is designed to work with the reactor. `tcp_servus_obtinere_fd()` returns a file descriptor that can be registered with the reactor for accepting new connections:

```c
TcpServus* servus = /* ... */;
integer fd = tcp_servus_obtinere_fd(servus);
reactor_adicere(reactor, fd, REACTOR_LEGERE, on_accept, servus);
```

Similarly, `tcp_obtinere_fd()` on client connections returns FDs suitable for reactor monitoring. The TCP library provides non-blocking mode via `tcp_ponere_non_blocans()` which is essential for use with the reactor.

### actor.h (Actor System)

The actor system provides message-passing between entities via mailboxes (`Capsa`). The reactor can drive an actor system's event loop, dispatching messages on timer ticks or in response to I/O events. The reactor provides the scheduling/timing infrastructure while actors provide the communication model.

### selectio.h (CSS Selectors)

The selectio library (CSS selector parsing and matching for STML documents) is unrelated to the reactor -- the naming similarity to POSIX `select()` is coincidental. Selectio operates on DOM-like trees, not I/O.

### http.h (HTTP Client/Server)

The HTTP library provides request parsing (`HttpParser`) that works incrementally, making it suitable for non-blocking I/O driven by the reactor. An HTTP server built on rhubarb would typically use the reactor to manage client connections, feeding received bytes into the incremental HTTP parser.

## Gotchas and Important Notes

1. **Arena allocation**: The reactor is allocated from a `Piscina`. It is not individually freed by `reactor_destruere()` -- memory is reclaimed when the arena is destroyed. `reactor_destruere()` only resets the reactor's state.

2. **Callback duration**: Callbacks should return quickly. A long-running callback blocks the entire event loop since it is single-threaded. For expensive work, consider breaking it into smaller chunks across multiple poll iterations.

3. **FD removal during callbacks**: Removing an FD from within its own callback is safe because `reactor_removere()` only marks the entry as inactive -- it does not shift the array. The poll FD array is rebuilt on the next `_rebuilere_poll_fds()` call.

4. **Timer precision**: Timer resolution depends on the poll timeout and how long callbacks take. A timer set for 20ms might not fire for 1020ms if a previous `poll()` call blocks for 1000ms with no I/O events.

5. **`REACTOR_FD_MAXIMA` is 1000** (`M` in Latin numerals). `REACTOR_TIMER_MAXIMA` is 128 (`CXXVIII`). Exceeding these limits causes `reactor_adicere` / `reactor_timer_adicere` to return failure.

6. **Inactive slot reuse for timers**: When a timer fires (one-shot) or is cancelled, its slot is marked inactive and can be reused by `reactor_timer_adicere`. FD entries, however, are not reused -- they are appended to the end of the array.

7. **`REACTOR_NON_INVENIT`** (`0xFFFFFFFFu`) is used internally as a sentinel for "not found" and is also the error value returned by `reactor_timer_adicere`. This is the same value as `REACTOR_TIMER_ERROR`.

8. **Time source**: Uses `gettimeofday()` for millisecond timestamps. This is wall-clock time and can be affected by system clock changes (NTP adjustments, etc.). For a monotonic clock, `clock_gettime(CLOCK_MONOTONIC, ...)` would be preferable but is not available in strict C89.

9. **`reactor_currere` default timeout**: The continuous event loop uses a 1000ms (`M`) poll timeout. This means if there are no timers and no I/O, each loop iteration takes up to 1 second before checking `sistere_petitum`.
