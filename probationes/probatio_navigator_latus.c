/* probatio_navigator_latus.c - navigator in latere (N3.2): index
 * per hospitem (electus j k, ictus, duplex), articulus paginatus (j k,
 * sagittae pedis, Esc), versio in loco (comes versionis suae), semitae
 * (argumentum, locus, titulus loci), bibliotheca vacua, notae in pede.
 * Bibliotheca in volumine temporario: Rhubarb (fixum verum, en),
 * Rheum et Beta (la; Beta duas versiones habet). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "volumen.h"
#include "moneta.h"
#include "filum.h"
#include "vicipaedia.h"
#include "navigator_bibliotheca.h"
#include "navigator.h"
#include "navigator_petitio.h"
#include "reactor.h"
#include "servus_scriptus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define LATITUDO  CCCLX
#define ALTITUDO  CDLXXII

#define PAGINA(titulus, corpus) \
    "<html><head><title>" titulus "</title></head><body>" \
    "<section data-mw-section-id=\"0\">" corpus "</section>" \
    "</body></html>"

#define URL_ANGLICUM "https://en.wikipedia.org/wiki/Rhubarb"
#define URL_RHEUM   "https://la.wikipedia.org/wiki/Rheum"
#define URL_BETA    "https://la.wikipedia.org/wiki/Beta"

nomen structura {
         InternamentumChorda* intern;
          InsulaRepositorium* repo;
                     Volumen* volumen;
        NavigatorBibliotheca* bibliotheca;
                         i32  numerator;
           NavigatorMontatio  m;
                 Dispensator* d;
                         b32  notae;      /* notas_habet probationis */
                     Piscina* p;
                     Reactor* reactor;    /* N3.3: petitiones */
    NavigatorPetitioOptiones  optiones;   /* radix ad servum scriptum */
} Mundus;

interior Reactor*
reactor_mundi (
    vacuum* ctx)
{
    redde ((Mundus*)ctx)->reactor;
}

interior b32
notas_habere (
    vacuum* ctx,
    chorda  locus)
{
    (vacuum)locus;
    redde ((Mundus*)ctx)->notae;
}

interior vacuum
offerre (
                 Mundus* w,
     constans character* url,
                 chorda  html)
{
    (vacuum)navigator_versionem_offerre(w->bibliotheca,
        chorda_ex_literis(
        url, w->p), chorda_ex_literis(url, w->p), "vicipaedia", html,
        vicipaedia_articulus, NIHIL);
}

/* latus super bibliothecam; plena: Rhubarb (1, versio 2), Rheum (3,
 * 4), Beta (5, versiones 6 et 7) */
interior Mundus*
mundus (
    CredoContextus* c,
               b32  plena)
{
    Mundus* w;
     Motus* motus;

    thema_initiare();
    w = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(w, ZEPHYRUM, magnitudo(Mundus));
    c->datum   = w;   /* purgare volumen claudit, etiam post NECESSE */
    w->p       = c->piscina;
    w->intern  = internamentum_creare(c->piscina);
    w->volumen  = volumen_temporarium(c->piscina,
        "probatio_navigator");
    w->bibliotheca  = w->volumen ? navigator_bibliotheca_aperire(
        w->volumen, moneta_numerator, &w->numerator,
        c->piscina) : NIHIL;
    si (!w->bibliotheca)
    {
        redde NIHIL;
    }
    si (plena)
    {
        offerre(w, URL_ANGLICUM, filum_legere_totum(
            "probationes/fixa/vicipaedia/rhubarb.html", c->piscina));
        offerre(w, URL_RHEUM, chorda_ex_literis(PAGINA("Rheum",
            "<p>Rheum est <a rel=\"mw:WikiLink\" href=\"./Planta\">"
            "genus</a> plantarum.</p>"), c->piscina));
        offerre(w, URL_BETA, chorda_ex_literis(PAGINA("Beta",
            "<p>Beta est planta.</p>"), c->piscina));
        offerre(w, URL_BETA, chorda_ex_literis(PAGINA("Beta",
            "<p>Beta vulgaris est olus.</p>"), c->piscina));
        /* N3.4: index favores solos monstrat - omnes tres favores */
        (vacuum)navigator_favorem_ponere(w->bibliotheca,
            chorda_ex_literis("1", c->piscina), VERUM);
        (vacuum)navigator_favorem_ponere(w->bibliotheca,
            chorda_ex_literis("3", c->piscina), VERUM);
        (vacuum)navigator_favorem_ponere(w->bibliotheca,
            chorda_ex_literis("5", c->piscina), VERUM);
    }
    w->repo = insula_repositorium_creare(c->piscina, w->intern,
        "<hospes/>");
    si (   !w->repo
        || !navigator_montare(&w->m, c->piscina, w->intern, w->repo,
               "n1", w->bibliotheca, LATITUDO, ALTITUDO, NIHIL))
    {
        redde NIHIL;
    }
    w->m.notas_habet = notas_habere;
    w->m.notas_habet_ctx = w;
    w->reactor = reactor_creare(c->piscina);
    w->m.reactor = reactor_mundi;
    w->m.reactor_ctx = w;
    w->optiones.agens = chorda_ex_literis("proba/1", c->piscina);
    w->m.optiones_petitionis = &w->optiones;
    w->d = dispensator_creare(c->piscina, w->intern, w->repo,
        w->m.actiones, navigator_componere, &w->m, CCC);
    si (!w->d)
    {
        redde NIHIL;
    }
    motus         = dispensator_motus(w->d);
    motus->ramus  = w->m.ramus;
    dispensator_recomponere(w->d);
    redde w;
}

interior vacuum
mundum_claudere (
    Mundus* w)
{
    si (w && w->volumen)
    {
        volumen_claudere(w->volumen);
        w->volumen = NIHIL;
    }
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    mundum_claudere((Mundus*)c->datum);
}

interior vacuum
clavis (
       Mundus* w,
    character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.producta  = (s32)(insignatus character)typus;
    e.datum.clavis.clavis    = (clavis_t)typus;
    e.datum.clavis.runa      = (s32)typus;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior vacuum
clavis_nominata (
       Mundus* w,
     clavis_t  k,
          s32  producta)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis    = k;
    e.datum.clavis.producta  = producta;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
}

interior b32
campus_est (
                 Mundus* w,
     constans character* titulus,
     constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(&w->m.ramus, titulus);
    si (valor == NIHIL)
    {
        redde a == NIHIL || a->mensura == ZEPHYRUM;
    }
    redde a && chorda_aequalis_literis(*a, valor);
}

/* textus pictus continens fragmentum? */
interior b32
pictum (
                 Mundus* w,
     constans character* fragmentum,
                Piscina* p)
{
               Mandata* md;
     constans Mandatum* x;
                   i32  i;

    md = mandata_creare(p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS
            && chorda_continet(x->textus, chorda_ex_literis(fragmentum,
                p)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* textus continens fragmentum colore thematis pictus? */
interior b32
pictum_colore (
                  Mundus* w,
      constans character* fragmentum,
              ColorThema  color,
                 Piscina* p)
{
                Mandata* md;
      constans Mandatum* x;
           ColorMandati  quaesitus;
                    i32  i;

    quaesitus  = figura_color_thematis(color);
    md         = mandata_creare(p, w->intern);
    pingere(dispensator_arbor(w->d), w->m.figurae, ZEPHYRUM, md);
    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus       == MANDATUM_TEXTUS
            && x->color.genus == quaesitus.genus
            && x->color.valor == quaesitus.valor
            && chorda_continet(x->textus, chorda_ex_literis(fragmentum,
                p)))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ictus (aut duplex) in medio componentis id */
interior b32
ictus_in (
                 Mundus* w,
     constans character* id,
                    b32  geminus,
                Piscina* p)
{
     Componens* c;
       Eventus  e;

    c = componens_invenire_per_id(dispensator_arbor(w->d),
        chorda_ex_literis(id, p));
    si (!c || c->fines.altitudo <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = geminus ? EVENTUS_MUS_DUPLEX
                                 : EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = c->fines.x + c->fines.latitudo / II;
    e.datum.mus.y       = c->fines.y + c->fines.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(w->d, &e);
    dispensator_recomponere(w->d);
    redde VERUM;
}

interior b32
argumentum (
                 Mundus* w,
     constans character* arg)
{
    b32 r;

    r = navigator_argumentum_ponere(&w->m, arg);
    dispensator_recomponere(w->d);
    redde r;
}

/* ===== N3.3: aperire per URL ===== */

/* servus scriptus gradu uno (octeti NIHIL et claudere FALSUM =
 * tacitus); radix optionum ad eum */
interior ServusScriptus*
servum_ponere (
                Mundus* w,
    constans character* octeti,
                   b32  claudere)
{
    ServusGradus* g;
  ServusScriptus* s;
       character* radix;

    g = (ServusGradus*)piscina_allocare(w->p, magnitudo(ServusGradus));
    g->octeti = octeti;
    g->mensura = ZEPHYRUM;
    g->mora_ms = ZEPHYRUM;
    g->claudere = claudere;
    s = servum_scriptum_creare(w->reactor, g, (octeti || claudere) ? I
        : ZEPHYRUM, w->p);
    si (s)
    {
        radix = (character*)piscina_allocare(w->p, LXIV);
        sprintf(radix, "http://127.0.0.1:%u",
            (insignatus integer)s->portus);
        w->optiones.radix_fontis = radix;
    }
    redde s;
}

interior constans character*
responsum_ducenti (
    constans character* corpus,
               Piscina* p)
{
    character* s;

    s = (character*)piscina_allocare(p, (memoriae_index)(strlen(corpus)
        + CXXVIII));
    sprintf(s, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n"
        "Content-Length: %u\r\n\r\n%s",
        (insignatus integer)strlen(corpus), corpus);
    redde s;
}

/* litterae in promptam, deinde Enter */
interior vacuum
scribere (
                Mundus* w,
    constans character* textus)
{
    i32 i;

    per (i = ZEPHYRUM; textus[i]; i++)
    {
        clavis(w, textus[i]);
    }
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
}

/* reactor poscitur usque ad pulsum mutatum (petitio finita) */
interior b32
pervenire (
    Mundus* w)
{
    i32 k;

    per (k = ZEPHYRUM; k < CL; k++)
    {
        (vacuum)reactor_poll(w->reactor, XX);
        si (navigator_pulsare(&w->m))
        {
            dispensator_recomponere(w->d);
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
prompta_clausa (
    Mundus* w)
{
    redde campus_est(w, "prompta", NIHIL)
        || campus_est(w, "prompta", "nulla");
}

interior vacuum
_sectio_aperire (
    CredoContextus* c)
{
            Mundus* w = mundus(c, VERUM);
    ServusScriptus* s;

    CREDO_NECESSE_NON_NIHIL(w);
    s = servum_ponere(w, responsum_ducenti(PAGINA("Planta",
        "<p>Planta viva est.</p>"), c->piscina), FALSUM);
    CREDO_NECESSE_NON_NIHIL(s);

    CREDO_GRADUS("o: prompta url aperitur; mora nulla sine petitione");
    CREDO_AEQUALIS_I32((i32)navigator_mora(&w->m), (i32)-I);
    clavis(w, 'o');
    CREDO_VERUM(campus_est(w, "prompta", "url"));

    CREDO_GRADUS("URL + Enter: petitio, nota 'Fetching', index manet");
    scribere(w, "https://la.wikipedia.org/wiki/Planta");
    CREDO_VERUM(prompta_clausa(w));
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(pictum(w, "Fetching la.wikipedia.org/wiki/Planta",
        c->piscina));

    CREDO_GRADUS("petitio finita: articulus apertus, versio fixa");
    CREDO_VERUM(pervenire(w));
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(navigator_articulum_invenire(w->bibliotheca,
        chorda_ex_literis("https://la.wikipedia.org/wiki/Planta",
        c->piscina)) != NIHIL);
    CREDO_VERUM(!campus_est(w, "versio", NIHIL));
    CREDO_VERUM(pictum(w, "Planta viva est.", c->piscina));
    CREDO_AEQUALIS_I32(s->acceptae, I);
    CREDO_FALSUM(navigator_pulsare(&w->m));

    CREDO_GRADUS("URL servata: statim, sine rete");
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    clavis(w, 'o');
    scribere(w, "https://la.wikipedia.org/wiki/Beta");
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "articulus", "5"));
    CREDO_AEQUALIS_I32(s->acceptae, I);

    CREDO_GRADUS("$nav(URL): argumentum cum :// petit");
    CREDO_VERUM(argumentum(w, "https://la.wikipedia.org/wiki/Herba"));
    CREDO_VERUM(pictum(w, "Fetching la.wikipedia.org/wiki/Herba",
        c->piscina));
    CREDO_VERUM(pervenire(w));
    CREDO_AEQUALIS_I32(s->acceptae, II);
    servum_scriptum_claudere(s);
}

interior vacuum
_sectio_errores (
    CredoContextus* c)
{
            Mundus* w = mundus(c, VERUM);
    ServusScriptus* s;

    CREDO_NECESSE_NON_NIHIL(w);
    s = servum_ponere(w, "HTTP/1.1 404 Not Found\r\nContent-Length: 0"
        "\r\n\r\n", FALSUM);
    CREDO_NECESSE_NON_NIHIL(s);

    CREDO_GRADUS("404: 'Not found', nihil servatum");
    clavis(w, 'o');
    scribere(w, "https://la.wikipedia.org/wiki/Nusquam");
    CREDO_VERUM(pervenire(w));
    CREDO_VERUM(pictum(w, "Not found", c->piscina));
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(w->bibliotheca),
        III);

    CREDO_GRADUS("situs alienus: 'Unsupported site', sine rete");
    clavis(w, 'o');
    scribere(w, "https://example.com/pagina");
    CREDO_VERUM(pictum(w, "Unsupported site", c->piscina));
    CREDO_AEQUALIS_I32(s->acceptae, I);

    CREDO_GRADUS("verba sine '://': quaestio; 404 -> 'Not found'");
    clavis(w, 'o');
    scribere(w, "rhabarbarum");
    CREDO_VERUM(pictum(w,
        "Searching en.wikipedia.org for 'rhabarbarum'",
        c->piscina));
    CREDO_VERUM(pervenire(w));
    CREDO_VERUM(pictum(w, "Not found", c->piscina));
    servum_scriptum_claudere(s);
}

/* petitio nova pendentem abrumpit STATIM (non solum in pulsu
 * proximo): prima ad servum tardum (CCC ms) qui bene responderet;
 * reactor sine pulsu poscitur - sine abruptione prima servaretur et
 * ut petitio currens acciperetur. Globulus "Open URL...". */
interior vacuum
_sectio_supersedere (
    CredoContextus* c)
{
            Mundus* w = mundus(c, VERUM);
      ServusGradus* g;
    ServusScriptus* tardus;
    ServusScriptus* vivus;
         character* radix;
               i32  k;

    CREDO_NECESSE_NON_NIHIL(w);
    g = (ServusGradus*)piscina_allocare(c->piscina,
        magnitudo(ServusGradus));
    g->octeti   = responsum_ducenti(PAGINA("Prima", "<p>Prima.</p>"),
        c->piscina);
    g->mensura   = ZEPHYRUM;
    g->mora_ms   = CCC;
    g->claudere  = FALSUM;
    tardus       = servum_scriptum_creare(w->reactor, g, I, c->piscina);
    CREDO_NECESSE_NON_NIHIL(tardus);
    radix = (character*)piscina_allocare(c->piscina, LXIV);
    sprintf(radix, "http://127.0.0.1:%u",
        (insignatus integer)tardus->portus);
    w->optiones.radix_fontis = radix;

    CREDO_GRADUS("globulus 'Open URL...': prompta aperitur");
    CREDO_VERUM(pictum(w, "Open URL", c->piscina));
    CREDO_VERUM(ictus_in(w, "aperire-url", FALSUM, c->piscina));
    CREDO_VERUM(campus_est(w, "prompta", "url"));
    scribere(w, "https://la.wikipedia.org/wiki/Prima");
    CREDO_VERUM(pictum(w, "Fetching la.wikipedia.org/wiki/Prima",
        c->piscina));
    per (k = ZEPHYRUM; k < V; k++)
    {
        (vacuum)reactor_poll(w->reactor, X);
    }

    CREDO_GRADUS("altera (servus alius) primam statim abrumpit");
    vivus = servum_ponere(w, responsum_ducenti(PAGINA("Altera",
        "<p>Altera pagina.</p>"), c->piscina), FALSUM);
    CREDO_NECESSE_NON_NIHIL(vivus);
    clavis(w, 'o');
    scribere(w, "https://la.wikipedia.org/wiki/Altera");
    per (k = ZEPHYRUM; k < L; k++)
    {
        (vacuum)reactor_poll(w->reactor, X);   /* sine pulsu */
    }
    CREDO_VERUM(pervenire(w));
    CREDO_VERUM(pictum(w, "Altera pagina.", c->piscina));
    CREDO_VERUM(navigator_articulum_invenire(w->bibliotheca,
        chorda_ex_literis("https://la.wikipedia.org/wiki/Prima",
        c->piscina)) == NIHIL);
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(w->bibliotheca),
        IV);
    servum_scriptum_claudere(tardus);
    servum_scriptum_claudere(vivus);
}

/* N3.3b: quaestio - eventus, electio, apertio (servatus statim,
 * alius petitur), mus, "No results", hospes ex articulo, Esc */
interior vacuum
_sectio_quaestio (
    CredoContextus* c)
{
            Mundus* w = mundus(c, VERUM);
    ServusScriptus* s;
    ServusScriptus* a;

    CREDO_NECESSE_NON_NIHIL(w);
    s = servum_ponere(w, responsum_ducenti("{\"pages\":["
        "{\"key\":\"Roma\",\"title\":\"Roma\","
        "\"description\":\"Urbs aeterna\"},"
        "{\"key\":\"Beta\",\"title\":\"Beta\","
        "\"description\":\"Genus plantarum\"}]}", c->piscina), FALSUM);
    CREDO_NECESSE_NON_NIHIL(s);

    CREDO_GRADUS("'la: Roma': quaestio in la.wikipedia.org");
    clavis(w, 'o');
    scribere(w, "la: Roma");
    CREDO_VERUM(pictum(w, "Searching la.wikipedia.org for 'Roma'",
        c->piscina));
    CREDO_VERUM(pervenire(w));
    CREDO_VERUM(chorda_incipit(chorda_aedificator_spectare(s->petitio),
        chorda_ex_literis("GET /w/rest.php/v1/search/page?q=Roma"
        "&limit=20",
        c->piscina)));

    CREDO_GRADUS("eventus: titulus - descriptio, campi quaestionis");
    CREDO_VERUM(campus_est(w, "visus", "eventus"));
    CREDO_VERUM(campus_est(w, "quaestio", "Roma"));
    CREDO_VERUM(campus_est(w, "quaestio_hospes", "la.wikipedia.org"));
    CREDO_VERUM(pictum(w, "Urbs aeterna", c->piscina));
    CREDO_VERUM(pictum(w, "Genus plantarum", c->piscina));

    CREDO_GRADUS("j + Enter in 'Beta' (servatus): statim, sine rete");
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus_eventus", "1"));
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "articulus", "5"));
    CREDO_AEQUALIS_I32(s->acceptae, I);

    CREDO_GRADUS("in pagina (la): sine praefixo, hospes articuli");
    clavis(w, 'o');
    scribere(w, "Roma");
    CREDO_VERUM(pictum(w, "Searching la.wikipedia.org for 'Roma'",
        c->piscina));
    CREDO_VERUM(pervenire(w));
    CREDO_VERUM(campus_est(w, "visus", "eventus"));

    CREDO_GRADUS("mus: duplex in 'Roma' (non servatus): petitur");
    a = servum_ponere(w, responsum_ducenti(PAGINA("Roma",
        "<p>Roma caput mundi.</p>"), c->piscina), FALSUM);
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_VERUM(ictus_in(w, "r1", VERUM, c->piscina));
    CREDO_VERUM(pervenire(w));
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(pictum(w, "Roma caput mundi.", c->piscina));
    servum_scriptum_claudere(a);

    CREDO_GRADUS("Esc ex eventibus: ad paginam si articulus");
    clavis(w, 'o');
    servum_scriptum_claudere(s);
    s = servum_ponere(w, responsum_ducenti("{\"pages\":[{\"key\":"
        "\"X\",\"title\":\"X\"}]}", c->piscina), FALSUM);
    scribere(w, "X");
    CREDO_VERUM(pervenire(w));
    CREDO_VERUM(campus_est(w, "visus", "eventus"));
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    servum_scriptum_claudere(s);

    CREDO_GRADUS("nulli eventus: 'No results', visus manet");
    s = servum_ponere(w, responsum_ducenti("{\"pages\":[]}",
        c->piscina), FALSUM);
    clavis(w, 'o');
    scribere(w, "nihil");
    CREDO_VERUM(pervenire(w));
    CREDO_VERUM(pictum(w, "No results", c->piscina));
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    servum_scriptum_claudere(s);
}

/* N3.4: favores - b in pagina et in dominio, stella pedis (mus),
 * non favores absunt, dominium sine favoribus */
interior vacuum
_sectio_favores (
    CredoContextus* c)
{
    Mundus* w = mundus(c, VERUM);

    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("b in pagina: favor tollitur; stella flava, obscura");
    CREDO_VERUM(argumentum(w, "3"));
    CREDO_VERUM(navigator_articulum_invenire(w->bibliotheca,
        chorda_ex_literis(URL_RHEUM, c->piscina))->favor);
    CREDO_VERUM(pictum_colore(w, "\342\230\205", COLOR_WARNING,
        c->piscina));
    clavis(w, 'b');
    CREDO_FALSUM(navigator_articulum_invenire(w->bibliotheca,
        chorda_ex_literis(URL_RHEUM, c->piscina))->favor);
    CREDO_VERUM(pictum_colore(w, "\342\230\205", COLOR_TEXT_DIM,
        c->piscina));
    CREDO_FALSUM(pictum_colore(w, "\342\230\205", COLOR_WARNING,
        c->piscina));

    CREDO_GRADUS("ictus in stella: favor iterum; b iterum tollit");
    CREDO_VERUM(ictus_in(w, "favor", FALSUM, c->piscina));
    CREDO_VERUM(navigator_articulum_invenire(w->bibliotheca,
        chorda_ex_literis(URL_RHEUM, c->piscina))->favor);
    clavis(w, 'b');

    CREDO_GRADUS("Esc: dominium; non favor (Rheum) abest");
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(campus_est(w, "dominium", "wikipedia.org"));
    CREDO_VERUM(pictum(w, "Rhubarb", c->piscina));
    CREDO_VERUM(pictum(w, "Beta", c->piscina));
    CREDO_FALSUM(pictum(w, "Rheum", c->piscina));

    CREDO_GRADUS("b in electo (Rhubarb, primus): tollitur ex indice");
    clavis(w, 'b');
    CREDO_FALSUM(pictum(w, "Rhubarb", c->piscina));
    CREDO_VERUM(pictum(w, "Beta", c->piscina));

    CREDO_GRADUS("ultimus tollitur: radix sine favoribus");
    clavis(w, 'b');
    CREDO_FALSUM(pictum(w, "Beta", c->piscina));
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    CREDO_VERUM(campus_est(w, "dominium", NIHIL));
    CREDO_VERUM(pictum(w, "No bookmarks yet", c->piscina));
    CREDO_AEQUALIS_I32(navigator_articulorum_numerus(w->bibliotheca),
        III);

    CREDO_GRADUS("Esc ex pagina, dominio sine favoribus: radix");
    CREDO_VERUM(argumentum(w, "3"));
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(campus_est(w, "dominium", NIHIL));
    mundum_claudere(w);
}

interior vacuum
_sectio_index (
    CredoContextus* c)
{
    Mundus* w = mundus(c, VERUM);

    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("N3.4 radix: 'Saved', 'Bookmarks', dominium, numerus");
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(pictum(w, "Saved", c->piscina));
    CREDO_VERUM(pictum(w, "Bookmarks", c->piscina));
    CREDO_VERUM(pictum(w, "wikipedia.org", c->piscina));
    CREDO_VERUM(pictum(w, "3 bookmarks", c->piscina));
    CREDO_FALSUM(pictum(w, "Rhubarb", c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_locus_titulus(&w->m,
        c->piscina), "Saved");

    CREDO_GRADUS("Enter in dominio: descendit; favores per hospitem");
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
    CREDO_VERUM(campus_est(w, "dominium", "wikipedia.org"));
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_locus_titulus(&w->m,
        c->piscina), "Saved - wikipedia.org");
    CREDO_VERUM(pictum(w, "en.wikipedia.org", c->piscina));
    CREDO_VERUM(pictum(w, "la.wikipedia.org", c->piscina));
    CREDO_VERUM(pictum(w, "Rhubarb", c->piscina));
    CREDO_VERUM(pictum(w, "Rheum", c->piscina));
    CREDO_VERUM(pictum(w, "2 versions", c->piscina));
    CREDO_VERUM(pictum(w, "1 version", c->piscina));

    CREDO_GRADUS("ordo: hospites, deinde tituli (Rhubarb Beta Rheum)");
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus_articulus", "5"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus_articulus", "3"));
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "electus_articulus", "3"));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "electus_articulus", "5"));

    CREDO_GRADUS("mus: ictus eligit, duplex aperit");
    CREDO_VERUM(ictus_in(w, "r1", FALSUM, c->piscina));
    CREDO_VERUM(campus_est(w, "electus_articulus", "1"));
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(ictus_in(w, "r3", VERUM, c->piscina));
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "articulus", "5"));

    CREDO_GRADUS("Esc: ad indicem, electus manet");
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(campus_est(w, "articulus", NIHIL));
    CREDO_VERUM(campus_est(w, "electus_articulus", "5"));

    CREDO_GRADUS("Enter: articulus electus, pagina prima");
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "articulus", "5"));
    CREDO_VERUM(campus_est(w, "pagina", "0"));
    CREDO_VERUM(campus_est(w, "versio", "7"));

    CREDO_GRADUS("Esc ex pagina: dominium manet; Esc iterum: radix");
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    CREDO_VERUM(campus_est(w, "dominium", "wikipedia.org"));
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    CREDO_VERUM(campus_est(w, "visus", "index"));
    CREDO_VERUM(campus_est(w, "dominium", NIHIL));
    CREDO_VERUM(pictum(w, "Bookmarks", c->piscina));

    CREDO_GRADUS("mus: duplex in dominio descendit");
    CREDO_VERUM(ictus_in(w, "d1", VERUM, c->piscina));
    CREDO_VERUM(campus_est(w, "dominium", "wikipedia.org"));
    mundum_claudere(w);
}

interior vacuum
_sectio_pagina (
    CredoContextus* c)
{
    Mundus* w = mundus(c, VERUM);

    CREDO_NECESSE_NON_NIHIL(w);
    clavis_nominata(w, CLAVIS_REDITUS, XIII);   /* in dominium */
    clavis_nominata(w, CLAVIS_REDITUS, XIII);

    CREDO_GRADUS("Rhubarb: titulus, textus, pes (hospes, paginae)");
    CREDO_VERUM(campus_est(w, "articulus", "1"));
    CREDO_VERUM(pictum(w, "Rhubarb", c->piscina));
    CREDO_VERUM(pictum(w, "fleshy, edible stalks", c->piscina));
    CREDO_VERUM(pictum(w, "Rhubarb - en.wikipedia.org - 1/",
        c->piscina));

    CREDO_GRADUS("j k: paginae; limes ad initium");
    clavis(w, 'j');
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    CREDO_VERUM(pictum(w, " - 2/", c->piscina));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "pagina", "0"));
    clavis(w, 'k');
    CREDO_VERUM(campus_est(w, "pagina", "0"));

    CREDO_GRADUS("mus: sagittae pedis");
    CREDO_VERUM(ictus_in(w, "pagina-proxima", FALSUM, c->piscina));
    CREDO_VERUM(campus_est(w, "pagina", "1"));
    CREDO_VERUM(ictus_in(w, "pagina-prior", FALSUM, c->piscina));
    CREDO_VERUM(campus_est(w, "pagina", "0"));

    CREDO_GRADUS("limes ad finem: j in pagina ultima manet");
    per (;;)
    {
        chorda* a = insula_ramus_attributum(&w->m.ramus, "pagina");
        chorda  ante = a ? chorda_transcribere(*a, c->piscina)
                         : chorda_ex_literis("", c->piscina);

        clavis(w, 'j');
        a = insula_ramus_attributum(&w->m.ramus, "pagina");
        si (a && chorda_aequalis(*a, ante))
        {
            frange;
        }
    }
    CREDO_VERUM(pictum(w, "Rhubarb - en.wikipedia.org", c->piscina));

    CREDO_GRADUS("notae: pilcrow in pede si locus notas habet");
    CREDO_FALSUM(pictum(w, "\302\266", c->piscina));
    w->notae = VERUM;
    CREDO_VERUM(pictum(w, "\302\266", c->piscina));
    mundum_claudere(w);
}

interior vacuum
_sectio_versio (
    CredoContextus* c)
{
    Mundus* w = mundus(c, VERUM);
    chorda  locus;

    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("sine versione: ultima, et versio FIGITUR (locus)");
    CREDO_VERUM(argumentum(w, "navigator/pagina/5"));
    CREDO_VERUM(pictum(w, "Beta vulgaris est olus.", c->piscina));
    CREDO_VERUM(campus_est(w, "versio", "7"));

    CREDO_GRADUS("versio nominata: vetus legitur");
    CREDO_VERUM(argumentum(w, "navigator/pagina/5?versio=6"));
    CREDO_VERUM(pictum(w, "Beta est planta.", c->piscina));
    CREDO_FALSUM(pictum(w, "vulgaris", c->piscina));

    CREDO_GRADUS("locus versionem nominat (comes versionis suae)");
    locus = navigator_locus(&w->m, c->piscina);
    CREDO_VERUM(chorda_incipit(locus, chorda_ex_literis(
        "navigator/pagina/5", c->piscina)));
    CREDO_VERUM(chorda_continet(locus, chorda_ex_literis("versio=6",
        c->piscina)));
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_locus_titulus(&w->m,
        c->piscina), "Beta - la.wikipedia.org - 1/1");

    CREDO_GRADUS("index: locus sine articulo; dominium articuli");
    clavis_nominata(w, CLAVIS_EFFUGIUM, XXVII);
    locus = navigator_locus(&w->m, c->piscina);
    CREDO_FALSUM(chorda_continet(locus, chorda_ex_literis("versio",
        c->piscina)));
    CREDO_FALSUM(chorda_continet(locus, chorda_ex_literis("/5",
        c->piscina)));
    CREDO_CHORDA_AEQUALIS_LITERIS(navigator_locus_titulus(&w->m,
        c->piscina), "Saved - wikipedia.org");
    mundum_claudere(w);
}

interior vacuum
_sectio_argumentum (
    CredoContextus* c)
{
    Mundus* w = mundus(c, VERUM);

    CREDO_NECESSE_NON_NIHIL(w);

    CREDO_GRADUS("identitas nuda: articulus, pagina prima");
    CREDO_VERUM(argumentum(w, "3"));
    CREDO_VERUM(campus_est(w, "visus", "pagina"));
    CREDO_VERUM(campus_est(w, "articulus", "3"));
    CREDO_VERUM(pictum(w, "Rheum est genus plantarum.", c->piscina));

    CREDO_GRADUS("index nexuum: pagina nova post articulum (N3.1b)");
    CREDO_VERUM(pictum(w, "Rheum - la.wikipedia.org - 1/2",
        c->piscina));
    CREDO_FALSUM(pictum(w, "Links", c->piscina));
    clavis(w, 'j');
    CREDO_VERUM(pictum(w, "Links", c->piscina));
    CREDO_VERUM(pictum(w, "Planta", c->piscina));
    CREDO_FALSUM(pictum(w, "plantarum", c->piscina));
    CREDO_VERUM(pictum(w, "Rheum - la.wikipedia.org - 2/2",
        c->piscina));
    clavis(w, 'k');
    CREDO_VERUM(pictum(w, "plantarum", c->piscina));

    CREDO_GRADUS("articulus ignotus: FALSUM, nihil mutatum");
    CREDO_FALSUM(argumentum(w, "99"));
    CREDO_VERUM(campus_est(w, "articulus", "3"));
    CREDO_FALSUM(argumentum(w, "navigator/pagina/99"));
    CREDO_VERUM(campus_est(w, "articulus", "3"));

    CREDO_GRADUS("vacuum: index");
    CREDO_VERUM(argumentum(w, ""));
    CREDO_VERUM(campus_est(w, "visus", "index"));
    mundum_claudere(w);
}

interior vacuum
_sectio_vacua (
    CredoContextus* c)
{
    Mundus* w = mundus(c, FALSUM);

    CREDO_NECESSE_NON_NIHIL(w);
    CREDO_GRADUS("bibliotheca vacua: nuntius, Enter nihil");
    CREDO_VERUM(pictum(w, "No bookmarks yet", c->piscina));
    clavis_nominata(w, CLAVIS_REDITUS, XIII);
    CREDO_VERUM(campus_est(w, "visus", "index"));
    mundum_claudere(w);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "aperire", _sectio_aperire, NIHIL, mundum_purgare, NIHIL, NIHIL },
    { "errores", _sectio_errores, NIHIL, mundum_purgare, NIHIL, NIHIL },
    { "supersedere", _sectio_supersedere, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "quaestio", _sectio_quaestio, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "index", _sectio_index, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "favores", _sectio_favores, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "pagina", _sectio_pagina, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "versio", _sectio_versio, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "argumentum", _sectio_argumentum, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { "vacua", _sectio_vacua, NIHIL, mundum_purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("navigator_latus", SECTIONES);
}
