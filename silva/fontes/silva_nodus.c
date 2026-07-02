/* silva_nodus.c - Nodus uniformis + valor signatus */

#include "silva_nodus.h"
#include <stdio.h>

/* ==================================================
 * Constructores valorum
 * ================================================== */

SilvaValor
silva_valor_nihil (vacuum)
{
    SilvaValor v;

    v.genus = SILVA_VALOR_NIHIL;
    v.datum.nodus = NIHIL;
    redde v;
}

SilvaValor
silva_valor_nodus (SilvaNodus* nodus)
{
    SilvaValor v;

    v.genus = SILVA_VALOR_NODUS;
    v.datum.nodus = nodus;
    redde v;
}

SilvaValor
silva_valor_token (SilvaToken* token)
{
    SilvaValor v;

    v.genus = SILVA_VALOR_TOKEN;
    v.datum.token = token;
    redde v;
}

SilvaValor
silva_valor_lista (Xar* lista)
{
    SilvaValor v;

    v.genus = SILVA_VALOR_LISTA;
    v.datum.lista = lista;
    redde v;
}

SilvaValor
silva_valor_index (s32 index)
{
    SilvaValor v;

    v.genus = SILVA_VALOR_INDEX;
    v.datum.index = index;
    redde v;
}

SilvaValor
silva_valor_lista_nova (Piscina* piscina)
{
    redde silva_valor_lista(
        xar_creare(piscina, magnitudo(SilvaValor)));
}


/* ==================================================
 * Nodus
 * ================================================== */

SilvaNodus*
silva_nodus_creare (
    Piscina* piscina,
    s32      genus,
    i32      numerus_locorum)
{
    SilvaNodus* nodus;
    i32 i;

    nodus = (SilvaNodus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaNodus));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    nodus->genus = genus;
    nodus->numerus_locorum = numerus_locorum;
    nodus->pater = NIHIL;
    nodus->loci = NIHIL;

    si (numerus_locorum > ZEPHYRUM)
    {
        nodus->loci = (SilvaValor*)piscina_allocare(piscina,
            (memoriae_index)(magnitudo(SilvaValor)
                * (memoriae_index)numerus_locorum));
        si (nodus->loci == NIHIL)
        {
            redde NIHIL;
        }
        per (i = ZEPHYRUM; i < numerus_locorum; i++)
        {
            nodus->loci[i] = silva_valor_nihil();
        }
    }
    redde nodus;
}

b32
silva_valor_congruit (
    SilvaValor        valor,
    SilvaLocusSpecies species)
{
    commutatio (species)
    {
        casus SILVA_LOCUS_NODUS:
            redde (valor.genus == SILVA_VALOR_NODUS) ? VERUM : FALSUM;
        casus SILVA_LOCUS_TOKEN:
            redde (valor.genus == SILVA_VALOR_TOKEN) ? VERUM : FALSUM;
        casus SILVA_LOCUS_LISTA_NODUS:
        casus SILVA_LOCUS_LISTA_TOKEN:
        casus SILVA_LOCUS_LISTA_MIXTA:
            redde (valor.genus == SILVA_VALOR_LISTA) ? VERUM : FALSUM;
        casus SILVA_LOCUS_INDEX:
            redde (valor.genus == SILVA_VALOR_INDEX) ? VERUM : FALSUM;
        ordinarius:
            redde FALSUM;
    }
}

b32
silva_nodus_ponere (
    SilvaNodus*       nodus,
    i32               locus,
    SilvaValor        valor,
    SilvaLocusSpecies species)
{
    si (nodus == NIHIL || locus >= nodus->numerus_locorum)
    {
        fprintf(stderr, "silva_nodus: locus %d extra fines\n", (int)locus);
        redde FALSUM;
    }
    /* S32: signum contra speciem */
    si (!silva_valor_congruit(valor, species))
    {
        fprintf(stderr,
            "silva_nodus: signum valoris (%d) speciei loci (%d) non congruit\n",
            (int)valor.genus, (int)species);
        redde FALSUM;
    }
    /* S32: semel tantum scribere (dominus unicus) */
    si (nodus->loci[locus].genus != SILVA_VALOR_NIHIL)
    {
        fprintf(stderr,
            "silva_nodus: locus %d iam scriptus (dominus duplex)\n",
            (int)locus);
        redde FALSUM;
    }
    nodus->loci[locus] = valor;
    redde VERUM;
}

b32
silva_nodus_appendere (
    Piscina*          piscina,
    SilvaNodus*       nodus,
    i32               locus,
    SilvaValor        valor,
    SilvaLocusSpecies species)
{
    SilvaValor* elementum;

    si (nodus == NIHIL || locus >= nodus->numerus_locorum)
    {
        redde FALSUM;
    }
    si (species != SILVA_LOCUS_LISTA_NODUS
        && species != SILVA_LOCUS_LISTA_TOKEN
        && species != SILVA_LOCUS_LISTA_MIXTA)
    {
        fprintf(stderr,
            "silva_nodus: appendere in locum non-listam (species %d)\n",
            (int)species);
        redde FALSUM;
    }
    si (nodus->loci[locus].genus == SILVA_VALOR_NIHIL)
    {
        nodus->loci[locus] = silva_valor_lista_nova(piscina);
    }
    si (nodus->loci[locus].genus != SILVA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    elementum = (SilvaValor*)xar_addere(nodus->loci[locus].datum.lista);
    si (elementum == NIHIL)
    {
        redde FALSUM;
    }
    *elementum = valor;
    redde VERUM;
}

Xar*
silva_nodus_liberi (
    Piscina*             piscina,
    constans SilvaNodus* nodus)
{
    Xar* liberi;
    i32 i;

    liberi = xar_creare(piscina, magnitudo(SilvaNodus*));
    si (nodus == NIHIL)
    {
        redde liberi;
    }

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans SilvaValor* v = &nodus->loci[i];

        si (v->genus == SILVA_VALOR_NODUS && v->datum.nodus != NIHIL)
        {
            SilvaNodus** locus = (SilvaNodus**)xar_addere(liberi);

            si (locus != NIHIL)
            {
                *locus = v->datum.nodus;
            }
        }
        alioquin si (v->genus == SILVA_VALOR_LISTA
            && v->datum.lista != NIHIL)
        {
            i32 j;

            per (j = ZEPHYRUM; j < xar_numerus(v->datum.lista); j++)
            {
                SilvaValor* e = (SilvaValor*)xar_obtinere(
                    v->datum.lista, j);

                si (e != NIHIL && e->genus == SILVA_VALOR_NODUS
                    && e->datum.nodus != NIHIL)
                {
                    SilvaNodus** locus = (SilvaNodus**)xar_addere(liberi);

                    si (locus != NIHIL)
                    {
                        *locus = e->datum.nodus;
                    }
                }
            }
        }
    }
    redde liberi;
}
