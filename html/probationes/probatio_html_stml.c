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
  * LIMES SUBSTRATI NOMINATUS (inventus H6, 2026-09-15): valor
 * lexematis qui sequentiam claudentem elementi SUI continet
 * ('</lex-textus-crudus>' intra textum crudum scripti,
 * '</lex-commentarium>' intra commentarium) a scriptore RECUSATUR
 * ('valor sequentiam claudentem fert'): forma cruda eum ferre nequit
 * et scriptor ad textum fugatum NON recidit. In textu nudo idem
 * '</lex-textus>' innocuus est quia lexator HTML eum in tag clausurae
 * scindit. md valores manu scidit; html lexema scindere nequit
 * (dominus unus). PINNA infra: recusatio ipsa asseritur - cum
 * materia recidat, pinna rubet et in circuitum promovetur. Ledger
 * quaestio materiae.
 */

#include "latina.h"
#include "credo.h"
#include "html_arbor.h"
#include "html_registrum.h"
#include "html_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
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
    "knotapel/atlas/web/index.html",
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
    "<div\n"
};

#define NUMERUS_CASUUM ((i32)(magnitudo(CASUS)/magnitudo(CASUS[0])))

/* Causae nominatae */
enumeratio {
    CIRCUITUS_IDEM = 0,
    CIRCUITUS_PARSATOR_NIHIL,
    CIRCUITUS_SCRIPTURA_RECUSATA,
    CIRCUITUS_LECTIO_RECUSATA,
    CIRCUITUS_RESCRIPTURA_RECUSATA,
    CIRCUITUS_OCTETI_DISPARES,
    CIRCUITUS_ARBOR_DISPAR,
    CIRCUITUS_COMPRESSIO_VIVA
};

hic_manens constans character* CAUSAE[] = {
    "idem",
    "parsator NIHIL reddidit",
    "scriptura recusata",
    "lectio recusata",
    "rescriptura recusata",
    "octeti STML dispares inter cyclos",
    "arbor relecta dispar",
    "compressio templorum viva"
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

    f = fopen(via, "rb");
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

/* nuntius = causa scriptoris aut lectoris (NIHIL si nulla) */
nomen structura {
                    integer  causa;
         constans character* nuntius;
    MateriaArborDifferentia  differentia;
                        i32  octeti_stml;
} Circuitus;

/* Duo cycli + comparator. 'mutare' = culpa plantata probationis:
 * arborem relectam ANTE comparationem laedere (octeti iam scripti)
 * - oraculum separans: octeti idem, arbor dispar. */
hic_manens Circuitus
_circuitum_probare (
                            Piscina* piscina,
     constans MateriaArborConsilium* consilium,
                 constans character* fons,
                                i32  mensura,
                                b32  mutare)
{
    Circuitus c;
    MateriaNodus* radix;
    MateriaNodus* relecta;
    MateriaArborScriptura s1;
    MateriaArborScriptura s2;
    MateriaArborVitium vitium;

    memset(&c, ZEPHYRUM, magnitudo(Circuitus));
    c.causa = CIRCUITUS_IDEM;

    radix = html_arbor_parsare(piscina, fons, mensura);
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
    { c.causa = CIRCUITUS_OCTETI_DISPARES; redde c;
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

integer
principale (vacuum)
{
                Piscina* piscina;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;
  MateriaArborConsilium  consilium;
                    b32  praeteritus;
     constans character* radix_viae;
                    i32  i;
                    i32  circuitus_numerus  = ZEPHYRUM;
                    i32  octeti_fontis      = ZEPHYRUM;
                    i32  octeti_stml        = ZEPHYRUM;

    piscina = piscina_generare_dynamicum("probatio_html_stml", 65536);
    credo_aperire(piscina);

    si (!materia_lexicon_ratum_facere(&ratum, &HTML_LEXICON, &iudicium))
    {
        imprimere("LEXICON HTML RECUSATUM\n");
        redde I;
    }
    materia_arbor_consilium_nudum(&consilium, &HTML_REGISTRUM, &ratum,
        "html");


    /* ==================================================
     * PORTA: casus inlinei - duo cycli + comparator
     * ================================================== */

    imprimere("\n--- PORTA: circuitus STML, casus inlinei (%d) ---\n",
        (integer)NUMERUS_CASUUM);
    per (i = ZEPHYRUM; i < NUMERUS_CASUUM; i++)
    {
        Circuitus c = _circuitum_probare(piscina, &consilium, CASUS[i],
            (i32)strlen(CASUS[i]), FALSUM);

        si (c.causa != CIRCUITUS_IDEM)
        {
            imprimere("  casus %d: %s\n", (integer)i, CASUS[i]);
            _causam_imprimere(&c);
        }
        CREDO_AEQUALIS_S32 ((s32)c.causa, (s32)CIRCUITUS_IDEM);
        circuitus_numerus = circuitus_numerus + I;
    }


    /* ==================================================
     * PORTA: corpus totum
     * ================================================== */

    imprimere("\n--- PORTA: circuitus STML, corpus (%d) ---\n",
        (integer)NUMERUS_CORPORIS);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    { radix_viae = ".";
    }
    per (i = ZEPHYRUM; i < NUMERUS_CORPORIS; i++)
    {
        character  via[DXII];
        character* textus;
              i32  mensura = ZEPHYRUM;
        Circuitus  c;

        sprintf(via, "%s/%s", radix_viae, CORPUS[i]);
        textus = _plagulam_legere(piscina, via, &mensura);
        si (textus == NIHIL)
        {
            imprimere("  ABEST: %s\n", CORPUS[i]);
            CREDO_CULPA ("plagula corporis abest");
            perge;
        }
        c = _circuitum_probare(piscina, &consilium, textus, mensura,
            FALSUM);
        imprimere("  %-56s %7d -> %8d octeti STML  %s\n", CORPUS[i],
            (integer)mensura, (integer)c.octeti_stml, CAUSAE[c.causa]);
        _causam_imprimere(&c);
        CREDO_AEQUALIS_S32 ((s32)c.causa, (s32)CIRCUITUS_IDEM);
        circuitus_numerus  = circuitus_numerus + I;
        octeti_fontis      = octeti_fontis + mensura;
        octeti_stml        = octeti_stml + c.octeti_stml;
    }

    /* se metiens */
    CREDO_AEQUALIS_I32 (circuitus_numerus, NUMERUS_CASUUM
        + NUMERUS_CORPORIS);
    CREDO_MAIOR_I32 (octeti_fontis, (i32)(C * M));
    CREDO_MAIOR_I32 (octeti_stml, octeti_fontis);
    imprimere("  summa: %d circuitus, %d octeti fontis, %d STML\n",
        (integer)circuitus_numerus, (integer)octeti_fontis,
        (integer)octeti_stml);


    /* ==================================================
     * PINNA LIMITIS SUBSTRATI: valor sequentiam claudentem fert
     * ================================================== */

    {
        hic_manens constans character* LIMITES[] = {
            "<script>var t = '</lex-textus-crudus>';</script>",
            "<!-- </lex-commentarium> -->"
        };
        i32 k;

        imprimere("\n--- Pinna: sequentia claudens propria ---\n");
        per (k = ZEPHYRUM; k < II; k++)
        {
            Circuitus c = _circuitum_probare(piscina, &consilium,
                LIMITES[k], (i32)strlen(LIMITES[k]), FALSUM);

            /* RUBET CUM MATERIA RECIDAT - tunc in CASUS promovendi */
            CREDO_AEQUALIS_S32 ((s32)c.causa,
                (s32)CIRCUITUS_SCRIPTURA_RECUSATA);
            CREDO_NON_NIHIL (c.nuntius);
            si (c.nuntius != NIHIL)
            {
                CREDO_VERUM (strcmp(c.nuntius,
                    "valor sequentiam claudentem fert") == ZEPHYRUM);
            }
        }
    }


    /* ==================================================
     * PINNA SEPARATIONIS ORACULORUM: octeti idem, arbor dispar
     * ================================================== */

    {
        Circuitus c;

        imprimere("\n--- Probans separationem oraculorum ---\n");
        /* arbor relecta laesa POST rescripturam: octeti STML bis idem
         * manent, comparator solus dissentit. Si haec assertio
         * viridis esset sine laesione, comparator non curreret. */
        c = _circuitum_probare(piscina, &consilium, "<p>a</p><p>b</p>",
            (i32)XVI, VERUM);
        CREDO_AEQUALIS_S32 ((s32)c.causa, (s32)CIRCUITUS_ARBOR_DISPAR);
        CREDO_NON_NIHIL (c.differentia.campus);
    }


    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
