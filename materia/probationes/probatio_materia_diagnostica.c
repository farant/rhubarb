/* probatio_materia_diagnostica.c - Diagnostica derivata ex arbore
 * (materia-sedes par. VIII, B2)
 *
 * Registrum manu scriptum (radix, par, verbum, separator, malum) cum
 * tabula diagnosticorum trium specierum: GENUS (malum), ABSENTIA
 * (par/tok_clausura), VACUA (par/liberi); separator INANIS. Probatur:
 * arbor sana nihil dat; clausura absens punctum in fine nodi; lista
 * non scripta et lista separatore sola 'vacua'; nodus sine lexemate
 * punctum POST lexema prius (ordo praeordinis); ordo octetorum ruptus
 * (lexema post lexema posterius) cum lexemate suo; usus iteratus
 * semel; 'emissa' parsatoris cum tractu ex nodo computato; tractus
 * refutationis scriptoris (lexema cuius valor sequentiam claudentem
 * propriam fert - NUL id olim faciebat, sed attributum 'nul' eum nunc
 * fert, 2026-09-19).
 */

#include "latina.h"
#include "credo.h"
#include "materia_diagnostica.h"
#include "materia_arbor.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

hic_manens constans MateriaTokenForma FORMA = { ZEPHYRUM };

enumeratio {
    G_FINIS = 0, G_IDENT, G_SPATIA, G_NOVA_LINEA, G_NUMERUS_GENERUM
};

hic_manens constans MateriaLexGenus GENERA[] = {
    { "FINIS",      "",    MATERIA_LEX_FIXUM,
        MATERIA_MUNUS_FINIS },
    { "IDENT",      NIHIL, MATERIA_LEX_VERBATIM,
        MATERIA_MUNUS_SUBSTANTIVUM },
    { "SPATIA",     " ",   MATERIA_LEX_REPETITUM,
        MATERIA_MUNUS_SPATIUM },
    { "NOVA_LINEA", "\n",  MATERIA_LEX_TERMINATOR,
        MATERIA_MUNUS_LINEA }
};
hic_manens constans MateriaLexiconCoctum LEXICON = {
    GENERA, (i32)G_NUMERUS_GENERUM, "lex-", (s32)G_SPATIA
};

enumeratio { GR_RADIX = 0, GR_PAR, GR_VERBUM, GR_SEPARATOR, GR_MALUM };
/* loci intra genus: par {0 tok_apertura, 1 liberi, 2 tok_clausura} */
enumeratio { LP_APERTURA = 0, LP_LIBERI, LP_CLAUSURA };

hic_manens constans MateriaTabLocus LOCI[] = {
    { "liberi",       (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_apertura", (s32)MATERIA_LOCUS_TOKEN },
    { "liberi",       (s32)MATERIA_LOCUS_LISTA_NODUS },
    { "tok_clausura", (s32)MATERIA_LOCUS_TOKEN },
    { "tok",          (s32)MATERIA_LOCUS_TOKEN },
    { "tok",          (s32)MATERIA_LOCUS_TOKEN },
    { "tokens",       (s32)MATERIA_LOCUS_LISTA_TOKEN }
};
hic_manens constans MateriaTabGenus GEN[] = {
    { "radix",     ZEPHYRUM, (i32)I },
    { "par",       (i32)I,   (i32)III },
    { "verbum",    (i32)IV,  (i32)I },
    { "separator", (i32)V,   (i32)I },
    { "malum",     (i32)VI,  (i32)I }
};
hic_manens constans MateriaRegistrumCoctum REG = {
    GEN, (i32)V, LOCI, (i32)VII
};

hic_manens constans MateriaTabDiagnosticum TABULA[] = {
    { (s32)GR_PAR, (s32)LP_CLAUSURA,
      (s32)MATERIA_DIAGNOSTICUM_ABSENTIA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "par/tok_clausura", "')' exspectata" },
    { (s32)GR_PAR, (s32)LP_LIBERI,
      (s32)MATERIA_DIAGNOSTICUM_VACUA,
      (s32)MATERIA_GRAVITAS_ERRATUM,
      "par/liberi", "lista vacua" },
    { (s32)GR_MALUM, (s32)-I,
      (s32)MATERIA_DIAGNOSTICUM_GENUS,
      (s32)MATERIA_GRAVITAS_MONITUM,
      "malum", "malum" }
};
hic_manens constans s32 INANIA[] = { (s32)GR_SEPARATOR };
hic_manens constans MateriaDiagnosticaCocta DIAGNOSTICA = {
    TABULA, (i32)III, INANIA, (i32)I
};

hic_manens MateriaToken*
_lex (
               Piscina* piscina,
                   s32  genus,
    constans character* valor,
                   s32  offset,
                   i32  columna)
{
    redde materia_token_creare(piscina, &FORMA, genus,
        chorda_ex_literis(valor, piscina), offset, (i32)I, columna,
        ZEPHYRUM);
}

/* nodus unius loci lexematis (verbum, separator) */
hic_manens MateriaNodus*
_nodus_lexematis (
               Piscina* piscina,
                   s32  genus,
    constans character* valor,
                   s32  offset,
                   i32  columna)
{
    MateriaNodus* nodus = materia_nodus_creare(piscina, genus, (i32)I);

    si (nodus == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_nodus_ponere(nodus, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, valor,
                offset, columna)),
            MATERIA_LOCUS_TOKEN))
    {
        redde NIHIL;
    }
    redde nodus;
}

hic_manens MateriaNodus*
_radix_creare (
    Piscina* piscina)
{
    redde materia_nodus_creare(piscina, (s32)GR_RADIX, (i32)I);
}

hic_manens b32
_radici_addere (
         Piscina* piscina,
    MateriaNodus* radix,
    MateriaNodus* liberum)
{
    redde materia_nodus_appendere(piscina, radix, ZEPHYRUM,
        materia_valor_nodus(liberum), MATERIA_LOCUS_LISTA_NODUS);
}

/* ordinem k-tum contra exspectata conferre; divergentia impressa */
hic_manens b32
_ordo_congruit (
                   Xar* exitus,
                   i32  indicium,
    constans character* codex,
                   s32  gravitas,
                   s32  initium,
                   s32  finis,
                   i32  linea,
                   i32  columna)
{
    constans MateriaDiagnosticum* r;

    si (exitus == NIHIL || indicium >= xar_numerus(exitus))
    {
        imprimere("    ordo %u abest\n", indicium);
        redde FALSUM;
    }
    r = (constans MateriaDiagnosticum*)xar_obtinere(exitus, indicium);
    si (   strcmp(r->codex, codex) == ZEPHYRUM
        && r->gravitas             == gravitas
        && r->tractus.initium      == initium
        && r->tractus.finis        == finis
        && r->tractus.linea        == linea
        && r->tractus.columna      == columna)
    {
        redde VERUM;
    }
    imprimere("    ordo %u: %s [%d] %d-%d %u:%u (exspectatum %s [%d] "
        "%d-%d %u:%u)\n", indicium, r->codex, (integer)r->gravitas,
        (integer)r->tractus.initium, (integer)r->tractus.finis,
        r->tractus.linea, r->tractus.columna, codex,
        (integer)gravitas, (integer)initium, (integer)finis, linea,
        columna);
    redde FALSUM;
}

/* Apertura DERIVATA (ABSENTIA sola): tractus nodi ipsius cum nota
 * MATERIA_NOTA_COEPIT, et primaria notam MATERIA_NOTA_EXSPECTATUR
 * accipit. NUMERUS SOLUS NON SUFFICIT - sedes tractu falso plena
 * numerum transire sinit, ergo initium et finis quoque figuntur. */
hic_manens b32
_apertura_congruit (
    Xar* exitus,
    i32  indicium,
    s32  initium,
    s32  finis)
{
    constans MateriaDiagnosticum* r;

    si (exitus == NIHIL || indicium >= xar_numerus(exitus))
    {
        imprimere("    ordo %u abest\n", indicium);
        redde FALSUM;
    }
    r = (constans MateriaDiagnosticum*)xar_obtinere(exitus, indicium);
    si (   r->numerus_relatorum                      == (i32)I
        && r->relata                                 != NIHIL
        && r->relata[ZEPHYRUM].tractus.initium       == initium
        && r->relata[ZEPHYRUM].tractus.finis         == finis
        && r->nota                                   != NIHIL
        && strcmp(r->nota, MATERIA_NOTA_EXSPECTATUR) == ZEPHYRUM
        && r->relata[ZEPHYRUM].nota                  != NIHIL
        && strcmp(r->relata[ZEPHYRUM].nota, MATERIA_NOTA_COEPIT)
               == ZEPHYRUM)
    {
        redde VERUM;
    }
    imprimere("    apertura %u: relata %u, tractus %d-%d (exspectatum "
        "1, %d-%d)\n", indicium, r->numerus_relatorum,
        (integer)(r->relata != NIHIL
            ? r->relata[ZEPHYRUM].tractus.initium : (s32)-I),
        (integer)(r->relata != NIHIL
            ? r->relata[ZEPHYRUM].tractus.finis : (s32)-I),
        (integer)initium, (integer)finis);
    redde FALSUM;
}

/* Sedes UNA: nihil relatum, nulla nota - exitus talis diagnostici
 * octetim idem manet ac ante sedes multiplices (GENUS, VACUA). */
hic_manens b32
_sedes_unica (
    Xar* exitus,
    i32  indicium)
{
    constans MateriaDiagnosticum* r;

    si (exitus == NIHIL || indicium >= xar_numerus(exitus))
    {
        imprimere("    ordo %u abest\n", indicium);
        redde FALSUM;
    }
    r = (constans MateriaDiagnosticum*)xar_obtinere(exitus, indicium);
    si (   r->numerus_relatorum == ZEPHYRUM
        && r->relata            == NIHIL
        && r->nota              == NIHIL)
    {
        redde VERUM;
    }
    imprimere("    ordo %u sedes plures inopinatas fert: %u\n",
        indicium, r->numerus_relatorum);
    redde FALSUM;
}

s32
principale (vacuum)
{
                    b32  praeteritus;
                Piscina* piscina;
    MateriaLexiconRatum  ratum;
     MateriaLexIudicium  iudicium;

    piscina = piscina_generare_dynamicum("probatio_diagnostica",
        262144);
    si (piscina == NIHIL)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);
    imprimere("=== Probatio materia_diagnostica ===\n");
    CREDO_VERUM (materia_lexicon_ratum_facere(&ratum, &LEXICON,
        &iudicium));

    {
        MateriaNodus* radix;
        MateriaNodus* par;
                 Xar* exitus;

        imprimere("\n--- I. Arbor sana: nihil ---\n");
        /* "( x )" */
        radix  = _radix_creare(piscina);
        par    = materia_nodus_creare(piscina, (s32)GR_PAR, (i32)III);
        CREDO_NON_NIHIL (radix);
        CREDO_NON_NIHIL (par);
        CREDO_VERUM (materia_nodus_ponere(par, (i32)LP_APERTURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "(",
                ZEPHYRUM, (i32)I)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_appendere(piscina, par,
            (i32)LP_LIBERI, materia_valor_nodus(_nodus_lexematis(
                piscina, (s32)GR_VERBUM, "x", (s32)II, (i32)III)),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_ponere(par, (i32)LP_CLAUSURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, ")",
                (s32)IV, (i32)V)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (_radici_addere(piscina, radix, par));
        exitus = materia_diagnostica_derivare(piscina, radix, &REG,
            &DIAGNOSTICA, NIHIL, NIHIL);
        CREDO_NON_NIHIL (exitus);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), ZEPHYRUM);
    }

    {
        MateriaNodus* radix;
        MateriaNodus* par;
                 Xar* exitus;
                 Xar* emissa;

        imprimere("\n--- II. Clausura absens: punctum in fine ---\n");
        /* "( x" */
        radix  = _radix_creare(piscina);
        par    = materia_nodus_creare(piscina, (s32)GR_PAR, (i32)III);
        CREDO_VERUM (materia_nodus_ponere(par, (i32)LP_APERTURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "(",
                ZEPHYRUM, (i32)I)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_appendere(piscina, par,
            (i32)LP_LIBERI, materia_valor_nodus(_nodus_lexematis(
                piscina, (s32)GR_VERBUM, "x", (s32)II, (i32)III)),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (_radici_addere(piscina, radix, par));
        exitus = materia_diagnostica_derivare(piscina, radix, &REG,
            &DIAGNOSTICA, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), (i32)I);
        CREDO_VERUM (_ordo_congruit(exitus, ZEPHYRUM,
            "par/tok_clausura", (s32)MATERIA_GRAVITAS_ERRATUM,
            (s32)III, (s32)III, (i32)I, (i32)IV));
        /* apertura derivata: tractus nodi "( x" = 0-3 */
        CREDO_VERUM (_apertura_congruit(exitus, ZEPHYRUM, ZEPHYRUM,
            (s32)III));

        /* IX. emissa parsatoris: tractus ex nodo computatus, ordo
         * per initium */
        imprimere("\n--- IX. Emissa parsatoris ---\n");
        emissa = xar_creare(piscina, (i32)magnitudo(
            MateriaDiagnosticum));
        CREDO_NON_NIHIL (emissa);
        {
            MateriaDiagnosticum* e = (MateriaDiagnosticum*)xar_addere(
                emissa);

            CREDO_NON_NIHIL (e);
            memset(e, ZEPHYRUM, magnitudo(*e));
            e->gravitas         = (s32)MATERIA_GRAVITAS_ERRATUM;
            e->codex            = "par/probatio";
            e->causa            = "probatio vacua";
            e->nodus            = par;
            e->tractus.initium  = (s32)-I;
        }
        exitus = materia_diagnostica_derivare(piscina, radix, &REG,
            &DIAGNOSTICA, NIHIL, emissa);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), (i32)II);
        CREDO_VERUM (_ordo_congruit(exitus, ZEPHYRUM, "par/probatio",
            (s32)MATERIA_GRAVITAS_ERRATUM, ZEPHYRUM, (s32)III,
            (i32)I, (i32)I));
        CREDO_VERUM (_ordo_congruit(exitus, (i32)I,
            "par/tok_clausura", (s32)MATERIA_GRAVITAS_ERRATUM,
            (s32)III, (s32)III, (i32)I, (i32)IV));
    }

    {
        MateriaNodus* radix;
        MateriaNodus* par;
                 Xar* exitus;

        imprimere("\n--- III. Lista non scripta = vacua ---\n");
        /* "( )" */
        radix  = _radix_creare(piscina);
        par    = materia_nodus_creare(piscina, (s32)GR_PAR, (i32)III);
        CREDO_VERUM (materia_nodus_ponere(par, (i32)LP_APERTURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "(",
                ZEPHYRUM, (i32)I)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_ponere(par, (i32)LP_CLAUSURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, ")",
                (s32)II, (i32)III)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (_radici_addere(piscina, radix, par));
        exitus = materia_diagnostica_derivare(piscina, radix, &REG,
            &DIAGNOSTICA, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), (i32)I);
        CREDO_VERUM (_ordo_congruit(exitus, ZEPHYRUM, "par/liberi",
            (s32)MATERIA_GRAVITAS_ERRATUM, ZEPHYRUM, (s32)III,
            (i32)I, (i32)I));
        CREDO_VERUM (_sedes_unica(exitus, ZEPHYRUM));
    }

    {
        MateriaNodus* radix;
        MateriaNodus* par;
                 Xar* exitus;

        imprimere("\n--- IV. Lista separatore sola = vacua ---\n");
        /* "( ; )" */
        radix  = _radix_creare(piscina);
        par    = materia_nodus_creare(piscina, (s32)GR_PAR, (i32)III);
        CREDO_VERUM (materia_nodus_ponere(par, (i32)LP_APERTURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "(",
                ZEPHYRUM, (i32)I)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_appendere(piscina, par,
            (i32)LP_LIBERI, materia_valor_nodus(_nodus_lexematis(
                piscina, (s32)GR_SEPARATOR, ";", (s32)II, (i32)III)),
            MATERIA_LOCUS_LISTA_NODUS));
        CREDO_VERUM (materia_nodus_ponere(par, (i32)LP_CLAUSURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, ")",
                (s32)IV, (i32)V)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (_radici_addere(piscina, radix, par));
        exitus = materia_diagnostica_derivare(piscina, radix, &REG,
            &DIAGNOSTICA, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), (i32)I);
        CREDO_VERUM (_ordo_congruit(exitus, ZEPHYRUM, "par/liberi",
            (s32)MATERIA_GRAVITAS_ERRATUM, ZEPHYRUM, (s32)V,
            (i32)I, (i32)I));
    }

    {
        MateriaNodus* radix;
        MateriaNodus* malum;
                 Xar* exitus;

        imprimere("\n--- V. Genus diagnosticum (malum) ---\n");
        /* ")" */
        radix = _radix_creare(piscina);
        malum = materia_nodus_creare(piscina, (s32)GR_MALUM, (i32)I);
        CREDO_VERUM (materia_nodus_appendere(piscina, malum, ZEPHYRUM,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, ")",
                ZEPHYRUM, (i32)I)), MATERIA_LOCUS_LISTA_TOKEN));
        CREDO_VERUM (_radici_addere(piscina, radix, malum));
        exitus = materia_diagnostica_derivare(piscina, radix, &REG,
            &DIAGNOSTICA, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), (i32)I);
        CREDO_VERUM (_ordo_congruit(exitus, ZEPHYRUM, "malum",
            (s32)MATERIA_GRAVITAS_MONITUM, ZEPHYRUM, (s32)I, (i32)I,
            (i32)I));
        CREDO_VERUM (_sedes_unica(exitus, ZEPHYRUM));
    }

    {
        MateriaNodus* radix;
        MateriaNodus* par;
                 Xar* exitus;

        imprimere("\n--- VI. Nodus sine lexemate: punctum prius ---\n");
        /* "x (" - par sine lexematibus ullis */
        radix  = _radix_creare(piscina);
        CREDO_VERUM (_radici_addere(piscina, radix, _nodus_lexematis(
            piscina, (s32)GR_VERBUM, "x", ZEPHYRUM, (i32)I)));
        par = materia_nodus_creare(piscina, (s32)GR_PAR, (i32)III);
        CREDO_VERUM (_radici_addere(piscina, radix, par));
        exitus = materia_diagnostica_derivare(piscina, radix, &REG,
            &DIAGNOSTICA, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), (i32)II);
        /* ordo per codicem: 'par/liberi' ante 'par/tok_clausura' */
        CREDO_VERUM (_ordo_congruit(exitus, ZEPHYRUM, "par/liberi",
            (s32)MATERIA_GRAVITAS_ERRATUM, (s32)I, (s32)I, (i32)I,
            (i32)II));
        CREDO_VERUM (_ordo_congruit(exitus, (i32)I,
            "par/tok_clausura", (s32)MATERIA_GRAVITAS_ERRATUM,
            (s32)I, (s32)I, (i32)I, (i32)II));
    }

    {
         MateriaNodus* radix;
         MateriaToken* lexema_a;
                  Xar* exitus;

        imprimere("\n--- VII. Ordo octetorum ruptus ---\n");
        /* "b a" in arbore ordine inverso positum */
        radix     = _radix_creare(piscina);
        lexema_a  = _lex(piscina, (s32)G_IDENT, "a", ZEPHYRUM,
            (i32)I);
        CREDO_VERUM (_radici_addere(piscina, radix, _nodus_lexematis(
            piscina, (s32)GR_VERBUM, "b", (s32)II, (i32)III)));
        {
            MateriaNodus* posterius = materia_nodus_creare(piscina,
                (s32)GR_VERBUM, (i32)I);

            CREDO_VERUM (materia_nodus_ponere(posterius, ZEPHYRUM,
                materia_valor_token(lexema_a), MATERIA_LOCUS_TOKEN));
            CREDO_VERUM (_radici_addere(piscina, radix, posterius));
        }
        exitus = materia_diagnostica_derivare(piscina, radix, &REG,
            &DIAGNOSTICA, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), (i32)I);
        CREDO_VERUM (_ordo_congruit(exitus, ZEPHYRUM,
            MATERIA_CODEX_ORDO, (s32)MATERIA_GRAVITAS_ERRATUM,
            ZEPHYRUM, (s32)I, (i32)I, (i32)I));
        {
            constans MateriaDiagnosticum* r =
                (constans MateriaDiagnosticum*)xar_obtinere(exitus,
                    ZEPHYRUM);

            CREDO_VERUM (r->lexema == lexema_a);
            CREDO_NIHIL (r->nodus);
        }
    }

    {
        MateriaNodus* radix;
        MateriaToken* lexema;
                 Xar* exitus;

        imprimere("\n--- VIII. Usus iteratus semel visus ---\n");
        radix   = _radix_creare(piscina);
        lexema  = _lex(piscina, (s32)G_IDENT, "x", ZEPHYRUM, (i32)I);
        {
            MateriaNodus* prius = materia_nodus_creare(piscina,
                (s32)GR_VERBUM, (i32)I);
            MateriaNodus* iterum = materia_nodus_creare(piscina,
                (s32)GR_VERBUM, (i32)I);

            CREDO_VERUM (materia_nodus_ponere(prius, ZEPHYRUM,
                materia_valor_token(lexema), MATERIA_LOCUS_TOKEN));
            CREDO_VERUM (materia_nodus_ponere(iterum, ZEPHYRUM,
                materia_valor_token(lexema), MATERIA_LOCUS_TOKEN));
            CREDO_VERUM (_radici_addere(piscina, radix, prius));
            CREDO_VERUM (_radici_addere(piscina, radix, iterum));
        }
        exitus = materia_diagnostica_derivare(piscina, radix, &REG,
            &DIAGNOSTICA, NIHIL, NIHIL);
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), ZEPHYRUM);
    }

    {
        MateriaArborConsilium consilium;
        MateriaArborScriptura scriptura;
                 MateriaNodus* radix;
                 MateriaNodus* verbum;
                 MateriaToken* lexema;
                       chorda  valor;
                    character  memoria[XVI];

        imprimere("\n--- X. Tractus refutationis scriptoris ---\n");
        /* VEHICULUM REFUTATIONIS, non subiectum: hic tractum
         * refutationis probamus, non causam eius. Olim NUL vehiculum
         * erat; ex quo attributum 'nul' eum fert (2026-09-19) NUL non
         * amplius refutat, ergo vehiculum mutatum est ad limitem qui
         * MANET - valor sequentiam claudentem elementi sui ferens,
         * quem forma cruda ferre non potest (01M2KPJ0HW).
         *
         * littera constans esset (-Wcast-qual): series propria */
        memcpy(memoria, "a</lex-ident>b", (size_t)XIV);
        valor.datum    = (i8*)memoria;
        valor.mensura  = (i32)XIV;
        lexema = materia_token_creare(piscina, &FORMA, (s32)G_IDENT,
            valor, (s32)VI, (i32)II, (i32)III, ZEPHYRUM);
        CREDO_NON_NIHIL (lexema);
        radix   = _radix_creare(piscina);
        verbum  = materia_nodus_creare(piscina, (s32)GR_VERBUM,
            (i32)I);
        CREDO_VERUM (materia_nodus_ponere(verbum, ZEPHYRUM,
            materia_valor_token(lexema), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (_radici_addere(piscina, radix, verbum));
        materia_arbor_consilium_nudum(&consilium, &REG, &ratum,
            "probatio");
        scriptura = materia_arbor_scribere_nodum(piscina, radix,
            &consilium);
        CREDO_FALSUM (scriptura.successus);
        CREDO_AEQUALIS_S32 (scriptura.tractus.initium, (s32)VI);
        CREDO_AEQUALIS_I32 (scriptura.tractus.linea, (i32)II);
        CREDO_AEQUALIS_I32 (scriptura.tractus.columna, (i32)III);
    }

    {
         MateriaNodus* radix;
         MateriaNodus* imus;
                  Xar* exitus;
                  i32  k;

        imprimere("\n--- XI. Profunditas: ambulatio iterativa ---\n");
        /* catena par{liberi:[par{...}]} L milia profunda: ambulatio
         * recursiva acervum frangeret, et tractus per nodum computatus
         * quadraticus esset (quaestio 01M2R9MKFQ). Imus sine clausura:
         * ordo unus exspectatur. */
        radix  = _radix_creare(piscina);
        imus   = materia_nodus_creare(piscina, (s32)GR_PAR, (i32)III);
        CREDO_VERUM (materia_nodus_ponere(imus, (i32)LP_APERTURA,
            materia_valor_token(_lex(piscina, (s32)G_IDENT, "(",
                ZEPHYRUM, (i32)I)), MATERIA_LOCUS_TOKEN));
        CREDO_VERUM (materia_nodus_appendere(piscina, imus,
            (i32)LP_LIBERI, materia_valor_nodus(_nodus_lexematis(
                piscina, (s32)GR_VERBUM, "x", (s32)I, (i32)II)),
            MATERIA_LOCUS_LISTA_NODUS));
        per (k = ZEPHYRUM; k < (i32)50000; k++)
        {
            MateriaNodus* supra = materia_nodus_creare(piscina,
                (s32)GR_PAR, (i32)III);

            si (supra == NIHIL)
            {
                frange;
            }
            si (!materia_nodus_appendere(piscina, supra,
                    (i32)LP_LIBERI, materia_valor_nodus(imus),
                    MATERIA_LOCUS_LISTA_NODUS))
            {
                frange;
            }
            imus = supra;
        }
        CREDO_VERUM (_radici_addere(piscina, radix, imus));
        exitus = materia_diagnostica_derivare(piscina, radix, &REG,
            &DIAGNOSTICA, NIHIL, NIHIL);
        CREDO_NON_NIHIL (exitus);
        /* imus: clausura absens et lista plena; supra L milia: lista
         * plena, clausura absens quoque - ergo L milia + I ordines */
        CREDO_AEQUALIS_I32 (xar_numerus(exitus), (i32)50001);
    }

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
