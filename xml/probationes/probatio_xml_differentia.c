/* probatio_xml_differentia.c - Differentia contra expat et libxml2
 * super corpus silvestre (planum xml-arbor P12)
 *
 * Aurum: xml/build/aurum_silvestre.tsv (actio fabricae
 * xml_aurum_silvestre, ex plagulis vivis regeneratum; numquam
 * commissum). Pro omni plagula silvestri: verdictum nostrum (spatiis
 * solutis) contra expat; si ambo accipiunt, sigillum formae canonicae
 * (xml_oraculum) contra sigillum expat. libxml2: concordia numerata,
 * non pinnata (errores spatiorum non fatales, PROVENIENTIA.md).
 *
 * DIVERGENTIAE NOMINATAE per CAUSAM (non per viam):
 *   codificatio   nos recusamus (X8: codificatio non UTF-8 aut UTF-16),
 *                 expat accipit (ISO-8859-1 et ceterae)
 *   libxml2       expat a nobis dissentit, libxml2 nobiscum consentit
 *                 (verdictum): ubi oracula inter se dissentiunt, textus
 *                 specificationis decernit (PROVENIENTIA.md) - e.g.
 *                 expat sine entitatibus parametri (NEVER) recursionem
 *                 in entitate parametri interna non videt
 * Ceterae discordiae (ambo oracula contra nos) imprimuntur et
 * PINNANTUR ad numerum (solum decrescit). Plagulae supra
 * XML_MAXIMA_DIFFERENTIAE omittuntur cum numero (nominatim; arbor ~LX
 * x fontis, P5).
 */

#include "latina.h"
#include "credo.h"
#include "xml_coctum.h"
#include "xml_oraculum.h"
#include "xml_corpus_ambulare.h"
#include "materia_diagnosticum.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include "sigillum.h"
#include "filum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

#define XML_MAXIMA_DIFFERENTIAE ((i32)XII * MXXIV * MXXIV)

/* PINNA (solum decrescit): discordiae non nominatae */
#define PINNA_DISCORDIAE  ZEPHYRUM

nomen structura {
    character expat;      /* 'V' aut 'I' */
    character libxml2;
    character canon[SIGILLUM_HEX_MENSURA];
} Aurum;

nomen structura {
    TabulaDispersa* aurum;
               i32  plagulae;
               i32  sine_auro;
               i32  maximae;
               i32  concordes;
               i32  codificatio;
               i32  libxml2_nobiscum;
               i32  discordes;
               i32  libxml2_concordes;
               i32  impressa;
} Status;

/* aurum in tabulam (clavis: sigillum plagulae hex) */
interior TabulaDispersa*
_aurum_legere (
    Piscina* piscina)
{
         character  via[CCLVI];
            chorda  textus;
               i32  k = ZEPHYRUM;
    TabulaDispersa* t = tabula_dispersa_creare_chorda(piscina,
        MMXLVIII);

    sprintf(via, "%s/xml/build/aurum_silvestre.tsv",
        xml_corpus_radix());
    textus = filum_legere_totum(via, piscina);
    si (t == NIHIL || textus.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    dum (k < textus.mensura)
    {
        i32 initium = k;
        i32 finis;

        dum (k < textus.mensura && textus.datum[k] != '\n')
        {
            k++;
        }
        finis = k;
        k++;
        /* sha(64) \t V \t canon(64 aut -) \t V */
        si (   finis - initium              >= (i32)LXIX
            && textus.datum[initium]        != '#'
            && textus.datum[initium + LXIV] == '\t')
        {
            Aurum* a = (Aurum*)piscina_allocare(piscina,
                (memoriae_index)magnitudo(Aurum));
            chorda clavis;
               i32 j = initium + LXV;

            si (a == NIHIL)
            {
                redde NIHIL;
            }
            memset(a, ZEPHYRUM, magnitudo(Aurum));
            a->expat  = (character)textus.datum[j];
            j         += II;
            si (textus.datum[j] != '-')
            {
                memcpy(a->canon, textus.datum + j, (size_t)LXIV);
                j += LXIV;
            }
            alioquin
            {
                j++;
            }
            j++;
            a->libxml2 = j < finis ? (character)textus.datum[j] : '?';
            clavis.datum = textus.datum + initium;
            clavis.mensura = LXIV;
            (vacuum)tabula_dispersa_inserere(t, clavis, a);
        }
    }
    redde t;
}

interior b32
_codificatio (
    constans XmlCoctum* k)
{
    i32 i;

    per (i = ZEPHYRUM; k->diagnostica != NIHIL
         && i < xar_numerus(k->diagnostica); i++)
    {
        constans MateriaDiagnosticum* d =
            (constans MateriaDiagnosticum*)
            xar_obtinere(k->diagnostica, i);

        si (   strcmp(d->codex, XML_CODEX_CODIFICATIO) == ZEPHYRUM
            || strcmp(d->codex, XML_CODEX_UTF8)        == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_visor (
                    vacuum* datum,
              XmlCorpusFons fons,
        constans character* via,
    constans XmlCasusSelectus* s,
                     chorda textus,
                   Piscina* opus)
{
       Status* st = (Status*)datum;
     Sigillum  sg;
    character  hex[SIGILLUM_HEX_MENSURA];
       chorda  clavis;
       vacuum* v = NIHIL;
        Aurum* a;
    XmlCoctum  k;
          b32  nos;

    (vacuum)fons;
    (vacuum)s;
    si (textus.mensura > XML_MAXIMA_DIFFERENTIAE)
    {
        st->maximae++;
        imprimere("    MAXIMA OMISSA: %s\n", via);
        redde;
    }
    sg = sigillum_computare(textus.datum,
        (memoriae_index)textus.mensura);
    sigillum_hex(&sg, hex);
    clavis.datum    = (i8*)hex;
    clavis.mensura  = LXIV;
    si (!tabula_dispersa_invenire(st->aurum, clavis, &v))
    {
        st->sine_auro++;
        redde;
    }
    a = (Aurum*)v;
    st->plagulae++;
    k = xml_coquere(opus, (constans character*)textus.datum,
        (s32)textus.mensura, VERUM);
    nos = k.bene_formatum;
    si (nos == (a->libxml2 == 'V'))
    {
        st->libxml2_concordes++;
    }
    si (nos && a->expat == 'V')
    {
        ChordaAedificator* b = chorda_aedificator_creare(opus,
                                   (memoriae_index)textus.mensura
                                       + LXIV);
                   chorda c;

        si (b == NIHIL || !xml_oraculum_scribere(&k, opus, b))
        {
            st->discordes++;
            redde;
        }
        c   = chorda_aedificator_finire(b);
        sg  = sigillum_computare(c.datum, (memoriae_index)c.mensura);
        sigillum_hex(&sg, hex);
        si (memcmp(hex, a->canon, (size_t)LXIV) == ZEPHYRUM)
        {
            st->concordes++;
            redde;
        }
        st->discordes++;
        si (st->impressa++ < (i32)XXX)
        {
            imprimere("    CANONICUM DISPAR: %s\n", via);
        }
        redde;
    }
    si (!nos && a->expat != 'V')
    {
        st->concordes++;
        redde;
    }
    si (!nos && _codificatio(&k))
    {
        st->codificatio++;
        redde;
    }
    si (nos == (a->libxml2 == 'V'))
    {
        st->libxml2_nobiscum++;
        si (st->impressa++ < (i32)XXX)
        {
            imprimere("    LIBXML2 NOBISCUM (%s): %s\n", nos ? "accepta"
                : "recusata", via);
        }
        redde;
    }
    st->discordes++;
    si (st->impressa++ < (i32)XXX)
    {
        imprimere("    %s: %s", nos ? "NOS ACCIPIMUS, EXPAT RECUSAT"
            : "NOS RECUSAMUS, EXPAT ACCIPIT", via);
        si (   !nos && k.diagnostica != NIHIL
            && xar_numerus(k.diagnostica) > ZEPHYRUM)
        {
            constans MateriaDiagnosticum* d = (constans
                MateriaDiagnosticum*)xar_obtinere(k.diagnostica,
                ZEPHYRUM);

            imprimere(" [%s @%d: %s]", d->codex,
                (integer)d->tractus.initium,
                d->causa != NIHIL ? d->causa : "");
        }
        imprimere("\n");
    }
}

interior vacuum
_differentiam_probare (
    CredoContextus* c)
{
             Status  st;
    XmlCorpusNumeri  nn;
            Piscina* opus;

    memset(&st, ZEPHYRUM, magnitudo(st));
    st.aurum = _aurum_legere(c->piscina);
    si (st.aurum == NIHIL)
    {
        CREDO_CULPA ("aurum silvestre abest (xml_aurum_silvestre)");
        redde;
    }
    opus = piscina_generare_dynamicum("xml_differentia_opus",
        (memoriae_index)(I * MXXIV * MXXIV));
    CREDO_NON_NIHIL (opus);
    si (opus == NIHIL)
    {
        redde;
    }
    xml_corpus_ambulare(c->piscina, opus, xml_corpus_radix(),
        (i32)(I << XML_CORPUS_SILVESTRIA), _visor, &st, &nn);
    imprimere("  silvestria %u (omissae %u, sine auro %u, maximae %u):"
        " concordes %u, codificatio %u, libxml2 nobiscum %u, discordes"
        " %u; libxml2 concordes %u\n", (insignatus integer)st.plagulae,
        (insignatus integer)nn.omissae,
        (insignatus integer)st.sine_auro,
        (insignatus integer)st.maximae,
        (insignatus integer)st.concordes,
        (insignatus integer)st.codificatio,
        (insignatus integer)st.libxml2_nobiscum,
        (insignatus integer)st.discordes,
        (insignatus integer)st.libxml2_concordes);
    CREDO_MINOR_I32 (st.discordes, (i32)PINNA_DISCORDIAE + I);
    piscina_destruere(opus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "differentia contra expat super silvestria",
        _differentiam_probare,
        NIHIL, NIHIL, "gravis lentus", NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_differentia", SECTIONES, NIHIL,
        NIHIL);
}
