/* briar_plagulae.c - Membra aedificationis (vide briar_plagulae.h) */

#include "briar_plagulae.h"
#include "briar_arbor.h"
#include "briar_nexus.h"
#include "briar_contextus.h"
#include "briar_silva.h"
#include "briar_fabrica.h"
#include "filum.h"
#include "via.h"
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>

/* status ambulationis: acervus = viae in cursu (circulus), perfecta =
 * viae iam membra (semel) */
nomen structura {
                 Piscina* piscina;
     InternamentumChorda* intern;
      constans SilexFons* fons;
                     Xar* acervus;     /* constans character* */
          TabulaDispersa* perfecta;
                     Xar* membra;      /* BriarMembrum */
        BriarMembraCausa* causa;
} MembraAmbulatio;

interior b32
_refutare (
        MembraAmbulatio* amb,
     constans character* via,
                    i32  linea,
                 chorda  causa)
{
    amb->causa->causa  = causa;
    amb->causa->via    = via;
    amb->causa->linea  = linea;
    redde FALSUM;
}

/* "bibliotheca 'x' (linea N): <quid>" */
interior chorda
_causa_elementi (
               Piscina* piscina,
                chorda  via_elementi,
                   i32  linea,
    constans character* quid)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)256);
            character b[32];

    chorda_aedificator_appendere_literis(a, "bibliotheca '");
    chorda_aedificator_appendere_chorda(a, via_elementi);
    sprintf(b, "' (linea %d): ", (integer)linea);
    chorda_aedificator_appendere_literis(a, b);
    chorda_aedificator_appendere_literis(a, quid);
    redde chorda_aedificator_finire(a);
}

interior constans character*
_nomen_fili (
               Piscina* piscina,
    constans character* via)
{
    redde chorda_ut_cstr(via_nomen(chorda_ex_literis(via, piscina),
        piscina), piscina);
}

/* regio C plana: non fragmentum, non probatio, sine methodus */
interior b32
_regio_plana (
    constans BriarNexusRes* r)
{
    redde r->genus == BRIAR_NEXUS_REGIO
        && briar_nexus_titulus_est(r, "c")
        && !r->est_fragmentum
        && !chorda_aequalis_literis(briar_nexus_attributum(r, "munus"),
               "probatio")
        && !briar_nexus_attributum_habet(r, "methodus");
}

interior b32
_ambulare (
        MembraAmbulatio* amb,
     constans character* via_importantis,
                    Xar* nexus);

/* membrum unum: resolvere, probare, parsare, ambulare, addere */
interior b32
_membrum_visitare (
           MembraAmbulatio* amb,
        constans character* via_importantis,
    constans BriarNexusRes* r)
{
                Piscina* p     = amb->piscina;
                 chorda  data  = briar_nexus_attributum(r, "via");
                 chorda  partes[II];
                 chorda  plena;
     constans character* via;
                 chorda  textus;
          MateriaNodus* doc;
                   Xar* nexus;
                    i32  i;
                    b32  planum = FALSUM;

    si (data.mensura == ZEPHYRUM)
    {
        character b[64];

        sprintf(b, "bibliotheca sine via (linea %d)",
            (integer)r->linea_initium);
        redde _refutare(amb, via_importantis, r->linea_initium,
            chorda_ex_literis(b, p));
    }
    si (   data.mensura <= VIII
        || memcmp(data.datum + data.mensura - VIII, ".thistle",
               (size_t)VIII) != ZEPHYRUM)
    {
        redde _refutare(amb, via_importantis, r->linea_initium,
            _causa_elementi(p, data, r->linea_initium,
                "non est plagula .thistle"));
    }
    /* via contra directorium IMPORTANTIS (numquam cwd) */
    si (via_est_absoluta(data))
    {
        plena = data;
    }
    alioquin
    {
        partes[ZEPHYRUM] = via_directorium(chorda_ex_literis(
            via_importantis, p), p);
        partes[I]  = data;
        plena      = via_iungere(partes, II, p);
    }
    via = chorda_ut_cstr(via_normalizare(plena, p), p);

    /* circulus: via in acervo (in cursu) */
    per (i = ZEPHYRUM; i < xar_numerus(amb->acervus); i++)
    {
        si (strcmp(*(constans character**)xar_obtinere(amb->acervus, i),
                via) == ZEPHYRUM)
        {
            ChordaAedificator* a = chorda_aedificator_creare(p,
                (memoriae_index)256);
                          i32 j;

            chorda_aedificator_appendere_literis(a,
                "bibliotheca circularis: ");
            per (j = i; j < xar_numerus(amb->acervus); j++)
            {
                chorda_aedificator_appendere_literis(a, _nomen_fili(p,
                    *(constans character**)xar_obtinere(amb->acervus,
                    j)));
                chorda_aedificator_appendere_literis(a, " -> ");
            }
            chorda_aedificator_appendere_literis(a, _nomen_fili(p,
                via));
            chorda_aedificator_appendere_literis(a,
                " - partem communem in plagulam tertiam move");
            redde _refutare(amb, via_importantis, r->linea_initium,
                chorda_aedificator_finire(a));
        }
    }
    /* iam membrum: semel */
    si (tabula_dispersa_continet(amb->perfecta, chorda_ex_literis(via,
            p)))
    {
        redde VERUM;
    }
    si (!filum_existit(via) || filum_directorium_existit(via))
    {
        redde _refutare(amb, via_importantis, r->linea_initium,
            _causa_elementi(p, data, r->linea_initium,
                "plagula non exsistit"));
    }

    /* parsare ut tools/briar.c: arbor -> nexus -> contextus -> silva */
    textus = filum_legere_totum(via, p);
    doc    = textus.datum != NIHIL ? briar_arbor_parsare(p,
        (constans character*)textus.datum, (i32)textus.mensura) : NIHIL;
    nexus  = doc != NIHIL ? briar_nexus_texere(p, doc, amb->intern)
        : NIHIL;
    si (   nexus == NIHIL
        || briar_contexere(p, nexus, NIHIL) < ZEPHYRUM
        || briar_silvam_texere(p, nexus, amb->fons, via) < ZEPHYRUM)
    {
        redde _refutare(amb, via, ZEPHYRUM,
            chorda_ex_literis("bibliotheca: nexus membri fractus", p));
    }
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        constans BriarNexusRes* s = (constans BriarNexusRes*)
            xar_obtinere(nexus, i);

        si (   s->genus == BRIAR_NEXUS_REGIO
            && briar_nexus_titulus_est(s, "c")
            && s->linea_erroris > ZEPHYRUM)
        {
            redde _refutare(amb, via, s->linea_erroris, s->causa);
        }
        si (_regio_plana(s))
        {
            planum = VERUM;
        }
    }
    si (!planum)
    {
        redde _refutare(amb, via_importantis, r->linea_initium,
            _causa_elementi(p, data, r->linea_initium,
                "regio C plana nulla (nihil commune)"));
    }

    /* post-ordo: membra membri primum */
    *(constans character**)xar_addere(amb->acervus) = via;
    si (!_ambulare(amb, via, nexus))
    {
        redde FALSUM;
    }
    xar_removere_ultimum(amb->acervus);
    si (tabula_dispersa_continet(amb->perfecta, chorda_ex_literis(via,
            p)))
    {
        redde VERUM;    /* per aliam viam interim additum */
    }
    {
        BriarMembrum* m = (BriarMembrum*)xar_addere(amb->membra);

        m->via              = via;
        m->titulus          = briar_fabrica_titulus(p, via);
        m->octeti           = textus;
        m->documentum       = doc;
        m->nexus            = nexus;
        m->via_importantis  = via_importantis;
        m->linea_elementi   = r->linea_initium;
    }
    (vacuum)tabula_dispersa_inserere(amb->perfecta,
        chorda_ex_literis(via, p), (vacuum*)amb->membra);
    redde VERUM;
}

interior b32
_ambulare (
        MembraAmbulatio* amb,
     constans character* via_importantis,
                    Xar* nexus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        constans BriarNexusRes* r = (constans BriarNexusRes*)
            xar_obtinere(nexus, i);

        si (   r->genus == BRIAR_NEXUS_STML
            && briar_nexus_titulus_est(r, "bibliotheca")
            && !_membrum_visitare(amb, via_importantis, r))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

b32
briar_membra_colligere (
                 Piscina*  piscina,
      constans character*  via_radicis,
                     Xar*  nexus_radicis,
     InternamentumChorda*  intern,
      constans SilexFons*  fons,
                     Xar** membra,
        BriarMembraCausa*  causa)
{
       MembraAmbulatio  amb;
    constans character* radix;

    *membra               = NIHIL;
    causa->causa.datum    = NIHIL;
    causa->causa.mensura  = ZEPHYRUM;
    causa->via            = NIHIL;
    causa->linea          = ZEPHYRUM;
    si (   piscina       == NIHIL || via_radicis == NIHIL
        || nexus_radicis == NIHIL)
    {
        redde FALSUM;
    }
    radix = chorda_ut_cstr(via_normalizare(via_absoluta(
        chorda_ex_literis(via_radicis, piscina), piscina), piscina),
        piscina);
    amb.piscina  = piscina;
    amb.intern   = intern;
    amb.fons     = fons;
    amb.acervus   = xar_creare(piscina,
        (i32)magnitudo(constans character*));
    amb.perfecta = tabula_dispersa_creare_chorda(piscina, 32);
    amb.membra = xar_creare(piscina, (i32)magnitudo(BriarMembrum));
    amb.causa = causa;
    *(constans character**)xar_addere(amb.acervus) = radix;
    si (!_ambulare(&amb, radix, nexus_radicis))
    {
        redde FALSUM;
    }
    *membra = amb.membra;
    redde VERUM;
}
