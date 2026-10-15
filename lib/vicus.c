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
} PetitioAcervi;

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
    redde volumen_plagulam_condere(v->volumen,
        chorda_ex_literis(clavis_indicis, v->piscina),
        chorda_aedificator_finire(a), "vicus:latera");
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

/* DestinatioStrategia hospitis: Cmd+numerus ad radicem; ictus in
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
             Piscina* piscina)
{
     Destinatio  d;
      Componens* c;
         chorda  sp;

    si (arbor && est_imperium_tabulae(ev))
    {
        redde destinatio_ex_componente(arbor, piscina);
    }
    d = destinatio_geometrica(arbor, motus, focus, ev, piscina);
    si (   arbor && ev && motus && ev->genus == EVENTUS_MUS_DEPRESSUS
        && !chorda_vacua(motus->spatium))
    {
        c   = destinatio_componens(&d);
        sp  = c ? componens_spatium(c) : chorda_nulla_vici();
        si (   !chorda_vacua(sp)
            && !insula_via_continet(motus->spatium, sp))
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
         CampiRadicis c;

            latitudo = attributum_radicis(v->repo,
                "superficies_latitudo",
                (s32)v->latitudo);
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
    /* Cmd+numerus: tabula (depressa agit, liberata devoratur) */
    si (est_imperium_tabulae(ev))
    {
        si (ev->genus == EVENTUS_CLAVIS_DEPRESSUS)
        {
            tabulam_numero_ponere(v, ev->datum.clavis.runa);
        }
        redde VERUM;
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
            i32  i;
            i32  k;

    memset(t, ZEPHYRUM, magnitudo(VicusTabula));
    id_sinistri  = chorda_nulla_vici();
    a            = stml_attributum_capere(n, "id");
    t->id        = a ? *a : chorda_nulla_vici();
    a            = stml_attributum_capere(n, "focus");
    t->focus  = (a && chorda_aequalis_literis(*a, "dextrum"))
              ? VICUS_DEXTRUM : VICUS_SINISTRUM;
    t->acervus  = xar_creare(v->piscina, (i32)magnitudo(VicusLatus));
    sinistri    = chorda_nulla_vici();
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
            }
        }
    }
    latus_initiare(v, &t->sinistrum, t->id, VICUS_SINISTRUM, sinistri,
        id_sinistri, chorda_nulla_vici());
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
    index = volumen_plagulam_promere(v->volumen,
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
    dispensator_ponere_strategiam(d, destinare);
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
             b32  activa;
             s32  ls;
             i32  k;
             i32  n;

    t = tabula_invenire(v, p->tabula);
    si (!t)
    {
        redde FALSUM;
    }
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
            l->argumentum  = p->argumentum;
            frons          = *l;
            per (k = (i32)j; k < n - I; k++)
            {
                *(VicusLatus*)xar_obtinere(t->acervus, k) =
                    *(VicusLatus*)xar_obtinere(t->acervus, k + I);
            }
            *(VicusLatus*)xar_obtinere(t->acervus, n - I) = frons;
            si (activa)
            {
                registra_reficere(v);
                motum_aptare(v);
            }
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
        l = (VicusLatus*)xar_addere(t->acervus);
        si (!l)
        {
            redde FALSUM;
        }
        latus_initiare(v, l, t->id, VICUS_DEXTRUM, p->genus,
            id_novum(v, t, p->genus), p->argumentum);
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
    t->focus = VICUS_DEXTRUM;
    si (activa)
    {
        registra_reficere(v);
        motum_aptare(v);
    }
    redde indicem_scribere(v);
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
    p->mutare      = mutare;
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
    /* S3c: aperitiones pendentes extra tractationem eventus */
    per (i = ZEPHYRUM; v->petitiones
                       && i < xar_numerus(v->petitiones); i++)
    {
        (vacuum)petitionem_applicare(v,
            (constans PetitioAcervi*)xar_obtinere(v->petitiones, i));
        pingendum = VERUM;
    }
    si (v->petitiones)
    {
        xar_vacare(v->petitiones);
    }
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
    redde pingendum;
}

chorda
vicus_causa (
    constans Vicus* v)
{
    redde v ? v->causa : chorda_nulla_vici();
}
