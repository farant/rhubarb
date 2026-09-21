/* probatio_crusta_stml.c - Circuitus STML crustae: duo cycli, duo
 * oracula, referentiae relatae (P7)
 *
 * scribere -> legere -> scribere: octeti STML bis idem (vitium quod
 * se componit circuitum unum saepe superat); arbor parsata contra
 * arborem relectam per COMPARATOREM (materia_arbor_aequalis,
 * STRUCTURALIS - dislocatio dominii triviorum octetim invisibilis
 * est, solus comparator eam videt); arbor RELECTA per scriptorem
 * octetorum emissa == fons (catena clausa una assertione). Casus
 * casuum portarum arboris et arithmeticae (P3-P5, verbatim), casus
 * fixorum per lectorem casuum, et corpus totum P6 (plagulae domus ex
 * build/crusta_corpus.lst, fixa totae, FreeBSD) - piscina sua quaeque
 * plagula.
 *
 * REFERENTIAE: redirectio heredoc corpus suum per referentiam nominat
 * (decretum 01M2NJ16RG). Textus proiectus '<corpus(> #nodN' et
 * 'id="nodN"' fert; in arbore relecta locus corpus REFERENTIA est
 * cuius scopus nodus heredoc RELECTUS (identitas per viam relativam,
 * numerus referentiarum idem utrimque).
 *
 * FIDELITAS (spec par. X, MENSURATA 2026-09-16): comparator modo
 * FIDELITAS byte_offset, lineam, columnam addit; lector eas cursore
 * reficit qui octetos valorum ipsos numerat, ergo lineae novae in
 * lexematibus substantivis (SEPARATOR_LINEAE, corpora heredoc) recte
 * cadunt. Tenet in documento QUOQUE (casus inlinei, fixa, corpus
 * totum) - ASSERTA (causa ARBOR_INFIDELIS). Solum vexillum
 * initium_lineae dissentiebat: lector id ex triviis muneris LINEA
 * derivat, aedificator id nunc eadem regula ponit
 * (_lexema_recordare; CCCXV plagulae corporis ante id dispares).
 *
 * LIMES SUBSTRATI ALTER (inventus P7, materia eodem die aucta): valor
 * lexematis cuius margo alba initialis aut finalis lineam novam fert,
 * in elemento MIXTO (trivia ferens, ergo non crudo), forma fugata
 * scribi non potest - lex dominii triviorum STML marginem illam
 * dispositioni dat et lector eam in valorem non reddit. Olim TACITE
 * octeti perdebantur ("a 'b " + linea nova: duo); nunc scriptor
 * RECUSAT ('valor lexematis textui non tutus (mixtum)', custodia
 * _textus_tutus aucta). Casus 'simplex-apertus' adversarii (apex
 * apertus ad finem plagulae) eam pinnat; in corpore vero apex ad
 * finem plagulae solum eam attingit.
 *
 * LIMES SUBSTRATI NOMINATUS (01M2KPJ0HW, ut in html): valor lexematis
 * qui sequentiam claudentem elementi sui fert ('</crusta-litteralis>'
 * intra verbum) a scriptore RECUSATUR ('valor sequentiam claudentem
 * fert'). Casus 'forma-cruda' adversarii (commentarium sequentiam
 * suam ferens; apices citati forma fugata transeunt) eam pinnat per
 * casum; plagula tota transit (contextus lexicalis constructorum
 * apertorum, vide portam corporis). Recusatio causa alia =
 * fractura. Cum materia recidat, pinnae rubent et casus in circuitum
 * promoventur.
 *
 * CAUDA LEXEMATIS: profunditas backtick in cauda privata
 * (CrustaCauda) vivit quam proiectio NON fert (frons nulla);
 * consilium formam eandem lectori dat ut lexemata relecta caudam
 * (ad zephyrum) habeant - octeti et structura integra, profunditas
 * sola non relata. Limes nominatus, consumptor nullus adhuc.
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_exempla.h"
#include "crusta_lector.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "materia_arbor.h"
#include "materia_sedes.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_scribere.h"
#include "materia_token.h"
#include "piscina.h"
#include "chorda.h"
#include "xar.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* VISIO SEDIUM (materia-sedes A2): elementa verificata per corpus -
 * porta quae nihil verificavit mortua est */
hic_manens i32 SEDES_ELEMENTA = ZEPHYRUM;
hic_manens i32 SEDES_DERIVATA = ZEPHYRUM;

/* casus inlinei: porta arboris (P3/P5: XC casus + mala VII) et porta
 * arithmeticae (P4: V) verbatim */
hic_manens constans character* CASUS[] = {
    "echo hi\n",
    "",
    "\"a\"b'c' d",
    "x=1 y=2 cmd a=b",
    "A=1 if",
    "X=1\nfor i in a; do :; done\n>f\necho",
    "declare -a x=(1 [3]=y)",
    "a= b+=c d[i]=v",
    "$x ${y:-d} $(echo z) \\$ ~/p $'\\t' $\"s\"",
    "${x/a/b}${x:1:2}${#x}${x}",
    "${x:-}",
    "\"$(echo \"in\")\"",
    "echo `echo \\`echo d\\`` x",
    "a$(b)c",
    "a $(b)",
    "cmd 2>&1 >/dev/null <in &>all {fd}>x",
    "> f",
    "cat <<A <<<w",
    "a; b & c\n",
    "a && b || c",
    "a | b |& c && d",
    "! a | b; time -p c",
    "a &&\n\n  b",
    "a # c\n",
    "#!/bin/bash\n\na",
    "a # c",
    "a \\\n b",
    "a ) b",
    "\"abc",
    "$(a",
    "`a",
    "echo $ $((1+1))",
    "'a b' \"a\\\"b\" a\\ b $'\\x41\\101' \\\\x \"\\x\"",
    "if a; then b; elif c; then d; else e; fi",
    "if a\nthen\n b\nfi",
    "if a; then { b; fi",
    "if",
    "if ((x)) then :; fi; if [[ a ]] then :; fi",
    "if a; then b; fi > f | c",
    "for i in a b; do c; done",
    "for i in a b\ndo c; done",
    "for i; do :; done",
    "for i do :; done",
    "for ((i=0;i<3;i++)); do :; done",
    "for ((;;)) do :; done",
    "select x in a; do :; done",
    "while :; do break; done",
    "until a; do b; done | c",
    "for i in a",
    "while true do :; done",
    "case $x in a|b) echo 1 ;; (c) ;; d) esac",
    "case x in a) ;& b) ;;& c) esac",
    "case x in\n  a)\n b\n ;;\nesac",
    "case x in $v*) ;; esac",
    "case x in a esac",
    "{ a; b; } > f",
    "( a ) 2>&1",
    "{ a }",
    "{ { a; } }",
    "f() { :; }",
    "function g { :; }",
    "function h () ( : )",
    "f() > x",
    "f() { :; } && x",
    "coproc c { :; }",
    "coproc cmd a",
    "time ls |& cat",
    "{ a; } x",
    "[[ $a == b* && -f $c || ! ( x =~ ^a(b|c)$ ) ]]",
    "[[ -n $x ]] && y",
    "[[ a ]]",
    "[[ $x =~ a|b'c d'$v ]]",
    "[[",
    "[[ a b ]]",
    "cat <<A\nl1\n$v\nA\nrest\n",
    "cat <<'A'\nl1\n$v\nA\n",
    "cat <<-A\n\tx\n\tA\n",
    "cat <<A <<B |\n1\nA\n2\nB\nwc",
    "cat <<A <<B | wc\n1\nA\n2\nB\n",
    "cat <<A &&\nx\nA\ny",
    "echo $(cat <<A\nx\nA\n)",
    "echo $(cat <<A)\nx\nA\n",
    "echo `cat <<A`\nx\nA\n",
    "cat <<A",
    "cat <<A \"x\ny\"\nb\nA\n",
    "read x <<EOF; for i in \"$x\"\nvalue\nEOF\ndo\n :\ndone\n",
    "read x <<EOF; for i\nvalue\nEOF\ndo :; done\n",
    "read x <<EOF; case $x\nvalue\nEOF\nin a) ;; esac\n",
    "for ((;;))\ndo :; done",
    "if cat <<A\nx\nA\nthen :; fi",
    /* mala portae arboris */
    "fi",
    ")",
    ";;",
    "}",
    "; ;",
    "then",
    "a )",
    /* porta arithmeticae (structura per aedificatorem) */
    "$(( ))",
    "(( i += 1 )) > f && x",
    "echo $((1+1)) $((echo a); b)",
    "$((1 +))",
    "$((1+1",
    /* P9b: corpora heredoc in lacunis (listae post_*,
     * functio.interiecta, gradus absens clausus) */
    "cat <<A; [[ a ==\nx\nA\nb ]]\n",
    "[[ -n $(cat <<A)\nx\nA\n]]\n",
    "cat <<A; [[ ( a\nx\nA\n) ]]\n",
    "cat <<A; (( a ? 1 :\nx\nA\n2 ))\n",
    "cat <<A; (( i\nx\nA\n++ ))\n",
    "cat <<A; (( -\nx\nA\n1 ))\n",
    "echo $(( $(cat <<A) +\nx\nA\n1))\n",
    "cat <<A; for ((i=0\nx\nA\n;;)); do :; done\n",
    "cat <<A; function f\nx\nA\n{ :; }\n",
    "cat <<A; for\nx\nA\ni in a; do :; done\n"
};

#define NUMERUS_CASUUM ((i32)(magnitudo(CASUS)/magnitudo(CASUS[0])))

/* casus cum corpore heredoc APERTO (nodus heredoc in arbore, ergo
 * referentia scripta) - petitio quaeque corpus accipit */
hic_manens constans character* CASUS_HEREDOC[] = {
    "cat <<A\nl1\n$v\nA\nrest\n",
    "cat <<'A'\nl1\n$v\nA\n",
    "cat <<-A\n\tx\n\tA\n",
    "cat <<A <<B |\n1\nA\n2\nB\nwc",
    "cat <<A <<B | wc\n1\nA\n2\nB\n",
    "cat <<A &&\nx\nA\ny",
    "echo $(cat <<A\nx\nA\n)",
    "echo $(cat <<A)\nx\nA\n",
    "echo `cat <<A`\nx\nA\n",
    "cat <<A \"x\ny\"\nb\nA\n",
    "read x <<EOF; for i in \"$x\"\nvalue\nEOF\ndo\n :\ndone\n",
    "read x <<EOF; for i\nvalue\nEOF\ndo :; done\n",
    "read x <<EOF; case $x\nvalue\nEOF\nin a) ;; esac\n",
    "if cat <<A\nx\nA\nthen :; fi",
    "cat <<A; [[ a ==\nx\nA\nb ]]\n",
    "[[ -n $(cat <<A)\nx\nA\n]]\n",
    "cat <<A; [[ ( a\nx\nA\n) ]]\n",
    "cat <<A; (( a ? 1 :\nx\nA\n2 ))\n",
    "cat <<A; (( i\nx\nA\n++ ))\n",
    "cat <<A; (( -\nx\nA\n1 ))\n",
    "echo $(( $(cat <<A) +\nx\nA\n1))\n",
    "cat <<A; for ((i=0\nx\nA\n;;)); do :; done\n",
    "cat <<A; function f\nx\nA\n{ :; }\n"
};

#define NUMERUS_CASUUM_HEREDOC \
    ((i32)(magnitudo(CASUS_HEREDOC)/magnitudo(CASUS_HEREDOC[0])))

/* causae nominatae circuitus */
enumeratio {
    CIRCUITUS_IDEM = 0,
    CIRCUITUS_PARSATOR_NIHIL,
    CIRCUITUS_SCRIPTURA_RECUSATA_SEQUENTIA,
    CIRCUITUS_SCRIPTURA_RECUSATA_NUL,
    CIRCUITUS_SCRIPTURA_RECUSATA_MIXTUM,
    CIRCUITUS_SCRIPTURA_RECUSATA_ALIA,
    CIRCUITUS_LECTIO_RECUSATA,
    CIRCUITUS_RESCRIPTURA_RECUSATA,
    CIRCUITUS_OCTETI_DISPARES,
    CIRCUITUS_RELECTA_DISPAR_FONTI,
    CIRCUITUS_ARBOR_DISPAR,
    CIRCUITUS_ARBOR_INFIDELIS,
    CIRCUITUS_COMPRESSIO_VIVA,
    CIRCUITUS_NUMERUS_CAUSARUM
};

hic_manens constans character* CAUSAE[] = {
    "idem",
    "parsator NIHIL reddidit",
    "scriptura STML recusata: sequentia claudens",
    "scriptura STML recusata: NUL",
    "scriptura STML recusata: textui non tutus (mixtum)",
    "scriptura STML recusata: causa ALIA",
    "lectio STML recusata",
    "rescriptura STML recusata",
    "octeti STML dispares inter cyclos",
    "arbor relecta fontem non emittit",
    "arbor relecta dispar (STRUCTURALIS)",
    "arbor relecta infidelis (FIDELITAS: positiones)",
    "compressio templorum viva"
};

#define SEQUENTIA_CLAUDENS "valor sequentiam claudentem fert"
#define TEXTUI_NON_TUTUS   "valor lexematis textui non tutus (mixtum)"

/* nuntius = causa scriptoris aut lectoris (NIHIL si nulla) */
nomen structura {
                    integer  causa;
         constans character* nuntius;
    MateriaArborDifferentia  differentia;
    /* FIDELITAS: mensura, non causa */
    MateriaArborDifferentia differentia_fidelitatis;
                        b32 fidelis;
                        i32 octeti_stml;
                        s32 referentiae;   /* ad heredoc, parsata */
                        s32 referentiae_relectae;
              CrustaParsura relatio;
} Circuitus;

nomen structura {
    i32 plagulae;
    i32 octeti;
    i32 octeti_stml;
    i32 fideles;
    i32 per_causam[CIRCUITUS_NUMERUS_CAUSARUM];
} Summa;

interior constans character*
_plagulam_legere (
               Piscina* piscina,
    constans character* radix,
    constans character* via,
                   i32* mensura)
{
    character plena[4096];
    FILE* f;
    longus longitudo;
    character* memoria;
    size_t lecti;

    sprintf(plena, "%s/%s", radix, via);
    f = fopen(plena, "rb");
    si (f == NIHIL)
    {
        redde NIHIL;
    }
    fseek(f, 0L, SEEK_END);
    longitudo = ftell(f);
    fseek(f, 0L, SEEK_SET);
    si (longitudo < 0L)
    {
        fclose(f);
        redde NIHIL;
    }
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

interior MateriaNodus*
_liber (
    constans MateriaNodus* nodus,
                      i32  locus,
                      i32  i)
{
    MateriaValor* v;

    si (   nodus                    == NIHIL
        || nodus->loci[locus].genus != MATERIA_VALOR_LISTA)
    {
        redde NIHIL;
    }
    v = materia_valor_lista_obtinere(nodus->loci[locus], i);
    redde (v != NIHIL && v->genus == MATERIA_VALOR_NODUS)
        ? v->datum.nodus : NIHIL;
}

interior b32
_textus_continet (
                chorda  textus,
    constans character* litterae)
{
    i32 mensura = (i32)strlen(litterae);
    i32 i;

    si (mensura == ZEPHYRUM || textus.mensura < mensura)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i + mensura <= textus.mensura; i++)
    {
        si (memcmp(textus.datum + i, litterae, (size_t)mensura)
                == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* referentiae in subarbore quarum scopus nodus heredoc est; -I si
 * referentia ulla scopum nullum aut generis alius fert. Recursio in
 * probatione licet (casus parvi). */
interior s32
_referentiae_heredoc (
    constans MateriaNodus* nodus)
{
    s32 summa = ZEPHYRUM;
    i32 i;

    si (nodus == NIHIL)
    {
        redde ZEPHYRUM;
    }
    per (i = ZEPHYRUM; i < nodus->numerus_locorum; i++)
    {
        constans MateriaValor* v = &nodus->loci[i];
                          s32  sub;

        si (v->genus == MATERIA_VALOR_NODUS)
        {
            sub = _referentiae_heredoc(v->datum.nodus);
            si (sub < ZEPHYRUM)
            {
                redde sub;
            }
            summa += sub;
        }
        alioquin si (v->genus == MATERIA_VALOR_LISTA)
        {
            i32 k;

            per (k = ZEPHYRUM; k < materia_valor_lista_numerus(*v); k++)
            {
                MateriaValor* e = materia_valor_lista_obtinere(*v, k);

                si (e != NIHIL && e->genus == MATERIA_VALOR_NODUS)
                {
                    sub = _referentiae_heredoc(e->datum.nodus);
                    si (sub < ZEPHYRUM)
                    {
                        redde sub;
                    }
                    summa += sub;
                }
            }
        }
        alioquin si (v->genus == MATERIA_VALOR_REFERENTIA)
        {
            si (   v->datum.nodus        == NIHIL
                || v->datum.nodus->genus != (s32)CRUSTA_GENUS_HEREDOC)
            {
                redde (s32)-I;
            }
            summa++;
        }
    }
    redde summa;
}

/* Duo cycli + comparator + emissio relectae. 'mutare' = culpa
 * plantata probationis: caudam arboris relectae ANTE comparationem
 * tollere (octeti STML iam scripti bis idem) - oraculum separans:
 * octeti idem, arbor dispar. */
interior Circuitus
_circuitum_probare (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura,
                               b32  mutare)
{
                Circuitus c;
             MateriaNodus* radix;
             MateriaNodus* relecta;
    MateriaArborScriptura  s1;
    MateriaArborScriptura  s2;
       MateriaArborVitium  vitium;

    memset(&c, ZEPHYRUM, magnitudo(Circuitus));
    c.causa = CIRCUITUS_IDEM;

    radix = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
        &c.relatio);
    si (radix == NIHIL)
    {
        c.causa = CIRCUITUS_PARSATOR_NIHIL;
        redde c;
    }
    c.referentiae = _referentiae_heredoc(radix);

    s1 = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!s1.successus)
    {
        c.nuntius = s1.causa;
        si (   s1.causa                             != NIHIL
            && strcmp(s1.causa, SEQUENTIA_CLAUDENS) == ZEPHYRUM)
        {
            c.causa = CIRCUITUS_SCRIPTURA_RECUSATA_SEQUENTIA;
        }
        alioquin si (   s1.causa                != NIHIL
                     && strstr(s1.causa, "NUL") != NIHIL)
        {
            c.causa = CIRCUITUS_SCRIPTURA_RECUSATA_NUL;
        }
        alioquin si (   s1.causa                           != NIHIL
                     && strcmp(s1.causa, TEXTUI_NON_TUTUS) == ZEPHYRUM)
        {
            c.causa = CIRCUITUS_SCRIPTURA_RECUSATA_MIXTUM;
        }
        alioquin
        {
            c.causa = CIRCUITUS_SCRIPTURA_RECUSATA_ALIA;
        }
        redde c;
    }
    c.octeti_stml = s1.textus.mensura;
    {
        MateriaSedesRelatio sedes;

        si (!materia_sedes_verificare(piscina, radix, consilium, fons,
                mensura, &sedes))
        {
            imprimere("    (sedes: %s, elementum %d)\n",
                sedes.causa ? sedes.causa : "-", (integer)sedes.index);
        }
        CREDO_VERUM (sedes.sana);
        SEDES_ELEMENTA += sedes.elementa;
        SEDES_DERIVATA += sedes.derivata;
    }

    relecta = materia_arbor_legere(piscina, NIHIL, s1.textus, consilium,
        &vitium);
    si (relecta == NIHIL)
    {
        c.causa    = CIRCUITUS_LECTIO_RECUSATA;
        c.nuntius  = vitium.causa;
        redde c;
    }
    c.referentiae_relectae = _referentiae_heredoc(relecta);

    s2 = materia_arbor_scribere_nodum(piscina, relecta, consilium);
    si (!s2.successus)
    {
        c.causa    = CIRCUITUS_RESCRIPTURA_RECUSATA;
        c.nuntius  = s2.causa;
        redde c;
    }
    si (   s1.textus.mensura != s2.textus.mensura
        || memcmp(s1.textus.datum, s2.textus.datum,
               (size_t)s1.textus.mensura) != ZEPHYRUM)
    {
        c.causa = CIRCUITUS_OCTETI_DISPARES;
        redde c;
    }

    /* CATENA CLAUSA UNA ASSERTIONE: arbor RELECTA per scriptorem
     * octetorum emissa == fons (html H7) */
    {
        MateriaScripturaConsilium cs;
                 MateriaScriptura emissa;

        materia_scriptura_consilium_nudum(&cs, &CRUSTA_REGISTRUM);
        emissa = materia_scribere_nodum(piscina, relecta, &cs);
        si (   !emissa.successus || emissa.textus.mensura != mensura
            || (mensura > ZEPHYRUM
                && memcmp(emissa.textus.datum, fons,
                       (size_t)mensura) != ZEPHYRUM))
        {
            c.causa = CIRCUITUS_RELECTA_DISPAR_FONTI;
            redde c;
        }
    }

    si (mutare)
    {
        /* cauda programmatis relecti (lexema FINIS, semper scripta)
         * sublata */
        relecta->loci[CRUSTA_PROGRAMMA_CAUDA] = materia_valor_nihil();
    }

    si (!materia_arbor_aequalis(radix, relecta,
            MATERIA_ARBOR_COMPARATIO_STRUCTURALIS, &c.differentia))
    {
        c.causa = CIRCUITUS_ARBOR_DISPAR;
        redde c;
    }
    c.fidelis = materia_arbor_aequalis(radix, relecta,
        MATERIA_ARBOR_COMPARATIO_FIDELITAS, &c.differentia_fidelitatis);
    si (!c.fidelis)
    {
        c.causa = CIRCUITUS_ARBOR_INFIDELIS;
        redde c;
    }
    si (s1.census.spatia_vocationes != ZEPHYRUM)
    {
        c.causa = CIRCUITUS_COMPRESSIO_VIVA;
        redde c;
    }
    redde c;
}

interior vacuum
_causam_imprimere (
    constans Circuitus* c)
{
    si (c->causa == CIRCUITUS_IDEM)
    {
        redde;
    }
    imprimere("    CAUSA: %s", CAUSAE[c->causa]);
    si (c->nuntius != NIHIL)
    {
        imprimere(" - %s", c->nuntius);
    }
    si (   c->causa              == CIRCUITUS_ARBOR_DISPAR
        && c->differentia.campus != NIHIL)
    {
        imprimere(" - campus %s via %s", c->differentia.campus,
            c->differentia.via);
    }
    si (   c->causa
        == CIRCUITUS_ARBOR_INFIDELIS
        && c->differentia_fidelitatis.campus != NIHIL)
    {
        imprimere(" - campus %s via %s",
            c->differentia_fidelitatis.campus,
            c->differentia_fidelitatis.via);
    }
    imprimere("\n");
}

interior vacuum
_summam_addere (
                 Summa* summa,
    constans Circuitus* c,
                   i32  octeti)
{
    summa->plagulae++;
    summa->octeti       += octeti;
    summa->octeti_stml  += c->octeti_stml;
    summa->per_causam[c->causa]++;
    si (c->causa == CIRCUITUS_IDEM && c->fidelis)
    {
        summa->fideles++;
    }
}

/* plagulam unam corporis per circuitum ducere; piscina propria */
interior vacuum
_plagula (
                constans character* radix,
                constans character* via,
    constans MateriaArborConsilium* consilium,
                             Summa* summa)
{
    Piscina* piscina = piscina_generare_dynamicum("crusta_stml",
        4194304);
                   i32  mensura;
    constans character* fons;
             Circuitus  c;

    si (piscina == NIHIL)
    {
        imprimere("    piscina deficit: %s\n", via);
        CREDO_CULPA ("piscina plagulae deficit");
        redde;
    }
    fons = _plagulam_legere(piscina, radix, via, &mensura);
    si (fons == NIHIL)
    {
        imprimere("    illegibilis: %s\n", via);
        CREDO_CULPA ("plagula corporis illegibilis");
        piscina_destruere(piscina);
        redde;
    }
    c = _circuitum_probare(piscina, consilium, fons, mensura, FALSUM);
    si (c.causa != CIRCUITUS_IDEM)
    {
        imprimere("  %s (%d octeti)\n", via, (integer)mensura);
        _causam_imprimere(&c);
    }
    _summam_addere(summa, &c, mensura);
    piscina_destruere(piscina);
}

/* plagulam fixorum per casus lectoris (crusta_exempla) ducere: casus
 * quisque documentum suum; titulus casus recusati (sequentia)
 * servatur */
interior vacuum
_fixum_per_casus (
                           Piscina* piscina,
    constans MateriaArborConsilium* consilium,
                constans character* radix,
                constans character* via,
                             Summa* summa,
                            chorda* titulus_recusati)
{
                   i32  mensura;
    constans character* fons;
                   Xar* exempla;
                   i32  i;

    fons = _plagulam_legere(piscina, radix, via, &mensura);
    CREDO_NON_NIHIL (fons);
    si (fons == NIHIL)
    {
        redde;
    }
    exempla = crusta_exempla_legere(piscina, fons, mensura);
    CREDO_NON_NIHIL (exempla);
    si (exempla == NIHIL)
    {
        redde;
    }
    per (i = ZEPHYRUM; i < xar_numerus(exempla); i++)
    {
        constans CrustaExemplum* e =
            (constans CrustaExemplum*)xar_obtinere(exempla, i);
        Circuitus c = _circuitum_probare(piscina, consilium,
            (constans character*)e->datum.datum, e->datum.mensura,
            FALSUM);

        si (c.causa != CIRCUITUS_IDEM)
        {
            imprimere("  %s casus %d '%.*s'\n", via,
                (integer)e->numerus,
                (integer)e->titulus.mensura,
                (constans character*)e->titulus.datum);
            _causam_imprimere(&c);
            si (c.causa == CIRCUITUS_SCRIPTURA_RECUSATA_SEQUENTIA)
            {
                *titulus_recusati = e->titulus;
            }
        }
        _summam_addere(summa, &c, e->datum.mensura);
    }
}

interior b32
_eadem_via (
    constans character* a,
    constans character* b)
{
    redde strcmp(a, b) == ZEPHYRUM;
}

interior vacuum
_summam_imprimere (
    constans character* titulus,
        constans Summa* summa)
{
    i32 k;

    imprimere("  %s: %d documenta, %d octeti -> %d STML, fideles %d\n",
        titulus, (integer)summa->plagulae, (integer)summa->octeti,
        (integer)summa->octeti_stml, (integer)summa->fideles);
    per (k = ZEPHYRUM; k < CIRCUITUS_NUMERUS_CAUSARUM; k++)
    {
        si (summa->per_causam[k] != ZEPHYRUM)
        {
            imprimere("    %-44s %d\n", CAUSAE[k],
                (integer)summa->per_causam[k]);
        }
    }
}

s32
principale (vacuum)
{
                   Piscina* piscina;
       MateriaLexiconRatum  ratum;
        MateriaLexIudicium  iudicium;
     MateriaArborConsilium  consilium;
                       b32  praeteritus;
        constans character* radix;
                       i32  i;
                     Summa  casuum;
                     Summa  fixa;
                     Summa  corpus;
                    chorda  titulus_recusati;
                       i32  documenta;

    piscina = piscina_generare_dynamicum("probatio_crusta_stml",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    radix = getenv("RHUBARB_RADIX");
    si (radix == NIHIL)
    {
        radix = ".";
    }
    memset(&casuum, ZEPHYRUM, magnitudo(casuum));
    memset(&fixa, ZEPHYRUM, magnitudo(fixa));
    memset(&corpus, ZEPHYRUM, magnitudo(corpus));
    titulus_recusati.mensura  = ZEPHYRUM;
    titulus_recusati.datum    = NIHIL;

    si (!materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
            &iudicium))
    {
        imprimere("LEXICON CRUSTAE RECUSATUM\n");
        redde I;
    }
    materia_arbor_consilium_nudum(&consilium, &CRUSTA_REGISTRUM, &ratum,
        "crusta");
    /* forma lexematum: cauda crustae (profunditas backtick), ad
     * zephyrum in lexematibus relectis - vide caput */
    consilium.forma.mensura_caudae = (i32)magnitudo(CrustaCauda);


    /* ==================================================
     * PORTA: casus inlinei - duo cycli + comparator + emissio
     * ================================================== */

    imprimere("\n--- PORTA: circuitus STML, casus inlinei (%d) ---\n",
        (integer)NUMERUS_CASUUM);
    per (i = ZEPHYRUM; i < NUMERUS_CASUUM; i++)
    {
        Circuitus c = _circuitum_probare(piscina, &consilium, CASUS[i],
            (i32)strlen(CASUS[i]), FALSUM);

        si (c.causa != CIRCUITUS_IDEM)
        {
            imprimere("  casus %d: %s\n", (integer)i, CASUS[i]);
            _causam_imprimere(&c);
        }
        CREDO_AEQUALIS_S32 ((s32)c.causa, (s32)CIRCUITUS_IDEM);
        _summam_addere(&casuum, &c, (i32)strlen(CASUS[i]));
    }
    _summam_imprimere("inlinei", &casuum);


    /* ==================================================
     * PORTA: referentiae heredoc relatae
     * ================================================== */

    imprimere("\n--- PORTA: referentiae heredoc (%d casus) ---\n",
        (integer)NUMERUS_CASUUM_HEREDOC);
    per (i = ZEPHYRUM; i < NUMERUS_CASUUM_HEREDOC; i++)
    {
        MateriaNodus* r = crusta_arbor_parsare(piscina,
            CASUS_HEREDOC[i],
            (i32)strlen(CASUS_HEREDOC[i]), &CRUSTA_BASH, NIHIL);
        MateriaArborScriptura s;
                    Circuitus c;

        CREDO_NON_NIHIL (r);
        s = materia_arbor_scribere_nodum(piscina, r, &consilium);
        CREDO_VERUM (s.successus);
        CREDO_VERUM (_textus_continet(s.textus, "<corpus(> #nod"));
        CREDO_VERUM (_textus_continet(s.textus, "id=\"nod"));
        c = _circuitum_probare(piscina, &consilium, CASUS_HEREDOC[i],
            (i32)strlen(CASUS_HEREDOC[i]), FALSUM);
        CREDO_AEQUALIS_S32 ((s32)c.causa, (s32)CIRCUITUS_IDEM);
        /* petitio quaeque corpus: referentiae = heredoca, utrimque,
         * scopus generis heredoc semper (-I aliter) */
        CREDO_AEQUALIS_S32 (c.referentiae, (s32)c.relatio.heredoca);
        CREDO_AEQUALIS_S32 (c.referentiae_relectae,
            (s32)c.relatio.heredoca);
        CREDO_MAIOR_S32 (c.referentiae, ZEPHYRUM);
    }

    /* structura relecta: imperium -> redirectio.corpus -> heredoc
     * (liber II programmatis: imperium, separator, heredoc, ...) */
    {
        MateriaNodus* r = crusta_arbor_parsare(piscina,
            CASUS_HEREDOC[0],
            (i32)strlen(CASUS_HEREDOC[0]), &CRUSTA_BASH, NIHIL);
        MateriaArborScriptura s;
           MateriaArborVitium vitium;
                 MateriaNodus* relecta;
                 MateriaNodus* imperium;
                 MateriaNodus* redirectio;
                 MateriaNodus* heredoc;

        CREDO_NON_NIHIL (r);
        s = materia_arbor_scribere_nodum(piscina, r, &consilium);
        CREDO_VERUM (s.successus);
        relecta = s.successus ? materia_arbor_legere(piscina, NIHIL,
            s.textus, &consilium, &vitium) : NIHIL;
        CREDO_NON_NIHIL (relecta);
        imperium   = _liber(relecta, (i32)CRUSTA_PROGRAMMA_LIBERI,
            ZEPHYRUM);
        redirectio  = _liber(imperium, (i32)CRUSTA_IMPERIUM_LIBERI, I);
        heredoc     = _liber(relecta, (i32)CRUSTA_PROGRAMMA_LIBERI, II);
        CREDO_NON_NIHIL (redirectio);
        CREDO_NON_NIHIL (heredoc);
        si (redirectio != NIHIL && heredoc != NIHIL)
        {
            CREDO_AEQUALIS_S32 (redirectio->genus,
                (s32)CRUSTA_GENUS_REDIRECTIO);
            CREDO_AEQUALIS_S32 (heredoc->genus,
                (s32)CRUSTA_GENUS_HEREDOC);
            CREDO_AEQUALIS_S32 (
                (s32)redirectio->loci[CRUSTA_REDIRECTIO_CORPUS].genus,
                (s32)MATERIA_VALOR_REFERENTIA);
            CREDO_AEQUALIS_PTR (
                redirectio->loci[CRUSTA_REDIRECTIO_CORPUS].datum.nodus,
                heredoc);
        }
    }


    /* ==================================================
     * PORTA: fixa per casus (lector casuum)
     * ================================================== */

    imprimere("\n--- PORTA: circuitus STML, casus fixorum ---\n");
    _fixum_per_casus(piscina, &consilium, radix,
        "probationes/fixa/crusta/pathologiae.sh", &fixa,
        &titulus_recusati);
    _fixum_per_casus(piscina, &consilium, radix,
        "probationes/fixa/crusta/adversarius.sh", &fixa,
        &titulus_recusati);
    _summam_imprimere("fixa", &fixa);
    /* XXXVI + XVII casus. Recusatio per sequentiam claudentem
     * EXSTINCTA est (2026-09-21, planum clausurae gradus III):
     * 'forma-cruda' olim sola recusabatur, nunc circuitum integrum
     * facit - ergo IDEM LI -> LII et sequentia I -> ZEPHYRUM.
     * Pinna ZEPHYRUM viva manet: si quis recusationem reducat,
     * haec rubet et titulus infra eam NOMINAT. */
    CREDO_AEQUALIS_I32 (fixa.plagulae, (i32)LIII);
    CREDO_AEQUALIS_I32 (fixa.per_causam[CIRCUITUS_IDEM], (i32)LII);
    CREDO_AEQUALIS_I32 (
        fixa.per_causam[CIRCUITUS_SCRIPTURA_RECUSATA_SEQUENTIA],
        (i32)ZEPHYRUM);
    /* 'simplex-apertus': apex apertus ad finem, valor linea nova
     * terminatus in elemento mixto - recusatio nominata (vide caput) */
    CREDO_AEQUALIS_I32 (
        fixa.per_causam[CIRCUITUS_SCRIPTURA_RECUSATA_MIXTUM], (i32)I);
    /* Nullus casus per sequentiam recusatur, ergo titulus VACUUS -
     * sed lectio manet, ut recusatio reducta nomen suum ferat. */
    CREDO_AEQUALIS_I32 (titulus_recusati.mensura, (i32)ZEPHYRUM);


    /* ==================================================
     * PORTA: corpus totum - domus, fixa totae, FreeBSD
     * ================================================== */

    imprimere("\n--- PORTA: circuitus STML, corpus ---\n");
    {
        constans character* lista;
                       i32  mensura;

        lista = _plagulam_legere(piscina, radix,
            "build/crusta_corpus.lst",
            &mensura);
        si (lista == NIHIL)
        {
            CREDO_CULPA ("build/crusta_corpus.lst absens - cursor eam "
                "scribit (git ls-files '*.sh')");
        }
        alioquin
        {
            i32 ab = ZEPHYRUM;

            dum (ab < mensura)
            {
                      i32 ad = ab;
                character via[4096];

                dum (ad < mensura && lista[ad] != '\n')
                {
                    ad++;
                }
                si (ad > ab && ad - ab < (i32)4096)
                {
                    memcpy(via, lista + ab, (size_t)(ad - ab));
                    via[ad - ab] = '\0';
                    si (   !_eadem_via(via,
                            "probationes/fixa/crusta/pathologiae.sh")
                        && !_eadem_via(via,
                            "probationes/fixa/crusta/adversarius.sh"))
                    {
                        _plagula(radix, via, &consilium, &corpus);
                    }
                }
                ab = ad + I;
            }
        }
    }
    /* domus: omnes idem */
    CREDO_VERUM (corpus.plagulae >= (i32)CCXXX);
    CREDO_VERUM (corpus.octeti > (i32)800000);
    CREDO_AEQUALIS_I32 (corpus.per_causam[CIRCUITUS_IDEM],
        corpus.plagulae);

    _plagula(radix, "probationes/fixa/crusta/pathologiae.sh",
        &consilium,
        &corpus);
    _plagula(radix, "probationes/fixa/crusta/adversarius.sh",
        &consilium,
        &corpus);
    per (i = ZEPHYRUM; CRUSTA_FREEBSD_PLAGULAE[i] != NIHIL; i++)
    {
        _plagula(radix, CRUSTA_FREEBSD_PLAGULAE[i], &consilium,
            &corpus);
    }
    _summam_imprimere("corpus", &corpus);
    /* + II fixa + LXXXIX FreeBSD: omnes idem. adversarius.sh TOTA
     * transit: constructa aperta casuum priorum contextum lexicalem
     * casuum posteriorum mutant (apex apertus casus II reliqua
     * absorbet usque ad apicem proximum), ergo lexema recusandum in
     * plagula tota non nascitur - recusationes per casum et inlineae
     * pinnantur (supra, infra). Mensuratum, non derivatum. */
    CREDO_AEQUALIS_I32 (corpus.per_causam[CIRCUITUS_IDEM],
        corpus.plagulae);
    CREDO_MAIOR_I32 (corpus.octeti_stml, corpus.octeti);

    /* se metiens: documenta = casuum + casus fixorum + plagulae */
    documenta = casuum.plagulae + fixa.plagulae + corpus.plagulae;
    CREDO_AEQUALIS_I32 (casuum.plagulae, NUMERUS_CASUUM);
    CREDO_VERUM (documenta >= NUMERUS_CASUUM + (i32)XLIX + (i32)CCXXX
        + (i32)XCI);
    imprimere("  summa: %d documenta\n", (integer)documenta);


    /* ==================================================
     * MENSURA: FIDELITAS (spec par. X)
     * ================================================== */

    imprimere("\n--- Mensura: FIDELITAS ---\n");
    imprimere("  fideles: casuum %d/%d, fixa %d/%d, corpus %d/%d\n",
        (integer)casuum.fideles, (integer)casuum.plagulae,
        (integer)fixa.fideles, (integer)fixa.plagulae,
        (integer)corpus.fideles, (integer)corpus.plagulae);
    {
        Circuitus c = _circuitum_probare(piscina, &consilium, "a\nb\n",
            (i32)IV, FALSUM);

        CREDO_AEQUALIS_S32 ((s32)c.causa, (s32)CIRCUITUS_IDEM);
        imprimere("  'a\\nb\\n' fidelis %d", (integer)c.fidelis);
        si (!c.fidelis && c.differentia_fidelitatis.campus != NIHIL)
        {
            imprimere(" - campus %s via %s",
                c.differentia_fidelitatis.campus,
                c.differentia_fidelitatis.via);
        }
        imprimere("\n");
    }


    /* ==================================================
     * PROMOTUM: sequentia claudens propria (olim limes substrati)
     * ================================================== */

    {
        /* Commentarium (trivium crudum) et corpus heredoc (pars
         * litteralis cruda). Olim AMBO recusabantur, causa
         * 'valor sequentiam claudentem fert' (limes 01M2KPJ0HW), et
         * pinna illa recusationem ipsam asserebat cum nota 'RUBET
         * CUM MATERIA RECIDAT'. Recidit 2026-09-21: substratum
         * scalam fugae fert (planum clausurae gradus I-II), ergo
         * circuitus INTEGER est.
         *
         * Casus primus est ipse quem Fran in commento bash invenit.
         * Circuitus IDEM hic octetos fontis asserit, non solam
         * successionem - valor tacite mutatus periculum primum
         * plani est. */
        hic_manens constans character* PROMOTA[] = {
            "a # </crusta-commentum>",
            "cat <<X\n</crusta-litteralis>\nX\n"
        };
        Circuitus c;
              i32 k;

        imprimere("\n--- Sequentia claudens propria: circuitus ---\n");
        per (k = ZEPHYRUM; k < II; k++)
        {
            c = _circuitum_probare(piscina, &consilium, PROMOTA[k],
                (i32)strlen(PROMOTA[k]), FALSUM);
            CREDO_AEQUALIS_S32 ((s32)c.causa, (s32)CIRCUITUS_IDEM);
        }
    }


    /* ==================================================
     * PINNA SEPARATIONIS ORACULORUM: octeti idem, arbor dispar
     * ================================================== */

    {
        Circuitus c;

        imprimere("\n--- Probans separationem oraculorum ---\n");
        /* arbor relecta laesa POST rescripturam: octeti STML bis idem
         * manent, comparator solus dissentit. Si haec assertio viridis
         * esset sine laesione, comparator non curreret. */
        c = _circuitum_probare(piscina, &consilium, "a; b\n", (i32)V,
            VERUM);
        CREDO_AEQUALIS_S32 ((s32)c.causa, (s32)CIRCUITUS_ARBOR_DISPAR);
        CREDO_NON_NIHIL (c.differentia.campus);
    }

    imprimere("\n");
    imprimere("\n--- sedes (visio): %d elementa verificata, %d derivata"
        " ---\n", (integer)SEDES_ELEMENTA, (integer)SEDES_DERIVATA);
    CREDO_VERUM (SEDES_ELEMENTA > ZEPHYRUM);
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
