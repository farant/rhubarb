/* silva_quaestio.c - machina selectorum (QA: tags, *, spatium,
 * >, virgula; vide caput + DECISUS doc) */

#include "silva_quaestio.h"
#include <string.h>

/* ==================================================
 * Lexatio selectoris (manu - superficies parva)
 * ================================================== */

nomen structura {
    constans character* fons;
    i32                 positus;
    i32                 mensura;
} LectorSelectoris;

hic_manens vacuum
_spatia_transilire (LectorSelectoris* lector)
{
    dum (lector->positus < lector->mensura)
    {
        character c = lector->fons[lector->positus];

        si (c != ' ' && c != '\t' && c != '\n') frange;
        lector->positus++;
    }
}

hic_manens b32
_est_littera_tag (character c)
{
    redde ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')
        || c == '-' || c == '_') ? VERUM : FALSUM;
}

/* Tag legere: [a-z0-9_-]+ ; initium/mensura in selectorem */
hic_manens i32
_tag_legere (LectorSelectoris* lector, i32* initium_out)
{
    i32 initium = lector->positus;

    dum (lector->positus < lector->mensura
        && _est_littera_tag(lector->fons[lector->positus]))
    {
        lector->positus++;
    }
    *initium_out = initium;
    redde lector->positus - initium;
}

/* ==================================================
 * Compilatio
 * ================================================== */

hic_manens s32
_genus_invenire (
    constans SilvaRegistrumCoctum* tabularium,
    constans character*            tag,
    i32                            mensura)
{
    i32 i;

    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans character* titulus = tabularium->genera[i].titulus;

        si (titulus != NIHIL
            && strlen(titulus) == (size_t)mensura
            && memcmp(titulus, tag, (size_t)mensura) == ZEPHYRUM)
        {
            redde (s32)i;
        }
    }
    redde -I;
}

hic_manens Xar*
_gradum_novum (Piscina* piscina, Xar* catena,
    SilvaQuaestioCombinator combinator)
{
    SilvaQuaestioGradus* gradus =
        (SilvaQuaestioGradus*)xar_addere(catena);

    si (gradus == NIHIL) redde NIHIL;
    gradus->combinator = combinator;
    gradus->partes = xar_creare(piscina,
        (i32)magnitudo(SilvaQuaestioPars));
    redde gradus->partes;
}

SilvaQuaestio*
silva_quaestio_compilare (
    Piscina*                       piscina,
    constans SilvaRegistrumCoctum* tabularium,
    constans character*            selector,
    constans character**           causa_out)
{
    SilvaQuaestio* quaestio;
    LectorSelectoris lector;
    Xar* catena = NIHIL;
    Xar* partes = NIHIL;
    SilvaQuaestioCombinator pendens = SILVA_QUAESTIO_COMB_NULLUS;
    b32 spatium_visum = FALSUM;

    si (causa_out != NIHIL) *causa_out = NIHIL;
    si (piscina == NIHIL || tabularium == NIHIL || selector == NIHIL)
    {
        si (causa_out != NIHIL) *causa_out = "argumentum NIHIL";
        redde NIHIL;
    }

    quaestio = (SilvaQuaestio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaQuaestio));
    si (quaestio == NIHIL) redde NIHIL;
    quaestio->tabularium = tabularium;
    quaestio->catenae = xar_creare(piscina, (i32)magnitudo(Xar*));
    si (quaestio->catenae == NIHIL) redde NIHIL;

    /* forma ambigui ex registro PER NOMEN (exemplar commissionis;
     * absens = registrum sine generibus structuralibus - licet,
     * descensus canonicus tunc otiosis) */
    quaestio->genus_ambigui = -I;
    quaestio->locus_interpretationum = ZEPHYRUM;
    quaestio->locus_canonicae = ZEPHYRUM;
    {
        i32 i;

        per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
        {
            constans SilvaTabGenus* genus = &tabularium->genera[i];
            i32 k;

            si (genus->titulus == NIHIL
                || strcmp(genus->titulus, "ambiguus") != ZEPHYRUM)
            {
                perge;
            }
            quaestio->genus_ambigui = (s32)i;
            per (k = ZEPHYRUM; k < genus->loci_numerus; k++)
            {
                constans SilvaTabLocus* locus =
                    &tabularium->loci[genus->loci_offset + k];

                si (locus->titulus == NIHIL) perge;
                si (strcmp(locus->titulus, "interpretationes")
                    == ZEPHYRUM)
                {
                    quaestio->locus_interpretationum = (i32)k;
                }
                si (strcmp(locus->titulus, "canonica") == ZEPHYRUM)
                {
                    quaestio->locus_canonicae = (i32)k;
                }
            }
            frange;
        }
    }

    lector.fons = selector;
    lector.positus = ZEPHYRUM;
    lector.mensura = (i32)strlen(selector);

    _spatia_transilire(&lector);
    si (lector.positus >= lector.mensura)
    {
        si (causa_out != NIHIL) *causa_out = "selector vacuus";
        redde NIHIL;
    }

    dum (lector.positus < lector.mensura)
    {
        character c = lector.fons[lector.positus];

        si (c == ',')
        {
            /* unio: catena clausa, nova incipit */
            si (catena == NIHIL || partes == NIHIL
                || xar_numerus(partes) == ZEPHYRUM)
            {
                si (causa_out != NIHIL)
                {
                    *causa_out = "virgula sine composito";
                }
                redde NIHIL;
            }
            lector.positus++;
            _spatia_transilire(&lector);
            catena = NIHIL;
            partes = NIHIL;
            pendens = SILVA_QUAESTIO_COMB_NULLUS;
            spatium_visum = FALSUM;
            perge;
        }
        si (c == ' ' || c == '\t' || c == '\n')
        {
            _spatia_transilire(&lector);
            spatium_visum = VERUM;
            perge;
        }
        si (c == '>')
        {
            si (partes == NIHIL)
            {
                si (causa_out != NIHIL)
                {
                    *causa_out = "combinator sine composito priore";
                }
                redde NIHIL;
            }
            lector.positus++;
            _spatia_transilire(&lector);
            pendens = SILVA_QUAESTIO_COMB_FILIUS;
            spatium_visum = FALSUM;
            partes = NIHIL;  /* compositum novum sequatur */
            perge;
        }
        si (c == '*' || _est_littera_tag(c))
        {
            SilvaQuaestioPars* pars;

            /* compositum novum incipit? (post combinatorem aut
             * spatium aut initium catenae) */
            si (partes == NIHIL || spatium_visum)
            {
                si (partes != NIHIL && spatium_visum
                    && pendens == SILVA_QUAESTIO_COMB_NULLUS)
                {
                    pendens = SILVA_QUAESTIO_COMB_DESCENDENS;
                }
                si (catena == NIHIL)
                {
                    Xar** sedes;

                    catena = xar_creare(piscina,
                        (i32)magnitudo(SilvaQuaestioGradus));
                    si (catena == NIHIL) redde NIHIL;
                    sedes = (Xar**)xar_addere(quaestio->catenae);
                    si (sedes == NIHIL) redde NIHIL;
                    *sedes = catena;
                    pendens = SILVA_QUAESTIO_COMB_NULLUS;
                }
                partes = _gradum_novum(piscina, catena, pendens);
                si (partes == NIHIL) redde NIHIL;
                pendens = SILVA_QUAESTIO_COMB_NULLUS;
                spatium_visum = FALSUM;
            }

            pars = (SilvaQuaestioPars*)xar_addere(partes);
            si (pars == NIHIL) redde NIHIL;
            si (c == '*')
            {
                lector.positus++;
                pars->genus = SILVA_QUAESTIO_PARS_UNIVERSALIS;
                pars->nodi_genus = -I;
            }
            alioquin
            {
                i32 initium;
                i32 mensura = _tag_legere(&lector, &initium);
                s32 inventum = _genus_invenire(tabularium,
                    lector.fons + initium, mensura);

                si (inventum < ZEPHYRUM)
                {
                    si (causa_out != NIHIL)
                    {
                        *causa_out = "tag registro ignotum";
                    }
                    redde NIHIL;
                }
                pars->genus = SILVA_QUAESTIO_PARS_GENUS;
                pars->nodi_genus = inventum;
            }
            perge;
        }
        si (causa_out != NIHIL)
        {
            *causa_out = "signum inexpectatum in selectore";
        }
        redde NIHIL;
    }

    si (partes == NIHIL || xar_numerus(partes) == ZEPHYRUM
        || pendens != SILVA_QUAESTIO_COMB_NULLUS)
    {
        si (causa_out != NIHIL)
        {
            *causa_out = "selector imperfectus";
        }
        redde NIHIL;
    }
    redde quaestio;
}

/* ==================================================
 * Congruentia
 * ================================================== */

hic_manens b32
_composito_congruit (
    constans SilvaQuaestioGradus* gradus,
    constans SilvaNodus*          nodus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(gradus->partes); i++)
    {
        constans SilvaQuaestioPars* pars =
            (constans SilvaQuaestioPars*)xar_obtinere(
                gradus->partes, i);

        si (pars == NIHIL) redde FALSUM;
        commutatio (pars->genus)
        {
        casus SILVA_QUAESTIO_PARS_UNIVERSALIS:
            frange;
        casus SILVA_QUAESTIO_PARS_GENUS:
            si (nodus->genus != pars->nodi_genus) redde FALSUM;
            frange;
        ordinarius:
            /* QB/QC partes nondum exsequibiles */
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Catena a gradu `index` deorsum versus maiores: nodus composito
 * congruit, tum gradus prior contra patrem (FILIUS) aut maiorem
 * quemlibet (DESCENDENS, retentatione recursiva - "a b c" ubi b
 * inferior deest sed superior adest). */
hic_manens b32
_catenae_congruit (
    Xar*                 catena,
    i32                  index,
    constans SilvaNodus* nodus)
{
    constans SilvaQuaestioGradus* gradus;

    si (nodus == NIHIL) redde FALSUM;
    gradus = (constans SilvaQuaestioGradus*)xar_obtinere(catena,
        index);
    si (gradus == NIHIL) redde FALSUM;
    si (!_composito_congruit(gradus, nodus)) redde FALSUM;
    si (index == ZEPHYRUM) redde VERUM;

    commutatio (gradus->combinator)
    {
    casus SILVA_QUAESTIO_COMB_FILIUS:
        redde _catenae_congruit(catena, index - I, nodus->pater);
    casus SILVA_QUAESTIO_COMB_DESCENDENS:
    {
        constans SilvaNodus* maior = nodus->pater;

        dum (maior != NIHIL)
        {
            si (_catenae_congruit(catena, index - I, maior))
            {
                redde VERUM;
            }
            maior = maior->pater;
        }
        redde FALSUM;
    }
    ordinarius:
        redde FALSUM;  /* fratres = QB */
    }
}

b32
silva_quaestio_congruit (
    constans SilvaQuaestio* quaestio,
    constans SilvaNodus*    nodus)
{
    i32 c;

    si (quaestio == NIHIL || nodus == NIHIL) redde FALSUM;
    per (c = ZEPHYRUM; c < xar_numerus(quaestio->catenae); c++)
    {
        Xar** catena = (Xar**)xar_obtinere(quaestio->catenae, c);

        si (catena == NIHIL || *catena == NIHIL) perge;
        si (_catenae_congruit(*catena, xar_numerus(*catena) - I,
                nodus))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* ==================================================
 * Exsecutio - ambulatio arboris (canonica per ambigua)
 * ================================================== */

nomen structura {
    constans SilvaQuaestio* quaestio;
    Xar*                    resultata;
} AmbulatioQuaestionis;

hic_manens vacuum _valorem_percurrere (SilvaValor valor,
    AmbulatioQuaestionis* ambulatio, i32 profunditas);

hic_manens vacuum
_nodum_visitare (
    constans SilvaNodus*  nodus,
    AmbulatioQuaestionis* ambulatio,
    i32                   profunditas)
{
    i32 i;

    si (nodus == NIHIL || profunditas > LXIV) redde;

    si (silva_quaestio_congruit(ambulatio->quaestio, nodus))
    {
        SilvaQuaestioResultatum* r = (SilvaQuaestioResultatum*)
            xar_addere(ambulatio->resultata);

        si (r != NIHIL) r->nodus = nodus;
    }

    /* AMBIGUUS: lectio canonica sola descenditur (DECISUS;
     * :lectiones QC laxabit) */
    si (ambulatio->quaestio->genus_ambigui >= ZEPHYRUM
        && nodus->genus == ambulatio->quaestio->genus_ambigui)
    {
        constans SilvaValor* interps;
        constans SilvaValor* canonica;

        si (ambulatio->quaestio->locus_interpretationum
                >= (i32)nodus->numerus_locorum
            || ambulatio->quaestio->locus_canonicae
                >= (i32)nodus->numerus_locorum)
        {
            redde;
        }
        interps = &nodus->loci[
            ambulatio->quaestio->locus_interpretationum];
        canonica = &nodus->loci[ambulatio->quaestio->locus_canonicae];
        si (interps->genus == SILVA_VALOR_LISTA
            && canonica->genus == SILVA_VALOR_INDEX)
        {
            SilvaValor* electa = silva_valor_lista_obtinere(
                *interps, (i32)canonica->datum.index);

            si (electa != NIHIL)
            {
                _valorem_percurrere(*electa, ambulatio,
                    profunditas + I);
            }
        }
        redde;
    }

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        _valorem_percurrere(nodus->loci[i], ambulatio,
            profunditas + I);
    }
}

hic_manens vacuum
_valorem_percurrere (
    SilvaValor            valor,
    AmbulatioQuaestionis* ambulatio,
    i32                   profunditas)
{
    si (profunditas > LXIV) redde;

    si (valor.genus == SILVA_VALOR_LISTA)
    {
        i32 i;

        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL)
            {
                _valorem_percurrere(*elem, ambulatio,
                    profunditas + I);
            }
        }
        redde;
    }
    si (valor.genus == SILVA_VALOR_NODUS
        && valor.datum.nodus != NIHIL)
    {
        _nodum_visitare(valor.datum.nodus, ambulatio,
            profunditas + I);
    }
}

Xar*
silva_quaestio_exsequi (
    constans SilvaQuaestio* quaestio,
    SilvaValor              radix,
    Piscina*                piscina)
{
    AmbulatioQuaestionis ambulatio;

    si (quaestio == NIHIL || piscina == NIHIL) redde NIHIL;
    ambulatio.quaestio = quaestio;
    ambulatio.resultata = xar_creare(piscina,
        (i32)magnitudo(SilvaQuaestioResultatum));
    si (ambulatio.resultata == NIHIL) redde NIHIL;
    _valorem_percurrere(radix, &ambulatio, ZEPHYRUM);
    redde ambulatio.resultata;
}
