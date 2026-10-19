/* sonda recensoris: sectiones credo propositae - omnes hodie RUBRAE esse debent */
#include "latina.h"
#include "plist.h"
#include "credo.h"
#include "piscina.h"
#include "internamentum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

interior PlistResultus
_legere_literis (
    constans character* textus,
               Piscina* piscina,
   InternamentumChorda* intern)
{
    redde plist_legere(chorda_ex_literis(textus, piscina), piscina, intern);
}

interior vacuum
_abortire (vacuum)
{
    abort();
}

/* filius: profunditas magna legitur; ruina = SIGSEGV */
interior vacuum
_profunditatem_legere (
               Piscina* piscina,
   InternamentumChorda* intern)
{
    constans character* caput = "<plist version=\"1.0\">";
                   i32  gradus = 200000;
                   i32  k;
                   i32  pos;
                   i8*  alveus;
                chorda  textus;
         PlistResultus  r;

    alveus = (i8*)piscina_allocare(piscina,
        (memoriae_index)(gradus * XV + LXIV));
    pos = (i32)strlen(caput);
    memcpy(alveus, caput, (size_t)pos);
    per (k = ZEPHYRUM; k < gradus; k++)
    {
        memcpy(alveus + pos, "<array>", VII);
        pos += VII;
    }
    per (k = ZEPHYRUM; k < gradus; k++)
    {
        memcpy(alveus + pos, "</array>", VIII);
        pos += VIII;
    }
    memcpy(alveus + pos, "</plist>", VIII);
    pos += VIII;
    textus = chorda_ex_buffer(alveus, pos);
    r = plist_legere(textus, piscina, intern);
    si (r.status == PLIST_SUCCESSUS && !r.radix)
    {
        abort();
    }
}

/* filius: graphum cyclicum scribitur et confertur */
interior vacuum
_cyclum_tractare (
    Piscina* piscina)
{
    PlistValor* dictio = plist_dictio_creare(piscina);
    PlistValor* lista  = plist_lista_creare(piscina);
    PlistStatus status;

    si (plist_dictio_ponere(dictio, chorda_ex_literis("ego", piscina),
                            dictio, piscina))
    {
        (vacuum)plist_scribere(dictio, &status, piscina);
        si (status == PLIST_SUCCESSUS)
        {
            abort();
        }
        (vacuum)plist_aequalis(dictio, dictio);
    }
    si (plist_lista_addere(lista, lista, piscina))
    {
        (vacuum)plist_aequalis(lista, lista);
    }
}

hic_manens constans character _textus_medius[] =
    "<plist version=\"1.0\"><dict><key>alpha</key><string>"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
    "</string><key>beta</key><array><integer>1</integer>"
    "<real>0.5</real></array></dict></plist>";

/* filius: piscina parva - PLIST_ERROR_MEMORIA, non exitus processus */
interior vacuum
_piscinam_parvam_legere (
    InternamentumChorda* intern)
{
          Piscina* parva;
    PlistResultus  r;

    atexit(_abortire);   /* exire() piscinae fatalis -> SIGABRT */
    parva = piscina_generare_certae_magnitudinis("parva", MMXLVIII);
    r = plist_legere(chorda_ex_buffer((i8*)_textus_medius,
                         (i32)(magnitudo(_textus_medius) - I)),
                     parva, intern);
    si (r.status != PLIST_ERROR_MEMORIA)
    {
        abort();
    }
}

/* filius: dictio magna (XL milia clavium) */
interior vacuum
_dictionem_magnam_legere (
               Piscina* piscina,
   InternamentumChorda* intern)
{
           i32  numerus = 40000;
           i32  k;
           i32  pos = ZEPHYRUM;
     character* alveus;
  PlistResultus r;

    alveus = (character*)piscina_allocare(piscina,
        (memoriae_index)(numerus * XXX + LXIV));
    pos += (i32)sprintf(alveus + pos, "<plist version=\"1.0\"><dict>");
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        pos += (i32)sprintf(alveus + pos, "<key>k%08u</key><true/>",
                            (insignatus integer)k);
    }
    pos += (i32)sprintf(alveus + pos, "</dict></plist>");
    r = plist_legere(chorda_ex_buffer((i8*)alveus, pos), piscina, intern);
    si (r.status != PLIST_SUCCESSUS || plist_numerus(r.radix) != (i32)numerus)
    {
        abort();
    }
}

s32
principale (vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;
                    b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_plist_rec", 1048576);
    intern  = internamentum_creare(piscina);
    credo_aperire(piscina);

    /* F1 */
    {
        PlistValor* dictio;
        PlistValor* solum;
        PlistValor* inventum;
            chorda  scriptum;
       PlistStatus  status;
     PlistResultus  lectum;

        imprimere("\n--- F1: spatium praefixum in textu et clave ---\n");
        dictio = plist_dictio_creare(piscina);
        CREDO_VERUM(plist_dictio_ponere(dictio,
            chorda_ex_literis(" a", piscina),
            plist_chordam_creare(chorda_ex_literis("  x", piscina), piscina),
            piscina));
        CREDO_VERUM(plist_dictio_ponere(dictio,
            chorda_ex_literis("a", piscina),
            plist_chordam_creare(chorda_ex_literis("\n\tx", piscina),
                                 piscina),
            piscina));
        scriptum = plist_scribere(dictio, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_SUCCESSUS);
        lectum = plist_legere(scriptum, piscina, intern);
        CREDO_AEQUALIS_I32((i32)lectum.status, (i32)PLIST_SUCCESSUS);
        CREDO_NON_NIHIL(lectum.radix);
        si (lectum.radix)
        {
            CREDO_AEQUALIS_I32(plist_numerus(lectum.radix), II);
            inventum = plist_dictio_capere(lectum.radix, " a");
            CREDO_NON_NIHIL(inventum);
            si (inventum)
            {
                CREDO_CHORDA_AEQUALIS_LITERIS(inventum->textus, "  x");
            }
            CREDO_VERUM(plist_aequalis(dictio, lectum.radix));
        }

        solum = plist_chordam_creare(chorda_ex_literis("   ", piscina),
                                     piscina);
        scriptum = plist_scribere(solum, &status, piscina);
        lectum   = plist_legere(scriptum, piscina, intern);
        CREDO_AEQUALIS_I32((i32)lectum.status, (i32)PLIST_SUCCESSUS);
        si (lectum.radix)
        {
            CREDO_AEQUALIS_I32(lectum.radix->textus.mensura, III);
        }
    }

    /* F2 */
    {
        PlistValor* dictio;
            chorda  vacua;
            chorda  scriptum;
       PlistStatus  status;

        imprimere("\n--- F2: data vacua scribitur ut plutil accipit ---\n");
        vacua.datum   = NIHIL;
        vacua.mensura = ZEPHYRUM;
        dictio = plist_dictio_creare(piscina);
        CREDO_VERUM(plist_dictio_ponere(dictio,
            chorda_ex_literis("v", piscina),
            plist_data_creare(vacua, piscina), piscina));
        scriptum = plist_scribere(dictio, &status, piscina);
        CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_SUCCESSUS);
        CREDO_FALSUM(chorda_continet(scriptum,
            chorda_ex_literis("<data/>", piscina)));
        CREDO_CHORDA_CONTINET(scriptum,
            chorda_ex_literis("<data>", piscina));
        CREDO_CHORDA_CONTINET(scriptum,
            chorda_ex_literis("</data>", piscina));
    }

    /* F3 */
    imprimere("\n--- F3: nidificatio profunda non ruit ---\n");
    CREDO_NON_RUIT(_profunditatem_legere(piscina, intern));

    /* F4 */
    imprimere("\n--- F4: graphum cyclicum non ruit ---\n");
    CREDO_NON_RUIT(_cyclum_tractare(piscina));

    /* F5 */
    imprimere("\n--- F5: '=' nisi in fine quadrupli recusatur ---\n");
    CREDO_AEQUALIS_I32((i32)_legere_literis(
        "<plist version=\"1.0\"><data>QQ=A</data></plist>",
        piscina, intern).status, (i32)PLIST_ERROR_BASE64);
    CREDO_AEQUALIS_I32((i32)_legere_literis(
        "<plist version=\"1.0\"><data>QQ=Q</data></plist>",
        piscina, intern).status, (i32)PLIST_ERROR_BASE64);
    CREDO_AEQUALIS_I32((i32)_legere_literis(
        "<plist version=\"1.0\"><data>QQ==QUJD</data></plist>",
        piscina, intern).status, (i32)PLIST_ERROR_BASE64);

    /* F6 */
    {
        PlistValor* nan_nudum;
        PlistValor* dictio;
            chorda  scriptum;
       PlistStatus  status;
     PlistResultus  lectum;

        imprimere("\n--- F6: NaN sibi aequalis ---\n");
        nan_nudum = plist_realem_creare(NAN, piscina);
        CREDO_VERUM(plist_aequalis(nan_nudum, nan_nudum));
        dictio = plist_dictio_creare(piscina);
        CREDO_VERUM(plist_dictio_ponere(dictio,
            chorda_ex_literis("n", piscina),
            plist_realem_creare(NAN, piscina), piscina));
        scriptum = plist_scribere(dictio, &status, piscina);
        lectum   = plist_legere(scriptum, piscina, intern);
        CREDO_AEQUALIS_I32((i32)lectum.status, (i32)PLIST_SUCCESSUS);
        CREDO_VERUM(plist_aequalis(dictio, lectum.radix));
    }

    /* F7 */
    {
        constans character* mala[IV];
                       i32  k;

        imprimere("\n--- F7: UTF-8 invalidum recusatur ---\n");
        mala[0] = "a\xff" "b";
        mala[1] = "a\xc3";
        mala[2] = "\xc0\xaf";
        mala[3] = "\xed\xa0\x80";
        per (k = ZEPHYRUM; k < IV; k++)
        {
            PlistStatus status = PLIST_SUCCESSUS;
                 chorda scriptum;

            scriptum = plist_scribere(plist_chordam_creare(
                chorda_ex_literis(mala[k], piscina), piscina),
                &status, piscina);
            CREDO_AEQUALIS_I32((i32)status, (i32)PLIST_ERROR_SIGNUM);
            CREDO_CHORDA_VACUA(scriptum);
        }
    }

    /* F8 */
    imprimere("\n--- F8: piscina parva -> PLIST_ERROR_MEMORIA ---\n");
    CREDO_NON_RUIT(_piscinam_parvam_legere(intern));

    /* F9 */
    {
        PlistStatus status = PLIST_SUCCESSUS;
                b32 scriptum;

        imprimere("\n--- F9: scribere_plagulam fracta statum non SUCCESSUS fert ---\n");
        scriptum = plist_scribere_plagulam(
            plist_integrum_creare(1, piscina),
            "/nonexistens_rhubarb_plist/x.plist", &status, piscina);
        CREDO_FALSUM(scriptum);
        CREDO_INAEQUALITAS_I32((i32)status, (i32)PLIST_SUCCESSUS);
        status = PLIST_SUCCESSUS;
        scriptum = plist_scribere_plagulam(
            plist_integrum_creare(1, piscina), NIHIL, &status, piscina);
        CREDO_FALSUM(scriptum);
        CREDO_INAEQUALITAS_I32((i32)status, (i32)PLIST_SUCCESSUS);
    }

    /* F10 */
    imprimere("\n--- F10: dictio XL milium clavium intra tempus ---\n");
    CREDO_NON_PENDET(_dictionem_magnam_legere(piscina, intern), MMM);

    /* F11 */
    imprimere("\n--- F11: elementum aut textus in scalari recusatur ---\n");
    CREDO_AEQUALIS_I32((i32)_legere_literis(
        "<plist version=\"1.0\"><string>a<b>ZZZ</b>c</string></plist>",
        piscina, intern).status, (i32)PLIST_ERROR_STRUCTURA);
    CREDO_AEQUALIS_I32((i32)_legere_literis(
        "<plist version=\"1.0\"><dict><key>a<b/>c</key>"
        "<integer>1</integer></dict></plist>",
        piscina, intern).status, (i32)PLIST_ERROR_STRUCTURA);
    CREDO_AEQUALIS_I32((i32)_legere_literis(
        "<plist version=\"1.0\"><true>junk</true></plist>",
        piscina, intern).status, (i32)PLIST_ERROR_STRUCTURA);
    CREDO_AEQUALIS_I32((i32)_legere_literis(
        "<plist version=\"1.0\"><true><dict/></true></plist>",
        piscina, intern).status, (i32)PLIST_ERROR_STRUCTURA);

    /* F12 */
    {
        PlistResultus r;

        imprimere("\n--- F12: referentiae numericae et '&' nudum ---\n");
        r = _legere_literis(
            "<plist version=\"1.0\"><string>&#65;&#x42;</string></plist>",
            piscina, intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_SUCCESSUS);
        si (r.radix)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS(r.radix->textus, "AB");
        }
        CREDO_AEQUALIS_I32((i32)_legere_literis(
            "<plist version=\"1.0\"><string>a & b</string></plist>",
            piscina, intern).status, (i32)PLIST_ERROR_XML);
        CREDO_AEQUALIS_I32((i32)_legere_literis(
            "<plist version=\"1.0\"><string>&foo;</string></plist>",
            piscina, intern).status, (i32)PLIST_ERROR_XML);
    }

    /* F13 */
    imprimere("\n--- F13: sordes ante aut post radicem recusantur ---\n");
    CREDO_AEQUALIS_I32((i32)_legere_literis(
        "xyz junk <plist version=\"1.0\"><string>a</string></plist>",
        piscina, intern).status, (i32)PLIST_ERROR_XML);
    CREDO_AEQUALIS_I32((i32)_legere_literis(
        "<plist version=\"1.0\"><string>a</string></plist><sordes/>",
        piscina, intern).status, (i32)PLIST_ERROR_XML);
    CREDO_AEQUALIS_I32((i32)_legere_literis(
        "<plist version=\"1.0\"><string>a</string></plist>"
        "<plist version=\"1.0\"><integer>1</integer></plist>",
        piscina, intern).status, (i32)PLIST_ERROR_XML);

    /* F14 */
    {
        hic_manens constans character cum_nul[] =
            "<plist version=\"1.0\"><string>a\0b</string></plist>";

        imprimere("\n--- F14: signum C0 in lectione recusatur ---\n");
        CREDO_INAEQUALITAS_I32((i32)_legere_literis(
            "<plist version=\"1.0\"><string>a\001b</string></plist>",
            piscina, intern).status, (i32)PLIST_SUCCESSUS);
        CREDO_INAEQUALITAS_I32((i32)plist_legere(
            chorda_ex_buffer((i8*)cum_nul,
                             (i32)(magnitudo(cum_nul) - I)),
            piscina, intern).status, (i32)PLIST_SUCCESSUS);
    }

    /* F15 */
    {
        PlistResultus r;

        imprimere("\n--- F15: semita dictionis 'plist/dict/key[N]' ---\n");
        r = _legere_literis(
            "<plist version=\"1.0\"><dict><key>a</key></dict></plist>",
            piscina, intern);
        CREDO_AEQUALIS_I32((i32)r.status, (i32)PLIST_ERROR_STRUCTURA);
        CREDO_CHORDA_AEQUALIS_LITERIS(r.semita, "plist/dict/key[0]");
        r = _legere_literis(
            "<plist version=\"1.0\"><array><dict><key>a</key>"
            "<sordes/></dict></array></plist>",
            piscina, intern);
        CREDO_CHORDA_AEQUALIS_LITERIS(r.semita,
            "plist/array[0]/dict/key[0]/sordes");
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();

    redde praeteritus ? ZEPHYRUM : I;
}
