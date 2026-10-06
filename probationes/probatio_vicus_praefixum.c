/* probatio_vicus_praefixum.c - commutatio per claves et ictum
 * (insula-rami-plan T3b)
 *
 * Ctrl-A praefixum hospitis (applicatio id numquam videt): deinde n /
 * p / 1-9 (in textu agitur, clavis devoratur), Ctrl-A iterum = tabula
 * prior, Esc aut cetera = abolitio. Linea tabularum dum pendet
 * tingitur. Litterae praefixi in scribam non effluunt; insertio trans
 * commutationem perstat. Ictus in tabulam eam activat. */
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
#include "dispensator.h"
#include "manus_ludus.h"
#include "pictor_applicatio.h"
#include "scriba_applicatio.h"
#include "vicus.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

hic_manens Piscina*             piscina;
hic_manens InternamentumChorda* intern;

interior b32
scribam_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* v,
     InsulaRepositorium* r,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
    redde scriba_montare((ScribaMontatio*)sedes, p, in, v, r, id, radix,
                         latitudo, altitudo);
}

interior b32
pictorem_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* v,
     InsulaRepositorium* r,
     constans character* id,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo)
{
    redde pictor_montare((PictorMontatio*)sedes, p, in, v, r, id, radix,
                         latitudo, altitudo);
}

interior vacuum
scribae_gestum (
     Motus* motus,
    vacuum* ctx)
{
    scriba_gestum_ponere(motus, (ScribaActiones*)ctx);
}

interior vacuum
scribam_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    ScribaMontatio* m;

    m                 = (ScribaMontatio*)montatio;
    f->actiones       = m->actiones;
    f->figurae        = m->figurae;
    f->componere      = scriba_componere;
    f->componere_ctx  = &m->compositio;
    f->gestum_ponere  = scribae_gestum;
    f->gestum_ctx     = &m->actiones_ctx;
}

interior vacuum
pictorem_describere (
         vacuum* montatio,
    VicusFacies* f)
{
    PictorMontatio* m;

    m                 = (PictorMontatio*)montatio;
    f->actiones       = m->actiones;
    f->figurae        = m->figurae;
    f->componere      = pictor_componere;
    f->componere_ctx  = &m->compositio;
    f->fons           = pictor_imago_fons;
    f->fons_ctx       = &m->figurae_ctx;
}

interior b32
valor_est (
                chorda* a,
    constans character* v)
{
    redde a ? chorda_aequalis_literis(*a, v) : FALSUM;
}

interior b32
activa_est (
                 Vicus* v,
    constans character* id)
{
    redde chorda_aequalis_literis(v->activa, id);
}

interior b32
pendet (
    Vicus* v)
{
    redde valor_est(insula_attributum(v->repo, INSULA_EPHEMERA,
        "praefixum"), "1");
}

interior b32
imperium_a (
    ManusLudus* m)
{
    redde manus_ludus_clavem(m, 'a', MOD_IMPERIUM);
}

/* linea l folii (spatiis finalibus omissis) == expectata */
interior b32
linea_est (
    constans TabulaCharacterum* t,
                           i32  l,
           constans character* expectata)
{
    i32 finis;
    i32 n;

    finis = t->latitudo;
    dum (finis > ZEPHYRUM && tabula_cellula(t, l, finis - I) == ' ')
    {
        finis--;
    }
    n = (i32)strlen(expectata);
    redde n == finis
        && memcmp(&tabula_cellula(t, l, ZEPHYRUM), expectata, (size_t)n)
           == ZEPHYRUM;
}

interior i32
color (
    ColorThema c)
{
    redde color_ad_pixelum(thema_color(c));
}

/* pixelum lineae tabularum ultra titulos */
interior i32
pixelum_lineae (
          Vicus* v,
    Dispensator* d)
{
            Mandata* md;
    TabulaPixelorum* t;

    md = mandata_creare(piscina, intern);
    pingere(dispensator_arbor(d), vicus_figurae(v), ZEPHYRUM, md);
    t = tabula_pixelorum_creare_nuda(piscina, CDLXXX, CDLXXX);
    tabula_pixelorum_vacare(t, color(COLOR_SUPERFICIES));
    delineare_mandata(md, t, vicus_imago_fons, v);
    redde tabula_pixelorum_obtinere_pixelum(t, CCC, IV);
}

s32 principale (vacuum)
{
           Volumen* vol;
             Vicus* v;
       Dispensator* d;
        ManusLudus* m;
    ScribaMontatio* s1;
    ScribaMontatio* s2;

    piscina = piscina_generare_dynamicum("probatio_vicus_praefixum",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_vicus_praefixum");

    v = vicus_creare(piscina, intern, vol, NIHIL, CDLXXX, CDLXXX);
    CREDO_NON_NIHIL(v);
    CREDO_VERUM(vicus_genus_addere(v, "scriba",
        magnitudo(ScribaMontatio),
        scribam_montare, scribam_describere));
    CREDO_VERUM(vicus_genus_addere(v, "pictor",
        magnitudo(PictorMontatio),
        pictorem_montare, pictorem_describere));
    CREDO_VERUM(vicus_aperire(v,
        "<tabulae activa=\"s1\">"
        "<tabula id=\"s1\" genus=\"scriba\" titulus=\"scriba\"/>"
        "<tabula id=\"p1\" genus=\"pictor\" titulus=\"pictor\"/>"
        "<tabula id=\"s2\" genus=\"scriba\" titulus=\"secunda\"/>"
        "</tabulae>"));
    s1 = (ScribaMontatio*)vicus_tabula(v, ZEPHYRUM)->montatio;
    s2 = (ScribaMontatio*)vicus_tabula(v, II)->montatio;
    d = dispensator_creare(piscina, intern, v->repo, vicus_actiones(v),
        vicus_componere, v, CCC);
    m = manus_ludus_creare(piscina, d);
    vicus_dispensatorem_ligare(v, d);
    CREDO_FALSUM(color(COLOR_ACCENT_PRIMARY)
        == color(COLOR_BACKGROUND));

    imprimere("\n--- I: Ctrl-A - praefixum pendens, linea tincta\n");
    CREDO_VERUM(manus_ludus_scribere(m, "iab"));
    CREDO_AEQUALIS_I32(pixelum_lineae(v, d), color(COLOR_BACKGROUND));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(pendet(v));
    CREDO_VERUM(activa_est(v, "s1"));
    CREDO_AEQUALIS_I32(pixelum_lineae(v, d),
        color(COLOR_ACCENT_PRIMARY));

    imprimere("\n--- II: n - proxima; littera non effluit ---\n");
    CREDO_VERUM(manus_ludus_scribere(m, "n"));
    CREDO_VERUM(activa_est(v, "p1"));
    CREDO_FALSUM(pendet(v));
    CREDO_VERUM(linea_est(scriba_documentum_tabula(s1->doc), ZEPHYRUM,
        "ab"));
    CREDO_AEQUALIS_I32(pixelum_lineae(v, d), color(COLOR_BACKGROUND));

    imprimere("\n--- III: p - prior in ordine; insertio perstat ---\n");
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(manus_ludus_scribere(m, "p"));
    CREDO_VERUM(activa_est(v, "s1"));
    CREDO_VERUM(manus_ludus_scribere(m, "c"));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(manus_ludus_scribere(m, "n"));
    CREDO_VERUM(linea_est(scriba_documentum_tabula(s1->doc), ZEPHYRUM,
        "abc"));

    imprimere("\n--- IV: numerus; Ctrl-A Ctrl-A = tabula prior ---\n");
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(manus_ludus_scribere(m, "3"));
    CREDO_VERUM(activa_est(v, "s2"));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(activa_est(v, "p1"));
    CREDO_FALSUM(pendet(v));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(activa_est(v, "s2"));
    /* p in prima: ultima (circulus) */
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(manus_ludus_scribere(m, "1"));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(manus_ludus_scribere(m, "p"));
    CREDO_VERUM(activa_est(v, "s2"));

    imprimere("\n--- V: abolitio - Esc, littera alia, numerus\n");
    CREDO_VERUM(manus_ludus_scribere(m, "i"));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(manus_ludus_clavem(m, (character)XXVII, ZEPHYRUM));
    CREDO_FALSUM(pendet(v));
    CREDO_VERUM(activa_est(v, "s2"));
    /* Esc devoratus: scriba in modo inserendi manet */
    CREDO_VERUM(manus_ludus_scribere(m, "z"));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(manus_ludus_scribere(m, "x"));
    CREDO_FALSUM(pendet(v));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(manus_ludus_scribere(m, "9"));
    CREDO_FALSUM(pendet(v));
    CREDO_VERUM(activa_est(v, "s2"));
    CREDO_VERUM(manus_ludus_scribere(m, "y"));
    CREDO_VERUM(imperium_a(m));
    CREDO_VERUM(manus_ludus_scribere(m, "1"));
    CREDO_VERUM(linea_est(scriba_documentum_tabula(s2->doc), ZEPHYRUM,
        "zy"));

    imprimere("\n--- VI: ictus in tabulam ---\n");
    {
        Componens* c;

        c = componens_invenire_per_id(dispensator_arbor(d),
            chorda_ex_literis("vicus.tabula.p1", piscina));
        CREDO_NON_NIHIL(c);
        si (c)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(c->titulus, "p1");
        }
    }
    /* 'scriba' cellulae 0..7, 'pictor' 8..15 */
    CREDO_VERUM(manus_ludus_premere_ad(m, LX, IV));
    CREDO_VERUM(activa_est(v, "p1"));
    CREDO_VERUM(manus_ludus_premere_ad(m, X, IV));
    CREDO_VERUM(activa_est(v, "s1"));
    CREDO_FALSUM(insula_mendacium(v->repo));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
