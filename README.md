# Network Scanner in C — Educational Project

## Goal

Incremental development of a network scanner in C, built for learning purposes with a focus on cybersecurity. The project covers low-level network programming on Linux using POSIX APIs, starting from a TCP port scanner and progressively moving toward more advanced techniques such as ICMP ping sweep and SYN scan.

The long-term goal is to gradually replace utility functions (address conversion, byte order, checksum) with custom implementations, to understand what happens beneath the APIs. Socket syscalls (`socket()`, `connect()`, `close()`, `select()`) remain POSIX since they cannot be bypassed without writing assembly — they are the mandatory contact point with the kernel.

---

## Project Structure

```
.
├── .gitignore
├── LICENSE
├── Makefile
├── README.md
├── scripts
│   ├── benchmark.sh
│   └── test_env.sh
├── src
│   ├── netutils.c
│   ├── netutils.h
│   ├── port_scanner.c
│   ├── scan.c
│   └── scan.h
└── tests
    ├── test_netutils.c
    └── test_scan.c
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

Compiles and runs the test suites:
- `tests/test_netutils.c` — compares custom implementations against glibc originals
- `tests/test_scan.c` — automatic tests for `parse_range()`, manual tests for `scan_port()`

---

## Current State

### Phase 4 — Host discovery (ping sweep) (to do)

- **Next:** raw socket + ICMP ping sweep to discover active hosts on the subnet

---

## Roadmap

### Phase 1 — TCP Port Scanner (completed)

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

#### netutils.c (completed)

Custom reimplementations of byte-order conversion functions, to replace `htons`/`htonl`, `inet_pton`, and `memset` from glibc.

- `my_htons()` — done, verified against `htons` (manual check + test suite)
- `my_htonl()` — done, verified against `htonl` (manual check + test suite)
- `my_inet_pton()` — done, verified against `inet_pton` (manual check + test suite)
- `my_memset()` — done, verified against `memset` via `memcmp` (manual check + test suite)
- Test suite (`tests/test_netutils.c`) — done, all tests pass (`make test`)

---

### Phase 2 — Port range scanning (completed)

Extend the program to scan a range of ports on a single host, introducing timeout handling via `select()`.

**Concepts covered:**
- DROP vs REJECT as a firewall response, and what each reveals to an attacker
- Refactoring: scan logic extracted into `scan_port()` in `scan.c`/`scan.h`
- Defined port states: `PORT_OPEN`, `PORT_CLOSED`, `PORT_FILTERED`, `PORT_ERROR`
- Non-blocking sockets with `fcntl()` + `O_NONBLOCK`
- Timeout handling with `select()` and `getsockopt()`
- Port range parsing

**Usage:**
```bash
./bin/port_scanner <ip> <port>
./bin/port_scanner 127.0.0.1 1-1000
```

**Accepted port range formats:**
- `N` → single port `N`
- `N-M` → range from `N` to `M` (inclusive)
- `N-` → from `N` to 65535
- `-M` → from 0 to `M`
- `M-N` with `M > N` → rejected (inverted range not valid)

**Local testing:**
```bash
# Terminal 1 — open a dummy server
nc -l -p 999

# Terminal 2 — run the scanner
./bin/port_scanner 127.0.0.1 1-1000   # port 999 open
```

#### scan.c (completed)

- `scan_port()` — done, manually tested
- `parse_range()` — done, tested with automatic `assert` (single port, range, open range, NULL, invalid characters, inverted range)
- Test suite (`tests/test_scan.c`) — done, all tests pass (`make test`)
- Main integration — done, manually tested with `scripts/test_env.sh`

---

### Phase 3 — Threading (completed)

Make the scan parallel to reduce execution time using `pthread`, with concurrency control via mutex and semaphore.

**Concepts covered:**
- Thread lifecycle: `pthread_create()` → `pthread_join()`
- Mutex (`pthread_mutex_t`) to protect shared output (`printf`)
- Semaphore (`sem_t`) to cap concurrent threads (max 50) and avoid system saturation
- `thread_args` struct to pass multiple parameters to each thread
- Benchmark: sequential vs threaded scan (see Benchmarks section)

**Usage:** same as Phase 2 — threading is transparent to the user.

**Local testing:**
```bash
sudo ./scripts/test_env.sh    # opens 3 ports, filters 3, runs the scanner
sudo ./scripts/benchmark.sh   # 15 filtered ports, measures total scan time
```

---

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
| `htons()` / `htonl()` | 1 | Byte order, binary representation of numbers |
| `inet_pton()` | 1 | IP string parsing, binary address representation |
| `memset()` | 1 | Direct memory manipulation, byte-level writes |
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
| DROP vs REJECT | Why filtered ports require a timeout instead of an immediate response |
| Blocking vs non-blocking sockets | Why `connect()` needs `fcntl()` + `select()` to avoid hanging on filtered ports |
| Mutex and semaphore | Thread-safe output and concurrency control in threaded scan |
| ICMP echo request/reply | Ping sweep in phase 4 |
| TCP header and checksum | SYN scan in phase 7 |

---

## Benchmarks

### Phase 2 vs Phase 3 — Sequential vs Threaded scan

Test environment: `127.0.0.1`, range `60000-60100` (101 ports), 15 filtered ports (iptables DROP), timeout 2s per port.

| Version | Time |
|---|---|
| Sequential (Phase 2) | 30.028s |
| Threaded (Phase 3) | 2.003s |

**15x speedup** — filtered ports now wait in parallel instead of sequentially.

Reproduce with: `sudo ./scripts/benchmark.sh`

---

## Environment

- **OS:** Linux
- **Compiler:** GCC
- **Language:** C (C99 or later)
- **Dependencies:** none, glibc only (`-lpthread` linked at compile time)

Testing phases 4 and above requires at least two virtual machines on the same network (host-only or bridged).

---

## Disclaimer

This project is developed for educational purposes on owned networks or lab environments. Scanning networks or hosts without authorization is illegal.
