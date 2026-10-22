/* fiducia_diffisa_generare.c - tabulam diffisarum GENERARE
 * (fiducia-linux-plan T2)
 *
 * Usus: fiducia_diffisa_generare <directorium> <manu.stml> <exitus.c>
 *
 * <directorium> = instantanea OneCRL fixa (nomen = dies YYYY-MM-DD):
 * changeset.json (Mozilla Remote Settings, security-state/onecrl) et
 * SHA256SUMS - quisque fasciculus ibi nominatus PROBATUR ante
 * lectionem. Rationes enabled:false praetereuntur; issuerName +
 * serialNumber -> EMITTENS_SERIES, subject + pubKeyHash ->
 * SUBIECTUM_CLAVIS (base64; pubKeyHash XXXII octeti); aliud = vitium.
 * <manu.stml> = diffisae quas OneCRL non habet:
 *   <diffisa genus="emittens_series|subiectum_clavis" primum="base64"
 *            secundum="base64" causa="..." dies="YYYY-MM-DD" fons="URL"/>
 * omnia attributa necessaria.
 *
 * Exitus: lib/fiducia_diffisa_tabula.c ordine (genus, primum, secundum),
 * duplicatae semel, octeti in UNA tabula nominata (sectiones aequales
 * communes). Vitium quodvis -> exitus II cum causa nominata, nihil
 * scriptum. tools/fiducia_diffisa_generare.sh id struit et currit. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "chorda_aedificator.h"
#include "internamentum.h"
#include "xar.h"
#include "json.h"
#include "stml.h"
#include "base64.h"
#include "sigillum.h"
#include "filum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DIFFISAE_MAXIMAE  (IV * M)
#define CAUSA_MAXIMA      CCLVI

nomen structura {
          s32  genus;          /* I emittens_series, II subiectum_clavis */
           i8* primum;
          i32  primum_mensura;
           i8* secundum;
          i32  secundum_mensura;
    character  causa[CAUSA_MAXIMA];
          i32  primi_locus;    /* in tabula octetorum */
          i32  secundi_locus;
} Diffisa;

hic_manens Diffisa diffisae[DIFFISAE_MAXIMAE];
hic_manens s32     numerus_diffisarum = ZEPHYRUM;
hic_manens s32     praeteritae = ZEPHYRUM;      /* enabled:false */
hic_manens s32     duplicatae = ZEPHYRUM;
hic_manens s32     manu = ZEPHYRUM;

interior vacuum
_mori (
    constans character* nuntius,
    constans character* res)
{
    fprintf(stderr, "fiducia_diffisa_generare: %s%s%s\n", nuntius,
        res ? ": " : "", res ? res : "");
    exit(II);
}

/* SHA256SUMS: "hex  nomen" per lineam; quisque fasciculus probatur */
interior vacuum
_summas_probare (
    constans character* directorium,
               Piscina* p)
{
       character via[MXXIV];
          chorda summae;
             i32 i        = ZEPHYRUM;
             i32 probata  = ZEPHYRUM;

    sprintf(via, "%s/SHA256SUMS", directorium);
    summae = filum_legere_totum(via, p);
    si (summae.mensura == ZEPHYRUM)
    {
        _mori("SHA256SUMS deest", via);
    }
    dum (i < summae.mensura)
    {
              i32 a = i;
              i32 b;
        character hex[SIGILLUM_HEX_MENSURA];
        character nomen_fasciculi[CCLVI];
        character computatum[SIGILLUM_HEX_MENSURA];
           chorda contentum;
         Sigillum s;

        dum (i < summae.mensura && summae.datum[i] != '\n')
        {
            i++;
        }
        b = i;
        i++;
        si (b == a)
        {
            perge;
        }
        si (   b - a < LXVI + I || summae.datum[a + LXIV] != ' '
            || summae.datum[a + LXV] != ' '
            || b - a - LXVI          >= CCLVI)
        {
            _mori("SHA256SUMS: linea mala", via);
        }
        memcpy(hex, summae.datum + a, LXIV);
        hex[LXIV] = '\0';
        memcpy(nomen_fasciculi, summae.datum + a + LXVI,
            (size_t)(b - a - LXVI));
        nomen_fasciculi[b - a - LXVI] = '\0';
        sprintf(via, "%s/%s", directorium, nomen_fasciculi);
        contentum = filum_legere_totum(via, p);
        si (contentum.mensura == ZEPHYRUM)
        {
            _mori("fasciculus SHA256SUMS legi non potest", via);
        }
        s = sigillum_computare(contentum.datum,
            (memoriae_index)contentum.mensura);
        sigillum_hex(&s, computatum);
        si (strcmp(hex, computatum) != ZEPHYRUM)
        {
            _mori("SHA256SUMS non congruit", via);
        }
        probata++;
    }
    si (probata == ZEPHYRUM)
    {
        _mori("SHA256SUMS vacuum", directorium);
    }
}

interior i8*
_base64 (
                chorda  c,
                   i32* mensura,
    constans character* quid,
               Piscina* p)
{
    Base64Fructus f;

    si (c.mensura == ZEPHYRUM)
    {
        _mori("base64 vacuum", quid);
    }
    f = base64_decodificare(c, p);
    si (f.datum == NIHIL || f.mensura == ZEPHYRUM)
    {
        _mori("base64 malum", quid);
    }
    *mensura = (i32)f.mensura;
    redde f.datum;
}

interior Diffisa*
_nova (vacuum)
{
    si (numerus_diffisarum >= DIFFISAE_MAXIMAE)
    {
        _mori("diffisae nimis multae", NIHIL);
    }
    memset(&diffisae[numerus_diffisarum], ZEPHYRUM, magnitudo(Diffisa));
    redde &diffisae[numerus_diffisarum++];
}

/* "bug 2083382 (OneCRL)": numerus ultimus in details.bug */
interior vacuum
_causa_externa (
        JsonValor* m,
          Diffisa* d)
{
    chorda vitium = json_ad_chorda(json_objectum_capere(
        json_objectum_capere(m, "details"), "bug"));
    chorda id  = json_ad_chorda(json_objectum_capere(m, "id"));
       i32 a   = ZEPHYRUM;
       i32 b   = ZEPHYRUM;
       i32 i;

    per (i = ZEPHYRUM; i < vitium.mensura; i++)
    {
        si (vitium.datum[i] >= '0' && vitium.datum[i] <= '9')
        {
            si (   i == ZEPHYRUM || vitium.datum[i - I] < '0'
                || vitium.datum[i - I] > '9')
            {
                a = i;
            }
            b = i + I;
        }
    }
    si (b > a && b - a < XXXII)
    {
        sprintf(d->causa, "bug %.*s (OneCRL)", (integer)(b - a),
            (constans character*)vitium.datum + a);
    }
    alioquin
    {
        sprintf(d->causa, "OneCRL %.*s", (integer)(id.mensura < LXIV
            ? id.mensura : LXIV), (constans character*)id.datum);
    }
}

interior vacuum
_legere_mutationes (
    constans character* directorium,
               Piscina* p)
{
       character  via[MXXIV];
          chorda  textus;
    JsonResultus  r;
       JsonValor* mutationes;
             i32  k;

    sprintf(via, "%s/changeset.json", directorium);
    textus = filum_legere_totum(via, p);
    si (textus.mensura == ZEPHYRUM)
    {
        _mori("legi non potest", via);
    }
    r = json_legere(textus, p);
    si (!r.successus || r.radix == NIHIL)
    {
        _mori("JSON malum", via);
    }
    mutationes = json_objectum_capere(r.radix, "changes");
    si (!json_est_tabulatum(mutationes))
    {
        _mori("'changes' deest", via);
    }
    per (k = ZEPHYRUM; k < json_tabulatum_numerus(mutationes); k++)
    {
         JsonValor* m = json_tabulatum_obtinere(mutationes, k);
         JsonValor* e = json_objectum_capere(m, "enabled");
            chorda  emittens = json_ad_chorda(
                json_objectum_capere(m, "issuerName"));
            chorda series = json_ad_chorda(
                json_objectum_capere(m, "serialNumber"));
            chorda subiectum = json_ad_chorda(
                json_objectum_capere(m, "subject"));
            chorda clavis = json_ad_chorda(
                json_objectum_capere(m, "pubKeyHash"));
          Diffisa* d;

        si (e != NIHIL && !json_ad_boolean(e))
        {
            praeteritae++;
            perge;
        }
        si (emittens.mensura > ZEPHYRUM && series.mensura > ZEPHYRUM)
        {
            d         = _nova();
            d->genus  = I;
            d->primum = _base64(emittens, &d->primum_mensura,
                "issuerName", p);
            d->secundum = _base64(series, &d->secundum_mensura,
                "serialNumber", p);
        }
        alioquin si (   subiectum.mensura > ZEPHYRUM
                     && clavis.mensura > ZEPHYRUM)
        {
            d         = _nova();
            d->genus  = II;
            d->primum = _base64(subiectum, &d->primum_mensura,
                "subject",
                p);
            d->secundum = _base64(clavis, &d->secundum_mensura,
                "pubKeyHash", p);
            si (d->secundum_mensura != SIGILLUM_OCTETI)
            {
                _mori("pubKeyHash non XXXII octeti", NIHIL);
            }
        }
        alioquin
        {
            _mori("ratio OneCRL generis ignoti",
                chorda_ut_cstr(json_ad_chorda(json_objectum_capere(m,
                    "id")), p));
            redde;
        }
        _causa_externa(m, d);
    }
}

interior chorda
_attributum (
              StmlNodus* m,
     constans character* titulus)
{
    chorda* v = stml_attributum_capere(m, titulus);

    si (v == NIHIL || v->mensura == ZEPHYRUM)
    {
        _mori("attributum deest in indice manu", titulus);
    }
    redde *v;
}

interior vacuum
_legere_manu (
    constans character* via,
               Piscina* p)
{
          chorda  textus = filum_legere_totum(via, p);
    StmlResultus  r;
             Xar* lista;
             s32  i;

    si (textus.datum == NIHIL || textus.mensura == ZEPHYRUM)
    {
        _mori("legi non potest", via);
    }
    r = stml_legere(textus, p, internamentum_creare(p));
    si (!r.successus || r.elementum_radix == NIHIL)
    {
        _mori("STML malum", via);
    }
    lista = stml_invenire_omnes_liberos(r.elementum_radix, "diffisa",
        p);
    per (i = ZEPHYRUM; lista && i < (s32)xar_numerus(lista); i++)
    {
         StmlNodus* m      = *(StmlNodus**)xar_obtinere(lista, (i32)i);
            chorda  genus  = _attributum(m, "genus");
            chorda  causa  = _attributum(m, "causa");
            chorda  dies   = _attributum(m, "dies");
            chorda  fons   = _attributum(m, "fons");
           Diffisa* d      = _nova();

        si (chorda_aequalis_literis(genus, "emittens_series"))
        {
            d->genus = I;
        }
        alioquin si (chorda_aequalis_literis(genus, "subiectum_clavis"))
        {
            d->genus = II;
        }
        alioquin
        {
            _mori("genus ignotum in indice manu",
                chorda_ut_cstr(genus, p));
        }
        d->primum = _base64(_attributum(m, "primum"),
            &d->primum_mensura,
            "primum", p);
        d->secundum = _base64(_attributum(m, "secundum"),
            &d->secundum_mensura, "secundum", p);
        si (d->genus == II && d->secundum_mensura != SIGILLUM_OCTETI)
        {
            _mori("subiectum_clavis: secundum non XXXII octeti", NIHIL);
        }
        si (dies.mensura != X)
        {
            _mori("dies non YYYY-MM-DD", chorda_ut_cstr(dies, p));
        }
        si (causa.mensura + fons.mensura + XXXII >= CAUSA_MAXIMA)
        {
            _mori("causa aut fons nimis longa", NIHIL);
        }
        sprintf(d->causa, "%.*s (%.*s, %.*s)", (integer)causa.mensura,
            (constans character*)causa.datum, (integer)dies.mensura,
            (constans character*)dies.datum, (integer)fons.mensura,
            (constans character*)fons.datum);
        manu++;
    }
}

interior integer
_octetos_comparare (
    constans i8* a,
            i32  am,
    constans i8* b,
            i32  bm)
{
        i32 n = am < bm ? am : bm;
    integer c = memcmp(a, b, (size_t)n);

    si (c != ZEPHYRUM)
    {
        redde c;
    }
    redde am < bm ? -I : (am > bm ? I : ZEPHYRUM);
}

interior integer
_comparare (
    constans vacuum* x,
    constans vacuum* y)
{
    constans Diffisa* a = (constans Diffisa*)x;
    constans Diffisa* b = (constans Diffisa*)y;
             integer  c;

    si (a->genus != b->genus)
    {
        redde a->genus < b->genus ? -I : I;
    }
    c = _octetos_comparare(a->primum, a->primum_mensura, b->primum,
        b->primum_mensura);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    c = _octetos_comparare(a->secundum, a->secundum_mensura,
        b->secundum,
        b->secundum_mensura);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    redde strcmp(a->causa, b->causa);   /* ordo determinatus */
}

/* duplicatae (genus, primum, secundum aequalia) semel - prima post
 * ordinem servatur */
interior vacuum
_duplicatas_tollere (vacuum)
{
    s32 i;
    s32 j = ZEPHYRUM;

    per (i = ZEPHYRUM; i < numerus_diffisarum; i++)
    {
        si (j > ZEPHYRUM)
        {
            Diffisa* u = &diffisae[j - I];

            si (   u->genus == diffisae[i].genus
                && _octetos_comparare(u->primum, u->primum_mensura,
                       diffisae[i].primum, diffisae[i].primum_mensura)
                   == ZEPHYRUM
                && _octetos_comparare(u->secundum, u->secundum_mensura,
                       diffisae[i].secundum,
                       diffisae[i].secundum_mensura) == ZEPHYRUM)
            {
                duplicatae++;
                perge;
            }
        }
        si (j != i)
        {
            diffisae[j] = diffisae[i];
        }
        j++;
    }
    numerus_diffisarum = j;
}

/* sectio in tabula octetorum: aequalis prior reddit locum eius */
interior i32
_locare (
    ChordaAedificator* octeti,
                   i8* datum,
                  i32  mensura)
{
    chorda iam = chorda_aedificator_spectare(octeti);
       i32 i;

    per (i = ZEPHYRUM; i + mensura <= iam.mensura; i++)
    {
        si (memcmp(iam.datum + i, datum, (size_t)mensura) == ZEPHYRUM)
        {
            redde i;
        }
    }
    i = iam.mensura;
    chorda_aedificator_appendere_chorda(octeti, chorda_ex_buffer(datum,
        mensura));
    redde i;
}

interior chorda
_scribere (
    constans character* directorium,
    constans character* index_manu,
    constans character* tempus,
               Piscina* p)
{
    ChordaAedificator* a = chorda_aedificator_creare(p, MXXIV * MXXIV);
    ChordaAedificator* octeti = chorda_aedificator_creare(p,
                           CCLVI * MXXIV);
               chorda tabula;
            character b[CCCLXXX];
                  s32 i;

    per (i = ZEPHYRUM; i < numerus_diffisarum; i++)
    {
        diffisae[i].primi_locus = _locare(octeti, diffisae[i].primum,
            diffisae[i].primum_mensura);
        diffisae[i].secundi_locus = _locare(octeti,
            diffisae[i].secundum,
            diffisae[i].secundum_mensura);
    }
    tabula = chorda_aedificator_spectare(octeti);
    chorda_aedificator_appendere_literis(a,
        "/* fiducia_diffisa_tabula.c - GENERATUM: "
        "tools/fiducia_diffisa_generare.sh (NE EDITA MANU)\n *\n"
        " * Ex instantanea OneCRL fixa ");
    chorda_aedificator_appendere_literis(a, directorium);
    chorda_aedificator_appendere_literis(a, "/changeset.json\n * et ");
    chorda_aedificator_appendere_literis(a, index_manu);
    sprintf(b, ".\n * Diffisae %d (manu %d); praeteritae enabled:false"
        " %d, duplicatae %d;\n * octeti communes %d.\n */\n",
        (integer)numerus_diffisarum, (integer)manu,
        (integer)praeteritae, (integer)duplicatae,
        (integer)tabula.mensura);
    chorda_aedificator_appendere_literis(a, b);
    chorda_aedificator_appendere_literis(a,
        "#include \"fiducia_diffisa_tabula.h\"\n\n"
        "hic_manens constans i8 octeti_diffisarum[] = {\n");
    si (tabula.mensura == ZEPHYRUM)
    {
        chorda_aedificator_appendere_literis(a, "    0x00\n");
    }
    per (i = ZEPHYRUM; i < (s32)tabula.mensura; i++)
    {
        sprintf(b, "%s0x%02x%s", i % XI == ZEPHYRUM ? "    " : " ",
            (insignatus integer)tabula.datum[i],
            i + I == (s32)tabula.mensura ? "\n"
                : (i % XI == X ? ",\n" : ","));
        chorda_aedificator_appendere_literis(a, b);
    }
    chorda_aedificator_appendere_literis(a, "};\n\n"
        "constans FiduciaDiffisa fiducia_diffisae[] = {\n");
    per (i = ZEPHYRUM; i < numerus_diffisarum; i++)
    {
        sprintf(b, "    { %s,\n      octeti_diffisarum + %d, %d,"
            " octeti_diffisarum + %d, %d,\n      \"%s\" },\n",
            diffisae[i].genus == I ? "FIDUCIA_DIFFISA_EMITTENS_SERIES"
                                   : "FIDUCIA_DIFFISA_SUBIECTUM_CLAVIS",
            (integer)diffisae[i].primi_locus,
            (integer)diffisae[i].primum_mensura,
            (integer)diffisae[i].secundi_locus,
            (integer)diffisae[i].secundum_mensura, diffisae[i].causa);
        chorda_aedificator_appendere_literis(a, b);
    }
    sprintf(b, "    /* signum finis (tabula numquam vacua in C89) */\n"
        "    { FIDUCIA_DIFFISA_EMITTENS_SERIES, NIHIL, 0, NIHIL, 0,"
        " NIHIL }\n};\n\nconstans i32 fiducia_diffisae_numerus = %d;\n"
        "constans character* fiducia_diffisae_tempus = \"%s\";\n",
        (integer)numerus_diffisarum, tempus);
    chorda_aedificator_appendere_literis(a, b);
    redde chorda_aedificator_finire(a);
}

s32
principale (
      integer   argc,
    character** argv)
{
                Piscina* p;
     constans character* tempus;
                 size_t  n;

    si (argc != IV)
    {
        fprintf(stderr, "usus: fiducia_diffisa_generare <directorium>"
            " <manu.stml> <exitus.c>\n");
        redde II;
    }
    p = piscina_generare_dynamicum("fiducia_diffisa_generare",
        XVI * MXXIV * MXXIV);
    n = strlen(argv[I]);
    si (n > CDLXXX)
    {
        _mori("via nimis longa", argv[I]);
    }
    dum (n > ZEPHYRUM && argv[I][n - I] == '/')
    {
        argv[I][--n] = '\0';
    }
    tempus = strrchr(argv[I], '/') ? strrchr(argv[I], '/')
        + I : argv[I];
    si (   strlen(tempus) != X || tempus[IV] != '-'
        || tempus[VII]    != '-')
    {
        _mori("directorium non YYYY-MM-DD", tempus);
    }
    _summas_probare(argv[I], p);
    _legere_mutationes(argv[I], p);
    _legere_manu(argv[II], p);
    qsort(diffisae, (memoriae_index)numerus_diffisarum,
        magnitudo(Diffisa),
        _comparare);
    _duplicatas_tollere();
    si (!filum_scribere(argv[III], _scribere(argv[I], argv[II], tempus,
            p)))
    {
        _mori("scribi non potest", argv[III]);
    }
    redde ZEPHYRUM;
}
