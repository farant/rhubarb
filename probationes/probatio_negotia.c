/* probatio_negotia.c - negotia et facultates in volumine (negotia
 * plan T1): imago plena per actum, revisiones (mutatio par nihil
 * scribit), ordo activitatis, colloquia idempotentia, refutationes
 * nihil scribunt, suffixum ambiguum refutatur, acta alterius
 * scriptoris (manubrium secundum eiusdem plagulae) per
 * negotia_renovare videntur. Volumen temporarium per sectionem; ids
 * per moneta_numerator ("1", "2", ...) aut fontem fixum. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "moneta.h"
#include "negotia.h"
#include "credo.h"
#include <string.h>

nomen structura {
    constans character* constans* ids;
                             i32 k;
} FonsFixus;

nomen structura {
     Volumen* volumen;
     Negotia* n;
         i32  numerator;
} Mundus;

interior vacuum
fons_fixus (
       vacuum* ctx,
    character* effusio)
{
    FonsFixus* f = (FonsFixus*)ctx;

    strcpy(effusio, f->ids[f->k]);
    f->k++;
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    m->volumen = volumen_temporarium(c->piscina, "probatio_negotia");
    si (m->volumen)
    {
        m->n = negotia_aperire(m->volumen, moneta_numerator,
            &m->numerator, c->piscina);
    }
    c->datum = m;
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* m = (Mundus*)c->datum;

    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
}

interior chorda
lit (
    constans character* s,
               Piscina* p)
{
    redde chorda_ex_literis(s, p);
}

interior constans NegotiiFacultas*
facultas_ad (
    Xar* x,
    i32  k)
{
    redde (constans NegotiiFacultas*)xar_obtinere(x, k);
}

interior vacuum
_probare_creare (
    CredoContextus* c)
{
                   Mundus* m = (Mundus*)c->datum;
                   chorda  ng;
                   chorda  f1;
                   chorda  f2;
                      Xar* fs;
        constans Negotium* x;

    CREDO_NECESSE_NON_NIHIL(m->n);
    ng = negotia_negotium_creare(m->n, lit("negotia", c->piscina),
        lit("incepta ut loca", c->piscina));
    CREDO_CHORDA_NON_VACUA(ng);
    f1 = negotia_facultatem_creare(m->n, ng,
        lit("bibliotheca", c->piscina), NEGOTIA_PERFECTA,
        lit("acta in volumine", c->piscina));
    f2 = negotia_facultatem_creare(m->n, ng,
        lit("tabula", c->piscina), NEGOTIA_PROPOSITA, lit("",
        c->piscina));
    CREDO_CHORDA_NON_VACUA(f1);
    CREDO_CHORDA_NON_VACUA(f2);

    CREDO_GRADUS("negotium legitur");
    CREDO_AEQUALIS_I32(negotia_numerus(m->n), I);
    x = negotia_index(m->n, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_VERUM(chorda_aequalis(x->id, ng));
    CREDO_CHORDA_AEQUALIS_LITERIS(x->titulus, "negotia");
    CREDO_CHORDA_AEQUALIS_LITERIS(x->descriptio, "incepta ut loca");
    CREDO_NIHIL(negotia_index(m->n, I));

    CREDO_GRADUS("facultates ordine creationis");
    fs = negotia_facultates(m->n, ng, c->piscina);
    CREDO_NECESSE_NON_NIHIL(fs);
    CREDO_AEQUALIS_I32(xar_numerus(fs), II);
    CREDO_VERUM(chorda_aequalis(facultas_ad(fs, ZEPHYRUM)->id, f1));
    CREDO_VERUM(chorda_aequalis(facultas_ad(fs, I)->id, f2));
    CREDO_CHORDA_AEQUALIS_LITERIS(facultas_ad(fs, ZEPHYRUM)->corpus,
        "acta in volumine");
    CREDO_VERUM(chorda_aequalis(facultas_ad(fs, ZEPHYRUM)->negotium,
        ng));
    CREDO_LITERAE_AEQUALES(negotia_status_titulus(
        facultas_ad(fs, ZEPHYRUM)->status), "shipped");
    CREDO_LITERAE_AEQUALES(negotia_status_titulus(
        facultas_ad(fs, I)->status), "planned");
}

interior vacuum
_probare_status (
    CredoContextus* c)
{
    s32 s;

    CREDO_LITERAE_AEQUALES(negotia_status_titulus(NEGOTIA_PROPOSITA),
        "planned");
    CREDO_LITERAE_AEQUALES(negotia_status_titulus(NEGOTIA_INCHOATA),
        "partial");
    CREDO_LITERAE_AEQUALES(negotia_status_titulus(NEGOTIA_PERFECTA),
        "shipped");
    CREDO_LITERAE_AEQUALES(negotia_status_titulus(VII), "?");

    s = -I;
    CREDO_VERUM(negotia_status_legere(lit("inchoata", c->piscina), &s));
    CREDO_AEQUALIS_S32(s, NEGOTIA_INCHOATA);
    CREDO_VERUM(negotia_status_legere(lit("shipped", c->piscina), &s));
    CREDO_AEQUALIS_S32(s, NEGOTIA_PERFECTA);
    CREDO_VERUM(negotia_status_legere(lit("proposita", c->piscina),
        &s));
    CREDO_AEQUALIS_S32(s, NEGOTIA_PROPOSITA);
    CREDO_FALSUM(negotia_status_legere(lit("done", c->piscina), &s));
    CREDO_FALSUM(negotia_status_legere(lit("", c->piscina), &s));
}

interior vacuum
_probare_revisiones (
    CredoContextus* c)
{
                      Mundus* m = (Mundus*)c->datum;
                      chorda  ng;
                      chorda  f;
                         Xar* r;
                         s64  seq;
    constans NegotiiFacultas* praesens;

    CREDO_NECESSE_NON_NIHIL(m->n);
    ng = negotia_negotium_creare(m->n, lit("negotia", c->piscina),
        lit("", c->piscina));
    f = negotia_facultatem_creare(m->n, ng, lit("anulus", c->piscina),
        NEGOTIA_PROPOSITA, lit("Cmd+H/L", c->piscina));
    CREDO_CHORDA_NON_VACUA(f);
    CREDO_VERUM(negotia_facultatem_mutare(m->n, f,
        lit("anulus", c->piscina), NEGOTIA_PERFECTA,
        lit("Cmd+H/L", c->piscina)));
    CREDO_VERUM(negotia_facultatem_mutare(m->n, f,
        lit("anulus", c->piscina), NEGOTIA_PERFECTA,
        lit("Cmd+H/L per tabulam", c->piscina)));

    CREDO_GRADUS("tres revisiones, vetustissima prima");
    r = negotia_revisiones(m->n, f, c->piscina);
    CREDO_NECESSE_NON_NIHIL(r);
    CREDO_AEQUALIS_I32(xar_numerus(r), III);
    CREDO_AEQUALIS_S32(facultas_ad(r, ZEPHYRUM)->status,
        NEGOTIA_PROPOSITA);
    CREDO_AEQUALIS_S32(facultas_ad(r, I)->status, NEGOTIA_PERFECTA);
    CREDO_CHORDA_AEQUALIS_LITERIS(facultas_ad(r, I)->corpus,
        "Cmd+H/L");
    CREDO_CHORDA_AEQUALIS_LITERIS(facultas_ad(r, II)->corpus,
        "Cmd+H/L per tabulam");
    CREDO_VERUM(facultas_ad(r, ZEPHYRUM)->seq
        < facultas_ad(r, I)->seq);
    CREDO_VERUM(facultas_ad(r, I)->seq < facultas_ad(r, II)->seq);
    CREDO_CHORDA_NON_VACUA(facultas_ad(r, ZEPHYRUM)->momentum);

    CREDO_GRADUS("status praesens = revisio ultima");
    praesens = negotia_facultatem_invenire(m->n, f, NIHIL);
    CREDO_NECESSE_NON_NIHIL(praesens);
    CREDO_AEQUALIS_S32(praesens->status, NEGOTIA_PERFECTA);
    CREDO_CHORDA_AEQUALIS_LITERIS(praesens->corpus,
        "Cmd+H/L per tabulam");

    CREDO_GRADUS("mutatio par nihil scribit");
    seq = volumen_seq_ultimum(m->volumen);
    CREDO_VERUM(negotia_facultatem_mutare(m->n, f,
        lit("anulus", c->piscina), NEGOTIA_PERFECTA,
        lit("Cmd+H/L per tabulam", c->piscina)));
    CREDO_AEQUALIS_S64(volumen_seq_ultimum(m->volumen), seq);
    CREDO_AEQUALIS_I32(xar_numerus(negotia_revisiones(m->n, f,
        c->piscina)), III);

    CREDO_GRADUS("negotium: mutatio et mutatio par");
    CREDO_VERUM(negotia_negotium_mutare(m->n, ng,
        lit("negotia I", c->piscina), lit("", c->piscina)));
    CREDO_CHORDA_AEQUALIS_LITERIS(negotia_negotium_invenire(m->n, ng,
        NIHIL)->titulus, "negotia I");
    seq = volumen_seq_ultimum(m->volumen);
    CREDO_VERUM(negotia_negotium_mutare(m->n, ng,
        lit("negotia I", c->piscina), lit("", c->piscina)));
    CREDO_AEQUALIS_S64(volumen_seq_ultimum(m->volumen), seq);
}

interior vacuum
_probare_ordo (
    CredoContextus* c)
{
    Mundus* m = (Mundus*)c->datum;
    chorda  a;
    chorda  b;

    CREDO_NECESSE_NON_NIHIL(m->n);
    a = negotia_negotium_creare(m->n, lit("alpha", c->piscina),
        lit("", c->piscina));
    b = negotia_negotium_creare(m->n, lit("beta", c->piscina),
        lit("", c->piscina));
    CREDO_AEQUALIS_I32(negotia_numerus(m->n), II);

    CREDO_GRADUS("recentissimum primum");
    CREDO_VERUM(chorda_aequalis(negotia_index(m->n, ZEPHYRUM)->id, b));

    CREDO_GRADUS("facultas vetus negotium promovet");
    CREDO_CHORDA_NON_VACUA(negotia_facultatem_creare(m->n, a,
        lit("x", c->piscina), NEGOTIA_PROPOSITA, lit("", c->piscina)));
    CREDO_VERUM(chorda_aequalis(negotia_index(m->n, ZEPHYRUM)->id, a));
    CREDO_VERUM(chorda_aequalis(negotia_index(m->n, I)->id, b));

    CREDO_GRADUS("colloquium quoque promovet");
    CREDO_VERUM(negotia_colloquium_adiungere(m->n, b,
        lit("134c8417", c->piscina), lit("", c->piscina)));
    CREDO_VERUM(chorda_aequalis(negotia_index(m->n, ZEPHYRUM)->id, b));
}

interior vacuum
_probare_colloquia (
    CredoContextus* c)
{
    Mundus* m = (Mundus*)c->datum;
    chorda  ng;
       Xar* s;
       s64  seq;

    CREDO_NECESSE_NON_NIHIL(m->n);
    ng = negotia_negotium_creare(m->n, lit("negotia", c->piscina),
        lit("", c->piscina));
    CREDO_VERUM(negotia_colloquium_adiungere(m->n, ng,
        lit("s1", c->piscina), lit("", c->piscina)));
    CREDO_VERUM(negotia_colloquium_adiungere(m->n, ng,
        lit("s2", c->piscina), lit("", c->piscina)));
    seq = volumen_seq_ultimum(m->volumen);

    CREDO_GRADUS("iterum: VERUM, nihil scriptum");
    CREDO_VERUM(negotia_colloquium_adiungere(m->n, ng,
        lit("s1", c->piscina), lit("", c->piscina)));
    CREDO_AEQUALIS_S64(volumen_seq_ultimum(m->volumen), seq);

    s = negotia_colloquia(m->n, ng, c->piscina);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_AEQUALIS_I32(xar_numerus(s), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(s, ZEPHYRUM),
        "s1");
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(s, I), "s2");
}

interior vacuum
_probare_refutationes (
    CredoContextus* c)
{
    Mundus* m = (Mundus*)c->datum;
    chorda  ng;
    chorda  f;
       s64  seq;

    CREDO_NECESSE_NON_NIHIL(m->n);
    ng = negotia_negotium_creare(m->n, lit("negotia", c->piscina),
        lit("", c->piscina));
    f = negotia_facultatem_creare(m->n, ng, lit("x", c->piscina),
        NEGOTIA_PROPOSITA, lit("", c->piscina));
    seq = volumen_seq_ultimum(m->volumen);

    CREDO_CHORDA_VACUA(negotia_negotium_creare(m->n,
        lit("", c->piscina), lit("d", c->piscina)));
    CREDO_CHORDA_VACUA(negotia_facultatem_creare(m->n,
        lit("zzz", c->piscina), lit("y", c->piscina),
        NEGOTIA_PROPOSITA, lit("", c->piscina)));
    CREDO_CHORDA_VACUA(negotia_facultatem_creare(m->n, ng,
        lit("", c->piscina), NEGOTIA_PROPOSITA, lit("", c->piscina)));
    CREDO_CHORDA_VACUA(negotia_facultatem_creare(m->n, ng,
        lit("y", c->piscina), VII, lit("", c->piscina)));
    CREDO_FALSUM(negotia_facultatem_mutare(m->n, f,
        lit("x", c->piscina), -I, lit("", c->piscina)));
    CREDO_FALSUM(negotia_facultatem_mutare(m->n, lit("zzz",
        c->piscina), lit("x", c->piscina), NEGOTIA_PROPOSITA,
        lit("", c->piscina)));
    CREDO_FALSUM(negotia_negotium_mutare(m->n, ng, lit("", c->piscina),
        lit("", c->piscina)));
    CREDO_FALSUM(negotia_colloquium_adiungere(m->n, lit("zzz",
        c->piscina), lit("s1", c->piscina), lit("", c->piscina)));
    CREDO_FALSUM(negotia_colloquium_adiungere(m->n, ng,
        lit("", c->piscina), lit("", c->piscina)));

    CREDO_GRADUS("nihil scriptum");
    CREDO_AEQUALIS_S64(volumen_seq_ultimum(m->volumen), seq);
    CREDO_AEQUALIS_I32(negotia_numerus(m->n), I);
}

interior vacuum
_probare_suffixum (
    CredoContextus* c)
{
                         Mundus* m = (Mundus*)c->datum;
                        Negotia* n;
                      FonsFixus  fons;
                            b32  ambiguum;
                         chorda  ng;
              constans Negotium* x;
    hic_manens constans character* constans IDS[] = {
        "01NEG0001A", "01NEG0002A", "01NEG0003B"
    };

    CREDO_NECESSE_NON_NIHIL(m->volumen);
    fons.ids = IDS;
    fons.k = ZEPHYRUM;
    n = negotia_aperire(m->volumen, fons_fixus, &fons, c->piscina);
    CREDO_NECESSE_NON_NIHIL(n);
    ng = negotia_negotium_creare(n, lit("alpha", c->piscina),
        lit("", c->piscina));
    (vacuum)negotia_negotium_creare(n, lit("beta", c->piscina),
        lit("", c->piscina));
    (vacuum)negotia_facultatem_creare(n, ng, lit("x", c->piscina),
        NEGOTIA_PROPOSITA, lit("", c->piscina));

    CREDO_GRADUS("suffixum commune: ambiguum, nihil redditum");
    ambiguum = FALSUM;
    CREDO_NIHIL(negotia_negotium_invenire(n, lit("A", c->piscina),
        &ambiguum));
    CREDO_VERUM(ambiguum);

    CREDO_GRADUS("suffixum unicum et id plenum");
    ambiguum = VERUM;
    x = negotia_negotium_invenire(n, lit("1A", c->piscina), &ambiguum);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_FALSUM(ambiguum);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->titulus, "alpha");
    x = negotia_negotium_invenire(n, lit("01NEG0002A", c->piscina),
        NIHIL);
    CREDO_NECESSE_NON_NIHIL(x);
    CREDO_CHORDA_AEQUALIS_LITERIS(x->titulus, "beta");

    CREDO_GRADUS("ignotum: nihil, non ambiguum");
    ambiguum = VERUM;
    CREDO_NIHIL(negotia_negotium_invenire(n, lit("Z", c->piscina),
        &ambiguum));
    CREDO_FALSUM(ambiguum);

    CREDO_GRADUS("genera separata: facultas non est negotium");
    CREDO_NIHIL(negotia_negotium_invenire(n, lit("3B", c->piscina),
        NIHIL));
    CREDO_NON_NIHIL(negotia_facultatem_invenire(n, lit("B",
        c->piscina), NIHIL));
}

interior vacuum
_probare_alius_scriptor (
    CredoContextus* c)
{
      Mundus* m = (Mundus*)c->datum;
     Volumen* v2;
     Negotia* n2;
         i32  numerator_alter;
      chorda  ng;
      chorda  f;

    CREDO_NECESSE_NON_NIHIL(m->n);
    v2 = volumen_aperire(c->piscina, volumen_via(m->volumen));
    CREDO_NECESSE_NON_NIHIL(v2);
    numerator_alter = C;
    n2 = negotia_aperire(v2, moneta_numerator, &numerator_alter,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(n2);

    CREDO_GRADUS("scriptor alius: ante renovare invisum");
    ng = negotia_negotium_creare(n2, lit("beta", c->piscina),
        lit("", c->piscina));
    CREDO_CHORDA_NON_VACUA(ng);
    CREDO_AEQUALIS_I32(negotia_numerus(m->n), ZEPHYRUM);

    CREDO_GRADUS("renovare id invenit");
    CREDO_VERUM(negotia_renovare(m->n));
    CREDO_AEQUALIS_I32(negotia_numerus(m->n), I);
    CREDO_NON_NIHIL(negotia_negotium_invenire(m->n, ng, NIHIL));
    CREDO_FALSUM(negotia_renovare(m->n));

    CREDO_GRADUS("et e contrario");
    f = negotia_facultatem_creare(m->n, ng, lit("x", c->piscina),
        NEGOTIA_INCHOATA, lit("", c->piscina));
    CREDO_CHORDA_NON_VACUA(f);
    CREDO_VERUM(negotia_renovare(n2));
    CREDO_NON_NIHIL(negotia_facultatem_invenire(n2, f, NIHIL));

    volumen_claudere(v2);
}

interior vacuum
_probare_iterum_apertum (
    CredoContextus* c)
{
      Mundus* m = (Mundus*)c->datum;
     Negotia* n2;
      chorda  ng;
      chorda  f;

    CREDO_NECESSE_NON_NIHIL(m->n);
    ng = negotia_negotium_creare(m->n, lit("negotia", c->piscina),
        lit("d", c->piscina));
    f = negotia_facultatem_creare(m->n, ng, lit("x", c->piscina),
        NEGOTIA_PROPOSITA, lit("", c->piscina));
    (vacuum)negotia_facultatem_mutare(m->n, f, lit("x", c->piscina),
        NEGOTIA_PERFECTA, lit("", c->piscina));
    (vacuum)negotia_colloquium_adiungere(m->n, ng, lit("s1",
        c->piscina), lit("", c->piscina));

    n2 = negotia_aperire(m->volumen, moneta_numerator, &m->numerator,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(n2);
    CREDO_AEQUALIS_I32(negotia_numerus(n2), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(negotia_index(n2, ZEPHYRUM)->
        descriptio, "d");
    CREDO_AEQUALIS_I32(xar_numerus(negotia_facultates(n2, ng,
        c->piscina)), I);
    CREDO_AEQUALIS_S32(negotia_facultatem_invenire(n2, f, NIHIL)->
        status, NEGOTIA_PERFECTA);
    CREDO_AEQUALIS_I32(xar_numerus(negotia_revisiones(n2, f,
        c->piscina)), II);
    CREDO_AEQUALIS_I32(xar_numerus(negotia_colloquia(n2, ng,
        c->piscina)), I);
}

interior vacuum
_probare_summarium (
    CredoContextus* c)
{
                     Mundus* m = (Mundus*)c->datum;
                     chorda  ng;
                     chorda  vacuum_ng;
                        i32  k;
    hic_manens constans s32  status[] = {
        NEGOTIA_PERFECTA, NEGOTIA_INCHOATA, NEGOTIA_PERFECTA,
        NEGOTIA_PROPOSITA };

    CREDO_NECESSE_NON_NIHIL(m->n);
    ng = negotia_negotium_creare(m->n, lit("a", c->piscina),
        lit("", c->piscina));
    vacuum_ng = negotia_negotium_creare(m->n, lit("b", c->piscina),
        lit("", c->piscina));

    CREDO_GRADUS("negotium vacuum, ignotum: vacua");
    CREDO_AEQUALIS_I32(negotia_summarium(m->n, vacuum_ng,
        c->piscina).mensura, ZEPHYRUM);
    CREDO_AEQUALIS_I32(negotia_summarium(m->n, lit("ignotum",
        c->piscina), c->piscina).mensura, ZEPHYRUM);

    CREDO_GRADUS("ordo: shipped, partial, planned, conversations");
    per (k = ZEPHYRUM; k < IV; k++)
    {
        (vacuum)negotia_facultatem_creare(m->n, ng, lit("f",
            c->piscina), status[k], lit("", c->piscina));
    }
    (vacuum)negotia_colloquium_adiungere(m->n, ng, lit("s1",
        c->piscina), lit("", c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(negotia_summarium(m->n, ng,
        c->piscina), "2 shipped \302\267 1 partial \302\267 1 planned"
        " \302\267 1 conversation");
    (vacuum)negotia_colloquium_adiungere(m->n, ng, lit("s2",
        c->piscina), lit("", c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(negotia_summarium(m->n, ng,
        c->piscina), "2 shipped \302\267 1 partial \302\267 1 planned"
        " \302\267 2 conversations");

    CREDO_GRADUS("colloquia sola");
    (vacuum)negotia_colloquium_adiungere(m->n, vacuum_ng, lit("s3",
        c->piscina), lit("", c->piscina));
    CREDO_CHORDA_AEQUALIS_LITERIS(negotia_summarium(m->n, vacuum_ng,
        c->piscina), "1 conversation");
}

interior vacuum
_probare_versio (
    CredoContextus* c)
{
      Mundus* m = (Mundus*)c->datum;
     Volumen* v2;
     Negotia* n2;
         i32  numerator_alter;
      chorda  ng;
         s64  ante;
         s64  post;

    CREDO_NECESSE_NON_NIHIL(m->n);
    CREDO_GRADUS("scriptura propria versionem movet");
    ante = negotia_versio(m->n);
    ng   = negotia_negotium_creare(m->n, lit("a", c->piscina),
        lit("", c->piscina));
    post = negotia_versio(m->n);
    CREDO_VERUM(post > ante);

    CREDO_GRADUS("mutatio par: versio manet");
    CREDO_VERUM(negotia_negotium_mutare(m->n, ng, lit("a", c->piscina),
        lit("", c->piscina)));
    CREDO_VERUM(negotia_versio(m->n) == post);

    CREDO_GRADUS("scriptor alius: post renovare solum");
    v2 = volumen_aperire(c->piscina, volumen_via(m->volumen));
    CREDO_NECESSE_NON_NIHIL(v2);
    numerator_alter = C;
    n2 = negotia_aperire(v2, moneta_numerator, &numerator_alter,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(n2);
    (vacuum)negotia_facultatem_creare(n2, ng, lit("f", c->piscina),
        NEGOTIA_PROPOSITA, lit("", c->piscina));
    CREDO_VERUM(negotia_versio(m->n) == post);
    CREDO_VERUM(negotia_renovare(m->n));
    ante = negotia_versio(m->n);
    CREDO_VERUM(ante > post);

    CREDO_GRADUS("renovare vacua: versio manet");
    CREDO_FALSUM(negotia_renovare(m->n));
    CREDO_VERUM(negotia_versio(m->n) == ante);
    volumen_claudere(v2);
}

/* T6: via colloquii in nexu - servata, par nihil scribit, vacua non
 * delet, nova vincit, post reaperitionem manet */
interior vacuum
_probare_viam_colloquii (
    CredoContextus* c)
{
      Mundus* m = (Mundus*)c->datum;
     Negotia* n2;
      chorda  ng;
         s64  seq;

    CREDO_NECESSE_NON_NIHIL(m->n);
    ng = negotia_negotium_creare(m->n, lit("a", c->piscina),
        lit("", c->piscina));
    CREDO_VERUM(negotia_colloquium_adiungere(m->n, ng, lit("s1",
        c->piscina), lit("/a/b c", c->piscina)));
    CREDO_CHORDA_AEQUALIS_LITERIS(negotia_colloquii_via(m->n, ng, lit(
        "s1", c->piscina)), "/a/b c");

    CREDO_GRADUS("par et vacua: nihil scriptum, via manet");
    seq = volumen_seq_ultimum(m->volumen);
    CREDO_VERUM(negotia_colloquium_adiungere(m->n, ng, lit("s1",
        c->piscina), lit("/a/b c", c->piscina)));
    CREDO_VERUM(negotia_colloquium_adiungere(m->n, ng, lit("s1",
        c->piscina), lit("", c->piscina)));
    CREDO_AEQUALIS_S64(volumen_seq_ultimum(m->volumen), seq);
    CREDO_CHORDA_AEQUALIS_LITERIS(negotia_colloquii_via(m->n, ng, lit(
        "s1", c->piscina)), "/a/b c");

    CREDO_GRADUS("via nova vincit; colloquium unum");
    CREDO_VERUM(negotia_colloquium_adiungere(m->n, ng, lit("s1",
        c->piscina), lit("/c", c->piscina)));
    CREDO_VERUM(volumen_seq_ultimum(m->volumen) > seq);
    CREDO_CHORDA_AEQUALIS_LITERIS(negotia_colloquii_via(m->n, ng, lit(
        "s1", c->piscina)), "/c");
    CREDO_AEQUALIS_I32(xar_numerus(negotia_colloquia(m->n, ng,
        c->piscina)), I);
    CREDO_AEQUALIS_I32(negotia_colloquii_via(m->n, ng, lit("s9",
        c->piscina)).mensura, ZEPHYRUM);

    CREDO_GRADUS("reapertum");
    n2 = negotia_aperire(m->volumen, moneta_numerator, &m->numerator,
        c->piscina);
    CREDO_NECESSE_NON_NIHIL(n2);
    CREDO_CHORDA_AEQUALIS_LITERIS(negotia_colloquii_via(n2, ng,
        lit("s1",
        c->piscina)), "/c");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "creare et legere", _probare_creare, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "status", _probare_status, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "revisiones", _probare_revisiones, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "ordo activitatis", _probare_ordo, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "colloquia", _probare_colloquia, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "refutationes", _probare_refutationes, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "suffixum ambiguum", _probare_suffixum, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "alius scriptor", _probare_alius_scriptor, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "iterum apertum", _probare_iterum_apertum, mundum_parare,
        mundum_purgare,
        NIHIL, NIHIL },
    { "summarium", _probare_summarium, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "versio", _probare_versio, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "via colloquii", _probare_viam_colloquii, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (
    vacuum)
{
    redde credo_suitam_currere("negotia", SECTIONES);
}
