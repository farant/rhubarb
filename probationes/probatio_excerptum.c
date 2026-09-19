/* probatio_excerptum.c - Excerptum fontis cum signo (planum
 * materia-sedes B5)
 *
 * Contractus: margo numeri lineae (cifrae, minimum IV) + " | ",
 * linea fontis (CR sublato, moderatores praeter tabulam ut spatium),
 * deinde margo vacuus + '^' ad initium et '~' per CHARACTEREM UTF-8
 * usque ad finem tractus aut lineae. Tabula in margine sub linea
 * SERVATUR, ergo signum columnam veram tenet etiam in fonte
 * tabulato.
 *
 * Chordae expectatae hic CONTRACTUS sunt: codicem ad eas emenda,
 * numquam eas ad codicem.
 */

#include "latina.h"
#include "credo.h"
#include "chorda_aedificator.h"
#include "excerptum.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

interior vacuum
_probare (
                Piscina* piscina,
     constans character* fons,
                    s32  initium,
                    s32  finis,
                    i32  linea,
     constans character* expectatum)
{
     ChordaAedificator* aedificator;
                chorda  fructus;

    aedificator = chorda_aedificator_creare(piscina,
        (memoriae_index)CXXVIII);
    si (aedificator == NIHIL)
    {
        CREDO_CULPA ("aedificator");
        redde;
    }
    CREDO_VERUM (excerptum_scribere(aedificator, fons,
        (i32)strlen(fons), initium, finis, linea));
    fructus = chorda_aedificator_finire(aedificator);
    CREDO_CHORDA_AEQUALIS_LITERIS (fructus, expectatum);
}

/* Fenestra lineae longissimae (html minutum, css unius lineae).
 *
 * Probatio STRUCTURALIS consulto, non chorda expectata: indicem '^'
 * in linea signi quaerimus et characterem EIUSDEM indicis in linea
 * contenti inspicimus. Ita calculus ALTER quam ille qui in codice
 * vivit - chorda expectata manu computata eandem arithmeticam
 * repeteret quam probare volumus, et error trium columnarum ('...'
 * columnas praeripit) in ambobus pariter lateret.
 *
 * Fons: 'a' per totam lineam praeter 'X' unum ad 'sedes_tractus'.
 * Ergo character sub signo 'X' esse DEBET, ubicumque fenestra
 * inciderit. */
interior vacuum
_probare_fenestram (
    Piscina* piscina,
        s32  longitudo,
        s32  sedes_tractus,
        b32  fenestra_speranda)
{
     ChordaAedificator* aedificator;
                chorda  fructus;
             character* fons;
                   s32  index;
                   s32  divisio;
                   s32  caret;
                   b32  elisa;

    fons = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + II);
    si (fons == NIHIL)
    {
        CREDO_CULPA ("fons");
        redde;
    }
    per (index = ZEPHYRUM; index < longitudo; index++)
    {
        fons[index] = 'a';
    }
    fons[sedes_tractus]  = 'X';
    fons[longitudo]      = '\n';
    fons[longitudo + I]  = '\0';

    aedificator = chorda_aedificator_creare(piscina,
        (memoriae_index)MM);
    si (aedificator == NIHIL)
    {
        CREDO_CULPA ("aedificator");
        redde;
    }
    CREDO_VERUM (excerptum_scribere(aedificator, fons,
        (i32)longitudo + I, sedes_tractus, sedes_tractus + I, I));
    fructus = chorda_aedificator_finire(aedificator);

    /* divisio = nova linea inter lineam contenti et lineam signi */
    divisio = (s32)-I;
    per (index = ZEPHYRUM; index < (s32)fructus.mensura; index++)
    {
        si (fructus.datum[index] == '\n')
        {
            divisio = index;
            frange;
        }
    }
    CREDO_VERUM (divisio > ZEPHYRUM);

    /* puncta adsunt? */
    elisa = FALSUM;
    per (index = ZEPHYRUM; index + II < divisio; index++)
    {
        si (   fructus.datum[index]      == '.'
            && fructus.datum[index + I]  == '.'
            && fructus.datum[index + II] == '.')
        {
            elisa = VERUM;
            frange;
        }
    }
    CREDO_AEQUALIS_S32 ((s32)elisa, (s32)fenestra_speranda);

    /* linea contenti sub limite manere debet cum fenestra adhibita
     * est (margo et '...' pauca addunt) */
    si (fenestra_speranda)
    {
        CREDO_VERUM (divisio < (s32)CC);
    }

    /* signum: '^' post divisionem; character eiusdem indicis in
     * linea contenti 'X' esse debet */
    caret = (s32)-I;
    per (index = divisio + I; index < (s32)fructus.mensura; index++)
    {
        si (fructus.datum[index] == '^')
        {
            caret = index - (divisio + I);
            frange;
        }
    }
    CREDO_VERUM (caret > ZEPHYRUM);
    CREDO_VERUM (caret < divisio);
    CREDO_AEQUALIS_S32 ((s32)fructus.datum[caret], (s32)'X');
}

interior vacuum
_probare_multa (
                 Piscina* piscina,
      constans character* fons,
    constans ExcerptumSedes* sedes,
                      i32  numerus,
      constans character* expectatum)
{
     ChordaAedificator* aedificator;
                chorda  fructus;

    aedificator = chorda_aedificator_creare(piscina,
        (memoriae_index)CXXVIII);
    si (aedificator == NIHIL)
    {
        CREDO_CULPA ("aedificator");
        redde;
    }
    CREDO_VERUM (excerptum_scribere_multa(aedificator, fons,
        (i32)strlen(fons), sedes, numerus));
    fructus = chorda_aedificator_finire(aedificator);
    CREDO_CHORDA_AEQUALIS_LITERIS (fructus, expectatum);
}

s32
principale (vacuum)
{
                b32  praeteritus;
            Piscina* piscina;
  ChordaAedificator* aedificator;

    piscina = piscina_generare_dynamicum("probatio_excerptum",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);

    imprimere("\n--- Probans excerptum ---\n");

    /* signum sub charactere uno */
    _probare(piscina, "int x = 1;\n", (s32)IV, (s32)V, I,
        "   1 | int x = 1;\n     |     ^\n");
    /* tabula in margine SERVATUR: columna vera manet */
    _probare(piscina, "\tab\n", (s32)I, (s32)III, I,
        "   1 | \tab\n     | \t^~\n");
    /* UTF-8: e acutum duo octeti, character unus */
    _probare(piscina, "\xC3\xA9 = x\n", (s32)III, (s32)IV, I,
        "   1 | \xC3\xA9 = x\n     |   ^\n");
    /* TRES octeti NECESSARII sunt: casus supra plantam in larva
     * continuationis (0x80 -> 0xC0) NON videt, quia in sequentia
     * BINA numerus octetorum 'characterem incipientium' idem manet
     * (I recte, I planta); in sequentia TERNA differt (I contra II).
     * Planta viridis mensura PROBATIONIS est, non codicis. */
    _probare(piscina, "\xE2\x82\xAC = x\n", (s32)IV, (s32)V, I,
        "   1 | \xE2\x82\xAC = x\n     |   ^\n");
    /* tilde per CHARACTEREM, non per octetum: 'a', euro, 'b'.
     * Litterae SEIUNCTAE: '\xAC' 'b' effugium hexadecimale AVIDUM
     * est ('\xACb' extra limites) - iunctura litterarum eum claudit */
    _probare(piscina, "a\xE2\x82\xAC" "b\n", ZEPHYRUM, (s32)V, I,
        "   1 | a\xE2\x82\xAC" "b\n     | ^~~\n");
    /* linea sine nova-linea ad finem */
    _probare(piscina, "abc", (s32)III, (s32)III, I,
        "   1 | abc\n     |    ^\n");
    /* tractus ultra finem lineae: ad lineam solam truncatur */
    _probare(piscina, "abc\nd", (s32)I, (s32)V, I,
        "   1 | abc\n     |  ^~\n");
    /* CR ante nova-linea sublatum */
    _probare(piscina, "ab\r\n", ZEPHYRUM, (s32)II, I,
        "   1 | ab\n     | ^~\n");
    /* margo crescit cum numero lineae */
    _probare(piscina, "q\n", ZEPHYRUM, (s32)I, (i32)12345,
        "12345 | q\n      | ^\n");

    imprimere("\n--- Fenestra lineae longae ---\n");
    /* ad limitem IPSUM nihil mutatur: clientes priores (C89, bash,
     * css ordinarium) octetim iidem manere debent */
    _probare_fenestram(piscina, (s32)CLX, (s32)LXXX, FALSUM);
    /* supra limitem: puncta utrimque, signum sub 'X' manet */
    _probare_fenestram(piscina, (s32)CD, (s32)CC, VERUM);
    /* tractus PROPE INITIUM: puncta DEXTRA sola, ergo '...' nullum
     * ante signum - casus qui errorem trium columnarum nudaret si
     * spatia punctorum semper adderentur */
    _probare_fenestram(piscina, (s32)CD, (s32)V, VERUM);
    /* tractus PROPE FINEM: puncta SINISTRA sola */
    _probare_fenestram(piscina, (s32)CD, (s32)CD - (s32)V, VERUM);

    imprimere("\n--- Sedes multiplices ---\n");

    /* IDENTITAS: sedes una sine nota = excerptum_scribere OCTETIM.
     * Comparantur exitus ipsi, non chorda manu scripta - ita scissio
     * in _lineam_scribere/_signum_scribere mensuratur, non asseritur */
    {
        hic_manens constans ExcerptumSedes UNA[] = {
            { (s32)IV, (s32)V, I, NIHIL }
        };
        ChordaAedificator* unius;
        ChordaAedificator* multarum;
                   chorda  textus_unius;
                   chorda  textus_multarum;

        unius     = chorda_aedificator_creare(piscina,
            (memoriae_index)CXXVIII);
        multarum  = chorda_aedificator_creare(piscina,
            (memoriae_index)CXXVIII);
        CREDO_NON_NIHIL (unius);
        CREDO_NON_NIHIL (multarum);
        CREDO_VERUM (excerptum_scribere(unius, "int x = 1;\n",
            (i32)XI, (s32)IV, (s32)V, I));
        CREDO_VERUM (excerptum_scribere_multa(multarum,
            "int x = 1;\n", (i32)XI, UNA, (i32)I));
        textus_unius     = chorda_aedificator_finire(unius);
        textus_multarum  = chorda_aedificator_finire(multarum);
        /* CREDO_CHORDA_AEQUALIS (singulare) chordas confert;
         * CREDO_CHORDAE_AEQUALES (plurale) 'constans character*'
         * poscit - nomina fallunt, examen id invenit */
        CREDO_CHORDA_AEQUALIS (textus_unius, textus_multarum);
    }

    /* SEDES DUAE IN LINEA UNA: linea fontis SEMEL, signa BINA */
    {
        hic_manens constans ExcerptumSedes BINAE[] = {
            { (s32)V,   (s32)VI, I, "hic" },
            { (s32)VII, (s32)X,  I, "et hic" }
        };

        _probare_multa(piscina, "test a -nt b\n", BINAE, (i32)II,
            "   1 | test a -nt b\n"
            "     |      ^ hic\n"
            "     |        ^~~ et hic\n");
    }

    /* MARGO SEMEL MENSURATUS: minimum latitudinis IV est, ergo prima
     * divergentia NON ad 999/1000 sed ad 9999/10000 iacet (planum et
     * specificatio 999 dicebant - mensura eos corrigit). Sine margine
     * communi linea 9999 latitudinem IV, linea 10000 latitudinem V
     * acciperet et saeptum hiaret */
    {
        hic_manens constans ExcerptumSedes LATAE[] = {
            { ZEPHYRUM, (s32)II, (i32)9999,  NIHIL },
            { (s32)III, (s32)V,  (i32)10000, NIHIL }
        };

        _probare_multa(piscina, "ab\ncd\n", LATAE, (i32)II,
            " 9999 | ab\n"
            "      | ^~\n"
            "10000 | cd\n"
            "      | ^~\n");
    }

    /* EXEMPLUM SPECIFICATIONIS: 'if' non clausum, sedes ambae */
    {
        hic_manens constans ExcerptumSedes APERTUM[] = {
            { ZEPHYRUM,   (s32)XXIII, I,         "hic coepit" },
            { (s32)XXIII, (s32)XXIII, (i32)III,  "hic exspectatur" }
        };

        _probare_multa(piscina, "if true; then\n  echo x\n", APERTUM,
            (i32)II,
            "   1 | if true; then\n"
            "     | ^~~~~~~~~~~~~ hic coepit\n"
            "   3 | \n"
            "     | ^ hic exspectatur\n");
    }

    /* ORDO: series non ordinata RECUSATUR, non tacite ordinatur */
    {
        hic_manens constans ExcerptumSedes PERVERSAE[] = {
            { (s32)V,  (s32)VI, I, NIHIL },
            { (s32)I,  (s32)II, I, NIHIL }
        };
        ChordaAedificator* conatus;

        conatus = chorda_aedificator_creare(piscina,
            (memoriae_index)XXXII);
        CREDO_NON_NIHIL (conatus);
        CREDO_FALSUM (excerptum_scribere_multa(conatus,
            "test a -nt b\n",
            (i32)XIII, PERVERSAE, (i32)II));
        /* numerus ZEPHYRUM: nihil pingendum */
        CREDO_FALSUM (excerptum_scribere_multa(conatus, "ab", (i32)II,
            PERVERSAE, ZEPHYRUM));
        /* initium extra fontem, etiam in sede secunda */
        {
            hic_manens constans ExcerptumSedes EXTRA[] = {
                { ZEPHYRUM, (s32)I,   I, NIHIL },
                { (s32)IX,  (s32)IX,  I, NIHIL }
            };

            CREDO_FALSUM (excerptum_scribere_multa(conatus, "ab",
                (i32)II,
                EXTRA, (i32)II));
        }
    }

    /* initium extra fontem RECUSATUR */
    aedificator = chorda_aedificator_creare(piscina,
        (memoriae_index)XXXII);
    CREDO_NON_NIHIL (aedificator);
    CREDO_FALSUM (excerptum_scribere(aedificator, "ab", (i32)II,
        (s32)III, (s32)III, I));
    /* linea ZEPHYRUM RECUSATUR */
    CREDO_FALSUM (excerptum_scribere(aedificator, "ab", (i32)II,
        ZEPHYRUM, (s32)I, ZEPHYRUM));

    imprimere("\n");
    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);
    redde praeteritus ? ZEPHYRUM : I;
}
