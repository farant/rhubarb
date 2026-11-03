/* probatio_xml_stml.c - Circuitus STML xml: duo cycli, duo oracula
 *
 * scribere -> legere -> scribere: octeti STML bis idem (vitium quod se
 * componit circuitum unum saepe superat - stml semantica, lex M); arbor
 * parsata contra relectam per COMPARATOREM (materia_arbor_aequalis,
 * STRUCTURALIS - dislocatio dominii triviorum octetim invisibilis est,
 * solus comparator eam videt; deinde FIDELITAS: positiones); arbor
 * RELECTA per scriptorem octetorum emissa == fons (catena clausa);
 * sedes verificatae (materia_sedes_verificare). Casus: XML_CASUS
 * omnes, deinde corpora tota (suite W3C - casus selecti omnes, bene
 * formati aut non - et silvestria). Plagulae supra XML_MAXIMA_STML
 * OMITTUNTUR CUM NUMERO (nominatim): proiectio STML pretio substrati
 * (B10: ~XXX x parsationis) tectum sectionis excederet.
 *
 * Recusatio scriptoris causa nominata limes est (pinnandus), non
 * fractura; causa ALIA fractura est. Pinna separationis oraculorum:
 * cauda documenti relecti sublata -> octeti bis idem, arbor dispar.
 */

#include "latina.h"
#include "credo.h"
#include "xml_arbor.h"
#include "xml_lexicon.h"
#include "xml_registrum.h"
#include "xml_corpus_ambulare.h"
#include "materia_arbor.h"
#include "materia_sedes.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "piscina.h"
#include "lectiones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enumeratio {
    CIRCUITUS_IDEM = 0,
    CIRCUITUS_PARSATOR_NIHIL,
    CIRCUITUS_SCRIPTURA_RECUSATA,
    CIRCUITUS_LECTIO_RECUSATA,
    CIRCUITUS_RESCRIPTURA_RECUSATA,
    CIRCUITUS_OCTETI_DISPARES,
    CIRCUITUS_RELECTA_DISPAR_FONTI,
    CIRCUITUS_ARBOR_DISPAR,
    CIRCUITUS_ARBOR_INFIDELIS,
    CIRCUITUS_SEDES_INSANAE,
    CIRCUITUS_NUMERUS_CAUSARUM
};

hic_manens constans character* CAUSAE[] = {
    "idem",
    "parsator NIHIL reddidit",
    "scriptura STML recusata",
    "lectio STML recusata",
    "rescriptura STML recusata",
    "octeti STML dispares inter cyclos",
    "arbor relecta fontem non emittit",
    "arbor relecta dispar (STRUCTURALIS)",
    "arbor relecta infidelis (FIDELITAS: positiones)",
    "sedes non verificatae"
};

nomen structura {
                    integer  causa;
         constans character* nuntius;
    MateriaArborDifferentia  differentia;
                        i32  octeti_stml;
} Circuitus;

nomen structura {
    i32 plagulae;
    i32 octeti;
    i32 octeti_stml;
    i32 per_causam[CIRCUITUS_NUMERUS_CAUSARUM];
} Summa;

/* Duo cycli + comparator + emissio relectae. 'mutare': cauda documenti
 * relecti ANTE comparationem sublata (oraculum separans). */
interior Circuitus
_circuitum_probare (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               s32  mensura,
                               b32  mutare)
{
                Circuitus c;
             MateriaNodus* radix;
             MateriaNodus* relecta;
    MateriaArborScriptura  s1;
    MateriaArborScriptura  s2;
       MateriaArborVitium  vitium;
      MateriaSedesRelatio  sedes;
               XmlParsura  r;

    memset(&c, ZEPHYRUM, magnitudo(Circuitus));
    c.causa  = CIRCUITUS_IDEM;
    radix    = xml_arbor_parsare(piscina, fons, mensura, &r);
    si (radix == NIHIL)
    {
        c.causa = CIRCUITUS_PARSATOR_NIHIL;
        redde c;
    }
    s1 = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s1.successus)
    {
        c.causa    = CIRCUITUS_SCRIPTURA_RECUSATA;
        c.nuntius  = s1.causa;
        redde c;
    }
    c.octeti_stml = s1.textus.mensura;
    si (   !materia_sedes_verificare(piscina, radix, consilium, fons,
            (i32)mensura, &sedes) || !sedes.sana)
    {
        c.causa    = CIRCUITUS_SEDES_INSANAE;
        c.nuntius  = sedes.causa;
        redde c;
    }
    relecta = materia_arbor_legere(piscina, NIHIL, s1.textus,
        consilium,
        &vitium);
    si (relecta == NIHIL)
    {
        c.causa    = CIRCUITUS_LECTIO_RECUSATA;
        c.nuntius  = vitium.causa;
        redde c;
    }
    s2 = materia_arbor_scribere_nodum(piscina, relecta, consilium);
    si (!s2.successus)
    {
        c.causa    = CIRCUITUS_RESCRIPTURA_RECUSATA;
        c.nuntius  = s2.causa;
        redde c;
    }
    si (   s1.textus.mensura != s2.textus.mensura
        || memcmp(s1.textus.datum, s2.textus.datum,
               (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        c.causa = CIRCUITUS_OCTETI_DISPARES;
        redde c;
    }
    {
        MateriaScripturaConsilium cs;
                 MateriaScriptura emissa;

        materia_scriptura_consilium_nudum(&cs, &XML_REGISTRUM);
        emissa = materia_scribere_nodum(piscina, relecta, &cs);
        si (   !emissa.successus
            || emissa.textus.mensura != (i32)mensura
            || (mensura > ZEPHYRUM
                && memcmp(emissa.textus.datum, fons,
                       (size_t)mensura) != ZEPHYRUM))
        {
            c.causa = CIRCUITUS_RELECTA_DISPAR_FONTI;
            redde c;
        }
    }
    si (mutare)
    {
        relecta->loci[XML_DOCUMENTUM_CAUDA] = materia_valor_nihil();
    }
    si (!materia_arbor_aequalis(radix, relecta,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &c.differentia))
    {
        c.causa = CIRCUITUS_ARBOR_DISPAR;
        redde c;
    }
    si (!materia_arbor_aequalis(radix, relecta,
            MATERIA_ARBOR_COMPARATIO_FIDELITAS, &c.differentia))
    {
        c.causa = CIRCUITUS_ARBOR_INFIDELIS;
        redde c;
    }
    redde c;
}

interior vacuum
_causam_imprimere (
    constans character* titulus,
    constans Circuitus* c)
{
    si (c->causa == CIRCUITUS_IDEM)
    {
        redde;
    }
    imprimere("    %s: %s", titulus, CAUSAE[c->causa]);
    si (c->nuntius != NIHIL)
    {
        imprimere(" - %s", c->nuntius);
    }
    si (c->differentia.campus != NIHIL)
    {
        imprimere(" - campus %s via %s", c->differentia.campus,
            c->differentia.via);
    }
    imprimere("\n");
}

/* nominati: causae impressae (ad X); maximae: omissae cum numero;
 * consilium ex fixura suitae */
nomen structura {
                             Summa  fontes[XML_CORPUS_NUMERUS_FONTIUM];
                               i32  nominati;
                               i32  maximae;
    constans MateriaArborConsilium* consilium;
} Status;

/* plagulae maiores omittuntur cum numero: STML ~XXX x parsationis;
 * CXXVIII K = XCVII% silvestrium (7,692 ex 7,931), lex octetorum
 * omnes legit */
#define XML_MAXIMA_STML ((i32)CXXVIII * MXXIV)

interior vacuum
_visor (
                    vacuum* datum,
              XmlCorpusFons fons,
        constans character* via,
    constans XmlCasusSelectus* selectus,
                     chorda textus,
                   Piscina* opus)
{
                Status* st  = (Status*)datum;
                 Summa* s   = &st->fontes[fons];
             Circuitus  c;
    constans character* f = textus.datum != NIHIL
        ? (constans character*)textus.datum : "";

    (vacuum)selectus;
    si (   fons == XML_CORPUS_SILVESTRIA
        && textus.mensura > XML_MAXIMA_STML)
    {
        st->maximae++;
        redde;
    }
    c = _circuitum_probare(opus, st->consilium, f,
        (s32)textus.mensura, FALSUM);
    s->plagulae++;
    s->octeti       += textus.mensura;
    s->octeti_stml  += c.octeti_stml;
    s->per_causam[c.causa]++;
    si (c.causa != CIRCUITUS_IDEM && st->nominati < X)
    {
        _causam_imprimere(via, &c);
        st->nominati++;
    }
}

/* FIXURA SUITAE: lexicon et consilium arboris semel facta;
 * sectiones ea per monstratorem constantem LEGUNT. Piscina 'opus'
 * cuiusque sectionis propria. */
nomen structura {
      MateriaLexiconRatum ratum;
    MateriaArborConsilium consilium;
} FixuraStml;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
            FixuraStml* fx;
    MateriaLexIudicium  iudicium;

    fx = (FixuraStml*)piscina_allocare(c->piscina,
        magnitudo(FixuraStml));
    si (fx == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    CREDO_VERUM (materia_lexicon_ratum_facere(&fx->ratum, &XML_LEXICON,
        &iudicium));
    materia_arbor_consilium_nudum(&fx->consilium, &XML_REGISTRUM,
        &fx->ratum, "xml");
    c->datum = fx;
}


/* ==================================================
 * PORTA: casus inlinei
 * ================================================== */

interior vacuum
_casus_probare (
    CredoContextus* c)
{
    constans FixuraStml* fx = (constans FixuraStml*)c->suita;
                Piscina* opus;

    si (fx == NIHIL)
    {
        redde;
    }
    opus = piscina_generare_dynamicum("probatio_xml_stml_opus",
        4194304);
    si (opus == NIHIL)
    {
        redde;
    }
    {
        i32 k;
        i32 n = xml_casus_numerus();

        imprimere("  casus %d\n", (integer)n);
        per (k = ZEPHYRUM; k < n; k++)
        {
                  i32  l = (i32)strlen(XML_CASUS[k]);
            character* f = (character*)piscina_allocare(opus,
                (i64)(l + I));
            Circuitus circuitus;

            memcpy(f, XML_CASUS[k], (size_t)(l + I));
            circuitus = _circuitum_probare(opus, &fx->consilium, f,
                (s32)l,
                FALSUM);
            _causam_imprimere(XML_CASUS[k], &circuitus);
            CREDO_AEQUALIS_S32 ((s32)circuitus.causa,
                (s32)CIRCUITUS_IDEM);
            piscina_vacare(opus);
        }
    }
    piscina_destruere(opus);
}


/* ==================================================
 * PORTA: corpora tota
 * ================================================== */

interior vacuum
_corpora_probare (
    CredoContextus* c)
{
    constans FixuraStml* fx = (constans FixuraStml*)c->suita;
                Piscina* opus;
     constans character* radix = lectiones_ambitus("RHUBARB_RADIX");

    si (fx == NIHIL)
    {
        redde;
    }
    opus = piscina_generare_dynamicum("probatio_xml_stml_opus",
        4194304);
    si (opus == NIHIL)
    {
        redde;
    }
    {
                    Status  st;
           XmlCorpusNumeri  nn;
                       i32  f;
        constans character* TITULI[] = { "xmlts", "silvestria" };

        memset(&st, ZEPHYRUM, magnitudo(st));
        st.consilium = &fx->consilium;
        xml_corpus_ambulare(c->piscina, opus, radix != NIHIL ? radix
            : ".", (i32)((I << XML_CORPUS_XMLTS)
                | (I << XML_CORPUS_SILVESTRIA)), _visor, &st, &nn);
        si (nn.xmlts_absens)
        {
            imprimere("  OMISSA: suite W3C abest"
                " (./xml/xmlts_arcessere.sh)\n");
        }
        per (f = ZEPHYRUM; f < (i32)XML_CORPUS_NUMERUS_FONTIUM; f++)
        {
            Summa* s = &st.fontes[f];

            imprimere("  %s: %u plagulae, %u octeti -> %u STML, "
                "idem %u\n",
                TITULI[f], s->plagulae, s->octeti, s->octeti_stml,
                s->per_causam[CIRCUITUS_IDEM]);
            CREDO_AEQUALIS_I32 (s->per_causam[CIRCUITUS_IDEM],
                s->plagulae);
        }
        imprimere("  maximae omissae (> %u octeti): %u\n",
            (insignatus integer)XML_MAXIMA_STML,
            (insignatus integer)st.maximae);
        si (!nn.xmlts_absens)
        {
            CREDO_AEQUALIS_I32 (st.fontes[XML_CORPUS_XMLTS].plagulae,
                (i32)MMI);
        }
    }
    piscina_destruere(opus);
}


/* ==================================================
 * PINNA SEPARATIONIS ORACULORUM: octeti idem, arbor dispar
 * ================================================== */

interior vacuum
_separationem_probare (
    CredoContextus* c)
{
    constans FixuraStml* fx = (constans FixuraStml*)c->suita;
                Piscina* opus;

    si (fx == NIHIL)
    {
        redde;
    }
    opus = piscina_generare_dynamicum("probatio_xml_stml_opus",
        4194304);
    si (opus == NIHIL)
    {
        redde;
    }
    {
        character* f = (character*)piscina_allocare(opus, (i64)VIII);
        Circuitus  circuitus;

        memcpy(f, "<a/>\n", VI);
        circuitus = _circuitum_probare(opus, &fx->consilium, f, (s32)V,
            VERUM);
        CREDO_AEQUALIS_S32 ((s32)circuitus.causa,
            (s32)CIRCUITUS_ARBOR_DISPAR);
        CREDO_NON_NIHIL (circuitus.differentia.campus);
    }
    piscina_destruere(opus);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "circuitus STML: casus", _casus_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "circuitus STML: corpora", _corpora_probare, NIHIL, NIHIL,
        "gravis lentus", NIHIL },
    { "separatio oraculorum", _separationem_probare, NIHIL, NIHIL,
        NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_stml", SECTIONES,
        _suitam_parare, NIHIL);
}
