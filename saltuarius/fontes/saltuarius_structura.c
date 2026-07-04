/* saltuarius_structura.c - Implementatio indicis structurae
 * (variabiles "index" - "structura" macro latina, vide .h) */

#include "saltuarius_structura.h"
#include "silva.h"
#include "chorda_aedificator.h"

SaltuariusStructura*
saltuarius_structura_creare (Piscina* persistens)
{
    SaltuariusStructura* index;

    index = (SaltuariusStructura*)piscina_allocare_ordinatum(
        persistens, (memoriae_index)magnitudo(SaltuariusStructura),
        IV);
    si (index == NIHIL)
    {
        redde NIHIL;
    }
    index->arena = piscina_generare_dynamicum("salt_structura",
        262144);
    si (index->arena == NIHIL)
    {
        redde NIHIL;
    }
    index->nota = piscina_notare(index->arena);
    index->ordines = NIHIL;
    index->numerus = ZEPHYRUM;
    index->selectio = ZEPHYRUM;
    index->volumen = ZEPHYRUM;
    index->apertum = FALSUM;
    redde index;
}

/* Chorda ex SilvaChorda (aliae - vita = vita exp) */
interior chorda
_ex_silva (constans SilvaChorda* s)
{
    chorda c;

    c.mensura = (s != NIHIL) ? s->mensura : ZEPHYRUM;
    c.datum = (s != NIHIL) ? s->datum : NIHIL;
    redde c;
}

/* Titulum ordinis aedificare: praefixum + medium (praecisum XL
 * runas) + insigne (vocator spatia regit: "()" glutinatur,
 * " (omissum)" spatium fert) + "  :linea" si linea > 0 */
interior chorda
_titulum (SaltuariusStructura* index,
    constans character* praefixum, chorda medium,
    constans character* insigne, i32 linea)
{
    ChordaAedificator* aed = chorda_aedificator_creare(
        index->arena, CCLVI);
    chorda fructus;

    si (aed == NIHIL)
    {
        fructus.mensura = ZEPHYRUM;
        fructus.datum = NIHIL;
        redde fructus;
    }
    chorda_aedificator_appendere_literis(aed, praefixum);
    si (medium.mensura > ZEPHYRUM)
    {
        i32 mensura = (medium.mensura < XL) ? medium.mensura : XL;
        i32 k;

        per (k = ZEPHYRUM; k < mensura; k++)
        {
            chorda_aedificator_appendere_character(aed,
                (character)medium.datum[k]);
        }
    }
    si (insigne != NIHIL && insigne[ZEPHYRUM] != '\0')
    {
        chorda_aedificator_appendere_literis(aed, insigne);
    }
    si (linea > ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(aed, "  :");
        chorda_aedificator_appendere_i32(aed, linea);
    }
    fructus = chorda_aedificator_spectare(aed);
    redde fructus;
}

interior vacuum
_ordinem_addere (SaltuariusStructura* index,
    SaltuariusOrdoGenus genus, chorda titulus, b32 saltabile,
    chorda via, i32 linea)
{
    SaltuariusOrdo* ordo = &index->ordines[index->numerus];

    ordo->genus = genus;
    ordo->titulus = titulus;
    ordo->saltabile = saltabile;
    ordo->via = via;
    ordo->linea = linea;
    index->numerus++;
}

interior vacuum
_caput_sectionis (SaltuariusStructura* index,
    constans character* titulus)
{
    chorda vacua;

    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;
    _ordinem_addere(index, SALT_ORDO_TITULUS,
        _titulum(index, titulus, vacua, NIHIL, ZEPHYRUM),
        FALSUM, vacua, ZEPHYRUM);
}

/* Linea (1-basata) ex offset octeti in textu strati 0 (= fons
 * princeps ut scriptus). Vista rami lineam REGIONIS fert (linea
 * directivae apertae, omnibus bracchiis communis) - linea bracchii
 * IPSIUS ex corpus_initium hic derivatur (octeti publici sufficiunt,
 * silva non tangenda). */
interior i32
_linea_ex_offset (constans SaltuariusLiber* liber, s32 offset)
{
    i32 k;

    per (k = I; k < liber->numerus_linearum; k++)
    {
        si ((s32)liber->lineae[k].offset > offset)
        {
            redde k;
        }
    }
    redde liber->numerus_linearum;
}

interior constans character*
_rami_verbum (SilvaRamusGenus genus)
{
    commutatio (genus)
    {
        casus SILVA_RAMUS_IF:     redde "#if";
        casus SILVA_RAMUS_IFDEF:  redde "#ifdef";
        casus SILVA_RAMUS_IFNDEF: redde "#ifndef";
        casus SILVA_RAMUS_ELIF:   redde "#elif";
        casus SILVA_RAMUS_ELSE:   redde "#else";
        ordinarius:               redde "#?";
    }
}

b32
saltuarius_structura_aedificare (SaltuariusStructura* index,
    constans SaltuariusLiber* liber)
{
    constans SilvaExpansio* exp;
    s32 princeps;
    i32 n_inc;
    i32 n_mac;
    i32 n_ram;
    chorda vacua;

    piscina_reficere(index->arena, index->nota);
    index->ordines = NIHIL;
    index->numerus = ZEPHYRUM;
    index->selectio = ZEPHYRUM;
    index->volumen = ZEPHYRUM;
    index->apertum = FALSUM;
    vacua.mensura = ZEPHYRUM;
    vacua.datum = NIHIL;

    si (liber->parsura == NIHIL
        || liber->parsura->expansio == NIHIL)
    {
        redde FALSUM;
    }
    exp = liber->parsura->expansio;
    princeps = liber->parsura->fons_princeps;
    n_inc = silva_inclusiones_numerus(exp);
    n_mac = silva_macros_numerus(exp);
    n_ram = silva_rami_numerus(exp);

    /* summa maxima allocata (filtrum minus implet - arena refecta,
     * dispendium innocuum) */
    index->ordines = (SaltuariusOrdo*)piscina_allocare_ordinatum(
        index->arena,
        (memoriae_index)(III + n_inc + n_mac + n_ram)
            * (memoriae_index)magnitudo(SaltuariusOrdo), IV);
    si (index->ordines == NIHIL)
    {
        redde FALSUM;
    }

    /* INCLUSIONES (fons_ex == princeps: inclusa AB hac plagula) */
    {
        b32 prima = VERUM;
        i32 k;

        per (k = ZEPHYRUM; k < n_inc; k++)
        {
            SilvaInclusioVista vista;

            si (!silva_inclusio_vista(exp, k, &vista)
                || vista.fons_ex != princeps
                || vista.via == NIHIL)
            {
                perge;
            }
            si (prima)
            {
                _caput_sectionis(index, "INCLUSIONES");
                prima = FALSUM;
            }
            {
                constans character* insigne = "";
                b32 saltabile = (vista.fons_ad >= ZEPHYRUM)
                    ? VERUM : FALSUM;

                si (vista.est_praetermissa)
                {
                    insigne = " (praetermissa)";
                }
                alioquin si (vista.fons_ad < ZEPHYRUM)
                {
                    insigne = " (non resoluta)";
                }
                _ordinem_addere(index, SALT_ORDO_INCLUSIO,
                    _titulum(index, "", _ex_silva(vista.via),
                        insigne, ZEPHYRUM),
                    saltabile, _ex_silva(vista.via), ZEPHYRUM);
            }
        }
    }

    /* DEFINITIONES (diarium ut est: redefinitiones bis) */
    {
        b32 prima = VERUM;
        i32 k;

        per (k = ZEPHYRUM; k < n_mac; k++)
        {
            SilvaMacroVista vista;

            si (!silva_macro_vista(exp, k, &vista)
                || vista.fons_index != princeps
                || vista.titulus == NIHIL)
            {
                perge;
            }
            si (prima)
            {
                _caput_sectionis(index, "DEFINITIONES");
                prima = FALSUM;
            }
            _ordinem_addere(index, SALT_ORDO_DEFINITIO,
                _titulum(index, "", _ex_silva(vista.titulus),
                    vista.est_functio ? "()" : "", vista.linea),
                VERUM, vacua, vista.linea);
        }
    }

    /* REGIONES (bracchia conditionum, cum sorte) */
    {
        b32 prima = VERUM;
        i32 k;

        per (k = ZEPHYRUM; k < n_ram; k++)
        {
            SilvaRamusVista vista;

            si (!silva_ramus_vista(exp, k, &vista)
                || vista.fons_index != princeps)
            {
                perge;
            }
            si (prima)
            {
                _caput_sectionis(index, "REGIONES");
                prima = FALSUM;
            }
            {
                constans character* insigne = "";
                i32 linea_bracchii = vista.linea;

                si (vista.est_numquam)
                {
                    insigne = " (numquam)";
                }
                alioquin si (!vista.est_sumptum)
                {
                    insigne = " (omissum)";
                }
                /* corpus bracchii si adest; bracchium vacuum ->
                 * linea regionis (refugium) */
                si (vista.corpus_initium >= ZEPHYRUM)
                {
                    linea_bracchii = _linea_ex_offset(liber,
                        vista.corpus_initium);
                }
                _ordinem_addere(index, SALT_ORDO_REGIO,
                    _titulum(index, _rami_verbum(vista.genus),
                        vacua, insigne, linea_bracchii),
                    VERUM, vacua, linea_bracchii);
            }
        }
    }

    si (index->numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* selectio in primum saltabilem (tituli praetereuntur) */
    {
        s32 k;

        per (k = ZEPHYRUM; k < (s32)index->numerus; k++)
        {
            si (index->ordines[k].saltabile)
            {
                index->selectio = k;
                frange;
            }
        }
    }
    index->apertum = VERUM;
    redde VERUM;
}

vacuum
saltuarius_structura_claudere (SaltuariusStructura* index)
{
    index->apertum = FALSUM;
}

vacuum
saltuarius_structura_movere (SaltuariusStructura* index,
    s32 delta)
{
    s32 nova = index->selectio + delta;

    si (nova < ZEPHYRUM)
    {
        nova = ZEPHYRUM;
    }
    si (nova >= (s32)index->numerus)
    {
        nova = (s32)index->numerus - I;
    }
    index->selectio = nova;
}

vacuum
saltuarius_structura_aptare (SaltuariusStructura* index,
    s32 altitudo)
{
    si (altitudo < I)
    {
        redde;
    }
    si (index->selectio < index->volumen)
    {
        index->volumen = index->selectio;
    }
    si (index->selectio >= index->volumen + altitudo)
    {
        index->volumen = index->selectio - altitudo + I;
    }
    si (index->volumen < ZEPHYRUM)
    {
        index->volumen = ZEPHYRUM;
    }
}

constans SaltuariusOrdo*
saltuarius_structura_saltus (constans SaltuariusStructura* index)
{
    si (!index->apertum || index->selectio < ZEPHYRUM
        || index->selectio >= (s32)index->numerus)
    {
        redde NIHIL;
    }
    si (!index->ordines[index->selectio].saltabile)
    {
        redde NIHIL;
    }
    redde &index->ordines[index->selectio];
}
