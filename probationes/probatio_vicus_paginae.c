/* probatio_vicus_paginae.c - visus scribae super paginas communes
 * (vicus-latera S2b-3)
 *
 * Compositio VERA (vicus_applicatio; tabula 3 = scriba | scriba, ambo
 * in pagina "1"). Documentum UNUM; textus sinistri (sine Esc) in
 * dextro post ictum (commutatio effundit, focus reficit); mutatio
 * dextri in sinistrum non focatum per pulsum; scriptura sinistri
 * postea mutationem dextri SERVAT (visus stalus numquam scribit);
 * Ctrl+Shift+Dexter paginam novam "2" (visus solus movetur); commissio
 * per quietem medio in scribendo litteras sequentes non delet; pagina
 * visus per reaperturam servatur. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "eventus.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "scriba_applicatio.h"
#include "scriba_componentia.h"
#include "componens.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior ScribaMontatio*
visus (
    Vicus* v,
      i32  tabula,
      i32  quod)
{
    redde (ScribaMontatio*)vicus_latus(vicus_tabula(v, tabula),
        quod)->montatio;
}

/* linea prima folii laboris (quod visus ostendit) */
interior b32
laboris_est (
        ScribaMontatio* m,
    constans character* textus)
{
    redde memcmp(m->actiones_ctx.laboris.cellulae, textus,
        strlen(textus)) == ZEPHYRUM;
}

interior b32
continet (
         ScribaMontatio* m,
              character  c)
{
    i32 i;

    per (i = ZEPHYRUM; i < m->actiones_ctx.laboris.latitudo; i++)
    {
        si (m->actiones_ctx.laboris.cellulae[i] == c)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* index paginae in linea status (arbor scribae visus composita);
 * vacuus si nodus abest */
interior chorda
index_paginae (
              Vicus* v,
     ScribaMontatio* sm)
{
    Componens* r;
    Componens* c;

    r = scriba_componere(v->repo, NIHIL, piscina, intern,
        &sm->compositio);
    c = r ? componens_invenire_per_id(r, chorda_ex_literis("paginae",
        piscina)) : NIHIL;
    redde c ? c->titulus : chorda_ex_literis("", piscina);
}

/* Ctrl+Shift+sagitta (manus sagittas non mittit) */
interior vacuum
paginam_mutare (
    Dispensator* d,
       clavis_t  sagitta,
            s64  tempus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus                     = tempus;
    e.datum.clavis.clavis        = sagitta;
    e.datum.clavis.modificantes  = MOD_IMPERIUM | MOD_SHIFT;
    dispensator_tractare(d, &e);
}

interior chorda
pagina_visus (
                Volumen* vol,
     constans character* id)
{
    chorda c;
       b32 inventum;

    c = volumen_plagulam_promere(vol, chorda_concatenare(
        chorda_ex_literis("scriba/visus/", piscina),
        chorda_ex_literis(id, piscina), piscina), piscina, &inventum);
    redde inventum ? c : chorda_ex_literis("", piscina);
}

s32 principale (vacuum)
{
           Volumen* vol;
   VicusApplicatio  app;
   VicusApplicatio  app_altera;
             Vicus* v;
        ManusLudus* m;
    ScribaMontatio* sin;
    ScribaMontatio* dex;

    piscina = piscina_generare_dynamicum("probatio_vicus_paginae",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    vol = volumen_temporarium(piscina, "probatio_vicus_paginae");
    CREDO_VERUM(vicus_applicatio_aedificare(&app, piscina, intern, vol,
        NIHIL, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM));
    v = app.vicus;
    m = manus_ludus_creare(piscina, app.d);
    CREDO_VERUM(vicus_activam_ponere(v, "3"));
    dispensator_recomponere(app.d);
    sin = visus(v, II, VICUS_SINISTRUM);
    dex = visus(v, II, VICUS_DEXTRUM);

    imprimere("\n--- I: duo visus, documentum unum ---\n");
    CREDO_NON_NIHIL(sin->liber);
    CREDO_VERUM(sin->liber == dex->liber);
    CREDO_VERUM(sin->actiones_ctx.doc == dex->actiones_ctx.doc);
    CREDO_VERUM(sin->doc == dex->doc);
    CREDO_CHORDA_AEQUALIS_LITERIS(index_paginae(v, sin), "pagina 1/1");
    CREDO_CHORDA_AEQUALIS_LITERIS(index_paginae(v, dex), "pagina 1/1");

    imprimere("\n--- II: textus sinistri in dextro post ictum ---\n");
    /* sine Esc: insertio in gestu sinistri pendet */
    CREDO_VERUM(manus_ludus_scribere(m, "iuno"));
    CREDO_FALSUM(laboris_est(dex, "uno"));
    CREDO_VERUM(manus_ludus_premere_ad(m, CCLV, XX));
    CREDO_VERUM(vicus_latus_focatum(v)
        == vicus_latus(vicus_activa(v), VICUS_DEXTRUM));
    /* commutatio effundit, focus reficit */
    CREDO_VERUM(laboris_est(dex, "uno"));

    imprimere("\n--- III: mutatio dextri in sinistro per pulsum ---\n");
    CREDO_VERUM(manus_ludus_scribere(m, "iX"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(continet(dex, 'X'));
    /* sinister non focatus: stalus usque ad pulsum */
    CREDO_FALSUM(continet(sin, 'X'));
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_VERUM(continet(sin, 'X'));
    CREDO_FALSUM(vicus_pulsare(v));

    imprimere("\n--- IV: sinister scribens mutata dextri servat ---\n");
    /* dexter iterum committit (Y); ictus in sinistrum SINE pulsu:
     * focus ipse reficere debet, aliter Z super folium vetus Y
     * reverteret */
    CREDO_VERUM(manus_ludus_scribere(m, "iY"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_FALSUM(continet(sin, 'Y'));
    CREDO_VERUM(manus_ludus_premere_ad(m, IX, XX));
    CREDO_VERUM(continet(sin, 'Y'));
    CREDO_VERUM(manus_ludus_scribere(m, "iZ"));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(continet(sin, 'Z'));
    CREDO_VERUM(continet(sin, 'Y'));
    CREDO_VERUM(continet(sin, 'X'));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(continet(dex, 'Z'));
    CREDO_VERUM(continet(dex, 'Y'));

    imprimere("\n--- V: Ctrl+Shift+Dexter - pagina nova '2' ---\n");
    paginam_mutare(app.d, CLAVIS_DEXTER, M * C);
    CREDO_VERUM(sin->actiones_ctx.doc != dex->actiones_ctx.doc);
    /* m->doc per pulsum (scriba_reficere) cum actionibus concordat */
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(sin->doc == sin->actiones_ctx.doc);
    CREDO_FALSUM(continet(sin, 'X'));
    CREDO_VERUM(continet(dex, 'X'));
    CREDO_AEQUALIS_I32(scriba_liber_numerus(sin->liber), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(pagina_visus(vol,
        "3_sinistrum_scriba"),
        "2");
    /* index: sinister in pagina nova, dexter numerum novum (pulsus) */
    CREDO_CHORDA_AEQUALIS_LITERIS(index_paginae(v, sin), "pagina 2/2");
    CREDO_CHORDA_AEQUALIS_LITERIS(index_paginae(v, dex), "pagina 1/2");
    /* retro: pagina '1' iterum, documentum dextri idem */
    paginam_mutare(app.d, CLAVIS_SINISTER, M * C + I);
    CREDO_VERUM(sin->actiones_ctx.doc == dex->actiones_ctx.doc);
    CREDO_VERUM(continet(sin, 'X'));
    CREDO_VERUM(vicus_pulsare(v));
    CREDO_CHORDA_AEQUALIS_LITERIS(index_paginae(v, sin), "pagina 1/2");
    /* ante primam: nihil */
    paginam_mutare(app.d, CLAVIS_SINISTER, M * C + II);
    CREDO_CHORDA_AEQUALIS_LITERIS(pagina_visus(vol,
        "3_sinistrum_scriba"),
        "1");
    paginam_mutare(app.d, CLAVIS_DEXTER, M * C + III);
    /* imago sine fenestra (oculis inspicienda): "pagina 2/2" sinistra,
     * "pagina 1/2" dextra, in lineis status */
    (vacuum)vicus_pulsare(v);
    {
        TabulaPixelorum* tp;
          LudusFenestra* lf;
                    i32  x;
                    i32  y;
                    i32  diversi;
                    i32  fundum;

        tp = tabula_pixelorum_creare_nuda(piscina, CDLXXX, CDLXXX
            + VICUS_ALTITUDO_TABULARUM);
        lf = ludus_fenestra_creare(piscina, app.d, vicus_figurae(v),
            ZEPHYRUM, vicus_imago_fons, v, tp);
        CREDO_NON_NIHIL(lf);
        ludus_quadrum(lf, M * C + IV);
        CREDO_VERUM(ludus_fenestra_imaginem_scribere(lf,
            "build/probatio_vicus_paginae.png"));
        /* index PICTUS: in linea status sinistra (infima, VIII),
         * dextra textus status (x >= CXX), pixela a fundo diversa */
        y        = CDLXXX + VICUS_ALTITUDO_TABULARUM - IV;
        fundum   = tabula_pixelorum_obtinere_pixelum(tp, C, y);
        diversi  = ZEPHYRUM;
        per (x = CXX; x < CCXXXVI; x++)
        {
            si (tabula_pixelorum_obtinere_pixelum(tp, x, y) != fundum)
            {
                diversi++;
            }
        }
        CREDO_VERUM(diversi > ZEPHYRUM);
    }

    imprimere("\n--- VI: commissio per quietem, scribere pergit ---\n");
    /* insertio commissa post quietem (SCRIBA_QUIES_MS) dum scribitur:
     * visus focatus non stalus fit (cursor_laboris post commissionem
     * suam) - pulsus litteras nondum commissas non delet */
    CREDO_VERUM(manus_ludus_scribere(m, "iab"));
    dispensator_pulsare(app.d, M * M);
    CREDO_VERUM(manus_ludus_scribere(m, "c"));
    (vacuum)vicus_pulsare(v);
    CREDO_VERUM(continet(sin, 'c'));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_VERUM(memcmp(scriba_documentum_tabula(sin->actiones_ctx.doc)
        ->cellulae, "abc", III) == ZEPHYRUM);

    imprimere("\n--- VII: pagina visus per reaperturam servatur ---\n");
    CREDO_VERUM(vicus_applicatio_aedificare(&app_altera, piscina,
        intern, vol, NIHIL, CDLXXX,
        CDLXXX + VICUS_ALTITUDO_TABULARUM));
    sin = visus(app_altera.vicus, II, VICUS_SINISTRUM);
    dex = visus(app_altera.vicus, II, VICUS_DEXTRUM);
    CREDO_VERUM(sin->actiones_ctx.doc != dex->actiones_ctx.doc);
    CREDO_CHORDA_AEQUALIS_LITERIS(scriba_liber_nomen(sin->liber, I),
        "2");
    CREDO_VERUM(continet(dex, 'X'));
    CREDO_FALSUM(continet(sin, 'X'));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
