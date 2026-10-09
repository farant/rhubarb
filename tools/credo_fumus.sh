#!/bin/bash
# tools/credo_fumus.sh - porta SECTIONUM credo (credo v2: include/credo.h
# 'Sectiones'): suitae fictae compilantur et curruntur, plagula
# verdictorum (CREDO_VERDICTA) et effusio humana asseruntur.
#
#   I    suita mixta: TRANSIIT, FRACTA (cursus pergit), ABORTA per
#        NECESSE in adiutore (purgare tamen, sequentia non), VACUA,
#        titulus cum tabulatione effugitus, NECESSE in filio
#        CREDO_NON_RUIT (filius exit, suita reliqua SEMEL), exitus I
#   II   plagula: SECTIO X campi, SUITA VIII campi, linea per sectionem
#   III  suita sana: exitus 0, SUITA TRANSIIT, compendium ut olim
#   IV   filtrum CREDO_SECTIO: sectio sola; ignotum -> I, nominatum
#   V    sine CREDO_VERDICTA: nulla plagula
#   VI   ruina media: sectiones priores notae, linea SUITA absens
#
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
source tools/vexilla.sh
T="$(mktemp -d "${TMPDIR:-/tmp}/credo_fumus.XXXXXX")" || exit 2
trap 'rm -rf "${T:?}"' EXIT
fracta=0
credo () { if [ "$1" -eq 0 ]; then echo "  ok   $2"; else echo "  FRACTUM $2"; fracta=$((fracta + 1)); fi; }
struere () {   # $1 = fons in $T, $2 = binarium
    clang "${VEXILLA_C89[@]}" -Iinclude "$T/$1" lib/credo.c lib/piscina.c \
        lib/chorda.c lib/chorda_aedificator.c -o "$T/$2" > "$T/$2.err" 2>&1
}

cat > "$T/mixta.c" <<'FONS'
#include "credo.h"
#include <stdio.h>

interior vacuum _parare (CredoContextus* c)
{
    s32* numerus;

    numerus = (s32*)piscina_allocare(c->piscina, magnitudo(s32));
    *numerus = VII;
    c->datum = numerus;
}

interior vacuum _purgare (CredoContextus* c)
{
    imprimere("PURGARE %s\n", c->titulus);
}

interior vacuum _transiens (CredoContextus* c)
{
    CREDO_NON_NIHIL(c->datum);
    CREDO_AEQUALIS_S32(*(s32*)c->datum, VII);
}

interior vacuum _fracta (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
    CREDO_AEQUALIS_S32(I, II);
    CREDO_VERUM(II == II);
}

interior vacuum _adiutor (vacuum)
{
    vacuum* nullum;

    nullum = NIHIL;
    CREDO_NECESSE_NON_NIHIL(nullum);
    imprimere("NUMQUAM POST NECESSE\n");
}

interior vacuum _aborta (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
    _adiutor();
    imprimere("NUMQUAM IN SECTIONE\n");
}

interior vacuum _vacua (CredoContextus* c)
{
    (vacuum)c;
}

interior vacuum _tabulatio (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

interior vacuum _filius (CredoContextus* c)
{
    vacuum* nullum;

    (vacuum)c;
    nullum = NIHIL;
    CREDO_NON_RUIT(CREDO_NECESSE_NON_NIHIL(nullum));
    imprimere("POST FILIUM\n");
}

interior vacuum _ultima (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "transiens", _transiens, _parare, _purgare, NIHIL, NIHIL },
    { "fracta", _fracta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "aborta", _aborta, NIHIL, _purgare, NIHIL, NIHIL },
    { "vacua", _vacua, NIHIL, NIHIL, NIHIL, NIHIL },
    { "cum\ttabulatione", _tabulatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "filius", _filius, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ultima", _ultima, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("mixta", SECTIONES);
}
FONS

cat > "$T/sana.c" <<'FONS'
#include "credo.h"

interior vacuum _prima (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

interior vacuum _secunda (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(II == II);
    CREDO_VERUM(III == III);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "prima", _prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "secunda", _secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("sana", SECTIONES);
}
FONS

cat > "$T/ruina.c" <<'FONS'
#include "credo.h"
#include <signal.h>

interior vacuum _ante (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

interior vacuum _ruit (CredoContextus* c)
{
    (vacuum)c;
    /* SIGKILL, non abort(): abort() macOS buffra stdio effluit, ruina
     * vera (SEGV, KILL) nihil - planta 'sine fflush' sub abort()
     * viridis manebat */
    (vacuum)raise(SIGKILL);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ante", _ante, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ruit", _ruit, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("ruina", SECTIONES);
}
FONS

for s in mixta sana ruina; do
    struere "$s.c" "$s" || { echo "credo_fumus: $s non compilat:"; cat "$T/$s.err"; exit 2; }
done

# I + II: suita mixta
CREDO_VERDICTA="$T/v_mixta.tsv" "$T/mixta" > "$T/o_mixta" 2>&1; rc=$?
V="$T/v_mixta.tsv"
linea () { grep "^SECTIO	mixta	$1	" "$V"; }
[ "$rc" -eq 1 ]; credo $? "I suita mixta: exitus 1 (erat $rc)"
linea transiens | grep -q "	TRANSIIT	2	2	"; credo $? "I transiens: TRANSIIT 2/2, datum parare in piscina sectionis"
linea fracta | awk -F'\t' '$4 == "FRACTA" && $5 == 2 && $6 == 3 && $8 ~ /mixta\.c:[0-9]+$/ && $9 == "credo_aequalis" && $10 == "I == II"' | grep -q .; credo $? "I fracta: FRACTA 2/3, fractura prima nominata (filum:versus genus expressio), cursus pergit"
linea aborta | awk -F'\t' '$4 == "ABORTA" && $5 == 1 && $6 == 2 && $9 == "credo_necesse_non_nihil" && $10 == "nullum"' | grep -q .; credo $? "I aborta: ABORTA 1/2 per NECESSE in adiutore"
! grep -q 'NUMQUAM' "$T/o_mixta"; credo $? "I post NECESSE nihil currit (nec adiutor nec sectio)"
# puncta credo eandem lineam praecedunt: ancora sine '^'
grep -q 'PURGARE transiens$' "$T/o_mixta" && grep -q 'PURGARE aborta$' "$T/o_mixta"; credo $? "I purgare post transitum ET post abortum"
linea vacua | grep -q "	VACUA	0	0	"; credo $? "I vacua: VACUA 0/0"
grep -q "^SECTIO	mixta	cum\\\\ttabulatione	TRANSIIT	" "$V"; credo $? "I titulus cum tabulatione effugitus (\\\\t)"
[ "$(grep -c 'POST FILIUM$' "$T/o_mixta")" -eq 1 ] && [ "$(grep -c -- '--- Probans ultima ---' "$T/o_mixta")" -eq 1 ]; credo $? "I NECESSE in filio: filius exit, suita reliqua SEMEL (non in filio)"
linea filius | grep -q "	TRANSIIT	1	1	"; credo $? "I filius: CREDO_NON_RUIT transiit (filius normaliter exiit)"
grep -q "^SUITA	mixta	FRACTA	4	7	8	10	" "$V"; credo $? "I SUITA FRACTA 4/7 sectiones, 8/10 assertiones"
mali=$(awk -F'\t' '($1 == "SECTIO" && NF != 10) || ($1 == "SUITA" && NF != 8) || ($1 != "SECTIO" && $1 != "SUITA")' "$V" | wc -l | tr -d ' ')
[ "$mali" -eq 0 ] && [ "$(grep -c '^SECTIO	' "$V")" -eq 7 ]; credo $? "II campi: SECTIO X, SUITA VIII, VII lineae SECTIO ($mali malae)"
grep -q -- '--- Probans fracta ---' "$T/o_mixta" && grep -q ' FRACTA (credo_aequalis): I == II at ' "$T/o_mixta" \
    && grep -q '^=== SECTIONES: ' "$T/o_mixta" && grep -q '^Totalis:' "$T/o_mixta" && grep -q '^Conditio: FRACTA$' "$T/o_mixta"; credo $? "I effusio humana ut olim (Probans, FRACTA at, compendium)"

# III: suita sana
CREDO_VERDICTA="$T/v_sana.tsv" "$T/sana" > "$T/o_sana" 2>&1; rc=$?
[ "$rc" -eq 0 ] && grep -q "^SUITA	sana	TRANSIIT	2	2	3	3	" "$T/v_sana.tsv" && grep -q '^Conditio: OMNIA PRAETERIERUNT$' "$T/o_sana"; credo $? "III suita sana: exitus 0, SUITA TRANSIIT 2/2 3/3 (erat $rc)"

# IV: filtrum
CREDO_SECTIO=secunda CREDO_VERDICTA="$T/v_filtrum.tsv" "$T/sana" > "$T/o_filtrum" 2>&1; rc=$?
[ "$rc" -eq 0 ] && [ "$(grep -c '^SECTIO	' "$T/v_filtrum.tsv")" -eq 1 ] && grep -q "^SECTIO	sana	secunda	TRANSIIT	2	2	" "$T/v_filtrum.tsv" \
    && ! grep -q -- '--- Probans prima ---' "$T/o_filtrum"; credo $? "IV CREDO_SECTIO: sectio sola cursa et notata (erat $rc)"
CREDO_SECTIO=nulla CREDO_VERDICTA="$T/v_nulla.tsv" "$T/sana" > "$T/o_nulla" 2>&1; rc=$?
[ "$rc" -eq 1 ] && grep -q "CREDO_SECTIO 'nulla'" "$T/o_nulla" && ! grep -q '^SECTIO' "$T/v_nulla.tsv" \
    && grep -q "^SUITA	sana	FRACTA	0	0	" "$T/v_nulla.tsv"; credo $? "IV filtrum ignotum: exitus 1, nominatum, nihil cursum (erat $rc)"

# V: sine CREDO_VERDICTA
(cd "$T" && env -u CREDO_VERDICTA ./sana > o_sine 2>&1); rc=$?
[ "$rc" -eq 0 ] && [ -z "$(find "$T" -newer "$T/sana" -name '*.tsv' ! -name 'v_*')" ]; credo $? "V sine CREDO_VERDICTA: nulla plagula, exitus 0"

# VI: ruina media
# subshell: nuntius "Killed" testae ipsius tacetur (ruina consulta)
( CREDO_VERDICTA="$T/v_ruina.tsv" "$T/ruina" > "$T/o_ruina" 2>&1; exit $? ) 2>/dev/null; rc=$?
[ "$rc" -ne 0 ] && grep -q "^SECTIO	ruina	ante	TRANSIIT	1	1	" "$T/v_ruina.tsv" && ! grep -q '^SUITA' "$T/v_ruina.tsv"; credo $? "VI ruina: sectio prior nota, SUITA absens (erat $rc)"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus credo: sanum"; exit 0; fi
echo "fumus credo: FRACTUM ($fracta)"; exit 1
