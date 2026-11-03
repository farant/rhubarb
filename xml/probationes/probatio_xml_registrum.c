/* probatio_xml_registrum.c - Registrum et lexicon xml
 *
 * QUATTUOR CUSTODIAE (exemplar probatio_toml_registrum):
 *
 *  1. ORDO LEXICI enumerationem XmlLexGenus sequi DEBET, quia
 *     materia genera ut INDICES tractat. Assertio per TITULOS fit,
 *     non per numeros: permutatio ergo capitur, non absorbetur.
 *  2. RANCOR tabularum coctarum: xml_registrum_coctum.{h,c} ex
 *     xml.registrum.stml GENERATAE - redditio in memoria contra
 *     plagulas commissas OCTETIM (materia_registrum_recens).
 *  3. OFFSETS LOCORUM CONTIGUI, et LOCI NOMINATI (enumerationes
 *     xml_registrum.h) contra titulos tabulae. Genera quae
 *     enumerationem communem habent hic SEORSUM enumerantur.
 *  4. TRIVIA: SPATIA solum; COMMENTARIUM contentum (non trivium);
 *     munus LINEA NULLUM (XML lineas logicas non habet).
 * Et arbor minima '<a/>' per scriptorem -> lectorem -> scriptorem.
 */

#include "latina.h"
#include "credo.h"
#include "xml_registrum.h"
#include "xml_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_coctor.h"
#include "piscina.h"
#include "lectiones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

/* Quaesitio litterarum in chorda SINE fine NUL */
interior b32
_textus_continet (
                 chorda  textus,
     constans character* litterae)
{
    i32 mensura = (i32)strlen(litterae);
    i32 i;

    si (mensura == ZEPHYRUM || textus.mensura < mensura)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i + mensura <= textus.mensura; i++)
    {
        si (memcmp(textus.datum + i, litterae, (size_t)mensura)
                == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior MateriaToken*
_token (
               Piscina* piscina,
                   s32  genus,
    constans character* textus,
                   s32  offset)
{
    redde materia_token_creare(piscina, &FORMA, genus,
        chorda_ex_literis(textus, piscina), offset, (i32)I,
        (i32)(offset + I), ZEPHYRUM);
}

/* Nomina generum lexicalium, ordine XmlLexGenus. Fons veritatis
 * SECUNDUS consulto: si xml_lexicon.h permutetur, haec lista et
 * lexicon DISCREPABUNT et probatio cadet. */
hic_manens constans character* ORDO_EXSPECTATUS[] = {
    "FINIS",
    "SIGNUM_ORDINIS",
    "TEXTUS",
    "REFERENTIA",
    "CDATA",
    "COMMENTARIUM",
    "INSTRUCTIO",
    "DECLARATIO_XML",
    "INSTRUCTIO_FINIS",
    "TAG_APERTURA",
    "TAG_CLAUSURA",
    "TAG_FINIS",
    "TAG_FINIS_SOLUS",
    "TITULUS",
    "AEQUALE",
    "VALOR",
    "DOCTYPE",
    "PARS_INTERNA_APERTURA",
    "PARS_INTERNA_CLAUSURA",
    "DECLARATIO",
    "REFERENTIA_PARAMETRI",
    "VERBUM_PRAEDEFINITUM",
    "DELIM",
    "SPATIA",
    "IGNOTUM"
};

/* Tituli generum nodorum, ordine declarationis */
hic_manens constans character* GENERA_EXSPECTATA[] = {
    "documentum",
    "signum-ordinis",
    "declaratio-xml",
    "doctype",
    "pars-interna",
    "declaratio",
    "referentia-parametri",
    "elementum",
    "attributum",
    "textus",
    "referentia",
    "cdata",
    "commentarium",
    "instructio",
    "malum"
};

nomen structura {
                    s32  genus;
                    i32  locus;
     constans character* titulus;
} LocusNominatus;

#define LN(g, l, t) { (s32)(g), (i32)(l), (t) }

/* Omnis locus omnis generis: enumeratio xml_registrum.h contra
 * titulum tabulae generatae */
hic_manens constans LocusNominatus LOCI_NOMINATI[] = {
    LN(XML_GENUS_DOCUMENTUM, XML_DOCUMENTUM_LIBERI, "liberi"),
    LN(XML_GENUS_DOCUMENTUM, XML_DOCUMENTUM_CAUDA, "cauda"),
    LN(XML_GENUS_SIGNUM_ORDINIS, XML_LEXEMA_TOK, "tok"),
    LN(XML_GENUS_DECLARATIO_XML, XML_DECLARATIO_XML_TOK_APERTURA,
        "tok_apertura"),
    LN(XML_GENUS_DECLARATIO_XML, XML_DECLARATIO_XML_ATTRIBUTA,
        "attributa"),
    LN(XML_GENUS_DECLARATIO_XML, XML_DECLARATIO_XML_TOK_CLAUSURA,
        "tok_clausura"),
    LN(XML_GENUS_DOCTYPE, XML_DOCTYPE_TOK_APERTURA, "tok_apertura"),
    LN(XML_GENUS_DOCTYPE, XML_DOCTYPE_TOK_TITULUS, "tok_titulus"),
    LN(XML_GENUS_DOCTYPE, XML_DOCTYPE_ID_EXTERNUM, "id_externum"),
    LN(XML_GENUS_DOCTYPE, XML_DOCTYPE_PARS_INTERNA, "pars_interna"),
    LN(XML_GENUS_DOCTYPE, XML_DOCTYPE_TOK_CLAUSURA, "tok_clausura"),
    LN(XML_GENUS_PARS_INTERNA, XML_PARS_INTERNA_TOK_APERTURA,
        "tok_apertura"),
    LN(XML_GENUS_PARS_INTERNA, XML_PARS_INTERNA_LIBERI, "liberi"),
    LN(XML_GENUS_PARS_INTERNA, XML_PARS_INTERNA_TOK_CLAUSURA,
        "tok_clausura"),
    LN(XML_GENUS_DECLARATIO, XML_DECLARATIO_TOK_APERTURA,
        "tok_apertura"),
    LN(XML_GENUS_DECLARATIO, XML_DECLARATIO_PARTES, "partes"),
    LN(XML_GENUS_DECLARATIO, XML_DECLARATIO_TOK_CLAUSURA,
        "tok_clausura"),
    LN(XML_GENUS_REFERENTIA_PARAMETRI, XML_LEXEMA_TOK, "tok"),
    LN(XML_GENUS_ELEMENTUM, XML_ELEMENTUM_TOK_APERTURA, "tok_apertura"),
    LN(XML_GENUS_ELEMENTUM, XML_ELEMENTUM_ATTRIBUTA, "attributa"),
    LN(XML_GENUS_ELEMENTUM, XML_ELEMENTUM_TOK_FINIS, "tok_finis"),
    LN(XML_GENUS_ELEMENTUM, XML_ELEMENTUM_LIBERI, "liberi"),
    LN(XML_GENUS_ELEMENTUM, XML_ELEMENTUM_TOK_CLAUSURA, "tok_clausura"),
    LN(XML_GENUS_ELEMENTUM, XML_ELEMENTUM_TOK_CLAUSURA_FINIS,
        "tok_clausura_finis"),
    LN(XML_GENUS_ATTRIBUTUM, XML_ATTRIBUTUM_TOK_TITULUS, "tok_titulus"),
    LN(XML_GENUS_ATTRIBUTUM, XML_ATTRIBUTUM_TOK_AEQUALE, "tok_aequale"),
    LN(XML_GENUS_ATTRIBUTUM, XML_ATTRIBUTUM_TOK_VALOR, "tok_valor"),
    LN(XML_GENUS_TEXTUS, XML_LEXEMA_TOK, "tok"),
    LN(XML_GENUS_REFERENTIA, XML_LEXEMA_TOK, "tok"),
    LN(XML_GENUS_CDATA, XML_LEXEMA_TOK, "tok"),
    LN(XML_GENUS_COMMENTARIUM, XML_LEXEMA_TOK, "tok"),
    LN(XML_GENUS_INSTRUCTIO, XML_LEXEMA_TOK, "tok"),
    LN(XML_GENUS_MALUM, XML_MALUM_TOKENS, "tokens")
};


/* FIXURA SUITAE: ratum lexici semel factum; sectiones id per
 * monstratorem constantem LEGUNT. */
nomen structura {
    MateriaLexiconRatum ratum;
} FixuraLexici;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
          FixuraLexici* f;
    MateriaLexIudicium  iudicium;

    f = (FixuraLexici*)piscina_allocare(c->piscina,
        magnitudo(FixuraLexici));
    si (f == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    CREDO_VERUM (materia_lexicon_ratum_facere(&f->ratum, &XML_LEXICON,
        &iudicium));
    CREDO_AEQUALIS_S32 (iudicium.vitium, (s32)MATERIA_LEX_SANUM);
    c->datum = f;
}


/* ==================================================
 * PROBARE: lexicon portam transit, et ORDINEM servat
 * ================================================== */

interior vacuum
_lexicon_probare (
    CredoContextus* c)
{
    constans MateriaLexiconRatum* ratum;

    si (c->suita == NIHIL)
    {
        redde;
    }
    ratum = &((constans FixuraLexici*)c->suita)->ratum;
    {
        i32 i;


        CREDO_AEQUALIS_I32 (XML_LEXICON.numerus_generum,
            (i32)XML_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(ORDO_EXSPECTATUS)
            / magnitudo(ORDO_EXSPECTATUS[0])),
            (i32)XML_LEX_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)XML_LEX_NUMERUS_GENERUM, (i32)XXV);

        /* ORDO per titulos - permutatio capitur */
        per (i = ZEPHYRUM; i < (i32)XML_LEX_NUMERUS_GENERUM; i++)
        {
            constans character* t = materia_lexicon_titulus(ratum,
                (s32)i);

            CREDO_NON_NIHIL (t);
            CREDO_VERUM (strcmp(t, ORDO_EXSPECTATUS[i]) == ZEPHYRUM);
        }

        /* Sedes notae */
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(ratum,
            (s32)XML_LEX_SIGNUM_ORDINIS), "\357\273\277") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(ratum,
            (s32)XML_LEX_DECLARATIO_XML), "<?xml") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(ratum,
            (s32)XML_LEX_DOCTYPE), "<!DOCTYPE") == ZEPHYRUM);
        CREDO_VERUM (strcmp(materia_lexicon_orthographia(ratum,
            (s32)XML_LEX_TAG_FINIS_SOLUS), "/>") == ZEPHYRUM);
        CREDO_AEQUALIS_S32 ((s32)materia_lexicon_species(ratum,
            (s32)XML_LEX_TAG_APERTURA), (s32)MATERIA_LEX_VERBATIM);
        CREDO_VERUM (materia_lexicon_textum_fert(ratum,
            (s32)XML_LEX_TEXTUS));
        CREDO_VERUM (materia_lexicon_textum_fert(ratum,
            (s32)XML_LEX_VALOR));
        CREDO_VERUM (materia_lexicon_textum_fert(ratum,
            (s32)XML_LEX_IGNOTUM));
        CREDO_FALSUM (materia_lexicon_textum_fert(ratum,
            (s32)XML_LEX_AEQUALE));
    }
}


/* ==================================================
 * PROBARE: trivia - SPATIA solum; commentarium contentum
 * ================================================== */

interior vacuum
_trivia_probare (
    CredoContextus* c)
{
    constans MateriaLexiconRatum* ratum;

    si (c->suita == NIHIL)
    {
        redde;
    }
    ratum = &((constans FixuraLexici*)c->suita)->ratum;
    {
        i32 i;
        i32 trivia = ZEPHYRUM;


        per (i = ZEPHYRUM; i < (i32)XML_LEX_NUMERUS_GENERUM; i++)
        {
            si (materia_lexicon_trivium_est(ratum, (s32)i))
            {
                trivia++;
            }
        }
        CREDO_AEQUALIS_I32 (trivia, (i32)I);
        CREDO_VERUM (materia_lexicon_trivium_est(ratum,
            (s32)XML_LEX_SPATIA));
        CREDO_FALSUM (materia_lexicon_trivium_est(ratum,
            (s32)XML_LEX_COMMENTARIUM));
        CREDO_FALSUM (materia_lexicon_trivium_est(ratum,
            (s32)XML_LEX_TEXTUS));

        CREDO_VERUM (materia_lexicon_munus_habet(ratum,
            MATERIA_MUNUS_SPATIUM));
        CREDO_VERUM (materia_lexicon_munus_habet(ratum,
            MATERIA_MUNUS_FINIS));
        CREDO_FALSUM (materia_lexicon_munus_habet(ratum,
            MATERIA_MUNUS_LINEA));
        CREDO_FALSUM (materia_lexicon_munus_habet(ratum,
            MATERIA_MUNUS_COMMENTUM));
    }
}


/* ==================================================
 * PROBARE: porta rancoris tabularum coctarum
 * ================================================== */

interior vacuum
_tabulas_coctas_probare (
    CredoContextus* c)
{
    Piscina* piscina = c->piscina;

    {
        constans character* radix = lectiones_ambitus("RHUBARB_RADIX");
             MateriaRancor  rancor;

        si (!materia_registrum_recens(piscina, radix != NIHIL ? radix
            : ".", "xml/grammatica/xml.registrum.stml", &rancor))
        {
            imprimere("    recusatio: %.*s\n",
                (integer)rancor.causa.mensura,
                (constans character*)rancor.causa.datum);
            CREDO_CULPA ("declaratio absens aut recusata");
        }
        alioquin
        {
            si (!rancor.recens)
            {
                imprimere("    RANCIDUM: %.*s:%u\n",
                    (integer)rancor.via.mensura,
                    (constans character*)rancor.via.datum,
                    rancor.linea);
            }
            CREDO_VERUM (rancor.recens);
            CREDO_AEQUALIS_I32 (rancor.coctio.numerus_locorum,
                XML_REGISTRUM.numerus_locorum);
            CREDO_AEQUALIS_I32 (rancor.coctio.numerus_generum,
                XML_REGISTRUM.numerus_generum);
        }
    }
}


/* ==================================================
 * PROBARE: registrum - ordo, CONTIGUITAS locorum, loci nominati
 * ================================================== */

interior vacuum
_registrum_probare (
    CredoContextus* c)
{
    (vacuum)c;

    {
        i32 i;
        i32 exspectatus_offset;
        i32 numerus_nominatorum;


        CREDO_AEQUALIS_I32 (XML_REGISTRUM.numerus_generum,
            (i32)XML_GENUS_NUMERUS_GENERUM);
        CREDO_AEQUALIS_I32 ((i32)XML_GENUS_NUMERUS_GENERUM, (i32)XV);
        CREDO_AEQUALIS_I32 (XML_REGISTRUM.numerus_locorum, (i32)XXXIII);
        CREDO_AEQUALIS_I32 ((i32)(magnitudo(GENERA_EXSPECTATA)
            / magnitudo(GENERA_EXSPECTATA[0])),
            (i32)XML_GENUS_NUMERUS_GENERUM);

        per (i = ZEPHYRUM; i < (i32)XML_GENUS_NUMERUS_GENERUM; i++)
        {
            CREDO_NON_NIHIL (XML_REGISTRUM.genera[i].titulus);
            CREDO_VERUM (strcmp(XML_REGISTRUM.genera[i].titulus,
                GENERA_EXSPECTATA[i]) == ZEPHYRUM);
            CREDO_AEQUALIS_S32 (materia_arbor_genus_index(
                &XML_REGISTRUM, GENERA_EXSPECTATA[i],
                (i32)strlen(GENERA_EXSPECTATA[i])), (s32)i);
        }

        exspectatus_offset = ZEPHYRUM;
        per (i = ZEPHYRUM; i < (i32)XML_GENUS_NUMERUS_GENERUM; i++)
        {
            CREDO_AEQUALIS_I32 (XML_REGISTRUM.genera[i].loci_offset,
                exspectatus_offset);
            CREDO_MAIOR_I32 (XML_REGISTRUM.genera[i].loci_numerus,
                ZEPHYRUM);
            exspectatus_offset += XML_REGISTRUM.genera[i].loci_numerus;
        }
        CREDO_AEQUALIS_I32 (exspectatus_offset,
            XML_REGISTRUM.numerus_locorum);

        numerus_nominatorum = (i32)(magnitudo(LOCI_NOMINATI)
            / magnitudo(LOCI_NOMINATI[0]));
        CREDO_AEQUALIS_I32 (numerus_nominatorum,
            XML_REGISTRUM.numerus_locorum);
        per (i = ZEPHYRUM; i < numerus_nominatorum; i++)
        {
            constans LocusNominatus* n = &LOCI_NOMINATI[i];
                                i32  absolutus;

            CREDO_MINOR_I32 (n->locus,
                XML_REGISTRUM.genera[n->genus].loci_numerus);
            absolutus = XML_REGISTRUM.genera[n->genus].loci_offset
                      + n->locus;
            CREDO_VERUM (strcmp(XML_REGISTRUM.loci[absolutus].titulus,
                n->titulus) == ZEPHYRUM);
        }
    }
}


/* ==================================================
 * PROBARE: circuitus per materiam (arbor minima '<a/>')
 * ================================================== */

interior vacuum
_circuitum_probare (
    CredoContextus* c)
{
    constans MateriaLexiconRatum* ratum;
                         Piscina* piscina = c->piscina;

    si (c->suita == NIHIL)
    {
        redde;
    }
    ratum = &((constans FixuraLexici*)c->suita)->ratum;
    {
        MateriaArborConsilium compositus;
                 MateriaNodus* documentum;
                 MateriaNodus* elementum;
        MateriaArborScriptura  s1;
        MateriaArborScriptura  s2;
                 MateriaNodus* lecta;
           MateriaArborVitium  vitium;


        elementum = materia_nodus_creare(piscina,
            (s32)XML_GENUS_ELEMENTUM,
            XML_REGISTRUM.genera[XML_GENUS_ELEMENTUM].loci_numerus);
        CREDO_VERUM (materia_nodus_ponere(elementum,
            (i32)XML_ELEMENTUM_TOK_APERTURA, materia_valor_token(_token(
                piscina, (s32)XML_LEX_TAG_APERTURA, "<a", ZEPHYRUM)),
            MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_ponere(elementum,
            (i32)XML_ELEMENTUM_TOK_FINIS, materia_valor_token(_token(
                piscina, (s32)XML_LEX_TAG_FINIS_SOLUS, "/>", (s32)II)),
            MATERIA_LOCUS_TOKEN));
        documentum = materia_nodus_creare(piscina,
            (s32)XML_GENUS_DOCUMENTUM,
            XML_REGISTRUM.genera[XML_GENUS_DOCUMENTUM].loci_numerus);
        CREDO_VERUM (materia_nodus_appendere(piscina, documentum,
            (i32)XML_DOCUMENTUM_LIBERI, materia_valor_nodus(elementum),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_ponere(documentum,
            (i32)XML_DOCUMENTUM_CAUDA, materia_valor_token(_token(
                piscina, (s32)XML_LEX_FINIS, "", (s32)IV)),
            MATERIA_LOCUS_TOKEN));

        materia_arbor_consilium_nudum(&compositus, &XML_REGISTRUM,
            ratum, "xml");
        s1 = materia_arbor_scribere_nodum(piscina, documentum,
            &compositus);
        CREDO_VERUM (s1.successus);
        CREDO_VERUM (_textus_continet(s1.textus,
            "grammatica=\"xml\""));
        CREDO_VERUM (_textus_continet(s1.textus, "<xml-tag-apertura"));
        CREDO_VERUM (_textus_continet(s1.textus, "<elementum"));

        lecta = materia_arbor_legere(piscina, NIHIL, s1.textus,
            &compositus, &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);
        si (lecta != NIHIL)
        {
            CREDO_AEQUALIS_S32 (lecta->genus,
                (s32)XML_GENUS_DOCUMENTUM);
            s2 = materia_arbor_scribere_nodum(piscina, lecta,
                &compositus);
            CREDO_VERUM (s2.successus);
            CREDO_AEQUALIS_I32 (s2.textus.mensura, s1.textus.mensura);
            CREDO_VERUM (memcmp(s1.textus.datum, s2.textus.datum,
                (size_t)s1.textus.mensura) == ZEPHYRUM);
        }
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "lexicon: portam transit, ordinem servat", _lexicon_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "trivia: SPATIA solum, commentarium contentum", _trivia_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "tabulae coctae recentes", _tabulas_coctas_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "registrum: ordo, contiguitas, loci nominati", _registrum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "circuitus per materiam (<a/>)", _circuitum_probare,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("xml_registrum", SECTIONES,
        _suitam_parare, NIHIL);
}
