#!/bin/bash
# crusta/effectus_subsumptio.sh - probatio subsumptionis per domum
# (effectus-plan-3 T1; spec-3 par. VIII): omne .sh arboris (oracula/
# exclusa) a binario VETERE et a binario hodierno summatur; situs omnis
# novi a situ veteris eiusdem plagulae, sedis, elementi tegi debet
# (effectus -subsumere). Slice 3 (ordo) valores solum MINUIT - membrum
# novum aut regressio = defectus.
#
# Usage:  ./crusta/effectus_subsumptio.sh <binarium_vetus>
# Exit:   0 omnia subsumpta | 1 defectus nominati | 2 usus/aedificatio
#
# Instrumentum, non porta (lex C14: portae nihil spawnant) - per opus
# slice 3 manu curritur contra binarium slice 2 congelatum.
set -u
CRUSTA_DIR="$(cd "$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")" && pwd)"
RADIX_DIR="$(cd "$CRUSTA_DIR/.." && pwd)"
VETUS="${1:-}"
[ -n "$VETUS" ] && [ -x "$VETUS" ] || {
    echo "usus: effectus_subsumptio.sh <binarium_vetus>" >&2; exit 2; }
"$CRUSTA_DIR/effectus.sh" "$CRUSTA_DIR/effectus.sh" > /dev/null || exit 2
NOVUS="$CRUSTA_DIR/build/effectus"
TABULA="$CRUSTA_DIR/effectus_mandata.stml"
T="$(mktemp -d)"
trap 'rm -rf "$T"' EXIT

numerus=0
defectus=0
while IFS= read -r scriptum; do
    numerus=$((numerus + 1))
    "$VETUS" -radix "$RADIX_DIR" -tabula "$TABULA" "$scriptum" \
        > "$T/v.stml" 2>/dev/null || continue
    "$NOVUS" -radix "$RADIX_DIR" -tabula "$TABULA" "$scriptum" \
        > "$T/n.stml" 2>/dev/null || {
        echo "NOVUS FRACTUS: $scriptum"; defectus=$((defectus + 1)); continue; }
    if ! "$NOVUS" -subsumere "$T/v.stml" "$T/n.stml" > "$T/r.tsv"; then
        defectus=$((defectus + 1))
        echo "NON SUBSUMPTUM: $scriptum"
        sed 's/^/    /' "$T/r.tsv" | head -5
    fi
done < <(cd "$RADIX_DIR" && git ls-files '*.sh' | grep -v '^oracula/')
echo "subsumptio: $((numerus - defectus))/$numerus subsumpta"
[ "$defectus" -eq 0 ]
