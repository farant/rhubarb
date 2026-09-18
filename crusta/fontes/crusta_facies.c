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
