/* materia_diagnostica.c - Vide materia_diagnostica.h.
 *
 * Ambulatio UNA praeordine: nodus ANTE liberos notatur (ergo 'lexema
 * fontis ultimum visum' illud ante nodum est - punctum nodi sine
 * lexemate); loci ordine, listae ordine, REFERENTIA non descensa
 * (scopus, non possessio). Lexema quodque ordine OCTETORUM visitatur
 * (trivia ante, lexema, trivia post) - idem ordo quo scriptor
 * octetos emittit, quem probatio ordinis poscit.
 */

#include "materia_diagnostica.h"
#include "tabula_dispersa.h"
#include <string.h>

/* valor praesentiae in tabula lexematum visorum: sedes sola refert
 * (VERUM in punctatorem versum monitum daret) */
hic_manens i32 PRAESENS = I;

nomen structura {
                             Piscina* piscina;
     constans MateriaRegistrumCoctum* genera;
    constans MateriaDiagnosticaCocta* diagnostica;
          constans MateriaOrigoUncus* uncus;
                                 Xar* exitus;
                      TabulaDispersa* visa;
                                 b32  fons_notus;
                                 s32  fons;
                                 s32  finis_maximus;
                      MateriaTractus  ultimum;
                                 b32  ultimum_notum;
                                 b32  memoria_defecit;
} Derivatio;

interior vacuum
_addere (
                 Derivatio* d,
                       s32  gravitas,
        constans character* codex,
        constans character* causa,
     constans MateriaNodus* nodus,
     constans MateriaToken* lexema,
   constans MateriaTractus* tractus)
{
    MateriaDiagnosticum* r = (MateriaDiagnosticum*)xar_addere(
        d->exitus);

    si (r == NIHIL)
    {
        d->memoria_defecit = VERUM;
        redde;
    }
    r->gravitas  = gravitas;
    r->codex     = codex;
    r->causa     = causa;
    r->nodus     = nodus;
    r->lexema    = lexema;
    r->tractus   = *tractus;
}

/* punctum in fine tractus (latitudo nulla: '}' exspectata HIC) */
interior MateriaTractus
_punctum_finis (
    constans MateriaTractus* t)
{
    MateriaTractus p = *t;

    p.initium  = t->finis;
    p.linea    = t->linea_finis;
    p.columna  = t->columna_finis;
    redde p;
}

/* punctum post lexema fontis ultimum visum; initium plagulae si
 * nullum (nodus ante omnia) */
interior MateriaTractus
_punctum_prius (
    constans Derivatio* d)
{
    MateriaTractus p;

    si (d->ultimum_notum)
    {
        redde _punctum_finis(&d->ultimum);
    }
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.linea          = I;
    p.columna        = I;
    p.linea_finis    = I;
    p.columna_finis  = I;
    p.est_fons       = VERUM;
    redde p;
}

interior b32
_inane (
    constans Derivatio* d,
                   s32  genus)
{
    i32 k;

    per (k = ZEPHYRUM; k < d->diagnostica->numerus_inanium; k++)
    {
        si (d->diagnostica->inania[k] == genus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* lista 'vacua': non scripta, aut elementa nodorum generum inanium
 * sola (separator, heredoc - delta D5) */
interior b32
_vacua (
       constans Derivatio* d,
    constans MateriaValor* valor)
{
    i32 k;

    si (valor->genus == MATERIA_VALOR_NIHIL)
    {
        redde VERUM;
    }
    si (valor->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(
            *valor, k);

        si (e == NIHIL)
        {
            perge;
        }
        si (e->genus == MATERIA_VALOR_TOKEN)
        {
            redde FALSUM;
        }
        si (   e->genus       == MATERIA_VALOR_NODUS
            && e->datum.nodus != NIHIL
            && !_inane(d, e->datum.nodus->genus))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* VERUM si lexema iam visum (usus iteratus: transclusio); aliter
 * notatur. Clavis = octeti punctatoris ipsius. */
interior b32
_visum (
                Derivatio* d,
    constans MateriaToken* lexema)
{
    chorda  clavis;
        i8* octeti;

    octeti = (i8*)piscina_allocare(d->piscina,
        magnitudo(constans MateriaToken*));
    si (octeti == NIHIL)
    {
        d->memoria_defecit = VERUM;
        redde VERUM;
    }
    memcpy(octeti, &lexema, magnitudo(constans MateriaToken*));
    clavis.datum    = octeti;
    clavis.mensura  = (i32)magnitudo(constans MateriaToken*);
    si (tabula_dispersa_continet(d->visa, clavis))
    {
        redde VERUM;
    }
    si (!tabula_dispersa_inserere(d->visa, clavis, &PRAESENS))
    {
        d->memoria_defecit = VERUM;
    }
    redde FALSUM;
}

/* ordo octetorum: lexema fontis ante finem maximum iam visum =
 * octeti transpositi (heredoc crustae olim) */
interior vacuum
_lexema_notare (
                Derivatio* d,
    constans MateriaToken* lexema)
{
    MateriaTractus t;

    si (lexema == NIHIL)
    {
        redde;
    }
    materia_tractus_lexematis(d->uncus, lexema, &t);
    si (t.initium < ZEPHYRUM || !t.est_fons)
    {
        redde;
    }
    si (   d->fons_notus && t.fons_index == d->fons
        && t.initium < d->finis_maximus)
    {
        _addere(d, (s32)MATERIA_GRAVITAS_ERRATUM, MATERIA_CODEX_ORDO,
            "lexema ante finem prioris", NIHIL, lexema, &t);
    }
    si (!d->fons_notus)
    {
        d->fons_notus  = VERUM;
        d->fons        = t.fons_index;
    }
    si (t.fons_index == d->fons && t.finis > d->finis_maximus)
    {
        d->finis_maximus = t.finis;
    }
    d->ultimum        = t;
    d->ultimum_notum  = VERUM;
}

interior vacuum
_lexema_ambulare (
                Derivatio* d,
    constans MateriaToken* lexema)
{
    i32 k;

    si (lexema == NIHIL || _visum(d, lexema))
    {
        redde;
    }
    per (k = ZEPHYRUM; k < lexema->numerus_ante; k++)
    {
        _lexema_notare(d, lexema->spatia_ante[k]);
    }
    _lexema_notare(d, lexema);
    per (k = ZEPHYRUM; k < lexema->numerus_post; k++)
    {
        _lexema_notare(d, lexema->spatia_post[k]);
    }
}

interior vacuum
_valorem_ambulare (
                Derivatio* d,
    constans MateriaValor* valor);

interior vacuum
_nodum_ambulare (
                Derivatio* d,
    constans MateriaNodus* nodus)
{
    MateriaTractus t;
    MateriaTractus punctum;
               b32 habet;
               i32 k;

    si (nodus == NIHIL)
    {
        redde;
    }
    habet    = materia_tractus_nodi(d->uncus, nodus, &t);
    punctum  = _punctum_prius(d);
    per (k = ZEPHYRUM; k < d->diagnostica->numerus; k++)
    {
        constans MateriaTabDiagnosticum* r =
            &d->diagnostica->tabula[k];
               constans MateriaValor* valor;
                      MateriaTractus  finis;

        si (   r->genus != nodus->genus
            || (   r->species != (s32)MATERIA_DIAGNOSTICUM_GENUS
                && (   r->locus < ZEPHYRUM
                    || (i32)r->locus >= nodus->numerus_locorum)))
        {
            perge;
        }
        commutatio (r->species)
        {
        casus MATERIA_DIAGNOSTICUM_GENUS:
            _addere(d, r->gravitas, r->codex, r->causa, nodus, NIHIL,
                habet ? &t : &punctum);
            frange;
        casus MATERIA_DIAGNOSTICUM_ABSENTIA:
            valor = &nodus->loci[r->locus];
            si (   valor->genus == MATERIA_VALOR_NIHIL
                || (   valor->genus == MATERIA_VALOR_REFERENTIA
                    && valor->datum.nodus == NIHIL))
            {
                finis = habet ? _punctum_finis(&t) : punctum;
                _addere(d, r->gravitas, r->codex, r->causa, nodus,
                    NIHIL, &finis);
            }
            frange;
        casus MATERIA_DIAGNOSTICUM_VACUA:
            si (_vacua(d, &nodus->loci[r->locus]))
            {
                _addere(d, r->gravitas, r->codex, r->causa, nodus,
                    NIHIL, habet ? &t : &punctum);
            }
            frange;
        ordinarius:
            frange;
        }
    }
    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        _valorem_ambulare(d, &nodus->loci[k]);
    }
}

interior vacuum
_valorem_ambulare (
                Derivatio* d,
    constans MateriaValor* valor)
{
    i32 k;

    commutatio (valor->genus)
    {
    casus MATERIA_VALOR_TOKEN:
        _lexema_ambulare(d, valor->datum.token);
        frange;
    casus MATERIA_VALOR_NODUS:
        _nodum_ambulare(d, valor->datum.nodus);
        frange;
    casus MATERIA_VALOR_LISTA:
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor);
             k++)
        {
            constans MateriaValor* e = materia_valor_lista_obtinere(
                *valor, k);

            si (e != NIHIL)
            {
                _valorem_ambulare(d, e);
            }
        }
        frange;
    ordinarius:
        frange;
    }
}

/* ordo: initium, deinde codex; insertio stabilis (ordo inventionis
 * paribus servatur) */
interior b32
_prius (
    constans MateriaDiagnosticum* a,
    constans MateriaDiagnosticum* b)
{
    si (a->tractus.initium != b->tractus.initium)
    {
        redde (b32)(a->tractus.initium < b->tractus.initium);
    }
    redde (b32)(strcmp(a->codex, b->codex) < ZEPHYRUM);
}

Xar*
materia_diagnostica_derivare (
                            Piscina* piscina,
              constans MateriaNodus* radix,
    constans MateriaRegistrumCoctum* genera,
   constans MateriaDiagnosticaCocta* diagnostica,
         constans MateriaOrigoUncus* uncus,
                                Xar* emissa)
{
               Derivatio  d;
     MateriaDiagnosticum* series;
                     Xar* ordinata;
                     i32  numerus;
                     i32  k;

    si (piscina == NIHIL || genera == NIHIL || diagnostica == NIHIL)
    {
        redde NIHIL;
    }
    memset(&d, ZEPHYRUM, magnitudo(d));
    d.piscina      = piscina;
    d.genera       = genera;
    d.diagnostica  = diagnostica;
    d.uncus        = uncus;
    d.exitus       = xar_creare(piscina,
        (i32)magnitudo(MateriaDiagnosticum));
    d.visa         = tabula_dispersa_creare_chorda(piscina, (i32)MXXIV);
    si (d.exitus == NIHIL || d.visa == NIHIL)
    {
        redde NIHIL;
    }
    _nodum_ambulare(&d, radix);
    per (k = ZEPHYRUM; emissa != NIHIL && k < xar_numerus(emissa); k++)
    {
        constans MateriaDiagnosticum* e =
            (constans MateriaDiagnosticum*)xar_obtinere(emissa, k);
                       MateriaTractus t;

        si (e == NIHIL)
        {
            perge;
        }
        t = e->tractus;
        si (t.initium < ZEPHYRUM && e->nodus != NIHIL)
        {
            (vacuum)materia_tractus_nodi(uncus, e->nodus, &t);
        }
        _addere(&d, e->gravitas, e->codex, e->causa, e->nodus,
            e->lexema, &t);
    }
    si (d.memoria_defecit)
    {
        redde NIHIL;
    }
    numerus  = xar_numerus(d.exitus);
    series   = (MateriaDiagnosticum*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(MateriaDiagnosticum)
            * (memoriae_index)(numerus + I));
    ordinata = xar_creare(piscina,
        (i32)magnitudo(MateriaDiagnosticum));
    si (series == NIHIL || ordinata == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        MateriaDiagnosticum clavis =
            *(MateriaDiagnosticum*)xar_obtinere(d.exitus, k);
        s32 j = (s32)k - (s32)I;

        dum (j >= ZEPHYRUM && _prius(&clavis, &series[j]))
        {
            series[j + I] = series[j];
            j--;
        }
        series[j + I] = clavis;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        MateriaDiagnosticum* cella = (MateriaDiagnosticum*)xar_addere(
            ordinata);

        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = series[k];
    }
    redde ordinata;
}
