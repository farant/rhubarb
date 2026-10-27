/* contractio.c - Retia tensorum: structura, ordo avidus, tabula
 *
 * T2 (project-specs/contractio-plan.md): rete, ordo avidus cum clave
 * exacta, dispositio tabulae per vitam (primum aptum), sumptus
 * saturati, officina. Exsecutio (gradus calidus et referentiae) in
 * T3. Vide include/contractio.h et lib/contractio.worklog.md.
 */
#include "contractio.h"
#include "xar.h"
#include "magnus.h"
#include "sors.h"

/* id tensoris nullus (possessor absens) */
#define CONTRACTIO_NULLUS      ((i32)0xFFFFFFFFUL)

/* signum temporarium in IndexRetis.usus: index iam in hoc tensore */
#define CONTRACTIO_USUS_SIGNUM ((i32)0x80000000UL)


/* ==================================================
 * Structurae privatae
 * ================================================== */

nomen structura {
    i32 dimensio;
    i32 usus;            /* tensores qui indicem tenent: 0, I, II */
} IndexRetis;

/* indices tensoris in rete->membra [initium, initium + numerus) */
nomen structura {
    i32 initium;
    i32 numerus;
} TensorRetis;

structura ContractioRete {
    Piscina* piscina;
        Xar* indices;    /* IndexRetis */
        Xar* tensores;   /* TensorRetis */
        Xar* membra;     /* i32: indices omnium tensorum, continui */
        b32  ordinatum;
};

/* tensor in ordine: initiales indices ordine vocantis, intermedii
 * ordine id ascendente */
nomen structura {
    i32* indices;
    i32  numerus;
    i64  elementa;       /* saturata */
    i64  offset;         /* in tabula (elementa i32); solum intermedii */
} TensorOrdinis;

/* gradus cum consilio ansae: solum indices dimensionis >= 2 (indices
 * dimensionis 1 offset numquam movent et numero non finiuntur) */
nomen structura {
    i32  sinister;
    i32  dexter;
    i32  effectus;
    i32  externi;            /* indices effectus, ordine effectus */
    i32  interni;            /* indices communes, ordine sinistri */
    i32* dimensiones;        /* externi + interni */
    i64* passus_sinistri;    /* passus in sinistro; 0 si ibi absens */
    i64* passus_dextri;      /* passus in dextro; 0 si ibi absens */
} GradusOrdinis;

structura ContractioOrdo {
    constans ContractioRete* rete;
                        i32  initiales;          /* n */
                        i32  numerus_graduum;
              GradusOrdinis* gradus;
              TensorOrdinis* tensores;           /* n + gradus */
                        b32  tabula_saturata;
          ContractioSumptus  sumptus;
                        i32  candidatus;           /* victor (T5b) */
                        i32  numerus_candidatorum;
          ContractioSumptus* candidati;            /* simulati, 1.. */
                        b32* candidati_aestimati;
};

structura ContractioOfficina {
    constans ContractioOrdo* ordo;
                        i32* tabula;             /* NIHIL si vacua */
};

/* intervallum tabulae occupatum [offset, finis) */
nomen structura {
    i64 offset;
    i64 finis;
    i32 tensor;
} IntervallumTabulae;


/* ==================================================
 * Arithmetica saturata
 * ================================================== */

interior i64
_multiplicare_saturate (
    i64 a,
    i64 b)
{
    si (a == ZEPHYRUM || b == ZEPHYRUM)
    {
        redde ZEPHYRUM;
    }
    si (a > CONTRACTIO_SATURATUM / b)
    {
        redde CONTRACTIO_SATURATUM;
    }
    redde a * b;
}

interior i64
_addere_saturate (
    i64 a,
    i64 b)
{
    si (a > CONTRACTIO_SATURATUM - b)
    {
        redde CONTRACTIO_SATURATUM;
    }
    redde a + b;
}

/* numerus bitorum x: 0 pro 0, k + 1 pro 2^k <= x < 2^(k+1) */
interior i32
_bita (
    i64 x)
{
    i32 n = ZEPHYRUM;

    dum (x != ZEPHYRUM)
    {
        n++;
        x >>= I;
    }
    redde n;
}


/* ==================================================
 * Rete
 * ================================================== */

ContractioRete*
contractio_rete_creare (
    Piscina* piscina)
{
    ContractioRete* rete;

    si (piscina == NIHIL)
    {
        redde NIHIL;
    }
    rete = (ContractioRete*)piscina_allocare(piscina,
        magnitudo(ContractioRete));
    rete->piscina    = piscina;
    rete->indices    = xar_creare(piscina, (i32)magnitudo(IndexRetis));
    rete->tensores   = xar_creare(piscina, (i32)magnitudo(TensorRetis));
    rete->membra     = xar_creare(piscina, (i32)magnitudo(i32));
    rete->ordinatum  = FALSUM;
    redde rete;
}

b32
contractio_index_addere (
    ContractioRete* rete,
               i32  dimensio,
               i32* id)
{
    IndexRetis* index;

    si (   rete     == NIHIL || id == NIHIL || rete->ordinatum
        || dimensio == ZEPHYRUM)
    {
        redde FALSUM;
    }
    index = (IndexRetis*)xar_addere(rete->indices);
    si (index == NIHIL)
    {
        redde FALSUM;
    }
    index->dimensio  = dimensio;
    index->usus      = ZEPHYRUM;
    *id              = xar_numerus(rete->indices) - I;
    redde VERUM;
}

/* signa temporaria indicum [0, numerus) delere */
interior vacuum
_signa_delere (
    ContractioRete* rete,
     constans i32*  indices,
              i32   numerus)
{
    i32 j;

    per (j = ZEPHYRUM; j < numerus; j++)
    {
        IndexRetis* index = (IndexRetis*)xar_obtinere(rete->indices,
            indices[j]);

        index->usus &= ~CONTRACTIO_USUS_SIGNUM;
    }
}

b32
contractio_tensor_addere (
        ContractioRete* rete,
         constans i32*  indices,
                  i32   numerus,
                  i32*  id)
{
     TensorRetis* tensor;
             i32  numerus_indicum;
             i32  initium;
             i32  j;

    si (   rete == NIHIL || id == NIHIL || rete->ordinatum
        || (numerus > ZEPHYRUM && indices == NIHIL))
    {
        redde FALSUM;
    }
    numerus_indicum = xar_numerus(rete->indices);

    /* validatio in O(numerus): signum in usus pro indice iam viso */
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        IndexRetis* index;

        si (indices[j] >= numerus_indicum)
        {
            _signa_delere(rete, indices, j);
            redde FALSUM;
        }
        index = (IndexRetis*)xar_obtinere(rete->indices, indices[j]);
        si (   (index->usus & CONTRACTIO_USUS_SIGNUM) != ZEPHYRUM
            || index->usus                            >= II)
        {
            _signa_delere(rete, indices, j);
            redde FALSUM;
        }
        index->usus |= CONTRACTIO_USUS_SIGNUM;
    }
    _signa_delere(rete, indices, numerus);

    initium = xar_numerus(rete->membra);
    si (   numerus > ZEPHYRUM
        && xar_addere_multos(rete->membra, indices, numerus) != numerus)
    {
        xar_truncare(rete->membra, initium);
        redde FALSUM;
    }
    tensor = (TensorRetis*)xar_addere(rete->tensores);
    si (tensor == NIHIL)
    {
        xar_truncare(rete->membra, initium);
        redde FALSUM;
    }
    tensor->initium = initium;
    tensor->numerus = numerus;
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        IndexRetis* index = (IndexRetis*)xar_obtinere(rete->indices,
            indices[j]);

        index->usus++;
    }
    *id = xar_numerus(rete->tensores) - I;
    redde VERUM;
}

b32
contractio_rete_clausum (
    constans ContractioRete* rete)
{
    i32 numerus;
    i32 j;

    si (rete == NIHIL)
    {
        redde FALSUM;
    }
    numerus = xar_numerus(rete->indices);
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        constans IndexRetis* index = (constans IndexRetis*)xar_obtinere(
            rete->indices, j);

        si (index->usus != II)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Ordo avidus
 * ================================================== */

ContractioOptiones
contractio_optiones_ordinariae (vacuum)
{
    ContractioOptiones o;

    o.methodus       = CONTRACTIO_AVIDUS;
    o.conatus        = XVI;
    o.semen          = CONTRACTIO_SEMEN_ORDINARIUM;
    o.octeti_maximi  = CONTRACTIO_SATURATUM;
    o.candidatus     = CONTRACTIO_CANDIDATUS_OMNES;
    o.limen          = CONTRACTIO_LIMEN_ORDINARIUM;
    redde o;
}

/* possessor alter indicis respectu tensoris */
interior i32
_alter (
    constans i32* possessores,
             i32  index,
             i32  tensor)
{
    si (possessores[II * index] == tensor)
    {
        redde possessores[II * index + I];
    }
    redde possessores[II * index];
}

/* elementa effectus si a et b contrahuntur: productum dimensionum
 * indicum non communium (saturatum) */
interior i64
_elementa_effectus (
    constans TensorOrdinis* tensores,
              constans i32* possessores,
              constans i32* dimensiones,
                       i32  a,
                       i32  b)
{
    i64 elementa = I;
    i32 j;

    per (j = ZEPHYRUM; j < tensores[a].numerus; j++)
    {
        i32 index = tensores[a].indices[j];

        si (_alter(possessores, index, a) != b)
        {
            elementa = _multiplicare_saturate(elementa,
                dimensiones[index]);
        }
    }
    per (j = ZEPHYRUM; j < tensores[b].numerus; j++)
    {
        i32 index = tensores[b].indices[j];

        si (_alter(possessores, index, b) != a)
        {
            elementa = _multiplicare_saturate(elementa,
                dimensiones[index]);
        }
    }
    redde elementa;
}

/* clavis (elementa, a, b) lexicographice minor quam optima? */
interior b32
_clavis_minor (
    i64 elementa,
    i32 a,
    i32 b,
    i64 optima_elementa,
    i32 optimus_a,
    i32 optimus_b)
{
    si (elementa != optima_elementa)
    {
        redde elementa < optima_elementa;
    }
    si (a != optimus_a)
    {
        redde a < optimus_a;
    }
    redde b < optimus_b;
}

/* clavis paris: AVIDUS (T2) = elementa effectus (i64 saturata);
 * REDUCTIO = E_r - E_a - E_b (quisque E ad 2^62 limitatus, s64),
 * translata (+ 2^63) ut ordo i64 sine signo ordinem s64 servet */
interior i64
_clavis_paris (
    constans TensorOrdinis* tensores,
              constans i32* possessores,
              constans i32* dimensiones,
                       i32  genus,
                       i32  a,
                       i32  b)
{
    i64 effectus = _elementa_effectus(tensores, possessores,
        dimensiones,
        a, b);
    i64 limes = (i64)I << LXII;
    s64 clavis;

    si (genus != CONTRACTIO_CANDIDATUS_REDUCTIO)
    {
        redde effectus;
    }
    clavis = (s64)(effectus < limes ? effectus : limes)
        - (s64)(tensores[a].elementa
        < limes ? tensores[a].elementa : limes)
        - (s64)(tensores[b].elementa
        < limes ? tensores[b].elementa : limes);
    redde (i64)clavis ^ ((i64)I << LXIII);
}

/* par eligere: primum inter paria indicem communem habentia; si
 * nullum, inter omnia paria (vivi ascendentes, ergo a < b) */
interior vacuum
_par_eligere (
    constans TensorOrdinis* tensores,
              constans i32* possessores,
              constans i32* dimensiones,
              constans i32* vivi,
                       i32  numerus_vivorum,
                       i32  genus,
                       i32* sinister,
                       i32* dexter)
{
    b32 inventum  = FALSUM;
    i64 optima    = ZEPHYRUM;
    i32 oa        = ZEPHYRUM;
    i32 ob        = ZEPHYRUM;
    i32 x;
    i32 y;
    i32 j;

    per (x = ZEPHYRUM; x < numerus_vivorum; x++)
    {
        i32 a = vivi[x];

        per (j = ZEPHYRUM; j < tensores[a].numerus; j++)
        {
            i32 b = _alter(possessores, tensores[a].indices[j], a);
            i64 elementa;

            si (b == CONTRACTIO_NULLUS || b <= a)
            {
                perge;
            }
            elementa = _clavis_paris(tensores, possessores, dimensiones,
                genus, a, b);
            si (   !inventum
                || _clavis_minor(elementa, a, b, optima, oa, ob))
            {
                inventum  = VERUM;
                optima    = elementa;
                oa        = a;
                ob        = b;
            }
        }
    }
    si (!inventum)
    {
        /* componentes disiunctae: productum externum. Si omnes vivi
         * scalares sunt (in rete clauso semper), omnis clavis 1 est:
         * par = duo id minimi, sine scansione O(L^2) per gradum. */
        b32 sine_indicibus = VERUM;

        per (x = ZEPHYRUM; x < numerus_vivorum; x++)
        {
            si (tensores[vivi[x]].numerus != ZEPHYRUM)
            {
                sine_indicibus = FALSUM;
                frange;
            }
        }
        si (sine_indicibus)
        {
            *sinister  = vivi[ZEPHYRUM];
            *dexter    = vivi[I];
            redde;
        }
        per (x = ZEPHYRUM; x < numerus_vivorum; x++)
        {
            per (y = x + I; y < numerus_vivorum; y++)
            {
                i64 elementa = _clavis_paris(tensores, possessores,
                    dimensiones, genus, vivi[x], vivi[y]);

                si (   !inventum
                    || _clavis_minor(elementa, vivi[x], vivi[y],
                    optima, oa, ob))
                {
                    inventum  = VERUM;
                    optima    = elementa;
                    oa        = vivi[x];
                    ob        = vivi[y];
                }
            }
        }
    }
    *sinister  = oa;
    *dexter    = ob;
}

/* a et b in c contrahere: indices c (ordine id), possessores
 * renovati, elementa; redde multiplicationes gradus (productum
 * dimensionum unionis, saturatum) */
interior i64
_contrahere (
    TensorOrdinis* tensores,
              i32* possessores,
     constans i32* dimensiones,
              i32  a,
              i32  b,
              i32  c,
          Piscina* piscina)
{
     TensorOrdinis* t        = &tensores[c];
               i32  numerus  = ZEPHYRUM;
               i64  multiplicationes;
               i32  j;
               i32  k;

    per (j = ZEPHYRUM; j < tensores[a].numerus; j++)
    {
        si (_alter(possessores, tensores[a].indices[j], a) != b)
        {
            numerus++;
        }
    }
    per (j = ZEPHYRUM; j < tensores[b].numerus; j++)
    {
        si (_alter(possessores, tensores[b].indices[j], b) != a)
        {
            numerus++;
        }
    }
    t->numerus   = numerus;
    t->indices   = NIHIL;
    t->elementa  = I;
    t->offset    = CONTRACTIO_SATURATUM;
    si (numerus > ZEPHYRUM)
    {
        t->indices = (i32*)piscina_allocare(piscina,
            (memoriae_index)numerus * magnitudo(i32));
    }

    /* indices servati; communes semel in multiplicationes */
    numerus           = ZEPHYRUM;
    multiplicationes  = I;
    per (j = ZEPHYRUM; j < tensores[a].numerus; j++)
    {
        i32 index = tensores[a].indices[j];

        si (_alter(possessores, index, a) != b)
        {
            t->indices[numerus++] = index;
        }
        alioquin
        {
            multiplicationes = _multiplicare_saturate(multiplicationes,
                dimensiones[index]);
        }
    }
    per (j = ZEPHYRUM; j < tensores[b].numerus; j++)
    {
        i32 index = tensores[b].indices[j];

        si (_alter(possessores, index, b) != a)
        {
            t->indices[numerus++] = index;
        }
    }

    /* ordo id ascendens (insertio: tensores parvi) */
    per (j = I; j < numerus; j++)
    {
        i32 index = t->indices[j];

        k = j;
        dum (k > ZEPHYRUM && t->indices[k - I] > index)
        {
            t->indices[k] = t->indices[k - I];
            k--;
        }
        t->indices[k] = index;
    }

    /* possessores: a aut b -> c */
    per (j = ZEPHYRUM; j < numerus; j++)
    {
        i32 index = t->indices[j];

        si (   possessores[II * index] == a
            || possessores[II * index] == b)
        {
            possessores[II * index] = c;
        }
        alioquin
        {
            possessores[II * index + I] = c;
        }
        t->elementa = _multiplicare_saturate(t->elementa,
            dimensiones[index]);
    }
    redde _multiplicare_saturate(multiplicationes, t->elementa);
}

/* index dimensionis 1 offset numquam movet: e consilio ansae omittitur */
interior b32
_movet (
    i32 dimensio)
{
    redde dimensio > I;
}

/* passus indicum tensoris (index ultimus celerrimus) in tabulam per
 * indicem scribere (valor > 0), aut delere (ZEPHYRUM) */
interior vacuum
_passus_ponere (
    constans TensorOrdinis* t,
              constans i32* dimensiones,
                       i64* passus,
                       b32  ponere)
{
    i64 p = I;
    i32 j = t->numerus;

    dum (j > ZEPHYRUM)
    {
        j--;
        passus[t->indices[j]] = ponere ? p : ZEPHYRUM;
        p = _multiplicare_saturate(p, dimensiones[t->indices[j]]);
    }
}

/* consilium ansae gradus: externi = indices effectus dimensionis >= 2
 * (ordine effectus: offset effectus tunc simpliciter crescit),
 * interni = indices communes dimensionis >= 2 (ordine sinistri),
 * passus in utroque operando. Tabulae passus_a/passus_b nullae ante
 * et post. */
interior vacuum
_gradum_parare (
     constans TensorOrdinis* tensores,
               constans i32* dimensiones,
                        i64* passus_a,
                        i64* passus_b,
              GradusOrdinis* g,
                    Piscina* piscina)
{
    constans TensorOrdinis* a = &tensores[g->sinister];
    constans TensorOrdinis* b = &tensores[g->dexter];
    constans TensorOrdinis* c = &tensores[g->effectus];
                       i32  numerus;
                       i32  j;
                       i32  k = ZEPHYRUM;

    _passus_ponere(a, dimensiones, passus_a, VERUM);
    _passus_ponere(b, dimensiones, passus_b, VERUM);
    g->externi = ZEPHYRUM;
    g->interni = ZEPHYRUM;
    per (j = ZEPHYRUM; j < c->numerus; j++)
    {
        si (_movet(dimensiones[c->indices[j]]))
        {
            g->externi++;
        }
    }
    per (j = ZEPHYRUM; j < a->numerus; j++)
    {
        i32 index = a->indices[j];

        si (passus_b[index] != ZEPHYRUM && _movet(dimensiones[index]))
        {
            g->interni++;
        }
    }
    numerus             = g->externi + g->interni;
    g->dimensiones      = NIHIL;
    g->passus_sinistri  = NIHIL;
    g->passus_dextri    = NIHIL;
    si (numerus > ZEPHYRUM)
    {
        g->dimensiones = (i32*)piscina_allocare(piscina,
            (memoriae_index)numerus * magnitudo(i32));
        g->passus_sinistri = (i64*)piscina_allocare(piscina,
            (memoriae_index)numerus * magnitudo(i64));
        g->passus_dextri = (i64*)piscina_allocare(piscina,
            (memoriae_index)numerus * magnitudo(i64));
    }
    per (j = ZEPHYRUM; j < c->numerus; j++)
    {
        i32 index = c->indices[j];

        si (_movet(dimensiones[index]))
        {
            g->dimensiones[k]      = dimensiones[index];
            g->passus_sinistri[k]  = passus_a[index];
            g->passus_dextri[k]    = passus_b[index];
            k++;
        }
    }
    per (j = ZEPHYRUM; j < a->numerus; j++)
    {
        i32 index = a->indices[j];

        si (passus_b[index] != ZEPHYRUM && _movet(dimensiones[index]))
        {
            g->dimensiones[k]      = dimensiones[index];
            g->passus_sinistri[k]  = passus_a[index];
            g->passus_dextri[k]    = passus_b[index];
            k++;
        }
    }
    _passus_ponere(a, dimensiones, passus_a, FALSUM);
    _passus_ponere(b, dimensiones, passus_b, FALSUM);
}

/* tabula: intermedii ordine graduum; effectus gradus primo apto
 * ponitur dum operandi eius adhuc vivunt, deinde operandi liberantur */
interior vacuum
_tabulam_disponere (
    ContractioOrdo* ordo,
          Piscina* piscina)
{
    IntervallumTabulae* occupata;
                   i32  numerus  = ZEPHYRUM;
                   i64  summum   = ZEPHYRUM;
                   i32  s;
                   i32  j;

    ordo->tabula_saturata = FALSUM;
    si (ordo->numerus_graduum == ZEPHYRUM)
    {
        ordo->sumptus.octeti_officinae = ZEPHYRUM;
        redde;
    }
    occupata = (IntervallumTabulae*)piscina_allocare(piscina,
        (memoriae_index)ordo->numerus_graduum * magnitudo(IntervallumTabulae));

    per (s = ZEPHYRUM; s < ordo->numerus_graduum; s++)
    {
         constans GradusOrdinis* g = &ordo->gradus[s];
                  TensorOrdinis* t = &ordo->tensores[g->effectus];
                            i64  candidatum = ZEPHYRUM;
                            i64  finis;
                            i32  locus = numerus;

        si (t->elementa == CONTRACTIO_SATURATUM)
        {
            ordo->tabula_saturata = VERUM;
            frange;
        }
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            si (   occupata[j].offset              >= candidatum
                && occupata[j].offset - candidatum >= t->elementa)
            {
                locus = j;
                frange;
            }
            candidatum = occupata[j].finis;
        }
        /* finis tabulae <= summa intermediorum <= multiplicationes:
         * hic saturatio solum si multiplicationes iam saturatae
         * (officina tunc refutatur) - ramus custos, non probatus */
        finis = _addere_saturate(candidatum, t->elementa);
        si (finis == CONTRACTIO_SATURATUM)
        {
            ordo->tabula_saturata = VERUM;
            frange;
        }
        per (j = numerus; j > locus; j--)
        {
            occupata[j] = occupata[j - I];
        }
        occupata[locus].offset  = candidatum;
        occupata[locus].finis   = finis;
        occupata[locus].tensor  = g->effectus;
        numerus++;
        t->offset = candidatum;
        si (finis > summum)
        {
            summum = finis;
        }

        /* operandi liberantur (initiales numquam in tabula) */
        j = ZEPHYRUM;
        dum (j < numerus)
        {
            si (   occupata[j].tensor == g->sinister
                || occupata[j].tensor == g->dexter)
            {
                i32 k;

                per (k = j; k + I < numerus; k++)
                {
                    occupata[k] = occupata[k + I];
                }
                numerus--;
            }
            alioquin
            {
                j++;
            }
        }
    }
    si (ordo->tabula_saturata)
    {
        per (s = ZEPHYRUM; s < ordo->numerus_graduum; s++)
        {
            ordo->tensores[ordo->gradus[s].effectus].offset =
                CONTRACTIO_SATURATUM;
        }
        ordo->sumptus.octeti_officinae = CONTRACTIO_SATURATUM;
        redde;
    }
    ordo->sumptus.octeti_officinae = _multiplicare_saturate(summum,
        (i64)magnitudo(i32));
}

/* latitudo = max ceil(log2 elementa) super intermedios, EXACTE:
 * elementa saturata per magnum recomputantur */
interior i32
_latitudo (
    constans ContractioOrdo* ordo,
              constans i32* dimensiones,
                   Piscina* piscina)
{
    PiscinaNotatio nota      = piscina_notare(piscina);
               i32 latitudo  = ZEPHYRUM;
               i32 s;
               i32 j;

    per (s = ZEPHYRUM; s < ordo->numerus_graduum; s++)
    {
        constans TensorOrdinis* t =
            &ordo->tensores[ordo->gradus[s].effectus];
        i32 bita;

        si (t->elementa != CONTRACTIO_SATURATUM)
        {
            bita = _bita(t->elementa - I);
        }
        alioquin
        {
            Magnus productum = magnus_ex_s64((s64)I);

            per (j = ZEPHYRUM; j < t->numerus; j++)
            {
                productum = magnus_multiplica(productum,
                    magnus_ex_s64((s64)dimensiones[t->indices[j]]),
                    piscina);
            }
            bita = magnus_bitorum(magnus_subtrahe(productum,
                magnus_ex_s64((s64)I), piscina));
        }
        si (bita > latitudo)
        {
            latitudo = bita;
        }
    }
    piscina_reficere(piscina, nota);
    redde latitudo;
}


/* ==================================================
 * Constructio ordinis (communis omnibus methodis)
 * ================================================== */

/* status constructionis: ordo crescens, possessores indicum, vivi */
nomen structura {
    ContractioOrdo* ordo;
              i32* possessores;
              i32* dimensiones;
              i32* vivi;
              i32  numerus_vivorum;
              i32  gradus_facti;
              i64* passus_a;            /* NIHIL: sine consiliis ansae */
              i64* passus_b;
              i64  limes;               /* abicere si multiplicationes > */
} StatusOrdinis;

/* ordo novus cum tensoribus initialibus; consilia: passus pro consiliis
 * ansae (materializatio), aliter simulatio sola */
interior vacuum
_statum_incipere (
    constans ContractioRete* rete,
                        b32  consilia,
                        i64  limes,
                   Piscina* piscina,
             StatusOrdinis* st)
{
    ContractioOrdo* ordo;
               i32  n = xar_numerus(rete->tensores);
               i32  m = xar_numerus(rete->indices);
               i32  j;
               i32  k;

    ordo = (ContractioOrdo*)piscina_allocare(piscina,
        magnitudo(ContractioOrdo));
    ordo->rete                      = rete;
    ordo->initiales                 = n;
    ordo->numerus_graduum           = n > ZEPHYRUM ? n - I : ZEPHYRUM;
    ordo->gradus                    = NIHIL;
    ordo->tensores                  = NIHIL;
    ordo->tabula_saturata           = FALSUM;
    ordo->sumptus.apex              = ZEPHYRUM;
    ordo->sumptus.octeti_officinae  = ZEPHYRUM;
    ordo->sumptus.multiplicationes  = ZEPHYRUM;
    ordo->sumptus.latitudo          = ZEPHYRUM;
    ordo->candidatus                = CONTRACTIO_CANDIDATUS_AVIDUS;
    ordo->numerus_candidatorum      = ZEPHYRUM;
    ordo->candidati                 = NIHIL;
    ordo->candidati_aestimati       = NIHIL;
    st->ordo                        = ordo;
    st->possessores                 = NIHIL;
    st->dimensiones                 = NIHIL;
    st->vivi                        = NIHIL;
    st->numerus_vivorum             = n;
    st->gradus_facti                = ZEPHYRUM;
    st->passus_a                    = NIHIL;
    st->passus_b                    = NIHIL;
    st->limes                       = limes;
    si (n == ZEPHYRUM)
    {
        redde;
    }
    ordo->tensores = (TensorOrdinis*)piscina_allocare(piscina,
        (memoriae_index)(n + ordo->numerus_graduum)
        * magnitudo(TensorOrdinis));
    si (ordo->numerus_graduum > ZEPHYRUM)
    {
        ordo->gradus = (GradusOrdinis*)piscina_allocare(piscina,
            (memoriae_index)ordo->numerus_graduum
            * magnitudo(GradusOrdinis));
    }
    si (m > ZEPHYRUM)
    {
        st->dimensiones = (i32*)piscina_allocare(piscina,
            (memoriae_index)m * magnitudo(i32));
        st->possessores = (i32*)piscina_allocare(piscina,
            (memoriae_index)m * II * magnitudo(i32));
    }
    per (j = ZEPHYRUM; j < m; j++)
    {
        constans IndexRetis* index = (constans IndexRetis*)xar_obtinere(
            rete->indices, j);

        st->dimensiones[j]           = index->dimensio;
        st->possessores[II * j]      = CONTRACTIO_NULLUS;
        st->possessores[II * j + I]  = CONTRACTIO_NULLUS;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans TensorRetis* tr = (constans TensorRetis*)xar_obtinere(
            rete->tensores, k);
        TensorOrdinis* t = &ordo->tensores[k];

        t->numerus   = tr->numerus;
        t->indices   = NIHIL;
        t->elementa  = I;
        t->offset    = CONTRACTIO_SATURATUM;
        si (tr->numerus > ZEPHYRUM)
        {
            t->indices = (i32*)piscina_allocare(piscina,
                (memoriae_index)tr->numerus * magnitudo(i32));
            (vacuum)xar_copiare_ad_tabulam(rete->membra, t->indices,
                tr->initium, tr->numerus);
        }
        per (j = ZEPHYRUM; j < t->numerus; j++)
        {
            i32 index = t->indices[j];

            si (st->possessores[II * index] == CONTRACTIO_NULLUS)
            {
                st->possessores[II * index] = k;
            }
            alioquin
            {
                st->possessores[II * index + I] = k;
            }
            t->elementa = _multiplicare_saturate(t->elementa,
                st->dimensiones[index]);
        }
    }
    si (consilia && m > ZEPHYRUM)
    {
        st->passus_a = (i64*)piscina_allocare(piscina,
            (memoriae_index)m * magnitudo(i64));
        st->passus_b = (i64*)piscina_allocare(piscina,
            (memoriae_index)m * magnitudo(i64));
        per (j = ZEPHYRUM; j < m; j++)
        {
            st->passus_a[j] = ZEPHYRUM;
            st->passus_b[j] = ZEPHYRUM;
        }
    }
    st->vivi = (i32*)piscina_allocare(piscina,
        (memoriae_index)n * magnitudo(i32));
    per (k = ZEPHYRUM; k < n; k++)
    {
        st->vivi[k] = k;
    }
}

/* gradus unus: a < b vivi in c = n + s contrahuntur. FALSUM si
 * multiplicationes summae limitem superant (candidatus abicitur). */
interior b32
_gradum_facere (
    StatusOrdinis* st,
              i32  a,
              i32  b,
          Piscina* piscina)
{
    ContractioOrdo* ordo  = st->ordo;
               i32  s     = st->gradus_facti;
               i32  c     = ordo->initiales + s;
               i64  multiplicationes;
               i32  j;
               i32  k;

    multiplicationes = _contrahere(ordo->tensores, st->possessores,
        st->dimensiones, a, b, c, piscina);
    ordo->gradus[s].sinister  = a;
    ordo->gradus[s].dexter    = b;
    ordo->gradus[s].effectus  = c;
    si (st->passus_a != NIHIL)
    {
        _gradum_parare(ordo->tensores, st->dimensiones, st->passus_a,
            st->passus_b, &ordo->gradus[s], piscina);
    }
    alioquin
    {
        ordo->gradus[s].externi          = ZEPHYRUM;
        ordo->gradus[s].interni          = ZEPHYRUM;
        ordo->gradus[s].dimensiones      = NIHIL;
        ordo->gradus[s].passus_sinistri  = NIHIL;
        ordo->gradus[s].passus_dextri    = NIHIL;
    }
    ordo->sumptus.multiplicationes = _addere_saturate(
        ordo->sumptus.multiplicationes, multiplicationes);
    si (ordo->tensores[c].elementa > ordo->sumptus.apex)
    {
        ordo->sumptus.apex = ordo->tensores[c].elementa;
    }
    k = ZEPHYRUM;
    per (j = ZEPHYRUM; j < st->numerus_vivorum; j++)
    {
        si (st->vivi[j] != a && st->vivi[j] != b)
        {
            st->vivi[k++] = st->vivi[j];
        }
    }
    st->vivi[k++]        = c;
    st->numerus_vivorum  = k;
    st->gradus_facti++;
    redde ordo->sumptus.multiplicationes <= st->limes;
}

interior vacuum
_statum_finire (
    StatusOrdinis* st,
          Piscina* piscina)
{
    _tabulam_disponere(st->ordo, piscina);
    st->ordo->sumptus.latitudo = _latitudo(st->ordo, st->dimensiones,
        piscina);
}

/* producens avidus: clavis genere (AVIDUS: T2; REDUCTIO) */
interior b32
_avide_producere (
    StatusOrdinis* st,
              i32  genus,
          Piscina* piscina)
{
    dum (st->numerus_vivorum > I)
    {
        i32 a;
        i32 b;

        _par_eligere(st->ordo->tensores, st->possessores,
            st->dimensiones,
            st->vivi, st->numerus_vivorum, genus, &a, &b);
        si (!_gradum_facere(st, a, b, piscina))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * Min-fill in graphe linearum (indices vicini si tensorem communem
 * habent); eliminatio indicis = contractio duorum tensorum eius
 * (online), indices communes clausi e graphe statim tolluntur (vicini
 * eorum iam clique sunt: nulla impletio). Candidati per gradum: LXIV
 * vertices (gradus, id) minimi; electio (impletio, gradus, id) aut,
 * cum sorte, fortuita inter aequales (impletio, gradus).
 * ================================================== */

#define IMPLETIO_CANDIDATI LXIV

nomen structura {
    i32* vicini;
    i32  numerus;
    i32  capacitas;
} VicinitasIndicis;

interior vacuum
_vicinum_addere (
    VicinitasIndicis* v,
                 i32  index,
             Piscina* piscina)
{
    si (v->numerus == v->capacitas)
    {
        i32 capacitas = v->capacitas
            == ZEPHYRUM ? VIII : II * v->capacitas;
        i32* nova      = (i32*)piscina_allocare(piscina,
            (memoriae_index)capacitas * magnitudo(i32));
        i32 j;

        per (j = ZEPHYRUM; j < v->numerus; j++)
        {
            nova[j] = v->vicini[j];
        }
        v->vicini     = nova;
        v->capacitas  = capacitas;
    }
    v->vicini[v->numerus++] = index;
}

interior vacuum
_vicinum_tollere (
    VicinitasIndicis* v,
                 i32  index)
{
    i32 j;

    per (j = ZEPHYRUM; j < v->numerus; j++)
    {
        si (v->vicini[j] == index)
        {
            v->vicini[j] = v->vicini[--v->numerus];
            redde;
        }
    }
}

/* clique super vicinos ipsius v (praeter v): margines absentes
 * adduntur; si numerare, solum numerantur (nihil additur) */
interior i32
_impletio (
    VicinitasIndicis* vicinitates,
                 i32* signa,
                 i32* signum,
                 i32  v,
                 b32  addere,
             Piscina* piscina)
{
    VicinitasIndicis* nv        = &vicinitates[v];
                 i32  impletio  = ZEPHYRUM;
                 i32  p;
                 i32  q;

    per (p = ZEPHYRUM; p < nv->numerus; p++)
    {
        i32 x = nv->vicini[p];
        i32 j;

        (*signum)++;
        per (j = ZEPHYRUM; j < vicinitates[x].numerus; j++)
        {
            signa[vicinitates[x].vicini[j]] = *signum;
        }
        per (q = addere ? ZEPHYRUM : p + I; q < nv->numerus; q++)
        {
            i32 y = nv->vicini[q];

            si (y != x && signa[y] != *signum)
            {
                si (addere)
                {
                    _vicinum_addere(&vicinitates[x], y, piscina);
                }
                alioquin
                {
                    impletio++;
                }
            }
        }
    }
    redde impletio;
}

interior vacuum
_verticem_tollere (
    VicinitasIndicis* vicinitates,
                 b32* vivus,
                 i32  v)
{
    i32 j;

    per (j = ZEPHYRUM; j < vicinitates[v].numerus; j++)
    {
        _vicinum_tollere(&vicinitates[vicinitates[v].vicini[j]], v);
    }
    vicinitates[v].numerus  = ZEPHYRUM;
    vivus[v]                = FALSUM;
}

interior b32
_implendo_producere (
    StatusOrdinis* st,
             Sors* sors,
          Piscina* piscina)
{
    constans ContractioRete* rete         = st->ordo->rete;
                        i32  m            = xar_numerus(rete->indices);
                        i32  n            = st->ordo->initiales;
           VicinitasIndicis* vicinitates  = NIHIL;
                        b32* vivus        = NIHIL;
                        i32* signa        = NIHIL;
                        i32  signum       = ZEPHYRUM;
                        i32  reliqui      = m;
                        i32  k;
                        i32  j;

    si (m > ZEPHYRUM)
    {
        vicinitates = (VicinitasIndicis*)piscina_allocare(piscina,
            (memoriae_index)m * magnitudo(VicinitasIndicis));
        vivus   = (b32*)piscina_allocare(piscina, (memoriae_index)m
            * magnitudo(b32));
        signa   = (i32*)piscina_allocare(piscina, (memoriae_index)m
            * magnitudo(i32));
        per (j = ZEPHYRUM; j < m; j++)
        {
            vicinitates[j].vicini     = NIHIL;
            vicinitates[j].numerus    = ZEPHYRUM;
            vicinitates[j].capacitas  = ZEPHYRUM;
            vivus[j]                  = VERUM;
            signa[j]                  = ZEPHYRUM;
        }
        /* margines: paria indicum cuiusque tensoris initialis */
        per (k = ZEPHYRUM; k < n; k++)
        {
            constans TensorOrdinis* t = &st->ordo->tensores[k];
                               i32  p;
                               i32  q;

            per (p = ZEPHYRUM; p < t->numerus; p++)
            {
                i32 x = t->indices[p];

                signum++;
                per (j = ZEPHYRUM; j < vicinitates[x].numerus; j++)
                {
                    signa[vicinitates[x].vicini[j]] = signum;
                }
                per (q = ZEPHYRUM; q < t->numerus; q++)
                {
                    i32 y = t->indices[q];

                    si (y != x && signa[y] != signum)
                    {
                        _vicinum_addere(&vicinitates[x], y, piscina);
                        signa[y] = signum;
                    }
                }
            }
        }
    }

    dum (reliqui > ZEPHYRUM)
    {
        i32 candidati[IMPLETIO_CANDIDATI];
        i32 numerus_candidatorum = ZEPHYRUM;
        i32 v;
        i32 optima_impletio  = ZEPHYRUM;
        i32 optimus_gradus   = ZEPHYRUM;
        i32 aequales         = ZEPHYRUM;
        i32 electio;
        i32 a;
        i32 b;

        /* LXIV vertices vivi (gradus, id) minimi, ordine ascendente */
        per (v = ZEPHYRUM; v < m; v++)
        {
            i32 p;

            si (!vivus[v])
            {
                perge;
            }
            si (   numerus_candidatorum == IMPLETIO_CANDIDATI
                && vicinitates[candidati[numerus_candidatorum
                    - I]].numerus
                   <= vicinitates[v].numerus)
            {
                perge;
            }
            p = numerus_candidatorum < IMPLETIO_CANDIDATI
                ? numerus_candidatorum++ : IMPLETIO_CANDIDATI - I;
            dum (   p > ZEPHYRUM
                 && vicinitates[candidati[p - I]].numerus
                    > vicinitates[v].numerus)
            {
                candidati[p] = candidati[p - I];
                p--;
            }
            candidati[p] = v;
        }

        /* (impletio, gradus) minimi; aequales ordine candidatorum */
        per (j = ZEPHYRUM; j < numerus_candidatorum; j++)
        {
            i32 w = candidati[j];
            i32 f = _impletio(vicinitates, signa, &signum, w, FALSUM,
                piscina);
            i32 g = vicinitates[w].numerus;

            si (   j == ZEPHYRUM || f < optima_impletio
                || (f == optima_impletio && g < optimus_gradus))
            {
                optima_impletio  = f;
                optimus_gradus   = g;
                aequales         = ZEPHYRUM;
            }
            si (f == optima_impletio && g == optimus_gradus)
            {
                candidati[aequales++] = w;
            }
        }
        electio = sors != NIHIL && aequales > I
            ? sors_intra(sors, aequales) : ZEPHYRUM;
        v = candidati[electio];

        /* eliminatio: tensores duo indicis v contrahuntur */
        a = st->possessores[II * v];
        b = st->possessores[II * v + I];
        (vacuum)_impletio(vicinitates, signa, &signum, v, VERUM,
            piscina);
        _verticem_tollere(vicinitates, vivus, v);
        reliqui--;
        si (!_gradum_facere(st, a < b ? a : b, a < b ? b : a, piscina))
        {
            redde FALSUM;
        }
        /* indices communes, nunc clausi: e graphe sine impletione */
        per (j = ZEPHYRUM; j < st->ordo->tensores[a].numerus; j++)
        {
            i32 u   = st->ordo->tensores[a].indices[j];
            i32 o0  = st->possessores[II * u];
            i32 o1  = st->possessores[II * u + I];

            si (   vivus[u] && (o0 == a || o0 == b)
                && (o1 == a || o1 == b))
            {
                _verticem_tollere(vicinitates, vivus, u);
                reliqui--;
            }
        }
    }

    /* reliqui tensores (partes disiunctae, scalares): clavis T2 */
    redde _avide_producere(st, CONTRACTIO_CANDIDATUS_AVIDUS, piscina);
}

/* candidatus k: producere et finire in statu novo */
interior b32
_candidatum_currere (
        constans ContractioRete* rete,
                            i32  k,
    constans ContractioOptiones* optiones,
                            b32  consilia,
                            i64  limes,
                        Piscina* piscina,
                  StatusOrdinis* st)
{
     b32 bene;
    Sors sors;

    _statum_incipere(rete, consilia, limes, piscina, st);
    si (   k == CONTRACTIO_CANDIDATUS_AVIDUS
        || k == CONTRACTIO_CANDIDATUS_REDUCTIO)
    {
        bene = _avide_producere(st, k, piscina);
    }
    alioquin si (k == CONTRACTIO_CANDIDATUS_IMPLETIO)
    {
        bene = _implendo_producere(st, NIHIL, piscina);
    }
    alioquin
    {
        sors_seminare(&sors, optiones->semen,
            (i64)(k - CONTRACTIO_CANDIDATUS_FORTUITUS));
        bene = _implendo_producere(st, &sors, piscina);
    }
    si (bene)
    {
        _statum_finire(st, piscina);
    }
    redde bene;
}

/* clavis electionis: aptus (octeti <= limes) ante non aptum; inter
 * aptos (multiplicationes, octeti, k); inter non aptos (octeti, k) */
interior b32
_melior (
    constans ContractioSumptus* s,
                           i32  k,
    constans ContractioSumptus* optimus,
                           i32  optimus_k,
                           i64  octeti_maximi)
{
    b32 aptus          = s->octeti_officinae <= octeti_maximi;
    b32 optimus_aptus  = optimus->octeti_officinae <= octeti_maximi;

    si (aptus != optimus_aptus)
    {
        redde aptus;
    }
    si (aptus && s->multiplicationes != optimus->multiplicationes)
    {
        redde s->multiplicationes < optimus->multiplicationes;
    }
    si (s->octeti_officinae != optimus->octeti_officinae)
    {
        redde s->octeti_officinae < optimus->octeti_officinae;
    }
    redde k < optimus_k;
}

ContractioOrdo*
contractio_ordinare (
                ContractioRete* rete,
    constans ContractioOptiones* optiones,
                       Piscina* piscina)
{
        StatusOrdinis  st;
    ContractioSumptus* candidati;
                  b32* aestimati;
                  i32* optimi_gradus;
                  i32  numerus;
                  i32  optimus_k = ZEPHYRUM;
                  i32  k;
                  i32  s;

    si (   rete == NIHIL || optiones == NIHIL || piscina == NIHIL
        || (   optiones->methodus != CONTRACTIO_AVIDUS
            && optiones->methodus != CONTRACTIO_OPTIMUS)
        || (   optiones->methodus == CONTRACTIO_OPTIMUS
            && (   optiones->conatus > CONTRACTIO_CONATUS_MAXIMI
                || optiones->candidatus > CONTRACTIO_CANDIDATUS_IMPLETIO
                   + optiones->conatus))
        || !contractio_rete_clausum(rete))
    {
        redde NIHIL;
    }
    rete->ordinatum = VERUM;

    si (optiones->methodus == CONTRACTIO_AVIDUS)
    {
        (vacuum)_candidatum_currere(rete, CONTRACTIO_CANDIDATUS_AVIDUS,
            optiones, VERUM, CONTRACTIO_SATURATUM, piscina, &st);
        st.ordo->numerus_candidatorum  = I;
        st.ordo->candidati             = (ContractioSumptus*)
            piscina_allocare(piscina, magnitudo(ContractioSumptus));
        st.ordo->candidati_aestimati   = (b32*)piscina_allocare(piscina,
            magnitudo(b32));
        st.ordo->candidati[ZEPHYRUM]            = st.ordo->sumptus;
        st.ordo->candidati_aestimati[ZEPHYRUM]  = VERUM;
        redde st.ordo;
    }

    /* OPTIMUS: candidati in regione refecta, gradus victoris servati */
    numerus        = CONTRACTIO_CANDIDATUS_IMPLETIO + optiones->conatus;
    candidati      = (ContractioSumptus*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(ContractioSumptus));
    aestimati      = (b32*)piscina_allocare(piscina,
        (memoriae_index)numerus * magnitudo(b32));
    optimi_gradus  = (i32*)piscina_allocare(piscina,
        (memoriae_index)(II * xar_numerus(rete->tensores) + I)
        * magnitudo(i32));
    per (k = I; k <= numerus; k++)
    {
        PiscinaNotatio nota;
                   i64 limes = CONTRACTIO_SATURATUM;

        aestimati[k - I] = FALSUM;
        si (   optiones->candidatus != CONTRACTIO_CANDIDATUS_OMNES
            && optiones->candidatus != k)
        {
            perge;
        }
        si (   optimus_k != ZEPHYRUM
            && candidati[optimus_k - I].octeti_officinae
               <= optiones->octeti_maximi)
        {
            limes = candidati[optimus_k - I].multiplicationes;
        }
        nota = piscina_notare(piscina);
        si (_candidatum_currere(rete, k, optiones, FALSUM, limes,
            piscina,
            &st))
        {
            candidati[k - I] = st.ordo->sumptus;
            aestimati[k - I] = VERUM;
            si (   optimus_k == ZEPHYRUM
                || _melior(&candidati[k - I], k,
                       &candidati[optimus_k - I], optimus_k,
                       optiones->octeti_maximi))
            {
                optimus_k = k;
                per (s = ZEPHYRUM; s < st.ordo->numerus_graduum; s++)
                {
                    optimi_gradus[II * s] =
                        st.ordo->gradus[s].sinister;
                    optimi_gradus[II * s + I] =
                        st.ordo->gradus[s].dexter;
                }
            }
        }
        piscina_reficere(piscina, nota);
        si (   k == CONTRACTIO_CANDIDATUS_AVIDUS && aestimati[ZEPHYRUM]
            && optiones->candidatus == CONTRACTIO_CANDIDATUS_OMNES
            && candidati[ZEPHYRUM].multiplicationes < optiones->limen)
        {
            /* ordo avidus iam vilis: ceteri non aestimantur */
            per (k = k + I; k <= numerus; k++)
            {
                aestimati[k - I] = FALSUM;
            }
            frange;
        }
    }

    /* victor materializatur: gradus iterantur cum consiliis ansae */
    _statum_incipere(rete, VERUM, CONTRACTIO_SATURATUM, piscina, &st);
    per (s = ZEPHYRUM; s < st.ordo->numerus_graduum; s++)
    {
        (vacuum)_gradum_facere(&st, optimi_gradus[II * s],
            optimi_gradus[II * s + I], piscina);
    }
    _statum_finire(&st, piscina);
    st.ordo->candidatus            = optimus_k;
    st.ordo->numerus_candidatorum  = numerus;
    st.ordo->candidati             = candidati;
    st.ordo->candidati_aestimati   = aestimati;
    redde st.ordo;
}

ContractioSumptus
contractio_sumptus (
    constans ContractioOrdo* ordo)
{
    ContractioSumptus nullus;

    si (ordo == NIHIL)
    {
        nullus.apex              = ZEPHYRUM;
        nullus.octeti_officinae  = ZEPHYRUM;
        nullus.multiplicationes  = ZEPHYRUM;
        nullus.latitudo          = ZEPHYRUM;
        redde nullus;
    }
    redde ordo->sumptus;
}


/* ==================================================
 * Inspectio ordinis
 * ================================================== */

i32
contractio_ordo_gradus_numerus (
    constans ContractioOrdo* ordo)
{
    redde ordo == NIHIL ? ZEPHYRUM : ordo->numerus_graduum;
}

b32
contractio_ordo_gradus (
    constans ContractioOrdo* ordo,
                        i32  k,
                        i32* sinister,
                        i32* dexter,
                        i32* effectus)
{
    si (ordo == NIHIL || k >= ordo->numerus_graduum || sinister == NIHIL
        || dexter == NIHIL || effectus == NIHIL)
    {
        redde FALSUM;
    }
    *sinister  = ordo->gradus[k].sinister;
    *dexter    = ordo->gradus[k].dexter;
    *effectus  = ordo->gradus[k].effectus;
    redde VERUM;
}

b32
contractio_ordo_sedes (
    constans ContractioOrdo* ordo,
                        i32  tensor,
                        i64* offset,
                        i64* elementa)
{
    si (   ordo == NIHIL || offset == NIHIL || elementa == NIHIL
        || ordo->tabula_saturata || tensor < ordo->initiales
        || tensor - ordo->initiales >= ordo->numerus_graduum)
    {
        redde FALSUM;
    }
    *offset    = ordo->tensores[tensor].offset;
    *elementa  = ordo->tensores[tensor].elementa;
    redde VERUM;
}

b32
contractio_ordo_indices (
    constans ContractioOrdo*  ordo,
                        i32   tensor,
               constans i32** indices,
                        i32*  numerus)
{
    si (   ordo   == NIHIL || indices == NIHIL || numerus == NIHIL
        || tensor >= ordo->initiales + ordo->numerus_graduum)
    {
        redde FALSUM;
    }
    *indices = ordo->tensores[tensor].indices;
    *numerus = ordo->tensores[tensor].numerus;
    redde VERUM;
}

i32
contractio_ordo_candidatus (
    constans ContractioOrdo* ordo)
{
    redde ordo == NIHIL ? ZEPHYRUM : ordo->candidatus;
}

b32
contractio_ordo_candidati (
    constans ContractioOrdo* ordo,
                        i32  k,
          ContractioSumptus* simulatus)
{
    si (   ordo == NIHIL || simulatus == NIHIL || k == ZEPHYRUM
        || k > ordo->numerus_candidatorum
        || !ordo->candidati_aestimati[k - I])
    {
        redde FALSUM;
    }
    *simulatus = ordo->candidati[k - I];
    redde VERUM;
}


/* ==================================================
 * Exsecutio
 * ================================================== */

/* Multiplicationes non saturatae (< 2^64) implicant quemque gradum
 * habere < 64 indices dimensionis >= 2 in unione: numeratores
 * exsecutionis (T3) in acervo fixo capiunt - SI indices dimensionis
 * 1 omittuntur (eorum numerus non finitur: 100 indices dimensionis 1
 * communes dant multiplicationem unam). */
ContractioOfficina*
contractio_officina_creare (
    constans ContractioOrdo* ordo,
                       i64  octeti_maximi,
                   Piscina* piscina)
{
     ContractioOfficina* officina;
                    i64  octeti;

    si (ordo == NIHIL || piscina == NIHIL)
    {
        redde NIHIL;
    }
    /* limes 2^62: piscina mensuram prope 2^64 non tuto tractat
     * (offset + mensura exundat); nullum rete notum huc pervenit */
    octeti = ordo->sumptus.octeti_officinae;
    si (   octeti                         == CONTRACTIO_SATURATUM
        || ordo->sumptus.multiplicationes == CONTRACTIO_SATURATUM
        || octeti > ((i64)I << LXII)
        || octeti > octeti_maximi)
    {
        redde NIHIL;
    }
    officina = (ContractioOfficina*)piscina_conari_allocare(piscina,
        magnitudo(ContractioOfficina));
    si (officina == NIHIL)
    {
        redde NIHIL;
    }
    officina->ordo    = ordo;
    officina->tabula  = NIHIL;
    si (octeti > ZEPHYRUM)
    {
        officina->tabula = (i32*)piscina_conari_allocare(piscina,
            (memoriae_index)octeti);
        si (officina->tabula == NIHIL)
        {
            redde NIHIL;
        }
    }
    redde officina;
}

/* numeratores odometri: < 64 indices dimensionis >= 2 per gradum
 * (multiplicationes non saturatae, vide supra) */
#define CONTRACTIO_NUMERATORES LXIV

/* odometrum super indices consilii [primus, finis): assignatio
 * proxima, offsets in operandis renovati. FALSUM si exhaustum -
 * numeratores tunc nulli et offsets ad initium redierunt. */
interior b32
_numeratores_promovere (
    constans GradusOrdinis* g,
                       i32* numeratores,
                       i32  primus,
                       i32  finis,
                       i64* offset_a,
                       i64* offset_b)
{
    i32 k = finis;

    dum (k > primus)
    {
        k--;
        numeratores[k]++;
        *offset_a += g->passus_sinistri[k];
        *offset_b += g->passus_dextri[k];
        si (numeratores[k] < g->dimensiones[k])
        {
            redde VERUM;
        }
        *offset_a       -= g->passus_sinistri[k] * g->dimensiones[k];
        *offset_b       -= g->passus_dextri[k] * g->dimensiones[k];
        numeratores[k]  = ZEPHYRUM;
    }
    redde FALSUM;
}

/* gradus calidus: pro quoque elemento effectus summa productorum in
 * accumulatore duorum verborum (humile + numerus portationum: exacta
 * pro omni longitudine), reductio semel: (portatio * 2^64 + humile)
 * mod n, residuum_verbi = 2^64 mod n */
interior vacuum
_gradum_modulo (
    constans GradusOrdinis* g,
              constans i32* sinister,
              constans i32* dexter,
                       i32* effectus,
                       i64  elementa,
                       i64  modulus,
                       i64  residuum_verbi)
{
    i32 numeratores[CONTRACTIO_NUMERATORES];
    i32 summus  = g->externi + g->interni;
    i64 oa      = ZEPHYRUM;
    i64 ob      = ZEPHYRUM;
    i64 o;
    i32 k;

    per (k = ZEPHYRUM; k < summus; k++)
    {
        numeratores[k] = ZEPHYRUM;
    }
    per (o = ZEPHYRUM; o < elementa; o++)
    {
        i64 humile    = ZEPHYRUM;
        i64 portatio  = ZEPHYRUM;

        si (g->interni == ZEPHYRUM)
        {
            humile = (i64)sinister[oa] * (i64)dexter[ob];
        }
        alioquin
        {
            /* index communis ultimus in ansa stricta, ceteri per
             * odometrum */
            i32 ultimus  = summus - I;
            i32 d        = g->dimensiones[ultimus];
            i64 pa       = g->passus_sinistri[ultimus];
            i64 pb       = g->passus_dextri[ultimus];
            i64 ia       = oa;
            i64 ib       = ob;

            fac
            {
                i64 xa = ia;
                i64 xb = ib;
                i32 j;

                per (j = ZEPHYRUM; j < d; j++)
                {
                    i64 productum = (i64)sinister[xa] * (i64)dexter[xb];

                    humile += productum;
                    si (humile < productum)
                    {
                        portatio++;
                    }
                    xa += pa;
                    xb += pb;
                }
            }
            dum (_numeratores_promovere(g, numeratores, g->externi,
                ultimus, &ia, &ib));
        }
        effectus[o] = (i32)(((portatio % modulus) * residuum_verbi
            % modulus
            + humile % modulus) % modulus);
        (vacuum)_numeratores_promovere(g, numeratores, ZEPHYRUM,
            g->externi, &oa, &ob);
    }
}

/* operandum calidum: initialis ex valoribus, intermedius ex tabula */
interior constans i32*
_operandum_modulo (
       constans ContractioOrdo* ordo,
    constans ContractioOfficina* officina,
         constans i32* constans* valores,
                            i32  tensor)
{
    si (tensor < ordo->initiales)
    {
        redde valores[tensor];
    }
    redde officina->tabula + ordo->tensores[tensor].offset;
}

b32
contractio_exsequi_modulo (
      constans ContractioRete*  rete,
      constans ContractioOrdo*  ordo,
         ContractioOfficina*    officina,
    constans i32* constans*     valores,
                        i32     modulus,
                        i32*    exitus)
{
    i64 residuum_verbi;
    i32 n;
    i32 s;
    i32 k;

    si (   rete           == NIHIL || ordo == NIHIL || officina == NIHIL
        || exitus         == NIHIL || ordo->rete != rete
        || officina->ordo != ordo || modulus < II)
    {
        redde FALSUM;
    }
    n = ordo->initiales;
    si (n == ZEPHYRUM)
    {
        *exitus = I;
        redde VERUM;
    }
    si (valores == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < n; k++)
    {
        si (valores[k] == NIHIL)
        {
            redde FALSUM;
        }
    }
    si (ordo->numerus_graduum == ZEPHYRUM)
    {
        *exitus = valores[ZEPHYRUM][ZEPHYRUM] % modulus;
        redde VERUM;
    }
    residuum_verbi = (CONTRACTIO_SATURATUM % (i64)modulus + I)
        % (i64)modulus;
    per (s = ZEPHYRUM; s < ordo->numerus_graduum; s++)
    {
        constans GradusOrdinis* g = &ordo->gradus[s];
        constans TensorOrdinis* c = &ordo->tensores[g->effectus];

        _gradum_modulo(g,
            _operandum_modulo(ordo, officina, valores, g->sinister),
            _operandum_modulo(ordo, officina, valores, g->dexter),
            officina->tabula + c->offset, c->elementa, (i64)modulus,
            residuum_verbi);
    }
    *exitus = officina->tabula[ordo->tensores[ordo->gradus[
        ordo->numerus_graduum - I].effectus].offset];
    redde VERUM;
}

/* elementum anuli ad indicem in tabula elementorum */
interior constans vacuum*
_elementum (
    constans vacuum* tabula,
                i64  index,
     memoriae_index  mensura)
{
    redde (constans character*)tabula + (memoriae_index)index * mensura;
}

/* gradus referentiae: eadem ansa, operationes anuli; summa cuiusque
 * elementi in scrinio, transcripta in servatam, scrinium refectum */
interior b32
_gradum_anulo (
    constans GradusOrdinis* g,
           constans vacuum* sinister,
           constans vacuum* dexter,
                 character* effectus,
                       i64  elementa,
           constans Anulus* anulus,
                   Piscina* servata,
                   Piscina* scrinium,
                 character* officium)
{
    memoriae_index mensura = anulus->mensura;
               i32 numeratores[CONTRACTIO_NUMERATORES];
               i32 summus  = g->externi + g->interni;
               i64 oa      = ZEPHYRUM;
               i64 ob      = ZEPHYRUM;
               i64 o;
               i32 k;

    per (k = ZEPHYRUM; k < summus; k++)
    {
        numeratores[k] = ZEPHYRUM;
    }
    per (o = ZEPHYRUM; o < elementa; o++)
    {
         PiscinaNotatio  nota       = piscina_notare(scrinium);
              character* summa      = officium;
              character* altera     = officium + mensura;
              character* productum  = officium + II * mensura;
                    i64  ia         = oa;
                    i64  ib         = ob;

        anulus->nullum(anulus, summa);
        fac
        {
            character* t;

            si (   !anulus->multiplica(anulus,
                       _elementum(sinister, ia, mensura),
                       _elementum(dexter, ib, mensura), scrinium,
                       productum)
                || !anulus->adde(anulus, summa, productum, scrinium,
                       altera))
            {
                piscina_reficere(scrinium, nota);
                redde FALSUM;
            }
            t       = summa;
            summa   = altera;
            altera  = t;
        }
        dum (_numeratores_promovere(g, numeratores, g->externi, summus,
            &ia, &ib));
        anulus->transcribe(anulus, summa, servata,
            effectus + (memoriae_index)o * mensura);
        piscina_reficere(scrinium, nota);
        (vacuum)_numeratores_promovere(g, numeratores, ZEPHYRUM,
            g->externi, &oa, &ob);
    }
    redde VERUM;
}

/* intermedii in 'servata', operationes in 'scrinio'; FALSUM si
 * allocatio aut operatio anuli deficit */
interior b32
_exsequi_anulo_intra (
           constans ContractioOrdo* ordo,
         constans vacuum* constans* valores,
                   constans Anulus* anulus,
                           Piscina* servata,
                           Piscina* scrinium,
                           Piscina* piscina,
                            vacuum* exitus)
{
     memoriae_index   mensura = anulus->mensura;
    constans vacuum** data;
          character*  officium;
                i32   totales = ordo->initiales + ordo->numerus_graduum;
                i32   s;
                i32   k;

    data = (constans vacuum**)piscina_conari_allocare(servata,
        (memoriae_index)totales * magnitudo(constans vacuum*));
    officium = (character*)piscina_conari_allocare(servata,
        III * mensura);
    si (data == NIHIL || officium == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ordo->initiales; k++)
    {
        data[k] = valores[k];
    }
    per (s = ZEPHYRUM; s < ordo->numerus_graduum; s++)
    {
        constans GradusOrdinis* g = &ordo->gradus[s];
                           i64  elementa =
                               ordo->tensores[g->effectus].elementa;
                     character* effectus;

        /* limes 2^62 octetorum, ut officina: piscina prope 2^64
         * exundat (tabularium 01M5YW4S...) */
        si (elementa > ((i64)I << LXII) / (i64)mensura)
        {
            redde FALSUM;
        }
        effectus = (character*)piscina_conari_allocare(servata,
            (memoriae_index)elementa * mensura);
        si (   effectus == NIHIL
            || !_gradum_anulo(g, data[g->sinister], data[g->dexter],
                   effectus, elementa, anulus, servata, scrinium,
                   officium))
        {
            redde FALSUM;
        }
        data[g->effectus] = effectus;
    }
    anulus->transcribe(anulus,
        data[ordo->gradus[ordo->numerus_graduum - I].effectus], piscina,
        exitus);
    redde VERUM;
}

b32
contractio_exsequi_anulo (
      constans ContractioRete*  rete,
      constans ContractioOrdo*  ordo,
    constans vacuum* constans*  valores,
           constans Anulus*     anulus,
                    Piscina*    piscina,
                     vacuum*    exitus)
{
     Piscina* servata;
     Piscina* scrinium;
         b32  bene;
         i32  k;

    si (   rete == NIHIL || ordo == NIHIL || anulus == NIHIL
        || piscina == NIHIL || exitus == NIHIL || ordo->rete != rete
        || ordo->sumptus.multiplicationes == CONTRACTIO_SATURATUM
        || ordo->sumptus.apex == CONTRACTIO_SATURATUM)
    {
        redde FALSUM;
    }
    si (ordo->initiales == ZEPHYRUM)
    {
        anulus->unum(anulus, piscina, exitus);
        redde VERUM;
    }
    si (valores == NIHIL)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < ordo->initiales; k++)
    {
        si (valores[k] == NIHIL)
        {
            redde FALSUM;
        }
    }
    si (ordo->numerus_graduum == ZEPHYRUM)
    {
        anulus->transcribe(anulus, valores[ZEPHYRUM], piscina, exitus);
        redde VERUM;
    }
    servata   = piscina_generare_dynamicum("contractio_servata", 4096);
    scrinium  = piscina_generare_dynamicum("contractio_scrinium", 4096);
    bene     = servata != NIHIL && scrinium != NIHIL
        && _exsequi_anulo_intra(ordo, valores, anulus, servata,
        scrinium,
               piscina, exitus);
    si (servata != NIHIL)
    {
        piscina_destruere(servata);
    }
    si (scrinium != NIHIL)
    {
        piscina_destruere(scrinium);
    }
    redde bene;
}
