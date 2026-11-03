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
           i32 k;
           i32 ictus;
           i32 mora;
           i32 episema;
           i32 quilisma;

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
    /* T3: signa numerata per crustam (' 39, . 14, _ 8, w 2) */
    ictus = mora = episema = quilisma = ZEPHYRUM;
    per (k = ZEPHYRUM; k < g->numerus_notarum; k++)
    {
        ictus    += g->notae[k].ictus != ZEPHYRUM ? I : ZEPHYRUM;
        mora     += (i32)g->notae[k].mora;
        episema  += g->notae[k].episema != ZEPHYRUM ? I : ZEPHYRUM;
        quilisma += (g->notae[k].formae & GABC_FORMA_QUILISMA)
            != ZEPHYRUM
            ? I : ZEPHYRUM;
    }
    CREDO_AEQUALIS_I32 (ictus, (i32)XXXIX);
    CREDO_AEQUALIS_I32 (mora, (i32)XIV);
    CREDO_AEQUALIS_I32 (episema, (i32)VIII);
    CREDO_AEQUALIS_I32 (quilisma, (i32)II);
}

/* T3: formae - littera una per bit (ordo in capite plagulae); vv sss
 * notas iterant; directio et signa r1-r5 in structura */
interior vacuum
_formas_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "formae.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_notarum == XXX);
    CREDO_AEQUALIS_S32 ((s32)g->notae[XVII].directio, (s32)-I);
    CREDO_AEQUALIS_S32 ((s32)g->notae[XVIII].directio, (s32)I);
    CREDO_AEQUALIS_S32 ((s32)g->notae[XIX].directio, (s32)I);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XXV].signa,
        (i32)GABC_SIGNUM_ACCENTUS);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XXVI].signa,
        (i32)GABC_SIGNUM_ACCENTUS_REVERSUS);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XXVII].signa,
        (i32)GABC_SIGNUM_CIRCULUS);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XXVIII].signa,
        (i32)GABC_SIGNUM_SEMICIRCULUS);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XXIX].signa,
        (i32)GABC_SIGNUM_SEMICIRCULUS_REVERSUS);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tformae\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\t-\n"
        "nota\tg\t-3\t0\tv\t0\t0\t0\t4:8\n"
        "neuma\t1\t1\t-\n"
        "nota\tg\t-3\t0\tV\t0\t0\t0\t4:11\n"
        "neuma\t2\t1\t-\n"
        "nota\tg\t-3\t0\ts\t0\t0\t0\t4:14\n"
        "neuma\t3\t1\t-\n"
        "nota\tg\t-3\t0\to\t0\t0\t0\t4:17\n"
        "neuma\t4\t1\t-\n"
        "nota\tg\t-3\t0\tO\t0\t0\t0\t4:20\n"
        "neuma\t5\t1\t-\n"
        "nota\tg\t-3\t0\tw\t0\t0\t0\t4:23\n"
        "neuma\t6\t1\t-\n"
        "nota\tg\t-3\t0\tW\t0\t0\t0\t4:26\n"
        "neuma\t7\t1\t-\n"
        "nota\tg\t-3\t0\tr\t0\t0\t0\t4:29\n"
        "neuma\t8\t1\t-\n"
        "nota\tg\t-3\t0\tR\t0\t0\t0\t4:32\n"
        "neuma\t9\t1\t-\n"
        "nota\tg\t-3\t0\trR\t0\t0\t0\t4:35\n"
        "neuma\t10\t1\t-\n"
        "nota\tg\t-3\t0\t=\t0\t0\t0\t4:39\n"
        "neuma\t11\t1\t-\n"
        "nota\tg\t-3\t0\tq\t0\t0\t0\t4:42\n"
        "neuma\t12\t1\t-\n"
        "nota\tg\t-3\t0\t~\t0\t0\t0\t4:45\n"
        "neuma\t13\t1\t-\n"
        "nota\tg\t-3\t0\t<\t0\t0\t0\t4:48\n"
        "neuma\t14\t1\t-\n"
        "nota\tg\t-3\t0\t>\t0\t0\t0\t4:51\n"
        "neuma\t15\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t4:55\n"
        "neuma\t16\t1\t-\n"
        "nota\tg\t-3\t0\t@\t0\t0\t0\t4:58\n"
        "neuma\t17\t1\t-\n"
        "nota\tg\t-3\t0\tI\t0\t0\t0\t4:60\n"
        "neuma\t18\t1\t-\n"
        "nota\tg\t-3\t0\tI\t0\t0\t0\t4:62\n"
        "neuma\t19\t1\t-\n"
        "nota\tg\t-3\t0\to\t0\t0\t0\t4:65\n"
        "syllaba\t2\t1\tb\n"
        "neuma\t20\t2\t-\n"
        "nota\tg\t-3\t0\tv\t0\t0\t0\t5:3\n"
        "nota\tg\t-3\t0\tv\t0\t0\t0\t5:5\n"
        "neuma\t22\t3\t-\n"
        "nota\tg\t-3\t0\ts\t0\t0\t0\t5:7\n"
        "nota\tg\t-3\t0\ts\t0\t0\t0\t5:9\n"
        "nota\tg\t-3\t0\ts\t0\t0\t0\t5:10\n"
        "neuma\t25\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:12\n"
        "neuma\t26\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:16\n"
        "neuma\t27\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:20\n"
        "neuma\t28\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:24\n"
        "neuma\t29\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t0\t5:28\n");
}

/* T3: mora (. ..), episema (_ cum numeris), ictus (') et loci */
interior vacuum
_signa_rhythmica_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "signa_rhythmica.gabc");

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_notarum == IX);
    CREDO_AEQUALIS_S32 ((s32)g->notae[ZEPHYRUM].mora_locus, (s32)-I);
    CREDO_AEQUALIS_S32 ((s32)g->notae[II].mora_locus, (s32)I);
    CREDO_AEQUALIS_S32 ((s32)g->notae[VI].ictus_locus, (s32)-I);
    CREDO_AEQUALIS_S32 ((s32)g->notae[VII].ictus_locus, (s32)I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_tabula(c, g),
        "caput\tname\tsigna\n"
        "syllaba\t0\t0\t\n"
        "clavis\tc\t4\t0\n"
        "syllaba\t1\t1\ta\n"
        "neuma\t0\t1\t-\n"
        "nota\tg\t-3\t0\t-\t1\t0\t0\t4:8\n"
        "neuma\t1\t1\t-\n"
        "nota\tg\t-3\t0\t-\t2\t0\t0\t4:11\n"
        "neuma\t2\t1\t-\n"
        "nota\tg\t-3\t0\t-\t1\t0\t0\t4:15\n"
        "neuma\t3\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t1\t0\t4:19\n"
        "neuma\t4\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t3\t0\t4:22\n"
        "neuma\t5\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t13\t0\t4:26\n"
        "neuma\t6\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t1\t4:31\n"
        "neuma\t7\t1\t-\n"
        "nota\tg\t-3\t0\t-\t0\t0\t1\t4:34\n"
        "neuma\t8\t1\t-\n"
        "nota\tg\t-3\t0\t-\t1\t1\t1\t4:38\n");
}

/* Review Focus 1: alteratio valet usque ad finem verbi, divisionem,
 * signum aliud in eadem positione; positio sola (non octava); ficta
 * (r6-r8) notam solam; molle et parenthesis alterant */
interior vacuum
_alterationis_ambitum_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "alterationes.gabc");
           s32  sperata[XIV];
           i32  k;

    /* Sa(ixih) lu(i) | ta(i) | mi(ixi iyi) | re(ixib) | do(ixi) (,) (i)
     * | fa(gr6g) | la(iXi) | si(ix?i) */
    sperata[ZEPHYRUM] = -I;  sperata[I] = ZEPHYRUM;  sperata[II] = -I;
    sperata[III] = ZEPHYRUM; sperata[IV] = -I;       sperata[V] =
                                                         ZEPHYRUM;
    sperata[VI] = -I;        sperata[VII] = ZEPHYRUM; sperata[VIII] =
                                                          -I;
    sperata[IX] = ZEPHYRUM;  sperata[X] = -I;        sperata[XI] =
                                                         ZEPHYRUM;
    sperata[XII] = -I;       sperata[XIII] = -I;

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_notarum == XIV);
    per (k = ZEPHYRUM; k < XIV; k++)
    {
        CREDO_AEQUALIS_S32 (g->notae[k].alteratio, sperata[k]);
    }
    CREDO_AEQUALIS_I32 ((i32)g->notae[X].signa, (i32)GABC_SIGNUM_FICTA);
    CREDO_AEQUALIS_I32 ((i32)g->notae[XI].signa, ZEPHYRUM);
    /* la(iXi): signum molle; si(ix?i): parenthesis */
    CREDO_NECESSE (g->syllabae[X].numerus_elementorum == II);
    CREDO_AEQUALIS_S32 ((s32)g->syllabae[X].elementa[ZEPHYRUM].genus,
        (s32)GABC_EL_ALTERATIO);
    CREDO_VERUM (g->syllabae[X].elementa[ZEPHYRUM].molle);
    CREDO_NECESSE (g->syllabae[XI].numerus_elementorum == II);
    CREDO_VERUM (g->syllabae[XI].elementa[ZEPHYRUM].parenthesis);
    CREDO_FALSUM (g->syllabae[XI].elementa[ZEPHYRUM].molle);
}

/* Review Focus 2: clavis mutata sequentes solas resolvit (etiam intra
 * syllabam); cb3 si bemolle usque ad clavem proximam */
interior vacuum
_clavem_mutatam_probare (
    CredoContextus* c)
{
    GabcCantus* g = _legere_fixum(c, "claves_mutatae.gabc");
           s32  gradus[IX];
           s32  alteratio[IX];
           i32  k;

    /* a(g h | c3 | h g) (cb3) b(g n) c(gyg) (c4) d(g i) */
    gradus[ZEPHYRUM] = -III; gradus[I] = -II; gradus[II]  = ZEPHYRUM;
    gradus[III]                                           =
        -I;        gradus[IV] = -I; gradus[V] = VI;
    gradus[VI] =
        -I;         gradus[VII] = -III; gradus[VIII] = -I;
    per (k = ZEPHYRUM; k < IX; k++)
    {
        alteratio[k] = (k == IV || k == V) ? -I : ZEPHYRUM;
    }

    CREDO_NECESSE (g != NIHIL);
    CREDO_AEQUALIS_I32 (g->numerus_vitiorum, ZEPHYRUM);
    CREDO_NECESSE (g->numerus_notarum == IX);
    per (k = ZEPHYRUM; k < IX; k++)
    {
        CREDO_AEQUALIS_S32 (g->notae[k].gradus, gradus[k]);
        CREDO_AEQUALIS_S32 (g->notae[k].alteratio, alteratio[k]);
    }
    CREDO_NECESSE (g->syllabae[I].numerus_elementorum == III);
    CREDO_AEQUALIS_S32 ((s32)g->syllabae[I].elementa[I].genus,
        (s32)GABC_EL_CLAVIS);
    CREDO_VERUM (g->syllabae[II].elementa[ZEPHYRUM].clavis_bemolle);
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
    { "formae notarum, vv sss, directio, signa", _formas_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "signa rhythmica: mora, episema, ictus", _signa_rhythmica_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "alterationes: ambitus (verbum, divisio, signum)",
        _alterationis_ambitum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "claves mutatae et cb", _clavem_mutatam_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "salve_2435.gabc: sine vitio", _salve_legere_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("gabc", SECTIONES, NIHIL, NIHIL);
}
