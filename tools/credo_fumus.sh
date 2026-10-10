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
#   VII  signa (credo II): vocabulum clausum (lentus rete fenestra
#        repositorium), CREDO_SIGNA_OMITTE -> OMISSA nominata, omnes
#        omissae -> VACUA, signum ignotum (tabula aut ambitus) -> FRACTA
#   VIII fracturae exspectatae (credo II): causa in fonte; fracta ->
#        NOTA_FRACTA (suita transit), transiens -> INOPINATA (suita
#        FRACTA), causa vacua recusata
#   X    gradus (credo II): CREDO_GRADUS nominat; fractura prima eum fert
#        (effusio et campus XI SECTIO)
#   IX   fixura suitae (credo II): credo_suitam_currere_cum - parare et
#        purgare suitae SEMEL, c->suita in sectionibus, purgare post
#        abortum et post parare fractum
#   XII  ordo (credo II): CREDO_ORDO=mixtus:<semen> permutatio
#        reproducibilis, semen nominatum (effusio + linea ORDO);
#        'mixtus' sine semine semen eligit et nominat; valor ignotus ->
#        I, nihil cursum; status communis occultus ordine mixto patet
#   XIII convertor (credo II T8): tools/credo_convertere.py - formae A
#        (titulus in bloco) et B (titulus ante sectionem; titulus in bloco
#        -> CREDO_GRADUS) convertuntur, COMPILANTUR, currunt; recusationes
#        nominatae plagulam intactam relinquunt; -probare nihil scribit;
#        conversa -> exitus 2; xar/stml vetera == exspectata octetim;
#        nota commissionis (lint 5) solum pro convertibili
#   XI   area (credo II): credo_area(c) pigra sub TMPDIR
#        (<suita>.<n>.<pid>, suitae <suita>.suita.<pid>), eadem in
#        sectione, alia in alia, deleta post purgare (etiam post NECESSE
#        et in filio creata), nulla sine usu
#
# Exitus 0 sanum | 1 FRACTUM | 2 nihil actum.
set -u
RADIX="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$RADIX" || exit 2
source tools/vexilla.sh
T="$(mktemp -d "${TMPDIR:-/tmp}/credo_fumus.XXXXXX")" || exit 2
# CREDO_FUMUS_SERVA=1: $T servatur (inspectio effusionum)
trap '[ -n "${CREDO_FUMUS_SERVA:-}" ] || rm -rf "${T:?}"' EXIT
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

cat > "$T/signata.c" <<'FONS'
#include "credo.h"

interior vacuum _transit (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "localis", _transit, NIHIL, NIHIL, NIHIL, NIHIL },
    { "retis", _transit, NIHIL, NIHIL, "rete", NIHIL },
    { "lenta", _transit, NIHIL, NIHIL, "lentus rete", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("signata", SECTIONES);
}
FONS

cat > "$T/omnes_signatae.c" <<'FONS'
#include "credo.h"

interior vacuum _transit (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "prima", _transit, NIHIL, NIHIL, "rete", NIHIL },
    { "secunda", _transit, NIHIL, NIHIL, "rete", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("omnes_signatae", SECTIONES);
}
FONS

cat > "$T/ignota.c" <<'FONS'
#include "credo.h"

interior vacuum _transit (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "prima", _transit, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tarda", _transit, NIHIL, NIHIL, "tardus", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("ignota", SECTIONES);
}
FONS

cat > "$T/nota.c" <<'FONS'
#include "credo.h"

interior vacuum _transit (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

interior vacuum _fracta (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
    CREDO_AEQUALIS_S32(I, II);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "bona", _transit, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nota", _fracta, NIHIL, NIHIL, NIHIL,
      "I != II nondum (tabularium ...ABCDEF)" },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("nota", SECTIONES);
}
FONS

cat > "$T/sanata.c" <<'FONS'
#include "credo.h"

interior vacuum _transit (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "sanata", _transit, NIHIL, NIHIL, NIHIL, "olim fracta" },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("sanata", SECTIONES);
}
FONS

cat > "$T/causa_vacua.c" <<'FONS'
#include "credo.h"

interior vacuum _transit (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "prima", _transit, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine_causa", _transit, NIHIL, NIHIL, NIHIL, "" },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("causa_vacua", SECTIONES);
}
FONS

cat > "$T/suita_communis.c" <<'FONS'
#include "credo.h"
#include <stdio.h>

interior vacuum _parare_suitae (CredoContextus* c)
{
    s32* n;

    n = (s32*)piscina_allocare(c->piscina, magnitudo(s32));
    CREDO_NECESSE_NON_NIHIL(n);
    *n = XLII;
    c->datum = n;
    imprimere("PARARE SUITAE\n");
}

interior vacuum _purgare_suitae (CredoContextus* c)
{
    imprimere("PURGARE SUITAE %d\n", c->datum != NIHIL
        ? (integer)*(s32*)c->datum : -I);
}

interior vacuum _legit (CredoContextus* c)
{
    CREDO_NECESSE_NON_NIHIL(c->suita);
    CREDO_AEQUALIS_S32(*(s32*)c->suita, XLII);
}

interior vacuum _abortat (CredoContextus* c)
{
    (vacuum)c;
    CREDO_NECESSE(FALSUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "prima", _legit, NIHIL, NIHIL, NIHIL, NIHIL },
    { "secunda", _abortat, NIHIL, NIHIL, NIHIL, NIHIL },
    { "tertia", _legit, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere_cum("suita_communis", SECTIONES,
        _parare_suitae, _purgare_suitae);
}
FONS

cat > "$T/suita_fracta.c" <<'FONS'
#include "credo.h"
#include <stdio.h>

interior vacuum _parare_suitae (CredoContextus* c)
{
    (vacuum)c;
    CREDO_NECESSE(I == II);
}

interior vacuum _purgare_suitae (CredoContextus* c)
{
    (vacuum)c;
    imprimere("PURGARE SUITAE FRACTAE\n");
}

interior vacuum _legit (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "prima", _legit, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere_cum("suita_fracta", SECTIONES,
        _parare_suitae, _purgare_suitae);
}
FONS

cat > "$T/gradus.c" <<'FONS'
#include "credo.h"

interior vacuum _scaena (CredoContextus* c)
{
    (vacuum)c;
    CREDO_GRADUS("I: primus");
    CREDO_VERUM(I == I);
    CREDO_GRADUS("II: secundus");
    CREDO_AEQUALIS_S32(I, II);
    CREDO_GRADUS("III: tertius");
    CREDO_VERUM(I == III);
}

interior vacuum _sine_gradu (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(I == II);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "scaena", _scaena, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine_gradu", _sine_gradu, NIHIL, NIHIL, NIHIL, NIHIL },
    { "post", _sine_gradu, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("gradus", SECTIONES);
}
FONS

cat > "$T/area.c" <<'FONS'
#include "postulata_posix.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/* copiae viarum (area post sectionem deletur): prior, suitae, necesse */
hic_manens character _prior[DXII];
hic_manens character _suitae[DXII];
hic_manens character _necesse_via[DXII];

interior b32 _adest (constans character* via)
{
    structura stat st;

    redde stat(via, &st) == ZEPHYRUM && S_ISDIR(st.st_mode);
}

interior vacuum _scribere (constans character* dir, constans character* titulus)
{
    character via[DXII];
    FILE* f;

    sprintf(via, "%s/%s", dir, titulus);
    f = fopen(via, "w");
    si (f != NIHIL) { fputs("x\n", f); fclose(f); }
}

interior vacuum _parare_suitae (CredoContextus* c)
{
    constans character* a = credo_area(c);

    CREDO_NECESSE_NON_NIHIL(a);
    _scribere(a, "suitae.txt");
    imprimere("AREA suita %s\n", a);
    sprintf(_suitae, "%s", a);
}

interior vacuum _purgare_suitae (CredoContextus* c)
{
    (vacuum)c;
    imprimere("PURGARE SUITAE %s\n", _adest(_suitae) ? "adest" : "abest");
}

interior vacuum _prima (CredoContextus* c)
{
    constans character* a = credo_area(c);

    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(_adest(a));
    CREDO_VERUM(strcmp(credo_area(c), a) == ZEPHYRUM);
    CREDO_VERUM(strcmp(a, _suitae) != ZEPHYRUM);
    CREDO_VERUM(_adest(_suitae));
    _scribere(a, "prima.txt");
    sprintf(_prior, "%s", a);
    imprimere("AREA prima %s\n", a);
}

interior vacuum _secunda (CredoContextus* c)
{
    constans character* a = credo_area(c);

    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(strcmp(a, _prior) != ZEPHYRUM);
    CREDO_FALSUM(_adest(_prior));
    imprimere("AREA secunda %s\n", a);
}

/* NECESSE post usum: area in purgare adhuc adest, post eam deletur */
interior vacuum _necesse (CredoContextus* c)
{
    constans character* a = credo_area(c);
    character sub[DXII];

    CREDO_NECESSE_NON_NIHIL(a);
    sprintf(sub, "%s/profundum", a);
    (vacuum)mkdir(sub, 0700);
    _scribere(sub, "f.txt");
    imprimere("AREA necesse %s\n", a);
    sprintf(_necesse_via, "%s", a);
    CREDO_NECESSE(FALSUM);
}

interior vacuum _purgare_necesse (CredoContextus* c)
{
    (vacuum)c;
    imprimere("PURGARE necesse %s\n", _adest(_necesse_via) ? "adest"
        : "abest");
}

/* area in FILIO solo creata (CREDO_NON_RUIT): parens eam tamen delet */
interior vacuum _in_filio_corpus (CredoContextus* c)
{
    constans character* a = credo_area(c);

    /* filius per _exit finit: effusio statim effluenda */
    si (a != NIHIL) { _scribere(a, "filius.txt"); imprimere("AREA filius %s\n", a); }
    (vacuum)fflush(stdout);
}

interior vacuum _in_filio (CredoContextus* c)
{
    CREDO_NON_RUIT(_in_filio_corpus(c));
}

interior vacuum _sine (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(VERUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "prima", _prima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "secunda", _secunda, NIHIL, NIHIL, NIHIL, NIHIL },
    { "necesse", _necesse, NIHIL, _purgare_necesse, NIHIL, NIHIL },
    { "in_filio", _in_filio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine", _sine, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere_cum("area fumi", SECTIONES, _parare_suitae,
        _purgare_suitae);
}
FONS

cat > "$T/ordo.c" <<'FONS'
#include "credo.h"
#include <stdio.h>

interior vacuum _nominare (CredoContextus* c)
{
    imprimere("ORDO-SECTIO %s\n", c->titulus);
    CREDO_VERUM(VERUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "a", _nominare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "b", _nominare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "c", _nominare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "d", _nominare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "e", _nominare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("ordo", SECTIONES);
}
FONS

# status communis OCCULTUS: 'legens' innititur 'ponenti' ante se cursae
cat > "$T/dependens.c" <<'FONS'
#include "credo.h"

hic_manens s32 _communis = ZEPHYRUM;

interior vacuum _ponens (CredoContextus* c)
{
    (vacuum)c;
    _communis = VII;
    CREDO_VERUM(VERUM);
}

interior vacuum _legens (CredoContextus* c)
{
    (vacuum)c;
    CREDO_AEQUALIS_S32(_communis, VII);
}

interior vacuum _alia (CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM(VERUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "ponens", _ponens, NIHIL, NIHIL, NIHIL, NIHIL },
    { "alia", _alia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "legens", _legens, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32 principale (vacuum)
{
    redde credo_suitam_currere("dependens", SECTIONES);
}
FONS

for s in mixta sana ruina signata omnes_signatae ignota nota sanata causa_vacua suita_communis suita_fracta gradus area ordo dependens; do
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
# chorda vacua -> chorda_ut_cstr NIHIL -> printf("%s", NULL) (UB; macOS
# "(null)"): Speratus/Receptus vacui scribuntur vacui (credo II T4)
! grep -q '(null)' "$T/o_mixta"; credo $? "I Speratus/Receptus vacui numquam '(null)' (NECESSE)"
# puncta credo eandem lineam praecedunt: ancora sine '^'
grep -q 'PURGARE transiens$' "$T/o_mixta" && grep -q 'PURGARE aborta$' "$T/o_mixta"; credo $? "I purgare post transitum ET post abortum"
linea vacua | grep -q "	VACUA	0	0	"; credo $? "I vacua: VACUA 0/0"
grep -q "^SECTIO	mixta	cum\\\\ttabulatione	TRANSIIT	" "$V"; credo $? "I titulus cum tabulatione effugitus (\\\\t)"
[ "$(grep -c 'POST FILIUM$' "$T/o_mixta")" -eq 1 ] && [ "$(grep -c -- '--- Probans ultima ---' "$T/o_mixta")" -eq 1 ]; credo $? "I NECESSE in filio: filius exit, suita reliqua SEMEL (non in filio)"
linea filius | grep -q "	TRANSIIT	1	1	"; credo $? "I filius: CREDO_NON_RUIT transiit (filius normaliter exiit)"
grep -q "^SUITA	mixta	FRACTA	4	7	8	10	" "$V"; credo $? "I SUITA FRACTA 4/7 sectiones, 8/10 assertiones"
mali=$(awk -F'\t' '($1 == "SECTIO" && NF != 11) || ($1 == "SUITA" && NF != 8) || ($1 != "SECTIO" && $1 != "SUITA")' "$V" | wc -l | tr -d ' ')
[ "$mali" -eq 0 ] && [ "$(grep -c '^SECTIO	' "$V")" -eq 7 ]; credo $? "II campi: SECTIO XI (gradus, credo II), SUITA VIII, VII lineae SECTIO ($mali malae)"
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

# VII (credo II, T1): SIGNA - vocabulum clausum, OMISSA per
# CREDO_SIGNA_OMITTE (nominata, numquam tacita), omnes omissae = VACUA
env -u CREDO_SIGNA_OMITTE CREDO_VERDICTA="$T/v_sig.tsv" "$T/signata" > "$T/o_sig" 2>&1; rc=$?
[ "$rc" -eq 0 ] && [ "$(grep -c '	TRANSIIT	' "$T/v_sig.tsv")" -eq 4 ]; credo $? "VII signa sine omissione: omnes currunt (erat $rc)"
CREDO_SIGNA_OMITTE=rete CREDO_VERDICTA="$T/v_om.tsv" "$T/signata" > "$T/o_om" 2>&1; rc=$?
V="$T/v_om.tsv"
[ "$rc" -eq 0 ] && grep -q "^SECTIO	signata	localis	TRANSIIT	1	1	" "$V" \
    && grep -q "^SECTIO	signata	retis	OMISSA	0	0	" "$V" && grep -q "^SECTIO	signata	lenta	OMISSA	0	0	" "$V" \
    && grep -q "^SUITA	signata	TRANSIIT	1	1	" "$V"; credo $? "VII CREDO_SIGNA_OMITTE=rete: II OMISSA in plagula, suita transit cursa I (erat $rc)"
mali=$(awk -F'\t' '($1 == "SECTIO" && NF != 11) || ($1 == "SUITA" && NF != 8)' "$V" | wc -l | tr -d ' ')
[ "$mali" -eq 0 ]; credo $? "VII lineae OMISSA campos XI ferunt ($mali malae)"
grep -q "OMISSA retis (signum rete)" "$T/o_om" && grep -q "OMISSA lenta (signum rete)" "$T/o_om" \
    && ! grep -q -- '--- Probans retis ---' "$T/o_om"; credo $? "VII omissa in effusione nominatae, non cursae"
CREDO_SIGNA_OMITTE=rete CREDO_VERDICTA="$T/v_vac.tsv" "$T/omnes_signatae" > "$T/o_vac" 2>&1; rc=$?
[ "$rc" -eq 1 ] && grep -q "^SUITA	omnes_signatae	VACUA	0	0	" "$T/v_vac.tsv"; credo $? "VII omnes omissae: SUITA VACUA, exitus 1 (erat $rc)"
CREDO_VERDICTA="$T/v_ign.tsv" "$T/ignota" > "$T/o_ign" 2>&1; rc=$?
[ "$rc" -eq 1 ] && grep -q "signum ignotum 'tardus'" "$T/o_ign" && grep -q "tarda" "$T/o_ign" \
    && ! grep -q '^SECTIO' "$T/v_ign.tsv" && grep -q "^SUITA	ignota	FRACTA	0	0	" "$T/v_ign.tsv"; credo $? "VII signum ignotum in tabula: FRACTA nominatum, nihil cursum (erat $rc)"
CREDO_SIGNA_OMITTE=retis CREDO_VERDICTA="$T/v_ign2.tsv" "$T/signata" > "$T/o_ign2" 2>&1; rc=$?
[ "$rc" -eq 1 ] && grep -q "CREDO_SIGNA_OMITTE: signum ignotum 'retis'" "$T/o_ign2" && ! grep -q '^SECTIO' "$T/v_ign2.tsv"; credo $? "VII signum ignotum in CREDO_SIGNA_OMITTE: FRACTA, nihil cursum (erat $rc)"

# VIII (credo II, T2): FRACTURAE EXSPECTATAE - causa in fonte;
# fracta -> NOTA_FRACTA (suita transit), transiens -> INOPINATA (suita
# FRACTA), causa vacua recusatur
CREDO_VERDICTA="$T/v_nota.tsv" "$T/nota" > "$T/o_nota" 2>&1; rc=$?
V="$T/v_nota.tsv"
[ "$rc" -eq 0 ] && grep -q "^SECTIO	nota	bona	TRANSIIT	1	1	" "$V" \
    && awk -F'\t' '$1 == "SECTIO" && $3 == "nota" && $4 == "NOTA_FRACTA" && $5 == 1 && $6 == 2 && $9 == "credo_aequalis"' "$V" | grep -q . \
    && grep -q "^SUITA	nota	TRANSIIT	2	2	" "$V"; credo $? "VIII fracta exspectata: NOTA_FRACTA cum fractura, suita TRANSIIT, exitus 0 (erat $rc)"
grep -q "sectio nota: NOTA_FRACTA (1/2) - causa: I != II nondum (tabularium ...ABCDEF)" "$T/o_nota" \
    && ! grep -q '^Conditio: FRACTA$' "$T/o_nota" && grep -q 'exspectat' "$T/o_nota"; credo $? "VIII effusio: causa nominata, compendium non 'FRACTA' sed exspectatas nominans"
CREDO_VERDICTA="$T/v_san.tsv" "$T/sanata" > "$T/o_san" 2>&1; rc=$?
[ "$rc" -eq 1 ] && grep -q "^SECTIO	sanata	sanata	INOPINATA	1	1	" "$T/v_san.tsv" \
    && grep -q "^SUITA	sanata	FRACTA	0	1	" "$T/v_san.tsv" \
    && grep -q "INOPINATA" "$T/o_san" && grep -q "olim fracta" "$T/o_san" && grep -q "delenda" "$T/o_san"; credo $? "VIII fractura exspectata sanata: INOPINATA, suita FRACTA, linea delenda nominata (erat $rc)"
CREDO_VERDICTA="$T/v_cv.tsv" "$T/causa_vacua" > "$T/o_cv" 2>&1; rc=$?
[ "$rc" -eq 1 ] && grep -q "exspectata_fracta vacua in sectione sine_causa" "$T/o_cv" \
    && ! grep -q '^SECTIO' "$T/v_cv.tsv" && grep -q "^SUITA	causa_vacua	FRACTA	0	0	" "$T/v_cv.tsv"; credo $? "VIII causa vacua: recusata ante cursum, nominata (erat $rc)"

# census fracturarum exspectatarum ex repositorio (silva, sine tabulario)
tools/credo_fracturae_notae.py "$T/nota.c" "$T/sanata.c" > "$T/o_census" 2>&1; rc=$?
[ "$rc" -eq 0 ] && grep -q "nota.c:[0-9]*	nota	I != II nondum (tabularium ...ABCDEF)$" "$T/o_census" \
    && grep -q "sanata.c:[0-9]*	sanata	olim fracta$" "$T/o_census" \
    && grep -q "^fracturae exspectatae: 2$" "$T/o_census" && ! grep -q "	bona	" "$T/o_census"; credo $? "VIII census: II fracturae exspectatae cum sede et causa (silva), bona non (erat $rc)"

# IX (credo II, T3): FIXURA SUITAE - parare_suitae SEMEL, datum in
# c->suita omnium sectionum, purgare_suitae SEMEL post ultimam (etiam
# post sectionem abortam); parare_suitae fracta -> nulla sectio, purgare
# tamen
CREDO_VERDICTA="$T/v_sc.tsv" "$T/suita_communis" > "$T/o_sc" 2>&1; rc=$?
V="$T/v_sc.tsv"
[ "$rc" -eq 1 ] && [ "$(grep -c 'PARARE SUITAE$' "$T/o_sc")" -eq 1 ] \
    && [ "$(grep -c 'PURGARE SUITAE 42$' "$T/o_sc")" -eq 1 ]; credo $? "IX parare_suitae et purgare_suitae SEMEL, datum servatum (erat $rc)"
grep -q "^SECTIO	suita_communis	prima	TRANSIIT	2	2	" "$V" \
    && grep -q "^SECTIO	suita_communis	secunda	ABORTA	" "$V" \
    && grep -q "^SECTIO	suita_communis	tertia	TRANSIIT	2	2	" "$V"; credo $? "IX c->suita in omni sectione, etiam post abortam"
awk '/--- Probans tertia ---/ {t = NR} /PURGARE SUITAE 42$/ {p = NR} END {exit !(t && p > t)}' "$T/o_sc"; credo $? "IX purgare_suitae post sectionem ultimam"
CREDO_VERDICTA="$T/v_sf.tsv" "$T/suita_fracta" > "$T/o_sf" 2>&1; rc=$?
[ "$rc" -eq 1 ] && ! grep -q -- '--- Probans prima ---' "$T/o_sf" && grep -q 'PURGARE SUITAE FRACTAE$' "$T/o_sf" \
    && grep -q "^SECTIO	suita_fracta	(parare suitae)	ABORTA	" "$T/v_sf.tsv" \
    && grep -q "^SUITA	suita_fracta	FRACTA	" "$T/v_sf.tsv"; credo $? "IX parare_suitae fracta: nulla sectio, notata, purgare tamen (erat $rc)"

# X (credo II, T4): GRADUS - CREDO_GRADUS gradum currentem nominat;
# fractura PRIMA eum fert (effusio et campus XI plagulae); sine gradu
# vacuus; gradus inter sectiones non manet
CREDO_VERDICTA="$T/v_gr.tsv" "$T/gradus" > "$T/o_gr" 2>&1; rc=$?
V="$T/v_gr.tsv"
[ "$rc" -eq 1 ] && awk -F'\t' '$1 == "SECTIO" && $3 == "scaena" && $4 == "FRACTA" && $5 == 1 && $6 == 3 && $10 == "I == II" && $11 == "II: secundus"' "$V" | grep -q .; credo $? "X fractura prima gradum suum fert (campus XI) (erat $rc)"
awk -F'\t' '$1 == "SECTIO" && ($3 == "sine_gradu" || $3 == "post") && NF == 11 && $11 == ""' "$V" | wc -l | grep -q '^ *2$'; credo $? "X sine gradu campus vacuus; gradus sectionis prioris non manet"
grep -q ' FRACTA (credo_aequalis): I == II at .*gradus.c:[0-9]* (in gradu II: secundus)$' "$T/o_gr" \
    && grep -q -- '-- gradus I: primus --' "$T/o_gr" && grep -q -- '-- gradus III: tertius --' "$T/o_gr"; credo $? "X effusio: gradus nominati, fractura 'in gradu'"

# XI (credo II, T5): AREA - credo_area(c) pigra sub TMPDIR, deleta post
# purgare (sectionis aut suitae), etiam post NECESSE et in filio creata;
# TMPDIR proprium: post cursum VACUUM esse debet
mkdir -p "$T/tmp_area/"
TMPDIR="$T/tmp_area/" CREDO_VERDICTA="$T/v_ar.tsv" "$T/area" > "$T/o_ar" 2>&1; rc=$?
V="$T/v_ar.tsv"
[ "$rc" -eq 1 ] && grep -q "^SECTIO	area fumi	prima	TRANSIIT	5	5	" "$V" && grep -q "^SECTIO	area fumi	secunda	TRANSIIT	3	3	" "$V" \
    && grep -q "^SECTIO	area fumi	necesse	ABORTA	" "$V" && grep -q "^SECTIO	area fumi	sine	TRANSIIT	" "$V"; credo $? "XI sectiones: eadem area in sectione, alia in alia, prior deleta (erat $rc)"
grep -q "AREA prima $T/tmp_area/area_fumi\.1\.[0-9]*$" "$T/o_ar" && grep -q "AREA secunda $T/tmp_area/area_fumi\.2\.[0-9]*$" "$T/o_ar" \
    && grep -q "AREA suita $T/tmp_area/area_fumi\.suita\.[0-9]*$" "$T/o_ar"; credo $? "XI via: <TMPDIR>/<suita>.<n>.<pid> ('/' finalis demptus, titulus sanatus), suitae .suita."
grep -q 'PURGARE necesse adest$' "$T/o_ar" && grep -q 'PURGARE SUITAE adest$' "$T/o_ar" && grep -q 'AREA filius ' "$T/o_ar"; credo $? "XI area in purgare adhuc adest (sectio et suita); filius aream creavit"
[ -z "$(ls -A "$T/tmp_area")" ]; credo $? "XI post cursum TMPDIR vacuum: omnes areae deletae (NECESSE, filius, suita, profunda) - reliquiae: $(ls -A "$T/tmp_area" | tr '\n' ' ')"

# XII (credo II, T6): ORDO - permutatio reproducibilis per semen; semen
# nominatum; valor ignotus recusatus; ordo tabulae sine CREDO_ORDO
ordo_legere () { grep -o 'ORDO-SECTIO [a-e]$' "$1" | cut -d' ' -f2 | tr -d '\n'; }
"$T/ordo" > "$T/o_o0" 2>&1
[ "$(ordo_legere "$T/o_o0")" = "abcde" ] && ! grep -q 'ORDO mixtus' "$T/o_o0"; credo $? "XII sine CREDO_ORDO: ordo tabulae, nihil nominatum"
CREDO_ORDO=mixtus:7 CREDO_VERDICTA="$T/v_o7.tsv" "$T/ordo" > "$T/o_o7" 2>&1; rc7=$?
CREDO_ORDO=mixtus:7 "$T/ordo" > "$T/o_o7b" 2>&1
o7="$(ordo_legere "$T/o_o7")"
[ "$rc7" -eq 0 ] && [ "$o7" = "$(ordo_legere "$T/o_o7b")" ] && [ "$(echo "$o7" | fold -w1 | sort | tr -d '\n')" = "abcde" ]; credo $? "XII mixtus:7 permutatio (omnes semel), idem bis ($o7)"
# ordines DISTINCTI numerantur (semen neglectum = ordo unus semper)
for semen in 1 2 3 4 5 6 8 9; do
    CREDO_ORDO=mixtus:$semen "$T/ordo" > "$T/o_ox" 2>&1; ordo_legere "$T/o_ox"; echo
done | sort -u > "$T/ordines"
diversi="$(grep -c . "$T/ordines")"
[ "$diversi" -ge 5 ]; credo $? "XII semina alia ordines alios dant ($diversi/8 distincti)"
grep -q '=== ORDO mixtus semen 7 ===' "$T/o_o7" && grep -q "^ORDO	ordo	mixtus	7$" "$T/v_o7.tsv" \
    && [ "$(grep -c '^SECTIO	' "$T/v_o7.tsv")" -eq 5 ] && grep -q '^SUITA	ordo	TRANSIIT	5	5	' "$T/v_o7.tsv"; credo $? "XII semen in effusione et linea ORDO plagulae; SECTIO V, SUITA TRANSIIT"
CREDO_ORDO=mixtus "$T/ordo" > "$T/o_oa" 2>&1
semen="$(sed -n 's/^=== ORDO mixtus semen \([0-9]*\) ===$/\1/p' "$T/o_oa")"
[ -n "$semen" ] && [ "$(ordo_legere "$T/o_oa")" = "$(CREDO_ORDO=mixtus:$semen "$T/ordo" 2>&1 > "$T/o_ob"; ordo_legere "$T/o_ob")" ]; credo $? "XII 'mixtus' sine semine: semen electum nominatur et ordinem reddit (semen $semen)"
CREDO_ORDO=confusus "$T/ordo" > "$T/o_oi" 2>&1; rci=$?
[ "$rci" -eq 1 ] && grep -q "CREDO_ORDO ignotus 'confusus'" "$T/o_oi" && ! grep -q 'ORDO-SECTIO' "$T/o_oi"; credo $? "XII valor ignotus: exitus 1, nominatus, nihil cursum"
"$T/dependens" > /dev/null 2>&1; rcd=$?
fractum=""; for semen in $(seq 1 30); do
    CREDO_ORDO=mixtus:$semen "$T/dependens" > /dev/null 2>&1 || { fractum=$semen; break; }
done
[ "$rcd" -eq 0 ] && [ -n "$fractum" ]; credo $? "XII status communis occultus: ordine tabulae transit, ordine mixto (semen $fractum) frangitur"

# XIII (credo II, T8): CONVERTOR - structura per silvam, textus verbatim
mkdir -p "$T/conv"
vetus () {   # $1 = titulus, stdin = corpus inter credo_aperire et caudam
    { cat <<'CAPUT'
#include "latina.h"
#include "credo.h"
#include "piscina.h"

s32 principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("conv", 4096);
    credo_aperire(piscina);

CAPUT
    cat
    cat <<'CAUDA'

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
CAUDA
    } > "$T/conv/probatio_$1.c"
}
vetus forma_a <<'C'
    {
        imprimere("\n--- Probans prima ---\n");
        CREDO_VERUM(VERUM);
    }

    /* commentum secundae */
    {
        imprimere("\n--- Probans secunda ---\n");

        CREDO_NON_NIHIL(piscina);
    }
C
vetus forma_b <<'C'
    imprimere("\n--- Probans prima ---\n");
    CREDO_VERUM(VERUM);
    {
        imprimere("\n--- Probans gradus internus ---\n");
        CREDO_VERUM(I == I);
    }

    imprimere("\n--- Probans titulus"
              " fissus ---\n");
    CREDO_NON_NIHIL(piscina);
C
vetus inter <<'C'
    {
        imprimere("\n--- Probans prima ---\n");
        CREDO_VERUM(VERUM);
    }
    praeteritus = VERUM;
    {
        imprimere("\n--- Probans secunda ---\n");
        CREDO_VERUM(praeteritus);
    }
C
vetus sine_titulo <<'C'
    {
        imprimere("\n--- Probans prima ---\n");
        CREDO_VERUM(VERUM);
    }
    {
        CREDO_VERUM(VERUM);
    }
C
vetus redde <<'C'
    {
        imprimere("\n--- Probans prima ---\n");
        si (piscina == NIHIL) { redde I; }
        CREDO_VERUM(VERUM);
    }
C
vetus duplices <<'C'
    {
        imprimere("\n--- Probans eadem ---\n");
        CREDO_VERUM(VERUM);
    }
    {
        imprimere("\n--- Probans eadem ---\n");
        CREDO_VERUM(VERUM);
    }
C
# communis: variabilis principalis in sectionibus (status communis)
sed 's/        b32  praeteritus;/        b32  praeteritus;\n        s32  numerus = ZEPHYRUM;/' "$T/conv/probatio_forma_a.c" \
    | sed 's/CREDO_VERUM(VERUM);/numerus++; CREDO_VERUM(numerus > 0);/' > "$T/conv/probatio_communis.c"
CONV=./tools/credo_convertere.py
for f in forma_a forma_b; do
    cp "$T/conv/probatio_$f.c" "$T/conv/$f.orig"
    $CONV "$T/conv/probatio_$f.c" > "$T/conv/$f.out" 2>&1; rcc=$?
    struere "conv/probatio_$f.c" "conv_$f" || { cat "$T/conv_$f.err" | head -5; }
    CREDO_VERDICTA="$T/conv/v_$f.tsv" "$T/conv_$f" > "$T/conv/o_$f" 2>&1; rcr=$?
    echo "$f $rcc $rcr" >> "$T/conv/rc"
done
read -r _ ca ra < <(grep '^forma_a ' "$T/conv/rc"); read -r _ cb rb < <(grep '^forma_b ' "$T/conv/rc")
[ "$ca" -eq 0 ] && [ "$ra" -eq 0 ] && grep -q "^SUITA	forma_a	TRANSIIT	2	2	" "$T/conv/v_forma_a.tsv" \
    && grep -q '^_sectio_prima ($' "$T/conv/probatio_forma_a.c" && grep -q '^    /\* commentum secundae \*/$' "$T/conv/probatio_forma_a.c" \
    && ! grep -q 'Probans' "$T/conv/probatio_forma_a.c"; credo $? "XIII forma A: conversa, compilata, SUITA TRANSIIT 2/2, nomina ordinalia, vexillum servatum (rc $ca/$ra)"
[ "$cb" -eq 0 ] && [ "$rb" -eq 0 ] && grep -q "^SUITA	forma_b	TRANSIIT	2	2	" "$T/conv/v_forma_b.tsv" \
    && grep -q 'CREDO_GRADUS("gradus internus");' "$T/conv/probatio_forma_b.c" \
    && grep -q '{ "titulus fissus", _sectio_secunda,' "$T/conv/probatio_forma_b.c"; credo $? "XIII forma B: titulus in bloco -> CREDO_GRADUS, titulus fissus = sectio sua; currit (rc $cb/$rb)"
recusa () {   # $1 = fixura, $2 = causa (substringa)
    cp "$T/conv/probatio_$1.c" "$T/conv/$1.orig"
    $CONV "$T/conv/probatio_$1.c" > "$T/conv/$1.out" 2>&1; rcx=$?
    [ "$rcx" -eq 1 ] && grep -q "recusata: .* - .*$2" "$T/conv/$1.out" && cmp -s "$T/conv/probatio_$1.c" "$T/conv/$1.orig"
    credo $? "XIII recusata '$1': exitus 1, causa '$2', plagula intacta (rc $rcx: $(cut -c1-90 "$T/conv/$1.out" | sed 's/.* - //'))"
}
recusa inter "statutum inter blocos"
recusa sine_titulo "tituli 0"
recusa redde "redde/salta intra sectionem"
recusa duplices "tituli duplices"
recusa communis "variabilis principalis 'numerus'"
cp "$T/conv/forma_a.orig" "$T/conv/probatio_probare.c"
$CONV -probare "$T/conv/probatio_probare.c" > "$T/conv/probare.out" 2>&1; rcp=$?
[ "$rcp" -eq 0 ] && grep -q '^convertibilis: .*(forma A, sectiones 2)$' "$T/conv/probare.out" && cmp -s "$T/conv/probatio_probare.c" "$T/conv/forma_a.orig"; credo $? "XIII -probare: convertibilis nominata, nihil scriptum (rc $rcp)"
$CONV "$T/conv/probatio_forma_a.c" > "$T/conv/iterum.out" 2>&1; rci=$?
[ "$rci" -eq 2 ] && grep -q 'iam conversa' "$T/conv/iterum.out"; credo $? "XIII iam conversa: exitus 2, nihil mutatum (rc $rci)"
FX=probationes/fixa/credo_convertere
$CONV -suita xar -ad "$T/conv/xar.c" "$FX/xar.vetus" > /dev/null 2>&1 && cmp -s "$T/conv/xar.c" "$FX/xar.exspectata"; credo $? "XIII xar vetus -> exspectata octetim (forma A, XXXIV sectiones)"
$CONV -suita stml -ad "$T/conv/stml.c" "$FX/stml.vetus" > /dev/null 2>&1 && cmp -s "$T/conv/stml.c" "$FX/stml.exspectata"; credo $? "XIII stml vetus -> exspectata octetim (forma B, LXVIII sectiones)"
# nota commissionis (lint 5): convertibilis nominatur, conversa et lib tacent
mkdir -p "$T/conv/probationes"; cp "$T/conv/forma_a.orig" "$T/conv/probationes/probatio_nota.c"
nota="$(python3 -c "
import sys; sys.path.insert(0, 'pythonica'); import silva
for l in silva.notae_convertibilium(['$T/conv/probationes/probatio_nota.c', 'probationes/probatio_xar.c', 'lib/xar.c']): print(l)" 2>&1)"
[ "$(printf '%s\n' "$nota" | grep -c 'convertibilis')" -eq 1 ] && printf '%s\n' "$nota" | grep -q 'probatio_nota.c convertibilis (forma A, 2 sectiones)'; credo $? "XIII nota commissionis: convertibilis sola nominata ($(printf '%s' "$nota" | cut -c1-60))"

echo
if [ "$fracta" -eq 0 ]; then echo "fumus credo: sanum"; exit 0; fi
echo "fumus credo: FRACTUM ($fracta)"; exit 1
