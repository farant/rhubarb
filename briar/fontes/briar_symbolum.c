/* briar_symbolum.c - Definitio symboli ex corpore (spec par. 4.7).
 *
 * Nihil hic fenestram novit. Corpus intrat, textus exit.
 */

#include "briar_symbolum.h"

#include "chorda_aedificator.h"
#include "silva.h"
#include "tabula_dispersa.h"
#include "xar.h"
#include <string.h>

/* plagula parsata et SERVATA: ictus secundus in eadem gratis */
nomen structura {
           chorda  via;
           chorda  textus;
     SilvaPiscina* piscina;
     SilvaParsura* parsura;
   SilvaSemantica* semantica;
} BriarPlagulaParsata;

structura BriarCursorSymbolorum {
               Piscina* piscina;
    constans SilexFons* fons;
        TabulaDispersa* plagulae;   /* via -> BriarPlagulaParsata* */
                   i32  parsurae;
};

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

BriarCursorSymbolorum*
briar_cursorem_symbolorum_creare (
               Piscina* piscina,
    constans SilexFons* fons)
{
    BriarCursorSymbolorum* c;

    si (piscina == NIHIL || fons == NIHIL)
    {
        redde NIHIL;
    }
    c = (BriarCursorSymbolorum*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(BriarCursorSymbolorum));
    si (c == NIHIL)
    {
        redde NIHIL;
    }
    c->piscina   = piscina;
    c->fons      = fons;
    c->plagulae  = tabula_dispersa_creare_chorda(piscina, 32);
    c->parsurae  = ZEPHYRUM;
    redde c;
}

i32
briar_cursoris_parsurae (
    constans BriarCursorSymbolorum* cursor)
{
    redde (cursor != NIHIL) ? cursor->parsurae : ZEPHYRUM;
}

/* caput 'piscina.h' -> 'lib/piscina.c' (lex gemelli silicis).
 * Caput sine '.h' aut vacuum: vacua. */
interior chorda
_geminum (
    Piscina* piscina,
     chorda  caput)
{
    ChordaAedificator* a;

    si (   caput.mensura                   <= II
        || caput.datum[caput.mensura - II] != (i8)'.'
        || caput.datum[caput.mensura - I]  != (i8)'h')
    {
        redde _vacua();
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)64);
    chorda_aedificator_appendere_literis(a, "lib/");
    chorda_aedificator_appendere_chorda(a,
        chorda_sectio(caput, ZEPHYRUM, caput.mensura - I));
    chorda_aedificator_appendere_literis(a, "c");
    redde chorda_aedificator_finire(a);
}

/* plagulam parsare (aut e cursore sumere); NIHIL si gemellum abest
 * aut parsura fracta */
interior BriarPlagulaParsata*
_plagulam_sumere (
    BriarCursorSymbolorum* cursor,
                   chorda  via)
{
     BriarPlagulaParsata* p;
                  vacuum* prior = NIHIL;
                  chorda  textus;
                     b32  inventum = FALSUM;
                     Xar* clausura;
           SilvaExpansio* exp;
                     i32 k;

    si (tabula_dispersa_invenire(cursor->plagulae, via, &prior))
    {
        redde (BriarPlagulaParsata*)prior;
    }
    textus = silex_fons_legere(cursor->fons,
        chorda_ut_cstr(via, cursor->piscina), cursor->piscina,
        &inventum);
    si (!inventum || textus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    p = (BriarPlagulaParsata*)piscina_allocare(cursor->piscina,
        (memoriae_index)magnitudo(BriarPlagulaParsata));
    si (p == NIHIL)
    {
        redde NIHIL;
    }
    p->via        = via;
    p->textus     = textus;
    p->parsura    = NIHIL;
    p->semantica  = NIHIL;
    p->piscina    = silva_piscina_generare_dynamicum("briar_symbolum",
        (size_t)16777216);
    si (p->piscina == NIHIL)
    {
        redde NIHIL;
    }
    cursor->parsurae = cursor->parsurae + I;
    tabula_dispersa_inserere(cursor->plagulae, via, (vacuum*)p);

    clausura = silex_clausuram_e_contentis(cursor->piscina,
        cursor->fons, &textus, I);
    exp      = silva_expansio_creare(p->piscina);
    per (k = ZEPHYRUM;
        clausura != NIHIL && k < xar_numerus(clausura); k++)
    {
        constans SilexRes* res = (constans SilexRes*)xar_obtinere(
            clausura, k);

        si (   res->via.mensura                     <= II
            || res->via.datum[res->via.mensura - I] != (i8)'h')
        {
            perge;
        }
        silva_includendum_praebere(exp,
            chorda_ut_cstr(res->via, cursor->piscina),
            (constans character*)res->contentum.datum,
            (insignatus integer)res->contentum.mensura);
    }
    p->parsura = silva_parsare_cum_expansione(p->piscina, exp,
        chorda_ut_cstr(via, cursor->piscina),
        (constans character*)textus.datum,
        (insignatus integer)textus.mensura,
        &SILVA_C89_GRAMMATICA, NIHIL, NIHIL, NIHIL);
    si (p->parsura == NIHIL || p->parsura->commissio == NIHIL)
    {
        p->parsura = NIHIL;
        redde p;
    }
    /* errores parsurae NON obstant: definitiones plerumque manent,
     * et responsum 'non inventum' honestius est quam nihil */
    p->semantica = silva_c89_semantica_analysare(p->piscina,
        p->parsura);
    redde p;
}

BriarSymboliResponsum
briar_symbolum_quaerere (
    BriarCursorSymbolorum* cursor,
                   chorda  titulus,
                   chorda  caput)
{
     BriarSymboliResponsum  r;
       BriarPlagulaParsata* p;
                    chorda  via;
        insignatus integer  k;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.caput      = caput;
    r.via        = _vacua();
    r.definitio  = _vacua();
    si (cursor == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde r;
    }
    via = _geminum(cursor->piscina, caput);
    si (via.mensura == ZEPHYRUM)
    {
        redde r;
    }
    si (!silex_fons_existit(cursor->fons,
        chorda_ut_cstr(via, cursor->piscina), cursor->piscina))
    {
        redde r;   /* caput sine gemello - non vitium */
    }
    r.via  = via;
    p      = _plagulam_sumere(cursor, via);
    si (p == NIHIL || p->semantica == NIHIL || p->parsura == NIHIL)
    {
        redde r;
    }
    per (k = ZEPHYRUM; k < silva_c89_symbola_numerus(p->semantica);
        k++)
    {
        constans SemanticaSymbolum* s = silva_c89_symbolum_per_indicem(
            p->semantica, k);
              integer minimum = -I;
              integer maximum = ZEPHYRUM;

        si (   s                       == NIHIL
            || s->ex_systemate
            || s->est_implicitum
            || s->lexema               == NIHIL
            || s->lexema->fons_index   != p->parsura->fons_princeps
            || (i32)s->titulus.mensura != titulus.mensura
            || memcmp(s->titulus.datum, titulus.datum,
                (size_t)titulus.mensura) != ZEPHYRUM
            || silva_c89_definitio_functionis_corpus(s->declarans).genus
                == SILVA_VALOR_NIHIL)
        {
            perge;
        }
        silva_nodus_extensionem(s->declarans, p->parsura->fons_princeps,
            &minimum, &maximum);
        si (minimum < ZEPHYRUM || maximum <= minimum)
        {
            perge;
        }
        r.inventum   = VERUM;
        r.definitio  = chorda_sectio(p->textus, (i32)minimum,
            (i32)maximum);
        r.linea      = (i32)s->lexema->linea;
        redde r;
    }
    redde r;
}
