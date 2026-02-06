/* lapifex_lexere.c - Implementatio Lexeris
 *
 * Tokenizat fontem textualem usans exempla in terminalibus grammaticae.
 */

#include "lapifex_lexere.h"
#include "xar.h"
#include "chorda.h"

/* ================================================
 * Functiones Auxiliares Internae
 * ================================================ */

/* Verificare si character est spatium */
hic_manens b32
_est_spatium(character c)
{
    redde (c == ' ' || c == '\t' || c == '\n' || c == '\r');
}

/* Verificare si character est cifra */
hic_manens b32
_est_cifra(character c)
{
    redde (c >= '0' && c <= '9');
}

/* Verificare si character est littera vel underscore */
hic_manens b32
_est_littera(character c)
{
    redde ((c >= 'a' && c <= 'z') ||
            (c >= 'A' && c <= 'Z') ||
            c == '_');
}

/* Verificare si character est pars corporis identificatoris */
hic_manens b32
_est_identificator_corpus(character c)
{
    redde (_est_littera(c) || _est_cifra(c));
}

/* Saltare spatia, actualizare lineam et columnam */
hic_manens vacuum
_saltare_spatium(
    constans character*  fons,
    s32                  longitudo,
    s32*                 positio,
    s32*                 linea,
    s32*                 columna)
{
    dum (*positio < longitudo && _est_spatium(fons[*positio]))
    {
        si (fons[*positio] == '\n')
        {
            (*linea)++;
            *columna = I;
        }
        alioquin
        {
            (*columna)++;
        }
        (*positio)++;
    }
}

/* Congruere literalem: comparare fontem ad exemplum literale
 * Redde: longitudo congruentiae (0 si non congruit)
 */
hic_manens s32
_congruere_literalem(
    constans character*  fons,
    s32                  longitudo,
    s32                  positio,
    chorda*              exemplum)
{
    s32 ex_mens;
    s32 i;

    si (!exemplum) redde ZEPHYRUM;

    ex_mens = (s32)exemplum->mensura;
    si (positio + ex_mens > longitudo) redde ZEPHYRUM;

    per (i = ZEPHYRUM; i < ex_mens; i++)
    {
        si (fons[positio + i] != ((constans character*)exemplum->datum)[i])
        {
            redde ZEPHYRUM;
        }
    }

    redde ex_mens;
}

/* Scandere numerum: [0-9]+
 * Redde: longitudo congruentiae (0 si non congruit), ponit valorem
 */
hic_manens s32
_scandere_numerum(
    constans character*  fons,
    s32                  longitudo,
    s32                  positio,
    s64*                 valor)
{
    s32  initium;
    s64  v;

    si (positio >= longitudo || !_est_cifra(fons[positio]))
    {
        redde ZEPHYRUM;
    }

    initium = positio;
    v = 0;

    dum (positio < longitudo && _est_cifra(fons[positio]))
    {
        v = v * 10 + (s64)(fons[positio] - '0');
        positio++;
    }

    *valor = v;
    redde positio - initium;
}

/* Scandere identificatorem: [a-zA-Z_][a-zA-Z0-9_]*
 * Redde: longitudo congruentiae (0 si non congruit)
 */
hic_manens s32
_scandere_identificatorem(
    constans character*  fons,
    s32                  longitudo,
    s32                  positio)
{
    s32 initium;

    si (positio >= longitudo || !_est_littera(fons[positio]))
    {
        redde ZEPHYRUM;
    }

    initium = positio;
    positio++;

    dum (positio < longitudo && _est_identificator_corpus(fons[positio]))
    {
        positio++;
    }

    redde positio - initium;
}

/* ================================================
 * Lexere - Functio Principalis
 * ================================================ */

LapifexLexFructus
lapifex_lexere(
    LapifexGrammatica*       grammatica,
    constans character*      fons,
    s32                      longitudo,
    Piscina*                 piscina)
{
    LapifexLexFructus  fructus;
    Xar*               signa_xar;
    s32                positio;
    s32                linea;
    s32                columna;
    i32                num_symbola;
    s32                eof_terminalis;
    i32                i;

    /* Initializare fructum */
    fructus.signa = NIHIL;
    fructus.numerus = ZEPHYRUM;
    fructus.successus = FALSUM;
    fructus.error_positio = ZEPHYRUM;
    fructus.error_linea = I;
    fructus.error_columna = I;

    si (!grammatica || !piscina)
    {
        redde fructus;
    }

    /* Si fons NIHIL, tractare ut vacuum */
    si (!fons) longitudo = ZEPHYRUM;

    signa_xar = xar_creare(piscina, (i32)magnitudo(LapifexSignum));
    num_symbola = (i32)xar_numerus(grammatica->symbola);

    /* Invenire EOF terminalem */
    eof_terminalis = -I;
    per (i = ZEPHYRUM; i < num_symbola; i++)
    {
        LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
            grammatica->symbola, i);
        si (sym && sym->est_terminale &&
            chorda_aequalis_literis(*sym->titulus, "EOF"))
        {
            eof_terminalis = sym->index;
            frange;
        }
    }

    positio = ZEPHYRUM;
    linea = I;
    columna = I;

    /* Ansa principalis lexeris */
    per (;;)
    {
        LapifexSignum* signum;
        s32  optima_longitudo;
        s32  optimus_index;
        s64  optimus_valor;

        /* Saltare spatia */
        _saltare_spatium(fons, longitudo, &positio, &linea, &columna);

        /* Finis fontis? */
        si (positio >= longitudo)
        {
            /* Emittere EOF signum */
            signum = (LapifexSignum*)xar_addere(signa_xar);
            si (signum)
            {
                signum->terminalis = eof_terminalis;
                signum->valor = 0;
            }
            frange;
        }

        /* Probare omnia terminalia cum exemplum */
        optima_longitudo = ZEPHYRUM;
        optimus_index = -I;
        optimus_valor = 0;

        per (i = ZEPHYRUM; i < num_symbola; i++)
        {
            LapifexSymbolum* sym = (LapifexSymbolum*)xar_obtinere(
                grammatica->symbola, i);
            s32  congruentia;
            s64  val;

            si (!sym || !sym->est_terminale || !sym->exemplum) perge;

            congruentia = ZEPHYRUM;
            val = 0;

            /* Distinguere genus exempli */
            si ((s32)sym->exemplum->mensura > I &&
                ((constans character*)sym->exemplum->datum)[0] == '$')
            {
                /* Scanner praedefinitus */
                si (chorda_aequalis_literis(*sym->exemplum, "$numerus"))
                {
                    congruentia = _scandere_numerum(
                        fons, longitudo, positio, &val);
                }
                alioquin si (chorda_aequalis_literis(
                    *sym->exemplum, "$identificator"))
                {
                    congruentia = _scandere_identificatorem(
                        fons, longitudo, positio);
                    /* Valor: offset in altis XXXII bits, longitudo in imis XXXII bits */
                    val = ((s64)positio << XXXII) | (s64)congruentia;
                }
            }
            alioquin
            {
                /* Literale congruens */
                congruentia = _congruere_literalem(
                    fons, longitudo, positio, sym->exemplum);
                val = 0;
            }

            /* Optima congruentia (longissima vincit, tie-break per indicem) */
            si (congruentia > ZEPHYRUM && congruentia > optima_longitudo)
            {
                optima_longitudo = congruentia;
                optimus_index = sym->index;
                optimus_valor = val;
            }
        }

        /* Nulla congruentia? Error */
        si (optima_longitudo == ZEPHYRUM)
        {
            fructus.error_positio = positio;
            fructus.error_linea = linea;
            fructus.error_columna = columna;
            redde fructus;
        }

        /* Emittere signum */
        signum = (LapifexSignum*)xar_addere(signa_xar);
        si (signum)
        {
            signum->terminalis = optimus_index;
            signum->valor = optimus_valor;
        }

        /* Avanzare positio et columna */
        {
            s32 j;
            per (j = ZEPHYRUM; j < optima_longitudo; j++)
            {
                si (fons[positio + j] == '\n')
                {
                    linea++;
                    columna = I;
                }
                alioquin
                {
                    columna++;
                }
            }
        }
        positio += optima_longitudo;
    }

    /* Copiare signa ex Xar ad tabulam planam */
    {
        s32 num_signa = (s32)xar_numerus(signa_xar);
        si (num_signa > ZEPHYRUM)
        {
            fructus.signa = (LapifexSignum*)piscina_allocare(
                piscina,
                (memoriae_index)((s32)magnitudo(LapifexSignum) * num_signa));
            xar_copiare_ad_tabulam(signa_xar, fructus.signa,
                ZEPHYRUM, (i32)num_signa);
            fructus.numerus = num_signa;
        }
    }

    fructus.successus = VERUM;
    redde fructus;
}

/* ================================================
 * Lexere et Parsare - Commoditas
 * ================================================ */

LapifexParsaturaFructus
lapifex_lexere_et_parsare(
    LapifexTabula*           tabula,
    constans character*      fons,
    s32                      longitudo,
    LapifexReductioCallback  reductio,
    vacuum*                  contextus)
{
    LapifexParsaturaFructus  fructus;
    LapifexLexFructus        lex;

    /* Initializare fructum erroris */
    fructus.valor = 0;
    fructus.successus = FALSUM;
    fructus.error.status = 0;
    fructus.error.terminalis = 0;
    fructus.error.positio = 0;
    fructus.error.est_error = FALSUM;

    si (!tabula)
    {
        fructus.error.est_error = VERUM;
        redde fructus;
    }

    /* Lexere */
    lex = lapifex_lexere(
        tabula->grammatica, fons, longitudo,
        tabula->grammatica->piscina);

    si (!lex.successus)
    {
        fructus.error.est_error = VERUM;
        fructus.error.positio = lex.error_positio;
        redde fructus;
    }

    /* Parsare */
    fructus = lapifex_parsare(
        tabula, lex.signa, lex.numerus, reductio, contextus);

    redde fructus;
}
