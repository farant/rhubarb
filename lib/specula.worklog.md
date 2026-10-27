# specula worklog

## 2026-10-26 - created (vicus quies T6c)

A file watch whose fd the reactor / window loop can sleep on, so vicus
reads the notifications log only when it changes instead of per pulse.

- kqueue + EVFILT_VNODE on an `O_EVTONLY` fd (no bytes read - hence the
  rule-96 opt-out on the one `open`). `EV_CLEAR` is required: without it
  a vnode event stays active and the drain loop never ends.
- Delete/rename (rotation): the watched fd is closed and the path
  re-opened in the next drain; if the file is gone, the parent
  directory is watched (NOTE_WRITE = an entry was added) until it
  reappears. Creation of an absent file is seen the same way.
- CALLERS MUST DRAIN every pulse the fd is readable - an undrained
  kqueue fd keeps the loop awake forever (plant "non exhaurit").
- Variant files, not `#ifdef __APPLE__`: examen does not judge an
  `#ifdef __APPLE__` branch at all (an undefined call there passed), so
  the kqueue code lives in specula_macos.c (judged against the kevent
  surface certified by officina/auspex_posix.sh) and specula_posix.c
  returns NIHIL (Linux/Android: inotify later).
- New lib checklist hit: compile_tests_fontes_generata.sh must be
  regenerated (`./tools/compile_tests_fontes_generare.sh`) or the root
  suite never compiles the new .c (link error, "0 mutata"); a renamed
  lib leaves build/<old>.o behind - deleted.
