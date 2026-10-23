/* vicus.c - hospes applicationum (insula-rami-plan T1b; vicus-latera
 * S2a: tabula = par laterum) */

#include "vicus.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "stml.h"
#include "canon.h"
#include "thema.h"
#include "mandatum.h"
#include "componens.h"
#include "organum_montatio.h"
#include "membrum__prompta.h"

#include <stdio.h>
#include <string.h>
#include <time.h>

/* dispositio (S2a): plagula nova - forma vetus 'vicus/tabulae' non
 * legitur (volumina vetera dispositionem ordinariam accipiunt) */
hic_manens constans character clavis_indicis[] = "vicus/latera";
/* latus finitum (vicus-latera S1c): post titulum in linea, numquam in
 * indice */
hic_manens constans character suffixum_finitae[] = " [exitus]";
hic_manens constans character id_divisoris[] = "vicus.divisor";


/* ==================================================
 * Auxilia
 * ================================================== */

/* S3c: aperitio in acervo pendens (vicus_acervo_aperire); S3e:
 * mutare = argumentum frontis generis mutatur (vicus_acervo_mutare) */
nomen structura {
    chorda tabula;
    chorda genus;
    chorda argumentum;
       b32 mutare;
       s32 gradus;      /* T4f: +I / -I = gradus cycli (genus nullum);
                         * ZEPHYRUM = aperitio */
       b32 advocare;    /* D5: post aperitionem facies.advocatum */
       b32 historia;    /* D5: gradus historiae tabulae - locus */
       i32 locus;       /* index loci (si historia) */
       i32 latus;       /* historia lateris (VICUS_SINISTRUM aut
                         * VICUS_DEXTRUM) */
       b32 aditus;      /* comites P5a: post advocatum aditus pendet
                         * (Esc in prompta revocat) */
} PetitioAcervi;

/* comites P5a: gradus aditus tabulae (VicusTabula.aditus_gradus) */
#define ADITUS_NULLUS      ZEPHYRUM
#define ADITUS_PENDENS     I     /* prompta advocata aperta */
#define ADITUS_REVOCANDUS  II    /* Esc eam clausit: pulsus revocat
                                  * (historia petita, focus
                                  * differtus) */

/* D5: brevitas hospitis (vicus_brevitatem_addere): Cmd + runa +
 * modificantes (praeter Cmd, exacte) -> vicus_advocare(genus) */
nomen structura {
          s32  runa;
          i32  modificantes;
    character* genus;
} Brevitas;

/* modificantes quos brevitas discernit (Caps Lock, Num Lock, latera
 * ignorantur) */
#define MODI_BREVITATIS (MOD_SHIFT | MOD_IMPERIUM | MOD_ALT)

/* T4f: latus cycli - identitas ut petitio (genus + argumentum) */
nomen structura {
    chorda genus;
    chorda argumentum;   /* vacuum = sine argumento */
} LatusCycli;

interior chorda
chorda_nulla_vici (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior constans VicusGenus*
genus_invenire (
    constans Vicus* v,
            chorda  titulus)
{
    VicusGenus* g;
           i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(v->genera); i++)
    {
        g = (VicusGenus*)xar_obtinere(v->genera, i);
        si (chorda_aequalis(g->titulus, titulus))
        {
            redde g;
        }
    }
    redde NIHIL;
}

interior VicusTabula*
tabula_invenire (
    constans Vicus* v,
            chorda  id)
{
    VicusTabula* t;
            i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        si (chorda_aequalis(t->id, id))
        {
            redde t;
        }
    }
    redde NIHIL;
}

interior chorda
internare (
                  Vicus* v,
     constans character* litterae)
{
    chorda* c;

    c = chorda_internare_ex_literis(v->intern, litterae);
    redde c ? *c : chorda_nulla_vici();
}

interior constans character*
titulus_lateris (
    i32 latus)
{
    redde latus == VICUS_SINISTRUM ? "sinistrum" : "dextrum";
}

/* id lateris = via "<tabula>_<latus>_<genus>" (internata; canon 'nomen'
 * puncta non admittit - litterae, numeri, '_') */
interior chorda
id_lateris (
      Vicus* v,
     chorda  tabula,
        i32  latus,
     chorda  genus)
{
    chorda c;

    c = chorda_concatenare(tabula, chorda_ex_literis("_", v->piscina),
        v->piscina);
    c = chorda_concatenare(c, chorda_ex_literis(titulus_lateris(latus),
        v->piscina), v->piscina);
    c = chorda_concatenare(c, chorda_ex_literis("_", v->piscina),
        v->piscina);
    c = chorda_concatenare(c, genus, v->piscina);
    redde internare(v, chorda_ut_cstr(c, v->piscina));
}

/* frons acervi (ultimus); NIHIL si acervus vacuus */
interior VicusLatus*
frons_acervi (
    constans VicusTabula* t)
{
    si (!t || !t->acervus || xar_numerus(t->acervus) == ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde (VicusLatus*)xar_obtinere(t->acervus,
        xar_numerus(t->acervus) - I);
}

/* tabula utilis (praefixum, commutatio): latus aliquod visibile
 * montatum */
interior b32
tabula_montata (
    constans VicusTabula* t)
{
    VicusLatus* d;

    d = frons_acervi(t);
    redde t->sinistrum.montata || (d && d->montata);
}

/* latitudo lateris sinistri: dimidium cellulis rotundatum (deorsum) */
interior s32
latitudo_sinistri (
    s32 latitudo)
{
    redde (latitudo / II) / VICUS_CELLULA_LATITUDO
        * VICUS_CELLULA_LATITUDO;
}

interior s32
altitudo_laterum (
    s32 altitudo)
{
    redde altitudo > VICUS_ALTITUDO_TABULARUM
        ? altitudo - VICUS_ALTITUDO_TABULARUM : ZEPHYRUM;
}

/* dispositio in volumen (superscribitur) */
/* valor attributi effugitus ('&', '<', '"') */
interior vacuum
valorem_appendere (
     ChordaAedificator* a,
                chorda  c)
{
    i32 i;

    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        commutatio (c.datum[i])
        {
            casus '&':
                chorda_aedificator_appendere_literis(a, "&amp;");
                frange;
            casus '<':
                chorda_aedificator_appendere_literis(a, "&lt;");
                frange;
            casus '"':
                chorda_aedificator_appendere_literis(a, "&quot;");
                frange;
            ordinarius:
                chorda_aedificator_appendere_character(a,
                    (character)c.datum[i]);
                frange;
        }
    }
}

/* S3c: id et argumentum (si non vacuum) scribuntur - acervus ordinem
 * mutat, id ex ordine non iam derivari potest */
interior vacuum
latus_appendere (
       ChordaAedificator* a,
     constans VicusLatus* l)
{
    chorda_aedificator_appendere_literis(a, "<latus genus=\"");
    chorda_aedificator_appendere_chorda(a, l->genus);
    chorda_aedificator_appendere_literis(a, "\" id=\"");
    chorda_aedificator_appendere_chorda(a, l->id);
    chorda_aedificator_appendere_literis(a, "\"");
    si (l->argumentum.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " argumentum=\"");
        valorem_appendere(a, l->argumentum);
        chorda_aedificator_appendere_literis(a, "\"");
    }
    /* semitae R4d: semita persistens (restitutio post initium) */
    si (l->semita_servata.mensura > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, " semita=\"");
        valorem_appendere(a, l->semita_servata);
        chorda_aedificator_appendere_literis(a, "\"");
    }
    chorda_aedificator_appendere_literis(a, "/>");
}

interior b32
indicem_scribere (
    Vicus* v)
{
    ChordaAedificator* a;
          VicusTabula* t;
                  i32  i;
                  i32  k;

    a = chorda_aedificator_creare(v->piscina, (memoriae_index)DXII);
    chorda_aedificator_appendere_literis(a, "<tabulae activa=\"");
    chorda_aedificator_appendere_chorda(a, v->activa);
    chorda_aedificator_appendere_literis(a, "\">");
    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        chorda_aedificator_appendere_literis(a, "<tabula id=\"");
        chorda_aedificator_appendere_chorda(a, t->id);
        chorda_aedificator_appendere_literis(a, "\" focus=\"");
        chorda_aedificator_appendere_literis(a,
            titulus_lateris(t->focus));
        chorda_aedificator_appendere_literis(a, "\">");
        latus_appendere(a, &t->sinistrum);
        chorda_aedificator_appendere_literis(a, "<acervus>");
        per (k = ZEPHYRUM; k < xar_numerus(t->acervus); k++)
        {
            latus_appendere(a,
                (constans VicusLatus*)xar_obtinere(t->acervus, k));
        }
        chorda_aedificator_appendere_literis(a, "</acervus></tabula>");
    }
    chorda_aedificator_appendere_literis(a, "</tabulae>");
    redde volumen_praesens_ponere(v->volumen,
        chorda_ex_literis(clavis_indicis, v->piscina),
        chorda_aedificator_finire(a));
}

/* latus montare (descriptio NIHIL: praeteritur, causa) */
interior vacuum
latus_montare (
         Vicus* v,
    VicusLatus* l,
           s32  latitudo,
           s32  altitudo)
{
    si (!l->descriptio)
    {
        v->causa = chorda_concatenare(chorda_ex_literis(
            "genus ignotum, latus non montatum: ", v->piscina),
            l->genus,
            v->piscina);
        redde;
    }
    l->montatio = piscina_allocare(v->piscina, l->descriptio->mensura);
    si (!l->montatio)
    {
        redde;
    }
    memset(l->montatio, ZEPHYRUM, l->descriptio->mensura);
    l->montata = l->descriptio->montare(l->montatio, v->piscina,
        v->intern,
        v->volumen, v->repo, chorda_ut_cstr(l->id, v->piscina),
        l->argumentum.mensura > ZEPHYRUM
            ? chorda_ut_cstr(l->argumentum, v->piscina) : NIHIL,
        (i32)latitudo, (i32)altitudo, l->descriptio->ctx);
    si (!l->montata)
    {
        v->causa = chorda_concatenare(chorda_ex_literis(
            "montatio defecit: ", v->piscina), l->id, v->piscina);
        redde;
    }
    memset(&l->facies, ZEPHYRUM, magnitudo(VicusFacies));
    l->descriptio->describere(l->montatio, &l->facies);
}

/* superficies montationum (T2b, S2a): applicatio superficiem ex ramo
 * suo legit; hospes eam scribit nomine 'dispensator' (domini
 * applicationum superficiem dispensatori addicunt - in vico hospes
 * dispensator montationum est). Latus sinistrum dimidium, acervus
 * reliquum. */
nomen structura {
    s32 latitudo;
    s32 altitudo;
} Superficies;

interior vacuum
superficiem_mutator (
              StmlNodus* nodus,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans Superficies* sp;

    sp = (constans Superficies*)ctx;
    insula_attributum_ponere(nodus, p, in, "superficies_latitudo",
        chorda_ut_cstr(chorda_ex_s32(sp->latitudo, p), p));
    insula_attributum_ponere(nodus, p, in, "superficies_altitudo",
        chorda_ut_cstr(chorda_ex_s32(sp->altitudo, p), p));
}

/* via rami lateris (status-plan S8c): spatium arboris, registrorum,
 * Motus */
interior chorda
via_lateris (
       InsulaRepositorium* repo,
      constans VicusLatus* l,
                  Piscina* piscina)
{
    redde insula_ramus(repo, chorda_ut_cstr(l->genus, piscina),
        chorda_ut_cstr(l->id, piscina)).via;
}

interior vacuum
latus_superficiem_scribere (
         Vicus* v,
    VicusLatus* l,
           s32  latitudo,
           s32  altitudo)
{
    InsulaRamus ramus;
    Superficies sp;

    si (!l->montata)
    {
        redde;
    }
    sp.latitudo = latitudo;
    sp.altitudo = altitudo;
    ramus = insula_ramus(v->repo, chorda_ut_cstr(l->genus, v->piscina),
        chorda_ut_cstr(l->id, v->piscina));
    (vacuum)mutare_ramum(&ramus, superficiem_mutator,
        &sp);
}

interior vacuum
superficies_scribere (
    Vicus* v,
      s32  latitudo,
      s32  altitudo)
{
     VicusTabula* t;
          chorda  prior;
             s32  ls;
             s32  alt;
             i32  i;
             i32  k;

    ls     = latitudo_sinistri(latitudo);
    alt    = altitudo_laterum(altitudo);
    prior  = v->repo->scriptor;
    insula_scriptorem_ponere(v->repo, chorda_ex_literis("dispensator",
        v->piscina));
    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        latus_superficiem_scribere(v, &t->sinistrum, ls, alt);
        per (k = ZEPHYRUM; k < xar_numerus(t->acervus); k++)
        {
            latus_superficiem_scribere(v,
                (VicusLatus*)xar_obtinere(t->acervus, k),
                latitudo - ls, alt);
        }
    }
    /* T4b: stratum superficiem totam habet */
    si (v->stratum)
    {
        latus_superficiem_scribere(v, v->stratum, latitudo, altitudo);
    }
    insula_scriptorem_ponere(v->repo, prior);
}

interior s32
attributum_radicis (
    InsulaRepositorium* repo,
    constans character* titulus,
                   s32  praestitutum)
{
    chorda* a;
       s32  n;

    a = insula_attributum(repo, titulus);
    si (a && chorda_ut_s32(*a, &n))
    {
        redde n;
    }
    redde praestitutum;
}

/* S7a: veritas radicis; absens = FALSUM */
interior b32
veritas_radicis (
    InsulaRepositorium* repo,
    constans character* titulus)
{
    chorda* a;

    a = insula_attributum(repo, titulus);
    redde a && chorda_aequalis_literis(*a, "verum");
}

/* S7a: campi radicis una scriptura; valor NIHIL = campus tollitur */
nomen structura {
    constans character* tituli[VI];
    constans character* valores[VI];
                   i32  numerus;
} CampiRadicis;

interior vacuum
campos_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans CampiRadicis* c;
                      i32  i;

    c = (constans CampiRadicis*)ctx;
    per (i = ZEPHYRUM; i < c->numerus; i++)
    {
        si (c->valores[i])
        {
            insula_attributum_ponere(radix, p, in, c->tituli[i],
                c->valores[i]);
        }
        alioquin
        {
            (vacuum)insula_attributum_tollere(radix, c->tituli[i]);
        }
    }
}

/* T4a: systema hospitis (vicus_systema_addere) */
nomen structura {
          chorda  titulus;
    VicusSystema  systema;
          vacuum* ctx;
} SystemaHospitis;

/* S7a: radicem ut scriptor datus mutare (actio aut systema; domini ex
 * manifesto); scriptor prior restituitur */
interior b32
radicem_mutare (
                  Vicus* v,
     constans character* scriptor,
          InsulaMutator  fn,
                 vacuum* ctx)
{
    InsulaRamus radix;
         chorda prior;
            b32 bene;

    radix = insula_ramus_radix(v->repo);
    prior = v->repo->scriptor;
    insula_scriptorem_ponere(v->repo, internare(v, scriptor));
    bene   = mutare_ramum(&radix, fn, ctx);
    insula_scriptorem_ponere(v->repo, prior);
    redde bene;
}

/* S7a: campi temporis in ramo radicis, ordine partium VicusTempus */
hic_manens constans character* campi_temporis[VI] = {
    "tempus_annus", "tempus_mensis", "tempus_dies",
    "tempus_dies_hebdomadis", "tempus_hora", "tempus_minutum"
};

interior vacuum
partes_temporis (
    VicusTempus*  t,
            s32** partes)
{
    partes[ZEPHYRUM]  = &t->annus;
    partes[I]         = &t->mensis;
    partes[II]        = &t->dies;
    partes[III]       = &t->dies_hebdomadis;
    partes[IV]        = &t->hora;
    partes[V]         = &t->minutum;
}

/* S7a: tempus ex ramo radicis; campus absens = -1 (hora -1 = nullum) */
interior vacuum
tempus_insulae (
    constans Vicus* v,
       VicusTempus* t)
{
    s32* partes[VI];
    i32  i;

    partes_temporis(t, partes);
    per (i = ZEPHYRUM; i < VI; i++)
    {
        *partes[i] = attributum_radicis(v->repo, campi_temporis[i], -I);
    }
}


/* ==================================================
 * Praefixum (T3b): Ctrl-A, deinde n / p / 1-9 / 0 / Ctrl-A
 * ================================================== */

/* Cmd+1..9, Cmd+0 = tabula (Franus 2026-10-09; olim praefixum Ctrl-A,
 * quod nunc ad latus focatum it - tmux in terminali eo utitur) */
interior b32
est_imperium_tabulae (
    constans Eventus* ev)
{
    redde ev
        && (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
            || ev->genus == EVENTUS_CLAVIS_LIBERATUS)
        && (ev->datum.clavis.modificantes & MOD_SUPER)
        && ev->datum.clavis.runa >= '0' && ev->datum.clavis.runa <= '9';
}

/* T4f: Cmd+] / Cmd+[ = cyclus lateris dextri (Franus 2026-10-17) */
interior b32
est_imperium_cycli (
    constans Eventus* ev)
{
    redde ev
        && (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
            || ev->genus == EVENTUS_CLAVIS_LIBERATUS)
        && (ev->datum.clavis.modificantes & MOD_SUPER)
        && (ev->datum.clavis.runa == ']'
        || ev->datum.clavis.runa == '[');
}

/* comites P3: Cmd+T (sine Shift, Ctrl, Alt) = liber comes */
interior b32
est_imperium_comitis (
    constans Eventus* ev)
{
    redde ev
        && (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
            || ev->genus == EVENTUS_CLAVIS_LIBERATUS)
        && (ev->datum.clavis.modificantes & MOD_SUPER)
        && !(ev->datum.clavis.modificantes
             & (MOD_SHIFT | MOD_IMPERIUM | MOD_ALT))
        && ev->datum.clavis.runa == 't';
}

/* comites P5: Cmd+F (sine Shift, Ctrl, Alt) = aditus "quaestio" */
interior b32
est_imperium_quaestionis (
    constans Eventus* ev)
{
    redde ev
        && (   ev->genus == EVENTUS_CLAVIS_DEPRESSUS
            || ev->genus == EVENTUS_CLAVIS_LIBERATUS)
        && (ev->datum.clavis.modificantes & MOD_SUPER)
        && !(ev->datum.clavis.modificantes
             & (MOD_SHIFT | MOD_IMPERIUM | MOD_ALT))
        && ev->datum.clavis.runa == 'f';
}

/* D5: brevitas hospitis eventus clavis (depressi aut liberati); NIHIL
 * si nulla */
interior constans Brevitas*
brevitatem_invenire (
      constans Vicus* v,
    constans Eventus* ev)
{
     constans Brevitas* b;
                   i32  modi;
                   i32  i;

    si (   !v || !v->brevitates || !ev
        || (   ev->genus != EVENTUS_CLAVIS_DEPRESSUS
            && ev->genus != EVENTUS_CLAVIS_LIBERATUS)
        || !(ev->datum.clavis.modificantes & MOD_SUPER))
    {
        redde NIHIL;
    }
    modi = (i32)(ev->datum.clavis.modificantes & MODI_BREVITATIS);
    per (i = ZEPHYRUM; i < xar_numerus(v->brevitates); i++)
    {
        b = (constans Brevitas*)xar_obtinere(v->brevitates, i);
        si (b->runa == ev->datum.clavis.runa && b->modificantes == modi)
        {
            redde b;
        }
    }
    redde NIHIL;
}

/* T4b: spatium strati? via "<genus>:stratum_<genus>[/...]" (gradus
 * primus; vicus solus ids "stratum_" dat) */
interior b32
spatium_strati (
    chorda sp)
{
               hic_manens constans character praefixum[] = "stratum_";
                                         i32 i;

    per (i = ZEPHYRUM; i < sp.mensura && sp.datum[i] != ':'
                       && sp.datum[i] != '/'; i++)
    {
    }
    redde i < sp.mensura && sp.datum[i] == ':'
        && sp.mensura - i - I >= (i32)(magnitudo(praefixum) - I)
        && memcmp(sp.datum + i + I, praefixum, magnitudo(praefixum) - I)
               == ZEPHYRUM;
}

/* comites P5a: prompta lateris aperta? (membrum__prompta in ramo eius;
 * nomina in piscina data) */
interior b32
prompta_lateris_aperta (
                  Vicus* v,
    constans VicusLatus* l,
                Piscina* piscina)
{
    InsulaRamus r;

    r = insula_ramus(v->repo, chorda_ut_cstr(l->genus, piscina),
        chorda_ut_cstr(l->id, piscina));
    redde prompta_aperta(&r, NIHIL);
}

/* comites P5a: destinare eventus omnes videt ante latera. Esc in
 * prompta aperta lateris advocati (focatum, dextrum): aditus
 * revocandus (latus tamen Esc accipit et promptam claudit). Ictus
 * muris aditum finit. */
interior vacuum
aditum_observare (
               Vicus* v,
    constans Eventus* ev,
             Piscina* piscina)
{
    VicusTabula* t;
     VicusLatus* l;

    t = (v && ev) ? vicus_activa(v) : NIHIL;
    si (!t || t->aditus_gradus != ADITUS_PENDENS)
    {
        redde;
    }
    si (ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        t->aditus_gradus = ADITUS_NULLUS;
        redde;
    }
    si (   ev->genus               != EVENTUS_CLAVIS_DEPRESSUS
        || ev->datum.clavis.clavis != CLAVIS_EFFUGIUM)
    {
        redde;
    }
    l = vicus_latus_focatum(v);
    t->aditus_gradus = (   t->focus == VICUS_DEXTRUM && l && l->montata
                        && chorda_aequalis(l->id, t->aditus_frons)
                        && prompta_lateris_aperta(v, l, piscina))
                     ? ADITUS_REVOCANDUS : ADITUS_NULLUS;
}

/* DestinatioStrategia hospitis (ctx = Vicus*): Cmd+numerus, Cmd+] / [
 * et brevitates (D5) ad radicem; ictus in
 * latus NON focatum (spatium destinati non sub Motus.spatium - S8c:
 * organum nidificatum in latere focato idem latus est)
 * ad radicem - radix latus focat (S2a; ictus primus lateris, si vult,
 * etiam agit); cetera geometrica (Ctrl-A ad latus focatum) */
interior Destinatio
destinare (
            Componens* arbor,
       constans Motus* motus,
               chorda  focus,
     constans Eventus* ev,
              Piscina* piscina,
               vacuum* ctx)
{
     Destinatio  d;
      Componens* c;
         chorda  sp;

    aditum_observare((Vicus*)ctx, ev, piscina);
    si (arbor && (   est_imperium_tabulae(ev) || est_imperium_cycli(ev)
                  || est_imperium_comitis(ev)
                  || est_imperium_quaestionis(ev)
                  || brevitatem_invenire((constans Vicus*)ctx, ev)))
    {
        redde destinatio_ex_componente(arbor, piscina);
    }
    d = destinatio_geometrica(arbor, motus, focus, ev, piscina);
    si (   arbor && ev && motus && ev->genus == EVENTUS_MUS_DEPRESSUS
        && !chorda_vacua(motus->spatium))
    {
        c   = destinatio_componens(&d);
        sp  = c ? componens_spatium(c) : chorda_nulla_vici();
        /* T4b: ictus in strato ei manet (focum non mutat) */
        si (   !chorda_vacua(sp)
            && !insula_via_continet(motus->spatium, sp)
            && !spatium_strati(sp))
        {
            redde destinatio_ex_componente(arbor, piscina);
        }
    }
    redde d;
}

/* S4: textus horologii - hora ("7:52 PM") aut dies ("Friday, October
 * 9th, 2026"); FALSUM si nullum tempus. Exitus >= LXIV. */
interior b32
horologii_textus (
    constans Vicus* v,
         character* exitus)
{
    hic_manens constans character* dies_nomina[VII] = {
        "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday",
        "Friday", "Saturday"
    };
    hic_manens constans character* menses[XII] = {
        "January", "February", "March", "April", "May", "June", "July",
        "August", "September", "October", "November", "December"
    };
             VicusTempus  tempus;
    constans VicusTempus* t;
      constans character* ordo;

    tempus_insulae(v, &tempus);
    t = &tempus;
    si (t->hora < ZEPHYRUM || t->minutum < ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (   !veritas_radicis(v->repo, "horologium_dies")
        || t->mensis < I || t->mensis > XII
        || t->dies_hebdomadis < ZEPHYRUM || t->dies_hebdomadis > VI)
    {
        sprintf(exitus, "%d:%02d %s",
            (integer)(t->hora % XII == ZEPHYRUM ? XII : t->hora % XII),
            (integer)t->minutum, t->hora < XII ? "AM" : "PM");
        redde VERUM;
    }
    /* 1st 2nd 3rd, 11th 12th 13th, 21st 22nd 23rd, 31st */
    ordo = (t->dies % C >= XI && t->dies % C <= XIII) ? "th"
         : t->dies % X == I ? "st" : t->dies % X == II ? "nd"
         : t->dies % X == III ? "rd" : "th";
    sprintf(exitus, "%s, %s %d%s, %d", dies_nomina[t->dies_hebdomadis],
        menses[t->mensis - I], (integer)t->dies, ordo,
        (integer)t->annus);
    redde VERUM;
}

/* S5: textus pilae ("87%", "87%+"); FALSUM si nulla. Exitus >= XVI */
interior b32
pilae_textus (
    constans Vicus* v,
         character* exitus)
{
    s32 centesimae;

    centesimae = attributum_radicis(v->repo, "pila_centesimae", -I);
    si (centesimae < ZEPHYRUM)
    {
        redde FALSUM;
    }
    sprintf(exitus, "%d%%%s", (integer)centesimae,
        veritas_radicis(v->repo, "pila_oneratur") ? "+" : "");
    redde VERUM;
}

/* S4/S5: fines horologii in linea (latitudine superficiei): ad
 * dextrum, sinistrorsum pilae textu et II spatiis motum; FALSUM si
 * nullum. x0 inclusum, x1 exclusum */
interior b32
horologii_fines (
    constans Vicus* v,
               s32  latitudo,
               s32* x0,
               s32* x1)
{
    character horae[LXIV];
    character pila[XVI];

    si (!horologii_textus(v, horae))
    {
        redde FALSUM;
    }
    *x1 = latitudo - VICUS_CELLULA_LATITUDO;
    si (pilae_textus(v, pila))
    {
        *x1 -= ((s32)strlen(pila) + II) * VICUS_CELLULA_LATITUDO;
    }
    *x0 = *x1 - (s32)strlen(horae) * VICUS_CELLULA_LATITUDO;
    redde VERUM;
}

/* T4b: insigne admonitionum ("* 3": signum, spatium, numerus) II
 * cellulis sinistrorsum horologii (aut pilae, aut marginis); FALSUM
 * si nullae non lectae (campus radicis admonitiones_non_lectae). x0
 * inclusum, x1 exclusum */
interior b32
insignis_fines (
    constans Vicus* v,
               s32  latitudo,
               s32* x0,
               s32* x1,
               s32* numerus)
{
    character pila[XVI];
    character textus[XVI];
          s32 h0;
          s32 h1;

    *numerus = attributum_radicis(v->repo, "admonitiones_non_lectae",
        ZEPHYRUM);
    si (*numerus <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    si (horologii_fines(v, latitudo, &h0, &h1))
    {
        *x1 = h0 - II * VICUS_CELLULA_LATITUDO;
    }
    alioquin
    {
        *x1 = latitudo - VICUS_CELLULA_LATITUDO;
        si (pilae_textus(v, pila))
        {
            *x1 -= ((s32)strlen(pila) + II) * VICUS_CELLULA_LATITUDO;
        }
    }
    sprintf(textus, "%d", (integer)*numerus);
    *x0 = *x1 - ((s32)strlen(textus) + II) * VICUS_CELLULA_LATITUDO;
    redde VERUM;
}

/* tabula per numerum ('1'..'9', '0' = decima); absens aut non montata:
 * nihil */
interior vacuum
tabulam_numero_ponere (
    Vicus* v,
      s32  littera)
{
    VicusTabula* t;
            s32  index;

    index = littera == '0' ? IX
          : (littera >= '1' && littera <= '9') ? littera - '1' : -I;
    si (index >= ZEPHYRUM && index < (s32)xar_numerus(v->tabulae))
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, (i32)index);
        si (tabula_montata(t))
        {
            (vacuum)vicus_activam_ponere(v,
                chorda_ut_cstr(t->id, v->piscina));
        }
    }
}

/* ictus in latus non focatum (destinare eum ad radicem misit): latus
 * sub ictu focatur. Latus ex x contra dimidium superficiei. */
interior b32
latus_ictu_focare (
               Vicus* v,
    constans Eventus* ev)
{
    s32 latitudo;

    si (ev->datum.mus.y < VICUS_ALTITUDO_TABULARUM)
    {
        redde FALSUM;
    }
    latitudo = attributum_radicis(v->repo, "superficies_latitudo",
        (s32)v->latitudo);
    redde vicus_focum_ponere(v,
        ev->datum.mus.x < latitudo_sinistri(latitudo) ? VICUS_SINISTRUM
                                                      : VICUS_DEXTRUM);
}


/* ==================================================
 * Historia tabulae (semitae R3, project-specs/semitae-spec.md VI; D5:
 * linea temporis una per tabulam, Franus 2026-10-22)
 * ================================================== */

#define HISTORIA_MAXIMA  C

/* locus historiae: latus (id, genus, argumentum, latus tabulae) et
 * semita plena (arbor duplicata - ad applicandum; NIHIL = latus sine
 * semitis; textus STML valores vacuos ut attributa nuda scribit, ergo
 * textus ad COMPARANDUM solum), textus, signum camporum 'historia
 * impelle' (mutatio eorum = locus novus) */
nomen structura {
        chorda  id;
        chorda  genus;
        chorda  argumentum;
           i32  latus;
     StmlNodus* semita;
        chorda  textus;
        chorda  signum;
} StatioHistoriae;

/* signum: valores camporum 'impelle' arboris semitae (liberi
 * nominatim) */
interior vacuum
signum_impulsus (
                   Vicus* v,
        constans Organum* o,
      constans StmlNodus* n,
       ChordaAedificator* a)
{
    i32 i;

    per (i = ZEPHYRUM; n->attributa && i < xar_numerus(n->attributa);
         i++)
    {
                StmlAttributum* t;
        constans OrganumCampus* k;

        t = (StmlAttributum*)xar_obtinere(n->attributa, i);
        k = organum_campus(o, *t->titulus);
        si (k && k->historia == ORGANUM_HISTORIA_IMPELLE && t->valor)
        {
            chorda_aedificator_appendere_chorda(a, *t->titulus);
            chorda_aedificator_appendere_character(a, '=');
            chorda_aedificator_appendere_chorda(a, *t->valor);
            chorda_aedificator_appendere_character(a, ';');
        }
    }
    per (i = ZEPHYRUM; n->liberi && i < xar_numerus(n->liberi); i++)
    {
                       StmlNodus* l;
        constans OrganumMontatio* m;
                constans Organum* lo;

        l = *(StmlNodus**)xar_obtinere(n->liberi, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM || !l->titulus)
        {
            perge;
        }
        m   = organum_montatio(o, *l->titulus);
        lo  = m ? v->organa(v->organa_ctx, m->genus) : NIHIL;
        si (lo)
        {
            chorda_aedificator_appendere_chorda(a, *l->titulus);
            chorda_aedificator_appendere_character(a, '{');
            signum_impulsus(v, lo, l, a);
            chorda_aedificator_appendere_character(a, '}');
        }
    }
}

/* status lateris: semita plena (NIHIL: latus sine semitis), textus,
 * signum 'impelle' - in scheda semitarum */
interior vacuum
statum_lateris_legere (
          Vicus*  v,
     VicusLatus*  l,
      StmlNodus** plena,
         chorda*  textus,
         chorda*  signum)
{
     constans Organum* o;
    ChordaAedificator* a;

    *plena   = NIHIL;
    *textus  = chorda_nulla_vici();
    *signum  = chorda_nulla_vici();
    si (!v->organa || !l || !l->montata || !l->facies.ramus)
    {
        redde;
    }
    *plena = semita_legere_plenam(v->organa, v->organa_ctx,
        l->facies.ramus, chorda_ut_cstr(l->genus, v->scheda_semitarum),
        v->scheda_semitarum, v->intern);
    o = v->organa(v->organa_ctx, l->genus);
    a = chorda_aedificator_creare(v->scheda_semitarum,
        (memoriae_index)LXIV);
    si (!*plena || !o || !a)
    {
        *plena = NIHIL;
        redde;
    }
    *textus = stml_scribere(*plena, v->scheda_semitarum, FALSUM);
    signum_impulsus(v, o, *plena, a);
    *signum = chorda_aedificator_finire(a);
}

/* status lateris visus (comparatio pulsus proximi) */
interior vacuum
visum_ponere (
          Vicus* v,
     VicusLatus* l,
         chorda  textus,
         chorda  signum)
{
    l->historia_textus  = chorda_transcribere(textus, v->piscina);
    l->historia_signum  = chorda_transcribere(signum, v->piscina);
    l->historia_visa    = VERUM;
}

/* locus novus in historia lateris post cursorem: futura tolluntur,
 * antiquissimus abit si plena (HISTORIA_MAXIMA) */
interior vacuum
locum_addere (
                  Vicus* v,
            VicusTabula* t,
    constans VicusLatus* l,
                    i32  latus,
              StmlNodus* plena,
                 chorda  textus,
                 chorda  signum)
{
    StatioHistoriae* novus;

    si (!t->historia[latus])
    {
        t->historia[latus] = xar_creare(v->piscina,
            (i32)magnitudo(StatioHistoriae));
        t->historia_cursor[latus] = ZEPHYRUM;
        si (!t->historia[latus])
        {
            redde;
        }
    }
    si (xar_numerus(t->historia[latus]) > ZEPHYRUM)
    {
        xar_truncare(t->historia[latus], t->historia_cursor[latus] + I);
    }
    si (xar_numerus(t->historia[latus]) >= HISTORIA_MAXIMA)
    {
        Xar* nova;
        i32  i;

        nova = xar_creare(v->piscina, (i32)magnitudo(StatioHistoriae));
        per (i = I; nova && i < xar_numerus(t->historia[latus]); i++)
        {
            *(StatioHistoriae*)xar_addere(nova) =
                *(StatioHistoriae*)xar_obtinere(t->historia[latus], i);
        }
        si (nova)
        {
            t->historia[latus] = nova;
        }
    }
    novus = (StatioHistoriae*)xar_addere(t->historia[latus]);
    si (!novus)
    {
        redde;
    }
    novus->id          = l->id;
    novus->genus       = l->genus;
    novus->argumentum  = l->argumentum;
    novus->latus       = latus;
    novus->semita      = plena ? stml_duplicare(plena, v->piscina,
                             v->intern) : NIHIL;
    novus->textus = chorda_transcribere(textus, v->piscina);
    novus->signum = chorda_transcribere(signum, v->piscina);
    t->historia_cursor[latus] = xar_numerus(t->historia[latus]) - I;
}

/* comites P3b: historia ambitus servata (non currentis) */
nomen structura {
    Xar* loci;
    i32  cursor;
} HistoriaAmbitus;

/* comites P3b: clavis tabulae ambituum - "#" + ambitus (ambitus
 * vacuus, liber communis, clavem vacuam non dat) */
interior chorda
clavis_ambitus (
      chorda  ambitus,
     Piscina* piscina)
{
    redde chorda_concatenare(chorda_ex_literis("#", piscina), ambitus,
        piscina);
}

/* comites P3b: ambitus lateris (facies.historiae_ambitus) mutatus?
 * Historia currens sub ambitu priore servatur, historia ambitus novi
 * (aut nulla: locus primus postea) currens fit. VERUM si mutatus. */
interior b32
ambitum_aptare (
          Vicus* v,
    VicusTabula* t,
            i32  latus,
     VicusLatus* l)
{
     HistoriaAmbitus* servata;
              vacuum* inventa;
              chorda  ambitus;

    si (!l || !l->montata || !l->facies.historiae_ambitus)
    {
        redde FALSUM;
    }
    ambitus =
        l->facies.historiae_ambitus(l->facies.historiae_ambitus_ctx,
        v->scheda_semitarum);
    si (chorda_aequalis(ambitus, t->historia_ambitus[latus]))
    {
        redde FALSUM;
    }
    si (!t->historiae_ambituum[latus])
    {
        t->historiae_ambituum[latus] = tabula_dispersa_creare_chorda(
            v->piscina, XVI);
        si (!t->historiae_ambituum[latus])
        {
            redde FALSUM;
        }
    }
    /* currens servatur sub ambitu priore */
    servata = (HistoriaAmbitus*)piscina_allocare(v->piscina,
        magnitudo(HistoriaAmbitus));
    si (servata)
    {
        servata->loci    = t->historia[latus];
        servata->cursor  = t->historia_cursor[latus];
        (vacuum)tabula_dispersa_inserere(t->historiae_ambituum[latus],
            clavis_ambitus(t->historia_ambitus[latus], v->piscina),
            servata);
    }
    /* novus: servata aut nulla */
    si (   tabula_dispersa_invenire(t->historiae_ambituum[latus],
               clavis_ambitus(ambitus, v->scheda_semitarum), &inventa)
        && inventa)
    {
        t->historia[latus] = ((HistoriaAmbitus*)inventa)->loci;
        t->historia_cursor[latus] =
            ((HistoriaAmbitus*)inventa)->cursor;
    }
    alioquin
    {
        t->historia[latus]         = NIHIL;
        t->historia_cursor[latus]  = ZEPHYRUM;
    }
    t->historia_ambitus[latus] = chorda_transcribere(ambitus,
        v->piscina);
    redde VERUM;
}

/* historiae tabulae notare (vicus_pulsare): status omnium laterum
 * notatur (visa); latera VISIBILIA locos faciunt in historia lateris
 * sui - sinistrum in SINISTRA, frons acervi in DEXTRA: signum
 * 'impelle' mutatum: locus novus; aequale: locus currens eius lateris
 * substituitur. Frons mutata: locus frontis in DEXTRA. Historia lateris
 * vacua: locus primus (status currens). Latus sine semitis: textus
 * vacuus. */
interior vacuum
historiam_tabulae_notare (
          Vicus* v,
    VicusTabula* t)
{
         VicusLatus* f;
         VicusLatus* l;
    StatioHistoriae* currens;
          StmlNodus* plena;
             chorda  textus;
             chorda  signum;
                b32  frons_mutata;
                b32  prima;
                b32  impulsus;
                b32  ambitus_sinister;
                b32  ambitus_dexter;
                i32  latus;
                i32  k;
                i32  n;

    f             = frons_acervi(t);
    frons_mutata  = f && f->montata
                 && !chorda_aequalis(f->id, t->historia_frons);
    n             = I + xar_numerus(t->acervus);
    /* comites P3b: ambitus mutatus (liber alius) locum non facit */
    ambitus_sinister  = ambitum_aptare(v, t, VICUS_SINISTRUM,
        &t->sinistrum);
    ambitus_dexter    = !frons_mutata
                     && ambitum_aptare(v, t, VICUS_DEXTRUM, f);
    per (k = ZEPHYRUM; k < n; k++)
    {
        l      = k == ZEPHYRUM ? &t->sinistrum
                               : (VicusLatus*)xar_obtinere(t->acervus,
                                     k - I);
        latus  = k == ZEPHYRUM ? VICUS_SINISTRUM : VICUS_DEXTRUM;
        si (!l->montata)
        {
            perge;
        }
        statum_lateris_legere(v, l, &plena, &textus, &signum);
        si (   l->historia_visa
            && chorda_aequalis(textus, l->historia_textus))
        {
            perge;
        }
        prima     = !l->historia_visa;
        impulsus  = !chorda_aequalis(signum, l->historia_signum);
        visum_ponere(v, l, textus, signum);
        si (   prima || (k > ZEPHYRUM && l != f)
            || (l == f && frons_mutata)
            || (k == ZEPHYRUM && ambitus_sinister)
            || (l == f && ambitus_dexter))
        {
            perge;
        }
        currens = (   t->historia[latus]
                   && xar_numerus(t->historia[latus]) > ZEPHYRUM)
                ? (StatioHistoriae*)xar_obtinere(t->historia[latus],
                      t->historia_cursor[latus])
                : NIHIL;
        si (!impulsus)
        {
            si (currens && chorda_aequalis(currens->id, l->id) && plena)
            {
                currens->semita = stml_duplicare(plena, v->piscina,
                    v->intern);
                currens->textus = chorda_transcribere(textus,
                    v->piscina);
            }
            perge;
        }
        locum_addere(v, t, l, latus, plena, textus, signum);
    }
    l = &t->sinistrum;
    si (   l->montata
        && (   !t->historia[VICUS_SINISTRUM]
            || xar_numerus(t->historia[VICUS_SINISTRUM]) == ZEPHYRUM))
    {
        statum_lateris_legere(v, l, &plena, &textus, &signum);
        locum_addere(v, t, l, VICUS_SINISTRUM, plena, textus, signum);
    }
    si (   f && f->montata
        && (frons_mutata || !t->historia[VICUS_DEXTRUM]
            || xar_numerus(t->historia[VICUS_DEXTRUM]) == ZEPHYRUM))
    {
        statum_lateris_legere(v, f, &plena, &textus, &signum);
        locum_addere(v, t, f, VICUS_DEXTRUM, plena, textus, signum);
        t->historia_frons = f->id;
    }
}

/* semitae R4d: semita servata (index) post montationem applicatur -
 * pulsu primo lateris (historia nondum nata), deinde semitam_secuta */
interior vacuum
semitam_restituere (
          Vicus* v,
     VicusLatus* l)
{
     StmlNodus* semita;
        chorda  vitium;

    si (   !v->organa || !l || !l->montata || !l->facies.ramus
        || l->historia_visa || l->semita_servata.mensura == ZEPHYRUM)
    {
        redde;
    }
    semita = semita_ex_uri(v->organa, v->organa_ctx, l->semita_servata,
        v->scheda_semitarum, v->intern, &vitium);
    si (   semita
        && semita_applicare(v->organa, v->organa_ctx, l->facies.ramus,
               semita, v->scheda_semitarum, &vitium)
        && l->facies.semitam_secuta)
    {
        (vacuum)l->facies.semitam_secuta(l->facies.semitam_secuta_ctx);
    }
}

/* semitae R4d: semita persistens post pulsum - mutata: servatur;
 * VERUM si mutata (index scribendus). Computatio prima (servata
 * vacua) lateris SINE argumento tacite notatur: initium sine argumento
 * idem reddit (nec actum in volumine omni initio); cum argumento
 * servatur (argumentum statum transientem ferre potest:
 * "calendarium/dies/2026-10-19") */
interior b32
semitam_servare (
          Vicus* v,
     VicusLatus* l)
{
     StmlNodus* semita;
        chorda  uri;
           b32  prima;

    si (!v->organa || !l || !l->montata || !l->facies.ramus)
    {
        redde FALSUM;
    }
    semita = semita_legere_persistentem(v->organa, v->organa_ctx,
        l->facies.ramus, chorda_ut_cstr(l->genus, v->scheda_semitarum),
        v->scheda_semitarum, v->intern);
    uri = semita ? semita_ad_uri(v->organa, v->organa_ctx, semita,
        v->scheda_semitarum) : chorda_nulla_vici();
    si (   uri.mensura == ZEPHYRUM
        || chorda_aequalis(uri, l->semita_servata))
    {
        redde FALSUM;
    }
    prima              = l->semita_servata.mensura == ZEPHYRUM
                      && l->argumentum.mensura == ZEPHYRUM;
    l->semita_servata  = internare(v, chorda_ut_cstr(uri,
        v->scheda_semitarum));
    redde !prima;
}

/* petitio historiae: locus 'meta' lateris (cursor statim movetur) */
interior b32
historiae_petitionem_addere (
            Vicus* v,
      VicusTabula* t,
              i32  latus,
              s32  meta)
{
    PetitioAcervi* p;

    p = (PetitioAcervi*)xar_addere(v->petitiones);
    si (!p)
    {
        redde FALSUM;
    }
    memset(p, ZEPHYRUM, magnitudo(PetitioAcervi));
    p->tabula                  = t->id;
    p->genus                   = chorda_nulla_vici();
    p->argumentum              = chorda_nulla_vici();
    p->historia                = VERUM;
    p->locus                   = (i32)meta;
    p->latus                   = latus;
    t->historia_cursor[latus]  = (i32)meta;
    redde VERUM;
}

/* comites P5a: aditus incipit (petitio advocans) - focus et cursor
 * historiae dextrae ante eam (revocatio eo redit) */
interior vacuum
aditum_notare (
            Vicus* v,
    PetitioAcervi* p)
{
    VicusTabula* t;

    t = vicus_activa(v);
    si (!t)
    {
        redde;
    }
    t->aditus_gradus  = ADITUS_NULLUS;
    t->aditus_latus   = t->focus;
    t->aditus_cursor  = (   t->historia[VICUS_DEXTRUM]
                         && xar_numerus(t->historia[VICUS_DEXTRUM])
                            > ZEPHYRUM)
                      ? (s32)t->historia_cursor[VICUS_DEXTRUM] : -I;
    p->aditus = VERUM;
}

/* comites P5a: petitio advocans applicata - latus advocatum (frons)
 * cum prompta aperta: aditus pendet */
interior vacuum
aditum_pendere (
                     Vicus* v,
               VicusTabula* t,
       constans VicusLatus* l,
    constans PetitioAcervi* p)
{
    si (p->aditus && l && prompta_lateris_aperta(v, l, v->piscina))
    {
        t->aditus_gradus  = ADITUS_PENDENS;
        t->aditus_frons   = l->id;
    }
}

/* comites P5a (vicus_pulsare): ante petitiones - aditus revocandus:
 * historia dextra ad cursorem priorem (si post eum est); post
 * petitiones - focus ad latus prius; aditus pendens finitur si frons,
 * focus aut prompta mutata est */
interior vacuum
aditum_revocare (
    Vicus* v)
{
    VicusTabula* t;

    t = vicus_activa(v);
    si (!t || t->aditus_gradus != ADITUS_REVOCANDUS)
    {
        redde;
    }
    t->aditus_gradus          = ADITUS_NULLUS;
    t->focus_differtus        = VERUM;
    t->focus_differtus_latus  = t->aditus_latus;
    si (   v->petitiones && t->aditus_cursor >= ZEPHYRUM
        && t->historia[VICUS_DEXTRUM]
        && t->aditus_cursor
           < (s32)xar_numerus(t->historia[VICUS_DEXTRUM])
        && (s32)t->historia_cursor[VICUS_DEXTRUM] > t->aditus_cursor)
    {
        (vacuum)historiae_petitionem_addere(v, t, VICUS_DEXTRUM,
            t->aditus_cursor);
    }
}

/* focus differtus (aditus revocatus, comes ostensus): post
 * petitiones, quae focum dextrum cogere possunt */
interior vacuum
focum_differtum_applicare (
    Vicus* v)
{
    VicusTabula* t;

    t = vicus_activa(v);
    si (t && t->focus_differtus)
    {
        t->focus_differtus = FALSUM;
        (vacuum)vicus_focum_ponere(v, t->focus_differtus_latus);
    }
}

interior vacuum
aditum_finire (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    si (!t)
    {
        redde;
    }
    l = frons_acervi(t);
    si (   t->aditus_gradus == ADITUS_PENDENS
        && (   t->focus != VICUS_DEXTRUM || !l
            || !chorda_aequalis(l->id, t->aditus_frons)
            || !prompta_lateris_aperta(v, l, v->scheda_semitarum
                   ? v->scheda_semitarum : v->piscina)))
    {
        t->aditus_gradus = ADITUS_NULLUS;
    }
}

/* Ctrl+O (-I) / Ctrl+I (+I): locus historiae lateris FOCATI tabulae
 * activae (D5) - petitio (vicus_pulsare: latus in frontem aut iterum
 * apertum, semita plena applicata, focus ad latus eius); cursor
 * statim movetur (ictus plures ante pulsum). In termino: consumpta,
 * nihil. FALSUM si historia nulla (clavis transit). */
interior b32
historiam_gradi (
    Vicus* v,
      s32  gradus)
{
      VicusTabula* t;
              s32  meta;
              i32  latus;

    t = vicus_activa(v);
    si (!v->organa || !t || !v->petitiones)
    {
        redde FALSUM;
    }
    latus = t->focus
        == VICUS_SINISTRUM ? VICUS_SINISTRUM : VICUS_DEXTRUM;
    si (   !t->historia[latus]
        || xar_numerus(t->historia[latus]) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    meta = (s32)t->historia_cursor[latus] + gradus;
    si (meta < ZEPHYRUM || meta >= (s32)xar_numerus(t->historia[latus]))
    {
        redde VERUM;
    }
    (vacuum)historiae_petitionem_addere(v, t, latus, meta);
    redde VERUM;
}

/* Ctrl+O / Ctrl+I? */
interior s32
gradus_historiae (
    constans Eventus* ev)
{
    si (   !ev || ev->genus != EVENTUS_CLAVIS_DEPRESSUS
        || !(ev->datum.clavis.modificantes & MOD_IMPERIUM))
    {
        redde ZEPHYRUM;
    }
    redde ev->datum.clavis.runa == 'o' ? -I
         : ev->datum.clavis.runa == 'i' ? I : ZEPHYRUM;
}

/* <tractator/> radicis (T2b, S2a): mutatio magnitudinis ->
 * superficies montationum; ictus in latus non focatum -> focus (et
 * ictus primus lateris); Cmd+1..9, Cmd+0 -> tabula (Franus; Cmd numquam
 * ad programma terminalis it). */
interior b32
radicem_tractare (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
          Vicus* v;
     VicusLatus* l;

    (vacuum)destinatio;
    v = (Vicus*)ctx;
    si (!ev || !v)
    {
        redde FALSUM;
    }
    si (ev->genus == EVENTUS_MUTARE_MAGNITUDINEM)
    {
        superficies_scribere(v,
            (s32)ev->datum.mutare_magnitudinem.latitudo,
            (s32)ev->datum.mutare_magnitudinem.altitudo);
        redde FALSUM;
    }
    si (ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        /* S4: ictus in horologio (a margine textus sinistro ad
         * dextrum lineae): dies <-> hora */
        si (ev->datum.mus.y < VICUS_ALTITUDO_TABULARUM)
        {
                  s32 latitudo;
                  s32 x0;
                  s32 x1;
                  s32 numerus;
         CampiRadicis c;

            latitudo = attributum_radicis(v->repo,
                "superficies_latitudo",
                (s32)v->latitudo);
            /* T4b: insigne admonitionum centrum aperit */
            si (   insignis_fines(v, latitudo, &x0, &x1, &numerus)
                && ev->datum.mus.x >= x0 && ev->datum.mus.x < x1)
            {
                (vacuum)vicus_acervo_aperire(v, "admonitiones", NIHIL);
                redde VERUM;
            }
            /* S5: area horologii sola (pila ad dextrum non commutat) */
            si (   horologii_fines(v, latitudo, &x0, &x1)
                && ev->datum.mus.x >= x0 && ev->datum.mus.x < x1)
            {
                c.tituli[ZEPHYRUM]   = "horologium_dies";
                c.valores[ZEPHYRUM]  = veritas_radicis(v->repo,
                    "horologium_dies") ? "falsum" : "verum";
                c.numerus            = I;
                (vacuum)radicem_mutare(v, "vicus.radix",
                    campos_mutator, &c);
                redde VERUM;
            }
        }
        si (!latus_ictu_focare(v, ev))
        {
            redde FALSUM;
        }
        /* ictus primus etiam agit si latus vult (scriba: iussum) */
        l = vicus_latus_focatum(v);
        si (l && l->montata && l->facies.ictus_primus)
        {
            (vacuum)l->facies.ictus_primus(l->facies.ictus_primus_ctx,
                repo, motus, nodus, ev);
        }
        redde VERUM;
    }
    /* T4d: Esc quod latus frontale acervi (focatum, effugium_redit)
     * non consumpsit: latus tollitur, prius redit */
    si (   ev->genus               == EVENTUS_CLAVIS_DEPRESSUS
        && ev->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        VicusTabula* t;

        t = vicus_activa(v);
        l = vicus_latus_focatum(v);
        si (   t && l && t->focus == VICUS_DEXTRUM && l->montata
            && l->facies.effugium_redit)
        {
            redde vicus_acervo_redire(v);
        }
    }
    /* semitae R3, D5: Ctrl+O / Ctrl+I - historia TABULAE activae
     * (latus focatum clavem non consumpsit); historia nulla: transit */
    si (gradus_historiae(ev) != ZEPHYRUM)
    {
        redde historiam_gradi(v, gradus_historiae(ev));
    }
    /* Cmd+numerus: tabula (depressa agit, liberata devoratur) */
    si (est_imperium_tabulae(ev))
    {
        si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
        {
            tabulam_numero_ponere(v, ev->datum.clavis.runa);
        }
        redde VERUM;
    }
    /* T4f: Cmd+] / Cmd+[ - gradus cycli (depressa agit, liberata
     * devoratur) */
    si (est_imperium_cycli(ev))
    {
        si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
        {
            (vacuum)vicus_cyclo_gradi(v,
                ev->datum.clavis.runa == ']' ? I : -I);
        }
        redde VERUM;
    }
    /* comites P3: Cmd+T (depressa agit, liberata devoratur) */
    si (est_imperium_comitis(ev))
    {
        si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
        {
            (vacuum)vicus_comitem_commutare(v);
        }
        redde VERUM;
    }
    /* comites P5: Cmd+F - aditus "quaestio" lateris focati (nullus:
     * devoratur, ut terminale) */
    si (est_imperium_quaestionis(ev))
    {
        si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
        {
            (vacuum)vicus_aditum_sequi(v, "quaestio");
        }
        redde VERUM;
    }
    /* D5: brevitas hospitis (depressa agit, liberata devoratur) */
    {
        constans Brevitas* b;

        b = brevitatem_invenire(v, ev);
        si (b)
        {
            si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
            {
                (vacuum)vicus_advocare(v, b->genus);
            }
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* <tractator/> tabulae (T3b): ictus = activa (titulus = id) */
interior b32
tabulam_tractare (
    InsulaRepositorium* repo,
                 Motus* motus,
   constans Destinatio* destinatio,
             Componens* nodus,
      constans Eventus* ev,
                vacuum* ctx)
{
    (vacuum)repo;
    (vacuum)motus;
    (vacuum)destinatio;
    si (!ev || !ctx || !nodus || ev->genus != EVENTUS_MUS_DEPRESSUS)
    {
        redde FALSUM;
    }
    (vacuum)vicus_activam_ponere((Vicus*)ctx,
        chorda_ut_cstr(nodus->titulus, ((Vicus*)ctx)->piscina));
    redde VERUM;
}

/* titulus tabulae in linea (S2a): id, genus frontis si a sinistro
 * differt, suffixum si latus visibile finitum. Figura et zona ictus
 * eundem legunt. */
interior chorda
titulus_tabulae (
    constans VicusTabula* t,
                 Piscina* p)
{
     VicusLatus* d;
         chorda  c;

    d = frons_acervi(t);
    c = t->id;
    si (d && !chorda_aequalis(d->genus, t->sinistrum.genus))
    {
        c = chorda_concatenare(c, chorda_ex_literis(" ", p), p);
        c = chorda_concatenare(c, d->genus, p);
    }
    si (t->sinistrum.finita || (d && d->finita))
    {
        c = chorda_concatenare(c, chorda_ex_literis(suffixum_finitae,
            p),
            p);
    }
    redde c;
}

/* latitudo tabulae in pixelis: titulus + cellula utrimque */
interior s32
latitudo_tabulae (
    constans VicusTabula* t,
                 Piscina* p)
{
    redde ((s32)titulus_tabulae(t, p).mensura + II)
        * VICUS_CELLULA_LATITUDO;
}

/* <purus/> linea tabularum: tituli, activa inversa (ctx = Vicus*) */
interior vacuum
figura_tabularum (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    constans Vicus* v;
       VicusTabula* t;
             Fines  f;
               s32  x;
               s32  lat;
               i32  i;
               b32  activa;

    (vacuum)thema;
    v           = (constans Vicus*)ctx;
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    mandata_rectangulum(m, f, figura_color_thematis(COLOR_BACKGROUND),
        VERUM);
    x = ZEPHYRUM;
    per (i = ZEPHYRUM; v && i < xar_numerus(v->tabulae); i++)
    {
        t       = (VicusTabula*)xar_obtinere(v->tabulae, i);
        lat     = latitudo_tabulae(t, m->piscina);
        activa  = chorda_aequalis(t->id, v->activa);
        si (activa)
        {
            f.x         = x;
            f.y         = ZEPHYRUM;
            f.latitudo  = lat;
            f.altitudo  = c->fines.altitudo;
            mandata_rectangulum(m, f,
                figura_color_thematis(COLOR_SELECTION),
                                VERUM);
        }
        mandata_textus(m, x + VICUS_CELLULA_LATITUDO, ZEPHYRUM,
            titulus_tabulae(t, m->piscina),
            ZEPHYRUM, figura_color_thematis(activa ? COLOR_BACKGROUND
                                            : COLOR_TEXT));
        x += lat;
    }
    /* S4: horologium ad dextrum ("7:52 PM" aut dies), cellula
     * marginis */
    si (v)
    {
        character horae[LXIV];
           chorda textus;

        character pila[XVI];
              s32 x0;
              s32 x1;

        /* S5: pila ad marginem dextrum, horologium sinistrorsum */
        si (pilae_textus(v, pila))
        {
            textus = chorda_ex_literis(pila, m->piscina);
            mandata_textus(m, c->fines.latitudo - VICUS_CELLULA_LATITUDO
                - (s32)textus.mensura * VICUS_CELLULA_LATITUDO,
                ZEPHYRUM, textus, ZEPHYRUM,
                figura_color_thematis(COLOR_TEXT));
        }
        si (   horologii_textus(v, horae)
            && horologii_fines(v, c->fines.latitudo, &x0, &x1))
        {
            textus = chorda_ex_literis(horae, m->piscina);
            mandata_textus(m, x0, ZEPHYRUM, textus, ZEPHYRUM,
                figura_color_thematis(COLOR_TEXT));
        }
        /* T4b: insigne admonitionum - signum (octagonum VI x VI) et
         * numerus non lectarum */
        {
                  s32 numerus;
              Punctum p[VIII];
            character n[XVI];

            si (insignis_fines(v, c->fines.latitudo, &x0, &x1,
                &numerus))
            {
                p[ZEPHYRUM].x  = x0 + II;
                p[ZEPHYRUM].y  = I;
                p[I].x         = x0 + IV;
                p[I].y         = I;
                p[II].x        = x0 + VI;
                p[II].y        = III;
                p[III].x       = x0 + VI;
                p[III].y       = V;
                p[IV].x        = x0 + IV;
                p[IV].y        = VII;
                p[V].x         = x0 + II;
                p[V].y         = VII;
                p[VI].x        = x0;
                p[VI].y        = V;
                p[VII].x       = x0;
                p[VII].y       = III;
                mandata_polygonum(m, p, VIII, figura_color_thematis(
                    COLOR_WARNING), VERUM);
                sprintf(n, "%d", (integer)numerus);
                mandata_textus(m, x0 + II * VICUS_CELLULA_LATITUDO,
                    ZEPHYRUM, chorda_ex_literis(n, m->piscina),
                    ZEPHYRUM,
                    figura_color_thematis(COLOR_TEXT));
            }
        }
    }
}

/* <purus/> divisor laterum (S2a): rectangulum plenum componentis sui
 * (PARTES_NULLUM hospitis; radix eandem figuram accipit et nihil
 * pingit) */
interior vacuum
figura_divisoris (
    constans Componens* c,
               Mandata* m,
                   i32  thema,
                vacuum* ctx)
{
    Fines f;

    (vacuum)thema;
    (vacuum)ctx;
    si (!chorda_aequalis_literis(c->id, id_divisoris))
    {
        redde;
    }
    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.latitudo  = c->fines.latitudo;
    f.altitudo  = c->fines.altitudo;
    mandata_rectangulum(m, f, figura_color_thematis(COLOR_BORDER),
        VERUM);
}

/* Motus ligatus (T3a, S2a): ramus, spatium et gestus lateris FOCATI.
 * Sine latere focato montato: radix, nullus gestus. */
interior vacuum
motum_aptare (
    Vicus* v)
{
    VicusLatus* l;

    si (!v->motus)
    {
        redde;
    }
    memset(&v->motus->ramus, ZEPHYRUM, magnitudo(InsulaRamus));
    v->motus->spatium = chorda_nulla_vici();
    motus_gestum_ponere(v->motus, NIHIL, NIHIL, NIHIL, ZEPHYRUM);
    l = vicus_latus_focatum(v);
    si (!l || !l->montata)
    {
        redde;
    }
    v->motus->ramus = insula_ramus(v->repo,
        chorda_ut_cstr(l->genus, v->piscina),
        chorda_ut_cstr(l->id, v->piscina));
    v->motus->spatium = v->motus->ramus.via;
    si (l->facies.gestum_ponere)
    {
        l->facies.gestum_ponere(v->motus, l->facies.gestum_ctx);
    }
}

/* relinquens (T3a): gestus et pan/zoom in ramum ADHUC activum
 * effunduntur; gestus sordidus non effusus commutationem recusat
 * (textus perderetur). Pan/zoom recusata abiciuntur - aliter in ramum
 * advenientis effunderentur. Captura et ictus pendens abiciuntur. */
interior b32
motum_relinquere (
    Vicus* v)
{
    si (!v->motus)
    {
        redde VERUM;
    }
    si (   v->motus->gestus.sordidus
        && !motus_gestum_effundere(v->motus, v->repo))
    {
        v->causa = chorda_ex_literis(
            "gestus relinquentis non effusus: commutatio recusata",
            v->piscina);
        redde FALSUM;
    }
    si (v->motus->sordida)
    {
        (vacuum)motus_effundere(v->motus, v->repo);
        v->motus->sordida = FALSUM;
    }
    motus_captura_tollere(v->motus);
    xar_vacare(v->motus->ictus_pendens);
    redde VERUM;
}

/* registra lateris in spatio suo (S2a) */
interior vacuum
registra_lateris_miscere (
         Vicus* v,
    VicusLatus* l)
{
    si (!l || !l->montata)
    {
        redde;
    }
    si (l->facies.actiones)
    {
        (vacuum)actio_registrum_miscere_in_spatio(v->actiones,
            l->facies.actiones, via_lateris(v->repo, l, v->piscina));
    }
    si (l->facies.figurae)
    {
        (vacuum)figura_registrum_miscere_in_spatio(v->figurae,
            l->facies.figurae, via_lateris(v->repo, l, v->piscina));
    }
}

/* registra hospitis: vacua, deinde hospes ("") et latera visibilia
 * activae in spatiis suis (T2a, S2a) */
interior vacuum
registra_reficere (
    Vicus* v)
{
    VicusTabula* t;

    actio_registrum_vacare(v->actiones);
    figura_registrum_vacare(v->figurae);
    (vacuum)actio_registrare(v->actiones, "vicus.radix",
                             radicem_tractare, v);
    (vacuum)actio_registrare(v->actiones, "vicus.tabula",
                             tabulam_tractare, v);
    (vacuum)figura_registrare(v->figurae, PARTES_INDEX, ZEPHYRUM,
                              figura_tabularum, v);
    (vacuum)figura_registrare(v->figurae, PARTES_NULLUM, ZEPHYRUM,
                              figura_divisoris, v);
    /* T4b: stratum in omni tabula */
    registra_lateris_miscere(v, v->stratum);
    t = tabula_invenire(v, v->activa);
    si (!t)
    {
        redde;
    }
    registra_lateris_miscere(v, &t->sinistrum);
    registra_lateris_miscere(v, frons_acervi(t));
}

/* activa et prior (T3b; prior vacua = non scribitur - valor vacuus
 * numquam scribitur) */
nomen structura {
    chorda activa;
    chorda prior;
} Activa;

interior vacuum
activam_mutator (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    constans Activa* a;

    a = (constans Activa*)ctx;
    insula_attributum_ponere(radix, p, in, "activa",
        chorda_ut_cstr(a->activa, p));
    si (a->prior.mensura > ZEPHYRUM)
    {
        insula_attributum_ponere(radix, p, in, "prior",
            chorda_ut_cstr(a->prior, p));
    }
}


/* ==================================================
 * Dispositio legenda (S2a)
 * ================================================== */

/* id vacuum = ordinarium "<tabula>_<latus>_<genus>" */
interior vacuum
latus_initiare (
          Vicus* v,
     VicusLatus* l,
         chorda  tabula,
            i32  latus,
         chorda  genus,
         chorda  id,
         chorda  argumentum)
{
    memset(l, ZEPHYRUM, magnitudo(VicusLatus));
    l->genus       = genus;
    l->id          = id.mensura > ZEPHYRUM
                   ? internare(v, chorda_ut_cstr(id, v->piscina))
                   : id_lateris(v, tabula, latus, genus);
    l->argumentum = argumentum;
    l->descriptio = genus_invenire(v, genus);
}

interior chorda
attributum_nodi (
              StmlNodus* n,
     constans character* titulus)
{
    chorda* a;

    a = n ? stml_attributum_capere(n, titulus) : NIHIL;
    redde a ? *a : chorda_nulla_vici();
}

interior chorda
genus_nodi (
    StmlNodus* n)
{
    redde attributum_nodi(n, "genus");
}

/* elementum tabula -> VicusTabula (latera nondum montata) */
interior vacuum
tabulam_legere (
          Vicus* v,
    VicusTabula* t,
      StmlNodus* n)
{
      StmlNodus* filius;
      StmlNodus* m;
     VicusLatus* d;
         chorda* a;
         chorda  sinistri;
         chorda  id_sinistri;
         chorda  semita_sinistri;
            i32  i;
            i32  k;

    memset(t, ZEPHYRUM, magnitudo(VicusTabula));
    id_sinistri      = chorda_nulla_vici();
    semita_sinistri  = chorda_nulla_vici();
    a                = stml_attributum_capere(n, "id");
    t->id            = a ? *a : chorda_nulla_vici();
    a                = stml_attributum_capere(n, "focus");
    t->focus  = (a && chorda_aequalis_literis(*a, "dextrum"))
              ? VICUS_DEXTRUM : VICUS_SINISTRUM;
    t->focus_domus = VICUS_DEXTRUM;
    t->acervus = xar_creare(v->piscina, (i32)magnitudo(VicusLatus));
    sinistri = chorda_nulla_vici();
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(n); i++)
    {
        filius = stml_liberum_ad_indicem(n, i);
        si (filius->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (chorda_aequalis_literis(*filius->titulus, "latus"))
        {
            sinistri     = genus_nodi(filius);
            id_sinistri  = attributum_nodi(filius, "id");
            /* D5: semita sinistri (scriba: pagina) */
            semita_sinistri  = attributum_nodi(filius, "semita");
        }
        alioquin si (chorda_aequalis_literis(*filius->titulus,
                         "acervus"))
        {
            per (k = ZEPHYRUM; k < stml_numerus_liberorum(filius); k++)
            {
                m = stml_liberum_ad_indicem(filius, k);
                si (   m->genus != STML_NODUS_ELEMENTUM
                    || !chorda_aequalis_literis(*m->titulus, "latus"))
                {
                    perge;
                }
                d = (VicusLatus*)xar_addere(t->acervus);
                latus_initiare(v, d, t->id, VICUS_DEXTRUM,
                    genus_nodi(m), attributum_nodi(m, "id"),
                    attributum_nodi(m, "argumentum"));
                /* semitae R4d: applicatur pulsu primo (organa vici
                 * post creationem ponuntur) */
                d->semita_servata = attributum_nodi(m, "semita");
            }
        }
    }
    latus_initiare(v, &t->sinistrum, t->id, VICUS_SINISTRUM, sinistri,
        id_sinistri, chorda_nulla_vici());
    t->sinistrum.semita_servata = semita_sinistri;
    /* acervus numquam vacuus (decisio VIII): sinistrum iteratum */
    si (xar_numerus(t->acervus) == ZEPHYRUM)
    {
        d = (VicusLatus*)xar_addere(t->acervus);
        latus_initiare(v, d, t->id, VICUS_DEXTRUM, sinistri,
            chorda_nulla_vici(), chorda_nulla_vici());
    }
}

interior vacuum
tabulam_montare (
          Vicus* v,
    VicusTabula* t)
{
    s32 ls;
    s32 alt;
    i32 k;

    ls   = latitudo_sinistri((s32)v->latitudo);
    alt  = altitudo_laterum((s32)v->altitudo);
    latus_montare(v, &t->sinistrum, ls, alt);
    per (k = ZEPHYRUM; k < xar_numerus(t->acervus); k++)
    {
        latus_montare(v, (VicusLatus*)xar_obtinere(t->acervus, k),
            (s32)v->latitudo - ls, alt);
    }
}


/* ==================================================
 * Vita
 * ================================================== */

Vicus*
vicus_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
                    i32  latitudo,
                    i32  altitudo)
{
    Vicus* v;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    v = (Vicus*)piscina_allocare(piscina, magnitudo(Vicus));
    si (!v)
    {
        redde NIHIL;
    }
    memset(v, ZEPHYRUM, magnitudo(Vicus));
    v->piscina   = piscina;
    v->intern    = intern;
    v->volumen   = volumen;
    v->latitudo  = latitudo;
    v->altitudo  = altitudo;
    v->genera    = xar_creare(piscina, (i32)magnitudo(VicusGenus));
    v->actiones  = actio_registrum_creare(piscina, intern);
    v->figurae   = figura_registrum_creare(piscina);
    v->tabulae   = xar_creare(piscina, (i32)magnitudo(VicusTabula));
    v->petitiones  = xar_creare(piscina,
        (i32)magnitudo(PetitioAcervi));
    v->systemata_hospitis = xar_creare(piscina,
        (i32)magnitudo(SystemaHospitis));
    redde v;
}

/* S4: horam legere; VERUM si mutata (aut prima). S7a: lectio in ramo
 * radicis, systema 'horologium' scribit; nullum horologium = campi
 * absentes */
interior b32
horam_legere (
    Vicus* v)
{
     VicusTempus  t;
     VicusTempus  ante;
             s32* partes[VI];
             s32* priora[VI];
       character  numeri[VI][XVI];
    CampiRadicis  c;
             b32  mutatum;
             i32  i;

    si (!v->repo)
    {
        redde FALSUM;
    }
    memset(&t, ZEPHYRUM, magnitudo(t));
    t.hora     = -I;
    t.minutum  = -I;
    si (v->horologium)
    {
        v->horologium(v->horologium_ctx, &t);
    }
    partes_temporis(&t, partes);
    si (t.hora < ZEPHYRUM || t.minutum < ZEPHYRUM)
    {
        per (i = ZEPHYRUM; i < VI; i++)
        {
            *partes[i] = -I;
        }
    }
    /* tempus totum: minutum aut dies (medianocte) mutatum */
    tempus_insulae(v, &ante);
    partes_temporis(&ante, priora);
    mutatum = FALSUM;
    per (i = ZEPHYRUM; i < VI; i++)
    {
        si (*partes[i] != *priora[i])
        {
            mutatum = VERUM;
        }
    }
    si (!mutatum)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < VI; i++)
    {
        c.tituli[i]   = campi_temporis[i];
        c.valores[i]  = NIHIL;
        si (*partes[i] >= ZEPHYRUM)
        {
            sprintf(numeri[i], "%d", (integer)*partes[i]);
            c.valores[i] = numeri[i];
        }
    }
    c.numerus = VI;
    redde radicem_mutare(v, "horologium", campos_mutator, &c);
}

/* S5: pilam legere; VERUM si mutata. S7a: in ramo radicis (systema
 * 'horologium'); nulla = campi absentes */
interior b32
pilam_legere (
    Vicus* v)
{
             s32 centesimae;
             b32 oneratur;
       character numerus[XVI];
    CampiRadicis c;

    si (!v->repo)
    {
        redde FALSUM;
    }
    centesimae  = -I;
    oneratur    = FALSUM;
    si (v->pila && !v->pila(v->pila_ctx, &centesimae, &oneratur))
    {
        centesimae = -I;
    }
    oneratur = (centesimae >= ZEPHYRUM && oneratur) ? VERUM : FALSUM;
    si (   centesimae == attributum_radicis(v->repo, "pila_centesimae",
               -I)
        && oneratur == veritas_radicis(v->repo, "pila_oneratur"))
    {
        redde FALSUM;
    }
    c.tituli[ZEPHYRUM]   = "pila_centesimae";
    c.tituli[I]          = "pila_oneratur";
    c.valores[ZEPHYRUM]  = NIHIL;
    c.valores[I]         = NIHIL;
    si (centesimae >= ZEPHYRUM)
    {
        sprintf(numerus, "%d", (integer)centesimae);
        c.valores[ZEPHYRUM]  = numerus;
        c.valores[I]         = oneratur ? "verum" : "falsum";
    }
    c.numerus = II;
    redde radicem_mutare(v, "horologium", campos_mutator, &c);
}

vacuum
vicus_pilam_ponere (
          Vicus* v,
      VicusPila  pila,
         vacuum* ctx)
{
    si (!v)
    {
        redde;
    }
    v->pila      = pila;
    v->pila_ctx  = ctx;
    (vacuum)pilam_legere(v);
}

vacuum
vicus_horologium_ponere (
              Vicus* v,
    VicusHorologium  horologium,
             vacuum* ctx)
{
    si (!v)
    {
        redde;
    }
    v->horologium      = horologium;
    v->horologium_ctx  = ctx;
    (vacuum)horam_legere(v);
}

vacuum
vicus_horologium_locale (
         vacuum* ctx,
    VicusTempus* tempus)
{
       time_t nunc;
    struct tm* t;

    (vacuum)ctx;
    nunc  = time(NIHIL);
    t     = localtime(&nunc);
    si (!t)
    {
        redde;
    }
    tempus->annus            = (s32)t->tm_year + MCM;
    tempus->mensis           = (s32)t->tm_mon + I;
    tempus->dies             = (s32)t->tm_mday;
    tempus->dies_hebdomadis  = (s32)t->tm_wday;
    tempus->hora             = (s32)t->tm_hour;
    tempus->minutum          = (s32)t->tm_min;
}

b32
vicus_genus_addere (
                  Vicus* v,
     constans character* titulus,
         memoriae_index  mensura,
          VicusMontator  montare,
        VicusDescriptor  describere,
                 vacuum* ctx)
{
     VicusGenus* g;
         chorda  t;

    si (   !v || !titulus || !montare || !describere
        || mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    t = internare(v, titulus);
    si (chorda_vacua(t) || genus_invenire(v, t))
    {
        redde FALSUM;
    }
    g              = (VicusGenus*)xar_addere(v->genera);
    g->titulus     = t;
    g->mensura     = mensura;
    g->montare     = montare;
    g->describere  = describere;
    g->ctx         = ctx;
    redde VERUM;
}

b32
vicus_stratum_addere (
                  Vicus* v,
     constans character* genus)
{
     constans VicusGenus* g;
              VicusLatus* l;
               character  id[LXIV];
                     s32  latitudo;
                     s32  altitudo;
                  chorda  prior;

    si (!v || !v->repo || v->stratum || !genus || strlen(genus) > XL)
    {
        redde FALSUM;
    }
    g = genus_invenire(v, internare(v, genus));
    l = g ? (VicusLatus*)piscina_allocare(v->piscina,
        magnitudo(VicusLatus)) : NIHIL;
    si (!l)
    {
        redde FALSUM;
    }
    memset(l, ZEPHYRUM, magnitudo(VicusLatus));
    sprintf(id, "stratum_%s", genus);
    l->genus       = g->titulus;
    l->id          = internare(v, id);
    l->argumentum  = chorda_nulla_vici();
    l->descriptio  = g;
    latitudo = attributum_radicis(v->repo, "superficies_latitudo",
        (s32)v->latitudo);
    altitudo = attributum_radicis(v->repo, "superficies_altitudo",
        (s32)v->altitudo);
    latus_montare(v, l, latitudo, altitudo);
    si (!l->montata)
    {
        redde FALSUM;
    }
    v->stratum  = l;
    prior       = v->repo->scriptor;
    insula_scriptorem_ponere(v->repo, chorda_ex_literis("dispensator",
        v->piscina));
    latus_superficiem_scribere(v, l, latitudo, altitudo);
    insula_scriptorem_ponere(v->repo, prior);
    registra_reficere(v);
    redde VERUM;
}

b32
vicus_systema_addere (
                  Vicus* v,
     constans character* titulus,
           VicusSystema  systema,
                 vacuum* ctx)
{
    SystemaHospitis* s;
             chorda  t;
                i32  i;

    si (!v || !v->systemata_hospitis || !titulus || !systema)
    {
        redde FALSUM;
    }
    t = internare(v, titulus);
    per (i = ZEPHYRUM; i < xar_numerus(v->systemata_hospitis); i++)
    {
        si (chorda_aequalis(((SystemaHospitis*)xar_obtinere(
            v->systemata_hospitis, i))->titulus, t))
        {
            redde FALSUM;
        }
    }
    s = (SystemaHospitis*)xar_addere(v->systemata_hospitis);
    si (!s)
    {
        redde FALSUM;
    }
    s->titulus  = t;
    s->systema  = systema;
    s->ctx      = ctx;
    redde VERUM;
}

b32
vicus_radicem_scribere (
                  Vicus* v,
     constans character* scriptor,
     constans character* titulus,
     constans character* valor)
{
    CampiRadicis c;

    si (!v || !v->repo || !scriptor || !titulus)
    {
        redde FALSUM;
    }
    c.tituli[ZEPHYRUM]   = titulus;
    c.valores[ZEPHYRUM]  = valor;
    c.numerus            = I;
    redde radicem_mutare(v, scriptor, campos_mutator, &c);
}

vacuum
vicus_systemata_ponere (
               Vicus* v,
     StatusSystemata* systemata)
{
    si (v)
    {
        v->systemata = systemata;
    }
}

b32
vicus_aperire (
                  Vicus* v,
     constans character* index_ordinarius)
{
          chorda  index;
             b32  inventum;
    StmlResultus  res;
       StmlNodus* n;
     VicusTabula* t;
          chorda* a;
             i32  i;
     InsulaRamus  radix;
         Organum* organum;

    si (!v || !index_ordinarius)
    {
        redde FALSUM;
    }
    v->tabulae  = xar_creare(v->piscina, (i32)magnitudo(VicusTabula));
    v->causa    = chorda_nulla_vici();
    v->repo    = insula_repositorium_creare(v->piscina, v->intern,
        "<vicus/>");
    si (!v->repo)
    {
        redde FALSUM;
    }
    /* S7b: systemata status hic creantur (actarius unus), ante radicem
     * - status radicis (capsa) servatur et restituitur */
    si (v->systemata && !v->systemata->conservatio)
    {
        v->systemata->conservatio = conservatio_creare(v->piscina,
            v->intern, v->volumen, v->repo);
        v->systemata->relatio   = relatio_creare(v->piscina, v->intern,
            v->repo);
        v->systemata->dimissio  = dimissio_creare(v->piscina, v->intern,
            v->repo);
    }
    /* S7a: radix ex manifesto hospitis (canon, domini), infixo
     * (S7c-1) */
    organum = organum_legere_genus(v->piscina, v->intern, "vicus",
        "vicus");
    radix = insula_ramus_radix(v->repo);
    si (   !organum
        || !organum_ramum_parare(organum, &radix, v->systemata, NIHIL,
               "vicus", v->piscina, v->intern))
    {
        v->causa = chorda_ex_literis("manifestum hospitis malum",
            v->piscina);
        redde FALSUM;
    }

    /* dispositio: e volumine aut ordinaria (scribitur) */
    index = volumen_praesens_legere(v->volumen,
        chorda_ex_literis(clavis_indicis, v->piscina), v->piscina,
        &inventum);
    si (!inventum)
    {
        index = chorda_ex_literis(index_ordinarius, v->piscina);
    }
    res = stml_legere_ex_literis(chorda_ut_cstr(index, v->piscina),
        v->piscina, v->intern);
    si (!res.successus || !res.elementum_radix)
    {
        v->causa = chorda_ex_literis("dispositio tabularum mala",
            v->piscina);
        redde FALSUM;
    }
    a          = stml_attributum_capere(res.elementum_radix, "activa");
    v->activa  = a ? *a : chorda_nulla_vici();
    per (i = ZEPHYRUM;
         i < stml_numerus_liberorum(res.elementum_radix)
         && xar_numerus(v->tabulae) < VICUS_TABULAE; i++)
    {
        n = stml_liberum_ad_indicem(res.elementum_radix, i);
        si (   n->genus != STML_NODUS_ELEMENTUM
            || !chorda_aequalis_literis(*n->titulus, "tabula"))
        {
            perge;
        }
        t = (VicusTabula*)xar_addere(v->tabulae);
        tabulam_legere(v, t, n);
        tabulam_montare(v, t);
    }
    /* activa: data aut prima */
    si (   !tabula_invenire(v, v->activa)
        && xar_numerus(v->tabulae) > ZEPHYRUM)
    {
        v->activa = ((VicusTabula*)xar_obtinere(v->tabulae,
            ZEPHYRUM))->id;
    }
    si (!chorda_vacua(v->activa))
    {
        Activa ac;

        ac.activa         = v->activa;
        ac.prior.mensura  = ZEPHYRUM;
        ac.prior.datum    = NIHIL;
        (vacuum)radicem_mutare(v, "vicus.radix", activam_mutator,
            &ac);
    }
    /* latera quae focum in apertura petunt (terminale): sinistrum
     * prius, deinde frons acervi */
    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        si (   t->sinistrum.montata
            && t->sinistrum.facies.focus_in_apertura)
        {
            t->focus = VICUS_SINISTRUM;
        }
        alioquin si (   frons_acervi(t) && frons_acervi(t)->montata
                     && frons_acervi(t)->facies.focus_in_apertura)
        {
            t->focus = VICUS_DEXTRUM;
        }
    }
    si (!inventum)
    {
        (vacuum)indicem_scribere(v);
    }
    superficies_scribere(v, (s32)v->latitudo, (s32)v->altitudo);
    registra_reficere(v);
    redde VERUM;
}


/* ==================================================
 * Lectio, activa, focus
 * ================================================== */

i32
vicus_numerus_tabularum (
    constans Vicus* v)
{
    redde v ? xar_numerus(v->tabulae) : ZEPHYRUM;
}

VicusTabula*
vicus_tabula (
    constans Vicus* v,
               i32  index)
{
    si (!v || index >= xar_numerus(v->tabulae))
    {
        redde NIHIL;
    }
    redde (VicusTabula*)xar_obtinere(v->tabulae, index);
}

VicusTabula*
vicus_activa (
    constans Vicus* v)
{
    redde v ? tabula_invenire(v, v->activa) : NIHIL;
}

VicusLatus*
vicus_latus (
    VicusTabula* t,
            i32  latus)
{
    si (!t)
    {
        redde NIHIL;
    }
    redde latus == VICUS_SINISTRUM ? &t->sinistrum : frons_acervi(t);
}

VicusLatus*
vicus_latus_focatum (
    constans Vicus* v)
{
    VicusTabula* t;

    t = vicus_activa(v);
    redde t ? vicus_latus(t, t->focus) : NIHIL;
}

/* comites P4: frons dextra locum recenter quaerat (facies.
 * locum_renovare) - ante Cmd+T et focum in sinistrum */
interior vacuum
frontem_monere (
    VicusTabula* t)
{
    VicusLatus* f;

    f = t ? frons_acervi(t) : NIHIL;
    si (f && f->montata && f->facies.locum_renovare)
    {
        f->facies.locum_renovare(f->facies.locum_renovare_ctx);
    }
}

b32
vicus_focum_ponere (
    Vicus* v,
      i32  latus)
{
    VicusTabula* t;
     VicusLatus* l;

    t = vicus_activa(v);
    l = (latus == VICUS_SINISTRUM || latus == VICUS_DEXTRUM)
        ? vicus_latus(t, latus) : NIHIL;
    si (!t || !l || !l->montata)
    {
        redde FALSUM;
    }
    si (t->focus == latus)
    {
        redde VERUM;
    }
    si (!motum_relinquere(v))
    {
        redde FALSUM;
    }
    t->focus = latus;
    /* comites P4: ante scripturam in comite, locus frontis recens */
    si (latus == VICUS_SINISTRUM)
    {
        frontem_monere(t);
    }
    motum_aptare(v);
    redde indicem_scribere(v);
}

b32
vicus_activam_ponere (
                  Vicus* v,
     constans character* id)
{
    VicusTabula* t;
         Activa  a;

    si (!v || !v->repo || !id)
    {
        redde FALSUM;
    }
    t = tabula_invenire(v, internare(v, id));
    si (!t)
    {
        redde FALSUM;
    }
    a.activa         = t->id;
    a.prior.mensura  = ZEPHYRUM;
    a.prior.datum    = NIHIL;
    si (!chorda_aequalis(t->id, v->activa))
    {
        si (!motum_relinquere(v))
        {
            redde FALSUM;
        }
        a.prior = v->activa;
    }
    v->activa  = t->id;
    si (!radicem_mutare(v, "vicus.radix", activam_mutator, &a))
    {
        redde FALSUM;
    }
    registra_reficere(v);
    motum_aptare(v);
    redde indicem_scribere(v);
}

b32
vicus_latus_argumentum_ponere (
                  Vicus* v,
     constans character* id,
     constans character* argumentum)
{
    VicusTabula* t;
     VicusLatus* l;
         chorda  quaesitum;
            i32  i;
            i32  k;

    si (!v || !id)
    {
        redde FALSUM;
    }
    quaesitum  = internare(v, id);
    l          = NIHIL;
    per (i = ZEPHYRUM; !l && i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        si (chorda_aequalis(t->sinistrum.id, quaesitum))
        {
            l = &t->sinistrum;
        }
        per (k = ZEPHYRUM; !l && k < xar_numerus(t->acervus); k++)
        {
            si (chorda_aequalis(((VicusLatus*)xar_obtinere(t->acervus,
                k))->id, quaesitum))
            {
                l = (VicusLatus*)xar_obtinere(t->acervus, k);
            }
        }
    }
    si (!l)
    {
        redde FALSUM;
    }
    l->argumentum = (argumentum && argumentum[ZEPHYRUM])
        ? internare(v, argumentum) : chorda_nulla_vici();
    redde indicem_scribere(v);
}

b32
vicus_latus_focare (
                  Vicus* v,
     constans character* id)
{
    VicusTabula* t;
     VicusLatus* l;
     VicusLatus  frons;
         chorda  quaesitum;
            i32  i;
            i32  k;
            i32  n;
            i32  latus;

    si (!v || !id)
    {
        redde FALSUM;
    }
    quaesitum  = internare(v, id);
    t          = NIHIL;
    l          = NIHIL;
    latus      = VICUS_SINISTRUM;
    k          = ZEPHYRUM;
    per (i = ZEPHYRUM; !l && i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        si (chorda_aequalis(t->sinistrum.id, quaesitum))
        {
            l = &t->sinistrum;
            frange;
        }
        per (k = ZEPHYRUM; k < xar_numerus(t->acervus); k++)
        {
            si (chorda_aequalis(((VicusLatus*)xar_obtinere(t->acervus,
                k))->id, quaesitum))
            {
                l      = (VicusLatus*)xar_obtinere(t->acervus, k);
                latus  = VICUS_DEXTRUM;
                frange;
            }
        }
    }
    si (   !l || !l->montata
        || !vicus_activam_ponere(v, chorda_ut_cstr(t->id, v->piscina)))
    {
        redde FALSUM;
    }
    /* in frontem acervi (ut petitio S3c) */
    n = xar_numerus(t->acervus);
    si (latus == VICUS_DEXTRUM && k < n - I)
    {
        si (!motum_relinquere(v))
        {
            redde FALSUM;
        }
        frons = *l;
        per (; k < n - I; k++)
        {
            *(VicusLatus*)xar_obtinere(t->acervus, k) =
                *(VicusLatus*)xar_obtinere(t->acervus, k + I);
        }
        *(VicusLatus*)xar_obtinere(t->acervus, n - I) = frons;
        registra_reficere(v);
        motum_aptare(v);
    }
    redde vicus_focum_ponere(v, latus) && indicem_scribere(v);
}

ActioRegistrum*
vicus_actiones (
    constans Vicus* v)
{
    redde v ? v->actiones : NIHIL;
}

FiguraRegistrum*
vicus_figurae (
    constans Vicus* v)
{
    redde v ? v->figurae : NIHIL;
}

/* latera visibilia activae, sinistrum primum: fons primus qui
 * imaginem reddit */
constans Imago*
vicus_imago_fons (
     chorda  provenientia,
     vacuum* ctx)
{
       VicusTabula* t;
        VicusLatus* l;
    constans Imago* im;
               i32  k;

    t = vicus_activa((constans Vicus*)ctx);
    per (k = VICUS_SINISTRUM; t && k <= VICUS_DEXTRUM; k++)
    {
        l = vicus_latus(t, k);
        si (!l || !l->montata || !l->facies.fons)
        {
            perge;
        }
        im = l->facies.fons(provenientia, l->facies.fons_ctx);
        si (im)
        {
            redde im;
        }
    }
    redde NIHIL;
}

/* tabulae ut zonae ictus (PARTES_NULLUM: linea eas pingit) */
interior vacuum
tabulas_componere (
                  Vicus* v,
              Componens* linea,
                Piscina* piscina,
    InternamentumChorda* intern)
{
      VicusTabula* t;
        Componens* c;
            Fines  f;
              i32  i;

    f.x         = ZEPHYRUM;
    f.y         = ZEPHYRUM;
    f.altitudo  = VICUS_ALTITUDO_TABULARUM;
    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t = (VicusTabula*)xar_obtinere(v->tabulae, i);
        c = componens_creare(piscina, intern, chorda_ut_cstr(
            chorda_concatenare(chorda_ex_literis("vicus.tabula.",
            piscina), t->id, piscina), piscina), PARTES_NULLUM);
        f.latitudo = latitudo_tabulae(t, piscina);
        componens_ponere_fines(c, f);
        componens_ponere_actio(c, "vicus.tabula");
        componens_ponere_titulum(c, chorda_ut_cstr(t->id, piscina));
        componens_addere_liberum(linea, c);
        f.x += f.latitudo;
    }
}

/* arbor lateris: componere eius, translata, spatio signata (S2a) */
interior vacuum
latus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
             VicusLatus* l,
              Componens* radix,
                    s32  x)
{
    Componens* app;
        Fines  f;

    si (!l || !l->montata || !l->facies.componere)
    {
        redde;
    }
    app = l->facies.componere(repo, motus, piscina, intern,
                              l->facies.componere_ctx);
    si (!app)
    {
        redde;
    }
    f    = app->fines;
    f.x  += x;
    f.y  += VICUS_ALTITUDO_TABULARUM;
    componens_ponere_fines(app, f);
    componens_ponere_spatium(app, chorda_ut_cstr(via_lateris(repo, l,
        piscina), piscina));
    componens_addere_liberum(radix, app);
}

/* <componens/> <purus/> */
Componens*
vicus_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
            Vicus* v;
        Componens* radix;
        Componens* linea;
        Componens* divisor;
      VicusTabula* t;
            Fines  f;
              s32  latitudo;
              s32  altitudo;
              s32  ls;

    v = (Vicus*)ctx;
    si (!v || !repo || !piscina || !intern)
    {
        redde NIHIL;
    }
    latitudo = attributum_radicis(repo, "superficies_latitudo",
        (s32)v->latitudo);
    altitudo = attributum_radicis(repo, "superficies_altitudo",
        (s32)v->altitudo);
    ls = latitudo_sinistri(latitudo);
    radix = componens_creare(piscina, intern, "vicus", PARTES_NULLUM);
    f.x = ZEPHYRUM;
    f.y = ZEPHYRUM;
    f.latitudo = latitudo;
    f.altitudo = altitudo;
    componens_ponere_fines(radix, f);
    componens_ponere_actio(radix, "vicus.radix");
    linea = componens_creare(piscina, intern, "vicus.tabulae",
                             PARTES_INDEX);
    f.altitudo = VICUS_ALTITUDO_TABULARUM;
    componens_ponere_fines(linea, f);
    componens_addere_liberum(radix, linea);
    tabulas_componere(v, linea, piscina, intern);
    t = vicus_activa(v);
    si (t)
    {
        latus_componere(repo, motus, piscina, intern, &t->sinistrum,
            radix, ZEPHYRUM);
        latus_componere(repo, motus, piscina, intern, frons_acervi(t),
            radix, ls);
        /* divisor post latera (supra ea pingitur) */
        divisor = componens_creare(piscina, intern, id_divisoris,
            PARTES_NULLUM);
        f.x         = ls;
        f.y         = VICUS_ALTITUDO_TABULARUM;
        f.latitudo  = I;
        f.altitudo  = altitudo_laterum(altitudo);
        componens_ponere_fines(divisor, f);
        componens_addere_liberum(radix, divisor);
    }
    /* T4b: stratum ultimum (supra omnia; ictus prius), non
     * translatum */
    si (   v->stratum && v->stratum->montata
        && v->stratum->facies.componere)
    {
        Componens* s;

        s = v->stratum->facies.componere(repo, motus, piscina, intern,
            v->stratum->facies.componere_ctx);
        si (s)
        {
            componens_ponere_spatium(s, chorda_ut_cstr(via_lateris(repo,
                v->stratum, piscina), piscina));
            componens_addere_liberum(radix, s);
        }
    }
    redde radix;
}

vacuum
vicus_dispensatorem_ligare (
          Vicus* v,
    Dispensator* d)
{
    si (!v || !d)
    {
        redde;
    }
    v->motus = dispensator_motus(d);
    motum_aptare(v);
    dispensator_ponere_strategiam(d, destinare, v);
}

/* latus unum pulsare; *pingendum si visibile mutatum aut nunc
 * finitum */
interior vacuum
latus_pulsare (
    VicusLatus* l,
           b32  visibile,
           b32* pingendum)
{
    VicusPulsus p;

    si (   !l || !l->montata || l->finita || !l->facies.pulsare
        || (!visibile && !l->facies.vivit_in_fundo))
    {
        redde;
    }
    p = l->facies.pulsare(l->facies.pulsare_ctx);
    si (p.mutatum && visibile)
    {
        *pingendum = VERUM;
    }
    si (p.finitus)
    {
        /* linea tabularum mutatur, quodcumque latus */
        l->finita   = VERUM;
        *pingendum  = VERUM;
    }
}

/* S3c: id lateris novi in acervo: ordinarium, aut '_2', '_3'... si
 * iam in tabula */
interior b32
id_in_tabula (
    constans VicusTabula* t,
                  chorda  id)
{
    i32 k;

    si (chorda_aequalis(t->sinistrum.id, id))
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(t->acervus); k++)
    {
        si (chorda_aequalis(((constans VicusLatus*)xar_obtinere(
                t->acervus, k))->id, id))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior chorda
id_novum (
          Vicus* v,
    VicusTabula* t,
         chorda  genus)
{
    chorda basis;
    chorda c;
       s32 n;

    basis  = id_lateris(v, t->id, VICUS_DEXTRUM, genus);
    c      = basis;
    n      = II;
    dum (id_in_tabula(t, c))
    {
        c = chorda_concatenare(basis, chorda_ex_literis("_",
            v->piscina),
            v->piscina);
        c = chorda_concatenare(c, chorda_ex_s32(n, v->piscina),
            v->piscina);
        n++;
    }
    redde c;
}

/* T4f: positio lateris (genus + argumentum) in cyclo; -I si non in
 * cyclo */
interior s32
latus_in_cyclo (
    constans Vicus* v,
            chorda  genus,
            chorda  argumentum)
{
    constans LatusCycli* c;
                    i32  k;

    per (k = ZEPHYRUM; v->cyclus && k < xar_numerus(v->cyclus); k++)
    {
        c = (constans LatusCycli*)xar_obtinere(v->cyclus, k);
        si (   chorda_aequalis(c->genus, genus)
            && chorda_aequalis(c->argumentum, argumentum))
        {
            redde (s32)k;
        }
    }
    redde -I;
}

/* T4f: domus acervi = latus summum quod in cyclo non est; nullum ->
 * imum (ZEPHYRUM) */
interior i32
domus_acervi (
          constans Vicus* v,
    constans VicusTabula* t)
{
    constans VicusLatus* l;
                    s32  k;

    per (k = (s32)xar_numerus(t->acervus) - I; k >= ZEPHYRUM; k--)
    {
        l = (constans VicusLatus*)xar_obtinere(t->acervus, (i32)k);
        si (latus_in_cyclo(v, l->genus, l->argumentum) < ZEPHYRUM)
        {
            redde (i32)k;
        }
    }
    redde ZEPHYRUM;
}

/* S3c: petitio applicata - praesens in frontem (ceteri ordinem
 * servant), aliter montatur; focus ad dextrum. Relinquens (si activa)
 * prius effunditur. FALSUM (causa) si effusio aut montatio deficit. */
interior b32
petitionem_applicare (
                     Vicus* v,
    constans PetitioAcervi* p)
{
     VicusTabula* t;
      VicusLatus* l;
      VicusLatus  frons;
          chorda  prior;
          chorda  frons_prior;
          chorda  id_novi;
             b32  domus_in_fronte;
             b32  activa;
             s32  ls;
             i32  k;
             i32  n;

    t = tabula_invenire(v, p->tabula);
    si (!t)
    {
        redde FALSUM;
    }
    /* T4f: domus in fronte nunc? (focus eius servatur si tegitur) */
    l            = frons_acervi(t);
    frons_prior  = l ? l->id : chorda_nulla_vici();
    domus_in_fronte  = l
        && domus_acervi(v, t) == xar_numerus(t->acervus) - I;
    activa = chorda_aequalis(t->id, v->activa);
    si (activa && !motum_relinquere(v))
    {
        redde FALSUM;
    }
    n = xar_numerus(t->acervus);
    /* S3e: mutare - frons generis (ultimus eius generis in acervo)
     * argumentum novum accipit et in frontem venit; focus manet.
     * Nullum: aperitio ut S3c (infra) */
    si (p->mutare)
    {
        s32 j;

        per (j = (s32)n - I; j >= ZEPHYRUM; j--)
        {
            l = (VicusLatus*)xar_obtinere(t->acervus, (i32)j);
            si (!chorda_aequalis(l->genus, p->genus))
            {
                perge;
            }
            si (   !l->montata || !l->facies.argumentum_ponere
                || !l->facies.argumentum_ponere(
                       l->facies.argumentum_ponere_ctx,
                       chorda_ut_cstr(p->argumentum, v->piscina)))
            {
                si (activa)
                {
                    motum_aptare(v);
                }
                redde FALSUM;
            }
            /* D5: argumentum transiens - identitas manet */
            si (!l->facies.argumentum_transiens)
            {
                l->argumentum = p->argumentum;
            }
            frons = *l;
            per (k = (i32)j; k < n - I; k++)
            {
                *(VicusLatus*)xar_obtinere(t->acervus, k) =
                    *(VicusLatus*)xar_obtinere(t->acervus, k + I);
            }
            *(VicusLatus*)xar_obtinere(t->acervus, n - I) = frons;
            /* comites P5: aditus advocans - focus dexter */
            si (p->advocare)
            {
                t->focus = VICUS_DEXTRUM;
            }
            si (activa)
            {
                registra_reficere(v);
                motum_aptare(v);
            }
            l = frons_acervi(t);
            si (p->advocare && l && l->montata && l->facies.advocatum)
            {
                (vacuum)l->facies.advocatum(l->facies.advocatum_ctx);
            }
            aditum_pendere(v, t, l, p);
            redde indicem_scribere(v);
        }
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        l = (VicusLatus*)xar_obtinere(t->acervus, k);
        si (   chorda_aequalis(l->genus, p->genus)
            && chorda_aequalis(l->argumentum, p->argumentum))
        {
            frons = *l;
            per (; k < n - I; k++)
            {
                *(VicusLatus*)xar_obtinere(t->acervus, k) =
                    *(VicusLatus*)xar_obtinere(t->acervus, k + I);
            }
            *(VicusLatus*)xar_obtinere(t->acervus, n - I) = frons;
            frange;
        }
    }
    si (k == n)
    {
        /* id ANTE xar_addere: locus post truncationem (Esc, T4f)
         * reddit latus vetus non vacuatum - id eius '_2' cogebat */
        id_novi  = id_novum(v, t, p->genus);
        l        = (VicusLatus*)xar_addere(t->acervus);
        si (!l)
        {
            redde FALSUM;
        }
        latus_initiare(v, l, t->id, VICUS_DEXTRUM, p->genus, id_novi,
            p->argumentum);
        ls = latitudo_sinistri((s32)v->latitudo);
        latus_montare(v, l, (s32)v->latitudo - ls,
            altitudo_laterum((s32)v->altitudo));
        si (!l->montata)
        {
            xar_truncare(t->acervus, n);
            si (activa)
            {
                motum_aptare(v);
            }
            redde FALSUM;
        }
        /* D5: argumentum transiens - montatio id accepit, identitas
         * sine argumento */
        si (l->facies.argumentum_transiens)
        {
            l->argumentum = chorda_nulla_vici();
        }
        prior = v->repo->scriptor;
        insula_scriptorem_ponere(v->repo, chorda_ex_literis(
            "dispensator", v->piscina));
        latus_superficiem_scribere(v, l, (s32)v->latitudo - ls,
            altitudo_laterum((s32)v->altitudo));
        insula_scriptorem_ponere(v->repo, prior);
        /* S3f: mutare sine latere generis - latus novum argumentum per
         * faciem ponit (pictor: picturam novam condit) */
        si (p->mutare && l->facies.argumentum_ponere)
        {
            (vacuum)l->facies.argumentum_ponere(
                l->facies.argumentum_ponere_ctx,
                chorda_ut_cstr(p->argumentum, v->piscina));
        }
    }
    l = frons_acervi(t);
    si (domus_in_fronte && l && !chorda_aequalis(l->id, frons_prior))
    {
        t->focus_domus = t->focus;
    }
    t->focus = VICUS_DEXTRUM;
    si (activa)
    {
        registra_reficere(v);
        motum_aptare(v);
    }
    /* D5: latus advocatum se parat (post frontem et focum) */
    si (p->advocare && l && l->montata && l->facies.advocatum)
    {
        (vacuum)l->facies.advocatum(l->facies.advocatum_ctx);
    }
    aditum_pendere(v, t, l, p);
    redde indicem_scribere(v);
}

/* D5: gradus historiae applicatus - latus loci (sinistrum; in acervo
 * per id: in frontem; clausum: iterum apertum per genus et
 * argumentum), semita plena applicata (semitam_secuta), focus ad
 * latus eius; status visus et frons notantur ne locus novus fiat */
interior b32
historiam_applicare (
                     Vicus* v,
    constans PetitioAcervi* p)
{
        VicusTabula* t;
    StatioHistoriae* locus;
         VicusLatus* l;
         VicusLatus* f;
         VicusLatus  frons;
      PetitioAcervi  q;
          StmlNodus* plena;
             chorda  textus;
             chorda  signum;
             chorda  vitium;
                b32  activa;
                i32  k;
                i32  n;

    t = tabula_invenire(v, p->tabula);
    si (   !t || !t->historia[p->latus] || p->locus < ZEPHYRUM
        || p->locus >= xar_numerus(t->historia[p->latus]))
    {
        redde FALSUM;
    }
    locus   = (StatioHistoriae*)xar_obtinere(t->historia[p->latus],
        p->locus);
    activa  = chorda_aequalis(t->id, v->activa);
    l       = &t->sinistrum;
    si (locus->latus == VICUS_DEXTRUM)
    {
        n = xar_numerus(t->acervus);
        per (k = ZEPHYRUM; k < n; k++)
        {
            l = (VicusLatus*)xar_obtinere(t->acervus, k);
            si (chorda_aequalis(l->id, locus->id))
            {
                frange;
            }
        }
        si (k == n)
        {
            /* latus clausum: iterum aperitur */
            memset(&q, ZEPHYRUM, magnitudo(PetitioAcervi));
            q.tabula      = t->id;
            q.genus       = locus->genus;
            q.argumentum  = locus->argumentum;
            si (!petitionem_applicare(v, &q))
            {
                redde FALSUM;
            }
            l          = frons_acervi(t);
            locus->id  = l->id;
        }
        alioquin si (k != n - I)
        {
            si (activa && !motum_relinquere(v))
            {
                redde FALSUM;
            }
            frons = *l;
            per (; k < n - I; k++)
            {
                *(VicusLatus*)xar_obtinere(t->acervus, k) =
                    *(VicusLatus*)xar_obtinere(t->acervus, k + I);
            }
            *(VicusLatus*)xar_obtinere(t->acervus, n - I) = frons;
            l = frons_acervi(t);
            si (activa)
            {
                registra_reficere(v);
                motum_aptare(v);
            }
            (vacuum)indicem_scribere(v);
        }
    }
    si (   locus->semita && l->montata && l->facies.ramus
        && semita_applicare(v->organa, v->organa_ctx, l->facies.ramus,
               locus->semita, v->scheda_semitarum, &vitium)
        && l->facies.semitam_secuta)
    {
        (vacuum)l->facies.semitam_secuta(l->facies.semitam_secuta_ctx);
    }
    si (activa)
    {
        (vacuum)vicus_focum_ponere(v, locus->latus);
    }
    alioquin
    {
        t->focus = locus->latus;
    }
    statum_lateris_legere(v, l, &plena, &textus, &signum);
    visum_ponere(v, l, textus, signum);
    f                             = frons_acervi(t);
    t->historia_frons             = f ? f->id : chorda_nulla_vici();
    t->historia_cursor[p->latus]  = p->locus;
    redde VERUM;
}

/* S3c/S3e: petitio in acervo tabulae activae addenda */
interior b32
petitionem_addere (
                  Vicus* v,
     constans character* genus,
     constans character* argumentum,
                    b32  mutare)
{
    PetitioAcervi* p;
           chorda  g;

    si (!v || !genus || !v->petitiones)
    {
        redde FALSUM;
    }
    g = internare(v, genus);
    si (chorda_vacua(g) || !genus_invenire(v, g) || !vicus_activa(v))
    {
        redde FALSUM;
    }
    p = (PetitioAcervi*)xar_addere(v->petitiones);
    si (!p)
    {
        redde FALSUM;
    }
    p->tabula  = v->activa;
    p->genus   = g;
    p->argumentum  = (argumentum && argumentum[ZEPHYRUM] != '\0')
                   ? chorda_transcribere(chorda_ex_literis(argumentum,
                         v->piscina), v->piscina)
                   : chorda_nulla_vici();
    p->mutare    = mutare;
    p->gradus    = ZEPHYRUM;
    p->advocare  = FALSUM;
    p->aditus    = FALSUM;
    p->historia  = FALSUM;
    p->locus     = ZEPHYRUM;
    redde VERUM;
}

/* T4f: gradus cycli applicatus - positio ex fronte VERA (petitiones
 * priores iam applicatae); genus domus praeteritur (latus cycli imum);
 * domus: latera supra eam tolluntur, focus domus redit */
interior b32
cyclum_applicare (
                     Vicus* v,
    constans PetitioAcervi* p)
{
      VicusTabula* t;
    PetitioAcervi  q;
       VicusLatus* domus_latus;
       VicusLatus* frons;
       LatusCycli* c;
              s32  positio;
              s32  n;
              i32  domus;
              i32  k;
              b32  activa;

    t = tabula_invenire(v, p->tabula);
    n = v->cyclus ? (s32)xar_numerus(v->cyclus) : ZEPHYRUM;
    si (!t || n == ZEPHYRUM || xar_numerus(t->acervus) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    domus        = domus_acervi(v, t);
    domus_latus  = (VicusLatus*)xar_obtinere(t->acervus, domus);
    frons        = frons_acervi(t);
    positio      = (domus == xar_numerus(t->acervus) - I) ? -I
                 : latus_in_cyclo(v, frons->genus, frons->argumentum);
    c            = NIHIL;
    per (k = ZEPHYRUM; k <= (i32)n; k++)
    {
        positio = (positio + I + p->gradus + n + I) % (n + I) - I;
        si (positio < ZEPHYRUM)
        {
            frange;
        }
        c = (LatusCycli*)xar_obtinere(v->cyclus, (i32)positio);
        si (   !chorda_aequalis(c->genus, domus_latus->genus)
            || !chorda_aequalis(c->argumentum, domus_latus->argumentum))
        {
            frange;
        }
    }
    si (positio >= ZEPHYRUM && c)
    {
        memset(&q, ZEPHYRUM, magnitudo(PetitioAcervi));
        q.tabula      = p->tabula;
        q.genus       = c->genus;
        q.argumentum  = c->argumentum;
        redde petitionem_applicare(v, &q);
    }
    activa = chorda_aequalis(t->id, v->activa);
    si (activa && !motum_relinquere(v))
    {
        redde FALSUM;
    }
    xar_truncare(t->acervus, domus + I);
    t->focus = t->focus_domus;
    si (activa)
    {
        registra_reficere(v);
        motum_aptare(v);
    }
    redde indicem_scribere(v);
}

b32
vicus_acervo_redire (
    Vicus* v)
{
    VicusTabula* t;
            i32  n;

    t = vicus_activa(v);
    n = t ? xar_numerus(t->acervus) : ZEPHYRUM;
    si (n <= I || !motum_relinquere(v))
    {
        redde FALSUM;
    }
    xar_truncare(t->acervus, n - I);
    t->focus = VICUS_DEXTRUM;
    registra_reficere(v);
    motum_aptare(v);
    redde indicem_scribere(v);
}

b32
vicus_cyclum_ponere (
                           Vicus* v,
    constans character* constans* genera,
    constans character* constans* argumenta,
                             i32  numerus)
{
                    Xar* cyclus;
             LatusCycli* c;
                 chorda  g;
     constans character* a;
                    i32  k;

    si (!v || (numerus > ZEPHYRUM && !genera))
    {
        redde FALSUM;
    }
    cyclus = xar_creare(v->piscina, (i32)magnitudo(LatusCycli));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        g = genera[k] ? internare(v, genera[k]) : chorda_nulla_vici();
        c = (!chorda_vacua(g) && genus_invenire(v, g) && cyclus)
            ? (LatusCycli*)xar_addere(cyclus) : NIHIL;
        si (!c)
        {
            redde FALSUM;
        }
        a         = argumenta ? argumenta[k] : NIHIL;
        c->genus  = g;
        c->argumentum  = (a && a[ZEPHYRUM] != '\0')
                       ? chorda_transcribere(chorda_ex_literis(a,
                             v->piscina), v->piscina)
                       : chorda_nulla_vici();
    }
    v->cyclus = numerus > ZEPHYRUM ? cyclus : NIHIL;
    redde VERUM;
}

b32
vicus_cyclo_gradi (
    Vicus* v,
      s32  directio)
{
    PetitioAcervi* p;

    si (   !v || !v->cyclus || xar_numerus(v->cyclus) == ZEPHYRUM
        || (directio != I && directio != -I) || !vicus_activa(v)
        || !v->petitiones)
    {
        redde FALSUM;
    }
    p = (PetitioAcervi*)xar_addere(v->petitiones);
    si (!p)
    {
        redde FALSUM;
    }
    memset(p, ZEPHYRUM, magnitudo(PetitioAcervi));
    p->tabula      = v->activa;
    p->genus       = chorda_nulla_vici();
    p->argumentum  = chorda_nulla_vici();
    p->gradus      = directio;
    redde VERUM;
}

b32
vicus_acervo_aperire (
                  Vicus* v,
     constans character* genus,
     constans character* argumentum)
{
    redde petitionem_addere(v, genus, argumentum, FALSUM);
}

b32
vicus_acervo_mutare (
                  Vicus* v,
     constans character* genus,
     constans character* argumentum)
{
    redde petitionem_addere(v, genus, argumentum, VERUM);
}

b32
vicus_advocare (
                  Vicus* v,
     constans character* genus)
{
    PetitioAcervi* p;

    si (!petitionem_addere(v, genus, NIHIL, FALSUM))
    {
        redde FALSUM;
    }
    p = (PetitioAcervi*)xar_obtinere(v->petitiones,
        xar_numerus(v->petitiones) - I);
    p->advocare = VERUM;
    aditum_notare(v, p);
    redde VERUM;
}

b32
vicus_aditum_sequi (
                  Vicus* v,
     constans character* titulus)
{
             VicusTabula* t;
              VicusLatus* l;
        constans Organum* o;
    constans OrganumIter* it;
           PetitioAcervi* p;
                  chorda  genus;
                  chorda  semita;
                     i32  i;

    t = v ? vicus_activa(v) : NIHIL;
    l = t ? vicus_latus(t, t->focus) : NIHIL;
    o   = (l && l->montata && v->organa)
        ? v->organa(v->organa_ctx, l->genus) : NIHIL;
    it  = o ? organum_iter(o, titulus) : NIHIL;
    si (!it)
    {
        redde FALSUM;
    }
    /* P5b: semita in codice (facies.iter_semita) manifestum supplet */
    semita = it->semita;
    si (l->facies.iter_semita)
    {
        chorda c;

        c = l->facies.iter_semita(l->facies.iter_semita_ctx, titulus,
            v->piscina);
        si (!chorda_vacua(c))
        {
            semita = c;
        }
    }
    /* semita nulla: latus ipsum (focus manet) */
    si (chorda_vacua(semita))
    {
        redde it->advocare && l->facies.advocatum
            && l->facies.advocatum(l->facies.advocatum_ctx);
    }
    genus = semita;
    per (i = ZEPHYRUM; i < genus.mensura; i++)
    {
        si (genus.datum[i] == '/' || genus.datum[i] == '?')
        {
            genus.mensura = i;
            frange;
        }
    }
    si (!petitionem_addere(v, chorda_ut_cstr(genus, v->piscina),
            chorda_ut_cstr(semita, v->piscina), VERUM))
    {
        redde FALSUM;
    }
    p = (PetitioAcervi*)xar_obtinere(v->petitiones,
        xar_numerus(v->petitiones) - I);
    p->advocare = it->advocare;
    si (p->advocare)
    {
        aditum_notare(v, p);
    }
    redde VERUM;
}

b32
vicus_brevitatem_addere (
                  Vicus* v,
                    s32  runa,
                    i32  modificantes,
     constans character* genus)
{
     Brevitas* b;
       chorda  g;
          i32  i;

    si (   !v || !genus || runa <= ZEPHYRUM
        || (modificantes != ZEPHYRUM && modificantes != (i32)MOD_SHIFT))
    {
        redde FALSUM;
    }
    si (runa >= 'A' && runa <= 'Z')
    {
        runa = runa - 'A' + 'a';
    }
    g = internare(v, genus);
    si (chorda_vacua(g) || !genus_invenire(v, g))
    {
        redde FALSUM;
    }
    si (!v->brevitates)
    {
        v->brevitates = xar_creare(v->piscina,
            (i32)magnitudo(Brevitas));
    }
    per (i = ZEPHYRUM; v->brevitates && i < xar_numerus(v->brevitates);
         i++)
    {
        b = (Brevitas*)xar_obtinere(v->brevitates, i);
        si (b->runa == runa && b->modificantes == modificantes)
        {
            redde FALSUM;
        }
    }
    b = v->brevitates ? (Brevitas*)xar_addere(v->brevitates) : NIHIL;
    si (!b)
    {
        redde FALSUM;
    }
    b->runa          = runa;
    b->modificantes  = modificantes;
    b->genus         = chorda_ut_cstr(g, v->piscina);
    redde b->genus != NIHIL;
}

/* comites P3: praefixum "comes/..." = comes activus */
interior b32
comes_activus (
    chorda praefixum)
{
    redde praefixum.mensura > V
        && memcmp(praefixum.datum, "comes/", VI) == ZEPHYRUM;
}

/* comites P5b: semita canonica lateris ut URI (in v->piscina - nota
 * comitis eam statim scribit); vacua si latus sine semitis */
interior chorda
semitam_lateris_legere (
         Vicus* v,
    VicusLatus* l)
{
    StmlNodus* semita;

    si (!v->organa || !l || !l->montata || !l->facies.ramus)
    {
        redde chorda_nulla_vici();
    }
    semita = semita_legere(v->organa, v->organa_ctx, l->facies.ramus,
        chorda_ut_cstr(l->genus, v->piscina), v->piscina, v->intern);
    redde semita ? semita_ad_uri(v->organa, v->organa_ctx, semita,
        v->piscina) : chorda_nulla_vici();
}

/* comites P3: comes frontis dextrae in sinistro ponere ("comes/" +
 * locus, "Notes · " + titulus loci) si differt a secuto (aut cogere);
 * VERUM si positus aut iam aequalis */
interior b32
comitem_sequi (
           Vicus* v,
     VicusTabula* t,
             b32  cogere)
{
    VicusLatus* s;
    VicusLatus* f;
        chorda  praefixum;
        chorda  titulus;
           b32  ok;

    s = &t->sinistrum;
    f = frons_acervi(t);
    si (!f || !f->montata || !s->montata || !s->facies.comitem_ponere)
    {
        redde FALSUM;
    }
    praefixum = chorda_concatenare(chorda_ex_literis("comes/",
        v->piscina), vicus_latus_locus(v, f, v->piscina), v->piscina);
    si (!cogere && chorda_aequalis(praefixum, t->comes_secutus))
    {
        redde VERUM;
    }
    titulus = chorda_concatenare(chorda_ex_literis("Notes \302\267 ",
        v->piscina), vicus_latus_locus_titulus(v, f, v->piscina),
        v->piscina);
    ok = s->facies.comitem_ponere(s->facies.comitem_ponere_ctx,
        praefixum, titulus, semitam_lateris_legere(v, f));
    si (ok)
    {
        t->comes_secutus = praefixum;
    }
    redde ok;
}

/* comites P3: dum comes in sinistro tabulae activae ostenditur, frontem
 * dextram sequitur (pulsus); VERUM si mutatum */
interior b32
comitem_sequendo_aptare (
    Vicus* v)
{
     VicusTabula* t;
      VicusLatus* s;
          chorda  currens;
          chorda  prius;

    t = vicus_activa(v);
    s = t ? &t->sinistrum : NIHIL;
    si (   !s || !s->montata || !s->facies.comes_currens
        || !s->facies.comitem_ponere)
    {
        redde FALSUM;
    }
    currens = s->facies.comes_currens(s->facies.comes_currens_ctx,
        v->piscina);
    si (!comes_activus(currens))
    {
        redde FALSUM;
    }
    prius = t->comes_secutus;
    (vacuum)comitem_sequi(v, t, FALSUM);
    redde !chorda_aequalis(prius, t->comes_secutus);
}

b32
vicus_pulsare (
    Vicus* v)
{
    VicusTabula* t;
     VicusLatus* d;
     VicusLatus* l;
            i32  i;
            i32  k;
            b32  activa;
            b32  pingendum;

    si (!v)
    {
        redde FALSUM;
    }
    pingendum = FALSUM;
    /* S4: horologium - quadrum solum cum minutum mutatur */
    si (horam_legere(v))
    {
        /* S5: pila cum minuto legitur */
        (vacuum)pilam_legere(v);
        pingendum = VERUM;
    }
    /* T4b: stratum semper visibile */
    latus_pulsare(v->stratum, VERUM, &pingendum);
    /* T4a: systemata hospitis */
    per (i = ZEPHYRUM; v->systemata_hospitis
                       && i < xar_numerus(v->systemata_hospitis); i++)
    {
        constans SystemaHospitis* s;

        s = (constans SystemaHospitis*)xar_obtinere(
            v->systemata_hospitis, i);
        si (s->systema(v, s->ctx))
        {
            pingendum = VERUM;
        }
    }
    /* comites P5a: aditus revocandus (Esc in prompta advocata) */
    aditum_revocare(v);
    /* S3c: aperitiones pendentes extra tractationem eventus */
    per (i = ZEPHYRUM; v->petitiones
                       && i < xar_numerus(v->petitiones); i++)
    {
        constans PetitioAcervi* p;

        p = (constans PetitioAcervi*)xar_obtinere(v->petitiones, i);
        (vacuum)(p->historia ? historiam_applicare(v, p)
               : p->gradus != ZEPHYRUM ? cyclum_applicare(v, p)
                                       : petitionem_applicare(v, p));
        pingendum = VERUM;
    }
    si (v->petitiones)
    {
        xar_vacare(v->petitiones);
    }
    focum_differtum_applicare(v);
    aditum_finire(v);
    per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
    {
        t       = (VicusTabula*)xar_obtinere(v->tabulae, i);
        activa  = chorda_aequalis(t->id, v->activa);
        d       = frons_acervi(t);
        latus_pulsare(&t->sinistrum, activa, &pingendum);
        per (k = ZEPHYRUM; k < xar_numerus(t->acervus); k++)
        {
            l = (VicusLatus*)xar_obtinere(t->acervus, k);
            latus_pulsare(l, activa && l == d, &pingendum);
        }
    }
    /* comites P3: comes sinister frontem dextram sequitur */
    si (comitem_sequendo_aptare(v))
    {
        pingendum = VERUM;
    }
    /* semitae R3: historia laterum (status post pulsum); R4d: semita
     * servata restituitur (pulsu primo) et servatur (index semel) */
    si (v->organa && v->scheda_semitarum)
    {
        b32 servanda;

        piscina_vacare(v->scheda_semitarum);
        servanda = FALSUM;
        per (i = ZEPHYRUM; i < xar_numerus(v->tabulae); i++)
        {
            t = (VicusTabula*)xar_obtinere(v->tabulae, i);
            /* D5: sinistrum semitam fert (scriba: pagina) */
            semitam_restituere(v, &t->sinistrum);
            si (semitam_servare(v, &t->sinistrum))
            {
                servanda = VERUM;
            }
            per (k = ZEPHYRUM; k < xar_numerus(t->acervus); k++)
            {
                l = (VicusLatus*)xar_obtinere(t->acervus, k);
                semitam_restituere(v, l);
                si (semitam_servare(v, l))
                {
                    servanda = VERUM;
                }
            }
            /* D5: historia tabulae (linea temporis una) */
            historiam_tabulae_notare(v, t);
        }
        si (servanda)
        {
            (vacuum)indicem_scribere(v);
        }
    }
    redde pingendum;
}

vacuum
vicus_organa_ponere (
              Vicus* v,
      SemitaOrganum  organa,
             vacuum* ctx)
{
    si (!v)
    {
        redde;
    }
    v->organa      = organa;
    v->organa_ctx  = ctx;
    si (organa && !v->scheda_semitarum)
    {
        v->scheda_semitarum = piscina_generare_dynamicum(
            "vicus_semitae", LXIV * M);
    }
}

chorda
vicus_latus_locus (
                  Vicus* v,
    constans VicusLatus* l,
                Piscina* piscina)
{
    si (!v || !l || !l->montata)
    {
        redde chorda_ex_literis("", piscina);
    }
    si (l->facies.locus)
    {
        redde l->facies.locus(l->facies.locus_ctx, piscina);
    }
    si (v->organa && l->facies.ramus)
    {
        redde semita_locus(v->organa, v->organa_ctx, l->facies.ramus,
            chorda_ut_cstr(l->genus, piscina), piscina);
    }
    /* latus sine semitis: genus */
    redde chorda_transcribere(l->genus, piscina);
}

chorda
vicus_latus_locus_titulus (
                  Vicus* v,
    constans VicusLatus* l,
                Piscina* piscina)
{
    si (!v || !l || !l->montata)
    {
        redde chorda_ex_literis("", piscina);
    }
    si (l->facies.locus_titulus)
    {
        redde l->facies.locus_titulus(l->facies.locus_titulus_ctx,
            piscina);
    }
    redde vicus_latus_locus(v, l, piscina);
}

chorda
vicus_causa (
    constans Vicus* v)
{
    redde v ? v->causa : chorda_nulla_vici();
}

b32
vicus_comitem_ostendere (
      Vicus* v,
     chorda  praefixum,
     chorda  titulus,
     chorda  semita)
{
    VicusTabula* t;
     VicusLatus* s;
     VicusLatus* f;

    t = v ? vicus_activa(v) : NIHIL;
    s = t ? &t->sinistrum : NIHIL;
    si (   !s || !s->montata || !s->facies.comitem_ponere
        || praefixum.mensura == ZEPHYRUM
        || !s->facies.comitem_ponere(s->facies.comitem_ponere_ctx,
               praefixum, titulus, semita))
    {
        redde FALSUM;
    }
    /* sequela a fronte currenti: comes non statim ad eam redit */
    f = frons_acervi(t);
    t->comes_secutus = (f && f->montata)
        ? chorda_concatenare(chorda_ex_literis("comes/", v->piscina),
              vicus_latus_locus(v, f, v->piscina), v->piscina)
        : chorda_nulla_vici();
    t->focus_differtus        = VERUM;
    t->focus_differtus_latus  = VICUS_SINISTRUM;
    redde VERUM;
}

b32
vicus_comitem_commutare (
    Vicus* v)
{
     VicusTabula* t;
      VicusLatus* s;
          chorda  currens;
             b32  ok;

    t = v ? vicus_activa(v) : NIHIL;
    s = t ? &t->sinistrum : NIHIL;
    si (   !s || !s->montata || !s->facies.comitem_ponere
        || !s->facies.comes_currens)
    {
        redde FALSUM;
    }
    currens = s->facies.comes_currens(s->facies.comes_currens_ctx,
        v->piscina);
    si (comes_activus(currens))
    {
        ok = s->facies.comitem_ponere(s->facies.comitem_ponere_ctx,
            chorda_nulla_vici(), chorda_nulla_vici(),
            chorda_nulla_vici());
        t->comes_secutus = chorda_nulla_vici();
    }
    alioquin
    {
        frontem_monere(t);
        ok = comitem_sequi(v, t, VERUM);
    }
    si (ok && chorda_aequalis(t->id, v->activa))
    {
        (vacuum)vicus_focum_ponere(v, VICUS_SINISTRUM);
    }
    alioquin si (ok)
    {
        t->focus = VICUS_SINISTRUM;
    }
    redde ok;
}
