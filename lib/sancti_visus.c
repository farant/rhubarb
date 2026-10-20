/* sancti_visus.c - bibliotheca sanctorum in latere vici
 * (include/sancti_visus.h). Forma centri admonitionum: arbor
 * dispositionis in cellulis (LITTERA x LINEA), figura radicis omnia
 * pingit, praecepta per membrum__praecepta, paginae indicis per
 * membrum__album, quaestio per membrum__prompta, textus sancti per
 * paginarium (involutio et paginae). */

#include "sancti_visus.h"
#include "membrum__album.h"
#include "membrum__praecepta.h"
#include "membrum__prompta.h"
#include "paginarium.h"
#include "chorda_aedificator.h"
#include "dispositio.h"
#include "fons.h"
#include "thema.h"
#include "motus.h"
#include "semita.h"
#include "organum_montatio.h"

#include <stdio.h>
#include <string.h>

#define SANCTI_LATITUDO   CCC      /* superficies absens */
#define SANCTI_ALTITUDO   CCLVI
#define SANCTI_LITTERA    VI
#define SANCTI_LINEA      VIII
#define SANCTI_TITULI     III      /* lineae tituli */
#define SANCTI_CAPUT      II       /* lineae capitis (index, pes) */
#define SANCTI_ORDO       III      /* lineae ordinis: titulus, vita */
#define SANCTI_CAPUT_VISUS VI      /* cellulae capitis sancti (lineae
                                    * IV spatio SANCTI_SPATIUM) */
#define SANCTI_SPATIUM    X        /* pixela per lineam textus sancti */

#define VISUS_INDICIS     I
#define VISUS_SANCTI      II
#define REQUIRIT_ELECTUM  I

hic_manens constans character actio_sanctorum[] = "sancti.agere";

hic_manens constans character* constans menses_breves[XII] = {
    "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep",
    "Oct", "Nov", "Dec"
};

hic_manens constans character* constans menses[XII] = {
    "January", "February", "March", "April", "May", "June", "July",
    "August", "September", "October", "November", "December"
};

/* figura: contextus privatus; organum manifesti (semitae R2) */
nomen structura {
    SanctorumMontatio* m;
     constans Organum* organum;
              Piscina* scheda;
                  s32  latitudo_montationis;
                  s32  altitudo_montationis;
} SanctorumFigurae;


/* ==================================================
 * Status (insula)
 * ================================================== */

interior chorda
_campus (
    constans SanctorumMontatio* m,
            constans character* titulus)
{
    chorda* a;
    chorda  vacua;

    a = insula_ramus_attributum(&m->ramus, titulus);
    si (a)
    {
        redde *a;
    }
    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    redde vacua;
}

interior s32
_campus_s32 (
    constans SanctorumMontatio* m,
            constans character* titulus)
{
    chorda a;
       s32 v;

    a = _campus(m, titulus);
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &v)
        && v >= ZEPHYRUM)
        ? v : ZEPHYRUM;
}

nomen structura {
    constans character* constans* tituli;
    constans character* constans* valores;
                             i32 numerus;
} GrexCamporum;

interior vacuum
_campos_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    GrexCamporum* grex;
             i32  i;

    grex = (GrexCamporum*)ctx;
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

/* campi actu uno, scriptore bibliothecae ("" aut NIHIL tollit) */
interior b32
_campos_scribere (
             SanctorumMontatio* m,
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
        actio_sanctorum, m->piscina));
    ok = mutare_ramum(&m->ramus, _campos_ponere, &grex);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

interior b32
_campum_scribere (
      SanctorumMontatio* m,
     constans character* titulus,
     constans character* valor)
{
    constans character* t[I];
    constans character* v[I];

    t[ZEPHYRUM] = titulus;
    v[ZEPHYRUM] = valor;
    redde _campos_scribere(m, t, v, I);
}

interior s32
_superficies (
     constans SanctorumMontatio* m,
                            b32  latitudo)
{
    constans SanctorumFigurae* f;
                          s32  v;
                       chorda  a;
                          s32  n;

    f = (constans SanctorumFigurae*)m->figurae_ctx;
    v = !f ? ZEPHYRUM : latitudo ? f->latitudo_montationis
                                 : f->altitudo_montationis;
    si (v <= ZEPHYRUM)
    {
        v = latitudo ? SANCTI_LATITUDO : SANCTI_ALTITUDO;
    }
    a = _campus(m, latitudo ? "superficies_latitudo"
        : "superficies_altitudo");
    redde (a.mensura > ZEPHYRUM && chorda_ut_s32(a, &n) && n > ZEPHYRUM)
        ? n : v;
}

interior b32
_visus_sancti (
    constans SanctorumMontatio* m)
{
    redde chorda_aequalis_literis(_campus(m, "visus"), "sanctus");
}


/* ==================================================
 * Ordo indicis
 * ================================================== */

/* clavis calendarii: mensis * C + dies festi primi; sine festo
 * ultimi */
interior s32
_clavis_calendarii (
    constans Sanctus* s)
{
    constans SanctiFestum* f;

    f = sancti_festum_primum(s);
    redde f ? f->mensis * C + f->dies : M * X;
}

/* clavis aetatis: obitus, deinde ortus, deinde saeculum (medium);
 * ignoti ultimi */
interior s32
_clavis_aetatis (
    constans Sanctus* s)
{
    si (s->obitus.notus && s->obitus.annus != ZEPHYRUM)
    {
        redde s->obitus.annus;
    }
    si (s->ortus.notus && s->ortus.annus != ZEPHYRUM)
    {
        redde s->ortus.annus;
    }
    si (s->obitus.notus && s->obitus.saeculum != ZEPHYRUM)
    {
        redde s->obitus.saeculum * C - L;
    }
    si (s->ortus.notus && s->ortus.saeculum != ZEPHYRUM)
    {
        redde s->ortus.saeculum * C - L;
    }
    redde M * C;
}

interior s32
_titulos_comparare (
    constans Sanctus* a,
    constans Sanctus* b)
{
    s32 c;

    c = chorda_comparare(a->titulus, b->titulus);
    redde c != ZEPHYRUM ? c : chorda_comparare(a->id, b->id);
}

interior s32
_comparare_calendarium (
    constans vacuum* x,
    constans vacuum* y)
{
      constans Sanctus* a = *(constans Sanctus* constans*)x;
      constans Sanctus* b = *(constans Sanctus* constans*)y;
                   s32  clavis_prima;
                   s32  clavis_secunda;

    clavis_prima    = _clavis_calendarii(a);
    clavis_secunda  = _clavis_calendarii(b);
    redde clavis_prima != clavis_secunda ? (clavis_prima
        < clavis_secunda ? -I : I) : _titulos_comparare(a, b);
}

interior s32
_comparare_nomen (
    constans vacuum* x,
    constans vacuum* y)
{
    redde _titulos_comparare(*(constans Sanctus* constans*)x,
        *(constans Sanctus* constans*)y);
}

interior s32
_comparare_aetas (
    constans vacuum* x,
    constans vacuum* y)
{
      constans Sanctus* a = *(constans Sanctus* constans*)x;
      constans Sanctus* b = *(constans Sanctus* constans*)y;
                   s32  clavis_prima;
                   s32  clavis_secunda;

    clavis_prima    = _clavis_aetatis(a);
    clavis_secunda  = _clavis_aetatis(b);
    redde clavis_prima != clavis_secunda ? (clavis_prima
        < clavis_secunda ? -I : I) : _titulos_comparare(a, b);
}

/* quaestio activa: littera aliqua praeter spatia */
interior b32
_activa (
    chorda q)
{
    i32 i;

    per (i = ZEPHYRUM; i < q.mensura; i++)
    {
        si (q.datum[i] != ' ')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* quaestio currens: prompta aperta = textus vivus (dum scribitur),
 * aliter quaestio commissa; textus in exitus[PROMPTA_MAXIMA] */
interior chorda
_quaestio (
    constans SanctorumMontatio* m,
                     character* exitus)
{
    chorda q;

    si (prompta_aperta(&m->ramus, NIHIL))
    {
        q.datum    = (i8*)exitus;
        q.mensura  = prompta_textus(&m->ramus, exitus);
        redde q;
    }
    redde _campus(m, "quaestio");
}

/* ordo currens: 0 calendarium, 1 nomen, 2 aetas, 3 congruentia (solum
 * cum quaestione activa; sine ea calendarium) */
interior s32
_ordo (
    constans SanctorumMontatio* m)
{
       chorda o;
    character textus[PROMPTA_MAXIMA];

    o = _campus(m, "ordo");
    si (chorda_aequalis_literis(o, "nomen"))
    {
        redde I;
    }
    si (chorda_aequalis_literis(o, "aetas"))
    {
        redde II;
    }
    si (   chorda_aequalis_literis(o, "congruentia")
        && _activa(_quaestio(m, textus)))
    {
        redde III;
    }
    redde ZEPHYRUM;
}

/* sancti indicis (quaestione currenti filtrati, fzf) ordine currenti
 * - Xar de (constans Sanctus*); congruentia: ordo sancti_congruentes */
interior Xar*
_sancti (
    constans SanctorumMontatio* m,
                       Piscina* p)
{
       chorda  q;
          Xar* x;
          i32  i;
    character  textus[PROMPTA_MAXIMA];

    q = _quaestio(m, textus);
    si (_activa(q))
    {
        x = sancti_congruentes(m->corpus, q, p);
        si (x && _ordo(m) == III)
        {
            redde x;
        }
    }
    alioquin
    {
        x = xar_creare(p, (i32)magnitudo(Sanctus*));
        per (i = ZEPHYRUM; x && i < sancti_numerus(m->corpus); i++)
        {
            *(constans Sanctus**)xar_addere(x) = sancti_ad_indicem(
                m->corpus, i);
        }
    }
    si (!x)
    {
        redde NIHIL;
    }
    xar_ordinare(x, _ordo(m) == I ? _comparare_nomen : _ordo(m) == II
        ? _comparare_aetas : _comparare_calendarium);
    redde x;
}

interior constans Sanctus*
_sanctus (
    Xar* x,
    s32  i)
{
    si (!x || i < ZEPHYRUM || i >= (s32)xar_numerus(x))
    {
        redde NIHIL;
    }
    redde *(constans Sanctus**)xar_obtinere(x, (i32)i);
}

interior s32
_index_id (
       Xar* x,
    chorda  id)
{
    i32 i;

    per (i = ZEPHYRUM; id.mensura > ZEPHYRUM && x
         && i < xar_numerus(x); i++)
    {
        si (chorda_aequalis(_sanctus(x, (s32)i)->id, id))
        {
            redde (s32)i;
        }
    }
    redde -I;
}

/* electus praestitutus: calendarium = primus a hodie (ultimo anni
 * transito, primus); ceteri ordines = primus */
interior s32
_electus_praestitutus (
    constans SanctorumMontatio* m,
                           Xar* x)
{
    s32 mensis;
    s32 dies;
    s32 hodie;
    i32 i;

    si (!x || xar_numerus(x) == ZEPHYRUM)
    {
        redde -I;
    }
    si (_ordo(m) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    mensis  = I;
    dies    = I;
    si (m->hodie)
    {
        (vacuum)m->hodie(m->hodie_ctx, &mensis, &dies);
    }
    hodie = mensis * C + dies;
    per (i = ZEPHYRUM; i < xar_numerus(x); i++)
    {
        si (_clavis_calendarii(_sanctus(x, (s32)i)) >= hodie)
        {
            redde (s32)i;
        }
    }
    redde ZEPHYRUM;
}

interior s32
_electus (
    constans SanctorumMontatio* m,
                           Xar* x)
{
    s32 i;

    i = _index_id(x, _campus(m, "electus"));
    redde i >= ZEPHYRUM ? i : _electus_praestitutus(m, x);
}

/* capacitas in cellulis: altitudo minus margines, titulus, caput */
interior s32
_capacitas (
    constans SanctorumMontatio* m)
{
    redde _superficies(m, FALSUM) / SANCTI_LINEA - II - SANCTI_TITULI
        - SANCTI_CAPUT;
}

/* album: res = index + 1, id "r<res>" */
interior Album*
_album (
    constans SanctorumMontatio* m,
                           Xar* x,
                       Piscina* p)
{
        Album* l;
          i32  i;
    character  id[XVI];

    l = album_creare(p, _capacitas(m));
    per (i = ZEPHYRUM; l && x && i < xar_numerus(x); i++)
    {
        sprintf(id, "r%d", (integer)(i + I));
        (vacuum)album_addere(l, id, ALBUM_RES, SANCTI_ORDO,
            (s32)(i + I));
    }
    redde l;
}

interior s32
_pagina (
    Album* l,
      s32  electus)
{
    s32 k;

    k = electus >= ZEPHYRUM ? album_pagina_rei(l, electus + I, ZEPHYRUM)
        : ZEPHYRUM;
    redde k < ZEPHYRUM ? ZEPHYRUM : k;
}

/* id ordinis "r<n>" -> n; 0 si non ordo */
interior s32
_id_ordinis (
    chorda id)
{
    s32 n;

    si (id.mensura < II || id.datum[ZEPHYRUM] != 'r')
    {
        redde ZEPHYRUM;
    }
    id.datum++;
    id.mensura--;
    redde chorda_ut_s32(id, &n) && n > ZEPHYRUM ? n : ZEPHYRUM;
}


/* ==================================================
 * Textus
 * ================================================== */

/* praecidere ad columnas (runae, non octeti) */
interior chorda
_praecidere (
    chorda textus,
       s32 columnae)
{
    i32 i;
    s32 runae;

    runae = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si ((textus.datum[i] & 0xC0) != 0x80)
        {
            si (runae == columnae)
            {
                textus.mensura = i;
                redde textus;
            }
            runae++;
        }
    }
    redde textus;
}

interior vacuum
_textum_pingere (
       Mandata* md,
           s32  x,
           s32  y,
        chorda  textus,
           i32  fons,
    ColorThema  color)
{
    mandata_textus(md, x, y, chorda_transcribere(textus, md->piscina),
        fons, figura_color_thematis(color));
}

/* munera iuncta ", " */
interior vacuum
_munera_appendere (
    ChordaAedificator* a,
     constans Sanctus* s)
{
    i32 j;

    per (j = ZEPHYRUM; j < xar_numerus(s->munera); j++)
    {
        si (j > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, ", ");
        }
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(
            s->munera, j));
    }
}

/* linea secunda ordinis: aetas · munera */
interior chorda
_linea_vitae (
    constans Sanctus* s,
             Piscina* p)
{
    ChordaAedificator* a;
               chorda  aetas;

    a      = chorda_aedificator_creare(p, LXIV);
    aetas  = sancti_aetas_textus(s, p);
    chorda_aedificator_appendere_chorda(a, aetas);
    si (aetas.mensura > ZEPHYRUM && xar_numerus(s->munera) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " \302\267 ");
    }
    _munera_appendere(a, s);
    redde chorda_aedificator_finire(a);
}

/* dies incertus: "7 March 1274", "March 1274", "c. 1225", "4th
 * century"; vacua si nihil */
interior vacuum
_diem_appendere (
      ChordaAedificator* a,
    constans SanctiDies* d)
{
    character numerus[XXXII];

    si (d->circa)
    {
        chorda_aedificator_appendere_literis(a, "c. ");
    }
    si (d->annus == ZEPHYRUM)
    {
        si (d->saeculum != ZEPHYRUM)
        {
            s32 n = d->saeculum < ZEPHYRUM ? -d->saeculum : d->saeculum;

            sprintf(numerus, "%d%s century%s", (integer)n,
                (n % C >= XI
                    && n % C <= XIII) ? "th" : n % X == I ? "st"
                : n % X == II ? "nd" : n % X == III ? "rd" : "th",
                d->saeculum < ZEPHYRUM ? " BC" : "");
            chorda_aedificator_appendere_literis(a, numerus);
        }
        redde;
    }
    si (d->dies > ZEPHYRUM && d->mensis >= I && d->mensis <= XII)
    {
        sprintf(numerus, "%d %s ", (integer)d->dies, menses[d->mensis
            - I]);
        chorda_aedificator_appendere_literis(a, numerus);
    }
    alioquin si (d->mensis >= I && d->mensis <= XII)
    {
        sprintf(numerus, "%s ", menses[d->mensis - I]);
        chorda_aedificator_appendere_literis(a, numerus);
    }
    si (d->annus < ZEPHYRUM)
    {
        sprintf(numerus, "%d BC", (integer)-d->annus);
    }
    alioquin
    {
        sprintf(numerus, "%d", (integer)d->annus);
    }
    chorda_aedificator_appendere_literis(a, numerus);
}

interior b32
_dies_datus (
    constans SanctiDies* d)
{
    redde d->notus && (d->annus != ZEPHYRUM || d->saeculum != ZEPHYRUM);
}

/* "Born c. 1225 · died 7 March 1274, Fossanova"; "Martyred at Rome" */
interior chorda
_linea_ortus_obitus (
    constans Sanctus* s,
             Piscina* p)
{
    ChordaAedificator* a;
                  b32  ortus;

    a      = chorda_aedificator_creare(p, CXXVIII);
    ortus  = _dies_datus(&s->ortus);
    si (ortus)
    {
        chorda_aedificator_appendere_literis(a, "Born ");
        _diem_appendere(a, &s->ortus);
        si (s->ortus.locus.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, ", ");
            chorda_aedificator_appendere_chorda(a, s->ortus.locus);
        }
    }
    si (   s->obitus.notus
        && (_dies_datus(&s->obitus)
        || s->obitus.locus.mensura > ZEPHYRUM))
    {
        si (ortus)
        {
            chorda_aedificator_appendere_literis(a, " \302\267 ");
        }
        chorda_aedificator_appendere_literis(a, s->martyr
            ? (ortus ? "martyred " : "Martyred ")
            : (ortus ? "died " : "Died "));
        si (_dies_datus(&s->obitus))
        {
            _diem_appendere(a, &s->obitus);
            si (s->obitus.locus.mensura > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(a, ", ");
                chorda_aedificator_appendere_chorda(a, s->obitus.locus);
            }
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(a, "at ");
            chorda_aedificator_appendere_chorda(a, s->obitus.locus);
        }
    }
    redde chorda_aedificator_finire(a);
}

/* "Canonized 18 July 1323 by John XXII · priest" */
interior chorda
_linea_canonizationis (
    constans Sanctus* s,
             Piscina* p)
{
    ChordaAedificator* a;
                  b32  aliquid;

    a        = chorda_aedificator_creare(p, CXXVIII);
    aliquid  = FALSUM;
    si (   s->canonizatio == SANCTI_CANONIZATIO_FORMALIS
        || s->canonizatio == SANCTI_CANONIZATIO_AEQUIPOLLENS)
    {
        chorda_aedificator_appendere_literis(a,
            s->canonizatio == SANCTI_CANONIZATIO_FORMALIS ? "Canonized"
            : "Equipollent canonization");
        si (s->canonizatio_dies.annus != ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, " ");
            _diem_appendere(a, &s->canonizatio_dies);
        }
        si (s->canonizatio_papa.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, " by ");
            chorda_aedificator_appendere_chorda(a, s->canonizatio_papa);
        }
        aliquid = VERUM;
    }
    si (aliquid && xar_numerus(s->munera) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " \302\267 ");
    }
    _munera_appendere(a, s);
    redde chorda_aedificator_finire(a);
}

interior constans character*
_gradus_textus (
    SanctiGradus g)
{
    commutatio (g)
    {
        casus SANCTI_GRADUS_SOLLEMNITAS:
            redde "solemnity";
        casus SANCTI_GRADUS_FESTUM:
            redde "feast";
        casus SANCTI_GRADUS_MEMORIA:
            redde "memorial";
        casus SANCTI_GRADUS_MEMORIA_AD_LIBITUM:
            redde "optional memorial";
        casus SANCTI_GRADUS_COMMEMORATIO:
            redde "commemoration";
        ordinarius:
            redde "";
    }
}

/* "Feast: 28 January (memorial)"; plura "Feasts: ..."; sine generali
 * aut usa: "Martyrology: 21 January" */
interior chorda
_linea_festorum (
    constans Sanctus* s,
             Piscina* p)
{
    ChordaAedificator* a;
                  i32  j;
                  i32  n;
            character  textus[XCVI];

    a = chorda_aedificator_creare(p, CXXVIII);
    n = ZEPHYRUM;
    per (j = ZEPHYRUM; j < xar_numerus(s->festa); j++)
    {
        constans SanctiFestum* f = (constans SanctiFestum*)xar_obtinere(
            s->festa, j);

        si (   f->calendarium == SANCTI_CALENDARIUM_GENERALE
            || f->calendarium == SANCTI_CALENDARIUM_USA)
        {
            n++;
        }
    }
    per (j = ZEPHYRUM; j < xar_numerus(s->festa); j++)
    {
        constans SanctiFestum* f = (constans SanctiFestum*)xar_obtinere(
            s->festa, j);
           constans character* g;

        si (   (n > ZEPHYRUM
                && f->calendarium != SANCTI_CALENDARIUM_GENERALE
                && f->calendarium != SANCTI_CALENDARIUM_USA)
            || (n == ZEPHYRUM
                && f->calendarium != SANCTI_CALENDARIUM_MARTYROLOGIUM)
            || f->mensis < I || f->mensis > XII)
        {
            perge;
        }
        g = _gradus_textus(f->gradus);
        sprintf(textus, "%s%d %s%s%s%s%s",
            chorda_aedificator_longitudo(a) == ZEPHYRUM
                ? (n == ZEPHYRUM ? "Martyrology: " : n > I ? "Feasts: "
                : "Feast: ")
                : " \302\267 ", (integer)f->dies, menses[f->mensis - I],
            (*g
                || f->calendarium
                    == SANCTI_CALENDARIUM_USA) ? " (" : "",
            g,
            f->calendarium
                == SANCTI_CALENDARIUM_USA ? (*g ? ", US" : "US")
            : "", (*g || f->calendarium == SANCTI_CALENDARIUM_USA) ? ")"
            : "");
        chorda_aedificator_appendere_literis(a, textus);
        si (n == ZEPHYRUM)
        {
            frange;
        }
    }
    redde chorda_aedificator_finire(a);
}

interior constans character*
_opus_textus (
    chorda opus)
{
    si (chorda_aequalis_literis(opus, "mr1916"))
    {
        redde "Roman Martyrology (1916)";
    }
    si (chorda_aequalis_literis(opus, "butler1812"))
    {
        redde "Butler, Lives of the Saints (1812)";
    }
    si (chorda_aequalis_literis(opus, "ce1913"))
    {
        redde "Catholic Encyclopedia (1913)";
    }
    si (chorda_aequalis_literis(opus, "sedes"))
    {
        redde "Holy See";
    }
    si (chorda_aequalis_literis(opus, "usccb2026"))
    {
        redde "USCCB calendar (2026)";
    }
    redde "source";
}

/* corpus textus sancti: paragraphi, socii, fontes - "\n\n" separati */
interior chorda
_corpus_sancti (
    constans Sanctus* s,
             Piscina* p)
{
    ChordaAedificator* a;
                  i32  j;

    a = chorda_aedificator_creare(p, IV * M);
    per (j = ZEPHYRUM; j < xar_numerus(s->paragraphi); j++)
    {
        chorda_aedificator_appendere_chorda(a, *(chorda*)xar_obtinere(
            s->paragraphi, j));
        chorda_aedificator_appendere_literis(a, "\n\n");
    }
    si (xar_numerus(s->socii) > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "Companions: ");
        per (j = ZEPHYRUM; j < xar_numerus(s->socii); j++)
        {
            si (j > ZEPHYRUM)
            {
                chorda_aedificator_appendere_literis(a, ", ");
            }
            chorda_aedificator_appendere_chorda(a,
                ((constans SanctiSocius*)xar_obtinere(s->socii,
                j))->textus);
        }
        chorda_aedificator_appendere_literis(a, ".\n\n");
    }
    chorda_aedificator_appendere_literis(a, "Sources: ");
    per (j = ZEPHYRUM; j < xar_numerus(s->fontes); j++)
    {
        constans SanctiFons* f = (constans SanctiFons*)xar_obtinere(
            s->fontes, j);

        si (j > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, " \302\267 ");
        }
        chorda_aedificator_appendere_literis(a, _opus_textus(f->opus));
        si (f->locus.mensura > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(a, ", ");
            chorda_aedificator_appendere_chorda(a, f->locus);
        }
    }
    redde chorda_aedificator_finire(a);
}

/* lineae textus in visu sancti (regio inter caput et pedem) */
interior s32
_lineae_textus (
    constans SanctorumMontatio* m)
{
    s32 n;

    n = (_superficies(m, FALSUM) / SANCTI_LINEA - II - SANCTI_TITULI
        - SANCTI_CAPUT_VISUS - SANCTI_CAPUT) * SANCTI_LINEA
        / SANCTI_SPATIUM;
    redde n < I ? I : n;
}

interior PaginariumResultus
_paginae_sancti (
    constans SanctorumMontatio* m,
              constans Sanctus* s,
                        chorda* corpus,
                       Piscina* p)
{
    PaginariumConfig config;

    *corpus  = _corpus_sancti(s, p);
    config   = paginarium_config_defectus();
    config.latitudo   = (i32)(_superficies(m, VERUM) / SANCTI_LITTERA
        - IV);
    config.altitudo   = (i32)_lineae_textus(m);
    redde paginarium_paginare(*corpus, config, p);
}

interior constans Sanctus*
_apertus (
    constans SanctorumMontatio* m)
{
    redde sancti_invenire(m->corpus, _campus(m, "sanctus_apertus"));
}


/* ==================================================
 * Praecepta
 * ================================================== */

nomen structura {
    SanctorumMontatio* m;
              Piscina* p;
                  Xar* sancti;
                Album* l;
                  s32  electus;
} ContextusPraecepti;

interior b32
_requisita (
        i32  requisita,
     vacuum* ctx)
{
    ContextusPraecepti* cp;

    cp = (ContextusPraecepti*)ctx;
    redde !(requisita & REQUIRIT_ELECTUM) || cp->electus >= ZEPHYRUM;
}

interior b32
_electum_scribere (
    ContextusPraecepti* cp,
                   s32  i)
{
      constans Sanctus* s;
                   s32  k;
             character  pagina[XVI];
    constans character* tituli[II];
    constans character* valores[II];

    s = _sanctus(cp->sancti, i);
    si (!s)
    {
        redde VERUM;
    }
    k = _pagina(cp->l, i);
    sprintf(pagina, "%d", (integer)k);
    tituli[ZEPHYRUM]   = "electus";
    valores[ZEPHYRUM]  = chorda_ut_cstr(s->id, cp->p);
    tituli[I]          = "pagina";
    valores[I]         = k > ZEPHYRUM ? pagina : "";
    redde _campos_scribere(cp->m, tituli, valores, II);
}

interior vacuum
_contextum_parare (
     SanctorumMontatio* m,
               Piscina* p,
    ContextusPraecepti* cp)
{
    cp->m        = m;
    cp->p        = p;
    cp->sancti   = _sancti(m, p);
    cp->l        = _album(m, cp->sancti, p);
    cp->electus  = _electus(m, cp->sancti);
}

/* electus praestitutus SCRIBITUR (t, ordo novus, quaestio): campus
 * explicitus, non solum computatus */
interior b32
_praestitutum_scribere (
    SanctorumMontatio* m,
              Piscina* p)
{
    ContextusPraecepti cp;

    _contextum_parare(m, p, &cp);
    redde _electum_scribere(&cp, _electus_praestitutus(m, cp.sancti));
}

interior b32
fac_proximum (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _electum_scribere(cp, cp->electus + I);
}

interior b32
fac_priorem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _electum_scribere(cp, cp->electus - I);
}

interior b32
_paginam_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 k;

    k = _pagina(cp->l, cp->electus) + gradus;
    si (k < ZEPHYRUM || k >= album_paginae(cp->l))
    {
        redde VERUM;
    }
    redde _electum_scribere(cp, album_res_prima(cp->l, k) - I);
}

interior b32
fac_paginam_priorem (
    vacuum* ctx)
{
    redde _paginam_vertere((ContextusPraecepti*)ctx, -I);
}

interior b32
fac_paginam_proximam (
    vacuum* ctx)
{
    redde _paginam_vertere((ContextusPraecepti*)ctx, I);
}

/* Enter: sanctum electum aperire */
interior b32
_aperire (
    ContextusPraecepti* cp,
                   s32  i)
{
      constans Sanctus* s;
    constans character* tituli[IV];
    constans character* valores[IV];

    s = _sanctus(cp->sancti, i);
    si (!s)
    {
        redde VERUM;
    }
    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "sanctus";
    tituli[I]          = "sanctus_apertus";
    valores[I]         = chorda_ut_cstr(s->id, cp->p);
    tituli[II]         = "electus";
    valores[II]        = valores[I];
    tituli[III]        = "pagina_textus";
    valores[III]       = "";
    redde _campos_scribere(cp->m, tituli, valores, IV);
}

interior b32
fac_aperire (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _aperire(cp, cp->electus);
}

/* /: quaestio (prompta, quaestione currenti praeimpleta); ordo
 * congruentiae (index scribendo filtratur, optimum primum) */
interior b32
fac_quaestionem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[III];
    constans character* valores[III];

    tituli[ZEPHYRUM]   = "ordo";
    valores[ZEPHYRUM]  = "congruentia";
    tituli[I]          = "electus";
    valores[I]         = "";
    tituli[II]         = "pagina";
    valores[II]        = "";
    redde _campos_scribere(cp->m, tituli, valores, III)
        && prompta_aperire(&cp->m->ramus, actio_sanctorum, "quaestio",
               _campus(cp->m, "quaestio"), cp->p);
}

/* t: ad hodie (electus praestitutus ordinis currentis) */
interior b32
fac_hodie (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;

    redde _praestitutum_scribere(cp->m, cp->p);
}

/* s: ordo calendarium -> nomen -> aetas -> calendarium; cum quaestione
 * activa aetas -> congruentia -> calendarium */
interior b32
fac_ordinem (
    vacuum* ctx)
{
      ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
      constans character* tituli[III];
      constans character* valores[III];
                     s32  o;
               character  textus[PROMPTA_MAXIMA];

    o                 = _ordo(cp->m);
    tituli[ZEPHYRUM]  = "ordo";
    valores[ZEPHYRUM]  = o == ZEPHYRUM ? "nomen" : o == I ? "aetas"
        : (o == II && _activa(_quaestio(cp->m, textus))) ? "congruentia"
        : "calendarium";
    tituli[I]    = "electus";
    valores[I]   = "";
    tituli[II]   = "pagina";
    valores[II]  = "";
    redde _campos_scribere(cp->m, tituli, valores, III)
        && _praestitutum_scribere(cp->m, cp->p);
}

/* Esc in indice: quaestio primum (ordo calendarii redit); deinde non
 * consumitur (vicus latus tollit) */
interior b32
fac_redire_indice (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[IV];
    constans character* valores[IV];

    si (_campus(cp->m, "quaestio").mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    tituli[ZEPHYRUM]   = "quaestio";
    valores[ZEPHYRUM]  = "";
    tituli[I]          = "electus";
    valores[I]         = "";
    tituli[II]         = "pagina";
    valores[II]        = "";
    tituli[III]        = "ordo";
    valores[III]       = "calendarium";
    redde _campos_scribere(cp->m, tituli, valores, IV)
        && _praestitutum_scribere(cp->m, cp->p);
}

/* visus sancti: [ ] paginae textus */
interior b32
_paginam_textus_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
       constans Sanctus* s;
                 chorda  corpus;
     PaginariumResultus  r;
                    s32  k;
              character  numerus[XVI];

    s = _apertus(cp->m);
    si (!s)
    {
        redde VERUM;
    }
    r = _paginae_sancti(cp->m, s, &corpus, cp->p);
    k = _campus_s32(cp->m, "pagina_textus") + gradus;
    si (k < ZEPHYRUM || k >= (s32)r.numerus_paginarum)
    {
        redde VERUM;
    }
    sprintf(numerus, "%d", (integer)k);
    redde _campum_scribere(cp->m, "pagina_textus", k > ZEPHYRUM
        ? numerus : "");
}

interior b32
fac_textum_priorem (
    vacuum* ctx)
{
    redde _paginam_textus_vertere((ContextusPraecepti*)ctx, -I);
}

interior b32
fac_textum_proximum (
    vacuum* ctx)
{
    redde _paginam_textus_vertere((ContextusPraecepti*)ctx, I);
}

/* visus sancti: j k - sanctus proximus / prior ordine indicis */
interior b32
_sanctum_vertere (
    ContextusPraecepti* cp,
                   s32  gradus)
{
    s32 i;

    i = _index_id(cp->sancti, _campus(cp->m, "sanctus_apertus"));
    si (i < ZEPHYRUM || !_sanctus(cp->sancti, i + gradus))
    {
        redde VERUM;
    }
    redde _aperire(cp, i + gradus);
}

interior b32
fac_sanctum_proximum (
    vacuum* ctx)
{
    redde _sanctum_vertere((ContextusPraecepti*)ctx, I);
}

interior b32
fac_sanctum_priorem (
    vacuum* ctx)
{
    redde _sanctum_vertere((ContextusPraecepti*)ctx, -I);
}

/* Esc in visu sancti: ad indicem (electus = apertus); sanctus per
 * argumentum apertus (origo) - non consumitur: vicus latus tollit,
 * prius (calendarium) redit */
interior b32
fac_ad_indicem (
    vacuum* ctx)
{
    ContextusPraecepti* cp = (ContextusPraecepti*)ctx;
    constans character* tituli[IV];
    constans character* valores[IV];

    si (chorda_aequalis_literis(_campus(cp->m, "origo"), "argumentum"))
    {
        redde FALSUM;
    }

    tituli[ZEPHYRUM]   = "visus";
    valores[ZEPHYRUM]  = "index";
    tituli[I]          = "electus";
    valores[I]         = chorda_ut_cstr(_campus(cp->m,
        "sanctus_apertus"),
        cp->p);
    tituli[II]   = "pagina_textus";
    valores[II]  = "";
    /* semitae R2: semita indicis sine sancto (electus eum servat) */
    tituli[III]   = "sanctus_apertus";
    valores[III]  = "";
    redde _campos_scribere(cp->m, tituli, valores, IV);
}

hic_manens constans Praeceptum TABULA_PRAECEPTORUM[] = {
    { "sancti.proximus", "Next saint", "j", VISUS_INDICIS, ZEPHYRUM,
        fac_proximum },
    { "sancti.prior", "Previous saint", "k", VISUS_INDICIS, ZEPHYRUM,
        fac_priorem },
    { "sancti.pagina_prior", "Previous page", "[", VISUS_INDICIS,
        ZEPHYRUM, fac_paginam_priorem },
    { "sancti.pagina_proxima", "Next page", "]", VISUS_INDICIS,
        ZEPHYRUM, fac_paginam_proximam },
    { "sancti.aperire", "Read", "Enter", VISUS_INDICIS,
        REQUIRIT_ELECTUM, fac_aperire },
    { "sancti.quaestio", "Search", "/", VISUS_INDICIS, ZEPHYRUM,
        fac_quaestionem },
    { "sancti.hodie", "Today", "t", VISUS_INDICIS, ZEPHYRUM,
        fac_hodie },
    { "sancti.ordo", "Sort", "s", VISUS_INDICIS, ZEPHYRUM,
        fac_ordinem },
    { "sancti.redire", "Back", "Esc", VISUS_INDICIS, ZEPHYRUM,
        fac_redire_indice },
    { "sancti.textus_prior", "Previous page", "[", VISUS_SANCTI,
        ZEPHYRUM, fac_textum_priorem },
    { "sancti.textus_proximus", "Next page", "]", VISUS_SANCTI,
        ZEPHYRUM, fac_textum_proximum },
    { "sancti.sanctus_proximus", "Next saint", "j", VISUS_SANCTI,
        ZEPHYRUM, fac_sanctum_proximum },
    { "sancti.sanctus_prior", "Previous saint", "k", VISUS_SANCTI,
        ZEPHYRUM, fac_sanctum_priorem },
    { "sancti.ad_indicem", "Back to list", "Esc", VISUS_SANCTI,
        ZEPHYRUM, fac_ad_indicem },
    { NIHIL, NIHIL, NIHIL, ZEPHYRUM, ZEPHYRUM, NIHIL }
};

/* Reditus promptae: quaestio committitur (vacua tollit), electus
 * praestitutus, prompta clauditur */
interior b32
_promptam_committere (
    SanctorumMontatio* m,
               chorda  textus,
              Piscina* p)
{
     constans character* tituli[IV];
     constans character* valores[IV];
              character* q;
                    i32  i;
                    i32  n;

    q = (character*)piscina_allocare(p, (memoriae_index)textus.mensura
        + I);
    si (!q)
    {
        redde FALSUM;
    }
    /* quota duplex in attributo irrepraesentabilis: omittitur */
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si (textus.datum[i] != '"')
        {
            q[n++] = (character)textus.datum[i];
        }
    }
    q[n] = '\0';
    (vacuum)prompta_claudere(&m->ramus, actio_sanctorum, p);
    tituli[ZEPHYRUM]   = "quaestio";
    valores[ZEPHYRUM]  = q;
    tituli[I]          = "electus";
    valores[I]         = "";
    tituli[II]         = "pagina";
    valores[II]        = "";
    /* quaestio vacua: ordo calendarii (ut Esc quaestionem tollens) */
    tituli[III]        = "ordo";
    valores[III]       = _activa(chorda_ex_literis(q,
        p)) ? "congruentia"
        : "calendarium";
    redde _campos_scribere(m, tituli, valores, IV)
        && _praestitutum_scribere(m, p);
}

interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
       SanctorumMontatio* m;
        SanctorumFigurae* privatum;
     constans Praeceptum* t;
      ContextusPraecepti  cp;
                     i32  visus;
                     s32  n;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m         = (SanctorumMontatio*)ctx;
    privatum  = (SanctorumFigurae*)m->figurae_ctx;
    si (!privatum || !m->corpus)
    {
        redde FALSUM;
    }
    piscina_vacare(privatum->scheda);
    /* prompta aperta claves omnes accipit */
    si (prompta_aperta(&m->ramus, NIHIL))
    {
               chorda textus;
        PromptaExitus exitus;

        exitus = prompta_clavem_agere(&m->ramus, actio_sanctorum, ev,
            &textus, privatum->scheda);
        si (exitus == PROMPTA_REDITUS)
        {
            redde _promptam_committere(m, textus, privatum->scheda);
        }
        si (exitus != PROMPTA_NIHIL)
        {
            redde VERUM;
        }
    }
    visus = _visus_sancti(m) ? VISUS_SANCTI : VISUS_INDICIS;
    si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
    {
        t = praecepta_ad_clavem(TABULA_PRAECEPTORUM, visus, ev);
        si (!t)
        {
            redde FALSUM;
        }
        _contextum_parare(m, privatum->scheda, &cp);
        redde praecepta_status(t, visus, _requisita, &cp)
            == PRAECEPTUM_PRAESTO && t->fn(&cp);
    }
    si (   !nodus || ev->datum.mus.botton != MUS_SINISTER
        || (   ev->genus != EVENTUS_MUS_DEPRESSUS
            && ev->genus != EVENTUS_MUS_DUPLEX))
    {
        redde FALSUM;
    }
    _contextum_parare(m, privatum->scheda, &cp);
    si (visus == VISUS_SANCTI)
    {
        si (   ev->genus == EVENTUS_MUS_DEPRESSUS
            && (   chorda_aequalis_literis(nodus->id, "pagina-prior")
                || chorda_aequalis_literis(nodus->id,
                "pagina-proxima")))
        {
            redde _paginam_textus_vertere(&cp, chorda_aequalis_literis(
                nodus->id, "pagina-prior") ? -I : I);
        }
        redde FALSUM;
    }
    n = nodus->partes
        == PARTES_ITEM ? _id_ordinis(nodus->id) : ZEPHYRUM;
    si (n > ZEPHYRUM)
    {
        redde ev->genus == EVENTUS_MUS_DUPLEX ? _aperire(&cp, n - I)
            : _electum_scribere(&cp, n - I);
    }
    si (   ev->genus == EVENTUS_MUS_DEPRESSUS
        && (   chorda_aequalis_literis(nodus->id, "pagina-prior")
            || chorda_aequalis_literis(nodus->id, "pagina-proxima")))
    {
        redde _paginam_vertere(&cp, chorda_aequalis_literis(nodus->id,
            "pagina-prior") ? -I : I);
    }
    redde FALSUM;
}


/* ==================================================
 * Figura
 * ================================================== */

/* ordo indicis: electus fundo selectionis, ceteri linea infra;
 * titulus et festum (dextrum), vita obscura infra */
interior vacuum
_ordinem_pingere (
               Mandata* md,
    constans Componens* c,
      constans Sanctus* s,
                   b32  electus)
{
    constans SanctiFestum* f;
                   chorda  festum;
                   chorda  t;
                      s32  spatium;
                  Punctum  p;
                  Punctum  q;
                character  textus[XVI];

    si (electus)
    {
        figura_ordinem_electum_pingere(md, c->fines);
    }
    alioquin
    {
        p.x = c->fines.x;
        p.y = c->fines.y + c->fines.altitudo - I;
        q.x = c->fines.x + c->fines.latitudo - I;
        q.y = p.y;
        mandata_linea(md, p, q, I, figura_color_thematis(
            COLOR_SUPERFICIES));
    }
    spatium  = c->fines.latitudo - VIII;
    f        = sancti_festum_primum(s);
    si (f && f->mensis >= I && f->mensis <= XII)
    {
        sprintf(textus, "%s %d", menses_breves[f->mensis - I],
            (integer)f->dies);
        festum = chorda_ex_literis(textus, md->piscina);
        _textum_pingere(md, c->fines.x + c->fines.latitudo - IV
            - (s32)festum.mensura * SANCTI_LITTERA, c->fines.y + IV,
            festum, ZEPHYRUM, electus ? COLOR_TEXT : COLOR_TEXT_DIM);
        spatium -= (s32)(festum.mensura + II) * SANCTI_LITTERA;
    }
    t = _praecidere(s->titulus, spatium / SANCTI_LITTERA);
    _textum_pingere(md, c->fines.x + IV, c->fines.y + IV, t, ZEPHYRUM,
        COLOR_TEXT);
    t = _praecidere(_linea_vitae(s, md->piscina), (c->fines.latitudo
        - VIII) / SANCTI_LITTERA);
    _textum_pingere(md, c->fines.x + IV, c->fines.y + IV + SANCTI_LINEA
        + II, t, ZEPHYRUM, electus ? COLOR_TEXT : COLOR_TEXT_DIM);
}

interior vacuum
_paginas_pingere (
               Mandata* md,
    constans Componens* f,
                   s32  k,
                   s32  n)
{
    character textus[XXIV];

    si (chorda_aequalis_literis(f->id, "index_paginae") && n > I)
    {
        sprintf(textus, "%d/%d", (integer)(k + I), (integer)n);
        _textum_pingere(md, f->fines.x + (f->fines.latitudo
            - (s32)strlen(textus) * SANCTI_LITTERA) / II,
            f->fines.y + IV, chorda_ex_literis(textus, md->piscina),
            ZEPHYRUM, COLOR_TEXT_DIM);
    }
    alioquin si (chorda_aequalis_literis(f->id, "pagina-prior"))
    {
        figura_sagittam_pingere(md, f->fines, -I, k <= ZEPHYRUM
            ? FIGURA_OBSCURUM : ZEPHYRUM);
    }
    alioquin si (chorda_aequalis_literis(f->id, "pagina-proxima"))
    {
        figura_sagittam_pingere(md, f->fines, I, k >= n - I
            ? FIGURA_OBSCURUM : ZEPHYRUM);
    }
}

interior vacuum
_indicem_pingere (
     SanctorumMontatio* m,
    constans Componens* c,
               Mandata* md)
{
           Xar* x;
         Album* l;
     Componens* f;
           s32  e;
           s32  k;
           s32  n;
           s32  id;
           i32  i;
        chorda  q;
     character  textus[CXXVIII];
     character  vivus[PROMPTA_MAXIMA];

    x = _sancti(m, md->piscina);
    l = _album(m, x, md->piscina);
    e = _electus(m, x);
    k = _pagina(l, e);
    n = album_paginae(l);
    q = _quaestio(m, vivus);
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f = componens_liberum(c, i);
        si (chorda_aequalis_literis(f->id, "titulus"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                chorda_ex_literis("Saints", md->piscina), FONS_TITULI,
                COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput"))
        {
            si (prompta_aperta(&m->ramus, NIHIL))
            {
                prompta_pingere(md, f->fines, "search:", &m->ramus);
                perge;
            }
            si (q.mensura > ZEPHYRUM)
            {
                sprintf(textus,
                    "%u of %u \302\267 \"%.*s\" \302\267 %s",
                    (insignatus integer)(x ? xar_numerus(x)
                    : (i32)ZEPHYRUM),
                    (insignatus integer)sancti_numerus(m->corpus),
                    (integer)(q.mensura > XL ? XL : q.mensura),
                    (constans character*)q.datum, _ordo(m) == I
                    ? "by name" : _ordo(m) == II ? "by lifetime"
                    : _ordo(m) == III ? "best match" : "by date");
            }
            alioquin
            {
                sprintf(textus, "%u saints \302\267 %s",
                    (insignatus integer)sancti_numerus(m->corpus),
                    _ordo(m) == I ? "by name" : _ordo(m) == II
                    ? "by lifetime" : "by date");
            }
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis(textus, md->piscina), ZEPHYRUM,
                COLOR_TEXT_DIM);
        }
        alioquin si (chorda_aequalis_literis(f->id, "nihil"))
        {
            si (q.mensura > ZEPHYRUM)
            {
                sprintf(textus, "No saints match \"%.*s\"",
                    (integer)(q.mensura > XL ? XL : q.mensura),
                    (constans character*)q.datum);
            }
            alioquin
            {
                sprintf(textus, "No saints found (sancti/)");
            }
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis(textus, md->piscina), ZEPHYRUM,
                COLOR_TEXT_DIM);
        }
        alioquin si (f->partes == PARTES_ITEM)
        {
            id = _id_ordinis(f->id);
            si (id > ZEPHYRUM && _sanctus(x, id - I))
            {
                _ordinem_pingere(md, f, _sanctus(x, id - I),
                    id - I == e);
            }
        }
        alioquin
        {
            _paginas_pingere(md, f, k, n);
        }
    }
}

interior vacuum
_sanctum_pingere (
     SanctorumMontatio* m,
    constans Componens* c,
               Mandata* md)
{
       constans Sanctus* s;
              Componens* f;
                 chorda  corpus;
     PaginariumResultus  r;
       PaginariumPagina* pg;
                    s32  k;
                    i32  i;
                    i32  j;
                    s32  columnae;
                Punctum  p;
                Punctum  q;

    s = _apertus(m);
    si (!s)
    {
        redde;
    }
    r = _paginae_sancti(m, s, &corpus, md->piscina);
    k = _campus_s32(m, "pagina_textus");
    si (k >= (s32)r.numerus_paginarum)
    {
        k = (s32)r.numerus_paginarum - I;
    }
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        f         = componens_liberum(c, i);
        columnae  = (f->fines.latitudo - VIII) / SANCTI_LITTERA;
        si (chorda_aequalis_literis(f->id, "titulus"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + II,
                _praecidere(s->titulus, (f->fines.latitudo - VIII)
                / VIII), FONS_TITULI, COLOR_TEXT);
        }
        alioquin si (chorda_aequalis_literis(f->id, "caput_sancti"))
        {
            chorda lineae[IV];
               s32 y;

            lineae[ZEPHYRUM]  = chorda_aequalis(s->titulus_latinus,
                s->titulus) ? chorda_ex_literis("", md->piscina)
                : s->titulus_latinus;
            lineae[I]    = _linea_ortus_obitus(s, md->piscina);
            lineae[II]   = _linea_canonizationis(s, md->piscina);
            lineae[III]  = _linea_festorum(s, md->piscina);
            y            = f->fines.y + II;
            /* lineae vacuae omittuntur (caput compactum) */
            per (j = ZEPHYRUM; j < IV; j++)
            {
                si (lineae[j].mensura == ZEPHYRUM)
                {
                    perge;
                }
                _textum_pingere(md, f->fines.x + IV, y,
                    _praecidere(lineae[j], columnae), ZEPHYRUM,
                    j == ZEPHYRUM ? COLOR_TEXT_DIM : COLOR_TEXT);
                y += SANCTI_SPATIUM;
            }
            p.x = f->fines.x;
            p.y = f->fines.y + f->fines.altitudo - II;
            q.x = f->fines.x + f->fines.latitudo - I;
            q.y = p.y;
            mandata_linea(md, p, q, I, figura_color_thematis(
                COLOR_SUPERFICIES));
        }
        alioquin si (chorda_aequalis_literis(f->id, "textus"))
        {
            pg = k >= ZEPHYRUM ? paginarium_pagina_obtinere(&r, (i32)k)
                : NIHIL;
            per (j = ZEPHYRUM; pg
                 && j < paginarium_pagina_numerus_linearum(pg); j++)
            {
                PaginariumLinea* ln = paginarium_linea_obtinere(pg, j);

                si (!ln || ln->est_vacua)
                {
                    perge;
                }
                _textum_pingere(md, f->fines.x + IV, f->fines.y
                    + II + (s32)j * SANCTI_SPATIUM,
                    paginarium_linea_reddere(
                    corpus, ln, md->piscina), ZEPHYRUM, COLOR_TEXT);
            }
        }
        alioquin si (chorda_aequalis_literis(f->id, "pes"))
        {
            _textum_pingere(md, f->fines.x + IV, f->fines.y + IV,
                chorda_ex_literis("Esc back \302\267 j/k saint",
                md->piscina), ZEPHYRUM, COLOR_TEXT_DIM);
        }
        alioquin
        {
            _paginas_pingere(md, f, k < ZEPHYRUM ? ZEPHYRUM : k,
                (s32)r.numerus_paginarum);
        }
    }
}

interior vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    SanctorumFigurae* contextus;
               Fines  mensa;

    (vacuum)thema;
    contextus       = (SanctorumFigurae*)ctx;
    mensa.x         = ZEPHYRUM;
    mensa.y         = ZEPHYRUM;
    mensa.latitudo  = c->fines.latitudo;
    mensa.altitudo  = c->fines.altitudo;
    figura_mensam_pingere(md, mensa, chorda_aequalis_literis(c->titulus,
        "focatum"));
    mensa.x         = SANCTI_LITTERA;
    mensa.y         = SANCTI_LINEA;
    mensa.latitudo  = c->fines.latitudo - II * SANCTI_LITTERA;
    mensa.altitudo  = c->fines.altitudo - II * SANCTI_LINEA;
    mandata_rectangulum(md, mensa, figura_color_thematis(
        COLOR_BACKGROUND), VERUM);
    si (!contextus || !contextus->m)
    {
        redde;
    }
    si (_visus_sancti(contextus->m) && _apertus(contextus->m))
    {
        _sanctum_pingere(contextus->m, c, md);
    }
    alioquin
    {
        _indicem_pingere(contextus->m, c, md);
    }
}

interior b32
_figuras_registrare (
    SanctorumMontatio* m,
                  i32  latitudo,
                  i32  altitudo)
{
    SanctorumFigurae* contextus;

    contextus = (SanctorumFigurae*)piscina_allocare(m->piscina,
        magnitudo(SanctorumFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    contextus->m        = m;
    contextus->organum  = NIHIL;
    contextus->scheda                = piscina_generare_dynamicum(
        "sancti_scheda", LXIV * M);
    contextus->latitudo_montationis  = (s32)latitudo;
    contextus->altitudo_montationis  = (s32)altitudo;
    m->figurae_ctx                   = contextus;
    redde contextus->scheda && figura_registrare(m->figurae,
        PARTES_TABULA, ZEPHYRUM, figura_tabulae, contextus);
}


/* ==================================================
 * Componere (dispositio in cellulis)
 * ================================================== */

nomen structura {
           s32  index;
     Componens* c;
} LocusComponentis;

nomen structura {
             Dispositio* d;
                    Xar* loci;
                Piscina* piscina;
    InternamentumChorda* intern;
} Disponens;

interior s32
_locum_addere (
             Disponens* dp,
                   s32  parens,
    DispositioDirectio  directio,
       DispositioGenus  genus_x,
                   s32  valor_x,
       DispositioGenus  genus_y,
                   s32  valor_y)
{
    DispositioForma forma;

    dispositio_formam_initiare(&forma);
    forma.directio        = directio;
    forma.latitudo.genus  = genus_x;
    forma.latitudo.valor  = valor_x;
    forma.altitudo.genus  = genus_y;
    forma.altitudo.valor  = valor_y;
    redde dispositio_addere(dp->d, parens, &forma);
}

interior Componens*
_componentem_locare (
              Disponens* dp,
                    s32  parens,
     constans character* id,
                 Partes  partes,
        DispositioGenus  genus_x,
                    s32  valor_x,
        DispositioGenus  genus_y,
                    s32  lineae)
{
            Componens* c;
     LocusComponentis* l;
                  s32  index;

    index = _locum_addere(dp, parens, DISPOSITIO_LINEA, genus_x,
        valor_x,
        genus_y, lineae);
    c     = componens_creare(dp->piscina, dp->intern, id, partes);
    si (!c || index < ZEPHYRUM)
    {
        redde NIHIL;
    }
    componens_ponere_actio(c, actio_sanctorum);
    l = (LocusComponentis*)xar_addere(dp->loci);
    si (l)
    {
        l->index  = index;
        l->c      = c;
    }
    redde c;
}

/* caput vel pes: textus crescens et paginae (sagittae, index) */
interior vacuum
_lineam_paginarum_disponere (
              Disponens* dp,
                    s32  radix_d,
     constans character* id)
{
    s32 linea;

    linea = _locum_addere(dp, radix_d, DISPOSITIO_LINEA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, SANCTI_CAPUT);
    (vacuum)_componentem_locare(dp, linea, id, PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, SANCTI_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-prior",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        SANCTI_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "index_paginae",
        PARTES_INDEX, DISPOSITIO_FIXA, VII, DISPOSITIO_FIXA,
        SANCTI_CAPUT);
    (vacuum)_componentem_locare(dp, linea, "pagina-proxima",
        PARTES_BOTTONE, DISPOSITIO_FIXA, III, DISPOSITIO_FIXA,
        SANCTI_CAPUT);
}

interior vacuum
_indicem_disponere (
    SanctorumMontatio* m,
            Disponens* dp,
                  s32  radix_d,
              Piscina* piscina)
{
                    Xar* x;
                  Album* l;
     constans AlbumOrdo* o;
                    s32  regio;
                    s32  a;
                    s32  b;
                    s32  t;
                    s32  i;

    (vacuum)_componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, SANCTI_TITULI);
    _lineam_paginarum_disponere(dp, radix_d, "caput");
    regio = _locum_addere(dp, radix_d, DISPOSITIO_COLUMNA,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    x = _sancti(m, piscina);
    l = _album(m, x, piscina);
    si (!l || album_numerus(l) == ZEPHYRUM)
    {
        (vacuum)_componentem_locare(dp, regio, "nihil", PARTES_INDEX,
            DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
            SANCTI_ORDO);
        redde;
    }
    album_pagina(l, _pagina(l, _electus(m, x)), &a, &b, &t);
    per (i = a; i < b; i++)
    {
        o = album_ordo(l, (i32)i);
        (vacuum)_componentem_locare(dp, regio, chorda_ut_cstr(o->id,
            piscina), PARTES_ITEM, DISPOSITIO_CRESCENS, ZEPHYRUM,
            DISPOSITIO_FIXA, o->altitudo);
    }
}

interior vacuum
_sanctum_disponere (
     Disponens* dp,
           s32  radix_d)
{
    (vacuum)_componentem_locare(dp, radix_d, "titulus", PARTES_TITULUS,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA, SANCTI_TITULI);
    (vacuum)_componentem_locare(dp, radix_d, "caput_sancti",
        PARTES_INDEX, DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_FIXA,
        SANCTI_CAPUT_VISUS);
    (vacuum)_componentem_locare(dp, radix_d, "textus", PARTES_INDEX,
        DISPOSITIO_CRESCENS, ZEPHYRUM, DISPOSITIO_CRESCENS, ZEPHYRUM);
    _lineam_paginarum_disponere(dp, radix_d, "pes");
}

Componens*
sancti_visus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    SanctorumMontatio* m;
            Componens* radix;
            Disponens  dp;
      DispositioForma  forma;
                  s32  latitudo;
                  s32  altitudo;
                  s32  columnae;
                  s32  lineae;
                  s32  radix_d;
                  s32  i;
                Fines  f;

    (vacuum)repo;
    m = (SanctorumMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo = _superficies(m, VERUM);
    altitudo = _superficies(m, FALSUM);
    radix     = componens_creare(piscina, intern, "sancti",
        PARTES_TABULA);
    si (!radix)
    {
        redde NIHIL;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    componens_ponere_fines(radix, f);
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_sanctorum);
    si (motus_ramus_focatus(motus, m->ramus))
    {
        componens_ponere_titulum(radix, "focatum");
    }
    columnae    = latitudo / SANCTI_LITTERA;
    lineae      = altitudo / SANCTI_LINEA;
    dp.d        = dispositio_creare(piscina);
    dp.loci     = xar_creare(piscina, (i32)magnitudo(LocusComponentis));
    dp.piscina  = piscina;
    dp.intern   = intern;
    si (!dp.d || !dp.loci)
    {
        redde radix;
    }
    dispositio_formam_initiare(&forma);
    forma.directio           = DISPOSITIO_COLUMNA;
    forma.latitudo.genus     = DISPOSITIO_CRESCENS;
    forma.altitudo.genus     = DISPOSITIO_CRESCENS;
    forma.spatium_sinistrum  = I;
    forma.spatium_dextrum    = I;
    forma.spatium_superum    = I;
    forma.spatium_inferum    = I;
    radix_d                  = dispositio_addere(dp.d, -I, &forma);
    si (_visus_sancti(m) && _apertus(m))
    {
        _sanctum_disponere(&dp, radix_d);
    }
    alioquin
    {
        _indicem_disponere(m, &dp, radix_d, piscina);
    }
    dispositio_computare(dp.d, columnae, lineae, NIHIL, NIHIL);
    per (i = ZEPHYRUM; i < (s32)xar_numerus(dp.loci); i++)
    {
        LocusComponentis* loc;

        loc = (LocusComponentis*)xar_obtinere(dp.loci, (i32)i);
        componens_ponere_fines(loc->c, dispositio_ad_pixela(
            dispositio_fines(dp.d, loc->index), columnae, lineae,
            latitudo, altitudo, SANCTI_LITTERA, SANCTI_LINEA));
        componens_addere_liberum(radix, loc->c);
    }
    redde radix;
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
sancti_visus_montare (
       SanctorumMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
   constans SanctiCorpus* corpus,
          SanctorumHodie  hodie,
                  vacuum* hodie_ctx,
                     i32  latitudo,
                     i32  altitudo,
         StatusSystemata* systemata)
{
    Organum* organum;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(SanctorumMontatio));
    m->piscina    = piscina;
    m->intern     = intern;
    m->corpus     = corpus;
    m->hodie      = hodie;
    m->hodie_ctx  = hodie_ctx;
    m->ramus      = id ? insula_ramus(repo, "sancti", id)
                       : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "sancti", "sancti");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"sancti\"", id ? id : "sancti", piscina,
               intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae
        || !praecepta_verificare(TABULA_PRAECEPTORUM)
        || !actio_registrare(m->actiones, actio_sanctorum, agere, m)
        || !_figuras_registrare(m, latitudo, altitudo))
    {
        redde FALSUM;
    }
    ((SanctorumFigurae*)m->figurae_ctx)->organum = organum;
    redde VERUM;
}

/* SemitaOrganum lateris: manifestum suum (liberi nulli); ctx =
 * SanctorumMontatio* */
interior constans Organum*
_organa (
     vacuum* ctx,
     chorda  genus)
{
    SanctorumMontatio* m = (SanctorumMontatio*)ctx;

    si (   !m || !m->figurae_ctx
        || !chorda_aequalis_literis(genus, "sancti"))
    {
        redde NIHIL;
    }
    redde ((SanctorumFigurae*)m->figurae_ctx)->organum;
}

/* argumentum ut semita: URI ("sancti/...", "sancti?..."), vacuum =
 * index, aliter id nudum (alias <sancti visus="sanctus"
 * sanctus_apertus="id"/>) */
interior StmlNodus*
_semita_argumenti (
     SanctorumMontatio* m,
    constans character* argumentum,
                chorda* vitium)
{
       StmlNodus* n;
             i32  l;

    l = (i32)strlen(argumentum);
    si (   l == ZEPHYRUM
        || (strncmp(argumentum, "sancti", VI) == ZEPHYRUM
        && (l == VI || argumentum[VI] == '/' || argumentum[VI] == '?')))
    {
        redde semita_ex_uri(_organa, m, chorda_ex_literis(l == ZEPHYRUM
            ? "sancti/index" : argumentum, m->piscina), m->piscina,
            m->intern, vitium);
    }
    n = stml_elementum_creare(m->piscina, m->intern, "sancti");
    si (   !n
        || !stml_attributum_addere(n, m->piscina, m->intern, "visus",
               "sanctus")
        || !stml_attributum_addere(n, m->piscina, m->intern,
               "sanctus_apertus", argumentum))
    {
        redde NIHIL;
    }
    redde n;
}

b32
sancti_visus_argumentum_ponere (
       SanctorumMontatio* m,
      constans character* argumentum)
{
              StmlNodus* semita;
                 chorda* apertus;
                 chorda* visus;
                 chorda  vitium;
     constans character* tituli[II];
     constans character* valores[II];

    si (!m || !m->corpus)
    {
        redde FALSUM;
    }
    semita = _semita_argumenti(m, argumentum ? argumentum : "",
        &vitium);
    si (!semita)
    {
        redde FALSUM;
    }
    /* sanctus ignotus: recusatur ante scripturam */
    apertus = stml_attributum_capere(semita, "sanctus_apertus");
    si (   apertus && apertus->mensura > ZEPHYRUM
        && !sancti_invenire(m->corpus, *apertus))
    {
        redde FALSUM;
    }
    si (!semita_applicare(_organa, m, &m->ramus, semita, m->piscina,
            &vitium))
    {
        redde FALSUM;
    }
    /* sequelae lateris (non semita): origo, electus = apertus */
    visus = insula_ramus_attributum(&m->ramus, "visus");
    si (visus && chorda_aequalis_literis(*visus, "sanctus"))
    {
        tituli[ZEPHYRUM]   = "origo";
        valores[ZEPHYRUM]  = "argumentum";
        tituli[I]          = "electus";
        valores[I]         = chorda_ut_cstr(_campus(m,
            "sanctus_apertus"),
            m->piscina);
        redde _campos_scribere(m, tituli, valores, II);
    }
    redde _campum_scribere(m, "origo", "index");
}
