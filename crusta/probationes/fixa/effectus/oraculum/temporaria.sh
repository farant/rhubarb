#!/bin/bash
# oraculum fixum: objecta mktemp (effectus-plan-2 T3) - quae sub eis
# leguntur et scribuntur classis temporaria sunt
T="$(mktemp -d)"
echo x > "$T/a"
cat "$T/a" > /dev/null
F=$(mktemp)
echo y > "$F"
[ -s "$F" ] || exit 1
rm -rf "$T" "$F"
exit 0
