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

/* Area virginis NIDIFICATA: nec area nec nidus ante scriptorem
 * exsistit, ergo 'directorium creatum est' aliquid probat. */
#define VIA_AREAE    "/tmp/probatio_icones_area"
#define VIA_RADICIS  VIA_AREAE "/nidus"
#define VIA_ICONSET  VIA_RADICIS "/AppIcon.iconset"
#define VIA_ICNS     VIA_RADICIS "/AppIcon.icns"

/* Nomina Apple MANU scripta, non ex ORDINIBUS bibliothecae lecta:
 * oraculum a tabula quam probat independens. */
hic_manens constans character* PLAGULAE_APPLE[X] = {
    "icon_16x16.png",      "icon_16x16@2x.png",
    "icon_32x32.png",      "icon_32x32@2x.png",
    "icon_128x128.png",    "icon_128x128@2x.png",
    "icon_256x256.png",    "icon_256x256@2x.png",
    "icon_512x512.png",    "icon_512x512@2x.png"
};

/* \x89 P N G \r \n \x1a \n */
hic_manens constans i8 SIGNUM_PNG[VIII] = {
    (i8)CXXXVII, (i8)LXXX, (i8)LXXVIII, (i8)LXXI,
    (i8)XIII,    (i8)X,    (i8)XXVI,    (i8)X
};

/* Longitudo magni-endiana octetum post octetum legitur: numquam
 * memcpy in i32 hospitis, qui in machina parvi-endiana inversus
 * esset. */
interior i32
_be32_legere (
    constans i8* p)
{
    redde ((i32)p[ZEPHYRUM] << XXIV) | ((i32)p[I] << XVI)
         | ((i32)p[II] << VIII) | (i32)p[III];
}

/* memcmp EXPLICITE: PNG octetum NULLUM ad sedem VIII fert (longitudo
 * IHDR = 00 00 00 0D), ergo comparatio quae ad NULLUM consisteret
 * signum solum iudicaret et viride mentiretur. */
interior b32
_octeti_aequales (
    chorda a,
    chorda b)
{
    si (   a.datum   == NIHIL || b.datum == NIHIL
        || a.mensura != b.mensura)
    {
        redde FALSUM;
    }
    redde (memcmp(a.datum, b.datum, (size_t)a.mensura) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

/* Profundissima PRIMUM: filum_delere remove() est et directoria
 * VACUA sola tollit. Ante scriptorem vocatur, ne directoria cursus
 * prioris 'scriptor ea creavit' viride mendax faciant. */
interior vacuum
_aream_purgare (vacuum)
{
    character via[CCLVI];
          i32 i;

    per (i = ZEPHYRUM; i < X; i++)
    {
        sprintf(via, "%.200s/%.40s", VIA_ICONSET, PLAGULAE_APPLE[i]);
        filum_delere(via);
    }
    filum_delere(VIA_ICONSET);
    filum_delere(VIA_ICNS);
    filum_delere(VIA_RADICIS);
    filum_delere(VIA_AREAE);
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

    /* ---- I3: continens .icns PURUS ---- */
    {
               Icones petitio;
        IconesFructus fructus;
         IconesStatus status;
               chorda sedes;
               chorda icns;
               chorda absens;
                Imago fons;
               chorda codices[VIII];
               chorda onera[VIII];
                  i32 positio;
                  i32 longitudo;
                  i32 numerus;
                  i32 i;

        imprimere("\n--- I3: continens .icns purus ---\n");

        fons = _fingere(LXIV, piscina);
        CREDO_NON_NIHIL(fons.pixela);

        memset(&petitio, ZEPHYRUM, magnitudo(Icones));
        petitio.fons      = &fons;
        petitio.titulus   = chorda_ex_literis("AppIcon", piscina);
        petitio.recidere  = ICONES_RECIDERE_CENTRUM;

        /* Fons LXIV: icon_16x16 (ic04), icon_16x16@2x (ic11),
         * icon_32x32 (ic05), icon_32x32@2x (ic12) = IV chunki */
        CREDO_VERUM(icones_reddere(&petitio, &fructus, &status, &sedes,
                                   piscina));
        CREDO_AEQUALIS_I32(xar_numerus(fructus.partes), IV);

        icns = icones_icns_codificare(&fructus, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)ICONES_SUCCESSUS);

        /* 'absens' legibile in rubro, non chorda NIHIL */
        absens = chorda_ex_literis("absens", piscina);
        per (i = ZEPHYRUM; i < VIII; i++)
        {
            codices[i]        = absens;
            onera[i].datum    = NIHIL;
            onera[i].mensura  = ZEPHYRUM;
        }
        positio = (i32)VIII;
        numerus = ZEPHYRUM;

        si (icns.datum == NIHIL || icns.mensura < (i32)VIII)
        {
            CREDO_CULPA("continens vacuus: nihil ambulandum");
        }
        alioquin
        {
            CREDO_VERUM(memcmp(icns.datum, "icns", (size_t)IV)
                        == ZEPHYRUM);
            /* longitudo DECLARATA == longitudo VERA */
            CREDO_AEQUALIS_I32(_be32_legere(icns.datum + IV),
                               icns.mensura);

            /* Ambulatio per tabulam chunkorum. Custodiae ANTE quamque
             * lectionem: longitudo < VIII aut ultra finem ambulationem
             * FRANGIT, et assertiones post gyrum id clamant - numquam
             * SIGSEGV, numquam gyrus infinitus. */
            dum (   numerus < (i32)VIII
                 && icns.mensura - positio >= (i32)VIII)
            {
                longitudo = _be32_legere(icns.datum + positio + IV);
                si (   longitudo < (i32)VIII
                    || longitudo > icns.mensura - positio)
                {
                    frange;
                }
                codices[numerus] = chorda_ex_buffer(
                    icns.datum + positio, (i32)IV);
                onera[numerus] = chorda_ex_buffer(
                    icns.datum + positio + VIII,
                    longitudo - (i32)VIII);
                positio += longitudo;
                numerus++;
            }
        }

        /* AD FINEM EXACTE: nec ante consistit, nec ultra legit */
        CREDO_AEQUALIS_I32(positio, icns.mensura);
        CREDO_AEQUALIS_I32(numerus, IV);

        /* codices in ORDINE tabulae */
        CREDO_CHORDA_AEQUALIS_LITERIS(codices[ZEPHYRUM], "ic04");
        CREDO_CHORDA_AEQUALIS_LITERIS(codices[I], "ic11");
        CREDO_CHORDA_AEQUALIS_LITERIS(codices[II], "ic05");
        CREDO_CHORDA_AEQUALIS_LITERIS(codices[III], "ic12");

        /* onus quodque PNG verum, et IIDEM octeti partis suae (D6 in
         * continente, non solum in partibus) */
        per (i = ZEPHYRUM; i < IV; i++)
        {
            IconesPars* p = (IconesPars*)xar_obtinere(fructus.partes,
                i);

            CREDO_VERUM(   onera[i].mensura > (i32)VIII
                        && memcmp(onera[i].datum, SIGNUM_PNG,
                                  (size_t)VIII) == ZEPHYRUM);
            CREDO_VERUM(   p != NIHIL
                        && _octeti_aequales(onera[i], p->octeti));
        }
        /* par geminum XXXII px (ic11, ic05) octetim aequale */
        CREDO_VERUM(_octeti_aequales(onera[I], onera[II]));
    }

    /* ---- I6: scriptores in area virgine NIDIFICATA ---- */
    {
               Icones petitio;
        IconesFructus fructus;
         IconesStatus status;
               chorda sedes;
               chorda icns_pura;
               chorda lectum;
               chorda alterum;
                Imago fons;
            character via[CCLVI];
                  i32 i;

        imprimere("\n--- I6: scriptores ---\n");

        _aream_purgare();
        CREDO_FALSUM(filum_directorium_existit(VIA_AREAE));

        /* Fons DXII, non MXXIV: MXXIV solus circa XIII MB piscinae
         * consumeret. DXII NOVEM plagulas reddit et PARIA GEMINA DUO
         * attingit (XXXII et DXII px) - CCLVI unum solum. */
        fons = _fingere(DXII, piscina);
        CREDO_NON_NIHIL(fons.pixela);

        memset(&petitio, ZEPHYRUM, magnitudo(Icones));
        petitio.fons      = &fons;
        petitio.titulus   = chorda_ex_literis("AppIcon", piscina);
        petitio.recidere  = ICONES_RECIDERE_CENTRUM;

        CREDO_VERUM(icones_reddere(&petitio, &fructus, &status, &sedes,
                                   piscina));
        CREDO_AEQUALIS_I32(xar_numerus(fructus.partes), IX);
        CREDO_AEQUALIS_I32(fructus.omissa, (i32)ICONES_LATERA_MXXIV);

        /* .iconset: radix ET nidus absunt, ergo mkdir -p requiritur */
        CREDO_VERUM(icones_iconset_scribere(&fructus, VIA_RADICIS,
                                            &status, &sedes, piscina));
        CREDO_AEQUALIS_I32((i32)status, (i32)ICONES_SUCCESSUS);
        CREDO_VERUM(filum_directorium_existit(VIA_ICONSET));

        /* plagula QUAEQUE octetos partis SUAE fert, non solum exsistit.
         * Paria gemina sola non sufficiunt: scriptor qui octetos partis
         * primae ubique scriberet ea aequalia servaret. */
        per (i = ZEPHYRUM; i < xar_numerus(fructus.partes); i++)
        {
            IconesPars* p = (IconesPars*)xar_obtinere(fructus.partes,
                i);

            si (p == NIHIL)
            {
                CREDO_CULPA("pars NIHIL");
                perge;
            }
            sprintf(via, "%.200s/%.40s", VIA_ICONSET,
                    chorda_ut_cstr(p->semita, piscina));
            lectum = filum_legere_totum(via, piscina);
            CREDO_VERUM(_octeti_aequales(lectum, p->octeti));
        }

        /* paria gemina OCTETIM aequalia IN DISCO */
        lectum   = filum_legere_totum(VIA_ICONSET "/icon_16x16@2x.png",
                                      piscina);
        alterum  = filum_legere_totum(VIA_ICONSET "/icon_32x32.png",
                                      piscina);
        CREDO_CHORDA_NON_VACUA(lectum);
        CREDO_VERUM(_octeti_aequales(lectum, alterum));

        lectum   =
            filum_legere_totum(VIA_ICONSET "/icon_256x256@2x.png",
                                      piscina);
        alterum  = filum_legere_totum(VIA_ICONSET "/icon_512x512.png",
                                      piscina);
        CREDO_CHORDA_NON_VACUA(lectum);
        CREDO_VERUM(_octeti_aequales(lectum, alterum));

        /* MXXIV omissum: plagula eius NON scripta */
        CREDO_FALSUM(filum_existit(VIA_ICONSET "/icon_512x512@2x.png"));

        /* .icns: plagula == codificatio PURA, octetim */
        icns_pura = icones_icns_codificare(&fructus, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)ICONES_SUCCESSUS);
        CREDO_CHORDA_NON_VACUA(icns_pura);

        CREDO_VERUM(icones_icns_scribere(&fructus, VIA_ICNS, &status,
                                         &sedes, piscina));
        CREDO_AEQUALIS_I32((i32)status, (i32)ICONES_SUCCESSUS);
        lectum = filum_legere_totum(VIA_ICNS, piscina);
        CREDO_VERUM(_octeti_aequales(lectum, icns_pura));

        _aream_purgare();
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
