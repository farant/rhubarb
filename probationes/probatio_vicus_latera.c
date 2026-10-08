/* probatio_vicus_latera.c - duo latera eiusdem generis (vicus-latera
 * S2a-2)
 *
 * Compositio VERA (vicus_applicatio: dispositio ordinaria, tabula 3 =
 * scriba | scriba; concha /bin/sh pro tabula 1). Claves ad latus
 * focatum solum; ictus in latus non focatum id focat et NIHIL aliud
 * agit, ictus secundus agit (in pictore, tabula 2 - scriba ictum non
 * tractat); Motus (ramus, spatium) latus sequitur;
 * gestus relinquentis effunditur; focus durabilis; super cum spatio
 * suo (idem id 'pagina' in utroque latere); Ctrl-A 0 = tabula
 * decima. */
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
    /* scriba ictum non tractat (cursor non movetur): probatio in
     * pictore, tabula 2 = scriba | pictor */
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

    imprimere("\n--- V: Ctrl-A 0 = tabula decima ---\n");
    CREDO_VERUM(manus_ludus_clavem(m, 'a', MOD_IMPERIUM));
    CREDO_VERUM(manus_ludus_scribere(m, "0"));
    CREDO_CHORDA_AEQUALIS_LITERIS(v->activa, "10");
    CREDO_VERUM(manus_ludus_clavem(m, 'a', MOD_IMPERIUM));
    CREDO_VERUM(manus_ludus_scribere(m, "1"));
    CREDO_CHORDA_AEQUALIS_LITERIS(v->activa, "1");
    CREDO_FALSUM(insula_mendacium(v->repo));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
