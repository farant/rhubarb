/* probatio_communio_forma.c - communio_forma (communio-plan-2 T2):
 * acta ut JSON (iter et reditus ad octetum), validatio in limine
 * retis (M4), corpora mala, massae nominatae. */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "json.h"
#include "moneta.h"
#include "volumen.h"
#include "communio_forma.h"
#include "credo.h"
#include <string.h>
#include <stdio.h>

hic_manens Piscina* piscina;

interior vacuum
_parare (
    CredoContextus* c)
{
    piscina = c->piscina;
}

interior vacuum
_purgare (
    CredoContextus* c)
{
    (vacuum)c;
    piscina = NIHIL;
}

interior chorda
lit (
    constans character* s)
{
    redde chorda_ex_literis(s, piscina);
}

/* actum plenum: basis, datum cum citationibus, retro-lineis, novis
 * lineis et 'é'; hora prope 2^62 */
interior VolumenActum
actum_bonum (vacuum)
{
    VolumenActum a;
       character replica[MONETA_ULID_MENSURA];

    moneta_ulid(replica);
    memset(&a, ZEPHYRUM, magnitudo(a));
    a.genus               = lit("p/ictus");
    a.datum               = lit("<ictus t=\"a\\\"b\">\\ retro\n"
                                "linea \xc3\xa9</ictus>");
    a.momentum           = lit("2026-10-27T10:00:00.000Z");
    a.identitas.hora     = ((s64)I << LXII) + (s64)M * V;
    a.identitas.replica  = lit(replica);
    a.identitas.machina  = lit("machina-A");
    a.basis.hora         = a.identitas.hora - I;
    a.basis.replica      = a.identitas.replica;
    a.basis.machina      = a.identitas.machina;
    a.seq                = VII;
    a.seq_originis       = XLII;
    redde a;
}

interior b32
reditus (
    constans VolumenActum* a,
             VolumenActum* exitus,
                   chorda* textus)
{
       JsonValor* o;
    JsonResultus  r;
          chorda  causa;

    o = communio_actum_scribere(a, piscina);
    si (!o)
    {
        redde FALSUM;
    }
    *textus  = json_scribere(o, piscina);
    r        = json_legere(*textus, piscina);
    redde r.successus && communio_actum_legere(r.radix, exitus, &causa,
        piscina);
}

/* chorda continet subchordam? */
interior b32
continet (
                chorda  c,
    constans character* sub)
{
    i32 n;
    i32 i;

    n = (i32)strlen(sub);
    per (i = ZEPHYRUM; i + n <= c.mensura; i++)
    {
        si (memcmp(c.datum + i, sub, (size_t)n) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_sectio_reditus (
    CredoContextus* c)
{
    VolumenActum a;
    VolumenActum b;
          chorda textus;

    (vacuum)c;
    a = actum_bonum();
    CREDO_NECESSE(reditus(&a, &b, &textus));
    CREDO_CHORDA_AEQUALIS(b.genus, a.genus);
    CREDO_CHORDA_AEQUALIS(b.datum, a.datum);
    CREDO_CHORDA_AEQUALIS(b.momentum, a.momentum);
    CREDO_AEQUALIS_S64(b.identitas.hora, a.identitas.hora);
    CREDO_CHORDA_AEQUALIS(b.identitas.replica, a.identitas.replica);
    CREDO_CHORDA_AEQUALIS(b.identitas.machina, a.identitas.machina);
    CREDO_AEQUALIS_S64(b.basis.hora, a.basis.hora);
    CREDO_CHORDA_AEQUALIS(b.basis.replica, a.basis.replica);
    CREDO_CHORDA_AEQUALIS(b.basis.machina, a.basis.machina);
    CREDO_AEQUALIS_S64(b.seq_originis, XLII);
    CREDO_AEQUALIS_S64(b.seq, ZEPHYRUM);

    CREDO_GRADUS("sine basi");
    a.basis.hora             = ZEPHYRUM;
    a.basis.replica.datum    = NIHIL;
    a.basis.replica.mensura  = ZEPHYRUM;
    a.basis.machina          = a.basis.replica;
    CREDO_NECESSE(reditus(&a, &b, &textus));
    CREDO_FALSUM(continet(textus, "basis_hora"));
    CREDO_AEQUALIS_S64(b.basis.hora, ZEPHYRUM);
    CREDO_AEQUALIS_I32(b.basis.replica.mensura, ZEPHYRUM);
    CREDO_CHORDA_AEQUALIS(b.datum, a.datum);
}

/* actum malum: FALSUM cum causa non vacua */
interior b32
malum (
    constans VolumenActum* a)
{
    chorda causa;

    causa.datum    = NIHIL;
    causa.mensura  = ZEPHYRUM;
    redde !communio_actum_validum(a, &causa, piscina)
        && causa.mensura > ZEPHYRUM;
}

interior vacuum
_sectio_validum (
    CredoContextus* c)
{
          VolumenActum  a;
          VolumenActum  m;
                chorda  causa;
             character  replica[MONETA_ULID_MENSURA];
    constans character* signa;
                   i32  i;

    (vacuum)c;
    a = actum_bonum();
    CREDO_VERUM(communio_actum_validum(&a, &causa, piscina));

    CREDO_GRADUS("replica XXV signorum");
    m                            = a;
    m.identitas.replica.mensura  = XXV;
    CREDO_VERUM(malum(&m));

    CREDO_GRADUS("replica minuscula");
    memcpy(replica, a.identitas.replica.datum, (size_t)XXVI);
    replica[XXVI] = '\0';
    per (i = ZEPHYRUM; i < XXVI; i++)
    {
        si (replica[i] >= 'A' && replica[i] <= 'Z')
        {
            replica[i] = (character)(replica[i] - 'A' + 'a');
        }
    }
    m                    = a;
    m.identitas.replica  = lit(replica);
    CREDO_VERUM(malum(&m));

    CREDO_GRADUS("signa extra Crockford: I L O U");
    signa = "ILOU";
    per (i = ZEPHYRUM; i < IV; i++)
    {
        memcpy(replica, a.identitas.replica.datum, (size_t)XXVI);
        replica[V]           = signa[i];
        m                    = a;
        m.identitas.replica  = lit(replica);
        CREDO_VERUM(malum(&m));
    }

    CREDO_GRADUS("hora, seq_originis, genus");
    m                 = a;
    m.identitas.hora  = ZEPHYRUM;
    CREDO_VERUM(malum(&m));
    m               = a;
    m.seq_originis  = ZEPHYRUM;
    CREDO_VERUM(malum(&m));
    m        = a;
    m.genus  = lit("");
    CREDO_VERUM(malum(&m));

    CREDO_GRADUS("via sub aliena/ reservata");
    m        = a;
    m.genus  = lit("plagula-condita");
    m.datum  = lit("{\"via\":\"aliena/X/y\",\"sigillum\":\"0\"}");
    CREDO_VERUM(malum(&m));
    m.genus = lit("plagula-remota");
    m.datum = lit("{\"via\":\"aliena/X/y\"}");
    CREDO_VERUM(malum(&m));
    m.datum = lit("{\"via\":\"p/documentum\"}");
    CREDO_VERUM(communio_actum_validum(&m, &causa, piscina));
}

interior vacuum
_sectio_corpora (
    CredoContextus* c)
{
    VolumenActum  a;
    VolumenActum  b;
             Xar* acta;
             Xar* lecta;
          chorda  corpus;
          chorda  causa;
             i32  i;

    (vacuum)c;
    a = actum_bonum();
    b = actum_bonum();
    acta = xar_creare(piscina, magnitudo(VolumenActum));
    *(VolumenActum*)xar_addere(acta) = a;
    *(VolumenActum*)xar_addere(acta) = b;

    CREDO_GRADUS("corpus bonum");
    corpus  = communio_acta_scribere(acta, piscina);
    lecta   = communio_acta_legere(corpus, &causa, piscina);
    CREDO_NECESSE_NON_NIHIL(lecta);
    CREDO_AEQUALIS_I32(xar_numerus(lecta), II);
    CREDO_CHORDA_AEQUALIS(((VolumenActum*)xar_obtinere(lecta, I))
        ->identitas.replica, b.identitas.replica);

    CREDO_GRADUS("corpora mala");
    CREDO_NIHIL(communio_acta_legere(lit("non json"), &causa, piscina));
    CREDO_NIHIL(communio_acta_legere(lit("{\"alia\":[]}"), &causa,
        piscina));
    CREDO_NIHIL(communio_acta_legere(lit("{\"acta\":[{\"genus\":1}]}"),
        &causa, piscina));
    b.seq_originis                         = ZEPHYRUM;
    *(VolumenActum*)xar_obtinere(acta, I)  = b;
    CREDO_NIHIL(communio_acta_legere(communio_acta_scribere(acta,
        piscina), &causa, piscina));
    CREDO_VERUM(causa.mensura > ZEPHYRUM);

    CREDO_GRADUS("plus quam COMMUNIO_ACTA_MAXIMA");
    acta = xar_creare(piscina, magnitudo(VolumenActum));
    per (i = ZEPHYRUM; i <= COMMUNIO_ACTA_MAXIMA; i++)
    {
        *(VolumenActum*)xar_addere(acta) = a;
    }
    CREDO_NIHIL(communio_acta_legere(communio_acta_scribere(acta,
        piscina), &causa, piscina));
}

interior vacuum
_sectio_massae (
    CredoContextus* c)
{
    VolumenActum  a;
             Xar* l;
       character  datum[CD];
       character  prima[LXV];
       character  secunda[LXV];
             i32  i;

    (vacuum)c;
    per (i = ZEPHYRUM; i < LXIV; i++)
    {
        prima[i]    = "0123456789abcdef"[i % XVI];
        secunda[i]  = "fedcba9876543210"[i % XVI];
    }
    prima[LXIV]    = '\0';
    secunda[LXIV]  = '\0';
    /* duo sigilla, series LXV, series maiuscula */
    sprintf(datum, "{\"s\":\"%s\",\"t\":\"%s\",\"u\":\"%sa\","
        "\"v\":\"ABCDEF%s\"}", prima, secunda, prima, secunda + VI);
    a        = actum_bonum();
    a.datum  = lit(datum);
    l        = communio_massas_nominatas(&a, piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), II);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(l, ZEPHYRUM),
        prima);
    CREDO_CHORDA_AEQUALIS_LITERIS(*(chorda*)xar_obtinere(l, I),
        secunda);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "iter et reditus", _sectio_reditus, _parare, _purgare, NIHIL,
        NIHIL },
    { "validum (M4)", _sectio_validum, _parare, _purgare, NIHIL,
        NIHIL },
    { "corpus malum", _sectio_corpora, _parare, _purgare, NIHIL,
        NIHIL },
    { "massae nominatae", _sectio_massae, _parare, _purgare, NIHIL,
        NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("communio_forma", SECTIONES);
}
