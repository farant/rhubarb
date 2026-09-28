/* probatio_toml_stml.c - Circuitus STML toml: duo cycli, duo oracula
 *
 * scribere -> legere -> scribere: octeti STML bis idem (vitium quod se
 * componit circuitum unum saepe superat - stml semantica, lex M); arbor
 * parsata contra relectam per COMPARATOREM (materia_arbor_aequalis,
 * STRUCTURALIS - dislocatio dominii triviorum octetim invisibilis est,
 * solus comparator eam videt; deinde FIDELITAS: positiones); arbor
 * RELECTA per scriptorem octetorum emissa == fons (catena clausa);
 * sedes verificatae (materia_sedes_verificare). Casus: fontes portae
 * arboris (Q4) omnes, deinde corpora tota (toml-test valida ET
 * invalida, domus, silvestria).
 *
 * Recusatio scriptoris causa nominata limes est (pinnandus), non
 * fractura; causa ALIA fractura est. Pinna separationis oraculorum:
 * cauda documenti relecti sublata -> octeti bis idem, arbor dispar.
 */

#include "latina.h"
#include "credo.h"
#include "toml_arbor.h"
#include "toml_lexicon.h"
#include "toml_registrum.h"
#include "toml_corpus_ambulare.h"
#include "materia_arbor.h"
#include "materia_sedes.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* fontes portae arboris (Q4), verbatim */
hic_manens constans character* CASUS[] = {
    "a = 1\n", "", "[a.b]\nc = 1\n", "[[p]]\nx = 1\n[[p]]\nx = 2\n",
    "[ a . \"b\" ]",
    "s = \"x\"\ni = 1\nf = 1.5\nb = true\nt = 1979-05-27\nl = 'x'\n"
        "m = '''x'''\n",
    "a = [1, [2, 3], {x = 1}]\n", "a = [\n  1, # c\n  2,\n]\n",
    "t = { a = 1, b.c = 2 }\n", "t = {}\ns = []\n",
    "t = {a = 1,\n b = 2}\n", "# c\na = 1\n", "a = 1 # c\n",
    "a = 1\n\n# c\nb = 2\n", "a = [1, # c\n 2]\n", "a = 1\nb = 2\n",
    "a = \nb = 2\n", "a 1\nb = 2\n", "a..b = 1\n", "= 1\n", "]]\n",
        "}\n",
    "a = 1\n= x\nb = 2\n]]\nc = 3\n}\nd = 4\n", "a = hello\nb = 2\n",
    "a = 1 2\n", "a = [1 2]\n", "a = [1, 2", "a = \"abc",
        "a = \"abc\\\"",
    "a = '''x", "[a", "t = {a = 1", "a = [[[[",
    "a = 1\r\n", "a = [\r\n  1, # c\r\n  2,\r\n]\r\n",
    "\xef\xbb\xbf" "a = 1\n",
    /* chordae et tempora quae STML fugare debet */
    "s = \"</toml-chorda-gemina>\"\n", "s = \"a\\u00e9\"\n",
    "c = 'x' # </toml-commentum>\n", "d = 1979-05-27 07:32:00Z\n",
    "m = \"\"\"\nline\n  indent\n\"\"\"\n"
};

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

hic_manens MateriaArborConsilium CONSILIUM;

/* Duo cycli + comparator + emissio relectae. 'mutare': cauda documenti
 * relecti ANTE comparationem sublata (oraculum separans). */
interior Circuitus
_circuitum_probare (
               Piscina* piscina,
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
              TomlParsura  r;

    memset(&c, ZEPHYRUM, magnitudo(Circuitus));
    c.causa  = CIRCUITUS_IDEM;
    radix    = toml_arbor_parsare(piscina, fons, mensura, &r);
    si (radix == NIHIL)
    {
        c.causa = CIRCUITUS_PARSATOR_NIHIL;
        redde c;
    }
    s1 = materia_arbor_scribere_nodum(piscina, radix, &CONSILIUM);
    si (!s1.successus)
    {
        c.causa    = CIRCUITUS_SCRIPTURA_RECUSATA;
        c.nuntius  = s1.causa;
        redde c;
    }
    c.octeti_stml = s1.textus.mensura;
    si (   !materia_sedes_verificare(piscina, radix, &CONSILIUM, fons,
            (i32)mensura, &sedes) || !sedes.sana)
    {
        c.causa    = CIRCUITUS_SEDES_INSANAE;
        c.nuntius  = sedes.causa;
        redde c;
    }
    relecta = materia_arbor_legere(piscina, NIHIL, s1.textus,
        &CONSILIUM,
        &vitium);
    si (relecta == NIHIL)
    {
        c.causa    = CIRCUITUS_LECTIO_RECUSATA;
        c.nuntius  = vitium.causa;
        redde c;
    }
    s2 = materia_arbor_scribere_nodum(piscina, relecta, &CONSILIUM);
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

        materia_scriptura_consilium_nudum(&cs, &TOML_REGISTRUM);
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
        relecta->loci[TOML_DOCUMENTUM_CAUDA] = materia_valor_nihil();
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

nomen structura {
    Summa fontes[TOML_CORPUS_NUMERUS_FONTIUM];
      i32 nominati;       /* causae impressae (ad X) */
} Status;

interior vacuum
_visor (
                 vacuum* datum,
         TomlCorpusFons  fons,
     constans character* via,
                 chorda  textus,
                Piscina* opus)
{
                Status* st  = (Status*)datum;
                 Summa* s   = &st->fontes[fons];
             Circuitus  c;
    constans character* f = textus.datum != NIHIL
        ? (constans character*)textus.datum : "";

    c = _circuitum_probare(opus, f, (s32)textus.mensura, FALSUM);
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

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
                Piscina* opus;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;
     constans character* radix = getenv("RHUBARB_RADIX");

    piscina = piscina_generare_dynamicum("probatio_toml_stml", 262144);
    opus = piscina_generare_dynamicum("probatio_toml_stml_opus",
        4194304);
    si (!piscina || !opus)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &TOML_LEXICON,
        &iudicium));
    materia_arbor_consilium_nudum(&CONSILIUM, &TOML_REGISTRUM, &ratum,
        "toml");


    /* ==================================================
     * PORTA: casus inlinei
     * ================================================== */

    {
        i32 k;
        i32 n = (i32)(magnitudo(CASUS) / magnitudo(CASUS[0]));

        imprimere("\n--- PORTA: circuitus STML, casus (%d) ---\n",
            (integer)n);
        per (k = ZEPHYRUM; k < n; k++)
        {
                  i32  l = (i32)strlen(CASUS[k]);
            character* f = (character*)piscina_allocare(opus,
                (i64)(l + I));
            Circuitus c;

            memcpy(f, CASUS[k], (size_t)(l + I));
            c = _circuitum_probare(opus, f, (s32)l, FALSUM);
            _causam_imprimere(CASUS[k], &c);
            CREDO_AEQUALIS_S32 ((s32)c.causa, (s32)CIRCUITUS_IDEM);
            piscina_vacare(opus);
        }
    }


    /* ==================================================
     * PORTA: corpora tota
     * ================================================== */

    {
                    Status  st;
          TomlCorpusNumeri  nn;
                       i32  f;
        constans character* TITULI[] = { "toml-test", "domus",
            "silvestria" };

        imprimere("\n--- PORTA: circuitus STML, corpora ---\n");
        memset(&st, ZEPHYRUM, magnitudo(st));
        toml_corpus_ambulare(piscina, opus, radix
            != NIHIL ? radix : ".",
            _visor, &st, &nn);
        CREDO_VERUM (nn.indices_lecti);
        per (f = ZEPHYRUM; f < (i32)TOML_CORPUS_NUMERUS_FONTIUM; f++)
        {
            Summa* s = &st.fontes[f];

            imprimere("  %s: %u plagulae, %u octeti -> %u STML, "
                "idem %u\n",
                TITULI[f], s->plagulae, s->octeti, s->octeti_stml,
                s->per_causam[CIRCUITUS_IDEM]);
            CREDO_MAIOR_I32 (s->plagulae, ZEPHYRUM);
            CREDO_AEQUALIS_I32 (s->per_causam[CIRCUITUS_IDEM],
                s->plagulae);
        }
        CREDO_AEQUALIS_I32 (st.fontes[TOML_CORPUS_TOML_TEST].plagulae,
            (i32)(CCV + CDLXXIV));
    }


    /* ==================================================
     * PINNA SEPARATIONIS ORACULORUM: octeti idem, arbor dispar
     * ================================================== */

    {
        character* f = (character*)piscina_allocare(opus, (i64)VIII);
        Circuitus  c;

        imprimere("\n--- Probans separationem oraculorum ---\n");
        memcpy(f, "a = 1\n", VII);
        c = _circuitum_probare(opus, f, (s32)VI, VERUM);
        CREDO_AEQUALIS_S32 ((s32)c.causa, (s32)CIRCUITUS_ARBOR_DISPAR);
        CREDO_NON_NIHIL (c.differentia.campus);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(opus);
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
