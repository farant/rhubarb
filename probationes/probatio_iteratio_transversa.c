/* probatio_iteratio_transversa.c - iteratio inter fontes (eventus
 * B6b; spec D6, FRAN Q3): sessio in terminali notata in dispositione
 * fenestrae iteratur, et vice versa per codificatorem (octeti, rivus).
 *
 * Toy in DUABUS dispositionibus: fenestra (pixela) et terminalis
 * (fines ad cellulas Moduli VI x VIII extensi, linea capitis una:
 * translatio_y VIII). Iteratio SEMANTICA statum finalem servat;
 * CRUDA non. Transitus terminalis: eventus -> codificator (cellula
 * per Modulum) -> octeti -> rivus (lectio pigra: relatio una, eventus
 * unus) -> dispensator. Inventum: dispositio non ad cellulas posita
 * ictum ad marginem perdit (centrum cellulae extra componentem).
 */
#include "latina.h"
#include "piscina.h"
#include "internamentum.h"
#include "xar.h"
#include "chorda_aedificator.h"
#include "dispensator.h"
#include "manus_ludus.h"
#include "rivus_terminalis.h"
#include "codificator_terminalis.h"
#include "ludus_toy.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define CELLULA_LATITUDO  VI
#define CELLULA_ALTITUDO  VIII

nomen structura {
     InsulaRepositorium* repo;
            Dispensator* d;
             ManusLudus* m;
              ToyStatus  toy;
} Sessio;

/* terminalis: fines ad cellulas + linea capitis; fenestra: pixela */
interior vacuum
sessio_creare (
                 Sessio* s,
                Piscina* piscina,
    InternamentumChorda* intern,
                    b32  terminalis,
                    b32  ad_cellulas)
{
    ActioRegistrum* reg;

    memset(s, ZEPHYRUM, magnitudo(Sessio));
    si (terminalis)
    {
        s->toy.translatio_y = CELLULA_ALTITUDO;
    }
    si (ad_cellulas)
    {
        s->toy.modulus_latitudo = CELLULA_LATITUDO;
        s->toy.modulus_altitudo = CELLULA_ALTITUDO;
    }
    s->repo = insula_repositorium_creare(piscina, intern,
        "<documentum/>", "<ephemera/>");
    reg = actio_registrum_creare(piscina, intern);
    toy_registrare(reg, &s->toy);
    s->d = dispensator_creare(piscina, intern, s->repo, reg,
        toy_componere, &s->toy, CCC);
    s->m = manus_ludus_creare(piscina, s->d);
}

interior chorda
status (
     Sessio* s,
    Piscina* piscina)
{
    chorda c;

    c = chorda_concatenare(
        insula_scribere(s->repo, INSULA_DURABILIS, piscina),
        insula_scribere(s->repo, INSULA_EPHEMERA, piscina), piscina);
    redde chorda_concatenare(c, dispensator_focus(s->d), piscina);
}

/* Transitus terminalis (ManusTraditio): codificator -> rivus ->
 * dispensator. Silentium post octetos (ESC solus). */
nomen structura {
    RivusTerminalis* rivus;
    CodificatorModi  modi;
            Piscina* piscina;
} Transitus;

interior vacuum
_transitum_creare (
    Transitus* t,
      Piscina* piscina,
          s32  cellula_codificatoris)
{
    i8 modi[RIVUS_MODI_MAXIMUM];

    memset(t, ZEPHYRUM, magnitudo(Transitus));
    t->piscina                = piscina;
    t->rivus                  = rivus_creare(piscina, CELLULA_LATITUDO,
        CELLULA_ALTITUDO);
    t->modi.mus               = CODIFICATOR_MUS_OMNIS;
    t->modi.cellula_latitudo  = cellula_codificatoris;
    t->modi.cellula_altitudo  = (cellula_codificatoris
        == CELLULA_LATITUDO)
        ? CELLULA_ALTITUDO : cellula_codificatoris;
    (vacuum)rivus_modos_intrare(t->rivus, RIVUS_MODUS_SUPER, modi,
        RIVUS_MODI_MAXIMUM);
}

interior vacuum
_transire (
              vacuum* ctx,
         Dispensator* d,
    constans Eventus* e)
{
             Transitus* t = (Transitus*)ctx;
     ChordaAedificator* a = chorda_aedificator_creare(t->piscina, LXIV);
                chorda  o;
               Eventus  x;

    (vacuum)codificator_eventa(&t->modi, e, I, a);
    o = chorda_aedificator_finire(a);
    (vacuum)rivus_tradere(t->rivus, o.datum, o.mensura);
    dum (rivus_eventum(t->rivus, e->tempus, &x))
    {
        dispensator_tractare(d, &x);
    }
    si (rivus_mora_ms(t->rivus) > ZEPHYRUM)
    {
        rivus_moram(t->rivus, e->tempus);
        dum (rivus_eventum(t->rivus, e->tempus, &x))
        {
            dispensator_tractare(d, &x);
        }
    }
}

interior Eventus
_murem (
    eventus_genus_t  genus,
                s64  tempus,
                s32  x,
                s32  y,
       mus_botton_t  botton)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus              = genus;
    e.tempus             = tempus;
    e.datum.mus.x        = x;
    e.datum.mus.y        = y;
    e.datum.mus.botton   = botton;
    e.datum.mus.pressio  = EVENTUS_PRESSIO_IGNOTA;
    redde e;
}

s32 principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                 Sessio  t;
                 Sessio  f;
                 Sessio  b;
                    Xar* notata_t;
                    Xar* notata_f;
                    Xar* div;
                 chorda  status_t;
                 chorda  status_f;
              Transitus  tr;
                    i32  i;

    piscina = piscina_generare_dynamicum("probatio_iteratio_transversa",
        LXIV * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    imprimere("\n--- A. sessio terminalis notata ---\n");
    sessio_creare(&t, piscina, intern, VERUM, VERUM);
    notata_t = xar_creare(piscina, (i32)magnitudo(EventusNotatum));
    dispensator_notarium_ponere(t.d, notata_t);
    {
        /* b1 ad marginem (columna 2: x 9 - intra b1 terminalis [6,60),
         * extra b1 fenestrae [10,60)); super, tractus in tabula; ESC */
        constans character* octeti =
            "\033[<0;2;4M\033[<0;2;4m"
            "\033[<35;21;8M"
            "\033[<0;21;8M\033[<32;22;9M\033[<32;23;10M\033[<0;23;10m"
            "\033";
        RivusTerminalis* r = rivus_creare(piscina, CELLULA_LATITUDO,
            CELLULA_ALTITUDO);
                     i8 modi[RIVUS_MODI_MAXIMUM];
                Eventus e;
                    s64 tempus = M;

        (vacuum)rivus_modos_intrare(r, RIVUS_MODUS_SUPER, modi,
            RIVUS_MODI_MAXIMUM);
        (vacuum)rivus_tradere(r, (constans i8*)octeti,
            (i32)strlen(octeti));
        dum (rivus_eventum(r, tempus, &e))
        {
            dispensator_tractare(t.d, &e);
            tempus += X;
        }
        rivus_moram(r, tempus);
        dum (rivus_eventum(r, tempus, &e))
        {
            dispensator_tractare(t.d, &e);
        }
    }
    status_t = status(&t, piscina);
    CREDO_NON_NIHIL(insula_attributum(t.repo, INSULA_EPHEMERA,
        "numerus"));
    CREDO_NON_NIHIL(insula_attributum(t.repo, INSULA_EPHEMERA, "fuga"));
    CREDO_CHORDA_AEQUALIS_LITERIS(*insula_attributum(t.repo,
        INSULA_DURABILIS, "puncta"), "3");

    imprimere("\n--- B. semantica in fenestra: status idem ---\n");
    sessio_creare(&b, piscina, intern, FALSUM, FALSUM);
    div = xar_creare(piscina, (i32)magnitudo(ManusDivergentia));
    /* b1 pressus et liberatus: positio cruda (9) in fenestra radix */
    CREDO_AEQUALIS_I32(manus_ludus_iterare(b.m, notata_t,
        MANUS_ITERATIO_SEMANTICA, div), II);
    CREDO_VERUM(chorda_aequalis(status(&b, piscina), status_t));

    imprimere("\n--- C. cruda in fenestra: b1 amissus ---\n");
    sessio_creare(&b, piscina, intern, FALSUM, FALSUM);
    (vacuum)manus_ludus_iterare(b.m, notata_t, MANUS_ITERATIO_CRUDA,
        NIHIL);
    CREDO_NIHIL(insula_attributum(b.repo, INSULA_EPHEMERA, "numerus"));
    CREDO_FALSUM(chorda_aequalis(status(&b, piscina), status_t));

    imprimere("\n--- D. sessio fenestrae notata (pixela) ---\n");
    sessio_creare(&f, piscina, intern, FALSUM, FALSUM);
    notata_f = xar_creare(piscina, (i32)magnitudo(EventusNotatum));
    dispensator_notarium_ponere(f.d, notata_f);
    {
        Eventus ev[IX];

        /* b1 ad marginem sinistram (locale x 0) */
        ev[0] = _murem(EVENTUS_MUS_DEPRESSUS, M, X, XV, MUS_SINISTER);
        ev[1] = _murem(EVENTUS_MUS_LIBERATUS, M + L, X, XV,
            MUS_SINISTER);
        ev[2] = _murem(EVENTUS_MUS_MOTUS, M + C, CXX, L,
            (mus_botton_t)ZEPHYRUM);
        ev[3] = _murem(EVENTUS_MUS_DEPRESSUS, M + CC, CXX, L,
            MUS_SINISTER);
        ev[4] = _murem(EVENTUS_MUS_MOTUS, M + CCXX, CXXV, LV,
            MUS_SINISTER);
        ev[5] = _murem(EVENTUS_MUS_MOTUS, M + CCXL, CXXXV, LXV,
            MUS_SINISTER);
        ev[6] = _murem(EVENTUS_MUS_LIBERATUS, M + CCC, CXXXV, LXV,
            MUS_SINISTER);
        memset(&ev[7], ZEPHYRUM, II * magnitudo(Eventus));
        ev[7].genus                = EVENTUS_CLAVIS_DEPRESSUS;
        ev[7].tempus               = M + CD;
        ev[7].datum.clavis.clavis  = CLAVIS_EFFUGIUM;
        ev[7].datum.clavis.typus   = (character)XXVII;
        ev[7].datum.clavis.codex   = EVENTUS_CODEX_EFFUGIUM;
        ev[8]                      = ev[7];
        ev[8].genus                = EVENTUS_CLAVIS_LIBERATUS;
        ev[8].tempus               = M + CDXX;
        ev[8].datum.clavis.actio   = EVENTUS_ACTIO_SOLUTA;
        per (i = ZEPHYRUM; i < IX; i++)
        {
            dispensator_tractare(f.d, &ev[i]);
        }
    }
    status_f = status(&f, piscina);
    CREDO_NON_NIHIL(insula_attributum(f.repo, INSULA_EPHEMERA,
        "numerus"));

    imprimere("\n--- E. per terminalem, ad cellulas: idem ---\n");
    sessio_creare(&b, piscina, intern, VERUM, VERUM);
    _transitum_creare(&tr, piscina, CELLULA_LATITUDO);
    (vacuum)manus_ludus_iterare_per(b.m, notata_f,
        MANUS_ITERATIO_SEMANTICA, NIHIL, _transire, &tr);
    CREDO_VERUM(chorda_aequalis(status(&b, piscina), status_f));
    CREDO_NON_NIHIL(insula_attributum(b.repo, INSULA_EPHEMERA,
        "numerus"));

    imprimere("\n--- F. NON ad cellulas: margo amissus ---\n");
    /* x 10 -> cellula 1 -> centrum 9: extra b1 [10,60) */
    sessio_creare(&b, piscina, intern, VERUM, FALSUM);
    _transitum_creare(&tr, piscina, CELLULA_LATITUDO);
    (vacuum)manus_ludus_iterare_per(b.m, notata_f,
        MANUS_ITERATIO_SEMANTICA, NIHIL, _transire, &tr);
    CREDO_NIHIL(insula_attributum(b.repo, INSULA_EPHEMERA, "numerus"));
    CREDO_FALSUM(chorda_aequalis(status(&b, piscina), status_f));

    imprimere("\n--- G. traditio NIHIL = iterare ---\n");
    sessio_creare(&b, piscina, intern, FALSUM, FALSUM);
    CREDO_AEQUALIS_I32(manus_ludus_iterare_per(b.m, notata_t,
        MANUS_ITERATIO_SEMANTICA, NIHIL, NIHIL, NIHIL), II);
    CREDO_VERUM(chorda_aequalis(status(&b, piscina), status_t));

    imprimere("\n");
    credo_imprimere_compendium();
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
