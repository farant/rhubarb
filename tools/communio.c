/* tools/communio.c - instrumentum communionis in apparatu (communio-
 * plan-2 T7; bin non installatur: ./bin/aedilis tools/communio.c &&
 * bash build/aedilis/communio/struere.sh -> build/aedilis/communio/
 * communio)
 *
 *   communio tessera -via FILUM
 *       tesseram novam in FILUM (0600, numquam supra exsistens)
 *       cudit et SOLUM digestum imprimit - administrator eum inscribit
 *       (communio_apparatum_addere per canalem); tessera numquam
 *       imprimitur
 *   communio replica -volumen V [-creare]
 *       replicam voluminis imprimit; -creare = volumen novum si deest
 *       (sine eo via falsa errorem dat, numquam volumen tacite creat)
 *   communio currere -volumen V -servus URL -tessera FILUM [-semel]
 *           [-creare]
 *       clientem currit: -semel = sine mora usque ad quietem (exitus 0
 *       si sine errore), aliter semper (tractio dilata), status per
 *       mutationem
 *   communio inscribere -servus URL -email ADDR -tessera FILUM
 *           -titulus NOMEN
 *       apparatum per epistulam inscribit (plan-3 T6): tesseram novam
 *       in FILUM cudit, servus epistulam cum nexu et codice mittit (si
 *       ADDR permissa est); codex hic scribitur AUT nexus aperitur -
 *       utrumque prius sufficit. Post XV minuta exitus I.
 */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "volumen.h"
#include "reactor.h"
#include "communio_cliens.h"

#include <stdio.h>
#include <string.h>
#include <poll.h>

#define GYRI_SEMEL_MAXIMI ((i32)DC * (i32)C)   /* X ms: ~X minuta */

interior s32
_usus (vacuum)
{
    fprintf(stderr, "usus: communio tessera -via FILUM\n"
        "       communio replica -volumen V [-creare]\n"
        "       communio currere -volumen V -servus URL -tessera FILUM"
        " [-semel] [-creare]\n"
        "       communio inscribere -servus URL -email ADDR -tessera"
        " FILUM -titulus NOMEN\n");
    redde II;
}

/* -nomen valor ex argv; NIHIL si abest */
interior constans character*
_optio (
                integer   argc,
              character** argv,
     constans character*  nomen_optionis)
{
    integer k;

    per (k = II; k + I < argc; k++)
    {
        si (strcmp(argv[k], nomen_optionis) == ZEPHYRUM)
        {
            redde argv[k + I];
        }
    }
    redde NIHIL;
}

interior b32
_signum (
                integer   argc,
              character** argv,
     constans character*  nomen_optionis)
{
    integer k;

    per (k = II; k < argc; k++)
    {
        si (strcmp(argv[k], nomen_optionis) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_statum_imprimere (
    CommunioCliens* c)
{
    CommunioStatus s;

    s = communio_cliens_status(c);
    printf("communio: missa %u, recepta %u, ascensus %ld, "
        "descensus %ld%s%.*s\n", (insignatus integer)s.missa,
        (insignatus integer)s.recepta, (longus)s.ascensus,
        (longus)s.descensus, s.error.mensura
            > ZEPHYRUM ? ", error: " : "",
        (integer)s.error.mensura, (constans character*)s.error.datum);
    fflush(stdout);
}

interior s32
_currere (
            Piscina* p,
            Volumen* v,
constans character* servus,
constans character* via_tesserae,
                b32  semel)
{
           Reactor* r;
    CommunioCliens* c;
         character  tessera[XLIV];
    CommunioStatus  prior;
    CommunioStatus  nunc;
               i32  gyri;
               s32  exitus;

    si (!communio_tesseram_legere(via_tesserae, tessera))
    {
        redde I;
    }
    r = reactor_creare(p);
    c = r ? communio_clientem_creare(p, r, v, servus, chorda_ex_literis(
            tessera, p), NIHIL, NIHIL) : NIHIL;
    memset(tessera, ZEPHYRUM, magnitudo(tessera));
    si (!c)
    {
        fprintf(stderr, "communio: cliens creari nequit\n");
        redde I;
    }
    si (semel)
    {
        communio_cliens_moram_ponere(c, ZEPHYRUM);
    }
    communio_cliens_incipere(c);
    memset(&prior, ZEPHYRUM, magnitudo(prior));
    exitus = ZEPHYRUM;
    per (gyri = ZEPHYRUM; !semel || gyri < GYRI_SEMEL_MAXIMI; gyri++)
    {
        (vacuum)reactor_poll(r, X);
        nunc = communio_cliens_status(c);
        si (   nunc.missa         != prior.missa
            || nunc.recepta       != prior.recepta
            || nunc.error.mensura != prior.error.mensura)
        {
            _statum_imprimere(c);
            prior = nunc;
        }
        si (semel && communio_cliens_otiosus(c))
        {
            frange;
        }
        si (semel && nunc.error.mensura > ZEPHYRUM)
        {
            exitus = I;
            frange;
        }
    }
    si (semel && !communio_cliens_otiosus(c) && exitus == ZEPHYRUM)
    {
        fprintf(stderr, "communio: quies non attingitur\n");
        exitus = I;
    }
    _statum_imprimere(c);
    communio_clientem_destruere(c);
    reactor_destruere(r);
    redde exitus;
}

#define INSCRIPTIO_GYRI ((i32)CDL)   /* II s quisque: XV minuta */

/* inscriptio: codex in stdin aut nexus apertus, utrum prius */
interior s32
_inscribere (
            Piscina* p,
constans character* servus,
constans character* epistula,
constans character* via_tesserae,
constans character* titulus)
{
           Reactor* r;
         character  aditus[MONETA_ULID_MENSURA];
         character  apparatus[MONETA_ULID_MENSURA];
         character  linea[LXIV];
     structura pollfd  pf;
               i32 gyri;
               b32 stdin_apertum;
               s32 rc;

    r = reactor_creare(p);
    si (   !r
        || !communio_inscriptionem_petere(r, servus, chorda_ex_literis(
            epistula, p), via_tesserae, chorda_ex_literis(titulus, p),
            aditus, p))
    {
        redde I;
    }
    printf("communio: epistula missa ad %s (si permissa est).\n"
        "Codicem scribe (XXXX-XXXX) et Enter, aut nexum in epistula "
        "aperi.\n", epistula);
    fflush(stdout);
    stdin_apertum = VERUM;
    per (gyri = ZEPHYRUM; gyri < INSCRIPTIO_GYRI; gyri++)
    {
        pf.fd       = ZEPHYRUM;
        pf.events   = POLLIN;
        pf.revents  = ZEPHYRUM;
        si (stdin_apertum && poll(&pf, I, (integer)MM) > ZEPHYRUM)
        {
            si (!fgets(linea, (integer)magnitudo(linea), stdin))
            {
                stdin_apertum = FALSUM;
            }
            alioquin
            {
                linea[strcspn(linea, "\r\n")] = '\0';
                si (linea[ZEPHYRUM])
                {
                    rc = communio_inscriptionem_confirmare(r, servus,
                        chorda_ex_literis(aditus, p), chorda_ex_literis(
                        linea, p), apparatus, p);
                    si (rc == ZEPHYRUM)
                    {
                        printf("communio: apparatus inscriptus: %s\n",
                            apparatus);
                        reactor_destruere(r);
                        redde ZEPHYRUM;
                    }
                    si (rc == -II)
                    {
                        printf("communio: rete deficit - iterum"
                            " tempta\n");
                    }
                    alioquin
                    {
                        printf("communio: codex non valet - iterum"
                            " scribe\n");
                    }
                    fflush(stdout);
                }
            }
        }
        alioquin si (!stdin_apertum)
        {
            (vacuum)reactor_poll(r, (i32)MM);
        }
        si (communio_tessera_valet(r, servus, via_tesserae, p))
        {
            printf("communio: apparatus inscriptus (per nexum)\n");
            reactor_destruere(r);
            redde ZEPHYRUM;
        }
    }
    fprintf(stderr,
        "communio: XV minuta transierunt - aditus expiravit\n");
    reactor_destruere(r);
    redde I;
}

s32
principale (
      integer   argc,
    character** argv)
{
                Piscina* p;
                Volumen* v;
     constans character* via;
     constans character* servus;
     constans character* tessera;
              character  digestum[LXV];
                    s32  exitus;
                 chorda  replica;

    si (argc < II)
    {
        redde _usus();
    }
    si (strcmp(argv[I], "inscribere") == ZEPHYRUM)
    {
        constans character* servus_i   = _optio(argc, argv, "-servus");
        constans character* epistula   = _optio(argc, argv, "-email");
        constans character* tessera_i  = _optio(argc, argv, "-tessera");
        constans character* titulus    = _optio(argc, argv, "-titulus");

        si (!servus_i || !epistula || !tessera_i || !titulus)
        {
            redde _usus();
        }
        p = piscina_generare_dynamicum("communio",
            (memoriae_index)LXIV *
            (memoriae_index)MXXIV);
        exitus = p ? _inscribere(p, servus_i, epistula, tessera_i,
                         titulus) : I;
        si (p)
        {
            piscina_destruere(p);
        }
        redde exitus;
    }
    si (strcmp(argv[I], "tessera") == ZEPHYRUM)
    {
        via = _optio(argc, argv, "-via");
        si (!via)
        {
            redde _usus();
        }
        si (!communio_tesseram_scribere(via, digestum))
        {
            redde I;
        }
        printf("%s\n", digestum);
        redde ZEPHYRUM;
    }
    via = _optio(argc, argv, "-volumen");
    si (!via)
    {
        redde _usus();
    }
    p = piscina_generare_dynamicum("communio", (memoriae_index)LXIV *
        (memoriae_index)MXXIV);
    v = !p ? NIHIL : _signum(argc, argv, "-creare")
                   ? volumen_aperire_aut_creare(p, via)
                   : volumen_aperire(p, via);
    si (!v)
    {
        fprintf(stderr, "communio: volumen aperiri nequit: %s\n", via);
        redde I;
    }
    exitus = ZEPHYRUM;
    si (strcmp(argv[I], "replica") == ZEPHYRUM)
    {
        replica = volumen_replica(v);
        printf("%.*s\n", (integer)replica.mensura,
            (constans character*)replica.datum);
    }
    alioquin si (strcmp(argv[I], "currere") == ZEPHYRUM)
    {
        servus   = _optio(argc, argv, "-servus");
        tessera  = _optio(argc, argv, "-tessera");
        exitus   = (servus && tessera)
                 ? _currere(p, v, servus, tessera, _signum(argc, argv,
                       "-semel"))
                 : _usus();
    }
    alioquin
    {
        exitus = _usus();
    }
    volumen_claudere(v);
    piscina_destruere(p);
    redde exitus;
}
