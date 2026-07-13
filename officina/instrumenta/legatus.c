/* legatus.c - LEGATUS: servus LSP silvae (vide legatus.h)
 *
 * Status: Praeparatio calida una (systema + capita, ~0.15s) +
 * thesaurus documentorum. Documentum per versionem: piscina
 * versionis (textus) + effimera silvae (arbores) - AMBAE deletae
 * cum versione proxima (C7: memoria ligata = documenta aperta).
 * Nuntius quisque piscinam propriam habet, deletam post
 * tractationem.
 */

#include "legatus.h"

#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "json.h"
#include "tabellarius.h"
#include "silva.h"
#include "praeparator.h"

#include <stdlib.h>
#include <string.h>

#define LEGATUS_VIA_MAXIMA 1024

/* documentum apertum (in piscina perenni; membra versionis
 * mutantur) */
nomen structura {
    chorda        via;        /* "./..." relativa radici; NUL post
                               * datum (via_c = (character*)datum) */
    chorda        uri;        /* VERBATIM clientis - relata in
                               * publicationibus (nulla quaestio
                               * codificationis reversae) */
    s64           versio;
    b32           apertum;
    Piscina*      piscina_versionis;   /* textus versionis */
    chorda        textus;
    memoriae_index* lineae_initia;     /* tabula linearum (positio
                                        * <-> octeti + remappa
                                        * utf-16) */
    insignatus integer n_lineae;
    SilvaPiscina* effimera;            /* arbores versionis */
    SilvaParsura* parsura;
    SilvaSemantica* sem;
} LegatusDocumentum;

nomen structura {
    Piscina*        perennis;
    FILE*           extra;
    Praeparatio     praeparatio;
    b32             praeparata;
    b32             initiatum;
    b32             exitus_petitus;    /* shutdown receptum */
    b32             utf16;
    b32             cum_posix;
    character       radix[LEGATUS_VIA_MAXIMA];
    memoriae_index  radix_mensura;
    TabulaDispersa* exclusiones;       /* viae "./..." pinnatae */
    TabulaDispersa* documenta;         /* via -> LegatusDocumentum* */
    Xar*            omnia_documenta;   /* LegatusDocumentum* (ordo
                                        * apertionis; purgatio + D) */
} Legatus;

/* quid ansa post nuntium faciat */
nomen enumeratio {
    LEGATUS_PERGE = 0,
    LEGATUS_FINI
} LegatusCursus;

/* ==================================================
 * auxilia parva
 * ================================================== */

interior b32
_methodus_est (chorda methodus, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (methodus.mensura == (i32)m && methodus.datum != NIHIL
        && memcmp(methodus.datum, litterae, m) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

interior b32
_praefixum_est (chorda methodus, constans character* praefixum)
{
    memoriae_index m = strlen(praefixum);

    redde (methodus.mensura >= (i32)m && methodus.datum != NIHIL
        && memcmp(methodus.datum, praefixum, m) == ZEPHYRUM)
        ? VERUM : FALSUM;
}

interior chorda
_transcribere_nul (Piscina* piscina, chorda fons)
{
    chorda c;
    character* d = (character*)piscina_allocare(piscina,
        (memoriae_index)fons.mensura + I);

    c.mensura = ZEPHYRUM;
    c.datum = NIHIL;
    si (d == NIHIL)
    {
        redde c;
    }
    si (fons.mensura > ZEPHYRUM && fons.datum != NIHIL)
    {
        memcpy(d, fons.datum, (memoriae_index)fons.mensura);
    }
    d[fons.mensura] = '\0';
    c.mensura = fons.mensura;
    c.datum = (i8*)d;
    redde c;
}

interior s32
_hexadecimalis (character c)
{
    si (c >= '0' && c <= '9')
    {
        redde (s32)(c - '0');
    }
    si (c >= 'a' && c <= 'f')
    {
        redde (s32)(c - 'a' + X);
    }
    si (c >= 'A' && c <= 'F')
    {
        redde (s32)(c - 'A' + X);
    }
    redde -I;
}

/* "file:///..." -> via absoluta (percent-decodata). FALSUM =
 * schema alienum / nimis longa / fuga prava. */
interior b32
_uri_ad_viam_absolutam (chorda uri, character* finis,
    memoriae_index capacitas)
{
    memoriae_index i = VII;   /* post "file://" */
    memoriae_index f = ZEPHYRUM;
    constans character* d = (constans character*)uri.datum;

    si (uri.datum == NIHIL || uri.mensura < (i32)VIII
        || memcmp(d, "file://", VII) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    dum (i < (memoriae_index)uri.mensura)
    {
        character c = d[i];

        si (f + II >= capacitas)
        {
            redde FALSUM;
        }
        si (c == '%' && i + II < (memoriae_index)uri.mensura)
        {
            s32 a = _hexadecimalis(d[i + I]);
            s32 b = _hexadecimalis(d[i + II]);

            si (a < ZEPHYRUM || b < ZEPHYRUM)
            {
                redde FALSUM;
            }
            finis[f] = (character)(a * XVI + b);
            f++;
            i += III;
        }
        alioquin
        {
            finis[f] = c;
            f++;
            i++;
        }
    }
    finis[f] = '\0';
    redde VERUM;
}

/* via absoluta -> "./..." relativa radici; FALSUM = extra radicem */
interior b32
_via_relativa (constans Legatus* l, constans character* absoluta,
    character* finis, memoriae_index capacitas)
{
    memoriae_index m = l->radix_mensura;
    memoriae_index reliqua;

    si (m == ZEPHYRUM || strncmp(absoluta, l->radix, m) != ZEPHYRUM
        || absoluta[m] != '/')
    {
        redde FALSUM;
    }
    reliqua = strlen(absoluta + m);
    si (reliqua + II >= capacitas)
    {
        redde FALSUM;
    }
    finis[ZEPHYRUM] = '.';
    memcpy(finis + I, absoluta + m, reliqua + I);
    redde VERUM;
}

/* exclusum: lista pinnata (exclusiones.txt) + filtra viarum unci */
interior b32
_exclusum (constans Legatus* l, constans character* via_c,
    chorda via)
{
    si (l->exclusiones != NIHIL
        && tabula_dispersa_continet(l->exclusiones, via))
    {
        redde VERUM;
    }
    si (strstr(via_c, "scratchpad/") != NIHIL
        || strstr(via_c, "build/") != NIHIL
        || strstr(via_c, "fixa/") != NIHIL
        || strstr(via_c, "amalgama/") != NIHIL)
    {
        redde VERUM;
    }
    redde FALSUM;
}

interior vacuum
_exclusiones_onerare (Legatus* l)
{
    character via_listae[LEGATUS_VIA_MAXIMA + LXIV];
    insignatus integer mensura = ZEPHYRUM;
    character* textus;
    memoriae_index i;
    memoriae_index initium;

    sprintf(via_listae,
        "%s/silva/probationes/fixa/examinis/exclusiones.txt",
        l->radix);
    textus = praeparator_plagulam_legere(l->perennis, via_listae,
        &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr,
            "legatus: exclusiones.txt illegibilis (%s)\n",
            via_listae);
        redde;
    }
    l->exclusiones = tabula_dispersa_creare_chorda(l->perennis,
        (i32)CXXVIII);
    si (l->exclusiones == NIHIL)
    {
        redde;
    }
    initium = ZEPHYRUM;
    per (i = ZEPHYRUM; i <= (memoriae_index)mensura; i++)
    {
        si (i == (memoriae_index)mensura || textus[i] == '\n')
        {
            memoriae_index finis_lineae = i;

            si (finis_lineae > initium
                && textus[finis_lineae - I] == '\r')
            {
                finis_lineae--;
            }
            si (finis_lineae > initium)
            {
                chorda clavis;

                clavis.mensura = (i32)(finis_lineae - initium);
                clavis.datum = (i8*)(textus + initium);
                (vacuum)tabula_dispersa_inserere(l->exclusiones,
                    clavis, NIHIL);
            }
            initium = i + I;
        }
    }
}

/* ==================================================
 * positiones (LSP <-> octeti; remappa utf-16 per lineam - corpus
 * BMP-solum probatum, sed paria surrogata recte tractantur)
 * ================================================== */

interior memoriae_index
_lineae_initium (constans LegatusDocumentum* doc,
    insignatus integer linea0)
{
    si (doc->lineae_initia == NIHIL || doc->n_lineae == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (linea0 >= doc->n_lineae)
    {
        linea0 = doc->n_lineae - I;
    }
    redde doc->lineae_initia[linea0];
}

interior memoriae_index
_lineae_terminus (constans LegatusDocumentum* doc,
    insignatus integer linea0)
{
    si (doc->lineae_initia == NIHIL || doc->n_lineae == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (linea0 + I < doc->n_lineae)
    {
        redde doc->lineae_initia[linea0 + I] - I;   /* ante '\n' */
    }
    redde (memoriae_index)doc->textus.mensura;
}

/* unitates utf-16 inter octetos [a,b) */
interior insignatus integer
_utf16_unitates (constans i8* d, memoriae_index a,
    memoriae_index b)
{
    insignatus integer unitates = ZEPHYRUM;
    memoriae_index i = a;

    dum (i < b)
    {
        insignatus character c = (insignatus character)d[i];

        si (c < 0x80u)
        {
            unitates += I;
            i += I;
        }
        alioquin si (c < 0xE0u)
        {
            unitates += I;
            i += II;
        }
        alioquin si (c < 0xF0u)
        {
            unitates += I;
            i += III;
        }
        alioquin
        {
            unitates += II;   /* astralis = par surrogatum */
            i += IV;
        }
    }
    redde unitates;
}

/* progressio ab a per N unitates utf-16 (limitata b) */
interior memoriae_index
_utf16_progredi (constans i8* d, memoriae_index a,
    memoriae_index b, insignatus integer unitates)
{
    memoriae_index i = a;

    dum (i < b && unitates > ZEPHYRUM)
    {
        insignatus character c = (insignatus character)d[i];

        si (c < 0x80u)
        {
            unitates -= I;
            i += I;
        }
        alioquin si (c < 0xE0u)
        {
            unitates -= I;
            i += II;
        }
        alioquin si (c < 0xF0u)
        {
            unitates -= I;
            i += III;
        }
        alioquin
        {
            unitates = unitates >= II ? unitates - II : ZEPHYRUM;
            i += IV;
        }
    }
    redde i < b ? i : b;
}

/* positio LSP -> offset octeti (limitata ad finem lineae) */
interior memoriae_index
_positio_ad_byte (constans Legatus* l,
    constans LegatusDocumentum* doc, insignatus integer linea0,
    insignatus integer character0)
{
    memoriae_index a = _lineae_initium(doc, linea0);
    memoriae_index b = _lineae_terminus(doc, linea0);

    si (!l->utf16)
    {
        memoriae_index octetum = a + character0;

        redde octetum < b ? octetum : b;
    }
    redde _utf16_progredi(doc->textus.datum, a, b, character0);
}

/* columna silvae (1-basata, octeti) -> character LSP in linea
 * (1-basata) data */
interior insignatus integer
_columna_ad_lsp (constans Legatus* l,
    constans LegatusDocumentum* doc, insignatus integer linea1,
    insignatus integer columna1)
{
    memoriae_index a;
    memoriae_index b;
    memoriae_index octetum;

    si (columna1 == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (!l->utf16)
    {
        redde columna1 - I;
    }
    a = _lineae_initium(doc, linea1 - I);
    b = _lineae_terminus(doc, linea1 - I);
    octetum = a + (columna1 - I);
    si (octetum > b)
    {
        octetum = b;
    }
    redde _utf16_unitates(doc->textus.datum, a, octetum);
}

/* offset octeti -> (linea0, character LSP) - quaestio binaria */
interior vacuum
_byte_ad_positio (constans Legatus* l,
    constans LegatusDocumentum* doc, memoriae_index octetum,
    insignatus integer* linea0, insignatus integer* character0)
{
    insignatus integer lo = ZEPHYRUM;
    insignatus integer hi;
    memoriae_index a;

    *linea0 = ZEPHYRUM;
    *character0 = ZEPHYRUM;
    si (doc->lineae_initia == NIHIL || doc->n_lineae == ZEPHYRUM)
    {
        redde;
    }
    hi = doc->n_lineae - I;
    dum (lo < hi)
    {
        insignatus integer medius = lo + (hi - lo + I) / II;

        si (doc->lineae_initia[medius] <= octetum)
        {
            lo = medius;
        }
        alioquin
        {
            hi = medius - I;
        }
    }
    *linea0 = lo;
    a = doc->lineae_initia[lo];
    si (!l->utf16)
    {
        *character0 = (insignatus integer)(octetum - a);
    }
    alioquin
    {
        *character0 = _utf16_unitates(doc->textus.datum, a,
            octetum);
    }
}

/* ==================================================
 * scriptura responsorum / publicationum
 * ================================================== */

interior vacuum
_respondere (Legatus* l, chorda corpus)
{
    tabellarius_epistulam_scribere(l->extra, corpus);
}

interior JsonValor*
_positio_json (Piscina* p, insignatus integer linea,
    insignatus integer columna)
{
    JsonValor* pos = json_objectum_creare(p);

    json_objectum_ponere(pos, "line",
        json_integer_creare(p, (s64)linea));
    json_objectum_ponere(pos, "character",
        json_integer_creare(p, (s64)columna));
    redde pos;
}

interior JsonValor*
_regio_json (Piscina* p, insignatus integer l0,
    insignatus integer c0, insignatus integer l1,
    insignatus integer c1)
{
    JsonValor* regio = json_objectum_creare(p);

    json_objectum_ponere(regio, "start", _positio_json(p, l0, c0));
    json_objectum_ponere(regio, "end", _positio_json(p, l1, c1));
    redde regio;
}

interior JsonValor*
_diagnosticum_json (Piscina* p, insignatus integer l0,
    insignatus integer c0, insignatus integer l1,
    insignatus integer c1, insignatus integer severitas,
    constans character* nuntius)
{
    JsonValor* diag = json_objectum_creare(p);

    json_objectum_ponere(diag, "range",
        _regio_json(p, l0, c0, l1, c1));
    json_objectum_ponere(diag, "severity",
        json_integer_creare(p, (s64)severitas));
    json_objectum_ponere(diag, "source",
        json_chorda_creare_literis(p, "silva"));
    json_objectum_ponere(diag, "message",
        json_chorda_creare_literis(p, nuntius));
    redde diag;
}

/* publicare diagnostica (lista NIHIL = vacua - purgat undulas) */
interior vacuum
_publicare (Legatus* l, Piscina* pn, chorda uri, s64 versio,
    JsonValor* lista)
{
    JsonValor* params = json_objectum_creare(pn);
    chorda corpus;

    json_objectum_ponere(params, "uri",
        json_chorda_creare(pn, uri));
    si (versio >= (s64)ZEPHYRUM)
    {
        json_objectum_ponere(params, "version",
            json_integer_creare(pn, versio));
    }
    json_objectum_ponere(params, "diagnostics",
        lista != NIHIL ? lista : json_tabulatum_creare(pn));
    corpus = tabellarius_nuntiationem(pn,
        "textDocument/publishDiagnostics", params);
    _respondere(l, corpus);
}

/* ==================================================
 * lamina analysis (didOpen/didChange -> publish)
 * ================================================== */

interior vacuum
_analysare_et_publicare (Legatus* l, Piscina* pn,
    LegatusDocumentum* doc)
{
    JsonValor* lista = json_tabulatum_creare(pn);
    insignatus integer omissa = ZEPHYRUM;

    /* arbores versionis prioris demoliri */
    si (doc->effimera != NIHIL)
    {
        silva_piscina_destruere(doc->effimera);
        doc->effimera = NIHIL;
        doc->parsura = NIHIL;
        doc->sem = NIHIL;
    }
    doc->effimera = silva_piscina_generare_dynamicum("legatus_arbor",
        8388608);
    si (doc->effimera == NIHIL || !l->praeparata)
    {
        fprintf(stderr, "legatus: apparatus analysis fractus\n");
        _publicare(l, pn, doc->uri, doc->versio, NIHIL);
        redde;
    }
    doc->sem = praeparator_analysare(&l->praeparatio, doc->effimera,
        (constans character*)doc->via.datum,
        (constans character*)doc->textus.datum,
        (insignatus integer)doc->textus.mensura, &doc->parsura);

    /* errores syntaxis: v0 = extensio degenerata ad 0:0 (chunk D
     * positiones veras ex nodis erroris promovebit - C13) */
    si (doc->parsura != NIHIL
        && doc->parsura->numerus_errorum > ZEPHYRUM)
    {
        character nuntius_b[CCLVI];

        sprintf(nuntius_b, "syntaxis fracta (%u loci)",
            doc->parsura->numerus_errorum);
        json_tabulatum_addere(lista, _diagnosticum_json(pn,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, I, nuntius_b));
    }

    si (doc->sem != NIHIL)
    {
        insignatus integer n = silva_c89_diagnostica_numerus(
            doc->sem);
        insignatus integer i;

        per (i = ZEPHYRUM; i < n; i++)
        {
            constans SemanticaDiagnosticum* d =
                silva_c89_diagnosticum_per_indicem(doc->sem, i);
            insignatus integer severitas_lsp;
            character nuntius_b[DXII];

            si (d == NIHIL || d->severitas == (int)EXAMEN_INFRA)
            {
                perge;
            }
            /* filtrum trans-plagulare (C3): solum via documenti */
            si (d->via.mensura != (unsigned int)doc->via.mensura
                || d->via.datum == NIHIL
                || memcmp(d->via.datum, doc->via.datum,
                       (memoriae_index)doc->via.mensura)
                    != ZEPHYRUM)
            {
                omissa++;
                perge;
            }
            severitas_lsp =
                d->severitas == (int)EXAMEN_VIOLATIO ? I
                : d->severitas == (int)EXAMEN_SUSPECTUM ? II : III;
            sprintf(nuntius_b, "%s%s%s", d->causa,
                d->provisionale ? " (provisionale)" : "",
                d->linea == ZEPHYRUM ? " (positio ignota)" : "");
            si (d->linea > ZEPHYRUM)
            {
                insignatus integer c0 = _columna_ad_lsp(l, doc,
                    d->linea, d->columna);
                insignatus integer c1 = d->columna > ZEPHYRUM
                    ? _columna_ad_lsp(l, doc, d->linea,
                          d->columna + (d->longitudo > ZEPHYRUM
                                            ? d->longitudo : I))
                    : c0 + I;

                si (c1 <= c0)
                {
                    c1 = c0 + I;
                }
                json_tabulatum_addere(lista, _diagnosticum_json(pn,
                    d->linea - I, c0, d->linea - I, c1,
                    severitas_lsp, nuntius_b));
            }
            alioquin
            {
                json_tabulatum_addere(lista, _diagnosticum_json(pn,
                    ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, severitas_lsp,
                    nuntius_b));
            }
        }
    }
    si (omissa > ZEPHYRUM)
    {
        fprintf(stderr,
            "legatus: %u diagnostica aliarum plagularum omissa "
            "(%.*s)\n", omissa, (int)doc->via.mensura,
            (constans character*)doc->via.datum);
    }
    _publicare(l, pn, doc->uri, doc->versio, lista);
}

/* ==================================================
 * thesaurus documentorum
 * ================================================== */

interior LegatusDocumentum*
_documentum_invenire (Legatus* l, chorda via)
{
    vacuum* valor = NIHIL;

    si (l->documenta != NIHIL
        && tabula_dispersa_invenire(l->documenta, via, &valor))
    {
        redde (LegatusDocumentum*)valor;
    }
    redde NIHIL;
}

interior LegatusDocumentum*
_documentum_parare (Legatus* l, chorda via, chorda uri)
{
    LegatusDocumentum* doc = _documentum_invenire(l, via);

    si (doc == NIHIL)
    {
        doc = (LegatusDocumentum*)piscina_allocare(l->perennis,
            magnitudo(LegatusDocumentum));
        si (doc == NIHIL)
        {
            redde NIHIL;
        }
        memset(doc, ZEPHYRUM, magnitudo(LegatusDocumentum));
        doc->via = _transcribere_nul(l->perennis, via);
        doc->uri = _transcribere_nul(l->perennis, uri);
        si (doc->via.datum == NIHIL || doc->uri.datum == NIHIL)
        {
            redde NIHIL;
        }
        (vacuum)tabula_dispersa_inserere(l->documenta, doc->via,
            doc);
        si (l->omnia_documenta != NIHIL)
        {
            LegatusDocumentum** locus = (LegatusDocumentum**)
                xar_addere(l->omnia_documenta);

            si (locus != NIHIL)
            {
                *locus = doc;
            }
        }
    }
    redde doc;
}

/* textum versionis novae ponere (piscina versionis prioris
 * demolitur - textus + arbores priores simul) */
interior b32
_documentum_textum_ponere (Legatus* l, LegatusDocumentum* doc,
    chorda textus, s64 versio)
{
    Piscina* nova = piscina_generare_dynamicum("legatus_documentum",
        (memoriae_index)textus.mensura + CCLVI);
    chorda copia;
    memoriae_index* initia;
    insignatus integer n_lineae = I;

    (vacuum)l;
    si (nova == NIHIL)
    {
        redde FALSUM;
    }
    copia = _transcribere_nul(nova, textus);
    si (copia.datum == NIHIL)
    {
        piscina_destruere(nova);
        redde FALSUM;
    }
    /* tabula linearum */
    {
        memoriae_index i;

        per (i = ZEPHYRUM; i < (memoriae_index)copia.mensura; i++)
        {
            si (copia.datum[i] == '\n')
            {
                n_lineae++;
            }
        }
        initia = (memoriae_index*)piscina_allocare(nova,
            n_lineae * magnitudo(memoriae_index));
        si (initia == NIHIL)
        {
            piscina_destruere(nova);
            redde FALSUM;
        }
        initia[ZEPHYRUM] = ZEPHYRUM;
        {
            insignatus integer lin = I;

            per (i = ZEPHYRUM; i < (memoriae_index)copia.mensura;
                 i++)
            {
                si (copia.datum[i] == '\n')
                {
                    initia[lin] = i + I;
                    lin++;
                }
            }
        }
    }
    /* arbores ANTE textum demoliri (arbores in fontem monstrant) */
    si (doc->effimera != NIHIL)
    {
        silva_piscina_destruere(doc->effimera);
        doc->effimera = NIHIL;
        doc->parsura = NIHIL;
        doc->sem = NIHIL;
    }
    si (doc->piscina_versionis != NIHIL)
    {
        piscina_destruere(doc->piscina_versionis);
    }
    doc->piscina_versionis = nova;
    doc->textus = copia;
    doc->lineae_initia = initia;
    doc->n_lineae = n_lineae;
    doc->versio = versio;
    doc->apertum = VERUM;
    redde VERUM;
}

interior vacuum
_documentum_claudere (Legatus* l, Piscina* pn,
    LegatusDocumentum* doc)
{
    _publicare(l, pn, doc->uri, -I, NIHIL);   /* undulas purgare */
    si (doc->effimera != NIHIL)
    {
        silva_piscina_destruere(doc->effimera);
        doc->effimera = NIHIL;
        doc->parsura = NIHIL;
        doc->sem = NIHIL;
    }
    si (doc->piscina_versionis != NIHIL)
    {
        piscina_destruere(doc->piscina_versionis);
        doc->piscina_versionis = NIHIL;
    }
    doc->textus.mensura = ZEPHYRUM;
    doc->textus.datum = NIHIL;
    doc->lineae_initia = NIHIL;
    doc->n_lineae = ZEPHYRUM;
    doc->apertum = FALSUM;
}

/* ==================================================
 * tractatores nuntiorum
 * ================================================== */

/* uri ex params.textDocument.uri -> via relativa; VERUM si intra
 * radicem (via_c/via_ex impleta) */
interior b32
_viam_extrahere (Legatus* l, JsonValor* doc_v,
    character* via_c, memoriae_index capacitas, chorda* via_ex,
    chorda* uri_ex)
{
    character absoluta[LEGATUS_VIA_MAXIMA];
    JsonValor* uri_v;
    chorda uri;

    si (doc_v == NIHIL || !json_est_objectum(doc_v))
    {
        redde FALSUM;
    }
    uri_v = json_objectum_capere(doc_v, "uri");
    si (uri_v == NIHIL || !json_est_chorda(uri_v))
    {
        redde FALSUM;
    }
    uri = json_ad_chorda(uri_v);
    *uri_ex = uri;
    si (!_uri_ad_viam_absolutam(uri, absoluta,
            magnitudo(absoluta)))
    {
        redde FALSUM;
    }
    si (!_via_relativa(l, absoluta, via_c, capacitas))
    {
        redde FALSUM;
    }
    via_ex->mensura = (i32)strlen(via_c);
    via_ex->datum = (i8*)via_c;
    redde VERUM;
}

interior s64
_versionem_extrahere (JsonValor* doc_v)
{
    JsonValor* v;

    si (doc_v == NIHIL)
    {
        redde -I;
    }
    v = json_objectum_capere(doc_v, "version");
    si (v == NIHIL || !json_est_integer(v))
    {
        redde -I;
    }
    redde json_ad_integer(v);
}

interior vacuum
_didopen_tractare (Legatus* l, Piscina* pn, JsonValor* params)
{
    character via_c[LEGATUS_VIA_MAXIMA];
    chorda via;
    chorda uri;
    JsonValor* doc_v;
    JsonValor* textus_v;
    LegatusDocumentum* doc;

    si (params == NIHIL)
    {
        redde;
    }
    doc_v = json_objectum_capere(params, "textDocument");
    si (!_viam_extrahere(l, doc_v, via_c, magnitudo(via_c), &via,
            &uri))
    {
        redde;   /* extra radicem / uri pravum - tacite */
    }
    si (_exclusum(l, via_c, via))
    {
        _publicare(l, pn, uri, _versionem_extrahere(doc_v), NIHIL);
        redde;
    }
    textus_v = json_objectum_capere(doc_v, "text");
    si (textus_v == NIHIL || !json_est_chorda(textus_v))
    {
        redde;
    }
    doc = _documentum_parare(l, via, uri);
    si (doc == NIHIL
        || !_documentum_textum_ponere(l, doc,
               json_ad_chorda(textus_v),
               _versionem_extrahere(doc_v)))
    {
        redde;
    }
    _analysare_et_publicare(l, pn, doc);
}

interior vacuum
_didchange_tractare (Legatus* l, Piscina* pn, JsonValor* params)
{
    character via_c[LEGATUS_VIA_MAXIMA];
    chorda via;
    chorda uri;
    JsonValor* doc_v;
    JsonValor* mutationes;
    JsonValor* ultima;
    JsonValor* textus_v;
    LegatusDocumentum* doc;
    i32 n;

    si (params == NIHIL)
    {
        redde;
    }
    doc_v = json_objectum_capere(params, "textDocument");
    si (!_viam_extrahere(l, doc_v, via_c, magnitudo(via_c), &via,
            &uri))
    {
        redde;
    }
    si (_exclusum(l, via_c, via))
    {
        _publicare(l, pn, uri, _versionem_extrahere(doc_v), NIHIL);
        redde;
    }
    /* sync PLENUS: elementum ULTIMUM contentChanges = textus totus
     * (C9 - ne "emendetur" in incrementalem: change:1 nuntiavimus) */
    mutationes = json_objectum_capere(params, "contentChanges");
    si (mutationes == NIHIL || !json_est_tabulatum(mutationes))
    {
        redde;
    }
    n = json_tabulatum_numerus(mutationes);
    si (n <= ZEPHYRUM)
    {
        redde;
    }
    ultima = json_tabulatum_obtinere(mutationes, n - I);
    si (ultima == NIHIL || !json_est_objectum(ultima))
    {
        redde;
    }
    textus_v = json_objectum_capere(ultima, "text");
    si (textus_v == NIHIL || !json_est_chorda(textus_v))
    {
        redde;
    }
    doc = _documentum_parare(l, via, uri);
    si (doc == NIHIL
        || !_documentum_textum_ponere(l, doc,
               json_ad_chorda(textus_v),
               _versionem_extrahere(doc_v)))
    {
        redde;
    }
    _analysare_et_publicare(l, pn, doc);
}

interior vacuum
_didclose_tractare (Legatus* l, Piscina* pn, JsonValor* params)
{
    character via_c[LEGATUS_VIA_MAXIMA];
    chorda via;
    chorda uri;
    JsonValor* doc_v;
    LegatusDocumentum* doc;

    si (params == NIHIL)
    {
        redde;
    }
    doc_v = json_objectum_capere(params, "textDocument");
    si (!_viam_extrahere(l, doc_v, via_c, magnitudo(via_c), &via,
            &uri))
    {
        redde;
    }
    doc = _documentum_invenire(l, via);
    si (doc != NIHIL && doc->apertum)
    {
        _documentum_claudere(l, pn, doc);
    }
}

/* ==================================================
 * hover + documentSymbol (chunk C): descensus per extenta
 * (silva_nodus_extensionem chunki 0) - index spatialis non opus
 * ================================================== */

interior b32
_extentum_continet (constans SilvaNodus* n, s32 fons, s32 octetum,
    s32* a_ex, s32* b_ex)
{
    s32 a = -I;
    s32 b = ZEPHYRUM;

    silva_nodus_extensionem(n, fons, &a, &b);
    si (a >= ZEPHYRUM && a <= octetum && octetum < b)
    {
        *a_ex = a;
        *b_ex = b;
        redde VERUM;
    }
    redde FALSUM;
}

interior constans SilvaNodus*
_filium_continentem (constans SilvaNodus* n, s32 fons, s32 octetum,
    s32* a_ex, s32* b_ex)
{
    insignatus integer k;

    per (k = ZEPHYRUM; k < n->numerus_locorum; k++)
    {
        SilvaValor v = n->loci[k];

        si (v.genus == SILVA_VALOR_NODUS && v.datum.nodus != NIHIL)
        {
            si (_extentum_continet(v.datum.nodus, fons, octetum,
                    a_ex, b_ex))
            {
                redde v.datum.nodus;
            }
        }
        alioquin si (v.genus == SILVA_VALOR_LISTA)
        {
            insignatus integer m = silva_valor_lista_numerus(v);
            insignatus integer j;

            per (j = ZEPHYRUM; j < m; j++)
            {
                SilvaValor* e = silva_valor_lista_obtinere(v, j);

                si (e != NIHIL && e->genus == SILVA_VALOR_NODUS
                    && e->datum.nodus != NIHIL
                    && _extentum_continet(e->datum.nodus, fons,
                           octetum, a_ex, b_ex))
                {
                    redde e->datum.nodus;
                }
            }
        }
    }
    redde NIHIL;
}

interior JsonValor*
_regionem_ex_extentis (Legatus* l, Piscina* pn,
    constans LegatusDocumentum* doc, s32 a, s32 b)
{
    insignatus integer l0;
    insignatus integer c0;
    insignatus integer l1;
    insignatus integer c1;

    _byte_ad_positio(l, doc, (memoriae_index)a, &l0, &c0);
    _byte_ad_positio(l, doc, (memoriae_index)b, &l1, &c1);
    redde _regio_json(pn, l0, c0, l1, c1);
}

/* NB: nexus symbolorum = SEDES USUS solum (inventum sessionis M4b)
 * - hover super NOMINE DECLARATO nullum reddit; parca nominata
 * (exportatio nominis declaratoris silva-latere = via retro). */
interior vacuum
_hover_tractare (Legatus* l, Piscina* pn, JsonValor* id,
    JsonValor* params)
{
    character via_c[LEGATUS_VIA_MAXIMA];
    chorda via;
    chorda uri;
    JsonValor* doc_v;
    JsonValor* pos_v;
    LegatusDocumentum* doc;
    memoriae_index octetum;
    s32 fons;
    s32 a_e = -I;
    s32 b_e = ZEPHYRUM;
    s32 el_a = -I;
    s32 el_b = ZEPHYRUM;
    constans SilvaNodus* nodus = NIHIL;
    constans SemanticaSymbolum* symbolum_electum = NIHIL;
    constans TypusC89* typus_electus = NIHIL;

    si (params == NIHIL)
    {
        _respondere(l, tabellarius_responsum(pn, id, NIHIL));
        redde;
    }
    doc_v = json_objectum_capere(params, "textDocument");
    pos_v = json_objectum_capere(params, "position");
    si (!_viam_extrahere(l, doc_v, via_c, magnitudo(via_c), &via,
            &uri)
        || pos_v == NIHIL)
    {
        _respondere(l, tabellarius_responsum(pn, id, NIHIL));
        redde;
    }
    doc = _documentum_invenire(l, via);
    si (doc == NIHIL || !doc->apertum || doc->sem == NIHIL
        || doc->parsura == NIHIL
        || doc->parsura->commissio == NIHIL)
    {
        _respondere(l, tabellarius_responsum(pn, id, NIHIL));
        redde;
    }
    octetum = _positio_ad_byte(l, doc,
        (insignatus integer)json_ad_integer(
            json_objectum_capere(pos_v, "line")),
        (insignatus integer)json_ad_integer(
            json_objectum_capere(pos_v, "character")));
    fons = doc->parsura->fons_princeps;

    /* elementum radicis continens (filtrum fontis principis
     * nodos lexici naturaliter excludit) */
    {
        SilvaValor radix = doc->parsura->commissio->radix;
        insignatus integer n_el = silva_valor_lista_numerus(radix);
        insignatus integer k;

        per (k = ZEPHYRUM; k < n_el; k++)
        {
            SilvaValor* v = silva_valor_lista_obtinere(radix, k);

            si (v != NIHIL && v->genus == SILVA_VALOR_NODUS
                && v->datum.nodus != NIHIL
                && _extentum_continet(v->datum.nodus, fons,
                       (s32)octetum, &a_e, &b_e))
            {
                nodus = v->datum.nodus;
                frange;
            }
        }
    }

    /* descensus: profundissimum symbolum/typus vincit */
    dum (nodus != NIHIL)
    {
        constans SemanticaSymbolum* s = silva_c89_symbolum_nodi(
            doc->sem, nodus);
        constans TypusC89* t = silva_c89_typus_expressionis(
            doc->sem, nodus);

        si (s != NIHIL)
        {
            symbolum_electum = s;
            typus_electus = NIHIL;   /* symbolum profundius vincit */
            el_a = a_e;
            el_b = b_e;
        }
        alioquin si (t != NIHIL)
        {
            typus_electus = t;
            symbolum_electum = NIHIL;
            el_a = a_e;
            el_b = b_e;
        }
        nodus = _filium_continentem(nodus, fons, (s32)octetum,
            &a_e, &b_e);
    }

    si (symbolum_electum == NIHIL && typus_electus == NIHIL)
    {
        _respondere(l, tabellarius_responsum(pn, id, NIHIL));
        redde;
    }
    {
        character typus_b[CCLVI];
        character valor_b[DXII];
        JsonValor* resultatum;
        JsonValor* contenta;

        si (symbolum_electum != NIHIL)
        {
            si (silva_c89_typum_scribere(symbolum_electum->typus,
                    typus_b, (insignatus integer)CCLVI)
                > ZEPHYRUM)
            {
                sprintf(valor_b, "%.*s : %s",
                    (int)symbolum_electum->titulus.mensura,
                    (constans character*)
                        symbolum_electum->titulus.datum,
                    typus_b);
            }
            alioquin
            {
                /* typus irreddibilis (functio/acies) - nomen solum */
                sprintf(valor_b, "%.*s",
                    (int)symbolum_electum->titulus.mensura,
                    (constans character*)
                        symbolum_electum->titulus.datum);
            }
        }
        alioquin
        {
            si (silva_c89_typum_scribere(typus_electus, typus_b,
                    (insignatus integer)CCLVI) == ZEPHYRUM)
            {
                _respondere(l, tabellarius_responsum(pn, id,
                    NIHIL));
                redde;
            }
            sprintf(valor_b, "%s", typus_b);
        }
        resultatum = json_objectum_creare(pn);
        contenta = json_objectum_creare(pn);
        json_objectum_ponere(contenta, "kind",
            json_chorda_creare_literis(pn, "plaintext"));
        json_objectum_ponere(contenta, "value",
            json_chorda_creare_literis(pn, valor_b));
        json_objectum_ponere(resultatum, "contents", contenta);
        si (el_a >= ZEPHYRUM)
        {
            json_objectum_ponere(resultatum, "range",
                _regionem_ex_extentis(l, pn, doc, el_a, el_b));
        }
        _respondere(l, tabellarius_responsum(pn, id, resultatum));
    }
}

interior vacuum
_documentsymbol_tractare (Legatus* l, Piscina* pn, JsonValor* id,
    JsonValor* params)
{
    character via_c[LEGATUS_VIA_MAXIMA];
    chorda via;
    chorda uri;
    JsonValor* doc_v;
    LegatusDocumentum* doc;
    JsonValor* lista = json_tabulatum_creare(pn);

    si (params == NIHIL)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    doc_v = json_objectum_capere(params, "textDocument");
    si (!_viam_extrahere(l, doc_v, via_c, magnitudo(via_c), &via,
            &uri))
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    doc = _documentum_invenire(l, via);
    si (doc == NIHIL || !doc->apertum || doc->sem == NIHIL
        || doc->parsura == NIHIL)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    {
        s32 fons = doc->parsura->fons_princeps;
        insignatus integer n = silva_c89_symbola_numerus(doc->sem);
        insignatus integer i;

        per (i = ZEPHYRUM; i < n; i++)
        {
            constans SemanticaSymbolum* s =
                silva_c89_symbolum_per_indicem(doc->sem, i);
            s32 a = -I;
            s32 b = ZEPHYRUM;
            s64 species;
            JsonValor* symbolum_v;
            JsonValor* sedes_v;

            si (s == NIHIL || s->ex_systemate
                || s->profunditas != ZEPHYRUM || s->est_implicitum
                || s->declarans == NIHIL)
            {
                perge;
            }
            silva_nodus_extensionem(s->declarans, fons, &a, &b);
            si (a < ZEPHYRUM)
            {
                perge;   /* symbolum plagulae alienae (capitis) */
            }
            commutatio (s->genus)
            {
                casus SYMBOLUM_FUNCTIO:  species = 12; frange;
                casus SYMBOLUM_TYPEDEF:  species = 5;  frange;
                casus SYMBOLUM_CONSTANS: species = 14; frange;
                ordinarius:              species = 13; frange;
            }
            symbolum_v = json_objectum_creare(pn);
            sedes_v = json_objectum_creare(pn);
            {
                chorda titulus;

                titulus.mensura = (i32)s->titulus.mensura;
                titulus.datum = (i8*)s->titulus.datum;
                json_objectum_ponere(symbolum_v, "name",
                    json_chorda_creare(pn, titulus));
            }
            json_objectum_ponere(symbolum_v, "kind",
                json_integer_creare(pn, species));
            json_objectum_ponere(sedes_v, "uri",
                json_chorda_creare(pn, doc->uri));
            json_objectum_ponere(sedes_v, "range",
                _regionem_ex_extentis(l, pn, doc, a, b));
            json_objectum_ponere(symbolum_v, "location", sedes_v);
            json_tabulatum_addere(lista, symbolum_v);
        }
    }
    _respondere(l, tabellarius_responsum(pn, id, lista));
}

/* ==================================================
 * initialize
 * ================================================== */

interior vacuum
_codificationem_negotiari (Legatus* l, JsonValor* params)
{
    JsonValor* v;

    l->utf16 = VERUM;   /* ordinarius LSP obligatorius */
    si (params == NIHIL)
    {
        redde;
    }
    v = json_objectum_capere(params, "capabilities");
    si (v == NIHIL)
    {
        redde;
    }
    v = json_objectum_capere(v, "general");
    si (v == NIHIL)
    {
        redde;
    }
    v = json_objectum_capere(v, "positionEncodings");
    si (v == NIHIL || !json_est_tabulatum(v))
    {
        redde;
    }
    {
        i32 n = json_tabulatum_numerus(v);
        i32 i;

        per (i = ZEPHYRUM; i < n; i++)
        {
            JsonValor* e = json_tabulatum_obtinere(v, i);

            si (e != NIHIL && json_est_chorda(e))
            {
                chorda c = json_ad_chorda(e);

                si (c.mensura == (i32)V && c.datum != NIHIL
                    && memcmp(c.datum, "utf-8", V) == ZEPHYRUM)
                {
                    l->utf16 = FALSUM;
                    redde;
                }
            }
        }
    }
}

interior vacuum
_radicem_statuere (Legatus* l, JsonValor* params,
    constans LegatusConfiguratio* cfg)
{
    l->radix[ZEPHYRUM] = '\0';
    si (cfg != NIHIL && cfg->radix != NIHIL)
    {
        memoriae_index m = strlen(cfg->radix);

        si (m < magnitudo(l->radix))
        {
            memcpy(l->radix, cfg->radix, m + I);
        }
    }
    alioquin si (params != NIHIL)
    {
        JsonValor* v = json_objectum_capere(params, "rootUri");

        si (v == NIHIL || !json_est_chorda(v))
        {
            JsonValor* plicae = json_objectum_capere(params,
                "workspaceFolders");

            si (plicae != NIHIL
                && json_tabulatum_numerus(plicae) > ZEPHYRUM)
            {
                JsonValor* prima = json_tabulatum_obtinere(plicae,
                    ZEPHYRUM);

                si (prima != NIHIL)
                {
                    v = json_objectum_capere(prima, "uri");
                }
            }
        }
        si (v != NIHIL && json_est_chorda(v))
        {
            (vacuum)_uri_ad_viam_absolutam(json_ad_chorda(v),
                l->radix, magnitudo(l->radix));
        }
    }
    /* caudam '/' tondere */
    {
        memoriae_index m = strlen(l->radix);

        dum (m > I && l->radix[m - I] == '/')
        {
            l->radix[m - I] = '\0';
            m--;
        }
        l->radix_mensura = m;
    }
}

interior vacuum
_initialize_tractare (Legatus* l, Piscina* pn, JsonValor* id,
    JsonValor* params, constans LegatusConfiguratio* cfg)
{
    si (l->initiatum)
    {
        _respondere(l, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_PETITIO_INVALIDA,
            "iam initiatum"));
        redde;
    }
    _radicem_statuere(l, params, cfg);
    _codificationem_negotiari(l, params);
    si (params != NIHIL)
    {
        JsonValor* optiones = json_objectum_capere(params,
            "initializationOptions");

        si (optiones != NIHIL)
        {
            JsonValor* posix_v = json_objectum_capere(optiones,
                "posix");

            si (posix_v != NIHIL && json_est_boolean(posix_v))
            {
                l->cum_posix = json_ad_boolean(posix_v);
            }
        }
    }

    /* praeparatio calida (systema + capita, ~0.15s semel) */
    si (l->radix_mensura > ZEPHYRUM)
    {
        PraeparatorConfiguratio pc;

        memset(&pc, ZEPHYRUM, magnitudo(PraeparatorConfiguratio));
        pc.radix = l->radix;
        pc.cum_posix = l->cum_posix ? I : ZEPHYRUM;
        pc.cum_latina = ZEPHYRUM;
        pc.sine_capitibus = ZEPHYRUM;
        l->praeparata = praeparator_praeparare(&l->praeparatio,
            l->perennis, &pc) ? VERUM : FALSUM;
    }
    si (!l->praeparata)
    {
        _respondere(l, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_INTERNUS,
            "apparatus praeparari non potuit"));
        redde;
    }
    _exclusiones_onerare(l);
    l->initiatum = VERUM;

    {
        JsonValor* resultatum = json_objectum_creare(pn);
        JsonValor* caps = json_objectum_creare(pn);
        JsonValor* sync = json_objectum_creare(pn);
        JsonValor* servus = json_objectum_creare(pn);

        json_objectum_ponere(sync, "openClose",
            json_boolean_creare(pn, VERUM));
        json_objectum_ponere(sync, "change",
            json_integer_creare(pn, I));
        json_objectum_ponere(sync, "save",
            json_boolean_creare(pn, VERUM));
        json_objectum_ponere(caps, "textDocumentSync", sync);
        json_objectum_ponere(caps, "positionEncoding",
            json_chorda_creare_literis(pn,
                l->utf16 ? "utf-16" : "utf-8"));
        json_objectum_ponere(caps, "hoverProvider",
            json_boolean_creare(pn, VERUM));
        json_objectum_ponere(caps, "documentSymbolProvider",
            json_boolean_creare(pn, VERUM));
        json_objectum_ponere(servus, "name",
            json_chorda_creare_literis(pn, "legatus"));
        json_objectum_ponere(servus, "version",
            json_chorda_creare_literis(pn, "0"));
        json_objectum_ponere(resultatum, "capabilities", caps);
        json_objectum_ponere(resultatum, "serverInfo", servus);
        _respondere(l, tabellarius_responsum(pn, id, resultatum));
    }
}

/* ==================================================
 * dispensator
 * ================================================== */

interior LegatusCursus
_nuntium_tractare (Legatus* l, Piscina* pn, TabellariusNuntius* n,
    constans LegatusConfiguratio* cfg)
{
    si (n->genus == TABELLARIUS_PRAVUM)
    {
        si (n->radix == NIHIL)
        {
            _respondere(l, tabellarius_errorem(pn, NIHIL,
                TABELLARIUS_ERROR_ANALYSIS, "analysis fracta"));
        }
        alioquin
        {
            _respondere(l, tabellarius_errorem(pn, n->id,
                TABELLARIUS_ERROR_PETITIO_INVALIDA,
                "petitio invalida"));
        }
        redde LEGATUS_PERGE;
    }
    si (n->genus == TABELLARIUS_RESPONSUM)
    {
        redde LEGATUS_PERGE;   /* numquam petimus - omittere */
    }
    si (n->genus == TABELLARIUS_PETITIO)
    {
        si (_methodus_est(n->methodus, "initialize"))
        {
            _initialize_tractare(l, pn, n->id, n->params, cfg);
        }
        alioquin si (!l->initiatum)
        {
            /* OMNIS petitio ante initialize (etiam shutdown) */
            _respondere(l, tabellarius_errorem(pn, n->id,
                TABELLARIUS_ERROR_NONDUM_INITIATUM,
                "nondum initiatum"));
        }
        alioquin si (_methodus_est(n->methodus, "shutdown"))
        {
            l->exitus_petitus = VERUM;
            _respondere(l, tabellarius_responsum(pn, n->id, NIHIL));
        }
        alioquin si (_methodus_est(n->methodus, "textDocument/hover"))
        {
            _hover_tractare(l, pn, n->id, n->params);
        }
        alioquin si (_methodus_est(n->methodus,
                         "textDocument/documentSymbol"))
        {
            _documentsymbol_tractare(l, pn, n->id, n->params);
        }
        alioquin
        {
            /* etiam petitiones "$/" - eadem responsio (-32601) */
            _respondere(l, tabellarius_errorem(pn, n->id,
                TABELLARIUS_ERROR_METHODUS_IGNOTA,
                "methodus ignota"));
        }
        redde LEGATUS_PERGE;
    }
    /* nuntiationes */
    si (_methodus_est(n->methodus, "exit"))
    {
        redde LEGATUS_FINI;
    }
    si (!l->initiatum || _praefixum_est(n->methodus, "$/"))
    {
        redde LEGATUS_PERGE;   /* tacite omissae */
    }
    si (_methodus_est(n->methodus, "textDocument/didOpen"))
    {
        _didopen_tractare(l, pn, n->params);
    }
    alioquin si (_methodus_est(n->methodus, "textDocument/didChange"))
    {
        _didchange_tractare(l, pn, n->params);
    }
    alioquin si (_methodus_est(n->methodus, "textDocument/didClose"))
    {
        _didclose_tractare(l, pn, n->params);
    }
    /* didSave: chunk D (lamina .h); ceterae nuntiationes tacitae */
    redde LEGATUS_PERGE;
}

/* ==================================================
 * ansa principalis
 * ================================================== */

s32
legatus_currere (FILE* intra, FILE* extra,
    constans LegatusConfiguratio* cfg)
{
    Legatus l;
    s32 exitus = I;   /* sine exit ordinato = abnormis */

    memset(&l, ZEPHYRUM, magnitudo(Legatus));
    l.extra = extra;
    l.utf16 = VERUM;
    si (cfg != NIHIL)
    {
        l.cum_posix = cfg->cum_posix;
    }
    l.perennis = piscina_generare_dynamicum("legatus", 1048576);
    si (l.perennis == NIHIL || intra == NIHIL || extra == NIHIL)
    {
        redde I;
    }
    l.documenta = tabula_dispersa_creare_chorda(l.perennis,
        (i32)LXIV);
    l.omnia_documenta = xar_creare(l.perennis,
        (i32)magnitudo(LegatusDocumentum*));

    per (;;)
    {
        Piscina* pn = piscina_generare_dynamicum("legatus_nuntius",
            LXIV * 1024);
        b32 finitus = FALSUM;
        chorda corpus;
        TabellariusNuntius n;
        LegatusCursus cursus;

        si (pn == NIHIL)
        {
            frange;
        }
        corpus = tabellarius_epistulam_legere(intra, pn, &finitus);
        si (finitus)
        {
            piscina_destruere(pn);
            frange;   /* cliens mortuus / fluxus corruptus -> 1 */
        }
        n = tabellarius_nuntium_legere(corpus, pn);
        cursus = _nuntium_tractare(&l, pn, &n, cfg);
        piscina_destruere(pn);
        si (cursus == LEGATUS_FINI)
        {
            exitus = l.exitus_petitus ? ZEPHYRUM : I;
            frange;
        }
    }

    /* purgatio (machina in-processu iterum agi potest) */
    si (l.omnia_documenta != NIHIL)
    {
        i32 n_doc = xar_numerus(l.omnia_documenta);
        i32 i;

        per (i = ZEPHYRUM; i < n_doc; i++)
        {
            LegatusDocumentum** doc = (LegatusDocumentum**)
                xar_obtinere(l.omnia_documenta, i);

            si (doc != NIHIL && *doc != NIHIL)
            {
                si ((*doc)->effimera != NIHIL)
                {
                    silva_piscina_destruere((*doc)->effimera);
                }
                si ((*doc)->piscina_versionis != NIHIL)
                {
                    piscina_destruere((*doc)->piscina_versionis);
                }
            }
        }
    }
    si (l.praeparata)
    {
        praeparator_destruere(&l.praeparatio);
    }
    piscina_destruere(l.perennis);
    redde exitus;
}
