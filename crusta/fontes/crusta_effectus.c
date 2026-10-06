/* crusta_effectus.c - summarium effectuum scripti. Vide
 * crusta_effectus.h (situs, regulae, limites).
 *
 * Aestimator verborum, ambitus et definitiones ex crusta_fontationes.c
 * TRANSLATI sunt (effectus-plan T3, sectiones notatae); fontationes
 * T4 proiectio huius summarii factae, T8 retiratae (genus fabricae
 * 'effectus' eas subsumit). Nova: situs omnium generum, tabula
 * mandatorum, scripta_in_ambitu, emissio STML; causae irresolutorum
 * nominatae (effectus-plan-2 T1). */

#include "latina.h"
#include "crusta_effectus.h"
#include "crusta_arbor.h"
#include "crusta_registrum.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "filum.h"
#include "chorda.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VIA_MAXIMA          (IV * MXXIV)
#define PROFUNDITAS_MAXIMA  XVI
#define AMBITUS_MAXIMI      LXIV


/* ==================================================
 * Structurae internae
 * ================================================== */

nomen structura {
                character* via;      /* absoluta */
    constans MateriaNodus* radix;  /* NIHIL = illegibilis / non sana */
       constans character* causa;     /* cur radix NIHIL */
                      i32  ordo;      /* index in plagulis ambitus */
                      Xar* imperia;       /* MateriaNodus* */
                      Xar* assignationes; /* MateriaNodus* */
                      Xar* iterationes;   /* MateriaNodus* */
                      Xar* redirectiones; /* MateriaNodus* */
                      Xar* iudicia;       /* [[ ]] probationes */
                      Xar* verba;         /* verba imperii et 'for' */
                      Xar* variabiles;    /* $X, ${X} */
                      Xar* functiones;    /* MateriaNodus* functio */
} Plagula;

nomen structura {
                character* titulus;
    /* NIHIL et !inanis = ignotum (causa dicit cur) */
    constans MateriaNodus* verbum;
                      b32  inanis;     /* 'X=' : chorda vacua */
    constans MateriaNodus* functio;    /* NIHIL = globalis */
                  Plagula* plagula;
       constans character* causa;
} Definitio;

nomen structura {
    constans MateriaNodus* functio;
                character* titulus;
} Localis;

/* situs effectus unus - attributa dialecti (NIHIL = absens) */
nomen structura {
      constans character* elementum;
               character* via;
               character* exemplar;
               character* titulus;    /* ambitus_lectio */
               character* textus;     /* verbum fontis, ubi partialis */
               character* custodia;   /* '[ -x P ] || S': P */
      constans character* forma;
      constans character* resolutio;
      constans character* classis;
      constans character* medium;
               character* mandatum;
               character* operator;
      constans character* causa;
                     s32  scripta;     /* -I absens, 0, I */
                 Plagula* plagula;
          MateriaTractus  tractus;
} Situs;

nomen structura {
    character* radix_via;   /* $0 ambitus */
          Xar* plagulae;    /* Plagula* */
          Xar* definitiones;
          Xar* locales;
          Xar* functiones;  /* character*: nomina functionum ambitus */
          Xar* assignata;   /* character*: nomina quae ambitus ponit */
          Xar* situs;       /* Situs - iterationis currentis */
    character* custodia;    /* aedificator custoditus: P */
          b32  liber;       /* attingitur via sine custodia */
} Ambitus;

/* arcus exsecutionis: processus pater -> scriptum exsecutum */
nomen structura {
      Ambitus* pater;
    character* filius;     /* radix absoluta ambitus filii */
    character* custodia;   /* NIHIL = sine custodia */
} Arcus;

/* ordo mandati in tabula */
nomen structura {
     character* titulus;
     StmlNodus* nodus;
} Mandatum;

nomen structura {
                Piscina* piscina;
    InternamentumChorda* intern;
     constans character* radix;
                    i32  radix_mensura;
                    Xar* visi;       /* character*: radices ambituum */
                    Xar* ambitus;    /* Ambitus*, ordine inventionis */
                    Xar* tabula;     /* Mandatum */
                    Xar* arcus;      /* Arcus */
    /* causa defectus aestimationis; tituli in aestimatione (cyclus) */
     constans character* causa;
                    Xar* acervus;
} Derivatio;


/* ---- translata ex crusta_fontationes.c (T3) ---- */


/* ==================================================
 * Chordae et viae
 * ================================================== */

interior character*
_duplicare (
               Piscina* piscina,
    constans character* s)
{
    memoriae_index  n = strlen(s);
         character* d = (character*)piscina_allocare(piscina, n + I);

    si (d != NIHIL)
    {
        memcpy(d, s, n + I);
    }
    redde d;
}

/* textum appendere in aream VIA_MAXIMA; FALSUM = nimis longum */
interior b32
_appendere (
            character* area,
                  i32* longitudo,
    constans character* s,
                  i32  n)
{
    si (*longitudo + n + I > (i32)VIA_MAXIMA)
    {
        redde FALSUM;
    }
    memcpy(area + *longitudo, s, (size_t)n);
    *longitudo        += n;
    area[*longitudo]  = '\0';
    redde VERUM;
}

interior b32
_aequalis (
                chorda  c,
    constans character* s)
{
    redde chorda_aequalis_literis(c, s);
}

/* viam absolutam normare in loco: '//' '.' '..' solvuntur */
interior vacuum
_viam_normare (
    character* via)
{
    character exitus[VIA_MAXIMA];
          i32 initia[VIA_MAXIMA / II];
          i32 numerus    = ZEPHYRUM;
          i32 longitudo  = ZEPHYRUM;
          i32 k          = ZEPHYRUM;
          i32 n          = (i32)strlen(via);

    exitus[ZEPHYRUM] = '\0';
    dum (k < n)
    {
        i32 ab;
        i32 mensura;

        dum (k < n && via[k] == '/')
        {
            k++;
        }
        ab = k;
        dum (k < n && via[k] != '/')
        {
            k++;
        }
        mensura = k - ab;
        si (mensura == ZEPHYRUM || (mensura == I && via[ab] == '.'))
        {
            perge;
        }
        si (mensura == II && via[ab] == '.' && via[ab + I] == '.')
        {
            si (numerus > ZEPHYRUM)
            {
                numerus--;
                longitudo = initia[numerus];
            }
            perge;
        }
        si (numerus >= (i32)(VIA_MAXIMA / II))
        {
            redde;
        }
        initia[numerus] = longitudo;
        numerus++;
        exitus[longitudo] = '/';
        longitudo++;
        memcpy(exitus + longitudo, via + ab, (size_t)mensura);
        longitudo += mensura;
    }
    si (longitudo == ZEPHYRUM)
    {
        exitus[longitudo] = '/';
        longitudo++;
    }
    exitus[longitudo] = '\0';
    memcpy(via, exitus, (size_t)longitudo + I);
}

/* via absoluta ex via et directorio (via relativa -> dir/via) */
interior b32
_absolutam_facere (
    constans character* via,
    constans character* directorium,
             character* exitus)
{
    i32 longitudo = ZEPHYRUM;

    exitus[ZEPHYRUM] = '\0';
    si (via[ZEPHYRUM] != '/')
    {
        si (   !_appendere(exitus, &longitudo, directorium,
                   (i32)strlen(directorium))
            || !_appendere(exitus, &longitudo, "/", I))
        {
            redde FALSUM;
        }
    }
    si (!_appendere(exitus, &longitudo, via, (i32)strlen(via)))
    {
        redde FALSUM;
    }
    _viam_normare(exitus);
    redde VERUM;
}

/* dirname in loco: "/a/b" -> "/a", "/a" -> "/", "a" -> "." */
interior vacuum
_directorium_viae (
    character* via)
{
    s32 k = (s32)strlen(via) - I;

    dum (k > ZEPHYRUM && via[k] == '/')
    {
        k--;
    }
    dum (k >= ZEPHYRUM && via[k] != '/')
    {
        k--;
    }
    si (k < ZEPHYRUM)
    {
        strcpy(via, ".");
        redde;
    }
    dum (k > ZEPHYRUM && via[k] == '/')
    {
        k--;
    }
    via[k + I] = '\0';
}

interior vacuum
_caudam_viae (
    character* via)
{
    s32 n = (s32)strlen(via);
    s32 k;

    dum (n > I && via[n - I] == '/')
    {
        n--;
    }
    via[n]  = '\0';
    k       = n - I;
    dum (k >= ZEPHYRUM && via[k] != '/')
    {
        k--;
    }
    memmove(via, via + k + I, (size_t)(n - k));
}

/* via arbori relativa, aut NIHIL si extra arborem */
interior constans character*
_relativa (
             Derivatio* d,
    constans character* via)
{
    si (   strncmp(via, d->radix, (size_t)d->radix_mensura) == ZEPHYRUM
        && via[d->radix_mensura]                            == '/')
    {
        redde via + d->radix_mensura + I;
    }
    redde NIHIL;
}

/* sub build/ : 'build/...' aut '.../build/...' */
interior b32
_sub_build (
    constans character* relativa)
{
    redde strncmp(relativa, "build/", VI) == ZEPHYRUM
        || strstr(relativa, "/build/") != NIHIL;
}


/* ==================================================
 * Nodi
 * ================================================== */

interior constans MateriaToken*
_token (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (   nodus == NIHIL || locus >= nodus->numerus_locorum
        || nodus->loci[locus].genus != MATERIA_VALOR_TOKEN)
    {
        redde NIHIL;
    }
    redde nodus->loci[locus].datum.token;
}

interior constans MateriaNodus*
_nodus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (   nodus == NIHIL || locus >= nodus->numerus_locorum
        || nodus->loci[locus].genus != MATERIA_VALOR_NODUS)
    {
        redde NIHIL;
    }
    redde nodus->loci[locus].datum.nodus;
}

/* locus vacuus: NIHIL aut lista sine elementis */
interior b32
_locus_vacuus (
    constans MateriaNodus* nodus,
                      i32  locus)
{
    si (nodus == NIHIL || locus >= nodus->numerus_locorum)
    {
        redde VERUM;
    }
    si (nodus->loci[locus].genus == MATERIA_VALOR_NIHIL)
    {
        redde VERUM;
    }
    si (nodus->loci[locus].genus == MATERIA_VALOR_LISTA)
    {
        redde materia_valor_lista_numerus(nodus->loci[locus])
            == ZEPHYRUM;
    }
    redde FALSUM;
}

/* nodi listae loci (Xar de MateriaNodus*), separatoribus omissis */
interior Xar*
_nodi_listae (
                  Piscina* piscina,
    constans MateriaNodus* nodus,
                      i32  locus)
{
    Xar* exitus = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));
    i32  k;

    si (   exitus                   == NIHIL || nodus == NIHIL
        || locus                    >= nodus->numerus_locorum
        || nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde exitus;
    }
    per (k = ZEPHYRUM;
         k < materia_valor_lista_numerus(nodus->loci[locus]); k++)
    {
        MateriaValor* e = materia_valor_lista_obtinere(
            nodus->loci[locus], k);

        si (   e->genus              == MATERIA_VALOR_NODUS
            && e->datum.nodus->genus != (s32)CRUSTA_GENUS_SEPARATOR)
        {
            *(MateriaNodus**)xar_addere(exitus) = e->datum.nodus;
        }
    }
    redde exitus;
}

interior constans MateriaNodus*
_functio_circumdans (
    constans MateriaNodus* nodus)
{
    constans MateriaNodus* n = nodus;

    dum (n != NIHIL)
    {
        si (n->genus == (s32)CRUSTA_GENUS_FUNCTIO)
        {
            redde n;
        }
        n = n->pater;
    }
    redde NIHIL;
}

/* lexemata nodi ordine (sine triviis) in aream */
interior vacuum
_textum_colligere (
    constans MateriaNodus* nodus,
                character* area,
                      i32* longitudo)
{
    i32 k;
    i32 j;

    si (nodus == NIHIL)
    {
        redde;
    }
    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        constans MateriaValor* v = &nodus->loci[k];

        si (v->genus == MATERIA_VALOR_TOKEN)
        {
            (vacuum)_appendere(area, longitudo,
                (constans character*)v->datum.token->valor.datum,
                (i32)v->datum.token->valor.mensura);
        }
        alioquin si (v->genus == MATERIA_VALOR_NODUS)
        {
            _textum_colligere(v->datum.nodus, area, longitudo);
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            per (j = ZEPHYRUM; j < materia_valor_lista_numerus(*v); j++)
            {
                MateriaValor* e = materia_valor_lista_obtinere(*v, j);

                si (e->genus == MATERIA_VALOR_TOKEN)
                {
                    chorda tv = e->datum.token->valor;

                    (vacuum)_appendere(area, longitudo,
                        (constans character*)tv.datum, (i32)tv.mensura);
                }
                alioquin si (e->genus == MATERIA_VALOR_NODUS)
                {
                    _textum_colligere(e->datum.nodus, area, longitudo);
                }
            }
        }
    }
}

/* titulus staticus imperii in C (NIHIL si nullus aut non staticus) */
interior character*
_titulus_staticus (
                  Piscina* piscina,
    constans MateriaNodus* verbum)
{
    chorda valor;

    si (   verbum == NIHIL
        || !crusta_verbum_staticum(piscina, verbum, &valor))
    {
        redde NIHIL;
    }
    redde chorda_ut_cstr(valor, piscina);
}


/* ==================================================
 * Aestimatio verborum
 * ================================================== */

/* CAUSA (effectus-plan-2 T1): defectus PRIMUS vincit - intimus ante
 * exteriorem advenit, ergo '"$v"' cum v="$1" 'argumentum' dicit, non
 * causam exteriorem. Vocabularium in effectus.canon (causa). */
interior b32
_deficere (
             Derivatio* d,
    constans character* causa)
{
        si (d->causa == NIHIL)
        {
        d->causa = causa;
        }
    redde FALSUM;
}

interior b32
_in_nominibus (
                   Xar* nomina,
    constans character* verbum);


interior b32
_partes_aestimare (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* nodus,
                      i32  locus,
                character* area,
                      i32* longitudo,
                      i32  profunditas);

interior b32
_verbum_aestimare (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* verbum,
                character* area,
                      i32* longitudo,
                      i32  profunditas)
{
    si (verbum == NIHIL || verbum->genus != (s32)CRUSTA_GENUS_VERBUM)
    {
        redde FALSUM;
    }
    redde _partes_aestimare(d, a, p, verbum, (i32)CRUSTA_VERBUM_PARTES,
        area, longitudo, profunditas);
}

interior b32
_localis_est (
                  Ambitus* a,
    constans MateriaNodus* functio,
       constans character* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(a->locales); k++)
    {
        Localis* l = (Localis*)xar_obtinere(a->locales, k);

        si (   l->functio                  == functio
            && strcmp(l->titulus, titulus) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* $titulus ad locum 'usus': definitiones ambitus (locales functionis
 * circumdantis si declaratae) - omnes plenae et aequales, aliter
 * FALSUM (praefixum definitionis unicae servatur). inanes_omittere:
 * verbum TOTUM est '$X' in sede imperii/fontationis - definitio vacua
 * ('X=""' ante 'X=via') plagulam nullam nominat, ergo omittitur (intra
 * verbum longius '"$P/x.sh"' viam mutaret: numquam omittitur) */
interior b32
_variabilem_intus (
              Derivatio* d,
                Ambitus* a,
                Plagula* p,
    constans MateriaNodus* usus,
    constans character* titulus,
              character* area,
                    i32* longitudo,
                    i32  profunditas,
                    b32  inanes_omittere)
{
    constans MateriaNodus* functio;
                      b32  localis;
                character  primus[VIA_MAXIMA];
                      i32  primus_longitudo  = ZEPHYRUM;
                      i32  numerus           = ZEPHYRUM;
                      b32  plenus            = VERUM;
                      i32  k;

    si (strcmp(titulus, "0") == ZEPHYRUM)
    {
        redde _appendere(area, longitudo, a->radix_via,
            (i32)strlen(a->radix_via));
    }
    si (strcmp(titulus, "BASH_SOURCE") == ZEPHYRUM)
    {
        redde _appendere(area, longitudo, p->via, (i32)strlen(p->via));
    }
        si (profunditas > PROFUNDITAS_MAXIMA)
        {
        redde _deficere(d, "profunditas");
        }
    functio = _functio_circumdans(usus);
    localis = functio != NIHIL && _localis_est(a, functio, titulus);
    primus[ZEPHYRUM] = '\0';
    per (k = ZEPHYRUM; k < xar_numerus(a->definitiones); k++)
    {
         Definitio* def = (Definitio*)xar_obtinere(a->definitiones, k);
         character  valor[VIA_MAXIMA];
               i32  valor_longitudo = ZEPHYRUM;
               b32  bonus;

        si (strcmp(def->titulus, titulus) != ZEPHYRUM)
        {
            perge;
        }
        si (localis ? def->functio != functio : def->functio != NIHIL)
        {
            perge;
        }
        valor[ZEPHYRUM] = '\0';
        si (def->inanis)
        {
            bonus = VERUM;
        }
                alioquin si (def->verbum == NIHIL)
        {
            bonus = _deficere(d, def->causa);
        }
        alioquin
        {
            bonus = _verbum_aestimare(d, a, def->plagula, def->verbum,
                valor, &valor_longitudo, profunditas + I);
        }
        si (inanes_omittere && bonus && valor_longitudo == ZEPHYRUM)
        {
            perge;
        }
        si (numerus == ZEPHYRUM)
        {
            memcpy(primus, valor, (size_t)valor_longitudo + I);
            primus_longitudo  = valor_longitudo;
            plenus            = bonus;
        }
                alioquin si (   !bonus || !plenus
                             || strcmp(primus, valor) != ZEPHYRUM)
        {
            /* definitiones discordes aut ignotae (ignotae causam suam
             * iam posuerunt) */
            redde _deficere(d, "discordia");
        }
        numerus++;
    }
        si (numerus == ZEPHYRUM)
        {
        /* argumentum ($1 $@ ...), nomen a read/mapfile positum, aut
         * ambitus externus */
        redde _deficere(d,
              (titulus[ZEPHYRUM] >= '0' && titulus[ZEPHYRUM] <= '9')
            || strcmp(titulus, "@") == ZEPHYRUM
            || strcmp(titulus, "*") == ZEPHYRUM
            || strcmp(titulus, "#") == ZEPHYRUM ? "argumentum"
            : _in_nominibus(a->assignata, titulus) ? "ansa_read"
            : "ambitus");
        }
    si (!_appendere(area, longitudo, primus, primus_longitudo))
    {
        redde FALSUM;
    }
    redde plenus;
}

/* RECURSIO (plan-2 T1): definitio quae se ipsam nominat
 * (obj_files="$obj_files $obj" in ansa) valorem finitum non habet;
 * acervus titulorum in aestimatione cyclum nominat antequam
 * profunditas eum celet */
interior b32
_variabilem_aestimare (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* usus,
       constans character* titulus,
                character* area,
                      i32* longitudo,
                      i32  profunditas,
                      b32  inanes_omittere)
{
    b32 bonus;
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(d->acervus); k++)
    {
        si (strcmp(*(constans character**)xar_obtinere(d->acervus, k),
                titulus) == ZEPHYRUM)
        {
            redde _deficere(d, "recursio");
        }
    }
    *(constans character**)xar_addere(d->acervus) = titulus;
    bonus = _variabilem_intus(d, a, p, usus, titulus, area, longitudo,
        profunditas, inanes_omittere);
    xar_truncare(d->acervus, xar_numerus(d->acervus) - I);
    redde bonus;
}

/* $( ... ): idiomata sola (vide caput) */
interior b32
_substitutionem_aestimare (
              Derivatio* d,
                Ambitus* a,
                Plagula* p,
    constans MateriaNodus* pars,
              character* area,
                    i32* longitudo,
                    i32  profunditas)
{
                      Xar* sententiae;
    constans MateriaNodus* s;
                character  valor[VIA_MAXIMA];
                      i32  valor_longitudo = ZEPHYRUM;
                      Xar* argumenta;
                character* t;

    valor[ZEPHYRUM] = '\0';
    sententiae = _nodi_listae(d->piscina, pars,
        (i32)CRUSTA_SUBSTITUTIO_LIBERI);
    si (sententiae == NIHIL || xar_numerus(sententiae) != I)
    {
        redde FALSUM;
    }
    s = *(MateriaNodus**)xar_obtinere(sententiae, ZEPHYRUM);
    si (s->genus == (s32)CRUSTA_GENUS_CATENA)
    {
                         Xar* membra = _nodi_listae(d->piscina, s,
                                  (i32)CRUSTA_CATENA_LIBERI);
        constans MateriaNodus* cd;
        constans MateriaNodus* op;
        constans MateriaNodus* secundum;
        constans MateriaToken* tok;
                    character* tp;
                          Xar* argumenta_secundi;
                          i32  k;
                    character  absoluta[VIA_MAXIMA];

        si (membra == NIHIL || xar_numerus(membra) != III)
        {
            redde FALSUM;
        }
        cd        = *(MateriaNodus**)xar_obtinere(membra, ZEPHYRUM);
        op        = *(MateriaNodus**)xar_obtinere(membra, I);
        secundum  = *(MateriaNodus**)xar_obtinere(membra, II);
        tok       = _token(op, (i32)CRUSTA_OPERATOR_TOK);
        si (   cd->genus       != (s32)CRUSTA_GENUS_IMPERIUM
            || secundum->genus != (s32)CRUSTA_GENUS_IMPERIUM
            || tok             == NIHIL || !_aequalis(tok->valor, "&&"))
        {
            redde FALSUM;
        }
        t = _titulus_staticus(d->piscina, crusta_imperium_titulus(cd));
        tp = _titulus_staticus(d->piscina,
            crusta_imperium_titulus(secundum));
        argumenta = crusta_imperium_argumenta(d->piscina, cd);
        argumenta_secundi = crusta_imperium_argumenta(d->piscina,
            secundum);
        si (   t == NIHIL || tp == NIHIL || strcmp(t, "cd") != ZEPHYRUM
            || strcmp(tp, "pwd") != ZEPHYRUM || argumenta == NIHIL
            || xar_numerus(argumenta) != I
            || argumenta_secundi == NIHIL)
        {
            redde FALSUM;
        }
        per (k = ZEPHYRUM; k < xar_numerus(argumenta_secundi); k++)
        {
            character* o = _titulus_staticus(d->piscina,
                *(MateriaNodus**)xar_obtinere(argumenta_secundi, k));

            si (o == NIHIL || o[ZEPHYRUM] != '-')
            {
                redde FALSUM;
            }
        }
        si (   !_verbum_aestimare(d, a, p,
                *(MateriaNodus**)xar_obtinere(argumenta, ZEPHYRUM),
                valor, &valor_longitudo, profunditas + I)
            || !_absolutam_facere(valor, d->radix, absoluta))
        {
            redde FALSUM;
        }
        redde _appendere(area, longitudo, absoluta,
            (i32)strlen(absoluta));
    }
    si (s->genus != (s32)CRUSTA_GENUS_IMPERIUM)
    {
        redde FALSUM;
    }
    t = _titulus_staticus(d->piscina, crusta_imperium_titulus(s));
    argumenta = crusta_imperium_argumenta(d->piscina, s);
    si (t == NIHIL || argumenta == NIHIL)
    {
        redde FALSUM;
    }
    si (   (strcmp(t, "dirname") == ZEPHYRUM
            || strcmp(t, "basename") == ZEPHYRUM)
        && xar_numerus(argumenta) == I)
    {
        si (!_verbum_aestimare(d, a, p,
                *(MateriaNodus**)xar_obtinere(argumenta, ZEPHYRUM),
                valor, &valor_longitudo, profunditas + I))
        {
            redde FALSUM;
        }
        si (t[ZEPHYRUM] == 'd')
        {
            _directorium_viae(valor);
        }
        alioquin
        {
            _caudam_viae(valor);
        }
        redde _appendere(area, longitudo, valor, (i32)strlen(valor));
    }
    si (   strcmp(t, "readlink")  == ZEPHYRUM
        && xar_numerus(argumenta) == II)
    {
        character* o = _titulus_staticus(d->piscina,
            *(MateriaNodus**)xar_obtinere(argumenta, ZEPHYRUM));
        character absoluta[VIA_MAXIMA];

        si (   o == NIHIL || strcmp(o, "-f") != ZEPHYRUM
            || !_verbum_aestimare(d, a, p,
                   *(MateriaNodus**)xar_obtinere(argumenta, I),
                   valor, &valor_longitudo, profunditas + I)
            || !_absolutam_facere(valor, d->radix, absoluta))
        {
            redde FALSUM;
        }
        redde _appendere(area, longitudo, absoluta,
            (i32)strlen(absoluta));
    }
    redde FALSUM;
}

interior b32
_partes_aestimare (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* nodus,
                      i32  locus,
                character* area,
                      i32* longitudo,
                      i32  profunditas)
{
    constans MateriaValor* partes;
                      i32  k;

    si (nodus == NIHIL || locus >= nodus->numerus_locorum)
    {
        redde VERUM;
    }
    partes = &nodus->loci[locus];
    si (partes->genus != MATERIA_VALOR_LISTA)
    {
        redde VERUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*partes); k++)
    {
                 MateriaValor* e = materia_valor_lista_obtinere(*partes,
                     k);
        constans MateriaNodus* pars;
        constans MateriaToken* t;
                    character* titulus;

        si (e->genus != MATERIA_VALOR_NODUS)
        {
            perge;
        }
        pars = e->datum.nodus;
        commutatio (pars->genus)
        {
            casus CRUSTA_GENUS_PARS_LITTERALIS:
                t = _token(pars, (i32)CRUSTA_PARS_TOK);
                si (   t == NIHIL || !_appendere(area, longitudo,
                        (constans character*)t->valor.datum,
                        (i32)t->valor.mensura))
                {
                    redde FALSUM;
                }
                frange;
            casus CRUSTA_GENUS_PARS_EFFUGIUM:
                t = _token(pars, (i32)CRUSTA_PARS_TOK);
                si (t == NIHIL)
                {
                    redde FALSUM;
                }
                si (   t->valor.mensura >= II
                    && !_appendere(area, longitudo,
                        (constans character*)t->valor.datum + I,
                        (i32)t->valor.mensura - I))
                {
                    redde FALSUM;
                }
                frange;
            casus CRUSTA_GENUS_PARS_CONTINUATIO:
                frange;
            casus CRUSTA_GENUS_PARS_SIMPLEX:
            {
                i32 ad;

                t = _token(pars, (i32)CRUSTA_PARS_TOK);
                si (t == NIHIL)
                {
                    redde FALSUM;
                }
                ad = (i32)t->valor.mensura;
                si (ad > I && t->valor.datum[ad - I] == '\'')
                {
                    ad--;
                }
                si (   ad > I && !_appendere(area, longitudo,
                        (constans character*)t->valor.datum + I, ad
                            - I))
                {
                    redde FALSUM;
                }
                frange;
            }
            casus CRUSTA_GENUS_PARS_GEMINA:
            casus CRUSTA_GENUS_PARS_VERSA:
                si (!_partes_aestimare(d, a, p, pars,
                        (i32)CRUSTA_GEMINA_PARTES, area, longitudo,
                        profunditas))
                {
                    redde FALSUM;
                }
                frange;
            casus CRUSTA_GENUS_PARS_PARAMETRUM:
                t = _token(pars, (i32)CRUSTA_PARAMETRUM_TOK_TITULUS);
                si (t == NIHIL)
                {
                    redde FALSUM;
                }
                titulus = chorda_ut_cstr(t->valor, d->piscina);
                si (   titulus == NIHIL
                    || !_variabilem_aestimare(d, a, p, pars,
                        titulus, area, longitudo, profunditas, FALSUM))
                {
                    redde FALSUM;
                }
                frange;
            casus CRUSTA_GENUS_PARS_EXPANSIO:
            {
                constans MateriaToken* sub = _token(pars,
                    (i32)CRUSTA_EXPANSIO_TOK_SUBSCRIPTUM);

                t = _token(pars, (i32)CRUSTA_EXPANSIO_TOK_TITULUS);
                si (   t == NIHIL
                    || !_locus_vacuus(pars,
                    (i32)CRUSTA_EXPANSIO_TOK_PRAEFIXUM)
                    || !_locus_vacuus(pars,
                    (i32)CRUSTA_EXPANSIO_TOK_OPERATOR)
                    || !_locus_vacuus(pars,
                    (i32)CRUSTA_EXPANSIO_ARGUMENTA))
                {
                    /* ${X:-y}, ${#X}, ${!X} ... */
                    redde _deficere(d, "operator");
                }
                titulus = chorda_ut_cstr(t->valor, d->piscina);
                si (titulus == NIHIL)
                {
                    redde FALSUM;
                }
                si (   sub != NIHIL
                    && !(strcmp(titulus, "BASH_SOURCE") == ZEPHYRUM
                        && _aequalis(sub->valor, "[0]")))
                {
                    redde _deficere(d, "tabulatum");   /* subscripta */
                }
                si (!_variabilem_aestimare(d, a, p, pars, titulus, area,
                        longitudo, profunditas, FALSUM))
                {
                    redde FALSUM;
                }
                frange;
            }
            casus CRUSTA_GENUS_PARS_SUBSTITUTIO:
                                si (!_substitutionem_aestimare(d, a, p,
                                    pars, area,
                                    longitudo, profunditas))
                                {
                    redde _deficere(d, "substitutio");
                                }
                frange;
                        casus CRUSTA_GENUS_PARS_DOMUS:
                redde _deficere(d, "ambitus");     /* ~ : HOME */
            ordinarius:
                /* effugia $'..', arithmetica, ... */
                redde _deficere(d, "operator");
        }
    }
    redde VERUM;
}


/* ==================================================
 * Plagulae: lectio, parsura, nodi collecti
 * ================================================== */

interior vacuum
_nodos_colligere (
       Piscina* piscina,
       Plagula* p)
{
    Xar* acervus  = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));
    i32  magn     = (i32)magnitudo(MateriaNodus*);

    p->imperia = xar_creare(piscina, magn);
    p->assignationes = xar_creare(piscina, magn);
    p->iterationes = xar_creare(piscina, magn);
    p->redirectiones = xar_creare(piscina, magn);
    p->iudicia = xar_creare(piscina, magn);
    p->verba = xar_creare(piscina, magn);
    p->variabiles = xar_creare(piscina, magn);
    p->functiones = xar_creare(piscina, magn);
    *(constans MateriaNodus**)xar_addere(acervus) = p->radix;
    dum (xar_numerus(acervus) > ZEPHYRUM)
    {
        constans MateriaNodus* n =
            *(constans MateriaNodus**)xar_obtinere(
            acervus, xar_numerus(acervus) - I);
                          Xar* liberi;
                          Xar* destinatum = NIHIL;
                          i32  k;

        xar_truncare(acervus, xar_numerus(acervus) - I);
        commutatio (n->genus)
        {
            casus CRUSTA_GENUS_IMPERIUM:
                destinatum = p->imperia;
                frange;
            casus CRUSTA_GENUS_ASSIGNATIO:
                destinatum = p->assignationes;
                frange;
            casus CRUSTA_GENUS_ITERATIO:
                destinatum = p->iterationes;
                frange;
            casus CRUSTA_GENUS_REDIRECTIO:
                destinatum = p->redirectiones;
                frange;
            casus CRUSTA_GENUS_IUDICIUM_PRAEPOSITA:
            casus CRUSTA_GENUS_IUDICIUM_BINARIA:
                destinatum = p->iudicia;
                frange;
            casus CRUSTA_GENUS_PARS_PARAMETRUM:
            casus CRUSTA_GENUS_PARS_EXPANSIO:
                destinatum = p->variabiles;
                frange;
            casus CRUSTA_GENUS_FUNCTIO:
                destinatum = p->functiones;
                frange;
            casus CRUSTA_GENUS_VERBUM:
                /* verba quae bash per globum expandit: argumenta
                 * imperii et lista 'for' (non valores assignationum,
                 * non scopi redirectionum, non exemplaria 'case') */
                si (   n->pater != NIHIL
                    && (   n->pater->genus == (s32)CRUSTA_GENUS_IMPERIUM
                        || n->pater->genus
                           == (s32)CRUSTA_GENUS_ITERATIO))
                {
                    destinatum = p->verba;
                }
                frange;
            ordinarius:
                frange;
        }
        si (destinatum != NIHIL)
        {
            *(constans MateriaNodus**)xar_addere(destinatum) = n;
        }
        liberi = materia_nodus_liberi(piscina, n);
        si (liberi == NIHIL)
        {
            perge;
        }
        per (k = xar_numerus(liberi); k > ZEPHYRUM; k--)
        {
            *(constans MateriaNodus**)xar_addere(acervus) =
                *(MateriaNodus**)xar_obtinere(liberi, k - I);
        }
    }
}

interior Plagula*
_plagulam_parare (
             Derivatio* d,
    constans character* via)
{
         Plagula* p = (Plagula*)piscina_allocare(d->piscina,
                          (memoriae_index)magnitudo(Plagula));
           chorda fons;
    CrustaParsura relatio;

    si (p == NIHIL)
    {
        redde NIHIL;
    }
    memset(p, ZEPHYRUM, magnitudo(Plagula));
    p->via  = _duplicare(d->piscina, via);
    fons    = filum_legere_totum(via, d->piscina);
    si (fons.datum == NIHIL)
    {
        p->causa = "illegibilis";
        redde p;
    }
    memset(&relatio, ZEPHYRUM, magnitudo(relatio));
    p->radix = crusta_arbor_parsare(d->piscina,
        (constans character*)fons.datum, (i32)fons.mensura,
        &CRUSTA_BASH,
        &relatio);
    si (p->radix == NIHIL || !relatio.sana)
    {
        p->radix = NIHIL;
        p->causa = "parsura non sana";
        redde p;
    }
    _nodos_colligere(d->piscina, p);
    redde p;
}


/* ==================================================
 * Definitiones et locales ambitus
 * ================================================== */

interior b32
_declaratio_est (
    constans character* t)
{
    redde t != NIHIL && (   strcmp(t, "local") == ZEPHYRUM
                         || strcmp(t, "declare") == ZEPHYRUM
                         || strcmp(t, "typeset") == ZEPHYRUM
                         || strcmp(t, "export") == ZEPHYRUM
                         || strcmp(t, "readonly") == ZEPHYRUM);
}

interior b32
_localis_verbum (
    constans character* t)
{
    redde t != NIHIL && (   strcmp(t, "local") == ZEPHYRUM
                         || strcmp(t, "declare") == ZEPHYRUM
                         || strcmp(t, "typeset") == ZEPHYRUM);
}

interior vacuum
_localem_addere (
                  Ambitus* a,
                Derivatio* d,
    constans MateriaNodus* functio,
       constans character* titulus)
{
    Localis* l;

    si (_localis_est(a, functio, titulus))
    {
        redde;
    }
    l           = (Localis*)xar_addere(a->locales);
    l->functio  = functio;
    l->titulus  = _duplicare(d->piscina, titulus);
}

/* 'local a b=1' in functione: a et b locales */
interior vacuum
_locales_colligere (
    Derivatio* d,
      Ambitus* a,
      Plagula* p)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < xar_numerus(p->imperia); k++)
    {
        constans MateriaNodus* im =
            *(constans MateriaNodus**)xar_obtinere(
                                        p->imperia, k);
        constans MateriaNodus* functio = _functio_circumdans(im);
                    character* t;
                          Xar* liberi;

        si (functio == NIHIL)
        {
            perge;
        }
        t = _titulus_staticus(d->piscina, crusta_imperium_titulus(im));
        si (!_localis_verbum(t))
        {
            perge;
        }
        liberi = _nodi_listae(d->piscina, im,
            (i32)CRUSTA_IMPERIUM_LIBERI);
        per (j = I; j < xar_numerus(liberi); j++)
        {
            constans MateriaNodus* e = *(MateriaNodus**)xar_obtinere(
                liberi, j);

            si (e->genus == (s32)CRUSTA_GENUS_ASSIGNATIO)
            {
                constans MateriaToken* tt = _token(e,
                    (i32)CRUSTA_ASSIGNATIO_TOK_TITULUS);

                si (tt != NIHIL)
                {
                    _localem_addere(a, d, functio,
                        chorda_ut_cstr(tt->valor, d->piscina));
                }
            }
            alioquin si (e->genus == (s32)CRUSTA_GENUS_VERBUM)
            {
                character* s = _titulus_staticus(d->piscina, e);

                si (s != NIHIL && s[ZEPHYRUM] != '-')
                {
                    _localem_addere(a, d, functio, s);
                }
            }
        }
    }
}

interior vacuum
_definitionem_addere (
                      Ambitus* a,
                    Derivatio* d,
                      Plagula* p,
           constans character* titulus,
        constans MateriaNodus* verbum,
                          b32  inanis,
        constans MateriaNodus* locus,
                          b32  declaratio_localis,
           constans character* causa)
{
    constans MateriaNodus* functio = _functio_circumdans(locus);
                Definitio* def;

    si (titulus == NIHIL)
    {
        redde;
    }
    si (   functio != NIHIL && !declaratio_localis
        && !_localis_est(a, functio, titulus))
    {
        functio = NIHIL;   /* assignatio in functione sine 'local' */
    }
    def               = (Definitio*)xar_addere(a->definitiones);
    def->titulus      = _duplicare(d->piscina, titulus);
    def->verbum       = verbum;
    def->inanis       = inanis;
        def->functio  = functio;
    def->plagula      = p;
    def->causa        = causa;
}

interior vacuum
_definitiones_colligere (
    Derivatio* d,
      Ambitus* a,
      Plagula* p)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(p->assignationes); k++)
    {
        constans MateriaNodus* as =
            *(constans MateriaNodus**)xar_obtinere(
                                        p->assignationes, k);
        constans MateriaNodus* im = as->pater;
        constans MateriaToken* tt;
        constans MateriaToken* op;
        constans MateriaNodus* valor;
                    character* t = NIHIL;
                          b32  ignotum;

        tt     = _token(as, (i32)CRUSTA_ASSIGNATIO_TOK_TITULUS);
        op     = _token(as, (i32)CRUSTA_ASSIGNATIO_TOK_OPERATOR);
        valor  = _nodus(as, (i32)CRUSTA_ASSIGNATIO_VALOR);

        si (tt == NIHIL)
        {
            perge;
        }
        si (im != NIHIL && im->genus == (s32)CRUSTA_GENUS_IMPERIUM)
        {
            t = _titulus_staticus(d->piscina,
                crusta_imperium_titulus(im));
            si (   crusta_imperium_titulus(im) != NIHIL
                && !_declaratio_est(t))
            {
                perge;   /* 'X=1 cmd': ambitus imperii solius */
            }
        }
                ignotum = !_locus_vacuus(as,
                    (i32)CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM)
                    || (op != NIHIL && !_aequalis(op->valor, "="))
                    || (valor != NIHIL
                    && valor->genus != (s32)CRUSTA_GENUS_VERBUM);
        /* causa ignoti (T1): tabulatum (X=(..), X[i]=, X+=(..)), aliter
         * operator (X+=verbum) */
        _definitionem_addere(a, d, p, chorda_ut_cstr(tt->valor,
            d->piscina),
            ignotum ? NIHIL : valor, !ignotum && valor == NIHIL, as,
            _localis_verbum(t),
            (   !_locus_vacuus(as,
            (i32)CRUSTA_ASSIGNATIO_TOK_SUBSCRIPTUM)
             || (valor != NIHIL
                 && valor->genus == (s32)CRUSTA_GENUS_TABULATUM))
                ? "tabulatum" : "operator");
    }
    per (k = ZEPHYRUM; k < xar_numerus(p->iterationes); k++)
    {
        constans MateriaNodus* it =
            *(constans MateriaNodus**)xar_obtinere(
                                        p->iterationes, k);
        constans MateriaToken* tt;

        tt = _token(it, (i32)CRUSTA_ITERATIO_TOK_TITULUS);

        si (tt != NIHIL)
        {
                        _definitionem_addere(a, d, p,
                            chorda_ut_cstr(tt->valor, d->piscina),
                            NIHIL, FALSUM,
                            it,
                            FALSUM, "ansa_read");
        }
    }
}


/* ==================================================
 * Plagulae ambitus, scripta, variabilis sola
 * ================================================== */

interior b32
_plagulam_in_ambitu (
                Ambitus* a,
     constans character* via)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(a->plagulae); k++)
    {
        si (strcmp((*(Plagula**)xar_obtinere(a->plagulae, k))->via, via)
                == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* scriptum an binarium: '.sh' aut '#!' */
interior b32
_scriptum_est (
              Derivatio* d,
     constans character* via)
{
    memoriae_index n = strlen(via);
            chorda c;

    si (n > III && strcmp(via + n - III, ".sh") == ZEPHYRUM)
    {
        redde VERUM;
    }
    c = filum_legere_totum(via, d->piscina);
    redde c.datum != NIHIL && c.mensura >= II
        && c.datum[ZEPHYRUM] == '#'
        && c.datum[I] == '!';
}

/* verbum = '$X' / '${X}' / '"$X"' / '"${X}"' solum? reddit partem
 * variabilis (NIHIL aliter) - subscripta et operatores exclusi */
interior constans MateriaNodus*
_variabilis_sola (
    constans MateriaNodus* verbum)
{
    constans MateriaNodus* n      = verbum;
                      i32  locus  = (i32)CRUSTA_VERBUM_PARTES;

    dum (n != NIHIL)
    {
        constans MateriaNodus* pars;
        constans MateriaValor* l;
                          i32  k;
                          i32  numerus = ZEPHYRUM;

        si (   locus                >= n->numerus_locorum
            || n->loci[locus].genus != MATERIA_VALOR_LISTA)
        {
            redde NIHIL;
        }
        l     = &n->loci[locus];
        pars  = NIHIL;
        per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*l); k++)
        {
            MateriaValor* e = materia_valor_lista_obtinere(*l, k);

            si (e->genus == MATERIA_VALOR_NODUS)
            {
                pars = e->datum.nodus;
                numerus++;
            }
        }
        si (numerus != I)
        {
            redde NIHIL;
        }
        si (pars->genus == (s32)CRUSTA_GENUS_PARS_PARAMETRUM)
        {
            redde pars;
        }
        si (pars->genus == (s32)CRUSTA_GENUS_PARS_EXPANSIO)
        {
            redde (   _locus_vacuus(pars,
                (i32)CRUSTA_EXPANSIO_TOK_PRAEFIXUM)
                   && _locus_vacuus(pars,
                   (i32)CRUSTA_EXPANSIO_TOK_SUBSCRIPTUM)
                   && _locus_vacuus(pars,
                   (i32)CRUSTA_EXPANSIO_TOK_OPERATOR)
                   && _locus_vacuus(pars,
                   (i32)CRUSTA_EXPANSIO_ARGUMENTA))
                ? pars : NIHIL;
        }
        si (pars->genus != (s32)CRUSTA_GENUS_PARS_GEMINA)
        {
            redde NIHIL;
        }
        n      = pars;
        locus  = (i32)CRUSTA_GEMINA_PARTES;
    }
    redde NIHIL;
}


/* ==================================================
 * Indices verborum
 * ================================================== */

/* aedificia et verba bash 5.2 (census T2: crusta/instrumenta/
 * mandata_census.c); effectus eorum per syntaxim tractantur */
hic_manens constans character* AEDIFICIA[] = {
    ":", ".", "[", "alias", "bg", "bind", "break", "builtin", "caller",
    "cd", "command", "compgen", "complete", "compopt", "continue",
    "declare", "dirs", "disown", "echo", "enable", "eval", "exec",
    "exit", "export", "false", "fc", "fg", "getopts", "hash", "help",
    "history", "jobs", "kill", "let", "local", "logout", "mapfile",
    "popd", "printf", "pushd", "pwd", "read", "readarray", "readonly",
    "return", "set", "shift", "shopt", "source", "suspend", "test",
    "time", "times", "trap", "true", "type", "typeset", "ulimit",
    "umask", "unalias", "unset", "wait", NIHIL
};

/* operatores probationis unarii plagularum ('-a' consulto abest:
 * in '[ ]' coniunctio est) */
hic_manens constans character* UNARIA[] = {
    "-b", "-c", "-d", "-e", "-f", "-g", "-h", "-k", "-p", "-r", "-s",
    "-u", "-w", "-x", "-G", "-L", "-N", "-O", "-S", NIHIL
};

hic_manens constans character* BINARIA[] = {
    "-nt", "-ot", "-ef", NIHIL
};

/* variabiles quas bash ipse ponit (non ex ambitu) */
hic_manens constans character* INTERNAE[] = {
    "BASH", "BASHOPTS", "BASHPID", "BASH_ARGC", "BASH_ARGV",
    "BASH_COMMAND", "BASH_LINENO", "BASH_REMATCH", "BASH_SOURCE",
    "BASH_SUBSHELL", "BASH_VERSINFO", "BASH_VERSION", "COMP_CWORD",
    "COMP_LINE", "COMP_WORDS", "DIRSTACK", "EPOCHREALTIME",
    "EPOCHSECONDS", "EUID", "FUNCNAME", "GROUPS", "HOSTNAME",
    "HOSTTYPE", "IFS", "LINENO", "MACHTYPE", "OLDPWD", "OPTARG",
    "OPTIND", "OSTYPE", "PIPESTATUS", "PPID", "PWD", "RANDOM", "REPLY",
    "SECONDS", "SHELLOPTS", "SHLVL", "SRANDOM", "UID", "MAPFILE", NIHIL
};

/* radices systematis: identitas_clang eas figit (spec 3 par. III.5) */
hic_manens constans character* SYSTEMA[] = {
    "/usr/", "/System/", "/Library/", "/bin/", "/sbin/", "/dev/",
    "/private/var/db/", "/opt/homebrew/Cellar/", "/opt/homebrew/lib/",
    "/opt/homebrew/opt/", "/Applications/Xcode.app/", NIHIL
};

interior b32
_in_serie (
    constans character** series,
    constans character*  verbum)
{
    i32 i;

    per (i = ZEPHYRUM; verbum != NIHIL && series[i] != NIHIL; i++)
    {
        si (strcmp(series[i], verbum) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_in_nominibus (
                   Xar* nomina,
    constans character* verbum)
{
    i32 i;

    per (i = ZEPHYRUM; verbum != NIHIL && i < xar_numerus(nomina); i++)
    {
        si (strcmp(*(character**)xar_obtinere(nomina, i), verbum)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_nomen_addere (
               Piscina* piscina,
                   Xar* nomina,
    constans character* verbum)
{
    si (verbum != NIHIL && !_in_nominibus(nomina, verbum))
    {
        *(character**)xar_addere(nomina) = _duplicare(piscina, verbum);
    }
}

/* verbum in indice spatiis separato ('-e -f')? */
interior b32
_in_indice (
    constans character* index,
    constans character* verbum)
{
        memoriae_index  n;
    constans character* c = index;

    si (index == NIHIL || verbum == NIHIL)
    {
        redde FALSUM;
    }
    n = strlen(verbum);
    dum (*c != '\0')
    {
        constans character* f;

        dum (*c == ' ' || *c == '\n' || *c == '\t')
        {
            c++;
        }
        f = c;
        dum (*f != '\0' && *f != ' ' && *f != '\n' && *f != '\t')
        {
            f++;
        }
        si (   (memoriae_index)(f - c) == n && n > ZEPHYRUM
            && strncmp(c, verbum, n)   == ZEPHYRUM)
        {
            redde VERUM;
        }
        c = f;
    }
    redde FALSUM;
}

interior character*
_iungere (
               Piscina* piscina,
    constans character* a,
    constans character* b)
{
    memoriae_index  na = strlen(a);
    memoriae_index  nb = strlen(b);
         character* r  = (character*)piscina_allocare(piscina,
                            na + nb + I);

    si (r != NIHIL)
    {
        memcpy(r, a, na);
        memcpy(r + na, b, nb + I);
    }
    redde r;
}


/* ==================================================
 * Globi
 * ================================================== */

/* pars litteralis (non citata) cum '*' '?' '[' : bash eam expandit */
interior b32
_globus_habet (
    constans MateriaNodus* verbum)
{
    constans MateriaValor* partes;
                      i32  k;

    si (   verbum                  == NIHIL
        || verbum->numerus_locorum <= (i32)CRUSTA_VERBUM_PARTES)
    {
        redde FALSUM;
    }
    partes = &verbum->loci[CRUSTA_VERBUM_PARTES];
    si (partes->genus != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*partes); k++)
    {
                 MateriaValor* e = materia_valor_lista_obtinere(*partes,
                     k);
        constans MateriaToken* t;
                          i32  j;

        si (   e->genus != MATERIA_VALOR_NODUS
            || e->datum.nodus->genus
                != (s32)CRUSTA_GENUS_PARS_LITTERALIS)
        {
            perge;
        }
        t = _token(e->datum.nodus, (i32)CRUSTA_PARS_TOK);
        per (j = ZEPHYRUM; t != NIHIL && j < t->valor.mensura; j++)
        {
            character c = (character)t->valor.datum[j];

            si (c == '*' || c == '?' || c == '[')
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

/* congruentia globi: '*' et '?' numquam '/' transeunt (bash) */
interior b32
_globus_congruit (
    constans character* exemplar,
    constans character* textus)
{
    dum (*exemplar != '\0')
    {
        si (*exemplar == '*')
        {
            exemplar++;
            per (;;)
            {
                si (_globus_congruit(exemplar, textus))
                {
                    redde VERUM;
                }
                si (*textus == '\0' || *textus == '/')
                {
                    redde FALSUM;
                }
                textus++;
            }
        }
        si (*textus == '\0')
        {
            redde FALSUM;
        }
        si (*exemplar == '?')
        {
            si (*textus == '/')
            {
                redde FALSUM;
            }
        }
        alioquin si (*exemplar == '[')
        {
             constans character* f         = strchr(exemplar, ']');
                            b32  inventum  = FALSUM;
             constans character* c;

            si (f == NIHIL)
            {
                si (*textus != '[')
                {
                    redde FALSUM;
                }
            }
            alioquin
            {
                per (c = exemplar + I; c < f; c++)
                {
                    si (*c == *textus)
                    {
                        inventum = VERUM;
                    }
                }
                si (!inventum)
                {
                    redde FALSUM;
                }
                exemplar = f;
            }
        }
        alioquin si (*exemplar != *textus)
        {
            redde FALSUM;
        }
        exemplar++;
        textus++;
    }
    redde *textus == '\0';
}


/* ==================================================
 * Situs: creatio, cwd, classis viae
 * ================================================== */

interior Situs*
_situm_creare (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
       constans character* elementum,
    constans MateriaNodus* nodus)
{
    Situs* s = (Situs*)xar_addere(a->situs);

    (vacuum)d;
    si (s == NIHIL)
    {
        redde NIHIL;
    }
    memset(s, ZEPHYRUM, magnitudo(Situs));
    s->elementum  = elementum;
    s->plagula    = p;
    s->scripta    = -I;
    si (   nodus == NIHIL || !materia_tractus_nodi(NIHIL, nodus,
            &s->tractus))
    {
        s->tractus.initium = -I;
    }
    redde s;
}

/* cd W -> directorium absolutum, W relativa ad 'basis' (cwd ante cd;
 * NIHIL = ignotum: W absoluta tamen valet). NIHIL = W ignotum. */
interior character*
_cd_aestimare (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* imperium,
       constans character* basis,
                      b32* est_cd)
{
    character* t;
          Xar* argumenta;
    character  valor[VIA_MAXIMA];
          i32  longitudo = ZEPHYRUM;
    character  absoluta[VIA_MAXIMA];

    *est_cd = FALSUM;
    t = _titulus_staticus(d->piscina,
        crusta_imperium_titulus(imperium));
    argumenta = crusta_imperium_argumenta(d->piscina, imperium);
    si (   t         == NIHIL || strcmp(t, "cd") != ZEPHYRUM
        || argumenta == NIHIL || xar_numerus(argumenta) != I)
    {
        redde NIHIL;
    }
    *est_cd          = VERUM;
    valor[ZEPHYRUM]  = '\0';
    si (   !_verbum_aestimare(d, a, p,
               *(MateriaNodus**)xar_obtinere(argumenta, ZEPHYRUM),
               valor, &longitudo, ZEPHYRUM)
        || (valor[ZEPHYRUM] != '/' && basis == NIHIL)
        || !_absolutam_facere(valor, basis != NIHIL ? basis : d->radix,
               absoluta))
    {
        redde NIHIL;
    }
    redde _duplicare(d->piscina, absoluta);
}

/* cwd situs (spec par. IV.3), ordine fontis non fluxus: primum 'cd W'
 * summi gradus plagulae quae ANTE situm FINIUNT, quisque relativus ad
 * cwd ante se ('cd lib' deinde 'cd ..' = radix; 'cd lib' argumentum
 * suum non mutat - oraculum T5 'lib/lib' invenit); deinde 'cd W' in
 * catena eadem ante situm, super cwd illud. NIHIL = cwd ignotum. */
interior constans character*
_directorium_loci (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* nodus)
{
     constans MateriaNodus* m    = nodus;
        constans character* cwd  = d->radix;
            MateriaTractus  situs_tractus;
                       b32  est_cd;
                       i32  k;

    /* summus gradus, ordine fontis */
    si (materia_tractus_nodi(NIHIL, nodus, &situs_tractus))
    {
        per (k = ZEPHYRUM; k < xar_numerus(p->imperia); k++)
        {
            constans MateriaNodus* c = *(constans MateriaNodus**)
                xar_obtinere(p->imperia, k);
                    MateriaTractus  t;
                         character* dir;

            si (   c->pater        == NIHIL
                || c->pater->genus != (s32)CRUSTA_GENUS_PROGRAMMA
                || !materia_tractus_nodi(NIHIL, c, &t)
                || t.finis > situs_tractus.initium)
            {
                perge;
            }
            dir = _cd_aestimare(d, a, p, c, cwd, &est_cd);
            si (est_cd)
            {
                cwd = dir;   /* NIHIL = ignotum donec cd absolutum */
            }
        }
    }
    /* membrum catenae: ascende donec pater catena sit */
    dum (   m               != NIHIL && m->pater != NIHIL
         && m->pater->genus != (s32)CRUSTA_GENUS_CATENA)
    {
        si (   m->pater->genus == (s32)CRUSTA_GENUS_FUNCTIO
            || m->pater->genus == (s32)CRUSTA_GENUS_PROGRAMMA)
        {
            m = NIHIL;
            frange;
        }
        m = m->pater;
    }
    si (m != NIHIL && m->pater != NIHIL)
    {
        Xar* membra = _nodi_listae(d->piscina, m->pater,
            (i32)CRUSTA_CATENA_LIBERI);

        per (k = ZEPHYRUM; k < xar_numerus(membra); k++)
        {
            constans MateriaNodus* c = *(MateriaNodus**)xar_obtinere(
                membra, k);
                        character* dir;

            si (c == m)
            {
                frange;
            }
            si (c->genus != (s32)CRUSTA_GENUS_IMPERIUM)
            {
                perge;
            }
            dir = _cd_aestimare(d, a, p, c, cwd, &est_cd);
            si (est_cd)
            {
                cwd = dir;
            }
        }
    }
    redde cwd;
}

interior constans character*
_classis_arboris (
    constans character* relativa)
{
    memoriae_index n = strlen(relativa);

    si (   _sub_build(relativa) || strcmp(relativa, "build") == ZEPHYRUM
        || (n > VI && strcmp(relativa + n - VI, "/build") == ZEPHYRUM))
    {
        redde "build";
    }
    si (strncmp(relativa, "bin/", IV) == ZEPHYRUM)
    {
        redde "instrumentum_domus";
    }
    redde "arbor";
}

interior constans character*
_classis_externa (
    constans character* absoluta)
{
    i32 i;

    per (i = ZEPHYRUM; SYSTEMA[i] != NIHIL; i++)
    {
        si (strncmp(absoluta, SYSTEMA[i], strlen(SYSTEMA[i]))
            == ZEPHYRUM)
        {
            redde "systema";
        }
    }
    redde "externa";
}

/* verbum -> via, forma, resolutio, classis situs. absoluta_out
 * (NIHIL licet) = via absoluta ubi plena. VERUM = plena. */
interior b32
_viam_classificare (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* verbum,
    constans MateriaNodus* locus,
                    Situs* s,
                character* absoluta_out)
{
                 character  valor[VIA_MAXIMA];
                       i32  longitudo = ZEPHYRUM;
                 character  absoluta[VIA_MAXIMA];
                       b32  plenus;
        constans character* cwd;
        constans character* rel;
        constans character* causa_verbi;

    valor[ZEPHYRUM]  = '\0';
    d->causa         = NIHIL;
    {
        constans MateriaNodus* sola = _variabilis_sola(verbum);
        constans MateriaToken* t = sola == NIHIL ? NIHIL : _token(sola,
            sola->genus == (s32)CRUSTA_GENUS_PARS_PARAMETRUM
                ? (i32)CRUSTA_PARAMETRUM_TOK_TITULUS
                : (i32)CRUSTA_EXPANSIO_TOK_TITULUS);
        character* titulus = t == NIHIL ? NIHIL
            : chorda_ut_cstr(t->valor, d->piscina);

        plenus = titulus != NIHIL
            ? _variabilem_aestimare(d, a, p, sola, titulus, valor,
                  &longitudo, ZEPHYRUM, VERUM)
            : _verbum_aestimare(d, a, p, verbum, valor, &longitudo,
                  ZEPHYRUM);
        }
    /* causa verbi ANTE cwd (cd W aestimatio causam suam poneret) */
    causa_verbi  = d->causa != NIHIL ? d->causa : "valor ignotus";
    cwd          = _directorium_loci(d, a, p, locus);
    s->forma     = "via";
    si (!plenus)
    {
        si (   longitudo > ZEPHYRUM && strchr(valor, '/') != NIHIL
            && cwd != NIHIL && _absolutam_facere(valor, cwd, absoluta))
        {
            si (valor[longitudo - I] != '/')
            {
                _directorium_viae(absoluta);
            }
            rel                   = _relativa(d, absoluta);
                        s->forma  = "praefixum";
            s->resolutio          = "partialis";
            s->causa              = causa_verbi;
            {
                character textus[VIA_MAXIMA];
                      i32 n = ZEPHYRUM;

                textus[ZEPHYRUM] = '\0';
                _textum_colligere(verbum, textus, &n);
                s->textus = _duplicare(d->piscina, textus);
            }
            si (rel != NIHIL)
            {
                s->via      = _iungere(d->piscina, rel, "/");
                s->classis  = _classis_arboris(s->via);
            }
            alioquin si (strcmp(absoluta, d->radix) == ZEPHYRUM)
            {
                s->via      = _duplicare(d->piscina, "./");
                s->classis  = "arbor";
            }
            alioquin
            {
                s->via = strcmp(absoluta, "/") == ZEPHYRUM
                    ? _duplicare(d->piscina, "/")
                    : _iungere(d->piscina, absoluta, "/");
                s->classis = _classis_externa(absoluta);
            }
            redde FALSUM;
        }
        longitudo        = ZEPHYRUM;
        valor[ZEPHYRUM]  = '\0';
        _textum_colligere(verbum, valor, &longitudo);
        s->via                = _duplicare(d->piscina, valor);
                s->resolutio  = "nulla";
        s->causa              = causa_verbi;
        redde FALSUM;
    }
    si (cwd == NIHIL || !_absolutam_facere(valor, cwd, absoluta))
    {
        s->via        = _duplicare(d->piscina, valor);
        s->resolutio  = "nulla";
        s->causa      = "cwd ignotum";
        redde FALSUM;
    }
    si (_globus_habet(verbum))
    {
        s->forma = "globus";
    }
    s->resolutio  = "plena";
    rel           = _relativa(d, absoluta);
    si (rel == NIHIL && strcmp(absoluta, d->radix) == ZEPHYRUM)
    {
        s->via      = _duplicare(d->piscina, ".");
        s->classis  = "arbor";
    }
    alioquin si (rel == NIHIL)
    {
        s->via      = _duplicare(d->piscina, absoluta);
        s->classis  = _classis_externa(absoluta);
    }
    alioquin
    {
        s->via      = _duplicare(d->piscina, rel);
        s->classis  = _classis_arboris(rel);
    }
    si (absoluta_out != NIHIL)
    {
        strcpy(absoluta_out, absoluta);
    }
    redde VERUM;
}


/* ==================================================
 * Situs per syntaxim: redirectiones, probationes, globi, ambitus
 * ================================================== */

/* verbum = substitutio processus ('<(cat x)')? redirectio ex ea
 * nihil ipsa legit - imperia intus situs sui sunt */
interior b32
_processum_habet (
    constans MateriaNodus* verbum)
{
    i32 k;

    si (   verbum                  == NIHIL
        || verbum->numerus_locorum <= (i32)CRUSTA_VERBUM_PARTES
        || verbum->loci[CRUSTA_VERBUM_PARTES].genus
            != MATERIA_VALOR_LISTA)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < materia_valor_lista_numerus(
             verbum->loci[CRUSTA_VERBUM_PARTES]); k++)
    {
        MateriaValor* e = materia_valor_lista_obtinere(
            verbum->loci[CRUSTA_VERBUM_PARTES], k);

        si (   e->genus == MATERIA_VALOR_NODUS
            && e->datum.nodus->genus
                == (s32)CRUSTA_GENUS_PARS_PROCESSUS)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior vacuum
_redirectiones_tractare (
    Derivatio* d,
      Ambitus* a,
      Plagula* p)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(p->redirectiones); k++)
    {
        constans MateriaNodus* r = *(constans MateriaNodus**)
            xar_obtinere(p->redirectiones, k);
        constans MateriaToken* op = _token(r,
            (i32)CRUSTA_REDIRECTIO_TOK_OPERATOR);
        constans MateriaNodus* scopus = _nodus(r,
            (i32)CRUSTA_REDIRECTIO_SCOPUS);
                     character* o;
                     character* s;
                           b32  legit;
                           b32  scribit;

        si (op == NIHIL || scopus == NIHIL || _processum_habet(scopus))
        {
            perge;
        }
        o = chorda_ut_cstr(op->valor, d->piscina);
        si (o == NIHIL || strncmp(o, "<<", II) == ZEPHYRUM)
        {
            perge;   /* heredoc, herestring: nulla plagula */
        }
        s = _titulus_staticus(d->piscina, scopus);
        si (strcmp(o, ">&") == ZEPHYRUM || strcmp(o, "<&") == ZEPHYRUM)
        {
            /* duplicatio descriptoris ('2>&1', '>&-'): nulla plagula;
             * '>& verbum' = '&>' */
            si (   strcmp(o, "<&") == ZEPHYRUM || s == NIHIL
                || s[ZEPHYRUM]     == '-'
                || (s[ZEPHYRUM] >= '0' && s[ZEPHYRUM] <= '9'))
            {
                perge;
            }
        }
        si (s != NIHIL && strncmp(s, "/dev/", V) == ZEPHYRUM)
        {
            perge;
        }
        legit = strcmp(o, "<") == ZEPHYRUM
            || strcmp(o, "<>") == ZEPHYRUM;
        scribit = strcmp(o, "<") != ZEPHYRUM;
        si (legit)
        {
            Situs* x = _situm_creare(d, a, p, "lectio", r);

            si (x != NIHIL)
            {
                x->medium    = "redirectio";
                x->operator  = o;
                (vacuum)_viam_classificare(d, a, p, scopus, r, x,
                    NIHIL);
            }
        }
        si (scribit)
        {
            Situs* x = _situm_creare(d, a, p, "scriptura", r);

            si (x != NIHIL)
            {
                x->medium    = "redirectio";
                x->operator  = o;
                (vacuum)_viam_classificare(d, a, p, scopus, r, x,
                    NIHIL);
            }
        }
    }
}

interior vacuum
_probationem_addere (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* verbum,
       constans character* operator)
{
    Situs* x;

    si (verbum == NIHIL || verbum->genus != (s32)CRUSTA_GENUS_VERBUM)
    {
        redde;
    }
    x = _situm_creare(d, a, p, "probatio", verbum);
    si (x != NIHIL)
    {
        x->medium    = "aedificium";
        x->operator  = _duplicare(d->piscina, operator);
        (vacuum)_viam_classificare(d, a, p, verbum, verbum, x, NIHIL);
    }
}

/* '[[ ]]': iudicium-praeposita (-f x) et iudicium-binaria (a -nt b) */
interior vacuum
_iudicia_tractare (
    Derivatio* d,
      Ambitus* a,
      Plagula* p)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(p->iudicia); k++)
    {
        constans MateriaNodus* n = *(constans MateriaNodus**)
            xar_obtinere(p->iudicia, k);
        constans MateriaToken* op;
                    character* o;

        si (n->genus == (s32)CRUSTA_GENUS_IUDICIUM_PRAEPOSITA)
        {
            op  = _token(n, (i32)CRUSTA_PRAEPOSITA_TOK_OPERATOR);
            o   = op ? chorda_ut_cstr(op->valor, d->piscina) : NIHIL;
            si (_in_serie(UNARIA, o))
            {
                _probationem_addere(d, a, p, _nodus(n,
                    (i32)CRUSTA_PRAEPOSITA_OPERANDUM), o);
            }
        }
        alioquin
        {
            op  = _token(n, (i32)CRUSTA_BINARIA_TOK_OPERATOR);
            o   = op ? chorda_ut_cstr(op->valor, d->piscina) : NIHIL;
            si (_in_serie(BINARIA, o))
            {
                _probationem_addere(d, a, p, _nodus(n,
                    (i32)CRUSTA_BINARIA_SINISTER), o);
                _probationem_addere(d, a, p, _nodus(n,
                    (i32)CRUSTA_BINARIA_DEXTER), o);
            }
        }
    }
}

/* '[' et 'test': operatores in argumentis */
interior vacuum
_probationes_imperii (
          Derivatio* d,
            Ambitus* a,
            Plagula* p,
                Xar* verba,
                i32  ab)
{
    i32 n = xar_numerus(verba);
    i32 k;

    per (k = ab + I; k < n; k++)
    {
        character* o = _titulus_staticus(d->piscina,
            *(MateriaNodus**)xar_obtinere(verba, k));

        si (_in_serie(UNARIA, o) && k + I < n)
        {
            _probationem_addere(d, a, p,
                *(MateriaNodus**)xar_obtinere(verba, k + I), o);
            k++;
        }
        alioquin si (_in_serie(BINARIA, o) && k > ab + I && k + I < n)
        {
            _probationem_addere(d, a, p,
                *(MateriaNodus**)xar_obtinere(verba, k - I), o);
            _probationem_addere(d, a, p,
                *(MateriaNodus**)xar_obtinere(verba, k + I), o);
            k++;
        }
    }
}

/* globi in argumentis et listis 'for': enumeratio directorii (fons
 * UNUS enumerationum - lectiones per tabulam globum servant tantum) */
interior vacuum
_globos_tractare (
    Derivatio* d,
      Ambitus* a,
      Plagula* p)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(p->verba); k++)
    {
        constans MateriaNodus* v = *(constans MateriaNodus**)
            xar_obtinere(p->verba, k);
                        Situs* x;

        si (   v->pater->genus == (s32)CRUSTA_GENUS_IMPERIUM
            && crusta_imperium_titulus(v->pater) == v)
        {
            perge;   /* titulus ('[' non globus est) */
        }
        si (!_globus_habet(v))
        {
            perge;
        }
        x = _situm_creare(d, a, p, "enumeratio", v);
        si (x == NIHIL)
        {
            perge;
        }
        x->medium = "aedificium";
        si (   _viam_classificare(d, a, p, v, v, x, NIHIL)
            && strcmp(x->forma, "globus") == ZEPHYRUM)
        {
              constans character* g = strpbrk(x->via, "*?[");
                       character* f = NIHIL;
                       character* c;

            per (c = x->via; g != NIHIL && c < g; c++)
            {
                si (*c == '/')
                {
                    f = c;
                }
            }
            si (f == NIHIL)
            {
                x->exemplar  = x->via;
                x->via       = _duplicare(d->piscina, "./");
            }
            alioquin
            {
                x->exemplar  = _duplicare(d->piscina, f + I);
                f[I]         = '\0';
            }
        }
    }
}

/* nomina quae ambitus ponit praeter assignationes: read, mapfile,
 * getopts, printf -v, declare/local/export sine valore, unset */
interior vacuum
_assignata_colligere (
    Derivatio* d,
      Ambitus* a,
      Plagula* p)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < xar_numerus(p->imperia); k++)
    {
        constans MateriaNodus* im = *(constans MateriaNodus**)
            xar_obtinere(p->imperia, k);
                    character* t = _titulus_staticus(d->piscina,
                        crusta_imperium_titulus(im));
                           Xar* argumenta;
                     character* ultimum       = NIHIL;
                           i32  positionalia  = ZEPHYRUM;

        si (t == NIHIL)
        {
            perge;
        }
        argumenta = crusta_imperium_argumenta(d->piscina, im);
        per (j = ZEPHYRUM; argumenta && j < xar_numerus(argumenta); j++)
        {
            character* s = _titulus_staticus(d->piscina,
                *(MateriaNodus**)xar_obtinere(argumenta, j));
            character* aeq;

            si (s == NIHIL)
            {
                perge;
            }
            si (s[ZEPHYRUM] == '-')
            {
                b32 valor_nomen = (strcmp(t, "read") == ZEPHYRUM
                                   && strcmp(s, "-a") == ZEPHYRUM)
                    || (strcmp(t, "printf") == ZEPHYRUM
                        && strcmp(s, "-v") == ZEPHYRUM);
                b32 valor_alius = (strcmp(t, "read") == ZEPHYRUM
                    && _in_indice("-d -i -n -N -p -t -u", s))
                    || ((strcmp(t, "mapfile") == ZEPHYRUM
                         || strcmp(t, "readarray") == ZEPHYRUM)
                        && _in_indice("-d -n -O -s -u -C -c", s));

                si (   (valor_nomen || valor_alius)
                    && j + I < xar_numerus(argumenta))
                {
                    j++;
                    si (valor_nomen)
                    {
                        _nomen_addere(d->piscina, a->assignata,
                            _titulus_staticus(d->piscina,
                                *(MateriaNodus**)xar_obtinere(
                                    argumenta, j)));
                    }
                }
                perge;
            }
            positionalia++;
            si (   strcmp(t, "read") == ZEPHYRUM
                || _in_indice("declare typeset local export readonly "
                              "unset", t))
            {
                aeq = strchr(s, '=');
                si (aeq != NIHIL)
                {
                    *aeq = '\0';
                }
                _nomen_addere(d->piscina, a->assignata, s);
            }
            alioquin si (   strcmp(t, "getopts") == ZEPHYRUM
                         && positionalia         == II)
            {
                _nomen_addere(d->piscina, a->assignata, s);
            }
            ultimum = s;
        }
        si (   strcmp(t, "mapfile")   == ZEPHYRUM
            || strcmp(t, "readarray") == ZEPHYRUM)
        {
            _nomen_addere(d->piscina, a->assignata,
                ultimum != NIHIL ? ultimum : "MAPFILE");
        }
    }
}

interior b32
_specialis (
    constans character* titulus)
{
    character c = titulus[ZEPHYRUM];

    redde c == '\0' || (c >= '0' && c <= '9') || c == '?' || c == '#'
        || c == '@' || c == '*' || c == '$' || c == '!' || c == '-'
        || (c == '_' && titulus[I] == '\0')
        || _in_serie(INTERNAE, titulus);
}

interior vacuum
_variabiles_tractare (
    Derivatio* d,
      Ambitus* a,
      Plagula* p)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(p->variabiles); k++)
    {
        constans MateriaNodus* n = *(constans MateriaNodus**)
            xar_obtinere(p->variabiles, k);
        constans MateriaToken* t = _token(n,
            n->genus == (s32)CRUSTA_GENUS_PARS_PARAMETRUM
                ? (i32)CRUSTA_PARAMETRUM_TOK_TITULUS
                : (i32)CRUSTA_EXPANSIO_TOK_TITULUS);
                    character* titulus;
                        Situs* x;

        si (t == NIHIL)
        {
            perge;
        }
        titulus = chorda_ut_cstr(t->valor, d->piscina);
        si (   titulus == NIHIL || _specialis(titulus)
            || _in_nominibus(a->assignata, titulus))
        {
            perge;
        }
        x = _situm_creare(d, a, p, "ambitus_lectio", n);
        si (x != NIHIL)
        {
            x->titulus = titulus;
        }
    }
}


/* ==================================================
 * Tabula mandatorum (crusta/effectus_mandata.stml)
 * ================================================== */

interior StmlNodus*
_mandatum_invenire (
             Derivatio* d,
    constans character* titulus)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(d->tabula); k++)
    {
        Mandatum* m = (Mandatum*)xar_obtinere(d->tabula, k);

        si (strcmp(m->titulus, titulus) == ZEPHYRUM)
        {
            redde m->nodus;
        }
    }
    redde NIHIL;
}

/* attributum ordinis ut chorda C (NIHIL = absens) */
interior character*
_ordinis (
             Derivatio* d,
             StmlNodus* m,
    constans character* titulus)
{
    chorda* v = stml_attributum_capere(m, titulus);

    redde v == NIHIL ? NIHIL : chorda_ut_cstr(*v, d->piscina);
}

/* munus in verbum: lectio scriptura exsecutio enumeratio probatio;
 * exemplar et nullum nihil faciunt */
interior vacuum
_munus_applicare (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* verbum,
       constans character* munus,
                character* titulus,
                      b32  recursio,
                      b32  in_loco)
{
    Situs* x;

    si (   munus == NIHIL || strcmp(munus, "nullum") == ZEPHYRUM
        || strcmp(munus, "exemplar") == ZEPHYRUM)
    {
        redde;
    }
    x = _situm_creare(d, a, p,
        strcmp(munus, "lectio") == ZEPHYRUM ? "lectio"
        : strcmp(munus, "scriptura") == ZEPHYRUM ? "scriptura"
        : strcmp(munus, "exsecutio") == ZEPHYRUM ? "exsecutio"
        : strcmp(munus, "enumeratio") == ZEPHYRUM ? "enumeratio"
        : "probatio", verbum);
    si (x == NIHIL)
    {
        redde;
    }
    x->medium    = "mandatum";
    x->mandatum  = titulus;
    si (   _viam_classificare(d, a, p, verbum, verbum, x, NIHIL)
        && recursio && strcmp(x->forma, "via") == ZEPHYRUM)
    {
        memoriae_index n = strlen(x->via);

        x->forma = "praefixum";
        si (n == ZEPHYRUM || x->via[n - I] != '/')
        {
            x->via = _iungere(d->piscina, x->via, "/");
        }
    }
    si (in_loco && strcmp(x->elementum, "lectio") == ZEPHYRUM)
    {
        Situs* w = _situm_creare(d, a, p, "scriptura", verbum);

        si (w != NIHIL)
        {
            Situs copia = *x;

            copia.elementum  = "scriptura";
            copia.scripta    = -I;
            *w               = copia;
        }
    }
}

interior vacuum
_ignotum_addere (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* nodus,
                character* mandatum,
       constans character* causa)
{
    Situs* x = _situm_creare(d, a, p, "ignotum", nodus);

    si (x != NIHIL)
    {
        x->mandatum  = mandatum;
        x->causa     = causa;
    }
}

/* imperium per ordinem tabulae (grammatica in commentario tabulae) */
interior vacuum
_tabulam_applicare (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* imperium,
                      Xar* verba,
                      i32  ab,
                StmlNodus* m,
                character* titulus)
{
           character* argumenta  = _ordinis(d, m, "argumenta");
           character* primum     = _ordinis(d, m, "primum");
           character* ultimum    = _ordinis(d, m, "ultimum");
           character* cum_valore = _ordinis(d, m,
               "optiones_cum_valore");
           character* binis     = _ordinis(d, m,
                                      "optiones_binis_valoribus");
            character* o_lectio = _ordinis(d, m, "optio_lectio");
            character* o_scriptura = _ordinis(d, m, "optio_scriptura");
            character* exemplaria = _ordinis(d, m, "exemplar_optiones");
            character* ignotae = _ordinis(d, m, "optiones_ignotae");
            character* recursiones = _ordinis(d, m, "recursio");
            character* in_loco_optiones = _ordinis(d, m, "in_loco");
            character* purum = _ordinis(d, m, "purum");
            character* ignotum = _ordinis(d, m, "ignotum");
                  Xar* positionalia;
                  b32  sine_exemplari  = FALSUM;
                  b32  recursio        = FALSUM;
                  b32  in_loco         = FALSUM;
                  b32  finis           = FALSUM;
                  i32  n;
                  i32  k;

    si (purum != NIHIL && strcmp(purum, "verum") == ZEPHYRUM)
    {
        redde;
    }
    si (ignotum != NIHIL && strcmp(ignotum, "verum") == ZEPHYRUM)
    {
        character* causa = _ordinis(d, m, "causa");

        _ignotum_addere(d, a, p, imperium, titulus,
            causa != NIHIL ? causa : "mandatum ignotum");
        redde;
    }
    positionalia = xar_creare(d->piscina,
        (i32)magnitudo(MateriaNodus*));
    n = xar_numerus(verba);
    per (k = ab + I; k < n; k++)
    {
        constans MateriaNodus* v = *(MateriaNodus**)xar_obtinere(verba,
            k);
                    character* s = _titulus_staticus(d->piscina, v);

        si (   !finis && s != NIHIL && s[ZEPHYRUM] == '-'
            && s[I] != '\0')
        {
            si (strcmp(s, "--") == ZEPHYRUM)
            {
                finis = VERUM;
                perge;
            }
            si (_in_indice(ignotae, s))
            {
                _ignotum_addere(d, a, p, imperium, titulus,
                    _iungere(d->piscina, "optio ", s));
                redde;
            }
            si (_in_indice(exemplaria, s))
            {
                sine_exemplari = VERUM;
            }
            si (_in_indice(recursiones, s))
            {
                recursio = VERUM;
            }
            si (_in_indice(in_loco_optiones, s))
            {
                in_loco = VERUM;
            }
            si (_in_indice(binis, s))
            {
                k += II;
            }
            alioquin si (_in_indice(cum_valore, s) && k + I < n)
            {
                k++;
                si (_in_indice(o_lectio, s))
                {
                    _munus_applicare(d, a, p, *(MateriaNodus**)
                        xar_obtinere(verba, k), "lectio", titulus,
                        FALSUM, FALSUM);
                }
                alioquin si (_in_indice(o_scriptura, s))
                {
                    _munus_applicare(d, a, p, *(MateriaNodus**)
                        xar_obtinere(verba, k), "scriptura", titulus,
                        FALSUM, FALSUM);
                }
            }
            perge;
        }
        si (   s != NIHIL && (strcmp(s, "-") == ZEPHYRUM
                           || (strchr(s, '=') != NIHIL
                               && strchr(s, '/') == NIHIL)))
        {
            perge;   /* stdin; 'var=valor' (awk) */
        }
        *(constans MateriaNodus**)xar_addere(positionalia) = v;
    }
    n = xar_numerus(positionalia);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans character* munus = argumenta;

        si (   k == ZEPHYRUM && primum != NIHIL
            && !(sine_exemplari && strcmp(primum, "exemplar")
                 == ZEPHYRUM))
        {
            munus = primum;
        }
        si (k == n - I && n >= II && ultimum != NIHIL)
        {
            munus = ultimum;
        }
        _munus_applicare(d, a, p,
            *(MateriaNodus**)xar_obtinere(positionalia, k), munus,
            titulus, recursio, in_loco);
    }
}


/* ==================================================
 * Imperia: fontationes, exsecutiones, tabula
 * ================================================== */

interior b32
_ambitum_derivare (
             Derivatio* d,
    constans character* radix);

/* AEDIFICATOR CUSTODITUS (Fran 2026-10-05, T6 (a)): '[ -x P ] || S'
 * (aut -f, -e; test) ubi P binarium domus (classis instrumentum_domus)
 * - S solum currit si P abest, et P provenientia sua clavem tenet.
 * Reddit P (arbori relativum) aut NIHIL. */
interior character*
_custodiam_quaerere (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* imperium)
{
     constans MateriaNodus* catena = imperium->pater;
                       Xar* membra;
     constans MateriaNodus* operator;
     constans MateriaNodus* probatio;
     constans MateriaToken* tok;
                 character* t;
                       Xar* argumenta;
                 character* o;
                     Situs  tmp;
                       i32  k;
                       i32  n;

    si (catena == NIHIL || catena->genus != (s32)CRUSTA_GENUS_CATENA)
    {
        redde NIHIL;
    }
    membra = _nodi_listae(d->piscina, catena,
        (i32)CRUSTA_CATENA_LIBERI);
    n = xar_numerus(membra);
    per (k = II; k < n; k++)
    {
        si (*(MateriaNodus**)xar_obtinere(membra, k) == imperium)
        {
            frange;
        }
    }
    si (k >= n)
    {
        redde NIHIL;
    }
    operator  = *(MateriaNodus**)xar_obtinere(membra, k - I);
    probatio  = *(MateriaNodus**)xar_obtinere(membra, k - II);
    tok       = _token(operator, (i32)CRUSTA_OPERATOR_TOK);
    si (   tok             == NIHIL || !_aequalis(tok->valor, "||")
        || probatio->genus != (s32)CRUSTA_GENUS_IMPERIUM)
    {
        redde NIHIL;
    }
    t = _titulus_staticus(d->piscina,
        crusta_imperium_titulus(probatio));
    argumenta = crusta_imperium_argumenta(d->piscina, probatio);
    si (   t == NIHIL || argumenta == NIHIL
        || !(strcmp(t, "[") == ZEPHYRUM
        || strcmp(t, "test") == ZEPHYRUM)
        || xar_numerus(argumenta) < II)
    {
        redde NIHIL;
    }
    o = _titulus_staticus(d->piscina,
        *(MateriaNodus**)xar_obtinere(argumenta, ZEPHYRUM));
    si (!_in_indice("-x -f -e", o))
    {
        redde NIHIL;
    }
    memset(&tmp, ZEPHYRUM, magnitudo(tmp));
    si (   !_viam_classificare(d, a, p,
               *(MateriaNodus**)xar_obtinere(argumenta, I), probatio,
               &tmp, NIHIL)
        || tmp.classis                               == NIHIL
        || strcmp(tmp.classis, "instrumentum_domus") != ZEPHYRUM)
    {
        redde NIHIL;
    }
    redde tmp.via;
}

/* verbum in sede fontationis/exsecutionis */
interior b32
_locum_tractare (
              Derivatio* d,
                Ambitus* a,
                Plagula* p,
    constans MateriaNodus* verbum,
                    b32  fontatum,
              character* custodia)
{
                 Situs* x;
             character  absoluta[VIA_MAXIMA];
    constans character* rel;

    x = _situm_creare(d, a, p, fontatum ? "fontatio" : "exsecutio",
        verbum);
    si (x == NIHIL)
    {
        redde FALSUM;
    }
    x->medium    = "aedificium";
    x->custodia  = custodia;
    si (!_viam_classificare(d, a, p, verbum, verbum, x, absoluta))
    {
        redde VERUM;
    }
    rel = _relativa(d, absoluta);
    si (rel == NIHIL || _sub_build(rel))
    {
        redde VERUM;   /* externum (identitas alibi) aut productum */
    }
    si (!filum_existit(absoluta))
    {
        si (fontatum || strncmp(rel, "bin/", IV) != ZEPHYRUM)
        {
            x->resolutio  = "nulla";
            x->causa      = "absens";
        }
        redde VERUM;   /* bin/ nondum structum: instrumentum domus */
    }
    si (fontatum)
    {
        si (!_plagulam_in_ambitu(a, absoluta))
        {
            Plagula* nova = _plagulam_parare(d, absoluta);

            si (nova == NIHIL)
            {
                redde FALSUM;
            }
            nova->ordo = xar_numerus(a->plagulae);
            *(Plagula**)xar_addere(a->plagulae) = nova;
        }
        redde VERUM;
    }
    si (!_scriptum_est(d, absoluta))
    {
        redde VERUM;
    }
    {
        Arcus* arc = (Arcus*)xar_addere(d->arcus);

        arc->pater     = a;
        arc->filius    = _duplicare(d->piscina, absoluta);
        arc->custodia  = custodia;
    }
    redde _ambitum_derivare(d, absoluta);   /* processus novus */
}

/* titulus imperii (aut verbum sequens post exec/env/...) */
interior b32
_imperium_tractare (
                Derivatio* d,
                  Ambitus* a,
                  Plagula* p,
    constans MateriaNodus* imperium,
                      Xar* verba,   /* titulus + argumenta */
                      i32  ab)
{
    constans MateriaNodus* titulus;
                character* t;
                StmlNodus* m;
                      i32  k;

    si (ab >= xar_numerus(verba))
    {
        redde VERUM;
    }
    titulus  = *(MateriaNodus**)xar_obtinere(verba, ab);
    t        = _titulus_staticus(d->piscina, titulus);
    si (t == NIHIL || strchr(t, '/') != NIHIL)
    {
        b32 bene = _locum_tractare(d, a, p, titulus, FALSUM,
            ab == ZEPHYRUM ? _custodiam_quaerere(d, a, p, imperium)
                           : NIHIL);

        /* binarium DOMUS cum ordine tabulae (bin/compilator, T7):
         * argumenta quoque per tabulam - situs exsecutionis ultimus */
        si (bene && xar_numerus(a->situs) > ZEPHYRUM)
        {
            Situs* x = (Situs*)xar_obtinere(a->situs,
                xar_numerus(a->situs) - I);

            si (   x->classis                               != NIHIL
                && strcmp(x->classis, "instrumentum_domus") == ZEPHYRUM
                && x->via                                   != NIHIL)
            {
                character* basis = strrchr(x->via, '/');
                StmlNodus* m = _mandatum_invenire(d,
                    basis != NIHIL ? basis + I : x->via);

                si (m != NIHIL)
                {
                    _tabulam_applicare(d, a, p, imperium, verba, ab, m,
                        basis != NIHIL ? basis + I : x->via);
                }
            }
        }
        redde bene;
    }
    si (strcmp(t, "source") == ZEPHYRUM || strcmp(t, ".") == ZEPHYRUM)
    {
        si (ab + I < xar_numerus(verba))
        {
            redde _locum_tractare(d, a, p,
                *(MateriaNodus**)xar_obtinere(verba, ab + I), VERUM,
                NIHIL);
        }
        redde VERUM;
    }
    si (strcmp(t, "bash") == ZEPHYRUM || strcmp(t, "sh") == ZEPHYRUM)
    {
        per (k = ab + I; k < xar_numerus(verba); k++)
        {
            constans MateriaNodus* v = *(MateriaNodus**)xar_obtinere(
                verba, k);
                        character* s = _titulus_staticus(d->piscina, v);

            si (s != NIHIL && s[ZEPHYRUM] == '-')
            {
                si (strcmp(s, "-c") == ZEPHYRUM)
                {
                    _ignotum_addere(d, a, p, imperium, t, "optio -c");
                    redde VERUM;
                }
                perge;
            }
            redde _locum_tractare(d, a, p, v, FALSUM, NIHIL);
        }
        redde VERUM;
    }
    si (   strcmp(t, "exec")  == ZEPHYRUM
        || strcmp(t, "nohup") == ZEPHYRUM
        || strcmp(t, "time")  == ZEPHYRUM)
    {
        redde _imperium_tractare(d, a, p, imperium, verba, ab + I);
    }
    si (strcmp(t, "command") == ZEPHYRUM)
    {
        character* s = ab + I < xar_numerus(verba)
            ? _titulus_staticus(d->piscina,
                  *(MateriaNodus**)xar_obtinere(verba, ab + I))
            : NIHIL;

        si (s != NIHIL && s[ZEPHYRUM] == '-')
        {
            redde VERUM;   /* 'command -v x': quaestio */
        }
        redde _imperium_tractare(d, a, p, imperium, verba, ab + I);
    }
    si (strcmp(t, "env") == ZEPHYRUM)
    {
        per (k = ab + I; k < xar_numerus(verba); k++)
        {
            character* s = _titulus_staticus(d->piscina,
                *(MateriaNodus**)xar_obtinere(verba, k));

            si (   s != NIHIL
                && (s[ZEPHYRUM] == '-' || strchr(s, '=') != NIHIL))
            {
                perge;
            }
            redde _imperium_tractare(d, a, p, imperium, verba, k);
        }
        redde VERUM;
    }
    si (strcmp(t, "[") == ZEPHYRUM || strcmp(t, "test") == ZEPHYRUM)
    {
        _probationes_imperii(d, a, p, verba, ab);
        redde VERUM;
    }
    si (   (strcmp(t, "cd") == ZEPHYRUM
        || strcmp(t, "pushd") == ZEPHYRUM)
        && ab + I < xar_numerus(verba))
    {
        character* s = _titulus_staticus(d->piscina,
            *(MateriaNodus**)xar_obtinere(verba, ab + I));

        /* 'cd W': bash W probat (cd deficit si abest) - oraculum
         * stat eius videt (T5) */
        si (s == NIHIL || s[ZEPHYRUM] != '-')
        {
            _probationem_addere(d, a, p,
                *(MateriaNodus**)xar_obtinere(verba, ab + I), t);
        }
        redde VERUM;
    }
    si (strcmp(t, "eval") == ZEPHYRUM)
    {
        _ignotum_addere(d, a, p, imperium, t, "eval opacum");
        redde VERUM;
    }
    si (_in_serie(AEDIFICIA, t) || _in_nominibus(a->functiones, t))
    {
        redde VERUM;   /* aedificium; functio: corpus ambulatur */
    }
    m = _mandatum_invenire(d, t);
    si (m != NIHIL)
    {
        _tabulam_applicare(d, a, p, imperium, verba, ab, m, t);
        redde VERUM;
    }
    _ignotum_addere(d, a, p, imperium, t, "mandatum ignotum");
    redde VERUM;
}

interior b32
_loca_tractare (
    Derivatio* d,
      Ambitus* a,
      Plagula* p)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(p->imperia); k++)
    {
        constans MateriaNodus* im =
            *(constans MateriaNodus**)xar_obtinere(
                                        p->imperia, k);
        constans MateriaNodus* titulus = crusta_imperium_titulus(im);
                          Xar* verba;
                          Xar* argumenta;
                          i32  j;

        si (titulus == NIHIL)
        {
            perge;
        }
        verba = xar_creare(d->piscina, (i32)magnitudo(MateriaNodus*));
        argumenta = crusta_imperium_argumenta(d->piscina, im);
        si (verba == NIHIL || argumenta == NIHIL)
        {
            redde FALSUM;
        }
        *(constans MateriaNodus**)xar_addere(verba) = titulus;
        per (j = ZEPHYRUM; j < xar_numerus(argumenta); j++)
        {
            *(MateriaNodus**)xar_addere(verba) =
                *(MateriaNodus**)xar_obtinere(argumenta, j);
        }
        si (!_imperium_tractare(d, a, p, im, verba, ZEPHYRUM))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}


/* ==================================================
 * scripta_in_ambitu (sine ordine)
 * ================================================== */

interior b32
_aequat (
    constans character* a,
    constans character* b)
{
    redde a != NIHIL && b != NIHIL && strcmp(a, b) == ZEPHYRUM;
}

interior b32
_incipit (
    constans character* textus,
    constans character* praefixum)
{
    redde strncmp(textus, praefixum, strlen(praefixum)) == ZEPHYRUM;
}

/* scriptura w viam situs s tegit? (aequalis, globus, praefixum) */
interior b32
_tegit (
    constans Situs* w,
    constans Situs* s)
{
    si (_aequat(s->elementum, "enumeratio"))
    {
        redde _incipit(w->via, s->via);
    }
    si (_aequat(w->forma, "praefixum"))
    {
        redde _incipit(s->via, w->via);
    }
    si (_aequat(w->forma, "globus"))
    {
        redde _aequat(s->forma, "globus") ? _aequat(w->via, s->via)
            : _aequat(s->forma, "praefixum") ? _incipit(w->via, s->via)
            : _globus_congruit(w->via, s->via);
    }
    redde _aequat(s->forma, "globus") ? _globus_congruit(s->via, w->via)
        : _aequat(s->forma, "praefixum") ? _incipit(w->via, s->via)
        : _aequat(w->via, s->via);
}

interior vacuum
_scripta_computare (
    Ambitus* a)
{
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < xar_numerus(a->situs); k++)
    {
        Situs* s = (Situs*)xar_obtinere(a->situs, k);

        si (   _aequat(s->elementum, "scriptura")
            || _aequat(s->elementum, "ambitus_lectio")
            || _aequat(s->elementum, "ignotum")
            || _aequat(s->resolutio, "nulla") || s->via == NIHIL)
        {
            perge;
        }
        s->scripta = ZEPHYRUM;
        per (j = ZEPHYRUM; j < xar_numerus(a->situs); j++)
        {
            Situs* w = (Situs*)xar_obtinere(a->situs, j);

            si (   _aequat(w->elementum, "scriptura")
                && !_aequat(w->resolutio, "nulla") && w->via != NIHIL
                && _tegit(w, s))
            {
                s->scripta = I;
                frange;
            }
        }
    }
}


/* ==================================================
 * Ambitus: punctum fixum
 * ================================================== */

/* ambitum (processum) scripti 'radix' derivare: plagulae fontatae
 * definitiones communicant, ergo iteratur dum plagula nova advenit -
 * situs iterationis ULTIMAE soli valent */
interior b32
_ambitum_derivare (
              Derivatio* d,
     constans character* radix)
{
    Ambitus* a;
    Plagula* prima;
        i32  k;
        i32  j;
        i32  numerus;

    per (k = ZEPHYRUM; k < xar_numerus(d->visi); k++)
    {
        si (strcmp(*(character**)xar_obtinere(d->visi, k), radix)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    si (xar_numerus(d->visi) >= (i32)AMBITUS_MAXIMI)
    {
        redde FALSUM;
    }
    *(character**)xar_addere(d->visi) = _duplicare(d->piscina, radix);
    a = (Ambitus*)piscina_allocare(d->piscina,
        (memoriae_index)magnitudo(Ambitus));
    prima = _plagulam_parare(d, radix);
    si (a == NIHIL || prima == NIHIL)
    {
        redde FALSUM;
    }
    memset(a, ZEPHYRUM, magnitudo(Ambitus));
    *(Ambitus**)xar_addere(d->ambitus) = a;
    a->radix_via = _duplicare(d->piscina, radix);
    a->plagulae = xar_creare(d->piscina, (i32)magnitudo(Plagula*));
    *(Plagula**)xar_addere(a->plagulae) = prima;
    fac
    {
        numerus          = xar_numerus(a->plagulae);
        a->definitiones  = xar_creare(d->piscina,
            (i32)magnitudo(Definitio));
        a->locales       = xar_creare(d->piscina,
            (i32)magnitudo(Localis));
        a->functiones    = xar_creare(d->piscina,
            (i32)magnitudo(character*));
        a->assignata     = xar_creare(d->piscina,
            (i32)magnitudo(character*));
        a->situs         = xar_creare(d->piscina,
            (i32)magnitudo(Situs));
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            Plagula* p = *(Plagula**)xar_obtinere(a->plagulae, k);

            si (p->radix == NIHIL)
            {
                perge;
            }
            _locales_colligere(d, a, p);
            per (j = ZEPHYRUM; j < xar_numerus(p->functiones); j++)
            {
                constans MateriaNodus* f = *(constans MateriaNodus**)
                    xar_obtinere(p->functiones, j);
                constans MateriaToken* t = _token(f,
                    (i32)CRUSTA_FUNCTIO_TOK_TITULUS);

                si (t != NIHIL)
                {
                    _nomen_addere(d->piscina, a->functiones,
                        chorda_ut_cstr(t->valor, d->piscina));
                }
            }
        }
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            Plagula* p = *(Plagula**)xar_obtinere(a->plagulae, k);

            si (p->radix != NIHIL)
            {
                _definitiones_colligere(d, a, p);
                _assignata_colligere(d, a, p);
            }
        }
        per (k = ZEPHYRUM; k < xar_numerus(a->definitiones); k++)
        {
            _nomen_addere(d->piscina, a->assignata,
                ((Definitio*)xar_obtinere(a->definitiones,
                k))->titulus);
        }
        per (k = ZEPHYRUM; k < xar_numerus(a->locales); k++)
        {
            _nomen_addere(d->piscina, a->assignata,
                ((Localis*)xar_obtinere(a->locales, k))->titulus);
        }
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            Plagula* p = *(Plagula**)xar_obtinere(a->plagulae, k);

            si (p->radix == NIHIL)
            {
                Situs* x = _situm_creare(d, a, p, "ignotum", NIHIL);

                si (x != NIHIL)
                {
                    x->causa = p->causa;
                }
                perge;
            }
            si (!_loca_tractare(d, a, p))
            {
                redde FALSUM;
            }
            _redirectiones_tractare(d, a, p);
            _iudicia_tractare(d, a, p);
            _globos_tractare(d, a, p);
            _variabiles_tractare(d, a, p);
        }
    }
    dum (xar_numerus(a->plagulae) != numerus);
    _scripta_computare(a);
    redde VERUM;
}


/* ==================================================
 * Emissio STML
 * ================================================== */

interior s32
_situs_comparare (
    constans vacuum* x,
    constans vacuum* y)
{
    constans Situs* a = (constans Situs*)x;
    constans Situs* b = (constans Situs*)y;
               s32  c;

    si (a->plagula->ordo != b->plagula->ordo)
    {
        redde a->plagula->ordo < b->plagula->ordo ? -I : I;
    }
    si (a->tractus.initium != b->tractus.initium)
    {
        redde a->tractus.initium < b->tractus.initium ? -I : I;
    }
    c = (s32)strcmp(a->elementum, b->elementum);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    redde (s32)strcmp(a->via ? a->via : (a->titulus ? a->titulus : ""),
                      b->via ? b->via : (b->titulus ? b->titulus : ""));
}

interior b32
_attributum (
             Derivatio* d,
             StmlNodus* e,
    constans character* titulus,
    constans character* valor)
{
    si (valor == NIHIL)
    {
        redde VERUM;
    }
    redde stml_attributum_addere(e, d->piscina, d->intern, titulus,
        valor);
}

interior constans character*
_via_relativa (
             Derivatio* d,
    constans character* via)
{
    constans character* rel = _relativa(d, via);

    redde rel != NIHIL ? rel : via;
}

interior StmlNodus*
_situm_emittere (
         Derivatio* d,
    constans Situs* s)
{
    StmlNodus* e = stml_elementum_creare(d->piscina, d->intern,
        s->elementum);
     character sedes[XCVI];
     character octeti[XLVIII];
           b32 bonum;

    si (e == NIHIL)
    {
        redde NIHIL;
    }
    si (s->tractus.initium >= ZEPHYRUM)
    {
        sprintf(sedes, "%u:%u-%u:%u",
            (insignatus integer)s->tractus.linea,
            (insignatus integer)s->tractus.columna,
            (insignatus integer)s->tractus.linea_finis,
            (insignatus integer)s->tractus.columna_finis);
        sprintf(octeti, "%ld-%ld", (longus)s->tractus.initium,
            (longus)s->tractus.finis);
    }
    alioquin
    {
        strcpy(sedes, "1:1-1:1");
        strcpy(octeti, "0-0");
    }
    bonum = _attributum(d, e, "via", s->via)
        && _attributum(d, e, "titulus", s->titulus)
        && _attributum(d, e, "exemplar", s->exemplar)
        && _attributum(d, e, "textus", s->textus)
        && _attributum(d, e, "forma", s->forma)
        && _attributum(d, e, "resolutio", s->resolutio)
        && _attributum(d, e, "classis",
               _aequat(s->resolutio, "nulla") ? NIHIL : s->classis)
        && _attributum(d, e, "scripta_in_ambitu",
               s->scripta < ZEPHYRUM ? NIHIL
               : s->scripta > ZEPHYRUM ? "verum" : "falsum")
        && _attributum(d, e, "assignatum",
               _aequat(s->elementum, "ambitus_lectio") ? "falsum"
               : NIHIL)
        && _attributum(d, e, "per",
               _aequat(s->elementum, "ambitus_lectio")
               || _aequat(s->elementum, "ignotum") ? NIHIL : s->medium)
        && _attributum(d, e, "mandatum", s->mandatum)
        && _attributum(d, e, "operator", s->operator)
        && _attributum(d, e, "causa", s->causa)
        && _attributum(d, e, "custodia", s->custodia)
        && _attributum(d, e, "plagula",
               _via_relativa(d, s->plagula->via))
        && _attributum(d, e, "sedes", sedes)
        && _attributum(d, e, "octeti", octeti);
    redde bonum ? e : NIHIL;
}

/* duo situs idem (eadem sedes, idem elementum, via, operator)? */
interior b32
_idem (
    constans Situs* a,
    constans Situs* b)
{
    redde a->plagula == b->plagula
        && a->tractus.initium == b->tractus.initium
        && a->tractus.finis == b->tractus.finis
        && strcmp(a->elementum, b->elementum) == ZEPHYRUM
        && ((a->via == NIHIL && b->via == NIHIL)
            || _aequat(a->via, b->via))
        && ((a->operator == NIHIL && b->operator == NIHIL)
            || _aequat(a->operator, b->operator))
        && ((a->titulus == NIHIL && b->titulus == NIHIL)
            || _aequat(a->titulus, b->titulus));
}

interior StmlNodus*
_emittere (
             Derivatio* d,
    constans character* scriptum)
{
    StmlNodus* radix_summarii = stml_elementum_creare(d->piscina,
        d->intern, "effectus");
           i32 k;
           i32 j;

    si (   radix_summarii == NIHIL
        || !_attributum(d, radix_summarii, "lingua", "bash")
        || !_attributum(d, radix_summarii, "radix",
               _via_relativa(d, scriptum)))
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(d->ambitus); k++)
    {
        Ambitus* a = *(Ambitus**)xar_obtinere(d->ambitus, k);
      StmlNodus* pr = stml_elementum_creare(d->piscina, d->intern,
                         "processus");
         Situs* prior = NIHIL;

        si (   pr == NIHIL
            || !_attributum(d, pr, "radix", _via_relativa(d,
            a->radix_via))
            || !_attributum(d, pr, "custodia", a->custodia)
            || !stml_liberum_addere(radix_summarii, pr))
        {
            redde NIHIL;
        }
        xar_ordinare(a->situs, _situs_comparare);
        per (j = ZEPHYRUM; j < xar_numerus(a->situs); j++)
        {
             Situs* s = (Situs*)xar_obtinere(a->situs, j);
         StmlNodus* e;

            si (prior != NIHIL && _idem(prior, s))
            {
                perge;
            }
            prior  = s;
            e      = _situm_emittere(d, s);
            si (e == NIHIL || !stml_liberum_addere(pr, e))
            {
                redde NIHIL;
            }
        }
    }
    redde radix_summarii;
}


/* ==================================================
 * Interfacies
 * ================================================== */

interior b32
_tabulam_onerare (
             Derivatio* d,
             StmlNodus* mandata)
{
     StmlNodus* radix_tabulae = mandata;
           i32  k;

    si (radix_tabulae == NIHIL)
    {
           character via[VIA_MAXIMA];
              chorda fons;
        StmlResultus r;

        si (strlen(d->radix) + XL >= (memoriae_index)VIA_MAXIMA)
        {
            redde FALSUM;
        }
        sprintf(via, "%s/crusta/effectus_mandata.stml", d->radix);
        fons = filum_legere_totum(via, d->piscina);
        si (fons.datum == NIHIL)
        {
            redde FALSUM;
        }
        r = stml_legere(fons, d->piscina, d->intern);
        si (!r.successus)
        {
            redde FALSUM;
        }
        radix_tabulae = r.elementum_radix;
    }
    si (   radix_tabulae == NIHIL || radix_tabulae->titulus == NIHIL
        || !chorda_aequalis_literis(*radix_tabulae->titulus, "mandata"))
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; radix_tabulae->liberi
                       && k < xar_numerus(radix_tabulae->liberi); k++)
    {
        StmlNodus* m = *(StmlNodus**)xar_obtinere(radix_tabulae->liberi,
            k);
        character* t;

        si (m->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        t = _ordinis(d, m, "titulus");
        si (t != NIHIL)
        {
            Mandatum* o = (Mandatum*)xar_addere(d->tabula);

            o->titulus  = t;
            o->nodus    = m;
        }
    }
    redde xar_numerus(d->tabula) > ZEPHYRUM;
}

interior Ambitus*
_ambitum_invenire (
             Derivatio* d,
    constans character* radix)
{
    i32 k;

    per (k = ZEPHYRUM; k < xar_numerus(d->ambitus); k++)
    {
        Ambitus* a = *(Ambitus**)xar_obtinere(d->ambitus, k);

        si (strcmp(a->radix_via, radix) == ZEPHYRUM)
        {
            redde a;
        }
    }
    redde NIHIL;
}

/* CUSTODIA PROCESSUUM (post punctum fixum; graphus, non vocatio): liber
 * = attingitur ab radice via arcuum SINE custodia; ceteri custoditi,
 * P ex arcu custodito in eos aut ex patre custodito. Scriptum et
 * custodite et libere attingibile LIBERUM manet (vexilla.sh). */
interior vacuum
_custodias_computare (
    Derivatio* d)
{
    b32 mutatum;
    i32 k;

    si (xar_numerus(d->ambitus) == ZEPHYRUM)
    {
        redde;
    }
    (*(Ambitus**)xar_obtinere(d->ambitus, ZEPHYRUM))->liber = VERUM;
    fac
    {
        mutatum = FALSUM;
        per (k = ZEPHYRUM; k < xar_numerus(d->arcus); k++)
        {
            Arcus* arc     = (Arcus*)xar_obtinere(d->arcus, k);
          Ambitus* filius  = _ambitum_invenire(d, arc->filius);

            si (   filius        != NIHIL && !filius->liber
                && arc->pater->liber
                && arc->custodia == NIHIL)
            {
                filius->liber  = VERUM;
                mutatum        = VERUM;
            }
        }
    }
    dum (mutatum);
    fac
    {
        mutatum = FALSUM;
        per (k = ZEPHYRUM; k < xar_numerus(d->arcus); k++)
        {
            Arcus* arc     = (Arcus*)xar_obtinere(d->arcus, k);
          Ambitus* filius  = _ambitum_invenire(d, arc->filius);

            si (filius == NIHIL || filius->liber || filius->custodia)
            {
                perge;
            }
            si (arc->custodia != NIHIL)
            {
                filius->custodia  = arc->custodia;
                mutatum           = VERUM;
            }
            alioquin si (arc->pater->custodia != NIHIL)
            {
                filius->custodia  = arc->pater->custodia;
                mutatum           = VERUM;
            }
        }
    }
    dum (mutatum);
}

StmlNodus*
crusta_effectus_derivare (
               Piscina*  piscina,
    InternamentumChorda* intern,
    constans character*  radix,
    constans character*  scriptum,
             StmlNodus*  mandata,
    constans character** causa_out)
{
    Derivatio d;
    character absoluta[VIA_MAXIMA];

    si (causa_out != NIHIL)
    {
        *causa_out = NIHIL;
    }
    d.piscina        = piscina;
    d.intern         = intern;
    d.radix          = radix;
    d.radix_mensura  = (i32)strlen(radix);
    d.visi           = xar_creare(piscina, (i32)magnitudo(character*));
    d.ambitus        = xar_creare(piscina, (i32)magnitudo(Ambitus*));
    d.tabula         = xar_creare(piscina, (i32)magnitudo(Mandatum));
    d.arcus          = xar_creare(piscina, (i32)magnitudo(Arcus));
    d.causa          = NIHIL;
    d.acervus        = xar_creare(piscina,
        (i32)magnitudo(constans character*));
    si (   d.visi == NIHIL || d.ambitus == NIHIL || d.tabula == NIHIL
        || !_absolutam_facere(scriptum, radix, absoluta))
    {
        redde NIHIL;
    }
    si (!_tabulam_onerare(&d, mandata))
    {
        si (causa_out != NIHIL)
        {
            *causa_out = "tabula mandatorum illegibilis";
        }
        redde NIHIL;
    }
    si (!filum_existit(absoluta))
    {
        si (causa_out != NIHIL)
        {
            *causa_out = "scriptum absens";
        }
        redde NIHIL;
    }
    si (!_ambitum_derivare(&d, absoluta))
    {
        si (causa_out != NIHIL)
        {
            *causa_out = "memoria deficit aut ambitus nimis multi";
        }
        redde NIHIL;
    }
    _custodias_computare(&d);
    redde _emittere(&d, absoluta);
}


/* ==================================================
 * Oraculum: liber interpositionis -> summarium observatum
 * (effectus-spec par. VI.2; planum T5)
 * ================================================== */

nomen structura {
          i32  pid;
    character* programma;   /* progname imaginis praesentis */
          Xar* viae_path;   /* character*: directoria PATH sua */
          Xar* lecta;       /* character*: viae lectae (ante_scripta) */
} Processus;

nomen structura {
          i32  pid;
    character* vocans;      /* programma quod exsequebatur */
    character* cwd;
          Xar* argumenta;   /* character* */
} Exsecutio;

interior Processus*
_processum_quaerere (
     Derivatio* d,
           Xar* processus,
           i32  pid)
{
    Processus* p;
          i32  k;

    per (k = ZEPHYRUM; k < xar_numerus(processus); k++)
    {
        p = (Processus*)xar_obtinere(processus, k);
        si (p->pid == pid)
        {
            redde p;
        }
    }
    p             = (Processus*)xar_addere(processus);
    p->pid        = pid;
    p->programma  = _duplicare(d->piscina, "?");
    p->viae_path  = xar_creare(d->piscina, (i32)magnitudo(character*));
    p->lecta      = xar_creare(d->piscina, (i32)magnitudo(character*));
    redde p;
}

/* campos lineae tabulis divisos in seriem (NIHIL terminatam) */
interior i32
_campi (
               Piscina*  piscina,
                chorda   linea,
             character** campi,
                   i32   maximum)
{
    i32 n   = ZEPHYRUM;
    i32 ab  = ZEPHYRUM;
    i32 k;

    per (k = ZEPHYRUM; k <= linea.mensura && n < maximum; k++)
    {
        si (k == linea.mensura || linea.datum[k] == '\t')
        {
            campi[n++] = chorda_ut_cstr(chorda_sectio(linea, ab, k),
                piscina);
            ab = k + I;
        }
    }
    redde n;
}

/* 'a:b:c' -> Xar de character* */
interior vacuum
_path_dividere (
                Derivatio* d,
       constans character* path,
                      Xar* exitus)
{
    constans character* c = path;

    dum (c != NIHIL && *c != '\0')
    {
        constans character* f = strchr(c, ':');
            memoriae_index  n = f != NIHIL ? (memoriae_index)(f - c)
                                           : strlen(c);
                 character* t = (character*)piscina_allocare(d->piscina,
                                    n + I);

        memcpy(t, c, n);
        t[n]                              = '\0';
        *(character**)xar_addere(exitus)  = t;
        c                                 = f != NIHIL ? f + I : NIHIL;
    }
}

/* via absoluta normata arbori relativa; NIHIL extra arborem aut ipsa
 * radix */
interior constans character*
_observatam_relativam (
             Derivatio* d,
    constans character* via,
    constans character* cwd)
{
    character absoluta[VIA_MAXIMA];

    si (!_absolutam_facere(via, cwd != NIHIL ? cwd : d->radix,
            absoluta))
    {
        redde NIHIL;
    }
    redde _relativa(d, absoluta) != NIHIL
        ? _duplicare(d->piscina, _relativa(d, absoluta)) : NIHIL;
}

interior vacuum
_observatum_addere (
             Derivatio* d,
               Ambitus* a,
               Plagula* p,
    constans character* elementum,
    constans character* relativa,
    constans character* forma,
             character* mandatum)
{
    Situs* x;
      i32  k;

    per (k = ZEPHYRUM; k < xar_numerus(a->situs); k++)
    {
        Situs* s = (Situs*)xar_obtinere(a->situs, k);

        si (   _aequat(s->elementum, elementum)
            && _aequat(s->via, relativa))
        {
            redde;   /* unicum per (elementum, via) */
        }
    }
    x = _situm_creare(d, a, p, elementum, NIHIL);
    si (x == NIHIL)
    {
        redde;
    }
    x->via        = _duplicare(d->piscina, relativa);
    x->forma      = forma;
    x->resolutio  = "plena";
    x->classis    = _classis_arboris(relativa);
    x->medium     = "observatum";
    x->mandatum   = mandatum;
}

/* argv mandati per ordinem tabulae (logica _tabulam_applicare, super
 * chordas: bash globos iam expandit, viae relativae ad cwd) */
interior vacuum
_argv_interpretari (
             Derivatio* d,
               Ambitus* a,
               Plagula* p,
             StmlNodus* m,
             character* titulus,
                   Xar* argumenta,
    constans character* cwd)
{
    character* munus_omnium  = _ordinis(d, m, "argumenta");
    character* primum        = _ordinis(d, m, "primum");
    character* ultimum       = _ordinis(d, m, "ultimum");
    character* cum_valore    = _ordinis(d, m, "optiones_cum_valore");
    character* binis = _ordinis(d, m,
        "optiones_binis_valoribus");
    character* o_lectio     = _ordinis(d, m, "optio_lectio");
    character* o_scriptura  = _ordinis(d, m, "optio_scriptura");
    character* exemplaria   = _ordinis(d, m, "exemplar_optiones");
    character* recursiones  = _ordinis(d, m, "recursio");
    character* in_loco_opt  = _ordinis(d, m, "in_loco");
    character* ignotae      = _ordinis(d, m, "optiones_ignotae");
    character* purum        = _ordinis(d, m, "purum");
    character* ignotum      = _ordinis(d, m, "ignotum");
          Xar* positionalia;
          b32  sine_exemplari  = FALSUM;
          b32  recursio        = FALSUM;
          b32  in_loco         = FALSUM;
          b32  finis           = FALSUM;
          i32  n               = xar_numerus(argumenta);
          i32  k;

    si (_aequat(purum, "verum") || _aequat(ignotum, "verum"))
    {
        redde;
    }
    positionalia = xar_creare(d->piscina, (i32)magnitudo(character*));
    per (k = I; k < n; k++)
    {
        character* s = *(character**)xar_obtinere(argumenta, k);

        si (!finis && s[ZEPHYRUM] == '-' && s[I] != '\0')
        {
            si (strcmp(s, "--") == ZEPHYRUM)
            {
                finis = VERUM;
                perge;
            }
            si (_in_indice(ignotae, s))
            {
                redde;   /* ignotus utrimque (staticum: ignotum) */
            }
            si (_in_indice(exemplaria, s))
            {
                sine_exemplari = VERUM;
            }
            si (_in_indice(recursiones, s))
            {
                recursio = VERUM;
            }
            si (_in_indice(in_loco_opt, s))
            {
                in_loco = VERUM;
            }
            si (_in_indice(binis, s))
            {
                k += II;
            }
            alioquin si (_in_indice(cum_valore, s) && k + I < n)
            {
                character* v = *(character**)xar_obtinere(argumenta,
                    ++k);
                constans character* rel = _observatam_relativam(d, v,
                    cwd);

                si (rel != NIHIL && _in_indice(o_lectio, s))
                {
                    _observatum_addere(d, a, p, "lectio", rel, "via",
                        titulus);
                }
                alioquin si (rel != NIHIL && _in_indice(o_scriptura, s))
                {
                    _observatum_addere(d, a, p, "scriptura", rel, "via",
                        titulus);
                }
            }
            perge;
        }
        si (   strcmp(s, "-") == ZEPHYRUM
            || (strchr(s, '=') != NIHIL && strchr(s, '/') == NIHIL))
        {
            perge;
        }
        *(character**)xar_addere(positionalia) = s;
    }
    n = xar_numerus(positionalia);
    per (k = ZEPHYRUM; k < n; k++)
    {
        constans character* munus = munus_omnium;
        constans character* rel;
        constans character* elementum;

        si (   k == ZEPHYRUM && primum != NIHIL
            && !(sine_exemplari && _aequat(primum, "exemplar")))
        {
            munus = primum;
        }
        si (k == n - I && n >= II && ultimum != NIHIL)
        {
            munus = ultimum;
        }
        si (   munus == NIHIL || _aequat(munus, "nullum")
            || _aequat(munus, "exemplar"))
        {
            perge;
        }
        rel = _observatam_relativam(d,
            *(character**)xar_obtinere(positionalia, k), cwd);
        si (rel == NIHIL)
        {
            perge;
        }
        elementum = _aequat(munus, "lectio") ? "lectio"
            : _aequat(munus, "scriptura") ? "scriptura"
            : _aequat(munus, "exsecutio") ? "exsecutio"
            : _aequat(munus, "enumeratio") ? "enumeratio" : "probatio";
        _observatum_addere(d, a, p, elementum, rel,
            recursio ? "praefixum" : "via", titulus);
        si (in_loco && _aequat(elementum, "lectio"))
        {
            _observatum_addere(d, a, p, "scriptura", rel, "via",
                titulus);
        }
    }
}

StmlNodus*
crusta_effectus_observata (
               Piscina*  piscina,
    InternamentumChorda* intern,
    constans character*  radix,
    constans character*  scriptum,
                 chorda  liber,
             StmlNodus*  mandata,
                   Xar*  ante_scripta,
    constans character** causa_out)
{
    Derivatio  d;
      Ambitus* a;
      Plagula* p;
          Xar* processus;
          Xar* exsecutiones;
    character  absoluta[VIA_MAXIMA];
          i32  ab = ZEPHYRUM;
          i32  k;

    si (causa_out != NIHIL)
    {
        *causa_out = NIHIL;
    }
    d.piscina        = piscina;
    d.intern         = intern;
    d.radix          = radix;
    d.radix_mensura  = (i32)strlen(radix);
    d.visi           = xar_creare(piscina, (i32)magnitudo(character*));
    d.ambitus        = xar_creare(piscina, (i32)magnitudo(Ambitus*));
    d.tabula         = xar_creare(piscina, (i32)magnitudo(Mandatum));
    d.arcus          = xar_creare(piscina, (i32)magnitudo(Arcus));
    d.causa          = NIHIL;
    d.acervus        = xar_creare(piscina,
        (i32)magnitudo(constans character*));
    a = (Ambitus*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Ambitus));
    p = (Plagula*)piscina_allocare(piscina,
        (memoriae_index)magnitudo(Plagula));
    si (   a == NIHIL || p == NIHIL || d.ambitus == NIHIL
        || !_absolutam_facere(scriptum, radix, absoluta)
        || !_tabulam_onerare(&d, mandata))
    {
        si (causa_out != NIHIL)
        {
            *causa_out = "tabula mandatorum illegibilis aut memoria";
        }
        redde NIHIL;
    }
    memset(a, ZEPHYRUM, magnitudo(Ambitus));
    memset(p, ZEPHYRUM, magnitudo(Plagula));
    p->via = _duplicare(piscina, absoluta);
    a->radix_via = p->via;
    a->situs = xar_creare(piscina, (i32)magnitudo(Situs));
    a->plagulae = xar_creare(piscina, (i32)magnitudo(Plagula*));
    *(Plagula**)xar_addere(a->plagulae) = p;
    *(Ambitus**)xar_addere(d.ambitus) = a;
    processus = xar_creare(piscina, (i32)magnitudo(Processus));
    exsecutiones = xar_creare(piscina, (i32)magnitudo(Exsecutio));

    per (k = ZEPHYRUM; k <= liber.mensura; k++)
    {
            character* campi[V];
                  i32  n;
            Processus* pr;
               chorda  linea;

        si (k < liber.mensura && liber.datum[k] != '\n')
        {
            perge;
        }
        linea  = chorda_sectio(liber, ab, k);
        ab     = k + I;
        n      = _campi(piscina, linea, campi, V);
        si (n < III)
        {
            perge;
        }
        pr = _processum_quaerere(&d, processus,
            (i32)strtol(campi[I], NIHIL, X));
        si (_aequat(campi[ZEPHYRUM], "P"))
        {
            pr->programma = campi[II];
            pr->viae_path = xar_creare(piscina,
                (i32)magnitudo(character*));
            si (n >= IV)
            {
                _path_dividere(&d, campi[III], pr->viae_path);
            }
        }
        alioquin si (_aequat(campi[ZEPHYRUM], "D"))
        {
            Exsecutio* e = (Exsecutio*)xar_addere(exsecutiones);

            e->pid     = pr->pid;
            e->vocans  = pr->programma;
            e->cwd     = campi[II];
            e->argumenta  = xar_creare(piscina,
                (i32)magnitudo(character*));
        }
        alioquin si (   _aequat(campi[ZEPHYRUM], "A") && n >= IV
                     && xar_numerus(exsecutiones) > ZEPHYRUM)
        {
             Exsecutio* ultima = NIHIL;
                   i32  j;

            /* ultima exsecutio EIUSDEM pid */
            per (j = ZEPHYRUM; j < xar_numerus(exsecutiones); j++)
            {
                Exsecutio* e = (Exsecutio*)xar_obtinere(exsecutiones,
                    j);

                si (e->pid == pr->pid)
                {
                    ultima = e;
                }
            }
            si (ultima != NIHIL)
            {
                *(character**)xar_addere(ultima->argumenta) =
                    campi[III];
            }
        }
        alioquin si (   _aequat(campi[ZEPHYRUM], "E") && n >= V
                     && _aequat(pr->programma, "bash"))
        {
            constans character* genus = campi[II];
                     character  normata[VIA_MAXIMA];
            constans character* rel;
                     character  directorium[VIA_MAXIMA];
                           s32  rc = (s32)strtol(campi[IV], NIHIL, X);
                           i32  j;
                           b32  per_path = FALSUM;

            si (   campi[III][ZEPHYRUM] != '/'
                || strlen(campi[III])   >= (memoriae_index)VIA_MAXIMA)
            {
                perge;   /* dirfd alius aut via nimis longa */
            }
            strcpy(normata, campi[III]);
            _viam_normare(normata);
            strcpy(directorium, normata);
            _directorium_viae(directorium);
            per (j = ZEPHYRUM; j < xar_numerus(pr->viae_path); j++)
            {
                si (strcmp(*(character**)xar_obtinere(pr->viae_path, j),
                        directorium) == ZEPHYRUM)
                {
                    per_path = VERUM;
                }
            }
            rel = _relativa(&d, normata);
            si (   rel == NIHIL || per_path
                || _aequat(genus, "SHEBANG")
                || (rc < ZEPHYRUM && _aequat(genus, "EXECVE")))
            {
                perge;
            }
            si (_aequat(genus, "LEGERE"))
            {
                _observatum_addere(&d, a, p, "lectio", rel, "via",
                    pr->programma);
                _nomen_addere(piscina, pr->lecta, rel);
            }
            alioquin si (_aequat(genus, "SCRIBERE"))
            {
                _observatum_addere(&d, a, p, "scriptura", rel, "via",
                    pr->programma);
                si (   ante_scripta != NIHIL
                    && _in_nominibus(pr->lecta, rel))
                {
                    _nomen_addere(piscina, ante_scripta, rel);
                }
            }
            alioquin si (_aequat(genus, "EXECVE"))
            {
                _observatum_addere(&d, a, p, "exsecutio", rel, "via",
                    pr->programma);
            }
            alioquin si (_aequat(genus, "OPENDIR"))
            {
                _observatum_addere(&d, a, p, "enumeratio", rel, "via",
                    pr->programma);
            }
            alioquin
            {
                _observatum_addere(&d, a, p, "probatio", rel, "via",
                    pr->programma);
            }
        }
    }
    /* argv mandatorum quos bash exsecutus est (SIP: per tabulam) */
    per (k = ZEPHYRUM; k < xar_numerus(exsecutiones); k++)
    {
        Exsecutio* e = (Exsecutio*)xar_obtinere(exsecutiones, k);
        character* titulus;
        StmlNodus* m;

        si (   !_aequat(e->vocans, "bash")
            || xar_numerus(e->argumenta) == ZEPHYRUM)
        {
            perge;
        }
        titulus = *(character**)xar_obtinere(e->argumenta, ZEPHYRUM);
        si (strrchr(titulus, '/') != NIHIL)
        {
            titulus = strrchr(titulus, '/') + I;
        }
        m = _mandatum_invenire(&d, titulus);
        si (m != NIHIL)
        {
            _argv_interpretari(&d, a, p, m, titulus, e->argumenta,
                e->cwd);
        }
    }
    redde _emittere(&d, absoluta);
}


/* ==================================================
 * Comparatio: observata quae summarium staticum non tegit
 * ================================================== */

interior constans character*
_attributi (
             StmlNodus* n,
    constans character* titulus,
               Piscina* piscina)
{
    chorda* v = stml_attributum_capere(n, titulus);

    redde v == NIHIL ? NIHIL : chorda_ut_cstr(*v, piscina);
}

/* situs staticus s viam observatam tegit? */
interior b32
_staticus_tegit (
              Piscina* piscina,
           StmlNodus* s,
    constans character* elementum_observati,
    constans character* via_observata)
{
      constans character* el;
      constans character* via;
      constans character* forma;
      constans character* resolutio;
      constans character* exemplar;
          memoriae_index  n;

    el         = chorda_ut_cstr(*s->titulus, piscina);
    via        = _attributi(s, "via", piscina);
    forma      = _attributi(s, "forma", piscina);
    resolutio  = _attributi(s, "resolutio", piscina);
    exemplar   = _attributi(s, "exemplar", piscina);
    si (via == NIHIL || _aequat(resolutio, "nulla"))
    {
        redde FALSUM;
    }
    /* genera compatibilia */
    si (   _aequat(elementum_observati, "lectio")
        && !(_aequat(el, "lectio") || _aequat(el, "fontatio")
             || _aequat(el, "exsecutio")))
    {
        redde FALSUM;
    }
    si (   _aequat(elementum_observati, "scriptura")
        && !_aequat(el, "scriptura"))
    {
        redde FALSUM;
    }
    si (   _aequat(elementum_observati, "exsecutio")
        && !(_aequat(el, "exsecutio") || _aequat(el, "fontatio")))
    {
        redde FALSUM;
    }
    si (   _aequat(elementum_observati, "enumeratio")
        && !_aequat(el, "enumeratio"))
    {
        redde FALSUM;
    }
    n = strlen(via);
    si (_aequat(el, "enumeratio") && exemplar != NIHIL)
    {
        character exemplum[VIA_MAXIMA];

        /* directorium ipsum (opendir) aut res globo congruens (stat) */
        si (   strncmp(via, via_observata, n > ZEPHYRUM ? n - I : n)
                   == ZEPHYRUM
            && strlen(via_observata) + I == n)
        {
            redde VERUM;
        }
        si (n + strlen(exemplar) + I >= (memoriae_index)VIA_MAXIMA)
        {
            redde FALSUM;
        }
        strcpy(exemplum, strcmp(via, "./") == ZEPHYRUM ? "" : via);
        strcat(exemplum, exemplar);
        redde _globus_congruit(exemplum, via_observata);
    }
    si (_aequat(forma, "globus"))
    {
        redde _globus_congruit(via, via_observata);
    }
    si (_aequat(forma, "praefixum"))
    {
        redde strncmp(via_observata, via, n) == ZEPHYRUM
            || (n > ZEPHYRUM && via[n - I] == '/'
                && strncmp(via_observata, via, n - I) == ZEPHYRUM
                && via_observata[n - I] == '\0');
    }
    redde strcmp(via, via_observata) == ZEPHYRUM
        || (n > ZEPHYRUM && via[n - I] == '/'
            && strncmp(via, via_observata, n - I) == ZEPHYRUM
            && via_observata[n - I] == '\0');
}

/* situs staticus IRRESOLUTUS generis compatibilis et mandati eiusdem
 * observatum explicat (clavis ibi iam IGNOTUM est): mandatum
 * observatum 'bash' = syscallum ipsius bash (redirectio, aedificium);
 * aliter titulus mandati (argv) */
interior b32
_irresolutus_explicat (
               Piscina* piscina,
             StmlNodus* s,
    constans character* elementum_observati,
    constans character* mandatum_observati)
{
    constans character* el = chorda_ut_cstr(*s->titulus, piscina);
    constans character* resolutio = _attributi(s, "resolutio", piscina);
    constans character* medium = _attributi(s, "per", piscina);
    constans character* mandatum = _attributi(s, "mandatum", piscina);

    si (_aequat(resolutio, "plena") || resolutio == NIHIL)
    {
        redde FALSUM;
    }
    si (!(_aequat(el, elementum_observati)
          || (_aequat(elementum_observati, "lectio")
              && (_aequat(el, "fontatio") || _aequat(el, "exsecutio")))
          || _aequat(elementum_observati, "probatio")))
    {
        redde FALSUM;
    }
    si (_aequat(mandatum_observati, "bash"))
    {
        redde _aequat(medium, "redirectio")
            || _aequat(medium, "aedificium");
    }
    redde _aequat(mandatum, mandatum_observati);
}

Xar*
crusta_effectus_non_tecta (
       Piscina* piscina,
     StmlNodus* staticum,
     StmlNodus* observatum,
           Xar* explicata)
{
    Xar* exitus   = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    Xar* statici  = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    Xar* radices  = xar_creare(piscina, (i32)magnitudo(character*));
    i32  i;
    i32  j;

    si (exitus == NIHIL || staticum == NIHIL || observatum == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; staticum->liberi
                       && i < xar_numerus(staticum->liberi); i++)
    {
                 StmlNodus* pr;
        constans character* r;

        pr = *(StmlNodus**)xar_obtinere(staticum->liberi, i);

        si (pr->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        r = _attributi(pr, "radix", piscina);
        si (r != NIHIL)
        {
            *(constans character**)xar_addere(radices) = r;
        }
        per (j = ZEPHYRUM; pr->liberi && j < xar_numerus(pr->liberi);
             j++)
        {
            StmlNodus* s = *(StmlNodus**)xar_obtinere(pr->liberi, j);

            si (s->genus == STML_NODUS_ELEMENTUM)
            {
                *(StmlNodus**)xar_addere(statici) = s;
            }
        }
    }
    per (i = ZEPHYRUM; observatum->liberi
                       && i < xar_numerus(observatum->liberi); i++)
    {
        StmlNodus* pr = *(StmlNodus**)xar_obtinere(observatum->liberi,
            i);

        per (j = ZEPHYRUM; pr->liberi && j < xar_numerus(pr->liberi);
             j++)
        {
                     StmlNodus* o = *(StmlNodus**)xar_obtinere(
                                        pr->liberi, j);
            constans character* el;
            constans character* via;
                           b32  tectum = FALSUM;
                           i32  k;

            si (o->genus != STML_NODUS_ELEMENTUM)
            {
                perge;
            }
            el   = chorda_ut_cstr(*o->titulus, piscina);
            via  = _attributi(o, "via", piscina);
            si (via == NIHIL)
            {
                perge;
            }
            /* scripta ipsa (radices processuum) bash legit */
            per (k = ZEPHYRUM; k < xar_numerus(radices); k++)
            {
                si (strcmp(*(constans character**)xar_obtinere(radices,
                        k), via) == ZEPHYRUM
                    && !_aequat(el, "scriptura"))
                {
                    tectum = VERUM;
                }
            }
            per (k = ZEPHYRUM; !tectum && k < xar_numerus(statici); k++)
            {
                tectum = _staticus_tegit(piscina,
                    *(StmlNodus**)xar_obtinere(statici, k), el, via);
            }
            si (!tectum && explicata != NIHIL)
            {
                constans character* md = _attributi(o, "mandatum",
                    piscina);

                per (k = ZEPHYRUM; k < xar_numerus(statici); k++)
                {
                    si (_irresolutus_explicat(piscina,
                            *(StmlNodus**)xar_obtinere(statici, k), el,
                            md))
                    {
                        *(StmlNodus**)xar_addere(explicata)  = o;
                        tectum                               = VERUM;
                        frange;
                    }
                }
            }
            si (!tectum)
            {
                *(StmlNodus**)xar_addere(exitus) = o;
            }
        }
    }
    redde exitus;
}


/* ==================================================
 * Catenae verdicti (planum T6, A4): radices ex declarationibus fabricae
 * ================================================== */

/* 'ingressus' genere effectus in arbore (recursive) */
interior vacuum
_ingressus_colligere (
              Piscina* piscina,
            StmlNodus* nodus,
                  Xar* exitus)
{
    i32 k;

    si (nodus == NIHIL)
    {
        redde;
    }
    si (   nodus->genus   == STML_NODUS_ELEMENTUM
        && nodus->titulus != NIHIL
        && chorda_aequalis_literis(*nodus->titulus, "ingressus"))
    {
        chorda* g = stml_attributum_capere(nodus, "genus");
        chorda* v = stml_attributum_capere(nodus, "via");

        si (   g != NIHIL && v != NIHIL
            && chorda_aequalis_literis(*g, "effectus"))
        {
            _nomen_addere(piscina, exitus, chorda_ut_cstr(*v, piscina));
        }
    }
    per (k = ZEPHYRUM; nodus->liberi && k < xar_numerus(nodus->liberi);
         k++)
    {
        _ingressus_colligere(piscina,
            *(StmlNodus**)xar_obtinere(nodus->liberi, k), exitus);
    }
}

Xar*
crusta_effectus_catenae (
               Piscina*  piscina,
    InternamentumChorda* intern,
    constans character*  radix,
    constans character** causa_out)
{
       character  via[VIA_MAXIMA];
          chorda  fons;
    StmlResultus  r;
             Xar* exitus = xar_creare(piscina,
                 (i32)magnitudo(character*));
             i32 k;

    si (causa_out != NIHIL)
    {
        *causa_out = NIHIL;
    }
    si (   exitus               == NIHIL
        || strlen(radix) + LXIV >= (memoriae_index)VIA_MAXIMA)
    {
        redde NIHIL;
    }
    sprintf(via, "%s/fabrica.stml", radix);
    fons = filum_legere_totum(via, piscina);
    si (fons.datum == NIHIL)
    {
        redde exitus;   /* arbor sine fabrica: catenae nullae */
    }
    r = stml_legere(fons, piscina, intern);
    si (!r.successus || r.elementum_radix == NIHIL)
    {
        si (causa_out != NIHIL)
        {
            *causa_out = "fabrica.stml legi non potest";
        }
        redde NIHIL;
    }
    per (k = ZEPHYRUM; r.elementum_radix->liberi != NIHIL
                       && k < xar_numerus(r.elementum_radix->liberi);
         k++)
    {
        StmlNodus* s = *(StmlNodus**)xar_obtinere(
            r.elementum_radix->liberi, k);
            chorda* v;
      StmlResultus  d;

        si (   s->genus != STML_NODUS_ELEMENTUM
            || !chorda_aequalis_literis(*s->titulus, "subsystema"))
        {
            perge;
        }
        v = stml_attributum_capere(s, "via");
        si (   v == NIHIL
            || (memoriae_index)v->mensura + strlen(radix) + XL
                              >= (memoriae_index)VIA_MAXIMA)
        {
            perge;
        }
        sprintf(via, "%s/%.*s/aedificatio.stml", radix,
            (integer)v->mensura, (constans character*)v->datum);
        fons = filum_legere_totum(via, piscina);
        si (fons.datum == NIHIL)
        {
            perge;
        }
        d = stml_legere(fons, piscina, intern);
        si (d.successus)
        {
            _ingressus_colligere(piscina, d.radix, exitus);
        }
    }
    redde exitus;
}

b32
crusta_effectus_plagulam_tenet (
             StmlNodus* summarium,
    constans character* via)
{
    i32 i;
    i32 j;

    per (i = ZEPHYRUM; summarium != NIHIL && summarium->liberi
                       && i < xar_numerus(summarium->liberi); i++)
    {
        StmlNodus* pr = *(StmlNodus**)xar_obtinere(summarium->liberi,
            i);
           chorda* r;

        si (pr->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        si (stml_attributum_capere(pr, "custodia") != NIHIL)
        {
            perge;   /* aedificator custoditus: extra catenam */
        }
        r = stml_attributum_capere(pr, "radix");
        si (r != NIHIL && chorda_aequalis_literis(*r, via))
        {
            redde VERUM;
        }
        per (j = ZEPHYRUM; pr->liberi && j < xar_numerus(pr->liberi);
             j++)
        {
            StmlNodus* s = *(StmlNodus**)xar_obtinere(pr->liberi, j);
               chorda* p;

            si (s->genus != STML_NODUS_ELEMENTUM)
            {
                perge;
            }
            p = stml_attributum_capere(s, "plagula");
            si (p != NIHIL && chorda_aequalis_literis(*p, via))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}
