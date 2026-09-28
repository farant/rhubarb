/* toml_lector.c - Lector toml (vide toml_lector.h)
 *
 * Functio (modi, positionis): nulla memoria praeter situm et
 * profunditatem. Lexema = segmentum fontis [cursor, ad); fons numquam
 * copiatur. Columnae in octetis.
 */

#include "toml_lector.h"
#include "utf8.h"
#include <string.h>


/* ==================================================
 * Octeti et situs
 * ================================================== */

interior s32
_octetus (
    constans TomlLector* l,
                    s32  i)
{
    si (i < ZEPHYRUM || i >= l->mensura)
    {
        redde (s32)-I;
    }
    redde (s32)(insignatus character)l->fons[i];
}

interior b32
_digitus (
    s32 c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_littera (
    s32 c)
{
    redde (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

/* clavis nuda: A-Za-z0-9_- */
interior b32
_nudus (
    s32 c)
{
    redde _littera(c) || _digitus(c) || c == '_' || c == '-';
}

/* lexema numero simile: A-Za-z0-9_+-.: */
interior b32
_valoris (
    s32 c)
{
    redde _nudus(c) || c == '+' || c == '.' || c == ':';
}

interior vacuum
_progredi (
    TomlLector* l,
           s32  ad)
{
    s32 i;

    per (i = l->situs.cursor; i < ad; i++)
    {
        si (l->fons[i] == '\n')
        {
            l->situs.linea++;
            l->situs.linea_initium = i + I;
        }
    }
    l->situs.cursor = ad;
}

/* chorda.datum i8* est sed fons constans: unio (exemplar crusta). */
interior chorda
_segmentum (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = fons + ab;
    c.datum    = u.m;
    c.mensura  = (i32)(ad - ab);
    redde c;
}

/* lexema [cursor, ad) generis dati */
interior MateriaToken*
_facere (
    TomlLector* l,
           s32  genus,
           s32  ad)
{
    MateriaToken* t;
             s32  ab = l->situs.cursor;

    t = materia_token_creare(l->piscina, &l->forma, genus,
        _segmentum(l->fons, ab, ad), ab, l->situs.linea,
        (i32)(ab - l->situs.linea_initium) + I, ZEPHYRUM);
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    _progredi(l, ad);
    redde t;
}

/* sequentia UTF-8 una (aut octetus unus si invalida) ut IGNOTUM */
interior MateriaToken*
_ignotum (
    TomlLector* l)
{
    constans i8* p        = (constans i8*)(l->fons + l->situs.cursor);
    constans i8* initium  = p;
    constans i8* finis    = (constans i8*)(l->fons + l->mensura);
            s32  longitudo;

    (vacuum)utf8_decodere(&p, finis);
    longitudo = (s32)(p - initium);
    si (longitudo < (s32)I)
    {
        longitudo = (s32)I;
    }
    redde _facere(l, (s32)TOML_LEX_IGNOTUM,
        l->situs.cursor + longitudo);
}


/* ==================================================
 * Chordae: fines quaerere (effugia solum ut claudens inveniatur;
 * decodatio in coctione)
 * ================================================== */

/* "..." aut '...': usque ad claudentem aut ante lineam novam (chorda
 * aperta tota manet, lexema lineam non transit) */
interior s32
_finis_chordae (
    constans TomlLector* l,
                    s32  ab,
                    s32  claudens,
                    b32  effugia)
{
    s32 j = ab + I;

    dum (j < l->mensura)
    {
        s32 b = _octetus(l, j);

        si (b == '\n' || (b == '\r' && _octetus(l, j + I) == '\n'))
        {
            redde j;
        }
        si (   effugia && b == '\\' && j + I < l->mensura
            && _octetus(l, j + I) != '\n')
        {
            j += II;
            perge;
        }
        si (b == claudens)
        {
            redde j + I;
        }
        j++;
    }
    redde l->mensura;
}

/* """...""" aut '''...''': claudens = primi tres; usque ad duo
 * signa ultra contento pertinent ('"""a"""""' = contentum 'a""').
 * Aperta usque ad finem fontis. */
interior s32
_finis_multae (
    constans TomlLector* l,
                    s32  ab,
                    s32  claudens,
                    b32  effugia)
{
    s32 j = ab + III;

    dum (j < l->mensura)
    {
        si (effugia && _octetus(l, j) == '\\' && j + I < l->mensura)
        {
            j += II;
            perge;
        }
        si (   _octetus(l, j)      == claudens
            && _octetus(l, j + I)  == claudens
            && _octetus(l, j + II) == claudens)
        {
            i32 plus = ZEPHYRUM;

            j += III;
            dum (plus < II && _octetus(l, j) == claudens)
            {
                j++;
                plus++;
            }
            redde j;
        }
        j++;
    }
    redde l->mensura;
}


/* ==================================================
 * Lexema numero simile
 * ================================================== */

/* forma diei integri DDDD-DD-DD ab 'ab' */
interior b32
_dies_integer (
    constans TomlLector* l,
                    s32  ab)
{
    s32 k;

    per (k = ZEPHYRUM; k < (s32)X; k++)
    {
        s32 c = _octetus(l, ab + k);

        si (k == IV || k == VII)
        {
            si (c != '-')
            {
                redde FALSUM;
            }
        }
        alioquin si (!_digitus(c))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_aequat (
    constans TomlLector* l,
                    s32  ab,
                    s32  ad,
     constans character* verbum)
{
    s32 n = (s32)strlen(verbum);

    redde (ad - ab) == n
        && memcmp(l->fons + ab, verbum, (size_t)n) == ZEPHYRUM;
}

interior MateriaToken*
_numero_simile (
    TomlLector* l)
{
    s32 ab  = l->situs.cursor;
    s32 j   = ab;
    s32 k;
    b32 colon = FALSUM;
    s32 primus;

    dum (VERUM)
    {
        dum (j < l->mensura && _valoris(_octetus(l, j)))
        {
            j++;
        }
        /* spatium unum diem integrum et horam iungit */
        si (   j - ab == (s32)X && _dies_integer(l, ab)
            && _octetus(l, j) == ' ' && _digitus(_octetus(l, j + I))
            && _digitus(_octetus(l, j + II))
            && _octetus(l, j + III) == ':')
        {
            j++;
            perge;
        }
        frange;
    }

    si (_aequat(l, ab, j, "true"))
    {
        redde _facere(l, (s32)TOML_LEX_VERUM, j);
    }
    si (_aequat(l, ab, j, "false"))
    {
        redde _facere(l, (s32)TOML_LEX_FALSUM, j);
    }
    per (k = ab; k < j; k++)
    {
        si (_octetus(l, k) == ':')
        {
            colon = VERUM;
        }
    }
    si (   colon
        || (j - ab >= (s32)V && _digitus(_octetus(l, ab))
            && _digitus(_octetus(l, ab + I))
            && _digitus(_octetus(l, ab + II))
            && _digitus(_octetus(l, ab + III))
            && _octetus(l, ab + IV) == '-'))
    {
        redde _facere(l, (s32)TOML_LEX_TEMPUS, j);
    }
    primus = _octetus(l, ab);
    si (   _digitus(primus) || primus == '+' || primus == '-'
        || (j - ab >= (s32)III
            && (memcmp(l->fons + ab, "inf", III) == ZEPHYRUM
                || memcmp(l->fons + ab, "nan", III) == ZEPHYRUM)))
    {
        redde _facere(l, (s32)TOML_LEX_NUMERUS, j);
    }
    redde _facere(l, (s32)TOML_LEX_IGNOTUM, j);
}


/* ==================================================
 * Modi
 * ================================================== */

interior MateriaToken*
_clavis (
    TomlLector* l,
           s32  c)
{
    s32 i = l->situs.cursor;
    s32 j;

    si (_nudus(c))
    {
        j = i;
        dum (j < l->mensura && _nudus(_octetus(l, j)))
        {
            j++;
        }
        redde _facere(l, (s32)TOML_LEX_CLAVIS_NUDA, j);
    }
    commutatio (c)
    {
        casus '"':
            redde _facere(l, (s32)TOML_LEX_CLAVIS_GEMINA,
                _finis_chordae(l, i, '"', VERUM));
        casus '\'':
            redde _facere(l, (s32)TOML_LEX_CLAVIS_SIMPLEX,
                _finis_chordae(l, i, '\'', FALSUM));
        casus '.':
            redde _facere(l, (s32)TOML_LEX_PUNCTUM, i + I);
        casus '=':
            redde _facere(l, (s32)TOML_LEX_SIGNUM, i + I);
        casus '[':
            si (_octetus(l, i + I) == '[')
            {
                redde _facere(l,
                    (s32)TOML_LEX_SERIES_TABULARUM_APERTURA,
                    i + II);
            }
            redde _facere(l, (s32)TOML_LEX_TABULA_APERTURA, i + I);
        casus ']':
            si (_octetus(l, i + I) == ']')
            {
                redde _facere(l,
                    (s32)TOML_LEX_SERIES_TABULARUM_CLAUSURA,
                    i + II);
            }
            redde _facere(l, (s32)TOML_LEX_TABULA_CLAUSURA, i + I);
        casus '}':
            redde _facere(l, (s32)TOML_LEX_COMPACTA_CLAUSURA, i + I);
        casus ',':
            redde _facere(l, (s32)TOML_LEX_COMMA, i + I);
        ordinarius:
            frange;
    }
    redde _ignotum(l);
}

interior MateriaToken*
_valor (
    TomlLector* l,
           s32  c)
{
    s32 i = l->situs.cursor;

    commutatio (c)
    {
        casus '"':
            si (_octetus(l, i + I) == '"' && _octetus(l, i + II) == '"')
            {
                redde _facere(l, (s32)TOML_LEX_CHORDA_GEMINA_MULTA,
                    _finis_multae(l, i, '"', VERUM));
            }
            redde _facere(l, (s32)TOML_LEX_CHORDA_GEMINA,
                _finis_chordae(l, i, '"', VERUM));
        casus '\'':
            si (   _octetus(l, i + I)  == '\''
                && _octetus(l, i + II) == '\'')
            {
                redde _facere(l, (s32)TOML_LEX_CHORDA_SIMPLEX_MULTA,
                    _finis_multae(l, i, '\'', FALSUM));
            }
            redde _facere(l, (s32)TOML_LEX_CHORDA_SIMPLEX,
                _finis_chordae(l, i, '\'', FALSUM));
        casus '[':
            redde _facere(l, (s32)TOML_LEX_SERIES_APERTURA, i + I);
        casus ']':
            redde _facere(l, (s32)TOML_LEX_SERIES_CLAUSURA, i + I);
        casus '{':
            redde _facere(l, (s32)TOML_LEX_COMPACTA_APERTURA, i + I);
        casus '}':
            redde _facere(l, (s32)TOML_LEX_COMPACTA_CLAUSURA, i + I);
        casus ',':
            redde _facere(l, (s32)TOML_LEX_COMMA, i + I);
        ordinarius:
            frange;
    }
    si (_valoris(c))
    {
        redde _numero_simile(l);
    }
    redde _ignotum(l);
}


/* ==================================================
 * Publica
 * ================================================== */

b32
toml_lector_incipere (
             TomlLector* lector,
                Piscina* piscina,
     constans character* fons,
                    s32  mensura)
{
    si (   lector == NIHIL || piscina == NIHIL || fons == NIHIL
        || mensura < ZEPHYRUM)
    {
        redde FALSUM;
    }
    lector->piscina               = piscina;
    lector->fons                  = fons;
    lector->mensura               = mensura;
    lector->situs.cursor          = ZEPHYRUM;
    lector->situs.linea           = I;
    lector->situs.linea_initium   = ZEPHYRUM;
    lector->profunditas           = ZEPHYRUM;
    lector->forma.mensura_caudae  = ZEPHYRUM;
    redde VERUM;
}

MateriaToken*
toml_lector_proximum (
     TomlLector* l,
      TomlModus  modus)
{
    s32 i = l->situs.cursor;
    s32 c = _octetus(l, i);
    s32 j;

    si (i >= l->mensura)
    {
        redde _facere(l, (s32)TOML_LEX_FINIS, l->mensura);
    }

    /* communia: spatia, commentum, linea nova */
    si (c == ' ' || c == '\t')
    {
        j = i;
        dum (_octetus(l, j) == ' ' || _octetus(l, j) == '\t')
        {
            j++;
        }
        redde _facere(l, (s32)TOML_LEX_SPATIUM, j);
    }
    si (c == '#')
    {
        j = i;
        dum (   j < l->mensura && _octetus(l, j) != '\n'
             && !(_octetus(l, j) == '\r' && _octetus(l, j + I) == '\n'))
        {
            j++;
        }
        redde _facere(l, (s32)TOML_LEX_COMMENTUM, j);
    }
    si (c == '\n' || (c == '\r' && _octetus(l, i + I) == '\n'))
    {
        redde _facere(l, l->profunditas == ZEPHYRUM
            ? (s32)TOML_LEX_LINEA_FINIS : (s32)TOML_LEX_LINEA,
            i + (c == '\r' ? II : I));
    }
    si (c == '\r')
    {
        redde _facere(l, (s32)TOML_LEX_IGNOTUM, i + I);
    }

    si (modus == TOML_MODUS_VALOR)
    {
        redde _valor(l, c);
    }
    redde _clavis(l, c);
}

TomlSitus
toml_lector_situs (
    constans TomlLector* lector)
{
    redde lector->situs;
}

vacuum
toml_lector_situm_reponere (
     TomlLector* lector,
      TomlSitus  situs)
{
    lector->situs = situs;
}

vacuum
toml_lector_profunditatem_ponere (
    TomlLector* lector,
           i32  profunditas)
{
    lector->profunditas = profunditas;
}
