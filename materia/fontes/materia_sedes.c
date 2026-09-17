/* materia_sedes.c - Vide materia_sedes.h. */

#include "materia_sedes.h"
#include "materia_nodus.h"
#include "stml.h"
#include "tabula_dispersa.h"
#include "xar.h"
#include <string.h>

nomen structura {
     constans MateriaToken* lexema;   /* NIHIL = nodus */
            MateriaTractus  tractus;
} Exspectatum;

nomen structura {
                       Piscina* piscina;
    constans MateriaOrigoUncus* uncus;
                           Xar* exspectata;   /* Exspectatum */
                           Xar* lineae;       /* s32: initia linearum */
                TabulaDispersa* visa;         /* lexemata iam scripta */
                           b32  memoria_defecit;
} Collatio;

interior b32
_recusare (
    MateriaSedesRelatio* relatio,
     constans character* causa,
                    i32  index)
{
    si (relatio->causa == NIHIL)
    {
        relatio->causa = causa;
        relatio->index = index;
    }
    relatio->sana = FALSUM;
    redde FALSUM;
}


/* ==================================================
 * Latus fontis: tabula linearum
 * ================================================== */

interior Xar*
_lineas_legere (
               Piscina* piscina,
    constans character* fons,
                   i32  mensura)
{
    Xar* lineae = xar_creare(piscina, (i32)magnitudo(s32));
    s32* cella;
    i32  i;

    si (lineae == NIHIL)
    {
        redde NIHIL;
    }
    cella = (s32*)xar_addere(lineae);
    si (cella == NIHIL)
    {
        redde NIHIL;
    }
    *cella = ZEPHYRUM;
    per (i = ZEPHYRUM; i < mensura; i++)
    {
        si (fons[i] == '\n')
        {
            cella = (s32*)xar_addere(lineae);
            si (cella == NIHIL)
            {
                redde NIHIL;
            }
            *cella = (s32)(i + I);
        }
    }
    redde lineae;
}

/* linea et columna ex octeto per tabulam linearum (quaesitio bina) */
interior vacuum
_sedem_computare (
    Xar* lineae,
    s32  octetus,
    i32* linea,
    i32* columna)
{
    i32 infima  = ZEPHYRUM;
    i32 summa   = xar_numerus(lineae) - I;

    dum (infima < summa)
    {
        i32 media = (infima + summa + I) / II;

        si (*(s32*)xar_obtinere(lineae, media) <= octetus)
        {
            infima = media;
        }
        alioquin
        {
            summa = media - I;
        }
    }
    *linea = infima + I;
    *columna = (i32)(octetus - *(s32*)xar_obtinere(lineae, infima))
        + I;
}


/* ==================================================
 * Latus arboris: tractus exspectati ordine scriptoris
 * ================================================== */

/* VERUM si lexema iam visum (usus iteratus); aliter notatur */
interior b32
_visum_notare (
                 Collatio* v,
    constans MateriaToken* lexema)
{
    chorda  clavis;
        i8* octeti;

    octeti = (i8*)piscina_allocare(v->piscina,
        magnitudo(constans MateriaToken*));
    si (octeti == NIHIL)
    {
        v->memoria_defecit = VERUM;
        redde VERUM;
    }
    memcpy(octeti, &lexema, magnitudo(constans MateriaToken*));
    clavis.datum    = octeti;
    clavis.mensura  = (i32)magnitudo(constans MateriaToken*);
    si (tabula_dispersa_continet(v->visa, clavis))
    {
        redde VERUM;
    }
    (vacuum)tabula_dispersa_inserere(v->visa, clavis, (vacuum*)VERUM);
    redde FALSUM;
}

/* tractus exspectatus lexematis: initium ex sede efficaci, lineae et
 * columnae ex TABULA (fontis) aut portatae (derivati) */
interior MateriaTractus
_tractus_lexematis (
                 Collatio* v,
    constans MateriaToken* lexema)
{
    MateriaTractus t;

    materia_tractus_lexematis(v->uncus, lexema, &t);
    si (t.initium >= ZEPHYRUM && t.est_fons)
    {
        t.finis = t.initium + (s32)lexema->valor.mensura;
        _sedem_computare(v->lineae, t.initium, &t.linea, &t.columna);
        _sedem_computare(v->lineae, t.finis, &t.linea_finis,
            &t.columna_finis);
    }
    redde t;
}

interior vacuum
_exspectatum_addere (
                   Collatio* v,
      constans MateriaToken* lexema,
    constans MateriaTractus* t)
{
    Exspectatum* e;

    si (t->initium < ZEPHYRUM)
    {
        redde;
    }
    e = (Exspectatum*)xar_addere(v->exspectata);
    si (e == NIHIL)
    {
        v->memoria_defecit = VERUM;
        redde;
    }
    e->lexema   = lexema;
    e->tractus  = *t;
}

/* summa nodi: minimum initii, maximum finis, fons primus solus -
 * scripta hic SEORSUM a materia_tractus_conferre (oraculum
 * disiunctum) */
interior vacuum
_summam_augere (
             MateriaTractus* summa,
                        b32* inventum,
    constans MateriaTractus* t)
{
    si (t->initium < ZEPHYRUM)
    {
        redde;
    }
    si (!*inventum)
    {
        *summa     = *t;
        *inventum  = VERUM;
        redde;
    }
    si (t->fons_index != summa->fons_index)
    {
        redde;
    }
    si (t->initium < summa->initium)
    {
        summa->initium  = t->initium;
        summa->linea    = t->linea;
        summa->columna  = t->columna;
    }
    si (t->finis > summa->finis)
    {
        summa->finis          = t->finis;
        summa->linea_finis    = t->linea_finis;
        summa->columna_finis  = t->columna_finis;
    }
}

interior vacuum
_valorem_ambulare (
                  Collatio* v,
     constans MateriaValor* valor,
            MateriaTractus* summa,
                       b32* inventum);

interior vacuum
_nodum_ambulare (
                  Collatio* v,
     constans MateriaNodus* nodus,
            MateriaTractus* summa_parentis,
                       b32* inventum_parentis)
{
    MateriaTractus summa;
               b32 inventum = FALSUM;
               i32 locus_exspectati;
               i32 k;

    si (nodus == NIHIL)
    {
        redde;
    }
    /* locus in serie ANTE liberos (ordo documenti), postea impletur */
    locus_exspectati = xar_numerus(v->exspectata);
    si (xar_addere(v->exspectata) == NIHIL)
    {
        v->memoria_defecit = VERUM;
        redde;
    }
    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        _valorem_ambulare(v, &nodus->loci[k], &summa, &inventum);
    }
    {
        Exspectatum* e = (Exspectatum*)xar_obtinere(v->exspectata,
            locus_exspectati);

        e->lexema = NIHIL;
        si (inventum)
        {
            e->tractus = summa;
            _summam_augere(summa_parentis, inventum_parentis, &summa);
        }
        alioquin
        {
            memset(&e->tractus, ZEPHYRUM, magnitudo(e->tractus));
            e->tractus.initium = (s32)-I;   /* nodus sine sede */
        }
    }
}

interior vacuum
_lexema_ambulare (
                  Collatio* v,
     constans MateriaToken* lexema,
            MateriaTractus* summa,
                       b32* inventum)
{
    MateriaTractus t;
               i32 k;

    si (lexema == NIHIL)
    {
        redde;
    }
    t = _tractus_lexematis(v, lexema);
    _summam_augere(summa, inventum, &t);
    si (_visum_notare(v, lexema))
    {
        redde;   /* usus iteratus: transclusio, elementum nullum */
    }
    _exspectatum_addere(v, lexema, &t);
    per (k = ZEPHYRUM; k < lexema->numerus_ante; k++)
    {
        MateriaTractus a = _tractus_lexematis(v,
            lexema->spatia_ante[k]);

        _exspectatum_addere(v, lexema->spatia_ante[k], &a);
    }
    per (k = ZEPHYRUM; k < lexema->numerus_post; k++)
    {
        MateriaTractus p = _tractus_lexematis(v,
            lexema->spatia_post[k]);

        _exspectatum_addere(v, lexema->spatia_post[k], &p);
    }
}

interior vacuum
_valorem_ambulare (
                  Collatio* v,
     constans MateriaValor* valor,
            MateriaTractus* summa,
                       b32* inventum)
{
    i32 k;

    commutatio (valor->genus)
    {
    casus MATERIA_VALOR_TOKEN:
        _lexema_ambulare(v, valor->datum.token, summa, inventum);
        frange;
    casus MATERIA_VALOR_NODUS:
        _nodum_ambulare(v, valor->datum.nodus, summa, inventum);
        frange;
    casus MATERIA_VALOR_LISTA:
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor); k++)
        {
            constans MateriaValor* e =
                materia_valor_lista_obtinere(*valor, k);

            si (e != NIHIL)
            {
                _valorem_ambulare(v, e, summa, inventum);
            }
        }
        frange;
    ordinarius:
        frange;
    }
}


/* ==================================================
 * Latus documenti
 * ================================================== */

/* elementa sedibus ornata ordine documenti (praeordine) */
interior vacuum
_elementa_colligere (
          Xar* acta,
    StmlNodus* nodus)
{
    i32 k;

    si (nodus == NIHIL || nodus->genus != STML_NODUS_ELEMENTUM)
    {
        redde;
    }
    si (stml_attributum_capere(nodus, "octeti") != NIHIL)
    {
        StmlNodus** cella = (StmlNodus**)xar_addere(acta);

        si (cella != NIHIL)
        {
            *cella = nodus;
        }
    }
    si (nodus->liberi == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < xar_numerus(nodus->liberi); k++)
    {
        _elementa_colligere(acta,
            *(StmlNodus**)xar_obtinere(nodus->liberi, k));
    }
}

/* numeri decimales ex chorda (separator quilibet non cifra);
 * reddit numerum lectorum, ad capacitatem */
interior i32
_numeros_legere (
    constans chorda* valor,
                i32* numeri,
                i32  capacitas)
{
    i32 lecti  = ZEPHYRUM;
    i32 i      = ZEPHYRUM;

    si (valor == NIHIL)
    {
        redde ZEPHYRUM;
    }
    dum (i < valor->mensura && lecti < capacitas)
    {
        si (valor->datum[i] >= (i8)'0' && valor->datum[i] <= (i8)'9')
        {
            i32 n = ZEPHYRUM;

            dum (   i < valor->mensura
                 && valor->datum[i] >= (i8)'0'
                 && valor->datum[i] <= (i8)'9')
            {
                n = n * X + (i32)(valor->datum[i] - (i8)'0');
                i++;
            }
            numeri[lecti] = n;
            lecti++;
        }
        alioquin
        {
            i++;
        }
    }
    redde lecti;
}

interior b32
_chordae_pares (
    constans chorda* a,
    constans chorda* b)
{
    si (a == NIHIL || b == NIHIL)
    {
        redde (b32)(a == b);
    }
    redde chorda_aequalis(*a, *b);
}

/* attributum visionis: sedes/octeti ubique, 'visio' in radice */
interior b32
_attributum_visionis (
    constans chorda* titulus,
                b32  radix)
{
    redde (b32)(   chorda_aequalis_literis(*titulus, "sedes")
                || chorda_aequalis_literis(*titulus, "octeti")
                || (radix && chorda_aequalis_literis(*titulus,
                                 "visio")));
}

/* index attributi proximi quod visionis non est; numerus si nullum */
interior i32
_attributum_proximum (
    Xar* attributa,
    i32  ab,
    b32  radix)
{
    si (attributa == NIHIL)
    {
        redde ZEPHYRUM;
    }
    dum (ab < xar_numerus(attributa))
    {
        StmlAttributum* a = (StmlAttributum*)xar_obtinere(attributa,
            ab);

        si (   a != NIHIL && a->titulus != NIHIL
            && !_attributum_visionis(a->titulus, radix))
        {
            redde ab;
        }
        ab++;
    }
    redde xar_numerus(attributa);
}

/* proiectio nuda et visio aequales praeter attributa visionis */
interior b32
_arbores_aequales (
    StmlNodus* nuda,
    StmlNodus* visio,
          b32  radix)
{
    i32 ordo_nudae;
    i32 ordo_visionis;
    i32 numerus_nudae;
    i32 numerus_visionis;
    i32 k;

    si (nuda == NIHIL || visio == NIHIL)
    {
        redde (b32)(nuda == visio);
    }
    si (   nuda->genus      != visio->genus
        || !_chordae_pares(nuda->titulus, visio->titulus)
        || !_chordae_pares(nuda->valor, visio->valor)
        || nuda->crudus     != visio->crudus
        || nuda->fragmentum != visio->fragmentum
        || !_chordae_pares(nuda->fragmentum_id, visio->fragmentum_id))
    {
        redde FALSUM;
    }
    numerus_nudae = nuda->attributa
        != NIHIL ? xar_numerus(nuda->attributa)
                                   : ZEPHYRUM;
    numerus_visionis = visio->attributa
        != NIHIL ? xar_numerus(visio->attributa)
                                   : ZEPHYRUM;
    ordo_nudae = _attributum_proximum(nuda->attributa, ZEPHYRUM, radix);
    ordo_visionis = _attributum_proximum(visio->attributa, ZEPHYRUM,
        radix);
    dum (ordo_nudae < numerus_nudae && ordo_visionis < numerus_visionis)
    {
        StmlAttributum* a = (StmlAttributum*)xar_obtinere(
            nuda->attributa, ordo_nudae);
        StmlAttributum* b = (StmlAttributum*)xar_obtinere(
            visio->attributa, ordo_visionis);

        si (   !_chordae_pares(a->titulus, b->titulus)
            || !_chordae_pares(a->valor, b->valor))
        {
            redde FALSUM;
        }
        ordo_nudae = _attributum_proximum(nuda->attributa, ordo_nudae
            + I, radix);
        ordo_visionis = _attributum_proximum(visio->attributa,
            ordo_visionis + I, radix);
    }
    si (ordo_nudae < numerus_nudae || ordo_visionis < numerus_visionis)
    {
        redde FALSUM;
    }
    numerus_nudae = nuda->liberi
        != NIHIL ? xar_numerus(nuda->liberi)  : ZEPHYRUM;
    numerus_visionis = visio->liberi
        != NIHIL ? xar_numerus(visio->liberi) : ZEPHYRUM;
    si (numerus_nudae != numerus_visionis)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < numerus_nudae; k++)
    {
        si (!_arbores_aequales(
                *(StmlNodus**)xar_obtinere(nuda->liberi, k),
                *(StmlNodus**)xar_obtinere(visio->liberi, k), FALSUM))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Ingressus
 * ================================================== */

b32
materia_sedes_verificare (
                           Piscina* piscina,
             constans MateriaNodus* radix,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura,
               MateriaSedesRelatio* relatio)
{
    MateriaArborConsilium  nudum;
    MateriaArborConsilium  ornatum;
    MateriaArborScriptura  scriptura_nuda;
    MateriaArborScriptura  scriptura_ornata;
                 Collatio  v;
           MateriaTractus  summa;
                      b32  inventum = FALSUM;
                      Xar* acta;
       MateriaArborVitium  vitium;
                      i32  j = ZEPHYRUM;
                      i32  k;

    memset(relatio, ZEPHYRUM, magnitudo(*relatio));
    relatio->sana = VERUM;
    si (   piscina == NIHIL || radix == NIHIL || consilium == NIHIL
        || fons    == NIHIL)
    {
        redde _recusare(relatio, "argumenta nihil", ZEPHYRUM);
    }
    si (consilium->loci_admissi != NIHIL)
    {
        redde _recusare(relatio,
            "visio partialis: verificatio sedium non tractat",
            ZEPHYRUM);
    }
    si (consilium->templa_activa)
    {
        redde _recusare(relatio,
            "templa activa: spatia sine elemento", ZEPHYRUM);
    }
    si (   consilium->frons                 != NIHIL
        && consilium->frons->liberos_ornare != NIHIL)
    {
        redde _recusare(relatio,
            "frons liberos ornat: ordo elementorum ignotus", ZEPHYRUM);
    }

    /* (1) visio == nuda praeter attributa visionis */
    nudum                   = *consilium;
    nudum.sedes_scribere    = FALSUM;
    ornatum                 = *consilium;
    ornatum.sedes_scribere  = VERUM;
    scriptura_nuda = materia_arbor_proicere_nodum(piscina, radix,
        &nudum);
    scriptura_ornata = materia_arbor_proicere_nodum(piscina, radix,
        &ornatum);
    si (!scriptura_nuda.successus || !scriptura_ornata.successus)
    {
        redde _recusare(relatio, "proiectio fracta", ZEPHYRUM);
    }
    si (!_arbores_aequales(scriptura_nuda.arbor, scriptura_ornata.arbor,
        VERUM))
    {
        redde _recusare(relatio, "visio a proiectione nuda differt",
            ZEPHYRUM);
    }

    /* (2) tractus exspectati contra elementa, ordine */
    memset(&v, ZEPHYRUM, magnitudo(v));
    v.piscina     = piscina;
    v.uncus       = consilium->origo;
    v.exspectata  = xar_creare(piscina, (i32)magnitudo(Exspectatum));
    v.lineae      = _lineas_legere(piscina, fons, mensura);
    v.visa        = tabula_dispersa_creare_chorda(piscina, (i32)MXXIV);
    acta          = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    si (   v.exspectata == NIHIL || v.lineae == NIHIL || v.visa == NIHIL
        || acta         == NIHIL)
    {
        redde _recusare(relatio, "memoria deficit", ZEPHYRUM);
    }
    _nodum_ambulare(&v, radix, &summa, &inventum);
    _elementa_colligere(acta, scriptura_ornata.arbor);
    si (v.memoria_defecit)
    {
        redde _recusare(relatio, "memoria deficit", ZEPHYRUM);
    }
    per (k = ZEPHYRUM; k < xar_numerus(v.exspectata); k++)
    {
        constans Exspectatum* e =
            (constans Exspectatum*)xar_obtinere(v.exspectata, k);
                    StmlNodus* elementum;
                          i32  numeri[IV];

        si (e->tractus.initium < ZEPHYRUM)
        {
            /* nodus aut lexema sine sede: elementum sine octetis */
            perge;
        }
        si (j >= xar_numerus(acta))
        {
            redde _recusare(relatio, "elementa pauciora exspectatis",
                j);
        }
        elementum = *(StmlNodus**)xar_obtinere(acta, j);
        relatio->elementa++;
        si (   _numeros_legere(stml_attributum_capere(elementum,
                   "octeti"), numeri, (i32)II) != II
            || (s32)numeri[0] != e->tractus.initium
            || (s32)numeri[I] != e->tractus.finis)
        {
            redde _recusare(relatio, "octeti dispares", j);
        }
        si (   _numeros_legere(stml_attributum_capere(elementum,
                   "sedes"), numeri, (i32)IV) != IV
            || numeri[0]   != e->tractus.linea
            || numeri[I]   != e->tractus.columna
            || numeri[II]  != e->tractus.linea_finis
            || numeri[III] != e->tractus.columna_finis)
        {
            redde _recusare(relatio, "sedes dispares", j);
        }
        si (e->lexema == NIHIL)
        {
            relatio->nodi++;
        }
        alioquin si (!e->tractus.est_fons)
        {
            relatio->derivata++;
        }
        alioquin
        {
            si (   e->tractus.finis > (s32)mensura
                || (   e->lexema->valor.mensura > ZEPHYRUM
                    && memcmp(fons + e->tractus.initium,
                           e->lexema->valor.datum,
                           (size_t)e->lexema->valor.mensura)
                       != ZEPHYRUM))
            {
                redde _recusare(relatio,
                    "segmentum fontis valori dispar", j);
            }
            relatio->lexemata++;
        }
        j++;
    }
    si (j != xar_numerus(acta))
    {
        redde _recusare(relatio, "elementa plura exspectatis", j);
    }

    /* (3) lector visionem recusat */
    {
        chorda textus = stml_scribere(scriptura_ornata.arbor, piscina,
            VERUM);

        si (   materia_arbor_legere(piscina, NIHIL, textus, consilium,
                   &vitium) != NIHIL
            || vitium.causa                              == NIHIL
            || strncmp(vitium.causa, "visio", (size_t)V) != ZEPHYRUM)
        {
            redde _recusare(relatio, "lector visionem non recusavit",
                ZEPHYRUM);
        }
    }
    redde relatio->sana;
}
