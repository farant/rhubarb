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

/* porta soni ficta: slotae necessariae nihil agunt; carmen horologio
 * probationis (unus cantor, ut in vico) */
hic_manens s64 ficta_pulsus;
hic_manens b32 ficta_sonat;
hic_manens i32 ficta_finita;

interior b32
_ficta_pulsare (
     vacuum* ctx,
     chorda  genus)
{
    (vacuum)ctx;
    (vacuum)genus;
    redde FALSUM;
}

interior vacuum
_ficta_agere (
    vacuum* ctx)
{
    (vacuum)ctx;
}

interior vacuum
_ficta_tacere (
    vacuum* ctx)
{
    (vacuum)ctx;
    ficta_sonat = FALSUM;
}

interior b32
_ficta_canere (
     vacuum* ctx,
     chorda  textus,
        b32  intactus)
{
    (vacuum)ctx;
    (vacuum)textus;
    (vacuum)intactus;
    redde FALSUM;
}

interior i64
_ficta_vocem_incipere (
                 vacuum* ctx,
     constans character* tibia,
                    b32  vibratus,
                    f32  nota,
                    f32  lucrum)
{
    (vacuum)ctx;
    (vacuum)tibia;
    (vacuum)vibratus;
    (vacuum)nota;
    (vacuum)lucrum;
    redde ZEPHYRUM;
}

interior vacuum
_ficta_vocem_ponere (
    vacuum* ctx,
       i64  vox,
       f32  nota,
       f32  lucrum,
       i32  ms)
{
    (vacuum)ctx;
    (vacuum)vox;
    (vacuum)nota;
    (vacuum)lucrum;
    (vacuum)ms;
}

interior vacuum
_ficta_vocem_dimittere (
    vacuum* ctx,
       i64  vox)
{
    (vacuum)ctx;
    (vacuum)vox;
}

interior b32
_ficta_carmen_incipere (
     vacuum* ctx,
     chorda  carmen,
        s64  pulsus)
{
    (vacuum)ctx;
    (vacuum)carmen;
    ficta_sonat   = VERUM;
    ficta_pulsus  = pulsus;
    redde VERUM;
}

interior vacuum
_ficta_carmen_quaerere (
    vacuum* ctx,
       s64  pulsus)
{
    (vacuum)ctx;
    ficta_pulsus = pulsus;
}

interior vacuum
_ficta_carmen_sistere (
    vacuum* ctx)
{
    (vacuum)ctx;
    ficta_finita++;
    ficta_sonat = FALSUM;
}

interior s64
_ficta_carmen_pulsus (
    vacuum* ctx)
{
    (vacuum)ctx;
    redde ficta_sonat ? ficta_pulsus : -I;
}

/* sectio possessoris: porta ficta quieta, deinde mundus */
interior vacuum
_possessorem_parare (
    CredoContextus* c)
{
    ficta_sonat   = FALSUM;
    ficta_pulsus  = -I;
    ficta_finita  = ZEPHYRUM;
    mundum_parare(c);
}

/* latus antiphonale in tabula data super plagulam via */
interior AntiphonaleMontatio*
_antiphonale_in_tabula (
                Mundus* x,
    constans character* tabula,
    constans character* via)
{
    VicusLatus* l;

    si (   !vicus_activam_ponere(x->app.vicus, tabula)
        || !vicus_acervo_mutare(x->app.vicus, "antiphonale", via))
    {
        redde NIHIL;
    }
    (vacuum)vicus_pulsare(x->app.vicus);
    (vacuum)vicus_pulsare(x->app.vicus);
    l = latus_antiphonale(x->app.vicus);
    redde l ? (AntiphonaleMontatio*)l->montatio : NIHIL;
}

/* recensio finalis: duo latera (tabulis diversis) unum cantorem
 * communicant - solum latus quod carmen incepit pulsum videt et
 * sistere aut quaerere potest */
interior vacuum
_possessorem_probare (
    CredoContextus* c)
{
                 Mundus* x = (Mundus*)c->datum;
         VicusPortaSoni  porta;
    AntiphonaleMontatio* ma;
    AntiphonaleMontatio* mb;
                 chorda  carmen;
              character  a[CCLVI];
              character  b[CCLVI];

    CREDO_NECESSE(aedificare(x, c));
    sprintf(a, "%s/a.gabc", credo_area(c));
    sprintf(b, "%s/b.gabc", credo_area(c));
    CREDO_NECESSE(filum_scribere_literis(a,
        "name:Alpha;\n%%\n(c4) a(g)\n"));
    CREDO_NECESSE(filum_scribere_literis(b,
        "name:Beta;\n%%\n(c4) b(h)\n"));
    memset(&porta, ZEPHYRUM, magnitudo(porta));
    porta.pulsare          = _ficta_pulsare;
    porta.agere            = _ficta_agere;
    porta.tacere           = _ficta_tacere;
    porta.canere           = _ficta_canere;
    porta.vocem_incipere   = _ficta_vocem_incipere;
    porta.vocem_ponere     = _ficta_vocem_ponere;
    porta.vocem_dimittere  = _ficta_vocem_dimittere;
    porta.carmen_incipere  = _ficta_carmen_incipere;
    porta.carmen_quaerere  = _ficta_carmen_quaerere;
    porta.carmen_sistere   = _ficta_carmen_sistere;
    porta.carmen_pulsus    = _ficta_carmen_pulsus;
    vicus_applicatio_soni_portam_ponere(&x->app, &porta);
    ma = _antiphonale_in_tabula(x, "2", a);
    mb = _antiphonale_in_tabula(x, "3", b);
    CREDO_NECESSE(ma != NIHIL && mb != NIHIL && ma != mb);
    carmen = chorda_ex_literis("<carmina/>", c->piscina);

    CREDO_GRADUS("A incipit: B pulsum non videt");
    CREDO_VERUM(ma->porta.incipere(ma->porta.ctx, carmen, ZEPHYRUM));
    ficta_pulsus = CDLXXX;
    CREDO_AEQUALIS_S64(ma->porta.pulsus(ma->porta.ctx), CDLXXX);
    CREDO_AEQUALIS_S64(mb->porta.pulsus(mb->porta.ctx), -I);

    CREDO_GRADUS("B incipit: A pulsum amittit, sistere/quaerere A "
                 "nihil agunt");
    CREDO_VERUM(mb->porta.incipere(mb->porta.ctx, carmen, CMLX));
    CREDO_AEQUALIS_S64(ma->porta.pulsus(ma->porta.ctx), -I);
    CREDO_AEQUALIS_S64(mb->porta.pulsus(mb->porta.ctx), CMLX);
    ma->porta.sistere(ma->porta.ctx);
    ma->porta.quaerere(ma->porta.ctx, ZEPHYRUM);
    CREDO_AEQUALIS_I32(ficta_finita, ZEPHYRUM);
    CREDO_AEQUALIS_S64(mb->porta.pulsus(mb->porta.ctx), CMLX);

    CREDO_GRADUS("B sistit: cantor sistitur");
    mb->porta.sistere(mb->porta.ctx);
    CREDO_AEQUALIS_I32(ficta_finita, I);
    CREDO_AEQUALIS_S64(mb->porta.pulsus(mb->porta.ctx), -I);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iussum $antiphonale", _iussum_probare, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "praeco antiphonale", _praeco_probare, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "possessor carminis", _possessorem_probare, _possessorem_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_antiphonale", SECTIONES);
}
