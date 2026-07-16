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
