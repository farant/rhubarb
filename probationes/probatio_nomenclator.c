/* probatio_nomenclator.c - nomenclator P2a (nomenclator-plan): index
 * personarum ordine clavis cum titulis litterarum (album), electio,
 * claves (j k [ ]), paginae (titulus litterae iteratus), mus (ordo,
 * sagittae), figurae (titulus, numerus, nomen, rubricae, societas,
 * electa). Superficies CCCLX x CCCXX: XVI ordines per paginam. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "volumen.h"
#include "insula.h"
#include "componens.h"
#include "dispensator.h"
#include "eventus.h"
#include "personae.h"
#include "nomenclator.h"
#include "figura.h"
#include "mandatum.h"
#include "thema.h"
#include "fons.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

nomen structura {
      InternamentumChorda* intern;
       InsulaRepositorium* repo;
      NomenclatorMontatio  m;
              Dispensator* d;
                  Volumen* volumen;
       PersonaeDocumentum* documentum;
} Mundus;

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    thema_initiare();
    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    x->intern   = internamentum_creare(c->piscina);
    x->volumen  = volumen_temporarium(c->piscina,
        "probatio_nomenclator");
    si (x->volumen)
    {
        x->documentum = personae_aperire(c->piscina, x->intern,
            x->volumen, "personae", ZEPHYRUM);
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

/* montare (CCCLX x CCCXX) et dispensatorem ligare */
interior b32
montare (
     Mundus* x,
    Piscina* p)
{
    Motus* motus;

    x->repo = insula_repositorium_creare(p, x->intern, "<hospes/>");
    si (   !x->repo || !x->documentum
        || !nomenclator_montare(&x->m, p, x->intern, x->repo, "n1",
               x->documentum, CCCLX, CCCXX, NIHIL))
    {
        redde FALSUM;
    }
    x->d = dispensator_creare(p, x->intern, x->repo, x->m.actiones,
        nomenclator_componere, &x->m, CCC);
    si (!x->d)
    {
        redde FALSUM;
    }
    motus         = dispensator_motus(x->d);
    motus->ramus  = x->m.ramus;
    dispensator_recomponere(x->d);
    redde VERUM;
}

interior chorda
lit (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

/* contactus fixturae: 1 Anna Rossi (familia roma), 2 Banca d'Italia
 * (societas), 3 Luca Bianchi (amici), 4 Andrea Conti, 5 3M
 * (societas) */
interior b32
contactus_parare (
     Mundus* x,
    Piscina* p)
{
    (vacuum)personae_hominem_creare(x->documentum, lit("Anna", p),
        lit("Rossi", p));
    (vacuum)personae_societatem_creare(x->documentum, lit(
        "Banca d'Italia", p));
    (vacuum)personae_hominem_creare(x->documentum, lit("Luca", p),
        lit("Bianchi", p));
    (vacuum)personae_hominem_creare(x->documentum, lit("Andrea", p),
        lit("Conti", p));
    (vacuum)personae_societatem_creare(x->documentum, lit("3M", p));
    (vacuum)personae_rubricas_ponere(x->documentum, I, lit(
        "roma familia", p));
    (vacuum)personae_rubricas_ponere(x->documentum, III, lit("amici",
        p));
    redde montare(x, p);
}

interior b32
campus_est (
                 Mundus* x,
     constans character* titulus,
     constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(&x->m.ramus, titulus);
    redde valor ? (a
        && chorda_aequalis_literis(*a, valor)) : a == NIHIL;
}

/* ordines indicis (tituli et res) ordine arboris = "l_ p5 lB ..." ? */
interior b32
ordines_sunt (
                 Mundus* x,
     constans character* exspectati)
{
     Componens* radix;
     Componens* l;
     character  visi[CCLVI];
           i32  i;

    radix           = dispensator_arbor(x->d);
    visi[ZEPHYRUM]  = '\0';
    per (i = ZEPHYRUM; radix && i < componens_numerus_liberorum(radix);
         i++)
    {
        l = componens_liberum(radix, i);
        si (   (l->partes != PARTES_INDEX && l->partes != PARTES_ITEM)
            || chorda_aequalis_literis(l->id, "caput")
            || chorda_aequalis_literis(l->id, "index_paginae"))
        {
            perge;
        }
        si (visi[ZEPHYRUM])
        {
            strcat(visi, " ");
        }
        strncat(visi, (constans character*)l->id.datum,
            (size_t)l->id.mensura);
    }
    si (strcmp(visi, exspectati) != ZEPHYRUM)
    {
        imprimere("  ordines: '%s'\n", visi);
        redde FALSUM;
    }
    redde VERUM;
}

interior vacuum
clavis (
       Mundus* x,
    character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                  = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.producta  = (s32)(insignatus character)typus;
    e.datum.clavis.clavis    = (clavis_t)typus;
    e.datum.clavis.runa      = (s32)typus;
    dispensator_tractare(x->d, &e);
}

interior Fines
fines_componentis (
                 Mundus* x,
     constans character* id,
                Piscina* p)
{
     Componens* c;
         Fines  f;

    memset(&f, ZEPHYRUM, magnitudo(f));
    c = componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis(id, p));
    si (c)
    {
        f = c->fines;
    }
    redde f;
}

interior b32
adest (
                 Mundus* x,
     constans character* id,
                Piscina* p)
{
    redde fines_componentis(x, id, p).altitudo > ZEPHYRUM;
}

interior b32
ictus_in (
                 Mundus* x,
     constans character* id,
                Piscina* p)
{
      Fines f;
    Eventus e;

    f = fines_componentis(x, id, p);
    si (f.altitudo <= ZEPHYRUM)
    {
        redde FALSUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_MUS_DEPRESSUS;
    e.datum.mus.x       = f.x + f.latitudo / II;
    e.datum.mus.y       = f.y + f.altitudo / II;
    e.datum.mus.botton  = MUS_SINISTER;
    dispensator_tractare(x->d, &e);
    redde VERUM;
}

interior Mandata*
reddere (
     Mundus* x,
    Piscina* p)
{
    Mandata* md;

    md = mandata_creare(p, x->intern);
    pingere(dispensator_arbor(x->d), x->m.figurae, ZEPHYRUM, md);
    redde md;
}

interior b32
punctum_intra (
    Fines a,
    Fines f)
{
    redde a.x >= f.x && a.y >= f.y && a.x < f.x + f.latitudo
        && a.y < f.y + f.altitudo;
}

/* textus partem continens in finibus? (fons: -1 = quivis) */
interior b32
textus_in (
                 Mandata* md,
                   Fines  f,
      constans character* pars,
                     s32  fons)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_TEXTUS && punctum_intra(x->fines, f)
            && chorda_continet(x->textus, chorda_ex_literis(pars,
                   md->piscina))
            && (fons < ZEPHYRUM || (s32)x->fons == fons))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* rectangulum impletum coloris COLOR_SELECTION in finibus? */
interior b32
electum_in (
     Mandata* md,
       Fines  f)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus       == MANDATUM_RECTANGULUM && x->impletum
            && x->color.genus == COLOR_MANDATI_THEMA
            && x->color.valor == (i32)COLOR_SELECTION
            && punctum_intra(x->fines, f))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* clavis nominata (Enter, Esc) aut Ctrl + littera */
interior vacuum
clavis_specialis (
      Mundus* x,
    clavis_t  codex,
         s32  producta,
         s32  runa,
         i32  modificantes)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                      = EVENTUS_CLAVIS_DEPRESSUS;
    e.datum.clavis.clavis        = codex;
    e.datum.clavis.producta      = producta;
    e.datum.clavis.runa          = runa;
    e.datum.clavis.modificantes  = modificantes;
    dispensator_tractare(x->d, &e);
}

interior vacuum
typare (
                 Mundus* x,
     constans character* s)
{
    per (; *s; s++)
    {
        clavis(x, *s);
    }
}

interior vacuum
reditus (
    Mundus* x)
{
    clavis_specialis(x, CLAVIS_REDITUS, XIII, ZEPHYRUM, ZEPHYRUM);
}

interior vacuum
effugium (
    Mundus* x)
{
    clavis_specialis(x, CLAVIS_EFFUGIUM, XXVII, ZEPHYRUM, ZEPHYRUM);
}

/* persona id: nomina (praenomen|cognomen|titulus) = "a|b|c" ? */
interior b32
persona_est (
                 Mundus* x,
                    s32  id,
     constans character* exspectata)
{
    constans Persona* q;
           character  visa[CXXVIII];

    q = personae_persona(x->documentum, id);
    si (!q)
    {
        redde FALSUM;
    }
    sprintf(visa, "%.*s|%.*s|%.*s", (integer)q->praenomen.mensura,
        q->praenomen.datum ? (constans character*)q->praenomen.datum
        : "",
        (integer)q->cognomen.mensura, q->cognomen.datum
        ? (constans character*)q->cognomen.datum : "",
        (integer)q->titulus.mensura, q->titulus.datum
        ? (constans character*)q->titulus.datum : "");
    si (strcmp(visa, exspectata) != ZEPHYRUM)
    {
        imprimere("  persona %d: '%s'\n", (integer)id, visa);
        redde FALSUM;
    }
    redde VERUM;
}

/* montatio: ramus, focus in radice, electa = prima ordinis */
interior vacuum
_probare_montationem (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x->documentum);
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_NON_NIHIL(x->m.ramus.repo);
    CREDO_VERUM(x->m.documentum == x->documentum);
    CREDO_AEQUALIS_S32(nomenclator_persona_electa(&x->m), V);
    CREDO_VERUM(adest(x, "nomenclator", c->piscina));
}

/* ordo: clavis ("3m" < "banca" < "bianchi" < "conti" < "rossi"),
 * tituli litterarum ("#" = "l_") */
interior vacuum
_probare_ordinem (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_VERUM(ordines_sunt(x, "l_ p5 lB p2 p3 lC p4 lR p1"));
}

/* claves: j k movent; in fine manet; Enter nihil (P5) */
interior vacuum
_probare_claves (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "persona_electa", "2"));
    clavis(x, 'j');
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "persona_electa", "4"));
    clavis(x, 'k');
    CREDO_VERUM(campus_est(x, "persona_electa", "3"));
    clavis(x, 'j');
    clavis(x, 'j');
    clavis(x, 'j');
    CREDO_AEQUALIS_S32(nomenclator_persona_electa(&x->m), I);
}

/* paginae: XX "R..." - pagina I lR + XV, pagina II "lR-c" + V; ]
 * eligit primam paginae; j ex ultima paginae I paginam vertit */
interior vacuum
_probare_paginas (
    CredoContextus* c)
{
       Mundus* x;
    character  cognomen[XVI];
          i32  i;

    x = (Mundus*)c->datum;
    per (i = ZEPHYRUM; i < XX; i++)
    {
        sprintf(cognomen, "Rossi%c", (character)('a' + i));
        (vacuum)personae_hominem_creare(x->documentum, lit("Anna",
            c->piscina), lit(cognomen, c->piscina));
    }
    CREDO_NECESSE(montare(x, c->piscina));
    CREDO_VERUM(adest(x, "lR", c->piscina));
    CREDO_VERUM(adest(x, "p15", c->piscina));
    CREDO_FALSUM(adest(x, "p16", c->piscina));
    clavis(x, ']');
    CREDO_VERUM(adest(x, "lR-c", c->piscina));
    CREDO_VERUM(adest(x, "p16", c->piscina));
    CREDO_VERUM(adest(x, "p20", c->piscina));
    CREDO_VERUM(campus_est(x, "persona_electa", "16"));
    clavis(x, '[');
    CREDO_VERUM(adest(x, "p1", c->piscina));
    CREDO_VERUM(campus_est(x, "persona_electa", "1"));
    /* j ex p15 (ultima paginae I): p16, pagina II */
    per (i = ZEPHYRUM; i < XIV; i++)
    {
        clavis(x, 'j');
    }
    CREDO_VERUM(campus_est(x, "persona_electa", "15"));
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "persona_electa", "16"));
    CREDO_VERUM(adest(x, "p16", c->piscina));
    CREDO_FALSUM(adest(x, "p1", c->piscina));
}

/* mus: ictus in ordine eligit; sagitta paginam vertit */
interior vacuum
_probare_murem (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_NECESSE(ictus_in(x, "p4", c->piscina));
    CREDO_VERUM(campus_est(x, "persona_electa", "4"));
    /* titulus litterae: nihil */
    CREDO_NECESSE(ictus_in(x, "lB", c->piscina));
    CREDO_VERUM(campus_est(x, "persona_electa", "4"));
    CREDO_VERUM(adest(x, "pagina-proxima", c->piscina));
}

/* figurae: titulus 8x16, numerus, nomen et rubricae in ordine,
 * societas signata, electa fundo selectionis */
interior vacuum
_probare_figuras (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, fines_componentis(x, "titulus",
        c->piscina), "Contacts", FONS_TITULI));
    CREDO_VERUM(textus_in(md, fines_componentis(x, "caput", c->piscina),
        "5 contacts", -I));
    CREDO_VERUM(textus_in(md, fines_componentis(x, "p1", c->piscina),
        "Anna Rossi", -I));
    CREDO_VERUM(textus_in(md, fines_componentis(x, "p1", c->piscina),
        "familia roma", -I));
    CREDO_VERUM(textus_in(md, fines_componentis(x, "lB", c->piscina),
        "B", -I));
    /* electa prima (5): fundus selectionis; aliae non */
    CREDO_VERUM(electum_in(md, fines_componentis(x, "p5", c->piscina)));
    CREDO_FALSUM(electum_in(md, fines_componentis(x, "p1",
        c->piscina)));
}

/* nova persona (n): verbum ultimum cognomen, cetera praenomen; verbum
 * unum praenomen; vacuum recusatum (prompta manet); Esc claudit; nova
 * electa */
interior vacuum
_probare_novum_hominem (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    clavis(x, 'n');
    CREDO_VERUM(campus_est(x, "prompta", "homo"));
    typare(x, "Maria Grazia Verdi");
    reditus(x);
    CREDO_VERUM(persona_est(x, VI, "Maria Grazia|Verdi|"));
    CREDO_FALSUM(campus_est(x, "prompta", "homo"));
    CREDO_VERUM(campus_est(x, "persona_electa", "6"));
    clavis(x, 'n');
    typare(x, "Cher");
    reditus(x);
    CREDO_VERUM(persona_est(x, VII, "Cher||"));
    /* vacuum: recusatum, prompta manet; Esc claudit */
    clavis(x, 'n');
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "homo"));
    CREDO_AEQUALIS_I32(personae_numerus(x->documentum), VII);
    effugium(x);
    CREDO_FALSUM(campus_est(x, "prompta", "homo"));
}

/* nova societas (N); nova in pagina altera electa */
interior vacuum
_probare_novam_societatem (
    CredoContextus* c)
{
       Mundus* x;
    character  cognomen[XVI];
          i32  i;
       chorda* s;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    clavis(x, 'N');
    CREDO_VERUM(campus_est(x, "prompta", "societas"));
    typare(x, "Acme");
    reditus(x);
    CREDO_VERUM(persona_est(x, VI, "||Acme"));
    CREDO_NECESSE_NON_NIHIL(personae_persona(x->documentum, VI));
    CREDO_VERUM(personae_persona(x->documentum, VI)->genus
        == PERSONA_SOCIETAS);
    CREDO_VERUM(campus_est(x, "persona_electa", "6"));
    /* XX "Rossi*" (paginae II): "Zeta Zulu" in pagina II electa */
    per (i = ZEPHYRUM; i < XX; i++)
    {
        sprintf(cognomen, "Rossi%c", (character)('a' + i));
        (vacuum)personae_hominem_creare(x->documentum, lit("Anna",
            c->piscina), lit(cognomen, c->piscina));
    }
    clavis(x, 'n');
    typare(x, "Zeta Zulu");
    reditus(x);
    s = insula_ramus_attributum(&x->m.ramus, "pagina");
    CREDO_VERUM(s && !chorda_aequalis_literis(*s, "0"));
    CREDO_VERUM(adest(x, "p27", c->piscina));
}

/* delere (X): electa sequens (ultima: prior); revocare u, reficere
 * C-r */
interior vacuum
_probare_deletionem (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    /* ordo: 5 2 3 4 1 */
    CREDO_NECESSE(ictus_in(x, "p2", c->piscina));
    clavis(x, 'X');
    CREDO_NIHIL(personae_persona(x->documentum, II));
    CREDO_VERUM(campus_est(x, "persona_electa", "3"));
    CREDO_NECESSE(ictus_in(x, "p1", c->piscina));
    clavis(x, 'X');
    CREDO_NIHIL(personae_persona(x->documentum, I));
    CREDO_VERUM(campus_est(x, "persona_electa", "4"));
    clavis(x, 'u');
    CREDO_NON_NIHIL(personae_persona(x->documentum, I));
    clavis_specialis(x, (clavis_t)'r', ZEPHYRUM, 'r',
        (i32)MOD_IMPERIUM);
    CREDO_NIHIL(personae_persona(x->documentum, I));
}

/* prompta picta: titulus "name:" in componente prompta */
interior vacuum
_probare_promptam (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_FALSUM(adest(x, "prompta", c->piscina));
    clavis(x, 'n');
    typare(x, "Jo");
    CREDO_VERUM(adest(x, "prompta", c->piscina));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, fines_componentis(x, "prompta",
        c->piscina), "name:", -I));
    CREDO_VERUM(textus_in(md, fines_componentis(x, "prompta",
        c->piscina), "Jo", -I));
    effugium(x);
    clavis(x, 'N');
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, fines_componentis(x, "prompta",
        c->piscina), "business:", -I));
}

/* menu "+ New" (P2b-2): globulus aperit infra se; res "New person" /
 * "New business" cum clavibus; ictus in "New business" promptam
 * societatis aperit et menu claudit; Esc claudit */
interior vacuum
_probare_menu_novum (
    CredoContextus* c)
{
        Mundus* x;
     Componens* e;
     Componens* r;
         Fines  globulus;
       Eventus  ev;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_VERUM(adest(x, "nova", c->piscina));
    globulus = fines_componentis(x, "nova", c->piscina);
    CREDO_NECESSE(ictus_in(x, "nova", c->piscina));
    e = componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("elenchus", c->piscina));
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(e->fines.y >= globulus.y + globulus.altitudo);
    CREDO_AEQUALIS_I32(componens_numerus_liberorum(e), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(componens_liberum(e,
        ZEPHYRUM)->titulus,
        "New person");
    r = componens_liberum(e, I);
    CREDO_CHORDA_AEQUALIS_LITERIS(r->titulus, "New business");
    /* ictus in "New business": prompta societatis, menu clausum */
    memset(&ev, ZEPHYRUM, magnitudo(Eventus));
    ev.genus = EVENTUS_MUS_DEPRESSUS;
    ev.datum.mus.x = e->fines.x + r->fines.x + r->fines.latitudo
        / II;
    ev.datum.mus.y = e->fines.y + r->fines.y + r->fines.altitudo
        / II;
    ev.datum.mus.botton = MUS_SINISTER;
    dispensator_tractare(x->d, &ev);
    CREDO_VERUM(campus_est(x, "prompta", "societas"));
    CREDO_FALSUM(adest(x, "elenchus", c->piscina));
    /* Esc: claudit */
    effugium(x);
    CREDO_NECESSE(ictus_in(x, "nova", c->piscina));
    CREDO_VERUM(adest(x, "elenchus", c->piscina));
    effugium(x);
    CREDO_FALSUM(adest(x, "elenchus", c->piscina));
}

/* visus rubricarum (g): greges ordine canonico, "(none)" ultimus;
 * persona in gregibus pluribus pluries ("p<id>-<k>") */
interior vacuum
_probare_greges (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    clavis(x, 'g');
    CREDO_VERUM(campus_est(x, "visus", "rubricae"));
    /* amici: 3; familia: 1; roma: 1; (none): 5 2 4 */
    CREDO_VERUM(ordines_sunt(x, "g0 p3-0 g1 p1-1 g2 p1-2 g3 p5-3 p2-3"
        " p4-3"));
    clavis(x, 'g');
    CREDO_VERUM(campus_est(x, "visus", "index"));
    CREDO_VERUM(ordines_sunt(x, "l_ p5 lB p2 p3 lC p4 lR p1"));
}

/* filtrum: ictus in titulo gregis filtrat (iterum tollit); f prompta;
 * filtrum in indice; Esc tollit; malum recusatum */
interior vacuum
_probare_filtrum (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    clavis(x, 'g');
    CREDO_NECESSE(ictus_in(x, "g1", c->piscina));
    CREDO_VERUM(campus_est(x, "rubrica_filtrum", "familia"));
    CREDO_VERUM(ordines_sunt(x, "g0 p1-0"));
    CREDO_VERUM(campus_est(x, "persona_electa", "1"));
    CREDO_NECESSE(ictus_in(x, "g0", c->piscina));
    CREDO_VERUM(campus_est(x, "rubrica_filtrum", NIHIL));
    /* f amici, Enter; visus indicis filtratus */
    clavis(x, 'f');
    CREDO_VERUM(campus_est(x, "prompta", "filtrum"));
    typare(x, "amici");
    reditus(x);
    CREDO_VERUM(campus_est(x, "rubrica_filtrum", "amici"));
    clavis(x, 'g');
    CREDO_VERUM(ordines_sunt(x, "lB p3"));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, fines_componentis(x, "caput", c->piscina),
        "1 of 5 contacts", -I));
    /* Esc: filtrum tollit */
    effugium(x);
    CREDO_VERUM(campus_est(x, "rubrica_filtrum", NIHIL));
    CREDO_VERUM(ordines_sunt(x, "l_ p5 lB p2 p3 lC p4 lR p1"));
    /* malum: prompta manet */
    clavis(x, 'f');
    typare(x, "Amici");
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "filtrum"));
    CREDO_VERUM(campus_est(x, "rubrica_filtrum", NIHIL));
    effugium(x);
}

/* segmenta All | Tags: ictus visum mutat */
interior vacuum
_probare_segmenta (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_NECESSE(ictus_in(x, "visus-rubricarum", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "rubricae"));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, fines_componentis(x, "g0", c->piscina),
        "#amici", -I));
    CREDO_VERUM(textus_in(md, fines_componentis(x, "g3", c->piscina),
        "(none)", -I));
    CREDO_NECESSE(ictus_in(x, "visus-indicis", c->piscina));
    CREDO_VERUM(campus_est(x, "visus", "index"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "montatio", _probare_montationem, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "ordo", _probare_ordinem, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "claves", _probare_claves, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "paginae", _probare_paginas, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "mus", _probare_murem, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "figurae", _probare_figuras, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "novus_homo", _probare_novum_hominem, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "nova_societas", _probare_novam_societatem, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "deletio", _probare_deletionem, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "prompta", _probare_promptam, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "menu_novum", _probare_menu_novum, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "greges", _probare_greges, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "filtrum", _probare_filtrum, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "segmenta", _probare_segmenta, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("nomenclator", SECTIONES);
}
