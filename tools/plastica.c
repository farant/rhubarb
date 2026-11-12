/* plastica - instrumentum: res plasticae (descriptiones STML in
 * plastica/res) in paginam aspectuum PNG
 * (project-specs/plastica-spec.md IV.3; plastica-plan.md T3). Logica
 * in lib/plastica.c et lib/plastica_specimen.c; hic argumenta et
 * effusio sola.
 *
 * Usus:
 *
 *   plastica specimen <res.stml> -o <via.png>
 *   plastica texturae -o <via.png>
 *
 * specimen: descriptionem legit, numeros et tabulam partium imprimit,
 * paginam (IV aspectus x IV modi, plastica_specimen.h) scribit.
 * Tabula partium: numerus partis, color eius in ordine 'partes'
 * paginae, signum, titulus; pars invisa notatur.
 *
 * texturae: omnia genera texturarum genitarum (plastica_textura.h)
 * in pagina una: tabulae tres (cruda, exemplaria, Atkinson), in
 * quaque genera IX ordine tabulae generum, quodque tegula bis
 * repetita dextrorsum et deorsum (sutura appareret) et bis aucta.
 *
 * Descriptio recusata: "<via>:<linea>: <causa>" in stderr, nihil
 * scribitur.
 *
 * Exitus: 0 bene; 1 refutatum (causa nominata); 2 usus.
 *
 * Aedificatio: ./tools/plastica_struere.sh -> bin/plastica. */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur */
/* <aedilis obiectum="build/fabrica/provenientia/plastica.c"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "imago_png.h"
#include "statua.h"
#include "plastica.h"
#include "plastica_specimen.h"
#include "plastica_textura.h"
#include "statuae_quadrum.h"
#include "dithering.h"
#include "provenientia.h"
#include <stdio.h>
#include <string.h>

interior s32
usum_imprimere (vacuum)
{
    fprintf(stderr,
        "usus: plastica specimen <res.stml> -o <via.png>\n"
        "      plastica texturae -o <via.png>\n");
    redde II;
}

interior constans character*
signi_titulus (
    i32 signa)
{
    si (signa & STATUA_SIGNUM_GRESSIBILE)
    {
        redde "gressibile";
    }
    si (signa & STATUA_SIGNUM_SOLIDUM)
    {
        redde "solidum";
    }
    redde "-";
}

/* pars cuius omnes tractus alpha infra limen habent non pingitur */
interior b32
pars_invisa (
    constans Statua* s,
                i32  nodus)
{
    constans StatuaRete* rete = &s->retia[s->nodi[nodus].rete];
                    i32  j;

    per (j = ZEPHYRUM; j < rete->partium; j++)
    {
        s32 materia = s->partes[rete->pars_prima + j].materia;

        si (   materia < ZEPHYRUM
            || s->materiae[materia].limen_alpha < 0.0f
            || s->materiae[materia].color[III]
                >= s->materiae[materia].limen_alpha)
        {
            redde FALSUM;
        }
    }
    redde rete->partium > ZEPHYRUM;
}

interior vacuum
numeros_imprimere (
    constans character* via,
       constans Statua* s,
       constans chorda* partes,
               Piscina* piscina)
{
    PlasticaMensurae m = plastica_metiri(s, piscina);
                 i32 n;
                 i32 pars = ZEPHYRUM;

    imprimere("%s: %u partes (%u invisae), %u triangula\n", via,
        (insignatus)m.partium, (insignatus)m.invisae,
        (insignatus)m.triangulorum);
    imprimere("fines: x %.2f .. %.2f, y %.2f .. %.2f,"
        " z %.2f .. %.2f\n",
        (f64)m.minimum.x, (f64)m.maximum.x, (f64)m.minimum.y,
        (f64)m.maximum.y, (f64)m.minimum.z, (f64)m.maximum.z);
    imprimere("vestigium: %u cellae solidae, %u gressibiles\n",
        (insignatus)m.cellae_solidae,
        (insignatus)m.cellae_gressibiles);
    imprimere("partes:\n");
    per (n = ZEPHYRUM; n < s->nodorum; n++)
    {
        si (s->nodi[n].rete < ZEPHYRUM)
        {
            perge;
        }
        imprimere("  %3u  %-8s %-11s %.*s%s\n", (insignatus)pars,
            plastica_specimen_color(pars),
            signi_titulus(s->nodi[n].signa),
            (integer)partes[n].mensura,
            (constans character*)partes[n].datum,
            pars_invisa(s, n) ? "  (invisa)" : "");
        pars++;
    }
}

interior s32
exsequi_specimen (
    constans character* via,
    constans character* exitus,
               Piscina* piscina)
{
    PlasticaFructus f;
         PngFructus png;
             chorda textus;
              Imago imago;

    textus = filum_legere_totum(via, piscina);
    si (textus.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "plastica: plagula '%s' legi non potest\n",
            via);
        redde I;
    }
    f = plastica_legere(textus, piscina);
    si (!f.successus)
    {
        fprintf(stderr, "%s:%u: %.*s\n", via, (insignatus)f.linea,
            (integer)f.error.mensura,
            (constans character*)f.error.datum);
        redde I;
    }
    si (!plastica_specimen_pingere(f.statua, &imago, piscina))
    {
        fprintf(stderr, "plastica: pagina pingi non potuit\n");
        redde I;
    }
    png = imago_png_scribere(&imago, exitus, piscina);
    si (!png.successus)
    {
        fprintf(stderr, "plastica: PNG scribi non potest: %s\n",
            exitus);
        redde I;
    }
    numeros_imprimere(via, f.statua, f.partes, piscina);
    imprimere("%s: %u x %u\n", exitus, (insignatus)imago.latitudo,
        (insignatus)imago.altitudo);
    redde ZEPHYRUM;
}

/* colores exempli cuiusque generis (obscurus, medius, clarus),
 * ordine plastica_textura_genus */
hic_manens constans i8 COLORES_EXEMPLI[PLASTICA_TEXTURA_GENERA][IX] =
{
    { 0x4a, 0x34, 0x22,  0x6b, 0x4a, 0x2b,  0x8a, 0x66, 0x40 },
    { 0x5a, 0x40, 0x28,  0x8c, 0x68, 0x40,  0xa8, 0x84, 0x58 },
    { 0x5e, 0x58, 0x4e,  0x8f, 0x86, 0x76,  0xa8, 0x9c, 0x88 },
    { 0x6e, 0x62, 0x58,  0x9c, 0x4a, 0x38,  0xb8, 0x62, 0x48 },
    { 0x7a, 0x5c, 0x28,  0xb0, 0x8c, 0x44,  0xd0, 0xb0, 0x68 },
    { 0x24, 0x4a, 0x26,  0x3a, 0x6e, 0x30,  0x5c, 0x92, 0x3c },
    { 0x34, 0x4c, 0x28,  0x4e, 0x70, 0x34,  0x78, 0x98, 0x48 },
    { 0x4c, 0x38, 0x26,  0x6e, 0x54, 0x38,  0x94, 0x7c, 0x5c },
    { 0x70, 0x70, 0x70,  0x90, 0x90, 0x90,  0xb0, 0xb0, 0xb0 }
};

#define EXEMPLI_COLUMNAE  III
#define EXEMPLI_AUCTIO    II
#define EXEMPLI_CELLA     \
    (PLASTICA_TEXTURA_LATUS * II * EXEMPLI_AUCTIO)
#define EXEMPLI_TABULA    (EXEMPLI_CELLA * EXEMPLI_COLUMNAE)

interior s32
exsequi_texturas (
    constans character* exitus,
               Piscina* piscina)
{
    DitheringFructus atkinson;
          PngFructus png;
               Imago pagina;
               Imago tegula;
                 b32 activi[XVI];
      memoriae_index pixela = (memoriae_index)EXEMPLI_TABULA
          * EXEMPLI_TABULA;
                  i8* cruda  = (i8*)piscina_allocare(piscina,
                      pixela * IV);
                  i8* larva    = (i8*)piscina_allocare(piscina, pixela);
                  i8* indices  = (i8*)piscina_allocare(piscina, pixela);
                 i32  g;
                 i32  x;
                 i32  y;

    pagina.latitudo  = EXEMPLI_TABULA * III;
    pagina.altitudo  = EXEMPLI_TABULA;
    pagina.pixela    = (i8*)piscina_allocare(piscina, pixela * XII);
    si (!cruda || !larva || !indices || !pagina.pixela)
    {
        fprintf(stderr, "plastica: memoria deficit\n");
        redde I;
    }
    memset(larva, I, pixela);
    memset(indices, ZEPHYRUM, pixela);
    per (g = ZEPHYRUM; g < PLASTICA_TEXTURA_GENERA; g++)
    {
        i32 origo_x = (g % EXEMPLI_COLUMNAE) * EXEMPLI_CELLA;
        i32 origo_y = (g / EXEMPLI_COLUMNAE) * EXEMPLI_CELLA;

        si (!plastica_textura_generare(
            chorda_ex_literis(plastica_textura_genus(g), piscina),
            PLASTICA_TEXTURA_LATUS, PLASTICA_TEXTURA_LATUS, ZEPHYRUM,
            COLORES_EXEMPLI[g], &tegula, piscina))
        {
            fprintf(stderr, "plastica: textura '%s' deficit\n",
                plastica_textura_genus(g));
            redde I;
        }
        per (y = ZEPHYRUM; y < EXEMPLI_CELLA; y++)
        {
            per (x = ZEPHYRUM; x < EXEMPLI_CELLA; x++)
            {
                i32 tx = (x / EXEMPLI_AUCTIO) % PLASTICA_TEXTURA_LATUS;
                i32 ty = (y / EXEMPLI_AUCTIO) % PLASTICA_TEXTURA_LATUS;

                memcpy(cruda + ((memoriae_index)(origo_y + y)
                        * EXEMPLI_TABULA + origo_x + x) * IV,
                    tegula.pixela + ((memoriae_index)ty
                        * PLASTICA_TEXTURA_LATUS + tx) * IV, IV);
            }
        }
    }
    dithering_praeparare_omnes(activi);
    atkinson = dithering_atkinson_colorum(cruda, EXEMPLI_TABULA,
        EXEMPLI_TABULA, activi, piscina);
    si (   !statuae_picturam_dithering(cruda, larva, EXEMPLI_TABULA,
            EXEMPLI_TABULA, activi, indices, piscina)
        || !atkinson.successus || !atkinson.indices)
    {
        fprintf(stderr, "plastica: dithering deficit\n");
        redde I;
    }
    per (y = ZEPHYRUM; y < EXEMPLI_TABULA; y++)
    {
        memoriae_index  fons = (memoriae_index)y * EXEMPLI_TABULA;
                    i8* linea = pagina.pixela
                        + (memoriae_index)y * pagina.latitudo * IV;

        memcpy(linea, cruda + fons * IV,
            (memoriae_index)EXEMPLI_TABULA * IV);
        dithering_indices_ad_rgba(indices + fons,
            linea + (memoriae_index)EXEMPLI_TABULA * IV,
            EXEMPLI_TABULA, I);
        dithering_indices_ad_rgba(atkinson.indices + fons,
            linea + (memoriae_index)EXEMPLI_TABULA * VIII,
            EXEMPLI_TABULA, I);
    }
    png = imago_png_scribere(&pagina, exitus, piscina);
    si (!png.successus)
    {
        fprintf(stderr, "plastica: PNG scribi non potest: %s\n",
            exitus);
        redde I;
    }
    per (g = ZEPHYRUM; g < PLASTICA_TEXTURA_GENERA; g++)
    {
        imprimere("  %u  %s\n", (insignatus)g,
            plastica_textura_genus(g));
    }
    imprimere("%s: %u x %u\n", exitus, (insignatus)pagina.latitudo,
        (insignatus)pagina.altitudo);
    redde ZEPHYRUM;
}

externus constans ProvenientiaRelatio provenientia_plastica;

s32
principale (
      integer   argc,
    character** argv)
{
    constans character* via     = NIHIL;
    constans character* exitus  = NIHIL;
               Piscina* piscina;
               integer  k;
                   b32  texturae;
                   s32  r;

    si (provenientia_respondere((s32)argc, argv,
        &provenientia_plastica))
    {
        redde ZEPHYRUM;
    }
    si (argc < II)
    {
        redde usum_imprimere();
    }
    texturae = strcmp(argv[I], "texturae") == ZEPHYRUM;
    si (!texturae && strcmp(argv[I], "specimen") != ZEPHYRUM)
    {
        redde usum_imprimere();
    }
    per (k = II; k < argc; k++)
    {
        si (strcmp(argv[k], "-o") == ZEPHYRUM)
        {
            si (k + I >= argc || exitus)
            {
                redde usum_imprimere();
            }
            exitus = argv[++k];
            perge;
        }
        si (argv[k][ZEPHYRUM] == '-' || via)
        {
            redde usum_imprimere();
        }
        via = argv[k];
    }
    si (!exitus || (texturae ? via != NIHIL : via == NIHIL))
    {
        redde usum_imprimere();
    }
    piscina  = piscina_generare_dynamicum("plastica", M * M);
    r        = texturae ? exsequi_texturas(exitus, piscina)
        : exsequi_specimen(via, exitus, piscina);
    piscina_destruere(piscina);
    redde r;
}
