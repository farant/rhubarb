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
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
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
                      b32  sine_notis;   /* P5b: sine volumine */
                     Dies  dies_aperta;  /* P6c: porta diei notat */
                      s32  aperitiones;
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

/* hodie fixa: 14 X 2026 */
interior b32
hodie_fixa (
    vacuum* ctx,
      Dies* hodie)
{
    (vacuum)ctx;
    *hodie = fasti_dies(MMXXVI, X, XIV);
    redde VERUM;
}

/* P6c: porta diei ficta - dies notatur */
interior b32
diem_notare (
     vacuum* ctx,
       Dies  dies)
{
    Mundus* x;

    x               = (Mundus*)ctx;
    x->dies_aperta  = dies;
    x->aperitiones++;
    redde VERUM;
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
               x->documentum, x->sine_notis ? NIHIL : x->volumen,
               hodie_fixa, NIHIL, CCCLX, CCCXX, NIHIL))
    {
        redde FALSUM;
    }
    nomenclator_portam_diei_ponere(&x->m, diem_notare, x);
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

/* P5b: textus commissus (imprimibiles, ut fenestra; scriba "per
 * TEXTUM venit") */
interior vacuum
textum_mittere (
                 Mundus* x,
     constans character* t)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                   = EVENTUS_TEXTUS;
    e.datum.textus.genus      = EVENTUS_TEXTUS_COMMISSUM;
    e.datum.textus.contentum  = chorda_ex_literis(t, x->m.piscina);
    dispensator_tractare(x->d, &e);
}

/* P5b: pagina I libri (lectio nova ex volumine); NIHIL si abest */
interior ScribaDocumentum*
pagina_libri (
                 Mundus* x,
     constans character* praefixum,
                Piscina* p)
{
    ScribaLiber* l;

    l = scriba_liber_aperire_in_spatio(p, x->intern, x->volumen,
        praefixum, LXXII, XXX);
    redde l ? scriba_liber_pagina(l, chorda_ex_literis("1", p)) : NIHIL;
}

/* P5b: textus in initio paginae I libri committitur */
interior b32
librum_parare (
                 Mundus* x,
     constans character* praefixum,
     constans character* textus,
                Piscina* p)
{
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    d = pagina_libri(x, praefixum, p);
    si (!d)
    {
        redde FALSUM;
    }
    tabula_initiare(&folium, p, d->latitudo, d->altitudo);
    memcpy(folium.cellulae, scriba_documentum_tabula(d)->cellulae,
        (memoriae_index)(d->latitudo * d->altitudo));
    memcpy(&tabula_cellula(&folium, ZEPHYRUM, ZEPHYRUM), textus,
        strlen(textus));
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

/* P5b: littera in pagina I libri? */
interior b32
littera_in_libro (
                 Mundus* x,
     constans character* praefixum,
              character  littera,
                Piscina* p)
{
              ScribaDocumentum* d;
    constans TabulaCharacterum* t;
                           i32  i;

    d = pagina_libri(x, praefixum, p);
    si (!d)
    {
        redde FALSUM;
    }
    t = scriba_documentum_tabula(d);
    per (i = ZEPHYRUM; i < t->latitudo * t->altitudo; i++)
    {
        si (t->cellulae[i] == (i8)littera)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* P5b: persona aperitur ictu duplici in ordine p<id> */
interior b32
personam_aperire (
                 Mundus* x,
     constans character* ordo,
                Piscina* p)
{
    redde ictus_in(x, ordo, p) && ictus_in(x, ordo, p);
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

/* Backspace n vicibus */
interior vacuum
retro (
    Mundus* x,
       i32  n)
{
    per (; n > ZEPHYRUM; n--)
    {
        clavis_specialis(x, CLAVIS_RETRORSUM, ZEPHYRUM, ZEPHYRUM,
            ZEPHYRUM);
    }
}

/* index (Xar de chorda) personae = "a|b" ? */
interior b32
index_est (
          constans Xar* l,
    constans character* exspectatus)
{
    character  visus[CCLVI];
          i32  i;
       chorda* c;

    visus[ZEPHYRUM] = '\0';
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        c = (chorda*)xar_obtinere(l, i);
        si (i > ZEPHYRUM)
        {
            strcat(visus, "|");
        }
        strncat(visus, (constans character*)c->datum,
            (size_t)c->mensura);
    }
    si (strcmp(visus, exspectatus) != ZEPHYRUM)
    {
        imprimere("  index: '%s'\n", visus);
        redde FALSUM;
    }
    redde VERUM;
}

/* P5a: aperire (Enter, ictus duplex), redire (Esc) */
interior vacuum
_probare_aperire (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    reditus(x);
    CREDO_VERUM(campus_est(x, "persona_aperta", "5"));
    CREDO_VERUM(adest(x, "f0", c->piscina));
    CREDO_FALSUM(adest(x, "p5", c->piscina));
    effugium(x);
    CREDO_VERUM(campus_est(x, "persona_aperta", NIHIL));
    CREDO_VERUM(adest(x, "p5", c->piscina));
    CREDO_NECESSE(ictus_in(x, "p1", c->piscina));
    CREDO_NECESSE(ictus_in(x, "p1", c->piscina));
    CREDO_VERUM(campus_est(x, "persona_aperta", "1"));
    /* redire globulo */
    CREDO_NECESSE(ictus_in(x, "redire", c->piscina));
    CREDO_VERUM(campus_est(x, "persona_aperta", NIHIL));
}

/* P5a: campi - telephona (addere, delere vacuo), epistula, rubricae,
 * nomen. Anna Rossi: f0 nomen, f1 + phone, f2 + email, f3 tags, f4
 * birthday */
interior vacuum
_probare_campos (
    CredoContextus* c)
{
              Mundus* x;
    constans Persona* q;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_NECESSE(ictus_in(x, "p1", c->piscina));
    reditus(x);
    CREDO_VERUM(campus_est(x, "persona_aperta", "1"));
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "campus_electus", "1"));
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "telephonum"));
    typare(x, "+39 06 123");
    reditus(x);
    q = personae_persona(x->documentum, I);
    CREDO_NECESSE_NON_NIHIL(q);
    CREDO_VERUM(index_est(q->telephona, "+39 06 123"));
    /* f1 phone, f2 + phone: alterum */
    clavis(x, 'j');
    reditus(x);
    typare(x, "333");
    reditus(x);
    q = personae_persona(x->documentum, I);
    CREDO_VERUM(index_est(q->telephona, "+39 06 123|333"));
    /* f1 phone, f2 phone novum (electum), f3 + phone: f1 vacuum ->
     * deletum */
    clavis(x, 'k');
    CREDO_VERUM(campus_est(x, "campus_electus", "1"));
    reditus(x);
    retro(x, X);
    reditus(x);
    q = personae_persona(x->documentum, I);
    CREDO_VERUM(index_est(q->telephona, "333"));
    /* f0 nomen, f1 333, f2 + phone, f3 + email */
    clavis(x, 'j');
    clavis(x, 'j');
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "epistula"));
    typare(x, "anna@example.org");
    reditus(x);
    q = personae_persona(x->documentum, I);
    CREDO_VERUM(index_est(q->epistulae, "anna@example.org"));
    /* f3 email (electa), f4 + email, f5 tags */
    clavis(x, 'j');
    clavis(x, 'j');
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "rubricae"));
    typare(x, " amici");
    reditus(x);
    q = personae_persona(x->documentum, I);
    CREDO_VERUM(index_est(q->rubricae, "amici|familia|roma"));
    /* nomen: "Anna Rossi" -> "Anna Bianchi" */
    clavis(x, 'k');
    clavis(x, 'k');
    clavis(x, 'k');
    clavis(x, 'k');
    clavis(x, 'k');
    clavis(x, 'k');
    CREDO_VERUM(campus_est(x, "campus_electus", NIHIL)
        || campus_est(x, "campus_electus", "0"));
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "nomen"));
    retro(x, V);
    typare(x, "Bianchi");
    reditus(x);
    CREDO_VERUM(persona_est(x, I, "Anna|Bianchi|"));
}

/* P5a: natalis - "1986-03-02" (aetas XL die 14 X 2026), "03-02" sine
 * anno, "--12-25", malum recusatum, vacuum tollit */
interior vacuum
_probare_natalem (
    CredoContextus* c)
{
              Mundus* x;
    constans Persona* q;
             Mandata* md;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_NECESSE(ictus_in(x, "p1", c->piscina));
    reditus(x);
    clavis(x, 'j');
    clavis(x, 'j');
    clavis(x, 'j');
    clavis(x, 'j');
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    typare(x, "1986-03-02");
    reditus(x);
    q = personae_persona(x->documentum, I);
    CREDO_VERUM(q && q->natalis_positus && q->natalis.annus == MCMLXXXVI
        && q->natalis.mensis == III && q->natalis.dies == II);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, fines_componentis(x, "f4", c->piscina),
        "2 March 1986", -I));
    CREDO_VERUM(textus_in(md, fines_componentis(x, "f4", c->piscina),
        "40", -I));
    reditus(x);
    retro(x, X);
    typare(x, "03-02");
    reditus(x);
    q = personae_persona(x->documentum, I);
    CREDO_VERUM(q && q->natalis_positus
        && q->natalis.annus == ZEPHYRUM);
    md = reddere(x, c->piscina);
    CREDO_FALSUM(textus_in(md, fines_componentis(x, "f4", c->piscina),
        "40", -I));
    reditus(x);
    retro(x, V);
    typare(x, "--12-25");
    reditus(x);
    q = personae_persona(x->documentum, I);
    CREDO_VERUM(q && q->natalis.mensis == XII
        && q->natalis.dies == XXV);
    /* malum: prompta manet */
    reditus(x);
    retro(x, VII);
    typare(x, "13-40");
    reditus(x);
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    /* vacuum: tollit */
    retro(x, V);
    reditus(x);
    q = personae_persona(x->documentum, I);
    CREDO_VERUM(q && !q->natalis_positus);
}

/* P5a: genus (t, segmenta), delere ex visu personae, figura */
interior vacuum
_probare_personam_aliam (
    CredoContextus* c)
{
     Mundus* x;
    Mandata* md;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_NECESSE(ictus_in(x, "p1", c->piscina));
    reditus(x);
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, fines_componentis(x, "f0", c->piscina),
        "Anna Rossi", FONS_TITULI));
    CREDO_VERUM(textus_in(md, fines_componentis(x, "f1", c->piscina),
        "+ phone", -I));
    CREDO_VERUM(textus_in(md, fines_componentis(x, "imago", c->piscina),
        "photo", -I));
    /* f3 = Tags: "#familia #roma" */
    CREDO_VERUM(textus_in(md, fines_componentis(x, "f3", c->piscina),
        "#familia #roma", -I));
    clavis(x, 't');
    CREDO_VERUM(personae_persona(x->documentum, I)->genus
        == PERSONA_SOCIETAS);
    CREDO_NECESSE(ictus_in(x, "genus-homo", c->piscina));
    CREDO_VERUM(personae_persona(x->documentum, I)->genus
        == PERSONA_HOMO);
    clavis(x, 'X');
    CREDO_NIHIL(personae_persona(x->documentum, I));
    CREDO_VERUM(campus_est(x, "persona_aperta", NIHIL));
}

/* P5b: notae - liber "persona/<id>" in dimidio infero visus personae;
 * liber personam apertam sequitur; index notas non ostendit */
interior vacuum
_probare_notas (
    CredoContextus* c)
{
      Mundus* x;
     Mandata* md;
       Fines  f;
       Fines  totum;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(librum_parare(x, "persona/1", "quinque", c->piscina));
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    totum.x         = ZEPHYRUM;
    totum.y         = ZEPHYRUM;
    totum.latitudo  = CCCLX;
    totum.altitudo  = CCCXX;
    CREDO_NECESSE(personam_aperire(x, "p1", c->piscina));
    CREDO_VERUM(campus_est(x, "persona_aperta", "1"));
    f = fines_componentis(x, "notae", c->piscina);
    CREDO_NECESSE(f.altitudo > ZEPHYRUM);
    /* scriba in coordinatis suis pingit: textus ubivis (locus in
     * sectione charta probatur) */
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, totum, "quinque", -I));
    /* index: notae absunt */
    effugium(x);
    md = reddere(x, c->piscina);
    CREDO_FALSUM(textus_in(md, totum, "quinque", -I));
    /* persona II: liber suus (vacuus); I redit */
    CREDO_NECESSE(personam_aperire(x, "p2", c->piscina));
    CREDO_VERUM(campus_est(x, "persona_aperta", "2"));
    md = reddere(x, c->piscina);
    CREDO_FALSUM(textus_in(md, totum, "quinque", -I));
    effugium(x);
    CREDO_NECESSE(personam_aperire(x, "p1", c->piscina));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, totum, "quinque", -I));
}

/* P5b: umbra plana: rectangulum impletum fines (II infra-dextrorsum)
 * exactis? */
interior b32
umbra_est (
    Mandata* md,
      Fines  area)
{
    constans Mandatum* x;
                  i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(md->elementa); i++)
    {
        x = (constans Mandatum*)xar_obtinere(md->elementa, i);
        si (   x->genus == MANDATUM_RECTANGULUM && x->impletum
            && x->fines.x == area.x + II && x->fines.y == area.y + II
            && x->fines.latitudo == area.latitudo
            && x->fines.altitudo == area.altitudo)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* P5b: charta cellulis exacta - pagina nova ex loco "notae" (sine
 * volutione, sine spatio vacuo): area = (pagina + II) x (pagina + III)
 * cellulae = locus minus margines chartae et umbra */
interior vacuum
_probare_chartam (
    CredoContextus* c)
{
               Mundus* x;
            Componens* scriba;
                Fines  locus;
     ScribaDocumentum* d;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_NECESSE(personam_aperire(x, "p1", c->piscina));
    locus   = fines_componentis(x, "notae", c->piscina);
    scriba  = componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("radix", c->piscina));
    d       = pagina_libri(x, "persona/1", c->piscina);
    CREDO_NECESSE(scriba && d && locus.altitudo > ZEPHYRUM);
    CREDO_AEQUALIS_S32(scriba->fines.x, locus.x + VIII);
    CREDO_AEQUALIS_S32(scriba->fines.y, locus.y + VIII);
    CREDO_AEQUALIS_S32(scriba->fines.latitudo, ((s32)d->latitudo + II)
        * VI);
    CREDO_AEQUALIS_S32(scriba->fines.altitudo, ((s32)d->altitudo + III)
        * VIII);
    CREDO_AEQUALIS_S32((locus.latitudo - XVIII) / VI, (s32)d->latitudo
        + II);
    CREDO_AEQUALIS_S32((locus.altitudo - XVIII) / VIII,
        (s32)d->altitudo + III);
    CREDO_VERUM(umbra_est(reddere(x, c->piscina), scriba->fines));
}

/* P5b: focus in notis - ictus in charta; claves nomenclatoris (j X t)
 * interea nihil agunt, "ix" in librum; Esc (modus normalis), Esc ex
 * notis (visus personae manet), Esc ad indicem */
interior vacuum
_probare_focum_notarum (
    CredoContextus* c)
{
    Mundus* x;
     Motus* motus;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    motus = dispensator_motus(x->d);
    /* hospes gestum ponit (ut vicus in foco lateris) */
    nomenclator_gestum_ponere(motus, &x->m);
    CREDO_NECESSE(personam_aperire(x, "p1", c->piscina));
    CREDO_NECESSE(ictus_in(x, "notae", c->piscina));
    CREDO_VERUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_notae", c->piscina)));
    textum_mittere(x, "i");
    textum_mittere(x, "x");
    effugium(x);
    clavis(x, 'j');
    clavis(x, 'X');
    clavis(x, 't');
    CREDO_VERUM(campus_est(x, "campus_electus", NIHIL));
    CREDO_VERUM(persona_est(x, I, "Anna|Rossi|"));
    CREDO_VERUM(personae_persona(x->documentum, I)->genus
        == PERSONA_HOMO);
    (vacuum)motus_gestum_effundere(motus, x->repo);
    CREDO_VERUM(littera_in_libro(x, "persona/1", 'x', c->piscina));
    effugium(x);
    CREDO_FALSUM(chorda_continet(motus->spatium_intimum,
        chorda_ex_literis("_notae", c->piscina)));
    CREDO_VERUM(campus_est(x, "persona_aperta", "1"));
    effugium(x);
    CREDO_VERUM(campus_est(x, "persona_aperta", NIHIL));
}

/* P5b: sine volumine - visus personae sine notis (locus manet),
 * reficere FALSUM */
interior vacuum
_probare_sine_volumine (
    CredoContextus* c)
{
    Mundus* x;

    x              = (Mundus*)c->datum;
    x->sine_notis  = VERUM;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    nomenclator_gestum_ponere(dispensator_motus(x->d), &x->m);
    CREDO_NECESSE(personam_aperire(x, "p1", c->piscina));
    CREDO_VERUM(adest(x, "notae", c->piscina));
    CREDO_FALSUM(componens_invenire_per_id(dispensator_arbor(x->d),
        chorda_ex_literis("radix", c->piscina)) != NIHIL);
    CREDO_FALSUM(nomenclator_reficere(&x->m));
}

/* P6b: argumentum lateris - "<id>" personam aperit (liber notarum
 * sequitur), "" et NIHIL ad indicem; id malum aut ignotum FALSUM,
 * status manet */
interior vacuum
_probare_argumentum (
    CredoContextus* c)
{
      Mundus* x;
     Mandata* md;
       Fines  totum;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(librum_parare(x, "persona/1", "quinque", c->piscina));
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    totum.x         = ZEPHYRUM;
    totum.y         = ZEPHYRUM;
    totum.latitudo  = CCCLX;
    totum.altitudo  = CCCXX;
    CREDO_VERUM(nomenclator_argumentum_ponere(&x->m, "1"));
    dispensator_recomponere(x->d);
    CREDO_VERUM(campus_est(x, "persona_aperta", "1"));
    CREDO_VERUM(adest(x, "f0", c->piscina));
    md = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, totum, "quinque", -I));
    CREDO_VERUM(nomenclator_argumentum_ponere(&x->m, "3"));
    CREDO_VERUM(campus_est(x, "persona_aperta", "3"));
    CREDO_FALSUM(nomenclator_argumentum_ponere(&x->m, "99"));
    CREDO_FALSUM(nomenclator_argumentum_ponere(&x->m, "abc"));
    CREDO_FALSUM(nomenclator_argumentum_ponere(&x->m, "0"));
    CREDO_VERUM(campus_est(x, "persona_aperta", "3"));
    CREDO_VERUM(nomenclator_argumentum_ponere(&x->m, ""));
    CREDO_VERUM(campus_est(x, "persona_aperta", NIHIL));
    CREDO_VERUM(nomenclator_argumentum_ponere(&x->m, "2"));
    CREDO_VERUM(nomenclator_argumentum_ponere(&x->m, NIHIL));
    CREDO_VERUM(campus_est(x, "persona_aperta", NIHIL));
}

/* P6c: natalis personae I ponere (annus 0 = sine anno) */
interior b32
natalem_ponere (
     Mundus* x,
        s32  annus,
        s32  mensis,
        s32  dies)
{
    Dies d;

    d = fasti_dies(annus, mensis, dies);
    redde personae_natalem_ponere(x->documentum, I, &d) > ZEPHYRUM;
}

interior b32
dies_aperta_est (
     Mundus* x,
        s32  annus,
        s32  mensis,
        s32  dies)
{
    si (   x->dies_aperta.annus  != annus
        || x->dies_aperta.mensis != mensis
        || x->dies_aperta.dies   != dies)
    {
        imprimere("  dies aperta: %d-%d-%d\n",
            (integer)x->dies_aperta.annus,
            (integer)x->dies_aperta.mensis,
            (integer)x->dies_aperta.dies);
        redde FALSUM;
    }
    redde VERUM;
}

/* P6c: ad natalem - 'c' et globulus "ad_natalem" portam diei cum
 * natali proximo (hodie XIV X MMXXVI inclusa) vocant: futurus hoc
 * anno, hodie, praeteritus (anno proximo), sine anno, 29 II (28 II
 * anni communis) */
interior vacuum
_probare_ad_natalem (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_NECESSE(natalem_ponere(x, MCMLXXXVI, XII, XXV));
    CREDO_NECESSE(personam_aperire(x, "p1", c->piscina));
    clavis(x, 'c');
    CREDO_AEQUALIS_S32(x->aperitiones, I);
    CREDO_VERUM(dies_aperta_est(x, MMXXVI, XII, XXV));
    CREDO_NECESSE(natalem_ponere(x, MCMLXXXVI, X, XIV));
    clavis(x, 'c');
    CREDO_VERUM(dies_aperta_est(x, MMXXVI, X, XIV));
    CREDO_NECESSE(natalem_ponere(x, MCMLXXXVI, III, II));
    clavis(x, 'c');
    CREDO_VERUM(dies_aperta_est(x, MMXXVII, III, II));
    CREDO_NECESSE(natalem_ponere(x, ZEPHYRUM, XII, XXV));
    clavis(x, 'c');
    CREDO_VERUM(dies_aperta_est(x, MMXXVI, XII, XXV));
    CREDO_NECESSE(natalem_ponere(x, MM, II, XXIX));
    dispensator_recomponere(x->d);
    CREDO_NECESSE(ictus_in(x, "ad_natalem", c->piscina));
    CREDO_VERUM(dies_aperta_est(x, MMXXVII, II, XXVIII));
    CREDO_AEQUALIS_S32(x->aperitiones, V);
}

/* P6c: sine natali (Banca) aut sine porta: globulus abest, 'c' nihil */
interior vacuum
_probare_ad_natalem_absens (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_NECESSE(personam_aperire(x, "p2", c->piscina));
    CREDO_FALSUM(adest(x, "ad_natalem", c->piscina));
    clavis(x, 'c');
    CREDO_AEQUALIS_S32(x->aperitiones, ZEPHYRUM);
    effugium(x);
    CREDO_NECESSE(natalem_ponere(x, MCMLXXXVI, XII, XXV));
    CREDO_NECESSE(personam_aperire(x, "p1", c->piscina));
    CREDO_VERUM(adest(x, "ad_natalem", c->piscina));
    nomenclator_portam_diei_ponere(&x->m, NIHIL, NIHIL);
    dispensator_recomponere(x->d);
    CREDO_FALSUM(adest(x, "ad_natalem", c->piscina));
    clavis(x, 'c');
    CREDO_AEQUALIS_S32(x->aperitiones, ZEPHYRUM);
}

/* P4: quaestio viva - '/' promptam "search:" aperit; dum typatur,
 * index = ordo similitudinis (sine titulis), electa = prima; "ross"
 * solam Annam, "ar" Annam ante Andream; Esc in prompta: quaestio
 * nulla commissa - index plenus redit */
interior vacuum
_probare_quaestionem_vivam (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    /* electa extra fructus (Banca): sub quaestione prima fructuum */
    clavis(x, 'j');
    CREDO_AEQUALIS_S32(nomenclator_persona_electa(&x->m), II);
    clavis(x, '/');
    CREDO_VERUM(campus_est(x, "prompta", "quaestio"));
    typare(x, "ross");
    CREDO_VERUM(ordines_sunt(x, "p1"));
    CREDO_AEQUALIS_S32(nomenclator_persona_electa(&x->m), I);
    retro(x, IV);
    typare(x, "ar");
    CREDO_VERUM(ordines_sunt(x, "p1 p4"));
    CREDO_AEQUALIS_S32(nomenclator_persona_electa(&x->m), I);
    effugium(x);
    CREDO_FALSUM(campus_est(x, "prompta", "quaestio"));
    CREDO_VERUM(campus_est(x, "quaestio", NIHIL));
    CREDO_VERUM(ordines_sunt(x, "l_ p5 lB p2 p3 lC p4 lR p1"));
}

/* P4: Reditus committit (prompta clauditur, ordines manent); j et
 * Reditus personam aperiunt; Esc ex persona ad ordines; Esc in indice
 * quaestionem tollit; '/' quaestione praeimpleta; Esc in prompta
 * quaestionem commissam servat */
interior vacuum
_probare_quaestionem_commissam (
    CredoContextus* c)
{
      Mundus* x;
     Mandata* md;
       Fines  totum;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    clavis(x, '/');
    typare(x, "ar");
    reditus(x);
    CREDO_FALSUM(campus_est(x, "prompta", "quaestio"));
    CREDO_VERUM(campus_est(x, "quaestio", "ar"));
    CREDO_VERUM(ordines_sunt(x, "p1 p4"));
    /* titulus quaestionem ostendit, caput "2 of 5" */
    totum.x         = ZEPHYRUM;
    totum.y         = ZEPHYRUM;
    totum.latitudo  = CCCLX;
    totum.altitudo  = CCCXX;
    md              = reddere(x, c->piscina);
    CREDO_VERUM(textus_in(md, totum, "/ar", -I));
    CREDO_VERUM(textus_in(md, totum, "2 of 5 contacts", -I));
    clavis(x, 'j');
    reditus(x);
    CREDO_VERUM(campus_est(x, "persona_aperta", "4"));
    effugium(x);
    CREDO_VERUM(ordines_sunt(x, "p1 p4"));
    /* '/' praeimpleta; Esc in prompta - commissa manet */
    clavis(x, '/');
    retro(x, I);
    typare(x, "x");
    CREDO_VERUM(ordines_sunt(x, "nihil_inventum"));
    effugium(x);
    CREDO_VERUM(campus_est(x, "quaestio", "ar"));
    CREDO_VERUM(ordines_sunt(x, "p1 p4"));
    /* Esc in indice tollit */
    effugium(x);
    CREDO_VERUM(campus_est(x, "quaestio", NIHIL));
    CREDO_VERUM(ordines_sunt(x, "l_ p5 lB p2 p3 lC p4 lR p1"));
    /* '"' effugitus: commissus, deinde praeimpletus et solutus */
    clavis(x, '/');
    typare(x, "ar\"");
    reditus(x);
    CREDO_VERUM(ordines_sunt(x, "nihil_inventum"));
    CREDO_FALSUM(campus_est(x, "quaestio", "ar\""));
    clavis(x, '/');
    retro(x, I);
    CREDO_VERUM(ordines_sunt(x, "p1 p4"));
}

/* P4: quaestio intra filtrum rubricae; Esc primum quaestionem, deinde
 * filtrum tollit; visus rubricarum: ordo planus similitudinis */
interior vacuum
_probare_quaestionem_filtro (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    clavis(x, 'f');
    typare(x, "roma");
    reditus(x);
    clavis(x, '/');
    typare(x, "ar");
    reditus(x);
    CREDO_VERUM(ordines_sunt(x, "p1"));
    effugium(x);
    CREDO_VERUM(campus_est(x, "quaestio", NIHIL));
    CREDO_VERUM(campus_est(x, "rubrica_filtrum", "roma"));
    effugium(x);
    CREDO_VERUM(campus_est(x, "rubrica_filtrum", NIHIL));
    clavis(x, 'g');
    CREDO_VERUM(campus_est(x, "visus", "rubricae"));
    clavis(x, '/');
    typare(x, "ar");
    reditus(x);
    CREDO_VERUM(ordines_sunt(x, "p1 p4"));
    effugium(x);
    CREDO_VERUM(ordines_sunt(x, "g0 p3-0 g1 p1-1 g2 p1-2 g3 p5-3 p2-3"
        " p4-3"));
}

/* natalis Annae (campus f4) per promptam: aperta si clausa, vacuata,
 * textus, Reditus */
interior vacuum
natalem_typare (
                 Mundus* x,
     constans character* textus)
{
    si (!campus_est(x, "prompta", "natalis"))
    {
        reditus(x);
    }
    retro(x, XX);
    typare(x, textus);
    reditus(x);
}

interior b32
natalis_est (
     Mundus* x,
        s32  annus,
        s32  mensis,
        s32  dies)
{
    constans Persona* q;

    q = personae_persona(x->documentum, I);
    si (   !q || !q->natalis_positus || q->natalis.annus != annus
        || q->natalis.mensis != mensis || q->natalis.dies != dies)
    {
        si (q)
        {
            imprimere("  natalis: %d %d-%d-%d\n",
                (integer)q->natalis_positus, (integer)q->natalis.annus,
                (integer)q->natalis.mensis, (integer)q->natalis.dies);
        }
        redde FALSUM;
    }
    redde VERUM;
}

/* natalis forma americana (Franus 10-15): M/D/YYYY et MM/DD/YYYY, M/D
 * et MM/DD sine anno; annus IV cifris (86 ambiguum), dies vera; mala:
 * prompta manet, natalis non mutatur */
interior vacuum
_probare_natalem_americanum (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(contactus_parare(x, c->piscina));
    CREDO_NECESSE(personam_aperire(x, "p1", c->piscina));
    clavis(x, 'j');
    clavis(x, 'j');
    clavis(x, 'j');
    clavis(x, 'j');
    CREDO_VERUM(campus_est(x, "campus_electus", "4"));
    natalem_typare(x, "3/2/1986");
    CREDO_VERUM(natalis_est(x, MCMLXXXVI, III, II));
    natalem_typare(x, "12/25");
    CREDO_VERUM(natalis_est(x, ZEPHYRUM, XII, XXV));
    natalem_typare(x, "07/04/1990");
    CREDO_VERUM(natalis_est(x, MCMXC, VII, IV));
    natalem_typare(x, "02/29");
    CREDO_VERUM(natalis_est(x, ZEPHYRUM, II, XXIX));
    /* mala: prompta manet, natalis (II 29) manet */
    natalem_typare(x, "3/2/86");
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    natalem_typare(x, "13/02/1986");
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    natalem_typare(x, "2/30");
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    natalem_typare(x, "3/2/0000");
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    natalem_typare(x, "1/2/3/4");
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    natalem_typare(x, "/2/1986");
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    natalem_typare(x, "3//1986");
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    natalem_typare(x, "3/2/");
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    natalem_typare(x, "003/2");
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    natalem_typare(x, "3/2/1986x");
    CREDO_VERUM(campus_est(x, "prompta", "natalis"));
    CREDO_VERUM(natalis_est(x, ZEPHYRUM, II, XXIX));
    /* ISO manet */
    natalem_typare(x, "1986-03-02");
    CREDO_VERUM(natalis_est(x, MCMLXXXVI, III, II));
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
    { "aperire", _probare_aperire, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "campi", _probare_campos, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "natalis", _probare_natalem, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "persona_alia", _probare_personam_aliam, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "notae", _probare_notas, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "charta", _probare_chartam, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "focus_notarum", _probare_focum_notarum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "sine_volumine", _probare_sine_volumine, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "argumentum", _probare_argumentum, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "ad_natalem", _probare_ad_natalem, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "ad_natalem_absens", _probare_ad_natalem_absens, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "quaestio_viva", _probare_quaestionem_vivam, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "quaestio_commissa", _probare_quaestionem_commissam,
        mundum_parare, mundum_purgare, NIHIL, NIHIL },
    { "quaestio_filtro", _probare_quaestionem_filtro, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "natalis_americanus", _probare_natalem_americanum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("nomenclator", SECTIONES);
}
