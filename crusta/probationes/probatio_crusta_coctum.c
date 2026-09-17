/* probatio_crusta_coctum.c - Porta visionis coctae (P11a)
 *
 * crusta_coctum_scribere contra textum quem bash 5.2.15 ipse imprimit:
 * quisque casus lineis 'f() {' et '}' involvitur (forma oraculi P11b),
 * parsatur, per emissorem octetim idem probatur, coquitur et cum
 * 'declare -f f' comparatur. Textus exspectati MENSURATI sunt
 * (2026-09-16, 'env -i LC_ALL=C PATH=/nonexistent bash -r -c' - ambitus
 * auri oraculi - generator in scratch P11a/P11b),
 * numquam ex impressore nostro scripti.
 *
 * Familiae regularum (plan P11a, 'measured mechanics'): ordo
 * redirectionum, '|&', formae functionum, iteratio et cyclus, electio
 * cum terminatoribus et corpore vacuo, iudicium ('-n' verbi nudi),
 * arithmetica verbatim, commentaria abiecta, duo modi (substitutio
 * reparsata), connexiones et saltus, corpora heredoc dilata (pipa, et,
 * asyncum, ';' cum vexillo, grex, nidificatio sinistra et dextra,
 * delimitator citatus, '<<-'), formae fd redirectionum, verba ('$'...''
 * decoctum, continuationes abiectae).
 *
 * Mensura sui: CASUS_NUMERUS pinnatum (XL, >= XII planum poscit;
 * P11b addidit IV: spatium post '$(', continuatio in backtick,
 * vexillum heredoc post functionem, regulae '$'...'' sub LC_ALL=C).
 */

#include "latina.h"
#include "credo.h"
#include "crusta_arbor.h"
#include "crusta_coctum.h"
#include "crusta_registrum.h"
#include "materia_scribere.h"
#include "piscina.h"
#include <stdio.h>
#include <string.h>

nomen structura {
    constans character* titulus;
    constans character* fons;
    constans character* exspectatum;
} CasusCoctus;

hic_manens constans CasusCoctus CASUS[] = {
    { "probe-spec",
      "if true; then :; fi; a | b && c || d; ! x",
      "f () \n"
      "{ \n"
      "    if true; then\n"
      "        :;\n"
      "    fi;\n"
      "    a | b && c || d;\n"
      "    ! x\n"
      "}\n" },
    { "redirectio-ordo",
      "2>&1 A=1 cmd x >o B=2 y",
      "f () \n"
      "{ \n"
      "    A=1 cmd x B=2 y 2>&1 > o\n"
      "}\n" },
    { "pipa-erroris",
      "a |& b",
      "f () \n"
      "{ \n"
      "    a 2>&1 | b\n"
      "}\n" },
    { "functio-crustula",
      "h() ( : )",
      "f () \n"
      "{ \n"
      "    function h () \n"
      "    { \n"
      "        ( : )\n"
      "    }\n"
      "}\n" },
    { "iteratio",
      "for i in a b; do x; done",
      "f () \n"
      "{ \n"
      "    for i in a b;\n"
      "    do\n"
      "        x;\n"
      "    done\n"
      "}\n" },
    { "electio-vacua",
      "case $x in a|b) y;; c) ;; esac",
      "f () \n"
      "{ \n"
      "    case $x in \n"
      "        a | b)\n"
      "            y\n"
      "        ;;\n"
      "        c)\n"
      "\n"
      "        ;;\n"
      "    esac\n"
      "}\n" },
    { "heredoc-pipa",
      "cat <<A <<B | wc -l\n"
      "one\n"
      "A\n"
      "two $x\n"
      "B",
      "f () \n"
      "{ \n"
      "    cat <<A <<B |\n"
      "one\n"
      "A\n"
      "two $x\n"
      "B\n"
      "  wc -l\n"
      "}\n" },
    { "iudicium",
      "[[  -f  x  &&  a ==  b ]]; [[ a ]]; [[ (a) && (b||c) ]]",
      "f () \n"
      "{ \n"
      "    [[ -f x && a == b ]];\n"
      "    [[ -n a ]];\n"
      "    [[ ( -n a ) && ( -n b || -n c ) ]]\n"
      "}\n" },
    { "arithmetica",
      "((  a+b  )); echo $(( 1 +2 ))",
      "f () \n"
      "{ \n"
      "    ((  a+b  ));\n"
      "    echo $(( 1 +2 ))\n"
      "}\n" },
    { "commenta",
      "# c\n"
      "\n"
      "x   y   # t",
      "f () \n"
      "{ \n"
      "    x y\n"
      "}\n" },
    { "repetitio",
      "while a; do b; c; done\n"
      "until a\n"
      "do b\n"
      "done",
      "f () \n"
      "{ \n"
      "    while a; do\n"
      "        b;\n"
      "        c;\n"
      "    done;\n"
      "    until a; do\n"
      "        b;\n"
      "    done\n"
      "}\n" },
    { "elif",
      "if a; then b; elif c; then d; else e; fi",
      "f () \n"
      "{ \n"
      "    if a; then\n"
      "        b;\n"
      "    else\n"
      "        if c; then\n"
      "            d;\n"
      "        else\n"
      "            e;\n"
      "        fi;\n"
      "    fi\n"
      "}\n" },
    { "grex-crustula",
      "{ a; b; } > o 2>&1\n"
      "( a; b ) < i",
      "f () \n"
      "{ \n"
      "    { \n"
      "        a;\n"
      "        b\n"
      "    } > o 2>&1;\n"
      "    ( a;\n"
      "    b ) < i\n"
      "}\n" },
    { "asynca",
      "a & b; a &\n"
      "a; b &\n"
      "a && b &",
      "f () \n"
      "{ \n"
      "    a & b;\n"
      "    a & a;\n"
      "    b & a && b &\n"
      "}\n" },
    { "assignationes",
      "x=( 1   2  [k]=v ) y=$'it\\'s' z=\"a\\\n"
      "b\" w=ab\\\n"
      "cd",
      "f () \n"
      "{ \n"
      "    x=(1 2 [k]=v) y='it'\\''s' z=\"ab\" w=abcd\n"
      "}\n" },
    { "praefixa",
      "! ! a; time time a; ! time -p b | c",
      "f () \n"
      "{ \n"
      "    a;\n"
      "    time a;\n"
      "    time -p ! b | c\n"
      "}\n" },
    { "socius",
      "coproc c { :; }; coproc a b; coproc N ( a )",
      "f () \n"
      "{ \n"
      "    coproc c { \n"
      "        :\n"
      "    };\n"
      "    coproc COPROC a b;\n"
      "    coproc N ( a )\n"
      "}\n" },
    { "cyclus",
      "for ((;;)); do x; done\n"
      "for ((  i=0 ;i<3;  i++ )); do :; done\n"
      "select i; do :; done\n"
      "for i in; do :; done",
      "f () \n"
      "{ \n"
      "    for ((1; 1; 1))\n"
      "    do\n"
      "        x;\n"
      "    done;\n"
      "    for ((i=0 ; i<3; i++ ))\n"
      "    do\n"
      "        :;\n"
      "    done;\n"
      "    select i in \"$@\";\n"
      "    do\n"
      "        :;\n"
      "    done;\n"
      "    for i in ;\n"
      "    do\n"
      "        :;\n"
      "    done\n"
      "}\n" },
    { "substitutio",
      "echo $(  a;b  ) `  c;d  ` <( e ) >(f) \"$(a  |  b)\" ${x:-$(a;"
      "b)}",
      "f () \n"
      "{ \n"
      "    echo $(a; b) `  c;d  ` <(e) >(f) \"$(a | b)\" ${x:-$(a; b)"
      "}\n"
      "}\n" },
    { "substitutio-modus",
      "x=$({ a; b; }; while c; do d; done; case e in f) g;; esac)",
      "f () \n"
      "{ \n"
      "    x=$({ a; b; }; while c; do\n"
      "    d;\n"
      "done; case e in \n"
      "    f)\n"
      "        g\n"
      "    ;;\n"
      "esac)\n"
      "}\n" },
    { "substitutio-linea",
      "x=$(a & b && c\n"
      "d) y=$(a\n"
      "\n"
      "b) z=$(a;\n"
      "b) w=$() v=$( # c\n"
      ")",
      "f () \n"
      "{ \n"
      "    x=$(a & b && c\n"
      "d) y=$(a\n"
      "b) z=$(a; b) w=$() v=$()\n"
      "}\n" },
    { "substitutio-functio",
      "x=$(g() { :; }; cat <<A\n"
      "body\n"
      "A\n"
      ")",
      "f () \n"
      "{ \n"
      "    x=$(function g () \n"
      "{ \n"
      "    :\n"
      "}; cat <<A\n"
      "body\n"
      "A\n"
      ")\n"
      "}\n" },
    { "redirectio-fd",
      "a {fd}>o &>p &>>q >|r <>s >&- <&3 2>&1 3>&2- <<<w",
      "f () \n"
      "{ \n"
      "    a {fd}> o &> p &>> q >| r 0<> s 1>&- 0<&3 2>&1 3>&2- <<< w"
      "\n"
      "}\n" },
    { "redirectio-dup",
      "a >&2 1>&2 >&$x >&o 2>&- <&- 4<&3 0<i 1>o 3<>s >> p 2>>q\n"
      "<&$fd 2>&$x 3<<<w {fd}>&- {v}<&3",
      "f () \n"
      "{ \n"
      "    a 1>&2 1>&2 >&$x >&o 2>&- 0>&- 4<&3 < i > o 3<> s >> p 2>>"
      " q;\n"
      "    <&$fd 2>&$x 3<<< w {fd}>&- {v}<&3\n"
      "}\n" },
    { "heredoc-tabulae",
      "cat <<-A\n"
      "\tx\n"
      "\tA",
      "f () \n"
      "{ \n"
      "    cat <<-A\n"
      "x\n"
      "A\n"
      "\n"
      "}\n" },
    { "heredoc-et",
      "cat <<'A' && b\n"
      "$x\n"
      "A",
      "f () \n"
      "{ \n"
      "    cat <<'A' && \n"
      "$x\n"
      "A\n"
      " b\n"
      "}\n" },
    { "heredoc-vexillum",
      "cat <<A; b; c\n"
      "x\n"
      "A",
      "f () \n"
      "{ \n"
      "    cat <<A\n"
      "x\n"
      "A\n"
      "\n"
      "    b\n"
      "    c\n"
      "}\n" },
    { "heredoc-asynca",
      "cat <<A &\n"
      "x\n"
      "A",
      "f () \n"
      "{ \n"
      "    cat <<A &\n"
      "x\n"
      "A\n"
      " \n"
      "}\n" },
    { "heredoc-repetitio",
      "while cat <<A; do b; done\n"
      "x\n"
      "A",
      "f () \n"
      "{ \n"
      "    while cat <<A\n"
      "x\n"
      "A\n"
      " do\n"
      "        b;\n"
      "    done\n"
      "}\n" },
    { "heredoc-grex",
      "{ cat <<A; }; b\n"
      "x\n"
      "A\n"
      "(cat <<B); c\n"
      "y\n"
      "B",
      "f () \n"
      "{ \n"
      "    { \n"
      "        cat <<A\n"
      "x\n"
      "A\n"
      "\n"
      "    }\n"
      "    b;\n"
      "    ( cat <<B\n"
      "y\n"
      "B\n"
      " )\n"
      "    c\n"
      "}\n" },
    { "heredoc-nidus",
      "a | cat <<B | c\n"
      "y\n"
      "B\n"
      "a && cat <<C || c\n"
      "z\n"
      "C",
      "f () \n"
      "{ \n"
      "    a | cat <<B |\n"
      "y\n"
      "B\n"
      "  c\n"
      "    a && cat <<C\n"
      "z\n"
      "C\n"
      " || c\n"
      "}\n" },
    { "heredoc-citatum",
      "cat <<\\A <<A\"B\"\n"
      "x\n"
      "A\n"
      "y\n"
      "AB\n"
      "cat <<C\n"
      "a\\\n"
      "b $(x;y) `z`\n"
      "C",
      "f () \n"
      "{ \n"
      "    cat <<'A' <<'AB'\n"
      "x\n"
      "A\n"
      "y\n"
      "AB\n"
      "\n"
      "    cat <<C\n"
      "ab $(x;y) `z`\n"
      "C\n"
      "\n"
      "}\n" },
    { "functiones",
      "function g { :; }\n"
      "function g() { :; } > o\n"
      "g() { :; }; g\n"
      "f2() if a; then b; fi\n"
      "h() ( : ) > o",
      "f () \n"
      "{ \n"
      "    function g () \n"
      "    { \n"
      "        :\n"
      "    };\n"
      "    function g () \n"
      "    { \n"
      "        :\n"
      "    } > o;\n"
      "    function g () \n"
      "    { \n"
      "        :\n"
      "    };\n"
      "    g;\n"
      "    function f2 () \n"
      "    { \n"
      "        if a; then\n"
      "            b;\n"
      "        fi\n"
      "    };\n"
      "    function h () \n"
      "    { \n"
      "        ( : ) > o\n"
      "    }\n"
      "}\n" },
    { "electio-termini",
      "case x in (a) b;& c) d;;& *) ;; esac; case x in esac\n"
      "case x in a) b; c;; d) e\n"
      "esac",
      "f () \n"
      "{ \n"
      "    case x in \n"
      "        a)\n"
      "            b\n"
      "        ;&\n"
      "        c)\n"
      "            d\n"
      "        ;;&\n"
      "        *)\n"
      "\n"
      "        ;;\n"
      "    esac;\n"
      "    case x in \n"
      "    esac;\n"
      "    case x in \n"
      "        a)\n"
      "            b;\n"
      "            c\n"
      "        ;;\n"
      "        d)\n"
      "            e\n"
      "        ;;\n"
      "    esac\n"
      "}\n" },
    { "verba",
      "echo $'a\\tb' $\"c\" {1..3} 'q  r' \"s  $t\" \"a\\\"b\" `a \\`"
      "b\\``",
      "f () \n"
      "{ \n"
      "    echo 'a\tb' \"c\" {1..3} 'q  r' \"s  $t\" \"a\\\"b\" `a \\"
      "`b\\``\n"
      "}\n" },
    { "conditio-arithmetica",
      "if ((a)) >o; then :; fi",
      "f () \n"
      "{ \n"
      "    if ((a)) > o; then\n"
      "        :;\n"
      "    fi\n"
      "}\n" },
    { "substitutio-spatium",
      "echo $( (a) ) $(  (b);c ) $((d); e) $((echo  a);   b)",
      "f () \n"
      "{ \n"
      "    echo $( ( a )) $( ( b ); c) $((d); e) $((echo  a);   b)\n"
      "}\n" },
    { "gravis-continuatio",
      "v=`printf %s 'a\\\n"
      "b'` w=\"`a \\\\\\\n"
      "b`\"",
      "f () \n"
      "{ \n"
      "    v=`printf %s 'ab'` w=\"`a \\\\b`\"\n"
      "}\n" },
    { "functio-vexillum",
      "g() { cat <<A && b\n"
      "x\n"
      "A\n"
      "}; c",
      "f () \n"
      "{ \n"
      "    function g () \n"
      "    { \n"
      "        cat <<A && \n"
      "x\n"
      "A\n"
      " b\n"
      "    };\n"
      "    c\n"
      "}\n" },
    { "effugia-regulae",
      "x=$'\\ca\\cA\\c\?\\x41\\x4g\\xg\\101\\8\\\?\\z' y=$'\\u0024\\u"
      "2222\\u80\\U0001F600\\U41\\u'",
      "f () \n"
      "{ \n"
      "    x='\001\001\177A\004g\\xgA\\8\?\\z' y='$\\u2222\\u0080\\U0"
      "001F600A\\u'\n"
      "}\n" }
};

#define CASUS_NUMERUS 40

/* lineam primam differentem imprimere (diagnostica) */
interior vacuum
_differentiam_imprimere (
     constans character* titulus,
                 chorda  habitum,
     constans character* exspectatum)
{
    i32 n        = (i32)strlen(exspectatum);
    i32 k        = ZEPHYRUM;
    i32 linea    = I;
    i32 initium  = ZEPHYRUM;

    dum (   k < n && k < habitum.mensura
         && (character)habitum.datum[k] == exspectatum[k])
    {
        si (exspectatum[k] == '\n')
        {
            linea++;
            initium = k + I;
        }
        k++;
    }
    imprimere("    casus '%s': linea %d differt (octetus %d)\n",
        titulus, (integer)linea, (integer)k);
    imprimere("      exspectatum: '%.*s'\n",
        (integer)(n - initium), exspectatum + initium);
    imprimere("      habitum:     '%.*s'\n",
        (integer)(habitum.mensura > initium ? habitum.mensura - initium
            : ZEPHYRUM),
        (constans character*)habitum.datum + initium);
}

interior vacuum
_casum_probare (
                Piscina* piscina,
    constans CasusCoctus* probandum)
{
          i32  mensura;
    character* fons;
    MateriaNodus*        radix;
    CrustaParsura        relatio;
    MateriaScriptura     emissa;
    MateriaScripturaConsilium consilium;
    chorda               coctum;
    i32                  mensura_casus = (i32)strlen(probandum->fons);
    b32                  par;

    mensura = mensura_casus + (i32)IX;
    fons    = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura + I);
    memcpy(fons, "f() {\n", VI);
    memcpy(fons + VI, probandum->fons, (size_t)mensura_casus);
    memcpy(fons + VI + mensura_casus, "\n}\n", III);
    fons[mensura] = '\0';

    radix = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
        &relatio);
    CREDO_NON_NIHIL (radix);
    si (radix == NIHIL)
    {
        redde;
    }
    si (!relatio.sana)
    {
        imprimere("    casus '%s': parsura non sana (mala %d)\n",
            probandum->titulus, (integer)relatio.mala);
    }
    CREDO_VERUM (relatio.sana);

    /* lex octetorum ante visionem */
    materia_scriptura_consilium_nudum(&consilium, &CRUSTA_REGISTRUM);
    emissa = materia_scribere_nodum(piscina, radix, &consilium);
    CREDO_VERUM (   emissa.successus && emissa.textus.mensura == mensura
                 && memcmp(emissa.textus.datum, fons, (size_t)mensura)
                    == ZEPHYRUM);

    coctum = crusta_coctum_scribere(piscina, radix);
    CREDO_NON_NIHIL (coctum.datum);
    par = coctum.datum != NIHIL
        && coctum.mensura == (i32)strlen(probandum->exspectatum)
        && memcmp(coctum.datum, probandum->exspectatum,
            (size_t)coctum.mensura) == ZEPHYRUM;
    si (!par && coctum.datum != NIHIL)
    {
        _differentiam_imprimere(probandum->titulus, coctum,
            probandum->exspectatum);
    }
    CREDO_VERUM (par);
}

/* programma sine involucro: textus coctus pinnatus (forma nostra, non
 * oraculi - modus functionis gradu zephyro) */
interior chorda
_coquere (
               Piscina* piscina,
    constans character* litterae)
{
          i32  mensura = (i32)strlen(litterae);
    character* fons    = (character*)piscina_allocare(piscina,
        (memoriae_index)mensura + I);
    MateriaNodus* radix;
    chorda        vacua;

    memcpy(fons, litterae, (size_t)mensura + I);
    radix = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
        NIHIL);
    si (radix == NIHIL)
    {
        vacua.mensura  = ZEPHYRUM;
        vacua.datum    = NIHIL;
        redde vacua;
    }
    redde crusta_coctum_scribere(piscina, radix);
}

s32
principale (vacuum)
{
         b32  praeteritus;
     Piscina* piscina;
         i32  numerus;
         i32  k;
      chorda  c;

    piscina = piscina_generare_dynamicum("probatio_crusta_coctum",
        1048576);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ==================================================
     * PROBARE: casus contra 'declare -f' bash 5.2.15
     * ================================================== */

    imprimere("\n--- Probans casus contra declare -f ---\n");

    numerus = (i32)(magnitudo(CASUS) / magnitudo(CASUS[ZEPHYRUM]));
    per (k = ZEPHYRUM; k < numerus; k++)
    {
        _casum_probare(piscina, &CASUS[k]);
    }
    imprimere("    casus %d\n", (integer)numerus);
    CREDO_AEQUALIS_I32 (numerus, (i32)CASUS_NUMERUS);
    CREDO_VERUM (numerus >= (i32)XII);


    /* ==================================================
     * PROBARE: contractus (radix NIHIL, programma vacuum, radix
     * programmatis)
     * ================================================== */

    imprimere("\n--- Probans contractum ---\n");

    c = crusta_coctum_scribere(piscina, NIHIL);
    CREDO_NON_NIHIL (c.datum);
    CREDO_AEQUALIS_I32 (c.mensura, ZEPHYRUM);

    c = _coquere(piscina, "");
    CREDO_NON_NIHIL (c.datum);
    CREDO_AEQUALIS_I32 (c.mensura, ZEPHYRUM);

    c = _coquere(piscina, "# solum commentum\n\n");
    CREDO_NON_NIHIL (c.datum);
    CREDO_AEQUALIS_I32 (c.mensura, ZEPHYRUM);

    /* programma: modus functionis gradu zephyro, '\n' finalis */
    c = _coquere(piscina, "a;b\n\nc &");
    CREDO_CHORDA_AEQUALIS_LITERIS (c, "a;\nb;\nc &\n");

    /* functio in radice: forma 'declare -f' etiam cum 'function' */
    c = _coquere(piscina, "function g { :; }");
    CREDO_CHORDA_AEQUALIS_LITERIS (c, "g () \n{ \n    :\n}\n");

    /* nodus sententiae sine programmate */
    {
              i32 mensura = III;
        character fons[IV];
        MateriaNodus* radix;
        MateriaNodus* sententia;

        memcpy(fons, "a|b", IV);
        radix = crusta_arbor_parsare(piscina, fons, mensura,
            &CRUSTA_BASH, NIHIL);
        CREDO_NON_NIHIL (radix);
        si (radix != NIHIL)
        {
            sententia = materia_valor_lista_obtinere(
                radix->loci[CRUSTA_PROGRAMMA_LIBERI],
                ZEPHYRUM)->datum.nodus;
            c = crusta_coctum_scribere(piscina, sententia);
            CREDO_CHORDA_AEQUALIS_LITERIS (c, "a | b\n");
        }
    }

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
