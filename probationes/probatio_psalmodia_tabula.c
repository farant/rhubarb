/* probatio_psalmodia_tabula.c - lector tabulae tonorum
 * (include/psalmodia_tabula.h)
 *
 * Plan: project-specs/psalmodia-plan-1.md R2 (lectio, forma
 * canonica, totalitas). Tabula vera: musica/toni_psalmorum.stml;
 * cetera fixa hic in literis (UTF-8 per octales: \303\263 = o
 * acutum, \342\200\240 = crux).
 *
 * GRAMMATICA FORMAE CANONICAE (psalmodia_tabulam_scribere), linea una
 * per rem, columnae tabulis separatae, '-' = vacua; textus effugitur
 * (\t \n \r \\). Ordo: tonus, initium, flexa, mediationes (quaeque
 * cum locis suis), terminationes (item), exempla - intra genus ordine
 * fontis.
 *   tonus       modus  clavis  tenor  pagina  altera(0|1)
 *   initium     notae syllabarum spatio iunctae      (si adest)
 *   flexa       notae                                (si adest)
 *   mediatio    differentia  monosyllaba(0|1)  ad_libitum(0|1)
 *               vicaria  clavis  tenor  initium
 *   terminatio  (columnae ut mediatio)
 *   locus       praeparatio|accentus|nota  notae  superveniens
 *               anticipata(0|1)
 *   exemplum    pars  pagina  forma  differentia  nota
 *               monosyllaba(0|1)  primus(0|1)  canticum(0|1)  textus
 *               notae syllabarum spatio iunctae
 */

#include "latina.h"
#include "credo.h"
#include "psalmodia_tabula.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "piscina.h"
#include "sors.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

interior PsalmodiaTabula*
_legere (
         CredoContextus* ctx,
     constans character* literae)
{
    redde psalmodia_tabulam_legere(chorda_ex_literis(literae,
        ctx->piscina), ctx->piscina);
}

interior chorda
_forma (
              CredoContextus* ctx,
    constans PsalmodiaTabula* tabula)
{
    ChordaAedificator* a = chorda_aedificator_creare(ctx->piscina, MMM);

    si (!psalmodia_tabulam_scribere(tabula, a))
    {
        redde chorda_ex_literis("(forma fracta)", ctx->piscina);
    }
    redde chorda_aedificator_finire(a);
}

interior chorda
_vitia (
              CredoContextus* ctx,
    constans PsalmodiaTabula* tabula)
{
    ChordaAedificator* a = chorda_aedificator_creare(ctx->piscina, MMM);

    si (!psalmodia_vitia_scribere(tabula, "x", a))
    {
        redde chorda_ex_literis("(vitia fracta)", ctx->piscina);
    }
    redde chorda_aedificator_finire(a);
}

interior b32
_aequat (
                 chorda  ch,
     constans character* literae)
{
    redde chorda_aequalis_literis(ch, literae);
}

/* tonus modi dati (altera: positio altera) aut NIHIL */
interior constans PsalmodiaTonus*
_tonus (
    constans PsalmodiaTabula* tabula,
          constans character* modus,
                         b32  altera)
{
    i32 k;

    per (k = ZEPHYRUM; k < tabula->numerus_tonorum; k++)
    {
        si (   _aequat(tabula->toni[k].modus, modus)
            && tabula->toni[k].positio_altera == altera)
        {
            redde &tabula->toni[k];
        }
    }
    redde NIHIL;
}

interior constans PsalmodiaClausula*
_terminatio (
     constans PsalmodiaTonus* tonus,
          constans character* differentia)
{
    i32 k;

    per (k = ZEPHYRUM; k < tonus->numerus_terminationum; k++)
    {
        si (_aequat(tonus->terminationes[k].differentia, differentia))
        {
            redde &tonus->terminationes[k];
        }
    }
    redde NIHIL;
}

interior b32
_locus_aequat (
    constans PsalmodiaLocus* locus,
         PsalmodiaLociGenus  genus,
         constans character* notae,
         constans character* superveniens)
{
    redde locus->genus == genus && _aequat(locus->notae, notae)
        && _aequat(locus->superveniens, superveniens);
}


    /* ==================================================
     * Tabula vera
     * ================================================== */

interior vacuum
_tabulam_veram_probare (
    CredoContextus* ctx)
{
                        chorda  fons;
               PsalmodiaTabula* t;
       constans PsalmodiaTonus* tonus;
    constans PsalmodiaClausula* cl;
                           i32  k;
                           i32  mediationes    = ZEPHYRUM;
                           i32  terminationes  = ZEPHYRUM;
                           i32  exempla        = ZEPHYRUM;
                       clock_t  initium;

    fons = filum_legere_totum("musica/toni_psalmorum.stml",
        ctx->piscina);
    CREDO_NECESSE (fons.mensura > ZEPHYRUM);
    initium  = clock();
    t        = psalmodia_tabulam_legere(fons, ctx->piscina);
    imprimere("  tabula vera lecta: %ld ms\n",
        (longus)((clock() - initium) * M / CLOCKS_PER_SEC));
    CREDO_NECESSE (t != NIHIL);
    si (t->numerus_vitiorum > ZEPHYRUM)
    {
        chorda v = _vitia(ctx, t);
        imprimere("%.*s", (integer)v.mensura, (character*)v.datum);
    }
    CREDO_AEQUALIS_I32 (t->numerus_vitiorum, ZEPHYRUM);
    CREDO_VERUM (psalmodia_successus(t));
    CREDO_AEQUALIS_I32 (t->numerus_tonorum, (i32)XI);
    per (k = ZEPHYRUM; k < t->numerus_tonorum; k++)
    {
        mediationes    += t->toni[k].numerus_mediationum;
        terminationes  += t->toni[k].numerus_terminationum;
        exempla        += t->toni[k].numerus_exemplorum;
    }
    CREDO_AEQUALIS_I32 (mediationes, (i32)XX);
    CREDO_AEQUALIS_I32 (terminationes, (i32)XXXV);
    CREDO_AEQUALIS_I32 (exempla, (i32)CCXLVI);

    /* tonus I */
    tonus = _tonus(t, "1", FALSUM);
    CREDO_NECESSE (tonus != NIHIL);
    CREDO_AEQUALIS_S32 (tonus->modus_numerus, (s32)I);
    CREDO_VERUM (_aequat(tonus->clavis, "c4"));
    CREDO_VERUM (_aequat(tonus->tenor, "h"));
    CREDO_VERUM (_aequat(tonus->pagina, "4*"));
    CREDO_NECESSE (tonus->numerus_initii == II);
    CREDO_VERUM (_aequat(tonus->initium[ZEPHYRUM], "f"));
    CREDO_VERUM (_aequat(tonus->initium[I], "gh"));
    CREDO_VERUM (_aequat(tonus->flexa, "g"));
    CREDO_NECESSE (tonus->numerus_mediationum >= I);
    cl = &tonus->mediationes[ZEPHYRUM];
    CREDO_NECESSE (cl->numerus_locorum == IV);
    CREDO_VERUM (_locus_aequat(&cl->loci[ZEPHYRUM],
        PSALMODIA_LOCUS_ACCENTUS, "ixi", "h"));
    CREDO_VERUM (_locus_aequat(&cl->loci[I], PSALMODIA_LOCUS_NOTA,
        "h", ""));
    CREDO_VERUM (_locus_aequat(&cl->loci[II], PSALMODIA_LOCUS_ACCENTUS,
        "g", "h"));
    CREDO_VERUM (_locus_aequat(&cl->loci[III], PSALMODIA_LOCUS_NOTA,
        "h", ""));
    CREDO_FALSUM (cl->monosyllaba);
    CREDO_FALSUM (cl->ad_libitum);

    /* D2: superveniens anticipata */
    cl = _terminatio(tonus, "D2");
    CREDO_NECESSE (cl != NIHIL && cl->numerus_locorum >= II);
    CREDO_VERUM (_locus_aequat(&cl->loci[cl->numerus_locorum - II],
        PSALMODIA_LOCUS_ACCENTUS, "gf", "g"));
    CREDO_VERUM (cl->loci[cl->numerus_locorum - II].anticipata);
    CREDO_VERUM (_locus_aequat(&cl->loci[cl->numerus_locorum - I],
        PSALMODIA_LOCUS_NOTA, "d", ""));

    /* peregrinus: tenor et initium secundae partis */
    tonus = _tonus(t, "peregrinus", FALSUM);
    CREDO_NECESSE (tonus != NIHIL);
    CREDO_AEQUALIS_S32 (tonus->modus_numerus, (s32)ZEPHYRUM);
    CREDO_NECESSE (tonus->numerus_terminationum == I);
    CREDO_VERUM (_aequat(tonus->terminationes[ZEPHYRUM].tenor, "g"));
    CREDO_VERUM (_aequat(tonus->terminationes[ZEPHYRUM].initium,
        "hixi"));
    CREDO_CHORDA_VACUA (tonus->terminationes[ZEPHYRUM].differentia);

    /* IV altera */
    tonus = _tonus(t, "4", VERUM);
    CREDO_NECESSE (tonus != NIHIL);
    CREDO_VERUM (_aequat(tonus->clavis, "c3"));
    CREDO_AEQUALIS_S32 (tonus->modus_numerus, (s32)IV);
    CREDO_NON_NIHIL (_terminatio(tonus, "A*"));

    /* in directum: terminatio monosyllaba */
    tonus = _tonus(t, "in directum", FALSUM);
    CREDO_NECESSE (tonus != NIHIL);
    CREDO_AEQUALIS_I32 (tonus->numerus_initii, ZEPHYRUM);
    CREDO_NECESSE (tonus->numerus_terminationum == II);
    CREDO_VERUM (tonus->terminationes[I].monosyllaba);
}


    /* ==================================================
     * Forma canonica
     * ================================================== */

interior constans character FIXUM_DUO_TONI[] =
    "<toni_psalmorum>\n"
    "  <tonus modus=\"2\" clavis=\"f3\" tenor=\"h\" pagina=\"8*\">\n"
    "    <initium>e f</initium>\n"
    "    <flexa>f</flexa>\n"
    "    <!-- commentarium nihil confert -->\n"
    "    <mediatio>\n"
    "      <accentus superveniens=\"h\">i</accentus>\n"
    "      <nota>h</nota>\n"
    "    </mediatio>\n"
    "    <mediatio casus=\"monosyllaba\"><nota>i</nota></mediatio>\n"
    "    <terminatio differentia=\"D\" vicaria=\"A\">\n"
    "      <praeparatio>g</praeparatio>\n"
    "      <accentus superveniens=\"f\">e</accentus>\n"
    "      <nota>f</nota>\n"
    "    </terminatio>\n"
    "    <exemplum pagina=\"8*\" pars=\"terminatio\"\n"
    "        differentia=\"D\"\n"
    "        textus=\"D\303\263mino meo\" notae=\"h g e fr f\"/>\n"
    "  </tonus>\n"
    "  <tonus modus=\"peregrinus\" clavis=\"c4\" tenor=\"h\""
    " pagina=\"24*\">\n"
    "    <terminatio tenor=\"g\" initium=\"hixi\"\n"
    "        ad_libitum=\"verum\">\n"
    "      <accentus superveniens=\"f\"\n"
    "          anticipata=\"verum\">f</accentus>\n"
    "      <nota>ed</nota>\n"
    "    </terminatio>\n"
    "    <exemplum pars=\"versus\" primus=\"falsum\"\n"
    "        canticum=\"verum\"\n"
    "        forma=\"A\" nota=\"hebr.\" casus=\"monosyllaba\"\n"
    "        textus=\"a b\" notae=\"h * g\"/>\n"
    "  </tonus>\n"
    "</toni_psalmorum>\n";

interior constans character FORMA_DUO_TONI[] =
    "tonus\t2\tf3\th\t8*\t0\n"
    "initium\te f\n"
    "flexa\tf\n"
    "mediatio\t-\t0\t0\t-\t-\t-\t-\n"
    "locus\taccentus\ti\th\t0\n"
    "locus\tnota\th\t-\t0\n"
    "mediatio\t-\t1\t0\t-\t-\t-\t-\n"
    "locus\tnota\ti\t-\t0\n"
    "terminatio\tD\t0\t0\tA\t-\t-\t-\n"
    "locus\tpraeparatio\tg\t-\t0\n"
    "locus\taccentus\te\tf\t0\n"
    "locus\tnota\tf\t-\t0\n"
    "exemplum\tterminatio\t8*\t-\tD\t-\t0\t1\t0\tD\303\263mino meo"
    "\th g e fr f\n"
    "tonus\tperegrinus\tc4\th\t24*\t0\n"
    "terminatio\t-\t0\t1\t-\t-\tg\thixi\n"
    "locus\taccentus\tf\tf\t1\n"
    "locus\tnota\ted\t-\t0\n"
    "exemplum\tversus\t-\tA\t-\thebr.\t1\t0\t1\ta b\th * g\n";

interior vacuum
_scripturam_probare (
    CredoContextus* ctx)
{
    PsalmodiaTabula* t = _legere(ctx, FIXUM_DUO_TONI);

    CREDO_NECESSE (t != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_vitia(ctx, t), "");
    CREDO_CHORDA_AEQUALIS_LITERIS (_forma(ctx, t), FORMA_DUO_TONI);
}


    /* ==================================================
     * Exempla: notae per syllabam
     * ================================================== */

interior constans character FIXUM_EXEMPLA[] =
    "<toni_psalmorum>\n"
    "<tonus modus=\"1\" clavis=\"c4\" tenor=\"h\">\n"
    "<exemplum pars=\"versus\" textus=\"x\" notae=\"f  gh h\n"
    "   g \342\200\240 h  hr * g \"/>\n"
    "<exemplum pars=\"semiversus\" primus=\"falsum\" canticum=\"verum\""
    " textus=\"y\" notae=\"h\"/>\n"
    "</tonus>\n"
    "</toni_psalmorum>\n";

interior vacuum
_exempla_probare (
    CredoContextus* ctx)
{
    constans character* exspectatae[] = {
        "f", "gh", "h", "g", "\342\200\240", "h", "hr", "*", "g"
    };
               PsalmodiaTabula* t = _legere(ctx, FIXUM_EXEMPLA);
    constans PsalmodiaExemplum* ex;
                           i32  k;

    CREDO_NECESSE (t != NIHIL);
    CREDO_CHORDA_AEQUALIS_LITERIS (_vitia(ctx, t), "");
    CREDO_NECESSE (t->numerus_tonorum == I);
    CREDO_NECESSE (t->toni[ZEPHYRUM].numerus_exemplorum == II);
    ex = &t->toni[ZEPHYRUM].exempla[ZEPHYRUM];
    CREDO_AEQUALIS_I32 ((i32)ex->pars, (i32)PSALMODIA_PARS_VERSUS);
    CREDO_NECESSE (ex->numerus_notarum == IX);
    per (k = ZEPHYRUM; k < (i32)IX; k++)
    {
        CREDO_VERUM (_aequat(ex->notae[k], exspectatae[k]));
    }
    CREDO_VERUM (ex->primus);
    CREDO_FALSUM (ex->canticum);
    CREDO_AEQUALIS_I32 (ex->linea, (i32)III);
    ex = &t->toni[ZEPHYRUM].exempla[I];
    CREDO_AEQUALIS_I32 ((i32)ex->pars, (i32)PSALMODIA_PARS_SEMIVERSUS);
    CREDO_FALSUM (ex->primus);
    CREDO_VERUM (ex->canticum);
    CREDO_AEQUALIS_I32 (ex->linea, (i32)V);
}


    /* ==================================================
     * Totalitas
     * ================================================== */

/* tonus bonus (linea II), tonus sine clave (III), elementum ignotum
 * (VII) */
interior constans character FIXUM_MIXTUM[] =
    "<toni_psalmorum>\n"
    "<tonus modus=\"1\" clavis=\"c4\" tenor=\"h\">"
    "<mediatio><nota>h</nota></mediatio></tonus>\n"
    "<tonus modus=\"2\" tenor=\"h\">\n"
    "<mediatio><nota>h</nota></mediatio>\n"
    "</tonus>\n"
    "<tonus modus=\"3\" clavis=\"c4\" tenor=\"j\"><terminatio>\n"
    "<praeparatoi>g</praeparatoi><nota>g</nota></terminatio></tonus>\n"
    "</toni_psalmorum>\n";

/* lectio et scriptura plena - in filio sub CREDO_NON_RUIT */
interior vacuum
_omnia_scribere (
    constans i8* octeti,
            i32  mensura)
{
              Piscina* p;
               chorda  fons;
      PsalmodiaTabula* t;
    ChordaAedificator* a;

    p = piscina_generare_dynamicum("psalmodia_totalitas",
            (memoriae_index)(LXIV * MXXIV));
    fons.datum = (i8*)malloc((size_t)(mensura
        > ZEPHYRUM ? mensura : I));
    si (p == NIHIL || fons.datum == NIHIL)
    {
        redde;
    }
    si (mensura > ZEPHYRUM)
    {
        memcpy(fons.datum, octeti, (size_t)mensura);
    }
    fons.mensura  = mensura;
    t             = psalmodia_tabulam_legere(fons, p);
    a             = chorda_aedificator_creare(p, MXXIV);
    si (t != NIHIL && a != NIHIL)
    {
        (vacuum)psalmodia_tabulam_scribere(t, a);
        (vacuum)psalmodia_vitia_scribere(t, "x", a);
    }
    free(fons.datum);
    piscina_destruere(p);
}

interior vacuum
_totalitas_probare (
    CredoContextus* ctx)
{
    constans character* fracta[] = {
        "",
        "<toni_psalmorum>",
        "<toni_psalmorum><tonus modus=\"1\" clavis=\"c4\"",
        "<instrumentarium/>",
        "<toni_psalmorum>textus nudus</toni_psalmorum>",
        NIHIL
    };
               PsalmodiaTabula* t;
    constans PsalmodiaClausula* cl;
                            i8  temere[MM];
                          Sors  sors;
                           i32  k;

    per (k = ZEPHYRUM; fracta[k] != NIHIL; k++)
    {
        CREDO_NON_RUIT (_omnia_scribere((constans i8*)fracta[k],
            (i32)strlen(fracta[k])));
        t = _legere(ctx, fracta[k]);
        CREDO_NECESSE (t != NIHIL);
        CREDO_MAIOR_I32 (t->numerus_vitiorum, ZEPHYRUM);
        CREDO_FALSUM (psalmodia_successus(t));
    }
    t = _legere(ctx, "<instrumentarium/>");
    CREDO_AEQUALIS_I32 (t->numerus_tonorum, ZEPHYRUM);
    CREDO_CHORDA_CONTINET (_vitia(ctx, t),
        chorda_ex_literis("instrumentarium", ctx->piscina));

    sors_seminare(&sors, (i64)MMXXVI, (i64)ZEPHYRUM);
    per (k = ZEPHYRUM; k < (i32)MM; k++)
    {
        temere[k] = (i8)sors_intra(&sors, CCLVI);
    }
    CREDO_NON_RUIT (_omnia_scribere(temere, (i32)MM));

    /* tabula mixta: toni boni manent, vitia locata */
    CREDO_NON_RUIT (_omnia_scribere((constans i8*)FIXUM_MIXTUM,
        (i32)strlen(FIXUM_MIXTUM)));
    t = _legere(ctx, FIXUM_MIXTUM);
    CREDO_NECESSE (t != NIHIL);
    CREDO_AEQUALIS_I32 (t->numerus_tonorum, (i32)II);
    CREDO_VERUM (_aequat(t->toni[ZEPHYRUM].modus, "1"));
    CREDO_VERUM (_aequat(t->toni[I].modus, "3"));
    CREDO_NECESSE (t->numerus_vitiorum == II);
    CREDO_AEQUALIS_I32 (t->vitia[ZEPHYRUM].linea, (i32)III);
    CREDO_CHORDA_CONTINET (t->vitia[ZEPHYRUM].causa,
        chorda_ex_literis("clavis", ctx->piscina));
    CREDO_AEQUALIS_I32 (t->vitia[I].linea, (i32)VII);
    CREDO_CHORDA_CONTINET (t->vitia[I].causa,
        chorda_ex_literis("praeparatoi", ctx->piscina));
    /* elementum ignotum praeteritur, locus sequens legitur */
    cl = &t->toni[I].terminationes[ZEPHYRUM];
    CREDO_AEQUALIS_I32 (cl->numerus_locorum, (i32)I);

    /* attributum ignotum: vitium, tonus tamen legitur */
    t = _legere(ctx,
        "<toni_psalmorum>\n<tonus modus=\"1\" clavis=\"c4\""
        " tenor=\"h\" tonica=\"h\"/>\n</toni_psalmorum>\n");
    CREDO_NECESSE (t != NIHIL);
    CREDO_AEQUALIS_I32 (t->numerus_tonorum, (i32)I);
    CREDO_CHORDA_AEQUALIS_LITERIS (_vitia(ctx, t),
        "x:2: attributum ignotum \"tonica\" in <tonus>\n");
}

interior constans CredoSectio SECTIONES[] = {
    { "tabula vera: XI toni sine vitio, loci, variantes",
        _tabulam_veram_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "forma canonica tabulae duorum tonorum", _scripturam_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "exempla: notae per syllabam, primus, canticum",
        _exempla_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "totalitas: fracta, temere, mixta - toni boni manent",
        _totalitas_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("psalmodia_tabula", SECTIONES, NIHIL,
        NIHIL);
}
