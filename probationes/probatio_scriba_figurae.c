/* probatio_scriba_figurae.c - componere et figurae scribae (S2)
 *
 * Arbor: fines (columna in cellulis, pagina cellula una ab ora),
 * data figurae in arbore (puncta, titulus), volutio sine statu.
 * Figurae: pixela vera (pingere -> delineare_mandata): mensa, charta,
 * margo, textus, cursor (colore modi), character sub cursore colore
 * chartae, selectio, status. */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "color.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "figura.h"
#include "mandatum.h"
#include "tabula_pixelorum.h"
#include "delineare_mandata.h"
#include "scriba_documentum.h"
#include "scriba_actiones.h"
#include "scriba_componentia.h"
#include "scriba_figurae.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior Componens*
arbor (
    constans character* ephemera,
                   i32  latitudo,
                   i32  altitudo)
{
    InsulaRepositorium* repo;
      ScribaCompositio  cfg;

    repo = insula_repositorium_creare(piscina, intern,
        "<documentum latitudo=\"16\" altitudo=\"6\"/>", ephemera);
    cfg.fenestra_latitudo  = latitudo;
    cfg.fenestra_altitudo  = altitudo;
    cfg.cellula_latitudo   = VI;
    cfg.cellula_altitudo   = VIII;
    cfg.status_lineae      = I;
    redde scriba_componere(repo, NIHIL, piscina, intern, &cfg);
}

interior Componens*
liberum (
             Componens* c,
    constans character* id)
{
    redde componens_invenire_per_id(c, chorda_ex_literis(id, piscina));
}

interior i32
color (
    ColorThema c)
{
    redde color_ad_pixelum(thema_color(c));
}

/* numerus pixelorum colore dato in cellula (pixela absoluta) */
interior i32
in_cellula (
    TabulaPixelorum* t,
                s32  x,
                s32  y,
                i32  pix)
{
    i32 n;
    s32 i;
    s32 j;

    n = ZEPHYRUM;
    per (j = ZEPHYRUM; j < VIII; j++)
    {
        per (i = ZEPHYRUM; i < VI; i++)
        {
            si (tabula_pixelorum_obtinere_pixelum(t, (i32)(x + i),
                (i32)(y + j)) == pix)
            {
                n++;
            }
        }
    }
    redde n;
}

interior TabulaPixelorum*
reddere (
        Componens* radix,
    ScribaFigurae* sf)
{
     FiguraRegistrum* reg;
             Mandata* m;
     TabulaPixelorum* t;

    reg = figura_registrum_creare(piscina);
    scriba_figurae_registrare(reg, ZEPHYRUM, sf);
    m = mandata_creare(piscina, intern);
    pingere(radix, reg, ZEPHYRUM, m);
    t = tabula_pixelorum_creare_nuda(piscina,
        (i32)radix->fines.latitudo,
        (i32)radix->fines.altitudo);
    tabula_pixelorum_vacare(t, color(COLOR_BACKGROUND));
    delineare_mandata(m, t, NIHIL, NIHIL);
    redde t;
}

/* folium (pixela): cellula (l, c) -> angulus sinister superior */
#define FOLIUM_X(c) (VI + (s32)(c) * VI)
#define FOLIUM_Y(l) (VIII + (s32)(l) * VIII)

s32 principale (vacuum)
{
             Volumen* vol;
    ScribaDocumentum* doc;
      ScribaActiones  sa;
       ScribaFigurae  sf;
           Componens* r;
           Componens* pagina;
           Componens* prosp;
           Componens* status;
     TabulaPixelorum* t;

    piscina = piscina_generare_dynamicum("probatio_scriba_figurae",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_scriba_figurae");
    doc = scriba_documentum_creare(piscina, intern, vol, XVI, VI, IV);
    scriba_actiones_initiare(&sa, doc, piscina);
    memcpy(&tabula_cellula(&sa.laboris, ZEPHYRUM, ZEPHYRUM), "salve",
        V);
    memcpy(&tabula_cellula(&sa.laboris, II, ZEPHYRUM), "mundus", VI);
    sf.sa = &sa;

    imprimere("\n--- I: arbor - fines, data figurae, status ---\n");
    r = arbor("<ephemera superficies_latitudo=\"480\""
              " superficies_altitudo=\"200\" cursor_linea=\"2\""
              " cursor_columna=\"8\" modus=\"normalis\"/>", CCCXX, CC);
    CREDO_NON_NIHIL(r);
    prosp   = liberum(r, "prospectus");
    pagina  = liberum(r, "pagina");
    status  = liberum(r, "status");
    CREDO_NON_NIHIL(prosp);
    CREDO_NON_NIHIL(pagina);
    CREDO_NON_NIHIL(status);
    CREDO_AEQUALIS_S32(prosp->fines.altitudo, CXCII);
    CREDO_AEQUALIS_S32(status->fines.y, CXCII);
    CREDO_AEQUALIS_S32(pagina->fines.x, VI);
    CREDO_AEQUALIS_S32(pagina->fines.y, VIII);
    CREDO_AEQUALIS_S32(pagina->fines.latitudo, XCVI);
    CREDO_AEQUALIS_S32(pagina->fines.altitudo, XLVIII);
    CREDO_CHORDA_AEQUALIS_LITERIS(pagina->actio, "pagina.clavis");
    CREDO_VERUM(pagina->focusabilis);
    CREDO_AEQUALIS_I32(pagina->numerus_punctorum, I);
    CREDO_AEQUALIS_S32(pagina->puncta[ZEPHYRUM].x, VIII);
    CREDO_AEQUALIS_S32(pagina->puncta[ZEPHYRUM].y, II);
    CREDO_CHORDA_AEQUALIS_LITERIS(pagina->titulus, "normalis");
    CREDO_CHORDA_AEQUALIS_LITERIS(status->titulus, "NORMALIS 3:9");
    /* totum videtur: nulla volutio */
    CREDO_AEQUALIS_S32(prosp->translatio.x, ZEPHYRUM);
    CREDO_AEQUALIS_S32(prosp->translatio.y, ZEPHYRUM);

    imprimere("\n--- II: pixela - mensa, charta, margo, cursor ---\n");
    t = reddere(r, &sf);
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, CCC, C),
                       color(COLOR_SUPERFICIES));
    CREDO_AEQUALIS_I32(in_cellula(t, FOLIUM_X(XII), FOLIUM_Y(IV),
        color(COLOR_BACKGROUND)), XLVIII);                 /* charta */
    CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, V, XX),
                       color(COLOR_BORDER));          /* margo */
    CREDO_VERUM(in_cellula(t, FOLIUM_X(ZEPHYRUM), FOLIUM_Y(ZEPHYRUM),
        color(COLOR_TEXT)) > ZEPHYRUM);                   /* 's' */
    /* cursor in cellula vacua: plena colore cursoris */
    CREDO_AEQUALIS_I32(in_cellula(t, FOLIUM_X(VIII), FOLIUM_Y(II),
        color(COLOR_STATUS_NORMAL)), XLVIII);
    /* modi colore distinguuntur (thema mutatum id non tacite
     * frangat) */
    CREDO_VERUM(color(COLOR_STATUS_INSERT)
        != color(COLOR_STATUS_NORMAL));
    /* status: modus colore suo */
    CREDO_VERUM(in_cellula(t, II, CXCII, color(COLOR_STATUS_NORMAL))
                > ZEPHYRUM);

    imprimere("\n--- III: cursor super litteram, modo inserendi ---\n");
    r = arbor("<ephemera superficies_latitudo=\"480\""
              " superficies_altitudo=\"200\" cursor_linea=\"0\""
              " cursor_columna=\"1\" modus=\"inserere\"/>", CCCXX, CC);
    t = reddere(r, &sf);
    CREDO_VERUM(in_cellula(t, FOLIUM_X(I), FOLIUM_Y(ZEPHYRUM),
        color(COLOR_STATUS_INSERT)) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(in_cellula(t, FOLIUM_X(I), FOLIUM_Y(ZEPHYRUM),
        color(COLOR_STATUS_NORMAL)), ZEPHYRUM);
    /* 'a' sub cursore colore chartae */
    CREDO_VERUM(in_cellula(t, FOLIUM_X(I), FOLIUM_Y(ZEPHYRUM),
        color(COLOR_BACKGROUND)) > ZEPHYRUM);
    CREDO_AEQUALIS_I32(in_cellula(t, FOLIUM_X(I), FOLIUM_Y(ZEPHYRUM),
        color(COLOR_TEXT)), ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS_LITERIS(liberum(r, "status")->titulus,
        "INSERERE 1:2");
    CREDO_VERUM(in_cellula(t, II, CXCII, color(COLOR_STATUS_INSERT))
                > ZEPHYRUM);

    imprimere("\n--- IV: selectio visualis (lineae totae) ---\n");
    r = arbor("<ephemera superficies_latitudo=\"480\""
              " superficies_altitudo=\"200\" cursor_linea=\"2\""
              " cursor_columna=\"0\" modus=\"visualis\""
              " selectio_linea=\"0\" selectio_columna=\"0\"/>", CCCXX,
              CC);
    pagina = liberum(r, "pagina");
    CREDO_AEQUALIS_I32(pagina->numerus_punctorum, II);
    CREDO_AEQUALIS_S32(pagina->puncta[I].y, ZEPHYRUM);
    t = reddere(r, &sf);
    CREDO_AEQUALIS_I32(in_cellula(t, FOLIUM_X(XII), FOLIUM_Y(I),
        color(COLOR_SELECTION)), XLVIII);        /* linea I selecta */
    CREDO_AEQUALIS_I32(in_cellula(t, FOLIUM_X(XII), FOLIUM_Y(III),
        color(COLOR_BACKGROUND)), XLVIII);       /* linea III non */
    CREDO_AEQUALIS_I32(in_cellula(t, FOLIUM_X(ZEPHYRUM),
        FOLIUM_Y(ZEPHYRUM),
        color(COLOR_TEXT)), ZEPHYRUM);           /* textus inversus */
    /* cursor et verbum modi colore visuali */
    CREDO_VERUM(in_cellula(t, FOLIUM_X(ZEPHYRUM), FOLIUM_Y(II),
        color(COLOR_STATUS_VISUAL)) > ZEPHYRUM);
    CREDO_VERUM(in_cellula(t, II, CXCII, color(COLOR_STATUS_VISUAL))
                > ZEPHYRUM);

    imprimere("\n--- V: volutio sine statu - cursor centratus ---\n");
    /* superficies X x V cellularum: prospectus X x IV; folium cum
     * margine XVIII x VIII */
    r = arbor("<ephemera superficies_latitudo=\"60\""
              " superficies_altitudo=\"40\" cursor_linea=\"2\""
              " cursor_columna=\"3\"/>", LX, XL);
    prosp = liberum(r, "prospectus");
    CREDO_AEQUALIS_S32(prosp->translatio.x, ZEPHYRUM);
    CREDO_AEQUALIS_S32(prosp->translatio.y, -VIII);
    r = arbor("<ephemera superficies_latitudo=\"60\""
              " superficies_altitudo=\"40\" cursor_linea=\"5\""
              " cursor_columna=\"15\"/>", LX, XL);
    prosp = liberum(r, "prospectus");
    CREDO_AEQUALIS_S32(prosp->translatio.x, -XLVIII);   /* VIII limes */
    CREDO_AEQUALIS_S32(prosp->translatio.y, -XXXII);    /* IV limes */

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
