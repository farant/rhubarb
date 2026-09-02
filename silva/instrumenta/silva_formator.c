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
#include "silva_nodus.h"
#include "silva_contextus.h"
#include "silva_parsare.h"
#include "silva_commissio.h"
#include "silva_c89_oraculum.h"
#include "silva_tabulae_c89.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

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

    d->regula       = regula;
    d->nuntius      = nuntius;
    d->linea        = linea;
    d->columna      = columna;
    d->inventum     = inventum;
    d->exspectatum  = exspectatum;

    d->numerus_emendationum = ZEPHYRUM;
}

interior vacuum
_emendationem_ponere (
    FormatorDivergentia* d,
                    i32  linea_a,
                    i32  columna_a,
                    i32  linea_b,
                    i32  columna_b,
                 chorda  insertum,
                    b32  tolerans);

/* emendationem divergentiae ULTIMAE addere (exemplar: _addere
 * tum _emendare - detector ipse editionem sanantem novit) */
interior vacuum
_emendare_ipsum (
       Xar* divergentiae,
       i32  linea_a,
       i32  columna_a,
       i32  linea_b,
       i32  columna_b,
    chorda  insertum,
       b32  tolerans)
{
    FormatorDivergentia* d;
                    i32  n;

    n = xar_numerus(divergentiae);
    si (n == (i32)ZEPHYRUM) redde;
    d = (FormatorDivergentia*)xar_obtinere(divergentiae, n - I);
    _emendationem_ponere(d, linea_a, columna_a, linea_b, columna_b,
        insertum, tolerans);
}

/* emendationem divergentiae DATAE addere (fractura R16 ordinem
 * longitudinis iam natum armat) */
interior vacuum
_emendationem_ponere (
    FormatorDivergentia* d,
                    i32  linea_a,
                    i32  columna_a,
                    i32  linea_b,
                    i32  columna_b,
                 chorda  insertum,
                    b32  tolerans)
{
    si (d->numerus_emendationum >= (i32)II) redde;

    d->emendationes[d->numerus_emendationum].linea_a    = linea_a;
    d->emendationes[d->numerus_emendationum].columna_a  = columna_a;
    d->emendationes[d->numerus_emendationum].linea_b    = linea_b;
    d->emendationes[d->numerus_emendationum].columna_b  = columna_b;
    d->emendationes[d->numerus_emendationum].tolerans   = tolerans;
    d->emendationes[d->numerus_emendationum].insertum   = insertum;
    d->numerus_emendationum                             += I;
}

/* stricta: spatium octetos spatiales solum tegere DEBET
 * (violatio = vitium detectoris, plagula tota recusatur) */
interior vacuum
_emendare (
       Xar* divergentiae,
       i32  linea_a,
       i32  columna_a,
       i32  linea_b,
       i32  columna_b,
    chorda  insertum)
{
    _emendare_ipsum(divergentiae, linea_a, columna_a, linea_b,
        columna_b, insertum, FALSUM);
}

/* tolerans: iunctura quae commentum aut continuationem licite
 * offendere potest - tunc tacite dilatatur, divergentia superest */
interior vacuum
_emendare_tolerans (
       Xar* divergentiae,
       i32  linea_a,
       i32  columna_a,
       i32  linea_b,
       i32  columna_b,
    chorda  insertum)
{
    _emendare_ipsum(divergentiae, linea_a, columna_a, linea_b,
        columna_b, insertum, VERUM);
}

/* textus inserendus: novae lineae tum spatia */
interior chorda
_textus_emendationis (
    Piscina* piscina,
        i32  novae,
        i32  spatia)
{
    chorda  c;
        i8* datum;
       i32  n;
       i32  i;

    n          = novae + spatia;
    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    si (n <= (i32)ZEPHYRUM) redde c;

    datum = (i8*)piscina_allocare(piscina, (memoriae_index)n);
    si (!datum) redde c;
    per (i = ZEPHYRUM; i < novae; i += I) datum[i] = '\n';
    per (i = novae; i < n; i += I) datum[i] = ' ';
    c.datum    = datum;
    c.mensura  = n;
    redde c;
}

/* R6: claves ante parenthesim - Latinae (IDENTIFICATOR in fluxu
 * crudo: verba clausa Anglica solum lexantur) et Anglicae */
interior b32
_clavis_ante_parenthesim (
    SilvaToken* lexema)
{
    si (   lexema->genus == SILVA_LEX_IF
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

    datum           = commentum->valor.datum;
    n               = commentum->valor.mensura;
    i               = ZEPHYRUM;
    lineae_intra    = ZEPHYRUM;
    initium_lineae  = ZEPHYRUM;

    dum (i < n)
    {
        si (datum[i] == '\n')
        {
            lineae_intra    += I;
            i               += I;
            initium_lineae  = i;
            perge;
        }
        si (datum[i] == '=')
        {
            i32 initium_cursus;
            i32 longitudo;

            initium_cursus = i;
            dum (i < n && datum[i] == '=') i += I;
            longitudo = i - initium_cursus;

            si (   longitudo >= VEXILLUM_MINIMUS
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

/* longitudines linearum (octetis, sine '\n'/'\r'), 1-basatae;
 * [0] vacua. Custodia LXXII glomerum (R7/R9/R17) lineam TOTAM
 * metitur - commentarium caudae inclusum, quod extensio nodi non
 * videt (venatio: legatus.c membra structurae LXXIV-LXXXV post
 * R7; semantica.c catenae LXXV-LXXXV post R17 A/C) */
interior constans i32*
_longitudines_metiri (
               Piscina*  piscina,
    constans character*  fons,
                   i32   mensura,
                   i32*  numerus_exitus,
          constans i32** initia_exitus)
{
    i32* tabula;
    i32* initia;
    i32  numerus;
    i32  i;
    i32  linea;
    i32  initium;

    numerus = I;
    per (i = ZEPHYRUM; i < mensura; i += I)
    {
        si (fons[i] == '\n') numerus += I;
    }
    tabula = (i32*)piscina_allocare(piscina,
        (memoriae_index)(numerus + I) * magnitudo(i32));
    initia = (i32*)piscina_allocare(piscina,
        (memoriae_index)(numerus + I) * magnitudo(i32));
    si (!tabula || !initia) redde NIHIL;
    tabula[ZEPHYRUM] = ZEPHYRUM;
    initia[ZEPHYRUM] = ZEPHYRUM;

    linea    = I;
    initium  = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= mensura; i += I)
    {
        i32 longitudo;

        si (i != mensura && fons[i] != '\n') perge;
        longitudo = i - initium;
        si (   longitudo                     != (i32)ZEPHYRUM
            && fons[initium + longitudo - I] == '\r')
        {
            longitudo -= I;
        }
        si (linea <= numerus)
        {
            tabula[linea] = longitudo;
            initia[linea] = initium;
        }
        linea    += I;
        initium  = i + I;
        si (i == mensura) frange;
    }
    *numerus_exitus  = numerus;
    *initia_exitus   = initia;
    redde tabula;
}

interior vacuum
_longitudinem_censere (
             Xar* divergentiae,
    constans i32* longitudines,
             i32  numerus_linearum)
{
    i32 linea;

    per (linea = I; linea <= numerus_linearum; linea += I)
    {
        si (longitudines[linea] > (i32)LONGITUDO_RECTA)
        {
            _addere(divergentiae, "longitudo-lxxii",
                "linea nimis longa", linea, LONGITUDO_RECTA + I,
                (s32)longitudines[linea], LONGITUDO_RECTA);
        }
    }
}


/* ==================================================
 * R12 (pars plagulae): linea nova finalis unica
 * ================================================== */

interior vacuum
_finem_censere (
               Piscina* piscina,
                   Xar* divergentiae,
    constans character* fons,
                   i32  mensura)
{
    i32 lineae;
    i32 novae_caudae;
    i32 initium_ultimae;
    i32 i;

    si (mensura == (i32)ZEPHYRUM) redde;

    lineae           = I;
    initium_ultimae  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mensura; i += I)
    {
        si (fons[i] == '\n')
        {
            lineae           += I;
            initium_ultimae  = i + I;
        }
    }

    si (fons[mensura - I] != '\n')
    {
        i32 columna_finis;

        columna_finis = mensura - initium_ultimae + I;
        _addere(divergentiae, "spatia-caudae",
            "linea nova finalis deest", lineae, I, ZEPHYRUM, I);
        _emendare(divergentiae, lineae, columna_finis, lineae,
            columna_finis,
            _textus_emendationis(piscina, I, ZEPHYRUM));
        redde;
    }

    novae_caudae  = ZEPHYRUM;
    i             = mensura;
    dum (   i > (i32)ZEPHYRUM
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
        _emendare(divergentiae, lineae - novae_caudae + I, I,
            lineae, I,
            _textus_emendationis(piscina, ZEPHYRUM, ZEPHYRUM));
    }
}


/* ==================================================
 * R13 intervalla (flumine): II vacuae ante vexillum,
 * I post, cumulus III+ vacuarum ubique nimius
 * ================================================== */

interior b32
_vexillum_continet (
    SilvaToken* lexema)
{
     constans i8* datum;
             i32  n;
             i32  i;
             i32  cursus;

    si (   lexema->genus != SILVA_LEX_COMMENTUM_CLAUSUM
        && lexema->genus != SILVA_LEX_COMMENTUM_LINEA)
    {
        redde FALSUM;
    }
    datum   = lexema->valor.datum;
    n       = lexema->valor.mensura;
    cursus  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i += I)
    {
        si (datum[i] == '=')
        {
            cursus += I;
            si (cursus >= (i32)VEXILLUM_MINIMUS) redde VERUM;
        }
        alioquin
        {
            cursus = ZEPHYRUM;
        }
    }
    redde FALSUM;
}

/* regione linearum [linea_a, linea_b] directivam quaerere (linea cuius
 * octetus primus non-spatialis '#' est, aut continuatio '\\' prioris)
 * - pro R13: acervus directivarum inter functiones separator est ut
 * vexillum, non lineae vacuae (quaestio 01M1FQ0JTN: '#define' inter
 * functiones 'N pro 1' in aeternum, emendatio tolerans tacite
 * cadebat) */
interior b32
_regio_directivam_habet (
    constans character* fons,
                   i32  mensura,
                   i32  linea_a,
                   i32  linea_b)
{
    i32 i;
    i32 linea;
    b32 initium;
    b32 continuata;

    si (linea_b < linea_a) redde FALSUM;
    linea       = I;
    initium     = VERUM;
    continuata  = FALSUM;
    per (i = ZEPHYRUM; i < mensura; i += I)
    {
        si (fons[i] == '\n')
        {
            continuata  = i > (i32)ZEPHYRUM && fons[i - I] == '\\';
            linea       += I;
            initium     = VERUM;
            si (linea > linea_b) redde FALSUM;
            perge;
        }
        si (linea < linea_a) perge;
        si (initium && fons[i] != ' ' && fons[i] != '\t')
        {
            si (fons[i] == '#' || continuata) redde VERUM;
            initium = FALSUM;
        }
    }
    redde FALSUM;
}

/* regione linearum [linea_a, linea_b] vexillum quaerere
 * (cursus '=' >= X) - pro R13 inter functiones */
interior b32
_regio_vexillum_habet (
    constans character* fons,
                   i32  mensura,
                   i32  linea_a,
                   i32  linea_b)
{
    i32 i;
    i32 linea;
    i32 cursus;

    si (linea_b < linea_a) redde FALSUM;
    linea   = I;
    cursus  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mensura; i += I)
    {
        si (fons[i] == '\n')
        {
            linea   += I;
            cursus  = ZEPHYRUM;
            si (linea > linea_b) redde FALSUM;
            perge;
        }
        si (linea < linea_a) perge;
        si (fons[i] == '=')
        {
            cursus += I;
            si (cursus >= (i32)VEXILLUM_MINIMUS) redde VERUM;
        }
        alioquin cursus = ZEPHYRUM;
    }
    redde FALSUM;
}

/* cursum novarum linearum [nl_primus, finis) ad 'servandae'
 * novas reducere: (servandae+1)-a usque ad ultimam delentur
 * (emendatio divergentiae ultimae) */
interior vacuum
_novas_delere (
    Piscina* piscina,
        Xar* divergentiae,
        Xar* cruda,
        i32  nl_primus,
        i32  finis,
        i32  servandae)
{
    SilvaToken* nl_prima_delenda;
    SilvaToken* nl_ultima;
           i32  visae;
           i32  j;

    nl_prima_delenda  = NIHIL;
    nl_ultima         = NIHIL;
    visae             = ZEPHYRUM;
    per (j = nl_primus; j < finis; j += I)
    {
        SilvaToken* t;

        t = _lexema(cruda, j);
        si (t->genus != SILVA_LEX_NOVA_LINEA) perge;
        visae += I;
        si (visae == servandae + I) nl_prima_delenda = t;
        nl_ultima = t;
    }
    si (nl_prima_delenda && nl_ultima)
    {
        _emendare(divergentiae, nl_prima_delenda->linea,
            nl_prima_delenda->columna, nl_ultima->linea,
            nl_ultima->columna + nl_ultima->valor.mensura,
            _textus_emendationis(piscina, ZEPHYRUM, ZEPHYRUM));
    }
}

interior vacuum
_intervalla_censere (
    Piscina* piscina,
        Xar* divergentiae,
        Xar* cruda)
{
    SilvaToken* prior;
           i32  numerus;
           i32  i;
           i32  nl_cursus;
           i32  nl_primus;

    prior      = NIHIL;
    numerus    = cruda ? xar_numerus(cruda) : (i32)ZEPHYRUM;
    nl_cursus  = ZEPHYRUM;
    nl_primus  = ZEPHYRUM;

    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        SilvaToken* lexema;
               i32  vacuae;

        lexema = _lexema(cruda, i);
        si (lexema->genus == SILVA_LEX_NOVA_LINEA)
        {
            si (nl_cursus == (i32)ZEPHYRUM) nl_primus = i;
            nl_cursus += I;
            perge;
        }
        si (   lexema->genus == SILVA_LEX_SPATIA
            || lexema->genus == SILVA_LEX_TABULAE
            || lexema->genus == SILVA_LEX_CONTINUATIO)
        {
            perge;
        }

        vacuae = nl_cursus != (i32)ZEPHYRUM
            ? nl_cursus - I : (i32)ZEPHYRUM;
        si (vacuae >= (i32)III)
        {
            _addere(divergentiae, "intervalla",
                "lineae vacuae nimis multae", lexema->linea, I,
                (s32)vacuae, II);
            _novas_delere(piscina, divergentiae, cruda,
                nl_primus, i, III);
        }
        si (   _vexillum_continet(lexema) && prior != NIHIL
            && vacuae < (i32)III && vacuae != (i32)II)
        {
            _addere(divergentiae, "intervalla",
                "duae lineae vacuae ante vexillum exspectatae",
                lexema->linea, lexema->columna, (s32)vacuae,
                II);
            /* emendatio solum si vexillum lineam suam incipit
             * (nl_cursus > 0); in linea codicis = non fixabile */
            si (nl_cursus != (i32)ZEPHYRUM)
            {
                _emendare(divergentiae, lexema->linea, I,
                    lexema->linea, I,
                    _textus_emendationis(piscina, II - vacuae,
                        ZEPHYRUM));
            }
        }
        si (   prior         != NIHIL && _vexillum_continet(prior)
            && lexema->genus != SILVA_LEX_EOF
            && !_vexillum_continet(lexema)
            && vacuae < (i32)III && vacuae != (i32)I)
        {
            /* vexillum sequens = regula ANTE hiatum possidet
             * (bellum unius octeti persistentiae: post I vult,
             * ante II - contradictio aeterna) */
            _addere(divergentiae, "intervalla",
                "una linea vacua post vexillum exspectata",
                lexema->linea, lexema->columna, (s32)vacuae,
                I);
            si (vacuae == (i32)ZEPHYRUM)
            {
                si (nl_cursus != (i32)ZEPHYRUM)
                {
                    _emendare(divergentiae, lexema->linea, I,
                        lexema->linea, I,
                        _textus_emendationis(piscina, I,
                            ZEPHYRUM));
                }
            }
            alioquin
            {
                /* vacuae II: unam servare (novae II) */
                _novas_delere(piscina, divergentiae, cruda,
                    nl_primus, i, II);
            }
        }
        prior      = lexema;
        nl_cursus  = ZEPHYRUM;
    }
}


/* ==================================================
 * R15 ordo-inclusionum (flumine): postulata_posix.h prima;
 * domesticae ("...") ante systemicas (<...>)
 * ================================================== */

interior b32
_continet_literas (
                 chorda  textus,
     constans character* quaesitum)
{
    i32 mensura_quaesiti;
    i32 i;

    mensura_quaesiti = (i32)strlen(quaesitum);
    si (   mensura_quaesiti == (i32)ZEPHYRUM
        || textus.mensura < mensura_quaesiti)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i + mensura_quaesiti <= textus.mensura;
        i += I)
    {
        si (memcmp(textus.datum + i, quaesitum,
            (memoriae_index)mensura_quaesiti) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior i32
_proximum_verum (
    Xar* cruda,
    i32  numerus,
    i32  index)
{
    dum (index < numerus)
    {
        SilvaLexemaGenus g;

        g = _lexema(cruda, index)->genus;
        si (   g != SILVA_LEX_SPATIA && g != SILVA_LEX_TABULAE
            && g != SILVA_LEX_NOVA_LINEA
            && g != SILVA_LEX_CONTINUATIO
            && g != SILVA_LEX_COMMENTUM_CLAUSUM
            && g != SILVA_LEX_COMMENTUM_LINEA)
        {
            redde index;
        }
        index += I;
    }
    redde numerus;
}

interior vacuum
_inclusiones_ordinem_censere (
    Xar* divergentiae,
    Xar* cruda)
{
    i32 numerus;
    i32 i;
    b32 vidi_systemicam;
    i32 numerus_inclusionum;

    numerus            = cruda ? xar_numerus(cruda)
        : (i32)ZEPHYRUM;
    vidi_systemicam      = FALSUM;
    numerus_inclusionum  = ZEPHYRUM;

    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        SilvaToken* lexema;
               i32  j;
               i32  k;

        lexema = _lexema(cruda, i);
        si (   lexema->genus != SILVA_LEX_CANCELLUM
            || !lexema->initium_lineae)
        {
            perge;
        }
        j = _proximum_verum(cruda, numerus, i + I);
        si (j >= numerus) perge;
        si (   _lexema(cruda, j)->genus != SILVA_LEX_IDENTIFICATOR
            || !chorda_aequalis_literis(
                _lexema(cruda, j)->valor, "include"))
        {
            perge;
        }
        k = _proximum_verum(cruda, numerus, j + I);
        si (k >= numerus) perge;

        si (_lexema(cruda, k)->genus == SILVA_LEX_STRING_LIT)
        {
            si (vidi_systemicam)
            {
                _addere(divergentiae, "ordo-inclusionum",
                    "inclusio domestica post systemicam",
                    lexema->linea, lexema->columna, ZEPHYRUM,
                    ZEPHYRUM);
            }
            si (   _continet_literas(_lexema(cruda, k)->valor,
                "postulata_posix")
                && numerus_inclusionum != (i32)ZEPHYRUM)
            {
                _addere(divergentiae, "ordo-inclusionum",
                    "postulata_posix.h prima inclusio esse"
                    " debet", lexema->linea, lexema->columna,
                    ZEPHYRUM, ZEPHYRUM);
            }
            numerus_inclusionum += I;
        }
        alioquin si (_lexema(cruda, k)->genus
                     == SILVA_LEX_MINOR)
        {
            vidi_systemicam      = VERUM;
            numerus_inclusionum  += I;
        }
    }
}


/* ==================================================
 * pars arboris - regulae structurales (tranche II.a:
 * R1 typus-in-linea-sua, R2 spatium-definitionis,
 * R3 bracchia-allman, R4 custos-una-linea,
 * R8 parametra-singula)
 *
 * CAUTIO positionum: lexemata arboris EXPANSA sunt et campos
 * lexicales def-sitos ferunt (si->if lineam latinae monstrat!).
 * Ergo omnis inspectio positionis per radicem originis it:
 * silva_token_radix directe, extensionem_lineis intra se.
 * ================================================== */

nomen structura {
             Xar* divergentiae;
         Piscina* piscina;
             Xar* continuationes;   /* ContinuatioSpatium (R11) */
             Xar* sententiae;       /* ContinuatioSpatium OMNIUM
                                     * sententiarum (uni-lineares
                                     * quoque) - fractura R16 */
             Xar* catena_ops;       /* CatenaSedes (R17 vindicata) */
             s32  fons_princeps;
    constans i32* longitudines;     /* [numerus_linearum + I],
                                     * 1-basatae - custodia LXXII */
    constans i32* initia_linearum;  /* offset initii lineae - ancorae
                                     * nuntiorum */
                   i32  numerus_linearum;
    constans character* fons;
} FormatorAmbitus;

interior i32
_longitudo_lineae (
    constans FormatorAmbitus* ambitus,
                         i32  linea)
{
    si (   !ambitus->longitudines || linea < (i32)I
        || linea > ambitus->numerus_linearum)
    {
        redde ZEPHYRUM;
    }
    redde ambitus->longitudines[linea];
}

/* visus textus lineae [columna_a, columna_b) (1-basatae, b exclusiva),
 * ad lineam clausus; reddit mensuram (ZEPHYRUM = nihil) */
interior i32
_textus_lineae (
    constans FormatorAmbitus*  ambitus,
                         i32   linea,
                         i32   columna_a,
                         i32   columna_b,
          constans character** textus)
{
    i32 longitudo;

    *textus = "";
    si (   !ambitus->fons || !ambitus->initia_linearum
        || linea < (i32)I || linea > ambitus->numerus_linearum)
    {
        redde ZEPHYRUM;
    }
    longitudo = ambitus->longitudines[linea];
    si (columna_a < (i32)I) columna_a = I;
    si (columna_b > longitudo + I) columna_b = longitudo + I;
    si (columna_b <= columna_a) redde ZEPHYRUM;
    *textus = ambitus->fons + ambitus->initia_linearum[linea]
        + columna_a - I;
    redde columna_b - columna_a;
}

/* idem, ab octeto primo non-spatiali lineae (sinistrum sententiae) */
interior i32
_textus_lineae_sine_indentatione (
    constans FormatorAmbitus*  ambitus,
                         i32   linea,
                         i32   columna_b,
          constans character** textus)
{
    i32 n;
    i32 i;

    n = _textus_lineae(ambitus, linea, I, columna_b, textus);
    i = ZEPHYRUM;
    dum (i < n && ((*textus)[i] == ' ' || (*textus)[i] == '\t'))
    {
        i += I;
    }
    *textus += i;
    redde n - i;
}

/* nuntius cum ancora: "<basis> (ancora '<textus>' l.<linea><cauda>)" -
 * textus ad XXXII octetos decurtatus. Ancora = membrum ad quod
 * ceterae ordinantur (typus latissimus, sinistrum longissimum);
 * cauda = regula ipsa (' + II', ': typus latissimus') - lector
 * regulam non iterum derivet (ter male derivata 2026-09-01). Piscina
 * fracta = basis nuda. */
interior constans character*
_nuntius_ancorae (
               Piscina* piscina,
    constans character* basis,
    constans character* ancora,
                   i32  mensura_ancorae,
                   i32  linea,
    constans character* cauda)
{
             character* buffer;
    constans character* ellipsis;
        memoriae_index  capacitas;

    ellipsis = "";
    si (mensura_ancorae > (i32)32)
    {
        mensura_ancorae  = 32;
        ellipsis         = "...";
    }
    capacitas = strlen(basis) + strlen(cauda)
        + (memoriae_index)mensura_ancorae + 48;
    buffer = (character*)piscina_allocare(piscina, capacitas);
    si (!buffer) redde basis;
    sprintf(buffer, "%s (ancora '%.*s%s' l.%u%s)", basis,
        (integer)mensura_ancorae, ancora, ellipsis,
        (insignatus integer)linea, cauda);
    redde buffer;
}

/* R17: sedes operatoris comparationis a catena vindicata -
 * R10-ante ordinationi eius cedit */
nomen structura {
    i32 linea;
    i32 columna;
} CatenaSedes;

/* R11: extensio sententiae multi-linearis - lineae internae
 * continuationes sunt (indentatio >= ca + IV postulata) */
nomen structura {
    i32 la;
    i32 ca;
    i32 lb;
} ContinuatioSpatium;

interior vacuum
_spatium_continuationis_addere (
    FormatorAmbitus* ambitus,
                i32  la,
                i32  ca,
                i32  lb)
{
    ContinuatioSpatium* spatium;

    si (lb < la) redde;
    /* sententiae omnes (uni-lineares quoque): fractura R16
     * sententiam intimam lineam continentem quaerit */
    si (ambitus->sententiae)
    {
        spatium = (ContinuatioSpatium*)xar_addere(
            ambitus->sententiae);
        si (spatium)
        {
            spatium->la = la;
            spatium->ca = ca;
            spatium->lb = lb;
        }
    }
    si (!ambitus->continuationes || lb <= la) redde;
    spatium = (ContinuatioSpatium*)xar_addere(
        ambitus->continuationes);
    si (!spatium) redde;
    spatium->la = la;
    spatium->ca = ca;
    spatium->lb = lb;
}

interior SilvaNodus*
_valor_nodus (
    SilvaValor valor)
{
    redde valor.genus == SILVA_VALOR_NODUS
        ? valor.datum.nodus : NIHIL;
}

interior SilvaToken*
_valor_radix (
    SilvaValor valor)
{
    si (   valor.genus       != SILVA_VALOR_TOKEN
        || valor.datum.token == NIHIL)
    {
        redde NIHIL;
    }
    redde silva_token_radix(valor.datum.token);
}

/* lexema FONTIS solum: expansa NIHIL reddunt. Operatores e
 * macro expansi radicem ad sedem INVOCATIONIS collabunt -
 * geometria eorum absona est (venatio flatura: -21 'pro' 0 in
 * FLATURA_SCRIBERE_BITS(...)) et iudicari nequit. */
interior SilvaToken*
_token_fons (
    SilvaValor valor)
{
    si (   valor.genus       != SILVA_VALOR_TOKEN
        || valor.datum.token == NIHIL
        || !silva_token_est_fons(valor.datum.token))
    {
        redde NIHIL;
    }
    redde valor.datum.token;
}

interior b32
_extensio (
    constans SilvaNodus* nodus,
                    s32  fons_index,
                    i32* linea_a,
                    i32* columna_a,
                    i32* linea_b,
                    i32* columna_b)
{
    si (!nodus) redde FALSUM;
    silva_nodus_extensionem_lineis(nodus, fons_index, linea_a,
        columna_a, linea_b, columna_b);
    redde *linea_a != (i32)ZEPHYRUM;
}

/* custodia fontis: lexema fontis principis est? Capitibus
 * praebitis arbor contentum capitum quoque fert - divergentiae
 * ad plagulam iudicatam solam pertinent. */
interior b32
_principalis (
    FormatorAmbitus* ambitus,
         SilvaToken* lexema)
{
    redde lexema != NIHIL
        && lexema->fons_index == ambitus->fons_princeps;
}

/* catenam declaratoris descendere ad FUNCTIONIS extimam */
interior SilvaNodus*
_declarator_functionis (
    SilvaNodus* declarator)
{
    dum (declarator)
    {
        commutatio (declarator->genus)
        {
            casus SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS:
                redde declarator;
            casus SILVA_C89_GENUS_DECLARATOR_MONSTRATOR:
                declarator = _valor_nodus(
                    silva_c89_declarator_monstrator_internum(
                        declarator));
                frange;
            casus SILVA_C89_GENUS_PARENTHESIS:
                declarator = _valor_nodus(
                    silva_c89_parenthesis_internum(declarator));
                frange;
            casus SILVA_C89_GENUS_DECLARATOR_ACIEI:
                declarator = _valor_nodus(
                    silva_c89_declarator_aciei_internum(
                        declarator));
                frange;
            ordinarius:
                redde NIHIL;
        }
    }
    redde NIHIL;
}

/* titulum declaratoris invenire (radix originis reddita) */
interior SilvaToken*
_titulus_declaratoris (
    SilvaNodus* declarator)
{
    dum (declarator)
    {
        commutatio (declarator->genus)
        {
            casus SILVA_C89_GENUS_DECLARATOR_TITULUS:
                redde _valor_radix(
                    silva_c89_declarator_titulus_tok_titulus(
                        declarator));
            casus SILVA_C89_GENUS_DECLARATOR_MONSTRATOR:
                declarator = _valor_nodus(
                    silva_c89_declarator_monstrator_internum(
                        declarator));
                frange;
            casus SILVA_C89_GENUS_PARENTHESIS:
                declarator = _valor_nodus(
                    silva_c89_parenthesis_internum(declarator));
                frange;
            casus SILVA_C89_GENUS_DECLARATOR_ACIEI:
                declarator = _valor_nodus(
                    silva_c89_declarator_aciei_internum(
                        declarator));
                frange;
            casus SILVA_C89_GENUS_DECLARATOR_FUNCTIONIS:
                declarator = _valor_nodus(
                    silva_c89_declarator_functionis_internum(
                        declarator));
                frange;
            ordinarius:
                redde NIHIL;
        }
    }
    redde NIHIL;
}

/* parametrum '(vacuum)' - exceptio R8 (in linea manet) */
interior b32
_parametrum_vacuum (
    SilvaNodus* parametrum)
{
    SilvaValor  specificatores;
    SilvaValor* primus;
    SilvaNodus* nodus_primus;
    SilvaValor  verba;
    SilvaValor* verbum;

    si (!parametrum) redde FALSUM;
    si (_valor_nodus(silva_c89_parametrum_declarator(parametrum))
        != NIHIL)
    {
        redde FALSUM;
    }
    specificatores =
        silva_c89_parametrum_specificatores(parametrum);
    si (silva_valor_lista_numerus(specificatores) != (i32)I)
    {
        redde FALSUM;
    }
    primus = silva_valor_lista_obtinere(specificatores,
        ZEPHYRUM);
    nodus_primus = primus ? _valor_nodus(*primus) : NIHIL;
    si (   !nodus_primus || nodus_primus->genus
        != SILVA_C89_GENUS_TYPUS_PRIMITIVUS)
    {
        redde FALSUM;
    }
    verba = silva_c89_typus_primitivus_tok_verba(nodus_primus);
    si (silva_valor_lista_numerus(verba) != (i32)I) redde FALSUM;
    verbum = silva_valor_lista_obtinere(verba, ZEPHYRUM);
    redde verbum != NIHIL
        && verbum->genus == SILVA_VALOR_TOKEN
        && verbum->datum.token != NIHIL
        && verbum->datum.token->genus == SILVA_LEX_VOID;
}

/* R3: bracchia corporis in lineis suis, columna possessoris */
interior vacuum
_corpus_censere (
        FormatorAmbitus* ambitus,
             SilvaNodus* corpus,
    constans SilvaNodus* possessor)
{
    SilvaToken* aperta;
    SilvaToken* clausa;
           i32  la;
           i32  ca;
           i32  lb;
           i32  cb;
           b32  situs;

    si (!corpus || corpus->genus != SILVA_C89_GENUS_CORPUS)
    {
        redde;
    }

    aperta = _valor_radix(silva_c89_corpus_tok_aperta(corpus));
    clausa = _valor_radix(silva_c89_corpus_tok_clausa(corpus));
    situs  = _extensio(possessor, ambitus->fons_princeps,
        &la, &ca, &lb, &cb);

    si (_principalis(ambitus, aperta))
    {
        si (!aperta->initium_lineae)
        {
            _addere(ambitus->divergentiae, "bracchia-allman",
                "brachium apertum in linea sua exspectatum",
                aperta->linea, aperta->columna,
                (s32)aperta->columna, I);
            si (situs)
            {
                _emendare(ambitus->divergentiae, aperta->linea,
                    aperta->columna, aperta->linea,
                    aperta->columna,
                    _textus_emendationis(ambitus->piscina, I,
                        ca - I));
            }
        }
        alioquin si (situs && aperta->columna != ca)
        {
            _addere(ambitus->divergentiae, "bracchia-allman",
                "columna brachii aperti", aperta->linea,
                aperta->columna, (s32)aperta->columna, (s32)ca);
            _emendare(ambitus->divergentiae, aperta->linea, I,
                aperta->linea, aperta->columna,
                _textus_emendationis(ambitus->piscina,
                    ZEPHYRUM, ca - I));
        }
    }
    si (_principalis(ambitus, clausa))
    {
        si (!clausa->initium_lineae)
        {
            _addere(ambitus->divergentiae, "bracchia-allman",
                "brachium clausum in linea sua exspectatum",
                clausa->linea, clausa->columna,
                (s32)clausa->columna, I);
            si (situs)
            {
                _emendare(ambitus->divergentiae, clausa->linea,
                    clausa->columna, clausa->linea,
                    clausa->columna,
                    _textus_emendationis(ambitus->piscina, I,
                        ca - I));
            }
        }
        alioquin si (situs && clausa->columna != ca)
        {
            _addere(ambitus->divergentiae, "bracchia-allman",
                "columna brachii clausi", clausa->linea,
                clausa->columna, (s32)clausa->columna, (s32)ca);
            _emendare(ambitus->divergentiae, clausa->linea, I,
                clausa->linea, clausa->columna,
                _textus_emendationis(ambitus->piscina,
                    ZEPHYRUM, ca - I));
        }
    }
}

/* R4: ramus sine brachiis in linea ancorae (')' aut 'alioquin') */
interior vacuum
_ramum_censere (
    FormatorAmbitus* ambitus,
         SilvaValor  ramus,
         SilvaToken* ancora)
{
    SilvaNodus* nodus;
           i32  la;
           i32  ca;
           i32  lb;
           i32  cb;

    nodus = _valor_nodus(ramus);
    si (!nodus || !_principalis(ambitus, ancora)) redde;
    si (nodus->genus == SILVA_C89_GENUS_CORPUS) redde;
    si (!_extensio(nodus, ambitus->fons_princeps,
        &la, &ca, &lb, &cb))
    {
        redde;
    }
    si (la != ancora->linea)
    {
        _addere(ambitus->divergentiae, "custos-una-linea",
            "corpus sine brachiis in linea conditionis"
            " exspectatum", la, ca, (s32)la,
            (s32)ancora->linea);
        /* iunctura tolerans: commentum aut continuatio in
         * spatio eam licite obstruit (tunc residua manet) */
        _emendare_tolerans(ambitus->divergentiae,
            ancora->linea,
            ancora->columna + ancora->valor.mensura, la, ca,
            _textus_emendationis(ambitus->piscina, ZEPHYRUM,
                I));
    }
}

/* R1 + R2 + R8 super caput functionis (definitionis AUT
 * prototypi - nucleus communis; porta prototyporum 2a clausa) */
interior vacuum
_functionis_caput_censere (
    FormatorAmbitus* ambitus,
         SilvaNodus* functionis)
{
     SilvaToken* titulus;
     SilvaToken* apertum;
     SilvaValor  parametra;
            i32  numerus;
            i32  prior_linea;
            i32  i;

    si (!functionis) redde;

    titulus = _titulus_declaratoris(_valor_nodus(
        silva_c89_declarator_functionis_internum(functionis)));
    apertum = _valor_radix(
        silva_c89_declarator_functionis_tok_apertum(functionis));
    si (!_principalis(ambitus, titulus)) redde;

    /* R1: titulus in columna prima lineae suae */
    si (   titulus && (!titulus->initium_lineae
        || titulus->columna != (i32)I))
    {
        _addere(ambitus->divergentiae, "typus-in-linea-sua",
            "titulus functionis in columna prima lineae suae"
            " exspectatus", titulus->linea, titulus->columna,
            (s32)titulus->columna, I);
        si (!titulus->initium_lineae)
        {
            /* titulus post specificatores: linea nova ante */
            _emendare(ambitus->divergentiae, titulus->linea,
                titulus->columna, titulus->linea,
                titulus->columna,
                _textus_emendationis(ambitus->piscina, I,
                    ZEPHYRUM));
        }
        alioquin
        {
            /* linea sua, columna prava: indentatio deleta */
            _emendare(ambitus->divergentiae, titulus->linea, I,
                titulus->linea, titulus->columna,
                _textus_emendationis(ambitus->piscina,
                    ZEPHYRUM, ZEPHYRUM));
        }
    }

    /* R2 (definitio): spatium unicum ante parenthesim */
    si (titulus && apertum && apertum->linea == titulus->linea)
    {
        i32 finis_tituli;

        finis_tituli = titulus->columna + titulus->valor.mensura;
        si (apertum->columna != finis_tituli + I)
        {
            _addere(ambitus->divergentiae,
                "spatium-definitionis",
                "spatium unicum inter titulum et parenthesim"
                " definitionis", apertum->linea,
                apertum->columna,
                (s32)(apertum->columna - finis_tituli), I);
            _emendare_tolerans(ambitus->divergentiae,
                apertum->linea, finis_tituli, apertum->linea,
                apertum->columna,
                _textus_emendationis(ambitus->piscina,
                    ZEPHYRUM, I));
        }
    }

    /* R8: parametra singula ('(vacuum)' exceptum) */
    parametra = silva_c89_declarator_functionis_parametra(
        functionis);
    numerus = silva_valor_lista_numerus(parametra);
    si (numerus == (i32)I)
    {
        SilvaValor* elementum;

        elementum = silva_valor_lista_obtinere(parametra,
            ZEPHYRUM);
        si (   elementum && _parametrum_vacuum(
            _valor_nodus(*elementum)))
        {
            redde;
        }
    }
    prior_linea = apertum ? apertum->linea : (i32)ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        SilvaValor* elementum;
        SilvaNodus* parametrum;
               i32  la;
               i32  ca;
               i32  lb;
               i32  cb;

        elementum = silva_valor_lista_obtinere(parametra, i);
        parametrum = elementum ? _valor_nodus(*elementum)
            : NIHIL;
        si (!_extensio(parametrum, ambitus->fons_princeps,
            &la, &ca, &lb, &cb))
        {
            perge;
        }
        si (la <= prior_linea)
        {
            _addere(ambitus->divergentiae, "parametra-singula",
                "parametrum in linea sua exspectatum", la, ca,
                (s32)la, (s32)(prior_linea + I));
            /* indentatio provisoria IV - R7 (G2c) ordinabit */
            _emendare(ambitus->divergentiae, la, ca, la, ca,
                _textus_emendationis(ambitus->piscina, I, IV));
        }
        prior_linea = lb;
    }
}

/* prototypum verum: declaratio cum declaratore functionis cuius
 * internum TITULUS est (monstratores functionum - internum
 * PARENTHESIS - excluduntur; R7 eos exemptos vult) */
interior SilvaNodus*
_prototypi_functionis (
    SilvaNodus* declaratio)
{
    SilvaValor  declaratores;
    SilvaValor* d;
    SilvaNodus* declarator;
    SilvaNodus* functionis;
    SilvaNodus* internum;

    si (   !declaratio
        || declaratio->genus != SILVA_C89_GENUS_DECLARATIO)
    {
        redde NIHIL;
    }
    declaratores = silva_c89_declaratio_declaratores(declaratio);
    si (silva_valor_lista_numerus(declaratores) != (i32)I)
    {
        redde NIHIL;
    }
    d           = silva_valor_lista_obtinere(declaratores, ZEPHYRUM);
    declarator  = d ? _valor_nodus(*d) : NIHIL;
    functionis  = _declarator_functionis(declarator);
    si (!functionis) redde NIHIL;
    internum = _valor_nodus(
        silva_c89_declarator_functionis_internum(functionis));
    si (   !internum || internum->genus
        != SILVA_C89_GENUS_DECLARATOR_TITULUS)
    {
        redde NIHIL;
    }
    redde functionis;
}

/* R2 (vocatio): nullum spatium ante parenthesim */
interior vacuum
_vocationem_censere (
    FormatorAmbitus* ambitus,
         SilvaNodus* vocatio)
{
    SilvaNodus* functio;
    SilvaToken* apertum;
           i32  la;
           i32  ca;
           i32  lb;
           i32  cb;

    functio = _valor_nodus(silva_c89_vocatio_functio(vocatio));
    /* parenthesis FONTIS sola iudicatur: vocatio in corpore macri
     * lexemata omnia ad sedem invocationis collabit - hiatus
     * mensuratus mendax (negativus; XLVI ordines falsi e macris
     * CREDO in probationibus). Extensio functionis quoque fida
     * esse debet (quaestio silvae, non custos localis). */
    apertum = _token_fons(
        silva_c89_vocatio_tok_apertum(vocatio));
    si (!functio || !_principalis(ambitus, apertum)) redde;
    si (!silva_nodus_geometria_fida(ambitus->piscina, functio,
        ambitus->fons_princeps))
    {
        redde;
    }
    si (!_extensio(functio, ambitus->fons_princeps,
        &la, &ca, &lb, &cb))
    {
        redde;
    }
    si (apertum->linea == lb && apertum->columna != cb)
    {
        _addere(ambitus->divergentiae, "spatium-definitionis",
            "nullum spatium ante parenthesim vocationis"
            " exspectatum", apertum->linea, apertum->columna,
            (s32)(apertum->columna - cb), ZEPHYRUM);
        _emendare_tolerans(ambitus->divergentiae,
            apertum->linea, cb, apertum->linea,
            apertum->columna,
            _textus_emendationis(ambitus->piscina, ZEPHYRUM,
                ZEPHYRUM));
    }
}


/* ==================================================
 * R7 columnae-binae: ordo declarationum duabus columnis
 * (typi dextre ordinati, tituli sinistre; hiatus =
 * I + stellae maximae; stella in hiatu, non in columna
 * typorum). cb = una POST oram typi, ergo: stella ad cb,
 * tituli ad cb + hiatus.
 * ================================================== */

nomen structura {
    i32 linea;
    i32 ca;              /* columna initii specificatorum */
    i32 cb;              /* una post oram typorum */
    i32 stellae;
    i32 stella_prima;    /* columna; 0 = nulla */
    i32 titulus_columna;
} R7Membrum;

#define R7_MEMBRA_MAXIMA 128

/* extensio valoris (token aut nodus aut lista) in lineis;
 * cb = una post finem */
interior b32
_valoris_extensio (
    FormatorAmbitus* ambitus,
         SilvaValor  valor,
                i32* linea_a,
                i32* columna_a,
                i32* linea_b,
                i32* columna_b)
{
    si (valor.genus == SILVA_VALOR_TOKEN)
    {
        SilvaToken* radix;

        radix = _valor_radix(valor);
        si (!radix) redde FALSUM;
        *linea_a    = radix->linea;
        *columna_a  = radix->columna;
        *linea_b    = radix->linea;
        *columna_b  = radix->columna + radix->valor.mensura;
        redde VERUM;
    }
    si (valor.genus == SILVA_VALOR_NODUS)
    {
        redde _extensio(valor.datum.nodus,
            ambitus->fons_princeps, linea_a, columna_a,
            linea_b, columna_b);
    }
    si (valor.genus == SILVA_VALOR_LISTA)
    {
               i32  numerus;
        SilvaValor* primus;
        SilvaValor* ultimus;
               i32  lx;
               i32  cx;

        numerus = silva_valor_lista_numerus(valor);
        si (numerus == (i32)ZEPHYRUM) redde FALSUM;
        primus  = silva_valor_lista_obtinere(valor, ZEPHYRUM);
        ultimus = silva_valor_lista_obtinere(valor,
            numerus - I);
        si (!primus || !ultimus) redde FALSUM;
        si (!_valoris_extensio(ambitus, *primus, linea_a,
            columna_a, &lx, &cx))
        {
            redde FALSUM;
        }
        redde _valoris_extensio(ambitus, *ultimus, &lx, &cx,
            linea_b, columna_b);
    }
    redde FALSUM;
}

/* catenam metiri: stellae + stella prima + titulus.
 * FALSUM = membrum exemptum (functio-monstrator,
 * qualificatores post stellam, catena ignota) */
interior b32
_declaratorem_metiri (
    SilvaNodus*  declarator,
           i32*  stellae,
           i32*  stella_prima,
    SilvaToken** titulus)
{
    *stellae       = ZEPHYRUM;
    *stella_prima  = ZEPHYRUM;
    *titulus       = NIHIL;

    dum (declarator)
    {
        commutatio (declarator->genus)
        {
            casus SILVA_C89_GENUS_DECLARATOR_INITIATUS:
                declarator = _valor_nodus(
                    silva_c89_declarator_initiatus_declarator(
                        declarator));
                frange;
            casus SILVA_C89_GENUS_DECLARATOR_MONSTRATOR:
            {
                SilvaToken* stella;

                si (silva_valor_lista_numerus(
                    silva_c89_declarator_monstrator_qualificatores(
                        declarator)) != (i32)ZEPHYRUM)
                {
                    redde FALSUM;
                }
                stella = _valor_radix(
                    silva_c89_declarator_monstrator_tok_stella(
                        declarator));
                si (stella)
                {
                    *stellae += I;
                    si (   *stella_prima == (i32)ZEPHYRUM
                        || stella->columna < *stella_prima)
                    {
                        *stella_prima = stella->columna;
                    }
                }
                declarator = _valor_nodus(
                    silva_c89_declarator_monstrator_internum(
                        declarator));
                frange;
            }
            casus SILVA_C89_GENUS_DECLARATOR_ACIEI:
                declarator = _valor_nodus(
                    silva_c89_declarator_aciei_internum(
                        declarator));
                frange;
            casus SILVA_C89_GENUS_DECLARATOR_TITULUS:
                *titulus = _valor_radix(
                    silva_c89_declarator_titulus_tok_titulus(
                        declarator));
                redde *titulus != NIHIL;
            ordinarius:
                redde FALSUM;
        }
    }
    redde FALSUM;
}

/* membrum ordinis colligere e specificatoribus + declaratore
 * uno; FALSUM = exemptum */
interior b32
_membrum_colligere (
    FormatorAmbitus* ambitus,
         SilvaValor  specificatores,
         SilvaNodus* declarator,
          R7Membrum* exitus)
{
           i32  la;
           i32  ca;
           i32  lb;
           i32  cb;
    SilvaToken* titulus;

    si (!_valoris_extensio(ambitus, specificatores, &la, &ca,
        &lb, &cb))
    {
        redde FALSUM;
    }
    si (la != lb) redde FALSUM;

    /* specificator bloccalis in situ (structura/unio/enumeratio)
     * - cb post '}' geometriae columnarum absonus: exemptum
     * (venatio js_lexema: unio uni-linearis localem 'chorda c'
     * in aeternum fugabat) */
    {
        i32 n_spec;
        i32 s;

        n_spec = silva_valor_lista_numerus(specificatores);
        per (s = ZEPHYRUM; s < n_spec; s += I)
        {
            SilvaValor* e;
            SilvaNodus* n_s;

            e    = silva_valor_lista_obtinere(specificatores, s);
            n_s  = e ? _valor_nodus(*e) : NIHIL;
            si (   n_s && (n_s->genus == SILVA_C89_GENUS_STRUCTURA
                || n_s->genus == SILVA_C89_GENUS_UNIO
                || n_s->genus == SILVA_C89_GENUS_ENUMERATIO))
            {
                redde FALSUM;
            }
        }
    }

    si (!_declaratorem_metiri(declarator, &exitus->stellae,
        &exitus->stella_prima, &titulus))
    {
        redde FALSUM;
    }
    si (titulus->linea != la) redde FALSUM;
    /* sanitas: typus ANTE titulum desinere debet - extensio
     * macro-mendax (radix expansi) titulum transgreditur et
     * columna titulorum se ipsam in aeternum fugat (venatio
     * flatura 1811: 245 -> 267 -> 289...) */
    si (titulus->columna < cb) redde FALSUM;
    exitus->linea            = la;
    exitus->ca               = ca;
    exitus->cb               = cb;
    exitus->titulus_columna  = titulus->columna;
    redde VERUM;
}

interior vacuum
_ordinem_censere (
    FormatorAmbitus* ambitus,
          R7Membrum* membra,
                i32  numerus)
{
    constans character* ancora_textus;
                   i32  ancora_mensura;
                   i32  ancora_linea;
             character  cauda_tituli[48];
                   i32  cb_maxima;
                   i32  stellae_maximae;
                   i32  hiatus;
                   i32  i;

    si (numerus == (i32)ZEPHYRUM) redde;

    cb_maxima        = ZEPHYRUM;
    stellae_maximae  = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        si (membra[i].cb > cb_maxima)
        {
            cb_maxima = membra[i].cb;
        }
        si (membra[i].stellae > stellae_maximae)
        {
            stellae_maximae = membra[i].stellae;
        }
    }
    hiatus = I + stellae_maximae;

    /* ancora nuntiorum: membrum typo latissimo (primum) */
    ancora_textus   = "";
    ancora_mensura  = ZEPHYRUM;
    ancora_linea    = membra[ZEPHYRUM].linea;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        si (membra[i].cb != cb_maxima) perge;
        ancora_linea   = membra[i].linea;
        ancora_mensura = _textus_lineae(ambitus, membra[i].linea,
            membra[i].ca, membra[i].cb, &ancora_textus);
        frange;
    }
    sprintf(cauda_tituli, " + I + stellae %u",
        (insignatus integer)stellae_maximae);

    /* custodia LXXII GLOMERIS TOTIUS (ut R9): ordinatio lineam
     * ullam (TOTAM metitam - commentarium caudae inclusum) ultra
     * LXXII truderet? Glomus non ordinatur; divergentia LINT sola
     * in membro latissimo causam nominat. Praedicatum a spatiis
     * praesentibus independens (tractio = columna recta - columna
     * praesens; linea tota crescit tractione) - non oscillat. */
    {
        b32 cadit;
        i32 latissimum;
        i32 longitudo_latissima;

        cadit                = FALSUM;
        latissimum           = ZEPHYRUM;
        longitudo_latissima  = ZEPHYRUM;
        per (i = ZEPHYRUM; i < numerus; i += I)
        {
            s32 tractio;
            i32 longitudo;

            tractio = (s32)(cb_maxima + hiatus)
                - (s32)membra[i].titulus_columna;
            si (tractio <= (s32)ZEPHYRUM) perge;
            longitudo = _longitudo_lineae(ambitus, membra[i].linea)
                + (i32)tractio;
            si (   longitudo > (i32)LONGITUDO_RECTA
                && longitudo > longitudo_latissima)
            {
                cadit                = VERUM;
                latissimum           = i;
                longitudo_latissima  = longitudo;
            }
        }
        si (cadit)
        {
            _addere(ambitus->divergentiae, "columnae-binae",
                "glomus non ordinatum: ordinatio LXXII"
                " excederet", membra[latissimum].linea,
                membra[latissimum].titulus_columna,
                (s32)longitudo_latissima, LONGITUDO_RECTA);
            redde;
        }
    }

    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        b32 stella_recta;

        si (membra[i].cb != cb_maxima)
        {
            /* typi dextre ordinati: totum membrum dextrorsum
             * trudere (cb < maxima semper - maxima est) */
            _addere(ambitus->divergentiae, "columnae-binae",
                _nuntius_ancorae(ambitus->piscina,
                    "columna typorum dextra non ordinata",
                    ancora_textus, ancora_mensura, ancora_linea,
                    ": typus latissimus"),
                membra[i].linea, membra[i].cb - I,
                (s32)(membra[i].cb - I), (s32)(cb_maxima - I));
            si (membra[i].cb < cb_maxima)
            {
                _emendare(ambitus->divergentiae,
                    membra[i].linea, membra[i].ca,
                    membra[i].linea, membra[i].ca,
                    _textus_emendationis(ambitus->piscina,
                        ZEPHYRUM, cb_maxima - membra[i].cb));
            }
            perge;
        }
        stella_recta = membra[i].stellae == (i32)ZEPHYRUM
            || membra[i].stella_prima == cb_maxima;
        si (!stella_recta)
        {
            _addere(ambitus->divergentiae, "columnae-binae",
                _nuntius_ancorae(ambitus->piscina,
                    "stella in hiatu post typum exspectata",
                    ancora_textus, ancora_mensura, ancora_linea,
                    ": stella ad oram typi latissimi"),
                membra[i].linea, membra[i].stella_prima,
                (s32)membra[i].stella_prima, (s32)cb_maxima);
            si (membra[i].stella_prima > cb_maxima)
            {
                /* tractio tolerans (commentum obsistere potest) */
                _emendare_tolerans(ambitus->divergentiae,
                    membra[i].linea, cb_maxima,
                    membra[i].linea, membra[i].stella_prima,
                    _textus_emendationis(ambitus->piscina,
                        ZEPHYRUM, ZEPHYRUM));
            }
        }
        si (membra[i].titulus_columna != cb_maxima + hiatus)
        {
            _addere(ambitus->divergentiae, "columnae-binae",
                _nuntius_ancorae(ambitus->piscina,
                    "columna titulorum non ordinata",
                    ancora_textus, ancora_mensura, ancora_linea,
                    cauda_tituli),
                membra[i].linea, membra[i].titulus_columna,
                (s32)membra[i].titulus_columna,
                (s32)(cb_maxima + hiatus));
            /* solum stella recta: aliter spatium stellam
             * contineret - iteratio sequens post stellam
             * motam id sanat */
            si (stella_recta)
            {
                si (membra[i].titulus_columna
                    < cb_maxima + hiatus)
                {
                    _emendare(ambitus->divergentiae,
                        membra[i].linea,
                        membra[i].titulus_columna,
                        membra[i].linea,
                        membra[i].titulus_columna,
                        _textus_emendationis(ambitus->piscina,
                            ZEPHYRUM, cb_maxima + hiatus
                                - membra[i].titulus_columna));
                }
                alioquin
                {
                    /* tractio tolerans */
                    _emendare_tolerans(ambitus->divergentiae,
                        membra[i].linea, cb_maxima + hiatus,
                        membra[i].linea,
                        membra[i].titulus_columna,
                        _textus_emendationis(ambitus->piscina,
                            ZEPHYRUM, ZEPHYRUM));
                }
            }
        }
    }
}

/* ordines membrorum structurae/unionis (linea vacua findit) */
interior vacuum
_membra_censere (
    FormatorAmbitus* ambitus,
         SilvaValor  membra_lista)
{
    R7Membrum membra[R7_MEMBRA_MAXIMA];
          i32 plena;
          i32 linea_prior;
          i32 numerus;
          i32 i;

    plena        = ZEPHYRUM;
    linea_prior  = ZEPHYRUM;
    numerus      = silva_valor_lista_numerus(membra_lista);

    per (i = ZEPHYRUM; i < numerus; i += I)
    {
         SilvaValor* elementum;
         SilvaNodus* membrum;
         SilvaValor  declaratores;
         SilvaNodus* declarator;
          R7Membrum  novum;
                b32  sanum;

        elementum = silva_valor_lista_obtinere(membra_lista,
            i);
        membrum  = elementum ? _valor_nodus(*elementum) : NIHIL;
        sanum    = FALSUM;

        si (   membrum
            && membrum->genus == SILVA_C89_GENUS_MEMBRUM)
        {
            declaratores = silva_c89_membrum_declaratores(
                membrum);
            si (silva_valor_lista_numerus(declaratores)
                == (i32)I)
            {
                SilvaValor* d;

                d = silva_valor_lista_obtinere(declaratores,
                    ZEPHYRUM);
                declarator = d ? _valor_nodus(*d) : NIHIL;
                sanum = _membrum_colligere(ambitus,
                    silva_c89_membrum_specificatores(membrum),
                    declarator, &novum);
            }
        }

        si (!sanum)
        {
            _ordinem_censere(ambitus, membra, plena);
            plena        = ZEPHYRUM;
            linea_prior  = ZEPHYRUM;
            perge;
        }
        /* membrum alterum EADEM linea: R7 regula trans lineas
         * est - ordo finditur, membrum non participat */
        si (   linea_prior != (i32)ZEPHYRUM
            && novum.linea == linea_prior)
        {
            _ordinem_censere(ambitus, membra, plena);
            plena        = ZEPHYRUM;
            linea_prior  = ZEPHYRUM;
            perge;
        }
        si (   (linea_prior != (i32)ZEPHYRUM
                && novum.linea > linea_prior + I)
            || plena >= (i32)R7_MEMBRA_MAXIMA)
        {
            _ordinem_censere(ambitus, membra, plena);
            plena = ZEPHYRUM;
        }
        membra[plena]  = novum;
        plena          += I;
        linea_prior    = novum.linea;
    }
    _ordinem_censere(ambitus, membra, plena);
}

/* R9: glomus assignationum - operatores ordinati ad max(finis
 * sinistri) + II ("xyz  = valor"); sinistris AEQUILATIS ad
 * max + I (spatium duplex nihil signat ubi nulla suffarcinatio
 * - decretum Frani 2026-08-19). Glomera singula (n < II) tacent.
 * Custodia LXXII est GLOMERIS TOTIUS: si ordinatio membrum
 * ullum transgrederetur, glomus totum ad minimum cadit (cb + I
 * quodque) - uniformitas invarians est, exceptio per membrum
 * eam scindebat (raggedness nuntii quam Fran invenit).
 * Praedicatum a spatiis praesentibus independens - non
 * oscillat. */
interior vacuum
_aequationes_censere (
    FormatorAmbitus* ambitus,
      constans  i32* cb,
      constans  i32* operator_columnae,
      constans  i32* lineae,
      constans  i32* fines,
                i32  numerus)
{
    constans character* ancora_textus;
    constans character* cauda;
                   i32  ancora_mensura;
                   i32  ancora_linea;
                   i32  cb_maxima;
                   i32  columna_recta;
                   b32  aequilata;
                   b32  cadit;
                   i32  i;

    si (numerus < (i32)II) redde;

    cb_maxima = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        si (cb[i] > cb_maxima) cb_maxima = cb[i];
    }
    aequilata = VERUM;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        si (cb[i] != cb_maxima)
        {
            aequilata = FALSUM;
            frange;
        }
    }
    columna_recta = cb_maxima + (aequilata ? I : II);

    cadit = FALSUM;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        si ((fines[i] - operator_columnae[i]) + columna_recta
            - I > (i32)LONGITUDO_RECTA)
        {
            cadit = VERUM;
            frange;
        }
    }

    /* ancora nuntiorum: sinistrum longissimum (primum) et regula */
    ancora_textus   = "";
    ancora_mensura  = ZEPHYRUM;
    ancora_linea    = lineae[ZEPHYRUM];
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        si (cb[i] != cb_maxima) perge;
        ancora_linea   = lineae[i];
        ancora_mensura = _textus_lineae_sine_indentatione(ambitus,
            lineae[i], cb[i], &ancora_textus);
        frange;
    }
    cauda = cadit ? ", minimum (LXXII)"
        : (aequilata ? " + I (aequilata)" : " + II");

    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        i32 exspectata;

        exspectata = cadit ? cb[i] + I : columna_recta;
        si (operator_columnae[i] == exspectata) perge;
        _addere(ambitus->divergentiae,
            "aequatio-assignationum",
            _nuntius_ancorae(ambitus->piscina,
                "operator '=' glomeris non ordinatus",
                ancora_textus, ancora_mensura, ancora_linea, cauda),
            lineae[i], operator_columnae[i],
            (s32)operator_columnae[i],
            (s32)exspectata);
        si (operator_columnae[i] < exspectata)
        {
            _emendare(ambitus->divergentiae, lineae[i],
                operator_columnae[i], lineae[i],
                operator_columnae[i],
                _textus_emendationis(ambitus->piscina,
                    ZEPHYRUM,
                    exspectata - operator_columnae[i]));
        }
        alioquin
        {
            /* tractio tolerans: cb ex extensione arboris venit
             * quae MENTIRI potest (sinister macro-expansus -
             * radix ad invocationem collabitur, coloratio 999)
             * aut commentum inter sinistrum et operatorem sedet
             * - obstructa tacite dilatatur */
            _emendare_tolerans(ambitus->divergentiae, lineae[i],
                exspectata, lineae[i],
                operator_columnae[i],
                _textus_emendationis(ambitus->piscina,
                    ZEPHYRUM, ZEPHYRUM));
        }
    }
}

/* glomeri R9 alere: apta appenditur (fractura contiguitatis
 * flusa), non-apta glomus findit et censet */
interior vacuum
_aeq_pascere (
    FormatorAmbitus* ambitus,
                i32* aeq_cb,
                i32* aeq_op,
                i32* aeq_lineae,
                i32* aeq_fines,
                i32* aeq_plena,
                i32* aeq_linea_prior,
                b32  apta,
                i32  cb,
                i32  op_columna,
                i32  la,
                i32  finis)
{
    si (apta)
    {
        /* assignatio altera EADEM linea: non participat */
        si (   *aeq_linea_prior != (i32)ZEPHYRUM
            && la               == *aeq_linea_prior)
        {
            _aequationes_censere(ambitus, aeq_cb, aeq_op,
                aeq_lineae, aeq_fines, *aeq_plena);
            *aeq_plena        = ZEPHYRUM;
            *aeq_linea_prior  = ZEPHYRUM;
            redde;
        }
        si (   (*aeq_linea_prior != (i32)ZEPHYRUM
                && la > *aeq_linea_prior + I)
            || *aeq_plena >= (i32)R7_MEMBRA_MAXIMA)
        {
            _aequationes_censere(ambitus, aeq_cb, aeq_op,
                aeq_lineae, aeq_fines, *aeq_plena);
            *aeq_plena = ZEPHYRUM;
        }
        aeq_cb[*aeq_plena]      = cb;
        aeq_op[*aeq_plena]      = op_columna;
        aeq_lineae[*aeq_plena]  = la;
        aeq_fines[*aeq_plena]   = finis;
        *aeq_plena              += I;
        *aeq_linea_prior        = la;
    }
    alioquin
    {
        _aequationes_censere(ambitus, aeq_cb, aeq_op,
            aeq_lineae, aeq_fines, *aeq_plena);
        *aeq_plena        = ZEPHYRUM;
        *aeq_linea_prior  = ZEPHYRUM;
    }
}

/* declaratio initiata glomeri R9 alenda? (porta G1 clausa -
 * Fran 'nuntium 770' invenit: '=' initiatorum a nulla regula
 * regebatur). cb = una post declaratorem (suffixa aciei
 * participant, ut R7). */
interior b32
_declarationem_aequatione_metiri (
    FormatorAmbitus* ambitus,
         SilvaNodus* nodus,
         SilvaNodus* declarator,
                i32* cb,
                i32* op_columna,
                i32* la,
                i32* finis)
{
    SilvaToken* op_tok;
           i32  xla;
           i32  xca;
           i32  xlb;
           i32  xcb;
           i32  sla;
           i32  sca;
           i32  slb;
           i32  scb;

    si (   !declarator || declarator->genus
        != SILVA_C89_GENUS_DECLARATOR_INITIATUS)
    {
        redde FALSUM;
    }
    op_tok = _token_fons(
        silva_c89_declarator_initiatus_tok_operator(
            declarator));
    si (!_principalis(ambitus, op_tok)) redde FALSUM;
    si (   !_valoris_extensio(ambitus,
        silva_c89_declarator_initiatus_declarator(declarator),
        &xla, &xca, &xlb, &xcb)
        || xla != xlb || op_tok->linea != xla)
    {
        redde FALSUM;
    }
    si (   !_extensio(nodus, ambitus->fons_princeps, &sla, &sca,
        &slb, &scb)
        || sla != slb)
    {
        redde FALSUM;
    }
    *cb          = xcb;
    *op_columna  = op_tok->columna;
    *la          = xla;
    *finis       = scb;
    redde VERUM;
}

/* ordines localium in capite corporis + glomera assignationum
 * (R9: '=' per glomus ordinata; linea vacua findit; declarationes
 * initiatae participant) */
interior vacuum
_corpus_interius_censere (
    FormatorAmbitus* ambitus,
         SilvaNodus* corpus)
{
     SilvaValor elementa;
      R7Membrum membra[R7_MEMBRA_MAXIMA];
            i32 plena;
            i32 linea_prior;
            i32 numerus;
            i32 i;
            b32 in_declarationibus;
            i32 aeq_cb[R7_MEMBRA_MAXIMA];
            i32 aeq_op[R7_MEMBRA_MAXIMA];
            i32 aeq_lineae[R7_MEMBRA_MAXIMA];
            i32 aeq_fines[R7_MEMBRA_MAXIMA];
            i32 aeq_plena;
            i32 aeq_linea_prior;

    elementa  = silva_c89_corpus_elementa(corpus);
    numerus   = silva_valor_lista_numerus(elementa);

    plena               = ZEPHYRUM;
    linea_prior         = ZEPHYRUM;
    in_declarationibus  = VERUM;
    aeq_plena           = ZEPHYRUM;
    aeq_linea_prior     = ZEPHYRUM;

    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        SilvaValor* elementum;
        SilvaNodus* nodus;

        elementum  = silva_valor_lista_obtinere(elementa, i);
        nodus      = elementum ? _valor_nodus(*elementum) : NIHIL;

        /* R7: cursus declarationum ducens */
        si (in_declarationibus)
        {
                  b32 sanum;
            R7Membrum novum;

            sanum = FALSUM;
            si (   nodus
                && nodus->genus == SILVA_C89_GENUS_DECLARATIO)
            {
                SilvaValor  declaratores;
                SilvaNodus* declarator;

                declarator   = NIHIL;
                declaratores = silva_c89_declaratio_declaratores(
                    nodus);
                si (silva_valor_lista_numerus(declaratores)
                    == (i32)I)
                {
                    SilvaValor* d;

                    d = silva_valor_lista_obtinere(
                        declaratores, ZEPHYRUM);
                    declarator = d ? _valor_nodus(*d) : NIHIL;
                    sanum = _membrum_colligere(ambitus,
                        silva_c89_declaratio_specificatores(
                            nodus), declarator, &novum);
                }

                /* R9: initiata glomeri alitur (etiam R7-exempta;
                 * non-initiata glomus findit) */
                {
                    b32 aeq_apta;
                    i32 dcb;
                    i32 op_col;
                    i32 dla;
                    i32 finis;

                    dcb     = ZEPHYRUM;
                    op_col  = ZEPHYRUM;
                    dla     = ZEPHYRUM;
                    finis   = ZEPHYRUM;
                    aeq_apta = _declarationem_aequatione_metiri(
                        ambitus, nodus, declarator, &dcb,
                        &op_col, &dla, &finis);
                    _aeq_pascere(ambitus, aeq_cb, aeq_op,
                        aeq_lineae, aeq_fines, &aeq_plena,
                        &aeq_linea_prior, aeq_apta, dcb,
                        op_col, dla, finis);
                }

                si (!sanum)
                {
                    /* declaratio exempta - ordinem findit */
                    _ordinem_censere(ambitus, membra, plena);
                    plena        = ZEPHYRUM;
                    linea_prior  = ZEPHYRUM;
                    perge;
                }
                si (   linea_prior != (i32)ZEPHYRUM
                    && novum.linea == linea_prior)
                {
                    /* eadem linea - non participat */
                    _ordinem_censere(ambitus, membra, plena);
                    plena        = ZEPHYRUM;
                    linea_prior  = ZEPHYRUM;
                    perge;
                }
                si (   (linea_prior != (i32)ZEPHYRUM
                        && novum.linea > linea_prior + I)
                    || plena >= (i32)R7_MEMBRA_MAXIMA)
                {
                    _ordinem_censere(ambitus, membra, plena);
                    plena = ZEPHYRUM;
                }
                membra[plena]  = novum;
                plena          += I;
                linea_prior    = novum.linea;
                perge;
            }
            _ordinem_censere(ambitus, membra, plena);
            plena               = ZEPHYRUM;
            in_declarationibus  = FALSUM;
        }

        /* R9: glomera assignationum simplicium */
        {
            SilvaNodus* assignatio;
            SilvaToken* operator_tok;
                   i32  la;
                   i32  ca;
                   i32  lb;
                   i32  cb;
                   i32  sla;
                   i32  sca;
                   i32  slb;
                   i32  scb;
                   b32  apta;

            apta          = FALSUM;
            assignatio    = NIHIL;
            operator_tok  = NIHIL;
            scb           = ZEPHYRUM;
            si (   nodus && nodus->genus
                == SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS)
            {
                assignatio = _valor_nodus(
                    silva_c89_sententia_expressionis_expressio(
                        nodus));
            }
            si (   assignatio && assignatio->genus
                == SILVA_C89_GENUS_ASSIGNATIO)
            {
                operator_tok = _token_fons(
                    silva_c89_assignatio_tok_operator(
                        assignatio));
                si (   operator_tok && _valoris_extensio(ambitus,
                    silva_c89_assignatio_sinister(assignatio),
                    &la, &ca, &lb, &cb)
                    && la                  == lb
                    && operator_tok->linea == la
                    && _extensio(nodus, ambitus->fons_princeps,
                        &sla, &sca, &slb, &scb)
                    && sla                 == slb)
                {
                    apta = VERUM;
                }
            }

            _aeq_pascere(ambitus, aeq_cb, aeq_op, aeq_lineae,
                aeq_fines, &aeq_plena, &aeq_linea_prior, apta,
                apta ? cb : (i32)ZEPHYRUM,
                apta ? operator_tok->columna : (i32)ZEPHYRUM,
                apta ? la : (i32)ZEPHYRUM,
                apta ? scb : (i32)ZEPHYRUM);
        }
    }
    _ordinem_censere(ambitus, membra, plena);
    _aequationes_censere(ambitus, aeq_cb, aeq_op, aeq_lineae,
        aeq_fines, aeq_plena);
}

/* R7 super parametra (forma multi-linearis sola - forma
 * uni-linearis iam R8 flagravit); nucleus communis definitionibus
 * et prototypis */
interior vacuum
_parametra_ordinem_functionis (
    FormatorAmbitus* ambitus,
         SilvaNodus* functionis)
{
     SilvaToken* apertum;
     SilvaValor  parametra;
      R7Membrum  membra[R7_MEMBRA_MAXIMA];
            i32  numerus;
            i32  plena;
            i32  i;

    si (!functionis) redde;
    apertum = _valor_radix(
        silva_c89_declarator_functionis_tok_apertum(functionis));
    si (!_principalis(ambitus, apertum)) redde;
    parametra = silva_c89_declarator_functionis_parametra(
        functionis);
    numerus = silva_valor_lista_numerus(parametra);
    si (   numerus == (i32)ZEPHYRUM
        || numerus > (i32)R7_MEMBRA_MAXIMA)
    {
        redde;
    }

    plena = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
         SilvaValor* elementum;
         SilvaNodus* parametrum;
         SilvaNodus* declarator_parametri;
         SilvaValor  d;

        elementum = silva_valor_lista_obtinere(parametra, i);
        parametrum = elementum ? _valor_nodus(*elementum)
            : NIHIL;
        /* lista virgulas ut lexemata fert - omittere (VITIUM
         * G1 latens: bail hic omnem functionem multi-parametri
         * R7 caecam fecit; R8 semper recte omisit) */
        si (!parametrum) perge;
        si (parametrum->genus != SILVA_C89_GENUS_PARAMETRUM)
        {
            redde;
        }
        d = silva_c89_parametrum_declarator(parametrum);
        declarator_parametri = _valor_nodus(d);
        si (!_membrum_colligere(ambitus,
            silva_c89_parametrum_specificatores(parametrum),
            declarator_parametri, &membra[plena]))
        {
            redde;
        }
        /* parametra duo eadem linea: R8 primum findat */
        si (   plena               != (i32)ZEPHYRUM
            && membra[plena].linea == membra[plena - I].linea)
        {
            redde;
        }
        plena += I;
    }
    /* forma uni-linearis (parametrum in linea parenthesis)
     * R8 pertinet - hic tacemus */
    si (   apertum && plena != (i32)ZEPHYRUM
        && membra[ZEPHYRUM].linea == apertum->linea)
    {
        redde;
    }
    _ordinem_censere(ambitus, membra, plena);
}


/* ==================================================
 * R17 catena-logica: catena &&/|| multi-linearis in
 * conditione si/dum/per. Operandum primum ad
 * parenthesim + IV (spatia III post '('), operatores
 * ducentes ad parenthesim + I, comparationes
 * bi-characterum ordine R9-simili ad max(cb) + I
 * (decretum Frani 2026-08-19; nota: + I hic, R9 + II).
 * ================================================== */

#define CATENA_MEMBRA_MAXIMA 64

interior b32
_operator_logicus (
    SilvaLexemaGenus genus)
{
    redde genus == SILVA_LEX_ET_ET
        || genus == SILVA_LEX_VEL_VEL;
}

interior b32
_comparatio_bichar (
    SilvaLexemaGenus genus)
{
    redde genus == SILVA_LEX_AEQUALIS_AEQUALIS
        || genus == SILVA_LEX_NON_AEQUALIS
        || genus == SILVA_LEX_MINOR_AEQUALIS
        || genus == SILVA_LEX_MAIOR_AEQUALIS;
}

nomen structura {
    SilvaToken* ops[CATENA_MEMBRA_MAXIMA];   /* logici */
           i32  numerus_operatorum;
    SilvaNodus* rami[CATENA_MEMBRA_MAXIMA];
           i32  numerus_ramorum;
} CatenaCollectio;

/* spinam logicam colligere: operatores &&/|| et ramos (folia
 * non-logica) ordine fontis; parentheses interiores NON
 * aperiuntur (catenae nidificatae ancoram propriam habent) */
interior vacuum
_catenam_colligere (
         SilvaNodus* nodus,
    CatenaCollectio* collectio)
{
    si (!nodus) redde;
    si (nodus->genus == SILVA_C89_GENUS_BINARIUM)
    {
        SilvaToken* op;

        op = _token_fons(
            silva_c89_binarium_tok_operator(nodus));
        si (op && _operator_logicus(op->genus))
        {
            _catenam_colligere(_valor_nodus(
                silva_c89_binarium_sinister(nodus)), collectio);
            si (collectio->numerus_operatorum
                < (i32)CATENA_MEMBRA_MAXIMA)
            {
                collectio->ops[collectio->numerus_operatorum]
                    = op;
                collectio->numerus_operatorum += I;
            }
            _catenam_colligere(_valor_nodus(
                silva_c89_binarium_dexter(nodus)), collectio);
            redde;
        }
    }
    si (collectio->numerus_ramorum
        < (i32)CATENA_MEMBRA_MAXIMA)
    {
        collectio->rami[collectio->numerus_ramorum]  = nodus;
        collectio->numerus_ramorum                   += I;
    }
}

interior vacuum
_catenam_censere (
    FormatorAmbitus* ambitus,
         SilvaValor  valor_apertum,
         SilvaValor  valor_clausum,
         SilvaNodus* conditio)
{
         SilvaToken* aperta;
         SilvaToken* clausa;
         SilvaToken* radix_op;
    CatenaCollectio  collectio;
                i32  columna_operandi;
                i32  columna_operatoris;
                i32  cb_maxima;
                i32  i;
                i32  la;
                i32  ca;
                i32  lb;
                i32  cb;

    aperta = _valor_radix(valor_apertum);
    clausa = _valor_radix(valor_clausum);
    si (   !_principalis(ambitus, aperta)
        || !_principalis(ambitus, clausa))
    {
        redde;
    }
    si (clausa->linea <= aperta->linea) redde;
    si (   !conditio
        || conditio->genus != SILVA_C89_GENUS_BINARIUM)
    {
        redde;
    }
    radix_op = _token_fons(
        silva_c89_binarium_tok_operator(conditio));
    si (!radix_op || !_operator_logicus(radix_op->genus))
    {
        redde;
    }

    collectio.numerus_operatorum  = ZEPHYRUM;
    collectio.numerus_ramorum     = ZEPHYRUM;
    _catenam_colligere(conditio, &collectio);

    columna_operandi    = aperta->columna + IV;
    columna_operatoris  = aperta->columna + I;

    /* A: operandum primum ad parenthesim + IV. Custodia LXXII:
     * spatia tria lineam totam (metitam) ultra LXXII non trudunt
     * - fractura R16 eam prius scindat, A iteratione sequenti. */
    si (   _extensio(conditio, ambitus->fons_princeps, &la, &ca,
        &lb, &cb)
        && la == aperta->linea
        && ca != columna_operandi
        && !(columna_operandi > ca
            && _longitudo_lineae(ambitus, la)
                + (columna_operandi - ca) > (i32)LONGITUDO_RECTA))
    {
        _addere(ambitus->divergentiae, "catena-logica",
            "spatia tria post parenthesim catenae exspectata",
            la, ca, (s32)ca, (s32)columna_operandi);
        _emendare_tolerans(ambitus->divergentiae, la,
            aperta->columna + I, la, ca,
            _textus_emendationis(ambitus->piscina, ZEPHYRUM,
                III));
    }

    /* B: operatores ducentes ad parenthesim + I */
    per (i = ZEPHYRUM; i < collectio.numerus_operatorum;
        i += I)
    {
        SilvaToken* op;

        op = collectio.ops[i];
        si (   !_principalis(ambitus, op)
            || !op->initium_lineae)
        {
            perge;
        }
        si (op->columna != columna_operatoris)
        {
            _addere(ambitus->divergentiae, "catena-logica",
                "operator catenae ad parenthesim + I"
                " exspectatus", op->linea, op->columna,
                (s32)op->columna, (s32)columna_operatoris);
            _emendare(ambitus->divergentiae, op->linea, I,
                op->linea, op->columna,
                _textus_emendationis(ambitus->piscina,
                    ZEPHYRUM, columna_operatoris - I));
        }
    }

    /* C: comparationes bi-characterum ordinatae ad
     * max(cb sinistrorum) + I; participes = rami
     * uni-lineares columnam operandi incipientes. Sedes
     * omnium participum vindicantur (R10-ante cedit).
     * Custodia LXXII GLOMERIS TOTIUS (ut R9): si linea ulla
     * (TOTA metita - rami plures in una linea, cauda) trans-
     * grederetur, omnes ad cb suum + I cadunt - uniformitas,
     * non exceptio per membrum (quae CV octetos in legatus.c
     * peperit: extensio rami lineam non videbat). */
    {
            SilvaToken* part_op[CATENA_MEMBRA_MAXIMA];
                   i32  part_scb[CATENA_MEMBRA_MAXIMA];
                   i32  part_sca[CATENA_MEMBRA_MAXIMA];
    constans character* ancora_textus;
    constans character* cauda;
                   i32  ancora_mensura;
                   i32  ancora_linea;
                   i32  part_n;
                   b32  cadit;

        part_n     = ZEPHYRUM;
        cb_maxima  = ZEPHYRUM;
        per (i = ZEPHYRUM; i < collectio.numerus_ramorum; i += I)
        {
            SilvaNodus* ramus;
            SilvaToken* op;
                   i32  rla;
                   i32  rca;
                   i32  rlb;
                   i32  rcb;
                   i32  sla;
                   i32  sca;
                   i32  slb;
                   i32  scb;

            ramus = collectio.rami[i];
            si (!ramus || ramus->genus != SILVA_C89_GENUS_BINARIUM)
            {
                perge;
            }
            op = _token_fons(
                silva_c89_binarium_tok_operator(ramus));
            si (   !op || !_comparatio_bichar(op->genus)
                || !_principalis(ambitus, op))
            {
                perge;
            }
            si (   !_extensio(ramus, ambitus->fons_princeps, &rla,
                &rca, &rlb, &rcb)
                || rla != rlb || rca != columna_operandi)
            {
                perge;
            }
            si (   !_valoris_extensio(ambitus,
                silva_c89_binarium_sinister(ramus), &sla, &sca,
                &slb, &scb)
                || sla != slb || op->linea != sla)
            {
                perge;
            }
            si (part_n >= (i32)CATENA_MEMBRA_MAXIMA) frange;
            part_op[part_n]   = op;
            part_scb[part_n]  = scb;
            part_sca[part_n]  = sca;
            part_n            += I;
            si (scb > cb_maxima) cb_maxima = scb;
        }
        si (part_n == (i32)ZEPHYRUM) redde;

        cadit = FALSUM;
        per (i = ZEPHYRUM; i < part_n; i += I)
        {
            s32 tractio;

            tractio = (s32)(cb_maxima + I)
                - (s32)part_op[i]->columna;
            si (   tractio > (s32)ZEPHYRUM
                && _longitudo_lineae(ambitus, part_op[i]->linea)
                    + (i32)tractio > (i32)LONGITUDO_RECTA)
            {
                cadit = VERUM;
                frange;
            }
        }

        /* ancora nuntiorum: sinistrum longissimum (primum) et regula */
        ancora_textus   = "";
        ancora_mensura  = ZEPHYRUM;
        ancora_linea    = part_op[ZEPHYRUM]->linea;
        per (i = ZEPHYRUM; i < part_n; i += I)
        {
            si (part_scb[i] != cb_maxima) perge;
            ancora_linea   = part_op[i]->linea;
            ancora_mensura = _textus_lineae(ambitus,
                part_op[i]->linea, part_sca[i], part_scb[i],
                &ancora_textus);
            frange;
        }
        cauda = cadit ? ", minimum (LXXII)" : " + I";

        per (i = ZEPHYRUM; i < part_n; i += I)
        {
             SilvaToken* op;
                    i32  columna_recta;
            CatenaSedes* sedes;

            op = part_op[i];

            /* vindicare (etiam sine divergentia) */
            si (ambitus->catena_ops)
            {
                sedes = (CatenaSedes*)xar_addere(
                    ambitus->catena_ops);
                si (sedes)
                {
                    sedes->linea    = op->linea;
                    sedes->columna  = op->columna;
                }
            }

            columna_recta = cadit ? part_scb[i] + I
                : cb_maxima + I;
            si (op->columna == columna_recta) perge;
            _addere(ambitus->divergentiae, "catena-logica",
                _nuntius_ancorae(ambitus->piscina,
                    "comparatio catenae non ordinata",
                    ancora_textus, ancora_mensura, ancora_linea,
                    cauda),
                op->linea, op->columna, (s32)op->columna,
                (s32)columna_recta);
            si (op->columna < columna_recta)
            {
                _emendare(ambitus->divergentiae, op->linea,
                    op->columna, op->linea, op->columna,
                    _textus_emendationis(ambitus->piscina,
                        ZEPHYRUM, columna_recta - op->columna));
            }
            alioquin
            {
                /* tractio tolerans */
                _emendare_tolerans(ambitus->divergentiae,
                    op->linea, columna_recta, op->linea,
                    op->columna,
                    _textus_emendationis(ambitus->piscina,
                        ZEPHYRUM, ZEPHYRUM));
            }
        }
    }
}

/* estne sedes operatoris a catena vindicata? */
interior b32
_catena_vindicata (
    FormatorAmbitus* ambitus,
                i32  linea,
                i32  columna)
{
    i32 numerus;
    i32 i;

    numerus = ambitus->catena_ops
        ? xar_numerus(ambitus->catena_ops) : (i32)ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        constans CatenaSedes* sedes;

        sedes = (constans CatenaSedes*)xar_obtinere(
            ambitus->catena_ops, i);
        si (sedes->linea == linea && sedes->columna == columna)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* definitio: caput + parametra uno vocamine */
interior vacuum
_definitionem_censere (
    FormatorAmbitus* ambitus,
         SilvaNodus* definitio)
{
    SilvaNodus* functionis;

    functionis = _declarator_functionis(_valor_nodus(
        silva_c89_definitio_functionis_declarator(definitio)));
    _functionis_caput_censere(ambitus, functionis);
    _parametra_ordinem_functionis(ambitus, functionis);
}


/* ==================================================
 * R10 operatores: binarii spatiati, accessus/conversio/
 * unarii/postcrementum arti. ASSIGNATIO exclusa (R9).
 * ================================================== */

interior vacuum
_binarium_operatorem_censere (
    FormatorAmbitus* ambitus,
         SilvaNodus* nodus)
{
    SilvaToken* operator_tok;
           i32  sla;
           i32  sca;
           i32  slb;
           i32  scb;
           i32  dla;
           i32  dca;
           i32  dlb;
           i32  dcb;

    operator_tok = _token_fons(
        silva_c89_binarium_tok_operator(nodus));
    si (!_principalis(ambitus, operator_tok)) redde;

    si (   _valoris_extensio(ambitus,
        silva_c89_binarium_sinister(nodus), &sla, &sca, &slb,
        &scb)
        && operator_tok->linea   == slb
        && operator_tok->columna != scb + I
        && !(_comparatio_bichar(operator_tok->genus)
            && _catena_vindicata(ambitus, operator_tok->linea,
                operator_tok->columna)))
    {
        _addere(ambitus->divergentiae, "operatores",
            "spatium unicum ante operatorem binarium",
            operator_tok->linea, operator_tok->columna,
            (s32)(operator_tok->columna - scb), I);
        _emendare_tolerans(ambitus->divergentiae,
            operator_tok->linea, scb, operator_tok->linea,
            operator_tok->columna,
            _textus_emendationis(ambitus->piscina, ZEPHYRUM,
                I));
    }
    si (   _valoris_extensio(ambitus,
        silva_c89_binarium_dexter(nodus), &dla, &dca, &dlb,
        &dcb)
        && dla == operator_tok->linea
        && dca != operator_tok->columna
            + operator_tok->valor.mensura + I)
    {
        _addere(ambitus->divergentiae, "operatores",
            "spatium unicum post operatorem binarium",
            operator_tok->linea, dca,
            (s32)(dca - (operator_tok->columna
                + operator_tok->valor.mensura)), I);
        _emendare_tolerans(ambitus->divergentiae,
            operator_tok->linea,
            operator_tok->columna
                + operator_tok->valor.mensura,
            operator_tok->linea, dca,
            _textus_emendationis(ambitus->piscina, ZEPHYRUM,
                I));
    }
}

interior vacuum
_accessum_censere (
    FormatorAmbitus* ambitus,
         SilvaNodus* nodus)
{
    SilvaToken* operator_tok;
    SilvaToken* titulus;
           i32  bla;
           i32  bca;
           i32  blb;
           i32  bcb;

    operator_tok = _token_fons(
        silva_c89_accessus_tok_operator(nodus));
    si (!_principalis(ambitus, operator_tok)) redde;

    si (   _valoris_extensio(ambitus,
        silva_c89_accessus_basis(nodus), &bla, &bca, &blb,
        &bcb)
        && operator_tok->linea   == blb
        && operator_tok->columna != bcb)
    {
        _addere(ambitus->divergentiae, "operatores",
            "accessus basi arte iungendus",
            operator_tok->linea, operator_tok->columna,
            (s32)(operator_tok->columna - bcb), ZEPHYRUM);
        _emendare_tolerans(ambitus->divergentiae,
            operator_tok->linea, bcb, operator_tok->linea,
            operator_tok->columna,
            _textus_emendationis(ambitus->piscina, ZEPHYRUM,
                ZEPHYRUM));
    }
    titulus = _valor_radix(
        silva_c89_accessus_tok_titulus(nodus));
    si (   _principalis(ambitus, titulus)
        && titulus->linea == operator_tok->linea
        && titulus->columna != operator_tok->columna
            + operator_tok->valor.mensura)
    {
        _addere(ambitus->divergentiae, "operatores",
            "titulus accessui arte iungendus",
            titulus->linea, titulus->columna,
            (s32)(titulus->columna
                - (operator_tok->columna
                    + operator_tok->valor.mensura)), ZEPHYRUM);
        _emendare_tolerans(ambitus->divergentiae,
            titulus->linea,
            operator_tok->columna
                + operator_tok->valor.mensura,
            titulus->linea, titulus->columna,
            _textus_emendationis(ambitus->piscina, ZEPHYRUM,
                ZEPHYRUM));
    }
}

interior vacuum
_conversionem_censere (
    FormatorAmbitus* ambitus,
         SilvaNodus* nodus)
{
    SilvaToken* clausum;
           i32  ila;
           i32  ica;
           i32  ilb;
           i32  icb;

    clausum = _token_fons(
        silva_c89_conversio_tok_clausum(nodus));
    si (!_principalis(ambitus, clausum)) redde;
    si (   _valoris_extensio(ambitus,
        silva_c89_conversio_internum(nodus), &ila, &ica, &ilb,
        &icb)
        && ila == clausum->linea
        && ica != clausum->columna + I)
    {
        _addere(ambitus->divergentiae, "operatores",
            "conversio operando arte iungenda",
            clausum->linea, ica,
            (s32)(ica - clausum->columna - I), ZEPHYRUM);
        _emendare_tolerans(ambitus->divergentiae,
            clausum->linea, clausum->columna + I,
            clausum->linea, ica,
            _textus_emendationis(ambitus->piscina, ZEPHYRUM,
                ZEPHYRUM));
    }
}

interior vacuum
_unarium_censere (
    FormatorAmbitus* ambitus,
         SilvaNodus* nodus)
{
    SilvaToken* operator_tok;
           i32  ila;
           i32  ica;
           i32  ilb;
           i32  icb;

    operator_tok = _token_fons(
        silva_c89_unarium_tok_operator(nodus));
    si (!_principalis(ambitus, operator_tok)) redde;
    si (   _valoris_extensio(ambitus,
        silva_c89_unarium_internum(nodus), &ila, &ica, &ilb,
        &icb)
        && ila == operator_tok->linea
        && ica != operator_tok->columna
            + operator_tok->valor.mensura)
    {
        _addere(ambitus->divergentiae, "operatores",
            "operator unarius operando arte iungendus",
            operator_tok->linea, ica,
            (s32)(ica - (operator_tok->columna
                + operator_tok->valor.mensura)), ZEPHYRUM);
        _emendare_tolerans(ambitus->divergentiae,
            operator_tok->linea,
            operator_tok->columna
                + operator_tok->valor.mensura,
            operator_tok->linea, ica,
            _textus_emendationis(ambitus->piscina, ZEPHYRUM,
                ZEPHYRUM));
    }
}

interior vacuum
_nodum_percurrere (
    FormatorAmbitus* ambitus,
         SilvaNodus* nodus);

/* catena si/alioquin-si: caput = si primum (columna brachiorum
 * catenae totius e capite mensuratur) */
interior vacuum
_si_censere (
        FormatorAmbitus* ambitus,
             SilvaNodus* nodus,
    constans SilvaNodus* caput)
{
     SilvaToken* clausum;
     SilvaToken* verbum_alioquin;
     SilvaNodus* consequens;
     SilvaNodus* ramus_alioquin;
     SilvaValor  valor_alioquin;

    clausum     = _valor_radix(silva_c89_si_tok_clausum(nodus));
    consequens  = _valor_nodus(silva_c89_si_consequens(nodus));

    /* R17: catena logica in conditione (ANTE percursum -
     * vindicationes R10 praecedere debent) */
    _catenam_censere(ambitus, silva_c89_si_tok_apertum(nodus),
        silva_c89_si_tok_clausum(nodus),
        _valor_nodus(silva_c89_si_conditio(nodus)));

    /* R11: conditio multi-linearis = continuationes */
    si (_principalis(ambitus, clausum))
    {
        i32 la;
        i32 ca;
        i32 lb;
        i32 cb;

        si (_extensio(nodus, ambitus->fons_princeps, &la, &ca,
            &lb, &cb))
        {
            _spatium_continuationis_addere(ambitus, la, ca,
                clausum->linea);
        }
    }

    si (   consequens
        && consequens->genus == SILVA_C89_GENUS_CORPUS)
    {
        _corpus_censere(ambitus, consequens, caput);
    }
    alioquin si (clausum)
    {
        _ramum_censere(ambitus, silva_c89_si_consequens(nodus),
            clausum);
    }
    _nodum_percurrere(ambitus,
        _valor_nodus(silva_c89_si_conditio(nodus)));
    _nodum_percurrere(ambitus, consequens);

    valor_alioquin = silva_c89_si_alioquin(nodus);
    ramus_alioquin = _valor_nodus(valor_alioquin);
    verbum_alioquin = _valor_radix(
        silva_c89_si_tok_alioquin(nodus));
    si (ramus_alioquin)
    {
        si (ramus_alioquin->genus == SILVA_C89_GENUS_SI)
        {
            _si_censere(ambitus, ramus_alioquin, caput);
        }
        alioquin
        {
            si (ramus_alioquin->genus == SILVA_C89_GENUS_CORPUS)
            {
                _corpus_censere(ambitus, ramus_alioquin, caput);
            }
            alioquin si (verbum_alioquin)
            {
                _ramum_censere(ambitus, valor_alioquin,
                    verbum_alioquin);
            }
            _nodum_percurrere(ambitus, ramus_alioquin);
        }
    }
}

interior vacuum
_nodum_percurrere (
    FormatorAmbitus* ambitus,
         SilvaNodus* nodus)
{
    si (!nodus) redde;

    commutatio (nodus->genus)
    {
        casus SILVA_C89_GENUS_AMBIGUUS:
            /* lectiones plures vivae (oraculum non decrevit) -
             * arbor ipsa nescit quid sit, ergo spatia iudicari
             * NEQUEUNT. Ambulare ambas lectiones = regulae
             * contradictorias imponere (oscillatio '(i32)-I'
             * cast/binarium quam tabula_dispersa detexit). */
            redde;
        casus SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS:
        {
            SilvaNodus* corpus;

            /* definitio ex macro expansa (AED_CTYPE(x, f) -> functio):
             * lexemata omnia ad invocationem collabuntur - R1/R2/R8 et
             * corpus columnas mendaces iudicarent et in aeternum
             * pugnarent (officina_machinula.c: punctum fixum XII non
             * attinctum). Geometria infida = nodus non iudicatur. */
            si (!silva_nodus_geometria_fida(ambitus->piscina, nodus,
                ambitus->fons_princeps))
            {
                frange;
            }
            corpus = _valor_nodus(
                silva_c89_definitio_functionis_corpus(nodus));
            _definitionem_censere(ambitus, nodus);
            si (corpus) _corpus_censere(ambitus, corpus, nodus);
            frange;
        }
        casus SILVA_C89_GENUS_SI:
            _si_censere(ambitus, nodus, nodus);
            redde;   /* subarbor manualiter percursa */
        casus SILVA_C89_GENUS_DUM:
        {
            SilvaNodus* corpus;
            SilvaToken* clausum;

            corpus = _valor_nodus(silva_c89_dum_corpus(nodus));
            clausum = _valor_radix(
                silva_c89_dum_tok_clausum(nodus));
            _catenam_censere(ambitus,
                silva_c89_dum_tok_apertum(nodus),
                silva_c89_dum_tok_clausum(nodus),
                _valor_nodus(silva_c89_dum_conditio(nodus)));
            si (_principalis(ambitus, clausum))
            {
                i32 la;
                i32 ca;
                i32 lb;
                i32 cb;

                si (_extensio(nodus, ambitus->fons_princeps,
                    &la, &ca, &lb, &cb))
                {
                    _spatium_continuationis_addere(ambitus,
                        la, ca, clausum->linea);
                }
            }
            si (   corpus
                && corpus->genus == SILVA_C89_GENUS_CORPUS)
            {
                _corpus_censere(ambitus, corpus, nodus);
            }
            alioquin si (clausum)
            {
                _ramum_censere(ambitus,
                    silva_c89_dum_corpus(nodus), clausum);
            }
            frange;
        }
        casus SILVA_C89_GENUS_PER:
        {
            SilvaNodus* corpus;
            SilvaToken* clausum;

            corpus = _valor_nodus(silva_c89_per_corpus(nodus));
            clausum = _valor_radix(
                silva_c89_per_tok_clausum(nodus));
            si (_principalis(ambitus, clausum))
            {
                i32 la;
                i32 ca;
                i32 lb;
                i32 cb;

                si (_extensio(nodus, ambitus->fons_princeps,
                    &la, &ca, &lb, &cb))
                {
                    _spatium_continuationis_addere(ambitus,
                        la, ca, clausum->linea);
                }
            }
            si (   corpus
                && corpus->genus == SILVA_C89_GENUS_CORPUS)
            {
                _corpus_censere(ambitus, corpus, nodus);
            }
            alioquin si (clausum)
            {
                _ramum_censere(ambitus,
                    silva_c89_per_corpus(nodus), clausum);
            }
            frange;
        }
        casus SILVA_C89_GENUS_COMMUTATIO:
        {
            SilvaNodus* corpus;

            corpus = _valor_nodus(
                silva_c89_commutatio_corpus(nodus));
            si (   corpus
                && corpus->genus == SILVA_C89_GENUS_CORPUS)
            {
                _corpus_censere(ambitus, corpus, nodus);
            }
            frange;
        }
        casus SILVA_C89_GENUS_VOCATIO:
            _vocationem_censere(ambitus, nodus);
            frange;
        casus SILVA_C89_GENUS_BINARIUM:
            _binarium_operatorem_censere(ambitus, nodus);
            frange;
        casus SILVA_C89_GENUS_ACCESSUS:
            _accessum_censere(ambitus, nodus);
            frange;
        casus SILVA_C89_GENUS_CONVERSIO:
            _conversionem_censere(ambitus, nodus);
            frange;
        casus SILVA_C89_GENUS_UNARIUM:
            _unarium_censere(ambitus, nodus);
            frange;
        casus SILVA_C89_GENUS_SENTENTIA_EXPRESSIONIS:
        casus SILVA_C89_GENUS_DECLARATIO:
        casus SILVA_C89_GENUS_REDDE:
        {
            i32 la;
            i32 ca;
            i32 lb;
            i32 cb;
            b32 forma_bloccalis;

            /* declarationes corpus typi ferentes (structura/
             * unio/enumeratio/congeries) formam bloccalem
             * habent - lineae internae continuationes NON
             * sunt ('};' in columna I rectum est) */
            forma_bloccalis = FALSUM;
            si (nodus->genus == SILVA_C89_GENUS_DECLARATIO)
            {
                SilvaValor specificatores;
                       i32 n_spec;
                       i32 s;

                specificatores =
                    silva_c89_declaratio_specificatores(nodus);
                n_spec = silva_valor_lista_numerus(
                    specificatores);
                per (s = ZEPHYRUM; s < n_spec; s += I)
                {
                    SilvaValor* e;
                    SilvaNodus* n_s;

                    e = silva_valor_lista_obtinere(
                        specificatores, s);
                    n_s = e ? _valor_nodus(*e) : NIHIL;
                    si (   n_s && (n_s->genus
                            == SILVA_C89_GENUS_STRUCTURA
                        || n_s->genus == SILVA_C89_GENUS_UNIO
                        || n_s->genus
                            == SILVA_C89_GENUS_ENUMERATIO))
                    {
                        forma_bloccalis = VERUM;
                        frange;
                    }
                }
            }
            si (   nodus->genus == SILVA_C89_GENUS_DECLARATIO
                && !forma_bloccalis)
            {
                SilvaValor declaratores;
                       i32 n_d;
                       i32 d;

                declaratores = silva_c89_declaratio_declaratores(
                    nodus);
                n_d = silva_valor_lista_numerus(declaratores);
                per (d = ZEPHYRUM; d < n_d && !forma_bloccalis;
                    d += I)
                {
                    SilvaValor* e;
                    SilvaNodus* n_d_nodus;

                    e = silva_valor_lista_obtinere(
                        declaratores, d);
                    n_d_nodus = e ? _valor_nodus(*e) : NIHIL;
                    si (   n_d_nodus && n_d_nodus->genus
                        == SILVA_C89_GENUS_DECLARATOR_INITIATUS
                        && _valor_nodus(
                            silva_c89_declarator_initiatus_initiator(
                                n_d_nodus)) != NIHIL
                        && _valor_nodus(
                            silva_c89_declarator_initiatus_initiator(
                                n_d_nodus))->genus
                            == SILVA_C89_GENUS_CONGERIES)
                    {
                        forma_bloccalis = VERUM;
                    }
                    /* prototypa formam R1 sequuntur (titulus
                     * in columna prima) - lineae eorum
                     * continuationes NON sunt (bellum R11/R1
                     * quod lux prima json.c detexit: lint id
                     * tacite falso numerabat, fix prototypum
                     * physice indentavit et differre unitates
                     * iungere non potuit) */
                    si (   n_d_nodus
                        && _declarator_functionis(n_d_nodus)
                            != NIHIL)
                    {
                        forma_bloccalis = VERUM;
                    }
                }
            }
            si (   !forma_bloccalis
                && _extensio(nodus, ambitus->fons_princeps,
                    &la, &ca, &lb, &cb))
            {
                _spatium_continuationis_addere(ambitus, la, ca,
                    lb);
            }
            frange;
        }
        casus SILVA_C89_GENUS_CORPUS:
            _corpus_interius_censere(ambitus, nodus);
            frange;
        casus SILVA_C89_GENUS_STRUCTURA:
            _membra_censere(ambitus,
                silva_c89_structura_membra(nodus));
            frange;
        casus SILVA_C89_GENUS_UNIO:
            _membra_censere(ambitus,
                silva_c89_unio_membra(nodus));
            frange;
        ordinarius:
            frange;
    }

    {
        Xar* liberi;
        i32  numerus;
        i32  i;

        liberi   = silva_nodus_liberi(ambitus->piscina, nodus);
        numerus  = liberi ? xar_numerus(liberi) : (i32)ZEPHYRUM;
        per (i = ZEPHYRUM; i < numerus; i += I)
        {
            _nodum_percurrere(ambitus,
                *(SilvaNodus**)xar_obtinere(liberi, i));
        }
    }
}


/* ==================================================
 * R11 continuatio (flumine + extensionibus arboris):
 * lineae internae sententiarum >= ca + IV; fractura iuxta
 * operatorem binarium ANTE operatorem (operator initio
 * continuationis, non fine lineae). STAR/AMPERSAND
 * ambigui exclusi; ASSIGNATIO exclusa (mos domus:
 * '=' finem lineae claudere licet).
 * ================================================== */

interior b32
_operator_ducibilis (
    SilvaLexemaGenus genus)
{
    redde genus == SILVA_LEX_PLUS
        || genus == SILVA_LEX_MINUS
        || genus == SILVA_LEX_SOLIDUS
        || genus == SILVA_LEX_PERCENTUM
        || genus == SILVA_LEX_ET_ET
        || genus == SILVA_LEX_VEL_VEL
        || genus == SILVA_LEX_BARRA
        || genus == SILVA_LEX_CARET
        || genus == SILVA_LEX_MINOR
        || genus == SILVA_LEX_MAIOR
        || genus == SILVA_LEX_MINOR_AEQUALIS
        || genus == SILVA_LEX_MAIOR_AEQUALIS
        || genus == SILVA_LEX_AEQUALIS_AEQUALIS
        || genus == SILVA_LEX_NON_AEQUALIS
        || genus == SILVA_LEX_SINISTRORSUM
        || genus == SILVA_LEX_DEXTRORSUM;
}

interior constans ContinuatioSpatium*
_spatium_invenire (
    Xar* continuationes,
    i32  linea,
    b32  interius)
{
    i32 numerus;
    i32 i;

    numerus = continuationes ? xar_numerus(continuationes)
        : (i32)ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        constans ContinuatioSpatium* spatium;

        spatium = (constans ContinuatioSpatium*)xar_obtinere(
            continuationes, i);
        si (interius)
        {
            si (linea > spatium->la && linea <= spatium->lb)
            {
                redde spatium;
            }
        }
        alioquin si (   linea >= spatium->la
                     && linea < spatium->lb)
        {
            redde spatium;
        }
    }
    redde NIHIL;
}

/* lexema primum (non-trivia) lineae datae in fluxu crudo; NIHIL */
interior SilvaToken*
_lexema_primum_lineae (
    Xar* cruda,
    i32  linea)
{
    i32 numerus;
    i32 i;

    numerus = cruda ? xar_numerus(cruda) : (i32)ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        SilvaToken* t;

        t = _lexema(cruda, i);
        si (t->linea < linea) perge;
        si (t->linea > linea) frange;
        si (t->initium_lineae) redde t;
    }
    redde NIHIL;
}

interior vacuum
_continuationes_censere (
    Piscina* piscina,
        Xar* divergentiae,
        Xar* cruda,
        Xar* continuationes)
{
    i32 numerus;
    i32 i;

    si (   !continuationes
        || xar_numerus(continuationes) == (i32)ZEPHYRUM)
    {
        redde;
    }
    numerus = cruda ? xar_numerus(cruda) : (i32)ZEPHYRUM;

    per (i = ZEPHYRUM; i < numerus; i += I)
    {
                         SilvaToken* lexema;
        constans ContinuatioSpatium* spatium;

        lexema = _lexema(cruda, i);

        /* indentatio continuationis */
        si (lexema->initium_lineae)
        {
            spatium = _spatium_invenire(continuationes,
                lexema->linea, VERUM);
            si (   spatium
                && lexema->columna < spatium->ca + IV)
            {
                SilvaToken* ancora;

                ancora = _lexema_primum_lineae(cruda, spatium->la);
                _addere(divergentiae, "continuatio",
                    _nuntius_ancorae(piscina,
                        "continuatio parum indentata",
                        ancora ? (constans character*)
                            ancora->valor.datum : "",
                        ancora ? ancora->valor.mensura
                            : (i32)ZEPHYRUM,
                        spatium->la, " + IV"),
                    lexema->linea, lexema->columna,
                    (s32)lexema->columna,
                    (s32)(spatium->ca + IV));
                _emendare(divergentiae, lexema->linea, I,
                    lexema->linea, I,
                    _textus_emendationis(piscina, ZEPHYRUM,
                        spatium->ca + IV - lexema->columna));
            }
        }

        /* operator claudens lineam intra sententiam */
        si (   _operator_ducibilis(lexema->genus)
            && i + I < numerus)
        {
            i32 j;
            b32 commentum_visum;

            j                = i + I;
            commentum_visum  = FALSUM;
            dum (   j < numerus
                 && (_lexema(cruda, j)->genus == SILVA_LEX_SPATIA
                    || _lexema(cruda, j)->genus
                        == SILVA_LEX_COMMENTUM_CLAUSUM))
            {
                si (_lexema(cruda, j)->genus
                    == SILVA_LEX_COMMENTUM_CLAUSUM)
                {
                    commentum_visum = VERUM;
                }
                j += I;
            }
            si (   j < numerus && _lexema(cruda, j)->genus
                == SILVA_LEX_NOVA_LINEA)
            {
                spatium = _spatium_invenire(continuationes,
                    lexema->linea, FALSUM);
                si (spatium)
                {
                    SilvaToken* sequens;
                           i32  k;

                    _addere(divergentiae, "continuatio",
                        "operator initio continuationis"
                        " ponendus, non fine lineae",
                        lexema->linea, lexema->columna,
                        ZEPHYRUM, ZEPHYRUM);

                    /* motus bi-span: ambo aut neuter (commentum
                     * aut continuatio '\\' in via = neuter) */
                    sequens  = NIHIL;
                    k        = j + I;
                    dum (k < numerus)
                    {
                        SilvaLexemaGenus g;

                        g = _lexema(cruda, k)->genus;
                        si (   g != SILVA_LEX_SPATIA
                            && g != SILVA_LEX_TABULAE
                            && g != SILVA_LEX_NOVA_LINEA)
                        {
                            si (   g != SILVA_LEX_EOF
                                && g != SILVA_LEX_CONTINUATIO
                                && g != SILVA_LEX_COMMENTUM_CLAUSUM
                                && g != SILVA_LEX_COMMENTUM_LINEA)
                            {
                                sequens = _lexema(cruda, k);
                            }
                            frange;
                        }
                        k += I;
                    }
                    si (!commentum_visum && sequens)
                    {
                        _emendare(divergentiae,
                            lexema->linea, lexema->columna,
                            lexema->linea, lexema->columna,
                            _textus_emendationis(piscina, I,
                                spatium->ca + IV - I));
                        _emendare(divergentiae,
                            lexema->linea, lexema->columna
                                + lexema->valor.mensura,
                            sequens->linea, sequens->columna,
                            _textus_emendationis(piscina,
                                ZEPHYRUM, I));
                    }
                }
            }
        }
    }
}


/* ==================================================
 * R16 fractura (FIX): linea longa in candidato optimo
 * scinditur. Lexemata CRUDA (macro-honesta): candidati =
 * SPATIA post virgulam / post '=' / ante operatorem
 * binarium (positione: lexema prius operandum finit -
 * '-' unarium exclusum). Electio: operator logicus
 * profunditate minima (catenae ad &&/|| franguntur),
 * aliter candidatus dextimus intra tectum cadens, nullo
 * cadente sinistimus (cauda iteratione sequenti). Inden-
 * tatio = sententia intima lineam continens, ca + IV -
 * R11/R17 iteratione sequenti perficiunt. Nulla fractura:
 * directivae et continuationes '\', lineae sine candidato
 * (littera chordae sola), cauda commentarii sola trans-
 * grediens (codex cadit; commentum intactum per
 * contractum). Tectum = LXXII planum (porta: sub inden-
 * tatione gravi tectum = ca + XL ut STML).
 * ================================================== */

#define FRACTURA_CANDIDATI_MAXIMI 64

/* candidatus fracturae: index = lexema SPATIA in cruda;
 * profunditas = parentheses relativa initio lineae; logicus = ante
 * && aut ||; genus_proximi = operator sequens (COMMA/ASSIGNATIO =
 * lexema prius: fractura post id) */
nomen structura {
                 i32 index;
                 s32 profunditas;
                 b32 logicus;
    SilvaLexemaGenus genus_proximi;
} FracturaCandidatus;

/* gradus praecedentiae operatoris binarii (maior = artius ligat);
 * ZEPHYRUM = non operator. Indentatio continuationis: candidatus
 * artius quam operator lineam ducens ligans = gradus interior
 * (+ IV), aliter idem gradus (catenae ad idem, argumenta ad idem) */
interior i32
_praecedentia (
    SilvaLexemaGenus genus)
{
    commutatio (genus)
    {
        casus SILVA_LEX_VEL_VEL:            redde I;
        casus SILVA_LEX_ET_ET:              redde II;
        casus SILVA_LEX_BARRA:              redde III;
        casus SILVA_LEX_CARET:              redde IV;
        casus SILVA_LEX_AEQUALIS_AEQUALIS:
        casus SILVA_LEX_NON_AEQUALIS:       redde VI;
        casus SILVA_LEX_MINOR:
        casus SILVA_LEX_MAIOR:
        casus SILVA_LEX_MINOR_AEQUALIS:
        casus SILVA_LEX_MAIOR_AEQUALIS:     redde VII;
        casus SILVA_LEX_SINISTRORSUM:
        casus SILVA_LEX_DEXTRORSUM:         redde VIII;
        casus SILVA_LEX_PLUS:
        casus SILVA_LEX_MINUS:              redde IX;
        casus SILVA_LEX_SOLIDUS:
        casus SILVA_LEX_PERCENTUM:          redde X;
        ordinarius:                         redde ZEPHYRUM;
    }
}

interior b32
_operandum_finiens (
    SilvaLexemaGenus genus)
{
    redde genus == SILVA_LEX_IDENTIFICATOR
        || genus == SILVA_LEX_INTEGER
        || genus == SILVA_LEX_FLOAT
        || genus == SILVA_LEX_CHARACTER_LIT
        || genus == SILVA_LEX_STRING_LIT
        || genus == SILVA_LEX_PAREN_CLAUSA
        || genus == SILVA_LEX_QUADRA_CLAUSA;
}

interior b32
_commentum_est (
    SilvaLexemaGenus genus)
{
    redde genus == SILVA_LEX_COMMENTUM_CLAUSUM
        || genus == SILVA_LEX_COMMENTUM_LINEA;
}

/* sententia intima lineam continens: la maxima, pari la
 * registrata postrema (ordo ambulationis = exterior prius);
 * *ca == ZEPHYRUM post reditum = nulla */
interior vacuum
_sententiam_intimam_invenire (
    Xar* sententiae,
    i32  linea,
    i32* la_exitus,
    i32* ca_exitus)
{
    i32 n;
    i32 i;

    *la_exitus  = ZEPHYRUM;
    *ca_exitus  = ZEPHYRUM;
    n           = sententiae ? xar_numerus(sententiae) : (i32)ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i += I)
    {
        constans ContinuatioSpatium* s;

        s = (constans ContinuatioSpatium*)xar_obtinere(
            sententiae, i);
        si (linea < s->la || linea > s->lb) perge;
        si (s->la >= *la_exitus)
        {
            *la_exitus = s->la;
            *ca_exitus = s->ca;
        }
    }
}

interior vacuum
_fracturas_censere (
    Piscina* piscina,
        Xar* divergentiae,
        Xar* cruda,
        Xar* sententiae,
        i32  tectum)
{
    i32 numerus_div;
    i32 numerus;
    i32 k;

    numerus      = cruda ? xar_numerus(cruda) : (i32)ZEPHYRUM;
    numerus_div  = xar_numerus(divergentiae);

    per (k = ZEPHYRUM; k < numerus_div; k += I)
    {
        FormatorDivergentia* d;
         FracturaCandidatus  candidati[FRACTURA_CANDIDATI_MAXIMI];
                 SilvaToken* prius;
                 SilvaToken* electum;
           SilvaLexemaGenus  genus_primi;
                        i32  n_cand;
                        i32  i;
                        i32  linea;
                        i32  columna_primi;
                        i32  finis_codicis;
                        i32  ca;
                        i32  la;
                        i32  indentatio;
                        i32  electus;
                        b32  habet;
                        b32  primum;
                        b32  commentum_visum;
                        b32  directiva;
                        s32  profunditas;

        d = (FormatorDivergentia*)xar_obtinere(divergentiae, k);
        si (   strcmp(d->regula, "longitudo-lxxii") != ZEPHYRUM
            || d->numerus_emendationum              != (i32)ZEPHYRUM)
        {
            perge;
        }
        linea = d->linea;

        /* cedere regulae alii lineam novam in hac linea inserenti
         * (R8 parametra, R1 titulus): insertiones duae eodem loco
         * lineam vacuam parerent - plagae adiacentes, non imbricatae,
         * ambae applicantur (venatio semantica.c 4861: prototypum cum
         * linea vacua inter parametra). Linea iteratione sequenti
         * denuo iudicatur. */
        {
            b32 cedit;
            i32 j;

            cedit = FALSUM;
            per (j = ZEPHYRUM; j < numerus_div && !cedit; j += I)
            {
                constans FormatorDivergentia* alia;
                                         i32  e;

                si (j == k) perge;
                alia = (constans FormatorDivergentia*)xar_obtinere(
                    divergentiae, j);
                per (e = ZEPHYRUM; e < alia->numerus_emendationum;
                    e += I)
                {
                    constans FormatorEmendatio* em;

                    em = &alia->emendationes[e];
                    si ((em->linea_a == linea || em->linea_b == linea)
                        && em->insertum.mensura > (i32)ZEPHYRUM
                        && em->insertum.datum[ZEPHYRUM] == '\n')
                    {
                        cedit = VERUM;
                        frange;
                    }
                }
            }
            si (cedit) perge;
        }

        n_cand           = ZEPHYRUM;
        prius            = NIHIL;
        primum           = VERUM;
        columna_primi    = ZEPHYRUM;
        genus_primi      = SILVA_LEX_EOF;
        finis_codicis    = ZEPHYRUM;
        commentum_visum  = FALSUM;
        directiva        = FALSUM;
        profunditas      = ZEPHYRUM;

        per (i = ZEPHYRUM; i < numerus; i += I)
        {
                  SilvaToken* t;
            SilvaLexemaGenus  g;

            t = _lexema(cruda, i);
            g = t->genus;
            /* linea prior continuata ('\') = corpus macri */
            si (t->linea == linea - I && g == SILVA_LEX_CONTINUATIO)
            {
                directiva = VERUM;
            }
            si (t->linea < linea) perge;
            si (   t->linea > linea || g == SILVA_LEX_NOVA_LINEA
                || g == SILVA_LEX_EOF)
            {
                frange;
            }
            si (g == SILVA_LEX_CONTINUATIO)
            {
                directiva = VERUM;
                frange;
            }
            si (_commentum_est(g))
            {
                commentum_visum = VERUM;
                perge;
            }
            si (g == SILVA_LEX_SPATIA || g == SILVA_LEX_TABULAE)
            {
                SilvaToken* proximum;

                si (   prius  == NIHIL || commentum_visum
                    || i + I  >= numerus
                    || n_cand >= (i32)FRACTURA_CANDIDATI_MAXIMI)
                {
                    perge;
                }
                proximum = _lexema(cruda, i + I);
                si (   proximum->linea != linea
                    || _commentum_est(proximum->genus)
                    || proximum->genus == SILVA_LEX_NOVA_LINEA
                    || proximum->genus == SILVA_LEX_CONTINUATIO
                    || proximum->genus == SILVA_LEX_SPATIA
                    || proximum->genus == SILVA_LEX_TABULAE
                    || proximum->genus == SILVA_LEX_EOF)
                {
                    perge;
                }
                si (   prius->genus == SILVA_LEX_COMMA
                    || prius->genus == SILVA_LEX_ASSIGNATIO)
                {
                    candidati[n_cand].index          = i;
                    candidati[n_cand].profunditas    = profunditas;
                    candidati[n_cand].logicus        = FALSUM;
                    candidati[n_cand].genus_proximi  = prius->genus;
                    n_cand                           += I;
                }
                alioquin si (   _operator_ducibilis(proximum->genus)
                             && _operandum_finiens(prius->genus))
                {
                    candidati[n_cand].index        = i;
                    candidati[n_cand].profunditas  = profunditas;
                    candidati[n_cand].logicus       =
                        proximum->genus == SILVA_LEX_ET_ET
                        || proximum->genus == SILVA_LEX_VEL_VEL;
                    candidati[n_cand].genus_proximi =
                        proximum->genus;
                    n_cand += I;
                }
                perge;
            }

            /* lexema codicis */
            si (primum)
            {
                primum         = FALSUM;
                columna_primi  = t->columna;
                genus_primi    = g;
                si (g == SILVA_LEX_CANCELLUM)
                {
                    directiva = VERUM;
                    frange;
                }
            }
            si (!commentum_visum)
            {
                finis_codicis = t->columna + t->valor.mensura - I;
            }
            si (   g == SILVA_LEX_PAREN_APERTA
                || g == SILVA_LEX_QUADRA_APERTA
                || g == SILVA_LEX_BRACE_APERTA)
            {
                profunditas += I;
            }
            alioquin si (   g == SILVA_LEX_PAREN_CLAUSA
                         || g == SILVA_LEX_QUADRA_CLAUSA
                         || g == SILVA_LEX_BRACE_CLAUSA)
            {
                profunditas -= I;
            }
            prius = t;
        }

        si (   directiva || n_cand == (i32)ZEPHYRUM
            || finis_codicis <= tectum)
        {
            perge;
        }

        /* electio I: logici cadentes - profunditas minima, tum
         * dextimus */
        habet    = FALSUM;
        electus  = ZEPHYRUM;
        per (i = ZEPHYRUM; i < n_cand; i += I)
        {
            SilvaToken* t;

            si (!candidati[i].logicus) perge;
            t = _lexema(cruda, candidati[i].index);
            si (t->columna - I > tectum) perge;
            si (   !habet
                || candidati[i].profunditas
                    < candidati[electus].profunditas
                || (candidati[i].profunditas
                        == candidati[electus].profunditas
                    && t->columna
                        > _lexema(cruda,
                            candidati[electus].index)->columna))
            {
                habet    = VERUM;
                electus  = i;
            }
        }
        /* electio II: quilibet cadens dextimus (ordo = columnae) */
        si (!habet)
        {
            per (i = ZEPHYRUM; i < n_cand; i += I)
            {
                si (_lexema(cruda, candidati[i].index)->columna - I
                    <= tectum)
                {
                    habet    = VERUM;
                    electus  = i;
                }
            }
        }
        /* electio III: nullus cadit - sinistimus */
        si (!habet) electus = ZEPHYRUM;

        /* indentatio: linea prima sententiae -> ca + IV; linea
         * continuata -> gradus suus, nisi candidatus artius ligat
         * quam operator lineam ducens (comparatio intra ramum
         * catenae ||) -> gradus interior (+ IV). Sine sententia
         * (parsura fracta, membra, tabulae) linea ipsa ancora. */
        _sententiam_intimam_invenire(sententiae, linea, &la, &ca);
        si (ca == (i32)ZEPHYRUM || la == linea)
        {
            si (ca == (i32)ZEPHYRUM) ca = columna_primi;
            indentatio = ca + IV - I;
        }
        alioquin
        {
            indentatio = columna_primi - I;
            si (_praecedentia(candidati[electus].genus_proximi)
                > _praecedentia(genus_primi))
            {
                indentatio += IV;
            }
        }
        electum = _lexema(cruda, candidati[electus].index);
        _emendationem_ponere(d, linea, electum->columna, linea,
            electum->columna + electum->valor.mensura,
            _textus_emendationis(piscina, I, indentatio), FALSUM);
        d->nuntius = "linea nimis longa - frangibilis";
    }
}


/* ==================================================
 * ambitus nominatus (-intra): extenta functionum
 * titulo datarum; divergentiae extra ea cadunt
 * ================================================== */

nomen structura {
    i32 linea_a;
    i32 linea_b;
} ExtentumIntra;

/* titulus nodi radicis: definitio functionis aut prototypum
 * verum (_prototypi_functionis); NIHIL ceteris */
interior SilvaToken*
_titulus_radicis (
    SilvaNodus* nodus_radicis)
{
    SilvaNodus* functionis;

    si (!nodus_radicis) redde NIHIL;
    si (nodus_radicis->genus
        == SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS)
    {
        functionis = _declarator_functionis(_valor_nodus(
            silva_c89_definitio_functionis_declarator(
                nodus_radicis)));
    }
    alioquin
    {
        functionis = _prototypi_functionis(nodus_radicis);
    }
    si (!functionis) redde NIHIL;
    redde _titulus_declaratoris(_valor_nodus(
        silva_c89_declarator_functionis_internum(functionis)));
}

/* extentum nodi radicis registrare si titulus congruit; inventae
 * notare. Extentum a linea post nodum priorem incipit. */
interior vacuum
_intra_registrare (
    constans FormatorIntra* intra,
                       Xar* extenta,
                SilvaNodus* nodus_radicis,
                       i32  linea_a,
                       i32  linea_b)
{
    SilvaToken* titulus;
           i32  k;

    titulus = _titulus_radicis(nodus_radicis);
    si (!titulus) redde;
    per (k = ZEPHYRUM; k < intra->numerus; k += I)
    {
        ExtentumIntra* e;

        si (!chorda_aequalis(titulus->valor,
            intra->functiones[k]))
        {
            perge;
        }
        si (intra->inventae) intra->inventae[k] = VERUM;
        e = (ExtentumIntra*)xar_addere(extenta);
        si (!e) redde;
        e->linea_a = linea_a;
        e->linea_b = linea_b;
    }
}

/* divergentia TOTA intra extentum aliquod (sedes et emendationes
 * omnes)? Transgredientes cadunt - conservative: dimidium
 * editionis extra ambitum numquam applicatur. */
interior b32
_intra_continetur (
    constans FormatorDivergentia* d,
                             Xar* extenta)
{
    i32 n;
    i32 i;

    n = xar_numerus(extenta);
    per (i = ZEPHYRUM; i < n; i += I)
    {
        constans ExtentumIntra* e;
                           b32  intus;
                           i32  j;

        e      = (constans ExtentumIntra*)xar_obtinere(extenta, i);
        intus  = d->linea >= e->linea_a && d->linea <= e->linea_b;
        per (j = ZEPHYRUM; intus && j < d->numerus_emendationum;
            j += I)
        {
            si (   d->emendationes[j].linea_a < e->linea_a
                || d->emendationes[j].linea_b > e->linea_b)
            {
                intus = FALSUM;
            }
        }
        si (intus) redde VERUM;
    }
    redde FALSUM;
}

/* divergentias extra extenta demere (copia nova; NIHIL si piscina
 * fracta - contractus formator_lint) */
interior Xar*
_intra_servare (
    Piscina* piscina,
        Xar* divergentiae,
        Xar* extenta)
{
    Xar* servatae;
    i32  n;
    i32  i;

    servatae = xar_creare(piscina,
        magnitudo(FormatorDivergentia));
    si (!servatae) redde NIHIL;
    n = xar_numerus(divergentiae);
    per (i = ZEPHYRUM; i < n; i += I)
    {
        FormatorDivergentia* d;
        FormatorDivergentia* copia;

        d = (FormatorDivergentia*)xar_obtinere(divergentiae, i);
        si (!_intra_continetur(d, extenta)) perge;
        copia = (FormatorDivergentia*)xar_addere(servatae);
        si (!copia) redde NIHIL;
        *copia = *d;
    }
    redde servatae;
}


/* ==================================================
 * introitus
 * ================================================== */

Xar*
formator_lint_intra (
                   Piscina* piscina,
            SilvaContextus* contextus,
        constans character* fons,
                       i32  mensura,
    constans FormatorIntra* intra)
{
             Xar* divergentiae;
             Xar* cruda;
             Xar* extenta_intra;
             Xar* sententiae;
    constans i32* longitudines;
    constans i32* initia_linearum;
    SilvaParsura* parsura;
             i32  numerus;
             i32  numerus_linearum;
             i32  i;

    extenta_intra     = NIHIL;
    sententiae        = NIHIL;
    numerus_linearum  = ZEPHYRUM;
    si (intra)
    {
        si (intra->inventae)
        {
            per (i = ZEPHYRUM; i < intra->numerus; i += I)
            {
                intra->inventae[i] = FALSUM;
            }
        }
        extenta_intra = xar_creare(piscina,
            magnitudo(ExtentumIntra));
        si (!extenta_intra) redde NIHIL;
    }

    divergentiae = xar_creare(piscina,
        magnitudo(FormatorDivergentia));
    si (!divergentiae || !fons) redde divergentiae;
    longitudines = _longitudines_metiri(piscina, fons, mensura,
        &numerus_linearum, &initia_linearum);
    si (!longitudines) redde NIHIL;

    cruda    = silva_lexare_cruda(piscina, fons, mensura, ZEPHYRUM);
    numerus  = cruda ? xar_numerus(cruda) : (i32)ZEPHYRUM;

    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        SilvaToken* lexema;

        lexema = _lexema(cruda, i);

        /* R5: tabulae (genus proprium - litterae immunes);
         * emendatio: tabula = IV spatia (decretum G2) */
        si (lexema->genus == SILVA_LEX_TABULAE)
        {
            _addere(divergentiae, "indentatio-quaterna",
                "tabulae pro spatiis", lexema->linea,
                lexema->columna, (s32)lexema->valor.mensura,
                ZEPHYRUM);
            _emendare(divergentiae, lexema->linea,
                lexema->columna, lexema->linea,
                lexema->columna + lexema->valor.mensura,
                _textus_emendationis(piscina, ZEPHYRUM,
                    lexema->valor.mensura * IV));
        }

        /* R12: spatia in cauda lineae */
        si (   (lexema->genus == SILVA_LEX_SPATIA
                || lexema->genus == SILVA_LEX_TABULAE)
            && i + I < numerus)
        {
            SilvaToken* sequens;

            sequens = _lexema(cruda, i + I);
            si (   sequens->genus == SILVA_LEX_NOVA_LINEA
                || sequens->genus == SILVA_LEX_EOF)
            {
                _addere(divergentiae, "spatia-caudae",
                    "spatia in cauda lineae", lexema->linea,
                    lexema->columna,
                    (s32)lexema->valor.mensura, ZEPHYRUM);
                _emendare(divergentiae, lexema->linea,
                    lexema->columna, lexema->linea,
                    lexema->columna + lexema->valor.mensura,
                    _textus_emendationis(piscina, ZEPHYRUM,
                        ZEPHYRUM));
            }
        }

        /* R6: spatium unicum inter clavem et parenthesim */
        si (   _clavis_ante_parenthesim(lexema)
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
                _emendare(divergentiae, sequens->linea,
                    sequens->columna, sequens->linea,
                    sequens->columna,
                    _textus_emendationis(piscina, ZEPHYRUM, I));
            }
            alioquin si (   sequens->genus         == SILVA_LEX_SPATIA
                         && sequens->valor.mensura != (i32)I
                         && i + II < numerus
                         && _lexema(cruda, i + II)->genus
                         == SILVA_LEX_PAREN_APERTA)
            {
                _addere(divergentiae, "spatium-post-claves",
                    "spatium unicum post clavem exspectatum",
                    lexema->linea, lexema->columna,
                    (s32)sequens->valor.mensura, I);
                _emendare(divergentiae, sequens->linea,
                    sequens->columna, sequens->linea,
                    sequens->columna + sequens->valor.mensura,
                    _textus_emendationis(piscina, ZEPHYRUM, I));
            }
        }

        /* R14: vexilla intra commenta sola */
        si (   lexema->genus == SILVA_LEX_COMMENTUM_CLAUSUM
            || lexema->genus == SILVA_LEX_COMMENTUM_LINEA)
        {
            _vexilla_censere(divergentiae, lexema);
        }

        /* R10 (virgula): nullum spatium ante, spatium post */
        si (lexema->genus == SILVA_LEX_COMMA)
        {
            si (   i != (i32)ZEPHYRUM
                && _lexema(cruda, i - I)->genus
                    == SILVA_LEX_SPATIA)
            {
                SilvaToken* spatia;

                spatia = _lexema(cruda, i - I);
                _addere(divergentiae, "operatores",
                    "nullum spatium ante virgulam",
                    lexema->linea, lexema->columna,
                    (s32)spatia->valor.mensura, ZEPHYRUM);
                _emendare(divergentiae, spatia->linea,
                    spatia->columna, spatia->linea,
                    spatia->columna + spatia->valor.mensura,
                    _textus_emendationis(piscina, ZEPHYRUM,
                        ZEPHYRUM));
            }
            si (i + I < numerus)
            {
                SilvaLexemaGenus g;

                g = _lexema(cruda, i + I)->genus;
                si (   g != SILVA_LEX_SPATIA
                    && g != SILVA_LEX_NOVA_LINEA
                    && g != SILVA_LEX_CONTINUATIO
                    && g != SILVA_LEX_COMMENTUM_CLAUSUM
                    && g != SILVA_LEX_COMMENTUM_LINEA
                    && g != SILVA_LEX_EOF)
                {
                    _addere(divergentiae, "operatores",
                        "spatium post virgulam deest",
                        lexema->linea, lexema->columna,
                        ZEPHYRUM, I);
                    _emendare(divergentiae, lexema->linea,
                        lexema->columna + I, lexema->linea,
                        lexema->columna + I,
                        _textus_emendationis(piscina, ZEPHYRUM,
                            I));
                }
            }
        }
    }

    _longitudinem_censere(divergentiae, longitudines,
        numerus_linearum);
    _finem_censere(piscina, divergentiae, fons, mensura);
    _intervalla_censere(piscina, divergentiae, cruda);
    _inclusiones_ordinem_censere(divergentiae, cruda);

    /* pars arboris: parsura fracta => regulae fluminis solae
     * (fragmenta licita - lint numquam frangit) */
    si (contextus == NIHIL)
    {
        contextus = silva_contextus_creare(piscina);
        si (contextus == NIHIL) redde divergentiae;
        silva_contextus_latinam_addere(contextus);
    }
    parsura = silva_c89_parsare_cum_contextu(piscina, contextus,
        "lint", fons, mensura, NIHIL);
    si (   parsura && parsura->successus
        && parsura->numerus_errorum == ZEPHYRUM
        && parsura->commissio)
    {
        FormatorAmbitus ambitus;
             SilvaValor radix;
                    i32 lb_prior;
                    i32 lb_radicis_prior;

        ambitus.divergentiae   = divergentiae;
        ambitus.piscina        = piscina;
        ambitus.fons_princeps  = parsura->fons_princeps;
        ambitus.continuationes = xar_creare(piscina,
            magnitudo(ContinuatioSpatium));
        ambitus.catena_ops     = xar_creare(piscina,
            magnitudo(CatenaSedes));
        ambitus.sententiae     = xar_creare(piscina,
            magnitudo(ContinuatioSpatium));
        ambitus.longitudines      = longitudines;
        ambitus.numerus_linearum  = numerus_linearum;
        ambitus.initia_linearum   = initia_linearum;
        ambitus.fons              = fons;
        radix                     = parsura->commissio->radix;
        lb_prior                  = ZEPHYRUM;
        lb_radicis_prior          = ZEPHYRUM;

        si (radix.genus == SILVA_VALOR_LISTA)
        {
            i32 n;
            i32 j;

            n = silva_valor_lista_numerus(radix);
            per (j = ZEPHYRUM; j < n; j += I)
            {
                SilvaValor* elementum;
                SilvaNodus* nodus_radicis;

                elementum = silva_valor_lista_obtinere(radix,
                    j);
                nodus_radicis = elementum
                    ? _valor_nodus(*elementum) : NIHIL;
                si (!nodus_radicis) perge;

                /* ambitus nominatus: extentum radicis a linea
                 * post nodum priorem (commentarium ducens et
                 * intervalla supra ad hanc functionem pertinent) */
                si (intra)
                {
                    i32 la_r;
                    i32 ca_r;
                    i32 lb_r;
                    i32 cb_r;

                    si (_extensio(nodus_radicis,
                        ambitus.fons_princeps, &la_r, &ca_r,
                        &lb_r, &cb_r))
                    {
                        _intra_registrare(intra, extenta_intra,
                            nodus_radicis, lb_radicis_prior + I,
                            lb_r);
                        lb_radicis_prior = lb_r;
                    }
                }

                /* R13: una linea vacua inter functiones
                 * (commentarium ducens ad functionem
                 * pertinet; vexillum interpositum = regula
                 * vexillorum, par omissum) */
                si (   nodus_radicis->genus
                    == SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS
                    && silva_nodus_geometria_fida(ambitus.piscina,
                        nodus_radicis, ambitus.fons_princeps))
                {
                    i32 la;
                    i32 ca;
                    i32 lb;
                    i32 cb;

                    si (_extensio(nodus_radicis,
                        ambitus.fons_princeps, &la, &ca, &lb,
                        &cb))
                    {
                        SilvaCommentariumVista vista;
                                           i32 la_effectiva;

                        la_effectiva = la;
                        si (   silva_commentarium_ducens(
                            nodus_radicis,
                            ambitus.fons_princeps, &vista)
                            != ZEPHYRUM
                            && (i32)vista.linea < la_effectiva)
                        {
                            la_effectiva = (i32)vista.linea;
                        }
                        si (   lb_prior != (i32)ZEPHYRUM
                            && !_regio_vexillum_habet(fons,
                                mensura, lb_prior + I,
                                la_effectiva - I)
                            && !_regio_directivam_habet(fons,
                                mensura, lb_prior + I,
                                la_effectiva - I)
                            && la_effectiva - lb_prior - I
                                != (i32)I)
                        {
                            i32 vacuae_inter;

                            vacuae_inter = la_effectiva
                                - lb_prior - I;
                            _addere(divergentiae,
                                "intervalla",
                                "una linea vacua inter"
                                " functiones exspectata",
                                la_effectiva, I,
                                (s32)vacuae_inter, I);
                            si (vacuae_inter == (i32)ZEPHYRUM)
                            {
                                /* conglutinatae: nova ante */
                                _emendare(divergentiae,
                                    la_effectiva, I,
                                    la_effectiva, I,
                                    _textus_emendationis(
                                        piscina, I, ZEPHYRUM));
                            }
                            alioquin
                            {
                                /* nimis: unam vacuam servare
                                 * (tolerans: commentum liberum
                                 * in spatio obstruere potest) */
                                _emendare_tolerans(divergentiae,
                                    lb_prior + II, I,
                                    la_effectiva, I,
                                    _textus_emendationis(
                                        piscina, ZEPHYRUM,
                                        ZEPHYRUM));
                            }
                        }
                        lb_prior = lb;
                    }
                }
                alioquin
                {
                    lb_prior = ZEPHYRUM;
                }

                /* prototypa radicis (porta 2a clausa): R1/R2/
                 * R8/R7-parametra - locales NON tanguntur (haec
                 * via radicem solam ambulat) */
                {
                    SilvaNodus* functionis;

                    functionis = _prototypi_functionis(
                        nodus_radicis);
                    si (   functionis
                        && silva_nodus_geometria_fida(ambitus.piscina,
                            nodus_radicis, ambitus.fons_princeps))
                    {
                        _functionis_caput_censere(&ambitus,
                            functionis);
                        _parametra_ordinem_functionis(&ambitus,
                            functionis);
                    }
                }

                _nodum_percurrere(&ambitus, nodus_radicis);
            }
        }
        alioquin si (radix.genus == SILVA_VALOR_NODUS)
        {
            _nodum_percurrere(&ambitus, radix.datum.nodus);
        }

        _continuationes_censere(piscina, divergentiae, cruda,
            ambitus.continuationes);
        sententiae = ambitus.sententiae;
    }

    /* R16 fractura post arborem (sententiae) - parsura fracta =
     * sententiae nullae, indentatio ex linea ipsa */
    _fracturas_censere(piscina, divergentiae, cruda, sententiae,
        (i32)LONGITUDO_RECTA);

    si (intra)
    {
        redde _intra_servare(piscina, divergentiae,
            extenta_intra);
    }
    redde divergentiae;
}

Xar*
formator_lint (
               Piscina* piscina,
        SilvaContextus* contextus,
    constans character* fons,
                   i32  mensura)
{
    redde formator_lint_intra(piscina, contextus, fons, mensura,
        NIHIL);
}

Xar*
formator_extenta (
               Piscina* piscina,
        SilvaContextus* contextus,
    constans character* fons,
                   i32  mensura)
{
             Xar* extenta;
    SilvaParsura* parsura;
      SilvaValor  radix;
             i32  lb_prior;
             i32  n;
             i32  j;

    extenta = xar_creare(piscina, magnitudo(FormatorExtentum));
    si (!extenta || !fons) redde extenta;
    si (contextus == NIHIL)
    {
        contextus = silva_contextus_creare(piscina);
        si (contextus == NIHIL) redde extenta;
        silva_contextus_latinam_addere(contextus);
    }
    parsura = silva_c89_parsare_cum_contextu(piscina, contextus,
        "extenta", fons, mensura, NIHIL);
    si (   !parsura || !parsura->successus
        || parsura->numerus_errorum != ZEPHYRUM || !parsura->commissio)
    {
        redde extenta;
    }
    radix = parsura->commissio->radix;
    si (radix.genus != SILVA_VALOR_LISTA) redde extenta;

    lb_prior  = ZEPHYRUM;
    n         = silva_valor_lista_numerus(radix);
    per (j = ZEPHYRUM; j < n; j += I)
    {
        SilvaValor* elementum;
        SilvaNodus* nodus_radicis;
        SilvaToken* titulus;
               i32  la;
               i32  ca;
               i32  lb;
               i32  cb;

        elementum      = silva_valor_lista_obtinere(radix, j);
        nodus_radicis  = elementum ? _valor_nodus(*elementum) : NIHIL;
        si (!nodus_radicis) perge;
        si (!_extensio(nodus_radicis, parsura->fons_princeps, &la,
            &ca, &lb, &cb))
        {
            perge;
        }
        titulus = _titulus_radicis(nodus_radicis);
        si (titulus)
        {
            FormatorExtentum* x;

            x = (FormatorExtentum*)xar_addere(extenta);
            si (!x) redde NIHIL;
            x->titulus     = titulus->valor;
            x->linea_a     = lb_prior + I;
            x->linea_nodi  = la;
            x->linea_b     = lb;
            x->definitio = nodus_radicis->genus
                == SILVA_C89_GENUS_DEFINITIO_FUNCTIONIS;
        }
        lb_prior = lb;
    }
    redde extenta;
}


/* ==================================================
 * G2 -scribere: applicatio emendationum ad punctum
 * fixum. Applicator caecus et suspiciosus: octetos
 * spatiales SOLUM delet, spatia/novas solum inserit,
 * editiones '\n' lineas directivarum numquam tangunt,
 * plagae imbricatae dilatantur (iteratio sequens eas
 * denuo generat), applicatio descendens (coordinatae
 * omnes in textu originali validae manent). Regulae
 * per iterationem componuntur, non per choreographiam.
 * ================================================== */

#define ITERATIONES_MAXIMAE 12

nomen structura {
       i32 offset_a;
       i32 offset_b;
    chorda insertum;
} EmendatioPlana;

nomen structura {
    i32* initia;      /* [numerus + I]; ultimum = mensura */
    b32* directivae;  /* [numerus] */
    i32  numerus;
} IndexLinearum;

interior b32
_lineas_metiri (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura,
         IndexLinearum* index)
{
    i32 lineae;
    i32 i;
    i32 l;

    lineae = I;
    per (i = ZEPHYRUM; i < mensura; i += I)
    {
        si (fons[i] == '\n') lineae += I;
    }
    index->numerus = lineae;
    index->initia  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(lineae + I) * magnitudo(i32));
    index->directivae = (b32*)piscina_allocare(piscina,
        (memoriae_index)lineae * magnitudo(b32));
    si (!index->initia || !index->directivae) redde FALSUM;

    l                 = ZEPHYRUM;
    index->initia[l]  = ZEPHYRUM;
    l                 += I;
    per (i = ZEPHYRUM; i < mensura; i += I)
    {
        si (fons[i] == '\n')
        {
            index->initia[l]  = i + I;
            l                 += I;
        }
    }
    index->initia[lineae] = mensura;

    per (l = ZEPHYRUM; l < lineae; l += I)
    {
        i32 a;
        i32 b;
        i32 j;
        b32 directiva;

        a          = index->initia[l];
        b          = index->initia[l + I];
        directiva  = FALSUM;
        per (j = a; j < b; j += I)
        {
            si (fons[j] == ' ' || fons[j] == '\t') perge;
            directiva = fons[j] == '#';
            frange;
        }
        si (   !directiva && l != (i32)ZEPHYRUM
            && index->directivae[l - I])
        {
            /* continuatio: linea prior directiva in '\\'
             * desinens ('\n' ad initia[l] - I sedet) */
            i32 f;

            f = index->initia[l];
            si (f >= (i32)II)
            {
                i32 ante;

                ante = f - II;
                si (fons[ante] == '\r')
                {
                    si (   ante           != (i32)ZEPHYRUM
                        && fons[ante - I] == '\\')
                    {
                        directiva = VERUM;
                    }
                }
                alioquin si (fons[ante] == '\\')
                {
                    directiva = VERUM;
                }
            }
        }
        index->directivae[l] = directiva;
    }
    redde VERUM;
}

interior i32
_offset_e_coordinatis (
    constans IndexLinearum* index,
                       i32  mensura,
                       i32  linea,
                       i32  columna)
{
    i32 offset;

    si (   linea < (i32)I || linea > index->numerus
        || columna < (i32)I)
    {
        redde mensura + I;
    }
    offset = index->initia[linea - I] + columna - I;
    si (offset > mensura) redde mensura + I;
    redde offset;
}

interior b32
_octetus_spatialis (
    character c)
{
    redde c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

/* emendationes divergentiarum complanare et custodire.
 * violatio_spatialis: emendatio octetos non-spatiales delere
 * (aut inserere) conata est - vitium detectoris, vocator
 * plagulam totam recusat, numquam tacet. Emendationes lineas
 * directivarum '\n' tangentes tacite dilatantur (residuae
 * honestae - lint eas adhuc nominat). */
interior i32
_emendationes_complanare (
                   Piscina*  piscina,
                       Xar*  divergentiae,
    constans IndexLinearum*  index,
        constans character*  fons,
                       i32   mensura,
            EmendatioPlana** exitus,
                       b32*  violatio_spatialis)
{
    EmendatioPlana* planae;
               i32  capacitas;
               i32  n;
               i32  numerus_div;
               i32  i;

    *violatio_spatialis  = FALSUM;
    *exitus              = NIHIL;
    numerus_div          = xar_numerus(divergentiae);

    capacitas = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus_div; i += I)
    {
        FormatorDivergentia* d;

        d = (FormatorDivergentia*)xar_obtinere(divergentiae, i);
        capacitas += d->numerus_emendationum;
    }
    si (capacitas == (i32)ZEPHYRUM) redde ZEPHYRUM;

    planae = (EmendatioPlana*)piscina_allocare(piscina,
        (memoriae_index)capacitas * magnitudo(EmendatioPlana));
    si (!planae) redde ZEPHYRUM;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < numerus_div; i += I)
    {
        FormatorDivergentia* d;
                        i32  e;

        d = (FormatorDivergentia*)xar_obtinere(divergentiae, i);
        per (e = ZEPHYRUM; e < d->numerus_emendationum; e += I)
        {
            constans FormatorEmendatio* em;
                                   i32  a;
                                   i32  b;
                                   i32  j;
                                   b32  sana;
                                   b32  habet_novam;

            em = &d->emendationes[e];
            a  = _offset_e_coordinatis(index, mensura,
                em->linea_a, em->columna_a);
            b  = _offset_e_coordinatis(index, mensura,
                em->linea_b, em->columna_b);
            si (a > mensura || b > mensura || b < a) perge;

            /* custodia spatialis: deleta spatialia solum */
            sana         = VERUM;
            habet_novam  = FALSUM;
            per (j = a; j < b; j += I)
            {
                si (!_octetus_spatialis(fons[j]))
                {
                    sana = FALSUM;
                    frange;
                }
                si (fons[j] == '\n') habet_novam = VERUM;
            }
            per (j = ZEPHYRUM;
                sana && j < em->insertum.mensura; j += I)
            {
                character c;

                c = (character)em->insertum.datum[j];
                si (c != ' ' && c != '\n')
                {
                    sana = FALSUM;
                }
                si (c == '\n') habet_novam = VERUM;
            }
            si (!sana)
            {
                /* tolerans: iunctura licite obstructa (commentum,
                 * continuatio) - tacite dilatata, non vitium */
                si (!em->tolerans)
                {
                    *violatio_spatialis = VERUM;
                }
                perge;
            }

            /* custodia directivarum: editio '\n' lineam
             * praeprocessoris scinderet */
            si (habet_novam)
            {
                b32 tangit;
                i32 lin;

                tangit = FALSUM;
                per (lin = em->linea_a;
                    lin <= em->linea_b && lin <= index->numerus;
                    lin += I)
                {
                    si (index->directivae[lin - I])
                    {
                        tangit = VERUM;
                    }
                }
                si (tangit) perge;
            }

            planae[n].offset_a  = a;
            planae[n].offset_b  = b;
            planae[n].insertum  = em->insertum;
            n                   += I;
        }
    }
    *exitus = planae;
    redde n;
}

interior integer
_emendationes_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans EmendatioPlana* ea;
    constans EmendatioPlana* eb;

    ea = (constans EmendatioPlana*)a;
    eb = (constans EmendatioPlana*)b;
    si (ea->offset_a != eb->offset_a)
    {
        redde ea->offset_a < eb->offset_a ? I : -I;
    }
    si (ea->offset_b != eb->offset_b)
    {
        redde ea->offset_b < eb->offset_b ? I : -I;
    }
    redde ZEPHYRUM;
}

/* descendens acceptare (imbricatae dilatae), ascendens
 * aedificare */
interior chorda
_emendationes_applicare (
            Piscina* piscina,
             chorda  fons,
     EmendatioPlana* planae,
                i32  numerus,
                i32* applicatae)
{
    ChordaAedificator* aed;
                  b32* acceptae;
                  i32  min_initium;
                  i32  i;
                  s32  j;
                  i32  cursor;
               chorda  vacua;

    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;
    *applicatae    = ZEPHYRUM;

    qsort(planae, (memoriae_index)numerus,
        magnitudo(EmendatioPlana), _emendationes_comparare);

    acceptae = (b32*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(b32));
    si (!acceptae) redde vacua;

    min_initium = fons.mensura + I;
    per (i = ZEPHYRUM; i < numerus; i += I)
    {
        si (   planae[i].offset_b <= min_initium
            && planae[i].offset_a < min_initium)
        {
            acceptae[i] = VERUM;
            min_initium = planae[i].offset_a;
            *applicatae += I;
        }
        alioquin
        {
            acceptae[i] = FALSUM;
        }
    }

    aed = chorda_aedificator_creare(piscina, fons.mensura + 64);
    si (!aed) redde vacua;
    cursor = ZEPHYRUM;
    per (j = (s32)numerus - I; j >= (s32)ZEPHYRUM; j -= I)
    {
        chorda pars;

        si (!acceptae[j]) perge;
        pars.datum    = fons.datum + cursor;
        pars.mensura  = planae[j].offset_a - cursor;
        chorda_aedificator_appendere_chorda(aed, pars);
        si (planae[j].insertum.mensura != (i32)ZEPHYRUM)
        {
            chorda_aedificator_appendere_chorda(aed,
                planae[j].insertum);
        }
        cursor = planae[j].offset_b;
    }
    {
        chorda pars;

        pars.datum    = fons.datum + cursor;
        pars.mensura  = fons.mensura - cursor;
        chorda_aedificator_appendere_chorda(aed, pars);
    }
    redde chorda_aedificator_finire(aed);
}

/* porta serierum: lexemata non-spatialia (commenta INCLUSA -
 * octeti eorum identici esse debent) genere et valore paria */
interior b32
_trivia_spatialis_est (
    SilvaLexemaGenus genus)
{
    redde genus == SILVA_LEX_SPATIA
        || genus == SILVA_LEX_TABULAE
        || genus == SILVA_LEX_NOVA_LINEA
        || genus == SILVA_LEX_CONTINUATIO;
}

interior b32
_series_aequalis (
               Piscina* piscina,
    constans character* a,
                   i32  na,
    constans character* b,
                   i32  nb)
{
    Xar* cruda_a;
    Xar* cruda_b;
    i32  numerus_a;
    i32  numerus_b;
    i32  ia;
    i32  ib;

    cruda_a = silva_lexare_cruda(piscina, a, na, ZEPHYRUM);
    cruda_b = silva_lexare_cruda(piscina, b, nb, ZEPHYRUM);
    si (!cruda_a || !cruda_b) redde FALSUM;
    numerus_a = xar_numerus(cruda_a);
    numerus_b = xar_numerus(cruda_b);

    ia = ZEPHYRUM;
    ib = ZEPHYRUM;
    per (;;)
    {
        SilvaToken* ta;
        SilvaToken* tb;

        dum (   ia < numerus_a
             && _trivia_spatialis_est(_lexema(cruda_a, ia)->genus))
        {
            ia += I;
        }
        dum (   ib < numerus_b
             && _trivia_spatialis_est(_lexema(cruda_b, ib)->genus))
        {
            ib += I;
        }
        si (ia >= numerus_a || ib >= numerus_b) frange;
        ta = _lexema(cruda_a, ia);
        tb = _lexema(cruda_b, ib);
        si (   ta->genus != tb->genus
            || !chorda_aequalis(ta->valor, tb->valor))
        {
            redde FALSUM;
        }
        ia += I;
        ib += I;
    }
    redde ia >= numerus_a && ib >= numerus_b;
}

FormatorScriptum
formator_scribere_intra (
                   Piscina* piscina,
            SilvaContextus* contextus,
        constans character* fons,
                       i32  mensura,
    constans FormatorIntra* intra)
{
    FormatorScriptum fructus;
       FormatorIntra intra_localis;
              chorda originalis;
              chorda curr;
                 i32 iteratio;
                 b32 convergit;

    fructus.successus    = FALSUM;
    fructus.mutatum      = FALSUM;
    fructus.iterationes  = ZEPHYRUM;
    fructus.applicatae   = ZEPHYRUM;
    fructus.querela      = NIHIL;

    /* copia laboris - fons ipse numquam tangitur */
    originalis.mensura = mensura;
    originalis.datum   = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura + I));
    fructus.textus = originalis;
    si (!originalis.datum)
    {
        fructus.querela = "piscina fracta";
        redde fructus;
    }
    memcpy(originalis.datum, fons, (memoriae_index)mensura);

    /* inventae propriae si vocator nullas dedit - recusatio
     * ignoti proprietas MACHINAE est, non CLI */
    si (   intra && intra->inventae == NIHIL
        && intra->numerus > (i32)ZEPHYRUM)
    {
        intra_localis = *intra;
        intra_localis.inventae = (b32*)piscina_allocare(piscina,
            (memoriae_index)intra->numerus * magnitudo(b32));
        si (!intra_localis.inventae)
        {
            fructus.querela = "piscina fracta";
            redde fructus;
        }
        intra = &intra_localis;
    }

    curr       = originalis;
    convergit  = FALSUM;

    per (iteratio = ZEPHYRUM; iteratio < ITERATIONES_MAXIMAE;
        iteratio += I)
    {
                   Xar* divergentiae;
         IndexLinearum  index;
        EmendatioPlana* planae;
                   i32  numerus_planarum;
                   b32  violatio;
                chorda  nova;
                   i32  applicatae;

        fructus.iterationes = iteratio + I;
        divergentiae = formator_lint_intra(piscina, contextus,
            (constans character*)curr.datum, curr.mensura, intra);
        si (!divergentiae)
        {
            fructus.querela  = "lint fractum";
            fructus.textus   = originalis;
            redde fructus;
        }
        si (intra && iteratio == (i32)ZEPHYRUM)
        {
            i32 k;

            per (k = ZEPHYRUM; k < intra->numerus; k += I)
            {
                si (!intra->inventae[k])
                {
                    fructus.querela  = "functio intra ignota";
                    fructus.textus   = originalis;
                    redde fructus;
                }
            }
        }
        si (!_lineas_metiri(piscina,
            (constans character*)curr.datum, curr.mensura,
            &index))
        {
            fructus.querela  = "piscina fracta";
            fructus.textus   = originalis;
            redde fructus;
        }
        numerus_planarum = _emendationes_complanare(piscina,
            divergentiae, &index,
            (constans character*)curr.datum, curr.mensura,
            &planae, &violatio);
        /* INDAGO (FORMATOR_INDAGO=1): iteratio, emendationes planae,
         * divergentiae armatae (regula:linea:columna) - oscillatio
         * legibilis sine debugger (venatio officina_machinula.c) */
        si (getenv("FORMATOR_INDAGO") != NIHIL)
        {
            i32 di;
            i32 nd;
            i32 impressae;

            nd = xar_numerus(divergentiae);
            fprintf(stderr, "indago iteratio %u: planae %u\n",
                (insignatus integer)(iteratio + I),
                (insignatus integer)numerus_planarum);
            impressae = ZEPHYRUM;
            per (di = ZEPHYRUM; di < nd && impressae < (i32)24;
                di += I)
            {
                constans FormatorDivergentia* dv;

                dv = (constans FormatorDivergentia*)xar_obtinere(
                    divergentiae, di);
                si (dv->numerus_emendationum == (i32)ZEPHYRUM) perge;
                fprintf(stderr, "  %s:%u:%u %s\n", dv->regula,
                    (insignatus integer)dv->linea,
                    (insignatus integer)dv->columna, dv->nuntius);
                impressae += I;
            }
        }
        si (violatio)
        {
            fructus.querela =
                "emendatio non-spatialis (vitium detectoris)";
            fructus.textus   = originalis;
            fructus.mutatum  = FALSUM;
            redde fructus;
        }
        si (numerus_planarum == (i32)ZEPHYRUM)
        {
            convergit = VERUM;
            frange;
        }
        nova = _emendationes_applicare(piscina, curr, planae,
            numerus_planarum, &applicatae);
        si (applicatae == (i32)ZEPHYRUM)
        {
            /* omnes dilatae (directivae) - residuae honestae */
            convergit = VERUM;
            frange;
        }
        si (nova.datum == NIHIL)
        {
            fructus.querela  = "piscina fracta";
            fructus.textus   = originalis;
            fructus.mutatum  = FALSUM;
            redde fructus;
        }
        si (!_series_aequalis(piscina, fons, mensura,
            (constans character*)nova.datum, nova.mensura))
        {
            fructus.querela  = "series lexematum mutata";
            fructus.textus   = originalis;
            fructus.mutatum  = FALSUM;
            redde fructus;
        }
        curr                = nova;
        fructus.mutatum     = VERUM;
        fructus.applicatae  += applicatae;
    }

    si (!convergit)
    {
        fructus.querela =
            "punctum fixum intra XII iterationes non attinctum";
        fructus.textus   = originalis;
        fructus.mutatum  = FALSUM;
        redde fructus;
    }
    fructus.textus     = curr;
    fructus.successus  = VERUM;
    redde fructus;
}

FormatorScriptum
formator_scribere (
               Piscina* piscina,
        SilvaContextus* contextus,
    constans character* fons,
                   i32  mensura)
{
    redde formator_scribere_intra(piscina, contextus, fons,
        mensura, NIHIL);
}

/* (formator_latinam_praebere hic vivebat - custos LATINA_H
 * vacuabatur ut typi plagulae praebitae registrarentur.
 * Involucrum retiratum: typedefs lexici oraculo praeonerantur
 * ab ipsa silva - 94899b2, 01M0D4QV5S pars 1.) */
