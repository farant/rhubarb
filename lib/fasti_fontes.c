/* fasti_fontes.c - fontes dierum (include/fasti_fontes.h) */
#include "fasti_fontes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

nomen structura {
     constans character* titulus;
        FastiFonsLegere  legere;
       FastiFonsAperire  aperire;
                 vacuum* ctx;
} FonsServatus;

structura FastiFontes {
            Piscina* piscina;   /* tituli fontium copiantur */
       FonsServatus  fontes[FASTI_FONTES_MAXIMI];
                i32  numerus;
};

FastiFontes*
fasti_fontes_creare (
    Piscina* piscina)
{
    FastiFontes* f;

    f = piscina ? (FastiFontes*)piscina_allocare(piscina,
        magnitudo(FastiFontes)) : NIHIL;
    si (f)
    {
        memset(f, ZEPHYRUM, magnitudo(FastiFontes));
        f->piscina = piscina;
    }
    redde f;
}

interior constans FonsServatus*
fontem_invenire (
    constans FastiFontes* f,
                  chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; f && i < f->numerus; i++)
    {
        si (chorda_aequalis_literis(titulus, f->fontes[i].titulus))
        {
            redde &f->fontes[i];
        }
    }
    redde NIHIL;
}

b32
fasti_fontes_addere (
            FastiFontes* f,
     constans character* titulus,
        FastiFonsLegere  legere,
       FastiFonsAperire  aperire,
                 vacuum* ctx)
{
         FonsServatus* r;
            character* copia;
                  i32  i;

    si (!f || !titulus || !legere || f->numerus >= FASTI_FONTES_MAXIMI)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < f->numerus; i++)
    {
        si (strcmp(f->fontes[i].titulus, titulus) == ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    copia = (character*)piscina_allocare(f->piscina,
        (memoriae_index)strlen(titulus) + I);
    si (!copia)
    {
        redde FALSUM;
    }
    strcpy(copia, titulus);
    r           = &f->fontes[f->numerus++];
    r->titulus  = copia;
    r->legere   = legere;
    r->aperire  = aperire;
    r->ctx      = ctx;
    redde VERUM;
}

/* ordo: dies, genus, titulus */
interior integer
introitus_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans FastiIntroitus* x;
    constans FastiIntroitus* y;
                        s32  r;

    x = (constans FastiIntroitus*)a;
    y = (constans FastiIntroitus*)b;
    r = fasti_comparare(x->dies, y->dies);
    si (r == ZEPHYRUM)
    {
        r = chorda_comparare(x->genus, y->genus);
    }
    si (r == ZEPHYRUM)
    {
        r = chorda_comparare(x->titulus, y->titulus);
    }
    redde r < ZEPHYRUM ? -I : r > ZEPHYRUM ? I : ZEPHYRUM;
}

Xar*
fasti_fontes_legere (
    constans FastiFontes* f,
                    Dies  prima,
                    Dies  ultima,
                 Piscina* piscina)
{
               Xar* omnes;
               Xar* exitus;
    FastiIntroitus* plani;
            chorda  fons;
               i32  i;
               i32  j;
               i32  ante;

    exitus  = xar_creare(piscina, (i32)magnitudo(FastiIntroitus));
    omnes   = xar_creare(piscina, (i32)magnitudo(FastiIntroitus));
    si (!exitus || !omnes || !f)
    {
        redde exitus;
    }
    per (i = ZEPHYRUM; i < f->numerus; i++)
    {
        ante = xar_numerus(omnes);
        (vacuum)f->fontes[i].legere(f->fontes[i].ctx, prima, ultima,
            omnes, piscina);
        fons = chorda_ex_literis(f->fontes[i].titulus, piscina);
        per (j = ante; j < xar_numerus(omnes); j++)
        {
            ((FastiIntroitus*)xar_obtinere(omnes, j))->fons = fons;
        }
    }
    si (xar_numerus(omnes) == ZEPHYRUM)
    {
        redde exitus;
    }
    plani = (FastiIntroitus*)piscina_allocare(piscina,
        (memoriae_index)xar_numerus(omnes) * magnitudo(FastiIntroitus));
    si (!plani)
    {
        redde exitus;
    }
    per (i = ZEPHYRUM; i < xar_numerus(omnes); i++)
    {
        plani[i] = *(FastiIntroitus*)xar_obtinere(omnes, i);
    }
    qsort(plani, (size_t)xar_numerus(omnes), magnitudo(FastiIntroitus),
        introitus_comparare);
    per (i = ZEPHYRUM; i < xar_numerus(omnes); i++)
    {
        *(FastiIntroitus*)xar_addere(exitus) = plani[i];
    }
    redde exitus;
}

b32
fasti_fontes_aperire (
          constans FastiFontes* f,
       constans FastiIntroitus* introitus)
{
    constans FonsServatus* r;

    si (!f || !introitus)
    {
        redde FALSUM;
    }
    r = fontem_invenire(f, introitus->fons);
    redde r && r->aperire && r->aperire(r->ctx, introitus);
}

i32
fasti_annuum_expandere (
     s32  mensis,
     s32  dies,
    Dies  prima,
    Dies  ultima,
    Dies* exitus,
     i32  maximum)
{
    Dies d;
     s32 annus;
     i32 n;

    n = ZEPHYRUM;
    per (annus = prima.annus; annus <= ultima.annus && n < maximum;
        annus++)
    {
        /* 29 II in anno communi: 28 II (Franus 10-15) */
        d = fasti_dies(annus, mensis, mensis == II && dies == XXIX
            && !fasti_est_bissextilis(annus) ? XXVIII : dies);
        si (   fasti_dies_valida(d) && fasti_comparare(d, prima)
            >= ZEPHYRUM && fasti_comparare(d, ultima) <= ZEPHYRUM)
        {
            exitus[n++] = d;
        }
    }
    redde n;
}

chorda
fasti_introitum_textus (
    constans FastiIntroitus* introitus,
                    Piscina* piscina)
{
    character textus[XXXII];
          s32 aetas;

    si (!chorda_aequalis_literis(introitus->genus, FASTI_GENUS_NATALIS))
    {
        redde introitus->titulus;
    }
    aetas = introitus->annus_originis > ZEPHYRUM ? introitus->dies.annus
        - introitus->annus_originis : ZEPHYRUM;
    si (aetas > ZEPHYRUM)
    {
        sprintf(textus, " turns %d", (integer)aetas);
        redde chorda_concatenare(introitus->titulus, chorda_ex_literis(
            textus, piscina), piscina);
    }
    redde chorda_concatenare(introitus->titulus, chorda_ex_literis(
        "'s birthday", piscina), piscina);
}
