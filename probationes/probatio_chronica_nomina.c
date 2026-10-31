/* probatio_chronica_nomina.c - declarationes personarum et locorum
 * (chronica C5a): simplex, clausura anonyma, involucrum fragmentis
 * anonymis, capturae anticae, attributa involucri et propria, in
 * prosa (signa '<' '&' circa), nomen per lineas cum accentibus,
 * vitia (mixta, non clausum, nomen vacuum, captura summa, STML),
 * regio, minuscula. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "internamentum.h"
#include "chronica_eventus.h"
#include "chronica_nomina.h"
#include "credo.h"

/* lineae ex literis (n) -> Xar de chorda */
interior Xar*
lineas_facere (
               Piscina* p,
    constans character* textus[],
                   i32  n)
{
    Xar* x = xar_creare(p, (i32)magnitudo(chorda));
    i32  i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        *(chorda*)xar_addere(x) = chorda_ex_literis(textus[i], p);
    }
    redde x;
}

/* declarationes linearum; vitia in *vitia */
interior Xar*
legere (
               Piscina*  p,
    constans character*  textus[],
                   i32   n,
                   Xar** vitia)
{
    Xar* d = xar_creare(p, (i32)magnitudo(ChronicaDeclaratio));
    Xar* v = xar_creare(p, (i32)magnitudo(ChronicaVitium));

    si (!chronica_nomina_legere(lineas_facere(p, textus, n), p,
            internamentum_creare(p), d, v))
    {
        redde NIHIL;
    }
    *vitia = v;
    redde d;
}

interior constans ChronicaDeclaratio*
decl (
    Xar* d,
    i32  i)
{
    redde (constans ChronicaDeclaratio*)xar_obtinere(d, i);
}

interior constans ChronicaVitium*
vitium (
    Xar* v,
    i32  i)
{
    redde (constans ChronicaVitium*)xar_obtinere(v, i);
}

/* valor attributi declarationis; vacua si abest */
interior b32
attributum_est (
    constans ChronicaDeclaratio* d,
             constans character* titulus,
             constans character* valor)
{
    i32 i;

    per (i = ZEPHYRUM; d->attributa && i < xar_numerus(d->attributa);
         i++)
    {
        constans ChronicaAttributum* a = (constans ChronicaAttributum*)
            xar_obtinere(d->attributa, i);

        si (chorda_aequalis_literis(a->titulus, titulus))
        {
            redde chorda_aequalis_literis(a->valor, valor);
        }
    }
    redde FALSUM;
}

interior i32
attributa_numerus (
    constans ChronicaDeclaratio* d)
{
    redde d->attributa ? xar_numerus(d->attributa) : ZEPHYRUM;
}

interior vacuum
_sectio_simplex (
    CredoContextus* c)
{
     constans character* t[] = { "<person>Moses</person>" };
                    Xar* x;
                    Xar* v;

    x = legere(c->piscina, t, I, &v);
    CREDO_NECESSE(x != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(v), ZEPHYRUM);
    CREDO_NECESSE(xar_numerus(x) == I);
    CREDO_VERUM(decl(x, ZEPHYRUM)->genus == CHRONICA_PERSONA);
    CREDO_CHORDA_AEQUALIS_LITERIS(decl(x, ZEPHYRUM)->titulus, "Moses");
    CREDO_AEQUALIS_I32(decl(x, ZEPHYRUM)->linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(decl(x, ZEPHYRUM)->columna, ZEPHYRUM);
    CREDO_AEQUALIS_I32(decl(x, ZEPHYRUM)->regio_linea_finis, ZEPHYRUM);
    CREDO_AEQUALIS_I32(decl(x, ZEPHYRUM)->regio_columna_finis, XXII);
}

interior vacuum
_sectio_anonyma (
    CredoContextus* c)
{
     constans character* t[] = { "<place>Lyon</>" };
                    Xar* x;
                    Xar* v;

    x = legere(c->piscina, t, I, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    CREDO_VERUM(decl(x, ZEPHYRUM)->genus == CHRONICA_LOCUS);
    CREDO_CHORDA_AEQUALIS_LITERIS(decl(x, ZEPHYRUM)->titulus, "Lyon");
    CREDO_AEQUALIS_I32(xar_numerus(v), ZEPHYRUM);
}

/* fragmenta anonyma = instantiae parentis; linea cuiusque sua */
interior vacuum
_sectio_involucrum (
    CredoContextus* c)
{
    constans character* t[] = { "<person>", "  <>John</>",
        "  <>Greg</>", "  </>" };
                    Xar* x;
                    Xar* v;

    x = legere(c->piscina, t, IV, &v);
    CREDO_NECESSE(x != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(v), ZEPHYRUM);
    CREDO_NECESSE(xar_numerus(x) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(decl(x, ZEPHYRUM)->titulus, "John");
    CREDO_CHORDA_AEQUALIS_LITERIS(decl(x, I)->titulus, "Greg");
    CREDO_AEQUALIS_I32(decl(x, ZEPHYRUM)->linea, I);
    CREDO_AEQUALIS_I32(decl(x, I)->linea, II);
    CREDO_AEQUALIS_I32(decl(x, I)->regio_linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(decl(x, I)->regio_linea_finis, III);
    CREDO_AEQUALIS_I32(decl(x, I)->regio_columna_finis, V);
}

/* capturae anticae: idem ac fragmenta anonyma */
interior vacuum
_sectio_capturae (
    CredoContextus* c)
{
    constans character* t[] = { "<person>", "  <(>John", "  <(>Greg",
        "  </>" };
                    Xar* x;
                    Xar* v;

    x = legere(c->piscina, t, IV, &v);
    CREDO_NECESSE(x != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(v), ZEPHYRUM);
    CREDO_NECESSE(xar_numerus(x) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(decl(x, ZEPHYRUM)->titulus, "John");
    CREDO_CHORDA_AEQUALIS_LITERIS(decl(x, I)->titulus, "Greg");
    CREDO_AEQUALIS_I32(decl(x, ZEPHYRUM)->linea, I);
    CREDO_AEQUALIS_I32(decl(x, I)->linea, II);
}

/* attributa involucri descendunt; proprium vincit */
interior vacuum
_sectio_attributa (
    CredoContextus* c)
{
    constans character* t[] = { "<person born=\"1225\">",
        "  <>Thomas</>", "  <# born=\"1221\">Bonaventura</>", "</>" };
                    Xar* x;
                    Xar* v;

    x = legere(c->piscina, t, IV, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == II);
    CREDO_AEQUALIS_I32(xar_numerus(v), ZEPHYRUM);
    CREDO_VERUM(attributum_est(decl(x, ZEPHYRUM), "born", "1225"));
    CREDO_VERUM(attributum_est(decl(x, I), "born", "1221"));
    CREDO_AEQUALIS_I32(attributa_numerus(decl(x, I)), I);
}

/* in prosa: positio, signa '<' et '&' circa non nocent */
interior vacuum
_sectio_in_prosa (
    CredoContextus* c)
{
    constans character* t[] = {
        "x < y: legi de <person>Moses</person> & a < b." };
                    Xar* x;
                    Xar* v;

    x = legere(c->piscina, t, I, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    CREDO_AEQUALIS_I32(xar_numerus(v), ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(decl(x, ZEPHYRUM)->titulus, "Moses");
    CREDO_AEQUALIS_I32(decl(x, ZEPHYRUM)->columna, XV);
    CREDO_AEQUALIS_I32(decl(x, ZEPHYRUM)->regio_columna, XV);
    CREDO_AEQUALIS_I32(decl(x, ZEPHYRUM)->regio_columna_finis, XXXVII);
}

/* nomen per lineas, spatia collapsa, accentus ad octetum */
interior vacuum
_sectio_multilinea (
    CredoContextus* c)
{
    constans character* t[] = { "<person>Tom\303\241s", "",
        "   de Aquino</person>" };
                    Xar* x;
                    Xar* v;

    x = legere(c->piscina, t, III, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS(decl(x, ZEPHYRUM)->titulus,
        "Tom\303\241s de Aquino");
    CREDO_AEQUALIS_I32(decl(x, ZEPHYRUM)->regio_linea_finis, II);
}

/* vitia: regio tota; regio vitiosa nihil dat; sequens legitur */
interior vacuum
_sectio_vitia (
    CredoContextus* c)
{
    constans character* mixta[] = { "<person>", "  <>John</>",
        "  textus", "</>", "<person>Moses</person>" };
    constans character* aperta[] = { "<person>Moses",
        "<event year=\"1\">x</event>" };
     constans character* vacuum_nomen[]  = { "<person> </person>" };
     constans character* captura[]       = { "<person (>Moses" };
     constans character* stml[]          = { "<person>Moses</place>" };
                    Xar* x;
                    Xar* v;

    CREDO_GRADUS("distributio mixta; sequens legitur");
    x = legere(c->piscina, mixta, V, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(v) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS(vitium(v, ZEPHYRUM)->causa,
        "distributio mixta");
    CREDO_AEQUALIS_I32(vitium(v, ZEPHYRUM)->linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(vitium(v, ZEPHYRUM)->linea_finis, III);
    CREDO_NECESSE(xar_numerus(x) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS(decl(x, ZEPHYRUM)->titulus, "Moses");

    CREDO_GRADUS("non clausum: ad proximum <event");
    x = legere(c->piscina, aperta, II, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(v) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS(vitium(v, ZEPHYRUM)->causa,
        "non clausum");
    CREDO_AEQUALIS_I32(vitium(v, ZEPHYRUM)->linea_finis, I);
    CREDO_AEQUALIS_I32(vitium(v, ZEPHYRUM)->columna_finis, ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);

    CREDO_GRADUS("nomen vacuum");
    x = legere(c->piscina, vacuum_nomen, I, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(v) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS(vitium(v, ZEPHYRUM)->causa,
        "nomen vacuum");
    CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);

    CREDO_GRADUS("captura summa: forma non admissa");
    x = legere(c->piscina, captura, I, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(v) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS(vitium(v, ZEPHYRUM)->causa,
        "forma non admissa");

    CREDO_GRADUS("STML recusat: causa nominatur");
    x = legere(c->piscina, stml, I, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(v) == I);
    CREDO_VERUM(chorda_incipit(vitium(v, ZEPHYRUM)->causa,
        chorda_ex_literis("STML", c->piscina)));
    CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);
}

/* "Person", "people", "persona" non sunt tags */
interior vacuum
_sectio_minuscula (
    CredoContextus* c)
{
    constans character* t[] = {
        "<Person>X</Person> <people>Y</people> <persona>Z</persona>" };
                    Xar* x;
                    Xar* v;

    x = legere(c->piscina, t, I, &v);
    CREDO_NECESSE(x != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(v), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "simplex", _sectio_simplex, NIHIL, NIHIL, NIHIL, NIHIL },
    { "clausura anonyma", _sectio_anonyma, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "involucrum", _sectio_involucrum, NIHIL, NIHIL, NIHIL, NIHIL },
    { "capturae", _sectio_capturae, NIHIL, NIHIL, NIHIL, NIHIL },
    { "attributa", _sectio_attributa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "in prosa", _sectio_in_prosa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multilinea", _sectio_multilinea, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "minuscula", _sectio_minuscula, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("chronica_nomina", SECTIONES);
}
