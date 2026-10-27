/* probatio_navigator_bibliotheca.c - articuli servati et versiones
 * (navigator N2.4): versio nova solum si contentum mutatum (signa sola
 * = idem), versiones veteres manent, index ex actis reficitur,
 * extractio defecta nihil scribit. Volumen temporarium per sectionem;
 * ids per moneta_numerator ("1", "2", ...). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "moneta.h"
#include "sigillum.h"
#include "articulus.h"
#include "vicipaedia.h"
#include "navigator_bibliotheca.h"
#include "credo.h"
#include <string.h>

#define PAGINA(titulus, corpus) \
    "<html><head><title>" titulus "</title></head><body>" \
    "<section data-mw-section-id=\"0\">" corpus "</section>" \
    "</body></html>"

/* versio prima */
#define H_PRIMA PAGINA("Rheum", "<p>Rheum est planta.</p>")
/* signa sola mutata: attributa, nota referentiae, nexus categoriae */
#define H_SIGNA PAGINA("Rheum", \
    "<p id=\"mwAB\" data-mw=\"{}\">Rheum est <span>planta</span>." \
    "<sup class=\"mw-ref reference\"><a href=\"#c1\">[1]</a></sup>" \
    "</p>" \
    "<link rel=\"mw:PageProp/Category\" href=\"./Category:Herbae\"/>")
/* contentum mutatum */
#define H_ALTERA PAGINA("Rheum", "<p>Rheum est herba.</p>")
/* nexus relativus: scopus ex 'fons' solvitur */
#define H_NEXUS PAGINA("Rheum", \
    "<p>Rheum est <a rel=\"mw:WikiLink\" href=\"./Planta\">" \
    "planta</a>.</p>")
/* articulus alius */
#define H_BETA PAGINA("Beta", "<p>Beta vulgaris.</p>")

#define URL_RHEUM "https://la.wikipedia.org/wiki/Rheum"
#define URL_BETA  "https://la.wikipedia.org/wiki/Beta"

nomen structura {
                 Volumen* volumen;
    NavigatorBibliotheca* b;
                     i32  numerator;
} Mundus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    m->volumen = volumen_temporarium(c->piscina, "probatio_navigator");
    si (m->volumen)
    {
        m->b = navigator_bibliotheca_aperire(m->volumen,
            moneta_numerator, &m->numerator, c->piscina);
    }
    c->datum = m;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* m = (Mundus*)c->datum;

    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
}

interior chorda
lit (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

/* adaptator qui semper deficit */
interior Articulus*
extractio_defecta (
                 chorda  html,
     constans character* fons,
                Piscina* piscina)
{
    (vacuum)html;
    (vacuum)fons;
    (vacuum)piscina;
    redde NIHIL;
}

interior NavigatorExitus
offerre (
                       Mundus*  m,
           constans character*  url,
           constans character*  html,
     constans NavigatorVersio** versio,
                      Piscina*  p)
{
    redde navigator_versionem_offerre(m->b, lit(url, p), lit(url, p),
        "vicipaedia", lit(html, p), vicipaedia_articulus, versio);
}

interior constans NavigatorVersio*
versio (
    constans NavigatorArticulus* a,
                            i32  k)
{
    redde (constans NavigatorVersio*)xar_obtinere(a->versiones, k);
}

/* SHA-256 hex litterarum */
interior chorda
sigillum_literarum (
    constans character* s,
               Piscina* p)
{
     Sigillum sg;
    character hex[SIGILLUM_HEX_MENSURA];

    sg = sigillum_computare(s, (memoriae_index)strlen(s));
    sigillum_hex(&sg, hex);
    redde lit(hex, p);
}

interior vacuum
_probare_novum (
    CredoContextus* c)
{
                         Mundus* m = (Mundus*)c->datum;
       constans NavigatorVersio* v = NIHIL;
    constans NavigatorArticulus* a;

    CREDO_NECESSE_NON_NIHIL(m->b);

    CREDO_GRADUS("bibliotheca vacua");
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(m->b), ZEPHYRUM);
    CREDO_VERUM(navigator_articulus(m->b, ZEPHYRUM) == NIHIL);

    CREDO_GRADUS("oblatio prima: NOVUM, articulus + versio");
    CREDO_AEQUALIS_I32((i32)offerre(m, URL_RHEUM, H_PRIMA, &v,
        c->piscina), (i32)NAVIGATOR_NOVUM);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(m->b), I);
    a = navigator_articulus(m->b, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_CHORDA_AEQUALIS_LITERIS(a->identitas, "1");
    CREDO_CHORDA_AEQUALIS_LITERIS(a->url, URL_RHEUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(a->situs, "vicipaedia");
    CREDO_AEQUALIS_I32(xar_numerus(a->versiones), I);
    CREDO_VERUM(versio(a, ZEPHYRUM) == v);

    CREDO_GRADUS("versio: id, URL, titulus, massa, momentum");
    CREDO_CHORDA_AEQUALIS_LITERIS(v->identitas, "2");
    CREDO_CHORDA_AEQUALIS_LITERIS(v->url, URL_RHEUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->titulus, "Rheum");
    CREDO_VERUM(chorda_aequalis(v->massa, sigillum_literarum(H_PRIMA,
        c->piscina)));
    CREDO_VERUM(v->momentum.mensura > ZEPHYRUM);

    CREDO_GRADUS("invenire per URL");
    CREDO_VERUM(navigator_articulum_invenire(m->b, lit(URL_RHEUM,
        c->piscina)) == a);
    CREDO_VERUM(navigator_articulum_invenire(m->b, lit(URL_BETA,
        c->piscina)) == NIHIL);

    CREDO_GRADUS("HTML versionis ex massa");
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_versionis_html(m->b, v,
        c->piscina), H_PRIMA);
}

interior vacuum
_probare_idem (
    CredoContextus* c)
{
                       Mundus* m      = (Mundus*)c->datum;
     constans NavigatorVersio* prima  = NIHIL;
     constans NavigatorVersio* v      = NIHIL;
                          s64  acta;
                          s64  massae;

    CREDO_NECESSE_NON_NIHIL(m->b);
    CREDO_NECESSE(offerre(m, URL_RHEUM, H_PRIMA, &prima, c->piscina)
        == NAVIGATOR_NOVUM);
    acta    = volumen_summa_actorum(m->volumen);
    massae  = volumen_summa_massarum(m->volumen);

    CREDO_GRADUS("idem HTML: IDEM, versio ultima, nihil scriptum");
    CREDO_AEQUALIS_I32((i32)offerre(m, URL_RHEUM, H_PRIMA, &v,
        c->piscina), (i32)NAVIGATOR_IDEM);
    CREDO_VERUM(v == prima);

    CREDO_GRADUS("signa sola mutata: IDEM, nihil scriptum");
    v = NIHIL;
    CREDO_AEQUALIS_I32((i32)offerre(m, URL_RHEUM, H_SIGNA, &v,
        c->piscina), (i32)NAVIGATOR_IDEM);
    CREDO_VERUM(v == prima);
    CREDO_VERUM(volumen_summa_actorum(m->volumen) == acta);
    CREDO_VERUM(volumen_summa_massarum(m->volumen) == massae);
    CREDO_AEQUALIS_I32(xar_numerus(navigator_articulus(m->b,
        ZEPHYRUM)->versiones), I);
}

interior vacuum
_probare_versio_nova (
    CredoContextus* c)
{
                         Mundus* m      = (Mundus*)c->datum;
       constans NavigatorVersio* prima  = NIHIL;
       constans NavigatorVersio* v      = NIHIL;
    constans NavigatorArticulus* a;

    CREDO_NECESSE_NON_NIHIL(m->b);
    CREDO_NECESSE(offerre(m, URL_RHEUM, H_PRIMA, &prima, c->piscina)
        == NAVIGATOR_NOVUM);

    CREDO_GRADUS("contentum mutatum: VERSIO_NOVA");
    CREDO_AEQUALIS_I32((i32)offerre(m, URL_RHEUM, H_ALTERA, &v,
        c->piscina), (i32)NAVIGATOR_VERSIO_NOVA);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->identitas, "3");
    a = navigator_articulus(m->b, ZEPHYRUM);
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(m->b), I);
    CREDO_AEQUALIS_I32(xar_numerus(a->versiones), II);
    CREDO_VERUM(versio(a, ZEPHYRUM) == prima);
    CREDO_VERUM(versio(a, I) == v);

    CREDO_GRADUS("versio vetus manet legibilis");
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_versionis_html(m->b, prima,
        c->piscina), H_PRIMA);
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_versionis_html(m->b, v,
        c->piscina), H_ALTERA);

    CREDO_GRADUS("comparatio cum versione ULTIMA");
    CREDO_AEQUALIS_I32((i32)offerre(m, URL_RHEUM, H_ALTERA, &v,
        c->piscina), (i32)NAVIGATOR_IDEM);
    CREDO_AEQUALIS_I32((i32)offerre(m, URL_RHEUM, H_PRIMA, &v,
        c->piscina), (i32)NAVIGATOR_VERSIO_NOVA);
    CREDO_AEQUALIS_I32(xar_numerus(a->versiones), III);
}

/* URL petitionis alia (redirectio): nexus contra URL CANONICAM
 * solvuntur, ergo idem contentum */
interior vacuum
_probare_url_finalis (
    CredoContextus* c)
{
                       Mundus* m = (Mundus*)c->datum;
     constans NavigatorVersio* v = NIHIL;

    CREDO_NECESSE_NON_NIHIL(m->b);
    CREDO_GRADUS("versio URL ultimam petitionis servat");
    CREDO_AEQUALIS_I32((i32)navigator_versionem_offerre(m->b,
        lit(URL_RHEUM, c->piscina),
        lit("https://la.wikipedia.org/w/alia/Rheum_nomen", c->piscina),
        "vicipaedia", lit(H_NEXUS, c->piscina), vicipaedia_articulus,
        &v), (i32)NAVIGATOR_NOVUM);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_AEQUALIS_LITERIS(v->url,
        "https://la.wikipedia.org/w/alia/Rheum_nomen");

    CREDO_GRADUS("URL petitionis alia, idem HTML: IDEM");
    CREDO_AEQUALIS_I32((i32)navigator_versionem_offerre(m->b,
        lit(URL_RHEUM, c->piscina),
        lit("https://la.wikipedia.org/w/tertia/Rheum", c->piscina),
        "vicipaedia", lit(H_NEXUS, c->piscina), vicipaedia_articulus,
        NIHIL), (i32)NAVIGATOR_IDEM);
}

interior vacuum
_probare_separati (
    CredoContextus* c)
{
                         Mundus* m = (Mundus*)c->datum;
    constans NavigatorArticulus* rheum;
    constans NavigatorArticulus* beta;

    CREDO_NECESSE_NON_NIHIL(m->b);
    CREDO_GRADUS("duo articuli, NOVUM uterque");
    CREDO_AEQUALIS_I32((i32)offerre(m, URL_RHEUM, H_PRIMA, NIHIL,
        c->piscina), (i32)NAVIGATOR_NOVUM);
    CREDO_AEQUALIS_I32((i32)offerre(m, URL_BETA, H_BETA, NIHIL,
        c->piscina), (i32)NAVIGATOR_NOVUM);
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(m->b), II);
    rheum = navigator_articulum_invenire(m->b, lit(URL_RHEUM,
        c->piscina));
    beta  = navigator_articulum_invenire(m->b, lit(URL_BETA,
        c->piscina));
    CREDO_NECESSE(rheum && beta && rheum != beta);
    CREDO_VERUM(navigator_articulus(m->b, ZEPHYRUM) == rheum);
    CREDO_VERUM(navigator_articulus(m->b, I) == beta);
    CREDO_CHORDA_AEQUALIS_LITERIS(versio(beta, ZEPHYRUM)->titulus,
        "Beta");

    CREDO_GRADUS("versio nova unius alterum non tangit");
    CREDO_AEQUALIS_I32((i32)offerre(m, URL_BETA, H_ALTERA, NIHIL,
        c->piscina), (i32)NAVIGATOR_VERSIO_NOVA);
    CREDO_AEQUALIS_I32(xar_numerus(beta->versiones), II);
    CREDO_AEQUALIS_I32(xar_numerus(rheum->versiones), I);
}

/* index ex actis: bibliotheca secunda super idem volumen */
interior vacuum
_probare_reficere (
    CredoContextus* c)
{
                         Mundus* m = (Mundus*)c->datum;
           NavigatorBibliotheca* b2;
    constans NavigatorArticulus* a;
    constans NavigatorArticulus* a2;
       constans NavigatorVersio* v;
       constans NavigatorVersio* v2;
                            i32  i;
                            i32  j;

    CREDO_NECESSE_NON_NIHIL(m->b);
    offerre(m, URL_RHEUM, H_PRIMA, NIHIL, c->piscina);
    offerre(m, URL_BETA, H_BETA, NIHIL, c->piscina);
    offerre(m, URL_RHEUM, H_ALTERA, NIHIL, c->piscina);

    CREDO_GRADUS("bibliotheca secunda idem indicem legit");
    b2 = navigator_bibliotheca_aperire(m->volumen, moneta_numerator,
        &m->numerator, c->piscina);
    CREDO_NECESSE_NON_NIHIL(b2);
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(b2),
        navigator_articulorum_numerus(m->b));
    per (i = ZEPHYRUM; i < navigator_articulorum_numerus(m->b); i++)
    {
        a   = navigator_articulus(m->b, i);
        a2  = navigator_articulus(b2, i);
        CREDO_VERUM(chorda_aequalis(a->identitas, a2->identitas));
        CREDO_VERUM(chorda_aequalis(a->url, a2->url));
        CREDO_VERUM(chorda_aequalis(a->situs, a2->situs));
        CREDO_AEQUALIS_I32(xar_numerus(a2->versiones),
            xar_numerus(a->versiones));
        per (j = ZEPHYRUM; j < xar_numerus(a->versiones); j++)
        {
            v   = versio(a, j);
            v2  = versio(a2, j);
            CREDO_VERUM(chorda_aequalis(v->identitas, v2->identitas));
            CREDO_VERUM(chorda_aequalis(v->momentum, v2->momentum));
            CREDO_VERUM(chorda_aequalis(v->url, v2->url));
            CREDO_VERUM(chorda_aequalis(v->titulus, v2->titulus));
            CREDO_VERUM(chorda_aequalis(v->massa, v2->massa));
        }
    }

    CREDO_GRADUS("bibliotheca secunda comparat cum versione ultima");
    CREDO_AEQUALIS_I32((i32)navigator_versionem_offerre(b2,
        lit(URL_RHEUM, c->piscina), lit(URL_RHEUM, c->piscina),
        "vicipaedia", lit(H_ALTERA, c->piscina), vicipaedia_articulus,
        NIHIL), (i32)NAVIGATOR_IDEM);
}

interior vacuum
_probare_vitium (
    CredoContextus* c)
{
     Mundus* m = (Mundus*)c->datum;
        s64  acta;
        s64  massae;

    CREDO_NECESSE_NON_NIHIL(m->b);
    acta    = volumen_summa_actorum(m->volumen);
    massae  = volumen_summa_massarum(m->volumen);

    CREDO_GRADUS("extractio defecta (novum): VITIUM, nihil scriptum");
    CREDO_AEQUALIS_I32((i32)navigator_versionem_offerre(m->b,
        lit(URL_RHEUM, c->piscina), lit(URL_RHEUM, c->piscina),
        "vicipaedia", lit(H_PRIMA, c->piscina), extractio_defecta,
        NIHIL),
        (i32)NAVIGATOR_VITIUM);
    CREDO_VERUM(volumen_summa_actorum(m->volumen) == acta);
    CREDO_VERUM(volumen_summa_massarum(m->volumen) == massae);
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(m->b), ZEPHYRUM);

    CREDO_GRADUS("extractio defecta (servatus): VITIUM, nil scriptum");
    CREDO_NECESSE(offerre(m, URL_RHEUM, H_PRIMA, NIHIL, c->piscina)
        == NAVIGATOR_NOVUM);
    acta    = volumen_summa_actorum(m->volumen);
    massae  = volumen_summa_massarum(m->volumen);
    CREDO_AEQUALIS_I32((i32)navigator_versionem_offerre(m->b,
        lit(URL_RHEUM, c->piscina), lit(URL_RHEUM, c->piscina),
        "vicipaedia", lit(H_ALTERA, c->piscina), extractio_defecta,
        NIHIL), (i32)NAVIGATOR_VITIUM);
    CREDO_VERUM(volumen_summa_actorum(m->volumen) == acta);
    CREDO_VERUM(volumen_summa_massarum(m->volumen) == massae);
    CREDO_AEQUALIS_I32(xar_numerus(navigator_articulus(m->b,
        ZEPHYRUM)->versiones), I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "novum", _probare_novum, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "idem", _probare_idem, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "versio nova", _probare_versio_nova, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "URL finalis", _probare_url_finalis, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "separati", _probare_separati, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "reficere", _probare_reficere, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "vitium", _probare_vitium, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("navigator_bibliotheca", SECTIONES);
}
