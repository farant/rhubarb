/* probatio_scaena.c - prospectus stantes (scaena.h)
 *
 * scaena_struere: plagula in prospectus (textus contracti, res
 * ordine regionum, exitus), convenientia picturae et scaenae
 * utrimque, recusationes cum plagula et linea. Massa: scripta et
 * relecta campo pro campo par; lector massam truncatam, verbum
 * falsum, longitudines falsas et tabulas malas recusat. Casa vera
 * ex apps/scaena/loci componitur.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "graphis.h"
#include "scaena.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define PIXELA (GRAPHIS_LATITUDO * GRAPHIS_ALTITUDO)
#define VIA_CASAE "apps/scaena/loci/casa.scaena.stml"

nomen structura {
    constans character* titulus;
    constans character* textus;
} Plagula;

/* picturae et partes in memoria (nomina distincta) */
hic_manens constans Plagula PLAGULAE[] = {
    { "duo",
      "<pictura>\n"
      "<rectangulum titulus=\"a\" fines=\"0 0 20 20\""
      " colores=\"6\"/>\n"
      "<rectangulum titulus=\"b\" fines=\"40 0 20 20\""
      " colores=\"12\"/>\n"
      "</pictura>" },
    { "una",
      "<pictura>\n"
      "<rectangulum titulus=\"x\" fines=\"0 0 20 20\""
      " colores=\"6\"/>\n"
      "</pictura>" },
    { "nuda",
      "<pictura>\n"
      "<rectangulum fines=\"0 0 20 20\" colores=\"6\"/>\n"
      "</pictura>" },
    { "mala",
      "<pictura>\n\n<circulus/>\n</pictura>" },
    { "cum_parte",
      "<pictura>\n"
      "<grex titulus=\"x\">\n"
      "<usus fons=\"quadrum\"/>\n"
      "</grex>\n"
      "</pictura>" },
    { "cum_parte_mala",
      "<pictura>\n<usus fons=\"fracta\"/>\n</pictura>" },
    { "quadrum",
      "<pars>\n"
      "<rectangulum fines=\"0 0 20 20\" colores=\"6\"/>\n"
      "</pars>" },
    { "fracta",
      "<pars>\n\n\n<rectangulum fines=\"0 0\" colores=\"6\"/>\n"
      "</pars>" },
    { NIHIL, NIHIL }
};

interior chorda
_fons (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    chorda vacua;
       i32 k;

    (vacuum)contextus;
    per (k = ZEPHYRUM; PLAGULAE[k].titulus; k++)
    {
        si (chorda_aequalis_literis(titulus, PLAGULAE[k].titulus))
        {
            redde chorda_ex_literis(PLAGULAE[k].textus, piscina);
        }
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

interior ScaenaFructus
_strue (
        CredoContextus* c,
    constans character* literae)
{
    redde scaena_struere(chorda_ex_literis(literae, c->piscina),
        _fons, _fons, NIHIL, c->piscina);
}

interior ScaenaFructus
_scaenae (
        CredoContextus* c,
    constans character* literae)
{
    ScaenaFructus f = _strue(c, literae);

    si (!f.successus)
    {
        printf("scaena recusata ('%.*s' linea %u): %.*s\n",
            (integer)f.plagula.mensura,
            (constans character*)f.plagula.datum,
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    CREDO_NECESSE (f.prospectus != NIHIL);
    redde f;
}

#define RECUSATUR(literae, plagula_sperata, linea_sperata, fragmentum) \
    fac { \
        ScaenaFructus rf = _strue(c, (literae)); \
        CREDO_FALSUM (rf.successus); \
        CREDO_VERUM (rf.prospectus == NIHIL); \
        si (   rf.linea != (linea_sperata) \
            || !chorda_aequalis_literis(rf.plagula, \
                (plagula_sperata)) \
            || !chorda_continet(rf.error, \
                chorda_ex_literis((fragmentum), c->piscina))) \
        { \
            printf("plagula '%.*s' linea %u, nuntius: %.*s\n", \
                (integer)rf.plagula.mensura, \
                (constans character*)rf.plagula.datum, \
                (insignatus)rf.linea, (integer)rf.error.mensura, \
                (constans character*)rf.error.datum); \
        } \
        CREDO_AEQUALIS_I32 (rf.linea, (linea_sperata)); \
        CREDO_VERUM (chorda_aequalis_literis(rf.plagula, \
            (plagula_sperata))); \
        CREDO_VERUM (chorda_continet(rf.error, \
            chorda_ex_literis((fragmentum), c->piscina))); \
    } dum (FALSUM)

#define SCAENAE_DUAE \
    "<scaenae>\n" \
    "<prospectus titulus=\"primus\" pictura=\"duo\">\n" \
    "<res titulus=\"b\" exitus=\"alter-2\">  To the\n" \
    "   other. </res>\n" \
    "<descriptio>\n  A   low\n  room.\n</descriptio>\n" \
    "<res titulus=\"a\">Thing a.</res>\n" \
    "</prospectus>\n" \
    "<prospectus titulus=\"alter-2\" pictura=\"una\">\n" \
    "<descriptio>Second.</descriptio>\n" \
    "<res titulus=\"x\" exitus=\"mundus\">Out.</res>\n" \
    "</prospectus>\n" \
    "</scaenae>"

interior vacuum
_sectio_structura (
    CredoContextus* c)
{
                ScaenaFructus  f = _scaenae(c, SCAENAE_DUAE);
    constans ScaenaProspectus* p;

    CREDO_NECESSE (f.numerus == II);
    CREDO_AEQUALIS_I32 (f.plagula.mensura, ZEPHYRUM);
    p = &f.prospectus[ZEPHYRUM];
    CREDO_VERUM (chorda_aequalis_literis(p->titulus, "primus"));
    /* textus contractus */
    CREDO_VERUM (chorda_aequalis_literis(p->descriptio,
        "A low room."));
    /* res ordine regionum picturae (a, b), non ordine scripto */
    CREDO_NECESSE (p->rerum == II);
    CREDO_VERUM (chorda_aequalis_literis(p->res[ZEPHYRUM].titulus,
        "a"));
    CREDO_VERUM (chorda_aequalis_literis(p->res[ZEPHYRUM].textus,
        "Thing a."));
    CREDO_AEQUALIS_I32 (p->res[ZEPHYRUM].exitus.mensura, ZEPHYRUM);
    CREDO_VERUM (chorda_aequalis_literis(p->res[I].titulus, "b"));
    CREDO_VERUM (chorda_aequalis_literis(p->res[I].textus,
        "To the other."));
    CREDO_VERUM (chorda_aequalis_literis(p->res[I].exitus,
        "alter-2"));
    /* tabulae picturae: regio a = I, regio b = II */
    CREDO_NECESSE (p->indices != NIHIL && p->domini != NIHIL);
    CREDO_AEQUALIS_I32 (p->domini[5 * GRAPHIS_LATITUDO + 5], I);
    CREDO_AEQUALIS_I32 (p->domini[5 * GRAPHIS_LATITUDO + 45], II);
    CREDO_AEQUALIS_I32 (p->indices[5 * GRAPHIS_LATITUDO + 5], VI);
    CREDO_AEQUALIS_I32 (p->indices[5 * GRAPHIS_LATITUDO + 45], XII);
    p = &f.prospectus[I];
    CREDO_VERUM (chorda_aequalis_literis(p->titulus, "alter-2"));
    CREDO_NECESSE (p->rerum == I);
    CREDO_VERUM (chorda_aequalis_literis(p->res[ZEPHYRUM].exitus,
        SCAENA_EXITUS_MUNDUS));
    /* pictura sine regionibus et sine rebus valet */
    f = _scaenae(c,
        "<scaenae><prospectus titulus=\"solus\" pictura=\"nuda\">"
        "<descriptio>Empty.</descriptio></prospectus></scaenae>");
    CREDO_AEQUALIS_I32 (f.prospectus[ZEPHYRUM].rerum, ZEPHYRUM);
    /* pictura cum parte: fons partium adhibetur */
    f = _scaenae(c,
        "<scaenae><prospectus titulus=\"solus\""
        " pictura=\"cum_parte\">"
        "<descriptio>Piece.</descriptio>"
        "<res titulus=\"x\">X.</res></prospectus></scaenae>");
    CREDO_AEQUALIS_I32 (f.prospectus[ZEPHYRUM].domini[
        5 * GRAPHIS_LATITUDO + 5], I);
}

interior b32
_chordae_pares (
    chorda a,
    chorda b)
{
    redde a.mensura == b.mensura
        && (a.mensura == ZEPHYRUM
            || memcmp(a.datum, b.datum, a.mensura) == ZEPHYRUM);
}

interior vacuum
_sectio_massa (
    CredoContextus* c)
{
       ScaenaFructus f = _scaenae(c, SCAENAE_DUAE);
    ScaenaProspectus relectus;
              chorda massa;
              chorda altera;
                 i32 k;
                 i32 j;

    per (k = ZEPHYRUM; k < f.numerus; k++)
    {
        constans ScaenaProspectus* p = &f.prospectus[k];
                              i32  sperata;

        massa = scaena_scribere(p, c->piscina);
        CREDO_NECESSE (massa.datum != NIHIL);
        CREDO_VERUM (memcmp(massa.datum, "SCAENA01", VIII)
            == ZEPHYRUM);
        sperata = VIII + IV + p->titulus.mensura + IV
            + p->descriptio.mensura + IV + II * PIXELA;
        per (j = ZEPHYRUM; j < p->rerum; j++)
        {
            sperata += XII + p->res[j].titulus.mensura
                + p->res[j].textus.mensura + p->res[j].exitus.mensura;
        }
        CREDO_AEQUALIS_I32 (massa.mensura, sperata);
        /* prospectus idem, octeti idem */
        altera = scaena_scribere(p, c->piscina);
        CREDO_VERUM (_chordae_pares(massa, altera));
        /* relectus campo pro campo par */
        CREDO_NECESSE (scaena_legere(massa, &relectus, c->piscina));
        CREDO_VERUM (_chordae_pares(relectus.titulus, p->titulus));
        CREDO_VERUM (_chordae_pares(relectus.descriptio,
            p->descriptio));
        CREDO_NECESSE (relectus.rerum == p->rerum);
        per (j = ZEPHYRUM; j < p->rerum; j++)
        {
            CREDO_VERUM (_chordae_pares(relectus.res[j].titulus,
                p->res[j].titulus));
            CREDO_VERUM (_chordae_pares(relectus.res[j].textus,
                p->res[j].textus));
            CREDO_VERUM (_chordae_pares(relectus.res[j].exitus,
                p->res[j].exitus));
        }
        CREDO_VERUM (memcmp(relectus.indices, p->indices, PIXELA)
            == ZEPHYRUM);
        CREDO_VERUM (memcmp(relectus.domini, p->domini, PIXELA)
            == ZEPHYRUM);
        /* relectus nihil in massa tenet */
        CREDO_VERUM (relectus.indices < massa.datum
            || relectus.indices >= massa.datum + massa.mensura);
        CREDO_VERUM (relectus.titulus.datum < massa.datum
            || relectus.titulus.datum >= massa.datum + massa.mensura);
    }
    CREDO_VERUM (scaena_scribere(NIHIL, c->piscina).datum == NIHIL);
    CREDO_VERUM (scaena_scribere(&f.prospectus[ZEPHYRUM], NIHIL).datum
        == NIHIL);
}

/* exemplar massae mutabile */
interior chorda
_exemplar (
    CredoContextus* c,
            chorda  massa,
               i32  mensura)
{
    chorda e;

    e.mensura  = mensura;
    e.datum    = (i8*)piscina_allocare(c->piscina,
        (memoriae_index)(mensura > massa.mensura ? mensura
            : massa.mensura) + I);
    memset(e.datum, ZEPHYRUM,
        (mensura > massa.mensura ? mensura : massa.mensura) + I);
    memcpy(e.datum, massa.datum,
        mensura < massa.mensura ? mensura : massa.mensura);
    redde e;
}

interior vacuum
_sectio_lector (
    CredoContextus* c)
{
       ScaenaFructus f = _scaenae(c, SCAENAE_DUAE);
    ScaenaProspectus p;
              chorda massa = scaena_scribere(&f.prospectus[ZEPHYRUM],
                  c->piscina);
              chorda e;
                 i32 k;
                 i32 acceptae = ZEPHYRUM;
                 i32 campi;

    CREDO_NECESSE (massa.datum != NIHIL);
    campi = massa.mensura - II * PIXELA;
    /* massa truncata: omnis longitudo in campis, et exempla in
     * tabulis - numquam accepta, numquam ruina */
    per (k = ZEPHYRUM; k < campi + XVI; k++)
    {
        e = _exemplar(c, massa, k);
        si (scaena_legere(e, &p, c->piscina))
        {
            acceptae++;
        }
    }
    CREDO_AEQUALIS_I32 (acceptae, ZEPHYRUM);
    e = _exemplar(c, massa, massa.mensura - I);
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    e = _exemplar(c, massa, campi + PIXELA);
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    /* octetus superfluus */
    e = _exemplar(c, massa, massa.mensura + I);
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    /* verbum primum aliud */
    e             = _exemplar(c, massa, massa.mensura);
    e.datum[VII]  = '2';
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    /* longitudo tituli ingens (octeti VIII .. XI) */
    e              = _exemplar(c, massa, massa.mensura);
    e.datum[VIII]  = 0x7f;
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    e              = _exemplar(c, massa, massa.mensura);
    e.datum[VIII]  = 0xff;
    e.datum[IX]    = 0xff;
    e.datum[X]     = 0xff;
    e.datum[XI]    = 0xff;
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    /* titulus longior quam LXIV, etsi massa sufficit */
    e            = _exemplar(c, massa, massa.mensura);
    e.datum[XI]  = 65;
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    /* titulus vacuus */
    e            = _exemplar(c, massa, massa.mensura);
    e.datum[XI]  = ZEPHYRUM;
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    /* dominus numerum rerum excedit */
    e                           = _exemplar(c, massa, massa.mensura);
    e.datum[massa.mensura - I]  = III;
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    /* index palettae extra XVI */
    e               = _exemplar(c, massa, massa.mensura);
    e.datum[campi]  = XVI;
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    /* campus longior quam maximum suum, massa ceterum recta:
     * prospectus manu factus, scriptus, relectus - recusatur */
    {
        ScaenaProspectus nimius = f.prospectus[ZEPHYRUM];
               ScaenaRes res[II];
               character litterae[SCAENA_DESCRIPTIO_MAXIMA + II];

        memset(litterae, 'a', magnitudo(litterae));
        res[ZEPHYRUM]  = f.prospectus[ZEPHYRUM].res[ZEPHYRUM];
        res[I]         = f.prospectus[ZEPHYRUM].res[I];
        nimius.res     = res;
        /* integer accipitur */
        CREDO_VERUM (scaena_legere(scaena_scribere(&nimius,
            c->piscina), &p, c->piscina));
        nimius.titulus = chorda_ex_buffer((i8*)litterae,
            SCAENA_TITULUS_MAXIMUS + I);
        CREDO_FALSUM (scaena_legere(scaena_scribere(&nimius,
            c->piscina), &p, c->piscina));
        nimius.titulus = chorda_ex_buffer((i8*)litterae,
            SCAENA_TITULUS_MAXIMUS);
        CREDO_VERUM (scaena_legere(scaena_scribere(&nimius,
            c->piscina), &p, c->piscina));
        nimius.descriptio = chorda_ex_buffer((i8*)litterae,
            SCAENA_DESCRIPTIO_MAXIMA + I);
        CREDO_FALSUM (scaena_legere(scaena_scribere(&nimius,
            c->piscina), &p, c->piscina));
        nimius.descriptio = chorda_ex_buffer((i8*)litterae,
            SCAENA_DESCRIPTIO_MAXIMA);
        CREDO_VERUM (scaena_legere(scaena_scribere(&nimius,
            c->piscina), &p, c->piscina));
        res[I].textus = chorda_ex_buffer((i8*)litterae,
            SCAENA_TEXTUS_MAXIMUS + I);
        CREDO_FALSUM (scaena_legere(scaena_scribere(&nimius,
            c->piscina), &p, c->piscina));
        res[I].textus = chorda_ex_buffer((i8*)litterae,
            SCAENA_TEXTUS_MAXIMUS);
        res[I].exitus = chorda_ex_buffer((i8*)litterae,
            SCAENA_TITULUS_MAXIMUS + I);
        CREDO_FALSUM (scaena_legere(scaena_scribere(&nimius,
            c->piscina), &p, c->piscina));
        res[I].exitus = chorda_ex_buffer((i8*)litterae,
            SCAENA_TITULUS_MAXIMUS);
        CREDO_VERUM (scaena_legere(scaena_scribere(&nimius,
            c->piscina), &p, c->piscina));
    }
    /* argumenta nulla */
    CREDO_FALSUM (scaena_legere(massa, NIHIL, c->piscina));
    CREDO_FALSUM (scaena_legere(massa, &p, NIHIL));
    e.datum    = NIHIL;
    e.mensura  = ZEPHYRUM;
    CREDO_FALSUM (scaena_legere(e, &p, c->piscina));
    /* ... et massa integra accipitur */
    CREDO_VERUM (scaena_legere(massa, &p, c->piscina));
}

#define CAPUT "<scaenae>\n<prospectus titulus=\"p\" pictura=\"una\">\n"
#define SERMO "<descriptio>D.</descriptio>\n"
#define CAUDA "</prospectus>\n</scaenae>"

interior vacuum
_sectio_recusationes (
    CredoContextus* c)
{
    character longa[IV * M];
          i32 k;
          i32 n;

    RECUSATUR ("<scaenae", "", I, "not valid STML");
    RECUSATUR ("<pictura/>", "", I, "<scaenae>");
    RECUSATUR ("<scaenae>\n</scaenae>", "", I, "no <prospectus>");
    RECUSATUR ("<scaenae lux=\"1\">\n</scaenae>", "", I,
        "unknown attribute 'lux'");
    RECUSATUR ("<scaenae>\n<locus/>\n</scaenae>", "", II,
        "unknown element");
    RECUSATUR ("<scaenae>\n<prospectus pictura=\"una\">\n" SERMO
        "<res titulus=\"x\">X.</res>\n" CAUDA, "", II, "titulus");
    RECUSATUR ("<scaenae>\n<prospectus titulus=\"Hut\""
        " pictura=\"una\">\n" SERMO "<res titulus=\"x\">X.</res>\n"
        CAUDA, "", II, "titulus");
    RECUSATUR ("<scaenae>\n<prospectus titulus=\"mundus\""
        " pictura=\"una\">\n" SERMO "<res titulus=\"x\">X.</res>\n"
        CAUDA, "", II, "mundus");
    RECUSATUR ("<scaenae>\n<prospectus titulus=\"p\">\n" SERMO CAUDA,
        "", II, "pictura");
    RECUSATUR ("<scaenae>\n<prospectus titulus=\"p\""
        " pictura=\"a-b\">\n" SERMO CAUDA, "", II,
            "pictura");
    RECUSATUR ("<scaenae>\n<prospectus titulus=\"p\" pictura=\"una\""
        " lux=\"1\">\n" SERMO CAUDA, "", II,
        "unknown attribute 'lux'");
    RECUSATUR (CAPUT SERMO "<res titulus=\"x\">X.</res>\n"
        "</prospectus>\n<prospectus titulus=\"p\" pictura=\"una\">\n"
        SERMO "<res titulus=\"x\">X.</res>\n" CAUDA, "", VI,
        "'p' is written twice");
    /* descriptio */
    RECUSATUR (CAPUT "<res titulus=\"x\">X.</res>\n" CAUDA, "", II,
        "no <descriptio>");
    RECUSATUR (CAPUT SERMO SERMO "<res titulus=\"x\">X.</res>\n"
        CAUDA, "", IV, "one <descriptio>");
    RECUSATUR (CAPUT "<descriptio>  \n </descriptio>\n"
        "<res titulus=\"x\">X.</res>\n" CAUDA, "", III, "empty");
    RECUSATUR (CAPUT "<descriptio titulus=\"a\">D.</descriptio>\n"
        "<res titulus=\"x\">X.</res>\n" CAUDA, "", III,
        "unknown attribute 'titulus'");
    RECUSATUR (CAPUT "<descriptio>D.<res titulus=\"x\">X.</res>"
        "</descriptio>\n" CAUDA, "", III, "text only");
    /* res */
    RECUSATUR (CAPUT SERMO "<res>X.</res>\n" CAUDA, "", IV,
        "titulus");
    RECUSATUR (CAPUT SERMO "<res titulus=\"x\"></res>\n" CAUDA,
        "",
        IV, "'x' has no text");
    RECUSATUR (CAPUT SERMO "<res titulus=\"x\">X.</res>\n"
        "<res titulus=\"x\">Y.</res>\n" CAUDA, "", V,
        "'x' is written twice");
    RECUSATUR (CAPUT SERMO "<res titulus=\"x\" color=\"6\">X.</res>\n"
        CAUDA, "", IV, "unknown attribute 'color'");
    RECUSATUR (CAPUT SERMO "<res titulus=\"x\">X.</res>\n"
        "<exitus/>\n" CAUDA, "", V, "unknown element");
    /* convenientia utrimque */
    RECUSATUR (CAPUT SERMO CAUDA, "", II, "region 'x'");
    RECUSATUR (CAPUT SERMO "<res titulus=\"x\">X.</res>\n"
        "<res titulus=\"y\">Y.</res>\n" CAUDA, "", V,
        "'y' is not a region");
    /* exitus */
    RECUSATUR (CAPUT SERMO "<res titulus=\"x\" exitus=\"nusquam\">"
        "X.</res>\n" CAUDA, "", IV, "'nusquam'");
    RECUSATUR (CAPUT SERMO "<res titulus=\"x\" exitus=\"p\">"
        "X.</res>\n" CAUDA, "", IV, "its own view");
    /* pictura */
    RECUSATUR ("<scaenae>\n\n<prospectus titulus=\"p\""
        " pictura=\"nulla\">\n" SERMO CAUDA, "", III,
        "'nulla' was not found");
    RECUSATUR ("<scaenae>\n<prospectus titulus=\"p\""
        " pictura=\"mala\">\n" SERMO CAUDA, "mala", III,
        "unknown element");
    RECUSATUR ("<scaenae>\n<prospectus titulus=\"p\""
        " pictura=\"cum_parte_mala\">\n" SERMO CAUDA,
            "fracta", IV,
        "fines");
    /* textus nimis longi */
    n = (i32)sprintf(longa, CAPUT "<descriptio>");
    per (k = ZEPHYRUM; k < SCAENA_DESCRIPTIO_MAXIMA + I; k++)
    {
        longa[n++] = 'a';
    }
    (vacuum)sprintf(longa + n, "</descriptio>\n"
        "<res titulus=\"x\">X.</res>\n" CAUDA);
    RECUSATUR (longa, "", III, "2048");
    n = (i32)sprintf(longa,
        CAPUT SERMO "<res titulus=\"x\">");
    per (k = ZEPHYRUM; k < SCAENA_TEXTUS_MAXIMUS + I; k++)
    {
        longa[n++] = 'a';
    }
    (vacuum)sprintf(longa + n, "</res>\n" CAUDA);
    RECUSATUR (longa, "", IV, "512");
    /* longitudo maxima ipsa accipitur */
    n = (i32)sprintf(longa,
        CAPUT SERMO "<res titulus=\"x\">");
    per (k = ZEPHYRUM; k < SCAENA_TEXTUS_MAXIMUS; k++)
    {
        longa[n++] = 'a';
    }
    (vacuum)sprintf(longa + n, "</res>\n" CAUDA);
    CREDO_VERUM (_strue(c, longa).successus);
    /* fons picturarum nullus */
    CREDO_FALSUM (scaena_struere(chorda_ex_literis(
        CAPUT SERMO "<res titulus=\"x\">X.</res>\n" CAUDA,
        c->piscina), NIHIL, NIHIL, NIHIL, c->piscina).successus);
}

/* fontes veri: picturae ex apps/scaena/loci, partes ex graphis/res */
interior chorda
_fons_plagularum (
    constans character* directorium,
                chorda  titulus,
               Piscina* piscina)
{
    character via[CXXVIII];
       chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    (vacuum)sprintf(via, "%s/%.*s.stml", directorium,
        (integer)titulus.mensura,
        (constans character*)titulus.datum);
    si (!filum_existit(via))
    {
        redde vacua;
    }
    redde filum_legere_totum(via, piscina);
}

interior chorda
_fons_picturarum (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    (vacuum)contextus;
    redde _fons_plagularum("apps/scaena/loci", titulus, piscina);
}

interior chorda
_fons_partium (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    (vacuum)contextus;
    redde _fons_plagularum("graphis/res", titulus, piscina);
}

interior vacuum
_sectio_casa (
    CredoContextus* c)
{
               ScaenaFructus f = scaena_struere(
                   filum_legere_totum(VIA_CASAE,
                   c->piscina), _fons_picturarum, _fons_partium, NIHIL,
                   c->piscina);
    constans ScaenaProspectus* p;
                       chorda  massa;
             ScaenaProspectus  relectus;

    si (!f.successus)
    {
        printf("casa recusata ('%.*s' linea %u): %.*s\n",
            (integer)f.plagula.mensura,
            (constans character*)f.plagula.datum,
            (insignatus)f.linea, (integer)f.error.mensura,
            (constans character*)f.error.datum);
    }
    CREDO_NECESSE (f.successus);
    CREDO_NECESSE (f.numerus == II);
    p = &f.prospectus[ZEPHYRUM];
    CREDO_VERUM (chorda_aequalis_literis(p->titulus, "hut-hearth"));
    CREDO_NECESSE (p->rerum == V);
    CREDO_VERUM (chorda_aequalis_literis(p->res[ZEPHYRUM].titulus,
        "hearth"));
    CREDO_VERUM (chorda_aequalis_literis(p->res[IV].titulus, "turn"));
    CREDO_VERUM (chorda_aequalis_literis(p->res[IV].exitus,
        "hut-door"));
    CREDO_VERUM (p->descriptio.mensura > 100);
    /* descriptio in lineas fracta sine spatiis duplicibus exit */
    CREDO_FALSUM (chorda_continet(p->descriptio,
        chorda_ex_literis("  ", c->piscina)));
    CREDO_FALSUM (chorda_continet(p->descriptio,
        chorda_ex_literis("\n", c->piscina)));
    p = &f.prospectus[I];
    CREDO_VERUM (chorda_aequalis_literis(p->titulus, "hut-door"));
    CREDO_NECESSE (p->rerum == IV);
    CREDO_VERUM (chorda_aequalis_literis(p->res[ZEPHYRUM].titulus,
        "door"));
    CREDO_VERUM (chorda_aequalis_literis(p->res[ZEPHYRUM].exitus,
        SCAENA_EXITUS_MUNDUS));
    CREDO_VERUM (chorda_aequalis_literis(p->res[III].exitus,
        "hut-hearth"));
    massa = scaena_scribere(p, c->piscina);
    CREDO_NECESSE (massa.datum != NIHIL);
    CREDO_VERUM (scaena_legere(massa, &relectus, c->piscina));
    CREDO_AEQUALIS_I32 (relectus.rerum, IV);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "structura", _sectio_structura, NIHIL, NIHIL, NIHIL, NIHIL },
    { "massa", _sectio_massa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lector", _sectio_lector, NIHIL, NIHIL, NIHIL, NIHIL },
    { "recusationes", _sectio_recusationes, NIHIL, NIHIL, NIHIL,
      NIHIL },
    { "casa", _sectio_casa, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("scaena", SECTIONES);
}
