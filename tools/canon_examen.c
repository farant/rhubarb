/* canon_examen.c - plagulas STML contra canonem iudicare
 *
 * Usus:
 *   bin/canon_examen <plagula.stml> ...        canon per registrum
 *   bin/canon_examen -canon X.canon <plagula>  canon expressus
 *   bin/canon_examen -machina ...              TSV purum
 *
 * Exitus: 0 = sanum; 1 = VITIA; 2 = NIHIL IUDICATUM EST
 *   (disciplina domus: instrumentum quod nihil iudicavit successus
 *    non est - plagula sine canone TACERE non debet)
 */
#include "postulata_posix.h"

#include "latina.h"
#include "canon.h"
#include "filum.h"
#include <stdio.h>
#include <string.h>

#define REGISTRUM "canones.registrum"

interior constans character* vitium_campus(CanonVitium* v,
                                           character* buffer,
                                           i32 tectum);

/* campus 'detail' cum limitibus cardinalitatis */
interior constans character*
vitium_campus(
    CanonVitium* v,
    character*   buffer,
    i32          tectum)
{
    si (v->genus == CANON_LIBERI_PAUCI ||
        v->genus == CANON_LIBERI_MULTI)
    {
        sprintf(buffer, "%.*s (%u, limes %u)",
                v->detail ? (integer)v->detail->mensura : 1,
                v->detail ? (constans character*)v->detail->datum
                          : "-",
                v->numerus, v->limes);
        redde buffer;
    }

    si (!v->detail)
    {
        redde "-";
    }
    {
        i32 m;

        m = v->detail->mensura < tectum - I ? v->detail->mensura
                                            : tectum - I;
        memcpy(buffer, v->detail->datum, (memoriae_index)m);
        buffer[m] = '\0';
    }

    redde buffer;
}

s32
principale(
    s32          numerus,
    character**  argumenta)
{
    Piscina*             piscina;
    InternamentumChorda* intern;
    chorda               catalogus;
    constans character*  canon_expressus;
    b32                  machina;
    s32                  i;
    i32                  iudicatae;
    i32                  vitia_summa;

    canon_expressus = NIHIL;
    machina         = FALSUM;
    iudicatae       = ZEPHYRUM;
    vitia_summa     = ZEPHYRUM;

    piscina = piscina_generare_dynamicum("canon_examen", 4194304);
    intern  = internamentum_creare(piscina);

    catalogus = filum_legere_totum(REGISTRUM, piscina);

    per (i = I; i < numerus; i++)
    {
        constans character* via;
        chorda              via_canonis;
        chorda              fons;
        chorda              fons_canonis;
        Canon*              canon;
        chorda              causa;
        StmlResultus        r;
        Xar*                vitia;
        i32                 j;

        si (strcmp(argumenta[i], "-machina") == ZEPHYRUM)
        {
            machina = VERUM;
            perge;
        }
        si (strcmp(argumenta[i], "-canon") == ZEPHYRUM &&
            i + I < numerus)
        {
            canon_expressus = argumenta[++i];
            perge;
        }

        via = argumenta[i];

        fons = filum_legere_totum(via, piscina);
        si (fons.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "canon_examen: '%s' legi nequit\n", via);
            perge;
        }
        r = stml_legere(fons, piscina, intern);
        si (!r.successus)
        {
            fprintf(stderr, "canon_examen: '%s' parsari nequit "
                            "(gradus I, ante canonem)\n", via);
            vitia_summa++;
            perge;
        }

        /* canonem invenire: RADIX VINCIT, extensio cadit.
         * '.stml' quattuor dialectos fert, ergo extensio illis
         * nihil dicit; elementum radicis dialectum semper nominat */
        si (canon_expressus)
        {
            via_canonis = chorda_ex_literis(canon_expressus,
                                            piscina);
        }
        alioquin
        {
            via_canonis.datum   = NIHIL;
            via_canonis.mensura = ZEPHYRUM;
            si (r.elementum_radix && r.elementum_radix->titulus)
            {
                via_canonis = canon_registrum_quaerere_radice(
                    catalogus, r.elementum_radix->titulus, piscina);
            }
            si (via_canonis.mensura == ZEPHYRUM)
            {
                via_canonis = canon_registrum_quaerere(catalogus,
                                                       via, piscina);
            }
        }
        si (via_canonis.mensura == ZEPHYRUM)
        {
            fprintf(stderr,
                "canon_examen: '%s' canonem in %s non habet - "
                "NIHIL iudicatum\n", via, REGISTRUM);
            perge;
        }

        fons_canonis = filum_legere_totum(
            chorda_ut_cstr(via_canonis, piscina), piscina);
        si (fons_canonis.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "canon_examen: canon '%.*s' legi nequit\n",
                    (integer)via_canonis.mensura,
                    (constans character*)via_canonis.datum);
            perge;
        }

        canon = canon_legere(fons_canonis, piscina, intern, &causa);
        si (!canon)
        {
            fprintf(stderr, "canon_examen: canon '%.*s' fractus: %.*s\n",
                    (integer)via_canonis.mensura,
                    (constans character*)via_canonis.datum,
                    (integer)causa.mensura,
                    (constans character*)causa.datum);
            perge;
        }

        vitia = canon_iudicare(canon, r.radix, piscina);
        iudicatae++;

        per (j = ZEPHYRUM; j < xar_numerus(vitia); j++)
        {
            CanonVitium* v;
            character    buffer[DXII];

            v = (CanonVitium*)xar_obtinere(vitia, j);
            vitia_summa++;

            si (machina)
            {
                imprimere("%s\t%u\t%.*s\t%s\t%s\n", via,
                    (i32)v->genus,
                    v->elementum ? (integer)v->elementum->mensura : 1,
                    v->elementum
                        ? (constans character*)v->elementum->datum
                        : "-",
                    vitium_campus(v, buffer, (i32)magnitudo(buffer)),
                    canon_nuntius(v->genus));
            }
            alioquin
            {
                imprimere("%s: <%.*s> %s: %s\n", via,
                    v->elementum ? (integer)v->elementum->mensura : 1,
                    v->elementum
                        ? (constans character*)v->elementum->datum
                        : "-",
                    canon_nuntius(v->genus),
                    vitium_campus(v, buffer,
                                  (i32)magnitudo(buffer)));
            }
        }
    }

    /* NIHIL IUDICATUM: non successus */
    si (iudicatae == ZEPHYRUM)
    {
        fprintf(stderr,
            "canon_examen: NULLA plagula iudicata est\n");
        redde II;
    }

    si (!machina)
    {
        imprimere("canon_examen: plagulae %u / VITIA %u\n",
                  iudicatae, vitia_summa);
    }

    redde vitia_summa > ZEPHYRUM ? I : ZEPHYRUM;
}
