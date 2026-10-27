/* membrum__charta.c - charta notarum (include/membrum__charta.h). Ex
 * calendario translata (kalendarium A6d, A6e): scriba nidificata,
 * liber per clavem, area cellulis exacta, umbra plana. */
#include "membrum__charta.h"
#include "scriba_liber.h"
#include "scriba_actiones.h"
#include "scriba_componentia.h"
#include "componens.h"
#include "motus.h"

#include <stdio.h>
#include <string.h>

/* area in loco ad cellulas exacta: (pagina + II) x (pagina + III)
 * cellulae, intra locum minus marginem et umbram (II) */
interior Fines
area_computare (
    Fines locus,
      s32 pagina_latitudo,
      s32 pagina_altitudo)
{
    Fines f;
      s32 lat;
      s32 alt;

    f.x = locus.x + CHARTA_MARGO;
    f.y = locus.y + CHARTA_MARGO;
    lat = (locus.latitudo - II * CHARTA_MARGO - II) / CHARTA_LITTERA;
    alt = (locus.altitudo - II * CHARTA_MARGO - II) / CHARTA_LINEA;
    si (pagina_latitudo + II < lat)
    {
        lat = pagina_latitudo + II;
    }
    si (pagina_altitudo + III < alt)
    {
        alt = pagina_altitudo + III;
    }
    f.latitudo = lat > ZEPHYRUM ? lat * CHARTA_LITTERA : ZEPHYRUM;
    f.altitudo = alt > ZEPHYRUM ? alt * CHARTA_LINEA : ZEPHYRUM;
    redde f;
}

/* mensurae paginae novae ex loco (latitudo x altitudo pixela): area
 * sine termino paginae, minus margines scribae (II) et status (I) -
 * pagina nova sine volutione; area in pixelis */
interior vacuum
mensurae (
      s32  latitudo,
      s32  altitudo,
      i32* lat_cellulae,
      i32* alt_cellulae,
    Fines* area)
{
    Fines locus;

    locus.x         = ZEPHYRUM;
    locus.y         = ZEPHYRUM;
    locus.latitudo  = latitudo;
    locus.altitudo  = altitudo;
    *area           = area_computare(locus, CM, CM);
    *lat_cellulae   = (i32)(area->latitudo / CHARTA_LITTERA > IV
                    ? area->latitudo / CHARTA_LITTERA - II : II);
    *alt_cellulae   = (i32)(area->altitudo / CHARTA_LINEA > IV
                    ? area->altitudo / CHARTA_LINEA - III : I);
}

interior ScribaLiber*
librum_aperire (
                 Charta* c,
     constans character* clavis,
                    s32  latitudo,
                    s32  altitudo)
{
      i32 lc;
      i32 ac;
    Fines area;

    mensurae(latitudo, altitudo, &lc, &ac, &area);
    redde scriba_liber_aperire_in_spatio(c->piscina, c->intern,
        c->volumen, clavis, lc, ac);
}

interior vacuum
clavem_servare (
                 Charta* c,
     constans character* clavis)
{
    strncpy(c->clavis, clavis, CHARTA_CLAVIS_MAXIMA - I);
    c->clavis[CHARTA_CLAVIS_MAXIMA - I] = '\0';
}

b32
charta_montare (
                  Charta* c,
                 Piscina* piscina,
     InternamentumChorda* intern,
                 Volumen* volumen,
    constans InsulaRamus* hospes,
          ActioRegistrum* actiones,
         FiguraRegistrum* figurae,
      constans character* id,
      constans character* clavis,
                     s32  latitudo,
                     s32  altitudo,
         StatusSystemata* systemata)
{
    ScribaLiber* liber;
            i32  lc;
            i32  ac;
          Fines  area;

    si (!c)
    {
        redde FALSUM;
    }
    memset(c, ZEPHYRUM, magnitudo(Charta));
    si (!volumen)
    {
        redde VERUM;
    }
    si (   !piscina || !intern || !hospes || !actiones || !figurae
        || !id
        || !clavis || strlen(clavis) >= CHARTA_CLAVIS_MAXIMA)
    {
        redde FALSUM;
    }
    c->volumen  = volumen;
    c->piscina  = piscina;
    c->intern   = intern;
    liber       = librum_aperire(c, clavis, latitudo, altitudo);
    si (!liber)
    {
        redde FALSUM;
    }
    mensurae(latitudo, altitudo, &lc, &ac, &area);
    si (   !scriba_montare_in_ramo(&c->scriba, piscina, intern, volumen,
            hospes, id, (i32)area.latitudo, (i32)area.altitudo, liber,
            systemata)
        || !actio_registrum_miscere_in_spatio(actiones,
            c->scriba.actiones, c->scriba.ramus.via)
        || !figura_registrum_miscere_in_spatio(figurae,
            c->scriba.figurae, c->scriba.ramus.via))
    {
        redde FALSUM;
    }
    clavem_servare(c, clavis);
    c->montata = VERUM;
    redde VERUM;
}

b32
charta_clavem_ponere (
                 Charta* c,
                  Motus* motus,
     constans character* clavis,
                    s32  latitudo,
                    s32  altitudo)
{
    ScribaLiber* liber;

    si (   !c || !c->montata || !clavis
        || strlen(clavis)            >= CHARTA_CLAVIS_MAXIMA
        || strcmp(clavis, c->clavis) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (motus)
    {
        (vacuum)motus_gestum_effundere(motus, c->scriba.ramus.repo);
    }
    liber = librum_aperire(c, clavis, latitudo, altitudo);
    si (!liber || !scriba_librum_ponere(&c->scriba, liber))
    {
        redde FALSUM;
    }
    clavem_servare(c, clavis);
    redde VERUM;
}

Fines
charta_area (
    constans Charta* c,
              Fines  locus)
{
    si (!c || !c->montata || !c->scriba.doc)
    {
        locus.latitudo = ZEPHYRUM;
        locus.altitudo = ZEPHYRUM;
        redde locus;
    }
    redde area_computare(locus, (s32)c->scriba.doc->latitudo,
        (s32)c->scriba.doc->altitudo);
}

vacuum
charta_componere (
                 Charta* c,
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
              Componens* radix,
                  Fines  locus)
{
           Componens* app;
    ScribaCompositio  cfg;
               Fines  area;
               Fines  f;

    si (!c || !c->montata || !radix || locus.altitudo <= ZEPHYRUM)
    {
        redde;
    }
    area  = charta_area(c, locus);
    cfg   = c->scriba.compositio;
    cfg.fenestra_latitudo  = (i32)(area.latitudo > ZEPHYRUM
                                   ? area.latitudo : ZEPHYRUM);
    cfg.fenestra_altitudo  = (i32)(area.altitudo > ZEPHYRUM
                                   ? area.altitudo : ZEPHYRUM);
    app = scriba_componere(repo, motus, piscina, intern, &cfg);
    si (!app)
    {
        redde;
    }
    f    = app->fines;
    f.x  = area.x;
    f.y  = area.y;
    componens_ponere_fines(app, f);
    componens_ponere_spatium(app, chorda_ut_cstr(c->scriba.ramus.via,
        piscina));
    componens_addere_liberum(radix, app);
}

vacuum
charta_umbram_pingere (
    constans Charta* c,
            Mandata* md,
              Fines  locus)
{
    si (!c || !c->montata || !md)
    {
        redde;
    }
    figura_umbram_pingere(md, charta_area(c, locus));
}

b32
charta_focata (
    constans Charta* c,
     constans Motus* motus)
{
    redde c && c->montata && motus
        && motus->spatium_intimum.mensura > ZEPHYRUM
        && insula_via_continet(c->scriba.ramus.via,
               motus->spatium_intimum);
}

b32
charta_reficere (
    Charta* c)
{
    si (!c || !c->montata)
    {
        redde FALSUM;
    }
    redde scriba_reficere(&c->scriba);
}

b32
charta_occupata (
    constans Charta* c)
{
    redde c && c->montata && scriba_occupata(&c->scriba);
}

vacuum
charta_gestum_ponere (
    Charta* c,
     Motus* motus)
{
    si (!c || !c->montata || !motus)
    {
        redde;
    }
    (vacuum)scriba_reficere(&c->scriba);
    scriba_gestum_ponere(motus, &c->scriba.actiones_ctx);
}
