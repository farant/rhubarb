/* oratio_reticulum.c - instrumentum reticuli partitionum (T35 d, T36 b;
 * vide oratio_reticulum.h). Ordo operum: legere, optiones iudicare,
 * ordines eligere, partes struere, COMPUTARE (regiones piscinae
 * notatae) - et tum demum scribere: aedificator intra regionem
 * crescens memoriam reficiendam teneret. Tabula infimorum: series et
 * numeri EXTRA regionem allocati (tabula dispersa quoque ante notam),
 * infimum et aestimatio intra. */

#include "oratio_reticulum.h"
#include "partitio.h"
#include "partitio_aestimatio.h"
#include "chorda_aedificator.h"
#include "tabula_dispersa.h"
#include <string.h>

#define RETICULUM_PROFUNDITAS_MAXIMA IV
#define NUMERUS_TITULORUM(t) ((i32)(magnitudo(t) / magnitudo((t)[0])))

nomen structura {
    chorda* campi;     /* 0 via, 1 genus, 2.. columnae ordine capitis */
       i32  numerus;
       i32  linea;
} OrdoReticuli;

/* infimum k columnarum (T36 b): series indicum notarum ascendens;
 * incrementum = recti minus optimum serierum_minorum (k - I) */
nomen structura {
    i32  profunditas;
    i32* columnae;      /* [profunditas] indices notarum */
    i32  index;         /* in c->infima (valor tabulae dispersae) */
    i32  greges;
    i32  recti;
    s32  incrementum;
    i32  cadentes;      /* sortes sub basi */
    i32  inaestimati;
} InfimumReticuli;

nomen structura {
     s32  columna;       /* -I: nulla restat */
     i32  recti;
     s32  lucrum;
     b32  cadit_ulla;
     b32* cadit;         /* per sortem */
} FinisCatenae;

nomen structura {
                             Piscina*  piscina;
    constans OratioReticulumOptiones*  optiones;
                  constans character*  via;
                              chorda*  tituli;
                                 i32   numerus_titulorum;
                                 Xar*  ordines;
                        OrdoReticuli** manentes;
                                 i32   numerus;
                                 s32   index_auri;
                                 s32   index_sortium;
                              chorda*  valores_sortium; /* [ordo]; -I */
                                 i32*  notae;
                                 i32   numerus_notarum;
                                 b32*  gradus_columnae;
                              chorda** valores;
                              chorda   gradus_nullus;
                              chorda   gradus_unus;
                              chorda   gradus_plures;
                            Partitio** partes;
                            Partitio*  aurum;
                            Partitio*  sortes;
                            Partitio*  una;
                  PartitioAestimatio** aestimationes;
                   PartitioReticulum*  reticulum;
                      PartitioCatena*  catena_vetans;
                      PartitioCatena*  catena_libera;
                        FinisCatenae   finis_vetans;
                        FinisCatenae   finis_libera;
                     InfimumReticuli*  infima;
                                 i32   numerus_infimorum;
                                 i32*  selecti;          /* prima */
                                 i32   numerus_selectorum;
                                 b32*  constantes;       /* [nota] */
                                 i32   numerus_constantium;
                                 i32*  reticulum_index;  /* -> nota */
                                 i32   numerus_reticuli;
                   constans Partitio*  semen;
                                 i32*  semen_columnae;   /* notae */
                                 i32   numerus_seminis;
                                 i32   lucrum_minimum;
                                 i32*  ordo;
} ContextusReticuli;

hic_manens constans character* constans TITULI_COLUMNAE[] = {
    "columna", "greges", "ordines", "recti", "basis", "lucrum",
    "puritas", "puritas-intra", "inaestimati", "mutabiles",
    "cadentes-basi", "inaestimabilis", "greges-captivi",
    "ordines-captivi"
};
hic_manens constans character* constans TITULI_SORTIS[] = {
    "columna", "sors", "ordines", "recti", "basis", "inaestimati"
};
hic_manens constans character* constans TITULI_CATENAE[] = {
    "vetans", "gradus", "columna", "greges", "recti", "lucrum",
        "cadentes-gradu"
};
hic_manens constans character* constans TITULI_CONDICIONIS[] = {
    "gradus", "columna", "sors", "recti", "priores"
};
hic_manens constans character* constans TITULI_INFIMI[] = {
    "profunditas", "columnae", "greges", "recti", "incrementum",
        "cadentes-basi", "inaestimati"
};
hic_manens constans character* constans TITULI_GREGIS[] = {
    "partitio", "grex", "ordines", "aurum-maximum",
        "aurum-maximum-ordines",
    "puritas", "captivus", "habitus", "mutabilis",
        "sortes-suffragantes",
    "retenti", "recti"
};
hic_manens constans character* constans TITULI_GREGIS_SORTIS[] = {
    "partitio", "grex", "sors", "disciplina", "suffragium", "margo",
    "retenti", "recti"
};
hic_manens constans character* constans TITULI_TEGMINIS[] = {
    "subtilior", "crassior"
};
hic_manens constans character* constans TITULI_DUPLICIS[] = {
    "columna", "aequalis"
};


/* ==================================================
 * Adiutores chordarum et scriptionis
 * ================================================== */

interior chorda
_vacua (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior b32
_aequalis_literis (
                chorda  c,
    constans character* literae)
{
    i32 longitudo;

    si (literae == NIHIL)
    {
        redde FALSUM;
    }
    longitudo = (i32)strlen(literae);
    redde (b32)(c.mensura == longitudo
        && (longitudo == ZEPHYRUM
            || memcmp(c.datum, literae, (size_t)longitudo)
                == ZEPHYRUM));
}

interior b32
_aequales (
    chorda a,
    chorda b)
{
    redde (b32)(a.mensura == b.mensura
        && (a.mensura == ZEPHYRUM
            || memcmp(a.datum, b.datum, (size_t)a.mensura)
                == ZEPHYRUM));
}

interior b32
_praefixum_auri (
    chorda c)
{
    redde (b32)(c.mensura >= VI
        && memcmp(c.datum, "aurum-", (size_t)VI) == ZEPHYRUM);
}

interior b32
_numericum (
    chorda c)
{
    i32 i;

    si (c.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < c.mensura; i++)
    {
        si (c.datum[i] < '0' || c.datum[i] > '9')
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior i32
_numerus_listae (
    constans Xar* lista)
{
    redde lista != NIHIL ? xar_numerus(lista) : ZEPHYRUM;
}

interior vacuum
_scribe_literas (
     ChordaAedificator* a,
    constans character* literae)
{
    (vacuum)chorda_aedificator_appendere_literis(a, literae);
}

interior vacuum
_scribe_chordam (
     ChordaAedificator* a,
                chorda  c)
{
    si (c.mensura > ZEPHYRUM)
    {
        (vacuum)chorda_aedificator_appendere_chorda(a, c);
    }
}

interior vacuum
_scribe_numerum (
     ChordaAedificator* a,
                   i32  numerus)
{
    (vacuum)chorda_aedificator_appendere_i32(a, numerus);
}

interior vacuum
_scribe_integrum (
     ChordaAedificator* a,
                   s32  numerus)
{
    (vacuum)chorda_aedificator_appendere_s32(a, numerus);
}

interior vacuum
_scribe_signatum (
     ChordaAedificator* a,
                   s32  numerus)
{
    si (numerus >= ZEPHYRUM)
    {
        (vacuum)chorda_aedificator_appendere_character(a, '+');
    }
    (vacuum)chorda_aedificator_appendere_s32(a, numerus);
}

interior vacuum
_scribe_tabulam (
    ChordaAedificator* a)
{
    (vacuum)chorda_aedificator_appendere_character(a, '\t');
}

interior vacuum
_scribe_lineam_novam (
    ChordaAedificator* a)
{
    (vacuum)chorda_aedificator_appendere_character(a, '\n');
}

interior ChordaAedificator*
_causa (
               Piscina* piscina,
    constans character* textus)
{
    ChordaAedificator* a;

    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    _scribe_literas(a, "reticulum: ");
    _scribe_literas(a, textus);
    redde a;
}

interior OratioReticulumExitus
_recusatio (
    ChordaAedificator* a)
{
    OratioReticulumExitus exitus;

    exitus.iudicatum = FALSUM;
    exitus.causa = a
        != NIHIL ? chorda_aedificator_finire(a) : _vacua();
    exitus.relatio = _vacua();
    redde exitus;
}

/* campus index lineae (sine allocatione); vacuus si deest */
interior chorda
_campum_legere (
    chorda textus,
       i32 index)
{
    chorda c        = _vacua();
       i32 initium  = ZEPHYRUM;
       i32 k        = ZEPHYRUM;
       i32 i;

    per (i = ZEPHYRUM; i <= textus.mensura; i++)
    {
        si (i == textus.mensura || textus.datum[i] == '\t')
        {
            si (k == index)
            {
                c.datum    = textus.datum + initium;
                c.mensura  = i - initium;
                redde c;
            }
            k        = k + I;
            initium  = i + I;
        }
    }
    redde c;
}

/* campi lineae per tabulas; numerus = tabulae + I */
interior chorda*
_campos_findere (
    Piscina* piscina,
     chorda  textus,
        i32* numerus)
{
    chorda* campi;
       i32  tabulae  = ZEPHYRUM;
       i32  initium  = ZEPHYRUM;
       i32  k        = ZEPHYRUM;
       i32  i;

    per (i = ZEPHYRUM; i < textus.mensura; i++)
    {
        si (textus.datum[i] == '\t')
        {
            tabulae = tabulae + I;
        }
    }
    campi = (chorda*)piscina_allocare(piscina,
        (memoriae_index)(tabulae + I) * magnitudo(chorda));
    si (campi == NIHIL)
    {
        *numerus = ZEPHYRUM;
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i <= textus.mensura; i++)
    {
        si (i == textus.mensura || textus.datum[i] == '\t')
        {
            campi[k].datum    = textus.datum + initium;
            campi[k].mensura  = i - initium;
            k                 = k + I;
            initium           = i + I;
        }
    }
    *numerus = tabulae + I;
    redde campi;
}

interior s32
_index_tituli (
    constans ContextusReticuli* c,
                        chorda  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < c->numerus_titulorum; i++)
    {
        si (_aequales(c->tituli[i], titulus))
        {
            redde (s32)i;
        }
    }
    redde (s32)-I;
}

interior s32
_index_literis (
    constans ContextusReticuli* c,
            constans character* literae)
{
    i32 i;

    per (i = ZEPHYRUM; i < c->numerus_titulorum; i++)
    {
        si (_aequalis_literis(c->tituli[i], literae))
        {
            redde (s32)i;
        }
    }
    redde (s32)-I;
}

interior chorda
_notae_titulus (
    constans ContextusReticuli* c,
                           i32  k)
{
    redde c->tituli[c->notae[k]];
}

interior chorda
_sortis_titulus (
    constans ContextusReticuli* c,
                           i32  s)
{
    redde c->valores_sortium[c->sortes->primus[s]];
}

interior chorda
_auri_titulus (
    constans ContextusReticuli* c,
                           i32  aureus)
{
    redde c->valores[c->index_auri][c->aurum->primus[aureus]];
}

/* sortes quarum recti sub basi */
interior i32
_cadentes_basis (
    constans PartitioAestimatio* e)
{
    i32 s;
    i32 cadentes = ZEPHYRUM;

    per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
    {
        si (e->sortes[s].recti < e->sortes[s].basis)
        {
            cadentes = cadentes + I;
        }
    }
    redde cadentes;
}

interior i32
_puritas (
    constans AestimatioSortis* summa)
{
    redde summa->ordines > ZEPHYRUM
        ? (i32)((i64)summa->recti * (i64)M / (i64)summa->ordines)
        : ZEPHYRUM;
}


/* ==================================================
 * I. Legere
 * ================================================== */

interior ChordaAedificator*
_legere (
    ContextusReticuli* c,
               chorda  tsv)
{
    i32 initium  = ZEPHYRUM;
    i32 linea    = ZEPHYRUM;

    c->ordines = xar_creare(c->piscina, (i32)magnitudo(OrdoReticuli));
    si (c->ordines == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    dum (initium < tsv.mensura)
    {
         chorda  textus;
         chorda  secundus;
         chorda* campi;
            i32  numerus;
            i32  finis = initium;

        dum (finis < tsv.mensura && tsv.datum[finis] != '\n')
        {
            finis = finis + I;
        }
        linea           = linea + I;
        textus.datum    = tsv.datum + initium;
        textus.mensura  = finis - initium;
        initium         = finis + I;
        si (textus.mensura == ZEPHYRUM || textus.datum[ZEPHYRUM] == '#')
        {
            perge;
        }
        secundus = _campum_legere(textus, I);
        si (   _aequalis_literis(secundus, "COLUMNAE")
            && _aequalis_literis(_campum_legere(textus, II),
                c->optiones->genus))
        {
            campi = _campos_findere(c->piscina, textus, &numerus);
            si (campi == NIHIL)
            {
                redde _causa(c->piscina, "memoria deficit");
            }
            si (c->tituli == NIHIL)
            {
                c->tituli             = campi + III;
                c->numerus_titulorum  = numerus - III;
            }
            alioquin
            {
                b32 idem = (b32)(numerus - III == c->numerus_titulorum);
                i32 k;

                per (k = ZEPHYRUM; idem
                    && k < c->numerus_titulorum; k++)
                {
                    si (!_aequales(campi[III + k], c->tituli[k]))
                    {
                        idem = FALSUM;
                    }
                }
                si (!idem)
                {
                    ChordaAedificator* a = _causa(c->piscina,
                        "ordo COLUMNAE discrepans: linea ");

                    _scribe_numerum(a, linea);
                    redde a;
                }
            }
        }
        alioquin si (_aequalis_literis(secundus, c->optiones->genus))
        {
            OrdoReticuli* ordo;

            si (c->tituli == NIHIL)
            {
                ChordaAedificator* a = _causa(c->piscina,
                    "ordo ante COLUMNAE: linea ");

                _scribe_numerum(a, linea);
                redde a;
            }
            campi = _campos_findere(c->piscina, textus, &numerus);
            si (campi == NIHIL)
            {
                redde _causa(c->piscina, "memoria deficit");
            }
            si (numerus != II + c->numerus_titulorum)
            {
                ChordaAedificator* a = _causa(c->piscina,
                    "latitudo ordinis falsa: linea ");

                _scribe_numerum(a, linea);
                _scribe_literas(a, " (");
                _scribe_numerum(a, numerus);
                _scribe_literas(a, " campi, ");
                _scribe_numerum(a, II + c->numerus_titulorum);
                _scribe_literas(a, " exspectati)");
                redde a;
            }
            ordo = (OrdoReticuli*)xar_addere(c->ordines);
            si (ordo == NIHIL)
            {
                redde _causa(c->piscina, "memoria deficit");
            }
            ordo->campi    = campi;
            ordo->numerus  = numerus;
            ordo->linea    = linea;
        }
    }
    si (c->tituli == NIHIL)
    {
        ChordaAedificator* a = _causa(c->piscina,
            "genus sine ordine COLUMNAE: ");

        _scribe_literas(a, c->optiones->genus);
        redde a;
    }
    redde NIHIL;
}


/* ==================================================
 * II. Optiones iudicare (ordo recusationum fixus)
 * ================================================== */

interior ChordaAedificator*
_titulus_ignotus (
    Piscina* piscina,
     chorda  titulus)
{
    ChordaAedificator* a = _causa(piscina, "titulus ignotus: ");

    _scribe_chordam(a, titulus);
    redde a;
}

interior ChordaAedificator*
_literae_ignotae (
               Piscina* piscina,
    constans character* titulus)
{
    ChordaAedificator* a = _causa(piscina, "titulus ignotus: ");

    _scribe_literas(a, titulus);
    redde a;
}

interior b32
_in_lista (
    constans ContextusReticuli* c,
                  constans Xar* lista,
                           i32  index)
{
    i32 i;

    per (i = ZEPHYRUM; i < _numerus_listae(lista); i++)
    {
        constans chorda* t = (constans chorda*)xar_obtinere(lista, i);

        si (_index_tituli(c, *t) == (s32)index)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* positio columnae (index tituli) inter notas; -I si absens */
interior s32
_positio_notae (
    constans ContextusReticuli* c,
                           s32  index)
{
    i32 k;

    per (k = ZEPHYRUM; k < c->numerus_notarum; k++)
    {
        si ((s32)c->notae[k] == index)
        {
            redde (s32)k;
        }
    }
    redde (s32)-I;
}

interior ChordaAedificator*
_listam_iudicare (
    constans ContextusReticuli* c,
                  constans Xar* lista)
{
    i32 i;

    per (i = ZEPHYRUM; i < _numerus_listae(lista); i++)
    {
        constans chorda* t = (constans chorda*)xar_obtinere(lista, i);

        si (_index_tituli(c, *t) < ZEPHYRUM)
        {
            redde _titulus_ignotus(c->piscina, *t);
        }
    }
    redde NIHIL;
}

interior ChordaAedificator*
_optiones_iudicare (
    ContextusReticuli* c)
{
     constans OratioReticulumOptiones* o = c->optiones;
                    ChordaAedificator* a;
                                  i32  i;

    c->index_auri = _index_literis(c, o->aurum);
    si (c->index_auri < ZEPHYRUM)
    {
        redde _literae_ignotae(c->piscina, o->aurum);
    }
    si (!_praefixum_auri(c->tituli[c->index_auri]))
    {
        a = _causa(c->piscina,
            "columna auri praefixum aurum- non fert: ");
        _scribe_literas(a, o->aurum);
        redde a;
    }
    /* T36 c: sortes alternae pro columna sortium */
    si (o->alternae > ZEPHYRUM && o->sortes != NIHIL)
    {
        redde _causa(c->piscina, "ambae -sortes et -sortes-alternae");
    }
    si (o->alternae > ZEPHYRUM && o->alternae < II)
    {
        a = _causa(c->piscina, "sortes alternae pauciores quam II: ");
        _scribe_numerum(a, o->alternae);
        redde a;
    }
    c->index_sortium = (s32)-I;
    si (o->sortes != NIHIL)
    {
        c->index_sortium = _index_literis(c, o->sortes);
        si (c->index_sortium < ZEPHYRUM)
        {
            redde _literae_ignotae(c->piscina, o->sortes);
        }
    }
    per (i = ZEPHYRUM; i < _numerus_listae(o->ubi); i++)
    {
        constans OratioReticulumUbi* u =
            (constans OratioReticulumUbi*)xar_obtinere(o->ubi, i);

        si (_index_tituli(c, u->titulus) < ZEPHYRUM)
        {
            redde _titulus_ignotus(c->piscina, u->titulus);
        }
    }
    a = _listam_iudicare(c, o->columnae);
    si (a != NIHIL)
    {
        redde a;
    }
    per (i = ZEPHYRUM; i < _numerus_listae(o->columnae); i++)
    {
        constans chorda* t = (constans chorda*)xar_obtinere(o->columnae,
            i);

        si (_praefixum_auri(*t))
        {
            a = _causa(c->piscina, "columna aurea ut nota: ");
            _scribe_chordam(a, *t);
            redde a;
        }
    }
    a = _listam_iudicare(c, o->praeter);
    si (a != NIHIL)
    {
        redde a;
    }
    a = _listam_iudicare(c, o->gradus);
    si (a != NIHIL)
    {
        redde a;
    }
    si (o->greges != NIHIL)
    {
        si (strcmp(o->greges, "initium") == ZEPHYRUM)
        {
            si (_numerus_listae(o->initium) == ZEPHYRUM)
            {
                redde _causa(c->piscina,
                    "greges initium sine -initium");
            }
        }
        alioquin si (   strcmp(o->greges, "catena")        != ZEPHYRUM
                     && strcmp(o->greges, "catena-libera") != ZEPHYRUM
                     && _index_literis(c, o->greges) < ZEPHYRUM)
        {
            redde _literae_ignotae(c->piscina, o->greges);
        }
    }

    c->gradus_columnae = (b32*)piscina_allocare(c->piscina,
        (memoriae_index)c->numerus_titulorum * magnitudo(b32)
        + magnitudo(b32));
    c->notae = (i32*)piscina_allocare(c->piscina,
        (memoriae_index)c->numerus_titulorum * magnitudo(i32)
        + magnitudo(i32));
    si (c->gradus_columnae == NIHIL || c->notae == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    per (i = ZEPHYRUM; i < c->numerus_titulorum; i++)
    {
        c->gradus_columnae[i] = _in_lista(c, o->gradus, i);
    }
    c->numerus_notarum = ZEPHYRUM;
    si (_numerus_listae(o->columnae) > ZEPHYRUM)
    {
        per (i = ZEPHYRUM; i < _numerus_listae(o->columnae); i++)
        {
            constans chorda* t = (constans chorda*)xar_obtinere(
                o->columnae, i);
                        i32 index = (i32)_index_tituli(c, *t);

            si (!_in_lista(c, o->praeter, index))
            {
                c->notae[c->numerus_notarum]  = index;
                c->numerus_notarum            = c->numerus_notarum + I;
            }
        }
    }
    alioquin
    {
        per (i = ZEPHYRUM; i < c->numerus_titulorum; i++)
        {
            si (   (s32)i == c->index_sortium
                || _praefixum_auri(c->tituli[i])
                || _in_lista(c, o->praeter, i))
            {
                perge;
            }
            c->notae[c->numerus_notarum]  = i;
            c->numerus_notarum            = c->numerus_notarum + I;
        }
    }
    si (c->numerus_notarum == ZEPHYRUM)
    {
        redde _causa(c->piscina, "nullae columnae notae");
    }
    /* T36 b: semen - tituli noti, non aurei, inter notas */
    a = _listam_iudicare(c, o->initium);
    si (a != NIHIL)
    {
        redde a;
    }
    per (i = ZEPHYRUM; i < _numerus_listae(o->initium); i++)
    {
        constans chorda* t = (constans chorda*)xar_obtinere(o->initium,
            i);

        si (_praefixum_auri(*t))
        {
            a = _causa(c->piscina, "columna aurea ut initium: ");
            _scribe_chordam(a, *t);
            redde a;
        }
        si (_positio_notae(c, _index_tituli(c, *t)) < ZEPHYRUM)
        {
            a = _causa(c->piscina,
                "columna non inter notas ut initium: ");
            _scribe_chordam(a, *t);
            redde a;
        }
    }
    si (o->profunditas > RETICULUM_PROFUNDITAS_MAXIMA)
    {
        a = _causa(c->piscina, "profunditas maior quam IV: ");
        _scribe_numerum(a, o->profunditas);
        redde a;
    }
    redde NIHIL;
}


/* ==================================================
 * III. Ordines eligere (-ubi)
 * ================================================== */

interior b32
_ordo_manet (
    constans ContextusReticuli* c,
         constans OrdoReticuli* ordo)
{
     constans OratioReticulumOptiones* o = c->optiones;
                                  i32  u;

    per (u = ZEPHYRUM; u < _numerus_listae(o->ubi); u++)
    {
        constans OratioReticulumUbi* condicio =
            (constans OratioReticulumUbi*)xar_obtinere(o->ubi, u);
                                 s32 index    = _index_tituli(c,
                                     condicio->titulus);
                                 b32 inventum = FALSUM;
                                 i32 v;

        per (v = ZEPHYRUM; v < condicio->numerus; v++)
        {
            si (_aequales(ordo->campi[II + index],
                condicio->valores[v]))
            {
                inventum = VERUM;
                frange;
            }
        }
        si (!inventum)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

interior ChordaAedificator*
_ordines_eligere (
    ContextusReticuli* c)
{
    i32 numerus_ordinum = xar_numerus(c->ordines);
    i32 r;

    c->manentes = (OrdoReticuli**)piscina_allocare(c->piscina,
        (memoriae_index)(numerus_ordinum
            > ZEPHYRUM ? numerus_ordinum : I)
        * magnitudo(OrdoReticuli*));
    si (c->manentes == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    c->numerus = ZEPHYRUM;
    per (r = ZEPHYRUM; r < numerus_ordinum; r++)
    {
        OrdoReticuli* ordo = (OrdoReticuli*)xar_obtinere(c->ordines, r);

        si (!_ordo_manet(c, ordo))
        {
            perge;
        }
        c->manentes[c->numerus]  = ordo;
        c->numerus               = c->numerus + I;
    }
    si (c->numerus == ZEPHYRUM)
    {
        redde _causa(c->piscina, "nulli ordines post -ubi");
    }
    redde NIHIL;
}


/* ==================================================
 * IV. Partes struere (-gradus)
 * ================================================== */

/* infimum series notarum (a sinistra plicatum); NIHIL si memoria
 * deficit */
interior Partitio*
_infimum_seriei (
    constans ContextusReticuli* c,
                  constans i32* series,
                           i32  numerus)
{
    Partitio* p = c->partes[series[ZEPHYRUM]];
         i32  i;

    per (i = I; i < numerus && p != NIHIL; i++)
    {
        p = partitio_infimum(c->piscina, p, c->partes[series[i]]);
    }
    redde p;
}

interior ChordaAedificator*
_valores_struere (
    ContextusReticuli* c,
                  i32  index)
{
    chorda* valores;
       i32  r;

    si (c->valores[index] != NIHIL)
    {
        redde NIHIL;
    }
    valores = (chorda*)piscina_allocare(c->piscina,
        (memoriae_index)c->numerus * magnitudo(chorda));
    si (valores == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    per (r = ZEPHYRUM; r < c->numerus; r++)
    {
        chorda valor = c->manentes[r]->campi[II + index];

        si (c->gradus_columnae[index])
        {
            si (!_numericum(valor))
            {
                ChordaAedificator* a = _causa(c->piscina,
                    "gradus in valore non numerico: ");

                _scribe_chordam(a, c->tituli[index]);
                _scribe_literas(a, ", linea ");
                _scribe_numerum(a, c->manentes[r]->linea);
                redde a;
            }
            valor = _aequalis_literis(valor, "0") ? c->gradus_nullus
                : _aequalis_literis(valor, "1") ? c->gradus_unus
                : c->gradus_plures;
        }
        valores[r] = valor;
    }
    c->valores[index] = valores;
    redde NIHIL;
}

interior ChordaAedificator*
_partes_struere (
    ContextusReticuli* c)
{
    ChordaAedificator* a;
                  i32  k;

    c->valores = (chorda**)piscina_allocare(c->piscina,
        (memoriae_index)c->numerus_titulorum * magnitudo(chorda*));
    c->partes  = (Partitio**)piscina_allocare(c->piscina,
        (memoriae_index)c->numerus_notarum * magnitudo(Partitio*));
    si (c->valores == NIHIL || c->partes == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    memset(c->valores, ZEPHYRUM,
        (memoriae_index)c->numerus_titulorum * magnitudo(chorda*));
    c->gradus_nullus  = chorda_ex_literis("0", c->piscina);
    c->gradus_unus    = chorda_ex_literis("1", c->piscina);
    c->gradus_plures  = chorda_ex_literis("plures", c->piscina);
    per (k = ZEPHYRUM; k < c->numerus_notarum; k++)
    {
        a = _valores_struere(c, c->notae[k]);
        si (a != NIHIL)
        {
            redde a;
        }
    }
    a = _valores_struere(c, (i32)c->index_auri);
    si (a != NIHIL)
    {
        redde a;
    }
    si (c->optiones->alternae > ZEPHYRUM)
    {
        /* T36 c: sortes alternae - sors ordinis r = r mod N, tituli
         * 'alterna-0'.. semel structi */
           i32  numerus_alternarum  = c->optiones->alternae;
        chorda* tituli              = (chorda*)piscina_allocare(
            c->piscina,
            (memoriae_index)numerus_alternarum * magnitudo(chorda));
        i32 r;

        c->valores_sortium = (chorda*)piscina_allocare(c->piscina,
            (memoriae_index)c->numerus * magnitudo(chorda));
        si (tituli == NIHIL || c->valores_sortium == NIHIL)
        {
            redde _causa(c->piscina, "memoria deficit");
        }
        per (r = ZEPHYRUM; r < numerus_alternarum; r++)
        {
            ChordaAedificator* t = chorda_aedificator_creare(c->piscina,
                (memoriae_index)XVI);

            si (t == NIHIL)
            {
                redde _causa(c->piscina, "memoria deficit");
            }
            _scribe_literas(t, "alterna-");
            _scribe_numerum(t, r);
            tituli[r] = chorda_aedificator_finire(t);
        }
        per (r = ZEPHYRUM; r < c->numerus; r++)
        {
            c->valores_sortium[r] = tituli[r % numerus_alternarum];
        }
    }
    alioquin
    {
        a = _valores_struere(c, (i32)c->index_sortium);
        si (a != NIHIL)
        {
            redde a;
        }
        c->valores_sortium = c->valores[c->index_sortium];
    }
    per (k = ZEPHYRUM; k < c->numerus_notarum; k++)
    {
        c->partes[k] = partitio_ex_chordis(c->piscina, c->numerus,
            c->valores[c->notae[k]]);
        si (c->partes[k] == NIHIL)
        {
            redde _causa(c->piscina, "memoria deficit");
        }
    }
    c->aurum  = partitio_ex_chordis(c->piscina, c->numerus,
        c->valores[c->index_auri]);
    c->sortes = partitio_ex_chordis(c->piscina, c->numerus,
        c->valores_sortium);
    c->una    = partitio_una(c->piscina, c->numerus);
    si (c->aurum == NIHIL || c->sortes == NIHIL || c->una == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    si (c->sortes->numerus_gregum < II)
    {
        a = _causa(c->piscina, "sortes pauciores quam II: ");
        _scribe_numerum(a, c->sortes->numerus_gregum);
        redde a;
    }
    /* T36 b: constantes (grex unus post -ubi) extra reticulum et
     * infima; index reticuli -> nota */
    c->constantes      = (b32*)piscina_allocare(c->piscina,
        (memoriae_index)c->numerus_notarum * magnitudo(b32));
    c->reticulum_index = (i32*)piscina_allocare(c->piscina,
        (memoriae_index)c->numerus_notarum * magnitudo(i32));
    si (c->constantes == NIHIL || c->reticulum_index == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    c->numerus_reticuli     = ZEPHYRUM;
    c->numerus_constantium  = ZEPHYRUM;
    per (k = ZEPHYRUM; k < c->numerus_notarum; k++)
    {
        si (c->partes[k]->numerus_gregum <= I)
        {
            c->constantes[k]        = VERUM;
            c->numerus_constantium  = c->numerus_constantium + I;
        }
        alioquin
        {
            c->constantes[k] = FALSUM;
            c->reticulum_index[c->numerus_reticuli] = k;
            c->numerus_reticuli = c->numerus_reticuli + I;
        }
    }
    /* semen catenae: infimum columnarum -initium (non constantium) */
    c->numerus_seminis = _numerus_listae(c->optiones->initium);
    si (c->numerus_seminis > ZEPHYRUM)
    {
        i32 i;

        c->semen_columnae = (i32*)piscina_allocare(c->piscina,
            (memoriae_index)c->numerus_seminis * magnitudo(i32));
        si (c->semen_columnae == NIHIL)
        {
            redde _causa(c->piscina, "memoria deficit");
        }
        per (i = ZEPHYRUM; i < c->numerus_seminis; i++)
        {
            constans chorda* t = (constans chorda*)xar_obtinere(
                c->optiones->initium, i);
                        s32 positio = _positio_notae(c,
                            _index_tituli(c, *t));

            si (c->constantes[positio])
            {
                a = _causa(c->piscina, "columna constans ut initium: ");
                _scribe_chordam(a, *t);
                redde a;
            }
            c->semen_columnae[i] = (i32)positio;
        }
        c->semen = _infimum_seriei(c, c->semen_columnae,
            c->numerus_seminis);
        si (c->semen == NIHIL)
        {
            redde _causa(c->piscina, "memoria deficit");
        }
    }
    redde NIHIL;
}


/* ==================================================
 * V. Computare
 * ================================================== */

/* numerus subsetorum magnitudinis d ex m (productus exactus) */
interior i32
_numerus_combinationum (
    i32 m,
    i32 d)
{
    i64 n = I;
    i32 i;

    si (d > m)
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < d; i++)
    {
        n = n * (i64)(m - i) / (i64)(i + I);
    }
    redde (i32)n;
}

/* combinatio lexicographica proxima; FALSUM si ultima */
interior b32
_combinatio_proxima (
    i32* combinatio,
    i32  d,
    i32  m)
{
    i32 i = d;

    dum (i > ZEPHYRUM)
    {
        i = i - I;
        si (combinatio[i] < m - d + i)
        {
            i32 j;

            combinatio[i] = combinatio[i] + I;
            per (j = i + I; j < d; j++)
            {
                combinatio[j] = combinatio[j - I] + I;
            }
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* optimum rectorum inter series minores (d - I): singulae ex
 * aestimationibus, ceterae ex tabula dispersa (clavis = octeti
 * series) */
interior i32
_recti_optimum_serierum_minorum (
    constans ContextusReticuli* c,
                TabulaDispersa* tabula,
                  constans i32* series,
                           i32  d)
{
    i32 minor[RETICULUM_PROFUNDITAS_MAXIMA];
    i32 optimum = ZEPHYRUM;
    i32 omissa;

    per (omissa = ZEPHYRUM; omissa < d; omissa++)
    {
        i32 r = ZEPHYRUM;
        i32 k = ZEPHYRUM;
        i32 i;

        per (i = ZEPHYRUM; i < d; i++)
        {
            si (i != omissa)
            {
                minor[k]  = series[i];
                k         = k + I;
            }
        }
        si (d == II)
        {
            r = c->aestimationes[minor[ZEPHYRUM]]->summa.recti;
        }
        alioquin
        {
            chorda  clavis;
            vacuum* valor;

            clavis.datum    = (i8*)minor;
            clavis.mensura  = (i32)((memoriae_index)(d - I)
                * magnitudo(i32));
            si (tabula_dispersa_invenire(tabula, clavis, &valor))
            {
                r = c->infima[*(i32*)valor].recti;
            }
        }
        si (r > optimum)
        {
            optimum = r;
        }
    }
    redde optimum;
}

interior ChordaAedificator*
_finem_computare (
    ContextusReticuli* c,
       PartitioCatena* catena,
         FinisCatenae* finis)
{
       constans Partitio* ultima;
      PartitioAestimatio* prior;
                     b32* adhibita;
                     i32  numerus_sortium = c->sortes->numerus_gregum;
                     i32  i;
                     i32  s;

    ultima   = catena->numerus > ZEPHYRUM
        ? catena->partes[catena->numerus - I] : catena->semen;
    prior    = catena->numerus > ZEPHYRUM
        ? catena->aestimationes[catena->numerus - I] : catena->initium;
    adhibita = (b32*)piscina_allocare(c->piscina,
        (memoriae_index)c->numerus_notarum * magnitudo(b32));
    finis->cadit = (b32*)piscina_allocare(c->piscina,
        (memoriae_index)numerus_sortium * magnitudo(b32));
    si (adhibita == NIHIL || finis->cadit == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    memset(adhibita, ZEPHYRUM,
        (memoriae_index)c->numerus_notarum * magnitudo(b32));
    memset(finis->cadit, ZEPHYRUM,
        (memoriae_index)numerus_sortium * magnitudo(b32));
    per (i = ZEPHYRUM; i < catena->numerus; i++)
    {
        adhibita[catena->columnae[i]] = VERUM;
    }
    finis->columna     = (s32)-I;
    finis->recti       = ZEPHYRUM;
    finis->lucrum      = (s32)ZEPHYRUM;
    finis->cadit_ulla  = FALSUM;
    per (i = ZEPHYRUM; i < c->numerus_notarum; i++)
    {
             PiscinaNotatio  nota;
                   Partitio* candidata;
         PartitioAestimatio* e;

        si (adhibita[i])
        {
            perge;
        }
        nota       = piscina_notare(c->piscina);
        candidata  = partitio_infimum(c->piscina, ultima, c->partes[i]);
        e = candidata != NIHIL
            ? partitio_aestimare(c->piscina, candidata, c->aurum,
            c->sortes,
                c->optiones->limen)
            : NIHIL;
        si (   e != NIHIL
            && (finis->columna < ZEPHYRUM
            || e->summa.recti > finis->recti))
        {
            finis->columna  = (s32)i;
            finis->recti    = e->summa.recti;
            finis->lucrum     = (s32)e->summa.recti
                - (s32)prior->summa.recti;
            finis->cadit_ulla = FALSUM;
            per (s = ZEPHYRUM; s < numerus_sortium; s++)
            {
                finis->cadit[s] = (b32)(e->sortes[s].recti
                    < prior->sortes[s].recti);
                si (finis->cadit[s])
                {
                    finis->cadit_ulla = VERUM;
                }
            }
        }
        piscina_reficere(c->piscina, nota);
    }
    redde NIHIL;
}

interior ChordaAedificator*
_computare (
    ContextusReticuli* c)
{
    constans OratioReticulumOptiones* o      = c->optiones;
          constans Partitio* constans* partes =
              (constans Partitio* constans*)c->partes;
                    ChordaAedificator* a;
               PartitioCatenaOptiones  oc;
                                  i32  k = c->numerus_notarum;
                                  i32  i;
                                  i32  j;
                                  i32  q;

    c->aestimationes =
        (PartitioAestimatio**)piscina_allocare(c->piscina,
        (memoriae_index)k * magnitudo(PartitioAestimatio*));
    si (c->aestimationes == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    per (i = ZEPHYRUM; i < k; i++)
    {
        c->aestimationes[i] = partitio_aestimare(c->piscina,
            c->partes[i],
            c->aurum, c->sortes, o->limen);
        si (c->aestimationes[i] == NIHIL)
        {
            redde _causa(c->piscina, "memoria deficit");
        }
    }
    /* T36 b: reticulum super notas non constantes solas */
    {
        constans Partitio** partes_reticuli =
            (constans Partitio**)piscina_allocare(c->piscina,
            (memoriae_index)(c->numerus_reticuli
                > ZEPHYRUM ? c->numerus_reticuli : I)
            * magnitudo(Partitio*));

        si (partes_reticuli == NIHIL)
        {
            redde _causa(c->piscina, "memoria deficit");
        }
        per (i = ZEPHYRUM; i < c->numerus_reticuli; i++)
        {
            partes_reticuli[i] = c->partes[c->reticulum_index[i]];
        }
        c->reticulum = partitio_reticulum_struere(c->piscina,
            (constans Partitio* constans*)partes_reticuli,
            c->numerus_reticuli);
    }
    c->lucrum_minimum = o->lucrum > ZEPHYRUM ? o->lucrum : o->limen;
    partitio_catena_optiones_initium(&oc);
    oc.limen           = o->limen;
    oc.lucrum_minimum  = c->lucrum_minimum;
    oc.semen           = c->semen;
    oc.sortes_vetant   = VERUM;
    c->catena_vetans   = partitio_catenam_struere(c->piscina, partes,
        k, c->aurum, c->sortes, &oc);
    oc.sortes_vetant   = FALSUM;
    c->catena_libera   = partitio_catenam_struere(c->piscina, partes,
        k, c->aurum, c->sortes, &oc);
    si (   c->reticulum     == NIHIL || c->catena_vetans == NIHIL
        || c->catena_libera == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    a = _finem_computare(c, c->catena_vetans, &c->finis_vetans);
    si (a != NIHIL)
    {
        redde a;
    }
    a = _finem_computare(c, c->catena_libera, &c->finis_libera);
    si (a != NIHIL)
    {
        redde a;
    }

    /* T36 b: infima k columnarum non constantium, profunditate
     * II..N: series, numeri et tabula dispersa EXTRA regionem, infimum
     * et aestimatio intra; incrementum contra optimum serierum
     * minorum */
    {
        TabulaDispersa* tabula;
                   i32* series;
                   i32  m                   = c->numerus_reticuli;
                   i32  longitudo_serierum  = ZEPHYRUM;
                   i32  t                   = ZEPHYRUM;
                   i32  d;

        c->numerus_infimorum = ZEPHYRUM;
        per (d = II; d <= o->profunditas; d++)
        {
            i32 n = _numerus_combinationum(m, d);

            c->numerus_infimorum  = c->numerus_infimorum + n;
            longitudo_serierum    = longitudo_serierum + n * d;
        }
        c->infima = (InfimumReticuli*)piscina_allocare(c->piscina,
            (memoriae_index)(c->numerus_infimorum
                > ZEPHYRUM ? c->numerus_infimorum : I)
            * magnitudo(InfimumReticuli));
        series    = (i32*)piscina_allocare(c->piscina,
            (memoriae_index)(longitudo_serierum
                > ZEPHYRUM ? longitudo_serierum : I)
            * magnitudo(i32));
        tabula    = tabula_dispersa_creare_chorda(c->piscina,
            c->numerus_infimorum > XVI ? c->numerus_infimorum * II
            : XXXII);
        si (c->infima == NIHIL || series == NIHIL || tabula == NIHIL)
        {
            redde _causa(c->piscina, "memoria deficit");
        }
        q = ZEPHYRUM;
        per (d = II; d <= o->profunditas; d++)
        {
            i32 combinatio[RETICULUM_PROFUNDITAS_MAXIMA];

            si (_numerus_combinationum(m, d) == ZEPHYRUM)
            {
                perge;
            }
            per (i = ZEPHYRUM; i < d; i++)
            {
                combinatio[i] = i;
            }
            fac
            {
                    InfimumReticuli* recordum =&c->infima[q];
                     PiscinaNotatio  nota;
                           Partitio* infimum;
                 PartitioAestimatio* e;
                             chorda  clavis;

                recordum->profunditas  = d;
                recordum->columnae     = series + t;
                recordum->index        = q;
                per (i = ZEPHYRUM; i < d; i++)
                {
                    recordum->columnae[i] =
                        c->reticulum_index[combinatio[i]];
                }
                t             = t + d;
                clavis.datum  = (i8*)recordum->columnae;
                clavis.mensura  = (i32)((memoriae_index)d
                    * magnitudo(i32));
                si (!tabula_dispersa_inserere(tabula, clavis,
                    &recordum->index))
                {
                    redde _causa(c->piscina, "memoria deficit");
                }
                nota     = piscina_notare(c->piscina);
                infimum  = _infimum_seriei(c, recordum->columnae, d);
                e = infimum != NIHIL
                    ? partitio_aestimare(c->piscina, infimum, c->aurum,
                        c->sortes, o->limen)
                    : NIHIL;
                si (e == NIHIL)
                {
                    piscina_reficere(c->piscina, nota);
                    redde _causa(c->piscina, "memoria deficit");
                }
                recordum->greges  = infimum->numerus_gregum;
                recordum->recti   = e->summa.recti;
                recordum->incrementum  = (s32)e->summa.recti
                    - (s32)_recti_optimum_serierum_minorum(c, tabula,
                        recordum->columnae, d);
                recordum->cadentes     = _cadentes_basis(e);
                recordum->inaestimati  = e->summa.inaestimati;
                piscina_reficere(c->piscina, nota);
                q = q + I;
            }
            dum (_combinatio_proxima(combinatio, d, m));
        }
    }

    /* ordo columnarum: lucrum descendens, par -> ordo capitis */
    c->ordo = (i32*)piscina_allocare(c->piscina,
        (memoriae_index)k * magnitudo(i32));
    si (c->ordo == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    per (i = ZEPHYRUM; i < k; i++)
    {
        i32 v = i;

        j = i;
        dum (   j > ZEPHYRUM
             && c->aestimationes[c->ordo[j - I]]->lucrum
                < c->aestimationes[v]->lucrum)
        {
            c->ordo[j]  = c->ordo[j - I];
            j           = j - I;
        }
        c->ordo[j] = v;
    }
    /* selecti: prima quaeque profunditate, incremento descendente,
     * par -> ordo enumerationis (electio repetita, non ordinatio) */
    {
        b32* sumptus = (b32*)piscina_allocare(c->piscina,
            (memoriae_index)(c->numerus_infimorum
                > ZEPHYRUM ? c->numerus_infimorum : I)
            * magnitudo(b32));
         i32 d;

        c->selecti = (i32*)piscina_allocare(c->piscina,
            (memoriae_index)RETICULUM_PROFUNDITAS_MAXIMA
            * (memoriae_index)(o->prima > ZEPHYRUM ? o->prima : I)
            * magnitudo(i32));
        si (sumptus == NIHIL || c->selecti == NIHIL)
        {
            redde _causa(c->piscina, "memoria deficit");
        }
        memset(sumptus, ZEPHYRUM, (memoriae_index)(c->numerus_infimorum
            > ZEPHYRUM ? c->numerus_infimorum : I) * magnitudo(b32));
        c->numerus_selectorum = ZEPHYRUM;
        per (d = II; d <= o->profunditas; d++)
        {
            per (j = ZEPHYRUM; j < o->prima; j++)
            {
                i32 optimus = c->numerus_infimorum;

                per (i = ZEPHYRUM; i < c->numerus_infimorum; i++)
                {
                    si (sumptus[i] || c->infima[i].profunditas != d)
                    {
                        perge;
                    }
                    si (   optimus == c->numerus_infimorum
                        || c->infima[i].incrementum
                            > c->infima[optimus].incrementum)
                    {
                        optimus = i;
                    }
                }
                si (optimus == c->numerus_infimorum)
                {
                    frange;
                }
                sumptus[optimus]                   = VERUM;
                c->selecti[c->numerus_selectorum]  = optimus;
                c->numerus_selectorum              =
                    c->numerus_selectorum + I;
            }
        }
    }
    redde NIHIL;
}


/* ==================================================
 * VI. Scribere - ordines machinae
 * ================================================== */

interior vacuum
_caput_machinae (
               ChordaAedificator* a,
             constans character* via,
             constans character* genus,
    constans character* constans* tituli,
                             i32  numerus)
{
    i32 i;

    _scribe_literas(a, via);
    _scribe_tabulam(a);
    _scribe_literas(a, "COLUMNAE");
    _scribe_tabulam(a);
    _scribe_literas(a, genus);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        _scribe_tabulam(a);
        _scribe_literas(a, tituli[i]);
    }
    _scribe_lineam_novam(a);
}

interior vacuum
_ordinem_incipere (
     ChordaAedificator* a,
    constans character* via,
    constans character* genus)
{
    _scribe_literas(a, via);
    _scribe_tabulam(a);
    _scribe_literas(a, genus);
}

interior vacuum
_campus_numeri (
     ChordaAedificator* a,
                   i32  numerus)
{
    _scribe_tabulam(a);
    _scribe_numerum(a, numerus);
}

interior vacuum
_campus_integri (
     ChordaAedificator* a,
                   s32  numerus)
{
    _scribe_tabulam(a);
    _scribe_integrum(a, numerus);
}

interior vacuum
_campus_chordae (
     ChordaAedificator* a,
                chorda  c)
{
    _scribe_tabulam(a);
    _scribe_chordam(a, c);
}

/* tituli notarum separatore iuncti */
interior vacuum
_titulos_scribere (
             ChordaAedificator* a,
    constans ContextusReticuli* c,
                  constans i32* indices,
                           i32  numerus,
            constans character* separator)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (i > ZEPHYRUM)
        {
            _scribe_literas(a, separator);
        }
        _scribe_chordam(a, _notae_titulus(c, indices[i]));
    }
}

/* T36 c: partitio quam -greges nominat - p, e et indices notarum
 * (semen, deinde gradus) quorum valores gregem nominant; FALSUM si
 * titulus non inter notas. indices: notae + semen + I */
interior b32
_greges_eligere (
      constans ContextusReticuli*  c,
               constans Partitio** p,
     constans PartitioAestimatio** e,
                             i32*  indices,
                             i32*  numerus)
{
    constans OratioReticulumOptiones* o        = c->optiones;
             constans PartitioCatena* catena   = c->catena_vetans;
                                 b32  catenae  = VERUM;
                                 i32  gradus   = ZEPHYRUM;
                                 s32  positio;
                                 i32  i;

    *numerus = ZEPHYRUM;
    si (strcmp(o->greges, "catena-libera") == ZEPHYRUM)
    {
        catena = c->catena_libera;
        gradus = catena->numerus;
    }
    alioquin si (strcmp(o->greges, "catena") == ZEPHYRUM)
    {
        gradus = catena->numerus;
    }
    alioquin si (strcmp(o->greges, "initium") != ZEPHYRUM)
    {
        catenae = FALSUM;
    }
    si (catenae)
    {
        *p = gradus > ZEPHYRUM ? catena->partes[gradus - I]
            : catena->semen;
        *e = gradus > ZEPHYRUM ? catena->aestimationes[gradus - I]
            : catena->initium;
        per (i = ZEPHYRUM; i < c->numerus_seminis; i++)
        {
            indices[*numerus]  = c->semen_columnae[i];
            *numerus           = *numerus + I;
        }
        per (i = ZEPHYRUM; i < gradus; i++)
        {
            indices[*numerus]  = catena->columnae[i];
            *numerus           = *numerus + I;
        }
        redde VERUM;
    }
    positio = _positio_notae(c, _index_literis(c, o->greges));
    si (positio < ZEPHYRUM)
    {
        redde FALSUM;
    }
    *p                 = c->partes[positio];
    *e                 = c->aestimationes[positio];
    indices[ZEPHYRUM]  = (i32)positio;
    *numerus           = I;
    redde VERUM;
}

/* titulus gregis: valores columnarum in ordine repraesentante */
interior vacuum
_gregem_scribere (
             ChordaAedificator* a,
    constans ContextusReticuli* c,
             constans Partitio* p,
                  constans i32* indices,
                           i32  numerus,
                           i32  g,
            constans character* separator)
{
    i32 i;

    si (numerus == ZEPHYRUM)
    {
        _scribe_literas(a, "(una)");
        redde;
    }
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (i > ZEPHYRUM)
        {
            _scribe_literas(a, separator);
        }
        _scribe_chordam(a,
            c->valores[c->notae[indices[i]]][p->primus[g]]);
    }
}

/* aureus cum plurimis ordinibus gregis; par -> index minimus */
interior i32
_aureus_maximus (
    constans PartitioAestimatio* e,
                            i32  g)
{
    i32 a;
    i32 optimus = ZEPHYRUM;

    per (a = I; a < e->numerus_aureorum; a++)
    {
        si (   e->numeri[(memoriae_index)g * e->numerus_aureorum + a]
            > e->numeri[(memoriae_index)g * e->numerus_aureorum
                + optimus])
        {
            optimus = a;
        }
    }
    redde optimus;
}

interior vacuum
_greges_machinae (
    constans ContextusReticuli* c,
             ChordaAedificator* a,
            constans character* via)
{
    constans OratioReticulumOptiones* o = c->optiones;
                   constans Partitio* p = NIHIL;
         constans PartitioAestimatio* e = NIHIL;
                                 i32* indices;
                                 i32  numerus  = ZEPHYRUM;
                                 b32  electi   = FALSUM;
                                 i32  g;
                                 i32  s;

    indices = (i32*)piscina_allocare(c->piscina,
        (memoriae_index)(c->numerus_notarum + c->numerus_seminis + I)
        * magnitudo(i32));
    si (indices != NIHIL && o->greges != NIHIL)
    {
        electi = _greges_eligere(c, &p, &e, indices, &numerus);
    }
    _caput_machinae(a, via, "RETICULUM-GREX", TITULI_GREGIS,
        NUMERUS_TITULORUM(TITULI_GREGIS));
    per (g = ZEPHYRUM; electi && g < p->numerus_gregum; g++)
    {
        i32 aureus        = _aureus_maximus(e, g);
        i32 maximum       = e->numeri[(memoriae_index)g
            * e->numerus_aureorum + aureus];
        i32 suffragantes  = ZEPHYRUM;
        i32 retenti       = ZEPHYRUM;
        i32 recti         = ZEPHYRUM;

        per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
        {
            memoriae_index locus =
                (memoriae_index)g * e->numerus_sortium
                + s;

            si (e->suffragia[locus] != e->numerus_aureorum)
            {
                suffragantes = suffragantes + I;
            }
            retenti  = retenti + e->retenti[locus];
            recti    = recti + e->recti_gregum[locus];
        }
        _ordinem_incipere(a, via, "RETICULUM-GREX");
        _scribe_tabulam(a);
        _scribe_literas(a, o->greges);
        _scribe_tabulam(a);
        _gregem_scribere(a, c, p, indices, numerus, g, "+");
        _campus_numeri(a, p->magnitudines[g]);
        _campus_chordae(a, _auri_titulus(c, aureus));
        _campus_numeri(a, maximum);
        _campus_numeri(a, p->magnitudines[g] > ZEPHYRUM
            ? (i32)((i64)maximum * (i64)M / (i64)p->magnitudines[g])
            : ZEPHYRUM);
        _scribe_tabulam(a);
        si (e->captivus[g] < e->numerus_sortium)
        {
            _scribe_chordam(a, _sortis_titulus(c, e->captivus[g]));
        }
        alioquin
        {
            _scribe_literas(a, "-");
        }
        _scribe_tabulam(a);
        _scribe_literas(a, PARTITIO_TITULI_HABITUUM[e->habitus[g]]);
        _campus_numeri(a, e->mutabilis[g] ? I : ZEPHYRUM);
        _campus_numeri(a, suffragantes);
        _campus_numeri(a, retenti);
        _campus_numeri(a, recti);
        _scribe_lineam_novam(a);
    }
    _caput_machinae(a, via, "RETICULUM-GREX-SORS", TITULI_GREGIS_SORTIS,
        NUMERUS_TITULORUM(TITULI_GREGIS_SORTIS));
    per (g = ZEPHYRUM; electi && g < p->numerus_gregum; g++)
    {
        per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
        {
            memoriae_index locus =
                (memoriae_index)g * e->numerus_sortium
                + s;

            _ordinem_incipere(a, via, "RETICULUM-GREX-SORS");
            _scribe_tabulam(a);
            _scribe_literas(a, o->greges);
            _scribe_tabulam(a);
            _gregem_scribere(a, c, p, indices, numerus, g, "+");
            _campus_chordae(a, _sortis_titulus(c, s));
            _campus_numeri(a, p->magnitudines[g] - e->retenti[locus]);
            _scribe_tabulam(a);
            si (e->suffragia[locus] != e->numerus_aureorum)
            {
                _scribe_chordam(a, _auri_titulus(c,
                    e->suffragia[locus]));
            }
            alioquin
            {
                _scribe_literas(a, "-");
            }
            _campus_numeri(a, e->margo[locus]);
            _campus_numeri(a, e->retenti[locus]);
            _campus_numeri(a, e->recti_gregum[locus]);
            _scribe_lineam_novam(a);
        }
    }
}

interior vacuum
_catenam_machinae (
     constans ContextusReticuli* c,
              ChordaAedificator* a,
             constans character* via,
        constans PartitioCatena* catena,
                            i32  vetans)
{
    i32 i;
    i32 s;

    _ordinem_incipere(a, via, "RETICULUM-CATENA");
    _campus_numeri(a, vetans);
    _campus_numeri(a, ZEPHYRUM);
    _scribe_tabulam(a);
    si (c->numerus_seminis > ZEPHYRUM)
    {
        _titulos_scribere(a, c, c->semen_columnae, c->numerus_seminis,
            "+");
    }
    alioquin
    {
        _scribe_literas(a, "-");
    }
    _campus_numeri(a, catena->semen->numerus_gregum);
    _campus_numeri(a, catena->initium->summa.recti);
    _campus_integri(a, (s32)ZEPHYRUM);
    _campus_numeri(a, ZEPHYRUM);
    _scribe_lineam_novam(a);
    per (i = ZEPHYRUM; i < catena->numerus; i++)
    {
        constans PartitioAestimatio* prior = i > ZEPHYRUM
            ? catena->aestimationes[i - I] : catena->initium;
        constans PartitioAestimatio* e =
            catena->aestimationes[i];
                                 i32 cadentes = ZEPHYRUM;

        per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
        {
            si (e->sortes[s].recti < prior->sortes[s].recti)
            {
                cadentes = cadentes + I;
            }
        }
        _ordinem_incipere(a, via, "RETICULUM-CATENA");
        _campus_numeri(a, vetans);
        _campus_numeri(a, i + I);
        _campus_chordae(a, _notae_titulus(c, catena->columnae[i]));
        _campus_numeri(a, catena->partes[i]->numerus_gregum);
        _campus_numeri(a, e->summa.recti);
        _campus_integri(a, (s32)e->summa.recti
            - (s32)prior->summa.recti);
        _campus_numeri(a, cadentes);
        _scribe_lineam_novam(a);
    }
}

interior vacuum
_machinam_scribere (
    constans ContextusReticuli* c,
          ChordaAedificator* a)
{
    constans character* via  = c->via != NIHIL ? c->via : "";
                   i32  k    = c->numerus_notarum;
                   i32  q;
                   i32  i;
                   i32  j;
                   i32  s;

    _caput_machinae(a, via, "RETICULUM-COLUMNA", TITULI_COLUMNAE,
        NUMERUS_TITULORUM(TITULI_COLUMNAE));
    per (q = ZEPHYRUM; q < k; q++)
    {
        constans PartitioAestimatio* e = c->aestimationes[c->ordo[q]];

        _ordinem_incipere(a, via, "RETICULUM-COLUMNA");
        _campus_chordae(a, _notae_titulus(c, c->ordo[q]));
        _campus_numeri(a, e->numerus_gregum);
        _campus_numeri(a, e->summa.ordines);
        _campus_numeri(a, e->summa.recti);
        _campus_numeri(a, e->summa.basis);
        _campus_integri(a, e->lucrum);
        _campus_numeri(a, _puritas(&e->summa));
        _campus_numeri(a, e->puritas_intra);
        _campus_numeri(a, e->summa.inaestimati);
        _campus_numeri(a, e->greges_mutabiles);
        _campus_numeri(a, _cadentes_basis(e));
        _campus_numeri(a, e->inaestimabilis ? I : ZEPHYRUM);
        _campus_numeri(a, e->greges_captivi);
        _campus_numeri(a, e->ordines_captivi);
        _scribe_lineam_novam(a);
    }
    _caput_machinae(a, via, "RETICULUM-SORS", TITULI_SORTIS,
        NUMERUS_TITULORUM(TITULI_SORTIS));
    per (q = ZEPHYRUM; q < k; q++)
    {
        constans PartitioAestimatio* e = c->aestimationes[c->ordo[q]];

        per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
        {
            _ordinem_incipere(a, via, "RETICULUM-SORS");
            _campus_chordae(a, _notae_titulus(c, c->ordo[q]));
            _campus_chordae(a, _sortis_titulus(c, s));
            _campus_numeri(a, e->sortes[s].ordines);
            _campus_numeri(a, e->sortes[s].recti);
            _campus_numeri(a, e->sortes[s].basis);
            _campus_numeri(a, e->sortes[s].inaestimati);
            _scribe_lineam_novam(a);
        }
    }
    _caput_machinae(a, via, "RETICULUM-CATENA", TITULI_CATENAE,
        NUMERUS_TITULORUM(TITULI_CATENAE));
    _catenam_machinae(c, a, via, c->catena_vetans, I);
    _catenam_machinae(c, a, via, c->catena_libera, ZEPHYRUM);
    /* CONDICIO: gradus catenae liberae per sortem cadentem (LIV) */
    _caput_machinae(a, via, "RETICULUM-CONDICIO", TITULI_CONDICIONIS,
        NUMERUS_TITULORUM(TITULI_CONDICIONIS));
    per (i = ZEPHYRUM; i < c->catena_libera->numerus; i++)
    {
        constans PartitioAestimatio* prior = i > ZEPHYRUM
            ? c->catena_libera->aestimationes[i - I]
            : c->catena_libera->initium;
        constans PartitioAestimatio* e =
            c->catena_libera->aestimationes[i];

        per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
        {
            si (e->sortes[s].recti < prior->sortes[s].recti)
            {
                _ordinem_incipere(a, via, "RETICULUM-CONDICIO");
                _campus_numeri(a, i + I);
                _campus_chordae(a, _notae_titulus(c,
                    c->catena_libera->columnae[i]));
                _campus_chordae(a, _sortis_titulus(c, s));
                _campus_numeri(a, e->sortes[s].recti);
                _campus_numeri(a, prior->sortes[s].recti);
                _scribe_lineam_novam(a);
            }
        }
    }
    _caput_machinae(a, via, "RETICULUM-INFIMUM", TITULI_INFIMI,
        NUMERUS_TITULORUM(TITULI_INFIMI));
    per (q = ZEPHYRUM; q < c->numerus_selectorum; q++)
    {
        constans InfimumReticuli* recordum =&c->infima[c->selecti[q]];

        _ordinem_incipere(a, via, "RETICULUM-INFIMUM");
        _campus_numeri(a, recordum->profunditas);
        _scribe_tabulam(a);
        _titulos_scribere(a, c, recordum->columnae,
            recordum->profunditas, "+");
        _campus_numeri(a, recordum->greges);
        _campus_numeri(a, recordum->recti);
        _campus_integri(a, recordum->incrementum);
        _campus_numeri(a, recordum->cadentes);
        _campus_numeri(a, recordum->inaestimati);
        _scribe_lineam_novam(a);
    }
    _caput_machinae(a, via, "RETICULUM-TEGIT", TITULI_TEGMINIS,
        NUMERUS_TITULORUM(TITULI_TEGMINIS));
    k = c->numerus_reticuli;
    per (i = ZEPHYRUM; i < k; i++)
    {
        per (j = ZEPHYRUM; j < k; j++)
        {
            si (c->reticulum->tegit[i * k + j])
            {
                _ordinem_incipere(a, via, "RETICULUM-TEGIT");
                _campus_chordae(a, _notae_titulus(c,
                    c->reticulum_index[i]));
                _campus_chordae(a, _notae_titulus(c,
                    c->reticulum_index[j]));
                _scribe_lineam_novam(a);
            }
        }
    }
    _caput_machinae(a, via, "RETICULUM-DUPLEX", TITULI_DUPLICIS,
        NUMERUS_TITULORUM(TITULI_DUPLICIS));
    per (i = ZEPHYRUM; i < k; i++)
    {
        si (c->reticulum->geminus[i] != i)
        {
            _ordinem_incipere(a, via, "RETICULUM-DUPLEX");
            _campus_chordae(a, _notae_titulus(c,
                c->reticulum_index[i]));
            _campus_chordae(a, _notae_titulus(c,
                c->reticulum_index[c->reticulum->geminus[i]]));
            _scribe_lineam_novam(a);
        }
    }
    _greges_machinae(c, a, via);
}


/* ==================================================
 * VII. Scribere - relatio humana
 * ================================================== */

interior vacuum
_catenam_humanam (
    constans ContextusReticuli* c,
             ChordaAedificator* a,
       constans PartitioCatena* catena,
         constans FinisCatenae* finis)
{
    i32 i;
    i32 s;

    si (c->numerus_seminis > ZEPHYRUM)
    {
        _scribe_literas(a, "  0 initium ");
        _titulos_scribere(a, c, c->semen_columnae, c->numerus_seminis,
            " x ");
        _scribe_literas(a, ": greges ");
        _scribe_numerum(a, catena->semen->numerus_gregum);
        _scribe_literas(a, ", recti ");
    }
    alioquin
    {
        _scribe_literas(a, "  0 una: recti ");
    }
    _scribe_numerum(a, catena->initium->summa.recti);
    _scribe_lineam_novam(a);
    per (i = ZEPHYRUM; i < catena->numerus; i++)
    {
        constans PartitioAestimatio* prior = i > ZEPHYRUM
            ? catena->aestimationes[i - I] : catena->initium;
         constans PartitioAestimatio* e = catena->aestimationes[i];
                                 b32  primum = VERUM;

        _scribe_literas(a, "  ");
        _scribe_numerum(a, i + I);
        _scribe_literas(a, " + ");
        _scribe_chordam(a, _notae_titulus(c, catena->columnae[i]));
        _scribe_literas(a, ": greges ");
        _scribe_numerum(a, catena->partes[i]->numerus_gregum);
        _scribe_literas(a, ", recti ");
        _scribe_numerum(a, e->summa.recti);
        _scribe_literas(a, ", lucrum ");
        _scribe_signatum(a, (s32)e->summa.recti
            - (s32)prior->summa.recti);
        per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
        {
            si (e->sortes[s].recti < prior->sortes[s].recti)
            {
                _scribe_literas(a, primum ? ", cadit " : " ");
                _scribe_chordam(a, _sortis_titulus(c, s));
                primum = FALSUM;
            }
        }
        _scribe_lineam_novam(a);
    }
    _scribe_literas(a, "  finis: ");
    si (finis->columna < ZEPHYRUM)
    {
        _scribe_literas(a, "nulla columna restat");
    }
    alioquin si (finis->lucrum < (s32)c->lucrum_minimum)
    {
        _scribe_chordam(a, _notae_titulus(c, (i32)finis->columna));
        _scribe_literas(a, " lucrum ");
        _scribe_signatum(a, finis->lucrum);
        _scribe_literas(a, " < ");
        _scribe_numerum(a, c->lucrum_minimum);
    }
    alioquin si (finis->cadit_ulla)
    {
        _scribe_chordam(a, _notae_titulus(c, (i32)finis->columna));
        _scribe_literas(a, " cadit");
        per (s = ZEPHYRUM; s < c->sortes->numerus_gregum; s++)
        {
            si (finis->cadit[s])
            {
                _scribe_literas(a, " ");
                _scribe_chordam(a, _sortis_titulus(c, s));
            }
        }
    }
    alioquin
    {
        _scribe_chordam(a, _notae_titulus(c, (i32)finis->columna));
    }
    _scribe_lineam_novam(a);
}

interior vacuum
_greges_scribere (
    constans ContextusReticuli* c,
             ChordaAedificator* a)
{
    constans OratioReticulumOptiones* o = c->optiones;
                   constans Partitio* p = NIHIL;
         constans PartitioAestimatio* e = NIHIL;
                                 i32* indices;
                                 i32  numerus = ZEPHYRUM;
                                 i32  g;
                                 i32  s;

    indices = (i32*)piscina_allocare(c->piscina,
        (memoriae_index)(c->numerus_notarum + c->numerus_seminis + I)
        * magnitudo(i32));
    si (   indices == NIHIL || !_greges_eligere(c, &p, &e, indices,
        &numerus))
    {
        _scribe_literas(a, "GREGES ");
        _scribe_literas(a, o->greges);
        _scribe_literas(a, ": columna non inter notas\n");
        redde;
    }
    _scribe_literas(a, "GREGES ");
    _scribe_literas(a, o->greges);
    _scribe_lineam_novam(a);
    /* T36 c: habitus, margo suffragii cuiusque, captivus */
    per (g = ZEPHYRUM; g < p->numerus_gregum; g++)
    {
        i32 aureus;

        _scribe_literas(a, "  ");
        _gregem_scribere(a, c, p, indices, numerus, g, " / ");
        _scribe_literas(a, ": ordines ");
        _scribe_numerum(a, p->magnitudines[g]);
        _scribe_literas(a, "; aurum");
        per (aureus = ZEPHYRUM; aureus < e->numerus_aureorum; aureus++)
        {
            _scribe_literas(a, " ");
            _scribe_chordam(a, _auri_titulus(c, aureus));
            _scribe_literas(a, " ");
            _scribe_numerum(a, e->numeri[(memoriae_index)g
                * e->numerus_aureorum + aureus]);
        }
        _scribe_literas(a, "; habitus ");
        _scribe_literas(a, PARTITIO_TITULI_HABITUUM[e->habitus[g]]);
        _scribe_literas(a, "; suffragia");
        per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
        {
            memoriae_index locus = (memoriae_index)g
                * e->numerus_sortium + s;
                       i32 votum = e->suffragia[locus];

            _scribe_literas(a, " ");
            _scribe_chordam(a, _sortis_titulus(c, s));
            _scribe_literas(a, ":");
            si (votum == e->numerus_aureorum)
            {
                _scribe_literas(a, "-");
                perge;
            }
            _scribe_chordam(a, _auri_titulus(c, votum));
            _scribe_literas(a, "(");
            _scribe_numerum(a, e->margo[locus]);
            _scribe_literas(a, ")");
        }
        si (e->captivus[g] < e->numerus_sortium)
        {
            _scribe_literas(a, "; captivus ");
            _scribe_chordam(a, _sortis_titulus(c, e->captivus[g]));
        }
        si (e->mutabilis[g])
        {
            _scribe_literas(a, " !");
        }
        _scribe_lineam_novam(a);
    }
}

interior vacuum
_humanam_scribere (
    constans ContextusReticuli* c,
             ChordaAedificator* a)
{
    constans OratioReticulumOptiones* o = c->optiones;
         constans PartitioAestimatio* basis =
             c->catena_vetans->initium;
                                 i32 k = c->numerus_notarum;
                                 i32 q;
                                 i32 i;
                                 i32 j;
                                 i32 s;
                                 i32 d;
                                 b32 ullus;

    _scribe_literas(a, "reticulum ");
    _scribe_literas(a, c->via != NIHIL ? c->via : "");
    _scribe_literas(a, ": genus ");
    _scribe_literas(a, o->genus);
    _scribe_literas(a, ", ordines ");
    _scribe_numerum(a, c->numerus);
    _scribe_literas(a, ", sortes ");
    _scribe_numerum(a, c->sortes->numerus_gregum);
    _scribe_literas(a, ", limen ");
    _scribe_numerum(a, o->limen);
    _scribe_literas(a, ", lucrum minimum ");
    _scribe_numerum(a, c->lucrum_minimum);
    _scribe_lineam_novam(a);
    _scribe_literas(a, "  aurum ");
    _scribe_literas(a, o->aurum);
    _scribe_literas(a, ":");
    per (i = ZEPHYRUM; i < c->aurum->numerus_gregum; i++)
    {
        _scribe_literas(a, " ");
        _scribe_chordam(a, _auri_titulus(c, i));
        _scribe_literas(a, " ");
        _scribe_numerum(a, c->aurum->magnitudines[i]);
    }
    _scribe_literas(a, "; basis ");
    _scribe_numerum(a, basis->summa.basis);
    _scribe_literas(a, " (");
    _scribe_numerum(a, _puritas(&basis->summa));
    _scribe_literas(a, " permille)\n\n");

    _scribe_literas(a, "COLUMNAE SINGULAE (lucrum descendens)\n");
    per (q = ZEPHYRUM; q < k; q++)
    {
         constans PartitioAestimatio* e = c->aestimationes[c->ordo[q]];
                                 b32  primum = VERUM;

        si (e->inaestimabilis)
        {
            perge;
        }
        _scribe_literas(a, "  ");
        _scribe_chordam(a, _notae_titulus(c, c->ordo[q]));
        _scribe_literas(a, ": greges ");
        _scribe_numerum(a, e->numerus_gregum);
        _scribe_literas(a, ", puritas ");
        _scribe_numerum(a, _puritas(&e->summa));
        _scribe_literas(a, ", lucrum ");
        _scribe_signatum(a, e->lucrum);
        _scribe_literas(a, ", intra ");
        _scribe_numerum(a, e->puritas_intra);
        _scribe_literas(a, ", inaestimati ");
        _scribe_numerum(a, e->summa.inaestimati);
        _scribe_literas(a, ", mutabiles ");
        _scribe_numerum(a, e->greges_mutabiles);
        si (e->greges_captivi > ZEPHYRUM)
        {
            _scribe_literas(a, ", captivi ");
            _scribe_numerum(a, e->greges_captivi);
            _scribe_literas(a, " (");
            _scribe_numerum(a, e->ordines_captivi);
            _scribe_literas(a, ")");
        }
        per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
        {
            si (e->sortes[s].recti < e->sortes[s].basis)
            {
                _scribe_literas(a, primum ? ", cadit " : " ");
                _scribe_chordam(a, _sortis_titulus(c, s));
                primum = FALSUM;
            }
        }
        _scribe_lineam_novam(a);
    }
    ullus = FALSUM;
    per (q = ZEPHYRUM; q < k; q++)
    {
        si (c->aestimationes[c->ordo[q]]->inaestimabilis)
        {
            si (!ullus)
            {
                _scribe_literas(a, "  INAESTIMABILIS:");
                ullus = VERUM;
            }
            _scribe_literas(a, " ");
            _scribe_chordam(a, _notae_titulus(c, c->ordo[q]));
        }
    }
    si (ullus)
    {
        _scribe_lineam_novam(a);
    }
    si (c->numerus_constantium > ZEPHYRUM)
    {
        _scribe_literas(a, "  constantes:");
        per (i = ZEPHYRUM; i < k; i++)
        {
            si (c->constantes[i])
            {
                _scribe_literas(a, " ");
                _scribe_chordam(a, _notae_titulus(c, i));
            }
        }
        _scribe_lineam_novam(a);
    }

    _scribe_literas(a, "\nRETICULUM COLUMNARUM: altitudo ");
    _scribe_numerum(a, c->reticulum->altitudo);
    _scribe_literas(a, ", latitudo ");
    _scribe_numerum(a, c->reticulum->latitudo);
    _scribe_lineam_novam(a);
    per (i = ZEPHYRUM; i < c->numerus_reticuli; i++)
    {
        si (c->reticulum->geminus[i] != i)
        {
            _scribe_literas(a, "  aequalis: ");
            _scribe_chordam(a, _notae_titulus(c,
                c->reticulum_index[i]));
            _scribe_literas(a, " = ");
            _scribe_chordam(a, _notae_titulus(c,
                c->reticulum_index[c->reticulum->geminus[i]]));
            _scribe_lineam_novam(a);
        }
    }
    per (i = ZEPHYRUM; i < c->numerus_reticuli; i++)
    {
        per (j = ZEPHYRUM; j < c->numerus_reticuli; j++)
        {
            si (c->reticulum->tegit[i * c->numerus_reticuli + j])
            {
                _scribe_literas(a, "  tegit: ");
                _scribe_chordam(a, _notae_titulus(c,
                    c->reticulum_index[i]));
                _scribe_literas(a, " < ");
                _scribe_chordam(a, _notae_titulus(c,
                    c->reticulum_index[j]));
                _scribe_lineam_novam(a);
            }
        }
    }

    _scribe_literas(a, "\nCATENA SORTIBUS VETANTIBUS\n");
    _catenam_humanam(c, a, c->catena_vetans, &c->finis_vetans);
    _scribe_literas(a, "CATENA SINE VETO\n");
    _catenam_humanam(c, a, c->catena_libera, &c->finis_libera);
    d = ZEPHYRUM;
    dum (   d < c->catena_vetans->numerus
         && d < c->catena_libera->numerus
         && c->catena_vetans->columnae[d]
             == c->catena_libera->columnae[d])
    {
        d = d + I;
    }
    si (   d == c->catena_vetans->numerus
        && d == c->catena_libera->numerus)
    {
        _scribe_literas(a, "  catenae congruunt\n");
    }
    alioquin
    {
        _scribe_literas(a, "  divergentia: gradus ");
        _scribe_numerum(a, d + I);
        _scribe_lineam_novam(a);
    }

    /* CANDIDATI CONDICIONATI (decisio LIV): gradus catenae liberae
     * quorum sors ulla contra gradum priorem cadit */
    _scribe_literas(a, "\nCANDIDATI CONDICIONATI\n");
    ullus = FALSUM;
    per (i = ZEPHYRUM; i < c->catena_libera->numerus; i++)
    {
        constans PartitioAestimatio* prior = i > ZEPHYRUM
            ? c->catena_libera->aestimationes[i - I]
            : c->catena_libera->initium;
        constans PartitioAestimatio* e =
            c->catena_libera->aestimationes[i];
                                 b32 primum = VERUM;

        per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
        {
            si (e->sortes[s].recti >= prior->sortes[s].recti)
            {
                perge;
            }
            si (primum)
            {
                _scribe_literas(a, "  ");
                _scribe_chordam(a, _notae_titulus(c,
                    c->catena_libera->columnae[i]));
                _scribe_literas(a, " (gradus ");
                _scribe_numerum(a, i + I);
                _scribe_literas(a, "): ");
                primum = FALSUM;
            }
            alioquin
            {
                _scribe_literas(a, ", ");
            }
            _scribe_chordam(a, _sortis_titulus(c, s));
            _scribe_literas(a, " ");
            _scribe_numerum(a, prior->sortes[s].recti);
            _scribe_literas(a, " -> ");
            _scribe_numerum(a, e->sortes[s].recti);
        }
        si (!primum)
        {
            _scribe_lineam_novam(a);
            ullus = VERUM;
        }
    }
    si (!ullus)
    {
        _scribe_literas(a, "  nulli\n");
    }

    per (d = II; d <= o->profunditas; d++)
    {
        _scribe_literas(a, "\nINFIMA profunditate ");
        _scribe_numerum(a, d);
        _scribe_literas(a, " (prima ");
        _scribe_numerum(a, o->prima);
        _scribe_literas(a, ")\n");
        per (q = ZEPHYRUM; q < c->numerus_selectorum; q++)
        {
            constans InfimumReticuli* recordum =
                &c->infima[c->selecti[q]];

            si (recordum->profunditas != d)
            {
                perge;
            }
            _scribe_literas(a, "  ");
            _titulos_scribere(a, c, recordum->columnae,
                recordum->profunditas,
                " x ");
            _scribe_literas(a, ": greges ");
            _scribe_numerum(a, recordum->greges);
            _scribe_literas(a, ", recti ");
            _scribe_numerum(a, recordum->recti);
            _scribe_literas(a, ", incrementum ");
            _scribe_signatum(a, recordum->incrementum);
            _scribe_literas(a, ", cadentes ");
            _scribe_numerum(a, recordum->cadentes);
            _scribe_literas(a, ", inaestimati ");
            _scribe_numerum(a, recordum->inaestimati);
            _scribe_lineam_novam(a);
        }
    }
    si (o->greges != NIHIL)
    {
        _scribe_lineam_novam(a);
        _greges_scribere(c, a);
    }
}


/* ==================================================
 * Publica
 * ================================================== */

vacuum
oratio_reticulum_optiones_initium (
                    Piscina* piscina,
    OratioReticulumOptiones* optiones)
{
    si (optiones == NIHIL)
    {
        redde;
    }
    memset(optiones, ZEPHYRUM, magnitudo(*optiones));
    optiones->limen        = XX;
    optiones->profunditas  = II;
    optiones->prima        = XX;
    si (piscina != NIHIL)
    {
        optiones->ubi      = xar_creare(piscina,
            (i32)magnitudo(OratioReticulumUbi));
        optiones->columnae = xar_creare(piscina,
            (i32)magnitudo(chorda));
        optiones->praeter = xar_creare(piscina,
            (i32)magnitudo(chorda));
        optiones->gradus = xar_creare(piscina,
            (i32)magnitudo(chorda));
        optiones->initium = xar_creare(piscina,
            (i32)magnitudo(chorda));
    }
}

OratioReticulumExitus
oratio_reticulum_currere (
                             Piscina* piscina,
                              chorda  tsv,
                  constans character* via,
    constans OratioReticulumOptiones* optiones)
{
        ContextusReticuli  contextus;
        ChordaAedificator* a;
    OratioReticulumExitus  exitus;

    si (piscina == NIHIL)
    {
        redde _recusatio(NIHIL);
    }
    si (   optiones        == NIHIL || optiones->genus == NIHIL
        || optiones->aurum == NIHIL
        || (optiones->sortes == NIHIL
            && optiones->alternae == ZEPHYRUM))
    {
        redde _recusatio(_causa(piscina, "optiones deficiunt"));
    }
    memset(&contextus, ZEPHYRUM, magnitudo(contextus));
    contextus.piscina   = piscina;
    contextus.optiones  = optiones;
    contextus.via       = via;
    a                   = _legere(&contextus, tsv);
    si (a == NIHIL)
    {
        a = _optiones_iudicare(&contextus);
    }
    si (a == NIHIL)
    {
        a = _ordines_eligere(&contextus);
    }
    si (a == NIHIL)
    {
        a = _partes_struere(&contextus);
    }
    si (a == NIHIL)
    {
        a = _computare(&contextus);
    }
    si (a != NIHIL)
    {
        redde _recusatio(a);
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)MMMMXCVI);
    si (a == NIHIL)
    {
        redde _recusatio(_causa(piscina, "memoria deficit"));
    }
    si (optiones->machina)
    {
        _machinam_scribere(&contextus, a);
    }
    alioquin
    {
        _humanam_scribere(&contextus, a);
    }
    exitus.iudicatum  = VERUM;
    exitus.causa      = _vacua();
    exitus.relatio    = chorda_aedificator_finire(a);
    redde exitus;
}
