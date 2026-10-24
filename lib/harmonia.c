/* harmonia.c - Quid simul sonat (vide include/harmonia.h)
 *
 * AGNITIO: pro radice quaque inter classes praesentes classes rotantur
 * (radix -> 0) et cum exemplaribus EXACTE comparantur; exemplar primum
 * ordine tabulae vincit. Radix = bassus si congruit, aliter radix
 * congruens minima (ordine classis a C).
 */

#include "harmonia.h"
#include "partitura.h"
#include <string.h>

nomen structura {
    HarmoniaGenus genus;
              s32 gradus[V];   /* intervalla a radice; -1 finit */
} Exemplar;

hic_manens constans Exemplar EXEMPLARIA[] = {
    { HARMONIA_MAIOR,            { 0, 4, 7, -1 } },
    { HARMONIA_MINOR,            { 0, 3, 7, -1 } },
    { HARMONIA_DIMINUTA,         { 0, 3, 6, -1 } },
    { HARMONIA_AUCTA,            { 0, 4, 8, -1 } },
    { HARMONIA_SEPTIMA,          { 0, 4, 7, 10, -1 } },
    { HARMONIA_SEPTIMA,          { 0, 4, 10, -1 } },
    { HARMONIA_SEPTIMA_MAIOR,    { 0, 4, 7, 11, -1 } },
    { HARMONIA_SEPTIMA_MINOR,    { 0, 3, 7, 10, -1 } },
    { HARMONIA_DIMIDIA_DIMINUTA, { 0, 3, 6, 10, -1 } },
    { HARMONIA_DIMINUTA_SEPTIMA, { 0, 3, 6, 9, -1 } },
    { HARMONIA_SUSPENSA_IV,      { 0, 5, 7, -1 } },
    { HARMONIA_SUSPENSA_II,      { 0, 2, 7, -1 } },
    { HARMONIA_QUINTA,           { 0, 7, -1 } },
    { HARMONIA_TERTIA_MAIOR,     { 0, 4, -1 } },
    { HARMONIA_TERTIA_MINOR,     { 0, 3, -1 } }
};

#define HARMONIA_EXEMPLARIA \
    ((i32)(magnitudo(EXEMPLARIA) / magnitudo(EXEMPLARIA[0])))

/* classis -> bit */
interior i32
_bit (
    i32 classis)
{
    redde (i32)I << classis;
}

/* intervalla exemplaris ut bits */
interior i32
_intervalla (
    constans Exemplar* e)
{
    i32 b = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k < V && e->gradus[k] >= ZEPHYRUM; k++)
    {
        b |= _bit((i32)e->gradus[k]);
    }
    redde b;
}

/* suffixa per genus (ordine enumerationis) */
hic_manens constans character* SUFFIXA[] = {
    "", "m", "dim", "aug", "7", "maj7", "m7", "m7b5", "dim7", "sus4",
    "sus2", "(5)", "(3)", "m(3)", "", "", ""
};

/* classes rotatae ut radix 0 sit */
interior i32
_rotare (
    i32 classes,
    i32 radix)
{
    redde ((classes >> radix) | (classes << (XII - radix))) & 0xFFF;
}

/* index exemplaris quod classibus a radice congruit; -1 nullum */
interior s32
_exemplar (
    i32 classes,
    i32 radix)
{
    i32 r = _rotare(classes, radix);
    i32 i;

    per (i = ZEPHYRUM; i < HARMONIA_EXEMPLARIA; i++)
    {
        si (_intervalla(&EXEMPLARIA[i]) == r)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

Harmonia
harmonia_ex_classibus (
    i32 classes,
    i32 bassus)
{
    Harmonia h;
         s32 electum = -I;
         i32 r;

    memset(h.litterae, ZEPHYRUM, magnitudo(h.litterae));
    h.classes  = classes & 0xFFF;
    h.bassus   = bassus;
    h.radix    = bassus;
    si (h.classes == ZEPHYRUM)
    {
        h.genus = HARMONIA_NULLA;
        redde h;
    }
    si ((h.classes & (h.classes - I)) == ZEPHYRUM)
    {
        h.genus = HARMONIA_CLASSIS_UNA;
        per (r = ZEPHYRUM; r < XII; r++)
        {
            si (h.classes == _bit(r))
            {
                h.radix = r;
            }
        }
        redde h;
    }
    /* bassus primum, deinde classes a C */
    si ((h.classes & _bit(bassus)) && _exemplar(h.classes, bassus) >= 0)
    {
        electum = _exemplar(h.classes, bassus);
        h.radix = bassus;
    }
    per (r = ZEPHYRUM; r < XII && electum < ZEPHYRUM; r++)
    {
        si ((h.classes & _bit(r)) && _exemplar(h.classes, r) >= 0)
        {
            electum = _exemplar(h.classes, r);
            h.radix = r;
        }
    }
    h.genus = electum >= ZEPHYRUM ? EXEMPLARIA[electum].genus
                                  : HARMONIA_IGNOTA;
    si (h.genus == HARMONIA_IGNOTA)
    {
        h.radix = bassus;
    }
    redde h;
}

/* exemplaria sine tertia (harmonia_sine_tertia); radix unica in
 * singulis - nulla rotatio alterius congruit */
hic_manens constans Exemplar SINE_TERTIA[IV] = {
    { HARMONIA_CLASSIS_UNA,   { 0, -1 } },
    { HARMONIA_QUINTA,        { 0, 7, -1 } },
    { HARMONIA_SEPTIMA,       { 0, 7, 10, -1 } },
    { HARMONIA_SEPTIMA_MAIOR, { 0, 7, 11, -1 } }
};

b32
harmonia_sine_tertia (
     i32  classes,
     i32* radix)
{
    i32 r;
    i32 i;

    classes &= 0xFFF;
    /* radix absens: bit 0 rotatus deest, exemplaria omnia eum habent */
    per (r = ZEPHYRUM; r < XII; r++)
    {
        per (i = ZEPHYRUM; i < IV; i++)
        {
            si (_rotare(classes, r) == _intervalla(&SINE_TERTIA[i]))
            {
                *radix = r;
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

Harmonia
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
    Harmonia h;
    Harmonia litterae;   /* solum litterae notarum sonantium */

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
    h = harmonia_ex_classibus(classes,
        infima < CXXVIII ? infima % XII : ZEPHYRUM);
    memcpy(h.litterae, litterae.litterae, magnitudo(h.litterae));
    redde h;
}

/* classis -> littera et alteratio ex clave ad pulsum */
interior b32
_classem_scribere (
       constans Carmen* carmen,
     constans Harmonia* h,
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
     constans Harmonia* harmonia,
                   s64  pulsus,
     ChordaAedificator* textus)
{
     constans Harmonia* h     = harmonia;
                   b32  bene  = VERUM;
                   i32  k;

    commutatio (h->genus)
    {
        casus HARMONIA_NULLA:
            redde chorda_aedificator_appendere_character(textus, 'r');
        casus HARMONIA_CLASSIS_UNA:
            bene = chorda_aedificator_appendere_character(textus, '(');
            bene = _classem_scribere(carmen, h, h->radix, pulsus,
                textus)
                && bene;
            redde chorda_aedificator_appendere_character(textus, ')')
                && bene;
        casus HARMONIA_IGNOTA:
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
        SUFFIXA[h->genus]) && bene;
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
    constans Harmonia* a,
    constans Harmonia* b)
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
        Harmonia h   = harmonia_ad_pulsum(carmen, t);
             s64 ad  = t + tempus;

        /* tempora sequentia eiusdem acordi iunguntur */
        dum (ad < finis)
        {
            Harmonia proxima = harmonia_ad_pulsum(carmen, ad);

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
               Harmonia* h,
    constans CarmenNota* nota)
{
    i32 c = nota->nota % XII;

    si (nota->littera != '\0' && h->litterae[c] == '\0')
    {
        h->litterae[c] = nota->littera;
    }
}
