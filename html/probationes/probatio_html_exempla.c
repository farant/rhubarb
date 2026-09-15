/* probatio_html_exempla.c - Porta lectoris .dat html5lib (oraculum O1)
 *
 * I. CONTRACTUS LECTORIS in memoria: speculum support.py TestData -
 *    titulus = linea detonsa '#' incipiens (etiam spatiis ducta);
 *    sectio '\n' unum caudale perdit, ultima ante '#data' duo; tituli
 *    ante '#data' primum ignorantur; EOF unum solum perdit.
 * II. MANIFESTUM TOTUM (HTML_EXEMPLA_PLAGULAE) lectum, numeri PINNATI
 *    (FONTES.md): plagulae LIV, octeti 428448, exempla MDCCVIII,
 *    fragmenta CLXXXIV, script-on VIII, script-off XXVII; omne
 *    exemplum datum et documentum fert. Per plagulam: tres plagulae
 *    NUL ferentes numerantur nominatim - grep sine '-a' NIHIL in eis
 *    videt (tabula prima MDCXXV dixit). SE METIENS: lectae et octeti
 *    asseruntur, ne 'N/N' de plagulis numquam apertis mentiatur.
 * III. CASUS LIMITIS in fixura: initus vacuus (tests1 #62), initus
 *    lineam vacuam ferens (tests3 #7), fragmentum (tests_innerHTML_1
 *    #1), script-on (tests16 #84), NUL (plain-text-unsafe #2), EOF
 *    (tests1 #114).
 * Re-venditio = mutatio nominata horum numerorum.
 */

#include "latina.h"
#include "credo.h"
#include "html_exempla.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PLAGULAE_PINNATAE        ((i32)54)
#define OCTETI_PINNATI           ((i32)428448)
#define EXEMPLA_PINNATA          ((i32)1708)
#define FRAGMENTA_PINNATA        ((i32)184)
#define SCRIPTUM_APERTA_PINNATA  ((i32)8)
#define SCRIPTUM_CLAUSA_PINNATA  ((i32)27)

nomen structura {
    constans character* suffixum;
                   i32  exempla;
} PlagulaPinnata;

hic_manens constans PlagulaPinnata PER_PLAGULAM[] = {
    /* '/' ducens: suffixum nudum 'plain-text-unsafe.dat' etiam
     * 'pending-spec-changes-plain-text-unsafe.dat' caperet */
    { "/tests1.dat",            114 },
    { "/tests16.dat",           197 },
    { "/template.dat",          108 },
    { "/tests_innerHTML_1.dat",  85 },
    { "/domjs-unsafe.dat",       49 },
    { "/plain-text-unsafe.dat",  33 },
    { "/pending-spec-changes-plain-text-unsafe.dat", 1 }
};

#define NUMERUS_PER_PLAGULAM \
    ((i32)(magnitudo(PER_PLAGULAM) / magnitudo(PER_PLAGULAM[0])))

hic_manens character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

hic_manens b32
_suffixo_terminatur (
    constans character* via,
    constans character* suffixum)
{
    size_t a = strlen(via);
    size_t b = strlen(suffixum);

    redde (b32)(a >= b && strcmp(via + (a - b), suffixum) == ZEPHYRUM);
}

/* titulus plagulae sine directorio */
hic_manens constans character*
_titulus (
    constans character* via)
{
    constans character* t = strrchr(via, '/');

    redde t == NIHIL ? via : t + I;
}

/* Exempla plagulae manifesti cuius via suffixo terminatur; NIHIL si
 * abest aut lectio fracta. */
hic_manens Xar*
_exempla_plagulae (
               Piscina* piscina,
    constans character* radix,
    constans character* suffixum)
{
    i32 i;

    per (i = ZEPHYRUM; HTML_EXEMPLA_PLAGULAE[i] != NIHIL; i++)
    {
        si (_suffixo_terminatur(HTML_EXEMPLA_PLAGULAE[i], suffixum))
        {
            character  via[DXII];
            character* textus;
                  i32  mensura = ZEPHYRUM;

            sprintf(via, "%s/%s", radix, HTML_EXEMPLA_PLAGULAE[i]);
            textus = _plagulam_legere(piscina, via, &mensura);
            si (textus == NIHIL)
            {
                redde NIHIL;
            }
            redde html_exempla_legere(piscina, textus, mensura);
        }
    }
    redde NIHIL;
}

/* exemplum numero I-basato; NIHIL extra fines */
hic_manens HtmlExemplum*
_exemplum (
    Xar* exempla,
    i32  numerus)
{
    si (   exempla == NIHIL || numerus < I
        || numerus > (i32)xar_numerus(exempla))
    {
        redde NIHIL;
    }
    redde (HtmlExemplum*)xar_obtinere(exempla, numerus - I);
}

hic_manens vacuum
_contractus_lectoris (
    Piscina* piscina)
{
    constans character* fons =
        "#errors\n"
        "ignoratum ante #data primum\n"
        "#data\n"
        "A\n"
        "\n"
        "#errors\n"
        "#document\n"
        "| \"A\"\n"
        "\n"
        "#data\n"
        "#errors\n"
        "#document-fragment\n"
        "td\n"
        "  #script-on\n"
        "#new-errors\n"
        "ignoratum\n"
        "#document\n"
        "| <td>\n";
                   Xar* exempla;
          HtmlExemplum* e;

    imprimere("\n--- I. contractus lectoris ---\n");
    exempla = html_exempla_legere(piscina, fons, (i32)strlen(fons));
    CREDO_NON_NIHIL (exempla);
    si (exempla == NIHIL)
    {
        redde;
    }
    CREDO_AEQUALIS_I32 ((i32)xar_numerus(exempla), II);

    e = _exemplum(exempla, I);
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_AEQUALIS_I32 (e->numerus, I);
        CREDO_AEQUALIS_I32 (e->linea, III);
        /* 'A' + linea vacua = initus 'A\n' (unum ex duobus demptum) */
        CREDO_CHORDA_AEQUALIS_LITERIS (e->datum, "A\n");
        CREDO_NON_NIHIL (e->errores.datum);
        CREDO_CHORDA_VACUA (e->errores);
        CREDO_CHORDA_AEQUALIS_LITERIS (e->documentum, "| \"A\"");
        CREDO_VERUM (!e->fragmentum);
        CREDO_AEQUALIS_I32 (e->scriptum,
            (i32)HTML_EXEMPLUM_SCRIPTUM_INCERTUM);
    }

    e = _exemplum(exempla, II);
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_AEQUALIS_I32 (e->numerus, II);
        CREDO_AEQUALIS_I32 (e->linea, X);
        CREDO_NON_NIHIL (e->datum.datum);
        CREDO_CHORDA_VACUA (e->datum);
        CREDO_VERUM (e->fragmentum);
        CREDO_CHORDA_AEQUALIS_LITERIS (e->contextus, "td");
        /* titulus spatiis ductus = titulus (str.strip speculatum) */
        CREDO_AEQUALIS_I32 (e->scriptum,
            (i32)HTML_EXEMPLUM_SCRIPTUM_APERTUM);
        /* EOF: '\n' unum solum demptum */
        CREDO_CHORDA_AEQUALIS_LITERIS (e->documentum, "| <td>");
    }
}

hic_manens vacuum
_manifestum_totum (
               Piscina* piscina,
    constans character* radix)
{
    i32 i;
    i32 lectae          = ZEPHYRUM;
    i32 octeti          = ZEPHYRUM;
    i32 exempla         = ZEPHYRUM;
    i32 fragmenta       = ZEPHYRUM;
    i32 aperta          = ZEPHYRUM;
    i32 clausa          = ZEPHYRUM;
    i32 data            = ZEPHYRUM;
    i32 documenta       = ZEPHYRUM;
    i32 pinnatae_visae  = ZEPHYRUM;

    imprimere("\n--- II. manifestum totum ---\n");
    per (i = ZEPHYRUM; HTML_EXEMPLA_PLAGULAE[i] != NIHIL; i++)
    {
        character  via[DXII];
        character* textus;
              i32  mensura = ZEPHYRUM;
              Xar* ex;
              i32  n;
              i32  k;
              i32  fragmenta_plagulae = ZEPHYRUM;

        sprintf(via, "%s/%s", radix, HTML_EXEMPLA_PLAGULAE[i]);
        textus = _plagulam_legere(piscina, via, &mensura);
        si (textus == NIHIL)
        {
            imprimere("  ABEST: %s\n", HTML_EXEMPLA_PLAGULAE[i]);
            CREDO_CULPA ("plagula fixurae abest");
            perge;
        }
        lectae  = lectae + I;
        octeti  = octeti + mensura;
        ex      = html_exempla_legere(piscina, textus, mensura);
        CREDO_NON_NIHIL (ex);
        si (ex == NIHIL)
        {
            perge;
        }
        n = (i32)xar_numerus(ex);
        per (k = ZEPHYRUM; k < n; k++)
        {
            HtmlExemplum* e = (HtmlExemplum*)xar_obtinere(ex, k);

            si (e->fragmentum)
            {
                fragmenta_plagulae = fragmenta_plagulae + I;
            }
            si (e->scriptum == HTML_EXEMPLUM_SCRIPTUM_APERTUM)
            {
                aperta = aperta + I;
            }
            si (e->scriptum == HTML_EXEMPLUM_SCRIPTUM_CLAUSUM)
            {
                clausa = clausa + I;
            }
            si (e->datum.datum != NIHIL)
            {
                data = data + I;
            }
            si (e->documentum.datum != NIHIL)
            {
                documenta = documenta + I;
            }
        }
        exempla    = exempla + n;
        fragmenta  = fragmenta + fragmenta_plagulae;
        imprimere("  %-44s %4d exempla %3d fragmenta %6d octeti\n",
            _titulus(HTML_EXEMPLA_PLAGULAE[i]), (integer)n,
            (integer)fragmenta_plagulae, (integer)mensura);
        per (k = ZEPHYRUM; k < NUMERUS_PER_PLAGULAM; k++)
        {
            si (_suffixo_terminatur(HTML_EXEMPLA_PLAGULAE[i],
                    PER_PLAGULAM[k].suffixum))
            {
                CREDO_AEQUALIS_I32 (n, PER_PLAGULAM[k].exempla);
                pinnatae_visae = pinnatae_visae + I;
            }
        }
    }
    imprimere("  summa: %d plagulae, %d octeti, %d exempla, "
              "%d fragmenta, script-on %d, script-off %d\n",
        (integer)lectae, (integer)octeti, (integer)exempla,
        (integer)fragmenta, (integer)aperta, (integer)clausa);
    CREDO_AEQUALIS_I32 (lectae, PLAGULAE_PINNATAE);
    CREDO_AEQUALIS_I32 (octeti, OCTETI_PINNATI);
    CREDO_AEQUALIS_I32 (exempla, EXEMPLA_PINNATA);
    CREDO_AEQUALIS_I32 (fragmenta, FRAGMENTA_PINNATA);
    CREDO_AEQUALIS_I32 (aperta, SCRIPTUM_APERTA_PINNATA);
    CREDO_AEQUALIS_I32 (clausa, SCRIPTUM_CLAUSA_PINNATA);
    CREDO_AEQUALIS_I32 (data, exempla);
    CREDO_AEQUALIS_I32 (documenta, exempla);
    CREDO_AEQUALIS_I32 (pinnatae_visae, NUMERUS_PER_PLAGULAM);
}

hic_manens vacuum
_casus_limitis (
               Piscina* piscina,
    constans character* radix)
{
             Xar* ex;
    HtmlExemplum* e;

    imprimere("\n--- III. casus limitis in fixura ---\n");

    ex = _exempla_plagulae(piscina, radix, "/tests1.dat");
    CREDO_NON_NIHIL (ex);
    e = _exemplum(ex, I);
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_AEQUALIS_I32 (e->linea, I);
        CREDO_CHORDA_AEQUALIS_LITERIS (e->datum, "Test");
        CREDO_CHORDA_AEQUALIS_LITERIS (e->errores,
            "(1,0): expected-doctype-but-got-chars");
        CREDO_CHORDA_AEQUALIS_LITERIS (e->documentum,
            "| <html>\n|   <head>\n|   <body>\n|     \"Test\"");
        CREDO_VERUM (!e->fragmentum);
    }
    /* initus VACUUS: sectio data = linea vacua una */
    e = _exemplum(ex, (i32)62);
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_AEQUALIS_I32 (e->linea, (i32)870);
        CREDO_NON_NIHIL (e->datum.datum);
        CREDO_CHORDA_VACUA (e->datum);
        CREDO_CHORDA_AEQUALIS_LITERIS (e->documentum,
            "| <html>\n|   <head>\n|   <body>");
    }
    /* EOF: '\n' unum solum demptum */
    e = _exemplum(ex, (i32)114);
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (e->datum, "<frameset>");
        CREDO_CHORDA_AEQUALIS_LITERIS (e->documentum,
            "| <html>\n|   <head>\n|   <frameset>");
    }

    /* initus lineam vacuam INTRA se ferens; textus arboris exspectatae
     * '\n' fert - valor per lineas duas scriptus */
    ex = _exempla_plagulae(piscina, radix, "/tests3.dat");
    CREDO_NON_NIHIL (ex);
    e = _exemplum(ex, VII);
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (e->datum,
            "<!DOCTYPE html><html><head></head><body><pre>\n\n"
            "foo</pre></body></html>");
        CREDO_CHORDA_AEQUALIS_LITERIS (e->documentum,
            "| <!DOCTYPE html>\n| <html>\n|   <head>\n|   <body>\n"
            "|     <pre>\n|       \"\nfoo\"");
    }

    ex = _exempla_plagulae(piscina, radix, "/tests_innerHTML_1.dat");
    CREDO_NON_NIHIL (ex);
    e = _exemplum(ex, I);
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_CHORDA_AEQUALIS_LITERIS (e->datum, "<body><span>");
        CREDO_VERUM (e->fragmentum);
        CREDO_CHORDA_AEQUALIS_LITERIS (e->contextus, "body");
        CREDO_CHORDA_AEQUALIS_LITERIS (e->documentum, "| <span>");
    }

    ex = _exempla_plagulae(piscina, radix, "/tests16.dat");
    CREDO_NON_NIHIL (ex);
    e = _exemplum(ex, (i32)84);
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_AEQUALIS_I32 (e->scriptum,
            (i32)HTML_EXEMPLUM_SCRIPTUM_APERTUM);
        CREDO_CHORDA_AEQUALIS_LITERIS (e->datum,
            "<!doctype html><noscript><!--<noscript></noscript>-->"
            "</noscript>");
    }

    /* NUL intra initum: '<html>' NUL '<frameset></frameset>' - octeti
     * XXVIII, nullus in sede VI; chorda eum fert, literae non */
    ex = _exempla_plagulae(piscina, radix, "/plain-text-unsafe.dat");
    CREDO_NON_NIHIL (ex);
    e = _exemplum(ex, II);
    CREDO_NON_NIHIL (e);
    si (e != NIHIL)
    {
        CREDO_AEQUALIS_I32 (e->linea, XIV);
        CREDO_AEQUALIS_I32 (e->datum.mensura, XXVIII);
        CREDO_VERUM (e->datum.datum[VI] == (i8)ZEPHYRUM);
        CREDO_VERUM (memcmp(e->datum.datum,
            "<html>\0<frameset></frameset>", XXVIII) == ZEPHYRUM);
    }
}

integer
principale (vacuum)
{
                Piscina* piscina;
                    b32  praeteritus;
     constans character* radix;

    piscina = piscina_generare_dynamicum("probatio_html_exempla",
        4194304);
    credo_aperire(piscina);

    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }

    imprimere("\n--- PORTA: lector .dat html5lib ---\n");
    _contractus_lectoris(piscina);
    _manifestum_totum(piscina, radix);
    _casus_limitis(piscina, radix);

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
