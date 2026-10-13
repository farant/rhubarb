/* kalendarium.c - calendarium, pagina mensis (include/kalendarium.h;
 * agenda-plan A2a). Scriptura status per portam rami, scriptore
 * calendarium.agere (vocantibus extra dispensatorem scriptor ponitur et
 * restituitur - aliter domini scripturam tacite recusant). */

#include "kalendarium.h"

#include <string.h>

hic_manens constans character actio_calendarii[] = "calendarium.agere";

#define KALENDARIUM_TITULI_ALTITUDO  XVI
#define KALENDARIUM_LATITUDO         CCLXXX   /* superficies absens */
#define KALENDARIUM_ALTITUDO         CCLVI


/* ==================================================
 * Auxilia
 * ================================================== */

interior Dies
hodie (
    constans KalendariumMontatio* m)
{
    Dies d;

    si (m->hodie && m->hodie(m->hodie_ctx, &d) && fasti_dies_valida(d))
    {
        redde d;
    }
    redde fasti_dies(MMXXVI, I, I);
}

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_ramus_attributum(ramus, titulus);
    si (a && chorda_ut_s32(*a, &v) && v > ZEPHYRUM)
    {
        redde v;
    }
    redde praestitutum;
}

/* "YYYY-MM" -> annus, mensis; FALSUM si malum */
interior b32
mensem_legere (
    constans character* argumentum,
                   s32* annus,
                   s32* mensis)
{
    i32 i;

    si (   !argumentum || strlen(argumentum) != VII
        || argumentum[IV] != '-')
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < VII; i++)
    {
        si (i != IV && (argumentum[i] < '0' || argumentum[i] > '9'))
        {
            redde FALSUM;
        }
    }
    *annus   = (s32)((argumentum[ZEPHYRUM] - '0') * M
             + (argumentum[I] - '0') * C + (argumentum[II] - '0') * X
             + (argumentum[III] - '0'));
    *mensis  = (s32)((argumentum[V] - '0') * X + (argumentum[VI]
        - '0'));
    redde *mensis >= I && *mensis <= XII;
}

/* dies argumenti: hodie si in eo mense, aliter dies I */
interior Dies
dies_argumenti (
    constans KalendariumMontatio* m,
                             s32  annus,
                             s32  mensis)
{
    Dies h;

    h = hodie(m);
    si (h.annus == annus && h.mensis == mensis)
    {
        redde h;
    }
    redde fasti_dies(annus, mensis, I);
}

interior vacuum
diem_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    insula_attributum_ponere(radix, p, in, "dies_electus",
        (constans character*)ctx);
}

/* dies_electus scribere, scriptore calendarii (prior restituitur) */
interior b32
diem_scribere (
    KalendariumMontatio* m,
                   Dies  dies)
{
    character iso[XVI];
       chorda prior;
       chorda textus;
          b32 ok;

    textus = fasti_formare_diem(dies, FASTI_FORMA_ISO, m->piscina);
    si (textus.mensura <= ZEPHYRUM || textus.mensura >= XVI)
    {
        redde FALSUM;
    }
    memcpy(iso, textus.datum, (size_t)textus.mensura);
    iso[textus.mensura]  = '\0';
    prior                = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_calendarii, m->piscina));
    ok = mutare_ramum(&m->ramus, diem_ponere, iso);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}


/* ==================================================
 * Actio
 * ================================================== */

/* <actio/> calendarium.agere: claves (h/l/j/k/H/L/t) et ictus in
 * cellula (id = dies ISO) */
interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    KalendariumMontatio* m;
                   Dies  d;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m = (KalendariumMontatio*)ctx;
    si (ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        si (   nodus && fasti_parsare_iso(nodus->id, &d)
            && fasti_dies_valida(d))
        {
            redde diem_scribere(m, d);
        }
        redde FALSUM;
    }
    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    d = kalendarium_dies_electus(m);
    commutatio (ev->datum.clavis.producta)
    {
        casus 'h': d = fasti_addere_dies(d, -I);              frange;
        casus 'l': d = fasti_addere_dies(d, I);               frange;
        casus 'j': d = fasti_addere_dies(d, VII);             frange;
        casus 'k': d = fasti_addere_dies(d, -VII);            frange;
        casus 'H': d = fasti_addere_menses(d, -I);            frange;
        casus 'L': d = fasti_addere_menses(d, I);             frange;
        casus 't': d = hodie(m);                              frange;
        ordinarius: redde FALSUM;
    }
    redde diem_scribere(m, d);
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
kalendarium_montare (
     KalendariumMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
      constans character* argumentum,
        AgendaDocumentum* documentum,
        KalendariumHodie  hodie_porta,
                  vacuum* hodie_ctx,
         StatusSystemata* systemata)
{
    Organum* organum;
        s32  annus;
        s32  mensis;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    si (argumentum && !mensem_legere(argumentum, &annus, &mensis))
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(KalendariumMontatio));
    m->piscina     = piscina;
    m->intern      = intern;
    m->documentum  = documentum;
    m->hodie       = hodie_porta;
    m->hodie_ctx   = hodie_ctx;
    m->ramus       = id ? insula_ramus(repo, "calendarium", id)
                        : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "calendarium",
        "kalendarium");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"kalendarium\"", id ? id : "calendarium",
               piscina, intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae
        || !actio_registrare(m->actiones, actio_calendarii, agere, m))
    {
        redde FALSUM;
    }
    si (argumentum)
    {
        redde diem_scribere(m, dies_argumenti(m, annus, mensis));
    }
    redde VERUM;
}

b32
kalendarium_argumentum_ponere (
    KalendariumMontatio* m,
     constans character* argumentum)
{
    s32 annus;
    s32 mensis;

    si (!m || !mensem_legere(argumentum, &annus, &mensis))
    {
        redde FALSUM;
    }
    redde diem_scribere(m, dies_argumenti(m, annus, mensis));
}

Dies
kalendarium_dies_electus (
    constans KalendariumMontatio* m)
{
     chorda* a;
       Dies  d;

    a = insula_ramus_attributum(&m->ramus, "dies_electus");
    si (a && fasti_parsare_iso(*a, &d) && fasti_dies_valida(d))
    {
        redde d;
    }
    redde hodie(m);
}


/* ==================================================
 * Componere
 * ================================================== */

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                    s32  x,
                    s32  y,
                    s32  latitudo,
                    s32  altitudo)
{
    Componens* c;
        Fines  f;

    c = componens_creare(p, in, id, PARTES_NULLUM);
    si (!c)
    {
        redde NIHIL;
    }
    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    componens_ponere_fines(c, f);
    redde c;
}

/* <componens/> <purus/> */
Componens*
kalendarium_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    KalendariumMontatio* m;
              Componens* radix;
              Componens* cellula;
                   Dies  electus;
                   Dies  initium;
                   Dies  dies;
                    s32  latitudo;
                    s32  altitudo;
                    s32  lat_cellulae;
                    s32  alt_cellulae;
                    i32  k;

    (vacuum)repo;
    (vacuum)motus;
    m = (KalendariumMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo  = attributum_s32(&m->ramus, "superficies_latitudo",
        KALENDARIUM_LATITUDO);
    altitudo  = attributum_s32(&m->ramus, "superficies_altitudo",
        KALENDARIUM_ALTITUDO);
    radix = nodus(piscina, intern, "kalendarium", ZEPHYRUM, ZEPHYRUM,
        latitudo, altitudo);
    si (!radix)
    {
        redde NIHIL;
    }
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_calendarii);
    componens_addere_liberum(radix, nodus(piscina, intern, "titulus",
        ZEPHYRUM, ZEPHYRUM, latitudo, KALENDARIUM_TITULI_ALTITUDO));
    /* VI hebdomades x VII, dominica prima ante aut in die I */
    electus = kalendarium_dies_electus(m);
    initium = fasti_dies(electus.annus, electus.mensis, I);
    initium       = fasti_addere_dies(initium,
        -fasti_dies_hebdomadis(initium));
    lat_cellulae = latitudo / VII;
    alt_cellulae = (altitudo - KALENDARIUM_TITULI_ALTITUDO) / VI;
    per (k = ZEPHYRUM; k < XLII; k++)
    {
        dies     = fasti_addere_dies(initium, (s32)k);
        cellula  = nodus(piscina, intern, chorda_ut_cstr(
            fasti_formare_diem(dies, FASTI_FORMA_ISO, piscina),
            piscina),
            (s32)(k % VII) * lat_cellulae, KALENDARIUM_TITULI_ALTITUDO
            + (s32)(k / VII) * alt_cellulae, lat_cellulae,
            alt_cellulae);
        si (cellula)
        {
            componens_ponere_actio(cellula, actio_calendarii);
            componens_addere_liberum(radix, cellula);
        }
    }
    redde radix;
}
