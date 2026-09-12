/* icones.c - icones .app (contractus in capite).
 *
 * SEPTEM redditiones, DECEM plagulae: tabula ORDINES ter idem latus
 * sub nominibus logicis diversis nominat, ergo quodque latus SEMEL
 * scalatur et SEMEL codificatur, et partes geminae eundem indicem
 * octetorum ferunt. Ita .iconset et .icns constructione consentiunt,
 * non duabus semitis quae consentire debent.
 *
 * Nihil decodificat: Imago a vocante venit (par. III, D7).
 */

#include "icones.h"
#include "imago_opus.h"
#include "imago_png.h"
#include "filum.h"
#include "via.h"

#include <string.h>

nomen structura {
                i32  latera;
 constans character* semita;
 constans character* codex;     /* codex .icns */
                i32  vexillum;
} IconesOrdo;

/* Mensuratum per iconutil 2026-09-12 (spec par. IV), non ex chartis:
 * tria latera BIS apparent, et paria gemina octetim aequalia exeunt. */
hic_manens constans IconesOrdo ORDINES[X] = {
    {     XVI, "icon_16x16.png",      "ic04", ICONES_LATERA_XVI     },
    {   XXXII, "icon_16x16@2x.png",   "ic11", ICONES_LATERA_XXXII   },
    {   XXXII, "icon_32x32.png",      "ic05", ICONES_LATERA_XXXII   },
    {    LXIV, "icon_32x32@2x.png",   "ic12", ICONES_LATERA_LXIV    },
    { CXXVIII, "icon_128x128.png",    "ic07", ICONES_LATERA_CXXVIII },
    {   CCLVI, "icon_128x128@2x.png", "ic13", ICONES_LATERA_CCLVI   },
    {   CCLVI, "icon_256x256.png",    "ic08", ICONES_LATERA_CCLVI   },
    {    DXII, "icon_256x256@2x.png", "ic14", ICONES_LATERA_DXII    },
    {    DXII, "icon_512x512.png",    "ic09", ICONES_LATERA_DXII    },
    {   MXXIV, "icon_512x512@2x.png", "ic10", ICONES_LATERA_MXXIV   }
};

interior b32
_recusare (
    IconesStatus* status,
          chorda* sedes_vitii,
    IconesStatus  valor,
          chorda  sedes)
{
    *status       = valor;
    *sedes_vitii  = sedes;
    redde FALSUM;
}

/* Quadratum ad centrum, deinde ad latera exacta. Dimensiones IPSI
 * computamus et non 'intra limites' petimus, quia creatores
 * aspectum servantes rationem et productum truncant (imago_opus.c:
 * 263-271) et 512 ex crop 512 lato 511 dare possunt. Ad 1:1 nihil
 * scalatur: imago fontis ipsa redditur. */
interior Imago
_quadratum_scalare (
     constans Imago* fons,
                s32  latus_fontis,
                s32  latera,
            Piscina* piscina)
{
    s32 x = ((s32)fons->latitudo - latus_fontis) / II;
    s32 y = ((s32)fons->altitudo - latus_fontis) / II;

    si (latera == latus_fontis && x == ZEPHYRUM && y == ZEPHYRUM)
    {
        redde *fons;
    }
    redde imago_extrahere_et_scalare(fons, (i32)x, (i32)y,
                                     (i32)latus_fontis,
                                     (i32)latus_fontis, (i32)latera,
                                     (i32)latera, IMAGO_SCALA_AREA,
                                     piscina);
}

b32
icones_reddere (
    constans Icones* petitio,
      IconesFructus* fructus,
       IconesStatus* status,
             chorda* sedes_vitii,
            Piscina* piscina)
{
    chorda vacua;
    chorda cache[X];        /* octeti per ORDINEM, communes geminis */
       b32 habet[X];
       i32 petita;
       s32 latus;
       i32 i;
       i32 j;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status || !sedes_vitii)
    {
        redde FALSUM;
    }
    *status       = ICONES_SUCCESSUS;
    *sedes_vitii  = vacua;

    si (!petitio || !fructus || !piscina)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        vacua);
    }
    si (chorda_vacua(petitio->titulus))
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        chorda_ex_literis("titulus", piscina));
    }
    si (   !petitio->fons || !petitio->fons->pixela
        || petitio->fons->latitudo <= (i32)0
        || petitio->fons->altitudo <= (i32)0)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_FONS,
                        chorda_ex_literis("fons", piscina));
    }

    latus = ((s32)petitio->fons->latitudo
             < (s32)petitio->fons->altitudo)
        ? (s32)petitio->fons->latitudo : (s32)petitio->fons->altitudo;

    si (latus < XVI)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_MINIMUS,
                        chorda_ex_s64((s64)latus, piscina));
    }

    petita = (petitio->latera_petita == ZEPHYRUM)
        ? (i32)ICONES_SETUM_APPLE : petitio->latera_petita;
    si ((petita & ~((i32)ICONES_SETUM_APPLE)) != ZEPHYRUM)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_LATERA,
                        chorda_ex_s64((s64)petita, piscina));
    }

    fructus->partes         = xar_creare(piscina,
        (i32)magnitudo(IconesPars));
    fructus->omissa         = ZEPHYRUM;
    fructus->latera_fontis  = (i32)latus;
    si (!fructus->partes)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                        vacua);
    }

    per (i = ZEPHYRUM; i < X; i++)
    {
        cache[i] = vacua;
        habet[i] = FALSUM;
    }

    per (i = ZEPHYRUM; i < X; i++)
    {
         IconesPars* pars;
                b32  iam = FALSUM;

        si ((petita & ORDINES[i].vexillum) == ZEPHYRUM)
        {
            perge;
        }
        si (ORDINES[i].latera > (i32)latus)
        {
            fructus->omissa |= ORDINES[i].vexillum;
            perge;
        }

        /* octeti per LATUS communes: ordinem priorem eiusdem lateris
         * quaerimus antequam iterum codificemus */
        per (j = ZEPHYRUM; j < i; j++)
        {
            si (habet[j] && ORDINES[j].latera == ORDINES[i].latera)
            {
                cache[i]  = cache[j];
                habet[i]  = VERUM;
                iam       = VERUM;
                frange;
            }
        }
        si (!iam)
        {
                  Imago parva;
             PngFructus png;

            parva = _quadratum_scalare(petitio->fons, latus,
                                       (s32)ORDINES[i].latera,
                                       piscina);
            si (parva.pixela == NIHIL)
            {
                redde _recusare(status, sedes_vitii,
                                ICONES_ERROR_MEMORIA, vacua);
            }
            png = imago_png_codificare(&parva, piscina);
            si (!png.successus)
            {
                /* nuntius codificatoris IPSIUS, non meus generalis */
                redde _recusare(status, sedes_vitii, ICONES_ERROR_PNG,
                                png.error);
            }
            cache[i].datum    = png.datum;
            cache[i].mensura  = png.mensura;
            habet[i]          = VERUM;
        }

        pars = (IconesPars*)xar_addere(fructus->partes);
        si (!pars)
        {
            redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                            vacua);
        }
        pars->latera = ORDINES[i].latera;
        pars->semita = chorda_ex_literis(ORDINES[i].semita, piscina);
        pars->octeti = cache[i];
    }
    redde VERUM;
}
