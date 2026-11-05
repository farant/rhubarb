/* negotia.c - negotia et facultates in volumine (vide negotia.h).
 *
 * Index in memoria: negotia (Negotium), facultates (status praesens
 * + revisiones), nexus colloquiorum; ordo = Negotium* ordine
 * activitatis, post omnem lectionem refectus. Acta ex cursore
 * (navigator_bibliotheca acta_legere exemplar), sed OMNIUM
 * replicarum. Scripturae: actum appenditur, deinde ex cursore
 * legitur - status praesens semper ex actis, numquam ex argumentis
 * vocantis. */

#include "negotia.h"
#include "chorda_aedificator.h"
#include "json.h"
#include <string.h>

#define GENUS_NEGOTII       "negotium"
#define GENUS_FACULTATIS    "negotii-facultas"
#define GENUS_COLLOQUII     "negotii-colloquium"

nomen structura {
    NegotiiFacultas  praesens;
                Xar* revisiones;   /* NegotiiFacultas, vetustissima
                                   * prima */
} FacultasInterna;

nomen structura {
    chorda negotium;
    chorda sessio;
} NexusColloquii;

structura Negotia {
       Volumen* volumen;
       Piscina* piscina;
    MonetaFons  fons;
        vacuum* fontis_ctx;
           Xar* negotia;      /* Negotium */
           Xar* ordo;         /* Negotium*, activitate decrescente */
           Xar* facultates;   /* FacultasInterna, ordine creationis */
           Xar* colloquia;    /* NexusColloquii, ordine adiunctionis */
           s64  seq;          /* cursor: actum ultimum visum */
};

hic_manens constans character* constans SIGNA_STATUS[] = {
    "proposita", "inchoata", "perfecta"
};

hic_manens constans character* constans TITULI_STATUS[] = {
    "planned", "partial", "shipped"
};

interior chorda
vacua (vacuum)
{
    chorda v;

    v.datum    = NIHIL;
    v.mensura  = ZEPHYRUM;
    redde v;
}

interior b32
status_validus (
    s32 status)
{
    redde status >= NEGOTIA_PROPOSITA && status <= NEGOTIA_PERFECTA;
}


    /* ==================================================
     * Inquisitio in indice
     * ================================================== */

interior Negotium*
negotium_per_id (
    constans Negotia* n,
              chorda  id)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(n->negotia); i++)
    {
        Negotium* x = (Negotium*)xar_obtinere(n->negotia, i);

        si (chorda_aequalis(x->id, id))
        {
            redde x;
        }
    }
    redde NIHIL;
}

interior FacultasInterna*
facultatem_per_id (
    constans Negotia* n,
              chorda  id)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(n->facultates); i++)
    {
        FacultasInterna* f = (FacultasInterna*)xar_obtinere(
            n->facultates, i);

        si (chorda_aequalis(f->praesens.id, id))
        {
            redde f;
        }
    }
    redde NIHIL;
}

interior b32
nexus_exstat (
    constans Negotia* n,
              chorda  negotium,
              chorda  sessio)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(n->colloquia); i++)
    {
        constans NexusColloquii* x = (constans NexusColloquii*)
            xar_obtinere(n->colloquia, i);

        si (   chorda_aequalis(x->negotium, negotium)
            && chorda_aequalis(x->sessio, sessio))
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
activitatem_notare (
    Negotium* x,
         s64  seq)
{
    si (seq > x->seq_activitatis)
    {
        x->seq_activitatis = seq;
    }
}


    /* ==================================================
     * Acta in indicem
     * ================================================== */

interior vacuum
negotium_applicare (
          Negotia* n,
        JsonValor* radix,
              s64  seq)
{
     Negotium* x;
       chorda  id;

    id = json_capere_chorda(radix, "id", vacua());
    si (id.mensura == ZEPHYRUM)
    {
        redde;
    }
    x = negotium_per_id(n, id);
    si (!x)
    {
        x = (Negotium*)xar_addere(n->negotia);
        si (!x)
        {
            redde;
        }
        memset(x, ZEPHYRUM, magnitudo(Negotium));
        x->id = id;
    }
    x->titulus     = json_capere_chorda(radix, "titulus", vacua());
    x->descriptio  = json_capere_chorda(radix, "descriptio", vacua());
    x->seq         = seq;
    activitatem_notare(x, seq);
}

interior vacuum
facultatem_applicare (
                  Negotia* n,
                JsonValor* radix,
    constans VolumenActum* actum)
{
           Negotium* x;
    FacultasInterna* f;
    NegotiiFacultas  r;
    NegotiiFacultas* locus;
                s32  status;

    memset(&r, ZEPHYRUM, magnitudo(r));
    r.id        = json_capere_chorda(radix, "id", vacua());
    r.negotium  = json_capere_chorda(radix, "negotium", vacua());
    x           = negotium_per_id(n, r.negotium);
    si (   r.id.mensura == ZEPHYRUM || !x
        || !negotia_status_legere(json_capere_chorda(radix, "status",
               vacua()), &status))
    {
        redde;
    }
    r.titulus   = json_capere_chorda(radix, "titulus", vacua());
    r.status    = status;
    r.corpus    = json_capere_chorda(radix, "corpus", vacua());
    r.seq       = actum->seq;
    r.momentum  = actum->momentum;

    f = facultatem_per_id(n, r.id);
    si (!f)
    {
        f = (FacultasInterna*)xar_addere(n->facultates);
        si (!f)
        {
            redde;
        }
        f->revisiones = xar_creare(n->piscina,
            (i32)magnitudo(NegotiiFacultas));
    }
    alioquin si (!chorda_aequalis(f->praesens.negotium, r.negotium))
    {
        /* facultas negotium non mutat */
        redde;
    }
    f->praesens  = r;
    locus        = (NegotiiFacultas*)xar_addere(f->revisiones);
    si (locus)
    {
        *locus = r;
    }
    activitatem_notare(x, actum->seq);
}

interior vacuum
colloquium_applicare (
          Negotia* n,
        JsonValor* radix,
              s64  seq)
{
          Negotium* x;
    NexusColloquii* nexus;
            chorda  negotium;
            chorda  sessio;

    negotium  = json_capere_chorda(radix, "negotium", vacua());
    sessio    = json_capere_chorda(radix, "sessio", vacua());
    x         = negotium_per_id(n, negotium);
    si (   !x || sessio.mensura == ZEPHYRUM
        || nexus_exstat(n, negotium, sessio))
    {
        redde;
    }
    nexus = (NexusColloquii*)xar_addere(n->colloquia);
    si (!nexus)
    {
        redde;
    }
    nexus->negotium  = negotium;
    nexus->sessio    = sessio;
    activitatem_notare(x, seq);
}

interior vacuum
actum_applicare (
                  Negotia* n,
    constans VolumenActum* actum)
{
    JsonResultus r;

    r = json_legere(actum->datum, n->piscina);
    si (!r.successus || !r.radix)
    {
        redde;
    }
    si (chorda_aequalis_literis(actum->genus, GENUS_NEGOTII))
    {
        negotium_applicare(n, r.radix, actum->seq);
    }
    alioquin si (chorda_aequalis_literis(actum->genus,
                 GENUS_FACULTATIS))
    {
        facultatem_applicare(n, r.radix, actum);
    }
    alioquin si (chorda_aequalis_literis(actum->genus, GENUS_COLLOQUII))
    {
        colloquium_applicare(n, r.radix, actum->seq);
    }
}

/* ordo activitatis reficitur (insertio; negotia pauca) */
interior vacuum
ordinem_reficere (
    Negotia* n)
{
    i32 i;
    i32 j;

    n->ordo = xar_creare(n->piscina, (i32)magnitudo(Negotium*));
    per (i = ZEPHYRUM; i < xar_numerus(n->negotia); i++)
    {
        Negotium** locus = (Negotium**)xar_addere(n->ordo);

        si (!locus)
        {
            redde;
        }
        *locus = (Negotium*)xar_obtinere(n->negotia, i);
    }
    per (i = I; i < xar_numerus(n->ordo); i++)
    {
        per (j = i; j > ZEPHYRUM; j--)
        {
            Negotium** a = (Negotium**)xar_obtinere(n->ordo, j - I);
            Negotium** b = (Negotium**)xar_obtinere(n->ordo, j);
            Negotium*  t;

            si ((*a)->seq_activitatis >= (*b)->seq_activitatis)
            {
                frange;
            }
            t   = *a;
            *a  = *b;
            *b  = t;
        }
    }
}

/* acta trium generum post cursorem; VERUM si qua applicata */
interior b32
acta_legere (
    Negotia* n)
{
     constans character* genera[IV];
                    Xar* acta;
                    s64  ultimum;
                    i32  i;

    genera[ZEPHYRUM]  = GENUS_NEGOTII;
    genera[I]         = GENUS_FACULTATIS;
    genera[II]        = GENUS_COLLOQUII;
    genera[III]       = NIHIL;
    ultimum           = volumen_seq_ultimum(n->volumen);
    acta = volumen_acta_generum_legere(n->volumen, n->seq, genera,
        n->piscina);
    per (i = ZEPHYRUM; acta && i < xar_numerus(acta); i++)
    {
        constans VolumenActum* actum = (constans VolumenActum*)
            xar_obtinere(acta, i);

        si (actum->seq > n->seq)
        {
            n->seq = actum->seq;
        }
        actum_applicare(n, actum);
    }
    /* acta aliorum generum cursorem quoque promovent, ne renovare
     * ea semper quaerat */
    si (ultimum > n->seq)
    {
        n->seq = ultimum;
    }
    si (acta && xar_numerus(acta) > ZEPHYRUM)
    {
        ordinem_reficere(n);
        redde VERUM;
    }
    redde FALSUM;
}


    /* ==================================================
     * Scriptura
     * ================================================== */

interior chorda
identitatem_cudere (
    Negotia* n)
{
    character effusio[MONETA_ULID_MENSURA];

    memset(effusio, ZEPHYRUM, magnitudo(effusio));
    si (n->fons)
    {
        n->fons(n->fontis_ctx, effusio);
    }
    alioquin
    {
        moneta_ulid(effusio);
    }
    effusio[MONETA_ULID_MENSURA - I] = '\0';
    redde chorda_ex_literis(effusio, n->piscina);
}

/* "clavis":"valor" (cum comma praecedente nisi primum) */
interior vacuum
campum_scribere (
     ChordaAedificator* aed,
    constans character* clavis,
                chorda  valor,
                   b32  primum)
{
    si (!primum)
    {
        chorda_aedificator_appendere_character(aed, ',');
    }
    chorda_aedificator_appendere_character(aed, '"');
    chorda_aedificator_appendere_literis(aed, clavis);
    chorda_aedificator_appendere_literis(aed, "\":\"");
    chorda_aedificator_appendere_evasus_json(aed, valor);
    chorda_aedificator_appendere_character(aed, '"');
}

/* actum appendit et ex cursore legit; FALSUM si appendere defecit */
interior b32
actum_scribere (
                Negotia* n,
     constans character* genus,
      ChordaAedificator* aed)
{
    chorda_aedificator_appendere_character(aed, '}');
    si (volumen_actum_appendere(n->volumen, genus,
            chorda_aedificator_finire(aed)) == ZEPHYRUM)
    {
        redde FALSUM;
    }
    (vacuum)acta_legere(n);
    redde VERUM;
}

interior b32
negotium_scribere (
    Negotia* n,
     chorda  id,
     chorda  titulus,
     chorda  descriptio)
{
    ChordaAedificator* aed;

    aed = chorda_aedificator_creare(n->piscina, CCLVI);
    chorda_aedificator_appendere_character(aed, '{');
    campum_scribere(aed, "id", id, VERUM);
    campum_scribere(aed, "titulus", titulus, FALSUM);
    campum_scribere(aed, "descriptio", descriptio, FALSUM);
    redde actum_scribere(n, GENUS_NEGOTII, aed);
}

interior b32
facultatem_scribere (
    Negotia* n,
     chorda  id,
     chorda  negotium,
     chorda  titulus,
        s32  status,
     chorda  corpus)
{
    ChordaAedificator* aed;

    aed = chorda_aedificator_creare(n->piscina, CCLVI);
    chorda_aedificator_appendere_character(aed, '{');
    campum_scribere(aed, "id", id, VERUM);
    campum_scribere(aed, "negotium", negotium, FALSUM);
    campum_scribere(aed, "titulus", titulus, FALSUM);
    campum_scribere(aed, "status", chorda_ex_literis(
        SIGNA_STATUS[status], n->piscina), FALSUM);
    campum_scribere(aed, "corpus", corpus, FALSUM);
    redde actum_scribere(n, GENUS_FACULTATIS, aed);
}


    /* ==================================================
     * Interfacies
     * ================================================== */

Negotia*
negotia_aperire (
       Volumen* volumen,
    MonetaFons  fons,
        vacuum* fontis_ctx,
       Piscina* piscina)
{
    Negotia* n;

    si (!volumen || !piscina)
    {
        redde NIHIL;
    }
    n = (Negotia*)piscina_allocare(piscina, magnitudo(Negotia));
    si (!n)
    {
        redde NIHIL;
    }
    memset(n, ZEPHYRUM, magnitudo(Negotia));
    n->volumen     = volumen;
    n->piscina     = piscina;
    n->fons        = fons;
    n->fontis_ctx  = fontis_ctx;
    n->negotia     = xar_creare(piscina, (i32)magnitudo(Negotium));
    n->facultates  = xar_creare(piscina,
        (i32)magnitudo(FacultasInterna));
    n->colloquia   = xar_creare(piscina,
        (i32)magnitudo(NexusColloquii));
    n->ordo        = xar_creare(piscina, (i32)magnitudo(Negotium*));
    (vacuum)acta_legere(n);
    redde n;
}

b32
negotia_renovare (
    Negotia* n)
{
    si (!n || volumen_seq_ultimum(n->volumen) <= n->seq)
    {
        redde FALSUM;
    }
    redde acta_legere(n);
}

s64
negotia_versio (
    constans Negotia* n)
{
    redde n ? n->seq : ZEPHYRUM;
}

i32
negotia_numerus (
    constans Negotia* n)
{
    redde n ? xar_numerus(n->ordo) : ZEPHYRUM;
}

constans Negotium*
negotia_index (
    constans Negotia* n,
                 i32  k)
{
    si (!n || k >= xar_numerus(n->ordo))
    {
        redde NIHIL;
    }
    redde *(Negotium* constans*)xar_obtinere(n->ordo, k);
}

constans Negotium*
negotia_negotium_invenire (
    constans Negotia* n,
              chorda  id,
                 b32* ambiguum_out)
{
     constans Negotium* inventum;
                   i32  congruentes;
                   i32  i;

    si (ambiguum_out)
    {
        *ambiguum_out = FALSUM;
    }
    si (!n || id.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    inventum = negotium_per_id(n, id);
    si (inventum)
    {
        redde inventum;
    }
    congruentes = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(n->negotia); i++)
    {
        constans Negotium* x = (constans Negotium*)xar_obtinere(
            n->negotia, i);

        si (chorda_terminatur(x->id, id))
        {
            inventum = x;
            congruentes++;
        }
    }
    si (congruentes > I)
    {
        si (ambiguum_out)
        {
            *ambiguum_out = VERUM;
        }
        redde NIHIL;
    }
    redde inventum;
}

chorda
negotia_negotium_creare (
    Negotia* n,
     chorda  titulus,
     chorda  descriptio)
{
    chorda id;

    si (!n || titulus.mensura == ZEPHYRUM)
    {
        redde vacua();
    }
    id = identitatem_cudere(n);
    si (!negotium_scribere(n, id, titulus, descriptio))
    {
        redde vacua();
    }
    redde id;
}

b32
negotia_negotium_mutare (
    Negotia* n,
     chorda  id,
     chorda  titulus,
     chorda  descriptio)
{
    Negotium* x;

    si (!n || titulus.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    x = negotium_per_id(n, id);
    si (!x)
    {
        redde FALSUM;
    }
    si (   chorda_aequalis(x->titulus, titulus)
        && chorda_aequalis(x->descriptio, descriptio))
    {
        redde VERUM;
    }
    redde negotium_scribere(n, x->id, titulus, descriptio);
}

Xar*
negotia_facultates (
    constans Negotia* n,
              chorda  negotium,
             Piscina* piscina)
{
    Xar* x;
    i32  i;

    si (!n || !piscina)
    {
        redde NIHIL;
    }
    x = xar_creare(piscina, (i32)magnitudo(NegotiiFacultas));
    per (i = ZEPHYRUM; x && i < xar_numerus(n->facultates); i++)
    {
        constans FacultasInterna* f = (constans FacultasInterna*)
            xar_obtinere(n->facultates, i);
        NegotiiFacultas* locus;

        si (!chorda_aequalis(f->praesens.negotium, negotium))
        {
            perge;
        }
        locus = (NegotiiFacultas*)xar_addere(x);
        si (locus)
        {
            *locus = f->praesens;
        }
    }
    redde x;
}

constans NegotiiFacultas*
negotia_facultatem_invenire (
    constans Negotia* n,
              chorda  id,
                 b32* ambiguum_out)
{
    constans FacultasInterna* inventa;
                         i32  congruentes;
                         i32  i;

    si (ambiguum_out)
    {
        *ambiguum_out = FALSUM;
    }
    si (!n || id.mensura == ZEPHYRUM)
    {
        redde NIHIL;
    }
    inventa = facultatem_per_id(n, id);
    si (inventa)
    {
        redde &inventa->praesens;
    }
    congruentes = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(n->facultates); i++)
    {
        constans FacultasInterna* f = (constans FacultasInterna*)
            xar_obtinere(n->facultates, i);

        si (chorda_terminatur(f->praesens.id, id))
        {
            inventa = f;
            congruentes++;
        }
    }
    si (congruentes > I)
    {
        si (ambiguum_out)
        {
            *ambiguum_out = VERUM;
        }
        redde NIHIL;
    }
    redde inventa ? &inventa->praesens : NIHIL;
}

chorda
negotia_facultatem_creare (
    Negotia* n,
     chorda  negotium,
     chorda  titulus,
        s32  status,
     chorda  corpus)
{
     Negotium* x;
       chorda  id;

    si (!n || titulus.mensura == ZEPHYRUM || !status_validus(status))
    {
        redde vacua();
    }
    x = negotium_per_id(n, negotium);
    si (!x)
    {
        redde vacua();
    }
    id = identitatem_cudere(n);
    si (!facultatem_scribere(n, id, x->id, titulus, status, corpus))
    {
        redde vacua();
    }
    redde id;
}

b32
negotia_facultatem_mutare (
    Negotia* n,
     chorda  id,
     chorda  titulus,
        s32  status,
     chorda  corpus)
{
    FacultasInterna* f;

    si (!n || titulus.mensura == ZEPHYRUM || !status_validus(status))
    {
        redde FALSUM;
    }
    f = facultatem_per_id(n, id);
    si (!f)
    {
        redde FALSUM;
    }
    si (   chorda_aequalis(f->praesens.titulus, titulus)
        && f->praesens.status == status
        && chorda_aequalis(f->praesens.corpus, corpus))
    {
        redde VERUM;
    }
    redde facultatem_scribere(n, f->praesens.id, f->praesens.negotium,
        titulus, status, corpus);
}

Xar*
negotia_revisiones (
    constans Negotia* n,
              chorda  facultas,
             Piscina* piscina)
{
    constans FacultasInterna* f;
                         Xar* x;
                         i32  i;

    si (!n || !piscina)
    {
        redde NIHIL;
    }
    f = facultatem_per_id(n, facultas);
    si (!f)
    {
        redde NIHIL;
    }
    x = xar_creare(piscina, (i32)magnitudo(NegotiiFacultas));
    per (i = ZEPHYRUM; x && i < xar_numerus(f->revisiones); i++)
    {
        NegotiiFacultas* locus = (NegotiiFacultas*)xar_addere(x);

        si (locus)
        {
            *locus = *(constans NegotiiFacultas*)xar_obtinere(
                f->revisiones, i);
        }
    }
    redde x;
}

b32
negotia_colloquium_adiungere (
    Negotia* n,
     chorda  negotium,
     chorda  sessio)
{
             Negotium* x;
    ChordaAedificator* aed;

    si (!n || sessio.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    x = negotium_per_id(n, negotium);
    si (!x)
    {
        redde FALSUM;
    }
    si (nexus_exstat(n, x->id, sessio))
    {
        redde VERUM;
    }
    aed = chorda_aedificator_creare(n->piscina, CXXVIII);
    chorda_aedificator_appendere_character(aed, '{');
    campum_scribere(aed, "negotium", x->id, VERUM);
    campum_scribere(aed, "sessio", sessio, FALSUM);
    redde actum_scribere(n, GENUS_COLLOQUII, aed);
}

Xar*
negotia_colloquia (
    constans Negotia* n,
              chorda  negotium,
             Piscina* piscina)
{
    Xar* x;
    i32  i;

    si (!n || !piscina)
    {
        redde NIHIL;
    }
    x = xar_creare(piscina, (i32)magnitudo(chorda));
    per (i = ZEPHYRUM; x && i < xar_numerus(n->colloquia); i++)
    {
        constans NexusColloquii* nexus = (constans NexusColloquii*)
            xar_obtinere(n->colloquia, i);
        chorda* locus;

        si (!chorda_aequalis(nexus->negotium, negotium))
        {
            perge;
        }
        locus = (chorda*)xar_addere(x);
        si (locus)
        {
            *locus = nexus->sessio;
        }
    }
    redde x;
}

chorda
negotia_summarium (
    constans Negotia* n,
              chorda  negotium,
             Piscina* piscina)
{
    ChordaAedificator* a;
                  Xar* fs;
                  Xar* cs;
                  i32  numeri[III];
                  i32  colloquia;
                  s32  st;
                  i32  i;
                  b32  primum = VERUM;

    si (!n || !piscina || !negotium_per_id(n, negotium))
    {
        redde chorda_ex_literis("", piscina);
    }
    memset(numeri, ZEPHYRUM, magnitudo(numeri));
    fs = negotia_facultates(n, negotium, piscina);
    per (i = ZEPHYRUM; fs && i < xar_numerus(fs); i++)
    {
        st = ((constans NegotiiFacultas*)xar_obtinere(fs, i))->status;
        si (status_validus(st))
        {
            numeri[st]++;
        }
    }
    cs         = negotia_colloquia(n, negotium, piscina);
    colloquia  = cs ? xar_numerus(cs) : ZEPHYRUM;
    a          = chorda_aedificator_creare(piscina, LXIV);
    /* ordo: perfectae, inchoatae, propositae, colloquia */
    per (st = (s32)NEGOTIA_PERFECTA; st >= ZEPHYRUM; st--)
    {
        si (numeri[st] > ZEPHYRUM)
        {
            si (!primum)
            {
                (vacuum)chorda_aedificator_appendere_literis(a,
                    " \302\267 ");
            }
            (vacuum)chorda_aedificator_appendere_i32(a, numeri[st]);
            (vacuum)chorda_aedificator_appendere_character(a, ' ');
            (vacuum)chorda_aedificator_appendere_literis(a,
                negotia_status_titulus(st));
            primum = FALSUM;
        }
    }
    si (colloquia > ZEPHYRUM)
    {
        si (!primum)
        {
            (vacuum)chorda_aedificator_appendere_literis(a,
                " \302\267 ");
        }
        (vacuum)chorda_aedificator_appendere_i32(a, colloquia);
        (vacuum)chorda_aedificator_appendere_literis(a, colloquia == I
            ? " conversation" : " conversations");
    }
    redde chorda_aedificator_spectare(a);
}

constans character*
negotia_status_titulus (
    s32 status)
{
    redde status_validus(status) ? TITULI_STATUS[status] : "?";
}

b32
negotia_status_legere (
    chorda  verbum,
       s32* status_out)
{
    s32 s;

    per (s = NEGOTIA_PROPOSITA; s <= NEGOTIA_PERFECTA; s++)
    {
        si (   chorda_aequalis_literis(verbum, SIGNA_STATUS[s])
            || chorda_aequalis_literis(verbum, TITULI_STATUS[s]))
        {
            si (status_out)
            {
                *status_out = s;
            }
            redde VERUM;
        }
    }
    redde FALSUM;
}
