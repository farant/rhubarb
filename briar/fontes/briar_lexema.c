/* briar_lexema.c - Vide briar_lexema.h. Tabula linearum ex
 * md_lexema.c transcripta (una regula: terminator "\n", "\r", "\r\n";
 * terminator ultimus lineam vacuam ulteriorem non parit). */

#include "briar_lexema.h"
#include "briar_lexicon.h"
#include <string.h>

#define SAEPTUM_CURSUS_MINIMUS  III
#define SAEPTUM_INDENTATIO_MAX  III

b32
briar_lineae_scindere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
           BriarLineae* exitus)
{
    s32 i;
    s32 initium;
    i32 numerus;

    exitus->fons     = fons;
    exitus->mensura  = mensura;
    exitus->lineae   = xar_creare(piscina, (i32)magnitudo(BriarLinea));
    si (exitus->lineae == NIHIL)
    {
        redde FALSUM;
    }
    i        = ZEPHYRUM;
    initium  = ZEPHYRUM;
    numerus  = ZEPHYRUM;
    dum (i < (s32)mensura)
    {
        character c = fons[i];

        si (c == '\n' || c == '\r')
        {
            BriarLinea* l;
                   i32  term = I;

            si (   c           == '\r' && i + I < (s32)mensura
                && fons[i + I] == '\n')
            {
                term = II;
            }
            l = (BriarLinea*)xar_addere(exitus->lineae);
            si (l == NIHIL)
            {
                redde FALSUM;
            }
            numerus        = numerus + I;
            l->offset      = initium;
            l->mensura     = (i32)(i - initium);
            l->terminator  = term;
            l->numerus     = numerus;
            i              = i + (s32)term;
            initium        = i;
            perge;
        }
        i = i + I;
    }
    si (initium < (s32)mensura)
    {
        BriarLinea* l = (BriarLinea*)xar_addere(exitus->lineae);

        si (l == NIHIL)
        {
            redde FALSUM;
        }
        numerus        = numerus + I;
        l->offset      = initium;
        l->mensura     = (i32)((s32)mensura - initium);
        l->terminator  = ZEPHYRUM;
        l->numerus     = numerus;
    }
    redde VERUM;
}

i32
briar_lineae_numerus (
    constans BriarLineae* lineae)
{
    redde (lineae == NIHIL || lineae->lineae == NIHIL)
        ? ZEPHYRUM : xar_numerus(lineae->lineae);
}

constans BriarLinea*
briar_linea (
    constans BriarLineae* lineae,
                     i32  index)
{
    si (index < ZEPHYRUM || index >= briar_lineae_numerus(lineae))
    {
        redde NIHIL;
    }
    redde (constans BriarLinea*)xar_obtinere(lineae->lineae, index);
}

chorda
briar_chorda_fontis (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    chorda c;
    unio { constans character* c; i8* i; } u;

    u.c        = fons + ab;
    c.datum    = u.i;
    c.mensura  = (i32)(ad - ab);
    redde c;
}

interior b32
_initium_nominis (
    character c)
{
    redde (b32)((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || c == '_');
}

interior b32
_character_nominis (
    character c)
{
    redde (b32)(_initium_nominis(c) || (c >= '0' && c <= '9')
        || c == '-');
}

BriarClassis
briar_lineam_classificare (
    constans BriarLineae* lineae,
                     i32  index,
            BriarSaeptum* saeptum)
{
           BriarClassis  c;
    constans BriarLinea* l = briar_linea(lineae, index);
     constans character* t;
                    i32  n;
                    i32  i;

    c.genus            = BRIAR_LINEA_PROSA;
    c.titulus.datum    = NIHIL;
    c.titulus.mensura  = ZEPHYRUM;
    c.crudus           = FALSUM;
    c.dedens           = FALSUM;
    c.auto_clausum     = FALSUM;
    si (l == NIHIL)
    {
        redde c;
    }
    t = lineae->fons + l->offset;
    n = l->mensura;

    /* interpres: linea prima sola */
    si (index == ZEPHYRUM && n >= II && t[0] == '#' && t[1] == '!')
    {
        c.genus = BRIAR_LINEA_INTERPRES;
        redde c;
    }

    /* saeptum markdown: <= III spatia, deinde ``` aut ~~~ */
    {
              i32 spatia = ZEPHYRUM;
              i32 cursus = ZEPHYRUM;
        character signum;

        dum (   spatia < n && spatia < SAEPTUM_INDENTATIO_MAX
             && t[spatia] == ' ')
        {
            spatia = spatia + I;
        }
        si (spatia < n && (t[spatia] == '`' || t[spatia] == '~'))
        {
            signum = t[spatia];
            dum (spatia + cursus < n && t[spatia + cursus] == signum)
            {
                cursus = cursus + I;
            }
            si (cursus >= SAEPTUM_CURSUS_MINIMUS)
            {
                si (saeptum->aperta)
                {
                    /* clausura: idem signum, cursus >= aperiens,
                     * deinde spatia sola */
                    si (   signum == saeptum->signum
                        && cursus >= saeptum->longitudo)
                    {
                        i32 k     = spatia + cursus;
                        b32 sola  = VERUM;

                        dum (k < n)
                        {
                            si (t[k] != ' ' && t[k] != '\t')
                            {
                                sola = FALSUM;
                                frange;
                            }
                            k = k + I;
                        }
                        si (sola)
                        {
                            saeptum->aperta = FALSUM;
                        }
                    }
                    redde c;
                }
                saeptum->aperta     = VERUM;
                saeptum->signum     = signum;
                saeptum->longitudo  = cursus;
                redde c;
            }
        }
    }
    si (saeptum->aperta)
    {
        redde c;   /* intra saeptum omnis linea prosa */
    }

    /* tagi in columna 0 */
    si (n >= II && t[0] == '<')
    {
        si (_initium_nominis(t[1]))
        {
            i32 f;

            i = I;
            dum (i < n && _character_nominis(t[i]))
            {
                i = i + I;
            }
            c.genus    = BRIAR_LINEA_TAG_APERTUM;
            c.titulus  = briar_chorda_fontis(lineae->fons, l->offset
                + I,
                l->offset + (s32)i);
            si (i < n && t[i] == '!')
            {
                c.crudus  = VERUM;
                i         = i + I;
            }
            si (i < n && t[i] == '\\')
            {
                c.dedens = VERUM;
            }
            f = n;
            dum (f > ZEPHYRUM && (t[f - I] == ' ' || t[f - I] == '\t'))
            {
                f = f - I;
            }
            si (f >= II && t[f - II] == '/' && t[f - I] == '>')
            {
                c.auto_clausum = VERUM;
            }
            redde c;
        }
        si (t[1] == '/' && n >= (i32)4 && _initium_nominis(t[2]))
        {
            i = II;
            dum (i < n && _character_nominis(t[i]))
            {
                i = i + I;
            }
            si (i < n && t[i] == '>')
            {
                c.genus    = BRIAR_LINEA_TAG_CLAUSUM;
                c.titulus  = briar_chorda_fontis(lineae->fons,
                    l->offset + II, l->offset + (s32)i);
            }
            redde c;
        }
    }
    redde c;
}

vacuum
briar_fabrica_incipere (
             BriarFabrica* fabrica,
                  Piscina* piscina,
     constans BriarLineae* lineae)
{
    fabrica->piscina               = piscina;
    fabrica->lineae                = lineae;
    fabrica->forma.mensura_caudae  = ZEPHYRUM;
    fabrica->linea_ultima          = ZEPHYRUM;
}

/* linea quae octetum 'ab' continet (cursor resumit; retro licet) */
interior constans BriarLinea*
_lineam_invenire (
    BriarFabrica* fabrica,
             s32  ab)
{
    i32 n = briar_lineae_numerus(fabrica->lineae);
    i32 i = fabrica->linea_ultima;

    si (i >= n)
    {
        i = (n > ZEPHYRUM) ? n - I : ZEPHYRUM;
    }
    dum (i > ZEPHYRUM && briar_linea(fabrica->lineae, i)->offset > ab)
    {
        i = i - I;
    }
    dum (i + I < n && briar_linea(fabrica->lineae, i + I)->offset <= ab)
    {
        i = i + I;
    }
    fabrica->linea_ultima = i;
    redde briar_linea(fabrica->lineae, i);
}

MateriaToken*
briar_lexema_fabricare (
    BriarFabrica* fabrica,
             s32  genus,
             s32  ab,
             s32  ad)
{
    constans BriarLinea* l        = _lineam_invenire(fabrica, ab);
                    i32  linea    = I;
                    i32  columna  = I;

    si (l != NIHIL)
    {
        linea    = l->numerus;
        columna  = (i32)(ab - l->offset) + I;
    }
    redde materia_token_creare(fabrica->piscina, &fabrica->forma, genus,
        briar_chorda_fontis(fabrica->lineae->fons, ab, ad), ab, linea,
        columna, BRIAR_FONS_PLAGULAE);
}

MateriaToken*
briar_lexema_finis (
    BriarFabrica* fabrica)
{
    i32 n        = briar_lineae_numerus(fabrica->lineae);
    i32 linea    = I;
    i32 columna  = I;

    si (n > ZEPHYRUM)
    {
        constans BriarLinea* l = briar_linea(fabrica->lineae, n - I);

        si (l->terminator > ZEPHYRUM)
        {
            linea = l->numerus + I;
        }
        alioquin
        {
            linea    = l->numerus;
            columna  = l->mensura + I;
        }
    }
    redde materia_token_creare(fabrica->piscina, &fabrica->forma,
        (s32)BRIAR_LEX_FINIS,
        briar_chorda_fontis(fabrica->lineae->fons,
            (s32)fabrica->lineae->mensura,
            (s32)fabrica->lineae->mensura),
        (s32)fabrica->lineae->mensura, linea, columna,
        BRIAR_FONS_PLAGULAE);
}

MateriaToken*
briar_lexema_derivatum (
             BriarFabrica* fabrica,
                      s32  genus,
                   chorda  valor,
    constans MateriaToken* origo)
{
    redde materia_token_creare(fabrica->piscina, &fabrica->forma, genus,
        valor, (s32)-I, origo->linea, origo->columna,
        BRIAR_FONS_DERIVATUS);
}

Xar*
briar_lexare_crude (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
     BriarLineae  lineae;
    BriarFabrica  fabrica;
    BriarSaeptum  saeptum;
             Xar* xs;
             i32  n;
             i32  i;

    si (piscina == NIHIL || (fons == NIHIL && mensura > ZEPHYRUM))
    {
        redde NIHIL;
    }
    si (!briar_lineae_scindere(piscina, fons, mensura, &lineae))
    {
        redde NIHIL;
    }
    briar_fabrica_incipere(&fabrica, piscina, &lineae);
    xs = xar_creare(piscina, (i32)magnitudo(MateriaToken*));
    si (xs == NIHIL)
    {
        redde NIHIL;
    }
    saeptum.aperta     = FALSUM;
    saeptum.signum     = ' ';
    saeptum.longitudo  = ZEPHYRUM;
    n                  = briar_lineae_numerus(&lineae);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans BriarLinea* l = briar_linea(&lineae, i);
               BriarClassis  c = briar_lineam_classificare(&lineae, i,
                   &saeptum);
                       s32 genus;
             MateriaToken* t;
            MateriaToken** sedes;

        commutatio (c.genus)
        {
            casus BRIAR_LINEA_INTERPRES:
                genus = (s32)BRIAR_LEX_INTERPRES;
                frange;
            casus BRIAR_LINEA_TAG_APERTUM:
                genus = (s32)BRIAR_LEX_TAG_APERTUM;
                frange;
            casus BRIAR_LINEA_TAG_CLAUSUM:
                genus = (s32)BRIAR_LEX_TAG_CLAUSUM;
                frange;
            ordinarius:
                genus = (s32)BRIAR_LEX_PROSA;
                frange;
        }
        t = briar_lexema_fabricare(&fabrica, genus, l->offset,
            l->offset + (s32)l->mensura + (s32)l->terminator);
        sedes = (MateriaToken**)xar_addere(xs);
        si (t == NIHIL || sedes == NIHIL)
        {
            redde NIHIL;
        }
        *sedes = t;
    }
    {
        MateriaToken*  t      = briar_lexema_finis(&fabrica);
        MateriaToken** sedes  = (MateriaToken**)xar_addere(xs);

        si (t == NIHIL || sedes == NIHIL)
        {
            redde NIHIL;
        }
        *sedes = t;
    }
    redde xs;
}
