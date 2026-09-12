/* fasciculum_instrumentum.c - fasciculos .app struere et relegere
 *
 * Usus:
 *   bin/fasciculum_instrumentum -radix <Foo.app> -identitas <id>
 *       -exsecutabile <via> [-titulus <t>] [-icon <via.icns>]
 *       [-versio <v>] [-aedificatio <v>] [-sine-scandali]
 *   bin/fasciculum_instrumentum -legere <Foo.app>
 *
 * CUR EXSISTIT: fasciculum bibliotheca sine facie est, ergo nihil in
 * arbore fasciculum VERUM struere poterat - et quaestio una quam nulla
 * probatio respondere potest (an fasciculus duplici ictu aperiatur)
 * fasciculo VERO eget. tools/fasciculum_fumus.sh hoc utitur.
 *
 * Exitus: 0 factum; 1 RECUSATUM (status et sedes nominantur);
 *   2 NIHIL FACTUM (argumenta desunt aut ignota) - disciplina domus:
 *   instrumentum quod nihil fecit successus non est.
 */
#include "postulata_posix.h"

#include "latina.h"
#include "fasciculum.h"
#include "filum.h"
#include "piscina.h"
#include "internamentum.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>

interior constans character*
_titulus_status (
    FasciculumStatus status)
{
    commutatio (status)
    {
        casus FASCICULUM_SUCCESSUS:
            redde "SUCCESSUS";
        casus FASCICULUM_ERROR_DESUNT:
            redde "DESUNT";
        casus FASCICULUM_ERROR_EXSECUTABILE:
            redde "EXSECUTABILE";
        casus FASCICULUM_ERROR_ICON:
            redde "ICON";
        casus FASCICULUM_ERROR_PLISTA:
            redde "PLISTA";
        casus FASCICULUM_ERROR_NON_FASCICULUM:
            redde "NON_FASCICULUM";
        casus FASCICULUM_ERROR_DIRECTORIUM:
            redde "DIRECTORIUM";
        casus FASCICULUM_ERROR_SCRIPTIO:
            redde "SCRIPTIO";
        casus FASCICULUM_ERROR_MODUS:
            redde "MODUS";
        casus FASCICULUM_ERROR_MEMORIA:
            redde "MEMORIA";
        ordinarius:
            redde "IGNOTUS";
    }
}

interior vacuum
_usus (vacuum)
{
    imprimere("usus:\n");
    imprimere("  fasciculum_instrumentum -radix <Foo.app>"
              " -identitas <id> -exsecutabile <via>\n");
    imprimere("      [-titulus <t>] [-icon <via>] [-versio <v>]"
              " [-aedificatio <v>] [-sine-scandali]\n");
    imprimere("  fasciculum_instrumentum -legere <Foo.app>\n");
}

s32
principale (
       integer   numerus,
     character** argumenta)
{
                Piscina* piscina;
    InternamentumChorda* intern;
             Fasciculum  f;
                    Xar* partes;
       FasciculumStatus  status;
                 chorda  sedes;
     constans character* via_radicis    = NIHIL;
     constans character* identitas      = NIHIL;
     constans character* exsecutabile   = NIHIL;
     constans character* titulus        = NIHIL;
     constans character* icon           = NIHIL;
     constans character* versio         = NIHIL;
     constans character* aedificatio    = NIHIL;
     constans character* via_legenda    = NIHIL;
                    b32  sine_scandali  = FALSUM;
                integer  i;
                    s32  fructus        = II;

    per (i = I; i < numerus; i++)
    {
        constans character* a = argumenta[i];

        si (strcmp(a, "-sine-scandali") == ZEPHYRUM)
        {
            sine_scandali = VERUM;
        }
        alioquin si (i + I >= numerus)
        {
            imprimere("argumentum sine valore: %s\n", a);
            _usus();
            redde II;
        }
        alioquin si (strcmp(a, "-legere") == ZEPHYRUM)
        {
            via_legenda = argumenta[++i];
        }
        alioquin si (strcmp(a, "-radix") == ZEPHYRUM)
        {
            via_radicis = argumenta[++i];
        }
        alioquin si (strcmp(a, "-identitas") == ZEPHYRUM)
        {
            identitas = argumenta[++i];
        }
        alioquin si (strcmp(a, "-exsecutabile") == ZEPHYRUM)
        {
            exsecutabile = argumenta[++i];
        }
        alioquin si (strcmp(a, "-titulus") == ZEPHYRUM)
        {
            titulus = argumenta[++i];
        }
        alioquin si (strcmp(a, "-icon") == ZEPHYRUM)
        {
            icon = argumenta[++i];
        }
        alioquin si (strcmp(a, "-versio") == ZEPHYRUM)
        {
            versio = argumenta[++i];
        }
        alioquin si (strcmp(a, "-aedificatio") == ZEPHYRUM)
        {
            aedificatio = argumenta[++i];
        }
        alioquin
        {
            imprimere("argumentum ignotum: %s\n", a);
            _usus();
            redde II;
        }
    }

    si (!via_legenda && (!via_radicis || !identitas || !exsecutabile))
    {
        _usus();
        redde II;
    }

    piscina = piscina_generare_dynamicum("fasciculum_instrumentum",
                                         1048576);
    si (!piscina)
    {
        imprimere("piscina creari non potuit\n");
        redde II;
    }
    intern = internamentum_creare(piscina);
    si (!intern)
    {
        imprimere("internamentum creari non potuit\n");
        piscina_destruere(piscina);
        redde II;
    }

    si (via_legenda)
    {
        si (!fasciculum_legere(via_legenda, &f, &status, &sedes,
                               piscina, intern))
        {
            imprimere("RECUSATUM %s: %s\n", _titulus_status(status),
                      chorda_ut_cstr(sedes, piscina));
            fructus = I;
        }
        alioquin
        {
            imprimere("identitas: %s\n",
                      chorda_ut_cstr(f.identitas, piscina));
            imprimere("titulus: %s\n",
                      chorda_ut_cstr(f.titulus, piscina));
            imprimere("versio: %s\n",
                      chorda_ut_cstr(f.versio, piscina));
            imprimere("aedificatio: %s\n",
                      chorda_ut_cstr(f.versio_aedificationis,
                                     piscina));
            imprimere("exsecutabile: %s\n",
                      f.exsecutabile ? f.exsecutabile : "(nullum)");
            imprimere("icon: %s\n", f.icon ? f.icon : "(nullus)");
            imprimere("sine_scandali: %s\n",
                      f.sine_scandali ? "VERUM" : "FALSUM");
            imprimere("plista_extra: %d paria\n",
                      (integer)plist_numerus(f.plista_extra));
            fructus = ZEPHYRUM;
        }
    }
    alioquin
    {
        memset(&f, ZEPHYRUM, magnitudo(Fasciculum));
        f.identitas = chorda_ex_literis(identitas, piscina);
        si (titulus)
        {
            f.titulus = chorda_ex_literis(titulus, piscina);
        }
        si (versio)
        {
            f.versio = chorda_ex_literis(versio, piscina);
        }
        si (aedificatio)
        {
            f.versio_aedificationis = chorda_ex_literis(aedificatio,
                                                        piscina);
        }
        f.exsecutabile   = exsecutabile;
        f.icon           = icon;
        f.sine_scandali  = sine_scandali;

        si (!fasciculum_reddere(&f, &partes, &status, &sedes, piscina))
        {
            imprimere("RECUSATUM %s: %s\n", _titulus_status(status),
                      chorda_ut_cstr(sedes, piscina));
            fructus = I;
        }
        alioquin si (!fasciculum_scribere(partes, via_radicis, &status,
                                          &sedes, piscina))
        {
            imprimere("RECUSATUM %s: %s\n", _titulus_status(status),
                      chorda_ut_cstr(sedes, piscina));
            fructus = I;
        }
        alioquin
        {
            imprimere("%s\n", via_radicis);
            fructus = ZEPHYRUM;
        }
    }

    piscina_destruere(piscina);
    redde fructus;
}
