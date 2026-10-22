/* probatio_cantuale.c - Probationes libri cantilenarum
 * (include/cantuale.h)
 *
 * Liber totus in lectione iudicatur: vitia cum titulo cantionis et
 * offset in textu eius. Electio per Sors seminatam - numeri exacti
 * non asseruntur, sed fines (omnes eliguntur, aliena numquam,
 * vitanda numquam bis) et determinismus (idem semen = eadem series).
 */
#include <stdio.h>
#include <string.h>
#include "latina.h"
#include "credo.h"
#include "cantuale.h"

#define LIBER_PROBUS \
    "<cantuale>\n" \
    "  <!-- commentum ignoratur -->\n" \
    "  <cantilena titulus=\"arpeggium\" genera=\"finis  laetus\">\n" \
    "    <textus!>t180 o6 l16 e g# b >e8</textus>\n" \
    "  </cantilena>\n" \
    "  <cantilena titulus=\"pulsatio\" genera=\"permissio\" intactus>\n" \
    "    <textus!>@punctum t160 o6 l16 c r c r8 <g4</textus>\n" \
    "  </cantilena>\n" \
    "  <cantilena titulus=\"sola\">\n" \
    "    <textus!>c</textus>\n" \
    "  </cantilena>\n" \
    "</cantuale>\n"

#define LIBER_ELECTIONIS \
    "<cantuale>\n" \
    "  <cantilena titulus=\"finis_a\" genera=\"finis\">" \
    "<textus!>c</textus></cantilena>\n" \
    "  <cantilena titulus=\"finis_b\" genera=\"finis laetus\">" \
    "<textus!>d</textus></cantilena>\n" \
    "  <cantilena titulus=\"finis_c\" genera=\"finis\">" \
    "<textus!>e</textus></cantilena>\n" \
    "  <cantilena titulus=\"rogatio\" genera=\"permissio\">" \
    "<textus!>f</textus></cantilena>\n" \
    "  <cantilena titulus=\"error\" genera=\"vitium\">" \
    "<textus!>g</textus></cantilena>\n" \
    "</cantuale>\n"

interior chorda
_chorda (
        CredoContextus* c,
    constans character* litterae)
{
    redde chorda_ex_literis(litterae, c->piscina);
}

/* librum legit; VERUM asserit */
interior b32
_librum_legere (
     CredoContextus* c,
    constans character* litterae,
           Cantuale* liber)
{
    CantualeVitium vitium;
               b32 bene;

    bene = cantuale_legere(_chorda(c, litterae), c->piscina, liber,
                           &vitium);
    CREDO_VERUM (bene);
    si (!bene)
    {
        imprimere("  liber: '%.*s' %d: %s\n",
                  (integer)vitium.titulus.mensura,
                  (constans character*)vitium.titulus.datum,
                  (integer)vitium.offset,
                  vitium.causa != NIHIL ? vitium.causa : "(nulla)");
    }
    redde bene;
}

/* librum legit; FALSUM, titulum cantionis, offset et librum vacuum
 * asserit ("" = liber ipse) */
interior vacuum
_vitium_libri (
     CredoContextus* c,
    constans character* litterae,
    constans character* titulus,
                    i32 offset)
{
          Cantuale liber;
    CantualeVitium vitium;
               b32 bene;

    vitium.titulus  = _chorda(c, "(nullus)");
    vitium.offset   = CMXCIX;
    vitium.causa    = NIHIL;
    bene = cantuale_legere(_chorda(c, litterae), c->piscina, &liber,
                           &vitium);
    CREDO_FALSUM (bene);
    CREDO_CHORDA_AEQUALIS_LITERIS (vitium.titulus, titulus);
    CREDO_AEQUALIS_S32 ((s32)vitium.offset, (s32)offset);
    CREDO_VERUM (vitium.causa != NIHIL);
    CREDO_AEQUALIS_I32 (liber.numerus, ZEPHYRUM);
    CREDO_VERUM (liber.cantiones == NIHIL);
    si (bene || (s32)vitium.offset != (s32)offset)
    {
        imprimere("  '%s': titulus '%.*s', offset %d, causa %s\n",
                  litterae, (integer)vitium.titulus.mensura,
                  (constans character*)vitium.titulus.datum,
                  (integer)vitium.offset,
                  vitium.causa != NIHIL ? vitium.causa : "(nulla)");
    }
}

/* liber unius cantionis 'a' cum textu dato */
interior constans character*
_liber_unius (
        CredoContextus* c,
    constans character* textus)
{
    memoriae_index  n = strlen(textus) + CXXVIII;
         character* b = (character*)piscina_allocare(c->piscina, n);

    sprintf(b, "<cantuale><cantilena titulus=\"a\" genera=\"finis\">"
               "<textus!>%s</textus></cantilena></cantuale>", textus);
    redde b;
}

/* "l64 " + n x "c " */
interior constans character*
_notae_multae (
     CredoContextus* c,
                i32  n)
{
    character* b = (character*)piscina_allocare(c->piscina,
        (memoriae_index)(n * II + VIII));
         i32 k;

    strcpy(b, "l64 ");
    per (k = ZEPHYRUM; k < n; k++)
    {
        strcat(b, "c ");
    }
    redde b;
}


    /* ==================================================
     * LEGERE: cantiones, tituli, genera, textus, cantilenae
     * ================================================== */

interior vacuum
_sectio_legere (
    CredoContextus* c)
{
    Cantuale liber;

    si (!_librum_legere(c, LIBER_PROBUS, &liber))
    {
        redde;
    }
    CREDO_AEQUALIS_I32 (liber.numerus, III);
    si (liber.numerus != III)
    {
        redde;
    }

    /* arpeggium: genera spatiis duplicibus separata */
    CREDO_CHORDA_AEQUALIS_LITERIS (liber.cantiones[ZEPHYRUM].titulus,
                                   "arpeggium");
    CREDO_CHORDA_AEQUALIS_LITERIS (liber.cantiones[ZEPHYRUM].textus,
                                   "t180 o6 l16 e g# b >e8");
    CREDO_AEQUALIS_I32 (liber.cantiones[ZEPHYRUM].genera_numerus, II);
    si (liber.cantiones[ZEPHYRUM].genera_numerus == II)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (
            liber.cantiones[ZEPHYRUM].genera[ZEPHYRUM], "finis");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            liber.cantiones[ZEPHYRUM].genera[I], "laetus");
    }
    CREDO_FALSUM (liber.cantiones[ZEPHYRUM].intactus);
    CREDO_AEQUALIS_I32 (liber.cantiones[ZEPHYRUM].cantilena.numerus,
        IV);
    si (liber.cantiones[ZEPHYRUM].cantilena.numerus == IV)
    {
        /* o6 e = LXXXVIII, >e = C */
        CREDO_AEQUALIS_I32 (
            liber.cantiones[ZEPHYRUM].cantilena.notae[ZEPHYRUM].nota,
            LXXXVIII);
        CREDO_AEQUALIS_I32 (
            liber.cantiones[ZEPHYRUM].cantilena.notae[III].nota, C);
    }

    /* pulsatio: intactus, praeparatum, '<' intra textum crudum */
    CREDO_CHORDA_AEQUALIS_LITERIS (liber.cantiones[I].textus,
        "@punctum t160 o6 l16 c r c r8 <g4");
    CREDO_VERUM (liber.cantiones[I].intactus);
    CREDO_AEQUALIS_I32 (liber.cantiones[I].genera_numerus, I);
    CREDO_AEQUALIS_I32 (liber.cantiones[I].cantilena.numerus, III);
    si (liber.cantiones[I].cantilena.numerus == III)
    {
            constans Cantio* p = &liber.cantiones[I];
        constans CantusNota* n = &p->cantilena.notae[II];

        CREDO_AEQUALIS_I32 (n->nota, LXXIX);   /* o5 g */
        CREDO_CHORDA_AEQUALIS_LITERIS (n->praeparatum, "punctum");
        /* notae in textum cantionis spectant */
        CREDO_VERUM (   n->praeparatum.datum >= p->textus.datum
                     && n->praeparatum.datum
                        < p->textus.datum + p->textus.mensura);
    }

    /* sola: sine generibus */
    CREDO_CHORDA_AEQUALIS_LITERIS (liber.cantiones[II].titulus, "sola");
    CREDO_AEQUALIS_I32 (liber.cantiones[II].genera_numerus, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (liber.cantiones[II].cantilena.numerus, I);
}

interior vacuum
_sectio_limites (
    CredoContextus* c)
{
    Cantuale liber;

    /* liber vacuus licet */
    si (_librum_legere(c, "<cantuale></cantuale>", &liber))
    {
        CREDO_AEQUALIS_I32 (liber.numerus, ZEPHYRUM);
    }
    /* genera vacua = nulla */
    si (_librum_legere(c,
        "<cantuale><cantilena titulus=\"a\" genera=\"\">"
                          "<textus!>c</textus></cantilena></cantuale>",
                       &liber))
    {
        CREDO_AEQUALIS_I32 (liber.cantiones[ZEPHYRUM].genera_numerus,
                            ZEPHYRUM);
    }
    /* XXXII notae: limes ipse */
    si (_librum_legere(c, _liber_unius(c, _notae_multae(c, XXXII)),
                       &liber))
    {
        CREDO_AEQUALIS_I32 (liber.cantiones[ZEPHYRUM].cantilena.numerus,
                            CANTUALE_NOTAE_MAXIMAE);
    }
    /* tituli cum cifris et '_' */
    CREDO_VERUM (_librum_legere(c,
        "<cantuale><cantilena titulus=\"finis_2\" genera=\"finis_x\">"
        "<textus!>c</textus></cantilena></cantuale>", &liber));
}


    /* ==================================================
     * VITIA: titulus cantionis et offset in textu eius
     * ================================================== */

interior vacuum
_sectio_vitia (
    CredoContextus* c)
{
    /* liber ipse: titulus vacuus */
    _vitium_libri(c, "<cantuale><cantilena", "", ZEPHYRUM);
    _vitium_libri(c, "<liber></liber>", "", ZEPHYRUM);
    _vitium_libri(c,
        "<cantuale><cantio titulus=\"a\"><textus!>c</textus>"
                     "</cantio></cantuale>", "", ZEPHYRUM);
    _vitium_libri(c, "<cantuale><cantilena genera=\"finis\">"
                     "<textus!>c</textus></cantilena></cantuale>",
                  "", ZEPHYRUM);

    /* cantio nominata */
    _vitium_libri(c, "<cantuale>"
                     "<cantilena titulus=\"a\"><textus!>c</textus>"
                     "</cantilena>"
                     "<cantilena titulus=\"a\"><textus!>d</textus>"
                     "</cantilena></cantuale>", "a", ZEPHYRUM);
    _vitium_libri(c, "<cantuale><cantilena titulus=\"Arp\">"
                     "<textus!>c</textus></cantilena></cantuale>",
                  "Arp", ZEPHYRUM);
    _vitium_libri(c,
        "<cantuale><cantilena titulus=\"a\" genera=\"finis\">"
                     "</cantilena></cantuale>", "a", ZEPHYRUM);
    _vitium_libri(c, "<cantuale><cantilena titulus=\"a\" "
                     "genera=\"finis Laetus\"><textus!>c</textus>"
                     "</cantilena></cantuale>", "a", ZEPHYRUM);

    /* textus: offset lectoris cantus, '@' praeparati ignoti, nota
     * XXXIII, cantilena sine notis */
    _vitium_libri(c, _liber_unius(c, "t180 l3 c"), "a", VI);
    _vitium_libri(c, _liber_unius(c, "c @nullum d"), "a", II);
    _vitium_libri(c, _liber_unius(c, _notae_multae(c, XXXIII)), "a",
                  LXVIII);
    _vitium_libri(c, _liber_unius(c, ""), "a", ZEPHYRUM);
    _vitium_libri(c, _liber_unius(c, "t90 r4 r2"), "a", ZEPHYRUM);
}


    /* ==================================================
     * INVENIRE et GENERA
     * ================================================== */

interior vacuum
_sectio_invenire (
    CredoContextus* c)
{
            Cantuale  liber;
     constans Cantio* b;

    si (!_librum_legere(c, LIBER_ELECTIONIS, &liber))
    {
        redde;
    }
    b = cantuale_invenire(&liber, _chorda(c, "finis_b"));
    CREDO_VERUM (b == &liber.cantiones[I]);
    CREDO_VERUM (cantuale_invenire(&liber, _chorda(c, "finis"))
        == NIHIL);
    CREDO_VERUM (cantuale_invenire(&liber, _chorda(c, "finis_bb"))
                 == NIHIL);
    CREDO_VERUM (cantuale_invenire(&liber, _chorda(c, "")) == NIHIL);
    si (b != NIHIL)
    {
        CREDO_VERUM (cantio_generis_est(b, _chorda(c, "laetus")));
        CREDO_VERUM (cantio_generis_est(b, _chorda(c, "finis")));
        CREDO_FALSUM (cantio_generis_est(b, _chorda(c, "laet")));
        CREDO_FALSUM (cantio_generis_est(b, _chorda(c, "")));
        CREDO_FALSUM (cantio_generis_est(b, _chorda(c, "permissio")));
    }
}


    /* ==================================================
     * ELIGERE: sors aequa, aliena numquam, vitanda numquam bis
     * ================================================== */

interior vacuum
_sectio_eligere (
    CredoContextus* c)
{
            Cantuale  liber;
                Sors  sors;
                Sors  gemina;
     constans Cantio* prior = NIHIL;
     constans Cantio* e;
              chorda  finis;
                 i32  numeri[V];
                 i32  k;
                 b32  alienum   = FALSUM;
                 b32  iteratum  = FALSUM;
                 b32  pares     = VERUM;

    si (!_librum_legere(c, LIBER_ELECTIONIS, &liber))
    {
        redde;
    }
    finis = _chorda(c, "finis");

    /* CCC electiones sine vitanda: tres finis fere aequae */
    per (k = ZEPHYRUM; k < V; k++)
    {
        numeri[k] = ZEPHYRUM;
    }
    sors_seminare(&sors, XLII, ZEPHYRUM);
    per (k = ZEPHYRUM; k < CCC; k++)
    {
        e = cantuale_eligere(&liber, finis, &sors, NIHIL);
        si (e == NIHIL || !cantio_generis_est(e, finis))
        {
            alienum = VERUM;
            perge;
        }
        numeri[e - liber.cantiones]++;
    }
    CREDO_FALSUM (alienum);
    per (k = ZEPHYRUM; k < III; k++)
    {
        CREDO_MAIOR_I32 (numeri[k], LX);
        CREDO_MINOR_I32 (numeri[k], CXL);
    }
    CREDO_AEQUALIS_I32 (numeri[III], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (numeri[IV], ZEPHYRUM);

    /* cum vitanda: numquam eadem bis */
    per (k = ZEPHYRUM; k < CCC; k++)
    {
        e = cantuale_eligere(&liber, finis, &sors, prior);
        si (e == NIHIL || e == prior)
        {
            iteratum = VERUM;
        }
        prior = e;
    }
    CREDO_FALSUM (iteratum);

    /* candidata sola: vitanda redditur; vitanda aliena ignoratur */
    e = &liber.cantiones[IV];
    CREDO_VERUM (cantuale_eligere(&liber, _chorda(c, "vitium"), &sors,
        e)
                 == e);
    CREDO_VERUM (cantuale_eligere(&liber, _chorda(c, "permissio"),
        &sors,
                                  &liber.cantiones[ZEPHYRUM])
                 == &liber.cantiones[III]);

    /* genus ignotum aut praefixum: NIHIL */
    CREDO_VERUM (cantuale_eligere(&liber, _chorda(c, "nullum"), &sors,
                                  NIHIL) == NIHIL);
    CREDO_VERUM (cantuale_eligere(&liber, _chorda(c, "fin"), &sors,
                                  NIHIL) == NIHIL);

    /* determinismus: idem semen, eadem series */
    sors_seminare(&sors, VII, ZEPHYRUM);
    sors_seminare(&gemina, VII, ZEPHYRUM);
    prior = NIHIL;
    per (k = ZEPHYRUM; k < L; k++)
    {
        constans Cantio* x = cantuale_eligere(&liber, finis, &sors,
            prior);
        constans Cantio* y = cantuale_eligere(&liber, finis, &gemina,
                                              prior);

        si (x != y)
        {
            pares = FALSUM;
        }
        prior = x;
    }
    CREDO_VERUM (pares);
}


    /* ==================================================
     * ORDINARIUM: liber commissus legitur, occasiones plenae
     * ================================================== */

interior vacuum
_sectio_ordinarium (
    CredoContextus* c)
{
              Cantuale  liber;
        CantualeVitium  vitium;
                   b32  bene;
                   i32  k;
                   i32  o;
    constans character* OCCASIONES[III];

    OCCASIONES[ZEPHYRUM] = "permissio";
    OCCASIONES[I] = "finis";
    OCCASIONES[II] = "vitium";
    bene = cantuale_ordinarium(c->piscina, &liber, &vitium);
    CREDO_VERUM (bene);
    si (!bene)
    {
        imprimere("  cantuale.stml: '%.*s' %d: %s\n",
                  (integer)vitium.titulus.mensura,
                  (constans character*)vitium.titulus.datum,
                  (integer)vitium.offset,
                  vitium.causa != NIHIL ? vitium.causa : "(nulla)");
        redde;
    }
    /* occasio quaeque II cantiones saltem - vitanda alteram habet */
    per (o = ZEPHYRUM; o < III; o++)
    {
        chorda genus  = _chorda(c, OCCASIONES[o]);
           i32 n      = ZEPHYRUM;

        per (k = ZEPHYRUM; k < liber.numerus; k++)
        {
            si (cantio_generis_est(&liber.cantiones[k], genus))
            {
                n++;
            }
        }
        CREDO_MAIOR_AUT_AEQUALIS_I32 (n, II);
        si (n < II)
        {
            imprimere("  occasio '%s': %d cantiones\n", OCCASIONES[o],
                      (integer)n);
        }
    }
}


hic_manens constans CredoSectio SECTIONES[] = {
    { "legere", _sectio_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "limites", _sectio_limites, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "invenire", _sectio_invenire, NIHIL, NIHIL, NIHIL, NIHIL },
    { "eligere", _sectio_eligere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ordinarium", _sectio_ordinarium, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("cantuale", SECTIONES);
}
