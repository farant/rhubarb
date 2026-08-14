#include "postulata_posix.h"

#include "manus.h"
#include "credo.h"
#include "piscina.h"

#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
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

/* Scaenaria pro operationibus singulis: verdictum PLACITUM reddunt,
 * ut semita C cuiusque operationis probetur (quid reddat, an manum
 * frangat, quid causa dicat) sine pictore. */
#define SCEN_OK       V    /* ok:true, visum "factum"            */
#define SCEN_RECUSANS VI   /* ok:false cum causa NOMINATA        */
#define SCEN_ERRORES  VII  /* visum "3|exceptio: ..." (parsura)  */
#define SCEN_SINE_COLL VIII /* visum "-1|..." collector abest    */
#define SCEN_AFFORD   IX   /* tabulatum affordantiarum           */
#define SCEN_AFFORD_PRAVUS X /* responsum quod tabulatum NON est */
#define SCEN_LECTIO   XI   /* tabulatum tabulatorum (ordines)   */

/* Operationes quas _agere_capere exercet. */
#define OP_NULLUM        0   /* nihil - JS collectoris capitur */
#define OP_PREMERE       I
#define OP_SCRIBERE      II
#define OP_PREMERE_TEXTUM III
#define OP_EXISTIT       IV
#define OP_NUMERUS       V
#define OP_TEXTUS        VI
#define OP_ABEST         VII
#define OP_ABEST_OMNINO  VIII
#define OP_TEXTUM_PAGINAE IX
#define OP_NUMERUS_MORA  X
#define OP_TEXTUS_MORA   XI
#define OP_TEXTUM_MORA   XII
#define OP_ERRORES       XIII
#define OP_EXISTIT_ASS   XIV
#define OP_ABEST_MORA    XV
#define OP_NUMERUS_ASS   XVI
#define OP_TEXTUS_ASS    XVII
#define OP_TEXTUS_CONT   XVIII
#define OP_TEXTUM_ABEST  XIX
#define OP_AESTIMARE     XX
#define OP_IMAGO         XXI
#define OP_IMAGO_CULPAE  XXII
#define OP_PURGARE       XXIII
#define OP_CONT_MORA     XXIV
#define OP_TXT_ABEST_MORA XXV
#define OP_OMNINO_MORA   XXVI
#define OP_REFICERE      XXVII
#define OP_AFFORD        XXVIII
#define OP_VOLVERE       XXIX
#define OP_VOLVERE_AD    XXX
#define OP_LEGERE        XXXI
#define OP_TEXTUM_VACUUM XXXII
#define OP_EXSPECTARE    XXXIII
#define OP_EXSPECTARE_NO XXXIV
#define OP_ABEST_EXSP    XXXV

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
        /* EXHAURIENDUM ANTE RESPONSUM - et spatium amplum.
         *
         * MENSURATUM 2026-08-13: petitio affordantiarum IV milia
         * octetorum excessit (praeambulum JS crevit), et simulacrum
         * SEMEL legebat in fuste IV milium. Quod restabat in
         * receptaculo manebat; close() cum datis non lectis RST
         * mittit, quod responsum IAM SCRIPTUM delet - unde cliens
         * 'applicatio iussum non accepit' vidit.
         *
         * Vitium LATENS erat: dum petitiones sub limine manebant,
         * lectio una sufficiebat. Mutatio mea limen transiit, non
         * vitium creavit.
         *
         * (Eadem lex quae fixturis http/tcp anno praeterito imposita
         *  est: accipe obstruens, EXHAURI, deinde claude.) */
        character petitio[XVI * M];
        character corpus[DXII];
        s32       fd;
        s32       lecta;
        s32       summa = 0;
        s32       codex = CC;

        fd = (s32)accept(fd_auscultans, NIHIL, NIHIL);
        si (fd < 0)
        {
            perge;
        }

        dum (summa < (s32)(magnitudo(petitio) - I))
        {
            constans character* finis_capitum;

            lecta = (s32)read(fd, petitio + summa,
                              magnitudo(petitio) - I - (memoriae_index)summa);
            si (lecta <= 0)
            {
                frange;
            }
            summa += lecta;
            petitio[summa] = '\0';

            finis_capitum = strstr(petitio, "\r\n\r\n");
            si (finis_capitum != NIHIL)
            {
                constans character* cl = strstr(petitio, "Content-Length:");
                s32                 longitudo_corporis = 0;
                s32                 habemus;

                si (cl != NIHIL)
                {
                    longitudo_corporis = (s32)atoi(cl + XV);
                }
                habemus = summa - (s32)(finis_capitum + IV - petitio);
                si (habemus >= longitudo_corporis)
                {
                    frange;
                }
            }
        }

        si (summa <= 0)
        {
            (vacuum)close(fd);
            perge;
        }

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
        alioquin si (scenario == SCEN_OK)
        {
            sprintf(corpus, "{\"status\":\"perfectum\","
                            "\"valor\":{\"ok\":true,\"visum\":\"factum\"}}");
        }
        alioquin si (scenario == SCEN_RECUSANS)
        {
            /* Causa NOMINATA, ut probemus eam per manum ad nuntium
             * fracturae intactam pervenire. */
            sprintf(corpus, "{\"status\":\"perfectum\",\"valor\":{"
                "\"ok\":false,"
                "\"visum\":\"elementum impeditum (disabled)\"}}");
        }
        alioquin si (scenario == SCEN_ERRORES)
        {
            sprintf(corpus, "{\"status\":\"perfectum\",\"valor\":{"
                "\"ok\":false,"
                "\"visum\":\"3|exceptio: ReferenceError\"}}");
        }
        alioquin si (scenario == SCEN_SINE_COLL)
        {
            sprintf(corpus, "{\"status\":\"perfectum\",\"valor\":{"
                "\"ok\":true,\"visum\":\"-1|collector abest\"}}");
        }
        alioquin si (scenario == SCEN_AFFORD)
        {
            /* x NEGATIVUM consulto: elementum supra marginem
             * volutum. Campi loci SIGNATI sunt, et si quis eos in
             * i32 (insignatum) verteret, -V in numerum immanem
             * abiret - decipula domus nota. */
            sprintf(corpus, "{\"status\":\"perfectum\",\"valor\":["
                "{\"genus\":1,\"selector\":\"#b1\",\"titulus\":\"Salve\","
                "\"valor\":\"\",\"impedimentum\":\"\","
                "\"x\":8,\"y\":10,\"latitudo\":32,\"altitudo\":15},"
                "{\"genus\":2,\"selector\":\"#i1\",\"titulus\":\"scribe\","
                "\"valor\":\"textus\",\"impedimentum\":\"impedita\","
                "\"x\":-5,\"y\":9,\"latitudo\":146,\"altitudo\":17}]}");
        }
        alioquin si (scenario == SCEN_LECTIO)
        {
            /* Ordines LONGITUDINIS DIVERSAE consulto: numerus
             * columnarum per ordinem variat (filii visibiles), et
             * parsura id ferre debet. */
            sprintf(corpus, "{\"status\":\"perfectum\",\"valor\":"
                "[[\"totum\",\"179.1s\",\"187.6s\"],[\"solum\"]]}");
        }
        alioquin si (scenario == SCEN_AFFORD_PRAVUS)
        {
            /* Forma recta, contentum pravum: pagina respondit sed
             * non tabulato. Index vacuus hic 'nihil adest'
             * MENTIRETUR - ergo manus frangi DEBET. */
            sprintf(corpus, "{\"status\":\"perfectum\",\"valor\":{"
                "\"ok\":true,\"visum\":\"non sum tabulatum\"}}");
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

interior vacuum
_js_legere (character* destinatio, memoriae_index maximum);

nomen structura {
    b32            manus_aperta;
    b32            manus_fracta;
    b32            notatum;
    memoriae_index totalis;      /* quot asserta post DUO vocamina */
    character      genus[LXIV];
    character      exspectatum[CCLVI];
    character      visum[DXII];
    character      causa[DXII];
    /* JS huius capturae, statim lectus: capturae sequentes plagulam
     * SUPERSCRIBUNT, ergo qui eam postea legit alienum inspicit. */
    character      js[MMMMXCVI];
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
    _js_legere(c.js, magnitudo(c.js));
    piscina_destruere(p);
    _simulacrum_finire(proles);
    redde c;
}

/* ========================================================================
 * Actio capta: quid operatio reddat, et QUOD JS miserit
 *
 * Duo probantur simul, quia unus cursus utrumque parit:
 *   (a) semita C - valor redditus, fractura, textus causae;
 *   (b) FORMA JS geniti - custos regressionis. Si quis 'act(' ex
 *       actionibus tulerit, aut 'qt(' ex petitione textuali, hic
 *       clamatur. Semita illa aliter SOLA probatione fumi tegitur,
 *       quae fenestram aperit et in suita automatica NUMQUAM currit.
 *
 * QUOD HIC NON PROBATUR: an JS illud in pictore recte AGAT. Id
 * probatio fumi sola potest; simulacrum quod paginam fingeret 'JS
 * meus contra DOM meum' esset.
 * ======================================================================== */

nomen structura {
    b32       fructus;      /* quod operatio reddidit */
    b32       fracta;
    i32       numerus;      /* pro OP_ERRORES */
    character causa[DXII];
    character primus[CCLVI];
    character js[MMMMXCVI];
} Actio;

interior vacuum
_js_legere (
    character*     destinatio,
    memoriae_index maximum)
{
    FILE* f = fopen(VIA_ULTIMI, "r");
    i32   lecta = 0;

    destinatio[0] = '\0';
    si (f == NIHIL)
    {
        redde;
    }
    lecta = (i32)fread(destinatio, I, maximum - I, f);
    (vacuum)fclose(f);
    destinatio[lecta] = '\0';
}

interior Actio
_agere_capere (
    s32 scenario,
    s32 operatio)
{
    Actio     a;
    Piscina*  p;
    Manus*    m;
    i32       portus = 0;
    s32       proles;

    memset(&a, 0, magnitudo(a));
    proles = _simulacrum_incipere(scenario, &portus);
    si (proles < 0)
    {
        redde a;
    }

    p = piscina_generare_dynamicum("actio_manus", CXXVIII * M);
    /* Sessio SEPARATA: asserta hic notata (multa consulto fallentia)
     * suitam principalem non rubent. */
    credo_aperire(p);

    m = manus_aperire(p, "127.0.0.1", portus);
    si (m != NIHIL)
    {
        commutatio (operatio)
        {
        casus OP_NULLUM:
            a.fructus = VERUM;
            frange;
        casus OP_EXSPECTARE:
            a.fructus = manus_exspectare(m, ".tessera", VERUM,
                                         MANUS_MORA_BREVIS);
            frange;
        casus OP_EXSPECTARE_NO:
            a.fructus = manus_exspectare(m, ".numquam", VERUM,
                                         MANUS_MORA_BREVIS);
            frange;
        casus OP_ABEST_EXSP:
            a.fructus = manus_exspectare(m, ".rota", FALSUM,
                                         MANUS_MORA_BREVIS);
            frange;
        casus OP_LEGERE:
            {
                Lectio l = manus_legere(m, "tr", p);

                a.numerus = l.numerus;
                a.fructus = VERUM;
                si (l.numerus > ZEPHYRUM)
                {
                    sprintf(a.primus, "%d:%.*s|%.*s|%.*s",
                        (integer)l.lineae[0].numerus,
                        (integer)l.lineae[0].cellulae[0].mensura,
                        (constans character*)l.lineae[0].cellulae[0].datum,
                        (integer)l.lineae[0].cellulae[1].mensura,
                        (constans character*)l.lineae[0].cellulae[1].datum,
                        (integer)l.lineae[0].cellulae[2].mensura,
                        (constans character*)l.lineae[0].cellulae[2].datum);
                }
                si (l.numerus > I)
                {
                    character cauda[LXIV];
                    sprintf(cauda, " |%d:%.*s",
                        (integer)l.lineae[1].numerus,
                        (integer)l.lineae[1].cellulae[0].mensura,
                        (constans character*)l.lineae[1].cellulae[0].datum);
                    strcat(a.primus, cauda);
                }
            }
            frange;
        casus OP_TEXTUM_VACUUM:
            a.fructus = manus_premere_textum(m, "");
            frange;
        casus OP_VOLVERE:
            a.fructus = manus_volvere(m, (s32)CC);
            frange;
        casus OP_VOLVERE_AD:
            a.fructus = manus_volvere_ad(m, "#imum");
            frange;
        casus OP_AFFORD:
            {
                Affordantiae aff = manus_affordantiae(m, p);

                a.numerus = aff.numerus;
                a.fructus = VERUM;
                si (aff.numerus > ZEPHYRUM)
                {
                    /* Omnis campus PRIMI in unam chordam, ut assertum
                     * unum totam semitam parsurae iudicet. */
                    sprintf(a.primus,
                        "%d|%.*s|%.*s|%.*s|%.*s|%d,%d,%dx%d",
                        (integer)aff.res[0].genus,
                        (integer)aff.res[0].selector.mensura,
                        (constans character*)aff.res[0].selector.datum,
                        (integer)aff.res[0].titulus.mensura,
                        (constans character*)aff.res[0].titulus.datum,
                        (integer)aff.res[0].valor.mensura,
                        (constans character*)aff.res[0].valor.datum,
                        (integer)aff.res[0].impedimentum.mensura,
                        (constans character*)aff.res[0].impedimentum.datum,
                        (integer)aff.res[0].x, (integer)aff.res[0].y,
                        (integer)aff.res[0].latitudo,
                        (integer)aff.res[0].altitudo);
                }
                si (aff.numerus > I)
                {
                    /* SECUNDI solum quae parsuram probant: x
                     * negativum et impedimentum non vacuum. */
                    character cauda[CXXVIII];
                    sprintf(cauda, " |2:%d|%.*s|%d",
                        (integer)aff.res[1].genus,
                        (integer)aff.res[1].impedimentum.mensura,
                        (constans character*)aff.res[1].impedimentum.datum,
                        (integer)aff.res[1].x);
                    strcat(a.primus, cauda);
                }
            }
            frange;
        casus OP_PREMERE:
            a.fructus = manus_premere(m, "#pyxis");
            frange;
        casus OP_SCRIBERE:
            a.fructus = manus_scribere(m, "#campus", "textus datus");
            frange;
        casus OP_PREMERE_TEXTUM:
            a.fructus = manus_premere_textum(m, "condere");
            frange;
        casus OP_EXISTIT:
            a.fructus = manus_existit(m, ".tessera");
            frange;
        casus OP_NUMERUS:
            a.numerus = manus_numerus(m, ".tessera");
            a.fructus = VERUM;
            frange;
        casus OP_TEXTUS:
            {
                chorda t = manus_textus(m, "#titulus");
                a.fructus = (t.mensura > 0) ? VERUM : FALSUM;
            }
            frange;
        casus OP_ABEST:
            a.fructus = CREDO_MANUS_ABEST(m, ".nulla");
            frange;
        casus OP_ABEST_OMNINO:
            a.fructus = CREDO_MANUS_ABEST_OMNINO(m, ".nulla");
            frange;
        casus OP_TEXTUM_PAGINAE:
            a.fructus = CREDO_MANUS_TEXTUM(m, "salve munde");
            frange;
        casus OP_NUMERUS_MORA:
            a.fructus = CREDO_MANUS_NUMERUS_MORA(m, ".t", (i32)III,
                                                 MANUS_MORA_BREVIS);
            frange;
        casus OP_TEXTUS_MORA:
            a.fructus = CREDO_MANUS_TEXTUS_MORA(m, "#t", "quid",
                                                MANUS_MORA_BREVIS);
            frange;
        casus OP_TEXTUM_MORA:
            a.fructus = CREDO_MANUS_TEXTUM_MORA(m, "quid",
                                                MANUS_MORA_BREVIS);
            frange;
        casus OP_EXISTIT_ASS:
            a.fructus = CREDO_MANUS_EXISTIT(m, ".t");
            frange;
        casus OP_ABEST_MORA:
            a.fructus = CREDO_MANUS_ABEST_MORA(m, ".t",
                                               MANUS_MORA_BREVIS);
            frange;
        casus OP_NUMERUS_ASS:
            a.fructus = CREDO_MANUS_NUMERUS(m, ".t", (i32)III);
            frange;
        casus OP_TEXTUS_ASS:
            a.fructus = CREDO_MANUS_TEXTUS(m, "#t", "quid");
            frange;
        casus OP_TEXTUS_CONT:
            a.fructus = CREDO_MANUS_TEXTUS_CONTINET(m, "#t", "qu");
            frange;
        casus OP_TEXTUM_ABEST:
            a.fructus = CREDO_MANUS_TEXTUM_ABEST(m, "nusquam");
            frange;
        casus OP_AESTIMARE:
            {
                chorda v = manus_aestimare(m, "1+1", MANUS_MORA_BREVIS);
                _copiare(a.primus, v, magnitudo(a.primus));
                a.fructus = (v.mensura > 0) ? VERUM : FALSUM;
            }
            frange;
        casus OP_IMAGO:
            a.fructus = manus_imaginem(m, "build/probatio_imago.png");
            frange;
        casus OP_IMAGO_CULPAE:
            /* Semita _frangere -> _imaginem: probatur ne recurrat
             * neque causam ORIGINALEM superscribat. */
            manus_imaginem_culpae_ponere(m, "build/probatio_culpa.png");
            a.fructus = manus_premere(m, "#pyxis");
            frange;
        casus OP_PURGARE:
            /* Post purgationem numerus ad ZEPHYRUM cadat, quamvis
             * scaenarium adhuc "3|..." reddat: purgare acervum
             * PAGINAE vacuat, ergo interrogatio sequens nihil
             * inveniat. (Simulacrum responsum idem reddit, ergo
             * hoc probat vocamen MISSUM esse - JS eius in a.js
             * inspicitur.) */
            manus_errores_purgare(m);
            a.fructus = VERUM;
            frange;
        casus OP_CONT_MORA:
            a.fructus = CREDO_MANUS_TEXTUS_CONTINET_MORA(m, "#t", "qu",
                                                    MANUS_MORA_BREVIS);
            frange;
        casus OP_TXT_ABEST_MORA:
            a.fructus = CREDO_MANUS_TEXTUM_ABEST_MORA(m, "nusquam",
                                                    MANUS_MORA_BREVIS);
            frange;
        casus OP_OMNINO_MORA:
            a.fructus = CREDO_MANUS_ABEST_OMNINO_MORA(m, ".n",
                                                    MANUS_MORA_BREVIS);
            frange;
        casus OP_REFICERE:
            /* Actio fallit -> manus fracta -> reficere temptatur.
             * Applicatione VIVA reficitur; MORTUA fracta manet. */
            (vacuum)manus_premere(m, "#pyxis");
            a.numerus = manus_fracta(m) ? (i32)I : (i32)ZEPHYRUM;
            a.fructus = manus_reficere(m);
            frange;
        casus OP_ERRORES:
            {
                chorda primus;
                a.numerus = manus_errores(m, &primus);
                _copiare(a.primus, primus, magnitudo(a.primus));
                a.fructus = VERUM;
            }
            frange;
        ordinarius:
            frange;
        }
        a.fracta = manus_fracta(m);
        _copiare(a.causa, manus_causa(m), magnitudo(a.causa));
        manus_claudere(m);
    }

    credo_claudere();
    _js_legere(a.js, magnitudo(a.js));
    piscina_destruere(p);
    _simulacrum_finire(proles);
    redde a;
}

/* Omnes actiones captae. Sessio credo GLOBALIS est, et _agere_capere
 * eam claudit - ergo capturae OMNES ante sessionem principalem fiant,
 * et phasis secunda de eis solum interroget. (Prima forma capiebat
 * INTRA sessionem principalem: prima captura eam clausit et sequens
 * assertum 'credo nunquam aperitum' clamavit.) */
nomen structura {
    Actio premere_ok, premere_no, scribere_ok;
    Actio textualis_ok, textualis_no;
    Actio collector, existit_ok, existit_no;
    Actio omnino, paginae, abest, abest_no;
    Actio n_mora, t_mora, x_mora;
    Actio numerus, textus;
    Actio cum_err, sine_c;
    Actio existit_ass, abest_mora, numerus_ass;
    Actio textus_ass, textus_cont, textum_abest;
    Actio aestimare, imago, imago_culpae;
    Actio purgare, cont_mora, txt_abest_mora, omnino_mora;
    Actio reficere_vivax, reficere_mortua;
    Actio afford, afford_pravus;
    Actio volvere, volvere_ad;
    Actio legere, textum_vacuum;
    Actio exsp, exsp_no, abest_exsp;
} Omnia;

interior vacuum
_omnia_capere (
    Omnia* o)
{
    o->premere_ok   = _agere_capere(SCEN_OK,        OP_PREMERE);
    o->premere_no   = _agere_capere(SCEN_RECUSANS,  OP_PREMERE);
    o->scribere_ok  = _agere_capere(SCEN_OK,        OP_SCRIBERE);
    o->textualis_ok = _agere_capere(SCEN_OK,        OP_PREMERE_TEXTUM);
    o->textualis_no = _agere_capere(SCEN_RECUSANS,  OP_PREMERE_TEXTUM);
    o->collector    = _agere_capere(SCEN_OK,        OP_NULLUM);
    o->existit_ok   = _agere_capere(SCEN_OK,        OP_EXISTIT);
    o->existit_no   = _agere_capere(SCEN_RECUSANS,  OP_EXISTIT);
    o->omnino       = _agere_capere(SCEN_OK,        OP_ABEST_OMNINO);
    o->paginae      = _agere_capere(SCEN_OK,        OP_TEXTUM_PAGINAE);
    o->abest        = _agere_capere(SCEN_OK,        OP_ABEST);
    o->abest_no     = _agere_capere(SCEN_RECUSANS,  OP_ABEST);
    o->n_mora       = _agere_capere(SCEN_OK,        OP_NUMERUS_MORA);
    o->t_mora       = _agere_capere(SCEN_OK,        OP_TEXTUS_MORA);
    o->x_mora       = _agere_capere(SCEN_OK,        OP_TEXTUM_MORA);
    o->numerus      = _agere_capere(SCEN_OK,        OP_NUMERUS);
    o->textus       = _agere_capere(SCEN_OK,        OP_TEXTUS);
    o->cum_err      = _agere_capere(SCEN_ERRORES,   OP_ERRORES);
    o->sine_c       = _agere_capere(SCEN_SINE_COLL, OP_ERRORES);
    o->existit_ass  = _agere_capere(SCEN_OK,        OP_EXISTIT_ASS);
    o->abest_mora   = _agere_capere(SCEN_OK,        OP_ABEST_MORA);
    o->numerus_ass  = _agere_capere(SCEN_OK,        OP_NUMERUS_ASS);
    o->textus_ass   = _agere_capere(SCEN_OK,        OP_TEXTUS_ASS);
    o->textus_cont  = _agere_capere(SCEN_OK,        OP_TEXTUS_CONT);
    o->textum_abest = _agere_capere(SCEN_OK,        OP_TEXTUM_ABEST);
    o->aestimare    = _agere_capere(SCEN_OK,        OP_AESTIMARE);
    o->imago        = _agere_capere(SCEN_OK,        OP_IMAGO);
    o->imago_culpae = _agere_capere(SCEN_RECUSANS,  OP_IMAGO_CULPAE);
    o->exsp           = _agere_capere(SCEN_OK,     OP_EXSPECTARE);
    o->exsp_no        = _agere_capere(SCEN_FALSUM, OP_EXSPECTARE_NO);
    o->abest_exsp     = _agere_capere(SCEN_FALSUM, OP_ABEST_EXSP);
    o->legere         = _agere_capere(SCEN_LECTIO,        OP_LEGERE);
    o->textum_vacuum  = _agere_capere(SCEN_OK,            OP_TEXTUM_VACUUM);
    o->volvere        = _agere_capere(SCEN_OK,            OP_VOLVERE);
    o->volvere_ad     = _agere_capere(SCEN_OK,            OP_VOLVERE_AD);
    o->afford         = _agere_capere(SCEN_AFFORD,        OP_AFFORD);
    o->afford_pravus  = _agere_capere(SCEN_AFFORD_PRAVUS, OP_AFFORD);
    o->purgare        = _agere_capere(SCEN_OK, OP_PURGARE);
    o->cont_mora      = _agere_capere(SCEN_OK, OP_CONT_MORA);
    o->txt_abest_mora = _agere_capere(SCEN_OK, OP_TXT_ABEST_MORA);
    o->omnino_mora    = _agere_capere(SCEN_OK, OP_OMNINO_MORA);
    o->reficere_vivax = _agere_capere(SCEN_RECUSANS, OP_REFICERE);
    o->reficere_mortua = _agere_capere(SCEN_PENDENS, OP_REFICERE);
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
    Omnia    o;
    b32      praeteritus;

    /* Prolis mors dum scribimus probationem occidere non debet. */
    (vacuum)signal(SIGPIPE, SIG_IGN);

    /* ========================================================
     * PHASIS I: fracturas capere (ANTE sessionem principalem)
     * ======================================================== */

    falsum  = _capere(SCEN_FALSUM,  ".nusquam");
    pendens = _capere(SCEN_PENDENS, ".nusquam");
    culpa   = _capere(SCEN_CULPA,   ".nusquam");
    _omnia_capere(&o);

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
        /* Ex CAPTURA, non ex plagula: capturae posteriores eam
         * superscribunt. Custos regressionis - si quis moram in C
         * reduxerit, haec rumpuntur, et debent. Mora IN PAGINA fit. */
        CREDO_VERUM (_continet(falsum.js, "new Promise"));
        CREDO_VERUM (_continet(falsum.js, "setTimeout"));
        CREDO_VERUM (_continet(falsum.js, "Date.now()"));
        CREDO_VERUM (_continet(falsum.js, ".nusquam"));
    }

    imprimere("\n--- Actiones: valor, fractura, causa ---\n");
    {
        CREDO_VERUM  (o.premere_ok.fructus);
        CREDO_FALSUM (o.premere_ok.fracta);

        /* Recusatio: valor FALSUM, manus fracta, et causa PICTORIS
         * intacta usque ad nuntium - non 'falsum' nudum. */
        CREDO_FALSUM (o.premere_no.fructus);
        CREDO_VERUM  (o.premere_no.fracta);
        CREDO_VERUM  (_continet(o.premere_no.causa, "manus_premere"));
        CREDO_VERUM  (_continet(o.premere_no.causa, "#pyxis"));
        CREDO_VERUM  (_continet(o.premere_no.causa, "impeditum (disabled)"));

        CREDO_VERUM  (o.scribere_ok.fructus);
        CREDO_VERUM  (o.textualis_ok.fructus);
        CREDO_FALSUM (o.textualis_no.fructus);
        /* Petitio textualis textum QUAESITUM nominet, non selectorem
         * (qui non exstat). */
        CREDO_VERUM  (_continet(o.textualis_no.causa, "premere_textum"));
        CREDO_VERUM  (_continet(o.textualis_no.causa, "condere"));

        imprimere("  causa recusationis: '%s'\n", o.premere_no.causa);
    }

    imprimere("\n--- Forma JS geniti (custos regressionis) ---\n");
    {
        /* Collector post aperire iniectus - tres fontes, omnes. */
        CREDO_VERUM (_continet(o.collector.js, "__manus_errores"));
        CREDO_VERUM (_continet(o.collector.js, "unhandledrejection"));
        CREDO_VERUM (_continet(o.collector.js, "console.error"));

        /* PORTA AGIBILITATIS in actionibus. Si quis 'act(' tulerit,
         * pyxides impeditae et obtectae silentio succedant iterum -
         * duo viridia mendacia hodie mensurata. */
        CREDO_VERUM (_continet(o.premere_ok.js, "var c=act(e)"));
        CREDO_VERUM (_continet(o.premere_ok.js, "e.click()"));
        CREDO_VERUM (_continet(o.scribere_ok.js, "var c=act(e)"));
        CREDO_VERUM (_continet(o.scribere_ok.js, "e.value="));
        /* Eventus manu immissi: sine iis facies reactiva nihil sentit. */
        CREDO_VERUM (_continet(o.scribere_ok.js, "dispatchEvent"));
        CREDO_VERUM (_continet(o.scribere_ok.js, "'input'"));

        /* Petitio textualis per qt() it (elementum IMUM), non per
         * querySelector. */
        CREDO_VERUM (_continet(o.textualis_ok.js, "var e=qt("));
        CREDO_VERUM (_continet(o.textualis_ok.js, "var c=act(e)"));

        /* VISIBILITAS: sondae et asserta per qn(), non per
         * querySelectorAll crudum. */
        CREDO_VERUM  (_continet(o.existit_ok.js, "var n=qn("));
        CREDO_FALSUM (_continet(o.existit_ok.js, "document.querySelectorAll(\""));

        /* ABEST_OMNINO CONTRA: crudum consulto - solum hoc assertum
         * de DOM ipso loquitur, non de eo quod usor videt. */
        CREDO_VERUM (_continet(o.omnino.js, "document.querySelectorAll("));

        CREDO_VERUM (_continet(o.paginae.js, "var n=qtn("));

        /* Mora IN PAGINA manet: gyrus C nihil de condicione scit. */
        CREDO_VERUM (_continet(o.premere_ok.js, "new Promise"));
        CREDO_VERUM (_continet(o.premere_ok.js, "setTimeout"));

        /* SCRIBERE: assignatio PROBANDA.
         *
         * Vitium mensuratum ab agente qui ut usor tractabat:
         * 'scribere #select <textus visibilis>' exitum ZEPHYRUM
         * dabat et NIHIL mutabat, quia .value optionem congruentem
         * quaerit. Tria custodimus - congruentia optionum, recusatio
         * elementi sine valore, relectio post assignationem.
         *
         * MORES ipsi HINC NON probantur (simulacrum paginam non
         * habet); probatio VIVA contra applicationem currentem eos
         * iudicavit. Haec custodia est ne forma redeat. */
        CREDO_VERUM (_continet(o.scribere_ok.js, "e.tagName==='SELECT'"));
        CREDO_VERUM (_continet(o.scribere_ok.js, "e.selectedIndex=o.index"));
        CREDO_VERUM (_continet(o.scribere_ok.js, "nulla optio congruit"));
        CREDO_VERUM (_continet(o.scribere_ok.js, "valorem non habet"));
        CREDO_VERUM (_continet(o.scribere_ok.js, "valor non mansit"));

        /* TEXTUS: innerText, NON textContent.
         *
         * 'textus body' fontem <script> totum reddebat (XXXII milia
         * octetorum) - textContent enim quae usor videre NON POTEST
         * quoque legit, quod legem 'VISIBILE, NON PRAESENS' huius
         * bibliothecae frangebat. */
        CREDO_VERUM  (_continet(o.textus.js, "innerText"));
        CREDO_VERUM  (_continet(o.textus.js, "e.tagName==='SELECT'"));

        /* SPATIA COACTA in petitione textuali.
         *
         * Vitium mensuratum: 'praevolatus 75.0s' fefellit dum textus
         * in schermo idem esset - pagina enim DUO spatia scribit et
         * navigatrum ea in unum cogit. Petitio quod usor LEGIT
         * describere debet, non quod fons scribit. */
        CREDO_VERUM (_continet(o.textualis_ok.js, "function _nz("));
        CREDO_VERUM (_continet(o.textualis_ok.js, "_nz(_tx(e)).indexOf(q)"));

        /* ABSENTIA suum nuntium habet, distinctum ab agibilitate:
         * 'nihil hoc fert' et 'adest sed premi non potest' duo
         * vitia sunt et olim unum nomen ferebant. */
        CREDO_VERUM (_continet(o.textualis_ok.js, "hunc textum fert"));
        CREDO_VERUM (_continet(o.premere_ok.js, "huic selectori congruit"));

        /* ICTUS SVG: HTMLElement solus '.click()' habet. Sine hac
         * semita elementa picturae - virgae flammae, puncta
         * tendentiae - omnino premi non poterant. */
        CREDO_VERUM (_continet(o.premere_ok.js, "typeof e.click==='function'"));
        CREDO_VERUM (_continet(o.premere_ok.js, "new MouseEvent('click'"));

        /* Volvere: SPECTANDI causa, ergo porta agibilitatis ABEST
         * consulto - ad pyxidem impeditam volvere legitimum est. */
        CREDO_VERUM  (_continet(o.volvere_ad.js, "scrollIntoView"));
        CREDO_FALSUM (_continet(o.volvere_ad.js, "var c=act(e)"));
        CREDO_VERUM  (_continet(o.volvere.js, "window.scrollBy(0,200)"));
    }

    imprimere("\n--- Volvere ---\n");
    {
        CREDO_VERUM (o.volvere.fructus);
        CREDO_VERUM (o.volvere_ad.fructus);
        CREDO_FALSUM (o.volvere.fracta);
    }

    imprimere("\n--- Exspectatio: interrogationes quoque morantur ---\n");
    {
        /* ASYMMETRIA quam haec claudit: actiones IAM morabantur
         * (per _agere cum promisso), interrogationes NON. Scriptum
         * crustae ergo agere poterat sed non legere, et ad somnum
         * retrudebatur - quod haec bibliotheca vetat. */
        CREDO_VERUM  (o.exsp.fructus);
        CREDO_FALSUM (o.exsp.fracta);

        /* Mora IN PAGINA, non hic: unum iter retis, et condiciones
         * TRANSEUNTES non elabuntur. */
        CREDO_VERUM (_continet(o.exsp.js, "new Promise"));
        CREDO_VERUM (_continet(o.exsp.js, "var n=qn("));

        /* Terminus transiens: FALSUM, manus fracta, causa NOMINATA -
         * et numerus visus quoque, nam 'nihil apparuit' et 'tria
         * manserunt' diversa sunt. */
        CREDO_FALSUM (o.exsp_no.fructus);
        CREDO_VERUM  (o.exsp_no.fracta);
        CREDO_VERUM  (_continet(o.exsp_no.causa, "nihil VISIBILE congruit"));
        CREDO_VERUM  (_continet(o.exsp_no.causa, "visa:"));

        /* ABSENTIA aliud verbum in causa habet: 'adhuc adest'. */
        CREDO_FALSUM (o.abest_exsp.fructus);
        CREDO_VERUM  (_continet(o.abest_exsp.causa, "adhuc adest"));
        CREDO_VERUM  (_continet(o.abest_exsp.js, "n===0"));
    }

    imprimere("\n--- Lectio: contentum structuratum ---\n");
    {
        /* Ordines longitudinis DIVERSAE: numerus columnarum ex
         * filiis visibilibus venit, ergo per ordinem variat.
         * Parsura quae longitudinem fixam praesumeret hic frangeret. */
        CREDO_AEQUALIS_I32 (o.legere.numerus, (i32)II);
        CREDO_FALSUM (o.legere.fracta);
        imprimere("  primus: '%s'\n", o.legere.primus);
        CREDO_VERUM (_continet(o.legere.primus, "3:totum|179.1s|187.6s"));
        CREDO_VERUM (_continet(o.legere.primus, "1:solum"));

        /* Lectio EANDEM legem visibilitatis et EANDEM coactionem
         * spatiorum adhibet quam petitio textualis - una definitio
         * eius quod usor legit, non duae. */
        CREDO_VERUM (_continet(o.legere.js, "function v(e)"));
        CREDO_VERUM (_continet(o.legere.js, "_nz(_tx(ch[j]))"));

        /* PETITIO TEXTUALIS VACUA: chorda vacua omnibus congrueret
         * et corpus premeret exitu ZEPHYRO. Bis hodie viridem
         * vacuum ita peperi. */
        CREDO_FALSUM (o.textum_vacuum.fructus);
        CREDO_VERUM  (o.textum_vacuum.fracta);
        CREDO_VERUM  (_continet(o.textum_vacuum.causa, "VACUA"));
    }

    imprimere("\n--- Asserta cetera + formae _MORA ---\n");
    {
        CREDO_VERUM (o.abest.fructus);
        CREDO_VERUM (o.omnino.fructus);
        CREDO_VERUM (o.paginae.fructus);
        /* Formae _MORA: usque ad hunc diem A NULLA probatione
         * vocatae - semita numquam cursa. */
        CREDO_VERUM (o.n_mora.fructus);
        CREDO_VERUM (o.t_mora.fructus);
        CREDO_VERUM (o.x_mora.fructus);
        CREDO_VERUM (_continet(o.n_mora.js, "var n=qn("));

        /* Assertum fallens manum frangit (differentia a sonda). */
        CREDO_FALSUM (o.abest_no.fructus);
        CREDO_VERUM  (o.abest_no.fracta);
    }

    imprimere("\n--- Sondae TACENT, asserta CLAMANT ---\n");
    {
        /* Sonda condicionem falsam reddit et manum INTACTAM linquit -
         * interrogatio est, non assertum. */
        CREDO_FALSUM (o.existit_no.fructus);
        CREDO_FALSUM (o.existit_no.fracta);

        CREDO_VERUM (o.numerus.fructus);
        CREDO_VERUM (o.textus.fructus);
    }

    imprimere("\n--- manus_errores: parsura 'N|primus' ---\n");
    {
        /* Chirurgia chordarum manu scripta circa '|' - pars
         * fragilissima bibliothecae, et usque ad hunc diem sine
         * ulla probatione automatica. Si haec parsura frangatur,
         * CREDO_MANUS_SINE_ERRORIBUS nullos errores in aeternum
         * nuntiet: viride quod nihil significat. */
        CREDO_AEQUALIS_I32 (o.cum_err.numerus, (i32)III);
        CREDO_VERUM (_continet(o.cum_err.primus, "exceptio: ReferenceError"));

        /* '-1' = collector abest. Numerus NEGATIVUS ad ZEPHYRUM
         * cadat, ne error fictus nuntietur. */
        CREDO_AEQUALIS_I32 (o.sine_c.numerus, (i32)ZEPHYRUM);

        /* Interrogatio manum non frangit utrolibet modo. */
        CREDO_FALSUM (o.cum_err.fracta);
        CREDO_FALSUM (o.sine_c.fracta);

        imprimere("  errores: %d, primus '%s'\n",
                  (integer)o.cum_err.numerus, o.cum_err.primus);
    }

    imprimere("\n--- Affordantiae: parsura et recusatio ---\n");
    {
        /* Quod hic probatur SEMITA C est: tabulatum JSON -> structurae.
         * An JS ipse elementa recte inveniat HINC NON APPARET - id
         * probatio fumi contra applicationem VIVAM iudicat (eadem lex
         * quae huic plagulae praeest). */
        CREDO_AEQUALIS_I32 (o.afford.numerus, (i32)II);
        CREDO_FALSUM (o.afford.fracta);

        imprimere("  primus: '%s'\n", o.afford.primus);

        /* Omnis campus primi, uno asserto. */
        CREDO_VERUM (_continet(o.afford.primus, "1|#b1|Salve|||8,10,32x15"));

        /* Secundus: impedimentum PERVENIT, et x NEGATIVUM negativum
         * MANET. Si quis campos loci in i32 (insignatum) verteret, -V
         * hic numerus immanis fieret - assertum quod decipulam domus
         * ipsam custodit. */
        CREDO_VERUM (_continet(o.afford.primus, "2:2|impedita|-5"));

        /* JS missum eandem legem visibilitatis fert quam actiones -
         * si quis enumerationem a v() separaret, index affordantias
         * ferret quas premere recusaret. */
        CREDO_VERUM (_continet(o.afford.js, "function v(e)"));
        CREDO_VERUM (_continet(o.afford.js, "getBoundingClientRect"));

        /* Iudicium impedimenti PROPRIUM est (_imp), non act().
         *
         * NOTA DE ASSERTO IPSO: primum hic 'scrollIntoView abest'
         * scripsi - et recte FRACTUM est. Praeambulum enim act()
         * DEFINIT (visus eum trahit), ergo textus adest etiamsi
         * enumeratio eum numquam vocet. Praesentia litterarum non
         * est mos vocandi; ergo quid enumeratio VOCET asserimus. */
        CREDO_VERUM (_continet(o.afford.js, "impedimentum:_imp(e,rc)"));

        /* Responsum quod tabulatum non est: manus FRANGITUR. Index
         * vacuus 'nihil adest' mentiretur - genus defectus quod haec
         * domus semper clamosum facit. */
        CREDO_AEQUALIS_I32 (o.afford_pravus.numerus, ZEPHYRUM);
        CREDO_VERUM (o.afford_pravus.fracta);
        CREDO_VERUM (_continet(o.afford_pravus.causa, "tabulatum"));
    }

    imprimere("\n--- Superficies reliqua ---\n");
    {
        CREDO_VERUM (o.existit_ass.fructus);
        CREDO_VERUM (o.abest_mora.fructus);
        CREDO_VERUM (o.numerus_ass.fructus);
        CREDO_VERUM (o.textus_ass.fructus);
        CREDO_VERUM (o.textus_cont.fructus);
        CREDO_VERUM (o.textum_abest.fructus);

        /* aestimare valorem CRUDUM reddit (JSON), non verdictum. */
        CREDO_VERUM (o.aestimare.fructus);
        CREDO_VERUM (_continet(o.aestimare.js, "1+1"));

        CREDO_VERUM  (o.imago.fructus);
        CREDO_FALSUM (o.imago.fracta);

        /* Imago culpae: actio fallit, manus frangitur, et causa
         * ORIGINALIS manet - captura eam non superscribit neque in
         * se recurrit. */
        CREDO_FALSUM (o.imago_culpae.fructus);
        CREDO_VERUM  (o.imago_culpae.fracta);
        CREDO_VERUM  (_continet(o.imago_culpae.causa, "impeditum"));

        /* Purgatio: acervum PAGINAE mutat in loco ('length=0'), non
         * reponit - custodes enim ad illum acervum ligati sunt. */
        CREDO_VERUM (o.purgare.fructus);
        CREDO_VERUM (_continet(o.purgare.js, "__manus_errores"));
        CREDO_VERUM (_continet(o.purgare.js, "a.length=0"));
        CREDO_FALSUM (o.purgare.fracta);

        /* Formae _MORA quae hodie desiderabantur (TEXTUS_CONTINET,
         * TEXTUM_ABEST, ABEST_OMNINO gemellos non habebant). */
        CREDO_VERUM (o.cont_mora.fructus);
        CREDO_VERUM (o.txt_abest_mora.fructus);
        CREDO_VERUM (o.omnino_mora.fructus);

        /* TERMINUS SECTIONIS. Applicatio VIVA: actio fefellit, manus
         * fracta est, reficere SUCCEDIT et manum integram linquit -
         * sectio sequens ergo re vera currit. */
        CREDO_AEQUALIS_I32 (o.reficere_vivax.numerus, (i32)I);
        CREDO_VERUM  (o.reficere_vivax.fructus);
        CREDO_FALSUM (o.reficere_vivax.fracta);

        /* Applicatio MORTUA: reficere RECUSAT et manus fracta manet.
         * Sine hac distinctione probatio cadaver per omnes sectiones
         * interrogaret, terminum plenum quaeque urens. */
        CREDO_FALSUM (o.reficere_mortua.fructus);
        CREDO_VERUM  (o.reficere_mortua.fracta);
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

    /* QUID HAEC PROBATIO NON PROBAT - impressum, non solum
     * commentatum: probatio quae limites suos tacet fidem parit quam
     * non meruit, et haec suita SPONTE currit dum fumus MANU. Qui
     * viride hic videt scire debet quid adhuc non probatum sit. */
    imprimere("\n  NOTA: haec structuram et fistulas probat - JS in"
              " pictore VERO agere\n"
              "        NON probatur. Id sola probatio fumi potest:\n"
              "        cd ../silicetum/laboratorium && ./fumus.sh\n"
              "        (fenestram aperit; a nullo cursore sponte"
              " curritur)\n");

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
