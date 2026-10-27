/* probatio_functio.c - Probationes functionis (include/functio.h)
 *
 * Gradus diatonici (maior, minor cum elevatis), inversiones,
 * applicatae, radices deminutae, dyades, clausulae (perfecta,
 * imperfecta, dimidia, plagalis, deceptiva, nulla sine requie), linea
 * partiturae.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "carmen.h"
#include "partitura.h"
#include "explicatio.h"
#include "functio.h"
#include <stdio.h>
#include <string.h>

/* carmen partium datarum (NIHIL finit), 4/4 */
interior constans Carmen*
_carmen (
          CredoContextus*  c,
      constans character*  clavis,
      constans character** partes)
{
          character* s = (character*)piscina_allocare(c->piscina,
              M * IV);
             Carmina* liber;
      CarminisVitium  v;
                 i32  i;
                 i32  n;

    liber = (Carmina*)piscina_allocare(c->piscina, magnitudo(Carmina));
    n = (i32)sprintf(s, "<carmina><carmen titulus=\"f\" clavis=\"%s\">",
                     clavis);
    per (i = ZEPHYRUM; partes[i] != NIHIL; i++)
    {
        n += (i32)sprintf(s + n,
            "<pars titulus=\"p%u\" instrumentum=\"aether\">"
                     "<frustum mensura=\"1\"><textus!>%s</textus>"
                     "</frustum></pars>", i, partes[i]);
    }
    sprintf(s + n, "</carmen></carmina>");
    si (   !carmina_legere(chorda_ex_literis(s, c->piscina), c->piscina,
        liber,
                        &v) || liber->numerus < I)
    {
        imprimere("  carmen: %s (offset %u)\n", v.causa ? v.causa : "-",
                  v.offset);
        redde NIHIL;
    }
    redde &liber->carmina[ZEPHYRUM];
}

interior Functio*
_functio (
        CredoContextus* c,
       constans Carmen* k)
{
    Explicatio* e = (Explicatio*)piscina_allocare(c->piscina,
        magnitudo(Explicatio));
       Functio* f = (Functio*)piscina_allocare(c->piscina,
           magnitudo(Functio));

    si (   k == NIHIL || !explicatio_carminis(k, c->piscina, e)
        || !functio_carminis(k, e, c->piscina, f))
    {
        redde NIHIL;
    }
    redde f;
}

/* numeralis fenestrae primae carminis unius mensurae */
interior chorda
_numeralis (
          CredoContextus* c,
      constans character* clavis,
      constans character* a,
      constans character* b,
      constans character* d,
      constans character* e)
{
    constans character* partes[V];
               Functio* f;
                chorda  vacua;

    partes[0]      = a;
    partes[1]      = b;
    partes[2]      = d;
    partes[3]      = e;
    partes[4]      = NIHIL;
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    f              = _functio(c, _carmen(c, clavis, partes));
    si (f == NIHIL || f->numerus < I)
    {
        redde vacua;
    }
    redde chorda_ex_literis(f->gradus[ZEPHYRUM].numeralis, c->piscina);
}

/* gradus per mensuras (fenestra una per mensuram) */
interior b32
_progressio (
          CredoContextus* c,
     constans character* clavis,
     constans character* bassus,
     constans character* media,
     constans character* melodia,
    constans character* constans* sperati)
{
    constans character* partes[IV];
               Functio* f;
                   i32  i;

    partes[0]  = bassus;
    partes[1]  = media;
    partes[2]  = melodia;
    partes[3]  = NIHIL;
    f          = _functio(c, _carmen(c, clavis, partes));
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; sperati[i] != NIHIL; i++)
    {
        si (   i >= f->numerus
            || strcmp(f->gradus[i].numeralis, sperati[i]))
        {
            imprimere("  fenestra %u: '%s' (sperabatur '%s')\n", i,
                      i < f->numerus ? f->gradus[i].numeralis : "-",
                      sperati[i]);
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_sectio_diatonici (
    CredoContextus* c)
{
    hic_manens constans character* constans MAIOR_[] = {
        "I", "ii", "iii", "IV", "V", "vi", "viio", NIHIL
    };
    hic_manens constans character* constans MINOR_[] = {
        "i", "iv", "V", "viio", "III", "VI", "VII", NIHIL
    };

    CREDO_VERUM (_progressio(c, "c maior",
        "o3 c1 | d1 | e1 | f1 | g1 | a1 | b1",
        "o4 e1 | f1 | g1 | a1 | b1 | >c1 | d1",
        "o5 g1 | a1 | b1 | >c1 | d1 | e1 | f1", MAIOR_));
    /* minor: VI elevatus gradus suus (vio, non bVII) */
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "a minor", "o3 f#1",
        "o4 a1", "o5 c1", NIHIL), "vio");
    CREDO_VERUM (_progressio(c, "a minor",
        "o2 a1 | o3 d1 | o2 e1 | g#1 | o3 c1 | o2 f1 | g1",
        "o4 c1 | f1 | g#1 | b1 | e1 | a1 | b1",
        "o4 e1 | a1 | b1 | >d1 | <g1 | >c1 | d1", MINOR_));
}

interior vacuum
_sectio_conversiones (
    CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 e1",
        "o4 g1", "o5 c1", NIHIL), "I6");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 g1",
        "o4 c1", "o5 e1", NIHIL), "I64");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o2 g1",
        "o4 b1", "o4 d1", "o5 f1"), "V7");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o2 b1",
        "o4 d1", "o4 f1", "o5 g1"), "V65");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 d1",
        "o4 f1", "o4 g1", "o4 b1"), "V43");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 f1",
        "o4 g1", "o4 b1", "o5 d1"), "V42");
    /* septima minor in ii, dimidia diminuta in vii, maior in I */
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 d1",
        "o4 f1", "o4 a1", "o5 c1"), "ii7");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o2 b1",
        "o4 d1", "o4 f1", "o4 a1"), "viih7");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 c1",
        "o4 e1", "o4 g1", "o4 b1"), "IM7");
    /* tertia minor in basso; diminuta 6 et 64 */
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 f1",
        "o4 a1", "o5 d1", NIHIL), "ii6");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 d1",
        "o4 f1", "o4 b1", NIHIL), "viio6");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 f1",
        "o4 b1", "o5 d1", NIHIL), "viio64");
}

interior vacuum
_sectio_applicatae (
    CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 d1",
        "o4 f#1", "o4 a1", NIHIL), "V/V");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 d1",
        "o4 f#1", "o4 a1", "o5 c1"), "V7/V");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 e1",
        "o4 g#1", "o4 b1", NIHIL), "V/vi");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 a1",
        "o4 c#1", "o4 e1", NIHIL), "V/ii");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 f#1",
        "o4 a1", "o5 c1", "o5 e-1"), "viio7/V");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 c1",
        "o4 e1", "o4 b-1", NIHIL), "V7/IV");
    /* septima diminuta tonicae (Ab extra clavem): viio7, non viio7/I */
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o2 b1",
        "o4 d1", "o4 f1", "o4 a-1"), "viio7");
    /* B maior in C: V/iii; F# maior: meta B diminuta - non applicata */
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 b1",
        "o4 d#1", "o4 f#1", NIHIL), "V/iii");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 f#1",
        "o4 a#1", "o5 c#1", NIHIL), "bV");
}

interior vacuum
_sectio_deminutae (
    CredoContextus* c)
{
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o2 b-1",
        "o4 d1", "o4 f1", NIHIL), "bVII");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o2 a-1",
        "o4 c1", "o4 e-1", NIHIL), "bVI");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 f1",
        "o4 a-1", "o4 d-1", NIHIL), "bII6");
    /* minor: Bb in a minor = Neapolitana */
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "a minor", "o3 d1",
        "o4 f1", "o4 b-1", NIHIL), "bII6");
}

interior vacuum
_sectio_binae (
    CredoContextus* c)
{
    constans character* partes[III];
               Functio* f;

    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 c1",
        "o4 e1", NIHIL, NIHIL), "I(3)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 d1",
        "o4 f1", NIHIL, NIHIL), "ii(3)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 c1",
        "o4 g1", NIHIL, NIHIL), "I(5)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 c1",
        "o4 c1", NIHIL, NIHIL), "(I)");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 c1",
        "o4 c#1", "o4 d1", NIHIL), "?");
    partes[0]  = "o3 c1 | r1 | c1";
    partes[1]  = "o4 e1 | r1 | e1";
    partes[2]  = NIHIL;
    f          = _functio(c, _carmen(c, "c maior", partes));
    CREDO_NECESSE (f != NIHIL && f->numerus == III);
    CREDO_LITERAE_AEQUALES (f->gradus[I].numeralis, "-");
}

/* clausula fenestrae secundae (mensura 2) */
interior FunctioClausula
_clausula (
          CredoContextus* c,
      constans character* clavis,
      constans character* bassus,
      constans character* media,
      constans character* melodia)
{
    constans character* partes[IV];
               Functio* f;

    partes[0]  = bassus;
    partes[1]  = media;
    partes[2]  = melodia;
    partes[3]  = NIHIL;
    f          = _functio(c, _carmen(c, clavis, partes));
    si (f == NIHIL || f->numerus < II)
    {
        redde (FunctioClausula)XCIX;
    }
    redde f->gradus[I].clausula;
}

interior vacuum
_sectio_clausulae (
    CredoContextus* c)
{
    /* V7 -> I radicales, summa C: perfecta */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 g1 | o3 c1",
        "o4 f1 | e1", "o4 b1 | >c1"), (i32)FUNCTIO_PERFECTA);
    /* summa E: imperfecta; V6 -> I: imperfecta */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 g1 | o3 c1",
        "o4 d1 | c1", "o4 b1 | >e1"), (i32)FUNCTIO_IMPERFECTA);
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 b1 | o3 c1",
        "o4 d1 | e1", "o4 g1 | g1"), (i32)FUNCTIO_IMPERFECTA);
    /* V7 -> I6, summa tonica: imperfecta (I non radicalis) */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 g1 | o3 e1",
        "o4 f1 | g1", "o4 b1 | >c1"), (i32)FUNCTIO_IMPERFECTA);
    /* V6 -> I, summa tonica: imperfecta (V non radicalis) */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 b1 | o3 c1",
        "o4 d1 | e1", "o4 g1 | >c1"), (i32)FUNCTIO_IMPERFECTA);
    /* viio -> I: imperfecta */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 b1 | o3 c1",
        "o4 d1 | e1", "o4 f1 | e1"), (i32)FUNCTIO_IMPERFECTA);
    /* adventus dimidiam mensuram tantum implet: nulla */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior",
        "o2 g1 | o3 c2 <g2",
        "o4 f1 | e2 d2", "o4 b1 | >c1"), (i32)FUNCTIO_NULLA);
    /* I -> V: dimidia */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o3 c1 | <g1",
        "o4 e1 | d1", "o4 g1 | b1"), (i32)FUNCTIO_DIMIDIA);
    /* IV -> I: plagalis */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o3 f1 | c1",
        "o4 a1 | g1", "o5 c1 | c1"), (i32)FUNCTIO_PLAGALIS);
    /* V -> vi: deceptiva */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o3 g1 | a1",
        "o4 b1 | >c1", "o5 d1 | e1"), (i32)FUNCTIO_FALLAX);
    /* duae voces: V(3) -> unisonus tonicae - perfecta */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o3 g1 | c1",
        "o4 b1 | c1", NIHIL), (i32)FUNCTIO_PERFECTA);
    /* minor: V -> i, summa A */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "a minor", "o2 e1 | a1",
        "o4 g#1 | >c1", "o4 b1 | >a1"), (i32)FUNCTIO_PERFECTA);
    /* sine requie: nota summa ultima octava - nulla */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 g1 | o3 c1",
        "o4 f1 | e1", "o4 b1 | >c2 e4 g8 >c8"), (i32)FUNCTIO_NULLA);
    /* requies per pausam: pars summa tacet - perfecta */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 g1 | o3 c1",
        "o4 f1 | e1", "o4 b1 | >c2 r2"), (i32)FUNCTIO_PERFECTA);
    /* nota nova in tempore III (4/4: post dimidium) - nulla; in
     * dimidio ipso licet */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 g1 | o3 c1",
        "o4 f1 | e1", "o4 b1 | >c2. d4"), (i32)FUNCTIO_NULLA);
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 g1 | o3 c1",
        "o4 f1 | e1", "o4 b1 | >c2 e2"), (i32)FUNCTIO_PERFECTA);
    /* adventus non in initio mensurae: nulla */
    CREDO_AEQUALIS_I32 ((i32)_clausula(c, "c maior", "o2 g2 o3 c2",
        "o4 f2 e2", "o4 b2 >c2"), (i32)FUNCTIO_NULLA);
    CREDO_LITERAE_AEQUALES (functio_clausulae_titulus(FUNCTIO_PERFECTA),
                            "perfecta");
    CREDO_VERUM (functio_clausulae_titulus(FUNCTIO_NULLA) == NIHIL);
}

interior vacuum
_sectio_linea (
    CredoContextus* c)
{
    constans character* partes[IV];
       constans Carmen* k;
               Functio* f;
        PartituraForma  forma;
     ChordaAedificator* a;
                chorda  t;

    partes[0]  = "o2 g1 | o3 c1";
    partes[1]  = "o4 f1 | e1";
    partes[2]  = "o4 b1 | >c1";
    partes[3]  = NIHIL;
    k          = _carmen(c, "c maior", partes);
    f          = _functio(c, k);
    CREDO_NECESSE (f != NIHIL);
    memset(&forma, ZEPHYRUM, magnitudo(forma));
    forma.additae[ZEPHYRUM].titulus = "functio";
    forma.additae[ZEPHYRUM].scribere = functio_lineam_scribere;
    forma.additae[ZEPHYRUM].ctx = f;
    a = chorda_aedificator_creare(c->piscina, M);
    CREDO_NECESSE (a != NIHIL && partitura_scribere(k, &forma, a));
    t = chorda_aedificator_finire(a);
    CREDO_CHORDA_CONTINET (t, chorda_ex_literis("  functio  V7:1\n",
        c->piscina));
    CREDO_CHORDA_CONTINET (t, chorda_ex_literis(
        "  functio  I(3):1 [perf.]\n", c->piscina));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "diatonici", _sectio_diatonici, NIHIL, NIHIL, NIHIL, NIHIL },
    { "conversiones", _sectio_conversiones, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "applicatae", _sectio_applicatae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "deminutae", _sectio_deminutae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "binae", _sectio_binae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausulae", _sectio_clausulae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "linea", _sectio_linea, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("functio", SECTIONES);
}
