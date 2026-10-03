/* crusta_fontationes.c - fontationes scripti statice derivatae.
 * Vide crusta_fontationes.h (regulae, genera, limites). */

#include "latina.h"
#include "crusta_fontationes.h"
#include "crusta_arbor.h"
#include "crusta_registrum.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "filum.h"
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
                      Xar* imperia;       /* MateriaNodus* */
                      Xar* assignationes; /* MateriaNodus* */
                      Xar* iterationes;   /* MateriaNodus* */
} Plagula;

nomen structura {
                character* titulus;
    constans MateriaNodus* verbum;   /* NIHIL et !vacuum = ignotum */
                      b32  inanis;     /* 'X=' : chorda vacua */
    constans MateriaNodus* functio;  /* NIHIL = globalis */
                  Plagula* plagula;
} Definitio;

nomen structura {
    constans MateriaNodus* functio;
                character* titulus;
} Localis;

nomen structura {
    character* radix_via;   /* $0 ambitus */
          Xar* plagulae;    /* Plagula* */
          Xar* definitiones;
          Xar* locales;
          Xar* exitus;      /* CrustaFontatio - iterationis currentis */
} Ambitus;

nomen structura {
               Piscina* piscina;
    constans character* radix;
                   i32  radix_mensura;
                   Xar* exitus;     /* CrustaFontatio, omnes ambitus */
                   Xar* visi;       /* character*: radices ambituum */
} Derivatio;


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
                    (vacuum)_appendere(area, longitudo,
                        (constans character*)e->datum.token->valor.datum,
                        (i32)e->datum.token->valor.mensura);
                }
                alioquin si (e->genus == MATERIA_VALOR_NODUS)
                {
                    _textum_colligere(e->datum.nodus, area, longitudo);
                }
            }
        }
    }
}

interior i32
_linea_prima (
    constans MateriaNodus* nodus)
{
    i32 k;
    i32 j;
    i32 l;

    si (nodus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    per (k = ZEPHYRUM; k < nodus->numerus_locorum; k++)
    {
        constans MateriaValor* v = &nodus->loci[k];

        si (v->genus == MATERIA_VALOR_TOKEN)
        {
            redde v->datum.token->linea;
        }
        si (v->genus == MATERIA_VALOR_NODUS)
        {
            l = _linea_prima(v->datum.nodus);
            si (l != ZEPHYRUM)
            {
                redde l;
            }
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            per (j = ZEPHYRUM; j < materia_valor_lista_numerus(*v); j++)
            {
                MateriaValor* e = materia_valor_lista_obtinere(*v, j);

                si (e->genus == MATERIA_VALOR_TOKEN)
                {
                    redde e->datum.token->linea;
                }
                si (e->genus == MATERIA_VALOR_NODUS)
                {
                    l = _linea_prima(e->datum.nodus);
                    si (l != ZEPHYRUM)
                    {
                        redde l;
                    }
                }
            }
        }
    }
    redde ZEPHYRUM;
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
        redde FALSUM;
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
            bonus = FALSUM;
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
            redde FALSUM;   /* definitiones discordes aut ignotae */
        }
        numerus++;
    }
    si (numerus == ZEPHYRUM)
    {
        redde FALSUM;       /* ambitus externus: ignotum */
    }
    si (!_appendere(area, longitudo, primus, primus_longitudo))
    {
        redde FALSUM;
    }
    redde plenus;
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
                    redde FALSUM;   /* ${X:-y}, ${#X}, ${!X} ... */
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
                    redde FALSUM;   /* subscripta alia */
                }
                si (!_variabilem_aestimare(d, a, p, pars, titulus, area,
                        longitudo, profunditas, FALSUM))
                {
                    redde FALSUM;
                }
                frange;
            }
            casus CRUSTA_GENUS_PARS_SUBSTITUTIO:
                si (!_substitutionem_aestimare(d, a, p, pars, area,
                        longitudo, profunditas))
                {
                    redde FALSUM;
                }
                frange;
            ordinarius:
                redde FALSUM;   /* domus, effugia, arithmetica, ... */
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
    Xar* acervus = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));

    p->imperia = xar_creare(piscina,
        (i32)magnitudo(MateriaNodus*));
    p->assignationes = xar_creare(piscina,
        (i32)magnitudo(MateriaNodus*));
    p->iterationes = xar_creare(piscina,
        (i32)magnitudo(MateriaNodus*));
    *(constans MateriaNodus**)xar_addere(acervus) = p->radix;
    dum (xar_numerus(acervus) > ZEPHYRUM)
    {
        constans MateriaNodus* n =
            *(constans MateriaNodus**)xar_obtinere(
            acervus, xar_numerus(acervus) - I);
                          Xar* liberi;
                          i32  k;

        xar_truncare(acervus, xar_numerus(acervus) - I);
        si (n->genus == (s32)CRUSTA_GENUS_IMPERIUM)
        {
            *(constans MateriaNodus**)xar_addere(p->imperia) = n;
        }
        alioquin si (n->genus == (s32)CRUSTA_GENUS_ASSIGNATIO)
        {
            *(constans MateriaNodus**)xar_addere(p->assignationes) = n;
        }
        alioquin si (n->genus == (s32)CRUSTA_GENUS_ITERATIO)
        {
            *(constans MateriaNodus**)xar_addere(p->iterationes) = n;
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
                      b32  declaratio_localis)
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
    def           = (Definitio*)xar_addere(a->definitiones);
    def->titulus  = _duplicare(d->piscina, titulus);
    def->verbum   = verbum;
    def->inanis   = inanis;
    def->functio  = functio;
    def->plagula  = p;
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
        constans MateriaToken* tt = _token(as,
                                        (i32)CRUSTA_ASSIGNATIO_TOK_TITULUS);
        constans MateriaToken* op = _token(as,
                                        (i32)CRUSTA_ASSIGNATIO_TOK_OPERATOR);
        constans MateriaNodus* valor = _nodus(as,
                                        (i32)CRUSTA_ASSIGNATIO_VALOR);
                    character* t = NIHIL;
                          b32  ignotum;

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
        _definitionem_addere(a, d, p, chorda_ut_cstr(tt->valor,
            d->piscina),
            ignotum ? NIHIL : valor, !ignotum && valor == NIHIL, as,
            _localis_verbum(t));
    }
    per (k = ZEPHYRUM; k < xar_numerus(p->iterationes); k++)
    {
        constans MateriaNodus* it =
            *(constans MateriaNodus**)xar_obtinere(
                                        p->iterationes, k);
        constans MateriaToken* tt = _token(it,
                                        (i32)CRUSTA_ITERATIO_TOK_TITULUS);

        si (tt != NIHIL)
        {
            _definitionem_addere(a, d, p,
                chorda_ut_cstr(tt->valor, d->piscina), NIHIL, FALSUM,
                it,
                FALSUM);
        }
    }
}


/* ==================================================
 * Loca: fontationes et exsecutiones
 * ================================================== */

interior vacuum
_exitum_addere (
                Ambitus* a,
              Derivatio* d,
    CrustaFontatioGenus  genus,
     constans character* via,
                Plagula* p,
                    i32  linea,
                    b32  fontatum)
{
        CrustaFontatio* f    =
            (CrustaFontatio*)xar_addere(a->exitus);
    constans character* rel  = _relativa(d, p->via);

    f->genus     = genus;
    f->via       = _duplicare(d->piscina, via);
    f->plagula   = _duplicare(d->piscina, rel != NIHIL ? rel : p->via);
    f->linea     = linea;
    f->fontatum  = fontatum;
}

/* cwd loci: ultimum 'cd W' plenum in catena eadem, aliter radix */
interior constans character*
_directorium_loci (
              Derivatio* d,
                Ambitus* a,
                Plagula* p,
    constans MateriaNodus* imperium)
{
    constans MateriaNodus* catena = imperium->pater;
                      Xar* membra;
                character* exitus = NIHIL;
                      i32  k;

    si (catena == NIHIL || catena->genus != (s32)CRUSTA_GENUS_CATENA)
    {
        redde d->radix;
    }
    membra = _nodi_listae(d->piscina, catena,
        (i32)CRUSTA_CATENA_LIBERI);
    per (k = ZEPHYRUM; k < xar_numerus(membra); k++)
    {
        constans MateriaNodus* m = *(MateriaNodus**)xar_obtinere(membra,
            k);
                    character* t;
                          Xar* argumenta;
                    character  valor[VIA_MAXIMA];
                          i32  longitudo = ZEPHYRUM;
                    character  absoluta[VIA_MAXIMA];

        si (m == imperium)
        {
            frange;
        }
        si (m->genus != (s32)CRUSTA_GENUS_IMPERIUM)
        {
            perge;
        }
        t = _titulus_staticus(d->piscina, crusta_imperium_titulus(m));
        argumenta = crusta_imperium_argumenta(d->piscina, m);
        si (   t == NIHIL || strcmp(t, "cd") != ZEPHYRUM
            || argumenta == NIHIL
            || xar_numerus(argumenta) != I)
        {
            perge;
        }
        valor[ZEPHYRUM] = '\0';
        si (   _verbum_aestimare(d, a, p,
                   *(MateriaNodus**)xar_obtinere(argumenta, ZEPHYRUM),
                   valor, &longitudo, ZEPHYRUM)
            && _absolutam_facere(valor, d->radix, absoluta))
        {
            exitus = _duplicare(d->piscina, absoluta);
        }
        alioquin
        {
            redde NIHIL;   /* cd ignotum: cwd ignotum */
        }
    }
    redde exitus != NIHIL ? exitus : d->radix;
}

interior b32
_ambitum_derivare (
             Derivatio* d,
    constans character* radix);

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

/* verbum in sede fontationis/exsecutionis */
interior b32
_locum_tractare (
              Derivatio* d,
                Ambitus* a,
                Plagula* p,
    constans MateriaNodus* imperium,
    constans MateriaNodus* verbum,
                    b32  fontatum)
{
             character  valor[VIA_MAXIMA];
                   i32  longitudo = ZEPHYRUM;
             character  absoluta[VIA_MAXIMA];
                   b32  plenus;
                   i32  linea = _linea_prima(verbum);
    constans character* cwd;
    constans character* rel;

    valor[ZEPHYRUM] = '\0';
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
    cwd = _directorium_loci(d, a, p, imperium);
    si (!plenus)
    {
        /* praefixum notum sub build/ = productum ('$BUILD_DIR/$nomen') */
        si (   longitudo > ZEPHYRUM && cwd != NIHIL
            && _absolutam_facere(valor, cwd, absoluta))
        {
            si (valor[longitudo - I] != '/')
            {
                _directorium_viae(absoluta);
            }
            rel = _relativa(d, absoluta);
            si (   rel != NIHIL && (_sub_build(rel)
                    || strcmp(rel, "build") == ZEPHYRUM
                    || (strlen(rel) > VI
                        && strcmp(rel + strlen(rel) - VI, "/build")
                            == ZEPHYRUM)))
            {
                character productum[VIA_MAXIMA];
                      i32 n = ZEPHYRUM;

                productum[ZEPHYRUM] = '\0';
                (vacuum)_appendere(productum, &n, rel,
                    (i32)strlen(rel));
                (vacuum)_appendere(productum, &n, "/*", II);
                _exitum_addere(a, d, CRUSTA_FONTATIO_PRODUCTUM,
                    productum, p,
                    linea, fontatum);
                redde VERUM;
            }
        }
        longitudo        = ZEPHYRUM;
        valor[ZEPHYRUM]  = '\0';
        _textum_colligere(verbum, valor, &longitudo);
        _exitum_addere(a, d, CRUSTA_FONTATIO_IRRESOLUTUM, valor, p,
            linea,
            fontatum);
        redde VERUM;
    }
    si (cwd == NIHIL || !_absolutam_facere(valor, cwd, absoluta))
    {
        _exitum_addere(a, d, CRUSTA_FONTATIO_IRRESOLUTUM, valor, p,
            linea,
            fontatum);
        redde VERUM;
    }
    rel = _relativa(d, absoluta);
    si (rel == NIHIL)
    {
        si (fontatum)
        {
            _exitum_addere(a, d, CRUSTA_FONTATIO_EXTERNUM, absoluta, p,
                linea, fontatum);
        }
        redde VERUM;   /* exsecutum externum: identitas alibi */
    }
    si (_sub_build(rel))
    {
        _exitum_addere(a, d, CRUSTA_FONTATIO_PRODUCTUM, rel, p, linea,
            fontatum);
        redde VERUM;
    }
    si (!filum_existit(absoluta))
    {
        si (!fontatum && strncmp(rel, "bin/", IV) == ZEPHYRUM)
        {
            _exitum_addere(a, d, CRUSTA_FONTATIO_INSTRUMENTUM, rel, p,
                linea, fontatum);
        }
        alioquin
        {
            character causa[VIA_MAXIMA];
                  i32 n = ZEPHYRUM;

            causa[ZEPHYRUM] = '\0';
            (vacuum)_appendere(causa, &n, "absens: ", VIII);
            (vacuum)_appendere(causa, &n, rel, (i32)strlen(rel));
            _exitum_addere(a, d, CRUSTA_FONTATIO_IRRESOLUTUM, causa, p,
                linea, fontatum);
        }
        redde VERUM;
    }
    si (fontatum)
    {
        _exitum_addere(a, d, CRUSTA_FONTATIO_FASCICULUS, rel, p, linea,
            fontatum);
        si (!_plagulam_in_ambitu(a, absoluta))
        {
            Plagula* nova = _plagulam_parare(d, absoluta);

            si (nova == NIHIL)
            {
                redde FALSUM;
            }
            *(Plagula**)xar_addere(a->plagulae) = nova;
        }
        redde VERUM;
    }
    si (!_scriptum_est(d, absoluta))
    {
        _exitum_addere(a, d, CRUSTA_FONTATIO_INSTRUMENTUM, rel, p,
            linea,
            fontatum);
        redde VERUM;
    }
    _exitum_addere(a, d, CRUSTA_FONTATIO_FASCICULUS, rel, p, linea,
        fontatum);
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
                      i32  k;

    si (ab >= xar_numerus(verba))
    {
        redde VERUM;
    }
    titulus  = *(MateriaNodus**)xar_obtinere(verba, ab);
    t        = _titulus_staticus(d->piscina, titulus);
    si (t != NIHIL && strchr(t, '/') == NIHIL)
    {
        si (   strcmp(t, "source") == ZEPHYRUM
            || strcmp(t, ".")      == ZEPHYRUM)
        {
            si (ab + I < xar_numerus(verba))
            {
                redde _locum_tractare(d, a, p, imperium,
                    *(MateriaNodus**)xar_obtinere(verba, ab + I),
                    VERUM);
            }
            redde VERUM;
        }
        si (   strcmp(t, "bash") == ZEPHYRUM
            || strcmp(t, "sh")   == ZEPHYRUM)
        {
            per (k = ab + I; k < xar_numerus(verba); k++)
            {
                constans MateriaNodus* v =
                    *(MateriaNodus**)xar_obtinere(
                    verba, k);
                           character* s = _titulus_staticus(d->piscina,
                               v);

                si (s != NIHIL && s[ZEPHYRUM] == '-')
                {
                    si (strcmp(s, "-c") == ZEPHYRUM)
                    {
                        redde VERUM;   /* textus, non plagula */
                    }
                    perge;
                }
                redde _locum_tractare(d, a, p, imperium, v, FALSUM);
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
        redde VERUM;   /* PATH, functio, aedificium */
    }
    redde _locum_tractare(d, a, p, imperium, titulus, FALSUM);
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
 * Ambitus: punctum fixum
 * ================================================== */

/* ambitum (processum) scripti 'radix' derivare: plagulae fontatae
 * definitiones communicant, ergo iteratur dum plagula nova advenit -
 * exitus iterationis ULTIMAE soli valent */
interior b32
_ambitum_derivare (
              Derivatio* d,
     constans character* radix)
{
    Ambitus* a;
    Plagula* prima;
        i32  k;
        i32  numerus;
        i32  j;

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
    a->radix_via = _duplicare(d->piscina, radix);
    a->plagulae = xar_creare(d->piscina, (i32)magnitudo(Plagula*));
    *(Plagula**)xar_addere(a->plagulae) = prima;
    fac
    {
        numerus = xar_numerus(a->plagulae);
        a->definitiones = xar_creare(d->piscina,
            (i32)magnitudo(Definitio));
        a->locales = xar_creare(d->piscina,
            (i32)magnitudo(Localis));
        a->exitus       = xar_creare(d->piscina,
                              (i32)magnitudo(CrustaFontatio));
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            Plagula* p = *(Plagula**)xar_obtinere(a->plagulae, k);

            si (p->radix != NIHIL)
            {
                _locales_colligere(d, a, p);
            }
        }
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            Plagula* p = *(Plagula**)xar_obtinere(a->plagulae, k);

            si (p->radix != NIHIL)
            {
                _definitiones_colligere(d, a, p);
            }
        }
        per (k = ZEPHYRUM; k < numerus; k++)
        {
            Plagula* p = *(Plagula**)xar_obtinere(a->plagulae, k);

            si (p->radix == NIHIL)
            {
                _exitum_addere(a, d, CRUSTA_FONTATIO_IRRESOLUTUM,
                    p->causa,
                    p, ZEPHYRUM, FALSUM);
                perge;
            }
            si (!_loca_tractare(d, a, p))
            {
                redde FALSUM;
            }
        }
    }
    dum (xar_numerus(a->plagulae) != numerus);
    /* radix ambitus ipsa: fasciculus */
    {
        constans character* rel = _relativa(d, radix);

        si (rel != NIHIL)
        {
            _exitum_addere(a, d, CRUSTA_FONTATIO_FASCICULUS, rel, prima,
                ZEPHYRUM, FALSUM);
        }
    }
    per (j = ZEPHYRUM; j < xar_numerus(a->exitus); j++)
    {
        *(CrustaFontatio*)xar_addere(d->exitus) =
            *(CrustaFontatio*)xar_obtinere(a->exitus, j);
    }
    redde VERUM;
}


/* ==================================================
 * Interfacies
 * ================================================== */

interior s32
_fontationes_comparare (
    constans vacuum* x,
    constans vacuum* y)
{
    constans CrustaFontatio* a = (constans CrustaFontatio*)x;
    constans CrustaFontatio* b = (constans CrustaFontatio*)y;

    s32 c;

    si (a->genus != b->genus)
    {
        redde a->genus < b->genus ? -I : I;
    }
    c = (s32)strcmp(a->via, b->via);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    /* unicum servat primum: locus verus (linea > 0) ante radicem */
    si ((a->linea == ZEPHYRUM) != (b->linea == ZEPHYRUM))
    {
        redde a->linea == ZEPHYRUM ? I : -I;
    }
    redde ZEPHYRUM;
}

constans character*
crusta_fontatio_genus_titulus (
    CrustaFontatioGenus genus)
{
    commutatio (genus)
    {
        casus CRUSTA_FONTATIO_FASCICULUS:   redde "fasciculus";
        casus CRUSTA_FONTATIO_INSTRUMENTUM: redde "instrumentum";
        casus CRUSTA_FONTATIO_PRODUCTUM:    redde "productum";
        casus CRUSTA_FONTATIO_EXTERNUM:     redde "externum";
        casus CRUSTA_FONTATIO_IRRESOLUTUM:  redde "irresolutum";
    }
    redde "?";
}

Xar*
crusta_fontationes_derivare (
               Piscina* piscina,
    constans character* radix,
    constans character* scriptum,
    constans character** causa_out)
{
    Derivatio  d;
    character  absoluta[VIA_MAXIMA];
          Xar* unica;
          i32  k;

    si (causa_out != NIHIL)
    {
        *causa_out = NIHIL;
    }
    d.piscina        = piscina;
    d.radix          = radix;
    d.radix_mensura  = (i32)strlen(radix);
    d.exitus = xar_creare(piscina,
        (i32)magnitudo(CrustaFontatio));
    d.visi = xar_creare(piscina, (i32)magnitudo(character*));
    si (   d.exitus == NIHIL || d.visi == NIHIL
        || !_absolutam_facere(scriptum, radix, absoluta))
    {
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
    xar_ordinare(d.exitus, _fontationes_comparare);
    unica = xar_creare(piscina, (i32)magnitudo(CrustaFontatio));
    si (unica == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; k < xar_numerus(d.exitus); k++)
    {
        CrustaFontatio* f = (CrustaFontatio*)xar_obtinere(d.exitus, k);

        si (xar_numerus(unica) > ZEPHYRUM)
        {
            CrustaFontatio* u = (CrustaFontatio*)xar_obtinere(unica,
                xar_numerus(unica) - I);

            si (   u->genus               == f->genus
                && strcmp(u->via, f->via) == ZEPHYRUM)
            {
                perge;
            }
        }
        *(CrustaFontatio*)xar_addere(unica) = *f;
    }
    redde unica;
}
