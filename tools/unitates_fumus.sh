#!/bin/bash
# tools/unitates_fumus.sh - porta 'unitates' (spec unitates-codicis U3
# gradus 2c): politica et declarationes unitatum super ARBOREM VERAM.
#
#   I   canon: unitates.stml et omnis unitas.stml (tractae + novae)
#       contra canones registri - VITIA 0
#   II  'bin/fabrica quid -omnes': lectio sine recusatione, unitates
#       directorii (unitas.stml) omnes adsunt
#   III responsa nota: vendor alienum, pars _posix ad basim, probatio
#       applicationis per praefixum, amalgama generatum, archivum
#       archivatum, knotapel remissum, caput auxiliare probationum
#   IV  iudex lint_latinus: alienum (vendor, oracula), remissum
#       (knotapel), archivatum, fixtura NON iudicantur (causa nominata);
#       lib/ et generatum (amalgama) iudicantur; iudex ignotus = exitus 2
#
# Debetur politicae, canonibus, omni declarationi et nucleo C
# (clausura tools/fabrica.c: lib/unitates.c) - I s, vilis.
#
# Exitus: 0 sanum | 1 FRACTUM | 2 bin/fabrica aut bin/canon_examen deest.
# <porta titulus="unitates" genus="praeiudicium" tegit="unitates.stml unitates.canon unitas.canon unitas.stml */unitas.stml canones.registrum" currit="tools/fabrica.c"/>
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
[ -x bin/fabrica ] || { echo "fumus unitatum: bin/fabrica deest (./tools/fabrica_struere.sh)"; exit 2; }
[ -x bin/canon_examen ] || { echo "fumus unitatum: bin/canon_examen deest (./tools/canon_struere.sh)"; exit 2; }
T="$(mktemp -d "${TMPDIR:-/tmp}/unitates_fumus.XXXXXX")"
trap 'rm -rf "$T"' EXIT
fracta=0

# I canon
git ls-files -co --exclude-standard | grep -E '(^|/)unitas\.stml$' > "$T/declarationes"
nd=$(wc -l < "$T/declarationes" | tr -d ' ')
bin/canon_examen unitates.stml $(cat "$T/declarationes") > "$T/o1" 2>&1; rc1=$?
if [ "$rc1" -eq 0 ] && grep -q 'VITIA 0' "$T/o1" && [ "$nd" -gt 0 ]; then
    echo "  I   canon: politica + $nd declarationes, VITIA 0 OK"
else
    echo "  I   FRACTUM (rc=$rc1, declarationes $nd)"; sed 's/^/      /' "$T/o1" | head -20; fracta=1
fi

# II lectio
bin/fabrica quid -omnes -machina > "$T/o2" 2>&1; rc2=$?
nu=$(awk -F'\t' '$5 ~ /unitas\.stml$/' "$T/o2" | wc -l | tr -d ' ')
if [ "$rc2" -eq 0 ] && [ "$nu" -eq "$nd" ]; then
    echo "  II  lectio: $(wc -l < "$T/o2" | tr -d ' ') unitates, $nu directorii = declarationes OK"
else
    echo "  II  FRACTUM (rc=$rc2, directorii $nu pro $nd)"; sed 's/^/      /' "$T/o2" | head -20; fracta=1
fi

# III responsa nota: via proprietas valor
expectare () {
    local via="$1" proprietas="$2" valor="$3" linea
    linea=$(bin/fabrica quid -machina "$via" 2>&1 | awk -F'\t' -v p="$proprietas" '$2 == p')
    if [ "$(printf '%s' "$linea" | cut -f3)" = "$valor" ]; then
        return 0
    fi
    echo "      $via: $proprietas exspectatum '$valor', datum: $linea"
    return 1
}
KNOT=$(git ls-files 'knotapel/*.c' | head -1)
ARCH=$(git ls-files 'archivum/*.c' | head -1)
mala=0
expectare vendor/sqlite3.c cura aliena || mala=1
expectare oracula/clay/clay_vinculum.c cura aliena || mala=1
expectare lib/energia_posix.c unitas "energia (/elementa_programmatis/bibliotheca)" || mala=1
expectare probationes/probatio_pictor_strata.c unitas "pictor (/elementa_programmatis/applicatio)" || mala=1
expectare silva/amalgama/silva.c provenientia generatum || mala=1
expectare officina/amalgama/officina.h provenientia manu || mala=1
expectare "$ARCH" vita archivata || mala=1
expectare "$KNOT" cura remissa || mala=1
expectare probationes/pictor_commune.h munus probatio || mala=1
expectare lib/fabrica_sanare.c catena "fabrica_sanare < fabrica < rhubarb" || mala=1
if [ "$mala" -eq 0 ]; then
    echo "  III responsa nota (X) OK"
else
    echo "  III FRACTUM"; fracta=1
fi

# IV iudex lint_latinus
FIXA=$(git ls-files 'probationes/fixa/*.c' | head -1)
bin/fabrica quid -iudex lint_latinus -machina lib/xar.c vendor/sqlite3.c \
    oracula/clay/clay_vinculum.c "$KNOT" "$ARCH" "$FIXA" silva/amalgama/silva.c > "$T/o4" 2>&1; rc4=$?
ni=$(awk -F'\t' '$2 == "IUDICAT"' "$T/o4" | wc -l | tr -d ' ')
nn=$(awk -F'\t' '$2 == "NON_IUDICAT" && $3 != ""' "$T/o4" | wc -l | tr -d ' ')
bin/fabrica quid -iudex nusquam lib/xar.c > "$T/o4b" 2>&1; rc4b=$?
if [ "$rc4" -eq 0 ] && [ "$ni" -eq 2 ] && grep -q '^lib/xar.c	IUDICAT' "$T/o4" \
   && grep -q '^silva/amalgama/silva.c	IUDICAT' "$T/o4" && [ "$nn" -eq 5 ] \
   && [ "$rc4b" -eq 2 ] && grep -q "noti: lint_latinus" "$T/o4b"; then
    echo "  IV  iudex lint_latinus: lib/ et generatum iudicantur, V exclusa cum causis, iudex ignotus exitus 2 OK"
else
    echo "  IV  FRACTUM (rc=$rc4, iudicat $ni, exclusa cum causa $nn, ignotus rc=$rc4b)"; sed 's/^/      /' "$T/o4" "$T/o4b"; fracta=1
fi

if [ "$fracta" -ne 0 ]; then echo "fumus unitatum: FRACTUM"; exit 1; fi
echo "fumus unitatum: sanum"
exit 0
