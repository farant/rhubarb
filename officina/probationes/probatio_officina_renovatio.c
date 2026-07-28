/* probatio_officina_renovatio.c - renovatio sui residentis
 * (01KY4185QN: dimidium AGENDI vigiliae)
 *
 * Tres scaenae:
 *   I.  recens: renovare nihil agit ("iam recens") - machina
 *       in-processu, transcriptum aureum (exemplar legati).
 *   II. explorator praevius fractus: renovatio RECUSATA, residens
 *       vivus manet (petitio sequens respondetur) - machina
 *       in-processu; stipula renovatoris exitu 1 cadit.
 *   III. INTEGRA: furca + fistulae + exec VERUS. Infans machinam
 *       agit; stipula renovatoris HANC IPSAM probationem ut
 *       legatum renatum exsequitur (argv modus -fingere-legatum).
 *       Probat: exec factus, fistula EADEM respondet SINE
 *       initialize (protocollum renatus), signum novum refertur.
 *
 * Semitae defectus exec in probatio_processus (XIX); semita felix
 * hic sola integre agitur - in machina in-processu exec processum
 * probationis ipsum substitueret. */

#include "legatus.h"
#include "tabellarius.h"
#include "json.h"
#include "credo.h"
#include "piscina.h"
#include "chorda.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>     /* fork, pipe, dup2, alarm, _exit */
#include <sys/wait.h>   /* waitpid */
#include <sys/stat.h>   /* chmod */
#include <utime.h>      /* mtime binarii ficti */
#include <signal.h>     /* SIGPIPE neglegere (III) */

/* signum lancinationis (breve = XVI prima) et signum "novum" quod
 * stipula renovatoris echat - identitas ante/post exec */
#define SIGNUM_VETUS \
    "abadabadabadabadabadabadabadabadabadabadabadabadabadabadabadabad"
#define SIGNUM_NOVUM \
    "cafecafecafecafecafecafecafecafecafecafecafecafecafecafecafecafe"

interior constans character* _via_probationis = "";

interior constans character*
_radix (vacuum)
{
    constans character* r = getenv("RHUBARB_RADIX");

    redde r != NIHIL ? r : ".";
}

interior vacuum
_scribe_lineam (FILE* pl, Piscina* p, constans character* corpus)
{
    (vacuum)tabellarius_lineam_scribere(pl,
        chorda_ex_literis(corpus, p));
}

interior chorda
_linea_cruda (FILE* pl, Piscina* p, b32* bene)
{
    b32 finitus = FALSUM;
    chorda corpus = tabellarius_lineam_legere(pl, p, &finitus);

    *bene = finitus ? FALSUM : VERUM;
    redde corpus;
}

interior TabellariusNuntius
_lege_lineam (FILE* pl, Piscina* p, b32* bene)
{
    chorda corpus = _linea_cruda(pl, p, bene);
    TabellariusNuntius n;

    memset(&n, ZEPHYRUM, magnitudo(TabellariusNuntius));
    si (!*bene)
    {
        redde n;
    }
    redde tabellarius_nuntium_legere(corpus, p);
}

interior b32
_chorda_est (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (c.mensura == (i32)m && c.datum != NIHIL
        && memcmp(c.datum, litterae, m) == ZEPHYRUM) ? VERUM : FALSUM;
}

interior b32
_chorda_continet (chorda c, constans character* particula)
{
    memoriae_index m = strlen(particula);
    memoriae_index i;

    si (c.datum == NIHIL || (memoriae_index)c.mensura < m)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i + m <= (memoriae_index)c.mensura; i++)
    {
        si (memcmp(c.datum + i, particula, m) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* textus effectus instrumenti MCP: result.content[0].text */
interior chorda
_mcp_textus (TabellariusNuntius* n)
{
    redde json_ad_chorda(json_objectum_capere(
        json_tabulatum_obtinere(json_objectum_capere(
            json_objectum_capere(n->radix, "result"), "content"),
            ZEPHYRUM), "text"));
}

/* plagulam parvam scribere (contentum litterale) */
interior vacuum
_plagulam_scribere (constans character* via,
    constans character* contentum)
{
    FILE* pl = fopen(via, "wb");

    CREDO_NON_NIHIL(pl);
    si (pl != NIHIL)
    {
        fputs(contentum, pl);
        fclose(pl);
    }
}

/* binarium fictum antiquum + manifestum fontis veri: vigilia
 * FONTES_SUPERANT figitur (exemplar probatio_vigilia legati) */
interior vacuum
_stalitatem_fingere (constans character* via_binarii,
    constans character* via_manifesti)
{
    structura utimbuf tempora;

    _plagulam_scribere(via_manifesti,
        "# probatio renovationis\n"
        "officina/instrumenta/legatus.c\n");
    _plagulam_scribere(via_binarii, "x");
    tempora.actime = 1000000L;
    tempora.modtime = 1000000L;
    CREDO_VERUM(utime(via_binarii, &tempora) == ZEPHYRUM);
}

/* ==================================================
 * I. recens: renovare nihil agit
 * ================================================== */

interior vacuum
probatio_renovare_recens (Piscina* p)
{
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;
    chorda cruda;

    imprimere("--- Probans renovare recentem (nihil agendum) ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{\"protocolVersion\":\"2025-03-26\","
        "\"capabilities\":{},"
        "\"clientInfo\":{\"name\":\"probatio\"}}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"tools/list\"}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"renovare\",\"arguments\":{}}}");
    rewind(intra);

    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    cfg.radix = _radix();
    cfg.modus_mcp = VERUM;
    /* sine binarium_via: vigilia quieta = RECENS */
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);

    rewind(extra);
    n = _lege_lineam(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);
    CREDO_VERUM(n.genus == TABELLARIUS_RESPONSUM);

    cruda = _linea_cruda(extra, p, &bene);   /* tools/list */
    CREDO_VERUM(bene);
    CREDO_VERUM(_chorda_continet(cruda, "\"renovare\""));

    n = _lege_lineam(extra, p, &bene);   /* renovare */
    CREDO_VERUM(bene);
    CREDO_VERUM(_chorda_continet(_mcp_textus(&n), "iam recens"));

    fclose(intra);
    fclose(extra);
}

/* ==================================================
 * II. explorator praevius fractus: recusatio, residens vivit
 * ================================================== */

interior vacuum
probatio_renovare_explorator_fractus (Piscina* p)
{
    constans character* via_binarii =
        "officina/build/probatio_renovatio_binarium";
    constans character* via_manifesti =
        "officina/build/probatio_renovatio_manifestum";
    constans character* via_stipulae =
        "officina/build/probatio_renovatio_fractum.sh";
    FILE* intra = tmpfile();
    FILE* extra = tmpfile();
    LegatusConfiguratio cfg;
    b32 bene = FALSUM;
    TabellariusNuntius n;
    chorda textus;

    imprimere("--- Probans exploratorem fractum (recusatio) ---\n");
    CREDO_VERUM(intra != NIHIL && extra != NIHIL);

    _stalitatem_fingere(via_binarii, via_manifesti);
    _plagulam_scribere(via_stipulae,
        "#!/bin/bash\n"
        "echo \"aedificatio ficta fracta\" 1>&2\n"
        "exit 1\n");
    CREDO_VERUM(chmod(via_stipulae, 0755) == ZEPHYRUM);

    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
        "\"params\":{}}");
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"renovare\",\"arguments\":{}}}");
    /* petitio secunda: ansa PERGIT post recusationem (vivus) */
    _scribe_lineam(intra, p,
        "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":\"tools/call\","
        "\"params\":{\"name\":\"renovare\",\"arguments\":{}}}");
    rewind(intra);

    memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
    cfg.radix = _radix();
    cfg.modus_mcp = VERUM;
    cfg.binarium_via = via_binarii;
    cfg.via_manifesti = via_manifesti;
    cfg.signum = SIGNUM_VETUS;
    cfg.via_renovatoris = via_stipulae;
    CREDO_VERUM(legatus_currere(intra, extra, &cfg) == ZEPHYRUM);

    rewind(extra);
    n = _lege_lineam(extra, p, &bene);   /* initialize */
    CREDO_VERUM(bene);

    n = _lege_lineam(extra, p, &bene);   /* renovare prima */
    CREDO_VERUM(bene);
    textus = _mcp_textus(&n);
    CREDO_VERUM(_chorda_continet(textus, "RECUSATA"));
    CREDO_VERUM(_chorda_continet(textus, "aedificatio ficta fracta"));

    n = _lege_lineam(extra, p, &bene);   /* renovare altera - VIVIT */
    CREDO_VERUM(bene);
    CREDO_VERUM(_chorda_continet(_mcp_textus(&n), "RECUSATA"));

    fclose(intra);
    fclose(extra);
    remove(via_binarii);
    remove(via_manifesti);
    remove(via_stipulae);
}

/* ==================================================
 * III. integra: furca + fistulae + exec verus + renatus
 * ================================================== */

interior vacuum
probatio_renovatio_integra (Piscina* p)
{
    constans character* via_binarii =
        "officina/build/probatio_renovatio_binarium2";
    constans character* via_manifesti =
        "officina/build/probatio_renovatio_manifestum2";
    constans character* via_stipulae =
        "officina/build/probatio_renovatio_renovator.sh";
    integer p2i[II];   /* parens -> infans (stdin infantis) */
    integer i2p[II];   /* infans -> parens (stdout infantis) */
    pid_t pid;

    imprimere("--- Probans renovationem integram (exec verus) ---\n");

    (vacuum)signal(SIGPIPE, SIG_IGN);
    alarm(120);   /* custos: fistula pendens = mors honesta */

    _stalitatem_fingere(via_binarii, via_manifesti);

    /* stipula renovatoris: explorator signum novum echat; exec
     * HANC probationem ut legatum renatum agit */
    {
        FILE* pl = fopen(via_stipulae, "wb");

        CREDO_NON_NIHIL(pl);
        si (pl != NIHIL)
        {
            fprintf(pl,
                "#!/bin/bash\n"
                "if [ \"$1\" = \"-aedificare-solum\" ]; then\n"
                "    echo %s\n"
                "    exit 0\n"
                "fi\n"
                "exec \"%s\" -fingere-legatum \"$@\" -radix \"%s\""
                " -signum %s\n",
                SIGNUM_NOVUM, _via_probationis, _radix(),
                SIGNUM_NOVUM);
            fclose(pl);
        }
    }
    CREDO_VERUM(chmod(via_stipulae, 0755) == ZEPHYRUM);

    CREDO_VERUM(pipe(p2i) == ZEPHYRUM);
    CREDO_VERUM(pipe(i2p) == ZEPHYRUM);
    (vacuum)fflush(NIHIL);
    pid = fork();
    /* CAVETO: nulla adfirmatio ANTE ramum infantis - CREDO in
     * utroque processu curreret et punctum eius in buffro stdio
     * hereditato post dup2 in fistulam efflueret (JSON '.{...}'
     * corruptum - mensuratum primo cursu) */
    si (pid == (pid_t)ZEPHYRUM)
    {
        /* INFANS: machina super fistulas; numquam redit */
        LegatusConfiguratio cfg;

        (vacuum)fflush(stdout);   /* cingulum: buffrum vacuum ANTE
                                   * translationem fd */
        (vacuum)dup2(p2i[ZEPHYRUM], ZEPHYRUM);
        (vacuum)dup2(i2p[I], I);
        /* omnes exemplaria originalia claudere - exemplar retentum
         * exec superviveret et EOF numquam adveniret */
        close(p2i[ZEPHYRUM]); close(p2i[I]);
        close(i2p[ZEPHYRUM]); close(i2p[I]);

        memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
        cfg.radix = _radix();
        cfg.modus_mcp = VERUM;
        cfg.binarium_via = via_binarii;
        cfg.via_manifesti = via_manifesti;
        cfg.signum = SIGNUM_VETUS;
        cfg.via_renovatoris = via_stipulae;
        _exit((integer)legatus_currere(stdin, stdout, &cfg));
    }

    /* PARENS */
    CREDO_VERUM(pid > ZEPHYRUM);
    close(p2i[ZEPHYRUM]);
    close(i2p[I]);
    {
        FILE* ad_infantem = fdopen(p2i[I], "w");
        FILE* ab_infante = fdopen(i2p[ZEPHYRUM], "r");
        b32 bene = FALSUM;
        TabellariusNuntius n;
        chorda cruda;
        chorda textus;

        CREDO_VERUM(ad_infantem != NIHIL && ab_infante != NIHIL);

        /* 1. initialize: identitas VETUS in serverInfo.version */
        _scribe_lineam(ad_infantem, p,
            "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\","
            "\"params\":{}}");
        n = _lege_lineam(ab_infante, p, &bene);
        CREDO_VERUM(bene);
        {
            chorda versio = json_ad_chorda(json_objectum_capere(
                json_objectum_capere(json_objectum_capere(n.radix,
                    "result"), "serverInfo"), "version"));

            CREDO_VERUM(_chorda_est(versio, "abadabadabadabad"));
        }

        /* 2. renovare: explorator felix, signum novum promissum */
        _scribe_lineam(ad_infantem, p,
            "{\"jsonrpc\":\"2.0\",\"id\":2,\"method\":\"tools/call\","
            "\"params\":{\"name\":\"renovare\",\"arguments\":{}}}");
        n = _lege_lineam(ab_infante, p, &bene);
        CREDO_VERUM(bene);
        textus = _mcp_textus(&n);
        CREDO_VERUM(_chorda_continet(textus, "renovatio parata"));
        CREDO_VERUM(_chorda_continet(textus, "cafecafecafecafe"));

        /* post responsum infans se transformavit (stipula ->
         * probatio -fingere-legatum). 3. tools/list SINE
         * initialize: renatus = initium sponte factum - defectus
         * hic "nondum initiatum" pareret, non indicem */
        _scribe_lineam(ad_infantem, p,
            "{\"jsonrpc\":\"2.0\",\"id\":3,\"method\":"
            "\"tools/list\"}");
        cruda = _linea_cruda(ab_infante, p, &bene);
        CREDO_VERUM(bene);
        CREDO_VERUM(_chorda_continet(cruda, "\"renovare\""));

        /* 4. renovare iterum: identitas NOVA, vigilia recens */
        _scribe_lineam(ad_infantem, p,
            "{\"jsonrpc\":\"2.0\",\"id\":4,\"method\":\"tools/call\","
            "\"params\":{\"name\":\"renovare\",\"arguments\":{}}}");
        n = _lege_lineam(ab_infante, p, &bene);
        CREDO_VERUM(bene);
        textus = _mcp_textus(&n);
        CREDO_VERUM(_chorda_continet(textus, "iam recens"));
        CREDO_VERUM(_chorda_continet(textus, "cafecafecafecafe"));

        /* 5. EOF -> exitus ordinatus renati */
        fclose(ad_infantem);
        {
            integer status = ZEPHYRUM;

            CREDO_VERUM(waitpid(pid, &status, ZEPHYRUM) == pid);
            CREDO_VERUM(WIFEXITED(status));
            CREDO_VERUM(WEXITSTATUS(status) == ZEPHYRUM);
        }
        fclose(ab_infante);
    }

    alarm(ZEPHYRUM);
    remove(via_binarii);
    remove(via_manifesti);
    remove(via_stipulae);
}

/* ==================================================
 * principale (+ modus -fingere-legatum pro III)
 * ================================================== */

integer
principale (integer argc, character** argv)
{
    Piscina* piscina;

    si (argc > I && strcmp(argv[I], "-fingere-legatum") == ZEPHYRUM)
    {
        /* legatus renatus fictus: probatio III huc exec facit.
         * Sine binario/manifesto = vigilia recens; signum novum
         * identitatem post-exec probat. */
        LegatusConfiguratio cfg;
        integer i;

        memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
        cfg.modus_mcp = VERUM;
        per (i = II; i < argc; i++)
        {
            si (strcmp(argv[i], "-renatus") == ZEPHYRUM)
            {
                cfg.renatus = VERUM;
            }
            alioquin si (strcmp(argv[i], "-radix") == ZEPHYRUM
                && i + I < argc)
            {
                cfg.radix = argv[i + I];
                i++;
            }
            alioquin si (strcmp(argv[i], "-signum") == ZEPHYRUM
                && i + I < argc)
            {
                cfg.signum = argv[i + I];
                i++;
            }
        }
        redde (integer)legatus_currere(stdin, stdout, &cfg);
    }

    _via_probationis = argv[ZEPHYRUM];

    imprimere("\n========================================\n");
    imprimere("PROBATIO OFFICINA RENOVATIO\n");
    imprimere("========================================\n\n");

    piscina = piscina_generare_dynamicum("probatio_renovatio",
        1048576);
    si (piscina == NIHIL)
    {
        imprimere("piscina generari non potuit\n");
        redde I;
    }
    credo_aperire(piscina);

    probatio_renovare_recens(piscina);
    probatio_renovare_explorator_fractus(piscina);
    probatio_renovatio_integra(piscina);

    credo_imprimere_compendium();

    {
        b32 omnia = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        imprimere("========================================\n\n");
        redde omnia ? ZEPHYRUM : I;
    }
}
