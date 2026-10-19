/* probatio_sententia_fissio.c
 *
 * Probationes unitatum pro bibliotheca sententia_fissio.
 * Portatum ex kisci-events/tests/utils/sentence-splitter.test.ts
 */

#include "sententia_fissio.h"
#include "credo.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>


/* ==================================================
 * Auxiliaris: Creare chorda ex literis
 * ================================================== */

interior chorda
_chorda_ex_literis (
    constans character* literis,
               Piscina* piscina)
{
    chorda result;
       i32 longitudo;

    longitudo       = (i32)strlen(literis);
    result.datum    = piscina_allocare(piscina, (i64)(longitudo + I));
    result.mensura  = longitudo;
    memcpy(result.datum, literis, (size_t)longitudo);

    redde result;
}

/* Comparare chorda cum literis */
interior b32
_chorda_aequalis_literis (
                chorda  s,
    constans character* literis)
{
    i32 longitudo;

    longitudo = (i32)strlen(literis);

    si (s.mensura != longitudo)
    {
        redde FALSUM;
    }

    redde memcmp(s.datum, literis, (size_t)longitudo) == ZEPHYRUM;
}


/* ==================================================
 * Probationes
 * ================================================== */


    /* ==================================================
     * 1. Non findere pro Rev. Mr. pattern
     * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "Miss Ward found herself obliged to be attached to the Rev. Mr. Norris, a friend of her brother-in-law.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 2. Multiplices abbreviationes in una sententia
     * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "Dr. Smith met with Mr. Johnson and Mrs. Williams at 3 p.m. yesterday.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 3. Abbreviationes maiusculae
     * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "MR. JONES and DR. SMITH arrived. They were late.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "MR. JONES and DR. SMITH arrived."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "They were late."));
    }
}


    /* ==================================================
     * 4. No. sequutum per numerum
     * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "Captain Hagberd's person, clad in No. 1 sail-cloth from head to foot.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 5. Sententiae quotatae (straight quotes)
     * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "\"This is one sentence.\" She said. \"This is another.\"";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, III);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "\"This is one sentence.\""));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "She said."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[II],
            "\"This is another.\""));
    }
}


    /* ==================================================
     * 6. Numeri decimales
     * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "The price was $3.50 for the item. It was expensive.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "The price was $3.50 for the item."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "It was expensive."));
    }
}


    /* ==================================================
     * 7. Abbreviationes viarum (St., Ave., etc.)
     * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "He lives at 123 Main St. near the park. She lives on Elm Ave. downtown.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "He lives at 123 Main St. near the park."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "She lives on Elm Ave. downtown."));
    }
}


    /* ==================================================
     * 8. Abbreviationes temporis (p.m., a.m.)
     * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "The meeting is at 3 p.m. today. Don't be late.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "The meeting is at 3 p.m. today."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "Don't be late."));
    }
}


    /* ==================================================
     * 9. Rev. Dr. pattern complexum
     * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "The Rev. Dr. Martin Luther King Jr. gave a speech.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 10. U.S. et U.K.
     * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "He traveled from the U.S. to the U.K. last year.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 11. etc. ad finem sententiae
     * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "He bought apples, oranges, bananas, etc. It was a lot of fruit.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "He bought apples, oranges, bananas, etc."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "It was a lot of fruit."));
    }
}


    /* ==================================================
     * 12. i.e. et e.g.
     * ================================================== */

interior vacuum
_sectio_decima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "He likes citrus fruits (e.g. oranges and lemons). She prefers berries.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "He likes citrus fruits (e.g. oranges and lemons)."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "She prefers berries."));
    }
}


    /* ==================================================
     * 13. Ellipsis
     * ================================================== */

interior vacuum
_sectio_decima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "He wasn't sure... maybe tomorrow. We'll see.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "He wasn't sure... maybe tomorrow."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "We'll see."));
    }
}


    /* ==================================================
     * 14. Interrogationes et exclamationes
     * ================================================== */

interior vacuum
_sectio_decima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "What time is it? It's late! Let's go.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, III);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "What time is it?"));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "It's late!"));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[II],
            "Let's go."));
    }
}


    /* ==================================================
     * 15. Fr. et Sr. tituli religiosi
     * ================================================== */

interior vacuum
_sectio_decima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "Fr. John celebrated mass. Sr. Mary taught the children.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "Fr. John celebrated mass."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "Sr. Mary taught the children."));
    }
}


    /* ==================================================
     * 16. Gradus militares
     * ================================================== */

interior vacuum
_sectio_decima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "Col. Sanders founded KFC. Gen. Patton led the troops.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "Col. Sanders founded KFC."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "Gen. Patton led the troops."));
    }
}


    /* ==================================================
     * 17. Tituli consecutivi
     * ================================================== */

interior vacuum
_sectio_decima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "The Rev. Fr. Thomas O'Malley presided.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 18. Prof. Dr. pattern
     * ================================================== */

interior vacuum
_sectio_decima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "Prof. Dr. Einstein discovered relativity.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 19. Jr. et Sr. suffixes
     * ================================================== */

interior vacuum
_sectio_decima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "Martin Luther King Jr. was a civil rights leader. His father was Martin Luther King Sr. too.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "Martin Luther King Jr. was a civil rights leader."));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "His father was Martin Luther King Sr. too."));
    }
}


    /* ==================================================
     * 20. Abbreviationes Latinae et nomina Romana
     * ================================================== */

interior vacuum
_sectio_vicesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "C. IULI CAESARIS DE BELLO GALLICO COMMENTARIUS PRIMUS";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 21. Litterae singulae abbreviationes
     * ================================================== */

interior vacuum
_sectio_vicesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "P. Smith and L. Johnson met with M. Williams.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 22. Textus Latinus cum a.u.c. et p.u.c.
     * ================================================== */

interior vacuum
_sectio_vicesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "L. Sergius Catilina, homo patricii generis, magna vi et animi et corporis, sed ingenio malo pravoque, praetorius, qui iam a. u. c. 689 caedem consulum facere eaque facta rerum potiri constituerat, casu autem rem perficere prohibitus erat, ascitis ad consilium rei publicae opprimendae hominibus omnis generis perditissiis atque audacissimis, quos inopia, cupiditas, scelera stimulabant, consulatum in annum 691 p. u. c. petivit, sed cum, quae in animo habebat, perniciosa rei publicae consilia parum occultata essent, studiis bonorum omnium M. Tullius Cicero una cum C. Antonio consul factus est.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 23. Smart quotes (curly double quotes)
     * ================================================== */

interior vacuum
_sectio_vicesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        /* Along with ... \u201C ... \u201D ... \u201D */
        /* UTF-8: left double quote = E2 80 9C, right double quote = E2 80 9D */
        constans character* input =
            "Along with his friend Hilaire Belloc and in books like the 1910 \xe2\x80\x9c" "What's Wrong with the World\xe2\x80\x9d he advocated a view called \xe2\x80\x9c" "Distributionism\xe2\x80\x9d that was best summed up by his expression that every man ought to be allowed to own \xe2\x80\x9cthree acres and a cow.\xe2\x80\x9d Though not known as a political thinker, his political influence has circled the world.";
        constans character* expected0 =
            "Along with his friend Hilaire Belloc and in books like the 1910 \xe2\x80\x9c" "What's Wrong with the World\xe2\x80\x9d he advocated a view called \xe2\x80\x9c" "Distributionism\xe2\x80\x9d that was best summed up by his expression that every man ought to be allowed to own \xe2\x80\x9cthree acres and a cow.\xe2\x80\x9d";
        constans character* expected1 =
            "Though not known as a political thinker, his political influence has circled the world.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            expected0));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            expected1));
    }
}


    /* ==================================================
     * 24. Smart quotes "orthodox"
     * ================================================== */

interior vacuum
_sectio_vicesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "Nothing more strangely indicates an enormous and silent evil of modern society than the extraordinary use which is made nowadays of the word \xe2\x80\x9corthodox.\xe2\x80\x9d In former days the heretic was proud of not being a heretic.";
        constans character* expected0 =
            "Nothing more strangely indicates an enormous and silent evil of modern society than the extraordinary use which is made nowadays of the word \xe2\x80\x9corthodox.\xe2\x80\x9d";
        constans character* expected1 =
            "In former days the heretic was proud of not being a heretic.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            expected0));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            expected1));
    }
}


    /* ==================================================
     * 25. Textus cum straight double quotes (frontend paste)
     * ================================================== */

interior vacuum
_sectio_vicesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "Along with his friend Hilaire Belloc and in books like the 1910 \"What's Wrong with the World\" he advocated a view called \"Distributionism\" that was best summed up by his expression that every man ought to be allowed to own \"three acres and a cow.\" Though not known as a political thinker, his political influence has circled the world.";
        constans character* expected0 =
            "Along with his friend Hilaire Belloc and in books like the 1910 \"What's Wrong with the World\" he advocated a view called \"Distributionism\" that was best summed up by his expression that every man ought to be allowed to own \"three acres and a cow.\"";
        constans character* expected1 =
            "Though not known as a political thinker, his political influence has circled the world.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            expected0));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            expected1));
    }
}


    /* ==================================================
     * 26. Findere post "fired at us" ante "Peculiar"
     * ================================================== */

interior vacuum
_sectio_vicesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "I am inclined to think that this blaze may have been the casting of the huge gun, in the vast pit sunk into their planet, from which their shots were fired at us. Peculiar markings, as yet unexplained, were seen near the site of that outbreak during the next two oppositions.";
        constans character* expected0 =
            "I am inclined to think that this blaze may have been the casting of the huge gun, in the vast pit sunk into their planet, from which their shots were fired at us.";
        constans character* expected1 =
            "Peculiar markings, as yet unexplained, were seen near the site of that outbreak during the next two oppositions.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            expected0));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            expected1));
    }
}


    /* ==================================================
     * 27. Satan's cum smart apostrophe
     * ================================================== */

interior vacuum
_sectio_vicesima_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        /* \u2019 = \xe2\x80\x99 (right single quote / smart apostrophe) */
        constans character* input =
            "In all their little books of verse you will not find a finer defiance of God than Satan\xe2\x80\x99s. Nor will you find the grandeur of paganism felt as that fiery Christian felt it who described Faranata lifting his head as in disdain of hell.";
        constans character* expected0 =
            "In all their little books of verse you will not find a finer defiance of God than Satan\xe2\x80\x99s.";
        constans character* expected1 =
            "Nor will you find the grandeur of paganism felt as that fiery Christian felt it who described Faranata lifting his head as in disdain of hell.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            expected0));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            expected1));
    }
}


    /* ==================================================
     * 28. Smart single quotes 'Another time.'
     * ================================================== */

interior vacuum
_sectio_vicesima_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        /* \u2018 = \xe2\x80\x98 (left single), \u2019 = \xe2\x80\x99 (right single) */
        /* UTF-8: left single quote = E2 80 98, right single quote = E2 80 99 */
        constans character* input =
            "Well, I offered to bring you to her; but she said, \xe2\x80\x98" "Another time.\xe2\x80\x99 Never mind, old fellow, perhaps there'll be a smash, and you'll have a chance of rescuing her and cutting out the Duke of Strelsau!";
        constans character* expected0 =
            "Well, I offered to bring you to her; but she said, \xe2\x80\x98" "Another time.\xe2\x80\x99";
        constans character* expected1 =
            "Never mind, old fellow, perhaps there'll be a smash, and you'll have a chance of rescuing her and cutting out the Duke of Strelsau!";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            expected0));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            expected1));
    }
}


    /* ==================================================
     * 29. George II. (numerus Romanus)
     * ================================================== */

interior vacuum
_sectio_vicesima_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "Well then--and I must premise that I am going, perforce, to rake up the very scandal which my dear Lady Burlesdon wishes forgotten--in the year 1733, George II. sitting then on the throne, peace reigning for the moment, and the King and the Prince of Wales being not yet at loggerheads, there came on a visit to the English Court a certain prince, who was afterwards known to history as Rudolf the Third of Ruritania.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 30. Dialogus cum attributione (non findere)
     * ================================================== */

interior vacuum
_sectio_tricesima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "\"Nonsense!\" said she, tossing her head; and after a moment she went on: \"Now, here's Sir Jacob Borrodaile offering you exactly what you might be equal to.\"";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 31. Maiuscula post quote claudentem
     * ================================================== */

interior vacuum
_sectio_tricesima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "\"This is the end.\" The next sentence begins here.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "\"This is the end.\""));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "The next sentence begins here."));
    }
}


    /* ==================================================
     * 32. Dialogus cum 'I' attributione
     * ================================================== */

interior vacuum
_sectio_tricesima_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "\"A thousand thanks!\" I murmured.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 33. "Hello!" I said pattern
     * ================================================== */

interior vacuum
_sectio_tricesima_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
             constans character* input = "\"Hello!\" I said.";
                         chorda  textus;
        SententiaeFissioFructus  fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, I);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            input));
    }
}


    /* ==================================================
     * 34. "Goodbye!" I went... (findere)
     * ================================================== */

interior vacuum
_sectio_tricesima_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "\"Goodbye!\" I went to the store later.";
                         chorda textus;
        SententiaeFissioFructus fructus;

        textus   = _chorda_ex_literis(input, piscina);
        fructus  = sententia_fissio(textus, piscina);

        CREDO_AEQUALIS_I32(fructus.numerus, II);
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[ZEPHYRUM],
            "\"Goodbye!\""));
        CREDO_VERUM(_chorda_aequalis_literis(fructus.sententiae[I],
            "I went to the store later."));
    }
}


    /* ==================================================
     * 35. Probare sententia_numerare
     * ================================================== */

interior vacuum
_sectio_tricesima_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input =
            "First sentence. Second sentence. Third sentence.";
                    chorda textus;
                       i32 numerus;

        textus   = _chorda_ex_literis(input, piscina);
        numerus  = sententia_numerare(textus);

        CREDO_AEQUALIS_I32(numerus, III);
    }
}


    /* ==================================================
     * 36. Probare Iterator
     * ================================================== */

interior vacuum
_sectio_tricesima_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
        constans character* input = "First. Second. Third.";
                    chorda  textus;
         SententiaIterator* iter;
                    chorda  sent;

        textus  = _chorda_ex_literis(input, piscina);
        iter    = sententia_iterator_creare(textus, piscina);

        CREDO_NON_NIHIL(iter);

        /* Prima */
        sent = sententia_iterator_proxima(iter);
        CREDO_VERUM(_chorda_aequalis_literis(sent, "First."));
        CREDO_AEQUALIS_S32(sententia_iterator_index(iter), ZEPHYRUM);

        /* Secunda */
        sent = sententia_iterator_proxima(iter);
        CREDO_VERUM(_chorda_aequalis_literis(sent, "Second."));
        CREDO_AEQUALIS_S32(sententia_iterator_index(iter), I);

        /* Tertia */
        sent = sententia_iterator_proxima(iter);
        CREDO_VERUM(_chorda_aequalis_literis(sent, "Third."));
        CREDO_AEQUALIS_S32(sententia_iterator_index(iter), II);

        /* Retro ad secundam */
        sent = sententia_iterator_prior(iter);
        CREDO_VERUM(_chorda_aequalis_literis(sent, "Second."));
        CREDO_AEQUALIS_S32(sententia_iterator_index(iter), I);

        /* Numerus totalis */
        CREDO_AEQUALIS_I32(sententia_iterator_numerus(iter), III);

        /* Reset */
        sententia_iterator_reset(iter);
        CREDO_AEQUALIS_S32(sententia_iterator_index(iter), -1);
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "1. Rev. Mr. pattern", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "2. Multiplices abbreviationes", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "3. Abbreviationes maiusculae", _sectio_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "4. No. sequutum per numerum", _sectio_quarta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "5. Sententiae quotatae", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "6. Numeri decimales", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "7. Abbreviationes viarum", _sectio_septima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "8. Abbreviationes temporis", _sectio_octava, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "9. Rev. Dr. pattern", _sectio_nona, NIHIL, NIHIL, NIHIL, NIHIL },
    { "10. U.S. et U.K.", _sectio_decima, NIHIL, NIHIL, NIHIL, NIHIL },
    { "11. etc. ad finem", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "12. e.g. in parentheses", _sectio_decima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "13. Ellipsis", _sectio_decima_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "14. Interrogationes et exclamationes", _sectio_decima_quarta,
        NIHIL, NIHIL, NIHIL, NIHIL },
    { "15. Fr. et Sr.", _sectio_decima_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "16. Gradus militares", _sectio_decima_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "17. Tituli consecutivi", _sectio_decima_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "18. Prof. Dr. pattern", _sectio_decima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "19. Jr. et Sr. suffixes", _sectio_decima_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "20. Nomina Romana", _sectio_vicesima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "21. Litterae singulae", _sectio_vicesima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "22. a.u.c. et p.u.c.", _sectio_vicesima_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "23. Smart quotes (double)", _sectio_vicesima_tertia, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "24. Smart quotes 'orthodox'", _sectio_vicesima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "25. Straight double quotes", _sectio_vicesima_quinta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "26. Findere ante 'Peculiar'", _sectio_vicesima_sexta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "27. Satan's (smart apostrophe)", _sectio_vicesima_septima, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "28. Smart single quotes", _sectio_vicesima_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "29. George II. (Roman numeral)", _sectio_vicesima_nona, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "30. Dialogus cum attributione", _sectio_tricesima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "31. Maiuscula post quote", _sectio_tricesima_prima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "32. 'I murmured' attributio", _sectio_tricesima_secunda, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "33. 'Hello!' I said", _sectio_tricesima_tertia, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "34. 'I' cum multis verbis", _sectio_tricesima_quarta, NIHIL,
        NIHIL, NIHIL, NIHIL },
    { "35. sententia_numerare", _sectio_tricesima_quinta, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "36. Iterator", _sectio_tricesima_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("sententia_fissio", SECTIONES);
}
