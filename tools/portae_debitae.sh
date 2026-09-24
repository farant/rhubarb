#!/bin/bash
# tools/portae_debitae.sh - QUAE PORTAE mutationi debentur, et CUR.
#
# Pro quaque via: portae (PORTAE in pythonica/silva.py) quae eam
# tegunt - per clausuras aedilis probationum suitae, et per lentes
# inventarii 'suitae probationum' quas graphus non videt ('currit
# binaria', 'tegit viae'). Causa prima sub quaque porta. Portae extra
# PORTAE (GUI, consulto seorsum) sub MANU; viae nullius portae sub
# INTECTA. Spec: project-specs/portae-debitae-spec.md.
#
# Usus:
#   ./tools/portae_debitae.sh [viae...]
#       sine viis: viae mutatae contra HEAD + novae non ignoratae
#   PORTAE_DEBITAE_INVENTARIUM=<titulus>   inventarium alterum
#
# Sumptus: via C (.c/.h/.m) clausuras omnium suitarum verrit (~XVIII
# s); viae ceterae statim.
#
# Exitus: 0 iudicatum | 2 NIHIL iudicatum (nulla via mutata, aut
#         inventarium illegibile - numquam cum viridi confundendum).
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
exec python3 - "$@" <<'PY'
import os
import sys
sys.path.insert(0, 'pythonica')
import silva

viae = sys.argv[1:] or silva.viae_mutatae()
if not viae:
    print('portae debitae: nulla via mutata - nihil iudicatum',
          file=sys.stderr)
    sys.exit(2)
res = os.environ.get('PORTAE_DEBITAE_INVENTARIUM',
                     silva.INVENTARIUM_SUITARUM)
try:
    sys.stdout.write(silva.portae_debitae_relatio(viae, res))
except silva.SilvaError as ex:
    print('portae debitae: %s' % ex, file=sys.stderr)
    sys.exit(2)
PY
