/* scriba_liber.c - liber paginarum scribae (vicus-latera S2b) */

#include "scriba_liber.h"
#include "chorda_aedificator.h"
#include "stml.h"
#include "xar.h"

#include <string.h>

#define INTERVALLUM  LXIV

/* liber communis vici: mos prior (A6c: spatium per liber) */
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
                 chorda index;       /* A6c: plagula indicis */
                 chorda spatium;     /* A6c: praefixum paginarum */
    /* lector L5: natus = index in volumine; piger ante natalem */
                    b32 natus;
                    b32  (*natale)(vacuum* ctx, ScribaLiber* liber);
                 vacuum* natale_ctx;
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
        l->index,
        chorda_aedificator_finire(a), "scriba:paginae");
}

/* natalis libri pigri: index scribitur, natale vocatur (semel);
 * natus: nihil */
interior b32
librum_nasci (
    ScribaLiber* l)
{
    si (l->natus)
    {
        redde VERUM;
    }
    si (!indicem_scribere(l))
    {
        redde FALSUM;
    }
    l->natus = VERUM;
    si (l->natale)
    {
        (vacuum)l->natale(l->natale_ctx, l);
    }
    redde VERUM;
}

/* natale documenti nascituri (ctx = liber): liber nascitur */
interior b32
paginam_natam (
    vacuum* ctx)
{
    redde librum_nasci((ScribaLiber*)ctx);
}

/* index servatur: liber piger hoc ipso nascitur */
interior b32
indicem_servare (
    ScribaLiber* l)
{
    si (!l->natus)
    {
        redde librum_nasci(l);
    }
    redde indicem_scribere(l);
}

/* nomina paginarum ex plagula indicis (lectio sola): Xar de chorda;
 * *inventum FALSUM si plagula absens; NIHIL si index malus */
interior Xar*
nomina_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                 chorda  via_indicis,
                    b32* inventum)
{
          chorda  index;
    StmlResultus  res;
       StmlNodus* n;
          chorda* a;
             Xar* nomina;
             i32  i;

    nomina = xar_creare(piscina, (i32)magnitudo(chorda));
    index  = volumen_plagulam_promere(volumen, via_indicis, piscina,
        inventum);
    si (!nomina || !*inventum)
    {
        redde nomina;
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
            *(chorda*)xar_addere(nomina) = *a;
        }
    }
    redde nomina;
}

/* liber cum viis datis (index, praefixum paginarum); piger: index
 * absens non scribitur (liber nondum natus) */
interior ScribaLiber*
librum_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                 chorda  via_indicis,
                 chorda  spatium_paginarum,
                    i32  latitudo,
                    i32  altitudo,
                    b32  piger)
{
     ScribaLiber* l;
             Xar* nomina;
             b32  inventum;
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
    l->index    = via_indicis;
    l->spatium  = spatium_paginarum;
    nomina = nomina_legere(piscina, intern, volumen, l->index,
        &inventum);
    si (!inventum)
    {
        nomen_addere(l, chorda_ex_literis("1", piscina));
        si (piger)
        {
            redde l;
        }
        l->natus = VERUM;
        redde indicem_scribere(l) ? l : NIHIL;
    }
    l->natus = VERUM;
    si (!nomina)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        nomen_addere(l, *(chorda*)xar_obtinere(nomina, i));
    }
    si (xar_numerus(l->nomina) == ZEPHYRUM)
    {
        nomen_addere(l, chorda_ex_literis("1", piscina));
        (vacuum)indicem_scribere(l);
    }
    redde l;
}

ScribaLiber*
scriba_liber_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo)
{
    si (!piscina)
    {
        redde NIHIL;
    }
    redde librum_aperire(piscina, intern, volumen, chorda_ex_literis(
        clavis_indicis, piscina), chorda_ex_literis("paginae/",
        piscina),
        latitudo, altitudo, FALSUM);
}

ScribaLiber*
scriba_liber_aperire_in_spatio (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                    i32  latitudo,
                    i32  altitudo)
{
    chorda basis;

    si (!piscina || !praefixum || !praefixum[ZEPHYRUM])
    {
        redde NIHIL;
    }
    basis = chorda_concatenare(chorda_ex_literis(praefixum, piscina),
        chorda_ex_literis("/paginae", piscina), piscina);
    redde librum_aperire(piscina, intern, volumen, basis,
        chorda_concatenare(basis, chorda_ex_literis("/", piscina),
        piscina), latitudo, altitudo, FALSUM);
}

ScribaLiber*
scriba_liber_aperire_pigre (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                    i32  latitudo,
                    i32  altitudo,
                    b32  (*natale)(vacuum* ctx, ScribaLiber* liber),
                 vacuum* natale_ctx)
{
          chorda  basis;
     ScribaLiber* l;

    si (!piscina || !praefixum || !praefixum[ZEPHYRUM])
    {
        redde NIHIL;
    }
    basis = chorda_concatenare(chorda_ex_literis(praefixum, piscina),
        chorda_ex_literis("/paginae", piscina), piscina);
    l = librum_aperire(piscina, intern, volumen, basis,
        chorda_concatenare(basis, chorda_ex_literis("/", piscina),
        piscina), latitudo, altitudo, VERUM);
    si (l)
    {
        l->natale      = natale;
        l->natale_ctx  = natale_ctx;
    }
    redde l;
}

b32
scriba_liber_natus (
    constans ScribaLiber* l)
{
    redde l && l->natus;
}

b32
scriba_liber_exstat (
                Volumen* volumen,
     constans character* praefixum,
                Piscina* piscina)
{
    b32 inventum;

    si (!volumen || !praefixum || !praefixum[ZEPHYRUM] || !piscina)
    {
        redde FALSUM;
    }
    (vacuum)volumen_plagulam_promere(volumen, chorda_concatenare(
        chorda_ex_literis(praefixum, piscina), chorda_ex_literis(
        "/paginae", piscina), piscina), piscina, &inventum);
    redde inventum;
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
    spatium = chorda_ut_cstr(chorda_concatenare(l->spatium, titulus,
        l->piscina), l->piscina);
    *sedes = scriba_documentum_aperire(l->piscina, l->intern,
        l->volumen,
        spatium);
    si (!*sedes && !l->natus)
    {
        /* liber piger: pagina nascitura; scriptura prima librum
         * nascit */
        *sedes = scriba_documentum_nasciturum(l->piscina, l->intern,
            l->volumen, spatium, l->latitudo, l->altitudo, INTERVALLUM,
            paginam_natam, l);
    }
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
    si (!indicem_servare(l))
    {
        xar_truncare(l->nomina, xar_numerus(l->nomina) - I);
        xar_truncare(l->documenta, xar_numerus(l->documenta) - I);
        redde chorda_vacua_libri();
    }
    redde scriba_liber_nomen(l, xar_numerus(l->nomina) - I);
}

ScribaDocumentum*
scriba_liber_paginam_condere (
    ScribaLiber* l,
         chorda  titulus)
{
          i32 i;
    character c;

    si (!l || titulus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < titulus.mensura; i++)
    {
        c = (character)titulus.datum[i];
        si (!(   (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
              || c == '_' || c == '-'))
        {
            redde NIHIL;
        }
    }
    /* pagina nominata = propositum: liber piger nascitur */
    si (!librum_nasci(l))
    {
        redde NIHIL;
    }
    si (scriba_liber_index(l, titulus) < ZEPHYRUM)
    {
        nomen_addere(l, titulus);
        si (!indicem_scribere(l))
        {
            xar_truncare(l->nomina, xar_numerus(l->nomina) - I);
            xar_truncare(l->documenta, xar_numerus(l->documenta) - I);
            redde NIHIL;
        }
    }
    redde scriba_liber_pagina(l, titulus);
}

/* linea folii ut textus: cellulae usque ad finem contenti, vacuae et
 * continuationes tabulae ut spatia; vacua si spatia sola */
interior chorda
lineam_legere (
    constans TabulaCharacterum* t,
                          i32  linea,
                      Piscina* piscina)
{
    chorda c;
       s32 finis;
       i32 k;
       b32 plena;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    finis      = tabula_invenire_finem_contenti(t, linea);
    si (finis < ZEPHYRUM)
    {
        redde c;
    }
    c.datum = (i8*)piscina_allocare(piscina, (memoriae_index)finis + I);
    si (!c.datum)
    {
        redde c;
    }
    plena = FALSUM;
    per (k = ZEPHYRUM; k <= (i32)finis; k++)
    {
        character x = tabula_cellula(t, linea, k);

        /* '\0' tutela sola: proiectio reaperta spatia scribit */
        si (x == '\0' || x == TAB_CONTINUATIO || x == '\t')
        {
            x = ' ';
        }
        si (x != ' ')
        {
            plena = VERUM;
        }
        c.datum[k] = (i8)x;
    }
    c.mensura = plena ? (i32)finis + I : ZEPHYRUM;
    redde c;
}

/* D5: viae libri ex praefixo (NIHIL = liber communis vici, mos
 * prior): plagula indicis et praefixum paginarum ('.../') */
interior vacuum
vias_libri (
    constans character* praefixum,
               Piscina* piscina,
                chorda* index,
                chorda* spatium)
{
    si (!praefixum)
    {
        *index    = chorda_ex_literis(clavis_indicis, piscina);
        *spatium  = chorda_ex_literis("paginae/", piscina);
        redde;
    }
    *index = chorda_concatenare(chorda_ex_literis(praefixum, piscina),
        chorda_ex_literis("/paginae", piscina), piscina);
    *spatium = chorda_concatenare(*index, chorda_ex_literis("/",
        piscina), piscina);
}

/* lineae non vacuae paginae unius ad exitus (maximum: lineae summae
 * in exitu, 0 = omnes); documentum absens non creatur */
interior vacuum
paginam_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                 chorda  spatium,
                 chorda  titulus,
                    i32  maximum,
                    Xar* exitus)
{
              ScribaDocumentum* doc;
    constans TabulaCharacterum* t;
                           i32  linea;

    doc = scriba_documentum_aperire(piscina, intern, volumen,
        chorda_ut_cstr(chorda_concatenare(spatium, titulus, piscina),
        piscina));
    t = doc ? scriba_documentum_tabula(doc) : NIHIL;
    per (linea = ZEPHYRUM; t && linea < t->altitudo; linea++)
    {
        chorda c = lineam_legere(t, linea, piscina);

        si (c.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (maximum > ZEPHYRUM && xar_numerus(exitus) >= maximum)
        {
            redde;
        }
        *(chorda*)xar_addere(exitus) = c;
    }
}

b32
scriba_liber_lineas_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                    i32  maximum,
                    Xar* exitus)
{
    chorda  index;
    chorda  spatium;
       Xar* nomina;
       b32  inventum;
       i32  i;

    si (!piscina || !intern || !volumen || !praefixum || !exitus)
    {
        redde FALSUM;
    }
    vias_libri(praefixum, piscina, &index, &spatium);
    nomina = nomina_legere(piscina, intern, volumen, index, &inventum);
    si (!inventum)
    {
        redde VERUM;
    }
    si (!nomina)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        paginam_legere(piscina, intern, volumen, spatium,
            *(chorda*)xar_obtinere(nomina, i), maximum, exitus);
    }
    redde VERUM;
}

b32
scriba_liber_nomina_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                    Xar* exitus)
{
    chorda  index;
    chorda  spatium;
       Xar* nomina;
       b32  inventum;
       i32  i;

    si (!piscina || !intern || !volumen || !exitus)
    {
        redde FALSUM;
    }
    vias_libri(praefixum, piscina, &index, &spatium);
    nomina = nomina_legere(piscina, intern, volumen, index, &inventum);
    si (!inventum)
    {
        redde VERUM;
    }
    si (!nomina)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        *(chorda*)xar_addere(exitus) = *(chorda*)xar_obtinere(nomina,
            i);
    }
    redde VERUM;
}

b32
scriba_liber_paginam_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                 chorda  titulus,
                    Xar* exitus)
{
    chorda index;
    chorda spatium;

    si (   !piscina || !intern || !volumen || !exitus
        || titulus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    vias_libri(praefixum, piscina, &index, &spatium);
    paginam_legere(piscina, intern, volumen, spatium, titulus, ZEPHYRUM,
        exitus);
    redde VERUM;
}

chorda
scriba_liber_praefixum (
    constans ScribaLiber* l)
{
      hic_manens constans character cauda[] = "/paginae";
                             chorda c;

    si (   !l || chorda_aequalis_literis(l->index, clavis_indicis)
        || l->index.mensura < (i32)(magnitudo(cauda) - I))
    {
        redde chorda_vacua_libri();
    }
    c          = l->index;
    c.mensura  = l->index.mensura - (i32)(magnitudo(cauda) - I);
    redde c;
}

/* comites P5b: via notae comitis */
interior chorda
via_comitis (
    constans character* praefixum,
               Piscina* piscina)
{
    redde chorda_concatenare(chorda_ex_literis(praefixum, piscina),
        chorda_ex_literis("/comes", piscina), piscina);
}

/* valores ut TEXTUS liberorum, non attributa: scriptor STML
 * attributa cruda scribit ('"' in semita parsationem frangit), textum
 * evadit (&lt; &amp;) et lector revocat */
interior b32
campum_addere (
                StmlNodus* n,
                  Piscina* piscina,
      InternamentumChorda* intern,
       constans character* titulus,
                   chorda  valor)
{
    StmlNodus* c;
    StmlNodus* t;

    c = stml_elementum_creare(piscina, intern, titulus);
    t = stml_textum_creare_ex_chorda(piscina, intern, valor);
    redde c && t && stml_liberum_addere(c, t)
        && stml_liberum_addere(n, c);
}

interior chorda
campum_legere (
             StmlNodus* radix,
    constans character* titulus,
               Piscina* piscina)
{
    StmlNodus* c;

    c = stml_invenire_liberum(radix, titulus);
    redde c ? stml_textus_internus(c, piscina) : chorda_vacua_libri();
}

b32
scriba_liber_comitem_notare (
                Volumen* volumen,
    InternamentumChorda* intern,
     constans character* praefixum,
                 chorda  titulus,
                 chorda  semita,
                Piscina* piscina)
{
     StmlNodus* n;
        chorda  via;
        chorda  textus;
        chorda  prior;
           b32  inventum;

    si (   !volumen || !intern || !piscina || !praefixum || !praefixum[
            ZEPHYRUM])
    {
        redde FALSUM;
    }
    n = stml_elementum_creare(piscina, intern, "comes");
    si (   !n
        || (   titulus.mensura > ZEPHYRUM
            && !campum_addere(n, piscina, intern, "titulus", titulus))
        || (   semita.mensura > ZEPHYRUM
            && !campum_addere(n, piscina, intern, "semita", semita)))
    {
        redde FALSUM;
    }
    textus  = stml_scribere(n, piscina, FALSUM);
    via     = via_comitis(praefixum, piscina);
    prior = volumen_plagulam_promere(volumen, via, piscina,
        &inventum);
    si (inventum && chorda_aequalis(prior, textus))
    {
        redde VERUM;
    }
    redde volumen_plagulam_condere(volumen, via, textus,
        "scriba:comes");
}

b32
scriba_liber_comitem_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                 chorda* titulus,
                 chorda* semita)
{
          chorda textus;
    StmlResultus res;
             b32 inventum;

    si (titulus)
    {
        *titulus = chorda_vacua_libri();
    }
    si (semita)
    {
        *semita = chorda_vacua_libri();
    }
    si (   !piscina || !intern || !volumen || !praefixum || !titulus
        || !semita)
    {
        redde FALSUM;
    }
    textus = volumen_plagulam_promere(volumen, via_comitis(praefixum,
        piscina), piscina, &inventum);
    si (!inventum)
    {
        redde FALSUM;
    }
    res = stml_legere_ex_literis(chorda_ut_cstr(textus, piscina),
        piscina,
        intern);
    si (!res.successus || !res.elementum_radix)
    {
        redde FALSUM;
    }
    *titulus  = campum_legere(res.elementum_radix, "titulus", piscina);
    *semita   = campum_legere(res.elementum_radix, "semita", piscina);
    redde VERUM;
}

b32
scriba_liber_textum_legere (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
     constans character* praefixum,
                 chorda* exitus)
{
                        chorda  index;
                        chorda  spatium;
                           Xar* nomina;
                           b32  inventum;
                           b32  prima;
                           i32  i;
                           i32  linea;
                           s32  ultima;
             ChordaAedificator* a;
              ScribaDocumentum* doc;
    constans TabulaCharacterum* t;

    si (!piscina || !intern || !volumen || !exitus)
    {
        redde FALSUM;
    }
    *exitus = chorda_vacua_libri();
    vias_libri(praefixum, piscina, &index, &spatium);
    nomina = nomina_legere(piscina, intern, volumen, index, &inventum);
    si (!inventum)
    {
        redde VERUM;
    }
    si (!nomina)
    {
        redde FALSUM;
    }
    a      = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    prima  = VERUM;
    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        doc = scriba_documentum_aperire(piscina, intern, volumen,
            chorda_ut_cstr(chorda_concatenare(spatium,
            *(chorda*)xar_obtinere(nomina, i), piscina), piscina));
        t       = doc ? scriba_documentum_tabula(doc) : NIHIL;
        ultima  = -I;
        per (linea = ZEPHYRUM; t && linea < (i32)t->altitudo; linea++)
        {
            si (lineam_legere(t, linea, piscina).mensura > ZEPHYRUM)
            {
                ultima = (s32)linea;
            }
        }
        si (ultima < ZEPHYRUM)
        {
            perge;
        }
        si (!prima)
        {
            chorda_aedificator_appendere_literis(a, "\n\n");
        }
        prima = FALSUM;
        per (linea = ZEPHYRUM; (s32)linea <= ultima; linea++)
        {
            si (linea > ZEPHYRUM)
            {
                chorda_aedificator_appendere_character(a, '\n');
            }
            chorda_aedificator_appendere_chorda(a, lineam_legere(t,
                linea,
                piscina));
        }
    }
    *exitus = chorda_aedificator_finire(a);
    redde VERUM;
}
