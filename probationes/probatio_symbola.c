/* probatio_symbola.c - Probationes bibliothecae Symbola */

#include "latina.h"
#include "macho.h"
#include "symbola.h"
#include "piscina.h"
#include "credo.h"
#include <stdio.h>


    /* ==================================================
	 * Probare tabula_symbolorum_ex_macho creatio
	 * ================================================== */

interior vacuum
_sectio_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
            constans character* via = "probationes/fixa/simplex";
                         MachO* macho;
              TabulaSymbolorum* tabula;
                           i32  numerus;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        tabula = tabula_symbolorum_ex_macho(macho, piscina);
        CREDO_NON_NIHIL(tabula);

        /* Verificare habet symbola */
        numerus = tabula_symbolorum_numerus(tabula);
        imprimere("  Total symbols: %d\n", numerus);
        CREDO_MAIOR_I32(numerus, ZEPHYRUM);
    }
}


    /* ==================================================
	 * Probare tabula_symbolorum_symbolum (index access)
	 * ================================================== */

interior vacuum
_sectio_secunda (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
            constans character* via = "probationes/fixa/simplex";
                         MachO* macho;
              TabulaSymbolorum* tabula;
                      Symbolum* sym;
                           i32  numerus;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        tabula = tabula_symbolorum_ex_macho(macho, piscina);
        CREDO_NON_NIHIL(tabula);

        numerus = tabula_symbolorum_numerus(tabula);

        /* Obtinere primum symbolum */
        si (numerus > ZEPHYRUM)
        {
            sym = tabula_symbolorum_symbolum(tabula, ZEPHYRUM);
            CREDO_NON_NIHIL(sym);
        }

        /* Verificare index invalidus reddit NIHIL */
        sym = tabula_symbolorum_symbolum(tabula, numerus + X);
        CREDO_NIHIL(sym);

        sym = tabula_symbolorum_symbolum(tabula, (i32)(ZEPHYRUM - I));
        CREDO_NIHIL(sym);
    }
}


    /* ==================================================
	 * Probare symbolorum_iterator
	 * ================================================== */

interior vacuum
_sectio_tertia (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
            constans character* via = "probationes/fixa/simplex";
                         MachO* macho;
              TabulaSymbolorum* tabula;
            SymbolorumIterator  iter;
                      Symbolum* sym;
                           i32  numerus_iteratus;
                           i32  numerus_totalis;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        tabula = tabula_symbolorum_ex_macho(macho, piscina);
        CREDO_NON_NIHIL(tabula);

        numerus_totalis   = tabula_symbolorum_numerus(tabula);
        numerus_iteratus  = ZEPHYRUM;

        iter = symbolorum_iterator_initium(tabula);

        dum ((sym = symbolorum_iterator_proximum(&iter)) != NIHIL)
        {
            chorda appellatio = symbolum_nomen(sym, piscina);

            si (appellatio.mensura > ZEPHYRUM && appellatio.datum)
            {
                imprimere("  [%d] %.*s (addr=0x%lx, type=0x%x, sect=%d)\n",
                          numerus_iteratus,
                          appellatio.mensura, appellatio.datum,
                          (unsigned long)symbolum_addressa(sym),
                          symbolum_genus(sym),
                          symbolum_numerus_sectionis(sym));
            }

            numerus_iteratus++;
        }

        imprimere("  Total iterated: %d\n", numerus_iteratus);

        /* Verificare numerus iteratus == numerus totalis */
        CREDO_AEQUALIS_I32(numerus_iteratus, numerus_totalis);

        /* Verificare iterator finis */
        CREDO_VERUM(symbolorum_iterator_finis(&iter));
    }
}


    /* ==================================================
	 * Probare symbolum_invenire
	 * ================================================== */

interior vacuum
_sectio_quarta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
            constans character* via = "probationes/fixa/simplex";
                         MachO* macho;
              TabulaSymbolorum* tabula;
                      Symbolum* main_sym;
                      Symbolum* non_existens;
                        chorda  appellatio;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        tabula = tabula_symbolorum_ex_macho(macho, piscina);
        CREDO_NON_NIHIL(tabula);

        /* Invenire _main (debet existere) */
        main_sym = symbolum_invenire(tabula, "_main", piscina);
        CREDO_NON_NIHIL(main_sym);

        appellatio = symbolum_nomen(main_sym, piscina);
        imprimere("  Found: %.*s at 0x%lx\n",
                  appellatio.mensura, appellatio.datum,
                  (unsigned long)symbolum_addressa(main_sym));

        /* Verificare nomen */
        CREDO_CHORDA_CONTINET(appellatio, chorda_ex_literis("main",
            piscina));

        /* Quaerere symbolum non existens */
        non_existens = symbolum_invenire(tabula, "_non_existens_xyz",
            piscina);
        CREDO_NIHIL(non_existens);
    }
}


    /* ==================================================
	 * Probare symbolum attributa
	 * ================================================== */

interior vacuum
_sectio_quinta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
            constans character* via = "probationes/fixa/simplex";
                         MachO* macho;
              TabulaSymbolorum* tabula;
                      Symbolum* main_sym;
                        chorda  appellatio;
                memoriae_index  addressa;
                           i32  genus;
                           i32  sect;
                           i32  desc;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        tabula = tabula_symbolorum_ex_macho(macho, piscina);
        CREDO_NON_NIHIL(tabula);

        main_sym = symbolum_invenire(tabula, "_main", piscina);
        CREDO_NON_NIHIL(main_sym);

        /* Verificare nomen */
        appellatio = symbolum_nomen(main_sym, piscina);
        CREDO_VERUM(appellatio.mensura > ZEPHYRUM);
        imprimere("  Name: %.*s\n", appellatio.mensura,
            appellatio.datum);

        /* Verificare addressa */
        addressa = symbolum_addressa(main_sym);
        imprimere("  Address: 0x%lx\n", (unsigned long)addressa);
        CREDO_VERUM(addressa > ZEPHYRUM);

        /* Verificare genus */
        genus = symbolum_genus(main_sym);
        imprimere("  Type: 0x%x\n", genus);

        /* Verificare sectio */
        sect = symbolum_numerus_sectionis(main_sym);
        imprimere("  Section: %d\n", sect);
        CREDO_VERUM(sect > ZEPHYRUM);  /* _main debet esse in sectione */

        /* Verificare descriptor */
        desc = symbolum_descriptor(main_sym);
        imprimere("  Descriptor: 0x%x\n", desc);
    }
}


    /* ==================================================
	 * Probare symbolum_est_* predicates
	 * ================================================== */

interior vacuum
_sectio_sexta (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
            constans character* via = "probationes/fixa/simplex";
                         MachO* macho;
              TabulaSymbolorum* tabula;
                      Symbolum* main_sym;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        tabula = tabula_symbolorum_ex_macho(macho, piscina);
        CREDO_NON_NIHIL(tabula);

        main_sym = symbolum_invenire(tabula, "_main", piscina);
        CREDO_NON_NIHIL(main_sym);

        /* Verificare _main est functio */
        imprimere("  _main is function: %s\n",
                  symbolum_est_functio(main_sym) ? "sic" : "non");
        CREDO_VERUM(symbolum_est_functio(main_sym));

        /* Verificare _main est externum */
        imprimere("  _main is external: %s\n",
                  symbolum_est_externum(main_sym) ? "sic" : "non");
        CREDO_VERUM(symbolum_est_externum(main_sym));

        /* Verificare _main non est indefinitum */
        imprimere("  _main is undefined: %s\n",
                  symbolum_est_indefinitum(main_sym) ? "sic" : "non");
        CREDO_FALSUM(symbolum_est_indefinitum(main_sym));

        /* Verificare _main non est debug */
        imprimere("  _main is debug: %s\n",
                  symbolum_est_debug(main_sym) ? "sic" : "non");
        CREDO_FALSUM(symbolum_est_debug(main_sym));
    }
}


    /* ==================================================
	 * Probare cum_functione (plures symbola)
	 * ================================================== */

interior vacuum
_sectio_septima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
            constans character* via = "probationes/fixa/cum_functione";
                         MachO* macho;
              TabulaSymbolorum* tabula;
                      Symbolum* main_sym;
                      Symbolum* calculare_sym;
                      Symbolum* printf_sym;
                           i32  numerus;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        tabula = tabula_symbolorum_ex_macho(macho, piscina);
        CREDO_NON_NIHIL(tabula);

        numerus = tabula_symbolorum_numerus(tabula);
        imprimere("  Total symbols: %d\n", numerus);
        CREDO_MAIOR_I32(numerus, ZEPHYRUM);

        /* Invenire _main */
        main_sym = symbolum_invenire(tabula, "_main", piscina);
        CREDO_NON_NIHIL(main_sym);
        CREDO_VERUM(symbolum_est_functio(main_sym));
        CREDO_VERUM(symbolum_est_externum(main_sym));

        /* Invenire _calculare_summa */
        calculare_sym = symbolum_invenire(tabula, "_calculare_summa",
            piscina);
        CREDO_NON_NIHIL(calculare_sym);
        CREDO_VERUM(symbolum_est_functio(calculare_sym));
        CREDO_VERUM(symbolum_est_externum(calculare_sym));

        /* Invenire _printf (externum indefinitum) */
        printf_sym = symbolum_invenire(tabula, "_printf", piscina);
        CREDO_NON_NIHIL(printf_sym);
        CREDO_VERUM(symbolum_est_externum(printf_sym));
        CREDO_VERUM(symbolum_est_indefinitum(printf_sym));
        CREDO_FALSUM(symbolum_est_functio(printf_sym));  /* Indefinitum non est functio */
    }
}


    /* ==================================================
	 * Probare symbola_obtinere_functiones
	 * ================================================== */

interior vacuum
_sectio_octava (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
            constans character*  via = "probationes/fixa/cum_functione";
                         MachO*  macho;
              TabulaSymbolorum*  tabula;
                      Symbolum** functiones;
                           i32   numerus_functionum;
                           i32   i;
                           b32   invenit_main;
                           b32   invenit_calculare;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        tabula = tabula_symbolorum_ex_macho(macho, piscina);
        CREDO_NON_NIHIL(tabula);

        functiones = symbola_obtinere_functiones(tabula,
            &numerus_functionum,
            piscina);
        imprimere("  Functions found: %d\n", numerus_functionum);

        si (functiones != NIHIL)
        {
            CREDO_MAIOR_I32(numerus_functionum, ZEPHYRUM);

            invenit_main       = FALSUM;
            invenit_calculare  = FALSUM;

            per (i = ZEPHYRUM; i < numerus_functionum; i++)
            {
                chorda appellatio = symbolum_nomen(functiones[i],
                    piscina);
                imprimere("    [%d] %.*s\n", i, appellatio.mensura,
                    appellatio.datum);

                si (chorda_continet(appellatio,
                    chorda_ex_literis("main", piscina)))
                {
                    invenit_main = VERUM;
                }
                si (chorda_continet(appellatio,
                    chorda_ex_literis("calculare",
                    piscina)))
                {
                    invenit_calculare = VERUM;
                }

                /* Verificare omnia sunt functiones */
                CREDO_VERUM(symbolum_est_functio(functiones[i]));
            }

            /* Verificare invenit functiones speratas */
            CREDO_VERUM(invenit_main);
            CREDO_VERUM(invenit_calculare);
        }
    }
}


    /* ==================================================
	 * Probare symbola_obtinere_externa_indefinita
	 * ================================================== */

interior vacuum
_sectio_nona (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
            constans character*  via = "probationes/fixa/cum_functione";
                         MachO*  macho;
              TabulaSymbolorum*  tabula;
                      Symbolum** externa;
                           i32   numerus_externa;
                           i32   i;
                           b32   invenit_printf;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        tabula = tabula_symbolorum_ex_macho(macho, piscina);
        CREDO_NON_NIHIL(tabula);

        externa = symbola_obtinere_externa_indefinita(tabula,
            &numerus_externa, piscina);
        imprimere("  External undefined symbols: %d\n",
            numerus_externa);

        si (externa != NIHIL)
        {
            CREDO_MAIOR_I32(numerus_externa, ZEPHYRUM);

            invenit_printf = FALSUM;

            per (i = ZEPHYRUM; i < numerus_externa; i++)
            {
                chorda appellatio = symbolum_nomen(externa[i], piscina);
                imprimere("    [%d] %.*s\n", i, appellatio.mensura,
                    appellatio.datum);

                si (chorda_continet(appellatio,
                    chorda_ex_literis("printf",
                    piscina)))
                {
                    invenit_printf = VERUM;
                }

                /* Verificare omnia sunt externa et indefinita */
                CREDO_VERUM(symbolum_est_externum(externa[i]));
                CREDO_VERUM(symbolum_est_indefinitum(externa[i]));
            }

            /* Verificare invenit printf */
            CREDO_VERUM(invenit_printf);
        }
    }
}


    /* ==================================================
	 * Probare symbolum_invenire_per_addressa
	 * ================================================== */

interior vacuum
_sectio_decima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
            constans character* via = "probationes/fixa/simplex";
                         MachO* macho;
              TabulaSymbolorum* tabula;
                      Symbolum* main_sym;
                      Symbolum* found_sym;
                memoriae_index  addressa;

        macho = macho_aperire(via, piscina);
        CREDO_NON_NIHIL(macho);

        tabula = tabula_symbolorum_ex_macho(macho, piscina);
        CREDO_NON_NIHIL(tabula);

        /* Obtinere addressa _main */
        main_sym = symbolum_invenire(tabula, "_main", piscina);
        CREDO_NON_NIHIL(main_sym);

        addressa = symbolum_addressa(main_sym);
        imprimere("  _main address: 0x%lx\n", (unsigned long)addressa);

        /* Invenire per addressa */
        found_sym = symbolum_invenire_per_addressa(tabula, addressa,
            piscina);
        CREDO_NON_NIHIL(found_sym);

        /* Verificare est idem symbolum */
        CREDO_VERUM(symbolum_addressa(found_sym) == addressa);

        /* Verificare addressa invalida reddit NIHIL */
        found_sym = symbolum_invenire_per_addressa(tabula,
            0x123456789ABC,
            piscina);
        CREDO_NIHIL(found_sym);
    }
}


    /* ==================================================
	 * Probare NIHIL handling
	 * ================================================== */

interior vacuum
_sectio_decima_prima (
    CredoContextus* c)
{
    Piscina* piscina;

    piscina = c->piscina;
    {
          TabulaSymbolorum* tabula;
                  Symbolum* sym;
                    chorda  appellatio;

        /* tabula_symbolorum_ex_macho cum NIHIL */
        tabula = tabula_symbolorum_ex_macho(NIHIL, piscina);
        CREDO_NIHIL(tabula);

        /* tabula_symbolorum_numerus cum NIHIL */
        CREDO_VERUM(tabula_symbolorum_numerus(NIHIL) == ZEPHYRUM);

        /* symbolum_invenire cum NIHIL */
        sym = symbolum_invenire(NIHIL, "_main", piscina);
        CREDO_NIHIL(sym);

        /* symbolum_nomen cum NIHIL */
        appellatio = symbolum_nomen(NIHIL, piscina);
        CREDO_VERUM(appellatio.mensura == ZEPHYRUM);

        /* symbolum_addressa cum NIHIL */
        CREDO_VERUM(symbolum_addressa(NIHIL) == ZEPHYRUM);

        /* symbolum_est_* cum NIHIL */
        CREDO_FALSUM(symbolum_est_functio(NIHIL));
        CREDO_FALSUM(symbolum_est_externum(NIHIL));
        CREDO_FALSUM(symbolum_est_indefinitum(NIHIL));
        CREDO_FALSUM(symbolum_est_debug(NIHIL));
    }
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "tabula_symbolorum_ex_macho", _sectio_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "tabula_symbolorum_symbolum", _sectio_secunda, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "symbolorum_iterator", _sectio_tertia, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "symbolum_invenire", _sectio_quarta, NIHIL, NIHIL, NIHIL, NIHIL },
    { "symbolum attributa", _sectio_quinta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "symbolum_est_* predicates", _sectio_sexta, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { "cum_functione (plures symbola)", _sectio_septima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "symbola_obtinere_functiones", _sectio_octava, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "symbola_obtinere_externa_indefinita", _sectio_nona, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "symbolum_invenire_per_addressa", _sectio_decima, NIHIL, NIHIL,
        NIHIL, NIHIL },
    { "NIHIL handling", _sectio_decima_prima, NIHIL, NIHIL, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("symbola", SECTIONES);
}
