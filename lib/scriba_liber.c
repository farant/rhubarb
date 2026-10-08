/* scriba_liber.c - liber paginarum scribae (vicus-latera S2b) */

#include "scriba_liber.h"
#include "chorda_aedificator.h"
#include "stml.h"
#include "xar.h"

#include <string.h>

#define INTERVALLUM  LXIV

hic_manens constans character clavis_indicis[] = "scriba/paginae";

structura ScribaLiber {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
                    i32  latitudo;
                    i32  altitudo;
                    Xar* nomina;      /* Xar de chorda (internatae) */
                    Xar* documenta;   /* Xar de ScribaDocumentum*;
                                       * NIHIL donec apertum */
};

interior chorda
chorda_vacua_libri (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior vacuum
nomen_addere (
    ScribaLiber* l,
         chorda  titulus)
{
     chorda* c;
     chorda* sedes;

    c = chorda_internare(l->intern, titulus);
    sedes = (chorda*)xar_addere(l->nomina);
    *sedes = c ? *c : titulus;
    *(ScribaDocumentum**)xar_addere(l->documenta) = NIHIL;
}

interior b32
indicem_scribere (
    ScribaLiber* l)
{
    ChordaAedificator* a;
                  i32  i;

    a = chorda_aedificator_creare(l->piscina, (memoriae_index)CCLVI);
    chorda_aedificator_appendere_literis(a, "<paginae>");
    per (i = ZEPHYRUM; i < xar_numerus(l->nomina); i++)
    {
        chorda_aedificator_appendere_literis(a, "<pagina nomen=\"");
        chorda_aedificator_appendere_chorda(a,
            *(chorda*)xar_obtinere(l->nomina, i));
        chorda_aedificator_appendere_literis(a, "\"/>");
    }
    chorda_aedificator_appendere_literis(a, "</paginae>");
    redde volumen_plagulam_condere(l->volumen,
        chorda_ex_literis(clavis_indicis, l->piscina),
        chorda_aedificator_finire(a), "scriba:paginae");
}

ScribaLiber*
scriba_liber_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo)
{
     ScribaLiber* l;
          chorda  index;
             b32  inventum;
    StmlResultus  res;
       StmlNodus* n;
          chorda* a;
             i32  i;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    l = (ScribaLiber*)piscina_allocare(piscina, magnitudo(ScribaLiber));
    si (!l)
    {
        redde NIHIL;
    }
    memset(l, ZEPHYRUM, magnitudo(ScribaLiber));
    l->piscina   = piscina;
    l->intern    = intern;
    l->volumen   = volumen;
    l->latitudo  = latitudo;
    l->altitudo  = altitudo;
    l->nomina    = xar_creare(piscina, (i32)magnitudo(chorda));
    l->documenta = xar_creare(piscina,
        (i32)magnitudo(ScribaDocumentum*));
    index = volumen_plagulam_promere(volumen,
        chorda_ex_literis(clavis_indicis, piscina), piscina, &inventum);
    si (!inventum)
    {
        nomen_addere(l, chorda_ex_literis("1", piscina));
        redde indicem_scribere(l) ? l : NIHIL;
    }
    res = stml_legere_ex_literis(chorda_ut_cstr(index, piscina),
        piscina,
        intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(res.elementum_radix);
         i++)
    {
        n = stml_liberum_ad_indicem(res.elementum_radix, i);
        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        a = stml_attributum_capere(n, "nomen");
        si (a && a->mensura > ZEPHYRUM)
        {
            nomen_addere(l, *a);
        }
    }
    si (xar_numerus(l->nomina) == ZEPHYRUM)
    {
        nomen_addere(l, chorda_ex_literis("1", piscina));
        (vacuum)indicem_scribere(l);
    }
    redde l;
}

i32
scriba_liber_numerus (
    constans ScribaLiber* l)
{
    redde l ? xar_numerus(l->nomina) : ZEPHYRUM;
}

chorda
scriba_liber_nomen (
    constans ScribaLiber* l,
                     i32  index)
{
    si (!l || index >= xar_numerus(l->nomina))
    {
        redde chorda_vacua_libri();
    }
    redde *(chorda*)xar_obtinere(l->nomina, index);
}

s32
scriba_liber_index (
    constans ScribaLiber* l,
                  chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; l && i < xar_numerus(l->nomina); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(l->nomina, i),
            titulus))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

ScribaDocumentum*
scriba_liber_pagina (
    ScribaLiber* l,
         chorda  titulus)
{
       ScribaDocumentum** sedes;
                    s32   i;
     constans character*  spatium;

    i = scriba_liber_index(l, titulus);
    si (i < ZEPHYRUM)
    {
        redde NIHIL;
    }
    sedes = (ScribaDocumentum**)xar_obtinere(l->documenta, (i32)i);
    si (*sedes)
    {
        redde *sedes;
    }
    spatium = chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(
        "paginae/", l->piscina), titulus, l->piscina), l->piscina);
    *sedes = scriba_documentum_aperire(l->piscina, l->intern,
        l->volumen,
        spatium);
    si (!*sedes)
    {
        *sedes = scriba_documentum_creare(l->piscina, l->intern,
            l->volumen, spatium, l->latitudo, l->altitudo, INTERVALLUM);
    }
    redde *sedes;
}

chorda
scriba_liber_pagina_nova (
    ScribaLiber* l)
{
    s32 n;

    si (!l)
    {
        redde chorda_vacua_libri();
    }
    n = (s32)xar_numerus(l->nomina) + I;
    dum (scriba_liber_index(l, chorda_ex_s32(n, l->piscina))
        >= ZEPHYRUM)
    {
        n++;
    }
    nomen_addere(l, chorda_ex_s32(n, l->piscina));
    si (!indicem_scribere(l))
    {
        xar_truncare(l->nomina, xar_numerus(l->nomina) - I);
        xar_truncare(l->documenta, xar_numerus(l->documenta) - I);
        redde chorda_vacua_libri();
    }
    redde scriba_liber_nomen(l, xar_numerus(l->nomina) - I);
}
