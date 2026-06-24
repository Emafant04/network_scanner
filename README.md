# Network Scanner in C — Educational Project

## Goal

Incremental development of a network scanner in C, built for learning purposes with a focus on cybersecurity. The project covers low-level network programming on Linux using POSIX APIs, starting from a TCP port scanner and progressively moving toward more advanced techniques such as ICMP ping sweep and SYN scan.

The long-term goal is to gradually replace utility functions (address conversion, byte order, checksum) with custom implementations, to understand what happens beneath the APIs. Socket syscalls (`socket()`, `connect()`, `close()`) remain POSIX since they cannot be bypassed without writing assembly — they are the mandatory contact point with the kernel.

---

## Project Structure

```
.
├── .gitignore
├── LICENSE
├── Makefile
├── README.md
├── .README.md.kate-swp
├── src
│   ├── netutils.c
│   ├── netutils.h
│   ├── port_scanner.c
│   ├── scan.c
│   └── scan.h
└── tests
    └── test_netutils.c
```

---

## Build

```bash
make
```

Compiles `src/port_scanner.c` and places the binary in `bin/port_scanner`.

```bash
make test
```

Compiles and runs the test suite (`tests/test_netutils.c`), comparing custom implementations against glibc originals.

---

## Current State

### Phase 2 — — scan.c (in progress)

Extend the program to scan a range of ports on a single host, introducing timeout handling via `setsockopt()` or `select()`.
-rearrange the structure of the program for a more scalable implementation

- `scan_port()` — done(to be tested)
- `parse_range()` — done(to be tested)


---

## Roadmap

### Phase 1 — TCP Port Scanner 

Minimal program that attempts a TCP connection to a single host and port, returning whether the port is open or closed.

**Concepts covered:**
- Difference between the C standard library and POSIX API
- TCP socket lifecycle: `socket()` → `connect()` → `close()`
- `sockaddr_in` structure and address preparation
- Network byte order and conversion functions (`htons`, `inet_pton`)
- Error handling via `errno`

**Usage:**
```bash
./bin/port_scanner <ip> <port>
./bin/port_scanner 127.0.0.1 22
```

**Local testing:**
```bash
# Terminal 1 — open a dummy server
nc -l -p 1234

# Terminal 2 — run the scanner
./bin/port_scanner 127.0.0.1 1234   # open
./bin/port_scanner 127.0.0.1 9999   # closed
```

#### — netutils.c (done)

Custom reimplementations of byte-order conversion functions, to replace `htons`/`htonl` and `inet_pton` from glibc.

- `my_htons()` — done, verified against `htons` (manual check + test suite)
- `my_htonl()` — done, verified against `htonl` (manual check + test suite)
- `my_inet_pton()` — done, verified against `inet_pton` (manual check + test suite)
- `my_memset()` — done, verified against `memset` (manual check + test suite) - memcmp
- Test suite (`tests/test_netutils.c`) — done, all tests pass (`make test`)


### Phase 2 — Port range scanning(in progress)
Extend the program to scan a range of ports on a single host, introducing timeout handling via `setsockopt()` or `select()`.

**Concepts covered:**
- "Drop vs Reject" as a response from a firewall
- Refactoring: Scan logic extracted in scan_port() in scan.c/scan.h
- Defined port states: PORT_OPEN, PORT_CLOSED, PORT_FILTERED, PORT_ERROR
- Non-blocking socket with fcntl() + O_NONBLOCK 
- Timeout handling with select() 
- Port range parsing 

**Usage:**
```bash
./bin/port_scanner <ip> <port>
./bin/port_scanner 127.0.0.1 1-1000
```

**Local testing:**
```bash
# Terminal 1 — open a dummy server
nc -l -p 999

# Terminal 2 — run the scanner
./bin/port_scanner 127.0.0.1 1-1000   # port 999 open
```


### Phase 3 — Threading
Make the scan parallel to reduce execution time, introducing `pthread` and concurrency management.

### Phase 4 — Host discovery (ping sweep)
Discover which hosts are alive on the subnet using raw sockets and manually crafted ICMP packets. Requires root privileges.

**Custom implementation planned:** ICMP checksum, manual ICMP header construction.

### Phase 5 — Banner grabbing
Connect to open ports and read the service response to identify it, without implementing full application-layer protocols.

### Phase 6 — Automatic subnet discovery
Automatically retrieve the IP address and subnet mask of the local network interface using `getifaddrs()`.

**Custom implementation planned:** manual parsing of `/proc/net/fib_trie` as an alternative to `getifaddrs`.

### Phase 7 — SYN scan (advanced)
Half-open scan: manually craft a TCP packet using raw sockets, send only the SYN and interpret the response without completing the three-way handshake. Stealthier than TCP connect scan, leaves fewer traces in target logs.

**Custom implementation planned:** manual TCP header construction, TCP checksum.

---

## Functions to Reimplement

| Function | Phase | What it teaches |
|---|---|---|
| `htons()` / `htonl()` | 2 | Byte order, binary representation of numbers |
| `inet_pton()` | 2 | IP string parsing, binary address representation |
| ICMP checksum | 4 | Internet checksum algorithm (RFC 1071) |
| ICMP header construction | 4 | ICMP packet structure |
| TCP header construction | 7 | TCP segment structure, flags, sequence numbers |
| TCP checksum | 7 | TCP pseudo-header in checksum calculation |

---

## Background Concepts

| Concept | Where it applies |
|---|---|
| TCP/IP stack and encapsulation | Why ICMP requires raw sockets |
| Three-way handshake | Logic behind TCP connect scan |
| Network byte order (big-endian) | `htons()`, `htonl()` in address preparation |
| Unix file descriptors | Sockets are file descriptors |
| Syscalls and userspace/kernelspace | Difference between POSIX API and C standard library |
| ICMP echo request/reply | Ping sweep in phase 4 |
| TCP header and checksum | SYN scan in phase 7 |

---

## Environment

- **OS:** Linux
- **Compiler:** GCC
- **Language:** C (C99 or later)
- **Dependencies:** none, glibc only

Testing phases 4 and above requires at least two virtual machines on the same network (host-only or bridged).

---

## Disclaimer

This project is developed for educational purposes on owned networks or lab environments. Scanning networks or hosts without authorization is illegal.
