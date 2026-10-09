/* probatio_vicus_compositio.c - compositio hospitis (insula-rami-plan
 * T2b)
 *
 * Dispensator UNUS super repositorium vici (registra hospitis,
 * vicus_componere). Arbor: radix 'vicus', linea tabularum (VIII
 * pixela), arbor activae infra eam translata. Superficies ramorum =
 * fenestra minus linea (aperire, mutatio magnitudinis). Ictus in
 * coordinatis schirmi translationem sequitur. Commutatio: arbor
 * pictoris. Pixela lineae: activa colore selectionis. vicus-latera
 * S2a: latera dimidia (sinistrum cellulis rotundatum), radix lateris
 * in spatio suo, divisor supra latera. */
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
     constans character* argumentum,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)argumentum;
    (vacuum)ctx;
    redde scriba_montare((ScribaMontatio*)sedes, p, in, v, r, id, radix,
                         latitudo, altitudo, NIHIL);
}

interior b32
pictorem_montare (
                 vacuum* sedes,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* v,
     InsulaRepositorium* r,
     constans character* id,
     constans character* argumentum,
     constans character* radix,
                    i32  latitudo,
                    i32  altitudo,
                 vacuum* ctx)
{
    (vacuum)argumentum;
    (vacuum)ctx;
    redde pictor_montare((PictorMontatio*)sedes, p, in, v, r, id, radix,
                         latitudo, altitudo);
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
    f->fons           = NIHIL;
    f->fons_ctx       = NIHIL;
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
superficies_est (
                 Vicus* v,
    constans character* genus,
    constans character* id,
    constans character* latitudo,
    constans character* altitudo)
{
    InsulaRamus ramus;

    ramus = insula_ramus(v->repo, genus, id);
    redde valor_est(insula_ramus_attributum(&ramus, INSULA_EPHEMERA,
        "superficies_latitudo"), latitudo)
        && valor_est(insula_ramus_attributum(&ramus, INSULA_EPHEMERA,
        "superficies_altitudo"), altitudo);
}

interior Componens*
liberum (
           Dispensator* d,
    constans character* id)
{
    redde componens_invenire_per_id(dispensator_arbor(d),
        chorda_ex_literis(id, piscina));
}

interior i32
color (
    ColorThema c)
{
    redde color_ad_pixelum(thema_color(c));
}

interior Eventus
magnitudo_nova (
    i32 latitudo,
    i32 altitudo)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                               = EVENTUS_MUTARE_MAGNITUDINEM;
    e.tempus                              = M;
    e.datum.mutare_magnitudinem.latitudo  = latitudo;
    e.datum.mutare_magnitudinem.altitudo  = altitudo;
    redde e;
}

/* numerus pixelorum colore dato in rectangulo */
interior i32
numerare (
    TabulaPixelorum* t,
                s32  x,
                s32  y,
                s32  latitudo,
                s32  altitudo,
                i32  pix)
{
    i32 n;
    s32 i;
    s32 j;

    n = ZEPHYRUM;
    per (j = y; j < y + altitudo; j++)
    {
        per (i = x; i < x + latitudo; i++)
        {
            si (tabula_pixelorum_obtinere_pixelum(t, (i32)i, (i32)j)
                == pix)
            {
                n++;
            }
        }
    }
    redde n;
}

s32 principale (vacuum)
{
         Volumen* vol;
           Vicus* v;
     Dispensator* d;
      ManusLudus* m;
       Componens* c;
         Punctum  p;

    piscina = piscina_generare_dynamicum("probatio_vicus_compositio",
        LXIV * M);
    si (!piscina)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    thema_initiare();
    vol = volumen_temporarium(piscina, "probatio_vicus_compositio");

    v = vicus_creare(piscina, intern, vol, NIHIL, CDLXXX, CDLXXX);
    CREDO_NON_NIHIL(v);
    CREDO_VERUM(vicus_genus_addere(v, "scriba",
        magnitudo(ScribaMontatio),
        scribam_montare, scribam_describere, NIHIL));
    CREDO_VERUM(vicus_genus_addere(v, "pictor",
        magnitudo(PictorMontatio),
        pictorem_montare, pictorem_describere, NIHIL));
    CREDO_VERUM(vicus_aperire(v,
        "<tabulae activa=\"s1\">"
        "<tabula id=\"s1\"><latus genus=\"scriba\"/>"
        "<acervus><latus genus=\"pictor\"/></acervus></tabula>"
        "<tabula id=\"p1\"><latus genus=\"pictor\"/></tabula>"
        "</tabulae>"));

    imprimere("\n--- I: superficies ramorum post aperire ---\n");
    /* fenestra CDLXXX, linea VIII: latera CCXL x CDLXXII (S2a) */
    CREDO_VERUM(superficies_est(v, "scriba", "s1_sinistrum_scriba",
        "240",
        "472"));
    CREDO_VERUM(superficies_est(v, "pictor", "s1_dextrum_pictor", "240",
        "472"));
    CREDO_VERUM(superficies_est(v, "pictor", "p1_sinistrum_pictor",
        "240",
        "472"));

    imprimere("\n--- II: arbor - radix, linea, activa translata ---\n");
    d = dispensator_creare(piscina, intern, v->repo, vicus_actiones(v),
        vicus_componere, v, CCC);
    m = manus_ludus_creare(piscina, d);
    CREDO_NON_NIHIL(dispensator_arbor(d));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_arbor(d)->id, "vicus");
    c = liberum(d, "vicus.tabulae");
    CREDO_NON_NIHIL(c);
    si (c)
    {
        CREDO_AEQUALIS_S32(c->fines.y, ZEPHYRUM);
        CREDO_AEQUALIS_S32(c->fines.latitudo, CDLXXX);
        CREDO_AEQUALIS_S32(c->fines.altitudo, VIII);
    }
    /* linea status scribae in linea ULTIMA fenestrae */
    c = liberum(d, "status");
    CREDO_NON_NIHIL(c);
    si (c)
    {
        p.x  = ZEPHYRUM;
        p.y  = ZEPHYRUM;
        p    = manus_ludus_ad_schirmum(m, c, p);
        CREDO_AEQUALIS_S32(p.y, CDLXXII);
    }
    /* latus dextrum: radix pictoris in spatio suo, x CCXL */
    c = componens_invenire_in_spatio(dispensator_arbor(d),
        chorda_ex_literis("s1_dextrum_pictor", piscina),
        chorda_ex_literis("radix", piscina));
    CREDO_NON_NIHIL(c);
    si (c)
    {
        CREDO_AEQUALIS_S32(c->fines.x, CCXL);
        CREDO_AEQUALIS_S32(c->fines.y, VIII);
        CREDO_CHORDA_AEQUALIS_LITERIS(c->spatium, "s1_dextrum_pictor");
    }
    CREDO_NON_NIHIL(liberum(d, "vicus.divisor"));

    imprimere("\n--- III: ictus in coordinatis schirmi ---\n");
    /* y XII = linea marginis scribae (translata); sine translatione
     * pagina esset */
    CREDO_VERUM(manus_ludus_premere_ad(m, XXX, XII));
    CREDO_FALSUM(chorda_aequalis_literis(dispensator_focus(d),
        "pagina"));
    CREDO_VERUM(manus_ludus_premere_ad(m, XXX, XX));
    CREDO_CHORDA_AEQUALIS_LITERIS(dispensator_focus(d), "pagina");

    imprimere("\n--- IV: mutatio magnitudinis - rami sequuntur ---\n");
    {
        Eventus e;

        e = magnitudo_nova(CCC, CCCXX);
        dispensator_tractare(d, &e);
    }
    CREDO_VERUM(valor_est(insula_attributum(v->repo, INSULA_EPHEMERA,
        "superficies_altitudo"), "320"));
    /* dimidium CL (cellulis rotundatum), reliquum CL */
    CREDO_VERUM(superficies_est(v, "scriba", "s1_sinistrum_scriba",
        "150",
        "312"));
    CREDO_VERUM(superficies_est(v, "pictor", "s1_dextrum_pictor", "150",
        "312"));
    CREDO_VERUM(superficies_est(v, "pictor", "p1_sinistrum_pictor",
        "150",
        "312"));
    c = liberum(d, "status");
    CREDO_NON_NIHIL(c);
    si (c)
    {
        p.x  = ZEPHYRUM;
        p.y  = ZEPHYRUM;
        p    = manus_ludus_ad_schirmum(m, c, p);
        CREDO_AEQUALIS_S32(p.y, CCCXII);
    }
    CREDO_FALSUM(insula_mendacium(v->repo));

    imprimere("\n--- V: pixela lineae tabularum ---\n");
    {
             Mandata* md;
     TabulaPixelorum* t;

        md = mandata_creare(piscina, intern);
        pingere(dispensator_arbor(d), vicus_figurae(v), ZEPHYRUM, md);
        t = tabula_pixelorum_creare_nuda(piscina, CCC, CCCXX);
        tabula_pixelorum_vacare(t, color(COLOR_SUPERFICIES));
        delineare_mandata(md, t, vicus_imago_fons, v);
        /* activa 's1 pictor' (S2a: genus dextri a sinistro differt;
         * cellulae 0..10): fundus selectionis */
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, I, I),
            color(COLOR_SELECTION));
        /* titulus activae inversus: colore fundi, nullum textus */
        CREDO_VERUM(numerare(t, VI, ZEPHYRUM, LIV, VIII,
            color(COLOR_BACKGROUND)) > ZEPHYRUM);
        CREDO_AEQUALIS_I32(numerare(t, VI, ZEPHYRUM, LIV, VIII,
            color(COLOR_TEXT)), ZEPHYRUM);
        /* 'p1' (cellulae 11..14): fundus, textus */
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, LXVII,
            I), color(COLOR_BACKGROUND));
        CREDO_VERUM(numerare(t, LXXII, ZEPHYRUM, XII, VIII,
            color(COLOR_TEXT)) > ZEPHYRUM);
        /* divisor in x CL (dimidium post mutationem), supra latera */
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, CL, C),
            color(COLOR_BORDER));
        /* ultra tabulas: fundus lineae, non mensa */
        CREDO_AEQUALIS_I32(tabula_pixelorum_obtinere_pixelum(t, CC, IV),
            color(COLOR_BACKGROUND));
    }

    imprimere("\n--- VI: commutatio - arbor pictoris ---\n");
    CREDO_VERUM(vicus_activam_ponere(v, "p1"));
    dispensator_recomponere(d);
    CREDO_NON_NIHIL(liberum(d, "tabula"));
    CREDO_NON_NIHIL(liberum(d, "vicus.tabulae"));
    CREDO_NIHIL(liberum(d, "pagina"));

    imprimere("\n");
    credo_imprimere_compendium();
    volumen_claudere(vol);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
