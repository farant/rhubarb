/* norma_ad_c.c - normae nominatae -> fons C (norma-spec-2 §III.3;
 * norma-plan-3 A5, regula R7). Nodi in tabula 'nodi[]' post-ordine;
 * referentia <ad> = vocatio _struere_<titulus> memorata in 'facta'. */
#include "norma_ad_c.h"
#include "chorda_aedificator.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
                   Piscina* p;
    constans NormaNominata* normae;
                       i32  numerus;
                       i32  radix;
        constans character* praefixum;
         ChordaAedificator* sententiae;  /* functio currens */
                       Xar* gignentes;   /* chorda, unici */
                       i32  nodi;
                    chorda* causa;
                       b32  fractum;
} Emissor;

interior vacuum
_recusare (
                Emissor* em,
     constans character* nuntius)
{
    si (!em->fractum)
    {
        em->fractum = VERUM;
        si (em->causa)
        {
            *em->causa = chorda_ex_literis(nuntius, em->p);
        }
    }
}

interior b32
_identificator (
    chorda c)
{
    i32 i;

    si (c.mensura == 0 || (c.datum[0] >= '0' && c.datum[0] <= '9'))
    {
        redde FALSUM;
    }
    per (i = 0; i < c.mensura; i++)
    {
        character k = (character)c.datum[i];

        si (!(   (k >= 'a' && k <= 'z') || (k >= 'A' && k <= 'Z')
              || (k >= '0' && k <= '9') || k == '_'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* litterae C: effugia " \ ? (trigraphi) et moderatores; > DIX recusat */
interior vacuum
_literae (
               Emissor* em,
     ChordaAedificator* a,
                chorda  c)
{
          i32 i;
    character octalis[VIII];

    si (c.mensura > DIX)
    {
        _recusare(em, "textus longior quam DIX octeti (C89)");
        redde;
    }
    chorda_aedificator_appendere_character(a, '"');
    per (i = 0; i < c.mensura; i++)
    {
        insignatus character k = (insignatus character)c.datum[i];

        si (k == '"' || k == '\\' || k == '?')
        {
            chorda_aedificator_appendere_character(a, '\\');
            chorda_aedificator_appendere_character(a, (character)k);
        }
        alioquin si (k == '\n')
        {
            chorda_aedificator_appendere_literis(a, "\\n");
        }
        alioquin si (k < 0x20 || k == 0x7F)
        {
            sprintf(octalis, "\\%03o", (insignatus integer)k);
            chorda_aedificator_appendere_literis(a, octalis);
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, (character)k);
        }
    }
    chorda_aedificator_appendere_character(a, '"');
}

interior vacuum
_sententia (
               Emissor* em,
    constans character* textus)
{
    chorda_aedificator_appendere_literis(em->sententiae, textus);
}

interior vacuum
_nodus_nominatus (
     Emissor* em,
         i32  i)
{
    character b[XXXII];

    sprintf(b, "nodi[%u]", (insignatus integer)i);
    _sententia(em, b);
}

/* "    norma_X(nodi[i]" - vocatio modificatoris aperta */
interior vacuum
_modificator (
                Emissor* em,
     constans character* functio,
                    i32  i)
{
    _sententia(em, "    ");
    _sententia(em, functio);
    _sententia(em, "(");
    _nodus_nominatus(em, i);
}

interior vacuum
_s64 (
     Emissor* em,
         s64  v)
{
    character b[XLVIII];

    si (v == (s64)LONG_MIN)
    {
        _sententia(em, "(s64)(-9223372036854775807L - 1L)");
        redde;
    }
    sprintf(b, "(s64)%ldL", (longus)v);
    _sententia(em, b);
}

interior vacuum
_f64 (
     Emissor* em,
         f64  v)
{
    character b[XLVIII];
          i32 praecisio;

    si (v != v || v - v != 0.0)
    {
        _recusare(em, "numerus fluitans non finitus");
        redde;
    }
    per (praecisio = I; praecisio <= XVII; praecisio++)
    {
        sprintf(b, "%.*g", (integer)praecisio, v);
        si (strtod(b, NIHIL) == v)
        {
            frange;
        }
    }
    si (!strchr(b, '.') && !strchr(b, 'e'))
    {
        strcat(b, ".0");
    }
    _sententia(em, b);
}

interior i32
_index_normae (
           Emissor* em,
    constans Norma* n)
{
    i32 i;

    per (i = 0; i < em->numerus; i++)
    {
        si (em->normae[i].norma == n)
        {
            redde i;
        }
    }
    redde em->numerus;
}

interior constans character*
_constructor (
    NormaGenus g)
{
    commutatio (g)
    {
        casus NORMA_NULLUM:    redde "norma_nullum";
        casus NORMA_BOOLEAN:   redde "norma_boolean";
        casus NORMA_INTEGER:   redde "norma_integer";
        casus NORMA_NUMERUS:   redde "norma_numerus";
        casus NORMA_TEXTUS:    redde "norma_textus";
        casus NORMA_OBJECTUM:  redde "norma_objectum";
        ordinarius:            redde "norma_liberum";
    }
}

interior vacuum
_gignens_notare (
    Emissor* em,
     chorda  titulus)
{
    i32 i;

    per (i = 0; i < xar_numerus(em->gignentes); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(em->gignentes, i),
                titulus))
        {
            redde;
        }
    }
    *(chorda*)xar_addere(em->gignentes) = titulus;
}

/* sententias nodi emittit (post-ordine); reddit indicem in nodi[] */
interior i32
_nodus (
            Emissor* em,
     constans Norma* n,
                i32  profunditas)
{
    NormaVisus v;
           i32 k;
           i32 me;
           i32 i;
           i32 f;

    si (em->fractum)
    {
        redde 0;
    }
    si (profunditas > CXXVIII)
    {
        _recusare(em, "profunditas > CXXVIII: circulus aedificatorum?");
        redde 0;
    }
    k = _index_normae(em, n);
    si (k < em->numerus && profunditas > 0)
    {
        si (k == em->radix)
        {
            _recusare(em, "circulus: norma se ipsam continet");
            redde 0;
        }
        me = em->nodi++;
        _sententia(em, "    ");
        _nodus_nominatus(em, me);
        _sententia(em, " = _struere_");
        chorda_aedificator_appendere_chorda(em->sententiae,
            em->normae[k].titulus);
        _sententia(em, "(piscina, facta);\n");
        redde me;
    }
    v = norma_visus(n);
    si (!n || v.error_schematis.mensura > 0)
    {
        _recusare(em, "nodus pravus");
        redde 0;
    }
    si (v.genus == NORMA_TABULATUM)
    {
        f   = _nodus(em, v.elementum, profunditas + I);
        me  = em->nodi++;
        _sententia(em, "    ");
        _nodus_nominatus(em, me);
        _sententia(em, " = norma_tabulatum(piscina, ");
        _nodus_nominatus(em, f);
        _sententia(em, ");\n");
    }
    alioquin si (v.genus == NORMA_DISCRIMEN)
    {
        me = em->nodi++;
        _sententia(em, "    ");
        _nodus_nominatus(em, me);
        _sententia(em, " = norma_discrimen(piscina, ");
        _literae(em, em->sententiae, v.clavis_discriminis);
        _sententia(em, ");\n");
    }
    alioquin
    {
        me = em->nodi++;
        _sententia(em, "    ");
        _nodus_nominatus(em, me);
        _sententia(em, " = ");
        _sententia(em, _constructor(v.genus));
        _sententia(em, "(piscina);\n");
    }
    per (i = 0; v.genus == NORMA_OBJECTUM
        && i < xar_numerus(v.campi); i++)
    {
        NormaCampus* c = (NormaCampus*)xar_obtinere(v.campi, i);

        f = _nodus(em, c->valor, profunditas + I);
        _modificator(em, "norma_campus", me);
        _sententia(em, ", ");
        _literae(em, em->sententiae, c->titulus);
        _sententia(em, ", ");
        _nodus_nominatus(em, f);
        _sententia(em, c->requiritur ? ", VERUM);\n" : ", FALSUM);\n");
    }
    per (i = 0; v.genus == NORMA_DISCRIMEN
                && i < xar_numerus(v.variationes); i++)
    {
        NormaVariatio* va = (NormaVariatio*)xar_obtinere(v.variationes,
            i);

        f = _nodus(em, va->objectum, profunditas + I);
        _modificator(em, "norma_variatio", me);
        _sententia(em, ", ");
        _literae(em, em->sententiae, va->valor);
        _sententia(em, ", ");
        _nodus_nominatus(em, f);
        _sententia(em, ");\n");
    }
    si (   (v.genus == NORMA_OBJECTUM || v.genus == NORMA_DISCRIMEN)
        && v.modus != NORMA_CLAUSUM)
    {
        _modificator(em, "norma_modus", me);
        _sententia(em, v.modus == NORMA_APERTUM ? ", NORMA_APERTUM);\n"
                                                : ", NORMA_NOTANDUM);\n");
    }
    si (v.habet_intra)
    {
        _modificator(em, "norma_intra", me);
        _sententia(em, ", ");
        _s64(em, v.minimum);
        _sententia(em, ", ");
        _s64(em, v.maximum);
        _sententia(em, ");\n");
    }
    si (v.habet_intra_fluitans)
    {
        _modificator(em, "norma_intra_fluitans", me);
        _sententia(em, ", ");
        _f64(em, v.minimum_fluitans);
        _sententia(em, ", ");
        _f64(em, v.maximum_fluitans);
        _sententia(em, ");\n");
    }
    si (v.habet_longitudinem)
    {
        character b[XLVIII];

        _modificator(em, "norma_longitudo", me);
        sprintf(b, ", (i32)%luUL, (i32)%luUL);\n",
            (insignatus longus)v.longitudo_minima,
            (insignatus longus)v.longitudo_maxima);
        _sententia(em, b);
    }
    si (v.forma.mensura > 0)
    {
        _modificator(em, "norma_forma", me);
        _sententia(em, ", ");
        _literae(em, em->sententiae, v.forma);
        _sententia(em, ");\n");
    }
    si (v.licita && xar_numerus(v.licita) > 0)
    {
        _sententia(em, "    {\n        constans character* constans"
                       " licita[] = { ");
        per (i = 0; i < xar_numerus(v.licita); i++)
        {
            _literae(em, em->sententiae,
                *(chorda*)xar_obtinere(v.licita,
                i));
            _sententia(em, ", ");
        }
        _sententia(em, "NIHIL };\n\n    ");
        _modificator(em, "norma_electio", me);
        _sententia(em, ", licita);\n    }\n");
    }
    si (v.descriptio.mensura > 0)
    {
        _modificator(em, "norma_descriptio", me);
        _sententia(em, ", ");
        _literae(em, em->sententiae, v.descriptio);
        _sententia(em, ");\n");
    }
    si (v.gignens_titulus.mensura > 0)
    {
        si (!_identificator(v.gignens_titulus))
        {
            _recusare(em, "gignens non identificator C");
            redde 0;
        }
        _gignens_notare(em, v.gignens_titulus);
        _modificator(em, "norma_gignens", me);
        _sententia(em, ", ");
        _sententia(em, em->praefixum);
        _sententia(em, "gignens_");
        chorda_aedificator_appendere_chorda(em->sententiae,
            v.gignens_titulus);
        _sententia(em, ", NIHIL);\n");
        _modificator(em, "norma_gignens_titulus", me);
        _sententia(em, ", ");
        _literae(em, em->sententiae, v.gignens_titulus);
        _sententia(em, ");\n");
    }
    alioquin si (v.gignens)
    {
        _recusare(em, "gignens cum functione sine titulo");
        redde 0;
    }
    si (v.aut_nullum)
    {
        _modificator(em, "norma_aut_nullum", me);
        _sententia(em, ");\n");
    }
    redde me;
}

interior chorda
_custos (
    constans character* caput_titulus,
               Piscina* p)
{
    chorda c = chorda_ex_literis(caput_titulus, p);
       i32 i;

    per (i = 0; i < c.mensura; i++)
    {
        character k = (character)c.datum[i];

        si (k >= 'a' && k <= 'z')
        {
            c.datum[i] = (i8)(k - 'a' + 'A');
        }
        alioquin si (!((k >= 'A' && k <= 'Z')
                     || (k >= '0' && k <= '9')))
        {
            c.datum[i] = '_';
        }
    }
    redde c;
}

NormaFonsC
norma_ad_c (
    constans NormaNominata* normae,
                       i32  numerus,
        constans character* praefixum,
        constans character* caput_titulus,
        constans character* fons_titulus,
                   Piscina* piscina,
                    chorda* causa)
{
              Emissor  em;
           NormaFonsC  fons;
    ChordaAedificator* caput = chorda_aedificator_creare(piscina, M);
    ChordaAedificator* corpus = chorda_aedificator_creare(piscina,
        M * IV);
    ChordaAedificator* functiones = chorda_aedificator_creare(piscina,
                                        M * IV);
                  i32 i;
            character b[CCLVI];
               chorda custos  = _custos(caput_titulus, piscina);

    memset(&em, 0, magnitudo(em));
    memset(&fons, 0, magnitudo(fons));
    em.p          = piscina;
    em.normae     = normae;
    em.numerus    = numerus;
    em.praefixum  = praefixum;
    em.causa      = causa;
    em.gignentes  = xar_creare(piscina, (i32)magnitudo(chorda));
    si (causa)
    {
        causa->datum    = NIHIL;
        causa->mensura  = 0;
    }
    si (   !_identificator(chorda_ex_literis(praefixum, piscina))
        || strstr(fons_titulus, "*/"))
    {
        _recusare(&em, "praefixum non identificator aut fons cum '*/'");
        redde fons;
    }
    /* functiones _struere_ + titulus */
    per (i = 0; i < numerus && !em.fractum; i++)
    {
        ChordaAedificator* sententiae =
            chorda_aedificator_creare(piscina,
                                            M);
                      i32 radix_nodi;

        si (!_identificator(normae[i].titulus))
        {
            _recusare(&em, "titulus normae non identificator C");
            frange;
        }
        em.sententiae  = sententiae;
        em.radix       = i;
        em.nodi        = 0;
        radix_nodi     = _nodus(&em, normae[i].norma, 0);
        chorda_aedificator_appendere_literis(functiones,
            "\ninterior Norma*\n_struere_");
        chorda_aedificator_appendere_chorda(functiones,
            normae[i].titulus);
        sprintf(b, " (\n    Piscina* piscina,\n     Norma** facta)\n{\n"
            "    Norma* nodi[%u];\n\n    si (facta[%u])\n    {\n"
            "        redde facta[%u];\n    }\n",
            (insignatus integer)em.nodi, (insignatus integer)i,
            (insignatus integer)i);
        chorda_aedificator_appendere_literis(functiones, b);
        chorda_aedificator_appendere_chorda(functiones,
            chorda_aedificator_finire(sententiae));
        sprintf(b,
            "    facta[%u] = nodi[%u];\n    redde nodi[%u];\n}\n",
            (insignatus integer)i, (insignatus integer)radix_nodi,
            (insignatus integer)radix_nodi);
        chorda_aedificator_appendere_literis(functiones, b);
    }
    si (em.fractum)
    {
        redde fons;
    }
    /* caput */
    sprintf(b,
        "/* GENERATUM a bin/norma c ex %.80s - noli manu mutare */\n",
        fons_titulus);
    chorda_aedificator_appendere_literis(caput, b);
    chorda_aedificator_appendere_literis(caput, "#ifndef ");
    chorda_aedificator_appendere_chorda(caput, custos);
    chorda_aedificator_appendere_literis(caput, "\n#define ");
    chorda_aedificator_appendere_chorda(caput, custos);
    chorda_aedificator_appendere_literis(caput,
        "\n\n#include \"latina.h\"\n"
        "#include \"piscina.h\"\n#include \"norma.h\"\n\n");
    per (i = 0; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(caput, "Norma*\n");
        chorda_aedificator_appendere_literis(caput, praefixum);
        chorda_aedificator_appendere_chorda(caput, normae[i].titulus);
        chorda_aedificator_appendere_literis(caput,
            " (\n    Piscina* piscina);\n\n");
    }
    per (i = 0; i < xar_numerus(em.gignentes); i++)
    {
        chorda_aedificator_appendere_literis(caput,
            "/* vocans definit (norma-spec-2 §III.3) */\nJsonValor*\n");
        chorda_aedificator_appendere_literis(caput, praefixum);
        chorda_aedificator_appendere_literis(caput, "gignens_");
        chorda_aedificator_appendere_chorda(caput,
            *(chorda*)xar_obtinere(em.gignentes, i));
        chorda_aedificator_appendere_literis(caput,
            " (\n       Sors* sors,\n    Piscina* piscina,\n"
            "     vacuum* datum);\n\n");
    }
    chorda_aedificator_appendere_literis(caput, "#endif\n");
    /* corpus */
    sprintf(b,
        "/* GENERATUM a bin/norma c ex %.80s - noli manu mutare */\n",
        fons_titulus);
    chorda_aedificator_appendere_literis(corpus, b);
    chorda_aedificator_appendere_literis(corpus, "#include \"");
    chorda_aedificator_appendere_literis(corpus, caput_titulus);
    chorda_aedificator_appendere_literis(corpus, "\"\n\n");
    per (i = 0; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(corpus,
            "interior Norma* _struere_");
        chorda_aedificator_appendere_chorda(corpus, normae[i].titulus);
        chorda_aedificator_appendere_literis(corpus,
            " (Piscina* piscina, Norma** facta);\n");
    }
    chorda_aedificator_appendere_chorda(corpus,
        chorda_aedificator_finire(functiones));
    per (i = 0; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(corpus, "\nNorma*\n");
        chorda_aedificator_appendere_literis(corpus, praefixum);
        chorda_aedificator_appendere_chorda(corpus, normae[i].titulus);
        sprintf(b,
            " (\n    Piscina* piscina)\n{\n    Norma* facta[%u];\n"
            "      i32  i;\n\n    per (i = 0; i < %u; i++)\n    {\n"
            "        facta[i] = NIHIL;\n    }\n    redde _struere_",
            (insignatus integer)numerus, (insignatus integer)numerus);
        chorda_aedificator_appendere_literis(corpus, b);
        chorda_aedificator_appendere_chorda(corpus, normae[i].titulus);
        chorda_aedificator_appendere_literis(corpus,
            "(piscina, facta);\n}\n");
    }
    fons.caput   = chorda_aedificator_finire(caput);
    fons.corpus  = chorda_aedificator_finire(corpus);
    redde fons;
}
