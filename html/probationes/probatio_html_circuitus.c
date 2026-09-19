/* probatio_html_circuitus.c - Corpus html5lib per legem octetorum et
 * circuitum STML (2026-09-15; Fran: 'are we doing round trip byte for
 * byte for the whole html5lib test corpus?')
 *
 * Oraculum (probatio_html_oraculum) visionem coctam solam confert;
 * lex octetorum (emissio directa == fons) et circuitus STML (scribere
 * -> legere -> scribere, comparator, emissio arboris RELECTAE == fons)
 * in paginis XIV et casibus inlineis solum currebant. Hic casus OMNES
 * corporis (script-on inclusi, fragmenta per aditum fragmenti cum
 * contextu suo), piscina sua quisque:
 *
 *  I.  OCTETI: emissio directa octetim == initus. PINNA: omnes.
 *  II. STML: cycli duo + comparator STRUCTURALIS + arbor relecta
 *      emissa octetim == initus. PINNA: idem == totalis -
 *      recusationes; recusationes per CAUSAM NOMINATAM pinnatae -
 *      limites substrati noti: NUL in lexemate ('valor lexematis NUL
 *      fert', 'valor trivii NUL fert') et sequentia claudens propria
 *      ('valor sequentiam claudentem fert', 01M2KJ3PK2). Causa alia =
 *      fractura. Cum materia recidat, pinnae recusationum rubent et
 *      casus in circuitum promoventur.
 *
 * Corpus adversarium densissimum quod habemus: markup malignum
 * MDCCVIII casibus - portae O7 (annotationes: synthesis, sedes,
 * exemplar) hic integrae probantur, non in casibus manu electis.
 */

#include "latina.h"
#include "credo.h"
#include "html_arbor.h"
#include "html_alienum.h"
#include "html_exempla.h"
#include "html_registrum.h"
#include "html_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* exempla iudicata: corpus totum (probatio_html_exempla pinna) */
#define TOTALIS_PINNATUS     ((i32)1708)
/* recusationes STML per causam, cursus primus 2026-09-15: NUL XXXVI
 * (plagulae 'unsafe' III; limes formae crudae 01M2KPJ0HW - cum
 * materia recidat, hae rubent et casus in circuitum promoventur),
 * sequentia claudens NULLA (corpus eam non fert), aliae NULLAE
 *
 * MATERIA RECESSIT 2026-09-19: attributum 'nul' offsets fert, ut 'cr'
 * ante eum - ergo XXXVI -> ZEPHYRUM, et casus omnes MDCCVIII per
 * circuitum plenum eunt. Quod commentarium supra praedixit factum
 * est; pinna PROMOTIONEM notat, non relaxationem. */
#define RECUSATA_NUL         ((i32)0)
#define RECUSATA_SEQUENTIA   ((i32)0)
#define FRACTAE_IMPRESSAE    ((i32)40)

/* Causae nominatae circuitus */
enumeratio {
    CIRCUITUS_IDEM = 0,
    CIRCUITUS_PARSATOR_NIHIL,
    CIRCUITUS_OCTETI_DISPARES_FONTI,
    CIRCUITUS_SCRIPTURA_RECUSATA_NUL,
    CIRCUITUS_SCRIPTURA_RECUSATA_SEQUENTIA,
    CIRCUITUS_SCRIPTURA_RECUSATA_ALIA,
    CIRCUITUS_LECTIO_RECUSATA,
    CIRCUITUS_RESCRIPTURA_RECUSATA,
    CIRCUITUS_STML_DISPARES,
    CIRCUITUS_ARBOR_DISPAR,
    CIRCUITUS_RELECTA_DISPAR_FONTI,
    CIRCUITUS_NUMERUS_CAUSARUM
};

hic_manens constans character* CAUSAE[] = {
    "idem",
    "parsator NIHIL",
    "emissio directa a fonte dispar",
    "scriptura STML recusata: NUL",
    "scriptura STML recusata: sequentia claudens",
    "scriptura STML recusata: causa ALIA",
    "lectio STML recusata",
    "rescriptura STML recusata",
    "octeti STML dispares inter cyclos",
    "arbor relecta dispar",
    "arbor relecta fontem non emittit"
};

nomen structura {
               integer  causa;
    constans character* nuntius;
} Circuitus;

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
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    si (lecti != (size_t)longitudo)
    {
        redde NIHIL;
    }
    *mensura = (i32)longitudo;
    redde memoria;
}

hic_manens constans character*
_titulus (
    constans character* via)
{
    constans character* t = strrchr(via, '/');

    redde t == NIHIL ? via : t + I;
}

hic_manens b32
_octeti_idem (
    chorda a,
    chorda b)
{
    redde (b32)(a.mensura == b.mensura
                && (a.mensura == ZEPHYRUM
                    || memcmp(a.datum, b.datum, (size_t)a.mensura)
                           == ZEPHYRUM));
}

/* Casus unus: parsare (documentum aut fragmentum), emissio directa,
 * circuitus STML. Ordo causarum = ordo inventionis: octeti ante
 * STML, ut fractura octetorum numquam post recusationem STML lateat. */
hic_manens Circuitus
_casum_probare (
                            Piscina* piscina,
     constans MateriaArborConsilium* consilium,
              constans HtmlExemplum* e)
{
                    Circuitus c;
                 MateriaNodus* radix;
                 MateriaNodus* relecta;
    MateriaScripturaConsilium  cs;
             MateriaScriptura  emissa;
        MateriaArborScriptura  s1;
        MateriaArborScriptura  s2;
           MateriaArborVitium  vitium;
      MateriaArborDifferentia  differentia;

    c.causa    = CIRCUITUS_IDEM;
    c.nuntius  = NIHIL;

    si (e->fragmentum)
    {
        HtmlAlienum spatium;
             chorda titulus;

        html_exempla_contextus(piscina, e->contextus, &spatium,
            &titulus);
        radix = html_arbor_parsare_fragmentum(piscina,
            (constans character*)e->datum.datum, e->datum.mensura,
            titulus, spatium);
    }
    alioquin
    {
        radix = html_arbor_parsare(piscina,
            (constans character*)e->datum.datum, e->datum.mensura);
    }
    si (radix == NIHIL)
    {
        c.causa = CIRCUITUS_PARSATOR_NIHIL;
        redde c;
    }

    /* I. lex octetorum */
    materia_scriptura_consilium_nudum(&cs, &HTML_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &cs);
    si (!emissa.successus || !_octeti_idem(emissa.textus, e->datum))
    {
        c.causa = CIRCUITUS_OCTETI_DISPARES_FONTI;
        redde c;
    }

    /* II. circuitus STML */
    s1 = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s1.successus)
    {
        c.nuntius = s1.causa;
        si (s1.causa != NIHIL && strstr(s1.causa, "NUL") != NIHIL)
        {
            c.causa = CIRCUITUS_SCRIPTURA_RECUSATA_NUL;
        }
        alioquin si (   s1.causa != NIHIL
                     && strcmp(s1.causa,
                            "valor sequentiam claudentem fert")
                            == ZEPHYRUM)
        {
            c.causa = CIRCUITUS_SCRIPTURA_RECUSATA_SEQUENTIA;
        }
        alioquin
        {
            c.causa = CIRCUITUS_SCRIPTURA_RECUSATA_ALIA;
        }
        redde c;
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
    si (!_octeti_idem(s1.textus, s2.textus))
    {
        c.causa = CIRCUITUS_STML_DISPARES;
        redde c;
    }
    si (!materia_arbor_aequalis(radix, relecta,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &differentia))
    {
        c.causa    = CIRCUITUS_ARBOR_DISPAR;
        c.nuntius  = differentia.campus;
        redde c;
    }
    emissa = materia_scribere_nodum(piscina, relecta, &cs);
    si (!emissa.successus || !_octeti_idem(emissa.textus, e->datum))
    {
        c.causa = CIRCUITUS_RELECTA_DISPAR_FONTI;
        redde c;
    }
    redde c;
}

integer
principale (vacuum)
{
                Piscina* piscina;
     constans character* radix_viae;
              character  via[DXII];
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;
  MateriaArborConsilium  consilium;
                    i32  summae[CIRCUITUS_NUMERUS_CAUSARUM];
                    i32  totalis            = ZEPHYRUM;
                    i32  fragmenta          = ZEPHYRUM;
                    i32  fractae_impressae  = ZEPHYRUM;
                    i32  f;
                    i32  k;

    piscina = piscina_generare_dynamicum("probatio_html_circuitus",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    si (!materia_lexicon_ratum_facere(&ratum, &HTML_LEXICON, &iudicium))
    {
        imprimere("LEXICON HTML RECUSATUM\n");
        redde I;
    }
    materia_arbor_consilium_nudum(&consilium, &HTML_REGISTRUM, &ratum,
        "html");
    per (k = ZEPHYRUM; k < (i32)CIRCUITUS_NUMERUS_CAUSARUM; k++)
    {
        summae[k] = ZEPHYRUM;
    }

    imprimere("\n--- CIRCUITUS: corpus html5lib per octetos et STML "
              "---\n");
    per (f = ZEPHYRUM; HTML_EXEMPLA_PLAGULAE[f] != NIHIL; f++)
    {
        character* textus;
              i32  mensura = ZEPHYRUM;
              Xar* exempla;
              i32  n;
              i32  i;

        sprintf(via, "%s/%s", radix_viae, HTML_EXEMPLA_PLAGULAE[f]);
        textus = _plagulam_legere(piscina, via, &mensura);
        CREDO_NON_NIHIL (textus);
        si (textus == NIHIL)
        {
            perge;
        }
        exempla = html_exempla_legere(piscina, textus, mensura);
        CREDO_NON_NIHIL (exempla);
        si (exempla == NIHIL)
        {
            perge;
        }
        n = (i32)xar_numerus(exempla);
        per (i = ZEPHYRUM; i < n; i++)
        {
            HtmlExemplum* e = (HtmlExemplum*)xar_obtinere(exempla, i);
                 Piscina* p;
               Circuitus  c;

            si (e == NIHIL)
            {
                perge;
            }
            totalis = totalis + I;
            si (e->fragmentum)
            {
                fragmenta = fragmenta + I;
            }
            p = piscina_generare_dynamicum("casus", 4194304);
            si (p == NIHIL)
            {
                CREDO_CULPA ("piscina casus");
                perge;
            }
            c                = _casum_probare(p, &consilium, e);
            summae[c.causa]  = summae[c.causa] + I;
            si (   c.causa != CIRCUITUS_IDEM
                && c.causa != CIRCUITUS_SCRIPTURA_RECUSATA_NUL
                && c.causa != CIRCUITUS_SCRIPTURA_RECUSATA_SEQUENTIA
                && fractae_impressae < FRACTAE_IMPRESSAE)
            {
                fractae_impressae = fractae_impressae + I;
                imprimere("  %s #%d%s: %s%s%s\n",
                    _titulus(HTML_EXEMPLA_PLAGULAE[f]),
                    (integer)e->numerus,
                    e->fragmentum ? " [fr]" : "",
                    CAUSAE[c.causa],
                    c.nuntius != NIHIL ? " - " : "",
                    c.nuntius != NIHIL ? c.nuntius : "");
            }
            piscina_destruere(p);
        }
    }

    imprimere("\n--- per causam ---\n");
    per (k = ZEPHYRUM; k < (i32)CIRCUITUS_NUMERUS_CAUSARUM; k++)
    {
        si (summae[k] > ZEPHYRUM)
        {
            imprimere("  %5d  %s\n", (integer)summae[k], CAUSAE[k]);
        }
    }
    imprimere("--- CIRCUITUS: %d casus (%d fragmenta); octeti idem "
              "%d; STML idem %d, recusata NUL %d, sequentia %d ---\n",
        (integer)totalis, (integer)fragmenta,
        (integer)(totalis - summae[CIRCUITUS_PARSATOR_NIHIL]
                  - summae[CIRCUITUS_OCTETI_DISPARES_FONTI]),
        (integer)summae[CIRCUITUS_IDEM],
        (integer)summae[CIRCUITUS_SCRIPTURA_RECUSATA_NUL],
        (integer)summae[CIRCUITUS_SCRIPTURA_RECUSATA_SEQUENTIA]);

    /* PINNAE */
    CREDO_AEQUALIS_I32 (totalis, TOTALIS_PINNATUS);
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_PARSATOR_NIHIL], ZEPHYRUM);
    /* I. lex octetorum: omnes */
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_OCTETI_DISPARES_FONTI],
        ZEPHYRUM);
    /* II. STML: recusationes nominatae EXACTE, aliae nullae, ceteri
     * idem */
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_SCRIPTURA_RECUSATA_NUL],
        RECUSATA_NUL);
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_SCRIPTURA_RECUSATA_SEQUENTIA],
        RECUSATA_SEQUENTIA);
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_SCRIPTURA_RECUSATA_ALIA],
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_LECTIO_RECUSATA], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_RESCRIPTURA_RECUSATA],
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_STML_DISPARES], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_ARBOR_DISPAR], ZEPHYRUM);
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_RELECTA_DISPAR_FONTI],
        ZEPHYRUM);
    CREDO_AEQUALIS_I32 (summae[CIRCUITUS_IDEM],
        TOTALIS_PINNATUS - RECUSATA_NUL - RECUSATA_SEQUENTIA);

    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
