/* probatio_stml_html.c - emissio STML -> HTML (spec instrumenti
 * par. 5, ordo probationum par. 8)
 *
 * ORDO GRAVITATE: (1) lex spatii albi par. 5.9 - emissor liberos
 * solos ambulans 'x y' in 'xy' contrahit, defectus tacitus
 * verisimillimus; (2) relectio per html_lexema = oraculum secundum
 * GENERIS ALIENI (non a scriptore ipso factum); (3) recusationes
 * singulae; (4) clausura per leges HTML; (5) attributa (effugium
 * duplex); (6) catena macrorum tota. Decretum singulariorum
 * (emittuntur ut scripta) pinnatur - ne quis tabulam recusationis
 * readdat. */
#include "latina.h"
#include "stml.h"
#include "stml_macros.h"
#include "stml_html.h"
#include "html_lexema.h"
#include "credo.h"
#include "chorda.h"
#include "piscina.h"
#include "xar.h"
#include "internamentum.h"
#include <stdio.h>
#include <string.h>

/* legere -> vertere (sine catena - pro documentis sine macris) */
interior StmlHtmlResultus
_vertere_litteras (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* fons)
{
        StmlResultus lectum;
    StmlHtmlResultus vacuus;

    lectum = stml_legere_ex_literis(fons, piscina, intern);
    si (!lectum.successus)
    {
        vacuus.successus     = FALSUM;
        vacuus.html.datum    = NIHIL;
        vacuus.html.mensura  = ZEPHYRUM;
        vacuus.vitium        = STML_HTML_MEMORIA;
        vacuus.detail        = vacuus.html;
        redde vacuus;
    }
    redde stml_html_vertere(lectum.radix, piscina);
}

/* catena tota: legere -> expandere -> distribuere -> vertere */
interior StmlHtmlResultus
_vertere_catena (
                Piscina* piscina,
    InternamentumChorda* intern,
     constans character* fons)
{
               StmlResultus lectum;
       StmlExpansioResultus expansio;
    StmlDistributioResultus distributio;
           StmlHtmlResultus vacuus;

    vacuus.successus     = FALSUM;
    vacuus.html.datum    = NIHIL;
    vacuus.html.mensura  = ZEPHYRUM;
    vacuus.vitium        = STML_HTML_MEMORIA;
    vacuus.detail        = vacuus.html;

    lectum = stml_legere_ex_literis(fons, piscina, intern);
    si (!lectum.successus)
    {
        redde vacuus;
    }
    expansio = stml_expandere(lectum.radix, piscina, intern);
    si (!expansio.successus)
    {
        redde vacuus;
    }
    distributio = stml_distribuere(expansio.radix_expansa, piscina,
                                   intern);
    si (!distributio.successus)
    {
        redde vacuus;
    }
    redde stml_html_vertere(distributio.radix_distributa, piscina);
}

s32
principale (
    vacuum)
{
                Piscina* piscina;
    InternamentumChorda* intern;

    piscina = piscina_generare_dynamicum("probatio_stml_html",
                                         1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    /* --- (1) LEX SPATII ALBI par. 5.9: spatium sine linea nova =
     * nodus TEXTUS; cum linea nova = trivia. AMBO 'x y' reddant --- */
    {
        StmlHtmlResultus res;

        imprimere("\n--- par. 5.9: spatium album ---\n");
        res = _vertere_litteras(piscina, intern,
            "<r><a>x</a> <b>y</b></r>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (res.html,
                "<!DOCTYPE html>\n<r><a>x</a> <b>y</b></r>");
        }
        res = _vertere_litteras(piscina, intern,
            "<r><a>x</a>\n<b>y</b></r>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (res.html,
                "<!DOCTYPE html>\n<r><a>x</a>\n<b>y</b></r>");
        }
    }

    /* --- (2) RELECTIO: oraculum secundum generis alieni -
     * html_lexema emissionem lexat; genus PRAVUM/IMPERFECTUM
     * nullum, et fidelitas lexatoris emissionem reddit --- */
    {
        StmlHtmlResultus res;

        imprimere("\n--- par. 8.3: relectio per html_lexema ---\n");
        res = _vertere_litteras(piscina, intern,
            "<html lang=\"la\"><head><title>T &amp; U</title>"
            "<style!>b { color: #ddd; }</style>"
            "<script!>if (a<b && c) { x(\"s\"); }</script></head>"
            "<body><h1>Salve</h1><!-- nota --><img alt=\"\"/>"
            "<p>a &lt; b</p></body></html>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            Xar* lexemata;
            i32  i;
            i32  num;
            b32  sanum = VERUM;

            lexemata = html_lexare(piscina,
                                   (constans character*)
                                       res.html.datum,
                                   res.html.mensura);
            CREDO_NON_NIHIL (lexemata);
            si (lexemata)
            {
                num = xar_numerus(lexemata);
                CREDO_VERUM (num > ZEPHYRUM);
                per (i = ZEPHYRUM; i < num; i++)
                {
                    HtmlLexema* lexema = (HtmlLexema*)
                        xar_obtinere(lexemata, i);
                    constans character* titulus =
                        html_lexema_genus_nomen(lexema->genus);

                    si (   strstr(titulus, "PRAVUM")    != NIHIL
                        || strstr(titulus, "IMPERFECT") != NIHIL)
                    {
                        sanum = FALSUM;
                        imprimere("  lexema malum: %s\n", titulus);
                    }
                }
                CREDO_VERUM (sanum);
                CREDO_CHORDA_AEQUALIS (
                    html_lexemata_emittere(piscina, lexemata),
                    res.html);
            }
        }
    }

    /* --- (3) RECUSATIONES: quaeque vitium suum nominat --- */
    {
        StmlHtmlResultus res;

        imprimere("\n--- par. 5: recusationes ---\n");

        res = _vertere_litteras(piscina, intern,
            "<?xml version=\"1.0\"?><r/>");
        CREDO_FALSUM (res.successus);
        CREDO_AEQUALIS_I32 ((i32)res.vitium,
                            (i32)STML_HTML_PROCESSIO);

        res = _vertere_litteras(piscina, intern,
            "<r><<sel>></r>");
        CREDO_FALSUM (res.successus);
        CREDO_AEQUALIS_I32 ((i32)res.vitium,
                            (i32)STML_HTML_TRANSCLUSIO);

        res = _vertere_litteras(piscina, intern,
            "<res><% &c;><nota>x</nota></%></res>");
        CREDO_FALSUM (res.successus);
        CREDO_AEQUALIS_I32 ((i32)res.vitium,
                            (i32)STML_HTML_AUGMENTATIO);

        res = _vertere_litteras(piscina, intern,
            "<r><.x>y</.x></r>");
        CREDO_FALSUM (res.successus);
        CREDO_AEQUALIS_I32 ((i32)res.vitium,
                            (i32)STML_HTML_TITULUS_PUNCTATUS);

        res = _vertere_litteras(piscina, intern, "< >x</>");
        CREDO_FALSUM (res.successus);
        CREDO_AEQUALIS_I32 ((i32)res.vitium,
                            (i32)STML_HTML_TITULUS_NIHIL);

        res = _vertere_litteras(piscina, intern,
            "<r><br>x</br></r>");
        CREDO_FALSUM (res.successus);
        CREDO_AEQUALIS_I32 ((i32)res.vitium,
                            (i32)STML_HTML_VACUUM_CUM_LIBERIS);

        res = _vertere_litteras(piscina, intern,
            "<r><script>x</script></r>");
        CREDO_FALSUM (res.successus);
        CREDO_AEQUALIS_I32 ((i32)res.vitium,
                            (i32)STML_HTML_CRUDUS_DEEST);
        CREDO_VERUM (chorda_aequalis_literis(res.detail,
                                             "script"));

        /* '&nbsp;' et '&amp;nbsp;' arborem EANDEM dant - figura
         * ipsa recusatur, ens nominatur (par. 5.10) */
        res = _vertere_litteras(piscina, intern,
            "<r>a&nbsp;b</r>");
        CREDO_FALSUM (res.successus);
        CREDO_AEQUALIS_I32 ((i32)res.vitium,
                            (i32)STML_HTML_ENS_AMBIGUUM);
        CREDO_VERUM (chorda_aequalis_literis(res.detail,
                                             "&nbsp;"));

        /* 'Tom & Jerry' autem transit - solum figura ambigua */
        res = _vertere_litteras(piscina, intern,
            "<r>Tom & Jerry</r>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (res.html,
                "<!DOCTYPE html>\n<r>Tom &amp; Jerry</r>");
        }

        /* '<a href>' -> href="true" nexus vivus ad plagulam
         * 'true' esset (par. 5.11) */
        res = _vertere_litteras(piscina, intern,
            "<r><a href=\"#\" x/></r>");
        CREDO_FALSUM (res.successus);
        CREDO_AEQUALIS_I32 ((i32)res.vitium,
                            (i32)STML_HTML_ATTRIBUTUM_VERUM);
        CREDO_VERUM (chorda_aequalis_literis(res.detail, "x"));

        /* elementum attributi extra praefixum = anomalum post
         * expansionem (par. 5.4) - arbor manualis, nam parsator
         * positionem iam ipse iudicat (par. 6.3) */
        {
                   StmlNodus* radix;
                   StmlNodus* solutum;
            StmlHtmlResultus  manu;

            radix = stml_elementum_creare(piscina, intern, "r");
            CREDO_NON_NIHIL (radix);
            stml_liberum_addere(radix,
                stml_textum_creare(piscina, intern, "x"));
            solutum = stml_elementum_creare(piscina, intern, "@");
            CREDO_NON_NIHIL (solutum);
            si (radix && solutum)
            {
                solutum->attributum_titulus =
                    chorda_internare(intern,
                                     chorda_ex_literis("m",
                                                       piscina));
                stml_liberum_addere(radix, solutum);
                manu = stml_html_vertere(radix, piscina);
                CREDO_FALSUM (manu.successus);
                CREDO_AEQUALIS_I32 ((i32)manu.vitium,
                    (i32)STML_HTML_ATTRIBUTUM_SOLUTUM);
            }
        }
    }

    /* --- (4) CLAUSURA per leges HTML: vacua nuda, cetera paria
     * semper - '<div/>' navigatori div APERTUS esset --- */
    {
        StmlHtmlResultus res;

        imprimere("\n--- par. 5.3: clausura ---\n");
        res = _vertere_litteras(piscina, intern,
            "<r><br/><hr/><div/><meta charset=\"utf-8\"/></r>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (res.html,
                "<!DOCTYPE html>\n"
                "<r><br><hr><div></div><meta charset=\"utf-8\">"
                "</r>");
        }
    }

    /* --- (5) ATTRIBUTA: effugium DUPLEX (par. 5.4) - inscripta
     * CRUDA transeunt, elementa attributorum decocta evadunt;
     * sepulcrum omittit; alt="" servatur; booleanum nudum --- */
    {
        StmlHtmlResultus res;

        imprimere("\n--- par. 5.4: attributa ---\n");

        /* inscriptum CRUDUM: octeti '&amp;' ut sunt (effugium
         * iteratum '&amp;amp;' esset) */
        res = _vertere_litteras(piscina, intern,
            "<r><a x=\"a&amp;b\"/></r>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (res.html,
                "<!DOCTYPE html>\n<r><a x=\"a&amp;b\"></a></r>");
        }

        /* elementum attributi DECOCTUM: evade '&' '"' */
        res = _vertere_litteras(piscina, intern,
            "<r><a><@t=>x &amp; \"y\"</></a></r>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (res.html,
                "<!DOCTYPE html>\n"
                "<r><a t=\"x &amp; &quot;y&quot;\"></a></r>");
        }

        /* sepulcrum: nomen omittitur; alt="" NON omittitur
         * (onus lectorum auditorum fert) */
        res = _vertere_litteras(piscina, intern,
            "<r><a><@t=/></a><img alt=\"\"/></r>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (res.html,
                "<!DOCTYPE html>\n"
                "<r><a></a><img alt=\"\"></r>");
        }

        /* booleanum notum: forma nuda */
        res = _vertere_litteras(piscina, intern,
            "<r><input disabled/></r>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (res.html,
                "<!DOCTYPE html>\n<r><input disabled></r>");
        }
    }

    /* --- (6) CATENA TOTA: templum + distributio + crudum; et
     * SINGULARIA EMITTUNTUR UT SCRIPTA (decretum 2026-09-01) --- */
    {
        StmlHtmlResultus res;

        imprimere("\n--- par. 6: catena macrorum ---\n");
        res = _vertere_catena(piscina, intern,
            "<html><body>"
            "<#@c t=\"@t\"><p>&@t;</p></#>"
            "<<#@c t=\"salve\">>"
            "<ul><li><>a</><>b</></li></ul>"
            "</body></html>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (res.html,
                "<!DOCTYPE html>\n"
                "<html><body><p>salve</p>"
                "<ul><li>a</li><li>b</li></ul></body></html>");
        }

        /* decretum singulariorum: corpus geminum EMITTITUR -
         * emissor validitatem HTML numquam iudicat */
        res = _vertere_catena(piscina, intern,
            "<html><body><>x</><>y</></body></html>");
        CREDO_VERUM (res.successus);
        si (res.successus)
        {
            CREDO_CHORDA_AEQUALIS_LITERIS (res.html,
                "<!DOCTYPE html>\n"
                "<html><body>x</body><body>y</body></html>");
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    si (credo_omnia_praeterierunt())
    {
        credo_claudere();
        redde ZEPHYRUM;
    }
    credo_claudere();
    redde I;
}
