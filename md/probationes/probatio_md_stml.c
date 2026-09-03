/* probatio_md_stml.c - Circuitus proiectionis STML (A8, spec par. VII)
 *
 * Scribere -> legere -> scribere BIS (vitium quod se componit
 * circuitum unum saepe superat). ORACULA: octeti (emissio ex arbore
 * lecta contra fontem), comparator STRUCTURALIS et - quod CSS non
 * poterat - FIDELIS: markdown munus LINEA fert, ergo lector positiones
 * ex cursore reficit. Lexemata DERIVATA (fons I) per uncum originis
 * portantur (f="1", linea/columna) et cursorem non movent.
 */

#include "latina.h"
#include "credo.h"
#include "md_arbor.h"
#include "md_stml.h"
#include "md_registrum.h"
#include "md_lexicon.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_lexicon.h"
#include "materia_token.h"
#include "chorda.h"
#include "piscina.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior character*
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

/* Circuitus plenus. VERUM = omnia sana; causa imprimitur ante FALSUM. */
interior b32
_circuitum_probare (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura,
                               b32  fidelis)
{
              MateriaNodus* radix;
              MateriaNodus* lecta;
              MateriaNodus* relecta;
      MateriaArborScriptura s1;
      MateriaArborScriptura s2;
      MateriaArborScriptura s3;
         MateriaArborVitium vitium;
    MateriaArborDifferentia d;

    radix = md_arbor_parsare(piscina, fons, mensura);
    si (radix == NIHIL)
    {
        imprimere("    (parsura NIHIL)\n");
        redde FALSUM;
    }
    s1 = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s1.successus)
    {
        imprimere("    (scriptura I: %s)\n", s1.causa ? s1.causa : "-");
        redde FALSUM;
    }
    lecta = materia_arbor_legere(piscina, NIHIL, s1.textus, consilium,
        &vitium);
    si (lecta == NIHIL)
    {
        imprimere("    (lectio I: %s, linea %d)\n",
            vitium.causa ? vitium.causa : "-", (integer)vitium.linea);
        redde FALSUM;
    }
    s2 = materia_arbor_scribere_nodum(piscina, lecta, consilium);
    si (   !s2.successus || s1.textus.mensura != s2.textus.mensura
        || memcmp(s1.textus.datum, s2.textus.datum,
        (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        imprimere("    (documenta I/II dispari: %d contra %d)\n",
            (integer)s1.textus.mensura, (integer)s2.textus.mensura);
        redde FALSUM;
    }
    relecta = materia_arbor_legere(piscina, NIHIL, s2.textus, consilium,
        &vitium);
    si (relecta == NIHIL)
    {
        imprimere("    (lectio II: %s)\n",
            vitium.causa ? vitium.causa : "-");
        redde FALSUM;
    }
    s3 = materia_arbor_scribere_nodum(piscina, relecta, consilium);
    si (   !s3.successus || s1.textus.mensura != s3.textus.mensura
        || memcmp(s1.textus.datum, s3.textus.datum,
        (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        imprimere("    (circuitus II dispar)\n");
        redde FALSUM;
    }
    si (!materia_arbor_aequalis(radix, lecta,
        MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &d))
    {
        imprimere("    (arbor dispar: %s @ %s)\n",
            d.campus ? d.campus : "-", d.via);
        redde FALSUM;
    }
    si (   fidelis && !materia_arbor_aequalis(radix, lecta,
            MATERIA_ARBOR_COMPARATIO_FIDELITAS, &d))
    {
        imprimere("    (fidelitas dispar: %s @ %s",
            d.campus ? d.campus : "-", d.via);
        si (d.lexema_a != NIHIL && d.lexema_b != NIHIL)
        {
            imprimere(" - a %d:%d b %d:%d", (integer)d.lexema_a->linea,
                (integer)d.lexema_a->columna,
                (integer)d.lexema_b->linea,
                (integer)d.lexema_b->columna);
        }
        imprimere(")\n");
        redde FALSUM;
    }
    {
                 MateriaScriptura emissa;
        MateriaScripturaConsilium cs;

        materia_scriptura_consilium_nudum(&cs, &MD_REGISTRUM);
        cs.fons_index  = MD_FONS_PLAGULAE;
        emissa         = materia_scribere_nodum(piscina, lecta, &cs);
        si (   !emissa.successus || emissa.textus.mensura != mensura
            || (mensura > ZEPHYRUM
                && memcmp(emissa.textus.datum, fons, (size_t)mensura)
                    != ZEPHYRUM))
        {
            imprimere("    (lecta->octeti dispari)\n");
            redde FALSUM;
        }
    }
    redde VERUM;
}

integer
principale (vacuum)
{
                  Piscina* piscina;
       constans character* radix_viae;
      MateriaLexiconRatum  ratum;
       MateriaLexIudicium  iudicium;
    MateriaArborConsilium  consilium;
                      b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_md_stml", 4194304);
    credo_aperire(piscina);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    si (!materia_lexicon_ratum_facere(&ratum, &MD_LEXICON, &iudicium))
    {
        imprimere("LEXICON MD RECUSATUM\n");
        redde I;
    }
    md_stml_consilium(&consilium, &ratum);


    /* ========================================================
     * CASUS INLINEI: quaeque forma nodi per proiectionem
     * ======================================================== */

    {
        hic_manens constans character* CASUS[] = {
            "# Titulus *em* `c`\n",
            "para one\ncontinued  \nhard\\\nbreak\n",
            "Setext\n===\n",
            "---\n",
            "```c\nint x;\n```\n",
            "    code\n\n    more\n",
            "<div>\nx\n</div>\n",
            "---\nname: x\n---\nbody\n",
            "- a\n  - b\n\n- c\n",
            "1) [ ] task\n2) [x] done\n",
            "> quote\n> - item\n",
            "| a | b |\n| :-- | --: |\n| 1 |\n",
            "[foo]: /url \"t\"\n\n[foo] [bar][foo] ![i](/p) <http://x.y> www.x.com\n",
            "\\*esc\\* &amp; ~~del~~ **strong** _em_\n",
            "a\r\nb\r\n",
            "",
            "\n\n"
        };
        i32 numerus = (i32)(magnitudo(CASUS) / magnitudo(CASUS[0]));
        i32 i;

        imprimere("\n--- Probans circuitum STML inlineum (structuralis + fidelis) ---\n");
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            CREDO_VERUM (_circuitum_probare(piscina, &consilium,
                CASUS[i],
                (i32)strlen(CASUS[i]), VERUM));
        }
    }


    /* ========================================================
     * DERIVATA in proiectione: f="1", positio portata, cursor immotus
     * ======================================================== */

    {
        constans character* f = "[foo]: /u\n\n[foo] \\*x\\*\n";
              MateriaNodus* radix = md_arbor_parsare(piscina, f,
                  (i32)strlen(f));
        MateriaArborScriptura s;

        imprimere("\n--- Probans derivata in proiectione ---\n");
        CREDO_NON_NIHIL (radix);
        s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
        CREDO_VERUM (s.successus);
        CREDO_NON_NIHIL (strstr((character*)s.textus.datum, "f=\"1\""));
        CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
            "<md-derivatum"));
        CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
            "grammatica=\"md\""));
        CREDO_NIHIL (strstr((character*)s.textus.datum, "<ante>"));   /* trivia nulla */
    }


    /* ========================================================
     * SEDES per proiectionem: derivatum positionem PORTATAM servat,
     * cursor lectoris eo non promovetur (lexema fontis sequens sedem
     * eandem habet quam in parsura)
     * ======================================================== */

    {
        constans character* f = "[foo]: /url\n\n[foo] b\n";
              MateriaNodus* radix = md_arbor_parsare(piscina, f,
                  (i32)strlen(f));
        MateriaArborScriptura s;
           MateriaArborVitium vitium;
        MateriaNodus* lecta;
        MateriaNodus* pa;
        MateriaNodus* pb;
        MateriaNodus* na;
        MateriaNodus* nb;
        constans MateriaToken* ua;
        constans MateriaToken* ub;
        constans MateriaToken* ta;
        constans MateriaToken* tb;

        imprimere("\n--- Probans sedes derivatorum per proiectionem ---\n");
        CREDO_NON_NIHIL (radix);
        s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
        CREDO_VERUM (s.successus);
        lecta = materia_arbor_legere(piscina, NIHIL, s.textus,
            &consilium, &vitium);
        CREDO_NON_NIHIL (lecta);
        /* paragraphus = blocus III (definitio, vacua, paragraphus) */
        pa =
            materia_valor_lista_obtinere(radix->loci[MD_DOCUMENTUM_BLOCI],
            II)->datum.nodus;
        pb =
            materia_valor_lista_obtinere(lecta->loci[MD_DOCUMENTUM_BLOCI],
            II)->datum.nodus;
        CREDO_AEQUALIS_S32 (pa->genus, (s32)MD_GENUS_PARAGRAPHUS);
        CREDO_AEQUALIS_S32 (pb->genus, (s32)MD_GENUS_PARAGRAPHUS);
        na =
            materia_valor_lista_obtinere(pa->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus
            ->loci[MD_INLINEA_LIBERI], ZEPHYRUM)->datum.nodus;
        nb =
            materia_valor_lista_obtinere(pb->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus
            ->loci[MD_INLINEA_LIBERI], ZEPHYRUM)->datum.nodus;
        CREDO_AEQUALIS_S32 (na->genus, (s32)MD_GENUS_NEXUS);
        ua = na->loci[MD_NEXUS_URL].datum.token;
        ub = nb->loci[MD_NEXUS_URL].datum.token;
        CREDO_AEQUALIS_S32 (ua->fons_index, MD_FONS_DERIVATUS);
        CREDO_AEQUALIS_S32 (ub->fons_index, MD_FONS_DERIVATUS);
        CREDO_AEQUALIS_S32 (ub->byte_offset, (s32)-I);     /* octetos nullos tenet */
        CREDO_AEQUALIS_I32 (ub->linea, ua->linea);           /* sedes portata */
        CREDO_AEQUALIS_I32 (ub->columna, ua->columna);
        /* textus ' b' post nexum: sedes eadem utrimque - cursor derivato
         * NON promotus */
        ta = materia_valor_lista_obtinere(materia_valor_lista_obtinere(
            pa->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus->loci[MD_INLINEA_LIBERI],
            I)
            ->datum.nodus->loci[MD_TEXTUS_CRUDUM],
            ZEPHYRUM)->datum.token;
        tb = materia_valor_lista_obtinere(materia_valor_lista_obtinere(
            pb->loci[MD_PARAGRAPHUS_INLINEA].datum.nodus->loci[MD_INLINEA_LIBERI],
            I)
            ->datum.nodus->loci[MD_TEXTUS_CRUDUM],
            ZEPHYRUM)->datum.token;
        CREDO_AEQUALIS_S32 (tb->byte_offset, ta->byte_offset);
        CREDO_AEQUALIS_I32 (tb->linea, ta->linea);
        CREDO_AEQUALIS_I32 (tb->columna, ta->columna);
        CREDO_AEQUALIS_S32 (ta->byte_offset, (s32)18);   /* "[foo]: /url\n\n[foo]" = XVIII */
    }


    /* ========================================================
     * CORPUS TOTUM per circuitum, se metiens
     * ======================================================== */

    {
        character via[DXII];
             FILE* lista;
              i32  plagulae = ZEPHYRUM;
              i32  fractae  = ZEPHYRUM;
              s64  octeti   = ZEPHYRUM;

        imprimere("\n--- Probans corpus per circuitum STML ---\n");
        sprintf(via, "%s/md/build/corpus_md.txt", radix_viae);
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("md/build/corpus_md.txt absens - e radice per cursorem curre");
        }
        alioquin
        {
            character linea[DXII];

            dum (fgets(linea, (integer)magnitudo(linea), lista)
                != NIHIL)
            {
                     Piscina* p;
                   character* textus;
                         i32  mensura  = ZEPHYRUM;
                      size_t  l        = strlen(linea);

                dum (   l > ZEPHYRUM
                     && (linea[l - I] == '\n' || linea[l - I] == '\r'))
                {
                    linea[l - I]  = '\0';
                    l             = l - I;
                }
                si (l == ZEPHYRUM)
                {
                    perge;
                }
                sprintf(via, "%s/%s", radix_viae, linea);
                p = piscina_generare_dynamicum("corpus_md_stml",
                    4194304);
                textus = _plagulam_legere(p, via, &mensura);
                si (textus == NIHIL)
                {
                    piscina_destruere(p);
                    perge;
                }
                plagulae  = plagulae + I;
                octeti    = octeti + (s64)mensura;
                si (!_circuitum_probare(p, &consilium, textus, mensura,
                    VERUM))
                {
                    fractae = fractae + I;
                    imprimere("  FRACTA: %s\n", linea);
                }
                piscina_destruere(p);
            }
            fclose(lista);
            imprimere("  plagulae %d, octeti %ld, fractae %d\n",
                (integer)plagulae, (longus)octeti, (integer)fractae);
            CREDO_AEQUALIS_I32 (fractae, ZEPHYRUM);
            CREDO_MAIOR_I32 (plagulae, (i32)1000);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
