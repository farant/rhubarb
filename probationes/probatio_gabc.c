/* probatio_gabc.c - lector gabc (include/gabc.h)
 *
 * Fixa in probationes/fixa/gabc/ (scripta manu, una res quaeque;
 * linea I commentarium '%' dicit quid figat). Plan:
 * project-specs/gabc-plan.md T2 (nucleus): caput, syllabae, textus
 * simplex, notae, neumae, claves, divisiones, tabula canonica.
 *
 * GRAMMATICA TABULAE (gabc_tabulam_scribere), linea una per rem,
 * columnae tabulis separatae; textus effugitur (\t \n \r \\):
 *   caput    clavis  valor
 *   syllaba  index  verbum_incipit(0|1)  textus
 *   clavis   c|f  linea  bemolle(0|1)
 *   neuma    prima  numerus  figura|-
 *   nota     positio  gradus  alteratio  formae|-  mora  episema
 *            ictus  linea:columna
 *   divisio  genus  numerus  alta(0|1)  parenthesis(0|1)
 *   alteratio genus  positio  molle(0|1)  parenthesis(0|1)
 *   spatium  genus  factor|-
 *   fractio  iustificata(0|1)  custos(-1|0|1)
 *   custos   positio|-
 *   signum   textus  nota
 * formae: littera una per bit, ordine bitorum:
 *   I v V s o O w W r R = q ~ < > - @
 */

#include "latina.h"
#include "credo.h"
#include "gabc.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "piscina.h"
#include <string.h>

interior GabcCantus*
_legere_fixum (
         CredoContextus* c,
     constans character* titulus)
{
    character via[CCLVI];

    strcpy(via, "probationes/fixa/gabc/");
    strcat(via, titulus);
    redde gabc_legere(filum_legere_totum(via, c->piscina), c->piscina);
}

interior chorda
_tabula (
         CredoContextus* c,
    constans GabcCantus* cantus)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, MMM);

    si (!gabc_tabulam_scribere(cantus, a))
    {
        redde chorda_ex_literis("(tabula fracta)", c->piscina);
    }
    redde chorda_aedificator_finire(a);
}

interior b32
_aequat (
                 chorda  ch,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde ch.mensura == n && (n == ZEPHYRUM
        || memcmp(ch.datum, literae, (size_t)n) == ZEPHYRUM);
}

/* textus cantatus syllabae (cursus omnes coniuncti) aequat? */
interior b32
_textus_aequat (
    constans GabcSyllaba* s,
      constans character* literae)
{
    character receptaculum[CCLVI];
          i32 n = ZEPHYRUM;
          i32 r;

    per (r = ZEPHYRUM; r < s->numerus_textuum; r++)
    {
        si (   s->textus[r].genus == GABC_TEXTUS_CANTATUS
            && n + (i32)s->textus[r].textus.mensura < CCLVI)
        {
            memcpy(receptaculum + n, s->textus[r].textus.datum,
                (size_t)s->textus[r].textus.mensura);
            n += (i32)s->textus[r].textus.mensura;
        }
    }
    redde n == (i32)strlen(literae)
        && memcmp(receptaculum, literae, (size_t)n) == ZEPHYRUM;
}

interior vacuum
_caput_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "caput.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_VERUM (gabc_successus(g));
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (g->numerus_capitum, (i32)VI);
    CREDO_VERUM (_aequat(g->titulus, "Salve"));
    CREDO_VERUM (_aequat(g->modus, "5"));
    CREDO_AEQUALIS_S32 (g->modus_numerus, (s32)V);
    CREDO_AEQUALIS_I32 (g->numerus_annotationum, (i32)II);
    CREDO_VERUM (_aequat(g->annotationes[ZEPHYRUM], "Ant."));
    CREDO_VERUM (_aequat(g->annotationes[I], "V"));
    CREDO_AEQUALIS_I32 (g->lineae, (i32)IV);
    CREDO_AEQUALIS_I32 (g->capita[ZEPHYRUM].linea, (i32)II);
    CREDO_AEQUALIS_I32 (g->capita[V].linea, (i32)VIII);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tSalve\n"
        "caput\tmode\t5\n"
        "caput\tannotation\tAnt.\n"
        "caput\tannotation\tV\n"
        "caput\tfoo\tbar\n"
        "caput\tcommentary\tunus\\nduo\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t11:8\n");
}

interior vacuum
_syllabas_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "syllabae.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_syllabarum == VI);
    /* syllaba 0: clavis sola, sine textu */
    CREDO_VERUM (_textus_aequat(&g->syllabae[ZEPHYRUM], ""));
    CREDO_FALSUM (g->syllabae[ZEPHYRUM].verbum_incipit);
    CREDO_VERUM (_textus_aequat(&g->syllabae[I], "Sal"));
    CREDO_VERUM (_textus_aequat(&g->syllabae[II], "ve"));
    CREDO_VERUM (_textus_aequat(&g->syllabae[III], "Re"));
    CREDO_VERUM (_textus_aequat(&g->syllabae[IV], "gi"));
    CREDO_VERUM (_textus_aequat(&g->syllabae[V], "na"));
    CREDO_VERUM (g->syllabae[I].verbum_incipit);
    CREDO_FALSUM (g->syllabae[II].verbum_incipit);
    CREDO_VERUM (g->syllabae[III].verbum_incipit);
    CREDO_FALSUM (g->syllabae[IV].verbum_incipit);
    CREDO_FALSUM (g->syllabae[V].verbum_incipit);
    CREDO_AEQUALIS_I32 (g->syllabae[III].linea, (i32)IV);
    CREDO_AEQUALIS_I32 (g->syllabae[III].columna, (i32)XVIII);
    CREDO_AEQUALIS_S32 (g->syllabae[III].vocalis_initium, (s32)-I);
}

interior vacuum
_neumas_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "neumae.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (g->numerus_notarum, (i32)IX);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tneumae\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t2\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:8\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:9\n"
        "syllaba\t2\t1\tb\n"
        "neuma\t2\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:14\n"
        "neuma\t3\t1\t-\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:16\n"
        "syllaba\t3\t1\tc\n"
        "neuma\t4\t2\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:21\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:22\n"
        "neuma\t6\t2\t-\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:24\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:25\n"
        "syllaba\t4\t1\td\n"
        "neuma\t8\t1\t-\n"
        "nota\tj\t0\t0\t-\t0\t0\t0\t4:30\n");
}

interior vacuum
_claves_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "claves.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    /* c4: do in j; f3: fa in h, do in e; c1: do in d */
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tclaves\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\t-\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:8\n"
        "syllaba\t2\t0\t\n"
        "clavis\tf\t3\t0\n"
        "syllaba\t3\t1\tb\n"
        "neuma\t1\t1\t-\n"
        "nota\th\t3\t0\t-\t0\t0\t0\t4:18\n"
        "syllaba\t4\t0\t\n"
        "clavis\tc\t1\t0\n"
        "syllaba\t5\t1\tc\n"
        "neuma\t2\t1\t-\n"
        "nota\td\t0\t0\t-\t0\t0\t0\t4:28\n");
}

interior vacuum
_divisiones_probare (
    CredoContextus* c)
{
           GabcCantus* g = _legere_fixum(c, "divisiones.gabc");
    ChordaAedificator* a;

    CREDO_NECESSE (g != NIHIL);
    CREDO_VERUM (gabc_successus(g));
    CREDO_NECESSE (g->numerus_vitiorum == I);
    CREDO_AEQUALIS_S32 ((s32)g->vitia[ZEPHYRUM].gravitas,
        (s32)GABC_MONITUM);
    CREDO_AEQUALIS_S32 ((s32)g->vitia[ZEPHYRUM].genus,
        (s32)GABC_DIVISIO_IN_NEUMA);
    a = chorda_aedificator_creare(c->piscina, CCLVI);
    CREDO_VERUM (gabc_vitia_scribere(g, "divisiones.gabc", a));
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_aedificator_finire(a),
        "divisiones.gabc:4:45: monitum: divisio intra notas"
        " coniunctas\n");
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tdivisiones\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:8\n"
        "syllaba\t2\t0\t\n"
        "divisio\tvirgula\t0\t0\t0\n"
        "syllaba\t3\t1\tb\n"
        "neuma\t1\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:17\n"
        "syllaba\t4\t0\t\n"
        "divisio\tminima\t0\t0\t0\n"
        "syllaba\t5\t1\tc\n"
        "neuma\t2\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:26\n"
        "syllaba\t6\t0\t\n"
        "divisio\tminor\t0\t0\t0\n"
        "syllaba\t7\t1\td\n"
        "neuma\t3\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:35\n"
        "syllaba\t8\t0\t\n"
        "divisio\tmaior\t0\t0\t0\n"
        "syllaba\t9\t1\te\n"
        "neuma\t4\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:44\n"
        "divisio\tminor\t0\t0\t0\n"
        "neuma\t5\t1\t-\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:46\n"
        "syllaba\t10\t0\t\n"
        "divisio\tfinalis\t0\t0\t0\n"
        "syllaba\t11\t0\t\n"
        "divisio\tminimis\t0\t0\t0\n"
        "syllaba\t12\t0\t\n"
        "divisio\tmaior_punctata\t0\t0\t0\n"
        "syllaba\t13\t0\t\n"
        "divisio\tminima\t0\t1\t0\n"
        "syllaba\t14\t0\t\n"
        "divisio\tvirgula\t0\t0\t1\n"
        "syllaba\t15\t0\t\n"
        "divisio\tdominicana\t3\t0\t0\n");
}

/* Review Focus 3: textus sine notis et notae sine textu locum servant */
interior vacuum
_textum_sine_notis_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "textus_sine_notis.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_syllabarum == VI);
    CREDO_AEQUALIS_I32 (g->syllabae[III].numerus_elementorum, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\ttextus\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\tve\n"
        "neuma\t0\t2\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:9\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:10\n"
        "syllaba\t2\t0\t\n"
        "neuma\t2\t2\t-\n"
        "nota\th\t-2\t0\t-\t0\t0\t0\t4:14\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:15\n"
        "syllaba\t3\t1\t*\n"
        "syllaba\t4\t1\tre\n"
        "neuma\t4\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:25\n"
        "syllaba\t5\t0\t\n"
        "divisio\tfinalis\t0\t0\t0\n");
}

/* fons verus: syllabae CXLI et notae CXLV (numeratae extra lectorem,
 * per instrumenta crustae, T2 gradus I) */
interior vacuum
_salve_legere_probare (
    CredoContextus* c)
{
    GabcCantus* g = gabc_legere(filum_legere_totum(
        "musica/fontes/salve_2435.gabc", c->piscina), c->piscina);

    CREDO_NECESSE (g != NIHIL);
    CREDO_VERUM (gabc_successus(g));
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (g->numerus_capitum, (i32)V);
    CREDO_VERUM (_aequat(g->titulus, "Salve Regina (simple tone)"));
    CREDO_AEQUALIS_S32 (g->modus_numerus, (s32)V);
    CREDO_AEQUALIS_I32 (g->numerus_syllabarum, (i32)CXLI);
    CREDO_AEQUALIS_I32 (g->numerus_notarum, (i32)CXLV);
    CREDO_VERUM (_textus_aequat(&g->syllabae[I], "SAl"));
    CREDO_VERUM (g->syllabae[I].verbum_incipit);
}

interior constans CredoSectio SECTIONES[] = {
    { "caput: paria, annotationes, valor per lineas", _caput_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "syllabae: verbum_incipit", _syllabas_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "neumae: coniunctio et gradus sub c4", _neumas_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves: mutatio sequentes solas resolvit", _claves_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "divisiones: genera et monitum intra neumam",
        _divisiones_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus sine notis, notae sine textu",
        _textum_sine_notis_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "salve_2435.gabc: sine vitio", _salve_legere_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("gabc", SECTIONES, NIHIL, NIHIL);
}
