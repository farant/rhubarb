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
#include "cives.h"
#include "moneta.h"
#include "reactor.h"
#include "hospitium.h"
#include "communio_servus.h"
#include "communio_cliens.h"
#include "sigillum.h"
#include "lectiones.h"
#include "filum.h"
#include "credo.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>

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
                 chorda  sig_cognominis;
                    Xar* recentia_ante;
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
    sig_cognominis = pictor_documentum_sigillum_compositi_hex(homonyma,
        piscina);
    recentia_ante = pictor_documenta_recentia(b, piscina);

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

    CREDO_GRADUS("homonyma propria intacta (recensio C1)");
    homonyma = pictor_documentum_aperire(piscina, intern, b, "p");
    CREDO_NECESSE_NON_NIHIL(homonyma);
    CREDO_CHORDA_AEQUALIS(pictor_documentum_sigillum_compositi_hex(
        homonyma, piscina), sig_cognominis);
    l = pictor_documenta_recentia(b, piscina);
    CREDO_NECESSE_NON_NIHIL(l);
    CREDO_NECESSE_NON_NIHIL(recentia_ante);
    CREDO_AEQUALIS_I32(xar_numerus(l), xar_numerus(recentia_ante));
    per (k = ZEPHYRUM; k < xar_numerus(l); k++)
    {
        CREDO_CHORDA_AEQUALIS(((PictorPictura*)xar_obtinere(l, k))
            ->spatium, ((PictorPictura*)xar_obtinere(recentia_ante, k))
            ->spatium);
        CREDO_AEQUALIS_S64(((PictorPictura*)xar_obtinere(l, k))->seq,
            ((PictorPictura*)xar_obtinere(recentia_ante, k))->seq);
    }

    CREDO_GRADUS("scriptura in alienam recusatur");
    numerus = volumen_summa_actorum(b);
    CREDO_AEQUALIS_S64(ictus(aliena, V, V, I), (s64)ZEPHYRUM);
    CREDO_FALSUM(pictor_documentum_revocare(aliena));
    CREDO_AEQUALIS_S64(volumen_summa_actorum(b), numerus);
    volumen_claudere(a);
    volumen_claudere(b);
}

/* recensio I3: observator historiae vocationes numerat */
interior vacuum
observator_numerans (
    Historia* h,
      vacuum* ctx)
{
    (vacuum)h;
    *(i32*)ctx += I;
}

/* T5: picturam A in B sequi - sigillum aequale */
interior b32
aequales (
    PictorDocumentum* a,
    PictorDocumentum* b)
{
    redde chorda_aequalis(pictor_documentum_sigillum_compositi_hex(a,
        piscina), pictor_documentum_sigillum_compositi_hex(b, piscina));
}

/* T5 (communio II.9): documentum alienum apertum acta nova sequitur -
 * ictus novi, revocatio cum ramo, strata nova et occulta; nihil novum
 * -> FALSUM; aequale aperturae recenti (Review Focus 4) */
interior vacuum
_sectio_sequi (
    CredoContextus* c)
{
                Volumen* a;
                Volumen* b;
    InternamentumChorda* intern;
       PictorDocumentum* da;
       PictorDocumentum* aliena;
       PictorDocumentum* recens;
                    i32  vocationes;
                    s64  cursor;

    (vacuum)c;
    vocationes = ZEPHYRUM;
    thema_initiare();
    intern  = internamentum_creare(piscina);
    a       = volumen_temporarium(piscina, "probatio_communio_sa");
    b       = volumen_temporarium(piscina, "probatio_communio_sb");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    da = pictor_documentum_creare(piscina, intern, a, "p", C, LX, II);
    CREDO_NECESSE_NON_NIHIL(da);
    CREDO_NECESSE(operationes(da, ZEPHYRUM));
    cursor = ZEPHYRUM;
    CREDO_NECESSE(communio_transferre(a, b, &cursor, NIHIL, piscina));
    aliena = pictor_documentum_aperire_alienum(piscina, intern, b,
        volumen_replica(a), "p");
    CREDO_NECESSE_NON_NIHIL(aliena);
    CREDO_VERUM(aequales(da, aliena));

    CREDO_GRADUS("nihil novum");
    CREDO_FALSUM(pictor_documentum_sequi(aliena));

    CREDO_GRADUS("sequitur: ictus novi; observatores vocati");
    CREDO_VERUM(historia_observatorem_addere(aliena->historia,
        observator_numerans, &vocationes));
    CREDO_NECESSE(ictus(da, LXX, XXX, IX) > ZEPHYRUM);
    CREDO_NECESSE(ictus(da, LXXX, XXX, X) > ZEPHYRUM);
    CREDO_NECESSE(communio_transferre(a, b, &cursor, NIHIL, piscina));
    CREDO_VERUM(pictor_documentum_sequi(aliena));
    CREDO_VERUM(vocationes > ZEPHYRUM);
    CREDO_VERUM(aequales(da, aliena));
    CREDO_FALSUM(pictor_documentum_sequi(aliena));

    CREDO_GRADUS("revocatio sequitur (ramus)");
    CREDO_NECESSE(pictor_documentum_revocare(da));
    CREDO_NECESSE(pictor_documentum_revocare(da));
    CREDO_NECESSE(ictus(da, XC, L, XI) > ZEPHYRUM);
    CREDO_NECESSE(communio_transferre(a, b, &cursor, NIHIL, piscina));
    CREDO_VERUM(pictor_documentum_sequi(aliena));
    CREDO_VERUM(aequales(da, aliena));
    recens = pictor_documentum_aperire_alienum(piscina, intern, b,
        volumen_replica(a), "p");
    CREDO_NECESSE_NON_NIHIL(recens);
    CREDO_VERUM(aequales(recens, aliena));

    CREDO_GRADUS("revocatio sola sequitur");
    CREDO_NECESSE(pictor_documentum_revocare(da));
    CREDO_NECESSE(communio_transferre(a, b, &cursor, NIHIL, piscina));
    CREDO_VERUM(pictor_documentum_sequi(aliena));
    CREDO_VERUM(aequales(da, aliena));

    CREDO_GRADUS("strata sequuntur");
    CREDO_NECESSE(pictor_documentum_actum(da, chorda_ex_literis(
        "<stratum actio=\"novum\" id=\"3\" supra=\"1\"/>", piscina))
        > ZEPHYRUM);
    CREDO_NECESSE(ictus(da, XX, L, IV) > ZEPHYRUM);
    CREDO_NECESSE(pictor_documentum_actum(da, chorda_ex_literis(
        "<stratum actio=\"visibile\" id=\"1\" valor=\"0\"/>", piscina))
        > ZEPHYRUM);
    CREDO_NECESSE(communio_transferre(a, b, &cursor, NIHIL, piscina));
    CREDO_VERUM(pictor_documentum_sequi(aliena));
    CREDO_VERUM(aequales(da, aliena));
    CREDO_AEQUALIS_I32(pictor_documentum_numerus_stratorum(aliena),
        pictor_documentum_numerus_stratorum(da));
    volumen_claudere(a);
    volumen_claudere(b);
}

/* acta ictuum picturae "p" post cursorem (checkpoints omissi:
 * intervallum II ea interserit) */
interior Xar*
ictus_post (
    Volumen* vol,
        s64  cursor)
{
    Xar* omnia;
    Xar* ictus_soli;
    i32  i;

    omnia       = volumen_acta_propria_legere(vol, cursor, piscina);
    ictus_soli  = xar_creare(piscina, magnitudo(VolumenActum));
    per (i = ZEPHYRUM; omnia && i < xar_numerus(omnia); i++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(omnia, i);

        si (chorda_aequalis_literis(a->genus, "p/ictus"))
        {
            *(VolumenActum*)xar_addere(ictus_soli) = *a;
        }
    }
    redde ictus_soli;
}

/* plan-2 T1 (M1): acta extra ordinem advecta - ictus posterior ante
 * priorem importatur; sequi semper aequale aperturae recenti */
interior vacuum
_sectio_sequi_extra_ordinem (
    CredoContextus* c)
{
                Volumen* a;
                Volumen* b;
    InternamentumChorda* intern;
       PictorDocumentum* da;
       PictorDocumentum* aliena;
       PictorDocumentum* recens;
                    Xar* nova;
                    s64  cursor;
                    s64  ultimum;

    (vacuum)c;
    thema_initiare();
    intern  = internamentum_creare(piscina);
    a       = volumen_temporarium(piscina, "probatio_communio_oa");
    b       = volumen_temporarium(piscina, "probatio_communio_ob");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    da = pictor_documentum_creare(piscina, intern, a, "p", C, LX, II);
    CREDO_NECESSE_NON_NIHIL(da);
    CREDO_NECESSE(operationes(da, ZEPHYRUM));
    cursor = ZEPHYRUM;
    CREDO_NECESSE(communio_transferre(a, b, &cursor, NIHIL, piscina));
    aliena = pictor_documentum_aperire_alienum(piscina, intern, b,
        volumen_replica(a), "p");
    CREDO_NECESSE_NON_NIHIL(aliena);

    /* duo ictus novi in A; B posteriorem prius accipit */
    CREDO_NECESSE(ictus(da, LXX, XXX, IX) > ZEPHYRUM);
    ultimum = ictus(da, LXXX, XXX, X);
    CREDO_NECESSE(ultimum > ZEPHYRUM);
    nova = ictus_post(a, cursor);
    CREDO_NECESSE_NON_NIHIL(nova);
    CREDO_NECESSE(xar_numerus(nova) == II);

    CREDO_GRADUS("posterior prius");
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b,
        (VolumenActum*)xar_obtinere(nova, I)), (i32)VOLUMEN_IMPORTATUM);
    (vacuum)pictor_documentum_sequi(aliena);
    recens = pictor_documentum_aperire_alienum(piscina, intern, b,
        volumen_replica(a), "p");
    CREDO_NECESSE_NON_NIHIL(recens);
    CREDO_VERUM(aequales(recens, aliena));

    CREDO_GRADUS("prior postea: status idem ac A");
    CREDO_AEQUALIS_I32((i32)volumen_actum_importare(b,
        (VolumenActum*)xar_obtinere(nova, ZEPHYRUM)),
        (i32)VOLUMEN_IMPORTATUM);
    CREDO_VERUM(pictor_documentum_sequi(aliena));
    CREDO_VERUM(aequales(da, aliena));
    recens = pictor_documentum_aperire_alienum(piscina, intern, b,
        volumen_replica(a), "p");
    CREDO_NECESSE_NON_NIHIL(recens);
    CREDO_VERUM(aequales(recens, aliena));
    volumen_claudere(a);
    volumen_claudere(b);
}

/* mensura: pictura D ictuum - apertio aliena et sequi unius ictus
 * novi (nulla assertio temporis) */
interior vacuum
_sectio_mensura (
    CredoContextus* c)
{
                Volumen* a;
                Volumen* b;
    InternamentumChorda* intern;
       PictorDocumentum* da;
       PictorDocumentum* aliena;
                    s64  cursor;
                clock_t  t0;
                    i32  k;

    (vacuum)c;
    thema_initiare();
    intern  = internamentum_creare(piscina);
    a       = volumen_temporarium(piscina, "probatio_communio_ma5");
    b       = volumen_temporarium(piscina, "probatio_communio_mb5");
    CREDO_NECESSE_NON_NIHIL(a);
    CREDO_NECESSE_NON_NIHIL(b);
    da = pictor_documentum_creare(piscina, intern, a, "p", CC, CC,
        LXIV);
    CREDO_NECESSE_NON_NIHIL(da);
    per (k = ZEPHYRUM; k < D; k++)
    {
        (vacuum)ictus(da, (k * VII) % CC, (k * XIII) % CC, k % XVI);
    }
    cursor = ZEPHYRUM;
    CREDO_NECESSE(communio_transferre(a, b, &cursor, NIHIL, piscina));
    t0 = clock();
    aliena = pictor_documentum_aperire_alienum(piscina, intern, b,
        volumen_replica(a), "p");
    imprimere("  apertio aliena D ictuum: %.1f ms\n",
        (duplex)(clock() - t0) * 1000.0 / (duplex)CLOCKS_PER_SEC);
    CREDO_NECESSE_NON_NIHIL(aliena);
    (vacuum)ictus(da, I, I, I);
    CREDO_NECESSE(communio_transferre(a, b, &cursor, NIHIL, piscina));
    t0 = clock();
    CREDO_VERUM(pictor_documentum_sequi(aliena));
    imprimere("  sequi ictus unius: %.1f ms\n",
        (duplex)(clock() - t0) * 1000.0 / (duplex)CLOCKS_PER_SEC);
    CREDO_VERUM(aequales(da, aliena));
    volumen_claudere(a);
    volumen_claudere(b);
}


/* ==================================================
 * plan-2 T6: per servum (hospitium + communio_servus + duo clientes in
 * reactore uno)
 * ================================================== */

nomen structura {
            Volumen* vs;
              Cives* cives;
     CommunioServus* servus;
            Reactor* reactor;
          Hospitium* hospitium;
                i32  portus;
          character  basis[LXIV];
          character  ta[XLIV];
          character  tb[XLIV];
} Orbis;

nomen structura {
    i32 vocationes;
    i32 numerus;
} Adventus;

interior vacuum
advenerunt (
    vacuum* ctx,
       i32  numerus)
{
    Adventus* a = (Adventus*)ctx;

    a->vocationes++;
    a->numerus += numerus;
}

interior b32
hospitium_aperire (
    Orbis* o)
{
    HospitiumConfiguratio cfg;

    memset(&cfg, ZEPHYRUM, magnitudo(cfg));
    cfg.reactor   = o->reactor;
    cfg.portus    = o->portus;
    o->hospitium  = hospitium_creare(piscina, &cfg);
    si (   !o->hospitium || !communio_servum_montare(o->servus,
            o->hospitium))
    {
        redde FALSUM;
    }
    o->portus = (i32)hospitium_portus(o->hospitium);
    sprintf(o->basis, "http://127.0.0.1:%d", (integer)o->portus);
    redde VERUM;
}

interior b32
apparatum_dare (
          Orbis* o,
         chorda  civis,
      character* tessera)
{
    character digestum[LXV];
    character id[MONETA_ULID_MENSURA];

    redde communio_membrum_addere(o->servus,
        chorda_ex_literis("familia",
               piscina), civis)
        && communio_tesseram_facere(tessera, digestum)
        && communio_apparatum_addere(o->servus, civis,
        chorda_ex_literis(
               "familia", piscina), chorda_ex_literis("apparatus",
               piscina), chorda_ex_literis(digestum, piscina), id);
}

interior b32
orbem_parare (
    Orbis* o)
{
    chorda primus;
    chorda amanda;

    memset(o, ZEPHYRUM, magnitudo(*o));
    o->vs = volumen_temporarium(piscina, "probatio_communio_servus");
    o->cives = o->vs ? cives_aperire(o->vs, NIHIL, piscina) : NIHIL;
    si (!o->cives || !volumen_transactionem_incipere(o->vs))
    {
        redde FALSUM;
    }
    (vacuum)cives_creare(o->cives, chorda_ex_literis("franciscus",
        piscina), chorda_ex_literis("Franciscus", piscina), &primus,
        piscina);
    (vacuum)cives_creare(o->cives, chorda_ex_literis("amanda", piscina),
        chorda_ex_literis("Amanda", piscina), &amanda, piscina);
    (vacuum)volumen_transactionem_committere(o->vs);
    o->servus = communio_servum_creare(piscina, o->vs, o->cives,
        NIHIL);
    o->reactor = reactor_creare(piscina);
    redde o->servus && o->reactor && hospitium_aperire(o)
        && apparatum_dare(o, primus, o->ta)
        && apparatum_dare(o, amanda, o->tb);
}

interior vacuum
orbem_purgare (
    Orbis* o)
{
    communio_servum_destruere(o->servus);
    si (o->hospitium)
    {
        hospitium_destruere(o->hospitium);
    }
    si (o->reactor)
    {
        reactor_destruere(o->reactor);
    }
    si (o->vs)
    {
        volumen_claudere(o->vs);
    }
}

interior CommunioCliens*
clientem (
                 Orbis* o,
               Volumen* v,
    constans character* tessera,
              Adventus* adventus)
{
    redde communio_clientem_creare(piscina, o->reactor, v, o->basis,
        chorda_ex_literis(tessera, piscina), adventus ? advenerunt
                                                      : NIHIL,
                                                      adventus);
}

/* reactorem agere donec cliens otiosus (aut tectum in ms) */
interior b32
donec_otiosus (
             Orbis* o,
    CommunioCliens* c,
               i32  tectum_ms)
{
    i32 gyri;

    per (gyri = ZEPHYRUM; !communio_cliens_otiosus(c)
         && gyri < tectum_ms / X; gyri++)
    {
        (vacuum)reactor_poll(o->reactor, X);
    }
    redde communio_cliens_otiosus(c);
}

interior vacuum
agere (
    Orbis* o,
      i32  ms)
{
    i32 gyri;

    per (gyri = ZEPHYRUM; gyri < ms / X; gyri++)
    {
        (vacuum)reactor_poll(o->reactor, X);
    }
}

/* ambo clientes ad quietem: A mittit, deinde B excitatus trahit */
interior b32
synchronizare (
             Orbis* o,
    CommunioCliens* ca,
    CommunioCliens* cb)
{
    communio_cliens_excitare(ca);
    si (!donec_otiosus(o, ca, M * V))
    {
        redde FALSUM;
    }
    communio_cliens_excitare(cb);
    redde donec_otiosus(o, cb, M * V);
}

/* acta propria non derivata voluminis */
interior i32
acta_communicanda (
    Volumen* v)
{
    Xar* l;
    i32  i;
    i32  n;

    l = volumen_acta_propria_legere(v, ZEPHYRUM, piscina);
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si (!communio_actum_derivatum((VolumenActum*)xar_obtinere(l, i),
                piscina))
        {
            n++;
        }
    }
    redde n;
}

interior s64
seq_proprium_ultimum (
    Volumen* v)
{
    Xar* l;

    l = volumen_acta_propria_legere(v, ZEPHYRUM, piscina);
    redde (l && xar_numerus(l) > ZEPHYRUM)
        ? ((VolumenActum*)xar_obtinere(l, xar_numerus(l) - I))->seq
        : ZEPHYRUM;
}

interior vacuum
_sectio_per_servum (
    CredoContextus* c)
{
                  Orbis  o;
                Volumen* va;
                Volumen* vb;
    InternamentumChorda* intern;
       PictorDocumentum* da;
       PictorDocumentum* aliena;
         CommunioCliens* ca;
         CommunioCliens* cb;
               Adventus  adventus;
                    Xar* l;
         CommunioStatus  sb;
                clock_t  t0;

    (vacuum)c;
    thema_initiare();
    memset(&adventus, ZEPHYRUM, magnitudo(adventus));
    CREDO_NECESSE(orbem_parare(&o));
    intern  = internamentum_creare(piscina);
    va      = volumen_temporarium(piscina, "probatio_communio_ca");
    vb      = volumen_temporarium(piscina, "probatio_communio_cb");
    CREDO_NECESSE(va && vb);
    da = pictor_documentum_creare(piscina, intern, va, "p", C, LX, II);
    CREDO_NECESSE(da && operationes(da, ZEPHYRUM));
    ca = clientem(&o, va, o.ta, NIHIL);
    cb = clientem(&o, vb, o.tb, &adventus);
    CREDO_NECESSE(ca && cb);
    communio_cliens_moram_ponere(ca, ZEPHYRUM);
    communio_cliens_moram_ponere(cb, ZEPHYRUM);
    communio_cliens_incipere(ca);
    communio_cliens_incipere(cb);
    t0 = clock();
    CREDO_NECESSE(synchronizare(&o, ca, cb));
    imprimere("  per servum (pictura): %.1f ms\n",
        (duplex)(clock() - t0) * 1000.0 / (duplex)CLOCKS_PER_SEC);
    CREDO_AEQUALIS_I32(communio_cliens_status(ca).error.mensura,
        ZEPHYRUM);
    CREDO_AEQUALIS_I32(communio_cliens_status(cb).error.mensura,
        ZEPHYRUM);
    CREDO_VERUM(adventus.numerus > ZEPHYRUM);
    l = pictor_documenta_aliena_enumerare(vb, piscina);
    CREDO_NECESSE(l && xar_numerus(l) == I);
    CREDO_CHORDA_AEQUALIS(((PictorPicturaAliena*)xar_obtinere(l,
        ZEPHYRUM))->replica, volumen_replica(va));
    aliena = pictor_documentum_aperire_alienum(piscina, intern, vb,
        volumen_replica(va), "p");
    CREDO_NECESSE_NON_NIHIL(aliena);
    CREDO_VERUM(aequales(da, aliena));
    CREDO_AEQUALIS_I32(acta_replicae(vb, volumen_replica(va)),
        acta_communicanda(va));
    CREDO_AEQUALIS_S64(communio_cliens_status(ca).ascensus,
        seq_proprium_ultimum(va));

    CREDO_GRADUS("cursores manent");
    sb = communio_cliens_status(cb);
    communio_clientem_destruere(cb);
    cb = clientem(&o, vb, o.tb, NIHIL);
    CREDO_NECESSE_NON_NIHIL(cb);
    CREDO_AEQUALIS_S64(communio_cliens_status(cb).descensus,
        sb.descensus);
    CREDO_AEQUALIS_S64(communio_cliens_status(cb).ascensus,
        sb.ascensus);
    communio_cliens_moram_ponere(cb, ZEPHYRUM);
    communio_cliens_incipere(cb);
    CREDO_VERUM(synchronizare(&o, ca, cb));
    CREDO_AEQUALIS_I32(communio_cliens_status(cb).recepta, ZEPHYRUM);
    communio_clientem_destruere(ca);
    communio_clientem_destruere(cb);
    volumen_claudere(va);
    volumen_claudere(vb);
    orbem_purgare(&o);
}

interior vacuum
_sectio_vivum (
    CredoContextus* c)
{
                  Orbis  o;
                Volumen* va;
                Volumen* vb;
    InternamentumChorda* intern;
       PictorDocumentum* da;
       PictorDocumentum* aliena;
         CommunioCliens* ca;
         CommunioCliens* cb;
               Adventus  adventus;
                    i32  gyri;

    (vacuum)c;
    thema_initiare();
    memset(&adventus, ZEPHYRUM, magnitudo(adventus));
    CREDO_NECESSE(orbem_parare(&o));
    intern  = internamentum_creare(piscina);
    va      = volumen_temporarium(piscina, "probatio_communio_va");
    vb      = volumen_temporarium(piscina, "probatio_communio_vb");
    CREDO_NECESSE(va && vb);
    da = pictor_documentum_creare(piscina, intern, va, "p", C, LX, II);
    CREDO_NECESSE(da && operationes(da, ZEPHYRUM));
    ca = clientem(&o, va, o.ta, NIHIL);
    cb = clientem(&o, vb, o.tb, &adventus);
    CREDO_NECESSE(ca && cb);
    communio_cliens_moram_ponere(ca, ZEPHYRUM);
    communio_cliens_incipere(ca);
    communio_cliens_incipere(cb);
    CREDO_NECESSE(donec_otiosus(&o, ca, M * V));
    per (gyri = ZEPHYRUM; adventus.vocationes == ZEPHYRUM && gyri < D;
         gyri++)
    {
        agere(&o, X);
    }
    CREDO_NECESSE(adventus.vocationes > ZEPHYRUM);
    agere(&o, C);
    aliena = pictor_documentum_aperire_alienum(piscina, intern, vb,
        volumen_replica(va), "p");
    CREDO_NECESSE_NON_NIHIL(aliena);
    CREDO_VERUM(aequales(da, aliena));

    CREDO_GRADUS("ictus novi per tractionem dilatam");
    adventus.vocationes = ZEPHYRUM;
    CREDO_NECESSE(ictus(da, LXX, XXX, IX) > ZEPHYRUM);
    CREDO_NECESSE(ictus(da, LXXX, XXX, X) > ZEPHYRUM);
    per (gyri = ZEPHYRUM; adventus.vocationes == ZEPHYRUM && gyri < CCC;
         gyri++)
    {
        agere(&o, X);
    }
    CREDO_VERUM(adventus.vocationes > ZEPHYRUM);
    CREDO_VERUM(pictor_documentum_sequi(aliena));
    CREDO_VERUM(aequales(da, aliena));
    communio_clientem_destruere(ca);
    communio_clientem_destruere(cb);
    volumen_claudere(va);
    volumen_claudere(vb);
    orbem_purgare(&o);
}

interior vacuum
_sectio_servus_cadit (
    CredoContextus* c)
{
                  Orbis  o;
                Volumen* va;
                Volumen* vb;
    InternamentumChorda* intern;
       PictorDocumentum* da;
       PictorDocumentum* aliena;
         CommunioCliens* ca;
         CommunioCliens* cb;
                    i32  k;

    (vacuum)c;
    thema_initiare();
    CREDO_NECESSE(orbem_parare(&o));
    intern  = internamentum_creare(piscina);
    va      = volumen_temporarium(piscina, "probatio_communio_xa");
    vb      = volumen_temporarium(piscina, "probatio_communio_xb");
    CREDO_NECESSE(va && vb);
    da = pictor_documentum_creare(piscina, intern, va, "p", C, LX, II);
    CREDO_NECESSE(da && operationes(da, ZEPHYRUM));
    ca = clientem(&o, va, o.ta, NIHIL);
    cb = clientem(&o, vb, o.tb, NIHIL);
    CREDO_NECESSE(ca && cb);
    communio_cliens_moram_ponere(ca, ZEPHYRUM);
    communio_cliens_moram_ponere(cb, ZEPHYRUM);
    communio_cliens_incipere(ca);
    communio_cliens_incipere(cb);
    CREDO_NECESSE(synchronizare(&o, ca, cb));

    CREDO_GRADUS("servus cadit; A pergit pingere");
    hospitium_destruere(o.hospitium);
    o.hospitium = NIHIL;
    per (k = ZEPHYRUM; k < III; k++)
    {
        CREDO_NECESSE(ictus(da, X + k * X, L, IV) > ZEPHYRUM);
    }
    communio_cliens_excitare(ca);
    agere(&o, MD);
    CREDO_VERUM(communio_cliens_status(ca).error.mensura > ZEPHYRUM);
    CREDO_FALSUM(communio_cliens_status(ca).connexus);

    CREDO_GRADUS("servus redit: nihil duplicatum, nihil amissum");
    CREDO_NECESSE(hospitium_aperire(&o));
    CREDO_NECESSE(synchronizare(&o, ca, cb));
    CREDO_AEQUALIS_I32(acta_replicae(vb, volumen_replica(va)),
        acta_communicanda(va));
    CREDO_AEQUALIS_S64(communio_cliens_status(ca).ascensus,
        seq_proprium_ultimum(va));
    CREDO_AEQUALIS_I32(communio_cliens_status(ca).error.mensura,
        ZEPHYRUM);
    aliena = pictor_documentum_aperire_alienum(piscina, intern, vb,
        volumen_replica(va), "p");
    CREDO_NECESSE_NON_NIHIL(aliena);
    CREDO_VERUM(aequales(da, aliena));
    communio_clientem_destruere(ca);
    communio_clientem_destruere(cb);
    volumen_claudere(va);
    volumen_claudere(vb);
    orbem_purgare(&o);
}

/* chorda continet subchordam? */
interior b32
continet_literas (
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
_sectio_tessera_mala (
    CredoContextus* c)
{
             Orbis  o;
           Volumen* va;
           Volumen* vb;
    CommunioCliens* ca;
    CommunioCliens* cb;
         character  falsa[XLIV];
         character  digestum[LXV];

    (vacuum)c;
    CREDO_NECESSE(orbem_parare(&o));
    va = volumen_temporarium(piscina, "probatio_communio_ma");
    vb = volumen_temporarium(piscina, "probatio_communio_mb");
    CREDO_NECESSE(va && vb);
    CREDO_NECESSE(volumen_actum_appendere(va, "x/actum",
        chorda_ex_literis("{}", piscina)) > ZEPHYRUM);
    CREDO_NECESSE(communio_tesseram_facere(falsa, digestum));
    ca = clientem(&o, va, o.ta, NIHIL);
    cb = clientem(&o, vb, falsa, NIHIL);
    CREDO_NECESSE(ca && cb);
    communio_cliens_moram_ponere(ca, ZEPHYRUM);
    communio_cliens_moram_ponere(cb, ZEPHYRUM);
    communio_cliens_incipere(ca);
    communio_cliens_incipere(cb);
    CREDO_NECESSE(donec_otiosus(&o, ca, M * V));
    communio_cliens_excitare(cb);
    agere(&o, D);
    CREDO_VERUM(communio_cliens_status(cb).error.mensura > ZEPHYRUM);
    CREDO_FALSUM(continet_literas(communio_cliens_status(cb).error,
        falsa));
    CREDO_AEQUALIS_I32(acta_replicae(vb, volumen_replica(va)),
        ZEPHYRUM);
    communio_clientem_destruere(ca);
    communio_clientem_destruere(cb);
    volumen_claudere(va);
    volumen_claudere(vb);
    orbem_purgare(&o);
}

interior s64
ms_nunc (vacuum)
{
    structura timeval tv;

    (vacuum)gettimeofday(&tv, NIHIL);
    redde (s64)tv.tv_sec * M + (s64)(tv.tv_usec / M);
}

/* mensura (nulla assertio temporis): pictura D ictuum per servum,
 * tempus murale ab incipere ad B otiosum */
interior vacuum
_sectio_mensura_per_servum (
    CredoContextus* c)
{
                  Orbis  o;
                Volumen* va;
                Volumen* vb;
    InternamentumChorda* intern;
       PictorDocumentum* da;
       PictorDocumentum* aliena;
         CommunioCliens* ca;
         CommunioCliens* cb;
                    i32  k;
                    s64  t0;

    (vacuum)c;
    thema_initiare();
    CREDO_NECESSE(orbem_parare(&o));
    intern  = internamentum_creare(piscina);
    va      = volumen_temporarium(piscina, "probatio_communio_da");
    vb      = volumen_temporarium(piscina, "probatio_communio_db");
    CREDO_NECESSE(va && vb);
    da = pictor_documentum_creare(piscina, intern, va, "p", C, LX,
        LXIV);
    CREDO_NECESSE_NON_NIHIL(da);
    per (k = ZEPHYRUM; k < D; k++)
    {
        CREDO_NECESSE(ictus(da, k % C, (k / C) % LX, k % XVI)
            > ZEPHYRUM);
    }
    ca = clientem(&o, va, o.ta, NIHIL);
    cb = clientem(&o, vb, o.tb, NIHIL);
    CREDO_NECESSE(ca && cb);
    communio_cliens_moram_ponere(ca, ZEPHYRUM);
    communio_cliens_moram_ponere(cb, ZEPHYRUM);
    t0 = ms_nunc();
    communio_cliens_incipere(ca);
    communio_cliens_incipere(cb);
    CREDO_NECESSE(synchronizare(&o, ca, cb));
    imprimere("  D ictus per servum: %ld ms (murale)\n",
        (longus)(ms_nunc() - t0));
    aliena = pictor_documentum_aperire_alienum(piscina, intern, vb,
        volumen_replica(va), "p");
    CREDO_NECESSE_NON_NIHIL(aliena);
    CREDO_VERUM(aequales(da, aliena));
    communio_clientem_destruere(ca);
    communio_clientem_destruere(cb);
    volumen_claudere(va);
    volumen_claudere(vb);
    orbem_purgare(&o);
}

/* plan-2 T7: tessera in filo 0600 - scribere numquam supra
 * exsistens, legere recusat modum latiorem */
interior vacuum
_sectio_tessera_in_filo (
    CredoContextus* c)
{
            character via[CCLVI];
            character tessera[XLIV];
            character digestum[LXV];
            character iterum[LXV];
            character moneta[MONETA_ULID_MENSURA];
             Sigillum g;
            character hex[SIGILLUM_HEX_MENSURA];
    structura stat  st;

    (vacuum)c;
    moneta_ulid(moneta);
    sprintf(via, "/tmp/probatio_communio_tessera_%s", moneta);
    CREDO_NECESSE(communio_tesseram_scribere(via, digestum));
    CREDO_AEQUALIS_I32((i32)strlen(digestum), LXIV);
    CREDO_NECESSE(lectiones_stat(via, &st) == ZEPHYRUM);
    CREDO_AEQUALIS_I32((i32)(st.st_mode & 0777), (i32)0600);

    CREDO_GRADUS("lectio: digestum congruit");
    CREDO_NECESSE(communio_tesseram_legere(via, tessera));
    CREDO_AEQUALIS_I32((i32)strlen(tessera), XLIII);
    g = sigillum_computare(tessera, (memoriae_index)strlen(tessera));
    sigillum_hex(&g, hex);
    CREDO_AEQUALIS_I32((i32)memcmp(hex, digestum, (size_t)LXIV),
        ZEPHYRUM);

    CREDO_GRADUS("numquam supra exsistens");
    CREDO_FALSUM(communio_tesseram_scribere(via, iterum));
    CREDO_NECESSE(communio_tesseram_legere(via, tessera));
    g = sigillum_computare(tessera, (memoriae_index)strlen(tessera));
    sigillum_hex(&g, hex);
    CREDO_AEQUALIS_I32((i32)memcmp(hex, digestum, (size_t)LXIV),
        ZEPHYRUM);

    CREDO_GRADUS("modus 0644 recusatur");
    CREDO_NECESSE(filum_modum_ponere(via, 0644));
    CREDO_FALSUM(communio_tesseram_legere(via, tessera));
    (vacuum)filum_delere(via);
    CREDO_FALSUM(communio_tesseram_legere(via, tessera));
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
    { "sequi (T5)", _sectio_sequi, _parare, _purgare, NIHIL, NIHIL },
    { "sequi extra ordinem (plan-2 T1)", _sectio_sequi_extra_ordinem,
        _parare, _purgare, NIHIL, NIHIL },
    { "mensura: D ictus", _sectio_mensura, _parare, _purgare, NIHIL,
        NIHIL },
    { "per servum (plan-2 T6)", _sectio_per_servum, _parare, _purgare,
        NIHIL, NIHIL },
    { "vivum: tractio dilata", _sectio_vivum, _parare, _purgare, NIHIL,
        NIHIL },
    { "servus cadit", _sectio_servus_cadit, _parare, _purgare, NIHIL,
        NIHIL },
    { "tessera mala", _sectio_tessera_mala, _parare, _purgare, NIHIL,
        NIHIL },
    { "mensura per servum: D ictus", _sectio_mensura_per_servum,
        _parare,
        _purgare, NIHIL, NIHIL },
    { "tessera in filo", _sectio_tessera_in_filo, _parare, _purgare,
        NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("communio", SECTIONES);
}
