/* probatio_gabc_carmen.c - conversor gabc -> carmen
 * (include/gabc_carmen.h, plan T7)
 *
 * Quaeque probatio gabc brevem (in textu) convertit et textum SEMPER
 * per carmina_legere relegit ("textus manet veritas"); notae ex carmine
 * relecto asseruntur. Acceptio: salve_2435.gabc forma ordinaria = salve
 * libri (carmina.stml) nota pro nota (initium, longitudo, nota,
 * littera, syllaba), incisa totidem; clavis consulto differt (f ionius,
 * spec par. III).
 */

#include "latina.h"
#include "credo.h"
#include "gabc.h"
#include "gabc_carmen.h"
#include "carmen.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

#define SEXTA  ((s64)(CMLX / IV))   /* pulsus sextae decimae */

nomen structura {
                  b32 successus;
               chorda textus;       /* commentarium + carmen */
    GabcCarmenRelatio relatio;
              Carmina liber;        /* textus relectus */
                  b32 lectum;
} Conversio;

interior Conversio
_convertere_datum (
              CredoContextus* c,
                      chorda  datum,
    constans GabcCarmenForma* forma)
{
          Conversio  v;
         GabcCantus* g;
    GabcCarmenForma  f;
  ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
      MMMCMXCIX);
  ChordaAedificator* b = chorda_aedificator_creare(c->piscina,
      MMMCMXCIX);
     CarminisVitium vitium;

    memset(&v, ZEPHYRUM, magnitudo(v));
    si (forma != NIHIL)
    {
        f = *forma;
    }
    alioquin
    {
        gabc_carmen_formam_ordinariam(&f);
    }
    g = gabc_legere(datum, c->piscina);
    v.successus = g != NIHIL && gabc_carmen_scribere(g, &f, c->piscina,
        a, &v.relatio);
    v.textus = chorda_aedificator_finire(a);
    si (v.successus)
    {
        chorda_aedificator_appendere_literis(b, "<carmina>\n");
        chorda_aedificator_appendere_chorda(b, v.textus);
        chorda_aedificator_appendere_literis(b, "</carmina>\n");
        v.lectum = carmina_legere(chorda_aedificator_finire(b),
            c->piscina, &v.liber, &vitium);
        si (!v.lectum)
        {
            imprimere("  carmina_legere: %s (offset %u)\n%.*s\n",
                vitium.causa != NIHIL ? vitium.causa : "?",
                (insignatus integer)vitium.offset,
                (integer)v.textus.mensura,
                (constans character*)v.textus.datum);
        }
    }
    redde v;
}

interior Conversio
_convertere (
              CredoContextus* c,
          constans character* gabc,
    constans GabcCarmenForma* forma)
{
    redde _convertere_datum(c, chorda_ex_literis(gabc, c->piscina),
        forma);
}

interior GabcCarmenForma
_forma_do (
    s32 do_fixum)
{
    GabcCarmenForma f;

    gabc_carmen_formam_ordinariam(&f);
    f.do_fixum = do_fixum;
    redde f;
}

interior b32
_continet (
         CredoContextus* c,
                 chorda  textus,
     constans character* acus)
{
    character* s = (character*)piscina_allocare(c->piscina,
        (memoriae_index)(textus.mensura + I));

    si (s == NIHIL)
    {
        redde FALSUM;
    }
    memcpy(s, textus.datum, (size_t)textus.mensura);
    s[textus.mensura] = '\0';
    si (strstr(s, acus) == NIHIL)
    {
        imprimere("  deest \"%s\" in:\n%s\n", acus, s);
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_abest (
         CredoContextus* c,
                 chorda  textus,
     constans character* acus)
{
    character* s = (character*)piscina_allocare(c->piscina,
        (memoriae_index)(textus.mensura + I));

    si (s == NIHIL)
    {
        redde FALSUM;
    }
    memcpy(s, textus.datum, (size_t)textus.mensura);
    s[textus.mensura] = '\0';
    redde strstr(s, acus) == NIHIL;
}

/* nota index partis primae; pulsus in carmine */
interior constans CarmenNota*
_nota (
    constans Carmen* k,
                i32  index,
                s64* pulsus)
{
    i32 f;

    per (f = ZEPHYRUM; f < k->partes[ZEPHYRUM].numerus_frustorum; f++)
    {
        constans CarmenFrustum* fr = &k->partes[ZEPHYRUM].frusta[f];

        si (index < fr->numerus)
        {
            *pulsus = fr->pulsus + fr->notae[index].pulsus;
            redde &fr->notae[index];
        }
        index -= fr->numerus;
    }
    redde NIHIL;
}

interior i32
_notae (
    constans Carmen* k)
{
    i32 f;
    i32 n = ZEPHYRUM;

    per (f = ZEPHYRUM; f < k->partes[ZEPHYRUM].numerus_frustorum; f++)
    {
        n += k->partes[ZEPHYRUM].frusta[f].numerus;
    }
    redde n;
}

interior i32
_lineae (
    constans Carmen* k)
{
    i32 f;
    i32 n = ZEPHYRUM;

    per (f = ZEPHYRUM; f < k->partes[ZEPHYRUM].numerus_frustorum; f++)
    {
        n += k->partes[ZEPHYRUM].frusta[f].numerus_linearum;
    }
    redde n;
}

interior i32
_approximationes (
     constans GabcCarmenRelatio* r,
          GabcApproximatioGenus  genus)
{
    i32 i;
    i32 n = ZEPHYRUM;

    per (i = ZEPHYRUM; i < r->numerus_approximationum; i++)
    {
        si (r->approximationes[i].genus == genus)
        {
            n++;
        }
    }
    redde n;
}

/* longitudines (sextae decimae) et spiritus post notam (sextae decimae,
 * 0 = nullus) -> initia et longitudines notarum carminis relecti */
interior vacuum
_rhythmum_asserere (
    constans Carmen* k,
         constans s32* longitudines,
         constans s32* spiritus,
                  i32  numerus)
{
    i32 i;
    s64 initium = ZEPHYRUM;

    CREDO_AEQUALIS_I32 (_notae(k), numerus);
    si (_notae(k) != numerus)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
                        s64  p = -I;
        constans CarmenNota* n = _nota(k, i, &p);

        CREDO_AEQUALIS_S64 (p, initium);
        CREDO_AEQUALIS_S64 (n->longitudo, (s64)longitudines[i] * SEXTA);
        initium += (s64)(longitudines[i] + spiritus[i]) * SEXTA;
    }
}


/* ==================================================
 * Rhythmus (G8)
 * ================================================== */

interior vacuum
_rhythmum_probare (
    CredoContextus* c)
{
    /* g; g. mora; g_ episema; g._ (episema cum mora: signum); fgwh
     * (nota ante quilisma III); divisiones , ; : ; hg.. (puncta ambarum);
     * g_g unisonus ligatus 3+2 -> '&'; :: finalis sine spiritu */
    hic_manens constans s32 LONGAE[] = { 2, 4, 3, 4, 3, 2, 2, 2, 2, 4,
        4, 5 };
    hic_manens constans s32 SPIRITUS[] = { 0, 0, 0, 0, 0, 0, 1, 1, 2, 0,
        0, 0 };
    GabcCarmenForma f = _forma_do(LX);
          Conversio v = _convertere(c, "%%\n(c4) a(g) b(g.) c(g_) "
              "d(g._) e(fgwh) (,) f(h) (;) g(h) (:) h(hg..) i(g_g) (::)",
              &f);

    CREDO_NECESSE (v.successus && v.lectum);
    _rhythmum_asserere(&v.liber.carmina[ZEPHYRUM], LONGAE, SPIRITUS,
        XII);
    CREDO_AEQUALIS_I32 (_lineae(&v.liber.carmina[ZEPHYRUM]), III);
    CREDO_VERUM (_continet(c, v.textus, "g4 & g16</textus>"));
    CREDO_VERUM (_continet(c, v.textus,
        "a8 r16 | a8 r16 | a8 r8 | a4"));
    CREDO_VERUM (_continet(c, v.textus, "metrum=\"liberum\""));
    CREDO_AEQUALIS_I32 (v.relatio.signa[GABC_CARMEN_EPISEMA_CUM_MORA],
        I);
    CREDO_AEQUALIS_I32 (v.relatio.numerus_approximationum, ZEPHYRUM);
}

/* Review Focus 5: unisoni et quilisma ad margines */
interior vacuum
_rhythmum_marginum_probare (
    CredoContextus* c)
{
    GabcCarmenForma f = _forma_do(LX);
          Conversio v;

    /* trans neumas intra syllabam ligati; trans syllabam numquam */
    {
        hic_manens constans s32 LONGAE[]    = { 2, 4, 2, 2 };
        hic_manens constans s32 SPIRITUS[]  = { 0, 0, 0, 0 };

        v = _convertere(c, "%%\n(c4) a(gh/hg) b(g)", &f);
        CREDO_NECESSE (v.successus && v.lectum);
        _rhythmum_asserere(&v.liber.carmina[ZEPHYRUM], LONGAE, SPIRITUS,
            IV);
    }
    /* divisio intra syllabam secat (monitum, non vitium) */
    {
        hic_manens constans s32 LONGAE[]    = { 2, 2 };
        hic_manens constans s32 SPIRITUS[]  = { 1, 0 };

        v = _convertere(c, "%%\n(c4) a(g;g)", &f);
        CREDO_NECESSE (v.successus && v.lectum);
        _rhythmum_asserere(&v.liber.carmina[ZEPHYRUM], LONGAE, SPIRITUS,
            II);
    }
    /* quilisma prima syllabae nihil longat */
    {
        hic_manens constans s32 LONGAE[]    = { 2, 2, 2 };
        hic_manens constans s32 SPIRITUS[]  = { 0, 0, 0 };

        v = _convertere(c, "%%\n(c4) a(g) b(gwh)", &f);
        CREDO_NECESSE (v.successus && v.lectum);
        _rhythmum_asserere(&v.liber.carmina[ZEPHYRUM], LONGAE, SPIRITUS,
            III);
    }
    /* divisio ante notam primam nihil; divisiones contiguae unum finem,
     * spiritu longissimo */
    {
        hic_manens constans s32 LONGAE[]    = { 2, 2 };
        hic_manens constans s32 SPIRITUS[]  = { 2, 0 };

        v = _convertere(c, "%%\n(c4) (;) a(g) (;) (:) b(g) (::)", &f);
        CREDO_NECESSE (v.successus && v.lectum);
        _rhythmum_asserere(&v.liber.carmina[ZEPHYRUM], LONGAE, SPIRITUS,
            II);
        CREDO_AEQUALIS_I32 (_lineae(&v.liber.carmina[ZEPHYRUM]), I);
        CREDO_VERUM (_continet(c, v.textus, "<textus!>o3 g8 r8 | g8<"));
    }
}


/* ==================================================
 * Altitudo (G9)
 * ================================================== */

interior vacuum
_altitudinem_probare (
    CredoContextus* c)
{
    GabcCarmenForma  f;
          Conversio  v;
                s64  p;
    constans Carmen* k;

    /* ordinarie: nota altissima = F4 (LXV) */
    v = _convertere(c, "%%\n(c4) a(gj)", NIHIL);
    CREDO_NECESSE (v.successus && v.lectum);
    k = &v.liber.carmina[ZEPHYRUM];
    CREDO_AEQUALIS_I32 (_nota(k, ZEPHYRUM, &p)->nota, LX);
    CREDO_AEQUALIS_I32 (_nota(k, I, &p)->nota, LXV);
    CREDO_AEQUALIS_I32 ((i32)v.relatio.do_midi, LXV);
    CREDO_VERUM (_continet(c, v.textus, "do = F4 -->"));

    /* do_fixum vincit */
    f = _forma_do(LX);
    v = _convertere(c, "%%\n(c4) a(gj)", &f);
    CREDO_NECESSE (v.successus && v.lectum);
    k = &v.liber.carmina[ZEPHYRUM];
    CREDO_AEQUALIS_I32 (_nota(k, ZEPHYRUM, &p)->nota, LV);
    CREDO_AEQUALIS_I32 (_nota(k, I, &p)->nota, LX);

    /* x bemolle: semitonium infra */
    v = _convertere(c, "%%\n(c4) a(hixij)", &f);
    CREDO_NECESSE (v.successus && v.lectum);
    k = &v.liber.carmina[ZEPHYRUM];
    CREDO_AEQUALIS_I32 (_nota(k, ZEPHYRUM, &p)->nota, LVII);
    CREDO_AEQUALIS_I32 (_nota(k, I, &p)->nota, LVIII);
    CREDO_AEQUALIS_I32 (_nota(k, II, &p)->nota, LX);
    CREDO_VERUM (_continet(c, v.textus, "a8 b-8 o4 c8"));

    /* cb3: si bemolle per systema */
    v = _convertere(c, "%%\n(cb3) a(gh)", &f);
    CREDO_NECESSE (v.successus && v.lectum);
    k = &v.liber.carmina[ZEPHYRUM];
    CREDO_AEQUALIS_I32 (_nota(k, ZEPHYRUM, &p)->nota, LVIII);
    CREDO_AEQUALIS_I32 (_nota(k, I, &p)->nota, LX);

    /* do = Gb4: fa = Cb5, octava litterae scriptae ('o5 c-' = LXXI);
     * finalis fa -> lydius super Cb */
    f = _forma_do(LXVI);
    v = _convertere(c, "%%\n(c4) a(jm)", &f);
    CREDO_NECESSE (v.successus && v.lectum);
    k = &v.liber.carmina[ZEPHYRUM];
    CREDO_AEQUALIS_I32 (_nota(k, ZEPHYRUM, &p)->nota, LXVI);
    CREDO_AEQUALIS_I32 (_nota(k, I, &p)->nota, LXXI);
    CREDO_VERUM (_nota(k, I, &p)->littera == 'c');
    CREDO_VERUM (_continet(c, v.textus, "o4 g-8 o5 c-8"));
    CREDO_VERUM (_continet(c, v.textus, "clavis=\"cb lydius\""));

    /* extra o0..o8: octava mota (approximatio) */
    f = _forma_do(CXXV);
    v = _convertere(c, "%%\n(c4) a(j)", &f);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (_nota(&v.liber.carmina[ZEPHYRUM], ZEPHYRUM,
        &p)->nota, CXIII);
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        GABC_CARMEN_EXTRA_AMBITUM), I);
}


/* ==================================================
 * Verba (Review Focus 3)
 * ================================================== */

interior vacuum
_verba_variae_probare (
    CredoContextus* c)
{
    Conversio v;

    /* casus primae litterae, accentus, ae ligata, interpunctio, '*()'
     * sine notis, syllaba sine textu (melisma), '{}', elisio, R/ */
    v = _convertere(c,
        "%%\n(c4)SAL(g)ve,(h) *() Ma(g)r\xc3\xad(gh)a.(g) "
        "c\xc3\xa6(g)(gh) Al{le}(g)l\xc3\xba(g)ia(g) a<e>u</e>(g) "
        "<sp>R/</sp>.(g)", NIHIL);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(c, v.textus,
        "<verba!>Sal-ve Ma-ri _-a cae _ _ "
        "Alle-lu-ia a _</verba>"));
    CREDO_AEQUALIS_I32 (v.relatio.numerus_approximationum, ZEPHYRUM);

    /* littera non Latina: approximatio, syllaba -> melisma */
    v = _convertere(c, "%%\n(c4)\xce\x9a\xcf\x8d(g)ri(g)e(g)", NIHIL);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(c, v.textus, "<verba!>_-ri-e</verba>"));
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        GABC_CARMEN_LITTERA_IGNOTA), I);

    /* accentus compositus (U+0301 post vocalem, T9): tacite tollitur */
    v = _convertere(c, "%%\n(c4)Ma(g)ri\xcc\x81(gh)a(g)", NIHIL);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(c, v.textus, "<verba!>Ma-ri _-a</verba>"));
    CREDO_AEQUALIS_I32 (v.relatio.numerus_approximationum, ZEPHYRUM);

    /* notae ante textum primum */
    v = _convertere(c, "%%\n(c4)(g) a(h)", NIHIL);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(c, v.textus, "<verba!>_ a</verba>"));

    /* nullus textus: verba nulla */
    v = _convertere(c, "%%\n(c4)(g)(h)", NIHIL);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_abest(c, v.textus, "<verba!>"));
}


/* ==================================================
 * Clavis
 * ================================================== */

interior vacuum
_clavem_probare (
    CredoContextus* c)
{
    Conversio v;

    /* finalis re, collectio do: do = Eb4 -> "f dorius" */
    v = _convertere(c, "%%\n(c4) a(jk) b(k)", NIHIL);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(c, v.textus, "clavis=\"f dorius\""));
    CREDO_AEQUALIS_I32 (v.relatio.numerus_approximationum, ZEPHYRUM);

    /* si bemolle in plus quam dimidia parte: collectio fa -> aeolius */
    v = _convertere(c, "%%\n(c4) a(kixik) b(k)", NIHIL);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(c, v.textus, "clavis=\"f aeolius\""));

    /* finalis si: modus nullus -> do maior + approximatio */
    v = _convertere(c, "%%\n(c4) a(ji)", NIHIL);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(c, v.textus, "clavis=\"f maior\""));
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        GABC_CARMEN_CLAVIS_SINE_MODO), I);

    /* finalis alterata extra collectionem */
    v = _convertere(c, "%%\n(c4) a(j) b(hxh)", NIHIL);
    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(c, v.textus, "clavis=\"f maior\""));
    CREDO_AEQUALIS_I32 (_approximationes(&v.relatio,
        GABC_CARMEN_CLAVIS_SINE_MODO), I);
}


/* ==================================================
 * Nexus (G11)
 * ================================================== */

interior vacuum
_nexus_probare (
    CredoContextus* c)
{
    hic_manens constans s64 INITIA[] = { 0, 480, 1440, 2160 };
    hic_manens constans s64 LONGITUDINES[] = { 480, 960, 480, 960 };
    hic_manens constans i32 NOTAE[] = { 0, 1, 3, 4 };
    hic_manens constans i32 NUMERI[] = { 1, 2, 1, 1 };
    hic_manens constans i32 SYLLABAE[] = { 1, 1, 1, 3 };
    hic_manens constans i32 ELEMENTA[] = { 0, 0, 2, 0 };
    hic_manens constans i32 COLUMNAE[] = { 8, 9, 12, 21 };
    Conversio v = _convertere(c, "%%\n(c4) a(gh/hg) (;) b(g.)", NIHIL);
          i32 i;

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_NECESSE (v.relatio.numerus_nexuum == IV);
    per (i = ZEPHYRUM; i < IV; i++)
    {
         constans GabcNexus* x = &v.relatio.nexus[i];
                        s64  p = -I;

        CREDO_AEQUALIS_S64 (x->initium, INITIA[i]);
        CREDO_AEQUALIS_S64 (x->longitudo, LONGITUDINES[i]);
        CREDO_AEQUALIS_I32 (x->nota, NOTAE[i]);
        CREDO_AEQUALIS_I32 (x->numerus, NUMERI[i]);
        CREDO_AEQUALIS_I32 (x->syllaba, SYLLABAE[i]);
        CREDO_AEQUALIS_I32 (x->elementum, ELEMENTA[i]);
        CREDO_AEQUALIS_I32 (x->linea, II);
        CREDO_AEQUALIS_I32 (x->columna, COLUMNAE[i]);
        /* idem initium ac nota carminis relecti */
        CREDO_VERUM (_nota(&v.liber.carmina[ZEPHYRUM], i, &p) != NIHIL);
        CREDO_AEQUALIS_S64 (p, x->initium);
    }
}


/* ==================================================
 * Strictus et recusationes
 * ================================================== */

interior vacuum
_strictum_probare (
    CredoContextus* c)
{
          GabcCarmenForma  f;
        GabcCarmenRelatio  r;
        ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
            LXIV);
               GabcCantus* g;
                Conversio  v;

    /* strictus + finalis si: FALSUM, exitus intactus, relatio nominat */
    gabc_carmen_formam_ordinariam(&f);
    f.strictus = VERUM;
    g = gabc_legere(chorda_ex_literis("%%\n(c4) a(ji)", c->piscina),
        c->piscina);
    CREDO_NECESSE (g != NIHIL);
    chorda_aedificator_appendere_literis(a, "X");
    CREDO_FALSUM (gabc_carmen_scribere(g, &f, c->piscina, a, &r));
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_aedificator_finire(a), "X");
    CREDO_AEQUALIS_I32 (r.numerus_approximationum, I);
    CREDO_AEQUALIS_I32 ((i32)r.approximationes[ZEPHYRUM].genus,
        (i32)GABC_CARMEN_CLAVIS_SINE_MODO);

    /* sine stricto: scriptum, approximatio servata */
    v = _convertere(c, "%%\n(c4) a(ji)", NIHIL);
    CREDO_VERUM (v.successus && v.lectum);
    CREDO_AEQUALIS_I32 (v.relatio.numerus_approximationum, I);

    /* cantus cum vitio, cantus sine notis: FALSUM */
    v = _convertere(c, "%%\n(c4) a(g", NIHIL);
    CREDO_FALSUM (v.successus);
    CREDO_AEQUALIS_I32 ((i32)v.textus.mensura, ZEPHYRUM);
    v = _convertere(c, "%%\n(c4) a() b(::)", NIHIL);
    CREDO_FALSUM (v.successus);
    CREDO_AEQUALIS_I32 ((i32)v.relatio.numerus_nexuum, ZEPHYRUM);
}

interior vacuum
_relatio_scripta_probare (
    CredoContextus* c)
{
            Conversio  v = _convertere(c, "%%\n(c4) a(j'i)", NIHIL);
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina, CCLVI);

    CREDO_NECESSE (v.successus);
    CREDO_VERUM (gabc_relationem_scribere(&v.relatio, "x.gabc", a));
    CREDO_VERUM (_continet(c, chorda_aedificator_finire(a),
        "x.gabc:2:10: clavis sine modo - finalis in si"));
    CREDO_VERUM (_continet(c, chorda_aedificator_finire(a),
        "x.gabc: signa non conversa: ictus 1\n"));
    CREDO_VERUM (_continet(c, chorda_aedificator_finire(a),
        "x.gabc: nota structurae: do = F4 (nota altissima in summo)\n"));
}

/* fragmenta Salve (omnia praefixa): numquam ruina; aut FALSUM aut
 * carmen quod carmina_legere accipit */
interior vacuum
_totalitatem_probare (
    CredoContextus* c)
{
    chorda salve = filum_legere_totum("musica/fontes/salve_2435.gabc",
                        c->piscina);
       i32 n;
       i32 fracti   = ZEPHYRUM;
       i32 scripti  = ZEPHYRUM;

    CREDO_NECESSE (salve.mensura > ZEPHYRUM);
    per (n = ZEPHYRUM; n <= salve.mensura; n++)
    {
        Piscina* p = piscina_generare_dynamicum("gabc_carmen_totalitas",
                         MMMCMXCIX);
             chorda frustum;
          Conversio v;
     CredoContextus loc = *c;

        si (p == NIHIL)
        {
            frange;
        }
        loc.piscina      = p;
        frustum.datum    = salve.datum;
        frustum.mensura  = n;
        v                = _convertere_datum(&loc, frustum, NIHIL);
        si (v.successus)
        {
            scripti++;
            si (!v.lectum)
            {
                fracti++;
            }
        }
        piscina_destruere(p);
    }
    imprimere("  gabc_carmen totalitas: praefixa %u, scripta %u, "
        "fracta %u\n", (insignatus integer)(salve.mensura + I),
        (insignatus integer)scripti, (insignatus integer)fracti);
    CREDO_AEQUALIS_I32 (fracti, ZEPHYRUM);
    CREDO_MAIOR_I32 (scripti, C);
}


/* ==================================================
 * ACCEPTIO: Salve
 * ================================================== */

interior vacuum
_salve_probare (
    CredoContextus* c)
{
         Conversio v = _convertere_datum(c, filum_legere_totum(
                           "musica/fontes/salve_2435.gabc", c->piscina),
                           NIHIL);
           Carmina  liber;
    CarminisVitium  vitium;
   constans Carmen* nos;
   constans Carmen* illud;
               i32  n;
               i32  k;

    CREDO_NECESSE (v.successus && v.lectum);
    CREDO_VERUM (_continet(c, v.textus, "clavis=\"f ionius\""));
    CREDO_NECESSE (carmina_ordinaria(c->piscina, &liber, &vitium));
    illud = carmina_invenire(&liber, chorda_ex_literis("salve",
        c->piscina));
    CREDO_NECESSE (illud != NIHIL);
    nos  = &v.liber.carmina[ZEPHYRUM];
    n    = _notae(nos);
    CREDO_AEQUALIS_I32 (n, _notae(illud));
    CREDO_AEQUALIS_I32 (_lineae(nos), _lineae(illud));
    per (k = ZEPHYRUM; k < n && k < _notae(illud); k++)
    {
                        s64  a = -I;
                        s64  b = -I;
        constans CarmenNota* x = _nota(nos, k, &a);
        constans CarmenNota* y = _nota(illud, k, &b);

        si (   a       != b || x->longitudo != y->longitudo
            || x->nota != y->nota || x->littera != y->littera
            || !chorda_aequalis(x->syllaba, y->syllaba))
        {
            imprimere("  salve nota %u: nostra %ld/%ld/%u/%.*s illius "
                "%ld/%ld/%u/%.*s\n", (insignatus integer)k, (longus)a,
                (longus)x->longitudo, (insignatus integer)x->nota,
                (integer)x->syllaba.mensura,
                (constans character*)x->syllaba.datum, (longus)b,
                (longus)y->longitudo, (insignatus integer)y->nota,
                (integer)y->syllaba.mensura,
                (constans character*)y->syllaba.datum);
            CREDO_VERUM (FALSUM);
            frange;
        }
    }
}

/* ACCEPTIO fixa (T10): liber nunc ex conversore venit, ergo parilitas
 * cum salve.py (deleto) contra copiam notarum eius fixam:
 * probationes/fixa/gabc/salve_py_notae.tsv (initium longitudo nota
 * littera syllaba; '#' commentarium, '-' melisma) */
interior vacuum
_salve_fixum_probare (
    CredoContextus* c)
{
         Conversio v = _convertere_datum(c, filum_legere_totum(
                           "musica/fontes/salve_2435.gabc", c->piscina),
                           NIHIL);
            chorda d = filum_legere_totum(
                           "probationes/fixa/gabc/salve_py_notae.tsv",
                           c->piscina);
   constans Carmen* nos;
               i32  i = ZEPHYRUM;
               i32  k = ZEPHYRUM;

    CREDO_NECESSE (v.successus && v.lectum && d.mensura > ZEPHYRUM);
    nos = &v.liber.carmina[ZEPHYRUM];
    dum (i < d.mensura)
    {
                        i32  initium = i;
                     longus  campi[III];
                  character  littera;
                  character  syllaba[XXXII];
                        s64  p = -I;
        constans CarmenNota* x;
                  character  linea[CXXVIII];
                        i32  n;

        dum (i < d.mensura && d.datum[i] != '\n')
        {
            i++;
        }
        n = i - initium;
        i++;
        si (n == ZEPHYRUM || d.datum[initium] == '#' || n >= CXXVIII)
        {
            perge;
        }
        memcpy(linea, d.datum + initium, (size_t)n);
        linea[n] = '\0';
        si (sscanf(linea, "%ld\t%ld\t%ld\t%c\t%31s", &campi[ZEPHYRUM],
                &campi[I], &campi[II], &littera, syllaba) != V)
        {
            CREDO_VERUM (FALSUM);
            frange;
        }
        x = _nota(nos, k, &p);
        si (   x               == NIHIL || p != (s64)campi[ZEPHYRUM]
            || x->longitudo    != (s64)campi[I]
            || (longus)x->nota != campi[II] || x->littera != littera
            || !chorda_aequalis_literis(x->syllaba,
                   strcmp(syllaba, "-") == ZEPHYRUM ? "" : syllaba))
        {
            imprimere("  salve fixa nota %u: %s\n",
                (insignatus integer)k,
                linea);
            CREDO_VERUM (FALSUM);
            frange;
        }
        k++;
    }
    CREDO_AEQUALIS_I32 (k, _notae(nos));
    CREDO_AEQUALIS_I32 (k, CXXXIX);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "rhythmus Solesmensis (G8)", _rhythmum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "rhythmus ad margines: unisoni, quilisma, divisiones",
        _rhythmum_marginum_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "altitudo (G9): summus, do fixum, bemolles, orthographia",
        _altitudinem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "verba variae", _verba_variae_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "clavis: finalis et collectio", _clavem_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "nexus (G11)", _nexus_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "strictus et recusationes", _strictum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "relatio scripta", _relatio_scripta_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "totalitas: praefixa Salve", _totalitatem_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "ACCEPTIO: Salve = salve libri", _salve_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "ACCEPTIO: Salve = notae salve.py fixae", _salve_fixum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("gabc_carmen", SECTIONES, NIHIL,
        NIHIL);
}
