/* tools/apns.c - finis gradus I http2 (http2-plan T11): petitio APNs
 * sandbox per nostram struem HTTP/2 + TLS + tesseram ES256.
 *
 * Usus:
 *   apns [-configuratio <via>] -praeparare
 *       configuratio (~/.rhubarb/ios/apns.stml), clavis (modus 0600,
 *       PKCS#8 P-256 - lecta, numquam monstrata), capacitas
 *       PUSH_NOTIFICATIONS signi fasciculi (per nundinae_apple, solum
 *       inspicitur - creatio: nundinae_apple -capacitas <app>
 *       PUSH_NOTIFICATIONS)
 *   apns [-configuratio <via>] -probare [-signum <hex64>] [-clavis_id <id>]
 *       POST /3/device/<signum> (ordinarie LXIV '0') per mittentem servi
 *       (apns_mittens: eadem via quam pulsus, sine iterationibus,
 *       tessera semel renovata); regula spec III.4, semel iteratur
 *       signo fortuito si ratio alia. -clavis_id: tessera cum id falso
 *       (cursus secundus: rejectio exspectata).
 * Exitus: 0 transit / omnia parata | 1 defectus / aliquid deest |
 *         2 configuratio aut clavis mala (campus nominatur) */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "lectiones.h"
#include "tessera_es256.h"
#include "nundinae_apple.h"
#include "reactor.h"
#include "apns.h"
#include "apns_mittens.h"
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

interior s32
_usus (vacuum)
{
    fprintf(stderr, "usus: apns [-configuratio <via>] -praeparare | "
        "-probare [-signum <hex64>] [-clavis_id <id>]\n");
    redde II;
}

interior vacuum
_scribere (
     constans character* praefixum,
                 chorda  c)
{
    fprintf(stderr, "%s%.*s\n", praefixum, (int)c.mensura,
        (constans character*)c.datum);
}

/* clavis lecta et statim oblita: solum an valeat */
interior b32
_clavem_probare (
    constans ApnsConfiguratio* c,
                      Piscina* p)
{
      i8* der;
     i32  n;
  chorda  causa;

    si (!tessera_clavem_legere(chorda_ut_cstr(c->clavis_via, p), &der,
        &n,
            &causa, p))
    {
        _scribere("apns: ", causa);
        redde FALSUM;
    }
    memset(der, ZEPHYRUM, (size_t)n);
    imprimere("clavis: PKCS#8 P-256, modus 0600 (%.*s)\n",
        (int)c->clavis_via.mensura,
        (constans character*)c->clavis_via.datum);
    redde VERUM;
}

interior s32
_praeparare (
    constans ApnsConfiguratio* c,
                      Piscina* p)
{
    NundinaeAppleConfiguratio  cfg;
                NundinaeApple  n;
                       chorda  causa;
           constans character* domus;
                    character  via[DXII];
                          b32  inest;

    imprimere("configuratio: clavis_id %.*s, turma %.*s, thema %.*s, "
        "hospes %.*s:%u\n", (int)c->clavis_id.mensura,
        (constans character*)c->clavis_id.datum, (int)c->turma.mensura,
        (constans character*)c->turma.datum, (int)c->thema.mensura,
        (constans character*)c->thema.datum, (int)c->hospes.mensura,
        (constans character*)c->hospes.datum,
        (insignatus integer)c->portus);
    si (!_clavem_probare(c, p))
    {
        redde II;
    }
    domus = lectiones_ambitus("HOME");
    si (!domus || strlen(domus) > CD)
    {
        fprintf(stderr, "apns: HOME nullum\n");
        redde I;
    }
    sprintf(via, "%s/.rhubarb/ios/nundinae_apple.stml", domus);
    si (   !nundinae_apple_configurationem_legere(via, &cfg, &causa, p)
        || !nundinae_apple_aperire(&n, &cfg, &causa, p))
    {
        _scribere("apns: capacitas non inspecta - ", causa);
        redde I;
    }
    si (!nundinae_apple_capacitas(&n, c->thema, "PUSH_NOTIFICATIONS",
            FALSUM, &inest, NIHIL, &causa, p))
    {
        nundinae_apple_claudere(&n);
        _scribere("apns: capacitas non inspecta - ", causa);
        redde I;
    }
    nundinae_apple_claudere(&n);
    si (!inest)
    {
        imprimere("capacitas PUSH_NOTIFICATIONS: ABSENS - creare: "
            "nundinae_apple -capacitas <app> PUSH_NOTIFICATIONS\n");
        redde I;
    }
    imprimere("capacitas PUSH_NOTIFICATIONS: praesens\napns: PARATUM\n");
    redde ZEPHYRUM;
}

/* exitus missionis unius (finis mittentis) */
nomen structura {
             b32 factum;
    PulsusExitus exitus;
       character ratio[CCLVI];
} Eventus;

interior vacuum
_finis (
                   vacuum* ctx,
    constans PulsusMissio* missio,
             PulsusExitus  exitus,
                   chorda  ratio)
{
     Eventus* e = (Eventus*)ctx;
         i32  n;

    (vacuum)missio;
    n          = ratio.mensura < CCLV ? ratio.mensura : CCLV;
    e->factum  = VERUM;
    e->exitus  = exitus;
    memcpy(e->ratio, ratio.datum, (size_t)n);
    e->ratio[n] = '\0';
}

interior s64
_ms_nunc (vacuum)
{
    structura timeval tv;

    gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)tv.tv_usec / M;
}

/* via obstruens super mittentem servi: missio una, reactor donec finis
 * (XL s ad summum) */
interior b32
_mittere (
                  ApnsMittens* mittens,
                      Reactor* reactor,
    constans ApnsConfiguratio* c,
           constans character* signum,
                      Eventus* e,
                      Piscina* p)
{
    PulsusMissio m;
             s64 terminus;

    memset(e, ZEPHYRUM, magnitudo(Eventus));
    m.signum   = chorda_ex_literis(signum, p);
    m.ambitus  = chorda_ex_literis("productio", p);
    m.thema    = c->thema;
    m.corpus     =
        chorda_ex_literis("{\"aps\":{\"alert\":\"probatio\"}}",
        p);
    m.apparatus  = chorda_ex_literis("apns-probare", p);
    si (!apns_mittens_mittere(mittens, &m, _finis, e))
    {
        redde FALSUM;
    }
    terminus = _ms_nunc() + (s64)XL * M;
    dum (!e->factum && _ms_nunc() < terminus)
    {
        (vacuum)reactor_poll(reactor, L);
    }
    redde e->factum;
}

/* regula spec III.4 super exitum: TRADITUM aut SIGNUM_MORTUUM =
 * tessera accepta; DEFECTUS localis ("apns: ...") deficit; ratio Apple
 * per apns_iudicare */
interior ApnsIudicium
_iudicare (
    constans Eventus* e,
                 b32  signum_nullum,
             Piscina* p)
{
    ApnsResponsum r;

    si (e->factum && e->exitus != PULSUS_DEFECTUS)
    {
        redde APNS_TRANSITUS;
    }
    si (!e->factum || strncmp(e->ratio, "apns: ", VI) == ZEPHYRUM)
    {
        redde APNS_DEFECTUS;
    }
    memset(&r, ZEPHYRUM, magnitudo(r));
    r.transita  = VERUM;
    r.ratio     = chorda_ex_literis(e->ratio, p);
    redde apns_iudicare(&r, signum_nullum);
}

interior vacuum
_eventum_monstrare (
    constans character* signum,
      constans Eventus* e)
{
    hic_manens constans character* constans NOMINA[III] = {
        "TRADITUM", "SIGNUM_MORTUUM", "DEFECTUS"
    };

    si (!e->factum)
    {
        imprimere("signum %.8s...: nullum responsum intra XL s\n",
            signum);
        redde;
    }
    imprimere("signum %.8s...: %s (%s)\n", signum,
        NOMINA[(i32)e->exitus], e->ratio);
}

interior s32
_probare (
    constans ApnsConfiguratio* c,
           constans character* signum,
           constans character* clavis_id,
                      Piscina* p)
{
                i8* der;
               i32  n;
            chorda  causa;
  ApnsConfiguratio  cfg;
           Reactor* reactor;
       ApnsMittens* mittens;
           Eventus  e;
      ApnsIudicium  iudicium;
         character  fortuitum[LXV];

    si (!tessera_clavem_legere(chorda_ut_cstr(c->clavis_via, p), &der,
        &n, &causa, p))
    {
        _scribere("apns: ", causa);
        redde II;
    }
    cfg = *c;
    si (clavis_id)
    {
        cfg.clavis_id = chorda_ex_literis(clavis_id, p);
    }
    reactor  = reactor_creare(p);
    mittens  = reactor ? apns_mittentem_aperire(reactor, &cfg, der, n,
        NIHIL, p) : NIHIL;
    memset(der, ZEPHYRUM, (size_t)n);
    si (!mittens)
    {
        fprintf(stderr, "apns: tessera fieri non potest (clavis_id aut "
            "turma non tuta)\n");
        si (reactor)
        {
            reactor_destruere(reactor);
        }
        redde II;
    }
    apns_mittentis_moras_ponere(mittens, NIHIL, ZEPHYRUM);
    (vacuum)_mittere(mittens, reactor, &cfg, signum, &e, p);
    _eventum_monstrare(signum, &e);
    iudicium = _iudicare(&e, strcmp(signum, APNS_SIGNUM_NULLUM)
        == ZEPHYRUM, p);
    si (iudicium == APNS_ITERANDUM)
    {
        apns_signum_fortuitum(fortuitum, _ms_nunc() / M);
        imprimere("ratio alia post signum nullum: iterum signo fortuito\n");
        (vacuum)_mittere(mittens, reactor, &cfg, fortuitum, &e, p);
        _eventum_monstrare(fortuitum, &e);
        iudicium = _iudicare(&e, FALSUM, p);
    }
    apns_mittentem_claudere(mittens);
    reactor_destruere(reactor);
    si (iudicium == APNS_TRANSITUS)
    {
        imprimere("apns: TRANSIT - tessera ab Apple accepta\n");
        redde ZEPHYRUM;
    }
    imprimere("apns: DEFECTUS (%s)\n", e.factum ? e.ratio
        : "nullum responsum");
    redde I;
}

s32
principale (
         integer   argc,
       character** argv)
{
                Piscina* p;
       ApnsConfiguratio  c;
                 chorda  causa;
              character  via[DXII];
     constans character* verbum;
     constans character* signum;
     constans character* clavis_id;
     constans character* domus;
                    s32  exitus;
                integer  i;

    verbum         = NIHIL;
    signum         = APNS_SIGNUM_NULLUM;
    clavis_id      = NIHIL;
    via[ZEPHYRUM]  = '\0';
    per (i = I; i < argc; i++)
    {
        si (   strcmp(argv[i], "-configuratio") == ZEPHYRUM
            && i + I < argc)
        {
            si (strlen(argv[i + I]) >= magnitudo(via))
            {
                redde _usus();
            }
            strcpy(via, argv[++i]);
        }
        alioquin si (   strcmp(argv[i], "-signum") == ZEPHYRUM
                     && i + I < argc)
        {
            signum = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-clavis_id") == ZEPHYRUM
                     && i + I < argc)
        {
            clavis_id = argv[++i];
        }
        alioquin si (   strcmp(argv[i], "-praeparare") == ZEPHYRUM
                     || strcmp(argv[i], "-probare")    == ZEPHYRUM)
        {
            verbum = argv[i];
        }
        alioquin
        {
            redde _usus();
        }
    }
    si (!verbum || strlen(signum) != LXIV)
    {
        redde _usus();
    }
    p = piscina_generare_dynamicum("apns", M * M);
    si (!p)
    {
        redde I;
    }
    si (via[ZEPHYRUM] == '\0')
    {
        domus = lectiones_ambitus("HOME");
        si (!domus || strlen(domus) > CD)
        {
            fprintf(stderr, "apns: HOME nullum\n");
            piscina_destruere(p);
            redde II;
        }
        sprintf(via, "%s/.rhubarb/ios/apns.stml", domus);
    }
    si (!apns_configurationem_legere(via, &c, &causa, p))
    {
        _scribere("", causa);
        piscina_destruere(p);
        redde II;
    }
    exitus = strcmp(verbum, "-praeparare") == ZEPHYRUM
        ? _praeparare(&c, p) : _probare(&c, signum, clavis_id, p);
    piscina_destruere(p);
    redde exitus;
}
