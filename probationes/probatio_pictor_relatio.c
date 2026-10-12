/* probatio_pictor_relatio.c - systema relationum in pictore
 * (status-plan S5b): stratum_activum (referentia documentum/stratum,
 * orbatus summum) reparatur cum stratum evanescit - revocatione,
 * deletione in documento communi (ramus alter quoque), ligatione nova
 * (pictura minor), restitutione stala. Refectio reparationem non
 * revocat (Franus: a). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "pictor_documentum.h"
#include "pictor_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

/* ctx = "attributum=valor" */
interior vacuum
ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
     constans character* par;
     constans character* signum;
              character  titulus[LXIV];

    par     = (constans character*)ctx;
    signum  = strchr(par, '=');
    memcpy(titulus, par, (size_t)(signum - par));
    titulus[signum - par] = '\0';
    insula_attributum_ponere(radix, p, in, titulus, signum + I);
}

/* stratum_activum a domino (strata.agere) */
interior b32
activum_ponere (
    InsulaRepositorium* repo,
        PictorMontatio* m,
    constans character* id,
               Piscina* p)
{
    character par[XXXII];

    sprintf(par, "stratum_activum=%s", id);
    insula_scriptorem_ponere(repo, chorda_ex_literis("strata.agere",
        p));
    redde mutare_ramum(&m->ramus, ponere, par);
}

interior b32
activum_est (
        PictorMontatio* m,
    constans character* id)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus,
        "stratum_activum");
    redde id ? (a && chorda_aequalis_literis(*a, id)) : a == NIHIL;
}

interior vacuum
actum (
        PictorMontatio* m,
    constans character* textus)
{
    (vacuum)pictor_documentum_actum(m->doc, chorda_ex_literis(textus,
        m->doc->piscina));
}

nomen structura {
     InsulaRepositorium* repo;
        StatusSystemata  systemata;
} Mundus;

interior b32
mundum_creare (
                 Mundus* w,
                Piscina* p,
    InternamentumChorda* in,
                Volumen* vol,
                    b32  cum_conservatione)
{
    memset(w, ZEPHYRUM, magnitudo(*w));
    w->repo = insula_repositorium_creare(p, in,
        "<hospes focus=\"tabula\"/>");
    si (!w->repo)
    {
        redde FALSUM;
    }
    w->systemata.relatio = relatio_creare(p, in, w->repo);
    si (cum_conservatione)
    {
        w->systemata.conservatio = conservatio_creare(p, in, vol,
            w->repo);
    }
    redde w->systemata.relatio != NIHIL;
}


/* I. revocatio stratum tollit; refectio reparationem non revocat */
interior vacuum
_probare_revocare (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  w;
         PictorMontatio  m;

    in  = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_pictor_relatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mundum_creare(&w, c->piscina, in, vol, FALSUM));
    CREDO_NECESSE(pictor_montare(&m, c->piscina, in, vol, w.repo, "p1",
        CDLXXX, CDLXXX, &w.systemata));
    actum(&m, "<stratum actio=\"novum\" id=\"2\" supra=\"1\"/>");
    CREDO_VERUM(activum_ponere(w.repo, &m, "2", c->piscina));
    CREDO_VERUM(activum_est(&m, "2"));
    CREDO_VERUM(pictor_documentum_revocare(m.doc));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(m.doc), I);
    CREDO_VERUM(activum_est(&m, "1"));
    CREDO_VERUM(pictor_documentum_reficere(m.doc));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(m.doc), II);
    CREDO_VERUM(activum_est(&m, "1"));   /* a: non revocatur */
    volumen_claudere(vol);
}


/* II. documentum commune: deletio in uno, ramus alter reparatur */
interior vacuum
_probare_ramos_communes (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  w;
         PictorMontatio  m1;
         PictorMontatio  m2;

    in  = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_pictor_relatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mundum_creare(&w, c->piscina, in, vol, FALSUM));
    CREDO_NECESSE(pictor_montare(&m1, c->piscina, in, vol, w.repo, "p1",
        CDLXXX, CDLXXX, &w.systemata));
    CREDO_NECESSE(pictor_montare(&m2, c->piscina, in, vol, w.repo, "p2",
        CDLXXX, CDLXXX, &w.systemata));
    /* m2 picturam p1 ostendit - documento EODEM (memoria communis) */
    m2.documenta = m1.documenta;
    CREDO_NECESSE(pictor_picturam_ponere(&m2, "p1"));
    CREDO_NECESSE(m2.doc == m1.doc);
    actum(&m1, "<stratum actio=\"novum\" id=\"2\" supra=\"1\"/>");
    actum(&m1, "<stratum actio=\"novum\" id=\"3\" supra=\"2\"/>");
    CREDO_VERUM(activum_ponere(w.repo, &m1, "2", c->piscina));
    CREDO_VERUM(activum_ponere(w.repo, &m2, "2", c->piscina));
    actum(&m1, "<stratum actio=\"deletum\" id=\"2\"/>");
    CREDO_VERUM(activum_est(&m1, "3"));
    CREDO_VERUM(activum_est(&m2, "3"));
    volumen_claudere(vol);
}


/* III. ligatio nova: pictura minor */
interior vacuum
_probare_ligationem_novam (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  w;
         PictorMontatio  m;

    in  = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_pictor_relatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(mundum_creare(&w, c->piscina, in, vol, FALSUM));
    CREDO_NECESSE(pictor_montare(&m, c->piscina, in, vol, w.repo, "p1",
        CDLXXX, CDLXXX, &w.systemata));
    actum(&m, "<stratum actio=\"novum\" id=\"2\" supra=\"1\"/>");
    actum(&m, "<stratum actio=\"novum\" id=\"3\" supra=\"2\"/>");
    CREDO_VERUM(activum_ponere(w.repo, &m, "3", c->piscina));
    CREDO_NECESSE(pictor_picturam_condere(&m, "alia"));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(m.doc), I);
    CREDO_VERUM(activum_est(&m, "1"));
    volumen_claudere(vol);
}


/* IV. restitutio stala reparatur in montatione */
interior vacuum
_probare_restitutionem (
    CredoContextus* c)
{
    InternamentumChorda* in;
                Volumen* vol;
                 Mundus  w;
         PictorMontatio  m;

    in  = internamentum_creare(c->piscina);
    thema_initiare();
    vol = volumen_temporarium(c->piscina, "probatio_pictor_relatio");
    CREDO_NECESSE_NON_NIHIL(vol);
    CREDO_NECESSE(volumen_plagulam_condere(vol, chorda_ex_literis(
        "status/p1", c->piscina), chorda_ex_literis(
        "<status versio=\"1\" genus=\"pictor\"><durabilis/>"
        "<ephemera stratum_activum=\"7\" zoom=\"2\"/></status>",
        c->piscina), "probatio"));
    CREDO_NECESSE(mundum_creare(&w, c->piscina, in, vol, VERUM));
    CREDO_NECESSE(pictor_montare(&m, c->piscina, in, vol, w.repo, "p1",
        CDLXXX, CDLXXX, &w.systemata));
    /* zoom restitutus, stratum 7 (nullum) reparatum */
    CREDO_VERUM(chorda_aequalis_literis(*insula_ramus_attributum(
        &m.ramus, "zoom"), "2"));
    CREDO_VERUM(activum_est(&m, "1"));
    volumen_claudere(vol);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "revocatio",     _probare_revocare,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "rami_communes", _probare_ramos_communes,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "ligatio_nova",  _probare_ligationem_novam,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { "restitutio",    _probare_restitutionem,
      NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("pictor_relatio", SECTIONES);
}
