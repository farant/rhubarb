/* xml_oraculum.c - Forma canonica (vide xml_oraculum.h) */

#include "xml_oraculum.h"
#include "xar.h"
#include <string.h>

interior vacuum
_effugere (
    ChordaAedificator* a,
               chorda  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t.mensura; i++)
    {
        commutatio (t.datum[i])
        {
            casus '&':  chorda_aedificator_appendere_literis(a,
                            "&amp;");
                        frange;
            casus '<':  chorda_aedificator_appendere_literis(a, "&lt;");
                        frange;
            casus '>':  chorda_aedificator_appendere_literis(a, "&gt;");
                        frange;
            casus '"':  chorda_aedificator_appendere_literis(a,
                            "&quot;");
                        frange;
            casus '\t': chorda_aedificator_appendere_literis(a, "&#9;");
                        frange;
            casus '\n': chorda_aedificator_appendere_literis(a,
                            "&#10;");
                        frange;
            casus '\r': chorda_aedificator_appendere_literis(a,
                            "&#13;");
                        frange;
            ordinarius:
                chorda_aedificator_appendere_character(a,
                    (character)t.datum[i]);
                frange;
        }
    }
}

interior integer
_comparare_chordas (
    chorda a,
    chorda b)
{
        i32 n = a.mensura < b.mensura ? a.mensura : b.mensura;
    integer r = n > ZEPHYRUM ? memcmp(a.datum, b.datum, (size_t)n)
        : ZEPHYRUM;

    si (r != ZEPHYRUM)
    {
        redde r;
    }
    redde a.mensura < b.mensura ? -I : (a.mensura > b.mensura ? I
        : ZEPHYRUM);
}

/* ordo indicum attributorum per titulos (insertio: pauca) */
interior vacuum
_attributa_scribere (
      ChordaAedificator* a,
     constans XmlCoctus* x,
                Piscina* piscina)
{
    i32 n = x->attributa
        != NIHIL ? xar_numerus(x->attributa) : ZEPHYRUM;
    i32* ordo;
    i32  k;
    i32  j;

    si (n == ZEPHYRUM)
    {
        redde;
    }
    ordo = (i32*)piscina_allocare(piscina,
        (memoriae_index)n * (memoriae_index)magnitudo(i32));
    si (ordo == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans XmlAttributumCoctum* nova = (constans
            XmlAttributumCoctum*)xar_obtinere(x->attributa, k);

        j = k;
        dum (   j > ZEPHYRUM && _comparare_chordas(((constans
            XmlAttributumCoctum*)xar_obtinere(x->attributa, ordo[j
                - I]))
                ->titulus, nova->titulus) > ZEPHYRUM)
        {
            ordo[j] = ordo[j - I];
            j--;
        }
        ordo[j] = k;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans XmlAttributumCoctum* at =
            (constans XmlAttributumCoctum*)
            xar_obtinere(x->attributa, ordo[k]);

        chorda_aedificator_appendere_character(a, ' ');
        chorda_aedificator_appendere_chorda(a, at->titulus);
        chorda_aedificator_appendere_literis(a, "=\"");
        _effugere(a, at->valor);
        chorda_aedificator_appendere_character(a, '"');
    }
}

/* nodus coctus (et subarbor) iterative */
interior vacuum
_nodum_scribere (
      ChordaAedificator* a,
     constans XmlCoctus* radix,
                Piscina* piscina)
{
    nomen structura {
        constans XmlCoctus* x;
                       i32  index;
    } Gradus;

    Xar* acervus = xar_creare(piscina, (i32)magnitudo(Gradus));
    Gradus* g;

    si (acervus == NIHIL)
    {
        redde;
    }
    g         = (Gradus*)xar_addere(acervus);
    g->x      = radix;
    g->index  = (i32)-I;
    dum (xar_numerus(acervus) > ZEPHYRUM)
    {
        constans XmlCoctus* x;

        g = (Gradus*)xar_obtinere(acervus, xar_numerus(acervus) - I);
        x = g->x;
        si (g->index == (i32)-I)
        {
            commutatio (x->genus)
            {
                casus XML_COCTUS_TEXTUS:
                    _effugere(a, x->textus);
                    xar_removere_ultimum(acervus);
                    perge;
                casus XML_COCTUS_INSTRUCTIO:
                    chorda_aedificator_appendere_literis(a, "<?");
                    chorda_aedificator_appendere_chorda(a, x->titulus);
                    chorda_aedificator_appendere_character(a, ' ');
                    chorda_aedificator_appendere_chorda(a, x->textus);
                    chorda_aedificator_appendere_literis(a, "?>");
                    xar_removere_ultimum(acervus);
                    perge;
                casus XML_COCTUS_ELEMENTUM:
                    chorda_aedificator_appendere_character(a, '<');
                    chorda_aedificator_appendere_chorda(a, x->titulus);
                    _attributa_scribere(a, x, piscina);
                    chorda_aedificator_appendere_character(a, '>');
                    frange;
                ordinarius:
                    /* commentarium: forma canonica nulla */
                    xar_removere_ultimum(acervus);
                    perge;
            }
        }
        g->index++;
        si (x->liberi != NIHIL && g->index < xar_numerus(x->liberi))
        {
            Gradus* h = (Gradus*)xar_addere(acervus);

            si (h == NIHIL)
            {
                redde;
            }
            h->x = *(constans XmlCoctus**)xar_obtinere(x->liberi,
                g->index);
            h->index = (i32)-I;
            perge;
        }
        chorda_aedificator_appendere_literis(a, "</");
        chorda_aedificator_appendere_chorda(a, x->titulus);
        chorda_aedificator_appendere_character(a, '>');
        xar_removere_ultimum(acervus);
    }
}

interior vacuum
_notationes_scribere (
      ChordaAedificator* a,
     constans XmlCoctum* coctum,
                Piscina* piscina)
{
    i32  n = xar_numerus(coctum->notationes);
    i32* ordo;
    i32  k;
    i32  j;

    ordo = (i32*)piscina_allocare(piscina,
        (memoriae_index)n * (memoriae_index)magnitudo(i32));
    si (ordo == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        chorda t =
            ((constans XmlNotatio*)xar_obtinere(coctum->notationes,
            k))->titulus;

        j = k;
        dum (j > ZEPHYRUM && _comparare_chordas(((constans XmlNotatio*)
            xar_obtinere(coctum->notationes, ordo[j - I]))->titulus, t)
                > ZEPHYRUM)
        {
            ordo[j] = ordo[j - I];
            j--;
        }
        ordo[j] = k;
    }
    chorda_aedificator_appendere_literis(a, "<!DOCTYPE ");
    chorda_aedificator_appendere_chorda(a, coctum->titulus_typi);
    chorda_aedificator_appendere_literis(a, " [\n");
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans XmlNotatio* nt = (constans XmlNotatio*)xar_obtinere(
            coctum->notationes, ordo[k]);

        chorda_aedificator_appendere_literis(a, "<!NOTATION ");
        chorda_aedificator_appendere_chorda(a, nt->titulus);
        si (nt->habet_publicum)
        {
            chorda_aedificator_appendere_literis(a, " PUBLIC '");
            chorda_aedificator_appendere_chorda(a, nt->publicum);
            chorda_aedificator_appendere_character(a, '\'');
            si (nt->habet_systema)
            {
                chorda_aedificator_appendere_literis(a, " '");
                chorda_aedificator_appendere_chorda(a, nt->systema);
                chorda_aedificator_appendere_character(a, '\'');
            }
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(a, " SYSTEM '");
            chorda_aedificator_appendere_chorda(a, nt->systema);
            chorda_aedificator_appendere_character(a, '\'');
        }
        chorda_aedificator_appendere_literis(a, ">\n");
    }
    chorda_aedificator_appendere_literis(a, "]>\n");
}

b32
xml_oraculum_scribere (
     constans XmlCoctum* coctum,
                Piscina* piscina,
      ChordaAedificator* exitus)
{
                   i32  n;
                   i32  k;
    constans XmlCoctus* d;

    si (   coctum == NIHIL || coctum->documentum == NIHIL
        || exitus == NIHIL)
    {
        redde FALSUM;
    }
    d = coctum->documentum;
    n = d->liberi != NIHIL ? xar_numerus(d->liberi) : ZEPHYRUM;
    /* forma secunda: notationes loco DOCTYPI (locus_typi: post
     * instructiones partis internae) */
    per (k = ZEPHYRUM; k <= n; k++)
    {
        si (   (s32)k == coctum->locus_typi
            && xar_numerus(coctum->notationes) > ZEPHYRUM)
        {
            _notationes_scribere(exitus, coctum, piscina);
        }
        si (k < n)
        {
            _nodum_scribere(exitus, *(constans XmlCoctus**)xar_obtinere(
                d->liberi, k), piscina);
        }
    }
    redde VERUM;
}
