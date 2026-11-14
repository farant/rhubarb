/* probatio_graphis_casa.c - picturae casae (apps/scaena/loci)
 *
 * Duo prospectus casae ex plagulis veris cum partibus ex
 * graphis/res componuntur; tituli regionum fixi (scaena eos
 * nominabit), regio quaeque sine foraminibus, digesta fixa
 * (mutatio picturae consulto fiat).
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "friatio.h"
#include "graphis.h"
#include "graphis_specimen.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LAT GRAPHIS_LATITUDO
#define ALT GRAPHIS_ALTITUDO

/* partes ex graphis/res, ut bin/graphis */
interior chorda
_fons (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    character via[CXXVIII];
       chorda vacua;

    (vacuum)contextus;
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    (vacuum)sprintf(via, "graphis/res/%.*s.stml",
        (integer)titulus.mensura,
        (constans character*)titulus.datum);
    si (!filum_existit(via))
    {
        redde vacua;
    }
    redde filum_legere_totum(via, piscina);
}

interior GraphisFructus
_prospectus (
        CredoContextus* c,
    constans character* via)
{
    GraphisFructus f = graphis_legere_cum_fonte(
        filum_legere_totum(via, c->piscina), _fons, NIHIL,
        c->piscina);

    si (!f.successus)
    {
        printf("%s recusata ('%.*s' linea %u): %.*s\n", via,
            (integer)f.plagula.mensura,
            (constans character*)f.plagula.datum,
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    redde f;
}

/* tituli ordine dato, nec plus */
interior vacuum
_titulos_probare (
             CredoContextus* c,
    constans GraphisFructus* f,
         constans character* constans* tituli,
                        i32  numerus)
{
    i32 k;

    (vacuum)c;
    CREDO_NECESSE (f->tabula->titulorum == numerus);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (!chorda_aequalis_literis(f->tabula->tituli[k], tituli[k]))
        {
            printf("titulus %u: '%.*s' (speratus '%s')\n",
                (insignatus)k,
                (integer)f->tabula->tituli[k].mensura,
                (constans character*)f->tabula->tituli[k].datum,
                tituli[k]);
        }
        CREDO_VERUM (chorda_aequalis_literis(f->tabula->tituli[k],
            tituli[k]));
    }
}

/* regio capsam suam fere implet (sine foraminibus magnis) */
interior vacuum
_regionem_plenam (
             CredoContextus* c,
    constans GraphisFructus* f,
                        i32  index,
                        i32  centesimae)
{
    GraphisRegio regiones[GRAPHIS_TITULI_MAXIMI];
             i32 numerus = graphis_regiones(f, regiones);
             i32 capsa;

    (vacuum)c;
    CREDO_NECESSE (index < numerus);
    capsa = (i32)(regiones[index].x_ultimus - regiones[index].x_primus
        + I) * (i32)(regiones[index].y_ultimus
        - regiones[index].y_primus + I);
    si (regiones[index].pixela * C < capsa * centesimae)
    {
        printf("regio %u: %u px in capsa %u\n", (insignatus)index,
            (insignatus)regiones[index].pixela, (insignatus)capsa);
    }
    CREDO_VERUM (regiones[index].pixela * C >= capsa * centesimae);
}

#define DIGESTUM_FOCI    0x30254be5u
#define DIGESTUM_OSTII   0x266fc49du

interior vacuum
_digestum_probare (
             CredoContextus* c,
    constans GraphisFructus* f,
         constans character* titulus,
                        i32  speratum)
{
    i32 digestum = friatio_fnv1a_literis(
        (constans character*)f->indices, LAT * ALT);

    (vacuum)c;
    si (digestum != speratum)
    {
        printf("digestum %s: 0x%08xu\n", titulus,
            (insignatus)digestum);
    }
    CREDO_AEQUALIS_I32 (digestum, speratum);
}

interior vacuum
_sectio_focus (
    CredoContextus* c)
{
    hic_manens constans character* constans TITULI[V] = {
        "hearth", "window", "jars", "book", "turn"
    };
    GraphisFructus f = _prospectus(c,
        "apps/scaena/loci/casa_focus.stml");

    _titulos_probare(c, &f, TITULI, V);
    /* focus et fenestra: capsae plenae; vasa tria cum intervallis */
    _regionem_plenam(c, &f, ZEPHYRUM, 90);
    _regionem_plenam(c, &f, I, 95);
    _regionem_plenam(c, &f, II, 55);
    _regionem_plenam(c, &f, III, 95);
    /* ignis in foco regionis foci est */
    CREDO_AEQUALIS_I32 (f.tabula->domini[187 * LAT + 227], I);
    CREDO_VERUM (f.tabula->lucentes[187 * LAT + 227] != ZEPHYRUM);
    /* collis in fenestra regionis fenestrae est */
    CREDO_AEQUALIS_I32 (f.tabula->domini[131 * LAT + 333], II);
    _digestum_probare(c, &f, "foci", DIGESTUM_FOCI);
}

interior vacuum
_sectio_ostium (
    CredoContextus* c)
{
    hic_manens constans character* constans TITULI[IV] = {
        "door", "coat", "bed", "turn"
    };
    GraphisFructus f = _prospectus(c,
        "apps/scaena/loci/casa_ostium.stml");

    _titulos_probare(c, &f, TITULI, IV);
    _regionem_plenam(c, &f, ZEPHYRUM, 95);
    _regionem_plenam(c, &f, I, 75);
    /* lectus in perspectiva: capsa non plena */
    _regionem_plenam(c, &f, II, 50);
    /* ansa ostii et stragula lecti regionum suarum sunt */
    CREDO_AEQUALIS_I32 (f.tabula->domini[137 * LAT + 199], I);
    CREDO_AEQUALIS_I32 (f.tabula->domini[213 * LAT + 360], III);
    _digestum_probare(c, &f, "ostii", DIGESTUM_OSTII);
}

/* prospectus ambo eandem cameram habent: angulus tecti idem */
interior vacuum
_sectio_camera (
    CredoContextus* c)
{
    GraphisFructus a = _prospectus(c,
        "apps/scaena/loci/casa_focus.stml");
    GraphisFructus b = _prospectus(c,
        "apps/scaena/loci/casa_ostium.stml");
               i32 x;
               i32 mala = ZEPHYRUM;

    per (x = ZEPHYRUM; x < LAT; x++)
    {
        si (memcmp(a.tabula->fons + (10 * LAT + x) * IV,
            b.tabula->fons + (10 * LAT + x) * IV, IV) != ZEPHYRUM)
        {
            mala++;
        }
    }
    CREDO_AEQUALIS_I32 (mala, ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "focus", _sectio_focus, NIHIL, NIHIL, NIHIL, NIHIL },
    { "ostium", _sectio_ostium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "camera", _sectio_camera, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("graphis_casa", SECTIONES);
}
