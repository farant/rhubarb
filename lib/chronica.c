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

hic_manens constans character actio_chronicae[] = "chronica.agere";
hic_manens constans character clavis_positionis[] = "chronica/annus";
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
                 CodexCaput  caput;
                        Xar* obscurae;      /* b32 per clausulam */
                        Xar* paginae;       /* LectorPagina */
} ChronicaFigurae;

nomen structura {
    constans character* constans* tituli;
    constans character* constans* valores;
                             i32 numerus;
} GrexCamporum;


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
                 s32  annus)
{
    character numerus[XVI];

    si (m->volumen)
    {
        sprintf(numerus, "%d", (integer)annus);
        (vacuum)volumen_praesens_ponere(m->volumen,
            chorda_ex_literis(clavis_positionis, m->piscina),
            chorda_ex_literis(numerus, m->piscina));
    }
}

/* annus novus: campus et praesentia; pagina ad primam */
interior b32
annum_scribere (
    ChronicaMontatio* m,
                 s32  annus)
{
    constans character* tituli[II];
    constans character* valores[II];
             character  numerus[XVI];

    sprintf(numerus, "%d", (integer)annus);
    tituli[ZEPHYRUM]   = "annus";
    valores[ZEPHYRUM]  = numerus;
    tituli[I]          = "pagina";
    valores[I]         = "";
    positionem_servare(m, annus);
    redde campos_scribere(m, tituli, valores, II);
}

interior b32
paginam_scribere (
    ChronicaMontatio* m,
                 s32  pagina)
{
    constans character* tituli[I];
    constans character* valores[I];
             character  numerus[XVI];

    sprintf(numerus, "%d", (integer)pagina);
    tituli[ZEPHYRUM]   = "pagina";
    valores[ZEPHYRUM]  = numerus;
    redde campos_scribere(m, tituli, valores, I);
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

/* linea parva: dies, genus, personae, locus (vacua omissa) */
interior chorda
lineam_parvam (
    constans ChronicaEventus* e,
                     Piscina* piscina)
{
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
    per (i = ZEPHYRUM; e->personae && i < xar_numerus(e->personae); i++)
    {
        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(personae, ", ");
        }
        chorda_aedificator_appendere_chorda(personae,
            *(chorda*)xar_obtinere(e->personae, i));
    }
    partem_addere(a, &prima, chorda_aedificator_finire(personae));
    partem_addere(a, &prima, e->locus);
    redde chorda_aedificator_finire(a);
}

interior vacuum
clausulam_addere (
      ChronicaFigurae* f,
    ChordaAedificator* a,
               chorda  textus,
                  b32  obscura)
{
    CodexClausula c;

    si (chorda_aedificator_longitudo(a) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "\n\n");
    }
    c.genus    = CODEX_PARAGRAPHUS;
    c.initium  = (i32)chorda_aedificator_longitudo(a);
    chorda_aedificator_appendere_chorda(a, textus);
    c.finis = (i32)chorda_aedificator_longitudo(a);
    *(CodexClausula*)xar_addere(f->caput.clausulae) = c;
    *(b32*)xar_addere(f->obscurae) = obscura;
}

/* paginae anni dati in area (cache); NIHIL si deficit */
interior ChronicaFigurae*
paginas_parare (
           ChronicaMontatio* m,
    constans ChronicaTabula* t,
                        s32  annus)
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
    si (   f->paginae && f->tabula == t && f->annus == annus
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
    f->obscurae = xar_creare(f->scheda, (i32)magnitudo(b32));
    a = chorda_aedificator_creare(f->scheda, (memoriae_index)MXXIV);
    per (i = ZEPHYRUM; i < xar_numerus(t->res); i++)
    {
        constans ChronicaRes* r = (constans ChronicaRes*)xar_obtinere(
            t->res, i);
        chorda parva;

        si (r->eventus.annus != annus)
        {
            perge;
        }
        clausulam_addere(f, a, r->eventus.descriptio, FALSUM);
        parva = lineam_parvam(&r->eventus, f->scheda);
        si (parva.mensura > ZEPHYRUM)
        {
            clausulam_addere(f, a, parva, VERUM);
        }
    }
    f->caput.textus  = chorda_aedificator_finire(a);
    f->paginae       = lector_paginare(&f->caput, (i32)area.latitudo,
        (i32)area.altitudo, f->scheda);
    f->tabula    = t;
    f->annus     = annus;
    f->latitudo  = area.latitudo;
    f->altitudo  = area.altitudo;
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
    *f      = paginas_parare(m, t, *annus);
    redde *f != NIHIL;
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

    si (!visum_parare(m, &annus, &f))
    {
        redde VERUM;
    }
    k = pagina_currens(m, f) + gradus;
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

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "chronica.annus_prior", "Previous year", "h", VISUS_ANNI,
        ZEPHYRUM, fac_annum_priorem },
    { "chronica.annus_proximus", "Next year", "l", VISUS_ANNI,
        ZEPHYRUM, fac_annum_proximum },
    { "chronica.pagina_proxima", "Next page", "j", VISUS_ANNI,
        ZEPHYRUM, fac_paginam_proximam },
    { "chronica.pagina_prior", "Previous page", "k", VISUS_ANNI,
        ZEPHYRUM, fac_paginam_priorem },
    { "chronica.deorsum", "Next page", "Down", VISUS_ANNI, ZEPHYRUM,
        fac_paginam_proximam },
    { "chronica.sursum", "Previous page", "Up", VISUS_ANNI, ZEPHYRUM,
        fac_paginam_priorem },
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

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
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
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, VISUS_ANNI, ev);
        redde t && praecepta_status(t, VISUS_ANNI, requisita_nulla, m)
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
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(f->caput.clausulae); i++)
    {
        constans CodexClausula* c = (constans CodexClausula*)
            xar_obtinere(f->caput.clausulae, i);

        si (initium >= c->initium && initium <= c->finis)
        {
            redde *(b32*)xar_obtinere(f->obscurae, i);
        }
    }
    redde FALSUM;
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
    si (!visum_parare(m, &annus, &f))
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
    /* pes: "<  Timeline - AD 1274 - 1/2  >" */
    mandata_textus(md, CHRONICA_SINISTRUM, y_pedis,
        chorda_ex_literis("<",
        md->piscina), FONS_PARVUS, figura_color_thematis(COLOR_TEXT));
    mandata_textus(md, CHRONICA_SINISTRUM
        + III * fons_latitudo(FONS_PARVUS),
        y_pedis, titulus, FONS_PARVUS,
        figura_color_thematis(COLOR_TEXT_DIM));
    mandata_textus(md, fines.latitudo - CHRONICA_SINISTRUM
        - fons_latitudo(FONS_PARVUS), y_pedis, chorda_ex_literis(">",
        md->piscina), FONS_PARVUS, figura_color_thematis(COLOR_TEXT));
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
    area               = area_eventuum(m);
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
        positionem_servare(m, valor);
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
        positionem_servare(m, annus);
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
