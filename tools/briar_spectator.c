/* briar_spectator.c - Spectator: fenestra super paginam faciei
 * (spec par. 4.7; '-visio' briaris hoc exsequitur).
 *
 * Est briar cum fenestra. REDDITIONEM NON ITERAT: eandem
 * briar_faciem_fingere vocat, eandem vestem e capsula legit, ergo
 * pagina quam ostendit et pagina quam '-html' scribit eaedem sunt
 * PER CONSTRUCTIONEM.
 *
 * Pagina in fenestram per 'capsula_radix' atrii venit: asseta e
 * DIRECTORIO leguntur, origo 'capsula://' manet, pagina discernere
 * non potest. Et quia pagina SUI SUFFICIENS est (styli et scriptum
 * intus), directorium plagulam UNAM tenet.
 *
 * Aedificatio: ./tools/briar_spectator_struere.sh
 */

#include "postulata_posix.h"
#include "latina.h"
#include "atrium.h"
#include "briar_arbor.h"
#include "briar_contextus.h"
#include "briar_fabrica.h"
#include "briar_facies.h"
#include "briar_imperium.h"
#include "briar_nexus.h"
#include "briar_silva.h"
#include "capsula.h"
#include "chorda.h"
#include "filum.h"
#include "internamentum.h"
#include "piscina.h"
#include "silex.h"
#include "via.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* symbola capsularum sunt contractus; capita genita numquam
 * includuntur (silva inclusionem citatam cum '..' solvere nequit -
 * regula speculi, nota 01KY0T6T64) */
externus constans CapsulaEmbed capsula_corpus_silicis;
externus constans CapsulaEmbed capsula_facies_briar;

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

interior constans character*
_iungere (
               Piscina* piscina,
    constans character* a,
    constans character* b)
{
    redde chorda_ut_cstr(chorda_concatenare(
        chorda_ex_literis(a, piscina),
        chorda_ex_literis(b, piscina), piscina), piscina);
}

/* thistle -> pagina scripta; reddit DIRECTORIUM paginae (vacuum si
 * fractum), et titulum plagulae per 'titulus' */
interior constans character*
_paginam_parare (
                 Piscina*  piscina,
     InternamentumChorda*  intern,
      constans SilexFons*  fons,
      constans character*  via_thistle,
      constans character** titulus,
                  chorda*  causa)
{
     BriarFabricaFructus  fructus;
    BriarFabricaOptiones  optiones;
             BriarVestis  vestis;
               character* textus;
                     i32  mensura = ZEPHYRUM;
           MateriaNodus* doc;
                    Xar* nexus;
                    Xar* fragmenta = NIHIL;
                  chorda octeti;
                  chorda pagina;
                character clavis[17];
      constans character* domus;
      constans character* dir;

    causa->datum    = NIHIL;
    causa->mensura  = ZEPHYRUM;
    *titulus        = NIHIL;
    textus          = _plagulam_legere(piscina, via_thistle, &mensura);
    si (textus == NIHIL)
    {
        *causa = chorda_ex_literis("plagula non lecta", piscina);
        redde NIHIL;
    }
    doc    = briar_arbor_parsare(piscina, textus, mensura);
    nexus  = briar_nexus_texere(piscina, doc, intern);
    (vacuum)briar_contexere(piscina, nexus, &fragmenta);
    (vacuum)briar_silvam_texere(piscina, nexus, fons);
    briar_optiones_plagulae(piscina, fons, via_thistle, &optiones);
    octeti.datum    = (i8*)textus;
    octeti.mensura  = mensura;
    fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones,
        octeti);
    /* recusatio paginam NON prohibet (lex par. 4.6 F4): causa in
     * margine stabit */
    si (!briar_vestem_legere(piscina, &capsula_facies_briar, &vestis,
        causa))
    {
        redde NIHIL;
    }
    /* via OPTIONUM, non argv crudum: ea est quae in capite paginae
     * stat et quam '-html' quoque scribit */
    pagina = briar_faciem_fingere(piscina, intern, nexus, fragmenta,
        &fructus, octeti, optiones.via_thistle, &vestis, causa);
    si (pagina.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    briar_fabrica_clavem_computare(fons->titulus,
        briar_fabrica_vexilla(fructus.forma), octeti, clavis);
    domus = briar_domus_proiecti(piscina, fructus.titulus, clavis);
    si (domus == NIHIL)
    {
        *causa = chorda_ex_literis("domus proiecti abest (HOME?)",
            piscina);
        redde NIHIL;
    }
    dir = _iungere(piscina, domus, "/facies");
    si (!briar_directoria_creare(piscina, dir))
    {
        *causa = chorda_ex_literis("directorium faciei non creatum",
            piscina);
        redde NIHIL;
    }
    si (!filum_scribere(_iungere(piscina, dir,
        _iungere(piscina, "/", _iungere(piscina, fructus.titulus,
        ".html"))), pagina))
    {
        *causa = chorda_ex_literis("pagina non scripta", piscina);
        redde NIHIL;
    }
    *titulus = fructus.titulus;
    redde dir;
}

s32
principale (
      integer   argc,
    character** argv)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
      constans SilexFons* fons;
      AtriumConfiguratio  figura;
                  Atrium* atrium;
                  chorda  causa;
                  chorda  causa_atrii;
      constans character* via_thistle   = NIHIL;
                     b32  solum_parare  = FALSUM;
      constans character* dir;
      constans character* titulus;
                 integer  k;

    /* plagula ANTE atrium_vexilla_legere legenda: illud argv suis
     * vexillis consumit ('-radix', '-portus'...) */
    per (k = I; k < argc; k++)
    {
        si (argv[k][0] != '-')
        {
            via_thistle = argv[k];
            frange;
        }
    }
    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-parare") == ZEPHYRUM)
        {
            solum_parare = VERUM;
        }
    }
    si (via_thistle == NIHIL)
    {
        fprintf(stderr, "usus: briar-spectator <x.thistle>"
            " [vexilla atrii]\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("briar_spectator",
        (memoriae_index)67108864);
    si (piscina == NIHIL)
    {
        redde I;
    }
    intern = internamentum_creare(piscina);
    /* 'latina.h sine implementatione' - caput solum est */
    silex_monitiones_tacere(VERUM);
    fons   = silex_fons_corporis(piscina, &capsula_corpus_silicis);
    si (fons == NIHIL)
    {
        fprintf(stderr, "briar-spectator: corpus infixum fractum\n");
        redde I;
    }
    dir = _paginam_parare(piscina, intern, fons, via_thistle, &titulus,
        &causa);
    si (dir == NIHIL)
    {
        fprintf(stderr, "briar-spectator: %.*s\n",
            (integer)causa.mensura,
            (constans character*)causa.datum);
        redde I;
    }
    /* '-parare': paginam scribere et exire. Porta fumi identitatem
     * octetorum contra '-html' sic probat SINE fenestra. */
    si (solum_parare)
    {
        imprimere("%s/%s.html\n", dir, titulus);
        redde ZEPHYRUM;
    }
    memset(&figura, ZEPHYRUM, magnitudo(figura));
    figura.titulus        = titulus;
    figura.latitudo       = 1100;
    figura.altitudo       = 860;
    figura.capsula        = &capsula_facies_briar;
    figura.capsula_radix  = dir;
    figura.via_initialis  = _iungere(piscina, titulus, ".html");
    atrium_vexilla_legere(&figura, argc, argv);

    atrium = atrium_creare(piscina, &figura, &causa_atrii);
    si (atrium == NIHIL)
    {
        fprintf(stderr, "briar-spectator: %.*s\n",
            (integer)causa_atrii.mensura,
            (constans character*)causa_atrii.datum);
        redde I;
    }
    imprimere("[spectator] %s\n", figura.via_initialis);
    si (atrium_portus(atrium) != ZEPHYRUM)
    {
        imprimere("[spectator] imperium: 127.0.0.1:%d/imperium\n",
            (integer)atrium_portus(atrium));
    }
    fflush(stdout);
    atrium_monstrare(atrium);
    dum (atrium_currendum(atrium))
    {
        (vacuum)atrium_gressus(atrium);
    }
    atrium_destruere(atrium);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
