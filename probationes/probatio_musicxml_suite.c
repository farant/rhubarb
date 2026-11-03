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
 *
 * CORPUS SILVESTRE (plan P8): MUSICXML_SILVESTRIA=<directorium> (ubi
 * ./tools/musicxml_arcessere.sh -silvestria fasciculos posuit) sectio
 * secunda currit - porta 'musicxml-oraculum' (tools/musicxml_silvestria.sh),
 * numquam porta radix (fasciculi non commissi). Per fasciculum
 * manifesti: sigillum fasciculi primum (mutatum = FRACTUM nominatum,
 * non regressio lectoris), deinde aurum plenum
 * (aurum_silvestre/<nomen>.tsv) aut sigillum tabulae nostrae
 * (silvestria.sigilla: linea una per lineam manifesti, eodem ordine).
 * Cum MUSICXML_TABULAS_SCRIBERE tabulas et silvestria.sigilla ibi
 * scribit.
 */

#include "latina.h"
#include "credo.h"
#include "musicxml.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "lectiones.h"
#include "piscina.h"
#include "sigillum.h"
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
_tabula_ex_datis (
     Piscina* p,
      chorda  datum)
{
         MusicxmlOpus* o;
         MusicxmlOrdo* r;
    ChordaAedificator* a = chorda_aedificator_creare(p, MMMCMXCIX);

    o = musicxml_legere(datum, p);
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
                   Piscina* q;
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
        numerus++;
        /* piscina per fontem: memoria fontis post eum redditur */
        q = piscina_generare_dynamicum("musicxml_fons",
                (memoriae_index)(LXIV * M));
        CREDO_NECESSE (q != NIHIL);
        nostra = _tabula_ex_datis(q, filum_legere_totum(via, q));
        si (directorium != NIHIL)
        {
            sprintf(aurum_via, "%s/%s.tsv", directorium,
                nomen_fasciculi);
            CREDO_VERUM (filum_scribere(aurum_via, nostra));
        }
        alioquin
        {
            chorda aurum;
            chorda nos;
               b32 par;

            sprintf(aurum_via, RADIX_FIXORUM "aurum/%s.tsv",
                nomen_fasciculi);
            aurum = _sine_commentis(q, filum_legere_totum(aurum_via,
                q));
            nos = _sine_commentis(q, nostra);
            par = chorda_aequalis(aurum, nos);
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
        piscina_destruere(q);
    }
    imprimere("  musicxml suita: %u fontes, %u discordes%s\n",
        (insignatus integer)numerus, (insignatus integer)discordes,
        directorium
            != NIHIL ? " (tabulae scriptae, nihil collatum)" : "");
}

/* linea proxima textus ab *i (sine '\n'); FALSUM in fine */
interior b32
_linea_proxima (
     chorda  t,
        i32* i,
     chorda* linea)
{
    i32 j = *i;

    si (*i >= t.mensura)
    {
        redde FALSUM;
    }
    dum (j < t.mensura && t.datum[j] != '\n')
    {
        j++;
    }
    *linea  = chorda_sectio(t, *i, j);
    *i      = j + I;
    redde VERUM;
}

interior vacuum
_hex (
     chorda  datum,
  character* exitus)
{
    Sigillum g = sigillum_computare(datum.datum,
                     (memoriae_index)datum.mensura);

    sigillum_hex(&g, exitus);
}

nomen enumeratio {
    FONS_ABSENS,
    FONS_MUTATUS,
    FONS_SCRIPTUS,
    FONS_PAR,
    FONS_PAR_AURO,
    FONS_DISCORS,
    FONS_DISCORS_AURO
} StatusFontis;

/* fons unus corporis silvestris, in piscina q (a vocante destructa);
 * hex fasciculi et tabulae in hex_tabulae redditur (scribendo) */
interior StatusFontis
_fontem_silvestrem (
                Piscina* q,
     constans character* via,
     constans character* nomen_fasciculi,
                 chorda  linea,
                    b32  pinum_adest,
                 chorda  pinum,
                 chorda  via_relativa,
     constans character* directorium,
              character* hex_tabulae,
                    i32  fracti)
{
    character hex[SIGILLUM_HEX_MENSURA + I];
    character aurum_via[MXXIV];
       chorda datum = filum_legere_totum(via, q);
       chorda nostra;

    si (datum.mensura == ZEPHYRUM)
    {
        redde FONS_ABSENS;
    }
    _hex(datum, hex);
    si (memcmp(hex, linea.datum, (size_t)LXIV) != ZEPHYRUM)
    {
        imprimere("  FASCICULUS MUTATUS (non regressio lectoris): %s\n",
            via);
        redde FONS_MUTATUS;
    }
    nostra = _tabula_ex_datis(q, datum);
    _hex(nostra, hex_tabulae);
    si (directorium != NIHIL)
    {
        sprintf(aurum_via, "%s/%s.tsv", directorium, nomen_fasciculi);
        redde filum_scribere(aurum_via, nostra) ? FONS_SCRIPTUS
                                                : FONS_DISCORS;
    }
    sprintf(aurum_via, RADIX_FIXORUM "aurum_silvestre/%s.tsv",
        nomen_fasciculi);
    si (filum_existit(aurum_via))
    {
        chorda aurum = _sine_commentis(q, filum_legere_totum(aurum_via,
            q));
        chorda nos = _sine_commentis(q, nostra);

        si (chorda_aequalis(aurum, nos))
        {
            redde FONS_PAR_AURO;
        }
        si (fracti < X)
        {
            _discrimen_monstrare(nomen_fasciculi, aurum, nos);
        }
        redde FONS_DISCORS_AURO;
    }
    si (   pinum_adest && pinum.mensura == LXVI + via_relativa.mensura
        && chorda_aequalis(chorda_sectio(pinum, LXVI, pinum.mensura),
        via_relativa)
        && memcmp(pinum.datum, hex_tabulae, (size_t)LXIV) == ZEPHYRUM)
    {
        redde FONS_PAR;
    }
    si (fracti < X)
    {
        imprimere("  SIGILLUM DISCORS: %s\n", nomen_fasciculi);
    }
    redde FONS_DISCORS;
}

/* manifestum et sigilla: numerus linearum idem, via eadem, sigilla
 * LXIV characteres hexadecimales */
interior b32
_data_congruunt (
    chorda manifestum,
    chorda sigilla)
{
       i32 i = ZEPHYRUM;
       i32 s = ZEPHYRUM;
       i32 k;
    chorda a;
    chorda b;

    si (   !_linea_proxima(manifestum, &i, &a)
        || !_linea_proxima(sigilla, &s, &b))
    {
        redde FALSUM;
    }
    dum (_linea_proxima(manifestum, &i, &a))
    {
        si (   !_linea_proxima(sigilla, &s, &b)
            || a.mensura != b.mensura
            || a.mensura < LXVI
            || !chorda_aequalis(chorda_sectio(a, LXIV, a.mensura),
                   chorda_sectio(b, LXIV, b.mensura)))
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < LXIV; k++)
        {
            character x = (character)b.datum[k];

            si (!((x >= '0' && x <= '9') || (x >= 'a' && x <= 'f')))
            {
                redde FALSUM;
            }
        }
    }
    redde !_linea_proxima(sigilla, &s, &b);
}

interior vacuum
_silvestria_probare (
    CredoContextus* c)
{
    constans character* radix =
        lectiones_ambitus("MUSICXML_SILVESTRIA");
     constans character* directorium;
                 chorda  manifestum;
                 chorda  sigilla;
                 chorda  linea;
      ChordaAedificator* nova      = NIHIL;
                    i32  i         = ZEPHYRUM;
                    i32  s         = ZEPHYRUM;
                    i32  numerus   = ZEPHYRUM;
                    i32  absentes  = ZEPHYRUM;
                    i32  fracti    = ZEPHYRUM;
                    i32  aurea     = ZEPHYRUM;

    directorium = lectiones_ambitus("MUSICXML_TABULAS_SCRIBERE");
    manifestum =
        filum_legere_totum(RADIX_FIXORUM "silvestria.manifestum",
        c->piscina);
    sigilla = filum_legere_totum(RADIX_FIXORUM "silvestria.sigilla",
        c->piscina);
    CREDO_NECESSE (manifestum.mensura > ZEPHYRUM);
    /* data commissa semper (etiam porta radix, sine corpore): manifestum
     * et sigilla linea pro linea congruunt (via eadem, sigilla hex) */
    si (directorium == NIHIL)
    {
        CREDO_VERUM (_data_congruunt(manifestum, sigilla));
    }
    si (radix == NIHIL)
    {
        redde;
    }
    si (directorium != NIHIL)
    {
        nova = chorda_aedificator_creare(c->piscina, CCLVI * M);
        chorda_aedificator_appendere_literis(nova,
            "DATA musicxml silvestria sigilla 1 (sha256 tabulae nostrae "
            "cum vitiis; linea una per lineam manifesti)\n");
    }
    (vacuum)_linea_proxima(sigilla, &s, &linea);   /* DATA */
    dum (_linea_proxima(manifestum, &i, &linea))
    {
                 character  via[MXXIV];
                 character  hex[SIGILLUM_HEX_MENSURA + I];
                    chorda  via_relativa;
                    chorda  pinum;
                       b32  pinum_adest;
                   Piscina* q;
              StatusFontis  status;

        si (   linea.mensura < LXVI || linea.datum[LXIV] != ' '
            || linea.mensura > CM)
        {
            perge;   /* DATA, '#' */
        }
        via_relativa  = chorda_sectio(linea, LXVI,
            linea.mensura);
        pinum_adest   = _linea_proxima(sigilla, &s, &pinum);
        sprintf(via, "%s/", radix);
        memcpy(via + strlen(via), via_relativa.datum,
            (size_t)via_relativa.mensura);
        via[strlen(radix) + I + (size_t)via_relativa.mensura] = '\0';
        numerus++;
        /* piscina per fontem: memoria fontis post eum redditur (olim
         * omnes in piscina sectionis - memoria sine fine crevit) */
        q = piscina_generare_dynamicum("musicxml_silvestre",
                (memoriae_index)(LXIV * M));
        CREDO_NECESSE (q != NIHIL);
        hex[ZEPHYRUM] = '\0';
        status = _fontem_silvestrem(q, via, strrchr(via, '/') + I,
            linea,
                     pinum_adest, pinum, via_relativa, directorium, hex,
                     fracti);
        piscina_destruere(q);
        commutatio (status)
        {
            casus FONS_ABSENS:
                absentes++;
                frange;
            casus FONS_SCRIPTUS:
                chorda_aedificator_appendere_literis(nova, hex);
                chorda_aedificator_appendere_literis(nova, "  ");
                chorda_aedificator_appendere_chorda(nova, via_relativa);
                chorda_aedificator_appendere_character(nova, '\n');
                frange;
            casus FONS_PAR:
                frange;
            casus FONS_PAR_AURO:
                aurea++;
                frange;
            casus FONS_DISCORS_AURO:
                aurea++;
                fracti++;
                CREDO_VERUM (FALSUM);
                frange;
            ordinarius:
                fracti++;
                CREDO_VERUM (FALSUM);
                frange;
        }
    }
    si (absentes == numerus)
    {
        imprimere("MUSICXML SILVESTRIA: OMISSA: corpus abest in %s "
            "(./tools/musicxml_arcessere.sh -silvestria)\n", radix);
        redde;
    }
    si (nova != NIHIL)
    {
        character via[MXXIV];

        sprintf(via, "%s/silvestria.sigilla", directorium);
        CREDO_VERUM (filum_scribere(via,
            chorda_aedificator_finire(nova)));
        imprimere("MUSICXML SILVESTRIA: scriptae %u (absentes %u)\n",
            (insignatus integer)(numerus - absentes),
            (insignatus integer)absentes);
        redde;
    }
    CREDO_AEQUALIS_I32 (absentes, ZEPHYRUM);
    si (fracti == ZEPHYRUM && absentes == ZEPHYRUM)
    {
        imprimere("MUSICXML SILVESTRIA: sana %u/%u (aurea plena %u)\n",
            (insignatus integer)numerus, (insignatus integer)numerus,
            (insignatus integer)aurea);
    }
    alioquin
    {
        imprimere("MUSICXML SILVESTRIA: FRACTA %u ex %u (absentes %u)\n",
            (insignatus integer)fracti, (insignatus integer)numerus,
            (insignatus integer)absentes);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "suita et fixa contra aurum", _suitam_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "corpus silvestre (MUSICXML_SILVESTRIA)", _silvestria_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("musicxml_suite", SECTIONES, NIHIL,
        NIHIL);
}
