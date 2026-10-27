/* probatio_acordum.c - Probationes acordi (include/acordum.h)
 *
 * Classes: C 0, C# 1, D 2, D# 3, E 4, F 5, F# 6, G 7, G# 8, A 9, A# 10,
 * B 11. Agnitio (exemplaria omnia a C; transposita, inversiones,
 * radices plures - bassus decernit, ignota; sine tertia) et lectio
 * nominis (suffixum longissimum, litterae, recusata, circuitus per
 * tabulam totam).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "acordum.h"
#include <stdio.h>
#include <string.h>

/* classes ex indice -1 terminato */
interior i32
_cl (
    constans s32* classes)
{
    i32 b = ZEPHYRUM;

    dum (*classes >= ZEPHYRUM)
    {
        b |= (i32)I << (i32)*classes;
        classes++;
    }
    redde b;
}

/* classes transpositae (sursum r semitonia) */
interior i32
_rotare_classes (
    i32 classes,
    i32 r)
{
    redde ((classes << r) | (classes >> (XII - r))) & 0xFFF;
}

interior b32
_est (
     constans s32* classes,
              s32  bassus,
     AcordumGenus  genus,
              s32  radix)
{
    Acordum h = acordum_ex_classibus(_cl(classes), (i32)bassus);

    si (h.genus == genus && (s32)h.radix == radix)
    {
        redde VERUM;
    }
    imprimere("  genus %u radix %u pro %u %d\n", (i32)h.genus, h.radix,
              (i32)genus, (integer)radix);
    redde FALSUM;
}


    /* ==================================================
     * EXEMPLARIA: omnia a C; transposita; dyades; unum; nihil; ignotum
     * ================================================== */

interior vacuum
_sectio_exemplaria (
    CredoContextus* c)
{
    hic_manens constans s32 MAIOR[]      = { 0, 4, 7, -1 };
    hic_manens constans s32 MINOR[]      = { 0, 3, 7, -1 };
    hic_manens constans s32 DIM[]        = { 0, 3, 6, -1 };
    hic_manens constans s32 AUG[]        = { 0, 4, 8, -1 };
    hic_manens constans s32 SEPT[]       = { 0, 4, 7, 10, -1 };
    hic_manens constans s32 SEPT_SINE[]  = { 0, 4, 10, -1 };
    hic_manens constans s32 SEPTIMA_MAIOR_[]     = { 0, 4, 7, 11,
        -1 };
    hic_manens constans s32 SEPTIMA_MINOR_[]     = { 0, 3, 7, 10,
        -1 };
    hic_manens constans s32 SEMI[]               = { 0, 3, 6, 10, -1 };
    hic_manens constans s32 DIMINUTA_SEPTIMA_[]  = { 0, 3, 6, 9,
        -1 };
    hic_manens constans s32 SUSPENSA_IV_[]  = { 0, 5, 7, -1 };
    hic_manens constans s32 SUSPENSA_II_[]  = { 0, 2, 7, -1 };
    hic_manens constans s32 QUINTA[]        = { 0, 7, -1 };
    hic_manens constans s32 TERTIA[]        = { 0, 4, -1 };
    hic_manens constans s32 TERTIA_M[]      = { 0, 3, -1 };
    hic_manens constans s32 UNUM[]          = { 0, -1 };
    hic_manens constans s32 NIHIL_[]        = { -1 };
    hic_manens constans s32 IGNOTUM[]       = { 0, 1, 2, -1 };
    hic_manens constans s32 D_MAIOR[]       = { 2, 6, 9, -1 };

    (vacuum)c;
    CREDO_VERUM (_est(MAIOR, 0, ACORDUM_MAIOR, 0));
    CREDO_VERUM (_est(MINOR, 0, ACORDUM_MINOR, 0));
    CREDO_VERUM (_est(DIM, 0, ACORDUM_DIMINUTA, 0));
    CREDO_VERUM (_est(AUG, 0, ACORDUM_AUCTA, 0));
    CREDO_VERUM (_est(SEPT, 0, ACORDUM_SEPTIMA, 0));
    CREDO_VERUM (_est(SEPT_SINE, 0, ACORDUM_SEPTIMA, 0));
    CREDO_VERUM (_est(SEPTIMA_MAIOR_, 0, ACORDUM_SEPTIMA_MAIOR, 0));
    CREDO_VERUM (_est(SEPTIMA_MINOR_, 0, ACORDUM_SEPTIMA_MINOR, 0));
    CREDO_VERUM (_est(SEMI, 0, ACORDUM_DIMIDIA_DIMINUTA, 0));
    CREDO_VERUM (_est(DIMINUTA_SEPTIMA_, 0, ACORDUM_DIMINUTA_SEPTIMA,
        0));
    CREDO_VERUM (_est(SUSPENSA_IV_, 0, ACORDUM_SUSPENSA_IV, 0));
    CREDO_VERUM (_est(SUSPENSA_II_, 0, ACORDUM_SUSPENSA_II, 0));
    CREDO_VERUM (_est(QUINTA, 0, ACORDUM_QUINTA, 0));
    CREDO_VERUM (_est(TERTIA, 0, ACORDUM_TERTIA_MAIOR, 0));
    CREDO_VERUM (_est(TERTIA_M, 0, ACORDUM_TERTIA_MINOR, 0));
    CREDO_VERUM (_est(UNUM, 0, ACORDUM_CLASSIS_UNA, 0));
    CREDO_VERUM (_est(NIHIL_, 0, ACORDUM_NULLA, 0));
    CREDO_VERUM (_est(IGNOTUM, 0, ACORDUM_IGNOTA, 0));
    CREDO_VERUM (_est(D_MAIOR, 2, ACORDUM_MAIOR, 2));
}

/* radices plures: bassus decernit; inversio; sexta solum radice in
 * basso (C6; aliter Am7) */
interior vacuum
_sectio_radices (
    CredoContextus* c)
{
    hic_manens constans s32 G_B[]              = { 7, 11, 2, -1 };
    hic_manens constans s32 A_SEPTIMA_MINOR[]  = { 9, 0, 4, 7, -1 };
    hic_manens constans s32 AUG[]              = { 0, 4, 8, -1 };
    hic_manens constans s32 DIMINUTA_SEPTIMA_[]  = { 2, 5, 8, 11,
        -1 };
    hic_manens constans s32 SUS[]                = { 0, 2, 7, -1 };
                    Acordum h;

    (vacuum)c;
    CREDO_VERUM (_est(G_B, 11, ACORDUM_MAIOR, 7));
    h = acordum_ex_classibus((I << VII) | (I << XI) | (I << II), XI);
    CREDO_AEQUALIS_I32 (h.bassus, XI);
    /* C E G A: sexta solum radice in basso (C6); super A Am7, super E
     * aut G Am7 inversum */
    CREDO_VERUM (_est(A_SEPTIMA_MINOR, 0, ACORDUM_SEXTA, 0));
    CREDO_VERUM (_est(A_SEPTIMA_MINOR, 9, ACORDUM_SEPTIMA_MINOR, 9));
    CREDO_VERUM (_est(A_SEPTIMA_MINOR, 4, ACORDUM_SEPTIMA_MINOR, 9));
    CREDO_VERUM (_est(A_SEPTIMA_MINOR, 7, ACORDUM_SEPTIMA_MINOR, 9));
    CREDO_VERUM (_est(AUG, 4, ACORDUM_AUCTA, 4));
    CREDO_VERUM (_est(DIMINUTA_SEPTIMA_, 5, ACORDUM_DIMINUTA_SEPTIMA,
        5));
    /* C D G: Csus2 basso C, Gsus4 basso G */
    CREDO_VERUM (_est(SUS, 0, ACORDUM_SUSPENSA_II, 0));
    CREDO_VERUM (_est(SUS, 7, ACORDUM_SUSPENSA_IV, 7));
}


/* sine tertia: exemplaria IV ab omni radice; propinqua cum tertia aut
 * suspensa non */
interior vacuum
_sectio_sine_tertia (
    CredoContextus* c)
{
    hic_manens constans s32 FORMAE[IV][IV] = {
        { 0, -1, -1, -1 },
        { 0, 7, -1, -1 },
        { 0, 7, 10, -1 },
        { 0, 7, 11, -1 }
    };
    hic_manens constans s32 CUM_TERTIA[][V] = {
        { 0, 4, 7, -1, -1 },
        { 0, 3, 7, -1, -1 },
        { 0, 4, 7, 10, -1 },
        { 0, 4, 10, -1, -1 },
        { 0, 4, -1, -1, -1 },
        { 0, 3, -1, -1, -1 },
        { 0, 2, 7, -1, -1 },
        { 0, 5, 7, -1, -1 },
        { 0, 10, -1, -1, -1 },
        { 0, 1, 2, -1, -1 },
        { -1, -1, -1, -1, -1 }
    };
    i32 f;
    i32 r;
    i32 radix;

    (vacuum)c;
    per (f = ZEPHYRUM; f < IV; f++)
    {
        per (r = ZEPHYRUM; r < XII; r++)
        {
            i32 cl = _rotare_classes(_cl(FORMAE[f]), r);

            radix = XCIX;
            CREDO_VERUM (acordum_sine_tertia(cl, &radix));
            CREDO_AEQUALIS_I32 (radix, r);
        }
    }
    /* bits supra XII ignorantur: F (5) cum bit XIII - sine larva F
     * et 'C#' (XIII - V = VIII) {0,8} legeretur */
    radix = XCIX;
    CREDO_VERUM (acordum_sine_tertia(((i32)I << V) | ((i32)I << XIII),
                                      &radix));
    CREDO_AEQUALIS_I32 (radix, V);
    per (f = ZEPHYRUM; f < (i32)(magnitudo(CUM_TERTIA)
                                / magnitudo(CUM_TERTIA[ZEPHYRUM])); f++)
    {
        per (r = ZEPHYRUM; r < XII; r++)
        {
            CREDO_FALSUM (acordum_sine_tertia(
                _rotare_classes(_cl(CUM_TERTIA[f]), r), &radix));
        }
    }
}


/* nomen -> acordum */
interior b32
_lege (
         CredoContextus* c,
     constans character* titulus,
                Acordum* a)
{
    redde acordum_legere(chorda_ex_literis(titulus, c->piscina), a);
}

/* nomen legitur cum genere, radice, basso, classibus */
interior b32
_lectum (
          CredoContextus* c,
      constans character* titulus,
            AcordumGenus  genus,
                     i32  radix,
                     i32  bassus,
            constans s32* classes)
{
    Acordum a;

    si (!_lege(c, titulus, &a))
    {
        imprimere("  '%s' non lectum\n", titulus);
        redde FALSUM;
    }
    si (   a.genus == genus && a.radix == radix && a.bassus == bassus
        && (classes == NIHIL || a.classes == _cl(classes)))
    {
        redde VERUM;
    }
    imprimere("  '%s': genus %u radix %u bassus %u classes %x\n",
              titulus, (i32)a.genus, a.radix, a.bassus, a.classes);
    redde FALSUM;
}


    /* ==================================================
     * LECTIO: nomina scripta harmoniae; bassus extra classes
     * ================================================== */

interior vacuum
_sectio_lectio (
    CredoContextus* c)
{
    hic_manens constans s32 C_MAIOR[]    = { 0, 4, 7, -1 };
    hic_manens constans s32 G_MAIOR[]    = { 7, 11, 2, -1 };
    hic_manens constans s32 A_SEPTIMA[]  = { 9, 1, 4, 7, -1 };
    hic_manens constans s32 FIS_DIM[]    = { 6, 9, 0, 3, -1 };
    hic_manens constans s32 B_SEMI[]     = { 10, 1, 4, 8, -1 };
    hic_manens constans s32 A_QUINTA[]   = { 9, 4, -1 };
    hic_manens constans s32 E_TERTIA[]   = { 4, 7, -1 };

    CREDO_VERUM (_lectum(c, "C", ACORDUM_MAIOR, 0, 0, C_MAIOR));
    CREDO_VERUM (_lectum(c, "G/B", ACORDUM_MAIOR, 7, 11, G_MAIOR));
    CREDO_VERUM (_lectum(c, "A7", ACORDUM_SEPTIMA, 9, 9, A_SEPTIMA));
    CREDO_VERUM (_lectum(c, "F#dim7", ACORDUM_DIMINUTA_SEPTIMA, 6, 6,
                         FIS_DIM));
    CREDO_VERUM (_lectum(c, "Bbm7b5", ACORDUM_DIMIDIA_DIMINUTA, 10, 10,
                         B_SEMI));
    CREDO_VERUM (_lectum(c, "A(5)", ACORDUM_QUINTA, 9, 9, A_QUINTA));
    CREDO_VERUM (_lectum(c, "Em(3)", ACORDUM_TERTIA_MINOR, 4, 4,
                         E_TERTIA));
    /* bassus transiens: D extra classes */
    CREDO_VERUM (_lectum(c, "C/D", ACORDUM_MAIOR, 0, 2, C_MAIOR));
    /* alterationes */
    CREDO_VERUM (_lectum(c, "C##", ACORDUM_MAIOR, 2, 2, NIHIL));
    CREDO_VERUM (_lectum(c, "Cbb", ACORDUM_MAIOR, 10, 10, NIHIL));
    CREDO_VERUM (_lectum(c, "Cb", ACORDUM_MAIOR, 11, 11, NIHIL));
    CREDO_VERUM (_lectum(c, "B#", ACORDUM_MAIOR, 0, 0, NIHIL));
    CREDO_VERUM (_lectum(c, "Db", ACORDUM_MAIOR, 1, 1, NIHIL));
    CREDO_VERUM (_lectum(c, "C#m", ACORDUM_MINOR, 1, 1, NIHIL));
    /* vocabularium additum */
    {
        hic_manens constans s32 C_SEXTA[]     = { 0, 4, 7, 9, -1 };
        hic_manens constans s32 C_NONA_ADD[]  = { 0, 2, 4, 7, -1 };
        hic_manens constans s32 G_NONA[]      = { 7, 11, 2, 5, 9, -1 };
        hic_manens constans s32 G_NONA_MIN[]  = { 7, 11, 2, 5, 8, -1 };
        hic_manens constans s32 G_SEPT_SUS[]  = { 7, 0, 2, 5, -1 };
        hic_manens constans s32 C_SEPT_AUG[]  = { 0, 4, 8, 10, -1 };

        CREDO_VERUM (_lectum(c, "C6", ACORDUM_SEXTA, 0, 0, C_SEXTA));
        CREDO_VERUM (_lectum(c, "Am6", ACORDUM_SEXTA_MINOR, 9, 9,
                             NIHIL));
        CREDO_VERUM (_lectum(c, "Cadd9", ACORDUM_NONA_ADDITA, 0, 0,
                             C_NONA_ADD));
        CREDO_VERUM (_lectum(c, "G9", ACORDUM_NONA, 7, 7, G_NONA));
        CREDO_VERUM (_lectum(c, "G7b9", ACORDUM_NONA_MINOR, 7, 7,
                             G_NONA_MIN));
        CREDO_VERUM (_lectum(c, "Fmaj9", ACORDUM_NONA_MAIOR, 5, 5,
                             NIHIL));
        CREDO_VERUM (_lectum(c, "Dm9", ACORDUM_NONA_MOLLIS, 2, 2,
            NIHIL));
        CREDO_VERUM (_lectum(c, "G7sus4", ACORDUM_SEPTIMA_SUSPENSA, 7,
            7,
                             G_SEPT_SUS));
        CREDO_VERUM (_lectum(c, "C7#5", ACORDUM_SEPTIMA_AUCTA, 0, 0,
                             C_SEPT_AUG));
    }
}

/* suffixum longissimum: "maj7" non "m" + reliquum; "7" = exemplar
 * primum (quattuor classes) */
interior vacuum
_sectio_suffixa (
    CredoContextus* c)
{
    hic_manens constans s32 C_SEPTIMA[] = { 0, 4, 7, 10, -1 };

    CREDO_VERUM (_lectum(c, "Cmaj7", ACORDUM_SEPTIMA_MAIOR, 0, 0,
                         NIHIL));
    CREDO_VERUM (_lectum(c, "Cm7b5", ACORDUM_DIMIDIA_DIMINUTA, 0, 0,
                         NIHIL));
    CREDO_VERUM (_lectum(c, "Cm7", ACORDUM_SEPTIMA_MINOR, 0, 0, NIHIL));
    CREDO_VERUM (_lectum(c, "Cm", ACORDUM_MINOR, 0, 0, NIHIL));
    CREDO_VERUM (_lectum(c, "Cm(3)", ACORDUM_TERTIA_MINOR, 0, 0,
        NIHIL));
    CREDO_VERUM (_lectum(c, "Cdim7", ACORDUM_DIMINUTA_SEPTIMA, 0, 0,
                         NIHIL));
    CREDO_VERUM (_lectum(c, "Cdim", ACORDUM_DIMINUTA, 0, 0, NIHIL));
    CREDO_VERUM (_lectum(c, "Caug", ACORDUM_AUCTA, 0, 0, NIHIL));
    CREDO_VERUM (_lectum(c, "Csus4", ACORDUM_SUSPENSA_IV, 0, 0, NIHIL));
    CREDO_VERUM (_lectum(c, "Csus2", ACORDUM_SUSPENSA_II, 0, 0, NIHIL));
    CREDO_VERUM (_lectum(c, "C7", ACORDUM_SEPTIMA, 0, 0, C_SEPTIMA));
    /* longissimum inter nova: "7sus4" non "7", "maj9" non "m", "7b9"
     * non "7", "m6" non "m" */
    CREDO_VERUM (_lectum(c, "C7sus4", ACORDUM_SEPTIMA_SUSPENSA, 0, 0,
                         NIHIL));
    CREDO_VERUM (_lectum(c, "Cmaj9", ACORDUM_NONA_MAIOR, 0, 0, NIHIL));
    CREDO_VERUM (_lectum(c, "C7b9", ACORDUM_NONA_MINOR, 0, 0, NIHIL));
    CREDO_VERUM (_lectum(c, "Cm6", ACORDUM_SEXTA_MINOR, 0, 0, NIHIL));
}

/* litterae ex nomine: radix scripta, ceterae gradibus litterarum */
interior vacuum
_sectio_litterae (
    CredoContextus* c)
{
    Acordum a;

    CREDO_NECESSE (_lege(c, "C#m", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[1], (i32)'c');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[4], (i32)'e');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[8], (i32)'g');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[0], ZEPHYRUM);
    CREDO_NECESSE (_lege(c, "Bdim7", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[11], (i32)'b');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[2], (i32)'d');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[5], (i32)'f');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[8], (i32)'a');
    CREDO_NECESSE (_lege(c, "Caug", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[8], (i32)'g');
    CREDO_NECESSE (_lege(c, "Bbm7b5", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[10], (i32)'b');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[1], (i32)'d');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[4], (i32)'f');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[8], (i32)'a');
    CREDO_NECESSE (_lege(c, "Fsus4", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[10], (i32)'b');
    CREDO_NECESSE (_lege(c, "Dsus2", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[4], (i32)'e');
    CREDO_NECESSE (_lege(c, "Db", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[1], (i32)'d');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[5], (i32)'f');
    CREDO_AEQUALIS_I32 ((i32)a.litterae[8], (i32)'a');
    /* bassus ut scriptus, etiam extra classes */
    CREDO_NECESSE (_lege(c, "G/B", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[11], (i32)'b');
    CREDO_NECESSE (_lege(c, "C/D", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[2], (i32)'d');
    /* gradus litterarum per exemplar: sexta +5 (C6 A, Bbm6 G), nona
     * minor (C7b9 Db), quinta aucta (F#7#5 C##) */
    CREDO_NECESSE (_lege(c, "C6", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[9], (i32)'a');
    CREDO_NECESSE (_lege(c, "Bbm6", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[7], (i32)'g');
    CREDO_NECESSE (_lege(c, "C7b9", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[1], (i32)'d');
    CREDO_NECESSE (_lege(c, "F#7#5", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[2], (i32)'c');
    CREDO_NECESSE (_lege(c, "G9", &a));
    CREDO_AEQUALIS_I32 ((i32)a.litterae[9], (i32)'a');
}

/* nomina mala: FALSUM, acordum non mutatur */
interior vacuum
_sectio_recusata (
    CredoContextus* c)
{
    hic_manens constans character* MALA[] = {
        "", "c", "H", "Cx", "Cm7x", "C/", "C/h", "(E)", "[C E]", "r",
        "C#b", "C###", "Cbbb", "C/D/E", " C", "Cmaj", NIHIL
    };
    Acordum a;
        i32 i;

    per (i = ZEPHYRUM; MALA[i] != NIHIL; i++)
    {
        a.genus = ACORDUM_IGNOTA;
        a.radix = XCIX;
        si (_lege(c, MALA[i], &a))
        {
            imprimere("  '%s' lectum\n", MALA[i]);
        }
        CREDO_FALSUM (_lege(c, MALA[i], &a));
        CREDO_AEQUALIS_I32 (a.radix, XCIX);
        CREDO_AEQUALIS_I32 ((i32)a.genus, (i32)ACORDUM_IGNOTA);
    }
}

/* circuitus: genus omne nominatum ab omni radice - suffixum scriptum
 * legitur, classes agnoscuntur ut idem genus */
interior vacuum
_sectio_circuitus (
    CredoContextus* c)
{
    hic_manens constans character* RADICES[XII] = {
        "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"
    };
    character scriptum[XVI];
      Acordum a;
      Acordum agnitum;
          i32 g;
          i32 r;

    per (g = ZEPHYRUM; g < (i32)ACORDUM_CLASSIS_UNA; g++)
    {
        per (r = ZEPHYRUM; r < XII; r++)
        {
            sprintf(scriptum, "%s%s", RADICES[r],
                    acordum_suffixum((AcordumGenus)g));
            CREDO_NECESSE (_lege(c, scriptum, &a));
            CREDO_AEQUALIS_I32 ((i32)a.genus, g);
            CREDO_AEQUALIS_I32 (a.radix, r);
            CREDO_AEQUALIS_I32 (a.bassus, r);
            agnitum = acordum_ex_classibus(a.classes, r);
            CREDO_AEQUALIS_I32 ((i32)agnitum.genus, g);
            CREDO_AEQUALIS_I32 (agnitum.radix, r);
        }
    }
    CREDO_CHORDA_AEQUALIS_LITERIS (
        chorda_ex_literis(acordum_suffixum(ACORDUM_NULLA), c->piscina),
        "");
}

/* proprietates: ordines quidam; genus extra = nulla; intervalla
 * (tertia, quinta, septima) in classibus acordi lecti (C + suffixum)
 * - tabula proprietatum exemplaribus congruit */
interior vacuum
_sectio_proprietates (
    CredoContextus* c)
{
    constans AcordumProprietates* p;
                         Acordum  a;
                       character  scriptum[XVI];
                             i32  g;

    p = acordum_proprietates(ACORDUM_SEPTIMA_MINOR);
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(p->suffixum,
        c->piscina), "m7");
    CREDO_AEQUALIS_S32 (p->tertia, III);
    CREDO_AEQUALIS_S32 (p->quinta, VII);
    CREDO_AEQUALIS_S32 (p->septima, X);
    CREDO_AEQUALIS_I32 (p->gradus, III);
    p = acordum_proprietates(ACORDUM_DIMINUTA_SEPTIMA);
    CREDO_AEQUALIS_S32 (p->septima, IX);
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(p->numeralis,
        c->piscina), "o");
    p = acordum_proprietates(ACORDUM_SUSPENSA_IV);
    CREDO_AEQUALIS_S32 (p->tertia, -I);
    CREDO_AEQUALIS_I32 (p->gradus, II);
    p = acordum_proprietates(ACORDUM_TERTIA_MINOR);
    CREDO_AEQUALIS_I32 (p->gradus, I);
    CREDO_AEQUALIS_S32 (p->quinta, -I);
    p = acordum_proprietates((AcordumGenus)XCIX);
    CREDO_AEQUALIS_I32 (p->gradus, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (p->tertia, -I);
    per (g = ZEPHYRUM; g < (i32)ACORDUM_CLASSIS_UNA; g++)
    {
        p = acordum_proprietates((AcordumGenus)g);
        sprintf(scriptum, "C%s", p->suffixum);
        CREDO_NECESSE (_lege(c, scriptum, &a));
        CREDO_VERUM (p->tertia < 0
                     || (a.classes & ((i32)I << p->tertia)) != 0);
        CREDO_VERUM (p->quinta < 0
                     || (a.classes & ((i32)I << p->quinta)) != 0);
        CREDO_VERUM (p->septima < 0
                     || (a.classes & ((i32)I << p->septima)) != 0);
        CREDO_VERUM (p->gradus > ZEPHYRUM);
        CREDO_AEQUALIS_I32 (p->additae & ~a.classes, ZEPHYRUM);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "exemplaria", _sectio_exemplaria, NIHIL, NIHIL, NIHIL, NIHIL },
    { "radices", _sectio_radices, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sine_tertia", _sectio_sine_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "lectio", _sectio_lectio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "suffixa", _sectio_suffixa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "litterae", _sectio_litterae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusata", _sectio_recusata, NIHIL, NIHIL, NIHIL, NIHIL },
    { "circuitus", _sectio_circuitus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "proprietates", _sectio_proprietates, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("acordum", SECTIONES);
}
