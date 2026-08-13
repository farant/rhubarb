#include "postulata_posix.h"

#include "manus.h"
#include "credo.h"
#include "piscina.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

/* probatio_manus.c
 *
 * QUID HAEC PROBAT: dimidium manus in PROTOCOLLO - tesseram petere,
 * tesseram pulsare, tres formas responsi imperii legere, DUAS
 * semitas termini distinguere, manum fractam tacere.
 *
 * QUID NON PROBAT - ET CUR ID DICITUR: nihil hic probat JS
 * generatum in WKWebView vero recte agere. Simulacrum paginam NON
 * simulat: responsa scripta reddit. Simulacrum quod paginam
 * fingeret 'JS meus contra DOM meum' esset - defectus qui
 * probatio_fori.js QUATER viridem et mendacem fecit. Fides in
 * paginam veram ex probatione fumi contra applicationem VIVAM
 * venit, non hinc.
 *
 * ERGO simulacrum servus HTTP nudus est, non hospitium+imperium:
 * vitium in illis vitium hic celare NON debet.
 *
 * SESSIO CREDO DUPLEX: asserta quae FALLERE debent suitam rubram
 * facerent. Ergo capiuntur in sessione credo SEPARATA (_capere),
 * et sessio principalis de captura interrogat - fractum fit
 * assertum POSITIVUM.
 */

#define SCEN_VERUM    I    /* pendens semel, deinde ok:true      */
#define SCEN_FALSUM   II   /* ok:false - pagina terminum tetigit */
#define SCEN_PENDENS  III  /* numquam respondet - app haeret     */
#define SCEN_CULPA    IV   /* JS iactavit                        */

#define VIA_ULTIMI    "build/manus_ultimum.js"

/* ========================================================================
 * Simulacrum: servus HTTP scriptus in prole
 * ======================================================================== */

interior vacuum
_respondere (
    s32                 fd,
    s32                 codex,
    constans character* corpus)
{
    character caput[DXII];
    s32       longitudo = (s32)strlen(corpus);

    sprintf(caput,
        "HTTP/1.1 %d %s\r\nContent-Type: application/json\r\n"
        "Content-Length: %d\r\nConnection: close\r\n\r\n",
        (integer)codex,
        (codex == CC) ? "OK"
                      : ((codex == CCII) ? "Accepted" : "Not Found"),
        (integer)longitudo);

    (vacuum)write(fd, caput, strlen(caput));
    (vacuum)write(fd, corpus, (memoriae_index)longitudo);
}

/* Corpus POST in plagulam scribere: sic probatio JS re vera missum
 * inspicere potest sine effugio in prole. */
interior vacuum
_ultimum_servare (
    constans character* petitio)
{
    constans character* corpus = strstr(petitio, "\r\n\r\n");
    FILE*               f;

    si (corpus == NIHIL)
    {
        redde;
    }
    corpus += IV;
    f = fopen(VIA_ULTIMI, "w");
    si (f == NIHIL)
    {
        redde;
    }
    (vacuum)fputs(corpus, f);
    (vacuum)fclose(f);
}

interior vacuum
_puer (
    s32 fd_auscultans,
    s32 scenario)
{
    s32 tessera = 0;
    s32 pulsus  = 0;

    dum (VERUM)
    {
        character petitio[MMMMXCVI];
        character corpus[DXII];
        s32       fd;
        s32       lecta;
        s32       codex = CC;

        fd = (s32)accept(fd_auscultans, NIHIL, NIHIL);
        si (fd < 0)
        {
            perge;
        }
        lecta = (s32)read(fd, petitio, magnitudo(petitio) - I);
        si (lecta <= 0)
        {
            (vacuum)close(fd);
            perge;
        }
        petitio[lecta] = '\0';

        si (strncmp(petitio, "POST", IV) == 0)
        {
            /* CCII, ut imperium VERUM: tessera acceptio est, non
             * effectus. Prius hic CC reddebam - quod ex mea
             * coniectura veniebat, non ex servo, et probatio mecum
             * consentiebat dum applicatio vera dissentiret. Fumus
             * id invenit; unitas non poterat. */
            _ultimum_servare(petitio);
            tessera++;
            sprintf(corpus, "{\"tessera\":%d}", (integer)tessera);
            codex = CCII;
        }
        alioquin si (strstr(petitio, "GET /imperium/0 ") != NIHIL)
        {
            /* Sonda vitae (manus_aperire). Imperium ipsum CDIV
             * reddit pro tessera ignota - responsum QUODLIBET
             * vitam probat. */
            codex = CDIV;
            sprintf(corpus, "{\"culpa\":\"tessera ignota\"}");
        }
        alioquin si (strstr(petitio, "GET /imperium/1 ") != NIHIL)
        {
            /* Tessera I = sonda paratitudinis (iussum '1' quod
             * manus ipsa mittit). Omni scenario SUCCEDAT: manus
             * quae iter plenum probare non potest omnino non
             * aperitur, et scenaria infra de manu APERTA loquuntur
             * - de applicatione scilicet quae sana fuit et postea
             * haesit aut iactavit. */
            sprintf(corpus, "{\"status\":\"perfectum\",\"valor\":1}");
        }
        alioquin si (scenario == SCEN_VERUM)
        {
            pulsus++;
            si (pulsus < II)
            {
                sprintf(corpus, "{\"status\":\"pendens\"}");
            }
            alioquin
            {
                sprintf(corpus, "{\"status\":\"perfectum\","
                                "\"valor\":{\"ok\":true,\"visum\":\"1\"}}");
            }
        }
        alioquin si (scenario == SCEN_FALSUM)
        {
            sprintf(corpus, "{\"status\":\"perfectum\","
                            "\"valor\":{\"ok\":false,\"visum\":\"0\"}}");
        }
        alioquin si (scenario == SCEN_CULPA)
        {
            sprintf(corpus, "{\"status\":\"culpa\","
                            "\"nuntius\":\"Can't find variable: x\"}");
        }
        alioquin
        {
            sprintf(corpus, "{\"status\":\"pendens\"}");
        }

        _respondere(fd, codex, corpus);
        (vacuum)close(fd);
    }
}

interior s32
_simulacrum_incipere (
    s32  scenario,
    i32* portus)
{
    structura sockaddr_in ad;
    structura sockaddr_in vera;
    socklen_t             longitudo = (socklen_t)magnitudo(vera);
    s32                   fd;
    s32                   optio = I;
    s32                   proles;

    fd = (s32)socket(AF_INET, SOCK_STREAM, 0);
    si (fd < 0)
    {
        redde -I;
    }
    (vacuum)setsockopt(fd, SOL_SOCKET, SO_REUSEADDR,
                       &optio, (socklen_t)magnitudo(optio));

    memset(&ad, 0, magnitudo(ad));
    ad.sin_family      = AF_INET;
    ad.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ad.sin_port        = 0;   /* portus ephemerus - nulla concursio */

    si (bind(fd, (structura sockaddr*)&ad, (socklen_t)magnitudo(ad)) < 0
     || listen(fd, XVI) < 0
     || getsockname(fd, (structura sockaddr*)&vera, &longitudo) < 0)
    {
        (vacuum)close(fd);
        redde -I;
    }
    *portus = (i32)ntohs(vera.sin_port);

    proles = (s32)fork();
    si (proles == 0)
    {
        _puer(fd, scenario);
        _exit(0);
    }
    (vacuum)close(fd);
    redde proles;
}

interior vacuum
_simulacrum_finire (
    s32 proles)
{
    si (proles > 0)
    {
        (vacuum)kill(proles, SIGKILL);
        (vacuum)waitpid(proles, NIHIL, 0);
    }
}

/* ========================================================================
 * Captura: assertum FALLENS in sessione credo separata
 * ======================================================================== */

nomen structura {
    b32            manus_aperta;
    b32            manus_fracta;
    b32            notatum;
    memoriae_index totalis;      /* quot asserta post DUO vocamina */
    character      genus[LXIV];
    character      exspectatum[CCLVI];
    character      visum[DXII];
    character      causa[DXII];
} Captura;

interior vacuum
_copiare (
    character*     destinatio,
    chorda         fons,
    memoriae_index maximum)
{
    memoriae_index n = (memoriae_index)fons.mensura;
    si (n > maximum - I)
    {
        n = maximum - I;
    }
    si (n > 0 && fons.datum != NIHIL)
    {
        memcpy(destinatio, fons.datum, n);
    }
    destinatio[n] = '\0';
}

interior Captura
_capere (
    s32                 scenario,
    constans character* selector)
{
    Captura        c;
    Piscina*       p;
    Manus*         m;
    CredoNotatio*  fracti;
    memoriae_index quot = 0;
    i32            portus = 0;
    s32            proles;

    memset(&c, 0, magnitudo(c));

    proles = _simulacrum_incipere(scenario, &portus);
    si (proles < 0)
    {
        redde c;
    }

    p = piscina_generare_dynamicum("captura_manus", CXXVIII * M);
    credo_aperire(p);

    m = manus_aperire(p, "127.0.0.1", portus);
    si (m != NIHIL)
    {
        c.manus_aperta = VERUM;

        CREDO_MANUS_EXISTIT_MORA(m, selector, MANUS_MORA_BREVIS);
        /* SECUNDUM consulto: manus fracta hoc TACERE debet, ergo
         * numerus totalis I manere debet, non II. */
        CREDO_MANUS_EXISTIT_MORA(m, selector, MANUS_MORA_BREVIS);

        c.manus_fracta = manus_fracta(m);
        _copiare(c.causa, manus_causa(m), magnitudo(c.causa));
    }

    c.totalis = credo_numerus_totalis();
    fracti    = credo_invenire_fractos(&quot);
    si (quot > 0 && fracti != NIHIL)
    {
        /* Credo 'Receptus' ex valor_primus imprimit, 'Speratus' ex
         * secundo - actuale ANTE exspectatum. */
        c.notatum = VERUM;
        _copiare(c.genus,       fracti->genus,          magnitudo(c.genus));
        _copiare(c.visum,       fracti->valor_primus,   magnitudo(c.visum));
        _copiare(c.exspectatum, fracti->valor_secundus, magnitudo(c.exspectatum));
    }

    credo_claudere();
    piscina_destruere(p);
    _simulacrum_finire(proles);
    redde c;
}

interior b32
_continet (
    constans character* fenum,
    constans character* acus)
{
    redde (strstr(fenum, acus) != NIHIL) ? VERUM : FALSUM;
}

/* ========================================================================
 * principale
 * ======================================================================== */

s32 principale (vacuum)
{
    Piscina* piscina;
    Captura  falsum;
    Captura  pendens;
    Captura  culpa;
    b32      praeteritus;

    /* Prolis mors dum scribimus probationem occidere non debet. */
    (vacuum)signal(SIGPIPE, SIG_IGN);

    /* ========================================================
     * PHASIS I: fracturas capere (ANTE sessionem principalem)
     * ======================================================== */

    falsum  = _capere(SCEN_FALSUM,  ".nusquam");
    pendens = _capere(SCEN_PENDENS, ".nusquam");
    culpa   = _capere(SCEN_CULPA,   ".nusquam");

    /* ========================================================
     * PHASIS II: de capturis interrogare
     * ======================================================== */

    piscina = piscina_generare_dynamicum("probatio_manus", CXXVIII * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Pagina terminum tetigit (ok:false) ---\n");
    {
        CREDO_VERUM (falsum.manus_aperta);
        CREDO_VERUM (falsum.notatum);
        CREDO_VERUM (falsum.manus_fracta);

        /* Nuntius AMBO latera nominet - hoc est totum pretium. */
        CREDO_VERUM (_continet(falsum.genus, "credo_manus_existit"));
        CREDO_VERUM (_continet(falsum.exspectatum, "I aut plura"));
        CREDO_CHORDAE_AEQUALES (falsum.visum, "0");

        /* Manus fracta TACET: duo vocamina, unum assertum. */
        CREDO_AEQUALIS_I32 ((i32)falsum.totalis, I);
    }

    imprimere("\n--- Applicatio haeret (semper pendens) ---\n");
    {
        CREDO_VERUM (pendens.manus_aperta);
        CREDO_VERUM (pendens.manus_fracta);

        /* CARDO HUIUS PROBATIONIS: eadem assertio, alia causa -
         * et nuntii DISTINGUI debent. Aliter probatio fracta
         * narrare non potest utrum facies tua fracta sit an
         * applicatio omnino mortua. */
        CREDO_VERUM (_continet(pendens.causa, "non respondit"));
        CREDO_FALSUM (_continet(pendens.causa, "assertum fractum"));
        CREDO_FALSUM (_continet(falsum.causa, "non respondit"));
    }

    imprimere("\n--- JS iactavit (culpa) ---\n");
    {
        CREDO_VERUM (culpa.manus_fracta);
        CREDO_VERUM (_continet(culpa.causa, "Can't find variable"));
    }

    imprimere("\n--- JS re vera missum ---\n");
    {
        FILE*     f;
        character js[MMMMXCVI];
        i32       lecta = 0;

        memset(js, 0, magnitudo(js));
        f = fopen(VIA_ULTIMI, "r");
        CREDO_NON_NIHIL (f);
        si (f != NIHIL)
        {
            lecta = (i32)fread(js, I, magnitudo(js) - I, f);
            (vacuum)fclose(f);
            js[lecta] = '\0';

            /* Custos regressionis: si quis moram in C reduxerit,
             * haec rumpuntur - et debent. Mora IN PAGINA fit. */
            CREDO_VERUM (_continet(js, "new Promise"));
            CREDO_VERUM (_continet(js, "setTimeout"));
            CREDO_VERUM (_continet(js, "Date.now()"));
            CREDO_VERUM (_continet(js, ".nusquam"));
        }
    }

    imprimere("\n--- Semita felix (pulsus plus quam unus) ---\n");
    {
        i32    portus = 0;
        s32    proles = _simulacrum_incipere(SCEN_VERUM, &portus);
        Manus* m;

        CREDO_VERUM (proles > 0);
        m = manus_aperire(piscina, "127.0.0.1", portus);
        CREDO_NON_NIHIL (m);
        si (m != NIHIL)
        {
            /* Simulacrum PENDENS primo pulsu reddit: si haec
             * transit, gyrus pulsationis re vera gyrat. */
            CREDO_MANUS_EXISTIT_MORA (m, ".tessera", MANUS_MORA_BREVIS);
            CREDO_FALSUM (manus_fracta(m));
            manus_claudere(m);
        }
        _simulacrum_finire(proles);
    }

    imprimere("\n--- Applicatio mortua: manus NIHIL, non manus muta ---\n");
    {
        i32    portus = 0;
        s32    proles = _simulacrum_incipere(SCEN_VERUM, &portus);
        Manus* m;

        _simulacrum_finire(proles);   /* portus nunc vacuus */

        /* Manus muta omnia asserta silentio praeterire faceret -
         * suita viridis contra applicationem quae numquam
         * cucurrit. Ergo NIHIL. */
        m = manus_aperire(piscina, "127.0.0.1", portus);
        CREDO_NIHIL (m);
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
