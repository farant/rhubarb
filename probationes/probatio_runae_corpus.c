/* probatio_runae_corpus.c - Corpus Lapidis (runae U1)
 *
 * Specimina textus ex Lapide (tools/runae_corpus.c, parsator html
 * domus): lingua quaeque in probationes/fixa/runae/corpus/<lingua>.txt.
 * Pro quaque lingua: filum adest, >= IV KiB, UTF-8 validum ab initio ad
 * finem, et SCRIPTURA recta - saltem C runae in scriptura principali
 * linguae (Devanagari pro hi, Thai pro th, ...; litterae Latinae pro
 * linguis scripturae Latinae). Custodia scripturae extractorem
 * deprehendit qui fila confundit aut textum perdit.
 *
 * NB linguae eiusdem scripturae (en/la/es/..., ar/fa) inter se non
 * distinguuntur - custodia scripturam probat, non linguam.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "utf8.h"
#include "credo.h"
#include <stdio.h>

#define CORPUS_VIA "probationes/fixa/runae/corpus/"
#define MENSURA_MINIMA (IV * MXXIV)
#define RUNAE_SCRIPTURAE_MINIMAE C

/* Lingua et scriptura eius principalis (intervallum codepointorum) */
nomen structura {
    constans character* lingua;
                   s32  prima;
                   s32  ultima;
} LinguaScriptura;

#define LATINA 0x61, 0x7A   /* litterae minusculae a-z */

hic_manens constans LinguaScriptura LINGUAE[] = {
    { "en", LATINA }, { "la", LATINA }, { "es", LATINA },
    { "fr", LATINA }, { "it", LATINA }, { "pt", LATINA },
    { "de", LATINA }, { "pl", LATINA }, { "id", LATINA },
    { "ceb", LATINA }, { "hu", LATINA }, { "ig", LATINA },
    { "nl", LATINA }, { "ro", LATINA }, { "rw", LATINA },
    { "sv", LATINA }, { "sw", LATINA }, { "tl", LATINA },
    { "tr", LATINA }, { "vi", LATINA }, { "yo", LATINA },
    { "el", 0x0370, 0x03FF },   /* Graeca */
    { "ru", 0x0400, 0x04FF },   /* Cyrillica */
    { "he", 0x0590, 0x05FF },   /* Hebraica */
    { "ar", 0x0600, 0x06FF },   /* Arabica */
    { "fa", 0x0600, 0x06FF },   /* Arabica (Persica) */
    { "hi", 0x0900, 0x097F },   /* Devanagari */
    { "bn", 0x0980, 0x09FF },   /* Bengalica */
    { "gu", 0x0A80, 0x0AFF },   /* Gujarati */
    { "ta", 0x0B80, 0x0BFF },   /* Tamulica */
    { "ml", 0x0D00, 0x0D7F },   /* Malayalam */
    { "th", 0x0E00, 0x0E7F },   /* Thai */
    { "ja", 0x3040, 0x30FF },   /* kana (distinguit a zh) */
    { "zh", 0x4E00, 0x9FFF },   /* CJK unificata */
    { "ko", 0xAC00, 0xD7A3 }    /* syllabae Hangul */
};

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
         i32  j;

    piscina = piscina_generare_dynamicum("probatio_runae_corpus",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Corpus Lapidis: %u linguae ---\n",
        (insignatus integer)(magnitudo(LINGUAE)
            / magnitudo(LINGUAE[0])));
    CREDO_AEQUALIS_I32 ((i32)(magnitudo(LINGUAE)
        / magnitudo(LINGUAE[0])),
        XXXV);

    per (j = ZEPHYRUM; j < (i32)(magnitudo(LINGUAE)
        / magnitudo(LINGUAE[0]));
         j++)
    {
        constans LinguaScriptura* l = &LINGUAE[j];
                       character  via[CXXVIII];
                          chorda  textus;
                     constans i8* cursor;
                     constans i8* finis;
                             i32  runae         = ZEPHYRUM;
                             i32  in_scriptura  = ZEPHYRUM;
                             b32  validum       = VERUM;

        sprintf(via, "%s%s.txt", CORPUS_VIA, l->lingua);
        textus = filum_legere_totum(via, piscina);
        si (textus.mensura < MENSURA_MINIMA)
        {
            imprimere("  FRACTA: %s - %u octeti (minimum %u)\n",
                l->lingua,
                (insignatus integer)textus.mensura,
                (insignatus integer)MENSURA_MINIMA);
        }
        CREDO_VERUM (textus.mensura >= MENSURA_MINIMA);
        si (textus.mensura == ZEPHYRUM)
        {
            perge;
        }

        cursor  = textus.datum;
        finis   = textus.datum + textus.mensura;
        dum (cursor < finis)
        {
            s32 runa = utf8_decodere(&cursor, finis);

            si (runa < ZEPHYRUM)
            {
                validum = FALSUM;
                frange;
            }
            runae++;
            si (runa >= l->prima && runa <= l->ultima)
            {
                in_scriptura++;
            }
        }
        si (!validum || in_scriptura < RUNAE_SCRIPTURAE_MINIMAE)
        {
            imprimere("  FRACTA: %s - validum %d, runae %u, in scriptura "
                "%u (U+%04X-U+%04X)\n", l->lingua, (integer)validum,
                (insignatus integer)runae,
                (insignatus integer)in_scriptura,
                (insignatus integer)l->prima,
                (insignatus integer)l->ultima);
        }
        CREDO_VERUM (validum);
        CREDO_VERUM (in_scriptura >= RUNAE_SCRIPTURAE_MINIMAE);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
