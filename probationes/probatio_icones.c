/* probatio_icones.c - nucleus purus, continens, recusationes, oracula
 *
 * Portae I1-I7 (project-specs/icones-spec.md par. VIII). Quaestio quam
 * nulla suita respondet - an Finder iconem XVI px PNG in ic04 REDDAT -
 * in tools/icones_fumus.sh manu vivit.
 */

#include "latina.h"
#include "icones.h"
#include "filum.h"
#include "credo.h"
#include "piscina.h"

#include <stdio.h>
#include <string.h>

interior Imago
_fingere (
        s32  latera,
    Piscina* piscina)
{
    Imago im;
      s32  x, y;

    im.latitudo = (i32)latera;
    im.altitudo = (i32)latera;
    im.pixela    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(latera * latera * IV));
    si (im.pixela == NIHIL)
    {
        redde im;
    }

    /* Gradus per diagonalem: quadratum uniforme deminutionem non
     * probaret, quia media cuiusque areae eadem esset. */
    per (y = ZEPHYRUM; y < latera; y++)
    {
        per (x = ZEPHYRUM; x < latera; x++)
        {
            i8* p = im.pixela + (((y * latera) + x) * IV);

            p[ZEPHYRUM]  = (i8)((x * 255) / latera);
            p[I]         = (i8)((y * 255) / latera);
            p[II]        = (i8)64;
            p[III]       = (i8)255;
        }
    }
    redde im;
}

interior IconesPars*
_partem_invenire (
      constans Xar* partes,
constans character* semita)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(partes); i++)
    {
        IconesPars* p = (IconesPars*)xar_obtinere(partes, i);

        si (p && chorda_aequalis_literis(p->semita, semita))
        {
            redde p;
        }
    }
    redde NIHIL;
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_icones", 16777216);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);

    /* ---- I1: nucleus PURUS ---- */
    {
               Icones  petitio;
        IconesFructus  fructus;
         IconesStatus  status;
               chorda  sedes;
                Imago  fons;
           IconesPars* a;
           IconesPars* b;

        imprimere("\n--- I1: nucleus purus ---\n");

        fons = _fingere(CCLVI, piscina);
        CREDO_NON_NIHIL(fons.pixela);

        memset(&petitio, ZEPHYRUM, magnitudo(Icones));
        petitio.fons      = &fons;
        petitio.titulus   = chorda_ex_literis("AppIcon", piscina);
        petitio.recidere  = ICONES_RECIDERE_CENTRUM;

        CREDO_VERUM(icones_reddere(&petitio, &fructus, &status, &sedes,
                                   piscina));
        CREDO_AEQUALIS_I32((i32)status, (i32)ICONES_SUCCESSUS);
        CREDO_AEQUALIS_I32(fructus.latera_fontis, CCLVI);

        /* Fons CCLVI tegit XVI, XXXII, LXIV, CXXVIII, CCLVI. Plagulae
         * per tabulam: icon_16x16, icon_16x16@2x, icon_32x32,
         * icon_32x32@2x, icon_128x128, icon_128x128@2x, icon_256x256 =
         * SEPTEM (XXXII et CCLVI bis apparent). DXII et MXXIV omissa.
         * Numerus enumeratus, non coniectus: primum VI scripsi et
         * unam plagulam capite computando amisi. */
        CREDO_AEQUALIS_I32(xar_numerus(fructus.partes), VII);
        CREDO_AEQUALIS_I32(fructus.omissa,
            (i32)(ICONES_LATERA_DXII | ICONES_LATERA_MXXIV));

        /* nomina Apple */
        CREDO_NON_NIHIL(_partem_invenire(fructus.partes,
                                         "icon_16x16.png"));
        CREDO_NON_NIHIL(_partem_invenire(fructus.partes,
                                         "icon_128x128@2x.png"));
        CREDO_NIHIL(_partem_invenire(fructus.partes,
                                     "icon_512x512@2x.png"));

        /* OCTETI COMMUNES: icon_16x16@2x et icon_32x32 eadem imago
         * XXXII px sunt, ergo eundem indicem ferre DEBENT - id probat
         * nos septem vicibus codificare, non decem, et cadit clare si
         * quis postea singulas plagulas separatim codificet. */
        a = _partem_invenire(fructus.partes, "icon_16x16@2x.png");
        b = _partem_invenire(fructus.partes, "icon_32x32.png");
        CREDO_NON_NIHIL(a);
        CREDO_NON_NIHIL(b);
        CREDO_AEQUALIS_I32(a->latera, XXXII);
        CREDO_AEQUALIS_I32(b->latera, XXXII);
        CREDO_VERUM(a->octeti.datum == b->octeti.datum);
        CREDO_CHORDA_NON_VACUA(a->octeti);

        a = _partem_invenire(fructus.partes, "icon_128x128@2x.png");
        b = _partem_invenire(fructus.partes, "icon_256x256.png");
        CREDO_NON_NIHIL(a);
        CREDO_NON_NIHIL(b);
        CREDO_VERUM(a->octeti.datum == b->octeti.datum);

        /* PNG verus: signum octetorum VIII */
        a = _partem_invenire(fructus.partes, "icon_16x16.png");
        CREDO_NON_NIHIL(a);
        CREDO_VERUM(a->octeti.mensura > (i32)8);
        {
            constans i8* sig = a->octeti.datum;

            CREDO_AEQUALIS_I32((i32)sig[I], (i32)'P');
            CREDO_AEQUALIS_I32((i32)sig[II], (i32)'N');
            CREDO_AEQUALIS_I32((i32)sig[III], (i32)'G');
        }

        /* PURITAS: nihil in disco */
        CREDO_FALSUM(filum_directorium_existit("AppIcon.iconset"));
        CREDO_FALSUM(filum_existit("icon_16x16.png"));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
