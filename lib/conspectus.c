/* conspectus.c - CONSPECTUS: exemplaria visus ex actis voluminis
 * derivata (project-specs/proiectiones.md, D4a). Vide conspectus.h.
 *
 * Structura: registrum tenet declarationes (cum fontibus), tabulam
 * inscriptionum ('nomen/clavis' -> Introitus), cursorem seq, acervum
 * computationum (dependentiae, circulus) et piscinam temporariam
 * (vacatur post omnem renovationem). Introitus piscinam PROPRIAM
 * habet: recomputatio aut eiectio eam destruit. Memoria per-rem
 * (id -> friatio vestigii + claves) in piscina fontis propria,
 * omni differentia ex integro aedificata - non crescit cum actis. */

#include "conspectus.h"
#include "conspectus_canon.h"
#include "stml.h"
#include "canon.h"
#include "organum.h"
#include "json.h"
#include "scrinium.h"
#include "tabula_dispersa.h"

#include <stdio.h>
#include <string.h>

#define CONSPECTUS_TECTUM_ORDINARIUM  (IV * MXXIV * MXXIV)
#define CONSPECTUS_ALVEUS             MXXIV

nomen enumeratio {
    FONS_DOCUMENTUM = I,
    FONS_SPATIUM,
    FONS_PURA
} FonsGenus;

/* res memorata fontis per-rem */
nomen structura {
    chorda  id;
       i64  friatio;    /* vestigii (FNV-1a LXIV) */
       Xar* claves;     /* chorda, in piscina fontis */
} RemMemorata;

nomen structura {
          FonsGenus genus;
                b32 omnia;          /* documentum: irritat omnia */
             chorda spatium;        /* documentum: spatium; spatium:
                                      * praefixum ante {clavis} */
              chorda  suffixum;       /* spatium: post {clavis} */
                 b32  per_rem_positum;
    ConspectusPerRem  per_rem;
             Piscina* memoria;        /* per-rem: res memoratae */
      TabulaDispersa* res;            /* id -> RemMemorata* */
                 Xar* ordo_rerum;     /* RemMemorata* */
                 b32  tactus;         /* in renovatione */
} Fons;

nomen structura Declaratio Declaratio;
nomen structura Introitus Introitus;

structura Declaratio {
             chorda  titulus;
                s32  versio;
             chorda  typus;
                Xar* fontes;          /* Fons */
                b32  opera_posita;
    ConspectusOpera  opera;
                Xar* introitus;       /* Introitus* */
};

structura Introitus {
          Declaratio* declaratio;
              chorda  clavis;
              chorda  inscriptio;
    ConspectusStatus  status;
             Piscina* piscina;
              vacuum* valor;
      memoriae_index  usus;
                 s64  seq;
                 i64  ultimum;         /* lectio ultima (LRU) */
                 Xar* dependentes;     /* Introitus* legentes */
                 b32  in_computatione;
};

nomen structura {
    chorda  spatium;
    vacuum* documentum;
} DocumentumLigatum;

structura ConspectusRegistrum {
                 Piscina* piscina;
     InternamentumChorda* intern;
                 Volumen* volumen;
          memoriae_index  tectum;
                     Xar* declarationes;  /* Declaratio* */
                     Xar* opera_orba;     /* chorda: orba */
                     Xar* documenta;      /* DocumentumLigatum */
                     Xar* omnes;          /* Introitus* */
          TabulaDispersa* tabula;         /* inscriptio -> Introitus* */
                     Xar* acervus;        /* Introitus* computantes */
                 Piscina* tempus;
                     b32  paratum;
                     s64  cursor;
          memoriae_index  usus;
                     i64  horologium;
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
vacua (vacuum)
{
    chorda c;

    c.mensura  = ZEPHYRUM;
    c.datum    = NIHIL;
    redde c;
}

interior chorda
attributum (
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* a;

    a = stml_attributum_capere(nodus, titulus);
    redde a ? *a : vacua();
}

interior vacuum
vitium_addere (
                   Xar* vitia,
               Piscina* piscina,
    constans character* nuntius)
{
    *(chorda*)xar_addere(vitia) = chorda_ex_literis(nuntius, piscina);
}

/* FNV-1a LXIV bitorum (chorda_friare XXXII tantum: collisio =
 * irritatio omissa) */
interior i64
friatio_longa (
    chorda c)
{
    i64 h;
    i32 i;

    h = (i64)0xcbf29ce4UL;
    h = (h << XVI << XVI) | (i64)0x84222325UL;
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        h ^= (i64)(insignatus character)c.datum[i];
        h *= ((i64)0x100UL << XVI << XVI) | (i64)0x1b3UL;
    }
    redde h;
}

interior Declaratio*
declaratio_quaerere (
    constans ConspectusRegistrum* r,
                          chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(r->declarationes); i++)
    {
        Declaratio* d = *(Declaratio**)xar_obtinere(r->declarationes,
            i);

        si (chorda_aequalis(d->titulus, titulus))
        {
            redde d;
        }
    }
    redde NIHIL;
}

interior vacuum*
documentum_ligatum (
    constans ConspectusRegistrum* r,
                          chorda  spatium)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(r->documenta); i++)
    {
        DocumentumLigatum* l = (DocumentumLigatum*)xar_obtinere(
            r->documenta, i);

        si (chorda_aequalis(l->spatium, spatium))
        {
            redde l->documentum;
        }
    }
    redde NIHIL;
}

/* piscina temporaria NOVA post opus magnum (acta lecta, differentia):
 * piscina_vacare omnes alveos memset - capacitas semel aucta (acta
 * multa simul) omnem lectionem posteriorem gravaret (mensuratum D4b:
 * ~XXX us per lectionem post V milia actorum). Via brevis (nihil
 * novum) piscinam parvam tantum vacat. */
interior vacuum
tempus_renovare (
    ConspectusRegistrum* r)
{
    Piscina* nova;

    nova = piscina_generare_dynamicum("conspectus_tempus", IV * MXXIV);
    si (!nova)
    {
        piscina_vacare(r->tempus);
        redde;
    }
    piscina_destruere(r->tempus);
    r->tempus = nova;
}


/* ==================================================
 * Irritatio
 * ================================================== */

interior vacuum
introitum_irritare (
    Introitus* e)
{
    i32 i;

    si (e->status != CONSPECTUS_RECENS)
    {
        redde;
    }
    e->status = CONSPECTUS_STALUM;
    per (i = ZEPHYRUM; e->dependentes
        && i < xar_numerus(e->dependentes);
         i++)
    {
        introitum_irritare(*(Introitus**)xar_obtinere(e->dependentes,
            i));
    }
}

interior vacuum
clavem_irritare (
    ConspectusRegistrum* r,
             Declaratio* d,
                 chorda  clavis)
{
       chorda  inscriptio;
       vacuum* valor;
    character  buffer[CCLVI];

    si (d->titulus.mensura + clavis.mensura + II
        > (i32)magnitudo(buffer))
    {
        redde;
    }
    sprintf(buffer, "%.*s/%.*s", (integer)d->titulus.mensura,
        (constans character*)d->titulus.datum, (integer)clavis.mensura,
        (constans character*)clavis.datum);
    inscriptio = chorda_ex_literis(buffer, r->tempus);
    si (tabula_dispersa_invenire(r->tabula, inscriptio, &valor))
    {
        introitum_irritare((Introitus*)valor);
    }
}

interior vacuum
declarationem_irritare (
    Declaratio* d)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(d->introitus); i++)
    {
        introitum_irritare(*(Introitus**)xar_obtinere(d->introitus, i));
    }
}

interior vacuum
claves_irritare (
    ConspectusRegistrum* r,
             Declaratio* d,
                    Xar* claves)
{
    i32 i;

    per (i = ZEPHYRUM; claves && i < xar_numerus(claves); i++)
    {
        clavem_irritare(r, d, *(chorda*)xar_obtinere(claves, i));
    }
}

interior b32
claves_aequales (
    Xar* a,
    Xar* b)
{
    i32 i;

    si (xar_numerus(a) != xar_numerus(b))
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < xar_numerus(a); i++)
    {
        si (!chorda_aequalis(*(chorda*)xar_obtinere(a, i),
            *(chorda*)xar_obtinere(b, i)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* per-rem: res omnes iterum enumeratae; vestigium aut claves
 * mutatae -> claves veteres et novae irritantur; res abiens ->
 * veteres. Memoria nova in piscina nova, vetus destruitur.
 * irritare FALSUM = basis sola (parare). */
interior vacuum
differentiam_facere (
     ConspectusRegistrum* r,
              Declaratio* d,
                    Fons* f,
                     b32  irritare)
{
            vacuum* documentum;
           Piscina* nova;
    TabulaDispersa* res;
               Xar* ordo;
               i32  n;
               i32  i;

    documentum = documentum_ligatum(r, f->spatium);
    si (!documentum)
    {
        redde;
    }
    n    = f->per_rem.numerus(documentum);
    nova = piscina_generare_dynamicum("conspectus_res",
        CONSPECTUS_ALVEUS);
    si (!nova)
    {
        redde;
    }
    res   = tabula_dispersa_creare_chorda(nova, n * II + XVI);
    ordo  = xar_creare(nova, (i32)magnitudo(RemMemorata*));
    per (i = ZEPHYRUM; i < n; i++)
    {
        RemMemorata* rm;
        RemMemorata* vetus;
             vacuum* inventum;
             chorda  id;
                Xar* claves;
                i32  k;

        id      = f->per_rem.id(documentum, i, r->tempus);
        claves  = xar_creare(r->tempus, (i32)magnitudo(chorda));
        f->per_rem.claves(f->per_rem.ctx, documentum, i, claves,
            r->tempus);
        rm = (RemMemorata*)piscina_allocare(nova,
            magnitudo(RemMemorata));
        rm->id = chorda_transcribere(id, nova);
        rm->friatio = friatio_longa(f->per_rem.vestigium(documentum, i,
            r->tempus));
        rm->claves  = xar_creare(nova, (i32)magnitudo(chorda));
        per (k = ZEPHYRUM; k < xar_numerus(claves); k++)
        {
            *(chorda*)xar_addere(rm->claves) = chorda_transcribere(
                *(chorda*)xar_obtinere(claves, k), nova);
        }
        (vacuum)tabula_dispersa_inserere(res, rm->id, rm);
        *(RemMemorata**)xar_addere(ordo) = rm;
        si (!irritare)
        {
            perge;
        }
        vetus = (f->res && tabula_dispersa_invenire(f->res, rm->id,
            &inventum)) ? (RemMemorata*)inventum : NIHIL;
        si (!vetus)
        {
            claves_irritare(r, d, rm->claves);
        }
        alioquin si (   vetus->friatio != rm->friatio
                     || !claves_aequales(vetus->claves, rm->claves))
        {
            claves_irritare(r, d, vetus->claves);
            claves_irritare(r, d, rm->claves);
        }
    }
    /* res abeuntes (deletae, aut revocatione sublatae) */
    per (i = ZEPHYRUM; irritare && f->ordo_rerum
         && i < xar_numerus(f->ordo_rerum); i++)
    {
        RemMemorata* vetus = *(RemMemorata**)xar_obtinere(f->ordo_rerum,
            i);

        si (!tabula_dispersa_continet(res, vetus->id))
        {
            claves_irritare(r, d, vetus->claves);
        }
    }
    si (f->memoria)
    {
        piscina_destruere(f->memoria);
    }
    f->memoria     = nova;
    f->res         = res;
    f->ordo_rerum  = ordo;
}

/* via plagulae ex datum actus 'plagula-condita'/'plagula-remota' */
interior chorda
via_plagulae (
     chorda  datum,
    Piscina* piscina)
{
    JsonResultus  lectum;
       JsonValor* v;

    lectum = json_legere(datum, piscina);
    si (!lectum.successus)
    {
        redde vacua();
    }
    v = json_objectum_capere(lectum.radix, "via");
    si (!v || !json_est_chorda(v))
    {
        redde vacua();
    }
    redde json_ad_chorda(v);
}

/* via sub spatio? ("agenda" -> "agenda/...") */
interior b32
sub_spatio (
    chorda via,
    chorda spatium)
{
    redde    spatium.mensura > ZEPHYRUM
          && via.mensura > spatium.mensura
          && memcmp(via.datum, spatium.datum, (size_t)spatium.mensura)
             == ZEPHYRUM
          && via.datum[spatium.mensura] == '/';
}

/* fons spatium: clavis ex via (praefixum + clavis + suffixum + '/'
 * aut finis); vacua si non congruit */
interior chorda
clavis_ex_spatio (
    chorda  via,
      Fons* f)
{
    chorda reliquum;
       i32 i;

    si (   via.mensura <= f->spatium.mensura
        || memcmp(via.datum, f->spatium.datum,
               (size_t)f->spatium.mensura) != ZEPHYRUM)
    {
        redde vacua();
    }
    reliquum.datum    = via.datum + f->spatium.mensura;
    reliquum.mensura  = via.mensura - f->spatium.mensura;
    si (f->suffixum.mensura == ZEPHYRUM)
    {
        i = ZEPHYRUM;
        dum (i < reliquum.mensura && reliquum.datum[i] != '/')
        {
            i++;
        }
        reliquum.mensura = i;
        redde reliquum;
    }
    per (i = ZEPHYRUM; i + f->suffixum.mensura <= reliquum.mensura; i++)
    {
        si (   memcmp(reliquum.datum + i, f->suffixum.datum,
                (size_t)f->suffixum.mensura) == ZEPHYRUM
            && (   i + f->suffixum.mensura == reliquum.mensura
                || reliquum.datum[i + f->suffixum.mensura] == '/'))
        {
            reliquum.mensura = i;
            redde reliquum;
        }
    }
    redde vacua();
}


/* ==================================================
 * Constructio
 * ================================================== */

ConspectusRegistrum*
conspectus_registrum_creare (
                Piscina* piscina,
    InternamentumChorda* intern,
                Volumen* volumen,
         memoriae_index  tectum)
{
    ConspectusRegistrum* r;

    si (!piscina || !intern || !volumen)
    {
        redde NIHIL;
    }
    r = (ConspectusRegistrum*)piscina_allocare(piscina,
        magnitudo(ConspectusRegistrum));
    memset(r, ZEPHYRUM, magnitudo(ConspectusRegistrum));
    r->piscina = piscina;
    r->intern = intern;
    r->volumen = volumen;
    r->tectum = tectum ? tectum : CONSPECTUS_TECTUM_ORDINARIUM;
    r->declarationes = xar_creare(piscina, (i32)magnitudo(Declaratio*));
    r->opera_orba = xar_creare(piscina, (i32)magnitudo(chorda));
    r->documenta     = xar_creare(piscina,
        (i32)magnitudo(DocumentumLigatum));
    r->omnes    = xar_creare(piscina, (i32)magnitudo(Introitus*));
    r->acervus  = xar_creare(piscina, (i32)magnitudo(Introitus*));
    r->tabula   = tabula_dispersa_creare_chorda(piscina, LXIV);
    r->tempus        = piscina_generare_dynamicum("conspectus_tempus",
        IV * MXXIV);
    si (!r->tempus || !r->tabula)
    {
        redde NIHIL;
    }
    r->cursor = volumen_seq_ultimum(volumen);
    redde r;
}

interior vacuum
introitum_liberare (
    ConspectusRegistrum* r,
              Introitus* e);

vacuum
conspectus_registrum_destruere (
    ConspectusRegistrum* r)
{
    Declaratio* d;
          Fons* f;
           i32  i;
           i32  k;

    si (!r || !r->tempus)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(r->omnes); i++)
    {
        introitum_liberare(r, *(Introitus**)xar_obtinere(r->omnes, i));
    }
    per (i = ZEPHYRUM; i < xar_numerus(r->declarationes); i++)
    {
        d = *(Declaratio**)xar_obtinere(r->declarationes, i);
        per (k = ZEPHYRUM; k < xar_numerus(d->fontes); k++)
        {
            f = (Fons*)xar_obtinere(d->fontes, k);
            si (f->memoria)
            {
                piscina_destruere(f->memoria);
            }
            f->memoria     = NIHIL;
            f->res         = NIHIL;
            f->ordo_rerum  = NIHIL;
        }
    }
    piscina_destruere(r->tempus);
    r->tempus   = NIHIL;
    r->paratum  = FALSUM;
}

/* canon + leges oneratoris; vitia nominata */
interior b32
canone_iudicare (
    ConspectusRegistrum* r,
              StmlNodus* radix,
                    Xar* vitia)
{
        Canon* canon;
          Xar* cv;
       chorda  causa;
          i32  i;
    character  buffer[DXII];

    causa = vacua();
    canon = canon_legere(conspectus_canon_textus(r->piscina),
        r->piscina,
        r->intern, &causa);
    si (!canon)
    {
        sprintf(buffer, "canon conspectus infixus fractus: %.*s",
            (integer)causa.mensura, (constans character*)causa.datum);
        vitium_addere(vitia, r->piscina, buffer);
        redde FALSUM;
    }
    cv = canon_iudicare(canon, radix, r->piscina);
    per (i = ZEPHYRUM; i < xar_numerus(cv); i++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(cv, i);

        sprintf(buffer, "linea %d: <%.*s> %s%s%.*s",
            (integer)(v->nodus ? v->nodus->linea : ZEPHYRUM),
            (integer)(v->elementum ? v->elementum->mensura : ZEPHYRUM),
            v->elementum ? (constans character*)v->elementum->datum
                         : "",
            canon_nuntius(v->genus),
            v->detail ? ": " : "",
            (integer)(v->detail ? v->detail->mensura : ZEPHYRUM),
            v->detail ? (constans character*)v->detail->datum : "");
        vitium_addere(vitia, r->piscina, buffer);
    }
    redde xar_numerus(cv) == ZEPHYRUM;
}

b32
conspectus_declarare (
    ConspectusRegistrum*  repertorium,
                 chorda   fons,
                    Xar** vitia)
{
    ConspectusRegistrum* r;
           StmlResultus  lectum;
              StmlNodus* radix;
             Declaratio* d;
                    Xar* v;
                    Xar* fontes;
                    i32  i;
              character  buffer[DXII];

    r = repertorium;
    si (!r)
    {
        redde FALSUM;
    }
    v = xar_creare(r->piscina, (i32)magnitudo(chorda));
    si (vitia)
    {
        *vitia = v;
    }
    /* fons copiatur: attributa in eum spectare possunt, vocans textum
     * brevis vitae dare licet (declaratio infixa, D4b) */
    fons    = chorda_transcribere(fons, r->piscina);
    lectum  = stml_legere(fons, r->piscina, r->intern);
    si (!lectum.successus || !lectum.elementum_radix)
    {
        sprintf(buffer, "linea %d: STML non legitur: %.*s",
            (integer)lectum.linea_erroris,
            (integer)lectum.error.mensura,
            (constans character*)lectum.error.datum);
        vitium_addere(v, r->piscina, buffer);
        redde FALSUM;
    }
    si (!canone_iudicare(r, lectum.radix, v))
    {
        redde FALSUM;
    }
    radix = lectum.elementum_radix;
    d = (Declaratio*)piscina_allocare(r->piscina,
        magnitudo(Declaratio));
    memset(d, ZEPHYRUM, magnitudo(Declaratio));
    d->titulus   = attributum(radix, "nomen");
    d->typus     = attributum(stml_invenire_liberum(radix, "clavis"),
        "typus");
    d->fontes     = xar_creare(r->piscina, (i32)magnitudo(Fons));
    d->introitus  = xar_creare(r->piscina, (i32)magnitudo(Introitus*));
    {
        chorda versio = attributum(radix, "versio");

        d->versio = ZEPHYRUM;
        per (i = ZEPHYRUM; i < versio.mensura; i++)
        {
            d->versio = d->versio * X + (s32)(versio.datum[i] - '0');
        }
    }
    si (declaratio_quaerere(r, d->titulus))
    {
        sprintf(buffer, "conspectus '%.*s' bis declaratus",
            (integer)d->titulus.mensura,
            (constans character*)d->titulus.datum);
        vitium_addere(v, r->piscina, buffer);
        redde FALSUM;
    }
    fontes = stml_invenire_omnes_liberos(radix, "fons", r->piscina);
    per (i = ZEPHYRUM; fontes && i < xar_numerus(fontes); i++)
    {
        StmlNodus* n           = *(StmlNodus**)xar_obtinere(fontes, i);
           chorda  documentum  = attributum(n, "documentum");
           chorda  spatium     = attributum(n, "spatium");
           chorda  pura        = attributum(n, "pura");
           chorda  irritat     = attributum(n, "irritat");
              i32  genera;
             Fons* f;

        genera = (documentum.mensura > ZEPHYRUM ? I : ZEPHYRUM)
               + (spatium.mensura > ZEPHYRUM ? I : ZEPHYRUM)
               + (pura.mensura > ZEPHYRUM ? I : ZEPHYRUM);
        si (genera != I)
        {
            sprintf(buffer, "linea %d: fons genere uno: documentum, "
                "spatium aut pura", (integer)n->linea);
            vitium_addere(v, r->piscina, buffer);
            perge;
        }
        si (   documentum.mensura > ZEPHYRUM
            && irritat.mensura == ZEPHYRUM)
        {
            sprintf(buffer, "linea %d: fons documentum irritat poscit "
                "(omnia aut per-rem)", (integer)n->linea);
            vitium_addere(v, r->piscina, buffer);
            perge;
        }
        si (   documentum.mensura == ZEPHYRUM
            && irritat.mensura > ZEPHYRUM)
        {
            sprintf(buffer, "linea %d: irritat solum cum documento",
                (integer)n->linea);
            vitium_addere(v, r->piscina, buffer);
            perge;
        }
        f = (Fons*)xar_addere(d->fontes);
        memset(f, ZEPHYRUM, magnitudo(Fons));
        si (documentum.mensura > ZEPHYRUM)
        {
            f->genus    = FONS_DOCUMENTUM;
            f->spatium  = documentum;
            f->omnia    = chorda_aequalis_literis(irritat, "omnia");
        }
        alioquin si (pura.mensura > ZEPHYRUM)
        {
            f->genus = FONS_PURA;
        }
        alioquin
        {
            chorda signum  = chorda_ex_literis("{clavis}", r->piscina);
               s32 locus   = chorda_invenire_index(spatium, signum);

            si (   locus < ZEPHYRUM
                || chorda_numerare_occurrentia(spatium, signum) != I)
            {
                sprintf(buffer, "linea %d: spatium '{clavis}' semel "
                    "continere debet", (integer)n->linea);
                vitium_addere(v, r->piscina, buffer);
                perge;
            }
            f->genus            = FONS_SPATIUM;
            f->spatium.datum    = spatium.datum;
            f->spatium.mensura  = (i32)locus;
            f->suffixum.datum = spatium.datum + locus
                + signum.mensura;
            f->suffixum.mensura = spatium.mensura - (i32)locus
                                - signum.mensura;
        }
    }
    si (xar_numerus(v) > ZEPHYRUM)
    {
        redde FALSUM;
    }
    *(Declaratio**)xar_addere(r->declarationes) = d;
    /* opera ante declarationem posita */
    per (i = ZEPHYRUM; i < xar_numerus(r->opera_orba); i++)
    {
        si (chorda_aequalis(*(chorda*)xar_obtinere(r->opera_orba, i),
            d->titulus))
        {
            *(chorda*)xar_obtinere(r->opera_orba, i) = vacua();
        }
    }
    redde VERUM;
}

b32
conspectus_opera_ponere (
     ConspectusRegistrum* repertorium,
      constans character* titulus,
         ConspectusOpera  opera)
{
    Declaratio* d;

    si (   !repertorium || !titulus || !opera.computare
        || !opera.codificare)
    {
        redde FALSUM;
    }
    d = declaratio_quaerere(repertorium, chorda_ex_literis(titulus,
        repertorium->tempus));
    si (!d)
    {
        *(chorda*)xar_addere(repertorium->opera_orba) =
            chorda_ex_literis(
            titulus, repertorium->piscina);
        redde VERUM;
    }
    d->opera         = opera;
    d->opera_posita  = VERUM;
    redde VERUM;
}

b32
conspectus_per_rem_ponere (
     ConspectusRegistrum* repertorium,
      constans character* titulus,
      constans character* spatium,
        ConspectusPerRem  per_rem)
{
    Declaratio* d;
           i32  i;

    si (   !repertorium || !titulus || !spatium || !per_rem.numerus
        || !per_rem.id || !per_rem.vestigium || !per_rem.claves)
    {
        redde FALSUM;
    }
    d = declaratio_quaerere(repertorium, chorda_ex_literis(titulus,
        repertorium->tempus));
    per (i = ZEPHYRUM; d && i < xar_numerus(d->fontes); i++)
    {
        Fons* f = (Fons*)xar_obtinere(d->fontes, i);

        si (   f->genus == FONS_DOCUMENTUM && !f->omnia
            && chorda_aequalis_literis(f->spatium, spatium))
        {
            f->per_rem          = per_rem;
            f->per_rem_positum  = VERUM;
            redde VERUM;
        }
    }
    redde FALSUM;
}

b32
conspectus_documentum_ligare (
    ConspectusRegistrum* repertorium,
     constans character* spatium,
                 vacuum* documentum)
{
    DocumentumLigatum* l;

    si (!repertorium || !spatium || !spatium[ZEPHYRUM] || !documentum)
    {
        redde FALSUM;
    }
    l = (DocumentumLigatum*)xar_addere(repertorium->documenta);
    l->spatium = chorda_ex_literis(spatium, repertorium->piscina);
    l->documentum = documentum;
    redde VERUM;
}

b32
conspectus_parare (
    ConspectusRegistrum*  repertorium,
                    Xar** vitia)
{
    ConspectusRegistrum* r;
                    Xar* v;
                    i32  i;
                    i32  k;
              character  buffer[DXII];

    r = repertorium;
    si (!r || !r->tempus)
    {
        redde FALSUM;
    }
    v = xar_creare(r->piscina, (i32)magnitudo(chorda));
    si (vitia)
    {
        *vitia = v;
    }
    per (i = ZEPHYRUM; i < xar_numerus(r->opera_orba); i++)
    {
        chorda t = *(chorda*)xar_obtinere(r->opera_orba, i);

        si (t.mensura > ZEPHYRUM)
        {
            sprintf(buffer, "opera sine declaratione: '%.*s'",
                (integer)t.mensura, (constans character*)t.datum);
            vitium_addere(v, r->piscina, buffer);
        }
    }
    per (i = ZEPHYRUM; i < xar_numerus(r->declarationes); i++)
    {
        Declaratio* d = *(Declaratio**)xar_obtinere(r->declarationes,
            i);

        si (!d->opera_posita)
        {
            sprintf(buffer, "conspectus '%.*s': opera C absentia",
                (integer)d->titulus.mensura,
                (constans character*)d->titulus.datum);
            vitium_addere(v, r->piscina, buffer);
        }
        per (k = ZEPHYRUM; k < xar_numerus(d->fontes); k++)
        {
            Fons* f = (Fons*)xar_obtinere(d->fontes, k);

            si (f->genus != FONS_DOCUMENTUM || f->omnia)
            {
                perge;
            }
            si (!f->per_rem_positum)
            {
                sprintf(buffer,
                    "conspectus '%.*s': fons '%.*s' per-rem "
                    "sine functionibus", (integer)d->titulus.mensura,
                    (constans character*)d->titulus.datum,
                    (integer)f->spatium.mensura,
                    (constans character*)f->spatium.datum);
                vitium_addere(v, r->piscina, buffer);
            }
            si (!documentum_ligatum(r, f->spatium))
            {
                sprintf(buffer,
                    "conspectus '%.*s': documentum '%.*s' non "
                    "ligatum", (integer)d->titulus.mensura,
                    (constans character*)d->titulus.datum,
                    (integer)f->spatium.mensura,
                    (constans character*)f->spatium.datum);
                vitium_addere(v, r->piscina, buffer);
            }
        }
    }
    si (xar_numerus(v) > ZEPHYRUM)
    {
        redde FALSUM;
    }
    /* basis per-rem et cursor ad finem: cache vacuum, nihil stalum */
    per (i = ZEPHYRUM; i < xar_numerus(r->declarationes); i++)
    {
        Declaratio* d = *(Declaratio**)xar_obtinere(r->declarationes,
            i);

        per (k = ZEPHYRUM; k < xar_numerus(d->fontes); k++)
        {
            Fons* f = (Fons*)xar_obtinere(d->fontes, k);

            si (f->genus == FONS_DOCUMENTUM && !f->omnia)
            {
                differentiam_facere(r, d, f, FALSUM);
            }
        }
    }
    r->cursor   = volumen_seq_ultimum(r->volumen);
    r->paratum  = VERUM;
    tempus_renovare(r);
    redde VERUM;
}


/* ==================================================
 * Cursor
 * ================================================== */

s64
conspectus_renovare (
    ConspectusRegistrum* repertorium)
{
    ConspectusRegistrum* r;
                    Xar* acta;
                    i32  i;
                    i32  k;
                    i32  j;

    r = repertorium;
    si (!r || !r->paratum)
    {
        redde r ? r->cursor : ZEPHYRUM;
    }
    /* nihil novum (casus frequentissimus: lectiones multae per
     * picturam) - quaestio una indicata, sine lectione actorum */
    si (volumen_seq_ultimum(r->volumen) <= r->cursor)
    {
        piscina_vacare(r->tempus);
        redde r->cursor;
    }
    acta = volumen_acta_legere(r->volumen, r->cursor, r->tempus);
    per (i = ZEPHYRUM; acta && i < xar_numerus(acta); i++)
    {
        VolumenActum* a = (VolumenActum*)xar_obtinere(acta, i);
              chorda  via;

        r->cursor  = a->seq;
        via        = a->genus;
        si (   chorda_aequalis_literis(a->genus, "plagula-condita")
            || chorda_aequalis_literis(a->genus, "plagula-remota"))
        {
            via = via_plagulae(a->datum, r->tempus);
        }
        si (via.mensura == ZEPHYRUM)
        {
            perge;
        }
        per (k = ZEPHYRUM; k < xar_numerus(r->declarationes); k++)
        {
            Declaratio* d =
                *(Declaratio**)xar_obtinere(r->declarationes,
                k);

            per (j = ZEPHYRUM; j < xar_numerus(d->fontes); j++)
            {
                Fons* f = (Fons*)xar_obtinere(d->fontes, j);

                si (f->genus == FONS_DOCUMENTUM)
                {
                    si (sub_spatio(via, f->spatium))
                    {
                        f->tactus = VERUM;
                    }
                }
                alioquin si (f->genus == FONS_SPATIUM)
                {
                    chorda clavis = clavis_ex_spatio(via, f);

                    si (clavis.mensura > ZEPHYRUM)
                    {
                        clavem_irritare(r, d, clavis);
                    }
                }
            }
        }
    }
    per (k = ZEPHYRUM; k < xar_numerus(r->declarationes); k++)
    {
        Declaratio* d = *(Declaratio**)xar_obtinere(r->declarationes,
            k);

        per (j = ZEPHYRUM; j < xar_numerus(d->fontes); j++)
        {
            Fons* f = (Fons*)xar_obtinere(d->fontes, j);

            si (!f->tactus)
            {
                perge;
            }
            f->tactus = FALSUM;
            si (f->omnia)
            {
                declarationem_irritare(d);
            }
            alioquin
            {
                differentiam_facere(r, d, f, VERUM);
            }
        }
    }
    tempus_renovare(r);
    redde r->cursor;
}


/* ==================================================
 * Lectio
 * ================================================== */

interior vacuum
introitum_liberare (
    ConspectusRegistrum* r,
              Introitus* e)
{
    si (e->piscina)
    {
        r->usus -= e->usus;
        piscina_destruere(e->piscina);
    }
    e->piscina  = NIHIL;
    e->valor    = NIHIL;
    e->usus     = ZEPHYRUM;
}

/* minime nuper lecta eicere dum usus > tectum (praeter servandum et
 * quae in computatione sunt) */
interior vacuum
eicere (
    ConspectusRegistrum* r,
              Introitus* servandus)
{
    dum (r->usus > r->tectum)
    {
        Introitus* minimus;
              i32  i;

        minimus = NIHIL;
        per (i = ZEPHYRUM; i < xar_numerus(r->omnes); i++)
        {
            Introitus* e = *(Introitus**)xar_obtinere(r->omnes, i);

            si (   e         == servandus || e->in_computatione
                || e->status == CONSPECTUS_ABSENS)
            {
                perge;
            }
            si (!minimus || e->ultimum < minimus->ultimum)
            {
                minimus = e;
            }
        }
        si (!minimus)
        {
            frange;
        }
        introitum_liberare(r, minimus);
        minimus->status = CONSPECTUS_ABSENS;
    }
}

interior vacuum
dependentem_addere (
    ConspectusRegistrum* r,
              Introitus* lectus,
              Introitus* legens)
{
    i32 i;

    si (!lectus->dependentes)
    {
        lectus->dependentes = xar_creare(r->piscina,
            (i32)magnitudo(Introitus*));
    }
    per (i = ZEPHYRUM; i < xar_numerus(lectus->dependentes); i++)
    {
        si (*(Introitus**)xar_obtinere(lectus->dependentes, i)
            == legens)
        {
            redde;
        }
    }
    *(Introitus**)xar_addere(lectus->dependentes) = legens;
}

interior Introitus*
introitum_quaerere (
    ConspectusRegistrum* r,
     constans character* titulus,
                 chorda  clavis)
{
       chorda  inscriptio;
       vacuum* valor;
    character  buffer[CCLVI];

    si (strlen(titulus) + (size_t)clavis.mensura + II
        > magnitudo(buffer))
    {
        redde NIHIL;
    }
    sprintf(buffer, "%s/%.*s", titulus, (integer)clavis.mensura,
        (constans character*)clavis.datum);
    inscriptio = chorda_ex_literis(buffer, r->tempus);
    redde tabula_dispersa_invenire(r->tabula, inscriptio, &valor)
        ? (Introitus*)valor : NIHIL;
}

constans vacuum*
conspectus_legere (
    ConspectusRegistrum* repertorium,
     constans character* titulus,
                 chorda  clavis)
{
    ConspectusRegistrum* r;
             Declaratio* d;
              Introitus* e;
              Introitus* legens;
                 vacuum* valor;
              character  buffer[CCLVI];

    r = repertorium;
    si (!r || !r->paratum || !titulus || clavis.mensura <= ZEPHYRUM)
    {
        redde NIHIL;
    }
    si (xar_numerus(r->acervus) == ZEPHYRUM)
    {
        (vacuum)conspectus_renovare(r);
    }
    d = declaratio_quaerere(r, chorda_ex_literis(titulus, r->tempus));
    si (!d || !organum_typum_iudicare(d->typus, clavis, r->tempus))
    {
        redde NIHIL;
    }
    legens = xar_numerus(r->acervus) > ZEPHYRUM
           ? *(Introitus**)xar_obtinere(r->acervus,
                 xar_numerus(r->acervus) - I)
           : NIHIL;
    e = introitum_quaerere(r, titulus, clavis);
    si (e && e->in_computatione)
    {
        redde NIHIL;   /* circulus */
    }
    r->horologium++;
    si (e && e->status == CONSPECTUS_RECENS)
    {
        e->ultimum = r->horologium;
        si (legens)
        {
            dependentem_addere(r, e, legens);
        }
        redde e->valor;
    }
    si (!e)
    {
        si (strlen(titulus) + (size_t)clavis.mensura + II
            > magnitudo(buffer))
        {
            redde NIHIL;
        }
        sprintf(buffer, "%s/%.*s", titulus, (integer)clavis.mensura,
            (constans character*)clavis.datum);
        e = (Introitus*)piscina_allocare(r->piscina,
            magnitudo(Introitus));
        memset(e, ZEPHYRUM, magnitudo(Introitus));
        e->declaratio  = d;
        e->clavis      = chorda_transcribere(clavis, r->piscina);
        e->inscriptio  = chorda_ex_literis(buffer, r->piscina);
        e->status      = CONSPECTUS_ABSENS;
        (vacuum)tabula_dispersa_inserere(r->tabula, e->inscriptio, e);
        *(Introitus**)xar_addere(r->omnes)      = e;
        *(Introitus**)xar_addere(d->introitus)  = e;
    }
    introitum_liberare(r, e);
    e->piscina = piscina_generare_dynamicum("conspectus",
        CONSPECTUS_ALVEUS);
    si (!e->piscina)
    {
        e->status = CONSPECTUS_ABSENS;
        redde NIHIL;
    }
    e->in_computatione = VERUM;
    *(Introitus**)xar_addere(r->acervus) = e;
    valor = d->opera.computare(d->opera.ctx, r, e->clavis, e->piscina);
    xar_truncare(r->acervus, xar_numerus(r->acervus) - I);
    e->in_computatione = FALSUM;
    si (legens)
    {
        dependentem_addere(r, e, legens);
    }
    si (!valor)
    {
        introitum_liberare(r, e);
        e->status = CONSPECTUS_ABSENS;
        redde NIHIL;
    }
    e->valor    = valor;
    e->status   = CONSPECTUS_RECENS;
    e->usus     = piscina_summa_usus(e->piscina);
    e->seq      = r->cursor;
    e->ultimum  = r->horologium;
    r->usus     += e->usus;
    eicere(r, e);
    redde valor;
}

ConspectusStatus
conspectus_status (
    ConspectusRegistrum* repertorium,
     constans character* titulus,
                 chorda  clavis)
{
    Introitus* e;

    si (!repertorium || !repertorium->paratum || !titulus)
    {
        redde CONSPECTUS_ABSENS;
    }
    si (xar_numerus(repertorium->acervus) == ZEPHYRUM)
    {
        (vacuum)conspectus_renovare(repertorium);
    }
    e = introitum_quaerere(repertorium, titulus, clavis);
    redde e ? e->status : CONSPECTUS_ABSENS;
}


/* ==================================================
 * Verificatio et mensura
 * ================================================== */

i32
conspectus_verificare (
    ConspectusRegistrum* repertorium,
                    Xar* discrepantes)
{
    ConspectusRegistrum* r;
                Piscina* p;
                    Xar* recentes;
                    i32  n;
                    i32  i;

    r = repertorium;
    si (!r || !r->paratum)
    {
        redde ZEPHYRUM;
    }
    (vacuum)conspectus_renovare(r);
    p = piscina_generare_dynamicum("conspectus_verificatio",
        IV * MXXIV);
    si (!p)
    {
        redde ZEPHYRUM;
    }
    /* imago: computatio interior cache mutare potest */
    recentes = xar_creare(p, (i32)magnitudo(Introitus*));
    per (i = ZEPHYRUM; i < xar_numerus(r->omnes); i++)
    {
        Introitus* e = *(Introitus**)xar_obtinere(r->omnes, i);

        si (e->status == CONSPECTUS_RECENS)
        {
            *(Introitus**)xar_addere(recentes) = e;
        }
    }
    n = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(recentes); i++)
    {
         Introitus* e = *(Introitus**)xar_obtinere(recentes, i);
        Declaratio* d = e->declaratio;
           Piscina* q;
            vacuum* novus;
               b32  aequales;

        si (e->status != CONSPECTUS_RECENS)
        {
            perge;
        }
        q = piscina_generare_dynamicum("conspectus_verificatio_unius",
            CONSPECTUS_ALVEUS);
        si (!q)
        {
            perge;
        }
        e->in_computatione = VERUM;
        *(Introitus**)xar_addere(r->acervus) = e;
        novus = d->opera.computare(d->opera.ctx, r, e->clavis, q);
        xar_truncare(r->acervus, xar_numerus(r->acervus) - I);
        e->in_computatione = FALSUM;
        aequales = novus && chorda_aequalis(
            d->opera.codificare(d->opera.ctx, novus, q),
            d->opera.codificare(d->opera.ctx, e->valor, q));
        si (!aequales)
        {
            n++;
            si (discrepantes)
            {
                *(chorda*)xar_addere(discrepantes) =
                    chorda_transcribere(
                    e->inscriptio, discrepantes->piscina);
            }
        }
        piscina_destruere(q);
    }
    piscina_destruere(p);
    redde n;
}

memoriae_index
conspectus_summa_usus (
    constans ConspectusRegistrum* repertorium)
{
    redde repertorium ? repertorium->usus : ZEPHYRUM;
}
