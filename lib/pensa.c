/* pensa.c - index cotidianus (include/pensa.h) */
#include "pensa.h"
#include "chorda_aedificator.h"
#include "stml.h"
#include "tabula_dispersa.h"
#include "scriba_liber.h"

#include <string.h>

#define TABULATIO  IV     /* '\t' = IV columnae indentationis */

nomen structura {
    chorda textus;        /* spatiis extremis demptis; vacua = linea
                           * vacua (sectiones separat) */
       i32 indentatio;    /* columnae (tab = TABULATIO) */
} LineaFormae;

/* spatia, tabulae et '\r' extrema dempta */
interior chorda
purgare (
    chorda c)
{
    dum (   c.mensura > ZEPHYRUM
         && (c.datum[ZEPHYRUM] == ' ' || c.datum[ZEPHYRUM] == '\t'))
    {
        c.datum++;
        c.mensura--;
    }
    dum (   c.mensura > ZEPHYRUM && (c.datum[c.mensura - I] == ' '
        || c.datum[c.mensura - I] == '\t'
        || c.datum[c.mensura - I] == '\r'))
    {
        c.mensura--;
    }
    redde c;
}

interior PensaGrex*
gregem_addere (
     PensaForma* forma,
         chorda  schedula,
        Piscina* piscina)
{
    PensaGrex* g;

    g = (PensaGrex*)xar_addere(forma->greges);
    si (!g)
    {
        redde NIHIL;
    }
    memset(g, ZEPHYRUM, magnitudo(PensaGrex));
    g->schedula  = schedula;
    g->res       = xar_creare(piscina, (i32)magnitudo(chorda));
    g->recursus  = recursus_legere(schedula.mensura
        > ZEPHYRUM ? schedula
        : chorda_ex_literis("every day", piscina), piscina, &g->vitium);
    redde g;
}

interior constans LineaFormae*
linea_ad (
     Xar* lineae,
     i32  k)
{
    redde (constans LineaFormae*)xar_obtinere(lineae, k);
}

/* sectio [a, b): dedentatio (indentatio minima), deinde linea summa cum
 * liberis = caput, sine liberis = res cotidiana */
interior vacuum
sectionem_legere (
    PensaForma* forma,
           Xar* lineae,
           i32  a,
           i32  b,
       Piscina* piscina)
{
    PensaGrex* g;
    PensaGrex* cotidianus;
          i32  minima;
          i32  k;
          i32  j;
          i32  h;
       chorda  vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    minima         = linea_ad(lineae, a)->indentatio;
    per (k = a + I; k < b; k++)
    {
        si (linea_ad(lineae, k)->indentatio < minima)
        {
            minima = linea_ad(lineae, k)->indentatio;
        }
    }
    cotidianus  = NIHIL;
    k           = a;
    dum (k < b)
    {
        j = k + I;
        dum (j < b && linea_ad(lineae, j)->indentatio > minima)
        {
            j++;
        }
        si (j > k + I)
        {
            g = gregem_addere(forma, linea_ad(lineae, k)->textus,
                piscina);
            per (h = k + I; g && h < j; h++)
            {
                *(chorda*)xar_addere(g->res) = linea_ad(lineae,
                    h)->textus;
            }
            cotidianus = NIHIL;
        }
        alioquin
        {
            si (!cotidianus)
            {
                cotidianus = gregem_addere(forma, vacua, piscina);
            }
            si (cotidianus)
            {
                *(chorda*)xar_addere(cotidianus->res) =
                    linea_ad(lineae, k)->textus;
            }
        }
        k = j;
    }
}

PensaForma*
pensa_formam_legere (
      chorda  textus,
     Piscina* piscina)
{
      PensaForma* forma;
             Xar* lineae;
     LineaFormae* l;
          chorda  linea;
             i32  i;
             i32  k;
             i32  a;
             i32  initium;
             i32  columnae;

    si (!piscina)
    {
        redde NIHIL;
    }
    forma = (PensaForma*)piscina_allocare(piscina,
        magnitudo(PensaForma));
    si (!forma)
    {
        redde NIHIL;
    }
    forma->greges  = xar_creare(piscina, (i32)magnitudo(PensaGrex));
    lineae         = xar_creare(piscina, (i32)magnitudo(LineaFormae));
    si (!forma->greges || !lineae)
    {
        redde NIHIL;
    }
    i = ZEPHYRUM;
    dum (i < textus.mensura)
    {
        initium = i;
        dum (i < textus.mensura && textus.datum[i] != '\n')
        {
            i++;
        }
        linea.datum    = textus.datum + initium;
        linea.mensura  = i - initium;
        i++;
        l = (LineaFormae*)xar_addere(lineae);
        si (!l)
        {
            redde NIHIL;
        }
        columnae = ZEPHYRUM;
        per (k = ZEPHYRUM; k < linea.mensura && (linea.datum[k] == ' '
             || linea.datum[k] == '\t'); k++)
        {
            columnae += linea.datum[k] == '\t' ? TABULATIO : I;
        }
        l->indentatio  = columnae;
        l->textus      = purgare(linea);
    }
    a = ZEPHYRUM;
    per (k = ZEPHYRUM; k <= xar_numerus(lineae); k++)
    {
        si (   k == xar_numerus(lineae)
            || linea_ad(lineae, k)->textus.mensura == ZEPHYRUM)
        {
            si (k > a)
            {
                sectionem_legere(forma, lineae, a, k, piscina);
            }
            a = k + I;
        }
    }
    redde forma;
}

Xar*
pensa_res_diei (
    constans PensaForma* forma,
                   Dies  d,
                Piscina* piscina)
{
                   Xar* exitus;
    constans PensaGrex* g;
                chorda  r;
                   b32  iterata;
                   i32  i;
                   i32  k;
                   i32  m;

    exitus = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; exitus && forma
        && i < xar_numerus(forma->greges);
         i++)
    {
        g = (constans PensaGrex*)xar_obtinere(forma->greges, i);
        si (!g->recursus || !recursus_continet(g->recursus, d))
        {
            perge;
        }
        per (k = ZEPHYRUM; k < xar_numerus(g->res); k++)
        {
            r        = *(chorda*)xar_obtinere(g->res, k);
            iterata  = FALSUM;
            per (m = ZEPHYRUM; m < xar_numerus(exitus); m++)
            {
                si (chorda_aequalis(*(chorda*)xar_obtinere(exitus, m),
                    r))
                {
                    iterata = VERUM;
                }
            }
            si (!iterata)
            {
                *(chorda*)xar_addere(exitus) = r;
            }
        }
    }
    redde exitus;
}

/* forma ex libro "pensa" (paginae ordine, lineis vacuis servatis);
 * liber absens = textus vacuus */
chorda
pensa_textum_legere (
                Volumen* volumen,
    InternamentumChorda* intern,
                Piscina* piscina)
{
    chorda textus;

    textus.datum    = NIHIL;
    textus.mensura  = ZEPHYRUM;
    (vacuum)scriba_liber_textum_legere(piscina, intern, volumen,
        "pensa",
        &textus);
    redde textus;
}


/* ==================================================
 * Signa
 * ================================================== */

structura PensaDocumentum {
                Piscina* piscina;
    InternamentumChorda* intern;
                Volumen* volumen;
         TabulaDispersa* signa;    /* clavis -> &signum_factum aut
                                    * &signum_infectum */
                    s64 seq;      /* actum ultimum lectum */
};

hic_manens constans character* constans GENERA_PENSORUM[] = {
    "pensa", NIHIL
};

/* valores tabulae: inscriptiones duae distinctae */
hic_manens character signum_factum;
hic_manens character signum_infectum;

/* "YYYY-MM-DD\x1f<res>" */
interior chorda
clavis_signi (
       Dies  d,
     chorda  res,
    Piscina* piscina)
{
    redde chorda_concatenare(chorda_concatenare(fasti_formare_diem(d,
        FASTI_FORMA_ISO, piscina), chorda_ex_literis("\x1f", piscina),
        piscina), res, piscina);
}

/* acta nova (post doc->seq) in tabulam */
interior vacuum
acta_legere (
    PensaDocumentum* doc)
{
              Xar* acta;
     VolumenActum* actum;
     StmlResultus  res;
           chorda* actio;
           chorda* dies;
             Dies  d;
           chorda  textus;
              i32  i;

    acta = volumen_acta_generum_legere(doc->volumen, doc->seq,
        GENERA_PENSORUM, doc->piscina);
    per (i = ZEPHYRUM; acta && i < xar_numerus(acta); i++)
    {
        actum = (VolumenActum*)xar_obtinere(acta, i);
        doc->seq = actum->seq;
        res = stml_legere(actum->datum, doc->piscina, doc->intern);
        si (!res.successus || !res.elementum_radix)
        {
            perge;
        }
        actio  = stml_attributum_capere(res.elementum_radix, "actio");
        dies   = stml_attributum_capere(res.elementum_radix, "dies");
        textus  = purgare(stml_textus_internus(res.elementum_radix,
            doc->piscina));
        si (   !actio || !dies || textus.mensura == ZEPHYRUM
            || !fasti_parsare_iso(*dies, &d))
        {
            perge;
        }
        (vacuum)tabula_dispersa_inserere(doc->signa, clavis_signi(d,
            textus, doc->piscina),
            chorda_aequalis_literis(*actio, "factum")
            ? (vacuum*)&signum_factum : (vacuum*)&signum_infectum);
    }
}

PensaDocumentum*
pensa_aperire (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen)
{
    PensaDocumentum* doc;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    doc = (PensaDocumentum*)piscina_allocare(piscina,
        magnitudo(PensaDocumentum));
    si (!doc)
    {
        redde NIHIL;
    }
    memset(doc, ZEPHYRUM, magnitudo(PensaDocumentum));
    doc->piscina  = piscina;
    doc->intern   = intern;
    doc->volumen  = volumen;
    doc->signa    = tabula_dispersa_creare_chorda(piscina, LXIV);
    si (!doc->signa)
    {
        redde NIHIL;
    }
    acta_legere(doc);
    redde doc;
}

b32
pensa_factum (
    PensaDocumentum* doc,
               Dies  d,
             chorda  res)
{
    vacuum* valor;

    res = purgare(res);
    si (!doc || res.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    acta_legere(doc);
    redde tabula_dispersa_invenire(doc->signa, clavis_signi(d, res,
        doc->piscina), &valor) && valor == (vacuum*)&signum_factum;
}

b32
pensa_ponere (
    PensaDocumentum* doc,
               Dies  d,
             chorda  res,
                b32  factum)
{
    StmlNodus* n;
    StmlNodus* t;

    res = purgare(res);
    si (!doc || res.mensura == ZEPHYRUM || !fasti_dies_valida(d))
    {
        redde FALSUM;
    }
    si (pensa_factum(doc, d, res) == (factum ? VERUM : FALSUM))
    {
        redde VERUM;
    }
    n = stml_elementum_creare(doc->piscina, doc->intern, "pensum");
    t = stml_textum_creare_ex_chorda(doc->piscina, doc->intern, res);
    si (   !n || !t
        || !stml_attributum_addere(n, doc->piscina, doc->intern,
        "actio",
               factum ? "factum" : "infectum")
        || !stml_attributum_addere_chorda(n, doc->piscina, doc->intern,
               "dies", fasti_formare_diem(d, FASTI_FORMA_ISO,
               doc->piscina))
        || !stml_liberum_addere(n, t)
        || volumen_actum_appendere(doc->volumen, "pensa",
               stml_scribere(n, doc->piscina, FALSUM)) <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    acta_legere(doc);
    redde VERUM;
}
