/* oratio_sententiae.c - Vide oratio_sententiae.h. */

#include "oratio_sententiae.h"
#include "oratio_registrum.h"
#include "materia_token.h"

/* lexema primum/ultimum PARTIUM elementi (cauda exclusa) */
interior constans MateriaToken*
_terminus (
    constans MateriaNodus* e,
                      b32  ultimum)
{
    constans MateriaValor* v;
                      i32  n;

    si (e->genus == (s32)ORATIO_GENUS_INTERPUNCTIO)
    {
        redde e->loci[ORATIO_INTERPUNCTIO_SIGNUM].datum.token;
    }
    v = &e->loci[e->genus == (s32)ORATIO_GENUS_VOCABULUM
        ? (i32)ORATIO_VOCABULUM_PARTES : (i32)ORATIO_NUMERUS_CRUDUM];
    si (v->genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    n = materia_valor_lista_numerus(*v);
    si (n == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde materia_valor_lista_obtinere(*v, ultimum ? n - I : ZEPHYRUM)
        ->datum.token;
}

Xar*
oratio_sententiae_extenta (
                  Piscina* piscina,
    constans MateriaNodus* radix)
{
    Xar* exitus = xar_creare(piscina, (i32)magnitudo(OratioSententia));
    constans MateriaValor* paragraphi;
    i32 np;
    i32 i;

    si (exitus == NIHIL || radix == NIHIL)
    {
        redde exitus;
    }
    paragraphi = &radix->loci[ORATIO_DOCUMENTUM_PARAGRAPHI];
    si (paragraphi->genus != MATERIA_VALOR_LISTA)
    {
        redde exitus;
    }
    np = materia_valor_lista_numerus(*paragraphi);
    per (i = ZEPHYRUM; i < np; i++)
    {
        constans MateriaNodus* par =
            materia_valor_lista_obtinere(*paragraphi, i)
            ->datum.nodus;
        constans MateriaValor* sententiae =
            &par->loci[ORATIO_PARAGRAPHUS_SENTENTIAE];
                          i32 ns;
                          i32 s;

        si (sententiae->genus != MATERIA_VALOR_LISTA)
        {
            perge;
        }
        ns = materia_valor_lista_numerus(*sententiae);
        per (s = ZEPHYRUM; s < ns; s++)
        {
            constans MateriaNodus* sen =
                materia_valor_lista_obtinere(*sententiae, s)
                ->datum.nodus;
            constans MateriaValor* elementa =
                &sen->loci[ORATIO_SENTENTIA_ELEMENTA];
                              i32  ne;
            constans MateriaToken* a;
            constans MateriaToken* b;
                  OratioSententia* x;

            si (elementa->genus != MATERIA_VALOR_LISTA)
            {
                perge;
            }
            ne = materia_valor_lista_numerus(*elementa);
            si (ne == ZEPHYRUM)
            {
                perge;
            }
            a = _terminus(materia_valor_lista_obtinere(*elementa,
                ZEPHYRUM)->datum.nodus,
                FALSUM);
            b = _terminus(materia_valor_lista_obtinere(*elementa, ne
                - I)->datum.nodus,
                VERUM);
            si (a == NIHIL || b == NIHIL)
            {
                perge;
            }
            x = (OratioSententia*)xar_addere(exitus);
            si (x == NIHIL)
            {
                redde NIHIL;
            }
            x->initium   = a->byte_offset;
            x->finis     = b->byte_offset + (s32)b->valor.mensura;
            x->linea     = a->linea;
            x->elementa  = ne;
        }
    }
    redde exitus;
}
