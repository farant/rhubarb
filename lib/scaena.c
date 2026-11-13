/*
 * scaena.c - prospectus stantes cum rebus tangendis (scaena.h)
 *
 * scaena_struere plagulam bis ambulat: primo titulos prospectuum
 * colligit (exitus eos petunt), deinde prospectum quemque componit:
 * picturam per graphis, textus contractos, res ordine regionum.
 * Recusatio prima manet et omnia abicit.
 *
 * Massa: verbum, campi numerati (longitudo XXXII bitorum ordine
 * magno, deinde octeti), tabulae picturae crudae. Lector omnem
 * longitudinem contra fines et contra reliquum massae probat.
 */

#include "scaena.h"
#include "stml.h"
#include "internamentum.h"
#include "xar.h"
#include <string.h>

#define ERRORIS_MENSURA  CCLVI
#define PIXELA           (GRAPHIS_LATITUDO * GRAPHIS_ALTITUDO)
#define VERBUM_MASSAE    "SCAENA01"
#define VERBI_MENSURA    VIII
#define COLORES_PALETTAE XVI

nomen structura {
      Piscina* piscina;
          b32  fractum;
          i32  linea;
       chorda  plagula;
    character  error[ERRORIS_MENSURA];
} StatusScaenae;


/* ==================================================
 * Recusatio
 * ================================================== */

interior vacuum
_appone (
         StatusScaenae* s,
                   i32* longitudo,
    constans character* fons,
                   i32  mensura)
{
    i32 k;

    per (k = ZEPHYRUM;
         k < mensura && *longitudo + I < ERRORIS_MENSURA; k++)
    {
        character littera = fons[k];

        si (littera == '\n' || littera == '\r' || littera == '\t')
        {
            littera = ' ';
        }
        s->error[(*longitudo)++] = littera;
    }
    s->error[*longitudo] = '\0';
}

/* nuntius = ante + medium + inter + alterum + post */
interior b32
_recusare (
         StatusScaenae* s,
                   i32  linea,
    constans character* ante,
       constans chorda* medium,
    constans character* inter,
       constans chorda* alterum,
    constans character* post)
{
    i32 longitudo = ZEPHYRUM;

    si (s->fractum)
    {
        redde FALSUM;
    }
    s->fractum  = VERUM;
    s->linea    = linea;
    si (ante)
    {
        _appone(s, &longitudo, ante, (i32)strlen(ante));
    }
    si (medium && medium->datum)
    {
        _appone(s, &longitudo, (constans character*)medium->datum,
            medium->mensura > LXIV ? LXIV : medium->mensura);
    }
    si (inter)
    {
        _appone(s, &longitudo, inter, (i32)strlen(inter));
    }
    si (alterum && alterum->datum)
    {
        _appone(s, &longitudo, (constans character*)alterum->datum,
            alterum->mensura > LXIV ? LXIV : alterum->mensura);
    }
    si (post)
    {
        _appone(s, &longitudo, post, (i32)strlen(post));
    }
    redde FALSUM;
}


/* ==================================================
 * Lectio plagulae
 * ================================================== */

interior b32
_titulus_est (
       constans chorda* titulus,
    constans character* literae)
{
    redde titulus && chorda_aequalis_literis(*titulus, literae);
}

interior b32
_attributa_licita (
                   StatusScaenae* s,
              constans StmlNodus* nodus,
    constans character* constans* licita)
{
    i32 numerus = nodus->attributa ? xar_numerus(nodus->attributa)
        : ZEPHYRUM;
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans StmlAttributum* a =
            (constans StmlAttributum*)xar_obtinere(nodus->attributa,
                k);
                            b32 notum = FALSUM;

        per (j = ZEPHYRUM; a && licita[j]; j++)
        {
            si (_titulus_est(a->titulus, licita[j]))
            {
                notum = VERUM;
                frange;
            }
        }
        si (!notum)
        {
            redde _recusare(s, nodus->linea, "unknown attribute '",
                a ? a->titulus : NIHIL, "'", NIHIL, NIHIL);
        }
    }
    redde VERUM;
}

/* nomen: I .. LXIV litterae ex a-z, 0-9, '_' (et '-' si licet) */
interior b32
_nomen_validum (
    constans chorda* valor,
                b32  cum_linea)
{
    i32 k;

    si (   !valor || valor->mensura < I
        || valor->mensura > SCAENA_TITULUS_MAXIMUS)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < valor->mensura; k++)
    {
        i8 littera = valor->datum[k];

        si (   !(littera >= 'a' && littera <= 'z')
            && !(littera >= '0' && littera <= '9') && littera != '_'
            && !(cum_linea && littera == '-'))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* liber elementum k-tum nodi; NIHIL si non elementum */
interior StmlNodus*
_elementum (
    constans StmlNodus* nodus,
                   i32  k)
{
    StmlNodus** locus = (StmlNodus**)xar_obtinere(nodus->liberi, k);

    redde (locus && *locus && (*locus)->genus == STML_NODUS_ELEMENTUM)
        ? *locus : NIHIL;
}

interior i32
_liberorum (
    constans StmlNodus* nodus)
{
    redde nodus->liberi ? xar_numerus(nodus->liberi) : ZEPHYRUM;
}

interior b32
_habet_elementa (
    constans StmlNodus* nodus)
{
    i32 k;

    per (k = ZEPHYRUM; k < _liberorum(nodus); k++)
    {
        si (_elementum(nodus, k))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_spatium (
    i8 littera)
{
    redde littera == ' ' || littera == '\t' || littera == '\n'
        || littera == '\r';
}

/* textus nodi: spatia contracta, margines sublati */
interior chorda
_textus_contractus (
    StatusScaenae* s,
        StmlNodus* nodus)
{
    chorda crudus = stml_textus_internus(nodus, s->piscina);
    chorda exitus;
       i32 k;
       i32 n       = ZEPHYRUM;
       b32 hiatus  = FALSUM;

    exitus.mensura  = ZEPHYRUM;
    exitus.datum    = (i8*)piscina_allocare(s->piscina,
        (memoriae_index)(crudus.mensura ? crudus.mensura : I));
    si (!exitus.datum)
    {
        redde exitus;
    }
    per (k = ZEPHYRUM; k < crudus.mensura; k++)
    {
        si (_spatium(crudus.datum[k]))
        {
            hiatus = n > ZEPHYRUM;
            perge;
        }
        si (hiatus)
        {
            exitus.datum[n++]  = ' ';
            hiatus             = FALSUM;
        }
        exitus.datum[n++] = crudus.datum[k];
    }
    exitus.mensura = n;
    redde exitus;
}

hic_manens constans character* constans LICITA_NULLA[] = { NIHIL };
hic_manens constans character* constans LICITA_PROSPECTUS[] = {
    "titulus", "pictura", NIHIL
};
hic_manens constans character* constans LICITA_REI[] = {
    "titulus", "exitus", NIHIL
};

/* res prospectus ex plagula, ordine scripto */
interior b32
_res_legere (
        StatusScaenae* s,
            StmlNodus* nodus,
            ScaenaRes* res,
                  i32* lineae,
                  i32* rerum,
               chorda* descriptio)
{
    b32 descripta = FALSUM;
    i32 k;
    i32 j;

    *rerum = ZEPHYRUM;
    per (k = ZEPHYRUM; k < _liberorum(nodus); k++)
    {
        StmlNodus* liber = _elementum(nodus, k);

        si (!liber)
        {
            perge;
        }
        si (_habet_elementa(liber))
        {
            redde _recusare(s, liber->linea, "<", liber->titulus,
                "> holds text only", NIHIL, NIHIL);
        }
        si (_titulus_est(liber->titulus, "descriptio"))
        {
            si (!_attributa_licita(s, liber, LICITA_NULLA))
            {
                redde FALSUM;
            }
            si (descripta)
            {
                redde _recusare(s, liber->linea,
                    "a view has one <descriptio>, not two", NIHIL,
                    NIHIL, NIHIL, NIHIL);
            }
            descripta    = VERUM;
            *descriptio  = _textus_contractus(s, liber);
            si (descriptio->mensura < I)
            {
                redde _recusare(s, liber->linea,
                    "<descriptio> is empty", NIHIL, NIHIL, NIHIL,
                    NIHIL);
            }
            si (descriptio->mensura > SCAENA_DESCRIPTIO_MAXIMA)
            {
                redde _recusare(s, liber->linea,
                    "<descriptio> is longer than 2048 characters",
                    NIHIL, NIHIL, NIHIL, NIHIL);
            }
        }
        alioquin si (_titulus_est(liber->titulus, "res"))
        {
            constans chorda* titulus =
                stml_attributum_capere(liber, "titulus");
            constans chorda* exitus  =
                stml_attributum_capere(liber, "exitus");
                  ScaenaRes* r;

            si (!_attributa_licita(s, liber, LICITA_REI))
            {
                redde FALSUM;
            }
            si (!titulus || titulus->mensura < I)
            {
                redde _recusare(s, liber->linea,
                    "<res> attribute 'titulus' is required", NIHIL,
                    NIHIL, NIHIL, NIHIL);
            }
            per (j = ZEPHYRUM; j < *rerum; j++)
            {
                si (chorda_aequalis(res[j].titulus, *titulus))
                {
                    redde _recusare(s, liber->linea, "the thing '",
                        titulus, "' is written twice", NIHIL, NIHIL);
                }
            }
            si (*rerum >= GRAPHIS_TITULI_MAXIMI)
            {
                redde _recusare(s, liber->linea,
                    "more than 255 things in a view", NIHIL, NIHIL,
                    NIHIL, NIHIL);
            }
            r = &res[*rerum];
            memset(r, ZEPHYRUM, magnitudo(*r));
            r->titulus  = *titulus;
            r->textus   = _textus_contractus(s, liber);
            si (r->textus.mensura < I)
            {
                redde _recusare(s, liber->linea, "the thing '",
                    titulus, "' has no text", NIHIL, NIHIL);
            }
            si (r->textus.mensura > SCAENA_TEXTUS_MAXIMUS)
            {
                redde _recusare(s, liber->linea, "the text of '",
                    titulus, "' is longer than 512 characters", NIHIL,
                    NIHIL);
            }
            si (exitus)
            {
                r->exitus = *exitus;
            }
            lineae[*rerum] = liber->linea;
            (*rerum)++;
        }
        alioquin
        {
            redde _recusare(s, liber->linea, "<", liber->titulus,
                "> is an unknown element in a view", NIHIL, NIHIL);
        }
    }
    si (!descripta)
    {
        redde _recusare(s, nodus->linea,
            "the view has no <descriptio>", NIHIL, NIHIL, NIHIL,
            NIHIL);
    }
    redde VERUM;
}

/* prospectum unum componere */
interior b32
_prospectum_struere (
       StatusScaenae* s,
           StmlNodus* nodus,
     constans chorda* tituli,
                 i32  titulorum,
         GraphisFons  picturae,
         GraphisFons  partes,
              vacuum* contextus,
    ScaenaProspectus* exitus)
{
    constans chorda* titulus = stml_attributum_capere(nodus,
        "titulus");
    constans chorda* pictura = stml_attributum_capere(nodus,
        "pictura");
          ScaenaRes res[GRAPHIS_TITULI_MAXIMI];
                i32 lineae[GRAPHIS_TITULI_MAXIMI];
                b32 usa[GRAPHIS_TITULI_MAXIMI];
     GraphisFructus f;
             chorda textus;
                i32 rerum;
                i32 k;
                i32 j;

    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    si (!_nomen_validum(pictura, FALSUM))
    {
        redde _recusare(s, nodus->linea,
            "<prospectus> attribute 'pictura' must be 1 to 64"
            " characters from a-z, 0-9 and '_'", NIHIL, NIHIL, NIHIL,
            NIHIL);
    }
    si (!_res_legere(s, nodus, res, lineae, &rerum,
        &exitus->descriptio))
    {
        redde FALSUM;
    }
    /* exitus: prospectus alius huius plagulae, aut mundus */
    per (k = ZEPHYRUM; k < rerum; k++)
    {
        b32 notus = FALSUM;

        si (   res[k].exitus.mensura == ZEPHYRUM
            || chorda_aequalis_literis(res[k].exitus,
                SCAENA_EXITUS_MUNDUS))
        {
            perge;
        }
        si (chorda_aequalis(res[k].exitus, *titulus))
        {
            redde _recusare(s, lineae[k], "the exit of '",
                &res[k].titulus, "' leads to its own view", NIHIL,
                NIHIL);
        }
        per (j = ZEPHYRUM; j < titulorum; j++)
        {
            si (chorda_aequalis(res[k].exitus, tituli[j]))
            {
                notus = VERUM;
            }
        }
        si (!notus)
        {
            redde _recusare(s, lineae[k], "the exit of '",
                &res[k].titulus, "' names a view '", &res[k].exitus,
                "' that is not in this file");
        }
    }
    /* pictura */
    textus = picturae ? picturae(*pictura, contextus, s->piscina)
        : chorda_ex_buffer(NIHIL, ZEPHYRUM);
    si (!textus.datum)
    {
        redde _recusare(s, nodus->linea, "the picture '", pictura,
            "' was not found", NIHIL, NIHIL);
    }
    f = graphis_legere_cum_fonte(textus, partes, contextus,
        s->piscina);
    si (!f.successus)
    {
        (vacuum)_recusare(s, f.linea, NIHIL, &f.error, NIHIL, NIHIL,
            NIHIL);
        s->plagula = f.plagula.mensura ? f.plagula
            : chorda_transcribere(*pictura, s->piscina);
        redde FALSUM;
    }
    /* convenientia utrimque */
    memset(usa, ZEPHYRUM, magnitudo(usa));
    exitus->rerum = f.tabula->titulorum;
    exitus->res   = (ScaenaRes*)piscina_allocare(s->piscina,
        (memoriae_index)((exitus->rerum ? exitus->rerum : I)
            * magnitudo(ScaenaRes)));
    si (!exitus->res)
    {
        redde _recusare(s, ZEPHYRUM, "out of memory", NIHIL, NIHIL,
            NIHIL, NIHIL);
    }
    per (k = ZEPHYRUM; k < f.tabula->titulorum; k++)
    {
        b32 inventa = FALSUM;

        per (j = ZEPHYRUM; j < rerum; j++)
        {
            si (chorda_aequalis(res[j].titulus, f.tabula->tituli[k]))
            {
                inventa  = VERUM;
                usa[j]   = VERUM;
                exitus->res[k].titulus = chorda_transcribere(
                    res[j].titulus, s->piscina);
                exitus->res[k].textus  = res[j].textus;
                exitus->res[k].exitus  = chorda_transcribere(
                    res[j].exitus, s->piscina);
            }
        }
        si (!inventa)
        {
            redde _recusare(s, nodus->linea, "the region '",
                &f.tabula->tituli[k], "' of picture '", pictura,
                "' has no <res>");
        }
    }
    per (j = ZEPHYRUM; j < rerum; j++)
    {
        si (!usa[j])
        {
            redde _recusare(s, lineae[j], "the thing '",
                &res[j].titulus, "' is not a region of picture '",
                pictura, "'");
        }
    }
    exitus->titulus  = chorda_transcribere(*titulus, s->piscina);
    exitus->indices  = f.indices;
    exitus->domini   = f.tabula->domini;
    redde VERUM;
}

interior ScaenaFructus
_fructus_recusatus (
    constans StatusScaenae* s,
                   Piscina* piscina)
{
    ScaenaFructus f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    f.error    = chorda_ex_literis(s->error, piscina);
    f.linea    = s->linea;
    f.plagula  = s->plagula;
    redde f;
}

ScaenaFructus
scaena_struere (
         chorda  textus,
    GraphisFons  picturae,
    GraphisFons  partes,
         vacuum* contextus,
        Piscina* piscina)
{
       ScaenaFructus  f;
       StatusScaenae  s;
        StmlResultus  r;
              chorda  tituli[SCAENA_PROSPECTUS_MAXIMI];
           StmlNodus* nodi[SCAENA_PROSPECTUS_MAXIMI];
    ScaenaProspectus* prospectus;
                 i32  numerus = ZEPHYRUM;
                 i32  k;
                 i32  j;

    memset(&f, ZEPHYRUM, magnitudo(f));
    memset(&s, ZEPHYRUM, magnitudo(s));
    si (!piscina)
    {
        redde f;
    }
    s.piscina = piscina;
    r = stml_legere(textus, piscina, internamentum_creare(piscina));
    si (!r.successus || !r.elementum_radix)
    {
        chorda causa = r.error;

        (vacuum)_recusare(&s, r.successus ? ZEPHYRUM : r.linea_erroris,
            "the scene file is not valid STML: ",
            causa.mensura ? &causa : NIHIL,
            causa.mensura ? NIHIL : "no root element", NIHIL, NIHIL);
        redde _fructus_recusatus(&s, piscina);
    }
    si (!_titulus_est(r.elementum_radix->titulus, "scaenae"))
    {
        (vacuum)_recusare(&s, r.elementum_radix->linea,
            "the root element must be <scaenae>", NIHIL, NIHIL, NIHIL,
            NIHIL);
        redde _fructus_recusatus(&s, piscina);
    }
    si (!_attributa_licita(&s, r.elementum_radix, LICITA_NULLA))
    {
        redde _fructus_recusatus(&s, piscina);
    }
    /* gradus primus: tituli prospectuum */
    per (k = ZEPHYRUM; k < _liberorum(r.elementum_radix); k++)
    {
              StmlNodus* liber = _elementum(r.elementum_radix, k);
        constans chorda* titulus;

        si (!liber)
        {
            perge;
        }
        si (!_titulus_est(liber->titulus, "prospectus"))
        {
            (vacuum)_recusare(&s, liber->linea, "<", liber->titulus,
                "> is an unknown element (expected <prospectus>)",
                NIHIL, NIHIL);
            redde _fructus_recusatus(&s, piscina);
        }
        si (!_attributa_licita(&s, liber, LICITA_PROSPECTUS))
        {
            redde _fructus_recusatus(&s, piscina);
        }
        titulus = stml_attributum_capere(liber, "titulus");
        si (!_nomen_validum(titulus, VERUM))
        {
            (vacuum)_recusare(&s, liber->linea,
                "<prospectus> attribute 'titulus' must be 1 to 64"
                " characters from a-z, 0-9, '-' and '_'", NIHIL,
                NIHIL, NIHIL, NIHIL);
            redde _fructus_recusatus(&s, piscina);
        }
        si (chorda_aequalis_literis(*titulus, SCAENA_EXITUS_MUNDUS))
        {
            (vacuum)_recusare(&s, liber->linea,
                "a view cannot be titled 'mundus' (that name means"
                " outside)", NIHIL, NIHIL, NIHIL, NIHIL);
            redde _fructus_recusatus(&s, piscina);
        }
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (chorda_aequalis(tituli[j], *titulus))
            {
                (vacuum)_recusare(&s, liber->linea, "the view '",
                    titulus, "' is written twice", NIHIL, NIHIL);
                redde _fructus_recusatus(&s, piscina);
            }
        }
        si (numerus >= SCAENA_PROSPECTUS_MAXIMI)
        {
            (vacuum)_recusare(&s, liber->linea,
                "more than 64 views in one file", NIHIL, NIHIL, NIHIL,
                NIHIL);
            redde _fructus_recusatus(&s, piscina);
        }
        tituli[numerus]  = *titulus;
        nodi[numerus]    = liber;
        numerus++;
    }
    si (numerus == ZEPHYRUM)
    {
        (vacuum)_recusare(&s, r.elementum_radix->linea,
            "the scene file has no <prospectus>", NIHIL, NIHIL, NIHIL,
            NIHIL);
        redde _fructus_recusatus(&s, piscina);
    }
    prospectus = (ScaenaProspectus*)piscina_allocare(piscina,
        (memoriae_index)(numerus * magnitudo(ScaenaProspectus)));
    si (!prospectus)
    {
        (vacuum)_recusare(&s, ZEPHYRUM, "out of memory", NIHIL, NIHIL,
            NIHIL, NIHIL);
        redde _fructus_recusatus(&s, piscina);
    }
    /* gradus alter: prospectus quisque */
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (!_prospectum_struere(&s, nodi[k], tituli, numerus,
            picturae, partes, contextus, &prospectus[k]))
        {
            redde _fructus_recusatus(&s, piscina);
        }
    }
    f.successus   = VERUM;
    f.prospectus  = prospectus;
    f.numerus     = numerus;
    redde f;
}


/* ==================================================
 * Massa
 * ================================================== */

interior vacuum
_scribe_numerum (
     i8* scopus,
    i32* cursor,
    i32  numerus)
{
    scopus[(*cursor)++] = (i8)(numerus >> XXIV);
    scopus[(*cursor)++] = (i8)(numerus >> XVI);
    scopus[(*cursor)++] = (i8)(numerus >> VIII);
    scopus[(*cursor)++] = (i8)numerus;
}

interior vacuum
_scribe_campum (
        i8* scopus,
       i32* cursor,
    chorda  campus)
{
    _scribe_numerum(scopus, cursor, campus.mensura);
    si (campus.mensura)
    {
        memcpy(scopus + *cursor, campus.datum, campus.mensura);
        *cursor += campus.mensura;
    }
}

chorda
scaena_scribere (
    constans ScaenaProspectus* prospectus,
                      Piscina* piscina)
{
    chorda massa;
       i32 mensura;
       i32 cursor = ZEPHYRUM;
       i32 k;

    massa.datum    = NIHIL;
    massa.mensura  = ZEPHYRUM;
    si (   !prospectus || !piscina || !prospectus->indices
        || !prospectus->domini
        || (prospectus->rerum > ZEPHYRUM && !prospectus->res))
    {
        redde massa;
    }
    mensura = VERBI_MENSURA + IV + prospectus->titulus.mensura + IV
        + prospectus->descriptio.mensura + IV + II * PIXELA;
    per (k = ZEPHYRUM; k < prospectus->rerum; k++)
    {
        mensura += III * IV + prospectus->res[k].titulus.mensura
            + prospectus->res[k].textus.mensura
            + prospectus->res[k].exitus.mensura;
    }
    massa.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)mensura);
    si (!massa.datum)
    {
        redde massa;
    }
    memcpy(massa.datum, VERBUM_MASSAE, VERBI_MENSURA);
    cursor = VERBI_MENSURA;
    _scribe_campum(massa.datum, &cursor, prospectus->titulus);
    _scribe_campum(massa.datum, &cursor, prospectus->descriptio);
    _scribe_numerum(massa.datum, &cursor, prospectus->rerum);
    per (k = ZEPHYRUM; k < prospectus->rerum; k++)
    {
        _scribe_campum(massa.datum, &cursor,
            prospectus->res[k].titulus);
        _scribe_campum(massa.datum, &cursor,
            prospectus->res[k].textus);
        _scribe_campum(massa.datum, &cursor,
            prospectus->res[k].exitus);
    }
    memcpy(massa.datum + cursor, prospectus->indices, PIXELA);
    cursor += PIXELA;
    memcpy(massa.datum + cursor, prospectus->domini, PIXELA);
    cursor         += PIXELA;
    massa.mensura  = cursor;
    redde massa;
}

/* numerus XXXII bitorum; FALSUM si massa deficit */
interior b32
_lege_numerum (
    chorda  massa,
       i32* cursor,
       i32* exitus)
{
    si (massa.mensura < IV || *cursor > massa.mensura - IV)
    {
        redde FALSUM;
    }
    *exitus = ((i32)massa.datum[*cursor] << XXIV)
        | ((i32)massa.datum[*cursor + I] << XVI)
        | ((i32)massa.datum[*cursor + II] << VIII)
        | (i32)massa.datum[*cursor + III];
    *cursor += IV;
    redde VERUM;
}

/* campus numeratus, in piscinam exscriptus; longitudo intra
 * maximum et intra reliquum massae */
interior b32
_lege_campum (
     chorda  massa,
        i32* cursor,
        i32  maximum,
     chorda* exitus,
    Piscina* piscina)
{
    i32 mensura;

    si (   !_lege_numerum(massa, cursor, &mensura)
        || mensura > maximum || mensura > massa.mensura - *cursor)
    {
        redde FALSUM;
    }
    exitus->mensura  = mensura;
    exitus->datum    = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura ? mensura : I));
    si (!exitus->datum)
    {
        redde FALSUM;
    }
    memcpy(exitus->datum, massa.datum + *cursor, mensura);
    *cursor += mensura;
    redde VERUM;
}

b32
scaena_legere (
              chorda  massa,
    ScaenaProspectus* exitus,
             Piscina* piscina)
{
    i32 cursor = VERBI_MENSURA;
    i32 k;

    si (   !exitus || !piscina || !massa.datum
        || massa.mensura < VERBI_MENSURA
        || memcmp(massa.datum, VERBUM_MASSAE, VERBI_MENSURA)
            != ZEPHYRUM)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(*exitus));
    si (   !_lege_campum(massa, &cursor, SCAENA_TITULUS_MAXIMUS,
            &exitus->titulus, piscina)
        || exitus->titulus.mensura < I
        || !_lege_campum(massa, &cursor, SCAENA_DESCRIPTIO_MAXIMA,
            &exitus->descriptio, piscina)
        || !_lege_numerum(massa, &cursor, &exitus->rerum)
        || exitus->rerum > GRAPHIS_TITULI_MAXIMI)
    {
        redde FALSUM;
    }
    exitus->res = (ScaenaRes*)piscina_allocare(piscina,
        (memoriae_index)((exitus->rerum ? exitus->rerum : I)
            * magnitudo(ScaenaRes)));
    si (!exitus->res)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < exitus->rerum; k++)
    {
        si (   !_lege_campum(massa, &cursor, SCAENA_TITULUS_MAXIMUS,
                &exitus->res[k].titulus, piscina)
            || !_lege_campum(massa, &cursor, SCAENA_TEXTUS_MAXIMUS,
                &exitus->res[k].textus, piscina)
            || !_lege_campum(massa, &cursor, SCAENA_TITULUS_MAXIMUS,
                &exitus->res[k].exitus, piscina))
        {
            redde FALSUM;
        }
    }
    /* tabulae: exacte quod restat */
    si (massa.mensura - cursor != II * PIXELA)
    {
        redde FALSUM;
    }
    exitus->indices = (i8*)piscina_allocare(piscina,
        (memoriae_index)PIXELA);
    exitus->domini  = (i8*)piscina_allocare(piscina,
        (memoriae_index)PIXELA);
    si (!exitus->indices || !exitus->domini)
    {
        redde FALSUM;
    }
    memcpy(exitus->indices, massa.datum + cursor, PIXELA);
    memcpy(exitus->domini, massa.datum + cursor + PIXELA, PIXELA);
    per (k = ZEPHYRUM; k < PIXELA; k++)
    {
        si (   exitus->indices[k] >= COLORES_PALETTAE
            || exitus->domini[k] > exitus->rerum)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}
