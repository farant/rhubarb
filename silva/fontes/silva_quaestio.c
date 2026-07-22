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
 * Pseudo-classes nativae (QC) - tabula nominum UNA:
 * entia usoris eandem tabulam extendunt (registro)
 * ================================================== */

nomen structura {
    constans character* titulus;
    s32                 genus;      /* SilvaQuaestioPseudoGenus */
    b32                 argumentum_necessarium; /* alioquin vetitum */
} _PseudoNativum;

hic_manens constans _PseudoNativum _pseudo_nativa[] = {
    { "primus",    SILVA_QUAESTIO_PSEUDO_PRIMUS,    FALSUM },
    { "ultimus",   SILVA_QUAESTIO_PSEUDO_ULTIMUS,   FALSUM },
    { "habet",     SILVA_QUAESTIO_PSEUDO_HABET,     VERUM  },
    { "non",       SILVA_QUAESTIO_PSEUDO_NON,       VERUM  },
    { "vocat",     SILVA_QUAESTIO_PSEUDO_VOCAT,     VERUM  },
    { "definit",   SILVA_QUAESTIO_PSEUDO_DEFINIT,   VERUM  },
    { "utitur",    SILVA_QUAESTIO_PSEUDO_UTITUR,    VERUM  },
    { "reddit",    SILVA_QUAESTIO_PSEUDO_REDDIT,    FALSUM },
    { "ambiguum",  SILVA_QUAESTIO_PSEUDO_AMBIGUUM,  FALSUM },
    { "lectiones", SILVA_QUAESTIO_PSEUDO_LECTIONES, FALSUM },
    { "sumptus",   SILVA_QUAESTIO_PSEUDO_SUMPTUS,   FALSUM },
    { "omissus",   SILVA_QUAESTIO_PSEUDO_OMISSUS,   FALSUM }
};

/* Index in tabula nativa aut -I */
hic_manens s32
_pseudo_nativum_invenire (constans character* fons, i32 mensura)
{
    s32 i;

    per (i = ZEPHYRUM; i < (s32)(magnitudo(_pseudo_nativa)
            / magnitudo(_pseudo_nativa[ZEPHYRUM])); i++)
    {
        constans character* titulus = _pseudo_nativa[i].titulus;

        si (strlen(titulus) == (size_t)mensura
            && memcmp(titulus, fons, (size_t)mensura) == ZEPHYRUM)
        {
            redde i;
        }
    }
    redde -I;
}

/* ==================================================
 * Tabula pseudo-classium usoris (QC) - impletur ANTE
 * compilationem (pseudo ignotum = fractura clara; forma
 * v1 [registratio post compilationem] incompatibilis)
 * ================================================== */

nomen structura {
    chorda                     titulus;
    SilvaQuaestioPseudoFunctio functio;
    vacuum*                    datum;
} _PseudoUsoris;

structura SilvaQuaestioPseudoRegistrum {
    Piscina* piscina;
    Xar*     entia;    /* _PseudoUsoris (valore) */
};

SilvaQuaestioPseudoRegistrum*
silva_quaestio_registrum_creare (Piscina* piscina)
{
    SilvaQuaestioPseudoRegistrum* registro;

    si (piscina == NIHIL) redde NIHIL;
    registro = (SilvaQuaestioPseudoRegistrum*)piscina_allocare(
        piscina,
        (memoriae_index)magnitudo(SilvaQuaestioPseudoRegistrum));
    si (registro == NIHIL) redde NIHIL;
    registro->piscina = piscina;
    registro->entia = xar_creare(piscina,
        (i32)magnitudo(_PseudoUsoris));
    si (registro->entia == NIHIL) redde NIHIL;
    redde registro;
}

b32
silva_quaestio_registrare (
    SilvaQuaestioPseudoRegistrum* registro,
    constans character*           titulus,
    SilvaQuaestioPseudoFunctio    functio,
    vacuum*                       datum)
{
    i32 mensura;
    i32 i;
    _PseudoUsoris* ens;

    si (registro == NIHIL || titulus == NIHIL || functio == NIHIL)
    {
        redde FALSUM;
    }
    mensura = (i32)strlen(titulus);
    si (mensura == ZEPHYRUM) redde FALSUM;
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        si (!_est_littera_tag(titulus[i])) redde FALSUM;
    }
    /* nomen nativum obumbrare vetitum */
    si (_pseudo_nativum_invenire(titulus, mensura) >= ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* titulus iam registratus -> renovatio in situ */
    per (i = ZEPHYRUM; i < xar_numerus(registro->entia); i++)
    {
        ens = (_PseudoUsoris*)xar_obtinere(registro->entia, i);
        si (ens != NIHIL && ens->titulus.mensura == mensura
            && memcmp(ens->titulus.datum, titulus,
                   (size_t)mensura) == ZEPHYRUM)
        {
            ens->functio = functio;
            ens->datum = datum;
            redde VERUM;
        }
    }
    ens = (_PseudoUsoris*)xar_addere(registro->entia);
    si (ens == NIHIL) redde FALSUM;
    ens->titulus = _chordam_copiare(registro->piscina, titulus,
        ZEPHYRUM, mensura);
    si (ens->titulus.datum == NIHIL) redde FALSUM;
    ens->functio = functio;
    ens->datum = datum;
    redde VERUM;
}

hic_manens constans _PseudoUsoris*
_pseudo_usoris_invenire (
    constans SilvaQuaestioPseudoRegistrum* registro,
    constans character*                    fons,
    i32                                    mensura)
{
    i32 i;

    si (registro == NIHIL) redde NIHIL;
    per (i = ZEPHYRUM; i < xar_numerus(registro->entia); i++)
    {
        constans _PseudoUsoris* ens =
            (constans _PseudoUsoris*)xar_obtinere(registro->entia,
                i);

        si (ens != NIHIL && ens->titulus.mensura == mensura
            && memcmp(ens->titulus.datum, fons,
                   (size_t)mensura) == ZEPHYRUM)
        {
            redde ens;
        }
    }
    redde NIHIL;
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

/* Locum nomine NUL-terminato in genere invenire (QC formae
 * semanticae - frater _locum_invenire chordae) */
hic_manens s32
_locum_titulo (
    constans SilvaRegistrumCoctum* tabularium,
    i32                            genus_index,
    constans character*            titulus)
{
    constans SilvaTabGenus* g = &tabularium->genera[genus_index];
    i32 k;

    per (k = ZEPHYRUM; k < g->loci_numerus; k++)
    {
        constans character* t =
            tabularium->loci[g->loci_offset + k].titulus;

        si (t != NIHIL && strcmp(t, titulus) == ZEPHYRUM)
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
    pars->pseudo_genus = -I;
    redde pars;
}

/* Argumentum pseudo legere: '(' iam visum; ad ')' aequilibratam
 * scandit (citata "..." transiliuntur - parentheses intra citata
 * non numerantur). VERUM: [initium,finis) sine parenthesibus,
 * positus post ')'. FALSUM: non clausum. */
hic_manens b32
_pseudo_arg_legere (LectorSelectoris* lector, i32* initium_out,
    i32* finis_out)
{
    i32 altitudo = I;

    lector->positus++;   /* '(' */
    *initium_out = lector->positus;
    dum (lector->positus < lector->mensura)
    {
        character c = lector->fons[lector->positus];

        si (c == '"')
        {
            lector->positus++;
            dum (lector->positus < lector->mensura
                && lector->fons[lector->positus] != '"')
            {
                lector->positus++;
            }
            si (lector->positus >= lector->mensura) redde FALSUM;
            lector->positus++;   /* '"' clausum */
            perge;
        }
        si (c == '(') altitudo++;
        alioquin si (c == ')')
        {
            altitudo--;
            si (altitudo == ZEPHYRUM)
            {
                *finis_out = lector->positus;
                lector->positus++;   /* ')' */
                redde VERUM;
            }
        }
        lector->positus++;
    }
    redde FALSUM;
}

/* Compilator internus - registro pro pseudo usoris, profunditas
 * pro recursione :habet/:non (custodita) */
hic_manens SilvaQuaestio*
_compilare_cum (
    Piscina*                               piscina,
    constans SilvaRegistrumCoctum*         tabularium,
    constans SilvaQuaestioPseudoRegistrum* registro,
    constans character*                    selector,
    constans character**                   causa_out,
    i32                                    profunditas)
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
    si (profunditas > XVI)
    {
        si (causa_out != NIHIL)
        {
            *causa_out = "pseudo nidificatio nimis profunda";
        }
        redde NIHIL;
    }

    quaestio = (SilvaQuaestio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaQuaestio));
    si (quaestio == NIHIL) redde NIHIL;
    quaestio->tabularium = tabularium;
    quaestio->catenae = xar_creare(piscina, (i32)magnitudo(Xar*));
    si (quaestio->catenae == NIHIL) redde NIHIL;
    quaestio->gradus_maximi = ZEPHYRUM;

    /* formae generum ex registro PER NOMEN (exemplar commissionis;
     * absens = -I, licet - praedicatum numquam congruit; ambiguus
     * absens = descensus canonicus otiosis) */
    quaestio->genus_ambigui = -I;
    quaestio->locus_interpretationum = ZEPHYRUM;
    quaestio->locus_canonicae = ZEPHYRUM;
    quaestio->lectiones_omnes = FALSUM;
    quaestio->genus_vocationis = -I;
    quaestio->locus_functionis = -I;
    quaestio->genus_folii = -I;
    quaestio->locus_folii_valoris = -I;
    quaestio->genus_declaratoris = -I;
    quaestio->locus_declaratoris_tituli = -I;
    quaestio->genus_redde = -I;
    quaestio->genus_rami_sumpti = -I;
    quaestio->genus_rami_omissi = -I;
    {
        i32 i;

        per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
        {
            constans character* titulus =
                tabularium->genera[i].titulus;

            si (titulus == NIHIL) perge;
            si (strcmp(titulus, "ambiguus") == ZEPHYRUM)
            {
                s32 k;

                quaestio->genus_ambigui = (s32)i;
                k = _locum_titulo(tabularium, i,
                    "interpretationes");
                si (k >= ZEPHYRUM)
                {
                    quaestio->locus_interpretationum = (i32)k;
                }
                k = _locum_titulo(tabularium, i, "canonica");
                si (k >= ZEPHYRUM)
                {
                    quaestio->locus_canonicae = (i32)k;
                }
            }
            alioquin si (strcmp(titulus, "vocatio") == ZEPHYRUM)
            {
                quaestio->genus_vocationis = (s32)i;
                quaestio->locus_functionis =
                    _locum_titulo(tabularium, i, "functio");
            }
            alioquin si (strcmp(titulus, "folium-identificator")
                == ZEPHYRUM)
            {
                quaestio->genus_folii = (s32)i;
                quaestio->locus_folii_valoris =
                    _locum_titulo(tabularium, i, "tok_valor");
            }
            alioquin si (strcmp(titulus, "declarator-titulus")
                == ZEPHYRUM)
            {
                quaestio->genus_declaratoris = (s32)i;
                quaestio->locus_declaratoris_tituli =
                    _locum_titulo(tabularium, i, "tok_titulus");
            }
            alioquin si (strcmp(titulus, "redde") == ZEPHYRUM)
            {
                quaestio->genus_redde = (s32)i;
            }
            alioquin si (strcmp(titulus, "ramus-sumptus")
                == ZEPHYRUM)
            {
                quaestio->genus_rami_sumpti = (s32)i;
            }
            alioquin si (strcmp(titulus, "ramus-omissus")
                == ZEPHYRUM)
            {
                quaestio->genus_rami_omissi = (s32)i;
            }
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
        si (c == ':')
        {
            /* pseudo-classis :titulus aut :titulus(arg) (QC) */
            SilvaQuaestioPars* pars;
            i32 initium;
            i32 mensura;
            s32 index_nativi;
            b32 habet_arg = FALSUM;
            i32 arg_initium = ZEPHYRUM;
            i32 arg_finis = ZEPHYRUM;

            partes = _compositum_parare(piscina, quaestio, &catena,
                partes, &pendens, &spatium_visum);
            si (partes == NIHIL) redde NIHIL;
            lector.positus++;   /* ':' */
            mensura = _tag_legere(&lector, &initium);
            si (mensura == ZEPHYRUM)
            {
                si (causa_out != NIHIL)
                {
                    *causa_out = "pseudo sine nomine";
                }
                redde NIHIL;
            }
            si (lector.positus < lector.mensura
                && lector.fons[lector.positus] == '(')
            {
                si (!_pseudo_arg_legere(&lector, &arg_initium,
                        &arg_finis))
                {
                    si (causa_out != NIHIL)
                    {
                        *causa_out = "argumentum pseudo non"
                            " clausum";
                    }
                    redde NIHIL;
                }
                habet_arg = VERUM;
            }
            pars = _partem_addere(partes);
            si (pars == NIHIL) redde NIHIL;
            pars->genus = SILVA_QUAESTIO_PARS_PSEUDO;
            index_nativi = _pseudo_nativum_invenire(
                lector.fons + initium, mensura);
            si (index_nativi >= ZEPHYRUM)
            {
                constans _PseudoNativum* nativum =
                    &_pseudo_nativa[index_nativi];

                si (nativum->argumentum_necessarium
                    && (!habet_arg || arg_finis == arg_initium))
                {
                    si (causa_out != NIHIL)
                    {
                        *causa_out = "pseudo argumentum requirit";
                    }
                    redde NIHIL;
                }
                si (!nativum->argumentum_necessarium && habet_arg)
                {
                    si (causa_out != NIHIL)
                    {
                        *causa_out = "pseudo argumentum non"
                            " accipit";
                    }
                    redde NIHIL;
                }
                pars->pseudo_genus = nativum->genus;
                si (nativum->genus == SILVA_QUAESTIO_PSEUDO_HABET
                    || nativum->genus == SILVA_QUAESTIO_PSEUDO_NON)
                {
                    /* selector nidificatus: fragmentum
                     * NUL-terminatum -> recursio */
                    character* fragmentum;
                    i32 arg_mensura = arg_finis - arg_initium;

                    fragmentum = (character*)piscina_allocare(
                        piscina,
                        (memoriae_index)(arg_mensura + I));
                    si (fragmentum == NIHIL) redde NIHIL;
                    memcpy(fragmentum, lector.fons + arg_initium,
                        (size_t)arg_mensura);
                    fragmentum[arg_mensura] = '\0';
                    pars->pseudo_quaestio = _compilare_cum(piscina,
                        tabularium, registro, fragmentum,
                        causa_out, profunditas + I);
                    si (pars->pseudo_quaestio == NIHIL)
                    {
                        redde NIHIL;   /* causa iam posita */
                    }
                }
                alioquin si (nativum->argumentum_necessarium)
                {
                    /* vocat/definit/utitur: argumentum textuale */
                    pars->pseudo_arg = _chordam_copiare(piscina,
                        lector.fons, arg_initium,
                        arg_finis - arg_initium);
                    si (pars->pseudo_arg.datum == NIHIL)
                    {
                        redde NIHIL;
                    }
                }
                si (nativum->genus
                    == SILVA_QUAESTIO_PSEUDO_LECTIONES)
                {
                    quaestio->lectiones_omnes = VERUM;
                }
            }
            alioquin
            {
                constans _PseudoUsoris* usoris =
                    _pseudo_usoris_invenire(registro,
                        lector.fons + initium, mensura);

                si (usoris == NIHIL)
                {
                    si (causa_out != NIHIL)
                    {
                        *causa_out = "pseudo ignotum";
                    }
                    redde NIHIL;
                }
                pars->pseudo_genus = SILVA_QUAESTIO_PSEUDO_USOR;
                pars->pseudo_functio = usoris->functio;
                pars->pseudo_datum = usoris->datum;
                si (habet_arg && arg_finis > arg_initium)
                {
                    pars->pseudo_arg = _chordam_copiare(piscina,
                        lector.fons, arg_initium,
                        arg_finis - arg_initium);
                    si (pars->pseudo_arg.datum == NIHIL)
                    {
                        redde NIHIL;
                    }
                }
            }
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

SilvaQuaestio*
silva_quaestio_compilare (
    Piscina*                       piscina,
    constans SilvaRegistrumCoctum* tabularium,
    constans character*            selector,
    constans character**           causa_out)
{
    redde _compilare_cum(piscina, tabularium, NIHIL, selector,
        causa_out, ZEPHYRUM);
}

SilvaQuaestio*
silva_quaestio_compilare_cum_registro (
    Piscina*                               piscina,
    constans SilvaRegistrumCoctum*         tabularium,
    constans SilvaQuaestioPseudoRegistrum* registro,
    constans character*                    selector,
    constans character**                   causa_out)
{
    redde _compilare_cum(piscina, tabularium, registro, selector,
        causa_out, ZEPHYRUM);
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

/* Praenuntiatio (recursio mutua: compositum -> pseudo -> catena
 * nidificata -> compositum) */
hic_manens b32 _pseudo_congruit (constans SilvaQuaestio* quaestio,
    constans SilvaQuaestioPars* pars, constans SilvaNodus* nodus);

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
        casus SILVA_QUAESTIO_PARS_PSEUDO:
            si (!_pseudo_congruit(quaestio, pars, nodus))
            {
                redde FALSUM;
            }
            frange;
        ordinarius:
            /* genus partis incognitum */
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

/* ==================================================
 * Pseudo-classes (QC) - praedicata et ambulatio subarboris
 * ================================================== */

hic_manens b32 _congruit_interna (constans SilvaQuaestio* quaestio,
    constans SilvaNodus* nodus, constans SilvaNodus* limes);
hic_manens b32 _liberos_quaerere (constans SilvaQuaestio* quaestio,
    constans SilvaQuaestioPars* pars, constans SilvaNodus* subiectum,
    constans SilvaNodus* nodus, b32 lectiones_omnes,
    i32 profunditas);

/* Valorem lexematis loci contra argumentum comparare (aequalitas) */
hic_manens b32
_loci_token_aequat (constans SilvaNodus* nodus, s32 locus,
    chorda petitum)
{
    constans SilvaValor* valor;

    si (locus < ZEPHYRUM || locus >= (s32)nodus->numerus_locorum)
    {
        redde FALSUM;
    }
    valor = &nodus->loci[locus];
    si (valor->genus != SILVA_VALOR_TOKEN
        || valor->datum.token == NIHIL)
    {
        redde FALSUM;
    }
    redde _valor_congruit_op(valor->datum.token->valor,
        SILVA_QUAESTIO_ATTR_AEQUALIS, petitum);
}

/* Nodus descendens praedicato pseudo satisfacitne? (HABET:
 * quaestio nidificata limite subiecti confinata; semantici:
 * genus + lexema) */
hic_manens b32
_nodo_probare (
    constans SilvaQuaestio*     quaestio,
    constans SilvaQuaestioPars* pars,
    constans SilvaNodus*        subiectum,
    constans SilvaNodus*        nodus)
{
    commutatio (pars->pseudo_genus)
    {
    casus SILVA_QUAESTIO_PSEUDO_HABET:
        redde _congruit_interna(pars->pseudo_quaestio, nodus,
            subiectum);
    casus SILVA_QUAESTIO_PSEUDO_VOCAT:
    {
        constans SilvaValor* valor;
        constans SilvaNodus* folium;

        si (nodus->genus != quaestio->genus_vocationis)
        {
            redde FALSUM;
        }
        si (quaestio->locus_functionis < ZEPHYRUM
            || quaestio->locus_functionis
                >= (s32)nodus->numerus_locorum)
        {
            redde FALSUM;
        }
        valor = &nodus->loci[quaestio->locus_functionis];
        si (valor->genus != SILVA_VALOR_NODUS
            || valor->datum.nodus == NIHIL)
        {
            redde FALSUM;
        }
        folium = valor->datum.nodus;
        si (folium->genus != quaestio->genus_folii) redde FALSUM;
        redde _loci_token_aequat(folium,
            quaestio->locus_folii_valoris, pars->pseudo_arg);
    }
    casus SILVA_QUAESTIO_PSEUDO_DEFINIT:
        si (nodus->genus != quaestio->genus_declaratoris)
        {
            redde FALSUM;
        }
        redde _loci_token_aequat(nodus,
            quaestio->locus_declaratoris_tituli,
            pars->pseudo_arg);
    casus SILVA_QUAESTIO_PSEUDO_UTITUR:
        si (nodus->genus != quaestio->genus_folii) redde FALSUM;
        redde _loci_token_aequat(nodus,
            quaestio->locus_folii_valoris, pars->pseudo_arg);
    casus SILVA_QUAESTIO_PSEUDO_REDDIT:
        redde (nodus->genus == quaestio->genus_redde)
            ? VERUM : FALSUM;
    ordinarius:
        redde FALSUM;
    }
}

/* Valorem percurrere quaerendo (boolean, exitus primo successu;
 * regulam ambigui _nodum_visitare speculatur) */
hic_manens b32
_valorem_quaerere (
    constans SilvaQuaestio*     quaestio,
    constans SilvaQuaestioPars* pars,
    constans SilvaNodus*        subiectum,
    SilvaValor                  valor,
    b32                         lectiones_omnes,
    i32                         profunditas)
{
    si (profunditas > LXIV) redde FALSUM;
    si (valor.genus == SILVA_VALOR_LISTA)
    {
        i32 i;

        per (i = ZEPHYRUM;
             i < silva_valor_lista_numerus(valor); i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor,
                i);

            si (elem != NIHIL
                && _valorem_quaerere(quaestio, pars, subiectum,
                       *elem, lectiones_omnes, profunditas + I))
            {
                redde VERUM;
            }
        }
        redde FALSUM;
    }
    si (valor.genus == SILVA_VALOR_NODUS
        && valor.datum.nodus != NIHIL)
    {
        si (_nodo_probare(quaestio, pars, subiectum,
                valor.datum.nodus))
        {
            redde VERUM;
        }
        redde _liberos_quaerere(quaestio, pars, subiectum,
            valor.datum.nodus, lectiones_omnes, profunditas + I);
    }
    redde FALSUM;
}

/* Per locos nodi descendere (nodus ipse NON probatur -
 * descendentes soli; regula lectionum ambigui servata) */
hic_manens b32
_liberos_quaerere (
    constans SilvaQuaestio*     quaestio,
    constans SilvaQuaestioPars* pars,
    constans SilvaNodus*        subiectum,
    constans SilvaNodus*        nodus,
    b32                         lectiones_omnes,
    i32                         profunditas)
{
    i32 i;

    si (profunditas > LXIV) redde FALSUM;
    si (quaestio->genus_ambigui >= ZEPHYRUM
        && nodus->genus == quaestio->genus_ambigui)
    {
        constans SilvaValor* interps;
        constans SilvaValor* canonica;

        si (quaestio->locus_interpretationum
                >= (i32)nodus->numerus_locorum
            || quaestio->locus_canonicae
                >= (i32)nodus->numerus_locorum)
        {
            redde FALSUM;
        }
        interps = &nodus->loci[quaestio->locus_interpretationum];
        canonica = &nodus->loci[quaestio->locus_canonicae];
        si (interps->genus != SILVA_VALOR_LISTA) redde FALSUM;
        si (lectiones_omnes)
        {
            redde _valorem_quaerere(quaestio, pars, subiectum,
                *interps, lectiones_omnes, profunditas + I);
        }
        si (canonica->genus == SILVA_VALOR_INDEX)
        {
            SilvaValor* electa = silva_valor_lista_obtinere(
                *interps, (i32)canonica->datum.index);

            si (electa != NIHIL)
            {
                redde _valorem_quaerere(quaestio, pars, subiectum,
                    *electa, lectiones_omnes, profunditas + I);
            }
        }
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        si (_valorem_quaerere(quaestio, pars, subiectum,
                nodus->loci[i], lectiones_omnes, profunditas + I))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* Pars pseudo contra nodum (QC) */
hic_manens b32
_pseudo_congruit (
    constans SilvaQuaestio*     quaestio,
    constans SilvaQuaestioPars* pars,
    constans SilvaNodus*        nodus)
{
    commutatio (pars->pseudo_genus)
    {
    casus SILVA_QUAESTIO_PSEUDO_PRIMUS:
    casus SILVA_QUAESTIO_PSEUDO_ULTIMUS:
    {
        s32 sedes = -I;
        constans SilvaValor* lista = _listam_continentem(nodus,
            &sedes);
        s32 i;   /* signatus: decrementum sub zephyrum terminat */
        s32 n;

        si (lista == NIHIL) redde VERUM;  /* filius solus (DECISUS) */
        n = (s32)silva_valor_lista_numerus(*lista);
        si (pars->pseudo_genus == SILVA_QUAESTIO_PSEUDO_PRIMUS)
        {
            per (i = sedes - I; i >= ZEPHYRUM; i--)
            {
                SilvaValor* elem = silva_valor_lista_obtinere(
                    *lista, (i32)i);

                si (elem != NIHIL
                    && elem->genus == SILVA_VALOR_NODUS
                    && elem->datum.nodus != NIHIL)
                {
                    redde FALSUM;   /* frater nodalis praecedens */
                }
            }
            redde VERUM;
        }
        per (i = sedes + I; i < n; i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(*lista,
                (i32)i);

            si (elem != NIHIL && elem->genus == SILVA_VALOR_NODUS
                && elem->datum.nodus != NIHIL)
            {
                redde FALSUM;   /* frater nodalis sequens */
            }
        }
        redde VERUM;
    }
    casus SILVA_QUAESTIO_PSEUDO_HABET:
    {
        b32 omnes = quaestio->lectiones_omnes;

        si (pars->pseudo_quaestio == NIHIL) redde FALSUM;
        si (pars->pseudo_quaestio->lectiones_omnes) omnes = VERUM;
        redde _liberos_quaerere(quaestio, pars, nodus, nodus,
            omnes, ZEPHYRUM);
    }
    casus SILVA_QUAESTIO_PSEUDO_NON:
        si (pars->pseudo_quaestio == NIHIL) redde FALSUM;
        redde _congruit_interna(pars->pseudo_quaestio, nodus,
            NIHIL) ? FALSUM : VERUM;
    casus SILVA_QUAESTIO_PSEUDO_VOCAT:
        si (quaestio->genus_vocationis < ZEPHYRUM
            || quaestio->genus_folii < ZEPHYRUM)
        {
            redde FALSUM;
        }
        redde _liberos_quaerere(quaestio, pars, nodus, nodus,
            quaestio->lectiones_omnes, ZEPHYRUM);
    casus SILVA_QUAESTIO_PSEUDO_DEFINIT:
        si (quaestio->genus_declaratoris < ZEPHYRUM) redde FALSUM;
        redde _liberos_quaerere(quaestio, pars, nodus, nodus,
            quaestio->lectiones_omnes, ZEPHYRUM);
    casus SILVA_QUAESTIO_PSEUDO_UTITUR:
        si (quaestio->genus_folii < ZEPHYRUM) redde FALSUM;
        redde _liberos_quaerere(quaestio, pars, nodus, nodus,
            quaestio->lectiones_omnes, ZEPHYRUM);
    casus SILVA_QUAESTIO_PSEUDO_REDDIT:
        si (quaestio->genus_redde < ZEPHYRUM) redde FALSUM;
        redde _liberos_quaerere(quaestio, pars, nodus, nodus,
            quaestio->lectiones_omnes, ZEPHYRUM);
    casus SILVA_QUAESTIO_PSEUDO_AMBIGUUM:
        redde (quaestio->genus_ambigui >= ZEPHYRUM
            && nodus->genus == quaestio->genus_ambigui)
            ? VERUM : FALSUM;
    casus SILVA_QUAESTIO_PSEUDO_LECTIONES:
        redde VERUM;   /* vexillum ambulationis - hic neutrale */
    casus SILVA_QUAESTIO_PSEUDO_SUMPTUS:
    casus SILVA_QUAESTIO_PSEUDO_OMISSUS:
    {
        s32 petitum = (pars->pseudo_genus
                == SILVA_QUAESTIO_PSEUDO_SUMPTUS)
            ? quaestio->genus_rami_sumpti
            : quaestio->genus_rami_omissi;
        constans SilvaNodus* n = nodus;

        si (petitum < ZEPHYRUM) redde FALSUM;
        dum (n != NIHIL)
        {
            si (n->genus == petitum) redde VERUM;
            n = n->pater;
        }
        redde FALSUM;
    }
    casus SILVA_QUAESTIO_PSEUDO_USOR:
        si (pars->pseudo_functio == NIHIL) redde FALSUM;
        redde pars->pseudo_functio(nodus, pars->pseudo_arg,
            pars->pseudo_datum);
    ordinarius:
        redde FALSUM;
    }
}

/* Catena a gradu `index` deorsum versus maiores: nodus composito
 * congruit, tum gradus prior contra patrem (FILIUS), maiorem
 * quemlibet (DESCENDENS, retentatione recursiva), aut fratrem
 * praecedentem (+/~ QB - elementa lexematum interposita [virgulae
 * congeriei] transiliuntur). limes (QC :habet, NIHIL licet =
 * liber): ascensus supra limitem vetitus - compositi omnes
 * descendentes STRICTI limitis (paritas CSS selectoris relativi).
 * vestigia (QB, NIHIL licet): quaternio nodorum per gradum,
 * scriptus in VIA VICTRICE SOLA (assignatio post successum
 * superiorem - retentationes mortuae vestigia sua secum ferunt). */
hic_manens b32
_catenae_congruit (
    constans SilvaQuaestio* quaestio,
    Xar*                    catena,
    i32                     index,
    constans SilvaNodus*    nodus,
    constans SilvaNodus*    limes,
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
        si (limes != NIHIL && nodus->pater == limes)
        {
            redde FALSUM;   /* limes ipse extra */
        }
        si (!_catenae_congruit(quaestio, catena, index - I,
                nodus->pater, limes, vestigia))
        {
            redde FALSUM;
        }
        frange;
    casus SILVA_QUAESTIO_COMB_DESCENDENS:
    {
        constans SilvaNodus* maior = nodus->pater;

        dum (maior != NIHIL && maior != limes)
        {
            si (_catenae_congruit(quaestio, catena, index - I,
                    maior, limes, vestigia))
            {
                frange;
            }
            maior = maior->pater;
        }
        si (maior == NIHIL || maior == limes) redde FALSUM;
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
                    elem->datum.nodus, limes, vestigia))
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

/* Congruentia interna: catenae omnes contra nodum, limite dato
 * (QC :habet - NIHIL = liber) */
hic_manens b32
_congruit_interna (
    constans SilvaQuaestio* quaestio,
    constans SilvaNodus*    nodus,
    constans SilvaNodus*    limes)
{
    i32 c;

    si (quaestio == NIHIL || nodus == NIHIL) redde FALSUM;
    per (c = ZEPHYRUM; c < xar_numerus(quaestio->catenae); c++)
    {
        Xar** catena = (Xar**)xar_obtinere(quaestio->catenae, c);

        si (catena == NIHIL || *catena == NIHIL) perge;
        si (_catenae_congruit(quaestio, *catena,
                xar_numerus(*catena) - I, nodus, limes, NIHIL))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
silva_quaestio_congruit (
    constans SilvaQuaestio* quaestio,
    constans SilvaNodus*    nodus)
{
    redde _congruit_interna(quaestio, nodus, NIHIL);
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
                xar_numerus(*catena) - I, nodus, NIHIL,
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

    /* AMBIGUUS: lectio canonica sola descenditur (DECISUS);
     * :lectiones (QC) lectiones OMNES aperit */
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
        si (interps->genus != SILVA_VALOR_LISTA) redde;
        si (ambulatio->quaestio->lectiones_omnes)
        {
            /* lista percursa = lectiones omnes descensae */
            _valorem_percurrere(*interps, ambulatio,
                profunditas + I);
            redde;
        }
        si (canonica->genus == SILVA_VALOR_INDEX)
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
