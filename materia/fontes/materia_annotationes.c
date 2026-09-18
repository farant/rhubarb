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
 * parsari nequit RETINETUR - lex silva_annotationes.h.
 *
 * 'nodus' HIC NON PONITUR: annotatio PENDENS nascitur et lexemate
 * SEQUENTE solvitur (vide _pendentes_solvere). */
interior vacuum
_commentarium_tractare (
                Collectio* c,
    constans MateriaToken* lexema,
    constans MateriaNodus* hospes)
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
    a->textus = purgatum;
    a->crudum = lexema->valor;
    /* NIHIL = pendens, antrorsum solvenda */
    a->nodus           = hospes;
    a->scopus.initium  = -I;
    materia_tractus_lexematis(c->origo, lexema, &a->commentarium);
    r                   = stml_legere(purgatum, c->piscina, c->intern);
    a->parsata          = r.successus;
    a->arbor            = r.elementum_radix;
    a->status           = r.status;
    a->linea_erroris    = r.linea_erroris;
    a->columna_erroris  = r.columna_erroris;
}


/* Annotationes PENDENTES lexemate hoc solvere: quaeque cuius
 * commentarium ANTE hoc lexema finit nodum eius possessorem accipit.
 *
 * ==================================================
 * CUR ADNEXIO ANTRORSUM, ET QUID MENSURA DOCUIT
 * ==================================================
 *
 * Opus I nodum POSSESSOREM TRIVII adhibebat. Crusta autem trivia
 * RETRO ligat (lex C7, crusta_lexicon.h:19), ergo ambitus ab eo quod
 * commentarium PRAECEDIT pendebat, non ab eo quod annotat. Mensuratum
 * 2026-09-18 in transitu operis V:
 *
 *   post 'fi': commentarium in <post> SEPARATORIS sedet (octeti
 *   57-58) - ambitus NIHIL continet, excusatio MORTUA nascitur;
 *
 *   post 'do': possessor est ITERATIO TOTA - una excusatio inventa
 *   DUO tacere fecit, alterum vitium VERUM alienum.
 *
 * Utrumque idem vitium: nimis angustum aut nimis latum, numquam id
 * quod auctor scripsit. Silva hoc ipsum per SUPRA/INTERIOR/PLAGULA
 * solvit; opus I id divergentia deposuit, quia mensura prima
 * ('if' in initio plagulae) casus SPECIALIS erat - trivium ducens
 * nihil habet cui retro ligetur.
 *
 * REMEDIUM: adnexio POSITIONALIS antrorsum. Lexemata ordine fontis
 * visitantur, ergo lexema PRIMUM post commentarium possessorem dat.
 * Trivium ducens eundem nodum reddit ac prius (lexema sequens est
 * lexema ipsum), ergo casus qui operabantur immoti manent. */

interior vacuum
_pendentes_solvere (
                Collectio* c,
    constans MateriaNodus* possessor,
    constans MateriaToken* lexema)
{
    i32 k;

    si (lexema->byte_offset < ZEPHYRUM)
    {
        redde;   /* syntheticum: sedem non dat */
    }
    per (k = ZEPHYRUM; k < xar_numerus(c->exitus); k++)
    {
        MateriaAnnotatio* a =
            (MateriaAnnotatio*)xar_obtinere(c->exitus, k);

        si (   a->nodus              == NIHIL
            && a->commentarium.finis <= lexema->byte_offset)
        {
            a->nodus = possessor;
        }
    }
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
    /* ORDO MOMENTI EST: trivia ANTE hoc lexema praecedunt, ergo hoc
     * lexema ea solvit; trivia POST id sequuntur, ergo lexema
     * PROXIMUM ea solvet. */
    per (k = ZEPHYRUM; k < lexema->numerus_ante; k++)
    {
        _commentarium_tractare(c, lexema->spatia_ante[k], NIHIL);
    }
    _pendentes_solvere(c, possessor, lexema);
    per (k = ZEPHYRUM; k < lexema->numerus_post; k++)
    {
        /* LINEA EADEM AUT PRAECEDENS (lex silvae,
         * silva_c89_semantica.c:1077): commentarium in CAUDA lineae
         * id annotat quod in ea linea iacet, non quod sequitur -
         * ita auctor scribit, et ita silva iam iudicat. Commentarium
         * in linea PROPRIA antrorsum solvitur ut cetera. */
        constans MateriaToken* nota = lexema->spatia_post[k];

        _commentarium_tractare(c, nota,
            nota->linea == lexema->linea ? possessor : NIHIL);
    }
}

/* Ambitus cuiusque annotationis: nodus LATISSIMUS qui eodem octeto
 * incipit ac possessor inventus, RADICE EXCEPTA.
 *
 * Cur latissimus: commentarium ante 'if' totam conditionem tegere
 * debet, non lexema 'if' solum. Cur radice excepta: radix plagulam
 * TOTAM tegit, et exclusio per plagulam est id quod haec ratio
 * consulto vetat (par. II specificationis). */
interior vacuum
_ambitus_ponere (
                Collectio* c,
    constans MateriaNodus* radix)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(c->exitus); k++)
    {
        MateriaAnnotatio* a =
            (MateriaAnnotatio*)xar_obtinere(c->exitus, k);
        constans MateriaNodus* nodus = a->nodus;
               MateriaTractus  t;

        si (   nodus == NIHIL
            || !materia_tractus_nodi(c->origo, nodus, &t))
        {
            perge;
        }
        dum (   nodus->pater != NIHIL
             && nodus->pater != radix)
        {
            MateriaTractus supra;

            si (   !materia_tractus_nodi(c->origo, nodus->pater,
                       &supra)
                || supra.initium != t.initium)
            {
                frange;
            }
            nodus  = nodus->pater;
            t      = supra;
        }
        a->nodus   = nodus;
        a->scopus  = t;
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
    _ambitus_ponere(&c, radix);
    redde c.exitus;
}
