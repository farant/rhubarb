/* negotia - instrumentum: negotia et facultates in volumine vici
 * (project-specs/negotia.md; plan negotia-plan.md T2). Logica tota
 * in lib/negotia.c; hic argumenta et effusio sola.
 *
 * Usus (vexilla singula linea: -volumen V in omni imperio; ordinarie
 * ~/.rhubarb/paginae-proba.volumen, volumen vici vivi):
 *
 *   negotia index
 *   negotia creare <titulus> [-descriptio T]            -> id
 *   negotia ostendere <negotium>
 *   negotia facultas <negotium> <titulus> [-status S]
 *                    [-corpus T | -corpus-filum F]       -> id
 *   negotia mutare <facultas> [-titulus T] [-status S]
 *                  [-corpus T | -corpus-filum F]
 *   negotia revisiones <facultas>
 *   negotia involvere <negotium> <sessio> [-via D]
 *   negotia pagina <negotium> <titulus> -genus pictor|adumbratio
 *                  [-corpus T | -corpus-filum F]        -> id
 *   negotia pagina-mutare <pagina> [-titulus T]
 *                  [-corpus T | -corpus-filum F]
 *   negotia paginae <negotium>  ("<suffixum>  <genus>  <titulus>")
 *   negotia png <pagina> <via.png>       adumbratio aut pictura
 *   negotia png -filum <F.stml> <via.png>  filum sine pagina
 *
 * <negotium>, <facultas>: id plenum aut suffixum unicum. S: planned |
 * partial | shipped (aut proposita | inchoata | perfecta; ordinarie
 * planned). mutare solum campos nominatos reponit (ceteri ex statu
 * praesenti). -corpus-filum per filum_* legitur ("-" = stdin).
 * involvere directorium colloquii servat (T6: Resume ibi): -via D,
 * ordinarie directorium currens (sessio ipsa vocat).
 *
 * Effusio: ids plena (creare, facultas); in indicibus suffixum VI
 * characterum + titulus. index: "<suffixum>  <titulus>  3 shipped ·
 * 2 partial · 4 planned · 2 conversations". ostendere: negotium,
 * descriptio, facultates (status, suffixum, titulus), colloquia (cum
 * via servata).
 * revisiones: "<momentum>  <status>  <linea prima corporis>",
 * vetustissima prima.
 *
 * Exitus: 0 bene; 1 refutatum (causa nominata; suffixum ambiguum
 * candidatos enumerat); 2 usus.
 *
 * Aedificatio: ./tools/negotia_struere.sh -> bin/negotia (in arbore
 * laboris: ./bin/negotia; installatio in ~ res main est). */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur -
 * sine ea bin/fabrica binarium ignotum iudicat */
/* <aedilis obiectum="build/fabrica/provenientia/negotia.c"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "xar.h"
#include "filum.h"
#include "volumen.h"
#include "negotia.h"
#include "via.h"
#include "adumbratio.h"
#include "internamentum.h"
#include "imago_png.h"
#include "pictor_documentum.h"
#include "thema.h"
#include "provenientia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VOLUMEN_ORDINARIUM "/.rhubarb/paginae-proba.volumen"

/* vexilla quae valorem ferunt (bitus in Imperium.vexilla) */
#define VEX_DESCRIPTIO   I
#define VEX_STATUS       II
#define VEX_CORPUS       IV
#define VEX_CORPUS_FILUM VIII
#define VEX_TITULUS      XVI
#define VEX_VIA          XXXII
#define VEX_GENUS        LXIV
#define VEX_FILUM        CXXVIII

/* superficies ordinaria adumbrationis sine latitudine declarata
 * (latus dextrum vici) */
#define ADUMBRATIO_LATITUDO  DC
#define ADUMBRATIO_ALTITUDO  DCC

nomen enumeratio {
    IMP_INDEX = 0,
    IMP_CREARE,
    IMP_OSTENDERE,
    IMP_FACULTAS,
    IMP_MUTARE,
    IMP_REVISIONES,
    IMP_INVOLVERE,
    IMP_PAGINA,
    IMP_PAGINAM_MUTARE,
    IMP_PAGINAE,
    IMP_PNG
} ImperiumGenus;

nomen structura {
     constans character* titulus;
                    i32  positiva;   /* argumenta sine vexillo */
                    i32  vexilla;    /* VEX_* permissa */
                    b32  scribit;    /* volumen creari licet */
} Imperium;

hic_manens constans Imperium IMPERIA[] = {
    { "index",      ZEPHYRUM, ZEPHYRUM,                    FALSUM },
    { "creare",     I,        VEX_DESCRIPTIO,              VERUM  },
    { "ostendere",  I,        ZEPHYRUM,                    FALSUM },
    { "facultas",   II,       VEX_STATUS | VEX_CORPUS
                                  | VEX_CORPUS_FILUM,      VERUM  },
    { "mutare",     I,        VEX_TITULUS | VEX_STATUS
                                  | VEX_CORPUS
                                  | VEX_CORPUS_FILUM,      VERUM  },
    { "revisiones", I,        ZEPHYRUM,                    FALSUM },
    { "involvere",  II,       VEX_VIA,                     VERUM  },
    { "pagina",     II,       VEX_GENUS | VEX_CORPUS
                                  | VEX_CORPUS_FILUM,      VERUM  },
    { "pagina-mutare", I,     VEX_TITULUS | VEX_CORPUS
                                  | VEX_CORPUS_FILUM,      VERUM  },
    { "paginae",    I,        ZEPHYRUM,                    FALSUM },
    { "png",        II,       VEX_FILUM,                   FALSUM }
};

#define IMPERIA_NUMERUS \
    ((i32)(magnitudo(IMPERIA) / magnitudo(IMPERIA[ZEPHYRUM])))

nomen structura {
     constans character* positiva[III];
                    i32  numerus_positivorum;
                    i32  vexilla;    /* VEX_* data */
     constans character* volumen;
     constans character* descriptio;
     constans character* status;
     constans character* corpus;
     constans character* corpus_filum;
     constans character* titulus;
     constans character* via;
     constans character* genus;
     constans character* filum;
} Argumenta;

interior s32
usum_imprimere (vacuum)
{
    fprintf(stderr,
        "usus: negotia index\n"
        "      negotia creare <titulus> [-descriptio T]\n"
        "      negotia ostendere <negotium>\n"
        "      negotia facultas <negotium> <titulus> [-status S]"
        " [-corpus T | -corpus-filum F]\n"
        "      negotia mutare <facultas> [-titulus T] [-status S]"
        " [-corpus T | -corpus-filum F]\n"
        "      negotia revisiones <facultas>\n"
        "      negotia involvere <negotium> <sessio> [-via D]\n"
        "      negotia pagina <negotium> <titulus> -genus"
        " pictor|adumbratio [-corpus T | -corpus-filum F]\n"
        "      negotia pagina-mutare <pagina> [-titulus T]"
        " [-corpus T | -corpus-filum F]\n"
        "      negotia paginae <negotium>\n"
        "      negotia png <pagina> <via.png>\n"
        "      negotia png -filum <F.stml> <via.png>\n"
        "      (omnia: -volumen V; S: planned | partial | shipped;"
        " F '-' = stdin)\n");
    redde II;
}

/* argv[II..] in Argumenta; FALSUM si vexillum ignotum, valor deest,
 * aut positiva nimia */
interior b32
argumenta_legere (
       integer   argc,
     character** argv,
     Argumenta*  a)
{
    integer k;

    memset(a, ZEPHYRUM, magnitudo(*a));
    per (k = II; k < argc; k++)
    {
        constans character* v = argv[k];

        si (v[ZEPHYRUM] != '-' || v[I] == '\0')
        {
            si (a->numerus_positivorum >= III)
            {
                redde FALSUM;
            }
            a->positiva[a->numerus_positivorum++] = v;
            perge;
        }
        si (k + I >= argc)
        {
            redde FALSUM;
        }
        si (strcmp(v, "-volumen") == ZEPHYRUM)
        {
            a->volumen = argv[++k];
        }
        alioquin si (strcmp(v, "-descriptio") == ZEPHYRUM)
        {
            a->descriptio  = argv[++k];
            a->vexilla     |= VEX_DESCRIPTIO;
        }
        alioquin si (strcmp(v, "-status") == ZEPHYRUM)
        {
            a->status   = argv[++k];
            a->vexilla  |= VEX_STATUS;
        }
        alioquin si (strcmp(v, "-corpus") == ZEPHYRUM)
        {
            a->corpus   = argv[++k];
            a->vexilla  |= VEX_CORPUS;
        }
        alioquin si (strcmp(v, "-corpus-filum") == ZEPHYRUM)
        {
            a->corpus_filum  = argv[++k];
            a->vexilla       |= VEX_CORPUS_FILUM;
        }
        alioquin si (strcmp(v, "-titulus") == ZEPHYRUM)
        {
            a->titulus = argv[++k];
            a->vexilla |= VEX_TITULUS;
        }
        alioquin si (strcmp(v, "-via") == ZEPHYRUM)
        {
            a->via      = argv[++k];
            a->vexilla  |= VEX_VIA;
        }
        alioquin si (strcmp(v, "-genus") == ZEPHYRUM)
        {
            a->genus    = argv[++k];
            a->vexilla  |= VEX_GENUS;
        }
        alioquin si (strcmp(v, "-filum") == ZEPHYRUM)
        {
            a->filum    = argv[++k];
            a->vexilla  |= VEX_FILUM;
        }
        alioquin
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* suffixum VI characterum (aut id totum si brevius) */
interior chorda
suffixum (
    chorda id)
{
    chorda s = id;

    si (s.mensura > VI)
    {
        s.datum    += s.mensura - VI;
        s.mensura  = VI;
    }
    redde s;
}

/* lineas omnes cum indentatione II spatiorum */
interior vacuum
lineas_imprimere (
    chorda c)
{
    i32 initium = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k <= c.mensura; k++)
    {
        si (k == c.mensura || c.datum[k] == '\n')
        {
            imprimere("  %.*s\n", (integer)(k - initium),
                (constans character*)(c.datum + initium));
            initium = k + I;
        }
    }
}

/* corpus ex filo aut stdin ("-"); lineae novae finales demuntur ut
 * -corpus et -corpus-filum par textum pariant */
interior b32
corpus_legere (
    constans character* via,
               Piscina* piscina,
                chorda* corpus_out)
{
    FilumOctetorumLector* lector;
       ChordaAedificator* aed;
                      i8  capsa[MMXLVIII];
                     s32  lecti;
                  chorda  pars;

    lector = filum_octeti_aperire(strcmp(via, "-") == ZEPHYRUM
        ? "/dev/stdin" : via, piscina);
    si (!lector)
    {
        redde FALSUM;
    }
    aed = chorda_aedificator_creare(piscina, MMXLVIII);
    dum ((lecti = filum_octeti_legere(lector, capsa,
        (i32)magnitudo(capsa))) > ZEPHYRUM)
    {
        pars.datum    = capsa;
        pars.mensura  = (i32)lecti;
        (vacuum)chorda_aedificator_appendere_chorda(aed, pars);
    }
    filum_octeti_claudere(lector);
    si (lecti < ZEPHYRUM)
    {
        redde FALSUM;
    }
    *corpus_out = chorda_aedificator_spectare(aed);
    dum (   corpus_out->mensura > ZEPHYRUM
         && corpus_out->datum[corpus_out->mensura - I] == '\n')
    {
        corpus_out->mensura--;
    }
    redde VERUM;
}

interior constans Negotium*
negotium_resolvere (
    constans Negotia* n,
  constans character* verbum,
             Piscina* piscina)
{
    constans Negotium* x;
                  b32  ambiguum = FALSUM;
               chorda  id;
                  i32  k;

    id  = chorda_ex_literis(verbum, piscina);
    x   = negotia_negotium_invenire(n, id, &ambiguum);
    si (x)
    {
        redde x;
    }
    si (!ambiguum)
    {
        fprintf(stderr, "negotia: negotium '%s' ignotum\n", verbum);
        redde NIHIL;
    }
    fprintf(stderr, "negotia: suffixum '%s' ambiguum:\n", verbum);
    per (k = ZEPHYRUM; k < negotia_numerus(n); k++)
    {
        x = negotia_index(n, k);
        si (chorda_terminatur(x->id, id))
        {
            fprintf(stderr, "  %.*s  %.*s\n", (integer)x->id.mensura,
                (constans character*)x->id.datum,
                (integer)x->titulus.mensura,
                (constans character*)x->titulus.datum);
        }
    }
    redde NIHIL;
}

interior constans NegotiiFacultas*
facultatem_resolvere (
    constans Negotia* n,
  constans character* verbum,
             Piscina* piscina)
{
    constans NegotiiFacultas* f;
                         b32  ambiguum = FALSUM;
                      chorda  id;
                         i32  k;
                         i32  j;

    id  = chorda_ex_literis(verbum, piscina);
    f   = negotia_facultatem_invenire(n, id, &ambiguum);
    si (f)
    {
        redde f;
    }
    si (!ambiguum)
    {
        fprintf(stderr, "negotia: facultas '%s' ignota\n", verbum);
        redde NIHIL;
    }
    fprintf(stderr, "negotia: suffixum '%s' ambiguum:\n", verbum);
    per (k = ZEPHYRUM; k < negotia_numerus(n); k++)
    {
        Xar* fs = negotia_facultates(n, negotia_index(n, k)->id,
            piscina);

        per (j = ZEPHYRUM; fs && j < xar_numerus(fs); j++)
        {
            f = (constans NegotiiFacultas*)xar_obtinere(fs, j);
            si (chorda_terminatur(f->id, id))
            {
                fprintf(stderr, "  %.*s  %.*s\n",
                    (integer)f->id.mensura,
                    (constans character*)f->id.datum,
                    (integer)f->titulus.mensura,
                    (constans character*)f->titulus.datum);
            }
        }
    }
    redde NIHIL;
}

interior constans NegotiiPagina*
paginam_resolvere (
      constans Negotia* n,
    constans character* verbum,
               Piscina* piscina)
{
    constans NegotiiPagina* g;
                       b32  ambiguum = FALSUM;
                    chorda  id;
                       i32  k;
                       i32  j;

    id  = chorda_ex_literis(verbum, piscina);
    g   = negotia_paginam_invenire(n, id, &ambiguum);
    si (g)
    {
        redde g;
    }
    si (!ambiguum)
    {
        fprintf(stderr, "negotia: pagina '%s' ignota\n", verbum);
        redde NIHIL;
    }
    fprintf(stderr, "negotia: suffixum '%s' ambiguum:\n", verbum);
    per (k = ZEPHYRUM; k < negotia_numerus(n); k++)
    {
        Xar* gs = negotia_paginae(n, negotia_index(n, k)->id, piscina);

        per (j = ZEPHYRUM; gs && j < xar_numerus(gs); j++)
        {
            g = (constans NegotiiPagina*)xar_obtinere(gs, j);
            si (chorda_terminatur(g->id, id))
            {
                fprintf(stderr, "  %.*s  %.*s\n",
                    (integer)g->id.mensura,
                    (constans character*)g->id.datum,
                    (integer)g->titulus.mensura,
                    (constans character*)g->titulus.datum);
            }
        }
    }
    redde NIHIL;
}

/* "<suffixum>  <genus>  <titulus>" per paginam, ordine creationis */
interior s32
paginas_imprimere (
      constans Negotia* n,
    constans character* verbum,
               Piscina* piscina)
{
    constans Negotium* x;
                  Xar* gs;
                  i32  j;

    x = negotium_resolvere(n, verbum, piscina);
    si (!x)
    {
        redde I;
    }
    gs = negotia_paginae(n, x->id, piscina);
    per (j = ZEPHYRUM; gs && j < xar_numerus(gs); j++)
    {
         constans NegotiiPagina* g;
                         chorda  s;

        g = (constans NegotiiPagina*)xar_obtinere(gs, j);
        s = suffixum(g->id);
        imprimere("%.*s  %s  %.*s\n", (integer)s.mensura,
            (constans character*)s.datum,
            negotia_pagina_genus_titulus(g->genus),
            (integer)g->titulus.mensura,
            (constans character*)g->titulus.datum);
    }
    redde ZEPHYRUM;
}

/* adumbratio -> PNG; vitium nominatum (exitus I), PNG tamen paginae
 * vitii scribitur ut videri possit */
interior s32
adumbrationem_scribere (
                chorda  documentum,
    constans character* via,
               Piscina* piscina)
{
    AdumbratioResultus r;

    thema_initiare();
    r = adumbrationem_reddere(documentum, ADUMBRATIO_LATITUDO,
        ADUMBRATIO_ALTITUDO, piscina, internamentum_creare(piscina));
    si (!adumbrationem_png_scribere(&r, via, piscina))
    {
        fprintf(stderr, "negotia: PNG scribi non potest: %s\n", via);
        redde I;
    }
    si (r.vitium.mensura > ZEPHYRUM)
    {
        fprintf(stderr, "negotia: adumbratio: %.*s\n",
            (integer)r.vitium.mensura,
            (constans character*)r.vitium.datum);
        redde I;
    }
    redde ZEPHYRUM;
}

/* corpus ex -corpus aut -corpus-filum; *datum FALSUM si neutrum */
interior b32
corpus_ex_argumentis (
    constans Argumenta* a,
               Piscina* piscina,
                chorda* corpus_out,
                   b32* datum_out)
{
    *datum_out = FALSUM;
    si (a->corpus)
    {
        *corpus_out  = chorda_ex_literis(a->corpus, piscina);
        *datum_out   = VERUM;
        redde VERUM;
    }
    si (a->corpus_filum)
    {
        si (!corpus_legere(a->corpus_filum, piscina, corpus_out))
        {
            fprintf(stderr, "negotia: filum '%s' legi non potest\n",
                a->corpus_filum);
            redde FALSUM;
        }
        *datum_out = VERUM;
    }
    redde VERUM;
}

interior s32
indicem_imprimere (
    Negotia* n,
    Piscina* piscina)
{
    i32 k;

    per (k = ZEPHYRUM; k < negotia_numerus(n); k++)
    {
        constans Negotium* x = negotia_index(n, k);
                   chorda  s = suffixum(x->id);
                   chorda  t = negotia_summarium(n, x->id, piscina);

        imprimere("%.*s  %.*s%s%.*s\n", (integer)s.mensura,
            (constans character*)s.datum, (integer)x->titulus.mensura,
            (constans character*)x->titulus.datum, t.mensura
            > ZEPHYRUM ? "  " : "", (integer)t.mensura,
            (constans character*)t.datum);
    }
    redde ZEPHYRUM;
}

interior s32
ostendere (
             Negotia* n,
  constans character* verbum,
             Piscina* piscina)
{
    constans Negotium* x;
                  Xar* fs;
                  Xar* cs;
                  i32  j;

    x = negotium_resolvere(n, verbum, piscina);
    si (!x)
    {
        redde I;
    }
    imprimere("%.*s  %.*s\n", (integer)x->id.mensura,
        (constans character*)x->id.datum, (integer)x->titulus.mensura,
        (constans character*)x->titulus.datum);
    si (x->descriptio.mensura > ZEPHYRUM)
    {
        lineas_imprimere(x->descriptio);
    }
    fs = negotia_facultates(n, x->id, piscina);
    imprimere("facultates (%u):\n",
        (insignatus integer)(fs ? xar_numerus(fs) : ZEPHYRUM));
    per (j = ZEPHYRUM; fs && j < xar_numerus(fs); j++)
    {
        constans NegotiiFacultas* f;
                          chorda  s;

        f = (constans NegotiiFacultas*)xar_obtinere(fs, j);
        s = suffixum(f->id);
        imprimere("  %s  %.*s  %.*s\n", negotia_status_titulus(
            f->status), (integer)s.mensura,
            (constans character*)s.datum, (integer)f->titulus.mensura,
            (constans character*)f->titulus.datum);
    }
    cs = negotia_colloquia(n, x->id, piscina);
    imprimere("colloquia (%u):\n",
        (insignatus integer)(cs ? xar_numerus(cs) : ZEPHYRUM));
    per (j = ZEPHYRUM; cs && j < xar_numerus(cs); j++)
    {
         chorda* c    = (chorda*)xar_obtinere(cs, j);
         chorda  via  = negotia_colloquii_via(n, x->id, *c);

        imprimere("  %.*s%s%.*s\n", (integer)c->mensura,
            (constans character*)c->datum, via.mensura > ZEPHYRUM
            ? "  " : "", (integer)via.mensura,
            (constans character*)via.datum);
    }
    redde ZEPHYRUM;
}

interior s32
revisiones_imprimere (
             Negotia* n,
  constans character* verbum,
             Piscina* piscina)
{
    constans NegotiiFacultas* f;
                         Xar* rs;
                         i32  j;

    f = facultatem_resolvere(n, verbum, piscina);
    si (!f)
    {
        redde I;
    }
    rs = negotia_revisiones(n, f->id, piscina);
    per (j = ZEPHYRUM; rs && j < xar_numerus(rs); j++)
    {
        constans NegotiiFacultas* r;
                             i32  finis = ZEPHYRUM;

        r = (constans NegotiiFacultas*)xar_obtinere(rs, j);
        dum (   finis < r->corpus.mensura
             && r->corpus.datum[finis] != '\n')
        {
            finis++;
        }
        imprimere("%.*s  %s  %.*s\n", (integer)r->momentum.mensura,
            (constans character*)r->momentum.datum,
            negotia_status_titulus(r->status), (integer)finis,
            (constans character*)r->corpus.datum);
    }
    redde ZEPHYRUM;
}

/* sessio Claude: [0-9A-Za-z-]+ (T6 eam in mandato crustae ponit) */
interior b32
sessio_valida (
    constans character* s)
{
    i32 k;

    si (s[ZEPHYRUM] == '\0')
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; s[k] != '\0'; k++)
    {
        character c = s[k];

        si (!(   (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z')
              || (c >= 'a' && c <= 'z') || c == '-'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* negotia-paginae T4: spatium picturae paginae "negotia/<id plenum>" */
interior constans character*
spatium_paginae (
    chorda  id,
   Piscina* piscina)
{
    redde chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(
        "negotia/", piscina), id, piscina), piscina);
}

/* T4: pictura paginae -> PNG (compositum); absens nominatur */
interior s32
picturam_scribere (
                Volumen* volumen,
                 chorda  id,
     constans character* via,
                Piscina* piscina)
{
     PictorDocumentum* doc;
           PngFructus  fr;

    doc = pictor_documentum_aperire(piscina, internamentum_creare(
        piscina), volumen, spatium_paginae(id, piscina));
    si (!doc)
    {
        fprintf(stderr, "negotia: pictura paginae abest\n");
        redde I;
    }
    fr = imago_png_scribere(&doc->proiectio, via, piscina);
    si (!fr.successus)
    {
        fprintf(stderr, "negotia: PNG scribi non potest: %s\n", via);
        redde I;
    }
    redde ZEPHYRUM;
}

interior s32
exsequi (
               Volumen* volumen,
               Negotia* n,
         ImperiumGenus  g,
    constans Argumenta* a,
                   s32  status,
               Piscina* piscina)
{
    chorda id;
    chorda corpus;
       b32 corpus_datum;

    commutatio (g)
    {
        casus IMP_INDEX:
            redde indicem_imprimere(n, piscina);
        casus IMP_OSTENDERE:
            redde ostendere(n, a->positiva[ZEPHYRUM], piscina);
        casus IMP_REVISIONES:
            redde revisiones_imprimere(n, a->positiva[ZEPHYRUM],
                piscina);
        casus IMP_CREARE:
            id = negotia_negotium_creare(n, chorda_ex_literis(
                a->positiva[ZEPHYRUM], piscina), chorda_ex_literis(
                a->descriptio ? a->descriptio : "", piscina));
            frange;
        casus IMP_FACULTAS:
        {
            constans Negotium* x;

            x = negotium_resolvere(n, a->positiva[ZEPHYRUM], piscina);
            si (   !x || !corpus_ex_argumentis(a, piscina, &corpus,
                &corpus_datum))
            {
                redde I;
            }
            id = negotia_facultatem_creare(n, x->id, chorda_ex_literis(
                a->positiva[I], piscina), status, corpus_datum ? corpus
                : chorda_ex_literis("", piscina));
            frange;
        }
        casus IMP_MUTARE:
        {
            constans NegotiiFacultas* f;
                     NegotiiFacultas  v;

            f = facultatem_resolvere(n, a->positiva[ZEPHYRUM], piscina);
            si (   !f || !corpus_ex_argumentis(a, piscina, &corpus,
                &corpus_datum))
            {
                redde I;
            }
            v = *f;
            si (!negotia_facultatem_mutare(n, v.id, a->titulus
                ? chorda_ex_literis(a->titulus, piscina) : v.titulus,
                a->status ? status : v.status, corpus_datum ? corpus
                : v.corpus))
            {
                fprintf(stderr, "negotia: mutatio refutata (titulus"
                    " vacuus?)\n");
                redde I;
            }
            redde ZEPHYRUM;
        }
        casus IMP_INVOLVERE:
        {
            constans Negotium* x;

            si (!sessio_valida(a->positiva[I]))
            {
                fprintf(stderr, "negotia: sessio '%s' invalida"
                    " ([0-9A-Za-z-]+)\n", a->positiva[I]);
                redde I;
            }
            x = negotium_resolvere(n, a->positiva[ZEPHYRUM], piscina);
            si (   !x || !negotia_colloquium_adiungere(n, x->id,
                chorda_ex_literis(a->positiva[I], piscina),
                via_absoluta(
                chorda_ex_literis(a->via ? a->via : ".", piscina),
                piscina)))
            {
                redde I;
            }
            redde ZEPHYRUM;
        }
        casus IMP_PNG:
        {
            constans NegotiiPagina* g;

            g = paginam_resolvere(n, a->positiva[ZEPHYRUM], piscina);
            si (!g)
            {
                redde I;
            }
            si (g->genus == NEGOTIA_PAGINA_PICTOR)
            {
                redde picturam_scribere(volumen, g->id, a->positiva[I],
                    piscina);
            }
            redde adumbrationem_scribere(g->corpus, a->positiva[I],
                piscina);
        }
        casus IMP_PAGINAE:
            redde paginas_imprimere(n, a->positiva[ZEPHYRUM], piscina);
        casus IMP_PAGINA:
        {
            constans Negotium* x;
                          s32  genus;

            si (   !a->genus || !negotia_pagina_genus_legere(
                chorda_ex_literis(a->genus, piscina), &genus))
            {
                fprintf(stderr, "negotia: -genus pictor aut"
                    " adumbratio\n");
                redde II;
            }
            x = negotium_resolvere(n, a->positiva[ZEPHYRUM], piscina);
            si (   !x || !corpus_ex_argumentis(a, piscina, &corpus,
                &corpus_datum))
            {
                redde I;
            }
            id = negotia_paginam_creare(n, x->id, genus,
                chorda_ex_literis(a->positiva[I], piscina), corpus_datum
                ? corpus : chorda_ex_literis("", piscina));
            /* T4: pictura vacua (magnitudo adumbrationis) */
            si (   id.mensura > ZEPHYRUM
                && genus == NEGOTIA_PAGINA_PICTOR
                && !pictor_documentum_creare(piscina,
                       internamentum_creare(piscina), volumen,
                       spatium_paginae(id, piscina),
                       ADUMBRATIO_LATITUDO, ADUMBRATIO_ALTITUDO, LXIV))
            {
                fprintf(stderr, "negotia: pictura condi non potest\n");
                redde I;
            }
            frange;
        }
        casus IMP_PAGINAM_MUTARE:
        {
            constans NegotiiPagina* g;
                     NegotiiPagina  v;

            g = paginam_resolvere(n, a->positiva[ZEPHYRUM], piscina);
            si (   !g || !corpus_ex_argumentis(a, piscina, &corpus,
                &corpus_datum))
            {
                redde I;
            }
            v = *g;
            si (!negotia_paginam_mutare(n, v.id, a->titulus
                ? chorda_ex_literis(a->titulus, piscina) : v.titulus,
                corpus_datum ? corpus : v.corpus))
            {
                fprintf(stderr, "negotia: mutatio paginae refutata"
                    " (titulus vacuus?)\n");
                redde I;
            }
            redde ZEPHYRUM;
        }
        ordinarius:
            redde II;
    }
    /* creare, facultas, pagina: id plenum */
    si (id.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "negotia: refutatum (titulus vacuus?)\n");
        redde I;
    }
    imprimere("%.*s\n", (integer)id.mensura,
        (constans character*)id.datum);
    redde ZEPHYRUM;
}

externus constans ProvenientiaRelatio provenientia_negotia;

s32
principale (
         integer   argc,
       character** argv)
{
               Piscina* piscina;
               Volumen* volumen;
               Negotia* n;
             Argumenta  a;
     constans Imperium* imp  = NIHIL;
         ImperiumGenus  g    = IMP_INDEX;
    constans character* via;
                   s32  status = (s32)NEGOTIA_PROPOSITA;
                   s32  exitus;
                   i32  k;

    si (provenientia_respondere((s32)argc, argv, &provenientia_negotia))
    {
        redde ZEPHYRUM;
    }
    si (argc < II || !argumenta_legere(argc, argv, &a))
    {
        redde usum_imprimere();
    }
    per (k = ZEPHYRUM; k < IMPERIA_NUMERUS; k++)
    {
        si (strcmp(argv[I], IMPERIA[k].titulus) == ZEPHYRUM)
        {
            imp  = &IMPERIA[k];
            g    = (ImperiumGenus)k;
        }
    }
    si (   !imp || a.numerus_positivorum != (g == IMP_PNG && a.filum
               ? I : imp->positiva)
        || (a.vexilla & ~imp->vexilla) != ZEPHYRUM
        || (a.corpus && a.corpus_filum))
    {
        redde usum_imprimere();
    }
    piscina = piscina_generare_dynamicum("negotia", M * M);
    /* png -filum: sine volumine */
    si (g == IMP_PNG && a.filum)
    {
        chorda documentum;

        si (!corpus_legere(a.filum, piscina, &documentum))
        {
            fprintf(stderr, "negotia: filum '%s' legi non potest\n",
                a.filum);
            piscina_destruere(piscina);
            redde I;
        }
        exitus = adumbrationem_scribere(documentum,
            a.positiva[ZEPHYRUM],
            piscina);
        piscina_destruere(piscina);
        redde exitus;
    }
    si (   a.status
        && !negotia_status_legere(chorda_ex_literis(a.status,
        piscina), &status))
    {
        fprintf(stderr, "negotia: status '%s' ignotus\n", a.status);
        piscina_destruere(piscina);
        redde usum_imprimere();
    }
    via = a.volumen;
    si (!via)
    {
        constans character* domus = getenv("HOME");
                 character* plena;

        si (!domus)
        {
            fprintf(stderr, "negotia: HOME deest; da -volumen\n");
            piscina_destruere(piscina);
            redde I;
        }
        plena = (character*)piscina_allocare(piscina, (memoriae_index)(
            strlen(domus) + strlen(VOLUMEN_ORDINARIUM) + I));
        strcpy(plena, domus);
        strcat(plena, VOLUMEN_ORDINARIUM);
        via = plena;
    }
    volumen = imp->scribit ? volumen_aperire_aut_creare(piscina, via)
        : volumen_aperire(piscina, via);
    n = volumen ? negotia_aperire(volumen, NIHIL, NIHIL,
        piscina) : NIHIL;
    si (!n)
    {
        fprintf(stderr, "negotia: volumen %s aperiri non potest\n",
            via);
        si (volumen)
        {
            volumen_claudere(volumen);
        }
        piscina_destruere(piscina);
        redde I;
    }
    exitus = exsequi(volumen, n, g, &a, status, piscina);
    volumen_claudere(volumen);
    piscina_destruere(piscina);
    redde exitus;
}
