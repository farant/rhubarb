# machina worklog

## 2026-08-13 — birth

Built so test metrics (and later, visual goldens) can say *which
machine* produced them. Fran's framing: "comparing apples and apples".

### The shape of the failure this prevents

A machine ID that silently degrades to a constant — `"unknown"`, or a
stubbed hash — would merge every machine's measurements into one
bucket. That is the exact corruption the library exists to prevent, and
it would look like the library working. So `machina_identitas` returns
an **empty chorda** when it cannot determine the machine, never a
placeholder. Refusal is loud; degradation is silent.

### Why hash a platform source rather than persist a random ID

Two designs were on the table:

1. generate a random UUID once, persist it in `~/.rhubarb/machina.id`
2. hash a platform-stable source with SHA-256

(2) won. `uuid.h` only *parses* UUIDs — there is no generator — so (1)
would have needed `/dev/urandom` **and** a state file that can be lost,
copied, or diverge between checkouts. (2) has no state at all: macOS
`gethostuuid()` and Linux `/etc/machine-id` are already stable machine
identifiers, and hashing makes ours opaque (one-way, so the hardware
UUID never reaches a committed file) and uniform in length.

Truncated to 16 hex chars: 64 bits, collision-free at any plausible
number of machines, and still readable in a table.

### The seam that makes it testable — and why it had to exist

`machina_identitas_ex_fonte(chorda fons, ...)` is exposed deliberately.
An implementation returning a **constant** passes every obvious test:
it is stable across calls, it is 16 hex characters, it is not the
source string. Only a test that hashes **two different sources and
compares** can see it.

Measured, by poisoning the hash to a fixed string: **20 of 22
assertions still passed.** Only the two cross-source comparisons
(probatio lines 70, 88) went red. Without that seam the module would
have had no way to prove it hashes anything at all.

### Tooling this dragged in (worth knowing)

silva's POSIX shim (`silva/fontes/systema_posix.h`) had no
`sys/utsname.h` and no `sysconf`, so the file judged REICE while clang
compiled it clean. Fixed properly rather than pinned:

- added a `sys/utsname.h` section with **measured** Darwin shapes —
  `_SYS_NAMELEN` 256, `struct utsname` 1280 bytes, fields at 512/1024
- added `sysconf` + `_SC_NPROCESSORS_ONLN` (Darwin value 58; Linux is
  84 — the shim describes Darwin, real compilation uses real headers)
- extended `officina/auspex_posix.sh` to certify the new shape, and
  **verified that new gate can fail** by poisoning the expected size
  to 1281 (exit 1, compile-time assert)

The shim's own doctrine is that shapes are real, not synthetic, because
officina's interpreters dereference members — so measuring rather than
recalling was mandatory, not fastidious.

### Note

The post-edit hook caught two invented `chorda_aedificator` function
names before compilation. Real names are
`chorda_aedificator_appendere_literis` and `chorda_aedificator_finire`.
Checking the header beats guessing from a naming pattern.
