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

/* Segmentum selectoris in piscinam copiare (quaestio compilata
 * selectorem vocantis supervivit - QB) */
hic_manens chorda
_chordam_copiare (Piscina* piscina, constans character* fons,
    i32 initium, i32 mensura)
{
    chorda c;
    character* datum;

    c.datum = NIHIL;
    c.mensura = ZEPHYRUM;
    datum = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura > ZEPHYRUM ? mensura : I));
    si (datum == NIHIL) redde c;
    si (mensura > ZEPHYRUM)
    {
        memcpy(datum, fons + initium, (size_t)mensura);
    }
    c.datum = (i8*)datum;
    c.mensura = mensura;
    redde c;
}

/* ==================================================
 * Compilatio
 * ================================================== */

/* Locum nomine in genere invenire: index intra loci generis aut -I
 * (QB attributa - resolutio PER GENUS, vocabularium annotationum) */
hic_manens s32
_locum_invenire (
    constans SilvaRegistrumCoctum* tabularium,
    s32                            genus,
    chorda                         locus)
{
    constans SilvaTabGenus* g;
    i32 k;

    si (genus < ZEPHYRUM
        || (i32)genus >= tabularium->numerus_generum)
    {
        redde -I;
    }
    g = &tabularium->genera[genus];
    per (k = ZEPHYRUM; k < g->loci_numerus; k++)
    {
        constans character* titulus =
            tabularium->loci[g->loci_offset + k].titulus;

        si (titulus != NIHIL
            && strlen(titulus) == (size_t)locus.mensura
            && memcmp(titulus, locus.datum,
                   (size_t)locus.mensura) == ZEPHYRUM)
        {
            redde (s32)k;
        }
    }
    redde -I;
}

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

/* Compositum novum incipere si opus (post combinatorem/spatium/
 * initium catenae) - partes currentes reddit; NIHIL = exhaustum.
 * QB: extractum ex ansa compilationis quia '[' et '$' composita
 * quoque incipere possunt (universalis implicitus). */
hic_manens Xar*
_compositum_parare (Piscina* piscina, SilvaQuaestio* quaestio,
    Xar** catena, Xar* partes,
    SilvaQuaestioCombinator* pendens, b32* spatium_visum)
{
    si (partes != NIHIL && !*spatium_visum)
    {
        redde partes;   /* compositum currens pergit */
    }
    si (partes != NIHIL && *spatium_visum
        && *pendens == SILVA_QUAESTIO_COMB_NULLUS)
    {
        *pendens = SILVA_QUAESTIO_COMB_DESCENDENS;
    }
    si (*catena == NIHIL)
    {
        Xar** sedes;

        *catena = xar_creare(piscina,
            (i32)magnitudo(SilvaQuaestioGradus));
        si (*catena == NIHIL) redde NIHIL;
        sedes = (Xar**)xar_addere(quaestio->catenae);
        si (sedes == NIHIL) redde NIHIL;
        *sedes = *catena;
        *pendens = SILVA_QUAESTIO_COMB_NULLUS;
    }
    partes = _gradum_novum(piscina, *catena, *pendens);
    si (partes == NIHIL) redde NIHIL;
    *pendens = SILVA_QUAESTIO_COMB_NULLUS;
    *spatium_visum = FALSUM;
    redde partes;
}

/* Partem puram addere (campi QB zephyrati - partes generum QA eas
 * numquam tangunt) */
hic_manens SilvaQuaestioPars*
_partem_addere (Xar* partes)
{
    SilvaQuaestioPars* pars =
        (SilvaQuaestioPars*)xar_addere(partes);

    si (pars == NIHIL) redde NIHIL;
    memset(pars, ZEPHYRUM, magnitudo(SilvaQuaestioPars));
    pars->nodi_genus = -I;
    redde pars;
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
    quaestio->gradus_maximi = ZEPHYRUM;

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
        si (c == '>' || c == '+' || c == '~')
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
            pendens = (c == '>') ? SILVA_QUAESTIO_COMB_FILIUS
                : (c == '+') ? SILVA_QUAESTIO_COMB_FRATER_PROXIMUS
                : SILVA_QUAESTIO_COMB_FRATER_SEQUENS;
            spatium_visum = FALSUM;
            partes = NIHIL;  /* compositum novum sequatur */
            perge;
        }
        si (c == '[')
        {
            /* attributum [locus op "valor"] (QB) */
            SilvaQuaestioPars* pars;
            i32 initium;
            i32 mensura;

            partes = _compositum_parare(piscina, quaestio, &catena,
                partes, &pendens, &spatium_visum);
            si (partes == NIHIL) redde NIHIL;
            lector.positus++;   /* '[' */
            _spatia_transilire(&lector);
            mensura = _tag_legere(&lector, &initium);
            si (mensura == ZEPHYRUM)
            {
                si (causa_out != NIHIL)
                {
                    *causa_out = "attributum sine loco";
                }
                redde NIHIL;
            }
            pars = _partem_addere(partes);
            si (pars == NIHIL) redde NIHIL;
            pars->genus = SILVA_QUAESTIO_PARS_ATTRIBUTUM;
            pars->locus_titulus = _chordam_copiare(piscina,
                lector.fons, initium, mensura);
            si (pars->locus_titulus.datum == NIHIL) redde NIHIL;
            pars->attr_op = SILVA_QUAESTIO_ATTR_EXSISTIT;
            _spatia_transilire(&lector);
            si (lector.positus < lector.mensura)
            {
                character oc = lector.fons[lector.positus];

                si (oc == '^' || oc == '$' || oc == '*')
                {
                    si (lector.positus + I >= lector.mensura
                        || lector.fons[lector.positus + I] != '=')
                    {
                        si (causa_out != NIHIL)
                        {
                            *causa_out = "operator attributi"
                                " malformatus";
                        }
                        redde NIHIL;
                    }
                    pars->attr_op = (oc == '^')
                        ? SILVA_QUAESTIO_ATTR_INCIPIT
                        : (oc == '$')
                            ? SILVA_QUAESTIO_ATTR_DESINIT
                            : SILVA_QUAESTIO_ATTR_CONTINET;
                    lector.positus += II;
                }
                alioquin si (oc == '=')
                {
                    pars->attr_op = SILVA_QUAESTIO_ATTR_AEQUALIS;
                    lector.positus++;
                }
            }
            si (pars->attr_op != SILVA_QUAESTIO_ATTR_EXSISTIT)
            {
                /* valor citatus: octeti ad '"' clausum, sine fugis
                 * (limes nominatus v1) */
                _spatia_transilire(&lector);
                si (lector.positus >= lector.mensura
                    || lector.fons[lector.positus] != '"')
                {
                    si (causa_out != NIHIL)
                    {
                        *causa_out = "valor attributi sine"
                            " citatione";
                    }
                    redde NIHIL;
                }
                lector.positus++;
                initium = lector.positus;
                dum (lector.positus < lector.mensura
                    && lector.fons[lector.positus] != '"')
                {
                    lector.positus++;
                }
                si (lector.positus >= lector.mensura)
                {
                    si (causa_out != NIHIL)
                    {
                        *causa_out = "citatio non clausa";
                    }
                    redde NIHIL;
                }
                pars->attr_valor = _chordam_copiare(piscina,
                    lector.fons, initium,
                    lector.positus - initium);
                si (pars->attr_valor.datum == NIHIL) redde NIHIL;
                lector.positus++;   /* '"' */
            }
            _spatia_transilire(&lector);
            si (lector.positus >= lector.mensura
                || lector.fons[lector.positus] != ']')
            {
                si (causa_out != NIHIL)
                {
                    *causa_out = "attributum non clausum";
                }
                redde NIHIL;
            }
            lector.positus++;
            /* validatio compilationis: compositum cum genus-tag ->
             * locus generi notus sit (viae mortuae v1 exstirpatae;
             * compositum sine tag ad tempus cursus differt) */
            {
                i32 p;

                per (p = ZEPHYRUM; p < xar_numerus(partes); p++)
                {
                    constans SilvaQuaestioPars* alia =
                        (constans SilvaQuaestioPars*)xar_obtinere(
                            partes, p);

                    si (alia == NIHIL
                        || alia->genus != SILVA_QUAESTIO_PARS_GENUS)
                    {
                        perge;
                    }
                    si (_locum_invenire(tabularium,
                            alia->nodi_genus,
                            pars->locus_titulus) < ZEPHYRUM)
                    {
                        si (causa_out != NIHIL)
                        {
                            *causa_out = "locus generi ignotus";
                        }
                        redde NIHIL;
                    }
                    frange;
                }
            }
            perge;
        }
        si (c == '$')
        {
            /* captura $nomen (QB) */
            SilvaQuaestioPars* pars;
            i32 initium;
            i32 mensura;

            partes = _compositum_parare(piscina, quaestio, &catena,
                partes, &pendens, &spatium_visum);
            si (partes == NIHIL) redde NIHIL;
            lector.positus++;   /* '$' */
            mensura = _tag_legere(&lector, &initium);
            si (mensura == ZEPHYRUM)
            {
                si (causa_out != NIHIL)
                {
                    *causa_out = "captura sine nomine";
                }
                redde NIHIL;
            }
            pars = _partem_addere(partes);
            si (pars == NIHIL) redde NIHIL;
            pars->genus = SILVA_QUAESTIO_PARS_CAPTURA;
            pars->captura_titulus = _chordam_copiare(piscina,
                lector.fons, initium, mensura);
            si (pars->captura_titulus.datum == NIHIL) redde NIHIL;
            perge;
        }
        si (c == '*' || _est_littera_tag(c))
        {
            SilvaQuaestioPars* pars;

            partes = _compositum_parare(piscina, quaestio, &catena,
                partes, &pendens, &spatium_visum);
            si (partes == NIHIL) redde NIHIL;

            pars = _partem_addere(partes);
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
    /* vestigia (QB): quaternio unus per exsecutionem, mensura =
     * catena longissima */
    {
        i32 c2;

        per (c2 = ZEPHYRUM; c2 < xar_numerus(quaestio->catenae);
             c2++)
        {
            Xar** una = (Xar**)xar_obtinere(quaestio->catenae, c2);

            si (una != NIHIL && *una != NIHIL
                && xar_numerus(*una) > quaestio->gradus_maximi)
            {
                quaestio->gradus_maximi = xar_numerus(*una);
            }
        }
    }
    redde quaestio;
}

/* ==================================================
 * Congruentia
 * ================================================== */

/* Comparatio chordae contra operatorem attributi (mensura-conscia;
 * petitum vacuum: AEQUALIS = vacuo soli, ceteri = VERUM) */
hic_manens b32
_valor_congruit_op (chorda habitum, s32 op, chorda petitum)
{
    si (op == SILVA_QUAESTIO_ATTR_AEQUALIS)
    {
        redde (habitum.mensura == petitum.mensura
            && (petitum.mensura == ZEPHYRUM
                || memcmp(habitum.datum, petitum.datum,
                       (size_t)petitum.mensura) == ZEPHYRUM))
            ? VERUM : FALSUM;
    }
    si (petitum.mensura == ZEPHYRUM) redde VERUM;
    si (habitum.mensura < petitum.mensura) redde FALSUM;
    si (op == SILVA_QUAESTIO_ATTR_INCIPIT)
    {
        redde memcmp(habitum.datum, petitum.datum,
            (size_t)petitum.mensura) == ZEPHYRUM ? VERUM : FALSUM;
    }
    si (op == SILVA_QUAESTIO_ATTR_DESINIT)
    {
        redde memcmp(habitum.datum
                + (habitum.mensura - petitum.mensura),
            petitum.datum, (size_t)petitum.mensura) == ZEPHYRUM
            ? VERUM : FALSUM;
    }
    /* CONTINET */
    {
        i32 i;

        per (i = ZEPHYRUM;
             i + petitum.mensura <= habitum.mensura; i++)
        {
            si (memcmp(habitum.datum + i, petitum.datum,
                    (size_t)petitum.mensura) == ZEPHYRUM)
            {
                redde VERUM;
            }
        }
        redde FALSUM;
    }
}

/* Attributum contra nodum (QB): locus PER GENUS resolvitur; TOKEN
 * valorem comparat, LISTA si elementum lexematis ULLUM congruit
 * (folium-chorda); non-lexema: exsistentia sola. */
hic_manens b32
_attributum_congruit (
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaQuaestioPars*    pars,
    constans SilvaNodus*           nodus)
{
    s32 k = _locum_invenire(tabularium, nodus->genus,
        pars->locus_titulus);
    constans SilvaValor* valor;

    si (k < ZEPHYRUM || k >= (s32)nodus->numerus_locorum)
    {
        redde FALSUM;
    }
    valor = &nodus->loci[k];
    si (pars->attr_op == SILVA_QUAESTIO_ATTR_EXSISTIT)
    {
        redde valor->genus != SILVA_VALOR_NIHIL ? VERUM : FALSUM;
    }
    si (valor->genus == SILVA_VALOR_TOKEN)
    {
        si (valor->datum.token == NIHIL) redde FALSUM;
        redde _valor_congruit_op(valor->datum.token->valor,
            pars->attr_op, pars->attr_valor);
    }
    si (valor->genus == SILVA_VALOR_LISTA)
    {
        i32 i;

        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(*valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(*valor,
                i);

            si (elem != NIHIL && elem->genus == SILVA_VALOR_TOKEN
                && elem->datum.token != NIHIL
                && _valor_congruit_op(elem->datum.token->valor,
                       pars->attr_op, pars->attr_valor))
            {
                redde VERUM;
            }
        }
        redde FALSUM;
    }
    redde FALSUM;
}

hic_manens b32
_composito_congruit (
    constans SilvaQuaestio*       quaestio,
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
        casus SILVA_QUAESTIO_PARS_ATTRIBUTUM:
            si (!_attributum_congruit(quaestio->tabularium, pars,
                    nodus))
            {
                redde FALSUM;
            }
            frange;
        casus SILVA_QUAESTIO_PARS_CAPTURA:
            frange;   /* neutralis - ligatio in exsequi (QB) */
        ordinarius:
            /* QC partes nondum exsequibiles */
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Listam continentem in patre invenire (QB fratres): loci patris
 * percursi, listae perscrutatae pro nodo; index elementi redditur.
 * NIHIL = nodus in loco NODO directo sedet - fratres nulli. */
hic_manens constans SilvaValor*
_listam_continentem (constans SilvaNodus* nodus, s32* index_out)
{
    constans SilvaNodus* pater = nodus->pater;
    i32 i;

    si (pater == NIHIL) redde NIHIL;
    per (i = ZEPHYRUM; i < (i32)pater->numerus_locorum; i++)
    {
        constans SilvaValor* valor = &pater->loci[i];
        i32 n;
        i32 k;

        si (valor->genus != SILVA_VALOR_LISTA) perge;
        n = silva_valor_lista_numerus(*valor);
        per (k = ZEPHYRUM; k < n; k++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(*valor,
                k);

            si (elem != NIHIL && elem->genus == SILVA_VALOR_NODUS
                && elem->datum.nodus == nodus)
            {
                *index_out = (s32)k;
                redde valor;
            }
        }
    }
    redde NIHIL;
}

/* Catena a gradu `index` deorsum versus maiores: nodus composito
 * congruit, tum gradus prior contra patrem (FILIUS), maiorem
 * quemlibet (DESCENDENS, retentatione recursiva), aut fratrem
 * praecedentem (+/~ QB - elementa lexematum interposita [virgulae
 * congeriei] transiliuntur). vestigia (QB, NIHIL licet): quaternio
 * nodorum per gradum, scriptus in VIA VICTRICE SOLA (assignatio
 * post successum superiorem - retentationes mortuae vestigia sua
 * secum ferunt). */
hic_manens b32
_catenae_congruit (
    constans SilvaQuaestio* quaestio,
    Xar*                    catena,
    i32                     index,
    constans SilvaNodus*    nodus,
    constans SilvaNodus**   vestigia)
{
    constans SilvaQuaestioGradus* gradus;

    si (nodus == NIHIL) redde FALSUM;
    gradus = (constans SilvaQuaestioGradus*)xar_obtinere(catena,
        index);
    si (gradus == NIHIL) redde FALSUM;
    si (!_composito_congruit(quaestio, gradus, nodus))
    {
        redde FALSUM;
    }
    si (index == ZEPHYRUM)
    {
        si (vestigia != NIHIL) vestigia[ZEPHYRUM] = nodus;
        redde VERUM;
    }

    commutatio (gradus->combinator)
    {
    casus SILVA_QUAESTIO_COMB_FILIUS:
        si (!_catenae_congruit(quaestio, catena, index - I,
                nodus->pater, vestigia))
        {
            redde FALSUM;
        }
        frange;
    casus SILVA_QUAESTIO_COMB_DESCENDENS:
    {
        constans SilvaNodus* maior = nodus->pater;

        dum (maior != NIHIL)
        {
            si (_catenae_congruit(quaestio, catena, index - I,
                    maior, vestigia))
            {
                frange;
            }
            maior = maior->pater;
        }
        si (maior == NIHIL) redde FALSUM;
        frange;
    }
    casus SILVA_QUAESTIO_COMB_FRATER_PROXIMUS:
    casus SILVA_QUAESTIO_COMB_FRATER_SEQUENS:
    {
        s32 sedes = -I;
        constans SilvaValor* lista = _listam_continentem(nodus,
            &sedes);
        b32 inventum = FALSUM;
        s32 i;   /* signatus: decrementum sub zephyrum terminat */

        si (lista == NIHIL) redde FALSUM;
        per (i = sedes - I; i >= ZEPHYRUM; i--)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(*lista,
                (i32)i);

            si (elem == NIHIL
                || elem->genus != SILVA_VALOR_NODUS
                || elem->datum.nodus == NIHIL)
            {
                perge;   /* lexema interpositum */
            }
            si (_catenae_congruit(quaestio, catena, index - I,
                    elem->datum.nodus, vestigia))
            {
                inventum = VERUM;
                frange;
            }
            si (gradus->combinator
                == SILVA_QUAESTIO_COMB_FRATER_PROXIMUS)
            {
                frange;   /* frater nodus proximus solus */
            }
        }
        si (!inventum) redde FALSUM;
        frange;
    }
    ordinarius:
        redde FALSUM;
    }
    si (vestigia != NIHIL) vestigia[index] = nodus;
    redde VERUM;
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
        si (_catenae_congruit(quaestio, *catena,
                xar_numerus(*catena) - I, nodus, NIHIL))
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
    Piscina*                piscina;
    constans SilvaNodus**   vestigia;  /* quaternio gradus_maximi */
} AmbulatioQuaestionis;

/* Capturas catenae congruentis ligare (QB): gradus OMNES cum
 * $nomine ex vestigiis - insectum v1 #1 (subiectum solum ligatum)
 * exstirpatum. NIHIL = catena sine capturis (nulla allocatio). */
hic_manens Xar*
_capturas_ligare (Xar* catena, constans SilvaNodus** vestigia,
    Piscina* piscina)
{
    Xar* capturae = NIHIL;
    i32 g;

    per (g = ZEPHYRUM; g < xar_numerus(catena); g++)
    {
        constans SilvaQuaestioGradus* gradus =
            (constans SilvaQuaestioGradus*)xar_obtinere(catena, g);
        i32 p;

        si (gradus == NIHIL) perge;
        per (p = ZEPHYRUM; p < xar_numerus(gradus->partes); p++)
        {
            constans SilvaQuaestioPars* pars =
                (constans SilvaQuaestioPars*)xar_obtinere(
                    gradus->partes, p);
            SilvaQuaestioCaptura* cap;

            si (pars == NIHIL
                || pars->genus != SILVA_QUAESTIO_PARS_CAPTURA)
            {
                perge;
            }
            si (capturae == NIHIL)
            {
                capturae = xar_creare(piscina,
                    (i32)magnitudo(SilvaQuaestioCaptura));
                si (capturae == NIHIL) redde NIHIL;
            }
            cap = (SilvaQuaestioCaptura*)xar_addere(capturae);
            si (cap == NIHIL) redde capturae;
            cap->titulus = pars->captura_titulus;
            cap->nodus = vestigia != NIHIL ? vestigia[g] : NIHIL;
        }
    }
    redde capturae;
}

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

    /* congruentia per catenam - catena congruens PRIMA ligat
     * (unio; DECISUS in capite) */
    per (i = ZEPHYRUM;
         i < xar_numerus(ambulatio->quaestio->catenae); i++)
    {
        Xar** catena = (Xar**)xar_obtinere(
            ambulatio->quaestio->catenae, i);

        si (catena == NIHIL || *catena == NIHIL) perge;
        si (_catenae_congruit(ambulatio->quaestio, *catena,
                xar_numerus(*catena) - I, nodus,
                ambulatio->vestigia))
        {
            SilvaQuaestioResultatum* r = (SilvaQuaestioResultatum*)
                xar_addere(ambulatio->resultata);

            si (r != NIHIL)
            {
                r->nodus = nodus;
                r->capturae = _capturas_ligare(*catena,
                    ambulatio->vestigia, ambulatio->piscina);
            }
            frange;
        }
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
    ambulatio.piscina = piscina;
    ambulatio.vestigia = NIHIL;
    si (quaestio->gradus_maximi > ZEPHYRUM)
    {
        ambulatio.vestigia = (constans SilvaNodus**)
            piscina_allocare_ordinatum(piscina,
                (memoriae_index)quaestio->gradus_maximi
                    * (memoriae_index)magnitudo(SilvaNodus*),
                (memoriae_index)magnitudo(vacuum*));
        si (ambulatio.vestigia == NIHIL) redde NIHIL;
    }
    _valorem_percurrere(radix, &ambulatio, ZEPHYRUM);
    redde ambulatio.resultata;
}
