/* aedilis.c - AEDILIS CLI (bin/aedilis) - Phasis A
 *
 * Machina in lib/aedilis.c vivit (pura, sutura extractoris);
 * hic vivunt: extractor silvae (.c/.h), cursus minoritatis -MM
 * (.m - clang oraculum per system() + plagulam temporalem),
 * provenientia git, emissio manifesti.
 *
 * Usus: bin/aedilis <fons.c> [--varians <verbum[,verbum...]>]
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
/* EFFUSIONES (2026-10-22): forma cuiusque modi definitio UNA -
 * appendere in aedificatorem; vocationes singulae, corpus et --plures
 * eadem functione utuntur, ergo octeti numquam divergunt. */
interior vacuum
_lineam_appendere (
     ChordaAedificator* aed,
    constans character* praefixum,
                chorda  via)
{
    chorda_aedificator_appendere_literis(aed, praefixum);
    chorda_aedificator_appendere_chorda(aed, via);
    chorda_aedificator_appendere_character(aed, '\n');
}

/* --enumerare: obiecta clausurae nuda */
interior vacuum
_obiecta_appendere (
    constans AedilisFructus* fructus,
          ChordaAedificator* aed)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(fructus->obiecta);
    per (i = 0; i < numerus; i++)
    {
        AedilisObiectum* obiectum;

        obiectum = (AedilisObiectum*)xar_obtinere(fructus->obiecta, i);
        _lineam_appendere(aed, "", obiectum->via);
    }
}

/* --partes: TSV (O/C/S/V via) */
interior vacuum
_partes_appendere (
    constans AedilisFructus* fructus,
          ChordaAedificator* aed)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(fructus->obiecta);
    per (i = 0; i < numerus; i++)
    {
        AedilisObiectum* obiectum;

        obiectum = (AedilisObiectum*)xar_obtinere(fructus->obiecta, i);
        _lineam_appendere(aed, "O\t", obiectum->via);
    }
    numerus = xar_numerus(fructus->capita);
    per (i = 0; i < numerus; i++)
    {
        AedilisCaput* caput;

        caput = (AedilisCaput*)xar_obtinere(fructus->capita, i);
        _lineam_appendere(aed, "C\t", caput->via);
    }
    numerus = xar_numerus(fructus->systemata);
    per (i = 0; i < numerus; i++)
    {
        _lineam_appendere(aed, "S\t",
            *(chorda*)xar_obtinere(fructus->systemata, i));
    }
    numerus = xar_numerus(fructus->vendores);
    per (i = 0; i < numerus; i++)
    {
        AedilisVendor* vendor;

        vendor = (AedilisVendor*)xar_obtinere(fructus->vendores, i);
        _lineam_appendere(aed, "V\t", vendor->fons);
    }
}

/* --aristae: arista graphi inclusionum (includens TAB inclusum) */
interior vacuum
_aristas_appendere (
    constans AedilisFructus* fructus,
          ChordaAedificator* aed)
{
    i32 i;
    i32 numerus;

    numerus = xar_numerus(fructus->capita);
    per (i = 0; i < numerus; i++)
    {
        AedilisCaput* caput;
                 i32  k;
                 i32  numerus_aristarum;

        caput = (AedilisCaput*)xar_obtinere(fructus->capita, i);
        si (caput->inclusa == NIHIL)
        {
            perge;
        }
        numerus_aristarum = xar_numerus(caput->inclusa);
        per (k = 0; k < numerus_aristarum; k++)
        {
            chorda_aedificator_appendere_chorda(aed, caput->via);
            chorda_aedificator_appendere_character(aed, '\t');
            chorda_aedificator_appendere_chorda(aed,
                *(chorda*)xar_obtinere(caput->inclusa, k));
            chorda_aedificator_appendere_character(aed, '\n');
        }
    }
}

/* aedificatorem ad stdout (per stdio, ordine cum imprimere servato) */
interior vacuum
_effusionem_emittere (
    ChordaAedificator* aed)
{
    chorda effusio;

    effusio = chorda_aedificator_finire(aed);
    si (effusio.mensura > 0)
    {
        (vacuum)fwrite(effusio.datum, I, (size_t)effusio.mensura,
            stdout);
    }
}

interior vacuum
_partes_imprimere (
    constans AedilisFructus* fructus,
                    Piscina* piscina)
{
    ChordaAedificator* aed;

    aed = chorda_aedificator_creare(piscina, MXXIV);
    _partes_appendere(fructus, aed);
    _effusionem_emittere(aed);
}

/* SCRIBERE SI MUTATUM (2026-10-23): manifestum idem non rescribitur -
 * mtime manet. Olim omnis derivatio manifestum rescribebat; actio
 * fabricae quae aedilem currit (speculum_hospes) arborem ideo semper
 * 'mutabat', et memoriae regenerationum fabricae vacabantur
 * (generatores bis per cursum radicis iudicati). */
interior b32
_scribere_si_mutatum (
    constans character* via,
                chorda  contentum,
               Piscina* piscina)
{
    chorda vetus;

    vetus = filum_legere_totum(via, piscina);
    si (   vetus.datum   != NIHIL
        && vetus.mensura == contentum.mensura
        && (   contentum.mensura == 0
            || memcmp(vetus.datum, contentum.datum,
                   (memoriae_index)contentum.mensura) == 0))
    {
        redde VERUM;
    }
    redde filum_scribere(via, contentum);
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
        || !_scribere_si_mutatum(chorda_ut_cstr(via_manifesti, piscina),
               aedilis_manifestum_scribere(fructus, piscina, NIHIL),
               piscina))
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

/* PLURES (2026-10-22): clausurae MULTORUM fontium UNO processu - fons
 * per lineam in plagula 'via_listae'. Extractor (memoria per cursum
 * + thesaurus) COMMUNIS est; fructus quisque in piscina SUA (destruitur
 * post fontem - extractor memoriam suam in piscina sua longaeva servat,
 * piscinam vocationis ignorat). Fons quisque SEMEL derivatur, deinde
 * modi petiti (--enumerare, --partes, --aristae) ex eodem fructu
 * scribuntur: modus cum '--partes-ad' / '--aristae-ad' in plagulam
 * suam, ceteri (unus summum) in stdout. Effusio modi cuiusque =
 * concatenatio effusionum singularum ordine listae, octetim (eaedem
 * functiones _*_appendere). Manifestum fontis quisque ad
 * '<dir>/<via cum / -> __>.stml'. Causae: compile_tests CCCLXXXI
 * processus (~XXXVIII s frigide); amalgama_fontes radicem quamque BIS
 * (partes, aristae) sine thesauro derivabat (silva XXIII radices,
 * XLVI processus, ~XX s). */
interior integer
_plures_currere (
                         Piscina* piscina,
    constans AedilisConfiguratio* configuratio,
              constans character* via_listae,
                          chorda  dir_manifestorum,
              constans character* varians_cstr,
                    AedilisSilva* extractor,
                             b32  enumerare,
                             b32  partes,
                          chorda  partes_ad,
                             b32  aristae,
                          chorda  aristae_ad)
{
               chorda  lista;
                  i32  i;
                  i32  initium_lineae;
    ChordaAedificator* aed_partium    = NIHIL;
    ChordaAedificator* aed_aristarum  = NIHIL;

    lista = filum_legere_totum(via_listae, piscina);
    si (lista.datum == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: --plures: %s legi nequit\n",
            via_listae);
        redde 1;
    }
    si (partes && partes_ad.mensura > 0)
    {
        aed_partium = chorda_aedificator_creare(piscina, LXIV * MXXIV);
    }
    si (aristae && aristae_ad.mensura > 0)
    {
        aed_aristarum = chorda_aedificator_creare(piscina,
            LXIV * MXXIV);
    }
    initium_lineae = 0;
    per (i = 0; i <= lista.mensura; i++)
    {
                 Piscina* scopi_piscina;
          AedilisFructus* fructus;
       ChordaAedificator* aed_stdout;
                  chorda  causa;
                  chorda  scopus;
               character* scopus_cstr;
                     i32  k;
                     i32  numerus;

        si (i < lista.mensura && lista.datum[i] != '\n')
        {
            perge;
        }
        scopus.datum    = lista.datum + initium_lineae;
        scopus.mensura  = i - initium_lineae;
        initium_lineae  = i + 1;
        dum (   scopus.mensura > 0
             && (   scopus.datum[scopus.mensura - 1] == '\r'
                 || scopus.datum[scopus.mensura - 1] == ' '))
        {
            scopus.mensura--;
        }
        si (scopus.mensura == 0)
        {
            perge;
        }
        scopi_piscina = piscina_generare_dynamicum("aedilis_scopus",
            CCLVI * MXXIV);
        si (scopi_piscina == NIHIL)
        {
            redde 1;
        }
        scopus_cstr    = chorda_ut_cstr(scopus, scopi_piscina);
        causa.datum    = NIHIL;
        causa.mensura  = 0;
        fructus = aedilis_derivare(scopi_piscina, configuratio,
            scopus_cstr, varians_cstr, aedilis_silva_extrahere,
            extractor, &causa);
        si (fructus == NIHIL)
        {
            fprintf(stderr, "AEDILIS RECUSAT: %s: %.*s\n", scopus_cstr,
                (s32)causa.mensura, (constans character*)causa.datum);
            piscina_destruere(scopi_piscina);
            redde 1;
        }
        numerus = xar_numerus(fructus->inresolutae);
        per (k = 0; k < numerus; k++)
        {
            chorda* via;

            via = (chorda*)xar_obtinere(fructus->inresolutae, k);
            fprintf(stderr, "AEDILIS CAUTIO: inclusio citata "
                "inresoluta \"%.*s\" (scopus %s)\n",
                (s32)via->mensura, (constans character*)via->datum,
                scopus_cstr);
        }
        si (dir_manifestorum.mensura > 0)
        {
            ChordaAedificator* nomen_manifesti;
                          i32  j;

            nomen_manifesti = chorda_aedificator_creare(scopi_piscina,
                CCLVI);
            chorda_aedificator_appendere_chorda(nomen_manifesti,
                dir_manifestorum);
            chorda_aedificator_appendere_character(nomen_manifesti,
                '/');
            per (j = 0; j < scopus.mensura; j++)
            {
                si (scopus.datum[j] == '/')
                {
                    chorda_aedificator_appendere_literis(nomen_manifesti,
                        "__");
                }
                alioquin
                {
                    chorda_aedificator_appendere_character(
                        nomen_manifesti, (character)scopus.datum[j]);
                }
            }
            chorda_aedificator_appendere_literis(nomen_manifesti,
                ".stml");
            si (!_manifestum_scribere_si_petitum(fructus,
                    chorda_aedificator_finire(nomen_manifesti),
                    scopi_piscina))
            {
                piscina_destruere(scopi_piscina);
                redde 1;
            }
        }
        aed_stdout = chorda_aedificator_creare(scopi_piscina, MXXIV);
        si (enumerare)
        {
            _obiecta_appendere(fructus, aed_stdout);
        }
        si (partes)
        {
            _partes_appendere(fructus,
                aed_partium != NIHIL ? aed_partium : aed_stdout);
        }
        si (aristae)
        {
            _aristas_appendere(fructus,
                aed_aristarum != NIHIL ? aed_aristarum : aed_stdout);
        }
        _effusionem_emittere(aed_stdout);
        piscina_destruere(scopi_piscina);
    }
    si (   aed_partium != NIHIL
        && !filum_scribere(chorda_ut_cstr(partes_ad, piscina),
               chorda_aedificator_finire(aed_partium)))
    {
        fprintf(stderr, "AEDILIS RECUSAT: --partes-ad non scriptum\n");
        redde 1;
    }
    si (   aed_aristarum != NIHIL
        && !filum_scribere(chorda_ut_cstr(aristae_ad, piscina),
               chorda_aedificator_finire(aed_aristarum)))
    {
        fprintf(stderr, "AEDILIS RECUSAT: --aristae-ad non scriptum\n");
        redde 1;
    }
    redde 0;
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
        _partes_imprimere(fructus, piscina);
    }
    redde exitus;
}

externus constans ProvenientiaRelatio provenientia_aedilis;

/* '--clavis-thesauri': clavis thesauri extractoris et fons eius
 * ("digestum" | "binarium") - fumus fabricae XLV binaria duo
 * comparat */
interior s32
_clavem_thesauri_imprimere (
    constans character* via_binarii)
{
                Piscina* piscina = piscina_generare_dynamicum("clavis",
                    65536);
               Sigillum  praefixum;
     constans character* fons = NIHIL;
              character  hex[SIGILLUM_HEX_MENSURA];

    si (   piscina == NIHIL
        || !aedilis_thesauri_praefixum(provenientia_aedilis.ingressus,
               via_binarii, "aedilis.stml", piscina, &praefixum, &fons))
    {
        fprintf(stderr, "AEDILIS RECUSAT: clavis thesauri computari "
            "nequit (aedilis.stml aut binarium)\n");
        redde 1;
    }
    sigillum_hex(&praefixum, hex);
    printf("%s\t%s\n", hex, fons);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}

/* --radices (cursores ad gradus T6): radices inclusae aedilis.stml
 * (sectio <inclusa>, ordine), una per lineam, radici relativae - FONS
 * UNUS radicum pro scriptis quae clang ipsa vocant (tools/vexilla.sh
 * vexilla_radices). Lector verus configurationis, non grep. Exitus 1 si
 * aedilis.stml legi nequit. */
interior s32
_radices_imprimere (vacuum)
{
                Piscina* piscina;
    AedilisConfiguratio* configuratio;
                 chorda  causa;
                    i32  i;

    piscina = piscina_generare_dynamicum("aedilis_radices", 262144);
    si (piscina == NIHIL)
    {
        redde 1;
    }
    causa.datum    = NIHIL;
    causa.mensura  = 0;
    configuratio = aedilis_configurationem_legere(piscina,
        "aedilis.stml", &causa);
    si (configuratio == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: %.*s\n", (s32)causa.mensura,
            (constans character*)causa.datum);
        piscina_destruere(piscina);
        redde 1;
    }
    aedilis_variantes_hospiti_aptare(configuratio, piscina);
    per (i = ZEPHYRUM; i < xar_numerus(configuratio->inclusa); i++)
    {
        chorda radix = *(chorda*)xar_obtinere(configuratio->inclusa, i);

        printf("%.*s\n", (s32)radix.mensura,
            (constans character*)radix.datum);
    }
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}

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
    si (   numerus_argumentorum                            == II
        && strcmp(argumenta_cruda[1], "--clavis-thesauri") == 0)
    {
        redde _clavem_thesauri_imprimere(argumenta_cruda[0]);
    }
    si (   numerus_argumentorum                    == II
        && strcmp(argumenta_cruda[1], "--radices") == 0)
    {
        redde _radices_imprimere();
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
        "Varians aut index ordine praelationis (android,posix);"
        " ordinarie praelatio configurationis");
    argumenta_addere_vexillum(parser, NIHIL, "--solitarius",
        "Etiam scriptum hermeticum emittere");
    argumenta_addere_vexillum(parser, NIHIL, "--currere",
        "Scriptum emissum statim exsequi");
    argumenta_addere_optionem(parser, NIHIL, "--scribere",
        "Scriptum etiam ad viam datam servare");
    argumenta_addere_optionem(parser, NIHIL, "--obiecta",
        "Thesaurus obiectorum scripti pro build/aedilis/obiecta (porta "
        "instrumentorum: obiecta communia non tanguntur)");
    argumenta_addere_optionem(parser, NIHIL, "--exitus",
        "Directorium manifesti, scriptorum et binarii pro "
        "build/aedilis/<basis> (porta instrumentorum: manifesta "
        "basium communium non obruuntur)");
    argumenta_addere_vexillum(parser, NIHIL, "--differentia",
        "Sextum capitum contra clang -MM comparare (sine emissione)");
    argumenta_addere_optionem(parser, NIHIL, "--memoria-oraculi",
        "Cum --differentia: effusiones clang -MM per fontem in "
        "directorio dato servare/legere (cursus unus portae)");
    argumenta_addere_vexillum(parser, NIHIL, "--enumerare",
        "Obiecta clausurae nuda imprimere (consumptoribus)");
    argumenta_addere_optionem(parser, NIHIL, "--plures",
        "Cum --enumerare: fontes ex plagula data (unus per lineam) UNO "
        "processu; effusio = concatenatio effusionum singularum");
    argumenta_addere_optionem(parser, NIHIL, "--partes-ad",
        "Cum --plures --partes: partes in plagulam datam "
        "(non stdout)");
    argumenta_addere_optionem(parser, NIHIL, "--aristae-ad",
        "Cum --plures --aristae: aristae in plagulam datam "
        "(non stdout)");
    argumenta_addere_optionem(parser, NIHIL, "--manifesta",
        "Cum --plures: manifestum fontis cuiusque ad <dir>/<via cum / -> "
        "__>.stml");
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
    argumenta_addere_vexillum(parser, NIHIL, "--nexus",
        "Vexilla nexus clausurae imprimere, unum per lineam "
        "(regulae aedilis.stml + annotata, deduplicata)");
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
    alioquin si (argumenta_obtinere_optionem(lecta, "--plures",
                     piscina).mensura > 0)
    {
        b32 modus_enumerandi;
        b32 modus_partium;
        b32 modus_aristarum;
        i32 ad_stdout;

        modus_enumerandi = argumenta_habet_vexillum(lecta,
            "--enumerare");
        modus_partium    = argumenta_habet_vexillum(lecta, "--partes");
        modus_aristarum  = argumenta_habet_vexillum(lecta, "--aristae");
        ad_stdout        = modus_enumerandi ? 1 : 0;
        si (   modus_partium
            && argumenta_obtinere_optionem(lecta, "--partes-ad",
                   piscina).mensura == 0)
        {
            ad_stdout++;
        }
        si (   modus_aristarum
            && argumenta_obtinere_optionem(lecta, "--aristae-ad",
                   piscina).mensura == 0)
        {
            ad_stdout++;
        }
        si (   argumenta_numerus_positionalium(lecta) != 0
            || (!modus_enumerandi && !modus_partium && !modus_aristarum)
            || ad_stdout > 1
            || via_manifesti.mensura > 0)
        {
            fprintf(stderr, "usus: aedilis --plures <lista> "
                "[--enumerare] [--partes [--partes-ad <via>]] "
                "[--aristae [--aristae-ad <via>]] [--manifesta <dir>] "
                "[--thesaurus <dir>] (stdout modo uno summum)\n");
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
                "usus: aedilis <fons.c> [--varians <verbum[,verbum...]>] | --corpus <dir> --partes | --radices\n");
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
    aedilis_variantes_hospiti_aptare(configuratio, piscina);

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
            /* versio extractoris = digestum actionis 'aedilis' in
             * aedificatione infixum (provenientia sua; circulus
             * commissionis P1) - bin/fabrica eandem clavem habet; sine
             * digesto (bootstrap) binarium ipsum (argv[0]) ut olim.
             * Illegibile = thesaurus RECUSATUR. */
            si (!aedilis_thesauri_praefixum(
                    provenientia_aedilis.ingressus, argumenta_cruda[0],
                    "aedilis.stml", piscina, &praefixum, NIHIL))
            {
                fprintf(stderr,
                    "AEDILIS RECUSAT: --thesaurus: binarium "
                    "(%s), aedilis.stml aut thesaurus legi nequit\n",
                    argumenta_cruda[0]);
                redde 1;
            }
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
    si (   scopus_cstr == NIHIL
        && argumenta_obtinere_optionem(lecta, "--plures",
               piscina).mensura > 0)
    {
        redde _plures_currere(piscina, configuratio,
            chorda_ut_cstr(argumenta_obtinere_optionem(lecta,
            "--plures",
                piscina), piscina),
            argumenta_obtinere_optionem(lecta, "--manifesta", piscina),
            varians_cstr, extractor,
            argumenta_habet_vexillum(lecta, "--enumerare"),
            argumenta_habet_vexillum(lecta, "--partes"),
            argumenta_obtinere_optionem(lecta, "--partes-ad", piscina),
            argumenta_habet_vexillum(lecta, "--aristae"),
            argumenta_obtinere_optionem(lecta, "--aristae-ad",
            piscina));
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
        si (!_manifestum_scribere_si_petitum(fructus, via_manifesti,
                piscina))
        {
            redde 1;
        }
        {
            ChordaAedificator* aed;

            aed = chorda_aedificator_creare(piscina, MXXIV);
            _obiecta_appendere(fructus, aed);
            _effusionem_emittere(aed);
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
        _partes_imprimere(fructus, piscina);
        redde 0;
    }

    /* testa-ios T3: consumptores sine scripto (tools/ios/app.sh) -
     * frameworks testae iOS ex regula 'testa_ios' */
    si (argumenta_habet_vexillum(lecta, "--nexus"))
    {
        Xar* vexilla;
        i32  i;
        i32  numerus;

        vexilla = aedilis_vexilla_nexus(fructus, piscina);
        numerus = xar_numerus(vexilla);
        per (i = 0; i < numerus; i++)
        {
            chorda* v;

            v = (chorda*)xar_obtinere(vexilla, i);
            imprimere("%.*s\n", (s32)v->mensura,
                (constans character*)v->datum);
        }
        redde 0;
    }

    si (argumenta_habet_vexillum(lecta, "--aristae"))
    {
        ChordaAedificator* aed;

        aed = chorda_aedificator_creare(piscina, MXXIV);
        _aristas_appendere(fructus, aed);
        _effusionem_emittere(aed);
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
                   chorda  exitus;
                   chorda  directorium;
                   chorda  via_manifesti;
        ChordaAedificator* aedificator;

        basis = via_nomen_radix(via_nomen(scopus, piscina),
            piscina);
        exitus = argumenta_obtinere_optionem(lecta, "--exitus",
            piscina);
        aedificator = chorda_aedificator_creare(piscina, 128);
        si (exitus.mensura > 0)
        {
            chorda_aedificator_appendere_chorda(aedificator, exitus);
        }
        alioquin
        {
            chorda_aedificator_appendere_literis(aedificator,
                "build/aedilis/");
            chorda_aedificator_appendere_chorda(aedificator, basis);
        }
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
        si (!_scribere_si_mutatum(chorda_ut_cstr(via_manifesti,
                piscina), manifestum, piscina))
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
                        chorda  obiecta;
            constans character* commissum;

            commissum = _commissum_obtinere(piscina);
            obiecta = argumenta_obtinere_optionem(lecta, "--obiecta",
                piscina);
            scriptum = aedilis_scriptum_scribere_ad(fructus,
                configuratio, piscina, FALSUM, commissum,
                exitus.mensura > 0 ? chorda_ut_cstr(exitus, piscina)
                : NIHIL, obiecta.mensura > 0 ? chorda_ut_cstr(obiecta,
                piscina) : NIHIL);
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

                solitarium = aedilis_scriptum_scribere_ad(fructus,
                    configuratio, piscina, VERUM, commissum,
                    exitus.mensura > 0 ? chorda_ut_cstr(exitus,
                    piscina) : NIHIL, NIHIL);
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
