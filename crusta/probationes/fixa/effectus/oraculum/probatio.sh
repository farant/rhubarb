#!/bin/bash
# oraculum fixum: probationes '[' '[[' test (effectus T5)
[ -f data/a.txt ] || exit 1
[[ -d data ]] || exit 1
test -e data/absens.txt && exit 1
# <tolera codex="lint:nt-aequalitas" (>fixum oraculi: -nt ut probatio plagularum, aequalitas nihil refert
[ data/a.txt -nt data/b.txt ] || true
exit 0
