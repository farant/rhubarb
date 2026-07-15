/* gesta.c - nucleus rerum gestarum (implementatio chunk A)
 *
 * Vide gesta.h pro contractibus, gesta/phase-log.md pro decisionibus
 * chunk A, project-specs/gesta-k1-spec-v2.md pro toto consilio.
 * Oraculum reductoris: smaragda.ts:723-774 (mersio superficialis);
 * exemplar HWM: libraries.ts:263-411 (seq 1-basata - consumptor
 * recens hwm=0 nihil praetermittit). */

#include "gesta.h"
#include "json.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Migrationes (spec-v2 par III - DDL ad verbum)
 * ================================================== */

interior constans character* constans GESTA_MIGRATIONES[] = {
    "CREATE TABLE tessellae("
    "  seq        INTEGER PRIMARY KEY AUTOINCREMENT,"
    "  id         TEXT NOT NULL UNIQUE,"
    "  res_id     TEXT NOT NULL,"
    "  genus_eventus TEXT NOT NULL,"
    "  datum      TEXT NOT NULL,"
    "  actor      TEXT NOT NULL,"
    "  origo      TEXT NOT NULL DEFAULT '',"
    "  branch_id  TEXT NOT NULL DEFAULT '',"
    "  creatum    TEXT NOT NULL DEFAULT "
    "(strftime('%Y-%m-%dT%H:%M:%fZ','now'))"
    ");"
    "CREATE INDEX idx_tessellae_res ON tessellae(res_id, seq);"
    "CREATE TABLE consumptores("
    "  titulus TEXT PRIMARY KEY, seq INTEGER NOT NULL);"
    "CREATE TABLE genera("
    "  titulus TEXT PRIMARY KEY, datum TEXT NOT NULL);"
    "CREATE TABLE res("
    "  res_id  TEXT PRIMARY KEY,"
    "  genus   TEXT NOT NULL DEFAULT '',"
    "  titulus TEXT NOT NULL DEFAULT '',"
    "  status  TEXT NOT NULL DEFAULT '',"
    "  datum   TEXT NOT NULL,"
    "  creatum TEXT NOT NULL,"
    "  mutatum TEXT NOT NULL);"
    "CREATE TABLE nexus("
    "  res_a TEXT, verbum TEXT, res_b TEXT,"
    "  PRIMARY KEY(res_a, verbum, res_b));"
    "CREATE VIRTUAL TABLE res_fts USING fts5("
    "  res_id UNINDEXED, titulus, corpus, notae,"
    "  tokenize='unicode61 remove_diacritics 2');"
    "CREATE TABLE sordidae(res_id TEXT PRIMARY KEY);"
};

#define GESTA_MIGRATIONES_NUMERUS I

structura GestaMundus {
    Piscina*            piscina;
    Scrinium*           scrinium;
    constans character* via_annalium;   /* copia in piscina */
    constans character* error;          /* litterae staticae aut
                                         * scrinium_error */
};

/* ==================================================
 * Auxilia
 * ================================================== */

/* litterae -> chorda (sine copia; vita = litterarum; exemplar
 * probatio_silva_examen _typus_symboli - unio contra qualem) */
interior chorda
_ch (constans character* litterae)
{
    chorda c;
    unio { constans character* l; i8* m; } u;

    u.l = litterae;
    c.datum = u.m;
    c.mensura = (i32)strlen(litterae);
    redde c;
}

/* chorda cum dato NIHIL -> "" (json_ad_chorda vacuam sine dato
 * reddit; sqlite3_bind_text dato NIHIL = SQL NULL - decipula
 * NOT NULL, inventum restitutionis) */
interior chorda
_chorda_tuta (chorda c)
{
    si (c.datum == NIHIL)
    {
        redde _ch("");
    }
    redde c;
}

interior b32
_chorda_est (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (memoriae_index)c.mensura == m
        && (m == ZEPHYRUM
            || memcmp(c.datum, litterae, m) == ZEPHYRUM);
}

/* chorda -> litterae NUL-terminatae in piscina */
interior constans character*
_litterae (Piscina* piscina, chorda c)
{
    character* l = (character*)piscina_allocare(piscina,
        (memoriae_index)c.mensura + I);

    si (l == NIHIL)
    {
        redde "";
    }
    si (c.mensura > ZEPHYRUM)
    {
        memcpy(l, c.datum, (memoriae_index)c.mensura);
    }
    l[c.mensura] = '\0';
    redde l;
}

interior b32
_fractum (GestaMundus* m, constans character* causa)
{
    m->error = causa;
    redde FALSUM;
}

/* plagulam totam legere (NUL appenso); NIHIL si illegibilis */
interior character*
_plagulam_legere (Piscina* piscina, constans character* via,
    i32* mensura_out)
{
    FILE* pl = fopen(via, "rb");
    long mensura_l;
    character* textus;

    *mensura_out = ZEPHYRUM;
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_l > 0L ? mensura_l + 1L : I));
    si (textus == NIHIL
        || (mensura_l > 0L
            && fread(textus, I, (memoriae_index)mensura_l, pl)
                != (memoriae_index)mensura_l))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    textus[mensura_l] = '\0';
    *mensura_out = (i32)mensura_l;
    redde textus;
}

/* ==================================================
 * Lectiones parvae (folds + acta)
 * ================================================== */

interior s64
_hwm_capere (GestaMundus* m, constans character* consumptor)
{
    ScriniumEnuntiatum* e;
    s64 seq = ZEPHYRUM;

    e = scrinium_praeparare(m->scrinium,
        "SELECT seq FROM consumptores WHERE titulus = ?");
    si (e == NIHIL)
    {
        redde ZEPHYRUM;
    }
    scrinium_ligare_textum(e, I, _ch(consumptor));
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        seq = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde seq;
}

/* genus datum ex plicatura; chorda vacua = absens */
interior chorda
_genus_datum_capere (GestaMundus* m, chorda titulus,
    Piscina* piscina)
{
    ScriniumEnuntiatum* e;
    chorda fructus;

    fructus.mensura = ZEPHYRUM;
    fructus.datum = NIHIL;
    e = scrinium_praeparare(m->scrinium,
        "SELECT datum FROM genera WHERE titulus = ?");
    si (e == NIHIL)
    {
        redde fructus;
    }
    scrinium_ligare_textum(e, I, titulus);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        fructus = scrinium_columna_textus(e, 0, piscina);
    }
    scrinium_finire(e);
    redde fructus;
}

nomen structura {
    b32    exsistit;
    chorda genus;
    chorda titulus;
    chorda status;
    chorda datum;
    chorda creatum;
} GestaResOrdo;

interior GestaResOrdo
_res_capere (GestaMundus* m, chorda res_id, Piscina* piscina)
{
    ScriniumEnuntiatum* e;
    GestaResOrdo ordo;

    memset(&ordo, ZEPHYRUM, magnitudo(GestaResOrdo));
    e = scrinium_praeparare(m->scrinium,
        "SELECT genus, titulus, status, datum, creatum FROM res"
        " WHERE res_id = ?");
    si (e == NIHIL)
    {
        redde ordo;
    }
    scrinium_ligare_textum(e, I, res_id);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        ordo.exsistit = VERUM;
        ordo.genus = scrinium_columna_textus(e, 0, piscina);
        ordo.titulus = scrinium_columna_textus(e, I, piscina);
        ordo.status = scrinium_columna_textus(e, II, piscina);
        ordo.datum = scrinium_columna_textus(e, III, piscina);
        ordo.creatum = scrinium_columna_textus(e, IV, piscina);
    }
    scrinium_finire(e);
    redde ordo;
}

/* transitio (ex -> ad) in machina generis? machina absens/vacua =
 * licita (genera sine statu). datum_generis vacuum = genus ignotum
 * (vocans decidit quid faciat). */
interior b32
_transitio_licita (Piscina* piscina, chorda datum_generis,
    chorda ex, chorda ad)
{
    JsonResultus r;
    JsonValor* machina;
    i32 i;
    i32 n;

    si (datum_generis.mensura == ZEPHYRUM)
    {
        redde VERUM;   /* genus ignotum hic non iudicatur */
    }
    r = json_legere(datum_generis, piscina);
    si (!r.successus)
    {
        redde VERUM;
    }
    machina = json_objectum_capere(r.radix, "machina");
    si (machina == NIHIL || !json_est_tabulatum(machina)
        || json_tabulatum_numerus(machina) == ZEPHYRUM)
    {
        redde VERUM;   /* sine machina = sine statu = licitum */
    }
    n = json_tabulatum_numerus(machina);
    per (i = ZEPHYRUM; i < n; i++)
    {
        JsonValor* par = json_tabulatum_obtinere(machina, i);
        JsonValor* p_ex;
        JsonValor* p_ad;
        chorda c_ex;
        chorda c_ad;

        si (par == NIHIL || !json_est_tabulatum(par)
            || json_tabulatum_numerus(par) < II)
        {
            perge;
        }
        p_ex = json_tabulatum_obtinere(par, ZEPHYRUM);
        p_ad = json_tabulatum_obtinere(par, I);
        c_ex = json_ad_chorda(p_ex);
        c_ad = json_ad_chorda(p_ad);
        si (c_ex.mensura == ex.mensura
            && (ex.mensura == ZEPHYRUM
                || memcmp(c_ex.datum, ex.datum,
                       (memoriae_index)ex.mensura) == ZEPHYRUM)
            && c_ad.mensura == ad.mensura
            && (ad.mensura == ZEPHYRUM
                || memcmp(c_ad.datum, ad.datum,
                       (memoriae_index)ad.mensura) == ZEPHYRUM))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Plicaturae (folds) - reductor ordinarius
 * Oraculum: smaragda.ts:723-774. Status entis = obiectum JSON UNUM
 * (titulus/status claves inclusae); columnae = proiectiones.
 * ================================================== */

interior b32
_est_eventus_generis (chorda genus_eventus)
{
    redde _chorda_est(genus_eventus, "definitio-generis")
        || _chorda_est(genus_eventus, "emendatio-generis");
}

/* obiectum sine clave (remotio; bibliotheca delere non habet -
 * refectio; TS: smaragda.ts:731-736) */
interior JsonValor*
_sine_clave (Piscina* piscina, JsonValor* obiectum, chorda clavis)
{
    JsonValor* novum = json_objectum_creare(piscina);
    JsonObjectumIterator iter = json_objectum_iterator(obiectum);
    chorda k;
    JsonValor* v;

    dum (json_objectum_iterator_proxima(&iter, &k, &v))
    {
        si (k.mensura == clavis.mensura
            && (k.mensura == ZEPHYRUM
                || memcmp(k.datum, clavis.datum,
                       (memoriae_index)k.mensura) == ZEPHYRUM))
        {
            perge;
        }
        json_objectum_ponere_chorda(novum, k, v);
    }
    redde novum;
}

/* eventum unum in plicaturam generum applicare */
interior vacuum
_generum_applicare (GestaMundus* m, chorda genus_eventus,
    chorda datum, Piscina* piscina)
{
    JsonResultus r;
    JsonValor* titulus;
    ScriniumEnuntiatum* e;

    si (!_est_eventus_generis(genus_eventus))
    {
        redde;
    }
    r = json_legere(datum, piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde;
    }
    titulus = json_objectum_capere(r.radix, "titulus");
    si (titulus == NIHIL || !json_est_chorda(titulus))
    {
        redde;
    }
    /* emendatio = substitutio definitionis TOTIUS (phase-log
     * decisio 6; TS upsert-per-par = artefactum granularitatis -
     * smaragda.ts:1819-1822 documentatum) */
    e = scrinium_praeparare(m->scrinium,
        "INSERT OR REPLACE INTO genera (titulus, datum)"
        " VALUES (?, ?)");
    si (e == NIHIL)
    {
        redde;
    }
    scrinium_ligare_textum(e, I, json_ad_chorda(titulus));
    scrinium_ligare_textum(e, II, datum);
    (vacuum)scrinium_gradi(e);
    scrinium_finire(e);
}

/* eventum unum in plicaturam rerum applicare */
interior vacuum
_rei_applicare (GestaMundus* m, chorda res_id, chorda genus_eventus,
    chorda datum, chorda actor, chorda creatum, Piscina* piscina)
{
    GestaResOrdo ordo;
    JsonValor* status_obiectum = NIHIL;
    JsonResultus r_datum;
    JsonValor* datum_obiectum = NIHIL;
    chorda genus_columna;
    chorda creatum_columna;
    b32 mutatum_est = FALSUM;

    si (_est_eventus_generis(genus_eventus))
    {
        redde;   /* res consumptor generum eventus praeterit */
    }
    ordo = _res_capere(m, res_id, piscina);
    si (ordo.exsistit && ordo.datum.mensura > ZEPHYRUM)
    {
        JsonResultus r = json_legere(ordo.datum, piscina);

        si (r.successus && json_est_objectum(r.radix))
        {
            status_obiectum = r.radix;
        }
    }
    si (status_obiectum == NIHIL)
    {
        status_obiectum = json_objectum_creare(piscina);
    }
    genus_columna = ordo.exsistit ? ordo.genus : _ch("");
    creatum_columna = ordo.exsistit ? ordo.creatum : creatum;

    r_datum = json_legere(datum, piscina);
    si (r_datum.successus && json_est_objectum(r_datum.radix))
    {
        datum_obiectum = r_datum.radix;
    }

    si (_chorda_est(genus_eventus, "creatio"))
    {
        /* RESET ad {} (TS: smaragda.ts:726 - 'created' litteralis
         * plicaturae reset; creatio secunda cruda statum purgat -
         * aurea XIV) */
        status_obiectum = json_objectum_creare(piscina);
        genus_columna = _ch("");
        creatum_columna = creatum;
        si (datum_obiectum != NIHIL)
        {
            JsonObjectumIterator iter =
                json_objectum_iterator(datum_obiectum);
            chorda k;
            JsonValor* v;

            dum (json_objectum_iterator_proxima(&iter, &k, &v))
            {
                si (_chorda_est(k, "genus"))
                {
                    genus_columna = json_ad_chorda(v);
                    perge;   /* genus = columna sola, non status */
                }
                json_objectum_ponere_chorda(status_obiectum, k, v);
            }
            /* status initialis ex genere si non datus (TS:
             * smaragda.ts:2099-2100 - eventus proprius ibi;
             * compactio nostra hic - divergentia in tabula) */
            si (!json_objectum_habet(status_obiectum, "status")
                && genus_columna.mensura > ZEPHYRUM)
            {
                chorda gd = _genus_datum_capere(m, genus_columna,
                    piscina);

                si (gd.mensura > ZEPHYRUM)
                {
                    JsonResultus rg = json_legere(gd, piscina);

                    si (rg.successus)
                    {
                        JsonValor* si_init = json_objectum_capere(
                            rg.radix, "status_initialis");

                        si (si_init != NIHIL
                            && json_est_chorda(si_init))
                        {
                            json_objectum_ponere(status_obiectum,
                                "status", si_init);
                        }
                    }
                }
            }
        }
        mutatum_est = VERUM;
    }
    alioquin si (_chorda_est(genus_eventus, "mutatio"))
    {
        /* mersio superficialis clavium summorum (TS: smaragda.ts:
         * 727-730); acies/obiecta TOTA substituuntur (:729) */
        si (datum_obiectum != NIHIL)
        {
            JsonObjectumIterator iter =
                json_objectum_iterator(datum_obiectum);
            chorda k;
            JsonValor* v;

            dum (json_objectum_iterator_proxima(&iter, &k, &v))
            {
                json_objectum_ponere_chorda(status_obiectum, k, v);
            }
        }
        mutatum_est = VERUM;
    }
    alioquin si (_chorda_est(genus_eventus, "remotio"))
    {
        /* clavem delet (TS: smaragda.ts:731-736; remotio !=
         * mutatio-ad-nihil) */
        JsonValor* clavis = (datum_obiectum != NIHIL)
            ? json_objectum_capere(datum_obiectum, "clavis")
            : NIHIL;

        si (clavis != NIHIL && json_est_chorda(clavis))
        {
            status_obiectum = _sine_clave(piscina, status_obiectum,
                json_ad_chorda(clavis));
            mutatum_est = VERUM;
        }
    }
    alioquin si (_chorda_est(genus_eventus, "status"))
    {
        /* plicatura caeca - machina NON hic iudicatur (TS:
         * smaragda.ts:737-739; validatio ad scripturam, phase-log
         * decisio 2) */
        JsonValor* novus = (datum_obiectum != NIHIL)
            ? json_objectum_capere(datum_obiectum, "novus")
            : NIHIL;

        si (novus != NIHIL && json_est_chorda(novus))
        {
            json_objectum_ponere(status_obiectum, "status", novus);
            mutatum_est = VERUM;
        }
    }
    alioquin si (_chorda_est(genus_eventus, "nota"))
    {
        /* series notarum appenditur (DISCESSUS a TS - spec-v2
         * par XIII: TS notas ut acies-totas rescribit) */
        JsonValor* textus = (datum_obiectum != NIHIL)
            ? json_objectum_capere(datum_obiectum, "textus")
            : NIHIL;

        si (textus != NIHIL && json_est_chorda(textus))
        {
            JsonValor* notae = json_objectum_capere(status_obiectum,
                "notae");
            JsonValor* nota = json_objectum_creare(piscina);

            si (notae == NIHIL || !json_est_tabulatum(notae))
            {
                notae = json_tabulatum_creare(piscina);
                json_objectum_ponere(status_obiectum, "notae",
                    notae);
            }
            json_objectum_ponere(nota, "textus", textus);
            json_objectum_ponere(nota, "actor",
                json_chorda_creare(piscina, actor));
            json_objectum_ponere(nota, "creatum",
                json_chorda_creare(piscina, creatum));
            json_tabulatum_addere(notae, nota);
            mutatum_est = VERUM;
        }
    }
    alioquin
    {
        /* genus_eventus ignotum = nihil agit (TS: smaragda.ts:
         * 771-772; nexus/denexus hic cadunt donec chunk B) */
        redde;
    }

    si (!mutatum_est && !ordo.exsistit)
    {
        redde;   /* nihil materiale, nulla series nova */
    }

    /* proiectio columnarum + scriptura */
    {
        JsonValor* v_titulus = json_objectum_capere(status_obiectum,
            "titulus");
        JsonValor* v_status = json_objectum_capere(status_obiectum,
            "status");
        chorda c_titulus = (v_titulus != NIHIL
            && json_est_chorda(v_titulus))
            ? json_ad_chorda(v_titulus) : _ch("");
        chorda c_status = (v_status != NIHIL
            && json_est_chorda(v_status))
            ? json_ad_chorda(v_status) : _ch("");
        chorda c_datum = json_scribere(status_obiectum, piscina);
        ScriniumEnuntiatum* e = scrinium_praeparare(m->scrinium,
            "INSERT OR REPLACE INTO res"
            " (res_id, genus, titulus, status, datum, creatum,"
            "  mutatum) VALUES (?, ?, ?, ?, ?, ?, ?)");

        si (e == NIHIL)
        {
            redde;
        }
        scrinium_ligare_textum(e, I, res_id);
        scrinium_ligare_textum(e, II, genus_columna);
        scrinium_ligare_textum(e, III, c_titulus);
        scrinium_ligare_textum(e, IV, c_status);
        scrinium_ligare_textum(e, V, c_datum);
        scrinium_ligare_textum(e, VI, creatum_columna);
        scrinium_ligare_textum(e, VII, creatum);
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
    }
}

/* eventum unum in plicaturam nexuum applicare (chunk B; aurea X;
 * TS conformatio: smaragda.ts:759-769 membra role-clavata ->
 * tripla plana nostra, spec par XIII) */
interior vacuum
_nexui_applicare (GestaMundus* m, chorda res_id,
    chorda genus_eventus, chorda datum, Piscina* piscina)
{
    b32 est_nexus = _chorda_est(genus_eventus, "nexus");
    b32 est_denexus = _chorda_est(genus_eventus, "denexus");
    JsonResultus r;
    JsonValor* verbum;
    JsonValor* alterum;
    ScriniumEnuntiatum* e;

    si (!est_nexus && !est_denexus)
    {
        redde;
    }
    r = json_legere(datum, piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde;
    }
    verbum = json_objectum_capere(r.radix, "verbum");
    alterum = json_objectum_capere(r.radix, "alterum");
    si (verbum == NIHIL || !json_est_chorda(verbum)
        || alterum == NIHIL || !json_est_chorda(alterum))
    {
        redde;
    }
    e = scrinium_praeparare(m->scrinium, est_nexus
        ? "INSERT OR IGNORE INTO nexus (res_a, verbum, res_b)"
          " VALUES (?, ?, ?)"
        : "DELETE FROM nexus WHERE res_a = ? AND verbum = ?"
          " AND res_b = ?");
    si (e == NIHIL)
    {
        redde;
    }
    scrinium_ligare_textum(e, I, res_id);
    scrinium_ligare_textum(e, II, json_ad_chorda(verbum));
    scrinium_ligare_textum(e, III, json_ad_chorda(alterum));
    (vacuum)scrinium_gradi(e);
    scrinium_finire(e);
}

/* genera consumptorum */
#define GESTA_CONSUMPTOR_RES    0
#define GESTA_CONSUMPTOR_GENERA 1
#define GESTA_CONSUMPTOR_NEXUS  2

/* consumptorem unum provehere: replicatio seq > hwm, applicatio,
 * hwm = seq ultima IN EADEM TRANSACTIONE (exacte-semel - emendatio
 * super TS at-least-once; libraries.ts:228-233) */
interior b32
_consumptorem_plicare (GestaMundus* m,
    constans character* consumptor, s32 genus_consumptoris)
{
    s64 hwm = _hwm_capere(m, consumptor);
    s64 ultima = hwm;
    ScriniumEnuntiatum* e;

    si (!scrinium_incipere(m->scrinium))
    {
        redde _fractum(m, scrinium_error(m->scrinium));
    }
    e = scrinium_praeparare(m->scrinium,
        "SELECT seq, res_id, genus_eventus, datum, actor, creatum"
        " FROM tessellae WHERE seq > ? ORDER BY seq");
    si (e == NIHIL)
    {
        (vacuum)scrinium_revolvere(m->scrinium);
        redde _fractum(m, scrinium_error(m->scrinium));
    }
    scrinium_ligare_numerum(e, I, hwm);
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        s64 seq = scrinium_columna_numerus(e, 0);
        chorda res_id = scrinium_columna_textus(e, I, m->piscina);
        chorda genus_ev = scrinium_columna_textus(e, II,
            m->piscina);
        chorda datum = scrinium_columna_textus(e, III, m->piscina);
        chorda actor = scrinium_columna_textus(e, IV, m->piscina);
        chorda creatum = scrinium_columna_textus(e, V, m->piscina);

        si (genus_consumptoris == GESTA_CONSUMPTOR_GENERA)
        {
            _generum_applicare(m, genus_ev, datum, m->piscina);
        }
        alioquin si (genus_consumptoris == GESTA_CONSUMPTOR_NEXUS)
        {
            _nexui_applicare(m, res_id, genus_ev, datum,
                m->piscina);
        }
        alioquin
        {
            _rei_applicare(m, res_id, genus_ev, datum, actor,
                creatum, m->piscina);
        }
        ultima = seq;
    }
    scrinium_finire(e);
    si (ultima > hwm)
    {
        ScriniumEnuntiatum* h = scrinium_praeparare(m->scrinium,
            "INSERT OR REPLACE INTO consumptores (titulus, seq)"
            " VALUES (?, ?)");

        si (h == NIHIL)
        {
            (vacuum)scrinium_revolvere(m->scrinium);
            redde _fractum(m, scrinium_error(m->scrinium));
        }
        scrinium_ligare_textum(h, I, _ch(consumptor));
        scrinium_ligare_numerum(h, II, ultima);
        (vacuum)scrinium_gradi(h);
        scrinium_finire(h);
    }
    si (!scrinium_committere(m->scrinium))
    {
        (vacuum)scrinium_revolvere(m->scrinium);
        redde _fractum(m, scrinium_error(m->scrinium));
    }
    redde VERUM;
}

b32
gesta_plicare (GestaMundus* mundus)
{
    si (mundus == NIHIL)
    {
        redde FALSUM;
    }
    /* ORDO PORTANS: genera ante res (spec-v2 par V); nexus tertius */
    si (!_consumptorem_plicare(mundus, "genera",
            GESTA_CONSUMPTOR_GENERA))
    {
        redde FALSUM;
    }
    si (!_consumptorem_plicare(mundus, "res",
            GESTA_CONSUMPTOR_RES))
    {
        redde FALSUM;
    }
    redde _consumptorem_plicare(mundus, "nexus",
        GESTA_CONSUMPTOR_NEXUS);
}

/* ==================================================
 * Scriptura
 * ================================================== */

/* lineam annalium construere et appendere (ANTE COMMIT - phase-log
 * decisio 5: ruina => annales superset, numquam historia amissa) */
interior b32
_annalem_appendere (GestaMundus* m, s64 seq,
    constans character* id_ev, chorda res_id,
    constans character* genus_eventus, JsonValor* datum_obiectum,
    constans character* actor, constans character* origo,
    chorda creatum)
{
    JsonValor* linea = json_objectum_creare(m->piscina);
    chorda textus;
    FILE* pl;

    json_objectum_ponere(linea, "seq",
        json_integer_creare(m->piscina, seq));
    json_objectum_ponere(linea, "id",
        json_chorda_creare(m->piscina, _ch(id_ev)));
    json_objectum_ponere(linea, "res_id",
        json_chorda_creare(m->piscina, res_id));
    json_objectum_ponere(linea, "genus_eventus",
        json_chorda_creare(m->piscina, _ch(genus_eventus)));
    json_objectum_ponere(linea, "datum", datum_obiectum);
    json_objectum_ponere(linea, "actor",
        json_chorda_creare(m->piscina, _ch(actor)));
    json_objectum_ponere(linea, "origo",
        json_chorda_creare(m->piscina, _ch(origo)));
    json_objectum_ponere(linea, "branch_id",
        json_chorda_creare(m->piscina, _ch("")));
    json_objectum_ponere(linea, "creatum",
        json_chorda_creare(m->piscina, creatum));
    textus = json_scribere(linea, m->piscina);

    pl = fopen(m->via_annalium, "ab");
    si (pl == NIHIL)
    {
        redde FALSUM;
    }
    si (textus.mensura > ZEPHYRUM
        && fwrite(textus.datum, I, (memoriae_index)textus.mensura,
               pl) != (memoriae_index)textus.mensura)
    {
        fclose(pl);
        redde FALSUM;
    }
    si (fputc('\n', pl) == EOF)
    {
        fclose(pl);
        redde FALSUM;
    }
    fflush(pl);
    fclose(pl);
    redde VERUM;
}

/* scriptura cruda: validatio + transactio + annales; SINE plicatura
 * (gesta_scribere eam semel in fine agit). nuntius_violationis_out
 * (NIHIL licet): litterae in piscina si machina violata / genus
 * ignotum - vocans notam custodiae appendit. */
interior b32
_scribere_crudum (GestaMundus* m, constans GestaEventum* e,
    character* res_id_out, constans character** violatio_out)
{
    JsonResultus r;
    JsonValor* datum_obiectum;
    chorda datum_canon;
    chorda res_id;
    character ulid_res[GESTA_RES_ID_MENSURA];
    character ulid_ev[SCRINIUM_ULID_MENSURA];
    constans character* genus_ev = e->genus_eventus;
    b32 est_creatio;
    b32 est_generis;
    s64 seq;
    chorda creatum;

    si (violatio_out != NIHIL)
    {
        *violatio_out = NIHIL;
    }
    si (genus_ev == NIHIL || e->actor == NIHIL)
    {
        redde _fractum(m, "eventum sine genere aut actore");
    }
    r = json_legere_literis(e->datum != NIHIL ? e->datum : "{}",
        m->piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde _fractum(m, "datum malformatum (obiectum JSON"
            " requiritur)");
    }
    datum_obiectum = r.radix;
    datum_canon = json_scribere(datum_obiectum, m->piscina);

    est_creatio = (strcmp(genus_ev, "creatio") == ZEPHYRUM);
    est_generis = _est_eventus_generis(_ch(genus_ev));

    /* res_id effectivum */
    si (est_generis)
    {
        JsonValor* titulus = json_objectum_capere(datum_obiectum,
            "titulus");

        si (titulus == NIHIL || !json_est_chorda(titulus))
        {
            redde _fractum(m, "eventus generis sine titulo");
        }
        res_id = json_ad_chorda(titulus);
    }
    alioquin si (est_creatio && e->res_id == NIHIL)
    {
        scrinium_ulid(ulid_res);
        res_id = _ch(ulid_res);
    }
    alioquin si (e->res_id != NIHIL)
    {
        res_id = _ch(e->res_id);
    }
    alioquin
    {
        redde _fractum(m, "eventum sine res_id");
    }

    /* identitas: creatio duplicata recusatur (phase-log decisio 2) */
    si (est_creatio)
    {
        GestaResOrdo ordo = _res_capere(m, res_id, m->piscina);

        si (ordo.exsistit)
        {
            redde _fractum(m, "creatio duplicata recusata (res iam"
                " exsistit)");
        }
    }

    /* validatio - iudicat, non obstat (violatio -> nota custodiae) */
    si (violatio_out != NIHIL)
    {
        si (est_creatio)
        {
            JsonValor* g = json_objectum_capere(datum_obiectum,
                "genus");

            si (g != NIHIL && json_est_chorda(g))
            {
                chorda gd = _genus_datum_capere(m,
                    json_ad_chorda(g), m->piscina);

                si (gd.mensura == ZEPHYRUM)
                {
                    *violatio_out = "violatio: genus ignotum ad"
                        " creationem (definitio-generis deest)";
                }
            }
        }
        alioquin si (strcmp(genus_ev, "status") == ZEPHYRUM)
        {
            GestaResOrdo ordo = _res_capere(m, res_id, m->piscina);
            JsonValor* novus = json_objectum_capere(datum_obiectum,
                "novus");

            si (ordo.exsistit && novus != NIHIL
                && json_est_chorda(novus)
                && ordo.genus.mensura > ZEPHYRUM)
            {
                chorda gd = _genus_datum_capere(m, ordo.genus,
                    m->piscina);

                si (!_transitio_licita(m->piscina, gd, ordo.status,
                        json_ad_chorda(novus)))
                {
                    *violatio_out = "violatio machinae: transitio"
                        " extra machinam generis";
                }
            }
        }
        alioquin si (est_generis)
        {
            /* status_initialis in machina appareat (si machina
             * datur) - lex definitionis */
            JsonValor* si_init = json_objectum_capere(
                datum_obiectum, "status_initialis");
            JsonValor* machina = json_objectum_capere(
                datum_obiectum, "machina");

            si (si_init != NIHIL && json_est_chorda(si_init)
                && machina != NIHIL && json_est_tabulatum(machina)
                && json_tabulatum_numerus(machina) > ZEPHYRUM)
            {
                chorda cs = json_ad_chorda(si_init);
                b32 inventum = FALSUM;
                i32 i;
                i32 n = json_tabulatum_numerus(machina);

                per (i = ZEPHYRUM; i < n && !inventum; i++)
                {
                    JsonValor* par = json_tabulatum_obtinere(
                        machina, i);
                    i32 j;

                    si (par == NIHIL || !json_est_tabulatum(par))
                    {
                        perge;
                    }
                    per (j = ZEPHYRUM;
                         j < json_tabulatum_numerus(par); j++)
                    {
                        chorda fin = json_ad_chorda(
                            json_tabulatum_obtinere(par, j));

                        si (fin.mensura == cs.mensura
                            && (cs.mensura == ZEPHYRUM
                                || memcmp(fin.datum, cs.datum,
                                       (memoriae_index)cs.mensura)
                                    == ZEPHYRUM))
                        {
                            inventum = VERUM;
                            frange;
                        }
                    }
                }
                si (!inventum)
                {
                    *violatio_out = "violatio definitionis:"
                        " status_initialis extra machinam";
                }
            }
        }
    }

    /* transactio: INSERT + creatum relegere + annales + sordidae */
    scrinium_ulid(ulid_ev);
    si (!scrinium_incipere(m->scrinium))
    {
        redde _fractum(m, scrinium_error(m->scrinium));
    }
    {
        ScriniumEnuntiatum* ins = scrinium_praeparare(m->scrinium,
            "INSERT INTO tessellae"
            " (id, res_id, genus_eventus, datum, actor, origo)"
            " VALUES (?, ?, ?, ?, ?, ?)");

        si (ins == NIHIL)
        {
            (vacuum)scrinium_revolvere(m->scrinium);
            redde _fractum(m, scrinium_error(m->scrinium));
        }
        scrinium_ligare_textum(ins, I, _ch(ulid_ev));
        scrinium_ligare_textum(ins, II, res_id);
        scrinium_ligare_textum(ins, III, _ch(genus_ev));
        scrinium_ligare_textum(ins, IV, datum_canon);
        scrinium_ligare_textum(ins, V, _ch(e->actor));
        scrinium_ligare_textum(ins, VI,
            _ch(e->origo != NIHIL ? e->origo : ""));
        si (scrinium_gradi(ins) != SCRINIUM_FACTUM)
        {
            scrinium_finire(ins);
            (vacuum)scrinium_revolvere(m->scrinium);
            redde _fractum(m, scrinium_error(m->scrinium));
        }
        scrinium_finire(ins);
    }
    seq = scrinium_ultimum_id(m->scrinium);
    {
        ScriniumEnuntiatum* sel = scrinium_praeparare(m->scrinium,
            "SELECT creatum FROM tessellae WHERE seq = ?");

        creatum = _ch("");
        si (sel != NIHIL)
        {
            scrinium_ligare_numerum(sel, I, seq);
            si (scrinium_gradi(sel) == SCRINIUM_ORDO)
            {
                creatum = scrinium_columna_textus(sel, 0,
                    m->piscina);
            }
            scrinium_finire(sel);
        }
    }
    si (!_annalem_appendere(m, seq, ulid_ev, res_id, genus_ev,
            datum_obiectum, e->actor,
            e->origo != NIHIL ? e->origo : "", creatum))
    {
        (vacuum)scrinium_revolvere(m->scrinium);
        redde _fractum(m, "annales non appensi");
    }
    si (!est_generis)
    {
        ScriniumEnuntiatum* so = scrinium_praeparare(m->scrinium,
            "INSERT OR IGNORE INTO sordidae (res_id) VALUES (?)");

        si (so != NIHIL)
        {
            scrinium_ligare_textum(so, I, res_id);
            (vacuum)scrinium_gradi(so);
            scrinium_finire(so);
        }
    }
    si (!scrinium_committere(m->scrinium))
    {
        (vacuum)scrinium_revolvere(m->scrinium);
        redde _fractum(m, scrinium_error(m->scrinium));
    }

    si (res_id_out != NIHIL)
    {
        i32 n = res_id.mensura < (i32)(GESTA_RES_ID_MENSURA - I)
            ? res_id.mensura : (i32)(GESTA_RES_ID_MENSURA - I);

        si (n > ZEPHYRUM)
        {
            memcpy(res_id_out, res_id.datum, (memoriae_index)n);
        }
        res_id_out[n] = '\0';
    }
    redde VERUM;
}

b32
gesta_scribere (GestaMundus* mundus, constans GestaEventum* eventum,
    character* res_id_out)
{
    constans character* violatio = NIHIL;
    character res_id_locale[GESTA_RES_ID_MENSURA];

    si (mundus == NIHIL || eventum == NIHIL)
    {
        redde FALSUM;
    }
    si (res_id_out == NIHIL)
    {
        res_id_out = res_id_locale;
    }
    si (!_scribere_crudum(mundus, eventum, res_id_out, &violatio))
    {
        redde FALSUM;
    }
    si (violatio != NIHIL)
    {
        /* nota custodiae (phase-log decisio 2): eventus primus IAM
         * scriptus - iudicat, non obstat */
        GestaEventum nota;
        JsonValor* d = json_objectum_creare(mundus->piscina);
        chorda d_textus;

        json_objectum_ponere(d, "textus",
            json_chorda_creare(mundus->piscina, _ch(violatio)));
        d_textus = json_scribere(d, mundus->piscina);
        nota.res_id = res_id_out;
        nota.genus_eventus = "nota";
        nota.datum = _litterae(mundus->piscina, d_textus);
        nota.actor = "machina";
        nota.origo = "gesta-custodia";
        si (!_scribere_crudum(mundus, &nota, NIHIL, NIHIL))
        {
            redde FALSUM;
        }
    }
    redde gesta_plicare(mundus);
}

/* ==================================================
 * Apertura / clausura
 * ================================================== */

GestaMundus*
gesta_aperire (Piscina* piscina, constans character* via_scrinii,
    constans character* via_annalium)
{
    GestaMundus* m;
    FILE* pl;

    si (piscina == NIHIL || via_scrinii == NIHIL
        || via_annalium == NIHIL)
    {
        redde NIHIL;
    }
    m = (GestaMundus*)piscina_allocare_ordinatum(piscina,
        (memoriae_index)magnitudo(GestaMundus),
        (memoriae_index)magnitudo(vacuum*));
    si (m == NIHIL)
    {
        redde NIHIL;
    }
    memset(m, ZEPHYRUM, magnitudo(GestaMundus));
    m->piscina = piscina;
    m->error = "";
    m->scrinium = scrinium_aperire(piscina, via_scrinii);
    si (m->scrinium == NIHIL)
    {
        redde NIHIL;
    }
    si (!scrinium_migrare(m->scrinium, GESTA_MIGRATIONES,
            GESTA_MIGRATIONES_NUMERUS))
    {
        scrinium_claudere(m->scrinium);
        redde NIHIL;
    }
    m->via_annalium = _litterae(piscina, _ch(via_annalium));
    /* annales creantur si absunt */
    pl = fopen(m->via_annalium, "ab");
    si (pl == NIHIL)
    {
        scrinium_claudere(m->scrinium);
        redde NIHIL;
    }
    fclose(pl);
    redde m;
}

vacuum
gesta_claudere (GestaMundus* mundus)
{
    si (mundus == NIHIL)
    {
        redde;
    }
    scrinium_claudere(mundus->scrinium);
    mundus->scrinium = NIHIL;
}

/* ==================================================
 * Replicatio + annales
 * ================================================== */

b32
gesta_replicare (GestaMundus* mundus)
{
    si (mundus == NIHIL)
    {
        redde FALSUM;
    }
    si (!scrinium_exsequi(mundus->scrinium, "DELETE FROM res")
        || !scrinium_exsequi(mundus->scrinium,
               "DELETE FROM genera")
        || !scrinium_exsequi(mundus->scrinium,
               "DELETE FROM nexus")
        || !scrinium_exsequi(mundus->scrinium,
               "DELETE FROM res_fts")
        || !scrinium_exsequi(mundus->scrinium,
               "DELETE FROM sordidae")
        || !scrinium_exsequi(mundus->scrinium,
               "DELETE FROM consumptores"))
    {
        redde _fractum(mundus, scrinium_error(mundus->scrinium));
    }
    si (!gesta_plicare(mundus))
    {
        redde FALSUM;
    }
    /* fabula replicationis FTS (INTENTIO B decisio 3): omnes res
     * sordidae fiunt - quaestio proxima indicem pigre reficit
     * (analogum libraries.ts:481-484, via cauda sordida) */
    si (!scrinium_exsequi(mundus->scrinium,
            "INSERT OR IGNORE INTO sordidae (res_id)"
            " SELECT res_id FROM res"))
    {
        redde _fractum(mundus, scrinium_error(mundus->scrinium));
    }
    redde VERUM;
}

/* ==================================================
 * FTS + quaestio + census (chunk B)
 * ================================================== */

/* columnam corpus construere: state.corpus + paria "clavis: valor"
 * attributorum chordarum (claves reservatae exceptae) + tags iuncta
 * (via 'extra' TS smaragda.ts:919-924, ADAPTATA: acies tags
 * inclusa - INTENTIO B decisio 1) */
interior chorda
_corpus_construere (Piscina* piscina, JsonValor* status_obiectum)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        CCLVI);
    JsonObjectumIterator iter;
    chorda k;
    JsonValor* v;
    JsonValor* corpus = json_objectum_capere(status_obiectum,
        "corpus");
    JsonValor* tags = json_objectum_capere(status_obiectum,
        "tags");

    si (corpus != NIHIL && json_est_chorda(corpus))
    {
        chorda_aedificator_appendere_chorda(aed,
            json_ad_chorda(corpus));
    }
    iter = json_objectum_iterator(status_obiectum);
    dum (json_objectum_iterator_proxima(&iter, &k, &v))
    {
        si (_chorda_est(k, "titulus") || _chorda_est(k, "status")
            || _chorda_est(k, "corpus") || _chorda_est(k, "notae")
            || _chorda_est(k, "tags")
            || _chorda_est(k, "ancorae"))
        {
            perge;
        }
        si (json_est_chorda(v))
        {
            chorda_aedificator_appendere_literis(aed, " ");
            chorda_aedificator_appendere_chorda(aed, k);
            chorda_aedificator_appendere_literis(aed, ": ");
            chorda_aedificator_appendere_chorda(aed,
                json_ad_chorda(v));
        }
    }
    si (tags != NIHIL && json_est_tabulatum(tags))
    {
        i32 i;

        per (i = ZEPHYRUM; i < json_tabulatum_numerus(tags); i++)
        {
            JsonValor* t = json_tabulatum_obtinere(tags, i);

            si (t != NIHIL && json_est_chorda(t))
            {
                chorda_aedificator_appendere_literis(aed, " ");
                chorda_aedificator_appendere_chorda(aed,
                    json_ad_chorda(t));
            }
        }
    }
    redde chorda_aedificator_finire(aed);
}

interior chorda
_notas_construere (Piscina* piscina, JsonValor* status_obiectum)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        CCLVI);
    JsonValor* notae = json_objectum_capere(status_obiectum,
        "notae");
    i32 i;

    si (notae != NIHIL && json_est_tabulatum(notae))
    {
        per (i = ZEPHYRUM; i < json_tabulatum_numerus(notae); i++)
        {
            JsonValor* n = json_tabulatum_obtinere(notae, i);
            JsonValor* t = (n != NIHIL)
                ? json_objectum_capere(n, "textus") : NIHIL;

            si (t != NIHIL && json_est_chorda(t))
            {
                si (i > ZEPHYRUM)
                {
                    chorda_aedificator_appendere_literis(aed, " ");
                }
                chorda_aedificator_appendere_chorda(aed,
                    json_ad_chorda(t));
            }
        }
    }
    redde chorda_aedificator_finire(aed);
}

b32
gesta_fts_exhaurire (GestaMundus* mundus)
{
    ScriniumEnuntiatum* sel;
    Xar* sordidae;
    i32 i;

    si (mundus == NIHIL)
    {
        redde FALSUM;
    }
    /* sordidas colligere ante transactionem scripturae */
    sordidae = xar_creare(mundus->piscina, (i32)magnitudo(chorda));
    sel = scrinium_praeparare(mundus->scrinium,
        "SELECT res_id FROM sordidae");
    si (sel == NIHIL || sordidae == NIHIL)
    {
        si (sel != NIHIL)
        {
            scrinium_finire(sel);
        }
        redde _fractum(mundus, scrinium_error(mundus->scrinium));
    }
    dum (scrinium_gradi(sel) == SCRINIUM_ORDO)
    {
        chorda* locus = (chorda*)xar_addere(sordidae);

        si (locus != NIHIL)
        {
            *locus = scrinium_columna_textus(sel, 0,
                mundus->piscina);
        }
    }
    scrinium_finire(sel);
    si (xar_numerus(sordidae) == ZEPHYRUM)
    {
        redde VERUM;
    }
    /* una transactione: delere-tunc-inserere quamque + purgatio
     * (TS: libraries.ts:1074-1098, :990-1038) */
    si (!scrinium_incipere(mundus->scrinium))
    {
        redde _fractum(mundus, scrinium_error(mundus->scrinium));
    }
    per (i = ZEPHYRUM; i < xar_numerus(sordidae); i++)
    {
        chorda res_id = *(chorda*)xar_obtinere(sordidae, i);
        GestaResOrdo ordo = _res_capere(mundus, res_id,
            mundus->piscina);
        ScriniumEnuntiatum* del = scrinium_praeparare(
            mundus->scrinium,
            "DELETE FROM res_fts WHERE res_id = ?");

        si (del == NIHIL)
        {
            (vacuum)scrinium_revolvere(mundus->scrinium);
            redde _fractum(mundus,
                scrinium_error(mundus->scrinium));
        }
        scrinium_ligare_textum(del, I, res_id);
        (vacuum)scrinium_gradi(del);
        scrinium_finire(del);

        si (ordo.exsistit)
        {
            JsonResultus r = json_legere(ordo.datum,
                mundus->piscina);
            JsonValor* st = (r.successus
                && json_est_objectum(r.radix))
                ? r.radix
                : json_objectum_creare(mundus->piscina);
            chorda corpus = _corpus_construere(mundus->piscina,
                st);
            chorda notae = _notas_construere(mundus->piscina, st);
            ScriniumEnuntiatum* ins = scrinium_praeparare(
                mundus->scrinium,
                "INSERT INTO res_fts (res_id, titulus, corpus,"
                " notae) VALUES (?, ?, ?, ?)");

            si (ins == NIHIL)
            {
                (vacuum)scrinium_revolvere(mundus->scrinium);
                redde _fractum(mundus,
                    scrinium_error(mundus->scrinium));
            }
            scrinium_ligare_textum(ins, I, res_id);
            scrinium_ligare_textum(ins, II,
                _chorda_tuta(ordo.titulus));
            scrinium_ligare_textum(ins, III, _chorda_tuta(corpus));
            scrinium_ligare_textum(ins, IV, _chorda_tuta(notae));
            (vacuum)scrinium_gradi(ins);
            scrinium_finire(ins);
        }
    }
    si (!scrinium_exsequi(mundus->scrinium,
            "DELETE FROM sordidae"))
    {
        (vacuum)scrinium_revolvere(mundus->scrinium);
        redde _fractum(mundus, scrinium_error(mundus->scrinium));
    }
    si (!scrinium_committere(mundus->scrinium))
    {
        (vacuum)scrinium_revolvere(mundus->scrinium);
        redde _fractum(mundus, scrinium_error(mundus->scrinium));
    }
    redde VERUM;
}

Xar*
gesta_quaerere (GestaMundus* mundus, constans character* textus,
    constans character* genus, constans character* status,
    Piscina* piscina)
{
    Xar* inventa;
    ScriniumEnuntiatum* e;
    b32 solum_spatia = VERUM;
    memoriae_index i;

    si (mundus == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    inventa = xar_creare(piscina, (i32)magnitudo(GestaInventum));
    si (inventa == NIHIL)
    {
        redde NIHIL;
    }
    /* quaestio vacua = nihil (TS: libraries.ts:1117) */
    si (textus == NIHIL)
    {
        redde inventa;
    }
    per (i = ZEPHYRUM; i < strlen(textus); i++)
    {
        si (textus[i] != ' ' && textus[i] != '\t')
        {
            solum_spatia = FALSUM;
            frange;
        }
    }
    si (solum_spatia)
    {
        redde inventa;
    }
    si (!gesta_fts_exhaurire(mundus))
    {
        redde inventa;
    }
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT f.res_id, r.genus, r.titulus, r.status"
        " FROM res_fts f JOIN res r ON r.res_id = f.res_id"
        " WHERE res_fts MATCH ?"
        " AND (?2 = '' OR r.genus = ?2)"
        " AND (?3 = '' OR r.status = ?3)"
        " ORDER BY bm25(res_fts) LIMIT 50");
    si (e == NIHIL)
    {
        redde inventa;
    }
    scrinium_ligare_textum(e, I, _ch(textus));
    scrinium_ligare_textum(e, II,
        _ch(genus != NIHIL ? genus : ""));
    scrinium_ligare_textum(e, III,
        _ch(status != NIHIL ? status : ""));
    /* error syntaxis MATCH -> gradi ERROR -> fructus vacuus
     * (honestum; citatio = stratum MCP) */
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        GestaInventum* inv = (GestaInventum*)xar_addere(inventa);

        si (inv != NIHIL)
        {
            inv->res_id = scrinium_columna_textus(e, 0, piscina);
            inv->genus = scrinium_columna_textus(e, I, piscina);
            inv->titulus = scrinium_columna_textus(e, II, piscina);
            inv->status = scrinium_columna_textus(e, III, piscina);
        }
    }
    scrinium_finire(e);
    redde inventa;
}

Xar*
gesta_census_generum (GestaMundus* mundus, Piscina* piscina)
{
    Xar* census;
    ScriniumEnuntiatum* e;

    si (mundus == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    census = xar_creare(piscina, (i32)magnitudo(GestaCensusOrdo));
    si (census == NIHIL)
    {
        redde NIHIL;
    }
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT genus, status, COUNT(*) FROM res"
        " GROUP BY genus, status ORDER BY genus, status");
    si (e == NIHIL)
    {
        redde census;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        GestaCensusOrdo* ordo = (GestaCensusOrdo*)xar_addere(
            census);

        si (ordo != NIHIL)
        {
            ordo->genus = scrinium_columna_textus(e, 0, piscina);
            ordo->status = scrinium_columna_textus(e, I, piscina);
            ordo->numerus = scrinium_columna_numerus(e, II);
        }
    }
    scrinium_finire(e);
    redde census;
}

Xar*
gesta_census_tagorum (GestaMundus* mundus, Piscina* piscina)
{
    Xar* census;
    ScriniumEnuntiatum* e;

    si (mundus == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    census = xar_creare(piscina, (i32)magnitudo(GestaTagNumerus));
    si (census == NIHIL)
    {
        redde NIHIL;
    }
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT datum FROM res");
    si (e == NIHIL)
    {
        redde census;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        chorda d = scrinium_columna_textus(e, 0, piscina);
        JsonResultus r = json_legere(d, piscina);
        JsonValor* tags;
        i32 i;

        si (!r.successus || !json_est_objectum(r.radix))
        {
            perge;
        }
        tags = json_objectum_capere(r.radix, "tags");
        si (tags == NIHIL || !json_est_tabulatum(tags))
        {
            perge;
        }
        per (i = ZEPHYRUM; i < json_tabulatum_numerus(tags); i++)
        {
            JsonValor* t = json_tabulatum_obtinere(tags, i);
            chorda ct;
            i32 j;
            b32 inventum = FALSUM;

            si (t == NIHIL || !json_est_chorda(t))
            {
                perge;
            }
            ct = json_ad_chorda(t);
            per (j = ZEPHYRUM; j < xar_numerus(census); j++)
            {
                GestaTagNumerus* tn = (GestaTagNumerus*)
                    xar_obtinere(census, j);

                si (tn != NIHIL && tn->tag.mensura == ct.mensura
                    && (ct.mensura == ZEPHYRUM
                        || memcmp(tn->tag.datum, ct.datum,
                               (memoriae_index)ct.mensura)
                            == ZEPHYRUM))
                {
                    tn->numerus = tn->numerus + I;
                    inventum = VERUM;
                    frange;
                }
            }
            si (!inventum)
            {
                GestaTagNumerus* tn = (GestaTagNumerus*)
                    xar_addere(census);

                si (tn != NIHIL)
                {
                    /* copia stabilis in piscinam datam */
                    character* cp = (character*)piscina_allocare(
                        piscina, (memoriae_index)ct.mensura + I);

                    si (cp != NIHIL)
                    {
                        si (ct.mensura > ZEPHYRUM)
                        {
                            memcpy(cp, ct.datum,
                                (memoriae_index)ct.mensura);
                        }
                        cp[ct.mensura] = '\0';
                        tn->tag = _ch(cp);
                    }
                    alioquin
                    {
                        tn->tag = _ch("");
                    }
                    tn->numerus = I;
                }
            }
        }
    }
    scrinium_finire(e);
    redde census;
}

b32
gesta_annales_verificare (GestaMundus* mundus)
{
    character* textus;
    i32 mensura = ZEPHYRUM;
    i32 cursor = ZEPHYRUM;
    ScriniumEnuntiatum* e;
    b32 fructus = VERUM;
    b32 acta_exhausta = FALSUM;

    si (mundus == NIHIL)
    {
        redde FALSUM;
    }
    textus = _plagulam_legere(mundus->piscina, mundus->via_annalium,
        &mensura);
    si (textus == NIHIL)
    {
        redde _fractum(mundus, "annales illegibiles");
    }
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT seq, id, res_id, genus_eventus, datum, actor,"
        " origo, creatum FROM tessellae ORDER BY seq");
    si (e == NIHIL)
    {
        redde _fractum(mundus, scrinium_error(mundus->scrinium));
    }
    dum (cursor < mensura)
    {
        i32 initium = cursor;
        chorda linea;
        JsonResultus r;

        dum (cursor < mensura && textus[cursor] != '\n')
        {
            cursor++;
        }
        linea.datum = (i8*)(textus + initium);
        linea.mensura = (i32)(cursor - initium);
        si (cursor < mensura)
        {
            cursor++;   /* '\n' transilitur */
        }
        si (linea.mensura == ZEPHYRUM)
        {
            perge;   /* linea vacua toleratur */
        }
        r = json_legere(linea, mundus->piscina);
        si (!r.successus || !json_est_objectum(r.radix))
        {
            mundus->error = (cursor >= mensura)
                ? "annales: linea lacera in cauda (fenestra ruinae)"
                : "annales corrupti (linea malformata intra)";
            fructus = FALSUM;
            frange;
        }
        si (acta_exhausta || scrinium_gradi(e) != SCRINIUM_ORDO)
        {
            acta_exhausta = VERUM;
            /* linea valida sine ordine actorum */
            mundus->error = (cursor >= mensura)
                ? "annales: linea extra in cauda (superset -"
                  " fenestra ruinae, sananda)"
                : "annales: lineae plures quam acta";
            fructus = FALSUM;
            frange;
        }
        {
            s64 seq_ordo = scrinium_columna_numerus(e, 0);
            chorda id_o = scrinium_columna_textus(e, I,
                mundus->piscina);
            chorda rid_o = scrinium_columna_textus(e, II,
                mundus->piscina);
            chorda gev_o = scrinium_columna_textus(e, III,
                mundus->piscina);
            chorda dat_o = scrinium_columna_textus(e, IV,
                mundus->piscina);
            chorda act_o = scrinium_columna_textus(e, V,
                mundus->piscina);
            chorda org_o = scrinium_columna_textus(e, VI,
                mundus->piscina);
            chorda cre_o = scrinium_columna_textus(e, VII,
                mundus->piscina);
            JsonValor* v_datum = json_objectum_capere(r.radix,
                "datum");
            chorda dat_l = (v_datum != NIHIL)
                ? json_scribere(v_datum, mundus->piscina)
                : _ch("");

            si (json_ad_integer(json_objectum_capere(r.radix,
                    "seq")) != seq_ordo
                || !_chorda_est(json_ad_chorda(json_objectum_capere(
                       r.radix, "id")),
                       _litterae(mundus->piscina, id_o))
                || !_chorda_est(json_ad_chorda(json_objectum_capere(
                       r.radix, "res_id")),
                       _litterae(mundus->piscina, rid_o))
                || !_chorda_est(json_ad_chorda(json_objectum_capere(
                       r.radix, "genus_eventus")),
                       _litterae(mundus->piscina, gev_o))
                || !_chorda_est(dat_l,
                       _litterae(mundus->piscina, dat_o))
                || !_chorda_est(json_ad_chorda(json_objectum_capere(
                       r.radix, "actor")),
                       _litterae(mundus->piscina, act_o))
                || !_chorda_est(json_ad_chorda(json_objectum_capere(
                       r.radix, "origo")),
                       _litterae(mundus->piscina, org_o))
                || !_chorda_est(json_ad_chorda(json_objectum_capere(
                       r.radix, "creatum")),
                       _litterae(mundus->piscina, cre_o)))
            {
                mundus->error = "annales: linea ordini non"
                    " congruit";
                fructus = FALSUM;
                frange;
            }
        }
    }
    si (fructus && scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        mundus->error = "annales incompleti (acta plura quam"
            " lineae)";
        fructus = FALSUM;
    }
    scrinium_finire(e);
    redde fructus;
}

GestaMundus*
gesta_ex_annalibus_restituere (Piscina* piscina,
    constans character* via_annalium,
    constans character* via_scrinii_novi)
{
    GestaMundus* m;
    character* textus;
    i32 mensura = ZEPHYRUM;
    i32 cursor = ZEPHYRUM;

    m = gesta_aperire(piscina, via_scrinii_novi, via_annalium);
    si (m == NIHIL)
    {
        redde NIHIL;
    }
    textus = _plagulam_legere(piscina, via_annalium, &mensura);
    si (textus == NIHIL)
    {
        redde NIHIL;
    }
    dum (cursor < mensura)
    {
        i32 initium = cursor;
        chorda linea;
        JsonResultus r;

        dum (cursor < mensura && textus[cursor] != '\n')
        {
            cursor++;
        }
        linea.datum = (i8*)(textus + initium);
        linea.mensura = (i32)(cursor - initium);
        si (cursor < mensura)
        {
            cursor++;
        }
        si (linea.mensura == ZEPHYRUM)
        {
            perge;
        }
        r = json_legere(linea, piscina);
        si (!r.successus || !json_est_objectum(r.radix))
        {
            perge;   /* linea lacera in cauda praetermittitur -
                      * verificare eam nominat */
        }
        {
            ScriniumEnuntiatum* ins = scrinium_praeparare(
                m->scrinium,
                "INSERT INTO tessellae (seq, id, res_id,"
                " genus_eventus, datum, actor, origo, branch_id,"
                " creatum) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?)");
            JsonValor* v_datum;
            chorda dat_l;

            si (ins == NIHIL)
            {
                redde NIHIL;
            }
            v_datum = json_objectum_capere(r.radix, "datum");
            dat_l = (v_datum != NIHIL)
                ? json_scribere(v_datum, piscina) : _ch("{}");
            scrinium_ligare_numerum(ins, I,
                json_ad_integer(json_objectum_capere(r.radix,
                    "seq")));
            scrinium_ligare_textum(ins, II, _chorda_tuta(
                json_ad_chorda(json_objectum_capere(r.radix,
                    "id"))));
            scrinium_ligare_textum(ins, III, _chorda_tuta(
                json_ad_chorda(json_objectum_capere(r.radix,
                    "res_id"))));
            scrinium_ligare_textum(ins, IV, _chorda_tuta(
                json_ad_chorda(json_objectum_capere(r.radix,
                    "genus_eventus"))));
            scrinium_ligare_textum(ins, V, dat_l);
            scrinium_ligare_textum(ins, VI, _chorda_tuta(
                json_ad_chorda(json_objectum_capere(r.radix,
                    "actor"))));
            scrinium_ligare_textum(ins, VII, _chorda_tuta(
                json_ad_chorda(json_objectum_capere(r.radix,
                    "origo"))));
            scrinium_ligare_textum(ins, VIII, _chorda_tuta(
                json_ad_chorda(json_objectum_capere(r.radix,
                    "branch_id"))));
            scrinium_ligare_textum(ins, IX, _chorda_tuta(
                json_ad_chorda(json_objectum_capere(r.radix,
                    "creatum"))));
            si (scrinium_gradi(ins) != SCRINIUM_FACTUM)
            {
                scrinium_finire(ins);
                redde NIHIL;
            }
            scrinium_finire(ins);
        }
    }
    si (!gesta_replicare(m))
    {
        redde NIHIL;
    }
    redde m;
}

/* ==================================================
 * Lectiones publicae
 * ================================================== */

chorda
gesta_res_datum (GestaMundus* mundus, constans character* res_id,
    Piscina* piscina)
{
    GestaResOrdo ordo;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (mundus == NIHIL || res_id == NIHIL)
    {
        redde vacua;
    }
    ordo = _res_capere(mundus, _ch(res_id), piscina);
    redde ordo.exsistit ? ordo.datum : vacua;
}

chorda
gesta_res_status (GestaMundus* mundus, constans character* res_id,
    Piscina* piscina)
{
    GestaResOrdo ordo;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (mundus == NIHIL || res_id == NIHIL)
    {
        redde vacua;
    }
    ordo = _res_capere(mundus, _ch(res_id), piscina);
    redde ordo.exsistit ? ordo.status : vacua;
}

chorda
gesta_genus_datum (GestaMundus* mundus, constans character* titulus,
    Piscina* piscina)
{
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (mundus == NIHIL || titulus == NIHIL)
    {
        redde vacua;
    }
    redde _genus_datum_capere(mundus, _ch(titulus), piscina);
}

s64
gesta_seq_ultima (GestaMundus* mundus)
{
    ScriniumEnuntiatum* e;
    s64 seq = ZEPHYRUM;

    si (mundus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT COALESCE(MAX(seq), 0) FROM tessellae");
    si (e == NIHIL)
    {
        redde ZEPHYRUM;
    }
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        seq = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde seq;
}

s64
gesta_hwm (GestaMundus* mundus, constans character* consumptor)
{
    si (mundus == NIHIL || consumptor == NIHIL)
    {
        redde ZEPHYRUM;
    }
    redde _hwm_capere(mundus, consumptor);
}

Scrinium*
gesta_scrinium (GestaMundus* mundus)
{
    redde (mundus != NIHIL) ? mundus->scrinium : NIHIL;
}

constans character*
gesta_error (constans GestaMundus* mundus)
{
    redde (mundus != NIHIL) ? mundus->error : "";
}
