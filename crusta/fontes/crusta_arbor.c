/* crusta_arbor.c - Aedificator arboris crustae, pars I (vide .h)
 *
 * GRADUS (frama): nodus in constructione, locus liberos recipiens,
 * modus lectoris dum gradus in vertice est, status per genus. Gradus
 * clausus nodum suum gradui infra appendit (locus eius currens):
 * lista-nodus appenditur, nodus singularis ponitur.
 *
 * VERBUM: cursus partium ADIACENTIUM (offset partis == finis verbi);
 * pars non adiacens aut trivium aut lexema non-pars verbum claudit.
 * Modus gradus verbi = modus parentis post initium verbi (VERBA post
 * verbum primum imperii, ASSIGNATIONES post aedificatorem).
 *
 * IMPERIUM: status ZEPHYRUM (positio imperii, INITIUM), I (verbum
 * visum, VERBA), II (verbum primum aedificator, ASSIGNATIONES).
 */

#include "crusta_arbor.h"
#include "crusta_lector.h"
#include "crusta_lexicon.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include <string.h>


/* ==================================================
 * Gradus et parsatura
 * ================================================== */

nomen structura {
    MateriaNodus* nodus;
             s32  genus;
    /* locus liberos recipiens */
             i32 locus;
    /* modus petendi dum in vertice */
      CrustaModus modus;
              i32 status;
    /* verbum: finis partis ultimae (adiacentia) */
             s32 verbi_finis;
    /* expansio: '/' aut ':' aut ZEPHYRUM */
             s32 divisor;
    /* substitutio backtick: regio aperta */
             b32 gravis;
} Gradus;

nomen structura {
                     Piscina* piscina;
                CrustaLector  lector;
         MateriaLexiconRatum  lexicon;
                         Xar* gradus;       /* Gradus per valorem */
                         Xar* pendentia;    /* MateriaToken* trivia */
                MateriaToken* prior;
    constans CrustaDialectus* dialectus;
               CrustaParsura* relatio;
                         b32  memoria_defecit;
} Aedificatio;

interior Gradus*
_vertex (
    Aedificatio* p)
{
    i32 n = xar_numerus(p->gradus);

    redde n > ZEPHYRUM ? (Gradus*)xar_obtinere(p->gradus, n
        - I) : NIHIL;
}

interior Gradus*
_infra (
    Aedificatio* p,
            i32  gradus_infra)
{
    i32 n = xar_numerus(p->gradus);

    redde n > gradus_infra
        ? (Gradus*)xar_obtinere(p->gradus, n - I
            - gradus_infra) : NIHIL;
}

interior i32
_loci_numerus (
    s32 genus)
{
    redde CRUSTA_REGISTRUM.genera[genus].loci_numerus;
}

interior MateriaLocusSpecies
_species (
    s32 genus,
    i32 locus)
{
    redde (MateriaLocusSpecies)CRUSTA_REGISTRUM.loci[
        CRUSTA_REGISTRUM.genera[genus].loci_offset + locus].species;
}

interior MateriaNodus*
_nodus (
    Aedificatio* p,
            s32  genus)
{
    MateriaNodus* n = materia_nodus_creare(p->piscina, genus,
        _loci_numerus(genus));

    si (n == NIHIL)
    {
        p->memoria_defecit = VERUM;
    }
    redde n;
}

/* nodum in locum parentis: lista appenditur, singularis ponitur */
interior b32
_nodum_dare (
       Aedificatio* p,
      MateriaNodus* parens,
               i32  locus,
      MateriaNodus* filius)
{
    MateriaLocusSpecies s = _species(parens->genus, locus);

    si (s == MATERIA_LOCUS_LISTA_NODUS)
    {
        redde materia_nodus_appendere(p->piscina, parens, locus,
            materia_valor_nodus(filius), s);
    }
    redde materia_nodus_ponere(parens, locus,
        materia_valor_nodus(filius),
        s);
}

interior b32
_token_ponere (
    MateriaNodus* nodus,
             i32  locus,
    MateriaToken* t)
{
    redde materia_nodus_ponere(nodus, locus, materia_valor_token(t),
        MATERIA_LOCUS_TOKEN);
}

interior b32
_token_appendere (
       Aedificatio* p,
      MateriaNodus* nodus,
               i32  locus,
      MateriaToken* t)
{
    redde materia_nodus_appendere(p->piscina, nodus, locus,
        materia_valor_token(t), MATERIA_LOCUS_LISTA_TOKEN);
}

interior b32
_absens (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    redde nodus->loci[locus].genus == MATERIA_VALOR_NIHIL;
}

interior Gradus*
_pellere (
    Aedificatio* p,
   MateriaNodus* nodus,
            i32  locus,
    CrustaModus  modus)
{
    Gradus* g = (Gradus*)xar_addere(p->gradus);
       i32  n;

    si (g == NIHIL)
    {
        p->memoria_defecit = VERUM;
        redde NIHIL;
    }
    memset(g, ZEPHYRUM, magnitudo(*g));
    g->nodus  = nodus;
    g->genus  = nodus->genus;
    g->locus  = locus;
    g->modus  = modus;
    n         = xar_numerus(p->gradus);
    si (p->relatio != NIHIL && n > p->relatio->profunditas_maxima)
    {
        p->relatio->profunditas_maxima = n;
    }
    redde g;
}

interior s32
_token_finis (
    constans MateriaToken* t)
{
    redde t->byte_offset + (s32)t->valor.mensura;
}


/* ==================================================
 * Ligator (C7)
 * ================================================== */

interior b32
_cumulare (
       Aedificatio* p,
      MateriaToken* t)
{
    MateriaToken** sedes = (MateriaToken**)xar_addere(p->pendentia);

    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    *sedes = t;
    redde VERUM;
}

interior b32
_solvere (
       Aedificatio* p,
      MateriaToken* sequens)
{
    i32 numerus = xar_numerus(p->pendentia);
    MateriaToken** plana;
    i32 divisio;
    i32 j;

    si (numerus == ZEPHYRUM)
    {
        p->prior = sequens;
        redde VERUM;
    }
    plana = (MateriaToken**)piscina_allocare_ordinatum(p->piscina,
        (memoriae_index)magnitudo(MateriaToken*)
            * (memoriae_index)numerus,
        (memoriae_index)magnitudo(MateriaToken*));
    si (plana == NIHIL)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        plana[j] = *(MateriaToken**)xar_obtinere(p->pendentia, j);
    }

    /* divisio post lineam novam ULTIMAM (LINEA aut LAMINA) */
    divisio = ZEPHYRUM;
    si (p->prior != NIHIL)
    {
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (   plana[j]->genus == (s32)CRUSTA_LEX_LINEA
                || plana[j]->genus == (s32)CRUSTA_LEX_LAMINA)
            {
                divisio = j + I;
            }
        }
    }
    si (divisio > ZEPHYRUM)
    {
        si (!materia_token_trivia_post_ponere(p->prior, p->piscina,
            plana,
                divisio))
        {
            redde FALSUM;
        }
    }
    si (divisio < numerus)
    {
        si (!materia_token_trivia_ante_ponere(sequens, p->piscina,
                plana + divisio, numerus - divisio))
        {
            redde FALSUM;
        }
    }
    xar_vacare(p->pendentia);
    p->prior = sequens;
    redde VERUM;
}


/* ==================================================
 * Genera gradum
 * ================================================== */

interior b32
_lista_est (
    s32 genus)
{
    redde genus == (s32)CRUSTA_GENUS_PROGRAMMA
        || genus == (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO
        || genus == (s32)CRUSTA_GENUS_PARS_PROCESSUS;
}

interior b32
_sententia_est (
    s32 genus)
{
    redde genus == (s32)CRUSTA_GENUS_IMPERIUM
        || genus == (s32)CRUSTA_GENUS_PIPA
        || genus == (s32)CRUSTA_GENUS_CATENA;
}

/* genera quorum clausura absens numeranda est: locus clausurae */
interior s32
_locus_clausurae (
    s32 genus)
{
    commutatio (genus)
    {
        casus CRUSTA_GENUS_PARS_GEMINA:
        casus CRUSTA_GENUS_PARS_VERSA:
            redde (s32)CRUSTA_GEMINA_TOK_CLAUSURA;
        casus CRUSTA_GENUS_PARS_EXPANSIO:
            redde (s32)CRUSTA_EXPANSIO_TOK_CLAUSURA;
        casus CRUSTA_GENUS_PARS_SUBSTITUTIO:
        casus CRUSTA_GENUS_PARS_PROCESSUS:
            redde (s32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA;
        casus CRUSTA_GENUS_TABULATUM:
            redde (s32)CRUSTA_TABULATUM_TOK_CLAUSURA;
        casus CRUSTA_GENUS_PARS_PARAMETRUM:
            redde (s32)CRUSTA_PARAMETRUM_TOK_TITULUS;
        ordinarius:
            redde (s32)-I;
    }
}

/* gradum verticis claudere: nodum gradui infra dare */
interior b32
_claudere (
    Aedificatio* p)
{
     Gradus* g = _vertex(p);
     Gradus* parens;
        s32  locus_clausurae;

    si (g == NIHIL)
    {
        redde FALSUM;
    }
    locus_clausurae = _locus_clausurae(g->genus);
    si (   locus_clausurae >= ZEPHYRUM
        && _absens(g->nodus, (i32)locus_clausurae)
        && p->relatio      != NIHIL)
    {
        p->relatio->clausurae_absentes++;
    }
    parens = _infra(p, I);
    si (parens == NIHIL)
    {
        redde FALSUM;   /* programma numquam clauditur hac via */
    }
    si (!_nodum_dare(p, parens->nodus, parens->locus, g->nodus))
    {
        redde FALSUM;
    }
    xar_removere_ultimum(p->gradus);
    redde VERUM;
}

/* gradum verticis claudere SINE datione (involvendus in catenam/pipam):
 * nodum reddit */
interior MateriaNodus*
_tollere (
    Aedificatio* p)
{
          Gradus* g = _vertex(p);
    MateriaNodus* n = g->nodus;

    xar_removere_ultimum(p->gradus);
    redde n;
}

/* post clausuram verbi: gradus infra reagit */
interior b32
_post_verbum (
    Aedificatio* p);

/* verbum in vertice claudere (si adest) */
interior b32
_verbum_claudere (
    Aedificatio* p)
{
    Gradus* g = _vertex(p);

    si (g != NIHIL && g->genus == (s32)CRUSTA_GENUS_VERBUM)
    {
        si (!_claudere(p))
        {
            redde FALSUM;
        }
        redde _post_verbum(p);
    }
    redde VERUM;
}

interior b32
_aedificator_est (
       Aedificatio* p,
      MateriaNodus* verbum)
{
    chorda v;
       s32 i;

    si (!crusta_verbum_staticum(p->piscina, verbum, &v))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; p->dialectus->aedificatores[i] != NIHIL; i++)
    {
        si (   (i32)strlen(p->dialectus->aedificatores[i]) == v.mensura
            && memcmp(p->dialectus->aedificatores[i], v.datum,
                (size_t)v.mensura) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* redirectio completa: petitio heredoc, gradus clausus */
interior b32
_redirectionem_finire (
    Aedificatio* p)
{
          Gradus* g       = _vertex(p);
    MateriaToken* signum  = NIHIL;

    si (g->nodus->loci[CRUSTA_REDIRECTIO_TOK_OPERATOR].genus
            == MATERIA_VALOR_TOKEN)
    {
        signum =
            g->nodus->loci[CRUSTA_REDIRECTIO_TOK_OPERATOR].datum.token;
    }
    si (   signum != NIHIL && signum->valor.mensura >= (i32)II
        && signum->valor.datum[ZEPHYRUM] == '<'
        && signum->valor.datum[I] == '<'
        && !(signum->valor.mensura == (i32)III
        && signum->valor.datum[II] == '<')
        && g->nodus->loci[CRUSTA_REDIRECTIO_SCOPUS].genus
            == MATERIA_VALOR_NODUS)
    {
        MateriaNodus* scopus =
            g->nodus->loci[CRUSTA_REDIRECTIO_SCOPUS].datum.nodus;
        chorda delimitator;
           b32 tabulae = signum->valor.mensura == (i32)III;

        si (!crusta_verbum_staticum(p->piscina, scopus, &delimitator))
        {
            delimitator.mensura  = ZEPHYRUM;
            delimitator.datum    = NIHIL;
        }
        si (!crusta_lector_heredoc_petere(&p->lector, delimitator,
                crusta_verbum_citatum(scopus), tabulae))
        {
            redde FALSUM;
        }
        si (p->relatio != NIHIL)
        {
            p->relatio->heredoca++;
        }
    }
    redde _claudere(p);
}

interior b32
_post_verbum (
    Aedificatio* p)
{
    Gradus* g = _vertex(p);

    si (g == NIHIL)
    {
        redde VERUM;
    }
    commutatio (g->genus)
    {
        casus CRUSTA_GENUS_REDIRECTIO:
            redde _redirectionem_finire(p);
        casus CRUSTA_GENUS_ASSIGNATIO:
            redde _claudere(p);
        casus CRUSTA_GENUS_IMPERIUM:
            si (g->status == I)
            {
                MateriaValor* liberi =
                    &g->nodus->loci[CRUSTA_IMPERIUM_LIBERI];

                si (   liberi->genus == MATERIA_VALOR_LISTA
                    && materia_valor_lista_numerus(*liberi) == (i32)I)
                {
                    MateriaValor* primus =
                        materia_valor_lista_obtinere(*liberi, ZEPHYRUM);

                    si (   primus->genus == MATERIA_VALOR_NODUS
                        && _aedificator_est(p, primus->datum.nodus))
                    {
                        g->status  = II;
                        g->modus   = CRUSTA_MODUS_ASSIGNATIONES;
                    }
                }
            }
            redde VERUM;
        ordinarius:
            redde VERUM;
    }
}

/* gradus pendentes intra sententiam claudere (verbum, parametrum,
 * redirectio, assignatio) usque ad imperium/pipam/catenam/listam */
interior b32
_intra_sententiam_claudere (
    Aedificatio* p)
{
    Gradus* g;

    dum ((g = _vertex(p)) != NIHIL)
    {
        si (_lista_est(g->genus) || _sententia_est(g->genus))
        {
            redde VERUM;
        }
        si (g->genus == (s32)CRUSTA_GENUS_VERBUM)
        {
            si (!_verbum_claudere(p))
            {
                redde FALSUM;
            }
            perge;
        }
        si (g->genus == (s32)CRUSTA_GENUS_REDIRECTIO)
        {
            si (!_redirectionem_finire(p))
            {
                redde FALSUM;
            }
            perge;
        }
        si (!_claudere(p))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* sententiam pendentem totam claudere (imperium, pipa, catena) usque
 * ad listam */
interior b32
_sententiam_claudere (
    Aedificatio* p)
{
    Gradus* g;

    si (!_intra_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    dum ((g = _vertex(p)) != NIHIL && _sententia_est(g->genus))
    {
        si (!_claudere(p))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Malum, separator, catena, pipa
 * ================================================== */

/* lexema quod grammatica ponere non potest: sententia pendens
 * clauditur, malum listae appenditur (aut mali ultimo adiacenti) */
interior b32
_malum (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* lista;
    MateriaValor* liberi;
    MateriaNodus* malum = NIHIL;

    si (!_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    lista   = _vertex(p);
    liberi  = &lista->nodus->loci[lista->locus];
    si (   liberi->genus == MATERIA_VALOR_LISTA
        && materia_valor_lista_numerus(*liberi) > ZEPHYRUM)
    {
        MateriaValor* ultimus = materia_valor_lista_obtinere(*liberi,
            materia_valor_lista_numerus(*liberi) - I);

        si (   ultimus->genus              == MATERIA_VALOR_NODUS
            && ultimus->datum.nodus->genus == (s32)CRUSTA_GENUS_MALUM)
        {
            malum = ultimus->datum.nodus;
        }
    }
    si (malum == NIHIL)
    {
        malum = _nodus(p, (s32)CRUSTA_GENUS_MALUM);
        si (malum == NIHIL)
        {
            redde FALSUM;
        }
        si (!_nodum_dare(p, lista->nodus, lista->locus, malum))
        {
            redde FALSUM;
        }
        si (p->relatio != NIHIL)
        {
            p->relatio->mala++;
        }
    }
    redde _token_appendere(p, malum, (i32)CRUSTA_MALUM_TOKENS, t);
}

interior b32
_separator (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* lista;
    MateriaNodus* s;

    /* separator sine sententia pendente (';' post ';', ';' initio):
     * bash 'syntax error near unexpected token' - malum */
    si (_lista_est(_vertex(p)->genus))
    {
        redde _malum(p, t);
    }
    si (!_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    lista  = _vertex(p);
    s      = _nodus(p, (s32)CRUSTA_GENUS_SEPARATOR);
    si (s == NIHIL || !_token_ponere(s, (i32)CRUSTA_SEPARATOR_TOK, t))
    {
        redde FALSUM;
    }
    redde _nodum_dare(p, lista->nodus, lista->locus, s);
}

interior b32
_operator_appendere (
       Aedificatio* p,
      MateriaNodus* parens,
               i32  locus,
      MateriaToken* t)
{
    MateriaNodus* o = _nodus(p, (s32)CRUSTA_GENUS_OPERATOR);

    si (o == NIHIL || !_token_ponere(o, (i32)CRUSTA_OPERATOR_TOK, t))
    {
        redde FALSUM;
    }
    redde _nodum_dare(p, parens, locus, o);
}

/* '&&' '||': rem pendentem (imperium aut pipa) in catenam involvere */
interior b32
_catena (
       Aedificatio* p,
      MateriaToken* t)
{
          Gradus* g;
    MateriaNodus* res = NIHIL;
    MateriaNodus* catena;

    si (!_intra_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (g->genus == (s32)CRUSTA_GENUS_IMPERIUM)
    {
        res  = _tollere(p);
        g    = _vertex(p);
    }
    si (g->genus == (s32)CRUSTA_GENUS_PIPA)
    {
        si (res != NIHIL && !_nodum_dare(p, g->nodus, g->locus, res))
        {
            redde FALSUM;
        }
        res  = _tollere(p);
        g    = _vertex(p);
    }
    si (g->genus != (s32)CRUSTA_GENUS_CATENA)
    {
        catena = _nodus(p, (s32)CRUSTA_GENUS_CATENA);
        si (   catena == NIHIL
            || _pellere(p, catena, (i32)CRUSTA_CATENA_LIBERI,
                CRUSTA_MODUS_INITIUM) == NIHIL)
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (res != NIHIL && !_nodum_dare(p, g->nodus, g->locus, res))
    {
        redde FALSUM;
    }
    redde _operator_appendere(p, g->nodus, g->locus, t);
}

/* '|' '|&': imperium pendens in pipam involvere */
interior b32
_pipa (
       Aedificatio* p,
      MateriaToken* t)
{
          Gradus* g;
    MateriaNodus* res = NIHIL;
    MateriaNodus* pipa;

    si (!_intra_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (g->genus == (s32)CRUSTA_GENUS_IMPERIUM)
    {
        res  = _tollere(p);
        g    = _vertex(p);
    }
    si (g->genus != (s32)CRUSTA_GENUS_PIPA)
    {
        pipa = _nodus(p, (s32)CRUSTA_GENUS_PIPA);
        si (   pipa == NIHIL
            || _pellere(p, pipa, (i32)CRUSTA_PIPA_LIBERI,
                CRUSTA_MODUS_INITIUM) == NIHIL)
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (res != NIHIL && !_nodum_dare(p, g->nodus, g->locus, res))
    {
        redde FALSUM;
    }
    redde _operator_appendere(p, g->nodus, g->locus, t);
}

/* '!' aut 'time' in positione imperii: pipa cum praefixis */
interior b32
_praefixum_pipae (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g = _vertex(p);
    MateriaNodus* pipa;

    si (g->genus != (s32)CRUSTA_GENUS_PIPA)
    {
        pipa = _nodus(p, (s32)CRUSTA_GENUS_PIPA);
        si (   pipa == NIHIL
            || _pellere(p, pipa, (i32)CRUSTA_PIPA_LIBERI,
                CRUSTA_MODUS_INITIUM) == NIHIL)
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    redde _token_appendere(p, g->nodus, (i32)CRUSTA_PIPA_PRAEFIXA, t);
}

/* 'time -p': '-p' verbum post 'time' in pipa sine imperio */
interior b32
_optio_temporis (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g = _vertex(p);
    MateriaValor* praefixa;
    MateriaValor* ultimum;

    si (   g->genus                 != (s32)CRUSTA_GENUS_PIPA
        || t->genus                 != (s32)CRUSTA_LEX_LITTERALIS
        || t->valor.mensura         != (i32)II
        || t->valor.datum[ZEPHYRUM] != '-'
        || t->valor.datum[I]        != 'p')
    {
        redde FALSUM;
    }
    praefixa = &g->nodus->loci[CRUSTA_PIPA_PRAEFIXA];
    si (   praefixa->genus != MATERIA_VALOR_LISTA
        || !_absens(g->nodus, (i32)CRUSTA_PIPA_LIBERI))
    {
        redde FALSUM;
    }
    ultimum = materia_valor_lista_obtinere(*praefixa,
        materia_valor_lista_numerus(*praefixa) - I);
    si (   ultimum->genus                      != MATERIA_VALOR_TOKEN
        || ultimum->datum.token->valor.mensura != (i32)IV
        || memcmp(ultimum->datum.token->valor.datum, "time", (size_t)IV)
            != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde _token_appendere(p, g->nodus, (i32)CRUSTA_PIPA_PRAEFIXA, t);
}


/* ==================================================
 * Imperium, assignatio, redirectio
 * ================================================== */

/* imperium aperire si vertex lista, catena aut pipa est */
interior b32
_imperium_aperire (
    Aedificatio* p)
{
    Gradus* g = _vertex(p);
    MateriaNodus* imperium;

    si (g->genus == (s32)CRUSTA_GENUS_IMPERIUM)
    {
        redde VERUM;
    }
    imperium = _nodus(p, (s32)CRUSTA_GENUS_IMPERIUM);
    si (imperium == NIHIL)
    {
        redde FALSUM;
    }
    redde _pellere(p, imperium, (i32)CRUSTA_IMPERIUM_LIBERI,
        CRUSTA_MODUS_INITIUM) != NIHIL;
}

interior CrustaModus
_modus_imperii (
    constans Gradus* g)
{
    si (g->status == I)
    {
        redde CRUSTA_MODUS_VERBA;
    }
    si (g->status == II)
    {
        redde CRUSTA_MODUS_ASSIGNATIONES;
    }
    redde CRUSTA_MODUS_INITIUM;
}

interior b32
_assignatio_incipere (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g;
    MateriaNodus* a;
    i32 locus = t->genus == (s32)CRUSTA_LEX_ASSIGNATIO_TITULUS
        ? (i32)CRUSTA_ASSIGNATIO_TOK_TITULUS
        : (i32)CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM;

    si (!_verbum_claudere(p))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (g->genus != (s32)CRUSTA_GENUS_TABULATUM)
    {
        si (!_imperium_aperire(p))
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    a = _nodus(p, (s32)CRUSTA_GENUS_ASSIGNATIO);
    si (a == NIHIL || !_token_ponere(a, locus, t))
    {
        redde FALSUM;
    }
    redde _pellere(p, a, (i32)CRUSTA_ASSIGNATIO_VALOR,
        g->genus == (s32)CRUSTA_GENUS_TABULATUM ? CRUSTA_MODUS_TABULATUM
            : _modus_imperii(g)) != NIHIL;
}

interior b32
_redirectio_incipere (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g;
    MateriaNodus* r;

    si (!_verbum_claudere(p))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    si (   g->genus == (s32)CRUSTA_GENUS_REDIRECTIO
        && t->genus == (s32)CRUSTA_LEX_REDIRECTIO_OPERATOR
        && _absens(g->nodus, (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR))
    {
        redde _token_ponere(g->nodus,
            (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR, t);
    }
    si (g->genus == (s32)CRUSTA_GENUS_REDIRECTIO)
    {
        /* redirectio sine scopo (2>) ante alteram: clausa absens */
        si (!_redirectionem_finire(p))
        {
            redde FALSUM;
        }
    }
    si (g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO)
    {
        si (!_claudere(p))
        {
            redde FALSUM;
        }
    }
    si (!_imperium_aperire(p))
    {
        redde FALSUM;
    }
    r = _nodus(p, (s32)CRUSTA_GENUS_REDIRECTIO);
    si (r == NIHIL)
    {
        redde FALSUM;
    }
    si (!_token_ponere(r, t->genus == (s32)CRUSTA_LEX_REDIRECTIO_FD
            ? (i32)CRUSTA_REDIRECTIO_TOK_FD
            : (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR, t))
    {
        redde FALSUM;
    }
    redde _pellere(p, r, (i32)CRUSTA_REDIRECTIO_SCOPUS,
        CRUSTA_MODUS_VERBA)
        != NIHIL;
}


/* ==================================================
 * Partes verbi
 * ================================================== */

interior b32
_pars_simplex (
    s32  genus_lexematis,
    s32* genus_partis)
{
    commutatio (genus_lexematis)
    {
        casus CRUSTA_LEX_LITTERALIS:
        /* P3: litteralis (intermedium nominatum) */
        casus CRUSTA_LEX_ARITHMETICA_PARTIS_APERTURA:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_LITTERALIS;
            redde VERUM;
        casus CRUSTA_LEX_EFFUGIUM:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_EFFUGIUM;
            redde VERUM;
        casus CRUSTA_LEX_CONTINUATIO:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_CONTINUATIO;
            redde VERUM;
        casus CRUSTA_LEX_SIMPLEX:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_SIMPLEX;
            redde VERUM;
        casus CRUSTA_LEX_EFFUGIA:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_EFFUGIA;
            redde VERUM;
        casus CRUSTA_LEX_DOMUS:
            *genus_partis = (s32)CRUSTA_GENUS_PARS_DOMUS;
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

interior b32
_pars_est (
    s32 genus_lexematis)
{
    s32 g;

    redde _pars_simplex(genus_lexematis, &g)
        || genus_lexematis == (s32)CRUSTA_LEX_GEMINA_APERTURA
        || genus_lexematis == (s32)CRUSTA_LEX_VERSA_APERTURA
        || genus_lexematis == (s32)CRUSTA_LEX_EXPANSIO_APERTURA
        || genus_lexematis == (s32)CRUSTA_LEX_SUBSTITUTIO_APERTURA
        || genus_lexematis == (s32)CRUSTA_LEX_GRAVIS
        || genus_lexematis == (s32)CRUSTA_LEX_PROCESSUS_APERTURA
        || genus_lexematis == (s32)CRUSTA_LEX_PARAMETRUM_SIGILLUM;
}

/* gradus qui partes directe recipit (gemina, versa) an verbum poscat */
interior b32
_partes_directe (
    s32 genus)
{
    redde genus == (s32)CRUSTA_GENUS_PARS_GEMINA
        || genus == (s32)CRUSTA_GENUS_PARS_VERSA;
}

/* verbum recipiens invenire aut aperire pro parte ad t */
interior b32
_verbum_parare (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g = _vertex(p);
    MateriaNodus* verbum;
    CrustaModus modus;

    si (g->genus == (s32)CRUSTA_GENUS_VERBUM)
    {
        si (g->verbi_finis == t->byte_offset)
        {
            redde VERUM;
        }
        si (!_verbum_claudere(p))
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (_partes_directe(g->genus))
    {
        redde VERUM;
    }
    /* 'time -p' */
    si (_optio_temporis(p, t))
    {
        redde FALSUM;   /* consumptum: vocans nihil amplius facit */
    }
    si (_lista_est(g->genus) || _sententia_est(g->genus))
    {
        si (!_imperium_aperire(p))
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (g->genus == (s32)CRUSTA_GENUS_IMPERIUM && g->status == ZEPHYRUM)
    {
        g->status  = I;
        g->modus   = CRUSTA_MODUS_VERBA;
    }
    modus = g->modus;
    si (g->genus == (s32)CRUSTA_GENUS_PARS_EXPANSIO)
    {
        modus = g->divisor == (s32)'/' ? CRUSTA_MODUS_EXPANSIO_EXEMPLAR
              : g->divisor == (s32)':' ? CRUSTA_MODUS_EXPANSIO_SECTIO
              : CRUSTA_MODUS_EXPANSIO_VERBUM;
    }
    verbum = _nodus(p, (s32)CRUSTA_GENUS_VERBUM);
    si (verbum == NIHIL)
    {
        redde FALSUM;
    }
    redde _pellere(p, verbum, (i32)CRUSTA_VERBUM_PARTES, modus)
        != NIHIL;
}

/* finem verbi renovare post partem (simplicem aut compositam
 * clausam) */
interior vacuum
_verbi_finem_ponere (
    Aedificatio* p,
            s32  finis)
{
    Gradus* g = _vertex(p);

    si (g != NIHIL && g->genus == (s32)CRUSTA_GENUS_VERBUM)
    {
        g->verbi_finis = finis;
    }
}

interior b32
_partem_recipere (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g;
    MateriaNodus* pars;
    s32 genus_partis;

    si (!_verbum_parare(p, t))
    {
        redde !p->memoria_defecit;
    }
    g = _vertex(p);

    si (_pars_simplex(t->genus, &genus_partis))
    {
        pars = _nodus(p, genus_partis);
        si (   pars == NIHIL
            || !_token_ponere(pars, (i32)CRUSTA_PARS_TOK, t)
            || !_nodum_dare(p, g->nodus, g->locus, pars))
        {
            redde FALSUM;
        }
        _verbi_finem_ponere(p, _token_finis(t));
        redde VERUM;
    }
    commutatio (t->genus)
    {
        casus CRUSTA_LEX_PARAMETRUM_SIGILLUM:
            pars = _nodus(p, (s32)CRUSTA_GENUS_PARS_PARAMETRUM);
            si (   pars == NIHIL
                || !_token_ponere(pars,
                (i32)CRUSTA_PARAMETRUM_TOK_SIGILLUM,
                    t))
            {
                redde FALSUM;
            }
            redde _pellere(p, pars, (i32)CRUSTA_PARAMETRUM_TOK_TITULUS,
                g->modus) != NIHIL;
        casus CRUSTA_LEX_GEMINA_APERTURA:
        casus CRUSTA_LEX_VERSA_APERTURA:
            pars = _nodus(p, t->genus == (s32)CRUSTA_LEX_GEMINA_APERTURA
                ? (s32)CRUSTA_GENUS_PARS_GEMINA
                : (s32)CRUSTA_GENUS_PARS_VERSA);
            si (   pars == NIHIL
                || !_token_ponere(pars,
                (i32)CRUSTA_GEMINA_TOK_APERTURA, t))
            {
                redde FALSUM;
            }
            redde _pellere(p, pars, (i32)CRUSTA_GEMINA_PARTES,
                CRUSTA_MODUS_GEMINA) != NIHIL;
        casus CRUSTA_LEX_EXPANSIO_APERTURA:
            pars = _nodus(p, (s32)CRUSTA_GENUS_PARS_EXPANSIO);
            si (   pars == NIHIL
                || !_token_ponere(pars,
                (i32)CRUSTA_EXPANSIO_TOK_APERTURA,
                    t))
            {
                redde FALSUM;
            }
            redde _pellere(p, pars, (i32)CRUSTA_EXPANSIO_ARGUMENTA,
                CRUSTA_MODUS_EXPANSIO) != NIHIL;
        casus CRUSTA_LEX_SUBSTITUTIO_APERTURA:
        casus CRUSTA_LEX_PROCESSUS_APERTURA:
            pars = _nodus(p, t->genus
                == (s32)CRUSTA_LEX_SUBSTITUTIO_APERTURA
                ? (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO
                : (s32)CRUSTA_GENUS_PARS_PROCESSUS);
            si (   pars == NIHIL
                || !_token_ponere(pars,
                    (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA, t))
            {
                redde FALSUM;
            }
            redde _pellere(p, pars, (i32)CRUSTA_SUBSTITUTIO_LIBERI,
                CRUSTA_MODUS_INITIUM) != NIHIL;
        casus CRUSTA_LEX_GRAVIS:
        {
               s32  finis;
               i32  gravis = crusta_lector_cauda(t)->gravis + I;
            Gradus* novus;

            pars = _nodus(p, (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO);
            si (   pars == NIHIL
                || !_token_ponere(pars,
                    (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA, t))
            {
                redde FALSUM;
            }
            finis = crusta_lector_gravem_quaerere(&p->lector,
                p->lector.situs.cursor, gravis);
            si (finis < ZEPHYRUM)
            {
                i32 n = xar_numerus(p->lector.regiones);

                finis = n > ZEPHYRUM
                    ? ((CrustaRegio*)xar_obtinere(p->lector.regiones,
                        n - I))->finis
                    : (s32)p->lector.mensura;
            }
            si (!crusta_lector_regionem_aperire(&p->lector, finis,
                gravis))
            {
                redde FALSUM;
            }
            novus = _pellere(p, pars, (i32)CRUSTA_SUBSTITUTIO_LIBERI,
                CRUSTA_MODUS_INITIUM);
            si (novus == NIHIL)
            {
                redde FALSUM;
            }
            novus->gravis = VERUM;
            redde VERUM;
        }
        ordinarius:
            redde FALSUM;
    }
}

/* pars composita in vertice clausa (lexema clausurae positum): gradum
 * claudere et finem verbi renovare */
interior b32
_partem_claudere (
       Aedificatio* p,
      MateriaToken* clausura)
{
    si (!_claudere(p))
    {
        redde FALSUM;
    }
    _verbi_finem_ponere(p, _token_finis(clausura));
    redde VERUM;
}


/* ==================================================
 * Expansio ${ }
 * ================================================== */

interior b32
_expansio (
       Aedificatio* p,
            Gradus* g,
      MateriaToken* t)
{
    commutatio (t->genus)
    {
        casus CRUSTA_LEX_EXPANSIO_PRAEFIXUM:
            redde _token_ponere(g->nodus,
                (i32)CRUSTA_EXPANSIO_TOK_PRAEFIXUM, t);
        casus CRUSTA_LEX_PARAMETRUM_TITULUS:
            g->status = I;
            redde _token_ponere(g->nodus,
                (i32)CRUSTA_EXPANSIO_TOK_TITULUS,
                t);
        casus CRUSTA_LEX_SUBSCRIPTUM:
            redde _token_ponere(g->nodus,
                (i32)CRUSTA_EXPANSIO_TOK_SUBSCRIPTUM, t);
        casus CRUSTA_LEX_EXPANSIO_OPERATOR:
            si (g->status < II)
            {
                g->status = II;
                si (t->valor.datum[ZEPHYRUM] == '/')
                {
                    g->divisor = (s32)'/';
                }
                alioquin si (   t->valor.mensura         == (i32)I
                             && t->valor.datum[ZEPHYRUM] == ':')
                {
                    g->divisor = (s32)':';
                }
                g->modus = g->divisor == (s32)'/'
                    ? CRUSTA_MODUS_EXPANSIO_EXEMPLAR
                    : g->divisor
                        == (s32)':' ? CRUSTA_MODUS_EXPANSIO_SECTIO
                    : CRUSTA_MODUS_EXPANSIO_VERBUM;
                redde _token_ponere(g->nodus,
                    (i32)CRUSTA_EXPANSIO_TOK_OPERATOR, t);
            }
            /* divisor intra argumenta: operator nudus; post '/' primum
             * substitutum sequitur sine divisore */
            si (g->divisor == (s32)'/')
            {
                g->divisor  = ZEPHYRUM;
                g->modus    = CRUSTA_MODUS_EXPANSIO_VERBUM;
            }
            redde _operator_appendere(p, g->nodus, g->locus, t);
        casus CRUSTA_LEX_EXPANSIO_CLAUSURA:
            si (!_token_ponere(g->nodus,
                (i32)CRUSTA_EXPANSIO_TOK_CLAUSURA,
                    t))
            {
                redde FALSUM;
            }
            redde _partem_claudere(p, t);
        ordinarius:
            /* pars in argumentis (status < II: titulus ruptus -
             * totalitas) */
            g->status = II;
            redde _partem_recipere(p, t);
    }
}


/* ==================================================
 * Dispensatio
 * ================================================== */

interior b32
_tractare (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g = _vertex(p);

    /* partes compositae in vertice quae lexema suum exspectant */
    si (g->genus == (s32)CRUSTA_GENUS_PARS_PARAMETRUM)
    {
        si (t->genus == (s32)CRUSTA_LEX_PARAMETRUM_TITULUS)
        {
            si (!_token_ponere(g->nodus,
                (i32)CRUSTA_PARAMETRUM_TOK_TITULUS,
                    t))
            {
                redde FALSUM;
            }
            redde _partem_claudere(p, t);
        }
        si (!_claudere(p))   /* titulus absens */
        {
            redde FALSUM;
        }
        g = _vertex(p);
    }
    si (g->genus == (s32)CRUSTA_GENUS_PARS_EXPANSIO)
    {
        /* argumenta: verbum in vertice tractatur infra; expansio ipsa
         * lexemata sua sumit */
        redde _expansio(p, g, t);
    }
    si (   g->genus            == (s32)CRUSTA_GENUS_VERBUM
        && _infra(p, I)->genus == (s32)CRUSTA_GENUS_PARS_EXPANSIO
        && (t->genus == (s32)CRUSTA_LEX_EXPANSIO_CLAUSURA
            || t->genus == (s32)CRUSTA_LEX_EXPANSIO_OPERATOR))
    {
        si (!_claudere(p))
        {
            redde FALSUM;
        }
        redde _expansio(p, _vertex(p), t);
    }
    si (   _partes_directe(g->genus)
        && t->genus == (s32)CRUSTA_LEX_GEMINA_CLAUSURA)
    {
        si (!_token_ponere(g->nodus, (i32)CRUSTA_GEMINA_TOK_CLAUSURA,
            t))
        {
            redde FALSUM;
        }
        redde _partem_claudere(p, t);
    }

    si (_pars_est(t->genus))
    {
        redde _partem_recipere(p, t);
    }

    commutatio (t->genus)
    {
        casus CRUSTA_LEX_SEPARATOR:
        casus CRUSTA_LEX_SEPARATOR_LINEAE:
            si (   g->genus == (s32)CRUSTA_GENUS_TABULATUM
                || _partes_directe(g->genus))
            {
                redde _malum(p, t);
            }
            redde _separator(p, t);
        casus CRUSTA_LEX_ET:
        casus CRUSTA_LEX_AUT:
            redde _catena(p, t);
        casus CRUSTA_LEX_PIPA:
        casus CRUSTA_LEX_PIPA_ERRORIS:
            redde _pipa(p, t);
        casus CRUSTA_LEX_ASSIGNATIO_TITULUS:
            redde _assignatio_incipere(p, t);
        casus CRUSTA_LEX_SUBSCRIPTUM:
            si (g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO)
            {
                redde _token_ponere(g->nodus,
                    (i32)CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM, t);
            }
            redde _assignatio_incipere(p, t);
        casus CRUSTA_LEX_ASSIGNATIO_OPERATOR:
            si (g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO)
            {
                redde _token_ponere(g->nodus,
                    (i32)CRUSTA_ASSIGNATIO_TOK_OPERATOR, t);
            }
            redde _malum(p, t);
        casus CRUSTA_LEX_REDIRECTIO_FD:
        casus CRUSTA_LEX_REDIRECTIO_OPERATOR:
            redde _redirectio_incipere(p, t);
        casus CRUSTA_LEX_PARENTHESIS:
            /* valor tabulae post '=' */
            si (   g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO
                && !_absens(g->nodus,
                (i32)CRUSTA_ASSIGNATIO_TOK_OPERATOR)
                && _absens(g->nodus, (i32)CRUSTA_ASSIGNATIO_VALOR))
            {
                MateriaNodus* tabula = _nodus(p,
                    (s32)CRUSTA_GENUS_TABULATUM);

                si (   tabula == NIHIL
                    || !_token_ponere(tabula,
                    (i32)CRUSTA_TABULATUM_TOK_APERTURA,
                        t))
                {
                    redde FALSUM;
                }
                redde _pellere(p, tabula, (i32)CRUSTA_TABULATUM_LIBERI,
                    CRUSTA_MODUS_TABULATUM) != NIHIL;
            }
            redde _malum(p, t);   /* subcrusta: P5 */
        casus CRUSTA_LEX_PARENTHESIS_CLAUSURA:
        {
            s32 i;
            s32 n           = (s32)xar_numerus(p->gradus);
            s32 claudendum  = (s32)-I;

            per (i = n - I; i >= ZEPHYRUM; i--)
            {
                Gradus* h = (Gradus*)xar_obtinere(p->gradus, (i32)i);

                si (   h->genus == (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO
                    || h->genus == (s32)CRUSTA_GENUS_PARS_PROCESSUS
                    || h->genus == (s32)CRUSTA_GENUS_TABULATUM)
                {
                    claudendum = h->genus;
                    frange;
                }
                si (h->genus == (s32)CRUSTA_GENUS_PROGRAMMA)
                {
                    frange;
                }
            }
            si (claudendum < ZEPHYRUM)
            {
                redde _malum(p, t);
            }
            si (claudendum == (s32)CRUSTA_GENUS_TABULATUM)
            {
                Gradus* h;

                si (!_verbum_claudere(p))
                {
                    redde FALSUM;
                }
                h = _vertex(p);
                dum (h->genus != (s32)CRUSTA_GENUS_TABULATUM)
                {
                    si (!_claudere(p))
                    {
                        redde FALSUM;
                    }
                    h = _vertex(p);
                }
                si (   !_token_ponere(h->nodus,
                        (i32)CRUSTA_TABULATUM_TOK_CLAUSURA, t)
                    || !_claudere(p))   /* tabulatum in assignationem */
                {
                    redde FALSUM;
                }
                redde _claudere(p);     /* assignatio in imperium */
            }
            si (!_sententiam_claudere(p))
            {
                redde FALSUM;
            }
            g = _vertex(p);
            si (!_token_ponere(g->nodus,
                    (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA, t))
            {
                redde FALSUM;
            }
            redde _partem_claudere(p, t);
        }
        casus CRUSTA_LEX_RESERVATUM:
            si (   t->valor.mensura         == (i32)I
                && t->valor.datum[ZEPHYRUM] == '!')
            {
                si (!_intra_sententiam_claudere(p))
                {
                    redde FALSUM;
                }
                redde _praefixum_pipae(p, t);
            }
            si (   t->valor.mensura == (i32)IV
                && memcmp(t->valor.datum, "time", (size_t)IV)
                    == ZEPHYRUM)
            {
                si (!_intra_sententiam_claudere(p))
                {
                    redde FALSUM;
                }
                redde _praefixum_pipae(p, t);
            }
            redde _malum(p, t);   /* constructa composita: P5 */
        ordinarius:
            redde _malum(p, t);
    }
}

/* FINIS intra regionem backtick: substitutio clauditur, backtick
 * clausurae petitur */
interior b32
_regionem_finire (
    Aedificatio* p)
{
    Gradus* g;
    MateriaToken* clausura;

    si (!_sententiam_claudere(p))
    {
        redde FALSUM;
    }
    g = _vertex(p);
    crusta_lector_regionem_claudere(&p->lector);
    si (g->genus != (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO || !g->gravis)
    {
        redde VERUM;   /* regio aliena (heredoc: P5) */
    }
    clausura = crusta_lector_proximum(&p->lector, CRUSTA_MODUS_VERBA);
    si (clausura == NIHIL)
    {
        redde FALSUM;
    }
    si (clausura->genus != (s32)CRUSTA_LEX_GRAVIS)
    {
        /* backtick clausurae absens (EOF): lexema alienum reddendum -
         * situs reponitur, substitutio clauditur absens */
        CrustaSitus s = crusta_lector_situs(&p->lector);

        s.cursor = clausura->byte_offset;
        crusta_lector_situm_reponere(&p->lector, s);
        g->gravis = FALSUM;
        redde _claudere(p);
    }
    si (   !_solvere(p, clausura)
        || !_token_ponere(g->nodus,
        (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA,
            clausura))
    {
        redde FALSUM;
    }
    redde _partem_claudere(p, clausura);
}

/* EOF: gradus omnes clausi, FINIS in caudam */
interior b32
_finem_tractare (
       Aedificatio* p,
      MateriaToken* t)
{
    Gradus* g;

    dum (   (g = _vertex(p)) != NIHIL
         && g->genus         != (s32)CRUSTA_GENUS_PROGRAMMA)
    {
        si (g->genus == (s32)CRUSTA_GENUS_VERBUM)
        {
            si (!_verbum_claudere(p))
            {
                redde FALSUM;
            }
        }
        alioquin si (g->genus == (s32)CRUSTA_GENUS_REDIRECTIO)
        {
            si (!_redirectionem_finire(p))
            {
                redde FALSUM;
            }
        }
        alioquin si (!_claudere(p))
        {
            redde FALSUM;
        }
    }
    redde _token_ponere(g->nodus, (i32)CRUSTA_PROGRAMMA_CAUDA, t);
}


/* ==================================================
 * Facies publica
 * ================================================== */

MateriaNodus*
crusta_arbor_parsare (
                     Piscina* piscina,
          constans character* fons,
                         i32  mensura,
    constans CrustaDialectus* dialectus,
               CrustaParsura* relatio)
{
             Aedificatio p;
      MateriaLexIudicium iudicium;
    MateriaNodus* programma;
    CrustaParsura relatio_propria;

    memset(&p, ZEPHYRUM, magnitudo(p));
    memset(&relatio_propria, ZEPHYRUM, magnitudo(relatio_propria));
    p.piscina    = piscina;
    p.dialectus  = dialectus;
    p.relatio    = relatio != NIHIL ? relatio : &relatio_propria;
    memset(p.relatio, ZEPHYRUM, magnitudo(*p.relatio));
    si (!materia_lexicon_ratum_facere(&p.lexicon, &CRUSTA_LEXICON,
            &iudicium))
    {
        redde NIHIL;
    }
    si (!crusta_lector_incipere(&p.lector, piscina, fons, mensura,
            dialectus))
    {
        redde NIHIL;
    }
    p.gradus     = xar_creare(piscina, (i32)magnitudo(Gradus));
    p.pendentia  = xar_creare(piscina, (i32)magnitudo(MateriaToken*));
    si (p.gradus == NIHIL || p.pendentia == NIHIL)
    {
        redde NIHIL;
    }
    programma = _nodus(&p, (s32)CRUSTA_GENUS_PROGRAMMA);
    si (   programma == NIHIL
        || _pellere(&p, programma, (i32)CRUSTA_PROGRAMMA_LIBERI,
            CRUSTA_MODUS_INITIUM) == NIHIL)
    {
        redde NIHIL;
    }

    dum (VERUM)
    {
              Gradus* g = _vertex(&p);
        MateriaToken* t = crusta_lector_proximum(&p.lector, g->modus);

        si (t == NIHIL || p.memoria_defecit)
        {
            redde NIHIL;
        }
        si (materia_lexicon_trivium_est(&p.lexicon, t->genus))
        {
            /* trivium verbum claudit, et assignationem sine valore
             * ('a= b': valor absens) */
            si (   g->genus == (s32)CRUSTA_GENUS_VERBUM
                && !_verbum_claudere(&p))
            {
                redde NIHIL;
            }
            g = _vertex(&p);
            si (   g->genus == (s32)CRUSTA_GENUS_ASSIGNATIO
                && !_claudere(&p))
            {
                redde NIHIL;
            }
            si (!_cumulare(&p, t))
            {
                redde NIHIL;
            }
            perge;
        }
        si (t->genus == (s32)CRUSTA_LEX_FINIS)
        {
            si (xar_numerus(p.lector.regiones) > ZEPHYRUM)
            {
                si (!_regionem_finire(&p))
                {
                    redde NIHIL;
                }
                perge;
            }
            si (!_solvere(&p, t) || !_finem_tractare(&p, t))
            {
                redde NIHIL;
            }
            frange;
        }
        si (!_solvere(&p, t) || !_tractare(&p, t))
        {
            redde NIHIL;
        }
    }
    p.relatio->sana = p.relatio->mala == ZEPHYRUM
                   && p.relatio->clausurae_absentes == ZEPHYRUM;
    redde programma;
}


/* ==================================================
 * Valor staticus verbi
 * ================================================== */

/* longitudo cruda subarboris (summa lexematum) */
interior i32
_longitudo_cruda (
    constans MateriaNodus* n)
{
    i32 summa = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < n->numerus_locorum; i++)
    {
        constans MateriaValor* v = &n->loci[i];

        si (v->genus == MATERIA_VALOR_TOKEN)
        {
            summa += v->datum.token->valor.mensura;
        }
        alioquin si (v->genus == MATERIA_VALOR_NODUS)
        {
            summa += _longitudo_cruda(v->datum.nodus);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*v); k++)
            {
                constans MateriaValor* e =
                    materia_valor_lista_obtinere(*v,
                    k);

                si (e->genus == MATERIA_VALOR_NODUS)
                {
                    summa += _longitudo_cruda(e->datum.nodus);
                }
                alioquin si (e->genus == MATERIA_VALOR_TOKEN)
                {
                    summa += e->datum.token->valor.mensura;
                }
            }
        }
    }
    redde summa;
}

/* retroversa logica ex cursu crudo n profunditatis d (regula
 * lectoris) */
interior i32
_retro_logica (
    i32 n,
    i32 gravis,
    b32 exuendum)
{
    i32 r = n;
    i32 k;

    per (k = ZEPHYRUM; k < gravis; k++)
    {
        r = exuendum ? r / (i32)II : (r + I) / (i32)II;
    }
    redde r;
}

/* effugium decodare in scriptorem; gemina: regulae "..." */
interior i32
_effugium_decoquere (
      constans MateriaToken* t,
                  character* exitus)
{
          i32 n       = ZEPHYRUM;
          i32 gravis  = crusta_lector_cauda(t)->gravis;
          i32 r;
          i32 scripti = ZEPHYRUM;
    character post;
          i32 k;

    dum (n < t->valor.mensura && t->valor.datum[n] == '\\')
    {
        n++;
    }
    si (n == t->valor.mensura)
    {
        /* cursus purus: paria effugia, impar litterale */
        r = _retro_logica(n, gravis, FALSUM);
        per (k = ZEPHYRUM; k < (r + I) / (i32)II; k++)
        {
            exitus[scripti++] = '\\';
        }
        redde scripti;
    }
    post  = (character)t->valor.datum[n];
    r     = _retro_logica(n, gravis, post == '$' || post == '`');
    per (k = ZEPHYRUM; k < r / (i32)II; k++)
    {
        exitus[scripti++] = '\\';
    }
    exitus[scripti++] = post;
    redde scripti;
}

interior i32
_effugia_decoquere (
      constans MateriaToken* t,
                  character* exitus)
{
    i32 i        = (i32)II;   /* post $' */
    i32 finis    = t->valor.mensura;
    i32 scripti  = ZEPHYRUM;

    si (finis > (i32)II && t->valor.datum[finis - I] == '\'')
    {
        finis--;
    }
    dum (i < finis)
    {
        character c = (character)t->valor.datum[i];

        si (c != '\\' || i + I >= finis)
        {
            exitus[scripti++] = c;
            i++;
            perge;
        }
        c = (character)t->valor.datum[i + I];
        i += II;
        commutatio (c)
        {
            casus 'n':  exitus[scripti++] = '\n'; frange;
            casus 't':  exitus[scripti++] = '\t'; frange;
            casus 'r':  exitus[scripti++] = '\r'; frange;
            casus 'a':  exitus[scripti++] = '\a'; frange;
            casus 'b':  exitus[scripti++] = '\b'; frange;
            casus 'f':  exitus[scripti++] = '\f'; frange;
            casus 'v':  exitus[scripti++] = '\v'; frange;
            casus 'e':
            casus 'E':  exitus[scripti++] = (character)27; frange;
            casus '\\': exitus[scripti++] = '\\'; frange;
            casus '\'': exitus[scripti++] = '\''; frange;
            casus '"':  exitus[scripti++] = '"'; frange;
            casus 'x':
            {
                i32 v = ZEPHYRUM;
                i32 d = ZEPHYRUM;

                dum (d < (i32)II && i < finis)
                {
                    character h = (character)t->valor.datum[i];
                          i32 valor;

                    si (h >= '0' && h <= '9')
                    {
                        valor = (i32)(h - '0');
                    }
                    alioquin si (h >= 'a' && h <= 'f')
                    {
                        valor = (i32)(h - 'a') + (i32)X;
                    }
                    alioquin si (h >= 'A' && h <= 'F')
                    {
                        valor = (i32)(h - 'A') + (i32)X;
                    }
                    alioquin
                    {
                        frange;
                    }
                    v = v * (i32)XVI + valor;
                    i++;
                    d++;
                }
                exitus[scripti++] = (character)v;
                frange;
            }
            ordinarius:
                si (c >= '0' && c <= '7')
                {
                    i32 v = (i32)(c - '0');
                    i32 d = I;

                    dum (   d < (i32)III && i < finis
                         && t->valor.datum[i] >= '0'
                         && t->valor.datum[i] <= '7')
                    {
                        v = v * (i32)VIII + (i32)(t->valor.datum[i]
                            - '0');
                        i++;
                        d++;
                    }
                    exitus[scripti++] = (character)v;
                }
                alioquin
                {
                    exitus[scripti++] = '\\';
                    exitus[scripti++] = c;
                }
                frange;
        }
    }
    redde scripti;
}

interior b32
_partes_decoquere (
    constans MateriaNodus* verbum,
                      i32  locus,
                character* exitus,
                      i32* scripti)
{
    constans MateriaValor* partes = &verbum->loci[locus];
                      i32  k;

    si (partes->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*partes); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*partes,
            k);
        constans MateriaNodus* pars;
        constans MateriaToken* t;

        si (e->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        pars = e->datum.nodus;
        commutatio (pars->genus)
        {
            casus CRUSTA_GENUS_PARS_LITTERALIS:
            casus CRUSTA_GENUS_PARS_DOMUS:
                t = pars->loci[CRUSTA_PARS_TOK].datum.token;
                memcpy(exitus + *scripti, t->valor.datum,
                    (size_t)t->valor.mensura);
                *scripti += t->valor.mensura;
                frange;
            casus CRUSTA_GENUS_PARS_EFFUGIUM:
                t = pars->loci[CRUSTA_PARS_TOK].datum.token;
                *scripti += _effugium_decoquere(t, exitus + *scripti);
                frange;
            casus CRUSTA_GENUS_PARS_CONTINUATIO:
                frange;
            casus CRUSTA_GENUS_PARS_SIMPLEX:
            {
                i32 ab = I;
                i32 ad;

                t   = pars->loci[CRUSTA_PARS_TOK].datum.token;
                ad  = t->valor.mensura;
                si (ad > I && t->valor.datum[ad - I] == '\'')
                {
                    ad--;
                }
                si (ad > ab)
                {
                    memcpy(exitus + *scripti, t->valor.datum + ab,
                        (size_t)(ad - ab));
                    *scripti += ad - ab;
                }
                frange;
            }
            casus CRUSTA_GENUS_PARS_EFFUGIA:
                t = pars->loci[CRUSTA_PARS_TOK].datum.token;
                *scripti += _effugia_decoquere(t, exitus + *scripti);
                frange;
            casus CRUSTA_GENUS_PARS_GEMINA:
            casus CRUSTA_GENUS_PARS_VERSA:
                si (!_partes_decoquere(pars, (i32)CRUSTA_GEMINA_PARTES,
                        exitus, scripti))
                {
                    redde FALSUM;
                }
                frange;
            ordinarius:
                redde FALSUM;   /* expansio: non staticum */
        }
    }
    redde VERUM;
}

b32
crusta_verbum_staticum (
                  Piscina* piscina,
    constans MateriaNodus* verbum,
                   chorda* valor)
{
          i32  longitudo;
    character* area;
          i32  scripti = ZEPHYRUM;

    si (verbum == NIHIL || verbum->genus != (s32)CRUSTA_GENUS_VERBUM)
    {
        redde FALSUM;
    }
    longitudo = _longitudo_cruda(verbum);
    area = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (area == NIHIL)
    {
        redde FALSUM;
    }
    si (!_partes_decoquere(verbum, (i32)CRUSTA_VERBUM_PARTES, area,
            &scripti))
    {
        redde FALSUM;
    }
    valor->mensura  = scripti;
    valor->datum    = (i8*)area;
    redde VERUM;
}

b32
crusta_verbum_citatum (
    constans MateriaNodus* verbum)
{
    constans MateriaValor* partes;
                      i32  k;

    si (verbum == NIHIL || verbum->genus != (s32)CRUSTA_GENUS_VERBUM)
    {
        redde FALSUM;
    }
    partes = &verbum->loci[CRUSTA_VERBUM_PARTES];
    si (partes->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*partes); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*partes,
            k);
                          s32 g;

        si (e->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        g = e->datum.nodus->genus;
        si (   g == (s32)CRUSTA_GENUS_PARS_SIMPLEX
            || g == (s32)CRUSTA_GENUS_PARS_GEMINA
            || g == (s32)CRUSTA_GENUS_PARS_EFFUGIUM
            || g == (s32)CRUSTA_GENUS_PARS_EFFUGIA
            || g == (s32)CRUSTA_GENUS_PARS_VERSA)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

constans MateriaNodus*
crusta_imperium_titulus (
    constans MateriaNodus* imperium)
{
    constans MateriaValor* liberi;
                      i32  k;

    si (   imperium        == NIHIL
        || imperium->genus != (s32)CRUSTA_GENUS_IMPERIUM)
    {
        redde NIHIL;
    }
    liberi = &imperium->loci[CRUSTA_IMPERIUM_LIBERI];
    si (liberi->genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*liberi); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*liberi,
            k);

        si (   e->genus              == MATERIA_VALOR_NODUS
            && e->datum.nodus->genus == (s32)CRUSTA_GENUS_VERBUM)
        {
            redde e->datum.nodus;
        }
    }
    redde NIHIL;
}

Xar*
crusta_imperium_argumenta (
                  Piscina* piscina,
    constans MateriaNodus* imperium)
{
                      Xar* exitus;
    constans MateriaNodus* titulus = crusta_imperium_titulus(imperium);
    constans MateriaValor* liberi;
                      b32  post_titulum = FALSUM;
                      i32  k;

    exitus = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));
    si (exitus == NIHIL || titulus == NIHIL)
    {
        redde exitus;
    }
    liberi = &imperium->loci[CRUSTA_IMPERIUM_LIBERI];
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*liberi); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*liberi,
            k);

        si (e->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        si (e->datum.nodus == titulus)
        {
            post_titulum = VERUM;
            perge;
        }
        si (   post_titulum
            && e->datum.nodus->genus == (s32)CRUSTA_GENUS_VERBUM)
        {
            MateriaNodus** sedes = (MateriaNodus**)xar_addere(exitus);

            si (sedes == NIHIL)
            {
                redde NIHIL;
            }
            *sedes = e->datum.nodus;
        }
    }
    redde exitus;
}
