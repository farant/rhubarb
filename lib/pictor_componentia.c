/* pictor_componentia.c - componere pictoris */

#include "pictor_componentia.h"
#include "xar.h"
#include "dispositio.h"

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
    redde "";
}


/* ==================================================
 * Componere
 * ================================================== */

/* <componens/> <purus/> */
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
    }

    status = nodus(piscina, intern, "status", PARTES_TITULUS, fs.x,
        fs.y,
                   fs.latitudo, fs.altitudo);
    componens_ponere_titulum(status,
        chorda_vacua(instrumentum) ? "nihil"
                                   : chorda_ut_cstr(instrumentum,
                                   piscina));

    componens_addere_liberum(prospectus, tabula);
    componens_addere_liberum(radix, prospectus);
    componens_addere_liberum(radix, status);
    redde radix;
}
