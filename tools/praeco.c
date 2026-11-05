/* praeco.c - praeco colloquiorum Claude Code (colloquia-plan T2)
 *
 * USUS:
 *   praeco hamus             JSON hami ex stdin + TMUX_PANE/TMUX ->
 *                            liber (praeconium_via); exitus 0 SEMPER
 *                            (hamus numquam sessionem frangit)
 *   praeco status <textus>   eventus "status" ab agente (panellus ex
 *                            TMUX_PANE; sessio per T3 resolvitur)
 *   praeco cantus [-intactus] <mml>
 *   praeco cantus -titulus T | -genus G
 *                            cantilena ad vicum (eventus "cantus"):
 *                            iudicata ANTE scripturam - fracta: causa
 *                            et caret, exitus 1, nihil scriptum;
 *                            -titulus/-genus ex cantuali infixo
 *                            (titulum electum imprimit)
 *   praeco antiphonale <via> plagulam .gabc in latere antiphonale
 *                            vici aperit (eventus "antiphonale", via
 *                            absoluta; relativa ad cwd); plagula
 *                            absens: exitus 1, nihil scriptum (gabc
 *                            pars II P8)
 *   praeco cantuale          cantilenae libri: titulus, genera, textus
 *   praeco cauda [n]         ultimi n eventus (XX) legibiles
 *   praeco tmux <servitor> <panellus>
 *                            eventus "tmux" (ex hamis tmux: fenestra,
 *                            panellus, sessio mutata)
 *   praeco tmux instituere   hamos tmux ponere: ~/.tmux.conf (sectio
 *                            signata) et servitor currens
 *
 * Tenue: omnis logica in lib/praeconium.c.
 */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur -
 * sine ea installatio (sanare installata) praeconem recusat */
/* <aedilis obiectum="build/fabrica/provenientia/praeco.c"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "praeconium.h"
#include "praeconium_cantus.h"
#include "cantuale.h"
#include "sors.h"
#include "provenientia.h"
#include "multiplexor.h"
#include "filum.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

/* comites P8: alveus viae pro realpath (PATH_MAX: macOS M+XXIV,
 * Linux IV*M+XCVI - maximum sumitur; examen <limits.h> non novit) */
#define VIA_MAXIMA (IV * M + XCVI)

/* hamus legit quantum datur; JSON hami parvus est */
#define PRAECO_STDIN_MAXIMUM (IV * M * M)

interior s64
nunc_ms (
    vacuum)
{
    structura timeval t;

    si (gettimeofday(&t, NIHIL) != ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    redde (s64)t.tv_sec * (s64)M + (s64)(t.tv_usec / M);
}

interior chorda
stdin_legere (
    Piscina* piscina)
{
       chorda  c;
       size_t  n;
       size_t  r;
    character* alveus;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    alveus     = (character*)piscina_allocare(piscina,
        PRAECO_STDIN_MAXIMUM);
    si (!alveus)
    {
        redde c;
    }
    n = ZEPHYRUM;
    dum (n < PRAECO_STDIN_MAXIMUM)
    {
        r = fread(alveus + n, I, PRAECO_STDIN_MAXIMUM - n, stdin);
        si (r == ZEPHYRUM)
        {
            frange;
        }
        n += r;
    }
    c.datum    = (i8*)alveus;
    c.mensura  = (i32)n;
    redde c;
}

interior integer
hamus (
    Piscina* piscina)
{
    PraeconiumEventus e;

    si (praeconium_ex_hamo(stdin_legere(piscina), getenv("TMUX_PANE"),
            getenv("TMUX"), nunc_ms(), &e, piscina))
    {
        (vacuum)praeconium_scribere(praeconium_via(piscina), &e,
            piscina);
    }
    redde ZEPHYRUM;
}

/* eventus ab agente: tempus, panellus (TMUX_PANE), servitor (TMUX ante
 * ','), via (PWD) - ceteri vacui */
interior vacuum
ambitus (
    PraeconiumEventus* e,
              Piscina* piscina)
{
    constans character* panellus;
    constans character* tmux;
                   i32  i;

    memset(e, ZEPHYRUM, magnitudo(*e));
    panellus     = getenv("TMUX_PANE");
    tmux         = getenv("TMUX");
    e->tempus    = nunc_ms();
    e->panellus  = chorda_ex_literis(panellus ? panellus : "", piscina);
    e->servitor  = chorda_ex_literis(tmux ? tmux : "", piscina);
    per (i = ZEPHYRUM; i < e->servitor.mensura
        && e->servitor.datum[i] != ','; i++)
    {
    }
    e->servitor.mensura  = i;
    e->via               =
        chorda_ex_literis(getenv("PWD") ? getenv("PWD")
        : "", piscina);
}

interior integer
status (
                Piscina* piscina,
     constans character* textus)
{
    PraeconiumEventus e;

    ambitus(&e, piscina);
    e.eventus = chorda_ex_literis("status", piscina);
    e.nuntius = chorda_ex_literis(textus, piscina);
    si (!praeconium_scribere(praeconium_via(piscina), &e, piscina))
    {
        fprintf(stderr, "praeco: scribere non potui\n");
        redde I;
    }
    redde ZEPHYRUM;
}

/* comites P8: eventus "tmux" ex hamo tmux - servitor et panellus ex
 * argumentis (#{socket_path}, #{pane_id}). Exitus 0 SEMPER: run-shell
 * errorem in linea status tmux monstraret */
interior integer
tmux_nuntiare (
                Piscina* piscina,
     constans character* servitor,
     constans character* panellus)
{
    PraeconiumEventus e;

    memset(&e, ZEPHYRUM, magnitudo(e));
    e.tempus    = nunc_ms();
    e.eventus   = chorda_ex_literis("tmux", piscina);
    e.servitor  = chorda_ex_literis(servitor, piscina);
    e.panellus  = chorda_ex_literis(panellus, piscina);
    (vacuum)praeconium_scribere(praeconium_via(piscina), &e, piscina);
    redde ZEPHYRUM;
}

/* via absoluta praeconis ipsius (argv[0]; sine '/' per PATH), nexibus
 * resolutis; NIHIL si non invenitur */
interior constans character*
viam_propriam (
    constans character* vocatum,
               Piscina* piscina)
{
    constans character* semita;
    constans character* initium;
    constans character* finis;
             character* candidatus;
             character* plena;
                   i32  n;

    plena = (character*)piscina_allocare(piscina, VIA_MAXIMA);
    si (!plena || !vocatum)
    {
        redde NIHIL;
    }
    si (strchr(vocatum, '/'))
    {
        redde realpath(vocatum, plena) ? plena : NIHIL;
    }
    semita   = getenv("PATH");
    initium  = semita;
    dum (initium && *initium)
    {
        finis       = strchr(initium, ':');
        n           = finis ? (i32)(finis - initium)
                            : (i32)strlen(initium);
        candidatus  = (character*)piscina_allocare(piscina,
            (memoriae_index)n + (memoriae_index)strlen(vocatum) + II);
        si (candidatus && n > ZEPHYRUM)
        {
            sprintf(candidatus, "%.*s/%s", (integer)n, initium,
                vocatum);
            si (   filum_existit(candidatus)
                && realpath(candidatus, plena))
            {
                redde plena;
            }
        }
        initium = finis ? finis + I : NIHIL;
    }
    redde NIHIL;
}

/* ~/.tmux.conf novum per fasciculum temporarium et renominationem
 * (nexus symbolicus resolvitur: scopus eius mutatur, non nexus); copia
 * prior semel in <conf>.ante-praeco */
interior b32
conf_scribere (
     constans character* via,
                 chorda  textus,
                Piscina* piscina)
{
     character* scopus;
     character* copia;
     character* novum;

    scopus  = (character*)piscina_allocare(piscina, VIA_MAXIMA);
    copia   = (character*)piscina_allocare(piscina, VIA_MAXIMA + XVI);
    novum   = (character*)piscina_allocare(piscina, VIA_MAXIMA + XVI);
    si (!scopus || !copia || !novum)
    {
        redde FALSUM;
    }
    si (!filum_existit(via) || !realpath(via, scopus))
    {
        si (strlen(via) >= VIA_MAXIMA)
        {
            redde FALSUM;
        }
        strcpy(scopus, via);
    }
    sprintf(copia, "%s.ante-praeco", scopus);
    sprintf(novum, "%s.praeco-novum", scopus);
    si (   filum_existit(scopus) && !filum_existit(copia)
        && !filum_copiare(scopus, copia))
    {
        redde FALSUM;
    }
    redde filum_scribere(novum, textus) && filum_movere(novum, scopus);
}

/* comites P8: hamos tmux ponere - fasciculus proprius
 * (<directorium libri>/tmux.conf), sectio signata in ~/.tmux.conf,
 * servitor currens ($TMUX, aliter ordinarius) eum legit */
interior integer
tmux_instituere (
                Piscina* piscina,
     constans character* vocatum)
{
     constans character* praeco;
     constans character* liber;
     constans character* domus;
     constans character* tmux;
              character* directorium;
              character* fasciculus;
              character* conf_via;
              character* socket;
              character* finis;
                 chorda  configuratio;
                 chorda  conf;
                 chorda  nova;
            Multiplexor  m;

    praeco = viam_propriam(vocatum, piscina);
    configuratio = praeco ? praeconium_tmux_configuratio(praeco,
        piscina) : chorda_ex_literis("", piscina);
    liber = praeconium_via(piscina);
    domus = getenv("HOME");
    si (configuratio.mensura == ZEPHYRUM || !liber || !domus)
    {
        fprintf(stderr, "praeco: via praeconis ignota (aut ' vel \" "
            "continet), aut HOME abest\n");
        redde I;
    }
    directorium  = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(liber) + I);
    fasciculus   = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(liber) + XVI);
    conf_via     = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(domus) + XVI);
    si (!directorium || !fasciculus || !conf_via)
    {
        redde I;
    }
    strcpy(directorium, liber);
    finis = strrchr(directorium, '/');
    si (finis)
    {
        *finis = '\0';
    }
    sprintf(fasciculus, "%s/tmux.conf", directorium);
    sprintf(conf_via, "%s/.tmux.conf", domus);
    (vacuum)filum_directorium_creare_cum_parentibus(directorium);
    si (!filum_scribere(fasciculus, configuratio))
    {
        fprintf(stderr, "praeco: %s scribere non potui\n", fasciculus);
        redde I;
    }
    imprimere("praeco: hami (praeco %s): %s\n", praeco, fasciculus);

    conf = filum_existit(conf_via) ? filum_legere_totum(conf_via,
        piscina) : chorda_ex_literis("", piscina);
    nova = praeconium_tmux_conf(conf, fasciculus, piscina);
    si (chorda_aequalis(nova, conf))
    {
        imprimere("praeco: %s: sectio iam praesens\n", conf_via);
    }
    alioquin si (conf_scribere(conf_via, nova, piscina))
    {
        imprimere("praeco: %s: sectio posita\n", conf_via);
    }
    alioquin
    {
        fprintf(stderr, "praeco: %s scribere non potui\n", conf_via);
        redde I;
    }

    memset(&m, ZEPHYRUM, magnitudo(m));
    tmux = getenv("TMUX");
    si (tmux && tmux[ZEPHYRUM])
    {
        socket = chorda_ut_cstr(chorda_ex_literis(tmux, piscina),
            piscina);
        finis  = socket ? strchr(socket, ',') : NIHIL;
        si (finis)
        {
            *finis = '\0';
        }
        m.socket = socket;
    }
    si (multiplexor_configurationem_legere(&m, fasciculus, piscina))
    {
        imprimere("praeco: servitor %s: hami positi\n",
            m.socket ? m.socket : "ordinarius");
    }
    alioquin
    {
        imprimere("praeco: servitor %s non legit (non currit?) - "
            "initio proximo ex ~/.tmux.conf\n",
            m.socket ? m.socket : "ordinarius");
    }
    redde ZEPHYRUM;
}

/* causa, textus, caret sub offset (ut auditio) */
interior vacuum
vitium_monstrare (
                     chorda  textus,
    constans CantualeVitium* vitium)
{
    i32 i;

    fprintf(stderr, "praeco: cantus: %s\n  %.*s\n  ",
        vitium->causa ? vitium->causa : "vitium",
        (integer)textus.mensura, (constans character*)textus.datum);
    per (i = ZEPHYRUM; i < vitium->offset; i++)
    {
        fputc(' ', stderr);
    }
    fputs("^\n", stderr);
}

interior integer
cantus (
       Piscina*  piscina,
       integer   argc,
     character** argv)
{
      PraeconiumEventus  e;
               Cantuale  liber;
         CantualeVitium  vitium;
        constans Cantio* cantio;
     constans character* titulus  = NIHIL;
     constans character* genus    = NIHIL;
     constans character* notatio  = NIHIL;
                 chorda  textus;
                    b32  intactus = FALSUM;
                   Sors  sors;
                integer  k;

    per (k = II; k < argc; k++)
    {
        si (strcmp(argv[k], "-intactus") == ZEPHYRUM)
        {
            intactus = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-titulus") == ZEPHYRUM
                     && k + I < argc)
        {
            titulus = argv[++k];
        }
        alioquin si (   strcmp(argv[k], "-genus") == ZEPHYRUM
                     && k + I < argc)
        {
            genus = argv[++k];
        }
        alioquin si (notatio == NIHIL && argv[k][ZEPHYRUM] != '-')
        {
            notatio = argv[k];
        }
        alioquin
        {
            fprintf(stderr, "praeco: cantus: argumentum ignotum: %s\n",
                argv[k]);
            redde I;
        }
    }
    si ((notatio != NIHIL) + (titulus != NIHIL) + (genus != NIHIL) != I)
    {
        fprintf(stderr,
            "usus: praeco cantus [-intactus] <mml> | cantus "
            "-titulus T | cantus -genus G\n");
        redde I;
    }
    si (notatio != NIHIL)
    {
        textus = chorda_ex_literis(notatio, piscina);
    }
    alioquin
    {
        si (!cantuale_ordinarium(piscina, &liber, &vitium))
        {
            fprintf(stderr, "praeco: cantuale fractum: %s\n",
                vitium.causa ? vitium.causa : "memoria");
            redde I;
        }
        sors_seminare(&sors, (i64)nunc_ms(), ZEPHYRUM);
        cantio = titulus != NIHIL
            ? cantuale_invenire(&liber, chorda_ex_literis(titulus,
                  piscina))
            : cantuale_eligere(&liber,
                  chorda_ex_literis(genus, piscina),
                  &sors, NIHIL);
        si (cantio == NIHIL)
        {
            fprintf(stderr, "praeco: cantus: %s sine cantilena: %s\n",
                titulus != NIHIL ? "titulus" : "genus",
                titulus != NIHIL ? titulus : genus);
            redde I;
        }
        textus    = cantio->textus;
        intactus  = intactus || cantio->intactus;
        imprimere("%.*s\n", (integer)cantio->titulus.mensura,
            (constans character*)cantio->titulus.datum);
    }
    ambitus(&e, piscina);
    si (!praeconium_cantum_parare(textus, intactus, &e, piscina,
        &vitium))
    {
        vitium_monstrare(textus, &vitium);
        redde I;
    }
    si (!praeconium_scribere(praeconium_via(piscina), &e, piscina))
    {
        fprintf(stderr, "praeco: scribere non potui\n");
        redde I;
    }
    redde ZEPHYRUM;
}

/* gabc pars II P8: plagula .gabc in latere antiphonale vici (eventus
 * "antiphonale", via absoluta); viam plenam imprimit */
interior integer
antiphonale_mittere (
                Piscina* piscina,
     constans character* via)
{
    PraeconiumEventus e;

    ambitus(&e, piscina);
    si (!praeconium_antiphonale_parare(via, &e, piscina))
    {
        fprintf(stderr, "praeco: antiphonale: plagula abest: %s\n",
            via);
        redde I;
    }
    si (!praeconium_scribere(praeconium_via(piscina), &e, piscina))
    {
        fprintf(stderr, "praeco: scribere non potui\n");
        redde I;
    }
    imprimere("%.*s\n", (integer)e.nuntius.mensura,
        (constans character*)e.nuntius.datum);
    redde ZEPHYRUM;
}

/* cantilenae libri infixi: titulus, genera, [intactus], textus */
interior integer
cantuale_monstrare (
    Piscina* piscina)
{
          Cantuale liber;
    CantualeVitium vitium;
         character genera[CXXVIII];
               i32 i;
               i32 j;

    si (!cantuale_ordinarium(piscina, &liber, &vitium))
    {
        fprintf(stderr, "praeco: cantuale fractum: %s\n",
            vitium.causa ? vitium.causa : "memoria");
        redde I;
    }
    per (i = ZEPHYRUM; i < liber.numerus; i++)
    {
        constans Cantio* c = &liber.cantiones[i];

        genera[ZEPHYRUM] = '\0';
        per (j = ZEPHYRUM; j < c->genera_numerus; j++)
        {
            si (strlen(genera) + (size_t)c->genera[j].mensura + II
                < magnitudo(genera))
            {
                si (j > ZEPHYRUM)
                {
                    strcat(genera, " ");
                }
                strncat(genera, (constans character*)c->genera[j].datum,
                    (size_t)c->genera[j].mensura);
            }
        }
        imprimere("%-14.*s %-18s %s%.*s\n", (integer)c->titulus.mensura,
            (constans character*)c->titulus.datum, genera,
            c->intactus ? "intactus  " : "", (integer)c->textus.mensura,
            (constans character*)c->textus.datum);
    }
    redde ZEPHYRUM;
}

interior integer
cauda (
    Piscina* piscina,
    integer  n)
{
                           Xar* l;
    constans PraeconiumEventus* e;
                           i32  i;
                           i32  primus;

    l = praeconium_caudam_legere(praeconium_via(piscina), LXIV * M,
        piscina);
    si (!l)
    {
        redde I;
    }
    primus = xar_numerus(l) > (i32)n ? xar_numerus(l) - (i32)n
        : ZEPHYRUM;
    per (i = primus; i < xar_numerus(l); i++)
    {
        e = (constans PraeconiumEventus*)xar_obtinere(l, i);
        imprimere("%ld %.*s %.*s %.*s %.*s %.*s\n", (long)e->tempus,
            (integer)e->eventus.mensura,
            (constans character*)e->eventus.datum,
            (integer)e->panellus.mensura,
            (constans character*)e->panellus.datum,
            (integer)e->genus.mensura,
            (constans character*)e->genus.datum,
            (integer)e->sessio.mensura,
            (constans character*)e->sessio.datum,
            (integer)e->nuntius.mensura,
            (constans character*)e->nuntius.datum);
    }
    redde ZEPHYRUM;
}

externus constans ProvenientiaRelatio provenientia_praeco;

integer
principale (
      integer   argc,
    character** argv)
{
    Piscina* piscina;

    si (provenientia_respondere((s32)argc, argv, &provenientia_praeco))
    {
        redde ZEPHYRUM;
    }
    piscina = piscina_generare_dynamicum("praeco", LXIV * M);
    si (!piscina || argc < II)
    {
        fprintf(stderr, "usus: praeco hamus | status <textus> | cantus "
            "... | cantuale | cauda [n]\n");
        /* hamus sine piscina aut argumentis: tamen 0 */
        redde argc >= II && strcmp(argv[I], "hamus") == ZEPHYRUM
            ? ZEPHYRUM : I;
    }
    si (strcmp(argv[I], "hamus") == ZEPHYRUM)
    {
        redde hamus(piscina);
    }
    si (strcmp(argv[I], "status") == ZEPHYRUM && argc >= III)
    {
        redde status(piscina, argv[II]);
    }
    si (strcmp(argv[I], "cantus") == ZEPHYRUM)
    {
        redde cantus(piscina, argc, argv);
    }
    si (strcmp(argv[I], "cantuale") == ZEPHYRUM)
    {
        redde cantuale_monstrare(piscina);
    }
    si (strcmp(argv[I], "antiphonale") == ZEPHYRUM && argc == III)
    {
        redde antiphonale_mittere(piscina, argv[II]);
    }
    si (strcmp(argv[I], "cauda") == ZEPHYRUM)
    {
        redde cauda(piscina, argc >= III ? atoi(argv[II]) : XX);
    }
    si (   strcmp(argv[I], "tmux")        == ZEPHYRUM && argc == III
        && strcmp(argv[II], "instituere") == ZEPHYRUM)
    {
        redde tmux_instituere(piscina, argv[ZEPHYRUM]);
    }
    si (strcmp(argv[I], "tmux") == ZEPHYRUM && argc >= IV)
    {
        redde tmux_nuntiare(piscina, argv[II], argv[III]);
    }
    fprintf(stderr,
        "usus: praeco hamus | status <textus> | cantus [-intactus] "
        "<mml> | cantus -titulus T | cantus -genus G | cantuale | "
        "antiphonale <via> | cauda [n] | tmux <servitor> <panellus> | "
        "tmux instituere\n");
    redde I;
}
