/* materia_annotationes.c - Collector annotationum STML in commentariis
 *
 * Descensus acervo explicito (ut materia_diagnostica.c: recursio in
 * arbore ab input arbitrario nata periculum est). Acervus possessorem
 * cuiusque valoris fert - id est quod hoc collectorem a
 * materia_diagnostica distinguit, cui cumulus unus globalis sufficit.
 *
 * 'scopus' per materia_tractus_nodi computatur, per annotationem
 * SEMEL. Quadraticum in arbore esset si in quoque nodo fieret; in
 * quaque annotatione factum, pretium numero ANNOTATIONUM ligatur,
 * qui paucus est (plagula domus mediana: nulla).
 */

#include "materia_annotationes.h"
#include <string.h>


/* ==================================================
 * Status collectionis
 * ================================================== */

nomen structura {
                         Piscina* piscina;
                             Xar* exitus;
    constans MateriaLexiconRatum* lexicon;
              constans character* praefixum;
                             i32  praefixum_mensura;
      constans MateriaOrigoUncus* origo;
             InternamentumChorda* intern;
                             b32  memoria_defecit;
} Collectio;

/* opus acervi: valor ambulandus cum nodo qui eum possidet */
nomen structura {
    constans MateriaValor* valor;
    constans MateriaNodus* possessor;
} Opus;


/* ==================================================
 * Purgatio decorationis
 * ================================================== */

interior b32
_spatium_est (
    i8 c)
{
    redde (b32)(c == (i8)' ' || c == (i8)'\t');
}

/* spatia ducentia, deinde praefixum si adest, deinde spatia iterum.
 * Chorda VISUS est, ergo nihil copiatur. */
interior chorda
_purgare (
                 chorda  valor,
     constans character* praefixum,
                    i32  praefixum_mensura)
{
    chorda r = valor;
       i32 k = ZEPHYRUM;

    dum (k < r.mensura && _spatium_est(r.datum[k]))
    {
        k++;
    }
    si (   praefixum_mensura > ZEPHYRUM
        && r.mensura - k >= praefixum_mensura
        && memcmp(r.datum + k, praefixum,
               (memoriae_index)praefixum_mensura) == ZEPHYRUM)
    {
        k += praefixum_mensura;
    }
    dum (k < r.mensura && _spatium_est(r.datum[k]))
    {
        k++;
    }
    r.datum    += k;
    r.mensura  -= k;
    redde r;
}


/* ==================================================
 * Commentarium unum
 * ================================================== */

/* Commentarium ad annotationem vertere, si annotatio est. PROSA
 * (primum non-spatium non '<') omittitur TACITE; quod '<' fert sed
 * parsari nequit RETINETUR - lex silva_annotationes.h. */
interior vacuum
_commentarium_tractare (
                Collectio* c,
    constans MateriaNodus* possessor,
    constans MateriaToken* lexema)
{
              chorda  purgatum;
    MateriaAnnotatio* a;
        StmlResultus  r;

    si (   materia_lexicon_munus(c->lexicon, lexema->genus)
        != MATERIA_MUNUS_COMMENTUM)
    {
        redde;
    }
    purgatum = _purgare(lexema->valor, c->praefixum,
        c->praefixum_mensura);
    si (   purgatum.mensura == ZEPHYRUM || purgatum.datum[ZEPHYRUM]
            != (i8)'<')
    {
        redde;
    }
    a = (MateriaAnnotatio*)xar_addere(c->exitus);
    si (a == NIHIL)
    {
        c->memoria_defecit = VERUM;
        redde;
    }
    memset(a, ZEPHYRUM, magnitudo(*a));
    a->textus  = purgatum;
    a->crudum  = lexema->valor;
    a->nodus   = possessor;
    materia_tractus_lexematis(c->origo, lexema, &a->commentarium);
    si (!materia_tractus_nodi(c->origo, possessor, &a->scopus))
    {
        /* nodus sine lexemate cum sede: ambitus nullus, ergo nihil
         * excusare potest. Retinetur ut consumptor id NOMINARE
         * possit - tacite cadere est quod hic arcus prohibet. */
        a->scopus.initium = -I;
    }
    r                   = stml_legere(purgatum, c->piscina, c->intern);
    a->parsata          = r.successus;
    a->arbor            = r.elementum_radix;
    a->status           = r.status;
    a->linea_erroris    = r.linea_erroris;
    a->columna_erroris  = r.columna_erroris;
}

interior vacuum
_lexema_tractare (
                Collectio* c,
    constans MateriaNodus* possessor,
    constans MateriaToken* lexema)
{
    i32 k;

    si (lexema == NIHIL || possessor == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < lexema->numerus_ante; k++)
    {
        _commentarium_tractare(c, possessor, lexema->spatia_ante[k]);
    }
    per (k = ZEPHYRUM; k < lexema->numerus_post; k++)
    {
        _commentarium_tractare(c, possessor, lexema->spatia_post[k]);
    }
}


/* ==================================================
 * Descensus
 * ================================================== */

interior b32
_premere (
             Collectio* c,
                   Xar* acervus,
 constans MateriaValor* valor,
 constans MateriaNodus* possessor)
{
    Opus* cella = (Opus*)xar_addere(acervus);

    si (cella == NIHIL)
    {
        c->memoria_defecit = VERUM;
        redde FALSUM;
    }
    cella->valor      = valor;
    cella->possessor  = possessor;
    redde VERUM;
}

/* loci nodi ordine INVERSO (summus primus eruitur, ergo ordo fontis
 * servatur) */
interior b32
_locos_premere (
            Collectio* c,
                  Xar* acervus,
constans MateriaNodus* nodus)
{
    i32 k;

    per (k = nodus->numerus_locorum; k > ZEPHYRUM; k--)
    {
        si (!_premere(c, acervus, &nodus->loci[k - I], nodus))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

Xar*
materia_annotationes_colligere (
                       Piscina* piscina,
         constans MateriaNodus* radix,
  constans MateriaLexiconRatum* lexicon,
            constans character* praefixum,
    constans MateriaOrigoUncus* origo,
           InternamentumChorda* intern)
{
    Collectio  c;
          Xar* acervus;

    si (piscina == NIHIL || radix == NIHIL || lexicon == NIHIL)
    {
        redde NIHIL;
    }
    memset(&c, ZEPHYRUM, magnitudo(c));
    c.piscina    = piscina;
    c.lexicon    = lexicon;
    c.praefixum  = praefixum;
    c.origo      = origo;
    c.intern     = intern;
    c.praefixum_mensura = praefixum == NIHIL
        ? ZEPHYRUM : (i32)strlen(praefixum);
    /* SINE INTERNAMENTO titulus elementi NIHIL redit et consumptor
     * tag nominare non potest - parsatio 'successus' manet, ergo
     * casus TACITE fallit. Ut silva_annotationes.c:412. */
    si (c.intern == NIHIL)
    {
        c.intern = internamentum_creare(piscina);
        si (c.intern == NIHIL)
        {
            redde NIHIL;
        }
    }
    c.exitus  = xar_creare(piscina,
        (i32)magnitudo(MateriaAnnotatio));
    acervus   = xar_creare(piscina, (i32)magnitudo(Opus));
    si (c.exitus == NIHIL || acervus == NIHIL)
    {
        redde NIHIL;
    }
    si (!_locos_premere(&c, acervus, radix))
    {
        redde NIHIL;
    }
    dum (xar_numerus(acervus) > ZEPHYRUM)
    {
        Opus opus;
         i32 ultimus = xar_numerus(acervus) - I;
         i32 k;

        opus = *(Opus*)xar_obtinere(acervus, ultimus);
        xar_truncare(acervus, ultimus);
        commutatio (opus.valor->genus)
        {
        casus MATERIA_VALOR_TOKEN:
            _lexema_tractare(&c, opus.possessor,
                opus.valor->datum.token);
            frange;
        casus MATERIA_VALOR_NODUS:
            si (   opus.valor->datum.nodus != NIHIL
                && !_locos_premere(&c, acervus,
                       opus.valor->datum.nodus))
            {
                redde NIHIL;
            }
            frange;
        casus MATERIA_VALOR_LISTA:
            per (k = materia_valor_lista_numerus(*opus.valor);
                 k > ZEPHYRUM; k--)
            {
                constans MateriaValor* elementum =
                    materia_valor_lista_obtinere(*opus.valor, k - I);

                si (   elementum != NIHIL
                    && !_premere(&c, acervus, elementum,
                           opus.possessor))
                {
                    redde NIHIL;
                }
            }
            frange;
        ordinarius:
            /* REFERENTIA numquam descensa: nodus ALIUS eiusdem
             * arboris, ergo descensus circuitum faceret. INDEX et
             * NIHIL nihil ferunt. */
            frange;
        }
    }
    si (c.memoria_defecit)
    {
        redde NIHIL;
    }
    redde c.exitus;
}
