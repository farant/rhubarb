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
                 CodexCaput  caput;
                        Xar* notae;         /* NotaClausulae */
                        Xar* paginae;       /* LectorPagina */
} ChronicaFigurae;

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

interior b32
visus_fluminis (
    constans ChronicaMontatio* m)
{
    redde !chorda_aequalis_literis(campus(m, "visus"), "annus");
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
    constans character* tituli[II];
    constans character* valores[II];
             character  numerus[XVI];

    sprintf(numerus, "%d", (integer)annus);
    tituli[ZEPHYRUM]   = "annus";
    valores[ZEPHYRUM]  = numerus;
    tituli[I]          = "pagina";
    valores[I]         = "";
    positionem_servare(m, annus, ZEPHYRUM);
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
    si (   f->paginae && f->tabula == t && f->flumen == flumen
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
    a = chorda_aedificator_creare(f->scheda, (memoriae_index)MXXIV);
    ultimus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(t->res); i++)
    {
        constans ChronicaRes* r = (constans ChronicaRes*)xar_obtinere(
            t->res, i);
        chorda parva;
           s32 a_r = r->eventus.annus;

        si (!flumen && a_r != annus)
        {
            perge;
        }
        clausulam_addere(f, a, r->eventus.descriptio, a_r, FALSUM,
            i == ZEPHYRUM || a_r != ultimus);
        ultimus  = a_r;
        parva    = lineam_parvam(&r->eventus, f->scheda);
        si (parva.mensura > ZEPHYRUM)
        {
            clausulam_addere(f, a, parva, a_r, VERUM, FALSUM);
        }
    }
    f->caput.textus  = chorda_aedificator_finire(a);
    f->paginae       = lector_paginare(&f->caput, (i32)area.latitudo,
        (i32)area.altitudo, f->scheda);
    f->tabula    = t;
    f->flumen    = flumen;
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
    *f      = paginas_parare(m, t, *annus, FALSUM);
    redde *f != NIHIL;
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
    constans character* tituli[II];
    constans character* valores[II];
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
    positionem_servare(m, annus, p - pagina_initii(f, annus));
    redde campos_scribere(m, tituli, valores, II);
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

/* v: flumen -> annus (annus summus paginae); annus -> flumen (annus
 * servatus, pagina ubi incipit) */
interior b32
fac_visum_vertere (
    vacuum* ctx)
{
       ChronicaMontatio* m = (ChronicaMontatio*)ctx;
        ChronicaFigurae* f;
     constans character* tituli[III];
     constans character* valores[III];
              character  anni[XVI];
                    s32  p;
                    s32  annus;

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = visus_fluminis(m) ? "annus" : "flumen";
    tituli[I]          = "pagina";
    valores[I]         = "";
    si (visus_fluminis(m) && flumen_parare(m, &f, &p))
    {
        annus = annus_paginae(f, p);
    }
    alioquin si (visus_fluminis(m) || !visum_parare(m, &annus, &f))
    {
        redde campos_scribere(m, tituli, valores, II);
    }
    sprintf(anni, "%d", (integer)annus);
    tituli[II]   = "annus";
    valores[II]  = anni;
    positionem_servare(m, annus, ZEPHYRUM);
    redde campos_scribere(m, tituli, valores, III);
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "chronica.annus_prior", "Previous year", "h",
        VISUS_ANNI | VISUS_FLUMINIS, ZEPHYRUM, fac_annum_priorem },
    { "chronica.annus_proximus", "Next year", "l",
        VISUS_ANNI | VISUS_FLUMINIS, ZEPHYRUM, fac_annum_proximum },
    { "chronica.pagina_proxima", "Next page", "j",
        VISUS_ANNI | VISUS_FLUMINIS, ZEPHYRUM, fac_paginam_proximam },
    { "chronica.pagina_prior", "Previous page", "k",
        VISUS_ANNI | VISUS_FLUMINIS, ZEPHYRUM, fac_paginam_priorem },
    { "chronica.deorsum", "Next page", "Down",
        VISUS_ANNI | VISUS_FLUMINIS, ZEPHYRUM, fac_paginam_proximam },
    { "chronica.sursum", "Previous page", "Up",
        VISUS_ANNI | VISUS_FLUMINIS, ZEPHYRUM, fac_paginam_priorem },
    { "chronica.vertere", "Year / continuous", "v",
        VISUS_ANNI | VISUS_FLUMINIS, ZEPHYRUM, fac_visum_vertere },
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
        visus  = visus_fluminis(m) ? VISUS_FLUMINIS : VISUS_ANNI;
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
    si (visus_fluminis(m) && flumen_parare(m, &f, &annus))
    {
        flumen_pingere(m, md, f, annus);
        pedem_pingere(md, fines, titulus, y_pedis);
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
    /* pes: "<  Timeline - AD 1274 - 1/2  >" */
    pedem_pingere(md, fines, titulus, y_pedis);
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
