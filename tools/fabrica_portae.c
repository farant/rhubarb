/* fabrica_portae.c - bin/fabrica portae: PORTAE DECLARATAE (plan
 * portae-declaratae T3; nucleus lib/portae.c, caput include/portae.h)
 *
 * Usus (ex radice repositorii):
 *   bin/fabrica portae debitae [-machina] -portae TSV VIA...
 *       quae portae viis mutatis debentur (et quae viae intectae)
 *   bin/fabrica portae tactus [-machina] -portae TSV (-omnes | VIA...)
 *       quas portas quaeque via tangit, rationibus OMNIBUS (-omnes =
 *       viae omnes arboris; comparatio T4)
 *   bin/fabrica portae declaratae [-machina] -portae TSV
 *       tabula portarum declaratarum et opertura derivata
 *   bin/fabrica portae relatio [-machina] -portae TSV [-tempora VIA]
 *       portae per genera unitatum: opertura, sumptus, intecta,
 *       rationes textus et infrastructurae (tools/fabrica_relatio.c)
 *
 * -portae TSV: registrum ab involucro traditum, linea una per portam
 * 'clavis<TAB>cursor[<TAB>directorium probationum]' (PORTAE et SUITAE
 * pythonicae; C Pythonem numquam parsat). Effusio -machina: contractus
 * in include/portae.h. Exitus: 0 computatum · 2 usus, registrum
 * illegibile aut RECUSATIO declarationum.
 *
 * Sutura vera: viae per 'git ls-files', plagulae per filum, effectus
 * per crusta_effectus (tabula mandatorum semel lecta), litterae
 * probationum per lexatorem silvae amalgamatae (commenta absunt;
 * plagula sine 'system'/'processus_' nihil vocat), clausurae per
 * suturae_clausura_c (aedilis, idem quod gradus fabricae). */

#include "postulata_posix.h"

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "filum.h"
#include "processus.h"
#include "internamentum.h"
#include "stml.h"
#include "fabrica.h"
#include "portae.h"
#include "crusta_effectus.h"
#include "fabrica_sutura.h"
#include "fabrica_litterae.h"

#include <stdio.h>
#include <string.h>

nomen structura {
    InternamentumChorda* intern;
                 chorda  radix;      /* absoluta, sine '/' finali */
              StmlNodus* mandata;    /* tabula effectus; NIHIL = legitur
                                      * a crusta ipsa */
} PortaeMundus;

interior constans character*
_cstr (
      chorda  c,
     Piscina* piscina)
{
    redde chorda_ut_cstr(c, piscina);
}

interior Xar*
_enumerare (
      vacuum* datum,
     Piscina* piscina)
{
    hic_manens constans character* constans argumenta[] = {
        "git", "ls-files", NIHIL
    };
        ProcessusResultus  r;
    chorda_fissio_fructus  lineae;
                      Xar* viae;
                      i32  i;

    (vacuum)datum;
    r = processus_exsequi(argumenta, (i32)XXX * M, piscina);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde NIHIL;
    }
    viae    = xar_creare(piscina, (i32)magnitudo(chorda));
    lineae  = chorda_fissio(r.effusio, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        si (lineae.elementa[i].mensura > ZEPHYRUM)
        {
            *(chorda*)xar_addere(viae) = lineae.elementa[i];
        }
    }
    redde viae;
}

interior b32
_legere (
      vacuum* datum,
      chorda  via,
     Piscina* piscina,
      chorda* textus)
{
    constans character* v = _cstr(via, piscina);

    (vacuum)datum;
    si (!filum_existit(v))
    {
        redde FALSUM;
    }
    *textus = filum_legere_totum(v, piscina);
    redde VERUM;
}

interior StmlNodus*
_effectus (
      vacuum* datum,
      chorda  cursor,
     Piscina* piscina)
{
           PortaeMundus* m      = (PortaeMundus*)datum;
     constans character* causa  = NIHIL;

    redde crusta_effectus_derivare(piscina, m->intern, _cstr(m->radix,
        piscina), _cstr(cursor, piscina), m->mandata, &causa);
}

interior Xar*
_litterae (
      vacuum* datum,
      chorda  fons,
     Piscina* piscina)
{
    chorda  textus;
       Xar* fructus;

    si (!_legere(datum, fons, piscina, &textus))
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(chorda));
    si (   !chorda_continet(textus, chorda_ex_literis("system",
        piscina))
        && !chorda_continet(textus, chorda_ex_literis("processus_",
               piscina)))
    {
        redde fructus;
    }
    si (!suturae_litteras_colligere(textus, piscina, fructus))
    {
        redde NIHIL;
    }
    redde fructus;
}

interior Xar*
_clausura (
      vacuum* datum,
      chorda  fons,
     Piscina* piscina)
{
    FabricaClausuraC  clausura;
              chorda  causa;
                 Xar* fructus;
                 i32  i;

    (vacuum)datum;
    causa = chorda_ex_literis("", piscina);
    si (!suturae_clausura_c(NIHIL, _cstr(fons, piscina), piscina,
            &clausura, &causa))
    {
        redde NIHIL;
    }
    fructus = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; i < xar_numerus(clausura.fontes); i++)
    {
        *(chorda*)xar_addere(fructus) = *(chorda*)xar_obtinere(
            clausura.fontes, i);
    }
    per (i = ZEPHYRUM; i < xar_numerus(clausura.capita); i++)
    {
        *(chorda*)xar_addere(fructus) = *(chorda*)xar_obtinere(
            clausura.capita, i);
    }
    redde fructus;
}

/* registrum TSV: 'clavis<TAB>cursor[<TAB>probationes]'; NIHIL =
 * illegibile (causa in stderr) */
interior Xar*
_registrum_legere (
    constans character* via,
               Piscina* piscina)
{
                   chorda  textus;
    chorda_fissio_fructus  lineae;
                      Xar* tabula;
                      i32  i;

    si (!filum_existit(via))
    {
        fprintf(stderr, "fabrica portae: registrum absens: %s\n", via);
        redde NIHIL;
    }
    textus = filum_legere_totum(via, piscina);
    tabula = xar_creare(piscina, (i32)magnitudo(PortaeIngressus));
    lineae = chorda_fissio(textus, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        chorda_fissio_fructus  campi;
              PortaeIngressus* p;

        si (lineae.elementa[i].mensura == ZEPHYRUM)
        {
            perge;
        }
        campi = chorda_fissio(lineae.elementa[i], '\t', piscina);
        si (campi.numerus < II || campi.numerus > III)
        {
            fprintf(stderr, "fabrica portae: %s:%u: linea registri "
                "mala (clavis<TAB>cursor[<TAB>probationes])\n", via,
                (insignatus integer)(i + I));
            redde NIHIL;
        }
        p          = (PortaeIngressus*)xar_addere(tabula);
        p->clavis  = campi.elementa[ZEPHYRUM];
        p->cursor  = campi.elementa[I];
        p->probationes  = campi.numerus == III ? campi.elementa[II]
            : chorda_ex_literis("", piscina);
    }
    redde tabula;
}

interior vacuum
_usus (vacuum)
{
    fprintf(stderr,
        "usus: bin/fabrica portae debitae [-machina] -portae TSV "
        "VIA...\n"
        "      bin/fabrica portae tactus [-machina] -portae TSV "
        "(-omnes | VIA...)\n"
        "      bin/fabrica portae declaratae [-machina] -portae TSV\n"
        "      bin/fabrica portae ordo [-machina] -portae TSV"
        " [-tempora VIA] PORTA...\n"
        "      bin/fabrica portae relatio [-machina] -portae TSV"
        " [-tempora VIA]\n");
}

interior vacuum
_scribere (
    chorda textus)
{
    (vacuum)fwrite(textus.datum, I, (size_t)textus.mensura, stdout);
}

s32
suturae_portae (
          s32   argc,
    character** argv,
      Piscina*  piscina)
{
               PortaeMundus  mundus;
               PortaeSutura  sutura;
         constans character* via_registri  = NIHIL;
                        b32  machina       = FALSUM;
                        b32  debitae;
                        b32  tactus;
                        b32  ordo;
                        b32  relatio;
         constans character* via_temporum  = "build/portae/tempora.tsv";
                        b32  omnes         = FALSUM;
                        Xar* viae;
                        Xar* declaratae;
                        Xar* recusationes = NIHIL;
                        Xar* operturae;
                        Xar* debita;
                        Xar* intecta = NIHIL;
                     chorda  causa;
                     chorda  tabula_mandatorum;
                        s32  a;

    si (   argc < III || (strcmp(argv[2], "debitae") != 0
        && strcmp(argv[2], "tactus") != 0
        && strcmp(argv[2], "declaratae") != 0
        && strcmp(argv[2], "ordo") != 0
        && strcmp(argv[2], "relatio") != 0))
    {
        _usus();
        redde II;
    }
    debitae  = strcmp(argv[2], "debitae") == 0;
    tactus   = strcmp(argv[2], "tactus") == 0;
    ordo     = strcmp(argv[2], "ordo") == 0;
    relatio  = strcmp(argv[2], "relatio") == 0;
    viae     = xar_creare(piscina, (i32)magnitudo(chorda));
    per (a = III; a < argc; a++)
    {
        si (strcmp(argv[a], "-machina") == 0)
        {
            machina = VERUM;
        }
        alioquin si (strcmp(argv[a], "-portae") == 0 && a + I < argc)
        {
            via_registri = argv[++a];
        }
        alioquin si (tactus && strcmp(argv[a], "-omnes") == 0)
        {
            omnes = VERUM;
        }
        alioquin si (   (ordo || relatio)
                     && strcmp(argv[a], "-tempora") == 0
                     && a + I < argc)
        {
            via_temporum = argv[++a];
        }
        alioquin si ((debitae || tactus || ordo) && argv[a][0] != '-')
        {
            *(chorda*)xar_addere(viae) = chorda_ex_literis(argv[a],
                piscina);
        }
        alioquin
        {
            _usus();
            redde II;
        }
    }
    si (via_registri == NIHIL)
    {
        _usus();
        redde II;
    }
    mundus.intern   = internamentum_creare(piscina);
    mundus.radix    = suturae_radix_absoluta(piscina);
    mundus.mandata  = NIHIL;
    tabula_mandatorum =
        filum_legere_totum("crusta/effectus_mandata.stml",
        piscina);
    si (tabula_mandatorum.mensura > ZEPHYRUM)
    {
        StmlResultus r = stml_legere(tabula_mandatorum, piscina,
            mundus.intern);

        mundus.mandata = r.successus ? r.elementum_radix : NIHIL;
    }
    sutura.datum                 = &mundus;
    sutura.plagulas_enumerare    = _enumerare;
    sutura.plagulam_legere       = _legere;
    sutura.effectus              = _effectus;
    sutura.litterae_probationis  = _litterae;
    sutura.clausura_c            = _clausura;
    sutura.tabula_portarum      = _registrum_legere(via_registri,
        piscina);
    si (sutura.tabula_portarum == NIHIL)
    {
        redde II;
    }
    causa = chorda_ex_literis("", piscina);
    declaratae = portae_declaratas_legere(&sutura, piscina, &causa,
        &recusationes);
    si (declaratae == NIHIL)
    {
        _scribere(debitae
            || tactus ? portae_debita_scribere(NIHIL, NIHIL,
            recusationes, ZEPHYRUM, machina, piscina)
            : portae_declaratas_scribere(NIHIL, recusationes, machina,
                piscina));
        redde II;
    }
    /* ORDO (circulus commissionis II T5): declarationes solae - genus
     * ex annotatione, sumptus ex tempora.tsv; derivatio non poscitur.
     * Tempora absentia = omnes sine historia (ordo nominum servatur). */
    si (ordo)
    {
        chorda tempora = filum_legere_totum(via_temporum, piscina);

        _scribere(portae_ordinem_scribere(portae_ordinare(declaratae,
            viae, tempora, piscina), machina, piscina));
        redde ZEPHYRUM;
    }
    operturae = portae_operturas_derivare(&sutura, declaratae, piscina);
    si (operturae == NIHIL)
    {
        fprintf(stderr, "fabrica portae: derivatio fracta (enumeratio "
            "'git ls-files' aut memoria)\n");
        redde II;
    }
    si (relatio)
    {
        Xar* omnes_viae = _enumerare(&mundus, piscina);

        si (omnes_viae == NIHIL)
        {
            fprintf(stderr, "fabrica portae: 'git ls-files' fractum\n");
            redde II;
        }
        redde suturae_portae_relatio(&sutura, declaratae, operturae,
            omnes_viae, filum_legere_totum(via_temporum, piscina),
            machina, piscina);
    }
    si (!debitae && !tactus)
    {
        _scribere(portae_declaratas_scribere(operturae, NIHIL, machina,
            piscina));
        redde ZEPHYRUM;
    }
    si (omnes)
    {
        viae = _enumerare(&mundus, piscina);
        si (viae == NIHIL)
        {
            fprintf(stderr, "fabrica portae: 'git ls-files' fractum\n");
            redde II;
        }
    }
    si (tactus)
    {
        debita = portae_tactus_computare(&sutura, operturae, viae,
            &intecta, piscina);
        si (debita == NIHIL)
        {
            fprintf(stderr, "fabrica portae: tactus computari "
                "nequeunt\n");
            redde II;
        }
        _scribere(portae_tactus_scribere(debita, intecta, machina,
            piscina));
        redde ZEPHYRUM;
    }
    debita = portae_debita_computare(&sutura, operturae, viae, &intecta,
        piscina);
    si (debita == NIHIL)
    {
        fprintf(stderr, "fabrica portae: debita computari nequeunt\n");
        redde II;
    }
    _scribere(portae_debita_scribere(debita, intecta, NIHIL,
        xar_numerus(viae), machina, piscina));
    redde ZEPHYRUM;
}
