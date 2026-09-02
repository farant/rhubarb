/* saltuarius_claves.c - Implementatio dispensationis clavium */

#include "saltuarius_claves.h"

vacuum
saltuarius_claves_parare (
    SaltuariusClaves* claves)
{
    claves->g_pendens = FALSUM;
    claves->f_pendens = FALSUM;
}

interior b32
_iussum_simplex (
    SaltuariusIussum* iussum,
     SaltuariusActio  genus)
{
    iussum->genus  = genus;
    iussum->runa   = ZEPHYRUM;
    iussum->x      = ZEPHYRUM;
    iussum->y      = ZEPHYRUM;
    redde VERUM;
}

interior b32
_clavem_tradere (
           SaltuariusClaves* claves,
            SaltuariusModus  modus,
    constans TesseraEventum* ev,
           SaltuariusIussum* iussum)
{
    b32 in_fonte = (modus == SALT_MODUS_FONS);

    /* f<littera>: clavis proxima quaevis runam portans saltat */
    si (claves->f_pendens)
    {
        claves->f_pendens = FALSUM;
        si (   ev->runa != ZEPHYRUM
            && (ev->modificatores & TESSERA_MODIFICATOR_IMPERIUM)
                == ZEPHYRUM)
        {
            _iussum_simplex(iussum, SALT_ACTIO_SALTARE);
            iussum->runa = ev->runa;
            redde VERUM;
        }
        redde FALSUM;  /* fuga/sagitta/etc: saltus abortus */
    }

    /* gg: 'g' secunda */
    si (claves->g_pendens)
    {
        claves->g_pendens = FALSUM;
        si (   ev->runa          == (s32)'g'
            && ev->modificatores == ZEPHYRUM)
        {
            redde _iussum_simplex(iussum, SALT_ACTIO_PRIMUM);
        }
        /* clavis alia: pendens purgatum, eventum NORMALITER
         * tractatur (cadit infra) */
    }

    /* ctrl-d / ctrl-u */
    si ((ev->modificatores & TESSERA_MODIFICATOR_IMPERIUM)
        != ZEPHYRUM)
    {
        si (ev->runa == (s32)'d')
        {
            redde _iussum_simplex(iussum,
                SALT_ACTIO_PAGINA_DEORSUM);
        }
        si (ev->runa == (s32)'u')
        {
            redde _iussum_simplex(iussum, SALT_ACTIO_PAGINA_SURSUM);
        }
        redde FALSUM;
    }

    /* claves speciales */
    commutatio (ev->clavis)
    {
        casus TESSERA_CLAVIS_SURSUM:
            redde _iussum_simplex(iussum, SALT_ACTIO_SURSUM);
        casus TESSERA_CLAVIS_DEORSUM:
            redde _iussum_simplex(iussum, SALT_ACTIO_DEORSUM);
        casus TESSERA_CLAVIS_SINISTRA:
            redde _iussum_simplex(iussum, in_fonte
                ? SALT_ACTIO_SINISTRA : SALT_ACTIO_PARENS);
        casus TESSERA_CLAVIS_DEXTRA:
            redde _iussum_simplex(iussum, in_fonte
                ? SALT_ACTIO_DEXTRA : SALT_ACTIO_INTRARE);
        casus TESSERA_CLAVIS_REDITUS:
            redde _iussum_simplex(iussum, SALT_ACTIO_INTRARE);
        casus TESSERA_CLAVIS_TABULA: si (in_fonte)
                                     {
            redde _iussum_simplex(iussum, SALT_ACTIO_STRUCTURA);
                                     }
        frange;
        casus TESSERA_CLAVIS_FUNCTIO: si (   in_fonte
                                          && ev->numerus == II)
                                      {
            redde _iussum_simplex(iussum, SALT_ACTIO_FRUCTUS);
                                      }
        frange;
        ordinarius:
            frange;
    }

    /* runae nudae */
    commutatio (ev->runa)
    {
        casus 'k': redde _iussum_simplex(iussum, SALT_ACTIO_SURSUM);
        casus 'j': redde _iussum_simplex(iussum, SALT_ACTIO_DEORSUM);
        casus 'h': redde _iussum_simplex(iussum, in_fonte
                       ? SALT_ACTIO_SINISTRA : SALT_ACTIO_PARENS);
        casus 'l': redde _iussum_simplex(iussum, in_fonte
                       ? SALT_ACTIO_DEXTRA : SALT_ACTIO_INTRARE);
        casus 'L': si (in_fonte)
                   {
            redde _iussum_simplex(iussum, SALT_ACTIO_STRATUM);
                   }
        frange;
        casus 'o': si (in_fonte)
                   {
            redde _iussum_simplex(iussum, SALT_ACTIO_ORIGO);
                   }
        frange;
        casus '/': si (in_fonte)
                   {
            redde _iussum_simplex(iussum, SALT_ACTIO_QUAESTIO);
                   }
        frange;
        casus 'n': si (in_fonte)
                   {
            redde _iussum_simplex(iussum, SALT_ACTIO_PROXIMUM);
                   }
        frange;
        casus 'N': si (in_fonte)
                   {
            redde _iussum_simplex(iussum, SALT_ACTIO_PRIUS);
                   }
        frange;
        casus 'q': redde _iussum_simplex(iussum, SALT_ACTIO_FINIRE);
        casus 'G': redde _iussum_simplex(iussum, SALT_ACTIO_ULTIMUM);
        casus 'g':
            claves->g_pendens = VERUM;
            redde FALSUM;
        casus 'f':
            claves->f_pendens = VERUM;
            redde FALSUM;
        ordinarius:
            frange;
    }
    redde FALSUM;
}

b32
saltuarius_claves_tradere (
           SaltuariusClaves* claves,
            SaltuariusModus  modus,
    constans TesseraEventum* eventum,
           SaltuariusIussum* iussum_out)
{
    commutatio (eventum->genus)
    {
        casus TESSERA_EVENTUM_CLAVIS:
            redde _clavem_tradere(claves, modus, eventum,
                iussum_out);
        casus TESSERA_EVENTUM_MUS:
            /* mus pendentia non purgat (innocuus) */
            commutatio (eventum->mus_genus)
            {
                casus TESSERA_MUS_ROTA_SURSUM:
                    redde _iussum_simplex(iussum_out,
                        SALT_ACTIO_ROTA_SURSUM);
                casus TESSERA_MUS_ROTA_DEORSUM:
                    redde _iussum_simplex(iussum_out,
                        SALT_ACTIO_ROTA_DEORSUM);
                casus TESSERA_MUS_PRESSUS:
                    _iussum_simplex(iussum_out, SALT_ACTIO_CLICUS);
                    iussum_out->x = eventum->mus_x;
                    iussum_out->y = eventum->mus_y;
                    redde VERUM;
                ordinarius:
                    redde FALSUM;
            }
        ordinarius:
            /* AMPLITUDO/RESUMPTUM/NIHIL: principale tractat */
            redde FALSUM;
    }
}
