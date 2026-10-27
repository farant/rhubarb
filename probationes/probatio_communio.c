/* probatio_communio.c - communio (project-specs/communio-spec.md,
 * plan communio-plan-1 T3): acta et massae inter duo volumina sine
 * rete - exportatio actorum propriorum post cursorem, massae prius,
 * importatio ordine, cursor solum post acta accepta. */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include "volumen.h"
#include "communio.h"
#include "internamentum.h"
#include "thema.h"
#include "pictor_documentum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>

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

interior s64
actum_addere (
               Volumen* vol,
    constans character* genus,
    constans character* datum)
{
    redde volumen_actum_appendere(vol, genus, chorda_ex_literis(datum,
        piscina));
}

/* numerus actorum replicae datae in volumine */
interior i32
acta_replicae (
    Volumen* vol,
     chorda  replica)
{
    Xar* l;
    i32  i;
    i32  n;

    l = volumen_acta_legere(vol, ZEPHYRUM, piscina);
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (chorda_aequalis(((VolumenActum*)xar_obtinere(l, i))
                ->identitas.replica, replica))
        {
            n++;
        }
    }
    redde n;
}

interior chorda
via_aliena (
               Volumen* fons,
    constans character* via)
{
    redde chorda_concatenare(chorda_concatenare(chorda_ex_literis(
        "aliena/", piscina), volumen_replica(fons), piscina),
        chorda_ex_literis(via, piscina), piscina);
}

/* transferre, iterum, idempotens a cursore nullo */
interior vacuum
_sectio_transferre (
    CredoContextus* c)
{
            Volumen* a;
            Volumen* b;
                s64  cursor;
    CommunioRelatio  r;
             chorda  lectum;
                b32  inventum;
                i32  k;
                i32  propria;

    (vacuum)c;
    a = volumen_temporarium(piscina, "probatio_communio_a");
    b = volumen_temporarium(piscina, "probatio_communio_b");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    per (k = ZEPHYRUM; k < V; k++)
    {
        CREDO_NECESSE(actum_addere(a, "x/actum", "{}") > ZEPHYRUM);
    }
    CREDO_NECESSE(volumen_plagulam_condere(a, chorda_ex_literis(
        "documentum", piscina), chorda_ex_literis("A-doc", piscina),
        "probatio"));
    propria = acta_replicae(a, volumen_replica(a));

    CREDO_GRADUS("transferre");
    cursor = ZEPHYRUM;
    CREDO_VERUM(communio_transferre(a, b, &cursor, &r, piscina));
    CREDO_AEQUALIS_I32(r.importata, propria);
    CREDO_AEQUALIS_I32(r.refusa, ZEPHYRUM);
    CREDO_AEQUALIS_S64(cursor, volumen_summa_actorum(a));
    CREDO_AEQUALIS_S64(r.cursor, cursor);
    CREDO_AEQUALIS_I32(acta_replicae(b, volumen_replica(a)), propria);
    lectum = volumen_plagulam_promere(b, via_aliena(a, "/documentum"),
        piscina, &inventum);
    CREDO_VERUM(inventum);
    CREDO_CHORDA_AEQUALIS_LITERIS(lectum, "A-doc");

    CREDO_GRADUS("iterum: nihil novum");
    CREDO_VERUM(communio_transferre(a, b, &cursor, &r, piscina));
    CREDO_AEQUALIS_I32(r.importata, ZEPHYRUM);
    CREDO_AEQUALIS_I32(r.iam_aderant, ZEPHYRUM);
    CREDO_AEQUALIS_S64(cursor, volumen_summa_actorum(a));

    CREDO_GRADUS("idempotens a cursore nullo");
    cursor = ZEPHYRUM;
    CREDO_VERUM(communio_transferre(a, b, &cursor, &r, piscina));
    CREDO_AEQUALIS_I32(r.importata, ZEPHYRUM);
    CREDO_AEQUALIS_I32(r.iam_aderant, propria);
    CREDO_AEQUALIS_I32(acta_replicae(b, volumen_replica(a)), propria);
    volumen_claudere(a);
    volumen_claudere(b);
}

/* eadem machina, ambo sensus: nullum actum perditur (Review Focus
 * 1); acta aliena non reexportantur (solum propria) */
interior vacuum
_sectio_eadem_machina (
    CredoContextus* c)
{
            Volumen* a;
            Volumen* b;
                s64  ab;
                s64  ba;
    CommunioRelatio  r;
                i32  k;
                i32  pa;
                i32  pb;

    (vacuum)c;
    a = volumen_temporarium(piscina, "probatio_communio_ma");
    b = volumen_temporarium(piscina, "probatio_communio_mb");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    per (k = ZEPHYRUM; k < X; k++)
    {
        (vacuum)actum_addere(a, "x/a", "{}");
        (vacuum)actum_addere(b, "x/b", "{}");
    }
    pa = acta_replicae(a, volumen_replica(a));
    pb = acta_replicae(b, volumen_replica(b));
    ab = ZEPHYRUM;
    ba = ZEPHYRUM;
    CREDO_VERUM(communio_transferre(a, b, &ab, &r, piscina));
    CREDO_VERUM(communio_transferre(b, a, &ba, &r, piscina));
    CREDO_AEQUALIS_I32(r.importata, pb);
    CREDO_AEQUALIS_I32(acta_replicae(a, volumen_replica(b)), pb);
    CREDO_AEQUALIS_I32(acta_replicae(b, volumen_replica(a)), pa);
    CREDO_AEQUALIS_S64(volumen_summa_actorum(a), (s64)(pa + pb));
    CREDO_AEQUALIS_S64(volumen_summa_actorum(b), (s64)(pa + pb));
    volumen_claudere(a);
    volumen_claudere(b);
}

/* derivata domi manent; massae ante acta sua */
interior vacuum
_sectio_massae_et_derivata (
    CredoContextus* c)
{
            Volumen* a;
            Volumen* b;
                s64  cursor;
    CommunioRelatio  r;
                b32  inventum;
          character  hex[SIGILLUM_HEX_MENSURA];
          character  datum[CXXVIII];

    (vacuum)c;
    a = volumen_temporarium(piscina, "probatio_communio_da");
    b = volumen_temporarium(piscina, "probatio_communio_db");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    CREDO_NECESSE(volumen_plagulam_condere(a, chorda_ex_literis(
        "checkpoint/7", piscina), chorda_ex_literis("CP", piscina),
        "pictor:checkpoint"));
    CREDO_NECESSE(volumen_massam_condere(a, chorda_ex_literis("IMAGO",
        piscina), hex));
    sprintf(datum, "<imago massa=\"%s\"/>", hex);
    CREDO_NECESSE(actum_addere(a, "x/imago", datum) > ZEPHYRUM);
    cursor = ZEPHYRUM;
    CREDO_VERUM(communio_transferre(a, b, &cursor, &r, piscina));
    CREDO_AEQUALIS_I32(r.refusa, ZEPHYRUM);
    CREDO_VERUM(r.massae >= I);

    CREDO_GRADUS("checkpoint non transit");
    (vacuum)volumen_plagulam_promere(b, via_aliena(a, "/checkpoint/7"),
        piscina, &inventum);
    CREDO_FALSUM(inventum);

    CREDO_GRADUS("massa imaginis transit");
    (vacuum)volumen_massam_promere(b, chorda_ex_literis(hex, piscina),
        piscina, &inventum);
    CREDO_VERUM(inventum);
    volumen_claudere(a);
    volumen_claudere(b);
}

/* refusa: actum corruptum in destinatione (identitas eadem, datum
 * aliud) - transitus ibi sistit, cursor ante id; iteratio non salit */
interior vacuum
_sectio_refusa (
    CredoContextus* c)
{
            Volumen* a;
            Volumen* b;
                Xar* l;
       VolumenActum  falsum;
                s64  cursor;
                s64  seq_falsi;
    CommunioRelatio  r;
                i32  k;

    (vacuum)c;
    a = volumen_temporarium(piscina, "probatio_communio_ra");
    b = volumen_temporarium(piscina, "probatio_communio_rb");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    per (k = ZEPHYRUM; k < V; k++)
    {
        (vacuum)actum_addere(a, "x/actum", "{}");
    }
    l = volumen_acta_legere(a, ZEPHYRUM, piscina);
    CREDO_NECESSE(l && xar_numerus(l) > III);
    falsum        = *(VolumenActum*)xar_obtinere(l, III);
    seq_falsi     = falsum.seq;
    falsum.datum  = chorda_ex_literis("{\"corruptum\":1}", piscina);
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b, &falsum),
        (i32)VOLUMEN_IMPORTATUM);
    cursor = ZEPHYRUM;
    CREDO_FALSUM(communio_transferre(a, b, &cursor, &r, piscina));
    CREDO_AEQUALIS_I32(r.refusa, I);
    CREDO_AEQUALIS_S64(cursor, seq_falsi - I);
    CREDO_AEQUALIS_I32(r.importata, III);

    CREDO_GRADUS("iteratio non salit");
    CREDO_FALSUM(communio_transferre(a, b, &cursor, &r, piscina));
    CREDO_AEQUALIS_S64(cursor, seq_falsi - I);
    volumen_claudere(a);
    volumen_claudere(b);
}

/* T4: ictus penicilli ad (x, y) */
interior s64
ictus (
     PictorDocumentum* doc,
                  i32  x,
                  i32  y,
                  i32  color)
{
    character t[CXXVIII];

    sprintf(t, "<ictus instrumentum=\"penicillus\" color=\"%lu\""
        " magnitudo=\"3\"><punctum x=\"%lu\" y=\"%lu\"/></ictus>",
        (insignatus longus)color, (insignatus longus)x,
        (insignatus longus)y);
    redde pictor_documentum_actum(doc, chorda_ex_literis(t, piscina));
}

/* series operationum eadem in A et in pictura homonyma B (colore
 * alio): seqs congruunt, ergo checkpoints B in numeris picturae A
 * iacent - documentum alienum eos legens pixela B ostenderet */
interior b32
operationes (
     PictorDocumentum* doc,
                  i32  color)
{
     i8* indices;
    i32  k;

    per (k = ZEPHYRUM; k < III; k++)
    {
        si (ictus(doc, X + k * X, X, (color + k) % XVI) <= ZEPHYRUM)
        {
            redde FALSUM;
        }
    }
    indices = (i8*)piscina_allocare(piscina, (memoriae_index)(XX * XX));
    memset(indices, (integer)((color + II) % XVI), (size_t)(XX * XX));
    redde pictor_documentum_imaginem_inserere(doc, indices, XX, XX, XL,
               XX, I) > ZEPHYRUM
        && pictor_documentum_revocare(doc)
        && pictor_documentum_reficere(doc)
        && pictor_documentum_revocare(doc)
        && ictus(doc, L, XL, (color + V) % XVI) > ZEPHYRUM;
}

/* T4 (spec D4): pictura alterius replicae in B legitur - pixela eadem
 * (strata, imago, revocatio, refectio, ramus), checkpoint fontis domi,
 * checkpoint picturae localis eiusdem nominis NON adhibetur; picturae
 * B propriae intactae; scriptura in alienam recusatur; enumeratio
 * localis alienas non numerat */
interior vacuum
_sectio_pictura_aliena (
    CredoContextus* c)
{
                Volumen* a;
                Volumen* b;
    InternamentumChorda* intern;
       PictorDocumentum* da;
       PictorDocumentum* radix_b;
       PictorDocumentum* homonyma;
       PictorDocumentum* aliena;
                    Xar* l;
                    s64  cursor;
                 chorda  sig_a;
                 chorda  sig_radicis;
                    s64  numerus;
                    i32  k;

    (vacuum)c;
    thema_initiare();
    intern  = internamentum_creare(piscina);
    a       = volumen_temporarium(piscina, "probatio_communio_pa");
    b       = volumen_temporarium(piscina, "probatio_communio_pb");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);

    /* A: pictura "p", intervallum II (checkpoints crebri) */
    da = pictor_documentum_creare(piscina, intern, a, "p", C, LX, II);
    CREDO_NECESSE_NON_NIHIL(da);
    CREDO_NECESSE(operationes(da, ZEPHYRUM));
    sig_a = pictor_documentum_sigillum_compositi_hex(da, piscina);

    /* B: pictura "p" propria PRIMA, eadem series (alio colore), deinde
     * radix propria */
    homonyma = pictor_documentum_creare(piscina, intern, b, "p", C, LX,
        II);
    CREDO_NECESSE_NON_NIHIL(homonyma);
    CREDO_NECESSE(operationes(homonyma, VII));
    radix_b = pictor_documentum_creare(piscina, intern, b, "", C, LX,
        II);
    CREDO_NECESSE_NON_NIHIL(radix_b);
    per (k = ZEPHYRUM; k < IV; k++)
    {
        (vacuum)ictus(radix_b, V, V + k, I);
    }
    sig_radicis = pictor_documentum_sigillum_compositi_hex(radix_b,
        piscina);

    cursor = ZEPHYRUM;
    CREDO_VERUM(communio_transferre(a, b, &cursor, NIHIL, piscina));

    CREDO_GRADUS("enumeratio aliena");
    l = pictor_documenta_aliena_enumerare(b, piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_AEQUALIS_I32(xar_numerus(l), I);
    CREDO_CHORDA_AEQUALIS(((PictorPicturaAliena*)xar_obtinere(l,
        ZEPHYRUM))->replica, volumen_replica(a));
    CREDO_CHORDA_AEQUALIS_LITERIS(((PictorPicturaAliena*)xar_obtinere(l,
        ZEPHYRUM))->spatium, "p");

    CREDO_GRADUS("enumeratio localis alienas non numerat");
    l = pictor_documenta_enumerare(b, piscina);
    CREDO_AEQUALIS_I32(l ? xar_numerus(l) : ZEPHYRUM, II);
    l = pictor_documenta_recentia(b, piscina);
    CREDO_AEQUALIS_I32(l ? xar_numerus(l) : ZEPHYRUM, II);

    CREDO_GRADUS("pixela eadem");
    aliena = pictor_documentum_aperire_alienum(piscina, intern, b,
        volumen_replica(a), "p");
    CREDO_NECESSE_NON_NIHIL(aliena);
    CREDO_CHORDA_AEQUALIS(pictor_documentum_sigillum_compositi_hex(
        aliena, piscina), sig_a);

    CREDO_GRADUS("propriae intactae");
    radix_b = pictor_documentum_aperire(piscina, intern, b, "");
    CREDO_NECESSE_NON_NIHIL(radix_b);
    CREDO_CHORDA_AEQUALIS(pictor_documentum_sigillum_compositi_hex(
        radix_b, piscina), sig_radicis);

    CREDO_GRADUS("scriptura in alienam recusatur");
    numerus = volumen_summa_actorum(b);
    CREDO_AEQUALIS_S64(ictus(aliena, V, V, I), (s64)ZEPHYRUM);
    CREDO_FALSUM(pictor_documentum_revocare(aliena));
    CREDO_AEQUALIS_S64(volumen_summa_actorum(b), numerus);
    volumen_claudere(a);
    volumen_claudere(b);
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "transferre, iterum, idempotens", _sectio_transferre, _parare,
        _purgare, NIHIL, NIHIL },
    { "eadem machina, ambo sensus", _sectio_eadem_machina, _parare,
        _purgare, NIHIL, NIHIL },
    { "massae et derivata", _sectio_massae_et_derivata, _parare,
        _purgare, NIHIL, NIHIL },
    { "refusa sistit", _sectio_refusa, _parare, _purgare, NIHIL,
        NIHIL },
    { "pictura aliena (T4)", _sectio_pictura_aliena, _parare, _purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("communio", SECTIONES);
}
