/*
 * selectio.c - Parser et Congruens Selectorum CSS
 *
 * Implementatio selectorum CSS pro documentis STML.
 * Sustinet selectores tituli, id, classis, attributi, et pseudo-classis
 * cum combinatoribus descendens, liberum, et fratribus.
 */

#include <stdio.h>
#include <string.h>
#include "selectio.h"

/* ==================================================
 * Status Parsoris Internus
 * ================================================== */

nomen structura {
    chorda               input;
    i32                  positus;      /* Positus currens */
    Piscina*             piscina;
    InternamentumChorda* intern;
} SelectioParser;

/* ==================================================
 * Adiutores Classificationis Characterum
 * ================================================== */

interior b32
_est_spatium (character c)
{
    redde c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

interior b32
_est_littera (character c)
{
    redde (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_' || c == '-';
}

interior b32
_est_numerus (character c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_est_identificator_initium (character c)
{
    redde _est_littera(c);
}

interior b32
_est_identificator_pars (character c)
{
    redde _est_littera(c) || _est_numerus(c);
}

/* ==================================================
 * Adiutores Parsoris
 * ================================================== */

interior character
_parser_currens (SelectioParser* parser)
{
    si (parser->positus >= parser->input.mensura) {
        redde '\0';
    }
    redde (character)parser->input.datum[parser->positus];
}

interior character
_parser_prospicere (
    SelectioParser* parser,
    i32             offset)
{
    i32 pos;
    pos = parser->positus + offset;
    si (pos >= parser->input.mensura) {
        redde '\0';
    }
    redde (character)parser->input.datum[pos];
}

interior vacuum
_parser_praeterire (SelectioParser* parser)
{
    si (parser->positus < parser->input.mensura) {
        parser->positus++;
    }
}

interior vacuum
_parser_praeterire_spatium (SelectioParser* parser)
{
    dum (_est_spatium(_parser_currens(parser))) {
        _parser_praeterire(parser);
    }
}

interior b32
_parser_ad_finem (SelectioParser* parser)
{
    redde parser->positus >= parser->input.mensura;
}

/* ==================================================
 * Parsatio Identificatoris
 * ================================================== */

interior chorda
_parser_legere_identificator (SelectioParser* parser)
{
    chorda     resultus;
    i32        initium;

    initium = parser->positus;

    si (!_est_identificator_initium(_parser_currens(parser))) {
        resultus.datum = NIHIL;
        resultus.mensura = 0;
        redde resultus;
    }

    dum (_est_identificator_pars(_parser_currens(parser))) {
        _parser_praeterire(parser);
    }

    resultus.datum = parser->input.datum + initium;
    resultus.mensura = parser->positus - initium;
    redde resultus;
}

/* ==================================================
 * Parsatio Selectoris Simplicis
 * ================================================== */

/* Legere selectorem tituli: div, span, etc. */
interior SelectioSimplex*
_parser_legere_titulum (SelectioParser* parser)
{
    SelectioSimplex* simplex;
    chorda           identificator;
    chorda*          interned;

    identificator = _parser_legere_identificator(parser);
    si (identificator.datum == NIHIL) {
        redde NIHIL;
    }

    interned = chorda_internare(parser->intern, identificator);
    si (interned == NIHIL) {
        redde NIHIL;
    }

    simplex = piscina_allocare(parser->piscina, magnitudo(SelectioSimplex));
    si (simplex == NIHIL) {
        redde NIHIL;
    }

    simplex->genus = SELECTIO_TITULUS;
    simplex->valor = interned;
    simplex->attr_op = ATTR_OP_EXISTIT;
    simplex->attr_valor = NIHIL;
    simplex->pseudo = PSEUDO_PRIMUS_LIBERUM;

    redde simplex;
}

/* Legere selectorem ID: #myid */
interior SelectioSimplex*
_parser_legere_id (SelectioParser* parser)
{
    SelectioSimplex* simplex;
    chorda           identificator;
    chorda*          interned;

    /* Praeterire '#' */
    _parser_praeterire(parser);

    identificator = _parser_legere_identificator(parser);
    si (identificator.datum == NIHIL) {
        redde NIHIL;
    }

    interned = chorda_internare(parser->intern, identificator);
    si (interned == NIHIL) {
        redde NIHIL;
    }

    simplex = piscina_allocare(parser->piscina, magnitudo(SelectioSimplex));
    si (simplex == NIHIL) {
        redde NIHIL;
    }

    simplex->genus = SELECTIO_ID;
    simplex->valor = interned;
    simplex->attr_op = ATTR_OP_EXISTIT;
    simplex->attr_valor = NIHIL;
    simplex->pseudo = PSEUDO_PRIMUS_LIBERUM;

    redde simplex;
}

/* Legere selectorem classis: .myclass */
interior SelectioSimplex*
_parser_legere_classis (SelectioParser* parser)
{
    SelectioSimplex* simplex;
    chorda           identificator;
    chorda*          interned;

    /* Praeterire '.' */
    _parser_praeterire(parser);

    identificator = _parser_legere_identificator(parser);
    si (identificator.datum == NIHIL) {
        redde NIHIL;
    }

    interned = chorda_internare(parser->intern, identificator);
    si (interned == NIHIL) {
        redde NIHIL;
    }

    simplex = piscina_allocare(parser->piscina, magnitudo(SelectioSimplex));
    si (simplex == NIHIL) {
        redde NIHIL;
    }

    simplex->genus = SELECTIO_CLASSIS;
    simplex->valor = interned;
    simplex->attr_op = ATTR_OP_EXISTIT;
    simplex->attr_valor = NIHIL;
    simplex->pseudo = PSEUDO_PRIMUS_LIBERUM;

    redde simplex;
}

/* Legere selectorem attributi: [attr], [attr=val], [attr^=val], etc. */
interior SelectioSimplex*
_parser_legere_attributum (SelectioParser* parser)
{
    SelectioSimplex*   simplex;
    chorda             attr_nomen;
    chorda             attr_valor;
    chorda*            internatum_nomen;
    chorda*            internatum_valor;
    AttributumOperator op;
    character          c;
    character          quote;
    i32                valor_initium;

    /* Praeterire '[' */
    _parser_praeterire(parser);
    _parser_praeterire_spatium(parser);

    /* Legere nomen attributi */
    attr_nomen = _parser_legere_identificator(parser);
    si (attr_nomen.datum == NIHIL) {
        redde NIHIL;
    }

    internatum_nomen = chorda_internare(parser->intern, attr_nomen);
    si (internatum_nomen == NIHIL) {
        redde NIHIL;
    }

    _parser_praeterire_spatium(parser);

    /* Verificare operatorem vel finem */
    c = _parser_currens(parser);
    si (c == ']') {
        /* [attr] - verificatio existentiae */
        _parser_praeterire(parser);
        op = ATTR_OP_EXISTIT;
        internatum_valor = NIHIL;
    } alioquin {
        /* Legere operatorem */
        si (c == '=') {
            op = ATTR_OP_AEQUALIS;
            _parser_praeterire(parser);
        } alioquin si (c == '^' && _parser_prospicere(parser, 1) == '=') {
            op = ATTR_OP_INCIPIT;
            _parser_praeterire(parser);
            _parser_praeterire(parser);
        } alioquin si (c == '$' && _parser_prospicere(parser, 1) == '=') {
            op = ATTR_OP_TERMINAT;
            _parser_praeterire(parser);
            _parser_praeterire(parser);
        } alioquin si (c == '*' && _parser_prospicere(parser, 1) == '=') {
            op = ATTR_OP_CONTINET;
            _parser_praeterire(parser);
            _parser_praeterire(parser);
        } alioquin si (c == '~' && _parser_prospicere(parser, 1) == '=') {
            op = ATTR_OP_VERBUM;
            _parser_praeterire(parser);
            _parser_praeterire(parser);
        } alioquin {
            redde NIHIL;  /* Operator invalidus */
        }

        _parser_praeterire_spatium(parser);

        /* Legere valorem - cum vel sine citationibus */
        c = _parser_currens(parser);
        si (c == '"' || c == '\'') {
            quote = c;
            _parser_praeterire(parser);
            valor_initium = parser->positus;
            dum (_parser_currens(parser) != quote && !_parser_ad_finem(parser)) {
                _parser_praeterire(parser);
            }
            attr_valor.datum = parser->input.datum + valor_initium;
            attr_valor.mensura = parser->positus - valor_initium;
            si (_parser_currens(parser) == quote) {
                _parser_praeterire(parser);
            }
        } alioquin {
            /* Valor sine citationibus - legere identifier */
            attr_valor = _parser_legere_identificator(parser);
            si (attr_valor.datum == NIHIL) {
                redde NIHIL;
            }
        }

        internatum_valor = chorda_internare(parser->intern, attr_valor);
        si (internatum_valor == NIHIL) {
            redde NIHIL;
        }

        _parser_praeterire_spatium(parser);
        si (_parser_currens(parser) != ']') {
            redde NIHIL;
        }
        _parser_praeterire(parser);
    }

    simplex = piscina_allocare(parser->piscina, magnitudo(SelectioSimplex));
    si (simplex == NIHIL) {
        redde NIHIL;
    }

    simplex->genus = SELECTIO_ATTRIBUTUM;
    simplex->valor = internatum_nomen;
    simplex->attr_op = op;
    simplex->attr_valor = internatum_valor;
    simplex->pseudo = PSEUDO_PRIMUS_LIBERUM;

    redde simplex;
}

/* Legere selectorem pseudo-classis: :first-child, :last-child, :empty */
interior SelectioSimplex*
_parser_legere_pseudo (SelectioParser* parser)
{
    SelectioSimplex*  simplex;
    chorda            identificator;
    PseudoClassisGenus pseudo;

    /* Praeterire ':' */
    _parser_praeterire(parser);

    identificator = _parser_legere_identificator(parser);
    si (identificator.datum == NIHIL) {
        redde NIHIL;
    }

    /* Congruere nomen pseudo-classis */
    si (chorda_aequalis_literis(identificator, "first-child")) {
        pseudo = PSEUDO_PRIMUS_LIBERUM;
    } alioquin si (chorda_aequalis_literis(identificator, "last-child")) {
        pseudo = PSEUDO_ULTIMUS_LIBERUM;
    } alioquin si (chorda_aequalis_literis(identificator, "empty")) {
        pseudo = PSEUDO_VACUUS;
    } alioquin {
        redde NIHIL;  /* Pseudo-classis ignota */
    }

    simplex = piscina_allocare(parser->piscina, magnitudo(SelectioSimplex));
    si (simplex == NIHIL) {
        redde NIHIL;
    }

    simplex->genus = SELECTIO_PSEUDO_CLASSIS;
    simplex->valor = NIHIL;
    simplex->attr_op = ATTR_OP_EXISTIT;
    simplex->attr_valor = NIHIL;
    simplex->pseudo = pseudo;

    redde simplex;
}

/* ==================================================
 * Parsatio Selectoris Compositi
 * ================================================== */

/* Legere selectorem compositum (plures selectores simplices sine combinatore)
 * e.g., div.class#id
 */
interior SelectioComposita*
_parser_legere_composita (SelectioParser* parser)
{
    SelectioComposita* composita;
    SelectioSimplex*   simplex;
    character          c;

    composita = piscina_allocare(parser->piscina, magnitudo(SelectioComposita));
    si (composita == NIHIL) {
        redde NIHIL;
    }

    composita->partes = xar_creare(parser->piscina, magnitudo(SelectioSimplex*));
    si (composita->partes == NIHIL) {
        redde NIHIL;
    }

    dum (!_parser_ad_finem(parser)) {
        c = _parser_currens(parser);

        si (_est_identificator_initium(c)) {
            simplex = _parser_legere_titulum(parser);
        } alioquin si (c == '#') {
            simplex = _parser_legere_id(parser);
        } alioquin si (c == '.') {
            simplex = _parser_legere_classis(parser);
        } alioquin si (c == '[') {
            simplex = _parser_legere_attributum(parser);
        } alioquin si (c == ':') {
            simplex = _parser_legere_pseudo(parser);
        } alioquin {
            /* Finis selectoris compositi */
            frange;
        }

        si (simplex == NIHIL) {
            redde NIHIL;
        }

        {
            SelectioSimplex** slot;
            slot = xar_addere(composita->partes);
            si (slot != NIHIL) {
                *slot = simplex;
            }
        }
    }

    /* Debet habere saltem unum selectorem simplicem */
    si (xar_numerus(composita->partes) == 0) {
        redde NIHIL;
    }

    redde composita;
}

/* ==================================================
 * Parsatio Selectoris Complexi (cum combinatoribus)
 * ================================================== */

/* Legere selectorem complexum cum combinatoribus */
interior SelectioSequentia*
_parser_legere_sequentia (SelectioParser* parser)
{
    SelectioSequentia* prima;
    SelectioSequentia* currens;
    SelectioSequentia* nova;
    SelectioComposita* composita;
    CombinatorGenus    combinator;
    character          c;
    b32                habuit_spatium;

    _parser_praeterire_spatium(parser);

    /* Legere primum selectorem compositum */
    composita = _parser_legere_composita(parser);
    si (composita == NIHIL) {
        redde NIHIL;
    }

    prima = piscina_allocare(parser->piscina, magnitudo(SelectioSequentia));
    si (prima == NIHIL) {
        redde NIHIL;
    }

    prima->composita = composita;
    prima->combinator = COMBINATOR_DESCENDENS;  /* Non usitatum pro primo */
    prima->sequens = NIHIL;

    currens = prima;

    /* Legere reliquos selectores compositos cum combinatoribus */
    dum (!_parser_ad_finem(parser)) {
        /* Verificare spatium (potentialis combinator descendens) */
        habuit_spatium = _est_spatium(_parser_currens(parser));
        _parser_praeterire_spatium(parser);

        si (_parser_ad_finem(parser)) {
            frange;
        }

        c = _parser_currens(parser);

        /* Determinare combinatorem */
        si (c == '>') {
            combinator = COMBINATOR_LIBERUM;
            _parser_praeterire(parser);
            _parser_praeterire_spatium(parser);
        } alioquin si (c == '+') {
            combinator = COMBINATOR_FRATER_ADJ;
            _parser_praeterire(parser);
            _parser_praeterire_spatium(parser);
        } alioquin si (c == '~') {
            combinator = COMBINATOR_FRATER_GEN;
            _parser_praeterire(parser);
            _parser_praeterire_spatium(parser);
        } alioquin si (habuit_spatium) {
            /* Spatium = combinator descendens */
            combinator = COMBINATOR_DESCENDENS;
        } alioquin {
            /* Nullus combinator et nullum spatium - finis selectoris */
            frange;
        }

        /* Legere proximum selectorem compositum */
        composita = _parser_legere_composita(parser);
        si (composita == NIHIL) {
            frange;
        }

        nova = piscina_allocare(parser->piscina, magnitudo(SelectioSequentia));
        si (nova == NIHIL) {
            redde NIHIL;
        }

        nova->composita = composita;
        nova->combinator = combinator;
        nova->sequens = NIHIL;

        currens->sequens = nova;
        currens = nova;
    }

    redde prima;
}

/* ==================================================
 * API Publica Parsationis
 * ================================================== */

SelectioResultus
selectio_legere (
    chorda               input,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    SelectioResultus  resultus;
    SelectioParser    parser;
    SelectioSequentia* selectio;

    resultus.successus = FALSUM;
    resultus.selectio = NIHIL;
    resultus.error_positus = 0;
    resultus.error.datum = NIHIL;
    resultus.error.mensura = 0;

    si (input.datum == NIHIL || input.mensura == 0) {
        resultus.error = chorda_ex_literis("Selector vacuus", piscina);
        redde resultus;
    }

    parser.input = input;
    parser.positus = 0;
    parser.piscina = piscina;
    parser.intern = intern;

    selectio = _parser_legere_sequentia(&parser);

    si (selectio == NIHIL) {
        resultus.error_positus = parser.positus;
        resultus.error = chorda_ex_literis("Syntaxis selectoris invalida", piscina);
        redde resultus;
    }

    resultus.successus = VERUM;
    resultus.selectio = selectio;
    redde resultus;
}

SelectioResultus
selectio_legere_ex_literis (
    constans character*  cstr,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    chorda input;
    input = chorda_ex_literis(cstr, piscina);
    redde selectio_legere(input, piscina, intern);
}

/* ==================================================
 * Adiutores Congruentiae
 * ================================================== */

/* Obtinere nodum parentem */
interior StmlNodus*
_nodus_parens (StmlNodus* nodus)
{
    redde nodus->parens;
}

/* Obtinere indicem nodi in parente */
interior s32
_nodus_index_in_parente (StmlNodus* nodus)
{
    StmlNodus* parens;
    i32        i;
    i32        num;
    StmlNodus* liberum;

    parens = _nodus_parens(nodus);
    si (parens == NIHIL || parens->liberi == NIHIL) {
        redde -1;
    }

    num = xar_numerus(parens->liberi);
    per (i = 0; i < num; i++) {
        liberum = *(StmlNodus**)xar_obtinere(parens->liberi, i);
        si (liberum == nodus) {
            redde (s32)i;
        }
    }

    redde -1;
}

/* Obtinere indicem inter fratres elementorum tantum */
interior s32
_nodus_index_elementorum (StmlNodus* nodus)
{
    StmlNodus* parens;
    i32        i;
    i32        num;
    i32        elem_index;
    StmlNodus* liberum;

    parens = _nodus_parens(nodus);
    si (parens == NIHIL || parens->liberi == NIHIL) {
        redde -1;
    }

    elem_index = 0;
    num = xar_numerus(parens->liberi);
    per (i = 0; i < num; i++) {
        liberum = *(StmlNodus**)xar_obtinere(parens->liberi, i);
        si (liberum == nodus) {
            redde (s32)elem_index;
        }
        si (liberum->genus == STML_NODUS_ELEMENTUM) {
            elem_index++;
        }
    }

    redde -1;
}

/* Verificare si nodus est primus liberum elementum */
interior b32
_est_primus_liberum (StmlNodus* nodus)
{
    redde _nodus_index_elementorum(nodus) == 0;
}

/* Verificare si nodus est ultimus liberum elementum */
interior b32
_est_ultimus_liberum (StmlNodus* nodus)
{
    StmlNodus* parens;
    i32        i;
    i32        num;
    StmlNodus* liberum;

    parens = _nodus_parens(nodus);
    si (parens == NIHIL || parens->liberi == NIHIL) {
        redde VERUM;  /* Sine parente = ultimus per defectum */
    }

    num = xar_numerus(parens->liberi);

    /* Scandere retro pro ultimo elemento */
    per (i = num - 1; i >= 0; i--) {
        liberum = *(StmlNodus**)xar_obtinere(parens->liberi, (i32)i);
        si (liberum->genus == STML_NODUS_ELEMENTUM) {
            redde liberum == nodus;
        }
    }

    redde FALSUM;
}

/* Verificare si nodus nullos liberos habet (vel solum textum spatii) */
interior b32
_est_vacuus (StmlNodus* nodus)
{
    i32        i;
    i32        num;
    StmlNodus* liberum;

    si (nodus->liberi == NIHIL) {
        redde VERUM;
    }

    num = xar_numerus(nodus->liberi);
    si (num == 0) {
        redde VERUM;
    }

    /* Verificare si omnes liberi sunt textus spatii tantum */
    per (i = 0; i < num; i++) {
        liberum = *(StmlNodus**)xar_obtinere(nodus->liberi, i);
        si (liberum->genus != STML_NODUS_TEXTUS) {
            redde FALSUM;
        }
        /* Verificare si textus non est spatium */
        si (liberum->valor != NIHIL) {
            chorda trimmed;
            trimmed = chorda_praecidere(*liberum->valor);
            si (trimmed.mensura > 0) {
                redde FALSUM;
            }
        }
    }

    redde VERUM;
}

/* Obtinere fratrem elementum priorem */
interior StmlNodus*
_frater_prior_elementum (StmlNodus* nodus)
{
    StmlNodus* parens;
    s32        my_index;
    s32        i;
    StmlNodus* liberum;

    parens = _nodus_parens(nodus);
    si (parens == NIHIL || parens->liberi == NIHIL) {
        redde NIHIL;
    }

    my_index = _nodus_index_in_parente(nodus);
    si (my_index <= 0) {
        redde NIHIL;
    }

    /* Scandere retro pro elemento priore */
    per (i = my_index - 1; i >= 0; i--) {
        liberum = *(StmlNodus**)xar_obtinere(parens->liberi, (i32)i);
        si (liberum->genus == STML_NODUS_ELEMENTUM) {
            redde liberum;
        }
    }

    redde NIHIL;
}

/* ==================================================
 * Congruentia Selectoris Simplicis
 * ================================================== */

interior b32
_simplex_congruit (
    SelectioSimplex* simplex,
    StmlNodus*       nodus)
{
    chorda* attr_valor;

    /* Solum elementa possunt congruere */
    si (nodus->genus != STML_NODUS_ELEMENTUM) {
        redde FALSUM;
    }

    commutatio (simplex->genus) {
        casus SELECTIO_TITULUS:
            /* Congruere nomen tituli */
            si (nodus->titulus == NIHIL) {
                redde FALSUM;
            }
            /* Velociter: comparatio indicis (si ambo internati) */
            si (nodus->titulus == simplex->valor) {
                redde VERUM;
            }
            /* Lente: comparatio chordae (defensiva) */
            redde chorda_aequalis(*nodus->titulus, *simplex->valor);

        casus SELECTIO_ID:
            /* Congruere attributum id */
            attr_valor = stml_attributum_capere(nodus, "id");
            si (attr_valor == NIHIL) {
                redde FALSUM;
            }
            /* Velociter: comparatio indicis (si ambo internati) */
            si (attr_valor == simplex->valor) {
                redde VERUM;
            }
            /* Lente: comparatio chordae (defensiva) */
            redde chorda_aequalis(*attr_valor, *simplex->valor);

        casus SELECTIO_CLASSIS:
            /* Congruere attributum labels/class
             * Nota: stml_titulum_habet expectat chordam null-terminatam,
             * sed chorda datum potest non esse null-terminata. Debemus
             * creare copiam temporariam null-terminatam.
             */
            {
                character class_buf[256];
                i32 class_len;

                class_len = simplex->valor->mensura;
                si (class_len >= 256) {
                    class_len = 255;
                }
                memcpy(class_buf, simplex->valor->datum, (magnitudo(character)) * class_len);
                class_buf[class_len] = '\0';

                redde stml_titulum_habet(nodus, class_buf);
            }

        casus SELECTIO_ATTRIBUTUM:
            /* Creare copiam null-terminatam nominis attributi */
            {
                character attr_buf[256];
                i32 attr_len;

                attr_len = simplex->valor->mensura;
                si (attr_len >= 256) {
                    attr_len = 255;
                }
                memcpy(attr_buf, simplex->valor->datum, (magnitudo(character)) * attr_len);
                attr_buf[attr_len] = '\0';

                attr_valor = stml_attributum_capere(nodus, attr_buf);
            }

            commutatio (simplex->attr_op) {
                casus ATTR_OP_EXISTIT:
                    redde attr_valor != NIHIL;

                casus ATTR_OP_AEQUALIS:
                    si (attr_valor == NIHIL) redde FALSUM;
                    /* Velociter: comparatio indicis */
                    si (attr_valor == simplex->attr_valor) redde VERUM;
                    /* Lente: comparatio chordae */
                    redde chorda_aequalis(*attr_valor, *simplex->attr_valor);

                casus ATTR_OP_INCIPIT:
                    si (attr_valor == NIHIL) redde FALSUM;
                    redde chorda_incipit(*attr_valor, *simplex->attr_valor);

                casus ATTR_OP_TERMINAT:
                    si (attr_valor == NIHIL) redde FALSUM;
                    redde chorda_terminatur(*attr_valor, *simplex->attr_valor);

                casus ATTR_OP_CONTINET:
                    si (attr_valor == NIHIL) redde FALSUM;
                    redde chorda_continet(*attr_valor, *simplex->attr_valor);

                casus ATTR_OP_VERBUM: {
                    /* Congruentia verbi - verificare si valor apparet ut verbum integrum */
                    i32       i;
                    i32       len;
                    i32       val_len;
                    character c_before;
                    character c_after;
                    s32       inventum;

                    si (attr_valor == NIHIL) redde FALSUM;

                    len = attr_valor->mensura;
                    val_len = simplex->attr_valor->mensura;

                    /* Quaerere congruentiam verbi */
                    per (i = 0; i <= len - val_len; i++) {
                        inventum = 1;
                        /* Verificare si subchorda congruit */
                        si (memcmp(attr_valor->datum + i,
                                   simplex->attr_valor->datum,
                                   (magnitudo(character)) * (i32)val_len) != 0) {
                            inventum = 0;
                        }
                        si (inventum) {
                            /* Verificare limites verbi */
                            c_before = (i == 0) ? ' ' : (character)attr_valor->datum[i - 1];
                            c_after = (i + val_len >= len) ? ' ' : (character)attr_valor->datum[i + val_len];
                            si ((c_before == ' ' || c_before == '\t') &&
                                (c_after == ' ' || c_after == '\t' || c_after == '\0')) {
                                redde VERUM;
                            }
                        }
                    }
                    redde FALSUM;
                }

                ordinarius:
                    redde FALSUM;
            }

        casus SELECTIO_PSEUDO_CLASSIS:
            commutatio (simplex->pseudo) {
                casus PSEUDO_PRIMUS_LIBERUM:
                    redde _est_primus_liberum(nodus);

                casus PSEUDO_ULTIMUS_LIBERUM:
                    redde _est_ultimus_liberum(nodus);

                casus PSEUDO_VACUUS:
                    redde _est_vacuus(nodus);

                ordinarius:
                    redde FALSUM;
            }

        ordinarius:
            redde FALSUM;
    }
}

/* ==================================================
 * Congruentia Selectoris Compositi
 * ================================================== */

interior b32
_composita_congruit (
    SelectioComposita* composita,
    StmlNodus*         nodus)
{
    i32              i;
    i32              num;
    SelectioSimplex* simplex;

    num = xar_numerus(composita->partes);
    per (i = 0; i < num; i++) {
        simplex = *(SelectioSimplex**)xar_obtinere(composita->partes, i);
        si (!_simplex_congruit(simplex, nodus)) {
            redde FALSUM;
        }
    }

    redde VERUM;
}

/* ==================================================
 * Congruentia Selectoris Complexi (cum combinatoribus)
 * ================================================== */

/* Declaratio anticipata pro recursione */
interior b32 _sequentia_congruit(SelectioSequentia* selectio, StmlNodus* nodus);

/* Verificare si aliquis antecessor congruit cum catena selectoris reliqua */
interior b32
_descendens_congruit (
    SelectioSequentia* selectio,
    StmlNodus*         nodus)
{
    StmlNodus* antecessor;

    antecessor = _nodus_parens(nodus);
    dum (antecessor != NIHIL) {
        si (_sequentia_congruit(selectio, antecessor)) {
            redde VERUM;
        }
        antecessor = _nodus_parens(antecessor);
    }

    redde FALSUM;
}

/* Verificare si parens congruit cum catena selectoris reliqua */
interior b32
_liberum_congruit (
    SelectioSequentia* selectio,
    StmlNodus*         nodus)
{
    StmlNodus* parens;

    parens = _nodus_parens(nodus);
    si (parens == NIHIL) {
        redde FALSUM;
    }

    redde _sequentia_congruit(selectio, parens);
}

/* Verificare si frater elementum immediate praecedens congruit */
interior b32
_frater_adjacens_congruit (
    SelectioSequentia* selectio,
    StmlNodus*         nodus)
{
    StmlNodus* prior;

    prior = _frater_prior_elementum(nodus);
    si (prior == NIHIL) {
        redde FALSUM;
    }

    redde _sequentia_congruit(selectio, prior);
}

/* Verificare si aliquis frater elementum praecedens congruit */
interior b32
_frater_generalis_congruit (
    SelectioSequentia* selectio,
    StmlNodus*         nodus)
{
    StmlNodus* parens;
    s32        my_index;
    s32        i;
    StmlNodus* liberum;

    parens = _nodus_parens(nodus);
    si (parens == NIHIL || parens->liberi == NIHIL) {
        redde FALSUM;
    }

    my_index = _nodus_index_in_parente(nodus);
    si (my_index <= 0) {
        redde FALSUM;
    }

    /* Verificare omnes fratres elementorum praecedentes */
    per (i = 0; i < my_index; i++) {
        liberum = *(StmlNodus**)xar_obtinere(parens->liberi, (i32)i);
        si (liberum->genus == STML_NODUS_ELEMENTUM) {
            si (_sequentia_congruit(selectio, liberum)) {
                redde VERUM;
            }
        }
    }

    redde FALSUM;
}

/* Congruere solum selectorem compositum in positione currente catenae.
 * NON recurrit in catenam - vocans tractat traversionem catenae.
 */
interior b32
_sequentia_congruit (
    SelectioSequentia* selectio,
    StmlNodus*         nodus)
{
    redde _composita_congruit(selectio->composita, nodus);
}

/* ==================================================
 * API Publica Congruentiae
 * ================================================== */

b32
selectio_congruit (
    SelectioSequentia* selectio,
    StmlNodus*         nodus)
{
    SelectioSequentia* currens;
    SelectioSequentia* ultima;

    si (selectio == NIHIL || nodus == NIHIL) {
        redde FALSUM;
    }

    /* Invenire ultimum selectorem in catena (dextrorsum) */
    ultima = selectio;
    dum (ultima->sequens != NIHIL) {
        ultima = ultima->sequens;
    }

    /* Incipere congruentiam ab ultimo selectore */
    si (!_composita_congruit(ultima->composita, nodus)) {
        redde FALSUM;
    }

    /* Si unus selector tantum, finitum est */
    si (selectio == ultima) {
        redde VERUM;
    }

    /* Laborare retrorsum per catenam */
    /* Debemus verificare relationes combinatorum */
    currens = selectio;
    dum (currens->sequens != ultima) {
        currens = currens->sequens;
    }

    /* Nunc currens->sequens == ultima, verificare combinatorem */
    commutatio (ultima->combinator) {
        casus COMBINATOR_DESCENDENS:
            redde _descendens_congruit(currens, nodus);

        casus COMBINATOR_LIBERUM:
            redde _liberum_congruit(currens, nodus);

        casus COMBINATOR_FRATER_ADJ:
            redde _frater_adjacens_congruit(currens, nodus);

        casus COMBINATOR_FRATER_GEN:
            redde _frater_generalis_congruit(currens, nodus);

        ordinarius:
            redde FALSUM;
    }
}

/* ==================================================
 * Functiones Quaestionis
 * ================================================== */

/* Quaestio recursiva profunditate prima pro prima congruentia */
interior StmlNodus*
_invenire_primum_recursivus (
    StmlNodus*         nodus,
    SelectioSequentia* selectio)
{
    StmlNodus* resultus;
    i32        i;
    i32        num;
    StmlNodus* liberum;

    /* Verificare nodum currentem */
    si (selectio_congruit(selectio, nodus)) {
        redde nodus;
    }

    /* Quaerere in liberis */
    si (nodus->liberi != NIHIL) {
        num = xar_numerus(nodus->liberi);
        per (i = 0; i < num; i++) {
            liberum = *(StmlNodus**)xar_obtinere(nodus->liberi, i);
            resultus = _invenire_primum_recursivus(liberum, selectio);
            si (resultus != NIHIL) {
                redde resultus;
            }
        }
    }

    redde NIHIL;
}

StmlNodus*
selectio_invenire_primum (
    StmlNodus*         radix,
    SelectioSequentia* selectio)
{
    si (radix == NIHIL || selectio == NIHIL) {
        redde NIHIL;
    }

    redde _invenire_primum_recursivus(radix, selectio);
}

/* Quaestio recursiva profunditate prima pro omnibus congruentiis */
interior vacuum
_invenire_omnes_recursivus (
    StmlNodus*         nodus,
    SelectioSequentia* selectio,
    Xar*               resultus)
{
    i32        i;
    i32        num;
    StmlNodus* liberum;

    /* Verificare nodum currentem */
    si (selectio_congruit(selectio, nodus)) {
        StmlNodus** slot;
        slot = xar_addere(resultus);
        si (slot != NIHIL) {
            *slot = nodus;
        }
    }

    /* Quaerere in liberis */
    si (nodus->liberi != NIHIL) {
        num = xar_numerus(nodus->liberi);
        per (i = 0; i < num; i++) {
            liberum = *(StmlNodus**)xar_obtinere(nodus->liberi, i);
            _invenire_omnes_recursivus(liberum, selectio, resultus);
        }
    }
}

Xar*
selectio_invenire_omnes (
    StmlNodus*         radix,
    SelectioSequentia* selectio,
    Piscina*           piscina)
{
    Xar* resultus;

    si (radix == NIHIL || selectio == NIHIL) {
        redde NIHIL;
    }

    resultus = xar_creare(piscina, magnitudo(StmlNodus*));
    si (resultus == NIHIL) {
        redde NIHIL;
    }

    _invenire_omnes_recursivus(radix, selectio, resultus);

    redde resultus;
}

/* ==================================================
 * Functiones Commoditatis
 * ================================================== */

StmlNodus*
stml_quaerere (
    StmlNodus*           radix,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    SelectioResultus resultus;

    resultus = selectio_legere_ex_literis(selector, piscina, intern);
    si (!resultus.successus) {
        redde NIHIL;
    }

    redde selectio_invenire_primum(radix, resultus.selectio);
}

Xar*
stml_quaerere_omnes (
    StmlNodus*           radix,
    constans character*  selector,
    Piscina*             piscina,
    InternamentumChorda* intern)
{
    SelectioResultus resultus;

    resultus = selectio_legere_ex_literis(selector, piscina, intern);
    si (!resultus.successus) {
        redde NIHIL;
    }

    redde selectio_invenire_omnes(radix, resultus.selectio, piscina);
}
