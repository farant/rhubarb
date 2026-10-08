/* aedilis.c - AEDILIS CLI (bin/aedilis) - Phasis A
 *
 * Machina in lib/aedilis.c vivit (pura, sutura extractoris);
 * hic vivunt: extractor silvae (.c/.h), cursus minoritatis -MM
 * (.m - clang oraculum per system() + plagulam temporalem),
 * provenientia git, emissio manifesti.
 *
 * Usus: bin/aedilis <fons.c> [--varians <verbum>]
 * Fructus: build/aedilis/<basis>/manifestum.stml
 * Postura defectus: RECUSARE CLAMOSE (exitus 1, causa nominata).
 *
 * Spec: project-specs/aedilis-spec-v2.md; parcum 01KXJ2HV.
 */

/* plagula provenientiae (fabrica T7): '-provenientia' respondetur */
/* <aedilis obiectum="build/fabrica/provenientia/aedilis.c"/> */

#include "postulata_posix.h"   /* getpid: plagulae temporariae per processum */
#include "latina.h"
#include "provenientia.h"
#include "lectiones.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "via.h"
#include "xar.h"
#include "argumenta.h"
#include "tabula_dispersa.h"
#include "sigillum.h"
#include "thesaurus.h"
#include "aedilis.h"
#include "aedilis_silva.h"

#include "silva.h"

#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

interior vacuum
_chordam_in_xar (
       Xar* xar,
    chorda  valor)
{
    chorda* locus;

    locus = (chorda*)xar_addere(xar);
    si (locus != NIHIL)
    {
        *locus = valor;
    }
}

/* Via temporaria PER PROCESSUM (2026-09-02): "build/aedilis/<basis>.tmp"
 * fixa inter aediles concurrentes communis erat - quattuor probationes
 * corporis silvae aedilem simul vocant (clausurae), et una lectio
 * effusum alienum accepit: plagula sine clausura parsata, porta
 * apparatus (latinizatae 155/156) recusavit. Nomen cum PID, plagula
 * post lectionem deleta. */
interior constans character*
_via_temporaria (
    constans character* basis,
               Piscina* piscina)
{
    character buffer[128];

    sprintf(buffer, "build/aedilis/%s.%ld.tmp", basis,
        (longus)getpid());
    redde chorda_ut_cstr(chorda_ex_literis(buffer, piscina), piscina);
}

/* Differentia-clausurae: sextum capitum nostrum (silva-cursus)
 * contra clang -MM oraculum. CONSENSUS / NOS-SOLI / ORACULUM-SOLUM;
 * exitus 0 = consensus purus (porta per codicem exitus). */
interior s32
_differentiam_currere (
               Piscina* piscina,
          AedilisSilva* extractoris,
        AedilisFructus* fructus,
    constans character* scopus_cstr)
{
    TabulaDispersa* nostra;
    TabulaDispersa* eorum;
               Xar* oraculi;
               Xar* nos_soli;
               Xar* oraculum_solum;
               i32  consensus;
               i32  i;
               i32  numerus;

    /* UNIO -MM super OMNES fontes clausurae: -MM unitatem
     * translationis solam videt, aedilis clausuram NEXUS -
     * capita per fontes obiectorum inventa (tls.h per http.c)
     * in unione demum comparabilia sunt. Obiecta annotata
     * utrimque omissa (symmetria: numquam ambulata). */
    oraculi = xar_creare(piscina, (i32)magnitudo(chorda));
    si (!aedilis_silva_oraculum(extractoris, scopus_cstr, piscina,
            &oraculi))
    {
        fprintf(stderr,
            "AEDILIS RECUSAT: oraculum -MM defecit: %s\n",
            scopus_cstr);
        redde 1;
    }
    numerus = xar_numerus(fructus->obiecta);
    per (i = 0; i < numerus; i++)
    {
        AedilisObiectum* obiectum;

        obiectum = (AedilisObiectum*)xar_obtinere(fructus->obiecta,
            i);
        si (   obiectum->absens
            || obiectum->origo == AEDILIS_ORIGO_ANNOTATIO)
        {
            perge;
        }
        si (!aedilis_silva_oraculum(extractoris,
                chorda_ut_cstr(obiectum->via, piscina), piscina,
                &oraculi))
        {
            fprintf(stderr,
                "AEDILIS RECUSAT: oraculum -MM defecit: %.*s\n",
                (s32)obiectum->via.mensura,
                (constans character*)obiectum->via.datum);
            redde 1;
        }
    }

    nostra   = tabula_dispersa_creare_chorda(piscina, 256);
    eorum    = tabula_dispersa_creare_chorda(piscina, 256);
    numerus  = xar_numerus(fructus->capita);
    per (i = 0; i < numerus; i++)
    {
        AedilisCaput* caput;

        caput = (AedilisCaput*)xar_obtinere(fructus->capita, i);
        (vacuum)tabula_dispersa_inserere(nostra, caput->via,
            NIHIL);
    }
    /* unionem deduplicare (caput idem ex pluribus TU) */
    {
        Xar* unica;

        unica    = xar_creare(piscina, (i32)magnitudo(chorda));
        numerus  = xar_numerus(oraculi);
        per (i = 0; i < numerus; i++)
        {
            chorda via;

            via = *(chorda*)xar_obtinere(oraculi, i);
            si (!tabula_dispersa_continet(eorum, via))
            {
                (vacuum)tabula_dispersa_inserere(eorum, via,
                    NIHIL);
                _chordam_in_xar(unica, via);
            }
        }
        oraculi = unica;
    }

    consensus       = 0;
    nos_soli        = xar_creare(piscina, (i32)magnitudo(chorda));
    oraculum_solum  = xar_creare(piscina, (i32)magnitudo(chorda));
    numerus         = xar_numerus(fructus->capita);
    per (i = 0; i < numerus; i++)
    {
        AedilisCaput* caput;

        caput = (AedilisCaput*)xar_obtinere(fructus->capita, i);
        si (tabula_dispersa_continet(eorum, caput->via))
        {
            consensus++;
        }
        alioquin
        {
            _chordam_in_xar(nos_soli, caput->via);
        }
    }
    numerus = xar_numerus(oraculi);
    per (i = 0; i < numerus; i++)
    {
        chorda via;

        via = *(chorda*)xar_obtinere(oraculi, i);
        si (!tabula_dispersa_continet(nostra, via))
        {
            _chordam_in_xar(oraculum_solum, via);
        }
    }

    imprimere("DIFFERENTIA %s\n", scopus_cstr);
    imprimere("consensus %d | nos-soli %u | oraculum-solum %u\n",
        consensus, xar_numerus(nos_soli),
        xar_numerus(oraculum_solum));
    numerus = xar_numerus(nos_soli);
    per (i = 0; i < numerus; i++)
    {
        chorda via;

        via = *(chorda*)xar_obtinere(nos_soli, i);
        imprimere("  NOS SOLI: %.*s\n", (s32)via.mensura,
            (constans character*)via.datum);
    }
    numerus = xar_numerus(oraculum_solum);
    per (i = 0; i < numerus; i++)
    {
        chorda via;

        via = *(chorda*)xar_obtinere(oraculum_solum, i);
        imprimere("  ORACULUM SOLUM: %.*s\n", (s32)via.mensura,
            (constans character*)via.datum);
    }

    redde (xar_numerus(nos_soli) == 0
        && xar_numerus(oraculum_solum) == 0) ? 0 : 1;
}

/* Provenientia git (optima conatio; NIHIL si abest) */
interior constans character*
_commissum_obtinere (
    Piscina* piscina)
{
                chorda  textus;
                   i32  finis;
    constans character* via_temporaria;
             character  mandatum[256];

        via_temporaria = _via_temporaria("commissum", piscina);
    sprintf(mandatum, "git rev-parse --short HEAD > %s 2>/dev/null",
        via_temporaria);
    si (system(mandatum) != 0)
    {
        (vacuum)remove(via_temporaria);
        redde NIHIL;
    }
    /* filius scripsit: exitus cursus, non ingressus (fabrica plan 5
     * T5b). Contentum (HEAD) solum in commentum struere.sh it */
    lectiones_notare(LECTIO_SCRIPSIT, via_temporaria);
    textus = filum_legere_totum(via_temporaria, piscina);
    (vacuum)remove(via_temporaria);
    finis = textus.mensura;
    dum (   finis > 0 && (textus.datum[finis - 1] == (i8)'\n'
        || textus.datum[finis - 1] == (i8)'\r'))
    {
        finis--;
    }
    si (finis == 0)
    {
        redde NIHIL;
    }
    textus.mensura = finis;
    redde chorda_ut_cstr(textus, piscina);
}

/* Partes fructus ut TSV: O obiecta, C capita, S systemata, V vendores */
interior vacuum
_partes_imprimere (
    constans AedilisFructus* fructus)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(fructus->obiecta);
    per (i = 0; i < numerus; i++)
    {
        AedilisObiectum* obiectum;

        obiectum = (AedilisObiectum*)xar_obtinere(fructus->obiecta, i);
        imprimere("O\t%.*s\n", (s32)obiectum->via.mensura,
            (constans character*)obiectum->via.datum);
    }
    numerus = xar_numerus(fructus->capita);
    per (i = 0; i < numerus; i++)
    {
        AedilisCaput* caput;

        caput = (AedilisCaput*)xar_obtinere(fructus->capita, i);
        imprimere("C\t%.*s\n", (s32)caput->via.mensura,
            (constans character*)caput->via.datum);
    }
    numerus = xar_numerus(fructus->systemata);
    per (i = 0; i < numerus; i++)
    {
        chorda via;

        via = *(chorda*)xar_obtinere(fructus->systemata, i);
        imprimere("S\t%.*s\n", (s32)via.mensura,
            (constans character*)via.datum);
    }
    numerus = xar_numerus(fructus->vendores);
    per (i = 0; i < numerus; i++)
    {
        AedilisVendor* vendor;

        vendor = (AedilisVendor*)xar_obtinere(fructus->vendores, i);
        imprimere("V\t%.*s\n", (s32)vendor->fons.mensura,
            (constans character*)vendor->fons.datum);
    }
}

/* --manifestum: clausura quam enumeratio aut partes legunt, ad viam
 * EXPLICITAM (non build/aedilis/<basis>/ - basis communis manifestum
 * binarii installati obrueret). Sine commisso git: fabrica sigillo
 * manifesti non utitur, et DLXXX derivationes silvae totidem cursus
 * git vitant. Via vacua = nihil agitur. */
interior b32
_manifestum_scribere_si_petitum (
    constans AedilisFructus* fructus,
                     chorda  via_manifesti,
                    Piscina* piscina)
{
    chorda via_parens;

    si (via_manifesti.mensura == 0)
    {
        redde VERUM;
    }
    via_parens = via_directorium(via_manifesti, piscina);
    si (   !filum_directorium_creare_cum_parentibus(
               chorda_ut_cstr(via_parens, piscina))
        || !filum_scribere(chorda_ut_cstr(via_manifesti, piscina),
               aedilis_manifestum_scribere(fructus, piscina, NIHIL)))
    {
        fprintf(stderr, "AEDILIS RECUSAT: manifestum non "
            "scriptum: %.*s\n", (s32)via_manifesti.mensura,
            (constans character*)via_manifesti.datum);
        redde FALSUM;
    }
    redde VERUM;
}

interior b32
_desinit_in_c (
    constans character* titulus)
{
    memoriae_index l;

    l = strlen(titulus);
    redde l > 2 && titulus[l - 2] == '.' && titulus[l - 1] == 'c';
}

interior integer
_chordas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans chorda* ca;
    constans chorda* cb;
     memoriae_index  minima;
            integer  r;

    ca = (constans chorda*)a;
    cb = (constans chorda*)b;
    minima = (memoriae_index)((ca->mensura < cb->mensura)
        ? ca->mensura : cb->mensura);
    r = memcmp(ca->datum, cb->datum, minima);
    si (r != 0)
    {
        redde r;
    }
    si (ca->mensura < cb->mensura)
    {
        redde -1;
    }
    si (ca->mensura > cb->mensura)
    {
        redde 1;
    }
    redde 0;
}

/* --corpus: clausurae OMNIUM fontium .c directorii uno cursu - sectio
 * 'F<tab>via' per fontem, deinde partes ut --partes; ordine nominum.
 * Extractor memor capita semel parsat trans scopos (cursus unus pro
 * CLVI). Fons recusatus: 'RECUSAT<tab>causa' sub sectione sua, ceteri
 * perguntur, exitus 1 in fine - consumptor sectionem vacuam clamat. */
/* --nexus-purus (eventus A1b): capita radicum inclusarum (non
 * recursive) quae <aedilis nexus="purus"/> ferunt per extractorem
 * (silva, non grep) inveniuntur; quodque per
 * aedilis_nexum_purum_probare iudicatur. Exitus 0 = omnia promissa
 * servata; 1 = fractum (causa: catena nominata). */
interior integer
_nexum_purum_currere (
                         Piscina* piscina,
    constans AedilisConfiguratio* configuratio,
                    AedilisSilva* memor)
{
    i32 d;
    i32 radices;
    i32 promittentia  = 0;
    i32 fracta        = 0;

    radices = xar_numerus(configuratio->inclusa);
    per (d = 0; d < radices; d++)
    {
              chorda dir =
                  *(chorda*)xar_obtinere(configuratio->inclusa,
                  d);
           character* dir_cstr  = chorda_ut_cstr(dir, piscina);
                 /* lectiones: notatur */
                 DIR* h         = opendir(dir_cstr);
    structura dirent* introitus;

        si (h == NIHIL)
        {
            lectiones_notare(LECTIO_ABSENS, dir_cstr);
            perge;
        }
        /* radix inclusionum enumerata: nomina = dependentia
         * (obumbratio - fabrica plan 2) */
        lectiones_notare(LECTIO_ENUMERAVIT, dir_cstr);
        dum ((introitus = readdir(h)) != NIHIL)
        {
             memoriae_index  l = strlen(introitus->d_name);
                  character* via;
                        Xar* directivae;
                        Xar* annotationes;
                        Xar* angulatae;
                        b32  ex_oraculo;
                        i32  a;
                        b32  promittit = FALSUM;
                     chorda  causa;

            si (   l < 3 || introitus->d_name[l - 2] != '.'
                || introitus->d_name[l - 1] != 'h')
            {
                perge;
            }
            via = (character*)piscina_allocare(piscina,
                strlen(dir_cstr) + l + 2);
            si (via == NIHIL)
            {
                perge;
            }
            sprintf(via, "%s/%s", dir_cstr, introitus->d_name);
            si (   !aedilis_silva_extrahere(memor, via, piscina,
                    &directivae,
                    &annotationes, &ex_oraculo, &angulatae)
                || annotationes == NIHIL)
            {
                perge;
            }
            per (a = 0; a < xar_numerus(annotationes); a++)
            {
                si (chorda_aequalis_literis(
                        *(chorda*)xar_obtinere(annotationes, a),
                        "nexus purus"))
                {
                    promittit = VERUM;
                }
            }
            si (!promittit)
            {
                perge;
            }
            promittentia++;
            causa.datum    = NIHIL;
            causa.mensura  = 0;
            si (!aedilis_nexum_purum_probare(piscina, configuratio, via,
                    aedilis_silva_extrahere, memor, &causa))
            {
                fracta++;
                fprintf(stderr, "AEDILIS NEXUS PURUS FRACTUS: %.*s\n",
                    (s32)causa.mensura,
                    (constans character*)causa.datum);
            }
        }
        closedir(h);
    }
    printf("nexus purus: %u capita promittunt, %u fracta\n",
        (insignatus integer)promittentia, (insignatus integer)fracta);
    redde (fracta > 0 || promittentia == 0) ? 1 : 0;
}

interior integer
_corpus_currere (
                         Piscina* piscina,
    constans AedilisConfiguratio* configuratio,
              constans character* directorium,
              constans character* varians_cstr,
                    AedilisSilva* memor)
{
    DIR*              d;
    structura dirent* introitus;
    chorda*           viae;
    i32               numerus;
    i32               i;
    integer           exitus;

    /* lectiones: notatur */
    d = opendir(directorium);
    si (d != NIHIL)
    {
        lectiones_notare(LECTIO_ENUMERAVIT, directorium);
    }
    si (d == NIHIL)
    {
        fprintf(stderr,
            "AEDILIS RECUSAT: directorium non apertum: %s\n",
            directorium);
        redde 1;
    }
    numerus = 0;
    dum ((introitus = readdir(d)) != NIHIL)
    {
        si (_desinit_in_c(introitus->d_name))
        {
            numerus++;
        }
    }
    viae = (chorda*)piscina_allocare(piscina,
        (memoriae_index)(numerus
            > 0 ? numerus : 1) * magnitudo(chorda));
    si (viae == NIHIL)
    {
        closedir(d);
        redde 1;
    }
    rewinddir(d);
    i = 0;
    dum ((introitus = readdir(d)) != NIHIL && i < numerus)
    {
             character* via;
        memoriae_index  l;

        si (!_desinit_in_c(introitus->d_name))
        {
            perge;
        }
        l    = strlen(directorium) + strlen(introitus->d_name) + 2;
        via  = (character*)piscina_allocare(piscina, l);
        si (via == NIHIL)
        {
            closedir(d);
            redde 1;
        }
        sprintf(via, "%s/%s", directorium, introitus->d_name);
        viae[i++] = chorda_ex_literis(via, piscina);
    }
    closedir(d);
    numerus = i;
    qsort(viae, (memoriae_index)numerus, magnitudo(chorda),
        _chordas_comparare);
    exitus = 0;
    per (i = 0; i < numerus; i++)
    {
        AedilisFructus* fructus;
                chorda  causa;
             character* via_cstr;

        causa.datum    = NIHIL;
        causa.mensura  = 0;
        via_cstr       = chorda_ut_cstr(viae[i], piscina);
        imprimere("F\t%s\n", via_cstr);
        fructus = aedilis_derivare(piscina, configuratio, via_cstr,
            varians_cstr, aedilis_silva_extrahere, memor, &causa);
        si (fructus == NIHIL)
        {
            imprimere("RECUSAT\t%.*s\n", (s32)causa.mensura,
                (constans character*)causa.datum);
            exitus = 1;
            perge;
        }
        _partes_imprimere(fructus);
    }
    redde exitus;
}

externus constans ProvenientiaRelatio provenientia_aedilis;


s32
principale (
          s32   numerus_argumentorum,
    character** argumenta_cruda)
{
                Piscina* piscina;
        ArgumentaParser* parser;
       ArgumentaFructus* lecta;
    AedilisConfiguratio* configuratio;
         AedilisFructus* fructus;
           AedilisSilva* extractor;
                 chorda  corpus_dir;
                 chorda  causa;
                 chorda  scopus;
                 chorda  varians;
                 chorda  manifestum;
                 chorda  via_manifesti;
              character* scopus_cstr;
     constans character* varians_cstr;
                clock_t  initium;
                clock_t  finis;

    si (provenientia_respondere(numerus_argumentorum, argumenta_cruda,
            &provenientia_aedilis))
    {
        redde ZEPHYRUM;
    }
    piscina = piscina_generare_dynamicum("aedilis", 16777216);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: piscina deest\n");
        redde 1;
    }
    causa.datum    = NIHIL;
    causa.mensura  = 0;

    parser = argumenta_creare(piscina);
    argumenta_ponere_descriptionem(parser,
        "aedilis - clausura dependentiarum derivata + manifestum");
    argumenta_addere_optionem(parser, NIHIL, "--varians",
        "Varians platformae (ordinarie praelatio configurationis)");
    argumenta_addere_vexillum(parser, NIHIL, "--solitarius",
        "Etiam scriptum hermeticum emittere");
    argumenta_addere_vexillum(parser, NIHIL, "--currere",
        "Scriptum emissum statim exsequi");
    argumenta_addere_optionem(parser, NIHIL, "--scribere",
        "Scriptum etiam ad viam datam servare");
    argumenta_addere_vexillum(parser, NIHIL, "--differentia",
        "Sextum capitum contra clang -MM comparare (sine emissione)");
    argumenta_addere_optionem(parser, NIHIL, "--memoria-oraculi",
        "Cum --differentia: effusiones clang -MM per fontem in "
        "directorio dato servare/legere (cursus unus portae)");
    argumenta_addere_vexillum(parser, NIHIL, "--enumerare",
        "Obiecta clausurae nuda imprimere (consumptoribus)");
    argumenta_addere_optionem(parser, NIHIL, "--manifestum",
        "Cum --enumerare aut --partes: manifestum etiam ad viam datam "
        "scribere (fabrica: clausura actionis memorabilis)");
    argumenta_addere_vexillum(parser, NIHIL, "--partes",
        "Partes fructus ut TSV imprimere (O/C/S/V via)");
    argumenta_addere_optionem(parser, NIHIL, "--corpus",
        "Directorium: clausurae OMNIUM fontium .c eius (sectiones F via; cum --partes)");
    argumenta_addere_vexillum(parser, NIHIL, "--nexus-purus",
        "Capita <aedilis nexus=\"purus\"/> radicum inclusarum iudicare "
        "(clausura sine regula nexus; eventus A1b)");
    argumenta_addere_optionem(parser, NIHIL, "--thesaurus",
        "Directorium thesauri: recorda extractionis per sigillum "
        "fontis trans cursus (build/aedilis/obiecta)");
    argumenta_addere_vexillum(parser, NIHIL, "--aristae",
        "Aristas graphi inclusionum imprimere (includens inclusum)");
    argumenta_addere_vexillum(parser, NIHIL, "--ordo",
        "Capita ordine topologico imprimere (cyclus = recusatio)");
    argumenta_addere_exemplum(parser, "aedilis lib/hospitium.c");
    argumenta_addere_exemplum(parser,
        "aedilis probationes/probatio_stml.c --currere");
    argumenta_addere_exemplum(parser,
        "aedilis lib/hospitium.c --differentia");
    lecta = argumenta_parsere(parser, (i32)numerus_argumentorum,
        (constans character* constans*)argumenta_cruda);

    via_manifesti = argumenta_obtinere_optionem(lecta,
        "--manifestum", piscina);
    si (   via_manifesti.mensura > 0
        && !argumenta_habet_vexillum(lecta, "--enumerare")
        && !argumenta_habet_vexillum(lecta, "--partes"))
    {
        fprintf(stderr, "usus: aedilis <fons.c> --enumerare|--partes "
            "--manifestum <via>\n");
        redde 1;
    }
    corpus_dir = argumenta_obtinere_optionem(lecta, "--corpus",
        piscina);
    si (argumenta_habet_vexillum(lecta, "--nexus-purus"))
    {
        scopus.datum    = NIHIL;
        scopus.mensura  = 0;
        scopus_cstr     = NIHIL;
    }
    alioquin si (corpus_dir.mensura > 0)
    {
        si (   argumenta_numerus_positionalium(lecta) != 0
            || !argumenta_habet_vexillum(lecta, "--partes"))
        {
            fprintf(stderr,
                "usus: aedilis --corpus <directorium> --partes\n");
            redde 1;
        }
        scopus.datum    = NIHIL;
        scopus.mensura  = 0;
        scopus_cstr     = NIHIL;
    }
    alioquin
    {
        si (argumenta_numerus_positionalium(lecta) != 1)
        {
            fprintf(stderr,
                "usus: aedilis <fons.c> [--varians <verbum>] | --corpus <dir> --partes\n");
            redde 1;
        }
        scopus       = argumenta_obtinere_positionalem(lecta, 0,
            piscina);
        scopus_cstr  = chorda_ut_cstr(scopus, piscina);
    }
    varians = argumenta_obtinere_optionem(lecta, "--varians",
        piscina);
    varians_cstr = (varians.mensura > 0)
        ? chorda_ut_cstr(varians, piscina) : NIHIL;

    si (   !filum_directorium_creare_si_necesse("build")
        || !filum_directorium_creare_si_necesse("build/aedilis"))
    {
        fprintf(stderr,
            "AEDILIS RECUSAT: build/aedilis non creatum\n");
        redde 1;
    }

    configuratio = aedilis_configurationem_legere(piscina,
        "aedilis.stml", &causa);
    si (configuratio == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: %.*s\n",
            (s32)causa.mensura, (constans character*)causa.datum);
        redde 1;
    }

    {
                    chorda  memoria;
                    chorda  radix_thesauri;
        constans character* memoria_cstr;
        constans character* radix_cstr;
                  Sigillum  praefixum;

        memoria = argumenta_obtinere_optionem(lecta,
            "--memoria-oraculi", piscina);
        memoria_cstr = (memoria.mensura > 0)
            ? chorda_ut_cstr(memoria, piscina) : NIHIL;
        radix_thesauri = argumenta_obtinere_optionem(lecta,
            "--thesaurus", piscina);
        radix_cstr = NIHIL;
        memset(&praefixum, ZEPHYRUM, magnitudo(praefixum));
        si (radix_thesauri.mensura > 0)
        {
            /* versio extractoris = binarium ipsum (argv[0]): quaevis
             * mutatio codicis aut silvae clavem mutat. Binarium
             * illegibile (PATH sine '/') = thesaurus RECUSATUR. */
                       chorda binarium;
                       chorda configuratio_octeti;
            SigillumContextus contextus;

            binarium = filum_legere_totum(argumenta_cruda[0], piscina);
            configuratio_octeti = filum_legere_totum("aedilis.stml",
                piscina);
            si (   binarium.mensura            == ZEPHYRUM
                || configuratio_octeti.mensura == ZEPHYRUM)
            {
                fprintf(stderr,
                    "AEDILIS RECUSAT: --thesaurus: binarium "
                    "(%s), aedilis.stml aut thesaurus legi nequit\n",
                    argumenta_cruda[0]);
                redde 1;
            }
            sigillum_incipere(&contextus);
            sigillum_addere(&contextus, binarium.datum,
                (memoriae_index)binarium.mensura);
            sigillum_addere(&contextus, configuratio_octeti.datum,
                (memoriae_index)configuratio_octeti.mensura);
            praefixum   = sigillum_finire(&contextus);
            radix_cstr  = chorda_ut_cstr(radix_thesauri, piscina);
        }
        /* extractor (lib/aedilis_silva.c, fabrica-6 T6): silva, -MM,
         * memoria per cursum, thesaurus */
        extractor = aedilis_silva_creare(piscina, configuratio,
            radix_cstr, &praefixum, memoria_cstr);
        si (extractor == NIHIL)
        {
            fprintf(stderr, "AEDILIS RECUSAT: extractor parari nequit "
                "(contextus silvae aut thesaurus %s)\n",
                radix_cstr != NIHIL ? radix_cstr : "-");
            redde 1;
        }
    }
    si (argumenta_habet_vexillum(lecta, "--nexus-purus"))
    {
        redde _nexum_purum_currere(piscina, configuratio, extractor);
    }
    si (scopus_cstr == NIHIL)
    {
        redde _corpus_currere(piscina, configuratio,
            chorda_ut_cstr(corpus_dir, piscina), varians_cstr,
            extractor);
    }
    initium = clock();
    fructus = aedilis_derivare(piscina, configuratio, scopus_cstr,
        varians_cstr, aedilis_silva_extrahere, extractor, &causa);
    finis = clock();
    si (fructus == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: %.*s\n",
            (s32)causa.mensura, (constans character*)causa.datum);
        redde 1;
    }
    /* inclusiones citatae inresolutae: manifestum INCOMPLETUM.
     * Nominantur, numquam recusantur (decretum phasis A) - fabrica
     * eas iudicat (P1). Exitus immutatus. */
    {
        i32 i;
        i32 numerus;

        numerus = xar_numerus(fructus->inresolutae);
        per (i = 0; i < numerus; i++)
        {
            chorda* via;

            via = (chorda*)xar_obtinere(fructus->inresolutae, i);
            fprintf(stderr, "AEDILIS CAUTIO: inclusio citata "
                "inresoluta \"%.*s\" (scopus %s)\n",
                (s32)via->mensura, (constans character*)via->datum,
                scopus_cstr);
        }
    }

    si (argumenta_habet_vexillum(lecta, "--differentia"))
    {
        redde _differentiam_currere(piscina, extractor,
            fructus, scopus_cstr);
    }

    si (argumenta_habet_vexillum(lecta, "--enumerare"))
    {
        i32 i;
        i32 numerus;

        si (!_manifestum_scribere_si_petitum(fructus, via_manifesti,
                piscina))
        {
            redde 1;
        }
        numerus = xar_numerus(fructus->obiecta);
        per (i = 0; i < numerus; i++)
        {
            AedilisObiectum* obiectum;

            obiectum = (AedilisObiectum*)xar_obtinere(
                fructus->obiecta, i);
            imprimere("%.*s\n", (s32)obiectum->via.mensura,
                (constans character*)obiectum->via.datum);
        }
        redde 0;
    }

    si (argumenta_habet_vexillum(lecta, "--partes"))
    {
        /* amalgama fontium (parcum …AR15): clausura radicis ut
         * ingressus actionis memorabilis */
        si (!_manifestum_scribere_si_petitum(fructus, via_manifesti,
                piscina))
        {
            redde 1;
        }
        _partes_imprimere(fructus);
        redde 0;
    }

    si (argumenta_habet_vexillum(lecta, "--aristae"))
    {
        i32 i;
        i32 numerus;

        numerus = xar_numerus(fructus->capita);
        per (i = 0; i < numerus; i++)
        {
            AedilisCaput* caput;
                     i32  k;
                     i32  numerus_aristarum;

            caput = (AedilisCaput*)xar_obtinere(fructus->capita,
                i);
            si (caput->inclusa == NIHIL)
            {
                perge;
            }
            numerus_aristarum = xar_numerus(caput->inclusa);
            per (k = 0; k < numerus_aristarum; k++)
            {
                chorda inclusum;

                inclusum = *(chorda*)xar_obtinere(caput->inclusa,
                    k);
                imprimere("%.*s\t%.*s\n",
                    (s32)caput->via.mensura,
                    (constans character*)caput->via.datum,
                    (s32)inclusum.mensura,
                    (constans character*)inclusum.datum);
            }
        }
        redde 0;
    }

    si (argumenta_habet_vexillum(lecta, "--ordo"))
    {
        Xar* ordinati;
        i32  i;
        i32  numerus;

        ordinati = aedilis_capita_ordinare(fructus, piscina,
            &causa);
        si (ordinati == NIHIL)
        {
            fprintf(stderr, "AEDILIS RECUSAT: %.*s\n",
                (s32)causa.mensura,
                (constans character*)causa.datum);
            redde 1;
        }
        numerus = xar_numerus(ordinati);
        per (i = 0; i < numerus; i++)
        {
            AedilisCaput* caput;

            caput = *(AedilisCaput**)xar_obtinere(ordinati, i);
            imprimere("%.*s\n", (s32)caput->via.mensura,
                (constans character*)caput->via.datum);
        }
        redde 0;
    }

    manifestum = aedilis_manifestum_scribere(fructus, piscina,
        _commissum_obtinere(piscina));

    {
                   chorda  basis;
                   chorda  directorium;
                   chorda  via_manifesti;
        ChordaAedificator* aedificator;

        basis = via_nomen_radix(via_nomen(scopus, piscina),
            piscina);
        aedificator = chorda_aedificator_creare(piscina, 128);
        chorda_aedificator_appendere_literis(aedificator,
            "build/aedilis/");
        chorda_aedificator_appendere_chorda(aedificator, basis);
        directorium = chorda_aedificator_finire(aedificator);
        si (!filum_directorium_creare_si_necesse(
                chorda_ut_cstr(directorium, piscina)))
        {
            fprintf(stderr,
                "AEDILIS RECUSAT: directorium manifesti\n");
            redde 1;
        }
        aedificator = chorda_aedificator_creare(piscina, 160);
        chorda_aedificator_appendere_chorda(aedificator,
            directorium);
        chorda_aedificator_appendere_literis(aedificator,
            "/manifestum.stml");
        via_manifesti = chorda_aedificator_finire(aedificator);
        si (!filum_scribere(chorda_ut_cstr(via_manifesti,
                piscina), manifestum))
        {
            fprintf(stderr,
                "AEDILIS RECUSAT: manifestum non scriptum\n");
            redde 1;
        }

        imprimere("AEDILIS: %s varians=%.*s\n", scopus_cstr,
            (s32)fructus->varians.mensura,
            (constans character*)fructus->varians.datum);
        imprimere(
            "obiecta %u | capita %u | systemata %u | vendores %u"
            " | tempus %.0f ms\n",
            xar_numerus(fructus->obiecta),
            xar_numerus(fructus->capita),
            xar_numerus(fructus->systemata),
            xar_numerus(fructus->vendores),
            (f64)(finis - initium) * 1000.0
                / (f64)CLOCKS_PER_SEC);
        imprimere("manifestum: %.*s\n",
            (s32)via_manifesti.mensura,
            (constans character*)via_manifesti.datum);

        /* scripta emissa ex manifesti veritate */
        {
                        chorda  scriptum;
                        chorda  via_scripti;
            constans character* commissum;

            commissum = _commissum_obtinere(piscina);
            scriptum = aedilis_scriptum_scribere(fructus,
                configuratio, piscina, FALSUM, commissum);
            aedificator = chorda_aedificator_creare(piscina, 160);
            chorda_aedificator_appendere_chorda(aedificator,
                directorium);
            chorda_aedificator_appendere_literis(aedificator,
                "/struere.sh");
            via_scripti = chorda_aedificator_finire(aedificator);
            si (!filum_scribere(chorda_ut_cstr(via_scripti,
                    piscina), scriptum))
            {
                fprintf(stderr,
                    "AEDILIS RECUSAT: scriptum non scriptum\n");
                redde 1;
            }
            imprimere("scriptum: %.*s\n",
                (s32)via_scripti.mensura,
                (constans character*)via_scripti.datum);

            si (argumenta_habet_vexillum(lecta, "--solitarius"))
            {
                chorda solitarium;
                chorda via_solitarii;

                solitarium = aedilis_scriptum_scribere(fructus,
                    configuratio, piscina, VERUM, commissum);
                aedificator = chorda_aedificator_creare(piscina,
                    160);
                chorda_aedificator_appendere_chorda(aedificator,
                    directorium);
                chorda_aedificator_appendere_literis(aedificator,
                    "/struere_solitarius.sh");
                via_solitarii = chorda_aedificator_finire(
                    aedificator);
                si (!filum_scribere(chorda_ut_cstr(via_solitarii,
                        piscina), solitarium))
                {
                    fprintf(stderr, "AEDILIS RECUSAT:"
                        " scriptum solitarium non scriptum\n");
                    redde 1;
                }
                imprimere("scriptum solitarium: %.*s\n",
                    (s32)via_solitarii.mensura,
                    (constans character*)via_solitarii.datum);
            }

            {
                chorda via_servandi;

                via_servandi = argumenta_obtinere_optionem(lecta,
                    "--scribere", piscina);
                si (   via_servandi.mensura > 0
                    && !filum_scribere(chorda_ut_cstr(
                            via_servandi, piscina), scriptum))
                {
                    fprintf(stderr, "AEDILIS RECUSAT:"
                        " scriptum non servatum\n");
                    redde 1;
                }
            }

            si (argumenta_habet_vexillum(lecta, "--currere"))
            {
                ChordaAedificator* mandatum;

                mandatum = chorda_aedificator_creare(piscina,
                    192);
                chorda_aedificator_appendere_literis(mandatum,
                    "bash ");
                chorda_aedificator_appendere_chorda(mandatum,
                    via_scripti);
                redde system(chorda_ut_cstr(
                    chorda_aedificator_finire(mandatum),
                    piscina)) == 0 ? 0 : 1;
            }
        }
    }

    redde 0;
}
