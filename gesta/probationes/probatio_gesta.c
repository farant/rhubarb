/* probatio_gesta.c - K1 chunk A: aureae 1-9, 13-14 (spec-v2 par XII)
 *
 * Quaeque aurea commentario 'TS: file:NNN' oraculum suum nominat -
 * expectata MANU derivata ex semanticis TS (smaragda.ts reductor;
 * libraries.ts HWM), bun numquam in constructione. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "json.h"
#include "gesta.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_DB  "gesta/build/probatio_gesta.db"
#define VIA_AN  "gesta/build/probatio_gesta.jsonl"
#define VIA_DB3 "gesta/build/probatio_gesta_restitutum.db"
#define VIA_DB4 "gesta/build/probatio_gesta_k3.db"
#define VIA_AN4 "gesta/build/probatio_gesta_k3.jsonl"
#define VIA_DB5 "gesta/build/probatio_gesta_k3b.db"
#define VIA_AN5 "gesta/build/probatio_gesta_k3b.jsonl"

interior vacuum
_purgare (vacuum)
{
    remove(VIA_DB);
    remove(VIA_DB "-wal");
    remove(VIA_DB "-shm");
    remove(VIA_AN);
    remove(VIA_DB3);
    remove(VIA_DB3 "-wal");
    remove(VIA_DB3 "-shm");
    remove(VIA_DB4);
    remove(VIA_DB4 "-wal");
    remove(VIA_DB4 "-shm");
    remove(VIA_AN4);
    remove(VIA_DB5);
    remove(VIA_DB5 "-wal");
    remove(VIA_DB5 "-shm");
    remove(VIA_AN5);
}

/* scriptura brevis asserta */
interior vacuum
_scribe (GestaMundus* m, constans character* res_id,
    constans character* genus_ev, constans character* datum)
{
    GestaEventum e;

    e.res_id = res_id;
    e.genus_eventus = genus_ev;
    e.datum = datum;
    e.actor = "fran";
    e.origo = "probatio";
    CREDO_VERUM (gesta_scribere(m, &e, NIHIL));
}

interior JsonValor*
_status_entis (GestaMundus* m, constans character* res_id,
    Piscina* piscina)
{
    chorda d = gesta_res_datum(m, res_id, piscina);
    JsonResultus r;

    si (d.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    r = json_legere(d, piscina);
    redde r.successus ? r.radix : NIHIL;
}

interior b32
_clavis_est_chorda (JsonValor* obiectum, constans character* clavis,
    constans character* valor)
{
    JsonValor* v = json_objectum_capere(obiectum, clavis);
    chorda c;
    memoriae_index m;

    si (v == NIHIL || !json_est_chorda(v))
    {
        redde FALSUM;
    }
    c = json_ad_chorda(v);
    m = strlen(valor);
    redde (memoriae_index)c.mensura == m
        && memcmp(c.datum, valor, m) == ZEPHYRUM;
}

/* numerus notarum quarum textus substringam continet */
interior i32
_notae_continentes (JsonValor* obiectum, constans character* pars)
{
    JsonValor* notae = json_objectum_capere(obiectum, "notae");
    i32 inventae = ZEPHYRUM;
    i32 i;

    si (notae == NIHIL || !json_est_tabulatum(notae))
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < json_tabulatum_numerus(notae); i++)
    {
        JsonValor* n = json_tabulatum_obtinere(notae, i);
        JsonValor* t = (n != NIHIL)
            ? json_objectum_capere(n, "textus") : NIHIL;

        si (t != NIHIL && json_est_chorda(t))
        {
            chorda c = json_ad_chorda(t);
            memoriae_index mp = strlen(pars);
            i32 j;

            si ((memoriae_index)c.mensura >= mp)
            {
                per (j = ZEPHYRUM;
                     (memoriae_index)j
                         <= (memoriae_index)c.mensura - mp; j++)
                {
                    si (memcmp(c.datum + j, pars, mp) == ZEPHYRUM)
                    {
                        inventae++;
                        frange;
                    }
                }
            }
        }
    }
    redde inventae;
}

/* numerus ordinum membrorum in indice (pars NIHIL = res tota);
 * -I = apparatus fractus */
interior s64
_membra_numerus (GestaMundus* m, constans character* res_id,
    constans character* pars, Piscina* piscina)
{
    ScriniumEnuntiatum* e;
    s64 n = -I;

    e = scrinium_praeparare(gesta_scrinium(m), pars != NIHIL
        ? "SELECT COUNT(*) FROM membra WHERE res_id = ?1"
          " AND pars = ?2"
        : "SELECT COUNT(*) FROM membra WHERE res_id = ?1");
    si (e == NIHIL)
    {
        redde -I;
    }
    scrinium_ligare_textum(e, I, chorda_ex_literis(res_id,
        piscina));
    si (pars != NIHIL)
    {
        scrinium_ligare_textum(e, II, chorda_ex_literis(pars,
            piscina));
    }
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        n = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde n;
}

/* acies membrorum partis ex statu rei (NIHIL = absens) */
interior JsonValor*
_membra_pars (JsonValor* st, constans character* pars)
{
    JsonValor* membra = (st != NIHIL)
        ? json_objectum_capere(st, "membra") : NIHIL;

    si (membra == NIHIL || !json_est_objectum(membra))
    {
        redde NIHIL;
    }
    redde json_objectum_capere(membra, pars);
}

/* numerus querelarum dati typi in salute */
interior i32
_querelae_typi (constans GestaSalus* s, constans character* typus)
{
    i32 n = ZEPHYRUM;
    i32 i;
    memoriae_index m = strlen(typus);

    per (i = ZEPHYRUM; i < s->numerus; i++)
    {
        si ((memoriae_index)s->querelae[i].typus.mensura == m
            && memcmp(s->querelae[i].typus.datum, typus, m)
                == ZEPHYRUM)
        {
            n++;
        }
    }
    redde n;
}

/* mensura plagulae in octetis (-1 = absens) */
interior long
_mensura_plagulae (constans character* via)
{
    FILE* pl = fopen(via, "rb");
    long n;

    si (pl == NIHIL)
    {
        redde -1L;
    }
    fseek(pl, 0L, SEEK_END);
    n = ftell(pl);
    fclose(pl);
    redde n;
}

/* numerus ex SQL uno cum ligamine textūs ?1 (NIHIL = sine
 * ligamine); -I = apparatus fractus */
interior s64
_numerus_sql_t (GestaMundus* m, constans character* sql,
    constans character* ligamen, Piscina* piscina)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(gesta_scrinium(m),
        sql);
    s64 n = -I;

    si (e == NIHIL)
    {
        redde -I;
    }
    si (ligamen != NIHIL)
    {
        scrinium_ligare_textum(e, I, chorda_ex_literis(ligamen,
            piscina));
    }
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        n = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde n;
}

/* numerus ex SQL uno cum ligamine numeri ?1 */
interior s64
_numerus_sql_s (GestaMundus* m, constans character* sql,
    s64 ligamen)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(gesta_scrinium(m),
        sql);
    s64 n = -I;

    si (e == NIHIL)
    {
        redde -I;
    }
    scrinium_ligare_numerum(e, I, ligamen);
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        n = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde n;
}

/* genus rei ex columna (chorda vacua = res absens) */
interior chorda
_genus_rei (GestaMundus* m, constans character* res_id,
    Piscina* piscina)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(gesta_scrinium(m),
        "SELECT genus FROM res WHERE res_id = ?");
    chorda g;

    g.mensura = ZEPHYRUM;
    g.datum = NIHIL;
    si (e == NIHIL)
    {
        redde g;
    }
    scrinium_ligare_textum(e, I, chorda_ex_literis(res_id,
        piscina));
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        g = scrinium_columna_textus(e, 0, piscina);
    }
    scrinium_finire(e);
    redde g;
}

interior b32
_chorda_aequa (chorda c, constans character* lit)
{
    memoriae_index m = strlen(lit);

    redde (memoriae_index)c.mensura == m
        && (m == ZEPHYRUM || memcmp(c.datum, lit, m) == ZEPHYRUM);
}

/* titulus in Xar chordarum (gesta_actiones_rei) inest? */
interior b32
_actiones_continent (Xar* actiones, constans character* titulus)
{
    i32 i;

    si (actiones == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(actiones); i++)
    {
        chorda* c = (chorda*)xar_obtinere(actiones, i);

        si (c != NIHIL && _chorda_aequa(*c, titulus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* chorda res_id in quaternionem NUL-terminatum */
interior vacuum
_res_id_figere (chorda c, character* quaternio)
{
    i32 n = c.mensura < (i32)(GESTA_RES_ID_MENSURA - I)
        ? c.mensura : (i32)(GESTA_RES_ID_MENSURA - I);

    si (n > ZEPHYRUM)
    {
        memcpy(quaternio, c.datum, (memoriae_index)n);
    }
    quaternio[n] = '\0';
}

/* numerus eventuum dati generis in flumine rei */
interior s64
_eventus_rei (GestaMundus* m, constans character* res_id,
    constans character* genus_eventus, Piscina* piscina)
{
    ScriniumEnuntiatum* e = scrinium_praeparare(gesta_scrinium(m),
        "SELECT COUNT(*) FROM tessellae WHERE res_id = ?1"
        " AND genus_eventus = ?2");
    s64 n = -I;

    si (e == NIHIL)
    {
        redde -I;
    }
    scrinium_ligare_textum(e, I, chorda_ex_literis(res_id,
        piscina));
    scrinium_ligare_textum(e, II, chorda_ex_literis(genus_eventus,
        piscina));
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        n = scrinium_columna_numerus(e, 0);
    }
    scrinium_finire(e);
    redde n;
}

/* cursus gradūs instantiae valori aequat? (valor NIHIL = absens
 * asserendus) */
interior b32
_cursus_est (GestaMundus* m, constans character* instantia,
    constans character* gradus, constans character* valor,
    Piscina* piscina)
{
    JsonValor* st = _status_entis(m, instantia, piscina);
    JsonValor* cursus = (st != NIHIL)
        ? json_objectum_capere(st, "cursus") : NIHIL;
    JsonValor* v = (cursus != NIHIL && json_est_objectum(cursus))
        ? json_objectum_capere(cursus, gradus) : NIHIL;

    si (v == NIHIL || !json_est_chorda(v))
    {
        redde valor == NIHIL;
    }
    si (valor == NIHIL)
    {
        redde FALSUM;
    }
    redde _chorda_aequa(json_ad_chorda(v), valor);
}

/* opus generatum gradūs ex statu instantiae (FALSUM = absens) */
interior b32
_opus_gradus (GestaMundus* m, constans character* instantia,
    constans character* gradus, character* quaternio,
    Piscina* piscina)
{
    JsonValor* st = _status_entis(m, instantia, piscina);
    JsonValor* opera = (st != NIHIL)
        ? json_objectum_capere(st, "opera") : NIHIL;
    JsonValor* v = (opera != NIHIL && json_est_objectum(opera))
        ? json_objectum_capere(opera, gradus) : NIHIL;

    si (v == NIHIL || !json_est_chorda(v))
    {
        redde FALSUM;
    }
    _res_id_figere(json_ad_chorda(v), quaternio);
    redde VERUM;
}

s32 principale (vacuum)
{
    Piscina* piscina;
    GestaMundus* m;
    b32 praeteritus;
    character id_a[GESTA_RES_ID_MENSURA];
    character id_b[GESTA_RES_ID_MENSURA];
    character id_q[GESTA_RES_ID_MENSURA];
    character id_l[GESTA_RES_ID_MENSURA];
    character id_lv[GESTA_RES_ID_MENSURA];
    character id_l2[GESTA_RES_ID_MENSURA];
    character id_l3[GESTA_RES_ID_MENSURA];
    character id_ls[GESTA_RES_ID_MENSURA];
    character id_c1[GESTA_RES_ID_MENSURA];
    character id_c2[GESTA_RES_ID_MENSURA];
    character id_o[GESTA_RES_ID_MENSURA];
    character id_o2[GESTA_RES_ID_MENSURA];
    character datum_ev[CXXVIII];

    piscina = piscina_generare_dynamicum("probatio_gesta",
        134217728);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    _purgare();

    m = gesta_aperire(piscina, VIA_DB, VIA_AN);
    CREDO_NON_NIHIL (m);
    si (m == NIHIL)
    {
        redde I;
    }

    /* ========================================================
     * I. Seq 1-basata (aurea VIII pars; TS: libraries.ts:245-247,
     * :373-378 - consumptor recens hwm=0 nihil praetermittat)
     * ======================================================== */
    _scribe(m, NIHIL, "creatio", "{\"titulus\":\"Prima\"}");
    CREDO_VERUM (gesta_seq_ultima(m) == (s64)I);
    CREDO_VERUM (gesta_hwm(m, "res") == (s64)I);

    /* ========================================================
     * II. Mersio superficialis (aurea I; TS: smaragda.ts:727-730)
     * ======================================================== */
    {
        GestaEventum e;
        JsonValor* st;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"titulus\":\"Merge\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_a));
        _scribe(m, id_a, "mutatio", "{\"a\":\"1\"}");
        _scribe(m, id_a, "mutatio", "{\"b\":\"2\"}");
        _scribe(m, id_a, "mutatio", "{\"a\":\"9\"}");
        st = _status_entis(m, id_a, piscina);
        CREDO_NON_NIHIL (st);
        CREDO_VERUM (_clavis_est_chorda(st, "a", "9"));
        CREDO_VERUM (_clavis_est_chorda(st, "b", "2"));
        CREDO_VERUM (_clavis_est_chorda(st, "titulus", "Merge"));
    }

    /* ========================================================
     * III. remotio != mutatio-ad-nihil (aurea II; TS:
     * smaragda.ts:731-736 contra :727)
     * ======================================================== */
    {
        JsonValor* st;

        _scribe(m, id_a, "mutatio", "{\"k\":\"5\"}");
        _scribe(m, id_a, "remotio", "{\"clavis\":\"k\"}");
        st = _status_entis(m, id_a, piscina);
        CREDO_VERUM (!json_objectum_habet(st, "k"));

        _scribe(m, id_a, "mutatio", "{\"k\":null}");
        st = _status_entis(m, id_a, piscina);
        CREDO_VERUM (json_objectum_habet(st, "k"));
        CREDO_VERUM (json_est_nullum(json_objectum_capere(st,
            "k")));
    }

    /* ========================================================
     * IV. Acies TOTA substituitur (aurea III; TS: smaragda.ts:729,
     * :8386 - nulla mersio partialis)
     * ======================================================== */
    {
        JsonValor* st;
        JsonValor* tags;

        _scribe(m, id_a, "mutatio", "{\"tags\":[\"a\"]}");
        _scribe(m, id_a, "mutatio", "{\"tags\":[\"b\",\"c\"]}");
        st = _status_entis(m, id_a, piscina);
        tags = json_objectum_capere(st, "tags");
        CREDO_NON_NIHIL (tags);
        CREDO_AEQUALIS_I32 ((i32)json_tabulatum_numerus(tags), II);
        {
            chorda primus = json_ad_chorda(
                json_tabulatum_obtinere(tags, ZEPHYRUM));

            CREDO_VERUM (primus.mensura == I
                && primus.datum[ZEPHYRUM] == (i8)'b');
        }
    }

    /* ========================================================
     * V. Genus ignotum = nihil agit (aurea IV; TS:
     * smaragda.ts:771-772); nexus quoque hic cadit donec chunk B
     * ======================================================== */
    {
        chorda ante = gesta_res_datum(m, id_a, piscina);
        chorda post;

        _scribe(m, id_a, "xyzzy", "{\"quidquid\":\"x\"}");
        _scribe(m, id_a, "nexus",
            "{\"verbum\":\"impedit\",\"alterum\":\"nemo\"}");
        post = gesta_res_datum(m, id_a, piscina);
        CREDO_VERUM (ante.mensura == post.mensura
            && memcmp(ante.datum, post.datum,
                   (memoriae_index)ante.mensura) == ZEPHYRUM);
    }

    /* ========================================================
     * VI. Genus definitum + creatio: status initialis; machina
     * caeca in plicatura, iudicium ad scripturam (aureae V + VII;
     * TS: smaragda.ts:737-739, :2099-2100, :2152/:2167 [TS
     * obstat - nos iudicamus, divergentia par XIII])
     * ======================================================== */
    _scribe(m, NIHIL, "definitio-generis",
        "{\"titulus\":\"quaestio\",\"status_initialis\":"
        "\"apertum\",\"machina\":[[\"apertum\",\"laborans\"],"
        "[\"laborans\",\"clausum\"],[\"apertum\",\"clausum\"]],"
        "\"reducer\":\"ordinarius\"}");
    {
        GestaEventum e;
        JsonValor* st;
        chorda status;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"quaestio\",\"titulus\":\"Casus\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_q));
        status = gesta_res_status(m, id_q, piscina);
        CREDO_VERUM (status.mensura == (i32)strlen("apertum")
            && memcmp(status.datum, "apertum",
                   strlen("apertum")) == ZEPHYRUM);

        /* transitio licita: nulla nota custodiae */
        _scribe(m, id_q, "status", "{\"novus\":\"laborans\"}");
        st = _status_entis(m, id_q, piscina);
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "violatio machinae"), ZEPHYRUM);

        /* transitio ILLICITA (laborans -> apertum extra machinam):
         * status TAMEN ponitur + nota custodiae appenditur */
        _scribe(m, id_q, "status", "{\"novus\":\"apertum\"}");
        st = _status_entis(m, id_q, piscina);
        CREDO_VERUM (_clavis_est_chorda(st, "status", "apertum"));
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "violatio machinae"), I);
    }

    /* ========================================================
     * VII. Plicatura generum: emendatio = substitutio TOTA (aurea
     * VI; TS granularitas alia - smaragda.ts:1819-1822 documentata;
     * phase-log decisio 6); definitio prava -> nota custodiae
     * ======================================================== */
    {
        chorda gd;
        JsonResultus r;
        JsonValor* machina;

        _scribe(m, NIHIL, "emendatio-generis",
            "{\"titulus\":\"quaestio\",\"status_initialis\":"
            "\"apertum\",\"machina\":[[\"apertum\",\"clausum\"]],"
            "\"reducer\":\"ordinarius\"}");
        gd = gesta_genus_datum(m, "quaestio", piscina);
        CREDO_VERUM (gd.mensura > ZEPHYRUM);
        r = json_legere(gd, piscina);
        CREDO_VERUM (r.successus);
        machina = json_objectum_capere(r.radix, "machina");
        CREDO_AEQUALIS_I32 ((i32)json_tabulatum_numerus(machina),
            I);

        /* status_initialis extra machinam -> definitio TAMEN
         * plicatur + nota custodiae (record-don't-block) */
        _scribe(m, NIHIL, "definitio-generis",
            "{\"titulus\":\"pravum\",\"status_initialis\":"
            "\"alibi\",\"machina\":[[\"hic\",\"illic\"]]}");
        gd = gesta_genus_datum(m, "pravum", piscina);
        CREDO_VERUM (gd.mensura > ZEPHYRUM);
        {
            JsonValor* st = _status_entis(m, "pravum", piscina);

            CREDO_NON_NIHIL (st);
            CREDO_AEQUALIS_I32 (_notae_continentes(st,
                "violatio definitionis"), I);
        }
    }

    /* ========================================================
     * VIII. HWM: provectio + iniectio cruda (aurea VIII; TS:
     * libraries.ts:373-378, :349-350) - eventus crudus infra hwm
     * usque ad plicaturam invisibilis
     * ======================================================== */
    {
        s64 seq_ante = gesta_seq_ultima(m);
        Scrinium* s = gesta_scrinium(m);
        ScriniumEnuntiatum* ins;
        JsonValor* st;

        CREDO_VERUM (gesta_hwm(m, "res") == seq_ante);
        ins = scrinium_praeparare(s,
            "INSERT INTO tessellae (id, res_id, genus_eventus,"
            " datum, actor, origo) VALUES ('CRUDUS0000000000000000"
            "000001', ?, 'mutatio', '{\"crudus\":\"ita\"}',"
            " 'machina', 'probatio-cruda')");
        CREDO_NON_NIHIL (ins);
        scrinium_ligare_textum(ins, I, chorda_ex_literis(id_a,
            piscina));
        CREDO_VERUM (scrinium_gradi(ins) == SCRINIUM_FACTUM);
        scrinium_finire(ins);

        CREDO_VERUM (gesta_seq_ultima(m) == seq_ante + I);
        CREDO_VERUM (gesta_hwm(m, "res") == seq_ante);
        st = _status_entis(m, id_a, piscina);
        CREDO_VERUM (!json_objectum_habet(st, "crudus"));

        CREDO_VERUM (gesta_plicare(m));
        CREDO_VERUM (gesta_hwm(m, "res") == seq_ante + I);
        st = _status_entis(m, id_a, piscina);
        CREDO_VERUM (_clavis_est_chorda(st, "crudus", "ita"));
    }

    /* ========================================================
     * IX. Idempotentia replicationis fascis (aurea IX; TS
     * contractus: libraries.ts:228-233) - hwm ad nihilum retro,
     * replicatio super plicaturas stantes -> status idem
     * ======================================================== */
    {
        chorda ante = gesta_res_datum(m, id_a, piscina);
        chorda post;

        CREDO_VERUM (scrinium_exsequi(gesta_scrinium(m),
            "DELETE FROM consumptores WHERE titulus = 'res'"));
        CREDO_VERUM (gesta_plicare(m));
        post = gesta_res_datum(m, id_a, piscina);
        CREDO_VERUM (ante.mensura == post.mensura
            && memcmp(ante.datum, post.datum,
                   (memoriae_index)ante.mensura) == ZEPHYRUM);
    }

    /* ========================================================
     * X. Replicatio == tabulae stantes (aurea XIII pars i)
     * ======================================================== */
    {
        chorda res_ante = gesta_res_datum(m, id_a, piscina);
        chorda q_ante = gesta_res_datum(m, id_q, piscina);
        chorda g_ante = gesta_genus_datum(m, "quaestio", piscina);
        chorda res_post;
        chorda q_post;
        chorda g_post;

        CREDO_VERUM (gesta_replicare(m));
        res_post = gesta_res_datum(m, id_a, piscina);
        q_post = gesta_res_datum(m, id_q, piscina);
        g_post = gesta_genus_datum(m, "quaestio", piscina);
        CREDO_VERUM (res_ante.mensura == res_post.mensura
            && memcmp(res_ante.datum, res_post.datum,
                   (memoriae_index)res_ante.mensura) == ZEPHYRUM);
        CREDO_VERUM (q_ante.mensura == q_post.mensura
            && memcmp(q_ante.datum, q_post.datum,
                   (memoriae_index)q_ante.mensura) == ZEPHYRUM);
        CREDO_VERUM (g_ante.mensura == g_post.mensura
            && memcmp(g_ante.datum, g_post.datum,
                   (memoriae_index)g_ante.mensura) == ZEPHYRUM);
    }

    /* ========================================================
     * XI. Annales: verificatio + restitutio (aurea XIII partes
     * ii-iii). NB iniectio cruda VIII lineam annalium NON habet -
     * verificatio id honeste nominat ("acta plura quam lineae");
     * asserimus fructum FALSUM deinde restitutionem ex annalibus
     * quae acta VERA (sine crudo) refert.
     * ======================================================== */
    {
        GestaMundus* m3;
        JsonValor* st;

        /* acta continent ordinem crudum sine linea annalium ->
         * verificare FALSUM (honestum; scriptura per gesta_scribere
         * sola paritatem tenet) */
        CREDO_VERUM (!gesta_annales_verificare(m));

        m3 = gesta_ex_annalibus_restituere(piscina, VIA_AN,
            VIA_DB3);
        CREDO_NON_NIHIL (m3);
        si (m3 != NIHIL)
        {
            /* mundus restitutus actis annalium congruit */
            CREDO_VERUM (gesta_annales_verificare(m3));
            st = _status_entis(m3, id_a, piscina);
            CREDO_NON_NIHIL (st);
            CREDO_VERUM (_clavis_est_chorda(st, "a", "9"));
            CREDO_VERUM (_clavis_est_chorda(st, "titulus",
                "Merge"));
            /* ordo crudus in annalibus ABEST - status sine eo */
            CREDO_VERUM (!json_objectum_habet(st, "crudus"));

            /* ================================================
             * XII. Linea lacera in cauda detegitur (aurea XIII
             * pars iv) - in mundo restituto, cuius annales actis
             * PERFECTE congruunt (supra assertum)
             * ================================================ */
            {
                FILE* pl = fopen(VIA_AN, "ab");

                CREDO_NON_NIHIL (pl);
                si (pl != NIHIL)
                {
                    fputs("{\"seq\":999,\"lacera", pl);
                    fclose(pl);
                }
                CREDO_VERUM (!gesta_annales_verificare(m3));
                CREDO_VERUM (strstr(gesta_error(m3), "lacera")
                    != NIHIL);
            }
            gesta_claudere(m3);
        }
    }

    /* ========================================================
     * XIII. creatio: duplicata recusatur; cruda secunda plicaturam
     * RESET facit (aurea XIV; TS: smaragda.ts:726)
     * ======================================================== */
    {
        GestaEventum e;
        JsonValor* st;

        e.res_id = id_a;
        e.genus_eventus = "creatio";
        e.datum = "{\"titulus\":\"Duplicata\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (!gesta_scribere(m, &e, NIHIL));

        /* iniectio cruda: creatio secunda in actis -> replicatio
         * statum purgat (plicatura reset litteralis) */
        {
            ScriniumEnuntiatum* ins = scrinium_praeparare(
                gesta_scrinium(m),
                "INSERT INTO tessellae (id, res_id, genus_eventus,"
                " datum, actor, origo) VALUES ('CRUDUS00000000000"
                "00000000002', ?, 'creatio',"
                " '{\"titulus\":\"Renata\"}', 'machina',"
                " 'probatio-cruda')");

            CREDO_NON_NIHIL (ins);
            scrinium_ligare_textum(ins, I,
                chorda_ex_literis(id_a, piscina));
            CREDO_VERUM (scrinium_gradi(ins) == SCRINIUM_FACTUM);
            scrinium_finire(ins);
        }
        CREDO_VERUM (gesta_plicare(m));
        st = _status_entis(m, id_a, piscina);
        CREDO_NON_NIHIL (st);
        CREDO_VERUM (_clavis_est_chorda(st, "titulus", "Renata"));
        CREDO_VERUM (!json_objectum_habet(st, "a"));
        CREDO_VERUM (!json_objectum_habet(st, "tags"));
    }

    /* ========================================================
     * XIV. Eventus veteres nexus/denexus = TUMULI ubique (K2
     * cutover, D2): in actis cadunt (historia sancta), statum rei
     * NON tangunt (reductor ignotum), indicem membra NON tangunt
     * (assertum XXI). Aurea X vetus tabulam nexus probabat -
     * tabula ipsa migratione III sublata.
     * ======================================================== */
    {
        GestaEventum e;
        chorda ante;
        chorda post;
        s64 seq_ante;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"titulus\":\"Nexa\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_b));
        ante = gesta_res_datum(m, id_b, piscina);
        seq_ante = gesta_seq_ultima(m);
        _scribe(m, id_b, "nexus",
            "{\"verbum\":\"impedit\",\"alterum\":\"alia\"}");
        _scribe(m, id_b, "nexus",
            "{\"verbum\":\"impedit\",\"alterum\":\"alia\"}");
        _scribe(m, id_b, "nexus",
            "{\"verbum\":\"tangit\",\"alterum\":\"alia\"}");
        _scribe(m, id_b, "denexus",
            "{\"verbum\":\"impedit\",\"alterum\":\"alia\"}");
        CREDO_VERUM (gesta_seq_ultima(m) == seq_ante + IV);
        post = gesta_res_datum(m, id_b, piscina);
        CREDO_VERUM (ante.mensura == post.mensura
            && memcmp(ante.datum, post.datum,
                   (memoriae_index)ante.mensura) == ZEPHYRUM);
    }

    /* ========================================================
     * XV. FTS: prosa/tag/praefixum/filtra; quaestio vacua = nihil;
     * status-flip re-indexatur post exhaustionem (aurea XI; TS:
     * libraries.ts:1117, :990-1038; corpus-columna INTENTIO B 1)
     * ======================================================== */
    {
        GestaEventum e;
        character id_f[GESTA_RES_ID_MENSURA];
        Xar* inv;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"quaestio\",\"titulus\":"
            "\"Parsura fracta\",\"corpus\":\"expansio macronis"
            " in capite alieno fallit\",\"tags\":[\"urgens\","
            "\"lsp\"]}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_f));

        /* quaestio vacua / spatia = nihil */
        inv = gesta_quaerere(m, NIHIL, NIHIL, NIHIL, piscina);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(inv), ZEPHYRUM);
        inv = gesta_quaerere(m, "   ", NIHIL, NIHIL, piscina);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(inv), ZEPHYRUM);

        /* prosa (corpus) */
        inv = gesta_quaerere(m, "expansio", NIHIL, NIHIL, piscina);
        CREDO_VERUM ((i32)xar_numerus(inv) >= I);

        /* tag (in columna corpus iuncta) */
        inv = gesta_quaerere(m, "urgens", NIHIL, NIHIL, piscina);
        CREDO_VERUM ((i32)xar_numerus(inv) >= I);

        /* praefixum (idioma Latinum - 'parsur*' invenit
         * 'Parsura') */
        inv = gesta_quaerere(m, "parsur*", NIHIL, NIHIL, piscina);
        CREDO_VERUM ((i32)xar_numerus(inv) >= I);

        /* filtrum statûs: apertum (initialis ex genere) */
        inv = gesta_quaerere(m, "parsur*", "quaestio", "apertum",
            piscina);
        CREDO_VERUM ((i32)xar_numerus(inv) >= I);
        inv = gesta_quaerere(m, "parsur*", "quaestio", "clausum",
            piscina);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(inv), ZEPHYRUM);

        /* status-flip -> sordida -> quaestio proxima re-indexat */
        _scribe(m, id_f, "status", "{\"novus\":\"laborans\"}");
        inv = gesta_quaerere(m, "parsur*", "quaestio", "laborans",
            piscina);
        CREDO_VERUM ((i32)xar_numerus(inv) >= I);

        /* nota quaesibilis (columna notae) */
        _scribe(m, id_f, "nota",
            "{\"textus\":\"radix inventa in praeparatore\"}");
        inv = gesta_quaerere(m, "praeparatore", NIHIL, NIHIL,
            piscina);
        CREDO_VERUM ((i32)xar_numerus(inv) >= I);
    }

    /* ========================================================
     * XVI. Census generum + tagorum
     * ======================================================== */
    {
        Xar* cg = gesta_census_generum(m, piscina);
        Xar* ct = gesta_census_tagorum(m, piscina);
        b32 quaestio_inventa = FALSUM;
        b32 urgens_inventum = FALSUM;
        i32 i;

        CREDO_NON_NIHIL (cg);
        CREDO_NON_NIHIL (ct);
        per (i = ZEPHYRUM; i < xar_numerus(cg); i++)
        {
            GestaCensusOrdo* o = (GestaCensusOrdo*)xar_obtinere(cg,
                i);

            si (o != NIHIL && o->genus.mensura
                    == (i32)strlen("quaestio")
                && memcmp(o->genus.datum, "quaestio",
                       strlen("quaestio")) == ZEPHYRUM
                && o->numerus > ZEPHYRUM)
            {
                quaestio_inventa = VERUM;
            }
        }
        CREDO_VERUM (quaestio_inventa);
        per (i = ZEPHYRUM; i < xar_numerus(ct); i++)
        {
            GestaTagNumerus* t = (GestaTagNumerus*)xar_obtinere(ct,
                i);

            si (t != NIHIL && t->tag.mensura
                    == (i32)strlen("urgens")
                && memcmp(t->tag.datum, "urgens",
                       strlen("urgens")) == ZEPHYRUM
                && t->numerus == (s64)I)
            {
                urgens_inventum = VERUM;
            }
        }
        CREDO_VERUM (urgens_inventum);

        /* ordo numeri: numeri numquam crescunt trans indicem;
         * paritate tag lexicographice non decrescit (desideratum
         * 'Census tags ordine numeri') */
        per (i = I; i < xar_numerus(ct); i++)
        {
            GestaTagNumerus* prior = (GestaTagNumerus*)
                xar_obtinere(ct, i - I);
            GestaTagNumerus* hic_t = (GestaTagNumerus*)
                xar_obtinere(ct, i);

            CREDO_VERUM (prior != NIHIL && hic_t != NIHIL);
            si (prior == NIHIL || hic_t == NIHIL)
            {
                frange;
            }
            CREDO_VERUM (prior->numerus >= hic_t->numerus);
            si (prior->numerus == hic_t->numerus)
            {
                i32 minima = prior->tag.mensura
                        < hic_t->tag.mensura
                    ? prior->tag.mensura : hic_t->tag.mensura;
                integer c = (minima > ZEPHYRUM)
                    ? memcmp(prior->tag.datum, hic_t->tag.datum,
                          (memoriae_index)minima)
                    : ZEPHYRUM;

                CREDO_VERUM (c < ZEPHYRUM
                    || (c == ZEPHYRUM && prior->tag.mensura
                            <= hic_t->tag.mensura));
            }
        }
    }

    /* ========================================================
     * XVII. ULID: structura + ordo (aurea XII; TS: libraries.ts:
     * 82-122; C: lib/scrinium.c:326-382) - numquam valores
     * litterales
     * ======================================================== */
    {
        character u1[SCRINIUM_ULID_MENSURA];
        character u2[SCRINIUM_ULID_MENSURA];

        scrinium_ulid(u1);
        scrinium_ulid(u2);
        CREDO_AEQUALIS_I32 ((i32)strlen(u1), XXVI);
        CREDO_AEQUALIS_I32 ((i32)strlen(u2), XXVI);
        CREDO_VERUM (strcmp(u1, u2) < ZEPHYRUM);
    }

    /* ========================================================
     * XVIII. Replicatio cum FTS: post replicationem quaestio
     * indicem pigre reficit (INTENTIO B decisio 3)
     * ======================================================== */
    {
        Xar* inv;

        CREDO_VERUM (gesta_replicare(m));
        inv = gesta_quaerere(m, "parsur*", NIHIL, NIHIL, piscina);
        CREDO_VERUM ((i32)xar_numerus(inv) >= I);
    }

    /* ========================================================
     * XIX. K2 G1+G2: reductor membrorum (TS: smaragda.ts:759-769)
     * - additum appendit SINE dedup (:762); remotum occurrentias
     * OMNES tollit, clavis cum acie vacua MANET (:768); plicatura
     * membra ordines speculatur
     * ======================================================== */
    _scribe(m, NIHIL, "definitio-generis",
        "{\"titulus\":\"filum\",\"species\":\"nexus\","
        "\"partes\":[{\"titulus\":\"a\",\"genera_licita\":[],"
        "\"cardinalitas\":\"unicus\"},{\"titulus\":\"b\","
        "\"genera_licita\":[\"quaestio\"],\"cardinalitas\":"
        "\"aliquot\"}],\"attributa\":[{\"titulus\":\"verbum\","
        "\"typus\":\"textus\",\"necessarium\":true}],"
        "\"status_initialis\":\"vigens\",\"machina\":"
        "[[\"vigens\",\"solutum\"]],\"reducer\":\"ordinarius\"}");
    {
        GestaEventum e;
        JsonValor* st;
        JsonValor* acies;
        chorda status;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"filum\",\"verbum\":\"tangit\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_l));
        status = gesta_res_status(m, id_l, piscina);
        CREDO_VERUM (status.mensura == (i32)strlen("vigens")
            && memcmp(status.datum, "vigens", strlen("vigens"))
                == ZEPHYRUM);

        /* G1: bis additum -> acies II (sine dedup); secundum
         * tectum unicum excedit -> nota custodiae, eventus TAMEN
         * cadit (D3+D11) */
        sprintf(datum_ev,
            "{\"pars\":\"a\",\"membrum\":\"%s\"}", id_a);
        _scribe(m, id_l, "membrum-additum", datum_ev);
        _scribe(m, id_l, "membrum-additum", datum_ev);
        st = _status_entis(m, id_l, piscina);
        acies = _membra_pars(st, "a");
        CREDO_NON_NIHIL (acies);
        CREDO_AEQUALIS_I32 ((i32)json_tabulatum_numerus(acies),
            II);
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "cardinalitas unicus excessa"), I);
        CREDO_VERUM (_membra_numerus(m, id_l, "a", piscina)
            == (s64)II);

        /* G2: remotum unum -> occurrentiae AMBAE tolluntur; clavis
         * manet cum acie vacua; index ordines congruentes omnes
         * delet */
        _scribe(m, id_l, "membrum-remotum", datum_ev);
        st = _status_entis(m, id_l, piscina);
        acies = _membra_pars(st, "a");
        CREDO_NON_NIHIL (acies);
        CREDO_VERUM (json_est_tabulatum(acies));
        CREDO_AEQUALIS_I32 ((i32)json_tabulatum_numerus(acies),
            ZEPHYRUM);
        CREDO_VERUM (_membra_numerus(m, id_l, "a", piscina)
            == (s64)ZEPHYRUM);
    }

    /* ========================================================
     * XX. K2 G3: plicatura membrorum - consumptor recens ex seq I
     * replicat (lex plicaturae K1; hwm=0 nihil praetermittit)
     * ======================================================== */
    {
        sprintf(datum_ev,
            "{\"pars\":\"b\",\"membrum\":\"%s\"}", id_q);
        _scribe(m, id_l, "membrum-additum", datum_ev);
        CREDO_VERUM (_membra_numerus(m, id_l, "b", piscina)
            == (s64)I);

        CREDO_VERUM (scrinium_exsequi(gesta_scrinium(m),
            "DELETE FROM consumptores WHERE titulus = 'membra'"));
        CREDO_VERUM (scrinium_exsequi(gesta_scrinium(m),
            "DELETE FROM membra"));
        CREDO_VERUM (gesta_plicare(m));
        CREDO_VERUM (_membra_numerus(m, id_l, "b", piscina)
            == (s64)I);
        CREDO_VERUM (_membra_numerus(m, id_l, "a", piscina)
            == (s64)ZEPHYRUM);
    }

    /* ========================================================
     * XXI. K2 G4: eventus veteres nexus/denexus = TUMULI in
     * plicatura membrorum (D2) - id_b eos in actis habet (XIV);
     * replicatio quoque nihil ex eis in membra ponit
     * ======================================================== */
    {
        CREDO_VERUM (_membra_numerus(m, id_b, NIHIL, piscina)
            == (s64)ZEPHYRUM);
        CREDO_VERUM (gesta_replicare(m));
        CREDO_VERUM (_membra_numerus(m, id_b, NIHIL, piscina)
            == (s64)ZEPHYRUM);
        CREDO_VERUM (_membra_numerus(m, id_l, "b", piscina)
            == (s64)I);
    }

    /* ========================================================
     * XXII. K2 G5: custodia membrorum (spec par VI I-VIII) -
     * quaeque violatio notam parit ET eventus cadit (index rem
     * veram monstrat; TS obstat: smaragda.ts:3731-3783)
     * ======================================================== */
    {
        GestaEventum e;
        JsonValor* st;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"filum\",\"verbum\":\"vexat\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_lv));

        /* I. res non-nexus (id_a genus vacuum) - membrum id_b ne
         * quaesita XXIV polluat (eventus cadit, index rem
         * monstrat!) */
        sprintf(datum_ev,
            "{\"pars\":\"a\",\"membrum\":\"%s\"}", id_b);
        _scribe(m, id_a, "membrum-additum", datum_ev);
        st = _status_entis(m, id_a, piscina);
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "membrum in genere non-nexu"), I);

        /* II. pars ignota - eventus cadit, index rem monstrat */
        sprintf(datum_ev,
            "{\"pars\":\"c\",\"membrum\":\"%s\"}", id_a);
        _scribe(m, id_lv, "membrum-additum", datum_ev);
        st = _status_entis(m, id_lv, piscina);
        CREDO_AEQUALIS_I32 (_notae_continentes(st, "pars ignota"),
            I);
        CREDO_VERUM (_membra_numerus(m, id_lv, "c", piscina)
            == (s64)I);

        /* III. membrum inexistens */
        _scribe(m, id_lv, "membrum-additum",
            "{\"pars\":\"b\",\"membrum\":"
            "\"PHANTASMA00000000000000000\"}");
        st = _status_entis(m, id_lv, piscina);
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "membrum inexistens"), I);

        /* IV. genus membri non licitum (pars b quaestionem
         * requirit; id_b genus vacuum) */
        sprintf(datum_ev,
            "{\"pars\":\"b\",\"membrum\":\"%s\"}", id_b);
        _scribe(m, id_lv, "membrum-additum", datum_ev);
        st = _status_entis(m, id_lv, piscina);
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "genus membri non licitum"), I);

        /* VII. membrum absens (id_a in parte b numquam fuit) */
        sprintf(datum_ev,
            "{\"pars\":\"b\",\"membrum\":\"%s\"}", id_a);
        _scribe(m, id_lv, "membrum-remotum", datum_ev);
        st = _status_entis(m, id_lv, piscina);
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "membrum absens"), I);

        /* VIII. limen inferius: id_l pars b (aliquot) membrum unum
         * habet - remotio sub limen it, TAMEN fit */
        sprintf(datum_ev,
            "{\"pars\":\"b\",\"membrum\":\"%s\"}", id_q);
        _scribe(m, id_l, "membrum-remotum", datum_ev);
        st = _status_entis(m, id_l, piscina);
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "cardinalitas sub limite"), I);
        CREDO_VERUM (_membra_numerus(m, id_l, "b", piscina)
            == (s64)ZEPHYRUM);
    }

    /* ========================================================
     * XXIII. K2 G6: status solutum in re nexus-speciei indicem
     * purgat (res manet); in re ordinaria membra non tangit
     * ======================================================== */
    {
        GestaEventum e;
        JsonValor* st;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"filum\",\"verbum\":\"ligat\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_l2));
        sprintf(datum_ev,
            "{\"pars\":\"a\",\"membrum\":\"%s\"}", id_q);
        _scribe(m, id_l2, "membrum-additum", datum_ev);
        CREDO_VERUM (_membra_numerus(m, id_l2, NIHIL, piscina)
            == (s64)I);

        /* solutum in re ordinaria (id_q non-nexus): membra
         * aliena intacta */
        _scribe(m, id_q, "status", "{\"novus\":\"solutum\"}");
        CREDO_VERUM (_membra_numerus(m, id_l2, NIHIL, piscina)
            == (s64)I);

        /* solutum in vinculo: index purgatur, res manet, transitio
         * vigens->solutum licita (sine nota violationis) */
        _scribe(m, id_l2, "status", "{\"novus\":\"solutum\"}");
        CREDO_VERUM (_membra_numerus(m, id_l2, NIHIL, piscina)
            == (s64)ZEPHYRUM);
        st = _status_entis(m, id_l2, piscina);
        CREDO_NON_NIHIL (st);
        CREDO_VERUM (_clavis_est_chorda(st, "status", "solutum"));
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "violatio machinae"), ZEPHYRUM);
    }

    /* ========================================================
     * XXIV. K2 G13: quaesita reversa (TS: smaragda.ts:4008
     * getRelationshipsForEntity, :4119 getRelatedEntities)
     * ======================================================== */
    {
        GestaEventum e;
        Xar* vincula;
        Xar* socii;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"filum\",\"verbum\":\"spectat\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_l3));
        sprintf(datum_ev,
            "{\"pars\":\"a\",\"membrum\":\"%s\"}", id_a);
        _scribe(m, id_l3, "membrum-additum", datum_ev);
        sprintf(datum_ev,
            "{\"pars\":\"b\",\"membrum\":\"%s\"}", id_q);
        _scribe(m, id_l3, "membrum-additum", datum_ev);

        /* vincula rei id_q: solum id_l3.b stat (id_l.b remotum
         * XXII; id_l2 solutum XXIII) */
        vincula = gesta_nexus_rei(m, id_q, piscina);
        CREDO_NON_NIHIL (vincula);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(vincula), I);
        {
            GestaNexusRei* n = (GestaNexusRei*)xar_obtinere(
                vincula, ZEPHYRUM);

            CREDO_NON_NIHIL (n);
            si (n != NIHIL)
            {
                CREDO_VERUM (n->nexus_res.mensura
                        == (i32)strlen(id_l3)
                    && memcmp(n->nexus_res.datum, id_l3,
                           strlen(id_l3)) == ZEPHYRUM);
                CREDO_VERUM (n->pars.mensura == I
                    && n->pars.datum[ZEPHYRUM] == (i8)'b');
                CREDO_VERUM (n->genus.mensura
                        == (i32)strlen("filum")
                    && memcmp(n->genus.datum, "filum",
                           strlen("filum")) == ZEPHYRUM);
            }
        }

        /* socii rei id_q: id_a per id_l3 (pars a) */
        socii = gesta_socii_rei(m, id_q, piscina);
        CREDO_NON_NIHIL (socii);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(socii), I);
        {
            GestaSocius* s = (GestaSocius*)xar_obtinere(socii,
                ZEPHYRUM);

            CREDO_NON_NIHIL (s);
            si (s != NIHIL)
            {
                CREDO_VERUM (s->membrum.mensura
                        == (i32)strlen(id_a)
                    && memcmp(s->membrum.datum, id_a,
                           strlen(id_a)) == ZEPHYRUM);
                CREDO_VERUM (s->pars.mensura == I
                    && s->pars.datum[ZEPHYRUM] == (i8)'a');
                CREDO_VERUM (s->nexus_res.mensura
                        == (i32)strlen(id_l3)
                    && memcmp(s->nexus_res.datum, id_l3,
                           strlen(id_l3)) == ZEPHYRUM);
            }
        }
    }

    /* ========================================================
     * XXV. K2 G7: salus - attributum necessarium absens + LEX
     * CHORDAE VACUAE (TS: smaragda.ts:4374-4386, :4378 - absens,
     * null, ET "" pro deficiente numerantur)
     * ======================================================== */
    _scribe(m, NIHIL, "definitio-generis",
        "{\"titulus\":\"charta\",\"attributa\":["
        "{\"titulus\":\"auctor\",\"typus\":\"textus\","
        "\"necessarium\":true},"
        "{\"titulus\":\"paginae\",\"typus\":\"numerus\"},"
        "{\"titulus\":\"publica\",\"typus\":\"veritas\"},"
        "{\"titulus\":\"tags\",\"typus\":\"tabulatum\"},"
        "{\"titulus\":\"mirus\",\"typus\":\"arcanum\"}],"
        "\"reducer\":\"ordinarius\"}");
    {
        GestaEventum e;
        GestaSalus s;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"charta\",\"titulus\":\"Prima\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_c1));

        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_VERUM (!s.sanus);
        CREDO_AEQUALIS_I32 (s.numerus, I);
        CREDO_AEQUALIS_I32 (_querelae_typi(&s,
            "attributum-necessarium-absens"), I);
        CREDO_VERUM (s.querelae[ZEPHYRUM].gravis);

        /* chorda vacua = absens (LEX :4378) */
        _scribe(m, id_c1, "mutatio", "{\"auctor\":\"\"}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_AEQUALIS_I32 (_querelae_typi(&s,
            "attributum-necessarium-absens"), I);

        /* null = absens */
        _scribe(m, id_c1, "mutatio", "{\"auctor\":null}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_AEQUALIS_I32 (_querelae_typi(&s,
            "attributum-necessarium-absens"), I);

        _scribe(m, id_c1, "mutatio", "{\"auctor\":\"Plinius\"}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_VERUM (s.sanus);
    }

    /* ========================================================
     * XXVI. K2 G8: salus - typi quattuor + typus ignotus TRANSIT
     * (TS: smaragda.ts:1790-1798, :1796) + chorda vacua typum NON
     * iudicat (:4391)
     * ======================================================== */
    {
        GestaSalus s;

        /* textus pro numero -> cautio (gravis FALSUM) */
        _scribe(m, id_c1, "mutatio", "{\"paginae\":\"multae\"}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_AEQUALIS_I32 (s.numerus, I);
        CREDO_AEQUALIS_I32 (_querelae_typi(&s,
            "typus-attributi-pravus"), I);
        CREDO_VERUM (!s.querelae[ZEPHYRUM].gravis);
        _scribe(m, id_c1, "mutatio", "{\"paginae\":37}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_VERUM (s.sanus);

        /* chorda pro veritate */
        _scribe(m, id_c1, "mutatio", "{\"publica\":\"ita\"}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_AEQUALIS_I32 (_querelae_typi(&s,
            "typus-attributi-pravus"), I);
        _scribe(m, id_c1, "mutatio", "{\"publica\":true}");

        /* chorda pro tabulato */
        _scribe(m, id_c1, "mutatio", "{\"tags\":\"unus\"}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_AEQUALIS_I32 (_querelae_typi(&s,
            "typus-attributi-pravus"), I);
        _scribe(m, id_c1, "mutatio",
            "{\"tags\":[\"a\",\"b\"]}");

        /* typus ignotus 'arcanum' TRANSIT (lex progressiva) */
        _scribe(m, id_c1, "mutatio", "{\"mirus\":{\"x\":1}}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_VERUM (s.sanus);

        /* chorda vacua typum non iudicat (paginae numerus, valor
         * "" - nec absens-querela [non necessarium] nec typus) */
        _scribe(m, id_c1, "mutatio", "{\"paginae\":\"\"}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_VERUM (s.sanus);
    }

    /* ========================================================
     * XXVII. K2 G9: salus - status ignotus; genera sine machina
     * praetereunt (TS: smaragda.ts:4402-4412)
     * ======================================================== */
    {
        GestaSalus s;

        /* charta sine machina: status quilibet praeteritur */
        _scribe(m, id_c1, "mutatio",
            "{\"status\":\"quidlibet\"}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_c1, piscina,
            &s));
        CREDO_VERUM (s.sanus);

        /* id_q status 'solutum' (XXIII) extra machinam quaestionis
         * [[apertum,clausum]] */
        CREDO_VERUM (gesta_salutem_aestimare(m, id_q, piscina,
            &s));
        CREDO_AEQUALIS_I32 (s.numerus, I);
        CREDO_AEQUALIS_I32 (_querelae_typi(&s, "status-ignotus"),
            I);

        /* clausum finis machinae est - salus tacet (transitio
         * ipsa illicita = res custodiae, non salutis) */
        _scribe(m, id_q, "status", "{\"novus\":\"clausum\"}");
        CREDO_VERUM (gesta_salutem_aestimare(m, id_q, piscina,
            &s));
        CREDO_VERUM (s.sanus);
    }

    /* ========================================================
     * XXVIII. K2 G10: salus - cardinalitas: limen inferius (id_l
     * partes ambae vacuae) et tectum (id_ls pars a bis impleta)
     * ======================================================== */
    {
        GestaEventum e;
        GestaSalus s;

        /* id_l: verbum adest, status vigens validus, membra a=[]
         * b=[] -> querelae II (a unicus 0!=1; b aliquot 0<1) */
        CREDO_VERUM (gesta_salutem_aestimare(m, id_l, piscina,
            &s));
        CREDO_AEQUALIS_I32 (s.numerus, II);
        CREDO_AEQUALIS_I32 (_querelae_typi(&s,
            "cardinalitas-violata"), II);

        /* id_l3: a=[id_a] b=[id_q] verbum adest -> sanum */
        CREDO_VERUM (gesta_salutem_aestimare(m, id_l3, piscina,
            &s));
        CREDO_VERUM (s.sanus);

        /* tectum: pars a (unicus) bis impleta, pars b impleta ->
         * querela UNA sola (tectum a; b sana) */
        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"filum\",\"verbum\":\"premit\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_ls));
        sprintf(datum_ev,
            "{\"pars\":\"a\",\"membrum\":\"%s\"}", id_a);
        _scribe(m, id_ls, "membrum-additum", datum_ev);
        sprintf(datum_ev,
            "{\"pars\":\"a\",\"membrum\":\"%s\"}", id_q);
        _scribe(m, id_ls, "membrum-additum", datum_ev);
        sprintf(datum_ev,
            "{\"pars\":\"b\",\"membrum\":\"%s\"}", id_q);
        _scribe(m, id_ls, "membrum-additum", datum_ev);
        CREDO_VERUM (gesta_salutem_aestimare(m, id_ls, piscina,
            &s));
        CREDO_AEQUALIS_I32 (s.numerus, I);
        CREDO_AEQUALIS_I32 (_querelae_typi(&s,
            "cardinalitas-violata"), I);
    }

    /* ========================================================
     * XXIX. K2 G11: insalubres enumerantur; filtrum generis;
     * sanae excluduntur (TS: smaragda.ts:4447-4467)
     * ======================================================== */
    {
        GestaEventum e;
        Xar* ins;
        b32 c1_inventa = FALSUM;
        b32 c2_inventa = FALSUM;
        i32 i;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"charta\",\"titulus\":\"Secunda\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_c2));

        /* filtrum: chartarum insalubris una sola (id_c2 sine
         * auctore; id_c1 sana) */
        ins = gesta_insalubres_enumerare(m, "charta", piscina);
        CREDO_NON_NIHIL (ins);
        CREDO_AEQUALIS_I32 ((i32)xar_numerus(ins), I);
        {
            GestaInsalubris* in0 = (GestaInsalubris*)xar_obtinere(
                ins, ZEPHYRUM);

            CREDO_NON_NIHIL (in0);
            si (in0 != NIHIL)
            {
                CREDO_VERUM (in0->res_id.mensura
                        == (i32)strlen(id_c2)
                    && memcmp(in0->res_id.datum, id_c2,
                           strlen(id_c2)) == ZEPHYRUM);
                CREDO_AEQUALIS_I32 (in0->salus.numerus, I);
            }
        }

        /* sine filtro: id_l (cardinalitas) et id_c2 insunt,
         * id_c1 sana ABEST */
        ins = gesta_insalubres_enumerare(m, NIHIL, piscina);
        CREDO_NON_NIHIL (ins);
        CREDO_VERUM ((i32)xar_numerus(ins) >= II);
        per (i = ZEPHYRUM; i < xar_numerus(ins); i++)
        {
            GestaInsalubris* in_i = (GestaInsalubris*)xar_obtinere(
                ins, i);

            si (in_i == NIHIL)
            {
                perge;
            }
            si (in_i->res_id.mensura == (i32)strlen(id_c1)
                && memcmp(in_i->res_id.datum, id_c1,
                       strlen(id_c1)) == ZEPHYRUM)
            {
                c1_inventa = VERUM;
            }
            si (in_i->res_id.mensura == (i32)strlen(id_c2)
                && memcmp(in_i->res_id.datum, id_c2,
                       strlen(id_c2)) == ZEPHYRUM)
            {
                c2_inventa = VERUM;
            }
        }
        CREDO_VERUM (!c1_inventa);
        CREDO_VERUM (c2_inventa);
    }

    /* ========================================================
     * XXX. K2 G12: OPUS-GRATIS - genus opus per eventus solos
     * definitum, vita integra ambulata, NULLUS codex machinae
     * novus (TS opera = API vera smaragda.ts:4568-4611; nobis
     * genera-ut-eventus ea GRATIS dant - decretum K2 Q5).
     * Transitus rectus pendens->perfectum consulto licitus (TS
     * design note :4534-4535, adprobationes simplices)
     * ======================================================== */
    _scribe(m, NIHIL, "definitio-generis",
        "{\"titulus\":\"opus\",\"status_initialis\":\"pendens\","
        "\"machina\":[[\"pendens\",\"susceptum\"],"
        "[\"susceptum\",\"perfectum\"],"
        "[\"pendens\",\"perfectum\"],[\"pendens\",\"omissum\"],"
        "[\"susceptum\",\"omissum\"]],\"attributa\":["
        "{\"titulus\":\"titulus\",\"typus\":\"textus\","
        "\"necessarium\":true},"
        "{\"titulus\":\"assignatum\",\"typus\":\"textus\"},"
        "{\"titulus\":\"prioritas\",\"typus\":\"textus\"},"
        "{\"titulus\":\"effectus\",\"typus\":\"textus\"}],"
        "\"reducer\":\"ordinarius\"}");
    {
        GestaEventum e;
        GestaSalus s;
        JsonValor* st;
        chorda status;

        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"opus\",\"titulus\":"
            "\"Recense caput\",\"prioritas\":\"alta\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_o));
        status = gesta_res_status(m, id_o, piscina);
        CREDO_VERUM (status.mensura == (i32)strlen("pendens")
            && memcmp(status.datum, "pendens",
                   strlen("pendens")) == ZEPHYRUM);

        /* susceptio (claimTask :4583) = mutatio + status */
        _scribe(m, id_o, "mutatio",
            "{\"assignatum\":\"claude\"}");
        _scribe(m, id_o, "status", "{\"novus\":\"susceptum\"}");

        /* perfectio (completeTask :4594) = mutatio + status */
        _scribe(m, id_o, "mutatio",
            "{\"effectus\":\"recensum, probatum\"}");
        _scribe(m, id_o, "status", "{\"novus\":\"perfectum\"}");

        status = gesta_res_status(m, id_o, piscina);
        CREDO_VERUM (status.mensura == (i32)strlen("perfectum")
            && memcmp(status.datum, "perfectum",
                   strlen("perfectum")) == ZEPHYRUM);
        CREDO_VERUM (gesta_salutem_aestimare(m, id_o, piscina,
            &s));
        CREDO_VERUM (s.sanus);
        st = _status_entis(m, id_o, piscina);
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "violatio machinae"), ZEPHYRUM);

        /* adprobatio simplex: pendens -> perfectum recta */
        e.res_id = NIHIL;
        e.genus_eventus = "creatio";
        e.datum = "{\"genus\":\"opus\",\"titulus\":"
            "\"Adproba consilium\"}";
        e.actor = "fran";
        e.origo = "probatio";
        CREDO_VERUM (gesta_scribere(m, &e, id_o2));
        _scribe(m, id_o2, "status", "{\"novus\":\"perfectum\"}");
        st = _status_entis(m, id_o2, piscina);
        CREDO_VERUM (_clavis_est_chorda(st, "status",
            "perfectum"));
        CREDO_AEQUALIS_I32 (_notae_continentes(st,
            "violatio machinae"), ZEPHYRUM);
    }

    /* ========================================================
     * K3 CHUNK A - fascis atomicus + actiones (spec-v2 par VII,
     * aureae G1-G11 + G24). Mundus recens m4: annales puri, ergo
     * gesta_annales_verificare asserenda (mundus m eos iniectione
     * cruda VIII consulto fregit).
     * ======================================================== */
    {
        GestaMundus* m4;
        GestaActioFructus fr;
        character id_p[GESTA_RES_ID_MENSURA];
        character id_p2[GESTA_RES_ID_MENSURA];
        character id_p3[GESTA_RES_ID_MENSURA];
        character id_qq[GESTA_RES_ID_MENSURA];
        character id_dec[GESTA_RES_ID_MENSURA];
        character id_vin[GESTA_RES_ID_MENSURA];
        character prae[GESTA_RES_ID_MENSURA];
        character ligamina_json[CXXVIII];

        m4 = gesta_aperire(piscina, VIA_DB4, VIA_AN4);
        CREDO_NON_NIHIL (m4);
        si (m4 == NIHIL)
        {
            redde I;
        }

        /* genera fundamenta (semen v3 in probatione - parcum/
         * decretum/quaestio/vinculum + actiones) */
        _scribe(m4, NIHIL, "definitio-generis",
            "{\"titulus\":\"parcum\",\"status_initialis\":"
            "\"parcatum\",\"machina\":[[\"parcatum\",\"tractum\"],"
            "[\"tractum\",\"clausum\"],[\"parcatum\",\"clausum\"]],"
            "\"reducer\":\"ordinarius\"}");
        _scribe(m4, NIHIL, "definitio-generis",
            "{\"titulus\":\"decretum\",\"reducer\":\"ordinarius\"}");
        _scribe(m4, NIHIL, "definitio-generis",
            "{\"titulus\":\"quaestio\",\"status_initialis\":"
            "\"apertum\",\"machina\":[[\"apertum\",\"laborans\"],"
            "[\"laborans\",\"clausum\"],[\"apertum\",\"clausum\"]],"
            "\"reducer\":\"ordinarius\"}");
        _scribe(m4, NIHIL, "definitio-generis",
            "{\"titulus\":\"vinculum\",\"species\":\"nexus\","
            "\"partes\":[{\"titulus\":\"a\",\"genera_licita\":[],"
            "\"cardinalitas\":\"unicus\"},{\"titulus\":\"b\","
            "\"genera_licita\":[],\"cardinalitas\":\"unicus\"}],"
            "\"status_initialis\":\"vigens\",\"machina\":"
            "[[\"vigens\",\"solutum\"]],\"reducer\":"
            "\"ordinarius\"}");

        /* actio princeps - sex effectus (spec par IV: $novus bis,
         * obumbrae ter, vinculum = res nexus-speciei propria) */
        _scribe(m4, NIHIL, "definitio-generis",
            "{\"titulus\":\"claudere-cum-decreto\",\"species\":"
            "\"actio\",\"opes\":[{\"titulus\":\"parcum\","
            "\"genus\":\"parcum\",\"status_necessarius\":"
            "\"tractum\"}],\"argumenta\":[{\"titulus\":\"cur\","
            "\"typus\":\"textus\",\"necessarium\":true}],"
            "\"effectus\":["
            "{\"verbum\":\"creatio\",\"ut\":\"decretum_novum\","
            "\"datum\":{\"genus\":\"decretum\",\"titulus\":"
            "\"$arg.cur\"}},"
            "{\"verbum\":\"creatio\",\"ut\":\"vinculum\","
            "\"datum\":{\"genus\":\"vinculum\",\"verbum\":"
            "\"natum-de\"}},"
            "{\"verbum\":\"membrum-additum\",\"res\":"
            "\"$novus.vinculum\",\"datum\":{\"pars\":\"a\","
            "\"membrum\":\"$novus.decretum_novum\"}},"
            "{\"verbum\":\"membrum-additum\",\"res\":"
            "\"$novus.vinculum\",\"datum\":{\"pars\":\"b\","
            "\"membrum\":\"$res.parcum\"}},"
            "{\"verbum\":\"nota\",\"res\":\"$res.parcum\","
            "\"datum\":{\"textus\":\"clausum: $arg.cur\"}},"
            "{\"verbum\":\"status\",\"res\":\"$res.parcum\","
            "\"datum\":{\"novus\":\"clausum\"}}]}");

        /* actio substitutionis: typi servati + intextum + $nunc */
        _scribe(m4, NIHIL, "definitio-generis",
            "{\"titulus\":\"signare\",\"species\":\"actio\","
            "\"opes\":[{\"titulus\":\"quaestio\",\"genus\":"
            "\"quaestio\"}],\"argumenta\":[{\"titulus\":\"n\","
            "\"typus\":\"numerus\",\"necessarium\":true},"
            "{\"titulus\":\"verbum\",\"typus\":\"textus\"}],"
            "\"effectus\":[{\"verbum\":\"mutatio\",\"res\":"
            "\"$res.quaestio\",\"datum\":{\"gradus\":\"$arg.n\","
            "\"descriptio\":\"compositum: $arg.verbum et $arg.n\","
            "\"quando\":\"$nunc\"}}]}");

        /* ====================================================
         * XXXI. G1: error mechanicus medio fasce -> NIHIL scriptum
         * (nulli ordines, nullae lineae annalium, res prima abest)
         * ==================================================== */
        {
            GestaFascisEventum fasciculus[III];
            GestaEventum e;
            s64 seq_ante;
            long an_ante;

            e.res_id = NIHIL;
            e.genus_eventus = "creatio";
            e.datum = "{\"genus\":\"quaestio\",\"titulus\":"
                "\"Stans\"}";
            e.actor = "fran";
            e.origo = "probatio";
            CREDO_VERUM (gesta_scribere(m4, &e, id_qq));

            seq_ante = gesta_seq_ultima(m4);
            an_ante = _mensura_plagulae(VIA_AN4);
            scrinium_ulid(prae);
            fasciculus[ZEPHYRUM].event_id = NIHIL;
            fasciculus[ZEPHYRUM].eventum.res_id = prae;
            fasciculus[ZEPHYRUM].eventum.genus_eventus = "creatio";
            fasciculus[ZEPHYRUM].eventum.datum =
                "{\"genus\":\"quaestio\",\"titulus\":\"Prima\"}";
            fasciculus[ZEPHYRUM].eventum.actor = "fran";
            fasciculus[ZEPHYRUM].eventum.origo = "probatio";
            /* creatio duplicata = error mechanicus */
            fasciculus[I].event_id = NIHIL;
            fasciculus[I].eventum.res_id = id_qq;
            fasciculus[I].eventum.genus_eventus = "creatio";
            fasciculus[I].eventum.datum =
                "{\"titulus\":\"Duplicata\"}";
            fasciculus[I].eventum.actor = "fran";
            fasciculus[I].eventum.origo = "probatio";
            fasciculus[II].event_id = NIHIL;
            fasciculus[II].eventum.res_id = id_qq;
            fasciculus[II].eventum.genus_eventus = "nota";
            fasciculus[II].eventum.datum =
                "{\"textus\":\"numquam\"}";
            fasciculus[II].eventum.actor = "fran";
            fasciculus[II].eventum.origo = "probatio";

            CREDO_VERUM (!gesta_fascis_scribere(m4, fasciculus,
                III, NIHIL));
            CREDO_VERUM (gesta_seq_ultima(m4) == seq_ante);
            CREDO_VERUM (_mensura_plagulae(VIA_AN4) == an_ante);
            {
                chorda d = gesta_res_datum(m4, prae, piscina);

                CREDO_AEQUALIS_I32 (d.mensura, ZEPHYRUM);
            }
        }

        /* ====================================================
         * XXXII. G2+G4: fascis felix - N eventus, N lineae
         * annalium ordine (verificare TENET), res_ids_out,
         * plicatura semel post, creatum UNUM trans fascem
         * ==================================================== */
        {
            GestaFascisEventum fasciculus[III];
            character ids[III * GESTA_RES_ID_MENSURA];
            s64 seq_ante = gesta_seq_ultima(m4);
            JsonValor* st;

            scrinium_ulid(prae);
            fasciculus[ZEPHYRUM].event_id = NIHIL;
            fasciculus[ZEPHYRUM].eventum.res_id = prae;
            fasciculus[ZEPHYRUM].eventum.genus_eventus = "creatio";
            fasciculus[ZEPHYRUM].eventum.datum =
                "{\"genus\":\"quaestio\",\"titulus\":\"Fascis\"}";
            fasciculus[ZEPHYRUM].eventum.actor = "fran";
            fasciculus[ZEPHYRUM].eventum.origo = "probatio";
            fasciculus[I].event_id = NIHIL;
            fasciculus[I].eventum.res_id = prae;
            fasciculus[I].eventum.genus_eventus = "mutatio";
            fasciculus[I].eventum.datum = "{\"k\":\"1\"}";
            fasciculus[I].eventum.actor = "fran";
            fasciculus[I].eventum.origo = "probatio";
            fasciculus[II].event_id = NIHIL;
            fasciculus[II].eventum.res_id = prae;
            fasciculus[II].eventum.genus_eventus = "nota";
            fasciculus[II].eventum.datum =
                "{\"textus\":\"intra fascem\"}";
            fasciculus[II].eventum.actor = "fran";
            fasciculus[II].eventum.origo = "probatio";

            CREDO_VERUM (gesta_fascis_scribere(m4, fasciculus, III,
                ids));
            CREDO_VERUM (gesta_seq_ultima(m4) == seq_ante + III);
            CREDO_VERUM (strcmp(ids, prae) == ZEPHYRUM);
            CREDO_VERUM (strcmp(ids + GESTA_RES_ID_MENSURA, prae)
                == ZEPHYRUM);
            st = _status_entis(m4, prae, piscina);
            CREDO_NON_NIHIL (st);
            CREDO_VERUM (_clavis_est_chorda(st, "k", "1"));
            CREDO_AEQUALIS_I32 (_notae_continentes(st,
                "intra fascem"), I);
            CREDO_VERUM (gesta_annales_verificare(m4));
            /* G4: creatum idem in omnibus ordinibus fascis */
            CREDO_VERUM (_numerus_sql_s(m4,
                "SELECT COUNT(DISTINCT creatum) FROM tessellae"
                " WHERE seq > ?1", seq_ante) == (s64)I);
        }

        /* ====================================================
         * XXXIII. G3: violatio machinae intra fascem -> eventus
         * cadit + nota custodiae ADIACENS eadem transactione;
         * status contra obumbram iudicatus (status_initialis)
         * ==================================================== */
        {
            GestaFascisEventum fasciculus[II];
            s64 seq_ante = gesta_seq_ultima(m4);
            JsonValor* st;

            scrinium_ulid(prae);
            fasciculus[ZEPHYRUM].event_id = NIHIL;
            fasciculus[ZEPHYRUM].eventum.res_id = prae;
            fasciculus[ZEPHYRUM].eventum.genus_eventus = "creatio";
            fasciculus[ZEPHYRUM].eventum.datum =
                "{\"genus\":\"quaestio\",\"titulus\":\"Violata\"}";
            fasciculus[ZEPHYRUM].eventum.actor = "fran";
            fasciculus[ZEPHYRUM].eventum.origo = "probatio";
            /* apertum (initialis ex obumbra) -> alienum: extra
             * machinam */
            fasciculus[I].event_id = NIHIL;
            fasciculus[I].eventum.res_id = prae;
            fasciculus[I].eventum.genus_eventus = "status";
            fasciculus[I].eventum.datum =
                "{\"novus\":\"alienum\"}";
            fasciculus[I].eventum.actor = "fran";
            fasciculus[I].eventum.origo = "probatio";

            CREDO_VERUM (gesta_fascis_scribere(m4, fasciculus, II,
                NIHIL));
            /* II eventus + nota custodiae = III */
            CREDO_VERUM (gesta_seq_ultima(m4) == seq_ante + III);
            CREDO_VERUM (_numerus_sql_s(m4,
                "SELECT COUNT(*) FROM tessellae WHERE seq = ?1"
                " AND genus_eventus = 'nota'"
                " AND actor = 'machina'",
                seq_ante + III) == (s64)I);
            st = _status_entis(m4, prae, piscina);
            CREDO_VERUM (_clavis_est_chorda(st, "status",
                "alienum"));
            CREDO_AEQUALIS_I32 (_notae_continentes(st,
                "violatio machinae"), I);
            CREDO_VERUM (gesta_annales_verificare(m4));
        }

        /* ====================================================
         * XXXIV. G5: porta OBSTAT - recusationes sex, quaeque
         * actio-recusata una + effectus NULLI; errores mechanici
         * (verbum ignotum, $novus antrorsum) idem; actio ignota /
         * genus non actio = apparatus (FALSUM)
         * ==================================================== */
        {
            GestaEventum e;
            s64 seq_ante;

            e.res_id = NIHIL;
            e.genus_eventus = "creatio";
            e.datum = "{\"genus\":\"parcum\",\"titulus\":"
                "\"Aedificatio K3\"}";
            e.actor = "fran";
            e.origo = "probatio";
            CREDO_VERUM (gesta_scribere(m4, &e, id_p));
            _scribe(m4, id_p, "status", "{\"novus\":\"tractum\"}");
            e.res_id = NIHIL;
            e.datum = "{\"genus\":\"parcum\",\"titulus\":\"Sero\"}";
            CREDO_VERUM (gesta_scribere(m4, &e, id_p2));

            /* 1. ligamen absens */
            seq_ante = gesta_seq_ultima(m4);
            CREDO_VERUM (gesta_agere(m4, "claudere-cum-decreto",
                "{}", "{\"cur\":\"x\"}", "fran", piscina, &fr));
            CREDO_VERUM (!fr.facta);
            CREDO_VERUM (strstr(fr.causa, "ligamen") != NIHIL);
            CREDO_VERUM (gesta_seq_ultima(m4) == seq_ante + I);

            /* 2. ops inexistens */
            CREDO_VERUM (gesta_agere(m4, "claudere-cum-decreto",
                "{\"parcum\":\"PHANTASMA00000000000000000\"}",
                "{\"cur\":\"x\"}", "fran", piscina, &fr));
            CREDO_VERUM (!fr.facta
                && strstr(fr.causa, "inexistens") != NIHIL);

            /* 3. genus opis non congruit */
            sprintf(ligamina_json, "{\"parcum\":\"%s\"}", id_qq);
            CREDO_VERUM (gesta_agere(m4, "claudere-cum-decreto",
                ligamina_json, "{\"cur\":\"x\"}", "fran", piscina, &fr));
            CREDO_VERUM (!fr.facta
                && strstr(fr.causa, "genus opis") != NIHIL);

            /* 4. status opis non congruit (parcatum, non tractum) */
            sprintf(ligamina_json, "{\"parcum\":\"%s\"}", id_p2);
            CREDO_VERUM (gesta_agere(m4, "claudere-cum-decreto",
                ligamina_json, "{\"cur\":\"x\"}", "fran", piscina, &fr));
            CREDO_VERUM (!fr.facta
                && strstr(fr.causa, "status opis") != NIHIL);

            /* 5. argumentum necessarium abest */
            sprintf(ligamina_json, "{\"parcum\":\"%s\"}", id_p);
            CREDO_VERUM (gesta_agere(m4, "claudere-cum-decreto",
                ligamina_json, "{}", "fran", piscina, &fr));
            CREDO_VERUM (!fr.facta
                && strstr(fr.causa, "necessarium") != NIHIL);

            /* 6. argumentum typum violat */
            CREDO_VERUM (gesta_agere(m4, "claudere-cum-decreto",
                ligamina_json, "{\"cur\":42}", "fran", piscina, &fr));
            CREDO_VERUM (!fr.facta
                && strstr(fr.causa, "typum") != NIHIL);

            /* effectus NULLI facti */
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m4, id_p,
                piscina), "tractum"));
            CREDO_VERUM (_numerus_sql_t(m4,
                "SELECT COUNT(*) FROM res WHERE genus = ?1",
                "decretum", piscina) == (s64)ZEPHYRUM);
            CREDO_VERUM (_numerus_sql_t(m4,
                "SELECT COUNT(*) FROM tessellae WHERE res_id = ?1"
                " AND genus_eventus = 'actio-recusata'",
                "claudere-cum-decreto", piscina) == (s64)VI);
        }
        /* errores mechanici: recusata, nihil praeterea */
        _scribe(m4, NIHIL, "definitio-generis",
            "{\"titulus\":\"explodere\",\"species\":\"actio\","
            "\"opes\":[],\"argumenta\":[],\"effectus\":["
            "{\"verbum\":\"detonare\",\"res\":\"x\","
            "\"datum\":{}}]}");
        _scribe(m4, NIHIL, "definitio-generis",
            "{\"titulus\":\"antrorsum\",\"species\":\"actio\","
            "\"opes\":[],\"argumenta\":[],\"effectus\":["
            "{\"verbum\":\"nota\",\"res\":\"$novus.cras\","
            "\"datum\":{\"textus\":\"x\"}},"
            "{\"verbum\":\"creatio\",\"ut\":\"cras\","
            "\"datum\":{\"genus\":\"decretum\"}}]}");
        {
            s64 seq_ante = gesta_seq_ultima(m4);

            CREDO_VERUM (gesta_agere(m4, "explodere", "{}", "{}",
                "fran", piscina, &fr));
            CREDO_VERUM (!fr.facta
                && strstr(fr.causa, "verbum ignotum") != NIHIL);
            /* referentia antrorsum: $novus ANTE creationem suam */
            CREDO_VERUM (gesta_agere(m4, "antrorsum", "{}", "{}",
                "fran", piscina, &fr));
            CREDO_VERUM (!fr.facta
                && strstr(fr.causa, "$novus.cras") != NIHIL);
            CREDO_VERUM (gesta_seq_ultima(m4) == seq_ante + II);
            CREDO_VERUM (_numerus_sql_t(m4,
                "SELECT COUNT(*) FROM res WHERE genus = ?1",
                "decretum", piscina) == (s64)ZEPHYRUM);
        }
        /* actio ignota / genus non actio = apparatus, non
         * recusata */
        CREDO_VERUM (!gesta_agere(m4, "nusquam", "{}", "{}",
            "fran", piscina, &fr));
        CREDO_VERUM (!gesta_agere(m4, "parcum", "{}", "{}",
            "fran", piscina, &fr));

        /* ====================================================
         * XXXV. G6+G7: receptum princeps - sex effectus atomice,
         * $novus bis + obumbrae ter SINE querela custodiae,
         * actio-facta ids prae-cusos VI omnes nominat
         * ==================================================== */
        {
            s64 seq_ante = gesta_seq_ultima(m4);
            JsonValor* st;

            sprintf(ligamina_json, "{\"parcum\":\"%s\"}", id_p);
            CREDO_VERUM (gesta_agere(m4, "claudere-cum-decreto",
                ligamina_json,
                "{\"cur\":\"K3 chunk A perfectum\"}", "fran", piscina,
                &fr));
            CREDO_VERUM (fr.facta);
            CREDO_AEQUALIS_I32 (fr.novae_numerus, II);
            /* VI effectus + actio-facta, NULLA nota custodiae */
            CREDO_VERUM (gesta_seq_ultima(m4) == seq_ante + VII);

            _res_id_figere(fr.res_novae[ZEPHYRUM], id_dec);
            _res_id_figere(fr.res_novae[I], id_vin);

            /* decretum: genus + titulus ex $arg.cur */
            CREDO_VERUM (_chorda_aequa(_genus_rei(m4, id_dec,
                piscina), "decretum"));
            st = _status_entis(m4, id_dec, piscina);
            CREDO_NON_NIHIL (st);
            CREDO_VERUM (_clavis_est_chorda(st, "titulus",
                "K3 chunk A perfectum"));

            /* vinculum: membra ambo in indice, querela nulla (G7 -
             * obumbrae rem modo creatam visibilem fecerunt) */
            CREDO_VERUM (_chorda_aequa(_genus_rei(m4, id_vin,
                piscina), "vinculum"));
            CREDO_VERUM (_membra_numerus(m4, id_vin, "a", piscina)
                == (s64)I);
            CREDO_VERUM (_membra_numerus(m4, id_vin, "b", piscina)
                == (s64)I);
            st = _status_entis(m4, id_vin, piscina);
            CREDO_AEQUALIS_I32 (_notae_continentes(st, "violatio"),
                ZEPHYRUM);

            /* parcum: nota substituta + status clausum */
            st = _status_entis(m4, id_p, piscina);
            CREDO_VERUM (_clavis_est_chorda(st, "status",
                "clausum"));
            CREDO_AEQUALIS_I32 (_notae_continentes(st,
                "clausum: K3 chunk A perfectum"), I);
            CREDO_AEQUALIS_I32 (_notae_continentes(st, "violatio"),
                ZEPHYRUM);

            /* G6: actio-facta una, eventus VI, omnes in actis */
            {
                ScriniumEnuntiatum* ef = scrinium_praeparare(
                    gesta_scrinium(m4),
                    "SELECT datum FROM tessellae WHERE res_id = ?"
                    " AND genus_eventus = 'actio-facta'");
                chorda d;
                JsonResultus r;
                JsonValor* ev;
                i32 i;

                CREDO_NON_NIHIL (ef);
                scrinium_ligare_textum(ef, I, chorda_ex_literis(
                    "claudere-cum-decreto", piscina));
                CREDO_VERUM (scrinium_gradi(ef) == SCRINIUM_ORDO);
                d = scrinium_columna_textus(ef, 0, piscina);
                CREDO_VERUM (scrinium_gradi(ef) != SCRINIUM_ORDO);
                scrinium_finire(ef);
                r = json_legere(d, piscina);
                CREDO_VERUM (r.successus);
                ev = json_objectum_capere(r.radix, "eventus");
                CREDO_NON_NIHIL (ev);
                CREDO_AEQUALIS_I32 (
                    (i32)json_tabulatum_numerus(ev), VI);
                per (i = ZEPHYRUM;
                     i < json_tabulatum_numerus(ev); i++)
                {
                    character id_ev[GESTA_RES_ID_MENSURA];

                    _res_id_figere(json_ad_chorda(
                        json_tabulatum_obtinere(ev, i)), id_ev);
                    CREDO_VERUM (_numerus_sql_t(m4,
                        "SELECT COUNT(*) FROM tessellae"
                        " WHERE id = ?1", id_ev, piscina)
                        == (s64)I);
                }
            }
            CREDO_VERUM (gesta_annales_verificare(m4));
        }

        /* ====================================================
         * XXXVI. Substitutio: signum solum typum servat (42 manet
         * numerus), intextum coercit, $nunc impletur
         * ==================================================== */
        {
            JsonValor* st;
            JsonValor* v;

            sprintf(ligamina_json, "{\"quaestio\":\"%s\"}", id_qq);
            CREDO_VERUM (gesta_agere(m4, "signare", ligamina_json,
                "{\"n\":42,\"verbum\":\"alta\"}", "fran", piscina, &fr));
            CREDO_VERUM (fr.facta);
            CREDO_AEQUALIS_I32 (fr.novae_numerus, ZEPHYRUM);
            st = _status_entis(m4, id_qq, piscina);
            CREDO_NON_NIHIL (st);
            v = json_objectum_capere(st, "gradus");
            CREDO_NON_NIHIL (v);
            CREDO_VERUM (json_est_integer(v));
            CREDO_VERUM (json_ad_integer(v) == (s64)XLII);
            CREDO_VERUM (_clavis_est_chorda(st, "descriptio",
                "compositum: alta et 42"));
            v = json_objectum_capere(st, "quando");
            CREDO_NON_NIHIL (v);
            CREDO_VERUM (json_est_chorda(v)
                && json_ad_chorda(v).mensura > ZEPHYRUM);
        }

        /* ====================================================
         * XXXVII. G8: violatio machinae in effectu SCRIBIT (nota
         * custodiae) et fascis perficitur (decisio 2)
         * ==================================================== */
        _scribe(m4, NIHIL, "definitio-generis",
            "{\"titulus\":\"praecipitare\",\"species\":\"actio\","
            "\"opes\":[{\"titulus\":\"q\",\"genus\":\"quaestio\"}],"
            "\"argumenta\":[],\"effectus\":["
            "{\"verbum\":\"status\",\"res\":\"$res.q\","
            "\"datum\":{\"novus\":\"laborans\"}},"
            "{\"verbum\":\"nota\",\"res\":\"$res.q\","
            "\"datum\":{\"textus\":\"post violationem\"}}]}");
        {
            GestaEventum e;
            character id_qv[GESTA_RES_ID_MENSURA];
            JsonValor* st;

            e.res_id = NIHIL;
            e.genus_eventus = "creatio";
            e.datum = "{\"genus\":\"quaestio\",\"titulus\":"
                "\"Violanda\"}";
            e.actor = "fran";
            e.origo = "probatio";
            CREDO_VERUM (gesta_scribere(m4, &e, id_qv));
            _scribe(m4, id_qv, "status", "{\"novus\":\"clausum\"}");

            sprintf(ligamina_json, "{\"q\":\"%s\"}", id_qv);
            CREDO_VERUM (gesta_agere(m4, "praecipitare",
                ligamina_json, "{}", "fran", piscina, &fr));
            CREDO_VERUM (fr.facta);
            st = _status_entis(m4, id_qv, piscina);
            /* clausum -> laborans extra machinam: cadit TAMEN */
            CREDO_VERUM (_clavis_est_chorda(st, "status",
                "laborans"));
            CREDO_AEQUALIS_I32 (_notae_continentes(st,
                "violatio machinae"), I);
            /* effectus sequens quoque factus - fascis integer */
            CREDO_AEQUALIS_I32 (_notae_continentes(st,
                "post violationem"), I);
        }

        /* ====================================================
         * XXXVIII. G10: affordantiae per genus + statum
         * ==================================================== */
        {
            GestaEventum e;
            Xar* act;

            e.res_id = NIHIL;
            e.genus_eventus = "creatio";
            e.datum = "{\"genus\":\"parcum\",\"titulus\":"
                "\"Alterum\"}";
            e.actor = "fran";
            e.origo = "probatio";
            CREDO_VERUM (gesta_scribere(m4, &e, id_p3));
            _scribe(m4, id_p3, "status", "{\"novus\":\"tractum\"}");

            act = gesta_actiones_rei(m4, id_p3, piscina);
            CREDO_NON_NIHIL (act);
            CREDO_VERUM (_actiones_continent(act,
                "claudere-cum-decreto"));
            CREDO_VERUM (!_actiones_continent(act, "signare"));
            /* parcatum: status_necessarius non congruit */
            act = gesta_actiones_rei(m4, id_p2, piscina);
            CREDO_VERUM (!_actiones_continent(act,
                "claudere-cum-decreto"));
            /* quaestio: signare (sine statu necessario) */
            act = gesta_actiones_rei(m4, id_qq, piscina);
            CREDO_VERUM (_actiones_continent(act, "signare"));
            CREDO_VERUM (!_actiones_continent(act,
                "claudere-cum-decreto"));
        }

        /* ====================================================
         * XXXIX. G11: emendatio-generis in actione = correctio in
         * loco - genus UNUM manet (verruca checkout_v4 mortua)
         * ==================================================== */
        _scribe(m4, NIHIL, "emendatio-generis",
            "{\"titulus\":\"signare\",\"species\":\"actio\","
            "\"opes\":[{\"titulus\":\"quaestio\",\"genus\":"
            "\"quaestio\"}],\"argumenta\":[{\"titulus\":\"n\","
            "\"typus\":\"numerus\",\"necessarium\":true},"
            "{\"titulus\":\"verbum\",\"typus\":\"textus\"}],"
            "\"effectus\":[{\"verbum\":\"mutatio\",\"res\":"
            "\"$res.quaestio\",\"datum\":{\"descriptio\":"
            "\"recensitum: $arg.verbum\"}}]}");
        {
            JsonValor* st;

            sprintf(ligamina_json, "{\"quaestio\":\"%s\"}", id_qq);
            CREDO_VERUM (gesta_agere(m4, "signare", ligamina_json,
                "{\"n\":7,\"verbum\":\"corr\"}", "fran", piscina, &fr));
            CREDO_VERUM (fr.facta);
            st = _status_entis(m4, id_qq, piscina);
            CREDO_VERUM (_clavis_est_chorda(st, "descriptio",
                "recensitum: corr"));
            CREDO_VERUM (_numerus_sql_t(m4,
                "SELECT COUNT(*) FROM genera WHERE titulus = ?1",
                "signare", piscina) == (s64)I);
        }

        /* ====================================================
         * XL. G24: flumen facta crescit - ordo rei actionis
         * NUMQUAM materializatur, FTS vacua manet, sordidae
         * exhauriuntur (E2 B5)
         * ==================================================== */
        {
            i32 k;

            per (k = ZEPHYRUM; k < X; k++)
            {
                sprintf(ligamina_json, "{\"quaestio\":\"%s\"}",
                    id_qq);
                CREDO_VERUM (gesta_agere(m4, "signare",
                    ligamina_json,
                    "{\"n\":7,\"verbum\":\"iterum\"}", "fran", piscina,
                    &fr));
                CREDO_VERUM (fr.facta);
            }
            CREDO_VERUM (_numerus_sql_t(m4,
                "SELECT COUNT(*) FROM tessellae WHERE res_id = ?1"
                " AND genus_eventus = 'actio-facta'",
                "signare", piscina) >= (s64)XI);
            CREDO_VERUM (_numerus_sql_t(m4,
                "SELECT COUNT(*) FROM res WHERE res_id = ?1",
                "signare", piscina) == (s64)ZEPHYRUM);
            CREDO_VERUM (gesta_fts_exhaurire(m4));
            CREDO_VERUM (_numerus_sql_t(m4,
                "SELECT COUNT(*) FROM res_fts WHERE res_id = ?1",
                "signare", piscina) == (s64)ZEPHYRUM);
            CREDO_VERUM (_numerus_sql_t(m4,
                "SELECT COUNT(*) FROM sordidae", NIHIL, piscina)
                == (s64)ZEPHYRUM);
        }

        /* ====================================================
         * XLI. Replicatio cum eventibus actionum: acta = data
         * (nullus motor re-ignitur; barra K1 tenet)
         * ==================================================== */
        {
            chorda ante = gesta_res_datum(m4, id_p, piscina);
            chorda post;

            CREDO_VERUM (gesta_replicare(m4));
            post = gesta_res_datum(m4, id_p, piscina);
            CREDO_VERUM (ante.mensura == post.mensura
                && memcmp(ante.datum, post.datum,
                       (memoriae_index)ante.mensura) == ZEPHYRUM);
            CREDO_VERUM (_membra_numerus(m4, id_vin, "a", piscina)
                == (s64)I);
        }

        gesta_claudere(m4);
    }

    /* ========================================================
     * K3 CHUNK B - processus + provectio (spec-v2 par V, aureae
     * G12-G19). Mundus recens m5.
     * ======================================================== */
    {
        GestaMundus* m5;
        GestaProcessusFructus pf;
        GestaActioFructus fr;
        character id_i[GESTA_RES_ID_MENSURA];
        character id_i2[GESTA_RES_ID_MENSURA];
        character id_g[GESTA_RES_ID_MENSURA];
        character id_ac[GESTA_RES_ID_MENSURA];
        character id_fr[GESTA_RES_ID_MENSURA];
        character op1[GESTA_RES_ID_MENSURA];
        character op2[GESTA_RES_ID_MENSURA];
        character id_pp[GESTA_RES_ID_MENSURA];
        character ligamina_json[CXXVIII];

        m5 = gesta_aperire(piscina, VIA_DB5, VIA_AN5);
        CREDO_NON_NIHIL (m5);
        si (m5 == NIHIL)
        {
            redde I;
        }

        /* genera fundamenta (opus ex XXX; cetera ex frusto A) */
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"opus\",\"status_initialis\":"
            "\"pendens\",\"machina\":[[\"pendens\",\"susceptum\"],"
            "[\"susceptum\",\"perfectum\"],"
            "[\"pendens\",\"perfectum\"],[\"pendens\",\"omissum\"],"
            "[\"susceptum\",\"omissum\"]],\"reducer\":"
            "\"ordinarius\"}");
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"parcum\",\"status_initialis\":"
            "\"parcatum\",\"machina\":[[\"parcatum\",\"tractum\"],"
            "[\"tractum\",\"clausum\"],[\"parcatum\",\"clausum\"]],"
            "\"reducer\":\"ordinarius\"}");
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"decretum\",\"reducer\":"
            "\"ordinarius\"}");
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"quaestio\",\"status_initialis\":"
            "\"apertum\",\"machina\":[[\"apertum\",\"laborans\"],"
            "[\"laborans\",\"clausum\"],[\"apertum\",\"clausum\"]],"
            "\"reducer\":\"ordinarius\"}");
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"vinculum\",\"species\":\"nexus\","
            "\"partes\":[{\"titulus\":\"a\",\"genera_licita\":[],"
            "\"cardinalitas\":\"unicus\"},{\"titulus\":\"b\","
            "\"genera_licita\":[],\"cardinalitas\":\"unicus\"}],"
            "\"status_initialis\":\"vigens\",\"machina\":"
            "[[\"vigens\",\"solutum\"]],\"reducer\":"
            "\"ordinarius\"}");
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"claudere-cum-decreto\",\"species\":"
            "\"actio\",\"opes\":[{\"titulus\":\"parcum\","
            "\"genus\":\"parcum\",\"status_necessarius\":"
            "\"tractum\"}],\"argumenta\":[{\"titulus\":\"cur\","
            "\"typus\":\"textus\",\"necessarium\":true}],"
            "\"effectus\":["
            "{\"verbum\":\"creatio\",\"ut\":\"decretum_novum\","
            "\"datum\":{\"genus\":\"decretum\",\"titulus\":"
            "\"$arg.cur\"}},"
            "{\"verbum\":\"creatio\",\"ut\":\"vinculum\","
            "\"datum\":{\"genus\":\"vinculum\",\"verbum\":"
            "\"natum-de\"}},"
            "{\"verbum\":\"membrum-additum\",\"res\":"
            "\"$novus.vinculum\",\"datum\":{\"pars\":\"a\","
            "\"membrum\":\"$novus.decretum_novum\"}},"
            "{\"verbum\":\"membrum-additum\",\"res\":"
            "\"$novus.vinculum\",\"datum\":{\"pars\":\"b\","
            "\"membrum\":\"$res.parcum\"}},"
            "{\"verbum\":\"nota\",\"res\":\"$res.parcum\","
            "\"datum\":{\"textus\":\"clausum: $arg.cur\"}},"
            "{\"verbum\":\"status\",\"res\":\"$res.parcum\","
            "\"datum\":{\"novus\":\"clausum\"}}]}");
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"antrorsum\",\"species\":\"actio\","
            "\"opes\":[],\"argumenta\":[],\"effectus\":["
            "{\"verbum\":\"nota\",\"res\":\"$novus.cras\","
            "\"datum\":{\"textus\":\"x\"}},"
            "{\"verbum\":\"creatio\",\"ut\":\"cras\","
            "\"datum\":{\"genus\":\"decretum\"}}]}");

        /* processus */
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"cursus-simplex\",\"species\":"
            "\"processus\",\"status_initialis\":\"currens\","
            "\"machina\":[[\"currens\",\"perfectus\"],"
            "[\"currens\",\"defectus\"],[\"currens\","
            "\"omissus\"]],\"opes\":[],\"argumenta\":[],"
            "\"ordines\":[\"unus\"],\"gradus\":["
            "{\"titulus\":\"primum\",\"ordo\":\"unus\","
            "\"genus_gradus\":\"opus\",\"positio\":0},"
            "{\"titulus\":\"secundum\",\"ordo\":\"unus\","
            "\"genus_gradus\":\"opus\",\"positio\":1,"
            "\"descriptio\":\"opus alterum\"}]}");
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"cursus-geminus\",\"species\":"
            "\"processus\",\"status_initialis\":\"currens\","
            "\"machina\":[[\"currens\",\"perfectus\"],"
            "[\"currens\",\"defectus\"],[\"currens\","
            "\"omissus\"]],\"ordines\":[\"a\",\"b\"],\"gradus\":["
            "{\"titulus\":\"para\",\"ordo\":\"a\","
            "\"genus_gradus\":\"opus\",\"positio\":0},"
            "{\"titulus\":\"proba\",\"ordo\":\"b\","
            "\"genus_gradus\":\"opus\",\"positio\":0},"
            "{\"titulus\":\"porta-ambo\",\"ordo\":\"b\","
            "\"genus_gradus\":\"porta\",\"positio\":1,"
            "\"condiciones\":[\"para\",\"proba\"]},"
            "{\"titulus\":\"fini\",\"ordo\":\"b\","
            "\"genus_gradus\":\"opus\",\"positio\":2}]}");
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"cursus-actionis\",\"species\":"
            "\"processus\",\"status_initialis\":\"currens\","
            "\"machina\":[[\"currens\",\"perfectus\"],"
            "[\"currens\",\"defectus\"],[\"currens\","
            "\"omissus\"]],\"opes\":[{\"titulus\":\"propositum\","
            "\"genus\":\"parcum\"}],\"argumenta\":["
            "{\"titulus\":\"cur_sigilli\",\"typus\":\"textus\","
            "\"necessarium\":true}],\"ordines\":[\"unus\"],"
            "\"gradus\":[{\"titulus\":\"claudere\",\"ordo\":"
            "\"unus\",\"genus_gradus\":\"actio\",\"positio\":0,"
            "\"actio\":\"claudere-cum-decreto\",\"ligamina\":"
            "{\"parcum\":\"$ops.propositum\"},\"argumenta\":"
            "{\"cur\":\"sigillum: $arg.cur_sigilli\"}}]}");
        _scribe(m5, NIHIL, "definitio-generis",
            "{\"titulus\":\"cursus-fractus\",\"species\":"
            "\"processus\",\"status_initialis\":\"currens\","
            "\"machina\":[[\"currens\",\"perfectus\"],"
            "[\"currens\",\"defectus\"],[\"currens\","
            "\"omissus\"]],\"ordines\":[\"unus\"],\"gradus\":["
            "{\"titulus\":\"rumpe\",\"ordo\":\"unus\","
            "\"genus_gradus\":\"actio\",\"positio\":0,"
            "\"actio\":\"antrorsum\"}]}");

        /* ====================================================
         * XLII. Instantiatio: photographia, generatio prima,
         * ligamina membra SINE custodia (G18), porta processūs,
         * species-fines apparatus
         * ==================================================== */
        {
            JsonValor* st;

            CREDO_VERUM (gesta_processum_incipere(m5,
                "cursus-simplex", "{}", "{}", "fran", piscina, &pf));
            CREDO_VERUM (pf.facta);
            memcpy(id_i, pf.instantia, GESTA_RES_ID_MENSURA);

            CREDO_VERUM (_chorda_aequa(_genus_rei(m5, id_i,
                piscina), "cursus-simplex"));
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, id_i,
                piscina), "currens"));
            /* primum generatum, secundum nondum */
            CREDO_VERUM (_cursus_est(m5, id_i, "primum",
                "incohatus", piscina));
            CREDO_VERUM (_cursus_est(m5, id_i, "secundum", NIHIL,
                piscina));
            CREDO_VERUM (_opus_gradus(m5, id_i, "primum", op1,
                piscina));
            CREDO_VERUM (_chorda_aequa(_genus_rei(m5, op1,
                piscina), "opus"));
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, op1,
                piscina), "pendens"));
            CREDO_VERUM (_membra_numerus(m5, id_i, "primum",
                piscina) == (s64)I);
            /* G18: vinculum instantia->opus sine querela */
            st = _status_entis(m5, id_i, piscina);
            CREDO_AEQUALIS_I32 (_notae_continentes(st,
                "violatio"), ZEPHYRUM);
            CREDO_VERUM (gesta_annales_verificare(m5));

            /* porta processūs: ligamen absens -> recusatus */
            CREDO_VERUM (gesta_processum_incipere(m5,
                "cursus-actionis", "{}",
                "{\"cur_sigilli\":\"x\"}", "fran", piscina, &pf));
            CREDO_VERUM (!pf.facta);
            CREDO_VERUM (strstr(pf.causa, "ligamen") != NIHIL);
            CREDO_VERUM (_eventus_rei(m5, "cursus-actionis",
                "processus-recusatus", piscina) == (s64)I);

            /* species-fines: actio non processus, processus non
             * actio, ignotum */
            CREDO_VERUM (!gesta_processum_incipere(m5,
                "claudere-cum-decreto", "{}", "{}", "fran", piscina,
                &pf));
            CREDO_VERUM (!gesta_agere(m5, "cursus-simplex", "{}",
                "{}", "fran", piscina, &fr));
            CREDO_VERUM (!gesta_processum_incipere(m5, "nusquam",
                "{}", "{}", "fran", piscina, &pf));
        }

        /* ====================================================
         * XLIII. G15: status operis provehit (etiam rectus
         * pendens->perfectum); scripturae non-membrorum sondam
         * negativam habent; effectus operis in gradus-perfectus
         * ==================================================== */
        {
            GestaEventum e;
            character id_x[GESTA_RES_ID_MENSURA];
            s64 eventa_ante;

            /* res aliena: status eius instantiam non tangit */
            e.res_id = NIHIL;
            e.genus_eventus = "creatio";
            e.datum = "{\"genus\":\"quaestio\",\"titulus\":"
                "\"Aliena\"}";
            e.actor = "fran";
            e.origo = "probatio";
            CREDO_VERUM (gesta_scribere(m5, &e, id_x));
            eventa_ante = _numerus_sql_t(m5,
                "SELECT COUNT(*) FROM tessellae WHERE res_id"
                " = ?1", id_i, piscina);
            _scribe(m5, id_x, "status",
                "{\"novus\":\"laborans\"}");
            CREDO_VERUM (_numerus_sql_t(m5,
                "SELECT COUNT(*) FROM tessellae WHERE res_id"
                " = ?1", id_i, piscina) == eventa_ante);

            /* perfectio operis primi (rectus pendens->perfectum)
             * -> gradus perfectus + secundum generatum */
            _scribe(m5, op1, "status",
                "{\"novus\":\"perfectum\"}");
            CREDO_VERUM (_cursus_est(m5, id_i, "primum",
                "perfectus", piscina));
            CREDO_VERUM (_cursus_est(m5, id_i, "secundum",
                "incohatus", piscina));
            CREDO_VERUM (_opus_gradus(m5, id_i, "secundum", op2,
                piscina));
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, id_i,
                piscina), "currens"));

            /* effectus operis in eventum gradus-perfectus fluit */
            _scribe(m5, op2, "mutatio",
                "{\"effectus\":\"confectum probatione\"}");
            _scribe(m5, op2, "status", "{\"novus\":\"susceptum\"}");
            _scribe(m5, op2, "status",
                "{\"novus\":\"perfectum\"}");
            CREDO_VERUM (_numerus_sql_t(m5,
                "SELECT COUNT(*) FROM tessellae WHERE res_id = ?1"
                " AND genus_eventus = 'gradus-perfectus'"
                " AND datum LIKE '%confectum%'", id_i, piscina)
                == (s64)I);

            /* omnes perfecti -> processus perfectus */
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, id_i,
                piscina), "perfectus"));
            CREDO_VERUM (_eventus_rei(m5, id_i,
                "processus-perfectus", piscina) == (s64)I);
            CREDO_VERUM (gesta_annales_verificare(m5));
        }

        /* ====================================================
         * XLIV. G12: photographia - emendatio generis instantiam
         * currentem NON tangit
         * ==================================================== */
        {
            character op_b[GESTA_RES_ID_MENSURA];

            CREDO_VERUM (gesta_processum_incipere(m5,
                "cursus-simplex", "{}", "{}", "fran", piscina, &pf));
            CREDO_VERUM (pf.facta);
            memcpy(id_i2, pf.instantia, GESTA_RES_ID_MENSURA);

            /* definitio mutatur medio cursu */
            _scribe(m5, NIHIL, "emendatio-generis",
                "{\"titulus\":\"cursus-simplex\",\"species\":"
                "\"processus\",\"status_initialis\":\"currens\","
                "\"machina\":[[\"currens\",\"perfectus\"]],"
                "\"ordines\":[\"unus\"],\"gradus\":["
                "{\"titulus\":\"aliud\",\"ordo\":\"unus\","
                "\"genus_gradus\":\"opus\",\"positio\":0}]}");

            CREDO_VERUM (_opus_gradus(m5, id_i2, "primum", op_b,
                piscina));
            _scribe(m5, op_b, "status",
                "{\"novus\":\"perfectum\"}");
            /* photographia: secundum (vetus), non aliud (novum) */
            CREDO_VERUM (_cursus_est(m5, id_i2, "secundum",
                "incohatus", piscina));
            CREDO_VERUM (_cursus_est(m5, id_i2, "aliud", NIHIL,
                piscina));
            CREDO_VERUM (_opus_gradus(m5, id_i2, "secundum", op_b,
                piscina));
            _scribe(m5, op_b, "status",
                "{\"novus\":\"perfectum\"}");
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, id_i2,
                piscina), "perfectus"));
        }

        /* ====================================================
         * XLV. G13+G14: cursus concurrentes; porta omnes-ex,
         * obstructa NIHIL emittit, transit SEMEL
         * ==================================================== */
        {
            character op_para[GESTA_RES_ID_MENSURA];
            character op_proba[GESTA_RES_ID_MENSURA];
            character op_fini[GESTA_RES_ID_MENSURA];

            CREDO_VERUM (gesta_processum_incipere(m5,
                "cursus-geminus", "{}", "{}", "fran", piscina, &pf));
            CREDO_VERUM (pf.facta);
            memcpy(id_g, pf.instantia, GESTA_RES_ID_MENSURA);

            /* G13: cursus ambo statim generati (concurrentes) */
            CREDO_VERUM (_cursus_est(m5, id_g, "para",
                "incohatus", piscina));
            CREDO_VERUM (_cursus_est(m5, id_g, "proba",
                "incohatus", piscina));
            CREDO_VERUM (_opus_gradus(m5, id_g, "para", op_para,
                piscina));
            CREDO_VERUM (_opus_gradus(m5, id_g, "proba", op_proba,
                piscina));

            /* proba perfecta - porta adhuc obstructa (para) ->
             * NIHIL emittitur (D10) */
            _scribe(m5, op_proba, "status",
                "{\"novus\":\"perfectum\"}");
            CREDO_VERUM (_eventus_rei(m5, id_g, "porta-perfecta",
                piscina) == (s64)ZEPHYRUM);
            CREDO_VERUM (_cursus_est(m5, id_g, "fini", NIHIL,
                piscina));

            /* para perfecta -> porta transit semel, fini
             * generatur */
            _scribe(m5, op_para, "status",
                "{\"novus\":\"perfectum\"}");
            CREDO_VERUM (_eventus_rei(m5, id_g, "porta-perfecta",
                piscina) == (s64)I);
            CREDO_VERUM (_cursus_est(m5, id_g, "porta-ambo",
                "perfectus", piscina));
            CREDO_VERUM (_cursus_est(m5, id_g, "fini",
                "incohatus", piscina));

            /* fini perfectum -> processus perfectus; porta SEMEL
             * transiit (G14) */
            CREDO_VERUM (_opus_gradus(m5, id_g, "fini", op_fini,
                piscina));
            _scribe(m5, op_fini, "status",
                "{\"novus\":\"perfectum\"}");
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, id_g,
                piscina), "perfectus"));
            CREDO_VERUM (_eventus_rei(m5, id_g, "porta-perfecta",
                piscina) == (s64)I);
        }

        /* ====================================================
         * XLVI. G16: actio-gradus recusatus PENDENS manet
         * (se-sanans) - status ligaminis postea adveniens per
         * membra "ops:" excitat et sanat
         * ==================================================== */
        {
            GestaEventum e;
            s64 recusatae_ante;

            e.res_id = NIHIL;
            e.genus_eventus = "creatio";
            e.datum = "{\"genus\":\"parcum\",\"titulus\":"
                "\"Propositum\"}";
            e.actor = "fran";
            e.origo = "probatio";
            CREDO_VERUM (gesta_scribere(m5, &e, id_pp));

            recusatae_ante = _eventus_rei(m5,
                "claudere-cum-decreto", "actio-recusata",
                piscina);
            sprintf(ligamina_json, "{\"propositum\":\"%s\"}",
                id_pp);
            CREDO_VERUM (gesta_processum_incipere(m5,
                "cursus-actionis", ligamina_json,
                "{\"cur_sigilli\":\"probatio B\"}", "fran", piscina,
                &pf));
            CREDO_VERUM (pf.facta);
            memcpy(id_ac, pf.instantia, GESTA_RES_ID_MENSURA);

            /* actio temptata, porta actionis recusavit (parcum
             * parcatum, non tractum): gradus PENDENS manet,
             * defectus nullus, instantia currens */
            CREDO_VERUM (_eventus_rei(m5, "claudere-cum-decreto",
                "actio-recusata", piscina)
                == recusatae_ante + I);
            CREDO_VERUM (_cursus_est(m5, id_ac, "claudere", NIHIL,
                piscina));
            CREDO_VERUM (_eventus_rei(m5, id_ac,
                "gradus-defectus", piscina) == (s64)ZEPHYRUM);
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, id_ac,
                piscina), "currens"));

            /* praecondicio advenit: parcum -> tractum. Ligamen
             * "ops:propositum" membrum est -> excitatio -> actio
             * iterum temptatur -> facta -> processus perfectus */
            _scribe(m5, id_pp, "status",
                "{\"novus\":\"tractum\"}");
            CREDO_VERUM (_cursus_est(m5, id_ac, "claudere",
                "perfectus", piscina));
            CREDO_VERUM (_eventus_rei(m5, id_ac, "actio-acta",
                piscina) == (s64)I);
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, id_ac,
                piscina), "perfectus"));
            /* effectus actionis re vera facti: parcum clausum,
             * decretum natum */
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, id_pp,
                piscina), "clausum"));
            {
                JsonValor* st = _status_entis(m5, id_pp, piscina);

                CREDO_AEQUALIS_I32 (_notae_continentes(st,
                    "clausum: sigillum: probatio B"), I);
            }
        }

        /* ====================================================
         * XLVII. G17: error mechanicus actionis -> gradus-defectus
         * -> cascata processus-defectus
         * ==================================================== */
        {
            CREDO_VERUM (gesta_processum_incipere(m5,
                "cursus-fractus", "{}", "{}", "fran", piscina, &pf));
            CREDO_VERUM (pf.facta);
            memcpy(id_fr, pf.instantia, GESTA_RES_ID_MENSURA);

            CREDO_VERUM (_cursus_est(m5, id_fr, "rumpe",
                "defectus", piscina));
            CREDO_VERUM (_eventus_rei(m5, id_fr,
                "gradus-defectus", piscina) == (s64)I);
            CREDO_VERUM (_eventus_rei(m5, id_fr,
                "processus-defectus", piscina) == (s64)I);
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, id_fr,
                piscina), "defectus"));
        }

        /* ====================================================
         * XLVIII. G19: replicatio == stans CUM historia
         * provectionis (eventus = data; motor numquam re-ignitur)
         * ==================================================== */
        {
            chorda i_ante = gesta_res_datum(m5, id_i, piscina);
            chorda g_ante = gesta_res_datum(m5, id_g, piscina);
            chorda ac_ante = gesta_res_datum(m5, id_ac, piscina);
            s64 membra_ante = _membra_numerus(m5, id_g, NIHIL,
                piscina);
            chorda i_post;
            chorda g_post;
            chorda ac_post;

            CREDO_VERUM (gesta_annales_verificare(m5));
            CREDO_VERUM (gesta_replicare(m5));
            i_post = gesta_res_datum(m5, id_i, piscina);
            g_post = gesta_res_datum(m5, id_g, piscina);
            ac_post = gesta_res_datum(m5, id_ac, piscina);
            CREDO_VERUM (i_ante.mensura == i_post.mensura
                && memcmp(i_ante.datum, i_post.datum,
                       (memoriae_index)i_ante.mensura)
                    == ZEPHYRUM);
            CREDO_VERUM (g_ante.mensura == g_post.mensura
                && memcmp(g_ante.datum, g_post.datum,
                       (memoriae_index)g_ante.mensura)
                    == ZEPHYRUM);
            CREDO_VERUM (ac_ante.mensura == ac_post.mensura
                && memcmp(ac_ante.datum, ac_post.datum,
                       (memoriae_index)ac_ante.mensura)
                    == ZEPHYRUM);
            CREDO_VERUM (_membra_numerus(m5, id_g, NIHIL, piscina)
                == membra_ante);
            CREDO_VERUM (_chorda_aequa(gesta_res_status(m5, id_fr,
                piscina), "defectus"));
        }

        gesta_claudere(m5);
    }

    gesta_claudere(m);
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
