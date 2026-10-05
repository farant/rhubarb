/* mandata_census.c - census titulorum imperii in scriptis domus
 * (effectus-plan.md T2; semen tabulae crusta/effectus_mandata.stml)
 *
 * Usus:  mandata_census [-radix DIR] < lista
 *        lista = viae scriptorum (una per lineam), radici relativae.
 * Effusio: linea una per titulum, tabulis separata, ordine situum
 *          descendente:  <classis>\t<titulus>\t<situs>\t<plagulae>
 *
 * Transitus DUO. Primus nomina functionum ubivis in corpore definita
 * colligit (sera_capere, mensor_suitae_nunc: functiones domus sunt,
 * non mandata). Secundus titulum omnis imperii classificat:
 *   functio      nomen functionis corporis
 *   aedificium   aedificium aut verbum bash ('echo', '[', 'source')
 *   via          titulus cum '/' (scriptum aut binarium per viam)
 *   dynamicum    titulus non staticus ('"$COMPILATOR"')
 *   externum     cetera - quae tabula mandatorum describere debet
 * Involucra (exec command env nohup) verbum sequens quoque numerant
 * (optiones et VAR=v praetereuntur), ut crusta_fontationes facit.
 * Plagula illegibilis aut parsura non sana in stderr nominatur et
 * praeteritur. Exitus: 0 sanum | 2 usus / memoria. */

#include "postulata_posix.h"

#include "latina.h"
#include "chorda.h"
#include "crusta_arbor.h"
#include "crusta_registrum.h"
#include "filum.h"
#include "materia_nodus.h"
#include "piscina.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define VIA_MAXIMA (IV * MXXIV)

nomen structura {
             character* titulus;
    constans character* classis;
                   i32  situs;
                   i32  plagulae;
                   i32  plagula_ultima;   /* index plagulae ultimae */
} Titulus;

/* aedificia et verba bash 5.2 quae ut titulus imperii stant */
hic_manens constans character* AEDIFICIA[] = {
    ":", ".", "[", "alias", "bg", "bind", "break", "builtin", "caller",
    "cd", "command", "compgen", "complete", "compopt", "continue",
    "declare", "dirs", "disown", "echo", "enable", "eval", "exec",
    "exit", "export", "false", "fc", "fg", "getopts", "hash", "help",
    "history", "jobs", "kill", "let", "local", "logout", "mapfile",
    "popd", "printf", "pushd", "pwd", "read", "readarray", "readonly",
    "return", "set", "shift", "shopt", "source", "suspend", "test",
    "times", "trap", "true", "type", "typeset", "ulimit", "umask",
    "unalias", "unset", "wait", NIHIL
};

hic_manens constans character* INVOLUCRA[] = {
    "exec", "command", "env", "nohup", NIHIL
};

interior b32
_in_serie (
    constans character** series,
    constans character*  titulus)
{
    i32 i;

    per (i = ZEPHYRUM; series[i] != NIHIL; i++)
    {
        si (strcmp(series[i], titulus) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

interior b32
_in_xar (
                   Xar* nomina,
    constans character* titulus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(nomina); i++)
    {
        si (strcmp(*(character**)xar_obtinere(nomina, i), titulus)
            == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* nodi generis dati in arbore (ambulatio iterativa) */
interior Xar*
_nodi_generis (
                  Piscina* piscina,
    constans MateriaNodus* radix,
                      s32  genus)
{
    Xar* acervus  = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));
    Xar* exitus   = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));

    *(constans MateriaNodus**)xar_addere(acervus) = radix;
    dum (xar_numerus(acervus) > ZEPHYRUM)
    {
        constans MateriaNodus* n = *(constans MateriaNodus**)
            xar_obtinere(acervus, xar_numerus(acervus) - I);
                          Xar* liberi;
                          i32  k;

        xar_truncare(acervus, xar_numerus(acervus) - I);
        si (n->genus == genus)
        {
            *(constans MateriaNodus**)xar_addere(exitus) = n;
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
    redde exitus;
}

interior character*
_staticum (
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

interior vacuum
_numerare (
               Piscina* piscina,
                   Xar* tituli,
                   Xar* functiones,
    constans character* titulus,
                   i32  plagula)
{
     Titulus* t = NIHIL;
         i32  i;

    per (i = ZEPHYRUM; i < xar_numerus(tituli); i++)
    {
        Titulus* c = (Titulus*)xar_obtinere(tituli, i);

        si (strcmp(c->titulus, titulus) == ZEPHYRUM)
        {
            t = c;
            frange;
        }
    }
    si (t == NIHIL)
    {
        t = (Titulus*)xar_addere(tituli);
        t->titulus        = chorda_ut_cstr(
            chorda_ex_literis(titulus, piscina), piscina);
        t->situs           = ZEPHYRUM;
        t->plagulae        = ZEPHYRUM;
        t->plagula_ultima  = (i32)-I;
        si (_in_xar(functiones, titulus))
        {
            t->classis = "functio";
        }
        alioquin si (_in_serie(AEDIFICIA, titulus))
        {
            t->classis = "aedificium";
        }
        alioquin si (strchr(titulus, '/') != NIHIL)
        {
            t->classis = "via";
        }
        alioquin
        {
            t->classis = "externum";
        }
    }
    t->situs++;
    si (t->plagula_ultima != plagula)
    {
        t->plagulae++;
        t->plagula_ultima = plagula;
    }
}

/* titulus post involucrum: optiones ('-x') et VAR=v praetereuntur;
 * "" = verbum non staticum, NIHIL = nullum */
interior constans character*
_titulus_involutus (
                  Piscina* piscina,
    constans MateriaNodus* imperium)
{
    Xar* argumenta = crusta_imperium_argumenta(piscina, imperium);
    i32  k;

    per (k = ZEPHYRUM; argumenta && k < xar_numerus(argumenta); k++)
    {
        character* v = _staticum(piscina,
            *(MateriaNodus**)xar_obtinere(argumenta, k));

        si (v == NIHIL)
        {
            redde "";
        }
        si (v[ZEPHYRUM] == '-' || strchr(v, '=') != NIHIL)
        {
            perge;
        }
        redde v;
    }
    redde NIHIL;
}

interior s32
_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans Titulus* x = (constans Titulus*)a;
    constans Titulus* y = (constans Titulus*)b;

    si (x->situs != y->situs)
    {
        redde x->situs > y->situs ? -I : I;
    }
    redde (s32)strcmp(x->titulus, y->titulus);
}

integer
principale (
      integer   argc,
    character** argv)
{
             Piscina* piscina;
                 Xar* viae;
                 Xar* radices;
                 Xar* functiones;
                 Xar* tituli;
           character  radix[VIA_MAXIMA];
           character  linea[VIA_MAXIMA];
           character  via[VIA_MAXIMA * II];
                 i32  dynamica = ZEPHYRUM;
                 i32  k;
                 i32  j;
             integer  i;
             Titulus* ordinati;

    radix[ZEPHYRUM] = '\0';
    per (i = I; i < argc; i++)
    {
        si (   strcmp(argv[i], "-radix") == ZEPHYRUM && i + I < argc
            && strlen(argv[i + I]) < magnitudo(radix))
        {
            strcpy(radix, argv[++i]);
        }
        alioquin
        {
            fprintf(stderr, "usus: mandata_census [-radix DIR] "
                            "< lista\n");
            redde II;
        }
    }
    si (   radix[ZEPHYRUM] == '\0' && getcwd(radix, magnitudo(radix))
        == NIHIL)
    {
        fprintf(stderr, "mandata_census: directorium operis ignotum\n");
        redde II;
    }
    piscina = piscina_generare_dynamicum("mandata_census",
        (memoriae_index)LXIV * M * M);
    si (piscina == NIHIL)
    {
        fprintf(stderr, "mandata_census: memoria deficit\n");
        redde II;
    }
    viae        = xar_creare(piscina, (i32)magnitudo(character*));
    radices     = xar_creare(piscina, (i32)magnitudo(MateriaNodus*));
    functiones  = xar_creare(piscina, (i32)magnitudo(character*));
    tituli      = xar_creare(piscina, (i32)magnitudo(Titulus));

    /* lectio et parsura; transitus primus: functiones */
    dum (fgets(linea, (integer)magnitudo(linea), stdin) != NIHIL)
    {
                 size_t n = strlen(linea);
                 chorda fons;
          CrustaParsura relatio;
          MateriaNodus*  arbor;
                   Xar*  definitiones;

        dum (   n > ZEPHYRUM && (linea[n - I] == '\n'
                              || linea[n - I] == '\r'))
        {
            linea[--n] = '\0';
        }
        si (n == ZEPHYRUM)
        {
            perge;
        }
        sprintf(via, "%s/%s", radix, linea);
        fons = filum_legere_totum(via, piscina);
        si (fons.datum == NIHIL)
        {
            fprintf(stderr, "mandata_census: %s illegibilis\n", linea);
            perge;
        }
        memset(&relatio, ZEPHYRUM, magnitudo(relatio));
        arbor = crusta_arbor_parsare(piscina,
            (constans character*)fons.datum, (i32)fons.mensura,
            &CRUSTA_BASH, &relatio);
        si (arbor == NIHIL || !relatio.sana)
        {
            fprintf(stderr, "mandata_census: %s parsura non sana\n",
                linea);
            perge;
        }
        *(character**)xar_addere(viae) = chorda_ut_cstr(
            chorda_ex_literis(linea, piscina), piscina);
        *(MateriaNodus**)xar_addere(radices) = arbor;
        definitiones = _nodi_generis(piscina, arbor,
            (s32)CRUSTA_GENUS_FUNCTIO);
        per (k = ZEPHYRUM; k < xar_numerus(definitiones); k++)
        {
            constans MateriaNodus* f = *(constans MateriaNodus**)
                xar_obtinere(definitiones, k);
                         character* titulus;

            si (   f->numerus_locorum <= (i32)CRUSTA_FUNCTIO_TOK_TITULUS
                || f->loci[CRUSTA_FUNCTIO_TOK_TITULUS].genus
                   != MATERIA_VALOR_TOKEN)
            {
                perge;
            }
            titulus = chorda_ut_cstr(
                f->loci[CRUSTA_FUNCTIO_TOK_TITULUS].datum.token->valor,
                piscina);
            si (!_in_xar(functiones, titulus))
            {
                *(character**)xar_addere(functiones) = titulus;
            }
        }
    }

    /* transitus secundus: tituli imperiorum */
    per (j = ZEPHYRUM; j < xar_numerus(radices); j++)
    {
        Xar* imperia = _nodi_generis(piscina,
            *(MateriaNodus**)xar_obtinere(radices, j),
            (s32)CRUSTA_GENUS_IMPERIUM);

        per (k = ZEPHYRUM; k < xar_numerus(imperia); k++)
        {
            constans MateriaNodus* imperium = *(constans MateriaNodus**)
                xar_obtinere(imperia, k);
                         character* titulus;

            si (crusta_imperium_titulus(imperium) == NIHIL)
            {
                perge;   /* assignatio sola */
            }
            titulus = _staticum(piscina,
                crusta_imperium_titulus(imperium));
            si (titulus == NIHIL)
            {
                dynamica++;
                perge;
            }
            _numerare(piscina, tituli, functiones, titulus, j);
            si (_in_serie(INVOLUCRA, titulus))
            {
                constans character* involutus =
                    _titulus_involutus(piscina,
                    imperium);

                si (involutus != NIHIL && involutus[ZEPHYRUM] == '\0')
                {
                    dynamica++;
                }
                alioquin si (involutus != NIHIL)
                {
                    _numerare(piscina, tituli, functiones, involutus,
                        j);
                }
            }
        }
    }

    ordinati = (Titulus*)piscina_allocare(piscina,
        (memoriae_index)(xar_numerus(tituli) + I) * magnitudo(Titulus));
    per (k = ZEPHYRUM; k < xar_numerus(tituli); k++)
    {
        ordinati[k] = *(Titulus*)xar_obtinere(tituli, k);
    }
    qsort(ordinati, (size_t)xar_numerus(tituli), magnitudo(Titulus),
        _comparare);
    per (k = ZEPHYRUM; k < xar_numerus(tituli); k++)
    {
        imprimere("%s\t%s\t%u\t%u\n", ordinati[k].classis,
            ordinati[k].titulus, (insignatus integer)ordinati[k].situs,
            (insignatus integer)ordinati[k].plagulae);
    }
    imprimere("dynamicum\t*\t%u\t-\n", (insignatus integer)dynamica);
    fprintf(stderr, "mandata_census: plagulae %u, functiones %u, "
                    "tituli %u\n",
        (insignatus integer)xar_numerus(radices),
        (insignatus integer)xar_numerus(functiones),
        (insignatus integer)xar_numerus(tituli));
    piscina_destruere(piscina);
    redde ZEPHYRUM;
}
