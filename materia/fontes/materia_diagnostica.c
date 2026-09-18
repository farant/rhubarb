/* materia_diagnostica.c - Vide materia_diagnostica.h.
 *
 * Ambulatio UNA, ITERATIVA (acervus explicitus, numquam recursio) et
 * LINEARIS: tractus nodi cuiusque ab imo cumulatur ut in scriptore
 * (A1), non per materia_tractus_nodi in quoque nodo - illud
 * subarborem suam iterum ambulat, ergo catena profunda quadratica
 * fit (XL milia: XXXI s, porta fuzz per moram occisa; quaestio
 * 01M2R9MKFQ). Nodus notatur cum subarbor eius PERACTA est; ordo
 * idem manet quia ordines per (initium, codex) ordinantur et nodus
 * sine tractu lexema idem prius habet (subarbor eius lexema nullum
 * fert). Loci ordine, listae ordine, REFERENTIA non descensa.
 * Lexema quodque ordine OCTETORUM visitatur (trivia ante, lexema,
 * trivia post) - idem ordo quo scriptor octetos emittit, quem
 * probatio ordinis poscit.
 */

#include "materia_diagnostica.h"
#include "materia_annotationes.h"
#include "materia_arbor.h"
#include "materia_exemplaria.h"
#include "materia_excusatio.h"
#include "stml_macros.h"
#include "tabula_dispersa.h"
#include <stdio.h>
#include <string.h>

/* valor praesentiae in tabula lexematum visorum: sedes sola refert
 * (VERUM in punctatorem versum monitum daret) */
hic_manens i32 PRAESENS = I;

nomen structura {
                             Piscina* piscina;
     constans MateriaRegistrumCoctum* genera;
    constans MateriaDiagnosticaCocta* diagnostica;
          constans MateriaOrigoUncus* uncus;
                                 Xar* exitus;
                      TabulaDispersa* visa;
                                 b32  fons_notus;
                                 s32  fons;
                                 s32  finis_maximus;
                      MateriaTractus  ultimum;
                                 b32  ultimum_notum;
    /* cumulatio tractus nodi currentis ab imo (ut scriptor, A1):
     * materia_tractus_nodi in quoque nodo quadraticum esset */
                      MateriaTractus tractus;
                                 b32 tractus_inventus;
                                 b32 memoria_defecit;
} Derivatio;

/* Reddit cellam scriptam (NIHIL si memoria defecit), ut vocans sedes
 * relatas addere possit sine cella iterum quaesita. */
interior MateriaDiagnosticum*
_addere (
                 Derivatio* d,
                       s32  gravitas,
        constans character* codex,
        constans character* causa,
     constans MateriaNodus* nodus,
     constans MateriaToken* lexema,
   constans MateriaTractus* tractus)
{
    MateriaDiagnosticum* r = (MateriaDiagnosticum*)xar_addere(
        d->exitus);

    si (r == NIHIL)
    {
        d->memoria_defecit = VERUM;
        redde NIHIL;
    }
    r->gravitas           = gravitas;
    r->codex              = codex;
    r->causa              = causa;
    r->nodus              = nodus;
    r->lexema             = lexema;
    r->tractus            = *tractus;
    r->nota               = NIHIL;
    r->relata             = NIHIL;
    r->numerus_relatorum  = ZEPHYRUM;
    redde r;
}

/* ABSENTIA sola sedem alteram GRATIS habet: tractus nodi ipsius
 * aperturam tegit, et ambulator eum iam computatum tenet (punctum ex
 * eo sumitur). Nulla declaratio nova: quod adest datur. 'apertura'
 * NIHIL = nullum lexema inventum, ergo sedes una manet.
 *
 * GENUS et VACUA nihil accipiunt: neutrum sedem alteram in manu
 * habet, et eam fingere programma esset, non tabula. */
interior vacuum
_absentiam_addere (
                          Derivatio* d,
    constans MateriaTabDiagnosticum* r,
              constans MateriaNodus* nodus,
            constans MateriaTractus* punctum,
            constans MateriaTractus* apertura)
{
    MateriaDiagnosticum* cella = _addere(d, r->gravitas, r->codex,
        r->causa, nodus, NIHIL, punctum);
     MateriaSedesRelata* sedes;

    si (cella == NIHIL)
    {
        redde;
    }
    cella->nota = MATERIA_NOTA_EXSPECTATUR;
    si (apertura == NIHIL)
    {
        redde;
    }
    sedes = (MateriaSedesRelata*)piscina_allocare(d->piscina,
        magnitudo(MateriaSedesRelata));
    si (sedes == NIHIL)
    {
        d->memoria_defecit = VERUM;
        redde;
    }
    sedes->tractus            = *apertura;
    sedes->nota               = MATERIA_NOTA_COEPIT;
    cella->relata             = sedes;
    cella->numerus_relatorum  = (i32)I;
}

/* punctum in fine tractus (latitudo nulla: '}' exspectata HIC) */
interior MateriaTractus
_punctum_finis (
    constans MateriaTractus* t)
{
    MateriaTractus p = *t;

    p.initium  = t->finis;
    p.linea    = t->linea_finis;
    p.columna  = t->columna_finis;
    redde p;
}

/* punctum post lexema fontis ultimum visum; initium plagulae si
 * nullum (nodus ante omnia) */
interior MateriaTractus
_punctum_prius (
    constans Derivatio* d)
{
    MateriaTractus p;

    si (d->ultimum_notum)
    {
        redde _punctum_finis(&d->ultimum);
    }
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.linea          = I;
    p.columna        = I;
    p.linea_finis    = I;
    p.columna_finis  = I;
    p.est_fons       = VERUM;
    redde p;
}

interior b32
_inane (
    constans Derivatio* d,
                   s32  genus)
{
    i32 k;

    per (k = ZEPHYRUM; k < d->diagnostica->numerus_inanium; k++)
    {
        si (d->diagnostica->inania[k] == genus)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* lista 'vacua': non scripta, aut elementa nodorum generum inanium
 * sola (separator, heredoc - delta D5) */
interior b32
_vacua (
       constans Derivatio* d,
    constans MateriaValor* valor)
{
    i32 k;

    si (valor->genus == MATERIA_VALOR_NIHIL)
    {
        redde VERUM;
    }
    si (valor->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*valor); k++)
    {
        constans MateriaValor* e = materia_valor_lista_obtinere(
            *valor, k);

        si (e == NIHIL)
        {
            perge;
        }
        si (e->genus == MATERIA_VALOR_TOKEN)
        {
            redde FALSUM;
        }
        si (   e->genus       == MATERIA_VALOR_NODUS
            && e->datum.nodus != NIHIL
            && !_inane(d, e->datum.nodus->genus))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* VERUM si lexema iam visum (usus iteratus: transclusio); aliter
 * notatur. Clavis = octeti punctatoris ipsius. */
interior b32
_visum (
                Derivatio* d,
    constans MateriaToken* lexema)
{
    chorda  clavis;
        i8* octeti;

    octeti = (i8*)piscina_allocare(d->piscina,
        magnitudo(constans MateriaToken*));
    si (octeti == NIHIL)
    {
        d->memoria_defecit = VERUM;
        redde VERUM;
    }
    memcpy(octeti, &lexema, magnitudo(constans MateriaToken*));
    clavis.datum    = octeti;
    clavis.mensura  = (i32)magnitudo(constans MateriaToken*);
    si (tabula_dispersa_continet(d->visa, clavis))
    {
        redde VERUM;
    }
    si (!tabula_dispersa_inserere(d->visa, clavis, &PRAESENS))
    {
        d->memoria_defecit = VERUM;
    }
    redde FALSUM;
}

/* ordo octetorum: lexema fontis ante finem maximum iam visum =
 * octeti transpositi (heredoc crustae olim) */
interior vacuum
_lexema_notare (
                Derivatio* d,
    constans MateriaToken* lexema)
{
    MateriaTractus t;

    si (lexema == NIHIL)
    {
        redde;
    }
    materia_tractus_lexematis(d->uncus, lexema, &t);
    si (t.initium < ZEPHYRUM || !t.est_fons)
    {
        redde;
    }
    si (   d->fons_notus && t.fons_index == d->fons
        && t.initium < d->finis_maximus)
    {
        _addere(d, (s32)MATERIA_GRAVITAS_ERRATUM, MATERIA_CODEX_ORDO,
            "lexema ante finem prioris", NIHIL, lexema, &t);
    }
    si (!d->fons_notus)
    {
        d->fons_notus  = VERUM;
        d->fons        = t.fons_index;
    }
    si (t.fons_index == d->fons && t.finis > d->finis_maximus)
    {
        d->finis_maximus = t.finis;
    }
    d->ultimum        = t;
    d->ultimum_notum  = VERUM;
}

interior vacuum
_lexema_ambulare (
                Derivatio* d,
    constans MateriaToken* lexema)
{
    MateriaTractus t;
               i32 k;

    si (lexema == NIHIL)
    {
        redde;
    }
    /* tractus nodi possidentis: lexema locorum quodque confert,
     * etiam usus iteratus (materia_tractus_nodi idem facit) -
     * trivia numquam */
    materia_tractus_lexematis(d->uncus, lexema, &t);
    materia_tractus_conferre(&d->tractus, &d->tractus_inventus, &t);
    si (_visum(d, lexema))
    {
        redde;
    }
    per (k = ZEPHYRUM; k < lexema->numerus_ante; k++)
    {
        _lexema_notare(d, lexema->spatia_ante[k]);
    }
    _lexema_notare(d, lexema);
    per (k = ZEPHYRUM; k < lexema->numerus_post; k++)
    {
        _lexema_notare(d, lexema->spatia_post[k]);
    }
}

/* nodus notatur (ordines eius), liberi NON descenduntur - descensum
 * acervus agit */
interior vacuum
_nodum_notare (
                  Derivatio* d,
      constans MateriaNodus* nodus,
                        b32  habet,
    constans MateriaTractus* cumulatus)
{
    MateriaTractus t;
    MateriaTractus punctum;
               i32 k;

    si (nodus == NIHIL)
    {
        redde;
    }
    t        = *cumulatus;
    punctum  = _punctum_prius(d);
    per (k = ZEPHYRUM; k < d->diagnostica->numerus; k++)
    {
        constans MateriaTabDiagnosticum* r =
            &d->diagnostica->tabula[k];
               constans MateriaValor* valor;
                      MateriaTractus  finis;
             constans MateriaTractus* apertura;

        si (   r->genus != nodus->genus
            || (   r->species != (s32)MATERIA_DIAGNOSTICUM_GENUS
                && (   r->locus < ZEPHYRUM
                    || (i32)r->locus >= nodus->numerus_locorum)))
        {
            perge;
        }
        commutatio (r->species)
        {
        casus MATERIA_DIAGNOSTICUM_GENUS:
            _addere(d, r->gravitas, r->codex, r->causa, nodus, NIHIL,
                habet ? &t : &punctum);
            frange;
        casus MATERIA_DIAGNOSTICUM_ABSENTIA:
            valor = &nodus->loci[r->locus];
            si (   valor->genus == MATERIA_VALOR_NIHIL
                || (   valor->genus == MATERIA_VALOR_REFERENTIA
                    && valor->datum.nodus == NIHIL))
            {
                finis     = habet ? _punctum_finis(&t) : punctum;
                apertura  = habet ? &t : NIHIL;
                _absentiam_addere(d, r, nodus, &finis, apertura);
            }
            frange;
        casus MATERIA_DIAGNOSTICUM_VACUA:
            si (_vacua(d, &nodus->loci[r->locus]))
            {
                _addere(d, r->gravitas, r->codex, r->causa, nodus,
                    NIHIL, habet ? &t : &punctum);
            }
            frange;
        ordinarius:
            frange;
        }
    }
}

/* opus acervi: valor ambulandus, aut EXITUS nodi (valor NIHIL) cum
 * cumulo parentis servato */
nomen structura {
    constans MateriaValor* valor;
    constans MateriaNodus* nodus;
           MateriaTractus  tractus_parentis;
                      b32  inventus_parentis;
} Opus;

interior b32
_opus_premere (
        Derivatio* d,
              Xar* acervus,
    constans Opus* opus)
{
    Opus* cella = (Opus*)xar_addere(acervus);

    si (cella == NIHIL)
    {
        d->memoria_defecit = VERUM;
        redde FALSUM;
    }
    *cella = *opus;
    redde VERUM;
}

interior b32
_valorem_premere (
                 Derivatio* d,
                       Xar* acervus,
     constans MateriaValor* valor)
{
    Opus opus;

    memset(&opus, ZEPHYRUM, magnitudo(opus));
    opus.valor = valor;
    redde _opus_premere(d, acervus, &opus);
}

/* nodum intrare: exitus eius premitur (cumulus parentis servatus),
 * deinde loci eius; cumulus novus vacuus incipit */
interior b32
_nodum_intrare (
                 Derivatio* d,
                       Xar* acervus,
     constans MateriaNodus* nodus)
{
    Opus opus;

    memset(&opus, ZEPHYRUM, magnitudo(opus));
    opus.nodus              = nodus;
    opus.tractus_parentis   = d->tractus;
    opus.inventus_parentis  = d->tractus_inventus;
    si (!_opus_premere(d, acervus, &opus))
    {
        redde FALSUM;
    }
    d->tractus_inventus = FALSUM;
    redde VERUM;
}

/* loci nodi in acervum ordine INVERSO (summus primus eruitur, ergo
 * ordo locorum servatur) */
interior b32
_locos_premere (
                 Derivatio* d,
                       Xar* acervus,
     constans MateriaNodus* nodus)
{
    i32 k;

    per (k = nodus->numerus_locorum; k > ZEPHYRUM; k--)
    {
        si (!_valorem_premere(d, acervus, &nodus->loci[k - I]))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior vacuum
_ambulare (
                 Derivatio* d,
     constans MateriaNodus* radix)
{
    Xar* acervus;

    si (radix == NIHIL)
    {
        redde;
    }
    acervus = xar_creare(d->piscina, (i32)magnitudo(Opus));
    si (acervus == NIHIL)
    {
        d->memoria_defecit = VERUM;
        redde;
    }
    si (   !_nodum_intrare(d, acervus, radix)
        || !_locos_premere(d, acervus, radix))
    {
        redde;
    }
    dum (xar_numerus(acervus) > ZEPHYRUM)
    {
        Opus opus;
         i32 ultimus = xar_numerus(acervus) - I;
         i32 k;

        opus = *(Opus*)xar_obtinere(acervus, ultimus);
        xar_truncare(acervus, ultimus);
        si (opus.valor == NIHIL)
        {
            /* exitus nodi: subarbor peracta, tractus cumulatus */
            MateriaTractus cumulatus  = d->tractus;
                       b32 habet      = d->tractus_inventus;

            _nodum_notare(d, opus.nodus, habet, &cumulatus);
            d->tractus           = opus.tractus_parentis;
            d->tractus_inventus  = opus.inventus_parentis;
            si (habet)
            {
                materia_tractus_conferre(&d->tractus,
                    &d->tractus_inventus, &cumulatus);
            }
            perge;
        }
        commutatio (opus.valor->genus)
        {
        casus MATERIA_VALOR_TOKEN:
            _lexema_ambulare(d, opus.valor->datum.token);
            frange;
        casus MATERIA_VALOR_NODUS:
            si (opus.valor->datum.nodus != NIHIL)
            {
                si (   !_nodum_intrare(d, acervus,
                           opus.valor->datum.nodus)
                    || !_locos_premere(d, acervus,
                           opus.valor->datum.nodus))
                {
                    redde;
                }
            }
            frange;
        casus MATERIA_VALOR_LISTA:
            per (k = materia_valor_lista_numerus(*opus.valor);
                 k > ZEPHYRUM; k--)
            {
                constans MateriaValor* elementum =
                    materia_valor_lista_obtinere(*opus.valor, k - I);

                si (   elementum != NIHIL
                    && !_valorem_premere(d, acervus, elementum))
                {
                    redde;
                }
            }
            frange;
        ordinarius:
            frange;
        }
    }
}

/* ordo: initium, deinde codex; insertio stabilis (ordo inventionis
 * paribus servatur) */
interior b32
_prius (
    constans MateriaDiagnosticum* a,
    constans MateriaDiagnosticum* b)
{
    si (a->tractus.initium != b->tractus.initium)
    {
        redde (b32)(a->tractus.initium < b->tractus.initium);
    }
    redde (b32)(strcmp(a->codex, b->codex) < ZEPHYRUM);
}

Xar*
materia_diagnostica_derivare (
                            Piscina* piscina,
              constans MateriaNodus* radix,
    constans MateriaRegistrumCoctum* genera,
   constans MateriaDiagnosticaCocta* diagnostica,
         constans MateriaOrigoUncus* uncus,
                                Xar* emissa)
{
               Derivatio  d;
     MateriaDiagnosticum* series;
                     Xar* ordinata;
                     i32  numerus;
                     i32  k;

    si (piscina == NIHIL || genera == NIHIL || diagnostica == NIHIL)
    {
        redde NIHIL;
    }
    memset(&d, ZEPHYRUM, magnitudo(d));
    d.piscina      = piscina;
    d.genera       = genera;
    d.diagnostica  = diagnostica;
    d.uncus        = uncus;
    d.exitus       = xar_creare(piscina,
        (i32)magnitudo(MateriaDiagnosticum));
    d.visa         = tabula_dispersa_creare_chorda(piscina, (i32)MXXIV);
    si (d.exitus == NIHIL || d.visa == NIHIL)
    {
        redde NIHIL;
    }
    _ambulare(&d, radix);
    per (k = ZEPHYRUM; emissa != NIHIL && k < xar_numerus(emissa); k++)
    {
        constans MateriaDiagnosticum* e =
            (constans MateriaDiagnosticum*)xar_obtinere(emissa, k);
                       MateriaTractus  t;
                  MateriaDiagnosticum* cella;

        si (e == NIHIL)
        {
            perge;
        }
        t = e->tractus;
        si (t.initium < ZEPHYRUM && e->nodus != NIHIL)
        {
            (vacuum)materia_tractus_nodi(uncus, e->nodus, &t);
        }
        cella = _addere(&d, e->gravitas, e->codex, e->causa, e->nodus,
            e->lexema, &t);
        /* Sedes relatae parsatoris transeunt intactae; computatio
         * 'initium -I' PRIMARIAM solam tangit, quia ex nodo derivatur
         * et sedes relata aliunde per definitionem spectat. Series
         * relatorum clientis eandem piscinam vivere debet. */
        si (cella != NIHIL)
        {
            cella->nota               = e->nota;
            cella->relata             = e->relata;
            cella->numerus_relatorum  = e->numerus_relatorum;
        }
    }
    si (d.memoria_defecit)
    {
        redde NIHIL;
    }
    numerus  = xar_numerus(d.exitus);
    series   = (MateriaDiagnosticum*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(MateriaDiagnosticum)
            * (memoriae_index)(numerus + I));
    ordinata = xar_creare(piscina,
        (i32)magnitudo(MateriaDiagnosticum));
    si (series == NIHIL || ordinata == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        MateriaDiagnosticum clavis =
            *(MateriaDiagnosticum*)xar_obtinere(d.exitus, k);
        s32 j = (s32)k - (s32)I;

        dum (j >= ZEPHYRUM && _prius(&clavis, &series[j]))
        {
            series[j + I] = series[j];
            j--;
        }
        series[j + I] = clavis;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        MateriaDiagnosticum* cella = (MateriaDiagnosticum*)xar_addere(
            ordinata);

        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = series[k];
    }
    redde ordinata;
}


/* ==================================================
 * Sequentia plena (specificatio exemplariorum par. III)
 * ================================================== */

/* Ordines unius Xar in alterum transferre. 'xar_extendere' NON
 * EXSTAT - pseudocodex specificationis eam finxit, et ansa haec est
 * quod revera scribitur. */
interior b32
_transferre (
          Xar* ad,
 constans Xar* ex)
{
    i32 k;

    si (ex == NIHIL)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(ex); k++)
    {
        MateriaDiagnosticum* cella =
            (MateriaDiagnosticum*)xar_addere(ad);

        si (cella == NIHIL)
        {
            redde FALSUM;
        }
        *cella = *(constans MateriaDiagnosticum*)xar_obtinere(ex, k);
    }
    redde VERUM;
}

/* Ordines sine sede nominare. VERUM = omnes locati. */
interior b32
_sedes_omnes_adsunt (
             Piscina*  piscina,
        constans Xar*  ordines,
  constans character** causa)
{
                   i32  k;
                   i32  sine    = ZEPHYRUM;
    constans character* primus  = NIHIL;

    per (k = ZEPHYRUM; k < xar_numerus(ordines); k++)
    {
        constans MateriaDiagnosticum* d =
            (constans MateriaDiagnosticum*)xar_obtinere(ordines, k);

        si (d->tractus.initium < ZEPHYRUM)
        {
            sine++;
            si (primus == NIHIL)
            {
                primus = d->codex;
            }
        }
    }
    si (sine == ZEPHYRUM)
    {
        redde VERUM;
    }
    si (causa != NIHIL)
    {
        character* c = (character*)piscina_allocare(piscina, CCLVI);

        si (c == NIHIL)
        {
            *causa = "ordines sine sede";
        }
        alioquin
        {
            sprintf(c, "regula '%s' ordines %d sine sede peperit - "
                "inventum sine sede locari non potest; regula nodum "
                "capiat", primus != NIHIL ? primus : "?",
                (integer)sine);
            *causa = c;
        }
    }
    redde FALSUM;
}

/* Gradum II per regulam quamque currere. Proiectio SEMEL fit et
 * regulis omnibus servit: parsura una, proiectio una. */
interior b32
_gradus_secundus (
                          Piscina*  piscina,
            constans MateriaNodus*  radix,
 constans MateriaDiagnosticaRatio*  ratio,
              InternamentumChorda*  intern,
                              Xar*  exitus,
               constans character** causa)
{
    MateriaArborConsilium consilium;
    MateriaArborScriptura scriptura;
                      i32 k;

    si (   ratio->regulae              == NIHIL
        || xar_numerus(ratio->regulae) == ZEPHYRUM)
    {
        redde VERUM;
    }
    materia_arbor_consilium_nudum(&consilium, ratio->tabularium,
        ratio->lexicon, ratio->grammatica);
    consilium.intern          = intern;
    consilium.sedes_scribere  = VERUM;
    scriptura = materia_arbor_proicere_nodum(piscina, radix,
        &consilium);
    si (!scriptura.successus || scriptura.arbor == NIHIL)
    {
        /* SCRIPTURA FRACTA ORDO EST, NON SILENTIUM. Refutatio muta
         * plagulam vitiosam a plagula sana non distingueret (utraque
         * 'nihil iudicatum'), et scriptor sedem suam IAM fert
         * (MateriaArborScriptura.tractus). Gradus II tacet - sine
         * proiectione exemplar nihil videt - sed gradus I superest. */
        MateriaDiagnosticum* cella =
            (MateriaDiagnosticum*)xar_addere(exitus);

        si (cella == NIHIL)
        {
            redde FALSUM;
        }
        memset(cella, ZEPHYRUM, magnitudo(*cella));
        cella->gravitas  = (s32)MATERIA_GRAVITAS_ERRATUM;
        cella->codex     = MATERIA_CODEX_SCRIPTURA;
        cella->causa     = scriptura.causa != NIHIL
            ? scriptura.causa : "scriptura fracta";
        cella->tractus   = scriptura.tractus;
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < xar_numerus(ratio->regulae); k++)
    {
        StmlNodus* regula = *(StmlNodus**)xar_obtinere(ratio->regulae,
                                k);
           StmlNodus* documentum;
StmlExpansioResultus  expansio;

        documentum = materia_exemplaria_componere(piscina,
            scriptura.arbor, regula, intern);
        si (documentum == NIHIL)
        {
            redde FALSUM;
        }
        expansio = stml_expandere(documentum, piscina, intern);
        si (!expansio.successus || expansio.radix_expansa == NIHIL)
        {
            redde FALSUM;
        }
        {
            Xar* ordines = materia_exemplaria_extrahere(piscina,
                               expansio.radix_expansa);

            /* SUBTRACTIO PER REGULAM, non per acervum totum:
             * bracchia in documento UNO vivunt, et sedes clavis est -
             * ordines regulae alterius sedes easdem ferre possunt. */
            si (ordines != NIHIL && !ratio->crudum)
            {
                ordines = materia_exemplaria_minuere(piscina,
                    expansio.radix_expansa, ordines, causa);
            }
            si (ordines == NIHIL)
            {
                redde FALSUM;
            }
            /* ORDO SINE SEDE REFUTATIO EST. Regula quae congruit sed
             * nodum non capit ('<situs/>') inventum LOCARI non potest,
             * et numerus tacite minor a sanitate non differt. Extractor
             * prior (pythonica) idem recusabat, nominatim. */
            si (!_sedes_omnes_adsunt(piscina, ordines, causa))
            {
                redde FALSUM;
            }
            si (!_transferre(exitus, ordines))
            {
                redde FALSUM;
            }
        }
    }
    redde VERUM;
}

Xar*
materia_diagnostica_plena (
                          Piscina*  piscina,
            constans MateriaNodus*  radix,
 constans MateriaDiagnosticaRatio*  ratio,
                              Xar*  emissa,
               constans character** causa)
{
    InternamentumChorda* intern;
                    Xar* exitus;
                    Xar* annotationes = NIHIL;

    si (causa != NIHIL)
    {
        *causa = NIHIL;
    }
    si (   piscina == NIHIL || radix == NIHIL || ratio == NIHIL
        || ratio->tabularium == NIHIL || ratio->lexicon == NIHIL)
    {
        redde NIHIL;
    }
    /* INTERNAMENTUM CLIENTIS, non nostrum: regulae eo lectae sunt,
     * et exemplaria identitatem internatam conferunt. */
    intern = ratio->intern != NIHIL
        ? ratio->intern : internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde NIHIL;
    }
    exitus = xar_creare(piscina, (i32)magnitudo(MateriaDiagnosticum));
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    si (!_transferre(exitus, materia_diagnostica_derivare(piscina,
            radix, ratio->tabularium, ratio->declarata, NIHIL,
            emissa)))
    {
        redde NIHIL;
    }
    si (!_gradus_secundus(piscina, radix, ratio, intern, exitus,
            causa))
    {
        redde NIHIL;
    }
    /* ANNOTATIONES SEMEL: excusatio una gradus AMBOS tegit, ergo
     * '<tolera>' utrumvis genus supprimit. Bis collectae essent si
     * gradus quisque suam vocationem haberet - et arbor bis
     * ambularetur sine causa. */
    si (ratio->praefixum != NIHIL)
    {
        annotationes = materia_annotationes_colligere(piscina, radix,
            ratio->lexicon, ratio->praefixum, NIHIL, intern);
    }
    redde materia_excusatio_applicare(piscina, exitus, annotationes,
        ratio->declarata, ratio->grammatica);
}
