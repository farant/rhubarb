#!/bin/bash
# tools/portae_debitae.sh - QUAE PORTAE mutationi debentur, et CUR.
#
# Pro quaque via: portae (registrum PORTAE in pythonica/silva.py) quae
# eam tegunt - calculus in C (bin/fabrica portae debitae; lib/portae.c)
# ex annotationibus <porta> cursorum et derivatione (effectus, clausurae
# aedilis, litterae probationum). Causa prima sub quaque porta. Portae
# manu (homo currit) sub MANU; viae nullius portae sub INTECTA. Spec:
# project-specs/portae-declaratae-spec.md. Tabularium non legitur.
#
# Usus:
#   ./tools/portae_debitae.sh [viae...]
#       sine viis: viae mutatae contra HEAD + novae non ignoratae
#   bin/fabrica portae tactus -portae TSV VIA...   rationes OMNES
#
# Sumptus: via C (.c/.h/.m) clausuras fontium portarum rogat (~II s
# calide; ~XXX s primum post bin/fabrica novum - thesaurus extractoris).
#
# Exitus: 0 iudicatum | 2 NIHIL iudicatum (nulla via mutata, aut
#         declarationes recusatae / bin/fabrica fractum - numquam cum
#         viridi confundendum).
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
exec python3 - "$@" <<'PY'
import sys
sys.path.insert(0, 'pythonica')
import silva

viae = sys.argv[1:] or silva.viae_mutatae()
if not viae:
    print('portae debitae: nulla via mutata - nihil iudicatum',
          file=sys.stderr)
    sys.exit(2)
try:
    sys.stdout.write(silva.portae_debitae_relatio(viae))
except silva.SilvaError as ex:
    print('portae debitae: %s' % ex, file=sys.stderr)
    sys.exit(2)
PY
