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
 *   plastica condere -volumen V [-fontes DIR]
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
 * condere: omnem plagulam *.stml in DIR (ordinarium plastica/res),
 * ordine nominum, componit, in GLB scribit (statua_glb_scribere, cum
 * nominibus partium) et ut REM in volumen addit sub titulo radicis
 * suae (titulus elementi res). Plagula GLB per lectorem relegitur
 * ANTE additionem. Linea una per plagulam:
 *
 *   addita    TITULUS   res nova in volumine
 *   mutata    TITULUS   octeti substituti, locus in ordine manet
 *   eadem     TITULUS   octeti iidem: nihil scriptum
 *   VIA:LINEA: CAUSA            descriptio recusata (stderr)
 *   VIA: recusata: CAUSA        titulus iam persona aut locus est,
 *                               aut bis in hoc cursu (stderr)
 *
 * Deinde res voluminis quae fontem in DIR non habent ("sine fonte:
 * TITULUS") et summa. NIHIL UMQUAM REMOVET: res sine fonte manu
 * tollitur (statuae removere). Plagula recusata ceteras non impedit;
 * res cuius plagula recusata est ut "sine fonte" apparet (titulus
 * eius legi non potuit).
 *
 * -volumen SEMPER nominandum (nulla via ordinaria: volumen vivum
 * solum nominatum scribitur); volumen absens creatur (ut statuae
 * addere).
 *
 * Descriptio recusata: "<via>:<linea>: <causa>" in stderr, nihil
 * scribitur.
 *
 * Exitus: 0 bene; 1 refutatum (causa nominata; condere: si qua
 * plagula recusata est aut DIR plagulas non habet); 2 usus.
 *
 * Aedificatio: ./tools/plastica_struere.sh -> bin/plastica. */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur */
/* <aedilis obiectum="build/fabrica/provenientia/plastica.c"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "xar.h"
#include "iter_directoria.h"
#include "volumen.h"
#include "statua_glb.h"
#include "statua_glb_scriptor.h"
#include "statuae_repositorium.h"
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
        "      plastica texturae -o <via.png>\n"
        "      plastica condere -volumen V [-fontes DIR]\n");
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

#define FONTES_ORDINARII  "plastica/res"
#define PLAGULAE_MAXIMAE  DXII
#define VIA_MAXIMA        MXXIV

nomen structura {
    i32 additae;
    i32 mutatae;
    i32 eaedem;
    i32 recusatae;
} Summa;

interior b32
est_plagula_stml (
    chorda titulus)
{
    redde titulus.mensura > V
        && memcmp(titulus.datum + titulus.mensura - V, ".stml", V)
            == ZEPHYRUM;
}

/* nomina plagularum *.stml directorii, ordine octetorum; -1 si
 * directorium aperiri non potest aut plagulae nimis multae */
interior s32
plagulas_colligere (
    constans character* fontes,
                chorda* nomina,
               Piscina* piscina)
{
     DirectoriumIterator* iter =
         directorium_iterator_aperire(fontes, piscina);
    DirectoriumIntroitus* introitus;
                     s32  numerus = ZEPHYRUM;
                     s32  k;
                     s32  j;

    si (!iter)
    {
        redde -I;
    }
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        si (   introitus->genus != INTROITUS_FILUM
            || !est_plagula_stml(introitus->titulus))
        {
            perge;
        }
        si (numerus >= PLAGULAE_MAXIMAE)
        {
            directorium_iterator_claudere(iter);
            redde -I;
        }
        nomina[numerus++] =
            chorda_transcribere(introitus->titulus, piscina);
    }
    directorium_iterator_claudere(iter);
    /* ordo nominum: insertio (plagulae paucae) */
    per (k = I; k < numerus; k++)
    {
        chorda hoc = nomina[k];

        per (j = k; j > ZEPHYRUM
            && chorda_comparare(nomina[j - I], hoc) > ZEPHYRUM; j--)
        {
            nomina[j] = nomina[j - I];
        }
        nomina[j] = hoc;
    }
    redde numerus;
}

/* inscriptio tituli in indice; NIHIL si abest */
interior StatuaeInscriptio*
inscriptio_tituli (
       Xar* index,
    chorda  titulus)
{
    i32 k;

    per (k = ZEPHYRUM; index && k < xar_numerus(index); k++)
    {
        StatuaeInscriptio* x =
            (StatuaeInscriptio*)xar_obtinere(index, k);

        si (x && chorda_aequalis(x->titulus, titulus))
        {
            redde x;
        }
    }
    redde NIHIL;
}

interior vacuum
recusatam_nuntiare (
    constans character* via,
    constans character* causa,
                chorda  additum)
{
    fprintf(stderr, "%s: recusata: %s%s%.*s\n", via, causa,
        additum.mensura ? ": " : "", (integer)additum.mensura,
        (constans character*)additum.datum);
}

/* plagulam unam componere et in volumen addere; titulus rei in
 * *titulus (vacuus si legi non potuit). FALSUM = recusata (causa
 * iam nuntiata) */
interior b32
rem_condere (
    constans character* via,
               Volumen* volumen,
                chorda* tituli,
                   s32  titulorum,
                chorda* titulus,
                 Summa* summa,
               Piscina* piscina)
{
      PlasticaFructus  f;
        StatuaFructus  relecta;
    StatuaeInscriptio* prior;
    StatuaeInscriptio* nova;
               chorda  vacua;
               chorda  textus;
               chorda  octeti;
               chorda  sigillum;
                  s32  k;

    vacua.datum       = NIHIL;
    vacua.mensura     = ZEPHYRUM;
    titulus->datum    = NIHIL;
    titulus->mensura  = ZEPHYRUM;
    textus            = filum_legere_totum(via, piscina);
    si (textus.mensura == ZEPHYRUM)
    {
        recusatam_nuntiare(via, "plagula legi non potest", vacua);
        redde FALSUM;
    }
    f = plastica_legere(textus, piscina);
    si (!f.successus)
    {
        fprintf(stderr, "%s:%u: %.*s\n", via, (insignatus)f.linea,
            (integer)f.error.mensura,
            (constans character*)f.error.datum);
        redde FALSUM;
    }
    /* nodus 0 radix est: titulus eius titulus rei */
    *titulus = f.partes[ZEPHYRUM];
    si (!statuae_titulus_validus(*titulus))
    {
        recusatam_nuntiare(via, "titulus rei invalidus (I..LXIV"
            " octeti, sine '/')", *titulus);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < titulorum; k++)
    {
        si (chorda_aequalis(tituli[k], *titulus))
        {
            recusatam_nuntiare(via, "titulus iam ab alia plagula"
                " huius cursus datus", *titulus);
            redde FALSUM;
        }
    }
    si (   inscriptio_tituli(statuae_enumerare_generis(volumen,
               STATUAE_GENUS_STATUA, piscina), *titulus)
        || inscriptio_tituli(statuae_enumerare_generis(volumen,
               STATUAE_GENUS_LOCUS, piscina), *titulus))
    {
        recusatam_nuntiare(via, "titulus iam persona aut locus in"
            " volumine est", *titulus);
        redde FALSUM;
    }
    octeti = statua_glb_scribere(f.statua, f.partes, piscina);
    si (!octeti.datum)
    {
        recusatam_nuntiare(via, "res in GLB scribi non potest",
            vacua);
        redde FALSUM;
    }
    relecta = statua_glb_legere(octeti, piscina);
    si (!relecta.successus)
    {
        recusatam_nuntiare(via, "GLB scriptum relegi non potest",
            relecta.error);
        redde FALSUM;
    }
    prior = inscriptio_tituli(statuae_enumerare_generis(volumen,
        STATUAE_GENUS_RES, piscina), *titulus);
    sigillum = prior ? prior->sigillum_hex : vacua;
    si (!statuae_addere_generis(volumen, *titulus, octeti,
        STATUAE_GENUS_RES))
    {
        recusatam_nuntiare(via, "volumen additionem recusavit",
            chorda_ex_literis(volumen_error(volumen)
                ? volumen_error(volumen) : "?", piscina));
        redde FALSUM;
    }
    nova = inscriptio_tituli(statuae_enumerare_generis(volumen,
        STATUAE_GENUS_RES, piscina), *titulus);
    si (!prior)
    {
        summa->additae++;
        imprimere("addita    ");
    }
    alioquin si (nova && chorda_aequalis(nova->sigillum_hex, sigillum))
    {
        summa->eaedem++;
        imprimere("eadem     ");
    }
    alioquin
    {
        summa->mutatae++;
        imprimere("mutata    ");
    }
    imprimere("%.*s\n", (integer)titulus->mensura,
        (constans character*)titulus->datum);
    redde VERUM;
}

interior s32
exsequi_condere (
    constans character* via_voluminis,
    constans character* fontes,
               Piscina* piscina)
{
      Volumen* volumen;
          Xar* index;
        Summa  summa;
       chorda* nomina;
       chorda* tituli;
    character  via[VIA_MAXIMA];
          s32  numerus;
          s32  titulorum = ZEPHYRUM;
          s32  k;
          s32  j;

    memset(&summa, ZEPHYRUM, magnitudo(summa));
    nomina = (chorda*)piscina_allocare(piscina,
        PLAGULAE_MAXIMAE * magnitudo(chorda));
    tituli = (chorda*)piscina_allocare(piscina,
        PLAGULAE_MAXIMAE * magnitudo(chorda));
    si (!nomina || !tituli)
    {
        fprintf(stderr, "plastica: memoria deficit\n");
        redde I;
    }
    numerus = plagulas_colligere(fontes, nomina, piscina);
    si (numerus < ZEPHYRUM)
    {
        fprintf(stderr, "plastica: directorium '%s' legi non potest"
            " (aut plus quam %u plagulae)\n", fontes,
            (insignatus)PLAGULAE_MAXIMAE);
        redde I;
    }
    si (numerus == ZEPHYRUM)
    {
        fprintf(stderr, "plastica: nullae plagulae *.stml in '%s'\n",
            fontes);
        redde I;
    }
    volumen = volumen_aperire_aut_creare(piscina, via_voluminis);
    si (!volumen)
    {
        fprintf(stderr, "plastica: volumen %s aperiri non potest\n",
            via_voluminis);
        redde I;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        chorda titulus;

        si (strlen(fontes) + (memoriae_index)nomina[k].mensura + II
            > VIA_MAXIMA)
        {
            fprintf(stderr, "plastica: via nimis longa in '%s'\n",
                fontes);
            summa.recusatae++;
            perge;
        }
        sprintf(via, "%s/%.*s", fontes, (integer)nomina[k].mensura,
            (constans character*)nomina[k].datum);
        si (!rem_condere(via, volumen, tituli, titulorum, &titulus,
            &summa, piscina))
        {
            summa.recusatae++;
        }
        si (titulus.mensura)
        {
            tituli[titulorum++] = titulus;
        }
    }
    index = statuae_enumerare_generis(volumen, STATUAE_GENUS_RES,
        piscina);
    per (k = ZEPHYRUM; index && k < (s32)xar_numerus(index); k++)
    {
        StatuaeInscriptio* x =
            (StatuaeInscriptio*)xar_obtinere(index, (i32)k);

        per (j = ZEPHYRUM; j < titulorum; j++)
        {
            si (chorda_aequalis(tituli[j], x->titulus))
            {
                frange;
            }
        }
        si (j == titulorum)
        {
            imprimere("sine fonte: %.*s\n",
                (integer)x->titulus.mensura,
                (constans character*)x->titulus.datum);
        }
    }
    imprimere("plastica: %u additae, %u mutatae, %u eaedem,"
        " %u recusatae\n", (insignatus)summa.additae,
        (insignatus)summa.mutatae, (insignatus)summa.eaedem,
        (insignatus)summa.recusatae);
    volumen_claudere(volumen);
    redde summa.recusatae ? I : ZEPHYRUM;
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
    si (strcmp(argv[I], "condere") == ZEPHYRUM)
    {
        constans character* volumen  = NIHIL;
        constans character* fontes   = NIHIL;

        per (k = II; k < argc; k++)
        {
            b32 est_volumen = strcmp(argv[k], "-volumen") == ZEPHYRUM;

            si (   (!est_volumen
                    && strcmp(argv[k], "-fontes") != ZEPHYRUM)
                || k + I >= argc || (est_volumen ? volumen : fontes))
            {
                redde usum_imprimere();
            }
            si (est_volumen)
            {
                volumen = argv[++k];
            }
            alioquin
            {
                fontes = argv[++k];
            }
        }
        si (!volumen)
        {
            redde usum_imprimere();
        }
        piscina  = piscina_generare_dynamicum("plastica", M * M);
        r        = exsequi_condere(volumen,
            fontes ? fontes : FONTES_ORDINARII, piscina);
        piscina_destruere(piscina);
        redde r;
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
