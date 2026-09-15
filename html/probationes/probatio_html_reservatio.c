/* probatio_html_reservatio.c - Porta reservationis (H8; spec par. XI.6)
 *
 * v1 NIHIL reservat in forma: puncta extensionis (ancora, spatium,
 * clonatum, reinserendum) in declaratione NOMINATA sunt, non
 * declarata. Recusatio documenti quod ea iam fert AUTOMATICA est et
 * hic per ordines tres asseritur, MENSURATA in materia_arbor.c:
 *
 *  (a) ut ELEMENTUM intra <elementum>: lector 'locus generi ignotus'
 *      (l. 3170) - documentum NIHIL, causa nominata;
 *  (b) <reinserendum> ut ELEMENTUM intra <liberi>: lector 'genus
 *      registro ignotum' (l. 3090);
 *  (c) ut ATTRIBUTUM in <elementum>: lector ACCIPIT - 'id' solum legit,
 *      census attributorum nullus (_nodum_legere) - PINNATUM, ne
 *      census futurus in materia tacite adveniat; canon eum iudicat
 *      ('attributum ignotum').
 *
 * Documenta manu scripta cum sigillo VIVO (materia_arbor_sigillum);
 * testis positivus: idem documentum sine re aliena lectorem transit
 * et canonem sine vitio - ne assertiones vacuae sint.
 */

#include "latina.h"
#include "credo.h"
#include "canon.h"
#include "chorda.h"
#include "html_registrum.h"
#include "html_lexicon.h"
#include "internamentum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


hic_manens constans character* RESERVATA[] = {
    "ancora", "spatium", "clonatum", "reinserendum"
};

/* Documentum: elementum '<p>' cum re aliena INTRA elementum
 * (liberum_alienum, '' = nulla) et attributo alieno in elemento
 * (attributum_alienum, '' = nullum). */
hic_manens chorda
_documentum_struere (
                Piscina* piscina,
                 chorda  sigillum,
     constans character* liberum_alienum,
     constans character* attributum_alienum,
     constans character* liberum_documenti)
{
    character* buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)MMMM);
    chorda c;

    sprintf(buffer,
        "<arbor grammatica=\"html\" registrum-sigillum=\"%.*s\""
        " fons=\"0\" b=\"0\" linea=\"1\" columna=\"1\">\n"
        "  <documentum>\n"
        "    <liberi>\n"
        "      <elementum%s>\n"
        "        <tok_apertura><lex-tag-apertura>&lt;p"
        "</lex-tag-apertura>"
        "</tok_apertura>\n"
        "        <tok_finis><lex-tag-finis/></tok_finis>\n"
        "%s"
        "      </elementum>\n"
        "%s"
        "    </liberi>\n"
        "    <cauda><lex-finis/></cauda>\n"
        "  </documentum>\n"
        "</arbor>\n",
        (integer)sigillum.mensura, (constans character*)sigillum.datum,
        attributum_alienum, liberum_alienum, liberum_documenti);
    c.datum    = (i8*)buffer;
    c.mensura  = (i32)strlen(buffer);
    redde c;
}

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

/* Vitia canonis in documento (s32 -I = fractura ante iudicium) */
hic_manens s32
_canone_iudicare (
                Piscina* piscina,
    InternamentumChorda* intern,
                  Canon* canon,
                 chorda  textus)
{
    StmlResultus  res;
             Xar* vitia;
             i32  n;
             i32  i;

    res = stml_legere(textus, piscina, intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    { redde (s32)-I;
    }
    vitia = canon_iudicare(canon, res.elementum_radix, piscina);
    si (vitia == NIHIL)
    { redde (s32)-I;
    }
    n = xar_numerus(vitia);
    per (i = ZEPHYRUM; i < n && i < IV; i++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, i);

        imprimere("      vitium: %s", canon_nuntius(v->genus));
        si (v->detail != NIHIL)
        {
            imprimere(" '%.*s'", (integer)v->detail->mensura,
                (constans character*)v->detail->datum);
        }
        imprimere("\n");
    }
    redde (s32)n;
}

integer
principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* radix_viae;
                  Canon* canon;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;
  MateriaArborConsilium  consilium;
     MateriaArborVitium  vitium;
                 chorda  sigillum;
                    b32  praeteritus;
                    i32  k;

    piscina = piscina_generare_dynamicum("probatio_html_reservatio",
        1048576);
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    { radix_viae = ".";
    }
    si (!materia_lexicon_ratum_facere(&ratum, &HTML_LEXICON, &iudicium))
    {
        imprimere("LEXICON HTML RECUSATUM\n");
        redde I;
    }
    materia_arbor_consilium_nudum(&consilium, &HTML_REGISTRUM, &ratum,
        "html");
    sigillum = materia_arbor_sigillum(piscina, &HTML_REGISTRUM);

    /* canon */
    {
        character  via[DXII];
        character* textus;
              i32  mensura = ZEPHYRUM;
           chorda  fons;
           chorda  causa;

        sprintf(via, "%s/html/grammatica/html.canon", radix_viae);
        textus = _plagulam_legere(piscina, via, &mensura);
        CREDO_NON_NIHIL (textus);
        fons.datum     = (i8*)textus;
        fons.mensura   = mensura;
        causa.datum    = NIHIL;
        causa.mensura  = ZEPHYRUM;
        canon = textus ? canon_legere(fons, piscina, intern, &causa)
                       : NIHIL;
        CREDO_NON_NIHIL (canon);
        si (canon == NIHIL)
        {
            credo_imprimere_compendium();
            redde I;
        }
    }


    /* ==================================================
     * TESTIS POSITIVUS: documentum purum transit
     * ================================================== */

    {
        chorda doc = _documentum_struere(piscina, sigillum, "", "", "");
        MateriaNodus* lecta;

        imprimere("\n--- Testis positivus: documentum purum ---\n");
        lecta = materia_arbor_legere(piscina, NIHIL, doc, &consilium,
            &vitium);
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);
        CREDO_AEQUALIS_S32 (_canone_iudicare(piscina, intern, canon,
            doc),
            (s32)ZEPHYRUM);
    }


    /* ==================================================
     * (a) ut ELEMENTUM intra elementum: 'locus generi ignotus'
     * ================================================== */

    imprimere("\n--- (a) res reservata ut elementum elementi ---\n");
    per (k = ZEPHYRUM; k < IV; k++)
    {
        character liberum[LXIV];
           chorda doc;
        MateriaNodus* lecta;

        sprintf(liberum, "        <%s/>\n", RESERVATA[k]);
        doc = _documentum_struere(piscina, sigillum, liberum, "", "");
        lecta = materia_arbor_legere(piscina, NIHIL, doc, &consilium,
            &vitium);
        imprimere("  <%s/>: %s\n", RESERVATA[k],
            vitium.causa ? vitium.causa : "(accepta)");
        CREDO_NIHIL (lecta);
        CREDO_NON_NIHIL (vitium.causa);
        si (vitium.causa != NIHIL)
        {
            CREDO_VERUM (strcmp(vitium.causa, "locus generi ignotus")
                == ZEPHYRUM);
        }
    }


    /* ==================================================
     * (b) <reinserendum> ut ELEMENTUM intra liberi: 'genus ignotum'
     * ================================================== */

    {
        chorda doc = _documentum_struere(piscina, sigillum, "", "",
            "      <reinserendum/>\n");
        MateriaNodus* lecta;

        imprimere("\n--- (b) reinserendum ut genus in liberis ---\n");
        lecta = materia_arbor_legere(piscina, NIHIL, doc, &consilium,
            &vitium);
        imprimere("  %s\n", vitium.causa ? vitium.causa : "(accepta)");
        CREDO_NIHIL (lecta);
        CREDO_NON_NIHIL (vitium.causa);
        si (vitium.causa != NIHIL)
        {
            CREDO_VERUM (strcmp(vitium.causa, "genus registro ignotum")
                == ZEPHYRUM);
        }
    }


    /* ==================================================
     * (c) ut ATTRIBUTUM: lector ACCIPIT (pinna), canon iudicat
     * ================================================== */

    imprimere("\n--- (c) res reservata ut attributum elementi ---\n");
    per (k = ZEPHYRUM; k < III; k++)
    {
        character attributum[LXIV];
           chorda doc;
        MateriaNodus* lecta;
        s32 vitia;

        sprintf(attributum, " %s=\"x\"", RESERVATA[k]);
        doc = _documentum_struere(piscina, sigillum, "", attributum,
            "");
        lecta = materia_arbor_legere(piscina, NIHIL, doc, &consilium,
            &vitium);
        /* PINNA: lector attributa ignota IGNORAT (mensuratum). Cum
         * materia census attributorum addat, haec rubet et ordo (c)
         * in recusationem lectoris promovetur. */
        CREDO_NON_NIHIL (lecta);
        CREDO_NIHIL (vitium.causa);
        vitia = _canone_iudicare(piscina, intern, canon, doc);
        imprimere("  %s=\"x\": lector accipit, canon vitia %d\n",
            RESERVATA[k], (integer)vitia);
        CREDO_MAIOR_S32 (vitia, (s32)ZEPHYRUM);
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
