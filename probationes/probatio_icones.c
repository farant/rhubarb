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
#include "imago.h"
#include "imago_opus.h"
#include "imago_collatio.h"

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

/* Area recusationum SEPARATA ab area I6, ne purgatio unius alteram
 * tangat. */
#define VIA_RECUSATA        "/tmp/probatio_icones_recusata"
#define VIA_RECUSATA_ICNS   VIA_RECUSATA "/AppIcon.icns"
#define VIA_RECUSATA_SET    VIA_RECUSATA "/AppIcon.iconset"
#define VIA_PLAGULA_PARENS  VIA_RECUSATA "/plagula"

/* Recusatio TOTA uno versu: redditum FALSUM, status SPECIFICUS, sedes
 * SPECIFICA. MACRO, non functio, ut __LINE__ lineam CASUS nominet -
 * aliter rubra omnia lineam adiutoris dicerent. Variabiles 'status' et
 * 'sedes' in ambitu vocantis esse debent. */
#define RECUSATIO(redditum, sperata, sedes_sperata)              \
    fac {                                                         \
        CREDO_FALSUM(redditum);                                   \
        CREDO_AEQUALIS_I32((i32)status, (i32)(sperata));          \
        CREDO_CHORDA_AEQUALIS_LITERIS(sedes, (sedes_sperata));    \
    } dum (FALSUM)

/* Fructus MANU factus, non per icones_reddere: partem unam fert cum
 * semita et octetis datis - forma quam scriptores iudicare debent. */
interior IconesFructus
_fructum_fingere (
    constans character* semita,
                chorda  octeti,
               Piscina* piscina)
{
    IconesFructus  f;
       IconesPars* p;

    memset(&f, ZEPHYRUM, magnitudo(IconesFructus));
    f.titulus  = chorda_ex_literis("AppIcon", piscina);
    f.partes   = xar_creare(piscina, (i32)magnitudo(IconesPars));
    p          = (IconesPars*)xar_addere(f.partes);
    si (p != NIHIL)
    {
        p->latera = XVI;
        p->semita = chorda_ex_literis(semita, piscina);
        p->octeti = octeti;
    }
    redde f;
}

/* Profundissima PRIMUM: filum_delere remove() est, directoria VACUA
 * sola tollit. Plagula prima iconset DIRECTORIUM esse potest (casus
 * SCRIPTIO). */
interior vacuum
_recusatam_purgare (vacuum)
{
    character via[CCLVI];
          i32 i;

    per (i = ZEPHYRUM; i < X; i++)
    {
        sprintf(via, "%.200s/%.40s", VIA_RECUSATA_SET,
            PLAGULAE_APPLE[i]);
        filum_delere(via);
    }
    filum_delere(VIA_RECUSATA_SET);
    filum_delere(VIA_RECUSATA "/.iconset");
    filum_delere(VIA_RECUSATA_ICNS);
    filum_delere(VIA_PLAGULA_PARENS);
    filum_delere(VIA_RECUSATA);
}

/* I7: limites MENSURATI 2026-09-12 contra sips_16.png congelatum,
 * INTERIOR tantum (pixela in utraque imagine plene opaca). Mensurae:
 * area >XVI nulla, media 2.26; bilinearis LXXV, 21.13; proximus CXII,
 * 93.41. Limites largi - area bis et semis infra mediam, bilinearis
 * ter et semis supra - ergo 'modus falsus per errorem' capitur, non
 * 'filtrum nostrum ab Apple differt'. */
#define LIMES_DIVERSA  VIII
#define LIMES_MEDIA    600      /* media per canalem, x C */

/* COMPARATIO INTERIOR. In margine disci filtra LEGITIME dissentiunt
 * (sips alpha latius spargit: pixelum (2,4) sips 111, nostrum 224),
 * et in RGBA recto margo omnes numeros dominabatur - delta maxima area
 * 224, bilinearis 255, indiscreta. Interior modum discernit. Marginem
 * alpha I2 in imago_opus pinnis exactis tenet.
 *
 * Numerus per imago_conferre super copias LARVATAS (pixela non
 * interiora in utraque ad nihilum posita): definitio 'differt'
 * bibliothecae ipsa, non nova. Media hic computatur, quia collatio
 * mediam non fert. 'intus' redditur ut vocans comparationem NON VACUAM
 * asserere possit - larva nimia 'nulla differentia' daret. */
interior vacuum
_interior_conferre (
    constans Imago* a,
    constans Imago* b,
               i32  tolerantia,
               i32* diversa,
               i32* media,
               i32* intus,
           Piscina* piscina)
{
               Imago la;
               Imago lb;
      CollatioRegula regula;
     CollatioFructus collatio;
                 i32 n;
                 i32 i;
                 i32 ch;
                 s64 summa = ZEPHYRUM;

    *diversa  = ZEPHYRUM;
    *media    = ZEPHYRUM;
    *intus    = ZEPHYRUM;
    si (   !a->pixela || !b->pixela
        || a->latitudo != b->latitudo || a->altitudo != b->altitudo)
    {
        *diversa  = MMMMXCVI;
        *media    = MMMMXCVI;
        redde;
    }
    n   = a->latitudo * a->altitudo;
    la  = *a;
    lb  = *b;
    la.pixela = (i8*)piscina_allocare(piscina,
        (memoriae_index)(n * IV));
    lb.pixela = (i8*)piscina_allocare(piscina,
        (memoriae_index)(n * IV));
    si (!la.pixela || !lb.pixela)
    {
        *diversa  = MMMMXCVI;
        *media    = MMMMXCVI;
        redde;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans i8* pa = a->pixela + (i * IV);
        constans i8* pb = b->pixela + (i * IV);

        si (pa[III] == 255 && pb[III] == 255)
        {
            memcpy(la.pixela + (i * IV), pa, (size_t)IV);
            memcpy(lb.pixela + (i * IV), pb, (size_t)IV);
            (*intus)++;
            per (ch = ZEPHYRUM; ch < III; ch++)
            {
                summa += (pa[ch] > pb[ch]) ? (s64)(pa[ch] - pb[ch])
                                           : (s64)(pb[ch] - pa[ch]);
            }
        }
        alioquin
        {
            memset(la.pixela + (i * IV), ZEPHYRUM, (size_t)IV);
            memset(lb.pixela + (i * IV), ZEPHYRUM, (size_t)IV);
        }
    }
    memset(&regula, ZEPHYRUM, magnitudo(CollatioRegula));
    regula.tolerantia  = tolerantia;
    collatio           = imago_conferre(&la, &lb, regula, piscina);
    *diversa           = collatio.pixela_diversa;
    *media    = (*intus > ZEPHYRUM)
        ? (i32)((summa * C) / ((s64)*intus * III)) : ZEPHYRUM;
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

    /* ---- I5: recusationes, quaeque statu SUO et sede SUA ---- */
    {
               Icones bona;
               Icones p;
        IconesFructus fructus;
        IconesFructus f;
         IconesStatus status;
               chorda sedes;
               chorda icns;
               chorda octeti;
               chorda vacua;
                Imago fons;
                Imago parva;
                Imago mala;

        imprimere("\n--- I5: recusationes ---\n");

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        octeti         = chorda_ex_literis("octeti", piscina);

        fons = _fingere(LXIV, piscina);
        CREDO_NON_NIHIL(fons.pixela);
        memset(&bona, ZEPHYRUM, magnitudo(Icones));
        bona.fons      = &fons;
        bona.titulus   = chorda_ex_literis("AppIcon", piscina);
        bona.recidere  = ICONES_RECIDERE_CENTRUM;

        /* ======== icones_reddere ======== */

        /* sine canali nihil nominari potest: FALSUM, non ruina */
        CREDO_FALSUM(icones_reddere(&bona, &fructus, NIHIL, &sedes,
                                    piscina));
        CREDO_FALSUM(icones_reddere(&bona, &fructus, &status, NIHIL,
                                    piscina));

        RECUSATIO(icones_reddere(NIHIL, &fructus, &status, &sedes,
                                 piscina),
                  ICONES_ERROR_DESUNT, "petitio");
        RECUSATIO(icones_reddere(&bona, NIHIL, &status, &sedes,
            piscina),
                  ICONES_ERROR_DESUNT, "fructus");
        /* piscina ABSENS quoque nominatur: sedes in memoria statica */
        RECUSATIO(icones_reddere(&bona, &fructus, &status, &sedes,
            NIHIL),
                  ICONES_ERROR_DESUNT, "piscina");

        p          = bona;
        p.titulus  = vacua;
        RECUSATIO(icones_reddere(&p, &fructus, &status, &sedes,
            piscina),
                  ICONES_ERROR_DESUNT, "titulus");

        /* fons ABSENS = DESUNT (caput: 'fons aut titulus'); fons
         * PRAESENS sed vitiosus = FONS, campo nominato */
        p       = bona;
        p.fons  = NIHIL;
        RECUSATIO(icones_reddere(&p, &fructus, &status, &sedes,
            piscina),
                  ICONES_ERROR_DESUNT, "fons");

        mala         = fons;
        mala.pixela  = NIHIL;
        p.fons       = &mala;
        RECUSATIO(icones_reddere(&p, &fructus, &status, &sedes,
            piscina),
                  ICONES_ERROR_FONS, "pixela");

        mala           = fons;
        mala.latitudo  = ZEPHYRUM;
        RECUSATIO(icones_reddere(&p, &fructus, &status, &sedes,
            piscina),
                  ICONES_ERROR_FONS, "latitudo");

        mala           = fons;
        mala.altitudo  = ZEPHYRUM;
        RECUSATIO(icones_reddere(&p, &fructus, &status, &sedes,
            piscina),
                  ICONES_ERROR_FONS, "altitudo");

        /* latus < XVI: sedes LATUS nominat */
        parva = _fingere(XII, piscina);
        CREDO_NON_NIHIL(parva.pixela);
        p       = bona;
        p.fons  = &parva;
        RECUSATIO(icones_reddere(&p, &fructus, &status, &sedes,
            piscina),
                  ICONES_ERROR_MINIMUS, "12");

        /* bit IGNOTUM: valor CXXVIII bit EXTRA setum est - NON
         * ICONES_LATERA_CXXVIII, quod VIII valet. Sedes bita ignota
         * SOLA nominat, non petitionem totam (quae 129 esset). */
        p                = bona;
        p.latera_petita  = (i32)ICONES_LATERA_XVI | (i32)CXXVIII;
        RECUSATIO(icones_reddere(&p, &fructus, &status, &sedes,
            piscina),
                  ICONES_ERROR_LATERA, "128");

        /* D10 LATIUS: latus PETITUM nullum a fonte LXIV tegitur. Olim
         * VERUM cum partibus nullis (inventum operis III). */
        p.latera_petita = (i32)(ICONES_LATERA_CXXVIII
                                | ICONES_LATERA_MXXIV);
        RECUSATIO(icones_reddere(&p, &fructus, &status, &sedes,
            piscina),
                  ICONES_ERROR_MINIMUS, "128 1024");

        /* gemellus POSITIVUS: latus UNUM tectum = successus cum omissis
         * (D2). Sine eo recusatio quae QUAMLIBET omissionem recusaret
         * ab hac discerni non posset. Sedes post successum PURGATA:
         * nulla recusatio prior eam inquinat. */
        p.latera_petita = (i32)(ICONES_LATERA_XVI
            | ICONES_LATERA_MXXIV);
        CREDO_VERUM(icones_reddere(&p, &fructus, &status, &sedes,
                                   piscina));
        CREDO_AEQUALIS_I32((i32)status, (i32)ICONES_SUCCESSUS);
        CREDO_CHORDA_VACUA(sedes);
        CREDO_AEQUALIS_I32(xar_numerus(fructus.partes), I);
        CREDO_AEQUALIS_I32(fructus.omissa, (i32)ICONES_LATERA_MXXIV);

        /* ======== scriptores: fructus bonus, IV partes ======== */

        CREDO_VERUM(icones_reddere(&bona, &fructus, &status, &sedes,
                                   piscina));
        CREDO_AEQUALIS_I32(xar_numerus(fructus.partes), IV);

        _recusatam_purgare();
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(
            VIA_RECUSATA));

        /* ---- iudex fructus, per icones_icns_scribere (sedem fert).
         * Area SCRIBILIS est (gemellus positivus infra), ergo 'plagula
         * non scripta' recusationem ANTE scriptionem probat, non
         * scriptionem impossibilem. ---- */
        RECUSATIO(icones_icns_scribere(NIHIL, VIA_RECUSATA_ICNS,
            &status,
                                       &sedes, piscina),
                  ICONES_ERROR_DESUNT, "fructus");

        memset(&f, ZEPHYRUM, magnitudo(IconesFructus));
        RECUSATIO(icones_icns_scribere(&f, VIA_RECUSATA_ICNS, &status,
                                       &sedes, piscina),
                  ICONES_ERROR_DESUNT, "partes");
        f.partes = xar_creare(piscina, (i32)magnitudo(IconesPars));
        RECUSATIO(icones_icns_scribere(&f, VIA_RECUSATA_ICNS, &status,
                                       &sedes, piscina),
                  ICONES_ERROR_DESUNT, "partes");

        f = _fructum_fingere("icon_17x17.png", octeti, piscina);
        RECUSATIO(icones_icns_scribere(&f, VIA_RECUSATA_ICNS, &status,
                                       &sedes, piscina),
                  ICONES_ERROR_LATERA, "icon_17x17.png");

        /* octeti VACUI: chunkus sine onere = icon inanis sine errore */
        f = _fructum_fingere("icon_16x16.png", vacua, piscina);
        RECUSATIO(icones_icns_scribere(&f, VIA_RECUSATA_ICNS, &status,
                                       &sedes, piscina),
                  ICONES_ERROR_DESUNT, "icon_16x16.png");

        CREDO_FALSUM(filum_existit(VIA_RECUSATA_ICNS));

        /* ---- icones_icns_scribere: argumenta et scriptio ---- */
        CREDO_FALSUM(icones_icns_scribere(&fructus, VIA_RECUSATA_ICNS,
                                          NIHIL, &sedes, piscina));
        CREDO_FALSUM(icones_icns_scribere(&fructus, VIA_RECUSATA_ICNS,
                                          &status, NIHIL, piscina));
        RECUSATIO(icones_icns_scribere(&fructus, NIHIL, &status, &sedes,
                                       piscina),
                  ICONES_ERROR_DESUNT, "via");
        RECUSATIO(icones_icns_scribere(&fructus, VIA_RECUSATA_ICNS,
                                       &status, &sedes, NIHIL),
                  ICONES_ERROR_DESUNT, "piscina");
        CREDO_FALSUM(filum_existit(VIA_RECUSATA_ICNS));

        /* via DIRECTORIUM est: fopen deficit, via ipsa nominatur */
        RECUSATIO(icones_icns_scribere(&fructus, VIA_RECUSATA, &status,
                                       &sedes, piscina),
                  ICONES_ERROR_SCRIPTIO, VIA_RECUSATA);

        /* gemellus POSITIVUS: eadem area SCRIBILIS est */
        CREDO_VERUM(icones_icns_scribere(&fructus, VIA_RECUSATA_ICNS,
                                         &status, &sedes, piscina));
        CREDO_VERUM(filum_existit(VIA_RECUSATA_ICNS));

        /* ---- icones_icns_codificare: sine sede, status et chorda
         * vacua ---- */
        icns = icones_icns_codificare(&fructus, NIHIL, piscina);
        CREDO_CHORDA_VACUA(icns);

        status  = ICONES_SUCCESSUS;
        icns    = icones_icns_codificare(&fructus, &status, NIHIL);
        CREDO_CHORDA_VACUA(icns);
        CREDO_AEQUALIS_I32((i32)status, (i32)ICONES_ERROR_DESUNT);

        /* iudicem VOCAT: pars ignota = LATERA, non continens */
        f     = _fructum_fingere("icon_17x17.png", octeti, piscina);
        icns  = icones_icns_codificare(&f, &status, piscina);
        CREDO_CHORDA_VACUA(icns);
        CREDO_AEQUALIS_I32((i32)status, (i32)ICONES_ERROR_LATERA);

        /* ---- icones_iconset_scribere ---- */
        CREDO_FALSUM(icones_iconset_scribere(&fructus, VIA_RECUSATA,
                                             NIHIL, &sedes, piscina));
        CREDO_FALSUM(icones_iconset_scribere(&fructus, VIA_RECUSATA,
                                             &status, NIHIL, piscina));
        RECUSATIO(icones_iconset_scribere(&fructus, NIHIL, &status,
                                          &sedes, piscina),
                  ICONES_ERROR_DESUNT, "via_radicis");
        RECUSATIO(icones_iconset_scribere(&fructus, VIA_RECUSATA,
                                          &status, &sedes, NIHIL),
                  ICONES_ERROR_DESUNT, "piscina");

        /* iudicem VOCAT, ANTE mkdir: partes nullae nihil creant */
        f.partes = xar_creare(piscina, (i32)magnitudo(IconesPars));
        RECUSATIO(icones_iconset_scribere(&f, VIA_RECUSATA, &status,
                                          &sedes, piscina),
                  ICONES_ERROR_DESUNT, "partes");

        /* SEMITA VACUA: LATERA, non SCRIPTIO. via_iungere partem vacuam
         * praeterit, ergo sine iudice scriptor in directorium ipsum
         * scribere conaretur (inventum operis III). Sedes vacua est,
         * quia id quod nominat vacuum est. */
        f = _fructum_fingere("", octeti, piscina);
        CREDO_FALSUM(icones_iconset_scribere(&f, VIA_RECUSATA, &status,
                                             &sedes, piscina));
        CREDO_AEQUALIS_I32((i32)status, (i32)ICONES_ERROR_LATERA);
        CREDO_CHORDA_VACUA(sedes);
        CREDO_FALSUM(filum_directorium_existit(VIA_RECUSATA_SET));

        /* titulus vacuus directorium OCCULTUM '.iconset' daret */
        f          = fructus;
        f.titulus  = vacua;
        RECUSATIO(icones_iconset_scribere(&f, VIA_RECUSATA, &status,
                                          &sedes, piscina),
                  ICONES_ERROR_DESUNT, "titulus");
        CREDO_FALSUM(filum_directorium_existit(
            VIA_RECUSATA "/.iconset"));

        /* radix sub PLAGULA: mkdir -p deficere debet */
        CREDO_VERUM(filum_scribere_literis(VIA_PLAGULA_PARENS,
                                           "non directorium"));
        RECUSATIO(icones_iconset_scribere(&fructus,
                                          VIA_PLAGULA_PARENS "/sub",
                                          &status, &sedes, piscina),
                  ICONES_ERROR_DIRECTORIUM, "AppIcon.iconset");

        /* plagula prima DIRECTORIO occupata: fopen deficit, semita
         * RELATIVA nominatur */
        CREDO_VERUM(filum_directorium_creare_cum_parentibus(
            VIA_RECUSATA_SET "/icon_16x16.png"));
        RECUSATIO(icones_iconset_scribere(&fructus, VIA_RECUSATA,
                                          &status, &sedes, piscina),
                  ICONES_ERROR_SCRIPTIO,
                  "AppIcon.iconset/icon_16x16.png");

        _recusatam_purgare();
    }

    /* ---- I7: qualitas contra sips CONGELATUM, interior ---- */
    {
         ImagoFructus  fons;
         ImagoFructus  sips;
         ImagoFructus  nostra;
               Icones  petitio;
        IconesFructus  fructus;
         IconesStatus  status;
               chorda  sedes;
           IconesPars* pars;
                Imago  alia;
                  i32  diversa;
                  i32  media;
                  i32  intus;

        imprimere("\n--- I7: qualitas contra sips congelatum ---\n");

        /* Fixa et oraculum: probationes/fixa/icones/generare.py. Exitus
         * sips fontis SOLIUS functio est, ergo congelari licet: codex
         * noster mutatur sine hoc fixo renovando. */
        fons = imago_caricare_ex_file(
            "probationes/fixa/icones/fons_256.png", piscina);
        sips = imago_caricare_ex_file(
            "probationes/fixa/icones/sips_16.png", piscina);
        CREDO_VERUM(fons.successus);
        CREDO_VERUM(sips.successus);

        si (!fons.successus || !sips.successus)
        {
            CREDO_CULPA("fixa icones legi non possunt (cwd = radix?)");
        }
        alioquin
        {
            memset(&petitio, ZEPHYRUM, magnitudo(Icones));
            petitio.fons = &fons.imago;
            petitio.titulus = chorda_ex_literis("AppIcon",
                piscina);
            petitio.latera_petita = ICONES_LATERA_XVI;
            CREDO_VERUM(icones_reddere(&petitio, &fructus, &status,
                &sedes,
                                       piscina));
            pars = (IconesPars*)xar_obtinere(fructus.partes, ZEPHYRUM);
            CREDO_NON_NIHIL(pars);
            nostra = imago_caricare_ex_memoria(pars->octeti.datum,
                                               pars->octeti.mensura,
                                               piscina);
            CREDO_VERUM(nostra.successus);

            _interior_conferre(&sips.imago, &nostra.imago, XVI,
                &diversa,
                               &media, &intus, piscina);
            imprimere("  icones:     intus %u, diversa %u, media %u\n",
                      intus, diversa, media);
            /* comparatio NON VACUA */
            CREDO_MAIOR_I32(intus, C);
            CREDO_MINOR_AUT_AEQUALIS_I32(diversa, LIMES_DIVERSA);
            CREDO_MINOR_AUT_AEQUALIS_I32(media, LIMES_MEDIA);

            /* CALIBRATIO IN PORTA IPSA: modi falsi limites EXCEDERE
             * debent, aliter limites nihil discernunt */
            alia = imago_scalare(&fons.imago, XVI, XVI,
                                 IMAGO_SCALA_BILINEARIS, piscina);
            _interior_conferre(&sips.imago, &alia, XVI, &diversa,
                &media,
                               &intus, piscina);
            imprimere("  bilinearis: intus %u, diversa %u, media %u\n",
                      intus, diversa, media);
            CREDO_MAIOR_I32(diversa, LIMES_DIVERSA);
            CREDO_MAIOR_I32(media, LIMES_MEDIA);

            alia = imago_scalare(&fons.imago, XVI, XVI,
                                 IMAGO_SCALA_PROXIMUS, piscina);
            _interior_conferre(&sips.imago, &alia, XVI, &diversa,
                &media,
                               &intus, piscina);
            imprimere("  proximus:   intus %u, diversa %u, media %u\n",
                      intus, diversa, media);
            CREDO_MAIOR_I32(diversa, LIMES_DIVERSA);
            CREDO_MAIOR_I32(media, LIMES_MEDIA);
        }
    }

    /* ---- I8: dimensiones EXACTAE et recisio CENTRALIS ---- */
    {
               Icones  petitio;
        IconesFructus  fructus;
         IconesStatus  status;
               chorda  sedes;
                Imago  centum;
                Imago  lata;
           IconesPars* pars;
         ImagoFructus  decodificata;
                  i32  i;
                  s32  x;
                  s32  y;
                  i32  rubra;
                  i32  caerulea;
                  i32  numerus;

        imprimere("\n--- I8: dimensiones exactae, recisio ---\n");

        /* Fons C x C: latus NON potestas binaria. Olim creator 'intra
         * limites' truncabat (M -> XV; C -> XV, XXXI, LXIII), et fixa
         * omnia priora potestates binariae erant - nihil id videbat. */
        centum = _fingere(C, piscina);
        CREDO_NON_NIHIL(centum.pixela);
        memset(&petitio, ZEPHYRUM, magnitudo(Icones));
        petitio.fons     = &centum;
        petitio.titulus  = chorda_ex_literis("AppIcon", piscina);
        CREDO_VERUM(icones_reddere(&petitio, &fructus, &status, &sedes,
                                   piscina));
        CREDO_AEQUALIS_I32(xar_numerus(fructus.partes), IV);
        per (i = ZEPHYRUM; i < xar_numerus(fructus.partes); i++)
        {
            pars = (IconesPars*)xar_obtinere(fructus.partes, i);
            si (pars == NIHIL)
            {
                CREDO_CULPA("pars NIHIL");
                perge;
            }
            /* IHDR: latitudo ad octetum XVI, altitudo ad XX */
            CREDO_AEQUALIS_I32(_be32_legere(pars->octeti.datum + XVI),
                               pars->latera);
            CREDO_AEQUALIS_I32(_be32_legere(pars->octeti.datum + XX),
                               pars->latera);
        }

        /* Fons CXX x C: margines RUBRI X columnarum utrimque, centrum
         * CAERULEUM. Recisio ad CENTRUM rubrum nullum retinet; recisio
         * ad sinistram X columnas rubras retineret. Recisio centralis
         * (D1) antea NUMQUAM probata erat: fixa omnia quadrata. */
        lata.latitudo = CXX;
        lata.altitudo = C;
        lata.pixela    = (i8*)piscina_allocare(piscina,
            (memoriae_index)(CXX * C * IV));
        CREDO_NON_NIHIL(lata.pixela);
        per (y = ZEPHYRUM; y < C; y++)
        {
            per (x = ZEPHYRUM; x < CXX; x++)
            {
                 i8* p      = lata.pixela + (((y * CXX) + x) * IV);
                b32  margo  = (x < X || x >= CX) ? VERUM : FALSUM;

                p[ZEPHYRUM]  = margo ? (i8)255 : (i8)ZEPHYRUM;
                p[I]         = (i8)ZEPHYRUM;
                p[II]        = margo ? (i8)ZEPHYRUM : (i8)255;
                p[III]       = (i8)255;
            }
        }
        memset(&petitio, ZEPHYRUM, magnitudo(Icones));
        petitio.fons           = &lata;
        petitio.titulus        = chorda_ex_literis("AppIcon", piscina);
        petitio.latera_petita  = ICONES_LATERA_LXIV;
        CREDO_VERUM(icones_reddere(&petitio, &fructus, &status, &sedes,
                                   piscina));
        CREDO_AEQUALIS_I32(fructus.latera_fontis, C);
        pars = (IconesPars*)xar_obtinere(fructus.partes, ZEPHYRUM);
        CREDO_NON_NIHIL(pars);
        decodificata = imago_caricare_ex_memoria(pars->octeti.datum,
                                                 pars->octeti.mensura,
                                                 piscina);
        CREDO_VERUM(decodificata.successus);
        CREDO_AEQUALIS_I32(decodificata.imago.latitudo, LXIV);
        CREDO_AEQUALIS_I32(decodificata.imago.altitudo, LXIV);
        rubra     = ZEPHYRUM;
        caerulea  = ZEPHYRUM;
        numerus = decodificata.imago.latitudo
            * decodificata.imago.altitudo;
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            constans i8* q = decodificata.imago.pixela + (i * IV);

            si (q[ZEPHYRUM] != ZEPHYRUM)
            {
                rubra++;
            }
            si (q[II] == 255)
            {
                caerulea++;
            }
        }
        CREDO_AEQUALIS_I32(rubra, ZEPHYRUM);
        /* gemellus POSITIVUS: imago vacua 'rubrum nullum' daret */
        CREDO_AEQUALIS_I32(caerulea, (i32)(LXIV * LXIV));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    credo_claudere();
    piscina_destruere(piscina);

    redde praeteritus ? ZEPHYRUM : I;
}
