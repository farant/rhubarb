/* dithering_ordinatum.c - color proximus et Bayer ordinatum (vide
 * dithering_ordinatum.h) */

#include "dithering_ordinatum.h"
#include "dithering_exemplaria_fontes.h"
#include <string.h>

/* colores activi cum indicibus palettae suis */
nomen structura {
    s32 color[XVI][III];
    s32 index[XVI];
    s32 numerus;
} PaletteActiva;

interior vacuum
_palettam_activam_parare (
     constans b32  colores_activi[XVI],
    PaletteActiva* p)
{
    s32 k;

    p->numerus = ZEPHYRUM;
    per (k = ZEPHYRUM; k < XVI; k++)
    {
        si (!colores_activi[k])
        {
            perge;
        }
        p->color[p->numerus][ZEPHYRUM] =
            (s32)AQUINAS_PALETTE[k][ZEPHYRUM];
        p->color[p->numerus][I]   = (s32)AQUINAS_PALETTE[k][I];
        p->color[p->numerus][II]  = (s32)AQUINAS_PALETTE[k][II];
        p->index[p->numerus]      = k;
        p->numerus++;
    }
}

/* locus (in activis) coloris proximi in spatio RGB; paria: prior
 * (index palettae minor) vincit */
interior s32
_activus_proximus (
    constans PaletteActiva* p,
                       s32  r,
                       s32  g,
                       s32  b)
{
    s32 optimus    = ZEPHYRUM;
    s32 distantia  = 0x7FFFFFFF;
    s32 k;
    s32 dr;
    s32 dg;
    s32 db;
    s32 d;

    per (k = ZEPHYRUM; k < p->numerus; k++)
    {
        dr  = r - p->color[k][ZEPHYRUM];
        dg  = g - p->color[k][I];
        db  = b - p->color[k][II];
        d   = dr * dr + dg * dg + db * db;
        si (d < distantia)
        {
            distantia  = d;
            optimus    = k;
        }
    }
    redde optimus;
}

/* fructus cum indicibus allocatis; successus FALSUM si argumenta
 * mala aut nullus color activus */
interior DitheringFructus
_fructum_parare (
      constans i8* rgba,
              i32  latitudo,
              i32  altitudo,
     constans b32  colores_activi[XVI],
          Piscina* piscina,
    PaletteActiva* p)
{
    DitheringFructus f;

    f.successus  = FALSUM;
    f.indices    = NIHIL;
    f.latitudo   = latitudo;
    f.altitudo   = altitudo;
    si (   !rgba || !piscina || !colores_activi || latitudo == ZEPHYRUM
        || altitudo == ZEPHYRUM)
    {
        redde f;
    }
    _palettam_activam_parare(colores_activi, p);
    si (p->numerus == ZEPHYRUM)
    {
        redde f;
    }
    f.indices = (i8*)piscina_allocare(piscina,
        (memoriae_index)latitudo * altitudo);
    f.successus = f.indices != NIHIL;
    redde f;
}

DitheringFructus
dithering_proximus_colorum (
     constans i8* rgba,
             i32  latitudo,
             i32  altitudo,
    constans b32  colores_activi[XVI],
         Piscina* piscina)
{
       PaletteActiva p;
    DitheringFructus f = _fructum_parare(rgba, latitudo, altitudo,
        colores_activi, piscina, &p);
      memoriae_index k;
      memoriae_index pixela = (memoriae_index)latitudo * altitudo;

    si (!f.successus)
    {
        redde f;
    }
    per (k = ZEPHYRUM; k < pixela; k++)
    {
        f.indices[k] = (i8)p.index[_activus_proximus(&p,
            (s32)rgba[k * IV], (s32)rgba[k * IV + I],
            (s32)rgba[k * IV + II])];
    }
    redde f;
}

/* ------------------------------------------------------------
 * Bayer ordinatum cum candidatis (ordinatum N colorum)
 *
 * Duo colores proximi palettae XVI colorum non sufficiunt: colorem
 * medium saepe nec proximus nec mixtio duorum bene reddit (pileus
 * ruber fit roseus maculatus). Ergo pro quoque colore pixeli CONSILIUM
 * fit: XVI candidati, quorum MEDIUM colorem pixeli approximat -
 * candidatus quisque color proximus est colori pixeli plus errore
 * candidatorum priorum cumulato (diffusio erroris, sed intra pixelum
 * unum, non ad vicinos). Candidati per claritatem ordinantur et
 * craticula Bayer unum eligit: index = craticula / IV.
 *
 * Consilium a colore solo pendet, non a loco: imago pauca colorum
 * distinctorum (statua picta) idem consilium saepe repetit, ergo
 * tabula dispersa INTRA VOCATIONEM consilia tenet (nihil inter
 * vocationes servatur). Tabula dimidio plena: consilia nova
 * computantur nec conduntur.
 * ------------------------------------------------------------ */

#define BAYER_CANDIDATI     XVI
#define BAYER_TABULAE_BITA  XII
#define BAYER_TABULAE_LOCI  (I << BAYER_TABULAE_BITA)
#define BAYER_CLAVIS_VACUA  0xFFFFFFFFu

nomen structura {
    i32 clavis;                       /* r | g << 8 | b << 16 */
     i8 candidati[BAYER_CANDIDATI];   /* indices palettae, ordinati */
} BayerConsilium;

/* claritas coloris activi (pondera integra CCXCIX, DLXXXVII, CXIV) */
interior s32
_claritas (
    constans PaletteActiva* p,
                       s32  k)
{
    redde 299 * p->color[k][ZEPHYRUM] + 587 * p->color[k][I]
        + 114 * p->color[k][II];
}

interior s32
_cogere (
    s32 v)
{
    si (v < ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde v > 255 ? 255 : v;
}

/* consilium coloris: candidati (loci in activis) per claritatem
 * crescentem, paria per locum minorem; deinde in indices palettae */
interior vacuum
_consilium_computare (
    constans PaletteActiva* p,
                       s32  r,
                       s32  g,
                       s32  b,
                        i8* candidati)
{
    s32 loci[BAYER_CANDIDATI];
    s32 error_r = ZEPHYRUM;
    s32 error_g = ZEPHYRUM;
    s32 error_b = ZEPHYRUM;
    s32 k;
    s32 j;
    s32 locus;

    per (k = ZEPHYRUM; k < BAYER_CANDIDATI; k++)
    {
        locus = _activus_proximus(p, _cogere(r + error_r),
            _cogere(g + error_g), _cogere(b + error_b));
        error_r += r - p->color[locus][ZEPHYRUM];
        error_g += g - p->color[locus][I];
        error_b += b - p->color[locus][II];
        /* insertio ordinata */
        j = k;
        dum (   j > ZEPHYRUM
             && (   _claritas(p, loci[j - I]) > _claritas(p, locus)
                 || (   _claritas(p, loci[j - I]) == _claritas(p, locus)
                     && loci[j - I] > locus)))
        {
            loci[j] = loci[j - I];
            j--;
        }
        loci[j] = locus;
    }
    per (k = ZEPHYRUM; k < BAYER_CANDIDATI; k++)
    {
        candidati[k] = (i8)p->index[loci[k]];
    }
}

DitheringFructus
dithering_bayer_colorum (
     constans i8* rgba,
             i32  latitudo,
             i32  altitudo,
    constans b32  colores_activi[XVI],
         Piscina* piscina)
{
       PaletteActiva p;
    DitheringFructus f = _fructum_parare(rgba, latitudo, altitudo,
        colores_activi, piscina, &p);
      BayerConsilium* tabula;
      BayerConsilium* c;
      memoriae_index  locus;
                 i32  x;
                 i32  y;
                 i32  clavis;
                 i32  sedes;
                 i32  condita = ZEPHYRUM;
                  i8  nova[BAYER_CANDIDATI];
         constans i8* candidati;

    si (!f.successus)
    {
        redde f;
    }
    tabula = (BayerConsilium*)piscina_allocare(piscina,
        BAYER_TABULAE_LOCI * magnitudo(BayerConsilium));
    per (sedes = ZEPHYRUM; sedes < BAYER_TABULAE_LOCI; sedes++)
    {
        tabula[sedes].clavis = BAYER_CLAVIS_VACUA;
    }
    per (y = ZEPHYRUM; y < altitudo; y++)
    {
        per (x = ZEPHYRUM; x < latitudo; x++)
        {
            locus   = (memoriae_index)y * latitudo + x;
            clavis  = (i32)rgba[locus * IV]
                | ((i32)rgba[locus * IV + I] << VIII)
                | ((i32)rgba[locus * IV + II] << XVI);
            sedes   = (clavis * 2654435761u) >> (XXXII
                - BAYER_TABULAE_BITA);
            dum (   tabula[sedes].clavis != clavis
                 && tabula[sedes].clavis != BAYER_CLAVIS_VACUA)
            {
                sedes = (sedes + I) & (BAYER_TABULAE_LOCI - I);
            }
            c = &tabula[sedes];
            si (c->clavis == clavis)
            {
                candidati = c->candidati;
            }
            alioquin si (condita < BAYER_TABULAE_LOCI / II)
            {
                _consilium_computare(&p, (s32)rgba[locus * IV],
                    (s32)rgba[locus * IV + I],
                    (s32)rgba[locus * IV + II], c->candidati);
                c->clavis = clavis;
                candidati = c->candidati;
                condita++;
            }
            alioquin
            {
                _consilium_computare(&p, (s32)rgba[locus * IV],
                    (s32)rgba[locus * IV + I],
                    (s32)rgba[locus * IV + II], nova);
                candidati = nova;
            }
            f.indices[locus] = candidati[DITHERING_BAYER[y & VII]
                [x & VII] * BAYER_CANDIDATI / LXIV];
        }
    }
    redde f;
}
