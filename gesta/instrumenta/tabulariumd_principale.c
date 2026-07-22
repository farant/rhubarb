/* tabulariumd_principale.c - daemon TCP servi tabularii (F0 forum)
 *
 * Machina eadem (gesta/fontes/tabularium.c) post socket: accipere
 * OBSTRUENS sequentiale + FILE* socketi (fdopen) in ansa per-lineam
 * exsistente - reactor nullus. Contractus clientis v1: connexio
 * per-petitionem (connecte -> lineae -> claude); multiplex reactoris
 * (exemplar hospitii) = sequela nominata. Daemon se praeinitiat
 * semel - initialize clientium idempotens (innoxium).
 *
 * Usus: tabulariumd [-radix dir] [-portus N] [-scrinium via]
 *   [-annales via] [-signum hex] [-binarium via] [-manifestum via]
 * Ordinarii: portus 8753 (conditio Romae a.C.), mundus fori
 * (forum.db + gesta/annales/forum.jsonl), proiectiones NIHIL.
 * Portus 0 = auto-selectus; portus ligatus in stdout imprimitur
 * ("portus N" - semen fumi). Ligatio fallita = daemon alter
 * fortasse vivit -> exitus 0 quietus (certamen start-if-absent se
 * solvit). Garrulitas cetera in stderr. */

#include "tabularium.h"
#include "tabellarius.h"
#include "tcp.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>   /* dup - FILE* lectionis/scriptionis separati */

#define TABULARIUMD_PORTUS_ORDINARIUS 8753

interior b32 _se_initiare (Tabularium* t);
interior vacuum _connexionem_servire (Tabularium* t, FILE* intra,
    FILE* extra);

/* initiatio synthetica per tractare - machina intacta manet.
 * Responsum in tmpfile captum; "result" in primis bytes = bene */
interior b32
_se_initiare (Tabularium* t)
{
    Piscina* pn = piscina_generare_dynamicum("tabulariumd_init",
        LXIV * 1024);
    FILE* effusio;
    b32 bene = FALSUM;

    si (pn == NIHIL)
    {
        redde FALSUM;
    }
    effusio = tmpfile();
    si (effusio != NIHIL)
    {
        character linea[DXII];
        chorda corpus = chorda_ex_literis(
            "{\"jsonrpc\":\"2.0\",\"id\":0,\"method\":"
            "\"initialize\",\"params\":{}}", pn);

        (vacuum)tabularium_tractare(t, pn, corpus, effusio);
        rewind(effusio);
        si (fgets(linea, (integer)magnitudo(linea), effusio)
            != NIHIL && strstr(linea, "\"result\"") != NIHIL)
        {
            bene = VERUM;
        }
        fclose(effusio);
    }
    piscina_destruere(pn);
    redde bene;
}

/* ansa per-lineam ad EOF - exemplar tabularium_currere (pn recens
 * per nuntium); tabellarius fluit post scriptionem quamque */
interior vacuum
_connexionem_servire (Tabularium* t, FILE* intra, FILE* extra)
{
    per (;;)
    {
        Piscina* pn = piscina_generare_dynamicum(
            "tabulariumd_nuntius", LXIV * 1024);
        b32 finitus = FALSUM;
        chorda corpus;

        si (pn == NIHIL)
        {
            frange;
        }
        corpus = tabellarius_lineam_legere(intra, pn, &finitus);
        si (finitus)
        {
            piscina_destruere(pn);
            frange;
        }
        (vacuum)tabularium_tractare(t, pn, corpus, extra);
        piscina_destruere(pn);
    }
}

s32 principale (integer argc, character** argv)
{
    TabulariumConfiguratio cfg;
    constans character* radix = ".";
    constans character* via_scrinii = "forum.db";
    constans character* via_annalium = "gesta/annales/forum.jsonl";
    constans character* signum = NIHIL;
    constans character* via_binarii = NIHIL;
    constans character* via_manifesti = NIHIL;
    i32 portus = TABULARIUMD_PORTUS_ORDINARIUS;
    integer k;
    Piscina* piscina;
    Tabularium* t;
    TcpServus* servus;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-radix") == ZEPHYRUM && k + I < argc)
        {
            radix = argv[k + I];
            k++;
        }
        alioquin si (strcmp(argv[k], "-portus") == ZEPHYRUM
            && k + I < argc)
        {
            portus = (i32)atoi(argv[k + I]);
            k++;
        }
        alioquin si (strcmp(argv[k], "-scrinium") == ZEPHYRUM
            && k + I < argc)
        {
            via_scrinii = argv[k + I];
            k++;
        }
        alioquin si (strcmp(argv[k], "-annales") == ZEPHYRUM
            && k + I < argc)
        {
            via_annalium = argv[k + I];
            k++;
        }
        alioquin si (strcmp(argv[k], "-signum") == ZEPHYRUM
            && k + I < argc)
        {
            signum = argv[k + I];
            k++;
        }
        alioquin si (strcmp(argv[k], "-binarium") == ZEPHYRUM
            && k + I < argc)
        {
            via_binarii = argv[k + I];
            k++;
        }
        alioquin si (strcmp(argv[k], "-manifestum") == ZEPHYRUM
            && k + I < argc)
        {
            via_manifesti = argv[k + I];
            k++;
        }
    }
    cfg.radix = radix;
    cfg.via_scrinii = via_scrinii;
    cfg.via_annalium = via_annalium;
    cfg.via_nexus = NIHIL;
    cfg.via_identitatum = NIHIL;
    cfg.via_citationum = NIHIL;
    cfg.via_tabulae = NIHIL;
    cfg.via_entitatum = NIHIL;
    cfg.signum = signum;
    cfg.via_binarii = via_binarii;
    cfg.via_manifesti = via_manifesti;

    piscina = piscina_generare_dynamicum("tabulariumd", 268435456);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "tabulariumd: piscina fracta\n");
        redde I;
    }
    t = tabularium_creare(piscina, &cfg);
    si (t == NIHIL)
    {
        fprintf(stderr, "tabulariumd: creatio fracta\n");
        redde I;
    }
    si (!_se_initiare(t))
    {
        fprintf(stderr, "tabulariumd: initiatio fracta (scrinium"
            " %s, annales %s)\n", via_scrinii, via_annalium);
        redde I;
    }
    {
        TcpServusOptiones opt = tcp_servus_optiones_default();
        TcpServusResultus rs;

        /* OBSTRUENS: accepti modum hereditant (tcp_posix) -
         * fdopen + tabellarius sine reactore */
        opt.non_blocans = FALSUM;
        rs = tcp_servus_creare_cum_optionibus("127.0.0.1", portus,
            &opt, piscina);
        si (!rs.successus)
        {
            fprintf(stderr, "tabulariumd: ligatio fallita (portus"
                " %d occupatus - daemon alter fortasse vivit) -"
                " exitus quietus\n", (int)portus);
            redde ZEPHYRUM;
        }
        servus = rs.servus;
        /* creare = socket+bind SOLUM; auscultare separatum */
        si (tcp_servus_auscultare(servus, ZEPHYRUM) != TCP_OK)
        {
            fprintf(stderr,
                "tabulariumd: auscultatio fracta\n");
            redde I;
        }
    }
    printf("portus %d\n", (int)tcp_servus_obtinere_portum(servus));
    fflush(stdout);
    fprintf(stderr, "tabulariumd: auscultat 127.0.0.1:%d (mundus"
        " %s)\n", (int)tcp_servus_obtinere_portum(servus),
        via_scrinii);

    per (;;)
    {
        Piscina* pc = piscina_generare_dynamicum(
            "tabulariumd_conexio", LXIV * 1024);
        TcpResultus rc;

        si (pc == NIHIL)
        {
            frange;
        }
        rc = tcp_servus_accipere(servus, pc);
        si (!rc.successus)
        {
            piscina_destruere(pc);
            perge;
        }
        {
            /* dup bis: FILE* uterque descriptorem proprium
             * possidet; originalis tcp_claudere clauditur */
            integer fd = (integer)tcp_obtinere_fd(rc.connexio);
            FILE* intra = fdopen(dup(fd), "r");
            FILE* extra = fdopen(dup(fd), "w");

            si (intra != NIHIL && extra != NIHIL)
            {
                _connexionem_servire(t, intra, extra);
            }
            si (intra != NIHIL)
            {
                fclose(intra);
            }
            si (extra != NIHIL)
            {
                fclose(extra);
            }
        }
        tcp_claudere(rc.connexio);
        piscina_destruere(pc);
    }
    redde I;
}
