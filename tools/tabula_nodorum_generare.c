/* tools/tabula_nodorum_generare.c - tabula nodorum GENERATA
 *
 * Ex extracto KnotInfo fixo (probationes/fixa/knotinfo/<versio>/
 * nodi_xiii.tsv: titulus, transitus, symmetria, codex PD) polynomia
 * Alexander (forma normalis) et Jones per laqueus COMPUTAT et
 * lib/tabula_nodorum_data.c scribit. Polynomia KnotInfo non leguntur
 * nisi in modo -collatio (extractio sola, plagula non commissa).
 *
 * Usus: tabula_nodorum_generare <nodi_xiii.tsv> <exitus.c>
 *       tabula_nodorum_generare -collatio <nodi_xiii.tsv>
 *           <polynomia.tsv>
 * Exitus: 0 bene · 1 discrepantia (collatio) · 2 usus, lectio aut
 * calculus.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "polynomium.h"
#include "laqueus.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NODI_MAXIMI    (XIV * M)
#define LINEA_MAXIMA   (VIII * M)
#define ORAE_MAXIMAE   (VIII * X)

nomen structura {
             character  titulus[XVI];
                   i32  transitus;
    constans character* symmetria;
                   i32  initium_pd;
                chorda  alexander;
                chorda  jones;
} Nodus;

hic_manens Nodus      nodi[NODI_MAXIMI];
hic_manens i32        numerus_nodorum = ZEPHYRUM;
hic_manens i32        piscina_pd[NODI_MAXIMI * ORAE_MAXIMAE];
hic_manens i32        numerus_pd = ZEPHYRUM;
hic_manens character  versio[LXIV];
hic_manens character  sha[LXXX];

interior vacuum
fracta (
    constans character* nuntius,
    constans character* res)
{
    fprintf(stderr, "tabula_nodorum_generare: %s%s\n", nuntius, res);
    exit(II);
}

/* "reversible" -> "TABULA_NODORUM_REVERSIBILIS"; NIHIL si ignota */
interior constans character*
symmetria_ex_textu (
    constans character* textus)
{
    si (textus[ZEPHYRUM] == '\0')
    {
        redde "TABULA_NODORUM_NULLA";
    }
    si (strcmp(textus, "reversible") == ZEPHYRUM)
    {
        redde "TABULA_NODORUM_REVERSIBILIS";
    }
    si (strcmp(textus, "chiral") == ZEPHYRUM)
    {
        redde "TABULA_NODORUM_CHIRALIS";
    }
    si (strcmp(textus, "fully amphicheiral") == ZEPHYRUM)
    {
        redde "TABULA_NODORUM_AMPHICHIRALIS_PLENA";
    }
    si (strcmp(textus, "negative amphicheiral") == ZEPHYRUM)
    {
        redde "TABULA_NODORUM_AMPHICHIRALIS_NEGATIVA";
    }
    si (strcmp(textus, "positive amphicheiral") == ZEPHYRUM)
    {
        redde "TABULA_NODORUM_AMPHICHIRALIS_POSITIVA";
    }
    redde NIHIL;
}

/* linea in campos per tabulas scinditur (in loco); reddit numerum */
interior i32
campi_scindere (
    character*  linea,
    character** campi,
          i32   maximi)
{
          i32  n = I;
    character* p;

    campi[ZEPHYRUM] = linea;
    per (p = linea; *p != '\0'; p++)
    {
        si (*p == '\n' || *p == '\r')
        {
            *p = '\0';
            frange;
        }
        si (*p == '\t' && n < maximi)
        {
            *p        = '\0';
            campi[n]  = p + I;
            n++;
        }
    }
    redde n;
}

/* invariantes nodi ex codice PD; transitus 0 = nodus trivialis */
interior vacuum
computare (
      Nodus* nodus,
    Piscina* piscina)
{
    Polynomium alexander  = polynomium_nullum();
    Polynomium normalis   = polynomium_nullum();
    Polynomium jones      = polynomium_nullum();

    si (nodus->transitus == ZEPHYRUM)
    {
        nodus->alexander  = chorda_ex_literis("1", piscina);
        nodus->jones      = chorda_ex_literis("1", piscina);
        redde;
    }
    si (   !laqueus_alexander_ex_pd(piscina_pd + nodus->initium_pd,
            nodus->transitus, piscina, &alexander)
        || !polynomium_normale(alexander, piscina, &normalis)
        || !laqueus_jones_ex_pd(piscina_pd + nodus->initium_pd,
            nodus->transitus, piscina, &jones))
    {
        fracta("codex PD refutatus: ", nodus->titulus);
    }
    nodus->alexander  = polynomium_ad_chordam(normalis, 't', piscina);
    nodus->jones      = polynomium_ad_chordam(jones, 't', piscina);
}

/* "# ... database_knotinfo V" et "# sha256 S" ex capite fixi */
interior vacuum
caput_legere (
    constans character* linea)
{
    constans character* p = strstr(linea, "database_knotinfo ");

    si (p != NIHIL && versio[ZEPHYRUM] == '\0')
    {
        sscanf(p + XVIII, "%63s", versio);
    }
    si (strncmp(linea, "# sha256 ", IX) == ZEPHYRUM)
    {
        sscanf(linea + IX, "%79s", sha);
    }
}

interior vacuum
fixum_legere (
    constans character* via,
    Piscina*            piscina)
{
         FILE* f = fopen(via, "r");
    character  linea[LINEA_MAXIMA];

    si (f == NIHIL)
    {
        fracta("legi non potest: ", via);
    }
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
         character* campi[IV];
         character* p;
             Nodus* nodus;
               i32  orae       = ZEPHYRUM;
               i32  valor      = ZEPHYRUM;
               b32  in_numero  = FALSUM;

        si (linea[ZEPHYRUM] == '#')
        {
            caput_legere(linea);
            perge;
        }
        si (campi_scindere(linea, campi, IV) != IV)
        {
            fracta("linea malformata: ", linea);
        }
        si (   numerus_nodorum >= NODI_MAXIMI
            || strlen(campi[ZEPHYRUM])
                >= magnitudo(nodi[ZEPHYRUM].titulus))
        {
            fracta("nimis multi nodi aut titulus longus: ",
                campi[ZEPHYRUM]);
        }
        nodus = &nodi[numerus_nodorum];
        strcpy(nodus->titulus, campi[ZEPHYRUM]);
        nodus->transitus   = (i32)atoi(campi[I]);
        nodus->symmetria   = symmetria_ex_textu(campi[II]);
        nodus->initium_pd  = numerus_pd;
        si (nodus->symmetria == NIHIL)
        {
            fracta("symmetria ignota: ", campi[II]);
        }
        /* symmetria vacua = nodus trivialis solus */
        si ((campi[II][ZEPHYRUM] == '\0') != (nodus->transitus
            == ZEPHYRUM))
        {
            fracta("symmetria vacua sine transitu nullo (aut contra): ",
                nodus->titulus);
        }
        /* "[[1,5,2,4],[3,1,4,6],...]": numeri soli */
        per (p = campi[III]; ; p++)
        {
            si (*p >= '0' && *p <= '9')
            {
                valor      = valor * X + (i32)(*p - '0');
                in_numero  = VERUM;
            }
            alioquin si (in_numero)
            {
                si (numerus_pd >= NODI_MAXIMI * ORAE_MAXIMAE)
                {
                    fracta("piscina PD plena: ", nodus->titulus);
                }
                piscina_pd[numerus_pd++] = valor;
                orae++;
                valor      = ZEPHYRUM;
                in_numero  = FALSUM;
            }
            si (*p == '\0')
            {
                frange;
            }
        }
        si (orae != IV * nodus->transitus)
        {
            fracta("codex PD longitudine falsa: ", nodus->titulus);
        }
        computare(nodus, piscina);
        numerus_nodorum++;
    }
    fclose(f);
    si (versio[ZEPHYRUM] == '\0' || sha[ZEPHYRUM] == '\0')
    {
        fracta("caput fixi sine versione aut sha256: ", via);
    }
}

interior vacuum
chordam_scribere (
      FILE* f,
    chorda  c)
{
    fwrite(c.datum, I, (size_t)c.mensura, f);
}

interior vacuum
scribere (
    constans character* fixum,
    constans character* via)
{
    FILE* f = fopen(via, "w");
     i32  k;
     i32  j;

    si (f == NIHIL)
    {
        fracta("scribi non potest: ", via);
    }
    fprintf(f,
        "/* GENERATUM: tools/tabula_nodorum_generare.sh - "
        "NE EDITA MANU\n"
        " *\n"
        " * Ex %s:\n"
        " * nomina, transitus, symmetria et codices PD ex KnotInfo\n"
        " * (C. Livingston, A. H. Moore, knotinfo.math.indiana.edu) "
        "per\n"
        " * database_knotinfo %s, knotinfo_data_complete.csv,\n"
        " * sha256 %s.\n"
        " * Polynomia Alexander (forma normalis) et Jones per laqueus "
        "ex\n"
        " * codicibus PD COMPUTATA sunt.\n"
        " */\n"
        "#include \"tabula_nodorum.h\"\n\n", fixum, versio, sha);
    fprintf(f, "constans character* TABULA_NODORUM_FONS =\n"
        "    \"KnotInfo (C. Livingston, A. H. Moore) per "
        "database_knotinfo "
        "%s, sha256 %s\";\n\n", versio, sha);
    fprintf(f, "constans i32 TABULA_NODORUM_PD[] = {\n");
    per (k = ZEPHYRUM; k < numerus_nodorum; k++)
    {
        si (nodi[k].transitus == ZEPHYRUM)
        {
            perge;
        }
        fprintf(f, "    /* %s */\n   ", nodi[k].titulus);
        per (j = ZEPHYRUM; j < IV * nodi[k].transitus; j++)
        {
            fprintf(f, " %u,", piscina_pd[nodi[k].initium_pd + j]);
        }
        fprintf(f, "\n");
    }
    fprintf(f, "};\n\nconstans NodusTabulae TABULA_NODORUM[] = {\n");
    per (k = ZEPHYRUM; k < numerus_nodorum; k++)
    {
        fprintf(f, "    { \"%s\", %u, %s, %u,\n        \"",
            nodi[k].titulus,
            nodi[k].transitus, nodi[k].symmetria, nodi[k].initium_pd);
        chordam_scribere(f, nodi[k].alexander);
        fprintf(f, "\",\n        \"");
        chordam_scribere(f, nodi[k].jones);
        fprintf(f, "\" },\n");
    }
    fprintf(f, "};\n\nconstans i32 TABULA_NODORUM_NUMERUS = %u;\n",
        numerus_nodorum);
    si (fclose(f) != ZEPHYRUM)
    {
        fracta("scriptura fracta: ", via);
    }
}

/* forma KnotInfo -> forma polynomium_ex_chorda: "t^(-2)-t^(-1)+ 1-3*t"
 * -> "t^-2-t^-1+1-3t" (spatia, '*', parentheses absunt); supra X
 * transitus KnotInfo "N/t" et "N/t^K" scribit -> "Nt^-1", "Nt^-K" */
interior chorda
alienum_purgare (
    constans character* textus,
               Piscina* piscina)
{
    character* alveus = (character*)piscina_allocare(piscina,
        (memoriae_index)(II * strlen(textus) + I));
          i32 n = ZEPHYRUM;

    per (; *textus != '\0'; textus++)
    {
        si (*textus == '/' && textus[I] == 't')
        {
            textus++;
            alveus[n++] = 't';
            alveus[n++] = '^';
            alveus[n++] = '-';
            si (textus[I] == '^')
            {
                textus++;
            }
            alioquin
            {
                alveus[n++] = '1';
            }
        }
        alioquin si (   *textus != ' ' && *textus != '*'
                     && *textus != '('
                     && *textus != ')')
        {
            alveus[n++] = *textus;
        }
    }
    redde chorda_ex_buffer((i8*)alveus, n);
}

interior b32
congruit (
    constans character* alienum,
                chorda  nostrum,
                   b32  normare,
               Piscina* piscina)
{
    Polynomium illud;
    Polynomium hoc;

    si (   !polynomium_ex_chorda(alienum_purgare(alienum, piscina),
        't',
            piscina, &illud)
        || !polynomium_ex_chorda(nostrum, 't', piscina, &hoc))
    {
        redde FALSUM;
    }
    si (normare && !polynomium_normale(illud, piscina, &illud))
    {
        redde FALSUM;
    }
    redde polynomium_aequalis(illud, hoc);
}

interior integer
conferre (
    constans character* via,
    Piscina*            piscina)
{
         FILE* f = fopen(via, "r");
    character  linea[LINEA_MAXIMA];
          i32  k              = ZEPHYRUM;
          i32  discrepantiae  = ZEPHYRUM;

    si (f == NIHIL)
    {
        fracta("legi non potest: ", via);
    }
    dum (fgets(linea, (integer)magnitudo(linea), f) != NIHIL)
    {
        character* campi[III];

        si (   campi_scindere(linea, campi, III) != III
            || k >= numerus_nodorum
            || strcmp(campi[ZEPHYRUM], nodi[k].titulus) != ZEPHYRUM)
        {
            fracta("polynomia ordine fixi non sunt: ", campi[ZEPHYRUM]);
        }
        si (!congruit(campi[I], nodi[k].alexander, VERUM, piscina))
        {
            fprintf(stderr, "  DISCREPAT Alexander %s: KnotInfo %s, "
                "laqueus %.*s\n", nodi[k].titulus, campi[I],
                (integer)nodi[k].alexander.mensura,
                (constans character*)nodi[k].alexander.datum);
            discrepantiae++;
        }
        si (!congruit(campi[II], nodi[k].jones, FALSUM, piscina))
        {
            fprintf(stderr, "  DISCREPAT Jones %s: KnotInfo %s, "
                "laqueus %.*s\n", nodi[k].titulus, campi[II],
                (integer)nodi[k].jones.mensura,
                (constans character*)nodi[k].jones.datum);
            discrepantiae++;
        }
        k++;
    }
    fclose(f);
    si (k != numerus_nodorum)
    {
        fracta("polynomia pauciora quam nodi: ", via);
    }
    si (discrepantiae != ZEPHYRUM)
    {
        fprintf(stderr, "tabula_nodorum_generare: %u discrepantiae\n",
            discrepantiae);
        redde I;
    }
    printf("tabula_nodorum_generare: %u nodi, Alexander et Jones cum "
        "KnotInfo congruunt\n", numerus_nodorum);
    redde ZEPHYRUM;
}

integer
principale (
      integer   argc,
    character** argv)
{
    Piscina* piscina = piscina_generare_dynamicum("tabula_nodorum",
        (memoriae_index)(LXIV * M));

    si (argc == IV && strcmp(argv[I], "-collatio") == ZEPHYRUM)
    {
        fixum_legere(argv[II], piscina);
        redde conferre(argv[III], piscina);
    }
    si (argc != III)
    {
        fracta("usus: tabula_nodorum_generare [-collatio] ",
            "nodi_xiii.tsv (exitus.c | polynomia.tsv)");
    }
    fixum_legere(argv[I], piscina);
    scribere(argv[I], argv[II]);
    redde ZEPHYRUM;
}
