/* probatio_crusta_canon.c - Porta canonis crustae (P8)
 *
 * crusta/grammatica/crusta.canon MANU SCRIPTUM est (exemplar
 * html.canon; decretum Frani 2026-09-01: registrum ex declaratione
 * generatur, canon manu). Canon manu scriptus SILENTER putrescit:
 * regula quae genus renominatum adhuc describit nihil custodit, regula
 * absens 'elementum ignotum' solum in iudicio dat. Ergo DUO custodes
 * praeter iudicium ipsum:
 *
 *  I.  CUSTOS DERIVAE (bidirectionalis): canon ut STML legitur et
 *      regulae eius contra TRES tabulas retiuntur - registrum
 *      generum (genus quodque regulam globalem UNAM), loci (locus
 *      quisque regulam intra= genus suum UNAM: CLXXVI), lexicon (genus
 *      lexematis quodque regulam 'crusta-' UNAM: L) - et involucrum
 *      (arbor, ante, post). REVERSUM: regula omnis canonis uni
 *      tabularum congruere debet, ne regula rancida taceat.
 *  II. PINNA SIGILLI: 'registrum-sigillum' in canone ELECTIO unius
 *      optionis; optio contra materia_arbor_sigillum VIVUM conferitur.
 *      Declaratio mutata sine canone mutato rubet hic ET in iudicio.
 *
 * IUDICIUM: documentum quodque quod porta P7 parit (casus inlinei
 * CII, casus fixorum XLIX per lectorem casuum, corpus totum: domus +
 * fixa + FreeBSD) contra canonem iudicatur - vitia ZEPHYRUM; numerus
 * documentorum se metitur. Casus duo fixorum a scriptore STML
 * RECUSANTUR (limites nominati P7: sequentia claudens, margo alba
 * mixta) - iudicari non possunt, per causam numerantur.
 *
 * SPECIES LOCORUM (G3): quid aedificator in locum quemque ponat MANU
 * ex arboribus MENSURATUM (probe super documenta omnia P7 et casus
 * angulares), non ex fonte aedificatoris lectum; aedificator mutatus
 * canonem mutare debet - haec porta id falsificat.
 *
 * TAG LEXEMATIS hic iterum mangulatur ('crusta-' + minusculae, '_'
 * -> '-'): si scriptor aliter mangulet, iudicium corporis rubet, si
 * hic aliter, custos derivae rubet. Ambo simul mentiri non possunt.
 */

#include "latina.h"
#include "credo.h"
#include "canon.h"
#include "chorda.h"
#include "crusta_arbor.h"
#include "crusta_exempla.h"
#include "crusta_lector.h"
#include "crusta_lexicon.h"
#include "crusta_registrum.h"
#include "internamentum.h"
#include "materia_arbor.h"
#include "materia_lexicon.h"
#include "materia_nodus.h"
#include "materia_token.h"
#include "piscina.h"
#include "stml.h"
#include "xar.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* casus inlinei portae P7 (porta arboris XC + mala VII, porta
 * arithmeticae V) verbatim */
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
    "fi",
    ")",
    ";;",
    "}",
    "; ;",
    "then",
    "a )",
    "$(( ))",
    "(( i += 1 )) > f && x",
    "echo $((1+1)) $((echo a); b)",
    "$((1 +))",
    "$((1+1",
    /* casus angulares P8: formae quas corpus non fert (species
     * locorum mensuratae) */
    "echo $((x ? 1 : 2)) $((-x + ~y + !z + ++a + b--)) $(( (1+2)*3 ))",
    "echo $((a[1] + ${b} + $(c) + $((d)) + \"1\"))",
    "for i in a; do :; done > f",
    "for ((;;)); do :; done 2>&1",
    "case x in a) esac > f",
    "[[ a ]] > f",
    "f() if a; then b; fi",
    "f() for ((;;)); do :; done",
    "coproc c [[ a ]]",
    "a | time b",
    "a && coproc x",
    "a | for i in x; do :; done",
    "if a; then b; elif cat <<A\nx\nA\nthen c; else d ); fi",
    "cat <<A\n\\$ \\\n ${x} $((1)) `b` $(c)\nA\n",
    "echo ${x:-$(cat <<A\nx\nA\n)}",
    "a=(1 ;",
    "case x in a b) ;; ) esac",
    "for if in a; do :; done",
    "[[ ! ( a ) && ( b == c ) || -f d ]]",
    "x=1 ((1)); x=1 [[ a ]]",
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

#define REGULAE_MAXIMAE 320

/* recusationes scriptoris STML per causam nominatam (P7) */
#define SEQUENTIA_CLAUDENS "valor sequentiam claudentem fert"
#define TEXTUI_NON_TUTUS   "valor lexematis textui non tutus (mixtum)"

nomen structura {
    i32 iudicata;
    i32 vitiosa;
    i32 recusata_sequentia;
    i32 recusata_mixtum;
    i32 recusata_alia;
    i32 fracta;
} Summa;


/* Chorda contra literas C - sine cast, sine allocatione */
interior b32
_aequalis_literis (
                chorda  c,
    constans character* literae)
{
    i32 mensura = (i32)strlen(literae);

    si (c.mensura != mensura)
    {
        redde FALSUM;
    }
    si (mensura == ZEPHYRUM)
    {
        redde VERUM;
    }
    redde (b32)(memcmp(c.datum, literae, (size_t)mensura) == ZEPHYRUM);
}


/* Regula canonis, lecta ex STML canonis ipsius */
nomen structura {
     StmlNodus* nodus;
        chorda  titulus_regulae;
        chorda  intra;         /* mensura ZEPHYRUM = globalis */
           b32  congruens;     /* custos reversus */
} RegulaCanonis;


interior character*
_plagulam_legere (
               Piscina* piscina,
    constans character* radix,
    constans character* via,
                   i32* mensura)
{
     character plena[4096];
         FILE*  f;
        longus  longitudo;
    character*  memoria;
        size_t  lecti;

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

/* Liberum elementare titulo dato invenire (primum) */
interior StmlNodus*
_liberum_invenire (
             StmlNodus* parens,
    constans character* titulus)
{
    i32 n;
    i32 i;

    si (parens == NIHIL || parens->liberi == NIHIL)
    {
        redde NIHIL;
    }
    n = xar_numerus(parens->liberi);
    per (i = ZEPHYRUM; i < n; i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(parens->liberi, i);

        si (   l->genus == STML_NODUS_ELEMENTUM && l->titulus != NIHIL
            && _aequalis_literis(*l->titulus, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

/* Liberum 'attributum' cuius nomen= datum est */
interior StmlNodus*
_attributum_regulae_invenire (
             StmlNodus* regula,
    constans character* titulus)
{
    i32 n;
    i32 i;

    n = xar_numerus(regula->liberi);
    per (i = ZEPHYRUM; i < n; i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(regula->liberi, i);
           chorda* attributi_titulus;

        si (l->genus != STML_NODUS_ELEMENTUM || l->titulus == NIHIL)
        {
            perge;
        }
        si (!_aequalis_literis(*l->titulus, "attributum"))
        {
            perge;
        }
        attributi_titulus = stml_attributum_capere(l, "nomen");
        si (   attributi_titulus != NIHIL
            && _aequalis_literis(*attributi_titulus, titulus))
        {
            redde l;
        }
    }
    redde NIHIL;
}

interior i32
_regulas_colligere (
        StmlNodus* radix,
    RegulaCanonis* regulae)
{
    i32 n;
    i32 i;
    i32 numerus = ZEPHYRUM;

    n = xar_numerus(radix->liberi);
    per (i = ZEPHYRUM; i < n && numerus < REGULAE_MAXIMAE; i++)
    {
        StmlNodus* l = *(StmlNodus**)xar_obtinere(radix->liberi, i);
           chorda* titulus_regulae;
           chorda* intra;

        si (l->genus != STML_NODUS_ELEMENTUM || l->titulus == NIHIL)
        {
            perge;
        }
        si (!_aequalis_literis(*l->titulus, "elementum"))
        {
            perge;
        }
        titulus_regulae = stml_attributum_capere(l, "nomen");
        si (titulus_regulae == NIHIL)
        {
            perge;
        }
        intra = stml_attributum_capere(l, "intra");
        regulae[numerus].nodus = l;
        regulae[numerus].titulus_regulae = *titulus_regulae;
        si (intra != NIHIL)
        {
            regulae[numerus].intra = *intra;
        }
        alioquin
        {
            regulae[numerus].intra.datum    = NIHIL;
            regulae[numerus].intra.mensura  = ZEPHYRUM;
        }
        regulae[numerus].congruens  = FALSUM;
        numerus                     = numerus + I;
    }
    redde numerus;
}

/* Quot regulae (titulus, intra) congruant; congruentes notantur.
 * intra NIHIL = regula globalis postulatur. */
interior i32
_regulam_numerare (
         RegulaCanonis* regulae,
                   i32  numerus,
    constans character* titulus,
    constans character* intra)
{
    i32 summa = ZEPHYRUM;
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        b32 titulus_idem;
        b32 intra_idem;

        titulus_idem = _aequalis_literis(regulae[i].titulus_regulae,
            titulus);
        si (intra == NIHIL)
        {
            intra_idem = (b32)(regulae[i].intra.mensura == ZEPHYRUM);
        }
        alioquin
        {
            intra_idem = _aequalis_literis(regulae[i].intra, intra);
        }
        si (titulus_idem && intra_idem)
        {
            regulae[i].congruens  = VERUM;
            summa                 = summa + I;
        }
    }
    redde summa;
}

/* praefixum + minusculae, '_' -> '-' (speculum materia_arbor) */
interior vacuum
_tag_lexematis (
             character* buffer,
    constans character* praefixum,
    constans character* titulus)
{
    i32 i;
    i32 j;

    j = ZEPHYRUM;
    per (i = ZEPHYRUM; praefixum[i] != '\0'; i++)
    {
        buffer[j]  = praefixum[i];
        j          = j + I;
    }
    per (i = ZEPHYRUM; titulus[i] != '\0'; i++)
    {
        character c = titulus[i];

        si (c == '_')
        {
            c = '-';
        }
        alioquin
        {
            c = (character)tolower((insignatus character)c);
        }
        buffer[j]  = c;
        j          = j + I;
    }
    buffer[j] = '\0';
}

/* parsare -> STML -> legere -> iudicare; summa per exitum. Vitia
 * prima X impressa. */
interior vacuum
_documentum_iudicare (
                           Piscina* piscina,
               InternamentumChorda* intern,
                             Canon* canon,
    constans MateriaArborConsilium* consilium,
                constans character* fons,
                               i32  mensura,
                constans character* titulus,
                             Summa* summa)
{
    MateriaNodus* radix;
    MateriaArborScriptura scriptura;
    StmlResultus res;
    Xar* vitia;
    i32 n;
    i32 i;

    radix = crusta_arbor_parsare(piscina, fons, mensura, &CRUSTA_BASH,
        NIHIL);
    si (radix == NIHIL)
    {
        imprimere("  %s: parsator NIHIL\n", titulus);
        summa->fracta++;
        redde;
    }
    scriptura = materia_arbor_scribere_nodum(piscina, radix, consilium);
    si (!scriptura.successus)
    {
        si (   scriptura.causa                             != NIHIL
            && strcmp(scriptura.causa, SEQUENTIA_CLAUDENS) == ZEPHYRUM)
        {
            summa->recusata_sequentia++;
        }
        alioquin si (   scriptura.causa != NIHIL
                     && strcmp(scriptura.causa, TEXTUI_NON_TUTUS)
                            == ZEPHYRUM)
        {
            summa->recusata_mixtum++;
        }
        alioquin
        {
            imprimere("  %s: scriptura STML recusata: %s\n", titulus,
                scriptura.causa ? scriptura.causa : "?");
            summa->recusata_alia++;
        }
        redde;
    }
    res = stml_legere(scriptura.textus, piscina, intern);
    si (!res.successus || res.elementum_radix == NIHIL)
    {
        imprimere("  %s: stml_legere fractum\n", titulus);
        summa->fracta++;
        redde;
    }
    vitia = canon_iudicare(canon, res.elementum_radix, piscina);
    si (vitia == NIHIL)
    {
        imprimere("  %s: canon_iudicare NIHIL (fractura)\n", titulus);
        summa->fracta++;
        redde;
    }
    n = xar_numerus(vitia);
    per (i = ZEPHYRUM; i < n && i < X; i++)
    {
        CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, i);

        imprimere("  %s: VITIUM %s", titulus, canon_nuntius(v->genus));
        si (v->elementum != NIHIL)
        {
            imprimere(" <%.*s>", (integer)v->elementum->mensura,
                (constans character*)v->elementum->datum);
        }
        si (v->detail != NIHIL)
        {
            imprimere(" '%.*s'", (integer)v->detail->mensura,
                (constans character*)v->detail->datum);
        }
        imprimere("\n");
    }
    summa->iudicata++;
    si (n > ZEPHYRUM)
    {
        summa->vitiosa++;
    }
}

/* plagulam unam iudicare, piscina propria (arbor + STML + documentum
 * STML per plagulam; intern communis) */
interior vacuum
_plagulam_iudicare (
               InternamentumChorda* intern,
                             Canon* canon,
    constans MateriaArborConsilium* consilium,
                constans character* radix,
                constans character* via,
                             Summa* summa)
{
    Piscina* piscina = piscina_generare_dynamicum("crusta_canon",
        4194304);
                   i32  mensura;
    constans character* fons;

    si (piscina == NIHIL)
    {
        imprimere("    piscina deficit: %s\n", via);
        summa->fracta++;
        redde;
    }
    fons = _plagulam_legere(piscina, radix, via, &mensura);
    si (fons == NIHIL)
    {
        imprimere("    illegibilis: %s\n", via);
        summa->fracta++;
        piscina_destruere(piscina);
        redde;
    }
    _documentum_iudicare(piscina, intern, canon, consilium, fons,
        mensura, via, summa);
    piscina_destruere(piscina);
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
    imprimere("  %s: iudicata %d (vitiosa %d), recusata sequentia %d "
        "mixtum %d alia %d, fracta %d\n", titulus,
        (integer)summa->iudicata, (integer)summa->vitiosa,
        (integer)summa->recusata_sequentia,
        (integer)summa->recusata_mixtum,
        (integer)summa->recusata_alia, (integer)summa->fracta);
}

s32
principale (vacuum)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
      constans character* radix_viae;
                   Canon* canon;
                  chorda  fons_canonis;
                  chorda  causa;
     MateriaLexiconRatum  ratum;
      MateriaLexIudicium  iudicium;
   MateriaArborConsilium  consilium;
           RegulaCanonis  regulae[REGULAE_MAXIMAE];
                     i32  numerus_regularum;
                   Summa  casuum;
                   Summa  fixorum;
                   Summa  corporis;
                     b32  praeteritus;
                     i32  i;

    piscina = piscina_generare_dynamicum("probatio_crusta_canon",
        (memoriae_index)16 * M * M);
    si (!piscina)
    {
        imprimere("FRACTA: piscina_generatio\n");
        redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);
    memset(&casuum, ZEPHYRUM, magnitudo(casuum));
    memset(&fixorum, ZEPHYRUM, magnitudo(fixorum));
    memset(&corporis, ZEPHYRUM, magnitudo(corporis));

    radix_viae = getenv("RHUBARB_RADIX");
    si (radix_viae == NIHIL)
    {
        radix_viae = ".";
    }
    si (!materia_lexicon_ratum_facere(&ratum, &CRUSTA_LEXICON,
            &iudicium))
    {
        imprimere("LEXICON CRUSTAE RECUSATUM\n");
        redde I;
    }
    materia_arbor_consilium_nudum(&consilium, &CRUSTA_REGISTRUM, &ratum,
        "crusta");
    consilium.forma.mensura_caudae = (i32)magnitudo(CrustaCauda);


    /* ==================================================
     * CANONEM ONERARE
     * ================================================== */

    imprimere("\n--- Canonem onerans ---\n");
    {
        character* textus;
              i32  mensura = ZEPHYRUM;

        textus = _plagulam_legere(piscina, radix_viae,
            "crusta/grammatica/crusta.canon", &mensura);
        CREDO_NON_NIHIL (textus);
        si (textus == NIHIL)
        {
            imprimere("  canon ABEST\n");
            credo_imprimere_compendium();
            redde I;
        }
        fons_canonis.datum    = (i8*)textus;
        fons_canonis.mensura  = mensura;
    }
    causa.datum = NIHIL;
    causa.mensura = ZEPHYRUM;
    canon = canon_legere(fons_canonis, piscina, intern, &causa);
    CREDO_NON_NIHIL (canon);
    si (canon == NIHIL)
    {
        si (causa.datum != NIHIL)
        {
            imprimere("  CAUSA: %.*s\n", (integer)causa.mensura,
                (constans character*)causa.datum);
        }
        credo_imprimere_compendium();
        redde I;
    }


    /* ==================================================
     * CUSTOS DERIVAE: regulae contra tres tabulas, utrimque
     * ================================================== */

    imprimere("\n--- Custos derivae ---\n");
    {
        StmlResultus res;

        res = stml_legere(fons_canonis, piscina, intern);
        CREDO_VERUM (res.successus);
        numerus_regularum = _regulas_colligere(res.elementum_radix,
            regulae);
        CREDO_MAIOR_I32 (numerus_regularum, ZEPHYRUM);
        CREDO_MINOR_I32 (numerus_regularum, (i32)REGULAE_MAXIMAE);
    }

    /* (a) genera: regula globalis una quaeque */
    per (i = ZEPHYRUM; i < CRUSTA_REGISTRUM.numerus_generum; i++)
    {
        constans character* titulus;
                       i32  quot;

        titulus = CRUSTA_REGISTRUM.genera[i].titulus;
        quot    = _regulam_numerare(regulae, numerus_regularum, titulus,
            NIHIL);

        si (quot != I)
        {
            imprimere("  genus '%s': regulae %d (I exspectata)\n",
                titulus, (integer)quot);
        }
        CREDO_AEQUALIS_I32 (quot, I);
    }
    CREDO_AEQUALIS_I32 (CRUSTA_REGISTRUM.numerus_generum,
        (i32)CRUSTA_GENUS_NUMERUS_GENERUM);

    /* (b) loci: regula intra= genus suum una quaeque */
    {
        i32 loci = ZEPHYRUM;

        per (i = ZEPHYRUM; i < CRUSTA_REGISTRUM.numerus_generum; i++)
        {
            constans MateriaTabGenus* genus =
                &CRUSTA_REGISTRUM.genera[i];
                                 i32 j;

            per (j = ZEPHYRUM; j < genus->loci_numerus; j++)
            {
                constans character* locus =
                    CRUSTA_REGISTRUM.loci[genus->loci_offset
                        + j].titulus;
                i32 quot = _regulam_numerare(regulae, numerus_regularum,
                    locus, genus->titulus);

                si (quot != I)
                {
                    imprimere("  locus '%s' intra '%s': regulae %d\n",
                        locus, genus->titulus, (integer)quot);
                }
                CREDO_AEQUALIS_I32 (quot, I);
                loci++;
            }
        }
        CREDO_AEQUALIS_I32 (loci, (i32)CLXXVI);
    }

    /* (c) lexemata: regula 'crusta-' una quaeque */
    per (i = ZEPHYRUM; i < CRUSTA_LEXICON.numerus_generum; i++)
    {
        character tag[LXIV];
              i32 quot;

        _tag_lexematis(tag, CRUSTA_LEXICON.praefixum_tagi,
            CRUSTA_LEXICON.genera[i].titulus);
        quot = _regulam_numerare(regulae, numerus_regularum, tag,
            NIHIL);
        si (quot != I)
        {
            imprimere("  lexema '%s': regulae %d\n", tag,
                (integer)quot);
        }
        CREDO_AEQUALIS_I32 (quot, I);
    }
    CREDO_AEQUALIS_I32 (CRUSTA_LEXICON.numerus_generum,
        (i32)CRUSTA_LEX_NUMERUS_GENERUM);

    /* (d) involucrum */
    CREDO_AEQUALIS_I32 (_regulam_numerare(regulae, numerus_regularum,
        "arbor", NIHIL), I);
    CREDO_AEQUALIS_I32 (_regulam_numerare(regulae, numerus_regularum,
        "ante", NIHIL), I);
    CREDO_AEQUALIS_I32 (_regulam_numerare(regulae, numerus_regularum,
        "post", NIHIL), I);

    /* (e) REVERSUM: regula omnis alicui tabulae congruit */
    {
        i32 congruentes = ZEPHYRUM;

        per (i = ZEPHYRUM; i < numerus_regularum; i++)
        {
            si (regulae[i].congruens)
            {
                congruentes = congruentes + I;
            }
            alioquin
            {
                imprimere("  regula RANCIDA: '%.*s'",
                    (integer)regulae[i].titulus_regulae.mensura,
                    (constans character*)
                        regulae[i].titulus_regulae.datum);
                si (regulae[i].intra.mensura > ZEPHYRUM)
                {
                    imprimere(" intra '%.*s'",
                        (integer)regulae[i].intra.mensura,
                        (constans character*)regulae[i].intra.datum);
                }
                imprimere("\n");
            }
        }
        CREDO_AEQUALIS_I32 (congruentes, numerus_regularum);
        imprimere("  regulae %d, omnes congruentes\n",
            (integer)numerus_regularum);
    }

    /* (II) PINNA SIGILLI: optio canonis == sigillum vivum */
    {
           chorda  sigillum_vivum;
        StmlNodus* arbor = NIHIL;
        StmlNodus* attributum;
        StmlNodus* optio;

        sigillum_vivum = materia_arbor_sigillum(piscina,
            &CRUSTA_REGISTRUM);
        per (i = ZEPHYRUM; i < numerus_regularum; i++)
        {
            si (   regulae[i].intra.mensura == ZEPHYRUM
                && _aequalis_literis(regulae[i].titulus_regulae,
                                     "arbor"))
            {
                arbor = regulae[i].nodus;
            }
        }
        CREDO_NON_NIHIL (arbor);
        attributum = arbor ? _attributum_regulae_invenire(arbor,
            "registrum-sigillum") : NIHIL;
        CREDO_NON_NIHIL (attributum);
        optio = attributum ? _liberum_invenire(attributum, "optio")
                           : NIHIL;
        CREDO_NON_NIHIL (optio);
        si (optio != NIHIL)
        {
            chorda pinna = stml_textus_normalizatus(optio, piscina);

            imprimere("  sigillum: pinna %.*s, vivum %.*s\n",
                (integer)pinna.mensura,
                (constans character*)pinna.datum,
                (integer)sigillum_vivum.mensura,
                (constans character*)sigillum_vivum.datum);
            CREDO_VERUM (chorda_aequalis(pinna, sigillum_vivum));
        }
    }


    /* ==================================================
     * IUDICIUM: casus inlinei
     * ================================================== */

    imprimere("\n--- Iudicans casus inlineos (%d) ---\n",
        (integer)NUMERUS_CASUUM);
    per (i = ZEPHYRUM; i < NUMERUS_CASUUM; i++)
    {
        character titulus[LXIV];

        sprintf(titulus, "casus %d", (integer)i);
        _documentum_iudicare(piscina, intern, canon, &consilium,
            CASUS[i], (i32)strlen(CASUS[i]), titulus, &casuum);
    }
    _summam_imprimere("casus", &casuum);
    CREDO_AEQUALIS_I32 (casuum.iudicata, NUMERUS_CASUUM);
    CREDO_AEQUALIS_I32 (casuum.vitiosa, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (casuum.fracta, ZEPHYRUM);


    /* ==================================================
     * IUDICIUM: casus fixorum per lectorem casuum
     * ================================================== */

    imprimere("\n--- Iudicans casus fixorum ---\n");
    {
        hic_manens constans character* FIXA[] = {
            "probationes/fixa/crusta/pathologiae.sh",
            "probationes/fixa/crusta/adversarius.sh"
        };
        i32 k;

        per (k = ZEPHYRUM; k < II; k++)
        {
                           i32  mensura;
            constans character* fons = _plagulam_legere(piscina,
                radix_viae, FIXA[k], &mensura);
                           Xar* exempla;

            CREDO_NON_NIHIL (fons);
            si (fons == NIHIL)
            {
                perge;
            }
            exempla = crusta_exempla_legere(piscina, fons, mensura);
            CREDO_NON_NIHIL (exempla);
            si (exempla == NIHIL)
            {
                perge;
            }
            per (i = ZEPHYRUM; i < xar_numerus(exempla); i++)
            {
                constans CrustaExemplum* e =
                    (constans CrustaExemplum*)xar_obtinere(exempla, i);
                character titulus[CXXVIII];

                sprintf(titulus, "%s casus %d", FIXA[k],
                    (integer)e->numerus);
                _documentum_iudicare(piscina, intern, canon, &consilium,
                    (constans character*)e->datum.datum,
                    e->datum.mensura, titulus, &fixorum);
            }
        }
    }
    _summam_imprimere("fixa", &fixorum);
    /* XXXVI + XVII casus; duo a scriptore recusati (limites P7) */
    CREDO_AEQUALIS_I32 (fixorum.iudicata, (i32)LI);
    CREDO_AEQUALIS_I32 (fixorum.vitiosa, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (fixorum.recusata_sequentia, (i32)I);
    CREDO_AEQUALIS_I32 (fixorum.recusata_mixtum, (i32)I);
    CREDO_AEQUALIS_I32 (fixorum.recusata_alia, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (fixorum.fracta, ZEPHYRUM);


    /* ==================================================
     * IUDICIUM: corpus totum (domus, fixa totae, FreeBSD)
     * ================================================== */

    imprimere("\n--- Iudicans corpus ---\n");
    {
        constans character* lista;
                       i32  mensura;

        lista = _plagulam_legere(piscina, radix_viae,
            "build/crusta_corpus.lst", &mensura);
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
                        _plagulam_iudicare(intern, canon, &consilium,
                            radix_viae, via, &corporis);
                    }
                }
                ab = ad + I;
            }
        }
    }
    CREDO_VERUM (corporis.iudicata >= (i32)CCXXX);
    _plagulam_iudicare(intern, canon, &consilium, radix_viae,
        "probationes/fixa/crusta/pathologiae.sh", &corporis);
    _plagulam_iudicare(intern, canon, &consilium, radix_viae,
        "probationes/fixa/crusta/adversarius.sh", &corporis);
    per (i = ZEPHYRUM; CRUSTA_FREEBSD_PLAGULAE[i] != NIHIL; i++)
    {
        _plagulam_iudicare(intern, canon, &consilium, radix_viae,
            CRUSTA_FREEBSD_PLAGULAE[i], &corporis);
    }
    _summam_imprimere("corpus", &corporis);
    CREDO_AEQUALIS_I32 (corporis.vitiosa, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (corporis.recusata_sequentia, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (corporis.recusata_mixtum, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (corporis.recusata_alia, ZEPHYRUM);
    CREDO_AEQUALIS_I32 (corporis.fracta, ZEPHYRUM);

    /* se metiens: documenta iudicata = casus + fixa (- II recusata) +
     * plagulae corporis */
    imprimere("  summa: %d documenta iudicata\n",
        (integer)(casuum.iudicata + fixorum.iudicata
            + corporis.iudicata));
    CREDO_VERUM (casuum.iudicata + fixorum.iudicata + corporis.iudicata
        >= NUMERUS_CASUUM + (i32)XLVII + (i32)CCXXX + (i32)XCI);

    imprimere("\n");
    credo_imprimere_compendium();

    praeteritus = credo_omnia_praeterierunt();
    redde praeteritus ? ZEPHYRUM : I;
}
