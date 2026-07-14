/* legatus.c - LEGATUS: servus LSP silvae (vide legatus.h)
 *
 * Status: Praeparatio calida una (systema + capita, ~0.15s) +
 * thesaurus documentorum. Documentum per versionem: piscina
 * versionis (textus) + effimera silvae (arbores) - AMBAE deletae
 * cum versione proxima (C7: memoria ligata = documenta aperta).
 * Nuntius quisque piscinam propriam habet, deletam post
 * tractationem. Vigilia sui (excubitor): fontes proprii
 * recentiores binario residente -> monitum in omni publicatione.
 */

#include "legatus.h"

#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "tabula_dispersa.h"
#include "json.h"
#include "tabellarius.h"
#include "similitudo.h"
#include "silva.h"
#include "praeparator.h"
#include "nexus_ordines.h"

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

/* ordo indicis trans-plagularis (v0.1b): forma tsv nexus - chordae
 * = sectiones in textum tsv (onus) aut copiae (superpositio) */
nomen structura LegatusOrdo {
    chorda titulus;
    chorda genus;              /* "functio"/"variabile"/... */
    chorda via;                /* relativa radici, SINE "./" */
    insignatus integer linea;
    insignatus integer columna;
    insignatus integer profunditas;
    b32 sedes;                 /* VERUM = sedes, FALSUM = usus */
    b32 mortuus;               /* superpositio: plagula re-iudicata */
    structura LegatusOrdo* proximus;   /* catena eiusdem tituli */
} LegatusOrdo;

/* extentum symboli profunditatis 0 = CHARTA SYMBOLI (pars 2
 * mcp-legati): extenta functionum (vocantes/vocata) + signatura
 * reddita ad tempus iudicii (symbolum sine re-parsu) */
nomen structura {
    chorda titulus;
    insignatus integer linea_a;   /* 1-basata */
    insignatus integer linea_b;
    s32 genus;                    /* SemanticaSymbolumGenus - custos
                                   * continentiae (variabilis ne
                                   * vocans fingatur) */
    chorda signatura;             /* "titulus : typus" (vacua =
                                   * irreddibilis) */
} LegatusFunctioExtentum;

/* tempus viae ad iudicium commemoratum (gradus III legis aetatum;
 * identitas cum se ipso - differentia quaevis = stalum) */
nomen structura {
    longus tempus;
} LegatusViaTempus;

nomen structura {
    Piscina*        perennis;
    Piscina*        piscina_capitum;   /* textus capitum; moritur
                                        * cum praeparatione
                                        * (reaedificatio ligata) */
    Piscina*        piscina_indicis;   /* tsv + ordines + extenta
                                        * (crescit cum superpositione
                                        * - ligatum editionibus) */
    TabulaDispersa* index_titulorum;   /* titulus -> LegatusOrdo*
                                        * (caput catenae) */
    Xar*            omnes_ordines;     /* LegatusOrdo* (necatio per
                                        * plagulam) */
    Xar*            nomina_indicis;    /* chorda (tituli distincti -
                                        * workspaceSymbol) */
    TabulaDispersa* extenta_functionum; /* via -> Xar<
                                        * LegatusFunctioExtentum>
                                        * (plagulae clausae) */
    TabulaDispersa* tempora_viarum;    /* via -> LegatusViaTempus*
                                        * (gradus III; in piscina
                                        * indicis - moritur cum ea) */
    longus          index_tempus;      /* mtime tsv ad onus (gradus
                                        * II; identitas) */
    b32             index_paratus;
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

    /* vigilia sui (excubitor chunk 3): binarium residens = classis
     * quam nullum scriptum videt. Clausura fontium propriorum ex
     * fabrica.tsv (ordines binarii) + build/inclusiones.tsv;
     * fons recentior binario -> diagnosticum in omni publicatione */
    TabulaDispersa* clausura_sua;      /* via (sine "./") -> NIHIL */
    longus          binarium_tempus;   /* mtime binarii; 0 = quieta */
    b32             se_stalus;
    character       stalus_causa[LEGATUS_VIA_MAXIMA];

    /* LEGATI (modus MCP): framing lineis, methodi tools -
     * quaestiones nomine-basatae; sine documentis */
    b32             modus_mcp;
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

interior chorda
_transcribere_nul_ex (Piscina* piscina,
    constans insignatus character* datum, insignatus integer mensura)
{
    chorda c;
    character* d = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura + I);

    c.mensura = ZEPHYRUM;
    c.datum = NIHIL;
    si (d == NIHIL)
    {
        redde c;
    }
    si (mensura > ZEPHYRUM && datum != NIHIL)
    {
        memcpy(d, datum, (memoriae_index)mensura);
    }
    d[mensura] = '\0';
    c.mensura = mensura;
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
 * index trans-plagularis (v0.1b): tsv nexus ad initialize +
 * superpositio viva per nexus_ordines in quaque analysi
 * ================================================== */

interior b32
_chorda_continet_literis (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);
    memoriae_index i;

    si (c.datum == NIHIL || (memoriae_index)c.mensura < m)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i + m <= (memoriae_index)c.mensura; i++)
    {
        si (memcmp(c.datum + i, litterae, m) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_chorda_incipit_literis (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (c.datum != NIHIL && (memoriae_index)c.mensura >= m
        && memcmp(c.datum, litterae, m) == ZEPHYRUM) ? VERUM : FALSUM;
}

interior b32
_chorda_desinit_literis (chorda c, constans character* litterae)
{
    memoriae_index m = strlen(litterae);

    redde (c.datum != NIHIL && (memoriae_index)c.mensura >= m
        && memcmp(c.datum + ((memoriae_index)c.mensura - m),
               litterae, m) == ZEPHYRUM) ? VERUM : FALSUM;
}

interior b32
_chordae_pares (chorda a, chorda b)
{
    redde (a.mensura == b.mensura && a.datum != NIHIL
        && b.datum != NIHIL
        && memcmp(a.datum, b.datum, (memoriae_index)a.mensura)
               == ZEPHYRUM) ? VERUM : FALSUM;
}

/* politica viarum (Q2 explorationis quintae): genita/fixturae
 * NUMQUAM sedes saltus; knotapel/tools realia sed secundaria */
interior b32
_via_omissa (chorda via)
{
    redde (_chorda_continet_literis(via, "amalgama/")
        || _chorda_continet_literis(via, "fixa/")) ? VERUM : FALSUM;
}

interior b32
_via_secundaria (chorda via)
{
    redde (_chorda_incipit_literis(via, "knotapel/")
        || _chorda_incipit_literis(via, "tools/")) ? VERUM : FALSUM;
}

interior insignatus integer
_numerus_ex_chorda (chorda c)
{
    insignatus integer n = ZEPHYRUM;
    memoriae_index i;

    per (i = ZEPHYRUM; i < (memoriae_index)c.mensura; i++)
    {
        character d = (character)c.datum[i];

        si (d < '0' || d > '9')
        {
            frange;
        }
        n = n * X + (insignatus integer)(d - '0');
    }
    redde n;
}

interior vacuum
_ordinem_indici_addere (Legatus* l, chorda titulus, b32 sedes,
    chorda genus, chorda via, insignatus integer linea,
    insignatus integer columna, insignatus integer profunditas)
{
    LegatusOrdo* ordo = (LegatusOrdo*)piscina_allocare(
        l->piscina_indicis, magnitudo(LegatusOrdo));
    vacuum* caput = NIHIL;

    si (ordo == NIHIL || titulus.mensura == ZEPHYRUM)
    {
        redde;
    }
    ordo->titulus = titulus;
    ordo->genus = genus;
    ordo->via = via;
    ordo->linea = linea;
    ordo->columna = columna;
    ordo->profunditas = profunditas;
    ordo->sedes = sedes;
    ordo->mortuus = FALSUM;
    si (tabula_dispersa_invenire(l->index_titulorum, titulus,
            &caput))
    {
        ordo->proximus = (LegatusOrdo*)caput;
    }
    alioquin
    {
        ordo->proximus = NIHIL;
        /* titulus novus - in indicem nominum (workspaceSymbol) */
        {
            chorda* locus = (chorda*)xar_addere(l->nomina_indicis);

            si (locus != NIHIL)
            {
                *locus = titulus;
            }
        }
    }
    (vacuum)tabula_dispersa_inserere(l->index_titulorum, titulus,
        ordo);
    {
        LegatusOrdo** locus = (LegatusOrdo**)xar_addere(
            l->omnes_ordines);

        si (locus != NIHIL)
        {
            *locus = ordo;
        }
    }
}

interior vacuum
_indicem_onerare (Legatus* l)
{
    character via_tsv[LEGATUS_VIA_MAXIMA + LXIV];
    insignatus integer mensura = ZEPHYRUM;
    character* textus;

    l->piscina_indicis = piscina_generare_dynamicum("legatus_index",
        1048576);
    si (l->piscina_indicis == NIHIL)
    {
        redde;
    }
    l->index_titulorum = tabula_dispersa_creare_chorda(
        l->piscina_indicis, (i32)DXII * XVI);
    l->omnes_ordines = xar_creare(l->piscina_indicis,
        (i32)magnitudo(LegatusOrdo*));
    l->nomina_indicis = xar_creare(l->piscina_indicis,
        (i32)magnitudo(chorda));
    l->extenta_functionum = tabula_dispersa_creare_chorda(
        l->piscina_indicis, (i32)CXXVIII);
    l->tempora_viarum = tabula_dispersa_creare_chorda(
        l->piscina_indicis, (i32)CXXVIII);
    si (l->index_titulorum == NIHIL || l->omnes_ordines == NIHIL
        || l->nomina_indicis == NIHIL
        || l->extenta_functionum == NIHIL)
    {
        redde;
    }
    l->index_paratus = VERUM;   /* etiam vacuus - superpositio agit */

    sprintf(via_tsv, "%s/build/nexus.tsv", l->radix);
    l->index_tempus = praeparator_tempus_plagulae(via_tsv);
    textus = praeparator_plagulam_legere(l->piscina_indicis,
        via_tsv, &mensura);
    si (textus == NIHIL)
    {
        fprintf(stderr, "legatus: nexus.tsv deest (%s) - index"
            " superpositione sola crescet\n", via_tsv);
        redde;
    }
    {
        memoriae_index i = ZEPHYRUM;
        insignatus integer ordines = ZEPHYRUM;

        dum (i < (memoriae_index)mensura)
        {
            chorda campi[VII];
            s32 campus = ZEPHYRUM;
            memoriae_index initium = i;

            /* lineam in septem campos secare */
            dum (i < (memoriae_index)mensura && textus[i] != '\n')
            {
                si (textus[i] == '\t' && campus < (s32)VI)
                {
                    campi[campus].datum = (i8*)(textus + initium);
                    campi[campus].mensura = (i32)(i - initium);
                    campus++;
                    initium = i + I;
                }
                i++;
            }
            campi[campus].datum = (i8*)(textus + initium);
            campi[campus].mensura = (i32)(i - initium);
            si (i < (memoriae_index)mensura)
            {
                i++;   /* '\n' */
            }
            si (campus != (s32)VI
                || campi[ZEPHYRUM].mensura == ZEPHYRUM
                || campi[ZEPHYRUM].datum[ZEPHYRUM] == '#')
            {
                perge;   /* commentarium / linea prava */
            }
            _ordinem_indici_addere(l, campi[ZEPHYRUM],
                (campi[I].mensura == (i32)V
                 && memcmp(campi[I].datum, "sedes", V) == ZEPHYRUM)
                    ? VERUM : FALSUM,
                campi[II], campi[III],
                _numerus_ex_chorda(campi[IV]),
                _numerus_ex_chorda(campi[V]),
                _numerus_ex_chorda(campi[VI]));
            ordines++;
        }
        fprintf(stderr, "legatus: index %u ordines (nexus.tsv)\n",
            ordines);
    }
}

/* superpositio: ordines vivos plagulae huius derivare (unitas
 * EADEM ac sweep - divergentia impossibilis constructione) */
interior vacuum
_superpositio_recipere (vacuum* datum,
    constans SilvaChorda* titulus, constans character* species,
    constans character* genus, constans SilvaChorda* via,
    insignatus integer linea, insignatus integer columna,
    insignatus integer profunditas)
{
    Legatus* l = (Legatus*)datum;
    chorda titulus_c;
    chorda genus_c;
    chorda via_c;
    constans insignatus character* via_datum = via->datum;
    insignatus integer via_mensura = via->mensura;

    si (via_mensura > II && via_datum[ZEPHYRUM] == '.'
        && via_datum[I] == '/')
    {
        via_datum += II;
        via_mensura -= II;
    }
    titulus_c = _transcribere_nul_ex(l->piscina_indicis,
        titulus->datum, titulus->mensura);
    via_c = _transcribere_nul_ex(l->piscina_indicis, via_datum,
        via_mensura);
    genus_c = chorda_ex_literis(genus, l->piscina_indicis);
    si (titulus_c.datum == NIHIL || via_c.datum == NIHIL)
    {
        redde;
    }
    _ordinem_indici_addere(l, titulus_c,
        strcmp(species, "sedes") == ZEPHYRUM ? VERUM : FALSUM,
        genus_c, via_c, linea, columna, profunditas);
}

/* ordines veteres plagulae necare (superpositio substituit) */
interior vacuum
_ordines_plagulae_necare (Legatus* l, chorda via_sine)
{
    i32 n = xar_numerus(l->omnes_ordines);
    i32 i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        LegatusOrdo** ordo = (LegatusOrdo**)xar_obtinere(
            l->omnes_ordines, i);

        si (ordo != NIHIL && *ordo != NIHIL && !(*ordo)->mortuus
            && (*ordo)->via.mensura == via_sine.mensura
            && memcmp((*ordo)->via.datum, via_sine.datum,
                   (memoriae_index)via_sine.mensura) == ZEPHYRUM)
        {
            (*ordo)->mortuus = VERUM;
        }
    }
}

/* GRADUS II legis aetatum (spec-v2 §V): tsv mutatum (uncus
 * post-commissionem regenerat) -> piscina indicis TOTA nova
 * (superpositio + chartae + tempora simul moriuntur - sub indice
 * vetere derivata). Deinde superpositio documentorum APERTORUM
 * re-emittitur ex arboribus vivis (sine re-parsu - foramen
 * editionum non-commissarum clausum). vi = etiam sine mutatione
 * (cascade gradus I). */
interior vacuum
_indicem_renovare (Legatus* l, Piscina* pn, b32 vi)
{
    character via_tsv[LEGATUS_VIA_MAXIMA + LXIV];

    si (!l->index_paratus || l->piscina_indicis == NIHIL
        || l->radix_mensura == ZEPHYRUM)
    {
        redde;
    }
    sprintf(via_tsv, "%s/build/nexus.tsv", l->radix);
    si (!vi && praeparator_tempus_plagulae(via_tsv)
            == l->index_tempus)
    {
        redde;
    }
    fprintf(stderr, "legatus: index renovatur (tsv %s)\n",
        vi ? "coactus" : "mutatus");
    piscina_destruere(l->piscina_indicis);
    l->piscina_indicis = NIHIL;
    l->index_titulorum = NIHIL;
    l->omnes_ordines = NIHIL;
    l->nomina_indicis = NIHIL;
    l->extenta_functionum = NIHIL;
    l->tempora_viarum = NIHIL;
    l->index_paratus = FALSUM;
    _indicem_onerare(l);
    si (!l->index_paratus || l->omnia_documenta == NIHIL)
    {
        redde;
    }
    {
        i32 n_doc = xar_numerus(l->omnia_documenta);
        i32 i;

        per (i = ZEPHYRUM; i < n_doc; i++)
        {
            LegatusDocumentum** doc_p = (LegatusDocumentum**)
                xar_obtinere(l->omnia_documenta, i);
            LegatusDocumentum* doc;
            chorda via_sine;

            si (doc_p == NIHIL || *doc_p == NIHIL)
            {
                perge;
            }
            doc = *doc_p;
            si (!doc->apertum || doc->sem == NIHIL
                || doc->parsura == NIHIL
                || doc->via.mensura <= (i32)II)
            {
                perge;
            }
            via_sine.datum = doc->via.datum + II;
            via_sine.mensura = doc->via.mensura - II;
            _ordines_plagulae_necare(l, via_sine);
            nexus_ordines_fundere(doc->parsura, doc->sem, pn,
                _superpositio_recipere, l);
        }
    }
}

interior LegatusOrdo*
_ordines_tituli (Legatus* l, chorda titulus)
{
    vacuum* caput = NIHIL;

    si (!l->index_paratus
        || !tabula_dispersa_invenire(l->index_titulorum, titulus,
               &caput))
    {
        redde NIHIL;
    }
    redde (LegatusOrdo*)caput;
}

interior chorda
_titulus_symboli (constans SemanticaSymbolum* s)
{
    chorda c;

    c.mensura = (i32)s->titulus.mensura;
    c.datum = (i8*)s->titulus.datum;
    redde c;
}

/* symbolum localis/staticus = quaestio plagulae solius */
interior b32
_symbolum_plagulae_solius (constans SemanticaSymbolum* s)
{
    redde (s->profunditas > ZEPHYRUM
        || (s->repositio & (insignatus integer)REPOSITIO_STATICA))
        ? VERUM : FALSUM;
}

/* "titulus : typus" ex symbolo SOLO (extracta ex hover, pars 2
 * mcp-legati - chartae symbolorum eadem via redduntur); typus
 * irreddibilis (acies) -> nomen solum. Redde mensuram scriptam. */
interior memoriae_index
_signaturam_scribere (constans SemanticaSymbolum* s,
    character* buffer, memoriae_index capacitas)
{
    character typus_b[CCLVI];

    si (capacitas < (memoriae_index)(s->titulus.mensura + CCLVI + IV))
    {
        redde ZEPHYRUM;
    }
    si (silva_c89_typum_scribere(s->typus, typus_b,
            (insignatus integer)CCLVI) > ZEPHYRUM)
    {
        sprintf(buffer, "%.*s : %s", (int)s->titulus.mensura,
            (constans character*)s->titulus.datum, typus_b);
    }
    alioquin
    {
        sprintf(buffer, "%.*s", (int)s->titulus.mensura,
            (constans character*)s->titulus.datum);
    }
    redde strlen(buffer);
}

/* ==================================================
 * scriptura responsorum / publicationum
 * ================================================== */

interior vacuum
_respondere (Legatus* l, chorda corpus)
{
    si (l->modus_mcp)
    {
        (vacuum)tabellarius_lineam_scribere(l->extra, corpus);
    }
    alioquin
    {
        tabellarius_epistulam_scribere(l->extra, corpus);
    }
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
    constans character* nuntius, constans character* fons)
{
    JsonValor* diag = json_objectum_creare(p);

    json_objectum_ponere(diag, "range",
        _regio_json(p, l0, c0, l1, c1));
    json_objectum_ponere(diag, "severity",
        json_integer_creare(p, (s64)severitas));
    json_objectum_ponere(diag, "source",
        json_chorda_creare_literis(p, fons));
    json_objectum_ponere(diag, "message",
        json_chorda_creare_literis(p, nuntius));
    redde diag;
}

/* Location ex ordine indicis (finis = initium + titulus) */
interior JsonValor*
_sedes_ex_ordine (Legatus* l, Piscina* pn, constans LegatusOrdo* o)
{
    character uri_b[LEGATUS_VIA_MAXIMA + LXIV];
    JsonValor* sedes_v = json_objectum_creare(pn);
    insignatus integer l0 = o->linea > ZEPHYRUM ? o->linea - I
                                                : ZEPHYRUM;
    insignatus integer c0 = o->columna > ZEPHYRUM ? o->columna - I
                                                  : ZEPHYRUM;

    si ((memoriae_index)o->via.mensura + l->radix_mensura + XVI
        >= magnitudo(uri_b))
    {
        redde NIHIL;
    }
    sprintf(uri_b, "file://%s/%.*s", l->radix,
        (int)o->via.mensura, (constans character*)o->via.datum);
    json_objectum_ponere(sedes_v, "uri",
        json_chorda_creare_literis(pn, uri_b));
    json_objectum_ponere(sedes_v, "range", _regio_json(pn, l0, c0,
        l0, c0 + (insignatus integer)o->titulus.mensura));
    redde sedes_v;
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

    /* errores syntaxis POSITI (chunk D, C13): nodi GENUS_ERROR in
     * elementis radicis; extenta per radicem originis. Effugium
     * intactum: nullo nodo posito (radices omnes syntheticae) ->
     * extensio degenerata 0:0. */
    si (doc->parsura != NIHIL
        && doc->parsura->numerus_errorum > ZEPHYRUM)
    {
        character nuntius_b[CCLVI];
        insignatus integer positi = ZEPHYRUM;

        sprintf(nuntius_b, "syntaxis fracta (%u loci)",
            doc->parsura->numerus_errorum);
        si (doc->parsura->commissio != NIHIL)
        {
            SilvaValor radix = doc->parsura->commissio->radix;
            insignatus integer n_el = silva_valor_lista_numerus(
                radix);
            insignatus integer k;
            s32 fons = doc->parsura->fons_princeps;

            per (k = ZEPHYRUM; k < n_el; k++)
            {
                SilvaValor* v = silva_valor_lista_obtinere(radix,
                    k);
                s32 a = -I;
                s32 b = ZEPHYRUM;

                si (v == NIHIL || v->genus != SILVA_VALOR_NODUS
                    || v->datum.nodus == NIHIL
                    || v->datum.nodus->genus
                        != (int)SILVA_C89_GENUS_ERROR)
                {
                    perge;
                }
                silva_nodus_extensionem(v->datum.nodus, fons, &a,
                    &b);
                si (a < ZEPHYRUM)
                {
                    perge;   /* radices omnes syntheticae */
                }
                {
                    insignatus integer l0;
                    insignatus integer c0;
                    insignatus integer l1;
                    insignatus integer c1;

                    _byte_ad_positio(l, doc, (memoriae_index)a,
                        &l0, &c0);
                    _byte_ad_positio(l, doc, (memoriae_index)b,
                        &l1, &c1);
                    json_tabulatum_addere(lista,
                        _diagnosticum_json(pn, l0, c0, l1, c1, I,
                            nuntius_b, "silva"));
                    positi++;
                }
            }
        }
        si (positi == ZEPHYRUM)
        {
            json_tabulatum_addere(lista, _diagnosticum_json(pn,
                ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, I, nuntius_b,
                "silva"));
        }
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
                    severitas_lsp, nuntius_b, "silva"));
            }
            alioquin
            {
                json_tabulatum_addere(lista, _diagnosticum_json(pn,
                    ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, severitas_lsp,
                    nuntius_b, "silva"));
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

    /* vigilia sui: documentum in clausura propria + discus recentior
     * binario = stalus (stat arbiter - apertio mere lectionis non
     * fallit). Semel stalus, monitum in OMNI publicatione manet
     * (responsa quaevis nunc suspecta) donec /reload-plugins. */
    si (!l->se_stalus && l->clausura_sua != NIHIL
        && doc->via.mensura > II)
    {
        chorda via_sine;

        via_sine.datum = doc->via.datum + II;
        via_sine.mensura = doc->via.mensura - II;
        si (tabula_dispersa_continet(l->clausura_sua, via_sine))
        {
            character via_b[LEGATUS_VIA_MAXIMA];

            si ((memoriae_index)via_sine.mensura < magnitudo(via_b))
            {
                longus t;

                sprintf(via_b, "%.*s", (int)via_sine.mensura,
                    (constans character*)via_sine.datum);
                t = praeparator_tempus_plagulae(via_b);
                si (t > l->binarium_tempus)
                {
                    l->se_stalus = VERUM;
                    sprintf(l->stalus_causa, "%s", via_b);
                }
            }
        }
    }
    si (l->se_stalus)
    {
        character nuntius_b[LEGATUS_VIA_MAXIMA + CXXVIII];

        sprintf(nuntius_b, "LEGATUS IPSE STALUS: %s recentior "
            "binario residente - responsa fortasse vetera; "
            "/reload-plugins renovat", l->stalus_causa);
        json_tabulatum_addere(lista, _diagnosticum_json(pn,
            ZEPHYRUM, ZEPHYRUM, ZEPHYRUM, I, II, nuntius_b,
            "excubitor"));
    }

    _publicare(l, pn, doc->uri, doc->versio, lista);

    /* superpositio indicis (v0.1b): ordines plagulae huius ex
     * analysi viva re-derivati - unitate EADEM ac sweep */
    si (l->index_paratus && doc->sem != NIHIL
        && doc->parsura != NIHIL && doc->via.mensura > II)
    {
        chorda via_sine;

        via_sine.datum = doc->via.datum + II;   /* sine "./" */
        via_sine.mensura = doc->via.mensura - II;
        _ordines_plagulae_necare(l, via_sine);
        nexus_ordines_fundere(doc->parsura, doc->sem, pn,
            _superpositio_recipere, l);
        (vacuum)tabula_dispersa_delere(l->extenta_functionum,
            via_sine);
    }
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

nomen structura {
    constans SemanticaSymbolum* symbolum;
    constans TypusC89*          typus;
    s32                         a;    /* extentum nodi electi */
    s32                         b;
} LegatusInventum;

/* descensus per extenta: symbolum/typus profundissimum ad octetum
 * (sedes USUS - nexus symbolorum ibi solum registrat) */
interior vacuum
_invenire_ad_byte (LegatusDocumentum* doc, memoriae_index octetum,
    LegatusInventum* inv)
{
    s32 fons = doc->parsura->fons_princeps;
    s32 a_e = -I;
    s32 b_e = ZEPHYRUM;
    constans SilvaNodus* nodus = NIHIL;

    inv->symbolum = NIHIL;
    inv->typus = NIHIL;
    inv->a = -I;
    inv->b = ZEPHYRUM;
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
    dum (nodus != NIHIL)
    {
        constans SemanticaSymbolum* s = silva_c89_symbolum_nodi(
            doc->sem, nodus);
        constans TypusC89* t = silva_c89_typus_expressionis(
            doc->sem, nodus);

        si (s != NIHIL)
        {
            inv->symbolum = s;
            inv->typus = NIHIL;
            inv->a = a_e;
            inv->b = b_e;
        }
        alioquin si (t != NIHIL)
        {
            inv->typus = t;
            inv->symbolum = NIHIL;
            inv->a = a_e;
            inv->b = b_e;
        }
        nodus = _filium_continentem(nodus, fons, (s32)octetum,
            &a_e, &b_e);
    }
}

/* nomen DECLARATUM ad octetum: symbolum->lexema = lexema nominis
 * declaratoris (inventum: _symbolum_registrare tok nominis tradit
 * - datum aderat ab M0!). Nexus sedes usus solum tegit; haec
 * ambulatio declarationes ipsas tegit. O(symbola) - satis. */
interior constans SemanticaSymbolum*
_symbolum_declaratum_ad_byte (LegatusDocumentum* doc,
    memoriae_index octetum)
{
    s32 fons = doc->parsura->fons_princeps;
    insignatus integer n = silva_c89_symbola_numerus(doc->sem);
    insignatus integer i;

    per (i = ZEPHYRUM; i < n; i++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(doc->sem, i);
        SilvaToken* radix_t;

        si (s == NIHIL || s->lexema == NIHIL)
        {
            perge;
        }
        radix_t = silva_token_radix(s->lexema);
        si (radix_t == NIHIL)
        {
            radix_t = s->lexema;
        }
        si (radix_t->fons_index == fons
            && radix_t->byte_offset >= ZEPHYRUM
            && (memoriae_index)radix_t->byte_offset <= octetum
            && octetum < (memoriae_index)radix_t->byte_offset
                + radix_t->longitudo)
        {
            redde s;
        }
    }
    redde NIHIL;
}

/* Location ex symbolo (definitio): lexema nominis -> uri + regio.
 * Fons princeps: conversio plena (tabula linearum). Fons alienus
 * (caput praebitum): uri per praeparatio.viae_capitum (basename ->
 * via absoluta), columnae octetorum (approximatio sub utf-16 -
 * capita quasi-ASCII, nota in worklog). NIHIL = sine sede honesta
 * (lexema syntheticum / systema / basename ignotum). */
interior JsonValor*
_sedes_ex_symbolo (Legatus* l, Piscina* pn, LegatusDocumentum* doc,
    constans SemanticaSymbolum* s)
{
    SilvaToken* radix_t;

    si (s == NIHIL || s->lexema == NIHIL)
    {
        redde NIHIL;
    }
    radix_t = silva_token_radix(s->lexema);
    si (radix_t == NIHIL)
    {
        radix_t = s->lexema;
    }
    si (radix_t->byte_offset < ZEPHYRUM)
    {
        redde NIHIL;
    }
    {
        JsonValor* sedes_v = json_objectum_creare(pn);
        JsonValor* regio;

        si (radix_t->fons_index == doc->parsura->fons_princeps)
        {
            insignatus integer l0;
            insignatus integer c0;
            insignatus integer l1;
            insignatus integer c1;

            _byte_ad_positio(l, doc,
                (memoriae_index)radix_t->byte_offset, &l0, &c0);
            _byte_ad_positio(l, doc,
                (memoriae_index)radix_t->byte_offset
                    + radix_t->longitudo, &l1, &c1);
            regio = _regio_json(pn, l0, c0, l1, c1);
            json_objectum_ponere(sedes_v, "uri",
                json_chorda_creare(pn, doc->uri));
        }
        alioquin
        {
            constans SilvaChorda* via_f = silva_fons_via(
                doc->parsura->expansio, radix_t->fons_index);
            character basename_b[CCLVI];
            chorda clavis;
            vacuum* via_plena = NIHIL;
            character uri_b[LEGATUS_VIA_MAXIMA + VIII];
            insignatus integer c0;

            si (via_f == NIHIL
                || l->praeparatio.viae_capitum == NIHIL
                || via_f->mensura >= (unsigned int)CCLVI)
            {
                redde NIHIL;
            }
            memcpy(basename_b, via_f->datum, via_f->mensura);
            clavis.mensura = (i32)via_f->mensura;
            clavis.datum = (i8*)basename_b;
            si (!tabula_dispersa_invenire(
                    l->praeparatio.viae_capitum, clavis,
                    &via_plena)
                || via_plena == NIHIL)
            {
                redde NIHIL;   /* systema aut basename ignotum */
            }
            si (strlen((constans character*)via_plena) + VIII
                >= magnitudo(uri_b))
            {
                redde NIHIL;
            }
            sprintf(uri_b, "file://%s",
                (constans character*)via_plena);
            json_objectum_ponere(sedes_v, "uri",
                json_chorda_creare_literis(pn, uri_b));
            c0 = radix_t->columna > ZEPHYRUM ? radix_t->columna - I
                                             : ZEPHYRUM;
            regio = _regio_json(pn,
                radix_t->linea > ZEPHYRUM ? radix_t->linea - I
                                          : ZEPHYRUM,
                c0,
                radix_t->linea > ZEPHYRUM ? radix_t->linea - I
                                          : ZEPHYRUM,
                c0 + radix_t->longitudo);
        }
        json_objectum_ponere(sedes_v, "range", regio);
        redde sedes_v;
    }
}

/* ==================================================
 * Macra (v0.2): resolutio positionis per ascensum originis
 * (nexus_ordines_titulus_macronis - EADEM logica quam emissio
 * indicis) + gradus verbi crudi (lineae directivarum: lexemata
 * consumpta, nihil expansum ibi). Corpus hover = linea #define ad
 * postulatum lecta (vista corpus non fert - inventum
 * explorationis; documentum apertum ante discum).
 * ================================================== */

/* ordo indicis genus "macro" fert? (ordines symbolorum et macrorum
 * catenas titulorum COMMUNICANT - custodes generis in ambabus viis) */
interior b32
_ordo_macro_est (constans LegatusOrdo* o)
{
    redde o->genus.mensura == (i32)V
        && memcmp(o->genus.datum, "macro", V) == ZEPHYRUM;
}

interior b32
_ordo_functio_est (constans LegatusOrdo* o)
{
    redde o->genus.mensura == (i32)VII
        && memcmp(o->genus.datum, "functio", VII) == ZEPHYRUM;
}

interior b32
_character_identificatoris (character c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || c == '_';
}

/* macro ad octetum. gradus I: lexema expansum cuius RADIX originis
 * positionem tegit (provenientia vera - lexemata expansa sedem
 * definitionis ferunt, radix sedem invocationis; trap
 * explorationis). gradus II: verbum crudum in textu (directivae
 * consumptae; corpora vacua invisibilia gradui I). titulus_out in
 * textum documenti aut piscinam parsurae spectat. */
interior b32
_macro_ad_byte (LegatusDocumentum* doc, memoriae_index octetum,
    b32 cum_verbo, chorda* titulus_out, s32* a_out, s32* b_out)
{
    si (doc->parsura != NIHIL && doc->parsura->lexemata != NIHIL)
    {
        insignatus integer n = silva_xar_numerus(
            doc->parsura->lexemata);
        insignatus integer k;

        per (k = ZEPHYRUM; k < n; k++)
        {
            SilvaToken** cella = (SilvaToken**)silva_xar_obtinere(
                doc->parsura->lexemata, k);
            SilvaToken* tok;
            SilvaToken* radix;

            si (cella == NIHIL || *cella == NIHIL)
            {
                perge;
            }
            tok = *cella;
            si (tok->origo.genus == SILVA_ORIGO_FONS)
            {
                perge;
            }
            radix = silva_token_radix(tok);
            si (radix == NIHIL
                || radix->fons_index
                    != doc->parsura->fons_princeps
                || radix->byte_offset < ZEPHYRUM)
            {
                perge;
            }
            si (octetum < (memoriae_index)radix->byte_offset
                || octetum >= (memoriae_index)radix->byte_offset
                    + (memoriae_index)radix->longitudo)
            {
                perge;
            }
            {
                constans SilvaChorda* t =
                    nexus_ordines_titulus_macronis(tok);

                si (t == NIHIL)
                {
                    perge;
                }
                titulus_out->mensura = (i32)t->mensura;
                titulus_out->datum = (i8*)t->datum;
                *a_out = radix->byte_offset;
                *b_out = radix->byte_offset
                    + (s32)radix->longitudo;
                redde VERUM;
            }
        }
    }
    si (!cum_verbo)
    {
        redde FALSUM;   /* gradus verbi crudi non petitus */
    }
    {
        memoriae_index a = octetum;
        memoriae_index b = octetum;
        memoriae_index m = (memoriae_index)doc->textus.mensura;

        si (octetum >= m
            || !_character_identificatoris(
                   (character)doc->textus.datum[octetum]))
        {
            redde FALSUM;
        }
        dum (a > ZEPHYRUM && _character_identificatoris(
                (character)doc->textus.datum[a - I]))
        {
            a--;
        }
        dum (b < m && _character_identificatoris(
                (character)doc->textus.datum[b]))
        {
            b++;
        }
        titulus_out->mensura = (i32)(b - a);
        titulus_out->datum = doc->textus.datum + a;
        *a_out = (s32)a;
        *b_out = (s32)b;
        redde VERUM;
    }
}

/* viae congruunt si aequae aut altera in alteram desinit (limite
 * '/'): fons praebitus = basename ("latina.h"), fons princeps =
 * via absoluta; ordo indicis = relativa sine "./" */
interior b32
_viae_congruunt (chorda a, constans SilvaChorda* b)
{
    constans i8* longa;
    constans i8* brevis_d;
    insignatus integer m_longae;
    insignatus integer m_brevis;

    si (a.mensura == (i32)b->mensura)
    {
        redde memcmp(a.datum, b->datum,
            (memoriae_index)a.mensura) == ZEPHYRUM;
    }
    si (a.mensura > (i32)b->mensura)
    {
        longa = a.datum;
        m_longae = a.mensura;
        brevis_d = (constans i8*)b->datum;
        m_brevis = b->mensura;
    }
    alioquin
    {
        longa = (constans i8*)b->datum;
        m_longae = b->mensura;
        brevis_d = a.datum;
        m_brevis = (insignatus integer)a.mensura;
    }
    si (m_brevis == ZEPHYRUM
        || longa[m_longae - m_brevis - I] != '/')
    {
        redde FALSUM;
    }
    redde memcmp(longa + (m_longae - m_brevis), brevis_d,
        (memoriae_index)m_brevis) == ZEPHYRUM;
}

/* extentum corporis macronis ex expansione documenti petentis:
 * vista cuius titulus + linea + via sedi indicis congruunt.
 * -1 = non visibilis in hoc TU (verbum crudum in macro aliena) -
 * consumptor ad lineam solam recidit. */
interior s32
_corpus_finis_macronis (LegatusDocumentum* doc,
    constans LegatusOrdo* sedes)
{
    insignatus integer n;
    insignatus integer k;

    si (doc == NIHIL || doc->parsura == NIHIL
        || doc->parsura->expansio == NIHIL)
    {
        redde -I;
    }
    n = silva_macros_numerus(doc->parsura->expansio);
    per (k = ZEPHYRUM; k < n; k++)
    {
        SilvaMacroVista vista;
        constans SilvaChorda* via_fontis;

        si (!silva_macro_vista(doc->parsura->expansio, k, &vista)
            || vista.titulus == NIHIL
            || vista.corpus_finis < ZEPHYRUM)
        {
            perge;
        }
        si (vista.linea != sedes->linea)
        {
            perge;
        }
        si ((i32)vista.titulus->mensura != sedes->titulus.mensura
            || memcmp(vista.titulus->datum, sedes->titulus.datum,
                   (memoriae_index)sedes->titulus.mensura)
                != ZEPHYRUM)
        {
            perge;
        }
        via_fontis = silva_fons_via(doc->parsura->expansio,
            vista.fons_index);
        si (via_fontis == NIHIL
            || !_viae_congruunt(sedes->via, via_fontis))
        {
            perge;
        }
        redde vista.corpus_finis;
    }
    redde -I;
}

/* prima sedes macronis in indice (via non omissa) */
interior LegatusOrdo*
_sedes_macronis (Legatus* l, chorda titulus)
{
    LegatusOrdo* o;

    si (!l->index_paratus)
    {
        redde NIHIL;
    }
    per (o = _ordines_tituli(l, titulus); o != NIHIL;
         o = o->proximus)
    {
        si (o->mortuus || !o->sedes || !_ordo_macro_est(o)
            || _via_omissa(o->via))
        {
            perge;
        }
        redde o;
    }
    redde NIHIL;
}

/* corpus #define sedis lectum (documentum apertum ante discum -
 * textus recentior). Finis EXACTUS ex extentis vistae quando macro
 * in TU petentis visibilis (corpus_finis - nulla scansio '\');
 * aliter linea sola + cauda " ...". Truncatio ad capacitatem
 * quoque " ..." signata. */
interior b32
_lineam_definitionis_legere (Legatus* l, Piscina* pn,
    LegatusDocumentum* doc_petitionis, constans LegatusOrdo* o,
    character* buffer, memoriae_index capacitas)
{
    constans i8* textus = NIHIL;
    memoriae_index mensura = ZEPHYRUM;

    {
        character via_ap[LEGATUS_VIA_MAXIMA];

        si ((memoriae_index)o->via.mensura + IV
            < magnitudo(via_ap))
        {
            chorda via_doc;
            LegatusDocumentum* doc;

            sprintf(via_ap, "./%.*s", (int)o->via.mensura,
                (constans character*)o->via.datum);
            via_doc.datum = (i8*)via_ap;
            via_doc.mensura = o->via.mensura + II;
            doc = _documentum_invenire(l, via_doc);
            si (doc != NIHIL && doc->apertum)
            {
                textus = doc->textus.datum;
                mensura = (memoriae_index)doc->textus.mensura;
            }
        }
    }
    si (textus == NIHIL)
    {
        character via_plena[LEGATUS_VIA_MAXIMA + LXIV];
        insignatus integer m_lecta = ZEPHYRUM;
        character* lectus;

        si ((memoriae_index)o->via.mensura + l->radix_mensura + IV
            >= magnitudo(via_plena))
        {
            redde FALSUM;
        }
        sprintf(via_plena, "%s/%.*s", l->radix,
            (int)o->via.mensura,
            (constans character*)o->via.datum);
        lectus = praeparator_plagulam_legere(pn, via_plena,
            &m_lecta);
        si (lectus == NIHIL)
        {
            redde FALSUM;
        }
        textus = (constans i8*)lectus;
        mensura = m_lecta;
    }
    {
        memoriae_index cursor = ZEPHYRUM;
        insignatus integer linea = I;
        memoriae_index a;
        memoriae_index b;
        memoriae_index scribenda;
        b32 continuatio = FALSUM;

        dum (linea < o->linea && cursor < mensura)
        {
            si (textus[cursor] == '\n')
            {
                linea++;
            }
            cursor++;
        }
        si (linea != o->linea)
        {
            redde FALSUM;
        }
        a = cursor;
        {
            s32 finis_corporis = _corpus_finis_macronis(
                doc_petitionis, o);

            si (finis_corporis > ZEPHYRUM
                && (memoriae_index)finis_corporis > a
                && (memoriae_index)finis_corporis <= mensura)
            {
                /* extenta EXACTA vistae - corpus totum,
                 * continuationes inclusae */
                b = (memoriae_index)finis_corporis;
            }
            alioquin
            {
                /* macro extra TU - linea sola */
                b = a;
                dum (b < mensura && textus[b] != '\n')
                {
                    b++;
                }
                dum (b > a && (textus[b - I] == '\r'
                    || textus[b - I] == ' '
                    || textus[b - I] == '\t'))
                {
                    b--;
                }
                si (b > a && textus[b - I] == '\\')
                {
                    continuatio = VERUM;
                    b--;
                    dum (b > a && (textus[b - I] == ' '
                        || textus[b - I] == '\t'))
                    {
                        b--;
                    }
                }
            }
        }
        si (b <= a)
        {
            redde FALSUM;
        }
        scribenda = b - a;
        si (scribenda > capacitas - VIII)
        {
            scribenda = capacitas - VIII;
            continuatio = VERUM;   /* truncatio signata */
        }
        memcpy(buffer, textus + a, scribenda);
        si (continuatio)
        {
            memcpy(buffer + scribenda, " ...",
                (memoriae_index)IV);
            scribenda += IV;
        }
        buffer[scribenda] = '\0';
    }
    redde VERUM;
}

/* gradus hover macronis; cum_verbo FALSUM = gradus originis solum
 * (ANTE redditionem symbolorum vocatur - invocatio scripta vincit
 * typum expansionis invisibilis), VERUM = etiam verbum crudum
 * (POST omnia - directivae). VERUM = responsum missum. */
interior b32
_hover_macro (Legatus* l, Piscina* pn, JsonValor* id,
    LegatusDocumentum* doc, memoriae_index octetum, b32 cum_verbo)
{
    chorda titulus;
    s32 a = -I;
    s32 b = ZEPHYRUM;
    LegatusOrdo* sedes;
    character linea_b[DXII * II];   /* corpora multilinearia */

    si (!_macro_ad_byte(doc, octetum, cum_verbo, &titulus, &a, &b))
    {
        redde FALSUM;
    }
    sedes = _sedes_macronis(l, titulus);
    si (sedes == NIHIL)
    {
        redde FALSUM;
    }
    si (!_lineam_definitionis_legere(l, pn, doc, sedes, linea_b,
            magnitudo(linea_b)))
    {
        sprintf(linea_b, "%.*s : macro", (int)titulus.mensura,
            (constans character*)titulus.datum);
    }
    {
        JsonValor* resultatum = json_objectum_creare(pn);
        JsonValor* contenta = json_objectum_creare(pn);

        json_objectum_ponere(contenta, "kind",
            json_chorda_creare_literis(pn, "plaintext"));
        json_objectum_ponere(contenta, "value",
            json_chorda_creare_literis(pn, linea_b));
        json_objectum_ponere(resultatum, "contents", contenta);
        si (a >= ZEPHYRUM)
        {
            json_objectum_ponere(resultatum, "range",
                _regionem_ex_extentis(l, pn, doc, a, b));
        }
        _respondere(l, tabellarius_responsum(pn, id, resultatum));
    }
    redde VERUM;
}

/* gradus definitionis macronis: omnes sedes (Location[]); VERUM =
 * responsum missum */
interior b32
_definitio_macronis (Legatus* l, Piscina* pn, JsonValor* id,
    LegatusDocumentum* doc, memoriae_index octetum)
{
    chorda titulus;
    s32 a;
    s32 b;
    JsonValor* lista;
    insignatus integer numerus = ZEPHYRUM;
    LegatusOrdo* o;

    si (!l->index_paratus
        || !_macro_ad_byte(doc, octetum, VERUM, &titulus, &a, &b))
    {
        redde FALSUM;
    }
    lista = json_tabulatum_creare(pn);
    per (o = _ordines_tituli(l, titulus); o != NIHIL;
         o = o->proximus)
    {
        si (o->mortuus || !o->sedes || !_ordo_macro_est(o)
            || _via_omissa(o->via))
        {
            perge;
        }
        {
            JsonValor* sedes_v = _sedes_ex_ordine(l, pn, o);

            si (sedes_v != NIHIL)
            {
                json_tabulatum_addere(lista, sedes_v);
                numerus++;
            }
        }
    }
    si (numerus == ZEPHYRUM)
    {
        redde FALSUM;
    }
    _respondere(l, tabellarius_responsum(pn, id, lista));
    redde VERUM;
}

/* Hover: sedes usus per descensum; nomina DECLARATA per ambulatio
 * symbolorum (parca "use-sites only" SOLUTA - lexema aderat). */
/* particulam in via quaerere (vicinitas: fontes prae amalgamate) */
interior b32
_via_particulam_habet (chorda via, constans character* particula)
{
    memoriae_index pm = strlen(particula);
    i32 j;

    si (via.datum == NIHIL || via.mensura < (i32)pm)
    {
        redde FALSUM;
    }
    per (j = ZEPHYRUM; j + (i32)pm <= via.mensura; j++)
    {
        si (memcmp(via.datum + j, particula, pm) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* vicinitas (politio post-debrief): give-up hoveris indicium dat
 * pro NIHIL nudo. Verbum sub cursore non resolutum -> simillima ex
 * nominibus indicis (similitudo; commentaria quoque - nomen in
 * commentario memoratum directionem accipit); nihil sub cursore ->
 * sedes proximae eiusdem viae per distantiam linearum (ordines
 * superpositione vivi). Cliens noster agens est: miss cum
 * directione melior nudo; editor humanus strepitum sentiret
 * (caveat acceptum, phase-log). */
interior b32
_hover_vicinitas (Legatus* l, Piscina* pn, JsonValor* id,
    LegatusDocumentum* doc, memoriae_index octetum)
{
    character valor_b[DXII];
    chorda verbum;
    insignatus integer linea_h = ZEPHYRUM;
    insignatus integer col_h = ZEPHYRUM;

    valor_b[ZEPHYRUM] = '\0';
    si (!l->index_paratus)
    {
        redde FALSUM;
    }
    _byte_ad_positio(l, doc, octetum, &linea_h, &col_h);

    /* verbum sub cursore (semantica gradus verbi crudi) */
    verbum.datum = NIHIL;
    verbum.mensura = ZEPHYRUM;
    {
        memoriae_index a = octetum;
        memoriae_index b = octetum;
        memoriae_index m = (memoriae_index)doc->textus.mensura;

        si (octetum < m && _character_identificatoris(
                (character)doc->textus.datum[octetum]))
        {
            dum (a > ZEPHYRUM && _character_identificatoris(
                    (character)doc->textus.datum[a - I]))
            {
                a--;
            }
            dum (b < m && _character_identificatoris(
                    (character)doc->textus.datum[b]))
            {
                b++;
            }
            verbum.datum = doc->textus.datum + a;
            verbum.mensura = (i32)(b - a);
        }
    }

    si (verbum.mensura > ZEPHYRUM && verbum.mensura < CXXVIII)
    {
        /* simillima ex nominibus indicis (acies plana - Xar
         * segmentatum est, similitudo aciem contiguam petit) */
        i32 numerus = xar_numerus(l->nomina_indicis);
        chorda* acies;
        SimilitudoFructus fructus[III];
        i32 n = ZEPHYRUM;

        acies = (chorda*)piscina_allocare(pn,
            (memoriae_index)(numerus > ZEPHYRUM ? numerus : I)
                * magnitudo(chorda));
        si (acies != NIHIL && numerus > ZEPHYRUM)
        {
            i32 c;

            per (c = ZEPHYRUM; c < numerus; c++)
            {
                acies[c] = *(chorda*)xar_obtinere(
                    l->nomina_indicis, c);
            }
            n = similitudo_optima(verbum, acies, numerus, fructus,
                III);
        }
        si (n > ZEPHYRUM)
        {
            int scripti = sprintf(valor_b,
                "nihil hic ('%.*s'); simillima:",
                (int)verbum.mensura,
                (constans character*)verbum.datum);
            i32 f;

            per (f = ZEPHYRUM; f < n; f++)
            {
                chorda t = acies[fructus[f].index];
                constans LegatusOrdo* sedes_prima = NIHIL;
                vacuum* valor;

                si (tabula_dispersa_invenire(l->index_titulorum,
                        t, &valor))
                {
                    constans LegatusOrdo* o =
                        (constans LegatusOrdo*)valor;

                    dum (o != NIHIL)
                    {
                        si (o->sedes && !o->mortuus)
                        {
                            b32 amalgama = _via_particulam_habet(
                                o->via, "/amalgama/");

                            si (sedes_prima == NIHIL)
                            {
                                sedes_prima = o;
                            }
                            /* fontes prae amalgamate (lector eo
                             * salit ubi editur) */
                            si (!amalgama)
                            {
                                sedes_prima = o;
                                frange;
                            }
                        }
                        o = o->proximus;
                    }
                }
                si ((memoriae_index)scripti
                    + (memoriae_index)t.mensura
                    + (sedes_prima != NIHIL
                        ? (memoriae_index)sedes_prima->via.mensura
                        : ZEPHYRUM)
                    + XXXII >= magnitudo(valor_b))
                {
                    frange;
                }
                si (sedes_prima != NIHIL)
                {
                    scripti += sprintf(valor_b + scripti,
                        " %.*s (%.*s:%u)",
                        (int)t.mensura,
                        (constans character*)t.datum,
                        (int)sedes_prima->via.mensura,
                        (constans character*)sedes_prima->via.datum,
                        sedes_prima->linea);
                }
                alioquin
                {
                    scripti += sprintf(valor_b + scripti, " %.*s",
                        (int)t.mensura,
                        (constans character*)t.datum);
                }
            }
        }
    }

    si (valor_b[ZEPHYRUM] == '\0' && doc->via.mensura > II)
    {
        /* sedes proximae eiusdem viae per distantiam linearum */
        chorda via_sine;
        i32 numerus = xar_numerus(l->omnes_ordines);
        i32 k;
        constans LegatusOrdo* optimi[III];
        insignatus integer distantiae[III];
        i32 n_opt = ZEPHYRUM;

        via_sine.datum = doc->via.datum + II;
        via_sine.mensura = doc->via.mensura - II;
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            constans LegatusOrdo* o = *(constans LegatusOrdo**)
                xar_obtinere(l->omnes_ordines, k);
            insignatus integer d;
            i32 j;
            b32 iam = FALSUM;

            si (o == NIHIL || !o->sedes || o->mortuus
                || o->via.mensura != via_sine.mensura
                || memcmp(o->via.datum, via_sine.datum,
                       (memoriae_index)via_sine.mensura)
                    != ZEPHYRUM)
            {
                perge;
            }
            d = o->linea > linea_h + I
                ? o->linea - (linea_h + I)
                : (linea_h + I) - o->linea;
            /* titulus iam retentus? (sedes plures eiusdem) */
            per (j = ZEPHYRUM; j < n_opt; j++)
            {
                si (optimi[j]->titulus.mensura == o->titulus.mensura
                    && memcmp(optimi[j]->titulus.datum,
                           o->titulus.datum,
                           (memoriae_index)o->titulus.mensura)
                        == ZEPHYRUM)
                {
                    si (d < distantiae[j])
                    {
                        optimi[j] = o;
                        distantiae[j] = d;
                    }
                    iam = VERUM;
                    frange;
                }
            }
            si (iam)
            {
                perge;
            }
            si (n_opt == III && d >= distantiae[II])
            {
                perge;   /* peior pessimo retento */
            }
            /* insertio ordinata (tres optimi) */
            per (j = n_opt < III ? n_opt : II; j > ZEPHYRUM
                && distantiae[j - I] > d; j--)
            {
                si (j < III)
                {
                    optimi[j] = optimi[j - I];
                    distantiae[j] = distantiae[j - I];
                }
            }
            si (j < III)
            {
                optimi[j] = o;
                distantiae[j] = d;
                si (n_opt < III)
                {
                    n_opt++;
                }
            }
        }
        si (n_opt > ZEPHYRUM)
        {
            int scripti = sprintf(valor_b,
                "nihil sub cursore; sedes proximae:");
            i32 f;

            per (f = ZEPHYRUM; f < n_opt; f++)
            {
                constans LegatusOrdo* o = optimi[f];

                si ((memoriae_index)scripti
                    + (memoriae_index)o->titulus.mensura
                    + (memoriae_index)o->genus.mensura + XXXII
                    >= magnitudo(valor_b))
                {
                    frange;
                }
                scripti += sprintf(valor_b + scripti,
                    " %.*s (%.*s) linea %u;",
                    (int)o->titulus.mensura,
                    (constans character*)o->titulus.datum,
                    (int)o->genus.mensura,
                    (constans character*)o->genus.datum,
                    o->linea);
            }
        }
    }

    si (valor_b[ZEPHYRUM] == '\0')
    {
        redde FALSUM;
    }
    {
        JsonValor* resultatum = json_objectum_creare(pn);
        JsonValor* contenta = json_objectum_creare(pn);

        json_objectum_ponere(contenta, "kind",
            json_chorda_creare_literis(pn, "plaintext"));
        json_objectum_ponere(contenta, "value",
            json_chorda_creare_literis(pn, valor_b));
        json_objectum_ponere(resultatum, "contents", contenta);
        _respondere(l, tabellarius_responsum(pn, id, resultatum));
        redde VERUM;
    }
}

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
    LegatusInventum inv;

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
    _invenire_ad_byte(doc, octetum, &inv);
    si (inv.symbolum == NIHIL && inv.typus == NIHIL)
    {
        /* nomen declaratum? */
        constans SemanticaSymbolum* s =
            _symbolum_declaratum_ad_byte(doc, octetum);

        si (s != NIHIL && s->lexema != NIHIL)
        {
            SilvaToken* rt = silva_token_radix(s->lexema);

            si (rt == NIHIL)
            {
                rt = s->lexema;
            }
            inv.symbolum = s;
            inv.a = rt->byte_offset;
            inv.b = rt->byte_offset + (s32)rt->longitudo;
        }
    }
    /* invocatio macronis sub positione VINCIT redditionem symboli/
     * typi (litterale 7, printf - typus expansionis NON est quod
     * scriptor scripsit); gradus originis solum, praecisus */
    si (_hover_macro(l, pn, id, doc, octetum, FALSUM))
    {
        redde;
    }
    si (inv.symbolum == NIHIL && inv.typus == NIHIL)
    {
        /* gradus macronis postremus (verbum crudum - directivae) */
        si (_hover_macro(l, pn, id, doc, octetum, VERUM))
        {
            redde;
        }
        /* vicinitas: miss cum directione pro NIHIL nudo */
        si (_hover_vicinitas(l, pn, id, doc, octetum))
        {
            redde;
        }
        _respondere(l, tabellarius_responsum(pn, id, NIHIL));
        redde;
    }
    {
        character typus_b[CCLVI];
        character valor_b[DXII];
        JsonValor* resultatum;
        JsonValor* contenta;

        si (inv.symbolum != NIHIL)
        {
            si (_signaturam_scribere(inv.symbolum, valor_b,
                    magnitudo(valor_b)) == ZEPHYRUM)
            {
                /* titulus ipse nimius (numquam re vera) */
                sprintf(valor_b, "%.*s",
                    (int)(inv.symbolum->titulus.mensura
                          < (insignatus integer)CCLVI
                        ? inv.symbolum->titulus.mensura
                        : (insignatus integer)CCLVI),
                    (constans character*)
                        inv.symbolum->titulus.datum);
            }
        }
        alioquin
        {
            si (silva_c89_typum_scribere(inv.typus, typus_b,
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
        si (inv.a >= ZEPHYRUM)
        {
            json_objectum_ponere(resultatum, "range",
                _regionem_ex_extentis(l, pn, doc, inv.a, inv.b));
        }
        _respondere(l, tabellarius_responsum(pn, id, resultatum));
    }
}

/* Definitio (gradus declarationis, v0.1a): sedes usus -> symbolum
 * -> lexema nominis (etiam in CAPITE praebito - saltus
 * trans-plagularis primus!); nomen declaratum -> se ipsum.
 * Definitio corporis .c alieni = post explorationem quintam
 * (index trans-plagularis). */
interior vacuum
_definitio_tractare (Legatus* l, Piscina* pn, JsonValor* id,
    JsonValor* params)
{
    character via_c[LEGATUS_VIA_MAXIMA];
    chorda via;
    chorda uri;
    JsonValor* doc_v;
    JsonValor* pos_v;
    LegatusDocumentum* doc;
    memoriae_index octetum;
    LegatusInventum inv;
    constans SemanticaSymbolum* s;

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
    _invenire_ad_byte(doc, octetum, &inv);
    s = inv.symbolum;
    si (s == NIHIL)
    {
        s = _symbolum_declaratum_ad_byte(doc, octetum);
    }
    si (s == NIHIL)
    {
        /* gradus macronis (v0.2) */
        si (_definitio_macronis(l, pn, id, doc, octetum))
        {
            redde;
        }
        _respondere(l, tabellarius_responsum(pn, id, NIHIL));
        redde;
    }
    /* gradus corporis (v0.1b): sedes .c ex indice ante gradum
     * declarationis; primariae ante secundarias (politica) */
    si (l->index_paratus && !_symbolum_plagulae_solius(s))
    {
        JsonValor* lista = json_tabulatum_creare(pn);
        insignatus integer corpora = ZEPHYRUM;
        LegatusOrdo* o = _ordines_tituli(l, _titulus_symboli(s));
        s32 gradus;

        per (gradus = ZEPHYRUM; gradus < (s32)II; gradus++)
        {
            LegatusOrdo* cursor = o;

            per (; cursor != NIHIL && corpora < (insignatus integer)V;
                 cursor = cursor->proximus)
            {
                si (cursor->mortuus || !cursor->sedes
                    || _ordo_macro_est(cursor)
                    || cursor->profunditas != ZEPHYRUM
                    || _via_omissa(cursor->via)
                    || !_chorda_desinit_literis(cursor->via, ".c"))
                {
                    perge;
                }
                si ((_via_secundaria(cursor->via) ? I : ZEPHYRUM)
                    != (insignatus integer)gradus)
                {
                    perge;
                }
                {
                    JsonValor* sedes_v = _sedes_ex_ordine(l, pn,
                        cursor);

                    si (sedes_v != NIHIL)
                    {
                        json_tabulatum_addere(lista, sedes_v);
                        corpora++;
                    }
                }
            }
        }
        si (corpora > ZEPHYRUM)
        {
            JsonValor* decl = _sedes_ex_symbolo(l, pn, doc, s);

            si (decl != NIHIL)
            {
                json_tabulatum_addere(lista, decl);
            }
            _respondere(l, tabellarius_responsum(pn, id, lista));
            redde;
        }
    }
    _respondere(l, tabellarius_responsum(pn, id,
        _sedes_ex_symbolo(l, pn, doc, s)));
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

        /* macra plagulae (v0.2 DECISUS: #define = pars superficiei
         * API - conspectus latina.h contenta sua monstrat) */
        si (doc->parsura->expansio != NIHIL)
        {
            insignatus integer nm = silva_macros_numerus(
                doc->parsura->expansio);
            insignatus integer im;

            per (im = ZEPHYRUM; im < nm; im++)
            {
                SilvaMacroVista vista;
                memoriae_index la;
                memoriae_index lb;
                JsonValor* symbolum_v;
                JsonValor* sedes_v;

                si (!silva_macro_vista(doc->parsura->expansio, im,
                        &vista)
                    || vista.titulus == NIHIL
                    || vista.fons_index != (int)fons
                    || vista.linea == ZEPHYRUM)
                {
                    perge;
                }
                la = _lineae_initium(doc, vista.linea - I);
                lb = _lineae_terminus(doc, vista.linea - I);
                si (lb <= la)
                {
                    perge;
                }
                symbolum_v = json_objectum_creare(pn);
                sedes_v = json_objectum_creare(pn);
                {
                    chorda titulus;

                    titulus.mensura = (i32)vista.titulus->mensura;
                    titulus.datum = (i8*)vista.titulus->datum;
                    json_objectum_ponere(symbolum_v, "name",
                        json_chorda_creare(pn, titulus));
                }
                json_objectum_ponere(symbolum_v, "kind",
                    json_integer_creare(pn, (s64)14));
                json_objectum_ponere(sedes_v, "uri",
                    json_chorda_creare(pn, doc->uri));
                json_objectum_ponere(sedes_v, "range",
                    _regionem_ex_extentis(l, pn, doc, (s32)la,
                        (s32)lb));
                json_objectum_ponere(symbolum_v, "location",
                    sedes_v);
                json_tabulatum_addere(lista, symbolum_v);
            }
        }
    }
    _respondere(l, tabellarius_responsum(pn, id, lista));
}

/* ==================================================
 * v0.1b: references / workspaceSymbol / incomingCalls
 * (+ outgoingCalls: inversum, grex per vocatum)
 * (index trans-plagularis; politica viarum ubique)
 * ================================================== */

#define LEGATUS_RELATIONES_MAXIMAE 200
#define LEGATUS_PLAGULAE_VOCANTIUM_MAXIMAE 20

/* relationes macronis in listam (sedes per cum_declarationibus,
 * usus semper); numerus additarum redditur */
interior insignatus integer
_relationes_macronis (Legatus* l, Piscina* pn, JsonValor* lista,
    chorda titulus, b32 cum_declarationibus)
{
    insignatus integer numerus = ZEPHYRUM;
    LegatusOrdo* o;

    per (o = _ordines_tituli(l, titulus);
         o != NIHIL && numerus < (insignatus integer)
                LEGATUS_RELATIONES_MAXIMAE;
         o = o->proximus)
    {
        si (o->mortuus || !_ordo_macro_est(o)
            || _via_omissa(o->via))
        {
            perge;
        }
        si (o->sedes && !cum_declarationibus)
        {
            perge;
        }
        {
            JsonValor* sedes_v = _sedes_ex_ordine(l, pn, o);

            si (sedes_v != NIHIL)
            {
                json_tabulatum_addere(lista, sedes_v);
                numerus++;
            }
        }
    }
    redde numerus;
}

/* symbolum ad positionem petitionis (usus AUT declaratum);
 * octetum_ex (NIHIL licet) = octetum positionis - gradus macronis
 * vocantis eo utitur cum symbolum deest */
interior constans SemanticaSymbolum*
_symbolum_petitionis (Legatus* l, JsonValor* params,
    LegatusDocumentum** doc_ex, memoriae_index* octetum_ex)
{
    character via_c[LEGATUS_VIA_MAXIMA];
    chorda via;
    chorda uri;
    JsonValor* doc_v;
    JsonValor* pos_v;
    LegatusDocumentum* doc;
    memoriae_index octetum;
    LegatusInventum inv;
    constans SemanticaSymbolum* s;

    *doc_ex = NIHIL;
    si (octetum_ex != NIHIL)
    {
        *octetum_ex = ZEPHYRUM;
    }
    si (params == NIHIL)
    {
        redde NIHIL;
    }
    doc_v = json_objectum_capere(params, "textDocument");
    pos_v = json_objectum_capere(params, "position");
    si (!_viam_extrahere(l, doc_v, via_c, magnitudo(via_c), &via,
            &uri)
        || pos_v == NIHIL)
    {
        redde NIHIL;
    }
    doc = _documentum_invenire(l, via);
    si (doc == NIHIL || !doc->apertum || doc->sem == NIHIL
        || doc->parsura == NIHIL
        || doc->parsura->commissio == NIHIL)
    {
        redde NIHIL;
    }
    *doc_ex = doc;
    octetum = _positio_ad_byte(l, doc,
        (insignatus integer)json_ad_integer(
            json_objectum_capere(pos_v, "line")),
        (insignatus integer)json_ad_integer(
            json_objectum_capere(pos_v, "character")));
    si (octetum_ex != NIHIL)
    {
        *octetum_ex = octetum;
    }
    _invenire_ad_byte(doc, octetum, &inv);
    s = inv.symbolum;
    si (s == NIHIL)
    {
        s = _symbolum_declaratum_ad_byte(doc, octetum);
    }
    redde s;
}

interior vacuum
_references_tractare (Legatus* l, Piscina* pn, JsonValor* id,
    JsonValor* params)
{
    LegatusDocumentum* doc = NIHIL;
    memoriae_index octetum = ZEPHYRUM;
    constans SemanticaSymbolum* s = _symbolum_petitionis(l, params,
        &doc, &octetum);
    JsonValor* lista = json_tabulatum_creare(pn);
    b32 cum_declarationibus = FALSUM;
    insignatus integer numerus = ZEPHYRUM;

    si (!l->index_paratus || (s == NIHIL && doc == NIHIL))
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    {
        JsonValor* ctx_v = json_objectum_capere(params, "context");

        si (ctx_v != NIHIL)
        {
            JsonValor* inc = json_objectum_capere(ctx_v,
                "includeDeclaration");

            si (inc != NIHIL && json_est_boolean(inc))
            {
                cum_declarationibus = json_ad_boolean(inc);
            }
        }
    }
    si (s == NIHIL)
    {
        /* gradus macronis (v0.2) */
        chorda titulus;
        s32 a;
        s32 b;

        si (_macro_ad_byte(doc, octetum, VERUM, &titulus, &a, &b))
        {
            (vacuum)_relationes_macronis(l, pn, lista, titulus,
                cum_declarationibus);
        }
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    {
        b32 plagulae_solius = _symbolum_plagulae_solius(s);
        chorda via_sine;
        LegatusOrdo* o = _ordines_tituli(l, _titulus_symboli(s));

        via_sine.datum = doc->via.datum + II;
        via_sine.mensura = doc->via.mensura - II;
        per (; o != NIHIL
             && numerus < (insignatus integer)
                    LEGATUS_RELATIONES_MAXIMAE;
             o = o->proximus)
        {
            si (o->mortuus || _ordo_macro_est(o)
                || _via_omissa(o->via))
            {
                perge;
            }
            si (o->sedes && !cum_declarationibus)
            {
                perge;
            }
            si (plagulae_solius)
            {
                si (o->via.mensura != via_sine.mensura
                    || memcmp(o->via.datum, via_sine.datum,
                           (memoriae_index)via_sine.mensura)
                        != ZEPHYRUM)
                {
                    perge;
                }
            }
            alioquin si (o->profunditas != ZEPHYRUM)
            {
                perge;   /* locales alieni eiusdem nominis */
            }
            {
                JsonValor* sedes_v = _sedes_ex_ordine(l, pn, o);

                si (sedes_v != NIHIL)
                {
                    json_tabulatum_addere(lista, sedes_v);
                    numerus++;
                }
            }
        }
        si (numerus >= (insignatus integer)
                LEGATUS_RELATIONES_MAXIMAE)
        {
            fprintf(stderr, "legatus: relationes ad tectum %d"
                " praecisae (%.*s)\n", LEGATUS_RELATIONES_MAXIMAE,
                (int)s->titulus.mensura,
                (constans character*)s->titulus.datum);
        }
    }
    _respondere(l, tabellarius_responsum(pn, id, lista));
}

interior s64
_species_lsp_ex_genere (chorda genus)
{
    si (_chorda_incipit_literis(genus, "functio"))
    {
        redde 12;
    }
    si (_chorda_incipit_literis(genus, "typedef"))
    {
        redde 5;
    }
    si (_chorda_incipit_literis(genus, "constans"))
    {
        redde 14;
    }
    si (_chorda_incipit_literis(genus, "macro"))
    {
        redde 14;   /* Constant - LSP genus macronis non habet */
    }
    redde 13;
}

interior vacuum
_workspacesymbol_tractare (Legatus* l, Piscina* pn, JsonValor* id,
    JsonValor* params)
{
    JsonValor* lista = json_tabulatum_creare(pn);
    chorda quaestio;
    JsonValor* q_v;

    si (params == NIHIL || !l->index_paratus)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    q_v = json_objectum_capere(params, "query");
    si (q_v == NIHIL || !json_est_chorda(q_v))
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    quaestio = json_ad_chorda(q_v);
    si (quaestio.mensura == ZEPHYRUM)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    {
        i32 n = xar_numerus(l->nomina_indicis);
        chorda* candidati = (chorda*)piscina_allocare(pn,
            (memoriae_index)(n > ZEPHYRUM ? n : I)
                * magnitudo(chorda));
        SimilitudoFructus fructus[XX];
        i32 inventa;
        i32 i;

        si (candidati == NIHIL)
        {
            _respondere(l, tabellarius_responsum(pn, id, lista));
            redde;
        }
        per (i = ZEPHYRUM; i < n; i++)
        {
            candidati[i] = *(chorda*)xar_obtinere(
                l->nomina_indicis, i);
        }
        inventa = similitudo_optima(quaestio, candidati, n, fructus,
            (i32)XX);
        per (i = ZEPHYRUM; i < inventa; i++)
        {
            chorda titulus = candidati[fructus[i].index];
            LegatusOrdo* o = _ordines_tituli(l, titulus);
            LegatusOrdo* optimus = NIHIL;
            s32 punctum_optimum = -I;

            per (; o != NIHIL; o = o->proximus)
            {
                s32 punctum;

                si (o->mortuus || !o->sedes
                    || o->profunditas != ZEPHYRUM
                    || _via_omissa(o->via))
                {
                    perge;
                }
                punctum = (_chorda_desinit_literis(o->via, ".c")
                              ? (s32)II : ZEPHYRUM)
                    + (_via_secundaria(o->via) ? ZEPHYRUM : (s32)I);
                si (punctum > punctum_optimum)
                {
                    punctum_optimum = punctum;
                    optimus = o;
                }
            }
            si (optimus != NIHIL)
            {
                JsonValor* symbolum_v = json_objectum_creare(pn);
                JsonValor* sedes_v = _sedes_ex_ordine(l, pn,
                    optimus);

                si (sedes_v == NIHIL)
                {
                    perge;
                }
                json_objectum_ponere(symbolum_v, "name",
                    json_chorda_creare(pn, titulus));
                json_objectum_ponere(symbolum_v, "kind",
                    json_integer_creare(pn,
                        _species_lsp_ex_genere(optimus->genus)));
                json_objectum_ponere(symbolum_v, "location",
                    sedes_v);
                json_tabulatum_addere(lista, symbolum_v);
            }
        }
    }
    _respondere(l, tabellarius_responsum(pn, id, lista));
}

/* extenta functionum plagulae (vocantes includentes - EXACTUS):
 * documentum apertum -> analysis viva; clausum -> cache aut
 * analysis ad postulatum (praeparator, ~0.2s semel per plagulam) */
interior Xar*
_extenta_ex_semantica (Legatus* l, constans SilvaParsura* parsura,
    constans SilvaSemantica* sem, Piscina* scopus)
{
    Xar* extenta = xar_creare(scopus,
        (i32)magnitudo(LegatusFunctioExtentum));
    insignatus integer n = silva_c89_symbola_numerus(sem);
    insignatus integer i;
    s32 fons = parsura->fons_princeps;

    (vacuum)l;
    si (extenta == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans SemanticaSymbolum* s =
            silva_c89_symbolum_per_indicem(sem, i);
        insignatus integer la;
        insignatus integer ca;
        insignatus integer lb;
        insignatus integer cb;

        /* CHARTAE (pars 2): omnia genera profunditatis 0 - custos
         * generis in _functio_continens variabiles arcet */
        si (s == NIHIL || s->profunditas != ZEPHYRUM
            || s->ex_systemate || s->est_implicitum
            || s->declarans == NIHIL)
        {
            perge;
        }
        silva_nodus_extensionem_lineis(s->declarans, fons, &la,
            &ca, &lb, &cb);
        si (la == ZEPHYRUM)
        {
            perge;
        }
        {
            LegatusFunctioExtentum* e = (LegatusFunctioExtentum*)
                xar_addere(extenta);
            chorda fons_t;
            character sig_b[DXII];

            si (e == NIHIL)
            {
                perge;
            }
            fons_t.mensura = (i32)s->titulus.mensura;
            fons_t.datum = (i8*)s->titulus.datum;
            e->titulus = _transcribere_nul(scopus, fons_t);
            e->linea_a = la;
            e->linea_b = lb;
            e->genus = (s32)s->genus;
            e->signatura.mensura = ZEPHYRUM;
            e->signatura.datum = NIHIL;
            si (_signaturam_scribere(s, sig_b, magnitudo(sig_b))
                > ZEPHYRUM)
            {
                e->signatura = chorda_ex_literis(sig_b, scopus);
            }
        }
    }
    redde extenta;
}

/* recentia viae (gradus III): tempus commemoratum == tempus disci?
 * introitus absens = numquam iudicata = non recens */
interior b32
_via_recens (Legatus* l, chorda via_sine)
{
    character via_plena[LEGATUS_VIA_MAXIMA + LXIV];
    vacuum* repositum = NIHIL;

    si (l->tempora_viarum == NIHIL
        || !tabula_dispersa_invenire(l->tempora_viarum, via_sine,
               &repositum)
        || repositum == NIHIL)
    {
        redde FALSUM;
    }
    si ((memoriae_index)via_sine.mensura + l->radix_mensura + IV
        >= magnitudo(via_plena))
    {
        redde VERUM;   /* via nimia - iudicium novum non iuvat */
    }
    sprintf(via_plena, "%s/%.*s", l->radix, (int)via_sine.mensura,
        (constans character*)via_sine.datum);
    /* identitas cum se ipso (comparatio contraria vigiliae: illa
     * '>' strictum [aequaevum = recens], haec '!=' [differentia
     * quaevis = stalum] - directiones erroris oppositae) */
    redde praeparator_tempus_plagulae(via_plena)
        == ((constans LegatusViaTempus*)repositum)->tempus
        ? VERUM : FALSUM;
}

/* IUDICIUM AD POSTULATUM (spec-v2 §VI): lege ex disco + analysa +
 * neca + funde superpositionem + extenta/chartae + tempus. Iudicium
 * SEMEL, consumptores tres (ordines/extenta/[diagnostica pars 3]);
 * arbores effimerae - nihil in piscinam capitum superstes (modus
 * sine documentis: decipula C11 exarmata). Titulus analysis =
 * "./via" (forma documentorum - ordines superpositionis eandem viam
 * ferunt). Redde extenta (NIHIL = illegibilis/apparatus fractus;
 * ordines veteres tunc INTACTI - sine parsu nihil necatur). */
interior Xar*
_recensere (Legatus* l, Piscina* pn, chorda via_sine)
{
    character via_plena[LEGATUS_VIA_MAXIMA + LXIV];
    character via_cum[LEGATUS_VIA_MAXIMA];
    insignatus integer mensura = ZEPHYRUM;
    character* textus;
    SilvaPiscina* effimera;
    SilvaParsura* parsura = NIHIL;
    SilvaSemantica* sem;
    Xar* extenta = NIHIL;
    longus tempus;

    si (!l->index_paratus || !l->praeparata
        || (memoriae_index)via_sine.mensura + l->radix_mensura + IV
               >= magnitudo(via_plena)
        || (memoriae_index)via_sine.mensura + IV
               >= magnitudo(via_cum))
    {
        redde NIHIL;
    }
    sprintf(via_plena, "%s/%.*s", l->radix, (int)via_sine.mensura,
        (constans character*)via_sine.datum);
    sprintf(via_cum, "./%.*s", (int)via_sine.mensura,
        (constans character*)via_sine.datum);
    tempus = praeparator_tempus_plagulae(via_plena);
    textus = praeparator_plagulam_legere(pn, via_plena, &mensura);
    si (textus == NIHIL)
    {
        redde NIHIL;
    }
    effimera = silva_piscina_generare_dynamicum("legatus_recensio",
        8388608);
    si (effimera == NIHIL)
    {
        redde NIHIL;
    }
    sem = praeparator_analysare(&l->praeparatio, effimera, via_cum,
        textus, mensura, &parsura);
    si (sem != NIHIL && parsura != NIHIL)
    {
        /* ordo verificatus (superpositio documentorum): neca ->
         * funde -> extenta */
        _ordines_plagulae_necare(l, via_sine);
        nexus_ordines_fundere(parsura, sem, pn,
            _superpositio_recipere, l);
        extenta = _extenta_ex_semantica(l, parsura, sem,
            l->piscina_indicis);
    }
    silva_piscina_destruere(effimera);
    si (extenta == NIHIL)
    {
        redde NIHIL;
    }
    {
        chorda clavis = _transcribere_nul(l->piscina_indicis,
            via_sine);
        LegatusViaTempus* vt = (LegatusViaTempus*)piscina_allocare(
            l->piscina_indicis, magnitudo(LegatusViaTempus));

        si (clavis.datum != NIHIL)
        {
            (vacuum)tabula_dispersa_delere(l->extenta_functionum,
                clavis);
            (vacuum)tabula_dispersa_inserere(l->extenta_functionum,
                clavis, extenta);
            si (vt != NIHIL && l->tempora_viarum != NIHIL)
            {
                vt->tempus = tempus;
                (vacuum)tabula_dispersa_delere(l->tempora_viarum,
                    clavis);
                (vacuum)tabula_dispersa_inserere(l->tempora_viarum,
                    clavis, vt);
            }
        }
    }
    redde extenta;
}

interior Xar*
_extenta_viae (Legatus* l, Piscina* pn, chorda via_sine)
{
    /* documentum apertum? (via cum "./") - arbores vivae, extenta
     * effimera in pn (crescebat olim piscina indicis per petitionem) */
    {
        character via_cum[LEGATUS_VIA_MAXIMA];
        chorda via_doc;
        LegatusDocumentum* doc;

        si ((memoriae_index)via_sine.mensura + IV
            >= magnitudo(via_cum))
        {
            redde NIHIL;
        }
        via_cum[ZEPHYRUM] = '.';
        via_cum[I] = '/';
        memcpy(via_cum + II, via_sine.datum,
            (memoriae_index)via_sine.mensura);
        via_doc.datum = (i8*)via_cum;
        via_doc.mensura = via_sine.mensura + II;
        doc = _documentum_invenire(l, via_doc);
        si (doc != NIHIL && doc->apertum && doc->sem != NIHIL
            && doc->parsura != NIHIL)
        {
            redde _extenta_ex_semantica(l, doc->parsura, doc->sem,
                pn);
        }
    }
    /* cache plagularum clausarum + recentia (gradus III) */
    {
        vacuum* repositum = NIHIL;

        si (tabula_dispersa_invenire(l->extenta_functionum,
                via_sine, &repositum)
            && _via_recens(l, via_sine))
        {
            redde (Xar*)repositum;
        }
    }
    /* stalum aut numquam visum -> iudicium (etiam superpositionem
     * emittit: plagula clausa numquam-in-tsv nunc respondet!) */
    redde _recensere(l, pn, via_sine);
}

interior constans LegatusFunctioExtentum*
_functio_continens (Xar* extenta, insignatus integer linea)
{
    i32 n;
    i32 i;

    si (extenta == NIHIL)
    {
        redde NIHIL;
    }
    n = xar_numerus(extenta);
    per (i = ZEPHYRUM; i < n; i++)
    {
        constans LegatusFunctioExtentum* e =
            (constans LegatusFunctioExtentum*)xar_obtinere(extenta,
                i);

        /* custos generis: chartae omnia genera ferunt (pars 2) -
         * continentia functionum solum */
        si (e != NIHIL && e->genus == (s32)SYMBOLUM_FUNCTIO
            && e->linea_a <= linea && linea <= e->linea_b)
        {
            redde e;
        }
    }
    redde NIHIL;
}

interior vacuum
_preparecallhierarchy_tractare (Legatus* l, Piscina* pn,
    JsonValor* id, JsonValor* params)
{
    LegatusDocumentum* doc = NIHIL;
    constans SemanticaSymbolum* s = _symbolum_petitionis(l, params,
        &doc, NIHIL);

    si (s == NIHIL || s->genus != (int)SYMBOLUM_FUNCTIO)
    {
        _respondere(l, tabellarius_responsum(pn, id, NIHIL));
        redde;
    }
    {
        JsonValor* sedes_v = _sedes_ex_symbolo(l, pn, doc, s);
        JsonValor* res = json_tabulatum_creare(pn);
        JsonValor* res_unum = json_objectum_creare(pn);
        chorda titulus = _titulus_symboli(s);

        si (sedes_v == NIHIL)
        {
            _respondere(l, tabellarius_responsum(pn, id, NIHIL));
            redde;
        }
        json_objectum_ponere(res_unum, "name",
            json_chorda_creare(pn, titulus));
        json_objectum_ponere(res_unum, "kind",
            json_integer_creare(pn, 12));
        json_objectum_ponere(res_unum, "uri",
            json_objectum_capere(sedes_v, "uri"));
        json_objectum_ponere(res_unum, "range",
            json_objectum_capere(sedes_v, "range"));
        json_objectum_ponere(res_unum, "selectionRange",
            json_objectum_capere(sedes_v, "range"));
        json_tabulatum_addere(res, res_unum);
        _respondere(l, tabellarius_responsum(pn, id, res));
    }
}

interior vacuum
_incomingcalls_tractare (Legatus* l, Piscina* pn, JsonValor* id,
    JsonValor* params)
{
    JsonValor* lista = json_tabulatum_creare(pn);
    JsonValor* res_v;
    chorda titulus;

    si (params == NIHIL || !l->index_paratus)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    res_v = json_objectum_capere(params, "item");
    si (res_v == NIHIL)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    titulus = json_ad_chorda(json_objectum_capere(res_v, "name"));
    si (titulus.mensura == ZEPHYRUM)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    {
        LegatusOrdo* o = _ordines_tituli(l, titulus);
        insignatus integer plagulae_analysatae = ZEPHYRUM;

        per (; o != NIHIL; o = o->proximus)
        {
            Xar* extenta;
            constans LegatusFunctioExtentum* vocans;

            si (o->mortuus || o->sedes || _ordo_macro_est(o)
                || o->profunditas != ZEPHYRUM
                || _via_omissa(o->via))
            {
                perge;
            }
            si (plagulae_analysatae
                >= (insignatus integer)
                       LEGATUS_PLAGULAE_VOCANTIUM_MAXIMAE)
            {
                fprintf(stderr, "legatus: vocantes ad tectum %d"
                    " plagularum praecisi\n",
                    LEGATUS_PLAGULAE_VOCANTIUM_MAXIMAE);
                frange;
            }
            extenta = _extenta_viae(l, pn, o->via);
            plagulae_analysatae++;
            vocans = _functio_continens(extenta, o->linea);
            si (vocans == NIHIL)
            {
                perge;   /* usus extra functionem (initiator...) */
            }
            {
                JsonValor* introitus = json_objectum_creare(pn);
                JsonValor* ab = json_objectum_creare(pn);
                JsonValor* ranges = json_tabulatum_creare(pn);
                character uri_b[LEGATUS_VIA_MAXIMA + LXIV];
                insignatus integer la0 = vocans->linea_a - I;

                sprintf(uri_b, "file://%s/%.*s", l->radix,
                    (int)o->via.mensura,
                    (constans character*)o->via.datum);
                json_objectum_ponere(ab, "name",
                    json_chorda_creare(pn, vocans->titulus));
                json_objectum_ponere(ab, "kind",
                    json_integer_creare(pn, 12));
                json_objectum_ponere(ab, "uri",
                    json_chorda_creare_literis(pn, uri_b));
                json_objectum_ponere(ab, "range", _regio_json(pn,
                    la0, ZEPHYRUM, vocans->linea_b - I, ZEPHYRUM));
                json_objectum_ponere(ab, "selectionRange",
                    _regio_json(pn, la0, ZEPHYRUM, la0, I));
                json_objectum_ponere(introitus, "from", ab);
                {
                    insignatus integer l0 = o->linea > ZEPHYRUM
                        ? o->linea - I : ZEPHYRUM;
                    insignatus integer c0 = o->columna > ZEPHYRUM
                        ? o->columna - I : ZEPHYRUM;

                    json_tabulatum_addere(ranges, _regio_json(pn,
                        l0, c0, l0,
                        c0 + (insignatus integer)titulus.mensura));
                }
                json_objectum_ponere(introitus, "fromRanges",
                    ranges);
                json_tabulatum_addere(lista, introitus);
            }
        }
    }
    _respondere(l, tabellarius_responsum(pn, id, lista));
}

/* sedes optima tituli (politica gradus corporis v0.1b): corpus .c
 * primarium -> .c secundarium -> quaevis sedes (caput/lexicon) */
interior constans LegatusOrdo*
_ordo_corporis_tituli (Legatus* l, chorda titulus)
{
    LegatusOrdo* caput = _ordines_tituli(l, titulus);
    s32 gradus;

    per (gradus = ZEPHYRUM; gradus < (s32)III; gradus++)
    {
        LegatusOrdo* o = caput;

        per (; o != NIHIL; o = o->proximus)
        {
            si (o->mortuus || !o->sedes || _ordo_macro_est(o)
                || o->profunditas != ZEPHYRUM
                || _via_omissa(o->via))
            {
                perge;
            }
            si (gradus < (s32)II
                && (!_chorda_desinit_literis(o->via, ".c")
                    || (_via_secundaria(o->via) ? I : ZEPHYRUM)
                           != (insignatus integer)gradus))
            {
                perge;
            }
            redde o;
        }
    }
    redde NIHIL;
}

/* outgoingCalls: inversum vocantium. Extentum functionis ad item
 * (range si adest ET titulus congruit, alioquin quaestio tituli),
 * deinde usus generis "functio" intra extentum = sedes vocationum.
 * GREX per vocatum (forma LSP vera: fromRanges plurales); "to" ex
 * politica corporis. Approximatio indicis: usus sine vocatione
 * (functio-punctator) includitur; vocatum sine sede omittitur. */
interior vacuum
_outgoingcalls_tractare (Legatus* l, Piscina* pn, JsonValor* id,
    JsonValor* params)
{
    JsonValor* lista = json_tabulatum_creare(pn);
    JsonValor* item_v;
    chorda titulus;
    character via_c[LEGATUS_VIA_MAXIMA];
    chorda via;
    chorda uri;
    chorda via_sine;
    Xar* extenta;
    constans LegatusFunctioExtentum* vocans_e = NIHIL;

    si (params == NIHIL || !l->index_paratus)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    item_v = json_objectum_capere(params, "item");
    si (item_v == NIHIL)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    titulus = json_ad_chorda(json_objectum_capere(item_v, "name"));
    si (titulus.mensura == ZEPHYRUM
        || !_viam_extrahere(l, item_v, via_c, magnitudo(via_c),
               &via, &uri)
        || via.mensura <= (i32)II)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    /* via "./x" -> sine praefixo (forma ordinum indicis) */
    via_sine.datum = via.datum + II;
    via_sine.mensura = via.mensura - II;

    extenta = _extenta_viae(l, pn, via_sine);
    {
        JsonValor* range_v = json_objectum_capere(item_v, "range");

        si (range_v != NIHIL)
        {
            insignatus integer linea_r = (insignatus integer)
                json_ad_integer(json_objectum_capere(
                    json_objectum_capere(range_v, "start"),
                    "line")) + I;

            vocans_e = _functio_continens(extenta, linea_r);
            si (vocans_e != NIHIL
                && !_chordae_pares(vocans_e->titulus, titulus))
            {
                vocans_e = NIHIL;   /* range aliena - titulus regit */
            }
        }
        si (vocans_e == NIHIL && extenta != NIHIL)
        {
            i32 n_e = xar_numerus(extenta);
            i32 k;

            per (k = ZEPHYRUM; k < n_e; k++)
            {
                constans LegatusFunctioExtentum* e =
                    (constans LegatusFunctioExtentum*)
                        xar_obtinere(extenta, k);

                si (e != NIHIL
                    && _chordae_pares(e->titulus, titulus))
                {
                    vocans_e = e;
                    frange;
                }
            }
        }
    }
    si (vocans_e == NIHIL)
    {
        _respondere(l, tabellarius_responsum(pn, id, lista));
        redde;
    }
    {
        chorda vocata_tituli[LEGATUS_RELATIONES_MAXIMAE];
        JsonValor* vocata_ranges[LEGATUS_RELATIONES_MAXIMAE];
        insignatus integer n_vocata = ZEPHYRUM;
        insignatus integer relationes = ZEPHYRUM;
        i32 n_ordines = xar_numerus(l->omnes_ordines);
        i32 k;

        per (k = ZEPHYRUM; k < n_ordines
             && relationes < (insignatus integer)
                    LEGATUS_RELATIONES_MAXIMAE; k++)
        {
            constans LegatusOrdo* o = *(constans LegatusOrdo**)
                xar_obtinere(l->omnes_ordines, k);
            JsonValor* ranges = NIHIL;

            si (o == NIHIL || o->mortuus || o->sedes
                || !_ordo_functio_est(o)
                || !_chordae_pares(o->via, via_sine)
                || o->linea < vocans_e->linea_a
                || o->linea > vocans_e->linea_b)
            {
                perge;
            }
            {
                insignatus integer g;

                per (g = ZEPHYRUM; g < n_vocata; g++)
                {
                    si (_chordae_pares(vocata_tituli[g],
                            o->titulus))
                    {
                        ranges = vocata_ranges[g];
                        frange;
                    }
                }
            }
            si (ranges == NIHIL)
            {
                constans LegatusOrdo* sedes_o =
                    _ordo_corporis_tituli(l, o->titulus);
                JsonValor* sedes_v;
                JsonValor* ad;
                JsonValor* introitus;

                si (sedes_o == NIHIL
                    || n_vocata >= (insignatus integer)
                           LEGATUS_RELATIONES_MAXIMAE)
                {
                    perge;
                }
                sedes_v = _sedes_ex_ordine(l, pn, sedes_o);
                si (sedes_v == NIHIL)
                {
                    perge;
                }
                introitus = json_objectum_creare(pn);
                ad = json_objectum_creare(pn);
                ranges = json_tabulatum_creare(pn);
                json_objectum_ponere(ad, "name",
                    json_chorda_creare(pn, o->titulus));
                json_objectum_ponere(ad, "kind",
                    json_integer_creare(pn, 12));
                json_objectum_ponere(ad, "uri",
                    json_objectum_capere(sedes_v, "uri"));
                json_objectum_ponere(ad, "range",
                    json_objectum_capere(sedes_v, "range"));
                json_objectum_ponere(ad, "selectionRange",
                    json_objectum_capere(sedes_v, "range"));
                json_objectum_ponere(introitus, "to", ad);
                json_objectum_ponere(introitus, "fromRanges",
                    ranges);
                json_tabulatum_addere(lista, introitus);
                vocata_tituli[n_vocata] = o->titulus;
                vocata_ranges[n_vocata] = ranges;
                n_vocata++;
            }
            {
                insignatus integer l0 = o->linea > ZEPHYRUM
                    ? o->linea - I : ZEPHYRUM;
                insignatus integer c0 = o->columna > ZEPHYRUM
                    ? o->columna - I : ZEPHYRUM;

                json_tabulatum_addere(ranges, _regio_json(pn, l0,
                    c0, l0, c0 + (insignatus integer)
                        o->titulus.mensura));
                relationes++;
            }
        }
    }
    _respondere(l, tabellarius_responsum(pn, id, lista));
}

/* ==================================================
 * praeparatio (structura + reaedificatio - lamina .h)
 * ================================================== */

/* Praeparationem (re)struere: capita in piscinam PROPRIAM leguntur
 * quae cum praeparatione moritur - reaedificationes repetitae
 * ligatae manent. VOCATOR arbores omnium documentorum ANTE
 * demoliatur (arbores in textus capitum monstrant - C11!). */
interior b32
_praeparationem_struere (Legatus* l)
{
    PraeparatorConfiguratio pc;

    si (l->praeparata)
    {
        praeparator_destruere(&l->praeparatio);
        l->praeparata = FALSUM;
    }
    si (l->piscina_capitum != NIHIL)
    {
        piscina_destruere(l->piscina_capitum);
        l->piscina_capitum = NIHIL;
    }
    si (l->radix_mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    l->piscina_capitum = piscina_generare_dynamicum("legatus_capita",
        1048576);
    si (l->piscina_capitum == NIHIL)
    {
        redde FALSUM;
    }
    memset(&pc, ZEPHYRUM, magnitudo(PraeparatorConfiguratio));
    pc.radix = l->radix;
    pc.cum_posix = l->cum_posix ? I : ZEPHYRUM;
    /* PIN (v0.2): cum_latina ZEPHYRUM MANEAT - si vertitur, latina
     * in textum lexici systematis concatenatur et provenientia
     * macrorum in "systema_c89.h" collabitur (lineae falsae; index
     * macrorum et saltus definitionis franguntur - trap inclusi
     * custode taciti, vide sessio.worklog.md) */
    pc.cum_latina = ZEPHYRUM;
    pc.sine_capitibus = ZEPHYRUM;
    l->praeparata = praeparator_praeparare(&l->praeparatio,
        l->piscina_capitum, &pc) ? VERUM : FALSUM;
    redde l->praeparata;
}

/* CUSTODIA AETATUM ad introitum petitionis (lex aetatum spec-v2
 * §V): gradus I (capita praeparationis - modo MCP; LSP in didSave
 * confidit, mos vetus) -> cascade in gradum II; alioquin gradus II
 * solus (tsv). Modo MCP tutum: sine documentis, arbores nullae in
 * piscinam capitum monstrant. */
interior vacuum
_aetates_custodire (Legatus* l, Piscina* pn)
{
    si (!l->initiatum)
    {
        redde;
    }
    si (l->modus_mcp && l->praeparata)
    {
        constans character* stalum = praeparator_caput_stalum(
            &l->praeparatio);

        si (stalum != NIHIL)
        {
            fprintf(stderr, "legati: praeparatio stala (%s) -"
                " reaedificatur\n", stalum);
            si (_praeparationem_struere(l))
            {
                _indicem_renovare(l, pn, VERUM);
            }
            redde;
        }
    }
    _indicem_renovare(l, pn, FALSUM);
}

/* didSave capitis: contextus reaedificatur ordine rigido (C11 -
 * decipula obiecti stalis): ① arbores OMNIUM documentorum moriuntur
 * (in textus capitum monstrant) ② praeparatio reaedificatur
 * ③ omnia documenta aperta re-analysantur et re-publicantur. */
interior vacuum
_didsave_tractare (Legatus* l, Piscina* pn, JsonValor* params)
{
    character via_c[LEGATUS_VIA_MAXIMA];
    chorda via;
    chorda uri;
    JsonValor* doc_v;
    memoriae_index m;

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
    m = strlen(via_c);
    si (m < II || via_c[m - II] != '.' || via_c[m - I] != 'h')
    {
        redde;   /* .c servatum: nihil reaedificandum */
    }
    fprintf(stderr,
        "legatus: caput servatum (%s) - contextus reaedificatur\n",
        via_c);
    {
        i32 n_doc = l->omnia_documenta != NIHIL
            ? xar_numerus(l->omnia_documenta) : (i32)ZEPHYRUM;
        i32 i;

        /* ① arbores primae moriuntur */
        per (i = ZEPHYRUM; i < n_doc; i++)
        {
            LegatusDocumentum** doc = (LegatusDocumentum**)
                xar_obtinere(l->omnia_documenta, i);

            si (doc != NIHIL && *doc != NIHIL
                && (*doc)->effimera != NIHIL)
            {
                silva_piscina_destruere((*doc)->effimera);
                (*doc)->effimera = NIHIL;
                (*doc)->parsura = NIHIL;
                (*doc)->sem = NIHIL;
            }
        }
        /* ② praeparatio nova */
        si (!_praeparationem_struere(l))
        {
            fprintf(stderr,
                "legatus: reaedificatio praeparationis FRACTA\n");
            redde;
        }
        /* ③ documenta aperta re-iudicata */
        per (i = ZEPHYRUM; i < n_doc; i++)
        {
            LegatusDocumentum** doc = (LegatusDocumentum**)
                xar_obtinere(l->omnia_documenta, i);

            si (doc != NIHIL && *doc != NIHIL && (*doc)->apertum)
            {
                _analysare_et_publicare(l, pn, *doc);
            }
        }
    }
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

/* ==================================================
 * vigilia sui (excubitor chunk 3)
 * ================================================== */

/* binarium_via in ordinem fabricae congruit si via ordine FINITUR
 * cum limite '/' (argv[0] absoluta, ordo radici relativus) */
interior b32
_vigilia_res_congruit (constans character* binarium_via,
    constans character* res, memoriae_index res_m)
{
    memoriae_index bin_m = strlen(binarium_via);

    si (res_m == ZEPHYRUM || bin_m < res_m)
    {
        redde FALSUM;
    }
    si (memcmp(binarium_via + (bin_m - res_m), res, res_m)
        != ZEPHYRUM)
    {
        redde FALSUM;
    }
    redde (bin_m == res_m
        || binarium_via[bin_m - res_m - I] == '/') ? VERUM : FALSUM;
}

/* clausuram fontium propriorum construere: ordines "binarium" ex
 * fabrica (fons .c) -> BFS trans graphum inclusionum -> tabula
 * viarum + percursio mtemporum. Quidquid deest = vigilia quieta
 * (numquam impedit). */
interior vacuum
_vigiliam_construere (Legatus* l,
    constans LegatusConfiguratio* cfg)
{
    constans character* binarium;
    constans character* fabrica_via;
    Piscina* effimera;
    character* textus;
    insignatus integer mensura;
    Xar* pila;                 /* chorda (viae visitandae) */
    Xar* omnes;                /* chorda (clausura tota - percursio) */
    TabulaDispersa* margines;  /* ex -> Xar<chorda>* (ad) */

    binarium = cfg != NIHIL ? cfg->binarium_via : NIHIL;
    si (binarium == NIHIL)
    {
        redde;   /* probationes sine fabrica - quieta */
    }
    l->binarium_tempus = praeparator_tempus_plagulae(binarium);
    si (l->binarium_tempus == 0L)
    {
        redde;
    }
    fabrica_via = (cfg->fabrica_via != NIHIL) ? cfg->fabrica_via
                                              : "fabrica.tsv";

    effimera = piscina_generare_dynamicum("legatus_vigilia",
        1048576);
    si (effimera == NIHIL)
    {
        l->binarium_tempus = 0L;
        redde;
    }
    pila = xar_creare(effimera, magnitudo(chorda));
    omnes = xar_creare(effimera, magnitudo(chorda));
    margines = tabula_dispersa_creare_chorda(effimera, CCLVI);
    l->clausura_sua = tabula_dispersa_creare_chorda(l->perennis,
        CCLVI);
    si (pila == NIHIL || omnes == NIHIL || margines == NIHIL
        || l->clausura_sua == NIHIL)
    {
        l->clausura_sua = NIHIL;
        l->binarium_tempus = 0L;
        piscina_destruere(effimera);
        redde;
    }

    /* fabrica: radices (ordines binarium quorum res = binarium) */
    textus = praeparator_plagulam_legere(effimera, fabrica_via,
        &mensura);
    si (textus != NIHIL)
    {
        character* p = textus;

        dum (*p != '\0')
        {
            character* linea = p;
            character* t1 = NIHIL;
            character* t2 = NIHIL;
            character* finis;

            dum (*p != '\0' && *p != '\n')
            {
                si (*p == '\t')
                {
                    si (t1 == NIHIL) { t1 = p; }
                    alioquin si (t2 == NIHIL) { t2 = p; }
                }
                p++;
            }
            finis = p;
            si (*p == '\n')
            {
                p++;
            }
            si (linea[ZEPHYRUM] == '#' || t1 == NIHIL || t2 == NIHIL)
            {
                perge;
            }
            si ((memoriae_index)(t1 - linea) == VIII
                && memcmp(linea, "binarium", VIII) == ZEPHYRUM
                && _vigilia_res_congruit(binarium, t1 + I,
                       (memoriae_index)(t2 - t1 - I))
                && (memoriae_index)(finis - t2) > III
                && memcmp(finis - II, ".c", II) == ZEPHYRUM)
            {
                chorda* radix_nova = (chorda*)xar_addere(pila);
                unio { constans character* c; i8* m; } u;

                u.c = t2 + I;
                radix_nova->datum = u.m;
                radix_nova->mensura = (i32)(finis - t2 - I);
            }
        }
    }
    si (xar_numerus(pila) == ZEPHYRUM)
    {
        l->clausura_sua = NIHIL;
        l->binarium_tempus = 0L;
        piscina_destruere(effimera);
        redde;
    }

    /* graphus inclusionum: margines ex -> ad */
    textus = praeparator_plagulam_legere(effimera,
        "build/inclusiones.tsv", &mensura);
    si (textus != NIHIL)
    {
        character* p = textus;

        dum (*p != '\0')
        {
            character* linea = p;
            character* t1 = NIHIL;
            character* t2 = NIHIL;

            dum (*p != '\0' && *p != '\n')
            {
                si (*p == '\t')
                {
                    si (t1 == NIHIL) { t1 = p; }
                    alioquin si (t2 == NIHIL) { t2 = p; }
                }
                p++;
            }
            si (*p == '\n')
            {
                p++;
            }
            si (linea[ZEPHYRUM] != '#' && t1 != NIHIL && t2 != NIHIL)
            {
                chorda ex;
                chorda ad;
                vacuum* valor;
                Xar* lista;
                unio { constans character* c; i8* m; } u;

                u.c = linea;
                ex.datum = u.m;
                ex.mensura = (i32)(t1 - linea);
                u.c = t1 + I;
                ad.datum = u.m;
                ad.mensura = (i32)(t2 - t1 - I);
                si (tabula_dispersa_invenire(margines, ex, &valor))
                {
                    lista = (Xar*)valor;
                }
                alioquin
                {
                    lista = xar_creare(effimera,
                        magnitudo(chorda));
                    si (lista == NIHIL)
                    {
                        perge;
                    }
                    (vacuum)tabula_dispersa_inserere(margines, ex,
                        lista);
                }
                {
                    chorda* locus = (chorda*)xar_addere(lista);

                    si (locus != NIHIL)
                    {
                        *locus = ad;
                    }
                }
            }
        }
    }

    /* BFS: pila -> clausura_sua (claves in perenni) + omnes */
    dum (xar_numerus(pila) > ZEPHYRUM)
    {
        chorda nodus = *(chorda*)xar_obtinere(pila,
            xar_numerus(pila) - I);
        vacuum* valor;

        xar_truncare(pila, xar_numerus(pila) - I);
        si (tabula_dispersa_continet(l->clausura_sua, nodus))
        {
            perge;
        }
        {
            chorda clavis = chorda_transcribere(nodus, l->perennis);
            chorda* locus;

            (vacuum)tabula_dispersa_inserere(l->clausura_sua,
                clavis, NIHIL);
            locus = (chorda*)xar_addere(omnes);
            si (locus != NIHIL)
            {
                *locus = clavis;
            }
        }
        si (tabula_dispersa_invenire(margines, nodus, &valor)
            && valor != NIHIL)
        {
            Xar* lista = (Xar*)valor;
            insignatus integer k;

            per (k = ZEPHYRUM; k < xar_numerus(lista); k++)
            {
                chorda* filius = (chorda*)xar_obtinere(lista, k);
                chorda* locus = (chorda*)xar_addere(pila);

                si (locus != NIHIL)
                {
                    *locus = *filius;
                }
            }
        }
    }

    /* percursio mtemporum: fons recentior binario -> stalus */
    {
        insignatus integer k;
        character via_b[LEGATUS_VIA_MAXIMA];

        per (k = ZEPHYRUM; k < xar_numerus(omnes); k++)
        {
            chorda* via = (chorda*)xar_obtinere(omnes, k);
            longus t;

            si ((memoriae_index)via->mensura >= magnitudo(via_b))
            {
                perge;
            }
            sprintf(via_b, "%.*s", (int)via->mensura,
                (constans character*)via->datum);
            t = praeparator_tempus_plagulae(via_b);
            si (t > l->binarium_tempus)
            {
                l->se_stalus = VERUM;
                sprintf(l->stalus_causa, "%.*s",
                    (int)via->mensura,
                    (constans character*)via->datum);
                frange;
            }
        }
    }

    piscina_destruere(effimera);
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
    (vacuum)_praeparationem_struere(l);
    si (!l->praeparata)
    {
        _respondere(l, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_INTERNUS,
            "apparatus praeparari non potuit"));
        redde;
    }
    _exclusiones_onerare(l);
    _indicem_onerare(l);
    _vigiliam_construere(l, cfg);
    si (l->se_stalus)
    {
        fprintf(stderr, "legatus: VIGILIA: %s recentior binario "
            "residente - /reload-plugins renovat\n",
            l->stalus_causa);
    }
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
        json_objectum_ponere(caps, "definitionProvider",
            json_boolean_creare(pn, VERUM));
        json_objectum_ponere(caps, "implementationProvider",
            json_boolean_creare(pn, VERUM));
        json_objectum_ponere(caps, "referencesProvider",
            json_boolean_creare(pn, VERUM));
        json_objectum_ponere(caps, "workspaceSymbolProvider",
            json_boolean_creare(pn, VERUM));
        json_objectum_ponere(caps, "callHierarchyProvider",
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
 * LEGATI - modus MCP (pars 1: framing + handshake)
 *
 * Quaestiones nomine-basatae, trahentes, disco-verae - complementum
 * LSP positionalis impellentis. Idem mundus calidus (radix,
 * praeparatio, index, vigilia); framing lineis-delimitatum; sine
 * documentis (arbores effimerae - decipula C11 exarmata, spec-v2
 * §V). Instrumenta implentur parte 3; hic sceleta.
 * ================================================== */

#define LEGATI_PROTOCOLLUM_PINNATUM "2025-06-18"

interior constans character* constans LEGATI_DOCTRINA =
    "LEGATI: quaestiones nomine-basatae de codice C89 huius arboris"
    " (complementum LSP positionalis). diagnostica {via} = iudicium"
    " plagulae ex disco (verdictum ACCIPE/REICE + diagnostica posita"
    " omnium graduum). symbolum {titulus} = charta symboli: sedes"
    " definitionis, genus, signatura, usus per plagulas; simillima"
    " si titulus ignotus. vocantes {titulus} = quae functiones eum"
    " vocant. vocata {titulus} = quas functiones ea vocat. Responsa"
    " statum disci recentem legunt (revalidatio pigra); lineae"
    " CAUTIO staleness aut sumptus aperiunt.";

/* instrumentum unius argumenti chordae -> {name, description,
 * inputSchema} */
interior JsonValor*
_mcp_instrumentum (Piscina* pn, constans character* titulus,
    constans character* descriptio, constans character* arg_titulus,
    constans character* arg_descriptio)
{
    JsonValor* instrumentum = json_objectum_creare(pn);
    JsonValor* schema = json_objectum_creare(pn);
    JsonValor* proprietates = json_objectum_creare(pn);
    JsonValor* arg = json_objectum_creare(pn);
    JsonValor* necessaria = json_tabulatum_creare(pn);

    json_objectum_ponere(arg, "type",
        json_chorda_creare_literis(pn, "string"));
    json_objectum_ponere(arg, "description",
        json_chorda_creare_literis(pn, arg_descriptio));
    json_objectum_ponere(proprietates, arg_titulus, arg);
    json_tabulatum_addere(necessaria,
        json_chorda_creare_literis(pn, arg_titulus));
    json_objectum_ponere(schema, "type",
        json_chorda_creare_literis(pn, "object"));
    json_objectum_ponere(schema, "properties", proprietates);
    json_objectum_ponere(schema, "required", necessaria);
    json_objectum_ponere(instrumentum, "name",
        json_chorda_creare_literis(pn, titulus));
    json_objectum_ponere(instrumentum, "description",
        json_chorda_creare_literis(pn, descriptio));
    json_objectum_ponere(instrumentum, "inputSchema", schema);
    redde instrumentum;
}

interior vacuum
_mcp_initialize_tractare (Legatus* l, Piscina* pn, JsonValor* id,
    JsonValor* params, constans LegatusConfiguratio* cfg)
{
    si (l->initiatum)
    {
        _respondere(l, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_PETITIO_INVALIDA, "iam initiatum"));
        redde;
    }
    /* opus LSP minus codificatione (effectus = textus, numeri
     * 1-basati - remappa non opus, spec-v2 §III) */
    _radicem_statuere(l, params, cfg);
    si (l->radix_mensura == ZEPHYRUM)
    {
        _respondere(l, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_INTERNUS,
            "radix ignota (-radix deest)"));
        redde;
    }
    si (!_praeparationem_struere(l))
    {
        _respondere(l, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_INTERNUS,
            "apparatus praeparari non potuit"));
        redde;
    }
    _exclusiones_onerare(l);
    _indicem_onerare(l);
    _vigiliam_construere(l, cfg);
    si (l->se_stalus)
    {
        fprintf(stderr, "legati: VIGILIA - %s recentior binario\n",
            l->stalus_causa);
    }
    l->initiatum = VERUM;

    {
        JsonValor* resultatum = json_objectum_creare(pn);
        JsonValor* caps = json_objectum_creare(pn);
        JsonValor* instrumenta_caps = json_objectum_creare(pn);
        JsonValor* servus = json_objectum_creare(pn);
        JsonValor* versio_v = params != NIHIL
            ? json_objectum_capere(params, "protocolVersion")
            : NIHIL;
        chorda versio = json_ad_chorda(versio_v);

        /* protocolVersion resonatum si datum (subcopia tools
         * stabilis trans versiones), alioquin pinna nostra */
        json_objectum_ponere(resultatum, "protocolVersion",
            versio.mensura > ZEPHYRUM
                ? json_chorda_creare(pn, versio)
                : json_chorda_creare_literis(pn,
                      LEGATI_PROTOCOLLUM_PINNATUM));
        json_objectum_ponere(caps, "tools", instrumenta_caps);
        json_objectum_ponere(resultatum, "capabilities", caps);
        json_objectum_ponere(servus, "name",
            json_chorda_creare_literis(pn, "legati"));
        json_objectum_ponere(servus, "version",
            json_chorda_creare_literis(pn, "0"));
        json_objectum_ponere(resultatum, "serverInfo", servus);
        json_objectum_ponere(resultatum, "instructions",
            json_chorda_creare_literis(pn, LEGATI_DOCTRINA));
        _respondere(l, tabellarius_responsum(pn, id, resultatum));
    }
}

interior vacuum
_mcp_toolslist_tractare (Legatus* l, Piscina* pn, JsonValor* id)
{
    JsonValor* resultatum = json_objectum_creare(pn);
    JsonValor* instrumenta = json_tabulatum_creare(pn);

    json_tabulatum_addere(instrumenta, _mcp_instrumentum(pn,
        "diagnostica",
        "Iudicium C89 plagulae ex disco (examen calidum): verdictum"
        " ACCIPE/REICE + diagnostica posita omnium graduum"
        " (violatio/suspectum/domesticum/infra).",
        "via",
        "via plagulae .c/.h, relativa radici aut absoluta"));
    json_tabulatum_addere(instrumenta, _mcp_instrumentum(pn,
        "symbolum",
        "Charta symboli nomine: sedes definitionis (corpus .c"
        " primum), genus, signatura, usus per plagulas; macris"
        " corpus definitionis; simillima si titulus ignotus.",
        "titulus",
        "nomen symboli (functio/variabile/typus/macro)"));
    json_tabulatum_addere(instrumenta, _mcp_instrumentum(pn,
        "vocantes",
        "Quae functiones titulum vocant (hierarchia vocationum"
        " intrans, nomine - sine documento aperto).",
        "titulus",
        "nomen functionis"));
    json_tabulatum_addere(instrumenta, _mcp_instrumentum(pn,
        "vocata",
        "Quas functiones functio nominata vocat (hierarchia"
        " vocationum exiens, nomine).",
        "titulus",
        "nomen functionis"));
    json_objectum_ponere(resultatum, "tools", instrumenta);
    _respondere(l, tabellarius_responsum(pn, id, resultatum));
}

/* effectus textus instrumenti (+ CAUTIO vigiliae si stalus) */
interior vacuum
_mcp_textum_respondere (Legatus* l, Piscina* pn, JsonValor* id,
    constans character* textus, b32 error_est)
{
    JsonValor* resultatum = json_objectum_creare(pn);
    JsonValor* contentus = json_tabulatum_creare(pn);
    JsonValor* fragmentum = json_objectum_creare(pn);
    constans character* corpus = textus;

    si (l->se_stalus)
    {
        character* cum_cautione = (character*)piscina_allocare(pn,
            strlen(textus) + LEGATUS_VIA_MAXIMA + CXXVIII);

        si (cum_cautione != NIHIL)
        {
            sprintf(cum_cautione,
                "%s\nCAUTIO: LEGATI IPSE STALUS (%s recentior"
                " binario) - /mcp reconnect renovat", textus,
                l->stalus_causa);
            corpus = cum_cautione;
        }
    }
    json_objectum_ponere(fragmentum, "type",
        json_chorda_creare_literis(pn, "text"));
    json_objectum_ponere(fragmentum, "text",
        json_chorda_creare_literis(pn, corpus));
    json_tabulatum_addere(contentus, fragmentum);
    json_objectum_ponere(resultatum, "content", contentus);
    si (error_est)
    {
        json_objectum_ponere(resultatum, "isError",
            json_boolean_creare(pn, VERUM));
    }
    _respondere(l, tabellarius_responsum(pn, id, resultatum));
}

interior vacuum
_mcp_toolscall_tractare (Legatus* l, Piscina* pn, JsonValor* id,
    JsonValor* params)
{
    chorda titulus;

    si (params == NIHIL)
    {
        _respondere(l, tabellarius_errorem(pn, id,
            TABELLARIUS_ERROR_PARAMETRA_INVALIDA,
            "params desunt"));
        redde;
    }
    titulus = json_ad_chorda(json_objectum_capere(params, "name"));
    si (_methodus_est(titulus, "diagnostica")
        || _methodus_est(titulus, "symbolum")
        || _methodus_est(titulus, "vocantes")
        || _methodus_est(titulus, "vocata"))
    {
        /* pars 3 implet; sceletum honestum */
        _mcp_textum_respondere(l, pn, id,
            "nondum implementum (pars 3)", VERUM);
        redde;
    }
    _respondere(l, tabellarius_errorem(pn, id,
        TABELLARIUS_ERROR_PARAMETRA_INVALIDA,
        "instrumentum ignotum"));
}

interior LegatusCursus
_mcp_tractare (Legatus* l, Piscina* pn, TabellariusNuntius* n,
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
        _aetates_custodire(l, pn);   /* gradus I+II ante responsum */
        si (_methodus_est(n->methodus, "initialize"))
        {
            _mcp_initialize_tractare(l, pn, n->id, n->params, cfg);
        }
        alioquin si (_methodus_est(n->methodus, "ping"))
        {
            /* ante initialize quoque licitum (MCP spec) */
            _respondere(l, tabellarius_responsum(pn, n->id,
                json_objectum_creare(pn)));
        }
        alioquin si (!l->initiatum)
        {
            _respondere(l, tabellarius_errorem(pn, n->id,
                TABELLARIUS_ERROR_NONDUM_INITIATUM,
                "nondum initiatum"));
        }
        alioquin si (_methodus_est(n->methodus, "tools/list"))
        {
            _mcp_toolslist_tractare(l, pn, n->id);
        }
        alioquin si (_methodus_est(n->methodus, "tools/call"))
        {
            _mcp_toolscall_tractare(l, pn, n->id, n->params);
        }
        alioquin
        {
            _respondere(l, tabellarius_errorem(pn, n->id,
                TABELLARIUS_ERROR_METHODUS_IGNOTA,
                "methodus ignota"));
        }
        redde LEGATUS_PERGE;
    }
    /* nuntiationes (notifications/initialized, cancelled...) -
     * tacite omissae; exitus = cliens fistulam claudit (EOF) */
    redde LEGATUS_PERGE;
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
        _aetates_custodire(l, pn);   /* gradus II ante responsum */
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
                         "textDocument/definition")
            || _methodus_est(n->methodus,
                   "textDocument/implementation"))
        {
            /* implementation in C89 = definitio corporis (alias -
             * exemplar clangd; nona operationis clientis) */
            _definitio_tractare(l, pn, n->id, n->params);
        }
        alioquin si (_methodus_est(n->methodus,
                         "textDocument/documentSymbol"))
        {
            _documentsymbol_tractare(l, pn, n->id, n->params);
        }
        alioquin si (_methodus_est(n->methodus,
                         "textDocument/references"))
        {
            _references_tractare(l, pn, n->id, n->params);
        }
        alioquin si (_methodus_est(n->methodus, "workspace/symbol"))
        {
            _workspacesymbol_tractare(l, pn, n->id, n->params);
        }
        alioquin si (_methodus_est(n->methodus,
                         "textDocument/prepareCallHierarchy"))
        {
            _preparecallhierarchy_tractare(l, pn, n->id, n->params);
        }
        alioquin si (_methodus_est(n->methodus,
                         "callHierarchy/incomingCalls"))
        {
            _incomingcalls_tractare(l, pn, n->id, n->params);
        }
        alioquin si (_methodus_est(n->methodus,
                         "callHierarchy/outgoingCalls"))
        {
            _outgoingcalls_tractare(l, pn, n->id, n->params);
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
    alioquin si (_methodus_est(n->methodus, "textDocument/didSave"))
    {
        _didsave_tractare(l, pn, n->params);
    }
    /* ceterae nuntiationes tacitae */
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
        l.modus_mcp = cfg->modus_mcp;
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
        corpus = l.modus_mcp
            ? tabellarius_lineam_legere(intra, pn, &finitus)
            : tabellarius_epistulam_legere(intra, pn, &finitus);
        si (finitus)
        {
            piscina_destruere(pn);
            /* MCP: cliens fistulam claudit = exitus ORDINATUS 0
             * (protocollum "exit" caret); LSP: EOF = abnormis 1 */
            si (l.modus_mcp)
            {
                exitus = ZEPHYRUM;
            }
            frange;
        }
        n = tabellarius_nuntium_legere(corpus, pn);
        cursus = l.modus_mcp
            ? _mcp_tractare(&l, pn, &n, cfg)
            : _nuntium_tractare(&l, pn, &n, cfg);
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
    si (l.piscina_capitum != NIHIL)
    {
        piscina_destruere(l.piscina_capitum);
    }
    si (l.piscina_indicis != NIHIL)
    {
        piscina_destruere(l.piscina_indicis);
    }
    piscina_destruere(l.perennis);
    redde exitus;
}
