/* scaena - instrumentum: plagulas scaenarum (apps/scaena/loci) in
 * prospectus componit et in volumen vici condit
 * (project-specs/graphis-spec.md 23a; graphis-plan.md T7). Logica in
 * lib/scaena.c et lib/scaena_repositorium.c; hic argumenta, plagulae
 * et effusio sola.
 *
 * Usus:
 *
 *   scaena condere -volumen V [-fontes DIR] [-partes DIR]
 *   scaena removere -volumen V TITULUS
 *   scaena index -volumen V
 *
 * condere: omnem plagulam *.scaena.stml in DIR (ordinarium
 * apps/scaena/loci), ordine nominum, componit; picturae ex DIR
 * eodem (DIR/<nomen>.stml), partes ex -partes (ordinarium
 * graphis/res). Si ulla plagula recusatur, NIHIL scribitur (volumen
 * ne aperitur quidem). Aliter, in transactione una, linea una per
 * prospectum:
 *
 *     addita    TITULUS    prospectus novus in volumine
 *     mutata    TITULUS    massa substituta
 *     eadem     TITULUS    octeti iidem: nihil scriptum
 *
 * deinde prospectus voluminis qui fontem non habent ("sine fonte:
 * TITULUS") et summa. Numquam removet. Titulus idem in duabus
 * plagulis recusatur.
 *
 * removere: prospectum ex indice tollit; titulus qui abest
 * refutatur.
 *
 * index: prospectus voluminis ordine additionis - titulus, numerus
 * rerum, res quaeque cum exitu suo.
 *
 * -volumen SEMPER nominandum: ordinarium nullum est.
 *
 * Recusatio: "<via>:<linea>: <causa>" in stderr; si in pictura aut
 * parte nata est, via eius nominatur.
 *
 * Exitus: 0 bene; 1 refutatum (causa nominata); 2 usus.
 *
 * Aedificatio: ./tools/scaena_struere.sh -> bin/scaena. */
/* plagula provenientiae (fabrica T7): '-provenientia' respondetur */
/* <aedilis obiectum="build/fabrica/provenientia/scaena.c"/> */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "filum.h"
#include "xar.h"
#include "iter_directoria.h"
#include "volumen.h"
#include "graphis.h"
#include "scaena.h"
#include "scaena_repositorium.h"
#include "provenientia.h"
#include <stdio.h>
#include <string.h>

#define FONTES_ORDINARII   "apps/scaena/loci"
#define PARTES_ORDINARIAE  "graphis/res"
#define SUFFIXUM           ".scaena.stml"
#define SUFFIXI_MENSURA    XII
#define VIA_MAXIMA         MXXIV
#define PLAGULAE_MAXIMAE   LXIV
#define PARATI_MAXIMI      MXXIV

/* contextus fontium: directoria, et pictura novissime petita (ut
 * recusatio in pictura a recusatione in parte discernatur) */
nomen structura {
    constans character* fontes;
    constans character* partes;
                chorda  pictura;
} Fontes;

/* prospectus compositus, ad condendum paratus */
nomen structura {
    chorda titulus;
    chorda massa;
} Paratus;

interior s32
usum_imprimere (vacuum)
{
    fprintf(stderr,
        "usus: scaena condere -volumen V [-fontes DIR]"
        " [-partes DIR]\n"
        "      scaena removere -volumen V TITULUS\n"
        "      scaena index -volumen V\n");
    redde II;
}

/* DIR/<nomen>.stml; FALSUM si nimis longa */
interior b32
viam_plagulae (
    constans character* directorium,
                chorda  titulus,
             character* via)
{
    si (strlen(directorium) + titulus.mensura + XVI > VIA_MAXIMA)
    {
        redde FALSUM;
    }
    (vacuum)sprintf(via, "%s/%.*s.stml", directorium,
        (integer)titulus.mensura, (constans character*)titulus.datum);
    redde VERUM;
}

interior chorda
plagulam_legere (
    constans character* directorium,
                chorda  titulus,
               Piscina* piscina)
{
    character via[VIA_MAXIMA];
       chorda vacua;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    si (   !viam_plagulae(directorium, titulus, via)
        || !filum_existit(via))
    {
        redde vacua;
    }
    redde filum_legere_totum(via, piscina);
}

/* GraphisFons: picturae ex directorio fontium */
interior chorda
fons_picturarum (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    Fontes* f = (Fontes*)contextus;

    f->pictura = chorda_transcribere(titulus, piscina);
    redde plagulam_legere(f->fontes, titulus, piscina);
}

/* GraphisFons: partes ex directorio partium */
interior chorda
fons_partium (
     chorda  titulus,
     vacuum* contextus,
    Piscina* piscina)
{
    redde plagulam_legere(((Fontes*)contextus)->partes, titulus,
        piscina);
}

interior b32
est_plagula_scaenae (
    chorda titulus)
{
    redde titulus.mensura > SUFFIXI_MENSURA
        && memcmp(titulus.datum + titulus.mensura - SUFFIXI_MENSURA,
            SUFFIXUM, SUFFIXI_MENSURA) == ZEPHYRUM;
}

/* nomina plagularum *.scaena.stml directorii, ordine octetorum; -1
 * si directorium aperiri non potest aut plagulae nimis multae */
interior s32
plagulas_colligere (
    constans character* fontes,
                chorda* nomina,
               Piscina* piscina)
{
     DirectoriumIterator* iter =
         directorium_iterator_aperire(fontes, piscina);
    DirectoriumIntroitus* introitus;
                     s32  numerus = ZEPHYRUM;
                     s32  k;
                     s32  j;

    si (!iter)
    {
        redde -I;
    }
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        si (   introitus->genus != INTROITUS_FILUM
            || !est_plagula_scaenae(introitus->titulus))
        {
            perge;
        }
        si (numerus >= PLAGULAE_MAXIMAE)
        {
            directorium_iterator_claudere(iter);
            redde -I;
        }
        nomina[numerus++] =
            chorda_transcribere(introitus->titulus, piscina);
    }
    directorium_iterator_claudere(iter);
    /* ordo nominum: insertio (plagulae paucae) */
    per (k = I; k < numerus; k++)
    {
        chorda hoc = nomina[k];

        per (j = k; j > ZEPHYRUM
            && chorda_comparare(nomina[j - I], hoc) > ZEPHYRUM; j--)
        {
            nomina[j] = nomina[j - I];
        }
        nomina[j] = hoc;
    }
    redde numerus;
}

interior vacuum
recusatam_nuntiare (
    constans character* via,
    constans character* causa,
                chorda  additum)
{
    fprintf(stderr, "%s: recusata: %s%s%.*s\n", via, causa,
        additum.mensura ? ": " : "", (integer)additum.mensura,
        (constans character*)additum.datum);
}

/* plagulam unam componere; prospectus eius in paratos addere.
 * FALSUM = recusata (causa iam nuntiata) */
interior b32
plagulam_parare (
    constans character* via,
                Fontes* fontes,
               Paratus* parati,
                   s32* paratorum,
               Piscina* piscina)
{
       ScaenaFructus f;
    ScaenaProspectus relectus;
           character via_nuntii[VIA_MAXIMA];
              chorda vacua;
              chorda textus;
              chorda massa;
                 i32 k;
                 s32 j;

    vacua.datum      = NIHIL;
    vacua.mensura    = ZEPHYRUM;
    fontes->pictura  = vacua;
    textus           = filum_legere_totum(via, piscina);
    si (textus.mensura == ZEPHYRUM)
    {
        recusatam_nuntiare(via, "plagula legi non potest", vacua);
        redde FALSUM;
    }
    f = scaena_struere(textus, fons_picturarum, fons_partium, fontes,
        piscina);
    si (!f.successus)
    {
        /* plagula = nomen picturae (si in ea ipsa nata est) aut
         * partis; vacua = plagula scaenarum */
        si (   f.plagula.mensura > ZEPHYRUM
            && viam_plagulae(
                   chorda_aequalis(f.plagula, fontes->pictura)
                       ? fontes->fontes : fontes->partes, f.plagula,
                   via_nuntii))
        {
            via = via_nuntii;
        }
        fprintf(stderr, "%s:%u: %.*s\n", via, (insignatus)f.linea,
            (integer)f.error.mensura,
            (constans character*)f.error.datum);
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < f.numerus; k++)
    {
        chorda titulus = f.prospectus[k].titulus;

        per (j = ZEPHYRUM; j < *paratorum; j++)
        {
            si (chorda_aequalis(parati[j].titulus, titulus))
            {
                recusatam_nuntiare(via, "titulus prospectus iam ab"
                    " alia plagula huius cursus datus", titulus);
                redde FALSUM;
            }
        }
        si (*paratorum >= PARATI_MAXIMI)
        {
            recusatam_nuntiare(via, "prospectus nimis multi", vacua);
            redde FALSUM;
        }
        massa = scaena_scribere(&f.prospectus[k], piscina);
        /* massa per lectorem probatur ANTE additionem */
        si (   !massa.datum || !scaena_titulus_validus(titulus)
            || !scaena_legere(massa, &relectus, piscina))
        {
            recusatam_nuntiare(via, "massa prospectus scribi aut"
                " relegi non potest", titulus);
            redde FALSUM;
        }
        parati[*paratorum].titulus  = titulus;
        parati[*paratorum].massa    = massa;
        (*paratorum)++;
    }
    redde VERUM;
}

/* sigillum tituli in indice; chorda vacua si abest */
interior chorda
sigillum_tituli (
       Xar* index,
    chorda  titulus)
{
    chorda vacua;
       i32 k;

    vacua.datum    = NIHIL;
    vacua.mensura  = ZEPHYRUM;
    per (k = ZEPHYRUM; index && k < xar_numerus(index); k++)
    {
        ScaenaInscriptio* x =
            (ScaenaInscriptio*)xar_obtinere(index, k);

        si (x && chorda_aequalis(x->titulus, titulus))
        {
            redde x->sigillum_hex;
        }
    }
    redde vacua;
}

interior s32
exsequi_condere (
    constans character* via_voluminis,
    constans character* directorium,
    constans character* partes,
               Piscina* piscina)
{
               Volumen*  volumen;
                   Xar*  index;
                Fontes   fontes;
               Paratus*  parati;
                chorda*  nomina;
    constans character** status;
             character   via[VIA_MAXIMA];
                   s32   numerus;
                   s32   paratorum  = ZEPHYRUM;
                   s32   recusatae  = ZEPHYRUM;
                   i32   additae    = ZEPHYRUM;
                   i32   mutatae    = ZEPHYRUM;
                   i32   eaedem     = ZEPHYRUM;
                   b32   bene       = VERUM;
                   s32   k;
                   s32   j;

    fontes.fontes           = directorium;
    fontes.partes           = partes;
    fontes.pictura.datum    = NIHIL;
    fontes.pictura.mensura  = ZEPHYRUM;
    nomina = (chorda*)piscina_allocare(piscina,
        PLAGULAE_MAXIMAE * magnitudo(chorda));
    parati = (Paratus*)piscina_allocare(piscina,
        PARATI_MAXIMI * magnitudo(Paratus));
    status = (constans character**)piscina_allocare(piscina,
        PARATI_MAXIMI * magnitudo(constans character*));
    si (!nomina || !parati || !status)
    {
        fprintf(stderr, "scaena: memoria deficit\n");
        redde I;
    }
    numerus = plagulas_colligere(directorium, nomina, piscina);
    si (numerus < ZEPHYRUM)
    {
        fprintf(stderr, "scaena: directorium '%s' legi non potest"
            " (aut plus quam %u plagulae)\n", directorium,
            (insignatus)PLAGULAE_MAXIMAE);
        redde I;
    }
    si (numerus == ZEPHYRUM)
    {
        fprintf(stderr, "scaena: nullae plagulae *%s in '%s'\n",
            SUFFIXUM, directorium);
        redde I;
    }
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        si (strlen(directorium) + (memoriae_index)nomina[k].mensura
            + II > VIA_MAXIMA)
        {
            fprintf(stderr, "scaena: via nimis longa in '%s'\n",
                directorium);
            recusatae++;
            perge;
        }
        sprintf(via, "%s/%.*s", directorium,
            (integer)nomina[k].mensura,
            (constans character*)nomina[k].datum);
        si (!plagulam_parare(via, &fontes, parati, &paratorum,
            piscina))
        {
            recusatae++;
        }
    }
    si (recusatae)
    {
        fprintf(stderr, "scaena: %d plagulae recusatae; nihil"
            " scriptum\n", (integer)recusatae);
        redde I;
    }
    volumen = volumen_aperire_aut_creare(piscina, via_voluminis);
    si (!volumen)
    {
        fprintf(stderr, "scaena: volumen %s aperiri non potest\n",
            via_voluminis);
        redde I;
    }
    /* omnes in transactione una: aut omnes aut nullus */
    bene = volumen_transactionem_incipere(volumen);
    per (k = ZEPHYRUM; bene && k < paratorum; k++)
    {
        chorda prius = sigillum_tituli(
            scaena_enumerare(volumen, piscina), parati[k].titulus);
        chorda post;

        bene = scaena_addere(volumen, parati[k].titulus,
            parati[k].massa);
        post = sigillum_tituli(scaena_enumerare(volumen, piscina),
            parati[k].titulus);
        si (prius.mensura == ZEPHYRUM)
        {
            status[k] = "addita";
            additae++;
        }
        alioquin si (chorda_aequalis(prius, post))
        {
            status[k] = "eadem";
            eaedem++;
        }
        alioquin
        {
            status[k] = "mutata";
            mutatae++;
        }
    }
    si (bene)
    {
        bene = volumen_transactionem_committere(volumen);
    }
    alioquin
    {
        (vacuum)volumen_transactionem_revolvere(volumen);
    }
    si (!bene)
    {
        fprintf(stderr, "scaena: volumen scripturam recusavit: %s;"
            " nihil scriptum\n", volumen_error(volumen)
                ? volumen_error(volumen) : "?");
        volumen_claudere(volumen);
        redde I;
    }
    per (k = ZEPHYRUM; k < paratorum; k++)
    {
        imprimere("%-9s %.*s\n", status[k],
            (integer)parati[k].titulus.mensura,
            (constans character*)parati[k].titulus.datum);
    }
    index = scaena_enumerare(volumen, piscina);
    per (k = ZEPHYRUM; index && k < (s32)xar_numerus(index); k++)
    {
        ScaenaInscriptio* x =
            (ScaenaInscriptio*)xar_obtinere(index, (i32)k);

        per (j = ZEPHYRUM; j < paratorum; j++)
        {
            si (chorda_aequalis(parati[j].titulus, x->titulus))
            {
                frange;
            }
        }
        si (j == paratorum)
        {
            imprimere("sine fonte: %.*s\n",
                (integer)x->titulus.mensura,
                (constans character*)x->titulus.datum);
        }
    }
    imprimere("scaena: %u additae, %u mutatae, %u eaedem\n",
        (insignatus)additae, (insignatus)mutatae,
        (insignatus)eaedem);
    volumen_claudere(volumen);
    redde ZEPHYRUM;
}

/* volumen quod iam est aperire; NIHIL (nuntiatum) si abest */
interior Volumen*
volumen_exstans (
    constans character* via,
               Piscina* piscina)
{
    Volumen* volumen = filum_existit(via)
        ? volumen_aperire(piscina, via) : NIHIL;

    si (!volumen)
    {
        fprintf(stderr, "scaena: volumen %s aperiri non potest\n",
            via);
    }
    redde volumen;
}

interior s32
exsequi_removere (
    constans character* via_voluminis,
    constans character* titulus,
               Piscina* piscina)
{
    Volumen* volumen = volumen_exstans(via_voluminis, piscina);
        b32  bene;

    si (!volumen)
    {
        redde I;
    }
    bene = scaena_removere(volumen,
        chorda_ex_literis(titulus, piscina));
    volumen_claudere(volumen);
    si (!bene)
    {
        fprintf(stderr, "scaena: prospectus '%s' in volumine non"
            " est\n", titulus);
        redde I;
    }
    imprimere("remota    %s\n", titulus);
    redde ZEPHYRUM;
}

interior s32
exsequi_indicem (
    constans character* via_voluminis,
               Piscina* piscina)
{
    Volumen* volumen = volumen_exstans(via_voluminis, piscina);
        Xar* index;
        s32  r = ZEPHYRUM;
        i32  k;
        i32  j;

    si (!volumen)
    {
        redde I;
    }
    index = scaena_enumerare(volumen, piscina);
    per (k = ZEPHYRUM; index && k < xar_numerus(index); k++)
    {
        ScaenaInscriptio* x =
            (ScaenaInscriptio*)xar_obtinere(index, k);
        ScaenaProspectus p;

        si (!scaena_legere(scaena_promere(volumen, x->titulus,
            piscina), &p, piscina))
        {
            fprintf(stderr, "scaena: massa prospectus '%.*s' legi"
                " non potest\n", (integer)x->titulus.mensura,
                (constans character*)x->titulus.datum);
            r = I;
            perge;
        }
        imprimere("%.*s  (%u res)\n", (integer)x->titulus.mensura,
            (constans character*)x->titulus.datum,
            (insignatus)p.rerum);
        per (j = ZEPHYRUM; j < p.rerum; j++)
        {
            imprimere("    %.*s", (integer)p.res[j].titulus.mensura,
                (constans character*)p.res[j].titulus.datum);
            si (p.res[j].exitus.mensura > ZEPHYRUM)
            {
                imprimere(" -> %.*s",
                    (integer)p.res[j].exitus.mensura,
                    (constans character*)p.res[j].exitus.datum);
            }
            imprimere("\n");
        }
    }
    imprimere("scaena: %u prospectus\n",
        (insignatus)(index ? xar_numerus(index) : ZEPHYRUM));
    volumen_claudere(volumen);
    redde r;
}

externus constans ProvenientiaRelatio provenientia_scaena;

s32
principale (
      integer   argc,
    character** argv)
{
    constans character* via_voluminis  = NIHIL;
    constans character* fontes         = NIHIL;
    constans character* partes         = NIHIL;
    constans character* positivum      = NIHIL;
               Piscina* piscina;
               integer  k;
                   b32  condere;
                   b32  removere;
                   s32  r;

    si (provenientia_respondere((s32)argc, argv,
        &provenientia_scaena))
    {
        redde ZEPHYRUM;
    }
    si (argc < II)
    {
        redde usum_imprimere();
    }
    condere   = strcmp(argv[I], "condere") == ZEPHYRUM;
    removere  = strcmp(argv[I], "removere") == ZEPHYRUM;
    si (   !condere && !removere
        && strcmp(argv[I], "index") != ZEPHYRUM)
    {
        redde usum_imprimere();
    }
    per (k = II; k < argc; k++)
    {
        constans character** scopus = NIHIL;

        si (strcmp(argv[k], "-volumen") == ZEPHYRUM)
        {
            scopus = &via_voluminis;
        }
        alioquin si (condere && strcmp(argv[k], "-fontes") == ZEPHYRUM)
        {
            scopus = &fontes;
        }
        alioquin si (condere && strcmp(argv[k], "-partes") == ZEPHYRUM)
        {
            scopus = &partes;
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
        si (argv[k][ZEPHYRUM] == '-' || positivum || !removere)
        {
            redde usum_imprimere();
        }
        positivum = argv[k];
    }
    si (!via_voluminis || (removere && !positivum))
    {
        redde usum_imprimere();
    }
    piscina = piscina_generare_dynamicum("scaena", M * M);
    si (!piscina)
    {
        fprintf(stderr, "scaena: memoria deficit\n");
        redde I;
    }
    si (condere)
    {
        r = exsequi_condere(via_voluminis,
            fontes ? fontes : FONTES_ORDINARII,
            partes ? partes : PARTES_ORDINARIAE, piscina);
    }
    alioquin si (removere)
    {
        r = exsequi_removere(via_voluminis, positivum, piscina);
    }
    alioquin
    {
        r = exsequi_indicem(via_voluminis, piscina);
    }
    piscina_destruere(piscina);
    redde r;
}
