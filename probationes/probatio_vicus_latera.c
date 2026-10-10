/* probatio_vicus_latera.c - duo latera eiusdem generis (vicus-latera
 * S2a-2)
 *
 * Compositio VERA (vicus_applicatio: dispositio ordinaria, tabula 3 =
 * scriba | scriba; concha /bin/sh pro tabula 1). Claves ad latus
 * focatum solum; ictus in latus non focatum id focat et NIHIL aliud
 * agit, ictus secundus agit (in pictore, tabula 2 - ictus pictus
 * visibilis); Motus (ramus, spatium) latus sequitur;
 * gestus relinquentis effunditur; focus durabilis; super cum spatio
 * suo (idem id 'pagina' in utroque latere); Cmd+0 = tabula
 * decima. S3a: ictus in scriba cursorem ponit (inserendo modum servat,
 * unitatem revocandi frangit; visualis ad normalem; dexter nihil).
 * VII: indicium foci - exemplar in margine lateris focati solum. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "motus.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "scriba_applicatio.h"
#include "pictor_applicatio.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "eventus.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "color.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior ScribaMontatio*
scriba (
    Vicus* v,
      i32  quod)
{
    redde (ScribaMontatio*)vicus_latus(vicus_activa(v), quod)->montatio;
}

/* attributum ephemerum rami lateris (cursor) */
interior s32
cursor_columna (
    Vicus* v,
      i32  quod)
{
     VicusLatus* l;
    InsulaRamus  r;
         chorda* a;
            s32  n;

    l = vicus_latus(vicus_activa(v), quod);
    r = insula_ramus(v->repo, "scriba", chorda_ut_cstr(l->id, piscina));
    a = insula_ramus_attributum(&r, INSULA_EPHEMERA, "cursor_columna");
    si (!a || !chorda_ut_s32(*a, &n))
    {
        redde ZEPHYRUM;
    }
    redde n;
}

interior s32
cursor_linea (
    Vicus* v,
      i32  quod)
{
     VicusLatus* l;
    InsulaRamus  r;
         chorda* a;
            s32  n;

    l = vicus_latus(vicus_activa(v), quod);
    r = insula_ramus(v->repo, "scriba", chorda_ut_cstr(l->id, piscina));
    a = insula_ramus_attributum(&r, INSULA_EPHEMERA, "cursor_linea");
    si (!a || !chorda_ut_s32(*a, &n))
    {
        redde -I;
    }
    redde n;
}

interior b32
modus_est (
                  Vicus* v,
     constans character* modus)
{
     VicusLatus* l;
    InsulaRamus  r;
         chorda* a;

    l = vicus_latus(vicus_activa(v), VICUS_SINISTRUM);
    r = insula_ramus(v->repo, "scriba", chorda_ut_cstr(l->id, piscina));
    a = insula_ramus_attributum(&r, INSULA_EPHEMERA, "modus");
    redde a && chorda_aequalis_literis(*a, modus);
}

/* punctum schirmi cellulae (linea, columna) paginae sinistrae: pagina
 * a cellula I lateris, latus sub tabulis */
interior vacuum
ictus_sinister (
    ManusLudus* m,
           s32  linea,
           s32  columna)
{
    (vacuum)manus_ludus_premere_ad(m, VI + columna * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII + IV);
}

/* cellula documenti sinistri (commissi) */
interior character
cellula (
    ScribaMontatio* sm,
               s32  linea,
               s32  columna)
{
    constans TabulaCharacterum* t;

    t = scriba_documentum_tabula(sm->doc);
    redde t->cellulae[(i32)linea * t->latitudo + (i32)columna];
}

/* quadrum sine fenestra; pixela colore marginis in x [a, b), linea
 * marginis summa sub tabulis (intra lineam ipsam) */
interior i32
puncta_marginis (
    VicusApplicatio* app,
                s32  a,
                s32  b)
{
    TabulaPixelorum* tp;
      LudusFenestra* lf;
                i32  margo;
                i32  n;
                s32  x;
                s32  y;

    tp = tabula_pixelorum_creare_nuda(piscina, CDLXXX, CDLXXX
        + VICUS_ALTITUDO_TABULARUM);
    lf = ludus_fenestra_creare(piscina, app->d,
        vicus_figurae(app->vicus),
        ZEPHYRUM, vicus_imago_fons, app->vicus, tp);
    si (!lf)
    {
        redde ZEPHYRUM;
    }
    ludus_quadrum(lf, M);
    margo  = (i32)color_ad_pixelum(thema_color(COLOR_BORDER));
    n      = ZEPHYRUM;
    per (y = VICUS_ALTITUDO_TABULARUM + I;
         y < VICUS_ALTITUDO_TABULARUM + VII; y++)
    {
        per (x = a; x < b; x++)
        {
            si ((i32)tabula_pixelorum_obtinere_pixelum(tp, (i32)x,
                    (i32)y) == margo)
            {
                n++;
            }
        }
    }
    redde n;
}

interior b32
linea_prima (
        ScribaMontatio* sm,
    constans character* textus)
{
    redde memcmp(scriba_documentum_tabula(sm->doc)->cellulae, textus,
        strlen(textus)) == ZEPHYRUM;
}

interior b32
spatium_est (
                chorda  c,
    constans character* textus)
{
    redde chorda_aequalis_literis(c, textus);
}

s32 principale (vacuum)
{
           Volumen* vol;
   VicusApplicatio  app;
             Vicus* v;
        ManusLudus* m;
             Motus* motus;
    PictorMontatio* pm;
            chorda  index;
               b32  inventum;

    piscina = piscina_generare_dynamicum("probatio_vicus_latera",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    vol = volumen_temporarium(piscina, "probatio_vicus_latera");
    CREDO_VERUM(vicus_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v      = app.vicus;
    m      = manus_ludus_creare(piscina, app.d);
    motus  = dispensator_motus(app.d);

    imprimere("\n--- 0: terminale focum in apertura capit ---\n");
    /* Franus: 'tmux a' statim post aperturam ad concham */
    CREDO_AEQUALIS_I32(vicus_tabula(v, ZEPHYRUM)->focus, VICUS_DEXTRUM);
    CREDO_AEQUALIS_I32(vicus_tabula(v, II)->focus, VICUS_SINISTRUM);
    CREDO_VERUM(spatium_est(motus->spatium, "1_dextrum_terminale"));

    imprimere("\n--- I: scriba | scriba - claves ad sinistrum ---\n");
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(app.d);
    CREDO_VERUM(spatium_est(motus->spatium, "3_sinistrum_scriba"));
    /* sine Esc: insertio in GESTU pendet usque ad commutationem */
    CREDO_VERUM(manus_ludus_scribere(m, "iuno"));
    CREDO_FALSUM(linea_prima(scriba(v, VICUS_SINISTRUM), "uno"));

    imprimere("\n--- II: ictus in dextrum - focus solum ---\n");
    /* latus dextrum ab x CCXL; pagina a cellula I lateris */
    CREDO_VERUM(manus_ludus_premere_ad(m, CCLV, XX));
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_DEXTRUM));
    CREDO_VERUM(spatium_est(motus->spatium, "3_dextrum_scriba"));
    /* gestus relinquentis effusus: textus sinistri commissus */
    CREDO_VERUM(linea_prima(scriba(v, VICUS_SINISTRUM), "uno"));
    CREDO_VERUM(manus_ludus_scribere(m, "iabcdef"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    /* claves ad dextrum solum (Esc domus cursorem non retrahit) */
    CREDO_AEQUALIS_S32(cursor_columna(v, VICUS_DEXTRUM), VI);
    CREDO_AEQUALIS_S32(cursor_columna(v, VICUS_SINISTRUM), III);
    /* ictus in sinistrum: focus redit, textus dextri commissus */
    CREDO_VERUM(manus_ludus_premere_ad(m, IX, XX));
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_SINISTRUM));
    CREDO_VERUM(linea_prima(scriba(v, VICUS_DEXTRUM), "abcdef"));

    imprimere("\n--- III: ictus primus focat, secundus agit ---\n");
    /* probatio in pictore (ictus pictus visibilis), tabula 2 =
     * scriba | pictor */
    CREDO_VERUM(vicus_activam_ponere(v, "2"));
    dispensator_recomponere(app.d);
    pm = (PictorMontatio*)vicus_latus(vicus_activa(v),
        VICUS_DEXTRUM)->montatio;
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(pm->doc), ZEPHYRUM);
    CREDO_VERUM(manus_ludus_premere_ad(m, CCCLX, CC));
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_DEXTRUM));
    CREDO_VERUM(spatium_est(motus->spatium, "2_dextrum_pictor"));
    /* primus: focus solum, nullus ictus pictus */
    CREDO_AEQUALIS_S64(pictor_documentum_cursor(pm->doc), ZEPHYRUM);
    CREDO_VERUM(manus_ludus_premere_ad(m, CCCLX, CC));
    CREDO_VERUM(pictor_documentum_cursor(pm->doc) > ZEPHYRUM);
    /* focus durabilis */
    index = volumen_plagulam_promere(vol, chorda_ex_literis(
        "vicus/latera", piscina), piscina, &inventum);
    CREDO_VERUM(chorda_continet(index, chorda_ex_literis(
        "<tabula id=\"2\" focus=\"dextrum\">", piscina)));
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(app.d);

    imprimere("\n--- IV: super cum spatio suo ---\n");
    CREDO_VERUM(manus_ludus_movere(m, XV, XX));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_super(app.d), "pagina");
    CREDO_VERUM(spatium_est(app.d->super_spatium,
        "3_sinistrum_scriba"));
    CREDO_VERUM(manus_ludus_movere(m, CCLV, XX));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_super(app.d), "pagina");
    CREDO_VERUM(spatium_est(app.d->super_spatium, "3_dextrum_scriba"));

    imprimere("\n--- V: Cmd+0 = tabula decima ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, '0', MOD_SUPER));
    CREDO_CHORDA_AEQUALIS_LITERIS(v->activa, "10");
    CREDO_VERUM(manus_ludus_clavem(m, '1', MOD_SUPER));
    CREDO_CHORDA_AEQUALIS_LITERIS(v->activa, "1");
    /* terminalis focatus omnem Cmd devorat (brevitates fenestrae):
     * hospes Cmd+numerus ANTE eum capit */
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    dispensator_recomponere(app.d);
    CREDO_VERUM(spatium_est(motus->spatium, "1_dextrum_terminale"));
    CREDO_VERUM(manus_ludus_clavem(m, '2', MOD_SUPER));
    CREDO_CHORDA_AEQUALIS_LITERIS(v->activa, "2");
    CREDO_VERUM(manus_ludus_clavem(m, '1', MOD_SUPER));
    CREDO_CHORDA_AEQUALIS_LITERIS(v->activa, "1");
    CREDO_FALSUM(insula_mendacium(v->repo));

    imprimere("\n--- VI: ictus cursorem ponit (S3a) ---\n");
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(app.d);
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_SINISTRUM));
    /* sinister in inserendo ex sectione I (sine Esc) */
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(modus_est(v, "normalis"));
    ictus_sinister(m, II, IV);
    CREDO_AEQUALIS_S32(cursor_linea(v, VICUS_SINISTRUM), II);
    CREDO_AEQUALIS_S32(cursor_columna(v, VICUS_SINISTRUM), IV);
    /* in inserendo: ictus modum servat, insertionem claudit */
    CREDO_VERUM(manus_ludus_scribere(m, "iab"));
    ictus_sinister(m, II, X);
    CREDO_VERUM(modus_est(v, "inserere"));
    CREDO_AEQUALIS_S32(cursor_columna(v, VICUS_SINISTRUM), X);
    CREDO_VERUM(manus_ludus_scribere(m, "c"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(cellula(scriba(v, VICUS_SINISTRUM), II, IV) == 'a');
    CREDO_VERUM(cellula(scriba(v, VICUS_SINISTRUM), II, V) == 'b');
    CREDO_VERUM(cellula(scriba(v, VICUS_SINISTRUM), II, X) == 'c');
    /* 'u': unitas post ictum sola revocatur */
    CREDO_VERUM(manus_ludus_scribere(m, "u"));
    CREDO_FALSUM(cellula(scriba(v, VICUS_SINISTRUM), II, X) == 'c');
    CREDO_VERUM(cellula(scriba(v, VICUS_SINISTRUM), II, IV) == 'a');
    /* visualis (domus: lineae solum, 'V'): ictus ad normalem,
     * selectio tollitur */
    CREDO_VERUM(manus_ludus_scribere(m, "Vj"));
    CREDO_VERUM(modus_est(v, "visualis"));
    ictus_sinister(m, I, I);
    CREDO_VERUM(modus_est(v, "normalis"));
    CREDO_AEQUALIS_S32(cursor_linea(v, VICUS_SINISTRUM), I);
    CREDO_AEQUALIS_S32(cursor_columna(v, VICUS_SINISTRUM), I);
    {
        InsulaRamus  r;
             chorda* a;
                s32  n;

        r = insula_ramus(v->repo, "scriba", "3_sinistrum_scriba");
        a = insula_ramus_attributum(&r, INSULA_EPHEMERA,
            "selectio_linea");
        CREDO_VERUM(a && chorda_ut_s32(*a, &n) && n == -I);
    }
    /* bottone dextro: nihil */
    {
        Eventus e;

        memset(&e, ZEPHYRUM, magnitudo(Eventus));
        e.genus        = EVENTUS_MUS_DEPRESSUS;
        e.tempus       = M * M;
        e.datum.mus.x  = VI + VII * VI + III;
        e.datum.mus.y       = VICUS_ALTITUDO_TABULARUM + VIII + III
            * VIII + IV;
        e.datum.mus.botton  = MUS_DEXTER;
        dispensator_tractare(app.d, &e);
    }
    CREDO_AEQUALIS_S32(cursor_linea(v, VICUS_SINISTRUM), I);
    CREDO_AEQUALIS_S32(cursor_columna(v, VICUS_SINISTRUM), I);

    imprimere("\n--- VII: indicium foci - exemplar in margine ---\n");
    /* linea marginis summa (y T+1..T+6, intra lineam marginis): puncta
     * colore marginis solum in latere focato */
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_SINISTRUM));
    CREDO_VERUM(puncta_marginis(&app, I, CCXXX) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(puncta_marginis(&app, CCL, CDLXX), ZEPHYRUM);
    CREDO_VERUM(vicus_focum_ponere(v, VICUS_DEXTRUM));
    CREDO_AEQUALIS_I32(puncta_marginis(&app, I, CCXXX), ZEPHYRUM);
    CREDO_VERUM(puncta_marginis(&app, CCL, CDLXX) > ZEPHYRUM);

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
