/* saltuarius_nexus.c - Implementatio pontis silvae */

#include "saltuarius_nexus.h"

interior chorda
_chorda_ex_silva (constans SilvaChorda* s)
{
    chorda c;

    c.mensura = s->mensura;
    c.datum = s->datum;
    redde c;
}

SaltuariusNexus*
saltuarius_nexus_creare (Piscina* persistens)
{
    SaltuariusNexus* nexus;
    SilvaPiscina* arena;

    nexus = (SaltuariusNexus*)piscina_allocare_ordinatum(persistens,
        (memoriae_index)magnitudo(SaltuariusNexus), IV);
    si (nexus == NIHIL)
    {
        redde NIHIL;
    }
    nexus->piscina = persistens;
    nexus->latina = tabula_dispersa_creare_chorda(persistens, DXII);
    nexus->numerus_latinorum = ZEPHYRUM;
    si (nexus->latina == NIHIL)
    {
        redde NIHIL;
    }

    /* indicem latinorum lexare: tripla "# define NOMEN" in
     * silva_latina_textus (exemplar probationis custodis silvae).
     * Arena temporaria: lexemata moriuntur, claves in textum
     * STATICUM amalgamae spectant - aeternae. */
    arena = silva_piscina_generare_dynamicum("nexus_latina",
        2097152);
    si (arena != NIHIL)
    {
        SilvaXar* lexemata = silva_lexare(arena,
            silva_latina_textus, silva_latina_mensura, ZEPHYRUM);

        si (lexemata != NIHIL)
        {
            i32 n = silva_xar_numerus(lexemata);
            i32 k;

            per (k = ZEPHYRUM; k + II < n; k++)
            {
                SilvaToken* t = *(SilvaToken**)silva_xar_obtinere(
                    lexemata, k);
                SilvaToken* d = *(SilvaToken**)silva_xar_obtinere(
                    lexemata, k + I);
                SilvaToken* titulus = *(SilvaToken**)
                    silva_xar_obtinere(lexemata, k + II);

                si (t->genus == SILVA_LEX_CANCELLUM
                    && t->initium_lineae
                    && d->genus == SILVA_LEX_IDENTIFICATOR
                    && d->valor.mensura == VI
                    && d->valor.datum[ZEPHYRUM] == 'd'
                    && chorda_aequalis_literis(
                        _chorda_ex_silva(&d->valor), "define")
                    && titulus->genus == SILVA_LEX_IDENTIFICATOR)
                {
                    si (tabula_dispersa_inserere(nexus->latina,
                        _chorda_ex_silva(&titulus->valor),
                        (vacuum*)(s64)I))
                    {
                        nexus->numerus_latinorum++;
                    }
                }
            }
        }
        silva_piscina_destruere(arena);
    }
    redde nexus;
}

b32
saltuarius_nexus_est_latinum (constans SaltuariusNexus* nexus,
    chorda titulus)
{
    redde tabula_dispersa_continet(nexus->latina, titulus);
}

b32
saltuarius_nexus_est_fons_c (chorda via)
{
    /* extensio manualis: ultimum '.' quaerere */
    s32 k;

    per (k = (s32)via.mensura - I; k >= ZEPHYRUM; k--)
    {
        si (via.datum[k] == '.')
        {
            i32 reliqui = via.mensura - (i32)k - I;

            si (reliqui == I
                && (via.datum[k + I] == 'c'
                    || via.datum[k + I] == 'h'))
            {
                redde VERUM;
            }
            redde FALSUM;
        }
        si (via.datum[k] == '/')
        {
            frange;
        }
    }
    redde FALSUM;
}

/* Classis pro genere lexematis (contextus directivae extra) */
interior SaltuariusClassis
_classis_generis (constans SaltuariusNexus* nexus,
    constans SilvaToken* t)
{
    si (t->genus >= SILVA_LEX_AUTO && t->genus <= SILVA_LEX_WHILE)
    {
        redde SALT_CLASSIS_VERBUM;
    }
    commutatio (t->genus)
    {
        casus SILVA_LEX_IDENTIFICATOR:
            si (saltuarius_nexus_est_latinum(nexus,
                _chorda_ex_silva(&t->valor)))
            {
                redde SALT_CLASSIS_LATINA;
            }
            redde SALT_CLASSIS_PLANUM;
        casus SILVA_LEX_COMMENTUM_CLAUSUM:
        casus SILVA_LEX_COMMENTUM_LINEA:
            redde SALT_CLASSIS_COMMENTUM;
        casus SILVA_LEX_STRING_LIT:
        casus SILVA_LEX_CHARACTER_LIT:
        casus SILVA_LEX_STRING_IMPERFECTUM:
        casus SILVA_LEX_CHARACTER_IMPERFECTUM:
            redde SALT_CLASSIS_CHORDA;
        casus SILVA_LEX_INTEGER:
        casus SILVA_LEX_FLOAT:
            redde SALT_CLASSIS_NUMERUS;
        casus SILVA_LEX_OCTETUS_IGNOTUS:
            redde SALT_CLASSIS_IGNOTUM;
        ordinarius:
            redde SALT_CLASSIS_PLANUM;
    }
}

vacuum
saltuarius_nexus_classificare (constans SaltuariusNexus* nexus,
    SilvaPiscina* arena_silvae, chorda textus, i8* classis_out)
{
    SilvaXar* cruda;
    i32 n;
    i32 k;
    b32 post_cancellum = FALSUM;

    per (k = ZEPHYRUM; k < textus.mensura; k++)
    {
        classis_out[k] = (i8)SALT_CLASSIS_PLANUM;
    }

    cruda = silva_lexare_cruda(arena_silvae,
        (constans character*)textus.datum, textus.mensura,
        ZEPHYRUM);
    si (cruda == NIHIL)
    {
        redde;
    }
    n = silva_xar_numerus(cruda);
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaToken* t = *(SilvaToken**)silva_xar_obtinere(cruda, k);
        SaltuariusClassis classis;
        i32 o;

        si (t->byte_offset < ZEPHYRUM || t->longitudo == ZEPHYRUM)
        {
            perge;   /* syntheticum aut EOF */
        }

        /* directiva: CANCELLUM initio lineae + verbum sequens */
        si (t->genus == SILVA_LEX_CANCELLUM && t->initium_lineae)
        {
            classis = SALT_CLASSIS_DIRECTIVA;
            post_cancellum = VERUM;
        }
        alioquin si (post_cancellum
            && t->genus == SILVA_LEX_IDENTIFICATOR)
        {
            classis = SALT_CLASSIS_DIRECTIVA;
            post_cancellum = FALSUM;
        }
        alioquin
        {
            si (t->genus != SILVA_LEX_SPATIA
                && t->genus != SILVA_LEX_TABULAE)
            {
                post_cancellum = FALSUM;
            }
            classis = _classis_generis(nexus, t);
        }

        si (classis != SALT_CLASSIS_PLANUM)
        {
            per (o = ZEPHYRUM; o < t->longitudo; o++)
            {
                i32 index = (i32)t->byte_offset + o;

                si (index < textus.mensura)
                {
                    classis_out[index] = (i8)classis;
                }
            }
        }
    }
}
