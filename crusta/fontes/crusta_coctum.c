/* crusta_coctum.c - Visio cocta: forma normalis 'declare -f' bash
 * 5.2.15
 *
 * Vide crusta_coctum.h (duo modi, conexiones, corpora dilata). Regulae
 * MENSURATAE sunt (plan P11a, 'measured mechanics'); quaeque
 * discrepantia oraculi P11b regula hic fit, numquam in aedificatore.
 *
 * Nomina status ad morem machinae:
 *   saltus        indentationes proximae omittendae (mandatum in
 *                 eadem linea post '&&', '|', '&' cum secundo)
 *   conexio       profunditas conexionum quae nunc imprimuntur; corpora
 *                 heredoc intra conexionem DIFFERUNTUR
 *   functio       >0 = modus functionis (';' + linea nova, grex
 *                 multilinearis); 0 = modus substitutionis
 *   post_heredoc  corpora modo effusa: conexio ';' proxima tacet
 *                 (semel)
 */

#include "crusta_coctum.h"
#include "crusta_arbor.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "chorda_aedificator.h"
#include "materia_token.h"
#include "xar.h"
#include <string.h>

/* signa conexionum */
#define CONEXIO_ET        'A'
#define CONEXIO_AUT       'O'
#define CONEXIO_ERRORIS   'E'

/* vexilla emissionis crudae */
#define CRUDUM_SINE_CONTINUATIONE  ((i32)I)
#define CRUDUM_SUBSTITUTIONES      ((i32)II)

nomen structura {
                  Piscina* piscina;
        ChordaAedificator* aedificator;
    constans MateriaNodus* radix;
                      i32  indentatio;
                      i32  saltus;
                      i32  conexio;
                      i32  functio;
                      b32  substitutio;
                      Xar* dilata;     /* MateriaNodus* redirectio */
                      b32  post_heredoc;
                      b32  tabulae;    /* '<<-': tabulae initiales */
                      b32  initium_lineae;
                      b32  deficit;
} Coctor;

/* sententia cum signo conexionis quod eam sequitur (ZEPHYRUM =
 * nullum) */
nomen structura {
    constans MateriaNodus* nodus;
                character  signum;
} Membrum;


/* ==================================================
 * Scriptura
 * ================================================== */

interior vacuum
_octeti (
                Coctor* c,
           constans i8* datum,
                   i32  mensura)
{
    i32 k;

    per (k = ZEPHYRUM; k < mensura; k++)
    {
        character o = (character)datum[k];

        si (c->tabulae && c->initium_lineae && o == '\t')
        {
            perge;
        }
        c->initium_lineae = (o == '\n') ? VERUM : FALSUM;
        si (!chorda_aedificator_appendere_character(c->aedificator, o))
        {
            c->deficit = VERUM;
        }
    }
}

interior vacuum
_litterae (
                 Coctor* c,
     constans character* litterae)
{
    _octeti(c, (constans i8*)litterae, (i32)strlen(litterae));
}

interior vacuum
_chorda (
    Coctor* c,
    chorda  s)
{
    _octeti(c, s.datum, s.mensura);
}

interior vacuum
_spatia (
    Coctor* c,
       i32  numerus)
{
    si (   numerus > ZEPHYRUM
        && !chorda_aedificator_appendere_repetita(c->aedificator, ' ',
            numerus))
    {
        c->deficit = VERUM;
    }
    si (numerus > ZEPHYRUM)
    {
        c->initium_lineae = FALSUM;
    }
}

interior character
_ultimum (
    Coctor* c)
{
    chorda v = chorda_aedificator_spectare(c->aedificator);

    redde v.mensura > ZEPHYRUM ? (character)v.datum[v.mensura - I]
        : '\0';
}

/* initium mandati: saltum consumere aut indentare */
interior vacuum
_initium_mandati (
    Coctor* c)
{
    si (c->saltus > ZEPHYRUM)
    {
        c->saltus--;
    }
    alioquin
    {
        _spatia(c, c->indentatio);
    }
}

/* ';' post corpus nisi octetus ultimus '&' aut linea nova */
interior vacuum
_semicolon (
    Coctor* c)
{
    character u = _ultimum(c);

    si (u == '&' || u == '\n')
    {
        redde;
    }
    _litterae(c, ";");
}

interior vacuum
_linea_nova (
                 Coctor* c,
     constans character* litterae)
{
    _litterae(c, "\n");
    _spatia(c, c->indentatio);
    _litterae(c, litterae);
}


/* ==================================================
 * Accessus arboris
 * ================================================== */

interior constans MateriaToken*
_token (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (   nodus                    == NIHIL
        || nodus->loci[locus].genus != MATERIA_VALOR_TOKEN)
    {
        redde NIHIL;
    }
    redde nodus->loci[locus].datum.token;
}

interior constans MateriaNodus*
_nodus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (   nodus                    == NIHIL
        || nodus->loci[locus].genus != MATERIA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde nodus->loci[locus].datum.nodus;
}

interior b32
_valor_est (
    constans MateriaToken* t,
       constans character* litterae)
{
    i32 n = (i32)strlen(litterae);

    redde t != NIHIL && t->valor.mensura == n
        && memcmp(t->valor.datum, litterae, (size_t)n) == ZEPHYRUM;
}

interior vacuum
_valor (
                   Coctor* c,
    constans MateriaToken* t)
{
    si (t != NIHIL)
    {
        _chorda(c, t->valor);
    }
}

interior i32
_numerus (
    constans MateriaValor* lista)
{
    si (lista == NIHIL || lista->genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    redde materia_valor_lista_numerus(*lista);
}

interior constans MateriaNodus*
_elementum (
    constans MateriaValor* lista,
                      i32  index)
{
    constans MateriaValor* e = materia_valor_lista_obtinere(*lista,
        index);

    redde (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
        ? e->datum.nodus : NIHIL;
}


/* ==================================================
 * Emissio cruda (arithmetica, backtick, corpora heredoc)
 * ================================================== */

interior vacuum
_crudum (
                   Coctor* c,
    constans MateriaValor* valor,
                      i32  vexilla);

interior vacuum
_substitutio (
                   Coctor* c,
    constans MateriaValor* liberi);

interior vacuum
_substitutionem_scribere (
                   Coctor* c,
    constans MateriaNodus* pars);

interior vacuum
_lexema (
                  Coctor* c,
    constans MateriaToken* t,
                     b32  ante,
                     b32  post,
                     i32  vexilla)
{
    i32 k;

    si (t == NIHIL)
    {
        redde;
    }
    si (   (vexilla & CRUDUM_SINE_CONTINUATIONE)
        && t->genus == (s32)CRUSTA_LEX_CONTINUATIO)
    {
        redde;
    }
    si (ante)
    {
        per (k = ZEPHYRUM; k < t->numerus_ante; k++)
        {
            si (   !(vexilla & CRUDUM_SINE_CONTINUATIONE)
                || t->spatia_ante[k]->genus != (s32)CRUSTA_LEX_LAMINA)
            {
                _chorda(c, t->spatia_ante[k]->valor);
            }
        }
    }
    _chorda(c, t->valor);
    si (post)
    {
        per (k = ZEPHYRUM; k < t->numerus_post; k++)
        {
            si (   !(vexilla & CRUDUM_SINE_CONTINUATIONE)
                || t->spatia_post[k]->genus != (s32)CRUSTA_LEX_LAMINA)
            {
                _chorda(c, t->spatia_post[k]->valor);
            }
        }
    }
}

interior vacuum
_crudum_nodus (
                   Coctor* c,
    constans MateriaNodus* nodus,
                      i32  vexilla)
{
    i32 k;

    si (nodus == NIHIL || nodus->genus == (s32)CRUSTA_GENUS_HEREDOC)
    {
        redde;
    }
    si (   (vexilla & CRUDUM_SUBSTITUTIONES)
        && (   nodus->genus == (s32)CRUSTA_GENUS_PARS_PROCESSUS
            || (   nodus->genus == (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO
                && _token(nodus, (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA)
                   != NIHIL
                && _token(nodus, (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA)
                   ->genus != (s32)CRUSTA_LEX_GRAVIS)))
    {
        _substitutionem_scribere(c, nodus);
        redde;
    }
    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        _crudum(c, &nodus->loci[k], vexilla);
    }
}

interior vacuum
_crudum (
                   Coctor* c,
    constans MateriaValor* valor,
                      i32  vexilla)
{
    i32 k;

    commutatio (valor->genus)
    {
        casus MATERIA_VALOR_TOKEN:
            _lexema(c, valor->datum.token, VERUM, VERUM, vexilla);
            frange;
        casus MATERIA_VALOR_NODUS:
            _crudum_nodus(c, valor->datum.nodus, vexilla);
            frange;
        casus MATERIA_VALOR_LISTA:
            per (k = ZEPHYRUM; k
                < materia_valor_lista_numerus(*valor); k++)
            {
                _crudum(c, materia_valor_lista_obtinere(*valor, k),
                    vexilla);
            }
            frange;
        ordinarius:
            frange;
    }
}

/* nodus crudus sine trivia exteriore: ante lexematis primi et post
 * ultimi (loci extremi) omittuntur */
interior vacuum
_crudum_interius (
                   Coctor* c,
    constans MateriaNodus* nodus,
                      i32  vexilla)
{
    i32 k;
    i32 n = nodus->numerus_locorum;

    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* v = &nodus->loci[k];

        si (v->genus == MATERIA_VALOR_TOKEN)
        {
            _lexema(c, v->datum.token, k != ZEPHYRUM, k + I != n,
                vexilla);
        }
        alioquin
        {
            _crudum(c, v, vexilla);
        }
    }
}


/* ==================================================
 * Verba
 * ================================================== */

interior vacuum
_partes (
                   Coctor* c,
    constans MateriaValor* partes);

/* lexema primum valoris (ordine locorum; heredoc omissa) */
interior constans MateriaToken*
_lexema_primum (
    constans MateriaValor* valor)
{
    constans MateriaToken* t = NIHIL;
                      i32  k;

    commutatio (valor->genus)
    {
        casus MATERIA_VALOR_TOKEN:
            redde valor->datum.token;
        casus MATERIA_VALOR_NODUS:
            si (   valor->datum.nodus == NIHIL
                || valor->datum.nodus->genus
                    == (s32)CRUSTA_GENUS_HEREDOC)
            {
                redde NIHIL;
            }
            per (k = ZEPHYRUM; k < valor->datum.nodus->numerus_locorum
                && t == NIHIL; k++)
            {
                t = _lexema_primum(&valor->datum.nodus->loci[k]);
            }
            redde t;
        casus MATERIA_VALOR_LISTA:
            per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor)
                && t == NIHIL; k++)
            {
                t = _lexema_primum(materia_valor_lista_obtinere(*valor,
                    k));
            }
            redde t;
        ordinarius:
            redde NIHIL;
    }
}

/* aedificator seorsum: textus ante scripturam inspiciendus */
interior ChordaAedificator*
_seorsum_incipere (
    Coctor* c)
{
    ChordaAedificator* seorsum = chorda_aedificator_creare(c->piscina,
        (memoriae_index)256);

    si (seorsum == NIHIL)
    {
        c->deficit = VERUM;
    }
    redde seorsum;
}

/* '$( )' aut '<( )' '>( )': textus reimpressus; si a '(' incipit
 * spatium post aperturam ('$( ( a ))'). Fons '$((' (arithmetica
 * recusata, lector retro): bash textum VERBATIM servat ('$((a); b)'
 * idem) - P11b mensuratum */
interior vacuum
_substitutionem_scribere (
                   Coctor* c,
    constans MateriaNodus* pars)
{
    constans MateriaToken* apertura    = _token(pars,
        (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA);
    constans MateriaToken* primum      = _lexema_primum(
        &pars->loci[CRUSTA_SUBSTITUTIO_LIBERI]);
         ChordaAedificator* principalis = c->aedificator;
         ChordaAedificator* seorsum;
                    chorda  textus;

    si (   pars->genus == (s32)CRUSTA_GENUS_PARS_SUBSTITUTIO
        && apertura != NIHIL && primum != NIHIL
        && primum->byte_offset
           == apertura->byte_offset + (s32)apertura->valor.mensura
        && primum->valor.mensura > ZEPHYRUM
        && primum->valor.datum[ZEPHYRUM] == '(')
    {
        _crudum_interius(c, pars, ZEPHYRUM);
        redde;
    }
    seorsum = _seorsum_incipere(c);
    si (seorsum == NIHIL)
    {
        redde;
    }
    c->aedificator = seorsum;
    _substitutio(c, &pars->loci[CRUSTA_SUBSTITUTIO_LIBERI]);
    c->aedificator  = principalis;
    textus          = chorda_aedificator_spectare(seorsum);
    _valor(c, apertura);
    si (textus.mensura > ZEPHYRUM && textus.datum[ZEPHYRUM] == '(')
    {
        _litterae(c, " ");
    }
    _chorda(c, textus);
    _valor(c, _token(pars, (i32)CRUSTA_SUBSTITUTIO_TOK_CLAUSURA));
}

/* backtick verbatim sed '\' + linea nova abiecta ubique (etiam intra
 * apices simplices) ubi cursus backslash impar est - bash ea legendo
 * tollit (P11b mensuratum; cursus par manet) */
interior vacuum
_gravem_scribere (
                   Coctor* c,
    constans MateriaNodus* pars)
{
     ChordaAedificator* principalis  = c->aedificator;
     ChordaAedificator* seorsum      = _seorsum_incipere(c);
                chorda  s;
                   i32  k = ZEPHYRUM;

    si (seorsum == NIHIL)
    {
        redde;
    }
    c->aedificator = seorsum;
    _crudum_interius(c, pars, ZEPHYRUM);
    c->aedificator  = principalis;
    s               = chorda_aedificator_spectare(seorsum);
    dum (k < s.mensura)
    {
        i32 cursus = ZEPHYRUM;

        si (s.datum[k] != '\\')
        {
            _octeti(c, s.datum + k, I);
            k++;
            perge;
        }
        dum (k + cursus < s.mensura && s.datum[k + cursus] == '\\')
        {
            cursus++;
        }
        si (   k + cursus < s.mensura && s.datum[k + cursus] == '\n'
            && (cursus % (i32)II) == (i32)I)
        {
            _octeti(c, s.datum + k, cursus - I);
            k += cursus + I;
        }
        alioquin
        {
            _octeti(c, s.datum + k, cursus);
            k += cursus;
        }
    }
}

/* '$'...'' decoctum, apicibus simplicibus reimpressum ('\'' pro ') */
interior vacuum
_effugia (
                   Coctor* c,
    constans MateriaNodus* pars)
{
    chorda valor;
       i32 k;

    si (!crusta_effugia_decoquere(c->piscina, pars, &valor))
    {
        _crudum_nodus(c, pars, ZEPHYRUM);
        redde;
    }
    _litterae(c, "'");
    per (k = ZEPHYRUM; k < valor.mensura; k++)
    {
        character o = (character)valor.datum[k];

        si (o == '\0')
        {
            frange;
        }
        si (o == '\'')
        {
            _litterae(c, "'\\''");
        }
        alioquin
        {
            _octeti(c, valor.datum + k, I);
        }
    }
    _litterae(c, "'");
}

interior vacuum
_pars (
                   Coctor* c,
    constans MateriaNodus* pars)
{
    constans MateriaToken* apertura;

    commutatio (pars->genus)
    {
        casus CRUSTA_GENUS_PARS_LITTERALIS:
        casus CRUSTA_GENUS_PARS_DOMUS:
        casus CRUSTA_GENUS_PARS_EFFUGIUM:
        casus CRUSTA_GENUS_PARS_SIMPLEX:
            _valor(c, _token(pars, (i32)CRUSTA_PARS_TOK));
            frange;
        casus CRUSTA_GENUS_PARS_CONTINUATIO:
            frange;
        casus CRUSTA_GENUS_PARS_PARAMETRUM:
            _valor(c, _token(pars,
                (i32)CRUSTA_PARAMETRUM_TOK_SIGILLUM));
            _valor(c, _token(pars, (i32)CRUSTA_PARAMETRUM_TOK_TITULUS));
            frange;
        casus CRUSTA_GENUS_PARS_EFFUGIA:
            _effugia(c, pars);
            frange;
        casus CRUSTA_GENUS_PARS_GEMINA:
            _valor(c, _token(pars, (i32)CRUSTA_GEMINA_TOK_APERTURA));
            _partes(c, &pars->loci[CRUSTA_GEMINA_PARTES]);
            _valor(c, _token(pars, (i32)CRUSTA_GEMINA_TOK_CLAUSURA));
            frange;
        casus CRUSTA_GENUS_PARS_VERSA:
            /* '$"..."' sine sigillo (locale non vertitur) */
            _litterae(c, "\"");
            _partes(c, &pars->loci[CRUSTA_GEMINA_PARTES]);
            _valor(c, _token(pars, (i32)CRUSTA_GEMINA_TOK_CLAUSURA));
            frange;
        casus CRUSTA_GENUS_PARS_EXPANSIO:
            _crudum_interius(c, pars,
                CRUDUM_SINE_CONTINUATIONE | CRUDUM_SUBSTITUTIONES);
            frange;
        casus CRUSTA_GENUS_PARS_SUBSTITUTIO:
            apertura = _token(pars,
                (i32)CRUSTA_SUBSTITUTIO_TOK_APERTURA);
            si (   apertura        != NIHIL
                && apertura->genus == (s32)CRUSTA_LEX_GRAVIS)
            {
                _gravem_scribere(c, pars);
                frange;
            }
            _substitutionem_scribere(c, pars);
            frange;
        casus CRUSTA_GENUS_PARS_PROCESSUS:
            _substitutionem_scribere(c, pars);
            frange;
        ordinarius:
            /* pars-arithmetica et ignota: verbatim */
            _crudum_interius(c, pars, CRUDUM_SINE_CONTINUATIONE);
            frange;
    }
}

interior vacuum
_partes (
                   Coctor* c,
    constans MateriaValor* partes)
{
    i32 k;

    per (k = ZEPHYRUM; k < _numerus(partes); k++)
    {
        constans MateriaNodus* pars = _elementum(partes, k);

        si (pars != NIHIL)
        {
            _pars(c, pars);
        }
    }
}

interior vacuum
_verbum (
                   Coctor* c,
    constans MateriaNodus* verbum)
{
    si (verbum == NIHIL)
    {
        redde;
    }
    si (verbum->genus != (s32)CRUSTA_GENUS_VERBUM)
    {
        _crudum_interius(c, verbum, CRUDUM_SINE_CONTINUATIONE);
        redde;
    }
    _partes(c, &verbum->loci[CRUSTA_VERBUM_PARTES]);
}


/* ==================================================
 * Redirectiones et corpora heredoc
 * ================================================== */

interior b32
_heredoc_est (
    constans MateriaNodus* redirectio)
{
    constans MateriaToken* operator = _token(redirectio,
        (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR);

    redde _valor_est(operator, "<<") || _valor_est(operator, "<<-");
}

/* delimitator: citatus -> 'valor decoctus'; aliter verbum ipsum */
interior vacuum
_delimitator (
                   Coctor* c,
    constans MateriaNodus* scopus,
                      b32  apices)
{
    chorda valor;

    si (!crusta_verbum_staticum(c->piscina, scopus, &valor))
    {
        _verbum(c, scopus);
        redde;
    }
    si (apices && crusta_verbum_citatum(scopus))
    {
        _litterae(c, "'");
        _chorda(c, valor);
        _litterae(c, "'");
        redde;
    }
    si (apices)
    {
        _verbum(c, scopus);
        redde;
    }
    _chorda(c, valor);
}

/* corpus + delimitator (sine linea nova finali) */
interior vacuum
_heredoc_corpus (
                   Coctor* c,
    constans MateriaNodus* redirectio)
{
    constans MateriaValor* corpus = &redirectio->loci[
        CRUSTA_REDIRECTIO_CORPUS];
    constans MateriaNodus* scopus = _nodus(redirectio,
        (i32)CRUSTA_REDIRECTIO_SCOPUS);
    constans MateriaNodus* heredoc = NIHIL;
                      i32  vexilla = CRUDUM_SINE_CONTINUATIONE;

    si (   corpus->genus       == MATERIA_VALOR_REFERENTIA
        && corpus->datum.nodus != NIHIL)
    {
        heredoc = corpus->datum.nodus;
    }
    si (crusta_verbum_citatum(scopus))
    {
        vexilla = ZEPHYRUM;
    }
    c->tabulae        = _valor_est(_token(redirectio,
        (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR), "<<-");
    c->initium_lineae = VERUM;
    si (heredoc != NIHIL)
    {
        _crudum(c, &heredoc->loci[CRUSTA_HEREDOC_PARTES], vexilla);
    }
    _delimitator(c, scopus, FALSUM);
    c->tabulae = FALSUM;
}

/* PRINT_DEFERRED: signum (nisi vacuum aut ';' nudum), dein corpora
 * dilata lineis suis et spatium */
interior vacuum
_dilata_effundere (
                 Coctor* c,
     constans character* signum)
{
    b32 scribendum = (signum[ZEPHYRUM] != '\0'
        && !(signum[ZEPHYRUM] == ';' && signum[I] == '\0'))
        ? VERUM : FALSUM;
    i32 n          = xar_numerus(c->dilata);
    i32 k;

    si (scribendum)
    {
        _litterae(c, signum);
    }
    si (n == ZEPHYRUM)
    {
        redde;
    }
    _litterae(c, "\n");
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaNodus** r = (constans MateriaNodus**)
            xar_obtinere(c->dilata, k);

        _heredoc_corpus(c, *r);
        _litterae(c, "\n");
    }
    si (scribendum)
    {
        _litterae(c, " ");
    }
    xar_vacare(c->dilata);
    c->post_heredoc = VERUM;
}

interior vacuum
_dilata_si (
                 Coctor* c,
     constans character* signum)
{
    si (xar_numerus(c->dilata) > ZEPHYRUM)
    {
        _dilata_effundere(c, signum);
    }
}

interior b32
_digiti (
    chorda s,
       i32 initium,
       i32 finis)
{
    i32 k;

    si (finis <= initium)
    {
        redde FALSUM;
    }
    per (k = initium; k < finis; k++)
    {
        si (s.datum[k] < '0' || s.datum[k] > '9')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_redirectio (
                   Coctor* c,
    constans MateriaNodus* r)
{
    constans MateriaToken* descriptor     = _token(r,
        (i32)CRUSTA_REDIRECTIO_TOK_FD);
    constans MateriaToken* operator     = _token(r,
        (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR);
    constans MateriaNodus* scopus = _nodus(r,
        (i32)CRUSTA_REDIRECTIO_SCOPUS);
                   chorda valor;

    si (   _valor_est(operator, ">") || _valor_est(operator, ">>")
        || _valor_est(operator, ">|"))
    {
        si (descriptor != NIHIL && !_valor_est(descriptor, "1"))
        {
            _valor(c, descriptor);
        }
        _valor(c, operator);
        _litterae(c, " ");
        _verbum(c, scopus);
    }
    alioquin si (   _valor_est(operator, "<")
                 || _valor_est(operator, "<<<"))
    {
        si (descriptor != NIHIL && !_valor_est(descriptor, "0"))
        {
            _valor(c, descriptor);
        }
        _valor(c, operator);
        _litterae(c, " ");
        _verbum(c, scopus);
    }
    alioquin si (_valor_est(operator, "<>"))
    {
        si (descriptor != NIHIL)
        {
            _valor(c, descriptor);
        }
        alioquin
        {
            _litterae(c, "0");
        }
        _litterae(c, "<> ");
        _verbum(c, scopus);
    }
    alioquin si (   _valor_est(operator, "&>")
                 || _valor_est(operator, "&>>"))
    {
        _valor(c, operator);
        _litterae(c, " ");
        _verbum(c, scopus);
    }
    alioquin si (   _valor_est(operator, "<<")
                 || _valor_est(operator, "<<-"))
    {
        si (descriptor != NIHIL && !_valor_est(descriptor, "0"))
        {
            _valor(c, descriptor);
        }
        _valor(c, operator);
        _delimitator(c, scopus, VERUM);
    }
    alioquin si (   _valor_est(operator, ">&")
                 || _valor_est(operator, "<&"))
    {
        b32 numerica = FALSUM;
        b32 claudens = FALSUM;

        si (crusta_verbum_staticum(c->piscina, scopus, &valor))
        {
            claudens  = (valor.mensura == I
                && valor.datum[ZEPHYRUM] == '-')
                ? VERUM : FALSUM;
            numerica = (   _digiti(valor, ZEPHYRUM, valor.mensura)
                         || (   valor.mensura > I
                             && valor.datum[valor.mensura - I] == '-'
                             && _digiti(valor, ZEPHYRUM,
                                 valor.mensura - I)))
                ? VERUM : FALSUM;
        }
        si (claudens || numerica)
        {
            si (descriptor != NIHIL)
            {
                _valor(c, descriptor);
            }
            alioquin
            {
                _litterae(c, _valor_est(operator, ">&") ? "1" : "0");
            }
            /* '<&-' ut '>&-' imprimitur (mensuratum) */
            _litterae(c, claudens ? ">&" : "");
            si (!claudens)
            {
                _valor(c, operator);
            }
            _chorda(c, valor);
        }
        alioquin
        {
            _valor(c, descriptor);
            _valor(c, operator);
            _verbum(c, scopus);
        }
    }
    alioquin
    {
        _valor(c, descriptor);
        _valor(c, operator);
        _verbum(c, scopus);
    }
}

/* redirectiones ordine; '|&' = '2>&1' in fine; corpora heredoc dilata
 * intra conexionem, aliter statim effusa */
interior vacuum
_redirectiones (
                   Coctor* c,
    constans MateriaValor* lista,
                      b32  erroris)
{
    i32 k;
    b32 primus    = VERUM;
    i32 heredoca  = ZEPHYRUM;
    i32 basis     = xar_numerus(c->dilata);

    c->post_heredoc = FALSUM;
    per (k = ZEPHYRUM; k < _numerus(lista); k++)
    {
        constans MateriaNodus* r = _elementum(lista, k);

        si (r == NIHIL || r->genus != (s32)CRUSTA_GENUS_REDIRECTIO)
        {
            perge;
        }
        si (!primus)
        {
            _litterae(c, " ");
        }
        primus = FALSUM;
        _redirectio(c, r);
        si (_heredoc_est(r))
        {
            constans MateriaNodus** sedes =
                (constans MateriaNodus**)xar_addere(c->dilata);

            si (sedes == NIHIL)
            {
                c->deficit = VERUM;
                redde;
            }
            *sedes = r;
            heredoca++;
        }
    }
    si (erroris)
    {
        si (!primus)
        {
            _litterae(c, " ");
        }
        _litterae(c, "2>&1");
    }
    si (heredoca == ZEPHYRUM || c->conexio > ZEPHYRUM)
    {
        redde;
    }
    _litterae(c, "\n");
    per (k = basis; k < xar_numerus(c->dilata); k++)
    {
        constans MateriaNodus** r = (constans MateriaNodus**)
            xar_obtinere(c->dilata, k);

        _heredoc_corpus(c, *r);
        _litterae(c, "\n");
    }
    xar_truncare(c->dilata, basis);
    c->post_heredoc = VERUM;
}

interior b32
_redirectiones_adsunt (
    constans MateriaValor* lista)
{
    i32 k;

    per (k = ZEPHYRUM; k < _numerus(lista); k++)
    {
        constans MateriaNodus* r = _elementum(lista, k);

        si (r != NIHIL && r->genus == (s32)CRUSTA_GENUS_REDIRECTIO)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}


/* ==================================================
 * Listae et conexiones
 * ================================================== */

interior vacuum
_mandatum (
                   Coctor* c,
    constans MateriaNodus* nodus,
                      b32  erroris);

interior vacuum
_conectere (
       Coctor* c,
    character  signum,
          b32  secundum)
{
    commutatio (signum)
    {
        casus '&':
        casus '|':
            _dilata_effundere(c, signum == '&' ? " &" : " |");
            si (signum != '&' || secundum)
            {
                _litterae(c, " ");
                c->saltus++;
            }
            frange;
        casus CONEXIO_ET:
        casus CONEXIO_AUT:
            _dilata_effundere(c, signum
                == CONEXIO_ET ? " && " : " || ");
            si (secundum)
            {
                c->saltus++;
            }
            frange;
        ordinarius:
            /* ';' aut linea nova */
            si (xar_numerus(c->dilata) == ZEPHYRUM)
            {
                si (!c->post_heredoc)
                {
                    _litterae(c, (c->substitutio && signum == '\n')
                        ? "\n" : ";");
                }
                alioquin
                {
                    c->post_heredoc = FALSUM;
                }
            }
            alioquin
            {
                _dilata_effundere(c, c->functio > ZEPHYRUM ? "" : ";");
            }
            si (c->functio > ZEPHYRUM)
            {
                _litterae(c, "\n");
            }
            alioquin
            {
                si (signum != '\n')
                {
                    _litterae(c, " ");
                }
                si (secundum)
                {
                    c->saltus++;
                }
            }
            frange;
    }
}

/* sententiae listae (aut catenae, pipae) cum signis conexionis;
 * separatores, operatores, heredoc et mala non sunt membra */
interior Xar*
_membra (
                   Coctor* c,
    constans MateriaValor* lista)
{
    Xar* membra = xar_creare(c->piscina, (i32)magnitudo(Membrum));
    i32  k;

    si (membra == NIHIL)
    {
        c->deficit = VERUM;
        redde NIHIL;
    }
    si (   lista->genus       == MATERIA_VALOR_NODUS
        && lista->datum.nodus != NIHIL)
    {
        Membrum* m = (Membrum*)xar_addere(membra);

        si (m != NIHIL)
        {
            m->nodus   = lista->datum.nodus;
            m->signum  = '\0';
        }
        redde membra;
    }
    per (k = ZEPHYRUM; k < _numerus(lista); k++)
    {
        constans MateriaNodus* e = _elementum(lista, k);
        constans MateriaToken* t;
                      Membrum* ultimus;
                    character  signum = '\0';

        si (e == NIHIL)
        {
            perge;
        }
        commutatio (e->genus)
        {
            casus CRUSTA_GENUS_HEREDOC:
            casus CRUSTA_GENUS_MALUM:
                perge;
            casus CRUSTA_GENUS_SEPARATOR:
                t = _token(e, (i32)CRUSTA_SEPARATOR_TOK);
                si (   t != NIHIL && t->genus
                    == (s32)CRUSTA_LEX_SEPARATOR_LINEAE)
                {
                    signum = '\n';
                }
                alioquin si (t != NIHIL && t->valor.mensura > ZEPHYRUM)
                {
                    signum = (character)t->valor.datum[ZEPHYRUM];
                }
                frange;
            casus CRUSTA_GENUS_OPERATOR:
                t = _token(e, (i32)CRUSTA_OPERATOR_TOK);
                si (_valor_est(t, "&&"))
                {
                    signum = CONEXIO_ET;
                }
                alioquin si (_valor_est(t, "||"))
                {
                    signum = CONEXIO_AUT;
                }
                alioquin si (_valor_est(t, "|&"))
                {
                    signum = CONEXIO_ERRORIS;
                }
                alioquin
                {
                    signum = '|';
                }
                frange;
            ordinarius:
            {
                Membrum* m = (Membrum*)xar_addere(membra);

                si (m == NIHIL)
                {
                    c->deficit = VERUM;
                    redde membra;
                }
                m->nodus   = e;
                m->signum  = '\0';
                perge;
            }
        }
        si (xar_numerus(membra) == ZEPHYRUM)
        {
            perge;
        }
        ultimus = (Membrum*)xar_obtinere(membra,
            xar_numerus(membra) - I);
        si (ultimus->signum == '\0')
        {
            ultimus->signum = signum;
        }
    }
    redde membra;
}

/* lista sententiarum: conexiones SINISTRORSUM nidificatae; '&' finalis
 * post ';' ultimo mandato ligatur */
interior vacuum
_lista (
                   Coctor* c,
    constans MateriaValor* lista)
{
          Xar* membra = _membra(c, lista);
          i32  n;
          i32  k;
          i32  gradus;
    character  ultimum;
    character  signum_prius;
          b32  ultimum_seorsum;
          b32  totum_seorsum;

    si (membra == NIHIL || xar_numerus(membra) == ZEPHYRUM)
    {
        redde;
    }
    n        = xar_numerus(membra);
    ultimum  = ((Membrum*)xar_obtinere(membra, n - I))->signum;
    si (n == I && ultimum != '&')
    {
        _mandatum(c, ((Membrum*)xar_obtinere(membra, ZEPHYRUM))->nodus,
            FALSUM);
        redde;
    }
    signum_prius = n > I
        ? ((Membrum*)xar_obtinere(membra, n - II))->signum : '\0';
    ultimum_seorsum = (ultimum == '&' && n > I
        && (signum_prius == ';' || signum_prius == '\n'
            || signum_prius == '\0')) ? VERUM : FALSUM;
    totum_seorsum  = (ultimum == '&' && !ultimum_seorsum)
        ? VERUM : FALSUM;
    gradus = (n - I) + (totum_seorsum ? I : ZEPHYRUM);

    _initium_mandati(c);
    c->saltus++;
    c->conexio += gradus;
    _mandatum(c, ((Membrum*)xar_obtinere(membra, ZEPHYRUM))->nodus,
        FALSUM);
    per (k = I; k < n; k++)
    {
        character signum = ((Membrum*)xar_obtinere(membra, k
            - I))->signum;
        constans MateriaNodus* nodus =
            ((Membrum*)xar_obtinere(membra, k))->nodus;

        si (signum == '\0')
        {
            signum = ';';
        }
        _conectere(c, signum, VERUM);
        si (k == n - I && ultimum_seorsum)
        {
            _initium_mandati(c);
            c->saltus++;
            c->conexio++;
            _mandatum(c, nodus, FALSUM);
            _conectere(c, '&', FALSUM);
            _dilata_si(c, "");
            c->conexio--;
        }
        alioquin
        {
            _mandatum(c, nodus, FALSUM);
        }
        _dilata_si(c, "");
        c->conexio--;
    }
    si (totum_seorsum)
    {
        _conectere(c, '&', FALSUM);
        _dilata_si(c, "");
        c->conexio--;
    }
}

/* catena: '&&' '||' SINISTRORSUM */
interior vacuum
_catena (
                   Coctor* c,
    constans MateriaNodus* catena)
{
    Xar* membra = _membra(c, &catena->loci[CRUSTA_CATENA_LIBERI]);
    i32  n;
    i32  k;

    si (membra == NIHIL || xar_numerus(membra) == ZEPHYRUM)
    {
        redde;
    }
    n = xar_numerus(membra);
    si (n == I)
    {
        _mandatum(c, ((Membrum*)xar_obtinere(membra, ZEPHYRUM))->nodus,
            FALSUM);
        redde;
    }
    _initium_mandati(c);
    c->saltus++;
    c->conexio += n - I;
    _mandatum(c, ((Membrum*)xar_obtinere(membra, ZEPHYRUM))->nodus,
        FALSUM);
    per (k = I; k < n; k++)
    {
        character signum = ((Membrum*)xar_obtinere(membra, k
            - I))->signum;

        _conectere(c, signum == CONEXIO_AUT ? CONEXIO_AUT : CONEXIO_ET,
            VERUM);
        _mandatum(c, ((Membrum*)xar_obtinere(membra, k))->nodus,
            FALSUM);
        _dilata_si(c, "");
        c->conexio--;
    }
}

/* pipa: 'time [-p]' ante '!' ('!' commutat); membra DEXTRORSUM; '|&' =
 * '2>&1' mandato sinistro + '|' */
interior vacuum
_pipa (
                   Coctor* c,
    constans MateriaNodus* pipa)
{
    constans MateriaValor* praefixa = &pipa->loci[CRUSTA_PIPA_PRAEFIXA];
                      Xar* membra;
                      b32  tempus   = FALSUM;
                      b32  posix    = FALSUM;
                      b32  negatio  = FALSUM;
                      i32  n;
                      i32  k;

    _initium_mandati(c);
    per (k = ZEPHYRUM; k < _numerus(praefixa); k++)
    {
        constans MateriaValor* e =
            materia_valor_lista_obtinere(*praefixa,
            k);

        si (e->genus != MATERIA_VALOR_TOKEN)
        {
            perge;
        }
        si (_valor_est(e->datum.token, "time"))
        {
            tempus = VERUM;
        }
        alioquin si (_valor_est(e->datum.token, "-p"))
        {
            posix = VERUM;
        }
        alioquin si (_valor_est(e->datum.token, "!"))
        {
            negatio = negatio ? FALSUM : VERUM;
        }
    }
    si (tempus)
    {
        _litterae(c, posix ? "time -p " : "time ");
    }
    si (negatio)
    {
        _litterae(c, "! ");
    }
    membra = _membra(c, &pipa->loci[CRUSTA_PIPA_LIBERI]);
    si (membra == NIHIL || xar_numerus(membra) == ZEPHYRUM)
    {
        redde;
    }
    n = xar_numerus(membra);
    si (n == I)
    {
        c->saltus++;
        _mandatum(c, ((Membrum*)xar_obtinere(membra, ZEPHYRUM))->nodus,
            FALSUM);
        redde;
    }
    per (k = ZEPHYRUM; k + I < n; k++)
    {
        Membrum* m = (Membrum*)xar_obtinere(membra, k);

        si (k > ZEPHYRUM)
        {
            _initium_mandati(c);
        }
        c->saltus++;
        c->conexio++;
        _mandatum(c, m->nodus,
            m->signum == CONEXIO_ERRORIS ? VERUM : FALSUM);
        _conectere(c, '|', VERUM);
    }
    _mandatum(c, ((Membrum*)xar_obtinere(membra, n - I))->nodus,
        FALSUM);
    per (k = ZEPHYRUM; k + I < n; k++)
    {
        _dilata_si(c, "");
        c->conexio--;
    }
}


/* ==================================================
 * Mandata composita
 * ================================================== */

interior vacuum
_substitutio (
                   Coctor* c,
    constans MateriaValor* liberi)
{
    Coctor servatus = *c;
       b32 deficit;

    c->indentatio    = ZEPHYRUM;
    c->saltus        = ZEPHYRUM;
    c->conexio       = ZEPHYRUM;
    c->functio       = ZEPHYRUM;
    c->substitutio   = VERUM;
    c->post_heredoc  = FALSUM;
    c->tabulae       = FALSUM;
    c->dilata        = xar_creare(c->piscina,
        (i32)magnitudo(MateriaNodus*));
    si (c->dilata == NIHIL)
    {
        servatus.deficit  = VERUM;
        *c                = servatus;
        redde;
    }
    _lista(c, liberi);
    _dilata_si(c, "");
    deficit  = c->deficit;
    *c       = servatus;
    si (deficit)
    {
        c->deficit = VERUM;
    }
}

interior vacuum
_grex (
                   Coctor* c,
    constans MateriaNodus* grex)
{
    si (c->functio > ZEPHYRUM)
    {
        _litterae(c, "{ \n");
        c->indentatio += IV;
    }
    alioquin
    {
        _litterae(c, "{ ");
        c->saltus++;
    }
    _lista(c, &grex->loci[CRUSTA_GREX_LIBERI]);
    _dilata_si(c, "");
    si (c->functio > ZEPHYRUM)
    {
        _litterae(c, "\n");
        c->indentatio -= IV;
        _spatia(c, c->indentatio);
    }
    alioquin
    {
        _semicolon(c);
        _litterae(c, " ");
    }
    _litterae(c, "}");
}

interior vacuum
_functio (
                   Coctor* c,
    constans MateriaNodus* functio)
{
    constans MateriaNodus* corpus = _nodus(functio,
        (i32)CRUSTA_FUNCTIO_CORPUS);
                      b32 grex   = (   corpus != NIHIL
                                     && corpus->genus
                                        == (s32)CRUSTA_GENUS_GREX)
                                    ? VERUM : FALSUM;

    si (   c->radix        == NIHIL
        || c->radix->genus != (s32)CRUSTA_GENUS_PROGRAMMA
        || functio->pater  != c->radix)
    {
        _litterae(c, "function ");
    }
    _valor(c, _token(functio, (i32)CRUSTA_FUNCTIO_TOK_TITULUS));
    _litterae(c, " () \n");
    _spatia(c, c->indentatio);
    _litterae(c, "{ \n");
    c->functio++;
    c->indentatio += IV;
    si (grex)
    {
        _lista(c, &corpus->loci[CRUSTA_GREX_LIBERI]);
    }
    alioquin
    {
        _mandatum(c, corpus, FALSUM);
    }
    _dilata_si(c, "");
    c->indentatio -= IV;
    c->functio--;
    _litterae(c, "\n");
    _spatia(c, c->indentatio);
    _litterae(c, "}");
    /* definitio functionis vexillum heredoc tollit ('};' post corpus
     * effusum; grex non tollit - P11b mensuratum) */
    c->post_heredoc = FALSUM;
    si (   grex && _redirectiones_adsunt(&corpus->loci[
        CRUSTA_GREX_REDIRECTIONES]))
    {
        _litterae(c, " ");
        _redirectiones(c, &corpus->loci[CRUSTA_GREX_REDIRECTIONES],
            FALSUM);
    }
}

/* if; elif = else + if nidificatum */
interior vacuum
_conditio (
                   Coctor* c,
    constans MateriaValor* probatio,
    constans MateriaValor* liberi,
    constans MateriaValor* rami,
                      i32  index)
{
    _litterae(c, "if ");
    c->saltus++;
    _lista(c, probatio);
    _dilata_si(c, "");
    _semicolon(c);
    _litterae(c, " then\n");
    c->indentatio += IV;
    _lista(c, liberi);
    _dilata_si(c, "");
    c->indentatio -= IV;
    _semicolon(c);
    si (index < _numerus(rami))
    {
        constans MateriaNodus* ramus = _elementum(rami, index);

        _linea_nova(c, "else\n");
        c->indentatio += IV;
        si (   ramus != NIHIL
            && _valor_est(_token(ramus, (i32)CRUSTA_RAMUS_TOK_APERTURA),
                "elif"))
        {
            _initium_mandati(c);
            _conditio(c, &ramus->loci[CRUSTA_RAMUS_PROBATIO],
                &ramus->loci[CRUSTA_RAMUS_LIBERI], rami, index + I);
        }
        alioquin si (ramus != NIHIL)
        {
            _lista(c, &ramus->loci[CRUSTA_RAMUS_LIBERI]);
            _dilata_si(c, "");
        }
        c->indentatio -= IV;
        _semicolon(c);
    }
    _linea_nova(c, "fi");
}

/* corpus 'do ... done' post caput */
interior vacuum
_cursus (
                   Coctor* c,
    constans MateriaNodus* cursus)
{
    c->indentatio += IV;
    si (cursus != NIHIL)
    {
        _lista(c, &cursus->loci[CRUSTA_CURSUS_LIBERI]);
    }
    _dilata_si(c, "");
    c->indentatio -= IV;
    _semicolon(c);
    _linea_nova(c, "done");
}

interior vacuum
_repetitio (
                   Coctor* c,
    constans MateriaNodus* repetitio)
{
    _valor(c, _token(repetitio, (i32)CRUSTA_REPETITIO_TOK_APERTURA));
    _litterae(c, " ");
    c->saltus++;
    _lista(c, &repetitio->loci[CRUSTA_REPETITIO_PROBATIO]);
    _dilata_si(c, "");
    _semicolon(c);
    _litterae(c, " do\n");
    _cursus(c, _nodus(repetitio, (i32)CRUSTA_REPETITIO_CURSUS));
}

interior vacuum
_iteratio (
                   Coctor* c,
    constans MateriaNodus* iteratio)
{
    constans MateriaValor* verba =
        &iteratio->loci[CRUSTA_ITERATIO_VERBA];
                      b32 primum = VERUM;
                      i32 k;

    _valor(c, _token(iteratio, (i32)CRUSTA_ITERATIO_TOK_APERTURA));
    _litterae(c, " ");
    _valor(c, _token(iteratio, (i32)CRUSTA_ITERATIO_TOK_TITULUS));
    si (_token(iteratio, (i32)CRUSTA_ITERATIO_TOK_IN) == NIHIL)
    {
        _litterae(c, " in \"$@\"");
    }
    alioquin
    {
        _litterae(c, " in ");
        per (k = ZEPHYRUM; k < _numerus(verba); k++)
        {
            constans MateriaNodus* v = _elementum(verba, k);

            si (v == NIHIL || v->genus != (s32)CRUSTA_GENUS_VERBUM)
            {
                perge;
            }
            si (!primum)
            {
                _litterae(c, " ");
            }
            primum = FALSUM;
            _verbum(c, v);
        }
    }
    _litterae(c, ";");
    _linea_nova(c, "do\n");
    _cursus(c, _nodus(iteratio, (i32)CRUSTA_ITERATIO_CURSUS));
}

interior vacuum
_trivia (
                   Coctor* c,
    constans MateriaToken* t,
                      b32  ante)
{
    i32 k;
    i32 n;

    si (t == NIHIL)
    {
        redde;
    }
    n = ante ? t->numerus_ante : t->numerus_post;
    per (k = ZEPHYRUM; k < n; k++)
    {
        _chorda(c, ante ? t->spatia_ante[k]->valor
            : t->spatia_post[k]->valor);
    }
}

/* segmentum 'for ((' in aedificatorem principalem: spatia initialia
 * dempta (finalia manent), vacuum -> '1' */
interior vacuum
_segmentum_scribere (
                 Coctor* c,
      ChordaAedificator* segmentum)
{
    chorda s = chorda_aedificator_spectare(segmentum);
       i32 k = ZEPHYRUM;

    dum (   k < s.mensura
         && (s.datum[k] == ' ' || s.datum[k] == '\t'
             || s.datum[k] == '\n'))
    {
        k++;
    }
    si (k == s.mensura)
    {
        _litterae(c, "1");
    }
    alioquin
    {
        _octeti(c, s.datum + k, s.mensura - k);
    }
    chorda_aedificator_reset(segmentum);
}

/* for (( a; b; c )): textus crudus inter '((' ';' ';' '))' */
interior vacuum
_cyclus (
                   Coctor* c,
    constans MateriaNodus* cyclus)
{
    constans MateriaValor* liberi      = &cyclus->loci[
        CRUSTA_CYCLUS_LIBERI];
        ChordaAedificator* principalis = c->aedificator;
        ChordaAedificator* segmentum;
                      i32  divisiones = ZEPHYRUM;
                      i32  k;

    segmentum = chorda_aedificator_creare(c->piscina,
        (memoriae_index)64);
    si (segmentum == NIHIL)
    {
        c->deficit = VERUM;
        redde;
    }
    _litterae(c, "for ((");
    c->aedificator = segmentum;
    _trivia(c, _token(cyclus, (i32)CRUSTA_CYCLUS_TOK_PARENTHESIS),
        FALSUM);
    per (k = ZEPHYRUM; k < _numerus(liberi); k++)
    {
        constans MateriaNodus* e = _elementum(liberi, k);
        constans MateriaToken* t;

        si (e == NIHIL || e->genus != (s32)CRUSTA_GENUS_OPERATOR)
        {
            _crudum(c, materia_valor_lista_obtinere(*liberi, k),
                CRUDUM_SINE_CONTINUATIONE);
            perge;
        }
        t = _token(e, (i32)CRUSTA_OPERATOR_TOK);
        _trivia(c, t, VERUM);
        c->aedificator = principalis;
        _segmentum_scribere(c, segmentum);
        _litterae(c, "; ");
        c->aedificator = segmentum;
        _trivia(c, t, FALSUM);
        divisiones++;
    }
    _trivia(c, _token(cyclus,
        (i32)CRUSTA_CYCLUS_TOK_PARENTHESIS_CLAUSURA),
        VERUM);
    c->aedificator = principalis;
    _segmentum_scribere(c, segmentum);
    per (k = divisiones; k < (i32)II; k++)
    {
        _litterae(c, "; 1");
    }
    _litterae(c, "))");
    _linea_nova(c, "do\n");
    _cursus(c, _nodus(cyclus, (i32)CRUSTA_CYCLUS_CURSUS));
}

interior vacuum
_electio (
                   Coctor* c,
    constans MateriaNodus* electio)
{
    constans MateriaValor* liberi =
        &electio->loci[CRUSTA_ELECTIO_LIBERI];
                      i32 k;

    _litterae(c, "case ");
    _verbum(c, _nodus(electio, (i32)CRUSTA_ELECTIO_VERBUM));
    _litterae(c, " in ");
    per (k = ZEPHYRUM; k < _numerus(liberi); k++)
    {
        constans MateriaNodus* optio = _elementum(liberi, k);
        constans MateriaValor* exemplaria;
        constans MateriaToken* terminator;
                          b32  primum = VERUM;
                          i32  j;

        si (optio == NIHIL || optio->genus != (s32)CRUSTA_GENUS_OPTIO)
        {
            perge;
        }
        exemplaria = &optio->loci[CRUSTA_OPTIO_EXEMPLARIA];
        _litterae(c, "\n");
        _spatia(c, c->indentatio + IV);
        per (j = ZEPHYRUM; j < _numerus(exemplaria); j++)
        {
            constans MateriaNodus* v = _elementum(exemplaria, j);

            si (v == NIHIL || v->genus != (s32)CRUSTA_GENUS_VERBUM)
            {
                perge;
            }
            si (!primum)
            {
                _litterae(c, " | ");
            }
            primum = FALSUM;
            _verbum(c, v);
        }
        _litterae(c, ")\n");
        c->indentatio += VIII;
        _lista(c, &optio->loci[CRUSTA_OPTIO_LIBERI]);
        _dilata_si(c, "");
        c->indentatio -= VIII;
        _litterae(c, "\n");
        _spatia(c, c->indentatio + IV);
        terminator = _token(optio, (i32)CRUSTA_OPTIO_TOK_TERMINATOR);
        si (terminator != NIHIL)
        {
            _valor(c, terminator);
        }
        alioquin
        {
            _litterae(c, ";;");
        }
    }
    _linea_nova(c, "esac");
}

/* expressio iudicii; positio = locus expressionis (verbum nudum ibi
 * '-n verbum') */
interior vacuum
_iudicium (
                   Coctor* c,
    constans MateriaNodus* e,
                      b32  positio)
{
    constans MateriaToken* operator;

    si (e == NIHIL)
    {
        redde;
    }
    commutatio (e->genus)
    {
        casus CRUSTA_GENUS_VERBUM:
            si (positio)
            {
                _litterae(c, "-n ");
            }
            _verbum(c, e);
            frange;
        casus CRUSTA_GENUS_IUDICIUM_PRAEPOSITA:
            operator = _token(e, (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR);
            _valor(c, operator);
            _litterae(c, " ");
            si (_valor_est(operator, "!"))
            {
                _iudicium(c, _nodus(e,
                    (i32)CRUSTA_PRAEPOSITA_OPERANDUM),
                    VERUM);
            }
            alioquin
            {
                _verbum(c, _nodus(e, (i32)CRUSTA_PRAEPOSITA_OPERANDUM));
            }
            frange;
        casus CRUSTA_GENUS_IUDICIUM_BINARIA:
            _verbum(c, _nodus(e, (i32)CRUSTA_BINARIA_SINISTER));
            _litterae(c, " ");
            operator = _token(e, (i32)CRUSTA_BINARIA_TOK_OPERATOR);
            si (operator != NIHIL)
            {
                _valor(c, operator);
                _litterae(c, " ");
            }
            _verbum(c, _nodus(e, (i32)CRUSTA_BINARIA_DEXTER));
            frange;
        casus CRUSTA_GENUS_IUDICIUM_CONIUNCTA:
            _iudicium(c, _nodus(e, (i32)CRUSTA_BINARIA_SINISTER),
                VERUM);
            _litterae(c, " ");
            _valor(c, _token(e, (i32)CRUSTA_BINARIA_TOK_OPERATOR));
            _litterae(c, " ");
            _iudicium(c, _nodus(e, (i32)CRUSTA_BINARIA_DEXTER), VERUM);
            frange;
        casus CRUSTA_GENUS_IUDICIUM_INCLUSA:
            _litterae(c, "( ");
            _iudicium(c, _nodus(e, (i32)CRUSTA_INCLUSA_EXPRESSIO),
                VERUM);
            _litterae(c, " )");
            frange;
        ordinarius:
            _crudum_interius(c, e, CRUDUM_SINE_CONTINUATIONE);
            frange;
    }
}

interior vacuum
_assignatio (
                   Coctor* c,
    constans MateriaNodus* a);

interior vacuum
_tabulatum (
                   Coctor* c,
    constans MateriaNodus* tabulatum)
{
    constans MateriaValor* liberi = &tabulatum->loci[
        CRUSTA_TABULATUM_LIBERI];
                      b32 primum = VERUM;
                      i32 k;

    _litterae(c, "(");
    per (k = ZEPHYRUM; k < _numerus(liberi); k++)
    {
        constans MateriaNodus* e = _elementum(liberi, k);

        si (e == NIHIL)
        {
            perge;
        }
        si (!primum)
        {
            _litterae(c, " ");
        }
        primum = FALSUM;
        si (e->genus == (s32)CRUSTA_GENUS_ASSIGNATIO)
        {
            _assignatio(c, e);
        }
        alioquin
        {
            _verbum(c, e);
        }
    }
    _litterae(c, ")");
}

interior vacuum
_assignatio (
                   Coctor* c,
    constans MateriaNodus* a)
{
    constans MateriaNodus* valor = _nodus(a,
        (i32)CRUSTA_ASSIGNATIO_VALOR);

    _valor(c, _token(a, (i32)CRUSTA_ASSIGNATIO_TOK_TITULUS));
    _valor(c, _token(a, (i32)CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM));
    _valor(c, _token(a, (i32)CRUSTA_ASSIGNATIO_TOK_OPERATOR));
    si (valor != NIHIL && valor->genus == (s32)CRUSTA_GENUS_TABULATUM)
    {
        _tabulatum(c, valor);
    }
    alioquin
    {
        _verbum(c, valor);
    }
}

interior vacuum
_imperium (
                   Coctor* c,
    constans MateriaNodus* imperium,
                      b32  erroris)
{
    constans MateriaValor* liberi =
        &imperium->loci[CRUSTA_IMPERIUM_LIBERI];
                      b32 verba = FALSUM;
                      i32 k;

    per (k = ZEPHYRUM; k < _numerus(liberi); k++)
    {
        constans MateriaNodus* e = _elementum(liberi, k);

        si (e == NIHIL || e->genus == (s32)CRUSTA_GENUS_REDIRECTIO)
        {
            perge;
        }
        si (verba)
        {
            _litterae(c, " ");
        }
        verba = VERUM;
        si (e->genus == (s32)CRUSTA_GENUS_ASSIGNATIO)
        {
            _assignatio(c, e);
        }
        alioquin
        {
            _verbum(c, e);
        }
    }
    si (_redirectiones_adsunt(liberi) || erroris)
    {
        si (verba)
        {
            _litterae(c, " ");
        }
        _redirectiones(c, liberi, erroris);
    }
}

/* mandatum unum (make_command_string_internal): indentatio, corpus,
 * redirectiones compositi */
interior vacuum
_mandatum (
                   Coctor* c,
    constans MateriaNodus* nodus,
                      b32  erroris)
{
    constans MateriaValor* redirectiones = NIHIL;

    si (nodus == NIHIL)
    {
        redde;
    }
    si (nodus->genus == (s32)CRUSTA_GENUS_PIPA)
    {
        _pipa(c, nodus);
        redde;
    }
    si (nodus->genus == (s32)CRUSTA_GENUS_CATENA)
    {
        _catena(c, nodus);
        redde;
    }
    _initium_mandati(c);
    commutatio (nodus->genus)
    {
        casus CRUSTA_GENUS_IMPERIUM:
            _imperium(c, nodus, erroris);
            redde;
        casus CRUSTA_GENUS_FUNCTIO:
            _functio(c, nodus);
            redde;
        casus CRUSTA_GENUS_GREX:
            _grex(c, nodus);
            redirectiones = &nodus->loci[CRUSTA_GREX_REDIRECTIONES];
            frange;
        casus CRUSTA_GENUS_CRUSTULA:
            _litterae(c, "( ");
            c->saltus++;
            _lista(c, &nodus->loci[CRUSTA_CRUSTULA_LIBERI]);
            _dilata_si(c, "");
            _litterae(c, " )");
            redirectiones = &nodus->loci[CRUSTA_CRUSTULA_REDIRECTIONES];
            frange;
        casus CRUSTA_GENUS_CONDITIO:
            _conditio(c, &nodus->loci[CRUSTA_CONDITIO_PROBATIO],
                &nodus->loci[CRUSTA_CONDITIO_LIBERI],
                &nodus->loci[CRUSTA_CONDITIO_RAMI], ZEPHYRUM);
            redirectiones = &nodus->loci[CRUSTA_CONDITIO_REDIRECTIONES];
            frange;
        casus CRUSTA_GENUS_ITERATIO:
            _iteratio(c, nodus);
            redirectiones = &nodus->loci[CRUSTA_ITERATIO_REDIRECTIONES];
            frange;
        casus CRUSTA_GENUS_CYCLUS:
            _cyclus(c, nodus);
            redirectiones = &nodus->loci[CRUSTA_CYCLUS_REDIRECTIONES];
            frange;
        casus CRUSTA_GENUS_REPETITIO:
            _repetitio(c, nodus);
            redirectiones =
                &nodus->loci[CRUSTA_REPETITIO_REDIRECTIONES];
            frange;
        casus CRUSTA_GENUS_ELECTIO:
            _electio(c, nodus);
            redirectiones = &nodus->loci[CRUSTA_ELECTIO_REDIRECTIONES];
            frange;
        casus CRUSTA_GENUS_ARITHMETICA:
            _lexema(c, _token(nodus,
                (i32)CRUSTA_ARITHMETICA_TOK_APERTURA),
                FALSUM, VERUM, CRUDUM_SINE_CONTINUATIONE);
            _crudum(c, &nodus->loci[CRUSTA_ARITHMETICA_EXPRESSIO],
                CRUDUM_SINE_CONTINUATIONE);
            _lexema(c, _token(nodus,
                (i32)CRUSTA_ARITHMETICA_TOK_CLAUSURA),
                VERUM, FALSUM, CRUDUM_SINE_CONTINUATIONE);
            redirectiones =
                &nodus->loci[CRUSTA_ARITHMETICA_REDIRECTIONES];
            frange;
        casus CRUSTA_GENUS_IUDICIUM:
            _litterae(c, "[[ ");
            _iudicium(c, _nodus(nodus, (i32)CRUSTA_IUDICIUM_EXPRESSIO),
                VERUM);
            _litterae(c, " ]]");
            redirectiones = &nodus->loci[CRUSTA_IUDICIUM_REDIRECTIONES];
            frange;
        casus CRUSTA_GENUS_SOCIUS:
            _litterae(c, "coproc ");
            si (_token(nodus, (i32)CRUSTA_SOCIUS_TOK_TITULUS) != NIHIL)
            {
                _valor(c, _token(nodus,
                    (i32)CRUSTA_SOCIUS_TOK_TITULUS));
            }
            alioquin
            {
                _litterae(c, "COPROC");
            }
            _litterae(c, " ");
            c->saltus++;
            _mandatum(c, _nodus(nodus, (i32)CRUSTA_SOCIUS_IMPERIUM),
                erroris);
            redde;
        ordinarius:
            _crudum_interius(c, nodus, CRUDUM_SINE_CONTINUATIONE);
            redde;
    }
    si (_redirectiones_adsunt(redirectiones) || erroris)
    {
        _litterae(c, " ");
        _redirectiones(c, redirectiones, erroris);
    }
}


/* ==================================================
 * Ingressus
 * ================================================== */

chorda
crusta_coctum_scribere (
                  Piscina* piscina,
    constans MateriaNodus* radix)
{
    Coctor c;
    chorda fructus;

    memset(&c, ZEPHYRUM, magnitudo(c));
    c.piscina  = piscina;
    c.radix    = radix;
    c.functio  = I;
    c.aedificator = chorda_aedificator_creare(piscina,
        (memoriae_index)1024);
    c.dilata      = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));
    si (c.aedificator == NIHIL || c.dilata == NIHIL)
    {
        fructus.mensura  = ZEPHYRUM;
        fructus.datum    = NIHIL;
        redde fructus;
    }
    si (radix != NIHIL && radix->genus == (s32)CRUSTA_GENUS_PROGRAMMA)
    {
        _lista(&c, &radix->loci[CRUSTA_PROGRAMMA_LIBERI]);
    }
    alioquin si (radix != NIHIL)
    {
        _mandatum(&c, radix, FALSUM);
    }
    _dilata_si(&c, "");
    si (   chorda_aedificator_longitudo(c.aedificator) > ZEPHYRUM
        && _ultimum(&c) != '\n')
    {
        _litterae(&c, "\n");
    }
    fructus = chorda_aedificator_finire(c.aedificator);
    si (c.deficit)
    {
        fructus.datum = NIHIL;
    }
    redde fructus;
}
