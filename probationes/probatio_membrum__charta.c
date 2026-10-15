/* probatio_membrum__charta.c - charta notarum (nomenclator-plan U5):
 * area cellulis exacta (pagina, locus minor, locus vacuus), paginae
 * novae ex loco, clavis librum vertit (liber prior servatur; clavis
 * eadem nihil agit), registra hospitis, focus, compositio in area,
 * umbra, non montata (volumen NIHIL) nihil agit */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "motus.h"
#include "mandatum.h"
#include "actio.h"
#include "figura.h"
#include "thema.h"
#include "scriba_documentum.h"
#include "scriba_liber.h"
#include "tabula_characterum.h"
#include "membrum__charta.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
     InternamentumChorda* intern;
      InsulaRepositorium* repo;
             InsulaRamus  hospes;
          ActioRegistrum* actiones;
         FiguraRegistrum* figurae;
                 Volumen* volumen;
                  Charta  charta;
} Mundus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    x->intern    = internamentum_creare(c->piscina);
    x->repo      = insula_repositorium_creare(c->piscina, x->intern,
        "<hospes><latus id=\"h1\"/></hospes>");
    x->actiones  = actio_registrum_creare(c->piscina, x->intern);
    x->figurae   = figura_registrum_creare(c->piscina);
    si (x->repo)
    {
        x->hospes = insula_ramus(x->repo, "latus", "h1");
    }
    c->datum = x;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    si (x && x->volumen)
    {
        volumen_claudere(x->volumen);
    }
}

/* volumen temporarium et charta montata (locus CCCLX x CCXL, clavis
 * "persona/1") */
interior b32
montare (
     Mundus* x,
    Piscina* p)
{
    x->volumen = volumen_temporarium(p, "probatio_membrum__charta");
    redde x->volumen && x->repo && x->actiones && x->figurae
        && charta_montare(&x->charta, p, x->intern, x->volumen,
               &x->hospes, x->actiones, x->figurae, "h1_notae",
               "persona/1", CCCLX, CCXL, NIHIL);
}

interior Fines
fines (
    s32 sinistra,
    s32 summa,
    s32 lat,
    s32 alt)
{
    Fines f;

    f.x         = sinistra;
    f.y         = summa;
    f.latitudo  = lat;
    f.altitudo  = alt;
    redde f;
}

interior b32
fines_sunt (
    Fines f,
      s32 sinistra,
      s32 summa,
      s32 lat,
      s32 alt)
{
    si (   f.x        != sinistra || f.y != summa || f.latitudo != lat
        || f.altitudo != alt)
    {
        imprimere("  fines: %d %d %d %d\n", (integer)f.x, (integer)f.y,
            (integer)f.latitudo, (integer)f.altitudo);
        redde FALSUM;
    }
    redde VERUM;
}

/* littera in cellula prima paginae currentis committitur */
interior b32
litteram_committere (
     Mundus* x,
    character littera,
    Piscina* p)
{
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    d = x->charta.scriba.doc;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_initiare(&folium, p, d->latitudo, d->altitudo);
    memcpy(folium.cellulae, scriba_documentum_tabula(d)->cellulae,
        (memoriae_index)(d->latitudo * d->altitudo));
    tabula_cellula(&folium, ZEPHYRUM, ZEPHYRUM) = littera;
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

interior character
littera_prima (
    Mundus* x)
{
    redde x->charta.scriba.doc ? (character)tabula_cellula(
        scriba_documentum_tabula(x->charta.scriba.doc), ZEPHYRUM,
        ZEPHYRUM) : '\0';
}

/* area: pagina ex loco montationis ((CCCLX - XVIII) / VI = LVII
 * cellulae, minus II; (CCXL - XVIII) / VIII = XXVII, minus III);
 * locus maior: pagina terminat; minor: locus; vacuus: nihil */
interior vacuum
_probare_aream (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(montare(x, c->piscina));
    CREDO_VERUM(x->charta.montata);
    CREDO_NECESSE_NON_NIHIL(x->charta.scriba.doc);
    CREDO_AEQUALIS_I32(x->charta.scriba.doc->latitudo, LV);
    CREDO_AEQUALIS_I32(x->charta.scriba.doc->altitudo, XXIV);
    CREDO_VERUM(fines_sunt(charta_area(&x->charta, fines(X, XX, CCCLX,
        CCXL)), XVIII, XXVIII, LVII * VI, XXVII * VIII));
    CREDO_VERUM(fines_sunt(charta_area(&x->charta, fines(X, XX, DC,
        CD)), XVIII, XXVIII, LVII * VI, XXVII * VIII));
    CREDO_VERUM(fines_sunt(charta_area(&x->charta, fines(ZEPHYRUM,
        ZEPHYRUM, CXX, LXXX)), VIII, VIII, XVII * VI, VII * VIII));
    CREDO_VERUM(fines_sunt(charta_area(&x->charta, fines(ZEPHYRUM,
        ZEPHYRUM, XVI, XVI)), VIII, VIII, ZEPHYRUM, ZEPHYRUM));
}

/* clavis: liber alius (vacuus), liber prior servatus; clavis eadem
 * nihil agit; liber in volumine manet */
interior vacuum
_probare_clavem (
    CredoContextus* c)
{
         Mundus* x;
    ScribaLiber* l;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(montare(x, c->piscina));
    CREDO_VERUM(strcmp(x->charta.clavis, "persona/1") == ZEPHYRUM);
    CREDO_NECESSE(litteram_committere(x, 'k', c->piscina));
    CREDO_VERUM(charta_clavem_ponere(&x->charta, NIHIL, "persona/2",
        CCCLX, CCXL));
    CREDO_VERUM(strcmp(x->charta.clavis, "persona/2") == ZEPHYRUM);
    CREDO_VERUM(littera_prima(x) != 'k');
    CREDO_FALSUM(charta_clavem_ponere(&x->charta, NIHIL, "persona/2",
        CCCLX, CCXL));
    CREDO_VERUM(charta_clavem_ponere(&x->charta, NIHIL, "persona/1",
        CCCLX, CCXL));
    CREDO_VERUM(littera_prima(x) == 'k');
    l = scriba_liber_aperire_in_spatio(c->piscina, x->intern,
        x->volumen, "persona/1", LV, XXIV);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_VERUM(tabula_cellula(scriba_documentum_tabula(
        scriba_liber_pagina(l, chorda_ex_literis("1", c->piscina))),
        ZEPHYRUM, ZEPHYRUM) == 'k');
}

/* registra scribae in hospitis, spatio rami scribae (sub hospite) */
interior vacuum
_probare_registra (
    CredoContextus* c)
{
     Mundus* x;
    ActioFn  fn;
     vacuum* ctx;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(montare(x, c->piscina));
    CREDO_VERUM(chorda_continet(x->charta.scriba.ramus.via,
        chorda_ex_literis("latus:h1", c->piscina)));
    CREDO_VERUM(chorda_continet(x->charta.scriba.ramus.via,
        chorda_ex_literis("scriba:h1_notae", c->piscina)));
    CREDO_VERUM(actio_invenire_in_spatio(x->actiones,
        x->charta.scriba.ramus.via, chorda_ex_literis("pagina.clavis",
        c->piscina), &fn, &ctx));
}

/* focus: spatium intimum sub ramo scribae, non hospitis */
interior vacuum
_probare_focum (
    CredoContextus* c)
{
    Mundus* x;
     Motus  motus;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(montare(x, c->piscina));
    motus_initiare(&motus, c->piscina);
    motus.spatium_intimum = x->charta.scriba.ramus.via;
    CREDO_VERUM(charta_focata(&x->charta, &motus));
    motus.spatium_intimum = x->hospes.via;
    CREDO_FALSUM(charta_focata(&x->charta, &motus));
    CREDO_FALSUM(charta_focata(&x->charta, NIHIL));
}

/* compositio: scriba filius radicis in area, spatio suo; locus vacuus
 * nihil addit; umbra pingitur */
interior vacuum
_probare_compositionem (
    CredoContextus* c)
{
       Mundus* x;
        Motus  motus;
    Componens* radix;
    Componens* l;
      Mandata* md;
        Fines  locus;

    x = (Mundus*)c->datum;
    thema_initiare();
    CREDO_NECESSE(montare(x, c->piscina));
    motus_initiare(&motus, c->piscina);
    radix = componens_creare(c->piscina, x->intern, "radix",
        PARTES_NULLUM);
    CREDO_NECESSE_NON_NIHIL(radix);
    locus = fines(X, XX, CCCLX, CCXL);
    charta_componere(&x->charta, x->repo, &motus, c->piscina,
        x->intern, radix, fines(ZEPHYRUM, ZEPHYRUM, CCCLX, ZEPHYRUM));
    CREDO_AEQUALIS_I32(componens_numerus_liberorum(radix), ZEPHYRUM);
    charta_componere(&x->charta, x->repo, &motus, c->piscina,
        x->intern, radix, locus);
    CREDO_NECESSE(componens_numerus_liberorum(radix) == I);
    l = componens_liberum(radix, ZEPHYRUM);
    CREDO_VERUM(fines_sunt(l->fines, XVIII, XXVIII, LVII * VI,
        XXVII * VIII));
    CREDO_VERUM(chorda_aequalis(l->spatium,
        x->charta.scriba.ramus.via));
    md = mandata_creare(c->piscina, x->intern);
    charta_umbram_pingere(&x->charta, md, locus);
    CREDO_VERUM(xar_numerus(md->elementa) > ZEPHYRUM);
}

/* volumen NIHIL: montata non; cetera nihil agunt */
interior vacuum
_probare_sine_volumine (
    CredoContextus* c)
{
       Mundus* x;
        Motus  motus;
    Componens* radix;
      Mandata* md;
        Fines  locus;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->repo != NIHIL);
    CREDO_VERUM(charta_montare(&x->charta, c->piscina, x->intern, NIHIL,
        &x->hospes, x->actiones, x->figurae, "h1_notae", "persona/1",
        CCCLX, CCXL, NIHIL));
    CREDO_FALSUM(x->charta.montata);
    CREDO_FALSUM(charta_clavem_ponere(&x->charta, NIHIL, "persona/2",
        CCCLX, CCXL));
    locus = fines(X, XX, CCCLX, CCXL);
    CREDO_VERUM(fines_sunt(charta_area(&x->charta, locus), X, XX,
        ZEPHYRUM, ZEPHYRUM));
    motus_initiare(&motus, c->piscina);
    motus.spatium_intimum = x->hospes.via;
    CREDO_FALSUM(charta_focata(&x->charta, &motus));
    CREDO_FALSUM(charta_reficere(&x->charta));
    charta_gestum_ponere(&x->charta, &motus);
    radix = componens_creare(c->piscina, x->intern, "radix",
        PARTES_NULLUM);
    CREDO_NECESSE_NON_NIHIL(radix);
    charta_componere(&x->charta, x->repo, &motus, c->piscina,
        x->intern, radix, locus);
    CREDO_AEQUALIS_I32(componens_numerus_liberorum(radix), ZEPHYRUM);
    md = mandata_creare(c->piscina, x->intern);
    charta_umbram_pingere(&x->charta, md, locus);
    CREDO_AEQUALIS_I32(xar_numerus(md->elementa), ZEPHYRUM);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "area", _probare_aream, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "clavis", _probare_clavem, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "registra", _probare_registra, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "focus", _probare_focum, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "compositio", _probare_compositionem, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "sine_volumine", _probare_sine_volumine, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("membrum__charta", SECTIONES);
}
