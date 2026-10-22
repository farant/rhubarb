/* probatio_vicus_indagatio.c - INDAGATIO in vico (genera-datorum D5,
 * S3b). Compositio VERA (vicus_applicatio, tabula 3 = scriba |
 * scriba); fixtura in volumine ante vicum (res agendae, persona cum
 * notis, liber diei, liber communis - ut probatio_indagatio_visus).
 * Cmd+Shift+F (brevitas hospitis: latus unum, prompta aperta, Cmd+F
 * nihil, etiam terminali focato), '$search' et '$search(verba)',
 * '$via' (argumentum transiens: identitas sine argumento), cyclus
 * Cmd+] / Cmd+[, porta Enter (dies,
 * persona, res, pagina libri communis in latere sinistro), Esc non
 * consumptum latus tollit, registrum commune (res et persona per
 * latera calendarii et nomenclatoris statim quaeruntur), reapertura
 * (q ex semita servata). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"
#include "thema.h"
#include "volumen.h"
#include "insula.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "moneta.h"
#include "fasti.h"
#include "agenda.h"
#include "personae.h"
#include "tabula_characterum.h"
#include "scriba_documentum.h"
#include "scriba_liber.h"
#include "scriba_applicatio.h"
#include "kalendarium.h"
#include "nomenclator.h"
#include "indagatio.h"
#include "indagatio_visus.h"
#include "vicus.h"
#include "vicus_applicatio.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_INDAGATIONIS "3_dextrum_indagatio"

nomen structura {
                Volumen* volumen;
    InternamentumChorda* intern;
                Piscina* piscina;
        VicusApplicatio  app;
                  Vicus* v;
             ManusLudus* m;
            Dispensator* d;
                    i32  numerator_agendae;
                    i32  numerator_personarum;
} Mundus;

interior chorda
lit (
    constans character* s,
               Piscina* piscina)
{
    redde chorda_ex_literis(s, piscina);
}

interior b32
paginam_scribere (
                 Mundus* x,
     constans character* praefixum,
     constans character* textus)
{
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;

    l = praefixum ? scriba_liber_aperire_in_spatio(x->piscina,
        x->intern, x->volumen, praefixum, XL, X)
        : scriba_liber_aperire(x->piscina, x->intern, x->volumen, XL,
        X);
    d = l ? scriba_liber_pagina(l, lit("1", x->piscina)) : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, x->piscina,
        d->latitudo, d->altitudo, textus);
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

/* fixtura ante vicum: res "1".."5" ("Rhubarb ...", "1" die XXI X),
 * "6" "Call the plumber", persona "1" Anna Rossi cum notis, liber
 * diei 2026-10-19, liber communis paginae "1" et "2" ("Pantry") */
/* pagina nova libri communis ("2") cum textu */
interior b32
paginam_novam_scribere (
                 Mundus* x,
     constans character* textus)
{
          ScribaLiber* l;
     ScribaDocumentum* d;
    TabulaCharacterum  folium;
               chorda  titulus;

    l = scriba_liber_aperire(x->piscina, x->intern, x->volumen, XL, X);
    titulus.datum = NIHIL;
    titulus.mensura = ZEPHYRUM;
    si (l)
    {
        titulus = scriba_liber_pagina_nova(l);
    }
    d = titulus.mensura > ZEPHYRUM ? scriba_liber_pagina(l, titulus)
                                   : NIHIL;
    si (!d)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, x->piscina,
        d->latitudo, d->altitudo, textus);
    redde scriba_documentum_committere(d, &folium) > ZEPHYRUM;
}

interior b32
fixturam_scribere (
    Mundus* x)
{
    constans character* constans tituli[] = {
        "Rhubarb tart", "Rhubarb jam", "Rhubarb pie", "Rhubarb wine",
        "Rhubarb soup"
    };
       AgendaDocumentum* agenda;
     PersonaeDocumentum* personae;
                 chorda  id;
                   Dies  d;
                    i32  i;

    agenda = agenda_aperire(x->piscina, x->intern, x->volumen, "agenda",
        ZEPHYRUM);
    personae = personae_aperire(x->piscina, x->intern, x->volumen,
        "personae", ZEPHYRUM);
    si (!agenda || !personae)
    {
        redde FALSUM;
    }
    agenda_monetam_ponere(agenda, moneta_numerator,
        &x->numerator_agendae);
    personae_monetam_ponere(personae, moneta_numerator,
        &x->numerator_personarum);
    per (i = ZEPHYRUM; i < V; i++)
    {
        id = agenda_rem_creare(agenda, lit(tituli[i], x->piscina));
        si (   id.mensura == ZEPHYRUM
            || !agenda_rubricas_ponere(agenda, id, lit("kitchen",
                   x->piscina)))
        {
            redde FALSUM;
        }
    }
    d = fasti_dies(MMXXVI, X, XXI);
    (vacuum)agenda_diem_ponere(agenda, lit("1", x->piscina), &d);
    (vacuum)agenda_rem_creare(agenda, lit("Call the plumber",
        x->piscina));
    (vacuum)personae_hominem_creare(personae, lit("Anna", x->piscina),
        lit("Rossi", x->piscina));
    redde paginam_scribere(x, "persona/1", "likes rhubarb")
        && paginam_scribere(x, "dies/2026-10-19",
               "rhubarb crumble recipe")
        && paginam_scribere(x, NIHIL, "Groceries\nrhubarb, sugar")
        && paginam_novam_scribere(x, "Pantry\nrhubarb syrup");
}

interior b32
vicum_aedificare (
                Mundus* x,
       VicusApplicatio* app)
{
    si (!vicus_applicatio_aedificare(app, x->piscina, x->intern,
            x->volumen, CDLXXX, CDLXXX + VICUS_ALTITUDO_TABULARUM))
    {
        redde FALSUM;
    }
    x->v = app->vicus;
    x->d = app->d;
    x->m = manus_ludus_creare(x->piscina, app->d);
    si (!x->m || !vicus_activam_ponere(x->v, "3"))
    {
        redde FALSUM;
    }
    dispensator_recomponere(x->d);
    redde vicus_focum_ponere(x->v, VICUS_SINISTRUM);
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(x, ZEPHYRUM, magnitudo(Mundus));
    c->datum    = x;
    x->piscina  = c->piscina;
    x->intern   = internamentum_creare(c->piscina);
    thema_initiare();
    (vacuum)setenv("SHELL", "/bin/sh", I);
    (vacuum)setenv("RHUBARB_SANCTI", "probationes/fixa/sancti", I);
    x->volumen = volumen_temporarium(c->piscina,
        "probatio_vicus_indagatio");
    si (   !x->volumen || !fixturam_scribere(x)
        || !vicum_aedificare(x, &x->app))
    {
        x->v = NIHIL;
    }
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

interior i32
numerus_acervi (
    Mundus* x)
{
    redde xar_numerus(vicus_activa(x->v)->acervus);
}

interior VicusLatus*
frons (
    Mundus* x)
{
    redde vicus_latus(vicus_activa(x->v), VICUS_DEXTRUM);
}

interior b32
frons_est (
                 Mundus* x,
     constans character* id)
{
    VicusLatus* d;

    d = frons(x);
    redde d && d->montata && chorda_aequalis_literis(d->id, id);
}

interior b32
focus_dexter (
    Mundus* x)
{
    redde vicus_latus_focatum(x->v) == frons(x);
}

/* campus rami indagationis; valor NIHIL = absens aut vacuus */
interior b32
campus_est (
                 Mundus* x,
     constans character* titulus,
     constans character* valor)
{
    InsulaRamus  r;
         chorda* a;

    r = insula_ramus(x->v->repo, "indagatio", ID_INDAGATIONIS);
    a = insula_ramus_attributum(&r, titulus);
    si (!valor)
    {
        redde !a || a->mensura == ZEPHYRUM;
    }
    redde a && chorda_aequalis_literis(*a, valor);
}

interior vacuum
pulsare (
    Mundus* x)
{
    (vacuum)vicus_pulsare(x->v);
    dispensator_recomponere(x->d);
}

/* ictus in cellulam paginae sinistrae */
interior vacuum
ictus (
    Mundus* x,
       s32  linea,
       s32  columna)
{
    (vacuum)manus_ludus_premere_ad(x->m, VI + columna * VI + III,
        VICUS_ALTITUDO_TABULARUM + VIII + linea * VIII + IV);
}

/* iussa in pagina sinistra ante usum (ut probatio_vicus_acervus):
 * ab linea II, una per lineam ('\n' = Enter); focus sinister
 * (mundus) */
interior vacuum
iussa_scribere (
                 Mundus* x,
     constans character* textus)
{
    character linea[LXIV];
          i32 n;

    ictus(x, II, ZEPHYRUM);
    (vacuum)manus_ludus_scribere(x->m, "i");
    dum (*textus)
    {
        per (n = ZEPHYRUM; textus[n] && textus[n] != '\n' && n < LXIII;
             n++)
        {
            linea[n] = textus[n];
        }
        linea[n] = '\0';
        (vacuum)manus_ludus_scribere(x->m, linea);
        textus += n;
        si (*textus == '\n')
        {
            (vacuum)manus_ludus_clavem(x->m, (character)XIII, ZEPHYRUM);
            textus++;
        }
    }
    (vacuum)manus_ludus_clavem(x->m, (character)XXVII, ZEPHYRUM);
}

/* ictus in iusso lineae (ictus primus etiam agit); pulsus applicat */
interior vacuum
iussum (
    Mundus* x,
       s32  linea)
{
    ictus(x, linea, I);
    pulsare(x);
}

/* Cmd+Shift+F (runa sine maiuscula, ut fenestra dat) */
interior vacuum
advocare (
    Mundus* x)
{
    (vacuum)manus_ludus_clavem(x->m, 'f', MOD_SUPER | MOD_SHIFT);
    pulsare(x);
}

interior vacuum
clavis (
       Mundus* x,
    character  typus)
{
    (vacuum)manus_ludus_clavem(x->m, typus, ZEPHYRUM);
    pulsare(x);
}

/* quaestio per promptam: Cmd+Shift+F, prompta purgata, verba, Enter */
interior vacuum
quaerere (
                 Mundus* x,
     constans character* verba)
{
    i32 i;

    advocare(x);
    per (i = ZEPHYRUM; i < LXIV && !campus_est(x, "prompta_textus",
             NIHIL); i++)
    {
        (vacuum)manus_ludus_clavem(x->m, (character)VIII, ZEPHYRUM);
    }
    (vacuum)manus_ludus_scribere(x->m, verba);
    clavis(x, (character)XIII);
}

interior b32
genus_frontis (
                 Mundus* x,
     constans character* genus)
{
    VicusLatus* d;

    d = frons(x);
    redde d && d->montata && chorda_aequalis_literis(d->genus, genus);
}

interior b32
index_continet (
                 Mundus* x,
     constans character* textus)
{
    chorda c;
       b32 inventum;

    c = volumen_praesens_legere(x->volumen, lit("vicus/latera",
        x->piscina), x->piscina, &inventum);
    redde inventum && chorda_continet(c, lit(textus, x->piscina));
}

/* inventa omnia verborum in indice lateris indagationis */
interior i32
inventa (
                 Mundus* x,
     constans character* verba)
{
                  VicusLatus* l;
            IndagatioPetitio  p;
            IndagatioFructus* f;
                         i32  n;
                         i32  i;
                         i32  k;

    n = ZEPHYRUM;
    per (k = ZEPHYRUM; k < numerus_acervi(x); k++)
    {
        l = (VicusLatus*)xar_obtinere(vicus_activa(x->v)->acervus, k);
        si (!chorda_aequalis_literis(l->id, ID_INDAGATIONIS))
        {
            perge;
        }
        memset(&p, ZEPHYRUM, magnitudo(p));
        p.verba      = lit(verba, x->piscina);
        p.capacitas  = V;
        p.ordo       = INDAGATIO_ORDO_OPTIMUS;
        f = indagatio_quaerere(((IndagatioMontatio*)l->montatio)->index,
            &p, x->piscina);
        per (i = ZEPHYRUM; f && i < xar_numerus(f->greges); i++)
        {
            n += ((IndagatioGrex*)xar_obtinere(f->greges, i))->numerus;
        }
    }
    redde n;
}

/* Cmd+Shift+F: latus unum, focus dexter, prompta aperta; Cmd+F
 * nihil */
interior vacuum
_sectio_brevitas (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);
    CREDO_AEQUALIS_I32(numerus_acervi(x), I);

    CREDO_GRADUS("Cmd+F sine Shift: nihil");
    (vacuum)manus_ludus_clavem(x->m, 'f', MOD_SUPER);
    pulsare(x);
    CREDO_AEQUALIS_I32(numerus_acervi(x), I);
    CREDO_VERUM(frons_est(x, "3_dextrum_scriba"));

    CREDO_GRADUS("Cmd+Shift+F: petitio, deinde latus cum prompta");
    (vacuum)manus_ludus_clavem(x->m, 'f', MOD_SUPER | MOD_SHIFT);
    CREDO_AEQUALIS_I32(numerus_acervi(x), I);
    pulsare(x);
    CREDO_AEQUALIS_I32(numerus_acervi(x), II);
    CREDO_VERUM(frons_est(x, ID_INDAGATIONIS));
    CREDO_VERUM(focus_dexter(x));
    CREDO_VERUM(campus_est(x, "prompta", "quaestio"));
    CREDO_VERUM(chorda_vacua(frons(x)->argumentum));

    CREDO_GRADUS("scribere et Enter: q committitur");
    (vacuum)manus_ludus_scribere(x->m, "rhubarb");
    clavis(x, (character)XIII);
    CREDO_VERUM(campus_est(x, "q", "rhubarb"));
    CREDO_FALSUM(campus_est(x, "prompta", "quaestio"));

    CREDO_GRADUS("iterum ex sinistro: idem latus, q in prompta");
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_SINISTRUM));
    advocare(x);
    CREDO_AEQUALIS_I32(numerus_acervi(x), II);
    CREDO_VERUM(focus_dexter(x));
    CREDO_VERUM(campus_est(x, "prompta", "quaestio"));
    CREDO_VERUM(campus_est(x, "prompta_textus", "rhubarb"));

    CREDO_GRADUS("terminale focatum (Cmd omnia devorat): hospes capit");
    /* tabula 1 = scriba | terminale */
    CREDO_VERUM(vicus_activam_ponere(x->v, "1"));
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_DEXTRUM));
    dispensator_recomponere(x->d);
    CREDO_NECESSE(chorda_aequalis_literis(vicus_latus_focatum(
        x->v)->genus, "terminale"));
    advocare(x);
    CREDO_VERUM(frons_est(x, "1_dextrum_indagatio"));
    CREDO_VERUM(focus_dexter(x));
}

/* '$search(verba)' (ut $via: latus novum focum capit, praesens non),
 * '$search' (ut Cmd+Shift+F),
 * '$via(indagatio?q=..)': latus unum, argumentum nullum */
interior vacuum
_sectio_iussa (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);
    iussa_scribere(x, "$search(rhubarb tart)\n$search(plumber)\n$search"
        "\n$via(indagatio?q=pie)");

    CREDO_GRADUS("'$search(rhubarb tart)': latus novum (focus eius)");
    iussum(x, II);
    CREDO_AEQUALIS_I32(numerus_acervi(x), II);
    CREDO_VERUM(frons_est(x, ID_INDAGATIONIS));
    CREDO_VERUM(campus_est(x, "q", "rhubarb tart"));
    CREDO_VERUM(focus_dexter(x));
    CREDO_VERUM(chorda_vacua(frons(x)->argumentum));

    CREDO_GRADUS("'$search(plumber)': idem latus, focus manet");
    CREDO_VERUM(vicus_focum_ponere(x->v, VICUS_SINISTRUM));
    iussum(x, III);
    CREDO_VERUM(campus_est(x, "q", "plumber"));
    CREDO_FALSUM(focus_dexter(x));
    CREDO_VERUM(chorda_vacua(frons(x)->argumentum));

    CREDO_GRADUS("'$search' sine argumento: prompta, focus dexter");
    iussum(x, IV);
    CREDO_AEQUALIS_I32(numerus_acervi(x), II);
    CREDO_VERUM(focus_dexter(x));
    CREDO_VERUM(campus_est(x, "prompta", "quaestio"));
    CREDO_VERUM(campus_est(x, "q", "plumber"));

    CREDO_GRADUS("'$via(indagatio?q=pie)': idem latus");
    iussum(x, V);
    CREDO_AEQUALIS_I32(numerus_acervi(x), II);
    CREDO_VERUM(frons_est(x, ID_INDAGATIONIS));
    CREDO_VERUM(campus_est(x, "q", "pie"));
    CREDO_VERUM(chorda_vacua(frons(x)->argumentum));
    CREDO_FALSUM(index_continet(x, "argumentum=\"rhubarb"));
    CREDO_FALSUM(index_continet(x, "argumentum=\"indagatio"));
}

/* cyclus: domus -> admonitiones -> colloquia -> agenda -> indagatio
 * -> domus; latus '$search(verba)' cycli est (argumentum nullum) */
interior vacuum
_sectio_cyclus (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);
    iussa_scribere(x, "$search(rhubarb)");

    CREDO_GRADUS("Cmd+] quater: indagatio");
    (vacuum)manus_ludus_clavem(x->m, ']', MOD_SUPER);
    (vacuum)manus_ludus_clavem(x->m, ']', MOD_SUPER);
    (vacuum)manus_ludus_clavem(x->m, ']', MOD_SUPER);
    (vacuum)manus_ludus_clavem(x->m, ']', MOD_SUPER);
    pulsare(x);
    CREDO_VERUM(frons_est(x, ID_INDAGATIONIS));
    CREDO_VERUM(focus_dexter(x));
    (vacuum)manus_ludus_clavem(x->m, ']', MOD_SUPER);
    pulsare(x);
    CREDO_AEQUALIS_I32(numerus_acervi(x), I);
    CREDO_VERUM(frons_est(x, "3_dextrum_scriba"));

    CREDO_GRADUS("Cmd+[ a domo: indagatio prima");
    (vacuum)manus_ludus_clavem(x->m, '[', MOD_SUPER);
    pulsare(x);
    CREDO_AEQUALIS_I32(numerus_acervi(x), II);
    CREDO_VERUM(frons_est(x, ID_INDAGATIONIS));
    (vacuum)manus_ludus_clavem(x->m, ']', MOD_SUPER);
    pulsare(x);
    CREDO_AEQUALIS_I32(numerus_acervi(x), I);

    CREDO_GRADUS("'$search(rhubarb)' deinde Cmd+]: domus");
    iussum(x, II);
    CREDO_AEQUALIS_I32(numerus_acervi(x), II);
    (vacuum)manus_ludus_clavem(x->m, ']', MOD_SUPER);
    pulsare(x);
    CREDO_AEQUALIS_I32(numerus_acervi(x), I);
    CREDO_VERUM(frons_est(x, "3_dextrum_scriba"));
}

/* Enter in invento: porta ut $via (dies, persona, res), pagina libri
 * communis ut '$scriba(nomen)' */
interior vacuum
_sectio_porta (
    CredoContextus* c)
{
                 Mundus* x;
    KalendariumMontatio* k;
                   Dies  d;
            InsulaRamus  r;
                 chorda* a;
         ScribaMontatio* sm;
                    i32  n;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);

    CREDO_GRADUS("notae diei: calendarium in die");
    quaerere(x, "crumble");
    CREDO_VERUM(campus_est(x, "q", "crumble"));
    clavis(x, (character)XIII);
    CREDO_NECESSE(genus_frontis(x, "calendarium"));
    CREDO_AEQUALIS_I32(numerus_acervi(x), III);
    k = (KalendariumMontatio*)frons(x)->montatio;
    d = kalendarium_dies_electus(k);
    CREDO_VERUM(d.annus == MMXXVI && d.mensis == X && d.dies == XIX);

    CREDO_GRADUS("res: calendarium in die rei");
    quaerere(x, "tart");
    CREDO_VERUM(frons_est(x, ID_INDAGATIONIS));
    clavis(x, (character)XIII);
    CREDO_NECESSE(genus_frontis(x, "calendarium"));
    CREDO_AEQUALIS_I32(numerus_acervi(x), III);
    k = (KalendariumMontatio*)frons(x)->montatio;
    d = kalendarium_dies_electus(k);
    CREDO_VERUM(d.annus == MMXXVI && d.mensis == X && d.dies == XXI);
    a = insula_ramus_attributum(&k->ramus, "res_electa");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "1"));

    CREDO_GRADUS("persona: nomenclator in persona");
    quaerere(x, "Anna");
    clavis(x, (character)XIII);
    CREDO_NECESSE(genus_frontis(x, "nomenclator"));
    r = ((NomenclatorMontatio*)frons(x)->montatio)->ramus;
    a = insula_ramus_attributum(&r, "persona_aperta");
    CREDO_VERUM(a && chorda_aequalis_literis(*a, "1"));

    CREDO_GRADUS("liber communis: pagina in latere sinistro");
    quaerere(x, "Pantry");
    n = numerus_acervi(x);
    clavis(x, (character)XIII);
    CREDO_AEQUALIS_I32(numerus_acervi(x), n);
    CREDO_VERUM(frons_est(x, ID_INDAGATIONIS));
    CREDO_VERUM(vicus_latus_focatum(x->v)
        == vicus_latus(vicus_activa(x->v), VICUS_SINISTRUM));
    sm = (ScribaMontatio*)vicus_latus(vicus_activa(x->v),
        VICUS_SINISTRUM)->montatio;
    CREDO_VERUM(sm->doc == scriba_liber_pagina(sm->liber,
        lit("2", x->piscina)));
}

/* Esc gradibus: prompta, q, deinde latus tollitur (non consumptum) */
interior vacuum
_sectio_effugium (
    CredoContextus* c)
{
    Mundus* x;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);
    quaerere(x, "rhubarb");
    CREDO_AEQUALIS_I32(numerus_acervi(x), II);

    CREDO_GRADUS("Esc: q vacua, latus manet");
    clavis(x, (character)XXVII);
    CREDO_VERUM(campus_est(x, "q", NIHIL));
    CREDO_AEQUALIS_I32(numerus_acervi(x), II);
    CREDO_VERUM(frons_est(x, ID_INDAGATIONIS));

    CREDO_GRADUS("Esc iterum: latus tollitur");
    clavis(x, (character)XXVII);
    CREDO_AEQUALIS_I32(numerus_acervi(x), I);
    CREDO_VERUM(frons_est(x, "3_dextrum_scriba"));
}

/* registrum commune: res nova in documento calendarii, persona nova
 * in documento nomenclatoris - statim quaeruntur */
interior vacuum
_sectio_communia (
    CredoContextus* c)
{
                 Mundus* x;
    KalendariumMontatio* k;
    NomenclatorMontatio* n;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);
    quaerere(x, "rhubarb");
    CREDO_AEQUALIS_I32(inventa(x, "fool"), ZEPHYRUM);

    CREDO_GRADUS("res per calendarium");
    CREDO_VERUM(vicus_acervo_aperire(x->v, "calendarium", "2026-10"));
    pulsare(x);
    CREDO_NECESSE(genus_frontis(x, "calendarium"));
    k = (KalendariumMontatio*)frons(x)->montatio;
    CREDO_VERUM(agenda_rem_creare(k->documentum, lit("Rhubarb fool",
        x->piscina)).mensura > ZEPHYRUM);
    CREDO_AEQUALIS_I32(inventa(x, "fool"), I);

    CREDO_GRADUS("persona per nomenclatorem");
    CREDO_AEQUALIS_I32(inventa(x, "Zeno"), ZEPHYRUM);
    CREDO_VERUM(vicus_acervo_aperire(x->v, "nomenclator", NIHIL));
    pulsare(x);
    CREDO_NECESSE(genus_frontis(x, "nomenclator"));
    n = (NomenclatorMontatio*)frons(x)->montatio;
    CREDO_VERUM(personae_hominem_creare(n->documentum, lit("Zeno",
        x->piscina), lit("Elea", x->piscina)).mensura > ZEPHYRUM);
    CREDO_AEQUALIS_I32(inventa(x, "Zeno"), I);
}

/* reapertura: latus sine argumento, q ex semita servata */
interior vacuum
_sectio_restitutio (
    CredoContextus* c)
{
             Mundus* x;
    VicusApplicatio  altera;

    x = (Mundus*)c->datum;
    CREDO_NECESSE(x && x->v);
    iussa_scribere(x, "$search(rhubarb tart)");
    iussum(x, II);
    CREDO_VERUM(campus_est(x, "q", "rhubarb tart"));

    CREDO_NECESSE(vicum_aedificare(x, &altera));
    pulsare(x);
    CREDO_AEQUALIS_I32(numerus_acervi(x), II);
    CREDO_VERUM(frons_est(x, ID_INDAGATIONIS));
    CREDO_VERUM(chorda_vacua(frons(x)->argumentum));
    CREDO_VERUM(campus_est(x, "q", "rhubarb tart"));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "brevitas", _sectio_brevitas, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "iussa", _sectio_iussa, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "cyclus", _sectio_cyclus, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "porta", _sectio_porta, mundum_parare, mundum_purgare, NIHIL,
        NIHIL },
    { "effugium", _sectio_effugium, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "communia", _sectio_communia, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { "reapertura", _sectio_restitutio, mundum_parare, mundum_purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("vicus_indagatio", SECTIONES);
}
