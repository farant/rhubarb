/* probatio_xml_computus.c - PORTA COMPUTI XML: numeri deterministici
 * corporis fixi (tres documenta inlinea, nostra) contra aurum pinnati
 * (nodi, lexemata, octeti STML, diagnostica, memoria piscinae parsurae
 * et coctionis, allocationes); tempora imprimuntur, non pinnantur.
 * Gemellus probatio_toml_computus (P14).
 *
 * Corpus INLINEUM (non suite W3C: licentia eam in repositorio vetat, et
 * aurum input CONGELATUM poscit): documentum partiturae simile
 * (MusicXML forma), documentum DTD grave (entitates, attributa
 * praestituta, notationes), documentum invalidum (lex octetorum etiam
 * ibi tenet).
 *
 * COMPUTUS_SCRIBERE=1 aurum rescribit - AUDI: regeneratio sine causa
 * nominata portam in numerum vacuum vertit. Causa in commissione.
 */
#include "latina.h"
#include "credo.h"
#include "piscina.h"
#include "xml_computus.h"
#include "filum.h"
#include "lectiones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA        DXII
#define COLUMNAE          XIII
#define NUMERUS_CORPORIS  III

hic_manens constans character* CORPUS[NUMERUS_CORPORIS] = {
    "partitura", "dtd", "invalidum"
};

hic_manens constans character* FONTES[NUMERUS_CORPORIS] = {
    "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
    "<!DOCTYPE score-partwise PUBLIC \"-//Recordare//DTD MusicXML 4.0"
    " Partwise//EN\" \"http://www.musicxml.org/dtds/partwise.dtd\">\n"
    "<score-partwise version=\"4.0\">\n"
    "  <part-list>\n    <score-part id=\"P1\"><part-name>S.</part-name>"
    "</score-part>\n  </part-list>\n"
    "  <part id=\"P1\">\n    <measure number=\"1\">\n"
    "      <attributes><divisions>480</divisions>"
    "<key><fifths>-2</fifths></key></attributes>\n"
    "      <note><pitch><step>D</step><octave>5</octave></pitch>"
    "<duration>480</duration><type>quarter</type>"
    "<lyric number=\"1\"><syllabic>single</syllabic><text>O</text>"
    "</lyric></note>\n"
    "      <note><pitch><step>E</step><alter>-1</alter>"
    "<octave>5</octave>"
    "</pitch><duration>480</duration><type>quarter</type></note>\n"
    "    </measure>\n  </part>\n</score-partwise>\n",
    "<!DOCTYPE doc [\n"
    "  <!ELEMENT doc (#PCDATA|a|b)*>\n"
    "  <!ATTLIST doc v CDATA 'praestitutum' t NMTOKENS #IMPLIED>\n"
    "  <!ENTITY e \"<a>entitas &#38;amp; textus</a>\">\n"
    "  <!ENTITY % p \"<!ENTITY q 'per parametrum'>\">\n"
    "  %p;\n"
    "  <!NOTATION n PUBLIC 'n' 'n.dtd'>\n"
    "  <?pi in dtd?>\n"
    "]>\n<doc t=' x  y '>&e;&q;<b/>&#x3C;&lt;<![CDATA[<c>]]></doc>\n",
    "<a><b attr=\"1\" attr=\"2\">&nulla;</c><!-- x -- y -->\n"
};

hic_manens constans character* TITULI[COLUMNAE] = {
    "octeti", "lexemata", "nodi", "octeti_stml", "diagnostica", "usus",
    "commissa", "otiosa", "apex", "alvei", "allocationes",
    "allocationes_arboris", "usus_coquendi"
};

/* lineam proximam (cum '\n', ut fgets) ex textu; FALSUM in fine */
interior b32
_lineam_legere (
     constans character** cursor,
              character*  linea,
                    i32   mensura)
{
    i32 n = ZEPHYRUM;

    si (**cursor == '\0')
    {
        redde FALSUM;
    }
    dum (**cursor != '\0' && n + I < mensura)
    {
        linea[n] = **cursor;
        n++;
        (*cursor)++;
        si (linea[n - I] == '\n')
        {
            frange;
        }
    }
    linea[n] = '\0';
    redde VERUM;
}

/* columnae pinnatae, ordine auri */
interior vacuum
_columnas (
     constans XmlComputus* c,
        insignatus longus  v[COLUMNAE])
{
    v[ZEPHYRUM]  = (insignatus longus)c->octeti_fontis;
    v[I]         = (insignatus longus)c->lexemata;
    v[II]        = (insignatus longus)c->nodi;
    v[III]       = (insignatus longus)c->octeti_stml;
    v[IV]        = (insignatus longus)c->diagnostica;
    v[V]         = (insignatus longus)c->usus;
    v[VI]        = (insignatus longus)c->commissa;
    v[VII]       = (insignatus longus)c->otiosa;
    v[VIII]      = (insignatus longus)c->apex;
    v[IX]        = (insignatus longus)c->alvei;
    v[X]         = (insignatus longus)c->allocationes;
    v[XI]        = (insignatus longus)c->allocationes_arboris;
    v[XII]       = (insignatus longus)c->usus_coquendi;
}

/* FIXURA SUITAE: corpus semel mensum (parare_suitae); sectiones
 * mensuras LEGUNT - proprietates (I) et collatio cum auro (II) sine
 * ordine inter se. */
nomen structura {
     constans character* radix;
              character  via_auri[VIA_MAXIMA];
            XmlComputus  computa[NUMERUS_CORPORIS];
      insignatus longus  facta[NUMERUS_CORPORIS][COLUMNAE];
                    b32  metita[NUMERUS_CORPORIS];
} FixuraComputi;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
    FixuraComputi* f;
              i32  i;

    f = (FixuraComputi*)piscina_allocare(c->piscina,
        magnitudo(FixuraComputi));
    si (f == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    c->datum = f;
    f->radix = lectiones_ambitus("RHUBARB_RADIX");
    si (f->radix == NIHIL)
    {
        f->radix = ".";
    }
    sprintf(f->via_auri, "%s/xml/probationes/fixa/computus/basis.tsv",
        f->radix);

    per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
    {
        (vacuum)xml_computus_metiri(FONTES[i], (i32)strlen(FONTES[i]),
            &f->computa[i]);
        _columnas(&f->computa[i], f->facta[i]);
        f->metita[i] = VERUM;
    }
}

/* I. mensura corporis: proprietates, semper - circuitus totus sanus
 * (etiam invalida); commissa = usus + otiosa; apex usum commissum non
 * superat; allocationes nodos superant. Tempora imprimuntur, non
 * pinnantur. */
interior vacuum
_mensuram_probare (
    CredoContextus* c)
{
    constans FixuraComputi* f = (constans FixuraComputi*)c->suita;
                       i32  i;

    si (f == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
    {
        constans XmlComputus* m = &f->computa[i];

        si (!f->metita[i])
        {
            perge;
        }
        imprimere("  %s\n    nodi %u lex %u stml %u diag %u  "
                  "dati %.2f MB"
                  "  alloc %lu  pars %.2f ms  emit %.2f  stml %.2f/%.2f"
                  "  conf %.2f  coq %.2f\n",
            CORPUS[i], (insignatus)m->nodi, (insignatus)m->lexemata,
            (insignatus)m->octeti_stml, (insignatus)m->diagnostica,
            (f64)m->usus / (1024.0 * 1024.0),
            (insignatus longus)m->allocationes, m->ms_parsandi,
            m->ms_emittendi, m->ms_arbor_scribendi, m->ms_arbor_legendi,
            m->ms_comparandi, m->ms_coquendi);
        CREDO_VERUM (m->successus);
        CREDO_VERUM (m->commissa == m->usus + m->otiosa);
        CREDO_VERUM (m->apex <= m->commissa);
        CREDO_VERUM (m->alvei >= I);
        CREDO_VERUM (m->allocationes > (memoriae_index)m->nodi);
        CREDO_VERUM (m->lexemata >= I && m->octeti_stml > ZEPHYRUM);
    }
    /* invalidum diagnostica habet, valida nulla */
    CREDO_VERUM (f->facta[ZEPHYRUM][IV] == ZEPHYRUM);
    CREDO_VERUM (f->facta[I][IV] == ZEPHYRUM);
    CREDO_VERUM (f->facta[II][IV] > ZEPHYRUM);
}

/* II. collatio cum auro (COMPUTUS_SCRIBERE=1 prius rescribit) */
interior vacuum
_aurum_conferre (
    CredoContextus* c)
{
    constans FixuraComputi* f = (constans FixuraComputi*)c->suita;
                    chorda  aurum;
                       i32  i;
                       i32  k;

    si (f == NIHIL)
    {
        redde;
    }
    si (lectiones_ambitus("COMPUTUS_SCRIBERE") != NIHIL)
    {
        imprimere("\n  AURUM SCRIBITUR - AUDI causam\n");
        {
            character textus[IV * MXXIV];

            textus[ZEPHYRUM] = '\0';
            sprintf(textus + strlen(textus), "# via");
            per (k = ZEPHYRUM; k < COLUMNAE; k++)
            {
                sprintf(textus + strlen(textus), "\t%s", TITULI[k]);
            }
            sprintf(textus + strlen(textus), "\n");
            per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
            {
                si (!f->metita[i])
                {
                    perge;
                }
                sprintf(textus + strlen(textus), "%s", CORPUS[i]);
                per (k = ZEPHYRUM; k < COLUMNAE; k++)
                {
                    sprintf(textus + strlen(textus), "\t%lu",
                        f->facta[i][k]);
                }
                sprintf(textus + strlen(textus), "\n");
            }
            CREDO_VERUM (filum_scribere_literis(f->via_auri, textus));
            imprimere("  scriptum: %s\n", f->via_auri);
        }
    }

    imprimere("  aurum: %s\n", f->via_auri);
    aurum = filum_legere_totum(f->via_auri, c->piscina);
    si (aurum.datum == NIHIL)
    {
        imprimere("  FRACTA: aurum absens - COMPUTUS_SCRIBERE=1\n");
        CREDO_NON_NIHIL (aurum.datum);
    }
    alioquin
    {
                  character  linea[MMXLVIII];
                        i32  lectae = ZEPHYRUM;
         constans character* cursor = chorda_ut_cstr(aurum, c->piscina);

        dum (_lineam_legere(&cursor, linea, (i32)magnitudo(linea)))
        {
                    character* campus;
            insignatus longus  exspectata[COLUMNAE];
                    character  titulus[DXII];
                          i32  n;
                          s32  idx;   /* -I = non inventum */

            si (linea[ZEPHYRUM] == '#' || linea[ZEPHYRUM] == '\n')
            {
                perge;
            }
            campus = strtok(linea, "\t\n");
            si (campus == NIHIL)
            {
                perge;
            }
            strncpy(titulus, campus, magnitudo(titulus) - I);
            titulus[magnitudo(titulus) - I]  = '\0';
            n                                = ZEPHYRUM;
            dum (   n < COLUMNAE
                 && (campus = strtok(NIHIL, "\t\n")) != NIHIL)
            {
                exspectata[n] = strtoul(campus, NIHIL, X);
                n++;
            }
            CREDO_AEQUALIS_I32 ((i32)n, (i32)COLUMNAE);
            idx = (s32)-I;
            per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
            {
                si (strcmp(CORPUS[i], titulus) == ZEPHYRUM)
                {
                    idx = (s32)i;
                }
            }
            CREDO_VERUM (idx >= ZEPHYRUM);
            si (idx < ZEPHYRUM || !f->metita[idx] || n != COLUMNAE)
            {
                perge;
            }
            lectae++;
            per (k = ZEPHYRUM; k < COLUMNAE; k++)
            {
                si (f->facta[idx][k] != exspectata[k])
                {
                    imprimere("  DIVERGIT %s.%s: %lu pro %lu\n",
                        titulus, TITULI[k], f->facta[idx][k],
                        exspectata[k]);
                }
                CREDO_VERUM (f->facta[idx][k] == exspectata[k]);
            }
        }
        /* aurum plagulas OMNES corporis ferat */
        CREDO_AEQUALIS_I32 ((i32)lectae, (i32)NUMERUS_CORPORIS);
    }

}

hic_manens constans CredoSectio SECTIONES[] = {
    { "I. mensura corporis (inlineum)", _mensuram_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "II. collatio cum auro", _aurum_conferre, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_computus", SECTIONES,
        _suitam_parare, NIHIL);
}
