/* sonda.c - sonda una-ictus viae LSP (desideratum 01KXJ2K1Z9)
 *
 * Machinam legati IN-PROCESSU agit super FILE* effimeris (exemplar
 * probationis, NON subprocessus): initialize -> didOpen (textus ex
 * disco, effugio JSON) -> [-servare didSave] -> shutdown/exit,
 * deinde effusum TOTUM legit et publicationem ULTIMAM plagulae
 * imprimit. "Ultimus vincit" hic STRUCTURALITER solvitur - cursus
 * ad EOF omnes publicationes fert, ultima sumitur (lectio quae
 * scriptulum reproductionis capitum 2026-07-17 ussit).
 *
 * Usus:  sonda <via> [-servare] [-crudum] [-radix <r>]
 * Exitus: 0 = publicatio visa | 2 = apparatus
 *
 * Adiutores probationis duplicati consulto (praecedens praeparator:
 * duplica, deinde consolida).
 */

#include "latina.h"
#include "legatus.h"
#include "tabellarius.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include <stdio.h>
#include <string.h>

hic_manens b32 crudum = FALSUM;
hic_manens b32 cum_servare = FALSUM;

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura_out)
{
         FILE* pl = fopen(via, "rb");
         long  mensura_l;
    character* textus;

    *mensura_out = ZEPHYRUM;
    si (pl == NIHIL)
    {
        redde NIHIL;
    }
    fseek(pl, 0L, SEEK_END);
    mensura_l = ftell(pl);
    fseek(pl, 0L, SEEK_SET);
    si (mensura_l < 0L)
    {
        fclose(pl);
        redde NIHIL;
    }
    textus = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura_l > 0L ? mensura_l + 1L : I));
    si (   textus == NIHIL
        || (mensura_l > 0L
            && fread(textus, I, (memoriae_index)mensura_l, pl)
                != (memoriae_index)mensura_l))
    {
        fclose(pl);
        redde NIHIL;
    }
    fclose(pl);
    *mensura_out = (i32)mensura_l;
    redde textus;
}

interior constans character*
_gravitas_titulus (
    s64 gravitas)
{
    commutatio ((int)gravitas)
    {
        casus 1:  redde "error";
        casus 2:  redde "cautio";
        casus 3:  redde "nota";
        casus 4:  redde "indicium";
        ordinarius: redde "ignota";
    }
}

interior b32
_chorda_est (
                chorda  c,
    constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (c.mensura == (i32)m && c.datum != NIHIL
        && memcmp(c.datum, litterae, m) == ZEPHYRUM) ? VERUM : FALSUM;
}

/* publicatio imprimitur: ordines examen-formae */
interior s32
_publicationem_imprimere (
             JsonValor* params,
    constans character* via)
{
    JsonValor* lista = json_objectum_capere(params, "diagnostics");
          i32  n;
          i32  k;

    si (lista == NIHIL || !json_est_tabulatum(lista))
    {
        redde -I;
    }
    n = (i32)json_tabulatum_numerus(lista);
    per (k = ZEPHYRUM; k < n; k++)
    {
        JsonValor* d = json_tabulatum_obtinere(lista, k);
        JsonValor* initium;
           chorda  nuntius;
           chorda  fons;
              s64  gravitas;
              s64  linea;
              s64  columna;

        si (d == NIHIL)
        {
            perge;
        }
        initium = json_objectum_capere(json_objectum_capere(d,
            "range"), "start");
        linea = json_ad_integer(json_objectum_capere(initium,
            "line")) + I;
        columna = json_ad_integer(json_objectum_capere(initium,
            "character")) + I;
        gravitas = json_ad_integer(json_objectum_capere(d,
            "severity"));
        nuntius  = json_ad_chorda(json_objectum_capere(d, "message"));
        fons     = json_ad_chorda(json_objectum_capere(d, "source"));
        imprimere("%s:%ld:%ld  [%s%s%.*s] %.*s\n", via,
            (long)linea, (long)columna,
            _gravitas_titulus(gravitas),
            fons.mensura > ZEPHYRUM ? "/" : "",
            (int)fons.mensura,
            fons.datum != NIHIL
                ? (constans character*)fons.datum : "",
            (int)nuntius.mensura,
            nuntius.datum != NIHIL
                ? (constans character*)nuntius.datum : "");
    }
    redde (s32)n;
}

s32
principale (
      integer   argc,
    character** argv)
{
    constans character* via    = NIHIL;
    constans character* radix  = ".";
               Piscina* piscina;
    FILE* intra;
    FILE* extra;
    character* fons_plagulae;
    i32 mensura_plagulae = ZEPHYRUM;
    character uri[1200];
    integer k;

    per (k = I; k < argc; k++)
    {
        si (strcmp(argv[k], "-crudum") == ZEPHYRUM)
        {
            crudum = VERUM;
        }
        alioquin si (strcmp(argv[k], "-servare") == ZEPHYRUM)
        {
            cum_servare = VERUM;
        }
        alioquin si (   strcmp(argv[k], "-radix") == ZEPHYRUM
                     && k + I < argc)
        {
            radix = argv[++k];
        }
        alioquin
        {
            via = argv[k];
        }
    }
    si (via == NIHIL)
    {
        fprintf(stderr, "usus: sonda <via> [-servare] [-crudum]"
            " [-radix <r>]\n");
        redde II;
    }

    piscina = piscina_generare_dynamicum("sonda", 67108864);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "sonda: piscina deest\n");
        redde II;
    }
    fons_plagulae = _plagulam_legere(piscina, via,
        &mensura_plagulae);
    si (fons_plagulae == NIHIL)
    {
        fprintf(stderr, "sonda: plagula illegibilis: %s\n", via);
        redde II;
    }
    si (via[ZEPHYRUM] == '/')
    {
        sprintf(uri, "file://%.900s", via);
    }
    alioquin
    {
        sprintf(uri, "file://%.256s/%.900s", radix, via);
    }

    /* epistulae in tmpfile - cursus ad EOF (exemplar probationis) */
    intra = tmpfile();
    extra = tmpfile();
    si (intra == NIHIL || extra == NIHIL)
    {
        fprintf(stderr, "sonda: tmpfile deest\n");
        redde II;
    }
    {
        ChordaAedificator* aed = chorda_aedificator_creare(piscina,
            (memoriae_index)(mensura_plagulae + 4096));
        character caput_b[2048];

        si (aed == NIHIL)
        {
            redde II;
        }
        sprintf(caput_b, "{\"jsonrpc\":\"2.0\",\"id\":1,"
            "\"method\":\"initialize\",\"params\":{"
            "\"processId\":null,\"rootUri\":null,"
            "\"capabilities\":{}}}");
        tabellarius_epistulam_scribere(intra,
            chorda_ex_literis(caput_b, piscina));
        sprintf(caput_b, "{\"jsonrpc\":\"2.0\","
            "\"method\":\"initialized\",\"params\":{}}");
        tabellarius_epistulam_scribere(intra,
            chorda_ex_literis(caput_b, piscina));

        /* didOpen: textus disci effugio JSON */
        sprintf(caput_b, "{\"jsonrpc\":\"2.0\","
            "\"method\":\"textDocument/didOpen\",\"params\":{"
            "\"textDocument\":{\"uri\":\"%s\","
            "\"languageId\":\"c\",\"version\":1,\"text\":\"", uri);
        (vacuum)chorda_aedificator_appendere_literis(aed, caput_b);
        {
            chorda textus;

            textus.datum    = (i8*)fons_plagulae;
            textus.mensura  = mensura_plagulae;
            (vacuum)chorda_aedificator_appendere_evasus_json(aed,
                textus);
        }
        (vacuum)chorda_aedificator_appendere_literis(aed,
            "\"}}}");
        tabellarius_epistulam_scribere(intra,
            chorda_aedificator_finire(aed));

        si (cum_servare)
        {
            sprintf(caput_b, "{\"jsonrpc\":\"2.0\","
                "\"method\":\"textDocument/didSave\",\"params\":{"
                "\"textDocument\":{\"uri\":\"%s\"}}}", uri);
            tabellarius_epistulam_scribere(intra,
                chorda_ex_literis(caput_b, piscina));
        }
        sprintf(caput_b, "{\"jsonrpc\":\"2.0\",\"id\":2,"
            "\"method\":\"shutdown\"}");
        tabellarius_epistulam_scribere(intra,
            chorda_ex_literis(caput_b, piscina));
        sprintf(caput_b, "{\"jsonrpc\":\"2.0\","
            "\"method\":\"exit\"}");
        tabellarius_epistulam_scribere(intra,
            chorda_ex_literis(caput_b, piscina));
    }
    rewind(intra);
    {
        LegatusConfiguratio cfg;

        memset(&cfg, ZEPHYRUM, magnitudo(LegatusConfiguratio));
        cfg.radix = radix;
        si (legatus_currere(intra, extra, &cfg) != ZEPHYRUM)
        {
            fprintf(stderr, "sonda: machina legati fracta\n");
            redde II;
        }
    }
    rewind(extra);

    /* effusum totum: publicatio ULTIMA vincit (structuraliter -
     * omnes praesentes, ultima sumitur) */
    {
        JsonValor* publicatio_ultima  = NIHIL;
              i32  publicationes      = ZEPHYRUM;

        per (;;)
        {
               b32 finitus = FALSUM;
            chorda corpus = tabellarius_epistulam_legere(extra,
                piscina, &finitus);
            TabellariusNuntius n;

            si (finitus)
            {
                frange;
            }
            n = tabellarius_nuntium_legere(corpus, piscina);
            si (   n.genus  == TABELLARIUS_NUNTIATIO
                && _chorda_est(n.methodus,
                       "textDocument/publishDiagnostics")
                && n.params != NIHIL)
            {
                publicationes++;
                publicatio_ultima = n.params;
                si (crudum)
                {
                    imprimere("%.*s\n", (int)corpus.mensura,
                        (constans character*)corpus.datum);
                }
            }
        }
        si (publicatio_ultima == NIHIL)
        {
            fprintf(stderr, "sonda: nulla publicatio visa\n");
            piscina_destruere(piscina);
            redde II;
        }
        si (!crudum)
        {
            s32 numerus = _publicationem_imprimere(
                publicatio_ultima, via);

            imprimere("sonda: diagnostica %ld"
                " (publicationes %ld%s)\n",
                (long)(numerus >= ZEPHYRUM ? numerus : ZEPHYRUM),
                (long)publicationes,
                cum_servare ? ", cum didSave" : "");
        }
    }
    fclose(intra);
    fclose(extra);
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
