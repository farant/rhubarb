/* silva_nodus.c - quaestiones originis C89 super nodos
 *
 * Nucleus (valores, constructio, liberi) in materia_nodus.c est
 * (silva-migratio T8); hic solae familiae quae catenam originis
 * per silva_token_radix ambulant. */

#include "silva_nodus.h"
#include <stdio.h>


/* ==================================================
 * Extensio fontis (LEGATUS chunk 0, ex sessione promota - motus
 * purus; ambulator contra API publicam scriptus erat)
 * ================================================== */

vacuum
silva_nodus_extensionem (
    constans SilvaNodus* n,
                    s32  fons_index,
                    s32* minimum,
                    s32* maximum)
{
    insignatus integer k;

    si (n == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        silva_valor_extensionem(n->loci[k], fons_index, minimum,
            maximum);
    }
}

vacuum
silva_valor_extensionem (
    SilvaValor  v,
           s32  fons_index,
           s32* minimum,
           s32* maximum)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_TOKEN:
            si (v.datum.token != NIHIL)
            {
                /* RADIX originis: lexemata expansa synthetica sunt
                 * (byte_offset -1) - sedes invocationis in fonte
                 * dato per catenam originis (macros latinae!) */
                SilvaToken* radix_t = silva_token_radix(
                    v.datum.token);

                si (radix_t == NIHIL)
                {
                    radix_t = v.datum.token;
                }
                si (   radix_t->fons_index  == fons_index
                    && radix_t->byte_offset >= ZEPHYRUM)
                {
                    s32 a = (s32)radix_t->byte_offset;
                    s32 b = a + (s32)silva_token_longitudo(radix_t);

                    si (*minimum < (s32)ZEPHYRUM || a < *minimum)
                    {
                        *minimum = a;
                    }
                    si (b > *maximum)
                    {
                        *maximum = b;
                    }
                }
            }
            frange;
        casus SILVA_VALOR_NODUS:
            silva_nodus_extensionem(v.datum.nodus, fons_index,
                minimum, maximum);
            frange;
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    silva_valor_extensionem(*elem, fons_index,
                        minimum, maximum);
                }
            }
            frange;
        }
        ordinarius:
            frange;
    }
}

interior vacuum
_extensionem_lineis_valoris (
    SilvaValor  v,
           s32  fons_index,
           i32* linea_a,
           i32* columna_a,
           i32* linea_b,
           i32* columna_b)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_TOKEN:
            si (v.datum.token != NIHIL)
            {
                SilvaToken* radix_t = silva_token_radix(
                    v.datum.token);

                si (radix_t == NIHIL)
                {
                    radix_t = v.datum.token;
                }
                si (   radix_t->fons_index  == fons_index
                    && radix_t->byte_offset >= ZEPHYRUM)
                {
                    i32 l   = radix_t->linea;
                    i32 c   = radix_t->columna;
                    i32 cf  = c + silva_token_longitudo(radix_t);

                    si (   *linea_a == ZEPHYRUM || l < *linea_a
                        || (l == *linea_a && c < *columna_a))
                    {
                        *linea_a    = l;
                        *columna_a  = c;
                    }
                    si (   *linea_b == ZEPHYRUM || l > *linea_b
                        || (l == *linea_b && cf > *columna_b))
                    {
                        *linea_b    = l;
                        *columna_b  = cf;
                    }
                }
            }
            frange;
        casus SILVA_VALOR_NODUS:
        {
            constans SilvaNodus* n = v.datum.nodus;
             insignatus integer  k;

            si (n != NIHIL)
            {
                per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
                {
                    _extensionem_lineis_valoris(n->loci[k],
                        fons_index, linea_a, columna_a, linea_b,
                        columna_b);
                }
            }
            frange;
        }
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    _extensionem_lineis_valoris(*elem, fons_index,
                        linea_a, columna_a, linea_b, columna_b);
                }
            }
            frange;
        }
        ordinarius:
            frange;
    }
}

vacuum
silva_nodus_extensionem_lineis (
    constans SilvaNodus* n,
                    s32  fons_index,
                    i32* linea_a,
                    i32* columna_a,
                    i32* linea_b,
                    i32* columna_b)
{
    insignatus integer k;

    *linea_a    = ZEPHYRUM;
    *columna_a  = ZEPHYRUM;
    *linea_b    = ZEPHYRUM;
    *columna_b  = ZEPHYRUM;
    si (n == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        _extensionem_lineis_valoris(n->loci[k], fons_index, linea_a,
            columna_a, linea_b, columna_b);
    }
}


/* ==================================================
 * Puritas fontis + geometria fida - quaestiones
 * provenientiae (comites honestatis extensionis:
 * extensio geometriam refert, hae utrum credenda sit)
 * ================================================== */

b32
silva_valor_est_fons_purus (
    SilvaValor v,
           s32 fons_index)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_TOKEN:
            si (v.datum.token != NIHIL)
            {
                si (!silva_token_est_fons(v.datum.token))
                {
                    redde FALSUM;
                }
                si (   fons_index                >= (s32)ZEPHYRUM
                    && v.datum.token->fons_index != fons_index)
                {
                    redde FALSUM;
                }
            }
            frange;
        casus SILVA_VALOR_NODUS:
            redde silva_nodus_est_fons_purus(v.datum.nodus,
                fons_index);
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (   elem != NIHIL
                    && !silva_valor_est_fons_purus(*elem,
                           fons_index))
                {
                    redde FALSUM;
                }
            }
            frange;
        }
        ordinarius:
            frange;
    }
    redde VERUM;
}

b32
silva_nodus_est_fons_purus (
    constans SilvaNodus* n,
                    s32  fons_index)
{
    insignatus integer k;

    si (n == NIHIL)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        si (!silva_valor_est_fons_purus(n->loci[k], fons_index))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* sedes radicis collecta - clavis collapsus (fons, byte) +
 * identitas lexematis arboris (idem bis visum = communicatio
 * ambigua, non collapsus) */
nomen structura {
           s32  fons;
           s32  byte;
    SilvaToken* token;
} SedesRadicis;

interior s32
_sedes_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans SedesRadicis* sa = (constans SedesRadicis*)a;
    constans SedesRadicis* sb = (constans SedesRadicis*)b;

    si (sa->fons != sb->fons)
    {
        redde (sa->fons < sb->fons) ? -I : I;
    }
    si (sa->byte != sb->byte)
    {
        redde (sa->byte < sb->byte) ? -I : I;
    }
    redde ZEPHYRUM;
}

/* FALSUM = sedes inscibilis inventa (radix synthetica) aut
 * defectus memoriae - fides statim negatur */
interior b32
_sedes_colligere (
    SilvaValor  v,
           s32  fons_index,
           Xar* sedes)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_TOKEN:
            si (v.datum.token != NIHIL)
            {
                SilvaToken* radix_t = silva_token_radix(
                    v.datum.token);

                si (radix_t == NIHIL)
                {
                    radix_t = v.datum.token;
                }
                si (radix_t->byte_offset < (s32)ZEPHYRUM)
                {
                    redde FALSUM;
                }
                si (   fons_index < (s32)ZEPHYRUM
                    || radix_t->fons_index == fons_index)
                {
                    SedesRadicis* addita =
                        (SedesRadicis*)xar_addere(sedes);

                    si (addita == NIHIL)
                    {
                        redde FALSUM;
                    }
                    addita->fons   = radix_t->fons_index;
                    addita->byte   = radix_t->byte_offset;
                    addita->token  = v.datum.token;
                }
            }
            frange;
        casus SILVA_VALOR_NODUS:
        {
            constans SilvaNodus* n = v.datum.nodus;
             insignatus integer  k;

            si (n != NIHIL)
            {
                per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
                {
                    si (!_sedes_colligere(n->loci[k], fons_index,
                            sedes))
                    {
                        redde FALSUM;
                    }
                }
            }
            frange;
        }
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (   elem != NIHIL
                    && !_sedes_colligere(*elem, fons_index, sedes))
                {
                    redde FALSUM;
                }
            }
            frange;
        }
        ordinarius:
            frange;
    }
    redde VERUM;
}

/* sedes ordinatae percurrere: sedes eadem a lexematis DISTINCTIS
 * vindicata = collapsus (lexema idem bis = communicatio ambigua) */
interior b32
_sedes_examinare (
    Xar* sedes)
{
    i32 numerus;
    i32 k;

    xar_ordinare(sedes, _sedes_comparare);
    numerus = xar_numerus(sedes);
    per (k = I; k < numerus; k++)
    {
        constans SedesRadicis* prior =
            (constans SedesRadicis*)xar_obtinere(sedes, k - I);
        constans SedesRadicis* currens =
            (constans SedesRadicis*)xar_obtinere(sedes, k);

        si (   prior->fons  == currens->fons
            && prior->byte  == currens->byte
            && prior->token != currens->token)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
silva_valor_geometria_fida (
       Piscina* piscina,
    SilvaValor  v,
           s32  fons_index)
{
    Xar* sedes;

    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    sedes = xar_creare(piscina, (i32)magnitudo(SedesRadicis));
    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    si (!_sedes_colligere(v, fons_index, sedes))
    {
        redde FALSUM;
    }
    redde _sedes_examinare(sedes);
}

b32
silva_nodus_geometria_fida (
                Piscina* piscina,
    constans SilvaNodus* n,
                    s32  fons_index)
{
                   Xar* sedes;
    insignatus integer  k;

    si (n == NIHIL)
    {
        redde VERUM;
    }
    si (piscina == NIHIL)
    {
        redde FALSUM;
    }
    sedes = xar_creare(piscina, (i32)magnitudo(SedesRadicis));
    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        si (!_sedes_colligere(n->loci[k], fons_index, sedes))
        {
            redde FALSUM;
        }
    }
    redde _sedes_examinare(sedes);
}


/* ==================================================
 * commentarium ducens - consumptor PRIMUS pinnae
 * "commenta sunt contenta" (VISIO; INTENTIO in
 * silva/phase-log.md 2026-07-14)
 * ================================================== */

/* lexema primum nodi in fonte dato (minimum byte_offset, RADICE
 * originis soluto - declarationes latinae macris incipiunt:
 * lexema expansum syntheticum est, invocatio trivia fert) */
interior vacuum
_lexema_primum_valoris (
    SilvaValor   v,
           s32   fons_index,
    SilvaToken** primum,
           s32*  minimum)
{
    commutatio (v.genus)
    {
        casus SILVA_VALOR_TOKEN:
            si (v.datum.token != NIHIL)
            {
                SilvaToken* radix_t = silva_token_radix(
                    v.datum.token);

                si (radix_t == NIHIL)
                {
                    radix_t = v.datum.token;
                }
                si (   radix_t->fons_index  == fons_index
                    && radix_t->byte_offset >= ZEPHYRUM
                    && (*minimum < (s32)ZEPHYRUM
                        || (s32)radix_t->byte_offset < *minimum))
                {
                    *minimum  = (s32)radix_t->byte_offset;
                    *primum   = radix_t;
                }
            }
            frange;
        casus SILVA_VALOR_NODUS:
            si (v.datum.nodus != NIHIL)
            {
                insignatus integer k;

                per (k = ZEPHYRUM;
                    k < v.datum.nodus->numerus_locorum; k++)
                {
                    _lexema_primum_valoris(v.datum.nodus->loci[k],
                        fons_index, primum, minimum);
                }
            }
            frange;
        casus SILVA_VALOR_LISTA:
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer k;

            per (k = ZEPHYRUM; k < m; k++)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(v, k);

                si (elem != NIHIL)
                {
                    _lexema_primum_valoris(*elem, fons_index,
                        primum, minimum);
                }
            }
            frange;
        }
        ordinarius:
            frange;
    }
}

integer
silva_commentarium_ducens (
       constans SilvaNodus* n,
                       s32  fons_index,
    SilvaCommentariumVista* vista)
{
            SilvaToken* primum   = NIHIL;
                   s32  minimum  = (s32)-I;
    insignatus integer  k;
                   s32  nl;
                   s32  initium;
                   s32  finis;
    insignatus integer  linea;

    si (vista == NIHIL)
    {
        redde ZEPHYRUM;
    }
    vista->initium  = (s32)-I;
    vista->finis    = (s32)-I;
    vista->linea    = ZEPHYRUM;
    si (n == NIHIL)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        _lexema_primum_valoris(n->loci[k], fons_index, &primum,
            &minimum);
    }
    si (primum == NIHIL || silva_token_ante_numerus(primum) == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    /* regula arcte-supra, ambulatio RETRO: spatia_ante = trivia
     * sui-lineae SOLA (politica divisionis lexatoris - commenta
     * finis-lineae prioris in spatia_post eius, exclusa
     * constructione); linea vacua = II NOVA_LINEA consecutivae
     * (SPATIA/TABULAE neglectis) -> bloccus finitur; commentum
     * numerum linearum novarum retexit (commenta multi-lineae
     * bloccus unus manent) */
    nl       = ZEPHYRUM;
    initium  = (s32)-I;
    finis    = (s32)-I;
    linea    = ZEPHYRUM;
    k        = silva_token_ante_numerus(primum);
    dum (k > ZEPHYRUM)
    {
        SilvaToken* t;

        k--;
        t = silva_token_ante(primum, (i32)k);
        si (t == NIHIL)
        {
            perge;
        }
        si (t->genus == SILVA_LEX_NOVA_LINEA)
        {
            nl++;
            si (nl >= (s32)II)
            {
                frange;
            }
            perge;
        }
        si (   t->genus == SILVA_LEX_SPATIA
            || t->genus == SILVA_LEX_TABULAE
            || t->genus == SILVA_LEX_CONTINUATIO)
        {
            perge;
        }
        si (   (t->genus == SILVA_LEX_COMMENTUM_CLAUSUM
                || t->genus == SILVA_LEX_COMMENTUM_LINEA)
            && t->byte_offset >= ZEPHYRUM)
        {
            si (finis < ZEPHYRUM)
            {
                finis = (s32)t->byte_offset
                    + (s32)silva_token_longitudo(t);
            }
            initium  = (s32)t->byte_offset;
            linea    = t->linea;
            nl       = ZEPHYRUM;
            perge;
        }
        frange;   /* genus inopinatum - tutela */
    }
    si (initium < ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    vista->initium  = initium;
    vista->finis    = finis;
    vista->linea    = linea;
    redde I;
}
