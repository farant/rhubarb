#!/bin/bash
# tools/provenientia_obiectum.sh - obiectum provenientiae pro
# installatore MANU scripto (plan fabrica 1a T7)
#
# Installatores ab aedile geniti obiectum per annotationem nectunt;
# installatores manu scripti (aedilis, canon, natura, stml) hunc
# vocant: manifestum clausurae renovat (bin/aedilis SCOPUS - ingressus
# digesti, etiam si structura indice manu utitur), plagulam
# provenientiae scribit, eam SEMPER compilat (parvum; laqueus mtime
# nullus) et viam obiecti in stdout reddit.
#
# Usus: OBJ="$(./tools/provenientia_obiectum.sh T bin/T tools/T.c [ACTIO])"
# (ACTIO: vide provenientia_scribere.sh - familia binariorum, 1b T5)
set -u
cd "$(dirname "${BASH_SOURCE[0]}")/.." || exit 1
T="${1:?usus: provenientia_obiectum.sh TITULUS ARTIFICIUM SCOPUS [ACTIO]}"
A="${2:?}"
S="${3:?}"
ACTIO="${4:-$T}"
source tools/vexilla.sh
if [ -x bin/aedilis ]; then
    bin/aedilis "$S" >/dev/null 2>&1 || { echo "provenientia_obiectum: aedilis $S fracta" >&2; exit 1; }
else
    export PROVENIENTIA_TOLERANS=1    # ovum: aedilis nondum structus
fi
./tools/provenientia_scribere.sh "$T" "$A" "$ACTIO" >&2 || exit 1
OBJ="build/fabrica/provenientia/$T.o"
clang "${VEXILLA_C89[@]}" -Iinclude -c "build/fabrica/provenientia/$T.c" \
    -o "$OBJ" >&2 || exit 1
echo "$OBJ"
