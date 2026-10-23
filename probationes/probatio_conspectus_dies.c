/* probatio_conspectus_dies.c - CONSPECTUS 'dies' (proiectiones D4b):
 * computatio pura (liturgia, natales, res, signa, libri dierum),
 * codex, registrum (irritatio per agendam, personas, librum diei),
 * COPIA (revocatio statum agendae vacat - conspectus titulos suos
 * tenere debet), sine fontibus. */

#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "xar.h"
#include "volumen.h"
#include "moneta.h"
#include "fasti.h"
#include "fasti_fontes.h"
#include "agenda.h"
#include "personae.h"
#include "calendarium_liturgicum.h"
#include "scriba_liber.h"
#include "scriba_documentum.h"
#include "tabula_characterum.h"
#include "conspectus.h"
#include "conspectus_dies.h"

#include <stdio.h>
#include <string.h>

nomen structura {
                 Volumen* volumen;
     InternamentumChorda* intern;
        AgendaDocumentum* agenda;
      PersonaeDocumentum* personae;
             FastiFontes* fasti;
   CalendariumLiturgicum* liturgia;
              DiesFontes  fontes;
     ConspectusRegistrum* repertorium;
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

interior Dies
diem (
    s32 d)
{
    redde fasti_dies(MMXXVI, X, d);
}

interior chorda
clavem (
        s32  d,
    Piscina* piscina)
{
    character buffer[XVI];

    sprintf(buffer, "2026-10-%02d", (integer)d);
    redde chorda_ex_literis(buffer, piscina);
}

interior vacuum
mundum_parare (
    CredoContextus* c)
{
    Mundus* m;
       Xar* vitia;

    m = (Mundus*)piscina_allocare(c->piscina, magnitudo(Mundus));
    memset(m, ZEPHYRUM, magnitudo(Mundus));
    c->datum   = m;
    m->intern  = internamentum_creare(c->piscina);
    m->volumen = volumen_temporarium(c->piscina,
        "probatio_conspectus_dies");
    si (!m->volumen)
    {
        redde;
    }
    /* intervallum II: checkpoints crebri - revocatio ex checkpoint
     * (ordo allocationis alius quam historiae; vide 'copia') */
    m->agenda = agenda_aperire(c->piscina, m->intern, m->volumen,
        "agenda", II);
    agenda_monetam_ponere(m->agenda, moneta_numerator,
        &m->numerator_agendae);
    m->personae = personae_aperire(c->piscina, m->intern, m->volumen,
        "personae", ZEPHYRUM);
    personae_monetam_ponere(m->personae, moneta_numerator,
        &m->numerator_personarum);
    m->fasti = fasti_fontes_creare(c->piscina);
    (vacuum)fasti_fontes_addere(m->fasti, "nomenclator",
        personae_fasti_legere, NIHIL, m->personae);
    m->liturgia         = calendarium_creare(c->piscina);
    m->fontes.agenda    = m->agenda;
    m->fontes.fontes    = m->fasti;
    m->fontes.liturgia  = m->liturgia;
    m->fontes.volumen   = m->volumen;
    m->fontes.intern    = m->intern;
    m->repertorium = conspectus_registrum_creare(c->piscina, m->intern,
        m->volumen, ZEPHYRUM);
    si (m->repertorium && (!conspectus_dies_registrare(m->repertorium,
            &m->fontes, &vitia) || !conspectus_parare(m->repertorium,
            &vitia)))
    {
        m->repertorium = NIHIL;
    }
}

interior vacuum
mundum_purgare (
    CredoContextus* c)
{
    Mundus* m;

    m = (Mundus*)c->datum;
    si (m && m->volumen)
    {
        volumen_claudere(m->volumen);
    }
}

/* liber diei: pagina "1" cum textu */
interior b32
librum_scribere (
                 Mundus* m,
                    s32  d,
     constans character* textus,
                Piscina* p)
{
          ScribaLiber* l;
     ScribaDocumentum* doc;
    TabulaCharacterum  folium;
            character  praefixum[XXXII];

    sprintf(praefixum, "dies/2026-10-%02d", (integer)d);
    l   = scriba_liber_aperire_in_spatio(p, m->intern, m->volumen,
        praefixum, XL, XL);
    doc = l ? scriba_liber_pagina(l, chorda_ex_literis("1", p)) : NIHIL;
    si (!doc)
    {
        redde FALSUM;
    }
    tabula_ex_literis_cum_dimensionibus(&folium, p, doc->latitudo,
        doc->altitudo, textus);
    redde scriba_documentum_committere(doc, &folium) > ZEPHYRUM;
}

interior chorda
rem_creare_die (
                 Mundus* m,
     constans character* titulus,
                    s32  d,
                Piscina* p)
{
    chorda id;
      Dies dies;

    id    = agenda_rem_creare(m->agenda, lit(titulus, p));
    dies  = diem(d);
    (vacuum)agenda_diem_ponere(m->agenda, id, &dies);
    redde id;
}

interior constans DiesConspectus*
legere (
     Mundus* m,
        s32  d,
    Piscina* p)
{
    redde (constans DiesConspectus*)conspectus_legere(m->repertorium,
        "dies", clavem(d, p));
}

interior ConspectusStatus
statum (
     Mundus* m,
        s32  d,
    Piscina* p)
{
    redde conspectus_status(m->repertorium, "dies", clavem(d, p));
}

interior chorda
lineam (
    Xar* l,
    i32  i)
{
    redde *(chorda*)xar_obtinere(l, i);
}


/* ==================================================
 * Sectiones
 * ================================================== */

/* computatio pura: liturgia, natales, res, signa, libri */
interior vacuum
_probare_computationem (
    CredoContextus* c)
{
            Mundus* m;
    DiesConspectus* v;
            chorda  anna;
            chorda  b;
              Dies  natalis;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->volumen);
    anna    = personae_hominem_creare(m->personae, lit("Anna",
        c->piscina), lit("Rossi", c->piscina));
    natalis = fasti_dies(MCMLXXXVI, X, XV);
    CREDO_MAIOR_S64(personae_natalem_ponere(m->personae, anna,
        &natalis),
        ZEPHYRUM);
    (vacuum)rem_creare_die(m, "panem emere", XV, c->piscina);
    b = rem_creare_die(m, "epistulam scribere", XV, c->piscina);
    (vacuum)agenda_perficere(m->agenda, b, diem(XV));
    CREDO_NECESSE(librum_scribere(m, XV, "\n    prima nota\n      "
        "secunda\n\n    tertia", c->piscina));

    v = conspectus_dies_computare(&m->fontes, diem(XV), c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_VERUM(fasti_dies_valida(v->dies) && v->dies.dies == XV);
    /* XV X MMXXVI feria V: S. Teresia de Avila (memoria) */
    CREDO_VERUM(v->liturgia);
    CREDO_CHORDA_NON_VACUA(v->celebratio);
    CREDO_CHORDA_NON_VACUA(v->festum);
    /* natalis: annus notus -> aetas */
    CREDO_AEQUALIS_I32(xar_numerus(v->natales), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(lineam(v->natales, ZEPHYRUM),
        "Anna Rossi turns 40");
    /* res apertae (perfecta non); signa */
    CREDO_AEQUALIS_I32(xar_numerus(v->apertae), I);
    CREDO_CHORDA_AEQUALIS_LITERIS(lineam(v->apertae, ZEPHYRUM),
        "panem emere");
    CREDO_AEQUALIS_I32(v->signum.assignatae, II);
    CREDO_AEQUALIS_I32(v->signum.apertae, I);
    CREDO_AEQUALIS_I32(v->signum.perfectae, I);
    /* libri: vacuae omissae, indentatio communis remota */
    CREDO_AEQUALIS_I32(xar_numerus(v->lineae_libri), III);
    CREDO_CHORDA_AEQUALIS_LITERIS(lineam(v->lineae_libri, ZEPHYRUM),
        "prima nota");
    CREDO_CHORDA_AEQUALIS_LITERIS(lineam(v->lineae_libri, I),
        "  secunda");
    CREDO_CHORDA_AEQUALIS_LITERIS(lineam(v->lineae_libri, II),
        "tertia");

    /* XVIII X MMXXVI dominica: celebratio (dominica) sed festum nullum
     * in mensis cellula */
    v = conspectus_dies_computare(&m->fontes, diem(XVIII), c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_CHORDA_NON_VACUA(v->celebratio);
    CREDO_CHORDA_VACUA(v->festum);
    CREDO_AEQUALIS_I32(xar_numerus(v->natales), ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(v->lineae_libri), ZEPHYRUM);

    /* dies invalida */
    CREDO_NIHIL(conspectus_dies_computare(&m->fontes, fasti_dies(MMXXVI,
        XIII, XLV), c->piscina));
}

interior vacuum
_probare_codicem (
    CredoContextus* c)
{
            Mundus* m;
    DiesConspectus* v;
            chorda  codex;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->volumen);
    (vacuum)rem_creare_die(m, "panem \"emere\" & <alia>", XV,
        c->piscina);
    CREDO_NECESSE(librum_scribere(m, XV, "nota", c->piscina));
    v = conspectus_dies_computare(&m->fontes, diem(XV), c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    codex = conspectus_dies_codificare(v, c->piscina);
    CREDO_CHORDA_INCIPIT(codex, lit("<dies", c->piscina));
    CREDO_CHORDA_CONTINET(codex, lit("2026-10-15", c->piscina));
    CREDO_CHORDA_CONTINET(codex, lit("nota", c->piscina));
    CREDO_CHORDA_CONTINET(codex, lit("emere", c->piscina));
    /* determinatus: computatio iterata, codex idem */
    CREDO_CHORDA_AEQUALIS(conspectus_dies_codificare(
        conspectus_dies_computare(&m->fontes, diem(XV), c->piscina),
        c->piscina), codex);
}

interior vacuum
_probare_registrum (
    CredoContextus* c)
{
                     Mundus* m;
    constans DiesConspectus* v;
                     chorda  a;
                     chorda  anna;
                       Dies  d;
                        Xar* discrepantes;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    discrepantes  = xar_creare(c->piscina, (i32)magnitudo(chorda));
    a             = rem_creare_die(m, "panem emere", XIV, c->piscina);
    v             = legere(m, XIV, c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_AEQUALIS_I32(xar_numerus(v->apertae), I);
    (vacuum)legere(m, XV, c->piscina);
    (vacuum)legere(m, XVI, c->piscina);

    /* res mota: 14 et 16 */
    d = diem(XVI);
    (vacuum)agenda_diem_ponere(m->agenda, a, &d);
    CREDO_AEQUALIS_I32((i32)statum(m, XIV, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)statum(m, XVI, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)statum(m, XV, c->piscina),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32(xar_numerus(legere(m, XVI, c->piscina)->apertae),
        I);

    /* nota rei (vestigium): 16 solus */
    (vacuum)legere(m, XIV, c->piscina);
    (vacuum)agenda_notam_addere(m->agenda, a, diem(XVI), lit("lac",
        c->piscina));
    CREDO_AEQUALIS_I32((i32)statum(m, XVI, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)statum(m, XIV, c->piscina),
        (i32)CONSPECTUS_RECENS);

    /* perfectio die 17: 16 (signa) et 17 (perfectae), non 14 */
    (vacuum)legere(m, XIV, c->piscina);
    (vacuum)legere(m, XVI, c->piscina);
    (vacuum)legere(m, XVII, c->piscina);
    (vacuum)agenda_perficere(m->agenda, a, diem(XVII));
    CREDO_AEQUALIS_I32((i32)statum(m, XVII, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)statum(m, XVI, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)statum(m, XIV, c->piscina),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32(legere(m, XVII, c->piscina)->signum.perfectae,
        I);

    /* natalis: personae omnia */
    (vacuum)legere(m, XVI, c->piscina);
    anna = personae_hominem_creare(m->personae, lit("Anna", c->piscina),
        lit("Rossi", c->piscina));
    d    = fasti_dies(MCMLXXXVI, X, XV);
    (vacuum)personae_natalem_ponere(m->personae, anna, &d);
    CREDO_AEQUALIS_I32((i32)statum(m, XV, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32(xar_numerus(legere(m, XV, c->piscina)->natales),
        I);

    /* liber diei: 15 solus */
    (vacuum)legere(m, XIV, c->piscina);
    (vacuum)legere(m, XVI, c->piscina);
    CREDO_NECESSE(librum_scribere(m, XV, "nota diei", c->piscina));
    CREDO_AEQUALIS_I32((i32)statum(m, XV, c->piscina),
        (i32)CONSPECTUS_STALUM);
    CREDO_AEQUALIS_I32((i32)statum(m, XIV, c->piscina),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32((i32)statum(m, XVI, c->piscina),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_I32(xar_numerus(legere(m, XV, c->piscina)
        ->lineae_libri), I);

    CREDO_AEQUALIS_I32(conspectus_verificare(m->repertorium,
        discrepantes), ZEPHYRUM);
}

/* COPIA: revocatio statum agendae vacat et replicat (lib/agenda.c,
 * piscina 'status'); conspectus diei ALTERIUS, recens manens, titulos
 * suos tenere debet - non indices in memoriam agendae */
interior vacuum
_probare_copiam (
    CredoContextus* c)
{
                     Mundus* m;
    constans DiesConspectus* v;
                     chorda  b;
                        Xar* discrepantes;
                        i32  i;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    discrepantes = xar_creare(c->piscina, (i32)magnitudo(chorda));
    /* res altera PRIOR creata: in checkpoint ante 'panem emere'
     * scribitur, titulo suo NOVO (longiore) - restitutio titulos alibi
     * ponit quam historia */
    b = rem_creare_die(m, "b", XVI, c->piscina);
    (vacuum)rem_creare_die(m, "panem emere", XIV, c->piscina);
    v = legere(m, XIV, c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    per (i = ZEPHYRUM; i < X; i++)
    {
        (vacuum)agenda_titulum_ponere(m->agenda, b, lit((i % II)
            ? "epistula longior quam ante, multo longior" : "brevis",
            c->piscina));
        (vacuum)agenda_titulum_ponere(m->agenda, b, lit("epistula "
            "longissima omnium quae umquam scriptae sunt", c->piscina));
        (vacuum)agenda_revocare(m->agenda);
        (vacuum)agenda_reficere(m->agenda);
        (vacuum)agenda_revocare(m->agenda);
    }
    CREDO_AEQUALIS_I32((i32)statum(m, XIV, c->piscina),
        (i32)CONSPECTUS_RECENS);
    CREDO_AEQUALIS_PTR(legere(m, XIV, c->piscina), v);
    CREDO_CHORDA_AEQUALIS_LITERIS(lineam(v->apertae, ZEPHYRUM),
        "panem emere");
    CREDO_AEQUALIS_I32(conspectus_verificare(m->repertorium,
        discrepantes), ZEPHYRUM);
}

/* sine fontibus: liturgia sola aut nihil - nulla ruina */
interior vacuum
_probare_sine_fontibus (
    CredoContextus* c)
{
        DiesFontes  fontes;
    DiesConspectus* v;
            Mundus* m;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m);
    memset(&fontes, ZEPHYRUM, magnitudo(DiesFontes));
    fontes.intern = m->intern;
    v = conspectus_dies_computare(&fontes, diem(XV), c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_FALSUM(v->liturgia);
    CREDO_CHORDA_VACUA(v->celebratio);
    CREDO_AEQUALIS_I32(xar_numerus(v->apertae), ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(v->natales), ZEPHYRUM);
    CREDO_AEQUALIS_I32(xar_numerus(v->lineae_libri), ZEPHYRUM);
    CREDO_AEQUALIS_I32(v->signum.assignatae, ZEPHYRUM);
    fontes.liturgia = m->liturgia;
    v = conspectus_dies_computare(&fontes, diem(XV), c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    CREDO_VERUM(v->liturgia);
}

/* introitus parvus: liber diei L vicibus mutatus (historia replicatur
 * in lectione) - opus lectionis in scheda, introitus solum fructum
 * tenet (volumen Frani 10-23: dies cum libro ~124 KB, mensis super
 * tectum, menses eiciebant se ipsos) */
interior vacuum
_probare_magnitudinem (
    CredoContextus* c)
{
                      Mundus* m;
     constans DiesConspectus* v;
                   character  textus[CXXVIII];
              memoriae_index  ante;
              memoriae_index  usus;
                         i32  i;

    m = (Mundus*)c->datum;
    CREDO_NECESSE(m && m->repertorium);
    per (i = ZEPHYRUM; i < L; i++)
    {
        sprintf(textus, "ora et labora\nversio %u",
            (insignatus integer)i);
        CREDO_NECESSE(librum_scribere(m, XIV, textus, c->piscina));
    }
    ante  = conspectus_summa_usus(m->repertorium);
    v     = legere(m, XIV, c->piscina);
    CREDO_NECESSE_NON_NIHIL(v);
    usus  = conspectus_summa_usus(m->repertorium) - ante;
    imprimere("  introitus diei cum libro: %lu octeti\n",
        (longus insignatus)usus);
    CREDO_AEQUALIS_I32(xar_numerus(v->lineae_libri), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(lineam(v->lineae_libri, I),
        "versio 49");
    CREDO_VERUM(usus < VIII * MXXIV);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "computatio",     _probare_computationem, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "codex",          _probare_codicem,       mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "registrum",      _probare_registrum,     mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "copia",          _probare_copiam,        mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "magnitudo",      _probare_magnitudinem,  mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { "sine fontibus",  _probare_sine_fontibus, mundum_parare,
        mundum_purgare, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("conspectus_dies", SECTIONES);
}
