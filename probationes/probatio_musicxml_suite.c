/* probatio_musicxml_suite.c - lector MusicXML contra aurum commissum
 *
 * Fontes in probationes/fixa/musicxml/aurum/INDEX (suita W3C - MIT,
 * commissa, suite/PROVENIENTIA.md - et fixa manu scripta); aurum
 * aurum/<nomen>.tsv = tabula NOSTRA (musicxml_tabulam_scribere in ordine
 * exsecutionis) + lineae vitiorum '!\tgravitas\tgenus\tlinea:columna'.
 * Lineae '#' non conferuntur (commentaria, '# divergentia:' ubi
 * music21 discordavit et nostrum servatum est). Aurum semel per
 * oraculum music21 aedificatum (tools/musicxml_oraculum.py, plan P7),
 * recensitum, commissum - haec porta music21 numquam vocat (M11).
 *
 * MUSICXML_TABULAS_SCRIBERE=<directorium>: tabulas nostras ibi scribit
 * (ad discrimen cum oraculo et ad aurum renovandum - causa in
 * commissione), nihil confert.
 */

#include "latina.h"
#include "credo.h"
#include "musicxml.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "lectiones.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

#define RADIX_FIXORUM "probationes/fixa/musicxml/"

interior vacuum
_vitia_appendere (
          ChordaAedificator* a,
    constans MusicxmlVitium* vitia,
                        i32  numerus)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda_aedificator_appendere_literis(a,
            vitia[i].gravitas == MUSICXML_VITIUM ? "!\tvitium\t"
                                                 : "!\tmonitum\t");
        chorda_aedificator_appendere_i32(a, (i32)vitia[i].genus);
        chorda_aedificator_appendere_character(a, '\t');
        chorda_aedificator_appendere_i32(a, vitia[i].linea);
        chorda_aedificator_appendere_character(a, ':');
        chorda_aedificator_appendere_i32(a, vitia[i].columna);
        chorda_aedificator_appendere_character(a, '\n');
    }
}

/* tabula nostra in ordine exsecutionis + vitia (opus, deinde ordo) */
interior chorda
_tabula_cum_vitiis (
                Piscina* p,
     constans character* via)
{
         MusicxmlOpus* o;
         MusicxmlOrdo* r;
    ChordaAedificator* a = chorda_aedificator_creare(p, MMMCMXCIX);

    o = musicxml_legere(filum_legere_totum(via, p), p);
    si (o == NIHIL || a == NIHIL)
    {
        redde chorda_ex_literis("", p);
    }
    r = musicxml_ordo(o, p);
    (vacuum)musicxml_tabulam_scribere(o, r, a);
    _vitia_appendere(a, o->vitia, o->numerus_vitiorum);
    si (r != NIHIL)
    {
        _vitia_appendere(a, r->vitia, r->numerus_vitiorum);
    }
    redde chorda_aedificator_finire(a);
}

/* lineae sine '#' */
interior chorda
_sine_commentis (
     Piscina* p,
      chorda  t)
{
     ChordaAedificator* a = chorda_aedificator_creare(p, MMMCMXCIX);
                   i32  i = ZEPHYRUM;

    dum (i < t.mensura)
    {
        i32 j = i;

        dum (j < t.mensura && t.datum[j] != '\n')
        {
            j++;
        }
        si (t.datum[i] != '#')
        {
            chorda_aedificator_appendere_chorda(a,
                chorda_sectio(t, i, j < t.mensura ? j + I : j));
        }
        i = j + I;
    }
    redde chorda_aedificator_finire(a);
}

/* linea prima discors (ad nuntium) */
interior vacuum
_discrimen_monstrare (
     constans character* titulus,
                 chorda  aurum,
                 chorda  nostra)
{
    i32 i      = ZEPHYRUM;
    i32 linea  = I;
    i32 a;
    i32 b;

    dum (   i < aurum.mensura && i < nostra.mensura
         && aurum.datum[i] == nostra.datum[i])
    {
        si (aurum.datum[i] == '\n')
        {
            linea++;
        }
        i++;
    }
    a = i;
    dum (a > ZEPHYRUM && aurum.datum[a - I] != '\n')
    {
        a--;
    }
    b = a;
    imprimere("  DISCORS %s linea %u\n    aurum : ", titulus,
        (insignatus integer)linea);
    dum (b < aurum.mensura && aurum.datum[b] != '\n')
    {
        imprimere("%c", (integer)aurum.datum[b++]);
    }
    imprimere("\n    nostra: ");
    b = a;
    dum (b < nostra.mensura && nostra.datum[b] != '\n')
    {
        imprimere("%c", (integer)nostra.datum[b++]);
    }
    imprimere("\n");
}

interior vacuum
_suitam_probare (
    CredoContextus* c)
{
                 chorda  index;
     constans character* directorium;
                    i32  i          = ZEPHYRUM;
                    i32  numerus    = ZEPHYRUM;
                    i32  discordes  = ZEPHYRUM;
                    b32  prima      = VERUM;

    index = filum_legere_totum(RADIX_FIXORUM "aurum/INDEX", c->piscina);
    CREDO_NECESSE (index.mensura > ZEPHYRUM);
    directorium = lectiones_ambitus("MUSICXML_TABULAS_SCRIBERE");
    dum (i < index.mensura)
    {
                       i32  j = i;
                 character  via[DXII];
                 character  aurum_via[DXII];
                    chorda  linea;
                    chorda  nostra;
        constans character* nomen_fasciculi;

        dum (j < index.mensura && index.datum[j] != '\n')
        {
            j++;
        }
        linea  = chorda_sectio(index, i, j);
        i      = j + I;
        si (   prima || linea.mensura == ZEPHYRUM
            || linea.datum[ZEPHYRUM] == '#'
            || linea.mensura > CD)
        {
            prima = FALSUM;
            perge;
        }
        strcpy(via, RADIX_FIXORUM);
        memcpy(via + strlen(RADIX_FIXORUM), linea.datum,
            (size_t)linea.mensura);
        via[strlen(RADIX_FIXORUM) + (size_t)linea.mensura] = '\0';
        nomen_fasciculi = strrchr(via, '/') + I;
        nostra = _tabula_cum_vitiis(c->piscina, via);
        numerus++;
        si (directorium != NIHIL)
        {
            sprintf(aurum_via, "%s/%s.tsv", directorium,
                nomen_fasciculi);
            CREDO_VERUM (filum_scribere(aurum_via, nostra));
            perge;
        }
        sprintf(aurum_via, RADIX_FIXORUM "aurum/%s.tsv",
            nomen_fasciculi);
        {
            chorda aurum = _sine_commentis(c->piscina,
                               filum_legere_totum(aurum_via,
                               c->piscina));
            chorda nos = _sine_commentis(c->piscina, nostra);
               b32 par = chorda_aequalis(aurum, nos);

            si (!par)
            {
                discordes++;
                si (discordes <= X)
                {
                    _discrimen_monstrare(nomen_fasciculi, aurum, nos);
                }
            }
            CREDO_VERUM (par);
        }
    }
    imprimere("  musicxml suita: %u fontes, %u discordes%s\n",
        (insignatus integer)numerus, (insignatus integer)discordes,
        directorium
            != NIHIL ? " (tabulae scriptae, nihil collatum)" : "");
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "suita et fixa contra aurum", _suitam_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("musicxml_suite", SECTIONES, NIHIL,
        NIHIL);
}
