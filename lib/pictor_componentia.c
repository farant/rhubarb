/* pictor_componentia.c - componere pictoris */

#include "pictor_componentia.h"
#include "xar.h"
#include "dispositio.h"
#include "pictor_documentum.h"
#include "exemplaria.h"

#include <stdio.h>
#include <string.h>


/* ==================================================
 * Auxilia (lectio insularum)
 * ================================================== */

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
             InsulaGenus  genus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_ramus_attributum(ramus, genus, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

interior chorda
attributum_chorda (
    constans InsulaRamus* ramus,
             InsulaGenus  genus,
      constans character* titulus)
{
    chorda* a;
    chorda  vacua;

    a = insula_ramus_attributum(ramus, genus, titulus);
    si (a)
    {
        redde *a;
    }
    vacua.mensura  = ZEPHYRUM;
    vacua.datum    = NIHIL;
    redde vacua;
}

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  w,
                    s32  h)
{
     Componens* c;
         Fines  f;

    c           = componens_creare(p, in, id, partes);
    f.x         = x;
    f.y         = y;
    f.latitudo  = w;
    f.altitudo  = h;
    componens_ponere_fines(c, f);
    redde c;
}

/* Fines cellularum -> pixela nostra: orae ad cellulas, PRAETER oram
 * quae superficiem tangit - ea ad superficiem ipsam (fenestra:
 * superficies non semper cellularum multiplex - nulla fascia vacua ad
 * marginem; terminalis: idem, superficies semper multiplex). */
interior Fines
_ad_pixela (
    Fines f,
      s32 columnae,
      s32 lineae,
      s32 latitudo,
      s32 altitudo,
      s32 cw,
      s32 ch)
{
    Fines p;

    p.x = f.x * cw;
    p.y = f.y * ch;
    p.latitudo  = (f.x + f.latitudo >= columnae) ? latitudo - p.x
                                                  : f.latitudo * cw;
    p.altitudo  = (f.y + f.altitudo >= lineae) ? altitudo - p.y
                                                : f.altitudo * ch;
    redde p;
}

constans character*
pictor_actio_instrumenti (
    chorda instrumentum)
{
    si (chorda_aequalis_literis(instrumentum, "penicillus"))
    {
        redde "penicillus.ictus";
    }
    si (chorda_aequalis_literis(instrumentum, "aspergillum"))
    {
        redde "aspergillum.ictus";
    }
    si (chorda_aequalis_literis(instrumentum, "spongia"))
    {
        redde "spongia.ictus";
    }
    redde "";
}


/* ==================================================
 * Componere
 * ================================================== */

/* P1a: quadratum lineae status (PARTES_BOTTONE, XX x XX) filium
 * status addere */
interior vacuum
quadratum_addere (
                Piscina* piscina,
    InternamentumChorda* intern,
              Componens* status,
     constans character* id,
                    s32  x,
                    s32  y,
     constans character* titulus)
{
    Componens* q;

    q = nodus(piscina, intern, id, PARTES_BOTTONE, x, y, XX, XX);
    componens_ponere_titulum(q, titulus);
    /* P1b: ictus palettam suam aperit */
    componens_ponere_actio(q, "palette.aperire");
    componens_addere_liberum(status, q);
}

/* P1b: palette (PARTES_DIALOGUS) supra quadratum (x, y radicis):
 * optiones XX x XX, II inter, IV margo, VI per lineam (exemplaria X).
 * genus 'instrumentum', 'color_primus'/'color_secundus' aut
 * 'exemplar' (P3; optiones coloribus primo et secundo, ut
 * quadratum); electum = valor currens (titulus ':electum') */
interior Componens*
palettam_componere (
                Piscina* piscina,
    InternamentumChorda* intern,
                 chorda  genus,
                    s32  x,
                    s32  y_quadrati,
                 chorda  instrumentum,
                    s32  color_currens,
                    s32  primus,
                    s32  secundus)
{
              Componens* palette;
              Componens* o;
                    b32  colores;
                    b32  exemplaria;
                    s32  n;
                    s32  per_lineam;
                    s32  k;
                    s32  latitudo;
                    s32  altitudo;
                    s32  valor;
              character  id[LXIV];
              character  titulus[LXIV];
     constans character* instrumenta[III];

    instrumenta[ZEPHYRUM]  = "penicillus";
    instrumenta[I]         = "aspergillum";
    instrumenta[II]        = "spongia";
    exemplaria             = chorda_aequalis_literis(genus, "exemplar");
    colores     = !exemplaria
               && !chorda_aequalis_literis(genus, "instrumentum");
    n           = exemplaria ? (s32)EXEMPLAR_NUMERUS : colores ? XVII
                                                               : III;
    /* P3: exemplaria X per lineam (IV lineae), cetera VI */
    per_lineam  = exemplaria ? X : VI;
    latitudo = IV + (n < per_lineam ? n : per_lineam) * (XX + II) - II
             + IV;
    altitudo = IV + ((n + per_lineam - I) / per_lineam) * (XX + II) - II
             + IV;
    palette   = nodus(piscina, intern, "palette", PARTES_DIALOGUS, x,
        y_quadrati - altitudo - II, latitudo, altitudo);
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (exemplaria)
        {
            /* Franus: optiones coloribus electis (ut quadratum) */
            sprintf(id, "optio.exemplar.%d", (integer)k);
            sprintf(titulus, "exemplar:%d:%d:%d%s", (integer)k,
                (integer)primus, (integer)secundus,
                k == color_currens ? ":electum" : "");
        }
        alioquin si (colores)
        {
            valor = k - I;   /* -1 nullus, deinde 0..15 */
            sprintf(id, "optio.%s.%d", chorda_ut_cstr(genus, piscina),
                (integer)valor);
            sprintf(titulus, "color:%d%s", (integer)valor,
                valor == color_currens ? ":electum" : "");
        }
        alioquin
        {
            sprintf(id, "optio.instrumentum.%s", instrumenta[k]);
            sprintf(titulus, "instrumentum:%s%s", instrumenta[k],
                chorda_aequalis_literis(instrumentum, instrumenta[k])
                    ? ":electum" : "");
        }
        o = nodus(piscina, intern, id, PARTES_BOTTONE,
            IV + (k % per_lineam) * (XX + II),
            IV + (k / per_lineam) * (XX + II), XX, XX);
        componens_ponere_titulum(o, titulus);
        /* actio = dominus attributi (domini.stml) */
        componens_ponere_actio(o, exemplaria ? "exemplar.ponere"
            : !colores ? "instrumentum.eligere"
            : chorda_aequalis_literis(genus, "color_primus")
            ? "color_primus.ponere" : "color_secundus.ponere");
        componens_addere_liberum(palette, o);
    }
    redde palette;
}

/* <componens/> <purus/> */
/* indicium foci (vicus-latera): ramus focatus - sine Motu aut ramo
 * radicis (applicatio sola) semper; in vico spatium Motus = id rami */
interior b32
focatum_est (
    constans Motus* motus,
       InsulaRamus  ramus)
{
    redde !motus || chorda_vacua(ramus.id)
        || chorda_aequalis(motus->spatium, ramus.id);
}

Componens*
pictor_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
           InsulaRamus  ramus;
      PictorCompositio* cfg;
             Componens* radix;
             Componens* prospectus;
             Componens* tabula;
             Componens* status;
                chorda  instrumentum;
                   s32  doc_latitudo;
                   s32  doc_altitudo;
                   s32  zoom;
                   s32  latitudo;
                   s32  altitudo;
                   s32  cw;
                   s32  ch;
            Dispositio* dispositio;
       DispositioForma  forma;
                   s32  d_radix;
                   s32  d_prospectus;
                   s32  d_status;
                 Fines  fp;
                 Fines  fs;
                   i32  n;
                   i32  i;

    si (!repo || !piscina || !intern || !ctx)
    {
        redde NIHIL;
    }
    cfg = (PictorCompositio*)ctx;
    /* R3: status pictoris per ramum (sine eo radix repositorii) */
    ramus = cfg->ramus.repo ? cfg->ramus : insula_ramus_radix(repo);
    /* 013 B3: superficies STATUS est (dispensator scribit); ante
     * nuntium primum magnitudo configurata */
    latitudo = attributum_s32(&ramus, INSULA_EPHEMERA,
        "superficies_latitudo",
        (s32)cfg->fenestra_latitudo);
    altitudo = attributum_s32(&ramus, INSULA_EPHEMERA,
        "superficies_altitudo",
        (s32)cfg->fenestra_altitudo);
    cw = (cfg->cellula_latitudo > ZEPHYRUM) ? (s32)cfg->cellula_latitudo
                                            : I;
    ch = (cfg->cellula_altitudo > ZEPHYRUM) ? (s32)cfg->cellula_altitudo
                                            : I;

    /* dispositio in cellulis: columna = prospectus (crescens,
     * praecisus) super lineam status (fixa) - orae ad cellulas */
    dispositio = dispositio_creare(piscina);
    dispositio_formam_initiare(&forma);
    forma.directio        = DISPOSITIO_COLUMNA;
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_CRESCENS;
    d_radix               = dispositio_addere(dispositio, -I, &forma);
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_CRESCENS;
    forma.praecidere_x    = VERUM;
    forma.praecidere_y    = VERUM;
    d_prospectus = dispositio_addere(dispositio, d_radix,
        &forma);
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus = DISPOSITIO_CRESCENS;
    forma.altitudo.genus = DISPOSITIO_FIXA;
    forma.altitudo.valor = (s32)cfg->status_lineae;
    d_status = dispositio_addere(dispositio, d_radix,
        &forma);
    dispositio_computare(dispositio, latitudo / cw, altitudo / ch,
        NIHIL,
        NIHIL);
    fp = _ad_pixela(dispositio_fines(dispositio, d_prospectus), latitudo
        / cw,
        altitudo / ch, latitudo, altitudo, cw, ch);
    fs = _ad_pixela(dispositio_fines(dispositio, d_status), latitudo
        / cw,
        altitudo / ch, latitudo, altitudo, cw, ch);
    instrumentum = attributum_chorda(&ramus, INSULA_EPHEMERA,
        "instrumentum");
    doc_latitudo = attributum_s32(&ramus, INSULA_DURABILIS, "latitudo",
        I);
    doc_altitudo = attributum_s32(&ramus, INSULA_DURABILIS, "altitudo",
        I);
    zoom = attributum_s32(&ramus, INSULA_EPHEMERA, "zoom", I);
    si (zoom < I)
    {
        zoom = I;
    }

    radix = nodus(piscina, intern, "radix", PARTES_NULLUM, ZEPHYRUM,
                  ZEPHYRUM, latitudo, altitudo);
    componens_ponere_actio(radix, "instrumentum.eligere");

    prospectus = nodus(piscina, intern, "prospectus", PARTES_PROSPECTUS,
                       fp.x, fp.y, fp.latitudo, fp.altitudo);
    componens_ponere_sectio(prospectus, VERUM);
    /* indicium foci: figura prospectus exemplar addit */
    si (focatum_est(motus, ramus))
    {
        componens_ponere_titulum(prospectus, "focatum");
    }
    componens_ponere_transformatio(prospectus,
        motus ? motus->pan.x : ZEPHYRUM,
        motus ? motus->pan.y : ZEPHYRUM,
        (i32)zoom);

    /* margo cellulae utrimque (ut folium scribae): margo paginae
     * (figura) in cellulis marginis visibilis, non extra prospectum */
    tabula = nodus(piscina, intern, "tabula", PARTES_TABULA, cw, ch,
                   doc_latitudo, doc_altitudo);
    componens_ponere_praedicatum(tabula, PRAEDICATUM_PROPRIUS);
    componens_ponere_focusabilis(tabula, VERUM);
    componens_ponere_actio(tabula,
        pictor_actio_instrumenti(instrumentum));
    n = motus
        && motus->ictus_pendens ? xar_numerus(motus->ictus_pendens)
                                      : ZEPHYRUM;
    si (n > ZEPHYRUM)
    {
        tabula->puncta = (Punctum*)piscina_allocare(piscina,
            (memoriae_index)n * magnitudo(Punctum));
        per (i = ZEPHYRUM; i < n; i++)
        {
            tabula->puncta[i] =
                *(Punctum*)xar_obtinere(motus->ictus_pendens,
                                                        i);
        }
        tabula->numerus_punctorum = n;
        /* aspergillum: praevisio guttarum - semen et radius in titulo
         * tabulae (data, ut puncta; figura ex eis guttas pingit) */
        si (chorda_aequalis_literis(instrumentum, "aspergillum"))
        {
            character titulus_guttarum[XLVIII];

            /* "semen radius color color_secundus exemplar" (Franus:
             * praevisio colore vero, non accentus; P3 exemplar) */
            sprintf(titulus_guttarum, "%ld %d %d %d %d",
                (longus)attributum_s32(&ramus, INSULA_EPHEMERA, "semen",
                    ZEPHYRUM),
                (integer)(PICTOR_ASPERGILLI_RADIUS
                          * attributum_s32(&ramus, INSULA_EPHEMERA,
                                "magnitudo", I)),
                (integer)attributum_s32(&ramus, INSULA_EPHEMERA,
                    "color_primus", ZEPHYRUM),
                (integer)attributum_s32(&ramus, INSULA_EPHEMERA,
                    "color_secundus", -I),
                (integer)attributum_s32(&ramus, INSULA_EPHEMERA,
                    "exemplar", ZEPHYRUM));
            componens_ponere_titulum(tabula, titulus_guttarum);
        }
        /* spongia: latus quadrati in titulo (figura quadrata colore
         * fundi pingit) */
        si (chorda_aequalis_literis(instrumentum, "spongia"))
        {
            character titulus_spongiae[XVI];

            sprintf(titulus_spongiae, "%d", (integer)(
                PICTOR_SPONGIAE_LATUS * attributum_s32(&ramus,
                    INSULA_EPHEMERA, "magnitudo", I)));
            componens_ponere_titulum(tabula, titulus_spongiae);
        }
    }

    status = nodus(piscina, intern, "status", PARTES_TITULUS, fs.x,
        fs.y,
                   fs.latitudo, fs.altitudo);
    componens_ponere_titulum(status,
        chorda_vacua(instrumentum) ? "nihil"
                                   : chorda_ut_cstr(instrumentum,
                                   piscina));

    /* P1a: quadrata (XX x XX, cellula inter se, centrata in linea):
     * instrumentum, color primus, color secundus (nullus ordinarie,
     * -1) - titulus dicit quid figura pingat */
    {
        character t[XLVIII];
              s32 y;

        y = fs.altitudo > XX ? (fs.altitudo - XX) / II : ZEPHYRUM;
        sprintf(t, "instrumentum:%s", chorda_vacua(instrumentum)
            ? "nihil" : chorda_ut_cstr(instrumentum, piscina));
        quadratum_addere(piscina, intern, status,
            "quadratum.instrumentum", cw, y, t);
        sprintf(t, "color:%d", (integer)attributum_s32(&ramus,
            INSULA_EPHEMERA, "color_primus", ZEPHYRUM));
        quadratum_addere(piscina, intern, status,
            "quadratum.color_primus", II * cw + XX, y, t);
        sprintf(t, "color:%d", (integer)attributum_s32(&ramus,
            INSULA_EPHEMERA, "color_secundus", -I));
        quadratum_addere(piscina, intern, status,
            "quadratum.color_secundus", III * cw + II * XX, y, t);
        /* P3: exemplar coloribus veris - "exemplar:<n>:<primus>:
         * <secundus>" */
        sprintf(t, "exemplar:%d:%d:%d", (integer)attributum_s32(&ramus,
            INSULA_EPHEMERA, "exemplar", ZEPHYRUM),
            (integer)attributum_s32(&ramus, INSULA_EPHEMERA,
                "color_primus", ZEPHYRUM),
            (integer)attributum_s32(&ramus, INSULA_EPHEMERA,
                "color_secundus", -I));
        quadratum_addere(piscina, intern, status, "quadratum.exemplar",
            IV * cw + III * XX, y, t);
    }

    componens_addere_liberum(prospectus, tabula);
    componens_addere_liberum(radix, prospectus);
    componens_addere_liberum(radix, status);
    /* P1b: palette aperta - filius ULTIMUS radicis (super tabulam
     * pingitur, ictus primum capit) */
    {
        chorda palette;
           s32 qx;
           s32 qy;

        palette = attributum_chorda(&ramus, INSULA_EPHEMERA, "palette");
        qy = fs.y + (fs.altitudo > XX ? (fs.altitudo - XX) / II
                                      : ZEPHYRUM);
        qx = chorda_aequalis_literis(palette, "color_primus")
            ? II * cw + XX
            : chorda_aequalis_literis(palette, "color_secundus")
            ? III * cw + II * XX
            : chorda_aequalis_literis(palette, "exemplar")
            ? IV * cw + III * XX : cw;
        si (   chorda_aequalis_literis(palette, "instrumentum")
            || chorda_aequalis_literis(palette, "color_primus")
            || chorda_aequalis_literis(palette, "color_secundus")
            || chorda_aequalis_literis(palette, "exemplar"))
        {
            componens_addere_liberum(radix, palettam_componere(piscina,
                intern, palette, fs.x + qx, qy, instrumentum,
                attributum_s32(&ramus, INSULA_EPHEMERA,
                    chorda_ut_cstr(palette, piscina),
                    chorda_aequalis_literis(palette, "color_secundus")
                    ? -I : ZEPHYRUM),
                attributum_s32(&ramus, INSULA_EPHEMERA, "color_primus",
                    ZEPHYRUM),
                attributum_s32(&ramus, INSULA_EPHEMERA,
                "color_secundus",
                    -I)));
        }
    }
    redde radix;
}
