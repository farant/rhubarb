/* probatio_html_stml.c - Circuitus STML: duo cycli, duo oracula (H6)
 *
 * scribere -> legere -> scribere: octeti STML bis idem (vitium quod se
 * componit circuitum unum saepe superat), et arbor parsata contra
 * arborem relectam per COMPARATOREM (materia_arbor_aequalis,
 * STRUCTURALIS): dislocatio dominii triviorum octetim invisibilis
 * est - solus comparator eam videt.
 *
 * FIDELITAS non curritur: lector lineam/columnam ex cursore reficit
 * quem munus LINEA solum movet, quod HTML non habet (SPATIA lineas
 * novas intra tag absorbet, extra tags lineae novae TEXTUS sunt).
 * Positiones ex octetis derivabiles sunt et documentum eas non
 * fert; STRUCTURALIS ergo modus assertus est (css exemplar).
 *
 * CENSUS COMPRESSIONIS: spatia_vocationes ZEPHYRUM - SPATIA VERBATIM
 * est, compressio templorum mortua (praesentia, non pinnae).
  * LIMES SUBSTRATI SOLUTUS (inventus H6 2026-09-15, solutus
 * 2026-09-21): valor lexematis qui sequentiam claudentem elementi
 * SUI continet ('</lex-textus-crudus>' intra textum crudum scripti,
 * '</lex-commentarium>' intra commentarium) olim a scriptore
 * RECUSABATUR ('valor sequentiam claudentem fert'), quia forma cruda
 * eum ferre nequibat. Nunc FERT: substratum scalam fugae habet
 * (planum clausurae gradus I-II) et materia recusationem deposuit
 * (gradus III). In textu nudo idem '</lex-textus>' semper innocuus
 * fuit quia lexator HTML eum in tag clausurae scindit. PINNA infra
 * PROMOTA: quinque casus, olim recusationem asserentes, nunc
 * circuitum OCTETIM INTEGRUM.
 *
 * ACCEPTATIO O7 (Fran 2026-09-15): annotationes visionis derivatae
 * (synthesis O7a; sedes O7b; exemplar O7c) circuitum INTEGRUM servant
 * - casus annotati infra (involucra ficta, partes tabulae fictae,
 * fragmenta) per cyclos duos, comparatorem et emissionem directam
 * arboris RELECTAE octetim == fons; et annotatio ipsa per circuitum
 * vivit (locus synthesis in arbore relecta, PORTA infra).
 */

#include "latina.h"
#include "lectiones.h"   /* lectiones_fopen: canalis (examen 96) */
#include "credo.h"
#include "html_arbor.h"
#include "html_registrum.h"
#include "html_lexicon.h"
#include "materia_arbor.h"
#include "materia_sedes.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


hic_manens constans character* CORPUS[] = {
    "probationes/fixa/html/adversarius.html",
    "probationes/fixa/html/adversarius_2.html",
    "probationes/fixa/html/laboratorium_index_2026-08-18.html",
    "probationes/fixa/html/villa_index_2026-08-18.html",
    "briar/facies/facies.html",
    "briar/probationes/fixa/facies/salve.html",
    "briar/probationes/fixa/facies/circulus.html",
    "briar/probationes/fixa/facies/fragmenta.html",
    "apps/villa/assets/index.html",
    "apps/mensor/assets/index.html",
    "tools/silex_assets/index.html",
    "probationes/fixa/html/knotapel_atlas_index_2026-02-26.html",
    "lib/speculum_assets/probatio_visus.html",
    "reference/aquinas-dithering-tool.html"
};

#define NUMERUS_CORPORIS ((i32)(magnitudo(CORPUS)/magnitudo(CORPUS[0])))

hic_manens constans character* CASUS[] = {
    "",
    "<p>a</p>",
    "<div id=\"x\" class='y' z>t</div>",
    "<a  href = x\n  >b</A >",
    "x &amp; y <!-- c --> <![CDATA[z]]>",
    "<!DOCTYPE html><html></html>",
    "<div cl",
    "</x>a",
    "<div>",
    "<img/>",
    "<a = x>",
    "</div",
    "<a /\n href=x>",
    "<script>a</b></script>",
    "<p>a<p>b",
    "<ul><li>a<li>b</ul>",
    "<br><br/><BR>x",
    "<table><tr><td>1<td>2<tr><td>3</table>",
    "<textarea><p></textarea>",
    "<p>\n  <b>x</b>\n</p>\n",
    "<p>textus cum </lex-textus> intra se</p>",
    "a\r\nb<p\r\n class=x>c\r\n</p>",
    "<div\n",
    /* O7a: involucra ficta, partes tabulae fictae, textus albus ante
     * html (malum), '</head>' in ficto */
    "<p>a",
    "<table><td>x",
    "<table><col><col>",
    "<title>t</title>x",
    "\n<p>x</p>\n",
    "</head>x",
    "<html><head></head><body></body></html>\n",
    /* O7b: sedes - fota (elementum, textus, input non celatus), post
     * body (textus, elementum, commentarium), caput post head */
    "<table><div>x</div><tr><td>1</table>",
    "<table>a &amp; b<tr><td>c",
    "<table><input type=hidden><input>",
    "<body></body><!--c--><p>x",
    "<html><body></body></html>x<!--c-->",
    "<head></head><title>t</title><p>x",
    "<bdy></body><br>",
    /* O7c: adoption agency - exemplaria, praecedens, involutio;
     * reconstructio; arca Noe; '</p>' et '</br>' ficta */
    "<a>1<p>2</a>3</p>",
    "<b><a><div>x</b>y</a>",
    "<p><b>x</p>y",
    "<b><b><b><b>x</b></b></b></b>y",
    "<a href=x>1<a href=y>2",
    "<table><a>1<td>2</td>3</table>",
    "x</p>y</br>z"
};

#define NUMERUS_CASUUM ((i32)(magnitudo(CASUS)/magnitudo(CASUS[0])))

/* O7a: fragmenta (contextus HTML) - contextus vertex sub acervo */
nomen structura {
    constans character* contextus;
    constans character* fons;
} Fragmentum;

hic_manens constans Fragmentum FRAGMENTA[] = {
    { "table", "<tr><td>x" },
    { "tbody", "<td>x</td>" },
    { "html",  "<p>a" },
    { "body",  "<td>x" },
    { "frameset", "<p><frame>" },
    /* O7b: sedes in radicem (referentia ad documentum) */
    { "table", "<tr><div>x" },
    { "tbody", "a<caption>x" },
    /* O7c: adoptio et reconstructio in fragmentis (exemplar fotum) */
    { "body",  "<b><p>x</b>y" },
    { "table", "<a>1<td>2</td>3" }
};

#define NUMERUS_FRAGMENTORUM \
    ((i32)(magnitudo(FRAGMENTA)/magnitudo(FRAGMENTA[0])))

/* Causae nominatae */
enumeratio {
    CIRCUITUS_IDEM = 0,
    CIRCUITUS_PARSATOR_NIHIL,
    CIRCUITUS_SCRIPTURA_RECUSATA,
    CIRCUITUS_LECTIO_RECUSATA,
    CIRCUITUS_RESCRIPTURA_RECUSATA,
    CIRCUITUS_OCTETI_DISPARES,
    CIRCUITUS_ARBOR_DISPAR,
    CIRCUITUS_COMPRESSIO_VIVA,
    CIRCUITUS_RELECTA_DISPAR_FONTI
};

hic_manens constans character* CAUSAE[] = {
    "idem",
    "parsator NIHIL reddidit",
    "scriptura recusata",
    "lectio recusata",
    "rescriptura recusata",
    "octeti STML dispares inter cyclos",
    "arbor relecta dispar",
    "compressio templorum viva",
    "arbor relecta fontem non emittit"
};

hic_manens character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = lectiones_fopen(via, "rb");
    si (f == NIHIL)
    { redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    { fclose(f); redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    { fclose(f); redde NIHIL;
    }
    rewind(f);

    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    { redde NIHIL;
    }

    *mensura = (i32)longitudo;
    redde memoria;
}

hic_manens MateriaNodus*
_liber (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  i)
{
    MateriaValor* v;

    si (   nodus                    == NIHIL
        || nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    { redde NIHIL;
    }
    v = materia_valor_lista_obtinere(nodus->loci[locus], i);
    redde (v != NIHIL && v->genus == MATERIA_VALOR_NODUS)
        ? v->datum.nodus : NIHIL;
}

/* Valor loci synthesis (NULLA si absens aut nodus non elementum) */
hic_manens s32
_synthesis (
    constans MateriaNodus* nodus)
{
    si (   nodus        == NIHIL
        || nodus->genus != (s32)HTML_GENUS_ELEMENTUM
        || nodus->loci[HTML_ELEMENTUM_SYNTHESIS].genus
               != MATERIA_VALOR_INDEX)
    { redde (s32)HTML_SYNTHESIS_NULLA;
    }
    redde nodus->loci[HTML_ELEMENTUM_SYNTHESIS].datum.index;
}

/* Referentia loci elementi (O7c: exemplar, praecedens), aut NIHIL */
hic_manens MateriaNodus*
_referentia_elementi (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (   nodus == NIHIL || nodus->genus != (s32)HTML_GENUS_ELEMENTUM
        || nodus->loci[locus].genus != MATERIA_VALOR_REFERENTIA)
    { redde NIHIL;
    }
    redde nodus->loci[locus].datum.nodus;
}

/* Sedes nodi (O7b): parens DOM alibi, aut NIHIL */
hic_manens MateriaNodus*
_sedes (
    constans MateriaNodus* nodus)
{
    i32 locus;

    si (nodus == NIHIL)
    { redde NIHIL;
    }
    si (nodus->genus == (s32)HTML_GENUS_ELEMENTUM)
    { locus = (i32)HTML_ELEMENTUM_SEDES;
    }
    alioquin si (nodus->genus == (s32)HTML_GENUS_TEXTUS)
    { locus = (i32)HTML_TEXTUS_SEDES;
    }
    alioquin si (nodus->genus == (s32)HTML_GENUS_REFERENTIA)
    { locus = (i32)HTML_REFERENTIA_SEDES;
    }
    alioquin si (nodus->genus == (s32)HTML_GENUS_COMMENTARIUM)
    { locus = (i32)HTML_COMMENTARIUM_SEDES;
    }
    alioquin
    { redde NIHIL;
    }
    si (nodus->loci[locus].genus != MATERIA_VALOR_REFERENTIA)
    { redde NIHIL;
    }
    redde nodus->loci[locus].datum.nodus;
}

hic_manens b32
_textus_continet (
                 chorda  textus,
     constans character* litterae)
{
    i32 mensura = (i32)strlen(litterae);
    i32 i;

    si (mensura == ZEPHYRUM || textus.mensura < mensura)
    { redde FALSUM;
    }
    per (i = ZEPHYRUM; i + mensura <= textus.mensura; i++)
    {
        si (memcmp(textus.datum + i, litterae, (size_t)mensura)
                == ZEPHYRUM)
        { redde VERUM;
        }
    }
    redde FALSUM;
}

/* nuntius = causa scriptoris aut lectoris (NIHIL si nulla) */
nomen structura {
                    integer  causa;
         constans character* nuntius;
    MateriaArborDifferentia  differentia;
                        i32  octeti_stml;
} Circuitus;

/* Duo cycli + comparator. 'mutare' = culpa plantata probationis:
 * arborem relectam ANTE comparationem laedere (octeti iam scripti)
 * - oraculum separans: octeti idem, arbor dispar. contextus NIHIL =
 * documentum, aliter fragmentum (O7a). Visio sedium (materia-sedes
 * A2): elementa verificata in *elementa / *derivata (numeratores
 * sectionis, non statica). */
hic_manens Circuitus
_circuitum_probare (
                            Piscina* piscina,
     constans MateriaArborConsilium* consilium,
                 constans character* fons,
                                i32  mensura,
                 constans character* contextus,
                                b32  mutare,
                               i32* elementa,
                               i32* derivata)
{
                Circuitus c;
             MateriaNodus* radix;
             MateriaNodus* relecta;
    MateriaArborScriptura  s1;
    MateriaArborScriptura  s2;
       MateriaArborVitium  vitium;

    memset(&c, ZEPHYRUM, magnitudo(Circuitus));
    c.causa = CIRCUITUS_IDEM;

    si (contextus == NIHIL)
    {
        radix = html_arbor_parsare(piscina, fons, mensura);
    }
    alioquin
    {
        radix = html_arbor_parsare_fragmentum(piscina, fons, mensura,
            chorda_ex_literis(contextus, piscina), HTML_ALIENUM_NULLUM);
    }
    si (radix == NIHIL)
    { c.causa = CIRCUITUS_PARSATOR_NIHIL; redde c;
    }

    s1 = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s1.successus)
    {
        c.causa    = CIRCUITUS_SCRIPTURA_RECUSATA;
        c.nuntius  = s1.causa;
        redde c;
    }
    c.octeti_stml = s1.textus.mensura;
    {
        MateriaSedesRelatio sedes;

        si (!materia_sedes_verificare(piscina, radix, consilium, fons,
                mensura, &sedes))
        {
            imprimere("    (sedes: %s, elementum %d)\n",
                sedes.causa ? sedes.causa : "-", (integer)sedes.index);
        }
        CREDO_VERUM (sedes.sana);
        *elementa += sedes.elementa;
        *derivata += sedes.derivata;
    }

    relecta = materia_arbor_legere(piscina, NIHIL, s1.textus, consilium,
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

    /* CATENA CLAUSA UNA ASSERTIONE: arbor RELECTA per scriptorem
     * octetorum emissa == fons. Sine hac 'STML bis idem' + 'arbor
     * aequalis' + 'parsata emittit fontem' catenam implicant, non
     * dicunt (Fran, H7). */
    {
        MateriaScripturaConsilium cs;
                 MateriaScriptura emissa;

        materia_scriptura_consilium_nudum(&cs, &HTML_REGISTRUM);
        emissa = materia_scribere_nodum(piscina, relecta, &cs);
        si (   !emissa.successus || emissa.textus.mensura != mensura
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
        /* liberum ultimum documenti relecti abscindere */
        MateriaValor* liberi = &relecta->loci[HTML_DOCUMENTUM_LIBERI];
        si (   liberi->genus == MATERIA_VALOR_LISTA
            && liberi->datum.lista.mensura > ZEPHYRUM)
        {
            liberi->datum.lista.mensura = liberi->datum.lista.mensura
                - I;
        }
    }

    si (!materia_arbor_aequalis(radix, relecta,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &c.differentia))
    { c.causa = CIRCUITUS_ARBOR_DISPAR; redde c;
    }
    si (s1.census.spatia_vocationes != ZEPHYRUM)
    { c.causa = CIRCUITUS_COMPRESSIO_VIVA; redde c;
    }
    redde c;
}

hic_manens vacuum
_causam_imprimere (
    constans Circuitus* c)
{
    si (c->causa == CIRCUITUS_IDEM)
    { redde;
    }
    imprimere("    CAUSA: %s", CAUSAE[c->causa]);
    si (c->nuntius != NIHIL)
    { imprimere(" - %s", c->nuntius);
    }
    si (   c->causa              == CIRCUITUS_ARBOR_DISPAR
        && c->differentia.campus != NIHIL)
    { imprimere(" - campus %s via %s", c->differentia.campus,
          c->differentia.via);
    }
    imprimere("\n");
}

/* FIXURA SUITAE: lexicon et consilium arboris semel facta; sectiones
 * ea per monstratorem constantem LEGUNT. */
nomen structura {
      MateriaLexiconRatum ratum;
    MateriaArborConsilium consilium;
} FixuraStml;

interior vacuum
_suitam_parare (
    CredoContextus* c)
{
            FixuraStml* f;
    MateriaLexIudicium  iudicium;

    f = (FixuraStml*)piscina_allocare(c->piscina,
        magnitudo(FixuraStml));
    si (f == NIHIL)
    {
        redde;   /* c->suita NIHIL: sectiones VACUAE, suita FRACTA */
    }
    si (!materia_lexicon_ratum_facere(&f->ratum, &HTML_LEXICON,
        &iudicium))
    {
        imprimere("LEXICON HTML RECUSATUM\n");
        redde;
    }
    materia_arbor_consilium_nudum(&f->consilium, &HTML_REGISTRUM,
        &f->ratum, "html");
    c->datum = f;
}


/* ==================================================
 * PORTA: casus inlinei, fragmenta, corpus totum - duo cycli +
 * comparator. Sectio una: 'circuitus_numerus' tres partes simul
 * numerat; visio sedium (materia-sedes A2) hic asseritur - porta
 * quae nihil verificavit mortua est.
 * ================================================== */

interior vacuum
_cyclos_probare (
    CredoContextus* c)
{
     constans FixuraStml* f        = (constans FixuraStml*)c->suita;
                 Piscina* piscina  = c->piscina;
      constans character* radix_viae;
                     i32  i;
                     i32  circuitus_numerus  = ZEPHYRUM;
                     i32  octeti_fontis      = ZEPHYRUM;
                     i32  octeti_stml        = ZEPHYRUM;
                     i32  sedes_elementa     = ZEPHYRUM;
                     i32  sedes_derivata     = ZEPHYRUM;

    si (f == NIHIL)
    {
        redde;
    }

    CREDO_GRADUS ("casus inlinei");
    per (i = ZEPHYRUM; i < NUMERUS_CASUUM; i++)
    {
        Circuitus circuitus = _circuitum_probare(piscina, &f->consilium,
            CASUS[i], (i32)strlen(CASUS[i]), NIHIL, FALSUM,
            &sedes_elementa, &sedes_derivata);

        si (circuitus.causa != CIRCUITUS_IDEM)
        {
            imprimere("  casus %d: %s\n", (integer)i, CASUS[i]);
            _causam_imprimere(&circuitus);
        }
        CREDO_AEQUALIS_S32 ((s32)circuitus.causa, (s32)CIRCUITUS_IDEM);
        circuitus_numerus = circuitus_numerus + I;
    }

    CREDO_GRADUS ("fragmenta");
    per (i = ZEPHYRUM; i < NUMERUS_FRAGMENTORUM; i++)
    {
        Circuitus circuitus = _circuitum_probare(piscina, &f->consilium,
            FRAGMENTA[i].fons, (i32)strlen(FRAGMENTA[i].fons),
            FRAGMENTA[i].contextus, FALSUM, &sedes_elementa,
            &sedes_derivata);

        si (circuitus.causa != CIRCUITUS_IDEM)
        {
            imprimere("  fragmentum %d [%s]: %s\n", (integer)i,
                FRAGMENTA[i].contextus, FRAGMENTA[i].fons);
            _causam_imprimere(&circuitus);
        }
        CREDO_AEQUALIS_S32 ((s32)circuitus.causa, (s32)CIRCUITUS_IDEM);
        circuitus_numerus = circuitus_numerus + I;
    }

    CREDO_GRADUS ("corpus");
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    { radix_viae = ".";
    }
    per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
    {
        character  via[DXII];
        character* textus;
              i32  mensura = ZEPHYRUM;
        Circuitus  circuitus;

        sprintf(via, "%s/%s", radix_viae, CORPUS[i]);
        textus = _plagulam_legere(piscina, via, &mensura);
        si (textus == NIHIL)
        {
            imprimere("  ABEST: %s\n", CORPUS[i]);
            CREDO_CULPA ("plagula corporis abest");
            perge;
        }
        circuitus = _circuitum_probare(piscina, &f->consilium, textus,
            mensura, NIHIL, FALSUM, &sedes_elementa, &sedes_derivata);
        imprimere("  %-56s %7d -> %8d octeti STML  %s\n", CORPUS[i],
            (integer)mensura, (integer)circuitus.octeti_stml,
            CAUSAE[circuitus.causa]);
        _causam_imprimere(&circuitus);
        CREDO_AEQUALIS_S32 ((s32)circuitus.causa, (s32)CIRCUITUS_IDEM);
        circuitus_numerus  = circuitus_numerus + I;
        octeti_fontis      = octeti_fontis + mensura;
        octeti_stml        = octeti_stml + circuitus.octeti_stml;
    }

    /* se metiens */
    CREDO_AEQUALIS_I32 (circuitus_numerus, NUMERUS_CASUUM
        + NUMERUS_FRAGMENTORUM + NUMERUS_CORPORIS);
    CREDO_MAIOR_I32 (octeti_fontis, (i32)(C * M));
    CREDO_MAIOR_I32 (octeti_stml, octeti_fontis);
    imprimere("  summa: %d circuitus, %d octeti fontis, %d STML\n",
        (integer)circuitus_numerus, (integer)octeti_fontis,
        (integer)octeti_stml);
    imprimere("  sedes (visio): %d elementa verificata, %d derivata\n",
        (integer)sedes_elementa, (integer)sedes_derivata);
    CREDO_VERUM (sedes_elementa > ZEPHYRUM);
}


/* ==================================================
 * PORTA: annotatio synthesis per circuitum VIVIT (O7a)
 * ================================================== */

interior vacuum
_synthesim_probare (
    CredoContextus* c)
{
    constans FixuraStml* f        = (constans FixuraStml*)c->suita;
                Piscina* piscina  = c->piscina;

    si (f == NIHIL)
    {
        redde;
    }
    {
                 MateriaNodus* radix;
                 MateriaNodus* relecta;
                 MateriaNodus* html;
                 MateriaNodus* corpus;
                 MateriaNodus* tabula;
                 MateriaNodus* sectio;
        MateriaArborScriptura  s;
           MateriaArborVitium  vitium;

        radix = html_arbor_parsare(piscina, "<table><td>x", (i32)XII);
        CREDO_NON_NIHIL (radix);
        s = materia_arbor_scribere_nodum(piscina, radix, &f->consilium);
        CREDO_VERUM (s.successus);
        /* locus index in STML: '<synthesis(> n' (forma textus) */
        CREDO_VERUM (_textus_continet(s.textus, "<synthesis("));
        relecta = s.successus ? materia_arbor_legere(piscina, NIHIL,
            s.textus, &f->consilium, &vitium) : NIHIL;
        CREDO_NON_NIHIL (relecta);
        html = _liber(relecta, (i32)HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        corpus = _liber(html, (i32)HTML_ELEMENTUM_LIBERI, (i32)I);
        tabula = _liber(corpus, (i32)HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        sectio = _liber(tabula, (i32)HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_AEQUALIS_S32 (_synthesis(html),
            (s32)HTML_SYNTHESIS_RADIX);
        CREDO_AEQUALIS_S32 (_synthesis(_liber(html,
            (i32)HTML_ELEMENTUM_LIBERI, ZEPHYRUM)),
            (s32)HTML_SYNTHESIS_CAPUT);
        CREDO_AEQUALIS_S32 (_synthesis(corpus),
            (s32)HTML_SYNTHESIS_CORPUS);
        CREDO_AEQUALIS_S32 (_synthesis(tabula),
            (s32)HTML_SYNTHESIS_NULLA);
        CREDO_AEQUALIS_S32 (_synthesis(sectio),
            (s32)HTML_SYNTHESIS_CORPUS_TABULAE);
        CREDO_AEQUALIS_S32 (_synthesis(_liber(sectio,
            (i32)HTML_ELEMENTUM_LIBERI, ZEPHYRUM)),
            (s32)HTML_SYNTHESIS_ORDO);
    }
}


/* ==================================================
 * PORTA: annotatio sedes per circuitum VIVIT (O7b)
 * ================================================== */

interior vacuum
_sedem_probare (
    CredoContextus* c)
{
    constans FixuraStml* f        = (constans FixuraStml*)c->suita;
                Piscina* piscina  = c->piscina;

    si (f == NIHIL)
    {
        redde;
    }
    {
                 MateriaNodus* radix;
                 MateriaNodus* relecta;
                 MateriaNodus* html;
                 MateriaNodus* corpus;
                 MateriaNodus* tabula;
                 MateriaNodus* fotum;
        MateriaArborScriptura  s;
           MateriaArborVitium  vitium;

        /* div fotum: sedes = body; post relectionem referentia in body
         * RELECTUM spectat (identitas per '#nodN' et id) */
        radix = html_arbor_parsare(piscina,
            "<table><div>x</div><tr><td>1</table>", (i32)XXXVI);
        CREDO_NON_NIHIL (radix);
        s = materia_arbor_scribere_nodum(piscina, radix, &f->consilium);
        CREDO_VERUM (s.successus);
        CREDO_VERUM (_textus_continet(s.textus, "<sedes(> #nod"));
        CREDO_VERUM (_textus_continet(s.textus, "id=\"nod"));
        relecta = s.successus ? materia_arbor_legere(piscina, NIHIL,
            s.textus, &f->consilium, &vitium) : NIHIL;
        CREDO_NON_NIHIL (relecta);
        html = _liber(relecta, (i32)HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        corpus  = _liber(html, (i32)HTML_ELEMENTUM_LIBERI, (i32)I);
        tabula  = _liber(corpus, (i32)HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        fotum   = _liber(tabula, (i32)HTML_ELEMENTUM_LIBERI, ZEPHYRUM);
        CREDO_NON_NIHIL (fotum);
        CREDO_NON_NIHIL (corpus);
        CREDO_AEQUALIS_PTR (_sedes(fotum), corpus);
        CREDO_NIHIL (_sedes(tabula));
    }
}


/* ==================================================
 * PORTA: annotationes exemplar et praecedens per circuitum VIVUNT
 * (O7c)
 * ================================================== */

interior vacuum
_exemplar_praecedentem_probare (
    CredoContextus* c)
{
    constans FixuraStml* f        = (constans FixuraStml*)c->suita;
                Piscina* piscina  = c->piscina;

    si (f == NIHIL)
    {
        redde;
    }
    {
                 MateriaNodus* radix;
                 MateriaNodus* relecta;
                 MateriaNodus* html;
                 MateriaNodus* corpus;
                 MateriaNodus* formans;
                 MateriaNodus* exemplar;
                 MateriaNodus* bloccum;
        MateriaArborScriptura  s;
           MateriaArborVitium  vitium;

        /* '<b><a><div>x</b>y</a>': a' (exemplar a) liber corporis post
         * b; div (liber a) sedem corpus et praecedentem a' fert - post
         * relectionem referentiae in nodos RELECTOS spectant */
        radix = html_arbor_parsare(piscina, "<b><a><div>x</b>y</a>",
            (i32)XXI);
        CREDO_NON_NIHIL (radix);
        s = materia_arbor_scribere_nodum(piscina, radix, &f->consilium);
        CREDO_VERUM (s.successus);
        CREDO_VERUM (_textus_continet(s.textus, "<exemplar(> #nod"));
        CREDO_VERUM (_textus_continet(s.textus, "<praecedens(> #nod"));
        relecta = s.successus ? materia_arbor_legere(piscina, NIHIL,
            s.textus, &f->consilium, &vitium) : NIHIL;
        CREDO_NON_NIHIL (relecta);
        html = _liber(relecta, (i32)HTML_DOCUMENTUM_LIBERI,
            ZEPHYRUM);
        corpus = _liber(html, (i32)HTML_ELEMENTUM_LIBERI, (i32)I);
        formans  = _liber(_liber(corpus, (i32)HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM), (i32)HTML_ELEMENTUM_LIBERI, ZEPHYRUM);   /* a */
        exemplar = _liber(corpus, (i32)HTML_ELEMENTUM_LIBERI, (i32)I);
        bloccum = _liber(formans, (i32)HTML_ELEMENTUM_LIBERI,
            ZEPHYRUM);
        CREDO_NON_NIHIL (formans);
        CREDO_NON_NIHIL (exemplar);
        CREDO_NON_NIHIL (bloccum);
        CREDO_AEQUALIS_PTR (_referentia_elementi(exemplar,
            (i32)HTML_ELEMENTUM_EXEMPLAR), formans);
        CREDO_AEQUALIS_PTR (_sedes(bloccum), corpus);
        CREDO_AEQUALIS_PTR (_referentia_elementi(bloccum,
            (i32)HTML_ELEMENTUM_PRAECEDENS), exemplar);
    }
}


/* ==================================================
 * PROMOTUM: sequentia claudens propria (olim limes substrati)
 * ================================================== */

interior vacuum
_sequentiam_claudentem_probare (
    CredoContextus* c)
{
    constans FixuraStml* f = (constans FixuraStml*)c->suita;
                Piscina* piscina = c->piscina;
                    i32  sedes_elementa = ZEPHYRUM;
                    i32  sedes_derivata = ZEPHYRUM;

    si (f == NIHIL)
    {
        redde;
    }
    {
        /* TRES FORMAE TERMINATORIS, non una. Lector STML '</T'
         * cum '>', '!' AUT SPATIO sequente terminat; custos
         * scriptoris olim '</T>' SOLUM quaerebat, ergo formae duae
         * posteriores eum transibant et documentum dabant quod
         * lector noster parsare NON poterat ('STML parsari non
         * potuit', mensuratum 2026-09-19). Casus '!' et spatii hic
         * MANENT: scala fugae easdem tres formas ferre debet, et
         * scala uno delimitatore angustior eandem rimam pareret.
         *
         * Olim recusatio ipsa asserebatur cum nota 'RUBET CUM
         * MATERIA RECIDAT'. Recidit 2026-09-21; ergo circuitus
         * OCTETIM INTEGER asseritur - valor tacite mutatus
         * periculum primum plani est, ideo octeti, non sola
         * successio. */
        hic_manens constans character* PROMOTA[] = {
            "<script>var t = '</lex-textus-crudus>';</script>",
            "<!-- </lex-commentarium> -->",
            "<script>var t = '</lex-textus-crudus x';</script>",
            "<script>var t = '</lex-textus-crudus!';</script>",
            "<!-- </lex-commentarium -->"
        };
        i32 k;

        per (k = ZEPHYRUM; k < V; k++)
        {
            Circuitus circuitus = _circuitum_probare(piscina,
                &f->consilium, PROMOTA[k], (i32)strlen(PROMOTA[k]),
                NIHIL,
                FALSUM, &sedes_elementa, &sedes_derivata);

            CREDO_AEQUALIS_S32 ((s32)circuitus.causa,
                (s32)CIRCUITUS_IDEM);
        }
    }
}


/* ==================================================
 * PINNA SEPARATIONIS ORACULORUM: octeti idem, arbor dispar
 * ================================================== */

interior vacuum
_separationem_oraculorum_probare (
    CredoContextus* c)
{
    constans FixuraStml* f = (constans FixuraStml*)c->suita;
                    i32  sedes_elementa = ZEPHYRUM;
                    i32  sedes_derivata = ZEPHYRUM;

    si (f == NIHIL)
    {
        redde;
    }
    {
        Circuitus circuitus;

        /* arbor relecta laesa POST rescripturam: octeti STML bis idem
         * manent, comparator solus dissentit. Si haec assertio
         * viridis esset sine laesione, comparator non curreret. */
        circuitus = _circuitum_probare(c->piscina, &f->consilium,
            "<p>a</p><p>b</p>", (i32)XVI, NIHIL, VERUM, &sedes_elementa,
            &sedes_derivata);
        CREDO_AEQUALIS_S32 ((s32)circuitus.causa,
            (s32)CIRCUITUS_ARBOR_DISPAR);
        CREDO_NON_NIHIL (circuitus.differentia.campus);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "circuitus STML: casus inlinei, fragmenta, corpus",
        _cyclos_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "annotatio synthesis relecta", _synthesim_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "annotatio sedes relecta", _sedem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "annotationes exemplar et praecedens relectae",
        _exemplar_praecedentem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "sequentia claudens propria: circuitus",
        _sequentiam_claudentem_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "separatio oraculorum", _separationem_oraculorum_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere_cum("html_stml", SECTIONES,
        _suitam_parare, NIHIL);
}
