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
    v.datum.lista.xar = lista;
    v.datum.lista.mensura = xar_numerus(lista);
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

SilvaValor
silva_valor_lista_appendere (
    Piscina*   piscina,
    SilvaValor lista,
    SilvaValor elementum)
{
    Xar*        xar;
    SilvaValor* slot;

    si (lista.genus != SILVA_VALOR_LISTA || lista.datum.lista.xar == NIHIL)
    {
        redde silva_valor_nihil();
    }
    xar = lista.datum.lista.xar;

    si (xar_numerus(xar) != lista.datum.lista.mensura)
    {
        /* Divergentia: furca alia ultra prospectum meum scripsit.
         * Praefixum meum in repositorium recens copiatur - prospectus
         * eius intactus manet (numerus repositorii = numerator
         * versionum gratuitus). */
        Xar* recens = xar_creare(piscina, (i32)magnitudo(SilvaValor));
        i32  i;

        si (recens == NIHIL)
        {
            redde silva_valor_nihil();
        }
        per (i = ZEPHYRUM; i < lista.datum.lista.mensura; i++)
        {
            SilvaValor* fons = (SilvaValor*)xar_obtinere(xar, i);
            SilvaValor* dest = (SilvaValor*)xar_addere(recens);

            si (fons == NIHIL || dest == NIHIL)
            {
                redde silva_valor_nihil();
            }
            *dest = *fons;
        }
        xar = recens;
    }

    slot = (SilvaValor*)xar_addere(xar);
    si (slot == NIHIL)
    {
        redde silva_valor_nihil();
    }
    *slot = elementum;

    {
        SilvaValor novus;

        novus.genus = SILVA_VALOR_LISTA;
        novus.datum.lista.xar = xar;
        novus.datum.lista.mensura = lista.datum.lista.mensura + I;
        redde novus;
    }
}

i32
silva_valor_lista_numerus (SilvaValor lista)
{
    si (lista.genus != SILVA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    redde lista.datum.lista.mensura;
}

SilvaValor*
silva_valor_lista_obtinere (SilvaValor lista, i32 index)
{
    si (lista.genus != SILVA_VALOR_LISTA || lista.datum.lista.xar == NIHIL
        || index >= lista.datum.lista.mensura)
    {
        redde NIHIL;
    }
    redde (SilvaValor*)xar_obtinere(lista.datum.lista.xar, index);
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
    SilvaValor novus;

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
    /* Appendere purum + religatio loci (nodus sub constructione
     * dominus unicus locorum suorum est) */
    novus = silva_valor_lista_appendere(piscina, nodus->loci[locus],
        valor);
    si (novus.genus != SILVA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    nodus->loci[locus] = novus;
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
            && v->datum.lista.xar != NIHIL)
        {
            i32 j;

            /* Mensura prospectus, non numerus repositorii (A½) */
            per (j = ZEPHYRUM; j < v->datum.lista.mensura; j++)
            {
                SilvaValor* e = (SilvaValor*)xar_obtinere(
                    v->datum.lista.xar, j);

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
