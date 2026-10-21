/* volumen.c - documentum = plagula sqlite una (vide volumen.h) */

#include "volumen.h"
#include "scrinium.h"
#include "filum.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "json.h"

#include <stdio.h>
#include <string.h>

structura Volumen {
               Piscina* piscina;
              Scrinium* scrinium;
    constans character* erratum;   /* proprium; scrinium_error alias */
                   b32  in_transactione;   /* vocator possidet */

    /* Volumen scratch: via servatur ut claudere eam delere possit. */
                   b32  temporarium;
    constans character* via;
};

/* migrationes solum-appende (lex scrinii: numquam mutandae, solum
 * novae appendantur) */
interior constans character* constans MIGRATIONES_VOLUMINIS[] = {
    "CREATE TABLE acta ("
    " seq INTEGER PRIMARY KEY AUTOINCREMENT,"
    " momentum TEXT NOT NULL"
    "   DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now')),"
    " genus TEXT NOT NULL,"
    " datum TEXT NOT NULL)",

    "CREATE TABLE massae ("
    " sigillum TEXT PRIMARY KEY,"
    " contentum BLOB NOT NULL)",

    "CREATE TABLE plagulae ("
    " via TEXT PRIMARY KEY,"
    " sigillum TEXT NOT NULL REFERENCES massae(sigillum),"
    " origo TEXT NOT NULL,"
    " momentum TEXT NOT NULL"
    "   DEFAULT (strftime('%Y-%m-%dT%H:%M:%fZ','now')))"
};

#define MIGRATIONES_VOLUMINIS_NUMERUS 3

interior Volumen*
_volumen_struere (
                     Piscina* piscina,
          constans character* via,
    constans VolumenOptiones* optiones);

interior Volumen*
_volumen_struere (
                     Piscina* piscina,
          constans character* via,
    constans VolumenOptiones* optiones)
{
    ScriniumOptiones so;

    Volumen* vol;

    vol = (Volumen*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Volumen));
    si (vol == NIHIL)
    {
        redde NIHIL;
    }
    vol->piscina          = piscina;
    vol->erratum          = NIHIL;
    vol->in_transactione  = FALSUM;
    /* EXPLICITE, quia struere structuram campo-post-campum implet
     * SINE memset: campus additus purgamentum ferret, et
     * 'temporarium' purgamentum non-nullum volumen VERUM usoris in
     * claudendo deleret. Vitium quod semel scriptum tacet et semel
     * currit clamat. */
    vol->temporarium  = FALSUM;
    vol->via          = NIHIL;
    {
        /* Via COPIATUR: vocator litteras suas liberare potest, et
         * volumen temporarium eas in claudendo adhuc eget. */
        ChordaAedificator* aed = chorda_aedificator_creare(
            piscina, (memoriae_index)64);
        si (aed != NIHIL)
        {
            chorda_aedificator_appendere_literis(aed, via);
            vol->via = chorda_ut_cstr(
                chorda_aedificator_finire(aed), piscina);
        }
    }
    memset(&so, 0, magnitudo(so));
    so.synchronus_plenus  = optiones && optiones->durabilitas_plena;
    vol->scrinium         = scrinium_aperire_cum(piscina, via, &so);
    si (vol->scrinium == NIHIL)
    {
        redde NIHIL;
    }
    si (!scrinium_migrare(vol->scrinium, MIGRATIONES_VOLUMINIS,
        MIGRATIONES_VOLUMINIS_NUMERUS))
    {
        scrinium_claudere(vol->scrinium);
        redde NIHIL;
    }
    redde vol;
}

interior Volumen*
_volumen_creare (
                     Piscina* piscina,
          constans character* via,
    constans VolumenOptiones* optiones)
{
    Volumen* vol;
     chorda  datum;

    si (filum_existit(via))
    {
        fprintf(stderr, "volumen_creare: plagula iam exsistit: %s\n",
            via);
        redde NIHIL;
    }
    vol = _volumen_struere(piscina, via, optiones);
    si (vol == NIHIL)
    {
        redde NIHIL;
    }
    datum = chorda_ex_literis("{\"versio\":1}", piscina);
    si (volumen_actum_appendere(vol, "volumen-creatum", datum) == 0)
    {
        scrinium_claudere(vol->scrinium);
        redde NIHIL;
    }
    redde vol;
}

Volumen*
volumen_creare (
               Piscina* piscina,
    constans character* via)
{
    redde _volumen_creare(piscina, via, NIHIL);
}

Volumen*
volumen_aperire (
               Piscina* piscina,
    constans character* via)
{
    si (!filum_existit(via))
    {
        fprintf(stderr, "volumen_aperire: plagula deest: %s\n", via);
        redde NIHIL;
    }
    redde _volumen_struere(piscina, via, NIHIL);
}

Volumen*
volumen_aperire_aut_creare (
               Piscina* piscina,
    constans character* via)
{
    redde volumen_aperire_aut_creare_cum(piscina, via, NIHIL);
}

Volumen*
volumen_aperire_aut_creare_cum (
                     Piscina* piscina,
          constans character* via,
    constans VolumenOptiones* optiones)
{
    si (filum_existit(via))
    {
        redde _volumen_struere(piscina, via, optiones);
    }
    redde _volumen_creare(piscina, via, optiones);
}

Scrinium*
volumen_scrinium (
    Volumen* volumen)
{
    redde volumen ? volumen->scrinium : NIHIL;
}

vacuum
volumen_claudere (
    Volumen* volumen)
{
    si (volumen == NIHIL)
    {
        redde;
    }
    scrinium_claudere(volumen->scrinium);
    si (volumen->temporarium && volumen->via != NIHIL)
    {
        /* Post claudere, non ante: scrinium plagulam adhuc tenet. */
        (vacuum)filum_delere(volumen->via);
        volumen->temporarium = FALSUM;
    }
}

Volumen*
volumen_temporarium (
               Piscina* piscina,
    constans character* praefixum)
{
     ChordaAedificator* aed;
    constans character* via = NIHIL;
               Volumen* vol;
                   i32  i;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    si (praefixum == NIHIL || praefixum[0] == '\0')
    {
        praefixum = "volumen";
    }

    /* Nomen liberum quaerere. Numerus, non tempus nec fors: volumen
     * pendens ita nomen suum RETINET, et qui in /tmp inspicit videt
     * quot cursus reliquerint. */
    per (i = I; i <= C; i = i + I)
    {
        character numerus[XXXII];

        aed = chorda_aedificator_creare(piscina, (memoriae_index)128);
        si (aed == NIHIL)
        {
            redde NIHIL;
        }
        sprintf(numerus, "%lu", (insignatus longus)i);
        chorda_aedificator_appendere_literis(aed, "/tmp/");
        chorda_aedificator_appendere_literis(aed, praefixum);
        chorda_aedificator_appendere_literis(aed, "-");
        chorda_aedificator_appendere_literis(aed, numerus);
        chorda_aedificator_appendere_literis(aed, ".volumen");
        via = chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
        si (via == NIHIL)
        {
            redde NIHIL;
        }
        si (!filum_existit(via))
        {
            frange;
        }
        via = NIHIL;
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "volumen_temporarium: C nomina occupata"
            " (praefixum '%s') - reliquiae in /tmp?\n", praefixum);
        redde NIHIL;
    }

    vol = volumen_creare(piscina, via);
    si (vol == NIHIL)
    {
        redde NIHIL;
    }
    vol->temporarium = VERUM;   /* via ex struere iam servata */
    redde vol;
}

constans character*
volumen_via (
    constans Volumen* volumen)
{
    si (volumen == NIHIL)
    {
        redde NIHIL;
    }
    redde volumen->via;
}

constans character*
volumen_error (
    constans Volumen* volumen)
{
    si (volumen == NIHIL)
    {
        redde "volumen NIHIL";
    }
    si (volumen->erratum != NIHIL)
    {
        redde volumen->erratum;
    }
    redde scrinium_error(volumen->scrinium);
}

s64
volumen_actum_appendere (
               Volumen* volumen,
    constans character* genus,
                chorda  datum)
{
    ScriniumEnuntiatum* e;
                chorda  genus_ch;
               integer  gradus;

    genus_ch = chorda_ex_literis(genus, volumen->piscina);
    e = scrinium_praeparare(volumen->scrinium,
        "INSERT INTO acta (genus, datum) VALUES (?, ?)");
    si (e == NIHIL)
    {
        redde 0;
    }
    scrinium_ligare_textum(e, 1, genus_ch);
    scrinium_ligare_textum(e, 2, datum);
    gradus = scrinium_gradi(e);
    scrinium_finire(e);
    si (gradus != SCRINIUM_FACTUM)
    {
        redde 0;
    }
    redde scrinium_ultimum_id(volumen->scrinium);
}

/* datum JSON actus 'plagula-condita' */
interior chorda
_actum_plagulae_fingere (
               Piscina* piscina,
                chorda  via,
    constans character* sigillum_hex,
    constans character* origo);

interior chorda
_actum_plagulae_fingere (
               Piscina* piscina,
                chorda  via,
    constans character* sigillum_hex,
    constans character* origo)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)256);

    chorda_aedificator_appendere_literis(a, "{\"via\":\"");
    chorda_aedificator_appendere_evasus_json(a, via);
    chorda_aedificator_appendere_literis(a, "\",\"sigillum\":\"");
    chorda_aedificator_appendere_literis(a, sigillum_hex);
    chorda_aedificator_appendere_literis(a, "\",\"origo\":\"");
    chorda_aedificator_appendere_literis_evasus_json(a, origo);
    chorda_aedificator_appendere_literis(a, "\"}");
    redde chorda_aedificator_finire(a);
}

b32
volumen_plagulam_condere (
               Volumen* volumen,
                chorda  via_relativa,
                chorda  contentum,
    constans character* origo)
{
              Sigillum  sig;
             character  hex[SIGILLUM_HEX_MENSURA];
                chorda  hex_ch;
                chorda  origo_ch;
                chorda  datum;
    ScriniumEnuntiatum* e;
               integer  gradus;

    sig = sigillum_computare((constans vacuum*)contentum.datum,
        (memoriae_index)contentum.mensura);
    sigillum_hex(&sig, hex);
    hex_ch    = chorda_ex_literis(hex, volumen->piscina);
    origo_ch  = chorda_ex_literis(origo, volumen->piscina);

    si (   !volumen->in_transactione
        && !scrinium_incipere(volumen->scrinium))
    {
        redde FALSUM;
    }

    /* massa: contentum idem semel (sigillum = clavis) */
    e = scrinium_praeparare(volumen->scrinium,
        "INSERT OR IGNORE INTO massae (sigillum, contentum)"
        " VALUES (?, ?)");
    si (e == NIHIL)
    {
        salta revolve;
    }
    scrinium_ligare_textum(e, 1, hex_ch);
    scrinium_ligare_massam(e, 2, contentum);
    gradus = scrinium_gradi(e);
    scrinium_finire(e);
    si (gradus != SCRINIUM_FACTUM)
    {
        salta revolve;
    }

    /* actum: veritas ante proiectionem */
    datum = _actum_plagulae_fingere(volumen->piscina, via_relativa,
        hex, origo);
    si (volumen_actum_appendere(volumen, "plagula-condita", datum)
        == 0)
    {
        salta revolve;
    }

    /* manifestum: upsert per viam */
    e = scrinium_praeparare(volumen->scrinium,
        "INSERT INTO plagulae (via, sigillum, origo) VALUES (?, ?, ?)"
        " ON CONFLICT(via) DO UPDATE SET"
        " sigillum = excluded.sigillum,"
        " origo = excluded.origo,"
        " momentum = strftime('%Y-%m-%dT%H:%M:%fZ','now')");
    si (e == NIHIL)
    {
        salta revolve;
    }
    scrinium_ligare_textum(e, 1, via_relativa);
    scrinium_ligare_textum(e, 2, hex_ch);
    scrinium_ligare_textum(e, 3, origo_ch);
    gradus = scrinium_gradi(e);
    scrinium_finire(e);
    si (gradus != SCRINIUM_FACTUM)
    {
        salta revolve;
    }

    si (volumen->in_transactione)
    {
        redde VERUM;   /* vocator committet */
    }
    redde scrinium_committere(volumen->scrinium);

revolve:
    si (!volumen->in_transactione)
    {
        scrinium_revolvere(volumen->scrinium);
    }
    redde FALSUM;
}

b32
volumen_plagulam_removere (
    Volumen* volumen,
     chorda  via_relativa)
{
    ScriniumEnuntiatum* e;
                chorda  datum;
               integer  gradus;

    si (   !volumen->in_transactione
        && !scrinium_incipere(volumen->scrinium))
    {
        redde FALSUM;
    }

    /* actum: veritas ante manifestum */
    {
        ChordaAedificator* a = chorda_aedificator_creare(
            volumen->piscina, (memoriae_index)128);

        chorda_aedificator_appendere_literis(a, "{\"via\":\"");
        chorda_aedificator_appendere_evasus_json(a, via_relativa);
        chorda_aedificator_appendere_literis(a, "\"}");
        datum = chorda_aedificator_finire(a);
    }
    si (volumen_actum_appendere(volumen, "plagula-remota", datum)
        == 0)
    {
        salta remove_revolve;
    }
    e = scrinium_praeparare(volumen->scrinium,
        "DELETE FROM plagulae WHERE via = ?");
    si (e == NIHIL)
    {
        salta remove_revolve;
    }
    scrinium_ligare_textum(e, 1, via_relativa);
    gradus = scrinium_gradi(e);
    scrinium_finire(e);
    si (gradus != SCRINIUM_FACTUM)
    {
        salta remove_revolve;
    }
    si (volumen->in_transactione)
    {
        redde VERUM;
    }
    redde scrinium_committere(volumen->scrinium);

remove_revolve:
    si (!volumen->in_transactione)
    {
        scrinium_revolvere(volumen->scrinium);
    }
    redde FALSUM;
}

b32
volumen_transactionem_incipere (
    Volumen* volumen)
{
    si (volumen->in_transactione)
    {
        redde FALSUM;   /* nidificatio vetita - clare */
    }
    si (!scrinium_incipere(volumen->scrinium))
    {
        redde FALSUM;
    }
    volumen->in_transactione = VERUM;
    redde VERUM;
}

b32
volumen_transactionem_committere (
    Volumen* volumen)
{
    si (!volumen->in_transactione)
    {
        redde FALSUM;
    }
    volumen->in_transactione = FALSUM;
    redde scrinium_committere(volumen->scrinium);
}

b32
volumen_transactionem_revolvere (
    Volumen* volumen)
{
    si (!volumen->in_transactione)
    {
        redde FALSUM;
    }
    volumen->in_transactione = FALSUM;
    redde scrinium_revolvere(volumen->scrinium);
}

b32
volumen_in_transactione (
    Volumen* volumen)
{
    redde volumen && volumen->in_transactione;
}

chorda
volumen_plagulam_promere (
    Volumen* volumen,
     chorda  via_relativa,
    Piscina* piscina,
        b32* inventum)
{
    ScriniumEnuntiatum* e;
                chorda  vacua;
                chorda  fructus;
               integer  gradus;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    *inventum      = FALSUM;

    e = scrinium_praeparare(volumen->scrinium,
        "SELECT m.contentum FROM plagulae p"
        " JOIN massae m ON m.sigillum = p.sigillum"
        " WHERE p.via = ?");
    si (e == NIHIL)
    {
        redde vacua;
    }
    scrinium_ligare_textum(e, 1, via_relativa);
    gradus = scrinium_gradi(e);
    si (gradus != SCRINIUM_ORDO)
    {
        scrinium_finire(e);
        redde vacua;
    }
    fructus = scrinium_columna_massa(e, 0, piscina);
    scrinium_finire(e);
    *inventum = VERUM;
    redde fructus;
}

Xar*
volumen_plagulas_enumerare (
    Volumen* volumen,
    Piscina* piscina)
{
    ScriniumEnuntiatum* e;
                   Xar* ordo;

    ordo = xar_creare(piscina, (i32)magnitudo(VolumenPlagula));
    si (ordo == NIHIL)
    {
        redde NIHIL;
    }
    e = scrinium_praeparare(volumen->scrinium,
        "SELECT via, sigillum, origo FROM plagulae ORDER BY via");
    si (e == NIHIL)
    {
        redde NIHIL;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        VolumenPlagula* p = (VolumenPlagula*)xar_addere(ordo);

        si (p == NIHIL)
        {
            scrinium_finire(e);
            redde NIHIL;
        }
        p->via           = scrinium_columna_textus(e, 0, piscina);
        p->sigillum_hex  = scrinium_columna_textus(e, 1, piscina);
        p->origo         = scrinium_columna_textus(e, 2, piscina);
    }
    scrinium_finire(e);
    redde ordo;
}

b32
volumen_massam_condere (
      Volumen* volumen,
       chorda  contentum,
    character* sigillum_hex_exitus)
{
              Sigillum  sig;
                chorda  hex_ch;
    ScriniumEnuntiatum* e;
               integer  gradus;

    sig = sigillum_computare((constans vacuum*)contentum.datum,
        (memoriae_index)contentum.mensura);
    sigillum_hex(&sig, sigillum_hex_exitus);
    hex_ch = chorda_ex_literis(sigillum_hex_exitus,
        volumen->piscina);

    e = scrinium_praeparare(volumen->scrinium,
        "INSERT OR IGNORE INTO massae (sigillum, contentum)"
        " VALUES (?, ?)");
    si (e == NIHIL)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, 1, hex_ch);
    scrinium_ligare_massam(e, 2, contentum);
    gradus = scrinium_gradi(e);
    scrinium_finire(e);
    redde gradus == SCRINIUM_FACTUM ? VERUM : FALSUM;
}

chorda
volumen_massam_promere (
    Volumen* volumen,
     chorda  sigillum_hex,
    Piscina* piscina,
        b32* inventum)
{
    ScriniumEnuntiatum* e;
                chorda  vacua;
                chorda  fructus;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    *inventum      = FALSUM;

    e = scrinium_praeparare(volumen->scrinium,
        "SELECT contentum FROM massae WHERE sigillum = ?");
    si (e == NIHIL)
    {
        redde vacua;
    }
    scrinium_ligare_textum(e, 1, sigillum_hex);
    si (scrinium_gradi(e) != SCRINIUM_ORDO)
    {
        scrinium_finire(e);
        redde vacua;
    }
    fructus = scrinium_columna_massa(e, 0, piscina);
    scrinium_finire(e);
    *inventum = VERUM;
    redde fructus;
}

Xar*
volumen_acta_legere (
    Volumen* volumen,
        s64  post_seq,
    Piscina* piscina)
{
    ScriniumEnuntiatum* e;
                   Xar* ordo;

    ordo = xar_creare(piscina, (i32)magnitudo(VolumenActum));
    si (ordo == NIHIL)
    {
        redde NIHIL;
    }
    e = scrinium_praeparare(volumen->scrinium,
        "SELECT seq, momentum, genus, datum FROM acta"
        " WHERE seq > ? ORDER BY seq");
    si (e == NIHIL)
    {
        redde NIHIL;
    }
    scrinium_ligare_numerum(e, 1, post_seq);
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        VolumenActum* a = (VolumenActum*)xar_addere(ordo);

        si (a == NIHIL)
        {
            scrinium_finire(e);
            redde NIHIL;
        }
        a->seq       = scrinium_columna_numerus(e, 0);
        a->momentum  = scrinium_columna_textus(e, 1, piscina);
        a->genus     = scrinium_columna_textus(e, 2, piscina);
        a->datum     = scrinium_columna_textus(e, 3, piscina);
    }
    scrinium_finire(e);
    redde ordo;
}

/* comparator viae pro ordine plicae (aequalis ORDER BY via) */
interior s32
_plagulas_per_viam (
    constans vacuum* a,
    constans vacuum* b);

interior s32
_plagulas_per_viam (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(((constans VolumenPlagula*)a)->via,
        ((constans VolumenPlagula*)b)->via);
}

Xar*
volumen_plicam_ad (
    Volumen* volumen,
        s64  ad_seq,
    Piscina* piscina)
{
    ScriniumEnuntiatum* e;
        TabulaDispersa* plica;
                   Xar* ordo;

    plica  = tabula_dispersa_creare_chorda(piscina, 128);
    ordo   = xar_creare(piscina, (i32)magnitudo(VolumenPlagula));
    si (plica == NIHIL || ordo == NIHIL)
    {
        redde NIHIL;
    }
    e = scrinium_praeparare(volumen->scrinium,
        "SELECT genus, datum FROM acta"
        " WHERE genus IN ('plagula-condita','plagula-remota')"
        " AND (? <= 0 OR seq <= ?) ORDER BY seq");
    si (e == NIHIL)
    {
        redde NIHIL;
    }
    scrinium_ligare_numerum(e, 1, ad_seq);
    scrinium_ligare_numerum(e, 2, ad_seq);
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
              chorda  genus   = scrinium_columna_textus(e, 0, piscina);
              chorda  datum   = scrinium_columna_textus(e, 1, piscina);
        JsonResultus  lectum  = json_legere(datum, piscina);
           JsonValor* via_v;
              chorda  via;

        si (!lectum.successus)
        {
            perge;   /* actum corruptum toleratur (lex plicae) */
        }
        via_v = json_objectum_capere(lectum.radix, "via");
        si (via_v == NIHIL || !json_est_chorda(via_v))
        {
            perge;
        }
        via = json_ad_chorda(via_v);
        si (chorda_aequalis_literis(genus, "plagula-remota"))
        {
            tabula_dispersa_delere(plica, via);
        }
        alioquin
        {
            JsonValor* sig_v = json_objectum_capere(lectum.radix,
                "sigillum");
            JsonValor* orig_v = json_objectum_capere(lectum.radix,
                "origo");
            VolumenPlagula* p;

            si (sig_v == NIHIL || !json_est_chorda(sig_v))
            {
                perge;
            }
            p = (VolumenPlagula*)piscina_allocare(piscina,
                (memoriae_index)magnitudo(VolumenPlagula));
            si (p == NIHIL)
            {
                scrinium_finire(e);
                redde NIHIL;
            }
            p->via           = via;
            p->sigillum_hex  = json_ad_chorda(sig_v);
            p->origo = (orig_v != NIHIL && json_est_chorda(orig_v))
                ? json_ad_chorda(orig_v)
                : chorda_ex_literis("", piscina);
            tabula_dispersa_inserere(plica, via, (vacuum*)p);
        }
    }
    scrinium_finire(e);

    /* plicam in ordinem viae effundere */
    {
        TabulaIterator iter = tabula_dispersa_iterator_initium(
            plica);
        chorda  clavis;
        vacuum* valor;

        dum (tabula_dispersa_iterator_proximum(&iter, &clavis,
            &valor))
        {
            VolumenPlagula* fons  = (VolumenPlagula*)valor;
            VolumenPlagula* p     = (VolumenPlagula*)xar_addere(ordo);

            si (p == NIHIL)
            {
                redde NIHIL;
            }
            *p = *fons;
        }
    }
    xar_ordinare(ordo, _plagulas_per_viam);
    redde ordo;
}

interior s64
_summa (
               Volumen* volumen,
    constans character* sql);

interior s64
_summa (
               Volumen* volumen,
    constans character* sql)
{
    ScriniumEnuntiatum* e;
                   s64  summa = 0;

    e = scrinium_praeparare(volumen->scrinium, sql);
    si (e == NIHIL)
    {
        redde 0;
    }
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        summa = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde summa;
}

s64
volumen_summa_actorum (
    Volumen* volumen)
{
    redde _summa(volumen, "SELECT COUNT(*) FROM acta");
}

s64
volumen_summa_plagularum (
    Volumen* volumen)
{
    redde _summa(volumen, "SELECT COUNT(*) FROM plagulae");
}

s64
volumen_summa_massarum (
    Volumen* volumen)
{
    redde _summa(volumen, "SELECT COUNT(*) FROM massae");
}


/* ========================================================================
 * MASSAE PER RIVUM (hospitium v3 T6)
 * ======================================================================== */

#define VOLUMEN_CAPSA_RIVI (LXIV * MXXIV)

structura VolumenMassaLector {
    Volumen* volumen;
        s64  ordo;
        i64  mensura;
        i64  offset;
};

/* LXIV signa hex minusculae */
interior b32
_sigillum_hex_validum (
    constans character* hex)
{
    i32 i;

    si (hex == NIHIL)
    {
        redde FALSUM;
    }
    per (i = 0; i < LXIV; i++)
    {
        si (!((hex[i] >= '0' && hex[i] <= '9')
              || (hex[i] >= 'a' && hex[i] <= 'f')))
        {
            redde FALSUM;
        }
    }
    redde hex[LXIV] == '\0';
}

/* VERUM si massa sigilli iam exstat; *ordo rowid (si non NIHIL),
 * *mensura longitudo (si non NIHIL) */
interior b32
_massa_quaerere (
    Volumen* volumen,
     chorda  hex,
        s64* ordo,
        i64* mensura)
{
    ScriniumEnuntiatum* e;
                   b32  inventa = FALSUM;

    e = scrinium_praeparare(volumen->scrinium,
        "SELECT rowid, length(contentum) FROM massae WHERE sigillum = ?");
    si (e == NIHIL)
    {
        redde FALSUM;
    }
    scrinium_ligare_textum(e, 1, hex);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        inventa = VERUM;
        si (ordo)
        {
            *ordo = scrinium_columna_numerus(e, 0);
        }
        si (mensura)
        {
            *mensura = (i64)scrinium_columna_numerus(e, 1);
        }
    }
    scrinium_finire(e);
    redde inventa;
}

b32
volumen_massam_condere_ex_filo (
               Volumen* volumen,
    constans character* via_fili,
                   i64  mensura,
    constans character* sigillum_notum)
{
                 Piscina* p;
                  chorda  hex;
      ScriniumEnuntiatum* e;
           ScriniumMassa* massa   = NIHIL;
    FilumOctetorumLector* lector  = NIHIL;
       SigillumContextus  ctx;
                      i8* capsa;
                     i64  scripta = 0;
                 integer  gradus;

    si (volumen == NIHIL || via_fili == NIHIL)
    {
        redde FALSUM;
    }
    volumen->erratum = NIHIL;
    si (!_sigillum_hex_validum(sigillum_notum))
    {
        volumen->erratum = "sigillum_hex malum (LXIV hex minusculae)";
        redde FALSUM;
    }
    si (mensura > 0x7FFFFFFFULL)
    {
        volumen->erratum = "massa nimis magna (ultra 2 GB)";
        redde FALSUM;
    }
    /* piscina propria: vocationes multae piscinam voluminis non
     * implent */
    p = piscina_generare_dynamicum("volumen_rivus",
        VOLUMEN_CAPSA_RIVI + MXXIV);
    si (p == NIHIL)
    {
        redde FALSUM;
    }
    hex = chorda_ex_literis(sigillum_notum, p);

    /* dedup: contentum idem iam conditum - filum non legitur */
    si (_massa_quaerere(volumen, hex, NIHIL, NIHIL))
    {
        piscina_destruere(p);
        redde VERUM;
    }

    /* savepoint: intra transactionem vocatoris quoque revolvi potest
     * sine eius opere perdendo */
    si (!scrinium_exsequi(volumen->scrinium, "SAVEPOINT massa_ex_filo"))
    {
        piscina_destruere(p);
        redde FALSUM;
    }
    e = scrinium_praeparare(volumen->scrinium,
        "INSERT INTO massae (sigillum, contentum) VALUES (?, ?)");
    si (e == NIHIL)
    {
        salta revolve;
    }
    scrinium_ligare_textum(e, 1, hex);
    scrinium_ligare_massam_nullam(e, 2, (i32)mensura);
    gradus = scrinium_gradi(e);
    scrinium_finire(e);
    si (gradus != SCRINIUM_FACTUM)
    {
        salta revolve;
    }

    lector = filum_octeti_aperire(via_fili, p);
    si (lector == NIHIL)
    {
        volumen->erratum = "filum massae legi nequit";
        salta revolve;
    }
    si (mensura > 0)
    {
        massa = scrinium_massam_aperire(volumen->scrinium, "massae",
            "contentum", scrinium_ultimum_id(volumen->scrinium), VERUM);
        si (massa == NIHIL)
        {
            salta revolve;
        }
    }
    capsa = (i8*)piscina_allocare(p, VOLUMEN_CAPSA_RIVI);
    sigillum_incipere(&ctx);
    per (;;)
    {
        s32 k = filum_octeti_legere(lector, capsa, VOLUMEN_CAPSA_RIVI);

        si (k < 0)
        {
            volumen->erratum = "filum massae legi nequit";
            salta revolve;
        }
        si (k == 0)
        {
            frange;
        }
        si (scripta + (i64)k > mensura)
        {
            volumen->erratum = "mensura mendax: filum longius";
            salta revolve;
        }
        si (!scrinium_massam_scribere(massa, capsa, (i32)k,
            (i32)scripta))
        {
            salta revolve;
        }
        sigillum_addere(&ctx, capsa, (memoriae_index)k);
        scripta += (i64)k;
    }
    si (scripta != mensura)
    {
        volumen->erratum = "mensura mendax: filum brevius";
        salta revolve;
    }
    {
         Sigillum s = sigillum_finire(&ctx);
        character computatum[SIGILLUM_HEX_MENSURA];

        sigillum_hex(&s, computatum);
        si (strcmp(computatum, sigillum_notum) != 0)
        {
            volumen->erratum =
                "sigillum non congruit cum contentu fili";
            salta revolve;
        }
    }
    scrinium_massam_claudere(massa);
    filum_octeti_claudere(lector);
    piscina_destruere(p);
    redde scrinium_exsequi(volumen->scrinium,
        "RELEASE massa_ex_filo");

revolve:
    scrinium_massam_claudere(massa);
    filum_octeti_claudere(lector);
    piscina_destruere(p);
    (vacuum)scrinium_exsequi(volumen->scrinium,
        "ROLLBACK TO massa_ex_filo");
    (vacuum)scrinium_exsequi(volumen->scrinium,
        "RELEASE massa_ex_filo");
    redde FALSUM;
}

VolumenMassaLector*
volumen_massam_aperire (
    Volumen* volumen,
     chorda  sigillum_hex,
        i64* mensura,
    Piscina* piscina)
{
    VolumenMassaLector* l;
                   s64  ordo  = 0;
                   i64  m     = 0;

    si (   volumen == NIHIL || piscina == NIHIL
        || !_massa_quaerere(volumen, sigillum_hex, &ordo, &m))
    {
        redde NIHIL;
    }
    l = (VolumenMassaLector*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(VolumenMassaLector));
    si (l == NIHIL)
    {
        redde NIHIL;
    }
    l->volumen  = volumen;
    l->ordo     = ordo;
    l->mensura  = m;
    l->offset   = 0;
    si (mensura)
    {
        *mensura = m;
    }
    redde l;
}

s32
volumen_massam_legere (
    VolumenMassaLector* lector,
                    i8* capsa,
                   i32  n)
{
    ScriniumMassa* massa;
              i64  reliqua;
              i32  m;
              b32  bene;

    si (lector == NIHIL || capsa == NIHIL)
    {
        redde -I;
    }
    reliqua = lector->mensura - lector->offset;
    si (reliqua == 0 || n == 0)
    {
        redde 0;
    }
    m = (reliqua < (i64)n) ? (i32)reliqua : n;
    /* ansa per partem: nulla lectio pendet inter partes, ergo nullum
     * snapshot WAL tenetur (checkpoint interim perficitur) */
    massa = scrinium_massam_aperire(lector->volumen->scrinium, "massae",
        "contentum", lector->ordo, FALSUM);
    si (massa == NIHIL)
    {
        redde -I;
    }
    bene = scrinium_massam_legere(massa, capsa, m, (i32)lector->offset);
    scrinium_massam_claudere(massa);
    si (!bene)
    {
        redde -I;
    }
    lector->offset += (i64)m;
    redde (s32)m;
}

vacuum
volumen_massam_claudere (
    VolumenMassaLector* lector)
{
    /* ansa non tenetur (per partem aperta) - nihil liberandum; vita in
     * piscina vocatoris */
    si (lector)
    {
        lector->offset = lector->mensura;
    }
}
