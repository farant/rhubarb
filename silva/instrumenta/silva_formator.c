/* silva_formator.c - machina lint formatoris (G1, tranche prima)
 *
 * Fluxus CRUDUS (silva_lexare_cruda), non expansus: trivia sedium
 * macro in fluxu expanso fonti fideles non sunt. Semantica gratis
 * ex substrato: tabulae genus proprium habent (SILVA_LEX_TABULAE),
 * ergo tabulae intra litteras chordarum sponte immunes; cursus '='
 * solum intra commenta quaeruntur, ergo vexilla ficta in litteris
 * tacent.
 */

#include "silva_formator.h"
#include "silva_token.h"
#include "silva_lexema.h"

#include <string.h>

#define VEXILLUM_MINIMUS  10
#define VEXILLUM_RECTUM   50
#define LONGITUDO_RECTA   72

/* ==================================================
 * adiutores
 * ================================================== */

interior SilvaToken*
_lexema (
    Xar* cruda,
    i32  index)
{
    redde *(SilvaToken**)xar_obtinere(cruda, index);
}

interior vacuum
_addere (
                   Xar* divergentiae,
    constans character* regula,
    constans character* nuntius,
                   i32  linea,
                   i32  columna,
                   s32  inventum,
                   s32  exspectatum)
{
    FormatorDivergentia* d;

    d = (FormatorDivergentia*)xar_addere(divergentiae);
    si (!d) redde;

    d->regula      = regula;
    d->nuntius     = nuntius;
    d->linea       = linea;
    d->columna     = columna;
    d->inventum    = inventum;
    d->exspectatum = exspectatum;
}

/* R6: claves ante parenthesim - Latinae (IDENTIFICATOR in fluxu
 * crudo: verba clausa Anglica solum lexantur) et Anglicae */
interior b32
_clavis_ante_parenthesim (
    SilvaToken* lexema)
{
    si (lexema->genus == SILVA_LEX_IF
        || lexema->genus == SILVA_LEX_WHILE
        || lexema->genus == SILVA_LEX_FOR
        || lexema->genus == SILVA_LEX_SWITCH)
    {
        redde VERUM;
    }
    si (lexema->genus != SILVA_LEX_IDENTIFICATOR) redde FALSUM;

    redde chorda_aequalis_literis(lexema->valor, "si")
        || chorda_aequalis_literis(lexema->valor, "dum")
        || chorda_aequalis_literis(lexema->valor, "per")
        || chorda_aequalis_literis(lexema->valor, "commutatio");
}

/* ==================================================
 * R14: vexilla (cursus '=' intra commentum)
 * ================================================== */

interior vacuum
_vexilla_censere (
           Xar* divergentiae,
    SilvaToken* commentum)
{
    constans i8* datum;
             i32  n;
             i32  i;
             i32  lineae_intra;
             i32  initium_lineae;

    datum          = commentum->valor.datum;
    n              = commentum->valor.mensura;
    i              = ZEPHYRUM;
    lineae_intra   = ZEPHYRUM;
    initium_lineae = ZEPHYRUM;

    dum (i < n)
    {
        si (datum[i] == '\n')
        {
            lineae_intra += I;
            i += I;
            initium_lineae = i;
            perge;
        }
        si (datum[i] == '=')
        {
            i32 initium_cursus;
            i32 longitudo;

            initium_cursus = i;
            dum (i < n && datum[i] == '=') i += I;
            longitudo = i - initium_cursus;

            si (longitudo >= VEXILLUM_MINIMUS
                && longitudo != VEXILLUM_RECTUM)
            {
                i32 columna;

                si (lineae_intra == ZEPHYRUM)
                {
                    columna = commentum->columna
                        + (initium_cursus - initium_lineae);
                }
                alioquin
                {
                    columna = initium_cursus - initium_lineae
                        + I;
                }
                _addere(divergentiae, "vexillum-quinquaginta",
                    "vexillum non quinquaginta",
                    commentum->linea + lineae_intra, columna,
                    (s32)longitudo, VEXILLUM_RECTUM);
            }
            perge;
        }
        i += I;
    }
}

/* ==================================================
 * R16: longitudo linearum (textu crudo)
 * ================================================== */

interior vacuum
_longitudinem_censere (
                   Xar* divergentiae,
    constans character* fons,
                   i32  mensura)
{
    i32 i;
    i32 linea;
    i32 initium;

    linea   = I;
    initium = ZEPHYRUM;

    per (i = ZEPHYRUM; i <= mensura; i += I)
    {
        b32 finis_lineae;
        i32 longitudo;

        finis_lineae = (i == mensura) || (fons[i] == '\n');
        si (!finis_lineae) perge;

        longitudo = i - initium;
        si (longitudo != (i32)ZEPHYRUM
            && fons[initium + longitudo - I] == '\r')
        {
            longitudo -= I;
        }
        si (longitudo > LONGITUDO_RECTA)
        {
            _addere(divergentiae, "longitudo-lxxii",
                "linea nimis longa", linea, LONGITUDO_RECTA + I,
                (s32)longitudo, LONGITUDO_RECTA);
        }

        linea += I;
        initium = i + I;
        si (i == mensura) frange;
    }
}

/* ==================================================
 * R12 (pars plagulae): linea nova finalis unica
 * ================================================== */

interior vacuum
_finem_censere (
                   Xar* divergentiae,
    constans character* fons,
                   i32  mensura)
{
    i32 lineae;
    i32 novae_caudae;
    i32 i;

    si (mensura == (i32)ZEPHYRUM) redde;

    lineae = I;
    per (i = ZEPHYRUM; i < mensura; i += I)
    {
        si (fons[i] == '\n') lineae += I;
    }

    si (fons[mensura - I] != '\n')
    {
        _addere(divergentiae, "spatia-caudae",
            "linea nova finalis deest", lineae, I, ZEPHYRUM, I);
        redde;
    }

    novae_caudae = ZEPHYRUM;
    i = mensura;
    dum (i > (i32)ZEPHYRUM
        && (fons[i - I] == '\n' || fons[i - I] == '\r'))
    {
        si (fons[i - I] == '\n') novae_caudae += I;
        i -= I;
    }
    si (novae_caudae > (i32)I)
    {
        _addere(divergentiae, "spatia-caudae",
            "lineae vacuae in fine plagulae", lineae, I,
            (s32)novae_caudae, I);
    }
}

/* ==================================================
 * introitus
 * ================================================== */

Xar*
formator_lint (
              Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    Xar* divergentiae;
    Xar* cruda;
    i32  numerus;
    i32  i;

    divergentiae = xar_creare(piscina,
        magnitudo(FormatorDivergentia));
    si (!divergentiae || !fons) redde divergentiae;

    cruda = silva_lexare_cruda(piscina, fons, mensura, ZEPHYRUM);
    numerus = cruda ? xar_numerus(cruda) : (i32)ZEPHYRUM;

    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        SilvaToken* lexema;

        lexema = _lexema(cruda, i);

        /* R5: tabulae (genus proprium - litterae immunes) */
        si (lexema->genus == SILVA_LEX_TABULAE)
        {
            _addere(divergentiae, "indentatio-quaterna",
                "tabulae pro spatiis", lexema->linea,
                lexema->columna, (s32)lexema->valor.mensura,
                ZEPHYRUM);
        }

        /* R12: spatia in cauda lineae */
        si ((lexema->genus == SILVA_LEX_SPATIA
                || lexema->genus == SILVA_LEX_TABULAE)
            && i + I < numerus)
        {
            SilvaToken* sequens;

            sequens = _lexema(cruda, i + I);
            si (sequens->genus == SILVA_LEX_NOVA_LINEA
                || sequens->genus == SILVA_LEX_EOF)
            {
                _addere(divergentiae, "spatia-caudae",
                    "spatia in cauda lineae", lexema->linea,
                    lexema->columna,
                    (s32)lexema->valor.mensura, ZEPHYRUM);
            }
        }

        /* R6: spatium unicum inter clavem et parenthesim */
        si (_clavis_ante_parenthesim(lexema)
            && i + I < numerus)
        {
            SilvaToken* sequens;

            sequens = _lexema(cruda, i + I);
            si (sequens->genus == SILVA_LEX_PAREN_APERTA)
            {
                _addere(divergentiae, "spatium-post-claves",
                    "spatium unicum post clavem exspectatum",
                    lexema->linea, lexema->columna, ZEPHYRUM,
                    I);
            }
            alioquin si (sequens->genus == SILVA_LEX_SPATIA
                && sequens->valor.mensura != (i32)I
                && i + II < numerus
                && _lexema(cruda, i + II)->genus
                    == SILVA_LEX_PAREN_APERTA)
            {
                _addere(divergentiae, "spatium-post-claves",
                    "spatium unicum post clavem exspectatum",
                    lexema->linea, lexema->columna,
                    (s32)sequens->valor.mensura, I);
            }
        }

        /* R14: vexilla intra commenta sola */
        si (lexema->genus == SILVA_LEX_COMMENTUM_CLAUSUM
            || lexema->genus == SILVA_LEX_COMMENTUM_LINEA)
        {
            _vexilla_censere(divergentiae, lexema);
        }
    }

    _longitudinem_censere(divergentiae, fons, mensura);
    _finem_censere(divergentiae, fons, mensura);

    redde divergentiae;
}
