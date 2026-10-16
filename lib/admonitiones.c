/* admonitiones.c - admonitiones generales (include/admonitiones.h) */
#include "admonitiones.h"

#include <stdio.h>
#include <string.h>

structura Admonitiones {
    Piscina* piscina;
        Xar* elementa;   /* Xar de Admonitio, recentissima primum */
        i32  maximum;
};

Admonitiones*
admonitiones_creare (
     Piscina* piscina,
         i32  maximum)
{
    Admonitiones* a;

    a = piscina ? (Admonitiones*)piscina_allocare(piscina,
        magnitudo(Admonitiones)) : NIHIL;
    si (!a)
    {
        redde NIHIL;
    }
    a->piscina   = piscina;
    a->maximum   = maximum > ZEPHYRUM ? maximum : I;
    a->elementa  = xar_creare(piscina, (i32)magnitudo(Admonitio));
    redde a->elementa ? a : NIHIL;
}

interior chorda
copia (
      chorda  c,
     Piscina* piscina)
{
    redde chorda_transcribere(c, piscina);
}

b32
admonitiones_addere (
               Admonitiones* a,
         constans Admonitio* x)
{
    Admonitio  nova;
    Admonitio* locus;
          i32  n;
          i32  i;
          i32  ad;

    si (!a || !x || x->id.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    n = xar_numerus(a->elementa);
    per (i = ZEPHYRUM; i < n; i++)
    {
        si (chorda_aequalis(((Admonitio*)xar_obtinere(a->elementa,
            i))->id, x->id))
        {
            redde FALSUM;
        }
    }
    /* locus ordinatus: post omnes recentiores aut aequales */
    ad = ZEPHYRUM;
    dum (ad < n && ((Admonitio*)xar_obtinere(a->elementa, ad))->tempus
        >= x->tempus)
    {
        ad++;
    }
    si (ad >= a->maximum)
    {
        redde FALSUM;   /* vetustior omnibus et index plenus */
    }
    nova             = *x;
    nova.id          = copia(x->id, a->piscina);
    nova.fons        = copia(x->fons, a->piscina);
    nova.titulus     = copia(x->titulus, a->piscina);
    nova.corpus      = copia(x->corpus, a->piscina);
    nova.actio       = copia(x->actio, a->piscina);
    nova.argumentum  = copia(x->argumentum, a->piscina);
    /* addere in fine, deinde deorsum ad locum movere */
    locus = (Admonitio*)xar_addere(a->elementa);
    si (!locus)
    {
        redde FALSUM;
    }
    per (i = n; i > ad; i--)
    {
        *(Admonitio*)xar_obtinere(a->elementa, i) = *(Admonitio*)
            xar_obtinere(a->elementa, i - I);
    }
    *(Admonitio*)xar_obtinere(a->elementa, ad) = nova;
    /* ultra maximum: vetustissima (ultima) eicitur */
    si (xar_numerus(a->elementa) > a->maximum)
    {
        xar_truncare(a->elementa, a->maximum);
    }
    redde VERUM;
}

b32
admonitionem_legere_notare (
    Admonitiones* a,
          chorda  id)
{
    Admonitio* x;
          i32  i;

    per (i = ZEPHYRUM; a && i < xar_numerus(a->elementa); i++)
    {
        x = (Admonitio*)xar_obtinere(a->elementa, i);
        si (chorda_aequalis(x->id, id))
        {
            x->lecta = VERUM;
            redde VERUM;
        }
    }
    redde FALSUM;
}

i32
admonitiones_numerus (
    constans Admonitiones* a)
{
    redde a ? xar_numerus(a->elementa) : ZEPHYRUM;
}

i32
admonitiones_non_lectae (
    constans Admonitiones* a)
{
    i32 i;
    i32 n;

    n = ZEPHYRUM;
    per (i = ZEPHYRUM; a && i < xar_numerus(a->elementa); i++)
    {
        si (!((constans Admonitio*)xar_obtinere(a->elementa, i))->lecta)
        {
            n++;
        }
    }
    redde n;
}

constans Admonitio*
admonitio (
    constans Admonitiones* a,
                      i32  index)
{
    si (!a || index >= xar_numerus(a->elementa))
    {
        redde NIHIL;
    }
    redde (constans Admonitio*)xar_obtinere(a->elementa, index);
}

/* prima linea textus */
interior chorda
linea_prima (
    chorda c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura && c.datum[i] != '\n'; i++)
    {
    }
    c.mensura = i;
    redde c;
}

b32
admonitio_ex_eventu (
    constans PraeconiumEventus* e,
                        chorda  titulus,
                     Admonitio* x,
                       Piscina* piscina)
{
    character id[CXXVIII];

    si (!e || !x)
    {
        redde FALSUM;
    }
    memset(x, ZEPHYRUM, magnitudo(*x));
    si (chorda_aequalis_literis(e->eventus, "Notification"))
    {
        si (   !chorda_aequalis_literis(e->genus, "permission_prompt")
            && !chorda_aequalis_literis(e->genus, "agent_needs_input")
            && !chorda_incipit(e->genus,
            chorda_ex_literis("elicitation",
                   piscina)))
        {
            redde FALSUM;
        }
        x->gradus = ADMONITIO_ATTENTIO;
        x->corpus = e->nuntius;
    }
    alioquin si (chorda_aequalis_literis(e->eventus, "Stop"))
    {
        x->gradus = ADMONITIO_NUNTIUS;
        x->corpus = linea_prima(e->nuntius);
    }
    alioquin si (chorda_aequalis_literis(e->eventus, "StopFailure"))
    {
        x->gradus = ADMONITIO_ATTENTIO;
        x->corpus = e->nuntius;
    }
    alioquin
    {
        redde FALSUM;
    }
    sprintf(id, "colloquia:%.*s:%ld", (integer)(e->sessio.mensura < LXIV
        ? e->sessio.mensura : LXIV),
        (constans character*)e->sessio.datum,
        (long)e->tempus);
    x->id       = chorda_ex_literis(id, piscina);
    x->fons     = chorda_ex_literis("colloquia", piscina);
    x->titulus  = titulus;
    x->tempus   = e->tempus;
    x->lecta    = FALSUM;
    x->actio    = chorda_ex_literis("panellus", piscina);
    x->argumentum  = chorda_concatenare(chorda_concatenare(e->panellus,
        chorda_ex_literis(" ", piscina), piscina), e->servitor,
        piscina);
    redde VERUM;
}
