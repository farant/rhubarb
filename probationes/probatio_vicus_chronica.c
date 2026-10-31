/* probatio_vicus_chronica.c - chronica in vico (C3): genus
 * 'chronica' montatur super fontes registri communis (eventa libri
 * communis vici), argumentum annum aperit, locus annum nominat, ictus
 * in sagitta pedis ad punctum schirmi VERUM (latus non in 0,0) annum
 * movet. */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "chronica.h"
#include "thema.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
#include "credo.h"

#include <stdlib.h>
#include <string.h>

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
} Mundus;

/* volumen cum pagina libri communis: eventa 1225 et 1274 */
interior vacuum
mundum_parare (
    CredoContextus* c)
{
                 Mundus* x;
    InternamentumChorda* intern;
            ScribaLiber* l;
       ScribaDocumentum* d;
      TabulaCharacterum  folium;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    c->datum = x;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_chronica");
    si (!x->volumen)
    {
        redde;
    }
    intern = internamentum_creare(c->piscina);
    l = scriba_liber_aperire(c->piscina, intern, x->volumen, LXXX, XX);
    d = l ? scriba_liber_paginam_condere(l,
        chorda_ex_literis("historia",
        c->piscina)) : NIHIL;
    si (d)
    {
        tabula_ex_literis_cum_dimensionibus(&folium, c->piscina,
            d->latitudo, d->altitudo,
            "<event year=\"1225\">Thomas nascitur</event>\n"
            "<event year=\"1274\">Thomas moritur</event>");
        (vacuum)scriba_documentum_committere(d, &folium);
    }
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* x = (Mundus*)c->datum;

    si (x && x->volumen)
    {
        volumen_claudere(x->volumen);
    }
}

interior b32
aedificare (
            Mundus* x,
    CredoContextus* c)
{
    redde x && x->volumen
        && vicus_applicatio_aedificare(&x->app, c->piscina,
        internamentum_creare(c->piscina), x->volumen, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM);
}

/* latus frontale dextrum tabulae activae, si chronica */
interior ChronicaMontatio*
chronica_frontalis (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata
        || !chorda_aequalis_literis(l->genus, "chronica"))
    {
        redde NIHIL;
    }
    redde (ChronicaMontatio*)l->montatio;
}

interior b32
campus_est (
      ChronicaMontatio* m,
    constans character* titulus,
    constans character* valor)
{
    chorda* a = insula_ramus_attributum(&m->ramus, titulus);

    redde a && chorda_aequalis_literis(*a, valor);
}

interior vacuum
_probare_latus (
    CredoContextus* c)
{
              Mundus* x = (Mundus*)c->datum;
               Vicus* v;
    ChronicaMontatio* m;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "chronica",
        "chronica/annus/1225"));
    (vacuum)vicus_pulsare(v);
    m = chronica_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_VERUM(campus_est(m, "annus", "1225"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(m, c->piscina),
        "chronica/annus/1225");
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus_titulus(m, c->piscina),
        "Timeline - AD 1225 - 1/1");

    CREDO_GRADUS("argumentum nudum: flumen");
    CREDO_VERUM(chronica_argumentum_ponere(m, "1274"));
    CREDO_VERUM(campus_est(m, "visus", "flumen"));
    CREDO_CHORDA_AEQUALIS_LITERIS(chronica_locus(m, c->piscina),
        "chronica/flumen/1225");
}

/* ictus ad punctum schirmi: sagitta dextra pedis -> annus proximus */
interior vacuum
_probare_ictum (
    CredoContextus* c)
{
              Mundus* x = (Mundus*)c->datum;
               Vicus* v;
    ChronicaMontatio* m;
           Componens* radix;
           Componens* sagitta;
          ManusLudus* manus;
             Punctum  locale;
             Punctum  schirmum;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "chronica",
        "chronica/annus/1225"));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    m = chronica_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(m);
    radix = componens_invenire_per_id(dispensator_arbor(x->app.d),
        chorda_ex_literis("chronica", c->piscina));
    CREDO_NECESSE_NON_NIHIL(radix);
    sagitta = componens_invenire_per_id(radix,
        chorda_ex_literis("sagitta_proxima", c->piscina));
    CREDO_NECESSE_NON_NIHIL(sagitta);
    locale.x  = sagitta->fines.x + sagitta->fines.latitudo / II;
    locale.y  = sagitta->fines.y + sagitta->fines.altitudo / II;
    manus     = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    schirmum = manus_ludus_ad_schirmum(manus, radix, locale);
    CREDO_VERUM(schirmum.x > locale.x);
    CREDO_VERUM(manus_ludus_premere_ad(manus, schirmum.x, schirmum.y));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(campus_est(m, "annus", "1274"));
}

/* C4a: ictus in textu eventus ad punctum schirmi eum eligit. Punctum
 * ex dispositione fluminis: columna eventuum post quintam partem
 * areae (limbus VI + margo IV = X px a sinistro), linea prima XII px
 * a summo (limbus VIII + margo IV) - lib/chronica.c */
interior vacuum
_probare_electionem_ictu (
    CredoContextus* c)
{
              Mundus* x = (Mundus*)c->datum;
               Vicus* v;
    ChronicaMontatio* m;
           Componens* radix;
          ManusLudus* manus;
             Punctum  locale;
             Punctum  schirmum;
              chorda* a;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    CREDO_VERUM(vicus_acervo_aperire(v, "chronica", "1225"));
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(x->app.d);
    m = chronica_frontalis(v);
    CREDO_NECESSE_NON_NIHIL(m);
    radix = componens_invenire_per_id(dispensator_arbor(x->app.d),
        chorda_ex_literis("chronica", c->piscina));
    CREDO_NECESSE_NON_NIHIL(radix);
    locale.x  = X + (radix->fines.latitudo - XX) / V + III;
    locale.y  = XII + III;
    manus     = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(manus);
    schirmum = manus_ludus_ad_schirmum(manus, radix, locale);
    CREDO_VERUM(schirmum.x > locale.x);
    CREDO_VERUM(manus_ludus_premere_ad(manus, schirmum.x, schirmum.y));
    (vacuum)vicus_pulsare(v);
    a = insula_ramus_attributum(&m->ramus, "electus");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(chorda_continet(*a, chorda_ex_literis("historia#",
        c->piscina)));
    CREDO_FALSUM(chorda_continet(*a, chorda_ex_literis("#p",
        c->piscina)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "latus", _probare_latus, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "ictus", _probare_ictum, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "electio ictu", _probare_electionem_ictu, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_chronica", SECTIONES);
}
