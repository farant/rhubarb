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
 * via -> BriarMembrum* (semel), tituli = titulus -> via (plagulae
 * genitae unicae) */
nomen structura {
                 Piscina* piscina;
     InternamentumChorda* intern;
      constans SilexFons* fons;
                     Xar* acervus;     /* constans character* */
          TabulaDispersa* perfecta;
          TabulaDispersa* tituli;
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

/* "<titulus>_regiones.h" */
interior chorda
_caput_membri (
               Piscina* piscina,
    constans character* titulus)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)64);

    chorda_aedificator_appendere_literis(a, titulus);
    chorda_aedificator_appendere_literis(a, "_regiones.h");
    redde chorda_aedificator_finire(a);
}

/* tabula derivationis ex indicibus membrorum (Xar de BriarMembrum*) */
interior BriarSilvaMembra*
_silva_membra (
    Piscina* piscina,
        Xar* indices)
{
    BriarSilvaMembra* sm = (BriarSilvaMembra*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(BriarSilvaMembra));
                  i32 i;

    sm->nomina  = tabula_dispersa_creare_chorda(piscina, 64);
    sm->textus  = tabula_dispersa_creare_chorda(piscina, 16);
    sm->ordo    = tabula_dispersa_creare_chorda(piscina, 16);
    per (i = ZEPHYRUM; indices != NIHIL
        && i < xar_numerus(indices); i++)
    {
        BriarMembrum* m = *(BriarMembrum**)xar_obtinere(indices, i);
              chorda* caput = (chorda*)piscina_allocare(piscina,
                  (memoriae_index)magnitudo(chorda));
              chorda* textus = (chorda*)piscina_allocare(piscina,
                  (memoriae_index)magnitudo(chorda));
                 i32 k;

        *caput   = _caput_membri(piscina, m->titulus);
        *textus  = m->caput_parsurae;
        tabula_dispersa_inserere(sm->textus, *caput, (vacuum*)textus);
        /* dependentia post-ordine (visibilia iam post-ordo) */
        {
            Xar* ordo = xar_creare(piscina, (i32)magnitudo(chorda));

            per (k = ZEPHYRUM; m->visibilia != NIHIL
                && k < xar_numerus(m->visibilia); k++)
            {
                *(chorda*)xar_addere(ordo) = _caput_membri(piscina,
                    (*(BriarMembrum**)xar_obtinere(m->visibilia,
                    k))->titulus);
            }
            tabula_dispersa_inserere(sm->ordo, *caput, (vacuum*)ordo);
        }
        per (k = ZEPHYRUM; m->nomina != NIHIL
            && k < xar_numerus(m->nomina); k++)
        {
            BriarNomenPublicum* n = (BriarNomenPublicum*)xar_obtinere(
                m->nomina, k);

            tabula_dispersa_inserere(sm->nomina, n->titulus,
                (vacuum*)caput);
        }
    }
    redde sm;
}

BriarSilvaMembra*
briar_membra_silvae (
    Piscina* piscina,
        Xar* membra)
{
    Xar* indices = xar_creare(piscina, (i32)magnitudo(BriarMembrum*));
    i32  i;

    per (i = ZEPHYRUM; membra != NIHIL && i < xar_numerus(membra); i++)
    {
        *(BriarMembrum**)xar_addere(indices) =
            (BriarMembrum*)xar_obtinere(membra, i);
    }
    redde _silva_membra(piscina, indices);
}

/* textus parsurae PROPRIUS: lineae '#include "m_regiones.h"' membrorum
 * VISIBILIUM tolluntur - textus eorum per ordinem in praeludio
 * importantis semel ponitur (briar_silva _parsare) */
interior chorda
_membra_tollere (
                      Piscina* piscina,
                       chorda  textus,
    constans BriarSilvaMembra* sm)
{
    ChordaAedificator* a = chorda_aedificator_creare(piscina,
        (memoriae_index)(textus.mensura + 16));
                  i32 i = ZEPHYRUM;

    dum (i < textus.mensura)
    {
         i32 f = i;
         i32 k = X;    /* strlen("#include \"") */
      chorda linea;

        dum (f < textus.mensura && textus.datum[f] != '\n')
        {
            f = f + I;
        }
        linea = chorda_sectio(textus, i, f);
        si (!(   linea.mensura > k + I
              && memcmp(linea.datum, "#include \"", (size_t)k)
                     == ZEPHYRUM
              && linea.datum[linea.mensura - I] == '"'
              && tabula_dispersa_continet(sm->textus,
                     chorda_sectio(linea, k, linea.mensura - I))))
        {
            chorda_aedificator_appendere_chorda(a, linea);
            chorda_aedificator_appendere_literis(a, "\n");
        }
        i = f + I;
    }
    redde chorda_aedificator_finire(a);
}

interior b32
_ambulare (
        MembraAmbulatio* amb,
     constans character* via_importantis,
                    Xar* nexus,
                    Xar* directa);

/* membrum unum: resolvere, probare, dependentia, silva, partitio */
interior BriarMembrum*
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
     constans character* titulus;
                 chorda  textus;
          MateriaNodus* doc;
                   Xar* nexus;
                   Xar* directa;
                   Xar* visibilia;
       BriarSilvaMembra* sm;
    BriarMembrumPartitum  pt;
                 vacuum* iam = NIHIL;
                    i32  i;
                    b32  planum = FALSUM;

    si (data.mensura == ZEPHYRUM)
    {
        character b[64];

        sprintf(b, "bibliotheca sine via (linea %d)",
            (integer)r->linea_initium);
        (vacuum)_refutare(amb, via_importantis, r->linea_initium,
            chorda_ex_literis(b, p));
        redde NIHIL;
    }
    si (   data.mensura <= VIII
        || memcmp(data.datum + data.mensura - VIII, ".thistle",
               (size_t)VIII) != ZEPHYRUM)
    {
        (vacuum)_refutare(amb, via_importantis, r->linea_initium,
            _causa_elementi(p, data, r->linea_initium,
                "non est plagula .thistle"));
        redde NIHIL;
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
            (vacuum)_refutare(amb, via_importantis, r->linea_initium,
                chorda_aedificator_finire(a));
            redde NIHIL;
        }
    }
    /* iam membrum: semel */
    si (tabula_dispersa_invenire(amb->perfecta, chorda_ex_literis(via,
            p), &iam))
    {
        redde (BriarMembrum*)iam;
    }
    si (!filum_existit(via) || filum_directorium_existit(via))
    {
        (vacuum)_refutare(amb, via_importantis, r->linea_initium,
            _causa_elementi(p, data, r->linea_initium,
                "plagula non exsistit"));
        redde NIHIL;
    }

    /* arbor -> nexus -> contextus; silva POST dependentia */
    textus = filum_legere_totum(via, p);
    doc    = textus.datum != NIHIL ? briar_arbor_parsare(p,
        (constans character*)textus.datum, (i32)textus.mensura) : NIHIL;
    nexus  = doc != NIHIL ? briar_nexus_texere(p, doc, amb->intern)
        : NIHIL;
    si (nexus == NIHIL || briar_contexere(p, nexus, NIHIL) < ZEPHYRUM)
    {
        (vacuum)_refutare(amb, via, ZEPHYRUM,
            chorda_ex_literis("bibliotheca: nexus membri fractus", p));
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        si (briar_nexus_regio_plana((constans BriarNexusRes*)
                xar_obtinere(nexus, i)))
        {
            planum = VERUM;
        }
    }
    si (!planum)
    {
        (vacuum)_refutare(amb, via_importantis, r->linea_initium,
            _causa_elementi(p, data, r->linea_initium,
                "regio C plana nulla (nihil commune)"));
        redde NIHIL;
    }
    titulus = briar_fabrica_titulus(p, via);
    {
        vacuum* alter = NIHIL;

        si (tabula_dispersa_invenire(amb->tituli, chorda_ex_literis(
                titulus, p), &alter))
        {
            ChordaAedificator* a = chorda_aedificator_creare(p,
                (memoriae_index)256);

            chorda_aedificator_appendere_literis(a,
                "bibliotheca: titulus '");
            chorda_aedificator_appendere_literis(a, titulus);
            chorda_aedificator_appendere_literis(a, "' iteratus: ");
            chorda_aedificator_appendere_literis(a,
                (constans character*)alter);
            chorda_aedificator_appendere_literis(a, ", ");
            chorda_aedificator_appendere_literis(a, via);
            chorda_aedificator_appendere_literis(a,
                " - plagulae genitae idem nomen haberent; alteram"
                " renomina");
            (vacuum)_refutare(amb, via_importantis, r->linea_initium,
                chorda_aedificator_finire(a));
            redde NIHIL;
        }
    }

    /* dependentia primum (post-ordo) */
    directa = xar_creare(p, (i32)magnitudo(BriarMembrum*));
    *(constans character**)xar_addere(amb->acervus) = via;
    si (!_ambulare(amb, via, nexus, directa))
    {
        redde NIHIL;
    }
    xar_removere_ultimum(amb->acervus);

    /* visibilia: dependentia directa + visibilia eorum, semel */
    visibilia = xar_creare(p, (i32)magnitudo(BriarMembrum*));
    {
        TabulaDispersa* visa = tabula_dispersa_creare_chorda(p, 16);

        per (i = ZEPHYRUM; i < xar_numerus(directa); i++)
        {
            BriarMembrum* d = *(BriarMembrum**)xar_obtinere(directa, i);
                     s32  k;

            /* post-ordo: visibilia d (iam post-ordo), deinde d */
            per (k = ZEPHYRUM; k <= (s32)xar_numerus(d->visibilia); k++)
            {
                BriarMembrum* v = k
                    == (s32)xar_numerus(d->visibilia) ? d
                    : *(BriarMembrum**)xar_obtinere(d->visibilia,
                        (i32)k);

                si (!tabula_dispersa_continet(visa, chorda_ex_literis(
                        v->via, p)))
                {
                    tabula_dispersa_inserere(visa, chorda_ex_literis(
                        v->via, p), (vacuum*)v);
                    *(BriarMembrum**)xar_addere(visibilia) = v;
                }
            }
        }
    }
    sm = _silva_membra(p, visibilia);
    si (briar_silvam_texere_cum_membris(p, nexus, amb->fons, via, sm)
        < ZEPHYRUM)
    {
        (vacuum)_refutare(amb, via, ZEPHYRUM,
            chorda_ex_literis("bibliotheca: silva membri fracta", p));
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        constans BriarNexusRes* s = (constans BriarNexusRes*)
            xar_obtinere(nexus, i);

        si (   s->genus == BRIAR_NEXUS_REGIO
            && briar_nexus_titulus_est(s, "c")
            && s->linea_erroris > ZEPHYRUM)
        {
            (vacuum)_refutare(amb, via, s->linea_erroris, s->causa);
            redde NIHIL;
        }
    }
    pt = briar_membrum_partiri(p, nexus, via, titulus);
    si (!pt.successus)
    {
        (vacuum)_refutare(amb, via, pt.linea_causae, pt.causa);
        redde NIHIL;
    }
    {
        BriarMembrum* m = (BriarMembrum*)xar_addere(amb->membra);

        m->via              = via;
        m->titulus          = titulus;
        m->octeti           = textus;
        m->documentum       = doc;
        m->nexus            = nexus;
        m->via_importantis  = via_importantis;
        m->linea_elementi   = r->linea_initium;
        m->caput            = pt.caput;
        m->caput_parsurae   = _membra_tollere(p, pt.caput_parsurae,
            sm);
        m->corpus     = pt.corpus;
        m->derivata   = pt.derivata;
        m->nomina     = briar_silva_nomina_publica(p, nexus);
        m->visibilia  = visibilia;
        tabula_dispersa_inserere(amb->perfecta, chorda_ex_literis(via,
            p), (vacuum*)m);
        tabula_dispersa_inserere(amb->tituli, chorda_ex_literis(titulus,
            p), (vacuum*)chorda_ut_cstr(chorda_ex_literis(via, p), p));
        redde m;
    }
}

interior b32
_ambulare (
        MembraAmbulatio* amb,
     constans character* via_importantis,
                    Xar* nexus,
                    Xar* directa)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(nexus); i++)
    {
        constans BriarNexusRes* r = (constans BriarNexusRes*)
            xar_obtinere(nexus, i);
                  BriarMembrum* m;

        si (   r->genus != BRIAR_NEXUS_STML
            || !briar_nexus_titulus_est(r, "bibliotheca"))
        {
            perge;
        }
        m = _membrum_visitare(amb, via_importantis, r);
        si (m == NIHIL)
        {
            redde FALSUM;
        }
        si (directa != NIHIL)
        {
            *(BriarMembrum**)xar_addere(directa) = m;
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
    amb.tituli = tabula_dispersa_creare_chorda(piscina, 32);
    amb.membra = xar_creare(piscina, (i32)magnitudo(BriarMembrum));
    amb.causa = causa;
    *(constans character**)xar_addere(amb.acervus) = radix;
    tabula_dispersa_inserere(amb.tituli, chorda_ex_literis(
        briar_fabrica_titulus(piscina, radix), piscina),
        (vacuum*)chorda_ut_cstr(chorda_ex_literis(radix, piscina),
        piscina));
    si (!_ambulare(&amb, radix, nexus_radicis, NIHIL))
    {
        redde FALSUM;
    }
    *membra = amb.membra;
    redde VERUM;
}
