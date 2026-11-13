/* graphis - instrumentum: picturae planae (descriptiones STML) in
 * schedam PNG ad inspiciendum (project-specs/graphis-spec.md III;
 * graphis-plan.md T4). Logica in lib/graphis.c, lib/graphis_lumen.c
 * et lib/graphis_specimen.c; hic argumenta et effusio sola.
 *
 * Usus:
 *
 *   graphis specimen <pictura.stml> -o <via.png> [-fontes DIR]
 *   graphis quadrum <pictura.stml> -o <via.png> -fines "x y lat alt"
 *                   [-fontes DIR]
 *   graphis iudicare <pictura.stml> ... [-fontes DIR]
 *
 * specimen: descriptionem legit, schedam scribit (pictura finita bis
 * aucta; iuxta fons, larva, illustratus, regiones -
 * graphis_specimen.h) et regiones imprimit: numerus, color in
 * tabula regionum, titulus, pixela possessa, capsa circumscripta.
 *
 * quadrum: pars picturae finitae quater aucta.
 *
 * iudicare: plagulam quamque legit; "ACCIPE <via>" aut recusatio.
 *
 * Partes ('usus') ex DIR/<nomen>.stml leguntur (ordinarium
 * graphis/res).
 *
 * Descriptio recusata: "<via>:<linea>: <causa>" in stderr; si
 * recusatio in parte nata est, via partis ("DIR/<nomen>.stml")
 * nominatur. Nihil scribitur.
 *
 * Exitus: 0 bene; 1 refutatum (causa nominata; iudicare: si qua
 * plagula recusata est); 2 usus.
 *
 * Aedificatio: ./tools/graphis_struere.sh -> bin/graphis. */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur */
/* <aedilis obiectum="build/fabrica/provenientia/graphis.c"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "imago.h"
#include "imago_png.h"
#include "graphis.h"
#include "graphis_specimen.h"
#include "provenientia.h"
#include <stdio.h>
#include <string.h>

#define FONTES_ORDINARII  "graphis/res"
#define VIA_MAXIMA        MXXIV

interior s32
usum_imprimere (vacuum)
{
    fprintf(stderr,
        "usus: graphis specimen <pictura.stml> -o <via.png>"
        " [-fontes DIR]\n"
        "      graphis quadrum <pictura.stml> -o <via.png>"
        " -fines \"x y lat alt\" [-fontes DIR]\n"
        "      graphis iudicare <pictura.stml> ... [-fontes DIR]\n");
    redde II;
}

/* via partis: DIR/<nomen>.stml; FALSUM si nimis longa */
interior b32
viam_partis (
    constans character* fontes,
                chorda  titulus,
             character* via)
{
    si (strlen(fontes) + titulus.mensura + XVI > VIA_MAXIMA)
    {
        redde FALSUM;
    }
    (vacuum)sprintf(via, "%s/%.*s.stml", fontes,
        (integer)titulus.mensura, (constans character*)titulus.datum);
    redde VERUM;
}

/* GraphisFons: partes ex directorio (contextus = via eius) */
interior chorda
fons_partium (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    character via[VIA_MAXIMA];
       chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (   !viam_partis((constans character*)contextus, titulus, via)
        || !filum_existit(via))
    {
        redde vacua;
    }
    redde filum_legere_totum(via, piscina);
}

/* descriptionem legere et componere; recusatio in stderr */
interior b32
picturam_legere (
    constans character* via,
    constans character* fontes,
        GraphisFructus* f,
               Piscina* piscina)
{
    character via_partis[VIA_MAXIMA];
       chorda textus;

    si (!filum_existit(via))
    {
        fprintf(stderr, "graphis: plagula '%s' legi non potest\n",
            via);
        redde FALSUM;
    }
    textus = filum_legere_totum(via, piscina);
    si (textus.mensura == ZEPHYRUM)
    {
        fprintf(stderr, "graphis: plagula '%s' legi non potest\n",
            via);
        redde FALSUM;
    }
    /* contextus fontis: via directorii (non mutatur) */
    *f = graphis_legere_cum_fonte(textus, fons_partium,
        (vacuum*)(character*)(size_t)fontes, piscina);
    si (f->successus)
    {
        redde VERUM;
    }
    si (   f->plagula.mensura > ZEPHYRUM
        && viam_partis(fontes, f->plagula, via_partis))
    {
        via = via_partis;
    }
    fprintf(stderr, "%s:%u: %.*s\n", via, (insignatus)f->linea,
        (integer)f->error.mensura,
        (constans character*)f->error.datum);
    redde FALSUM;
}

interior b32
png_scribere (
        constans Imago* imago,
    constans character* exitus,
               Piscina* piscina)
{
    PngFructus png;

    si (!imago->pixela)
    {
        fprintf(stderr, "graphis: imago pingi non potuit\n");
        redde FALSUM;
    }
    png = imago_png_scribere(imago, exitus, piscina);
    si (!png.successus)
    {
        fprintf(stderr, "graphis: PNG scribi non potest: %s\n",
            exitus);
        redde FALSUM;
    }
    imprimere("%s: %u x %u\n", exitus, (insignatus)imago->latitudo,
        (insignatus)imago->altitudo);
    redde VERUM;
}

interior vacuum
regiones_imprimere (
    constans GraphisFructus* f)
{
    GraphisRegio regiones[GRAPHIS_TITULI_MAXIMI];
             i32 numerus = graphis_regiones(f, regiones);
             i32 k;

    imprimere("regiones: %u\n", (insignatus)numerus);
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        constans GraphisRegio* r = &regiones[k];

        imprimere("  %3u  %-7s %-24.*s %6u px  (%d,%d)-(%d,%d)\n",
            (insignatus)(k + I), graphis_specimen_color_titulus(k + I),
            (integer)r->titulus.mensura,
            (constans character*)r->titulus.datum,
            (insignatus)r->pixela, (integer)r->x_primus,
            (integer)r->y_primus, (integer)r->x_ultimus,
            (integer)r->y_ultimus);
    }
}

interior s32
exsequi_specimen (
    constans character* via,
    constans character* exitus,
    constans character* fontes,
               Piscina* piscina)
{
    GraphisFructus f;
             Imago imago;

    si (!picturam_legere(via, fontes, &f, piscina))
    {
        redde I;
    }
    imago = graphis_specimen_pingere(&f, piscina);
    si (!png_scribere(&imago, exitus, piscina))
    {
        redde I;
    }
    regiones_imprimere(&f);
    redde ZEPHYRUM;
}

interior s32
exsequi_quadrum (
    constans character* via,
    constans character* exitus,
    constans character* fontes,
          constans s32* fines,
               Piscina* piscina)
{
    GraphisFructus f;
             Imago imago;

    si (!picturam_legere(via, fontes, &f, piscina))
    {
        redde I;
    }
    imago = graphis_quadrum_pingere(&f, fines[ZEPHYRUM], fines[I],
        fines[II], fines[III], piscina);
    si (!imago.pixela)
    {
        fprintf(stderr,
            "graphis: -fines \"%d %d %d %d\" non intra picturam"
            " %u x %u iacent\n", (integer)fines[ZEPHYRUM],
            (integer)fines[I], (integer)fines[II],
            (integer)fines[III], (insignatus)GRAPHIS_LATITUDO,
            (insignatus)GRAPHIS_ALTITUDO);
        redde I;
    }
    redde png_scribere(&imago, exitus, piscina) ? ZEPHYRUM : I;
}

/* "x y lat alt": quattuor integri, nihil aliud */
interior b32
fines_legere (
    constans character* textus,
                   s32* fines)
{
      integer a;
      integer b;
      integer c;
      integer d;
    character cauda;

    si (sscanf(textus, "%d %d %d %d %c", &a, &b, &c, &d, &cauda)
        != IV)
    {
        redde FALSUM;
    }
    fines[ZEPHYRUM]  = (s32)a;
    fines[I]         = (s32)b;
    fines[II]        = (s32)c;
    fines[III]       = (s32)d;
    redde VERUM;
}

externus constans ProvenientiaRelatio provenientia_graphis;

s32
principale (
      integer   argc,
    character** argv)
{
    constans character* exitus        = NIHIL;
    constans character* fontes        = NIHIL;
    constans character* fines_textus  = NIHIL;
               Piscina* piscina;
               integer  k;
               integer  plagulae    = ZEPHYRUM;
                   b32  specimen;
                   b32  quadrum;
                   s32  fines[IV];
                   s32  r           = ZEPHYRUM;

    si (provenientia_respondere((s32)argc, argv,
        &provenientia_graphis))
    {
        redde ZEPHYRUM;
    }
    si (argc < II)
    {
        redde usum_imprimere();
    }
    specimen  = strcmp(argv[I], "specimen") == ZEPHYRUM;
    quadrum   = strcmp(argv[I], "quadrum") == ZEPHYRUM;
    si (   !specimen && !quadrum
        && strcmp(argv[I], "iudicare") != ZEPHYRUM)
    {
        redde usum_imprimere();
    }
    /* vexilla; plagulae in argv compactantur ab indice II */
    per (k = II; k < argc; k++)
    {
        constans character** scopus = NIHIL;

        si (strcmp(argv[k], "-o") == ZEPHYRUM)
        {
            scopus = &exitus;
        }
        alioquin si (strcmp(argv[k], "-fontes") == ZEPHYRUM)
        {
            scopus = &fontes;
        }
        alioquin si (strcmp(argv[k], "-fines") == ZEPHYRUM)
        {
            scopus = &fines_textus;
        }
        si (scopus)
        {
            si (k + I >= argc || *scopus)
            {
                redde usum_imprimere();
            }
            *scopus = argv[++k];
            perge;
        }
        si (argv[k][ZEPHYRUM] == '-')
        {
            redde usum_imprimere();
        }
        argv[II + plagulae++] = argv[k];
    }
    si (   plagulae < I || ((specimen || quadrum) && plagulae != I)
        || ((specimen || quadrum) != (exitus != NIHIL))
        || (quadrum != (fines_textus != NIHIL))
        || (quadrum && !fines_legere(fines_textus, fines)))
    {
        redde usum_imprimere();
    }
    si (!fontes)
    {
        fontes = FONTES_ORDINARII;
    }
    piscina = piscina_generare_dynamicum("graphis", M * M);
    si (specimen)
    {
        r = exsequi_specimen(argv[II], exitus, fontes, piscina);
    }
    alioquin si (quadrum)
    {
        r = exsequi_quadrum(argv[II], exitus, fontes, fines, piscina);
    }
    alioquin
    {
        per (k = ZEPHYRUM; k < plagulae; k++)
        {
            GraphisFructus f;

            si (picturam_legere(argv[II + k], fontes, &f, piscina))
            {
                imprimere("ACCIPE %s\n", argv[II + k]);
            }
            alioquin
            {
                r = I;
            }
        }
    }
    piscina_destruere(piscina);
    redde r;
}
