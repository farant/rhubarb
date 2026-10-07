/* compilator.c - 'clang -c' per thesaurum contentorum (fabrica plan 2
 * T4, spec 2 par. III.2). Argumenta eadem ac 'clang -c ... -o <obj>
 * <fons>': runner verbum unum mutat.
 *
 * CLAVIS CAPITIS = sigillum(cwd ‖ identitas clang ‖ argumenta sine
 * valore -o ‖ via et octeti fontis ‖ indices radicum inclusionis). Sub
 * ea: index capitum depfile cursus ultimi (blobus). CLAVIS PLENA =
 * sigillum(caput ‖ per caput: via ‖ sigillum octetorum). Sub ea:
 * obiectum (blobus).
 *   - cwd in clave (A5): -g directorium operis infigit (spec XII.2).
 *   - radices -I (et directorium fontis): nomina .h ordinata - caput
 *     novum eiusdem nominis in radice priore OBUMBRAT (depfile id non
 *     videt; Review Focus 2).
 *   - identitas clang: /usr/bin/clang trampolinum est (CXIX KiB); verum
 *     binarium (clang -print-prog-name=clang, CCLVII MiB) semel
 *     sigillatur et per (via, mensura, mtime, inode) memoratur.
 *     FABRICA_CLANG (involucra probationum): octeti eius ipsi.
 * Hit: obiectum COPIATUR (temporarium + rename), numquam nexu duro -
 * inodus communis blobum corrumperet si quis destinationem in loco
 * rescriberet. Destinatio identica non tangitur. Miss: clang -MD -MF
 * temporarium; exitus et stderr transeunt; fractum nihil condit.
 * Argumenta non cacheabilia (sine -c/-o, fontes plures, -E/-S/-M*):
 * clang ipse fit (processus_transformare). */
/* <aedilis obiectum="build/fabrica/provenientia/compilator.c"/> */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "xar.h"
#include "sigillum.h"
#include "thesaurus.h"
#include "processus.h"
#include "provenientia.h"
#include "lectiones.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#define THESAURUS_ORDINARIUS "build/aedilis/obiecta"
#define VIA_MAXIMA (IV * MXXIV)

externus constans ProvenientiaRelatio provenientia_compilator;

/* optiones quarum valor argumentum sequens est */
hic_manens constans character* constans _cum_valore[] = {
    "-o", "-I", "-include", "-isystem", "-iquote", "-MF", "-MT", "-MQ",
    "-arch", "-x", "-framework", "-iframework", "-F", "-L", NIHIL
};

nomen structura {
               Piscina*  piscina;
             Thesaurus*  thesaurus;
    constans character*  clang;
             character** argumenta;   /* argv[1..] */
               integer   numerus;
               integer   index_exitus; /* index valoris -o */
    constans character*  exitus;
    constans character*  fons;
    /* radices inclusionis ordine clavis (-I, deinde directorium fontis):
     * obumbratio per NOMEN in clave plena (fabrica plan 5 T2) */
                   Xar* radices;
} Compilatio;

interior b32
_est_cum_valore (
    constans character* argumentum)
{
    integer k;

    per (k = ZEPHYRUM; _cum_valore[k] != NIHIL; k++)
    {
        si (strcmp(argumentum, _cum_valore[k]) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_terminatur (
    constans character* textus,
    constans character* finis)
{
    memoriae_index a;
    memoriae_index b;

    a = strlen(textus);
    b = strlen(finis);
    redde a > b && strcmp(textus + a - b, finis) == ZEPHYRUM;
}

/* cacheabilisne? -c et -o adsunt, fons unus .c aut .m, nulla
 * praeprocessio sola (-E, -S, -M*) */
interior b32
_argumenta_parare (
    Compilatio* c)
{
    integer i;
        b32 compilare;
    integer fontes;

    compilare        = FALSUM;
    fontes           = ZEPHYRUM;
    c->exitus        = NIHIL;
    c->fons          = NIHIL;
    c->index_exitus  = -I;
    per (i = ZEPHYRUM; i < c->numerus; i++)
    {
        constans character* a;

        a = c->argumenta[i];
        si (strcmp(a, "-c") == ZEPHYRUM)
        {
            compilare = VERUM;
        }
        alioquin si (   strcmp(a, "-E")      == ZEPHYRUM
                     || strcmp(a, "-S")      == ZEPHYRUM
                     || strncmp(a, "-M", II) == ZEPHYRUM)
        {
            redde FALSUM;
        }
        alioquin si (_est_cum_valore(a))
        {
            si (i + I >= c->numerus)
            {
                redde FALSUM;
            }
            si (strcmp(a, "-o") == ZEPHYRUM)
            {
                c->index_exitus  = i + I;
                c->exitus        = c->argumenta[i + I];
            }
            i++;
        }
        alioquin si (a[0] != '-')
        {
            fontes++;
            c->fons = a;
        }
    }
    redde compilare && c->exitus != NIHIL && fontes == I
        && (_terminatur(c->fons, ".c") || _terminatur(c->fons, ".m"));
}

/* octeti plagulae sigillati; FALSUM si absens */
interior b32
_plagulam_sigillare (
    constans character* via,
               Piscina* piscina,
              Sigillum* out)
{
    chorda contentum;

    si (!filum_existit(via))
    {
        redde FALSUM;
    }
    contentum = filum_legere_totum(via, piscina);
    *out = sigillum_computare(contentum.datum,
        (memoriae_index)contentum.mensura);
    redde VERUM;
}

/* IDENTITAS COMPILATORIS: FABRICA_CLANG -> octeti involucri; aliter
 * binarium verum per memoriam (via, mensura, mtime, inode) in
 * thesauro */
interior b32
_identitas_compilatoris (
    Compilatio* c,
      Sigillum* out)
{
    constans character* argv[III];
     ProcessusResultus  resultus;
             character  via[VIA_MAXIMA];
             character  memoria[VIA_MAXIMA + CXXVIII];
      structura stat    status;
              Sigillum  clavis;
                   Xar* capta;
                   Xar* ponenda;
        memoriae_index  longitudo;

    si (getenv("FABRICA_CLANG") != NIHIL)
    {
        redde _plagulam_sigillare(c->clang, c->piscina, out);
    }
    argv[0]   = c->clang;
    argv[I]   = "-print-prog-name=clang";
    argv[II]  = NIHIL;
    resultus  = processus_exsequi(argv, XXX * M, c->piscina);
    si (   !resultus.successus || resultus.codex_exitus != ZEPHYRUM
        || resultus.effusio.mensura == ZEPHYRUM
        || resultus.effusio.mensura >= VIA_MAXIMA)
    {
        redde FALSUM;
    }
    longitudo = (memoriae_index)resultus.effusio.mensura;
    memcpy(via, resultus.effusio.datum, longitudo);
    dum (   longitudo > ZEPHYRUM
         && (via[longitudo - I] == '\n' || via[longitudo - I] == '\r'))
    {
        longitudo--;
    }
    via[longitudo] = '\0';
    si (stat(via, &status) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    sprintf(memoria, "compilator identitas I\n%s\n%ld %ld %lu", via,
        (longus)status.st_size, (longus)status.st_mtime,
        (longus insignatus)status.st_ino);
    clavis = sigillum_computare(memoria, strlen(memoria));
    si (   thesaurus_actio_capere(c->thesaurus, &clavis, c->piscina,
            &capta)
        && xar_numerus(capta) == I)
    {
        *out = *(Sigillum*)xar_obtinere(capta, ZEPHYRUM);
        thesaurus_generationem_notare(c->thesaurus, &clavis);
        redde VERUM;
    }
    si (!_plagulam_sigillare(via, c->piscina, out))
    {
        redde FALSUM;
    }
    ponenda = xar_creare(c->piscina, (i32)magnitudo(Sigillum));
    *(Sigillum*)xar_addere(ponenda) = *out;
    (vacuum)thesaurus_actio_ponere(c->thesaurus, &clavis, ponenda);
    thesaurus_generationem_notare(c->thesaurus, &clavis);
    redde VERUM;
}

/* radix una: via sola in clave capitis (ordo radicum mutat) - nomina
 * capitum NON (fabrica plan 5 T2): enumeratio radicis tota omnem
 * verdictum caput quodvis novum faciebat irritum. Obumbratio per nomen
 * capitis usi in clave plena probatur (_obumbrationem_addere). */
interior vacuum
_radicem_addere (
           Compilatio* c,
    SigillumContextus* contextus,
   constans character* radix)
{
    sigillum_addere(contextus, radix, strlen(radix) + I);
    si (c->radices != NIHIL)
    {
        *(constans character**)xar_addere(c->radices) = radix;
    }
}

/* CLAVIS CAPITIS */
interior b32
_clavem_capitis (
    Compilatio* c,
      Sigillum* identitas,
      Sigillum* out)
{
    SigillumContextus  contextus;
             Sigillum  fons;
            character  cwd[VIA_MAXIMA];
            character  directorium[VIA_MAXIMA];
            character* ultimum;
              integer  i;

    si (   getcwd(cwd, magnitudo(cwd)) == NIHIL
        || !_plagulam_sigillare(c->fons, c->piscina, &fons))
    {
        redde FALSUM;
    }
    sigillum_incipere(&contextus);
    sigillum_addere(&contextus, "compilator II\n", XIV);
    sigillum_addere(&contextus, cwd, strlen(cwd) + I);
    sigillum_addere(&contextus, identitas->octeti, XXXII);
    per (i = ZEPHYRUM; i < c->numerus; i++)
    {
        si (i == c->index_exitus)
        {
            perge;
        }
        sigillum_addere(&contextus, c->argumenta[i],
            strlen(c->argumenta[i]) + I);
    }
    sigillum_addere(&contextus, "\001fons\n", VI);
    sigillum_addere(&contextus, c->fons, strlen(c->fons) + I);
    sigillum_addere(&contextus, fons.octeti, XXXII);
    /* radices: -I (ordine) deinde directorium fontis (quotae primum) */
    per (i = ZEPHYRUM; i < c->numerus; i++)
    {
        constans character* a;

        a = c->argumenta[i];
        si (strcmp(a, "-I") == ZEPHYRUM && i + I < c->numerus)
        {
            _radicem_addere(c, &contextus, c->argumenta[i + I]);
            i++;
        }
        alioquin si (strncmp(a, "-I", II) == ZEPHYRUM && a[II] != '\0')
        {
            _radicem_addere(c, &contextus, a + II);
        }
    }
    strcpy(directorium, c->fons);
    ultimum = strrchr(directorium, '/');
    si (ultimum != NIHIL)
    {
        *ultimum = '\0';
    }
    alioquin
    {
        strcpy(directorium, ".");
    }
    _radicem_addere(c, &contextus, chorda_ut_cstr(chorda_ex_literis(
        directorium, c->piscina), c->piscina));
    *out = sigillum_finire(&contextus);
    redde VERUM;
}

/* via sine './' initiali */
interior constans character*
_sine_puncto (
    constans character* via)
{
    dum (via[ZEPHYRUM] == '.' && via[I] == '/')
    {
        via += II;
    }
    redde via;
}

/* OBUMBRATIO PER NOMEN (fabrica plan 5 T2): caput 'via' in radice R
 * inventum - idem nomen relativum in OMNI radice altera quaeritur
 * (filum_existit: liber A/X notat; clavis bit unum). Radix posterior
 * quoque: caput includens in ea habitans directorium suum primum
 * quaerit. Via absoluta (systema) et via extra radices: nihil. */
interior vacuum
_obumbrationem_addere (
           Compilatio* c,
    SigillumContextus* contextus,
   constans character* via_capitis)
{
    constans character* via       = _sine_puncto(via_capitis);
    constans character* relativa  = NIHIL;
                   s32  inventa   = -I;
                   i32  k;

    si (c->radices == NIHIL || via[ZEPHYRUM] == '/')
    {
        redde;
    }
    per (k = ZEPHYRUM; inventa < ZEPHYRUM
        && k < xar_numerus(c->radices); k++)
    {
        constans character* r = _sine_puncto(*(constans character**)
            xar_obtinere(c->radices, k));
           memoriae_index n = strlen(r);

        si (n == ZEPHYRUM || strcmp(r, ".") == ZEPHYRUM)
        {
            inventa   = (s32)k;
            relativa  = via;
        }
        alioquin si (strncmp(via, r, n) == ZEPHYRUM && via[n] == '/')
        {
            inventa   = (s32)k;
            relativa  = via + n + I;
        }
    }
    si (inventa < ZEPHYRUM)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(c->radices); k++)
    {
        constans character* r = _sine_puncto(*(constans character**)
            xar_obtinere(c->radices, k));
                character candidata[VIA_MAXIMA];
                      b32 exstat;

        si ((s32)k == inventa)
        {
            perge;
        }
        si (strlen(r) + strlen(relativa) + II
            >= (memoriae_index)VIA_MAXIMA)
        {
            perge;
        }
        si (r[ZEPHYRUM] == '\0' || strcmp(r, ".") == ZEPHYRUM)
        {
            strcpy(candidata, relativa);
        }
        alioquin
        {
            sprintf(candidata, "%s/%s", r, relativa);
        }
        exstat = filum_existit(candidata);
        sigillum_addere(contextus, exstat ? "1" : "0", I);
    }
}

/* CLAVIS PLENA: caput ‖ per caput indicis: via ‖ sigillum ‖
 * obumbratio per nomen (T2). FALSUM si caput absens (miss). */
interior b32
_clavem_plenam (
     Compilatio* c,
       Sigillum* caput,
         chorda  index,
       Sigillum* out)
{
    SigillumContextus contextus;
                  i32 i;
                  i32 initium;

    sigillum_incipere(&contextus);
    sigillum_addere(&contextus, caput->octeti, XXXII);
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i < index.mensura; i++)
    {
           chorda via;
         Sigillum sigillum;

        si (index.datum[i] != '\n')
        {
            perge;
        }
        via      = chorda_sectio(index, initium, i);
        initium  = i + I;
        si (via.mensura == ZEPHYRUM)
        {
            perge;
        }
        si (!_plagulam_sigillare(chorda_ut_cstr(via, c->piscina),
                c->piscina, &sigillum))
        {
            redde FALSUM;
        }
        sigillum_addere(&contextus, via.datum,
            (memoriae_index)via.mensura);
        sigillum_addere(&contextus, "\n", I);
        sigillum_addere(&contextus, sigillum.octeti, XXXII);
        _obumbrationem_addere(c, &contextus,
            chorda_ut_cstr(via, c->piscina));
    }
    *out = sigillum_finire(&contextus);
    redde VERUM;
}

/* destinationem implere ex octetis: identica non tangitur;
 * aliter temporarium + rename. Identica: lectio comparationis in
 * libro L fit (filum), sed destinatio EXITUS est, non ingressus -
 * 'S' eam cursui huic possidendam notat (spec 3 par. XI corr. 1: sine
 * eo omnis cursus calidus 'ingressum build/ sine domino' haberet) */
interior b32
_collocare (
    Compilatio* c,
        chorda  octeti)
{
    Sigillum novum;
    Sigillum praesens;
   character temporarium[VIA_MAXIMA];

    novum = sigillum_computare(octeti.datum,
        (memoriae_index)octeti.mensura);
    si (   _plagulam_sigillare(c->exitus, c->piscina, &praesens)
        && sigillum_aequale(&novum, &praesens))
    {
        lectiones_notare(LECTIO_SCRIPSIT, c->exitus);
        redde VERUM;
    }
    sprintf(temporarium, "%s.compilator.%ld", c->exitus,
        (longus)getpid());
    si (!filum_scribere(temporarium, octeti))
    {
        (vacuum)filum_delere(temporarium);
        redde FALSUM;
    }
    si (!filum_movere(temporarium, c->exitus))
    {
        (vacuum)filum_delere(temporarium);
        redde FALSUM;
    }
    redde VERUM;
}

/* depfile ('scopus: a b \\\n c'): praerequisita post primum (fons) */
interior chorda
_index_dependentiarum (
      chorda  dependentiae,
     Piscina* piscina)
{
         i8* buffer;
        i32  i;
        i32  scriptum;
        i32  numerus_partium;
        b32  in_token;
        b32  post_colon;

    buffer = (i8*)piscina_allocare(piscina,
        (memoriae_index)dependentiae.mensura + I);
    scriptum         = ZEPHYRUM;
    numerus_partium  = ZEPHYRUM;
    in_token         = FALSUM;
    post_colon       = FALSUM;
    per (i = ZEPHYRUM; i < dependentiae.mensura; i++)
    {
        character ch;

        ch = (character)dependentiae.datum[i];
        si (!post_colon)
        {
            si (   ch == ':' && i + I < dependentiae.mensura
                && (dependentiae.datum[i + I] == ' '
                    || dependentiae.datum[i + I] == '\n'))
            {
                post_colon = VERUM;
            }
            perge;
        }
        si (ch == '\\' && i + I < dependentiae.mensura)
        {
            si (dependentiae.datum[i + I] == '\n')
            {
                i++;
                ch = ' ';
            }
            alioquin si (dependentiae.datum[i + I] == ' ')
            {
                i++;
                si (numerus_partium >= I)
                {
                    buffer[scriptum++] = ' ';
                }
                in_token = VERUM;
                perge;
            }
        }
        si (ch == ' ' || ch == '\n' || ch == '\t' || ch == '\r')
        {
            si (in_token)
            {
                numerus_partium++;
                si (numerus_partium > I)
                {
                    buffer[scriptum++] = '\n';
                }
                in_token = FALSUM;
            }
            perge;
        }
        /* token primum (fons) non scribitur */
        si (numerus_partium >= I)
        {
            buffer[scriptum++] = (i8)ch;
        }
        in_token = VERUM;
    }
    si (in_token)
    {
        numerus_partium++;
        si (numerus_partium > I)
        {
            buffer[scriptum++] = '\n';
        }
    }
    {
        chorda index;

        index.datum    = buffer;
        index.mensura  = scriptum;
        redde index;
    }
}

/* MISS: clang cum -o temporario et -MD -MF; deinde condere et
 * collocare. Reddit exitum clang. */
interior s32
_compilare (
    Compilatio* c,
      Sigillum* caput)
{
     constans character** argv;
      ProcessusResultus   resultus;
              character   obiectum[VIA_MAXIMA];
              character   via_dependentiarum[VIA_MAXIMA];
                integer   i;
                integer   n;
                 chorda   octeti;
                 chorda   index;
               Sigillum   plena;
               Sigillum   blobus_obiecti;
               Sigillum   blobus_indicis;
                    Xar*  exitus;

    sprintf(obiectum, "%s.compilator.%ld.o", c->exitus,
        (longus)getpid());
    sprintf(via_dependentiarum, "%s.compilator.%ld.d", c->exitus,
        (longus)getpid());
    argv = (constans character**)piscina_allocare(c->piscina,
        (memoriae_index)(c->numerus + VI) * magnitudo(character*));
    n          = ZEPHYRUM;
    argv[n++]  = c->clang;
    per (i = ZEPHYRUM; i < c->numerus; i++)
    {
        argv[n++] = (i == c->index_exitus) ? obiectum : c->argumenta[i];
    }
    argv[n++]  = "-MD";
    argv[n++]  = "-MF";
    argv[n++]  = via_dependentiarum;
    argv[n]    = NIHIL;
    resultus   = processus_exsequi(argv, ZEPHYRUM, c->piscina);
    (vacuum)fwrite(resultus.effusio.datum, I,
        (memoriae_index)resultus.effusio.mensura, stdout);
    (vacuum)fwrite(resultus.erratum.datum, I,
        (memoriae_index)resultus.erratum.mensura, stderr);
    si (!resultus.successus)
    {
        fprintf(stderr, "compilator: clang currere nequit: %s (%s)\n",
            c->clang, processus_error_nomen(resultus.error));
        (vacuum)filum_delere(obiectum);
        (vacuum)filum_delere(via_dependentiarum);
        redde I;
    }
    si (   resultus.codex_exitus != ZEPHYRUM
        || resultus.signum       != ZEPHYRUM)
    {
        (vacuum)filum_delere(obiectum);
        (vacuum)filum_delere(via_dependentiarum);
        redde resultus.codex_exitus != ZEPHYRUM
            ? (s32)resultus.codex_exitus : I;
    }
    octeti = filum_legere_totum(obiectum, c->piscina);
    index  =
        _index_dependentiarum(filum_legere_totum(via_dependentiarum,
        c->piscina),
        c->piscina);
    (vacuum)filum_delere(via_dependentiarum);
    /* condere: index sub capite, obiectum sub clave plena */
    exitus = xar_creare(c->piscina, (i32)magnitudo(Sigillum));
    si (   octeti.mensura > ZEPHYRUM && exitus != NIHIL
        && _clavem_plenam(c, caput, index, &plena)
        && thesaurus_ponere(c->thesaurus, index, &blobus_indicis)
        && thesaurus_ponere(c->thesaurus, octeti, &blobus_obiecti))
    {
        *(Sigillum*)xar_addere(exitus) = blobus_indicis;
        (vacuum)thesaurus_actio_ponere(c->thesaurus, caput, exitus);
        exitus = xar_creare(c->piscina, (i32)magnitudo(Sigillum));
        *(Sigillum*)xar_addere(exitus) = blobus_obiecti;
        (vacuum)thesaurus_actio_ponere(c->thesaurus, &plena, exitus);
        thesaurus_generationem_notare(c->thesaurus, caput);
        thesaurus_generationem_notare(c->thesaurus, &plena);
    }
    si (!_collocare(c, octeti))
    {
        fprintf(stderr, "compilator: %s scribi nequit\n", c->exitus);
        (vacuum)filum_delere(obiectum);
        redde I;
    }
    (vacuum)filum_delere(obiectum);
    redde ZEPHYRUM;
}

/* HIT: caput -> index -> clavis plena -> obiectum. FALSUM = miss. */
interior b32
_invenire (
    Compilatio* c,
      Sigillum* caput)
{
       Xar* capta;
    chorda  via_indicis;
    chorda  via_obiecti;
    chorda  index;
  Sigillum  plena;

    si (   !thesaurus_actio_capere(c->thesaurus, caput, c->piscina,
            &capta)
        || xar_numerus(capta) != I
        || !thesaurus_via(c->thesaurus,
            (Sigillum*)xar_obtinere(capta, ZEPHYRUM), c->piscina,
            &via_indicis))
    {
        redde FALSUM;
    }
    index = filum_legere_totum(chorda_ut_cstr(via_indicis, c->piscina),
        c->piscina);
    si (   !_clavem_plenam(c, caput, index, &plena)
        || !thesaurus_actio_capere(c->thesaurus, &plena, c->piscina,
            &capta)
        || xar_numerus(capta) != I
        || !thesaurus_via(c->thesaurus,
            (Sigillum*)xar_obtinere(capta, ZEPHYRUM), c->piscina,
            &via_obiecti))
    {
        redde FALSUM;
    }
    si (!_collocare(c, filum_legere_totum(
            chorda_ut_cstr(via_obiecti, c->piscina), c->piscina)))
    {
        redde FALSUM;
    }
    thesaurus_generationem_notare(c->thesaurus, caput);
    thesaurus_generationem_notare(c->thesaurus, &plena);
    redde VERUM;
}

s32
principale (
      integer   argc,
    character** argv)
{
               Piscina* piscina;
            Compilatio  c;
              Sigillum  identitas;
              Sigillum  caput;
    constans character* radix;
    constans character* transformanda[CCLVI];
               integer  i;

    si (provenientia_respondere(argc, argv, &provenientia_compilator))
    {
        redde ZEPHYRUM;
    }
    c.clang = getenv("FABRICA_CLANG");
    si (c.clang == NIHIL || c.clang[0] == '\0')
    {
        c.clang = "clang";
    }
    c.argumenta  = argv + I;
    c.numerus    = argc - I;
    piscina      = piscina_generare_dynamicum("compilator", 16777216);
    radix        = getenv("FABRICA_THESAURUS");
    si (radix == NIHIL || radix[0] == '\0')
    {
        radix = THESAURUS_ORDINARIUS;
    }
    c.piscina = piscina;
    c.radices = xar_creare(piscina,
        (i32)magnitudo(constans character*));
    c.thesaurus = (piscina != NIHIL)
        ? thesaurus_aperire(radix, piscina) : NIHIL;
    si (   c.thesaurus == NIHIL || !_argumenta_parare(&c)
        || !_identitas_compilatoris(&c, &identitas)
        || !_clavem_capitis(&c, &identitas, &caput))
    {
        /* non cacheabile: clang ipse fit */
        si (argc >= CCLV)
        {
            fprintf(stderr, "compilator: argumenta nimis multa\n");
            redde I;
        }
        transformanda[0] = c.clang;
        per (i = I; i < argc; i++)
        {
            transformanda[i] = argv[i];
        }
        transformanda[argc] = NIHIL;
        (vacuum)processus_transformare(transformanda);
        fprintf(stderr, "compilator: %s exsequi nequit\n", c.clang);
        redde I;
    }
    si (_invenire(&c, &caput))
    {
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }
    {
        s32 exitus;

        exitus = _compilare(&c, &caput);
        piscina_destruere(piscina);
        redde exitus;
    }
}
