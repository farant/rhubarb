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
#   V   filtrum crustae tools/iudex_unitatum.sh (formator, unci): iudex
#       formator_commissionis lib/ solum formare sinit - vendor (alienum),
#       amalgama (productum), generatum, fixtura omissa cum causis
#       (-causae); lint_latinus per idem filtrum amalgama iudicat;
#       prosa_anglica knotapel (Anglice, remissa) iudicat, fixturam md non
#   VI  custodes post-editionem (.claude/hooks/iudex_custodum.sh):
#       custos_iudicat lib/ iudicat, vendor / extra repositorium /
#       ignorata non; examen-custos in vendor/sqlite3.c (examen nudum
#       REICE) et censor-custos in fixtura censoris (censor nudus
#       flagrat) TACENT - silentium filtrum per uncum verum probat
#
# Debetur politicae, canonibus, omni declarationi et nucleo C
# (clausura tools/fabrica.c: lib/unitates.c) - I s, vilis.
#
# Exitus: 0 sanum | 1 FRACTUM | 2 bin/fabrica aut bin/canon_examen deest.
# <porta titulus="unitates" genus="praeiudicium" tegit="unitates.stml unitates.canon unitas.canon unitas.stml */unitas.stml canones.registrum tools/iudex_unitatum.sh tools/iudex_unitatum.c .claude/hooks/iudex_custodum.sh .claude/hooks/examen-custos.sh .claude/hooks/censor-custos.sh .claude/hooks/formator-custos.sh .claude/hooks/canon-custos.sh" currit="tools/fabrica.c"/>
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

# V filtrum crustae
GENF=$(git ls-files 'lib/*.c' | while read -r v; do head -n1 "$v" | grep -qw GENERATUM && { echo "$v"; break; }; done)
./tools/iudex_unitatum.sh -causae formator_commissionis lib/xar.c vendor/sqlite3.c \
    silva/amalgama/silva.h "$GENF" "$FIXA" > "$T/o5" 2> "$T/o5e"; rc5=$?
./tools/iudex_unitatum.sh lint_latinus silva/amalgama/silva.h vendor/sqlite3.c > "$T/o5b" 2>/dev/null; rc5b=$?
MDFIXA=$(git ls-files 'md/probationes/fixa/*.md' | head -1)
./tools/iudex_unitatum.sh prosa_anglica knotapel/CLAUDE.md "$MDFIXA" > "$T/o5c" 2>/dev/null; rc5c=$?
if [ "$rc5" -eq 0 ] && [ "$(cat "$T/o5")" = "lib/xar.c" ] && [ -n "$GENF" ] \
   && grep -q '^vendor/sqlite3.c: non iudicatur - cura aliena' "$T/o5e" \
   && grep -q '^silva/amalgama/silva.h: non iudicatur - munus amalgama' "$T/o5e" \
   && grep -q "^$GENF: non iudicatur - provenientia generatum" "$T/o5e" \
   && [ "$rc5b" -eq 0 ] && [ "$(cat "$T/o5b")" = "silva/amalgama/silva.h" ] \
   && [ -n "$MDFIXA" ] && [ "$rc5c" -eq 0 ] && [ "$(cat "$T/o5c")" = "knotapel/CLAUDE.md" ]; then
    echo "  V   filtrum crustae: formator lib/ solum (vendor, amalgama, generatum, fixtura cum causis), lint amalgama iudicat, prosa knotapel non fixturam OK"
else
    echo "  V   FRACTUM (rc=$rc5/$rc5b/$rc5c, generatum '$GENF')"; sed 's/^/      /' "$T/o5" "$T/o5e" "$T/o5b" "$T/o5c"; fracta=1
fi

# VI custodes post-editionem
. "$RADIX/.claude/hooks/iudex_custodum.sh"
c_lib=1; c_ven=0; c_ext=0; c_ign=0
custos_iudicat examen_commissionis "$RADIX/lib/xar.c"; c_lib=$?
custos_iudicat examen_commissionis "$RADIX/vendor/sqlite3.c"; c_ven=$?
custos_iudicat examen_commissionis "$T/extra.c"; c_ext=$?
custos_iudicat formator_commissionis "$RADIX/build/fumus_ignoratum.c"; c_ign=$?
CENS=$(git ls-files 'silva/probationes/fixa/censoris/*.c' | head -1)
h_ex=$(printf '{"tool_input":{"file_path":"%s"}}' "$RADIX/vendor/sqlite3.c" | bash "$RADIX/.claude/hooks/examen-custos.sh" 2>&1)
h_ce=$(printf '{"tool_input":{"file_path":"%s"}}' "$RADIX/$CENS" | bash "$RADIX/.claude/hooks/censor-custos.sh" 2>&1)
if [ "$c_lib" -eq 0 ] && [ "$c_ven" -eq 1 ] && [ "$c_ext" -eq 1 ] && [ "$c_ign" -eq 1 ] \
   && [ -n "$CENS" ] && [ -z "$h_ex" ] && [ -z "$h_ce" ]; then
    echo "  VI  custodes: lib/ iudicatur, vendor/extra/ignorata non; examen-custos et censor-custos tacent in vendor et fixtura OK"
else
    echo "  VI  FRACTUM (lib $c_lib, vendor $c_ven, extra $c_ext, ignorata $c_ign)"; printf '%s\n%s\n' "$h_ex" "$h_ce" | sed 's/^/      /' | head -10; fracta=1
fi

if [ "$fracta" -ne 0 ]; then echo "fumus unitatum: FRACTUM"; exit 1; fi
echo "fumus unitatum: sanum"
exit 0
