/* manus_instrumentum.c - bin/manus: manus ex crusta
 *
 * NOMEN PLAGULAE: binarium 'manus' est; plagula 'manus_instrumentum'
 * quia lib/manus.c iam manus.o parit et acervus obiectorum planus
 * est - duo manus.c collisionem tacitam facerent.
 *
 * PROBLEMA MENSURATUM (hac ipsa sessione, 2026-08-13): applicationem
 * vivam agitare volui et hoc scripsi -
 *
 *   ./bin/mensor_ui -vivum -portus 8792 > log 2>&1 &
 *   PID=$!; sleep 4
 *   T=$(curl -s -X POST --data "$JS" .../imperium)
 *   N=$(echo "$T" | tr -dc '0-9')
 *   sleep 1
 *   curl -s .../imperium/$N
 *   kill $PID
 *
 * Omnis linea vitium fert: duo somni DIVINATI, tessera per litteras
 * eruta, portus manu electus, fenestra amissa si quid in medio
 * cadit. Et pessimum: iussum ANTE paginam paratam missum valorem
 * PLAUSIBILEM sed falsum reddidit ('valor 0' - verum pro selectore
 * absente, indistinguibile a canali mortuo).
 *
 * Manus omnia illa iam recte agit - sed ex C sola vocari poterat.
 * Ergo bibliotheca non deerat; SUPERFICIES deerat.
 *
 * SESSIO EST PORTUS. Nullum indicium opacum inventum est: portus
 * IAM unicus est et in omni nuntio apparet. Indicium quod portum
 * celaret rem addere sine luce esset.
 *
 * USUS:
 *   bin/manus incipere ./bin/mensor_ui      -> sessionem gignit
 *   bin/manus affordantiae                  -> sessio SOLA sumitur
 *   bin/manus -s 8801 premere-textum Salve
 *   bin/manus imago /tmp/x.png
 *   bin/manus finire
 *
 * SESSIO OMITTI POTEST cum una sola vivit. Cum PLURES vivunt,
 * RECUSAMUS et eas nominamus - applicationem falsam agitare
 * silentio est id ipsum quod haec domus semper clamosum facit.
 *
 * EXITUS: 0 factum; I defectum (manus fracta, causa nominata);
 *         II NIHIL ACTUM (argumenta prava, sessio nulla aut ambigua).
 *
 * PERICULUM - MODUS EVOLUTIONIS SOLUS: per imperium loquitur, quod
 * JS arbitrarium exsequitur. Applicatio sub -vivum currat, numquam
 * vecta.
 */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "manus.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define VIAE_MENSURA   DXII
#define SESSIONES_MAX  XXXII

nomen structura {
    i32       portus;
    s32       pid;        /* ZEPHYRUM = adhaesa, non a nobis genita */
    character binarium[CCLVI];
} Sessio;

/* ==================================================
 * Sedes sessionum
 * ================================================== */

interior vacuum
_sedes (character* destinatio, memoriae_index maximum)
{
    constans character* domus = getenv("HOME");

    (vacuum)maximum;
    sprintf(destinatio, "%s/.rhubarb/manus",
            (domus != NIHIL) ? domus : "/tmp");
}

interior vacuum
_via_sessionis (character* destinatio, i32 portus)
{
    character sedes[VIAE_MENSURA];

    _sedes(sedes, magnitudo(sedes));
    sprintf(destinatio, "%s/%d.sessio", sedes, (integer)portus);
}

interior vacuum
_via_diarii (character* destinatio, i32 portus)
{
    character sedes[VIAE_MENSURA];

    _sedes(sedes, magnitudo(sedes));
    sprintf(destinatio, "%s/%d.effusio", sedes, (integer)portus);
}

/* Sedem PARARE ante quamlibet scriptionem.
 *
 * MENSURATUM: primum hoc solum in _sessionem_scribere vocabam, quae
 * POST detractionem currit - ergo proles diarium aperire non poterat
 * (sedes nondum erat), effusio in nihilum ibat, et cum applicatio
 * defecisset instrumentum me ad plagulam ABSENTEM misit. Defectus
 * qui suam ipsius diagnosim delevit. */
interior vacuum
_sedem_parare (vacuum)
{
    character domus[VIAE_MENSURA];
    character sedes[VIAE_MENSURA];
    constans character* h = getenv("HOME");

    sprintf(domus, "%s/.rhubarb", (h != NIHIL) ? h : "/tmp");
    (vacuum)mkdir(domus, 0755);   /* iam esse potest */
    _sedes(sedes, magnitudo(sedes));
    (vacuum)mkdir(sedes, 0755);
}

interior b32
_vivit (s32 pid)
{
    si (pid <= ZEPHYRUM)
    {
        redde VERUM;   /* adhaesa: portus solus testis est */
    }
    redde (kill((pid_t)pid, 0) == ZEPHYRUM) ? VERUM : FALSUM;
}

interior b32
_sessionem_scribere (constans Sessio* s)
{
    character via[VIAE_MENSURA];
    FILE*     f;

    _sedem_parare();
    _via_sessionis(via, s->portus);
    f = fopen(via, "w");
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    fprintf(f, "%d\t%d\t%s\n", (integer)s->portus, (integer)s->pid,
            s->binarium);
    fclose(f);
    redde VERUM;
}

interior b32
_sessionem_legere (i32 portus, Sessio* exitus)
{
    character via[VIAE_MENSURA];
    FILE*     f;
    integer   p;
    integer   q;

    _via_sessionis(via, portus);
    f = fopen(via, "r");
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    exitus->binarium[0] = '\0';
    si (fscanf(f, "%d\t%d\t%255[^\n]", &p, &q, exitus->binarium) < II)
    {
        fclose(f);
        redde FALSUM;
    }
    fclose(f);
    exitus->portus = (i32)p;
    exitus->pid    = (s32)q;
    redde VERUM;
}

/* Sessiones VIVAS colligere; mortuas eodem transitu purgare, ne
 * index crescat donec ambiguitas falsa fiat. */
interior i32
_sessiones_legere (Sessio* vas, i32 maximum)
{
    character      sedes[VIAE_MENSURA];
    DIR*           d;
    structura dirent* v;
    i32            n = ZEPHYRUM;

    _sedes(sedes, magnitudo(sedes));
    d = opendir(sedes);
    si (d == NIHIL)
    {
        redde ZEPHYRUM;
    }

    dum ((v = readdir(d)) != NIHIL && n < maximum)
    {
        Sessio s;
        i32    portus;

        si (strstr(v->d_name, ".sessio") == NIHIL)
        {
            perge;
        }
        portus = (i32)atoi(v->d_name);
        si (portus == ZEPHYRUM || !_sessionem_legere(portus, &s))
        {
            perge;
        }
        si (!_vivit(s.pid))
        {
            character via[VIAE_MENSURA];

            _via_sessionis(via, portus);
            (vacuum)remove(via);
            perge;
        }
        vas[n] = s;
        n++;
    }
    (vacuum)closedir(d);
    redde n;
}

/* ==================================================
 * Portus liber
 * ==================================================
 *
 * Liga ad ZEPHYRUM, lege quid nucleus dedit, solve. Inter solutionem
 * et usum applicationis fenestra certaminis MINIMA manet - eam
 * nominamus quia tacere de ea peius esset. In practica: applicatio
 * quae portum occupatum invenit CLAMAT (imperium_vivarium causam
 * reddit), ergo defectus apertus est, non tacitus.
 */
interior i32
_portus_liber (vacuum)
{
    structura sockaddr_in ad;
    socklen_t             longitudo = (socklen_t)magnitudo(ad);
    s32                   fd;
    i32                   portus = ZEPHYRUM;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    si (fd < ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    memset(&ad, 0, magnitudo(ad));
    ad.sin_family      = AF_INET;
    ad.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    ad.sin_port        = 0;

    si (bind(fd, (structura sockaddr*)&ad, (socklen_t)magnitudo(ad))
        == ZEPHYRUM
        && getsockname(fd, (structura sockaddr*)&ad, &longitudo)
        == ZEPHYRUM)
    {
        portus = (i32)ntohs(ad.sin_port);
    }
    (vacuum)close(fd);
    redde portus;
}

/* ==================================================
 * Applicationem detrahere
 * ==================================================
 *
 * CUR NON manus_incipere: illa effusionem per FISTULAS capit, et
 * fistula sine lectore applicationem obstruit aut necat cum
 * instrumentum exit. Sessio autem instrumentum SUPERVIVERE debet.
 * Ergo hic: setsid + effusio in plagulam. Diarium praeterea manet,
 * unde 'bin/manus effusio' postea legere potest.
 */
interior s32
_detrahere (constans character* binarium, i32 portus,
            integer argc_extra, character** argv_extra)
{
    pid_t      proles;
    character  vex_vivum[XVI];
    character  vex_portus[XVI];
    character  num[XXXII];
    character  diarium[VIAE_MENSURA];
    character* argumenta[XXXII];
    integer    i;
    integer    n = 0;

    strcpy(vex_vivum,  "-vivum");
    strcpy(vex_portus, "-portus");
    sprintf(num, "%d", (integer)portus);
    _via_diarii(diarium, portus);

    /* Cast necessarius: execv 'character* constans[]' poscit dum
     * binarium nostrum constans sit. Nihil scribimus. */
    argumenta[n++] = (character*)(memoriae_index)binarium;
    argumenta[n++] = vex_vivum;
    argumenta[n++] = vex_portus;
    argumenta[n++] = num;
    per (i = 0; i < argc_extra && n < (integer)(XXXII - I); i++)
    {
        argumenta[n++] = argv_extra[i];
    }
    argumenta[n] = NIHIL;

    proles = fork();
    si (proles < 0)
    {
        redde -I;
    }
    si (proles == 0)
    {
        s32 fd;

        (vacuum)setsid();
        fd = open(diarium, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        si (fd >= 0)
        {
            (vacuum)dup2(fd, I);
            (vacuum)dup2(fd, II);
            (vacuum)close(fd);
        }
        (vacuum)execv(binarium, argumenta);

        /* HUC PERVENIRE = execv defecit. Effusio iam ad diarium
         * ducta est, ergo causam ILLIC scribimus: aliter proles
         * tacite moritur, diarium ZEPHYRUM octetorum manet, et
         * instrumentum usorem ad plagulam vacuam mittit. Defectus
         * qui rationem suam delet bis constat. */
        fprintf(stderr, "manus: execv defecit: %s (errno %d): %s\n",
                strerror(errno), (integer)errno, binarium);
        fflush(stderr);
        _exit(CXXVII);
    }
    redde (s32)proles;
}

/* Portum exspectare postquam applicationem detraximus.
 *
 * CUR NON manus_incipere: illa applicationem IPSA gignit, quod iam
 * fecimus. Prima forma huius plagulae ei NIHIL pro argumentis
 * tradebat ut 'portum solum exspectaret' - abusus quem commentario
 * excusavi. Contractus eius vectorem NIHIL-terminatum poscit;
 * quidquid ex NIHIL sequitur eius culpa non est.
 *
 * Somnus hic LICITUS est et in manu vetitus: manus somnum vetat pro
 * condicionibus PAGINAE (ubi exspectatio intus fieri debet, ne
 * transeuntia elabantur). Hic autem processum ORIRI exspectamus -
 * res extra paginam, quam pagina nuntiare non potest. manus_incipere
 * ipsa idem facit. */
interior Manus*
_portum_exspectare (Piscina* piscina, i32 portus)
{
    structura timespec mora;
    Manus*             m;
    integer            conatus;

    mora.tv_sec  = 0;
    mora.tv_nsec = 50L * 1000L * 1000L;   /* L ms */

    per (conatus = 0; conatus < C; conatus++)   /* <= V s */
    {
        m = manus_aperire(piscina, "127.0.0.1", portus);
        si (m != NIHIL)
        {
            redde m;
        }
        (vacuum)nanosleep(&mora, NIHIL);
    }
    redde NIHIL;
}

/* ==================================================
 * Paratitudo paginae
 * ==================================================
 *
 * Portus respondens NON significat paginam esse. Iussum ante
 * paginam missum in documentum vacuum currit et valorem
 * PLAUSIBILEM reddit - defectus quem hoc instrumentum ipsum passum
 * est ('valor 0').
 *
 * Exspectatio IN PAGINA fit, more manus: promissum quod se solvit
 * cum 'load' venit aut terminus transit. Nullus somnus hinc.
 */
interior b32
_paginam_exspectare (Manus* m)
{
    chorda r = manus_aestimare(m,
        "new Promise(function(R){"
        "if(document.readyState==='complete')return R(1);"
        "window.addEventListener('load',function(){R(1);});"
        "setTimeout(function(){R(0);},2500);})",
        MANUS_MORA_LONGA);

    redde (r.mensura > ZEPHYRUM && r.datum[0] == '1') ? VERUM : FALSUM;
}

/* ==================================================
 * Sessionem eligere
 * ================================================== */

interior b32
_eligere (i32 rogatus, Sessio* exitus)
{
    Sessio vas[SESSIONES_MAX];
    i32    n;
    i32    i;

    si (rogatus != ZEPHYRUM)
    {
        si (!_sessionem_legere(rogatus, exitus))
        {
            fprintf(stderr, "manus: sessio %d ignota"
                    " (vide 'bin/manus sessiones')\n", (integer)rogatus);
            redde FALSUM;
        }
        redde VERUM;
    }

    n = _sessiones_legere(vas, (i32)SESSIONES_MAX);
    si (n == ZEPHYRUM)
    {
        fprintf(stderr, "manus: nulla sessio viva"
                " (curre 'bin/manus incipere <binarium>')\n");
        redde FALSUM;
    }
    si (n > I)
    {
        /* RECUSATIO, NON CONIECTURA: applicationem falsam agitare
         * silentio peius est quam nihil agere. */
        fprintf(stderr, "manus: sessiones PLURES vivunt -"
                " elige per -s <portus>:\n");
        per (i = ZEPHYRUM; i < n; i++)
        {
            fprintf(stderr, "  %d\t%s\n", (integer)vas[i].portus,
                    vas[i].binarium);
        }
        redde FALSUM;
    }
    *exitus = vas[0];
    redde VERUM;
}

/* ==================================================
 * Impressio affordantiarum
 * ================================================== */

interior constans character*
_genus_nomen (AffordantiaGenus g)
{
    commutatio (g)
    {
    casus AFFORDANTIA_PREMENDA:    redde "premenda";
    casus AFFORDANTIA_SCRIBENDA:   redde "scribenda";
    casus AFFORDANTIA_ELIGENDA:    redde "eligenda";
    casus AFFORDANTIA_COMMUTANDA:  redde "commutanda";
    ordinarius:                    redde "ignota";
    }
}

interior vacuum
_affordantias_imprimere (Affordantiae aff, b32 machina)
{
    i32 i;

    si (machina)
    {
        /* TSV PURUM: nullum caput, nullus ornatus. */
        per (i = ZEPHYRUM; i < aff.numerus; i++)
        {
            Affordantia* d = &aff.res[i];

            imprimere("%s\t%.*s\t%.*s\t%.*s\t%.*s\t%d\t%d\t%d\t%d\n",
                _genus_nomen(d->genus),
                (integer)d->selector.mensura,
                (constans character*)d->selector.datum,
                (integer)d->titulus.mensura,
                (constans character*)d->titulus.datum,
                (integer)d->valor.mensura,
                (constans character*)d->valor.datum,
                (integer)d->impedimentum.mensura,
                (constans character*)d->impedimentum.datum,
                (integer)d->x, (integer)d->y,
                (integer)d->latitudo, (integer)d->altitudo);
        }
        redde;
    }

    imprimere("affordantiae: %d\n", (integer)aff.numerus);
    per (i = ZEPHYRUM; i < aff.numerus; i++)
    {
        Affordantia* d = &aff.res[i];

        imprimere("%2d  %-10s  %-28.*s  %.*s",
            (integer)i, _genus_nomen(d->genus),
            (integer)d->selector.mensura,
            (constans character*)d->selector.datum,
            (integer)d->titulus.mensura,
            (constans character*)d->titulus.datum);

        si (d->valor.mensura > ZEPHYRUM)
        {
            imprimere("  = %.*s", (integer)d->valor.mensura,
                      (constans character*)d->valor.datum);
        }
        si (d->impedimentum.mensura > ZEPHYRUM)
        {
            imprimere("   [%.*s]", (integer)d->impedimentum.mensura,
                      (constans character*)d->impedimentum.datum);
        }
        imprimere("\n");
    }
}

/* ==================================================
 * Auxilium
 * ================================================== */

interior s32
_auxilium (vacuum)
{
    imprimere(
      "manus - applicationem vitreae vivam agitare (modus dev)\n\n"
      "  bin/manus incipere <binarium> [args...]  sessionem gignere\n"
      "  bin/manus adhaerere <portus>             sessioni currenti\n"
      "  bin/manus sessiones                      quae vivunt\n"
      "  bin/manus finire                         applicationem occidere\n\n"
      "  bin/manus affordantiae                   quid tangi possit\n"
      "  bin/manus lege <selector>                quid DICITUR\n"
      "  bin/manus abest <selector>               MORATUR donec abeat\n"
      "  bin/manus imago <via>                    photographia\n"
      "  bin/manus premere <selector>\n"
      "  bin/manus premere-textum <textus>\n"
      "  bin/manus scribere <selector> <textus>\n"
      "  bin/manus volvere <selector|+-pixela>   imago PROSPECTUM capit\n"
      "  bin/manus textus <selector>\n"
      "  bin/manus aestimare <js>\n"
      "  bin/manus errores                        culpae paginae\n"
      "  bin/manus effusio                        stdout applicationis\n\n"
      "  -s <portus>   sessionem nominare (omitte cum una sola vivit)\n"
      "  -exspecta     lege/textus: MORARI donec selector appareat\n"
      "                (actiones IAM morantur; solae interrogationes\n"
                       " non morabantur)\n"
      "  -mora <ms>    terminus exspectandi (ordinarius MM)\n"
      "  -machina      TSV purum, sine capite (affordantiae,\n"
      "                sessiones, lege)\n\n"
      /* COLUMNAE HIC NOMINANDAE.
       *
       * -machina caput ferre NON debet (mos domus: TSV purum, ne
       * awk lineam saltare cogatur), sed columnae innominatae
       * coniecturam poscunt: agens probans quattuor numeros ultimos
       * ut x/y/lat/alt RECTE divinavit - quod peius est quam prave
       * divinare, nam coniectura falsa statim appareret.
       * Praeterea campi VACUI in TSV tabulationes CONTINUAS pariunt,
       * quas oculus numerare non potest. Ergo index hic, ubi ille
       * iam quaesiverat. */
      "Columnae -machina:\n"
      "  affordantiae  genus selector titulus valor impedimentum\n"
      "                x y latitudo altitudo   (IX columnae; valor et\n"
      "                impedimentum vacua esse possunt - tabulationes\n"
      "                continuas exspecta)\n"
      "  sessiones     portus pid applicatio   (pid 0 = adhaesa)\n"
      "  lege          cellulae ordinis (filii visibiles; sine\n"
      "                filiis, textus ipsius) - numerus columnarum\n"
      "                per ordinem VARIAT\n\n"
      "Exitus: 0 factum; I defectum; II nihil actum.\n");
    redde II;
}

/* ==================================================
 * Principale
 * ================================================== */

s32 principale (integer argc, character** argv)
{
    Piscina*            piscina;
    Manus*              m;
    Sessio              sessio;
    constans character* verbum;
    character**         reliqua;
    integer             n_reliqua;
    integer             i;
    i32                 rogatus = ZEPHYRUM;
    b32                 machina = FALSUM;
    b32                 exspecta = FALSUM;
    Mora                mora = MANUS_MORA_ORDINARIA;
    s32                 codex   = ZEPHYRUM;

    si (argc < II)
    {
        redde _auxilium();
    }

    /* Vexilla ubicumque, non in loco primo solo: '-s' post verbum
     * scribere naturale est, et vexillum quod solum primo loco
     * legitur tacite elabitur (decipula mensurata in mensor_ui). */
    verbum    = NIHIL;
    reliqua   = NIHIL;
    n_reliqua = 0;
    per (i = I; i < argc; i++)
    {
        si (strcmp(argv[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[i], "-exspecta") == ZEPHYRUM)
        {
            exspecta = VERUM;
        }
        alioquin si (strcmp(argv[i], "-mora") == ZEPHYRUM && (i + I) < argc)
        {
            mora = (Mora)atoi(argv[i + I]);
            i++;
        }
        alioquin si (strcmp(argv[i], "-s") == ZEPHYRUM && (i + I) < argc)
        {
            rogatus = (i32)atoi(argv[i + I]);
            i++;
        }
        alioquin si (verbum == NIHIL)
        {
            verbum    = argv[i];
            reliqua   = &argv[i + I];
            n_reliqua = argc - i - I;
        }
    }

    si (verbum == NIHIL)
    {
        redde _auxilium();
    }

    piscina = piscina_generare_dynamicum("manus", M * M * IV);

    /* ---- incipere: sessionem gignere ---- */
    si (strcmp(verbum, "incipere") == ZEPHYRUM)
    {
        Sessio nova;
        i32    portus;
        s32    pid;

        si (n_reliqua < I)
        {
            fprintf(stderr, "manus: 'incipere' binarium poscit\n");
            redde II;
        }
        /* ANTE detractionem: proles diarium ibi aperit. */
        _sedem_parare();
        portus = _portus_liber();
        si (portus == ZEPHYRUM)
        {
            fprintf(stderr, "manus: portus liber inveniri non potuit\n");
            redde I;
        }
        pid = _detrahere(reliqua[0], portus, n_reliqua - I,
                         &reliqua[I]);
        si (pid < ZEPHYRUM)
        {
            fprintf(stderr, "manus: applicatio detrahi non potuit: %s\n",
                    reliqua[0]);
            redde I;
        }

        m = _portum_exspectare(piscina, portus);
        si (m == NIHIL)
        {
            character diarium[VIAE_MENSURA];

            _via_diarii(diarium, portus);
            fprintf(stderr,
                "manus: applicatio portum %d non aperuit"
                " (-vivum praebet? vide %s)\n",
                (integer)portus, diarium);
            (vacuum)kill((pid_t)pid, SIGTERM);
            redde I;
        }

        (vacuum)_paginam_exspectare(m);

        nova.portus = portus;
        nova.pid    = pid;
        strncpy(nova.binarium, reliqua[0], magnitudo(nova.binarium) - I);
        nova.binarium[magnitudo(nova.binarium) - I] = '\0';
        si (!_sessionem_scribere(&nova))
        {
            fprintf(stderr, "manus: sessio scribi non potuit\n");
            redde I;
        }
        imprimere("%d\n", (integer)portus);
        redde ZEPHYRUM;
    }

    /* ---- adhaerere: applicationi iam currenti ---- */
    si (strcmp(verbum, "adhaerere") == ZEPHYRUM)
    {
        Sessio nova;
        i32    portus;

        si (n_reliqua < I)
        {
            fprintf(stderr, "manus: 'adhaerere' portum poscit\n");
            redde II;
        }
        portus = (i32)atoi(reliqua[0]);
        m = manus_aperire(piscina, "127.0.0.1", portus);
        si (m == NIHIL)
        {
            fprintf(stderr, "manus: nihil respondet in %d\n",
                    (integer)portus);
            redde I;
        }
        nova.portus = portus;
        nova.pid    = ZEPHYRUM;   /* non nostra: portus solus testis */
        strcpy(nova.binarium, "(adhaesa)");
        (vacuum)_sessionem_scribere(&nova);
        imprimere("%d\n", (integer)portus);
        redde ZEPHYRUM;
    }

    /* ---- sessiones: index ---- */
    si (strcmp(verbum, "sessiones") == ZEPHYRUM)
    {
        Sessio vas[SESSIONES_MAX];
        i32    n = _sessiones_legere(vas, (i32)SESSIONES_MAX);
        i32    k;

        /* CAPUT in forma humana, ABSENS in -machina.
         *
         * Prius tres numeri nudi imprimebantur ('61059 24091
         * ./bin/mensor_ui') et agens probans eos cum tabula
         * 'SESSIONES' ipsius applicationis confudit - quae acta
         * probationum significat, non processus.
         *
         * VERBUM ipsum culpa NON est: 'sessio' utrumque recte
         * nominat, et quaevis applicatio idem verbum habere potest.
         * Quod deerat CAMPUS erat - unde 'portus' et 'applicatio'
         * quaestionem sine renominatione solvunt. */
        si (!machina && n > ZEPHYRUM)
        {
            imprimere("portus  pid     applicatio\n");
        }
        per (k = ZEPHYRUM; k < n; k++)
        {
            si (machina)
            {
                imprimere("%d\t%d\t%s\n", (integer)vas[k].portus,
                          (integer)vas[k].pid, vas[k].binarium);
            }
            alioquin
            {
                imprimere("%-7d %-7d %s\n", (integer)vas[k].portus,
                          (integer)vas[k].pid, vas[k].binarium);
            }
        }
        si (n == ZEPHYRUM)
        {
            /* In stderr: effusio -machina munda manet etiam vacua. */
            fprintf(stderr, "manus: nulla sessio viva\n");
        }
        redde (n > ZEPHYRUM) ? ZEPHYRUM : II;
    }

    /* ---- cetera sessionem poscunt ---- */
    si (!_eligere(rogatus, &sessio))
    {
        redde II;
    }

    si (strcmp(verbum, "finire") == ZEPHYRUM)
    {
        character via[VIAE_MENSURA];

        si (sessio.pid > ZEPHYRUM)
        {
            (vacuum)kill((pid_t)sessio.pid, SIGTERM);
        }
        _via_sessionis(via, sessio.portus);
        (vacuum)remove(via);
        imprimere("sessio %d finita\n", (integer)sessio.portus);
        redde ZEPHYRUM;
    }

    si (strcmp(verbum, "effusio") == ZEPHYRUM)
    {
        character diarium[VIAE_MENSURA];
        FILE*     f;
        character linea[VIAE_MENSURA];

        _via_diarii(diarium, sessio.portus);
        f = fopen(diarium, "r");
        si (f == NIHIL)
        {
            fprintf(stderr, "manus: nullum diarium (sessio adhaesa?)\n");
            redde II;
        }
        dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
        {
            fputs(linea, stdout);
        }
        fclose(f);
        redde ZEPHYRUM;
    }

    m = manus_aperire(piscina, "127.0.0.1", sessio.portus);
    si (m == NIHIL)
    {
        fprintf(stderr, "manus: sessio %d non respondet"
                " (applicatio obiit? 'bin/manus sessiones')\n",
                (integer)sessio.portus);
        redde I;
    }

    si (strcmp(verbum, "affordantiae") == ZEPHYRUM)
    {
        _affordantias_imprimere(manus_affordantiae(m, piscina), machina);
    }
    alioquin si (strcmp(verbum, "imago") == ZEPHYRUM && n_reliqua >= I)
    {
        si (manus_imaginem(m, reliqua[0]))
        {
            imprimere("%s\n", reliqua[0]);
        }
    }
    alioquin si (strcmp(verbum, "premere") == ZEPHYRUM && n_reliqua >= I)
    {
        (vacuum)manus_premere(m, reliqua[0]);
    }
    alioquin si (strcmp(verbum, "premere-textum") == ZEPHYRUM
                 && n_reliqua >= I)
    {
        (vacuum)manus_premere_textum(m, reliqua[0]);
    }
    alioquin si (strcmp(verbum, "scribere") == ZEPHYRUM && n_reliqua >= II)
    {
        (vacuum)manus_scribere(m, reliqua[0], reliqua[I]);
    }
    alioquin si (strcmp(verbum, "abest") == ZEPHYRUM && n_reliqua >= I)
    {
        /* ABSENTIA verbum PROPRIUM habet: '-exspecta' 'retempta donec
         * non vacuum' significat, quod abitum exprimere non potest -
         * rota volvens, dialogus clausus, ordo deletus. */
        (vacuum)manus_exspectare(m, reliqua[0], FALSUM, mora);
    }
    alioquin si (strcmp(verbum, "lege") == ZEPHYRUM && n_reliqua >= I)
    {
        Lectio l;
        i32    k;

        si (exspecta)
        {
            (vacuum)manus_exspectare(m, reliqua[0], VERUM, mora);
        }
        l = manus_legere(m, reliqua[0], piscina);

        per (k = ZEPHYRUM; k < l.numerus; k++)
        {
            i32 c;

            per (c = ZEPHYRUM; c < l.lineae[k].numerus; c++)
            {
                si (c > ZEPHYRUM)
                {
                    /* TAB in -machina, virgula in forma humana:
                     * eadem lex qua affordantiae reguntur. */
                    imprimere(machina ? "\t" : " | ");
                }
                imprimere("%.*s",
                    (integer)l.lineae[k].cellulae[c].mensura,
                    (constans character*)l.lineae[k].cellulae[c].datum);
            }
            imprimere("\n");
        }
    }
    alioquin si (strcmp(verbum, "volvere") == ZEPHYRUM && n_reliqua >= I)
    {
        /* Numerus an selector? Prima littera decidit, et discrimen
         * TUTUM est: identificator CSS cifra aut signo incipere NON
         * potest (constrictio linguae, non coniectura nostra). */
        constans character* arg = reliqua[0];

        si (arg[0] == '-' || arg[0] == '+'
            || (arg[0] >= '0' && arg[0] <= '9'))
        {
            (vacuum)manus_volvere(m, (s32)atoi(arg));
        }
        alioquin
        {
            (vacuum)manus_volvere_ad(m, arg);
        }
    }
    alioquin si (strcmp(verbum, "textus") == ZEPHYRUM && n_reliqua >= I)
    {
        chorda t;

        si (exspecta)
        {
            (vacuum)manus_exspectare(m, reliqua[0], VERUM, mora);
        }
        t = manus_textus(m, reliqua[0]);

        imprimere("%.*s\n", (integer)t.mensura,
                  (constans character*)t.datum);
    }
    alioquin si (strcmp(verbum, "aestimare") == ZEPHYRUM && n_reliqua >= I)
    {
        chorda r = manus_aestimare(m, reliqua[0], MANUS_MORA_ORDINARIA);

        imprimere("%.*s\n", (integer)r.mensura,
                  (constans character*)r.datum);
    }
    alioquin si (strcmp(verbum, "errores") == ZEPHYRUM)
    {
        chorda primus;
        i32    quot;

        primus.mensura = ZEPHYRUM;
        primus.datum   = NIHIL;
        quot = manus_errores(m, &primus);

        imprimere("errores: %d\n", (integer)quot);
        si (primus.mensura > ZEPHYRUM)
        {
            /* PRIMUS solus: ceteri fere sequelae eius sunt. */
            imprimere("primus: %.*s\n", (integer)primus.mensura,
                      (constans character*)primus.datum);
        }
    }
    alioquin
    {
        fprintf(stderr, "manus: verbum ignotum '%s'\n", verbum);
        redde _auxilium();
    }

    /* MANUS FRACTA: causam NOMINAT. Sine hoc instrumentum taceret et
     * exitum ZEPHYRUM daret - viride et mendax. */
    si (manus_fracta(m))
    {
        chorda causa = manus_causa(m);

        fprintf(stderr, "manus FRACTA: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
        codex = I;
    }

    /* NON manus_claudere: si manus applicationem genuisset eam
     * occideret, et sessio instrumentum supervivere DEBET. */
    redde codex;
}
