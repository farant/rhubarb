/* probatio_vicus_antiphonale.c - antiphonale in vico (gabc pars II P6):
 * '$antiphonale(~/a.gabc)' in pagina scribae scriptum et ictum latus
 * antiphonale in fronte dextra aperit ("~/" = HOME, hic area
 * probationis); '$antiphonale(~/b.gabc)' idem latus plagulam novam
 * accipit (vicus_acervo_mutare), non latus alterum. */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "volumen.h"
#include "filum.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "antiphonale.h"
#include "thema.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "credo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

nomen structura {
            Volumen* volumen;
    VicusApplicatio  app;
} Mundus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    c->datum = x;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_antiphonale");
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

/* latus frontale dextrum tabulae activae, si antiphonale */
interior VicusLatus*
latus_antiphonale (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = t ? vicus_latus(t, VICUS_DEXTRUM) : NIHIL;
    si (   !l || !l->montata
        || !chorda_aequalis_literis(l->genus, "antiphonale"))
    {
        redde NIHIL;
    }
    redde l;
}

/* verbum in pagina scribae scriptum et ictum (ut probatio_vicus_
 * chronica verbum_ferire) */
interior vacuum
verbum_ferire (
                Mundus* x,
            ManusLudus* m,
    constans character* verbum)
{
    (vacuum)manus_ludus_premere_ad(m, VI + III, VICUS_ALTITUDO_TABULARUM
        + VIII + II * VIII + IV);
    (vacuum)manus_ludus_scribere(m, "0d$i");
    (vacuum)manus_ludus_scribere(m, verbum);
    (vacuum)manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM);
    (vacuum)manus_ludus_premere_ad(m, VI + III * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + II * VIII + IV);
    (vacuum)vicus_pulsare(x->app.vicus);
    (vacuum)vicus_pulsare(x->app.vicus);
}

interior vacuum
_iussum_probare (
    CredoContextus* c)
{
        Mundus* x = (Mundus*)c->datum;
         Vicus* v;
    ManusLudus* m;
    VicusLatus* l;
        vacuum* montatio;
        chorda  id;
     character  via[CCLVI];

    CREDO_NECESSE(aedificare(x, c));
    CREDO_NECESSE(setenv("HOME", credo_area(c), I) == ZEPHYRUM);
    sprintf(via, "%s/a.gabc", credo_area(c));
    CREDO_NECESSE(filum_scribere_literis(via,
        "name:Alpha;\n%%\n(c4) a(g)\n"));
    sprintf(via, "%s/b.gabc", credo_area(c));
    CREDO_NECESSE(filum_scribere_literis(via,
        "name:Beta;\n%%\n(c4) b(h)\n"));
    v = x->app.vicus;
    m = manus_ludus_creare(c->piscina, x->app.d);
    CREDO_NECESSE_NON_NIHIL(m);
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(x->app.d);

    CREDO_GRADUS("$antiphonale(~/a.gabc): latus novum");
    verbum_ferire(x, m, "$antiphonale(~/a.gabc)");
    l = latus_antiphonale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_CHORDA_AEQUALIS_LITERIS(antiphonale_titulus(l->montatio,
        c->piscina), "Alpha");
    montatio  = l->montatio;
    id        = l->id;
    /* P7: porta soni in montatione (mutus et porta nulla in ea
     * iudicantur) */
    CREDO_VERUM(((AntiphonaleMontatio*)montatio)->porta.incipere
        != NIHIL);
    CREDO_VERUM(((AntiphonaleMontatio*)montatio)->porta.pulsus
        != NIHIL);

    CREDO_GRADUS("$antiphonale(~/b.gabc): idem latus, plagula nova");
    verbum_ferire(x, m, "$antiphonale(~/b.gabc)");
    l = latus_antiphonale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(l->montatio == montatio);
    CREDO_VERUM(chorda_aequalis(l->id, id));
    CREDO_CHORDA_AEQUALIS_LITERIS(antiphonale_titulus(l->montatio,
        c->piscina), "Beta");
}

/* lineam eventus "antiphonale" in librum praeconii appendit */
interior b32
eventum_scribere (
     constans character* liber,
                    s64  tempus,
     constans character* via,
                Piscina* piscina)
{
    character linea[DXII];

    sprintf(linea, "{\"tempus\":%lld,\"sessio\":\"\",\"eventus\":"
        "\"antiphonale\",\"panellus\":\"\",\"servitor\":\"\",\"via\":"
        "\"\",\"transcriptum\":\"\",\"genus\":\"\","
        "\"nuntius\":\"%s\"}\n",
        (long long)tempus, via);
    redde filum_appendere_firmiter(liber, chorda_ex_literis(linea,
        piscina));
}

/* P8: praeco antiphonale - eventus recentissimus post initium vici
 * latus antiphonale aperit aut mutat; vetus (ante initium)
 * neglegitur */
interior vacuum
_praeco_probare (
    CredoContextus* c)
{
        Mundus* x = (Mundus*)c->datum;
         Vicus* v;
    VicusLatus* l;
        vacuum* montatio;
     character  liber[CCLVI];
     character  a[CCLVI];
     character  b[CCLVI];
           s64  nunc;

    CREDO_NECESSE(aedificare(x, c));
    v = x->app.vicus;
    sprintf(liber, "%s/colloquia.jsonl", credo_area(c));
    sprintf(a, "%s/a.gabc", credo_area(c));
    sprintf(b, "%s/b.gabc", credo_area(c));
    CREDO_NECESSE(filum_scribere_literis(a,
        "name:Alpha;\n%%\n(c4) a(g)\n"));
    CREDO_NECESSE(filum_scribere_literis(b,
        "name:Beta;\n%%\n(c4) b(h)\n"));
    nunc = (s64)time(NIHIL) * M;
    CREDO_NECESSE(eventum_scribere(liber, CD, a, c->piscina));
    CREDO_NECESSE(vicus_activam_ponere(v, "3"));
    CREDO_VERUM(vicus_applicatio_admonitiones_aperire(&x->app, liber));

    /* cauda libri in apertione numquam inter eventus pulsus venit
     * (sequax a fine legit); eventus tempore ante initium vici post
     * apertionem lectus (liber rotatus et relectus, horologium
     * scriptoris retro) per tempus neglegitur */
    CREDO_GRADUS("eventus ante initium vici: neglegitur");
    CREDO_NECESSE(eventum_scribere(liber, CD + I, a, c->piscina));
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    CREDO_NIHIL(latus_antiphonale(v));

    CREDO_GRADUS("eventus novus: latus antiphonale in fronte");
    CREDO_NECESSE(eventum_scribere(liber, nunc + LX * M, b,
        c->piscina));
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    l = latus_antiphonale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_CHORDA_AEQUALIS_LITERIS(antiphonale_titulus(l->montatio,
        c->piscina), "Beta");
    montatio = l->montatio;

    CREDO_GRADUS("eventus alter: idem latus, plagula nova");
    CREDO_NECESSE(eventum_scribere(liber, nunc + LXI * M, a,
        c->piscina));
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    l = latus_antiphonale(v);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(l->montatio == montatio);
    CREDO_CHORDA_AEQUALIS_LITERIS(antiphonale_titulus(l->montatio,
        c->piscina), "Alpha");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iussum $antiphonale", _iussum_probare, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "praeco antiphonale", _praeco_probare, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_antiphonale", SECTIONES);
}
