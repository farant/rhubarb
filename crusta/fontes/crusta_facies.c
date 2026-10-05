/* crusta_facies.c - Vide crusta_facies.h. */

#include "crusta_facies.h"
#include "crusta_arbor.h"
#include "crusta_diagnostica.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "materia_pictor.h"
#include "iter_directoria.h"
#include "stml.h"
#include "chorda_aedificator.h"
#include "crusta_effectus.h"
#include "materia_annotationes.h"
#include "materia_exemplaria.h"
#include "materia_excusatio.h"
#include "stml_macros.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ==================================================
 * Regulae ex directorio
 * ================================================== */

/* causa cum re nominata ('quid: qua re') */
interior constans character*
_causa (
               Piscina* piscina,
    constans character* nuntius,
    constans character* res)
{
    memoriae_index  mensura = strlen(nuntius) + strlen(res) + IV;
         character* c;

    c = (character*)piscina_allocare(piscina, mensura);
    si (c == NIHIL)
    {
        redde nuntius;
    }
    sprintf(c, "%s: %s", nuntius, res);
    redde c;
}

interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* via,
                   i32* mensura)
{
         FILE* f;
        longus longitudo;
    character* memoria;
        size_t lecti;

    f = fopen(via, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    si (fseek(f, 0L, SEEK_END) != ZEPHYRUM)
    {
        fclose(f);
        redde NIHIL;
    }
    longitudo = ftell(f);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
    rewind(f);
    memoria = (character*)piscina_allocare(piscina,
        (memoriae_index)longitudo + I);
    si (memoria == NIHIL)
    {
        fclose(f);
        redde NIHIL;
    }
    lecti = fread(memoria, I, (size_t)longitudo, f);
    fclose(f);
    memoria[lecti]  = '\0';
    *mensura        = (i32)lecti;
    redde memoria;
}

interior b32
_stml_est (
    constans character* titulus)
{
    memoriae_index n = strlen(titulus);

    redde (b32)(n > V && strcmp(titulus + n - V, ".stml") == ZEPHYRUM);
}

/* directorium + '/' + titulus (chorda NON NUL-terminata est) */
interior character*
_viam_iungere (
                     Piscina* piscina,
          constans character* directorium,
             constans chorda* titulus)
{
    memoriae_index  nd = strlen(directorium);
         character* via;

    via = (character*)piscina_allocare(piscina,
        nd + (memoriae_index)titulus->mensura + II);
    si (via == NIHIL)
    {
        redde NIHIL;
    }
    memcpy(via, directorium, nd);
    via[nd] = '/';
    memcpy(via + nd + I, titulus->datum,
        (memoriae_index)titulus->mensura);
    via[nd + I + (memoriae_index)titulus->mensura] = '\0';
    redde via;
}

/* Vocans explicitus, deinde ambitus, deinde mos. Vide crusta_facies.h:
 * via ordinaria RELATIVA est, ergo instrumentum aliunde curritur
 * regulas invenire non posset sine ambitu. */
constans character*
crusta_lintrum_eligere (
    constans CrustaOptiones* optiones)
{
    constans character* ambitus;

    si (optiones != NIHIL && optiones->lintrum != NIHIL)
    {
        redde optiones->lintrum;
    }
    ambitus = getenv(CRUSTA_LINTRUM_AMBITUS);
    si (ambitus != NIHIL && ambitus[ZEPHYRUM] != '\0')
    {
        redde ambitus;
    }
    redde CRUSTA_LINTRUM;
}

Xar*
crusta_regulae_legere (
                Piscina*  piscina,
     constans character*  directorium,
    InternamentumChorda*  intern,
     constans character** causa)
{
     DirectoriumIterator* iter;
    DirectoriumIntroitus* introitus;
                     Xar* viae;
                     Xar* exitus;
                     i32  k;

    si (causa != NIHIL)
    {
        *causa = NIHIL;
    }
    si (piscina == NIHIL || directorium == NIHIL || intern == NIHIL)
    {
        redde NIHIL;
    }
    viae    = xar_creare(piscina, (i32)magnitudo(character*));
    exitus  = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
    si (viae == NIHIL || exitus == NIHIL)
    {
        redde NIHIL;
    }
    iter = directorium_iterator_aperire(directorium, piscina);
    si (iter == NIHIL)
    {
        si (causa != NIHIL)
        {
            *causa = _causa(piscina,
                "directorium regularum aperiri non potest",
                directorium);
        }
        redde NIHIL;
    }
    dum ((introitus = directorium_iterator_proximum(iter)) != NIHIL)
    {
        character* via;

        si (introitus->genus != INTROITUS_FILUM)
        {
            perge;
        }
        via = _viam_iungere(piscina, directorium, &introitus->titulus);
        si (via == NIHIL)
        {
            directorium_iterator_claudere(iter);
            redde NIHIL;
        }
        si (_stml_est(via))
        {
            character** cella = (character**)xar_addere(viae);

            si (cella == NIHIL)
            {
                directorium_iterator_claudere(iter);
                redde NIHIL;
            }
            *cella = via;
        }
    }
    directorium_iterator_claudere(iter);
    /* ORDO TITULORUM: ordo quem systema fert stabilis non est, et
     * ordo regularum ordinem ordinum aequalium determinat. */
    per (k = (i32)I; k < xar_numerus(viae); k++)
    {
         character* hic  = *(character**)xar_obtinere(viae, k);
               i32  j    = k;

        dum (j > ZEPHYRUM)
        {
            character** prior = (character**)xar_obtinere(viae,
                                    j - (i32)I);

            si (strcmp(*prior, hic) <= ZEPHYRUM)
            {
                frange;
            }
            *(character**)xar_obtinere(viae, j) = *prior;
            j--;
        }
        *(character**)xar_obtinere(viae, j) = hic;
    }
    per (k = ZEPHYRUM; k < xar_numerus(viae); k++)
    {
        constans character*  via = *(character**)xar_obtinere(viae, k);
                 character*  textus;
                       i32   mensura = ZEPHYRUM;
              StmlResultus   r;
                 StmlNodus** cella;

        textus = _plagulam_legere(piscina, via, &mensura);
        si (textus == NIHIL)
        {
            si (causa != NIHIL)
            {
                *causa = _causa(piscina, "regula legi non potest", via);
            }
            redde NIHIL;
        }
        r = stml_legere(chorda_ex_buffer((i8*)textus, mensura), piscina,
                intern);
        si (!r.successus || r.radix == NIHIL)
        {
            si (causa != NIHIL)
            {
                *causa = _causa(piscina, "regula fracta", via);
            }
            redde NIHIL;
        }
        cella = (StmlNodus**)xar_addere(exitus);
        si (cella == NIHIL)
        {
            redde NIHIL;
        }
        *cella = r.radix;
    }
    redde exitus;
}


/* ==================================================
 * Facies
 * ================================================== */

Xar*
crusta_diagnostica_omnia (
                Piscina*  piscina,
     constans character*  fons,
                    i32   mensura,
constans CrustaOptiones*  optiones,
     constans character** causa)
{
        MateriaLexiconRatum ratum;
         MateriaLexIudicium iudicium;
    MateriaDiagnosticaRatio ratio;
              CrustaParsura relatio;
               MateriaNodus* radix;
        InternamentumChorda* intern;
                        Xar* regulae;

    si (causa != NIHIL)
    {
        *causa = NIHIL;
    }
    si (piscina == NIHIL || fons == NIHIL)
    {
        redde NIHIL;
    }
    si (!materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
            &iudicium))
    {
        si (causa != NIHIL)
        {
            *causa = "lexicon crustae ratum fieri non potest";
        }
        redde NIHIL;
    }
    intern = optiones != NIHIL && optiones->intern != NIHIL
        ? optiones->intern : internamentum_creare(piscina);
    si (intern == NIHIL)
    {
        redde NIHIL;
    }
    /* REGULAE: datae, aut quidquid in lintro iacet. Materia nihil
     * legit - quae plagulae et ubi sitae res CLIENTIS est. */
    regulae = optiones != NIHIL && optiones->regulae != NIHIL
        ? optiones->regulae
        : crusta_regulae_legere(piscina,
              crusta_lintrum_eligere(optiones), intern, causa);
    si (regulae == NIHIL)
    {
        redde NIHIL;
    }
    radix = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
                &relatio);
    si (radix == NIHIL)
    {
        si (causa != NIHIL)
        {
            *causa = "arbor crustae aedificari non potest";
        }
        redde NIHIL;
    }
    memset(&ratio, ZEPHYRUM, magnitudo(ratio));
    ratio.grammatica  = "crusta";
    ratio.tabularium  = &CRUSTA_REGISTRUM;
    ratio.lexicon     = &ratum;
    ratio.declarata   = &CRUSTA_DIAGNOSTICA;
    /* commentaria crustae '#' ferunt, ergo praefixum solum sufficit */
    ratio.praefixum   = optiones != NIHIL && optiones->sine_excusatione
                            ? NIHIL : "#";
    ratio.regulae  = regulae;
    ratio.intern   = intern;
    ratio.crudum   = optiones != NIHIL && optiones->crudum;
    /* 'emissa': quod arbor non servat, solus parsator scit */
    redde materia_diagnostica_plena(piscina, radix, &ratio,
        relatio.diagnostica, causa);
}

chorda
crusta_diagnostica_textus (
                Piscina*  piscina,
     constans character*  via,
     constans character*  fons,
                    i32   mensura,
                    b32   excerptum,
constans CrustaOptiones*  optiones,
     constans character** causa)
{
    ChordaAedificator* a;
                  Xar* d;
                  i32  k;
               chorda  vacua;

    memset(&vacua, ZEPHYRUM, magnitudo(vacua));
    d = crusta_diagnostica_omnia(piscina, fons, mensura, optiones,
            causa);
    si (d == NIHIL)
    {
        redde vacua;
    }
    a = chorda_aedificator_creare(piscina, (memoriae_index)CCLVI);
    si (a == NIHIL)
    {
        redde vacua;
    }
    per (k = ZEPHYRUM; k < xar_numerus(d); k++)
    {
        chorda textus = materia_pictor_scribere(piscina,
            (constans MateriaDiagnosticum*)xar_obtinere(d, k), via,
            "crusta", fons, mensura, excerptum);

        si (!chorda_aedificator_appendere_chorda(a, textus))
        {
            redde vacua;
        }
    }
    redde chorda_aedificator_finire(a);
}


/* ==================================================
 * Effectus: regulae super summarium plagulae (effectus-plan T6)
 * ================================================== */

/* situs (elementum summarii) in documentum novum copiare: titulus et
 * attributa; summarium ipsum immotum manet */
interior StmlNodus*
_situm_copiare (
                Piscina* piscina,
    InternamentumChorda* intern,
              StmlNodus* situs)
{
     StmlNodus* novus;
           i32  k;

    novus = stml_elementum_creare(piscina, intern,
        chorda_ut_cstr(*situs->titulus, piscina));
    si (novus == NIHIL)
    {
        redde NIHIL;
    }
    per (k = ZEPHYRUM; situs->attributa
                       && k < xar_numerus(situs->attributa); k++)
    {
        StmlAttributum* a = (StmlAttributum*)xar_obtinere(
            situs->attributa, k);

        si (!stml_attributum_addere_chorda(novus, piscina, intern,
                chorda_ut_cstr(*a->titulus, piscina), *a->valor))
        {
            redde NIHIL;
        }
    }
    redde novus;
}

Xar*
crusta_effectus_diagnostica (
                Piscina*  piscina,
     constans character*  via,
     constans character*  fons,
                    i32   mensura,
                    Xar*  summaria,
                    b32   in_catena,
constans CrustaOptiones*  optiones,
     constans character** causa)
{
        MateriaLexiconRatum ratum;
         MateriaLexIudicium iudicium;
              CrustaParsura relatio;
               MateriaNodus* radix;
        InternamentumChorda* intern;
                 StmlNodus* documentum;
                 StmlNodus* processus;
                        Xar* exitus;
                        Xar* annotationes = NIHIL;
                        i32  i;
                        i32  j;
                        i32  k;

    si (causa != NIHIL)
    {
        *causa = NIHIL;
    }
    si (   piscina           == NIHIL || via == NIHIL || fons == NIHIL
        || summaria          == NIHIL || optiones == NIHIL
        || optiones->regulae == NIHIL || optiones->intern == NIHIL)
    {
        redde NIHIL;
    }
    intern = optiones->intern;
    si (!materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
            &iudicium))
    {
        redde NIHIL;
    }
    /* documentum UNUM per plagulam (exemplaria: sedes clavis est):
     * situs huius plagulae ex summariis omnibus */
    documentum  = stml_elementum_creare(piscina, intern, "effectus");
    processus   = stml_elementum_creare(piscina, intern, "processus");
    si (   documentum == NIHIL || processus == NIHIL
        || !stml_attributum_addere(documentum, piscina, intern,
        "lingua",
               "bash")
        || !stml_attributum_addere(documentum, piscina, intern, "radix",
               via)
        || !stml_attributum_addere(processus, piscina, intern, "radix",
               via)
        || !stml_liberum_addere(documentum, processus))
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(summaria); i++)
    {
        StmlNodus* sm = *(StmlNodus**)xar_obtinere(summaria, i);

        per (j = ZEPHYRUM; sm != NIHIL && sm->liberi
                           && j < xar_numerus(sm->liberi); j++)
        {
            StmlNodus* pr = *(StmlNodus**)xar_obtinere(sm->liberi, j);

            per (k = ZEPHYRUM; pr->liberi
                && k < xar_numerus(pr->liberi);
                 k++)
            {
                StmlNodus* s = *(StmlNodus**)xar_obtinere(pr->liberi,
                    k);
                    chorda* p;
                 StmlNodus* copia;

                si (s->genus != STML_NODUS_ELEMENTUM)
                {
                    perge;
                }
                p = stml_attributum_capere(s, "plagula");
                si (p == NIHIL || !chorda_aequalis_literis(*p, via))
                {
                    perge;
                }
                /* parsura plagulae IPSIUS fracta: gradus I crustae
                 * eam iam nuntiat - non iteratur */
                si (   chorda_aequalis_literis(*s->titulus, "ignotum")
                    && stml_attributum_capere(s, "mandatum") == NIHIL
                    && stml_attributum_capere(s, "causa")    != NIHIL
                    && (chorda_aequalis_literis(
                            *stml_attributum_capere(s, "causa"),
                            "parsura non sana")
                        || chorda_aequalis_literis(
                            *stml_attributum_capere(s, "causa"),
                            "illegibilis")))
                {
                    perge;
                }
                copia = _situm_copiare(piscina, intern, s);
                si (   copia == NIHIL
                    || !stml_liberum_addere(processus, copia))
                {
                    redde NIHIL;
                }
            }
        }
    }
    exitus = xar_creare(piscina, (i32)magnitudo(MateriaDiagnosticum));
    si (exitus == NIHIL)
    {
        redde NIHIL;
    }
    per (i = ZEPHYRUM; i < xar_numerus(optiones->regulae); i++)
    {
        StmlNodus* regula =
            *(StmlNodus**)xar_obtinere(optiones->regulae,
            i);
           StmlNodus* compositum;
StmlExpansioResultus  expansio;
                 Xar* ordines;

        compositum = materia_exemplaria_componere(piscina, documentum,
            regula, intern);
        si (compositum == NIHIL)
        {
            redde NIHIL;
        }
        expansio = stml_expandere(compositum, piscina, intern);
        si (!expansio.successus || expansio.radix_expansa == NIHIL)
        {
            si (causa != NIHIL)
            {
                *causa = "regula effectus expandi non potest";
            }
            redde NIHIL;
        }
        ordines = materia_exemplaria_extrahere(piscina,
            expansio.radix_expansa);
        si (ordines != NIHIL && !optiones->crudum)
        {
            ordines = materia_exemplaria_minuere(piscina,
                expansio.radix_expansa, ordines, causa);
        }
        si (ordines == NIHIL)
        {
            redde NIHIL;
        }
        per (j = ZEPHYRUM; j < xar_numerus(ordines); j++)
        {
            MateriaDiagnosticum* d = (MateriaDiagnosticum*)xar_addere(
                exitus);

            *d = *(MateriaDiagnosticum*)xar_obtinere(ordines, j);
            /* extra catenas verdicti: monitum (spec par. V.3, A4) */
            si (   !in_catena
                && d->gravitas == (s32)MATERIA_GRAVITAS_ERRATUM)
            {
                d->gravitas = (s32)MATERIA_GRAVITAS_MONITUM;
            }
        }
    }
    /* excusationes ex commentariis plagulae ipsius ('#'), lex eadem */
    si (!optiones->sine_excusatione)
    {
        radix = crusta_arbor_parsare(piscina, fons, mensura,
            &CRUSTA_BASH, &relatio);
        si (radix != NIHIL)
        {
            Xar* omnes = materia_annotationes_colligere(piscina, radix,
                &ratum, "#", NIHIL, intern);

            /* SOLAE excusationes effectuum: ceterae (gradus I crustae,
             * lintra crustae) in cursu crustae iudicantur - hic mortuae
             * aut sine causa BIS nominarentur */
            annotationes = xar_creare(piscina,
                (i32)magnitudo(MateriaAnnotatio));
            per (k = ZEPHYRUM; omnes != NIHIL && annotationes != NIHIL
                               && k < xar_numerus(omnes); k++)
            {
                MateriaAnnotatio* a = (MateriaAnnotatio*)xar_obtinere(
                    omnes, k);

                si (chorda_invenire_index(a->textus, chorda_ex_literis(
                        "codex=\"lint:effectus-", piscina)) >= ZEPHYRUM)
                {
                    *(MateriaAnnotatio*)xar_addere(annotationes) = *a;
                }
            }
        }
    }
    redde materia_excusatio_applicare(piscina, exitus, annotationes,
        &CRUSTA_DIAGNOSTICA, "crusta",
        materia_exemplaria_lintres(piscina, optiones->regulae));
}
