- ptrace (macos/unix)
  - ptrace is a syscall
  - parent can:
    - pause child process
    - read memory
    - change memory
    - see where it is executing
    - continue the execution


so as a debugger
- fork a process
- tell OS parent is allowed to trace
- send SIGSTOP to self so parent can set things up
- use execvp to replace code with target program 


decomposition
- platform layer
- event system
- breakpoints
- symbols?
- dwarf parser?


want to make a binary parser first.

so
- macho.h   - which parses macho format
- sectio.h  - which extracts sections (depends on macho)
- symbola.h - which parses the symbol table (depends on macho)
- dwarf.h   - to parse dwarf files (depends on macho)
