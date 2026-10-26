/* harmonia.c - Quid simul sonat (vide include/harmonia.h); agnitio
 * acordi in acordum.c
 */

#include "harmonia.h"
#include "partitura.h"
#include <string.h>

/* classis -> bit */
interior i32
_bit (
    i32 classis)
{
    redde (i32)I << classis;
}

Acordum
harmonia_ad_pulsum (
    constans Carmen* carmen,
                s64  pulsus)
{
         i32 classes  = ZEPHYRUM;
         i32 infima   = CXXVIII;
         b32 solus    = FALSUM;
         i32 i;
         i32 f;
         i32 j;
     Acordum h;
     Acordum litterae;   /* solum litterae notarum sonantium */

    memset(litterae.litterae, ZEPHYRUM, magnitudo(litterae.litterae));
    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        solus = solus || carmen->partes[i].solus;
    }
    per (i = ZEPHYRUM; i < carmen->numerus_partium; i++)
    {
        constans CarmenPars* p = &carmen->partes[i];

        si (p->mutus || (solus && !p->solus))
        {
            perge;
        }
        per (f = ZEPHYRUM; f < p->numerus_frustorum; f++)
        {
            constans CarmenFrustum* fr = &p->frusta[f];

            per (j = ZEPHYRUM; j < fr->numerus; j++)
            {
                constans CarmenNota* n   = &fr->notae[j];
                                s64  ab  = fr->pulsus + n->pulsus;

                si (   n->praeparatum.mensura > ZEPHYRUM || ab > pulsus
                    || ab + n->longitudo <= pulsus)
                {
                    perge;
                }
                classes |= _bit(n->nota % XII);
                harmonia_litteram_notare(&litterae, n);
                si (n->nota < infima)
                {
                    infima = n->nota;
                }
            }
        }
    }
    h = acordum_ex_classibus(classes,
        infima < CXXVIII ? infima % XII : ZEPHYRUM);
    memcpy(h.litterae, litterae.litterae, magnitudo(h.litterae));
    redde h;
}

/* classis -> littera et alteratio ex clave ad pulsum */
interior b32
_classem_scribere (
       constans Carmen* carmen,
      constans Acordum* h,
                   i32  classis,
                   s64  pulsus,
     ChordaAedificator* textus)
{
       CarmenNota n;
    PartituraNota o;
              b32 bene;
              s32 k;

    n.pulsus     = ZEPHYRUM;
    n.longitudo  = I;
    n.nota       = LX + classis;
    n.lucrum     = 1.0f;
    /* littera auctoris si nota huius classis eam tulit */
    n.littera = h->litterae[classis % XII];
    n.praeparatum.datum = NIHIL;
    n.praeparatum.mensura = ZEPHYRUM;
    n.offset = ZEPHYRUM;
    o = partitura_orthographia(carmen, &n, pulsus);
    bene = chorda_aedificator_appendere_character(textus, o.littera);
    per (k = ZEPHYRUM; k < o.alteratio; k++)
    {
        bene = chorda_aedificator_appendere_character(textus, '#')
            && bene;
    }
    per (k = ZEPHYRUM; k < -o.alteratio; k++)
    {
        bene = chorda_aedificator_appendere_character(textus, 'b')
            && bene;
    }
    redde bene;
}

b32
harmonia_scribere (
       constans Carmen* carmen,
      constans Acordum* harmonia,
                   s64  pulsus,
     ChordaAedificator* textus)
{
      constans Acordum* h     = harmonia;
                   b32  bene  = VERUM;
                   i32  k;

    commutatio (h->genus)
    {
        casus ACORDUM_NULLA:
            redde chorda_aedificator_appendere_character(textus, 'r');
        casus ACORDUM_CLASSIS_UNA:
            bene = chorda_aedificator_appendere_character(textus, '(');
            bene = _classem_scribere(carmen, h, h->radix, pulsus,
                textus)
                && bene;
            redde chorda_aedificator_appendere_character(textus, ')')
                && bene;
        casus ACORDUM_IGNOTA:
            /* classes a basso sursum */
            bene = chorda_aedificator_appendere_character(textus, '[');
            per (k = ZEPHYRUM; k < XII; k++)
            {
                i32 c = (h->bassus + (i32)k) % XII;

                si (h->classes & _bit(c))
                {
                    si (k > ZEPHYRUM)
                    {
                        bene = chorda_aedificator_appendere_character(
                            textus, ' ') && bene;
                    }
                    bene = _classem_scribere(carmen, h, c, pulsus,
                                             textus) && bene;
                }
            }
            redde chorda_aedificator_appendere_character(textus, ']')
                && bene;
        ordinarius:
            frange;
    }
    bene = _classem_scribere(carmen, h, h->radix, pulsus, textus);
    bene = chorda_aedificator_appendere_literis(textus,
        acordum_suffixum(h->genus)) && bene;
    si (h->bassus != h->radix)
    {
        bene = chorda_aedificator_appendere_character(textus, '/')
            && bene;
        bene = _classem_scribere(carmen, h, h->bassus, pulsus, textus)
            && bene;
    }
    redde bene;
}

interior b32
_aequales (
    constans Acordum* a,
    constans Acordum* b)
{
    redde a->genus == b->genus && a->radix == b->radix
        && a->bassus == b->bassus && a->classes == b->classes;
}

b32
harmonia_lineam_scribere (
               vacuum* ctx,
      constans Carmen* carmen,
                  s64  initium,
                  s64  finis,
    ChordaAedificator* textus)
{
    s64 tempus  = carmen_tempus_mensurae(carmen, initium);
    s64 t       = initium;
    b32 bene    = VERUM;
    b32 primum  = VERUM;

    (vacuum)ctx;
    dum (t < finis)
    {
         Acordum h   = harmonia_ad_pulsum(carmen, t);
             s64 ad  = t + tempus;

        /* tempora sequentia eiusdem acordi iunguntur */
        dum (ad < finis)
        {
            Acordum proxima = harmonia_ad_pulsum(carmen, ad);

            si (!_aequales(&h, &proxima))
            {
                frange;
            }
            ad += tempus;
        }
        ad = ad < finis ? ad : finis;
        si (!primum)
        {
            bene = chorda_aedificator_appendere_character(textus, ' ')
                && bene;
        }
        primum  = FALSUM;
        bene    = harmonia_scribere(carmen, &h, t, textus) && bene;
        bene = chorda_aedificator_appendere_character(textus, ':')
            && bene;
        bene  = partitura_valorem_scribere(ad - t, textus) && bene;
        t     = ad;
    }
    redde bene;
}

vacuum
harmonia_litteram_notare (
                Acordum* h,
    constans CarmenNota* nota)
{
    i32 c = nota->nota % XII;

    si (nota->littera != '\0' && h->litterae[c] == '\0')
    {
        h->litterae[c] = nota->littera;
    }
}
