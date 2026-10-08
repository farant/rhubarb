/* aedilis_silva.c - extractor aedilis per silvam (fabrica-6 T6;
 * motum ex tools/aedilis.c sine mutatione morum)
 *
 * Silva pro .c/.h: directivae inclusionum per expansionem (fons
 * princeps solus), annotationes <aedilis verbum="arg"/> per lexemata
 * commentariorum. .m: cursus oraculi clang -MM (system() + plagula
 * temporaria per processum). Memoria per cursum (tabula via ->
 * fructus) et thesaurus trans cursus (recordum sub sigillo praefixi et
 * octetorum fontis). Caput: include/aedilis_silva.h.
 */

/* getpid: plagulae temporariae per processum */
#include "postulata_posix.h"
#include "latina.h"
#include "lectiones.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "filum.h"
#include "via.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "sigillum.h"
#include "thesaurus.h"
#include "aedilis.h"
#include "aedilis_silva.h"

#include "silva.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* status extractoris: contextus silvae, configuratio (inclusa pro -MM),
 * memoria oraculi; memoria per cursum (piscina longaeva, tabula) et
 * thesaurus trans cursus (praefixum clavis) */
structura AedilisSilva {
                  SilvaContextus* contextus;
    constans AedilisConfiguratio* configuratio;
    /* --memoria-oraculi <dir>: effusio clang -MM per fontem, intra
     * cursum UNUM portae (arbor gelata, vexilla eadem) - NIHIL =
     * sine */
              constans character* memoria_oraculi;
                         Piscina* piscina;   /* longaeva: memoriae et
                                              * fructus */
                  TabulaDispersa* tabula;    /* via -> Memoria-
                                              * Extractoris* */
    /* thesaurus (fabrica plan 2 T3): recorda extractionis trans
     * cursus. NIHIL = sine. praefixum = sigillum(binarium aedilis ‖
     * aedilis.stml): extractio silvae PURA est in octetis fontis
     * (nulla IO in expansione; latina in silva infixa), ergo clavis
     * = sigillum(praefixum ‖ octeti fontis). */
                       Thesaurus* thesaurus;
                        Sigillum  praefixum;
};

/* Chordam ex octetis alienis in piscinam copiare (vistae silvae
 * in piscinam plagulae spectant quae mox destruitur) */
interior chorda
_chordam_copiare (
    constans i8* datum,
            i32  mensura,
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

/* Annotatio ANCORATA (grammatica STML una, frustum E3 2026-07-22):
 * post delimitatorem et spatia elementum <aedilis verbum="arg"/>;
 * attributum unicum -> "verbum arg" redditum (machina chordas
 * verbi easdem accipit - INTACTA). Elementa aliena (nid,
 * intentio, tolera...) numquam nostra. Forma vetus "aedilis: ..."
 * aut <aedilis malformatum -> recusatio_out VERUM (tombstone
 * clamosum - nulla prosa tacita; lectio spicae vetus: substring
 * prosam capiebat). mensura 0 = non annotatio. */
interior chorda
_annotationem_extrahere (
                          i8* datum,
                         i32  mensura,
                     Piscina* piscina,
                SilvaPiscina* arboris,
    SilvaInternamentumChorda* intern,
          constans character* via,
                         b32* recusatio_out)
{
    constans character* signum = "aedilis:";
                chorda  vacua;
                   i32  i;
                   i32  j;
                   i32  finis;

    vacua.datum     = NIHIL;
    vacua.mensura   = 0;
    *recusatio_out  = FALSUM;

    i = 0;
    si (   mensura >= 2 && datum[0] == (i8)'/'
        && (datum[1] == (i8)'*' || datum[1] == (i8)'/'))
    {
        i = 2;
    }
    dum (   i < mensura
         && (datum[i] == (i8)' ' || datum[i] == (i8)'\t'))
    {
        i++;
    }
    finis = mensura;
    si (   finis            >= 2 && datum[finis - 2] == (i8)'*'
        && datum[finis - 1] == (i8)'/')
    {
        finis -= 2;
    }
    dum (   finis > i
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

    si (   datum[i] == (i8)'<' && i + 1 < finis
        && ((datum[i + 1] >= (i8)'a' && datum[i + 1] <= (i8)'z')
            || (datum[i + 1] >= (i8)'A'
                && datum[i + 1] <= (i8)'Z')))
    {
              SilvaChorda corpus_annotationis;
        SilvaStmlResultus resultus;
        SilvaStmlNodus* nodus;
        SilvaStmlAttributum* attr;

        corpus_annotationis.mensura =
            (insignatus integer)(finis - i);
        corpus_annotationis.datum = datum + i;
        resultus = silva_stml_legere(corpus_annotationis, arboris,
            intern);
        si (   !resultus.successus
            || resultus.elementum_radix == NIHIL)
        {
            /* malformatum: si "<aedilis" textualiter, nostrum et
             * fractum -> clamare; alioquin annotatio aliena
             * (codex 74 examinis eam iam custodit) */
            si (   finis - i                        >= 8
                && memcmp(datum + i, "<aedilis", 8) == 0)
            {
                fprintf(stderr, "aedilis: annotatio malformata in"
                    " %s - <aedilis verbum=\"arg\"/>"
                    " exspectatum\n", via);
                *recusatio_out = VERUM;
            }
            redde vacua;
        }
        nodus = resultus.elementum_radix;
        si (   nodus->titulus == NIHIL || nodus->titulus->mensura != 7
            || memcmp(nodus->titulus->datum, "aedilis", 7) != 0)
        {
            redde vacua;   /* elementum alienum */
        }
        si (   nodus->attributa                    == NIHIL
            || silva_xar_numerus(nodus->attributa) != 1)
        {
            fprintf(stderr, "aedilis: annotatio in %s attributum"
                " unicum postulat (<aedilis verbum=\"arg\"/>)\n",
                via);
            *recusatio_out = VERUM;
            redde vacua;
        }
        attr = (SilvaStmlAttributum*)silva_xar_obtinere(
            nodus->attributa, 0);
        si (   attr        == NIHIL || attr->titulus == NIHIL
            || attr->valor == NIHIL || attr->valor->mensura == 0)
        {
            fprintf(stderr, "aedilis: annotatio in %s sine valore"
                " (<aedilis verbum=\"arg\"/>)\n", via);
            *recusatio_out = VERUM;
            redde vacua;
        }
        {
            chorda fructus;
               i32 mensura_fructus = (i32)(attr->titulus->mensura + 1
                   + attr->valor->mensura);

            fructus.mensura = mensura_fructus;
            fructus.datum = (i8*)piscina_allocare(piscina,
                (memoriae_index)mensura_fructus);
            si (fructus.datum == NIHIL)
            {
                redde vacua;
            }
            memcpy(fructus.datum, attr->titulus->datum,
                (memoriae_index)attr->titulus->mensura);
            fructus.datum[attr->titulus->mensura] = (i8)' ';
            memcpy(fructus.datum + attr->titulus->mensura + 1,
                attr->valor->datum,
                (memoriae_index)attr->valor->mensura);
            redde fructus;
        }
    }

    /* forma vetus "aedilis:" -> tombstone migrationis */
    per (j = 0; signum[j] != '\0'; j++)
    {
        si (i + j >= finis || datum[i + j] != (i8)signum[j])
        {
            redde vacua;   /* prosa */
        }
    }
    fprintf(stderr, "aedilis: FORMA VETUS \"aedilis: ...\" in %s -"
        " migra ad <aedilis verbum=\"arg\"/>\n", via);
    *recusatio_out = VERUM;
    redde vacua;
}

/* Via temporaria PER PROCESSUM (2026-09-02):
 * "build/aedilis/<basis>.tmp"
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

/* Cursus minoritatis: clang -MM per system(), plagula temporalis.
 * Directivae redditae = viae IAM RESOLUTAE (ex_oraculo). */
interior b32
_extractor_oraculi (
          AedilisSilva*  extractoris,
    constans character*  via,
               Piscina*  piscina,
                   Xar** directivae_out)
{
     ChordaAedificator* mandatum;
                chorda  textus;
             character* mandatum_cstr;
                   i32  i;
                   i32  numerus;
    constans character* via_temporaria;
    constans character* via_memoriae;

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
        via_temporaria = _via_temporaria("oraculum", piscina);
    chorda_aedificator_appendere_literis(mandatum, " > ");
    chorda_aedificator_appendere_literis(mandatum, via_temporaria);
    chorda_aedificator_appendere_literis(mandatum, " 2>/dev/null");
    mandatum_cstr = chorda_ut_cstr(
        chorda_aedificator_finire(mandatum), piscina);

    /* MEMORIA ORACULI (2026-10-02): porta aedilis -MM CCCCCCCXXIX
     * vicibus pro CCI fontibus currebat (~CXLI s, dimidium portae) -
     * effusio fontis idem in omni clausura eius cursu uno. Clavis = via
     * fontis ('/' -> '__'); solum effusio SUCCESSA servatur. */
    via_memoriae = NIHIL;
    si (extractoris->memoria_oraculi != NIHIL)
    {
        ChordaAedificator* clavis;
                      i32  k;

        clavis = chorda_aedificator_creare(piscina, 256);
        chorda_aedificator_appendere_literis(clavis,
            extractoris->memoria_oraculi);
        chorda_aedificator_appendere_literis(clavis, "/");
        per (k = 0; via[k] != '\0'; k++)
        {
            si (via[k] == '/')
            {
                chorda_aedificator_appendere_literis(clavis, "__");
            }
            alioquin
            {
                character unus[II];

                unus[0] = via[k];
                unus[I] = '\0';
                chorda_aedificator_appendere_literis(clavis, unus);
            }
        }
        chorda_aedificator_appendere_literis(clavis, ".mm");
        via_memoriae = chorda_ut_cstr(chorda_aedificator_finire(clavis),
            piscina);
    }
    si (via_memoriae != NIHIL && filum_existit(via_memoriae))
    {
        textus = filum_legere_totum(via_memoriae, piscina);
    }
    alioquin
    {
        si (system(mandatum_cstr) != 0)
        {
            (vacuum)remove(via_temporaria);
            redde FALSUM;
        }
        /* filius (redirectio) scripsit: exitus cursus, non ingressus
         * (fabrica plan 5 T5b - lectio sequens aliter 'sine domino') */
        lectiones_notare(LECTIO_SCRIPSIT, via_temporaria);
        textus = filum_legere_totum(via_temporaria, piscina);
        (vacuum)remove(via_temporaria);
        si (via_memoriae != NIHIL && textus.mensura > 0)
        {
            (vacuum)filum_scribere(via_memoriae, textus);
        }
    }
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

        dum (   i < textus.mensura
             && (textus.datum[i] == (i8)' '
                || textus.datum[i] == (i8)'\t'
                || textus.datum[i] == (i8)'\n'
                || textus.datum[i] == (i8)'\r'
                || textus.datum[i] == (i8)'\\'))
        {
            i++;
        }
        initium = i;
        dum (   i < textus.mensura && textus.datum[i] != (i8)' '
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

            signum.datum    = textus.datum + initium;
            signum.mensura  = i - initium;
            /* fontem ipsum praeterire: -MM eum nudum imprimit,
             * via nostra "./" praefixari potest - suffixo
             * congruere */
            longitudo_viae = strlen(via);
            si (   longitudo_viae >= (memoriae_index)signum.mensura
                && memcmp(via + longitudo_viae
                        - (memoriae_index)signum.mensura,
                    signum.datum,
                    (memoriae_index)signum.mensura) == 0)
            {
                perge;
            }
            /* -MM vias ut scriptas imprimit - "lib/../include/x.h"
             * ex inclusionibus ".." (porta inventum): normalizare
             * ante comparationem */
            _chordam_in_xar(*directivae_out,
                via_normalizare(signum, piscina));
        }
    }
    redde VERUM;
}

interior b32
_extractor_silvae (
                vacuum*  datum,
    constans character*  via,
               Piscina*  piscina,
                   Xar** directivae_out,
                   Xar** annotationes_out,
                   b32*  ex_oraculo_out,
                   Xar** angulatae_out)
{
    AedilisSilva* extractoris;
    SilvaPiscina*   arboris;
    SilvaParsura*   parsura;
    SilvaXar*       cruda;
    SilvaInternamentumChorda* intern;
    chorda          fons;
    memoriae_index  longitudo_viae;
    insignatus integer n;
    insignatus integer k;

    extractoris      = (AedilisSilva*)datum;
    *directivae_out  = xar_creare(piscina, (i32)magnitudo(chorda));
    *annotationes_out = xar_creare(piscina,
        (i32)magnitudo(chorda));
    *ex_oraculo_out = FALSUM;
    /* cursus -MM (.m) vias iam resolutas reddit: forma ignota */
    *angulatae_out = NIHIL;

    longitudo_viae = strlen(via);
    si (   longitudo_viae > 2 && via[longitudo_viae - 2] == '.'
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
    *angulatae_out = xar_creare(piscina, (i32)magnitudo(b32));

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
        si (   vista.fons_ex != parsura->fons_princeps
            || vista.via     == NIHIL)
        {
            perge;
        }
        _chordam_in_xar(*directivae_out,
            _chordam_copiare(vista.via->datum, vista.via->mensura,
                piscina));
        {
            b32* forma;

            forma = (b32*)xar_addere(*angulatae_out);
            si (forma != NIHIL)
            {
                *forma = vista.est_angulata ? VERUM : FALSUM;
            }
        }
    }

    cruda = silva_lexare_cruda(arboris,
        (constans character*)fons.datum, fons.mensura, 0);
    si (cruda != NIHIL)
    {
        intern = silva_internamentum_creare(arboris);
        si (intern == NIHIL)
        {
            silva_piscina_destruere(arboris);
            redde FALSUM;
        }
        n = silva_xar_numerus(cruda);
        per (k = 0; k < n; k++)
        {
            SilvaToken* lexema;
            chorda      annotatio;
            b32         recusatio;

            lexema = *(SilvaToken**)silva_xar_obtinere(cruda, k);
            si (   lexema == NIHIL
                || (lexema->genus != SILVA_LEX_COMMENTUM_CLAUSUM
                    && lexema->genus != SILVA_LEX_COMMENTUM_LINEA))
            {
                perge;
            }
            annotatio = _annotationem_extrahere(
                lexema->valor.datum, lexema->valor.mensura,
                piscina, arboris, intern, via, &recusatio);
            si (recusatio)
            {
                silva_piscina_destruere(arboris);
                redde FALSUM;
            }
            si (annotatio.mensura > 0)
            {
                _chordam_in_xar(*annotationes_out, annotatio);
            }
        }
    }

    silva_piscina_destruere(arboris);
    redde VERUM;
}

/* Memoria extractoris: via -> quod _extractor_silvae reddidit.
 * aedilis_derivare caput quodque per scopum SEMEL scandit, sed per
 * corpus (--corpus) idem caput in clausuris CLVI scoporum CLVI
 * vicibus parsaretur - portae corporis silvae in hoc XVI s per
 * probationem consumebant (2026-09-02). Fructus in piscina longaeva
 * vivunt; derivare eos legit tantum, numquam mutat. */
nomen structura {
    Xar* directivae;
    Xar* annotationes;
    Xar* angulatae;
    b32  ex_oraculo;
    b32  fructus;
} MemoriaExtractoris;


/* RECORDUM extractionis (octeti tuti, non lineae: annotationes
 * lineas novas ferunt):
 *   AEDILIS EXTRACTIO I\n
 *   D <n>\n   deinde n: <mensura> <0|1 angulata>\n<octeti>\n
 *   A <n>\n   deinde n: <mensura>\n<octeti>\n
 * Fructus fractus (FALSUM) numquam conditur. */
#define RECORDUM_CAPUT "AEDILIS EXTRACTIO I\n"

interior chorda
_recordum_scribere (
    constans MemoriaExtractoris* m,
                        Piscina* piscina)
{
    ChordaAedificator* aedificator;
                  i32  i;
                  i32  numerus;

    aedificator = chorda_aedificator_creare(piscina, IV * MXXIV);
    (vacuum)chorda_aedificator_appendere_literis(aedificator,
        RECORDUM_CAPUT);
    numerus = (m->directivae != NIHIL) ? xar_numerus(m->directivae)
        : ZEPHYRUM;
    (vacuum)chorda_aedificator_appendere_literis(aedificator, "D ");
    (vacuum)chorda_aedificator_appendere_i32(aedificator, numerus);
    (vacuum)chorda_aedificator_appendere_character(aedificator, '\n');
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda directiva;
           b32 angulata;

        directiva = *(chorda*)xar_obtinere(m->directivae, i);
        angulata  = (   m->angulatae != NIHIL
                     && i < xar_numerus(m->angulatae))
            ? *(b32*)xar_obtinere(m->angulatae, i) : FALSUM;
        (vacuum)chorda_aedificator_appendere_i32(aedificator,
            directiva.mensura);
        (vacuum)chorda_aedificator_appendere_literis(aedificator,
            angulata ? " 1\n" : " 0\n");
        (vacuum)chorda_aedificator_appendere_chorda(aedificator,
            directiva);
        (vacuum)chorda_aedificator_appendere_character(aedificator,
            '\n');
    }
    numerus = (m->annotationes != NIHIL)
        ? xar_numerus(m->annotationes) : ZEPHYRUM;
    (vacuum)chorda_aedificator_appendere_literis(aedificator, "A ");
    (vacuum)chorda_aedificator_appendere_i32(aedificator, numerus);
    (vacuum)chorda_aedificator_appendere_character(aedificator, '\n');
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        chorda annotatio;

        annotatio = *(chorda*)xar_obtinere(m->annotationes, i);
        (vacuum)chorda_aedificator_appendere_i32(aedificator,
            annotatio.mensura);
        (vacuum)chorda_aedificator_appendere_character(aedificator,
            '\n');
        (vacuum)chorda_aedificator_appendere_chorda(aedificator,
            annotatio);
        (vacuum)chorda_aedificator_appendere_character(aedificator,
            '\n');
    }
    redde chorda_aedificator_finire(aedificator);
}

/* numerus decimalis ad 'positio' usque ad 'terminus'; FALSUM si
 * forma fracta */
interior b32
_numerum_legere (
       chorda  textus,
          i32* positio,
    character  terminus,
          i32* numerus_out)
{
    i32 valor;
    i32 initium;

    valor    = ZEPHYRUM;
    initium  = *positio;
    dum (   *positio < textus.mensura
         && textus.datum[*positio] >= '0'
         && textus.datum[*positio] <= '9')
    {
        valor = valor * X + (i32)(textus.datum[*positio] - '0');
        (*positio)++;
    }
    si (   *positio == initium || *positio >= textus.mensura
        || textus.datum[*positio] != terminus)
    {
        redde FALSUM;
    }
    (*positio)++;
    *numerus_out = valor;
    redde VERUM;
}

/* octeti 'mensura' deinde '\n'; chorda copiata in piscinam */
interior b32
_octetos_legere (
      chorda  textus,
         i32* positio,
         i32  mensura,
     Piscina* piscina,
      chorda* out)
{
    si (   *positio + mensura               >= textus.mensura
        || textus.datum[*positio + mensura] != '\n')
    {
        redde FALSUM;
    }
    *out = _chordam_copiare(textus.datum + *positio,
        mensura, piscina);
    *positio += mensura + I;
    redde VERUM;
}

interior b32
_recordum_legere (
                chorda  textus,
               Piscina* piscina,
    MemoriaExtractoris* m)
{
    i32 positio;
    i32 numerus;
    i32 i;
    i32 longitudo_capitis;

    longitudo_capitis = (i32)strlen(RECORDUM_CAPUT);
    si (   textus.mensura < longitudo_capitis
        || memcmp(textus.datum, RECORDUM_CAPUT,
            (memoriae_index)longitudo_capitis) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    positio          = longitudo_capitis;
    m->directivae    = xar_creare(piscina, (i32)magnitudo(chorda));
    m->annotationes  = xar_creare(piscina, (i32)magnitudo(chorda));
    m->angulatae     = xar_creare(piscina, (i32)magnitudo(b32));
    m->ex_oraculo    = FALSUM;
    si (   positio + II > textus.mensura
        || textus.datum[positio]     != 'D'
        || textus.datum[positio + I] != ' ')
    {
        redde FALSUM;
    }
    positio += II;
    si (!_numerum_legere(textus, &positio, '\n', &numerus))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
           i32  mensura;
        chorda  directiva;
           b32* angulata;

        si (   !_numerum_legere(textus, &positio, ' ', &mensura)
            || positio + II > textus.mensura
            || textus.datum[positio + I] != '\n')
        {
            redde FALSUM;
        }
        angulata = (b32*)xar_addere(m->angulatae);
        si (angulata == NIHIL)
        {
            redde FALSUM;
        }
        *angulata  = (textus.datum[positio] == '1') ? VERUM : FALSUM;
        positio    += II;
        si (!_octetos_legere(textus, &positio, mensura, piscina,
                &directiva))
        {
            redde FALSUM;
        }
        _chordam_in_xar(m->directivae, directiva);
    }
    si (   positio + II > textus.mensura
        || textus.datum[positio]     != 'A'
        || textus.datum[positio + I] != ' ')
    {
        redde FALSUM;
    }
    positio += II;
    si (!_numerum_legere(textus, &positio, '\n', &numerus))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
           i32 mensura;
        chorda annotatio;

        si (   !_numerum_legere(textus, &positio, '\n', &mensura)
            || !_octetos_legere(textus, &positio, mensura, piscina,
            &annotatio))
        {
            redde FALSUM;
        }
        _chordam_in_xar(m->annotationes, annotatio);
    }
    redde positio == textus.mensura;
}

/* clavis extractionis: sigillum(praefixum ‖ octeti fontis). FALSUM si
 * fons legi nequit (extractor ipse tum defectum reddit). */
interior b32
_clavem_extractionis (
            AedilisSilva* memor,
      constans character* via,
                Sigillum* clavis_out)
{
                chorda fons;
     SigillumContextus contextus;

    fons = filum_legere_totum(via, memor->piscina);
    si (fons.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    sigillum_incipere(&contextus);
    sigillum_addere(&contextus, memor->praefixum.octeti, XXXII);
    sigillum_addere(&contextus, fons.datum,
        (memoriae_index)fons.mensura);
    *clavis_out = sigillum_finire(&contextus);
    redde VERUM;
}

/* THESAURUS: recordum quaerere (inventum -> m impletum, VERUM);
 * aliter extrahere et condere. Fontes .m (oraculum clang -MM, capita
 * legit) numquam conduntur. */
interior b32
_extractionem_per_thesaurum (
          AedilisSilva* memor,
    constans character* via,
    MemoriaExtractoris* m)
{
    Sigillum  clavis;
    Sigillum  blobus;
         Xar* exitus;
      chorda  via_blobi;
      chorda  recordum;
         i32  longitudo;

    longitudo = (i32)strlen(via);
    si (   longitudo > II && via[longitudo - II] == '.'
        && via[longitudo - I] == 'm')
    {
        redde FALSUM;
    }
    si (!_clavem_extractionis(memor, via, &clavis))
    {
        redde FALSUM;
    }
    si (   thesaurus_actio_capere(memor->thesaurus, &clavis,
            memor->piscina, &exitus)
        && xar_numerus(exitus) == I
        && thesaurus_via(memor->thesaurus,
            (Sigillum*)xar_obtinere(exitus, ZEPHYRUM), memor->piscina,
            &via_blobi))
    {
        recordum = filum_legere_totum(
            chorda_ut_cstr(via_blobi, memor->piscina), memor->piscina);
        si (_recordum_legere(recordum, memor->piscina, m))
        {
            m->fructus = VERUM;
            thesaurus_generationem_notare(memor->thesaurus, &clavis);
            redde VERUM;
        }
    }
    /* absens aut fractum: extrahere, deinde condere si sanum */
    m->fructus = _extractor_silvae(memor, via, memor->piscina,
        &m->directivae, &m->annotationes, &m->ex_oraculo,
        &m->angulatae);
    si (m->fructus && !m->ex_oraculo)
    {
        recordum = _recordum_scribere(m, memor->piscina);
        exitus = xar_creare(memor->piscina, (i32)magnitudo(Sigillum));
        si (   exitus != NIHIL
            && thesaurus_ponere(memor->thesaurus, recordum, &blobus))
        {
            *(Sigillum*)xar_addere(exitus) = blobus;
            (vacuum)thesaurus_actio_ponere(memor->thesaurus, &clavis,
                exitus);
            thesaurus_generationem_notare(memor->thesaurus, &clavis);
        }
    }
    redde VERUM;
}

b32
aedilis_silva_extrahere (
                vacuum*  datum,
    constans character*  via,
               Piscina*  piscina,
                   Xar** directivae_out,
                   Xar** annotationes_out,
                   b32*  ex_oraculo_out,
                   Xar** angulatae_out)
{
          AedilisSilva* memor;
    MemoriaExtractoris* m;
                vacuum* inventum;

    memor = (AedilisSilva*)datum;
    (vacuum)piscina;
    si (tabula_dispersa_invenire_literis(memor->tabula, via, &inventum))
    {
        m = (MemoriaExtractoris*)inventum;
    }
    alioquin
    {
        m = (MemoriaExtractoris*)piscina_allocare(memor->piscina,
            magnitudo(MemoriaExtractoris));
        si (m == NIHIL)
        {
            redde FALSUM;
        }
        m->directivae    = NIHIL;
        m->annotationes  = NIHIL;
        m->angulatae     = NIHIL;
        m->ex_oraculo    = FALSUM;
        si (   memor->thesaurus == NIHIL
            || !_extractionem_per_thesaurum(memor, via, m))
        {
            m->fructus = _extractor_silvae(memor, via,
                memor->piscina,
                &m->directivae, &m->annotationes, &m->ex_oraculo,
                &m->angulatae);
        }
        (vacuum)tabula_dispersa_inserere(memor->tabula,
            chorda_ex_literis(via, memor->piscina), m);
    }
    *directivae_out    = m->directivae;
    *annotationes_out  = m->annotationes;
    *angulatae_out     = m->angulatae;
    *ex_oraculo_out    = m->ex_oraculo;
    redde m->fructus;
}

AedilisSilva*
aedilis_silva_creare (
                         Piscina* piscina,
    constans AedilisConfiguratio* configuratio,
              constans character* radix_thesauri,
               constans Sigillum* praefixum,
              constans character* memoria_oraculi)
{
     AedilisSilva* extractor;
     SilvaPiscina* contextus_piscina;

    extractor = (AedilisSilva*)piscina_allocare(piscina,
        magnitudo(AedilisSilva));
    si (extractor == NIHIL)
    {
        redde NIHIL;
    }
    memset(extractor, ZEPHYRUM, magnitudo(AedilisSilva));
    contextus_piscina = silva_piscina_generare_dynamicum(
        "aedilis_contextus", 4194304);
    extractor->contextus = (contextus_piscina == NIHIL)
        ? NIHIL : silva_contextus_creare(contextus_piscina);
    si (   extractor->contextus == NIHIL
        || !silva_contextus_latinam_addere(extractor->contextus))
    {
        redde NIHIL;
    }
    extractor->configuratio     = configuratio;
    extractor->memoria_oraculi  = memoria_oraculi;
    extractor->piscina          = piscina;
    extractor->tabula           = tabula_dispersa_creare_chorda(piscina,
        512);
    si (extractor->tabula == NIHIL)
    {
        redde NIHIL;
    }
    si (radix_thesauri != NIHIL)
    {
        si (praefixum == NIHIL)
        {
            redde NIHIL;
        }
        extractor->thesaurus = thesaurus_aperire(radix_thesauri,
            piscina);
        si (extractor->thesaurus == NIHIL)
        {
            redde NIHIL;
        }
        extractor->praefixum = *praefixum;
    }
    redde extractor;
}

b32
aedilis_silva_oraculum (
           AedilisSilva*  extractor,
     constans character*  via,
                Piscina*  piscina,
                    Xar** directivae_out)
{
    redde _extractor_oraculi(extractor, via, piscina, directivae_out);
}
