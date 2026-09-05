/* materia_nodus.c - Nodus uniformis + valor signatus
 *
 * Vide materia_nodus.h; portatum ex silva_nodus.c cum semantica
 * eadem. Duo sola mutata:
 *   - genus lexematis s32 est, non enumeratio C89 (materia_lexicon)
 *   - sedes efficax per UNCUM quaeritur, non per catenam originis
 *     (vide caput: quinque familiae quaestionum eam poscunt)
 */

#include "materia_nodus.h"

#include <stdio.h>


/* ==================================================
 * Sedes
 * ================================================== */

vacuum
materia_sedes_tokeni (
    constans MateriaOrigoUncus* uncus,
         constans MateriaToken* token,
                  MateriaSedes* sedes)
{
    si (sedes == NIHIL)
    {
        redde;
    }
    si (token == NIHIL)
    {
        sedes->byte_offset  = (s32)-I;
        sedes->linea        = ZEPHYRUM;
        sedes->columna      = ZEPHYRUM;
        sedes->fons_index   = (s32)-I;
        sedes->est_fons     = FALSUM;
        redde;
    }

    si (uncus != NIHIL && uncus->sedes_quaerere != NIHIL)
    {
        uncus->sedes_quaerere(uncus->datum, token, sedes);
        redde;
    }

    /* Nulla derivatio: lexema origo sua est. Hoc pro CSS et HTML
     * RECTUM est, non degradatio. */
    sedes->byte_offset  = token->byte_offset;
    sedes->linea        = token->linea;
    sedes->columna      = token->columna;
    sedes->fons_index   = token->fons_index;
    sedes->est_fons     = VERUM;
}


/* ==================================================
 * Constructores valorum
 * ================================================== */

MateriaValor
materia_valor_nihil (vacuum)
{
    MateriaValor v;

    v.genus        = MATERIA_VALOR_NIHIL;
    v.datum.nodus  = NIHIL;
    redde v;
}

MateriaValor
materia_valor_nodus (
    MateriaNodus* nodus)
{
    MateriaValor v;

    v.genus        = MATERIA_VALOR_NODUS;
    v.datum.nodus  = nodus;
    redde v;
}

MateriaValor
materia_valor_token (
    MateriaToken* token)
{
    MateriaValor v;

    v.genus        = MATERIA_VALOR_TOKEN;
    v.datum.token  = token;
    redde v;
}

MateriaValor
materia_valor_index (
    s32 index)
{
    MateriaValor v;

    v.genus        = MATERIA_VALOR_INDEX;
    v.datum.index  = index;
    redde v;
}

MateriaValor
materia_valor_lista (
    Xar* lista)
{
    MateriaValor v;

    si (lista == NIHIL)
    {
        redde materia_valor_nihil();
    }
    v.genus                = MATERIA_VALOR_LISTA;
    v.datum.lista.xar      = lista;
    v.datum.lista.mensura  = xar_numerus(lista);
    redde v;
}

MateriaValor
materia_valor_lista_nova (
    Piscina* piscina)
{
    redde materia_valor_lista(
        xar_creare(piscina, magnitudo(MateriaValor)));
}

MateriaValor
materia_valor_lista_appendere (
         Piscina* piscina,
    MateriaValor  lista,
    MateriaValor  elementum)
{
             Xar* xar;
    MateriaValor* slot;

    si (   lista.genus           != MATERIA_VALOR_LISTA
        || lista.datum.lista.xar == NIHIL)
    {
        redde materia_valor_nihil();
    }
    xar = lista.datum.lista.xar;

    si (xar_numerus(xar) != lista.datum.lista.mensura)
    {
        /* Divergentia: furca alia ultra prospectum meum scripsit.
         * Praefixum meum in repositorium recens copiatur -
         * prospectus eius intactus manet. */
        Xar* recens = xar_creare(piscina, (i32)magnitudo(MateriaValor));
        i32  i;

        si (recens == NIHIL)
        {
            redde materia_valor_nihil();
        }
        per (i = ZEPHYRUM; i < lista.datum.lista.mensura; i++)
        {
            MateriaValor* fons = (MateriaValor*)xar_obtinere(xar, i);
            MateriaValor* dest = (MateriaValor*)xar_addere(recens);

            si (fons == NIHIL || dest == NIHIL)
            {
                redde materia_valor_nihil();
            }
            *dest = *fons;
        }
        xar = recens;
    }

    slot = (MateriaValor*)xar_addere(xar);
    si (slot == NIHIL)
    {
        redde materia_valor_nihil();
    }
    *slot = elementum;

    {
        MateriaValor novus;

        novus.genus                = MATERIA_VALOR_LISTA;
        novus.datum.lista.xar      = xar;
        novus.datum.lista.mensura  = lista.datum.lista.mensura + I;
        redde novus;
    }
}

i32
materia_valor_lista_numerus (
    MateriaValor lista)
{
    si (lista.genus != MATERIA_VALOR_LISTA)
    {
        redde ZEPHYRUM;
    }
    redde lista.datum.lista.mensura;
}

MateriaValor*
materia_valor_lista_obtinere (
    MateriaValor lista,
             i32 index)
{
    si (   lista.genus           != MATERIA_VALOR_LISTA
        || lista.datum.lista.xar == NIHIL
        || index                 >= lista.datum.lista.mensura)
    {
        redde NIHIL;
    }
    redde (MateriaValor*)xar_obtinere(lista.datum.lista.xar, index);
}


/* ==================================================
 * Nodus
 * ================================================== */

MateriaNodus*
materia_nodus_creare (
     Piscina* piscina,
         s32  genus,
         i32  numerus_locorum)
{
    MateriaNodus* nodus;
             i32  i;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }

    nodus = (MateriaNodus*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(MateriaNodus),
        (memoriae_index)magnitudo(vacuum*));
    si (nodus == NIHIL)
    {
        redde NIHIL;
    }

    nodus->genus            = genus;
    nodus->numerus_locorum  = numerus_locorum;
    nodus->pater            = NIHIL;
    nodus->loci             = NIHIL;

    si (numerus_locorum > ZEPHYRUM)
    {
        nodus->loci = (MateriaValor*)piscina_allocare_ordinatum(piscina,
            (memoriae_index)numerus_locorum
                * (memoriae_index)magnitudo(MateriaValor),
            (memoriae_index)magnitudo(vacuum*));
        si (nodus->loci == NIHIL)
        {
            redde NIHIL;
        }
        per (i = ZEPHYRUM; i < numerus_locorum; i++)
        {
            nodus->loci[i] = materia_valor_nihil();
        }
    }
    redde nodus;
}

b32
materia_valor_congruit (
         MateriaValor valor,
  MateriaLocusSpecies species)
{
    commutatio (species)
    {
        casus MATERIA_LOCUS_NODUS:
            redde (b32)(valor.genus == MATERIA_VALOR_NODUS);
        casus MATERIA_LOCUS_TOKEN:
            redde (b32)(valor.genus == MATERIA_VALOR_TOKEN);
        casus MATERIA_LOCUS_LISTA_NODUS:
        casus MATERIA_LOCUS_LISTA_TOKEN:
        casus MATERIA_LOCUS_LISTA_MIXTA:
            redde (b32)(valor.genus == MATERIA_VALOR_LISTA);
        casus MATERIA_LOCUS_INDEX:
            redde (b32)(valor.genus == MATERIA_VALOR_INDEX);
        ordinarius:
            redde FALSUM;
    }
}

b32
materia_nodus_ponere (
         MateriaNodus* nodus,
                  i32  locus,
         MateriaValor  valor,
  MateriaLocusSpecies  species)
{
    si (nodus == NIHIL || locus >= nodus->numerus_locorum)
    {
        fprintf(stderr, "materia_nodus: locus %d extra fines\n",
            (int)locus);
        redde FALSUM;
    }
    si (!materia_valor_congruit(valor, species))
    {
        fprintf(stderr,
            "materia_nodus: signum valoris (%d) speciei loci (%d) non congruit\n",
            (int)valor.genus, (int)species);
        redde FALSUM;
    }
    /* Semel tantum scribere - dominus unicus. Error hic apparet,
     * non ut arbor corrupta tribus operibus post. */
    si (nodus->loci[locus].genus != MATERIA_VALOR_NIHIL)
    {
        fprintf(stderr,
            "materia_nodus: locus %d iam scriptus (dominus duplex)\n",
            (int)locus);
        redde FALSUM;
    }
    nodus->loci[locus] = valor;
    redde VERUM;
}

b32
materia_nodus_lista_permutare (
              Piscina* piscina,
         MateriaNodus* nodus,
                  i32  locus,
         constans i32* ordo,
                  i32  n)
{
    constans MateriaValor* vetus;
             MateriaValor  nova;
                      b32* visum;
                      i32  k;

    si (   nodus == NIHIL || locus >= nodus->numerus_locorum
        || ordo  == NIHIL)
    {
        fprintf(stderr,
            "materia_nodus: permutare - locus %d extra fines\n",
            (int)locus);
        redde FALSUM;
    }
    vetus = &nodus->loci[locus];
    si (   vetus->genus                        != MATERIA_VALOR_LISTA
        || materia_valor_lista_numerus(*vetus) != n)
    {
        fprintf(stderr,
            "materia_nodus: permutare - locus %d non lista"
            " mensurae %d\n",
            (int)locus, (int)n);
        redde FALSUM;
    }
    visum = (b32*)piscina_allocare(piscina, (memoriae_index)(n
        > ZEPHYRUM
        ? n : I) * (memoriae_index)magnitudo(b32));
    si (visum == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        visum[k] = FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (ordo[k] >= n || visum[ordo[k]])
        {
            fprintf(stderr,
                "materia_nodus: permutare - ordo non permutatio"
                " (index %d)\n",
                (int)ordo[k]);
            redde FALSUM;
        }
        visum[ordo[k]] = VERUM;
    }
    nova = materia_valor_lista_nova(piscina);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(*vetus,
            ordo[k]);

        si (e == NIHIL)
        {
            redde FALSUM;
        }
        nova = materia_valor_lista_appendere(piscina, nova, *e);
        si (nova.genus != MATERIA_VALOR_LISTA)
        {
            redde FALSUM;
        }
    }
    nodus->loci[locus] = nova;
    redde VERUM;
}

b32
materia_nodus_reponere (
         MateriaNodus* nodus,
                  i32  locus,
         MateriaValor  valor,
  MateriaLocusSpecies  species)
{
    si (nodus == NIHIL || locus >= nodus->numerus_locorum)
    {
        fprintf(stderr,
            "materia_nodus: reponere - locus %d extra fines\n",
            (int)locus);
        redde FALSUM;
    }
    si (!materia_valor_congruit(valor, species))
    {
        fprintf(stderr,
            "materia_nodus: reponere - signum valoris (%d)"
            " speciei loci (%d) non congruit\n",
            (int)valor.genus, (int)species);
        redde FALSUM;
    }
    si (nodus->loci[locus].genus == MATERIA_VALOR_NIHIL)
    {
        fprintf(stderr,
            "materia_nodus: reponere - locus %d nondum scriptus"
            " (ponere)\n",
            (int)locus);
        redde FALSUM;
    }
    nodus->loci[locus] = valor;
    redde VERUM;
}

b32
materia_nodus_appendere (
              Piscina* piscina,
         MateriaNodus* nodus,
                  i32  locus,
         MateriaValor  valor,
  MateriaLocusSpecies  species)
{
    MateriaValor novus;

    si (nodus == NIHIL || locus >= nodus->numerus_locorum)
    {
        redde FALSUM;
    }
    si (   species != MATERIA_LOCUS_LISTA_NODUS
        && species != MATERIA_LOCUS_LISTA_TOKEN
        && species != MATERIA_LOCUS_LISTA_MIXTA)
    {
        fprintf(stderr,
            "materia_nodus: appendere in locum non-listam (species %d)\n",
            (int)species);
        redde FALSUM;
    }
    si (nodus->loci[locus].genus == MATERIA_VALOR_NIHIL)
    {
        nodus->loci[locus] = materia_valor_lista_nova(piscina);
    }
    si (nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    novus = materia_valor_lista_appendere(piscina, nodus->loci[locus],
        valor);
    si (novus.genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    nodus->loci[locus] = novus;
    redde VERUM;
}

Xar*
materia_nodus_liberi (
                  Piscina* piscina,
    constans MateriaNodus* nodus)
{
    Xar* liberi;
    i32  i;

    liberi = xar_creare(piscina, magnitudo(MateriaNodus*));
    si (nodus == NIHIL)
    {
        redde liberi;
    }

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaValor* v = &nodus->loci[i];

        si (v->genus == MATERIA_VALOR_NODUS && v->datum.nodus != NIHIL)
        {
            MateriaNodus** locus = (MateriaNodus**)xar_addere(liberi);

            si (locus != NIHIL)
            {
                *locus = v->datum.nodus;
            }
        }
        alioquin si (   v->genus           == MATERIA_VALOR_LISTA
                     && v->datum.lista.xar != NIHIL)
        {
            i32 j;

            /* Mensura PROSPECTUS, non numerus repositorii */
            per (j = ZEPHYRUM; j < v->datum.lista.mensura; j++)
            {
                MateriaValor* e = (MateriaValor*)xar_obtinere(
                    v->datum.lista.xar, j);

                si (   e != NIHIL && e->genus == MATERIA_VALOR_NODUS
                    && e->datum.nodus != NIHIL)
                {
                    MateriaNodus** locus =
                        (MateriaNodus**)xar_addere(liberi);

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
