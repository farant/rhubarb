/* icones.c - icones .app (contractus in capite).
 *
 * SEPTEM redditiones, DECEM plagulae: tabula ORDINES ter idem latus
 * sub nominibus logicis diversis nominat, ergo quodque latus SEMEL
 * scalatur et SEMEL codificatur, et partes geminae eundem indicem
 * octetorum ferunt. Ita .iconset et .icns constructione consentiunt,
 * non duabus semitis quae consentire debent.
 *
 * EXCEPTIO MENSURATA: ic04 et ic05 in continente ARGB ferunt, non PNG -
 * Finder et iconutil PNG ibi ut strepitum legunt (2026-09-12). Ibi
 * .icns et .iconset PIXELIS consentiunt (eadem imago scalata), non
 * octetis.
 *
 * Nihil decodificat: Imago a vocante venit (par. III, D7).
 */

#include "icones.h"
#include "imago_opus.h"
#include "imago_png.h"
#include "filum.h"
#include "via.h"
#include "chorda_aedificator.h"

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

/* Nomina camporum ut sedes recusationum, in memoria STATICA: sedes
 * etiam ubi piscina ipsa deest nominari debet. Tabulae MUTABILES, non
 * literae: literae sub vexillis domus constantes sunt, et const in i8*
 * abiicere -Wcast-qual vetat. */
hic_manens character SEDES_PETITIO[]      = "petitio";
hic_manens character SEDES_FRUCTUS[]      = "fructus";
hic_manens character SEDES_PISCINA[]      = "piscina";
hic_manens character SEDES_TITULUS[]      = "titulus";
hic_manens character SEDES_FONS[]         = "fons";
hic_manens character SEDES_PIXELA[]       = "pixela";
hic_manens character SEDES_LATITUDO[]     = "latitudo";
hic_manens character SEDES_ALTITUDO[]     = "altitudo";
hic_manens character SEDES_PARTES[]       = "partes";
hic_manens character SEDES_VIA[]          = "via";
hic_manens character SEDES_VIA_RADICIS[]  = "via_radicis";

#define SEDES_FIXA(tabula) \
    chorda_ex_buffer((i8*)(tabula), (i32)(magnitudo(tabula) - I))

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

/* Quadratum ad CENTRUM recisum, SEMEL pro omnibus lateribus.
 *
 * Olim regio et scala uno vocamine imago_extrahere_et_scalare
 * petebantur, et commentarium hic dicebat nos dimensiones IPSOS
 * computare. FALSUM erat: illa functio latera ut LIMITES accipit et ea
 * truncat (fons M px ad XVI -> XV), et modum AREA tacite bilinearem
 * faciebat (2026-09-12, oraculum sips congelatum invenit). Nunc regio
 * hic copiatur et imago_scalare - dimensiones EXACTAE - quodque latus
 * facit. Fons iam quadratus ipse redditur: nihil copiatur. */
interior Imago
_quadratum_recidere (
     constans Imago* fons,
                s32  latus,
            Piscina* piscina)
{
    Imago quadratum;
      s32 x = ((s32)fons->latitudo - latus) / II;
      s32 y = ((s32)fons->altitudo - latus) / II;
      s32 versus;

    si ((s32)fons->latitudo == latus && (s32)fons->altitudo == latus)
    {
        redde *fons;
    }
    quadratum.latitudo = (i32)latus;
    quadratum.altitudo = (i32)latus;
    quadratum.pixela    = (i8*)piscina_allocare(piscina,
        (memoriae_index)((s64)latus * latus * IV));
    si (quadratum.pixela == NIHIL)
    {
        redde quadratum;
    }
    per (versus = ZEPHYRUM; versus < latus; versus++)
    {
        memcpy(quadratum.pixela + ((s64)versus * latus * IV),
               fons->pixela
                   + ((((s64)y + versus) * (s64)fons->latitudo
                   + x) * IV),
               (size_t)((s64)latus * IV));
    }
    redde quadratum;
}

/* Longitudo magni-endiana octetum post octetum, significantissimum
 * PRIMUM - numquam memcpy i32 hospitis, qui in machina parvi-endiana
 * inversus scriberetur. Forma ipsa imago_png.c (ibi interior, ergo
 * hic iterata). */
interior vacuum
_be32_scribere (
     i8* destinatio,
    i32  valor)
{
    destinatio[ZEPHYRUM]  = (i8)((valor >> XXIV) & CCLV);
    destinatio[I]         = (i8)((valor >> XVI) & CCLV);
    destinatio[II]        = (i8)((valor >> VIII) & CCLV);
    destinatio[III]       = (i8)(valor & CCLV);
}

/* Codex .icns per SEMITAM partis. Partes codicem non ferunt (spec
 * par. III) et ordinem tabulae CUM LACUNIS sequuntur (latera omissa
 * aut non petita), ergo index directus in ORDINES falleret. NIHIL si
 * semita tabulae ignota est. */
interior constans character*
_codicem_invenire (
    chorda semita)
{
    i32 i;

    per (i = ZEPHYRUM; i < X; i++)
    {
        si (chorda_aequalis_literis(semita, ORDINES[i].semita))
        {
            redde ORDINES[i].codex;
        }
    }
    redde NIHIL;
}

/* Chunkus unus .icns: codex IV litterarum, longitudo magni-endiana
 * INCLUSO capite VIII octetorum, onus (PNG, aut ARGB pro ic04/ic05).
 * Longitudo SCRIPTA et sedes REDDITA separatim computantur: culpa in
 * longitudine tabulam in memoria non rumpit, sed ambulationem legentis
 * (I3) fallit.
 *
 * Redde: sedes proxima post chunkum. */
interior i32
_ordinem_scribere (
                 i8* tela,
                i32  sedes,
 constans character* codex,
             chorda  octeti)
{
    tela[sedes]        = (i8)codex[ZEPHYRUM];
    tela[sedes + I]    = (i8)codex[I];
    tela[sedes + II]   = (i8)codex[II];
    tela[sedes + III]  = (i8)codex[III];

    _be32_scribere(tela + sedes + IV, (i32)VIII + octeti.mensura);
    memcpy(tela + sedes + VIII, octeti.datum, (size_t)octeti.mensura);

    redde sedes + (i32)VIII + octeti.mensura;
}

/* Latera petita ut textus ("128 1024"): sedes recusationis quae QUID
 * petitum sit dicit, non vexilla nuda. Ordines tabulae ascendunt et
 * gemini adiacent, ergo latus ultimum scriptum duplicata tollit.
 * Magnitudo IGNOTA et TEXTUS: munus ipsum chorda_aedificator (cf.
 * continens .icns, ubi magnitudo nota est et octeti crudi). */
/* Codices quos Apple ARGB implet, non PNG. Finder et iconutil PNG in
 * his locis ut STREPITUM legunt (fasciculus SolumXVI, 2026-09-12),
 * sips/ImageIO recte - sips hic oraculum LENE nobis favens erat.
 * icp4/icp5 (remedium in spec par. IV nominatum) aeque male leguntur.
 * Tabula, non condicio in codice: si Apple mutat, linea mutatur. Non
 * columna ORDINUM, quia columna quinta omnes lineas tabulae ultra
 * LXXII columnas truderet. */
hic_manens constans character* CODICES_ARGB[II] = { "ic04", "ic05" };

interior b32
_onus_argb_poscitur (
    constans character* codex)
{
    i32 i;

    per (i = ZEPHYRUM; i < II; i++)
    {
        si (memcmp(codex, CODICES_ARGB[i], (size_t)IV) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* RLE plani unius (A, R, G aut B) in dest. Imperium < 0x80: (imperium
 * + I) octeti crudi sequuntur; >= 0x80: octetus sequens (imperium -
 * 125) vicibus. Regula Pythone super onera ipsa Apple VERIFICATA (ic04
 * 316, ic05 990 octeti: plana exacte, octeti reliqui nulli). CUPIDE:
 * cursus III..CXXX eiusdem valoris, aliter crudi usque ad CXXVIII aut
 * donec cursus III incipiat.
 *
 * Redde: octeti scripti. */
interior i32
_rle_planum (
    constans i8* pixela,
            i32  numerus,
            i32  canalis,
             i8* dest)
{
    i32 i        = ZEPHYRUM;
    i32 scripta  = ZEPHYRUM;
    i32 cursus;
    i32 initium;
    i32 n;
    i32 k;

    dum (i < numerus)
    {
        cursus = I;
        dum (   i + cursus < numerus
             && cursus < CXXVIII + II
             && pixela[((i + cursus) * IV) + canalis]
                == pixela[(i * IV) + canalis])
        {
            cursus++;
        }
        si (cursus >= III)
        {
            dest[scripta++]  = (i8)(CXXVIII + (cursus - III));
            dest[scripta++]  = pixela[(i * IV) + canalis];
            i                += cursus;
            perge;
        }

        initium  = i;
        n        = ZEPHYRUM;
        dum (i < numerus && n < CXXVIII)
        {
            si (   i + II < numerus
                && pixela[(i * IV) + canalis]
                   == pixela[((i + I) * IV) + canalis]
                && pixela[(i * IV) + canalis]
                   == pixela[((i + II) * IV) + canalis])
            {
                frange;
            }
            i++;
            n++;
        }
        dest[scripta++] = (i8)(n - I);
        per (k = ZEPHYRUM; k < n; k++)
        {
            dest[scripta++] = pixela[((initium + k) * IV) + canalis];
        }
    }
    redde scripta;
}

/* Onus ARGB (ic04/ic05): 'ARGB' + plana A, R, G, B, colore
 * PRAEMULTIPLICATO: R' = round(R x A / CCLV). iconutil plana RGB ut
 * praemultiplicata legit et per alpha dividit - MENSURATUM 2026-09-12:
 * nostrum rectum 82 ad alpha 101 -> Apple 207 = 82 x 255 / 101, in
 * omnibus pixelis partialibus (LII de LII ic04, CVIII de CVIII ic05).
 * Recta scripta marginem ALBESCENTEM darent, halonem quem D8 vetat.
 *
 * Assertio prior 'valores recti' FALSA erat: onera Apple quae
 * decodificavi ex redditione bilineari facta erant, ubi alpha solum 0
 * aut 255 est - ibi rectum et praemultiplicatum IDEM octeti sunt, ergo
 * probatio discernere non poterat.
 *
 * Tela ad pessimum casum: copia praemultiplicata n x IV, et planum n
 * octetorum crudorum n + n / CXXVIII + I poscit. */
interior chorda
_argb_codificare (
    constans Imago* imago,
           Piscina* piscina)
{
    chorda  onus;
        i8* tela;
        i8* praemultiplicata;
       i32  n;
       i32  i;
       i32  canalis;
       i32  alpha;
       i32  sedes;

    onus.datum    = NIHIL;
    onus.mensura  = ZEPHYRUM;
    n             = imago->latitudo * imago->altitudo;
    praemultiplicata = (i8*)piscina_allocare(piscina,
        (memoriae_index)(n * IV));
    tela          = (i8*)piscina_allocare(piscina,
        (memoriae_index)(IV + (IV * (n + (n / CXXVIII) + I))));
    si (!tela || !praemultiplicata)
    {
        redde onus;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        alpha = (i32)imago->pixela[(i * IV) + III];
        per (canalis = ZEPHYRUM; canalis < III; canalis++)
        {
            praemultiplicata[(i * IV) + canalis] = (i8)(
                (((i32)imago->pixela[(i * IV) + canalis] * alpha)
                 + (CXXVIII - I)) / CCLV);
        }
        praemultiplicata[(i * IV) + III] = (i8)alpha;
    }
    memcpy(tela, "ARGB", (size_t)IV);
    sedes = (i32)IV;
    sedes += _rle_planum(praemultiplicata, n, III, tela + sedes);
    sedes += _rle_planum(praemultiplicata, n, ZEPHYRUM, tela + sedes);
    sedes += _rle_planum(praemultiplicata, n, I, tela + sedes);
    sedes += _rle_planum(praemultiplicata, n, II, tela + sedes);
    onus.datum = tela;
    onus.mensura = sedes;
    redde onus;
}

interior chorda
_latera_nominare (
         i32  petita,
     Piscina* piscina)
{
    ChordaAedificator* aedificator;
               chorda  vacua;
                  i32  ultimum;
                  i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    aedificator = chorda_aedificator_creare(piscina, XXXII);
    si (!aedificator)
    {
        redde vacua;
    }
    ultimum = ZEPHYRUM;
    per (i = ZEPHYRUM; i < X; i++)
    {
        si (   (petita & ORDINES[i].vexillum) == ZEPHYRUM
            || ORDINES[i].latera              == ultimum)
        {
            perge;
        }
        si (ultimum != ZEPHYRUM)
        {
            chorda_aedificator_appendere_character(aedificator, ' ');
        }
        chorda_aedificator_appendere_i32(aedificator,
            ORDINES[i].latera);
        ultimum = ORDINES[i].latera;
    }
    redde chorda_aedificator_finire(aedificator);
}

/* Fructus IUDICATUR antequam scriptor quicquam tangat. Fructus manu
 * factus (non per icones_reddere) partes nullas, semitam tabulae
 * ignotam aut octetos vacuos ferre potest, et quisque .icns aut
 * .iconset VALIDUM sed inanem daret: icon vacuus sine errore, forma
 * ipsa quam haec bibliotheca claudere exsistit. Semita VACUA hic
 * LATERA est, non SCRIPTIO: via_iungere partes vacuas praeterit, ergo
 * sine iudice in directorium ipsum scriberetur.
 *
 * Sedes: nomen campi, aut SEMITA partis vitiosae. */
interior b32
_fructum_iudicare (
    constans IconesFructus* fructus,
              IconesStatus* status,
                    chorda* sedes_vitii)
{
    i32 numerus;
    i32 i;

    si (!fructus)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_FRUCTUS));
    }
    si (!fructus->partes || xar_numerus(fructus->partes) == ZEPHYRUM)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_PARTES));
    }
    numerus = xar_numerus(fructus->partes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        IconesPars* pars = (IconesPars*)xar_obtinere(fructus->partes,
            i);

        /* intra limites NUMQUAM NIHIL: ramus defensivus quem nulla
         * probatio attingit, ergo nulla planta */
        si (!pars)
        {
            redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                            SEDES_FIXA(SEDES_PARTES));
        }
        si (!_codicem_invenire(pars->semita))
        {
            redde _recusare(status, sedes_vitii, ICONES_ERROR_LATERA,
                            pars->semita);
        }
        si (chorda_vacua(pars->octeti) || chorda_vacua(pars->onus_icns))
        {
            redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                            pars->semita);
        }
    }
    redde VERUM;
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
     Imago imagines[X];   /* imago per ordinem, communis geminis */
       i32 petita;
       s32 latus;
       i32 i;
       i32 j;
       b32 tegit;
     Imago quadratum;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status || !sedes_vitii)
    {
        redde FALSUM;
    }
    *status       = ICONES_SUCCESSUS;
    *sedes_vitii  = vacua;

    si (!petitio)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_PETITIO));
    }
    si (!fructus)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_FRUCTUS));
    }
    si (!piscina)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_PISCINA));
    }
    si (chorda_vacua(petitio->titulus))
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_TITULUS));
    }

    /* Fons ABSENS = DESUNT, ut commentarium enumerationis dicit ('fons
     * aut titulus'); fons PRAESENS sed vitiosus = FONS, campo nominato.
     * Olim ambo FONS erant, contra caput ipsum. */
    si (!petitio->fons)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_FONS));
    }
    si (!petitio->fons->pixela)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_FONS,
                        SEDES_FIXA(SEDES_PIXELA));
    }
    si (petitio->fons->latitudo == (i32)ZEPHYRUM)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_FONS,
                        SEDES_FIXA(SEDES_LATITUDO));
    }
    si (petitio->fons->altitudo == (i32)ZEPHYRUM)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_FONS,
                        SEDES_FIXA(SEDES_ALTITUDO));
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
    /* bita IGNOTA SOLA nominantur, non petitio tota */
    si ((petita & ~((i32)ICONES_SETUM_APPLE)) != ZEPHYRUM)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_LATERA,
            chorda_ex_s64((s64)(petita & ~((i32)ICONES_SETUM_APPLE)),
                          piscina));
    }

    /* D10 LATIUS: fons qui latus PETITUM nullum tegit nihil reddit, et
     * successus sine parte icon nullus esset - recusatio, non fructus
     * vacuus cum omissis plenis. Sedes latera petita nominat. */
    tegit = FALSUM;
    per (i = ZEPHYRUM; i < X; i++)
    {
        si (   (petita & ORDINES[i].vexillum) != ZEPHYRUM
            && ORDINES[i].latera              <= (i32)latus)
        {
            tegit = VERUM;
            frange;
        }
    }
    si (!tegit)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_MINIMUS,
                        _latera_nominare(petita, piscina));
    }

    fructus->partes         = xar_creare(piscina,
        (i32)magnitudo(IconesPars));
    fructus->omissa         = ZEPHYRUM;
    fructus->latera_fontis  = (i32)latus;
    fructus->titulus        = petitio->titulus;
    si (!fructus->partes)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                        vacua);
    }

    /* recisio SEMEL, ante ansam laterum */
    quadratum = _quadratum_recidere(petitio->fons, latus, piscina);
    si (quadratum.pixela == NIHIL)
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
                cache[i]     = cache[j];
                imagines[i]  = imagines[j];
                habet[i]     = VERUM;
                iam          = VERUM;
                frange;
            }
        }
        si (!iam)
        {
                  Imago parva;
             PngFructus png;

            /* 1:1 nihil scalatur; aliter dimensiones EXACTAE */
            parva = (ORDINES[i].latera == (i32)latus)
                ? quadratum
                : imago_scalare(&quadratum, ORDINES[i].latera,
                                ORDINES[i].latera, IMAGO_SCALA_AREA,
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
            imagines[i]       = parva;
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
        /* continens: PNG communis, aut ARGB earundem pixelorum ubi
         * Apple ARGB poscit (CODICES_ARGB) */
        si (_onus_argb_poscitur(ORDINES[i].codex))
        {
            pars->onus_icns = _argb_codificare(&imagines[i], piscina);
            si (!pars->onus_icns.datum)
            {
                redde _recusare(status, sedes_vitii,
                                ICONES_ERROR_MEMORIA, vacua);
            }
        }
        alioquin
        {
            pars->onus_icns = cache[i];
        }
    }
    redde VERUM;
}

b32
icones_iconset_scribere (
    constans IconesFructus* fructus,
        constans character* via_radicis,
              IconesStatus* status,
                    chorda* sedes_vitii,
                   Piscina* piscina)
{
        chorda  vacua;
        chorda  relativa;     /* "AppIcon.iconset" - in sedibus */
        chorda  directorium;
        chorda  iungenda[II];
     character* via_directorii;
           i32  numerus;
           i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status || !sedes_vitii)
    {
        redde FALSUM;
    }
    *status       = ICONES_SUCCESSUS;
    *sedes_vitii  = vacua;

    si (!via_radicis)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_VIA_RADICIS));
    }
    si (!piscina)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_PISCINA));
    }
    si (!_fructum_iudicare(fructus, status, sedes_vitii))
    {
        redde FALSUM;
    }
    /* titulus vacuus directorium '.iconset' daret - OCCULTUM et sine
     * nomine */
    si (chorda_vacua(fructus->titulus))
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_TITULUS));
    }

    /* Suffixum .iconset FORMAE est, non electionis: iconutil
     * directorium sine eo recusat. Ergo bibliotheca id possidet. */
    relativa = chorda_concatenare(fructus->titulus,
        chorda_ex_literis(".iconset", piscina), piscina);
    iungenda[ZEPHYRUM]  = chorda_ex_literis(via_radicis, piscina);
    iungenda[I]         = relativa;
    directorium         = via_iungere(iungenda, II, piscina);
    via_directorii      = chorda_ut_cstr(directorium, piscina);
    /* allocatio sola hic deficere potest: nulla probatio id attingit */
    si (!relativa.datum || !via_directorii)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                        vacua);
    }

    /* Consumptor ALTER mkdir -p (primus fasciculum): radix nidificata
     * cuius parentes absunt aliter ENOENT caderet */
    si (!filum_directorium_creare_cum_parentibus(via_directorii))
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DIRECTORIUM,
                        relativa);
    }

    numerus = xar_numerus(fructus->partes);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        IconesPars* pars;
            chorda  via_plagulae;
         character* via_c;

        /* partes iam IUDICATAE: intra limites, semita nota */
        pars                = (IconesPars*)xar_obtinere(fructus->partes,
            i);
        iungenda[ZEPHYRUM]  = directorium;
        iungenda[I]         = pars->semita;
        via_plagulae        = via_iungere(iungenda, II, piscina);
        via_c               = chorda_ut_cstr(via_plagulae, piscina);
        si (!via_c)
        {
            redde _recusare(status, sedes_vitii, ICONES_ERROR_MEMORIA,
                            vacua);
        }

        /* partes geminae EOSDEM octetos scribunt: nihil hic
         * duplicatur nisi scriptio ipsa */
        si (!filum_scribere(via_c, pars->octeti))
        {
            iungenda[ZEPHYRUM] = relativa;
            redde _recusare(status, sedes_vitii, ICONES_ERROR_SCRIPTIO,
                            via_iungere(iungenda, II, piscina));
        }
    }
    redde VERUM;
}

chorda
icones_icns_codificare (
    constans IconesFructus* fructus,
              IconesStatus* status,
                   Piscina* piscina)
{
    chorda vacua;
    chorda continens;
    chorda sedes_neglecta;   /* iudex sedem poscit; codificatio sedem
                               * non refert */
        i8* tela;
       i32  mensura;
       i32  sedes;
       i32  numerus;
       i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status)
    {
        redde vacua;
    }
    *status = ICONES_SUCCESSUS;

    si (!piscina)
    {
        *status = ICONES_ERROR_DESUNT;
        redde vacua;
    }
    si (!_fructum_iudicare(fructus, status, &sedes_neglecta))
    {
        redde vacua;
    }

    /* Magnitudo NOTA ante allocationem, ergo tela semel allocatur - non
     * chorda_aedificator, qui magnitudini ignotae et TEXTO destinatur
     * (caput eius ipsum id dicit). Partes iam IUDICATAE: codex cuiusque
     * notus, octeti non vacui. */
    numerus = xar_numerus(fructus->partes);
    mensura = (i32)VIII;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        IconesPars* pars = (IconesPars*)xar_obtinere(fructus->partes,
            i);

        mensura += (i32)VIII + pars->onus_icns.mensura;
    }

    tela = (i8*)piscina_allocare(piscina, (memoriae_index)mensura);
    si (!tela)
    {
        *status = ICONES_ERROR_MEMORIA;
        redde vacua;
    }

    memcpy(tela, "icns", (size_t)IV);
    _be32_scribere(tela + IV, mensura);

    sedes = (i32)VIII;
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        IconesPars* pars = (IconesPars*)xar_obtinere(fructus->partes,
            i);

        sedes = _ordinem_scribere(tela, sedes,
                                  _codicem_invenire(pars->semita),
                                  pars->onus_icns);
    }

    continens.datum    = tela;
    continens.mensura  = mensura;
    redde continens;
}

b32
icones_icns_scribere (
    constans IconesFructus* fructus,
        constans character* via,
              IconesStatus* status,
                    chorda* sedes_vitii,
                   Piscina* piscina)
{
    chorda vacua;
    chorda continens;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;

    si (!status || !sedes_vitii)
    {
        redde FALSUM;
    }
    *status       = ICONES_SUCCESSUS;
    *sedes_vitii  = vacua;

    si (!via)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_VIA));
    }
    si (!piscina)
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_DESUNT,
                        SEDES_FIXA(SEDES_PISCINA));
    }
    /* iudicium HIC quoque, non solum in codificatione: sedem fert */
    si (!_fructum_iudicare(fructus, status, sedes_vitii))
    {
        redde FALSUM;
    }

    /* Gemellus TENUIS: octetos codificationis purae scribit et nihil
     * aliud computat, ergo plagula et chorda pura constructione
     * congruunt (I6 id octetim probat). Post iudicium sola MEMORIA hic
     * cadere potest. */
    continens = icones_icns_codificare(fructus, status, piscina);
    si (*status != ICONES_SUCCESSUS)
    {
        redde FALSUM;
    }
    si (!filum_scribere(via, continens))
    {
        redde _recusare(status, sedes_vitii, ICONES_ERROR_SCRIPTIO,
                        chorda_ex_literis(via, piscina));
    }
    redde VERUM;
}
