/* briar.c (instrumentum) - plagulas .thistle currere: programmata C89
 * litterata (prosa markdown, tags STML, regiones <c!>) cum '#!'.
 *
 *   briar [-vexillum] [-f <radix>] <x.thistle> [argumenta...]
 *   ./x.thistle [argumenta...]      (shebang '#!/usr/bin/env briar')
 *
 * Vexilla (non verba - plagulae thistle scripta sunt): (nihil) =
 * currere (aedificare si abest, deinde programma FIERI); -probatio =
 * probationem regionis munus="probatio" currere (exitus = iudicium);
 * -struere [-iterum] = aedificare solum, directorium proiecti
 * imprimere; -arbor = proiectio STML; -partes = clausura; -amalgama =
 * plagula una <t>.c (+ probatio_<t>.c) iuxta thistle, clang sola
 * compilanda (briar_amalgama); -versio = stampa corporis + sigilla
 * vexillorum. Forma shebang vexilla ut
 * argumentum PRIMUM post plagulam agnoscit ('./x.thistle -probatio');
 * '--' ea programmati relinquit. Regulae in briar_imperium (porta).
 *
 * Corpus: -f > ascensus e cwd (intra arborem rhubarb: discus) > corpus
 * INFIXUM (usus ordinarius scripti). Clavis: infixum = corpus.versio +
 * vexilla + octeti (ante parsuram); discus = contenta clausurae +
 * vexilla + octeti (post fabricam). Proiectum in
 * $HOME/.rhubarb/briar/<titulus>-<clavis>/ (forma silicis).
 * Aedificatio:
 * ./aedificare.sh per processus_exsequi (mora X min); cursus:
 * processus_transformare in bin/<titulus> (PID idem, stdio, cwd).
 * Aedificatio binarii: tools/briar_struere.sh.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "capsula.h"
#include "filum.h"
#include "internamentum.h"
#include "processus.h"
#include "silex.h"
#include "via.h"
#include "xar.h"
#include "briar_amalgama.h"
#include "briar_arbor.h"
#include "briar_contextus.h"
#include "briar_fabrica.h"
#include "briar_imperium.h"
#include "briar_nexus.h"
#include "briar_proiectio.h"
#include "briar_silva.h"
#include "materia_nodus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* corpus bibliothecarum a struere genitum - IDEM obiectum quod silex
 * (tools/corpus_infixum.sh); externus directus */
/* <aedilis obiectum="build/capsula_corpus_silicis.c"/> */
externus constans CapsulaEmbed capsula_corpus_silicis;

#define BRIAR_VERSIO "v0"
#define BRIAR_MORA_AEDIFICANDI_MS 600000

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
_texere (
               Piscina* piscina,
    constans character* a,
    constans character* b,
    constans character* c)
{
    ChordaAedificator* aed = chorda_aedificator_creare(piscina,
        (memoriae_index)256);

    chorda_aedificator_appendere_literis(aed, a);
    chorda_aedificator_appendere_literis(aed, b);
    si (c != NIHIL)
    {
        chorda_aedificator_appendere_literis(aed, c);
    }
    redde chorda_ut_cstr(chorda_aedificator_finire(aed), piscina);
}

interior vacuum
_auxilium (vacuum)
{
    imprimere(
        "briar %s - plagulas .thistle currere\n"
        "usus: briar [-vexillum] [-f <radix>] <x.thistle>"
        " [argumenta...]\n"
        "      ./x.thistle [-vexillum] [argumenta...]\n"
        "  (nihil)     aedificare si abest, deinde programma fieri\n"
        "  -probatio   probationem regionis munus=\"probatio\""
        " currere\n"
        "  -struere    aedificare solum; -iterum = clavem neglegere\n"
        "  -arbor      proiectionem STML imprimere\n"
                "  -partes     clausuram imprimere (via, origo)\n"
        "  -amalgama   plagulam UNAM <t>.c iuxta thistle scribere"
        " (effugium: clang sola)\n"
        "  -versio     stampam corporis et sigilla vexillorum\n"
        "  -f <radix>  arbor rhubarb (alioquin ascensus, alioquin"
        " corpus infixum)\n"
        "  --          post plagulam: vexilla programmati relinquere\n",
        BRIAR_VERSIO);
}

/* fons: -f > ascensus > infixum; *e_disco VERUM si discus */
interior SilexFons*
_fontem_aperire (
               Piscina* piscina,
    constans character* fabrica_opt,
                   b32* e_disco)
{
             SilexFons* fons     = NIHIL;
    constans character* fabrica  = fabrica_opt;

    *e_disco = FALSUM;
    si (fabrica == NIHIL)
    {
        fabrica = silex_fabricam_invenire(piscina, ".");
    }
    si (fabrica != NIHIL)
    {
        fons = silex_fons_disci(piscina, fabrica);
        si (fons == NIHIL && fabrica_opt != NIHIL)
        {
            fprintf(stderr,
                "briar: fabrica invalida (include/ deest): %s\n",
                fabrica);
            redde NIHIL;
        }
        *e_disco = (b32)(fons != NIHIL);
    }
    si (fons == NIHIL)
    {
        fons = silex_fons_corporis(piscina, &capsula_corpus_silicis);
    }
    si (fons == NIHIL)
    {
        fprintf(stderr, "briar: nec fabrica nec corpus - binarium sine"
            " corpore aedificatum?\n");
    }
    redde fons;
}

/* -arbor: proiectio STML (briar_proiectio - latus stml seorsum) */
interior s32
_arborem_imprimere (
         Piscina* piscina,
    MateriaNodus* radix)
{
    constans character* causa = NIHIL;
                chorda  textus = briar_proiectionem_scribere(piscina,
                    radix,
                    &causa);

    si (textus.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "briar: proiectio fracta: %s\n",
            causa ? causa : "-");
        redde I;
    }
    fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
    redde ZEPHYRUM;
}

interior vacuum
_effusionem_scribere (
      FILE* quo,
    chorda  c)
{
    si (c.mensura > ZEPHYRUM)
    {
        fwrite(c.datum, I, (size_t)c.mensura, quo);
    }
}

s32
principale (
      integer   argc,
    character** argv)
{
                 Piscina* piscina;
           BriarImperium  imp;
               SilexFons* fons;
                     b32  e_disco;
               character* textus;
                     i32  mensura = ZEPHYRUM;
           MateriaNodus* doc;
        InternamentumChorda* intern;
                    Xar* nexus;
                    Xar* fragmenta = NIHIL;
    BriarFabricaOptiones optiones;
     BriarFabricaFructus fructus;
                  chorda octeti;
                  chorda via_plena;
     constans character* stampa;
     constans character* dir;
     constans character* binarium;
               character clavis[17];

    piscina = piscina_generare_dynamicum("briar", 33554432);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "briar: piscina generari non potuit\n");
        redde I;
    }
    si (!briar_imperium_legere(piscina, (i32)argc,
        (constans character* constans*)argv, &imp))
    {
        fprintf(stderr, "briar: %.*s\n", (integer)imp.causa.mensura,
            (constans character*)imp.causa.datum);
        redde II;
    }
    si (imp.actio == BRIAR_ACTIO_AUXILIUM)
    {
        _auxilium();
        redde ZEPHYRUM;
    }
    silex_monitiones_tacere(VERUM);
    fons = _fontem_aperire(piscina, imp.fabrica, &e_disco);
    si (fons == NIHIL)
    {
        redde I;
    }
    si (imp.actio == BRIAR_ACTIO_VERSIO)
    {
        character hp[17];
        character hv[17];

        briar_vexilla_sigillum(briar_fabrica_vexilla(BRIAR_FORMA_PLANA),
            hp);
        briar_vexilla_sigillum(
            briar_fabrica_vexilla(BRIAR_FORMA_VITREA), hv);
        imprimere("briar %s\ncorpus: %s%s\nvexilla: plana %s"
            " vitrea %s\n",
            BRIAR_VERSIO, fons->titulus, e_disco ? " (discus)" : "",
            hp, hv);
        redde ZEPHYRUM;
    }

    /* plagula -> arbor -> nexus -> silva -> fabrica */
    textus = _plagulam_legere(piscina, imp.via, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "briar: plagula non lecta: %s\n", imp.via);
        redde I;
    }
    doc = briar_arbor_parsare(piscina, textus, mensura);
    si (doc == NIHIL)
    {
        fprintf(stderr, "briar: parsura NIHIL\n");
        redde I;
    }
    si (imp.actio == BRIAR_ACTIO_ARBOR)
    {
        redde _arborem_imprimere(piscina, doc);
    }
        intern  = internamentum_creare(piscina);
    nexus       = briar_nexus_texere(piscina, doc, intern);
    /* contextus (fragmenta contexta) ANTE silvam: radix cum '<<#x>>'
     * C non est */
    si (   nexus == NIHIL
        || briar_contexere(piscina, nexus, &fragmenta) < ZEPHYRUM
        || briar_silvam_texere(piscina, nexus, fons) < ZEPHYRUM)
    {
        fprintf(stderr, "briar: nexus fractus\n");
        redde I;
    }
    octeti.datum    = (i8*)textus;
    octeti.mensura  = mensura;
    via_plena = via_absoluta(chorda_ex_literis(imp.via, piscina),
        piscina);
    optiones.via_thistle  = via_plena.mensura > ZEPHYRUM
        ? chorda_ut_cstr(via_plena, piscina) : imp.via;
    optiones.fons_titulus  = fons->titulus;
    optiones.stampa        = fons->titulus;
    fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones,
        octeti);
    si (!fructus.successus)
    {
        fprintf(stderr, "%s:%d: %.*s\n", imp.via,
            (integer)fructus.linea_causae,
            (integer)fructus.causa.mensura,
            (constans character*)fructus.causa.datum);
        redde I;
    }
        si (imp.actio == BRIAR_ACTIO_PARTES)
        {
        i32 i;

        /* fragmenta: id, linea tagi, lineae transclusionum */
        per (i = ZEPHYRUM; fragmenta != NIHIL
            && i < xar_numerus(fragmenta);
            i++)
        {
            constans BriarFragmentum* fr =
                (constans BriarFragmentum*)xar_obtinere(fragmenta, i);
            i32 k;

            imprimere("#%.*s\tfragmentum:linea %d\t",
                (integer)fr->id.mensura,
                (constans character*)fr->id.datum,
                (integer)(fr->regio->linea_initium - I));
            si (xar_numerus(fr->usus) == ZEPHYRUM)
            {
                imprimere("non adhibitum\n");
                perge;
            }
            imprimere("adhibitum:lineae ");
            per (k = ZEPHYRUM; k < xar_numerus(fr->usus); k++)
            {
                imprimere("%s%d", k ? ", " : "",
                    (integer)*(i32*)xar_obtinere(fr->usus, k));
            }
            imprimere("\n");
        }
        /* capita DERIVATA (per regionem), ante clausuram */
        per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
        {
            constans BriarNexusRes* r =
                (constans BriarNexusRes*)xar_obtinere(
                nexus, i);
            i32 k;

            si (r->silva == NIHIL || r->silva->capita_derivata == NIHIL)
            {
                perge;
            }
            per (k = ZEPHYRUM; k
                < xar_numerus(r->silva->capita_derivata);
                k++)
            {
                chorda c =
                    *(chorda*)xar_obtinere(r->silva->capita_derivata,
                    k);

                imprimere("%.*s\tderivatum:linea %d\n",
                    (integer)c.mensura,
                    (constans character*)c.datum,
                    (integer)(r->linea_initium - I));
            }
        }
        per (i = ZEPHYRUM; i < xar_numerus(fructus.clausura); i++)
        {
            constans SilexRes* r = (constans SilexRes*)xar_obtinere(
                fructus.clausura, i);

            imprimere("%.*s\t%s\n", (integer)r->via.mensura,
                (constans character*)r->via.datum, r->origo);
        }
        redde ZEPHYRUM;
        }

        si (imp.actio == BRIAR_ACTIO_AMALGAMA)
        {
        BriarAmalgamaFructus am = briar_amalgamare(piscina, &fructus,
            fons, optiones.via_thistle);
                      chorda  causa;
                      chorda  directorium;
          constans character* dir_amalgamae;
                         i32  k;

        si (!am.successus)
        {
            fprintf(stderr, "briar: %s: %.*s\n", imp.via,
                (integer)am.causa.mensura,
                (constans character*)am.causa.datum);
            redde I;
        }
        directorium = via_directorium(chorda_ex_literis(
            optiones.via_thistle, piscina), piscina);
        dir_amalgamae = directorium.mensura > ZEPHYRUM
            ? chorda_ut_cstr(directorium, piscina) : ".";
        si (!briar_amalgama_scribere(piscina, &am, dir_amalgamae,
            &causa))
        {
            fprintf(stderr, "briar: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
            redde I;
        }
        per (k = ZEPHYRUM; k < xar_numerus(am.plagulae); k++)
        {
            constans BriarPlagula* p =
                (constans BriarPlagula*)xar_obtinere(am.plagulae, k);

            imprimere("%s/%.*s\n", dir_amalgamae,
                (integer)p->via.mensura,
                (constans character*)p->via.datum);
        }
        redde ZEPHYRUM;
        }

    /* clavis: infixum = stampa corporis; discus = contenta clausurae */
    stampa = e_disco ? briar_stampa_clausurae(piscina, fructus.clausura)
                     : fons->titulus;
    briar_fabrica_clavem_computare(stampa,
        briar_fabrica_vexilla(fructus.forma), octeti, clavis);
    dir = briar_domus_proiecti(piscina, fructus.titulus, clavis);
    si (dir == NIHIL)
    {
        fprintf(stderr, "briar: HOME ignotum\n");
        redde I;
    }
    binarium = _texere(piscina, dir, "/bin/", fructus.titulus);

    /* aedificare si abest aut -iterum */
    si (!filum_existit(binarium) || imp.iterum)
    {
                     chorda  causa;
         constans character* ordo[3];
          ProcessusResultus  res;

        si (!briar_fabricam_scribere(piscina, &fructus, dir, &causa))
        {
            fprintf(stderr, "briar: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
            redde I;
        }
        ordo[0] = "/bin/sh";
        ordo[1] = _texere(piscina, dir, "/aedificare.sh", NIHIL);
        ordo[2] = NIHIL;
        res = processus_exsequi(ordo, BRIAR_MORA_AEDIFICANDI_MS,
            piscina);
        si (!res.successus || res.codex_exitus != ZEPHYRUM)
        {
            _effusionem_scribere(stderr, res.effusio);
            _effusionem_scribere(stderr, res.erratum);
            fprintf(stderr,
                "briar: aedificatio defecit (%s, exitus %d): %s\n",
                res.successus ? "cucurrit"
                    : processus_error_nomen(res.error),
                (integer)res.codex_exitus, dir);
            redde I;
        }
        si (imp.actio == BRIAR_ACTIO_STRUERE)
        {
            _effusionem_scribere(stdout, res.effusio);
        }
    }
    si (imp.actio == BRIAR_ACTIO_STRUERE)
    {
        imprimere("%s\n", dir);
        redde ZEPHYRUM;
    }
    si (imp.actio == BRIAR_ACTIO_PROBATIO)
    {
        constans character* probatio = _texere(piscina, dir,
            "/bin/probatio_", fructus.titulus);
        constans character* ordo[3];

        si (!fructus.probatio_adest)
        {
            fprintf(stderr,
                "briar: %s: regio munus=\"probatio\" deest\n", imp.via);
            redde II;
        }
        si (filum_existit(probatio))
        {
            ordo[0] = probatio;
            ordo[1] = NIHIL;
        }
        alioquin
        {
            ordo[0] = "/bin/sh";
            ordo[1] = _texere(piscina, dir, "/probare.sh", NIHIL);
            ordo[2] = NIHIL;
        }
        fflush(stdout);
        (vacuum)processus_transformare(ordo);
        fprintf(stderr, "briar: exec defecit: %s\n", ordo[0]);
        redde I;
    }
    /* currere: programma FIERI */
    {
        constans character** ordo =
            (constans character**)piscina_allocare(
            piscina, (memoriae_index)((imp.numerus_reliquorum + II)
                * (i32)magnitudo(constans character*)));
        i32 k;

        ordo[0] = binarium;
        per (k = ZEPHYRUM; k < imp.numerus_reliquorum; k++)
        {
            ordo[k + I] = imp.reliqua[k];
        }
        ordo[imp.numerus_reliquorum + I] = NIHIL;
        fflush(stdout);
        (vacuum)processus_transformare(ordo);
        fprintf(stderr, "briar: exec defecit: %s\n", binarium);
        redde I;
    }
}
