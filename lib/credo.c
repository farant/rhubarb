/* credo.c */
#include "postulata_posix.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <setjmp.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>


/* ======================================================
 * Globalis Status
 * ====================================================== */

universalis        Piscina* _credo_piscina  = NIHIL;
universalis   CredoNotatio* _credo_primus   = NIHIL;
universalis   CredoNotatio* _credo_nunc     = NIHIL;
universalis memoriae_index  _credo_numerus  = ZEPHYRUM;

/* GRADUS (credo II): CREDO_GRADUS currens - notationibus additur */
hic_manens constans character* _credo_gradus = NIHIL;

/* FRACTURAE EXSPECTATAE (credo II): assertiones fractae sectionum quae
 * 'exspectata_fracta' ferunt (NOTA_FRACTA) - suitam non frangunt;
 * suitae veteres numquam ponunt (ZEPHYRUM) */
hic_manens memoriae_index _credo_fracti_exspectati = ZEPHYRUM;

/* sectiones (credo v2): punctum saltus NECESSE, et num cursor sectionem
 * currit / num in filio furcato sumus (ibi saltus vetitus) */
interior        jmp_buf  _credo_saltus;
interior            b32  _credo_in_sectione = FALSUM;
interior            b32  _credo_in_filio    = FALSUM;


/* ======================================================
 * Adiutores Interni
 * ====================================================== */

interior CredoNotatio*
_credo_allocare_notatio (
    vacuum)
{
    CredoNotatio* notatio;

    si (!_credo_piscina)
    {
        imprimere("FATALE: credo nunquam aperitum! Vocate credo_aperire primum.\n");
        exit(I);
    }

    notatio = (CredoNotatio*)piscina_allocare(_credo_piscina,
        magnitudo(CredoNotatio));
    si (!notatio)
    {
        redde NIHIL;
    }

    notatio->sequens = NIHIL;  /* Initiare sequens ad NIHIL */

    return notatio;
}

interior vacuum
_credo_concatenare (
    CredoNotatio* notatio)
{
    si (!notatio) redde;

    si (!_credo_primus)
    {
        _credo_primus  = notatio;
        _credo_nunc    = notatio;
    }
    alioquin
    {
        _credo_nunc->sequens  = notatio;
        _credo_nunc           = notatio;
    }

    _credo_numerus++;
}


/* ======================================================
 * Apparatus Globalis
 * ====================================================== */

vacuum
credo_aperire (
    Piscina* piscina)
{
    _credo_piscina            = piscina;
    _credo_primus             = NIHIL;
    _credo_nunc               = NIHIL;
    _credo_numerus            = ZEPHYRUM;
    _credo_fracti_exspectati  = ZEPHYRUM;
    _credo_gradus             = NIHIL;
}

vacuum
credo_claudere (
    vacuum)
{
    /* Piscina manet; conditio solum reficitur */
    _credo_piscina            = NIHIL;
    _credo_primus             = NIHIL;
    _credo_nunc               = NIHIL;
    _credo_numerus            = ZEPHYRUM;
    _credo_fracti_exspectati  = ZEPHYRUM;
    _credo_gradus             = NIHIL;
}


/* ======================================================
 * Interrogatio
 * ====================================================== */

memoriae_index
credo_numerus_totalis (
    vacuum)
{
    redde _credo_numerus;
}

memoriae_index
credo_numerus_praeteriti (
    vacuum)
{
    memoriae_index  summa    = ZEPHYRUM;
      CredoNotatio* notatio  = _credo_primus;

    dum (notatio)
    {
        si (notatio->praeteritus)
        {
            summa++;
        }
        notatio = notatio->sequens;
    }

    redde summa;
}

memoriae_index
credo_numerus_fracti (
    vacuum)
{
    redde _credo_numerus - credo_numerus_praeteriti();
}

b32
credo_omnia_praeterierunt (
    vacuum)
{
    /* Nihil probatum = NON praeteritum. Probatio quae nullam
     * assertionem fecit (corpus absens, plica mota, gyrus qui nihil
     * invenit) viridis erat quia fractae nullae - lumen viride,
     * nihil visum (2026-09-02). Totalis ZEPHYRUM hic FALSUM reddit;
     * compendium 'NIHIL PROBATUM' imprimit. */
    si (_credo_numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde credo_numerus_fracti() == _credo_fracti_exspectati;
}

/* Fractas invenire. COPIAS reddit, non catenam filtratam: 'sequens'
 * originalium filtrando corrumperetur, et catena principalis omnibus
 * ceteris interrogationibus servit.
 *
 * Declarata ab initio, definita 2026-08-13 - probatio prima quae eam
 * vocavit (probatio_manus, quae nuntium fracturae ipsum probat)
 * symbolum deesse invenit. */
CredoNotatio*
credo_invenire_fractos (
    memoriae_index* numerus_fructus)
{
      CredoNotatio* primus   = NIHIL;
      CredoNotatio* ultimus  = NIHIL;
      CredoNotatio* notatio  = _credo_primus;
    memoriae_index  summa    = ZEPHYRUM;

    si (numerus_fructus)
    {
        *numerus_fructus = ZEPHYRUM;
    }
    si (!_credo_piscina)
    {
        redde NIHIL;
    }

    dum (notatio)
    {
        si (!notatio->praeteritus)
        {
            CredoNotatio* copia = (CredoNotatio*)piscina_allocare(
                _credo_piscina, magnitudo(CredoNotatio));
            si (!copia)
            {
                frange;
            }
            *copia          = *notatio;
            copia->sequens  = NIHIL;

            si (ultimus)
            {
                ultimus->sequens = copia;
            }
            alioquin
            {
                primus = copia;
            }
            ultimus = copia;
            summa++;
        }
        notatio = notatio->sequens;
    }

    si (numerus_fructus)
    {
        *numerus_fructus = summa;
    }
    redde primus;
}

vacuum
credo_imprimere_compendium (
    vacuum)
{
    memoriae_index totalis     = credo_numerus_totalis();
    memoriae_index praeteriti  = credo_numerus_praeteriti();
    memoriae_index fracti      = credo_numerus_fracti();

    imprimere("\n");
    imprimere("=== CREDO COMPENDIUM ===\n");
    imprimere("Totalis:    %lu\n", (insignatus longus)totalis);
    imprimere("Praeteriti: %lu\n", (insignatus longus)praeteriti);
    imprimere("Fracti:     %lu\n", (insignatus longus)fracti);
    si (_credo_fracti_exspectati > ZEPHYRUM)
    {
        imprimere("Exspectati: %lu (NOTA_FRACTA - causa in fonte)\n",
            (insignatus longus)_credo_fracti_exspectati);
    }

    si (totalis == ZEPHYRUM)
    {
        /* tertia conditio: nulla assertio cucurrit - idem quod
         * credo_omnia_praeterierunt FALSUM reddit */
        imprimere("\nConditio: NIHIL PROBATUM\n");
    }
    alioquin si (fracti > _credo_fracti_exspectati)
    {
        imprimere("\nConditio: FRACTA\n");
    }
    alioquin si (fracti > ZEPHYRUM)
    {
        imprimere("\nConditio: OMNIA PRAETERIERUNT (fracturis "
            "exspectatis exceptis)\n");
    }
    alioquin
    {
        imprimere("\nConditio: OMNIA PRAETERIERUNT\n");
    }
    imprimere("=========================\n\n");
}


/* ======================================================
 * Recordatio Interni
 * ====================================================== */

/* credo III: linea tertia 'Differentia:' (NIHIL = nulla) post
 * Speratus/Receptus - formae binae priores immutatae manent. */
interior vacuum
_credo_notare_plenum (
    constans character* genus,
    constans character* expressio,
    constans character* valor_primus,
    constans character* valor_secundus,
    constans character* differentia,
    constans character* filum,
                   s32  versus,
                   b32  praeteritus)
{
    CredoNotatio* notatio = _credo_allocare_notatio();

    si (!notatio) redde;

    notatio->genus = chorda_ex_literis(genus,          _credo_piscina);
    notatio->expressio = chorda_ex_literis(expressio,
        _credo_piscina);
    notatio->valor_primus = chorda_ex_literis(valor_primus,
        _credo_piscina);
    notatio->valor_secundus = chorda_ex_literis(valor_secundus,
        _credo_piscina);
    notatio->filum = chorda_ex_literis(filum,          _credo_piscina);
    notatio->versus = versus;
    notatio->praeteritus = praeteritus;
    notatio->ordo = _credo_numerus;
    notatio->gradus = chorda_ex_literis(_credo_gradus != NIHIL
        ? _credo_gradus : "", _credo_piscina);

    _credo_concatenare(notatio);

    /* Signum statim */
    si (praeteritus)
    {
        imprimere(".");
    }
    alioquin
    {
        imprimere("F");
        imprimere("\n FRACTA (%s): %s at %s:%d",
                  chorda_ut_cstr(notatio->genus, _credo_piscina),
                  chorda_ut_cstr(notatio->expressio, _credo_piscina),
                  chorda_ut_cstr(notatio->filum, _credo_piscina),
                  versus);
        si (_credo_gradus != NIHIL)
        {
            imprimere(" (in gradu %s)", _credo_gradus);
        }
        imprimere("\n");
        /* chorda vacua: chorda_ut_cstr NIHIL reddit - "%.*s" cum
         * mensura numquam NIHIL ad "%s" dat (NECESSE, furca) */
        imprimere("  Speratus: %.*s\n",
                  (integer)notatio->valor_secundus.mensura,
                  (constans character*)notatio->valor_secundus.datum);
        imprimere("  Receptus: %.*s\n",
                  (integer)notatio->valor_primus.mensura,
                  (constans character*)notatio->valor_primus.datum);
        si (differentia != NIHIL && differentia[ZEPHYRUM] != '\0')
        {
            imprimere("  Differentia: %s\n", differentia);
        }
        /* effluere statim: probatio post fracturam saepe NIHIL
         * dereferens cadit, et per fistulam buffer plenus cum
         * relatione perit - segfault relationem absorbebat
         * (mensuratum 2026-07-31, desideratum 01KYWC5ZNX) */
        (vacuum)fflush(stdout);
    }
}

vacuum
_credo_notare (
    constans character* genus,
    constans character* expressio,
    constans character* valor_primus,
    constans character* valor_secundus,
    constans character* filum,
                   s32  versus,
                   b32  praeteritus)
{
    _credo_notare_plenum(genus, expressio, valor_primus, valor_secundus,
        NIHIL, filum, versus, praeteritus);
}


/* ==================================================
 * Redditio valorum (credo III, 2026-10-19)
 *
 * Chorda per MENSURAM (NUL intra licet), inter "", effugia \t \n \r
 * \\ \" et \xNN pro moderatoribus (octeti >= 0x80 crudi: UTF-8
 * legibilis manet; differentia hex in linea sua). Longa (> CLX) per
 * fenestram circa differentiam primam - numquam truncatio tacita.
 * ================================================== */

#define CREDO_FENESTRA_CHORDAE     CLX
#define CREDO_ANTE_CHORDAE         LX
#define CREDO_FENESTRA_OCTETORUM   XXXII
#define CREDO_ANTE_OCTETORUM       XVI
#define CREDO_ALVEUS_EFFUSIONIS             MXXIV

interior vacuum
_credo_appendere (
               character* scopus,
          memoriae_index  capacitas,
          memoriae_index* positio,
      constans character* textus)
{
    dum (*textus != '\0' && *positio + I < capacitas)
    {
        scopus[*positio] = *textus;
        (*positio)++;
        textus++;
    }
    scopus[*positio] = '\0';
}

interior vacuum
_credo_octetum_effugere (
          character* scopus,
     memoriae_index  capacitas,
     memoriae_index* positio,
                 i8  octetus)
{
    character parvus[VIII];

    commutatio (octetus)
    {
        casus '\t': strcpy(parvus, "\\t");  frange;
        casus '\n': strcpy(parvus, "\\n");  frange;
        casus '\r': strcpy(parvus, "\\r");  frange;
        casus '\\': strcpy(parvus, "\\\\"); frange;
        casus '"':  strcpy(parvus, "\\\""); frange;
        ordinarius:
            si (octetus < 0x20 || octetus == 0x7f)
            {
                sprintf(parvus, "\\x%02x", (insignatus integer)octetus);
            }
            alioquin
            {
                parvus[ZEPHYRUM]  = (character)octetus;
                parvus[I]         = '\0';
            }
            frange;
    }
    _credo_appendere(scopus, capacitas, positio, parvus);
}

/* index octeti primi diversi (aut mensura brevioris si praefixum) */
interior i32
_credo_differentia_prima (
    chorda a,
    chorda b)
{
    i32 minimum  = a.mensura < b.mensura ? a.mensura : b.mensura;
    i32 k        = ZEPHYRUM;

    dum (k < minimum && a.datum[k] == b.datum[k])
    {
        k++;
    }
    redde k;
}

interior vacuum
_credo_chordam_reddere (
          character* scopus,
     memoriae_index  capacitas,
             chorda  c,
                i32  prima)
{
    memoriae_index positio  = ZEPHYRUM;
               i32 initium  = ZEPHYRUM;
               i32 finis    = c.mensura;
               i32 k;
         character numerus[XCVI];

    scopus[ZEPHYRUM] = '\0';
    si (c.mensura > CREDO_FENESTRA_CHORDAE)
    {
        initium = prima > CREDO_ANTE_CHORDAE
            ? prima - CREDO_ANTE_CHORDAE : ZEPHYRUM;
        finis = initium + CREDO_FENESTRA_CHORDAE;
        si (finis > c.mensura)
        {
            finis = c.mensura;
        }
    }
    si (initium > ZEPHYRUM)
    {
        _credo_appendere(scopus, capacitas, &positio, "…");
    }
    _credo_appendere(scopus, capacitas, &positio, "\"");
    per (k = initium; k < finis; k++)
    {
        _credo_octetum_effugere(scopus, capacitas, &positio,
            c.datum[k]);
    }
    _credo_appendere(scopus, capacitas, &positio, "\"");
    si (finis < c.mensura)
    {
        _credo_appendere(scopus, capacitas, &positio, "…");
    }
    si (c.mensura <= CREDO_FENESTRA_CHORDAE)
    {
        sprintf(numerus, " (%u %s)", c.mensura,
            c.mensura == I ? "octetus" : "octeti");
    }
    alioquin
    {
        sprintf(numerus, " (%u octeti, fenestra [%u, %u))", c.mensura,
            initium, finis);
    }
    _credo_appendere(scopus, capacitas, &positio, numerus);
}

/* "'c' 0xNN" aut "finit" (chorda ante indicem desinit) */
interior vacuum
_credo_octetum_citare (
          character* scopus,
     memoriae_index  capacitas,
     memoriae_index* positio,
             chorda  c,
                i32  index)
{
    character hex[XVI];

    si (index >= c.mensura)
    {
        _credo_appendere(scopus, capacitas, positio, "finit");
        redde;
    }
    _credo_appendere(scopus, capacitas, positio, "'");
    _credo_octetum_effugere(scopus, capacitas, positio, c.datum[index]);
    sprintf(hex, "' 0x%02x", (insignatus integer)c.datum[index]);
    _credo_appendere(scopus, capacitas, positio, hex);
}

/* fractura chordarum: Receptus/Speratus redditi, Differentia (si
 * differentiam) octeti primi diversi */
interior vacuum
_credo_chordas_notare (
    constans character* genus,
    constans character* expressio,
                chorda  receptus,
                chorda  speratus,
                   b32  praeteritus,
                   b32  differentiam,
    constans character* filum,
                   s32  versus)
{
         character redditus_r[CREDO_ALVEUS_EFFUSIONIS];
         character redditus_s[CREDO_ALVEUS_EFFUSIONIS];
         character differentia[CCLVI];
    memoriae_index positio  = ZEPHYRUM;
               i32 prima    = ZEPHYRUM;
         character numerus[XXXII];

    si (praeteritus)
    {
        _credo_notare_plenum(genus, expressio, "", "", NIHIL, filum,
            versus, VERUM);
        redde;
    }
    si (differentiam)
    {
        prima = _credo_differentia_prima(receptus, speratus);
    }
    _credo_chordam_reddere(redditus_r, CREDO_ALVEUS_EFFUSIONIS,
        receptus, prima);
    _credo_chordam_reddere(redditus_s, CREDO_ALVEUS_EFFUSIONIS,
        speratus, prima);
    differentia[ZEPHYRUM] = '\0';
    si (differentiam)
    {
        sprintf(numerus, "octetus %u - speratus ", prima);
        _credo_appendere(differentia, CCLVI, &positio, numerus);
        _credo_octetum_citare(differentia, CCLVI, &positio, speratus,
            prima);
        _credo_appendere(differentia, CCLVI, &positio, ", receptus ");
        _credo_octetum_citare(differentia, CCLVI, &positio, receptus,
            prima);
    }
    _credo_notare_plenum(genus, expressio, redditus_r, redditus_s,
        differentiam ? differentia : NIHIL, filum, versus, FALSUM);
}

vacuum
_credo_notare_chordae (
    constans character* genus,
    constans character* expressio,
                chorda  receptus,
                chorda  speratus,
                   s32  comparatio,
    constans character* filum,
                   s32  versus)
{
    b32 praeteritus;

    commutatio (comparatio)
    {
        casus CREDO_COMPARATIO_CONTINET:
            praeteritus = chorda_continet(receptus, speratus);
            frange;
        casus CREDO_COMPARATIO_INCIPIT:
            praeteritus = chorda_incipit(receptus, speratus);
            frange;
        casus CREDO_COMPARATIO_TERMINATUR:
            praeteritus = chorda_terminatur(receptus, speratus);
            frange;
        ordinarius:
            praeteritus = chorda_aequalis(receptus, speratus);
            frange;
    }
    _credo_chordas_notare(genus, expressio, receptus, speratus,
        praeteritus, comparatio == CREDO_COMPARATIO_AEQUALIS, filum,
        versus);
}

vacuum
_credo_notare_literae (
    constans character* genus,
    constans character* expressio,
    constans character* receptus,
    constans character* speratus,
    constans character* filum,
                   s32  versus)
{
         character redditus[CREDO_ALVEUS_EFFUSIONIS];
            chorda c;

    si (receptus == NIHIL || speratus == NIHIL || !_credo_piscina)
    {
        si (receptus == speratus)
        {
            _credo_notare_plenum(genus, expressio, "", "", NIHIL, filum,
                versus, VERUM);
            redde;
        }
        redditus[ZEPHYRUM] = '\0';
        si (_credo_piscina)
        {
            c = chorda_ex_literis(receptus
                != NIHIL ? receptus : speratus,
                _credo_piscina);
            _credo_chordam_reddere(redditus, CREDO_ALVEUS_EFFUSIONIS, c,
                ZEPHYRUM);
        }
        _credo_notare_plenum(genus, expressio,
            receptus == NIHIL ? "NIHIL" : redditus,
            speratus == NIHIL ? "NIHIL" : redditus,
            NIHIL, filum, versus, FALSUM);
        redde;
    }
    _credo_chordas_notare(genus, expressio,
        chorda_ex_literis(receptus, _credo_piscina),
        chorda_ex_literis(speratus, _credo_piscina),
        strcmp(receptus, speratus) == ZEPHYRUM, VERUM, filum, versus);
}

interior vacuum
_credo_octetos_reddere (
                  character* scopus,
             memoriae_index  capacitas,
                constans i8* datum,
             memoriae_index  mensura,
             memoriae_index  prima)
{
    memoriae_index positio  = ZEPHYRUM;
    memoriae_index initium  = ZEPHYRUM;
    memoriae_index finis    = mensura;
    memoriae_index k;
         character parvus[XXXII];

    scopus[ZEPHYRUM] = '\0';
    si (mensura > CREDO_FENESTRA_OCTETORUM)
    {
        initium = prima > CREDO_ANTE_OCTETORUM
            ? prima - CREDO_ANTE_OCTETORUM : ZEPHYRUM;
        finis = initium + CREDO_FENESTRA_OCTETORUM;
        si (finis > mensura)
        {
            finis = mensura;
        }
    }
    si (initium > ZEPHYRUM)
    {
        _credo_appendere(scopus, capacitas, &positio, "…");
    }
    per (k = initium; k < finis; k++)
    {
        sprintf(parvus, k == prima ? "%s[%02x]" : "%s%02x",
            k > initium ? " " : "", (insignatus integer)datum[k]);
        _credo_appendere(scopus, capacitas, &positio, parvus);
    }
    si (finis < mensura)
    {
        _credo_appendere(scopus, capacitas, &positio, "…");
    }
    si (mensura <= CREDO_FENESTRA_OCTETORUM)
    {
        sprintf(parvus, " (%lu %s)", (insignatus longus)mensura,
            mensura == I ? "octetus" : "octeti");
        _credo_appendere(scopus, capacitas, &positio, parvus);
    }
    alioquin
    {
        character nota[XCVI];

        sprintf(nota, " (%lu octeti, fenestra [%lu, %lu))",
            (insignatus longus)mensura, (insignatus longus)initium,
            (insignatus longus)finis);
        _credo_appendere(scopus, capacitas, &positio, nota);
    }
}

vacuum
_credo_notare_octeti (
    constans character* genus,
    constans character* expressio,
       constans vacuum* receptus,
       constans vacuum* speratus,
        memoriae_index  mensura,
    constans character* filum,
                   s32  versus)
{
         constans i8* r      = (constans i8*)receptus;
         constans i8* s      = (constans i8*)speratus;
      memoriae_index  prima  = ZEPHYRUM;
           character  redditus_r[CREDO_ALVEUS_EFFUSIONIS];
           character  redditus_s[CREDO_ALVEUS_EFFUSIONIS];
           character  differentia[CXXVIII];

    si (r == NIHIL || s == NIHIL)
    {
        _credo_notare_plenum(genus, expressio,
            r == NIHIL ? "NIHIL" : "(octeti)",
            s == NIHIL ? "NIHIL" : "(octeti)", NIHIL, filum, versus,
            r == s || mensura == ZEPHYRUM);
        redde;
    }
    dum (prima < mensura && r[prima] == s[prima])
    {
        prima++;
    }
    si (prima == mensura)
    {
        _credo_notare_plenum(genus, expressio, "", "", NIHIL, filum,
            versus, VERUM);
        redde;
    }
    _credo_octetos_reddere(redditus_r, CREDO_ALVEUS_EFFUSIONIS, r,
        mensura,
        prima);
    _credo_octetos_reddere(redditus_s, CREDO_ALVEUS_EFFUSIONIS, s,
        mensura,
        prima);
    sprintf(differentia,
        "octetus %lu - speratus 0x%02x, receptus 0x%02x",
        (insignatus longus)prima, (insignatus integer)s[prima],
        (insignatus integer)r[prima]);
    _credo_notare_plenum(genus, expressio, redditus_r, redditus_s,
        differentia, filum, versus, FALSUM);
}

interior b32
_credo_relatio_s64 (
    s64 a,
    s64 b,
    s32 relatio)
{
    commutatio (relatio)
    {
        casus CREDO_RELATIO_INAEQUALIS:         redde a != b;
        casus CREDO_RELATIO_MAIOR:              redde a > b;
        casus CREDO_RELATIO_MINOR:              redde a < b;
        casus CREDO_RELATIO_MAIOR_AUT_AEQUALIS: redde a >= b;
        casus CREDO_RELATIO_MINOR_AUT_AEQUALIS: redde a <= b;
        ordinarius:                             redde a == b;
    }
}

interior b32
_credo_relatio_i64 (
    i64 a,
    i64 b,
    s32 relatio)
{
    commutatio (relatio)
    {
        casus CREDO_RELATIO_INAEQUALIS:         redde a != b;
        casus CREDO_RELATIO_MAIOR:              redde a > b;
        casus CREDO_RELATIO_MINOR:              redde a < b;
        casus CREDO_RELATIO_MAIOR_AUT_AEQUALIS: redde a >= b;
        casus CREDO_RELATIO_MINOR_AUT_AEQUALIS: redde a <= b;
        ordinarius:                             redde a == b;
    }
}

vacuum
_credo_notare_s64 (
    constans character* genus,
    constans character* expressio,
                   s64  valor_primus,
                   s64  valor_secundus,
                   s32  relatio,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];

    snprintf(buffer_primus,   64, "%lld", valor_primus);
    snprintf(buffer_secundus, 64, "%lld", valor_secundus);
    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, _credo_relatio_s64(valor_primus, valor_secundus,
        relatio));
}

vacuum
_credo_notare_i64 (
    constans character* genus,
    constans character* expressio,
                   i64  valor_primus,
                   i64  valor_secundus,
                   s32  relatio,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];

    snprintf(buffer_primus,   64, "%llu", valor_primus);
    snprintf(buffer_secundus, 64, "%llu", valor_secundus);
    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, _credo_relatio_i64(valor_primus, valor_secundus,
        relatio));
}

vacuum
_credo_notare_integrum (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus == valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%u", valor_primus);
    snprintf(buffer_secundus, 64, "%u", valor_secundus);

    praeteritus = (valor_primus == valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_s64 (
    constans character* genus,
    constans character* expressio,
                   s64  valor_primus,
                   s64  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%lld", valor_primus);
    snprintf(buffer_secundus, 64, "%lld", valor_secundus);

    praeteritus = (valor_primus == valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_inaequalitas (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus != valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_inaequalitas_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%u", valor_primus);
    snprintf(buffer_secundus, 64, "%u", valor_secundus);

    praeteritus = (valor_primus != valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_chorda (
    constans character* genus,
    constans character* expressio,
    constans character* valor_primus,
    constans character* valor_secundus,
    constans character* filum,
                   s32  versus)
{
       b32 praeteritus;
    chorda chorda_primus;
    chorda chorda_secundus;

    chorda_primus = chorda_ex_literis(valor_primus, _credo_piscina);
    chorda_secundus = chorda_ex_literis(valor_secundus, _credo_piscina);

    praeteritus = chorda_aequalis(chorda_primus, chorda_secundus);

    _credo_notare(genus, expressio, valor_primus,
                  valor_secundus, filum, versus, praeteritus);
}

vacuum
_credo_notare_chorda_literis (
    constans character* genus,
    constans character* expressio,
                chorda  valor_primus,
    constans character* valor_secundus,
    constans character* filum,
                   s32  versus)
{
    /* credo III: olim receptus ad CCLV octetos tacite truncatus -
     * nunc per redditionem communem (fenestra nominata, Differentia) */
    si (valor_secundus == NIHIL || !_credo_piscina)
    {
        _credo_notare_plenum(genus, expressio, "(chorda)", "NIHIL",
            NIHIL,
            filum, versus, FALSUM);
        redde;
    }
    _credo_chordas_notare(genus, expressio, valor_primus,
        chorda_ex_literis(valor_secundus, _credo_piscina),
        chorda_aequalis_literis(valor_primus, valor_secundus), VERUM,
        filum, versus);
}

vacuum
_credo_notare_integrum_maior (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus > valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_maior_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%u", valor_primus);
    snprintf(buffer_secundus, 64, "%u", valor_secundus);

    praeteritus = (valor_primus > valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_minor (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus < valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_minor_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%u", valor_primus);
    snprintf(buffer_secundus, 64, "%u", valor_secundus);

    praeteritus = (valor_primus < valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_maior_aut_aequalis (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus >= valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_maior_aut_aequalis_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%u", valor_primus);
    snprintf(buffer_secundus, 64, "%u", valor_secundus);

    praeteritus = (valor_primus >= valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_minor_aut_aequalis (
    constans character* genus,
    constans character* expressio,
                   s32  valor_primus,
                   s32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%d", valor_primus);
    snprintf(buffer_secundus, 64, "%d", valor_secundus);

    praeteritus = (valor_primus <= valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_integrum_minor_aut_aequalis_i32 (
    constans character* genus,
    constans character* expressio,
                   i32  valor_primus,
                   i32  valor_secundus,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
          b32 praeteritus;

    snprintf(buffer_primus,   64, "%u", valor_primus);
    snprintf(buffer_secundus, 64, "%u", valor_secundus);

    praeteritus = (valor_primus <= valor_secundus);

    _credo_notare(genus, expressio, buffer_primus, buffer_secundus,
        filum, versus, praeteritus);
}

vacuum
_credo_notare_f64_proximus (
    constans character* genus,
    constans character* expressio,
                   f64  valor_primus,
                   f64  valor_secundus,
                   f64  epsilon,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[64];
    character buffer_differentiae[96];
          b32 praeteritus;
          f64 differentia;

    snprintf(buffer_primus,   64, "%.10f", valor_primus);
    snprintf(buffer_secundus, 64, "%.10f (ε=%.10f)", valor_secundus,
        epsilon);

    differentia = fabs(valor_primus - valor_secundus);
    praeteritus = (differentia <= epsilon);

    snprintf(buffer_differentiae, 96, "%g (ε %g)", differentia,
        epsilon);
    _credo_notare_plenum(genus, expressio, buffer_primus,
        buffer_secundus,
        buffer_differentiae, filum, versus, praeteritus);
}

vacuum
_credo_notare_f64_relative (
    constans character* genus,
    constans character* expressio,
                   f64  valor_primus,
                   f64  valor_secundus,
                   f64  relativa,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[96];
    character buffer_differentiae[96];
          f64 differentia = fabs(valor_primus - valor_secundus);
          f64 maximum     = fabs(valor_primus) > fabs(valor_secundus)
                              ? fabs(valor_primus)
                              : fabs(valor_secundus);
          f64 limes       = relativa * maximum;

    snprintf(buffer_primus,   64, "%.17g", valor_primus);
    snprintf(buffer_secundus, 96, "%.17g (relative %g)", valor_secundus,
        relativa);
    snprintf(buffer_differentiae, 96, "|a-b| %g, limes %g", differentia,
        limes);
    /* NaN: comparatio falsa, ergo fracta */
    _credo_notare_plenum(genus, expressio, buffer_primus,
        buffer_secundus,
        buffer_differentiae, filum, versus, differentia <= limes);
}

/* ordo integer duplicium (IEEE 754): signum + magnitudo -> ordo
 * monotonus, ut differentia ordinum = numerus ULP; -0.0 et +0.0 idem */
interior i64
_credo_f64_ordinatum (
    f64 x)
{
    i64 octeti;
    i64 signum = (i64)I << LXIII;

    memcpy(&octeti, &x, magnitudo(octeti));
    si (octeti & signum)
    {
        redde ~octeti + (i64)I;
    }
    redde octeti | signum;
}

vacuum
_credo_notare_f64_ulp (
    constans character* genus,
    constans character* expressio,
                   f64  valor_primus,
                   f64  valor_secundus,
                   i64  distantia,
    constans character* filum,
                   s32  versus)
{
    character buffer_primus[64];
    character buffer_secundus[96];
    character buffer_differentiae[96];
          i64 a;
          i64 b;
          i64 inter;

    snprintf(buffer_primus,   64, "%.17g", valor_primus);
    snprintf(buffer_secundus, 96, "%.17g (%llu ULP)", valor_secundus,
        distantia);
    si (   valor_primus   != valor_primus
        || valor_secundus != valor_secundus)
    {
        _credo_notare_plenum(genus, expressio, buffer_primus,
            buffer_secundus, "NaN - numquam aequalis", filum, versus,
            FALSUM);
        redde;
    }
    a      = _credo_f64_ordinatum(valor_primus);
    b      = _credo_f64_ordinatum(valor_secundus);
    inter  = a >= b ? a - b : b - a;
    snprintf(buffer_differentiae, 96, "%llu ULP", inter);
    _credo_notare_plenum(genus, expressio, buffer_primus,
        buffer_secundus,
        buffer_differentiae, filum, versus, inter <= distantia);
}


/* ==================================================
 * Assertiones processus (spec stml-instrumentum 7.5.0)
 * ================================================== */

interior constans character*
_credo_signi_nomen (
    s32 signum)
{
    commutatio (signum)
    {
        casus SIGSEGV:
            redde "SEGV";
        casus SIGBUS:
            redde "BUS";
        casus SIGABRT:
            redde "ABRT";
        casus SIGFPE:
            redde "FPE";
        casus SIGILL:
            redde "ILL";
        casus SIGKILL:
            redde "KILL (mora excessa)";
        ordinarius:
            redde "signum ignotum";
    }
}

CredoProcessusFructus
credo_processus_incipere (
    vacuum)
{
    CredoProcessusFructus fructus;
                    pid_t pid;

    fructus.in_filio  = FALSUM;
    fructus.pid       = -I;

    /* Buffra ANTE furcam eicienda: aliter quidquid pendet in
     * filium transcribitur et BIS emittitur. */
    fflush(NIHIL);

    pid = fork();
    si (pid == ZEPHYRUM)
    {
        fructus.in_filio  = VERUM;
        fructus.pid       = ZEPHYRUM;
        _credo_in_filio   = VERUM;   /* NECESSE hic _exit, non saltus */
        redde fructus;
    }

    fructus.pid = (s32)pid;   /* -I si furca defecit */
    redde fructus;
}

vacuum
credo_processus_filium_finire (
    vacuum)
{
    /* _exit, NON exit: buffra parentis hereditata sunt, et exit()
     * ea eiceret. */
    _exit(ZEPHYRUM);
}

vacuum
credo_processus_iudicare (
    CredoProcessusFructus* fructus,
       constans character* genus,
       constans character* expressio,
                      i32  mora_ms,
                      s32  signum_exspectatum,
       constans character* filum,
                      s32  versus)
{
              integer status;
                pid_t peractus;
                  i32 elapsum_ms;
                  b32 praeteritus;
                  s32 signum_acceptum;
    structura timespec pausa;
  constans character* exspectatum;
  constans character* acceptum;

    si (fructus == NIHIL || fructus->in_filio)
    {
        redde;   /* filius numquam iudicat */
    }

    si (fructus->pid < ZEPHYRUM)
    {
        _credo_notare(genus, expressio, "furca defecit", "",
                      filum, versus, FALSUM);
        redde;
    }

    /* Terminus per POLLATIONEM, non per alarm(): SIGALRM cum omni
     * usu signorum in codice probato colliditur, et manubria sua
     * problemata reentrantiae ferunt. */
    status           = ZEPHYRUM;
    elapsum_ms       = ZEPHYRUM;
    signum_acceptum  = ZEPHYRUM;
    pausa.tv_sec     = (time_t)ZEPHYRUM;
    pausa.tv_nsec    = (longus)(M * M);   /* I ms */

    dum (VERUM)
    {
        peractus = waitpid((pid_t)fructus->pid, &status, WNOHANG);
        si (peractus == (pid_t)fructus->pid)
        {
            frange;
        }
        si (elapsum_ms >= mora_ms)
        {
            /* SIGKILL, non SIGTERM: processus impeditus manubrium
             * numquam curret. Praeterea SIGKILL in relatione
             * inambiguus est - NOS eum occidimus, non ipse ruit. */
            kill((pid_t)fructus->pid, SIGKILL);
            waitpid((pid_t)fructus->pid, &status, ZEPHYRUM);
            frange;
        }
        (vacuum)nanosleep(&pausa, NIHIL);
        elapsum_ms = elapsum_ms + I;
    }

    si (WIFSIGNALED(status))
    {
        signum_acceptum = (s32)WTERMSIG(status);
    }

    si (signum_exspectatum == ZEPHYRUM)
    {
        /* Praeterit SOLUM si normaliter exiit. Codex exitus ruinam
         * NARRARE NON POTEST - processus signo occisus nullum
         * habet; ergo WIFSIGNALED est mechanismus, non
         * WEXITSTATUS. */
        praeteritus = (b32)(WIFEXITED(status) != ZEPHYRUM);
    }
    alioquin
    {
        praeteritus = (b32)(signum_acceptum == signum_exspectatum);
    }

    /* 'Speratus' nominandus est, non vacuus relinquendus: relatio
     * 'Receptus SEGV / Speratus (null)' dimidiam narrat solum -
     * legens quid EXSPECTAVERIMUS scire debet. */
    si (signum_exspectatum == ZEPHYRUM)
    {
        exspectatum = "exitus normalis (nullum signum)";
    }
    alioquin
    {
        exspectatum = _credo_signi_nomen(signum_exspectatum);
    }

    si (signum_acceptum != ZEPHYRUM)
    {
        acceptum = _credo_signi_nomen(signum_acceptum);
    }
    alioquin si (WIFEXITED(status))
    {
        acceptum = "exitus normalis";
    }
    alioquin
    {
        acceptum = "status ignotus";
    }

    _credo_notare(genus, expressio, acceptum, exspectatum,
                  filum, versus, praeteritus);
}


/* ==================================================
 * Sectiones (credo v2; vide credo.h)
 * ================================================== */

/* tempus parietis in ms (gettimeofday: lexicon POSIX; credo nullam
 * bibliothecam praeter chordam et piscinam trahit) */
interior f64
_credo_ms_nunc (
    vacuum)
{
    structura timeval tv;

    si (gettimeofday(&tv, NIHIL) != ZEPHYRUM)
    {
        redde 0.0;
    }
    redde (f64)tv.tv_sec * 1000.0 + (f64)tv.tv_usec / 1000.0;
}

/* campus TSV: '\t' '\n' '\\' effugiuntur (forma machinae domus) */
interior vacuum
_credo_campum_scribere (
                    FILE* plagula,
      constans character* textus,
          memoriae_index  mensura)
{
    memoriae_index i;

    per (i = ZEPHYRUM; i < mensura; i++)
    {
        si (textus[i] == '\t')
        {
            fputs("\\t", plagula);
        }
        alioquin si (textus[i] == '\n')
        {
            fputs("\\n", plagula);
        }
        alioquin si (textus[i] == '\\')
        {
            fputs("\\\\", plagula);
        }
        alioquin
        {
            fputc(textus[i], plagula);
        }
    }
}

interior vacuum
_credo_literas_scribere (
                   FILE* plagula,
     constans character* textus)
{
    _credo_campum_scribere(plagula, textus, strlen(textus));
}

interior vacuum
_credo_chordam_scribere (
      FILE* plagula,
    chorda  c)
{
    si (c.datum != NIHIL)
    {
        _credo_campum_scribere(plagula, (constans character*)c.datum,
            (memoriae_index)c.mensura);
    }
}

vacuum
_credo_necesse (
    constans character* genus,
    constans character* expressio,
    constans character* filum,
                   s32  versus,
                   b32  praeteritus)
{
    _credo_notare(genus, expressio, expressio, "", filum, versus,
        praeteritus);
    si (praeteritus)
    {
        redde;
    }
    si (_credo_in_filio)
    {
        /* in filio CREDO_NON_RUIT: saltus cursorem PARENTIS in filio
         * resumeret et suitam reliquam bis curreret */
        (vacuum)fflush(stdout);
        _exit(I);
    }
    si (_credo_in_sectione)
    {
        longjmp(_credo_saltus, I);
    }
    /* extra sectionem: ut CREDO_VERUM (iam notatum) */
}

/* sectionem unam agere: setjmp in functione PARVA cuius locales post
 * setjmp numquam mutantur (aliter post longjmp indeterminati). VERUM
 * si NECESSE eam abrupit. */
interior b32
_credo_sectionem_agere (
    constans CredoSectio* sectio,
          CredoContextus* contextus)
{
    si (setjmp(_credo_saltus) == ZEPHYRUM)
    {
        _credo_in_sectione = VERUM;
        si (sectio->parare != NIHIL)
        {
            sectio->parare(contextus);
        }
        si (sectio->probare != NIHIL)
        {
            sectio->probare(contextus);
        }
        _credo_in_sectione = FALSUM;
        redde FALSUM;
    }
    _credo_in_sectione = FALSUM;
    redde VERUM;
}

/* exitus sectionis ex notationibus post 'ante' (NIHIL = ab initio) */
nomen structura {
     constans character* exitus;
         memoriae_index  praeteriti;
         memoriae_index  totales;
           CredoNotatio* fractura;   /* prima; NIHIL si nulla */
} CredoSectionisSumma;

interior CredoSectionisSumma
_credo_sectionem_summare (
     CredoNotatio* ante,
              b32  aborta)
{
    CredoSectionisSumma  summa;
           CredoNotatio* n;

    summa.praeteriti  = ZEPHYRUM;
    summa.totales     = ZEPHYRUM;
    summa.fractura    = NIHIL;
    per (n = (ante != NIHIL) ? ante->sequens : _credo_primus; n
        != NIHIL;
         n = n->sequens)
    {
        summa.totales++;
        si (n->praeteritus)
        {
            summa.praeteriti++;
        }
        alioquin si (summa.fractura == NIHIL)
        {
            summa.fractura = n;
        }
    }
    si (aborta)
    {
        summa.exitus = "ABORTA";
    }
    alioquin si (summa.totales == ZEPHYRUM)
    {
        summa.exitus = "VACUA";
    }
    alioquin si (summa.fractura != NIHIL)
    {
        summa.exitus = "FRACTA";
    }
    alioquin
    {
        summa.exitus = "TRANSIIT";
    }
    redde summa;
}

interior vacuum
_credo_sectionem_scribere (
                    FILE* plagula,
      constans character* suita,
      constans character* titulus,
     CredoSectionisSumma  summa,
                     f64  ms)
{
    character numerus[XXXII];

    fputs("SECTIO\t", plagula);
    _credo_literas_scribere(plagula, suita);
    fputc('\t', plagula);
    _credo_literas_scribere(plagula, titulus);
    fprintf(plagula, "\t%s\t%lu\t%lu\t%.0f\t", summa.exitus,
        (insignatus longus)summa.praeteriti,
        (insignatus longus)summa.totales, ms);
    si (summa.fractura != NIHIL)
    {
        _credo_chordam_scribere(plagula, summa.fractura->filum);
        sprintf(numerus, ":%d", (integer)summa.fractura->versus);
        fputs(numerus, plagula);
        fputc('\t', plagula);
        _credo_chordam_scribere(plagula, summa.fractura->genus);
        fputc('\t', plagula);
        _credo_chordam_scribere(plagula, summa.fractura->expressio);
        /* campus XI (credo II): gradus fracturae primae */
        fputc('\t', plagula);
        _credo_chordam_scribere(plagula, summa.fractura->gradus);
    }
    alioquin
    {
        fputs("\t\t\t", plagula);
    }
    fputc('\n', plagula);
    (vacuum)fflush(plagula);
}

vacuum
credo_gradum_ponere (
    constans character* titulus)
{
    _credo_gradus = titulus;
    si (titulus != NIHIL)
    {
        imprimere("\n  -- gradus %s --\n", titulus);
    }
}


/* ==================================================
 * AREA (credo II): via contextus ANTE cursum eius computatur,
 * directorium in credo_area PIGRE creatur, post purgare deletur
 * quidquid ibi stat (filius CREDO_NON_RUIT aream creare potest - parens
 * nescit, via tamen eadem). credo filum.c NON vocat: suitae
 * subsystematum credo sine filo ligant (silva, materia, ...).
 * ================================================== */

nomen structura {
     constans CredoContextus* contextus;   /* NIHIL = nulla area */
                   character  via[DXII];
                         b32  creata;
} CredoArea;

hic_manens CredoArea _credo_area_sectionis;
hic_manens CredoArea _credo_area_suitae;

/* arbor deleta (nexus ipse removetur, numquam destinatio); absens =
 * VERUM */
interior b32
_credo_arborem_delere (
    constans character* via)
{
    structura stat    st;
              DIR*    d;
    structura dirent* e;
         character    filius[DXII];
               b32    bene;

    si (lstat(via, &st) != ZEPHYRUM)
    {
        redde errno == ENOENT;
    }
    si (!S_ISDIR(st.st_mode))
    {
        redde unlink(via) == ZEPHYRUM;
    }
    d = opendir(via);
    si (d == NIHIL)
    {
        redde FALSUM;
    }
    bene = VERUM;
    dum ((e = readdir(d)) != NIHIL)
    {
        si (   strcmp(e->d_name, ".")  == ZEPHYRUM
            || strcmp(e->d_name, "..") == ZEPHYRUM)
        {
            perge;
        }
        si (strlen(via) + strlen(e->d_name) + II > magnitudo(filius))
        {
            bene = FALSUM;
            perge;
        }
        /* memcpy, non sprintf: longitudo supra probata, sed gcc -O2
         * (-Wformat-overflow) eam per sprintf non videt (musl T9) */
        {
            size_t lv = strlen(via);
            size_t ln = strlen(e->d_name);

            memcpy(filius, via, lv);
            filius[lv] = '/';
            memcpy(filius + lv + I, e->d_name, ln + I);
        }
        si (!_credo_arborem_delere(filius))
        {
            bene = FALSUM;
        }
    }
    (vacuum)closedir(d);
    redde bene && rmdir(via) == ZEPHYRUM;
}

/* '<TMPDIR>/<suita sanata>.<signum>.<pid>' - '/' finalis TMPDIR
 * (macOS) demitur; nihil creatur */
interior vacuum
_credo_aream_ponere (
                  CredoArea* area,
    constans CredoContextus* contextus,
         constans character* titulus_suitae,
         constans character* signum)
{
     constans character* tmp;
         memoriae_index  n;
         memoriae_index  i;
              character  ch;

    tmp = getenv("TMPDIR");
    si (tmp == NIHIL || tmp[0] == '\0')
    {
        tmp = "/tmp";
    }
    n = strlen(tmp);
    dum (n > I && tmp[n - I] == '/')
    {
        n--;
    }
    si (n > CD)
    {
        n = CD;
    }
    memcpy(area->via, tmp, n);
    area->via[n] = '/';
    n++;
    per (i = ZEPHYRUM; titulus_suitae[i] != '\0' && i < LXIV; i++)
    {
        ch = titulus_suitae[i];
        si (!(   (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')
              || (ch >= '0' && ch <= '9') || ch == '_' || ch == '-'))
        {
            ch = '_';
        }
        area->via[n] = ch;
        n++;
    }
    si (i == ZEPHYRUM)
    {
        area->via[n] = '_';
        n++;
    }
    sprintf(area->via + n, ".%s.%ld", signum, (longus)getpid());
    area->contextus  = contextus;
    area->creata     = FALSUM;
}

/* post purgare: quidquid in via stat deletur; deletio fracta
 * nominatur (reliquiae sub TMPDIR) */
interior vacuum
_credo_aream_delere (
    CredoArea* area)
{
    si (area->contextus == NIHIL)
    {
        redde;
    }
    si (!_credo_arborem_delere(area->via))
    {
        imprimere("\n  credo: area non deleta: %s\n", area->via);
    }
    area->contextus  = NIHIL;
    area->creata     = FALSUM;
}

constans character*
credo_area (
    CredoContextus* c)
{
    CredoArea* area;

    area = NIHIL;
    si (c != NIHIL && c == _credo_area_sectionis.contextus)
    {
        area = &_credo_area_sectionis;
    }
    alioquin si (c != NIHIL && c == _credo_area_suitae.contextus)
    {
        area = &_credo_area_suitae;
    }
    si (area == NIHIL)
    {
        redde NIHIL;
    }
    si (!area->creata)
    {
        /* reliquiae pid iterati prius deletae */
        si (   !_credo_arborem_delere(area->via)
            || mkdir(area->via, 0700) != ZEPHYRUM)
        {
            redde NIHIL;
        }
        area->creata = VERUM;
    }
    redde area->via;
}


/* ==================================================
 * ORDO (credo II): CREDO_ORDO=mixtus[:semen] - permutatio sectionum
 * reproducibilis (Fisher-Yates super xorshift32). Instrumentum quod
 * statum communem occultum invenit, non ordinarium, non porta.
 * Generator proprius: subsystemata credo sine 'sors' ligant.
 * ================================================== */

/* VERUM si valor notus: NIHIL/""/"tabula" -> ordo tabulae; "mixtus"
 * -> semen ex tempore et pid; "mixtus:<cifrae, IX ad summum>" */
interior b32
_credo_ordinem_legere (
     constans character* valor,
                    b32* mixtus,
      insignatus longus* semen)
{
    i32 i;

    *mixtus  = FALSUM;
    *semen   = ZEPHYRUM;
    si (   valor                   == NIHIL || valor[0] == '\0'
        || strcmp(valor, "tabula") == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (strncmp(valor, "mixtus", VI) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    *mixtus = VERUM;
    si (valor[VI] == '\0')
    {
        *semen = ((insignatus longus)time(NIHIL)
            ^ ((insignatus longus)getpid() << XVI)) % 1000000000UL;
        redde VERUM;
    }
    si (valor[VI] != ':' || valor[VII] == '\0')
    {
        redde FALSUM;
    }
    per (i = VII; valor[i] != '\0'; i++)
    {
        si (valor[i] < '0' || valor[i] > '9' || i >= VII + IX)
        {
            redde FALSUM;
        }
        *semen = *semen * X + (insignatus longus)(valor[i] - '0');
    }
    redde VERUM;
}

/* xorshift32 (status numquam ZEPHYRUM) */
interior insignatus longus
_credo_fortuitum (
    insignatus longus* status)
{
    insignatus longus x = *status;

    x        ^= (x << XIII) & 0xFFFFFFFFUL;
    x        ^= x >> XVII;
    x        ^= (x << V) & 0xFFFFFFFFUL;
    *status  = x;
    redde x;
}

/* ordo[] = 0..n-1, permutatus si mixtus (semen mixtum: semina parva
 * vicina ordines dissimiles dant) */
interior vacuum
_credo_ordinem_parare (
                  i32* ordo,
                  i32  n,
                  b32  mixtus,
    insignatus longus  semen)
{
    insignatus longus status;
                  i32 k;
                  i32 j;
                  i32 t;

    per (k = ZEPHYRUM; k < n; k++)
    {
        ordo[k] = k;
    }
    si (!mixtus)
    {
        redde;
    }
    status = (semen * 2654435761UL + 0x9E3779B9UL) & 0xFFFFFFFFUL;
    si (status == ZEPHYRUM)
    {
        status = I;
    }
    per (k = ZEPHYRUM; k < VIII; k++)
    {
        (vacuum)_credo_fortuitum(&status);
    }
    per (k = n - I; k > ZEPHYRUM; k--)
    {
        j        = (i32)(_credo_fortuitum(&status)
            % (insignatus longus)(k + I));
        t        = ordo[k];
        ordo[k]  = ordo[j];
        ordo[j]  = t;
    }
}

/* SIGNA (credo II): vocabulum CLAUSUM - facultates domus (fabrica-7
 * annotatio 'facultas') et 'lentus'. Verbum ignotum = vitium scribae,
 * non signum novum tacitum */
hic_manens constans character* constans _credo_signa_nota[] = {
    "lentus", "rete", "fenestra", "repositorium", NIHIL
};

interior b32
_credo_signum_notum (
     constans character* verbum,
         memoriae_index  mensura)
{
    i32 i;

    per (i = ZEPHYRUM; _credo_signa_nota[i] != NIHIL; i++)
    {
        si (   strlen(_credo_signa_nota[i]) == mensura
            && strncmp(_credo_signa_nota[i], verbum, mensura)
                == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* signa (verba spatio separata) omnia nota? primum ignotum in 'ignotum'
 * (capacitas XXXII) */
interior b32
_credo_signa_probare (
    constans character* signa,
             character* ignotum)
{
    constans character* p = signa;

    dum (p != NIHIL && *p != '\0')
    {
        memoriae_index n;

        dum (*p == ' ')
        {
            p++;
        }
        n = ZEPHYRUM;
        dum (p[n] != '\0' && p[n] != ' ')
        {
            n++;
        }
        si (n > ZEPHYRUM && !_credo_signum_notum(p, n))
        {
            sprintf(ignotum, "%.*s", (integer)(n > XXXI ? XXXI : n), p);
            redde FALSUM;
        }
        p += n;
    }
    redde VERUM;
}

/* signum commune 'signa' et 'omittenda'? id in 'commune' (capacitas
 * XXXII) */
interior b32
_credo_signum_commune (
    constans character* signa,
    constans character* omittenda,
             character* commune)
{
    constans character* p = signa;

    dum (p != NIHIL && omittenda != NIHIL && *p != '\0')
    {
            memoriae_index  n;
        constans character* q = omittenda;

        dum (*p == ' ')
        {
            p++;
        }
        n = ZEPHYRUM;
        dum (p[n] != '\0' && p[n] != ' ')
        {
            n++;
        }
        dum (n > ZEPHYRUM && *q != '\0')
        {
            memoriae_index m;

            dum (*q == ' ')
            {
                q++;
            }
            m = ZEPHYRUM;
            dum (q[m] != '\0' && q[m] != ' ')
            {
                m++;
            }
            si (m == n && strncmp(p, q, n) == ZEPHYRUM)
            {
                sprintf(commune, "%.*s", (integer)(n > XXXI ? XXXI : n),
                    p);
                redde VERUM;
            }
            q += m;
        }
        p += n;
    }
    redde FALSUM;
}

/* suita recusata ante cursum (filtrum, signum ignotum): SUITA FRACTA
 * 0/0, plagula clausa */
interior vacuum
_credo_suitam_recusare (
                  FILE* plagula,
    constans character* titulus_suitae)
{
    si (plagula != NIHIL)
    {
        fputs("SUITA\t", plagula);
        _credo_literas_scribere(plagula, titulus_suitae);
        fputs("\tFRACTA\t0\t0\t0\t0\t0\n", plagula);
        fclose(plagula);
    }
}

s32
credo_suitam_currere (
      constans character* titulus_suitae,
    constans CredoSectio* sectiones)
{
    redde credo_suitam_currere_cum(titulus_suitae, sectiones, NIHIL,
        NIHIL);
}

/* purgare_suitae (si data) et piscina suitae destruitur - SEMEL, in
 * omni exitu post parare_suitae */
interior vacuum
_credo_suitam_purgare (
      CredoFunctio  purgare_suitae,
    CredoContextus* suitae)
{
    si (purgare_suitae != NIHIL)
    {
        purgare_suitae(suitae);
    }
    _credo_aream_delere(&_credo_area_suitae);
    si (suitae->piscina != NIHIL)
    {
        piscina_destruere(suitae->piscina);
        suitae->piscina = NIHIL;
    }
}

s32
credo_suitam_currere_cum (
      constans character* titulus_suitae,
    constans CredoSectio* sectiones,
            CredoFunctio  parare_suitae,
            CredoFunctio  purgare_suitae)
{
         CredoContextus  suitae;
     constans character* filtrum;
     constans character* via_verdictorum;
     constans character* omittenda;
                  FILE* plagula;
         memoriae_index  cursae;
         memoriae_index  transeuntes;
         memoriae_index  omissae;
                    b32  inventa;
              character  signum[XXXII];
                    f64  initium_suitae;
                    i32  i;
                    i32  k;
                    i32  numerus_sectionum;
                   i32* ordo;
                    b32  mixtus;
      insignatus longus  semen;

    si (_credo_piscina == NIHIL)
    {
        Piscina* piscina;

        piscina = piscina_generare_dynamicum("credo", 1048576);
        si (piscina == NIHIL)
        {
            imprimere("FATALE: credo piscina suitae fracta\n");
            redde I;
        }
        credo_aperire(piscina);
    }
    filtrum          = getenv("CREDO_SECTIO");
    via_verdictorum  = getenv("CREDO_VERDICTA");
    si (filtrum != NIHIL && filtrum[0] == '\0')
    {
        filtrum = NIHIL;
    }
    plagula = NIHIL;
    si (via_verdictorum != NIHIL && via_verdictorum[0] != '\0')
    {
        plagula = fopen(via_verdictorum, "w");
        si (plagula == NIHIL)
        {
            imprimere("FATALE: CREDO_VERDICTA non scribibilis: %s\n",
                via_verdictorum);
            redde I;
        }
    }
    inventa = (filtrum == NIHIL);
    per (i = ZEPHYRUM; sectiones[i].titulus != NIHIL && !inventa; i++)
    {
        inventa = strcmp(sectiones[i].titulus, filtrum) == ZEPHYRUM;
    }
    si (!inventa)
    {
        imprimere("FRACTA: CREDO_SECTIO '%s' - sectio nulla in suita"
            " %s\n", filtrum, titulus_suitae);
        _credo_suitam_recusare(plagula, titulus_suitae);
        redde I;
    }
    /* SIGNA (credo II): vocabulum clausum in tabula et in ambitu ANTE
     * cursum - verbum ignotum nihil omitteret tacite */
    omittenda = getenv("CREDO_SIGNA_OMITTE");
    si (omittenda != NIHIL && !_credo_signa_probare(omittenda, signum))
    {
        imprimere("FRACTA: CREDO_SIGNA_OMITTE: signum ignotum '%s'"
            " (lentus rete fenestra repositorium)\n", signum);
        _credo_suitam_recusare(plagula, titulus_suitae);
        redde I;
    }
    per (i = ZEPHYRUM; sectiones[i].titulus != NIHIL; i++)
    {
        si (!_credo_signa_probare(sectiones[i].signa, signum))
        {
            imprimere("FRACTA: signum ignotum '%s' in sectione %s"
                " (lentus rete fenestra repositorium)\n", signum,
                sectiones[i].titulus);
            _credo_suitam_recusare(plagula, titulus_suitae);
            redde I;
        }
        /* fractura exspectata sine causa = vitium scribae (credo II):
         * causa in fonte est TOTA memoria fracturae */
        si (   sectiones[i].exspectata_fracta    != NIHIL
            && sectiones[i].exspectata_fracta[0] == '\0')
        {
            imprimere("FRACTA: exspectata_fracta vacua in sectione %s"
                " (causa scribenda)\n", sectiones[i].titulus);
            _credo_suitam_recusare(plagula, titulus_suitae);
            redde I;
        }
    }
    /* ORDO (credo II): valor ignotus ordinem tacite non mutaret */
    si (!_credo_ordinem_legere(getenv("CREDO_ORDO"), &mixtus, &semen))
    {
        imprimere("FRACTA: CREDO_ORDO ignotus '%s' (tabula | mixtus |"
            " mixtus:<semen>)\n", getenv("CREDO_ORDO"));
        _credo_suitam_recusare(plagula, titulus_suitae);
        redde I;
    }
    numerus_sectionum = ZEPHYRUM;
    dum (sectiones[numerus_sectionum].titulus != NIHIL)
    {
        numerus_sectionum++;
    }
    ordo = (i32*)piscina_allocare(_credo_piscina, (memoriae_index)(
        numerus_sectionum + I) * magnitudo(i32));
    si (ordo == NIHIL)
    {
        imprimere("FATALE: credo ordo sectionum\n");
        _credo_suitam_recusare(plagula, titulus_suitae);
        redde I;
    }
    _credo_ordinem_parare(ordo, numerus_sectionum, mixtus, semen);
    /* SECTIONES (analytica T4): numerus declaratus PRIMUM - lector post
     * ruinam 'N non cursae' computat (filtrum: congruentes solae) */
    si (plagula != NIHIL)
    {
        insignatus longus declaratae = ZEPHYRUM;

        per (i = ZEPHYRUM; i < numerus_sectionum; i++)
        {
            si (   filtrum                               == NIHIL
                || strcmp(sectiones[i].titulus, filtrum) == ZEPHYRUM)
            {
                declaratae++;
            }
        }
        fputs("SECTIONES\t", plagula);
        _credo_literas_scribere(plagula, titulus_suitae);
        fprintf(plagula, "\t%lu\n", declaratae);
        (vacuum)fflush(plagula);
    }
    si (mixtus)
    {
        /* semen NOMINATUM: cursus fractus eodem semine iteratur */
        imprimere("\n=== ORDO mixtus semen %lu ===\n", semen);
        si (plagula != NIHIL)
        {
            fputs("ORDO\t", plagula);
            _credo_literas_scribere(plagula, titulus_suitae);
            fprintf(plagula, "\tmixtus\t%lu\n", semen);
        }
    }
    cursae          = ZEPHYRUM;
    transeuntes     = ZEPHYRUM;
    omissae         = ZEPHYRUM;
    initium_suitae  = _credo_ms_nunc();
    /* FIXURA SUITAE (credo II): parare_suitae per viam sectionis
     * (NECESSE capitur); fractum -> nulla sectio, '(parare suitae)'
     * notatur, purgare_suitae tamen */
    memset(&suitae, ZEPHYRUM, magnitudo(suitae));
    suitae.titulus = titulus_suitae;
    _credo_aream_ponere(&_credo_area_suitae, &suitae, titulus_suitae,
        "suita");
    si (parare_suitae != NIHIL || purgare_suitae != NIHIL)
    {
        suitae.piscina = piscina_generare_dynamicum(titulus_suitae,
            1048576);
        si (suitae.piscina == NIHIL)
        {
            imprimere("FATALE: piscina suitae %s fracta\n",
                titulus_suitae);
            redde I;
        }
    }
    si (parare_suitae != NIHIL)
    {
                 CredoSectio  pseudo;
                CredoNotatio* ante;
         CredoSectionisSumma  summa;
                         b32  aborta;

        memset(&pseudo, ZEPHYRUM, magnitudo(pseudo));
        pseudo.titulus = "(parare suitae)";
        pseudo.probare = parare_suitae;
        imprimere("\n--- Parans suitam %s ---\n", titulus_suitae);
        ante    = _credo_nunc;
        aborta  = _credo_sectionem_agere(&pseudo, &suitae);
        summa   = _credo_sectionem_summare(ante, aborta);
        si (   strcmp(summa.exitus, "FRACTA") == ZEPHYRUM
            || strcmp(summa.exitus, "ABORTA") == ZEPHYRUM)
        {
            imprimere("\n  parare suitae: %s - nulla sectio currit\n",
                summa.exitus);
            si (plagula != NIHIL)
            {
                _credo_sectionem_scribere(plagula, titulus_suitae,
                    pseudo.titulus, summa, _credo_ms_nunc()
                    - initium_suitae);
            }
            _credo_suitam_purgare(purgare_suitae, &suitae);
            credo_imprimere_compendium();
            si (plagula != NIHIL)
            {
                fputs("SUITA\t", plagula);
                _credo_literas_scribere(plagula, titulus_suitae);
                fprintf(plagula, "\tFRACTA\t0\t0\t%lu\t%lu\t%.0f\n",
                    (insignatus longus)credo_numerus_praeteriti(),
                    (insignatus longus)credo_numerus_totalis(),
                    _credo_ms_nunc() - initium_suitae);
                fclose(plagula);
            }
            redde I;
        }
    }
    per (k = ZEPHYRUM; k < numerus_sectionum; k++)
    {
        constans CredoSectio* sectio;
              CredoContextus  contextus;
                CredoNotatio* ante;
         CredoSectionisSumma  summa;
                         b32  aborta;
                         f64  initium;

        /* i = sedes in tabula (area '<suita>.<i+1>.<pid>' stabilis) */
        i       = ordo[k];
        sectio  = &sectiones[i];
        si (   filtrum != NIHIL && strcmp(sectio->titulus, filtrum)
                != ZEPHYRUM)
        {
            perge;
        }
        /* OMISSA (credo II): signum in CREDO_SIGNA_OMITTE - nominata in
         * effusione et plagula, numquam tacita, numquam numerata ut
         * cursa */
        si (_credo_signum_commune(sectio->signa, omittenda, signum))
        {
            imprimere("\n--- OMISSA %s (signum %s) ---\n",
                sectio->titulus,
                signum);
            omissae++;
            si (plagula != NIHIL)
            {
                summa.exitus      = "OMISSA";
                summa.praeteriti  = ZEPHYRUM;
                summa.totales     = ZEPHYRUM;
                summa.fractura    = NIHIL;
                _credo_sectionem_scribere(plagula, titulus_suitae,
                    sectio->titulus, summa, 0.0);
            }
            perge;
        }
        /* INCIPIT (analytica T4) EFFLUXUM ante sectionem: ruina vera
         * (SEGV, KILL) nihil effluit - sectio incepta sine linea SECTIO
         * = sectio rupta, a lectore nominata. Nullum manubrium signi
         * (vide 'Assertiones processus'). */
        si (plagula != NIHIL)
        {
            fputs("INCIPIT\t", plagula);
            _credo_literas_scribere(plagula, titulus_suitae);
            fputc('\t', plagula);
            _credo_literas_scribere(plagula, sectio->titulus);
            fputc('\n', plagula);
            (vacuum)fflush(plagula);
        }
        imprimere("\n--- Probans %s ---\n", sectio->titulus);
        contextus.piscina  = piscina_generare_dynamicum(sectio->titulus,
            262144);
        contextus.datum    = NIHIL;
        contextus.titulus  = sectio->titulus;
        contextus.suita    = suitae.datum;
        si (contextus.piscina == NIHIL)
        {
            imprimere("FATALE: piscina sectionis %s fracta\n",
                sectio->titulus);
            _credo_suitam_purgare(purgare_suitae, &suitae);
            redde I;
        }
        sprintf(signum, "%lu", (insignatus longus)(i + I));
        _credo_aream_ponere(&_credo_area_sectionis, &contextus,
            titulus_suitae, signum);
        ante           = _credo_nunc;
        initium        = _credo_ms_nunc();
        _credo_gradus  = NIHIL;
        aborta         = _credo_sectionem_agere(sectio, &contextus);
        _credo_gradus  = NIHIL;
        si (sectio->purgare != NIHIL)
        {
            sectio->purgare(&contextus);
        }
        _credo_aream_delere(&_credo_area_sectionis);
        summa = _credo_sectionem_summare(ante, aborta);
        piscina_destruere(contextus.piscina);
        cursae++;
        /* FRACTURA EXSPECTATA (credo II): fracta -> NOTA_FRACTA (suita
         * transit, fractura servata); transiens -> INOPINATA (suita
         * frangitur: fractura sanata, linea delenda); VACUA manet */
        si (sectio->exspectata_fracta != NIHIL)
        {
            si (   strcmp(summa.exitus, "FRACTA") == ZEPHYRUM
                || strcmp(summa.exitus, "ABORTA") == ZEPHYRUM)
            {
                _credo_fracti_exspectati += summa.totales
                    - summa.praeteriti;
                summa.exitus = "NOTA_FRACTA";
            }
            alioquin si (strcmp(summa.exitus, "TRANSIIT") == ZEPHYRUM)
            {
                summa.exitus = "INOPINATA";
            }
        }
        si (strcmp(summa.exitus, "TRANSIIT") == ZEPHYRUM)
        {
            transeuntes++;
        }
        alioquin si (strcmp(summa.exitus, "NOTA_FRACTA") == ZEPHYRUM)
        {
            transeuntes++;
            imprimere("\n  sectio %s: NOTA_FRACTA (%lu/%lu) - causa:"
                " %s\n",
                sectio->titulus, (insignatus longus)summa.praeteriti,
                (insignatus longus)summa.totales,
                sectio->exspectata_fracta);
        }
        alioquin si (strcmp(summa.exitus, "INOPINATA") == ZEPHYRUM)
        {
            imprimere("\n  sectio %s: INOPINATA - fractura exspectata "
                "sanata ('%s'): linea exspectata_fracta delenda\n",
                sectio->titulus, sectio->exspectata_fracta);
        }
        alioquin
        {
            imprimere("\n  sectio %s: %s (%lu/%lu)\n", sectio->titulus,
                summa.exitus, (insignatus longus)summa.praeteriti,
                (insignatus longus)summa.totales);
        }
        si (plagula != NIHIL)
        {
            _credo_sectionem_scribere(plagula, titulus_suitae,
                sectio->titulus, summa, _credo_ms_nunc() - initium);
        }
        (vacuum)fflush(stdout);
    }
    _credo_suitam_purgare(purgare_suitae, &suitae);
    si (omissae > ZEPHYRUM)
    {
        imprimere("\n=== SECTIONES: %lu/%lu transierunt (%lu omissae)"
            " ===\n",
            (insignatus longus)transeuntes, (insignatus longus)cursae,
            (insignatus longus)omissae);
    }
    alioquin
    {
        imprimere("\n=== SECTIONES: %lu/%lu transierunt ===\n",
            (insignatus longus)transeuntes, (insignatus longus)cursae);
    }
    credo_imprimere_compendium();
    si (plagula != NIHIL)
    {
        fputs("SUITA\t", plagula);
        _credo_literas_scribere(plagula, titulus_suitae);
        /* VACUA (credo II): nulla sectio cursa (omnes omissae) - non
         * transitus */
        fprintf(plagula, "\t%s\t%lu\t%lu\t%lu\t%lu\t%.0f\n",
            cursae == ZEPHYRUM ? "VACUA"
            : (transeuntes == cursae
                && credo_numerus_fracti() == _credo_fracti_exspectati)
                ? "TRANSIIT" : "FRACTA",
            (insignatus longus)transeuntes, (insignatus longus)cursae,
            (insignatus longus)credo_numerus_praeteriti(),
            (insignatus longus)credo_numerus_totalis(),
            _credo_ms_nunc() - initium_suitae);
        fclose(plagula);
    }
    redde (transeuntes == cursae && cursae > ZEPHYRUM
           && credo_numerus_fracti()
               == _credo_fracti_exspectati) ? ZEPHYRUM : I;
}
