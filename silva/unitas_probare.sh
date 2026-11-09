#!/bin/bash

# silva/unitas_probare.sh - porta UNITAS-PERCURSUS: percursus ex plagula
# una (tools/instrumentum_unitas.sh, ordinarium) et ex obiectis singulis
# (SILVA_UNITAS=0) EADEM dicere debent. Optimizatio trans plagulas
# morem non mutat nisi codex mores indefinitos habet (aliasing, ordo
# evaluationis) - tunc modi divergunt, et hic clamant. Exemplar:
# knotapel/archive.sh (vivum contra instantaneum, effusio octetim).
#
#   I    ambo modi nectuntur (INSTRUMENTUM_NECTERE_SOLUM)
#   II   unitas VERA est: plagula conflata (build/percursus.unitas/
#        percursus.c) '#line' plagularum clausurae fert, statica
#        renominata (tabula non vacua), binarium ea recentius
#   IV   plagula clausurae (non radix) tacta: unitas renectitur
#   V    INTERIORA (budget XIII): aedificatio frigida PLANA est et
#        mortua.txt ex binario nexo (-dead_strip) scribit; renexio (IV)
#        ea applicat - 'static ' in plagula conflata ante prototypa
#        mortuorum; cache sedium capitis cum clave aliena (= caput
#        mutatum) renovatur, non creditur
#   III  'percursus -semantica lib' tribus modis (unitas plana, unitas
#        cum interioribus, obiecta): relatio (sine lineis
#        temporis/memoriae) et exitus eadem
#
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
# <porta titulus="unitas-percursus" tegit="tools/instrumentum_unitas.sh tools/corpus_infixum.sh silva/percursus.sh silva/unitas_probare.sh silva/instrumenta/silva_conflatio.c silva/instrumenta/silva_conflatio.h silva/instrumenta/principalia/conflator.c" currit="silva/instrumenta/principalia/percursus.c"/>

set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
T="$(mktemp -d "${TMPDIR:-/tmp}/unitas_probare.XXXXXX")" || exit 2
trap 'rm -rf "${T:?}"' EXIT
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
CORPUS="lib"
UNITAS="silva/build/percursus"
OBIECTA="silva/build/percursus_obiecta"
CONFLATA="silva/build/percursus.unitas/percursus.c"
MORTUA="silva/build/percursus.unitas/mortua.txt"
INTERIORA="silva/build/percursus.unitas/interiora.tsv"
SEDES_XAR="silva/build/percursus.unitas/sedes/include/xar.h.tsv"

# I  ex fontibus SEMPER: binarium et cache unitatis deleta - porta cuius
#    binarium non ab ipsa struitur sub planta MUTA est (silva.planta
#    tempus plagulae restituit; binarium plantae 'recens' videretur:
#    planta U4 sic viridis manebat, 2026-11-07)
rm -rf "$UNITAS" "$UNITAS.dSYM" "$UNITAS.unitas" "$OBIECTA" "$OBIECTA.dSYM"
INSTRUMENTUM_NECTERE_SOLUM=1 ./silva/percursus.sh > "$T/unitas.struere" 2>&1
credo $? "I unitas nexa ($UNITAS)"
SILVA_UNITAS=0 INSTRUMENTUM_NECTERE_SOLUM=1 ./silva/percursus.sh \
    > "$T/obiecta.struere" 2>&1
credo $? "I obiecta nexa ($OBIECTA)"
if [ ! -x "$UNITAS" ] || [ ! -x "$OBIECTA" ]; then
    echo "UNITAS PERCURSUS: FRACTA (binarium deest)"
    exit 1
fi

# II
lineae="$(grep -c '^#line 1 "' "$CONFLATA" 2> /dev/null)"
[ "${lineae:-0}" -ge 30 ] && grep -q '^#line 1 "lib/xar.c"$' "$CONFLATA" \
    && grep -q '^#line 1 "silva/fontes/silva_glr.c"$' "$CONFLATA"
credo $? "II plagula conflata: ${lineae:-0} plagulae per '#line' (xar.c, silva_glr.c inter eas)"
# find -newer tempora plena (nanosecundae): plagula conflata binario
# STRICTE recentior = binarium vetus
[ -f "$CONFLATA" ] && [ -z "$(find "$CONFLATA" -newer "$UNITAS" -print)" ]
credo $? "II binarium unitatis plagula conflata non vetustius"
# tabula staticorum vere applicata: clausura percursus hodie nullum
# staticum duplicem habet (amalgamator silvae duplicia recusat, S41),
# ergo compilatio sine renominatione munda est et relatio eadem - III
# tabulam vacuam non videt (planta U4, 2026-11-07); hic numeratur
renominata="$(grep -c '^#undef ' "$CONFLATA" 2> /dev/null)"
[ "${renominata:-0}" -ge 100 ]
credo $? "II statica per plagulam renominata (${renominata:-0} #undef)"

# V (pars frigida) aedificatio sine mortua.txt priore plana est, et
#   mortua ex binario suo scribit; binarium planum servatur (III)
cp "$UNITAS" "$T/plana"
mortua_frigida="$(grep -c . "$MORTUA" 2> /dev/null)"
statica_frigida="$(grep -c '^static ' "$CONFLATA" 2> /dev/null)"
[ "${mortua_frigida:-0}" -ge 100 ] && [ ! -s "$INTERIORA" ]
credo $? "V frigida: plana (interiora nulla), mortua ${mortua_frigida:-0} ex binario"
cp "$MORTUA" "$T/mortua.frigida" 2> /dev/null
# cache sedium xar.h cum clave ALIENA et sede falsa: renexio (IV) eam
# renovare debet (clavis contenti), non credere
if [ -f "$SEDES_XAR" ]; then
    printf '# clavis aliena\ninclude/xar.h\t1\t1\t1\n' > "$SEDES_XAR"
fi

# IV plagula CLAUSURAE (non radix) tacta -> unitas renectitur: stalitas
#    per clausuram servatam, et involucrum viam unitatis vere sumit
#    (binarium vetus unitatis aliter III fallere posset). Tempus
#    plagulae post restituitur (touch -r): contentum idem.
TACTA="silva/fontes/silva_glr.c"
touch -r "$TACTA" "$T/tacta.tempus"
ante="$(stat -f '%i %m' "$UNITAS")"
touch "$TACTA"
INSTRUMENTUM_NECTERE_SOLUM=1 ./silva/percursus.sh > "$T/unitas.iv" 2>&1
rciv=$?
post="$(stat -f '%i %m' "$UNITAS")"
touch -r "$T/tacta.tempus" "$TACTA"
[ "$rciv" -eq 0 ] && [ "$ante" != "$post" ]
credo $? "IV $TACTA tacta -> unitas renexa ($ante -> $post)"

# V (pars calida) renexio IV mortua frigida applicuit
numerus_interiorum="$(grep -c . "$INTERIORA" 2> /dev/null)"
statica_calida="$(grep -c '^static ' "$CONFLATA" 2> /dev/null)"
[ "${numerus_interiorum:-0}" -ge 100 ] \
    && [ $((${statica_calida:-0} - ${statica_frigida:-0})) -ge 100 ]
credo $? "V calida: interiora ${numerus_interiorum:-0}, lineae '^static ' in conflata ${statica_frigida:-0} -> ${statica_calida:-0}"
[ -f "$SEDES_XAR" ] && ! grep -q 'clavis aliena' "$SEDES_XAR" \
    && [ "$(grep -c . "$SEDES_XAR")" -ge 10 ]
credo $? "V cache sedium xar.h cum clave aliena renovata"
if cmp -s "$MORTUA" "$T/mortua.frigida"; then
    echo "       (mortua stabilia: eadem post interiora)"
else
    echo "       (mortua mutata post interiora: $(grep -c . "$MORTUA") - licet, tabula stala tuta)"
fi

# III
"./$UNITAS" -semantica "$CORPUS" > "$T/unitas.out" 2> "$T/unitas.err"
rcu=$?
"$T/plana" -semantica "$CORPUS" > "$T/plana.out" 2> "$T/plana.err"
rcp=$?
"./$OBIECTA" -semantica "$CORPUS" > "$T/obiecta.out" 2> "$T/obiecta.err"
rco=$?
nudare () { grep -v -e '^tempus' -e ' ms' -e 'MB' "$1"; }
nudare "$T/unitas.out" > "$T/unitas.nuda"
nudare "$T/obiecta.out" > "$T/obiecta.nuda"
nudare "$T/plana.out" > "$T/plana.nuda"
n="$(grep -c . "$T/unitas.nuda")"
[ "$rcu" = "$rco" ] && [ "$n" -gt 0 ] \
    && cmp -s "$T/unitas.nuda" "$T/obiecta.nuda"
credo $? "III percursus -semantica $CORPUS: relatio eadem ($n lineae), exitus $rcu = $rco"
[ "$rcp" = "$rco" ] && cmp -s "$T/plana.nuda" "$T/obiecta.nuda"
credo $? "III unitas plana (frigida): relatio eadem, exitus $rcp = $rco"
if ! cmp -s "$T/unitas.nuda" "$T/obiecta.nuda"; then
    diff "$T/obiecta.nuda" "$T/unitas.nuda" | head -20 | sed 's/^/      /'
fi

if [ "$fracta" -eq 0 ]; then
    echo "UNITAS PERCURSUS: sana"
    exit 0
fi
echo "UNITAS PERCURSUS: FRACTA ($fracta)"
exit 1
