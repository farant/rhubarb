#!/bin/bash
# tools/cursus.sh - CHASSIS CURSUUM (analytica T2): FONTANDUM in capite
# scripti, ante opus:
#
#   # <cursus titulus="md/compile_probationes.sh" versio="I"/>
#   source "$RADIX_DIR/tools/cursus.sh"
#
# Scriptum SEMEL sub 'bin/mensor cursus currere' iterum exsequitur:
# mensor cursum aperit (initium in diario), effusionem et erratum
# TRANSMITTIT ET CAPIT, exitum exactum et rusage ex wait4 sumit, finem
# scribit, acta servat, codicem exitus scripti reddit. Contextus
# (commissum, sordidum, ramus, arbor, machina, vocator) semel computatur
# et per ambitum hereditatur; cursus interior parentem accipit.
#
# CURSUS_INTUS = scriptum iam involutum (iteratio eiusdem scripti -
# exemplar ianuarum 'tee' - non iterum involvitur). CURSUS_NULLUS=1 =
# sine involucro. bin/mensor absens (arbor frigida) = scriptum sine
# involucro currit: analytica numquam scriptum frangit. Spec:
# project-specs/analytica-spec.md par. III. Porta: mensor-cursus.
_cursus_radix="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
if [ -z "${CURSUS_NULLUS:-}" ] && [ "${CURSUS_INTUS:-}" != "$0" ] \
    && [ -x "$_cursus_radix/bin/mensor" ]; then
    exec "$_cursus_radix/bin/mensor" cursus currere -- "$0" "$@"
fi
unset _cursus_radix
