/* probatio_vicus_iussa.c - iussa in pagina scribae (vicus-latera S3b)
 *
 * Compositio VERA (vicus_applicatio, tabula 3 = scriba | scriba).
 * Linea III sinistri: "$dies $foo $dies(x)". I: '$dies' colore
 * accentus pingitur, '$foo' (ignotum) non. II: ictus in '$dies' id
 * dato hodierno substituit (consumens, forma "MM/DD/YYYY"), cursor
 * post datum; latus alterum post pulsum idem videt. III: 'u' signum
 * restituit (commissio una). IV: ictus in '$foo' cursorem solum
 * ponit. V: '$dies(x)' (error: argumenta) nihil mutat. VI (S3b-2):
 * nuntius erroris in linea status (arbor, pixela colore erroris),
 * praecisus ante indicem paginae; clavis proxima et ictus proximus
 * tollunt. VII: '$dies(-1)', '$dies(7)'; '$dies(1, 2)' error; verbum
 * FALSUM sine errore -> "<verbum>: defecit". VIII: ictus primus in
 * latus non focatum iussum statim currit; extra iussum focat solum.
 * Sectio corpus (2026-10-13): '$eco<>..</>' (non consumens) textum
 * POST '</>' inserit, etiam si '</>' in linea sequenti; ictus intra
 * corpus nihil currit; consumens caput solum delet. Sectio dilatum
 * ($question): verbum 'mora' differt - ictus nuntium solum ponit,
 * pulsus primus picturam sinit, secundus verbum iterum currit (caput
 * substituitur, lineae novae infra '</>'), 'u' totum restituit; iussum
 * deletum ante pulsum: "mora: evanuit". */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "color.h"
#include "volumen.h"
#include "insula.h"
#include "eventus.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "tabula_characterum.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "scriba_documentum.h"
#include "scriba_applicatio.h"
#include "scriba_componentia.h"
#include "componens.h"
#include "iussum.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define LINEA III

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior ScribaMontatio*
visus (
    Vicus* v,
      i32  quod)
{
    redde (ScribaMontatio*)vicus_latus(vicus_activa(v), quod)->montatio;
}

/* punctum schirmi cellulae (linea, columna) paginae sinistrae */
interior vacuum
ictus (
    ManusLudus* m,
           s32  linea,
           s32  columna)
{
    (vacuum)manus_ludus_premere_ad(m, VI + columna * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII + IV);
}

/* initium lineae folii laboris est textus? */
interior b32
linea_est (
        ScribaMontatio* sm,
                   s32  linea,
    constans character* textus)
{
    constans TabulaCharacterum* t;

    t = &sm->actiones_ctx.laboris;
    redde memcmp(t->cellulae + (i32)linea * t->latitudo, textus,
        strlen(textus)) == ZEPHYRUM;
}

interior s32
attributum (
                 Vicus* v,
    constans character* titulus)
{
     VicusLatus* l;
    InsulaRamus  r;
         chorda* a;
            s32  n;

    l = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    r = insula_ramus(v->repo, "scriba", chorda_ut_cstr(l->id, piscina));
    a = insula_ramus_attributum(&r, titulus);
    si (!a || !chorda_ut_s32(*a, &n))
    {
        redde -I;
    }
    redde n;
}

/* pixela coloris dati in cellulis [a, b) lineae paginae sinistrae */
interior i32
pixela_coloris (
    TabulaPixelorum* tp,
                s32  linea,
                s32  a,
                s32  b,
                i32  color)
{
    s32 x;
    s32 y;
    i32 n;

    n = ZEPHYRUM;
    per (y = ZEPHYRUM; y < VIII; y++)
    {
        per (x = a * VI; x < b * VI; x++)
        {
            si ((i32)tabula_pixelorum_obtinere_pixelum(tp, (i32)(VI
                + x),
                    (i32)(VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII
                    + y)) == color)
            {
                n++;
            }
        }
    }
    redde n;
}

/* quadrum sine fenestra; via non NIHIL: imago scribitur */
interior TabulaPixelorum*
quadrum_pingere (
       VicusApplicatio* app,
    constans character* via)
{
    TabulaPixelorum* tp;
      LudusFenestra* lf;

    tp = tabula_pixelorum_creare_nuda(piscina, CDLXXX, CDLXXX
        + VICUS_ALTITUDO_TABULARUM);
    lf = ludus_fenestra_creare(piscina, app->d,
        vicus_figurae(app->vicus),
        ZEPHYRUM, vicus_imago_fons, app->vicus, tp);
    si (!lf)
    {
        redde NIHIL;
    }
    ludus_quadrum(lf, M);
    si (via)
    {
        (vacuum)ludus_fenestra_imaginem_scribere(lf, via);
    }
    redde tp;
}

/* nodus 'id' in arbore scribae sinistrae composita; NIHIL si abest */
interior Componens*
nodus_status (
                 Vicus* v,
        ScribaMontatio* sm,
    constans character* id)
{
    Componens* r;

    r = scriba_componere(v->repo, NIHIL, piscina, intern,
        &sm->compositio);
    redde r ? componens_invenire_per_id(r, chorda_ex_literis(id,
        piscina)) : NIHIL;
}

/* nuntius sinistri adest et praefixum (non vacuum) textus dati est */
interior b32
nuntius_est (
                 Vicus* v,
        ScribaMontatio* sm,
    constans character* textus)
{
    Componens* n;

    n = nodus_status(v, sm, "nuntius");
    redde n && n->titulus.mensura > ZEPHYRUM
        && (memoriae_index)n->titulus.mensura <= strlen(textus)
        && memcmp(n->titulus.datum, textus,
               (size_t)n->titulus.mensura) == ZEPHYRUM;
}

/* dies ab hodie, ut '$dies(N)' */
interior vacuum
dies_textus (
          s32  gradus,
    character* exitus)
{
          time_t nunc;
    structura tm  dies;

    nunc           = time(NIHIL);
    dies           = *localtime(&nunc);
    dies.tm_mday   += gradus;
    dies.tm_isdst  = -I;
    (vacuum)mktime(&dies);
    sprintf(exitus, "%02d/%02d/%04d", dies.tm_mon + I, dies.tm_mday,
        dies.tm_year + MCM);
}

/* verbum quod FALSUM sine errore reddit */
interior b32
frange_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* p,
     IussumEffectus* effectus)
{
    (vacuum)iussum;
    (vacuum)ctx;
    (vacuum)p;
    (vacuum)effectus;
    redde FALSUM;
}

/* corpus: textus "!" (ctx NIHIL: non consumens) aut "#" (ctx
 * quilibet: consumens) */
interior b32
resonare_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* p,
     IussumEffectus* effectus)
{
    (vacuum)iussum;
    effectus->textus = chorda_ex_literis(ctx ? "#" : "!", p);
    redde VERUM;
}

/* dilatum: vocatio prima differt, altera caput et lineas reddit;
 * ctx = numerus vocationum */
interior b32
mora_iussum (
    constans Iussum* iussum,
             vacuum* ctx,
            Piscina* p,
     IussumEffectus* effectus)
{
    (*(s32*)ctx)++;
    si (!iussum->iterum)
    {
        effectus->differre  = VERUM;
        effectus->nuntius   = chorda_ex_literis("mora: exspecta", p);
        redde VERUM;
    }
    effectus->caput    = chorda_ex_literis("<mora facta>", p);
    effectus->textus   = chorda_ex_literis("\nlinea una\nlinea duo", p);
    effectus->nuntius  = chorda_ex_literis("mora: facta", p);
    redde VERUM;
}

/* MUNDUS APPLICATIONIS (credo v2): applicatio vicus cum volumine
 * temporario - parare aedificat, purgare volumen SEMPER claudit (etiam post NECESSE
 * fractum: volumina temporaria relicta portas olim fregerunt). Gradus
 * I-VIII scenarium UNUM sunt (quisque statum priorem sequitur), ergo
 * sectio una, non sectiones ordine liberae. */
nomen structura {
             Volumen* vol;
     VicusApplicatio  app;
               Vicus* v;
          ManusLudus* m;
      ScribaMontatio* sin;
      ScribaMontatio* dex;
} MundusApplicationis;

interior vacuum
_applicationem_parare (
    CredoContextus* c)
{
    MundusApplicationis* ms;

    /* auxilia (attributum, nodus_status...) piscinam et intern
     * fasciculi legunt: sectionis sunt */
    piscina  = c->piscina;
    intern   = internamentum_creare(piscina);
    CREDO_NECESSE_NON_NIHIL(intern);
    ms = (MundusApplicationis*)piscina_allocare(piscina,
        magnitudo(MundusApplicationis));
    CREDO_NECESSE_NON_NIHIL(ms);
    memset(ms, ZEPHYRUM, magnitudo(MundusApplicationis));
    c->datum = ms;
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    ms->vol = volumen_temporarium(piscina, "probatio_vicus_iussa");
    CREDO_NECESSE_NON_NIHIL(ms->vol);
    CREDO_NECESSE(vicus_applicatio_aedificare(&ms->app, piscina, intern,
        ms->vol, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    ms->v = ms->app.vicus;
    ms->m = manus_ludus_creare(piscina, ms->app.d);
    CREDO_NECESSE(vicus_activam_ponere(ms->v, "3"));
    dispensator_recomponere(ms->app.d);
    ms->sin = visus(ms->v, VICUS_SINISTRUM);
    ms->dex = visus(ms->v, VICUS_DEXTRUM);
}

interior vacuum
_applicationem_purgare (
    CredoContextus* c)
{
    MundusApplicationis* ms = (MundusApplicationis*)c->datum;

    si (ms != NIHIL && ms->vol != NIHIL)
    {
        volumen_claudere(ms->vol);
    }
}

interior vacuum
_probare_iussa (
    CredoContextus* c)
{
      MundusApplicationis* ms   = (MundusApplicationis*)c->datum;
          VicusApplicatio* app  = &ms->app;
                    Vicus* v    = ms->v;
               ManusLudus* m    = ms->m;
           ScribaMontatio* sin  = ms->sin;
           ScribaMontatio* dex  = ms->dex;
                character  hodie[XXXII];
                   time_t  nunc;
       structura tm* tm;

    CREDO_NON_NIHIL(sin->actiones_ctx.iussa);
    CREDO_VERUM(sin->actiones_ctx.iussa == dex->actiones_ctx.iussa);
    ictus(m, LINEA, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$dies $foo $dies(x)"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(linea_est(sin, LINEA, "$dies $foo $dies(x)"));

    CREDO_GRADUS("I: '$dies' colore accentus, '$foo' non");
    {
        TabulaPixelorum* tp;
                    i32  accentus;

        tp = quadrum_pingere(app, NIHIL);
        CREDO_NON_NIHIL(tp);
        accentus = (i32)color_ad_pixelum(thema_color(
            COLOR_ACCENT_PRIMARY));
        CREDO_VERUM(pixela_coloris(tp, LINEA, ZEPHYRUM, V, accentus)
            > ZEPHYRUM);
        CREDO_AEQUALIS_I32(pixela_coloris(tp, LINEA, VI, X, accentus),
            ZEPHYRUM);
        CREDO_VERUM(pixela_coloris(tp, LINEA, XI, XIX, accentus)
            > ZEPHYRUM);
    }

    CREDO_GRADUS("II: ictus in '$dies' - datum hodiernum");
    nunc  = time(NIHIL);
    tm    = localtime(&nunc);
    sprintf(hodie, "%02d/%02d/%04d $foo", tm->tm_mon + I, tm->tm_mday,
        tm->tm_year + MCM);
    ictus(m, LINEA, II);
    CREDO_VERUM(linea_est(sin, LINEA, hodie));
    CREDO_AEQUALIS_S32(attributum(v, "cursor_linea"), LINEA);
    CREDO_AEQUALIS_S32(attributum(v, "cursor_columna"), X);
    /* commissum: latus alterum post pulsum */
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(linea_est(dex, LINEA, hodie));

    CREDO_GRADUS("III: 'u' signum restituit");
    CREDO_VERUM(manus_ludus_scribere(m, "u"));
    CREDO_VERUM(linea_est(sin, LINEA, "$dies $foo $dies(x)"));

    CREDO_GRADUS("IV: '$foo' ignotum - cursor solus");
    ictus(m, LINEA, VII);
    CREDO_VERUM(linea_est(sin, LINEA, "$dies $foo $dies(x)"));
    CREDO_AEQUALIS_S32(attributum(v, "cursor_columna"), VII);

    CREDO_GRADUS("V: '$dies(x)' - error, nihil mutatur");
    CREDO_NIHIL(nodus_status(v, sin, "nuntius"));
    ictus(m, LINEA, XIII);
    CREDO_VERUM(linea_est(sin, LINEA, "$dies $foo $dies(x)"));

    CREDO_GRADUS("VI: nuntius in linea status");
    {
                 Componens* n;
                 Componens* pg;
           TabulaPixelorum* tp;
                    chorda  plenus;
                       s32  x;
                       i32  error;
                       i32  numerus;
                       i32  fundum;

        plenus = chorda_ex_literis(
            "dies: numerus dierum non intellegitur: x", piscina);
        n   = nodus_status(v, sin, "nuntius");
        pg  = nodus_status(v, sin, "paginae");
        CREDO_NON_NIHIL(n);
        CREDO_NON_NIHIL(pg);
        /* praecisus: praefixum nuntii, ante indicem cellula una */
        CREDO_VERUM(n && n->titulus.mensura > ZEPHYRUM
            && n->titulus.mensura < plenus.mensura
            && memcmp(n->titulus.datum, plenus.datum,
                   (size_t)n->titulus.mensura) == ZEPHYRUM);
        CREDO_VERUM(n && pg && n->fines.x + n->fines.latitudo
            <= pg->fines.x - VI);
        /* pixela colore erroris in linea status sinistra */
        tp = quadrum_pingere(app, "build/probatio_vicus_iussa.png");
        CREDO_NON_NIHIL(tp);
        error    = (i32)color_ad_pixelum(thema_color(COLOR_ERROR));
        fundum   = ZEPHYRUM;
        numerus  = ZEPHYRUM;
        per (x = ZEPHYRUM; x < CCXL; x++)
        {
            per (fundum = ZEPHYRUM; fundum < VIII; fundum++)
            {
                si ((i32)tabula_pixelorum_obtinere_pixelum(tp, (i32)x,
                        (i32)(CDLXXX + VICUS_ALTITUDO_TABULARUM - VIII
                        + fundum)) == error)
                {
                    numerus++;
                }
            }
        }
        CREDO_VERUM(numerus > ZEPHYRUM);
    }
    /* clavis proxima tollit */
    CREDO_VERUM(manus_ludus_scribere(m, "l"));
    CREDO_NIHIL(nodus_status(v, sin, "nuntius"));
    /* ictus proximus tollit (etiam extra iussa) */
    ictus(m, LINEA, XIII);
    CREDO_NON_NIHIL(nodus_status(v, sin, "nuntius"));
    ictus(m, LINEA, VII);
    CREDO_NIHIL(nodus_status(v, sin, "nuntius"));

    CREDO_GRADUS("VII: '$dies(N)', argumenta plura, defecit");
    CREDO_VERUM(iussum_registrare(sin->actiones_ctx.iussa, "frange",
        VERUM, frange_iussum, NIHIL));
    ictus(m, V, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$dies(-1) x"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    ictus(m, VI, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$dies( 7 ) x"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    ictus(m, VII, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$dies(1, 2) $frange"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    ictus(m, V, II);
    dies_textus(-I, hodie);
    strcat(hodie, " x");
    CREDO_VERUM(linea_est(sin, V, hodie));
    ictus(m, VI, II);
    dies_textus(VII, hodie);
    strcat(hodie, " x");
    CREDO_VERUM(linea_est(sin, VI, hodie));
    ictus(m, VII, II);
    CREDO_VERUM(linea_est(sin, VII, "$dies(1, 2) $frange"));
    CREDO_VERUM(nuntius_est(v, sin, "dies: unum argumentum"));
    ictus(m, VII, XIV);
    CREDO_VERUM(linea_est(sin, VII, "$dies(1, 2) $frange"));
    CREDO_VERUM(nuntius_est(v, sin, "frange: defecit"));

    CREDO_GRADUS("VIII: ictus primus - iussum uno ictu");
    ictus(m, IX, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$dies $foo xyz"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    /* dexter focatus; ictus UNUS in '$dies' sinistri: focat, currit */
    CREDO_AEQUALIS_S32(attributum(v, "cursor_columna"), XIV);
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    dispensator_recomponere(app->d);
    ictus(m, IX, II);
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_SINISTRUM));
    dies_textus(ZEPHYRUM, hodie);
    strcat(hodie, " $foo xyz");
    CREDO_VERUM(linea_est(sin, IX, hodie));
    /* status scribae scriptus (domini: scriptor pagina.clavis) */
    CREDO_AEQUALIS_S32(attributum(v, "cursor_columna"), X);
    /* ictus primus extra iussum ('$foo' ignotum): focat solum */
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    dispensator_recomponere(app->d);
    ictus(m, IX, XII);
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_SINISTRUM));
    CREDO_AEQUALIS_S32(attributum(v, "cursor_columna"), X);
    CREDO_VERUM(linea_est(sin, IX, hodie));
}

interior vacuum
_probare_corpus (
    CredoContextus* c)
{
      MundusApplicationis* ms   = (MundusApplicationis*)c->datum;
               ManusLudus* m    = ms->m;
           ScribaMontatio* sin  = ms->sin;

    CREDO_NECESSE(iussum_registrare(sin->actiones_ctx.iussa, "eco",
        FALSUM, resonare_iussum, NIHIL));
    CREDO_NECESSE(iussum_registrare(sin->actiones_ctx.iussa, "ecoc",
        VERUM, resonare_iussum, ms));
    ictus(m, III, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$eco<>hi"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    ictus(m, IV, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "ithere</> z"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    ictus(m, VI, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$ecoc<>ab</> q"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(linea_est(sin, III, "$eco<>hi"));
    CREDO_VERUM(linea_est(sin, IV, "there</> z"));

    /* ictus intra corpus: nihil currit */
    ictus(m, III, VII);
    CREDO_VERUM(linea_est(sin, IV, "there</> z"));
    /* ictus in caput: textus post '</>' in linea IV */
    ictus(m, III, I);
    CREDO_VERUM(linea_est(sin, III, "$eco<>hi"));
    CREDO_VERUM(linea_est(sin, IV, "there</>! z"));
    /* consumens: caput deletur, textus post '</>' */
    ictus(m, VI, II);
    CREDO_VERUM(linea_est(sin, VI, "ab</># q"));
}

interior vacuum
_probare_dilatum (
    CredoContextus* c)
{
    MundusApplicationis* ms          = (MundusApplicationis*)c->datum;
        VicusApplicatio* app         = &ms->app;
                  Vicus* v           = ms->v;
             ManusLudus* m           = ms->m;
         ScribaMontatio* sin         = ms->sin;
                    s32  vocationes  = ZEPHYRUM;

    CREDO_NECESSE(iussum_registrare(sin->actiones_ctx.iussa, "mora",
        FALSUM, mora_iussum, &vocationes));
    ictus(m, XII, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "i$mora<>q</>"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(linea_est(sin, XII, "$mora<>q</>"));

    /* ictus: nuntius solum, folium intactum */
    ictus(m, XII, I);
    dispensator_recomponere(app->d);
    CREDO_AEQUALIS_S32(vocationes, I);
    CREDO_VERUM(nuntius_est(v, sin, "mora: exspecta"));
    CREDO_VERUM(linea_est(sin, XII, "$mora<>q</>"));
    /* pulsus primus: pictura sinitur, nihil currit */
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_S32(vocationes, I);
    CREDO_VERUM(linea_est(sin, XII, "$mora<>q</>"));
    /* pulsus secundus: verbum iterum, caput et lineae */
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(app->d);
    CREDO_AEQUALIS_S32(vocationes, II);
    CREDO_VERUM(linea_est(sin, XII, "<mora facta>q</>"));
    CREDO_VERUM(linea_est(sin, XIII, "linea una"));
    CREDO_VERUM(linea_est(sin, XIV, "linea duo"));
    CREDO_VERUM(nuntius_est(v, sin, "mora: facta"));
    /* pulsus ulterior: nihil iterum */
    (vacuum)vicus_pulsare(v);
    CREDO_AEQUALIS_S32(vocationes, II);
    /* 'u': commissio una - totum restituitur */
    ictus(m, XX, ZEPHYRUM);
    CREDO_VERUM(manus_ludus_scribere(m, "u"));
    CREDO_VERUM(linea_est(sin, XII, "$mora<>q</>"));
    CREDO_VERUM(linea_est(sin, XIII, "          ")
        || linea_est(sin, XIII, ""));

    /* iussum deletum ante pulsum: evanuit, nihil mutatur */
    ictus(m, XII, I);
    CREDO_AEQUALIS_S32(vocationes, III);
    ictus(m, XII, VII);
    CREDO_VERUM(manus_ludus_scribere(m, "dd"));
    (vacuum)vicus_pulsare(v);
    (vacuum)vicus_pulsare(v);
    dispensator_recomponere(app->d);
    CREDO_AEQUALIS_S32(vocationes, III);
    CREDO_VERUM(nuntius_est(v, sin, "mora: evanuit"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iussa in pagina scribae (scenarium I-VIII)", _probare_iussa,
      _applicationem_parare, _applicationem_purgare, NIHIL, NIHIL },
    { "corpus", _probare_corpus, _applicationem_parare,
      _applicationem_purgare, NIHIL, NIHIL },
    { "dilatum", _probare_dilatum, _applicationem_parare,
      _applicationem_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_iussa", SECTIONES);
}
