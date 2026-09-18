/* probatio_materia_excusatio.c - Excusationes declaratae
 * (project-specs/excusatio-declarata-spec.md par. V)
 *
 * Arbor eadem forma ac probatio_materia_annotationes: par prius
 * annotationem fert, par alterum nullam. Sedes DISPARES consulto -
 * porta 'frater extra scopum superest' fixturam symmetricam transire
 * sineret.
 *
 * OCTETI COHAERENT: commentarium ab octeto 0 incipit, pares ab C et
 * CX - id est POST notam LONGISSIMAM (LV octeti). Ante emendationem
 * adnexionis (opus V) pares ab XXXVII sedebant et notae longiores eos
 * TEGEBANT; adnexio tunc possessorem TRIVII adhibebat, ergo sedes
 * incohaerentes nihil nocebant. Adnexio POSITIONALIS eas curat, et
 * fixtura incohaerens statim rubuit - fixtura per accidens sana
 * numquam diu manet.
 *
 * Diagnostica MANU aedificantur: probatio iuncturam probat (sedes
 * contra scopum, codex contra codicem), non derivationem.
 */

#include "latina.h"
#include "credo.h"
#include "materia_excusatio.h"
#include "materia_annotationes.h"
#include "materia_diagnostica.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "materia_lexicon.h"
#include "chorda.h"
#include "piscina.h"
#include "stml.h"
#include <stdio.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

enumeratio {
    G_FINIS = 0, G_IDENT, G_SPATIA, G_NOVA_LINEA, G_COMMENTUM,
    G_NUMERUS_GENERUM
};

hic_manens constans MateriaLexGenus GENERA[] = {
    { "FINIS",      "",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },
    { "IDENT",      NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SPATIA",     " ",   MATERIA_LEX_REPETITUM,
        MATERIA_MUNUS_SPATIUM },
    { "NOVA_LINEA", "\n",  MATERIA_LEX_TERMINATOR,
        MATERIA_MUNUS_LINEA },
    { "COMMENTUM",  NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_COMMENTUM }
};
hic_manens constans MateriaLexiconCoctum LEXICON = {
    GENERA, (i32)G_NUMERUS_GENERUM, "lex-", (s32)G_SPATIA
};

enumeratio { GR_RADIX = 0, GR_PAR, GR_VERBUM };
enumeratio { LP_APERTURA = 0, LP_LIBERI, LP_CLAUSURA };

#define NOTA_BONA      "# <tolera codex=\"lint:x\" (>causa vera"
#define NOTA_SINE      "# <tolera codex=\"lint:x\"/>"
#define NOTA_FRACTA    "# <tolera codex=\"lint:x\""
#define NOTA_NOTA      "# <tolera codex=\"par/notus\" (>causa vera"
#define NOTA_IGNOTA    "# <tolera codex=\"par/ignotus\" (>causa vera"
#define NOTA_PICTA \
    "# <tolera codex=\"crusta:grex/tok_clausura\" (>causa vera"

/* tabula declaratorum: codex UNUS notus. Par 'notus'/'ignotus'
 * DISCERNIT - probatio quae ignotum solum ferret ab excusatione
 * quae SEMPER ignota est transiri posset. */
hic_manens constans MateriaTabDiagnosticum TABULA[] = {
    { ZEPHYRUM, (s32)-I, (s32)MATERIA_DIAGNOSTICUM_GENUS,
      (s32)MATERIA_GRAVITAS_ERRATUM, "par/notus", "causa ficta" }
};
hic_manens constans MateriaDiagnosticaCocta DECLARATA = {
    TABULA, (i32)I, NIHIL, ZEPHYRUM
};

hic_manens MateriaToken*
_lex (
               Piscina* piscina,
                   s32  genus,
    constans character* valor,
                   s32  offset)
{
    redde materia_token_creare(piscina, &FORMA, genus,
        chorda_ex_literis(valor, piscina), offset, (i32)I,
        (i32)(offset + I), ZEPHYRUM);
}

hic_manens MateriaNodus*
_par (
               Piscina* piscina,
    constans character* interius,
                   s32  offset,
    constans character* commentarium)
{
    MateriaNodus* nodus = materia_nodus_creare(piscina, (s32)GR_PAR,
                              (i32)III);
    MateriaNodus* intus;
    MateriaToken* apertura;
             s32  post;

    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    apertura = _lex(piscina, (s32)G_IDENT, "(", offset);
    si (commentarium != NIHIL)
    {
        MateriaToken* nota = _lex(piscina, (s32)G_COMMENTUM,
                                 commentarium, ZEPHYRUM);

        si (!materia_token_trivia_ante_ponere(apertura, piscina,
                &nota, (i32)I))
        {
            redde NIHIL;
        }
    }
    si (!materia_nodus_ponere(nodus, (i32)LP_APERTURA,
            materia_valor_token(apertura), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    intus = materia_nodus_creare(piscina, (s32)GR_VERBUM, (i32)I);
    si (intus == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(intus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, interius,
                offset + I)), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    si (!materia_nodus_appendere(piscina, nodus, (i32)LP_LIBERI,
            materia_valor_nodus(intus), MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    post = offset + I + (s32)strlen(interius);
    si (!materia_nodus_ponere(nodus, (i32)LP_CLAUSURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, ")",
                post)), MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde nodus;
}

hic_manens Xar*
_annotationes (
                         Piscina* piscina,
    constans MateriaLexiconRatum* ratum,
              constans character* commentarium)
{
    MateriaNodus* radix = materia_nodus_creare(piscina,
                              (s32)GR_RADIX, (i32)I);
    MateriaNodus* prius;
    MateriaNodus* alterum;

    si (radix == NIHIL)
    {
        redde NIHIL;
    }
    prius    = _par(piscina, "alpha", (s32)100, commentarium);
    alterum  = _par(piscina, "beta",  (s32)110, NIHIL);
    si (prius == NIHIL || alterum == NIHIL)
    {
        redde NIHIL;
    }
    si (   !materia_nodus_appendere(piscina, radix, ZEPHYRUM,
               materia_valor_nodus(prius), MATERIA_LOCUS_LISTA_NODUS)
        || !materia_nodus_appendere(piscina, radix, ZEPHYRUM,
               materia_valor_nodus(alterum),
               MATERIA_LOCUS_LISTA_NODUS))
    {
        redde NIHIL;
    }
    redde materia_annotationes_colligere(piscina, radix, ratum, "#",
        NIHIL, NIHIL);
}

/* diagnosticum manu: codex et sedes sola momenti sunt */
hic_manens b32
_diag (
                   Xar* in,
    constans character* codex,
                   s32  initium,
                   s32  finis)
{
    MateriaDiagnosticum* d = (MateriaDiagnosticum*)xar_addere(in);

    si (d == NIHIL)
    {
        redde FALSUM;
    }
    memset(d, ZEPHYRUM, magnitudo(*d));
    d->gravitas         = (s32)MATERIA_GRAVITAS_ERRATUM;
    d->codex            = codex;
    d->causa            = "causa ficta";
    d->tractus.initium  = initium;
    d->tractus.finis    = finis;
    d->tractus.linea    = (i32)I;
    d->tractus.columna  = (i32)I;
    redde VERUM;
}

hic_manens b32
_habet (
                   Xar* exitus,
    constans character* codex)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(exitus); k++)
    {
        constans MateriaDiagnosticum* d =
            (constans MateriaDiagnosticum*)xar_obtinere(exitus, k);

        si (strcmp(d->codex, codex) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

integer
principale (
    vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_excusatio",
        262144);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &LEXICON,
        &iudicium));

    {
        Xar* a = _annotationes(piscina, &ratum, NOTA_BONA);
        Xar* d = xar_creare(piscina,
                     (i32)magnitudo(MateriaDiagnosticum));
        Xar* e;

        imprimere("\n--- I. Excusatum cadit, frater superest ---\n");
        CREDO_NON_NIHIL (a);
        CREDO_AEQUALIS_I32 (xar_numerus(a), (i32)I);
        /* XXXVIII-XLIII intra par prius; XLVI-L intra alterum */
        CREDO_VERUM (_diag(d, "lint:x", (s32)101, (s32)106));
        CREDO_VERUM (_diag(d, "lint:x", (s32)111, (s32)115));
        CREDO_VERUM (_diag(d, "lint:y", (s32)102, (s32)105));
        e = materia_excusatio_applicare(piscina, d, a, NIHIL,
                NIHIL);
        CREDO_NON_NIHIL (e);
        /* excusatum unum cadit; frater EXTRA scopum superest; codex
         * alius intra scopum superest quoque */
        CREDO_AEQUALIS_I32 (xar_numerus(e), (i32)II);
        CREDO_FALSUM (_habet(e, MATERIA_CODEX_EXCUSATIO_MORTUA));
        CREDO_VERUM (_habet(e, "lint:y"));
    }

    {
        Xar* a = _annotationes(piscina, &ratum, NOTA_NOTA);
        Xar* d = xar_creare(piscina,
                     (i32)magnitudo(MateriaDiagnosticum));
                                 Xar* e;
        constans MateriaDiagnosticum* primum;

        imprimere("\n--- II. Excusatio MORTUA nominatur ---\n");
        /* nihil intra scopum: tolera nihil absorbet. CODEX
         * NON-LINTRIS consulto (porta IX par est). */
        CREDO_VERUM (_diag(d, "lint:x", (s32)111, (s32)115));
        e = materia_excusatio_applicare(piscina, d, a, NIHIL,
                NIHIL);
        CREDO_NON_NIHIL (e);
        CREDO_AEQUALIS_I32 (xar_numerus(e), (i32)II);
        CREDO_VERUM (_habet(e, MATERIA_CODEX_EXCUSATIO_MORTUA));
        /* ORDO: mortua in commentario sedet (octetus 0), ergo ANTE
         * inventum octeti XLVI cadit */
        primum = (constans MateriaDiagnosticum*)xar_obtinere(e,
                     ZEPHYRUM);
        CREDO_AEQUALIS_S32 (primum->tractus.initium, ZEPHYRUM);
        CREDO_VERUM (strcmp(primum->codex,
            MATERIA_CODEX_EXCUSATIO_MORTUA) == ZEPHYRUM);
    }

    {
        Xar* a = _annotationes(piscina, &ratum, NOTA_SINE);
        Xar* d = xar_creare(piscina,
                     (i32)magnitudo(MateriaDiagnosticum));
        Xar* e;

        imprimere("\n--- III. Sine causa: NON excusat ---\n");
        CREDO_NON_NIHIL (a);
        CREDO_AEQUALIS_I32 (xar_numerus(a), (i32)I);
        CREDO_VERUM (_diag(d, "lint:x", (s32)101, (s32)106));
        e = materia_excusatio_applicare(piscina, d, a, NIHIL,
                NIHIL);
        CREDO_NON_NIHIL (e);
        /* inventum SUPEREST, et excusatio ipsa nominatur */
        CREDO_AEQUALIS_I32 (xar_numerus(e), (i32)II);
        CREDO_VERUM (_habet(e, "lint:x"));
        CREDO_VERUM (_habet(e, MATERIA_CODEX_EXCUSATIO_SINE_CAUSA));
    }

    {
        Xar* a = _annotationes(piscina, &ratum, NOTA_FRACTA);
        Xar* d = xar_creare(piscina,
                     (i32)magnitudo(MateriaDiagnosticum));
        Xar* e;

        imprimere("\n--- IV. Fractum nominatur, non tacet ---\n");
        CREDO_NON_NIHIL (a);
        CREDO_AEQUALIS_I32 (xar_numerus(a), (i32)I);
        CREDO_VERUM (_diag(d, "lint:x", (s32)101, (s32)106));
        e = materia_excusatio_applicare(piscina, d, a, NIHIL,
                NIHIL);
        CREDO_NON_NIHIL (e);
        CREDO_AEQUALIS_I32 (xar_numerus(e), (i32)II);
        CREDO_VERUM (_habet(e, "lint:x"));
        CREDO_VERUM (_habet(e, MATERIA_CODEX_EXCUSATIO_FRACTA));
    }

    {
        Xar* a = _annotationes(piscina, &ratum, NOTA_IGNOTA);
        Xar* d = xar_creare(piscina,
                     (i32)magnitudo(MateriaDiagnosticum));
        Xar* e;

        imprimere("\n--- V. Codex non declaratus nominatur ---\n");
        CREDO_VERUM (_diag(d, "par/notus", (s32)101, (s32)106));
        e = materia_excusatio_applicare(piscina, d, a, &DECLARATA,
                NIHIL);
        CREDO_NON_NIHIL (e);
        CREDO_AEQUALIS_I32 (xar_numerus(e), (i32)II);
        CREDO_VERUM (_habet(e, MATERIA_CODEX_EXCUSATIO_IGNOTA));
        CREDO_VERUM (_habet(e, "par/notus"));
    }

    {
        Xar* a = _annotationes(piscina, &ratum, NOTA_NOTA);
        Xar* d = xar_creare(piscina,
                     (i32)magnitudo(MateriaDiagnosticum));
        Xar* e;

        imprimere("\n--- VI. Codex DECLARATUS excusat ---\n");
        /* par cum V: eadem tabula, codex notus - ergo probatio
         * discernit, non semper IGNOTA reddit. Et hoc ostendit
         * diagnostica DECLARATA excusari, non lintris sola. */
        CREDO_VERUM (_diag(d, "par/notus", (s32)101, (s32)106));
        e = materia_excusatio_applicare(piscina, d, a, &DECLARATA,
                NIHIL);
        CREDO_NON_NIHIL (e);
        CREDO_AEQUALIS_I32 (xar_numerus(e), ZEPHYRUM);
    }

    {
        Xar* a = _annotationes(piscina, &ratum, NOTA_PICTA);
        Xar* d = xar_creare(piscina,
                     (i32)magnitudo(MateriaDiagnosticum));
        Xar* e;

        imprimere("\n--- VII. Forma PICTA codicis excusat ---\n");
        /* Pictor 'crusta:' codici sine ':' praefigit, ergo usor
         * 'crusta:grex/tok_clausura' LEGIT dum codex ipse nudus est.
         * EX1 promittit id quod usor VIDIT sufficere. */
        CREDO_VERUM (_diag(d, "grex/tok_clausura", (s32)101, (s32)106));
        e = materia_excusatio_applicare(piscina, d, a, NIHIL,
                "crusta");
        CREDO_NON_NIHIL (e);
        CREDO_AEQUALIS_I32 (xar_numerus(e), ZEPHYRUM);
    }

    {
        Xar* a = _annotationes(piscina, &ratum, NOTA_PICTA);
        Xar* d = xar_creare(piscina,
                     (i32)magnitudo(MateriaDiagnosticum));
        Xar* e;

        imprimere("\n--- VIII. Sine grammatica: picta CADIT ---\n");
        /* Par cum VII: sine grammatica congruentia fieri NON potest,
         * ergo probatio VII grammaticam REVERA adhibet et non forte
         * viridis est. */
        CREDO_VERUM (_diag(d, "grex/tok_clausura", (s32)101, (s32)106));
        e = materia_excusatio_applicare(piscina, d, a, NIHIL, NIHIL);
        CREDO_NON_NIHIL (e);
        CREDO_AEQUALIS_I32 (xar_numerus(e), (i32)II);
        CREDO_VERUM (_habet(e, "grex/tok_clausura"));
        CREDO_VERUM (_habet(e, MATERIA_CODEX_EXCUSATIO_MORTUA));
    }

    {
        Xar* a = _annotationes(piscina, &ratum, NOTA_BONA);
        Xar* d = xar_creare(piscina,
                     (i32)magnitudo(MateriaDiagnosticum));
        Xar* e;

        imprimere("\n--- IX. Codex LINTRIS: mortua NON iudicata ---\n");
        /* Par cum II: eadem arbor, eadem sedes, codex SOLUS mutatus.
         * Regula gradus II quae victimam pareret non cucurrit, ergo
         * 'nihil absorbuit' nihil probat. Mensuratum in transitu
         * operis V: sine hac porta XIV excusationes VERAE domum
         * totam rubram fecerunt (EX8). */
        CREDO_VERUM (_diag(d, "lint:x", (s32)111, (s32)115));
        e = materia_excusatio_applicare(piscina, d, a, NIHIL, NIHIL);
        CREDO_NON_NIHIL (e);
        CREDO_AEQUALIS_I32 (xar_numerus(e), (i32)I);
        CREDO_FALSUM (_habet(e, MATERIA_CODEX_EXCUSATIO_MORTUA));
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
