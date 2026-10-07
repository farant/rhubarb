/* conflator.c - plagula C UNA ex programmate et clausura eius
 * (dev-time; consumptor: knotapel/archive.sh)
 *
 * Usus (ex radice repositorii):
 *   conflator [-r <radix_clientis/>]... <partes.tsv> <statica.tsv>
 *             <fons_radicis.c> <prooemium> <exitus.c>
 *
 * partes.tsv = 'aedilis <fons> --partes' (C caput, O obiectum, S
 * systema); statica.tsv = corpus.symbola.tsv (tools/corpus_infixum.sh);
 * radices clientium ex eodem (RADICES_CLIENTIUM, cum '/' finali).
 * Exitus: prooemium verbatim, capita ordine dependentiae, fontes cum
 * staticis per plagulam renominatis, fons radicis ultimus - mechanismus
 * silva_conflatio. Recusat (causa nominata, exitus 1): venditorium,
 * Objective-C, plagulam clausurae extra include/, lib/ et radices.
 */

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "xar.h"
#include "silva_conflatio.h"
#include <stdio.h>
#include <string.h>

#define RADICES_MAXIMAE XVI

interior chorda
_legere (
               Piscina* piscina,
    constans character* via,
                   b32* bene)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    *bene      = filum_existit(via);
    si (*bene)
    {
        c = filum_legere_totum(via, piscina);
    }
    alioquin
    {
        fprintf(stderr, "conflator: plagula non lecta: %s\n", via);
    }
    redde c;
}

interior b32
_terminatur (
                chorda  c,
    constans character* suffixum)
{
    i32 m = (i32)strlen(suffixum);

    redde (b32)(c.mensura >= m
        && memcmp(c.datum + (c.mensura - m), suffixum, (size_t)m)
            == ZEPHYRUM);
}

/* partes.tsv -> clausura (Xar de ConflatioPlagula); FALSUM cum causa
 * in stderr */
interior b32
_clausuram_legere (
                        Piscina* piscina,
    constans ConflatioContextus* contextus,
                         chorda  partes,
                            Xar* clausura)
{
    i32 i = ZEPHYRUM;

    dum (i < partes.mensura)
    {
           i32 f = i;
        chorda linea;
        chorda via;
     character genus;

        dum (f < partes.mensura && (character)partes.datum[f] != '\n')
        {
            f = f + I;
        }
        linea  = chorda_sectio(partes, i, f);
        i      = f + I;
        si (linea.mensura < III || (character)linea.datum[I] != '\t')
        {
            perge;
        }
        genus  = (character)linea.datum[ZEPHYRUM];
        via    = chorda_sectio(linea, II, linea.mensura);
        si (genus == 'S')
        {
            perge;   /* systema: inclusio manet in loco suo */
        }
        si (genus == 'V')
        {
            fprintf(stderr, "conflator: venditorium in clausura: %.*s"
                " - recusatur (sub vexillis domus non compilat)\n",
                (int)via.mensura, (constans character*)via.datum);
            redde FALSUM;
        }
        si (genus != 'C' && genus != 'O')
        {
            fprintf(stderr, "conflator: genus partis ignotum '%c': "
                "%.*s\n", genus, (int)via.mensura,
                (constans character*)via.datum);
            redde FALSUM;
        }
        si (_terminatur(via, ".m"))
        {
            fprintf(stderr, "conflator: Objective-C in clausura: %.*s"
                " - recusatur\n", (int)via.mensura,
                (constans character*)via.datum);
            redde FALSUM;
        }
        si (   !conflatio_caput_est(contextus, via)
            && !conflatio_fons_est(contextus, via))
        {
            fprintf(stderr, "conflator: plagula clausurae extra "
                "include/, lib/ et radices clientium: %.*s - "
                "recusatur\n", (int)via.mensura,
                (constans character*)via.datum);
            redde FALSUM;
        }
        {
            ConflatioPlagula* p = (ConflatioPlagula*)xar_addere(
                clausura);
                         b32 bene;

            p->via        = via;
            p->contentum  = _legere(piscina, chorda_ut_cstr(via,
                piscina), &bene);
            si (!bene)
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

s32
principale (
          s32   argc,
    character** argv)
{
               Piscina* piscina;
    constans character* radices[RADICES_MAXIMAE + I];
                   i32  numerus_radicum  = ZEPHYRUM;
                   s32  a                = I;
    ConflatioContextus  contextus;
                   Xar* clausura;
                   Xar* capita;
                   Xar* fontes;
     ChordaAedificator* aedificator;
      ConflatioPlagula  radix;
                chorda  partes;
                chorda  statica;
                chorda  prooemium;
                   b32  bene;
                   i32  k;

    dum (a + I < argc && strcmp(argv[a], "-r") == ZEPHYRUM)
    {
        si (numerus_radicum >= RADICES_MAXIMAE)
        {
            fprintf(stderr, "conflator: radices nimiae\n");
            redde I;
        }
        radices[numerus_radicum++]  = argv[a + I];
        a                           = a + II;
    }
    radices[numerus_radicum] = NIHIL;
    si (argc - a != V)
    {
        fprintf(stderr, "usus: conflator [-r <radix/>]... <partes.tsv>"
            " <statica.tsv> <fons_radicis.c> <prooemium> <exitus.c>\n");
        redde I;
    }
    piscina = piscina_generare_dynamicum("conflator", 4194304);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "conflator: piscina non generata\n");
        redde I;
    }
    contextus.radices_clientium  = radices;
    contextus.statica            = NIHIL;

    partes = _legere(piscina, argv[a], &bene);
    si (!bene)
    {
        redde I;
    }
    statica = _legere(piscina, argv[a + I], &bene);
    si (   !bene
        || !conflatio_statica_legere(piscina, statica, &contextus))
    {
        fprintf(stderr, "conflator: tabula staticorum non lecta\n");
        redde I;
    }
    radix.via        = chorda_ex_literis(argv[a + II], piscina);
    radix.contentum  = _legere(piscina, argv[a + II], &bene);
    si (!bene)
    {
        redde I;
    }
    prooemium = _legere(piscina, argv[a + III], &bene);
    si (!bene)
    {
        redde I;
    }

    clausura = xar_creare(piscina, (i32)magnitudo(ConflatioPlagula));
    si (!_clausuram_legere(piscina, &contextus, partes, clausura))
    {
        redde I;
    }
    capita  = conflatio_capita_ordinare(piscina, &contextus, clausura);
    fontes  = conflatio_fontes_ordinare(piscina, &contextus, clausura,
        capita);

    aedificator = chorda_aedificator_creare(piscina, (memoriae_index)
        1048576);
    chorda_aedificator_appendere_chorda(aedificator, prooemium);
    per (k = ZEPHYRUM; k < xar_numerus(capita); k++)
    {
        conflatio_plagulam_emittere(aedificator,
            *(constans ConflatioPlagula**)xar_obtinere(capita, k));
    }
    per (k = ZEPHYRUM; k < xar_numerus(fontes); k++)
    {
        conflatio_fontem_emittere(aedificator, &contextus,
            *(constans ConflatioPlagula**)xar_obtinere(fontes, k));
    }
    conflatio_plagulam_emittere(aedificator, &radix);

    si (!filum_scribere(argv[a + IV], chorda_aedificator_finire(
        aedificator)))
    {
        fprintf(stderr, "conflator: exitus non scriptus: %s\n",
            argv[a + IV]);
        redde I;
    }
    imprimere("conflator: %s (capita %d, fontes %d, radix %s)\n",
        argv[a + IV], (s32)xar_numerus(capita),
        (s32)xar_numerus(fontes),
        argv[a + II]);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
