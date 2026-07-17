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
#include <stdlib.h>
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
    ,
    /* migratio II (K2 spec par III, ADDITIVA - tabula nexus vetus
     * cum plicatura sua usque ad frustum C vivit, suitae virides
     * inter frusta): sine clave primaria - duplicata status licitus
     * (paritas reductoris, custodia scripturae ea nominat) */
    "CREATE TABLE membra("
    "  res_id  TEXT NOT NULL,"
    "  pars    TEXT NOT NULL,"
    "  membrum TEXT NOT NULL);"
    "CREATE INDEX idx_membra_membrum ON membra(membrum);"
    "CREATE INDEX idx_membra_res ON membra(res_id);"
    ,
    /* migratio III (K2 frustum C - CUTOVER): tabula nexus vetus
     * cadit, consumptor eius retiratur; vincula = res nexus-speciei
     * + index membra solum. Eventus veteres nexus/denexus in actis
     * manent ut TUMULI (D2). */
    "DROP TABLE nexus;"
    " DELETE FROM consumptores"
    " WHERE titulus = 'nexus';"
    ,
    /* migratio IV (K4 frustum A): index ramorum - servit lectiones
     * catenae (branch_id, seq) et examen puritatis trunci in
     * plicaturis avidis */
    "CREATE INDEX idx_tessellae_ramus ON tessellae(branch_id, seq);"
};

#define GESTA_MIGRATIONES_NUMERUS IV

structura GestaMundus {
    Piscina*            piscina;
    Scrinium*           scrinium;
    constans character* via_annalium;   /* copia in piscina */
    constans character* error;          /* litterae staticae aut
                                         * scrinium_error */
    b32                 provectio_activa;   /* custos re-entrantiae
                                             * motoris (K3 chunk B):
                                             * ansa exterior fixum
                                             * recomputat - excitatio
                                             * interior tacet */
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

interior b32
_chordae_pares (chorda a, chorda b)
{
    redde a.mensura == b.mensura
        && (a.mensura == ZEPHYRUM
            || memcmp(a.datum, b.datum,
                   (memoriae_index)a.mensura) == ZEPHYRUM);
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

/* obumbrae fascis (K3 B3): res in eodem fasce creatae, nondum
 * plicatae - validatio eventuum posteriorum eas videt. Exsistentia
 * et genus tantum; status = status_initialis generis; datum vacuum
 * (membra stantia ex obumbra nulla - salus posterior iudicat). */
nomen structura {
    chorda res_id;
    chorda genus;
} GestaObumbra;

/* obumbram quaerere (extractum K4 frustum B - via validationis
 * ramorum eandem umbram postulat) */
interior GestaResOrdo
_obumbram_quaerere (GestaMundus* m, chorda res_id, Xar* obumbrae,
    Piscina* piscina)
{
    GestaResOrdo ordo;
    i32 i;

    memset(&ordo, ZEPHYRUM, magnitudo(GestaResOrdo));
    si (obumbrae == NIHIL)
    {
        redde ordo;
    }
    per (i = ZEPHYRUM; i < xar_numerus(obumbrae); i++)
    {
        GestaObumbra* ob = (GestaObumbra*)xar_obtinere(obumbrae,
            i);

        si (ob == NIHIL || !_chordae_pares(ob->res_id, res_id))
        {
            perge;
        }
        ordo.exsistit = VERUM;
        ordo.genus = ob->genus;
        ordo.titulus = _ch("");
        ordo.status = _ch("");
        ordo.datum = _ch("");
        ordo.creatum = _ch("");
        si (ob->genus.mensura > ZEPHYRUM)
        {
            chorda gd = _genus_datum_capere(m, ob->genus,
                piscina);

            si (gd.mensura > ZEPHYRUM)
            {
                JsonResultus r = json_legere(gd, piscina);

                si (r.successus && json_est_objectum(r.radix))
                {
                    JsonValor* si_init = json_objectum_capere(
                        r.radix, "status_initialis");

                    si (si_init != NIHIL
                        && json_est_chorda(si_init))
                    {
                        ordo.status = json_ad_chorda(si_init);
                    }
                }
            }
        }
        frange;
    }
    redde ordo;
}

interior GestaResOrdo
_res_capere_cum_obumbra (GestaMundus* m, chorda res_id,
    Xar* obumbrae, Piscina* piscina)
{
    GestaResOrdo ordo = _res_capere(m, res_id, piscina);

    si (ordo.exsistit)
    {
        redde ordo;
    }
    redde _obumbram_quaerere(m, res_id, obumbrae, piscina);
}

/* prototypum lectionis ramorum (definitio in sectione Lectio
 * ramorum infra - validatio scripturae eam postulat, E2-B2) */
interior GestaResOrdo
_res_in_ramo_capere (GestaMundus* m, chorda res_id,
    constans character* ramus, Piscina* piscina);

/* ramus datus? (NIHIL aut "" = truncus) */
interior b32
_ramus_datus (constans character* ramus)
{
    redde ramus != NIHIL && ramus[0] != '\0';
}

/* lectio validationis (K4 frustum B, E2-B2): trunco = tabula stans
 * + obumbrae; ramo = plicatura catenae + obumbrae. Lectiones
 * validationis OMNES viae scripturae per hanc UNAM eunt -
 * permutatio una completa, numquam mixta (aliter custodia
 * phantasma "membrum inexistens" in ramis). */
interior GestaResOrdo
_res_validationis_capere (GestaMundus* m, chorda res_id,
    Xar* obumbrae, constans character* ramus, Piscina* piscina)
{
    GestaResOrdo ordo;

    si (!_ramus_datus(ramus))
    {
        redde _res_capere_cum_obumbra(m, res_id, obumbrae,
            piscina);
    }
    ordo = _res_in_ramo_capere(m, res_id, ramus, piscina);
    si (ordo.exsistit)
    {
        redde ordo;
    }
    redde _obumbram_quaerere(m, res_id, obumbrae, piscina);
}

/* genus datae speciei est? (K2 spec par IV: species absens =
 * genus rerum ordinarium; K3 species "actio" superveniunt -
 * generalizatio E2 par 4) */
interior b32
_species_est (GestaMundus* m, chorda genus_titulus,
    constans character* species, Piscina* piscina)
{
    chorda gd;
    JsonResultus r;
    JsonValor* v;

    si (genus_titulus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    gd = _genus_datum_capere(m, genus_titulus, piscina);
    si (gd.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    r = json_legere(gd, piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde FALSUM;
    }
    v = json_objectum_capere(r.radix, "species");
    redde v != NIHIL && json_est_chorda(v)
        && _chorda_est(json_ad_chorda(v), species);
}

interior b32
_species_nexus_est (GestaMundus* m, chorda genus_titulus,
    Piscina* piscina)
{
    redde _species_est(m, genus_titulus, "nexus", piscina);
}

/* partem nominatam in genere invenire (partes:
 * [{titulus, genera_licita, cardinalitas}]) */
interior JsonValor*
_partem_invenire (JsonValor* genus_radix, chorda titulus)
{
    JsonValor* partes;
    i32 i;

    si (genus_radix == NIHIL)
    {
        redde NIHIL;
    }
    partes = json_objectum_capere(genus_radix, "partes");
    si (partes == NIHIL || !json_est_tabulatum(partes))
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < json_tabulatum_numerus(partes); i++)
    {
        JsonValor* p = json_tabulatum_obtinere(partes, i);
        JsonValor* t = (p != NIHIL && json_est_objectum(p))
            ? json_objectum_capere(p, "titulus") : NIHIL;

        si (t != NIHIL && json_est_chorda(t)
            && _chordae_pares(json_ad_chorda(t), titulus))
        {
            redde p;
        }
    }
    redde NIHIL;
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

/* custodia membrorum (K2 spec par VI - iudicat, non obstat; TS
 * OBSTAT: smaragda.ts:3731-3783, :3952-3958, :3990-3998 -
 * divergentia D3). Querela prima inventa redditur (litterae
 * staticae); NIHIL = sanum. Status ANTE eventum legitur (tectum/
 * duplicatum contra membra stantia iudicantur). Obumbrae fascis
 * (NIHIL licet) res in eodem fasce creatas visibiles faciunt
 * (K3 B3 - vinculum modo creatum, membrum modo creatum). */
interior constans character*
_membrum_validare (GestaMundus* m, b32 additum, chorda res_id,
    JsonValor* datum_obiectum, Xar* obumbrae,
    constans character* ramus, Piscina* piscina)
{
    GestaResOrdo ordo;
    JsonValor* pars = json_objectum_capere(datum_obiectum, "pars");
    JsonValor* membrum = json_objectum_capere(datum_obiectum,
        "membrum");
    chorda c_pars;
    chorda c_membrum;
    JsonValor* genus_radix = NIHIL;
    JsonValor* pars_def;
    chorda cardinalitas;
    i32 acies_n = ZEPHYRUM;     /* membra stantia in parte */
    i32 in_acie = ZEPHYRUM;     /* occurrentiae membri dati */

    si (pars == NIHIL || !json_est_chorda(pars)
        || membrum == NIHIL || !json_est_chorda(membrum))
    {
        redde "violatio: eventus membri sine pare aut membro";
    }
    c_pars = json_ad_chorda(pars);
    c_membrum = json_ad_chorda(membrum);

    /* I. res vinculi genus nexus-speciei habeat (lectio
     * validationis - in ramo status rami iudicat, E2-B2) */
    ordo = _res_validationis_capere(m, res_id, obumbrae, ramus,
        piscina);
    si (!ordo.exsistit)
    {
        redde "violatio: membrum in genere non-nexu";
    }
    /* K3 B1: instantiae processuum quoque membra ferunt (opera
     * generata + ligamina opum; partes = gradus, motor solus
     * scribit) - custodia levis: membrum exsistat, partes non
     * iudicantur */
    si (_species_est(m, ordo.genus, "processus", piscina))
    {
        si (additum)
        {
            GestaResOrdo alter = _res_validationis_capere(m,
                c_membrum, obumbrae, ramus, piscina);

            si (!alter.exsistit)
            {
                redde "violatio: membrum inexistens";
            }
        }
        redde NIHIL;
    }
    si (!_species_nexus_est(m, ordo.genus, piscina))
    {
        redde "violatio: membrum in genere non-nexu";
    }
    {
        chorda gd = _genus_datum_capere(m, ordo.genus, piscina);
        JsonResultus r = json_legere(gd, piscina);

        si (r.successus && json_est_objectum(r.radix))
        {
            genus_radix = r.radix;
        }
    }
    /* II. pars nota sit */
    pars_def = _partem_invenire(genus_radix, c_pars);
    si (pars_def == NIHIL)
    {
        redde "violatio: pars ignota";
    }
    {
        JsonValor* c = json_objectum_capere(pars_def,
            "cardinalitas");

        cardinalitas = (c != NIHIL && json_est_chorda(c))
            ? json_ad_chorda(c) : _ch("quotlibet");
    }
    /* membra stantia ex statu rei */
    si (ordo.datum.mensura > ZEPHYRUM)
    {
        JsonResultus r = json_legere(ordo.datum, piscina);

        si (r.successus && json_est_objectum(r.radix))
        {
            JsonValor* membra = json_objectum_capere(r.radix,
                "membra");
            JsonValor* acies = (membra != NIHIL
                && json_est_objectum(membra))
                ? json_objectum_capere(membra,
                      _litterae(piscina, c_pars))
                : NIHIL;

            si (acies != NIHIL && json_est_tabulatum(acies))
            {
                i32 i;

                acies_n = json_tabulatum_numerus(acies);
                per (i = ZEPHYRUM; i < acies_n; i++)
                {
                    JsonValor* v = json_tabulatum_obtinere(acies,
                        i);

                    si (v != NIHIL && json_est_chorda(v)
                        && _chordae_pares(json_ad_chorda(v),
                               c_membrum))
                    {
                        in_acie++;
                    }
                }
            }
        }
    }
    si (additum)
    {
        GestaResOrdo alter = _res_validationis_capere(m, c_membrum,
            obumbrae, ramus, piscina);

        /* III. membrum exsistat */
        si (!alter.exsistit)
        {
            redde "violatio: membrum inexistens";
        }
        /* IV. genus membri in generibus licitis (acies vacua =
         * incoercita, TS smaragda.ts:3768) */
        {
            JsonValor* licita = json_objectum_capere(pars_def,
                "genera_licita");

            si (licita != NIHIL && json_est_tabulatum(licita)
                && json_tabulatum_numerus(licita) > ZEPHYRUM)
            {
                b32 licitum = FALSUM;
                i32 i;

                per (i = ZEPHYRUM;
                     i < json_tabulatum_numerus(licita); i++)
                {
                    JsonValor* g = json_tabulatum_obtinere(licita,
                        i);

                    si (g != NIHIL && json_est_chorda(g)
                        && _chordae_pares(json_ad_chorda(g),
                               alter.genus))
                    {
                        licitum = VERUM;
                        frange;
                    }
                }
                si (!licitum)
                {
                    redde "violatio: genus membri non licitum";
                }
            }
        }
        /* V. tectum unicum */
        si (_chorda_est(cardinalitas, "unicus")
            && acies_n >= I)
        {
            redde "violatio: cardinalitas unicus excessa";
        }
        /* VI. duplicatum (TS tacite sinit - D11: nos sinimus ET
         * nominamus) */
        si (in_acie > ZEPHYRUM)
        {
            redde "violatio: membrum duplicatum";
        }
    }
    alioquin
    {
        /* VII. membrum adsit */
        si (in_acie == ZEPHYRUM)
        {
            redde "violatio: membrum absens";
        }
        /* VIII. limen inferius */
        si ((_chorda_est(cardinalitas, "unicus")
                || _chorda_est(cardinalitas, "aliquot"))
            && acies_n <= I)
        {
            redde "violatio: cardinalitas sub limite";
        }
    }
    redde NIHIL;
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

/* fructus transformationis status (K4 frustum A) */
#define GESTA_TRANSFORMATUM_IGNOTUM 0
#define GESTA_TRANSFORMATUM_INANE   I
#define GESTA_TRANSFORMATUM_MUTATUM II

/* transformatio PURA status rei (K4 frustum A: decompositio
 * _rei_applicare, eadem ratio ac praeparare/validare/inserere K3):
 * obiectum status intra/extra, SINE scriptura tabulae - reductor
 * idem plicaturae avidae trunci ET plicaturis pigris ramorum
 * servit. m SOLUM pro generum lectione (status_initialis) - genera
 * trunco-sola (K4 decisio 8) ergo etiam in ramis recta. Corpus
 * reductoris VERBATIM ex _rei_applicare motum (barra G9: truncus
 * octetim idem). IGNOTUM = eventus reductori ignotus (nulla
 * scriptura umquam); INANE = agnitum sed non materiale (truncus
 * ordinem rescribit solum si res exsistit - mutatum renovatur);
 * MUTATUM = status materialiter mutatus. */
interior s32
_statum_transformare (GestaMundus* m, JsonValor** status_in_ex,
    chorda* genus_in_ex, chorda* creatum_in_ex, chorda genus_eventus,
    chorda datum, chorda actor, chorda creatum, Piscina* piscina)
{
    JsonValor* status_obiectum = *status_in_ex;
    chorda genus_columna = *genus_in_ex;
    chorda creatum_columna = *creatum_in_ex;
    JsonResultus r_datum;
    JsonValor* datum_obiectum = NIHIL;
    b32 mutatum_est = FALSUM;

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
    alioquin si (_chorda_est(genus_eventus, "membrum-additum")
        || _chorda_est(genus_eventus, "membrum-remotum"))
    {
        /* membra pars-clavata (TS: smaragda.ts:759-769): additum
         * appendit SINE deduplicatione (:762); remotum occurrentias
         * OMNES tollit, clavis cum acie vacua MANET (:768) */
        JsonValor* pars = (datum_obiectum != NIHIL)
            ? json_objectum_capere(datum_obiectum, "pars") : NIHIL;
        JsonValor* membrum = (datum_obiectum != NIHIL)
            ? json_objectum_capere(datum_obiectum, "membrum")
            : NIHIL;

        si (pars != NIHIL && json_est_chorda(pars)
            && membrum != NIHIL && json_est_chorda(membrum))
        {
            JsonValor* membra = json_objectum_capere(
                status_obiectum, "membra");
            constans character* pars_l = _litterae(piscina,
                json_ad_chorda(pars));
            JsonValor* acies;

            si (membra == NIHIL || !json_est_objectum(membra))
            {
                membra = json_objectum_creare(piscina);
                json_objectum_ponere(status_obiectum, "membra",
                    membra);
            }
            acies = json_objectum_capere(membra, pars_l);
            si (_chorda_est(genus_eventus, "membrum-additum"))
            {
                si (acies == NIHIL || !json_est_tabulatum(acies))
                {
                    acies = json_tabulatum_creare(piscina);
                    json_objectum_ponere(membra, pars_l, acies);
                }
                json_tabulatum_addere(acies,
                    json_chorda_creare(piscina,
                        json_ad_chorda(membrum)));
            }
            alioquin
            {
                JsonValor* nova = json_tabulatum_creare(piscina);
                chorda cm = json_ad_chorda(membrum);
                i32 i;
                i32 n = (acies != NIHIL
                    && json_est_tabulatum(acies))
                    ? json_tabulatum_numerus(acies) : ZEPHYRUM;

                per (i = ZEPHYRUM; i < n; i++)
                {
                    JsonValor* v = json_tabulatum_obtinere(acies,
                        i);

                    si (v != NIHIL && json_est_chorda(v)
                        && _chordae_pares(json_ad_chorda(v), cm))
                    {
                        perge;
                    }
                    json_tabulatum_addere(nova, v);
                }
                json_objectum_ponere(membra, pars_l, nova);
            }
            mutatum_est = VERUM;
        }
    }
    alioquin si (_chorda_est(genus_eventus, "gradus-incohatus")
        || _chorda_est(genus_eventus, "gradus-perfectus")
        || _chorda_est(genus_eventus, "gradus-defectus")
        || _chorda_est(genus_eventus, "porta-perfecta"))
    {
        /* cursus graduum instantiae processūs (K3 chunk B; paritas
         * oraculi processInstanceReducer smaragda.ts:4807-4879 -
         * status graduum in statum instantiae plicantur) */
        JsonValor* g = (datum_obiectum != NIHIL)
            ? json_objectum_capere(datum_obiectum, "gradus")
            : NIHIL;

        si (g != NIHIL && json_est_chorda(g))
        {
            JsonValor* cursus = json_objectum_capere(
                status_obiectum, "cursus");
            constans character* valor =
                _chorda_est(genus_eventus, "gradus-incohatus")
                    ? "incohatus"
                    : _chorda_est(genus_eventus, "gradus-defectus")
                        ? "defectus" : "perfectus";

            si (cursus == NIHIL || !json_est_objectum(cursus))
            {
                cursus = json_objectum_creare(piscina);
                json_objectum_ponere(status_obiectum, "cursus",
                    cursus);
            }
            json_objectum_ponere(cursus,
                _litterae(piscina, json_ad_chorda(g)),
                json_chorda_creare(piscina, _ch(valor)));
            mutatum_est = VERUM;
        }
    }
    alioquin si (_chorda_est(genus_eventus, "opus-genitum")
        || _chorda_est(genus_eventus, "actio-acta"))
    {
        /* provenientia graduum: opus generatum / actio-facta id */
        b32 genitum = _chorda_est(genus_eventus, "opus-genitum");
        JsonValor* g = (datum_obiectum != NIHIL)
            ? json_objectum_capere(datum_obiectum, "gradus")
            : NIHIL;
        JsonValor* v = (datum_obiectum != NIHIL)
            ? json_objectum_capere(datum_obiectum,
                  genitum ? "opus" : "factum")
            : NIHIL;

        si (g != NIHIL && json_est_chorda(g)
            && v != NIHIL && json_est_chorda(v))
        {
            constans character* clavis = genitum
                ? "opera" : "actae";
            JsonValor* tabula = json_objectum_capere(
                status_obiectum, clavis);

            si (tabula == NIHIL || !json_est_objectum(tabula))
            {
                tabula = json_objectum_creare(piscina);
                json_objectum_ponere(status_obiectum, clavis,
                    tabula);
            }
            json_objectum_ponere(tabula,
                _litterae(piscina, json_ad_chorda(g)), v);
            mutatum_est = VERUM;
        }
    }
    alioquin
    {
        /* genus_eventus ignotum = nihil agit (TS: smaragda.ts:
         * 771-772; nexus/denexus vetera hic cadunt = tumuli in
         * statu rei quoque, K2 D2; processus-perfectus/-defectus
         * signa mera - eventus status sequens plicat) */
        redde GESTA_TRANSFORMATUM_IGNOTUM;
    }

    *status_in_ex = status_obiectum;
    *genus_in_ex = genus_columna;
    *creatum_in_ex = creatum_columna;
    redde mutatum_est ? GESTA_TRANSFORMATUM_MUTATUM
        : GESTA_TRANSFORMATUM_INANE;
}

/* eventum unum in plicaturam rerum applicare (involucrum trunci
 * post decompositionem K4: lectio tabulae -> transformatio ->
 * scriptura tabulae; mores omnes priores octetim servati) */
interior vacuum
_rei_applicare (GestaMundus* m, chorda res_id, chorda genus_eventus,
    chorda datum, chorda actor, chorda creatum, Piscina* piscina)
{
    GestaResOrdo ordo;
    JsonValor* status_obiectum = NIHIL;
    chorda genus_columna;
    chorda creatum_columna;
    s32 fructus;

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

    fructus = _statum_transformare(m, &status_obiectum,
        &genus_columna, &creatum_columna, genus_eventus, datum,
        actor, creatum, piscina);
    si (fructus == GESTA_TRANSFORMATUM_IGNOTUM
        || (fructus == GESTA_TRANSFORMATUM_INANE && !ordo.exsistit))
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

/* ==================================================
 * Lectio ramorum (K4 frustum A) - plicatura pigra per catenam
 * ==================================================
 * Truncus avidus intactus; lectio rami = quaesitum catenae
 * parentum + plicatura in memoria per petitionem (K4 decisio 3).
 * TECTUM PER NEXUM (correctio E1-B2/D1): quisque parens ad punctum
 * FILII SUI tectus - nepos scripturas post-furcam parentis NON
 * videt (oraculum rimosum: _collectBranchTessellae sine tecto
 * parentum mediorum). Ramus = res generis "ramus" IN TRUNCO
 * {titulus, parens (res_id; "" = truncus), punctum (seq furcae)};
 * parametrum rami in nucleo = RES_ID rami (tituli inter activos
 * solos unici - resolutio tituli stratum tenentis est, frustum C).
 * Lectio rami fusi/abiecti LICET (archaeologia, E1-B8) -
 * scripturae solae recusantur (frustum B). */

nomen structura {
    chorda ramus;    /* res_id rami; "" = truncus */
    s64    tectum;   /* seq tectum inclusivum; -I = sine tecto */
} GestaCatenaNexus;

#define GESTA_CATENA_ALTITUDO_MAXIMA XVI

/* catena a ramo ad truncum: [{ramus, -I}, {parens, punctum_rami},
 * ..., {"", punctum_filii_primi}]. Ordo nexuum non portans
 * (clausulae OR in quaesito). NIHIL = ramus ignotus / non-ramus /
 * datum corruptum / catena nimis alta (ansae custodia). */
interior Xar*
_catena_rami (GestaMundus* m, constans character* ramus,
    Piscina* piscina)
{
    Xar* catena = xar_creare(piscina,
        (i32)magnitudo(GestaCatenaNexus));
    chorda currens = _ch(ramus);
    s64 tectum = -I;
    i32 gradus;

    si (catena == NIHIL)
    {
        (vacuum)_fractum(m, "xar catenae fractum");
        redde NIHIL;
    }
    per (gradus = ZEPHYRUM; gradus < GESTA_CATENA_ALTITUDO_MAXIMA;
        gradus++)
    {
        GestaCatenaNexus* nexus_novus =
            (GestaCatenaNexus*)xar_addere(catena);
        GestaResOrdo ordo;
        JsonResultus r;
        JsonValor* v_parens;
        JsonValor* v_punctum;

        si (nexus_novus == NIHIL)
        {
            (vacuum)_fractum(m, "xar catenae fractum");
            redde NIHIL;
        }
        nexus_novus->ramus = currens;
        nexus_novus->tectum = tectum;
        si (currens.mensura == ZEPHYRUM)
        {
            redde catena;   /* truncus attactus */
        }
        ordo = _res_capere(m, currens, piscina);
        si (!ordo.exsistit || !_chorda_est(ordo.genus, "ramus"))
        {
            (vacuum)_fractum(m, "ramus ignotus");
            redde NIHIL;
        }
        r = json_legere(ordo.datum, piscina);
        si (!r.successus || !json_est_objectum(r.radix))
        {
            (vacuum)_fractum(m, "datum rami corruptum");
            redde NIHIL;
        }
        v_parens = json_objectum_capere(r.radix, "parens");
        v_punctum = json_objectum_capere(r.radix, "punctum");
        si (v_parens == NIHIL || !json_est_chorda(v_parens)
            || v_punctum == NIHIL || !json_est_integer(v_punctum))
        {
            (vacuum)_fractum(m, "datum rami corruptum");
            redde NIHIL;
        }
        currens = json_ad_chorda(v_parens);
        tectum = json_ad_integer(v_punctum);
    }
    (vacuum)_fractum(m, "catena ramorum nimis alta");
    redde NIHIL;
}

/* res in ramo capere: plicatura in memoria super eventus catenae
 * (truncus ad punctum + parentes tecti + ramus ipse, ordine seq).
 * Ordo redditus proiectiones easdem fert ac tabula res (genus ex
 * creatio, titulus/status ex statu plicato). Quaestio pura - SINE
 * scriptura; pretium proportionale usui ramorum (decisio 3, sine
 * copia conservata v1). */
interior GestaResOrdo
_res_in_ramo_capere (GestaMundus* m, chorda res_id,
    constans character* ramus, Piscina* piscina)
{
    GestaResOrdo ordo;
    Xar* catena;
    ChordaAedificator* aed;
    ScriniumEnuntiatum* e;
    JsonValor* status_obiectum = NIHIL;
    chorda genus_columna = _ch("");
    chorda creatum_columna = _ch("");
    integer ligamen;
    i32 i;

    memset(&ordo, ZEPHYRUM, magnitudo(GestaResOrdo));
    catena = _catena_rami(m, ramus, piscina);
    si (catena == NIHIL)
    {
        redde ordo;   /* error iam positus */
    }
    aed = chorda_aedificator_creare(piscina, 256);
    si (aed == NIHIL)
    {
        (vacuum)_fractum(m, "aedificator fractus");
        redde ordo;
    }
    chorda_aedificator_appendere_literis(aed,
        "SELECT genus_eventus, datum, actor, creatum"
        " FROM tessellae WHERE res_id = ? AND (");
    per (i = ZEPHYRUM; i < xar_numerus(catena); i++)
    {
        GestaCatenaNexus* n = (GestaCatenaNexus*)xar_obtinere(
            catena, i);

        si (i > ZEPHYRUM)
        {
            chorda_aedificator_appendere_literis(aed, " OR ");
        }
        chorda_aedificator_appendere_literis(aed,
            (n != NIHIL && n->tectum >= ZEPHYRUM)
                ? "(branch_id = ? AND seq <= ?)"
                : "(branch_id = ?)");
    }
    chorda_aedificator_appendere_literis(aed, ") ORDER BY seq");
    e = scrinium_praeparare(m->scrinium,
        _litterae(piscina, chorda_aedificator_finire(aed)));
    si (e == NIHIL)
    {
        (vacuum)_fractum(m, scrinium_error(m->scrinium));
        redde ordo;
    }
    scrinium_ligare_textum(e, I, res_id);
    ligamen = II;
    per (i = ZEPHYRUM; i < xar_numerus(catena); i++)
    {
        GestaCatenaNexus* n = (GestaCatenaNexus*)xar_obtinere(
            catena, i);

        si (n == NIHIL)
        {
            perge;
        }
        /* _chorda_tuta: nexus trunci fert "" cum dato NIHIL (ex
         * json_ad_chorda parentis vacui) - ligare crudum = SQL
         * NULL, branch_id = NULL nihil congruit (decipula
         * documentata supra) */
        scrinium_ligare_textum(e, ligamen, _chorda_tuta(n->ramus));
        ligamen++;
        si (n->tectum >= ZEPHYRUM)
        {
            scrinium_ligare_numerum(e, ligamen, n->tectum);
            ligamen++;
        }
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        chorda genus_ev = scrinium_columna_textus(e, 0, piscina);
        chorda datum = scrinium_columna_textus(e, I, piscina);
        chorda actor = scrinium_columna_textus(e, II, piscina);
        chorda creatum = scrinium_columna_textus(e, III, piscina);
        s32 fructus;

        si (_est_eventus_generis(genus_ev))
        {
            perge;   /* consumptoris generum - non status rei */
        }
        si (!ordo.exsistit)
        {
            /* paritas involucri trunci: sine ordine exsistente
             * status vacuus, genus vacuum, creatum eventūs (eventus
             * INANIS statum non tangit - initium purum sequenti) */
            status_obiectum = json_objectum_creare(piscina);
            genus_columna = _ch("");
            creatum_columna = creatum;
        }
        fructus = _statum_transformare(m, &status_obiectum,
            &genus_columna, &creatum_columna, genus_ev, datum,
            actor, creatum, piscina);
        si (fructus == GESTA_TRANSFORMATUM_MUTATUM)
        {
            ordo.exsistit = VERUM;
        }
    }
    scrinium_finire(e);
    si (!ordo.exsistit)
    {
        redde ordo;
    }
    ordo.genus = genus_columna;
    ordo.creatum = creatum_columna;
    ordo.datum = json_scribere(status_obiectum, piscina);
    {
        JsonValor* v_titulus = json_objectum_capere(status_obiectum,
            "titulus");
        JsonValor* v_status = json_objectum_capere(status_obiectum,
            "status");

        ordo.titulus = (v_titulus != NIHIL
            && json_est_chorda(v_titulus))
            ? json_ad_chorda(v_titulus) : _ch("");
        ordo.status = (v_status != NIHIL
            && json_est_chorda(v_status))
            ? json_ad_chorda(v_status) : _ch("");
    }
    redde ordo;
}

/* eventum unum in plicaturam membrorum applicare (K2 chunk A -
 * index vinculorum: membrum-additum inserit [duplicata licita],
 * membrum-remotum congruentia OMNIA delet [paritas reductoris],
 * status "solutum" in re nexus-speciei ordines rei purgat [res et
 * historia manent]. Eventus veteres nexus/denexus = TUMULI hic
 * [K2 D2]; consumptor res ANTE membra currit - genus rei iam
 * plicatum cum solutum iudicatur.) */
interior vacuum
_membris_applicare (GestaMundus* m, chorda res_id,
    chorda genus_eventus, chorda datum, Piscina* piscina)
{
    b32 additum = _chorda_est(genus_eventus, "membrum-additum");
    b32 remotum = _chorda_est(genus_eventus, "membrum-remotum");
    b32 status_ev = _chorda_est(genus_eventus, "status");
    JsonResultus r;

    si (!additum && !remotum && !status_ev)
    {
        redde;
    }
    r = json_legere(datum, piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde;
    }
    si (status_ev)
    {
        JsonValor* novus = json_objectum_capere(r.radix, "novus");
        GestaResOrdo ordo;
        ScriniumEnuntiatum* e;

        si (novus == NIHIL || !json_est_chorda(novus)
            || !_chorda_est(json_ad_chorda(novus), "solutum"))
        {
            redde;
        }
        ordo = _res_capere(m, res_id, piscina);
        si (!ordo.exsistit
            || !_species_nexus_est(m, ordo.genus, piscina))
        {
            redde;   /* solutum in re ordinaria membra non tangit */
        }
        e = scrinium_praeparare(m->scrinium,
            "DELETE FROM membra WHERE res_id = ?");
        si (e == NIHIL)
        {
            redde;
        }
        scrinium_ligare_textum(e, I, res_id);
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
        redde;
    }
    {
        JsonValor* pars = json_objectum_capere(r.radix, "pars");
        JsonValor* membrum = json_objectum_capere(r.radix,
            "membrum");
        ScriniumEnuntiatum* e;

        si (pars == NIHIL || !json_est_chorda(pars)
            || membrum == NIHIL || !json_est_chorda(membrum))
        {
            redde;
        }
        e = scrinium_praeparare(m->scrinium, additum
            ? "INSERT INTO membra (res_id, pars, membrum)"
              " VALUES (?, ?, ?)"
            : "DELETE FROM membra WHERE res_id = ? AND pars = ?"
              " AND membrum = ?");
        si (e == NIHIL)
        {
            redde;
        }
        scrinium_ligare_textum(e, I, res_id);
        scrinium_ligare_textum(e, II, json_ad_chorda(pars));
        scrinium_ligare_textum(e, III, json_ad_chorda(membrum));
        (vacuum)scrinium_gradi(e);
        scrinium_finire(e);
    }
}

/* genera consumptorum */
#define GESTA_CONSUMPTOR_RES    0
#define GESTA_CONSUMPTOR_GENERA 1
#define GESTA_CONSUMPTOR_MEMBRA 2

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
    /* puritas trunci (K4, sutura una E2 par 1): plicaturae avidae
     * ordines ramorum numquam vident. hwm = limen, non numerus -
     * seq ramorum praetermissae nec morantur nec replicant (E2
     * par 4); census/FTS/salus/motor puri per constructionem. */
    e = scrinium_praeparare(m->scrinium,
        "SELECT seq, res_id, genus_eventus, datum, actor, creatum"
        " FROM tessellae WHERE seq > ? AND branch_id = ''"
        " ORDER BY seq");
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
        alioquin si (genus_consumptoris == GESTA_CONSUMPTOR_MEMBRA)
        {
            _membris_applicare(m, res_id, genus_ev, datum,
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
    /* ORDO PORTANS: genera ante res (spec-v2 par V); membra
     * TERTIUS - post res, quia solutum genus rei plicatum consulit
     * (K2 spec par V) */
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
    redde _consumptorem_plicare(mundus, "membra",
        GESTA_CONSUMPTOR_MEMBRA);
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
    constans character* ramus, chorda creatum)
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
        json_chorda_creare(m->piscina,
            _ch(ramus != NIHIL ? ramus : "")));
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

/* creatum unum per fascem: strftime semel lectum (inter enuntiata
 * progreditur - timestamp stabile per fascem, idem $nunc actionum;
 * forma eadem ac DEFAULT columnae) */
interior chorda
_nunc_capere (GestaMundus* m)
{
    ScriniumEnuntiatum* e;
    chorda nunc = _ch("");

    e = scrinium_praeparare(m->scrinium,
        "SELECT strftime('%Y-%m-%dT%H:%M:%fZ','now')");
    si (e == NIHIL)
    {
        redde nunc;
    }
    si (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        nunc = scrinium_columna_textus(e, 0, m->piscina);
    }
    scrinium_finire(e);
    redde nunc;
}

/* eventum paratum: GestaEventum solutum ante scripturam
 * (decompositio K3 spec-v2 par III: praeparare / validare /
 * inserere sine transactione propria - fascis BEGIN/COMMIT unum
 * tenet) */
nomen structura {
    constans character* event_id;      /* ULID NUL-terminatum */
    chorda              res_id;
    constans character* genus_eventus;
    chorda              datum_canon;
    JsonValor*          datum_obiectum;
    constans character* actor;
    constans character* origo;
    b32                 est_creatio;
    b32                 est_generis;
} GestaEventumParatum;

/* praeparare: syntaxis + res_id effectivum. ULID creationis hic
 * NON cuditur - fascis cudit (aut executor actionis prae-cudit ut
 * $novus liget - E2 par 1) et res_id expressum tradit. */
interior b32
_eventum_praeparare (GestaMundus* m, constans GestaEventum* e,
    GestaEventumParatum* p)
{
    JsonResultus r;

    memset(p, ZEPHYRUM, magnitudo(GestaEventumParatum));
    si (e->genus_eventus == NIHIL || e->actor == NIHIL)
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
    p->datum_obiectum = r.radix;
    p->datum_canon = json_scribere(r.radix, m->piscina);
    p->genus_eventus = e->genus_eventus;
    p->actor = e->actor;
    p->origo = e->origo != NIHIL ? e->origo : "";
    p->est_creatio = (strcmp(e->genus_eventus, "creatio")
        == ZEPHYRUM);
    p->est_generis = _est_eventus_generis(_ch(e->genus_eventus));

    /* res_id effectivum */
    si (p->est_generis)
    {
        JsonValor* titulus = json_objectum_capere(
            p->datum_obiectum, "titulus");

        si (titulus == NIHIL || !json_est_chorda(titulus))
        {
            redde _fractum(m, "eventus generis sine titulo");
        }
        p->res_id = json_ad_chorda(titulus);
    }
    alioquin si (e->res_id != NIHIL)
    {
        p->res_id = _ch(e->res_id);
    }
    alioquin
    {
        redde _fractum(m, "eventum sine res_id");
    }
    redde VERUM;
}

/* validatio - iudicat, non obstat (violatio -> nota custodiae
 * intra fascem). Lectiones purae contra plicaturas stantes et
 * obumbras fascis (B3). Litterae staticae aut in piscina;
 * NIHIL = sanum. */
interior constans character*
_eventum_validare (GestaMundus* m, constans GestaEventumParatum* p,
    Xar* obumbrae, constans character* ramus)
{
    si (p->est_creatio)
    {
        JsonValor* g = json_objectum_capere(p->datum_obiectum,
            "genus");

        si (g != NIHIL && json_est_chorda(g))
        {
            chorda gd = _genus_datum_capere(m, json_ad_chorda(g),
                m->piscina);

            si (gd.mensura == ZEPHYRUM)
            {
                redde "violatio: genus ignotum ad creationem"
                    " (definitio-generis deest)";
            }
        }
        redde NIHIL;
    }
    si (strcmp(p->genus_eventus, "status") == ZEPHYRUM)
    {
        /* in ramo: machina contra statum RAMI iudicat (decisio 13) */
        GestaResOrdo ordo = _res_validationis_capere(m, p->res_id,
            obumbrae, ramus, m->piscina);
        JsonValor* novus = json_objectum_capere(p->datum_obiectum,
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
                redde "violatio machinae: transitio extra"
                    " machinam generis";
            }
        }
        redde NIHIL;
    }
    si (strcmp(p->genus_eventus, "membrum-additum") == ZEPHYRUM
        || strcmp(p->genus_eventus, "membrum-remotum") == ZEPHYRUM)
    {
        /* custodia membrorum K2 spec par VI (I-VIII) */
        redde _membrum_validare(m,
            strcmp(p->genus_eventus, "membrum-additum")
                == ZEPHYRUM,
            p->res_id, p->datum_obiectum, obumbrae, ramus,
            m->piscina);
    }
    si (p->est_generis)
    {
        /* status_initialis in machina appareat (si machina datur)
         * - lex definitionis */
        JsonValor* si_init = json_objectum_capere(
            p->datum_obiectum, "status_initialis");
        JsonValor* machina = json_objectum_capere(
            p->datum_obiectum, "machina");

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
                JsonValor* par = json_tabulatum_obtinere(machina,
                    i);
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
                redde "violatio definitionis: status_initialis"
                    " extra machinam";
            }
        }
        redde NIHIL;
    }
    redde NIHIL;
}

/* inserere: INSERT + annales + sordidae - SINE BEGIN/COMMIT,
 * intra transactionem apertam fascis currit. Creatum expresse
 * ligatur (unum per fascem). branch_id expresse ligatur ('' pro
 * trunco - numquam DEFAULT, E2-B6); scripturae ramorum sordidas
 * NON tangunt (indices trunci caeci per constructionem). */
interior b32
_eventum_inserere (GestaMundus* m,
    constans GestaEventumParatum* p, chorda nunc,
    constans character* ramus)
{
    s64 seq;

    {
        ScriniumEnuntiatum* ins = scrinium_praeparare(m->scrinium,
            "INSERT INTO tessellae"
            " (id, res_id, genus_eventus, datum, actor, origo,"
            "  creatum, branch_id) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

        si (ins == NIHIL)
        {
            redde _fractum(m, scrinium_error(m->scrinium));
        }
        scrinium_ligare_textum(ins, I, _ch(p->event_id));
        scrinium_ligare_textum(ins, II, p->res_id);
        scrinium_ligare_textum(ins, III, _ch(p->genus_eventus));
        scrinium_ligare_textum(ins, IV, p->datum_canon);
        scrinium_ligare_textum(ins, V, _ch(p->actor));
        scrinium_ligare_textum(ins, VI, _ch(p->origo));
        scrinium_ligare_textum(ins, VII, nunc);
        scrinium_ligare_textum(ins, VIII,
            _ch(ramus != NIHIL ? ramus : ""));
        si (scrinium_gradi(ins) != SCRINIUM_FACTUM)
        {
            scrinium_finire(ins);
            redde _fractum(m, scrinium_error(m->scrinium));
        }
        scrinium_finire(ins);
    }
    seq = scrinium_ultimum_id(m->scrinium);
    si (!_annalem_appendere(m, seq, p->event_id, p->res_id,
            p->genus_eventus, p->datum_obiectum, p->actor,
            p->origo, ramus, nunc))
    {
        redde _fractum(m, "annales non appensi");
    }
    si (!p->est_generis && !_ramus_datus(ramus))
    {
        ScriniumEnuntiatum* so = scrinium_praeparare(m->scrinium,
            "INSERT OR IGNORE INTO sordidae (res_id) VALUES (?)");

        si (so != NIHIL)
        {
            scrinium_ligare_textum(so, I, p->res_id);
            (vacuum)scrinium_gradi(so);
            scrinium_finire(so);
        }
    }
    redde VERUM;
}

/* rector fascis: praeparatio + validatio OMNIUM ante scripturam;
 * error mechanicus totum recusat (nihil scriptum - G1); notae
 * custodiae interpositae statim post eventum suum (G3); BEGIN
 * unum, COMMIT unum, lineae annalium ordine seq ante COMMIT.
 * nunc vacuum = cuditur (executor actionis suum tradit - $nunc
 * et creatum idem). SINE plicatura - vocans agit. */
interior b32
_fascis_scribere (GestaMundus* m,
    constans GestaFascisEventum* eventa, i32 numerus,
    character* res_ids_out, chorda nunc,
    constans character* ramus)
{
    Xar* parata;      /* GestaEventumParatum */
    Xar* obumbrae;    /* GestaObumbra - res in fasce creatae (B3) */
    i32 i;

    si (numerus < I)
    {
        redde _fractum(m, "fascis vacuus");
    }
    /* K4 frustum B: scriptura in ramo - ramus exsistat, generis
     * ramus sit, ACTIVUS sit (fusus/abiectus = lineae temporis
     * clausae; lectio libera manet - archaeologia) */
    si (_ramus_datus(ramus))
    {
        GestaResOrdo r_ordo = _res_capere(m, _ch(ramus),
            m->piscina);

        si (!r_ordo.exsistit
            || !_chorda_est(r_ordo.genus, "ramus"))
        {
            redde _fractum(m, "ramus ignotus");
        }
        si (!_chorda_est(r_ordo.status, "activus"))
        {
            redde _fractum(m, "ramus non activus (fusus aut"
                " abiectus)");
        }
    }
    parata = xar_creare(m->piscina,
        (i32)magnitudo(GestaEventumParatum));
    obumbrae = xar_creare(m->piscina,
        (i32)magnitudo(GestaObumbra));
    si (parata == NIHIL || obumbrae == NIHIL)
    {
        redde _fractum(m, "piscina exhausta");
    }
    si (nunc.mensura == ZEPHYRUM)
    {
        nunc = _nunc_capere(m);
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        GestaEventum e;
        GestaEventumParatum p;
        constans character* violatio;

        e = eventa[i].eventum;
        /* ULID creationis: fascis cudit si vocans non dedit */
        si (e.genus_eventus != NIHIL && e.res_id == NIHIL
            && strcmp(e.genus_eventus, "creatio") == ZEPHYRUM)
        {
            character ulid[SCRINIUM_ULID_MENSURA];

            scrinium_ulid(ulid);
            e.res_id = _litterae(m->piscina, _ch(ulid));
        }
        si (!_eventum_praeparare(m, &e, &p))
        {
            redde FALSUM;
        }
        /* genera trunco-sola (K4 decisio 8): eventus generis cum
         * ramo = recusatio mechanica (oraculum tacite vorabat -
         * strictiores consulto, D5) */
        si (p.est_generis && _ramus_datus(ramus))
        {
            redde _fractum(m, "genera non ramificantur");
        }
        /* identitas: creatio duplicata recusatur - contra statum
         * VISUM (trunco: tabula stans; ramo: plicatura catenae -
         * res trunci post punctum nata ramo invisibilis = semantica
         * furcae honesta) ET obumbras (creatio bis in eodem fasce) */
        si (p.est_creatio)
        {
            GestaResOrdo ordo = _res_validationis_capere(m,
                p.res_id, obumbrae, ramus, m->piscina);

            si (ordo.exsistit)
            {
                redde _fractum(m, "creatio duplicata recusata"
                    " (res iam exsistit)");
            }
        }
        /* id eventus: datum aut cusum */
        si (eventa[i].event_id != NIHIL)
        {
            p.event_id = eventa[i].event_id;
        }
        alioquin
        {
            character uev[SCRINIUM_ULID_MENSURA];

            scrinium_ulid(uev);
            p.event_id = _litterae(m->piscina, _ch(uev));
        }

        violatio = _eventum_validare(m, &p, obumbrae, ramus);
        {
            GestaEventumParatum* locus =
                (GestaEventumParatum*)xar_addere(parata);

            si (locus == NIHIL)
            {
                redde _fractum(m, "piscina exhausta");
            }
            *locus = p;
        }
        si (violatio != NIHIL)
        {
            /* nota custodiae INTRA fascem (scriptura secunda K1
             * moritur - transactione eadem, sequenter adiacens) */
            GestaEventumParatum n;
            JsonValor* d = json_objectum_creare(m->piscina);
            character uev[SCRINIUM_ULID_MENSURA];
            GestaEventumParatum* locus;

            json_objectum_ponere(d, "textus",
                json_chorda_creare(m->piscina, _ch(violatio)));
            memset(&n, ZEPHYRUM, magnitudo(GestaEventumParatum));
            scrinium_ulid(uev);
            n.event_id = _litterae(m->piscina, _ch(uev));
            n.res_id = p.res_id;
            n.genus_eventus = "nota";
            n.datum_obiectum = d;
            n.datum_canon = json_scribere(d, m->piscina);
            n.actor = "machina";
            n.origo = "gesta-custodia";
            locus = (GestaEventumParatum*)xar_addere(parata);
            si (locus == NIHIL)
            {
                redde _fractum(m, "piscina exhausta");
            }
            *locus = n;
        }
        si (p.est_creatio)
        {
            JsonValor* g = json_objectum_capere(p.datum_obiectum,
                "genus");
            GestaObumbra* ob = (GestaObumbra*)xar_addere(
                obumbrae);

            si (ob != NIHIL)
            {
                ob->res_id = p.res_id;
                ob->genus = (g != NIHIL && json_est_chorda(g))
                    ? json_ad_chorda(g) : _ch("");
            }
        }
        si (res_ids_out != NIHIL)
        {
            character* sedes = res_ids_out
                + (memoriae_index)i
                    * (memoriae_index)GESTA_RES_ID_MENSURA;
            i32 n_oct = p.res_id.mensura
                    < (i32)(GESTA_RES_ID_MENSURA - I)
                ? p.res_id.mensura
                : (i32)(GESTA_RES_ID_MENSURA - I);

            si (n_oct > ZEPHYRUM)
            {
                memcpy(sedes, p.res_id.datum,
                    (memoriae_index)n_oct);
            }
            sedes[n_oct] = '\0';
        }
    }

    /* transactio una: INSERT + linea annalium quaeque ordine seq
     * (annales ANTE COMMIT - ruina => superset, lex K1) */
    si (!scrinium_incipere(m->scrinium))
    {
        redde _fractum(m, scrinium_error(m->scrinium));
    }
    per (i = ZEPHYRUM; i < xar_numerus(parata); i++)
    {
        constans GestaEventumParatum* p =
            (constans GestaEventumParatum*)xar_obtinere(parata,
                i);

        si (!_eventum_inserere(m, p, nunc, ramus))
        {
            (vacuum)scrinium_revolvere(m->scrinium);
            redde FALSUM;
        }
    }
    si (!scrinium_committere(m->scrinium))
    {
        (vacuum)scrinium_revolvere(m->scrinium);
        redde _fractum(m, scrinium_error(m->scrinium));
    }
    redde VERUM;
}

/* excitatio motoris provectionis (K3 chunk B; definitio in
 * sectione Processuum infra - motor totam grammaticam actionum
 * postulat). Eventus STATUS solus excitat (decisio 10). */
interior vacuum
_provectionem_excitare (GestaMundus* m,
    constans character* res_id);

b32
gesta_fascis_scribere (GestaMundus* mundus,
    constans GestaFascisEventum* eventa, i32 numerus,
    character* res_ids_out)
{
    chorda vacua;
    i32 i;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (mundus == NIHIL || eventa == NIHIL)
    {
        redde FALSUM;
    }
    /* res_ids necessaria excitationi - quaternio localis si vocans
     * non dedit */
    si (res_ids_out == NIHIL && numerus >= I)
    {
        res_ids_out = (character*)piscina_allocare(mundus->piscina,
            (memoriae_index)numerus
            * (memoriae_index)GESTA_RES_ID_MENSURA);
        si (res_ids_out == NIHIL)
        {
            redde _fractum(mundus, "piscina exhausta");
        }
    }
    si (!_fascis_scribere(mundus, eventa, numerus, res_ids_out,
            vacua, NIHIL))
    {
        redde FALSUM;
    }
    si (!gesta_plicare(mundus))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (eventa[i].eventum.genus_eventus != NIHIL
            && strcmp(eventa[i].eventum.genus_eventus, "status")
                == ZEPHYRUM)
        {
            _provectionem_excitare(mundus, res_ids_out
                + (memoriae_index)i
                    * (memoriae_index)GESTA_RES_ID_MENSURA);
        }
    }
    redde VERUM;
}

b32
gesta_scribere (GestaMundus* mundus, constans GestaEventum* eventum,
    character* res_id_out)
{
    GestaFascisEventum unus;
    chorda vacua;
    character locus[GESTA_RES_ID_MENSURA];

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (mundus == NIHIL || eventum == NIHIL)
    {
        redde FALSUM;
    }
    si (res_id_out == NIHIL)
    {
        res_id_out = locus;
    }
    unus.event_id = NIHIL;
    unus.eventum = *eventum;
    si (!_fascis_scribere(mundus, &unus, I, res_id_out, vacua,
            NIHIL))
    {
        redde FALSUM;
    }
    si (!gesta_plicare(mundus))
    {
        redde FALSUM;
    }
    /* excitatio motoris: eventus status in membro instantiae
     * currentis processum provehit (K3 decisio 10) */
    si (strcmp(eventum->genus_eventus, "status") == ZEPHYRUM)
    {
        _provectionem_excitare(mundus, res_id_out);
    }
    redde VERUM;
}

b32
gesta_in_ramo_scribere (GestaMundus* mundus,
    constans GestaEventum* eventum, constans character* ramus,
    character* res_id_out)
{
    GestaFascisEventum unus;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (mundus == NIHIL || eventum == NIHIL)
    {
        redde FALSUM;
    }
    si (!_ramus_datus(ramus))
    {
        redde gesta_scribere(mundus, eventum, res_id_out);
    }
    unus.event_id = NIHIL;
    unus.eventum = *eventum;
    /* SINE plicatura, SINE excitatione - scripturae ramorum
     * indices motoremque trunci numquam tangunt (decisiones 7/12);
     * re-ingressus in terram processuum per fusionem solam */
    redde _fascis_scribere(mundus, &unus, I, res_id_out, vacua,
        ramus);
}

/* ==================================================
 * Vita ramorum (K4 frustum B) - creare/enumerare/abicere/
 * confligentia/fundere
 * ==================================================
 * Ramus = res generis "ramus" IN TRUNCO - vita per viam scripturae
 * ordinariam (creatio/status), acta honesta. ABICERE FLAGSHIP
 * (charta E3: rami oraculi demo-ware erant - abicere numquam
 * cucurrit; nostrum vile consulto). FUSIO = replay-on-merge:
 * copiae CRUDAE in truncum (numquam per _fascis_scribere -
 * revalidatio notas phantasma pareret, E2-B3), transactione UNA
 * cum statu fusus (D2), attributione servata (D3), verrere sondae
 * post plicaturam (D4). */

b32
gesta_ramum_creare (GestaMundus* mundus,
    constans character* titulus, constans character* parens,
    constans character* actor, Piscina* piscina,
    character* res_id_out)
{
    JsonValor* d;
    GestaEventum e;
    chorda dj;

    si (mundus == NIHIL)
    {
        redde FALSUM;
    }
    si (titulus == NIHIL || titulus[0] == '\0' || piscina == NIHIL)
    {
        redde _fractum(mundus, "ramo titulus et piscina"
            " requiruntur");
    }
    /* parens: truncus (NIHIL/"") aut ramus ACTIVUS */
    si (_ramus_datus(parens))
    {
        GestaResOrdo p_ordo = _res_capere(mundus, _ch(parens),
            piscina);

        si (!p_ordo.exsistit
            || !_chorda_est(p_ordo.genus, "ramus"))
        {
            redde _fractum(mundus, "parens ramus ignotus");
        }
        si (!_chorda_est(p_ordo.status, "activus"))
        {
            redde _fractum(mundus, "parens ramus non activus");
        }
    }
    /* titulus inter ACTIVOS unicus (E1-B9: quaesitum unum, non
     * ambulatio nominum O(n) oraculi) */
    {
        ScriniumEnuntiatum* e_t = scrinium_praeparare(
            mundus->scrinium,
            "SELECT COUNT(*) FROM res WHERE genus = 'ramus'"
            " AND status = 'activus' AND titulus = ?");

        si (e_t == NIHIL)
        {
            redde _fractum(mundus,
                scrinium_error(mundus->scrinium));
        }
        scrinium_ligare_textum(e_t, I, _ch(titulus));
        si (scrinium_gradi(e_t) == SCRINIUM_ORDO
            && scrinium_columna_numerus(e_t, 0) > ZEPHYRUM)
        {
            scrinium_finire(e_t);
            redde _fractum(mundus, "titulus rami inter activos"
                " duplicatus");
        }
        scrinium_finire(e_t);
    }
    /* punctum = seq ANTE creationem - creatio ipsa ordo trunci
     * post furcam est (ramo proprio invisibilis, recte) */
    d = json_objectum_creare(piscina);
    json_objectum_ponere(d, "genus",
        json_chorda_creare(piscina, _ch("ramus")));
    json_objectum_ponere(d, "titulus",
        json_chorda_creare(piscina, _ch(titulus)));
    json_objectum_ponere(d, "parens",
        json_chorda_creare(piscina,
            _ch(parens != NIHIL ? parens : "")));
    json_objectum_ponere(d, "punctum",
        json_integer_creare(piscina, gesta_seq_ultima(mundus)));
    dj = json_scribere(d, piscina);
    e.res_id = NIHIL;
    e.genus_eventus = "creatio";
    e.datum = _litterae(piscina, dj);
    e.actor = actor != NIHIL ? actor : "machina";
    e.origo = "gesta-ramus";
    redde gesta_scribere(mundus, &e, res_id_out);
}

Xar*
gesta_ramos_enumerare (GestaMundus* mundus, Piscina* piscina)
{
    Xar* fructus;
    ScriniumEnuntiatum* e;

    si (mundus == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(GestaRamusOrdo));
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT res_id, titulus, status, datum FROM res"
        " WHERE genus = 'ramus' ORDER BY res_id");
    si (fructus == NIHIL || e == NIHIL)
    {
        redde NIHIL;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        GestaRamusOrdo* o = (GestaRamusOrdo*)xar_addere(fructus);
        chorda datum;
        JsonResultus r;

        si (o == NIHIL)
        {
            frange;
        }
        memset(o, ZEPHYRUM, magnitudo(GestaRamusOrdo));
        o->res_id = scrinium_columna_textus(e, 0, piscina);
        o->titulus = scrinium_columna_textus(e, I, piscina);
        o->status = scrinium_columna_textus(e, II, piscina);
        datum = scrinium_columna_textus(e, III, piscina);
        r = json_legere(datum, piscina);
        si (r.successus && json_est_objectum(r.radix))
        {
            JsonValor* v_parens = json_objectum_capere(r.radix,
                "parens");
            JsonValor* v_punctum = json_objectum_capere(r.radix,
                "punctum");

            si (v_parens != NIHIL && json_est_chorda(v_parens))
            {
                o->parens = v_parens != NIHIL
                    ? json_ad_chorda(v_parens) : _ch("");
            }
            si (v_punctum != NIHIL && json_est_integer(v_punctum))
            {
                o->punctum = json_ad_integer(v_punctum);
            }
        }
    }
    scrinium_finire(e);
    redde fructus;
}

b32
gesta_ramum_abicere (GestaMundus* mundus, constans character* ramus,
    constans character* actor)
{
    GestaResOrdo ordo;
    GestaEventum e;

    si (mundus == NIHIL || !_ramus_datus(ramus))
    {
        redde FALSUM;
    }
    ordo = _res_capere(mundus, _ch(ramus), mundus->piscina);
    si (!ordo.exsistit || !_chorda_est(ordo.genus, "ramus"))
    {
        redde _fractum(mundus, "ramus ignotus");
    }
    si (!_chorda_est(ordo.status, "activus"))
    {
        redde _fractum(mundus, "ramus non activus");
    }
    /* FLAGSHIP: eventus UNUS - abicere gratis, fingere audacter */
    e.res_id = ramus;
    e.genus_eventus = "status";
    e.datum = "{\"novus\":\"abiectus\"}";
    e.actor = actor != NIHIL ? actor : "machina";
    e.origo = "gesta-ramus";
    redde gesta_scribere(mundus, &e, NIHIL);
}

Xar*
gesta_confligentia (GestaMundus* mundus, constans character* ramus,
    Piscina* piscina)
{
    Xar* fructus;
    Xar* rei;      /* chorda - res_id confligentium */
    GestaResOrdo r_ordo;
    s64 punctum = ZEPHYRUM;
    i32 i;

    si (mundus == NIHIL || piscina == NIHIL
        || !_ramus_datus(ramus))
    {
        redde NIHIL;
    }
    r_ordo = _res_capere(mundus, _ch(ramus), piscina);
    si (!r_ordo.exsistit || !_chorda_est(r_ordo.genus, "ramus"))
    {
        (vacuum)_fractum(mundus, "ramus ignotus");
        redde NIHIL;
    }
    {
        JsonResultus r = json_legere(r_ordo.datum, piscina);
        JsonValor* v_p = (r.successus
            && json_est_objectum(r.radix))
            ? json_objectum_capere(r.radix, "punctum") : NIHIL;

        si (v_p != NIHIL && json_est_integer(v_p))
        {
            punctum = json_ad_integer(v_p);
        }
    }
    rei = xar_creare(piscina, (i32)magnitudo(chorda));
    fructus = xar_creare(piscina,
        (i32)magnitudo(GestaConfligentia));
    si (rei == NIHIL || fructus == NIHIL)
    {
        redde NIHIL;
    }
    /* res eadem utrimque post punctum tacta (definitio oraculi) */
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            mundus->scrinium,
            "SELECT DISTINCT t1.res_id FROM tessellae t1"
            " WHERE t1.branch_id = ?1 AND EXISTS"
            " (SELECT 1 FROM tessellae t2 WHERE t2.res_id ="
            " t1.res_id AND t2.branch_id = '' AND t2.seq > ?2)"
            " ORDER BY t1.res_id");

        si (e == NIHIL)
        {
            redde NIHIL;
        }
        scrinium_ligare_textum(e, I, _ch(ramus));
        scrinium_ligare_numerum(e, II, punctum);
        dum (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            chorda* c = (chorda*)xar_addere(rei);

            si (c != NIHIL)
            {
                *c = scrinium_columna_textus(e, 0, piscina);
            }
        }
        scrinium_finire(e);
    }
    per (i = ZEPHYRUM; i < xar_numerus(rei); i++)
    {
        chorda* rid = (chorda*)xar_obtinere(rei, i);
        GestaConfligentia* con;
        GestaResOrdo trunci;
        GestaResOrdo rami;

        si (rid == NIHIL)
        {
            perge;
        }
        trunci = _res_capere(mundus, *rid, piscina);
        rami = _res_in_ramo_capere(mundus, *rid, ramus, piscina);
        con = (GestaConfligentia*)xar_addere(fructus);
        si (con == NIHIL)
        {
            redde NIHIL;
        }
        con->res_id = *rid;
        con->status_trunci = trunci.exsistit ? trunci.datum
            : _ch("");
        con->status_rami = rami.exsistit ? rami.datum : _ch("");
    }
    redde fructus;
}

/* copianda: eventus rami materializati ANTE insertionem (SELECT
 * vivum + INSERT in tabulam eandem vitatur) */
nomen structura {
    chorda res_id;
    chorda genus_eventus;
    chorda datum;
    chorda actor;
    chorda creatum;
} GestaCopianda;

/* copiam unam in truncum inserere: INSERT crudum (branch_id '',
 * origo fusionis, attributio SERVATA - D3: columna fontis unica
 * oraculi actorem destruebat) + linea annalium + sordida (indices
 * trunci copias videre debent) - intra transactionem apertam
 * fusionis. E2-B3: NUMQUAM per _fascis_scribere - ramus iam vixit
 * record-don't-block, notae custodiae eius IN flumine copiato. */
interior b32
_copiam_inserere (GestaMundus* m, chorda res_id,
    chorda genus_eventus, chorda datum, chorda actor,
    chorda origo, chorda creatum, Piscina* piscina)
{
    character uev[SCRINIUM_ULID_MENSURA];
    constans character* id_ev;
    s64 seq;
    JsonValor* datum_obiectum;

    scrinium_ulid(uev);
    id_ev = _litterae(m->piscina, _ch(uev));
    {
        ScriniumEnuntiatum* ins = scrinium_praeparare(m->scrinium,
            "INSERT INTO tessellae"
            " (id, res_id, genus_eventus, datum, actor, origo,"
            "  creatum, branch_id)"
            " VALUES (?, ?, ?, ?, ?, ?, ?, '')");

        si (ins == NIHIL)
        {
            redde _fractum(m, scrinium_error(m->scrinium));
        }
        scrinium_ligare_textum(ins, I, _ch(id_ev));
        scrinium_ligare_textum(ins, II, res_id);
        scrinium_ligare_textum(ins, III, genus_eventus);
        scrinium_ligare_textum(ins, IV, datum);
        scrinium_ligare_textum(ins, V, actor);
        scrinium_ligare_textum(ins, VI, origo);
        scrinium_ligare_textum(ins, VII, creatum);
        si (scrinium_gradi(ins) != SCRINIUM_FACTUM)
        {
            scrinium_finire(ins);
            redde _fractum(m, scrinium_error(m->scrinium));
        }
        scrinium_finire(ins);
    }
    seq = scrinium_ultimum_id(m->scrinium);
    {
        JsonResultus r = json_legere(datum, piscina);

        datum_obiectum = (r.successus
            && json_est_objectum(r.radix))
            ? r.radix : json_objectum_creare(m->piscina);
    }
    si (!_annalem_appendere(m, seq, id_ev, res_id,
            _litterae(m->piscina, genus_eventus), datum_obiectum,
            _litterae(m->piscina, actor),
            _litterae(m->piscina, origo), NIHIL, creatum))
    {
        redde _fractum(m, "annales non appensi");
    }
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
    redde VERUM;
}

b32
gesta_ramum_fundere (GestaMundus* mundus, constans character* ramus,
    b32 vis, constans character* actor, Piscina* piscina,
    GestaFusioFructus* exitus)
{
    GestaResOrdo r_ordo;
    Xar* copianda;
    Xar* status_rei;   /* chorda - sonda post fusionem (D4) */
    chorda origo_fusionis;
    chorda nunc;
    i32 i;

    si (exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(GestaFusioFructus));
    exitus->causa = "";
    si (mundus == NIHIL || piscina == NIHIL
        || !_ramus_datus(ramus))
    {
        redde FALSUM;
    }
    r_ordo = _res_capere(mundus, _ch(ramus), piscina);
    si (!r_ordo.exsistit || !_chorda_est(r_ordo.genus, "ramus"))
    {
        redde _fractum(mundus, "ramus ignotus");
    }
    si (!_chorda_est(r_ordo.status, "activus"))
    {
        exitus->causa = "ramus non activus";
        redde VERUM;
    }
    /* decisio 17: fusio de imo sursum - parens fusus aut truncus */
    {
        JsonResultus r = json_legere(r_ordo.datum, piscina);
        JsonValor* v_parens = (r.successus
            && json_est_objectum(r.radix))
            ? json_objectum_capere(r.radix, "parens") : NIHIL;
        chorda c_parens = (v_parens != NIHIL
            && json_est_chorda(v_parens))
            ? json_ad_chorda(v_parens) : _ch("");

        si (c_parens.mensura > ZEPHYRUM)
        {
            GestaResOrdo p_ordo = _res_capere(mundus, c_parens,
                piscina);

            si (!p_ordo.exsistit
                || !_chorda_est(p_ordo.status, "fusus"))
            {
                exitus->causa = "parens non fusus (fusio de imo"
                    " sursum)";
                redde VERUM;
            }
        }
    }
    /* confligentiae obstant nisi vis */
    {
        Xar* conf = gesta_confligentia(mundus, ramus, piscina);

        si (conf == NIHIL)
        {
            redde FALSUM;
        }
        exitus->confligentia_numerus = xar_numerus(conf);
        si (xar_numerus(conf) > ZEPHYRUM && !vis)
        {
            exitus->causa = "confligentiae stant (vis cogit)";
            redde VERUM;
        }
    }
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
            64);

        si (aed == NIHIL)
        {
            redde _fractum(mundus, "aedificator fractus");
        }
        chorda_aedificator_appendere_literis(aed, "merge:");
        chorda_aedificator_appendere_chorda(aed, r_ordo.titulus);
        origo_fusionis = chorda_aedificator_finire(aed);
    }
    /* materializatio eventuum rami ordine seq (SELECT clausum ante
     * insertiones) */
    copianda = xar_creare(piscina, (i32)magnitudo(GestaCopianda));
    status_rei = xar_creare(piscina, (i32)magnitudo(chorda));
    si (copianda == NIHIL || status_rei == NIHIL)
    {
        redde _fractum(mundus, "piscina exhausta");
    }
    {
        ScriniumEnuntiatum* e = scrinium_praeparare(
            mundus->scrinium,
            "SELECT res_id, genus_eventus, datum, actor, creatum"
            " FROM tessellae WHERE branch_id = ? ORDER BY seq");

        si (e == NIHIL)
        {
            redde _fractum(mundus,
                scrinium_error(mundus->scrinium));
        }
        scrinium_ligare_textum(e, I, _ch(ramus));
        dum (scrinium_gradi(e) == SCRINIUM_ORDO)
        {
            GestaCopianda* c = (GestaCopianda*)xar_addere(
                copianda);

            si (c == NIHIL)
            {
                scrinium_finire(e);
                redde _fractum(mundus, "piscina exhausta");
            }
            c->res_id = scrinium_columna_textus(e, 0, piscina);
            c->genus_eventus = scrinium_columna_textus(e, I,
                piscina);
            c->datum = scrinium_columna_textus(e, II, piscina);
            c->actor = scrinium_columna_textus(e, III, piscina);
            c->creatum = scrinium_columna_textus(e, IV, piscina);
        }
        scrinium_finire(e);
    }
    nunc = _nunc_capere(mundus);
    /* transactio UNA: copiae omnes + status fusus (D2 - oraculum
     * ansam copiarum solam involvebat; ruina inter = ramus semel
     * fusus semelque activus, historia duplex) */
    si (!scrinium_incipere(mundus->scrinium))
    {
        redde _fractum(mundus, scrinium_error(mundus->scrinium));
    }
    per (i = ZEPHYRUM; i < xar_numerus(copianda); i++)
    {
        GestaCopianda* c = (GestaCopianda*)xar_obtinere(copianda,
            i);

        si (c == NIHIL || !_copiam_inserere(mundus, c->res_id,
                c->genus_eventus, c->datum, c->actor,
                origo_fusionis, c->creatum, piscina))
        {
            (vacuum)scrinium_revolvere(mundus->scrinium);
            redde FALSUM;
        }
        si (_chorda_est(c->genus_eventus, "status"))
        {
            chorda* s = (chorda*)xar_addere(status_rei);

            si (s != NIHIL)
            {
                *s = c->res_id;
            }
        }
    }
    si (!_copiam_inserere(mundus, _ch(ramus), _ch("status"),
            _ch("{\"novus\":\"fusus\"}"),
            _ch(actor != NIHIL ? actor : "machina"),
            origo_fusionis, nunc, piscina))
    {
        (vacuum)scrinium_revolvere(mundus->scrinium);
        redde FALSUM;
    }
    si (!scrinium_committere(mundus->scrinium))
    {
        (vacuum)scrinium_revolvere(mundus->scrinium);
        redde _fractum(mundus, scrinium_error(mundus->scrinium));
    }
    si (!gesta_plicare(mundus))
    {
        redde FALSUM;
    }
    /* verrere sondae (D4 - oraculum processus in fusione numquam
     * provehebat): eventus STATUS copiati processus trunci
     * provehunt - "opus perfectum" fusum instantiam suam provehit */
    per (i = ZEPHYRUM; i < xar_numerus(status_rei); i++)
    {
        chorda* s = (chorda*)xar_obtinere(status_rei, i);

        si (s != NIHIL)
        {
            _provectionem_excitare(mundus,
                _litterae(piscina, *s));
        }
    }
    exitus->fusa = VERUM;
    exitus->copiata = xar_numerus(copianda);
    redde VERUM;
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
               "DELETE FROM membra")
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

        /* res nexus-speciei NON indicantur (structura, non prosa -
         * paritas TS _shouldIndexInFts smaragda.ts:538, K2 D8);
         * deletio supra iam facta = delere-tunc-praeterire */
        si (ordo.exsistit
            && !_species_nexus_est(mundus, ordo.genus,
                   mundus->piscina))
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

/* comparator censūs tagorum: numerus descendens, tag ascendens
 * in paritate (ordo stabilis legibilis - desideratum 'Census tags
 * ordine numeri') */
interior integer
_tagos_comparare (constans vacuum* a, constans vacuum* b)
{
    constans GestaTagNumerus* ta = (constans GestaTagNumerus*)a;
    constans GestaTagNumerus* tb = (constans GestaTagNumerus*)b;

    si (ta->numerus != tb->numerus)
    {
        redde (ta->numerus > tb->numerus) ? -I : I;
    }
    {
        i32 minima = ta->tag.mensura < tb->tag.mensura
            ? ta->tag.mensura : tb->tag.mensura;
        integer c = (minima > ZEPHYRUM)
            ? memcmp(ta->tag.datum, tb->tag.datum,
                  (memoriae_index)minima)
            : ZEPHYRUM;

        si (c != ZEPHYRUM)
        {
            redde c;
        }
        si (ta->tag.mensura == tb->tag.mensura)
        {
            redde ZEPHYRUM;
        }
        redde ta->tag.mensura < tb->tag.mensura ? -I : I;
    }
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
    /* ordinatio (Xar segmentatus - acies plana, qsort,
     * rescriptio in ordinem) */
    {
        i32 n = xar_numerus(census);

        si (n > I)
        {
            GestaTagNumerus* plana = (GestaTagNumerus*)
                piscina_allocare_ordinatum(piscina,
                    (memoriae_index)n
                        * magnitudo(GestaTagNumerus),
                    (memoriae_index)magnitudo(vacuum*));

            si (plana != NIHIL)
            {
                i32 i;

                per (i = ZEPHYRUM; i < n; i++)
                {
                    plana[i] = *(GestaTagNumerus*)xar_obtinere(
                        census, i);
                }
                qsort(plana, (memoriae_index)n,
                    magnitudo(GestaTagNumerus), _tagos_comparare);
                per (i = ZEPHYRUM; i < n; i++)
                {
                    *(GestaTagNumerus*)xar_obtinere(census, i)
                        = plana[i];
                }
            }
        }
    }
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
    /* K4 (decisio 16): branch_id in comparationem intrat -
     * instrumentum veritatis physicae ramos quoque tegit */
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT seq, id, res_id, genus_eventus, datum, actor,"
        " origo, creatum, branch_id FROM tessellae ORDER BY seq");
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
            chorda brc_o = scrinium_columna_textus(e, VIII,
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
                       _litterae(mundus->piscina, cre_o))
                || !_chorda_est(json_ad_chorda(json_objectum_capere(
                       r.radix, "branch_id")),
                       _litterae(mundus->piscina, brc_o)))
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
 * Quaesita vinculorum (K2 chunk A - super indicem membra)
 * ================================================== */

Xar*
gesta_nexus_rei (GestaMundus* mundus, constans character* res_id,
    Piscina* piscina)
{
    Xar* fructus;
    ScriniumEnuntiatum* e;

    si (mundus == NIHIL || res_id == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(GestaNexusRei));
    si (fructus == NIHIL)
    {
        redde NIHIL;
    }
    /* TS getRelationshipsForEntity (smaragda.ts:4008); ordines
     * crudi - duplicata apparent (index honestus) */
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT m.res_id, m.pars, COALESCE(r.genus, '')"
        " FROM membra m LEFT JOIN res r ON r.res_id = m.res_id"
        " WHERE m.membrum = ? ORDER BY m.res_id, m.pars");
    si (e == NIHIL)
    {
        redde fructus;
    }
    scrinium_ligare_textum(e, I, _ch(res_id));
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        GestaNexusRei* n = (GestaNexusRei*)xar_addere(fructus);

        si (n != NIHIL)
        {
            n->nexus_res = scrinium_columna_textus(e, 0, piscina);
            n->pars = scrinium_columna_textus(e, I, piscina);
            n->genus = scrinium_columna_textus(e, II, piscina);
        }
    }
    scrinium_finire(e);
    redde fructus;
}

Xar*
gesta_socii_rei (GestaMundus* mundus, constans character* res_id,
    Piscina* piscina)
{
    Xar* fructus;
    ScriniumEnuntiatum* e;

    si (mundus == NIHIL || res_id == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(GestaSocius));
    si (fructus == NIHIL)
    {
        redde NIHIL;
    }
    /* TS getRelatedEntities (smaragda.ts:4119): socii = membra
     * altera vinculorum communium */
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT m2.membrum, m2.pars, m2.res_id"
        " FROM membra m1 JOIN membra m2 ON m2.res_id = m1.res_id"
        " WHERE m1.membrum = ?1 AND m2.membrum <> ?1"
        " ORDER BY m2.res_id, m2.pars");
    si (e == NIHIL)
    {
        redde fructus;
    }
    scrinium_ligare_textum(e, I, _ch(res_id));
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        GestaSocius* s = (GestaSocius*)xar_addere(fructus);

        si (s != NIHIL)
        {
            s->membrum = scrinium_columna_textus(e, 0, piscina);
            s->pars = scrinium_columna_textus(e, I, piscina);
            s->nexus_res = scrinium_columna_textus(e, II, piscina);
        }
    }
    scrinium_finire(e);
    redde fructus;
}

/* ==================================================
 * Salus (K2 chunk B) - aestimatio pura
 * Oraculum: _evaluateHealthPure smaragda.ts:4368-4415
 * ================================================== */

/* valor typo schematis congruit? (TS _validateAttributeType
 * smaragda.ts:1790-1798; typus ignotus TRANSIT - lex progressiva) */
interior b32
_typus_validus (JsonValor* v, chorda typus)
{
    si (_chorda_est(typus, "textus"))
    {
        redde json_est_chorda(v);
    }
    si (_chorda_est(typus, "numerus"))
    {
        redde json_est_integer(v) || json_est_fluitans(v);
    }
    si (_chorda_est(typus, "veritas"))
    {
        redde json_est_boolean(v);
    }
    si (_chorda_est(typus, "tabulatum"))
    {
        redde json_est_tabulatum(v);
    }
    redde VERUM;
}

interior vacuum
_querelam_addere (Xar* querelae, constans character* typus,
    constans character* nuntius, b32 gravis)
{
    GestaQuerela* q = (GestaQuerela*)xar_addere(querelae);

    si (q != NIHIL)
    {
        q->typus = _ch(typus);
        q->nuntius = _ch(nuntius);
        q->gravis = gravis;
    }
}

/* nuntius cum titulo intexto (litterae in piscina) */
interior constans character*
_nuntius_forma (Piscina* piscina, constans character* forma,
    chorda intextum, constans character* cauda)
{
    character* buf = (character*)piscina_allocare(piscina,
        strlen(forma) + (memoriae_index)intextum.mensura
        + strlen(cauda) + (memoriae_index)XVI);

    si (buf == NIHIL)
    {
        redde forma;
    }
    sprintf(buf, forma, (integer)intextum.mensura, intextum.datum,
        cauda);
    redde buf;
}

b32
gesta_salutem_aestimare (GestaMundus* mundus,
    constans character* res_id, Piscina* piscina,
    GestaSalus* exitus)
{
    GestaResOrdo ordo;
    JsonValor* st = NIHIL;
    JsonValor* genus_radix = NIHIL;
    Xar* querelae;

    si (mundus == NIHIL || res_id == NIHIL || piscina == NIHIL
        || exitus == NIHIL)
    {
        redde FALSUM;
    }
    exitus->sanus = VERUM;
    exitus->querelae = NIHIL;
    exitus->numerus = ZEPHYRUM;
    ordo = _res_capere(mundus, _ch(res_id), piscina);
    si (!ordo.exsistit)
    {
        redde _fractum(mundus, "res ignota ad salutem");
    }
    querelae = xar_creare(piscina, (i32)magnitudo(GestaQuerela));
    si (querelae == NIHIL)
    {
        redde _fractum(mundus, "piscina exhausta");
    }
    si (ordo.datum.mensura > ZEPHYRUM)
    {
        JsonResultus r = json_legere(ordo.datum, piscina);

        si (r.successus && json_est_objectum(r.radix))
        {
            st = r.radix;
        }
    }
    si (ordo.genus.mensura > ZEPHYRUM)
    {
        chorda gd = _genus_datum_capere(mundus, ordo.genus,
            piscina);

        si (gd.mensura > ZEPHYRUM)
        {
            JsonResultus r = json_legere(gd, piscina);

            si (r.successus && json_est_objectum(r.radix))
            {
                genus_radix = r.radix;
            }
        }
    }
    /* genus ignotum = nihil iudicandum (TS getGenusDef iacit -
     * nos praeterimus, divergentia mollis sub D4 spiritu) */
    si (genus_radix == NIHIL)
    {
        redde VERUM;
    }

    /* I-II. attributa necessaria + typi (TS :4374-4400; LEX
     * CHORDAE VACUAE :4378 - "" pro absente numeratur, et typum
     * NON iudicat :4391) */
    {
        JsonValor* attributa = json_objectum_capere(genus_radix,
            "attributa");

        si (attributa != NIHIL && json_est_tabulatum(attributa))
        {
            i32 i;

            per (i = ZEPHYRUM;
                 i < json_tabulatum_numerus(attributa); i++)
            {
                JsonValor* a = json_tabulatum_obtinere(attributa,
                    i);
                JsonValor* v_tit;
                JsonValor* v_typ;
                JsonValor* v_nec;
                JsonValor* v;
                chorda c_tit;
                chorda c_typ;
                b32 vacua_chorda;
                b32 absens;

                si (a == NIHIL || !json_est_objectum(a))
                {
                    perge;
                }
                v_tit = json_objectum_capere(a, "titulus");
                si (v_tit == NIHIL || !json_est_chorda(v_tit))
                {
                    perge;
                }
                c_tit = json_ad_chorda(v_tit);
                v_typ = json_objectum_capere(a, "typus");
                c_typ = (v_typ != NIHIL && json_est_chorda(v_typ))
                    ? json_ad_chorda(v_typ) : _ch("");
                v_nec = json_objectum_capere(a, "necessarium");
                v = (st != NIHIL)
                    ? json_objectum_capere(st,
                          _litterae(piscina, c_tit))
                    : NIHIL;
                vacua_chorda = (v != NIHIL && json_est_chorda(v)
                    && json_ad_chorda(v).mensura == ZEPHYRUM);
                absens = (v == NIHIL || json_est_nullum(v)
                    || vacua_chorda);
                si (v_nec != NIHIL && json_ad_boolean(v_nec)
                    && absens)
                {
                    _querelam_addere(querelae,
                        "attributum-necessarium-absens",
                        _nuntius_forma(piscina,
                            "attributum necessarium '%.*s'"
                            " abest%s", c_tit, ""),
                        VERUM);
                }
                alioquin si (!absens && c_typ.mensura > ZEPHYRUM
                    && !_typus_validus(v, c_typ))
                {
                    _querelam_addere(querelae,
                        "typus-attributi-pravus",
                        _nuntius_forma(piscina,
                            "attributum '%.*s' typum suum"
                            " violat%s", c_tit, ""),
                        FALSUM);
                }
            }
        }
    }

    /* III. status ignotus (TS :4402-4412; status absens aut genus
     * sine machina = praeteritur) */
    {
        JsonValor* v_st = (st != NIHIL)
            ? json_objectum_capere(st, "status") : NIHIL;
        JsonValor* machina = json_objectum_capere(genus_radix,
            "machina");

        si (v_st != NIHIL && json_est_chorda(v_st)
            && machina != NIHIL && json_est_tabulatum(machina)
            && json_tabulatum_numerus(machina) > ZEPHYRUM)
        {
            chorda cs = json_ad_chorda(v_st);
            b32 inventum = FALSUM;
            JsonValor* si_init = json_objectum_capere(genus_radix,
                "status_initialis");
            i32 i;

            si (si_init != NIHIL && json_est_chorda(si_init)
                && _chordae_pares(json_ad_chorda(si_init), cs))
            {
                inventum = VERUM;
            }
            per (i = ZEPHYRUM;
                 !inventum && i < json_tabulatum_numerus(machina);
                 i++)
            {
                JsonValor* par = json_tabulatum_obtinere(machina,
                    i);
                i32 j;

                si (par == NIHIL || !json_est_tabulatum(par))
                {
                    perge;
                }
                per (j = ZEPHYRUM;
                     j < json_tabulatum_numerus(par); j++)
                {
                    JsonValor* fin = json_tabulatum_obtinere(par,
                        j);

                    si (fin != NIHIL && json_est_chorda(fin)
                        && _chordae_pares(json_ad_chorda(fin),
                               cs))
                    {
                        inventum = VERUM;
                        frange;
                    }
                }
            }
            si (!inventum)
            {
                _querelam_addere(querelae, "status-ignotus",
                    _nuntius_forma(piscina,
                        "status '%.*s' extra machinam generis%s",
                        cs, ""),
                    VERUM);
            }
        }
    }

    /* IV. cardinalitas (species nexus solum; limen inferius ET
     * tectum contra membra stantia - K2 spec par VII) */
    si (_species_nexus_est(mundus, ordo.genus, piscina))
    {
        JsonValor* partes = json_objectum_capere(genus_radix,
            "partes");

        si (partes != NIHIL && json_est_tabulatum(partes))
        {
            JsonValor* membra = (st != NIHIL)
                ? json_objectum_capere(st, "membra") : NIHIL;
            i32 i;

            per (i = ZEPHYRUM;
                 i < json_tabulatum_numerus(partes); i++)
            {
                JsonValor* p = json_tabulatum_obtinere(partes, i);
                JsonValor* v_tit;
                JsonValor* v_card;
                JsonValor* acies;
                chorda c_tit;
                chorda c_card;
                i32 n = ZEPHYRUM;
                b32 violata;

                si (p == NIHIL || !json_est_objectum(p))
                {
                    perge;
                }
                v_tit = json_objectum_capere(p, "titulus");
                si (v_tit == NIHIL || !json_est_chorda(v_tit))
                {
                    perge;
                }
                c_tit = json_ad_chorda(v_tit);
                v_card = json_objectum_capere(p, "cardinalitas");
                c_card = (v_card != NIHIL
                    && json_est_chorda(v_card))
                    ? json_ad_chorda(v_card) : _ch("quotlibet");
                acies = (membra != NIHIL
                    && json_est_objectum(membra))
                    ? json_objectum_capere(membra,
                          _litterae(piscina, c_tit))
                    : NIHIL;
                si (acies != NIHIL && json_est_tabulatum(acies))
                {
                    n = json_tabulatum_numerus(acies);
                }
                violata = (_chorda_est(c_card, "unicus")
                        && n != I)
                    || (_chorda_est(c_card, "aliquot")
                        && n < I);
                si (violata)
                {
                    _querelam_addere(querelae,
                        "cardinalitas-violata",
                        _nuntius_forma(piscina,
                            "cardinalitas partis '%.*s'"
                            " violata%s", c_tit, ""),
                        VERUM);
                }
            }
        }
    }

    /* expositio: tabulatum contiguum in piscina */
    exitus->numerus = xar_numerus(querelae);
    exitus->sanus = (exitus->numerus == ZEPHYRUM);
    si (exitus->numerus > ZEPHYRUM)
    {
        i32 i;

        exitus->querelae = (GestaQuerela*)piscina_allocare(piscina,
            (memoriae_index)exitus->numerus
            * magnitudo(GestaQuerela));
        si (exitus->querelae == NIHIL)
        {
            exitus->numerus = ZEPHYRUM;
            redde _fractum(mundus, "piscina exhausta");
        }
        per (i = ZEPHYRUM; i < exitus->numerus; i++)
        {
            exitus->querelae[i] = *(GestaQuerela*)xar_obtinere(
                querelae, i);
        }
    }
    redde VERUM;
}

Xar*
gesta_insalubres_enumerare (GestaMundus* mundus,
    constans character* genus, Piscina* piscina)
{
    Xar* fructus;
    Xar* omnes;
    ScriniumEnuntiatum* e;
    i32 i;

    si (mundus == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina,
        (i32)magnitudo(GestaInsalubris));
    omnes = xar_creare(piscina, (i32)magnitudo(chorda));
    si (fructus == NIHIL || omnes == NIHIL)
    {
        redde NIHIL;
    }
    /* res colligere primum (exemplar exhaustionis FTS) */
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT res_id FROM res WHERE (?1 = '' OR genus = ?1)"
        " ORDER BY res_id");
    si (e == NIHIL)
    {
        redde fructus;
    }
    scrinium_ligare_textum(e, I, _ch(genus != NIHIL ? genus : ""));
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        chorda* locus = (chorda*)xar_addere(omnes);

        si (locus != NIHIL)
        {
            *locus = scrinium_columna_textus(e, 0, piscina);
        }
    }
    scrinium_finire(e);
    per (i = ZEPHYRUM; i < xar_numerus(omnes); i++)
    {
        chorda res_id = *(chorda*)xar_obtinere(omnes, i);
        GestaSalus salus;

        si (gesta_salutem_aestimare(mundus,
                _litterae(piscina, res_id), piscina, &salus)
            && !salus.sanus)
        {
            GestaInsalubris* ins = (GestaInsalubris*)xar_addere(
                fructus);

            si (ins != NIHIL)
            {
                ins->res_id = res_id;
                ins->salus = salus;
            }
        }
    }
    redde fructus;
}

/* ==================================================
 * Actiones (K3 chunk A) - recepta ut data
 * Oraculum: smaragda.ts:3180-3673 (defineActionGenus,
 * executeAction, _substituteParams :3376-3391). Divergentiae:
 * D1 (executio = eventus in flumine actionis, non tabulae
 * laterales), D3 (effectus scribunt-non-obstant intra fascem
 * portatum), D4 ($novus - TS creare-tunc-referre nequit).
 * ================================================== */

/* res creatae actionis currentis ('' titulus = innominata) */
nomen structura {
    chorda titulus;   /* 'ut' effectūs creationis */
    chorda res_id;
} GestaResNova;

/* contextus substitutionis signorum */
nomen structura {
    JsonValor* ligamina;    /* obiectum titulus -> res_id */
    JsonValor* argumenta;   /* obiectum titulus -> valor */
    Xar*       novae;       /* GestaResNova, ordine effectuum */
    chorda     nunc;        /* creatum fascis */
} GestaSubstitutio;

interior chorda
_novam_invenire (constans GestaSubstitutio* s, chorda titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(s->novae); i++)
    {
        GestaResNova* n = (GestaResNova*)xar_obtinere(s->novae,
            i);

        si (n != NIHIL && n->titulus.mensura > ZEPHYRUM
            && _chordae_pares(n->titulus, titulus))
        {
            redde n->res_id;
        }
    }
    redde _ch("");
}

interior b32
_littera_signi (i8 c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || c == '_' || c == '-';
}

interior b32
_praefixum_ad (chorda s, i32 pos, constans character* lit)
{
    memoriae_index n = strlen(lit);

    redde (memoriae_index)(s.mensura - pos) >= n
        && memcmp(s.datum + pos, lit, n) == ZEPHYRUM;
}

/* signum ad positionem solvere ('$' iam visum). *longitudo_out =
 * octeti signi consumpti (0 = non signum, '$' litteralis manet).
 * NIHIL cum *querela_out posito = signum notum sed inresolutum
 * (error mechanicus recepti). Vocans *querela_out NIHIL initiat. */
interior JsonValor*
_signum_solvere (Piscina* piscina, constans GestaSubstitutio* subst,
    chorda s, i32 pos, i32* longitudo_out,
    constans character** querela_out)
{
    chorda titulus;
    i32 initium;
    i32 finis;

    *longitudo_out = ZEPHYRUM;

    /* $nunc - sine nomine sequente */
    si (_praefixum_ad(s, pos, "$nunc")
        && (pos + V >= s.mensura
            || !_littera_signi(s.datum[pos + V])))
    {
        *longitudo_out = V;
        redde json_chorda_creare(piscina, subst->nunc);
    }
    si (_praefixum_ad(s, pos, "$arg.")
        || _praefixum_ad(s, pos, "$res.")
        || _praefixum_ad(s, pos, "$ops."))
    {
        initium = pos + V;
    }
    alioquin si (_praefixum_ad(s, pos, "$novus."))
    {
        initium = pos + VII;
    }
    alioquin
    {
        redde NIHIL;   /* '$' litteralis */
    }
    finis = initium;
    dum (finis < s.mensura && _littera_signi(s.datum[finis]))
    {
        finis++;
    }
    si (finis == initium)
    {
        redde NIHIL;   /* "$arg." sine nomine - litteralis */
    }
    titulus.datum = s.datum + initium;
    titulus.mensura = finis - initium;
    *longitudo_out = finis - pos;

    si (s.datum[pos + I] == 'a')   /* $arg.X */
    {
        JsonValor* v = json_objectum_capere(subst->argumenta,
            _litterae(piscina, titulus));

        si (v == NIHIL)
        {
            *querela_out = _nuntius_forma(piscina,
                "recusata: signum $arg.%.*s inresolutum%s",
                titulus, "");
        }
        redde v;
    }
    si (s.datum[pos + I] == 'r' || s.datum[pos + I] == 'o')
    {
        /* $res.X (stratum actionis) / $ops.X (stratum processūs) -
         * ambo ligamina legunt (K3 spec par V: substitutio duabus
         * stratis) */
        JsonValor* v = json_objectum_capere(subst->ligamina,
            _litterae(piscina, titulus));

        si (v == NIHIL || !json_est_chorda(v))
        {
            *querela_out = _nuntius_forma(piscina,
                "recusata: signum ligaminis %.*s inresolutum%s",
                titulus, "");
            redde NIHIL;
        }
        redde v;
    }
    /* $novus.N - creatio 'ut' PRIORE tantum (referentia antrorsum
     * = novae nondum impletae = error mechanicus) */
    {
        chorda nova = _novam_invenire(subst, titulus);

        si (nova.mensura == ZEPHYRUM)
        {
            *querela_out = _nuntius_forma(piscina,
                "recusata: signum $novus.%.*s inresolutum"
                " (creatio 'ut' prior deest)%s", titulus, "");
            redde NIHIL;
        }
        redde json_chorda_creare(piscina, nova);
    }
}

/* chordam substituere: signum solum = typus JSON servatur;
 * intextum = valores alieni ut JSON coercentur (paritas TS
 * :3376-3391). NIHIL = querela posita. */
interior JsonValor*
_chordam_substituere (Piscina* piscina,
    constans GestaSubstitutio* subst, chorda s,
    constans character** querela_out)
{
    ChordaAedificator* aed;
    i32 pos = ZEPHYRUM;
    i32 initium_litterae = ZEPHYRUM;

    /* signum solum */
    si (s.mensura > ZEPHYRUM && s.datum[ZEPHYRUM] == '$')
    {
        i32 longitudo = ZEPHYRUM;
        JsonValor* v = _signum_solvere(piscina, subst, s,
            ZEPHYRUM, &longitudo, querela_out);

        si (*querela_out != NIHIL)
        {
            redde NIHIL;
        }
        si (v != NIHIL && longitudo == s.mensura)
        {
            redde v;
        }
    }
    /* intextum */
    aed = chorda_aedificator_creare(piscina, s.mensura + XVI);
    dum (pos < s.mensura)
    {
        si (s.datum[pos] == '$')
        {
            i32 longitudo = ZEPHYRUM;
            JsonValor* v = _signum_solvere(piscina, subst, s, pos,
                &longitudo, querela_out);

            si (*querela_out != NIHIL)
            {
                redde NIHIL;
            }
            si (v != NIHIL)
            {
                chorda ante;

                ante.datum = s.datum + initium_litterae;
                ante.mensura = pos - initium_litterae;
                chorda_aedificator_appendere_chorda(aed, ante);
                si (json_est_chorda(v))
                {
                    chorda_aedificator_appendere_chorda(aed,
                        json_ad_chorda(v));
                }
                alioquin
                {
                    chorda_aedificator_appendere_chorda(aed,
                        json_scribere(v, piscina));
                }
                pos += longitudo;
                initium_litterae = pos;
                perge;
            }
        }
        pos++;
    }
    {
        chorda cauda;

        cauda.datum = s.datum + initium_litterae;
        cauda.mensura = s.mensura - initium_litterae;
        chorda_aedificator_appendere_chorda(aed, cauda);
    }
    redde json_chorda_creare(piscina,
        chorda_aedificator_finire(aed));
}

/* valorem recursive substituere (chordae; obiecta/tabulata
 * descenduntur; claves NON substituuntur) */
interior JsonValor*
_valorem_substituere (Piscina* piscina,
    constans GestaSubstitutio* subst, JsonValor* v,
    constans character** querela_out)
{
    si (json_est_chorda(v))
    {
        redde _chordam_substituere(piscina, subst,
            json_ad_chorda(v), querela_out);
    }
    si (json_est_objectum(v))
    {
        JsonValor* novum = json_objectum_creare(piscina);
        JsonObjectumIterator iter = json_objectum_iterator(v);
        chorda k;
        JsonValor* filius;

        dum (json_objectum_iterator_proxima(&iter, &k, &filius))
        {
            JsonValor* f = _valorem_substituere(piscina, subst,
                filius, querela_out);

            si (f == NIHIL)
            {
                redde NIHIL;
            }
            json_objectum_ponere_chorda(novum, k, f);
        }
        redde novum;
    }
    si (json_est_tabulatum(v))
    {
        JsonValor* novum = json_tabulatum_creare(piscina);
        i32 i;

        per (i = ZEPHYRUM; i < json_tabulatum_numerus(v); i++)
        {
            JsonValor* f = _valorem_substituere(piscina, subst,
                json_tabulatum_obtinere(v, i), querela_out);

            si (f == NIHIL)
            {
                redde NIHIL;
            }
            json_tabulatum_addere(novum, f);
        }
        redde novum;
    }
    redde v;
}

/* porta recepti: opes + argumenta contra tabulas stantes.
 * Litterae causae aut NIHIL = aperta. PORTA OBSTAT (decisio 2 -
 * recusatio est receptum negans, non machinam mentiri). */
interior constans character*
_portam_probare (GestaMundus* m, JsonValor* def_radix,
    JsonValor* ligamina, JsonValor* argumenta, Piscina* piscina)
{
    JsonValor* opes = json_objectum_capere(def_radix, "opes");
    JsonValor* args = json_objectum_capere(def_radix, "argumenta");
    i32 i;

    si (opes != NIHIL && json_est_tabulatum(opes))
    {
        per (i = ZEPHYRUM; i < json_tabulatum_numerus(opes); i++)
        {
            JsonValor* ops = json_tabulatum_obtinere(opes, i);
            JsonValor* v_tit;
            JsonValor* v_gen;
            JsonValor* v_st;
            JsonValor* lig;
            chorda c_tit;
            GestaResOrdo ordo;

            si (ops == NIHIL || !json_est_objectum(ops))
            {
                perge;
            }
            v_tit = json_objectum_capere(ops, "titulus");
            si (v_tit == NIHIL || !json_est_chorda(v_tit))
            {
                perge;
            }
            c_tit = json_ad_chorda(v_tit);
            lig = json_objectum_capere(ligamina,
                _litterae(piscina, c_tit));
            si (lig == NIHIL || !json_est_chorda(lig))
            {
                redde _nuntius_forma(piscina,
                    "recusata: ligamen '%.*s' absens%s", c_tit,
                    "");
            }
            ordo = _res_capere(m, json_ad_chorda(lig), piscina);
            si (!ordo.exsistit)
            {
                redde _nuntius_forma(piscina,
                    "recusata: ops '%.*s' inexistens%s", c_tit,
                    "");
            }
            v_gen = json_objectum_capere(ops, "genus");
            si (v_gen != NIHIL && json_est_chorda(v_gen)
                && !_chordae_pares(json_ad_chorda(v_gen),
                       ordo.genus))
            {
                redde _nuntius_forma(piscina,
                    "recusata: genus opis '%.*s' non"
                    " congruit%s", c_tit, "");
            }
            v_st = json_objectum_capere(ops,
                "status_necessarius");
            si (v_st != NIHIL && json_est_chorda(v_st)
                && !_chordae_pares(json_ad_chorda(v_st),
                       ordo.status))
            {
                redde _nuntius_forma(piscina,
                    "recusata: status opis '%.*s' non"
                    " congruit%s", c_tit, "");
            }
        }
    }
    si (args != NIHIL && json_est_tabulatum(args))
    {
        per (i = ZEPHYRUM; i < json_tabulatum_numerus(args); i++)
        {
            JsonValor* a = json_tabulatum_obtinere(args, i);
            JsonValor* v_tit;
            JsonValor* v_typ;
            JsonValor* v_nec;
            JsonValor* v;
            chorda c_tit;

            si (a == NIHIL || !json_est_objectum(a))
            {
                perge;
            }
            v_tit = json_objectum_capere(a, "titulus");
            si (v_tit == NIHIL || !json_est_chorda(v_tit))
            {
                perge;
            }
            c_tit = json_ad_chorda(v_tit);
            v = json_objectum_capere(argumenta,
                _litterae(piscina, c_tit));
            v_nec = json_objectum_capere(a, "necessarium");
            si (v_nec != NIHIL && json_ad_boolean(v_nec)
                && (v == NIHIL || json_est_nullum(v)))
            {
                redde _nuntius_forma(piscina,
                    "recusata: argumentum necessarium '%.*s'"
                    " abest%s", c_tit, "");
            }
            v_typ = json_objectum_capere(a, "typus");
            si (v != NIHIL && !json_est_nullum(v)
                && v_typ != NIHIL && json_est_chorda(v_typ)
                && !_typus_validus(v, json_ad_chorda(v_typ)))
            {
                redde _nuntius_forma(piscina,
                    "recusata: argumentum '%.*s' typum"
                    " violat%s", c_tit, "");
            }
        }
    }
    redde NIHIL;
}

/* actio-recusata in flumine rei actionis - tabularium
 * attentatuum integrum (portae recusatae ET errores mechanici,
 * interrogatio Q8) */
interior vacuum
_actionem_recusare (GestaMundus* m, constans character* titulus,
    JsonValor* ligamina, JsonValor* argumenta,
    constans character* causa, constans character* actor,
    constans character* origo, Piscina* piscina,
    GestaActioFructus* exitus)
{
    JsonValor* d = json_objectum_creare(piscina);
    GestaFascisEventum unus;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    json_objectum_ponere(d, "ligamina", ligamina);
    json_objectum_ponere(d, "argumenta", argumenta);
    json_objectum_ponere(d, "causa",
        json_chorda_creare(piscina, _ch(causa)));
    unus.event_id = NIHIL;
    unus.eventum.res_id = titulus;
    unus.eventum.genus_eventus = "actio-recusata";
    unus.eventum.datum = _litterae(piscina,
        json_scribere(d, piscina));
    unus.eventum.actor = actor;
    unus.eventum.origo = origo;
    si (_fascis_scribere(m, &unus, I, NIHIL, vacua, NIHIL))
    {
        (vacuum)gesta_plicare(m);
    }
    exitus->facta = FALSUM;
    exitus->causa = causa;
}

/* effectus in fascem aedificare: verbum probare, signa
 * substituere, ULIDs creationum cudere ($novus ordine crescit -
 * referentia antrorsum cadit naturaliter), ids eventuum
 * prae-cudere (E2 B4 - actio-facta eos ANTE scripturam nominat).
 * Causa mechanica aut NIHIL. */
interior constans character*
_effectus_aedificare (JsonValor* effectus, i32 n_eff,
    GestaSubstitutio* subst, constans character* actor,
    constans character* origo, GestaFascisEventum* fascis,
    JsonValor* eventus_ids, Piscina* piscina)
{
    i32 i;

    per (i = ZEPHYRUM; i < n_eff; i++)
    {
        JsonValor* eff = json_tabulatum_obtinere(effectus, i);
        JsonValor* v_verbum;
        JsonValor* v_datum;
        JsonValor* datum_subst;
        chorda c_verbum;
        chorda res_ev;
        constans character* querela = NIHIL;

        si (eff == NIHIL || !json_est_objectum(eff))
        {
            redde "recusata: effectus malformatus";
        }
        v_verbum = json_objectum_capere(eff, "verbum");
        si (v_verbum == NIHIL || !json_est_chorda(v_verbum))
        {
            redde "recusata: effectus sine verbo";
        }
        c_verbum = json_ad_chorda(v_verbum);
        si (!(_chorda_est(c_verbum, "creatio")
            || _chorda_est(c_verbum, "mutatio")
            || _chorda_est(c_verbum, "remotio")
            || _chorda_est(c_verbum, "status")
            || _chorda_est(c_verbum, "nota")
            || _chorda_est(c_verbum, "membrum-additum")
            || _chorda_est(c_verbum, "membrum-remotum")))
        {
            redde _nuntius_forma(piscina,
                "recusata: verbum ignotum '%.*s'%s", c_verbum,
                "");
        }
        v_datum = json_objectum_capere(eff, "datum");
        si (v_datum == NIHIL || json_est_nullum(v_datum))
        {
            datum_subst = json_objectum_creare(piscina);
        }
        alioquin
        {
            datum_subst = _valorem_substituere(piscina, subst,
                v_datum, &querela);
            si (datum_subst == NIHIL)
            {
                redde querela;
            }
            si (!json_est_objectum(datum_subst))
            {
                redde "recusata: datum effectus non obiectum";
            }
        }
        si (_chorda_est(c_verbum, "creatio"))
        {
            JsonValor* v_ut = json_objectum_capere(eff, "ut");
            GestaResNova* nova;
            character ulid[SCRINIUM_ULID_MENSURA];

            scrinium_ulid(ulid);
            res_ev = _ch(_litterae(piscina, _ch(ulid)));
            si (v_ut != NIHIL && json_est_chorda(v_ut)
                && json_ad_chorda(v_ut).mensura > ZEPHYRUM)
            {
                chorda c_ut = json_ad_chorda(v_ut);

                si (_novam_invenire(subst, c_ut).mensura
                    > ZEPHYRUM)
                {
                    redde _nuntius_forma(piscina,
                        "recusata: 'ut' duplicatum '%.*s'%s",
                        c_ut, "");
                }
                nova = (GestaResNova*)xar_addere(subst->novae);
                si (nova != NIHIL)
                {
                    nova->titulus = c_ut;
                    nova->res_id = res_ev;
                }
            }
            alioquin
            {
                nova = (GestaResNova*)xar_addere(subst->novae);
                si (nova != NIHIL)
                {
                    nova->titulus = _ch("");
                    nova->res_id = res_ev;
                }
            }
        }
        alioquin
        {
            JsonValor* v_res = json_objectum_capere(eff, "res");
            JsonValor* res_subst;

            si (v_res == NIHIL || !json_est_chorda(v_res))
            {
                redde "recusata: effectus sine re";
            }
            res_subst = _chordam_substituere(piscina, subst,
                json_ad_chorda(v_res), &querela);
            si (res_subst == NIHIL)
            {
                redde querela;
            }
            si (!json_est_chorda(res_subst))
            {
                redde "recusata: res effectus non chorda";
            }
            res_ev = json_ad_chorda(res_subst);
        }
        {
            character uev[SCRINIUM_ULID_MENSURA];
            constans character* id_l;

            scrinium_ulid(uev);
            id_l = _litterae(piscina, _ch(uev));
            json_tabulatum_addere(eventus_ids,
                json_chorda_creare(piscina, _ch(id_l)));
            fascis[i].event_id = id_l;
            fascis[i].eventum.res_id = _litterae(piscina, res_ev);
            fascis[i].eventum.genus_eventus = _litterae(piscina,
                c_verbum);
            fascis[i].eventum.datum = _litterae(piscina,
                json_scribere(datum_subst, piscina));
            fascis[i].eventum.actor = actor;
            fascis[i].eventum.origo = origo;
        }
    }
    redde NIHIL;
}

b32
gesta_agere (GestaMundus* mundus, constans character* actio_titulus,
    constans character* ligamina_json,
    constans character* argumenta_json, constans character* actor,
    Piscina* piscina, GestaActioFructus* exitus)
{
    constans character* actor_l;
    chorda def_datum;
    JsonResultus r;
    JsonValor* def_radix;
    JsonValor* ligamina;
    JsonValor* argumenta;
    JsonValor* effectus;
    JsonValor* eventus_ids;
    constans character* origo;
    constans character* causa;
    GestaSubstitutio subst;
    GestaFascisEventum* fascis;
    chorda nunc;
    i32 n_eff;
    i32 i;

    si (mundus == NIHIL || actio_titulus == NIHIL
        || piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(GestaActioFructus));
    exitus->causa = "";
    exitus->facta_id = "";
    actor_l = actor != NIHIL ? actor : "machina";

    /* definitio: genus speciei actio */
    def_datum = _genus_datum_capere(mundus, _ch(actio_titulus),
        piscina);
    si (def_datum.mensura == ZEPHYRUM)
    {
        redde _fractum(mundus, "actio ignota (definitio-generis"
            " deest)");
    }
    r = json_legere(def_datum, piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde _fractum(mundus, "definitio actionis malformata");
    }
    def_radix = r.radix;
    {
        JsonValor* species = json_objectum_capere(def_radix,
            "species");

        si (species == NIHIL || !json_est_chorda(species)
            || !_chorda_est(json_ad_chorda(species), "actio"))
        {
            redde _fractum(mundus, "genus non actio");
        }
    }
    r = json_legere_literis(ligamina_json != NIHIL
        ? ligamina_json : "{}", piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde _fractum(mundus, "ligamina malformata");
    }
    ligamina = r.radix;
    r = json_legere_literis(argumenta_json != NIHIL
        ? argumenta_json : "{}", piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde _fractum(mundus, "argumenta malformata");
    }
    argumenta = r.radix;
    {
        character* buf = (character*)piscina_allocare(piscina,
            strlen(actio_titulus) + VII);

        si (buf == NIHIL)
        {
            redde _fractum(mundus, "piscina exhausta");
        }
        sprintf(buf, "actio:%s", actio_titulus);
        origo = buf;
    }

    /* PORTA - obstat */
    causa = _portam_probare(mundus, def_radix, ligamina,
        argumenta, piscina);
    si (causa != NIHIL)
    {
        _actionem_recusare(mundus, actio_titulus, ligamina,
            argumenta, causa, actor_l, origo, piscina, exitus);
        redde VERUM;
    }

    /* substitutio + aedificatio fascis */
    nunc = _nunc_capere(mundus);
    subst.ligamina = ligamina;
    subst.argumenta = argumenta;
    subst.novae = xar_creare(piscina,
        (i32)magnitudo(GestaResNova));
    subst.nunc = nunc;
    effectus = json_objectum_capere(def_radix, "effectus");
    n_eff = (effectus != NIHIL && json_est_tabulatum(effectus))
        ? json_tabulatum_numerus(effectus) : ZEPHYRUM;
    fascis = (GestaFascisEventum*)piscina_allocare(piscina,
        (memoriae_index)(n_eff + I)
        * magnitudo(GestaFascisEventum));
    eventus_ids = json_tabulatum_creare(piscina);
    si (subst.novae == NIHIL || fascis == NIHIL
        || eventus_ids == NIHIL)
    {
        redde _fractum(mundus, "piscina exhausta");
    }
    causa = _effectus_aedificare(effectus, n_eff, &subst, actor_l,
        origo, fascis, eventus_ids, piscina);
    si (causa != NIHIL)
    {
        /* error mechanicus: nihil scriptum - recusata sola */
        _actionem_recusare(mundus, actio_titulus, ligamina,
            argumenta, causa, actor_l, origo, piscina, exitus);
        exitus->mechanica = VERUM;
        redde VERUM;
    }

    /* actio-facta postrema: index executionis in flumine
     * actionis, ids eventuum prae-cusos nominans (E2 B4); id
     * eventus ipsius quoque prae-cusum (motor processuum eum in
     * actio-acta nominat) */
    {
        JsonValor* d = json_objectum_creare(piscina);
        character uev[SCRINIUM_ULID_MENSURA];

        json_objectum_ponere(d, "ligamina", ligamina);
        json_objectum_ponere(d, "argumenta", argumenta);
        json_objectum_ponere(d, "eventus", eventus_ids);
        scrinium_ulid(uev);
        exitus->facta_id = _litterae(piscina, _ch(uev));
        fascis[n_eff].event_id = exitus->facta_id;
        fascis[n_eff].eventum.res_id = actio_titulus;
        fascis[n_eff].eventum.genus_eventus = "actio-facta";
        fascis[n_eff].eventum.datum = _litterae(piscina,
            json_scribere(d, piscina));
        fascis[n_eff].eventum.actor = actor_l;
        fascis[n_eff].eventum.origo = origo;
    }

    si (!_fascis_scribere(mundus, fascis, n_eff + I, NIHIL, nunc,
            NIHIL))
    {
        /* apparatus in scriptura fractus - fascis totus
         * revolutus, error mundi iam positus */
        exitus->causa = gesta_error(mundus);
        redde FALSUM;
    }
    si (!gesta_plicare(mundus))
    {
        redde FALSUM;
    }
    /* excitatio: effectus status membra instantiarum currentium
     * provehunt (K3 chunk B; custos re-entrantiae intus tacet -
     * ansa exterior fixum suum recomputat) */
    per (i = ZEPHYRUM; i < n_eff; i++)
    {
        si (strcmp(fascis[i].eventum.genus_eventus, "status")
            == ZEPHYRUM)
        {
            _provectionem_excitare(mundus,
                fascis[i].eventum.res_id);
        }
    }

    exitus->facta = VERUM;
    exitus->novae_numerus = xar_numerus(subst.novae);
    si (exitus->novae_numerus > ZEPHYRUM)
    {
        exitus->res_novae = (chorda*)piscina_allocare(piscina,
            (memoriae_index)exitus->novae_numerus
            * magnitudo(chorda));
        si (exitus->res_novae == NIHIL)
        {
            exitus->novae_numerus = ZEPHYRUM;
            redde _fractum(mundus, "piscina exhausta");
        }
        per (i = ZEPHYRUM; i < exitus->novae_numerus; i++)
        {
            exitus->res_novae[i] = ((GestaResNova*)xar_obtinere(
                subst.novae, i))->res_id;
        }
    }
    redde VERUM;
}

Xar*
gesta_actiones_rei (GestaMundus* mundus, constans character* res_id,
    Piscina* piscina)
{
    Xar* fructus;
    GestaResOrdo ordo;
    ScriniumEnuntiatum* e;

    si (mundus == NIHIL || res_id == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(chorda));
    si (fructus == NIHIL)
    {
        redde NIHIL;
    }
    ordo = _res_capere(mundus, _ch(res_id), piscina);
    si (!ordo.exsistit)
    {
        redde fructus;
    }
    /* percursus generum plenus - copiae parvae (E2 par 5: genera
     * VI hodie, IX post semen v3; columna species parcata) */
    e = scrinium_praeparare(mundus->scrinium,
        "SELECT titulus, datum FROM genera ORDER BY titulus");
    si (e == NIHIL)
    {
        redde fructus;
    }
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        chorda titulus = scrinium_columna_textus(e, 0, piscina);
        chorda datum = scrinium_columna_textus(e, I, piscina);
        JsonResultus r = json_legere(datum, piscina);
        JsonValor* species;
        JsonValor* opes;
        i32 i;

        si (!r.successus || !json_est_objectum(r.radix))
        {
            perge;
        }
        species = json_objectum_capere(r.radix, "species");
        si (species == NIHIL || !json_est_chorda(species)
            || !_chorda_est(json_ad_chorda(species), "actio"))
        {
            perge;
        }
        opes = json_objectum_capere(r.radix, "opes");
        si (opes == NIHIL || !json_est_tabulatum(opes))
        {
            perge;
        }
        per (i = ZEPHYRUM; i < json_tabulatum_numerus(opes); i++)
        {
            JsonValor* ops = json_tabulatum_obtinere(opes, i);
            JsonValor* v_gen;
            JsonValor* v_st;

            si (ops == NIHIL || !json_est_objectum(ops))
            {
                perge;
            }
            v_gen = json_objectum_capere(ops, "genus");
            si (v_gen != NIHIL && json_est_chorda(v_gen)
                && !_chordae_pares(json_ad_chorda(v_gen),
                       ordo.genus))
            {
                perge;
            }
            v_st = json_objectum_capere(ops,
                "status_necessarius");
            si (v_st != NIHIL && json_est_chorda(v_st)
                && !_chordae_pares(json_ad_chorda(v_st),
                       ordo.status))
            {
                perge;
            }
            {
                chorda* locus = (chorda*)xar_addere(fructus);

                si (locus != NIHIL)
                {
                    *locus = titulus;
                }
            }
            frange;
        }
    }
    scrinium_finire(e);
    redde fructus;
}

/* ==================================================
 * Processus (K3 chunk B) - orchestratio ut data
 * Oraculum: smaragda.ts:4655-5428 (_advanceProcess 5248-5377 -
 * ambulatio per cursus; processInstanceReducer 4807-4879 - status
 * graduum plicati). Divergentiae maiores: D7 (opera per membra,
 * non attributa - excitatio indicem equitat), D8 (opes/argumenta/
 * $ops AEDIFICATA - oraculum ea non habet, E1 B4), D9
 * (photographia ad ortum - oraculum vivo-iungit, E3 par 2 status
 * absurdos vivos monstravit), D10 (portae obstructae tacent),
 * D11/decisio 16 (actio recusata = gradus pendens retentus,
 * se-sanans), D12 (ansa exhauriendi plana - scrinium BEGIN
 * nidificatum non fert).
 * ================================================== */

/* valor chordae sub clave graduum-tituli (vacua = absens) */
interior chorda
_cursum_capere (Piscina* piscina, JsonValor* cursus,
    chorda gradus_titulus)
{
    si (cursus != NIHIL && json_est_objectum(cursus))
    {
        JsonValor* v = json_objectum_capere(cursus,
            _litterae(piscina, gradus_titulus));

        si (v != NIHIL && json_est_chorda(v))
        {
            redde json_ad_chorda(v);
        }
    }
    redde _ch("");
}

/* valor chordae clavis obiecti (vacua si absens/non chorda) */
interior chorda
_clavis_chorda (JsonValor* obiectum, constans character* clavis)
{
    JsonValor* v = (obiectum != NIHIL
        && json_est_objectum(obiectum))
        ? json_objectum_capere(obiectum, clavis) : NIHIL;

    si (v != NIHIL && json_est_chorda(v))
    {
        redde json_ad_chorda(v);
    }
    redde _ch("");
}

/* eventum instantiae in acervum emissionum ponere */
interior b32
_emissionem_ponere (Piscina* piscina, Xar* emissiones,
    constans character* res_id, constans character* genus_eventus,
    JsonValor* datum)
{
    GestaFascisEventum* e = (GestaFascisEventum*)xar_addere(
        emissiones);

    si (e == NIHIL)
    {
        redde FALSUM;
    }
    e->event_id = NIHIL;
    e->eventum.res_id = res_id;
    e->eventum.genus_eventus = genus_eventus;
    e->eventum.datum = _litterae(piscina,
        json_scribere(datum, piscina));
    e->eventum.actor = "machina";
    e->eventum.origo = "gesta-provectio";
    redde VERUM;
}

interior JsonValor*
_datum_gradus (Piscina* piscina, chorda gradus_titulus)
{
    JsonValor* d = json_objectum_creare(piscina);

    json_objectum_ponere(d, "gradus",
        json_chorda_creare(piscina, gradus_titulus));
    redde d;
}

/* emissiones (Xar segmentatus) scribere: complanatio -> fascis
 * micro + plicatura. VERUM = scriptum. */
interior b32
_emissiones_scribere (GestaMundus* m, Xar* emissiones)
{
    i32 n = xar_numerus(emissiones);
    GestaFascisEventum* plana;
    chorda vacua;
    i32 i;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (n == ZEPHYRUM)
    {
        redde VERUM;
    }
    plana = (GestaFascisEventum*)piscina_allocare(m->piscina,
        (memoriae_index)n * magnitudo(GestaFascisEventum));
    si (plana == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        plana[i] = *(GestaFascisEventum*)xar_obtinere(emissiones,
            i);
    }
    si (!_fascis_scribere(m, plana, n, NIHIL, vacua, NIHIL))
    {
        redde FALSUM;
    }
    redde gesta_plicare(m);
}

/* indices graduum ordinis dati positione ascendente; numerus
 * redditus (insertio - copiae parvae) */
interior i32
_ordinem_colligere (Piscina* piscina, JsonValor* gradus_tab,
    chorda ordo_titulus, i32** indices_out)
{
    i32 n = json_tabulatum_numerus(gradus_tab);
    i32* indices;
    i32 numerus = ZEPHYRUM;
    i32 i;

    indices = (i32*)piscina_allocare(piscina,
        (memoriae_index)(n > ZEPHYRUM ? n : I) * magnitudo(i32));
    *indices_out = indices;
    si (indices == NIHIL)
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        JsonValor* g = json_tabulatum_obtinere(gradus_tab, i);
        JsonValor* v_pos;
        s64 pos;
        i32 j;

        si (g == NIHIL || !json_est_objectum(g)
            || !_chordae_pares(_clavis_chorda(g, "ordo"),
                   ordo_titulus))
        {
            perge;
        }
        v_pos = json_objectum_capere(g, "positio");
        pos = (v_pos != NIHIL && json_est_integer(v_pos))
            ? json_ad_integer(v_pos) : ZEPHYRUM;
        j = numerus;
        dum (j > ZEPHYRUM)
        {
            JsonValor* prior = json_tabulatum_obtinere(gradus_tab,
                indices[j - I]);
            JsonValor* v_pp = json_objectum_capere(prior,
                "positio");
            s64 pp = (v_pp != NIHIL && json_est_integer(v_pp))
                ? json_ad_integer(v_pp) : ZEPHYRUM;

            si (pp <= pos)
            {
                frange;
            }
            indices[j] = indices[j - I];
            j--;
        }
        indices[j] = i;
        numerus++;
    }
    redde numerus;
}

/* titulus in acervo chordarum? (memoria actionum temptatarum) */
interior b32
_temptata_est (Xar* attemptae, chorda titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(attemptae); i++)
    {
        chorda* c = (chorda*)xar_obtinere(attemptae, i);

        si (c != NIHIL && _chordae_pares(*c, titulus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* obiectum JSON (aut vacuum si NIHIL/non obiectum) */
interior JsonValor*
_obiectum_aut_vacuum (Piscina* piscina, JsonValor* v)
{
    si (v != NIHIL && json_est_objectum(v))
    {
        redde v;
    }
    redde json_objectum_creare(piscina);
}

/* ansa exhauriendi ad fixum (E2 par 2 - transactiones planae,
 * NULLA recursio): computare gradus paratos ex plicaturis
 * CURRENTIBUS -> nihil = fixum -> alioquin fascis micro +
 * plicatura -> iterum. Actiones recusatae intra exhaustum unum
 * non re-temptantur (retentio = excitatio proxima, decisio 16).
 * Custos m->provectio_activa re-entrantiam tacet (agere intus
 * effectus status scribit - ansa exterior fixum recomputat). */
interior vacuum
_provehere_ad_fixum (GestaMundus* m, constans character* instantia)
{
    Piscina* piscina = m->piscina;
    Xar* attemptae;
    i32 iterationes = ZEPHYRUM;

    si (m->provectio_activa)
    {
        redde;
    }
    m->provectio_activa = VERUM;
    attemptae = xar_creare(piscina, (i32)magnitudo(chorda));
    si (attemptae == NIHIL)
    {
        m->provectio_activa = FALSUM;
        redde;
    }
    dum (VERUM)
    {
        GestaResOrdo res_ordo = _res_capere(m, _ch(instantia),
            piscina);
        JsonResultus r;
        JsonValor* st;
        JsonValor* gradus_tab;
        JsonValor* ordines;
        JsonValor* cursus;
        JsonValor* opera;
        GestaSubstitutio subst;
        Xar* emissiones;
        b32 aliquis_defectus = FALSUM;
        b32 aliquis_activus = FALSUM;
        b32 omnes_perfecti = VERUM;
        b32 fracta = FALSUM;
        i32 n_gr;
        i32 i;

        si (!res_ordo.exsistit
            || !_chorda_est(res_ordo.status, "currens"))
        {
            frange;
        }
        r = json_legere(res_ordo.datum, piscina);
        si (!r.successus || !json_est_objectum(r.radix))
        {
            frange;
        }
        st = r.radix;
        gradus_tab = json_objectum_capere(st, "gradus");
        ordines = json_objectum_capere(st, "ordines");
        si (gradus_tab == NIHIL || !json_est_tabulatum(gradus_tab)
            || ordines == NIHIL || !json_est_tabulatum(ordines))
        {
            frange;   /* photographia manca - nihil agendum */
        }
        cursus = json_objectum_capere(st, "cursus");
        opera = json_objectum_capere(st, "opera");
        subst.ligamina = _obiectum_aut_vacuum(piscina,
            json_objectum_capere(st, "opes"));
        subst.argumenta = _obiectum_aut_vacuum(piscina,
            json_objectum_capere(st, "argumenta"));
        subst.novae = xar_creare(piscina,
            (i32)magnitudo(GestaResNova));
        subst.nunc = _nunc_capere(m);
        n_gr = json_tabulatum_numerus(gradus_tab);
        emissiones = xar_creare(piscina,
            (i32)magnitudo(GestaFascisEventum));
        si (emissiones == NIHIL || subst.novae == NIHIL)
        {
            frange;
        }

        /* I. status graduum percensere; opera activa perfecta/
         * omissa in gradus-eventus vertere */
        per (i = ZEPHYRUM; i < n_gr; i++)
        {
            JsonValor* g = json_tabulatum_obtinere(gradus_tab, i);
            chorda g_titulus;
            chorda c;

            si (g == NIHIL || !json_est_objectum(g))
            {
                perge;
            }
            g_titulus = _clavis_chorda(g, "titulus");
            c = _cursum_capere(piscina, cursus, g_titulus);
            si (_chorda_est(c, "perfectus"))
            {
                perge;
            }
            omnes_perfecti = FALSUM;
            si (_chorda_est(c, "defectus"))
            {
                aliquis_defectus = VERUM;
                perge;
            }
            si (!_chorda_est(c, "incohatus"))
            {
                perge;   /* pendens - ambulatio II incipit */
            }
            si (_chorda_est(_clavis_chorda(g, "genus_gradus"),
                    "opus"))
            {
                chorda opus_id = _cursum_capere(piscina, opera,
                    g_titulus);
                GestaResOrdo opus_ordo;

                si (opus_id.mensura == ZEPHYRUM)
                {
                    aliquis_activus = VERUM;
                    perge;
                }
                opus_ordo = _res_capere(m, opus_id, piscina);
                si (!opus_ordo.exsistit)
                {
                    aliquis_activus = VERUM;
                    perge;
                }
                si (_chorda_est(opus_ordo.status, "perfectum"))
                {
                    JsonValor* d = _datum_gradus(piscina,
                        g_titulus);
                    JsonResultus ro = json_legere(opus_ordo.datum,
                        piscina);

                    si (ro.successus
                        && json_est_objectum(ro.radix))
                    {
                        JsonValor* eff = json_objectum_capere(
                            ro.radix, "effectus");

                        si (eff != NIHIL && json_est_chorda(eff))
                        {
                            json_objectum_ponere(d, "effectus",
                                eff);
                        }
                    }
                    si (!_emissionem_ponere(piscina, emissiones,
                            instantia, "gradus-perfectus", d))
                    {
                        fracta = VERUM;
                    }
                }
                alioquin si (_chorda_est(opus_ordo.status,
                                "omissum"))
                {
                    JsonValor* d = _datum_gradus(piscina,
                        g_titulus);

                    json_objectum_ponere(d, "causa",
                        json_chorda_creare(piscina,
                            _ch("opus omissum")));
                    si (!_emissionem_ponere(piscina, emissiones,
                            instantia, "gradus-defectus", d))
                    {
                        fracta = VERUM;
                    }
                }
                alioquin
                {
                    aliquis_activus = VERUM;
                }
            }
            alioquin
            {
                aliquis_activus = VERUM;
            }
        }

        /* II. ambulatio per cursus: gradum proximum pendentem
         * incipere conari (paritas oraculi _advanceProcess) */
        per (i = ZEPHYRUM;
             i < json_tabulatum_numerus(ordines) && !fracta; i++)
        {
            JsonValor* v_ordo = json_tabulatum_obtinere(ordines,
                i);
            i32* indices;
            i32 in_ordine;
            i32 j;

            si (v_ordo == NIHIL || !json_est_chorda(v_ordo))
            {
                perge;
            }
            in_ordine = _ordinem_colligere(piscina, gradus_tab,
                json_ad_chorda(v_ordo), &indices);
            per (j = ZEPHYRUM; j < in_ordine; j++)
            {
                JsonValor* g = json_tabulatum_obtinere(gradus_tab,
                    indices[j]);
                chorda g_titulus = _clavis_chorda(g, "titulus");
                chorda g_species = _clavis_chorda(g,
                    "genus_gradus");
                chorda c = _cursum_capere(piscina, cursus,
                    g_titulus);

                si (_chorda_est(c, "perfectus"))
                {
                    perge;   /* proximum in cursu */
                }
                si (c.mensura > ZEPHYRUM)
                {
                    frange;  /* incohatus/defectus - cursus stat */
                }
                /* pendens */
                si (_chorda_est(g_species, "porta"))
                {
                    JsonValor* condiciones = json_objectum_capere(
                        g, "condiciones");
                    b32 apertae = VERUM;

                    si (condiciones != NIHIL
                        && json_est_tabulatum(condiciones))
                    {
                        i32 k;

                        per (k = ZEPHYRUM;
                             k < json_tabulatum_numerus(
                                 condiciones); k++)
                        {
                            JsonValor* cond =
                                json_tabulatum_obtinere(
                                    condiciones, k);

                            si (cond == NIHIL
                                || !json_est_chorda(cond)
                                || !_chorda_est(_cursum_capere(
                                       piscina, cursus,
                                       json_ad_chorda(cond)),
                                       "perfectus"))
                            {
                                apertae = FALSUM;
                                frange;
                            }
                        }
                    }
                    si (apertae)
                    {
                        si (!_emissionem_ponere(piscina,
                                emissiones, instantia,
                                "porta-perfecta",
                                _datum_gradus(piscina,
                                    g_titulus)))
                        {
                            fracta = VERUM;
                        }
                    }
                    /* obstructa: NIHIL emittitur (D10) */
                    frange;
                }
                si (_chorda_est(g_species, "opus"))
                {
                    character ulid[SCRINIUM_ULID_MENSURA];
                    constans character* opus_id;
                    JsonValor* d_opus;
                    JsonValor* d_membrum;
                    JsonValor* d_genitum;
                    chorda t_opus;
                    constans character* querela = NIHIL;
                    JsonValor* t_subst;

                    scrinium_ulid(ulid);
                    opus_id = _litterae(piscina, _ch(ulid));
                    t_opus = _clavis_chorda(g, "opus_titulus");
                    si (t_opus.mensura == ZEPHYRUM)
                    {
                        t_opus = g_titulus;
                    }
                    t_subst = _chordam_substituere(piscina,
                        &subst, t_opus, &querela);
                    si (t_subst == NIHIL)
                    {
                        JsonValor* d = _datum_gradus(piscina,
                            g_titulus);

                        json_objectum_ponere(d, "causa",
                            json_chorda_creare(piscina,
                                _ch(querela)));
                        si (!_emissionem_ponere(piscina,
                                emissiones, instantia,
                                "gradus-defectus", d))
                        {
                            fracta = VERUM;
                        }
                        frange;
                    }
                    d_opus = json_objectum_creare(piscina);
                    json_objectum_ponere(d_opus, "genus",
                        json_chorda_creare(piscina, _ch("opus")));
                    json_objectum_ponere(d_opus, "titulus",
                        t_subst);
                    {
                        chorda descriptio = _clavis_chorda(g,
                            "descriptio");
                        chorda prioritas = _clavis_chorda(g,
                            "prioritas");

                        si (descriptio.mensura > ZEPHYRUM)
                        {
                            JsonValor* ds = _chordam_substituere(
                                piscina, &subst, descriptio,
                                &querela);

                            si (ds != NIHIL)
                            {
                                json_objectum_ponere(d_opus,
                                    "descriptio", ds);
                            }
                        }
                        si (prioritas.mensura > ZEPHYRUM)
                        {
                            json_objectum_ponere(d_opus,
                                "prioritas",
                                json_chorda_creare(piscina,
                                    prioritas));
                        }
                    }
                    d_membrum = json_objectum_creare(piscina);
                    json_objectum_ponere(d_membrum, "pars",
                        json_chorda_creare(piscina, g_titulus));
                    json_objectum_ponere(d_membrum, "membrum",
                        json_chorda_creare(piscina,
                            _ch(opus_id)));
                    d_genitum = _datum_gradus(piscina, g_titulus);
                    json_objectum_ponere(d_genitum, "opus",
                        json_chorda_creare(piscina,
                            _ch(opus_id)));
                    si (!_emissionem_ponere(piscina, emissiones,
                            opus_id, "creatio", d_opus)
                        || !_emissionem_ponere(piscina,
                               emissiones, instantia,
                               "membrum-additum", d_membrum)
                        || !_emissionem_ponere(piscina,
                               emissiones, instantia,
                               "opus-genitum", d_genitum)
                        || !_emissionem_ponere(piscina,
                               emissiones, instantia,
                               "gradus-incohatus",
                               _datum_gradus(piscina,
                                   g_titulus)))
                    {
                        fracta = VERUM;
                    }
                    frange;   /* cursus opus exspectat */
                }
                si (_chorda_est(g_species, "actio"))
                {
                    chorda actio_t = _clavis_chorda(g, "actio");
                    constans character* querela = NIHIL;
                    JsonValor* lig_subst;
                    JsonValor* arg_subst;
                    GestaActioFructus fa;
                    chorda* memoria;

                    si (_temptata_est(attemptae, g_titulus))
                    {
                        frange;   /* recusata hoc exhaustu -
                                   * excitatio proxima */
                    }
                    memoria = (chorda*)xar_addere(attemptae);
                    si (memoria != NIHIL)
                    {
                        *memoria = g_titulus;
                    }
                    lig_subst = _valorem_substituere(piscina,
                        &subst, _obiectum_aut_vacuum(piscina,
                            json_objectum_capere(g, "ligamina")),
                        &querela);
                    arg_subst = (querela == NIHIL)
                        ? _valorem_substituere(piscina, &subst,
                              _obiectum_aut_vacuum(piscina,
                                  json_objectum_capere(g,
                                      "argumenta")), &querela)
                        : NIHIL;
                    si (querela != NIHIL || actio_t.mensura
                            == ZEPHYRUM)
                    {
                        JsonValor* d = _datum_gradus(piscina,
                            g_titulus);

                        json_objectum_ponere(d, "causa",
                            json_chorda_creare(piscina,
                                _ch(querela != NIHIL ? querela
                                    : "gradus actionis sine"
                                      " actione")));
                        si (!_emissionem_ponere(piscina,
                                emissiones, instantia,
                                "gradus-defectus", d))
                        {
                            fracta = VERUM;
                        }
                        frange;
                    }
                    si (!gesta_agere(m,
                            _litterae(piscina, actio_t),
                            _litterae(piscina, json_scribere(
                                lig_subst, piscina)),
                            _litterae(piscina, json_scribere(
                                arg_subst, piscina)),
                            "machina", piscina, &fa))
                    {
                        JsonValor* d = _datum_gradus(piscina,
                            g_titulus);

                        json_objectum_ponere(d, "causa",
                            json_chorda_creare(piscina,
                                _ch(gesta_error(m))));
                        si (!_emissionem_ponere(piscina,
                                emissiones, instantia,
                                "gradus-defectus", d))
                        {
                            fracta = VERUM;
                        }
                        frange;
                    }
                    si (fa.facta)
                    {
                        JsonValor* d_acta = _datum_gradus(piscina,
                            g_titulus);

                        json_objectum_ponere(d_acta, "factum",
                            json_chorda_creare(piscina,
                                _ch(fa.facta_id)));
                        si (!_emissionem_ponere(piscina,
                                emissiones, instantia,
                                "actio-acta", d_acta)
                            || !_emissionem_ponere(piscina,
                                   emissiones, instantia,
                                   "gradus-perfectus",
                                   _datum_gradus(piscina,
                                       g_titulus)))
                        {
                            fracta = VERUM;
                        }
                    }
                    alioquin si (fa.mechanica)
                    {
                        JsonValor* d = _datum_gradus(piscina,
                            g_titulus);

                        json_objectum_ponere(d, "causa",
                            json_chorda_creare(piscina,
                                _ch(fa.causa)));
                        si (!_emissionem_ponere(piscina,
                                emissiones, instantia,
                                "gradus-defectus", d))
                        {
                            fracta = VERUM;
                        }
                    }
                    /* porta actionis recusavit: gradus pendens
                     * manet, NIHIL emittitur (decisio 16) */
                    frange;
                }
                frange;   /* genus gradūs ignotum - cursus stat */
            }
        }

        si (fracta)
        {
            frange;
        }

        /* III. scriptura aut terminatio */
        si (xar_numerus(emissiones) > ZEPHYRUM)
        {
            si (!_emissiones_scribere(m, emissiones))
            {
                frange;
            }
            iterationes++;
            si (iterationes > IV * n_gr + VIII)
            {
                frange;   /* cingulum contra ansam fugientem */
            }
            perge;
        }
        si (omnes_perfecti && n_gr > ZEPHYRUM)
        {
            JsonValor* d_status = json_objectum_creare(piscina);

            json_objectum_ponere(d_status, "novus",
                json_chorda_creare(piscina, _ch("perfectus")));
            si (!_emissionem_ponere(piscina, emissiones,
                    instantia, "processus-perfectus",
                    json_objectum_creare(piscina))
                || !_emissionem_ponere(piscina, emissiones,
                       instantia, "status", d_status))
            {
                frange;
            }
            (vacuum)_emissiones_scribere(m, emissiones);
            frange;
        }
        si (aliquis_defectus && !aliquis_activus)
        {
            /* cascata defectūs (E1 par 7): nihil activum, nihil
             * emissum, aliquis gradus defectus - processus cadit */
            JsonValor* d_status = json_objectum_creare(piscina);

            json_objectum_ponere(d_status, "novus",
                json_chorda_creare(piscina, _ch("defectus")));
            si (!_emissionem_ponere(piscina, emissiones,
                    instantia, "processus-defectus",
                    json_objectum_creare(piscina))
                || !_emissionem_ponere(piscina, emissiones,
                       instantia, "status", d_status))
            {
                frange;
            }
            (vacuum)_emissiones_scribere(m, emissiones);
            frange;
        }
        frange;   /* fixum: mundum exspectat (opus activum aut
                   * actio retenta) */
    }
    m->provectio_activa = FALSUM;
}

/* excitatio: res data membrum instantiae processūs currentis? ->
 * exhaurire. Sonda una super idx_membra_membrum (E2 par 3);
 * candidatae colliguntur ANTE exhaustum (enuntiatum finitum ante
 * scripturas). */
interior vacuum
_provectionem_excitare (GestaMundus* m, constans character* res_id)
{
    Xar* candidatae;
    ScriniumEnuntiatum* e;
    i32 i;

    si (m->provectio_activa)
    {
        redde;   /* ansa exterior fixum suum recomputat */
    }
    candidatae = xar_creare(m->piscina,
        (i32)magnitudo(GestaObumbra));
    si (candidatae == NIHIL)
    {
        redde;
    }
    e = scrinium_praeparare(m->scrinium,
        "SELECT DISTINCT mb.res_id, r.genus FROM membra mb"
        " JOIN res r ON r.res_id = mb.res_id"
        " WHERE mb.membrum = ?1 AND r.status = 'currens'");
    si (e == NIHIL)
    {
        redde;
    }
    scrinium_ligare_textum(e, I, _ch(res_id));
    dum (scrinium_gradi(e) == SCRINIUM_ORDO)
    {
        GestaObumbra* c = (GestaObumbra*)xar_addere(candidatae);

        si (c != NIHIL)
        {
            c->res_id = scrinium_columna_textus(e, 0, m->piscina);
            c->genus = scrinium_columna_textus(e, I, m->piscina);
        }
    }
    scrinium_finire(e);
    per (i = ZEPHYRUM; i < xar_numerus(candidatae); i++)
    {
        GestaObumbra* c = (GestaObumbra*)xar_obtinere(candidatae,
            i);

        si (c != NIHIL
            && _species_est(m, c->genus, "processus", m->piscina))
        {
            _provehere_ad_fixum(m,
                _litterae(m->piscina, c->res_id));
        }
    }
}

b32
gesta_processum_incipere (GestaMundus* mundus,
    constans character* processus_titulus,
    constans character* ligamina_json,
    constans character* argumenta_json, constans character* actor,
    Piscina* piscina, GestaProcessusFructus* exitus)
{
    constans character* actor_l;
    chorda def_datum;
    JsonResultus r;
    JsonValor* def_radix;
    JsonValor* ligamina;
    JsonValor* argumenta;
    constans character* origo;
    constans character* causa;
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    si (mundus == NIHIL || processus_titulus == NIHIL
        || piscina == NIHIL || exitus == NIHIL)
    {
        redde FALSUM;
    }
    memset(exitus, ZEPHYRUM, magnitudo(GestaProcessusFructus));
    exitus->causa = "";
    actor_l = actor != NIHIL ? actor : "machina";

    def_datum = _genus_datum_capere(mundus,
        _ch(processus_titulus), piscina);
    si (def_datum.mensura == ZEPHYRUM)
    {
        redde _fractum(mundus, "processus ignotus"
            " (definitio-generis deest)");
    }
    r = json_legere(def_datum, piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde _fractum(mundus, "definitio processūs malformata");
    }
    def_radix = r.radix;
    {
        JsonValor* species = json_objectum_capere(def_radix,
            "species");

        si (species == NIHIL || !json_est_chorda(species)
            || !_chorda_est(json_ad_chorda(species), "processus"))
        {
            redde _fractum(mundus, "genus non processus");
        }
    }
    r = json_legere_literis(ligamina_json != NIHIL
        ? ligamina_json : "{}", piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde _fractum(mundus, "ligamina malformata");
    }
    ligamina = r.radix;
    r = json_legere_literis(argumenta_json != NIHIL
        ? argumenta_json : "{}", piscina);
    si (!r.successus || !json_est_objectum(r.radix))
    {
        redde _fractum(mundus, "argumenta malformata");
    }
    argumenta = r.radix;
    {
        character* buf = (character*)piscina_allocare(piscina,
            strlen(processus_titulus) + XI);

        si (buf == NIHIL)
        {
            redde _fractum(mundus, "piscina exhausta");
        }
        sprintf(buf, "processus:%s", processus_titulus);
        origo = buf;
    }

    /* PORTA - grammatica eadem ac actionum (D8) */
    causa = _portam_probare(mundus, def_radix, ligamina,
        argumenta, piscina);
    si (causa != NIHIL)
    {
        JsonValor* d = json_objectum_creare(piscina);
        GestaFascisEventum unus;

        json_objectum_ponere(d, "ligamina", ligamina);
        json_objectum_ponere(d, "argumenta", argumenta);
        json_objectum_ponere(d, "causa",
            json_chorda_creare(piscina, _ch(causa)));
        unus.event_id = NIHIL;
        unus.eventum.res_id = processus_titulus;
        unus.eventum.genus_eventus = "processus-recusatus";
        unus.eventum.datum = _litterae(piscina,
            json_scribere(d, piscina));
        unus.eventum.actor = actor_l;
        unus.eventum.origo = origo;
        si (_fascis_scribere(mundus, &unus, I, NIHIL, vacua,
                NIHIL))
        {
            (vacuum)gesta_plicare(mundus);
        }
        exitus->causa = causa;
        redde VERUM;
    }

    /* PHOTOGRAPHIA (D9): definitio in datum instantiae - emendatio
     * generis instantias currentes numquam tangit (G12) */
    {
        JsonValor* d = json_objectum_creare(piscina);
        JsonValor* v_gradus = json_objectum_capere(def_radix,
            "gradus");
        JsonValor* v_ordines = json_objectum_capere(def_radix,
            "ordines");
        character instantia_ulid[SCRINIUM_ULID_MENSURA];
        GestaFascisEventum* fascis;
        i32 ligaminum_numerus = ZEPHYRUM;
        i32 n_fascis;
        JsonObjectumIterator iter;
        chorda k;
        JsonValor* v;
        i32 pos;

        json_objectum_ponere(d, "titulus",
            json_chorda_creare(piscina,
                _ch(processus_titulus)));
        json_objectum_ponere(d, "genus",
            json_chorda_creare(piscina,
                _ch(processus_titulus)));
        json_objectum_ponere(d, "status",
            json_chorda_creare(piscina, _ch("currens")));
        json_objectum_ponere(d, "ordines",
            (v_ordines != NIHIL && json_est_tabulatum(v_ordines))
                ? v_ordines : json_tabulatum_creare(piscina));
        json_objectum_ponere(d, "gradus",
            (v_gradus != NIHIL && json_est_tabulatum(v_gradus))
                ? v_gradus : json_tabulatum_creare(piscina));
        json_objectum_ponere(d, "opes", ligamina);
        json_objectum_ponere(d, "argumenta", argumenta);

        iter = json_objectum_iterator(ligamina);
        dum (json_objectum_iterator_proxima(&iter, &k, &v))
        {
            si (v != NIHIL && json_est_chorda(v))
            {
                ligaminum_numerus++;
            }
        }
        n_fascis = I + ligaminum_numerus;
        fascis = (GestaFascisEventum*)piscina_allocare(piscina,
            (memoriae_index)n_fascis
            * magnitudo(GestaFascisEventum));
        si (fascis == NIHIL)
        {
            redde _fractum(mundus, "piscina exhausta");
        }
        scrinium_ulid(instantia_ulid);
        fascis[ZEPHYRUM].event_id = NIHIL;
        fascis[ZEPHYRUM].eventum.res_id = _litterae(piscina,
            _ch(instantia_ulid));
        fascis[ZEPHYRUM].eventum.genus_eventus = "creatio";
        fascis[ZEPHYRUM].eventum.datum = _litterae(piscina,
            json_scribere(d, piscina));
        fascis[ZEPHYRUM].eventum.actor = actor_l;
        fascis[ZEPHYRUM].eventum.origo = origo;

        /* ligamina opum per membra (pars "ops:<titulus>") -
         * excitatio se-sanans eventūs status ligaminis videt
         * (G16; index membrorum = sonda una, decisio 10) */
        pos = I;
        iter = json_objectum_iterator(ligamina);
        dum (json_objectum_iterator_proxima(&iter, &k, &v)
            && pos < n_fascis)
        {
            JsonValor* dm;
            character* pars_buf;

            si (v == NIHIL || !json_est_chorda(v))
            {
                perge;
            }
            pars_buf = (character*)piscina_allocare(piscina,
                (memoriae_index)k.mensura + V);
            si (pars_buf == NIHIL)
            {
                redde _fractum(mundus, "piscina exhausta");
            }
            memcpy(pars_buf, "ops:", IV);
            si (k.mensura > ZEPHYRUM)
            {
                memcpy(pars_buf + IV, k.datum,
                    (memoriae_index)k.mensura);
            }
            pars_buf[IV + k.mensura] = '\0';
            dm = json_objectum_creare(piscina);
            json_objectum_ponere(dm, "pars",
                json_chorda_creare(piscina, _ch(pars_buf)));
            json_objectum_ponere(dm, "membrum", v);
            fascis[pos].event_id = NIHIL;
            fascis[pos].eventum.res_id =
                fascis[ZEPHYRUM].eventum.res_id;
            fascis[pos].eventum.genus_eventus = "membrum-additum";
            fascis[pos].eventum.datum = _litterae(piscina,
                json_scribere(dm, piscina));
            fascis[pos].eventum.actor = actor_l;
            fascis[pos].eventum.origo = origo;
            pos++;
        }

        si (!_fascis_scribere(mundus, fascis, pos, NIHIL, vacua,
                NIHIL))
        {
            redde FALSUM;
        }
        si (!gesta_plicare(mundus))
        {
            redde FALSUM;
        }
        memcpy(exitus->instantia, instantia_ulid,
            (memoriae_index)GESTA_RES_ID_MENSURA);
    }

    exitus->facta = VERUM;
    /* provectio prima: gradus primi cuiusque cursūs generantur */
    _provehere_ad_fixum(mundus, exitus->instantia);
    redde VERUM;
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
gesta_res_in_ramo_datum (GestaMundus* mundus,
    constans character* res_id, constans character* ramus,
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
    si (ramus == NIHIL || ramus[0] == '\0')
    {
        redde gesta_res_datum(mundus, res_id, piscina);
    }
    ordo = _res_in_ramo_capere(mundus, _ch(res_id), ramus,
        piscina);
    redde ordo.exsistit ? ordo.datum : vacua;
}

chorda
gesta_res_in_ramo_status (GestaMundus* mundus,
    constans character* res_id, constans character* ramus,
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
    si (ramus == NIHIL || ramus[0] == '\0')
    {
        redde gesta_res_status(mundus, res_id, piscina);
    }
    ordo = _res_in_ramo_capere(mundus, _ch(res_id), ramus,
        piscina);
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
