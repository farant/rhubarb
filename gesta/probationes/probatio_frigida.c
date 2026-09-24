/* probatio_frigida.c - via scripturae frigida per machinam
 * tabularii (frigida_currere; effusiones in tmpfile captae).
 * Porta NATALIS instrumenti quod antea portam NULLAM habebat - et
 * ideo die quo haec probatio nata est (2026-09-21) NEXU FRACTO
 * inventum est, a Iulio nemine sciente. */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "tabularium.h"
#include "frigida.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

#define VIA_DB "gesta/build/probatio_frigida.db"
#define VIA_AN "gesta/build/probatio_frigida.jsonl"
#define VIA_TB "gesta/build/probatio_frigida_tabula.md"

interior vacuum
_purgare (vacuum)
{
    remove(VIA_DB);
    remove(VIA_DB "-wal");
    remove(VIA_DB "-shm");
    remove(VIA_AN);
    remove(VIA_TB);
}

/* plagulam totam in piscinam legere ("" si abest) */
interior constans character*
_plagula_litterae (
               Piscina* pn,
    constans character* via)
{
         FILE* f = fopen(via, "rb");
         long  mensura;
    character* fructus;

    si (f == NIHIL)
    {
        redde "";
    }
    fseek(f, 0L, SEEK_END);
    mensura = ftell(f);
    fseek(f, 0L, SEEK_SET);
    fructus = (character*)piscina_allocare(pn,
        (memoriae_index)(mensura > 0L ? mensura + 1L : I));
    si (fructus == NIHIL)
    {
        fclose(f);
        redde "";
    }
    si (   mensura > 0L
        && fread(fructus, I, (memoriae_index)mensura, f)
            != (memoriae_index)mensura)
    {
        fclose(f);
        redde "";
    }
    fructus[mensura > 0L ? mensura : 0L] = '\0';
    fclose(f);
    redde fructus;
}

/* effusionem FILE* totam in piscinam legere */
interior constans character*
_effusio_litterae (
    Piscina* pn,
       FILE* f)
{
         long  mensura;
    character* fructus;

    fseek(f, 0L, SEEK_END);
    mensura = ftell(f);
    fseek(f, 0L, SEEK_SET);
    fructus = (character*)piscina_allocare(pn,
        (memoriae_index)(mensura > 0L ? mensura + 1L : I));
    si (fructus == NIHIL)
    {
        redde "";
    }
    si (   mensura > 0L
        && fread(fructus, I, (memoriae_index)mensura, f)
            != (memoriae_index)mensura)
    {
        redde "";
    }
    fructus[mensura > 0L ? mensura : 0L] = '\0';
    redde fructus;
}

/* quoties acus in textu occurrit */
interior i32
_quoties_continet (
    constans character* textus,
    constans character* acus)
{
    constans character* cursor   = textus;
                   i32  numerus  = ZEPHYRUM;
        memoriae_index  passus   = strlen(acus);

    si (passus == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    dum ((cursor = strstr(cursor, acus)) != NIHIL)
    {
        numerus++;
        cursor += passus;
    }
    redde numerus;
}

nomen structura {
               integer  exitus;
    constans character* effusio;
    constans character* errores;
} Cursus;

/* frigida_currere cum effusionibus captis (argv[0] fictum) */
interior Cursus
_curre (
    constans TabulariumConfiguratio*  cfg,
                            Piscina*  pn,
                            integer   argc,
                 constans character** argv)
{
       Cursus  c;
         FILE* fluxus_effusionis  = tmpfile();
         FILE* fluxus_errorum     = tmpfile();
    unio { constans character** l; character** m; } u;

    c.exitus   = -I;
    c.effusio  = "";
    c.errores  = "";
    si (fluxus_effusionis == NIHIL || fluxus_errorum == NIHIL)
    {
        redde c;
    }
    u.l = argv;
    c.exitus = frigida_currere(cfg, argc, u.m, fluxus_effusionis,
        fluxus_errorum);
    c.effusio = _effusio_litterae(pn, fluxus_effusionis);
    c.errores = _effusio_litterae(pn, fluxus_errorum);
    fclose(fluxus_effusionis);
    fclose(fluxus_errorum);
    redde c;
}

/* lineam MCP mittere (mundus seminandus) */
interior constans character*
_mitte (
            Tabularium* t,
               Piscina* pn,
    constans character* linea)
{
      FILE* effusio = tmpfile();
    chorda  corpus;
    unio { constans character* l; i8* m; } u;
    constans character* fructus;

    si (effusio == NIHIL)
    {
        redde "";
    }
    u.l             = linea;
    corpus.datum    = u.m;
    corpus.mensura  = (i32)strlen(linea);
    (vacuum)tabularium_tractare(t, pn, corpus, effusio);
    fructus = _effusio_litterae(pn, effusio);
    fclose(effusio);
    redde fructus;
}

/* res_id ex responso addere ("res <ULID> creata") */
interior vacuum
_res_id_capere (
    constans character* responsum,
             character* exitus)
{
    constans character* p = strstr(responsum, "res ");
                   i32  k = ZEPHYRUM;

    exitus[0] = '\0';
    si (p == NIHIL)
    {
        redde;
    }
    p += IV;
    dum (   k < (i32)(GESTA_RES_ID_MENSURA - I) && p[k] != '\0'
         && p[k] != ' ')
    {
        exitus[k] = p[k];
        k++;
    }
    exitus[k] = '\0';
}

s32
principale (vacuum)
{
                   Piscina* piscina;
    TabulariumConfiguratio  cfg;
                Tabularium* t;
                 character  id_q[GESTA_RES_ID_MENSURA];
                 character  id_p[GESTA_RES_ID_MENSURA];
                    Cursus  c;
                       b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_frigida",
        67108864);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    _purgare();

    cfg.radix             = ".";
    cfg.via_scrinii       = VIA_DB;
    cfg.via_annalium      = VIA_AN;
    cfg.via_nexus         = NIHIL;
    cfg.via_identitatum   = NIHIL;
    cfg.via_citationum    = NIHIL;
    cfg.via_tabulae       = VIA_TB;
    cfg.via_entitatum     = NIHIL;
    cfg.signum            = NIHIL;
    cfg.via_binarii       = NIHIL;
    cfg.via_manifesti     = NIHIL;
    cfg.via_renovatoris   = NIHIL;
    cfg.renovatio_exitus  = FALSUM;
    cfg.renatus           = FALSUM;

    /* mundus seminatur per machinam, deinde CLAUDITUR - via frigida
     * mundum SUUM aperit, ut in vita vera */
    t = tabularium_creare(piscina, &cfg);
    CREDO_NON_NIHIL (t);
    si (t == NIHIL)
    {
        redde I;
    }
    CREDO_VERUM (tabularium_se_initiare(t));
    _res_id_capere(_mitte(t, piscina, "{\"jsonrpc\":\"2.0\","
        "\"id\":1,\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"quaestio\","
        "\"titulus\":\"Frigida quaestio\"}}}"), id_q);
    _res_id_capere(_mitte(t, piscina, "{\"jsonrpc\":\"2.0\","
        "\"id\":2,\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"parcum\","
        "\"titulus\":\"Frigida parcum\"}}}"), id_p);
    CREDO_VERUM (strlen(id_q) == (memoriae_index)XXVI);
    CREDO_VERUM (strlen(id_p) == (memoriae_index)XXVI);
    /* regio et sub-regio pro -mappa (forma lectionis SINE operando) */
    (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":3,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"regio\","
        "\"titulus\":\"Frigida regio\"}}}");
    (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":4,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"regio\","
        "\"titulus\":\"Frigida sub-regio\"}}}");
    (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":5,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Frigida sub-regio\","
        "\"actus\":\"nexus\",\"verbum\":\"intra\",\"alterum\":"
        "\"Frigida regio\"}}}");
    /* visio intra regionem pro -mappa-plena */
    (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":6,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"addere\",\"arguments\":{\"genus\":\"desideratum\","
        "\"titulus\":\"Frigida visio\",\"natura\":\"visio\"}}}");
    (vacuum)_mitte(t, piscina, "{\"jsonrpc\":\"2.0\",\"id\":7,"
        "\"method\":\"tools/call\",\"params\":{\"name\":"
        "\"gerere\",\"arguments\":{\"res\":\"Frigida visio\","
        "\"actus\":\"nexus\",\"verbum\":\"intra\",\"alterum\":"
        "\"Frigida regio\"}}}");
    /* inventarium pro -inventarium (lectio formae machinae) */
    (vacuum)_mitte(t, piscina,
        "{\"jsonrpc\":\"2.0\",\"id\":8,\"method\":\"tools/call\",\""
        "params\":{\"name\":\"addere\",\"arguments\":{\"genus\":\"i"
        "nventarium\",\"titulus\":\"Frigida inventarium\"}}}");
    (vacuum)_mitte(t, piscina,
        "{\"jsonrpc\":\"2.0\",\"id\":9,\"method\":\"tools/call\",\""
        "params\":{\"name\":\"inventarium\",\"arguments\":{\"res\":"
        "\"Frigida inventarium\",\"actus\":\"ordines\",\"ordines\":"
        "\"x.sh,y.sh\"}}}");
    (vacuum)_mitte(t, piscina,
        "{\"jsonrpc\":\"2.0\",\"id\":10,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"inventarium\",\"arguments\":{\"res"
        "\":\"Frigida inventarium\",\"actus\":\"lens\",\"lens\":\"i"
        "n PORTAE\",\"genus_valoris\":\"ita-non\"}}}");
    (vacuum)_mitte(t, piscina,
        "{\"jsonrpc\":\"2.0\",\"id\":11,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"inventarium\",\"arguments\":{\"res"
        "\":\"Frigida inventarium\",\"actus\":\"cellae\",\"cellae\""
        ":\"[{\\\"ordo\\\":\\\"x.sh\\\",\\\"lens\\\":\\\"in PORTAE"
        "\\\",\\\"valor\\\":{\\\"genus\\\":\\\"ita-non\\\",\\\"valo"
        "r\\\":\\\"ita\\\"}}]\"}}}");
    tabularium_claudere(t);

    /* I. quae verba haec unitas novit (principale formas veteres
     * ipsum servat) */
    CREDO_VERUM (frigida_verbum_novit("-status"));
    CREDO_VERUM (frigida_verbum_novit("-mutatio"));
    CREDO_VERUM (frigida_verbum_novit("-nexus"));
    CREDO_VERUM (frigida_verbum_novit("-actor"));
    CREDO_VERUM (frigida_verbum_novit("-origo"));
    CREDO_FALSUM (frigida_verbum_novit("-crea"));
    CREDO_FALSUM (frigida_verbum_novit("Frigida quaestio"));
    CREDO_FALSUM (frigida_verbum_novit(NIHIL));

    /* II. -status: eventus in annalibus, actor et origo ordinarii,
     * ET tabula.md regenerata - probatio quod per MACHINAM itum
     * est (scriptura gestae directa proiectionem non tangeret) */
    {
        constans character* a[] = { "frigida", "-status",
            "Frigida quaestio", "laborans" };
        constans character* an;

        c = _curre(&cfg, piscina, IV, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_SCRIPTUM);
        CREDO_VERUM (strstr(c.effusio, "status laborans") != NIHIL);
        an = _plagula_litterae(piscina, VIA_AN);
        CREDO_VERUM (strstr(an, "\"novus\":\"laborans\"") != NIHIL);
        CREDO_VERUM (strstr(an, "\"origo\":\"frigida\"") != NIHIL);
        CREDO_VERUM (strstr(_plagula_litterae(piscina, VIA_TB),
            "[laborans] Frigida quaestio") != NIHIL);
    }

    /* III. -actor et -origo: provenientia commissi (casus usus
     * primus: 'commissum opus claudit') */
    {
        constans character* a[] = { "frigida", "-actor", "claude",
            "-origo", "commissum:5ead8e54", "-status",
            "Frigida quaestio", "clausum" };
        constans character* an;

        c = _curre(&cfg, piscina, VIII, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_SCRIPTUM);
        an = _plagula_litterae(piscina, VIA_AN);
        CREDO_VERUM (strstr(an, "commissum:5ead8e54") != NIHIL);
        CREDO_VERUM (strstr(an, "\"novus\":\"clausum\"") != NIHIL);
    }

    /* IV. -mutatio: progressus a porta scriptus; valor cum '"'
     * RECTE effugitur (forma vetus -crea id non facit: NB ibi) */
    {
        constans character* a[] = { "frigida", "-mutatio",
            "Frigida parcum", "progressus", "3412/5100" };
        constans character* b[] = { "frigida", "-mutatio",
            "Frigida parcum", "dictum", "dixit \"salve\"" };
        constans character* an;

        c = _curre(&cfg, piscina, V, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_SCRIPTUM);
        c = _curre(&cfg, piscina, V, b);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_SCRIPTUM);
        an = _plagula_litterae(piscina, VIA_AN);
        CREDO_VERUM (strstr(an, "3412/5100") != NIHIL);
        CREDO_VERUM (strstr(an, "dixit \\\"salve\\\"") != NIHIL);
    }

    /* V. -nexus canonicum transit */
    {
        constans character* a[] = { "frigida", "-nexus",
            "Frigida quaestio", "impeditur-a", "Frigida parcum" };

        c = _curre(&cfg, piscina, V, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_SCRIPTUM);
        CREDO_VERUM (strstr(c.effusio, "--impeditur-a-->")
            != NIHIL);
    }

    /* VI. IANUA UNA: synonymum per viam frigidam QUOQUE recusatur
     * (via circum verba canonica non exstat), nihil scribitur, et
     * IMPERIUM VALIDUM in lineam imperii versum ostenditur */
    {
        constans character* a[] = { "frigida", "-nexus",
            "Frigida quaestio", "pendet-ex", "Frigida parcum" };
                 character speratum[CCLVI];

        c = _curre(&cfg, piscina, V, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_RECUSATUM);
        CREDO_VERUM (strstr(c.errores, "synonymum") != NIHIL);
        CREDO_VERUM (strstr(c.errores, "IMPERIUM VALIDUM") != NIHIL);
        sprintf(speratum, "-nexus \"%s\" impeditur-a \"%s\"",
            id_q, id_p);
        CREDO_VERUM (strstr(c.errores, speratum) != NIHIL);
        /* clavis 'verbum' discernit: orthographia nuda in
         * annalibus IAM stat - semen eam inter synonyma fert */
        CREDO_AEQUALIS_I32 (_quoties_continet(
            _plagula_litterae(piscina, VIA_AN),
            "\"verbum\":\"pendet-ex\""), ZEPHYRUM);
    }

    /* VII. inversum: imperium validum operanda COMMUTATA fert, et
     * optiones vocantis (-actor) servat ut linea pasta valeat */
    {
        constans character* a[] = { "frigida", "-actor", "claude",
            "-nexus", "Frigida parcum", "impedit",
            "Frigida quaestio" };
                 character speratum[CCLVI];

        c = _curre(&cfg, piscina, VII, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_RECUSATUM);
        CREDO_VERUM (strstr(c.errores, "inversum") != NIHIL);
        sprintf(speratum, "-actor \"claude\" -nexus \"%s\""
            " impeditur-a \"%s\"", id_q, id_p);
        CREDO_VERUM (strstr(c.errores, speratum) != NIHIL);
    }

    /* VIII. CAUSAE OMNES SIMUL trans STRATA: causa lineae imperii
     * (operandum superfluum) ET causa machinae (synonymum) in
     * responso UNO - nihil scriptum, exitus USUS */
    {
        constans character* a[] = { "frigida", "-nexus",
            "Frigida quaestio", "pendet-ex", "Frigida parcum",
            "superfluum" };

        c = _curre(&cfg, piscina, VI, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_USUS);
        CREDO_VERUM (strstr(c.errores, "superflu") != NIHIL);
        CREDO_VERUM (strstr(c.errores, "synonymum") != NIHIL);
        CREDO_VERUM (strstr(c.errores, "IMPERIUM VALIDUM") != NIHIL);
    }

    /* IX. operanda absentia: OMNIA nominantur, forma valida cum
     * valoribus vocantis et locis-tenentibus */
    {
        constans character* a[] = { "frigida", "-mutatio",
            "Frigida parcum" };

        c = _curre(&cfg, piscina, III, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_USUS);
        CREDO_VERUM (strstr(c.errores, "<clavis>") != NIHIL);
        CREDO_VERUM (strstr(c.errores, "<valor>") != NIHIL);
        CREDO_VERUM (strstr(c.errores,
            "-mutatio \"Frigida parcum\" <clavis> <valor>")
            != NIHIL);
    }

    /* X. vexillum ignotum et valor optionis absens SIMUL */
    {
        constans character* a[] = { "frigida", "-origo" };
        constans character* b[] = { "frigida", "-statum",
            "Frigida quaestio", "clausum" };

        c = _curre(&cfg, piscina, II, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_USUS);
        CREDO_VERUM (strstr(c.errores, "-origo") != NIHIL);
        CREDO_VERUM (strstr(c.errores, "verbum deest") != NIHIL);
        c = _curre(&cfg, piscina, IV, b);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_USUS);
        CREDO_VERUM (strstr(c.errores, "-statum") != NIHIL);
        CREDO_VERUM (strstr(c.errores, "-status") != NIHIL);
    }

    /* XI. recusatio machinae in -status (res ignota): exitus
     * RECUSATUM, nuntius machinae transit */
    {
        constans character* a[] = { "frigida", "-status",
            "Res nusquam", "clausum" };

        c = _curre(&cfg, piscina, IV, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_RECUSATUM);
        CREDO_VERUM (strstr(c.errores, "ignota") != NIHIL);
    }

    /* XIII. -status ILLEGALIS per viam frigidam (opus 01M335DZJM):
     * praeiudicium machinae currit, legales proximi nominantur,
     * IMPERIUM VALIDUM statum legalem fert, nihil scriptum; -vis
     * scribit tamen (nota custodiae). 'Frigida parcum' parcatum est:
     * 'apertum' illegale, 'tractum' legale. */
    {
        constans character* a[] = { "frigida", "-status",
            "Frigida parcum", "apertum" };
        constans character* b[] = { "frigida", "-vis", "-status",
            "Frigida parcum", "apertum" };
        constans character* an_ante;
        constans character* an_post;
                 character  speratum[CCLVI];

        an_ante  = _plagula_litterae(piscina, VIA_AN);
        c        = _curre(&cfg, piscina, IV, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_RECUSATUM);
        CREDO_VERUM (strstr(c.errores, "tractum | clausum") != NIHIL);
        CREDO_VERUM (strstr(c.errores, "IMPERIUM VALIDUM") != NIHIL);
        /* res in imperio valido ut RES_ID (machina eam solvit -
         * forma pasta-parata inambigua), legales ut locus-tenens */
        sprintf(speratum, "-status \"%s\" <tractum | clausum>",
            id_p);
        CREDO_VERUM (strstr(c.errores, speratum) != NIHIL);
        CREDO_VERUM (strstr(c.errores, "-vis") != NIHIL);
        an_post = _plagula_litterae(piscina, VIA_AN);
        CREDO_VERUM (strcmp(an_ante, an_post) == ZEPHYRUM);

        CREDO_VERUM (frigida_verbum_novit("-vis"));
        c = _curre(&cfg, piscina, V, b);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_SCRIPTUM);
        CREDO_VERUM (strstr(c.effusio, "status apertum") != NIHIL);
        an_post = _plagula_litterae(piscina, VIA_AN);
        CREDO_VERUM (strstr(an_post, "violatio machinae") != NIHIL);
    }

    /* XII. -res: LECTIO per ianuam eandem (breviarium). Causa:
     * silva.commissio(opus=ID) opus ANTE portas praeiudicat - ne
     * vitium identificatoris post commissum demum appareat. Linea
     * prima 'Titulus (genus, status)' contractus lectoris est. */
    {
        constans character* a[] = { "frigida", "-res",
            "Frigida parcum" };
        constans character* b[] = { "frigida", "-res",
            "Res nusquam" };
        constans character* an_ante;
        constans character* an_post;

        CREDO_VERUM (frigida_verbum_novit("-res"));
        an_ante  = _plagula_litterae(piscina, VIA_AN);
        c        = _curre(&cfg, piscina, III, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_SCRIPTUM);
        CREDO_VERUM (strstr(c.effusio, "Frigida parcum (parcum, ")
            == c.effusio);
        CREDO_VERUM (strstr(c.effusio, "res_id ") != NIHIL);
        /* lectio NIHIL scribit */
        an_post = _plagula_litterae(piscina, VIA_AN);
        CREDO_VERUM (strcmp(an_ante, an_post) == ZEPHYRUM);
        c = _curre(&cfg, piscina, III, b);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_RECUSATUM);
    }

    /* -MAPPA: forma lectionis SINE operando - arbor nominum per
     * ianuam unam; filia indentata; nihil scribit; operanda
     * superflua = usus */
    {
        constans character* a[] = { "frigida", "-mappa" };
        constans character* b[] = { "frigida", "-mappa", "superfluum" };
        constans character* an_ante;
        constans character* an_post;

        CREDO_VERUM (frigida_verbum_novit("-mappa"));
        an_ante  = _plagula_litterae(piscina, VIA_AN);
        c        = _curre(&cfg, piscina, II, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_SCRIPTUM);
        CREDO_VERUM (strstr(c.effusio, "Frigida regio") != NIHIL);
        CREDO_VERUM (strstr(c.effusio, "\n  Frigida sub-regio")
            != NIHIL);
        CREDO_VERUM (strstr(c.effusio, "parca ") == NIHIL);
        an_post = _plagula_litterae(piscina, VIA_AN);
        CREDO_VERUM (strcmp(an_ante, an_post) == ZEPHYRUM);
        c = _curre(&cfg, piscina, III, b);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_USUS);
        CREDO_VERUM (strstr(c.errores, "operanda superflua") != NIHIL);
        /* -mappa lineas captas NON fert */
        CREDO_VERUM (strstr(c.effusio, "<visio (>") == NIHIL);
    }

    /* -MAPPA-PLENA: visiones et principia sub regione sua ut lineae
     * captae STML */
    {
        constans character* a[] = { "frigida", "-mappa-plena" };

        CREDO_VERUM (frigida_verbum_novit("-mappa-plena"));
        c = _curre(&cfg, piscina, II, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_SCRIPTUM);
        CREDO_VERUM (strstr(c.effusio, "MAPPA (plena)") != NIHIL);
        CREDO_VERUM (strstr(c.effusio, "\n  <visio (> Frigida visio")
            != NIHIL);
        CREDO_VERUM (strstr(c.effusio, "\n  Frigida sub-regio")
            != NIHIL);
    }

    /* -INVENTARIUM: forma machina tabulae per ianuam unam - linea
     * una per cellam PRAESENTEM (y.sh sine cella non apparet);
     * nihil scribit; res ignota recusatur; operanda superflua = usus */
    {
        constans character* a[] = { "frigida", "-inventarium",
            "Frigida inventarium" };
        constans character* b[] = { "frigida", "-inventarium",
            "Nullum inventarium" };
        constans character* d[] = { "frigida", "-inventarium",
            "Frigida inventarium", "superfluum" };
        constans character* an_ante;
        constans character* an_post;

        CREDO_VERUM (frigida_verbum_novit("-inventarium"));
        an_ante  = _plagula_litterae(piscina, VIA_AN);
        c        = _curre(&cfg, piscina, III, a);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_SCRIPTUM);
        CREDO_VERUM (strstr(c.effusio,
            "x.sh\tin PORTAE\tita-non\tita\n")
            != NIHIL);
        CREDO_VERUM (strstr(c.effusio, "y.sh") == NIHIL);
        an_post = _plagula_litterae(piscina, VIA_AN);
        CREDO_VERUM (strcmp(an_ante, an_post) == ZEPHYRUM);
        c = _curre(&cfg, piscina, III, b);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_RECUSATUM);
        c = _curre(&cfg, piscina, IV, d);
        CREDO_AEQUALIS_S32 (c.exitus, FRIGIDA_EXITUS_USUS);
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    _purgare();
    redde praeteritus ? ZEPHYRUM : I;
}
