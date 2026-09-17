/* oraculum.c - Instrumentum oraculi bash (crusta/oraculum.sh; P11b)
 *
 * Usus:
 *   oraculum -scribere           aura expectata.txt et sanitas.txt
 *   oraculum -probare            bash iterum currit, aura comparat
 *   oraculum -domus <plagulae>   plagulae domus VIVAE (sine auro)
 *
 * BASH: /opt/homebrew/bin/bash 5.2 (versio sine '5.2' RECUSATUR - Apple
 * /bin/bash 3.2 aliter deparsat). AMBITUS: 'env -i LC_ALL=C
 * PATH=/nonexistent bash -r' - locale fixum (aurum deterministicum;
 * mensuratum: '\u' et '\c' idem sub C et en_US.UTF-8) et concha
 * restricta sine PATH: casus qui '}' involucri praemature claudit
 * reliquum in summo gradu EXSEQUERETUR - sic nec imperia externa nec
 * redirectiones in plagulas attingit. processus_exsequi (argumenta ut
 * vector, nulla concha interposita).
 *
 * declare -f: scriptum 'f() {\n' + casus (+ '\n') + '}\ndeclare -f
 * f\n'; aurum solum si exitus 0, effusio a 'f () ' incipit et lineam
 * '## END' non fert (lector exemplorum sectionem ibi clauderet) -
 * aliter 'sine auro' cum causa nominata. Casus cum NUL: bash argumentum
 * cum NUL accipere non potest - sine auro. sanitas: 'bash -n -c casus',
 * status et linea prima erratorum ('-' si nulla).
 *
 * -domus: involucrum idem, arbor nostra coquitur et cum 'declare -f'
 * vivo comparatur; 'bash -n' contra relatio.sana; numeri per classem et
 * discordiae X primae utriusque. Exitus: 0 sanum, 1 aurum differt
 * (-probare), 2 usus / bash recusatus / plagula absens.
 */

#include "latina.h"
#include "chorda_aedificator.h"
#include "crusta_arbor.h"
#include "crusta_coctum.h"
#include "crusta_exempla.h"
#include "crusta_oraculum.h"
#include "crusta_registrum.h"
#include "piscina.h"
#include "processus.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BASH_VIA "/opt/homebrew/bin/bash"
#define MORA_MS  10000

nomen structura {
    i32 casus_numerus;
    i32 aurum;
    i32 recusati;
    i32 octeti_nulli;
    i32 alieni;
    i32 finis_in_effusione;
    i32 sanitas;
} Numeri;


/* ==================================================
 * Processus bash
 * ================================================== */

interior b32
_octetum_nullum_fert (
    chorda s)
{
    i32 k;

    per (k = ZEPHYRUM; k < s.mensura; k++)
    {
        si (s.datum[k] == '\0')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* linea prima (sine '\n' et '\r' finali) */
interior chorda
_linea_prima (
    chorda s)
{
    chorda l = s;
       i32 k = ZEPHYRUM;

    dum (k < s.mensura && s.datum[k] != '\n')
    {
        k++;
    }
    dum (k > ZEPHYRUM && s.datum[k - I] == '\r')
    {
        k--;
    }
    l.mensura = k;
    redde l;
}

interior b32
_versio (
     Piscina* piscina,
      chorda* linea)
{
     constans character* argumenta[] = { BASH_VIA, "--version", NIHIL };
      ProcessusResultus  r;
                    i32  k;

    r = processus_exsequi(argumenta, MORA_MS, piscina);
    si (!r.successus || r.codex_exitus != ZEPHYRUM)
    {
        redde FALSUM;
    }
    *linea = _linea_prima(r.effusio);
    per (k = ZEPHYRUM; k + III <= linea->mensura; k++)
    {
        si (memcmp(linea->datum + k, "5.2", III) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* an linea detonsa '## END' sit (lector exemplorum eam ut finem
 * sectionis legeret) */
interior b32
_finem_fert (
    chorda s)
{
    i32 ab = ZEPHYRUM;

    dum (ab < s.mensura)
    {
        i32 ad = ab;
        i32 k;
        i32 finis;

        dum (ad < s.mensura && s.datum[ad] != '\n')
        {
            ad++;
        }
        si (   ad - ab >= (i32)VI && memcmp(s.datum + ab, "## ", III)
            == ZEPHYRUM)
        {
            k      = ab + III;
            finis  = ad;
            dum (k < finis && (s.datum[k] == ' ' || s.datum[k] == '\t'))
            {
                k++;
            }
            dum (   finis > k && (s.datum[finis - I] == ' '
                || s.datum[finis - I] == '\t'
                || s.datum[finis - I] == '\r'))
            {
                finis--;
            }
            si (finis - k == (i32)III && memcmp(s.datum + k, "END", III)
                == ZEPHYRUM)
            {
                redde VERUM;
            }
        }
        ab = ad + I;
    }
    redde FALSUM;
}

/* 'declare -f f' super involucrum; FALSUM = sine auro, *causa
 * nominat */
interior b32
_declarare (
                Piscina*  piscina,
                 chorda   involutum,
                 chorda*  effusio,
     constans character** causa)
{
    constans character* argumenta[] = { "/usr/bin/env", "-i",
        "LC_ALL=C",
        "PATH=/nonexistent", BASH_VIA, "-r", "-c", NIHIL, NIHIL };
             character* scriptum;
     ProcessusResultus  r;

    si (_octetum_nullum_fert(involutum))
    {
        *causa = "NUL";
        redde FALSUM;
    }
    scriptum = (character*)piscina_allocare(piscina,
        (memoriae_index)involutum.mensura + (memoriae_index)XVI);
    si (scriptum == NIHIL)
    {
        *causa = "memoria";
        redde FALSUM;
    }
    memcpy(scriptum, involutum.datum, (size_t)involutum.mensura);
    memcpy(scriptum + involutum.mensura, "declare -f f\n", XIV);
    argumenta[VII]  = scriptum;
    r               = processus_exsequi(argumenta, MORA_MS, piscina);
    si (   !r.successus || r.codex_exitus != ZEPHYRUM
        || r.signum != ZEPHYRUM)
    {
        *causa = "bash recusat";
        redde FALSUM;
    }
    si (   r.effusio.mensura < (i32)VIII || memcmp(r.effusio.datum,
        "f () \n{ ", VIII) != ZEPHYRUM)
    {
        *causa = "effusio aliena";
        redde FALSUM;
    }
    si (_finem_fert(r.effusio))
    {
        *causa = "'## END' in effusione";
        redde FALSUM;
    }
    *effusio = r.effusio;
    redde VERUM;
}

/* 'bash -n -c datum': status et linea prima erratorum */
interior b32
_sanitas (
                Piscina* piscina,
                 chorda  datum,
                    i32* status,
                 chorda* erratum)
{
    constans character* argumenta[] = { "/usr/bin/env", "-i",
        "LC_ALL=C",
        "PATH=/nonexistent", BASH_VIA, "-r", "-n", "-c", NIHIL, NIHIL };
    ProcessusResultus r;

    si (_octetum_nullum_fert(datum))
    {
        redde FALSUM;
    }
    argumenta[VIII]  = (constans character*)datum.datum;
    r                = processus_exsequi(argumenta, MORA_MS, piscina);
    si (!r.successus || r.signum != ZEPHYRUM)
    {
        redde FALSUM;
    }
    *status   = r.codex_exitus;
    *erratum  = _linea_prima(r.erratum);
    redde VERUM;
}

interior chorda
_coquere (
          Piscina* piscina,
           chorda  textus,
    CrustaParsura* relatio)
{
    MateriaNodus* radix;
           chorda  vacua;

    radix = crusta_arbor_parsare(piscina, (constans character*)
        textus.datum, textus.mensura, &CRUSTA_BASH, relatio);
    si (radix == NIHIL)
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }
    redde crusta_coctum_scribere(piscina, radix);
}


/* ==================================================
 * Aura
 * ================================================== */

interior vacuum
_praefatio (
     ChordaAedificator* a,
    constans character* plagula,
    constans character* forma,
                chorda  versio)
{
    character dies[32];
       time_t nunc = time(NIHIL);

    strftime(dies, magnitudo(dies), "%Y-%m-%d", localtime(&nunc));
    chorda_aedificator_appendere_literis(a, "# ");
    chorda_aedificator_appendere_literis(a, plagula);
    chorda_aedificator_appendere_literis(a,
        " - aurum oraculi crustae (P11b): ");
    chorda_aedificator_appendere_literis(a, forma);
    chorda_aedificator_appendere_literis(a, "\n# bash: ");
    chorda_aedificator_appendere_chorda(a, versio);
    chorda_aedificator_appendere_literis(a,
        "\n# ambitus: env -i LC_ALL=C "
        "PATH=/nonexistent " BASH_VIA " -r\n# scriptum ");
    chorda_aedificator_appendere_literis(a, dies);
    chorda_aedificator_appendere_literis(a, " per ./crusta/oraculum.sh "
        "-scribere; numquam manu\n");
}

interior b32
_aura_componere (
                Piscina* piscina,
     constans character* radix,
                 chorda  versio,
      ChordaAedificator* expectata,
      ChordaAedificator* sanitas,
                 Numeri* n)
{
    constans character* causa;
                   Xar* collectio;
                   i32  k;

    collectio = crusta_oraculum_casus(piscina, radix, &causa);
    si (collectio == NIHIL)
    {
        fprintf(stderr, "oraculum: casus non lecti: %s\n",
            causa ? causa : "-");
        redde FALSUM;
    }
    memset(n, ZEPHYRUM, magnitudo(*n));
    _praefatio(expectata, "expectata.txt",
        "'declare -f f' super 'f() {' casus '}'", versio);
    _praefatio(sanitas, "sanitas.txt", "'bash -n -c casus'", versio);
    per (k = ZEPHYRUM; k < xar_numerus(collectio); k++)
    {
        constans CrustaCasusOraculi* c =
            (constans CrustaCasusOraculi*)xar_obtinere(collectio, k);
                              chorda effusio;
                              chorda erratum;
                                 i32 status;

        n->casus_numerus++;
        si (_declarare(piscina, crusta_oraculum_involvere(piscina,
                c->datum), &effusio, &causa))
        {
            chorda_aedificator_appendere_literis(expectata, "#### ");
            chorda_aedificator_appendere_chorda(expectata, c->clavis);
            chorda_aedificator_appendere_literis(expectata,
                "\n## EXSPECTATUM:\n");
            chorda_aedificator_appendere_chorda(expectata, effusio);
            chorda_aedificator_appendere_literis(expectata, "## END\n");
            n->aurum++;
        }
        alioquin si (strcmp(causa, "NUL") == ZEPHYRUM)
        {
            n->octeti_nulli++;
        }
        alioquin si (strcmp(causa, "bash recusat") == ZEPHYRUM)
        {
            n->recusati++;
        }
        alioquin si (strcmp(causa, "effusio aliena") == ZEPHYRUM)
        {
            n->alieni++;
        }
        alioquin
        {
            n->finis_in_effusione++;
        }
        si (_sanitas(piscina, c->datum, &status, &erratum))
        {
            chorda_aedificator_appendere_literis(sanitas, "#### ");
            chorda_aedificator_appendere_chorda(sanitas, c->clavis);
            chorda_aedificator_appendere_literis(sanitas,
                "\n## status: ");
            chorda_aedificator_appendere_i32(sanitas, status);
            chorda_aedificator_appendere_literis(sanitas,
                "\n## ERRATUM: ");
            si (erratum.mensura > ZEPHYRUM)
            {
                chorda_aedificator_appendere_chorda(sanitas, erratum);
            }
            alioquin
            {
                chorda_aedificator_appendere_literis(sanitas, "-");
            }
            chorda_aedificator_appendere_literis(sanitas, "\n");
            n->sanitas++;
        }
    }
    redde VERUM;
}

interior vacuum
_numeros_imprimere (
    constans Numeri* n)
{
    imprimere("casus %d: aurum declare -f %d, sine auro %d (bash "
        "recusat %d, NUL %d, effusio aliena %d, '## END' %d); sanitas "
        "%d\n",
        (integer)n->casus_numerus, (integer)n->aurum,
        (integer)(n->casus_numerus - n->aurum), (integer)n->recusati,
        (integer)n->octeti_nulli, (integer)n->alieni,
        (integer)n->finis_in_effusione, (integer)n->sanitas);
}

interior b32
_scribere_plagulam (
     constans character* radix,
     constans character* via,
                 chorda  textus)
{
    character plena[4096];
        FILE* f;
       size_t scripti;

    sprintf(plena, "%s/%s", radix, via);
    f = fopen(plena, "wb");
    si (f == NIHIL)
    {
        redde FALSUM;
    }
    scripti = fwrite(textus.datum, I, (size_t)textus.mensura, f);
    fclose(f);
    redde scripti == (size_t)textus.mensura;
}

/* corpus post praefationem ('####' primum) */
interior chorda
_corpus (
    chorda s)
{
    i32 k = ZEPHYRUM;

    dum (k < s.mensura)
    {
        si (   s.mensura - k                   >= (i32)IV
            && memcmp(s.datum + k, "####", IV) == ZEPHYRUM)
        {
            s.datum    += k;
            s.mensura  -= k;
            redde s;
        }
        dum (k < s.mensura && s.datum[k] != '\n')
        {
            k++;
        }
        k++;
    }
    s.mensura = ZEPHYRUM;
    redde s;
}

/* comparatio corporum; clavem casus primi differentis imprimit */
interior b32
_aurum_probare (
                Piscina* piscina,
     constans character* radix,
     constans character* via,
                 chorda  recens)
{
                   i32  mensura;
             character* vetus = crusta_oraculum_legere(piscina, radix,
                 via, &mensura);
                 chorda a;
                 chorda b;
                    i32 k       = ZEPHYRUM;
                    i32 clavis  = ZEPHYRUM;

    si (vetus == NIHIL)
    {
        imprimere("%s: ABSENS\n", via);
        redde FALSUM;
    }
    a.datum    = recens.datum;
    a.mensura  = recens.mensura;
    a          = _corpus(a);
    b.datum    = (i8*)vetus;
    b.mensura  = mensura;
    b          = _corpus(b);
    dum (k < a.mensura && k < b.mensura && a.datum[k] == b.datum[k])
    {
        si (   k + IV < a.mensura && a.datum[k] == '\n'
            && memcmp(a.datum + k + I, "####", IV) == ZEPHYRUM)
        {
            clavis = k + I;
        }
        k++;
    }
    si (k == a.mensura && k == b.mensura)
    {
        imprimere("%s: IDEM\n", via);
        redde VERUM;
    }
    {
        i32 finis = clavis;

        dum (finis < a.mensura && a.datum[finis] != '\n')
        {
            finis++;
        }
        imprimere("%s: DIFFERT ab casu '%.*s' (octetus %d corporis)\n",
            via, (integer)(finis - clavis),
            (constans character*)a.datum + clavis, (integer)k);
    }
    redde FALSUM;
}


/* ==================================================
 * Domus
 * ================================================== */

interior vacuum
_differentiam_imprimere (
     constans character* via,
                 chorda  exspectatum,
                 chorda  habitum)
{
    i32 k        = ZEPHYRUM;
    i32 initium  = ZEPHYRUM;
    i32 linea    = I;
    i32 fe;
    i32 fh;

    dum (   k < exspectatum.mensura && k < habitum.mensura
         && exspectatum.datum[k] == habitum.datum[k])
    {
        si (exspectatum.datum[k] == '\n')
        {
            initium = k + I;
            linea++;
        }
        k++;
    }
    fe = initium;
    dum (fe < exspectatum.mensura && exspectatum.datum[fe] != '\n')
    {
        fe++;
    }
    fh = initium;
    dum (fh < habitum.mensura && habitum.datum[fh] != '\n')
    {
        fh++;
    }
    imprimere("  %s linea %d\n    bash:   '%.*s'\n    crusta: '%.*s'\n",
        via, (integer)linea, (integer)(fe - initium),
        (constans character*)exspectatum.datum + initium,
        (integer)(fh > initium ? fh - initium : ZEPHYRUM),
        (constans character*)habitum.datum + initium);
}

interior integer
_domus (
                  integer   numerus,
    constans character* constans* viae)
{
        i32 pares      = ZEPHYRUM;
        i32 dispares   = ZEPHYRUM;
        i32 sine_auro  = ZEPHYRUM;
        i32 concordes  = ZEPHYRUM;
        i32 discordes  = ZEPHYRUM;
        i32 impressi   = ZEPHYRUM;
        i32 impressae  = ZEPHYRUM;
    integer k;

    per (k = ZEPHYRUM; k < numerus; k++)
    {
             Piscina* piscina =
                 piscina_generare_dynamicum("oraculum_domus",
                 1048576);
                  i32  mensura;
            character* fons;
               chorda  datum;
               chorda  effusio;
               chorda  coctum;
               chorda  erratum;
                  i32  status;
        CrustaParsura  relatio;
   constans character* causa;

        si (piscina == NIHIL)
        {
            redde II;
        }
        fons = crusta_oraculum_legere(piscina,
            viae[k][ZEPHYRUM] == '/' ? "" : ".", viae[k], &mensura);
        si (fons == NIHIL)
        {
            fprintf(stderr, "oraculum: plagula absens: %s\n", viae[k]);
            piscina_destruere(piscina);
            redde II;
        }
        datum.datum    = (i8*)fons;
        datum.mensura  = mensura;
        si (_declarare(piscina, crusta_oraculum_involvere(piscina,
            datum),
                &effusio, &causa))
        {
            coctum = _coquere(piscina,
                crusta_oraculum_involvere(piscina,
                datum), &relatio);
            si (   coctum.datum   != NIHIL
                && coctum.mensura == effusio.mensura
                && memcmp(coctum.datum, effusio.datum,
                    (size_t)effusio.mensura) == ZEPHYRUM)
            {
                pares++;
            }
            alioquin
            {
                dispares++;
                si (impressi < (i32)X && coctum.datum != NIHIL)
                {
                    _differentiam_imprimere(viae[k], effusio, coctum);
                    impressi++;
                }
            }
        }
        alioquin
        {
            sine_auro++;
            imprimere("  %s sine auro: %s\n", viae[k], causa);
        }
        si (_sanitas(piscina, datum, &status, &erratum))
        {
            MateriaNodus* radix = crusta_arbor_parsare(piscina, fons,
                mensura, &CRUSTA_BASH, &relatio);
                      b32 sana  = (radix != NIHIL && relatio.sana)
                          ? VERUM : FALSUM;
                      b32 accepta = status == ZEPHYRUM ? VERUM : FALSUM;

            si (accepta == sana)
            {
                concordes++;
            }
            alioquin
            {
                discordes++;
                si (impressae < (i32)X)
                {
                    imprimere("  %s sanitas: bash status %d ('%.*s'), "
                        "crusta sana %d (mala %d, clausurae absentes "
                        "%d)\n", viae[k], (integer)status,
                        (integer)erratum.mensura,
                        (constans character*)erratum.datum,
                        (integer)sana, (integer)relatio.mala,
                        (integer)relatio.clausurae_absentes);
                    impressae++;
                }
            }
        }
        piscina_destruere(piscina);
    }
    imprimere("domus: plagulae %d; declare -f: pares %d, dispares %d, "
        "sine auro %d; sanitas: concordes %d, discordes %d\n",
        (integer)numerus, (integer)pares, (integer)dispares,
        (integer)sine_auro, (integer)concordes, (integer)discordes);
    redde ZEPHYRUM;
}

integer
principale (
      integer   argc,
    character** argv)
{
                 Piscina* piscina;
      constans character* radix = getenv("RHUBARB_RADIX");
                  chorda  versio;
       ChordaAedificator* expectata;
       ChordaAedificator* sanitas;
                  Numeri  n;

    si (argc < II)
    {
        fprintf(stderr, "usus: oraculum -scribere | -probare | -domus "
            "<plagulae...>\n");
        redde II;
    }
    si (radix == NIHIL)
    {
        radix = ".";
    }
    piscina = piscina_generare_dynamicum("oraculum", 4194304);
    si (piscina == NIHIL)
    {
        redde II;
    }
    si (!_versio(piscina, &versio))
    {
        fprintf(stderr, "oraculum: %s non est bash 5.2 - RECUSATUR\n",
            BASH_VIA);
        redde II;
    }
    si (strcmp(argv[I], "-domus") == ZEPHYRUM)
    {
        redde _domus(argc - II, (constans character* constans*)
            (argv + II));
    }
    si (   strcmp(argv[I], "-scribere") != ZEPHYRUM
        && strcmp(argv[I], "-probare")  != ZEPHYRUM)
    {
        fprintf(stderr, "oraculum: modus ignotus: %s\n", argv[I]);
        redde II;
    }
    expectata = chorda_aedificator_creare(piscina,
        (memoriae_index)65536);
    sanitas = chorda_aedificator_creare(piscina,
        (memoriae_index)16384);
    si (   expectata == NIHIL || sanitas == NIHIL
        || !_aura_componere(piscina, radix, versio, expectata, sanitas,
            &n))
    {
        redde II;
    }
    _numeros_imprimere(&n);
    si (strcmp(argv[I], "-scribere") == ZEPHYRUM)
    {
        si (   !_scribere_plagulam(radix, CRUSTA_ORACULUM_EXPECTATA,
                chorda_aedificator_spectare(expectata))
            || !_scribere_plagulam(radix, CRUSTA_ORACULUM_SANITAS,
                chorda_aedificator_spectare(sanitas)))
        {
            fprintf(stderr, "oraculum: aurum scribi non potuit\n");
            redde II;
        }
        imprimere("scripta: %s, %s\n", CRUSTA_ORACULUM_EXPECTATA,
            CRUSTA_ORACULUM_SANITAS);
        redde ZEPHYRUM;
    }
    {
        b32 prima  = _aurum_probare(piscina, radix,
            CRUSTA_ORACULUM_EXPECTATA,
            chorda_aedificator_spectare(expectata));
        b32 altera = _aurum_probare(piscina, radix,
            CRUSTA_ORACULUM_SANITAS,
            chorda_aedificator_spectare(sanitas));

        redde (prima && altera) ? ZEPHYRUM : I;
    }
}
