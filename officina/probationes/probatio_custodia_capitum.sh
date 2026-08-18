#!/bin/bash
# probatio_custodia_capitum.sh - custodia aetatum MCP contra caput
# NATUM MEDIA SESSIONE (foramina A+B, 2026-08-18, 01KZ6GH88Z).
#
# Forma differentialis: plagula .c iudicatur ANTE caput eius natum
# (REICE rectum), caput nascitur, plagula ITERUM iudicatur - ACCIPE
# exspectatum. Ante sanationem REICE manebat (cache praeparationum
# derivatarum reaedificationi basis supervivebat); post, custodia
# derivatas purgat et ACCIPE redit.
#
# Residentem RECENTEM generat (JSON lineare in stdin, stderr in
# plagulam) - residentes vivos non tangit. Binarium PRAESENS
# iudicatur: ./officina/legatus.sh -aedificare-solum ante hoc si
# fontes mutasti. Ex radice repositorii currendum.
set -u
R="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
BIN="$R/officina/build/legatus/legatus"
OPUS="$(mktemp -d)"
CAPUT="$R/include/zz_custodia_probatio.h"
FONS="$R/lib/zz_custodia_probatio.c"

purgare() { rm -f "$CAPUT" "$FONS"; rm -rf "$OPUS"; }
trap purgare EXIT

[ -x "$BIN" ] || { echo "FRACTA: binarium abest ($BIN) - ./officina/legatus.sh -aedificare-solum primum"; exit 1; }

rm -f "$CAPUT"
cat > "$FONS" <<'EOH'
#include "postulata_posix.h"
#include "zz_custodia_probatio.h"
#include <string.h>
i32
zz_custodia_legere (ZzCustodiaProbatio* p)
{
    ZzCustodiaProbatio alter;
    memcpy(&alter, p, magnitudo(ZzCustodiaProbatio));
    redde alter.valor;
}
EOH

req() { printf '%s\n' "$1"; }
(
  req '{"jsonrpc":"2.0","id":1,"method":"initialize","params":{"protocolVersion":"2025-06-18","capabilities":{},"clientInfo":{"name":"probatio-custodiae","version":"0"}}}'
  sleep 1
  req '{"jsonrpc":"2.0","method":"notifications/initialized"}'
  sleep 1
  req '{"jsonrpc":"2.0","id":2,"method":"tools/call","params":{"name":"diagnostica","arguments":{"via":"lib/zz_custodia_probatio.c"}}}'
  sleep 4
  cat > "$CAPUT" <<'EOH'
#ifndef ZZ_CUSTODIA_PROBATIO_H
#define ZZ_CUSTODIA_PROBATIO_H
#include "latina.h"
nomen structura { i32 valor; } ZzCustodiaProbatio;
i32 zz_custodia_legere (ZzCustodiaProbatio* p);
#endif
EOH
  sleep 1
  req '{"jsonrpc":"2.0","id":3,"method":"tools/call","params":{"name":"diagnostica","arguments":{"via":"lib/zz_custodia_probatio.c"}}}'
  sleep 6
) | "$BIN" -mcp -radix "$R" -signum probatio-custodiae \
    -manifestum "$R/officina/build/legatus/legatus.vigilia" \
    > "$OPUS/exitus.jsonl" 2> "$OPUS/stderr.log"

VERDICTA="$(grep -o 'verdictum: [A-Z]*' "$OPUS/exitus.jsonl" | awk '{print $2}' | tr '\n' ' ')"
echo "verdicta: $VERDICTA(exspectata: REICE ACCIPE)"
if [ "$VERDICTA" = "REICE ACCIPE " ]; then
    echo "PROBATIO CUSTODIAE PRAETERIIT"
    exit 0
fi
echo "PROBATIO CUSTODIAE FRACTA - stderr sequitur:"
cat "$OPUS/stderr.log"
exit 1
