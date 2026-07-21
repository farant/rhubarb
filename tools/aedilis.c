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

#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "via.h"
#include "xar.h"
#include "argumenta.h"
#include "aedilis.h"

#include "silva.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

nomen structura {
    SilvaContextus*               contextus;
    constans AedilisConfiguratio* configuratio;
} ExtractorDatum;

/* Chordam ex octetis alienis in piscinam copiare (vistae silvae
 * in piscinam plagulae spectant quae mox destruitur) */
interior chorda
_chordam_copiare (constans i8* datum, i32 mensura,
    Piscina* piscina)
{
    chorda copia;

    copia.mensura = mensura;
    copia.datum = (i8*)piscina_allocare(piscina,
        (memoriae_index)(mensura > 0 ? mensura : 1));
    si (copia.datum != NIHIL && mensura > 0)
    {
        memcpy(copia.datum, datum, (memoriae_index)mensura);
    }
    redde copia;
}

interior vacuum
_chordam_in_xar (Xar* xar, chorda valor)
{
    chorda* locus;

    locus = (chorda*)xar_addere(xar);
    si (locus != NIHIL)
    {
        *locus = valor;
    }
}

/* Annotatio ANCORATA (lectio spicae: substring prosam capiebat):
 * post delimitatorem et spatia commentum cum "aedilis:" INCIPERE
 * debet. Reddit contentum post ancoram, tersum, "*"/"/" caudae
 * demptis; mensura 0 = non annotatio. */
interior chorda
_annotationem_extrahere (constans i8* datum, i32 mensura,
    Piscina* piscina)
{
    constans character* signum = "aedilis:";
    chorda vacua;
    i32 i;
    i32 j;
    i32 finis;

    vacua.datum = NIHIL;
    vacua.mensura = 0;

    i = 0;
    si (mensura >= 2 && datum[0] == (i8)'/'
        && (datum[1] == (i8)'*' || datum[1] == (i8)'/'))
    {
        i = 2;
    }
    dum (i < mensura
        && (datum[i] == (i8)' ' || datum[i] == (i8)'\t'))
    {
        i++;
    }
    per (j = 0; signum[j] != '\0'; j++)
    {
        si (i >= mensura || datum[i] != (i8)signum[j])
        {
            redde vacua;
        }
        i++;
    }
    dum (i < mensura
        && (datum[i] == (i8)' ' || datum[i] == (i8)'\t'))
    {
        i++;
    }
    finis = mensura;
    si (finis >= 2 && datum[finis - 2] == (i8)'*'
        && datum[finis - 1] == (i8)'/')
    {
        finis -= 2;
    }
    dum (finis > i
        && (datum[finis - 1] == (i8)' '
            || datum[finis - 1] == (i8)'\t'
            || datum[finis - 1] == (i8)'\n'
            || datum[finis - 1] == (i8)'\r'))
    {
        finis--;
    }
    si (finis <= i)
    {
        redde vacua;
    }
    redde _chordam_copiare(datum + i, finis - i, piscina);
}

/* Cursus minoritatis: clang -MM per system(), plagula temporalis.
 * Directivae redditae = viae IAM RESOLUTAE (ex_oraculo). */
interior b32
_extractor_oraculi (ExtractorDatum* extractoris,
    constans character* via, Piscina* piscina, Xar** directivae_out)
{
    ChordaAedificator* mandatum;
    chorda             textus;
    character*         mandatum_cstr;
    i32                i;
    i32                numerus;

    mandatum = chorda_aedificator_creare(piscina, 512);
    chorda_aedificator_appendere_literis(mandatum, "clang -MM");
    numerus = xar_numerus(extractoris->configuratio->inclusa);
    per (i = 0; i < numerus; i++)
    {
        chorda inclusum;

        inclusum = *(chorda*)xar_obtinere(
            extractoris->configuratio->inclusa, i);
        chorda_aedificator_appendere_literis(mandatum, " -I");
        chorda_aedificator_appendere_chorda(mandatum, inclusum);
    }
    chorda_aedificator_appendere_literis(mandatum, " ");
    chorda_aedificator_appendere_literis(mandatum, via);
    chorda_aedificator_appendere_literis(mandatum,
        " > build/aedilis/oraculum.tmp 2>/dev/null");
    mandatum_cstr = chorda_ut_cstr(
        chorda_aedificator_finire(mandatum), piscina);

    si (system(mandatum_cstr) != 0)
    {
        redde FALSUM;
    }
    textus = filum_legere_totum("build/aedilis/oraculum.tmp",
        piscina);
    si (textus.mensura == 0)
    {
        redde FALSUM;
    }

    /* forma: "basis.o: fons.m caput.h \\\n caput2.h ..." -
     * praeterire ad ':' primum, deinde signa albospatiata,
     * '\\' continuationes et fontem ipsum demptis */
    i = 0;
    dum (i < textus.mensura && textus.datum[i] != (i8)':')
    {
        i++;
    }
    si (i < textus.mensura)
    {
        i++;
    }
    dum (i < textus.mensura)
    {
        i32 initium;
        chorda signum;

        dum (i < textus.mensura
            && (textus.datum[i] == (i8)' '
                || textus.datum[i] == (i8)'\t'
                || textus.datum[i] == (i8)'\n'
                || textus.datum[i] == (i8)'\r'
                || textus.datum[i] == (i8)'\\'))
        {
            i++;
        }
        initium = i;
        dum (i < textus.mensura && textus.datum[i] != (i8)' '
            && textus.datum[i] != (i8)'\t'
            && textus.datum[i] != (i8)'\n'
            && textus.datum[i] != (i8)'\r'
            && textus.datum[i] != (i8)'\\')
        {
            i++;
        }
        si (i > initium)
        {
            memoriae_index longitudo_viae;

            signum.datum = textus.datum + initium;
            signum.mensura = i - initium;
            /* fontem ipsum praeterire: -MM eum nudum imprimit,
             * via nostra "./" praefixari potest - suffixo
             * congruere */
            longitudo_viae = strlen(via);
            si (longitudo_viae >= (memoriae_index)signum.mensura
                && memcmp(via + longitudo_viae
                        - (memoriae_index)signum.mensura,
                    signum.datum,
                    (memoriae_index)signum.mensura) == 0)
            {
                perge;
            }
            _chordam_in_xar(*directivae_out, signum);
        }
    }
    redde VERUM;
}

interior b32
_extractor_silvae (vacuum* datum, constans character* via,
    Piscina* piscina, Xar** directivae_out, Xar** annotationes_out,
    b32* ex_oraculo_out)
{
    ExtractorDatum* extractoris;
    SilvaPiscina*   arboris;
    SilvaParsura*   parsura;
    SilvaXar*       cruda;
    chorda          fons;
    memoriae_index  longitudo_viae;
    insignatus integer n;
    insignatus integer k;

    extractoris = (ExtractorDatum*)datum;
    *directivae_out = xar_creare(piscina, (i32)magnitudo(chorda));
    *annotationes_out = xar_creare(piscina,
        (i32)magnitudo(chorda));
    *ex_oraculo_out = FALSUM;

    longitudo_viae = strlen(via);
    si (longitudo_viae > 2 && via[longitudo_viae - 2] == '.'
        && via[longitudo_viae - 1] == 'm')
    {
        *ex_oraculo_out = VERUM;
        redde _extractor_oraculi(extractoris, via, piscina,
            directivae_out);
    }

    fons = filum_legere_totum(via, piscina);
    si (fons.mensura == 0)
    {
        redde FALSUM;
    }

    arboris = silva_piscina_generare_dynamicum("aedilis_arbor",
        8388608);
    si (arboris == NIHIL)
    {
        redde FALSUM;
    }
    parsura = silva_c89_parsare_cum_contextu(arboris,
        extractoris->contextus, via,
        (constans character*)fons.datum, fons.mensura, NIHIL);
    si (parsura == NIHIL || parsura->expansio == NIHIL)
    {
        silva_piscina_destruere(arboris);
        redde FALSUM;
    }

    n = silva_inclusiones_numerus(parsura->expansio);
    per (k = 0; k < n; k++)
    {
        SilvaInclusioVista vista;

        si (!silva_inclusio_vista(parsura->expansio, k, &vista))
        {
            perge;
        }
        si (vista.fons_ex != parsura->fons_princeps
            || vista.via == NIHIL)
        {
            perge;
        }
        _chordam_in_xar(*directivae_out,
            _chordam_copiare(vista.via->datum, vista.via->mensura,
                piscina));
    }

    cruda = silva_lexare_cruda(arboris,
        (constans character*)fons.datum, fons.mensura, 0);
    si (cruda != NIHIL)
    {
        n = silva_xar_numerus(cruda);
        per (k = 0; k < n; k++)
        {
            SilvaToken* lexema;
            chorda      annotatio;

            lexema = *(SilvaToken**)silva_xar_obtinere(cruda, k);
            si (lexema == NIHIL
                || (lexema->genus != SILVA_LEX_COMMENTUM_CLAUSUM
                    && lexema->genus != SILVA_LEX_COMMENTUM_LINEA))
            {
                perge;
            }
            annotatio = _annotationem_extrahere(
                lexema->valor.datum, lexema->valor.mensura,
                piscina);
            si (annotatio.mensura > 0)
            {
                _chordam_in_xar(*annotationes_out, annotatio);
            }
        }
    }

    silva_piscina_destruere(arboris);
    redde VERUM;
}

/* Provenientia git (optima conatio; NIHIL si abest) */
interior constans character*
_commissum_obtinere (Piscina* piscina)
{
    chorda textus;
    i32    finis;

    si (system("git rev-parse --short HEAD"
            " > build/aedilis/commissum.tmp 2>/dev/null") != 0)
    {
        redde NIHIL;
    }
    textus = filum_legere_totum("build/aedilis/commissum.tmp",
        piscina);
    finis = textus.mensura;
    dum (finis > 0 && (textus.datum[finis - 1] == (i8)'\n'
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

s32
principale (s32 numerus_argumentorum, character** argumenta_cruda)
{
    Piscina*             piscina;
    ArgumentaParser*     parser;
    ArgumentaFructus*    lecta;
    AedilisConfiguratio* configuratio;
    AedilisFructus*      fructus;
    ExtractorDatum       extractoris;
    SilvaPiscina*        contextus_piscina;
    SilvaContextus*      contextus;
    chorda               causa;
    chorda               scopus;
    chorda               varians;
    chorda               manifestum;
    character*           scopus_cstr;
    constans character*  varians_cstr;
    clock_t              initium;
    clock_t              finis;

    piscina = piscina_generare_dynamicum("aedilis", 16777216);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: piscina deest\n");
        redde 1;
    }
    causa.datum = NIHIL;
    causa.mensura = 0;

    parser = argumenta_creare(piscina);
    argumenta_ponere_descriptionem(parser,
        "aedilis - clausura dependentiarum derivata + manifestum");
    argumenta_addere_optionem(parser, NIHIL, "--varians",
        "Varians platformae (ordinarie praelatio configurationis)");
    argumenta_addere_exemplum(parser, "aedilis lib/hospitium.c");
    argumenta_addere_exemplum(parser,
        "aedilis lib/tcp.c --varians posix");
    lecta = argumenta_parsere(parser, (i32)numerus_argumentorum,
        (constans character* constans*)argumenta_cruda);

    si (argumenta_numerus_positionalium(lecta) != 1)
    {
        fprintf(stderr,
            "usus: aedilis <fons.c> [--varians <verbum>]\n");
        redde 1;
    }
    scopus = argumenta_obtinere_positionalem(lecta, 0, piscina);
    scopus_cstr = chorda_ut_cstr(scopus, piscina);
    varians = argumenta_obtinere_optionem(lecta, "--varians",
        piscina);
    varians_cstr = (varians.mensura > 0)
        ? chorda_ut_cstr(varians, piscina) : NIHIL;

    si (!filum_directorium_creare_si_necesse("build")
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

    contextus_piscina = silva_piscina_generare_dynamicum(
        "aedilis_contextus", 4194304);
    contextus = (contextus_piscina == NIHIL)
        ? NIHIL : silva_contextus_creare(contextus_piscina);
    si (contextus == NIHIL
        || !silva_contextus_latinam_addere(contextus))
    {
        fprintf(stderr, "AEDILIS RECUSAT: contextus deest\n");
        redde 1;
    }
    extractoris.contextus = contextus;
    extractoris.configuratio = configuratio;

    initium = clock();
    fructus = aedilis_derivare(piscina, configuratio, scopus_cstr,
        varians_cstr, _extractor_silvae, &extractoris, &causa);
    finis = clock();
    si (fructus == NIHIL)
    {
        fprintf(stderr, "AEDILIS RECUSAT: %.*s\n",
            (s32)causa.mensura, (constans character*)causa.datum);
        redde 1;
    }

    manifestum = aedilis_manifestum_scribere(fructus, piscina,
        _commissum_obtinere(piscina));

    {
        chorda     basis;
        chorda     directorium;
        chorda     via_manifesti;
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
    }

    redde 0;
}
