/* volumen.c - documentum = plagula sqlite una (vide volumen.h) */

#include "volumen.h"
#include "scrinium.h"
#include "filum.h"
#include "chorda_aedificator.h"

#include <stdio.h>

structura Volumen {
    Piscina*            piscina;
    Scrinium*           scrinium;
    constans character* erratum;   /* proprium; scrinium_error alias */
    b32                 in_transactione;   /* vocator possidet */
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
_volumen_struere (Piscina* piscina, constans character* via);

interior Volumen*
_volumen_struere (Piscina* piscina, constans character* via)
{
    Volumen* vol;

    vol = (Volumen*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Volumen));
    si (vol == NIHIL)
    {
        redde NIHIL;
    }
    vol->piscina = piscina;
    vol->erratum = NIHIL;
    vol->in_transactione = FALSUM;
    vol->scrinium = scrinium_aperire(piscina, via);
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

Volumen*
volumen_creare (Piscina* piscina, constans character* via)
{
    Volumen* vol;
    chorda   datum;

    si (filum_existit(via))
    {
        fprintf(stderr, "volumen_creare: plagula iam exsistit: %s\n",
            via);
        redde NIHIL;
    }
    vol = _volumen_struere(piscina, via);
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
volumen_aperire (Piscina* piscina, constans character* via)
{
    si (!filum_existit(via))
    {
        fprintf(stderr, "volumen_aperire: plagula deest: %s\n", via);
        redde NIHIL;
    }
    redde _volumen_struere(piscina, via);
}

vacuum
volumen_claudere (Volumen* volumen)
{
    si (volumen == NIHIL)
    {
        redde;
    }
    scrinium_claudere(volumen->scrinium);
}

constans character*
volumen_error (constans Volumen* volumen)
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
volumen_actum_appendere (Volumen* volumen, constans character* genus,
    chorda datum)
{
    ScriniumEnuntiatum* e;
    chorda              genus_ch;
    integer             gradus;

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
_actum_plagulae_fingere (Piscina* piscina, chorda via,
    constans character* sigillum_hex, constans character* origo);

interior chorda
_actum_plagulae_fingere (Piscina* piscina, chorda via,
    constans character* sigillum_hex, constans character* origo)
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
volumen_plagulam_condere (Volumen* volumen, chorda via_relativa,
    chorda contentum, constans character* origo)
{
    Sigillum            sig;
    character           hex[SIGILLUM_HEX_MENSURA];
    chorda              hex_ch;
    chorda              origo_ch;
    chorda              datum;
    ScriniumEnuntiatum* e;
    integer             gradus;

    sig = sigillum_computare((constans vacuum*)contentum.datum,
        (memoriae_index)contentum.mensura);
    sigillum_hex(&sig, hex);
    hex_ch = chorda_ex_literis(hex, volumen->piscina);
    origo_ch = chorda_ex_literis(origo, volumen->piscina);

    si (!volumen->in_transactione
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
volumen_plagulam_removere (Volumen* volumen, chorda via_relativa)
{
    ScriniumEnuntiatum* e;
    chorda              datum;
    integer             gradus;

    si (!volumen->in_transactione
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
volumen_transactionem_incipere (Volumen* volumen)
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
volumen_transactionem_committere (Volumen* volumen)
{
    si (!volumen->in_transactione)
    {
        redde FALSUM;
    }
    volumen->in_transactione = FALSUM;
    redde scrinium_committere(volumen->scrinium);
}

b32
volumen_transactionem_revolvere (Volumen* volumen)
{
    si (!volumen->in_transactione)
    {
        redde FALSUM;
    }
    volumen->in_transactione = FALSUM;
    redde scrinium_revolvere(volumen->scrinium);
}

chorda
volumen_plagulam_promere (Volumen* volumen, chorda via_relativa,
    Piscina* piscina, b32* inventum)
{
    ScriniumEnuntiatum* e;
    chorda              vacua;
    chorda              fructus;
    integer             gradus;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
    *inventum = FALSUM;

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
volumen_plagulas_enumerare (Volumen* volumen, Piscina* piscina)
{
    ScriniumEnuntiatum* e;
    Xar*                ordo;

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
        p->via = scrinium_columna_textus(e, 0, piscina);
        p->sigillum_hex = scrinium_columna_textus(e, 1, piscina);
        p->origo = scrinium_columna_textus(e, 2, piscina);
    }
    scrinium_finire(e);
    redde ordo;
}

b32
volumen_massam_condere (Volumen* volumen, chorda contentum,
    character* sigillum_hex_exitus)
{
    Sigillum            sig;
    chorda              hex_ch;
    ScriniumEnuntiatum* e;
    integer             gradus;

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
volumen_massam_promere (Volumen* volumen, chorda sigillum_hex,
    Piscina* piscina, b32* inventum)
{
    ScriniumEnuntiatum* e;
    chorda              vacua;
    chorda              fructus;

    vacua.datum = NIHIL;
    vacua.mensura = ZEPHYRUM;
    *inventum = FALSUM;

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
volumen_acta_legere (Volumen* volumen, s64 post_seq,
    Piscina* piscina)
{
    ScriniumEnuntiatum* e;
    Xar*                ordo;

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
        a->seq = scrinium_columna_numerus(e, 0);
        a->momentum = scrinium_columna_textus(e, 1, piscina);
        a->genus = scrinium_columna_textus(e, 2, piscina);
        a->datum = scrinium_columna_textus(e, 3, piscina);
    }
    scrinium_finire(e);
    redde ordo;
}

interior s64
_summa (Volumen* volumen, constans character* sql);

interior s64
_summa (Volumen* volumen, constans character* sql)
{
    ScriniumEnuntiatum* e;
    s64                 summa = 0;

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
volumen_summa_actorum (Volumen* volumen)
{
    redde _summa(volumen, "SELECT COUNT(*) FROM acta");
}

s64
volumen_summa_plagularum (Volumen* volumen)
{
    redde _summa(volumen, "SELECT COUNT(*) FROM plagulae");
}

s64
volumen_summa_massarum (Volumen* volumen)
{
    redde _summa(volumen, "SELECT COUNT(*) FROM massae");
}
