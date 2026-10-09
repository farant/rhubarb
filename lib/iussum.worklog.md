# iussum worklog

## 2026-10-08 - S3a: recognition only, known verbs only

Born for vicus-latera S3 (acme commands in scriba pages). Pure reader
over a TabulaCharacterum; no registry here - the host answers "is this
verb known?" through `IussumNotum`, an idea taken from prunifex (only
registered commands highlight or respond), which removes the need for
an escape syntax: `$5.00` and a half-typed `$foo(` stay prose.

Rules worth remembering: `$` must start the line or follow a
non-word character (`a$b`, `x_$dies` are not commands); arguments end
at the FIRST `)` on the same line (no nesting by design); an unclosed
`(` makes the whole token not-a-command rather than a command without
arguments, so half a command never runs. Empty cells are `'\0'` in
scriba pages - they count as spaces when trimming.

Gotcha: `tabula_cellula(t, l, c)` with s32 coordinates trips
-Wsign-conversion (i32 width is unsigned) - the lib goes through one
`cellula()` helper with explicit casts. And the installed `scribe`
judges against main's repo, so in a worktree it writes but answers
"via extra repositorium" (exit 4) - judge with ./silva/examen.sh.

## 2026-10-08 - S3b-1: the registry

Opaque `IussumRegistrum` over a Xar (a handful of verbs - linear
lookup is fine). A verb must have the same shape as in text
(`[a-z][a-z0-9_]*`), otherwise it could never be clicked; registering
an existing verb REPLACES it (plant: appending instead left the old
entry winning the lookup). `iussum_currere` clears the effect before
calling, so a verb never sees a stale error. Gotcha: `registrum` is a
latina.h macro (`register`) - the examen rejected it as a parameter
name; the `IussumNotum`-shaped function takes `ctx`.
