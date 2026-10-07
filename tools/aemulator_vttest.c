/* aemulator_vttest.c - vttest (../vttest, versio 2.7 (20251205), Thomas
 * Dickey; solum ut infans currit, numquam venditum) intra terminale
 * sine fenestra (aemulator-plan D7c)
 *
 * Usus: aemulator_vttest <directorium exitus> <scriptum> <vttest>
 *                        [argumenta...]
 *
 * Scriptum: linea una actio. 'mitte <octeti>' (effugia \r \n \e \\
 * \xHH) octetos ad infantem mittit, deinde exspectat donec effusio
 * quiescit (CD ms sine mutatione, X s summum) et photographiam capit:
 * NNN.txt (textus schirmi) et NNN.png (quadrum pictum, ut fenestra
 * ostenderet). '#' = commentarium. index.tsv: numerus, octeti missi.
 * Schirmum LXXX x XXIV (vttest id exspectat), cellula VI x VIII.
 *
 * Exitus: 0 scriptum totum; I infans ante finem exiit; II usus aut
 * structura fracta.
 */

#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "thema.h"
#include "tabula_pixelorum.h"
#include "ludus_fenestra.h"
#include "pseudoterminale.h"
#include "aemulator.h"
#include "aemulator_hospes.h"
#include "terminale.h"
#include <stdio.h>
#include <string.h>

#define COLUMNAE         LXXX
#define LINEAE           XXIV
#define CELLULA_X        VI
#define CELLULA_Y        VIII
#define PULSUS_MS        L
#define QUIES_PULSUS     VIII    /* CD ms sine mutatione */
#define PULSUS_MAXIMI    CC      /* X s */
#define LINEA_MAXIMA     MXXIV

/* effugia scripti in octetos; mensura reddita */
interior i32
effugia_solvere (
    constans character* fons,
                    i8* octeti)
{
    i32 n;
    i32 v;
    i32 k;
    s32 d;

    n = ZEPHYRUM;
    dum (*fons && *fons != '\n')
    {
        si (*fons != '\\' || !fons[I])
        {
            octeti[n++] = (i8)*fons++;
            perge;
        }
        fons++;
        commutatio (*fons)
        {
            casus 'r': octeti[n++] = '\r'; fons++; frange;
            casus 'n': octeti[n++] = '\n'; fons++; frange;
            casus 'e': octeti[n++] = (i8)0x1B; fons++; frange;
            casus '\\': octeti[n++] = '\\'; fons++; frange;
            casus 'x':
                fons++;
                v = ZEPHYRUM;
                per (k = ZEPHYRUM; k < II && *fons; k++, fons++)
                {
                    d = (*fons >= '0' && *fons <= '9') ? *fons - '0'
                        : (*fons >= 'a'
                            && *fons <= 'f') ? *fons - 'a' + X
                        : (*fons >= 'A'
                            && *fons <= 'F') ? *fons - 'A' + X
                        : -I;
                    si (d < ZEPHYRUM)
                    {
                        frange;
                    }
                    v = v * XVI + (i32)d;
                }
                octeti[n++] = (i8)v;
                frange;
            ordinarius:
                octeti[n++] = (i8)*fons++;
                frange;
        }
    }
    redde n;
}

/* pulsus donec effusio quiescit; FALSUM si infans exiit */
interior b32
quiescere (
    TerminaleApplicatio* app)
{
    AemulatorHospesPulsus p;
                      i32 quieti;
                      i32 k;

    quieti = ZEPHYRUM;
    per (k = ZEPHYRUM; k < PULSUS_MAXIMI && quieti < QUIES_PULSUS; k++)
    {
        p = terminale_pulsare(app, PULSUS_MS);
        si (p.finitus)
        {
            redde FALSUM;
        }
        quieti = p.mutatum ? ZEPHYRUM : quieti + I;
    }
    redde VERUM;
}

/* photographia NNN: textus et imago */
interior b32
photographiam_capere (
    TerminaleApplicatio* app,
          LudusFenestra* lf,
                Piscina* piscina,
     constans character* directorium,
                    i32  numerus,
                    s64* tempus)
{
    character via[CCLVI];
       chorda textus;
        FILE* f;

    *tempus += XVI;
    ludus_quadrum(lf, *tempus);
    sprintf(via, "%s/%03u.png", directorium, (unsigned)numerus);
    si (!ludus_fenestra_imaginem_scribere(lf, via))
    {
        redde FALSUM;
    }
    textus = aemulator_textum_effundere(
        aemulator_hospes_aemulator(app->hospes), piscina);
    sprintf(via, "%s/%03u.txt", directorium, (unsigned)numerus);
    f = fopen(via, "w");
    si (!f)
    {
        redde FALSUM;
    }
    (vacuum)fwrite(textus.datum, I, (memoriae_index)textus.mensura, f);
    (vacuum)fputc('\n', f);
    fclose(f);
    redde VERUM;
}

s32
principale (
                integer   numerus,
     constans character** argumenta)
{
                        Piscina* piscina;
            InternamentumChorda* intern;
                Pseudoterminale* pt;
    PseudoterminaleConfiguratio  cfg_pt;
            TerminaleApplicatio  app;
                TabulaPixelorum* tabula;
                  LudusFenestra* lf;
                          FILE* scriptum;
                          FILE* index;
                      character linea[LINEA_MAXIMA];
                      character via[CCLVI];
                             i8 octeti[LINEA_MAXIMA];
                            i32 n;
                            i32 photographiae;
                            s64 tempus;

    si (numerus < IV)
    {
        fprintf(stderr,
            "usus: aemulator_vttest <directorium> <scriptum> "
                        "<vttest> [argumenta...]\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("aemulator_vttest",
        LXIV * MXXIV * MXXIV);
    intern  = internamentum_creare(piscina);
    thema_initiare();
    pseudoterminale_configuratio_initiare(&cfg_pt);
    cfg_pt.argumenta = argumenta + III;
    cfg_pt.latitudo = COLUMNAE;
    cfg_pt.altitudo = LINEAE;
    pt = pseudoterminale_posix_creare(piscina, &cfg_pt, NIHIL, NIHIL);
    si (   !pt
        || !terminale_applicatio_aedificare(&app, piscina, intern, pt,
            COLUMNAE * CELLULA_X, LINEAE * CELLULA_Y))
    {
        fprintf(stderr,
            "aemulator_vttest: infans aut terminale fractum\n");
        redde II;
    }
    tabula = tabula_pixelorum_creare_nuda(piscina, COLUMNAE * CELLULA_X,
        LINEAE * CELLULA_Y);
    lf = ludus_fenestra_creare(piscina, app.d, app.figurae, ZEPHYRUM,
        NIHIL, NIHIL, tabula);
    scriptum = fopen(argumenta[II], "r");
    sprintf(via, "%s/index.tsv", argumenta[I]);
    index = fopen(via, "w");
    si (!lf || !scriptum || !index)
    {
        fprintf(stderr, "aemulator_vttest: scriptum aut directorium\n");
        redde II;
    }
    tempus         = ZEPHYRUM;
    photographiae  = ZEPHYRUM;
    (vacuum)quiescere(&app);
    dum (fgets(linea, LINEA_MAXIMA, scriptum))
    {
        si (strncmp(linea, "mitte", V) != ZEPHYRUM)
        {
            perge;
        }
        n = effugia_solvere(linea[V] == ' ' ? linea + VI : linea + V,
            octeti);
        si (n > ZEPHYRUM)
        {
            (vacuum)aemulator_hospes_scribere(app.hospes, octeti, n);
        }
        si (!quiescere(&app))
        {
            fprintf(index, "%03u\t(infans exiit) %s",
                (unsigned)photographiae,
                linea);
            fclose(index);
            terminale_claudere(&app);
            redde I;
        }
        si (!photographiam_capere(&app, lf, piscina, argumenta[I],
                photographiae, &tempus))
        {
            fprintf(stderr, "aemulator_vttest: photographia fracta\n");
            redde II;
        }
        fprintf(index, "%03u\t%s", (unsigned)photographiae, linea);
        photographiae++;
    }
    fclose(index);
    fclose(scriptum);
    terminale_claudere(&app);
    printf("aemulator_vttest: %u photographiae in %s\n",
        (unsigned)photographiae, argumenta[I]);
    redde ZEPHYRUM;
}
