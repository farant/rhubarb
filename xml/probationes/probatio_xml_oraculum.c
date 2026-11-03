/* probatio_xml_oraculum.c - Oraculum W3C: sententiae et exitus
 *
 * Pro omni casu selectionis (xml/probationes/fixa/xmlts/selectio.tsv)
 * non exclusio (exclusio '-' aut 'exitus-externus') et genere non
 * 'error': not-wf RECUSARI debet, valid et invalid ACCIPI (spec X1,
 * processor non validans); casus cum exitu (exclusio '-') formam
 * canonicam (xml_oraculum) octetim aequare. Numeri per classem
 * imprimuntur et PINNANTUR (solum crescunt - PINNAE infra, cum causa
 * motae).
 *
 * Ambitus: XMLTS_OMNIA=1 omnes fracturas imprimit (aliter XX primae);
 * XMLTS_CASUS=<ID> casum unum currit et diagnostica eius imprimit.
 * Suite absente: OMITTITUR CUM NUMERO (./xml/xmlts_arcessere.sh).
 */

#include "latina.h"
#include "credo.h"
#include "xml_coctum.h"
#include "xml_oraculum.h"
#include "xml_corpus_ambulare.h"
#include "materia_diagnosticum.h"
#include "chorda_aedificator.h"
#include "lectiones.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

/* PINNAE (solum crescunt): recusata recte, accepta recte, exitus
 * aequales */
#define PINNA_RECUSATA  CMXXII     /* 2026-10-30: 922/922 */
#define PINNA_ACCEPTA   CMXLVIII   /* 948/948 (sine-spatiis incl.) */
#define PINNA_EXITUS    CCCIX      /* 309/309 */

nomen structura {
                   i32  recusanda;        /* not-wf iudicata */
                   i32  recusata;         /* recte */
                   i32  accipienda;       /* valid + invalid iudicata */
                   i32  accepta;          /* recte */
                   i32  exitus;           /* exitus iudicati */
                   i32  exitus_aequales;
                   i32  exclusa;
                   i32  errores;          /* genus 'error' */
                   i32  impressa;
                   b32  omnia;
    constans character* casus_unus;
} Status;

interior vacuum
_primum_imprimere (
    constans XmlCoctum* k)
{
    si (   k->diagnostica != NIHIL
        && xar_numerus(k->diagnostica) > ZEPHYRUM)
    {
        constans MateriaDiagnosticum* d =
            (constans MateriaDiagnosticum*)
            xar_obtinere(k->diagnostica, ZEPHYRUM);

        imprimere(" [%s @%d: %s]", d->codex,
            (integer)d->tractus.initium,
            d->causa != NIHIL ? d->causa : "");
    }
}

interior vacuum
_visor (
                       vacuum* datum,
                XmlCorpusFons  fons,
           constans character* via,
    constans XmlCasusSelectus* s,
                       chorda  textus,
                      Piscina* opus)
{
       Status* st = (Status*)datum;
    XmlCoctum  k;
          b32  debet_accipi;
          b32  imprimendum;

    (vacuum)fons;
    si (s == NIHIL)
    {
        redde;
    }
    si (   st->casus_unus                != NIHIL
        && strcmp(st->casus_unus, s->id) != ZEPHYRUM)
    {
        redde;
    }
    si (strcmp(s->genus, "error") == ZEPHYRUM)
    {
        st->errores++;
        redde;
    }
    /* 'sine-spatiis' iudicatur, spatiis non solutis (xml_coquere
     * spatia FALSUM); exclusae solum codificatio (X8) et
     * entitas-externa (X3) */
    si (   strcmp(s->exclusio, "-")               != ZEPHYRUM
        && strcmp(s->exclusio, "exitus-externus") != ZEPHYRUM
        && strcmp(s->exclusio, "sine-spatiis")    != ZEPHYRUM)
    {
        st->exclusa++;
        redde;
    }
    k = xml_coquere(opus, (constans character*)textus.datum,
        (s32)textus.mensura, strcmp(s->spatia, "no") != ZEPHYRUM);
    debet_accipi = strcmp(s->genus, "not-wf") != ZEPHYRUM;
    imprimendum = st->omnia || st->casus_unus != NIHIL
        || st->impressa < (i32)XX;
    si (debet_accipi)
    {
        st->accipienda++;
        si (k.bene_formatum)
        {
            st->accepta++;
        }
        alioquin si (imprimendum)
        {
            imprimere("    RECUSATUM (%s) %s %s", s->genus, s->id, via);
            _primum_imprimere(&k);
            imprimere("\n");
            st->impressa++;
        }
    }
    alioquin
    {
        st->recusanda++;
        si (!k.bene_formatum)
        {
            st->recusata++;
        }
        alioquin si (imprimendum)
        {
            imprimere("    ACCEPTUM (not-wf) %s %s\n", s->id, via);
            st->impressa++;
        }
    }
    si (   k.bene_formatum && debet_accipi && strcmp(s->exitus, "-")
        != ZEPHYRUM && (strcmp(s->exclusio, "-") == ZEPHYRUM
            || strcmp(s->exclusio, "sine-spatiis") == ZEPHYRUM))
    {
        ChordaAedificator* a = chorda_aedificator_creare(opus, CCLVI);
                   chorda  exspectatum = xml_corpus_xmlts_legere(opus,
                               xml_corpus_radix(), s->exitus);
                   chorda scriptum;

        st->exitus++;
        si (a != NIHIL && xml_oraculum_scribere(&k, opus, a))
        {
            scriptum = chorda_aedificator_finire(a);
            si (   scriptum.mensura == exspectatum.mensura
                && (scriptum.mensura == ZEPHYRUM
                    || memcmp(scriptum.datum, exspectatum.datum,
                           (size_t)scriptum.mensura) == ZEPHYRUM))
            {
                st->exitus_aequales++;
            }
            alioquin si (imprimendum)
            {
                imprimere("    EXITUS DISPAR %s\n"
                    "      scriptum    '%.*s'\n"
                    "      exspectatum '%.*s'\n", s->id,
                    (integer)(scriptum.mensura < CC ? scriptum.mensura
                        : CC), (constans character*)scriptum.datum,
                    (integer)(exspectatum.mensura < CC
                        ? exspectatum.mensura : CC),
                    (constans character*)exspectatum.datum);
                st->impressa++;
            }
        }
    }
    si (st->casus_unus != NIHIL && k.diagnostica != NIHIL)
    {
        i32 j;

        per (j = ZEPHYRUM; j < xar_numerus(k.diagnostica); j++)
        {
            constans MateriaDiagnosticum* d = (constans
                MateriaDiagnosticum*)xar_obtinere(k.diagnostica, j);

            imprimere("      diagnosticum %s @%d: %s\n", d->codex,
                (integer)d->tractus.initium, d->causa
                    != NIHIL ? d->causa
                    : "");
        }
    }
}

interior vacuum
_oraculum_probare (
    CredoContextus* c)
{
             Status  st;
    XmlCorpusNumeri  nn;
            Piscina* opus;

    memset(&st, ZEPHYRUM, magnitudo(st));
    st.omnia       = lectiones_ambitus("XMLTS_OMNIA") != NIHIL;
    st.casus_unus  = lectiones_ambitus("XMLTS_CASUS");
    opus = piscina_generare_dynamicum("xml_oraculum_opus",
        (memoriae_index)(I * MXXIV * MXXIV));
    CREDO_NON_NIHIL (opus);
    si (opus == NIHIL)
    {
        redde;
    }
    xml_corpus_ambulare(c->piscina, opus, xml_corpus_radix(),
        (i32)(I << XML_CORPUS_XMLTS), _visor, &st, &nn);
    si (nn.xmlts_absens)
    {
        imprimere("  OMISSA: suite W3C abest"
            " (./xml/xmlts_arcessere.sh)\n");
        piscina_destruere(opus);
        redde;
    }
    imprimere("  not-wf recusata %u/%u, valid+invalid accepta %u/%u,"
        " exitus aequales %u/%u (exclusa %u, error %u)\n",
        (insignatus integer)st.recusata,
        (insignatus integer)st.recusanda,
        (insignatus integer)st.accepta,
        (insignatus integer)st.accipienda,
        (insignatus integer)st.exitus_aequales,
        (insignatus integer)st.exitus, (insignatus integer)st.exclusa,
        (insignatus integer)st.errores);
    si (st.casus_unus == NIHIL)
    {
        CREDO_MAIOR_I32 (st.recusata + I, (i32)PINNA_RECUSATA);
        CREDO_MAIOR_I32 (st.accepta + I, (i32)PINNA_ACCEPTA);
        CREDO_MAIOR_I32 (st.exitus_aequales + I, (i32)PINNA_EXITUS);
    }
    piscina_destruere(opus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "oraculum W3C: sententiae et exitus", _oraculum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_oraculum", SECTIONES, NIHIL,
        NIHIL);
}
