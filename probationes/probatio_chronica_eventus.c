/* probatio_chronica_eventus.c - eventa ex lineis paginae (chronica
 * C1): simplex, clausura anonyma, anni a.C., attributa, eventa per
 * lineas et in prosa, lineae vacuae numeratae, vitia cum positionibus,
 * nomina tonsa ad octetum, signa '<' '>' in textu, tag minusculus. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "chronica_eventus.h"
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

/* eventa linearum; vitia in *vitia (NIHIL licet) */
interior Xar*
legere (
               Piscina*  p,
    constans character*  textus[],
                   i32   n,
                   Xar** vitia)
{
    Xar* eventa  = xar_creare(p, (i32)magnitudo(ChronicaEventus));
    Xar* v       = xar_creare(p, (i32)magnitudo(ChronicaVitium));

    si (!chronica_eventa_legere(lineas_facere(p, textus, n), p, eventa,
            v))
    {
        redde NIHIL;
    }
    si (vitia)
    {
        *vitia = v;
    }
    redde eventa;
}

interior constans ChronicaEventus*
eventum (
    Xar* eventa,
    i32  i)
{
    redde (constans ChronicaEventus*)xar_obtinere(eventa, i);
}

interior constans ChronicaVitium*
vitium (
    Xar* vitia,
    i32  i)
{
    redde (constans ChronicaVitium*)xar_obtinere(vitia, i);
}

interior vacuum
_sectio_simplex (
    CredoContextus* c)
{
    constans character* t[] = {
        "<event year=\"1274\">Aquinas dies</event>" };
                    Xar* x;
                    Xar* v;

    x = legere(c->piscina, t, I, &v);
    CREDO_NECESSE(x != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(x), I);
    CREDO_AEQUALIS_I32(xar_numerus(v), ZEPHYRUM);
    CREDO_AEQUALIS_S32(eventum(x, ZEPHYRUM)->annus, MCCLXXIV);
    CREDO_AEQUALIS_S32(eventum(x, ZEPHYRUM)->mensis, ZEPHYRUM);
    CREDO_AEQUALIS_S32(eventum(x, ZEPHYRUM)->dies, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(eventum(x, ZEPHYRUM)->descriptio,
        "Aquinas dies");
    CREDO_AEQUALIS_I32(xar_numerus(eventum(x, ZEPHYRUM)->personae),
        ZEPHYRUM);
    CREDO_CHORDA_VACUA(eventum(x, ZEPHYRUM)->locus);
    CREDO_CHORDA_VACUA(eventum(x, ZEPHYRUM)->genus);
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->columna, ZEPHYRUM);
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->linea_finis, ZEPHYRUM);
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->columna_finis, XXXIX);
}

interior vacuum
_sectio_anonyma (
    CredoContextus* c)
{
    constans character* t[] = {
        "<event year=\"1274\">Aquinas dies</>" };
                    Xar* x;

    x = legere(c->piscina, t, I, NIHIL);
    CREDO_NECESSE(x != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(x), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(eventum(x, ZEPHYRUM)->descriptio,
        "Aquinas dies");
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->columna_finis, XXXIV);
}

interior vacuum
_sectio_ante_christum (
    CredoContextus* c)
{
    constans character* t[] = {
        "<event year=\"-1446\">Exodus</event>" };
                    Xar* x;

    x = legere(c->piscina, t, I, NIHIL);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    CREDO_AEQUALIS_S32(eventum(x, ZEPHYRUM)->annus, -MCDXLVI);
}

interior vacuum
_sectio_attributa (
    CredoContextus* c)
{
    constans character* t[] = {
        "<event year=\"-1446\" month=\"5\" day=\"1\" people=\"Moses; "
        "Aaron\" place=\"Egypt\" type=\"exodus\" source=\"Ex 12\">"
        "Moses ducit</event>" };
           constans ChronicaEventus* e;
                                Xar* x;

    x = legere(c->piscina, t, I, NIHIL);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    e = eventum(x, ZEPHYRUM);
    CREDO_AEQUALIS_S32(e->mensis, V);
    CREDO_AEQUALIS_S32(e->dies, I);
    CREDO_NECESSE(xar_numerus(e->personae) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(e->personae,
        ZEPHYRUM), "Moses");
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(e->personae,
        I), "Aaron");
    CREDO_CHORDA_AEQUALIS_LITERIS(e->locus, "Egypt");
    CREDO_CHORDA_AEQUALIS_LITERIS(e->genus, "exodus");
    CREDO_CHORDA_AEQUALIS_LITERIS(e->descriptio, "Moses ducit");
}

interior vacuum
_sectio_multilinea (
    CredoContextus* c)
{
    constans character* t[] = {
        "Prosa.",
        "<event year=\"-1446\">Moses",
        "   leads   the",
        "Exodus</event> post" };
           constans ChronicaEventus* e;
                                Xar* x;

    x = legere(c->piscina, t, IV, NIHIL);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    e = eventum(x, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(e->descriptio,
        "Moses leads the Exodus");
    CREDO_AEQUALIS_I32(e->linea, I);
    CREDO_AEQUALIS_I32(e->columna, ZEPHYRUM);
    CREDO_AEQUALIS_I32(e->linea_finis, III);
    CREDO_AEQUALIS_I32(e->columna_finis, XIV);
}

interior vacuum
_sectio_in_prosa (
    CredoContextus* c)
{
    constans character* t[] = {
        "Hodie legi. <event year=\"1225\">Thomas nascitur</event> "
        "Plura. <event year=\"1274\">obit</event>" };
                    Xar* x;

    x = legere(c->piscina, t, I, NIHIL);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == II);
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->columna, XII);
    CREDO_AEQUALIS_S32(eventum(x, ZEPHYRUM)->annus, MCCXXV);
    CREDO_CHORDA_AEQUALIS_LITERIS(eventum(x, ZEPHYRUM)->descriptio,
        "Thomas nascitur");
    CREDO_AEQUALIS_S32(eventum(x, I)->annus, MCCLXXIV);
    CREDO_CHORDA_AEQUALIS_LITERIS(eventum(x, I)->descriptio, "obit");
}

interior vacuum
_sectio_lineae_vacuae (
    CredoContextus* c)
{
    constans character* t[] = {
        "Titulus",
        "",
        "",
        "  <event year=\"1\">x</event>" };
                    Xar* x;

    x = legere(c->piscina, t, IV, NIHIL);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->linea, III);
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->columna, II);
}

/* eventum malum unum: nullum eventum, vitium unum cum causa */
interior vacuum
vitium_solum (
              CredoContextus* c,
         constans character*  linea,
         constans character*  causa)
{
     constans character* t[I];
                    Xar* x;
                    Xar* v;

    t[ZEPHYRUM]  = linea;
    x            = legere(c->piscina, t, I, &v);
    CREDO_NECESSE(x != NIHIL);
    CREDO_AEQUALIS_I32(xar_numerus(x), ZEPHYRUM);
    CREDO_NECESSE(xar_numerus(v) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS(vitium(v, ZEPHYRUM)->causa, causa);
    CREDO_AEQUALIS_I32(vitium(v, ZEPHYRUM)->linea, ZEPHYRUM);
    CREDO_AEQUALIS_I32(vitium(v, ZEPHYRUM)->columna, ZEPHYRUM);
}

interior vacuum
_sectio_vitia (
    CredoContextus* c)
{
    constans character* t[] = {
        "<event month=\"3\">sine anno</event>",
        "ibi <event year=\"1\">bonum</event>" };
                    Xar* x;
                    Xar* v;

    vitium_solum(c, "<event>sine anno</event>", "annus deest");
    vitium_solum(c, "<event year=\"0\">x</event>", "annus invalidus");
    vitium_solum(c, "<event year=\"abc\">x</event>",
        "annus invalidus");
    vitium_solum(c, "<event year=\"1\" month=\"13\">x</event>",
        "mensis invalidus");
    vitium_solum(c, "<event year=\"1\" day=\"4\">x</event>",
        "dies sine mense");
    vitium_solum(c, "<event year=\"1\" month=\"2\" day=\"40\">x</>",
        "dies invalidus");
    vitium_solum(c, "<event year=\"1\">numquam clausum",
        "non clausum");

    /* bonum post malum legitur; ambitus mali usque ad clausuram */
    x = legere(c->piscina, t, II, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->linea, I);
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->columna, IV);
    CREDO_NECESSE(xar_numerus(v) == I);
    CREDO_AEQUALIS_I32(vitium(v, ZEPHYRUM)->linea_finis, ZEPHYRUM);
    CREDO_AEQUALIS_I32(vitium(v, ZEPHYRUM)->columna_finis, XXXIV);
}

interior vacuum
_sectio_nomina (
    CredoContextus* c)
{
    constans character* t[] = {
        "<event year=\"1274\" people=\" Moses ;; Tom\303\241s de "
        "Aquino \" place=\"  Fossanova \">x</event>" };
           constans ChronicaEventus* e;
                                Xar* x;

    x = legere(c->piscina, t, I, NIHIL);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    e = eventum(x, ZEPHYRUM);
    CREDO_NECESSE(xar_numerus(e->personae) == II);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(e->personae,
        ZEPHYRUM), "Moses");
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(e->personae,
        I), "Tom\303\241s de Aquino");
    CREDO_CHORDA_AEQUALIS_LITERIS(e->locus, "Fossanova");
}

interior vacuum
_sectio_signa (
    CredoContextus* c)
{
    constans character* t1[] = {
        "<event year=\"1\">a < b > c</event>" };
    constans character* t2[] = {
        "<event year=\"1\">x <event year=\"2\">y</event>" };
                    Xar* x;
                    Xar* v;

    x = legere(c->piscina, t1, I, NIHIL);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS(eventum(x, ZEPHYRUM)->descriptio,
        "a < b > c");

    x = legere(c->piscina, t2, I, &v);
    CREDO_NECESSE(x != NIHIL && xar_numerus(x) == I);
    CREDO_AEQUALIS_S32(eventum(x, ZEPHYRUM)->annus, II);
    CREDO_AEQUALIS_I32(eventum(x, ZEPHYRUM)->columna, XVIII);
    CREDO_NECESSE(xar_numerus(v) == I);
    CREDO_CHORDA_AEQUALIS_LITERIS(vitium(v, ZEPHYRUM)->causa,
        "non clausum");
    CREDO_AEQUALIS_I32(vitium(v, ZEPHYRUM)->columna, ZEPHYRUM);
}

interior vacuum
_sectio_minuscula (
    CredoContextus* c)
{
    constans character* t[] = {
        "<Event year=\"1\">x</Event> <events year=\"2\">y</events>" };
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
    { "ante Christum", _sectio_ante_christum, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "attributa", _sectio_attributa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "multilinea", _sectio_multilinea, NIHIL, NIHIL, NIHIL, NIHIL },
    { "in prosa", _sectio_in_prosa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "lineae vacuae numerantur", _sectio_lineae_vacuae, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "vitia", _sectio_vitia, NIHIL, NIHIL, NIHIL, NIHIL },
    { "nomina", _sectio_nomina, NIHIL, NIHIL, NIHIL, NIHIL },
    { "signa in textu", _sectio_signa, NIHIL, NIHIL, NIHIL, NIHIL },
    { "minuscula", _sectio_minuscula, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("chronica_eventus", SECTIONES);
}
