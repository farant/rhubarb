/* probatio_capitula.c - Probationes indicis capitulorum */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "capitula.h"
#include "credo.h"
#include <stdio.h>

interior Piscina* arena = NIHIL;

interior IndexLibri
_legere (constans character* textus)
{
    redde capitula_legere(chorda_ex_literis(textus, arena), arena);
}

interior Capitulum*
_capitulum (IndexLibri idx, i32 positio)
{
    redde (Capitulum*)xar_obtinere(idx.capitula, positio);
}

interior b32
_romanus_valet (constans character* s)
{
    i32 valor = ZEPHYRUM;
    redde capitula_numerus_romanus(chorda_ex_literis(s, arena), &valor);
}

interior i32
_romanus_valor (constans character* s)
{
    i32 valor = ZEPHYRUM;
    (vacuum)capitula_numerus_romanus(chorda_ex_literis(s, arena), &valor);
    redde valor;
}

s32 principale (vacuum)
{
    b32 praeteritus;

    arena = piscina_generare_dynamicum("probatio_capitula", 65536);
    si (!arena)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(arena);

    /* ========================================================
     * PROBARE: numeri Romani - forma subtractiva stricta
     * ======================================================== */
    {
        imprimere("\n--- Probans capitula_numerus_romanus ---\n");

        CREDO_VERUM (_romanus_valet("I"));
        CREDO_VERUM (_romanus_valet("IV"));
        CREDO_VERUM (_romanus_valet("XIV"));
        CREDO_VERUM (_romanus_valet("XXXIX"));
        CREDO_VERUM (_romanus_valet("MCMXCIV"));
        CREDO_VERUM (_romanus_valet("MMMCMXCIX"));

        CREDO_AEQUALIS_I32 (_romanus_valor("I"), I);
        CREDO_AEQUALIS_I32 (_romanus_valor("IV"), IV);
        CREDO_AEQUALIS_I32 (_romanus_valor("XIV"), XIV);
        CREDO_AEQUALIS_I32 (_romanus_valor("XXXIX"), XXXIX);
        CREDO_AEQUALIS_I32 (_romanus_valor("MCMXCIV"), 1994);

        /* formae illicitae */
        CREDO_FALSUM (_romanus_valet(""));
        CREDO_FALSUM (_romanus_valet("IIII"));   /* cursus nimius */
        CREDO_FALSUM (_romanus_valet("VV"));     /* V bis */
        CREDO_FALSUM (_romanus_valet("IL"));     /* par illicitum */
        CREDO_FALSUM (_romanus_valet("IXX"));    /* post IX nihil */
        CREDO_FALSUM (_romanus_valet("XIIX"));   /* grex crescens */
        CREDO_FALSUM (_romanus_valet("iv"));     /* minuscula */
        CREDO_FALSUM (_romanus_valet("A"));

        /* VERBA ANGLICA ex litteris Romanis - causa ipsa regulae
         * strictae. Omnia respui debent. */
        CREDO_FALSUM (_romanus_valet("DID"));
        CREDO_FALSUM (_romanus_valet("MILD"));
        CREDO_FALSUM (_romanus_valet("CIVIC"));
        CREDO_FALSUM (_romanus_valet("VIM"));
        CREDO_FALSUM (_romanus_valet("DIM"));
        CREDO_FALSUM (_romanus_valet("LIVID"));

        /* MIX numerus VERUS est (1009) - superest consulto, et
         * praevisio consumptoris eum monstrat */
        CREDO_VERUM (_romanus_valet("MIX"));
    }

    /* ========================================================
     * PROBARE: index planus, nullae inscriptiones
     * ======================================================== */
    {
        IndexLibri idx = _legere(
            "Prooemium\n"
            "De methodo\n"
            "Epilogus\n");

        imprimere("\n--- Probans indicem planum ---\n");

        CREDO_AEQUALIS_I32 (xar_numerus(idx.capitula), III);
        CREDO_AEQUALIS_I32 (idx.gradus_maximus, ZEPHYRUM);

        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, ZEPHYRUM)->titulus,
            "Prooemium");
        CREDO_CHORDA_VACUA (_capitulum(idx, ZEPHYRUM)->inscriptio);
        CREDO_AEQUALIS_I32 (_capitulum(idx, ZEPHYRUM)->ordo, I);
        CREDO_AEQUALIS_I32 (_capitulum(idx, ZEPHYRUM)->linea, I);

        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, II)->titulus,
            "Epilogus");
        CREDO_AEQUALIS_I32 (_capitulum(idx, II)->ordo, III);
    }

    /* ========================================================
     * PROBARE: forma <inscriptio> <separator> <titulus>
     * ======================================================== */
    {
        IndexLibri idx = _legere(
            "1 - De methodo\n"
            "II - De materia\n"
            "Capitulum 3 : De fine\n"
            "Appendix 4 \xE2\x80\x94 Tabulae\n");

        imprimere("\n--- Probans separatores ---\n");

        CREDO_AEQUALIS_I32 (xar_numerus(idx.capitula), IV);

        CREDO_CHORDA_AEQUALIS_LITERIS (
            _capitulum(idx, ZEPHYRUM)->inscriptio, "1");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            _capitulum(idx, ZEPHYRUM)->titulus, "De methodo");

        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, I)->inscriptio,
            "II");
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, I)->titulus,
            "De materia");

        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, II)->inscriptio,
            "Capitulum 3");
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, II)->titulus,
            "De fine");

        /* lineola em (UTF-8, tres octeti) */
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, III)->inscriptio,
            "Appendix 4");
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, III)->titulus,
            "Tabulae");
    }

    /* ========================================================
     * PROBARE: forma <signum><punctum> <titulus>
     * ======================================================== */
    {
        IndexLibri idx = _legere(
            "1. De methodo\n"
            "2) De materia\n"
            "XIV. De fine\n"
            "1.2.3 De sectione\n"
            "I.4 De mixto\n");

        imprimere("\n--- Probans signa numeralia ---\n");

        CREDO_AEQUALIS_I32 (xar_numerus(idx.capitula), V);

        /* punctum finale DETRAHITUR: ancora '1' est, non '1.' */
        CREDO_CHORDA_AEQUALIS_LITERIS (
            _capitulum(idx, ZEPHYRUM)->inscriptio, "1");
        CREDO_CHORDA_AEQUALIS_LITERIS (
            _capitulum(idx, ZEPHYRUM)->titulus, "De methodo");

        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, I)->inscriptio,
            "2");
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, II)->inscriptio,
            "XIV");

        /* puncta INTERIORA manent */
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, III)->inscriptio,
            "1.2.3");
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, III)->titulus,
            "De sectione");
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, IV)->inscriptio,
            "I.4");
    }

    /* ========================================================
     * PROBARE: DECIPULA SUBTITULI - separator qui inscriptionem
     * NON facit. Regula lata hic "The Long Road" ut indicem
     * scriberet, et ancora MENTIENS nasceretur.
     * ======================================================== */
    {
        IndexLibri idx = _legere(
            "The Long Road - and What Came After\n"
            "Chapter One\n"
            "I am the walrus\n");

        imprimere("\n--- Probans decipulam subtituli ---\n");

        CREDO_AEQUALIS_I32 (xar_numerus(idx.capitula), III);

        CREDO_CHORDA_VACUA (_capitulum(idx, ZEPHYRUM)->inscriptio);
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, ZEPHYRUM)->titulus,
            "The Long Road - and What Came After");

        CREDO_CHORDA_VACUA (_capitulum(idx, I)->inscriptio);
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, I)->titulus,
            "Chapter One");

        /* 'I' numerus Romanus VERUS est: hic falsum positivum
         * MANET et notum est (vide capitula.h §III). Fixum ne
         * tacite mutetur. */
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, II)->inscriptio,
            "I");
    }

    /* ========================================================
     * PROBARE: nidificatio - duo spatia, quattuor, tabulus
     * ======================================================== */
    {
        IndexLibri idx = _legere(
            "Pars I\n"
            "  1. Prima\n"
            "  2. Secunda\n"
            "Pars II\n"
            "    3. Tertia\n"
            "\t4. Quarta\n");

        imprimere("\n--- Probans nidificationem ---\n");

        CREDO_AEQUALIS_I32 (xar_numerus(idx.capitula), VI);
        CREDO_AEQUALIS_I32 (idx.gradus_maximus, I);

        CREDO_AEQUALIS_I32 (_capitulum(idx, ZEPHYRUM)->gradus, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_capitulum(idx, I)->gradus, I);
        CREDO_AEQUALIS_I32 (_capitulum(idx, II)->gradus, I);

        /* egressus ad gradum ZEPHYRUM, deinde ingressus iterum -
         * latitudo IV, non II: acervus latitudinem NON memorat
         * ultra egressum */
        CREDO_AEQUALIS_I32 (_capitulum(idx, III)->gradus, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_capitulum(idx, IV)->gradus, I);

        /* tabulus ad columnam IV procedit = eadem latitudo */
        CREDO_AEQUALIS_I32 (_capitulum(idx, V)->gradus, I);

        /* ordo PLANUS est, per gradus continuus */
        CREDO_AEQUALIS_I32 (_capitulum(idx, V)->ordo, VI);
    }

    /* ========================================================
     * PROBARE: nidificatio profunda + egressus partialis
     * ======================================================== */
    {
        IndexLibri idx = _legere(
            "A\n"
            "  B\n"
            "    C\n"
            "  D\n"
            "E\n");

        imprimere("\n--- Probans egressum partialem ---\n");

        CREDO_AEQUALIS_I32 (_capitulum(idx, ZEPHYRUM)->gradus, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (_capitulum(idx, I)->gradus, I);
        CREDO_AEQUALIS_I32 (_capitulum(idx, II)->gradus, II);
        CREDO_AEQUALIS_I32 (_capitulum(idx, III)->gradus, I);
        CREDO_AEQUALIS_I32 (_capitulum(idx, IV)->gradus, ZEPHYRUM);
        CREDO_AEQUALIS_I32 (idx.gradus_maximus, II);
    }

    /* ========================================================
     * PROBARE: fines linearum omnium generum
     * ======================================================== */
    {
        IndexLibri crlf = _legere("Unum\r\nDuo\r\nTria");
        IndexLibri cr   = _legere("Unum\rDuo\rTria");
        IndexLibri sine = _legere("Unum\nDuo\nTria");

        imprimere("\n--- Probans fines linearum ---\n");

        /* linea ULTIMA sine fine legitima est in omnibus tribus */
        CREDO_AEQUALIS_I32 (xar_numerus(crlf.capitula), III);
        CREDO_AEQUALIS_I32 (xar_numerus(cr.capitula), III);
        CREDO_AEQUALIS_I32 (xar_numerus(sine.capitula), III);

        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(crlf, II)->titulus,
            "Tria");
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(cr, II)->titulus,
            "Tria");
    }

    /* ========================================================
     * PROBARE: lineae vacuae et albae
     * ======================================================== */
    {
        IndexLibri idx = _legere(
            "Unum\n"
            "\n"
            "   \n"
            "\t\n"
            "  Duo  \n"
            "\n");

        imprimere("\n--- Probans lineas vacuas ---\n");

        CREDO_AEQUALIS_I32 (xar_numerus(idx.capitula), II);

        /* linea alba gradum NON movet: 'Duo' indentatum est, ergo
         * gradus I - lineae vacuae inter eas nihil dicunt */
        CREDO_AEQUALIS_I32 (_capitulum(idx, I)->gradus, I);

        /* spatium finale praeciditur */
        CREDO_CHORDA_AEQUALIS_LITERIS (_capitulum(idx, I)->titulus,
            "Duo");

        /* numerus lineae FONTIS servatur, non numerus capituli */
        CREDO_AEQUALIS_I32 (_capitulum(idx, I)->linea, V);
    }

    /* ========================================================
     * PROBARE: casus limitis
     * ======================================================== */
    {
        IndexLibri vacuus  = _legere("");
        IndexLibri albus   = _legere("\n\n   \n");
        IndexLibri signum  = _legere("1.\n");
        IndexLibri longa   = _legere(
            "Praefatio ad editionem tertiam - De consilio\n");

        imprimere("\n--- Probans casus limitis ---\n");

        CREDO_AEQUALIS_I32 (xar_numerus(vacuus.capitula), ZEPHYRUM);
        CREDO_AEQUALIS_I32 (xar_numerus(albus.capitula), ZEPHYRUM);

        /* '1.' solum: nihil sequitur, ergo titulus est, non index -
         * titulus VACUUS numquam redditur */
        CREDO_AEQUALIS_I32 (xar_numerus(signum.capitula), I);
        CREDO_CHORDA_VACUA (_capitulum(signum, ZEPHYRUM)->inscriptio);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            _capitulum(signum, ZEPHYRUM)->titulus, "1.");

        /* inscriptio ultra XXIV characteres respuitur */
        CREDO_CHORDA_VACUA (_capitulum(longa, ZEPHYRUM)->inscriptio);
        CREDO_CHORDA_AEQUALIS_LITERIS (
            _capitulum(longa, ZEPHYRUM)->titulus,
            "Praefatio ad editionem tertiam - De consilio");
    }

    /* ========================================================
     * Compendium
     * ======================================================== */

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    alioquin
    {
        redde I;
    }
}
