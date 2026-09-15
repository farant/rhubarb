/* oratio_reticulum.c - instrumentum reticuli partitionum (T35 d; vide
 * oratio_reticulum.h). Ordo operum: legere, optiones iudicare, ordines
 * eligere, partes struere, COMPUTARE (regiones piscinae notatae) - et
 * tum demum scribere: aedificator intra regionem crescens memoriam
 * reficiendam teneret. */

#include "oratio_reticulum.h"
#include "partitio.h"
#include "partitio_aestimatio.h"
#include "chorda_aedificator.h"
#include <string.h>

#define RETICULUM_PARIA_PRIMA XX
#define NUMERUS_TITULORUM(t) ((i32)(magnitudo(t) / magnitudo((t)[0])))

nomen structura {
    chorda* campi;     /* 0 via, 1 genus, 2.. columnae ordine capitis */
       i32  numerus;
       i32  linea;
} OrdoReticuli;

nomen structura {
    i32 a;
    i32 b;
    i32 greges;
    i32 recti;
    s32 incrementum;
    i32 cadentes;
} ParReticuli;

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
                         ParReticuli*  paria;
                                 i32   numerus_parium;
                                 i32   lucrum_minimum;
                                 i32*  ordo;
} ContextusReticuli;

hic_manens constans character* constans TITULI_COLUMNAE[] = {
    "columna", "greges", "ordines", "recti", "basis", "lucrum",
    "puritas", "puritas-intra", "inaestimati", "mutabiles", "cadentes",
    "inaestimabilis"
};
hic_manens constans character* constans TITULI_SORTIS[] = {
    "columna", "sors", "ordines", "recti", "basis", "inaestimati"
};
hic_manens constans character* constans TITULI_CATENAE[] = {
    "vetans", "gradus", "columna", "greges", "recti", "lucrum",
        "cadentes"
};
hic_manens constans character* constans TITULI_PARIS[] = {
    "columna-a", "columna-b", "greges", "recti", "incrementum",
        "cadentes"
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
    redde c->valores[c->index_sortium][c->sortes->primus[s]];
}

interior chorda
_auri_titulus (
    constans ContextusReticuli* c,
                           i32  aureus)
{
    redde c->valores[c->index_auri][c->aurum->primus[aureus]];
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
    c->index_sortium = _index_literis(c, o->sortes);
    si (c->index_sortium < ZEPHYRUM)
    {
        redde _literae_ignotae(c->piscina, o->sortes);
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
    si (   o->greges != NIHIL && strcmp(o->greges, "catena") != ZEPHYRUM
        && _index_literis(c, o->greges) < ZEPHYRUM)
    {
        redde _literae_ignotae(c->piscina, o->greges);
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
    a = _valores_struere(c, (i32)c->index_sortium);
    si (a != NIHIL)
    {
        redde a;
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
        c->valores[c->index_sortium]);
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
    redde NIHIL;
}


/* ==================================================
 * V. Computare
 * ================================================== */

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
                                  i32  s;
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
    c->reticulum = partitio_reticulum_struere(c->piscina, partes,
        k);
    c->lucrum_minimum = o->lucrum > ZEPHYRUM ? o->lucrum : o->limen;
    partitio_catena_optiones_initium(&oc);
    oc.limen           = o->limen;
    oc.lucrum_minimum  = c->lucrum_minimum;
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

    /* paria: numeri soli servantur, regio piscinae per par */
    c->numerus_parium = k * (k - I) / II;
    c->paria = (ParReticuli*)piscina_allocare(c->piscina,
        (memoriae_index)(c->numerus_parium
            > ZEPHYRUM ? c->numerus_parium
        : I) * magnitudo(ParReticuli));
    si (c->paria == NIHIL)
    {
        redde _causa(c->piscina, "memoria deficit");
    }
    q = ZEPHYRUM;
    per (i = ZEPHYRUM; i < k; i++)
    {
        per (j = i + I; j < k; j++)
        {
                 PiscinaNotatio  nota;
                       Partitio* infimum;
             PartitioAestimatio* e;
                    ParReticuli* par = &c->paria[q];

            nota = piscina_notare(c->piscina);
            infimum = partitio_infimum(c->piscina, c->partes[i],
                c->partes[j]);
            e = infimum != NIHIL
                ? partitio_aestimare(c->piscina, infimum, c->aurum,
                    c->sortes, o->limen)
                : NIHIL;
            par->a            = i;
            par->b            = j;
            par->greges       = ZEPHYRUM;
            par->recti        = ZEPHYRUM;
            par->incrementum  = (s32)ZEPHYRUM;
            par->cadentes     = ZEPHYRUM;
            si (e != NIHIL)
            {
                i32 optima_singula =
                    c->aestimationes[i]->summa.recti
                    > c->aestimationes[j]->summa.recti
                    ? c->aestimationes[i]->summa.recti
                    : c->aestimationes[j]->summa.recti;

                par->greges  = infimum->numerus_gregum;
                par->recti   = e->summa.recti;
                par->incrementum = (s32)e->summa.recti
                    - (s32)optima_singula;
                per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
                {
                    si (e->sortes[s].recti < e->sortes[s].basis)
                    {
                        par->cadentes = par->cadentes + I;
                    }
                }
            }
            piscina_reficere(c->piscina, nota);
            q = q + I;
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
    /* paria: incrementum descendens, par -> ordo priorum */
    per (i = I; i < c->numerus_parium; i++)
    {
        ParReticuli par = c->paria[i];

        j = i;
        dum (   j > ZEPHYRUM
             && c->paria[j - I].incrementum < par.incrementum)
        {
            c->paria[j]  = c->paria[j - I];
            j            = j - I;
        }
        c->paria[j] = par;
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
    redde summa->ordines > ZEPHYRUM ? summa->recti * M / summa->ordines
        : ZEPHYRUM;
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
    _scribe_literas(a, "-");
    _campus_numeri(a, I);
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
    _caput_machinae(a, via, "RETICULUM-PAR", TITULI_PARIS,
        NUMERUS_TITULORUM(TITULI_PARIS));
    per (q = ZEPHYRUM; q < c->numerus_parium
        && q < RETICULUM_PARIA_PRIMA;
        q++)
    {
        constans ParReticuli* par = &c->paria[q];

        _ordinem_incipere(a, via, "RETICULUM-PAR");
        _campus_chordae(a, _notae_titulus(c, par->a));
        _campus_chordae(a, _notae_titulus(c, par->b));
        _campus_numeri(a, par->greges);
        _campus_numeri(a, par->recti);
        _campus_integri(a, par->incrementum);
        _campus_numeri(a, par->cadentes);
        _scribe_lineam_novam(a);
    }
    _caput_machinae(a, via, "RETICULUM-TEGIT", TITULI_TEGMINIS,
        NUMERUS_TITULORUM(TITULI_TEGMINIS));
    per (i = ZEPHYRUM; i < k; i++)
    {
        per (j = ZEPHYRUM; j < k; j++)
        {
            si (c->reticulum->tegit[i * k + j])
            {
                _ordinem_incipere(a, via, "RETICULUM-TEGIT");
                _campus_chordae(a, _notae_titulus(c, i));
                _campus_chordae(a, _notae_titulus(c, j));
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
            _campus_chordae(a, _notae_titulus(c, i));
            _campus_chordae(a, _notae_titulus(c,
                c->reticulum->geminus[i]));
            _scribe_lineam_novam(a);
        }
    }
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

    _scribe_literas(a, "  0 una: recti ");
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
    constans OratioReticulumOptiones* o       = c->optiones;
             constans PartitioCatena* catena  = c->catena_vetans;
                   constans Partitio* p;
         constans PartitioAestimatio* e;
                                 b32  catenae = (b32)(strcmp(o->greges,
                                     "catena") == ZEPHYRUM);
                              s32 nota    = (s32)-I;
                              i32 g;
                              i32 i;
                              i32 s;

    si (catenae)
    {
        p = catena->numerus > ZEPHYRUM
            ? catena->partes[catena->numerus - I] : catena->semen;
        e = catena->numerus > ZEPHYRUM
            ? catena->aestimationes[catena->numerus
                - I] : catena->initium;
    }
    alioquin
    {
        s32 index = _index_literis(c, o->greges);

        per (i = ZEPHYRUM; i < c->numerus_notarum; i++)
        {
            si ((s32)c->notae[i] == index)
            {
                nota = (s32)i;
            }
        }
        si (nota < ZEPHYRUM)
        {
            _scribe_literas(a, "GREGES ");
            _scribe_literas(a, o->greges);
            _scribe_literas(a, ": columna non inter notas\n");
            redde;
        }
        p = c->partes[nota];
        e = c->aestimationes[nota];
    }
    _scribe_literas(a, "GREGES ");
    _scribe_literas(a, o->greges);
    _scribe_lineam_novam(a);
    per (g = ZEPHYRUM; g < p->numerus_gregum; g++)
    {
        i32 primum     = e->numerus_aureorum;
        b32 mutabilis  = FALSUM;
        i32 aureus;

        _scribe_literas(a, "  ");
        si (catenae && catena->numerus == ZEPHYRUM)
        {
            _scribe_literas(a, "(una)");
        }
        alioquin si (catenae)
        {
            per (i = ZEPHYRUM; i < catena->numerus; i++)
            {
                si (i > ZEPHYRUM)
                {
                    _scribe_literas(a, " / ");
                }
                _scribe_chordam(a,
                    c->valores[c->notae[catena->columnae[i]]]
                    [p->primus[g]]);
            }
        }
        alioquin
        {
            _scribe_chordam(a,
                c->valores[c->notae[nota]][p->primus[g]]);
        }
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
        _scribe_literas(a, "; suffragia");
        per (s = ZEPHYRUM; s < e->numerus_sortium; s++)
        {
            i32 votum =
                e->suffragia[(memoriae_index)g * e->numerus_sortium
                + s];

            _scribe_literas(a, " ");
            _scribe_chordam(a, _sortis_titulus(c, s));
            _scribe_literas(a, ":");
            si (votum == e->numerus_aureorum)
            {
                _scribe_literas(a, "-");
                perge;
            }
            _scribe_chordam(a, _auri_titulus(c, votum));
            si (primum == e->numerus_aureorum)
            {
                primum = votum;
            }
            alioquin si (votum != primum)
            {
                mutabilis = VERUM;
            }
        }
        si (mutabilis)
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

    _scribe_literas(a, "\nRETICULUM COLUMNARUM: altitudo ");
    _scribe_numerum(a, c->reticulum->altitudo);
    _scribe_literas(a, ", latitudo ");
    _scribe_numerum(a, c->reticulum->latitudo);
    _scribe_lineam_novam(a);
    per (i = ZEPHYRUM; i < k; i++)
    {
        si (c->reticulum->geminus[i] != i)
        {
            _scribe_literas(a, "  aequalis: ");
            _scribe_chordam(a, _notae_titulus(c, i));
            _scribe_literas(a, " = ");
            _scribe_chordam(a, _notae_titulus(c,
                c->reticulum->geminus[i]));
            _scribe_lineam_novam(a);
        }
    }
    per (i = ZEPHYRUM; i < k; i++)
    {
        per (j = ZEPHYRUM; j < k; j++)
        {
            si (c->reticulum->tegit[i * k + j])
            {
                _scribe_literas(a, "  tegit: ");
                _scribe_chordam(a, _notae_titulus(c, i));
                _scribe_literas(a, " < ");
                _scribe_chordam(a, _notae_titulus(c, j));
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

    _scribe_literas(a, "\nPARIA (incrementum descendens, XX prima)\n");
    per (q = ZEPHYRUM; q < c->numerus_parium
        && q < RETICULUM_PARIA_PRIMA;
        q++)
    {
        constans ParReticuli* par = &c->paria[q];

        _scribe_literas(a, "  ");
        _scribe_chordam(a, _notae_titulus(c, par->a));
        _scribe_literas(a, " x ");
        _scribe_chordam(a, _notae_titulus(c, par->b));
        _scribe_literas(a, ": greges ");
        _scribe_numerum(a, par->greges);
        _scribe_literas(a, ", recti ");
        _scribe_numerum(a, par->recti);
        _scribe_literas(a, ", incrementum ");
        _scribe_signatum(a, par->incrementum);
        _scribe_literas(a, ", cadentes ");
        _scribe_numerum(a, par->cadentes);
        _scribe_lineam_novam(a);
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
    optiones->limen = XX;
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
        || optiones->aurum == NIHIL || optiones->sortes == NIHIL)
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
