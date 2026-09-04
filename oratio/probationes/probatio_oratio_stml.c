/* probatio_oratio_stml.c - Circuitus proiectionis STML orationis (T4)
 *
 * Scribere -> legere -> scribere BIS; oracula: octeti (emissio ex arbore
 * lecta contra fontem), comparator STRUCTURALIS et FIDELIS (LINEA munus
 * LINEA fert). Canalis derivatorum (gradus III) hic iam exercetur:
 * lexema derivatum in loco 'classes' manu positum f="1" et positionem
 * portatam fert, cursorem non movet, emissor id omittit. CORPUS:
 * plagulae md totae ut oratio (octeti quilibet). Culpa plantata: uncus
 * originis sublatus (consilium->origo NIHIL).
 */

#include "latina.h"
#include "credo.h"
#include "oratio_arbor.h"
#include "oratio_stml.h"
#include "oratio_lexema.h"
#include "oratio_registrum.h"
#include "oratio_lexicon.h"
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
          FILE* f = fopen(via, "rb");
        longus  longitudo;
     character* memoria;

    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    rewind(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (fread(memoria, I, (size_t)longitudo, f) != (size_t)longitudo)
    {
        fclose(f);
        redde NIHIL;
    }
    fclose(f);
    *mensura = (i32)longitudo;
    redde memoria;
}

/* Circuitus plenus super arborem datam. */
interior b32
_circuitum_arboris (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                      MateriaNodus* radix,
                constans character* fons,
                               i32  mensura,
                               b32  fidelis)
{
    MateriaNodus* lecta;
    MateriaNodus* relecta;
    MateriaArborScriptura s1;
    MateriaArborScriptura s2;
    MateriaArborScriptura s3;
    MateriaArborVitium vitium;
    MateriaArborDifferentia d;

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
        (size_t)s1.textus.mensura)
            != ZEPHYRUM)
    {
        imprimere("    (documenta I/II dispari)\n");
        redde FALSUM;
    }
    relecta = materia_arbor_legere(piscina, NIHIL, s2.textus, consilium,
        &vitium);
    si (relecta == NIHIL)
    {
        imprimere("    (lectio II)\n");
        redde FALSUM;
    }
    s3 = materia_arbor_scribere_nodum(piscina, relecta, consilium);
    si (   !s3.successus || s1.textus.mensura != s3.textus.mensura
        || memcmp(s1.textus.datum, s3.textus.datum,
        (size_t)s1.textus.mensura)
            != ZEPHYRUM)
    {
        imprimere("    (circuitus II dispar)\n");
        redde FALSUM;
    }
    si (!materia_arbor_aequalis(radix, lecta,
        MATERIA_ARBOR_COMPARATIO_STRUCTURALIS,
            &d))
    {
        imprimere("    (arbor dispar: %s @ %s)\n",
            d.campus ? d.campus : "-", d.via);
        redde FALSUM;
    }
    si (   fidelis && !materia_arbor_aequalis(radix, lecta,
            MATERIA_ARBOR_COMPARATIO_FIDELITAS, &d))
    {
        imprimere("    (fidelitas dispar: %s @ %s)\n",
            d.campus ? d.campus : "-",
            d.via);
        redde FALSUM;
    }
    {
                 MateriaScriptura emissa;
        MateriaScripturaConsilium cs;

        materia_scriptura_consilium_nudum(&cs, &ORATIO_REGISTRUM);
        cs.fons_index  = ORATIO_FONS_PLAGULAE;
        emissa         = materia_scribere_nodum(piscina, lecta, &cs);
        si (   !emissa.successus || emissa.textus.mensura != mensura
            || (mensura > ZEPHYRUM
                && memcmp(emissa.textus.datum, fons, (size_t)mensura)
                    != ZEPHYRUM))
        {
            imprimere("    (emissio ex arbore lecta != fons)\n");
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior b32
_circuitum_probare (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura,
                               b32  fidelis)
{
    MateriaNodus* radix = oratio_arbor_parsare(piscina, fons, mensura);

    si (radix == NIHIL)
    {
        imprimere("    (parsura NIHIL)\n");
        redde FALSUM;
    }
    redde _circuitum_arboris(piscina, consilium, radix, fons, mensura,
        fidelis);
}

s32
principale (vacuum)
{
                  Piscina* piscina;
       constans character* radix_viae;
      MateriaLexiconRatum  ratum;
       MateriaLexIudicium  iudicium;
    MateriaArborConsilium  consilium;

    piscina = piscina_generare_dynamicum("probatio_oratio_stml",
        8388608);
    credo_aperire(piscina);
    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    si (!materia_lexicon_ratum_facere(&ratum, &ORATIO_LEXICON,
        &iudicium))
    {
        imprimere("LEXICON ORATIONIS RECUSATUM\n");
        redde I;
    }
    oratio_stml_consilium(&consilium, &ratum);

    {
        hic_manens constans character* CASUS[] = {
            "Hello world. This is it.\n\nSecond para.\n",
            "  Indented start.\n\n\n  Next one\n",
            "Mr. Smith went home. He slept.",
            "\"Hello!\" she said. Then left. It cost $3.50. Wait... What?",
            "well-known don't e.g B2 1980s 1,024 3.14 x",
            "a.\r\n\r\nb.\r\n",
            "\xC3\x86r\xC3\xB8 \xC2\xA0x \xE2\x80\x9Cq\xE2\x80\x9D \xE2\x80\x94 don\xE2\x80\x99t \x01\x02.",
            "text </or-litterae> in textu\n",
            "",
            "\n\n \t\n"
        };
        i32 numerus = (i32)(magnitudo(CASUS) / magnitudo(CASUS[0]));
        i32 i;

        imprimere("\n--- Probans circuitum STML (structuralis + fidelis) ---\n");
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            CREDO_VERUM (_circuitum_probare(piscina, &consilium,
                CASUS[i],
                (i32)strlen(CASUS[i]), VERUM));
        }
    }

    imprimere("\n--- Probans canalem derivatorum (classes f=\"1\", emissor omittit) ---\n");
    {
        constans character* f = "Rosa alba.\n";
              MateriaNodus* radix = oratio_arbor_parsare(piscina, f,
                  (i32)strlen(f));
        MateriaNodus* par;
        MateriaNodus* sen;
        MateriaNodus* voc;
        MateriaToken* origo;
        MateriaToken* classes;
        MateriaArborScriptura s;

        CREDO_NON_NIHIL (radix);
        par =
            materia_valor_lista_obtinere(radix->loci[ORATIO_DOCUMENTUM_PARAGRAPHI],
            ZEPHYRUM)->datum.nodus;
        sen =
            materia_valor_lista_obtinere(par->loci[ORATIO_PARAGRAPHUS_SENTENTIAE],
            ZEPHYRUM)->datum.nodus;
        voc =
            materia_valor_lista_obtinere(sen->loci[ORATIO_SENTENTIA_ELEMENTA],
            ZEPHYRUM)->datum.nodus;
        CREDO_AEQUALIS_S32 (voc->genus, (s32)ORATIO_GENUS_VOCABULUM);
        origo =
            materia_valor_lista_obtinere(voc->loci[ORATIO_VOCABULUM_PARTES],
            ZEPHYRUM)->datum.token;
        classes = oratio_lexema_derivatum(piscina,
            (s32)ORATIO_LEX_DERIVATUM,
            chorda_ex_literis("substantivum adiectivum", piscina),
            origo);
        CREDO_NON_NIHIL (classes);
        CREDO_VERUM (materia_nodus_ponere(voc,
            (i32)ORATIO_VOCABULUM_CLASSES,
            materia_valor_token(classes), MATERIA_LOCUS_TOKEN));
        s = materia_arbor_scribere_nodum(piscina, radix, &consilium);
        CREDO_VERUM (s.successus);
        CREDO_NON_NIHIL (strstr((character*)s.textus.datum, "f=\"1\""));
        CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
            "<or-derivatum"));
        CREDO_NON_NIHIL (strstr((character*)s.textus.datum,
            "substantivum adiectivum"));
        CREDO_NIHIL (strstr((character*)s.textus.datum, "<ante>"));
        /* circuitus cum derivato: structuralis + fidelis + emissio == fons */
        CREDO_VERUM (_circuitum_arboris(piscina, &consilium, radix, f,
            (i32)strlen(f), VERUM));
    }

    imprimere("\n--- CORPUS: plagulae md totae ut oratio, circuitus STML ---\n");
    {
        character via[DXII];
        FILE* lista;
        i32 plagulae = ZEPHYRUM;
        i32 fractae  = ZEPHYRUM;
        i32 numerus_lineae = ZEPHYRUM;

        sprintf(via, "%s/oratio/build/corpus_md.txt", radix_viae);
        lista = fopen(via, "r");
        si (lista == NIHIL)
        {
            CREDO_CULPA ("oratio/build/corpus_md.txt absens - e radice per cursorem curre");
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
                    linea[--l] = '\0';
                }
                si (l == ZEPHYRUM)
                {
                    perge;
                }
                /* specimen: plagula quinta quaeque (corpus totum:
                 * ORATIO_CORPUS_TOTUS=1) - circuitus STML arboris
                 * verborum XL s super MCXXXIV plagulas est */
                numerus_lineae++;
                si (   getenv("ORATIO_CORPUS_TOTUS") == NIHIL
                    && (numerus_lineae % V)          != ZEPHYRUM)
                {
                    perge;
                }
                p = piscina_generare_dynamicum("oratio_corpus_stml",
                    67108864);
                sprintf(via, "%s/%s", radix_viae, linea);
                textus = _plagulam_legere(p, via, &mensura);
                si (textus == NIHIL)
                {
                    piscina_destruere(p);
                    perge;
                }
                plagulae++;
                si (!_circuitum_probare(p, &consilium, textus, mensura,
                    VERUM))
                {
                    fractae++;
                    si (fractae <= V)
                    {
                        imprimere("    FRACTA: %s\n", linea);
                    }
                }
                piscina_destruere(p);
            }
            fclose(lista);
            imprimere("    plagulae %d, fractae %d\n",
                (integer)plagulae,
                (integer)fractae);
            CREDO_MAIOR_I32 (plagulae, (i32)200);
            CREDO_AEQUALIS_I32 (fractae, ZEPHYRUM);
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();
    {
        b32 praeteritus = credo_omnia_praeterierunt();

        piscina_destruere(piscina);
        redde praeteritus ? ZEPHYRUM : I;
    }
}
