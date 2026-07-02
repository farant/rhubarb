/* silva_lexema.c - Lexator totalis silvae
 *
 * Furcatus ex arbor2_lexema.c, refictus:
 *   - emittit SilvaToken unificata (non Arbor2Lexema)
 *   - scanner triviae UNUS (arbor2 duo divergentia habuit - fons defectus
 *     directivi); commentum lineae NUMQUAM novam lineam consumit
 *   - lectio EFFECTIVA: laminae \<nl> intra lexemata transiliuntur et
 *     in scissuris memorantur (C89 phase 2)
 *   - valores = visus in fontem (nulla internatio, nulla copia) nisi
 *     lexema laminatum sit (tunc textus mundus synthesitur)
 *   - genera IMPERFECTUM pro litteris non terminatis; OCTETUS_IGNOTUS
 *     pro octetis non-C; numquam frangit
 *   - fluitantia cum puncto initiali (.5) recognita
 *   - verba clausa ANGLICA solum (latina per expansorem, non hic)
 */

#include "silva_lexema.h"
#include "chorda_aedificator.h"
#include <string.h>

/* ==================================================
 * Status lexatoris (internus)
 * ================================================== */

nomen structura {
    constans character* fons;
    i32                 mensura;
    i32                 positus;
    i32                 linea;
    i32                 columna;
    Piscina*            piscina;
    s32                 fons_index;
} SilvaLexator;

/* Contextus scansionis unius lexematis (laminas colligit) */
nomen structura {
    i32  initium;        /* byte offset initii */
    i32  linea_initium;
    i32  columna_initium;
    i32  effectivi;      /* numerus characterum effectivorum sumptorum */
    Xar* scissurae;      /* Xar de SilvaScissura; NIHIL si nullae */
} Scansio;


/* ==================================================
 * Verba clausa C89 (Anglica solum - vide caput)
 * ================================================== */

hic_manens constans structura {
    constans character* verbum;
    SilvaLexemaGenus    genus;
} VERBA_CLAUSA[] = {
    {"auto",     SILVA_LEX_AUTO},     {"break",    SILVA_LEX_BREAK},
    {"case",     SILVA_LEX_CASE},     {"char",     SILVA_LEX_CHAR},
    {"const",    SILVA_LEX_CONST},    {"continue", SILVA_LEX_CONTINUE},
    {"default",  SILVA_LEX_DEFAULT},  {"do",       SILVA_LEX_DO},
    {"double",   SILVA_LEX_DOUBLE},   {"else",     SILVA_LEX_ELSE},
    {"enum",     SILVA_LEX_ENUM},     {"extern",   SILVA_LEX_EXTERN},
    {"float",    SILVA_LEX_FLOAT_KW}, {"for",      SILVA_LEX_FOR},
    {"goto",     SILVA_LEX_GOTO},     {"if",       SILVA_LEX_IF},
    {"int",      SILVA_LEX_INT},      {"long",     SILVA_LEX_LONG},
    {"register", SILVA_LEX_REGISTER}, {"return",   SILVA_LEX_RETURN},
    {"short",    SILVA_LEX_SHORT},    {"signed",   SILVA_LEX_SIGNED},
    {"sizeof",   SILVA_LEX_SIZEOF},   {"static",   SILVA_LEX_STATIC},
    {"struct",   SILVA_LEX_STRUCT},   {"switch",   SILVA_LEX_SWITCH},
    {"typedef",  SILVA_LEX_TYPEDEF},  {"union",    SILVA_LEX_UNION},
    {"unsigned", SILVA_LEX_UNSIGNED}, {"void",     SILVA_LEX_VOID},
    {"volatile", SILVA_LEX_VOLATILE}, {"while",    SILVA_LEX_WHILE},
    {NIHIL,      SILVA_LEX_EOF}
};


/* ==================================================
 * Auxiliares - classificatio characterum
 * ================================================== */

interior b32
_est_cifra (character c)
{
    redde (c >= '0' && c <= '9') ? VERUM : FALSUM;
}

interior b32
_est_cifra_hex (character c)
{
    redde ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F')) ? VERUM : FALSUM;
}

interior b32
_est_littera (character c)
{
    redde ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        ? VERUM : FALSUM;
}

interior b32
_est_corpus_identificatoris (character c)
{
    redde (_est_littera(c) || _est_cifra(c)) ? VERUM : FALSUM;
}


/* ==================================================
 * Auxiliares - chorda ex fonte (visus, nulla copia)
 * chorda.datum est i8* sed fons constans est: unio castum
 * celat; nemo per visum scribit (contractus)
 * ================================================== */

interior chorda
_chorda_ex_fonte (constans character* fons, i32 mensura)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c = fons;
    c.datum = u.m;
    c.mensura = mensura;
    redde c;
}

interior chorda
_subchorda (chorda c, i32 a, i32 b)
{
    chorda pars;

    pars.datum = c.datum + a;
    pars.mensura = b - a;
    redde pars;
}


/* ==================================================
 * Auxiliares - positio et lectio cruda
 * ================================================== */

interior b32
_finis (SilvaLexator* lex)
{
    redde (lex->positus >= lex->mensura) ? VERUM : FALSUM;
}

interior character
_aspicere_crudum (SilvaLexator* lex, i32 offset)
{
    i32 pos;

    pos = lex->positus + offset;
    si (pos < ZEPHYRUM || pos >= lex->mensura)
    {
        redde '\0';
    }
    redde lex->fons[pos];
}

interior vacuum
_progredi_crudum (SilvaLexator* lex, i32 n)
{
    i32 i;

    per (i = ZEPHYRUM; i < n && !_finis(lex); i++)
    {
        si (lex->fons[lex->positus] == '\n')
        {
            lex->linea++;
            lex->columna = I;
        }
        alioquin
        {
            lex->columna++;
        }
        lex->positus++;
    }
}

/* Longitudo laminae ad positionem datam: 0 (nulla), 2 (\ \n), 3 (\ \r\n) */
interior i32
_lamina_hic (SilvaLexator* lex, i32 offset)
{
    character c;
    character c2;

    c = _aspicere_crudum(lex, offset);
    si (c != '\\')
    {
        redde ZEPHYRUM;
    }
    c2 = _aspicere_crudum(lex, offset + I);
    si (c2 == '\n')
    {
        redde II;
    }
    si (c2 == '\r' && _aspicere_crudum(lex, offset + II) == '\n')
    {
        redde III;
    }
    redde ZEPHYRUM;
}


/* ==================================================
 * Lectio EFFECTIVA - laminas transilit
 * ================================================== */

/* Character effectivus n-us ab positione currenti (laminis transilitis) */
interior character
_aspicere_eff (SilvaLexator* lex, i32 n)
{
    i32 pos;
    i32 restant;
    i32 lam;
    character c;

    pos = ZEPHYRUM;
    restant = n;
    dum (VERUM)
    {
        lam = _lamina_hic(lex, pos);
        dum (lam > ZEPHYRUM)
        {
            pos = pos + lam;
            lam = _lamina_hic(lex, pos);
        }
        si (lex->positus + pos >= lex->mensura)
        {
            redde '\0';
        }
        c = lex->fons[lex->positus + pos];
        si (restant == ZEPHYRUM)
        {
            redde c;
        }
        restant--;
        pos++;
    }
}

/* Estne finis effectivus? (solae laminae usque ad finem restant) */
interior b32
_finis_eff (SilvaLexator* lex)
{
    i32 pos;
    i32 lam;

    pos = ZEPHYRUM;
    lam = _lamina_hic(lex, pos);
    dum (lam > ZEPHYRUM)
    {
        pos = pos + lam;
        lam = _lamina_hic(lex, pos);
    }
    redde (lex->positus + pos >= lex->mensura) ? VERUM : FALSUM;
}

/* Sumere unum characterem effectivum: laminas ante eum consumit
 * et in scansione memorat */
interior vacuum
_sumere (SilvaLexator* lex, Scansio* s)
{
    i32 lam;
    SilvaScissura* locus;

    lam = _lamina_hic(lex, ZEPHYRUM);
    dum (lam > ZEPHYRUM)
    {
        si (s->scissurae == NIHIL)
        {
            s->scissurae = xar_creare(lex->piscina, magnitudo(SilvaScissura));
        }
        locus = (SilvaScissura*)xar_addere(s->scissurae);
        si (locus != NIHIL)
        {
            locus->offset = (s32)s->effectivi;
            locus->crlf = (lam == III) ? VERUM : FALSUM;
        }
        _progredi_crudum(lex, lam);
        lam = _lamina_hic(lex, ZEPHYRUM);
    }
    si (!_finis(lex))
    {
        _progredi_crudum(lex, I);
        s->effectivi++;
    }
}

interior vacuum
_scansio_incipere (SilvaLexator* lex, Scansio* s)
{
    s->initium = lex->positus;
    s->linea_initium = lex->linea;
    s->columna_initium = lex->columna;
    s->effectivi = ZEPHYRUM;
    s->scissurae = NIHIL;
}

/* Valor lexematis: visus directus si nullae laminae, aliter textus
 * mundus synthesitur (laminae exclusae) */
interior chorda
_valor_finire (SilvaLexator* lex, Scansio* s)
{
    chorda mundus;
    i8* datum;
    i32 i;
    i32 scriptum;

    si (s->scissurae == NIHIL)
    {
        redde _chorda_ex_fonte(lex->fons + s->initium,
            lex->positus - s->initium);
    }

    datum = (i8*)piscina_allocare(lex->piscina,
        (memoriae_index)s->effectivi);
    scriptum = ZEPHYRUM;
    i = s->initium;
    dum (i < lex->positus && scriptum < s->effectivi)
    {
        /* transilire laminas in textu crudo */
        si (lex->fons[i] == '\\' && i + I < lex->positus
            && lex->fons[i + I] == '\n')
        {
            i = i + II;
            perge;
        }
        si (lex->fons[i] == '\\' && i + II < lex->positus
            && lex->fons[i + I] == '\r' && lex->fons[i + II] == '\n')
        {
            i = i + III;
            perge;
        }
        datum[scriptum] = (i8)lex->fons[i];
        scriptum++;
        i++;
    }
    mundus.datum = datum;
    mundus.mensura = scriptum;
    redde mundus;
}

interior SilvaToken*
_lexema_finire (SilvaLexator* lex, Scansio* s, SilvaLexemaGenus genus)
{
    SilvaToken* token;

    token = silva_token_ex_fonte(lex->piscina, genus, _valor_finire(lex, s),
        (s32)s->initium, s->linea_initium, s->columna_initium, lex->fons_index);
    si (token != NIHIL)
    {
        token->longitudo = lex->positus - s->initium; /* longitudo CRUDA */
        token->scissurae = s->scissurae;
    }
    redde token;
}


/* ==================================================
 * Verba clausa
 * ================================================== */

interior SilvaLexemaGenus
_quaerere_verbum_clausum (chorda valor)
{
    i32 i;

    per (i = ZEPHYRUM; VERBA_CLAUSA[i].verbum != NIHIL; i++)
    {
        si ((i32)strlen(VERBA_CLAUSA[i].verbum) == valor.mensura
            && memcmp(VERBA_CLAUSA[i].verbum, valor.datum,
                   (memoriae_index)valor.mensura) == ZEPHYRUM)
        {
            redde VERBA_CLAUSA[i].genus;
        }
    }
    redde SILVA_LEX_IDENTIFICATOR;
}


/* ==================================================
 * Scanner triviae UNUS
 * Reddit lexema triviae proximum, vel NIHIL si non-trivia sequitur.
 * Valores triviae semper CRUDI (verbatim - fidelitas octetorum).
 * Commentum lineae novam lineam NUMQUAM consumit (fons divergentiae
 * arbor2 mortuus).
 * ================================================== */

interior SilvaToken*
_trivia_creare (SilvaLexator* lex, SilvaLexemaGenus genus,
                i32 initium, i32 linea_i, i32 columna_i)
{
    redde silva_token_ex_fonte(lex->piscina, genus,
        _chorda_ex_fonte(lex->fons + initium, lex->positus - initium),
        (s32)initium, linea_i, columna_i, lex->fons_index);
}

interior SilvaToken*
_trivia_proxima (SilvaLexator* lex)
{
    character c;
    character c2;
    i32 initium;
    i32 linea_i;
    i32 columna_i;
    i32 lam;
    SilvaToken* trivia;

    si (_finis(lex))
    {
        redde NIHIL;
    }

    c = _aspicere_crudum(lex, ZEPHYRUM);
    c2 = _aspicere_crudum(lex, I);
    initium = lex->positus;
    linea_i = lex->linea;
    columna_i = lex->columna;

    /* Continuatio INTER lexemata (lamina nudae positionis) */
    lam = _lamina_hic(lex, ZEPHYRUM);
    si (lam > ZEPHYRUM)
    {
        _progredi_crudum(lex, lam);
        redde _trivia_creare(lex, SILVA_LEX_CONTINUATIO,
            initium, linea_i, columna_i);
    }

    /* Spatia */
    si (c == ' ')
    {
        dum (!_finis(lex) && _aspicere_crudum(lex, ZEPHYRUM) == ' ')
        {
            _progredi_crudum(lex, I);
        }
        redde _trivia_creare(lex, SILVA_LEX_SPATIA,
            initium, linea_i, columna_i);
    }

    /* Tabulae */
    si (c == '\t')
    {
        dum (!_finis(lex) && _aspicere_crudum(lex, ZEPHYRUM) == '\t')
        {
            _progredi_crudum(lex, I);
        }
        redde _trivia_creare(lex, SILVA_LEX_TABULAE,
            initium, linea_i, columna_i);
    }

    /* Commentum clausum - lectio effectiva (apertura et clausura
     * laminari possunt); valor crudus. Non terminatum -> ad finem
     * (totalitas: numquam frangit) */
    si (_aspicere_eff(lex, ZEPHYRUM) == '/' && _aspicere_eff(lex, I) == '*')
    {
        Scansio s;

        _scansio_incipere(lex, &s);
        _sumere(lex, &s);  /* / */
        _sumere(lex, &s);  /* * */
        dum (!_finis_eff(lex))
        {
            si (_aspicere_eff(lex, ZEPHYRUM) == '*'
                && _aspicere_eff(lex, I) == '/')
            {
                _sumere(lex, &s);
                _sumere(lex, &s);
                frange;
            }
            _sumere(lex, &s);
        }
        /* laminae in triviis crudae manent - scissurae abiciuntur */
        redde _trivia_creare(lex, SILVA_LEX_COMMENTUM_CLAUSUM,
            initium, linea_i, columna_i);
    }

    /* Commentum lineae (C99 - signatum); novam lineam NON consumit */
    si (_aspicere_eff(lex, ZEPHYRUM) == '/' && _aspicere_eff(lex, I) == '/')
    {
        Scansio s;
        character ce;

        _scansio_incipere(lex, &s);
        _sumere(lex, &s);
        _sumere(lex, &s);
        dum (!_finis_eff(lex))
        {
            ce = _aspicere_eff(lex, ZEPHYRUM);
            si (ce == '\n' || ce == '\r')
            {
                frange;
            }
            _sumere(lex, &s);
        }
        trivia = _trivia_creare(lex, SILVA_LEX_COMMENTUM_LINEA,
            initium, linea_i, columna_i);
        si (trivia != NIHIL)
        {
            trivia->standard = SILVA_STANDARD_C99;
        }
        redde trivia;
    }

    /* Nova linea: \n, \r\n, \r nuda - valor verbatim */
    si (c == '\n')
    {
        _progredi_crudum(lex, I);
        redde _trivia_creare(lex, SILVA_LEX_NOVA_LINEA,
            initium, linea_i, columna_i);
    }
    si (c == '\r')
    {
        _progredi_crudum(lex, (c2 == '\n') ? II : I);
        redde _trivia_creare(lex, SILVA_LEX_NOVA_LINEA,
            initium, linea_i, columna_i);
    }

    redde NIHIL;
}


/* ==================================================
 * Lectores lexematum verorum (omnes effectivi)
 * ================================================== */

interior SilvaToken*
_legere_identificatorem (SilvaLexator* lex)
{
    Scansio s;
    SilvaToken* token;
    chorda valor;
    SilvaLexemaGenus genus;

    _scansio_incipere(lex, &s);
    dum (!_finis_eff(lex)
        && _est_corpus_identificatoris(_aspicere_eff(lex, ZEPHYRUM)))
    {
        _sumere(lex, &s);
    }

    valor = _valor_finire(lex, &s);
    genus = _quaerere_verbum_clausum(valor);

    token = silva_token_ex_fonte(lex->piscina, genus, valor,
        (s32)s.initium, s.linea_initium, s.columna_initium, lex->fons_index);
    si (token != NIHIL)
    {
        token->longitudo = lex->positus - s.initium;
        token->scissurae = s.scissurae;
    }
    redde token;
}

interior SilvaToken*
_legere_numerum (SilvaLexator* lex)
{
    Scansio s;
    character c;
    character c2;
    b32 est_hex;
    b32 est_fluitans;

    _scansio_incipere(lex, &s);
    est_hex = FALSUM;
    est_fluitans = FALSUM;

    c = _aspicere_eff(lex, ZEPHYRUM);

    /* Punctum initiale: .5 */
    si (c == '.')
    {
        est_fluitans = VERUM;
        _sumere(lex, &s);  /* . */
        dum (!_finis_eff(lex) && _est_cifra(_aspicere_eff(lex, ZEPHYRUM)))
        {
            _sumere(lex, &s);
        }
    }
    alioquin si (c == '0'
        && (_aspicere_eff(lex, I) == 'x' || _aspicere_eff(lex, I) == 'X'))
    {
        est_hex = VERUM;
        _sumere(lex, &s);
        _sumere(lex, &s);
        dum (!_finis_eff(lex) && _est_cifra_hex(_aspicere_eff(lex, ZEPHYRUM)))
        {
            _sumere(lex, &s);
        }
    }
    alioquin
    {
        /* decimales et octales: cifras omnes sumere (0-9; semantica
         * octalis posterioris curae est, lexema idem manet) */
        dum (!_finis_eff(lex) && _est_cifra(_aspicere_eff(lex, ZEPHYRUM)))
        {
            _sumere(lex, &s);
        }

        /* pars fractionalis */
        si (!_finis_eff(lex) && _aspicere_eff(lex, ZEPHYRUM) == '.'
            && _est_cifra(_aspicere_eff(lex, I)))
        {
            est_fluitans = VERUM;
            _sumere(lex, &s);
            dum (!_finis_eff(lex) && _est_cifra(_aspicere_eff(lex, ZEPHYRUM)))
            {
                _sumere(lex, &s);
            }
        }
        alioquin si (!_finis_eff(lex) && _aspicere_eff(lex, ZEPHYRUM) == '.'
            && !_est_cifra(_aspicere_eff(lex, I))
            && _aspicere_eff(lex, I) != '.')
        {
            /* 3. -> fluitans (C89: cifrae punctum sequens optionales),
             * sed 3.. relinquitur (punctum punctum) */
            est_fluitans = VERUM;
            _sumere(lex, &s);
        }
    }

    /* exponens */
    si (!est_hex)
    {
        c = _aspicere_eff(lex, ZEPHYRUM);
        si (c == 'e' || c == 'E')
        {
            c2 = _aspicere_eff(lex, I);
            si (_est_cifra(c2)
                || ((c2 == '+' || c2 == '-') && _est_cifra(_aspicere_eff(lex, II))))
            {
                est_fluitans = VERUM;
                _sumere(lex, &s);  /* e */
                c = _aspicere_eff(lex, ZEPHYRUM);
                si (c == '+' || c == '-')
                {
                    _sumere(lex, &s);
                }
                dum (!_finis_eff(lex) && _est_cifra(_aspicere_eff(lex, ZEPHYRUM)))
                {
                    _sumere(lex, &s);
                }
            }
        }
    }

    /* suffixus */
    c = _aspicere_eff(lex, ZEPHYRUM);
    si (est_fluitans)
    {
        si (c == 'f' || c == 'F' || c == 'l' || c == 'L')
        {
            _sumere(lex, &s);
        }
    }
    alioquin
    {
        si (c == 'u' || c == 'U')
        {
            _sumere(lex, &s);
            c = _aspicere_eff(lex, ZEPHYRUM);
            si (c == 'l' || c == 'L')
            {
                _sumere(lex, &s);
            }
        }
        alioquin si (c == 'l' || c == 'L')
        {
            _sumere(lex, &s);
            c = _aspicere_eff(lex, ZEPHYRUM);
            si (c == 'u' || c == 'U')
            {
                _sumere(lex, &s);
            }
        }
    }

    redde _lexema_finire(lex, &s,
        est_fluitans ? SILVA_LEX_FLOAT : SILVA_LEX_INTEGER);
}

/* Littera chordae vel characteris. terminator = '"' vel '\''.
 * Nova linea effectiva vel finis -> IMPERFECTUM (non consumitur). */
interior SilvaToken*
_legere_litteram (SilvaLexator* lex, character terminator,
                  SilvaLexemaGenus genus_perfectum,
                  SilvaLexemaGenus genus_imperfectum)
{
    Scansio s;
    character c;
    b32 perfectum;

    _scansio_incipere(lex, &s);
    perfectum = FALSUM;

    _sumere(lex, &s);  /* terminator aperiens */
    dum (!_finis_eff(lex))
    {
        c = _aspicere_eff(lex, ZEPHYRUM);
        si (c == terminator)
        {
            _sumere(lex, &s);
            perfectum = VERUM;
            frange;
        }
        si (c == '\n' || c == '\r')
        {
            frange;  /* imperfectum - novam lineam non consumere */
        }
        si (c == '\\')
        {
            _sumere(lex, &s);  /* \ */
            si (!_finis_eff(lex))
            {
                _sumere(lex, &s);  /* character effugii */
            }
            perge;
        }
        _sumere(lex, &s);
    }

    redde _lexema_finire(lex, &s,
        perfectum ? genus_perfectum : genus_imperfectum);
}

/* Interpunctio - munch maximalis, lectio effectiva */
interior SilvaToken*
_legere_interpunctionem (SilvaLexator* lex)
{
    Scansio s;
    character c;
    character c2;
    character c3;
    SilvaLexemaGenus genus;
    i32 n;

    _scansio_incipere(lex, &s);
    c  = _aspicere_eff(lex, ZEPHYRUM);
    c2 = _aspicere_eff(lex, I);
    c3 = _aspicere_eff(lex, II);
    n = I;
    genus = SILVA_LEX_OCTETUS_IGNOTUS;

    /* tres characteres */
    si (c == '.' && c2 == '.' && c3 == '.')      { genus = SILVA_LEX_ELLIPSIS; n = III; }
    alioquin si (c == '<' && c2 == '<' && c3 == '=') { genus = SILVA_LEX_SINISTRORSUM_ASSIGNATIO; n = III; }
    alioquin si (c == '>' && c2 == '>' && c3 == '=') { genus = SILVA_LEX_DEXTRORSUM_ASSIGNATIO; n = III; }
    /* duo characteres */
    alioquin si (c == '+' && c2 == '+') { genus = SILVA_LEX_INCREMENTUM; n = II; }
    alioquin si (c == '-' && c2 == '-') { genus = SILVA_LEX_DECREMENTUM; n = II; }
    alioquin si (c == '-' && c2 == '>') { genus = SILVA_LEX_SAGITTA; n = II; }
    alioquin si (c == '&' && c2 == '&') { genus = SILVA_LEX_ET_ET; n = II; }
    alioquin si (c == '|' && c2 == '|') { genus = SILVA_LEX_VEL_VEL; n = II; }
    alioquin si (c == '=' && c2 == '=') { genus = SILVA_LEX_AEQUALIS_AEQUALIS; n = II; }
    alioquin si (c == '!' && c2 == '=') { genus = SILVA_LEX_NON_AEQUALIS; n = II; }
    alioquin si (c == '<' && c2 == '=') { genus = SILVA_LEX_MINOR_AEQUALIS; n = II; }
    alioquin si (c == '>' && c2 == '=') { genus = SILVA_LEX_MAIOR_AEQUALIS; n = II; }
    alioquin si (c == '<' && c2 == '<') { genus = SILVA_LEX_SINISTRORSUM; n = II; }
    alioquin si (c == '>' && c2 == '>') { genus = SILVA_LEX_DEXTRORSUM; n = II; }
    alioquin si (c == '+' && c2 == '=') { genus = SILVA_LEX_PLUS_ASSIGNATIO; n = II; }
    alioquin si (c == '-' && c2 == '=') { genus = SILVA_LEX_MINUS_ASSIGNATIO; n = II; }
    alioquin si (c == '*' && c2 == '=') { genus = SILVA_LEX_STAR_ASSIGNATIO; n = II; }
    alioquin si (c == '/' && c2 == '=') { genus = SILVA_LEX_SOLIDUS_ASSIGNATIO; n = II; }
    alioquin si (c == '%' && c2 == '=') { genus = SILVA_LEX_PERCENTUM_ASSIGNATIO; n = II; }
    alioquin si (c == '&' && c2 == '=') { genus = SILVA_LEX_AMPERSAND_ASSIGNATIO; n = II; }
    alioquin si (c == '|' && c2 == '=') { genus = SILVA_LEX_BARRA_ASSIGNATIO; n = II; }
    alioquin si (c == '^' && c2 == '=') { genus = SILVA_LEX_CARET_ASSIGNATIO; n = II; }
    alioquin si (c == '#' && c2 == '#') { genus = SILVA_LEX_CANCELLUM_CANCELLUM; n = II; }
    /* unus character */
    alioquin si (c == '+') { genus = SILVA_LEX_PLUS; }
    alioquin si (c == '-') { genus = SILVA_LEX_MINUS; }
    alioquin si (c == '*') { genus = SILVA_LEX_STAR; }
    alioquin si (c == '/') { genus = SILVA_LEX_SOLIDUS; }
    alioquin si (c == '%') { genus = SILVA_LEX_PERCENTUM; }
    alioquin si (c == '&') { genus = SILVA_LEX_AMPERSAND; }
    alioquin si (c == '|') { genus = SILVA_LEX_BARRA; }
    alioquin si (c == '^') { genus = SILVA_LEX_CARET; }
    alioquin si (c == '~') { genus = SILVA_LEX_TILDE; }
    alioquin si (c == '!') { genus = SILVA_LEX_EXCLAMATIO; }
    alioquin si (c == '<') { genus = SILVA_LEX_MINOR; }
    alioquin si (c == '>') { genus = SILVA_LEX_MAIOR; }
    alioquin si (c == '=') { genus = SILVA_LEX_ASSIGNATIO; }
    alioquin si (c == '.') { genus = SILVA_LEX_PUNCTUM; }
    alioquin si (c == ',') { genus = SILVA_LEX_COMMA; }
    alioquin si (c == ':') { genus = SILVA_LEX_COLON; }
    alioquin si (c == ';') { genus = SILVA_LEX_SEMICOLON; }
    alioquin si (c == '?') { genus = SILVA_LEX_QUAESTIO; }
    alioquin si (c == '(') { genus = SILVA_LEX_PAREN_APERTA; }
    alioquin si (c == ')') { genus = SILVA_LEX_PAREN_CLAUSA; }
    alioquin si (c == '[') { genus = SILVA_LEX_QUADRA_APERTA; }
    alioquin si (c == ']') { genus = SILVA_LEX_QUADRA_CLAUSA; }
    alioquin si (c == '{') { genus = SILVA_LEX_BRACE_APERTA; }
    alioquin si (c == '}') { genus = SILVA_LEX_BRACE_CLAUSA; }
    alioquin si (c == '#') { genus = SILVA_LEX_CANCELLUM; }
    /* aliter: octetus ignotus (garbage, NUL, ...) - UNUS octetus */

    per (; n > ZEPHYRUM; n--)
    {
        _sumere(lex, &s);
    }

    redde _lexema_finire(lex, &s, genus);
}

/* Lexema verum proximum (numquam trivia; EOF ad finem) */
interior SilvaToken*
_lexema_proximum (SilvaLexator* lex)
{
    character c;
    character c2;

    si (_finis(lex))
    {
        chorda vacua;

        vacua.datum = NIHIL;
        vacua.mensura = ZEPHYRUM;
        redde silva_token_ex_fonte(lex->piscina, SILVA_LEX_EOF, vacua,
            (s32)lex->positus, lex->linea, lex->columna, lex->fons_index);
    }

    c = _aspicere_eff(lex, ZEPHYRUM);
    c2 = _aspicere_eff(lex, I);

    si (_est_littera(c))
    {
        redde _legere_identificatorem(lex);
    }
    si (_est_cifra(c) || (c == '.' && _est_cifra(c2)))
    {
        redde _legere_numerum(lex);
    }
    si (c == '"')
    {
        redde _legere_litteram(lex, '"',
            SILVA_LEX_STRING_LIT, SILVA_LEX_STRING_IMPERFECTUM);
    }
    si (c == '\'')
    {
        redde _legere_litteram(lex, '\'',
            SILVA_LEX_CHARACTER_LIT, SILVA_LEX_CHARACTER_IMPERFECTUM);
    }
    redde _legere_interpunctionem(lex);
}


/* ==================================================
 * Fluxus crudus + initium_lineae
 * ================================================== */

Xar*
silva_lexare_cruda (
    Piscina*            piscina,
    constans character* fons,
    i32                 mensura,
    s32                 fons_index)
{
    SilvaLexator lex;
    Xar* cruda;
    SilvaToken* token;
    SilvaToken** locus;
    b32 in_initio_lineae;

    lex.fons = fons;
    lex.mensura = mensura;
    lex.positus = ZEPHYRUM;
    lex.linea = I;
    lex.columna = I;
    lex.piscina = piscina;
    lex.fons_index = fons_index;

    cruda = xar_creare(piscina, magnitudo(SilvaToken*));
    in_initio_lineae = VERUM;

    dum (VERUM)
    {
        token = _trivia_proxima(&lex);
        si (token != NIHIL)
        {
            /* NOVA_LINEA incipit lineam logicam novam;
             * CONTINUATIO lineam logicam CONTINUAT (vexillum immutatum) */
            si (token->genus == SILVA_LEX_NOVA_LINEA)
            {
                in_initio_lineae = VERUM;
            }
            locus = (SilvaToken**)xar_addere(cruda);
            si (locus != NIHIL)
            {
                *locus = token;
            }
            perge;
        }

        token = _lexema_proximum(&lex);
        si (token == NIHIL)
        {
            frange;  /* allocatio fracta - fluxum partialem reddere */
        }
        token->initium_lineae = in_initio_lineae;
        in_initio_lineae = FALSUM;
        locus = (SilvaToken**)xar_addere(cruda);
        si (locus != NIHIL)
        {
            *locus = token;
        }
        si (token->genus == SILVA_LEX_EOF)
        {
            frange;
        }
    }

    redde cruda;
}


/* ==================================================
 * Transitus attachmenti
 * Regula: trailing lexematis praecedentis = trivia usque ad primam
 * NOVA_LINEA inclusive; reliqua = leading lexematis sequentis.
 * Trivia ante lexema primum -> leading eius.
 * ================================================== */

Xar*
silva_spatia_attachere (
    Piscina* piscina,
    Xar*     cruda)
{
    Xar* vera;
    Xar* pendentia;
    SilvaToken* token;
    SilvaToken* prior;
    SilvaToken** locus;
    i32 i;
    i32 j;
    i32 n;
    i32 divisio;

    vera = xar_creare(piscina, magnitudo(SilvaToken*));
    pendentia = xar_creare(piscina, magnitudo(SilvaToken*));
    prior = NIHIL;

    n = xar_numerus(cruda);
    per (i = ZEPHYRUM; i < n; i++)
    {
        token = *(SilvaToken**)xar_obtinere(cruda, i);

        si (token->genus == SILVA_LEX_SPATIA
            || token->genus == SILVA_LEX_TABULAE
            || token->genus == SILVA_LEX_NOVA_LINEA
            || token->genus == SILVA_LEX_CONTINUATIO
            || token->genus == SILVA_LEX_COMMENTUM_CLAUSUM
            || token->genus == SILVA_LEX_COMMENTUM_LINEA)
        {
            locus = (SilvaToken**)xar_addere(pendentia);
            si (locus != NIHIL)
            {
                *locus = token;
            }
            perge;
        }

        /* Lexema verum: pendentia dividere */
        si (xar_numerus(pendentia) > ZEPHYRUM)
        {
            /* divisio = index post primam NOVA_LINEA (si prior est);
             * aliter omnia ad leading (nihil trailing sine linea nova...
             * immo: si prior NIHIL, omnia leading) */
            divisio = ZEPHYRUM;
            si (prior != NIHIL)
            {
                divisio = xar_numerus(pendentia);  /* omnia trailing si nulla nova linea */
                per (j = ZEPHYRUM; j < xar_numerus(pendentia); j++)
                {
                    SilvaToken* t;

                    t = *(SilvaToken**)xar_obtinere(pendentia, j);
                    si (t->genus == SILVA_LEX_NOVA_LINEA)
                    {
                        divisio = j + I;
                        frange;
                    }
                }
            }

            si (divisio > ZEPHYRUM && prior != NIHIL)
            {
                prior->spatia_post = xar_creare(piscina, magnitudo(SilvaToken*));
                per (j = ZEPHYRUM; j < divisio; j++)
                {
                    locus = (SilvaToken**)xar_addere(prior->spatia_post);
                    si (locus != NIHIL)
                    {
                        *locus = *(SilvaToken**)xar_obtinere(pendentia, j);
                    }
                }
            }
            si (divisio < xar_numerus(pendentia))
            {
                token->spatia_ante = xar_creare(piscina, magnitudo(SilvaToken*));
                per (j = divisio; j < xar_numerus(pendentia); j++)
                {
                    locus = (SilvaToken**)xar_addere(token->spatia_ante);
                    si (locus != NIHIL)
                    {
                        *locus = *(SilvaToken**)xar_obtinere(pendentia, j);
                    }
                }
            }
            pendentia = xar_creare(piscina, magnitudo(SilvaToken*));
        }

        locus = (SilvaToken**)xar_addere(vera);
        si (locus != NIHIL)
        {
            *locus = token;
        }
        prior = token;
    }

    redde vera;
}


/* ==================================================
 * API principalis
 * ================================================== */

Xar*
silva_lexare (
    Piscina*            piscina,
    constans character* fons,
    i32                 mensura,
    s32                 fons_index)
{
    redde silva_spatia_attachere(piscina,
        silva_lexare_cruda(piscina, fons, mensura, fons_index));
}


/* ==================================================
 * Emissio - oraculum fidelitatis octetorum
 * ================================================== */

interior vacuum
_emittere_trivia (ChordaAedificator* aed, Xar* spatia)
{
    i32 i;
    SilvaToken* t;

    si (spatia == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(spatia); i++)
    {
        t = *(SilvaToken**)xar_obtinere(spatia, i);
        chorda_aedificator_appendere_chorda(aed, t->valor);
    }
}

interior vacuum
_emittere_valorem (ChordaAedificator* aed, SilvaToken* token)
{
    i32 i;
    i32 prius;
    SilvaScissura* sc;

    si (token->scissurae == NIHIL)
    {
        chorda_aedificator_appendere_chorda(aed, token->valor);
        redde;
    }

    prius = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(token->scissurae); i++)
    {
        sc = (SilvaScissura*)xar_obtinere(token->scissurae, i);
        chorda_aedificator_appendere_chorda(aed,
            _subchorda(token->valor, prius, (i32)sc->offset));
        chorda_aedificator_appendere_literis(aed, sc->crlf ? "\\\r\n" : "\\\n");
        prius = (i32)sc->offset;
    }
    chorda_aedificator_appendere_chorda(aed,
        _subchorda(token->valor, prius, token->valor.mensura));
}

chorda
silva_lexemata_emittere (
    Piscina* piscina,
    Xar*     lexemata)
{
    ChordaAedificator* aed;
    SilvaToken* token;
    i32 i;

    aed = chorda_aedificator_creare(piscina, 1024);
    per (i = ZEPHYRUM; i < xar_numerus(lexemata); i++)
    {
        token = *(SilvaToken**)xar_obtinere(lexemata, i);
        _emittere_trivia(aed, token->spatia_ante);
        _emittere_valorem(aed, token);
        _emittere_trivia(aed, token->spatia_post);
    }
    redde chorda_aedificator_finire(aed);
}
