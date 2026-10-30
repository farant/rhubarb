/* xml_documentum.c - API publica (vide xml_documentum.h) */

#include "xml_documentum.h"
#include "materia_diagnosticum.h"
#include "materia_pictor.h"
#include <string.h>

structura XmlDocumentum {
    XmlCoctum  coctum;
       chorda  textus;
      Piscina* piscina;
};

XmlDocumentum*
xml_documentum_legere (
      chorda  textus,
     Piscina* piscina)
{
    XmlDocumentum* d;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    d = (XmlDocumentum*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(XmlDocumentum));
    si (d == NIHIL)
    {
        redde NIHIL;
    }
    d->textus   = textus;
    d->piscina  = piscina;
    d->coctum  = xml_coquere(piscina, (constans character*)textus.datum,
        (s32)textus.mensura, VERUM);
    si (d->coctum.diagnostica == NIHIL)
    {
        redde NIHIL;
    }
    redde d;
}

b32
xml_documentum_successus (
    constans XmlDocumentum* documentum)
{
    redde documentum != NIHIL && documentum->coctum.bene_formatum;
}

Xar*
xml_documentum_diagnostica (
    constans XmlDocumentum* documentum)
{
    redde documentum != NIHIL ? documentum->coctum.diagnostica : NIHIL;
}

b32
xml_documentum_diagnostica_scribere (
    constans XmlDocumentum* documentum,
        constans character* via,
         ChordaAedificator* exitus)
{
    i32 k;

    si (documentum == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(documentum->coctum.diagnostica);
         k++)
    {
        chorda bloc = materia_pictor_scribere(documentum->piscina,
            (constans MateriaDiagnosticum*)xar_obtinere(
                documentum->coctum.diagnostica, k),
            via != NIHIL ? via : "", "xml",
            (constans character*)documentum->textus.datum,
            documentum->textus.mensura, VERUM);

        si (bloc.mensura == ZEPHYRUM)
        {
            redde FALSUM;
        }
        chorda_aedificator_appendere_chorda(exitus, bloc);
    }
    redde VERUM;
}

constans XmlCoctus*
xml_documentum_radix (
    constans XmlDocumentum* documentum)
{
                   i32  k;
    constans XmlCoctus* d;

    si (documentum == NIHIL || documentum->coctum.documentum == NIHIL)
    {
        redde NIHIL;
    }
    d = documentum->coctum.documentum;
    per (k = ZEPHYRUM; d->liberi != NIHIL && k < xar_numerus(d->liberi);
         k++)
    {
        constans XmlCoctus* f = *(constans XmlCoctus**)xar_obtinere(
            d->liberi, k);

        si (f->genus == XML_COCTUS_ELEMENTUM)
        {
            redde f;
        }
    }
    redde NIHIL;
}

constans XmlCoctum*
xml_documentum_coctum (
    constans XmlDocumentum* documentum)
{
    redde documentum != NIHIL ? &documentum->coctum : NIHIL;
}


/* ==================================================
 * Nodi cocti
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior b32
_titulus_aequat (
    constans XmlCoctus* x,
    constans character* titulus)
{
    i32 n;

    si (x->genus != XML_COCTUS_ELEMENTUM)
    {
        redde FALSUM;
    }
    si (titulus == NIHIL)
    {
        redde VERUM;
    }
    n = (i32)strlen(titulus);
    redde x->titulus.mensura == n
        && memcmp(x->titulus.datum, titulus, (size_t)n) == ZEPHYRUM;
}

XmlCoctusGenus
xml_nodi_genus (
    constans XmlCoctus* nodus)
{
    redde nodus != NIHIL ? nodus->genus : XML_COCTUS_DOCUMENTUM;
}

chorda
xml_titulus (
    constans XmlCoctus* nodus)
{
    redde nodus != NIHIL ? nodus->titulus : _vacua();
}

chorda
xml_elementi_localis (
    constans XmlCoctus* elementum)
{
    redde elementum != NIHIL ? elementum->localis : _vacua();
}

chorda
xml_elementi_spatium (
    constans XmlCoctus* elementum)
{
    redde elementum != NIHIL ? elementum->spatium : _vacua();
}

chorda
xml_textus (
    constans XmlCoctus* nodus)
{
    redde nodus != NIHIL ? nodus->textus : _vacua();
}

i32
xml_liberorum_numerus (
    constans XmlCoctus* nodus)
{
    redde nodus != NIHIL && nodus->liberi != NIHIL
        ? xar_numerus(nodus->liberi) : ZEPHYRUM;
}

constans XmlCoctus*
xml_liberum (
    constans XmlCoctus* nodus,
                   i32  index)
{
    si (   nodus == NIHIL || nodus->liberi == NIHIL
        || index >= xar_numerus(nodus->liberi))
    {
        redde NIHIL;
    }
    redde *(constans XmlCoctus**)xar_obtinere(nodus->liberi, index);
}

/* primum ELEMENTUM titulo dato ab indice 'ab' liberorum parentis */
interior constans XmlCoctus*
_quaerere (
    constans XmlCoctus* parens,
                   i32  ab,
    constans character* titulus)
{
    i32 k;

    per (k = ab; k < xml_liberorum_numerus(parens); k++)
    {
        constans XmlCoctus* f = xml_liberum(parens, k);

        si (_titulus_aequat(f, titulus))
        {
            redde f;
        }
    }
    redde NIHIL;
}

constans XmlCoctus*
xml_filius (
    constans XmlCoctus* elementum,
    constans character* titulus)
{
    redde elementum != NIHIL ? _quaerere(elementum, ZEPHYRUM, titulus)
        : NIHIL;
}

constans XmlCoctus*
xml_frater (
    constans XmlCoctus* nodus,
    constans character* titulus)
{
    si (nodus == NIHIL || nodus->parens == NIHIL)
    {
        redde NIHIL;
    }
    redde _quaerere(nodus->parens, nodus->index + I, titulus);
}

i32
xml_filiorum_numerus (
    constans XmlCoctus* elementum,
    constans character* titulus)
{
    i32 k;
    i32 n = ZEPHYRUM;

    per (k = ZEPHYRUM; k < xml_liberorum_numerus(elementum); k++)
    {
        si (_titulus_aequat(xml_liberum(elementum, k), titulus))
        {
            n++;
        }
    }
    redde n;
}

b32
xml_attributum (
    constans XmlCoctus* elementum,
    constans character* titulus,
                chorda* valor)
{
    i32 k;
    i32 n;

    si (   elementum            == NIHIL || titulus == NIHIL
        || elementum->attributa == NIHIL)
    {
        redde FALSUM;
    }
    n = (i32)strlen(titulus);
    per (k = ZEPHYRUM; k < xar_numerus(elementum->attributa); k++)
    {
        constans XmlAttributumCoctum* a =
            (constans XmlAttributumCoctum*)
            xar_obtinere(elementum->attributa, k);

        si (   a->titulus.mensura                           == n
            && memcmp(a->titulus.datum, titulus, (size_t)n) == ZEPHYRUM)
        {
            si (valor != NIHIL)
            {
                *valor = a->valor;
            }
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_aequat (
                 chorda  c,
     constans character* literae)
{
    i32 n = (i32)strlen(literae);

    redde c.mensura == n && (n == ZEPHYRUM
        || memcmp(c.datum, literae, (size_t)n) == ZEPHYRUM);
}

b32
xml_attributum_spatii (
    constans XmlCoctus* elementum,
    constans character* uri,
    constans character* localis,
                chorda* valor)
{
    i32 k;

    si (   elementum == NIHIL || uri == NIHIL || localis == NIHIL
        || elementum->attributa == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(elementum->attributa); k++)
    {
        constans XmlAttributumCoctum* a =
            (constans XmlAttributumCoctum*)
            xar_obtinere(elementum->attributa, k);

        si (_aequat(a->spatium, uri) && _aequat(a->localis, localis))
        {
            si (valor != NIHIL)
            {
                *valor = a->valor;
            }
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
xml_textus_totus (
    constans XmlCoctus* elementum,
               Piscina* piscina,
                chorda* textus)
{
    nomen structura {
        constans XmlCoctus* x;
                       i32  index;
    } Gradus;

    ChordaAedificator* a;
                  Xar* acervus;
               Gradus* g;

    si (elementum == NIHIL || piscina == NIHIL || textus == NIHIL)
    {
        redde FALSUM;
    }
    a        = chorda_aedificator_creare(piscina, LXIV);
    acervus  = xar_creare(piscina, (i32)magnitudo(Gradus));
    si (a == NIHIL || acervus == NIHIL)
    {
        redde FALSUM;
    }
    g         = (Gradus*)xar_addere(acervus);
    g->x      = elementum;
    g->index  = ZEPHYRUM;
    dum (xar_numerus(acervus) > ZEPHYRUM)
    {
        constans XmlCoctus* f;

        g = (Gradus*)xar_obtinere(acervus, xar_numerus(acervus) - I);
        si (g->index >= xml_liberorum_numerus(g->x))
        {
            xar_removere_ultimum(acervus);
            perge;
        }
        f = xml_liberum(g->x, g->index);
        g->index++;
        si (f->genus == XML_COCTUS_TEXTUS)
        {
            chorda_aedificator_appendere_chorda(a, f->textus);
        }
        alioquin si (f->genus == XML_COCTUS_ELEMENTUM)
        {
            Gradus* h = (Gradus*)xar_addere(acervus);

            si (h == NIHIL)
            {
                redde FALSUM;
            }
            h->x      = f;
            h->index  = ZEPHYRUM;
        }
    }
    *textus = chorda_aedificator_finire(a);
    redde VERUM;
}
