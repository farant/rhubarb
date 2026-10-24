/* probatio_aedilis_praefixum.c - clavis thesauri extractoris
 * (circulus commissionis P1): aedilis_thesauri_praefixum.
 *
 * Digestum validum (LXIV hex, actio 'aedilis') -> clavis ex digesto et
 * aedilis.stml SOLIS: binaria diversa clavem eandem habent (thesaurus
 * communis; editio extra extractorem clavem non mutat). 'ignotum' aut
 * forma prava -> clavis ex binario ut olim. Plagulae fictae in area
 * sectionis (credo_area).
 */
#include "latina.h"
#include "lectiones.h"   /* lectiones_fopen: canalis (examen 96) */
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "credo.h"
#include "sigillum.h"
#include "aedilis.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character* constans DIGESTUM_A =
    "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef";
hic_manens constans character* constans DIGESTUM_B =
    "fedcba9876543210fedcba9876543210fedcba9876543210fedcba9876543210";

/* plagulam in area scribere; via in piscina */
interior constans character*
_plagula (
         CredoContextus* c,
     constans character* titulus,
     constans character* contentum)
{
    ChordaAedificator* a = chorda_aedificator_creare(c->piscina,
        (memoriae_index)CXXVIII);
                  FILE* f;
                chorda  via;

    chorda_aedificator_appendere_literis(a, credo_area(c));
    chorda_aedificator_appendere_character(a, '/');
    chorda_aedificator_appendere_literis(a, titulus);
    via  = chorda_aedificator_finire(a);
    f    = lectiones_fopen(chorda_ut_cstr(via, c->piscina), "w");
    si (f != NIHIL)
    {
        fputs(contentum, f);
        fclose(f);
    }
    redde chorda_ut_cstr(via, c->piscina);
}

interior b32
_clavis (
         CredoContextus*  c,
     constans character*  ingressus,
     constans character*  binarium,
     constans character*  configuratio,
               Sigillum*  clavis,
     constans character** fons)
{
    redde aedilis_thesauri_praefixum(ingressus, binarium, configuratio,
        c->piscina, clavis, fons);
}

/* I. digestum validum: binaria DIVERSA, clavis EADEM; fons digestum */
interior vacuum
_sectio_digestum (
    CredoContextus* c)
{
     constans character* b1 = _plagula(c, "fabrica", "binarium unum");
     constans character* b2 = _plagula(c, "aedilis", "binarium b");
     constans character* cf = _plagula(c, "aedilis.stml", "<aedilis/>");
               Sigillum  k1;
               Sigillum  k2;
     constans character* fons = NIHIL;

    CREDO_NECESSE(_clavis(c, DIGESTUM_A, b1, cf, &k1, &fons));
    CREDO_VERUM(fons != NIHIL && strcmp(fons, "digestum") == 0);
    CREDO_NECESSE(_clavis(c, DIGESTUM_A, b2, cf, &k2, &fons));
    CREDO_VERUM(sigillum_aequale(&k1, &k2));
    /* binarium absens non obstat cum digestum validum est */
    CREDO_VERUM(_clavis(c, DIGESTUM_A, "nusquam/binarium", cf, &k2,
        &fons));
    CREDO_VERUM(sigillum_aequale(&k1, &k2));
}

/* II. digestum aliud aut configuratio alia -> clavis alia */
interior vacuum
_sectio_diversa (
    CredoContextus* c)
{
    constans character* b1 = _plagula(c, "fabrica", "binarium unum");
    constans character* cf = _plagula(c, "aedilis.stml", "<aedilis/>");
    constans character* cg = _plagula(c, "alia.stml",
        "<aedilis via=\"x\"/>");
               Sigillum k1;
               Sigillum k2;
               Sigillum k3;

    CREDO_NECESSE(_clavis(c, DIGESTUM_A, b1, cf, &k1, NIHIL));
    CREDO_NECESSE(_clavis(c, DIGESTUM_B, b1, cf, &k2, NIHIL));
    CREDO_NECESSE(_clavis(c, DIGESTUM_A, b1, cg, &k3, NIHIL));
    CREDO_FALSUM(sigillum_aequale(&k1, &k2));
    CREDO_FALSUM(sigillum_aequale(&k1, &k3));
}

/* III. 'ignotum' et formae pravae -> clavis ex BINARIO (ut olim):
 * aequalis sigillo(binarium + configuratio), binaria diversa
 * diversae */
interior vacuum
_sectio_binarium (
    CredoContextus* c)
{
    hic_manens constans character* constans pravae[] = {
        "ignotum", "",
        "0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcde",
        "0123456789ABCDEF0123456789abcdef"
        "0123456789abcdef0123456789abcdef",
        "0123456789abcdef0123456789abcdef"
        "0123456789abcdef0123456789abcdefx",
        NIHIL
    };
     constans character* b1 = _plagula(c, "fabrica", "binarium unum");
     constans character* b2 = _plagula(c, "aedilis", "binarium b");
     constans character* cf = _plagula(c, "aedilis.stml", "<aedilis/>");
      SigillumContextus  ctx;
               Sigillum  expecta;
               Sigillum  k1;
               Sigillum  k2;
     constans character* fons = NIHIL;
                    i32  i;

    sigillum_incipere(&ctx);
    sigillum_addere(&ctx, (constans vacuum*)"binarium unum",
        (memoriae_index)strlen("binarium unum"));
    sigillum_addere(&ctx, (constans vacuum*)"<aedilis/>",
        (memoriae_index)strlen("<aedilis/>"));
    expecta = sigillum_finire(&ctx);
    per (i = ZEPHYRUM; pravae[i] != NIHIL; i++)
    {
        CREDO_NECESSE(_clavis(c, pravae[i], b1, cf, &k1, &fons));
        CREDO_VERUM(fons != NIHIL && strcmp(fons, "binarium") == 0);
        CREDO_VERUM(sigillum_aequale(&k1, &expecta));
    }
    CREDO_NECESSE(_clavis(c, NIHIL, b1, cf, &k1, NIHIL));
    CREDO_VERUM(sigillum_aequale(&k1, &expecta));
    CREDO_NECESSE(_clavis(c, "ignotum", b2, cf, &k2, NIHIL));
    CREDO_FALSUM(sigillum_aequale(&k1, &k2));
}

/* IV. absentes: configuratio absens semper FALSUM; binarium absens
 * FALSUM solum cum clavis ex binario est */
interior vacuum
_sectio_absentes (
    CredoContextus* c)
{
     constans character* b1 = _plagula(c, "fabrica", "binarium unum");
     constans character* cf = _plagula(c, "aedilis.stml", "<aedilis/>");
               Sigillum  k;

    CREDO_FALSUM(_clavis(c, DIGESTUM_A, b1, "nusquam/aedilis.stml", &k,
        NIHIL));
    CREDO_FALSUM(_clavis(c, "ignotum", "nusquam/binarium", cf, &k,
        NIHIL));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "digestum validum", _sectio_digestum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "digestum aut configuratio diversa", _sectio_diversa, NIHIL,
        NIHIL,
      NIHIL, NIHIL },
    { "ignotum et pravae", _sectio_binarium, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "absentes", _sectio_absentes, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("aedilis_praefixum", SECTIONES);
}
