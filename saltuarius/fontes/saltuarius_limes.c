/* saltuarius_limes.c - Implementatio limitis disci */

#include "saltuarius_limes.h"
#include "iter_directoria.h"
#include "filum.h"
#include "via.h"
#include <string.h>

/* Chordam in literis NUL-terminatas transcribere (bibliothecae
 * systematis vias NUL-terminatas volunt; chorda non est!) */
interior constans character*
_literis (Piscina* piscina, chorda via)
{
    character* buffer;

    buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)via.mensura + I);
    si (buffer == NIHIL)
    {
        redde "";
    }
    memcpy(buffer, via.datum, (memoriae_index)via.mensura);
    buffer[via.mensura] = '\0';
    redde buffer;
}

/* Introitus directorii legere in tabulam (ex arena data); reddit
 * numerum, aut -1 si directorium non legibile. Ultra MAXIMI:
 * praecisio tacita. */
interior s32
_introitus_legere (Piscina* piscina, constans character* via,
    DirectoriumIntroitus** tabula_out)
{
    DirectoriumIterator* iter;
    DirectoriumIntroitus* intr;
    DirectoriumIntroitus* tabula;
    i32 numerus = ZEPHYRUM;

    *tabula_out = NIHIL;
    iter = directorium_iterator_aperire(via, piscina);
    si (iter == NIHIL)
    {
        redde -I;
    }
    tabula = (DirectoriumIntroitus*)piscina_allocare_ordinatum(
        piscina, (memoriae_index)SALT_INTROITUS_MAXIMI
            * (memoriae_index)magnitudo(DirectoriumIntroitus), IV);
    si (tabula == NIHIL)
    {
        directorium_iterator_claudere(iter);
        redde -I;
    }
    dum ((intr = directorium_iterator_proximum(iter)) != NIHIL
        && numerus < SALT_INTROITUS_MAXIMI)
    {
        /* TRANSCRIPTIO PROFUNDA necessaria: introitus iteratoris
         * "validus usque ad proximum" - titulus.datum in buffer
         * REUSO habitat; transcriptio valoris sola omnes titulos
         * ad ultimum lectum aliaret (morsus verus, probatio
         * integrationis eum invenit) */
        tabula[numerus] = *intr;
        tabula[numerus].titulus = chorda_transcribere(
            intr->titulus, piscina);
        numerus++;
    }
    directorium_iterator_claudere(iter);
    *tabula_out = tabula;
    redde (s32)numerus;
}

/* Selectionem tabularii ad titulum datum ponere (si inventus) */
interior vacuum
_seligere_titulum (SaltuariusTabularium* tab, chorda titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < tab->numerus; k++)
    {
        si (chorda_aequalis(tab->introitus[k].titulus, titulus))
        {
            tab->selectio = (s32)k;
            redde;
        }
    }
}

SaltuariusLimes*
saltuarius_limes_creare (Piscina* persistens,
    SaltuariusTabularium* tab_parens,
    SaltuariusTabularium* tab_currens,
    SaltuariusTabularium* tab_praevisus)
{
    SaltuariusLimes* limes;

    limes = (SaltuariusLimes*)piscina_allocare_ordinatum(persistens,
        (memoriae_index)magnitudo(SaltuariusLimes), IV);
    si (limes == NIHIL)
    {
        redde NIHIL;
    }
    limes->indicis = piscina_generare_dynamicum("salt_indicis",
        4194304);
    limes->praevisus = piscina_generare_dynamicum("salt_praevisus",
        4194304);
    si (limes->indicis == NIHIL || limes->praevisus == NIHIL)
    {
        redde NIHIL;
    }
    limes->nota_indicis = piscina_notare(limes->indicis);
    limes->nota_praevisus = piscina_notare(limes->praevisus);
    limes->tab_parens = tab_parens;
    limes->tab_currens = tab_currens;
    limes->tab_praevisus = tab_praevisus;
    redde limes;
}

vacuum
saltuarius_limes_destruere (SaltuariusLimes* limes)
{
    si (limes->indicis != NIHIL)
    {
        piscina_destruere(limes->indicis);
        limes->indicis = NIHIL;
    }
    si (limes->praevisus != NIHIL)
    {
        piscina_destruere(limes->praevisus);
        limes->praevisus = NIHIL;
    }
}

b32
saltuarius_limes_indices (SaltuariusLimes* limes,
    constans SaltuariusRes* res, SaltuariusColumnae* columnae,
    constans chorda* selectandum)
{
    DirectoriumIntroitus* tabula;
    s32 numerus;
    b32 legibile = VERUM;
    b32 est_radix_arboris;

    piscina_reficere(limes->indicis, limes->nota_indicis);

    /* currens */
    numerus = _introitus_legere(limes->indicis,
        _literis(limes->indicis, res->via_currens), &tabula);
    si (numerus < ZEPHYRUM)
    {
        legibile = FALSUM;
        numerus = ZEPHYRUM;
    }
    saltuarius_tabularium_implere(limes->tab_currens,
        limes->indicis, tabula, (i32)numerus);
    si (selectandum != NIHIL)
    {
        _seligere_titulum(limes->tab_currens, *selectandum);
    }

    /* parens (NIHIL ad radicem "/") */
    est_radix_arboris = chorda_aequalis_literis(res->via_currens,
        "/");
    si (est_radix_arboris)
    {
        columnae->parens = NIHIL;
    }
    alioquin
    {
        chorda via_parentis = via_directorium(res->via_currens,
            limes->indicis);

        numerus = _introitus_legere(limes->indicis,
            _literis(limes->indicis, via_parentis), &tabula);
        si (numerus < ZEPHYRUM)
        {
            numerus = ZEPHYRUM;
        }
        saltuarius_tabularium_implere(limes->tab_parens,
            limes->indicis, tabula, (i32)numerus);
        _seligere_titulum(limes->tab_parens,
            via_nomen(res->via_currens, limes->indicis));
        columnae->parens = limes->tab_parens;
    }

    columnae->currens = limes->tab_currens;
    redde legibile;
}

vacuum
saltuarius_limes_praevisum (SaltuariusLimes* limes,
    constans SaltuariusRes* res, SaltuariusColumnae* columnae)
{
    constans DirectoriumIntroitus* selectum;
    chorda partes[II];
    chorda via_selecti;

    piscina_reficere(limes->praevisus, limes->nota_praevisus);
    columnae->praevisus_genus = SALT_PRAEVISUS_NIHIL;
    columnae->praevisus_tab = NIHIL;
    columnae->praevisus_textus.mensura = ZEPHYRUM;
    columnae->praevisus_textus.datum = NIHIL;

    selectum = saltuarius_tabularium_selectum(limes->tab_currens);
    si (selectum == NIHIL)
    {
        redde;
    }
    partes[ZEPHYRUM] = res->via_currens;
    partes[I] = selectum->titulus;
    via_selecti = via_iungere(partes, II, limes->praevisus);

    commutatio (selectum->genus)
    {
        casus INTROITUS_DIRECTORIUM:
        {
            DirectoriumIntroitus* tabula;
            s32 numerus = _introitus_legere(limes->praevisus,
                _literis(limes->praevisus, via_selecti), &tabula);

            si (numerus < ZEPHYRUM)
            {
                columnae->praevisus_genus = SALT_PRAEVISUS_NUNTIUS;
                columnae->praevisus_nuntius = chorda_ex_literis(
                    "(non legibile)", limes->praevisus);
                redde;
            }
            saltuarius_tabularium_implere(limes->tab_praevisus,
                limes->praevisus, tabula, (i32)numerus);
            columnae->praevisus_genus = SALT_PRAEVISUS_DIRECTORIUM;
            columnae->praevisus_tab = limes->tab_praevisus;
            redde;
        }
        casus INTROITUS_FILUM:
        {
            chorda textus;
            i32 k;
            i32 speculandi;

            si (selectum->mensura
                > (memoriae_index)SALT_PRAEVISUS_MAXIMUS)
            {
                columnae->praevisus_genus = SALT_PRAEVISUS_NUNTIUS;
                columnae->praevisus_nuntius = chorda_ex_literis(
                    "(filum magnum)", limes->praevisus);
                redde;
            }
            textus = filum_legere_totum(
                _literis(limes->praevisus, via_selecti),
                limes->praevisus);
            si (textus.mensura == ZEPHYRUM)
            {
                columnae->praevisus_genus = SALT_PRAEVISUS_NUNTIUS;
                columnae->praevisus_nuntius = chorda_ex_literis(
                    (selectum->mensura > ZEPHYRUM)
                        ? "(non legibile)" : "(vacuum)",
                    limes->praevisus);
                redde;
            }
            /* custodia binarii: NUL in primis 512 octetis */
            speculandi = (textus.mensura < DXII)
                ? textus.mensura : DXII;
            per (k = ZEPHYRUM; k < speculandi; k++)
            {
                si (textus.datum[k] == ZEPHYRUM)
                {
                    columnae->praevisus_genus =
                        SALT_PRAEVISUS_NUNTIUS;
                    columnae->praevisus_nuntius = chorda_ex_literis(
                        "(filum binarium)", limes->praevisus);
                    redde;
                }
            }
            columnae->praevisus_genus = SALT_PRAEVISUS_TEXTUS;
            columnae->praevisus_textus = textus;
            redde;
        }
        ordinarius:
            columnae->praevisus_genus = SALT_PRAEVISUS_NUNTIUS;
            columnae->praevisus_nuntius = chorda_ex_literis(
                "(non intrabile)", limes->praevisus);
            redde;
    }
}

SaltuariusLiber*
saltuarius_limes_librum_aperire (SaltuariusLimes* limes,
    Piscina* persistens, SaltuariusNexus* nexus,
    constans SaltuariusRes* res, constans character** causa_out)
{
    constans DirectoriumIntroitus* selectum;
    chorda partes[II];
    chorda via_selecti;
    chorda textus;
    SaltuariusLiber* liber;

    *causa_out = "";
    selectum = saltuarius_tabularium_selectum(limes->tab_currens);
    si (selectum == NIHIL || selectum->genus != INTROITUS_FILUM)
    {
        *causa_out = "(non filum)";
        redde NIHIL;
    }
    si (selectum->mensura > (memoriae_index)SALT_PRAEVISUS_MAXIMUS)
    {
        *causa_out = "(filum magnum)";
        redde NIHIL;
    }

    /* arena praevisus ut spatium transitorium (liber transcribit;
     * praevisus post reditum ad columnas repletur) */
    piscina_reficere(limes->praevisus, limes->nota_praevisus);
    partes[ZEPHYRUM] = res->via_currens;
    partes[I] = selectum->titulus;
    via_selecti = via_iungere(partes, II, limes->praevisus);
    textus = filum_legere_totum(
        _literis(limes->praevisus, via_selecti), limes->praevisus);
    si (textus.mensura == ZEPHYRUM && selectum->mensura > ZEPHYRUM)
    {
        *causa_out = "(non legibile)";
        redde NIHIL;
    }
    {
        i32 speculandi = (textus.mensura < DXII)
            ? textus.mensura : DXII;
        i32 k;

        per (k = ZEPHYRUM; k < speculandi; k++)
        {
            si (textus.datum[k] == ZEPHYRUM)
            {
                *causa_out = "(filum binarium)";
                redde NIHIL;
            }
        }
    }
    liber = saltuarius_liber_aperire(persistens, nexus, via_selecti,
        textus);
    si (liber == NIHIL)
    {
        *causa_out = "(apertura fracta)";
    }
    redde liber;
}

i32
saltuarius_limes_capita_legere (SaltuariusLimes* limes,
    Piscina* persistens, constans chorda* viae, i32 numerus_viarum,
    SaltuariusCaput** capita_out)
{
    SaltuariusCaput* capita;
    i32 numerus = ZEPHYRUM;
    i32 v;

    (vacuum)limes;
    capita = (SaltuariusCaput*)piscina_allocare_ordinatum(
        persistens, (memoriae_index)DXII
            * (memoriae_index)magnitudo(SaltuariusCaput), IV);
    *capita_out = capita;
    si (capita == NIHIL)
    {
        redde ZEPHYRUM;
    }
    per (v = ZEPHYRUM; v < numerus_viarum; v++)
    {
        DirectoriumIterator* iter = directorium_iterator_aperire(
            _literis(persistens, viae[v]), persistens);
        DirectoriumIntroitus* intr;

        si (iter == NIHIL)
        {
            perge;   /* via -I absens: degradatio tacita */
        }
        dum ((intr = directorium_iterator_proximum(iter)) != NIHIL
            && numerus < DXII)
        {
            chorda titulus;

            si (intr->genus != INTROITUS_FILUM
                || intr->titulus.mensura < III
                || intr->titulus.datum[intr->titulus.mensura - II]
                    != '.'
                || intr->titulus.datum[intr->titulus.mensura - I]
                    != 'h')
            {
                perge;
            }
            titulus = chorda_transcribere(intr->titulus,
                persistens);
            {
                chorda partes[II];
                chorda via_plena;
                chorda textus;

                partes[ZEPHYRUM] = viae[v];
                partes[I] = titulus;
                via_plena = via_iungere(partes, II, persistens);
                textus = filum_legere_totum(
                    _literis(persistens, via_plena), persistens);
                si (textus.mensura == ZEPHYRUM)
                {
                    perge;
                }
                capita[numerus].titulus = titulus;
                capita[numerus].textus = textus;
                capita[numerus].via_absoluta = via_plena;
                numerus++;
            }
        }
        directorium_iterator_claudere(iter);
    }
    redde numerus;
}

SaltuariusLiber*
saltuarius_limes_librum_via (SaltuariusLimes* limes,
    Piscina* persistens, SaltuariusNexus* nexus, chorda via,
    constans character** causa_out)
{
    constans character* via_literis;
    memoriae_index mensura;
    chorda textus;

    *causa_out = "";
    piscina_reficere(limes->praevisus, limes->nota_praevisus);
    via_literis = _literis(limes->praevisus, via);
    mensura = filum_mensura(via_literis);
    si (mensura > (memoriae_index)SALT_PRAEVISUS_MAXIMUS)
    {
        *causa_out = "(filum magnum)";
        redde NIHIL;
    }
    textus = filum_legere_totum(via_literis, limes->praevisus);
    si (textus.mensura == ZEPHYRUM && mensura > ZEPHYRUM)
    {
        *causa_out = "(non legibile)";
        redde NIHIL;
    }
    {
        i32 speculandi = (textus.mensura < DXII)
            ? textus.mensura : DXII;
        i32 k;

        per (k = ZEPHYRUM; k < speculandi; k++)
        {
            si (textus.datum[k] == ZEPHYRUM)
            {
                *causa_out = "(filum binarium)";
                redde NIHIL;
            }
        }
    }
    {
        SaltuariusLiber* liber = saltuarius_liber_aperire(
            persistens, nexus, via, textus);

        si (liber == NIHIL)
        {
            *causa_out = "(apertura fracta)";
        }
        redde liber;
    }
}
