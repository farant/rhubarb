/* silva_arbor_aequalitas.c - comparator arborum silvae
 *
 * Plagula PROPRIA, non pars silva_arbor.c: consumptores ultra
 * arborem iam visibiles sunt (portae mutationis rogant 'an haec
 * transformatio arborem servaverit'), et comparator a scriptore
 * et lectore prorsus independens est - solas arbores tangit.
 *
 * Contractus plenus (modi, quid conferatur, quid CONSULTO non
 * videatur) in silva_arbor.h. Lege eum PRIUS quam quicquam hic
 * mutes - praesertim notam de dominio gemino triviorum, quae
 * explicat cur hunc comparatorem 'emendare' ut plus capiat
 * oraculum alterum T6 destrueret.
 */

#include "silva_arbor.h"
#include "xar.h"
#include <string.h>


nomen structura {
     SilvaArborComparatioModus  modus;
         SilvaArborDifferentia* differentia;
                     character  via[SILVA_ARBOR_VIA_CAPACITAS];
                           i32  via_longitudo;
                           i32  profunditas;
} ArborComparator;


/* Decimale in buffer; longitudinem scriptam reddit */
interior i32
_arbor_decimale (
          i32  numerus,
    character* buffer,
          i32  capacitas)
{
    character inversa[XVI];
          i32 longitudo;
          i32 i;

    si (capacitas < II)
    {
        redde ZEPHYRUM;
    }
    si (numerus == ZEPHYRUM)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        redde I;
    }

    longitudo = ZEPHYRUM;
    dum (numerus > ZEPHYRUM && longitudo < (i32)magnitudo(inversa))
    {
        inversa[longitudo]  = (character)('0' + (numerus % X));
        numerus             /= X;
        longitudo++;
    }
    si (longitudo + I > capacitas)
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < longitudo; i++)
    {
        buffer[i] = inversa[longitudo - I - i];
    }
    buffer[longitudo] = '\0';
    redde longitudo;
}

/* Segmentum semitae: 'genus.locus'. Comparator grammatica CARET
 * consulto (arbores duas solas tangit, nullum tabularium accipit),
 * ergo genus numero nominatur, non titulo - vocans quaerere potest.
 * Semita XXXV.0>XII.2 sedem tamen sine ambiguitate figit. */
interior vacuum
_arbor_via_premere (
     ArborComparator* comparator,
                 s32  genus,
                 i32  locus)
{
    character segmentum[XXXII];
          i32 longitudo;
          i32 prior;

    longitudo = _arbor_decimale((i32)genus, segmentum,
        (i32)magnitudo(segmentum));
    si (   longitudo     == ZEPHYRUM
        || longitudo + I >= (i32)magnitudo(segmentum))
    {
        redde;
    }
    segmentum[longitudo] = '.';
    longitudo++;
    longitudo += _arbor_decimale(locus, segmentum + longitudo,
        (i32)magnitudo(segmentum) - longitudo);

    prior = comparator->via_longitudo;

    /* '>' + segmentum + terminator */
    si (prior + longitudo + II >= SILVA_ARBOR_VIA_CAPACITAS)
    {
        redde;
    }
    si (prior > ZEPHYRUM)
    {
        comparator->via[prior] = '>';
        prior++;
    }
    memcpy(comparator->via + prior, segmentum, (size_t)longitudo);
    comparator->via_longitudo                   = prior + longitudo;
    comparator->via[comparator->via_longitudo]  = '\0';
}

interior vacuum
_arbor_via_restituere (
    ArborComparator* comparator,
                i32  longitudo)
{
    comparator->via_longitudo   = longitudo;
    comparator->via[longitudo]  = '\0';
}

/* Divergentiam nominare. Semper FALSUM reddit, ut vocantes
 * 'redde _arbor_divergere(...)' scribere possint. */
interior b32
_arbor_divergere (
        ArborComparator* comparator,
     constans character* campus,
    constans SilvaNodus* nodus_a,
    constans SilvaNodus* nodus_b,
    constans SilvaToken* lexema_a,
    constans SilvaToken* lexema_b,
                    s32  locus,
                    s32  index)
{
    SilvaArborDifferentia* differentia = comparator->differentia;

    si (differentia != NIHIL && differentia->campus == NIHIL)
    {
        differentia->campus    = campus;
        differentia->nodus_a   = nodus_a;
        differentia->nodus_b   = nodus_b;
        differentia->lexema_a  = lexema_a;
        differentia->lexema_b  = lexema_b;
        differentia->locus     = locus;
        differentia->index     = index;
        memcpy(differentia->via, comparator->via,
            (size_t)comparator->via_longitudo + I);
    }
    redde FALSUM;
}


/* ==================================================
 * Lexemata
 * ================================================== */

interior b32
_arbor_lexemata_aequalia (
    ArborComparator*,
    constans SilvaToken*,
    constans SilvaToken*,
    constans SilvaNodus*,
    constans SilvaNodus*,
    s32,
    s32);

/* Series triviorum: genus + valor, ORDINE, ambae listae.
 * Trivia triviorum non habent, ergo recursio hic finitur. */
interior b32
_arbor_trivia_aequalia (
        ArborComparator* comparator,
                    Xar* a,
                    Xar* b,
     constans character* campus,
    constans SilvaNodus* nodus_a,
    constans SilvaNodus* nodus_b,
                    s32  locus)
{
    i32 numerus_a;
    i32 numerus_b;
    i32 i;

    numerus_a = a ? xar_numerus(a) : (i32)ZEPHYRUM;
    numerus_b = b ? xar_numerus(b) : (i32)ZEPHYRUM;

    si (numerus_a != numerus_b)
    {
        redde _arbor_divergere(comparator, campus, nodus_a, nodus_b,
            NIHIL, NIHIL, locus, -I);
    }

    per (i = ZEPHYRUM; i < numerus_a; i++)
    {
        SilvaToken* trivium_a = *(SilvaToken**)xar_obtinere(a, i);
        SilvaToken* trivium_b = *(SilvaToken**)xar_obtinere(b, i);

        si (!_arbor_lexemata_aequalia(comparator, trivium_a, trivium_b,
                 nodus_a, nodus_b, locus, (s32)i))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_arbor_scissurae_aequales (
        ArborComparator* comparator,
    constans SilvaToken* a,
    constans SilvaToken* b,
    constans SilvaNodus* nodus_a,
    constans SilvaNodus* nodus_b,
                    s32  locus)
{
    i32 numerus_a;
    i32 numerus_b;
    i32 i;

    numerus_a = a->scissurae ? xar_numerus(a->scissurae) : (i32)ZEPHYRUM;
    numerus_b = b->scissurae ? xar_numerus(b->scissurae) : (i32)ZEPHYRUM;

    si (numerus_a != numerus_b)
    {
        redde _arbor_divergere(comparator, "scissurae/numerus", nodus_a,
            nodus_b, a, b, locus, -I);
    }

    per (i = ZEPHYRUM; i < numerus_a; i++)
    {
        SilvaScissura* scissura_a =
            (SilvaScissura*)xar_obtinere(a->scissurae, i);
        SilvaScissura* scissura_b =
            (SilvaScissura*)xar_obtinere(b->scissurae, i);

        si (scissura_a == NIHIL || scissura_b == NIHIL)
        {
            redde _arbor_divergere(comparator, "scissura/nihil", nodus_a,
                nodus_b, a, b, locus, (s32)i);
        }
        si (scissura_a->offset != scissura_b->offset)
        {
            redde _arbor_divergere(comparator, "scissura/offset", nodus_a,
                nodus_b, a, b, locus, (s32)i);
        }
        si (scissura_a->crlf != scissura_b->crlf)
        {
            redde _arbor_divergere(comparator, "scissura/crlf", nodus_a,
                nodus_b, a, b, locus, (s32)i);
        }
    }
    redde VERUM;
}

interior b32
_arbor_lexemata_aequalia (
        ArborComparator* comparator,
    constans SilvaToken* a,
    constans SilvaToken* b,
    constans SilvaNodus* nodus_a,
    constans SilvaNodus* nodus_b,
                    s32  locus,
                    s32  index)
{
    si (a == NIHIL || b == NIHIL)
    {
        si (a == b)
        {
            redde VERUM;
        }
        redde _arbor_divergere(comparator, "lexema/nihil", nodus_a, nodus_b,
            a, b, locus, index);
    }

    si (a->genus != b->genus)
    {
        redde _arbor_divergere(comparator, "lexema/genus", nodus_a, nodus_b,
            a, b, locus, index);
    }
    si (   a->valor.mensura != b->valor.mensura
        || (a->valor.mensura > ZEPHYRUM
            && memcmp(a->valor.datum, b->valor.datum,
                   (size_t)a->valor.mensura) != ZEPHYRUM))
    {
        redde _arbor_divergere(comparator, "lexema/valor", nodus_a, nodus_b,
            a, b, locus, index);
    }
    si (a->standard != b->standard)
    {
        redde _arbor_divergere(comparator, "lexema/standard", nodus_a,
            nodus_b, a, b, locus, index);
    }
    si (a->fons_index != b->fons_index)
    {
        redde _arbor_divergere(comparator, "lexema/fons", nodus_a, nodus_b,
            a, b, locus, index);
    }
    si (a->initium_lineae != b->initium_lineae)
    {
        redde _arbor_divergere(comparator, "lexema/initium-lineae", nodus_a,
            nodus_b, a, b, locus, index);
    }

    /* PROVENIENTIA - utroque modo. Vide notam in capite: haec est
     * custodia quae aliter transiret quia subiectum eius abesset */
    si ((a->byte_offset < ZEPHYRUM) != (b->byte_offset < ZEPHYRUM))
    {
        redde _arbor_divergere(comparator, "lexema/provenientia", nodus_a,
            nodus_b, a, b, locus, index);
    }

    si (comparator->modus == SILVA_ARBOR_COMPARATIO_FIDELITAS)
    {
        si (a->byte_offset != b->byte_offset)
        {
            redde _arbor_divergere(comparator, "lexema/offset", nodus_a,
                nodus_b, a, b, locus, index);
        }
        si (a->linea != b->linea)
        {
            redde _arbor_divergere(comparator, "lexema/linea", nodus_a,
                nodus_b, a, b, locus, index);
        }
        si (a->columna != b->columna)
        {
            redde _arbor_divergere(comparator, "lexema/columna", nodus_a,
                nodus_b, a, b, locus, index);
        }
        si (a->longitudo != b->longitudo)
        {
            redde _arbor_divergere(comparator, "lexema/longitudo", nodus_a,
                nodus_b, a, b, locus, index);
        }
    }

    si (!_arbor_trivia_aequalia(comparator, a->spatia_ante, b->spatia_ante,
             "trivia/ante", nodus_a, nodus_b, locus))
    {
        redde FALSUM;
    }
    si (!_arbor_trivia_aequalia(comparator, a->spatia_post, b->spatia_post,
             "trivia/post", nodus_a, nodus_b, locus))
    {
        redde FALSUM;
    }
    redde _arbor_scissurae_aequales(comparator, a, b, nodus_a, nodus_b,
        locus);
}


/* ==================================================
 * Nodi et valores
 * ================================================== */

interior b32
_arbor_nodi_aequales (
    ArborComparator*,
    constans SilvaNodus*,
    constans SilvaNodus*);

interior b32
_arbor_valores_aequales (
      ArborComparator* comparator,
           SilvaValor  a,
           SilvaValor  b,
  constans SilvaNodus* nodus_a,
  constans SilvaNodus* nodus_b,
                  s32  locus)
{
    i32 numerus_a;
    i32 numerus_b;
    i32 i;

    si (a.genus != b.genus)
    {
        redde _arbor_divergere(comparator, "locus/genus-valoris", nodus_a,
            nodus_b, NIHIL, NIHIL, locus, -I);
    }

    commutatio (a.genus)
    {
        casus SILVA_VALOR_NIHIL:
            redde VERUM;

        casus SILVA_VALOR_NODUS:
            redde _arbor_nodi_aequales(comparator, a.datum.nodus,
                b.datum.nodus);

        casus SILVA_VALOR_TOKEN:
            redde _arbor_lexemata_aequalia(comparator, a.datum.token,
                b.datum.token, nodus_a, nodus_b, locus, -I);

        casus SILVA_VALOR_INDEX:
            si (a.datum.index != b.datum.index)
            {
                redde _arbor_divergere(comparator, "locus/index", nodus_a,
                    nodus_b, NIHIL, NIHIL, locus, -I);
            }
            redde VERUM;

        casus SILVA_VALOR_LISTA:
            /* Mensura PROSPECTUS, numquam xar_numerus repositorii:
             * repositorium inter furcas GLR commune est */
            numerus_a = silva_valor_lista_numerus(a);
            numerus_b = silva_valor_lista_numerus(b);
            si (numerus_a != numerus_b)
            {
                redde _arbor_divergere(comparator, "lista/numerus", nodus_a,
                    nodus_b, NIHIL, NIHIL, locus, -I);
            }
            per (i = ZEPHYRUM; i < numerus_a; i++)
            {
                SilvaValor* elementum_a =
                    silva_valor_lista_obtinere(a, i);
                SilvaValor* elementum_b =
                    silva_valor_lista_obtinere(b, i);

                si (elementum_a == NIHIL || elementum_b == NIHIL)
                {
                    redde _arbor_divergere(comparator, "lista/elementum-nihil",
                        nodus_a, nodus_b, NIHIL, NIHIL, locus, (s32)i);
                }
                si (elementum_a->genus != elementum_b->genus)
                {
                    redde _arbor_divergere(comparator, "lista/genus-elementi",
                        nodus_a, nodus_b, NIHIL, NIHIL, locus, (s32)i);
                }
                si (elementum_a->genus == SILVA_VALOR_NODUS)
                {
                    si (!_arbor_nodi_aequales(comparator,
                             elementum_a->datum.nodus,
                             elementum_b->datum.nodus))
                    {
                        redde FALSUM;
                    }
                }
                alioquin si (elementum_a->genus == SILVA_VALOR_TOKEN)
                {
                    si (!_arbor_lexemata_aequalia(comparator,
                             elementum_a->datum.token,
                             elementum_b->datum.token,
                             nodus_a, nodus_b, locus, (s32)i))
                    {
                        redde FALSUM;
                    }
                }
                alioquin
                {
                    si (!_arbor_valores_aequales(comparator, *elementum_a,
                             *elementum_b, nodus_a, nodus_b, locus))
                    {
                        redde FALSUM;
                    }
                }
            }
            redde VERUM;

        ordinarius:
            redde _arbor_divergere(comparator, "locus/genus-ignotum",
                nodus_a, nodus_b, NIHIL, NIHIL, locus, -I);
    }
}

interior b32
_arbor_nodi_aequales (
     ArborComparator* comparator,
 constans SilvaNodus* a,
 constans SilvaNodus* b)
{
    i32 i;
    i32 via_prior;
    b32 fructus;

    si (a == NIHIL || b == NIHIL)
    {
        si (a == b)
        {
            redde VERUM;
        }
        redde _arbor_divergere(comparator, "nodus/nihil", a, b, NIHIL,
            NIHIL, -I, -I);
    }

    si (a->genus != b->genus)
    {
        redde _arbor_divergere(comparator, "nodus/genus", a, b, NIHIL,
            NIHIL, -I, -I);
    }
    si (a->numerus_locorum != b->numerus_locorum)
    {
        redde _arbor_divergere(comparator, "nodus/numerus-locorum", a, b,
            NIHIL, NIHIL, -I, -I);
    }

    /* PATER: nullitas sola, et INTERIORIBUS solis. Parentela
     * RADICUM comparationis EXTRA comparationem iacet: subarbor in
     * arbore maiore electa patrem habet, arbor eadem seorsum lecta
     * habere non potest. Radices conferre CIX divergentias falsas
     * super corpus dedit - artificium comparationis, non defectus
     * lectoris. */
    si (   comparator->profunditas > ZEPHYRUM
        && (a->pater == NIHIL) != (b->pater == NIHIL))
    {
        redde _arbor_divergere(comparator, "nodus/pater-nullitas", a, b,
            NIHIL, NIHIL, -I, -I);
    }

    fructus = VERUM;
    per (i = ZEPHYRUM; i < a->numerus_locorum && fructus; i++)
    {
        via_prior = comparator->via_longitudo;
        _arbor_via_premere(comparator, a->genus, i);
        comparator->profunditas++;
        fructus = _arbor_valores_aequales(comparator, a->loci[i], b->loci[i],
            a, b, (s32)i);
        comparator->profunditas--;
        _arbor_via_restituere(comparator, via_prior);
    }
    redde fructus;
}

b32
silva_arbor_aequalis (
          constans SilvaNodus* a,
          constans SilvaNodus* b,
    SilvaArborComparatioModus  modus,
        SilvaArborDifferentia* differentia)
{
    ArborComparator comparator;

    si (differentia != NIHIL)
    {
        differentia->campus    = NIHIL;
        differentia->nodus_a   = NIHIL;
        differentia->nodus_b   = NIHIL;
        differentia->lexema_a  = NIHIL;
        differentia->lexema_b  = NIHIL;
        differentia->locus     = -I;
        differentia->index     = -I;
        differentia->via[0]    = '\0';
    }

    comparator.modus          = modus;
    comparator.differentia    = differentia;
    comparator.via_longitudo  = ZEPHYRUM;
    comparator.profunditas    = ZEPHYRUM;
    comparator.via[0]         = '\0';

    redde _arbor_nodi_aequales(&comparator, a, b);
}
