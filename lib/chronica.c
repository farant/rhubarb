/* chronica.c - chronica in latere vici (vide include/chronica.h)
 *
 * Arbor: radix "chronica" (PARTES_TABULA, figura tota: annus, lineae
 * paginae, pes) et liberi sine figura qui ictus accipiunt: "supra" et
 * "infra" (dimidia areae eventuum), "sagitta_prior" et
 * "sagitta_proxima" (in pede). Annus visus semper RESOLUTUS: campus,
 * aliter praesentia, aliter annus hodie proximus - deinde ad annum
 * cum eventis proximum (aequalitas: prior). Paginae in cache per
 * (tabula, annus, area): tabula reaedificata sedem novam habet
 * (chronica_fontes), ergo sedes eadem = paginae eaedem. */

#include "chronica.h"
#include "membrum__praecepta.h"
#include "chorda_aedificator.h"
#include "codex.h"
#include "lector_paginae.h"
#include "semita.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"

#include <stdio.h>
#include <string.h>

#define CHRONICA_LATITUDO   CCCLX    /* superficies ordinaria */
#define CHRONICA_ALTITUDO   CCXL
#define CHRONICA_LIMBUS_X   VI       /* margo texturae (ut navigator) */
#define CHRONICA_LIMBUS_Y   VIII
#define CHRONICA_MARGO      IV       /* intra fundum, px */
#define CHRONICA_SINISTRUM  (CHRONICA_LIMBUS_X + CHRONICA_MARGO)
#define CHRONICA_CAPUT      XVI      /* altitudo anni (FONS_TITULI) */
#define CHRONICA_INTERVALLUM VIII    /* inter annum et eventa */
#define CHRONICA_PES        XIV      /* linea ima cum spatio */
#define VISUS_ANNI          I
#define VISUS_FLUMINIS      II
/* C5b: indices et paginae nominum */
#define VISUS_PERSONAE      IV
#define VISUS_LOCA          VIII
#define VISUS_PERSONA       XVI
#define VISUS_LOCUS         XXXII
#define VISUS_NOMINUM \
    (VISUS_PERSONAE | VISUS_LOCA | VISUS_PERSONA | VISUS_LOCUS)
#define VISUS_TEMPORIS      (VISUS_ANNI | VISUS_FLUMINIS)
#define VISUS_OMNES         (VISUS_TEMPORIS | VISUS_NOMINUM)

hic_manens constans character actio_chronicae[] = "chronica.agere";
hic_manens constans character clavis_positionis[] = "chronica/annus";
hic_manens constans character clavis_paginae[]    = "chronica/pagina";
hic_manens constans character textus_vacuus[] =
    "Write <event year=\"1274\">...</event> in a note";

hic_manens constans character* menses[XII] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

/* cache privatum (figurae_ctx) */
nomen structura {
                    Organum* organum;
                    Piscina* scheda;        /* paginae currentes */
                        i32  latitudo_montationis;
                        i32  altitudo_montationis;
    constans ChronicaTabula* tabula;     /* clavis cache */
                        s32  annus;
                        s32  latitudo;
                        s32  altitudo;
                        b32  flumen;
                        i32  visus_arcae;   /* VISUS_* paginarum */
                     chorda  nomen_arcae;   /* pagina nominis */
                 CodexCaput  caput;
                        Xar* notae;         /* NotaClausulae */
                        Xar* paginae;       /* LectorPagina */
                        Xar* unitates;      /* UnitasChronicae */
} ChronicaFigurae;

/* res eligibilis (C4a): eventus (descriptio), persona k, locus -
 * ambitus in textu capitis, pagina ubi incipit */
#define UNITAS_EVENTUS  ZEPHYRUM
#define UNITAS_PERSONA  I
#define UNITAS_LOCUS    II
/* C5b: linea indicis (res = entitas) */
#define UNITAS_NOMEN    III
#define UNITAS_DECLARATIO IV     /* res = index declarationis */

nomen structura {
    i32 res;       /* index in tabula->res */
    i32 genus;
    i32 k;         /* persona k */
    i32 initium;
    i32 finis;
    s32 pagina;
} UnitasChronicae;

/* pars lineae parvae (persona, locus) - octeti in linea */
nomen structura {
    i32 genus;
    i32 k;
    i32 initium;
    i32 finis;
} ParsParva;

/* per clausulam capitis: annus eventus, linea parva?, prima anni? */
nomen structura {
    s32 annus;
    b32 obscura;
    b32 prima;
} NotaClausulae;

nomen structura {
    constans character* constans* tituli;
    constans character* constans* valores;
                             i32 numerus;
} GrexCamporum;

interior b32
visum_currens (
    ChronicaMontatio*  m,
     ChronicaFigurae** f,
                 s32*  pagina);


/* ==================================================
 * Status (ramus)
 * ================================================== */

interior chorda
campus (
    constans ChronicaMontatio* m,
           constans character* titulus)
{
    chorda* a = insula_ramus_attributum(&m->ramus, titulus);
    chorda  v;

    si (a)
    {
        redde *a;
    }
    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

/* numerus campi; FALSUM si absens aut malus */
interior b32
campus_numerus (
    constans ChronicaMontatio* m,
           constans character* titulus,
                          s32* valor)
{
    chorda a = campus(m, titulus);

    redde a.mensura > ZEPHYRUM && chorda_ut_s32(a, valor);
}

/* visus currens (VISUS_*); absens aut ignotus: flumen */
interior i32
visus_genus (
    constans ChronicaMontatio* m)
{
    chorda v = campus(m, "visus");

    si (chorda_aequalis_literis(v, "annus"))
    {
        redde VISUS_ANNI;
    }
    si (chorda_aequalis_literis(v, "personae"))
    {
        redde VISUS_PERSONAE;
    }
    si (chorda_aequalis_literis(v, "loca"))
    {
        redde VISUS_LOCA;
    }
    si (chorda_aequalis_literis(v, "persona"))
    {
        redde VISUS_PERSONA;
    }
    si (chorda_aequalis_literis(v, "locus"))
    {
        redde VISUS_LOCUS;
    }
    redde VISUS_FLUMINIS;
}

interior b32
visus_fluminis (
    constans ChronicaMontatio* m)
{
    redde visus_genus(m) == VISUS_FLUMINIS;
}

interior b32
visus_nominum (
    constans ChronicaMontatio* m)
{
    redde (visus_genus(m) & VISUS_NOMINUM) != ZEPHYRUM;
}

interior vacuum
campos_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    GrexCamporum* grex = (GrexCamporum*)ctx;
             i32  i;

    per (i = ZEPHYRUM; i < grex->numerus; i++)
    {
        si (!grex->valores[i] || !grex->valores[i][ZEPHYRUM])
        {
            (vacuum)insula_attributum_tollere(radix, grex->tituli[i]);
        }
        alioquin
        {
            insula_attributum_ponere(radix, p, in, grex->tituli[i],
                grex->valores[i]);
        }
    }
}

/* campi actu uno, scriptore chronicae ("" tollit) */
interior b32
campos_scribere (
             ChronicaMontatio* m,
    constans character* constans* tituli,
    constans character* constans* valores,
                             i32  numerus)
{
    GrexCamporum grex;
          chorda prior;
             b32 ok;

    grex.tituli   = tituli;
    grex.valores  = valores;
    grex.numerus  = numerus;
    prior         = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_chronicae, m->piscina));
    ok = mutare_ramum(&m->ramus, campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior vacuum
positionem_servare (
    ChronicaMontatio* m,
                 s32  annus,
                 s32  pagina)
{
    character numerus[XVI];

    si (m->volumen)
    {
        sprintf(numerus, "%d", (integer)annus);
        (vacuum)volumen_praesens_ponere(m->volumen,
            chorda_ex_literis(clavis_positionis, m->piscina),
            chorda_ex_literis(numerus, m->piscina));
        /* flumen: pagina post eam ubi annus incipit (annus solus: 0) */
        sprintf(numerus, "%d", (integer)pagina);
        (vacuum)volumen_praesens_ponere(m->volumen,
            chorda_ex_literis(clavis_paginae, m->piscina),
            chorda_ex_literis(numerus, m->piscina));
    }
}

/* pagina fluminis servata (campo annus absente); FALSUM si nulla */
interior b32
paginam_servatam_legere (
    ChronicaMontatio* m,
                 s32* pagina)
{
    chorda valor;
       b32 inventum;

    si (!m->volumen)
    {
        redde FALSUM;
    }
    valor = volumen_praesens_legere(m->volumen,
        chorda_ex_literis(clavis_paginae, m->piscina), m->piscina,
        &inventum);
    redde inventum && chorda_ut_s32(valor, pagina);
}

/* annus novus: campus et praesentia; pagina ad primam */
interior b32
annum_scribere (
    ChronicaMontatio* m,
                 s32  annus)
{
    constans character* tituli[III];
    constans character* valores[III];
             character  numerus[XVI];

    sprintf(numerus, "%d", (integer)annus);
    tituli[ZEPHYRUM]   = "annus";
    valores[ZEPHYRUM]  = numerus;
    tituli[I]          = "pagina";
    valores[I]         = "";
    tituli[II]         = "electus";
    valores[II]        = "";
    positionem_servare(m, annus, ZEPHYRUM);
    redde campos_scribere(m, tituli, valores, III);
}

interior b32
paginam_scribere (
    ChronicaMontatio* m,
                 s32  pagina)
{
    constans character* tituli[II];
    constans character* valores[II];
             character  numerus[XVI];

    sprintf(numerus, "%d", (integer)pagina);
    tituli[ZEPHYRUM]   = "pagina";
    valores[ZEPHYRUM]  = numerus;
    tituli[I]          = "electus";
    valores[I]         = "";
    redde campos_scribere(m, tituli, valores, II);
}


/* ==================================================
 * Annus resolutus
 * ================================================== */

interior constans ChronicaTabula*
tabula (
    ChronicaMontatio* m)
{
    redde m->fontes ? chronica_tabula(m->fontes) : NIHIL;
}

/* annus quaesitus: campus, praesentia, hodie; FALSUM si nullus */
interior b32
annus_quaesitus (
    ChronicaMontatio* m,
                 s32* annus)
{
    chorda valor;
       b32 inventum;
      Dies hodie;

    si (campus_numerus(m, "annus", annus) && *annus != ZEPHYRUM)
    {
        redde VERUM;
    }
    si (m->volumen)
    {
        valor = volumen_praesens_legere(m->volumen,
            chorda_ex_literis(clavis_positionis, m->piscina),
            m->piscina, &inventum);
        si (   inventum && chorda_ut_s32(valor, annus)
            && *annus != ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    si (m->hodie && m->hodie(m->hodie_ctx, &hodie))
    {
        *annus = hodie.annus;
        redde VERUM;
    }
    redde FALSUM;
}

/* index anni cum eventis proximi (aequalitas: prior); -1 si nulli */
interior s32
index_anni (
    constans ChronicaTabula* t,
                        s32  quaesitus)
{
    s32 optimus;
    s32 distantia;
    i32 i;

    optimus    = -I;
    distantia  = ZEPHYRUM;
    per (i = ZEPHYRUM; t && i < xar_numerus(t->anni); i++)
    {
        s32 a = *(s32*)xar_obtinere(t->anni, i);
        s32 d = a > quaesitus ? a - quaesitus : quaesitus - a;

        si (optimus < ZEPHYRUM || d < distantia)
        {
            optimus    = (s32)i;
            distantia  = d;
        }
    }
    redde optimus;
}

/* index anni visi in t->anni; -1 si nulla eventa */
interior s32
index_currens (
                  ChronicaMontatio* m,
           constans ChronicaTabula* t)
{
    s32 annus;

    si (!t || xar_numerus(t->anni) == ZEPHYRUM)
    {
        redde -I;
    }
    si (!annus_quaesitus(m, &annus))
    {
        redde ZEPHYRUM;
    }
    redde index_anni(t, annus);
}


/* ==================================================
 * Paginae anni
 * ================================================== */

interior s32
superficies (
    constans ChronicaMontatio* m,
                          b32  latitudo)
{
     constans ChronicaFigurae* f;
                          s32  v;
                          s32  n;

    f = (constans ChronicaFigurae*)m->figurae_ctx;
    v = !f ? ZEPHYRUM : latitudo ? (s32)f->latitudo_montationis
                                 : (s32)f->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? CHRONICA_LATITUDO : CHRONICA_ALTITUDO;
    }
    si (   campus_numerus(m, latitudo ? "superficies_latitudo"
            : "superficies_altitudo", &n) && n > ZEPHYRUM)
    {
        v = n;
    }
    redde v;
}

/* area eventuum in spatio radicis */
interior Fines
area_eventuum (
    constans ChronicaMontatio* m)
{
    Fines f;
      s32 summum;

    summum      = CHRONICA_LIMBUS_Y + CHRONICA_MARGO + CHRONICA_CAPUT
                + CHRONICA_INTERVALLUM;
    f.x         = CHRONICA_SINISTRUM;
    f.y         = summum;
    f.latitudo  = superficies(m, VERUM) - II * CHRONICA_SINISTRUM;
    f.altitudo  = superficies(m, FALSUM) - summum - CHRONICA_LIMBUS_Y
                - CHRONICA_PES;
    si (f.latitudo < I)
    {
        f.latitudo = I;
    }
    si (f.altitudo < I)
    {
        f.altitudo = I;
    }
    redde f;
}

/* area fluminis tota (columna anni + columna eventuum) */
interior Fines
area_fluminis (
    constans ChronicaMontatio* m)
{
    Fines f;
      s32 summum;

    summum      = CHRONICA_LIMBUS_Y + CHRONICA_MARGO;
    f.x         = CHRONICA_SINISTRUM;
    f.y         = summum;
    f.latitudo  = superficies(m, VERUM) - II * CHRONICA_SINISTRUM;
    f.altitudo  = superficies(m, FALSUM) - summum - CHRONICA_LIMBUS_Y
                - CHRONICA_PES;
    si (f.latitudo < V)
    {
        f.latitudo = V;
    }
    si (f.altitudo < I)
    {
        f.altitudo = I;
    }
    redde f;
}

/* columna eventuum fluminis: quattuor quintae dextrae */
interior Fines
columna_fluminis (
    constans ChronicaMontatio* m)
{
    Fines f     = area_fluminis(m);
      s32 anni  = f.latitudo / V;

    f.x         += anni;
    f.latitudo  -= anni;
    redde f;
}

/* area visus currentis (dimidia ictuum) */
interior Fines
area_visus (
    constans ChronicaMontatio* m)
{
    redde visus_fluminis(m) ? area_fluminis(m) : area_eventuum(m);
    /* visus anni et nominum: area sub titulo magno */
}

interior vacuum
annum_formare (
          s32  annus,
    character* alveus)
{
    si (annus < ZEPHYRUM)
    {
        sprintf(alveus, "%d BC", (integer)-annus);
    }
    alioquin
    {
        sprintf(alveus, "AD %d", (integer)annus);
    }
}

interior vacuum
partem_addere (
    ChordaAedificator* a,
                  b32* prima,
               chorda  pars)
{
    si (pars.mensura == ZEPHYRUM)
    {
        redde;
    }
    si (!*prima)
    {
        chorda_aedificator_appendere_literis(a, " \302\267 ");
    }
    chorda_aedificator_appendere_chorda(a, pars);
    *prima = FALSUM;
}

/* linea parva: dies, genus, personae, locus (vacua omissa); partes
 * (NIHIL licet) ambitus personarum et loci accipit */
interior chorda
lineam_parvam (
    constans ChronicaEventus* e,
                         Xar* partes,
                     Piscina* piscina)
{
            ParsParva  pars;
                  i32  basis;
    ChordaAedificator* a;
    ChordaAedificator* personae;
                  b32  prima;
            character  dies[XVI];
                  i32  i;

    a = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    prima = VERUM;
    dies[ZEPHYRUM] = '\0';
    si (e->mensis >= I && e->mensis <= XII && e->dies > ZEPHYRUM)
    {
        sprintf(dies, "%d %s", (integer)e->dies, menses[e->mensis - I]);
    }
    alioquin si (e->mensis >= I && e->mensis <= XII)
    {
        sprintf(dies, "%s", menses[e->mensis - I]);
    }
    partem_addere(a, &prima, chorda_ex_literis(dies, piscina));
    partem_addere(a, &prima, e->genus);
    personae = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    /* " · " = IV octeti ante partem non primam */
    basis = (i32)chorda_aedificator_longitudo(a) + (prima ? ZEPHYRUM
                                                          : IV);
    per (i = ZEPHYRUM; e->personae && i < xar_numerus(e->personae); i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(personae, ", ");
        }
        pars.genus  = UNITAS_PERSONA;
        pars.k      = i;
        pars.initium  = basis + (i32)chorda_aedificator_longitudo(
            personae);
        chorda_aedificator_appendere_chorda(personae,
            *(chorda*)xar_obtinere(e->personae, i));
        pars.finis = basis
            + (i32)chorda_aedificator_longitudo(personae);
        si (partes)
        {
            *(ParsParva*)xar_addere(partes) = pars;
        }
    }
    partem_addere(a, &prima, chorda_aedificator_finire(personae));
    si (partes && e->locus.mensura > ZEPHYRUM)
    {
        pars.genus  = UNITAS_LOCUS;
        pars.k      = ZEPHYRUM;
        pars.initium  = (i32)chorda_aedificator_longitudo(a)
                      + (prima ? ZEPHYRUM : IV);
        pars.finis = pars.initium + e->locus.mensura;
        *(ParsParva*)xar_addere(partes) = pars;
    }
    partem_addere(a, &prima, e->locus);
    redde chorda_aedificator_finire(a);
}

interior vacuum
clausulam_addere (
      ChronicaFigurae* f,
    ChordaAedificator* a,
               chorda  textus,
                  s32  annus,
                  b32  obscura,
                  b32  prima)
{
    CodexClausula c;
    NotaClausulae n;

    si (chorda_aedificator_longitudo(a) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "\n\n");
    }
    c.genus    = CODEX_PARAGRAPHUS;
    c.initium  = (i32)chorda_aedificator_longitudo(a);
    chorda_aedificator_appendere_chorda(a, textus);
    c.finis = (i32)chorda_aedificator_longitudo(a);
    *(CodexClausula*)xar_addere(f->caput.clausulae) = c;
    n.annus = annus;
    n.obscura = obscura;
    n.prima = prima;
    *(NotaClausulae*)xar_addere(f->notae) = n;
}

/* res eligibilis: ambitus relativus ad clausulam ULTIMAM (eventus:
 * clausula tota) */
interior vacuum
unitatem_addere (
    ChronicaFigurae* f,
                i32  res,
                i32  genus,
                i32  k,
                i32  initium,
                i32  finis)
{
    constans CodexClausula* c = (constans CodexClausula*)xar_obtinere(
        f->caput.clausulae, xar_numerus(f->caput.clausulae) - I);
          UnitasChronicae u;

    u.res    = res;
    u.genus  = genus;
    u.k      = k;
    u.initium  = genus == UNITAS_EVENTUS ? c->initium
                                         : c->initium + initium;
    u.finis = genus == UNITAS_EVENTUS ? c->finis : c->initium
        + finis;
    u.pagina                                    = ZEPHYRUM;
    *(UnitasChronicae*)xar_addere(f->unitates)  = u;
}

/* paginae in area (cache): anni dati, aut (flumen) omnium annorum in
 * columna eventuum; NIHIL si deficit */
interior ChronicaFigurae*
paginas_parare (
           ChronicaMontatio* m,
    constans ChronicaTabula* t,
                        s32  annus,
                        b32  flumen)
{
      ChronicaFigurae* f;
    ChordaAedificator* a;
                Fines  area;
                  s32  ultimus;
                  i32  i;

    f     = (ChronicaFigurae*)m->figurae_ctx;
    area  = flumen ? columna_fluminis(m) : area_eventuum(m);
    si (!f || !t)
    {
        redde NIHIL;
    }
    si (   f->paginae && f->tabula == t
        && f->visus_arcae == (flumen ? VISUS_FLUMINIS : VISUS_ANNI)
        && (flumen || f->annus == annus)
        && f->latitudo == area.latitudo && f->altitudo == area.altitudo)
    {
        redde f;
    }
    piscina_vacare(f->scheda);
    f->paginae                = NIHIL;
    f->caput.titulus.datum    = NIHIL;
    f->caput.titulus.mensura  = ZEPHYRUM;
    f->caput.clausulae    = xar_creare(f->scheda,
        (i32)magnitudo(CodexClausula));
    f->notae = xar_creare(f->scheda, (i32)magnitudo(NotaClausulae));
    f->unitates = xar_creare(f->scheda,
        (i32)magnitudo(UnitasChronicae));
    a = chorda_aedificator_creare(f->scheda, (memoriae_index)MXXIV);
    ultimus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(t->res); i++)
    {
        constans ChronicaRes* r = (constans ChronicaRes*)xar_obtinere(
            t->res, i);
        chorda  parva;
           s32  a_r = r->eventus.annus;
           Xar* partes;
           i32  q;

        si (!flumen && a_r != annus)
        {
            perge;
        }
        clausulam_addere(f, a, r->eventus.descriptio, a_r, FALSUM,
            i == ZEPHYRUM || a_r != ultimus);
        unitatem_addere(f, i, UNITAS_EVENTUS, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM);
        ultimus  = a_r;
        partes   = xar_creare(f->scheda, (i32)magnitudo(ParsParva));
        parva    = lineam_parvam(&r->eventus, partes, f->scheda);
        si (parva.mensura > ZEPHYRUM)
        {
            clausulam_addere(f, a, parva, a_r, VERUM, FALSUM);
            per (q = ZEPHYRUM; partes && q < xar_numerus(partes); q++)
            {
                constans ParsParva* pp = (constans ParsParva*)
                    xar_obtinere(partes, q);

                unitatem_addere(f, i, pp->genus, pp->k, pp->initium,
                    pp->finis);
            }
        }
    }
    f->caput.textus  = chorda_aedificator_finire(a);
    f->paginae       = lector_paginare(&f->caput, (i32)area.latitudo,
        (i32)area.altitudo, f->scheda);
    per (i = ZEPHYRUM; f->paginae && i < xar_numerus(f->unitates); i++)
    {
        UnitasChronicae* u = (UnitasChronicae*)xar_obtinere(f->unitates,
            i);

        u->pagina = (s32)lector_pagina_positionis(f->paginae,
            u->initium);
    }
    f->tabula       = t;
    f->flumen       = flumen;
    f->visus_arcae  = flumen ? VISUS_FLUMINIS : VISUS_ANNI;
    f->annus        = annus;
    f->latitudo     = area.latitudo;
    f->altitudo     = area.altitudo;
    redde f->paginae ? f : NIHIL;
}

/* pagina visa (coacta in [0, n)) */
interior s32
pagina_currens (
    constans ChronicaMontatio* m,
     constans ChronicaFigurae* f)
{
    s32 k;
    s32 n;

    n = f && f->paginae ? (s32)xar_numerus(f->paginae) : I;
    si (!campus_numerus(m, "pagina", &k) || k < ZEPHYRUM)
    {
        k = ZEPHYRUM;
    }
    redde k >= n ? n - I : k;
}

/* status visus: annus resolutus et paginae; FALSUM si nulla eventa */
interior b32
visum_parare (
        ChronicaMontatio*  m,
                     s32*  annus,
         ChronicaFigurae** f)
{
    constans ChronicaTabula* t;
                        s32  k;

    t = tabula(m);
    k = index_currens(m, t);
    si (k < ZEPHYRUM)
    {
        redde FALSUM;
    }
    *annus  = *(s32*)xar_obtinere(t->anni, (i32)k);
    *f      = paginas_parare(m, t, *annus, FALSUM);
    redde *f != NIHIL;
}


/* ==================================================
 * Nomina (C5b): indices et paginae nominum
 * ================================================== */

/* "Moses · 3 events · declared 2" (partes nullae omissae) */
interior chorda
lineam_entitatis (
    constans ChronicaEntitas* e,
                     Piscina* piscina)
{
    ChordaAedificator* a;

    a = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    chorda_aedificator_appendere_chorda(a, e->titulus);
    si (e->eventa > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " \302\267 ");
        chorda_aedificator_appendere_s32(a, (s32)e->eventa);
        chorda_aedificator_appendere_literis(a, e->eventa == I
            ? " event" : " events");
    }
    si (e->declarationes > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " \302\267 declared ");
        chorda_aedificator_appendere_s32(a, (s32)e->declarationes);
    }
    redde chorda_aedificator_finire(a);
}

/* eventus nomen continet? (persona: people=, locus: place=) */
interior b32
eventus_nominis (
    constans ChronicaEventus* e,
                         b32  persona,
                      chorda  appellatio)
{
    i32 k;

    si (!persona)
    {
        redde chorda_aequalis(e->locus, appellatio);
    }
    per (k = ZEPHYRUM; e->personae && k < xar_numerus(e->personae); k++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(e->personae, k),
                appellatio))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* paginae indicis aut paginae nominis in area sub titulo (cache) */
interior ChronicaFigurae*
paginas_nominum_parare (
           ChronicaMontatio* m,
    constans ChronicaTabula* t,
                        i32  visus,
                     chorda  appellatio)
{
      ChronicaFigurae* f;
    ChordaAedificator* a;
                Fines  area;
                  i32  i;

    f     = (ChronicaFigurae*)m->figurae_ctx;
    area  = area_eventuum(m);
    si (!f || !t)
    {
        redde NIHIL;
    }
    si (   f->paginae && f->tabula == t && f->visus_arcae == visus
        && chorda_aequalis(f->nomen_arcae, appellatio)
        && f->latitudo == area.latitudo && f->altitudo == area.altitudo)
    {
        redde f;
    }
    piscina_vacare(f->scheda);
    f->paginae                = NIHIL;
    f->caput.titulus.datum    = NIHIL;
    f->caput.titulus.mensura  = ZEPHYRUM;
    f->caput.clausulae = xar_creare(f->scheda,
        (i32)magnitudo(CodexClausula));
    f->notae = xar_creare(f->scheda, (i32)magnitudo(NotaClausulae));
    f->unitates = xar_creare(f->scheda,
        (i32)magnitudo(UnitasChronicae));
    a = chorda_aedificator_creare(f->scheda, (memoriae_index)MXXIV);
    si (visus == VISUS_PERSONAE || visus == VISUS_LOCA)
    {
        Xar* x = visus == VISUS_PERSONAE ? t->personae : t->loca;

        per (i = ZEPHYRUM; x && i < xar_numerus(x); i++)
        {
            chorda l = lineam_entitatis((constans ChronicaEntitas*)
                xar_obtinere(x, i), f->scheda);

            clausulam_addere(f, a, l, ZEPHYRUM, FALSUM, FALSUM);
            unitatem_addere(f, i, UNITAS_NOMEN, ZEPHYRUM, ZEPHYRUM,
                l.mensura);
        }
    }
    alioquin
    {
                         b32 persona = visus == VISUS_PERSONA;
                         b32 titulus = FALSUM;
        ChronicaGenusNominis genus   = persona ? CHRONICA_PERSONA
                                                : CHRONICA_LOCUS;

        per (i = ZEPHYRUM; t->declarationes
            && i < xar_numerus(t->declarationes); i++)
        {
            constans ChronicaNomenRes* r = (constans ChronicaNomenRes*)
                xar_obtinere(t->declarationes, i);
            chorda l;

            si (   r->declaratio.genus != genus
                || !chorda_aequalis(r->declaratio.titulus, appellatio))
            {
                perge;
            }
            si (!titulus)
            {
                clausulam_addere(f, a, chorda_ex_literis("Declared in",
                    f->scheda), ZEPHYRUM, VERUM, FALSUM);
                titulus = VERUM;
            }
            l = chorda_concatenare(chorda_concatenare(r->titulus_libri,
                chorda_ex_literis(", page ", f->scheda), f->scheda),
                r->pagina, f->scheda);
            clausulam_addere(f, a, l, ZEPHYRUM, FALSUM, FALSUM);
            unitatem_addere(f, i, UNITAS_DECLARATIO, ZEPHYRUM, ZEPHYRUM,
                l.mensura);
        }
        per (i = ZEPHYRUM; i < xar_numerus(t->res); i++)
        {
            constans ChronicaRes* r = (constans ChronicaRes*)
                xar_obtinere(t->res, i);
            character anni[XXIV];
               chorda l;

            si (!eventus_nominis(&r->eventus, persona, appellatio))
            {
                perge;
            }
            annum_formare(r->eventus.annus, anni);
            l = chorda_concatenare(chorda_ex_literis(anni, f->scheda),
                chorda_concatenare(chorda_ex_literis("  ", f->scheda),
                r->eventus.descriptio, f->scheda), f->scheda);
            clausulam_addere(f, a, l, r->eventus.annus, FALSUM, FALSUM);
            unitatem_addere(f, i, UNITAS_EVENTUS, ZEPHYRUM, ZEPHYRUM,
                ZEPHYRUM);
        }
    }
    f->caput.textus  = chorda_aedificator_finire(a);
    f->paginae       = lector_paginare(&f->caput, (i32)area.latitudo,
        (i32)area.altitudo, f->scheda);
    per (i = ZEPHYRUM; f->paginae && i < xar_numerus(f->unitates); i++)
    {
        UnitasChronicae* u = (UnitasChronicae*)xar_obtinere(f->unitates,
            i);

        u->pagina = (s32)lector_pagina_positionis(f->paginae,
            u->initium);
    }
    f->tabula       = t;
    f->flumen       = FALSUM;
    f->visus_arcae  = visus;
    f->nomen_arcae  = chorda_transcribere(appellatio, f->scheda);
    f->latitudo     = area.latitudo;
    f->altitudo     = area.altitudo;
    redde f->paginae ? f : NIHIL;
}


/* ==================================================
 * Flumen: paginae et anni
 * ================================================== */

/* index clausulae quae offset continet (ultima si ultra) */
interior i32
clausula_ad (
    constans ChronicaFigurae* f,
                         i32  offset)
{
    i32 i;
    i32 n = xar_numerus(f->caput.clausulae);

    per (i = ZEPHYRUM; i + I < n; i++)
    {
        constans CodexClausula* c = (constans CodexClausula*)
            xar_obtinere(f->caput.clausulae, i);

        si (offset <= c->finis)
        {
            redde i;
        }
    }
    redde n > ZEPHYRUM ? n - I : ZEPHYRUM;
}

interior constans NotaClausulae*
nota_ad (
    constans ChronicaFigurae* f,
                         i32  offset)
{
    redde (constans NotaClausulae*)xar_obtinere(f->notae,
        clausula_ad(f, offset));
}

/* linea prima / ultima paginae p */
interior constans LectorLinea*
linea_paginae (
    constans ChronicaFigurae* f,
                         s32  p,
                         b32  ultima)
{
    constans LectorPagina* pg = (constans LectorPagina*)xar_obtinere(
        f->paginae, (i32)p);

    si (!pg || xar_numerus(pg->lineae) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde (constans LectorLinea*)xar_obtinere(pg->lineae, ultima
        ? xar_numerus(pg->lineae) - I : ZEPHYRUM);
}

/* annus lineae primae paginae p */
interior s32
annus_paginae (
    constans ChronicaFigurae* f,
                         s32  p)
{
    constans LectorLinea* l = linea_paginae(f, p, FALSUM);

    redde l ? nota_ad(f, l->initium)->annus : ZEPHYRUM;
}

/* pagina ubi annus incipit (clausula prima eius); 0 si ignotus */
interior s32
pagina_initii (
    constans ChronicaFigurae* f,
                         s32  annus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(f->notae); i++)
    {
        constans NotaClausulae* n = (constans NotaClausulae*)
            xar_obtinere(f->notae, i);

        si (n->prima && n->annus == annus)
        {
            redde (s32)lector_pagina_positionis(f->paginae,
                ((constans CodexClausula*)xar_obtinere(
                f->caput.clausulae, i))->initium);
        }
    }
    redde ZEPHYRUM;
}

/* flumen: paginae et pagina visa (annus resolutus, deinde pagina
 * intra cursum paginarum ab eo incipientium); FALSUM si nulla */
interior b32
flumen_parare (
       ChronicaMontatio*  m,
        ChronicaFigurae** f,
                    s32*  pagina)
{
    constans ChronicaTabula* t;
                        s32  k;
                        s32  annus;
                        s32  basis;
                        s32  finis;
                        s32  n;

    t = tabula(m);
    k = index_currens(m, t);
    si (k < ZEPHYRUM)
    {
        redde FALSUM;
    }
    annus  = *(s32*)xar_obtinere(t->anni, (i32)k);
    *f     = paginas_parare(m, t, annus, VERUM);
    si (!*f)
    {
        redde FALSUM;
    }
    n      = (s32)xar_numerus((*f)->paginae);
    basis  = pagina_initii(*f, annus);
    finis  = basis;
    dum (finis + I < n && annus_paginae(*f, finis + I) == annus)
    {
        finis++;
    }
    /* pagina: campus; annus ex praesentia: pagina servata */
    si (   !campus_numerus(m, "pagina", &k)
        && (   campus_numerus(m, "annus", &k)
            || !paginam_servatam_legere(m, &k)))
    {
        k = ZEPHYRUM;
    }
    si (k < ZEPHYRUM)
    {
        k = ZEPHYRUM;
    }
    *pagina = basis + k > finis ? finis : basis + k;
    redde VERUM;
}

/* paginam p fluminis tenere: annus summus et distantia ab initio */
interior b32
paginam_fluminis_scribere (
     ChronicaMontatio* m,
      ChronicaFigurae* f,
                  s32  p)
{
    constans character* tituli[III];
    constans character* valores[III];
             character  anni[XVI];
             character  paginae[XVI];
                   s32  annus;

    annus = annus_paginae(f, p);
    sprintf(anni, "%d", (integer)annus);
    sprintf(paginae, "%d", (integer)(p - pagina_initii(f, annus)));
    tituli[ZEPHYRUM]   = "annus";
    valores[ZEPHYRUM]  = anni;
    tituli[I]          = "pagina";
    valores[I]         = paginae;
    tituli[II]         = "electus";
    valores[II]        = "";
    positionem_servare(m, annus, p - pagina_initii(f, annus));
    redde campos_scribere(m, tituli, valores, III);
}

/* h / l: pagina ubi annus prior / proximus incipit (praeter
 * paginam visam) */
interior b32
flumen_annum_movere (
    ChronicaMontatio* m,
                 s32  gradus)
{
    constans ChronicaTabula* t;
            ChronicaFigurae* f;
                        s32  p;
                        s32  q;
                        s32  i;

    si (!flumen_parare(m, &f, &p))
    {
        redde VERUM;
    }
    t = f->tabula;
    si (gradus > ZEPHYRUM)
    {
        per (i = ZEPHYRUM; i < (s32)xar_numerus(t->anni); i++)
        {
            q = pagina_initii(f, *(s32*)xar_obtinere(t->anni, (i32)i));
            si (q > p)
            {
                redde paginam_fluminis_scribere(m, f, q);
            }
        }
        redde VERUM;
    }
    q = pagina_initii(f, annus_paginae(f, p));
    si (q < p)
    {
        redde paginam_fluminis_scribere(m, f, q);
    }
    per (i = (s32)xar_numerus(t->anni) - I; i >= ZEPHYRUM; i--)
    {
        q = pagina_initii(f, *(s32*)xar_obtinere(t->anni, (i32)i));
        si (q < p)
        {
            redde paginam_fluminis_scribere(m, f, q);
        }
    }
    redde VERUM;
}

interior b32
flumen_paginam_movere (
    ChronicaMontatio* m,
                 s32  gradus)
{
     ChronicaFigurae* f;
                 s32  p;

    si (!flumen_parare(m, &f, &p))
    {
        redde VERUM;
    }
    p += gradus;
    si (p < ZEPHYRUM || p >= (s32)xar_numerus(f->paginae))
    {
        redde VERUM;
    }
    redde paginam_fluminis_scribere(m, f, p);
}


/* ==================================================
 * Praecepta
 * ================================================== */

/* annus prior (-1) aut proximus (+1) cum eventis; in fine nihil */
interior b32
annum_movere (
    ChronicaMontatio* m,
                 s32  gradus)
{
    constans ChronicaTabula* t;
                        s32  k;

    si (visus_nominum(m))
    {
        redde VERUM;
    }
    si (visus_fluminis(m))
    {
        redde flumen_annum_movere(m, gradus);
    }
    t = tabula(m);
    k = index_currens(m, t);
    si (   k < ZEPHYRUM || k + gradus < ZEPHYRUM
        || k + gradus >= (s32)xar_numerus(t->anni))
    {
        redde VERUM;
    }
    redde annum_scribere(m, *(s32*)xar_obtinere(t->anni,
        (i32)(k + gradus)));
}

interior b32
paginam_movere (
    ChronicaMontatio* m,
                 s32  gradus)
{
     ChronicaFigurae* f;
                 s32  annus;
                 s32  k;

    si (visus_fluminis(m))
    {
        redde flumen_paginam_movere(m, gradus);
    }
    si (visus_nominum(m))
    {
        (vacuum)annus;
        si (!visum_currens(m, &f, &k))
        {
            redde VERUM;
        }
        k += gradus;
    }
    alioquin si (!visum_parare(m, &annus, &f))
    {
        redde VERUM;
    }
    alioquin
    {
        k = pagina_currens(m, f) + gradus;
    }
    si (k < ZEPHYRUM || k >= (s32)xar_numerus(f->paginae))
    {
        redde VERUM;
    }
    redde paginam_scribere(m, k);
}

interior b32
fac_annum_priorem (
    vacuum* ctx)
{
    redde annum_movere((ChronicaMontatio*)ctx, -I);
}

interior b32
fac_annum_proximum (
    vacuum* ctx)
{
    redde annum_movere((ChronicaMontatio*)ctx, I);
}

interior b32
fac_paginam_proximam (
    vacuum* ctx)
{
    redde paginam_movere((ChronicaMontatio*)ctx, I);
}

interior b32
fac_paginam_priorem (
    vacuum* ctx)
{
    redde paginam_movere((ChronicaMontatio*)ctx, -I);
}


/* ==================================================
 * Electio (C4a)
 * ================================================== */

/* visus currens: paginae et pagina visa; FALSUM si nulla eventa */
interior b32
visum_currens (
    ChronicaMontatio*  m,
     ChronicaFigurae** f,
                 s32*  pagina)
{
    s32 annus;

    si (visus_fluminis(m))
    {
        redde flumen_parare(m, f, pagina);
    }
    si (visus_nominum(m))
    {
        *f = paginas_nominum_parare(m, tabula(m), visus_genus(m),
            campus(m, "nomen"));
        si (!*f)
        {
            redde FALSUM;
        }
        *pagina = pagina_currens(m, *f);
        redde VERUM;
    }
    si (!visum_parare(m, &annus, f))
    {
        redde FALSUM;
    }
    *pagina = pagina_currens(m, *f);
    redde VERUM;
}

/* clavis rei: "<liber>#<pagina>#<linea>.<columna>" [+ "#p<k>"|"#l"] */
interior chorda
clavis_unitatis (
    constans ChronicaFigurae* f,
    constans UnitasChronicae* u,
                     Piscina* piscina)
{
    constans ChronicaRes* r;
       ChordaAedificator* a;

    a = chorda_aedificator_creare(piscina, (memoriae_index)CXXVIII);
    si (u->genus == UNITAS_NOMEN)
    {
        Xar* x = f->visus_arcae == VISUS_LOCA ? f->tabula->loca
                                              : f->tabula->personae;

        chorda_aedificator_appendere_literis(a, "n:");
        chorda_aedificator_appendere_chorda(a, ((constans
            ChronicaEntitas*)xar_obtinere(x, u->res))->titulus);
        redde chorda_aedificator_finire(a);
    }
    si (u->genus == UNITAS_DECLARATIO)
    {
        constans ChronicaNomenRes* d = (constans ChronicaNomenRes*)
            xar_obtinere(f->tabula->declarationes, u->res);

        chorda_aedificator_appendere_literis(a, "d:");
        chorda_aedificator_appendere_chorda(a, d->liber);
        chorda_aedificator_appendere_character(a, '#');
        chorda_aedificator_appendere_chorda(a, d->pagina);
        chorda_aedificator_appendere_character(a, '#');
        chorda_aedificator_appendere_s32(a, (s32)d->declaratio.linea);
        chorda_aedificator_appendere_character(a, '.');
        chorda_aedificator_appendere_s32(a, (s32)d->declaratio.columna);
        redde chorda_aedificator_finire(a);
    }
    r = (constans ChronicaRes*)xar_obtinere(f->tabula->res, u->res);
    chorda_aedificator_appendere_chorda(a, r->liber);
    chorda_aedificator_appendere_character(a, '#');
    chorda_aedificator_appendere_chorda(a, r->pagina);
    chorda_aedificator_appendere_character(a, '#');
    chorda_aedificator_appendere_s32(a, (s32)r->eventus.linea);
    chorda_aedificator_appendere_character(a, '.');
    chorda_aedificator_appendere_s32(a, (s32)r->eventus.columna);
    si (u->genus == UNITAS_PERSONA)
    {
        chorda_aedificator_appendere_literis(a, "#p");
        chorda_aedificator_appendere_s32(a, (s32)u->k);
    }
    alioquin si (u->genus == UNITAS_LOCUS)
    {
        chorda_aedificator_appendere_literis(a, "#l");
    }
    redde chorda_aedificator_finire(a);
}

/* index rei electae in f->unitates; -1 si nulla aut deleta */
interior s32
electa (
    ChronicaMontatio* m,
     ChronicaFigurae* f)
{
    chorda clavis = campus(m, "electus");
       i32 i;

    per (i = ZEPHYRUM; clavis.mensura > ZEPHYRUM
        && i < xar_numerus(f->unitates); i++)
    {
        si (chorda_aequalis(clavis_unitatis(f,
            (constans UnitasChronicae*)
                xar_obtinere(f->unitates, i), m->piscina), clavis))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

interior b32
electum_scribere (
         ChronicaMontatio* m,
          ChronicaFigurae* f,
                      s32  index)
{
    constans character* tituli[I];
    constans character* valores[I];

    tituli[ZEPHYRUM]   = "electus";
    valores[ZEPHYRUM]  = index < ZEPHYRUM ? "" : chorda_ut_cstr(
        clavis_unitatis(f, (constans UnitasChronicae*)xar_obtinere(
        f->unitates, (i32)index), m->piscina), m->piscina);
    redde campos_scribere(m, tituli, valores, I);
}

/* Tab (+1) / S-Tab (-1): res proxima / prior; pagina sequitur */
interior b32
electum_movere (
    ChronicaMontatio* m,
                 s32  gradus)
{
              ChronicaFigurae* f;
                          s32  p;
                          s32  u;
                          s32  n;
                          s32  i;
              UnitasChronicae* x;

    si (!visum_currens(m, &f, &p))
    {
        redde VERUM;
    }
    n = (s32)xar_numerus(f->unitates);
    u = electa(m, f);
    si (u >= ZEPHYRUM)
    {
        u += gradus;
    }
    alioquin si (gradus > ZEPHYRUM)
    {
        per (u = ZEPHYRUM; u < n; u++)
        {
            si (((UnitasChronicae*)xar_obtinere(f->unitates, (i32)u))
                ->pagina >= p)
            {
                frange;
            }
        }
    }
    alioquin
    {
        per (u = -I, i = ZEPHYRUM; i < n; i++)
        {
            si (((UnitasChronicae*)xar_obtinere(f->unitates, (i32)i))
                ->pagina <= p)
            {
                u = i;
            }
        }
    }
    si (u < ZEPHYRUM || u >= n)
    {
        redde VERUM;
    }
    x = (UnitasChronicae*)xar_obtinere(f->unitates, (i32)u);
    si (x->pagina != p)
    {
        (vacuum)(visus_fluminis(m)
            ? paginam_fluminis_scribere(m, f, x->pagina)
            : paginam_scribere(m, x->pagina));
    }
    redde electum_scribere(m, f, u);
}

interior b32
fac_electum_proximum (
    vacuum* ctx)
{
    redde electum_movere((ChronicaMontatio*)ctx, I);
}

interior b32
fac_electum_priorem (
    vacuum* ctx)
{
    redde electum_movere((ChronicaMontatio*)ctx, -I);
}

/* Esc: electio tollitur; nulla - latus prius (vicus) */
interior b32
fac_effugere (
    vacuum* ctx)
{
      ChronicaMontatio* m = (ChronicaMontatio*)ctx;
    constans character* tituli[I];
    constans character* valores[I];

    si (campus(m, "electus").mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    tituli[ZEPHYRUM]   = "electus";
    valores[ZEPHYRUM]  = "";
    redde campos_scribere(m, tituli, valores, I);
}

/* C5b: pagina nominis (visus persona aut locus, nomen) */
interior b32
nomen_aperire (
    ChronicaMontatio* m,
                 b32  persona,
              chorda  appellatio)
{
    constans character* tituli[IV];
    constans character* valores[IV];

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = persona ? "persona" : "locus";
    tituli[I]          = "nomen";
    valores[I]         = chorda_ut_cstr(appellatio, m->piscina);
    tituli[II]         = "pagina";
    valores[II]        = "";
    tituli[III]        = "electus";
    valores[III]       = "";
    redde campos_scribere(m, tituli, valores, IV);
}

/* C5b: index personarum (VERUM) aut locorum */
interior b32
indicem_aperire (
    ChronicaMontatio* m,
                 b32  personae)
{
    constans character* tituli[III];
    constans character* valores[III];

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = personae ? "personae" : "loca";
    tituli[I]          = "pagina";
    valores[I]         = "";
    tituli[II]         = "electus";
    valores[II]        = "";
    redde campos_scribere(m, tituli, valores, III);
}

interior b32
fac_personas (
    vacuum* ctx)
{
    redde indicem_aperire((ChronicaMontatio*)ctx, VERUM);
}

interior b32
fac_loca (
    vacuum* ctx)
{
    redde indicem_aperire((ChronicaMontatio*)ctx, FALSUM);
}

/* C4b: eventus domum per portam: "scriba/<pagina>?linea=<n>" et
 * "&liber=<liber>" si comes; persona aut locus nihil (C5) */
interior b32
domum_aperire (
    ChronicaMontatio* m,
     ChronicaFigurae* f,
                 s32  index)
{
    constans UnitasChronicae* x;
        constans ChronicaRes* r;
           ChordaAedificator* a;
                      chorda  liber;
                      chorda  pagina;
                         i32  linea;

    si (index < ZEPHYRUM)
    {
        redde VERUM;
    }
    x = (constans UnitasChronicae*)xar_obtinere(f->unitates,
        (i32)index);
    /* C5b: nomen (indicis, personae, loci) -> pagina eius */
    si (x->genus == UNITAS_NOMEN)
    {
        Xar* l = f->visus_arcae == VISUS_LOCA ? f->tabula->loca
                                              : f->tabula->personae;

        redde nomen_aperire(m, f->visus_arcae != VISUS_LOCA,
            ((constans ChronicaEntitas*)xar_obtinere(l, x->res))
            ->titulus);
    }
    si (x->genus == UNITAS_PERSONA || x->genus == UNITAS_LOCUS)
    {
        r = (constans ChronicaRes*)xar_obtinere(f->tabula->res, x->res);
        redde nomen_aperire(m, x->genus == UNITAS_PERSONA,
            x->genus == UNITAS_PERSONA
            ? *(chorda*)xar_obtinere(r->eventus.personae, x->k)
            : r->eventus.locus);
    }
    si (!m->aperire)
    {
        redde VERUM;
    }
    si (x->genus == UNITAS_DECLARATIO)
    {
        constans ChronicaNomenRes* d = (constans ChronicaNomenRes*)
            xar_obtinere(f->tabula->declarationes, x->res);

        liber   = d->liber;
        pagina  = d->pagina;
        linea   = d->declaratio.linea;
    }
    alioquin
    {
        r = (constans ChronicaRes*)xar_obtinere(f->tabula->res, x->res);
        liber = r->liber;
        pagina = r->pagina;
        linea = r->eventus.linea;
    }
    a = chorda_aedificator_creare(m->piscina, (memoriae_index)CXXVIII);
    chorda_aedificator_appendere_literis(a, "scriba/");
    chorda_aedificator_appendere_chorda(a, pagina);
    chorda_aedificator_appendere_literis(a, "?linea=");
    chorda_aedificator_appendere_s32(a, (s32)linea);
    si (liber.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "&liber=");
        chorda_aedificator_appendere_chorda(a, liber);
    }
    redde m->aperire(m->aperire_ctx, chorda_aedificator_finire(a));
}

interior b32
fac_domum (
    vacuum* ctx)
{
    ChronicaMontatio* m = (ChronicaMontatio*)ctx;
     ChronicaFigurae* f;
                 s32  p;

    si (!visum_currens(m, &f, &p))
    {
        redde VERUM;
    }
    redde domum_aperire(m, f, electa(m, f));
}

/* runae (UTF-8) in octetis [a, b) */
interior i32
runae (
    chorda textus,
       i32 a,
       i32 b)
{
    i32 n = ZEPHYRUM;

    per (; a < b && a < textus.mensura; a++)
    {
        si ((textus.datum[a] & 0xC0) != 0x80)
        {
            n++;
        }
    }
    redde n;
}

/* ictus in punctum radicis: res sub eo eligitur; FALSUM si extra
 * textum (pagina tum vertitur) */
interior b32
ictum_eligere (
    ChronicaMontatio* m,
             Punctum  q)
{
          ChronicaFigurae* f;
    constans LectorPagina* pg;
                    Fines  origo;
                      s32  p;
                      i32  i;
                      i32  u;

    si (!visum_currens(m, &f, &p))
    {
        redde FALSUM;
    }
    origo  = visus_fluminis(m) ? columna_fluminis(m) : area_eventuum(m);
    pg     = (constans LectorPagina*)xar_obtinere(f->paginae, (i32)p);
    per (i = ZEPHYRUM; pg && i < xar_numerus(pg->lineae); i++)
    {
        constans LectorLinea* l = (constans LectorLinea*)xar_obtinere(
            pg->lineae, i);
                         s32 x0  = origo.x + l->x;
                         s32 y0  = origo.y + l->y;
                         s32 w   = fons_latitudo(l->fons);
                         s32 k;
                         i32 off;

        si (   q.y < y0 || q.y >= y0 + fons_altitudo(l->fons)
            || q.x < x0 || w <= ZEPHYRUM
            || (q.x - x0) / w >= (s32)runae(f->caput.textus, l->initium,
                   l->finis))
        {
            perge;
        }
        /* runa k -> octetus */
        k    = (q.x - x0) / w;
        off  = l->initium;
        dum (off < l->finis)
        {
            si ((f->caput.textus.datum[off] & 0xC0) != 0x80)
            {
                si (k == ZEPHYRUM)
                {
                    frange;
                }
                k--;
            }
            off++;
        }
        per (u = ZEPHYRUM; u < xar_numerus(f->unitates); u++)
        {
            constans UnitasChronicae* x = (constans UnitasChronicae*)
                xar_obtinere(f->unitates, u);

            si (off >= x->initium && off < x->finis)
            {
                /* C4b: ictus in eventu iam electo domum aperit */
                si (electa(m, f) == (s32)u)
                {
                    redde domum_aperire(m, f, (s32)u);
                }
                redde electum_scribere(m, f, (s32)u);
            }
        }
        redde FALSUM;
    }
    redde FALSUM;
}

/* res electa inversa in pagina p (lineae quas ambitus tangit) */
interior vacuum
electam_pingere (
     ChronicaMontatio* m,
              Mandata* md,
      ChronicaFigurae* f,
                  s32  p,
                Fines  origo)
{
       constans LectorPagina* pg;
    constans UnitasChronicae* x;
                         s32  u;
                         i32  i;

    u   = electa(m, f);
    pg  = (constans LectorPagina*)xar_obtinere(f->paginae, (i32)p);
    si (u < ZEPHYRUM || !pg)
    {
        redde;
    }
    x = (constans UnitasChronicae*)xar_obtinere(f->unitates, (i32)u);
    per (i = ZEPHYRUM; i < xar_numerus(pg->lineae); i++)
    {
        constans LectorLinea* l = (constans LectorLinea*)xar_obtinere(
            pg->lineae, i);
                         i32 a = x->initium > l->initium ? x->initium
                                                          : l->initium;
                         i32 b = x->finis < l->finis ? x->finis
                                                      : l->finis;
                         s32 w = fons_latitudo(l->fons);
                       Fines r;

        si (a >= b)
        {
            perge;
        }
        r.x         = origo.x + l->x + (s32)runae(f->caput.textus,
            l->initium, a) * w;
        r.y         = origo.y + l->y;
        r.latitudo  = (s32)runae(f->caput.textus, a, b) * w;
        r.altitudo  = fons_altitudo(l->fons);
        mandata_rectangulum(md, r, figura_color_thematis(COLOR_TEXT),
            VERUM);
        mandata_textus(md, r.x, r.y, chorda_sectio(f->caput.textus, a,
            b), l->fons, figura_color_thematis(COLOR_BACKGROUND));
    }
}

/* pes cum electa: "-> note: <titulus libri>", "-> person: <n>",
 * "-> place: <n>"; sine: titulus loci */
interior chorda
pes_electae (
     ChronicaMontatio* m,
      ChronicaFigurae* f,
               chorda  titulus,
              Piscina* piscina)
{
    constans UnitasChronicae* x;
        constans ChronicaRes* r;
                         s32  u = electa(m, f);

    si (u < ZEPHYRUM)
    {
        redde titulus;
    }
    x = (constans UnitasChronicae*)xar_obtinere(f->unitates, (i32)u);
    si (x->genus == UNITAS_NOMEN)
    {
        Xar* l = f->visus_arcae == VISUS_LOCA ? f->tabula->loca
                                              : f->tabula->personae;

        redde chorda_concatenare(chorda_ex_literis(
            f->visus_arcae == VISUS_LOCA ? "-> place: " : "-> person: ",
            piscina), ((constans ChronicaEntitas*)xar_obtinere(l,
            x->res))->titulus, piscina);
    }
    si (x->genus == UNITAS_DECLARATIO)
    {
        redde chorda_concatenare(chorda_ex_literis("-> note: ",
            piscina),
            ((constans ChronicaNomenRes*)xar_obtinere(
            f->tabula->declarationes, x->res))->titulus_libri, piscina);
    }
    r = (constans ChronicaRes*)xar_obtinere(f->tabula->res, x->res);
    si (x->genus == UNITAS_PERSONA)

    {
        redde chorda_concatenare(chorda_ex_literis("-> person: ",
            piscina), *(chorda*)xar_obtinere(r->eventus.personae, x->k),
            piscina);
    }
    si (x->genus == UNITAS_LOCUS)
    {
        redde chorda_concatenare(chorda_ex_literis("-> place: ",
            piscina), r->eventus.locus, piscina);
    }
    redde chorda_concatenare(chorda_ex_literis("-> note: ", piscina),
        r->titulus_libri, piscina);
}

/* v: flumen -> annus (annus summus paginae); annus -> flumen (annus
 * servatus, pagina ubi incipit) */
interior b32
fac_visum_vertere (
    vacuum* ctx)
{
       ChronicaMontatio* m = (ChronicaMontatio*)ctx;
        ChronicaFigurae* f;
     constans character* tituli[IV];
     constans character* valores[IV];
              character  anni[XVI];
                    s32  p;
                    s32  annus;

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = visus_fluminis(m) ? "annus" : "flumen";
    tituli[I]          = "pagina";
    valores[I]         = "";
    tituli[II]         = "electus";
    valores[II]        = "";
    si (visus_fluminis(m) && flumen_parare(m, &f, &p))
    {
        annus = annus_paginae(f, p);
    }
    alioquin si (visus_fluminis(m) || !visum_parare(m, &annus, &f))
    {
        redde campos_scribere(m, tituli, valores, III);
    }
    sprintf(anni, "%d", (integer)annus);
    tituli[III]   = "annus";
    valores[III]  = anni;
    positionem_servare(m, annus, ZEPHYRUM);
    redde campos_scribere(m, tituli, valores, IV);
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "chronica.annus_prior", "Previous year", "h",
        VISUS_ANNI | VISUS_FLUMINIS, ZEPHYRUM, fac_annum_priorem },
    { "chronica.annus_proximus", "Next year", "l",
        VISUS_ANNI | VISUS_FLUMINIS, ZEPHYRUM, fac_annum_proximum },
    { "chronica.pagina_proxima", "Next page", "j",
        VISUS_OMNES, ZEPHYRUM, fac_paginam_proximam },
    { "chronica.pagina_prior", "Previous page", "k",
        VISUS_OMNES, ZEPHYRUM, fac_paginam_priorem },
    { "chronica.deorsum", "Next page", "Down",
        VISUS_OMNES, ZEPHYRUM, fac_paginam_proximam },
    { "chronica.sursum", "Previous page", "Up",
        VISUS_OMNES, ZEPHYRUM, fac_paginam_priorem },
    { "chronica.vertere", "Year / continuous", "v",
        VISUS_ANNI | VISUS_FLUMINIS, ZEPHYRUM, fac_visum_vertere },
    { "chronica.electum_proximum", "Next item", "Tab",
        VISUS_OMNES, ZEPHYRUM, fac_electum_proximum },
    { "chronica.electum_prius", "Previous item", "S-Tab",
        VISUS_OMNES, ZEPHYRUM, fac_electum_priorem },
    { "chronica.effugere", "Clear selection / back", "Esc",
        VISUS_OMNES, ZEPHYRUM, fac_effugere },
    { "chronica.domum", "Open note", "Enter",
        VISUS_OMNES, ZEPHYRUM, fac_domum },
    { "chronica.personae", "People", "p", VISUS_OMNES, ZEPHYRUM,
        fac_personas },
    { "chronica.loca", "Places", "P", VISUS_OMNES, ZEPHYRUM, fac_loca },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

interior b32
requisita_nulla (
        i32  requisita,
     vacuum* ctx)
{
    (vacuum)requisita;
    (vacuum)ctx;
    redde VERUM;
}


/* ==================================================
 * Actio
 * ================================================== */

interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
         ChronicaMontatio* m = (ChronicaMontatio*)ctx;
      constans Praeceptum* t;
                      i32  visus;

    (vacuum)repo;
    (vacuum)motus;
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        si (ev->datum.clavis.clavis == CLAVIS_SINISTER)
        {
            redde annum_movere(m, -I);
        }
        si (ev->datum.clavis.clavis == CLAVIS_DEXTER)
        {
            redde annum_movere(m, I);
        }
        visus  = visus_genus(m);
        t      = praecepta_ad_clavem(TABULA_PRAECEPTORUM, visus, ev);
        redde t && praecepta_status(t, visus, requisita_nulla, m)
            == PRAECEPTUM_PRAESTO && t->fn(m);
    }
    si (   !nodus || ev->genus != EVENTUS_MUS_DEPRESSUS
        || ev->datum.mus.botton != MUS_SINISTER)
    {
        redde FALSUM;
    }
    si (chorda_aequalis_literis(nodus->id, "sagitta_prior"))
    {
        redde annum_movere(m, -I);
    }
    si (chorda_aequalis_literis(nodus->id, "sagitta_proxima"))
    {
        redde annum_movere(m, I);
    }
    /* C4a: textus sub ictu eligitur (punctum locale: latus in vico
     * non in 0,0); extra textum dimidia paginam vertunt */
    si (   chorda_aequalis_literis(nodus->id, "supra")
        || chorda_aequalis_literis(nodus->id, "infra"))
    {
        Punctum q;

        q.x = nodus->fines.x + (des ? des->punctum_locale.x
                                    : (s32)ev->datum.mus.x
                                      - nodus->fines.x);
        q.y = nodus->fines.y + (des ? des->punctum_locale.y
                                    : (s32)ev->datum.mus.y
                                      - nodus->fines.y);
        si (ictum_eligere(m, q))
        {
            redde VERUM;
        }
    }
    si (chorda_aequalis_literis(nodus->id, "supra"))
    {
        redde paginam_movere(m, -I);
    }
    si (chorda_aequalis_literis(nodus->id, "infra"))
    {
        redde paginam_movere(m, I);
    }
    redde FALSUM;
}


/* ==================================================
 * Figura
 * ================================================== */

/* clausula obscura (linea parva)? */
interior b32
obscura (
    constans ChronicaFigurae* f,
                         i32  initium)
{
    redde xar_numerus(f->notae) > ZEPHYRUM
        && nota_ad(f, initium)->obscura;
}

interior vacuum
pedem_pingere (
     Mandata* md,
       Fines  fines,
      chorda  titulus,
         s32  y_pedis)
{
    mandata_textus(md, CHRONICA_SINISTRUM, y_pedis,
        chorda_ex_literis("<", md->piscina), FONS_PARVUS,
        figura_color_thematis(COLOR_TEXT));
    mandata_textus(md, CHRONICA_SINISTRUM
        + III * fons_latitudo(FONS_PARVUS), y_pedis, titulus,
        FONS_PARVUS, figura_color_thematis(COLOR_TEXT_DIM));
    mandata_textus(md, fines.latitudo - CHRONICA_SINISTRUM
        - fons_latitudo(FONS_PARVUS), y_pedis, chorda_ex_literis(">",
        md->piscina), FONS_PARVUS, figura_color_thematis(COLOR_TEXT));
}

/* flumen: annus in columna sinistra ad lineam primam anni et in summo
 * paginae (annus fluens); eventa in dextra */
interior vacuum
flumen_pingere (
     ChronicaMontatio* m,
              Mandata* md,
      ChronicaFigurae* f,
                  s32  p)
{
    constans LectorPagina* pg;
                    Fines  tota;
                    Fines  columna;
                      s32  latitudo_anni;
                      i32  i;
                character  anni[XXIV];

    tota = area_fluminis(m);
    columna = columna_fluminis(m);
    latitudo_anni = columna.x - tota.x;
    pg = (constans LectorPagina*)xar_obtinere(f->paginae, (i32)p);
    per (i = ZEPHYRUM; pg && i < xar_numerus(pg->lineae); i++)
    {
        constans LectorLinea* l = (constans LectorLinea*)xar_obtinere(
            pg->lineae, i);
        constans NotaClausulae* n = nota_ad(f, l->initium);
        constans CodexClausula* c = (constans CodexClausula*)
            xar_obtinere(f->caput.clausulae, clausula_ad(f,
            l->initium));

        mandata_textus(md, columna.x + l->x, columna.y + l->y,
            chorda_sectio(f->caput.textus, l->initium, l->finis),
            l->fons, figura_color_thematis(n->obscura ? COLOR_TEXT_DIM
            : COLOR_TEXT));
        si (i == ZEPHYRUM || (n->prima && l->initium == c->initium))
        {
            i32 fons;

            annum_formare(n->annus, anni);
            fons = (s32)strlen(anni) * fons_latitudo(FONS_TITULI)
                   <= latitudo_anni - IV ? FONS_TITULI : FONS_PARVUS;
            mandata_textus(md, tota.x, columna.y + l->y,
                chorda_ex_literis(anni, md->piscina), fons,
                figura_color_thematis(COLOR_TEXT));
        }
    }
    electam_pingere(m, md, f, p, columna);
}

/* C5b: titulus magnus visus nominum */
interior chorda
caput_nominum (
    ChronicaMontatio* m,
             Piscina* piscina)
{
    i32 v = visus_genus(m);

    si (v == VISUS_PERSONAE)
    {
        redde chorda_ex_literis("People", piscina);
    }
    si (v == VISUS_LOCA)
    {
        redde chorda_ex_literis("Places", piscina);
    }
    redde chorda_transcribere(campus(m, "nomen"), piscina);
}

interior vacuum
figura_chronicae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
     ChronicaMontatio* m = (ChronicaMontatio*)ctx;
      ChronicaFigurae* f;
                Fines  fines;
                Fines  area;
                Fines  fundus;
                  s32  annus;
                  s32  y_pedis;
               chorda  titulus;
            character  textus[XXIV];
                  i32  i;

    (vacuum)thema;
    fines.x         = ZEPHYRUM;
    fines.y         = ZEPHYRUM;
    fines.latitudo  = c->fines.latitudo;
    fines.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, fines, chorda_aequalis_literis(c->titulus,
        "focatum"));
    /* fundus intra limbum: textura solum in margine (ut navigator) */
    fundus.x         = CHRONICA_LIMBUS_X;
    fundus.y         = CHRONICA_LIMBUS_Y;
    fundus.latitudo  = fines.latitudo - II * CHRONICA_LIMBUS_X;
    fundus.altitudo  = fines.altitudo - II * CHRONICA_LIMBUS_Y;
    mandata_rectangulum(md, fundus, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (!m)
    {
        redde;
    }
    area     = area_eventuum(m);
    y_pedis  = fines.altitudo - CHRONICA_LIMBUS_Y - X;
    titulus  = chronica_locus_titulus(m, md->piscina);
    /* C5b: indices et paginae nominum (forma visus anni) */
    si (visus_nominum(m))
    {
        s32 p;

        mandata_textus(md, CHRONICA_SINISTRUM, CHRONICA_LIMBUS_Y
            + CHRONICA_MARGO, caput_nominum(m, md->piscina),
            FONS_TITULI,
            figura_color_thematis(COLOR_TEXT));
        si (visum_currens(m, &f, &p))
        {
            constans LectorPagina* pg = (constans LectorPagina*)
                xar_obtinere(f->paginae, (i32)p);

            per (i = ZEPHYRUM; pg && i < xar_numerus(pg->lineae); i++)
            {
                constans LectorLinea* l = (constans LectorLinea*)
                    xar_obtinere(pg->lineae, i);

                mandata_textus(md, area.x + l->x, area.y + l->y,
                    chorda_sectio(f->caput.textus, l->initium,
                    l->finis),
                    l->fons, figura_color_thematis(obscura(f,
                    l->initium)
                    ? COLOR_TEXT_DIM : COLOR_TEXT));
            }
            electam_pingere(m, md, f, p, area);
            titulus = pes_electae(m, f, titulus, md->piscina);
        }
        pedem_pingere(md, fines, titulus, y_pedis);
        redde;
    }
    si (visus_fluminis(m) && flumen_parare(m, &f, &annus))
    {
        flumen_pingere(m, md, f, annus);
        pedem_pingere(md, fines, pes_electae(m, f, titulus,
            md->piscina),
            y_pedis);
        redde;
    }
    si (visus_fluminis(m) || !visum_parare(m, &annus, &f))
    {
        mandata_textus(md, area.x, area.y, chorda_ex_literis(
            textus_vacuus, md->piscina), FONS_PARVUS,
            figura_color_thematis(COLOR_TEXT_DIM));
        mandata_textus(md, CHRONICA_SINISTRUM, y_pedis, titulus,
            FONS_PARVUS,
            figura_color_thematis(COLOR_TEXT_DIM));
        redde;
    }
    annum_formare(annus, textus);
    mandata_textus(md, CHRONICA_SINISTRUM, CHRONICA_LIMBUS_Y
        + CHRONICA_MARGO, chorda_ex_literis(
        textus, md->piscina), FONS_TITULI,
        figura_color_thematis(COLOR_TEXT));
    {
        constans LectorPagina* p = (constans LectorPagina*)xar_obtinere(
            f->paginae, (i32)pagina_currens(m, f));

        per (i = ZEPHYRUM; p && i < xar_numerus(p->lineae); i++)
        {
            constans LectorLinea* l = (constans LectorLinea*)
                xar_obtinere(p->lineae, i);

            mandata_textus(md, area.x + l->x, area.y + l->y,
                chorda_sectio(f->caput.textus, l->initium, l->finis),
                l->fons, figura_color_thematis(obscura(f, l->initium)
                ? COLOR_TEXT_DIM : COLOR_TEXT));
        }
    }
    electam_pingere(m, md, f, pagina_currens(m, f), area);
    /* pes: "<  Timeline - AD 1274 - 1/2  >" */
    pedem_pingere(md, fines, pes_electae(m, f, titulus, md->piscina),
        y_pedis);
}


/* ==================================================
 * Componere et montare
 * ================================================== */

interior vacuum
liberum_addere (
               Componens* radix,
                 Piscina* piscina,
     InternamentumChorda* intern,
      constans character* id,
                   Fines  fines)
{
    Componens* c = componens_creare(piscina, intern, id, PARTES_NULLUM);

    si (c)
    {
        componens_ponere_fines(c, fines);
        componens_ponere_actio(c, actio_chronicae);
        componens_addere_liberum(radix, c);
    }
}

Componens*
chronica_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    ChronicaMontatio* m = (ChronicaMontatio*)ctx;
           Componens* radix;
               Fines  f;
               Fines  area;
               Fines  dimidium;
               Fines  sagitta;
                 s32  littera;

    (vacuum)repo;
    si (!m)
    {
        redde NIHIL;
    }
    radix = componens_creare(piscina, intern, "chronica",
        PARTES_TABULA);
    si (!radix)
    {
        redde NIHIL;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = superficies(m, VERUM);
    f.altitudo  = superficies(m, FALSUM);
    componens_ponere_fines(radix, f);
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_chronicae);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    area               = area_visus(m);
    dimidium           = area;
    dimidium.altitudo  = area.altitudo / II;
    liberum_addere(radix, piscina, intern, "supra", dimidium);
    dimidium.y         = area.y + area.altitudo / II;
    dimidium.altitudo  = area.altitudo - area.altitudo / II;
    liberum_addere(radix, piscina, intern, "infra", dimidium);
    littera           = fons_latitudo(FONS_PARVUS);
    sagitta.x         = CHRONICA_LIMBUS_X;
    sagitta.y         = f.altitudo - CHRONICA_LIMBUS_Y - CHRONICA_PES;
    sagitta.latitudo  = CHRONICA_MARGO + II * littera;
    sagitta.altitudo  = CHRONICA_PES;
    liberum_addere(radix, piscina, intern, "sagitta_prior", sagitta);
    sagitta.x           = f.latitudo - CHRONICA_SINISTRUM
                        - II * littera;
    liberum_addere(radix, piscina, intern, "sagitta_proxima", sagitta);
    redde radix;
}

b32
chronica_montare (
       ChronicaMontatio* m,
                Piscina* piscina,
    InternamentumChorda* intern,
     InsulaRepositorium* repo,
     constans character* id,
         ChronicaFontes* fontes,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo,
        StatusSystemata* systemata)
{
            Organum* organum;
    ChronicaFigurae* f;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(ChronicaMontatio));
    m->piscina  = piscina;
    m->intern   = intern;
    m->fontes   = fontes;
    m->volumen  = volumen;
    m->ramus    = id ? insula_ramus(repo, "chronica", id)
                     : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "chronica",
        "chronica");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"chronica\"", id ? id : "chronica", piscina,
               intern))
    {
        redde FALSUM;
    }
    f = (ChronicaFigurae*)piscina_allocare(piscina,
        magnitudo(ChronicaFigurae));
    memset(f, ZEPHYRUM, magnitudo(ChronicaFigurae));
    f->organum               = organum;
    f->latitudo_montationis  = latitudo;
    f->altitudo_montationis  = altitudo;
    f->scheda = piscina_generare_dynamicum("chronica_paginae",
        IV * MXXIV);
    m->figurae_ctx  = f;
    m->actiones     = actio_registrum_creare(piscina, intern);
    m->figurae      = figura_registrum_creare(piscina);
    redde f->scheda && m->actiones && m->figurae
        && praecepta_verificare(TABULA_PRAECEPTORUM)
        && actio_registrare(m->actiones, actio_chronicae, agere, m)
        && figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_chronicae, m);
}


/* ==================================================
 * Semitae et locus
 * ================================================== */

interior constans Organum*
organa (
    vacuum* ctx,
    chorda  genus)
{
    ChronicaMontatio* m = (ChronicaMontatio*)ctx;

    si (   !m || !m->figurae_ctx
        || !chorda_aequalis_literis(genus, "chronica"))
    {
        redde NIHIL;
    }
    redde ((ChronicaFigurae*)m->figurae_ctx)->organum;
}

/* C5b: nomen in loco effugitum ut semitae (%XX extra A-Z a-z 0-9 -
 * _ . ~) */
interior vacuum
effugere (
    ChordaAedificator* a,
               chorda  valor)
{
       hic_manens constans character hex[] = "0123456789ABCDEF";
                                 i32 i;

    per (i = ZEPHYRUM; i < valor.mensura; i++)
    {
        insignatus character c = (insignatus character)valor.datum[i];

        si (   (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
            || (c >= '0' && c <= '9') || c == '-' || c == '_'
            || c == '.' || c == '~')
        {
            chorda_aedificator_appendere_character(a, (character)c);
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a, '%');
            chorda_aedificator_appendere_character(a, hex[c >> IV]);
            chorda_aedificator_appendere_character(a, hex[c & XV]);
        }
    }
}

interior s32
hex_valor (
    i8 c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'A' && c <= 'F')
    {
        redde (s32)(c - 'A') + X;
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a') + X;
    }
    redde -I;
}

/* %XX solvere; FALSUM si effugium malum aut vacuum */
interior b32
solvere (
     chorda  valor,
    Piscina* piscina,
     chorda* exitus)
{
    ChordaAedificator* a;
                  i32  i;

    a = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    per (i = ZEPHYRUM; i < valor.mensura; i++)
    {
        si (valor.datum[i] == '%')
        {
            s32 h;
            s32 l;

            si (i + II >= valor.mensura)
            {
                redde FALSUM;
            }
            h = hex_valor(valor.datum[i + I]);
            l = hex_valor(valor.datum[i + II]);
            si (h < ZEPHYRUM || l < ZEPHYRUM)
            {
                redde FALSUM;
            }
            chorda_aedificator_appendere_character(a,
                (character)(h * XVI + l));
            i += II;
        }
        alioquin
        {
            chorda_aedificator_appendere_character(a,
                (character)valor.datum[i]);
        }
    }
    *exitus = chorda_aedificator_finire(a);
    redde exitus->mensura > ZEPHYRUM;
}

b32
chronica_argumentum_ponere (
       ChronicaMontatio* m,
     constans character* argumentum)
{
    StmlNodus* semita;
       chorda* annus;
       chorda  vitium;
          s32  valor;

    si (!m || !m->figurae_ctx)
    {
        redde FALSUM;
    }
    /* C5b: "chronica/persona/<n>", "chronica/locus/<n>" manu (nomen
     * campus status, non ligatio) */
    si (argumentum)
    {
        hic_manens constans character persona[]  = "chronica/persona/";
        hic_manens constans character locus[]    = "chronica/locus/";
                                  b32 est_p;
                               chorda appellatio;

        est_p = strncmp(argumentum, persona, strlen(persona))
                == ZEPHYRUM;
        si (   est_p || strncmp(argumentum, locus, strlen(locus))
                     == ZEPHYRUM)
        {
            si (!solvere(chorda_ex_literis(argumentum + strlen(est_p
                    ? persona : locus), m->piscina), m->piscina,
                    &appellatio))
            {
                redde FALSUM;
            }
            redde nomen_aperire(m, est_p, appellatio);
        }
    }
    semita = semita_ex_argumento(organa, m, "chronica", argumentum
        ? argumentum : "", m->piscina, m->intern, &vitium);
    si (!semita)
    {
        redde FALSUM;
    }
    annus = stml_attributum_capere(semita, "annus");
    valor = ZEPHYRUM;
    si (   annus && annus->mensura > ZEPHYRUM
        && (!chorda_ut_s32(*annus, &valor) || valor == ZEPHYRUM))
    {
        redde FALSUM;
    }
    si (!semita_applicare(organa, m, &m->ramus, semita, m->piscina,
            &vitium))
    {
        redde FALSUM;
    }
    si (valor != ZEPHYRUM)
    {
        positionem_servare(m, valor, ZEPHYRUM);
    }
    redde chronica_semitam_secuta(m);
}

b32
chronica_semitam_secuta (
    vacuum* ctx)
{
    ChronicaMontatio* m = (ChronicaMontatio*)ctx;
                 s32  annus;

    /* historia et restitutio: annus nominatus fit positio servata */
    si (m && campus_numerus(m, "annus", &annus) && annus != ZEPHYRUM)
    {
        s32 pagina;

        si (   !visus_fluminis(m)
            || !campus_numerus(m, "pagina", &pagina))
        {
            pagina = ZEPHYRUM;
        }
        positionem_servare(m, annus, pagina);
    }
    redde m != NIHIL;
}

chorda
chronica_locus (
     vacuum* ctx,
    Piscina* piscina)
{
            ChronicaMontatio* m = (ChronicaMontatio*)ctx;
     constans ChronicaTabula* t;
                         s32  k;
                   character  textus[XL];

    si (!m || !piscina)
    {
        redde chorda_ex_literis("", piscina);
    }
    si (visus_nominum(m))
    {
                      i32  v = visus_genus(m);
        ChordaAedificator* a;

        si (v == VISUS_PERSONAE || v == VISUS_LOCA)
        {
            redde chorda_ex_literis(v == VISUS_PERSONAE
                ? "chronica/personae" : "chronica/loca", piscina);
        }
        a = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
        chorda_aedificator_appendere_literis(a, v == VISUS_PERSONA
            ? "chronica/persona/" : "chronica/locus/");
        effugere(a, campus(m, "nomen"));
        redde chorda_aedificator_finire(a);
    }
    si (visus_fluminis(m))
    {
        ChronicaFigurae* f;
                    s32  p;

        si (!flumen_parare(m, &f, &p))
        {
            redde chorda_ex_literis("chronica/flumen", piscina);
        }
        sprintf(textus, "chronica/flumen/%d", (integer)annus_paginae(f,
            p));
        redde chorda_ex_literis(textus, piscina);
    }
    t = tabula(m);
    k = index_currens(m, t);
    si (k < ZEPHYRUM)
    {
        redde chorda_ex_literis(clavis_positionis, piscina);
    }
    sprintf(textus, "%s/%d", clavis_positionis,
        (integer)*(s32*)xar_obtinere(t->anni, (i32)k));
    redde chorda_ex_literis(textus, piscina);
}

chorda
chronica_locus_titulus (
     vacuum* ctx,
    Piscina* piscina)
{
    ChronicaMontatio* m = (ChronicaMontatio*)ctx;
     ChronicaFigurae* f;
                 s32  annus;
           character  textus[LXIV];
           character  anni[XXIV];

    /* C5b: "People - 1/3", "Moses - 1/2" */
    si (m && visus_nominum(m))
    {
        s32 p;

        si (!visum_currens(m, &f, &p))
        {
            redde caput_nominum(m, piscina);
        }
        sprintf(textus, " - %d/%u", (integer)(p + I),
            (insignatus integer)xar_numerus(f->paginae));
        redde chorda_concatenare(caput_nominum(m, piscina),
            chorda_ex_literis(textus, piscina), piscina);
    }
    si (m && visus_fluminis(m))
    {
                         s32  p;
                         s32  ultimus;
        constans LectorLinea* l;

        si (!flumen_parare(m, &f, &p))
        {
            redde chorda_ex_literis("Timeline", piscina);
        }
        annus    = annus_paginae(f, p);
        l        = linea_paginae(f, p, VERUM);
        ultimus  = l ? nota_ad(f, l->initium)->annus : annus;
        annum_formare(annus, anni);
        si (ultimus == annus)
        {
            sprintf(textus, "Timeline \302\267 %s \302\267 %d/%u", anni,
                (integer)(p + I), (insignatus integer)xar_numerus(
                f->paginae));
        }
        alioquin
        {
            character ultimi[XXIV];

            annum_formare(ultimus, ultimi);
            sprintf(textus, "Timeline \302\267 %s \342\200\223 %s "
                "\302\267 %d/%u", anni, ultimi, (integer)(p + I),
                (insignatus integer)xar_numerus(f->paginae));
        }
        redde chorda_ex_literis(textus, piscina);
    }
    si (!m || !visum_parare(m, &annus, &f))
    {
        redde chorda_ex_literis("Timeline", piscina);
    }
    annum_formare(annus, anni);
    sprintf(textus, "Timeline - %s - %d/%u", anni,
        (integer)(pagina_currens(m, f) + I),
        (insignatus integer)xar_numerus(f->paginae));
    redde chorda_ex_literis(textus, piscina);
}
