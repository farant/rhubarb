/* silva_commissio.c - Implementatio passus commissionis */

#include "silva_commissio.h"
#include <stdio.h>
#include <string.h>

/* ==================================================
 * Oraculum
 * ================================================== */

SilvaOraculum*
silva_oraculum_creare (Piscina* piscina)
{
    SilvaOraculum* oraculum;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    oraculum = (SilvaOraculum*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaOraculum));
    si (oraculum == NIHIL)
    {
        redde NIHIL;
    }
    oraculum->piscina = piscina;
    oraculum->typi = tabula_dispersa_creare_chorda(piscina, XVI);
    si (oraculum->typi == NIHIL)
    {
        redde NIHIL;
    }
    oraculum->responsa = NIHIL;  /* pigre creatum */
    redde oraculum;
}

/* ==================================================
 * Verdicta praecomputata (sanatio oraculi 2026-07-06)
 * ================================================== */

nomen structura {
    constans SilvaNodus* sedes;
    s32                  victor;
} OraculumResponsum;

vacuum
silva_oraculum_responsa_vacare (SilvaOraculum* oraculum)
{
    si (oraculum == NIHIL) redde;
    si (oraculum->responsa == NIHIL)
    {
        oraculum->responsa = xar_creare(oraculum->piscina,
            (i32)magnitudo(OraculumResponsum));
        redde;
    }
    xar_vacare(oraculum->responsa);
}

b32
silva_oraculum_responsum_ponere (
    SilvaOraculum*       oraculum,
    constans SilvaNodus* sedes,
    s32                  victor)
{
    OraculumResponsum* slot;

    si (oraculum == NIHIL || sedes == NIHIL) redde FALSUM;
    si (oraculum->responsa == NIHIL)
    {
        silva_oraculum_responsa_vacare(oraculum);
        si (oraculum->responsa == NIHIL) redde FALSUM;
    }
    slot = (OraculumResponsum*)xar_addere(oraculum->responsa);
    si (slot == NIHIL) redde FALSUM;
    slot->sedes = sedes;
    slot->victor = victor;
    redde VERUM;
}

b32
silva_oraculum_responsum_quaerere (
    constans SilvaOraculum* oraculum,
    constans SilvaNodus*    sedes,
    s32*                    victor_out)
{
    i32 i;

    si (oraculum == NIHIL || oraculum->responsa == NIHIL
        || sedes == NIHIL)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(oraculum->responsa); i++)
    {
        constans OraculumResponsum* r =
            (constans OraculumResponsum*)xar_obtinere(
                oraculum->responsa, i);

        si (r != NIHIL && r->sedes == sedes)
        {
            si (victor_out != NIHIL) *victor_out = r->victor;
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
silva_oraculum_typum_addere (SilvaOraculum* oraculum, chorda titulus)
{
    /* Sine situ = "visibile ab initio" (situs 0) - typi praeonerati
     * (API, latina) ubique valent */
    redde silva_oraculum_typum_addere_situ(oraculum, titulus,
        ZEPHYRUM);
}

b32
silva_oraculum_typum_addere_situ (
    SilvaOraculum* oraculum,
    chorda         titulus,
    s32            situs)
{
    chorda copia;
    s32*   sedes;
    s32    prior;

    si (oraculum == NIHIL)
    {
        redde FALSUM;
    }
    /* Situs PRIMUS servatur (visibilitas a declaratione prima;
     * legalitas redeclarationis = res laminae lint, M2b sim X) */
    si (silva_oraculum_situs_typi(oraculum, titulus, &prior)
        && prior <= situs)
    {
        redde VERUM;
    }
    sedes = (s32*)piscina_allocare(oraculum->piscina,
        (memoriae_index)magnitudo(s32));
    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    *sedes = situs;
    /* Copia in piscinam oraculi: valores lexematum prospectus in
     * fontem sunt - fons post oraculum mori potest */
    copia = chorda_transcribere(titulus, oraculum->piscina);
    redde tabula_dispersa_inserere(oraculum->typi, copia,
        (vacuum*)sedes);
}

b32
silva_oraculum_situs_typi (
    constans SilvaOraculum* oraculum,
    chorda                  titulus,
    s32*                    situs_out)
{
    vacuum* valor = NIHIL;

    si (oraculum == NIHIL || situs_out == NIHIL)
    {
        redde FALSUM;
    }
    si (!tabula_dispersa_invenire(oraculum->typi, titulus, &valor)
        || valor == NIHIL)
    {
        redde FALSUM;
    }
    *situs_out = *(s32*)valor;
    redde VERUM;
}

b32
silva_oraculum_typum_addere_literis (
    SilvaOraculum*      oraculum,
    constans character* titulus)
{
    si (oraculum == NIHIL || titulus == NIHIL)
    {
        redde FALSUM;
    }
    redde silva_oraculum_typum_addere(oraculum,
        chorda_ex_literis(titulus, oraculum->piscina));
}

b32
silva_oraculum_typum_novit (
    constans SilvaOraculum* oraculum,
    chorda                  titulus)
{
    si (oraculum == NIHIL)
    {
        redde FALSUM;
    }
    redde tabula_dispersa_continet(oraculum->typi, titulus);
}


/* ==================================================
 * Ambulatio
 * ================================================== */

nomen structura {
    Piscina*                piscina;
    SilvaCommissio*         commissio;
    constans SilvaOraculum* oraculum;
    SilvaResolutor          resolutor;
    vacuum*                 datum_resolutoris;
} SilvaAmbulatio;

interior SilvaValor _valorem_committere (SilvaAmbulatio* ambulatio,
    SilvaValor valor, SilvaNodus* pater);

/* Normalizatio prospectus (A½): mensura exacta post commissionem -
 * consumptores xar_numerus tuto legere possunt */
interior SilvaValor
_listam_normalizare (SilvaAmbulatio* ambulatio, SilvaValor lista)
{
    Xar* exactum;
    i32  i;

    si (lista.genus != SILVA_VALOR_LISTA || lista.datum.lista.xar == NIHIL
        || xar_numerus(lista.datum.lista.xar) == lista.datum.lista.mensura)
    {
        redde lista;
    }
    exactum = xar_creare(ambulatio->piscina,
        (i32)magnitudo(SilvaValor));
    si (exactum == NIHIL)
    {
        redde lista;
    }
    per (i = ZEPHYRUM; i < lista.datum.lista.mensura; i++)
    {
        SilvaValor* fons = silva_valor_lista_obtinere(lista, i);
        SilvaValor* dest = (SilvaValor*)xar_addere(exactum);

        si (fons != NIHIL && dest != NIHIL)
        {
            *dest = *fons;
        }
    }
    redde silva_valor_lista(exactum);
}

/* Nodus ordinarius: loci per ambulationem, filii pater accipiunt */
interior vacuum
_nodum_committere (SilvaAmbulatio* ambulatio, SilvaNodus* nodus)
{
    i32 i;

    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        nodus->loci[i] = _valorem_committere(ambulatio, nodus->loci[i],
            nodus);
    }
}

/* Nodus AMBIGUUS: collapsus si oraculum novit, alioquin superstes
 * cum spina canonica */
interior vacuum
_ambiguum_committere (SilvaAmbulatio* ambulatio, SilvaNodus* nodus)
{
    SilvaCommissio* commissio = ambulatio->commissio;
    SilvaValor      interps;
    s32             canonica;

    /* Resolutio: oraculum iam novit? -> COLLAPSUS (victor super
     * allocationem copiatur - identitas manet, involucrum abiit) */
    si (ambulatio->oraculum != NIHIL && ambulatio->resolutor != NIHIL)
    {
        SilvaResolutioResponsum responsum;

        responsum.victor = -I;
        responsum.discriminans = NIHIL;
        ambulatio->resolutor(nodus, ambulatio->oraculum,
            ambulatio->datum_resolutoris, &responsum);
        si (responsum.victor >= ZEPHYRUM)
        {
            SilvaValor* victor_valor = silva_valor_lista_obtinere(
                nodus->loci[commissio->locus_interpretationum],
                (i32)responsum.victor);

            si (victor_valor != NIHIL
                && victor_valor->genus == SILVA_VALOR_NODUS
                && victor_valor->datum.nodus != NIHIL)
            {
                SilvaNodus* pater_servatus = nodus->pater;
                SilvaResolutioEventum* eventum;

                *nodus = *victor_valor->datum.nodus;  /* collapsus */
                nodus->pater = pater_servatus;

                eventum = (SilvaResolutioEventum*)xar_addere(
                    commissio->resolutiones);
                si (eventum != NIHIL)
                {
                    eventum->genus = SILVA_RESOLUTIO_COLLAPSUS;
                    eventum->sedes = nodus;
                    eventum->victor = responsum.victor;
                    eventum->genus_victoris = nodus->genus;
                    eventum->discriminans = responsum.discriminans;
                }
                _nodum_committere(ambulatio, nodus);  /* nunc ordinarius */
                redde;
            }
            fprintf(stderr, "silva_commissio: victor non-NODUS - "
                "involucrum manet\n");
        }
    }

    /* Superstes: registratur; interpretationes normalizatae et
     * ambulatae - internis omnium pater intra subarborem, radicibus
     * spina sola: canonica pater=ambiguum, aliae NIHIL. Canonica
     * ULTIMA ambulatur - in subarbore communi parens spinae vincit. */
    {
        SilvaNodus** slot = (SilvaNodus**)xar_addere(commissio->ambigui);

        si (slot != NIHIL)
        {
            *slot = nodus;
        }
    }

    nodus->loci[commissio->locus_interpretationum] = _listam_normalizare(
        ambulatio, nodus->loci[commissio->locus_interpretationum]);
    interps = nodus->loci[commissio->locus_interpretationum];
    canonica = nodus->loci[commissio->locus_canonicae].datum.index;

    {
        i32 numerus = silva_valor_lista_numerus(interps);
        i32 i;

        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaValor* elem;

            si (i == (i32)canonica) perge;  /* canonica ultima */
            elem = silva_valor_lista_obtinere(interps, i);
            si (elem != NIHIL)
            {
                *elem = _valorem_committere(ambulatio, *elem, NIHIL);
            }
        }
        si (canonica >= ZEPHYRUM && (i32)canonica < numerus)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(interps,
                (i32)canonica);

            si (elem != NIHIL)
            {
                *elem = _valorem_committere(ambulatio, *elem, nodus);
            }
        }
    }
}

interior SilvaValor
_valorem_committere (
    SilvaAmbulatio* ambulatio,
    SilvaValor      valor,
    SilvaNodus*     pater)
{
    commutatio (valor.genus)
    {
    casus SILVA_VALOR_NODUS:
        si (valor.datum.nodus == NIHIL)
        {
            redde valor;
        }
        valor.datum.nodus->pater = pater;
        si (valor.datum.nodus->genus
            == ambulatio->commissio->genus_ambigui)
        {
            _ambiguum_committere(ambulatio, valor.datum.nodus);
        }
        alioquin
        {
            _nodum_committere(ambulatio, valor.datum.nodus);
        }
        redde valor;

    casus SILVA_VALOR_LISTA:
    {
        i32 numerus;
        i32 i;

        valor = _listam_normalizare(ambulatio, valor);
        numerus = silva_valor_lista_numerus(valor);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            SilvaValor* elem = silva_valor_lista_obtinere(valor, i);

            si (elem != NIHIL)
            {
                *elem = _valorem_committere(ambulatio, *elem, pater);
            }
        }
        redde valor;
    }

    ordinarius:
        redde valor;  /* TOKEN / INDEX / NIHIL - intacta */
    }
}


/* ==================================================
 * API
 * ================================================== */

SilvaCommissio*
silva_committere (
    Piscina*                       piscina,
    SilvaValor                     radix,
    constans SilvaRegistrumCoctum* tabularium,
    constans SilvaOraculum*        oraculum,
    SilvaResolutor                 resolutor,
    vacuum*                        datum_resolutoris)
{
    SilvaCommissio* commissio;
    SilvaAmbulatio  ambulatio;
    s32 genus_ambigui = -I;
    s32 locus_interp = -I;
    s32 locus_canonicae = -I;
    i32 i;

    si (piscina == NIHIL || tabularium == NIHIL)
    {
        redde NIHIL;
    }

    /* Forma ambigui ex registro PER NOMEN (generator eam imposuit) */
    per (i = ZEPHYRUM; i < tabularium->numerus_generum; i++)
    {
        constans SilvaTabGenus* genus = &tabularium->genera[i];

        si (genus->titulus != NIHIL
            && strcmp(genus->titulus, "ambiguus") == ZEPHYRUM)
        {
            i32 k;

            genus_ambigui = (s32)i;
            per (k = ZEPHYRUM; k < genus->loci_numerus; k++)
            {
                constans SilvaTabLocus* locus =
                    &tabularium->loci[genus->loci_offset + k];

                si (locus->titulus == NIHIL) perge;
                si (strcmp(locus->titulus, "interpretationes")
                    == ZEPHYRUM)
                {
                    locus_interp = (s32)k;
                }
                si (strcmp(locus->titulus, "canonica") == ZEPHYRUM)
                {
                    locus_canonicae = (s32)k;
                }
            }
            frange;
        }
    }
    si (genus_ambigui < ZEPHYRUM || locus_interp < ZEPHYRUM
        || locus_canonicae < ZEPHYRUM)
    {
        fprintf(stderr,
            "silva_commissio: forma ambigui in registro abest\n");
        redde NIHIL;
    }

    commissio = (SilvaCommissio*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(SilvaCommissio));
    si (commissio == NIHIL)
    {
        redde NIHIL;
    }
    commissio->ambigui = xar_creare(piscina,
        (i32)magnitudo(SilvaNodus*));
    commissio->resolutiones = xar_creare(piscina,
        (i32)magnitudo(SilvaResolutioEventum));
    commissio->genus_ambigui = genus_ambigui;
    commissio->locus_interpretationum = (i32)locus_interp;
    commissio->locus_canonicae = (i32)locus_canonicae;

    ambulatio.piscina = piscina;
    ambulatio.commissio = commissio;
    ambulatio.oraculum = oraculum;
    ambulatio.resolutor = resolutor;
    ambulatio.datum_resolutoris = datum_resolutoris;

    commissio->radix = _valorem_committere(&ambulatio, radix, NIHIL);
    redde commissio;
}

i32
silva_recanonicare (
    SilvaCommissio*         commissio,
    constans SilvaOraculum* oraculum,
    SilvaResolutor          resolutor,
    vacuum*                 datum_resolutoris)
{
    i32 versi = ZEPHYRUM;
    i32 i;

    si (commissio == NIHIL || oraculum == NIHIL || resolutor == NIHIL)
    {
        redde ZEPHYRUM;
    }

    per (i = ZEPHYRUM; i < xar_numerus(commissio->ambigui); i++)
    {
        SilvaNodus** slot =
            (SilvaNodus**)xar_obtinere(commissio->ambigui, i);
        SilvaNodus* nodus;
        SilvaResolutioResponsum responsum;
        SilvaValor interps;
        s32 canonica;

        si (slot == NIHIL || *slot == NIHIL) perge;
        nodus = *slot;
        si (nodus->genus != commissio->genus_ambigui)
        {
            perge;  /* iam collapsus alio modo? - praetermittitur */
        }

        responsum.victor = -I;
        responsum.discriminans = NIHIL;
        resolutor(nodus, oraculum, datum_resolutoris, &responsum);
        si (responsum.victor < ZEPHYRUM)
        {
            perge;  /* adhuc ignotum */
        }

        interps = nodus->loci[commissio->locus_interpretationum];
        canonica = nodus->loci[commissio->locus_canonicae].datum.index;
        si (responsum.victor == canonica
            || (i32)responsum.victor
                >= silva_valor_lista_numerus(interps))
        {
            perge;  /* iam canonica aut extra fines */
        }

        /* Versio in loco (op sanctionata post commissionem) + spina
         * localiter retexta */
        {
            SilvaValor* vetus = silva_valor_lista_obtinere(interps,
                (i32)canonica);
            SilvaValor* novus = silva_valor_lista_obtinere(interps,
                (i32)responsum.victor);
            SilvaResolutioEventum* eventum;

            si (vetus != NIHIL && vetus->genus == SILVA_VALOR_NODUS
                && vetus->datum.nodus != NIHIL)
            {
                vetus->datum.nodus->pater = NIHIL;
            }
            si (novus != NIHIL && novus->genus == SILVA_VALOR_NODUS
                && novus->datum.nodus != NIHIL)
            {
                novus->datum.nodus->pater = nodus;
            }
            nodus->loci[commissio->locus_canonicae] =
                silva_valor_index(responsum.victor);

            eventum = (SilvaResolutioEventum*)xar_addere(
                commissio->resolutiones);
            si (eventum != NIHIL)
            {
                eventum->genus = SILVA_RESOLUTIO_RECANONICATA;
                eventum->sedes = nodus;
                eventum->victor = responsum.victor;
                eventum->genus_victoris =
                    (novus != NIHIL && novus->genus == SILVA_VALOR_NODUS
                     && novus->datum.nodus != NIHIL)
                    ? novus->datum.nodus->genus : -I;
                eventum->discriminans = responsum.discriminans;
            }
            versi++;
        }
    }
    redde versi;
}
