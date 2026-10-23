/* probatio_materia_token.c - Lexema substrati
 *
 * Duo puncta hic praeter usum ordinarium probantur, quia utrumque
 * consilium PORTAT:
 *   1. dispositio PINNATA (LXIV octeti) - campus additus AUDIRI debet
 *   2. cauda CONTIGUA - id est tota causa cur cauda punctatori
 *      praelata sit; si allocatio scinditur, argumentum perit
 */

#include "latina.h"
#include "credo.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

/* Forma frontis fictae: octo octeti privati */
hic_manens constans MateriaTokenForma FORMA_CUM_CAUDA  = { (i32)VIII };
hic_manens constans MateriaTokenForma FORMA_SINE_CAUDA = { ZEPHYRUM };


/* ==================================================
 * PROBARE: dispositio PINNATA
 * ================================================== */

interior vacuum
_dispositionem_probare (
    CredoContextus* c)
{
    (vacuum)c;
    {
        /* LXIV mensuratum est, non speratum. SilvaToken hodiernus
         * CXXVIII fert; dimidium est totum lucrum RP XIV-XVII.
         * Campus additus hunc numerum movet - id AUDIENDUM est. */
        CREDO_AEQUALIS_I32 ((i32)magnitudo(MateriaToken), (i32)64);
    }
}


/* ==================================================
 * PROBARE: creatio et campi
 * ================================================== */

interior vacuum
_creationem_probare (
    CredoContextus* c)
{
    Piscina* piscina  = c->piscina;
     chorda  valor    = chorda_ex_literis("int", piscina);

    {
        MateriaToken* t;

        t = materia_token_creare(piscina, &FORMA_SINE_CAUDA, (s32)7,
            valor, (s32)42, (i32)3, (i32)11, (s32)1);
        CREDO_NON_NIHIL (t);
        CREDO_AEQUALIS_S32 (t->genus, (s32)7);
        CREDO_AEQUALIS_S32 (t->byte_offset, (s32)42);
        CREDO_AEQUALIS_I32 (t->linea, (i32)3);
        CREDO_AEQUALIS_I32 (t->columna, (i32)11);
        CREDO_AEQUALIS_S32 (t->fons_index, (s32)1);
        CREDO_AEQUALIS_I32 (t->valor.mensura, (i32)3);

        /* Trivia zephyrum ex nativitate */
        CREDO_NIHIL (t->spatia_ante);
        CREDO_NIHIL (t->spatia_post);
        CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (t->numerus_post, ZEPHYRUM);

        /* piscina NIHIL: nihil, non ruina */
        CREDO_NIHIL (materia_token_creare(NIHIL, &FORMA_SINE_CAUDA,
            ZEPHYRUM, valor, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM));
    }
}


/* ==================================================
 * PROBARE: CAUDA - allocatio UNA, non duae
 * ================================================== */

interior vacuum
_caudam_probare (
    CredoContextus* c)
{
    Piscina* piscina  = c->piscina;
     chorda  valor    = chorda_ex_literis("int", piscina);

    {
        MateriaToken* sine;
        MateriaToken* cum;
        MateriaToken* alter;
              vacuum* cauda;
              vacuum* cauda_altera;
           character* octeti;
                 i32  i;

        sine = materia_token_creare(piscina, &FORMA_SINE_CAUDA,
            ZEPHYRUM,
            valor, ZEPHYRUM, (i32)I, (i32)I, ZEPHYRUM);
        cum  = materia_token_creare(piscina, &FORMA_CUM_CAUDA, ZEPHYRUM,
            valor, ZEPHYRUM, (i32)I, (i32)I, ZEPHYRUM);
        CREDO_NON_NIHIL (sine);
        CREDO_NON_NIHIL (cum);

        /* Sine cauda: NIHIL, non punctator ultra allocationem.
         * Hoc est cur vexillum CAUDA exsistit. */
        CREDO_NIHIL (materia_token_cauda(sine));

        cauda = materia_token_cauda(cum);
        CREDO_NON_NIHIL (cauda);

        /* CONTIGUA - argumentum totum. Cauda punctatori praelata est
         * ut allocatio UNA maneat (localitas, quam silva_token.h:9
         * generatione praecedente emit). Si hoc frangitur,
         * electio sensum amittit. */
        CREDO_AEQUALIS_I32 (
            (i32)((character*)cauda - (character*)cum),
            (i32)magnitudo(MateriaToken));

        /* Ordinata - piscina_allocare nuda id NON praestat */
        CREDO_AEQUALIS_I32 (
            (i32)((memoriae_index)(size_t)cauda % magnitudo(vacuum*)),
            ZEPHYRUM);

        /* Ad zephyrum posita */
        octeti = (character*)cauda;
        per (i = ZEPHYRUM; i < (i32)VIII; i++)
        {
            CREDO_AEQUALIS_I32 ((i32)octeti[i], ZEPHYRUM);
        }

        /* Scribi potest et manet */
        memcpy(cauda, "ABCDEFGH", (size_t)VIII);
        CREDO_VERUM (memcmp(materia_token_cauda(cum), "ABCDEFGH",
            (size_t)VIII) == ZEPHYRUM);

        /* Duo lexemata caudas suas NON communicant - error mensurae
         * hic apparet, non tribus operibus post */
        alter = materia_token_creare(piscina, &FORMA_CUM_CAUDA,
            ZEPHYRUM,
            valor, ZEPHYRUM, (i32)I, (i32)I, ZEPHYRUM);
        CREDO_NON_NIHIL (alter);
        cauda_altera = materia_token_cauda(alter);
        CREDO_NON_NIHIL (cauda_altera);
        CREDO_INAEQUALITAS_PTR (cauda, cauda_altera);
        /* altera adhuc munda - prima eam non contaminavit */
        CREDO_VERUM (memcmp(cauda_altera, "\0\0\0\0\0\0\0\0",
            (size_t)VIII) == ZEPHYRUM);

        /* ET PRIMA SUPERSTES POST ALLOCATIONEM ALTERIUS. Haec una
         * allocationem PARVAM deprehendit: si creare caudam in
         * mensura oblitus est, lexema proximum caudam prioris
         * OCCUPAT, et probationes supra id NON vident (punctatores
         * adhuc differunt, memoria adhuc munda videtur donec quis
         * scribat). Sine hac assertione porta caudae theatrum est. */
        CREDO_VERUM (memcmp(materia_token_cauda(cum), "ABCDEFGH",
            (size_t)VIII) == ZEPHYRUM);

        /* forma NIHIL = nulla cauda */
        CREDO_NIHIL (materia_token_cauda(materia_token_creare(piscina,
            NIHIL, ZEPHYRUM, valor, ZEPHYRUM, (i32)I, (i32)I,
            ZEPHYRUM)));
        CREDO_NIHIL (materia_token_cauda(NIHIL));
    }
}


/* ==================================================
 * PROBARE: trivia EXACTA
 * ================================================== */

interior vacuum
_trivia_probare (
    CredoContextus* c)
{
    Piscina* piscina  = c->piscina;
     chorda  valor    = chorda_ex_literis("int", piscina);

    {
        MateriaToken* t;
        MateriaToken* trivia[3];
                 i32  i;

        t = materia_token_creare(piscina, &FORMA_SINE_CAUDA, ZEPHYRUM,
            valor, ZEPHYRUM, (i32)I, (i32)I, ZEPHYRUM);
        CREDO_NON_NIHIL (t);
        per (i = ZEPHYRUM; i < (i32)III; i++)
        {
            trivia[i] = materia_token_creare(piscina, &FORMA_SINE_CAUDA,
                (s32)(i + I), valor, ZEPHYRUM, (i32)I, (i32)I,
                ZEPHYRUM);
            CREDO_NON_NIHIL (trivia[i]);
        }

        CREDO_VERUM (materia_token_trivia_ante_ponere(t, piscina,
            trivia, (i32)III));
        CREDO_AEQUALIS_I32 (t->numerus_ante, (i32)III);
        CREDO_NON_NIHIL (t->spatia_ante);
        per (i = ZEPHYRUM; i < (i32)III; i++)
        {
            CREDO_AEQUALIS_PTR (t->spatia_ante[i], trivia[i]);
        }

        /* Copia est, non alias: fons mutatus lexema non movet */
        trivia[0] = NIHIL;
        CREDO_NON_NIHIL (t->spatia_ante[0]);

        CREDO_VERUM (materia_token_trivia_post_ponere(t, piscina,
            trivia + I, (i32)II));
        CREDO_AEQUALIS_I32 (t->numerus_post, (i32)II);

        /* ante et post series DISTINCTAS tenent */
        CREDO_INAEQUALITAS_PTR (t->spatia_ante, t->spatia_post);

        /* numerus ZEPHYRUM licet - NIHIL, non series vacua */
        CREDO_VERUM (materia_token_trivia_ante_ponere(t, piscina,
            NIHIL, ZEPHYRUM));
        CREDO_NIHIL (t->spatia_ante);
        CREDO_AEQUALIS_I32 (t->numerus_ante, ZEPHYRUM);

        /* fonte NIHIL cum numero > ZEPHYRUM: RECUSAT */
        CREDO_FALSUM (materia_token_trivia_ante_ponere(t, piscina,
            NIHIL, (i32)II));
        CREDO_FALSUM (materia_token_trivia_ante_ponere(NIHIL, piscina,
            trivia, (i32)II));
    }
}


/* ==================================================
 * PROBARE: vexilla non se invicem conculcant
 * ================================================== */

interior vacuum
_vexilla_probare (
    CredoContextus* c)
{
    Piscina* piscina  = c->piscina;
     chorda  valor    = chorda_ex_literis("int", piscina);

    {
        MateriaToken* t;

        t = materia_token_creare(piscina, &FORMA_CUM_CAUDA, ZEPHYRUM,
            valor, ZEPHYRUM, (i32)I, (i32)I, ZEPHYRUM);
        CREDO_NON_NIHIL (t);
        CREDO_FALSUM (materia_token_initium_lineae(t));

        materia_token_initium_lineae_ponere(t, VERUM);
        CREDO_VERUM (materia_token_initium_lineae(t));
        /* CAUDA superest - vexillum alterum eam delere NON debet */
        CREDO_NON_NIHIL (materia_token_cauda(t));

        materia_token_initium_lineae_ponere(t, FALSUM);
        CREDO_FALSUM (materia_token_initium_lineae(t));
        CREDO_NON_NIHIL (materia_token_cauda(t));

        CREDO_FALSUM (materia_token_initium_lineae(NIHIL));
        /* non ruat */
        materia_token_initium_lineae_ponere(NIHIL, VERUM);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "dispositio pinnata", _dispositionem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "creatio et campi", _creationem_probare, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "cauda: allocatio una, non duae", _caudam_probare, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "trivia exacta", _trivia_probare, NIHIL, NIHIL, NIHIL, NIHIL },
    { "vexilla non se invicem conculcant", _vexilla_probare, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("materia_token", SECTIONES);
}
