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
                   i32  mensura;
                   i32  positus;
                   i32  linea;
                   i32  columna;
               Piscina* piscina;
                   s32  fons_index;
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
      SilvaLexemaGenus  genus;
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
_est_cifra (
    character c)
{
    redde (c >= '0' && c <= '9') ? VERUM : FALSUM;
}

interior b32
_est_cifra_hex (
    character c)
{
    redde ((c >= '0' && c <= '9') || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F')) ? VERUM : FALSUM;
}

interior b32
_est_littera (
    character c)
{
    redde ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_')
        ? VERUM : FALSUM;
}

interior b32
_est_corpus_identificatoris (
    character c)
{
    redde (_est_littera(c) || _est_cifra(c)) ? VERUM : FALSUM;
}


/* ==================================================
 * Auxiliares - chorda ex fonte (visus, nulla copia)
 * chorda.datum est i8* sed fons constans est: unio castum
 * celat; nemo per visum scribit (contractus)
 * ================================================== */

interior chorda
_chorda_ex_fonte (
    constans character* fons,
                   i32  mensura)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = fons;
    c.datum    = u.m;
    c.mensura  = mensura;
    redde c;
}

interior chorda
_subchorda (
    chorda c,
       i32 a,
       i32 b)
{
    chorda pars;

    pars.datum    = c.datum + a;
    pars.mensura  = b - a;
    redde pars;
}


/* ==================================================
 * Auxiliares - positio et lectio cruda
 * ================================================== */

interior b32
_finis (
    SilvaLexator* lex)
{
    redde (lex->positus >= lex->mensura) ? VERUM : FALSUM;
}

interior character
_aspicere_crudum (
    SilvaLexator* lex,
             i32  offset)
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
_progredi_crudum (
    SilvaLexator* lex,
             i32  n)
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
_lamina_hic (
    SilvaLexator* lex,
             i32  offset)
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
_aspicere_eff (
    SilvaLexator* lex,
             i32  n)
{
          i32 pos;
          i32 restant;
          i32 lam;
    character c;

    pos      = ZEPHYRUM;
    restant  = n;
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
_finis_eff (
    SilvaLexator* lex)
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
_sumere (
    SilvaLexator* lex,
         Scansio* s)
{
              i32  lam;
    SilvaScissura* locus;

    lam = _lamina_hic(lex, ZEPHYRUM);
    dum (lam > ZEPHYRUM)
    {
        si (s->scissurae == NIHIL)
        {
            s->scissurae = xar_creare(lex->piscina,
                magnitudo(SilvaScissura));
        }
        locus = (SilvaScissura*)xar_addere(s->scissurae);
        si (locus != NIHIL)
        {
            locus->offset  = (s32)s->effectivi;
            locus->crlf    = (lam == III) ? VERUM : FALSUM;
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


/* ==================================================
 * Classes characterum + cursus celeres (budget silvae gradus III,
 * 2026-11-03). Per characterem via effectiva ~III inspectiones
 * laminae (cum lectionibus limitatis) et vocationem _progredi_crudum
 * solvebat. Cursus celer characteres crudos classis datae directe
 * sumit: NULLA classis '\\' continet, ergo cursus ubi lamina incipere
 * posset cessat et via effectiva gradum unum facit - semantica eadem
 * per constructionem (sine lamina character effectivus = crudus).
 * ================================================== */

#define CLASSIS_IDENTIFICATORIS  I       /* [A-Za-z0-9_] */
#define CLASSIS_CIFRAE           II      /* [0-9] */
#define CLASSIS_HEX              IV      /* [0-9a-fA-F] */
#define CLASSIS_SPATII           VIII    /* ' ' */
#define CLASSIS_TABULAE          XVI     /* '\t' */
#define CLASSIS_CHORDAE          XXXII   /* corpus "..": non " \ \n \r */
#define CLASSIS_CHARACTERIS      LXIV    /* corpus '..': non ' \ \n \r */
#define CLASSIS_COMMENTI_CLAUSI  CXXVIII /* non * \ */
#define CLASSIS_COMMENTI_LINEAE  CCLVI   /* non \n \r \ */

hic_manens i32 _classes[CCLVI];
hic_manens b32 _classes_paratae = FALSUM;

/* Tabula ex praedicatis ipsis aedificatur (nulla classis manu
 * scripta - praedicata et tabula discordare non possunt) */
interior vacuum
_classes_parare (vacuum)
{
          i32 i;
          i32 k;
    character c;

    per (i = ZEPHYRUM; i < CCLVI; i++)
    {
        c = (character)i;
        k = ZEPHYRUM;
        si (_est_corpus_identificatoris(c))
        {
            k |= CLASSIS_IDENTIFICATORIS;
        }
        si (_est_cifra(c))
        {
            k |= CLASSIS_CIFRAE;
        }
        si (_est_cifra_hex(c))
        {
            k |= CLASSIS_HEX;
        }
        si (c == ' ')
        {
            k |= CLASSIS_SPATII;
        }
        si (c == '\t')
        {
            k |= CLASSIS_TABULAE;
        }
        si (c != '\\')
        {
            si (c != '"' && c != '\n' && c != '\r')
            {
                k |= CLASSIS_CHORDAE;
            }
            si (c != '\'' && c != '\n' && c != '\r')
            {
                k |= CLASSIS_CHARACTERIS;
            }
            si (c != '*')
            {
                k |= CLASSIS_COMMENTI_CLAUSI;
            }
            si (c != '\n' && c != '\r')
            {
                k |= CLASSIS_COMMENTI_LINEAE;
            }
        }
        _classes[i] = k;
    }
    _classes_paratae = VERUM;
}

interior i32
_classis_octeti (
    character c)
{
    redde _classes[(insignatus character)c];
}

/* Cursus celer: characteres crudi classis datae sumuntur (linea et
 * columna ut in _progredi_crudum); s NIHIL = trivia (effectivi non
 * numerantur). Aequivalens iterationibus _sumere sine lamina. */
interior vacuum
_cursum_sumere (
     SilvaLexator* lex,
          Scansio* s,
              i32  classis)
{
    i32 pos;
    i32 initium;
    i32 linea;
    i32 columna;

    pos      = lex->positus;
    initium  = pos;
    linea    = lex->linea;
    columna  = lex->columna;
    dum (   pos < lex->mensura
         && (_classis_octeti(lex->fons[pos]) & classis) != 0)
    {
        si (lex->fons[pos] == '\n')
        {
            linea++;
            columna = I;
        }
        alioquin
        {
            columna++;
        }
        pos++;
    }
    si (s != NIHIL)
    {
        s->effectivi = s->effectivi + (pos - initium);
    }
    lex->positus  = pos;
    lex->linea    = linea;
    lex->columna  = columna;
}

/* Sumere dum character EFFECTIVUS classis est: cursus celer, deinde
 * via effectiva (laminae) gradu uno, iterum */
interior vacuum
_sumere_dum (
     SilvaLexator* lex,
          Scansio* s,
              i32  classis)
{
    dum (VERUM)
    {
        _cursum_sumere(lex, s, classis);
        /* cursus ad octetum non-'\\' cessavit: is ipse effectivus est
         * et classem non habet - nulla lectio effectiva opus */
        si (   lex->positus            >= lex->mensura
            || lex->fons[lex->positus] != '\\')
        {
            redde;
        }
        si (   _finis_eff(lex)
            || (_classis_octeti(_aspicere_eff(lex, ZEPHYRUM)) & classis)
                == 0)
        {
            redde;
        }
        _sumere(lex, s);
    }
}

interior vacuum
_scansio_incipere (
    SilvaLexator* lex,
         Scansio* s)
{
    s->initium          = lex->positus;
    s->linea_initium    = lex->linea;
    s->columna_initium  = lex->columna;
    s->effectivi        = ZEPHYRUM;
    s->scissurae        = NIHIL;
}

/* Valor lexematis: visus directus si nullae laminae, aliter textus
 * mundus synthesitur (laminae exclusae) */
interior chorda
_valor_finire (
    SilvaLexator* lex,
         Scansio* s)
{
    chorda  mundus;
        i8* datum;
       i32  i;
       i32  scriptum;

    si (s->scissurae == NIHIL)
    {
        redde _chorda_ex_fonte(lex->fons + s->initium,
            lex->positus - s->initium);
    }

    datum = (i8*)piscina_allocare(lex->piscina,
        (memoriae_index)s->effectivi);
    scriptum  = ZEPHYRUM;
    i         = s->initium;
    dum (i < lex->positus && scriptum < s->effectivi)
    {
        /* transilire laminas in textu crudo */
        si (   lex->fons[i]     == '\\' && i + I < lex->positus
            && lex->fons[i + I] == '\n')
        {
            i = i + II;
            perge;
        }
        si (   lex->fons[i]     == '\\' && i + II < lex->positus
            && lex->fons[i + I] == '\r' && lex->fons[i + II] == '\n')
        {
            i = i + III;
            perge;
        }
        datum[scriptum] = (i8)lex->fons[i];
        scriptum++;
        i++;
    }
    mundus.datum    = datum;
    mundus.mensura  = scriptum;
    redde mundus;
}

interior SilvaToken*
_lexema_finire (
        SilvaLexator* lex,
             Scansio* s,
    SilvaLexemaGenus  genus)
{
    SilvaToken* token;

    token = silva_token_ex_fonte(lex->piscina, genus, _valor_finire(lex,
        s),
        (s32)s->initium, s->linea_initium, s->columna_initium,
        lex->fons_index);
    si (token != NIHIL)
    {
        silva_token_longitudo_ponere(token, lex->positus - s->initium); /* longitudo CRUDA */
        silva_token_scissurae_ponere(token, s->scissurae);
    }
    redde token;
}


/* ==================================================
 * Verba clausa
 * ================================================== */

/* longitudines verborum clausorum SEMEL computatae: olim strlen in omni
 * comparatione - ~XXXII vocationes per identificatorem, 18% temporis
 * lexatoris (silva/lexema_mensura.sh, 2026-10-15) */
hic_manens i32 _longitudines_clausorum[XL];
hic_manens b32 _longitudines_paratae = FALSUM;

interior SilvaLexemaGenus
_quaerere_verbum_clausum (
    chorda valor)
{
          i32 i;
    character c;

    si (!_longitudines_paratae)
    {
        per (i = ZEPHYRUM; VERBA_CLAUSA[i].verbum != NIHIL
            && i < XL; i++)
        {
            _longitudines_clausorum[i] =
                (i32)strlen(VERBA_CLAUSA[i].verbum);
        }
        _longitudines_paratae = VERUM;
    }
    /* verba clausa C89: II-VIII litterae minusculae, initio 'a'..'w' -
     * cetera (numeri Romani latinae, verba Latina longa, maiuscula)
     * statim identificatores */
    si (valor.mensura < II || valor.mensura > VIII)
    {
        redde SILVA_LEX_IDENTIFICATOR;
    }
    c = (character)valor.datum[ZEPHYRUM];
    si (c < 'a' || c > 'w')
    {
        redde SILVA_LEX_IDENTIFICATOR;
    }
    per (i = ZEPHYRUM; VERBA_CLAUSA[i].verbum != NIHIL; i++)
    {
        si (   _longitudines_clausorum[i]       == valor.mensura
            && VERBA_CLAUSA[i].verbum[ZEPHYRUM] == c
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
_trivia_creare (
        SilvaLexator* lex,
    SilvaLexemaGenus  genus,
                 i32  initium,
                 i32  linea_i,
                 i32  columna_i)
{
    redde silva_token_ex_fonte(lex->piscina, genus,
        _chorda_ex_fonte(lex->fons + initium, lex->positus - initium),
        (s32)initium, linea_i, columna_i, lex->fons_index);
}

interior SilvaToken*
_trivia_proxima (
    SilvaLexator* lex)
{
     character  c;
     character  c2;
           i32  initium;
           i32  linea_i;
           i32  columna_i;
           i32  lam;
    SilvaToken* trivia;

    si (_finis(lex))
    {
        redde NIHIL;
    }

    c = _aspicere_crudum(lex, ZEPHYRUM);

    /* Trivia SOLUM ab his incipit: lamina ('\\'), spatia, tabulae,
     * commentum ('/' crudum - lamina ante id iam supra reddita
     * esset), nova linea. Ceteri statim NIHIL (olim ~VI lectiones
     * effectivae ante quodque lexema verum - gradus III) */
    commutatio (c)
    {
        casus '\\':
        casus ' ':
        casus '\t':
        casus '/':
        casus '\n':
        casus '\r':
            frange;
        ordinarius:
            redde NIHIL;
    }

    c2         = _aspicere_crudum(lex, I);
    initium    = lex->positus;
    linea_i    = lex->linea;
    columna_i  = lex->columna;

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
        _cursum_sumere(lex, NIHIL, CLASSIS_SPATII);
        redde _trivia_creare(lex, SILVA_LEX_SPATIA,
            initium, linea_i, columna_i);
    }

    /* Tabulae */
    si (c == '\t')
    {
        _cursum_sumere(lex, NIHIL, CLASSIS_TABULAE);
        redde _trivia_creare(lex, SILVA_LEX_TABULAE,
            initium, linea_i, columna_i);
    }

    /* Commentum clausum - lectio effectiva (apertura et clausura
     * laminari possunt); valor crudus. Non terminatum -> ad finem
     * (totalitas: numquam frangit) */
    si (   _aspicere_eff(lex, ZEPHYRUM) == '/'
        && _aspicere_eff(lex, I)        == '*')
    {
        Scansio s;

        _scansio_incipere(lex, &s);
        _sumere(lex, &s);  /* / */
        _sumere(lex, &s);  /* * */
        dum (!_finis_eff(lex))
        {
            _cursum_sumere(lex, &s, CLASSIS_COMMENTI_CLAUSI);
            si (_finis_eff(lex))
            {
                frange;
            }
            si (   _aspicere_eff(lex, ZEPHYRUM) == '*'
                && _aspicere_eff(lex, I)        == '/')
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
    si (   _aspicere_eff(lex, ZEPHYRUM) == '/'
        && _aspicere_eff(lex, I)        == '/')
    {
          Scansio s;
        character ce;

        _scansio_incipere(lex, &s);
        _sumere(lex, &s);
        _sumere(lex, &s);
        dum (!_finis_eff(lex))
        {
            _cursum_sumere(lex, &s, CLASSIS_COMMENTI_LINEAE);
            si (_finis_eff(lex))
            {
                frange;
            }
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
            silva_token_standard_ponere(trivia, SILVA_STANDARD_C99);
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
_legere_identificatorem (
    SilvaLexator* lex)
{
             Scansio  s;
          SilvaToken* token;
              chorda  valor;
    SilvaLexemaGenus  genus;

    _scansio_incipere(lex, &s);
    _sumere_dum(lex, &s, CLASSIS_IDENTIFICATORIS);

    valor = _valor_finire(lex, &s);
    genus = _quaerere_verbum_clausum(valor);

    token = silva_token_ex_fonte(lex->piscina, genus, valor,
        (s32)s.initium, s.linea_initium, s.columna_initium,
        lex->fons_index);
    si (token != NIHIL)
    {
        silva_token_longitudo_ponere(token, lex->positus - s.initium);
        silva_token_scissurae_ponere(token, s.scissurae);
    }
    redde token;
}

interior SilvaToken*
_legere_numerum (
    SilvaLexator* lex)
{
      Scansio s;
    character c;
    character c2;
          b32 est_hex;
          b32 est_fluitans;

    _scansio_incipere(lex, &s);
    est_hex       = FALSUM;
    est_fluitans  = FALSUM;

    c = _aspicere_eff(lex, ZEPHYRUM);

    /* Punctum initiale: .5 */
    si (c == '.')
    {
        est_fluitans = VERUM;
        _sumere(lex, &s);  /* . */
        _sumere_dum(lex, &s, CLASSIS_CIFRAE);
    }
    alioquin si (   c == '0'
                 && (_aspicere_eff(lex, I) == 'x'
                 || _aspicere_eff(lex, I) == 'X'))
    {
        est_hex = VERUM;
        _sumere(lex, &s);
        _sumere(lex, &s);
        _sumere_dum(lex, &s, CLASSIS_HEX);
    }
    alioquin
    {
        /* decimales et octales: cifras omnes sumere (0-9; semantica
         * octalis posterioris curae est, lexema idem manet) */
        _sumere_dum(lex, &s, CLASSIS_CIFRAE);

        /* pars fractionalis */
        si (   !_finis_eff(lex) && _aspicere_eff(lex, ZEPHYRUM) == '.'
            && _est_cifra(_aspicere_eff(lex, I)))
        {
            est_fluitans = VERUM;
            _sumere(lex, &s);
            _sumere_dum(lex, &s, CLASSIS_CIFRAE);
        }
        alioquin si (   !_finis_eff(lex)
                     && _aspicere_eff(lex, ZEPHYRUM) == '.'
                     && !_est_cifra(_aspicere_eff(lex, I))
                     && _aspicere_eff(lex, I)        != '.')
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
            si (   _est_cifra(c2)
                || ((c2 == '+' || c2 == '-')
                    && _est_cifra(_aspicere_eff(lex, II))))
            {
                est_fluitans = VERUM;
                _sumere(lex, &s);  /* e */
                c = _aspicere_eff(lex, ZEPHYRUM);
                si (c == '+' || c == '-')
                {
                    _sumere(lex, &s);
                }
                _sumere_dum(lex, &s, CLASSIS_CIFRAE);
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
        /* Cursus suffixorum TOTUS unum lexema (M2d Chunk D):
         * pp-numerus C veri avidus est - "12LL" UNUM lexema
         * praeprocessus, vinculum ad CONVERSIONEM pertinet, non
         * ad lexationem. Scissura vetus "12L"+"L" sola strictior
         * norma ipsa erat (et lexicon L in 50 expandebat!).
         * LL/ULL C99: substratum lint per quaestionem notabit. */
        dum (c == 'u' || c == 'U' || c == 'l' || c == 'L')
        {
            _sumere(lex, &s);
            c = _aspicere_eff(lex, ZEPHYRUM);
        }
    }

    redde _lexema_finire(lex, &s,
        est_fluitans ? SILVA_LEX_FLOAT : SILVA_LEX_INTEGER);
}

/* Littera chordae vel characteris. terminator = '"' vel '\''.
 * Nova linea effectiva vel finis -> IMPERFECTUM (non consumitur). */
interior SilvaToken*
_legere_litteram (
                      SilvaLexator* lex,
                         character  terminator,
                  SilvaLexemaGenus  genus_perfectum,
                  SilvaLexemaGenus  genus_imperfectum)
{
      Scansio s;
    character c;
          b32 perfectum;

    _scansio_incipere(lex, &s);
    perfectum = FALSUM;

    _sumere(lex, &s);  /* terminator aperiens */
    dum (!_finis_eff(lex))
    {
        /* corpus purum (nec terminator, nec '\\', nec nova linea) */
        _cursum_sumere(lex, &s, (terminator == '"')
            ? CLASSIS_CHORDAE : CLASSIS_CHARACTERIS);
        si (_finis_eff(lex))
        {
            frange;
        }
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

/* Interpunctio - munch maximalis, lectio effectiva.
 * Gradus III: si nullus trium octetorum crudorum '\\' est, lectio
 * effectiva crudam aequat (lamina nulla) - lectio et progressio
 * directae; aliter via effectiva ut olim. Catena comparationum
 * (~XL) commutatione per characterem primum substituta. */
interior SilvaToken*
_legere_interpunctionem (
    SilvaLexator* lex)
{
             Scansio s;
           character c;
           character c2;
           character c3;
    SilvaLexemaGenus genus;
                 i32 n;
                 b32 crudum;
                 i32 k;

    _scansio_incipere(lex, &s);
    crudum = VERUM;
    per (k = ZEPHYRUM; k < III; k++)
    {
        si (_aspicere_crudum(lex, k) == '\\')
        {
            crudum = FALSUM;
            frange;
        }
    }
    si (crudum)
    {
        c   = _aspicere_crudum(lex, ZEPHYRUM);
        c2  = _aspicere_crudum(lex, I);
        c3  = _aspicere_crudum(lex, II);
    }
    alioquin
    {
        c   = _aspicere_eff(lex, ZEPHYRUM);
        c2  = _aspicere_eff(lex, I);
        c3  = _aspicere_eff(lex, II);
    }
    n      = I;
    genus  = SILVA_LEX_OCTETUS_IGNOTUS;

    commutatio (c)
    {
        casus '.':
            si (c2 == '.' && c3 == '.')
            { genus = SILVA_LEX_ELLIPSIS; n = III;
            }
            alioquin
            { genus = SILVA_LEX_PUNCTUM;
            }
            frange;
        casus '<':
            si (c2 == '<' && c3 == '=')
            { genus = SILVA_LEX_SINISTRORSUM_ASSIGNATIO; n = III;
            }
            alioquin si (c2 == '=')
            { genus = SILVA_LEX_MINOR_AEQUALIS; n = II;
            }
            alioquin si (c2 == '<')
            { genus = SILVA_LEX_SINISTRORSUM; n = II;
            }
            alioquin
            { genus = SILVA_LEX_MINOR;
            }
            frange;
        casus '>':
            si (c2 == '>' && c3 == '=')
            { genus = SILVA_LEX_DEXTRORSUM_ASSIGNATIO; n = III;
            }
            alioquin si (c2 == '=')
            { genus = SILVA_LEX_MAIOR_AEQUALIS; n = II;
            }
            alioquin si (c2 == '>')
            { genus = SILVA_LEX_DEXTRORSUM; n = II;
            }
            alioquin
            { genus = SILVA_LEX_MAIOR;
            }
            frange;
        casus '+':
            si (c2 == '+')
            { genus = SILVA_LEX_INCREMENTUM; n = II;
            }
            alioquin si (c2 == '=')
            { genus = SILVA_LEX_PLUS_ASSIGNATIO; n = II;
            }
            alioquin
            { genus = SILVA_LEX_PLUS;
            }
            frange;
        casus '-':
            si (c2 == '-')
            { genus = SILVA_LEX_DECREMENTUM; n = II;
            }
            alioquin si (c2 == '>')
            { genus = SILVA_LEX_SAGITTA; n = II;
            }
            alioquin si (c2 == '=')
            { genus = SILVA_LEX_MINUS_ASSIGNATIO; n = II;
            }
            alioquin
            { genus = SILVA_LEX_MINUS;
            }
            frange;
        casus '&':
            si (c2 == '&')
            { genus = SILVA_LEX_ET_ET; n = II;
            }
            alioquin si (c2 == '=')
            { genus = SILVA_LEX_AMPERSAND_ASSIGNATIO; n = II;
            }
            alioquin
            { genus = SILVA_LEX_AMPERSAND;
            }
            frange;
        casus '|':
            si (c2 == '|')
            { genus = SILVA_LEX_VEL_VEL; n = II;
            }
            alioquin si (c2 == '=')
            { genus = SILVA_LEX_BARRA_ASSIGNATIO; n = II;
            }
            alioquin
            { genus = SILVA_LEX_BARRA;
            }
            frange;
        casus '=':
            si (c2 == '=')
            { genus = SILVA_LEX_AEQUALIS_AEQUALIS; n = II;
            }
            alioquin
            { genus = SILVA_LEX_ASSIGNATIO;
            }
            frange;
        casus '!':
            si (c2 == '=')
            { genus = SILVA_LEX_NON_AEQUALIS; n = II;
            }
            alioquin
            { genus = SILVA_LEX_EXCLAMATIO;
            }
            frange;
        casus '*':
            si (c2 == '=')
            { genus = SILVA_LEX_STAR_ASSIGNATIO; n = II;
            }
            alioquin
            { genus = SILVA_LEX_STAR;
            }
            frange;
        casus '/':
            si (c2 == '=')
            { genus = SILVA_LEX_SOLIDUS_ASSIGNATIO; n = II;
            }
            alioquin
            { genus = SILVA_LEX_SOLIDUS;
            }
            frange;
        casus '%':
            si (c2 == '=')
            { genus = SILVA_LEX_PERCENTUM_ASSIGNATIO; n = II;
            }
            alioquin
            { genus = SILVA_LEX_PERCENTUM;
            }
            frange;
        casus '^':
            si (c2 == '=')
            { genus = SILVA_LEX_CARET_ASSIGNATIO; n = II;
            }
            alioquin
            { genus = SILVA_LEX_CARET;
            }
            frange;
        casus '#':
            si (c2 == '#')
            { genus = SILVA_LEX_CANCELLUM_CANCELLUM; n = II;
            }
            alioquin
            { genus = SILVA_LEX_CANCELLUM;
            }
            frange;
        casus '~': genus = SILVA_LEX_TILDE;         frange;
        casus ',': genus = SILVA_LEX_COMMA;         frange;
        casus ':': genus = SILVA_LEX_COLON;         frange;
        casus ';': genus = SILVA_LEX_SEMICOLON;     frange;
        casus '?': genus = SILVA_LEX_QUAESTIO;      frange;
        casus '(': genus = SILVA_LEX_PAREN_APERTA;  frange;
        casus ')': genus = SILVA_LEX_PAREN_CLAUSA;  frange;
        casus '[': genus = SILVA_LEX_QUADRA_APERTA; frange;
        casus ']': genus = SILVA_LEX_QUADRA_CLAUSA; frange;
        casus '{': genus = SILVA_LEX_BRACE_APERTA;  frange;
        casus '}': genus = SILVA_LEX_BRACE_CLAUSA;  frange;
        ordinarius:
            /* octetus ignotus (garbage, NUL, ...) - UNUS octetus */
            frange;
    }

    /* crudum: n octeti nec '\\' nec nova linea (trivia ante nos
     * redditur) - progressio directa; aliter via effectiva */
    si (crudum && lex->positus + n <= lex->mensura)
    {
        lex->positus  = lex->positus + n;
        lex->columna  = lex->columna + n;
        s.effectivi   = n;
    }
    alioquin
    {
        per (; n > ZEPHYRUM; n--)
        {
            _sumere(lex, &s);
        }
    }

    redde _lexema_finire(lex, &s, genus);
}

/* Lexema verum proximum (numquam trivia; EOF ad finem) */
interior SilvaToken*
_lexema_proximum (
    SilvaLexator* lex)
{
    character c;
    character c2;

    si (_finis(lex))
    {
        chorda vacua;

        vacua.datum    = NIHIL;
        vacua.mensura  = ZEPHYRUM;
        redde silva_token_ex_fonte(lex->piscina, SILVA_LEX_EOF, vacua,
            (s32)lex->positus, lex->linea, lex->columna,
            lex->fons_index);
    }

    /* sine '\\' in duobus octetis crudis effectivi = crudi */
    si (   _aspicere_crudum(lex, ZEPHYRUM) != '\\'
        && _aspicere_crudum(lex, I)        != '\\')
    {
        c   = _aspicere_crudum(lex, ZEPHYRUM);
        c2  = _aspicere_crudum(lex, I);
    }
    alioquin
    {
        c   = _aspicere_eff(lex, ZEPHYRUM);
        c2  = _aspicere_eff(lex, I);
    }

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
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   s32  fons_index)
{
    SilvaLexator   lex;
             Xar*  cruda;
      SilvaToken*  token;
      SilvaToken** locus;
             b32   in_initio_lineae;

    si (!_classes_paratae)
    {
        _classes_parare();
    }
    lex.fons        = fons;
    lex.mensura     = mensura;
    lex.positus     = ZEPHYRUM;
    lex.linea       = I;
    lex.columna     = I;
    lex.piscina     = piscina;
    lex.fons_index  = fons_index;

    cruda             = xar_creare(piscina, magnitudo(SilvaToken*));
    in_initio_lineae  = VERUM;

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
        silva_token_initium_lineae_ponere(token, in_initio_lineae);
        in_initio_lineae  = FALSUM;
        locus             = (SilvaToken**)xar_addere(cruda);
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

/* Pars pendentium [ab, ad) in latus lexematis SEMEL tradita. */
#define TRIVIA_ACERVUS XXXII

interior vacuum
_trivia_tradere (
        Piscina* piscina,
     SilvaToken* token,
            b32  post,
            Xar* pendentia,
            i32  ab,
            i32  ad)
{
     SilvaToken*  acervus[TRIVIA_ACERVUS];
     SilvaToken** series  = acervus;
            i32   n       = ad - ab;
            i32   j;

    si (n > (i32)TRIVIA_ACERVUS)
    {
        series = (SilvaToken**)piscina_allocare(piscina,
            (memoriae_index)n * magnitudo(SilvaToken*));
        si (series == NIHIL)
        {
            redde;
        }
    }
    per (j = ZEPHYRUM; j < n; j++)
    {
        series[j] = *(SilvaToken**)xar_obtinere(pendentia, ab + j);
    }
    si (post)
    {
        (vacuum)silva_token_post_ponere(piscina, token, series, n);
    }
    alioquin
    {
        (vacuum)silva_token_ante_ponere(piscina, token, series, n);
    }
}

/* Genera triviae (sex) */
interior b32
_est_trivia (
    s32 genus)
{
    commutatio (genus)
    {
        casus SILVA_LEX_SPATIA:
        casus SILVA_LEX_TABULAE:
        casus SILVA_LEX_NOVA_LINEA:
        casus SILVA_LEX_CONTINUATIO:
        casus SILVA_LEX_COMMENTUM_CLAUSUM:
        casus SILVA_LEX_COMMENTUM_LINEA:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* Pendentia = cruda[ab, i): trivia inter lexemata vera SEMPER
 * contigua sunt in fluxu crudo, ergo intervallum indicum sufficit
 * (budget silvae gradus III, 2026-11-03: olim Xar pendentium per
 * appendicem, et Xar NOVUS post quodque lexema cum triviis - xar et
 * allocatio ~LXXX% huius transitus) */
Xar*
silva_spatia_attachere (
    Piscina* piscina,
        Xar* cruda)
{
           Xar*  vera;
    SilvaToken*  token;
    SilvaToken*  prior;
    SilvaToken** locus;
           i32   i;
           i32   j;
           i32   n;
           i32   ab;
           i32   divisio;
           s32   primum_commentum   = -I;   /* in pendentibus [ab, i) */
           s32   ultimum_commentum  = -I;

    vera   = xar_creare(piscina, magnitudo(SilvaToken*));
    prior  = NIHIL;
    ab     = ZEPHYRUM;

    n = xar_numerus(cruda);
    per (i = ZEPHYRUM; i < n; i++)
    {
        token = *(SilvaToken**)xar_obtinere(cruda, i);

        si (_est_trivia(token->genus))
        {
            si (   token->genus == SILVA_LEX_COMMENTUM_CLAUSUM
                || token->genus == SILVA_LEX_COMMENTUM_LINEA)
            {
                si (primum_commentum < ZEPHYRUM)
                {
                    primum_commentum = (s32)i;
                }
                ultimum_commentum = (s32)i;
            }
            perge;
        }

        /* Lexema verum: pendentia [ab, i) dividere */
        si (i > ab)
        {
            /* divisio = index post primam NOVA_LINEA (si prior est);
             * aliter omnia trailing; si prior NIHIL, omnia leading */
            divisio = ab;
            si (prior != NIHIL)
            {
                divisio = i;
                per (j = ab; j < i; j++)
                {
                    SilvaToken* t;

                    t = *(SilvaToken**)xar_obtinere(cruda, j);
                    si (t->genus == SILVA_LEX_NOVA_LINEA)
                    {
                        divisio = j + I;
                        frange;
                    }
                }
            }

            /* Trivia SEMEL per latus traduntur (phasis V T7b): materia
             * seriem exactam copiat - pars pendentium in alveo contiguo
             * (acervus pro casu communi, piscina ultra). Latus totum
             * ponitur, ut olim Xar novus assignabatur. */
            /* SILVA_TOKEN_COMMENTUM_LEXATUM: commentum in parte cuique
             * lateri tradita (primum < divisio: prior; ultimum >=
             * divisio: lexema) */
            si (divisio > ab && prior != NIHIL)
            {
                _trivia_tradere(piscina, prior, VERUM, cruda, ab,
                    divisio);
                si (   primum_commentum >= ZEPHYRUM
                    && primum_commentum < (s32)divisio)
                {
                    prior->vexilla |= SILVA_TOKEN_COMMENTUM_LEXATUM;
                }
            }
            si (divisio < i)
            {
                _trivia_tradere(piscina, token, FALSUM, cruda, divisio,
                    i);
                si (ultimum_commentum >= (s32)divisio)
                {
                    token->vexilla |= SILVA_TOKEN_COMMENTUM_LEXATUM;
                }
            }
        }
        ab                 = i + I;
        primum_commentum   = -I;
        ultimum_commentum  = -I;

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
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
                   s32  fons_index)
{
    redde silva_spatia_attachere(piscina,
        silva_lexare_cruda(piscina, fons, mensura, fons_index));
}


/* ==================================================
 * Emissio - oraculum fidelitatis octetorum
 * ================================================== */

interior vacuum
_emittere_trivia (
      ChordaAedificator* aed,
    constans SilvaToken* token,
                    b32  post)
{
           i32  i;
           i32  n;
    SilvaToken* t;

    n = post ? silva_token_post_numerus(token)
             : silva_token_ante_numerus(token);
    per (i = ZEPHYRUM; i < n; i++)
    {
        t = post ? silva_token_post(token, i) : silva_token_ante(token,
            i);
        chorda_aedificator_appendere_chorda(aed, t->valor);
    }
}

interior vacuum
_emittere_valorem (
      ChordaAedificator* aed,
    constans SilvaToken* token)
{
              i32  i;
              i32  prius;
    SilvaScissura* sc;

    si (silva_token_scissurae(token) == NIHIL)
    {
        chorda_aedificator_appendere_chorda(aed, token->valor);
        redde;
    }

    prius = ZEPHYRUM;
    per (i = ZEPHYRUM; i
        < xar_numerus(silva_token_scissurae(token)); i++)
    {
        sc = (SilvaScissura*)xar_obtinere(silva_token_scissurae(token),
            i);
        chorda_aedificator_appendere_chorda(aed,
            _subchorda(token->valor, prius, (i32)sc->offset));
        chorda_aedificator_appendere_literis(aed,
            sc->crlf ? "\\\r\n" : "\\\n");
        prius = (i32)sc->offset;
    }
    chorda_aedificator_appendere_chorda(aed,
        _subchorda(token->valor, prius, token->valor.mensura));
}

vacuum
silva_lexema_emittere_in (
    ChordaAedificator* aed,
           SilvaToken* token)
{
    si (aed == NIHIL || token == NIHIL)
    {
        redde;
    }
    _emittere_trivia(aed, token, FALSUM);
    _emittere_valorem(aed, token);
    _emittere_trivia(aed, token, VERUM);
}

vacuum
silva_lexema_valorem_emittere_in (
      ChordaAedificator* aed,
    constans SilvaToken* token)
{
    si (aed == NIHIL || token == NIHIL)
    {
        redde;
    }
    _emittere_valorem(aed, token);
}

chorda
silva_lexemata_emittere (
    Piscina* piscina,
        Xar* lexemata)
{
    ChordaAedificator* aed;
           SilvaToken* token;
                  i32  i;

    aed = chorda_aedificator_creare(piscina, 1024);
    per (i = ZEPHYRUM; i < xar_numerus(lexemata); i++)
    {
        token = *(SilvaToken**)xar_obtinere(lexemata, i);
        silva_lexema_emittere_in(aed, token);
    }
    redde chorda_aedificator_finire(aed);
}
