#!/bin/bash
# xml/aurum_silvestre.sh - aurum oraculorum differentialium super corpus
# silvestre (planum xml-arbor P12): expat (verdictum, sigillum formae
# canonicae) et libxml2 (verdictum) pro omni plagula manifesti cuius
# sigillum congruit. Exitus xml/build/aurum_silvestre.tsv (NUMQUAM
# commissum: ex plagulis vivis derivatum - regula Frani, manifestum
# solum). Actio fabricae 'xml_aurum_silvestre' (xml/aedificatio.stml);
# sub FABRICA_SCRIPTURA in scripturam scribit. Octeti deterministici
# (sine die).
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
EXITUS="xml/build/aurum_silvestre.tsv"
if [ -n "${FABRICA_SCRIPTURA:-}" ]; then
    EXITUS="$FABRICA_SCRIPTURA/$EXITUS"
else
    EXITUS="$RADIX/$EXITUS"
fi
mkdir -p "$(dirname "$EXITUS")" || exit 1
# -B: expat_aurum.py expat_canonicum.py importat - sine -B python
# __pycache__/ in arbore scriberet (clone novus: "scripsit extra
# vestigium", actio FRACTA; arbor vetus .pyc iam habebat)
python3 -I -B "$RADIX/xml/instrumenta/expat_aurum.py" \
    "$RADIX/xml/probationes/fixa/silvestria.manifestum" > "$EXITUS.novum" \
    || exit 1
mv "$EXITUS.novum" "$EXITUS" || exit 1
echo "aurum silvestre: $(grep -vc '^#' "$EXITUS") plagulae -> $EXITUS"
