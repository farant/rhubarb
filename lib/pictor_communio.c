/* pictor_communio.c - communio intra pictorem (communio-plan-5 T5) */

#include "pictor_communio.h"
#include "communio_cliens.h"
#include "pictor_documentum.h"
#include "json.h"
#include "xar.h"
#include "filum.h"
#include "dispensator.h"
#include "claviatura.h"
#include "pulsus_forma.h"

#include <stdio.h>
#include <string.h>

#define BASIS_MAXIMA        CCLVI
#define VIA_MAXIMA          CCCXX
#define TESSERA_MAXIMA      XLIV
#define TEMPUS_INDICIS      ((i32)XXX * (i32)M)
#define CLAVIS_APPARATUUM   "communio/apparatus"
#define INDEX_MORA_MS       ((i32)M)
/* pulsus-plan T9 (spec III.4) */
#define PULSUS_THEMA        "earth.golden.pictor"
#define PULSUS_CLAVIS       "pictor.picturae"
#define PULSUS_UNUS         "{persona} shared a picture"
#define PULSUS_PLURES       "{persona} shared {numerus} pictures"
#define PULSUS_MORA         CCC
#define AMBITUS_MAXIMUS     XVI

structura PictorCommunio {
                 Piscina* piscina;
                 Piscina* scheda;     /* petitio indicis */
                 Reactor* reactor;
              Veredarius* veredarius;
                 Volumen* volumen;
               character  via[VIA_MAXIMA];
               character  basis[BASIS_MAXIMA];
               character  tessera[TESSERA_MAXIMA];
    PictorCommunioMutata  mutata;
                  vacuum* ctx;
          CommunioCliens* cliens;
                     i32  mora;
                     b32  mora_posita;
                     b32  recusata;
                     b32  clausa;
    /* index apparatuum: petitio una simul */
                     b32 index_in_itinere;
                     b32 index_respondit;
                     b32 index_iterum;
                     i32 signum;
     VeredariusResponsum responsum;
          ReactorTimerId hora;
                     b32 hora_viva;
    /* plan-5 T7: inscriptio et ligatio ad montationem */
      CommunioInscriptio* inscriptio;
                     i32  inscriptio_mora;
                     i32  inscriptio_vita;
          PictorMontatio* montatio;
             Dispensator* d;
               character  machina[XXXII];
                     s32  gradus_nuntiatus;
    /* index sine apparatu proprio (nondum ligatus): iterum post moram */
                     i32 index_conatus;
          ReactorTimerId hora_indicis;
                     b32 hora_indicis_viva;
    /* Franus 2026-10-30: epistula inscriptionis currentis - IN MEMORIA
     * SOLA ("Sent to ..."); numquam in volumine, vacua post
     * resumptionem, abiectionem, inscriptionem, clausuram */
               character epistula_missa[CCLVI];
    /* pulsus-plan T9: testa (pulsum_parare), tessera apparatus hex (in
     * memoria sola), petitio semel per processum */
       TestaPulsumPetere  pulsum_petere;
                  vacuum* pulsus_sui;
               character  pulsus_ambitus[AMBITUS_MAXIMUS];
               character  pulsus_tessera[PULSUS_TESSERA_MAXIMA + I];
                     b32  pulsus_armatus;
                     b32  pulsus_petitus;
};

s32
pictor_communio_inscriptionis_gradus (
    constans PictorCommunio* pc);

b32
pictor_communio_inscripta (
    constans PictorCommunio* pc);

/* mutatio: montatio ligata renovatur (inscripta, Shared pictures,
 * claviatura cum inscripta celata), deinde recompositio et mutata */
interior vacuum
_nuntiare (
    PictorCommunio* pc)
{
    s32 g;

    si (pc->clausa)
    {
        redde;
    }
    si (pc->montatio)
    {
        pc->montatio->compositio.inscripta =
            pictor_communio_inscripta(pc);
        si (pc->montatio->communia.scheda)
        {
            (vacuum)pictor_communia_parare(&pc->montatio->communia);
        }
        g = pictor_communio_inscriptionis_gradus(pc);
        si (   g != pc->gradus_nuntiatus
            && g == (s32)INSCRIPTIO_INSCRIPTA
            && pc->montatio->actiones_ctx.claviaturam_ponere)
        {
            pc->montatio->actiones_ctx.claviaturam_ponere(
                pc->montatio->actiones_ctx.claviatura_ctx,
                (i32)TESTA_CLAVIATURA_NULLA);
        }
        pc->gradus_nuntiatus = g;
    }
    si (pc->d)
    {
        dispensator_recomponere(pc->d);
    }
    si (pc->mutata)
    {
        pc->mutata(pc->ctx);
    }
}

/* tessera recusata (cliens aut index 401): desinere, filum delere */
interior vacuum
_recusatam_tractare (
    PictorCommunio* pc)
{
    si (pc->recusata)
    {
        redde;
    }
    pc->recusata = VERUM;
    si (pc->cliens)
    {
        communio_cliens_sistere(pc->cliens);
    }
    si (pc->index_in_itinere && !pc->index_respondit)
    {
        pc->veredarius->abrumpere(pc->veredarius->sui, pc->signum);
    }
    pc->index_in_itinere = FALSUM;
    (vacuum)filum_delere(pc->via);
    memset(pc->tessera, ZEPHYRUM, magnitudo(pc->tessera));
    _nuntiare(pc);
}

interior vacuum
_recusata (
    vacuum* ctx)
{
    _recusatam_tractare((PictorCommunio*)ctx);
}

interior vacuum
_index_progredi (
    vacuum* datum);

interior vacuum
_index_finis (
                          vacuum* ctx,
    constans VeredariusResponsum* r)
{
    PictorCommunio* pc = (PictorCommunio*)ctx;

    pc->responsum        = *r;
    pc->index_respondit  = VERUM;
    pc->hora = reactor_timer_adicere(pc->reactor, I, FALSUM,
        _index_progredi, pc);
    pc->hora_viva = pc->hora != (ReactorTimerId)-I;
}

/* GET /communio/apparatus (si nulla in itinere; aliter iterum post) */
interior vacuum
_indicem_petere (
    PictorCommunio* pc)
{
    VeredariusPetitio p;
            character url[BASIS_MAXIMA + XXXII];
            character auctoritas[LXIV];

    si (pc->recusata || pc->clausa || pc->tessera[ZEPHYRUM] == '\0')
    {
        redde;
    }
    si (pc->index_in_itinere)
    {
        pc->index_iterum = VERUM;
        redde;
    }
    piscina_vacare(pc->scheda);
    sprintf(url, "%s/communio/apparatus", pc->basis);
    sprintf(auctoritas, "Bearer %.43s", pc->tessera);
    memset(&p, ZEPHYRUM, magnitudo(p));
    p.methodus           = VEREDARIUS_GET;
    p.url                = url;
    p.auctoritas         = auctoritas;
    p.tempus_ms          = TEMPUS_INDICIS;
    pc->index_respondit  = FALSUM;
    pc->index_iterum     = FALSUM;
    pc->index_in_itinere  = pc->veredarius->petere(pc->veredarius->sui,
        &p, pc->scheda, _index_finis, pc, &pc->signum);
    memset(auctoritas, ZEPHYRUM, magnitudo(auctoritas));
}

interior vacuum
_indicem_petere (
    PictorCommunio* pc);

interior JsonValor*
_apparatus_invenire (
    constans PictorCommunio* pc,
                     chorda  replica,
                    Piscina* piscina);

interior vacuum
_index_iterum (
    vacuum* datum)
{
    PictorCommunio* pc = (PictorCommunio*)datum;

    pc->hora_indicis_viva = FALSUM;
    _indicem_petere(pc);
}

interior vacuum
_index_progredi (
    vacuum* datum)
{
    PictorCommunio* pc;
               i32  status;

    pc             = (PictorCommunio*)datum;
    pc->hora_viva  = FALSUM;
    si (!pc->index_in_itinere || !pc->index_respondit || pc->clausa)
    {
        redde;
    }
    pc->index_in_itinere = FALSUM;
    status = pc->responsum.successus ? pc->responsum.status : ZEPHYRUM;
    si (status == CDI)
    {
        _recusatam_tractare(pc);
        redde;
    }
    si (status == CC && pc->responsum.corpus.mensura > ZEPHYRUM)
    {
        (vacuum)volumen_praesens_ponere(pc->volumen, chorda_ex_literis(
            CLAVIS_APPARATUUM, pc->scheda), pc->responsum.corpus);
        /* plan-5 T7: apparatus proprius nondum in indice (ligatio
         * clientis post petitionem) - iterum post moram, V ad summum,
         * ut "Signed in as" personam habeat */
        si (   !_apparatus_invenire(pc, volumen_replica(pc->volumen),
                pc->scheda)
            && pc->index_conatus < V && !pc->hora_indicis_viva)
        {
            pc->index_conatus++;
            pc->hora_indicis = reactor_timer_adicere(pc->reactor,
                INDEX_MORA_MS, FALSUM, _index_iterum, pc);
            pc->hora_indicis_viva = pc->hora_indicis
                != (ReactorTimerId)-I;
        }
        _nuntiare(pc);
    }
    si (pc->index_iterum)
    {
        _indicem_petere(pc);
    }
}

/* index servatus: obiectum apparatus replicae, aut NIHIL */
interior JsonValor*
_apparatus_invenire (
    constans PictorCommunio* pc,
                     chorda  replica,
                    Piscina* piscina)
{
          chorda  valor;
             b32  inventum;
    JsonResultus  r;
       JsonValor* l;
       JsonValor* x;
          chorda  vacua;
             i32  i;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    valor = volumen_praesens_legere(pc->volumen, chorda_ex_literis(
        CLAVIS_APPARATUUM, piscina), piscina, &inventum);
    si (   !inventum || valor.mensura == ZEPHYRUM || replica.mensura
        == ZEPHYRUM)
    {
        redde NIHIL;
    }
    r = json_legere(valor, piscina);
    l = r.successus ? json_objectum_capere(r.radix,
        "apparatus") : NIHIL;
    per (i = ZEPHYRUM; l && i < json_tabulatum_numerus(l); i++)
    {
        x = json_tabulatum_obtinere(l, i);
        si (   x
            && chorda_aequalis(json_capere_chorda(x, "replica", vacua),
                replica))
        {
            redde x;
        }
    }
    redde NIHIL;
}

/* acta advecta: nuntiare; apparatus novus (replica ignota) -> index */
interior vacuum
_advenerunt (
    vacuum* ctx,
       i32  numerus)
{
    PictorCommunio* pc = (PictorCommunio*)ctx;
           Piscina* temporaria;
               Xar* l;
               i32  i;

    (vacuum)numerus;
    temporaria = piscina_generare_dynamicum("pictor_communio_advenit",
        (memoriae_index)LXIV * (memoriae_index)M);
    l = temporaria ? pictor_documenta_aliena_enumerare(pc->volumen,
        temporaria) : NIHIL;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (!_apparatus_invenire(pc,
            ((PictorPicturaAliena*)xar_obtinere(
                l, i))->replica, temporaria))
        {
            _indicem_petere(pc);
            frange;
        }
    }
    si (temporaria)
    {
        piscina_destruere(temporaria);
    }
    _nuntiare(pc);
}

/* pulsus-plan T9: eventus in caudam clientis; res = "<replica>:" +
 * spatium (C ad plurimum) */
interior b32
_eventum_nuntiare (
       PictorCommunio* pc,
         constans Xar* spatia,
                  b32  statim,
              Piscina* piscina)
{
    PulsusEventus ev;
           chorda replica;
           chorda id;
              i32 i;

    replica = volumen_replica(pc->volumen);
    si (!pc->cliens || pc->recusata || replica.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    memset(&ev, ZEPHYRUM, magnitudo(ev));
    ev.clavis  = chorda_ex_literis(PULSUS_CLAVIS, piscina);
    ev.thema   = chorda_ex_literis(PULSUS_THEMA, piscina);
    ev.unus    = chorda_ex_literis(PULSUS_UNUS, piscina);
    ev.plures  = chorda_ex_literis(PULSUS_PLURES, piscina);
    ev.mora_s  = PULSUS_MORA;
    ev.statim  = statim;
    ev.res     = xar_creare(piscina, (i32)magnitudo(chorda));
    si (!ev.res)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; spatia && i < xar_numerus(spatia)
         && i < PULSUS_RES_PER_PETITIONEM; i++)
    {
        id = chorda_concatenare(chorda_concatenare(replica,
            chorda_ex_literis(":", piscina), piscina),
            *(chorda*)xar_obtinere(spatia, i), piscina);
        *(chorda*)xar_addere(ev.res) = id;
    }
    communio_cliens_pulsum_nuntiare(pc->cliens, &ev);
    redde VERUM;
}

/* acta propria accepta -> eventus de picturis tactis (si quae) */
interior vacuum
_missa (
                   vacuum* ctx,
    constans VolumenActum* acta,
                      i32  numerus)
{
    PictorCommunio* pc = (PictorCommunio*)ctx;
           Piscina* temporaria;
               Xar* spatia;

    temporaria = piscina_generare_dynamicum("pictor_communio_missa",
        (memoriae_index)LXIV * (memoriae_index)M);
    spatia = temporaria ? pictor_spatia_actorum(pc->volumen, acta,
        numerus, temporaria) : NIHIL;
    si (spatia && xar_numerus(spatia) > ZEPHYRUM)
    {
        (vacuum)_eventum_nuntiare(pc, spatia, FALSUM, temporaria);
    }
    si (temporaria)
    {
        piscina_destruere(temporaria);
    }
}

/* tessera nota et cliens: inscribere (cliens copiat) */
interior vacuum
_pulsum_inscribere (
    PictorCommunio* pc)
{
    si (   !pc->cliens || pc->recusata || pc->pulsus_tessera[ZEPHYRUM]
        == '\0')
    {
        redde;
    }
    communio_cliens_pulsum_inscribere(pc->cliens, chorda_ex_literis(
        pc->pulsus_tessera, pc->scheda), chorda_ex_literis(
        pc->pulsus_ambitus, pc->scheda), chorda_ex_literis(PULSUS_THEMA,
        pc->scheda));
}

/* testa: tessera (aut vacua + causa: negata, defectus). Tessera
 * numquam in nuntio. */
interior vacuum
_pulsus_signum (
    vacuum* ctx,
    chorda  tessera_hex,
    chorda  causa)
{
    PictorCommunio* pc = (PictorCommunio*)ctx;

    si (pc->clausa)
    {
        redde;
    }
    si (   tessera_hex.mensura == ZEPHYRUM
        || tessera_hex.mensura > PULSUS_TESSERA_MAXIMA)
    {
        fprintf(stderr, "pictor: pulsus non inscriptus - %.*s\n",
            (integer)(causa.mensura < CCLVI ? causa.mensura : CCLVI),
            causa.datum ? (constans character*)causa.datum : "");
        redde;
    }
    memcpy(pc->pulsus_tessera, tessera_hex.datum,
        (size_t)tessera_hex.mensura);
    pc->pulsus_tessera[tessera_hex.mensura] = '\0';
    _pulsum_inscribere(pc);
}

/* armatus, inscripta, nondum petitum: testam rogare */
interior vacuum
_pulsum_petere (
    PictorCommunio* pc)
{
    si (   !pc->pulsus_armatus || pc->pulsus_petitus
        || !pc->pulsum_petere
        || !pictor_communio_inscripta(pc))
    {
        redde;
    }
    pc->pulsus_petitus = VERUM;
    (vacuum)pc->pulsum_petere(pc->pulsus_sui, _pulsus_signum, NIHIL,
        pc);
}

PictorCommunio*
pictor_communionem_aperire (
               Piscina* piscina,
               Reactor* reactor,
            Veredarius* veredarius,
               Volumen* volumen,
    constans character* via_tesserae,
    constans character* basis,
  PictorCommunioMutata  mutata,
                vacuum* ctx)
{
    PictorCommunio* pc;

    si (   !piscina || !reactor || !veredarius || !volumen
        || !via_tesserae || !basis || strlen(via_tesserae) >= VIA_MAXIMA
        || strlen(basis) >= BASIS_MAXIMA)
    {
        redde NIHIL;
    }
    pc = (PictorCommunio*)piscina_allocare(piscina,
        magnitudo(PictorCommunio));
    si (!pc)
    {
        redde NIHIL;
    }
    memset(pc, ZEPHYRUM, magnitudo(PictorCommunio));
    pc->scheda = piscina_generare_dynamicum("pictor_communio",
        (memoriae_index)CXXVIII * (memoriae_index)M);
    si (!pc->scheda)
    {
        redde NIHIL;
    }
    pc->piscina     = piscina;
    pc->reactor     = reactor;
    pc->veredarius  = veredarius;
    pc->volumen     = volumen;
    pc->mutata      = mutata;
    pc->ctx         = ctx;
    strcpy(pc->via, via_tesserae);
    strcpy(pc->basis, basis);
    redde pc;
}

b32
pictor_communio_inscripta (
    constans PictorCommunio* pc)
{
    character t[TESSERA_MAXIMA];
          b32 valet;

    si (!pc || pc->recusata || pc->clausa)
    {
        redde FALSUM;
    }
    /* plan-5 T7: inscriptio pendens - filum tesserae exstat sed nondum
     * inscriptum */
    si (   pc->inscriptio
        && communio_inscriptionis_gradus(pc->inscriptio)
           != INSCRIPTIO_INSCRIPTA)
    {
        redde FALSUM;
    }
    si (pc->tessera[ZEPHYRUM] != '\0')
    {
        redde VERUM;
    }
    valet = filum_existit(pc->via)
        && communio_tesseram_legere(pc->via, t);
    memset(t, ZEPHYRUM, magnitudo(t));
    redde valet;
}

vacuum
pictor_communio_incipere (
    PictorCommunio* pc)
{
    si (!pc || pc->clausa)
    {
        redde;
    }
    /* plan-5 T7: inscriptio pendens - tessera nondum valet (cliens eam
     * recusatam videret et deleret) */
    si (   pc->inscriptio
        && communio_inscriptionis_gradus(pc->inscriptio)
           != INSCRIPTIO_INSCRIPTA)
    {
        redde;
    }
    /* plan-5 T7: post recusationem cliens vetus desiit - novus */
    si (pc->cliens && pc->recusata)
    {
        communio_clientem_destruere(pc->cliens);
        pc->cliens = NIHIL;
    }
    si (pc->cliens)
    {
        pictor_communio_excitare(pc);
        redde;
    }
    /* post recusationem inscriptio nova tesseram novam scripsit */
    si (   !filum_existit(pc->via) || !communio_tesseram_legere(pc->via,
            pc->tessera))
    {
        memset(pc->tessera, ZEPHYRUM, magnitudo(pc->tessera));
        redde;
    }
    pc->recusata  = FALSUM;
    pc->cliens    = communio_clientem_creare(pc->piscina, pc->reactor,
        pc->veredarius, pc->volumen, pc->basis, chorda_ex_literis(
        pc->tessera, pc->scheda), _advenerunt, pc);
    si (!pc->cliens)
    {
        redde;
    }
    communio_cliens_recusationem_ponere(pc->cliens, _recusata, pc);
    communio_cliens_missa_ponere(pc->cliens, _missa, pc);
    _pulsum_inscribere(pc);
    si (pc->mora_posita)
    {
        communio_cliens_moram_ponere(pc->cliens, pc->mora);
    }
    communio_cliens_incipere(pc->cliens);
    _indicem_petere(pc);
}

/* review finalis II: applicatio resumpta - inscriptio pendens statim
 * inspicit; cliens petitiones fortasse mortuas abrumpit et renovat */
vacuum
pictor_communio_excitare (
    PictorCommunio* pc)
{
    si (!pc || pc->clausa)
    {
        redde;
    }
    si (pc->inscriptio)
    {
        communio_inscriptionem_excitare(pc->inscriptio);
    }
    si (pc->recusata || !pc->cliens)
    {
        redde;
    }
    communio_cliens_renovare(pc->cliens);
    _indicem_petere(pc);
}

chorda
pictor_communio_auctor (
    constans PictorCommunio* pc,
                     chorda  replica,
                    Piscina* piscina)
{
    JsonValor* x;
       chorda  vacua;
       chorda  persona;
       chorda  titulus;

    vacua  = chorda_ex_literis("", piscina);
    x      = pc ? _apparatus_invenire(pc, replica, piscina) : NIHIL;
    si (!x)
    {
        redde vacua;
    }
    persona = json_capere_chorda(x, "persona", vacua);
    titulus = json_capere_chorda(x, "titulus", vacua);
    si (persona.mensura == ZEPHYRUM)
    {
        redde titulus;
    }
    si (titulus.mensura == ZEPHYRUM)
    {
        redde persona;
    }
    redde chorda_concatenare(chorda_concatenare(persona,
        chorda_ex_literis(" \xC2\xB7 ", piscina), piscina), titulus,
        piscina);
}

chorda
pictor_communio_persona (
    constans PictorCommunio* pc,
                    Piscina* piscina)
{
    JsonValor* x;
       chorda  vacua;

    vacua = chorda_ex_literis("", piscina);
    x = pc ? _apparatus_invenire(pc, volumen_replica(pc->volumen),
        piscina) : NIHIL;
    redde x ? json_capere_chorda(x, "persona", vacua) : vacua;
}

vacuum
pictor_communio_moram_ponere (
    PictorCommunio* pc,
               i32  secundae)
{
    si (!pc)
    {
        redde;
    }
    pc->mora         = secundae;
    pc->mora_posita  = VERUM;
    si (pc->cliens)
    {
        communio_cliens_moram_ponere(pc->cliens, secundae);
    }
}

vacuum
pictor_communionem_claudere (
    PictorCommunio* pc)
{
    si (!pc || pc->clausa)
    {
        redde;
    }
    pc->clausa = VERUM;
    memset(pc->epistula_missa, ZEPHYRUM, magnitudo(pc->epistula_missa));
    memset(pc->pulsus_tessera, ZEPHYRUM, magnitudo(pc->pulsus_tessera));
    si (pc->hora_indicis_viva)
    {
        (vacuum)reactor_timer_cancellare(pc->reactor, pc->hora_indicis);
        pc->hora_indicis_viva = FALSUM;
    }
    si (pc->hora_viva)
    {
        (vacuum)reactor_timer_cancellare(pc->reactor, pc->hora);
        pc->hora_viva = FALSUM;
    }
    si (pc->index_in_itinere && !pc->index_respondit)
    {
        pc->veredarius->abrumpere(pc->veredarius->sui, pc->signum);
    }
    pc->index_in_itinere = FALSUM;
    si (pc->cliens)
    {
        communio_clientem_destruere(pc->cliens);
        pc->cliens = NIHIL;
    }
    si (pc->inscriptio)
    {
        communio_inscriptionem_relinquere(pc->inscriptio);
        pc->inscriptio = NIHIL;
    }
    memset(pc->tessera, ZEPHYRUM, magnitudo(pc->tessera));
    piscina_destruere(pc->scheda);
    pc->scheda = NIHIL;
}


/* ==================================================
 * plan-5 T7: inscriptio
 * ================================================== */

interior vacuum
_inscriptio_mutata (
              vacuum* ctx,
    InscriptioGradus  gradus)
{
    PictorCommunio* pc = (PictorCommunio*)ctx;

    /* recusata manet: incipere clientem a 401 sistitum agnoscit et
     * novum creat (review finalis I) */
    si (gradus == INSCRIPTIO_INSCRIPTA)
    {
        memset(pc->epistula_missa, ZEPHYRUM,
            magnitudo(pc->epistula_missa));
        pictor_communio_incipere(pc);
        /* pulsus-plan T9: statim post inscriptionem */
        _pulsum_petere(pc);
    }
    _nuntiare(pc);
}

interior vacuum
_inscriptionem_temperare (
    PictorCommunio* pc)
{
    si (pc->inscriptio && pc->inscriptio_mora > ZEPHYRUM)
    {
        communio_inscriptionis_moram_ponere(pc->inscriptio,
            pc->inscriptio_mora);
    }
    si (pc->inscriptio && pc->inscriptio_vita > ZEPHYRUM)
    {
        communio_inscriptionis_vitam_ponere(pc->inscriptio,
            pc->inscriptio_vita);
    }
}

b32
pictor_communio_inscriptionem_incipere (
        PictorCommunio* pc,
                chorda  epistula,
    constans character* titulus)
{
    si (!pc || pc->clausa || !titulus || pictor_communio_inscripta(pc))
    {
        redde FALSUM;
    }
    si (pc->inscriptio)
    {
        communio_inscriptionem_abicere(pc->inscriptio);
        pc->inscriptio = NIHIL;
    }
    memset(pc->epistula_missa, ZEPHYRUM, magnitudo(pc->epistula_missa));
    si (epistula.mensura < magnitudo(pc->epistula_missa))
    {
        memcpy(pc->epistula_missa, epistula.datum,
            (size_t)epistula.mensura);
    }
    pc->inscriptio = communio_inscriptionem_incipere(pc->piscina,
        pc->reactor, pc->veredarius, pc->volumen, pc->basis, epistula,
        pc->via, chorda_ex_literis(titulus, pc->piscina),
        _inscriptio_mutata, pc);
    _inscriptionem_temperare(pc);
    _nuntiare(pc);
    redde pc->inscriptio != NIHIL;
}

b32
pictor_communio_codicem_dare (
     PictorCommunio* pc,
             chorda  codex)
{
    redde pc && pc->inscriptio
        && communio_inscriptioni_codicem_dare(pc->inscriptio, codex);
}

s32
pictor_communio_inscriptionis_gradus (
    constans PictorCommunio* pc)
{
    redde (pc && pc->inscriptio)
        ? (s32)communio_inscriptionis_gradus(pc->inscriptio) : -I;
}

vacuum
pictor_communio_inscriptionem_abicere (
    PictorCommunio* pc)
{
    si (!pc || !pc->inscriptio)
    {
        redde;
    }
    communio_inscriptionem_abicere(pc->inscriptio);
    pc->inscriptio = NIHIL;
    memset(pc->epistula_missa, ZEPHYRUM, magnitudo(pc->epistula_missa));
    _nuntiare(pc);
}

b32
pictor_communio_inscriptionem_resumere (
    PictorCommunio* pc)
{
    si (!pc || pc->clausa || pc->inscriptio)
    {
        redde FALSUM;
    }
    pc->inscriptio = communio_inscriptionem_resumere(pc->piscina,
        pc->reactor, pc->veredarius, pc->volumen, pc->basis, pc->via,
        _inscriptio_mutata, pc);
    _inscriptionem_temperare(pc);
    redde pc->inscriptio != NIHIL;
}

vacuum
pictor_communio_inscriptionis_ponere (
    PictorCommunio* pc,
               i32  mora_ms,
               i32  vita_ms)
{
    si (!pc)
    {
        redde;
    }
    pc->inscriptio_mora = mora_ms;
    pc->inscriptio_vita = vita_ms;
    _inscriptionem_temperare(pc);
}

/* ligatio: functiones pro compositione et actionibus (ctx = pc) */
interior chorda
_auctor_ligatum (
    vacuum* ctx,
    chorda  replica,
   Piscina* piscina)
{
    redde pictor_communio_auctor((PictorCommunio*)ctx, replica,
        piscina);
}

interior chorda
_persona_ligata (
    vacuum* ctx,
   Piscina* piscina)
{
    redde pictor_communio_persona((PictorCommunio*)ctx, piscina);
}

interior s32
_gradus_ligatus (
    vacuum* ctx)
{
    redde pictor_communio_inscriptionis_gradus((PictorCommunio*)ctx);
}

interior b32
_incipere_ligatum (
    vacuum* ctx,
    chorda  epistula)
{
    PictorCommunio* pc = (PictorCommunio*)ctx;

    redde pictor_communio_inscriptionem_incipere(pc, epistula,
        pc->machina[ZEPHYRUM] ? pc->machina : "Mac");
}

interior b32
_codicem_ligatum (
    vacuum* ctx,
    chorda  codex)
{
    redde pictor_communio_codicem_dare((PictorCommunio*)ctx, codex);
}

interior vacuum
_abicere_ligatum (
    vacuum* ctx)
{
    pictor_communio_inscriptionem_abicere((PictorCommunio*)ctx);
}

interior vacuum
_excitare_ligatum (
    vacuum* ctx)
{
    pictor_communio_excitare((PictorCommunio*)ctx);
}

chorda
pictor_communio_epistula_missa (
    constans PictorCommunio* pc,
                    Piscina* piscina)
{
    s32 g;

    g = pictor_communio_inscriptionis_gradus(pc);
    si (   !pc || pc->epistula_missa[ZEPHYRUM] == '\0'
        || (   g != (s32)INSCRIPTIO_PETIT
            && g != (s32)INSCRIPTIO_EXSPECTAT
            && g != (s32)INSCRIPTIO_CODEX_FALSUS
            && g != (s32)INSCRIPTIO_CONFIRMAT))
    {
        redde chorda_ex_literis("", piscina);
    }
    redde chorda_ex_literis(pc->epistula_missa, piscina);
}

interior chorda
_epistula_ligata (
     vacuum* ctx,
    Piscina* piscina)
{
    redde pictor_communio_epistula_missa((constans PictorCommunio*)ctx,
        piscina);
}

interior b32
_statim_ligatum (
                 vacuum* ctx,
     constans character* spatium)
{
    redde pictor_communio_pulsum_statim((PictorCommunio*)ctx, spatium);
}

vacuum
pictor_communionem_ligare (
         PictorCommunio* pc,
         PictorMontatio* montatio,
            Dispensator* d,
     constans character* machina)
{
    si (!pc || !montatio)
    {
        redde;
    }
    pc->montatio           = montatio;
    pc->d                  = d;
    pc->machina[ZEPHYRUM]  = '\0';
    si (machina && strlen(machina) < magnitudo(pc->machina))
    {
        strcpy(pc->machina, machina);
    }
    montatio->compositio.communio_praesto      = VERUM;
    montatio->compositio.inscripta             =
        pictor_communio_inscripta(pc);
    montatio->compositio.auctor = _auctor_ligatum;
    montatio->compositio.persona = _persona_ligata;
    montatio->compositio.inscriptionis_gradus = _gradus_ligatus;
    montatio->compositio.epistula_missa = _epistula_ligata;
    montatio->compositio.auctor_ctx = pc;
    montatio->actiones_ctx.inscriptionem_incipere = _incipere_ligatum;
    montatio->actiones_ctx.codicem_dare = _codicem_ligatum;
    montatio->actiones_ctx.inscriptionem_abicere = _abicere_ligatum;
    montatio->actiones_ctx.communio_excitare = _excitare_ligatum;
    montatio->actiones_ctx.pulsum_statim = _statim_ligatum;
    montatio->actiones_ctx.inscriptio_ctx = pc;
    pc->gradus_nuntiatus = pictor_communio_inscriptionis_gradus(pc);
}


/* ==================================================
 * pulsus-plan T9
 * ================================================== */

vacuum
pictor_communio_pulsum_parare (
           PictorCommunio* pc,
    constans TestaAmbitus* a)
{
    si (!pc || pc->clausa || !a || !a->pulsum_petere)
    {
        redde;
    }
    pc->pulsum_petere  = a->pulsum_petere;
    pc->pulsus_sui     = a->pulsus_ctx;
    strcpy(pc->pulsus_ambitus, "productio");
    si (   a->pulsus_ambitus
        && strlen(a->pulsus_ambitus) < magnitudo(pc->pulsus_ambitus))
    {
        strcpy(pc->pulsus_ambitus, a->pulsus_ambitus);
    }
    pc->pulsus_armatus = VERUM;
    _pulsum_petere(pc);
}

b32
pictor_communio_pulsum_statim (
         PictorCommunio* pc,
     constans character* spatium)
{
    Piscina* temporaria;
        Xar* l;
        b32  factum;

    si (   !pc || pc->clausa || !pc->cliens
        || !pictor_communio_inscripta(pc))
    {
        redde FALSUM;
    }
    temporaria = piscina_generare_dynamicum("pictor_communio_statim",
        (memoriae_index)IV * (memoriae_index)M);
    si (!temporaria)
    {
        redde FALSUM;
    }
    l = xar_creare(temporaria, (i32)magnitudo(chorda));
    si (l && spatium)
    {
        *(chorda*)xar_addere(l) = chorda_ex_literis(spatium,
            temporaria);
    }
    factum = l && _eventum_nuntiare(pc, l, VERUM, temporaria);
    piscina_destruere(temporaria);
    redde factum;
}
