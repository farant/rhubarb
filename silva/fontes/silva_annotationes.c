/* silva_annotationes.c - Collector annotationum STML
 * (parcum 01KY3D7EJP, frustum A; contractus in capite) */

#include "silva_annotationes.h"
#include <string.h>

/* ==================================================
 * Ancoratio + purgatio
 * ================================================== */

/* finis contenti: delimitator claudens exutus (commentaria clausa
 * non terminata ad EOF delimitatorem carere possunt - tunc mensura
 * tota manet) */
interior i32
_finis_contenti (constans chorda* valor, s32 genus)
{
    i32 fine = valor->mensura;

    si (genus == SILVA_LEX_COMMENTUM_CLAUSUM
        && fine >= IV
        && valor->datum[fine - II] == '*'
        && valor->datum[fine - I] == '/')
    {
        fine -= II;
    }
    redde fine;
}

/* index contenti primi post delimitatorem apertum, spatia, et
 * decorationem linearum continuationis; -1 = commentarium vacuum */
interior s32
_contentum_primum (constans chorda* valor, i32 fine)
{
    i32 i = II;   /* post delimitatorem apertum (ambo bini) */

    dum (i < fine)
    {
        si (valor->datum[i] == ' ' || valor->datum[i] == '\t')
        {
            i++;
            perge;
        }
        si (valor->datum[i] == '\n')
        {
            i++;
            dum (i < fine && (valor->datum[i] == ' '
                || valor->datum[i] == '\t'))
            {
                i++;
            }
            si (i < fine && valor->datum[i] == '*')
            {
                i++;
                si (i < fine && valor->datum[i] == ' ')
                {
                    i++;
                }
            }
            perge;
        }
        redde (s32)i;
    }
    redde -I;
}

/* ancoratum = contentum primum '<' littera sequente (prosa numquam
 * fere tag incipit; "< 5" spatio non ancoratur) */
interior b32
_est_ancoratum (constans chorda* valor, s32 genus)
{
    i32 fine = _finis_contenti(valor, genus);
    s32 primus = _contentum_primum(valor, fine);
    i8 sequens;

    si (primus < ZEPHYRUM || (i32)primus + I >= fine)
    {
        redde FALSUM;
    }
    si (valor->datum[primus] != '<')
    {
        redde FALSUM;
    }
    sequens = valor->datum[primus + I];
    redde (sequens >= 'a' && sequens <= 'z')
        || (sequens >= 'A' && sequens <= 'Z');
}

/* praetransitus decorationis (spec par 2.2): linea prima intacta;
 * post quamque lineam novam spatia ducentia + asteriscus unus +
 * spatium unum optionale exuuntur. Lineae novae servantur - linea
 * erroris stml in lineam fontis remappari potest. */
interior chorda
_purgare (Piscina* piscina, constans chorda* valor, i32 fine)
{
    chorda purgatum;
    i8*    d;
    i32    i;
    i32    n = ZEPHYRUM;

    purgatum.mensura = ZEPHYRUM;
    purgatum.datum = NIHIL;
    si (fine <= II)
    {
        redde purgatum;
    }
    d = (i8*)piscina_allocare(piscina, (memoriae_index)(fine - II));
    si (d == NIHIL)
    {
        redde purgatum;
    }
    i = II;
    dum (i < fine)
    {
        si (valor->datum[i] == '\n')
        {
            d[n] = '\n';
            n++;
            i++;
            dum (i < fine && (valor->datum[i] == ' '
                || valor->datum[i] == '\t'))
            {
                i++;
            }
            si (i < fine && valor->datum[i] == '*')
            {
                i++;
                si (i < fine && valor->datum[i] == ' ')
                {
                    i++;
                }
            }
        }
        alioquin
        {
            d[n] = valor->datum[i];
            n++;
            i++;
        }
    }
    purgatum.datum = d;
    purgatum.mensura = n;
    redde purgatum;
}

/* ==================================================
 * Collectio
 * ================================================== */

/* annotationem unam addere: duplicata per (fons, byte_offset)
 * omissa (radix invocationis pluribus lexematibus expansis
 * COMMUNIS est - exemplar TOLERA) */
interior vacuum
_annotationem_addere (Xar* fructus, Piscina* piscina,
    InternamentumChorda* intern, constans SilvaToken* tr)
{
    i32             k;
    i32             fine;
    SilvaAnnotatio* a;
    StmlResultus    resultus;

    per (k = ZEPHYRUM; k < xar_numerus(fructus); k++)
    {
        constans SilvaAnnotatio* d = (constans SilvaAnnotatio*)
            xar_obtinere(fructus, k);

        si (d->fons_index == tr->fons_index
            && d->byte_offset == tr->byte_offset)
        {
            redde;
        }
    }
    fine = _finis_contenti(&tr->valor, (s32)tr->genus);
    a = (SilvaAnnotatio*)xar_addere(fructus);
    si (a == NIHIL)
    {
        redde;
    }
    a->crudum = tr->valor;
    a->fons_index = tr->fons_index;
    a->linea = tr->linea;
    a->columna = tr->columna;
    a->byte_offset = tr->byte_offset;
    a->modus = SILVA_ANNOTATIO_PLAGULA;
    a->unitas = NIHIL;
    a->textus = _purgare(piscina, &tr->valor, fine);

    resultus = stml_legere(a->textus, piscina, intern);
    a->parsata = resultus.successus;
    a->documentum = resultus.radix;
    a->arbor = resultus.elementum_radix;
    a->status = resultus.status;
    a->linea_erroris = resultus.linea_erroris;
    a->columna_erroris = resultus.columna_erroris;
    a->error = resultus.error;
}

/* trivia lexematis unius (ambo latera): commenta ancorata sola */
interior vacuum
_ex_lexemate (Xar* fructus, Piscina* piscina,
    InternamentumChorda* intern, constans SilvaToken* tok)
{
    i32 latus;

    per (latus = ZEPHYRUM; latus < II; latus++)
    {
        Xar* trivia = (latus == ZEPHYRUM) ? tok->spatia_ante
                                          : tok->spatia_post;
        i32 j;

        si (trivia == NIHIL)
        {
            perge;
        }
        per (j = ZEPHYRUM; j < xar_numerus(trivia); j++)
        {
            constans SilvaToken* tr = *(SilvaToken**)xar_obtinere(
                trivia, j);

            si (tr == NIHIL
                || ((s32)tr->genus != SILVA_LEX_COMMENTUM_CLAUSUM
                    && (s32)tr->genus != SILVA_LEX_COMMENTUM_LINEA))
            {
                perge;
            }
            si (tr->byte_offset < ZEPHYRUM)
            {
                perge;   /* synthetica - in fonte non exsistunt */
            }
            si (!_est_ancoratum(&tr->valor, (s32)tr->genus))
            {
                perge;
            }
            _annotationem_addere(fructus, piscina, intern, tr);
        }
    }
}

/* ==================================================
 * Affixio (spec par 2.3, octetis per fontem)
 * ================================================== */

Xar*
silva_annotationes_unitates (Piscina* piscina,
    constans SilvaParsura* parsura)
{
    SilvaValor radix;

    si (piscina == NIHIL || parsura == NIHIL
        || parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    radix = parsura->commissio->radix;
    si (radix.genus == SILVA_VALOR_LISTA)
    {
        Xar* unitates = xar_creare(piscina,
            (i32)magnitudo(SilvaNodus*));
        i32  n = silva_valor_lista_numerus(radix);
        i32  i;

        si (unitates == NIHIL)
        {
            redde NIHIL;
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(radix, i);

            si (v != NIHIL && v->genus == SILVA_VALOR_NODUS
                && v->datum.nodus != NIHIL)
            {
                SilvaNodus** locus = (SilvaNodus**)xar_addere(
                    unitates);

                si (locus != NIHIL)
                {
                    *locus = v->datum.nodus;
                }
            }
        }
        redde unitates;
    }
    si (radix.genus == SILVA_VALOR_NODUS
        && radix.datum.nodus != NIHIL)
    {
        redde silva_nodus_liberi(piscina, radix.datum.nodus);
    }
    redde NIHIL;
}

interior vacuum
_affigere (Piscina* piscina, constans SilvaParsura* parsura,
    Xar* fructus)
{
    Xar* unitates;
    i32  i;

    si (xar_numerus(fructus) == ZEPHYRUM)
    {
        redde;
    }
    unitates = silva_annotationes_unitates(piscina, parsura);
    per (i = ZEPHYRUM; i < xar_numerus(fructus); i++)
    {
        SilvaAnnotatio*      a = (SilvaAnnotatio*)xar_obtinere(
            fructus, i);
        s32                  optimum = -I;
        constans SilvaNodus* optima = NIHIL;
        i32                  u;

        a->modus = SILVA_ANNOTATIO_PLAGULA;
        a->unitas = NIHIL;
        si (unitates == NIHIL)
        {
            perge;
        }
        per (u = ZEPHYRUM; u < xar_numerus(unitates); u++)
        {
            constans SilvaNodus* unitas = *(SilvaNodus**)
                xar_obtinere(unitates, u);
            s32 minimum;
            s32 maximum;

            si (unitas == NIHIL)
            {
                perge;
            }
            /* CONTRACTUS extensionis: VOCATOR initiat (accumulator
             * est - minimum < 0, maximum 0; manet < 0 si nihil) */
            minimum = -I;
            maximum = ZEPHYRUM;
            silva_nodus_extensionem(unitas, a->fons_index,
                &minimum, &maximum);
            si (minimum < ZEPHYRUM)
            {
                perge;   /* unitas in hoc fonte absens */
            }
            si (minimum <= a->byte_offset
                && a->byte_offset <= maximum)
            {
                a->modus = SILVA_ANNOTATIO_INTERIOR;
                a->unitas = unitas;
                frange;
            }
            si (minimum > a->byte_offset
                && (optimum < ZEPHYRUM || minimum < optimum))
            {
                optimum = minimum;
                optima = unitas;
            }
        }
        si (a->modus == SILVA_ANNOTATIO_PLAGULA && optima != NIHIL)
        {
            a->modus = SILVA_ANNOTATIO_SUPRA;
            a->unitas = optima;
        }
    }
}

/* ==================================================
 * API
 * ================================================== */

Xar*
silva_annotationes_colligere (Piscina* piscina,
    constans SilvaParsura* parsura, InternamentumChorda* intern)
{
    Xar* fructus;
    i32  i;

    si (piscina == NIHIL || parsura == NIHIL)
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(SilvaAnnotatio));
    si (fructus == NIHIL)
    {
        redde NIHIL;
    }
    si (parsura->lexemata == NIHIL)
    {
        redde fructus;
    }
    si (intern == NIHIL)
    {
        intern = internamentum_creare(piscina);
        si (intern == NIHIL)
        {
            redde NIHIL;
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(parsura->lexemata); i++)
    {
        SilvaToken* tok = *(SilvaToken**)xar_obtinere(
            parsura->lexemata, i);
        SilvaToken* radix;

        si (tok == NIHIL)
        {
            perge;
        }
        _ex_lexemate(fructus, piscina, intern, tok);
        radix = silva_token_radix(tok);
        si (radix != NIHIL && radix != tok)
        {
            _ex_lexemate(fructus, piscina, intern, radix);
        }
    }
    /* trivia caudae: commentarium post lexema ultimum in EOF
     * equitat (casus scopi plagulae) - si EOF extra lexemata est,
     * hic attingitur; duplicata innocua */
    si (parsura->lexema_finis != NIHIL)
    {
        _ex_lexemate(fructus, piscina, intern, parsura->lexema_finis);
    }
    _affigere(piscina, parsura, fructus);
    redde fructus;
}
