/* nundinae_apple.c - instrumentum: cliens App Store Connect ex linea
 * mandatorum (testa-ios C2, plan R5). Verba: -status, -applicationes
 * (T1); -parare <app> [-tester <email>] (T2: signum, applicatio,
 * identitas distributionis in claviario, diploma App Store, grex
 * 'Familia', tester). Configuratio: -configuratio <via>, aliter
 * $HOME/.rhubarb/ios/nundinae_apple.stml. Clavis secreta numquam
 * imprimitur. Exitus: 0 | 1 fractum (Apple aut rete) | 2 recusatio
 * (configuratio, clavis, usus) | 3 pactum Apple non acceptum (omnis
 * petitio refutatur donec accipitur). */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "nundinae_apple.h"
#include "base64.h"
#include "filum.h"
#include "processus.h"
#include "json.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

interior s32
_usus (
    vacuum)
{
    fprintf(stderr, "usus: nundinae_apple [-configuratio <via>] "
        "-status | -applicationes | -parare <app> [-tester <email>] | "
        "-quaerere </v1/...>\n");
    redde II;
}

/* causa nuntiata; pactum non acceptum -> monitum clarum et III */
interior s32
_fractum (
     chorda  causa,
    Piscina* p)
{
    fprintf(stderr, "nundinae_apple: %.*s\n", (int)causa.mensura,
        (constans character*)causa.datum);
    si (chorda_continet(causa, chorda_ex_literis(
            NUNDINAE_APPLE_CODEX_PACTUM, p)))
    {
        fprintf(stderr, "nundinae_apple: PACTUM APPLE NON ACCEPTUM - "
            "dominus rationis pacta accipiat: "
            "developer.apple.com/account "
            "(Program License Agreement) et App Store Connect > "
            "Business; deinde iterum\n");
        redde III;
    }
    redde I;
}

/* -status: petitio levissima; "status: bonus" aut causa */
interior s32
_status (
    NundinaeApple* n,
          Piscina* p)
{
    NundinaeAppleResponsum r;

    si (nundinae_apple_statum_probare(n, &r, p))
    {
        imprimere("status: bonus\n");
        redde ZEPHYRUM;
    }
    redde _fractum(r.causa, p);
}

interior s32
_applicationes (
    NundinaeApple* n,
          Piscina* p)
{
     Xar* applicationes;
  chorda  causa;
     i32  i;

    applicationes = xar_creare(p,
        (i32)magnitudo(NundinaeAppleApplicatio));
    si (!nundinae_apple_applicationes(n, applicationes, &causa, p))
    {
        redde _fractum(causa, p);
    }
    per (i = ZEPHYRUM; i < xar_numerus(applicationes); i++)
    {
        NundinaeAppleApplicatio* a;

        a = (NundinaeAppleApplicatio*)xar_obtinere(applicationes, i);
        imprimere("%.*s\t%.*s\t%.*s\n", (int)a->id.mensura,
            (constans character*)a->id.datum,
            (int)a->signum_fasciculi.mensura,
            (constans character*)a->signum_fasciculi.datum,
            (int)a->titulus.mensura,
            (constans character*)a->titulus.datum);
    }
    redde ZEPHYRUM;
}

/* via sub ~/.rhubarb/ios */
interior constans character*
_sedes (
    constans character* nomen_plagulae,
               Piscina* p)
{
    constans character* domus;

    domus = getenv("HOME");
    redde chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(domus
        ? domus : "", p), chorda_concatenare(chorda_ex_literis(
        "/.rhubarb/ios/", p),
        chorda_ex_literis(nomen_plagulae, p), p), p), p);
}

/* 'security' cum argumentis; effusio (stdout) aut vacua si fractum */
interior chorda
_securitas (
    constans character* constans* argumenta,
                          Piscina* p)
{
    ProcessusResultus r;

    r = processus_exsequi(argumenta, LX * M, p);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        fprintf(stderr, "nundinae_apple: security %s fractum "
            "(exitus %d): "
            "%.*s\n", argumenta[I], (int)r.codex_exitus,
            (int)r.erratum.mensura,
            (constans character*)r.erratum.datum);
        redde chorda_ex_literis("", p);
    }
    redde r.effusio.mensura > ZEPHYRUM ? r.effusio
                                       : chorda_ex_literis(" ", p);
}

interior chorda
_identitates (
    Piscina* p)
{
    constans character* argumenta[V];

    argumenta[ZEPHYRUM]  = "/usr/bin/security";
    argumenta[I]         = "find-identity";
    argumenta[II]        = "-v";
    argumenta[III]       = "-p";
    argumenta[IV]        = "codesigning";
    {
        constans character* omnia[VI];

        memcpy(omnia, argumenta, magnitudo(argumenta));
        omnia[V] = NIHIL;
        redde _securitas(omnia, p);
    }
}

/* clavis distributionis (SECRETUM) scripta sola, 0600, numquam
 * superscripta (O_EXCL) */
interior b32
_secretum_scribere (
     constans character* via,
                 chorda  contentum)
{
    integer fd;
    ssize_t n;

    /* <tolera codex="CANALIS_PRAETERITUS" (>clavis distributionis */
    fd = open(via, O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW, 0600);
    si (fd < ZEPHYRUM)
    {
        redde FALSUM;
    }
    n = write(fd, contentum.datum, (size_t)contentum.mensura);
    close(fd);
    redde n == (ssize_t)contentum.mensura;
}

interior vacuum
_certificata_monstrare (
                       Xar* c)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(c); i++)
    {
        NundinaeAppleCertificatum* k;

        k = (NundinaeAppleCertificatum*)xar_obtinere(c, i);
        fprintf(stderr, "  certificatum %.*s  %.*s  exspirat %.*s\n",
            (int)k->id.mensura, (constans character*)k->id.datum,
            (int)k->titulus.mensura,
            (constans character*)k->titulus.datum,
            (int)k->exspirat.mensura,
            (constans character*)k->exspirat.datum);
    }
}

/* identitas distributionis: certificatum turmae cuius SHA-1 in
 * claviario
 * est -> reutitur; aliter clavis nova + petitio + importatio */
interior s32
_identitatem_parare (
                NundinaeApple* n,
    NundinaeAppleCertificatum* exitus,
                      Piscina* p)
{
                        Xar* certificata;
                     chorda  identitates;
                     chorda  causa;
                     chorda  sha;
                     chorda  textus_clavis;
                     chorda  petitio;
     NundinaeAppleClavisRsa  k;
              Base64Fructus  der;
         constans character* via_clavis;
         constans character* via_certificati;
         constans character* catena;
         constans character* argumenta[XI];
                        i32  i;

    certificata = xar_creare(p,
        (i32)magnitudo(NundinaeAppleCertificatum));
    si (!nundinae_apple_certificata_distributionis(n, certificata,
            &causa,
            p))
    {
        redde _fractum(causa, p);
    }
    identitates = _identitates(p);
    per (i = ZEPHYRUM; i < xar_numerus(certificata); i++)
    {
        NundinaeAppleCertificatum* c;

        c    = (NundinaeAppleCertificatum*)xar_obtinere(certificata, i);
        der  = base64_decodificare(c->contentum, p);
        si (!der.datum)
        {
            perge;
        }
        {
            chorda d;

            d.datum    = der.datum;
            d.mensura  = der.mensura;
            sha        = nundinae_apple_digestum_sha1(d, p);
        }
        si (chorda_continet(identitates, sha))
        {
            *exitus = *c;
            imprimere("identitas distributionis: %.*s (%.*s) reperta "
                "in "
                "claviario, SHA-1 %.*s\n", (int)c->titulus.mensura,
                (constans character*)c->titulus.datum,
                (int)c->numerus_serialis.mensura,
                (constans character*)c->numerus_serialis.datum,
                (int)sha.mensura, (constans character*)sha.datum);
            redde ZEPHYRUM;
        }
    }
    via_clavis       = _sedes("distributio.clavis", p);
    via_certificati  = _sedes("distributio.cer", p);
    si (filum_existit(via_clavis))
    {
        fprintf(stderr, "nundinae_apple: %s exstat sed nullum "
            "certificatum "
            "turmae eius in claviario - importa id manu aut tolle "
            "plagulam (numquam superscribitur)\n", via_clavis);
        redde II;
    }
    si (!nundinae_apple_clavem_rsa_generare(&k, p))
    {
        fprintf(stderr, "nundinae_apple: clavis RSA fieri nequit\n");
        redde I;
    }
    textus_clavis      = nundinae_apple_clavem_rsa_textum(&k, p);
    petitio  = nundinae_apple_petitionem_facere(&k, chorda_ex_literis(
        "rhubarb nundinae", p), p);
    nundinae_apple_clavem_rsa_delere(&k);
    si (   textus_clavis.mensura == ZEPHYRUM
        || petitio.mensura       == ZEPHYRUM
        || !_secretum_scribere(via_clavis, textus_clavis))
    {
        memset(textus_clavis.datum, ZEPHYRUM,
            (size_t)textus_clavis.mensura);
        fprintf(stderr, "nundinae_apple: clavis distributionis scribi "
            "nequit (%s)\n", via_clavis);
        redde I;
    }
    memset(textus_clavis.datum, ZEPHYRUM,
        (size_t)textus_clavis.mensura);
    si (!nundinae_apple_certificatum_creare(n, petitio, exitus, &causa,
            p))
    {
        /* clavis nostra inutilis: tollitur (numquam data Apple) */
        (vacuum)filum_delere(via_clavis);
        fprintf(stderr, "nundinae_apple: certificatum distributionis "
            "creari nequit; certificata turmae (nihil revocatur):\n");
        _certificata_monstrare(certificata);
        redde _fractum(causa, p);
    }
    der = base64_decodificare(exitus->contentum, p);
    {
        chorda d;

        d.datum    = der.datum;
        d.mensura  = der.mensura;
        si (!der.datum || !filum_scribere(via_certificati, d))
        {
            fprintf(stderr, "nundinae_apple: certificatum scribi "
                "nequit "
                "(%s)\n", via_certificati);
            redde I;
        }
        sha = nundinae_apple_digestum_sha1(d, p);
    }
    catena = chorda_ut_cstr(chorda_concatenare(chorda_ex_literis(
        getenv("HOME") ? getenv("HOME") : "", p), chorda_ex_literis(
        "/Library/Keychains/login.keychain-db", p), p), p);
    argumenta[ZEPHYRUM]  = "/usr/bin/security";
    argumenta[I]         = "import";
    argumenta[II]        = via_clavis;
    argumenta[III]       = "-k";
    argumenta[IV]        = catena;
    argumenta[V]         = "-t";
    argumenta[VI]        = "priv";
    argumenta[VII]       = "-f";
    argumenta[VIII]      = "openssl";
    argumenta[IX]        = "-T";
    argumenta[X]         = "/usr/bin/codesign";
    {
        constans character* omnia[XII];

        memcpy(omnia, argumenta, magnitudo(argumenta));
        omnia[XI] = NIHIL;
        si (_securitas(omnia, p).mensura == ZEPHYRUM)
        {
            redde I;
        }
    }
    {
        constans character* certificatum_argumenta[VI];

        certificatum_argumenta[ZEPHYRUM]  = "/usr/bin/security";
        certificatum_argumenta[I]         = "import";
        certificatum_argumenta[II]        = via_certificati;
        certificatum_argumenta[III]       = "-k";
        certificatum_argumenta[IV]        = catena;
        certificatum_argumenta[V]         = NIHIL;
        si (_securitas(certificatum_argumenta, p).mensura == ZEPHYRUM)
        {
            redde I;
        }
    }
    si (!chorda_continet(_identitates(p), sha))
    {
        fprintf(stderr, "nundinae_apple: importatum sed identitas "
            "%.*s non "
            "in claviario\n", (int)sha.mensura,
            (constans character*)sha.datum);
        redde I;
    }
    imprimere("identitas distributionis: %.*s (%.*s) CREATA, in "
        "claviario, SHA-1 %.*s; clavis in %s (0600)\n",
        (int)exitus->titulus.mensura,
        (constans character*)exitus->titulus.datum,
        (int)exitus->numerus_serialis.mensura,
        (constans character*)exitus->numerus_serialis.datum,
        (int)sha.mensura, (constans character*)sha.datum, via_clavis);
    redde ZEPHYRUM;
}

/* -quaerere: GET legendi solum, JSON responsi integrum (instrumentum
 * 'control plane'; nulla scriptura) */
interior s32
_quaerere (
          NundinaeApple* n,
     constans character* via,
                Piscina* p)
{
    NundinaeAppleResponsum r;
                    chorda s;

    si (strncmp(via, "/v1/", IV) != ZEPHYRUM && strncmp(via, "/v2/", IV)
        != ZEPHYRUM)
    {
        redde _usus();
    }
    r = nundinae_apple_vocare(n, HTTP_GET, via,
        chorda_ex_literis("", p),
        p);
    si (!r.successus)
    {
        redde _fractum(r.causa, p);
    }
    s = r.corpus ? json_scribere_pulchrum(r.corpus, p)
        : chorda_ex_literis(
        "", p);
    imprimere("%.*s\n", (int)s.mensura, (constans character*)s.datum);
    redde ZEPHYRUM;
}

interior s32
_parare (
          NundinaeApple* n,
     constans character* app,
     constans character* email,
                Piscina* p)
{
                    chorda  signum;
                    chorda  id_signi;
                    chorda  id_app;
                    chorda  id_gregis;
                    chorda  contentum;
                    chorda  causa;
                    chorda  vac;
                       b32  creatum;
                       s32  exitus;
 NundinaeAppleCertificatum  cert;
             Base64Fructus  diploma;
        constans character* via_diplomatis;

    vac     = chorda_ex_literis("", p);
    signum  = chorda_concatenare(chorda_ex_literis("earth.golden.", p),
        chorda_ex_literis(app, p), p);
    si (!nundinae_apple_signum_fasciculi(n, signum, chorda_concatenare(
            chorda_ex_literis("rhubarb ", p), chorda_ex_literis(app, p),
            p),
            &id_signi, &creatum, &causa, p))
    {
        redde _fractum(causa, p);
    }
    imprimere("signum fasciculi: %.*s (%.*s) %s\n", (int)signum.mensura,
        (constans character*)signum.datum, (int)id_signi.mensura,
        (constans character*)id_signi.datum, creatum ? "CREATUM"
                                                     : "repertum");
    si (!nundinae_apple_applicatio(n, signum, &id_app, &causa, p))
    {
        fprintf(stderr, "%.*s\n", (int)causa.mensura,
            (constans character*)causa.datum);
        redde chorda_continet(causa, chorda_ex_literis(
            NUNDINAE_APPLE_CODEX_PACTUM, p)) ? III : II;
    }
    imprimere("applicatio: %.*s (%.*s)\n", (int)signum.mensura,
        (constans character*)signum.datum, (int)id_app.mensura,
        (constans character*)id_app.datum);
    exitus = _identitatem_parare(n, &cert, p);
    si (exitus != ZEPHYRUM)
    {
        redde exitus;
    }
    si (!nundinae_apple_diploma(n, id_signi, cert.id,
            chorda_concatenare(
            chorda_concatenare(chorda_ex_literis("rhubarb ", p),
            chorda_ex_literis(app, p), p), chorda_concatenare(
            chorda_ex_literis(" App Store ", p), cert.numerus_serialis,
            p),
            p), &contentum, &creatum, &causa, p))
    {
        redde _fractum(causa, p);
    }
    diploma = base64_decodificare(contentum, p);
    via_diplomatis = _sedes(chorda_ut_cstr(chorda_concatenare(
        chorda_concatenare(chorda_ex_literis("profila/", p), signum, p),
        chorda_ex_literis(".mobileprovision", p), p), p), p);
    {
        chorda d;

        d.datum    = diploma.datum;
        d.mensura  = diploma.mensura;
        si (   !diploma.datum
            || !filum_directorium_creare_cum_parentibus(_sedes(
                   "profila",
                   p))
            || !filum_scribere(via_diplomatis, d))
        {
            fprintf(stderr,
                "nundinae_apple: diploma scribi nequit (%s)\n",
                via_diplomatis);
            redde I;
        }
    }
    imprimere("diploma App Store: %s %s\n", via_diplomatis, creatum
        ? "CREATUM" : "repertum");
    si (!nundinae_apple_grex(n, id_app, chorda_ex_literis("Familia", p),
            &id_gregis, &creatum, &causa, p))
    {
        redde _fractum(causa, p);
    }
    imprimere("grex internus: Familia (%.*s) %s\n",
        (int)id_gregis.mensura,
        (constans character*)id_gregis.datum, creatum ? "CREATUS"
                                                      : "repertus");
    si (email)
    {
        si (!nundinae_apple_tester(n, id_gregis, chorda_ex_literis(
                email,
                p), &creatum, &causa, p))
        {
            redde _fractum(causa, p);
        }
        imprimere("tester: %s %s\n", email, creatum ? "ADDITUS"
                                                     : "iam in grege");
    }
    (vacuum)vac;
    imprimere("PARATUM: %.*s\n", (int)signum.mensura,
        (constans character*)signum.datum);
    redde ZEPHYRUM;
}

s32
principale (
      integer   argc,
    character** argv)
{
                     Piscina* p;
   NundinaeAppleConfiguratio  cfg;
               NundinaeApple  n;
                      chorda  causa;
                   character  via_cfg[DXII];
          constans character* verbum;
          constans character* positivum;
          constans character* email;
                     integer  i;
                         s32  exitus;

    p = piscina_generare_dynamicum("nundinae_apple", M * M);
    si (!p)
    {
        redde I;
    }
    via_cfg[ZEPHYRUM]  = '\0';
    verbum             = NIHIL;
    positivum          = NIHIL;
    email              = NIHIL;
    per (i = I; i < argc; i++)
    {
        si (   strcmp(argv[i], "-configuratio") == ZEPHYRUM
            && i + I < argc)
        {
            i++;
            si (strlen(argv[i]) >= magnitudo(via_cfg))
            {
                redde _usus();
            }
            strcpy(via_cfg, argv[i]);
        }
        alioquin si (   strcmp(argv[i], "-tester") == ZEPHYRUM
                     && i + I < argc)
        {
            email = argv[++i];
        }
        alioquin si (argv[i][ZEPHYRUM] == '-' && !verbum)
        {
            verbum = argv[i];
        }
        alioquin si (verbum && !positivum && argv[i][ZEPHYRUM] != '-')
        {
            positivum = argv[i];
        }
        alioquin
        {
            redde _usus();
        }
    }
    si (!verbum)
    {
        redde _usus();
    }
    si (via_cfg[ZEPHYRUM] == '\0')
    {
        constans character* domus = getenv("HOME");

        si (!domus || strlen(domus) > CD)
        {
            fprintf(stderr, "nundinae_apple: HOME nullum\n");
            redde II;
        }
        sprintf(via_cfg, "%s/.rhubarb/ios/nundinae_apple.stml", domus);
    }
    si (!nundinae_apple_configurationem_legere(via_cfg, &cfg, &causa, p)
        || !nundinae_apple_aperire(&n, &cfg, &causa, p))
    {
        fprintf(stderr, "%.*s\n", (int)causa.mensura,
            (constans character*)causa.datum);
        redde II;
    }
    si (strcmp(verbum, "-status") == ZEPHYRUM)
    {
        exitus = _status(&n, p);
    }
    alioquin si (strcmp(verbum, "-quaerere") == ZEPHYRUM && positivum)
    {
        exitus = _quaerere(&n, positivum, p);
    }
    alioquin si (strcmp(verbum, "-parare") == ZEPHYRUM && positivum)
    {
        exitus = _parare(&n, positivum, email, p);
    }
    alioquin si (strcmp(verbum, "-applicationes") == ZEPHYRUM)
    {
        exitus = _applicationes(&n, p);
    }
    alioquin
    {
        exitus = _usus();
    }
    nundinae_apple_claudere(&n);
    piscina_destruere(p);
    redde exitus;
}
