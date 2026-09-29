/* probatio_runae_graphemata.c - runae: graphemata (U4)
 *
 * I.   GraphemeBreakTest.txt (Unicode 15.1, fixum): omnis linea, omnis
 *      limes interior per runae_rumpitur (status novus per lineam).
 *      '÷' (U+00F7) = limes, '×' (U+00D7) = nullus.
 * II.  Latitudo graphematum: casus probationum Ghostty
 *      (../ghostty/src/unicode/grapheme.zig, graphemeWidth) portati.
 * III. Aurum graphematum (oraculum ICU, tools/runae_oraculum.sh
 *      -graphemata): pro lingua quaque corporis numerus graphematum et
 *      friatio FNV-1a limitum (offsets octetorum) aequales esse debent.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "utf8.h"
#include "runae.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define PROBATIO_VIA "probationes/fixa/unicode/15.1.0/GraphemeBreakTest.txt"
#define AURUM_VIA "probationes/fixa/runae/aurum_graphemata.txt"
#define RUNAE_LINEAE_MAXIMAE XXXII
#define FRACTURAE_IMPRIMENDAE X


/* ==================================================
 * I. GraphemeBreakTest
 * ================================================== */

/* Lineam unam iudicare: "÷ 0020 × 0308 ÷ 0020 ÷ # ..." */
interior b32
_lineam_probare (
    constans i8* linea,
            i32  mensura,
            i32  numerus_lineae,
            i32* fracturae)
{
    s32 runae[RUNAE_LINEAE_MAXIMAE];
    b32 limites[RUNAE_LINEAE_MAXIMAE + I];   /* limes ANTE runam k */
    i32 n     = ZEPHYRUM;
    i32 k     = ZEPHYRUM;
    b32 bona  = VERUM;
    RunaeRuptura ruptura;

    dum (k < mensura && linea[k] != '#')
    {
        /* '÷' = C3 B7, '×' = C3 97 */
        si (   k + I < mensura && linea[k] == 0xC3
            && (linea[k + I] == 0xB7 || linea[k + I] == 0x97))
        {
            si (n < RUNAE_LINEAE_MAXIMAE + I)
            {
                limites[n] = (b32)(linea[k + I] == 0xB7);
            }
            k += II;
        }
        alioquin si (   (linea[k] >= '0' && linea[k] <= '9')
                     || (linea[k] >= 'A' && linea[k] <= 'F'))
        {
            s32 v = ZEPHYRUM;

            dum (   k < mensura
                 && (   (linea[k] >= '0' && linea[k] <= '9')
                     || (linea[k] >= 'A' && linea[k] <= 'F')))
            {
                v = v * XVI + ((linea[k] <= '9') ? (s32)(linea[k] - '0')
                                                 : (s32)(linea[k] - 'A')
                                                     + X);
                k++;
            }
            si (n < RUNAE_LINEAE_MAXIMAE)
            {
                runae[n++] = v;
            }
        }
        alioquin
        {
            k++;
        }
    }
    si (n == ZEPHYRUM)
    {
        redde VERUM;   /* linea sine datis */
    }
    runae_rupturam_initiare(&ruptura);
    per (k = I; k < n; k++)
    {
        b32 noster = runae_rumpitur(runae[k - I], runae[k], &ruptura);

        si (noster != limites[k])
        {
            bona = FALSUM;
            si (*fracturae < FRACTURAE_IMPRIMENDAE)
            {
                imprimere("  FRACTA: linea %u, inter U+%04X et U+%04X: "
                    "%s, exspectatum %s\n",
                    (insignatus integer)numerus_lineae,
                    (insignatus integer)runae[k - I],
                    (insignatus integer)runae[k],
                    noster ? "limes" : "nullus",
                    limites[k] ? "limes" : "nullus");
            }
        }
    }
    si (!bona)
    {
        (*fracturae)++;
    }
    redde bona;
}


/* ==================================================
 * II. Latitudo graphematum (Ghostty)
 * ================================================== */

nomen structura {
    constans character* octeti;
                   i32  runae_graphematis;   /* runae in graphemate primo */
                   i32  latitudo;
    constans character* causa;
} CasusGraphematis;

hic_manens constans CasusGraphematis CASUS_GRAPHEMATUM[] = {
    { "\xE2\x9D\xA4\xEF\xB8\x8F", II, II, "cor + VS16" },
    { "#\xEF\xB8\x8F", II, II, "# + VS16" },
    { "x\xEF\xB8\x8F", II, I, "x + VS16 (non basis: nihil)" },
    { "x\xEF\xB8\x8F\xEF\xB8\x8F", III, I, "x + VS16 + VS16" },
    { "#\xEF\xB8\x8E", II, I, "# + VS15" },
    { "\xE2\x8C\x9A\xEF\xB8\x8E", II, I, "horologium + VS15" },
    { "\xE2\x8C\x9A\xEF\xB8\x8E\xEF\xB8\x8F", III, I,
      "horologium + VS15 + VS16 (post VS non basis)" },
    { "\xF0\x9F\x8F\xB4\xE2\x80\x8D\xE2\x98\xA0\xEF\xB8\x8F", IV, II,
      "vexillum piraticum (ZWJ)" },
    { "\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x91\xA9\xE2\x80\x8D"
      "\xF0\x9F\x91\xA7", V, II, "familia (ZWJ)" },
    { "#\xEF\xB8\x8F\xE2\x83\xA3", III, II, "keycap #" },
    { "1\xE2\x83\xA3", II, I, "1 + keycap sine VS16" },
    { "\xF0\x9F\x91\x8B\xF0\x9F\x8F\xBF", II, II,
        "manus + modificator" },
    { "\xE0\xA4\x95\xE0\xA4\xBF", II, II,
        "devanagari ka + i (Mc amplificat)" },
    { "\xF0\x9F\x87\xA6\xF0\x9F\x87\xA7\xF0\x9F\x87\xA8", II, II,
      "RI RI RI: par primum" },
    { "\xF0\x9F\x87\xA8", I, II, "RI solum" },
    { "\xCC\x81\xCC\x82", II, ZEPHYRUM, "signa sola (latitudo 0)" },
    { "ab", I, I, "a b" },
    { "\xFF\xCC\x81", I, I, "octetus invalidus: graphema suum" },
    { "e\xCC\x81x", II, I, "e + acutum" },
    { "\r\n", II, ZEPHYRUM, "CR LF unum graphema" }
};


/* ==================================================
 * IV. Politica SIMPLEX (Terminal.app, mensurata 2026-09-28): ZWJ
 *     pictographa non iungit; Mc amplificat, VS16 manet
 * ================================================== */

hic_manens constans CasusGraphematis CASUS_SIMPLICES[] = {
    { "\xE0\xA4\x95\xE0\xA4\xBF", II, II, "ka + i (Mc amplificat)" },
    { "\xF0\x9F\x91\xA8\xE2\x80\x8D\xF0\x9F\x91\xA9\xE2\x80\x8D"
      "\xF0\x9F\x91\xA7", II, II,
          "familia: vir + ZWJ solum (GB11 non)" },
    { "\xE2\x9D\xA4\xEF\xB8\x8F", II, II, "cor + VS16 (manet II)" },
    { "\xF0\x9F\x91\x8B\xF0\x9F\x8F\xBF", II, II,
        "manus + modificator" },
    { "\xF0\x9F\x87\xA6\xF0\x9F\x87\xA7", II, II, "vexillum (par RI)" },
    { "e\xCC\x81", II, I, "e + acutum" },
    { "\xE4\xB8\xAD", I, II, "zhong solum" }
};


/* ==================================================
 * V. Textus pingendus (U6b): runae_latitudo_textus et
 *    runae_columnam_quaerere - UNITAS PINGENDA (runae.h)
 * ================================================== */

#define FAMILIA "\xF0\x9F\x91\xA8\xE2\x80\x8D" \
                "\xF0\x9F\x91\xA9\xE2\x80\x8D" \
                "\xF0\x9F\x91\xA7"

nomen structura {
    constans character* octeti;
         RunaePolitica  politica;
                   i32  latitudo;
    constans character* causa;
} CasusTextus;

hic_manens constans CasusTextus CASUS_TEXTUS[] = {
    { "", RUNAE_POLITICA_GRAPHEMATUM, ZEPHYRUM, "vacuum" },
    { "abc", RUNAE_POLITICA_GRAPHEMATUM, III, "ASCII" },
    { "a\xE5\xBA\x83" "b", RUNAE_POLITICA_GRAPHEMATUM, IV,
        "a guang b" },
    { "e\xCC\x81" "x", RUNAE_POLITICA_GRAPHEMATUM, II,
        "e + acutum, x" },
    { "a\x01" "b", RUNAE_POLITICA_GRAPHEMATUM, III, "regimen C0 = I" },
    { "\t", RUNAE_POLITICA_GRAPHEMATUM, I, "tabula (C0) = I" },
    { "\r\n", RUNAE_POLITICA_GRAPHEMATUM, II,
      "CR LF: octeti bini, non graphema unum" },
    { "\x7F", RUNAE_POLITICA_GRAPHEMATUM, I, "DEL = I" },
    { "\xC2\x80", RUNAE_POLITICA_GRAPHEMATUM, ZEPHYRUM,
      "regimen C1 = 0 (tessera nihil pingit)" },
    { "\xCC\x81", RUNAE_POLITICA_GRAPHEMATUM, ZEPHYRUM,
        "signum solum" },
    { "\xE0\x80\x80", RUNAE_POLITICA_GRAPHEMATUM, III,
      "series nimis longa: I per octetum (tessera '?' per octetum)" },
    { "a\xFF", RUNAE_POLITICA_GRAPHEMATUM, II, "octetus invalidus" },
    { FAMILIA, RUNAE_POLITICA_GRAPHEMATUM, II, "familia GRAPHEMATUM" },
    { FAMILIA, RUNAE_POLITICA_SIMPLEX, VI, "familia SIMPLEX" }
};

nomen structura {
    constans character* octeti;
         RunaePolitica  politica;
                   i32  columna;
                   i32  offset;   /* initium unitatis tegentis */
                   i32  columna_initii;
    constans character* causa;
} CasusColumnae;

hic_manens constans CasusColumnae CASUS_COLUMNARUM[] = {
    { "a\xE5\xBA\x83" "b", RUNAE_POLITICA_GRAPHEMATUM, ZEPHYRUM,
        ZEPHYRUM,
      ZEPHYRUM, "a" },
    { "a\xE5\xBA\x83" "b", RUNAE_POLITICA_GRAPHEMATUM, I, I, I,
      "guang, dimidium primum" },
    { "a\xE5\xBA\x83" "b", RUNAE_POLITICA_GRAPHEMATUM, II, I, I,
      "guang, dimidium alterum -> initium" },
    { "a\xE5\xBA\x83" "b", RUNAE_POLITICA_GRAPHEMATUM, III, IV, III,
        "b" },
    { "a\xE5\xBA\x83" "b", RUNAE_POLITICA_GRAPHEMATUM, IV, V, IV,
      "finis textus" },
    { "a\xE5\xBA\x83" "b", RUNAE_POLITICA_GRAPHEMATUM, XCIX, V, IV,
      "ultra finem -> finis" },
    { "\xCC\x81" "a", RUNAE_POLITICA_GRAPHEMATUM, ZEPHYRUM, II,
        ZEPHYRUM,
      "signum solum nihil tegit: a" },
    { "a\x01" "b", RUNAE_POLITICA_GRAPHEMATUM, II, II, II,
      "b post regimen" },
    { "x" FAMILIA "y", RUNAE_POLITICA_GRAPHEMATUM, II, I, I,
      "familia GRAPHEMATUM: dimidium alterum" },
    { "x" FAMILIA "y", RUNAE_POLITICA_GRAPHEMATUM, III, XIX, III,
      "y post familiam GRAPHEMATUM" },
    { "x" FAMILIA "y", RUNAE_POLITICA_SIMPLEX, III, VIII, III,
      "SIMPLEX: femina (graphema alterum)" },
    { "x" FAMILIA "y", RUNAE_POLITICA_SIMPLEX, VII, XIX, VII,
      "y post familiam SIMPLEX" }
};

/* Ambulator INDEPENDENS unitatum pingendarum (regula runae.h hic
 * iterum dicta, non vocata): reddit finem unitatis, latitudo in *lat */
interior constans i8*
_unitas_ambulatoris (
      constans i8* cursor,
      constans i8* finis,
    RunaePolitica  politica,
              i32* lat)
{
    constans i8* post = cursor;

    si (*cursor < 0x20 || *cursor == 0x7F)
    {
        *lat = I;
        redde cursor + I;
    }
    si (utf8_decodere(&post, finis) < ZEPHYRUM)
    {
        *lat = I;
        redde cursor + I;
    }
    redde runae_graphema_ex_politica(cursor, finis, politica, lat);
}

/* Proprietas per corpus: columna initii cuiusque unitatis visibilis
 * (quaeque XVI-a, ne quadratum tempus) ad idem initium redit; latitudo
 * tota = summa. Reddit numerum fracturarum. */
interior i32
_columnas_corporis (
                chorda  textus,
         RunaePolitica  politica,
    constans character* lingua)
{
    constans i8* initium    = textus.datum;
    constans i8* finis      = textus.datum + textus.mensura;
    constans i8* cursor     = initium;
            i32  columna    = ZEPHYRUM;
            i32  unitas     = ZEPHYRUM;
            i32  fracturae  = ZEPHYRUM;

    dum (cursor < finis)
    {
                i32  lat;
        constans i8* post = _unitas_ambulatoris(cursor, finis, politica,
            &lat);

        si (lat > ZEPHYRUM && (unitas % XVI) == ZEPHYRUM)
        {
                    i32  col_inventa;
            constans i8* inventum = runae_columnam_quaerere(initium,
                finis,
                politica, columna + lat - I, &col_inventa);

            si (inventum != cursor || col_inventa != columna)
            {
                si (fracturae < III)
                {
                    imprimere("  FRACTA: %s: columna %u -> offset %u "
                        "(exspectatum %u), columna initii %u\n", lingua,
                        (insignatus integer)(columna + lat - I),
                        (insignatus integer)(inventum - initium),
                        (insignatus integer)(cursor - initium),
                        (insignatus integer)col_inventa);
                }
                fracturae++;
            }
        }
        columna += lat;
        unitas++;
        cursor = post;
    }
    si (runae_latitudo_textus(initium, finis, politica) != columna)
    {
        imprimere("  FRACTA: %s: latitudo textus %u, summa %u\n",
            lingua,
            (insignatus integer)runae_latitudo_textus(initium, finis,
                politica),
            (insignatus integer)columna);
        fracturae++;
    }
    redde fracturae;
}


/* ==================================================
 * III. Aurum graphematum
 * ================================================== */

interior i32
_friare (
    i32 friatio,
    i32 valor)
{
    i32 k;

    per (k = ZEPHYRUM; k < IV; k++)
    {
        friatio ^= (valor >> (k * VIII)) & 0xFF;
        friatio *= 16777619;
    }
    redde friatio;
}

/* Limites corporis per runae_rumpitur: numerus graphematum et friatio
 * FNV-1a offsetorum limitum (0 et finis inclusi) */
interior vacuum
_limites_corporis (
    chorda  textus,
       i32* numerus,
       i32* friatio)
{
     constans i8* cursor  = textus.datum;
     constans i8* finis   = textus.datum + textus.mensura;
             s32  prior   = -I;
    RunaeRuptura  ruptura;

    *numerus = ZEPHYRUM;
    *friatio = 2166136261u;
    runae_rupturam_initiare(&ruptura);
    dum (cursor < finis)
    {
        i32 offset  = (i32)(cursor - textus.datum);
        s32 runa    = utf8_decodere(&cursor, finis);

        si (prior == -I || runae_rumpitur(prior, runa, &ruptura))
        {
            *friatio = _friare(*friatio, offset);
            (*numerus)++;
        }
        prior = runa;
    }
    *friatio = _friare(*friatio, textus.mensura);
}


s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;

    piscina = piscina_generare_dynamicum("probatio_runae_graphemata",
        4194304);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    /* I. GraphemeBreakTest */
    {
        chorda textus   = filum_legere_totum(PROBATIO_VIA, piscina);
           i32 initium  = ZEPHYRUM;
           i32 k;
           i32 lineae     = ZEPHYRUM;
           i32 probatae   = ZEPHYRUM;
           i32 fracturae  = ZEPHYRUM;

        imprimere("\n--- GraphemeBreakTest (Unicode %s) ---\n",
            RUNAE_VERSIO);
        CREDO_VERUM (textus.mensura > ZEPHYRUM);
        per (k = ZEPHYRUM; k <= textus.mensura; k++)
        {
            si (k == textus.mensura || textus.datum[k] == '\n')
            {
                lineae++;
                si (k > initium && textus.datum[initium] != '#')
                {
                    probatae++;
                    (vacuum)_lineam_probare(textus.datum + initium,
                        k - initium, lineae, &fracturae);
                }
                initium = k + I;
            }
        }
        imprimere("  lineae probatae: %u, fractae: %u\n",
            (insignatus integer)probatae,
            (insignatus integer)fracturae);
        CREDO_VERUM (probatae > M);
        CREDO_AEQUALIS_I32 (fracturae, ZEPHYRUM);
    }

    /* II. Latitudo graphematum */
    {
        i32 j;

        imprimere("\n--- Latitudo graphematum (Ghostty) ---\n");
        per (j = ZEPHYRUM;
             j < (i32)(magnitudo(CASUS_GRAPHEMATUM)
                       / magnitudo(CASUS_GRAPHEMATUM[0]));
             j++)
        {
            constans CasusGraphematis* c      = &CASUS_GRAPHEMATUM[j];
                        constans i8* initium  = (constans i8*)c->octeti;
                        constans i8* finis = initium
                            + strlen(c->octeti);
                         constans i8* post;
                         constans i8* cursor;
                                 i32  latitudo;
                                 i32  runae = ZEPHYRUM;

            post = runae_graphema_proximum(initium, finis,
                &latitudo);
            cursor = initium;
            dum (cursor < post)
            {
                (vacuum)utf8_decodere(&cursor, post);
                runae++;
            }
            si (   runae    != c->runae_graphematis
                || latitudo != c->latitudo)
            {
                imprimere("  FRACTA: %s: runae %u latitudo %u, exspectatae "
                    "%u et %u\n", c->causa, (insignatus integer)runae,
                    (insignatus integer)latitudo,
                    (insignatus integer)c->runae_graphematis,
                    (insignatus integer)c->latitudo);
            }
            CREDO_AEQUALIS_I32 (runae, c->runae_graphematis);
            CREDO_AEQUALIS_I32 (latitudo, c->latitudo);
        }
        {
             hic_manens constans i8 vacuum_textus[I]  = { 'x' };
                                i32 latitudo          = VII;

            /* initium == finis: nihil consumitur, latitudo 0 */
            CREDO_VERUM (runae_graphema_proximum(vacuum_textus,
                vacuum_textus, &latitudo) == vacuum_textus);
            CREDO_AEQUALIS_I32 (latitudo, ZEPHYRUM);
        }
    }

    /* IV. Politica SIMPLEX */
    {
        i32 j;

        imprimere("\n--- Politica SIMPLEX ---\n");
        per (j = ZEPHYRUM;
             j < (i32)(magnitudo(CASUS_SIMPLICES)
                       / magnitudo(CASUS_SIMPLICES[0]));
             j++)
        {
            constans CasusGraphematis* c      = &CASUS_SIMPLICES[j];
                        constans i8* initium  = (constans i8*)c->octeti;
                        constans i8* finis = initium
                            + strlen(c->octeti);
                         constans i8* post;
                         constans i8* cursor;
                                 i32  latitudo;
                                 i32  runae = ZEPHYRUM;

            post = runae_graphema_ex_politica(initium, finis,
                RUNAE_POLITICA_SIMPLEX, &latitudo);
            cursor = initium;
            dum (cursor < post)
            {
                (vacuum)utf8_decodere(&cursor, post);
                runae++;
            }
            si (   runae    != c->runae_graphematis
                || latitudo != c->latitudo)
            {
                imprimere("  FRACTA: SIMPLEX %s: runae %u latitudo %u, "
                    "exspectatae %u et %u\n", c->causa,
                    (insignatus integer)runae,
                    (insignatus integer)latitudo,
                    (insignatus integer)c->runae_graphematis,
                    (insignatus integer)c->latitudo);
            }
            CREDO_AEQUALIS_I32 (runae, c->runae_graphematis);
            CREDO_AEQUALIS_I32 (latitudo, c->latitudo);
        }
    }

    /* V. Textus pingendus: casus */
    {
        i32 j;

        imprimere("\n--- Textus pingendus: latitudo et columnae ---\n");
        per (j = ZEPHYRUM;
             j < (i32)(magnitudo(CASUS_TEXTUS)
                       / magnitudo(CASUS_TEXTUS[0]));
             j++)
        {
            constans CasusTextus* c        = &CASUS_TEXTUS[j];
                     constans i8* initium  = (constans i8*)c->octeti;
                             i32  lat      =
                                 runae_latitudo_textus(initium,
                                 initium + strlen(c->octeti),
                                 c->politica);

            si (lat != c->latitudo)
            {
                imprimere("  FRACTA: %s: latitudo %u, exspectata %u\n",
                    c->causa, (insignatus integer)lat,
                    (insignatus integer)c->latitudo);
            }
            CREDO_AEQUALIS_I32 (lat, c->latitudo);
        }
        per (j = ZEPHYRUM;
             j < (i32)(magnitudo(CASUS_COLUMNARUM)
                       / magnitudo(CASUS_COLUMNARUM[0]));
             j++)
        {
            constans CasusColumnae* c        = &CASUS_COLUMNARUM[j];
                       constans i8* initium  = (constans i8*)c->octeti;
                               i32  col      = XCIX;
                       constans i8* inventum = runae_columnam_quaerere(
                           initium, initium + strlen(c->octeti),
                           c->politica, c->columna, &col);

            si (   (i32)(inventum - initium) != c->offset
                || col                       != c->columna_initii)
            {
                imprimere("  FRACTA: %s: offset %u columna %u, "
                    "exspectata %u et %u\n", c->causa,
                    (insignatus integer)(inventum - initium),
                    (insignatus integer)col,
                    (insignatus integer)c->offset,
                    (insignatus integer)c->columna_initii);
            }
            CREDO_AEQUALIS_I32 ((i32)(inventum - initium), c->offset);
            CREDO_AEQUALIS_I32 (col, c->columna_initii);
        }
    }

    /* III. Aurum graphematum (oraculum ICU) + VI. columnae corporis */
    {
        chorda aurum    = filum_legere_totum(AURUM_VIA, piscina);
           i32 k        = ZEPHYRUM;
           i32 linguae  = ZEPHYRUM;

        imprimere("\n--- Aurum graphematum (ICU) ---\n");
        dum (k < aurum.mensura)
        {
            character lingua[VIII];
            character via[CXXVIII];
                  i32 j              = ZEPHYRUM;
                  i32 numerus_aurum  = ZEPHYRUM;
                  i32 friatio_aurum  = ZEPHYRUM;
                  i32 numerus;
                  i32 friatio;
               chorda textus;

            si (aurum.datum[k] == '#')
            {
                dum (k < aurum.mensura && aurum.datum[k] != '\n')
                {
                    k++;
                }
                k++;
                perge;
            }
            dum (k < aurum.mensura && aurum.datum[k] != ' ' && j < VII)
            {
                lingua[j++] = (character)aurum.datum[k++];
            }
            lingua[j] = '\0';
            k++;
            dum (   k < aurum.mensura && aurum.datum[k] >= '0'
                 && aurum.datum[k] <= '9')
            {
                numerus_aurum = numerus_aurum * X
                    + (i32)(aurum.datum[k++] - '0');
            }
            k++;
            dum (k < aurum.mensura && aurum.datum[k] != '\n')
            {
                i8 c = aurum.datum[k++];

                friatio_aurum = friatio_aurum * XVI
                    + (i32)((c <= '9') ? (c - '0') : (c - 'a') + X);
            }
            k++;
            sprintf(via, "probationes/fixa/runae/corpus/%s.txt",
                lingua);
            textus = filum_legere_totum(via, piscina);
            _limites_corporis(textus, &numerus, &friatio);
            /* VI. columnae per corpus, politica utraque */
            CREDO_AEQUALIS_I32 (_columnas_corporis(textus,
                RUNAE_POLITICA_GRAPHEMATUM, lingua), ZEPHYRUM);
            CREDO_AEQUALIS_I32 (_columnas_corporis(textus,
                RUNAE_POLITICA_SIMPLEX, lingua), ZEPHYRUM);
            si (numerus != numerus_aurum || friatio != friatio_aurum)
            {
                imprimere("  FRACTA: %s: graphemata %u (aurum %u), friatio "
                    "%08x (aurum %08x)\n", lingua,
                    (insignatus integer)numerus,
                    (insignatus integer)numerus_aurum,
                    (insignatus integer)friatio,
                    (insignatus integer)friatio_aurum);
            }
            CREDO_AEQUALIS_I32 (numerus, numerus_aurum);
            CREDO_AEQUALIS_I32 (friatio, friatio_aurum);
            linguae++;
        }
        imprimere("  linguae: %u\n", (insignatus integer)linguae);
        CREDO_AEQUALIS_I32 (linguae, XXXV);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
