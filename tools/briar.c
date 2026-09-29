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
 * compilanda (briar_amalgama); -app [-icon <via>] = fasciculus
 * <t>.app iuxta thistle cum icone (briar_fasciculum); -versio = stampa
 * corporis + sigilla
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
#include "imago.h"
#include "internamentum.h"
#include "processus.h"
#include "silex.h"
#include "via.h"
#include "xar.h"
#include "briar_amalgama.h"
#include "briar_bibliotheca.h"
#include "briar_dialectus.h"
#include "briar_facies.h"
#include "briar_fasciculum.h"
#include "briar_arbor.h"
#include "briar_contextus.h"
#include "briar_fabrica.h"
#include "briar_imperium.h"
#include "briar_mutationes.h"
#include "briar_nexus.h"
#include "briar_proiectio.h"
#include "briar_silva.h"
#include "compendium.h"
#include "materia_nodus.h"
#include "similitudo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* corpus bibliothecarum a struere genitum - IDEM obiectum quod silex
 * (tools/corpus_infixum.sh); externus directus */
/* <aedilis obiectum="build/capsula_corpus_silicis.c"/> */
externus constans CapsulaEmbed capsula_corpus_silicis;
/* vestis faciei (build/capsula_facies_briar.c). Caput genitum NON
 * includitur - silva inclusionem citatam cum '..' solvere nequit
 * (regula speculi, nota tabularii 01KY0T6T64): symbolum est
 * contractus. */
externus constans CapsulaEmbed capsula_facies_briar;
/* icon ordinarius '-app' (build/capsula_icon_briar.c, par. 4.8):
 * symbolum contractus est, caput genitum non includitur */
externus constans CapsulaEmbed capsula_icon_briar;
/* charta mutationum (build/capsula_mutationes_briar.c): versio
 * binarii EX ea legitur - numerus alibi non scribitur */
externus constans CapsulaEmbed capsula_mutationes_briar;
/* identitas aedificationis (build/briar_aedificatio.c, generatum a
 * tools/briar_struere.sh omni aedificatione) */
externus constans character briar_aedificatio_tempus[];
externus constans character briar_aedificatio_fontes[];
externus constans character briar_aedificatio_commissum[];

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

/* briar/MUTATIONES.md ex capsula (vacua si abest) */
interior chorda
_mutationes_legere (
    Piscina* piscina)
{
           Capsula* capsula = capsula_aperire(&capsula_mutationes_briar,
               piscina);
    CapsulaFructus lectum;

    si (capsula == NIHIL)
    {
        redde chorda_ex_literis("", piscina);
    }
    lectum = capsula_legere(capsula, "briar/MUTATIONES.md", piscina);
    redde lectum.datum;
}

/* "v3 — 2026-09-24" ex capite supremo chartae; "v0" si nullum.
 * Mutationes ineditae in charta: "v3+inedita(2) — 2026-09-24" -
 * binarium inter editiones se ipsum nominat (bugs/011 lapidis) */
interior chorda
_versio (
    Piscina* piscina)
{
               chorda charta =
                   _mutationes_legere(piscina);
               chorda caput =
                   briar_mutationes_caput(charta);
                  i32 n =
                      briar_mutationes_inedita(charta);
    ChordaAedificator* aed;
            character  b[XXXII];
                  i32  k      = ZEPHYRUM;

    si (caput.mensura == ZEPHYRUM)
    {
        redde chorda_ex_literis("v0", piscina);
    }
    si (n == ZEPHYRUM)
    {
        redde caput;
    }
    /* "vN" = praefixum usque ad spatium primum */
    dum (k < caput.mensura && caput.datum[k] != (i8)' ')
    {
        k++;
    }
    aed = chorda_aedificator_creare(piscina, (memoriae_index)LXIV);
    chorda_aedificator_appendere_chorda(aed, chorda_sectio(caput,
        ZEPHYRUM, k));
    sprintf(b, "+inedita(%u)", n);
    chorda_aedificator_appendere_literis(aed, b);
    chorda_aedificator_appendere_chorda(aed, chorda_sectio(caput, k,
        caput.mensura));
    redde chorda_aedificator_finire(aed);
}

interior vacuum
_auxilium (
    Piscina* piscina)
{
    /* linea prima = linea prima -versio, ex eodem fonte (_versio):
     * olim 'v%u' solum, sine '+inedita(n)' (lapide bugs/013) */
    chorda versio = _versio(piscina);

    imprimere(
        "briar %.*s\n"
        "plagulas .thistle currere\n"
        "usus: briar [-vexillum] [-f <radix>] <x.thistle>"
        " [argumenta...]\n"
        "      ./x.thistle [-vexillum] [argumenta...]\n"
        "  (nihil)     aedificare si abest, deinde programma fieri\n"
        "  -probatio   probationem regionis munus=\"probatio\""
        " currere\n"
        "  -struere    aedificare solum; -iterum = clavem neglegere\n"
        "  -arbor      proiectionem STML imprimere\n"
                "  -partes     clausuram imprimere (via, origo)\n"
        "  -visio      spectatorem (paginam litteratam) aperire\n"
        "  -html       paginam <t>.html iuxta thistle scribere"
        "\n"
        "  -amalgama   plagulam UNAM <t>.c iuxta thistle scribere"
        " (effugium: clang sola)\n"
        "  -app        fasciculum <t>.app iuxta thistle scribere\n"
        "  -icon <via> cum -app: fons iconis (alioquin infixus)\n"
        "  -versio     stampam corporis et sigilla vexillorum\n"
        "  -bibliothecae         bibliothecas corporis enumerare"
        " (sine plagula)\n"
        "  -bibliotheca <nomen>  caput bibliothecae imprimere;"
        " -fons = et fontes;\n"
        "                        -functiones = signaturae in lineam"
        " unam\n"
        "  -dialectus  charta dialecti: typi, vexilla, verba latina.h,"
        " laquei C89\n"
        "  -mutationes charta mutationum (quid in quaque versione)\n"
        "  -f <radix>  arbor rhubarb (alioquin ascensus, alioquin"
        " corpus infixum)\n"
        "  --          post plagulam: vexilla programmati relinquere\n",
        (integer)versio.mensura, (constans character*)versio.datum);
}


/* ==================================================
 * DOCUMENTATIO CORPORIS (-bibliothecae, -bibliotheca): ex fonte IPSO
 * quem scripta compilant (-f > ascensus > infixum) - versio exacta
 * ================================================== */

/* fontes implementationis bibliothecae: variantes aedilis (_posix,
 * _macos) praeter basem */
hic_manens constans character* constans FORMAE_FONTIUM[] = {
    "lib/%s.c", "lib/%s_posix.c", "lib/%s_macos.c", "lib/%s_macos.m"
};
#define FORMARUM_FONTIUM_NUMERUS \
    ((i32)(magnitudo(FORMAE_FONTIUM) / magnitudo(FORMAE_FONTIUM[0])))

/* via fontis k bibliothecae 'appellatio' (sine '.h') cuius caput in
 * 'radix' sedet: include/ -> formae lib/; radix clientis (toml Q13) ->
 * fons unus iuxta caput ('<radix>x.c'), ceterae formae NIHIL */
interior constans character*
_fontis_via (
                Piscina* piscina,
                 chorda  appellatio,
     constans character* radix,
                    i32  k)
{
    character* via = (character*)piscina_allocare(piscina,
        (memoriae_index)appellatio.mensura
            + (memoriae_index)strlen(radix)
        + XXXII);

    si (strcmp(radix, "include/") != ZEPHYRUM)
    {
        si (k != ZEPHYRUM)
        {
            redde NIHIL;
        }
        sprintf(via, "%s%s.c", radix, chorda_ut_cstr(appellatio,
            piscina));
        redde via;
    }
    sprintf(via, FORMAE_FONTIUM[k], chorda_ut_cstr(appellatio,
        piscina));
    redde via;
}

/* nomina bibliothecarum (capita sine '.h'): include/ primum, deinde
 * radices clientium ordine (SILEX_RADICES_CLIENTIUM - ordo quo silex
 * caput quaerit). 'radices' (NIHIL licet): Xar parallelum de
 * 'constans character*', radix cuiusque ('include/' aut radix). */
interior Xar*
_bibliothecarum_nomina (
               Piscina*  piscina,
    constans SilexFons*  fons,
                   Xar** radices)
{
    Xar* nomina = xar_creare(piscina, (i32)magnitudo(chorda));
    Xar* rr     = xar_creare(piscina,
                      (i32)magnitudo(constans character*));
    s32 r;   /* -I = include/ */
    i32 k;

    per (r = -I; nomina != NIHIL && rr != NIHIL
         && (r < ZEPHYRUM || SILEX_RADICES_CLIENTIUM[r] != NIHIL); r++)
    {
        constans character* radix = r < ZEPHYRUM ? "include/"
            : SILEX_RADICES_CLIENTIUM[r];
         character  directorium[LXIV];
               Xar* capita;

        /* directorium sine '/' caudali */
        sprintf(directorium, "%.*s", (integer)strlen(radix) - I, radix);
        capita = silex_fons_enumerare(fons, directorium, ".h", piscina);
        per (k = ZEPHYRUM; capita != NIHIL && k < xar_numerus(capita);
             k++)
        {
                         chorda   c = *(chorda*)xar_obtinere(capita, k);
                         chorda*  n = (chorda*)xar_addere(nomina);
             constans character** q = (constans character**)xar_addere(
                 rr);

            si (n != NIHIL && q != NIHIL)
            {
                *n = chorda_sectio(c, ZEPHYRUM, c.mensura - II);
                *q = radix;
            }
        }
    }
    si (radices != NIHIL)
    {
        *radices = rr;
    }
    redde nomina;
}

/* radix capitis 'appellatio' (ordo quaestionis) aut NIHIL */
interior constans character*
_radix_bibliothecae (
               Piscina* piscina,
    constans SilexFons* fons,
    constans character* appellatio)
{
    character* via = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(appellatio) + LXIV);
          s32 r;

    per (r = -I; r < ZEPHYRUM || SILEX_RADICES_CLIENTIUM[r] != NIHIL;
         r++)
    {
        constans character* radix = r < ZEPHYRUM ? "include/"
            : SILEX_RADICES_CLIENTIUM[r];

        sprintf(via, "%s%s.h", radix, appellatio);
        si (silex_fons_existit(fons, via, piscina))
        {
            redde radix;
        }
    }
    redde NIHIL;
}

interior vacuum
_corporis_caput (
     constans SilexFons* fons,
                    b32  e_disco)
{
    imprimere("corpus: %s%s\n", fons->titulus,
        e_disco ? " (discus)" : " (infixum)");
}

/* -bibliothecae: appellatio, fontes (h c m), descriptio aut '—' */
interior s32
_bibliothecas_enumerare (
               Piscina* piscina,
    constans SilexFons* fons,
                    b32 e_disco)
{
    Xar* radices   = NIHIL;
    Xar* nomina    = _bibliothecarum_nomina(piscina, fons, &radices);
    i32  latitudo  = VI;
    i32  sine      = ZEPHYRUM;
    i32  k;

    si (nomina == NIHIL || xar_numerus(nomina) == ZEPHYRUM)
    {
        fprintf(stderr, "briar: corpus sine include/*.h\n");
        redde I;
    }
    per (k = ZEPHYRUM; k < xar_numerus(nomina); k++)
    {
        chorda n = *(chorda*)xar_obtinere(nomina, k);

        latitudo = n.mensura > latitudo ? n.mensura : latitudo;
    }
    _corporis_caput(fons, e_disco);
    imprimere("%-*s  %-6s  %s\n", (integer)latitudo, "nomen", "fontes",
        "descriptio");
    per (k = ZEPHYRUM; k < xar_numerus(nomina); k++)
    {
                chorda  n = *(chorda*)xar_obtinere(nomina, k);
    constans character* radix = *(constans character**)xar_obtinere(
        radices, k);
        character  fontes[VIII];
        character* caput_via = (character*)piscina_allocare(piscina,
            (memoriae_index)n.mensura + (memoriae_index)strlen(radix)
            + XVI);
           b32 inventum = FALSUM;
        chorda textus;
        chorda desc;
           i32 f;
           i32 p = ZEPHYRUM;

        sprintf(caput_via, "%s%s.h", radix, chorda_ut_cstr(n, piscina));
        textus = silex_fons_legere(fons, caput_via, piscina, &inventum);
        desc = briar_bibliotheca_descriptio(textus, chorda_ex_literis(
            caput_via + strlen(radix), piscina));
        fontes[p++]  = 'h';
        fontes[p]    = '\0';
        per (f = ZEPHYRUM; f < FORMARUM_FONTIUM_NUMERUS; f++)
        {
             constans character* v = _fontis_via(piscina, n, radix, f);
                      character  signum;

            si (v == NIHIL)
            {
                perge;
            }
            signum = v[strlen(v) - I] == 'm' ? 'm' : 'c';
            si (   silex_fons_existit(fons, v, piscina)
                && strchr(fontes, signum) == NIHIL)
            {
                fontes[p++] = ' ';
                fontes[p++] = signum;
            }
            fontes[p] = '\0';
        }
        fontes[p] = '\0';
        si (desc.mensura == ZEPHYRUM)
        {
            sine++;
        }
        imprimere("%-*.*s  %-6s  %.*s\n", (integer)latitudo,
            (integer)n.mensura, (constans character*)n.datum, fontes,
            (integer)(desc.mensura > ZEPHYRUM ? desc.mensura : III),
            desc.mensura > ZEPHYRUM ? (constans character*)desc.datum
            : "\xe2\x80\x94");
    }
    imprimere("bibliothecae %d \xc2\xb7 sine descriptione %d\n",
        (integer)xar_numerus(nomina), (integer)sine);
    redde ZEPHYRUM;
}

/* plagulam imprimere cum vexillo; VERUM si inventa */
interior b32
_plagulam_ostendere (
               Piscina* piscina,
    constans SilexFons* fons,
    constans character* via)
{
       b32 inventum  = FALSUM;
    chorda textus    = silex_fons_legere(fons, via, piscina, &inventum);

    si (!inventum)
    {
        redde FALSUM;
    }
    imprimere("\n==== %s ====\n", via);
    fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
    si (   textus.mensura > ZEPHYRUM
        && textus.datum[textus.mensura - I] != '\n')
    {
        fputc('\n', stdout);
    }
    redde VERUM;
}

/* -dialectus: charta dialecti ex latina.h CORPORIS et vexillis quae
 * briar clang tradit (derivata - non putrescit) */
interior s32
_dialectum_ostendere (
                Piscina* piscina,
     constans SilexFons* fons,
                    b32  e_disco)
{
       b32 inventum  = FALSUM;
    chorda latina    = silex_fons_legere(fons, "include/latina.h",
        piscina, &inventum);
    chorda charta;

    si (!inventum)
    {
        fprintf(stderr, "briar: include/latina.h in corpore abest\n");
        redde I;
    }
    charta = briar_dialectus_charta(latina,
        briar_fabrica_vexilla(BRIAR_FORMA_PLANA),
        briar_fabrica_vexilla(BRIAR_FORMA_VITREA), piscina);
    _corporis_caput(fons, e_disco);
    fwrite(charta.datum, I, (size_t)charta.mensura, stdout);
    redde charta.mensura > ZEPHYRUM ? ZEPHYRUM : I;
}

/* -bibliotheca <appellatio> -functiones: signaturae functionum capitis
 * in lineam unam, typi reditus alineati. Declarationes per silvam
 * (compendium capitis - idem quod legati caput), caput ut plagula
 * PRINCIPALIS cum clausura e fonte silicis. */
interior s32
_functiones_ostendere (
               Piscina* piscina,
    constans SilexFons* fons,
    constans character* caput_via)
{
         chorda  textus;
     BriarSilva* silva = briar_silvam_capitis_texere(piscina, fons,
         caput_via, &textus);
            Xar* declarationes;
            Xar* ordo;
         chorda* lineae;
         chorda* tituli;
            i32  numerus = ZEPHYRUM;
            i32  i;
         chorda  charta;

    si (silva == NIHIL)
    {
        fprintf(stderr, "briar: %s parsari non potuit\n", caput_via);
        redde I;
    }
    si (silva->parsura->numerus_errorum > ZEPHYRUM)
    {
        fprintf(stderr, "briar: %s: parsura cum %u erroribus -"
            " signaturae fortasse incompletae\n", caput_via,
            silva->parsura->numerus_errorum);
    }
    declarationes = compendium_declarationes(silva->parsura,
        silva->semantica, piscina);
    ordo = compendium_ordo(declarationes, piscina);
    si (ordo == NIHIL)
    {
        briar_silvam_capitis_solvere(silva);
        redde I;
    }
    lineae = (chorda*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(chorda)
        * (memoriae_index)(xar_numerus(ordo) + I));
    tituli = (chorda*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(chorda)
        * (memoriae_index)(xar_numerus(ordo) + I));
    si (lineae == NIHIL || tituli == NIHIL)
    {
        briar_silvam_capitis_solvere(silva);
        redde I;
    }
    per (i = ZEPHYRUM; i < xar_numerus(ordo); i++)
    {
        constans CompendiumDeclaratio* d =
            (constans CompendiumDeclaratio*)xar_obtinere(declarationes,
                *(i32*)xar_obtinere(ordo, i));
        chorda corpus;

        si (d->genus != (s32)SYMBOLUM_FUNCTIO)
        {
            perge;
        }
        corpus.datum    = textus.datum + d->corpus_initium;
        corpus.mensura  = (i32)(d->corpus_finis - d->corpus_initium);
        lineae[numerus]   = compendium_contrahere(corpus, (i32)M,
            piscina);
        tituli[numerus]   = d->titulus;
        numerus++;
    }
    briar_silvam_capitis_solvere(silva);
    charta = briar_bibliotheca_functiones(lineae, tituli, numerus,
        piscina);
    imprimere("\n==== %s (functiones %d) ====\n", caput_via,
        (integer)numerus);
    fwrite(charta.datum, I, (size_t)charta.mensura, stdout);
    redde ZEPHYRUM;
}

/* -bibliotheca <appellatio> [-fons]: caput (API cum commentariis) et,
 * cum -fons, implementatio; ignotum = proximi nominati */
interior s32
_bibliothecam_ostendere (
               Piscina* piscina,
    constans SilexFons* fons,
                    b32 e_disco,
    constans character* appellatio,
                    b32 cum_fontibus,
                    b32 solae_functiones)
{
    character* caput_via = (character*)piscina_allocare(piscina,
        (memoriae_index)strlen(appellatio) + LXIV);   /* radix + ".h" */
    i32 f;

    constans character* radix = _radix_bibliothecae(piscina, fons,
        appellatio);

    sprintf(caput_via, "%s%s.h", radix != NIHIL ? radix : "include/",
        appellatio);
    si (radix == NIHIL)
    {
           Xar* nomina = _bibliothecarum_nomina(piscina, fons,
               NIHIL);
        chorda quaesita = chorda_ex_literis(appellatio, piscina);

        SimilitudoFructus optimi[V];
                      i32 n = ZEPHYRUM;
                      i32 k;

        si (nomina != NIHIL && xar_numerus(nomina) > ZEPHYRUM)
        {
            /* Xar segmentata est: tabula contigua per copiam, non
             * xar_obtinere(nomina, ZEPHYRUM) ut tabula (segmenta
             * contigua solum casu piscinae erant) */
            chorda* tabula = (chorda*)piscina_allocare(piscina,
                (memoriae_index)xar_numerus(nomina)
                * (memoriae_index)magnitudo(chorda));

            (vacuum)xar_copiare_ad_tabulam(nomina, tabula, ZEPHYRUM,
                xar_numerus(nomina));
            /* decurtata: nomen male scriptum in cauda ('sorss') -
             * praefixum optimum punctatur (subsequentia sola nihil
             * inveniret) */
            n = similitudo_optima_decurtata(quaesita, tabula,
                xar_numerus(nomina), optimi, V);
        }
        fprintf(stderr, "briar: bibliotheca ignota: %s", appellatio);
        per (k = ZEPHYRUM; k < n; k++)
        {
            chorda c = *(chorda*)xar_obtinere(nomina,
                (i32)optimi[k].index);

            fprintf(stderr, "%s%.*s", k == ZEPHYRUM ? " (fortasse: "
                : ", ", (integer)c.mensura,
                (constans character*)c.datum);
        }
        fprintf(stderr, "%s\n", n > ZEPHYRUM ? ")" : "");
        redde I;
    }
    _corporis_caput(fons, e_disco);
    si (solae_functiones)
    {
        redde _functiones_ostendere(piscina, fons, caput_via);
    }
    (vacuum)_plagulam_ostendere(piscina, fons, caput_via);
    si (cum_fontibus)
    {
        per (f = ZEPHYRUM; f < FORMARUM_FONTIUM_NUMERUS; f++)
        {
            constans character* v = _fontis_via(piscina,
                chorda_ex_literis(appellatio, piscina), radix, f);

            si (v != NIHIL)
            {
                (vacuum)_plagulam_ostendere(piscina, fons, v);
            }
        }
    }
    redde ZEPHYRUM;
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

/* -app (par. 4.8): consilium, icon DECODIFICATUS, fasciculus. Hic
 * SOLUM stb_image vivit (per imago.h): moduli briar pixela accipiunt,
 * numquam vias imaginum (icones D7). */
interior s32
_fasciculum_facere (
                   Piscina* piscina,
                       Xar* nexus,
    constans BriarImperium* imp,
        constans character* via_thistle,
        constans character* domus,
        constans character* binarium)
{
    BriarFasciculumConsilium consilium;
                      chorda causa;
                         i32 linea = ZEPHYRUM;
                ImagoFructus icon;

    si (!briar_fasciculum_consilium(piscina, nexus, via_thistle,
            imp->icon, &consilium, &causa, &linea))
    {
        fprintf(stderr, "%s:%d: %.*s\n", imp->via, (integer)linea,
            (integer)causa.mensura, (constans character*)causa.datum);
        redde I;
    }
    si (chorda_vacua(consilium.via_icon))
    {
              Capsula* capsula = capsula_aperire(&capsula_icon_briar,
                  piscina);
        CapsulaFructus lectum;

        si (capsula == NIHIL)
        {
            fprintf(stderr, "briar: capsula iconis non aperta\n");
            redde I;
        }
        lectum = capsula_legere(capsula,
            "briar/icon/app-icon-transparent.png", piscina);
        si (lectum.datum.mensura == ZEPHYRUM)
        {
            fprintf(stderr,
                "briar: icon ordinarius in capsula deest\n");
            redde I;
        }
        icon = imago_caricare_ex_memoria(lectum.datum.datum,
            lectum.datum.mensura, piscina);
    }
    alioquin
    {
        icon = imago_caricare_ex_file(
            chorda_ut_cstr(consilium.via_icon, piscina), piscina);
    }
    si (!icon.successus)
    {
        fprintf(stderr, "briar: icon non decodificatus (%s): %.*s\n",
            chorda_vacua(consilium.via_icon) ? "infixus"
                : chorda_ut_cstr(consilium.via_icon, piscina),
            (integer)icon.error.mensura,
            (constans character*)icon.error.datum);
        redde I;
    }
    si (!briar_fasciculum_scribere(piscina, &consilium, &icon.imago,
            binarium, domus, &causa))
    {
        fprintf(stderr, "briar: %.*s\n", (integer)causa.mensura,
            (constans character*)causa.datum);
        redde I;
    }
    imprimere("%.*s\n", (integer)consilium.via_app.mensura,
        (constans character*)consilium.via_app.datum);
    redde ZEPHYRUM;
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
     constans character* stampa;
     constans character* dir;
     constans character* binarium;
               character clavis[17];
             BriarVestis vestis_visionis;

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
        _auxilium(piscina);
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
           chorda versio = _versio(piscina);

        briar_vexilla_sigillum(briar_fabrica_vexilla(BRIAR_FORMA_PLANA),
            hp);
        briar_vexilla_sigillum(
            briar_fabrica_vexilla(BRIAR_FORMA_VITREA), hv);
        imprimere("briar %.*s\naedificatum: %s \xc2\xb7 fontes briar %s"
            " (%s)\ncorpus: %s%s\nvexilla: plana %s vitrea %s\n",
            (integer)versio.mensura, (constans character*)versio.datum,
            briar_aedificatio_tempus, briar_aedificatio_fontes,
            briar_aedificatio_commissum, fons->titulus,
            e_disco ? " (discus)" : "", hp, hv);
        redde ZEPHYRUM;
    }
    si (imp.actio == BRIAR_ACTIO_MUTATIONES)
    {
        chorda charta = _mutationes_legere(piscina);

        si (charta.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "briar: charta mutationum in capsula"
                " deest\n");
            redde I;
        }
        fwrite(charta.datum, I, (size_t)charta.mensura, stdout);
        redde ZEPHYRUM;
    }
    si (imp.actio == BRIAR_ACTIO_BIBLIOTHECAE)
    {
        redde _bibliothecas_enumerare(piscina, fons, e_disco);
    }
    si (imp.actio == BRIAR_ACTIO_BIBLIOTHECA)
    {
        redde _bibliothecam_ostendere(piscina, fons, e_disco,
            imp.bibliotheca, imp.fons_bibliothecae,
            imp.functiones_bibliothecae);
    }
    si (imp.actio == BRIAR_ACTIO_DIALECTUS)
    {
        redde _dialectum_ostendere(piscina, fons, e_disco);
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
        || briar_silvam_texere(piscina, nexus, fons, imp.via)
            < ZEPHYRUM)
    {
        fprintf(stderr, "briar: nexus fractus\n");
        redde I;
    }
    octeti.datum    = (i8*)textus;
    octeti.mensura  = mensura;
    briar_optiones_plagulae(piscina, fons, imp.via, &optiones);
    fructus = briar_fabricare(piscina, doc, nexus, fons, &optiones,
        octeti);
    /* '-html' recusationem TRANSIT: lex par. 4.6 F4 - pagina semper
     * redditur, causa in margine ad lineam suam. Plagula fracta est
     * ipsum momentum quo eam VIDERE maxime prodest (circulus inter
     * fragmenta quattuor paragraphus in stderr est, sed PICTURA in
     * margine). Causa tamen in stderr manet, cursoribus. */
    si (!fructus.successus)
    {
        fprintf(stderr, "%s:%d: %.*s\n", imp.via,
            (integer)fructus.linea_causae,
            (integer)fructus.causa.mensura,
            (constans character*)fructus.causa.datum);
        si (imp.actio != BRIAR_ACTIO_HTML)
        {
            redde I;
        }
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

        si (imp.actio == BRIAR_ACTIO_VISIO)
        {
        /* spectator binarium SEPARATUM est (par. 4.7 S1): briar
         * sine capite manet - fenestram numquam ipse aperit, ne
         * cursus omnis Cocoa ferat. Absens = recusatio quae eum
         * NOMINAT et scriptum aedificans dicit. */
        constans character* argumenta[3];

        argumenta[0] = "briar-spectator";
        argumenta[1] = optiones.via_thistle;
        argumenta[2] = NIHIL;
        si (!processus_transformare(argumenta))
        {
            fprintf(stderr, "briar: 'briar-spectator' non inventum in"
                " PATH - ./tools/briar_spectator_struere.sh eum"
                " aedificat\n");
            redde I;
        }
        redde ZEPHYRUM;   /* numquam huc pervenitur */
        }

        si (imp.actio == BRIAR_ACTIO_HTML)
        {
                  BriarVestis  vestis;
                       chorda  pagina;
                       chorda  causa;
                       chorda  directorium;
           constans character* dir_paginae;
           constans character* via_paginae;

        si (!briar_vestem_legere(piscina, &capsula_facies_briar,
            &vestis, &causa))
        {
            fprintf(stderr, "briar: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
            redde I;
        }
        pagina = briar_faciem_fingere(piscina, intern, nexus,
            fragmenta, &fructus, octeti, optiones.via_thistle, &vestis,
            &causa);
        si (pagina.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "briar: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
            redde I;
        }
        directorium = via_directorium(chorda_ex_literis(
            optiones.via_thistle, piscina), piscina);
        dir_paginae = directorium.mensura > ZEPHYRUM
            ? chorda_ut_cstr(directorium, piscina) : ".";
        via_paginae = chorda_ut_cstr(chorda_concatenare(
            chorda_concatenare(chorda_ex_literis(dir_paginae, piscina),
            chorda_ex_literis("/", piscina), piscina),
            chorda_concatenare(chorda_ex_literis(fructus.titulus,
            piscina), chorda_ex_literis(".html", piscina), piscina),
            piscina), piscina);
        si (!filum_scribere(via_paginae, pagina))
        {
            fprintf(stderr, "briar: pagina non scripta: %s\n",
                via_paginae);
            redde I;
        }
        imprimere("%s\n", via_paginae);
        redde ZEPHYRUM;
        }

    /* clavis: infixum = stampa corporis; discus = contenta clausurae */
    stampa = e_disco ? briar_stampa_clausurae(piscina, fructus.clausura)
                     : fons->titulus;
    /* fontes briar IPSIUS clavem intrant: fabrica mutata (e.g.
     * prototypi regionis probationis) proiectum novum poscit etiam sub
     * eodem commisso corporis - stampa SORDIDA inter aedificationes
     * eadem manet, et cache codicem genitum veterem reddebat
     * (2026-09-29) */
    stampa = _texere(piscina, stampa, " fontes briar ",
        briar_aedificatio_fontes);
    /* VISIO (par. 4.9): programma vitreum paginam suam in binario
     * fert, ergo vestis clavem intrat (V5) - briar cum vestibus novis
     * aedificat, paginam veterem e cache non reddit */
    si (fructus.forma == BRIAR_FORMA_VITREA)
    {
        chorda causa;

        si (!briar_vestem_legere(piscina, &capsula_facies_briar,
            &vestis_visionis, &causa))
        {
            fprintf(stderr, "briar: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
            redde I;
        }
        stampa = briar_stampa_vestita(piscina, stampa,
            &vestis_visionis);
    }
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

        /* pagina SOLUM cum proiectum scribitur: ictus cache nihil
         * reddit. Optiones eaedem ac '-html' (briar_optiones_plagulae),
         * ergo pagina eadem PER CONSTRUCTIONEM - fumus IV id probat */
        si (fructus.forma == BRIAR_FORMA_VITREA)
        {
            chorda pagina;

            causa.datum    = NIHIL;
            causa.mensura  = ZEPHYRUM;
            pagina = briar_faciem_fingere(piscina, intern, nexus,
                fragmenta, &fructus, octeti, optiones.via_thistle,
                &vestis_visionis, &causa);
            si (   pagina.mensura == ZEPHYRUM
                || !briar_visionem_addere(piscina, &fructus, pagina))
            {
                fprintf(stderr, "briar: visio non reddita: %.*s\n",
                    (integer)causa.mensura,
                    (constans character*)causa.datum);
                redde I;
            }
        }
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
    /* -app: aedificatio iam facta (supra), binarium in fasciculum */
    si (imp.actio == BRIAR_ACTIO_APP)
    {
        redde _fasciculum_facere(piscina, nexus, &imp,
            optiones.via_thistle, dir, binarium);
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
