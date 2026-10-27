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

/* carmen partium datarum (NIHIL finit), 4/4; mutatio: stml post
 * initium carminis (<clavis mensura=...>), NIHIL nulla */
interior constans Carmen*
_carmen_mutatum (
          CredoContextus*  c,
      constans character*  clavis,
      constans character*  mutatio,
      constans character** partes)
{
          character* s = (character*)piscina_allocare(c->piscina,
              M * IV);
             Carmina* liber;
      CarminisVitium  v;
                 i32  i;
                 i32  n;

    liber = (Carmina*)piscina_allocare(c->piscina, magnitudo(Carmina));
    n = (i32)sprintf(s,
        "<carmina><carmen titulus=\"f\" clavis=\"%s\">%s",
                     clavis, mutatio != NIHIL ? mutatio : "");
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

interior constans Carmen*
_carmen (
          CredoContextus*  c,
      constans character*  clavis,
      constans character** partes)
{
    redde _carmen_mutatum(c, clavis, NIHIL, partes);
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

/* functio_tertia_picardiae fenestrae II carminis trium vocum */
interior b32
_picardia_medio (
          CredoContextus* c,
      constans character* clavis,
      constans character* bassus,
      constans character* media,
      constans character* melodia)
{
    constans character* partes[IV];
       constans Carmen* k;
               Functio* f;

    partes[0]  = bassus;
    partes[1]  = media;
    partes[2]  = melodia;
    partes[3]  = NIHIL;
    k          = _carmen(c, clavis, partes);
    f          = _functio(c, k);
    si (f == NIHIL || f->explicatio->numerus_fenestrarum <= II)
    {
        imprimere("  picardia: functio nulla\n");
        redde FALSUM;
    }
    redde functio_tertia_picardiae(k, f->explicatio, II);
}

/* tertia Picardiae: A7 -> D in D minor = V7 -> I (perfecta); D maior
 * ultima sine dominante = I; medio D -> Gm = V/iv -> iv (non
 * Picardia); praedicatum per fenestras */
interior vacuum
_sectio_picardia (
    CredoContextus* c)
{
    hic_manens constans character* constans sine_dominante[] = {
        "i", "iv", "I", NIHIL };
    hic_manens constans character* constans applicata[] = {
        "V/iv", "iv", "V", "i", NIHIL };
    constans character* partes[V];
       constans Carmen* k;
               Functio* f;

    /* A7 -> D, voces IV (septima G -> F#, sensibilis C# -> D summa) */
    partes[0]  = "o2 a1 | o3 d1";
    partes[1]  = "o3 g1 | f#1";
    partes[2]  = "o3 e1 | a1";
    partes[3]  = "o4 c#1 | d1";
    partes[4]  = NIHIL;
    k          = _carmen(c, "d minor", partes);
    f          = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == II);
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(
        f->gradus[ZEPHYRUM].numeralis, c->piscina), "V7");
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(
        f->gradus[I].numeralis, c->piscina), "I");
    CREDO_AEQUALIS_I32 ((i32)f->gradus[I].clausula,
                        (i32)FUNCTIO_PERFECTA);
    CREDO_VERUM (functio_tertia_picardiae(k, f->explicatio, I));
    /* D maior ultima sine dominante: I */
    CREDO_VERUM (_progressio(c, "d minor", "o3 d1 | g1 | d1",
        "o4 f1 | b-1 | f#1", "o4 a1 | o5 d1 | o4 a1", sine_dominante));
    /* medio: D -> Gm = V/iv -> iv (non Picardia); ultima Dm */
    CREDO_VERUM (_progressio(c, "d minor", "o3 d1 | g1 | a1 | d1",
        "o4 f#1 | b-1 | c#1 | f1", "o4 a1 | o5 d1 | o4 e1 | a1",
        applicata));
    partes[0]  = "o3 d1 | g1 | a1 | d1";
    partes[1]  = "o4 f#1 | b-1 | c#1 | f#1";
    partes[2]  = "o4 a1 | o5 d1 | o4 e1 | a1";
    partes[3]  = NIHIL;
    k          = _carmen(c, "d minor", partes);
    f          = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == IV);
    CREDO_FALSUM (functio_tertia_picardiae(k, f->explicatio, ZEPHYRUM));
    CREDO_VERUM (functio_tertia_picardiae(k, f->explicatio, III));
    /* medio post dominantem, proxima V: Picardia */
    CREDO_VERUM (_picardia_medio(c, "d minor",
        "o3 d1 | a1 | d1 | a1 | d1",
        "o4 f1 | e1 | f#1 | e1 | f1", "o4 a1 | c#1 | a1 | c#1 | a1"));
    /* medio post iv (non dominantem): non Picardia */
    CREDO_FALSUM (_picardia_medio(c, "d minor",
        "o3 d1 | g1 | d1 | a1 | d1", "o4 f1 | b-1 | f#1 | e1 | f1",
        "o4 a1 | o5 d1 | o4 a1 | c#1 | a1"));
    /* medio post dominantem, proxima iv: V/iv, non Picardia */
    CREDO_FALSUM (_picardia_medio(c, "d minor",
        "o3 d1 | a1 | d1 | g1 | a1 | d1",
        "o4 f1 | e1 | f#1 | b-1 | e1 | f1",
        "o4 a1 | c#1 | a1 | o5 d1 | o4 c#1 | a1"));
    /* ultima D + F# sine quinta (tertia maior): Picardia */
    CREDO_VERUM (_picardia_medio(c, "d minor", "o3 d1 | a1 | d1",
        "o4 f1 | e1 | f#1", "o4 a1 | c#1 | o5 d1"));
    /* ultima Dm (tertia minor): non Picardia */
    CREDO_FALSUM (_picardia_medio(c, "d minor", "o3 d1 | a1 | d1",
        "o4 f1 | e1 | f1", "o4 a1 | c#1 | a1"));
    /* dimidia mensurae (non adventus): non Picardia */
    CREDO_FALSUM (_picardia_medio(c, "d minor", "o3 d2 a2 | d2 a2 | d1",
        "o4 f2 e2 | f#2 e2 | f1", "o4 a2 c#2 | a2 c#2 | a1"));
    /* clavis maior: numquam Picardia */
    partes[0]  = "o2 a1 | o3 d1";
    partes[1]  = "o3 g1 | f#1";
    partes[2]  = "o3 e1 | a1";
    partes[3]  = "o4 c#1 | d1";
    partes[4]  = NIHIL;
    k          = _carmen(c, "d maior", partes);
    f          = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == II);
    CREDO_FALSUM (functio_tertia_picardiae(k, f->explicatio, I));
}

/* vocabularium additum: numeralia (add6 non 6 - figura inversionis),
 * nonae dominantes, finis in Iadd6; figura nulla tonis additis */
interior vacuum
_sectio_vocabularium (
    CredoContextus* c)
{
    constans character* nona_dominans[] = {
        "o2 g1 | o3 c1", "o3 b1 | >c1", "o4 f1 | e1", "o4 a1 | >c1",
        NIHIL
    };
    constans character* clausula_sextae[] = {
        "o2 g1 | o3 c1", "o3 b1 | o4 e1", "o4 d1 | g1", "o4 f1 | a1",
        "o5 d1 | c1", NIHIL
    };
    Functio* f;

    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 c1",
        "o4 e1", "o4 g1", "o4 a1"), "Iadd6");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 c1",
        "o4 e1", "o4 g1", "o5 d1"), "Iadd9");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 e1",
        "o4 g1", "o5 c1", "o5 d1"), "Iadd9");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o2 g1",
        "o4 b1", "o5 f1", "o5 a1"), "V9");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "a minor", "o2 e1",
        "o4 g#1", "o5 d1", "o5 f1"), "V7b9");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o2 g1",
        "o4 c1", "o4 d1", "o4 f1"), "V7sus4");
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o2 g1",
        "o4 b1", "o5 d#1", "o5 f1"), "V+7");
    /* nona dominans applicata: D9 in C = V9/V */
    CREDO_CHORDA_AEQUALIS_LITERIS (_numeralis(c, "c maior", "o3 d1",
        "o4 f#1", "o5 c1", "o5 e1"), "V9/V");
    /* V9 -> I, V7 -> Iadd6: perfectae */
    f = _functio(c, _carmen(c, "c maior", nona_dominans));
    CREDO_NECESSE (f != NIHIL && f->numerus >= II);
    CREDO_AEQUALIS_I32 ((i32)f->gradus[I].clausula,
                        (i32)FUNCTIO_PERFECTA);
    f = _functio(c, _carmen(c, "c maior", clausula_sextae));
    CREDO_NECESSE (f != NIHIL && f->numerus >= II);
    CREDO_CHORDA_AEQUALIS_LITERIS (chorda_ex_literis(
        f->gradus[I].numeralis, c->piscina), "Iadd6");
    CREDO_AEQUALIS_I32 ((i32)f->gradus[I].clausula,
                        (i32)FUNCTIO_PERFECTA);
}

/* linea functionis carminis (partitura cum linea functio) */
interior chorda
_linea_functionis (
        CredoContextus* c,
       constans Carmen* k,
               Functio* f)
{
        PartituraForma  forma;
     ChordaAedificator* a;
                chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    memset(&forma, ZEPHYRUM, magnitudo(forma));
    forma.additae[ZEPHYRUM].titulus = "functio";
    forma.additae[ZEPHYRUM].scribere = functio_lineam_scribere;
    forma.additae[ZEPHYRUM].ctx = f;
    a = chorda_aedificator_creare(c->piscina, M);
    si (a == NIHIL || !partitura_scribere(k, &forma, a))
    {
        redde vacua;
    }
    redde chorda_aedificator_finire(a);
}

/* dominans praevia: D7 ante G maior in F maior = V7 (G:), adventus
 * perfecta; viio7 imperfecta; ii-V: Am7 manet iii7 in F; ad minorem
 * "e:"; C7 (non dominans G) in F manet */
interior vacuum
_sectio_modulatio (
    CredoContextus* c)
{
    hic_manens constans character* constans G_MAIOR =
        "<clavis mensura=\"2\" valor=\"g maior\"/>";
    constans character* partes[V];
       constans Carmen* k;
               Functio* f;
                chorda  t;

    partes[4]  = NIHIL;
    partes[0]  = "o3 d1 | g1";
    partes[1]  = "o4 c1 | <b1";
    partes[2]  = "o4 f#1 | d1";
    partes[3]  = "o5 a1 | g1";
    k          = _carmen_mutatum(c, "f maior", G_MAIOR, partes);
    f          = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == II);
    CREDO_LITERAE_AEQUALES (f->gradus[ZEPHYRUM].numeralis, "V7");
    CREDO_VERUM (f->gradus[ZEPHYRUM].praevia);
    CREDO_LITERAE_AEQUALES (f->gradus[I].numeralis, "I");
    CREDO_VERUM (!f->gradus[I].praevia);
    CREDO_AEQUALIS_I32 ((i32)f->gradus[I].clausula,
                        (i32)FUNCTIO_PERFECTA);
    t = _linea_functionis(c, k, f);
    CREDO_CHORDA_CONTINET (t, chorda_ex_literis("  functio  G: V7:1\n",
        c->piscina));
    CREDO_CHORDA_CONTINET (t, chorda_ex_literis(
        "  functio  I:1 [perf.]\n", c->piscina));
    /* viio7 tonicae novae: imperfecta */
    partes[0] = "o3 f#1 | g1";
    partes[1] = "o4 c1 | <b1";
    partes[2] = "o4 e-1 | d1";
    partes[3] = "o5 a1 | g1";
    f = _functio(c, _carmen_mutatum(c, "f maior", G_MAIOR, partes));
    CREDO_NECESSE (f != NIHIL && f->numerus == II);
    CREDO_LITERAE_AEQUALES (f->gradus[ZEPHYRUM].numeralis, "viio7");
    CREDO_VERUM (f->gradus[ZEPHYRUM].praevia);
    CREDO_AEQUALIS_I32 ((i32)f->gradus[I].clausula,
                        (i32)FUNCTIO_IMPERFECTA);
    /* ii-V: Am7 in F manet (iii7), D7 praevia */
    partes[0] = "o3 a2 d2 | g1";
    partes[1] = "o4 c2 c2 | <b1";
    partes[2] = "o4 e2 f#2 | d1";
    partes[3] = "o5 g2 a2 | g1";
    f = _functio(c, _carmen_mutatum(c, "f maior", G_MAIOR, partes));
    CREDO_NECESSE (f != NIHIL && f->numerus == III);
    CREDO_LITERAE_AEQUALES (f->gradus[ZEPHYRUM].numeralis, "iii7");
    CREDO_VERUM (!f->gradus[ZEPHYRUM].praevia);
    CREDO_LITERAE_AEQUALES (f->gradus[I].numeralis, "V7");
    CREDO_VERUM (f->gradus[I].praevia);
    /* ad minorem: B7 ante e minor, littera parva */
    partes[0] = "o3 b1 | e1";
    partes[1] = "o4 a1 | g1";
    partes[2] = "o4 d#1 | e1";
    partes[3] = "o5 f#1 | b1";
    k = _carmen_mutatum(c, "f maior",
        "<clavis mensura=\"2\" valor=\"e minor\"/>", partes);
    f = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == II);
    CREDO_LITERAE_AEQUALES (f->gradus[ZEPHYRUM].numeralis, "V7");
    CREDO_CHORDA_CONTINET (_linea_functionis(c, k, f),
        chorda_ex_literis(
        "  functio  e: V7:1\n", c->piscina));
    /* C7 ante G: non dominans clavis novae - in F manet */
    partes[0]  = "o3 c1 | g1";
    partes[1]  = "o4 b-1 | b1";
    partes[2]  = "o4 e1 | d1";
    partes[3]  = "o5 g1 | g1";
    k          = _carmen_mutatum(c, "f maior", G_MAIOR, partes);
    f          = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == II);
    CREDO_LITERAE_AEQUALES (f->gradus[ZEPHYRUM].numeralis, "V7");
    CREDO_VERUM (!f->gradus[ZEPHYRUM].praevia);
    CREDO_CHORDA_CONTINET (_linea_functionis(c, k, f),
        chorda_ex_literis(
        "  functio  V7:1\n", c->piscina));
    /* tonica clavis veteris numquam praevia: D post A7 in d minor ante
     * G maior Picardia est (Em proxima non IV) */
    partes[0] = "o3 a1 | d1 | e1";
    partes[1] = "o4 c#1 | d1 | <b1";
    partes[2] = "o4 g1 | f#1 | g1";
    partes[3] = "o5 e1 | a1 | g1";
    k = _carmen_mutatum(c, "d minor",
        "<clavis mensura=\"3\" valor=\"g maior\"/>", partes);
    f = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == III);
    CREDO_LITERAE_AEQUALES (f->gradus[I].numeralis, "I");
    CREDO_VERUM (!f->gradus[I].praevia);
    CREDO_VERUM (functio_tertia_picardiae(k, f->explicatio, I));
    /* prior in clave fenestrae: A7 (F maior) ante d minor = V7 -> D
     * Picardia */
    partes[0] = "o3 a1 | d1 | a1";
    partes[1] = "o4 c#1 | d1 | c#1";
    partes[2] = "o4 g1 | f#1 | e1";
    partes[3] = "o5 e1 | a1 | a1";
    k = _carmen_mutatum(c, "f maior",
        "<clavis mensura=\"2\" valor=\"d minor\"/>", partes);
    f = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == III);
    CREDO_VERUM (functio_tertia_picardiae(k, f->explicatio, I));
    /* proxima in clave fenestrae: G7 (praevia ad C) in d minor IV7 ->
     * D non Picardia */
    partes[0] = "o3 a1 | d1 | g1 | c1";
    partes[1] = "o4 c#1 | d1 | f1 | e1";
    partes[2] = "o4 g1 | f#1 | g1 | g1";
    partes[3] = "o5 e1 | a1 | b1 | >c1";
    k = _carmen_mutatum(c, "d minor",
        "<clavis mensura=\"4\" valor=\"c maior\"/>", partes);
    f = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == IV);
    CREDO_VERUM (f->gradus[II].praevia);
    CREDO_VERUM (!functio_tertia_picardiae(k, f->explicatio, I));
}

/* tonica veteris ante subdominantem: I cum clausula perfecta, non
 * "F: V"; pausa ante mutationem transilitur */
interior vacuum
_sectio_modulatio_finalis (
    CredoContextus* c)
{
    constans character* partes[V];
       constans Carmen* k;
               Functio* f;

    partes[4] = NIHIL;
    partes[0] = "o3 c1 | f1 | g1 | c1 | f1 | b-1 | c1 | f1";
    partes[1] = "o4 e1 | f1 | f1 | e1 | f1 | f1 | e1 | f1";
    partes[2] = "o4 g1 | a1 | b1 | g1 | a1 | b-1 | b-1 | a1";
    partes[3] = "o5 c1 | c1 | d1 | c1 | c1 | d1 | g1 | f1";
    k = _carmen_mutatum(c, "c maior",
        "<clavis mensura=\"5\" valor=\"f maior\"/>", partes);
    f = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == VIII);
    CREDO_LITERAE_AEQUALES (f->gradus[III].numeralis, "I");
    CREDO_VERUM (!f->gradus[III].praevia);
    CREDO_AEQUALIS_I32 ((i32)f->gradus[III].clausula,
                        (i32)FUNCTIO_PERFECTA);
    CREDO_AEQUALIS_I32 ((i32)f->gradus[IV].clausula,
                        (i32)FUNCTIO_NULLA);
    /* D7 pausa secuta, deinde G maior: praevia ("G: V7") */
    partes[0] = "o3 f1 | d2 r2 | g1";
    partes[1] = "o4 c1 | c2 r2 | b1";
    partes[2] = "o4 a1 | f#2 r2 | d1";
    partes[3] = "o5 f1 | a2 r2 | g1";
    k = _carmen_mutatum(c, "f maior",
        "<clavis mensura=\"3\" valor=\"g maior\"/>", partes);
    f = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus >= IV);
    CREDO_LITERAE_AEQUALES (f->gradus[I].numeralis, "V7");
    CREDO_VERUM (f->gradus[I].praevia);
    CREDO_CHORDA_CONTINET (_linea_functionis(c, k, f),
        chorda_ex_literis(
        "  functio  G: V7:2 -:2\n", c->piscina));
    /* nona praevia: D9 ante G maior in F */
    partes[0] = "o3 d1 | g1";
    partes[1] = "o4 f#1 | g1";
    partes[2] = "o5 c1 | <b1";
    partes[3] = "o5 e1 | d1";
    k = _carmen_mutatum(c, "f maior",
        "<clavis mensura=\"2\" valor=\"g maior\"/>", partes);
    f = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == II);
    CREDO_LITERAE_AEQUALES (f->gradus[ZEPHYRUM].numeralis, "V9");
    CREDO_VERUM (f->gradus[ZEPHYRUM].praevia);
    /* clavis parallela: G7 in C maior ante c minor - "c: V7" */
    partes[0] = "o3 g1 | c1";
    partes[1] = "o4 f1 | e-1";
    partes[2] = "o4 b1 | g1";
    partes[3] = "o5 d1 | c1";
    k = _carmen_mutatum(c, "c maior",
        "<clavis mensura=\"2\" valor=\"c minor\"/>", partes);
    f = _functio(c, k);
    CREDO_NECESSE (f != NIHIL && f->numerus == II);
    CREDO_VERUM (f->gradus[ZEPHYRUM].praevia);
    CREDO_CHORDA_CONTINET (_linea_functionis(c, k, f),
        chorda_ex_literis(
        "  functio  c: V7:1\n", c->piscina));
}

/* duplex usus: sexta in IV ante dominantem = ii65 (iih65 in minore);
 * ante I manet IVadd6 */
interior vacuum
_sectio_duplex (
    CredoContextus* c)
{
    constans character* partes[V];
               Functio* f;

    partes[4]  = NIHIL;
    partes[0]  = "o3 c1 | f1 | g1 | c1";
    partes[1]  = "o4 c1 | c1 | b1 | c1";
    partes[2]  = "o4 g1 | a1 | g1 | g1";
    partes[3]  = "o5 e1 | d1 | d1 | c1";
    f          = _functio(c, _carmen(c, "c maior", partes));
    CREDO_NECESSE (f != NIHIL && f->numerus == IV);
    CREDO_LITERAE_AEQUALES (f->gradus[I].numeralis, "ii65");
    partes[0]  = "o3 a1 | d1 | e1 | a1";
    partes[1]  = "o4 a1 | a1 | g#1 | a1";
    partes[2]  = "o4 e1 | f1 | e1 | e1";
    partes[3]  = "o5 c1 | b1 | b1 | a1";
    f          = _functio(c, _carmen(c, "a minor", partes));
    CREDO_NECESSE (f != NIHIL && f->numerus == IV);
    CREDO_LITERAE_AEQUALES (f->gradus[I].numeralis, "iih65");
    partes[0]  = "o3 c1 | f1 | c1";
    partes[1]  = "o4 c1 | c1 | c1";
    partes[2]  = "o4 g1 | a1 | g1";
    partes[3]  = "o5 e1 | d1 | e1";
    f          = _functio(c, _carmen(c, "c maior", partes));
    CREDO_NECESSE (f != NIHIL && f->numerus == III);
    CREDO_LITERAE_AEQUALES (f->gradus[I].numeralis, "IVadd6");
    /* ante viio quoque: F6 -> Bdim -> C */
    partes[0]  = "o3 f1 | b1 | c1";
    partes[1]  = "o4 c1 | d1 | c1";
    partes[2]  = "o4 a1 | f1 | e1";
    partes[3]  = "o5 d1 | d1 | c1";
    f          = _functio(c, _carmen(c, "c maior", partes));
    CREDO_NECESSE (f != NIHIL && f->numerus == III);
    CREDO_LITERAE_AEQUALES (f->gradus[ZEPHYRUM].numeralis, "ii65");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "vocabularium", _sectio_vocabularium, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "diatonici", _sectio_diatonici, NIHIL, NIHIL, NIHIL, NIHIL },
    { "conversiones", _sectio_conversiones, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "applicatae", _sectio_applicatae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "deminutae", _sectio_deminutae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "binae", _sectio_binae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausulae", _sectio_clausulae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "linea", _sectio_linea, NIHIL, NIHIL, NIHIL, NIHIL },
    { "picardia", _sectio_picardia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "modulatio", _sectio_modulatio, NIHIL, NIHIL, NIHIL, NIHIL },
    { "modulatio_finalis", _sectio_modulatio_finalis, NIHIL, NIHIL,
        NIHIL,
        NIHIL },
    { "duplex", _sectio_duplex, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("functio", SECTIONES);
}
