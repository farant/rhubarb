/* pictor_componentia.c - componere pictoris */

#include "pictor_componentia.h"
#include "xar.h"

#include <string.h>


/* ==================================================
 * Auxilia (lectio insularum)
 * ================================================== */

interior s32
attributum_s32 (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_attributum(repo, genus, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

interior chorda
attributum_chorda (
    InsulaRepositorium* repo,
           InsulaGenus  genus,
    constans character* titulus)
{
    chorda* a;
    chorda  vacua;

    a = insula_attributum(repo, genus, titulus);
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
    PictorCompositio* cfg;
           Componens* radix;
           Componens* prospectus;
           Componens* tabula;
           Componens* status;
              chorda  instrumentum;
                 s32  doc_latitudo;
                 s32  doc_altitudo;
                 s32  zoom;
                 s32  status_altitudo;
                 i32  n;
                 i32  i;

    si (!repo || !piscina || !intern || !ctx)
    {
        redde NIHIL;
    }
    cfg              = (PictorCompositio*)ctx;
    status_altitudo  = (s32)cfg->status_altitudo;
    instrumentum = attributum_chorda(repo, INSULA_EPHEMERA,
        "instrumentum");
    doc_latitudo = attributum_s32(repo, INSULA_DURABILIS, "latitudo",
        I);
    doc_altitudo = attributum_s32(repo, INSULA_DURABILIS, "altitudo",
        I);
    zoom = attributum_s32(repo, INSULA_EPHEMERA, "zoom", I);
    si (zoom < I)
    {
        zoom = I;
    }

    radix = nodus(piscina, intern, "radix", PARTES_NULLUM, ZEPHYRUM,
                  ZEPHYRUM, (s32)cfg->fenestra_latitudo,
                  (s32)cfg->fenestra_altitudo);
    componens_ponere_actio(radix, "instrumentum.eligere");

    prospectus = nodus(piscina, intern, "prospectus", PARTES_NULLUM,
                       ZEPHYRUM, ZEPHYRUM, (s32)cfg->fenestra_latitudo,
                       (s32)cfg->fenestra_altitudo - status_altitudo);
    componens_ponere_sectio(prospectus, VERUM);
    componens_ponere_transformatio(prospectus,
        motus ? motus->pan.x : ZEPHYRUM,
        motus ? motus->pan.y : ZEPHYRUM,
        (i32)zoom);

    tabula = nodus(piscina, intern, "tabula", PARTES_TABULA, ZEPHYRUM,
                   ZEPHYRUM, doc_latitudo, doc_altitudo);
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

    status = nodus(piscina, intern, "status", PARTES_TITULUS, ZEPHYRUM,
                   (s32)cfg->fenestra_altitudo - status_altitudo,
                   (s32)cfg->fenestra_latitudo, status_altitudo);
    componens_ponere_titulum(status,
        chorda_vacua(instrumentum) ? "nihil"
                                   : chorda_ut_cstr(instrumentum,
                                   piscina));

    componens_addere_liberum(prospectus, tabula);
    componens_addere_liberum(radix, prospectus);
    componens_addere_liberum(radix, status);
    redde radix;
}
