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

/* <intentio nid="01KY3TRZ2GY3HG9Y4WQRSGR2BX" res="01KY3D7EJP">
 * <causa>commenta sunt contentum cum structura - consumptor primus
 * pinnae; radix strati identitatis et intentionis</causa>
 * </intentio> */
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
    /* lineae directivae consumptae: commentarium supra #include
     * in lexemate directivae equitat quod fluxum expansum numquam
     * intrat (annotatio capitis plagulae supra clausuram
     * includendi - collocatio naturalissima). Duplicata innocua;
     * ordo annotationum harum post fluxum, ante caudam. */
    si (parsura->directivae != NIHIL)
    {
        per (i = ZEPHYRUM; i < xar_numerus(parsura->directivae);
             i++)
        {
            Xar* linea_directivae = *(Xar**)xar_obtinere(
                parsura->directivae, i);
            i32 j;

            per (j = ZEPHYRUM;
                 linea_directivae != NIHIL
                     && j < xar_numerus(linea_directivae);
                 j++)
            {
                SilvaToken* tok = *(SilvaToken**)xar_obtinere(
                    linea_directivae, j);
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

/* ==================================================
 * Identitates (frustum B): lectio arboris + locatio textus
 * ================================================== */

interior b32
_verbum_est (constans chorda* c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde c != NIHIL && c->datum != NIHIL
        && (memoriae_index)c->mensura == m
        && memcmp(c->datum, litterae, m) == ZEPHYRUM;
}

interior constans StmlAttributum*
_attributum_invenire (constans StmlNodus* nodus,
    constans character* titulus)
{
    i32 k;

    si (nodus->attributa == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(nodus->attributa); k++)
    {
        constans StmlAttributum* a = (constans StmlAttributum*)
            xar_obtinere(nodus->attributa, k);

        si (a != NIHIL && _verbum_est(a->titulus, titulus))
        {
            redde a;
        }
    }
    redde NIHIL;
}

interior vacuum
_identitatem_addere (Xar* fructus, StmlNodus* elementum,
    constans chorda* valor, b32 petitio, SilvaInsertioGenus genus)
{
    SilvaIdentitas* id = (SilvaIdentitas*)xar_addere(fructus);

    si (id == NIHIL)
    {
        redde;
    }
    id->elementum = elementum;
    si (valor != NIHIL)
    {
        id->valor = *valor;
    }
    alioquin
    {
        id->valor.mensura = ZEPHYRUM;
        id->valor.datum = NIHIL;
    }
    id->petitio = petitio;
    id->insertio_genus = genus;
    id->insertio_offset = -I;
}

/* ordo documenti: elementum ipsum, deinde liberi. Attributum
 * booleanum valorem internatum "true" fert - valor "nid" literalis
 * "true" impossibilis (ULID), ergo "true" = petitio. */
interior vacuum
_identitates_ex_arbore (Xar* fructus, StmlNodus* nodus)
{
    i32 k;

    si (nodus == NIHIL)
    {
        redde;
    }
    si ((s32)nodus->genus == STML_NODUS_ELEMENTUM)
    {
        si (_verbum_est(nodus->titulus, "nid"))
        {
            constans StmlAttributum* v = _attributum_invenire(nodus,
                "v");

            si (v == NIHIL)
            {
                _identitatem_addere(fructus, nodus, NIHIL, VERUM,
                    SILVA_INSERTIO_POST_TITULUM);
            }
            alioquin si (_verbum_est(v->valor, "true"))
            {
                _identitatem_addere(fructus, nodus, NIHIL, VERUM,
                    SILVA_INSERTIO_POST_ATTRIBUTUM);
            }
            alioquin
            {
                _identitatem_addere(fructus, nodus, v->valor,
                    FALSUM, SILVA_INSERTIO_NIHIL);
            }
        }
        alioquin
        {
            constans StmlAttributum* n = _attributum_invenire(nodus,
                "nid");

            si (n != NIHIL)
            {
                si (_verbum_est(n->valor, "true"))
                {
                    _identitatem_addere(fructus, nodus, NIHIL,
                        VERUM, SILVA_INSERTIO_POST_ATTRIBUTUM);
                }
                alioquin
                {
                    _identitatem_addere(fructus, nodus, n->valor,
                        FALSUM, SILVA_INSERTIO_NIHIL);
                }
            }
        }
    }
    si (nodus->liberi != NIHIL)
    {
        per (k = ZEPHYRUM; k < xar_numerus(nodus->liberi); k++)
        {
            _identitates_ex_arbore(fructus,
                *(StmlNodus**)xar_obtinere(nodus->liberi, k));
        }
    }
}

/* ==================================================
 * Locatio textualis sedium mintationis (octeti crudi - offsets
 * in fonte veri; textus purgatus offsets fontis non servat).
 * Scansio intra-tag sola: verba extra tags (prosa intentionis)
 * numquam congruunt; valores quotati saltantur; decoratio post
 * lineam novam intra tag exuitur; tags claudentes numquam
 * mintantur. CAVE: contentum crudum (<tag!>) tags fictos ferre
 * potest - dissensus numerorum tunc offsets -1 relinquit et
 * instrumentum recusat (clamor, non silentium).
 * ================================================== */

nomen structura {
    s32                offset;
    SilvaInsertioGenus genus;
} SedesMintationis;

interior b32
_verbi_initium (i8 c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || c == '_';
}

interior b32
_verbi_pars (i8 c)
{
    redde _verbi_initium(c) || (c >= '0' && c <= '9') || c == '-';
}

interior vacuum
_sedem_addere (Xar* sedes, s32 offset, SilvaInsertioGenus genus)
{
    SedesMintationis* s = (SedesMintationis*)xar_addere(sedes);

    si (s != NIHIL)
    {
        s->offset = offset;
        s->genus = genus;
    }
}

interior Xar*
_sedes_mintationis (Piscina* piscina, constans SilvaAnnotatio* a)
{
    constans chorda* v = &a->crudum;
    s32  genus_commentarii;
    i32  fine;
    i32  i = II;
    b32  in_tag = FALSUM;
    b32  in_claudens = FALSUM;
    b32  est_nid = FALSUM;
    b32  vidit_v = FALSUM;
    b32  titulus_lectus = FALSUM;
    s32  titulus_finis = -I;
    Xar* sedes = xar_creare(piscina,
        (i32)magnitudo(SedesMintationis));

    si (sedes == NIHIL || v->datum == NIHIL || v->mensura < II)
    {
        redde sedes;
    }
    genus_commentarii = (v->datum[I] == '*')
        ? SILVA_LEX_COMMENTUM_CLAUSUM : SILVA_LEX_COMMENTUM_LINEA;
    fine = _finis_contenti(v, genus_commentarii);

    dum (i < fine)
    {
        i8 c = v->datum[i];

        si (!in_tag)
        {
            si (c == '<')
            {
                in_tag = VERUM;
                in_claudens = FALSUM;
                est_nid = FALSUM;
                vidit_v = FALSUM;
                titulus_lectus = FALSUM;
                titulus_finis = -I;
                i++;
                si (i < fine && v->datum[i] == '/')
                {
                    in_claudens = VERUM;
                    i++;
                }
            }
            alioquin
            {
                i++;
            }
            perge;
        }
        si (c == '"')
        {
            i++;
            dum (i < fine && v->datum[i] != '"')
            {
                i++;
            }
            si (i < fine)
            {
                i++;
            }
            perge;
        }
        si (c == '>')
        {
            si (est_nid && !in_claudens && !vidit_v
                && titulus_finis >= ZEPHYRUM)
            {
                _sedem_addere(sedes,
                    a->byte_offset + titulus_finis,
                    SILVA_INSERTIO_POST_TITULUM);
            }
            in_tag = FALSUM;
            i++;
            perge;
        }
        si (c == '\n')
        {
            i++;
            dum (i < fine && (v->datum[i] == ' '
                || v->datum[i] == '\t'))
            {
                i++;
            }
            si (i < fine && v->datum[i] == '*')
            {
                i++;
                si (i < fine && v->datum[i] == ' ')
                {
                    i++;
                }
            }
            perge;
        }
        si (_verbi_initium(c))
        {
            i32 initium = i;

            dum (i < fine && _verbi_pars(v->datum[i]))
            {
                i++;
            }
            si (!titulus_lectus)
            {
                titulus_lectus = VERUM;
                titulus_finis = (s32)i;
                est_nid = (i - initium == III)
                    && v->datum[initium] == 'n'
                    && v->datum[initium + I] == 'i'
                    && v->datum[initium + II] == 'd';
                perge;
            }
            /* attributum: valutumne ('=' post spatia/decorationem)? */
            {
                i32 j = i;
                b32 valutum;
                b32 est_v = (i - initium == I)
                    && v->datum[initium] == 'v';
                b32 est_attr_nid = (i - initium == III)
                    && v->datum[initium] == 'n'
                    && v->datum[initium + I] == 'i'
                    && v->datum[initium + II] == 'd';

                dum (j < fine)
                {
                    i8 d = v->datum[j];

                    si (d == ' ' || d == '\t')
                    {
                        j++;
                        perge;
                    }
                    si (d == '\n')
                    {
                        j++;
                        dum (j < fine && (v->datum[j] == ' '
                            || v->datum[j] == '\t'))
                        {
                            j++;
                        }
                        si (j < fine && v->datum[j] == '*')
                        {
                            j++;
                            si (j < fine && v->datum[j] == ' ')
                            {
                                j++;
                            }
                        }
                        perge;
                    }
                    frange;
                }
                valutum = (j < fine && v->datum[j] == '=');
                si (!valutum)
                {
                    si (est_v && est_nid && !in_claudens)
                    {
                        vidit_v = VERUM;
                        _sedem_addere(sedes,
                            a->byte_offset + (s32)i,
                            SILVA_INSERTIO_POST_ATTRIBUTUM);
                    }
                    alioquin si (est_attr_nid && !est_nid
                        && !in_claudens)
                    {
                        _sedem_addere(sedes,
                            a->byte_offset + (s32)i,
                            SILVA_INSERTIO_POST_ATTRIBUTUM);
                    }
                }
                alioquin si (est_v && est_nid)
                {
                    vidit_v = VERUM;
                }
            }
            perge;
        }
        i++;
    }
    redde sedes;
}

Xar*
silva_annotationes_identitates (Piscina* piscina,
    constans SilvaAnnotatio* annotatio)
{
    Xar* fructus;
    Xar* sedes;
    i32  petitiones_arboris = ZEPHYRUM;
    i32  k;
    i32  j;

    si (piscina == NIHIL || annotatio == NIHIL
        || !annotatio->parsata || annotatio->documentum == NIHIL)
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(SilvaIdentitas));
    si (fructus == NIHIL)
    {
        redde NIHIL;
    }
    _identitates_ex_arbore(fructus, annotatio->documentum);
    per (k = ZEPHYRUM; k < xar_numerus(fructus); k++)
    {
        constans SilvaIdentitas* id = (constans SilvaIdentitas*)
            xar_obtinere(fructus, k);

        si (id->petitio)
        {
            petitiones_arboris++;
        }
    }
    si (petitiones_arboris == ZEPHYRUM)
    {
        redde fructus;
    }
    sedes = _sedes_mintationis(piscina, annotatio);
    si (sedes == NIHIL
        || xar_numerus(sedes) != petitiones_arboris)
    {
        redde fructus;   /* dissensus - offsets -1 manent */
    }
    j = ZEPHYRUM;
    per (k = ZEPHYRUM; k < xar_numerus(fructus); k++)
    {
        SilvaIdentitas* id = (SilvaIdentitas*)xar_obtinere(fructus,
            k);

        si (id->petitio)
        {
            constans SedesMintationis* s =
                (constans SedesMintationis*)xar_obtinere(sedes, j);

            j++;
            si (s != NIHIL && s->genus == id->insertio_genus)
            {
                id->insertio_offset = s->offset;
            }
        }
    }
    redde fructus;
}
