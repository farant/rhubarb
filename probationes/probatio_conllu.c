/* probatio_conllu.c - probationes lectoris CoNLL-U (conllu.h)
 *
 * Probationes lectoris ipsius ex probatio_oratio_oraculum.c translatae
 * (2026-10-17, knotapel D130 decretum 4 a): campi, rangae, nodi vacui,
 * CRLF, '# newdoc id', textus reconstructus, SpaceAfter, vitia cum
 * linea. Novae: caput numeratum, proprietates FEATS, arbor dependentiae
 * (conllu_arbor). Treebanks venditae (oratio/probationes/fixa/ud):
 * sententiae, verba et rangae contra FONTES.md; arbor omnis sana.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "credo.h"
#include "xar.h"
#include "filum.h"
#include "conllu.h"
#include "dependentia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_l (
    constans character* literae)
{
    chorda c;

    c.datum    = (i8*)(vacuum*)(memoriae_index)literae;
    c.mensura  = (i32)strlen(literae);
    redde c;
}

interior b32
_aequalis (
                chorda  c,
    constans character* literae)
{
    redde chorda_aequalis_literis(c, literae);
}

interior constans ConlluLexema*
_lexema (
    constans ConlluSententia* s,
                         i32  k)
{
    redde (constans ConlluLexema*)xar_obtinere(s->lexemata, k);
}

/* fixura communis: duae sententiae (nodus vacuus, CRLF, ranga, newdoc) */
interior Xar*
_fixura (
         Piscina* piscina,
    ConlluVitium* vitium)
{
    redde conllu_legere(piscina, _l(
        "# sent_id = a-1\n"
        "# text = Puella rosam amat.\n"
        "1\tPuella\tpuella\tNOUN\t_\tCase=Nom\t3\tnsubj\t_\t_\n"
        "2\trosam\trosa\tNOUN\t_\t_\t3\tobj\t_\t_\n"
        "3\tamat\tamo\tVERB\t_\t_\t0\troot\t_\tSpaceAfter=No\n"
        "3.1\tzzz\tzzz\tX\t_\t_\t_\t_\t_\t_\n"
        "4\t.\t.\tPUNCT\t_\t_\t3\tpunct\t_\t_\n"
        "\n"
        "\n"
        "# newdoc id = phi0690.x\n"
        "# sent_id = a-2\r\n"
        "1-2\tpronumque\t_\t_\t_\t_\t_\t_\t_\t_\r\n"
        "1\tpronum\tpronus\tADJ\t_\t_\t3\tadvcl\t_\t_\r\n"
        "2\tque\tque\tCCONJ\t_\t_\t3\tcc\t_\t_\r\n"
        "3\tabstulit\taufero\tVERB\t_\t_\t0\troot\t_\tSpaceAfter=No\r\n"
        "4\t.\t.\tPUNCT\t_\t_\t3\tpunct\t_\t_\r\n"), vitium);
}

interior vacuum
_probare_lectionem (
    CredoContextus* c)
{
                     Piscina* piscina  = c->piscina;
                ConlluVitium  vitium   = { ZEPHYRUM, NIHIL };
                         Xar* s        = _fixura(piscina, &vitium);
    constans ConlluSententia* a;
    constans ConlluSententia* b;

    CREDO_NECESSE_NON_NIHIL (s);
    CREDO_NECESSE (xar_numerus(s) == (i32)II);
    a = (constans ConlluSententia*)xar_obtinere(s, ZEPHYRUM);
    b = (constans ConlluSententia*)xar_obtinere(s, I);
    CREDO_VERUM (_aequalis(a->id, "a-1"));
    CREDO_VERUM (_aequalis(a->textus, "Puella rosam amat."));
    CREDO_AEQUALIS_I32 (a->linea, I);
    CREDO_AEQUALIS_I32 (xar_numerus(a->lexemata), (i32)IV);   /* 3.1 omissus */
    CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->forma, "Puella"));
    CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->lemma, "puella"));
    CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->upos, "NOUN"));
    CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->feats, "Case=Nom"));
    CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->head, "3"));
    CREDO_VERUM (_aequalis(_lexema(a, ZEPHYRUM)->deprel, "nsubj"));
    CREDO_AEQUALIS_I32 (_lexema(a, ZEPHYRUM)->a, I);
    CREDO_FALSUM (_lexema(a, ZEPHYRUM)->ranga);
    CREDO_VERUM (_lexema(a, ZEPHYRUM)->spatium_post);
    CREDO_FALSUM (_lexema(a, (i32)II)->spatium_post);
    CREDO_AEQUALIS_I32 (_lexema(a, (i32)III)->linea, (i32)VII);
    /* caput numeratum */
    CREDO_AEQUALIS_S32 (_lexema(a, ZEPHYRUM)->caput, III);
    CREDO_AEQUALIS_S32 (_lexema(a, (i32)II)->caput, ZEPHYRUM);
    CREDO_AEQUALIS_S32 (_lexema(a, (i32)III)->caput, III);
    /* b: sine '# text', CRLF, ranga, documentum */
    CREDO_VERUM (_aequalis(b->id, "a-2"));
    CREDO_AEQUALIS_I32 (b->textus.mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (xar_numerus(b->lexemata), (i32)V);
    CREDO_VERUM (_lexema(b, ZEPHYRUM)->ranga);
    CREDO_AEQUALIS_I32 (_lexema(b, ZEPHYRUM)->a, I);
    CREDO_AEQUALIS_I32 (_lexema(b, ZEPHYRUM)->b, (i32)II);
    CREDO_VERUM (_aequalis(_lexema(b, ZEPHYRUM)->forma, "pronumque"));
    CREDO_VERUM (_aequalis(_lexema(b, ZEPHYRUM)->upos, "_"));
    CREDO_AEQUALIS_S32 (_lexema(b, ZEPHYRUM)->caput, (s32)-I);   /* '_' */
    CREDO_VERUM (_aequalis(_lexema(b, I)->forma, "pronum"));
    CREDO_VERUM (_aequalis(_lexema(b, (i32)III)->misc,
        "SpaceAfter=No"));
    CREDO_VERUM (_aequalis(_lexema(b, (i32)IV)->misc, "_"));
    CREDO_AEQUALIS_I32 (a->documentum.mensura, ZEPHYRUM);
    CREDO_VERUM (_aequalis(b->documentum, "phi0690.x"));
}

interior vacuum
_probare_textum (
    CredoContextus* c)
{
                     Piscina* piscina  = c->piscina;
                ConlluVitium  vitium   = { ZEPHYRUM, NIHIL };
                         Xar* s        = _fixura(piscina, &vitium);

    CREDO_NECESSE_NON_NIHIL (s);
    CREDO_VERUM (_aequalis(conllu_textus(piscina,
        (constans ConlluSententia*)xar_obtinere(s, ZEPHYRUM)),
        "Puella rosam amat."));
    /* reconstructus: ranga superficiem dat, verba eius omittuntur */
    CREDO_VERUM (_aequalis(conllu_textus(piscina,
        (constans ConlluSententia*)xar_obtinere(s, I)),
        "pronumque abstulit."));
}

interior vacuum
_probare_spatium (
    CredoContextus* c)
{
    (vacuum)c;
    CREDO_VERUM (conllu_spatium_post(_l("_")));
    CREDO_FALSUM (conllu_spatium_post(_l("SpaceAfter=No")));
    CREDO_FALSUM (conllu_spatium_post(_l("X=1|SpaceAfter=No|Y=2")));
    CREDO_VERUM (conllu_spatium_post(_l("NoSpaceAfter=No")));
}

interior vacuum
_probare_vitia (
    CredoContextus* c)
{
         Piscina* piscina  = c->piscina;
    ConlluVitium  vitium   = { ZEPHYRUM, NIHIL };
             Xar* s;

    vitium.linea = ZEPHYRUM;
    vitium.causa = NIHIL;
    CREDO_NIHIL (conllu_legere(piscina, _l(
        "1\tx\tx\tNOUN\t_\t_\t0\troot\t_\n"), &vitium));
    CREDO_AEQUALIS_I32 (vitium.linea, I);
    CREDO_VERUM (vitium.causa != NIHIL
        && strcmp(vitium.causa, "campi non X") == ZEPHYRUM);
    CREDO_NIHIL (conllu_legere(piscina, _l(
        "# c\n1\tx\tx\tNOUN\t_\t_\t0\troot\t_\t_\n"
        "y\tx\tx\tNOUN\t_\t_\t0\troot\t_\t_\n"), &vitium));
    CREDO_AEQUALIS_I32 (vitium.linea, (i32)III);
    CREDO_VERUM (strcmp(vitium.causa, "ID non numerus") == ZEPHYRUM);
    CREDO_NIHIL (conllu_legere(piscina, _l(
        "3-2\tx\t_\t_\t_\t_\t_\t_\t_\t_\n"), &vitium));
    CREDO_VERUM (strcmp(vitium.causa, "ranga mala") == ZEPHYRUM);
    /* vitium NIHIL licet */
    CREDO_NIHIL (conllu_legere(piscina, _l("x\n"), NIHIL));
    /* fons vacuus: sententiae nullae, non vitium */
    s = conllu_legere(piscina, _l("\n\n# c\n\n"), &vitium);
    CREDO_NON_NIHIL (s);
    CREDO_AEQUALIS_I32 (s ? xar_numerus(s) : (i32)-I, ZEPHYRUM);
}

interior vacuum
_probare_proprietates (
    CredoContextus* c)
{
    chorda f = _l("Case=Nom|Gender=Fem|Number=Sing");

    (vacuum)c;
    CREDO_VERUM (_aequalis(conllu_proprietas(f, "Case"), "Nom"));
    CREDO_VERUM (_aequalis(conllu_proprietas(f, "Gender"), "Fem"));
    CREDO_VERUM (_aequalis(conllu_proprietas(f, "Number"), "Sing"));
    CREDO_AEQUALIS_I32 (conllu_proprietas(f, "Person").mensura,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (conllu_proprietas(f, "Cas").mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (conllu_proprietas(_l("_"), "Case").mensura,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (conllu_proprietas(_l(""), "Case").mensura,
        ZEPHYRUM);
    /* clavis tota: Case[psor] non Case */
    CREDO_VERUM (_aequalis(conllu_proprietas(_l("Case[psor]=Gen|Case=Abl"),
        "Case"), "Abl"));
    CREDO_VERUM (_aequalis(conllu_proprietas(_l("Case[psor]=Gen|Case=Abl"),
        "Case[psor]"), "Gen"));
    CREDO_AEQUALIS_I32 (conllu_proprietas(_l("Case=|Number=Plur"),
        "Case").mensura, ZEPHYRUM);
    CREDO_VERUM (_aequalis(conllu_proprietas(_l("Case=|Number=Plur"),
        "Number"), "Plur"));
}

interior vacuum
_probare_arborem (
    CredoContextus* c)
{
                     Piscina* piscina  = c->piscina;
                ConlluVitium  vitium   = { ZEPHYRUM, NIHIL };
                         Xar* s        = _fixura(piscina, &vitium);
                 ConlluArbor  arbor;
                         Xar* t;

    CREDO_NECESSE_NON_NIHIL (s);
    CREDO_NECESSE (conllu_arbor(piscina,
        (constans ConlluSententia*)xar_obtinere(s, ZEPHYRUM), &arbor));
    CREDO_AEQUALIS_I32 (arbor.arbor.numerus, IV);
    CREDO_AEQUALIS_I32 (arbor.arbor.capita[ZEPHYRUM], III);
    CREDO_AEQUALIS_I32 (arbor.arbor.capita[I], III);
    CREDO_AEQUALIS_I32 (arbor.arbor.capita[II], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (arbor.arbor.capita[III], III);
    CREDO_VERUM (_aequalis(arbor.verba[III]->forma, "."));
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(&arbor.arbor),
        (s32)DEPENDENTIA_SANA);
    /* ranga omissa: verba pronum que abstulit . */
    CREDO_NECESSE (conllu_arbor(piscina,
        (constans ConlluSententia*)xar_obtinere(s, I), &arbor));
    CREDO_AEQUALIS_I32 (arbor.arbor.numerus, IV);
    CREDO_VERUM (_aequalis(arbor.verba[ZEPHYRUM]->forma, "pronum"));
    CREDO_VERUM (_aequalis(arbor.verba[I]->forma, "que"));
    CREDO_AEQUALIS_I32 (arbor.arbor.capita[ZEPHYRUM], III);
    CREDO_AEQUALIS_I32 (arbor.arbor.capita[II], ZEPHYRUM);
    /* ID non consecutivi: FALSUM */
    t = conllu_legere(piscina, _l(
        "1\ta\ta\tX\t_\t_\t0\troot\t_\t_\n"
        "3\tb\tb\tX\t_\t_\t1\tdep\t_\t_\n"), &vitium);
    CREDO_NECESSE_NON_NIHIL (t);
    CREDO_FALSUM (conllu_arbor(piscina,
        (constans ConlluSententia*)xar_obtinere(t, ZEPHYRUM), &arbor));
    /* HEAD '_' in verbo: caput extra numerum, validatio reicit */
    t = conllu_legere(piscina, _l(
        "1\ta\ta\tX\t_\t_\t0\troot\t_\t_\n"
        "2\tb\tb\tX\t_\t_\t_\tdep\t_\t_\n"), &vitium);
    CREDO_NECESSE_NON_NIHIL (t);
    CREDO_NECESSE (conllu_arbor(piscina,
        (constans ConlluSententia*)xar_obtinere(t, ZEPHYRUM), &arbor));
    CREDO_AEQUALIS_I32 (arbor.arbor.capita[I], III);
    CREDO_AEQUALIS_S32 ((s32)dependentia_validare(&arbor.arbor),
        (s32)DEPENDENTIA_CAPUT_EXTRA);
}

/* treebanks venditae: sententiae, verba, rangae (FONTES.md); arbor
 * omnis sana */
interior vacuum
_probare_fixuras (
    CredoContextus* c)
{
    nomen structura {
        constans character* plagula;
                       i32  sententiae;
                       i32  verba;
                       i32  rangae;
    } Exspectatum;
    constans Exspectatum exspectata[V] = {
        { "la_circse-ud-test.conllu", DCCCXCIII,
          (i32)(XI * M + DIII), CCCXVI },
        { "la_llct-ud-dev.conllu", DCCCL, (i32)(XXIV * M + CLXXXIX),
            III },
        { "la_llct-ud-test.conllu", DCCCLXXXIV, (i32)(XXIV * M + LXXIX),
          II },
        { "en_ewt-ud-dev.conllu", MMI, (i32)(XXV * M + CXLVIII),
          CCCLIX },
        { "en_ewt-ud-test.conllu", MMLXXVII, (i32)(XXV * M + XCIV),
          CCCLIV }
    };
     constans character* radix = getenv("RHUBARB_RADIX");
                    i32  f;

    si (radix == NIHIL)
    {
        radix = ".";
    }
    per (f = ZEPHYRUM; f < (i32)V; f++)
    {
        Piscina* p = piscina_generare_dynamicum("conllu_treebank",
            1048576);
        character  via[512];
           chorda  fons;
              Xar* s;
     ConlluVitium  vitium = { ZEPHYRUM, NIHIL };
              i32  k;
              i32  verba   = ZEPHYRUM;
              i32  rangae  = ZEPHYRUM;
              i32  sanae   = ZEPHYRUM;

        (vacuum)c;
        si (strlen(radix) + strlen(exspectata[f].plagula) + XL
            >= magnitudo(via))
        {
            CREDO_CULPA ("RHUBARB_RADIX nimis longa");
            piscina_destruere(p);
            perge;
        }
        sprintf(via, "%s/oratio/probationes/fixa/ud/%s", radix,
            exspectata[f].plagula);
        fons = filum_legere_totum(via, p);
        si (fons.datum == NIHIL)
        {
            imprimere("  absens: %s\n", via);
            CREDO_CULPA ("treebank venditum absens");
            piscina_destruere(p);
            perge;
        }
        s = conllu_legere(p, fons, &vitium);
        CREDO_NON_NIHIL (s);
        si (s == NIHIL)
        {
            imprimere("  %s: linea %u: %s\n", exspectata[f].plagula,
                (insignatus integer)vitium.linea,
                vitium.causa != NIHIL ? vitium.causa : "?");
            piscina_destruere(p);
            perge;
        }
        CREDO_AEQUALIS_I32 (xar_numerus(s), exspectata[f].sententiae);
        per (k = ZEPHYRUM; k < xar_numerus(s); k++)
        {
            constans ConlluSententia* t =
                (constans ConlluSententia*)xar_obtinere(s, k);
                         ConlluArbor arbor;
                                 i32 j;

            per (j = ZEPHYRUM; j < xar_numerus(t->lexemata); j++)
            {
                si (_lexema(t, j)->ranga)
                {
                    rangae = rangae + I;
                }
            }
            si (conllu_arbor(p, t, &arbor))
            {
                verba = verba + arbor.arbor.numerus;
                si (dependentia_validare(&arbor.arbor)
                    == DEPENDENTIA_SANA)
                {
                    sanae = sanae + I;
                }
            }
        }
        CREDO_AEQUALIS_I32 (verba, exspectata[f].verba);
        CREDO_AEQUALIS_I32 (rangae, exspectata[f].rangae);
        CREDO_AEQUALIS_I32 (sanae, exspectata[f].sententiae);
        piscina_destruere(p);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "lectio", _probare_lectionem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "textus", _probare_textum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "spatium post", _probare_spatium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _probare_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "proprietates", _probare_proprietates, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "arbor", _probare_arborem, NIHIL, NIHIL, NIHIL, NIHIL },
    { "treebanks", _probare_fixuras, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("conllu", SECTIONES);
}
