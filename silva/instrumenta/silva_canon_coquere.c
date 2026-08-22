/* silva_canon_coquere.c - Proiectio grammaticae in canonem STML */

#include "silva_canon_coquere.h"
#include "silva_nodus.h"
#include "silva_token.h"
#include "piscina.h"
#include "xar.h"
#include "chorda_aedificator.h"
#include "friatio.h"
#include <stdio.h>
#include <string.h>

hic_manens constans character* HEX_CIFRAE = "0123456789abcdef";

/* Basis viae (post ultimum solidum).
 *
 * CUR: artificium COMMISSUM functio PURA introituum suorum esse
 * debet. Via exitus et via grammaticae ab INVOCATIONE pendent
 * (generare.sh ex silva/ currit, porta ex radice), ergo eas in
 * caput scribere duas regenerationes RECTAS dissentire faceret -
 * et porta quae exitum confert de re innoxia clamaret. */
interior constans character*
_basis (constans character* via)
{
    constans character* ultimum;

    si (via == NIHIL)
    {
        redde "";
    }
    ultimum = strrchr(via, '/');
    redde (ultimum != NIHIL) ? ultimum + I : via;
}

/* ==================================================
 * Auxiliares
 * ================================================== */

/* Imprimere chordam (non terminata nullo) */
interior vacuum
_ch (FILE* pl, constans chorda* c)
{
    si (c != NIHIL)
    {
        fprintf(pl, "%.*s", (int)c->mensura, (constans character*)c->datum);
    }
}

/* Tag lexematis: 'lex-' + nomen generis minusculum, lineolis pro
 * lineis subscriptis.
 *
 * MANGULATIO REPETITA, non communicata: scriptor eam in
 * silva_arbor.c tenet, et arborem huc trahere generatorem a MOTORE
 * pendere faceret - inversio directionis quam ordo aedificationis
 * vetat (generator tabulas gignit quas motor consumit).
 *
 * Ergo AEQUALITAS ASSERITUR, non structuraliter praestatur:
 * probatio_silva_canon per genera XCV currit et hunc exitum cum
 * silva_arbor_lexema_tag comparat. Divergentia probationem NOMINE
 * frangit - quod tacita communicatio non praestaret. */
interior vacuum
_lexema_tag (FILE* pl, SilvaLexemaGenus genus)
{
    constans character* titulus;
                   i32  i;

    titulus = silva_lexema_genus_nomen(genus);
    si (titulus == NIHIL)
    {
        redde;
    }
    fprintf(pl, "lex-");
    per (i = ZEPHYRUM; titulus[i] != '\0'; i++)
    {
        character c = titulus[i];

        si (c >= 'A' && c <= 'Z')
        {
            c = (character)(c - 'A' + 'a');
        }
        alioquin si (c == '_')
        {
            c = '-';
        }
        fputc(c, pl);
    }
}

/* An elementum lexematis TEXTUM ferre possit.
 *
 * Regula scriptoris duabus semitis divisa est:
 *   - silva_arbor_valor_portandus: verum nisi EOF aut orthographia
 *     fixa (illa in genere ipso vivit, ergo non scribitur)
 *   - _trivium_scribere: SPATIA/TABULAE numerum 'n=' ferunt,
 *     NOVA_LINEA/CONTINUATIO vexillum 'crlf' - nullum textum
 *
 * Tabula ORTHOGRAPHIAE lateri arboris est (hic_manens), ergo
 * conditio hic ENUMERATUR. Enumeratio a probatione custoditur:
 * probatio_silva_canon per genera XCV currit et hoc praedicatum
 * cum silva_arbor_valor_portandus comparat, carve-out triviorum
 * SEPARATIM asserto. */
interior b32
_textum_fert (SilvaLexemaGenus genus)
{
    commutatio (genus)
    {
        casus SILVA_LEX_OCTETUS_IGNOTUS:
        casus SILVA_LEX_STRING_IMPERFECTUM:
        casus SILVA_LEX_CHARACTER_IMPERFECTUM:
        casus SILVA_LEX_IDENTIFICATOR:
        casus SILVA_LEX_INTEGER:
        casus SILVA_LEX_FLOAT:
        casus SILVA_LEX_CHARACTER_LIT:
        casus SILVA_LEX_STRING_LIT:
        casus SILVA_LEX_COMMENTUM_CLAUSUM:
        casus SILVA_LEX_COMMENTUM_LINEA:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* An genus trivium sit (in <ante>/<post> solum sedens) */
interior b32
_trivium_est (SilvaLexemaGenus genus)
{
    commutatio (genus)
    {
        casus SILVA_LEX_SPATIA:
        casus SILVA_LEX_TABULAE:
        casus SILVA_LEX_NOVA_LINEA:
        casus SILVA_LEX_CONTINUATIO:
        casus SILVA_LEX_COMMENTUM_CLAUSUM:
        casus SILVA_LEX_COMMENTUM_LINEA:
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* Omnia lexemata ut liberos emittere (vocabularium clausum:
 * canon classes substitutionis non habet - consulto, spec par. 6) */
interior vacuum
_lexemata_liberos (FILE* pl, constans character* margo)
{
    i32 i;

    per (i = ZEPHYRUM; i < (i32)SILVA_LEX_NUMERUS_GENERUM; i++)
    {
        si ((SilvaLexemaGenus)i == SILVA_LEX_EOF)
        {
            perge;    /* EOF in <cauda> solum sedet */
        }
        fprintf(pl, "%s<liberum nomen=\"", margo);
        _lexema_tag(pl, (SilvaLexemaGenus)i);
        fprintf(pl, "\"/>\n");
    }
}

/* Attributa sedis, communia nodis et lexematibus portatis */
interior vacuum
_attributa_sedis (FILE* pl, constans character* margo)
{
    fprintf(pl,
        "%s<attributum nomen=\"b\"       genus=\"numerus\"/>\n"
        "%s<attributum nomen=\"linea\"   genus=\"numerus\"/>\n"
        "%s<attributum nomen=\"columna\" genus=\"numerus\"/>\n"
        "%s<attributum nomen=\"linea-initium\" genus=\"electio\"\n"
        "%s  nota=\"attributum NUDUM - stml praesentiam ut 'true'\n"
        "%s        internat, ergo electio, non veritas (illa\n"
        "%s        'verum|falsum' poscit)\">\n"
        "%s  <optio>true</optio>\n"
        "%s</attributum>\n",
        margo, margo, margo, margo, margo, margo, margo, margo, margo);
}

/* ==================================================
 * Involucrum: forma SCRIPTORIS, non grammaticae
 *
 * Haec pars derivari NEQUIT - nullum eius vestigium in grammatica
 * est. Manu scribitur et a porta corporis falsificatur.
 * ================================================== */

interior vacuum
_involucrum_scribere (FILE* pl, Xar* genera_radicis)
{
    i32 i;

    fprintf(pl,
        "  <!-- =============== INVOLUCRUM ===============\n"
        "       Forma SCRIPTORIS (silva_arbor.c), non grammaticae:\n"
        "       nullum eius vestigium in c89.stml est, ergo manu\n"
        "       scriptum. Porta corporis (probatio_silva_canon) eam\n"
        "       falsificat, non derivatio.\n"
        "       -->\n\n");

    /* --- radix --- */
    fprintf(pl,
        "  <elementum nomen=\"parsura\" radix=\"verum\">\n"
        "    <attributum nomen=\"grammatica\" genus=\"nomen\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"registrum-sigillum\" genus=\"nomen\"\n"
        "      necessarium=\"verum\"\n"
        "      nota=\"sigillum FNV-1a tabularum, hexadecimale\"/>\n"
        "    <attributum nomen=\"fons-princeps\" genus=\"numerus\"/>\n"
        "    <liberum nomen=\"fontes\" minimum=\"1\" maximum=\"1\"/>\n"
        "    <liberum nomen=\"cauda\"  maximum=\"1\"/>\n"
        "    <liberum nomen=\"directiva\"/>\n"
        "    <liberum nomen=\"invocatio-vacua\"/>\n"
        "    <liberum nomen=\"regio-directiva\"/>\n"
        "    <liberum nomen=\"regio-cruda\"/>\n"
        "    <liberum nomen=\"regio-finis\"/>\n");
    fprintf(pl,
        "    <!-- POLITIA, non derivatum: error et conditionalis in\n"
        "         gradu supremo SOLO (spec par. 0.1 cautio II) -\n"
        "         INFERTUM ex duabus sedibus constructionis, ergo\n"
        "         hic positum ut corpus id falsificet. -->\n"
        "    <liberum nomen=\"error\"/>\n"
        "    <liberum nomen=\"conditionalis\"/>\n"
        "    <liberum nomen=\"ambiguus\"/>\n");
    per (i = ZEPHYRUM; i < xar_numerus(genera_radicis); i++)
    {
        chorda** t = (chorda**)xar_obtinere(genera_radicis, i);

        si (t == NIHIL || *t == NIHIL)
        {
            perge;
        }
        fprintf(pl, "    <liberum nomen=\"");
        _ch(pl, *t);
        fprintf(pl, "\"/>\n");
    }
    fprintf(pl, "  </elementum>\n\n");

    /* --- tabula fontium --- */
    fprintf(pl,
        "  <elementum nomen=\"fontes\">\n"
        "    <liberum nomen=\"fons\" minimum=\"1\"\n"
        "      nota=\"tabula vacua mendacium esset: lexema quodlibet\n"
        "            fontem nominare potest\"/>\n"
        "  </elementum>\n\n"
        "  <elementum nomen=\"fons\">\n"
        "    <attributum nomen=\"index\" genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"via\"   genus=\"textus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"lexicon\" genus=\"electio\">\n"
        "      <optio>true</optio>\n"
        "    </attributum>\n"
        "    <attributum nomen=\"syntheticus\" genus=\"electio\">\n"
        "      <optio>true</optio>\n"
        "    </attributum>\n"
        "  </elementum>\n\n");

    /* --- cauda --- */
    fprintf(pl,
        "  <elementum nomen=\"cauda\"\n"
        "    nota=\"lexema EOF trivia caudae plagulae fert - ergo\n"
        "          documentum octetos finales non perdit\">\n");
    _attributa_sedis(pl, "    ");
    fprintf(pl,
        "    <liberum nomen=\"lex-eof\" minimum=\"1\" maximum=\"1\"/>\n"
        "  </elementum>\n\n");

    /* --- reinserenda: lamina lexematum quam arbor non fert --- */
    fprintf(pl,
        "  <!-- REINSERENDA: laminae lexematum quas arbor NON fert\n"
        "       (directivae, invocationes vacuae, regiones non\n"
        "       sumptae). Ordine octetorum inter nodos supremos\n"
        "       inseruntur. -->\n\n");

    fprintf(pl, "  <elementum nomen=\"directiva\">\n");
    _attributa_sedis(pl, "    ");
    _lexemata_liberos(pl, "    ");
    fprintf(pl, "  </elementum>\n\n");

    fprintf(pl,
        "  <elementum nomen=\"invocatio-vacua\"\n"
        "    nota=\"invocatio macri quae ZERO lexemata peperit:\n"
        "          octetos tegit quos arbor non fert\">\n");
    _attributa_sedis(pl, "    ");
    _lexemata_liberos(pl, "    ");
    fprintf(pl, "  </elementum>\n\n");

    fprintf(pl,
        "  <elementum nomen=\"regio-directiva\">\n"
        "    <attributum nomen=\"regio\"       genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"pater\"       genus=\"numerus\"/>\n"
        "    <attributum nomen=\"regio-fons\"  genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"regio-linea\" genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"ramus\"       genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"genus\"       genus=\"electio\">\n"
        "      <optio>if</optio>\n"
        "      <optio>ifdef</optio>\n"
        "      <optio>ifndef</optio>\n"
        "      <optio>elif</optio>\n"
        "      <optio>else</optio>\n"
        "    </attributum>\n"
        "    <attributum nomen=\"conditio\"    genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n");
    _attributa_sedis(pl, "    ");
    _lexemata_liberos(pl, "    ");
    fprintf(pl, "  </elementum>\n\n");

    fprintf(pl,
        "  <elementum nomen=\"regio-cruda\">\n"
        "    <attributum nomen=\"regio\"       genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"pater\"       genus=\"numerus\"/>\n"
        "    <attributum nomen=\"regio-fons\"  genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"regio-linea\" genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"ramus\"       genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"genus\"       genus=\"electio\">\n"
        "      <optio>if</optio>\n"
        "      <optio>ifdef</optio>\n"
        "      <optio>ifndef</optio>\n"
        "      <optio>elif</optio>\n"
        "      <optio>else</optio>\n"
        "    </attributum>\n"
        "    <attributum nomen=\"conditio\"    genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n");
    _attributa_sedis(pl, "    ");
    _lexemata_liberos(pl, "    ");
    fprintf(pl, "  </elementum>\n\n");

    fprintf(pl,
        "  <elementum nomen=\"regio-finis\">\n"
        "    <attributum nomen=\"regio\"       genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"pater\"       genus=\"numerus\"/>\n"
        "    <attributum nomen=\"regio-fons\"  genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"regio-linea\" genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n");
    _attributa_sedis(pl, "    ");
    _lexemata_liberos(pl, "    ");
    fprintf(pl, "  </elementum>\n\n");

    /* --- trivia --- */
    fprintf(pl,
        "  <!-- TRIVIA: unius domini semper (invariatum roundtrip) -->\n\n"
        "  <elementum nomen=\"ante\">\n"
        "    <liberum nomen=\"lex-spatia\"/>\n"
        "    <liberum nomen=\"lex-tabulae\"/>\n"
        "    <liberum nomen=\"lex-nova-linea\"/>\n"
        "    <liberum nomen=\"lex-continuatio\"/>\n"
        "    <liberum nomen=\"lex-commentum-clausum\"/>\n"
        "    <liberum nomen=\"lex-commentum-linea\"/>\n"
        "  </elementum>\n\n"
        "  <elementum nomen=\"post\">\n"
        "    <liberum nomen=\"lex-spatia\"/>\n"
        "    <liberum nomen=\"lex-tabulae\"/>\n"
        "    <liberum nomen=\"lex-nova-linea\"/>\n"
        "    <liberum nomen=\"lex-continuatio\"/>\n"
        "    <liberum nomen=\"lex-commentum-clausum\"/>\n"
        "    <liberum nomen=\"lex-commentum-linea\"/>\n"
        "  </elementum>\n\n"
        "  <elementum nomen=\"scissura\"\n"
        "    nota=\"lamina '\\\\'+nova-linea INTRA lexema\">\n"
        "    <attributum nomen=\"offset\" genus=\"numerus\"\n"
        "      necessarium=\"verum\"/>\n"
        "    <attributum nomen=\"crlf\" genus=\"electio\">\n"
        "      <optio>true</optio>\n"
        "    </attributum>\n"
        "  </elementum>\n\n");

    /* --- origines, NESTATAE intra elementum lexematis --- */
    fprintf(pl,
        "  <!-- ORIGINES: elementum originis INTRA elementum\n"
        "       lexematis sedet, iuxta textum valoris et trivia.\n"
        "       Catenae NESTATIONE LITTERALI: macro ex macro natum\n"
        "       elementum suum INTRA invocationem ferit. -->\n\n");

    fprintf(pl,
        "  <elementum nomen=\"expansio\">\n"
        "    <attributum nomen=\"macro\" genus=\"textus\"/>\n"
        "    <attributum nomen=\"def-f\" genus=\"numerus\"/>\n"
        "    <attributum nomen=\"def-l\" genus=\"numerus\"/>\n"
        "    <attributum nomen=\"def-c\" genus=\"numerus\"/>\n"
        "    <liberum nomen=\"extentum\" maximum=\"1\"/>\n");
    _lexemata_liberos(pl, "    ");
    fprintf(pl, "  </elementum>\n\n");

    fprintf(pl,
        "  <elementum nomen=\"pasta\">\n"
        "    <attributum nomen=\"macro\" genus=\"textus\"/>\n"
        "    <attributum nomen=\"def-f\" genus=\"numerus\"/>\n"
        "    <attributum nomen=\"def-l\" genus=\"numerus\"/>\n"
        "    <attributum nomen=\"def-c\" genus=\"numerus\"/>\n"
        "    <liberum nomen=\"extentum\" maximum=\"1\"/>\n");
    _lexemata_liberos(pl, "    ");
    fprintf(pl, "  </elementum>\n\n");

    fprintf(pl,
        "  <elementum nomen=\"stringificatio\">\n"
        "    <attributum nomen=\"macro\" genus=\"textus\"/>\n"
        "    <attributum nomen=\"def-f\" genus=\"numerus\"/>\n"
        "    <attributum nomen=\"def-l\" genus=\"numerus\"/>\n"
        "    <attributum nomen=\"def-c\" genus=\"numerus\"/>\n"
        "    <liberum nomen=\"extentum\" maximum=\"1\"/>\n");
    _lexemata_liberos(pl, "    ");
    fprintf(pl, "  </elementum>\n\n");

    fprintf(pl,
        "  <elementum nomen=\"api\"\n"
        "    nota=\"lexema ab API iniectum (speculum): nullam sedem\n"
        "          in plagula habet\">\n"
        "    <attributum nomen=\"macro\" genus=\"textus\"/>\n"
        "    <attributum nomen=\"def-f\" genus=\"numerus\"/>\n"
        "    <attributum nomen=\"def-l\" genus=\"numerus\"/>\n"
        "    <attributum nomen=\"def-c\" genus=\"numerus\"/>\n"
        "    <liberum nomen=\"extentum\" maximum=\"1\"/>\n");
    _lexemata_liberos(pl, "    ");
    fprintf(pl, "  </elementum>\n\n");

    fprintf(pl,
        "  <elementum nomen=\"extentum\"\n"
        "    nota=\"octeti invocationis [nomen..')'] quos NULLUM\n"
        "          lexema arboris monstrat - portandi, non\n"
        "          derivandi\">\n");
    _lexemata_liberos(pl, "    ");
    fprintf(pl, "  </elementum>\n\n");
}

/* ==================================================
 * Elementa lexematum (XCV, ex enumeratione)
 *
 * NON ex terminalibus grammaticae: illa terminalia SOLA declarat
 * quae parsat, et trivia (SPATIA, NOVA_LINEA, commenta) numquam
 * terminalia sunt - in <ante>/<post> vehuntur. Ergo enumeratio
 * fons est, non grammatica.
 * ================================================== */

interior vacuum
_lexemata_scribere (FILE* pl)
{
    i32 i;

    fprintf(pl,
        "  <!-- =============== LEXEMATA ===============\n"
        "       Ex enumeratione SilvaLexemaGenus (silva_token.h),\n"
        "       NON ex terminalibus grammaticae: trivia terminalia\n"
        "       non sunt, sed elementa lexematum sunt.\n"
        "       -->\n\n");

    per (i = ZEPHYRUM; i < (i32)SILVA_LEX_NUMERUS_GENERUM; i++)
    {
        SilvaLexemaGenus genus = (SilvaLexemaGenus)i;

        fprintf(pl, "  <elementum nomen=\"");
        _lexema_tag(pl, genus);
        si (_textum_fert(genus))
        {
            fprintf(pl, "\" textus=\"verum\">\n");
        }
        alioquin
        {
            fprintf(pl, "\">\n");
        }

        fprintf(pl,
            "    <attributum nomen=\"f\"        genus=\"numerus\"/>\n"
            "    <attributum nomen=\"standard\" genus=\"numerus\"/>\n");
        _attributa_sedis(pl, "    ");

        si (   genus == SILVA_LEX_SPATIA
            || genus == SILVA_LEX_TABULAE)
        {
            fprintf(pl,
                "    <attributum nomen=\"n\" genus=\"numerus\"\n"
                "      nota=\"numerus octetorum - textus non fertur\"/>\n");
        }
        si (   genus == SILVA_LEX_NOVA_LINEA
            || genus == SILVA_LEX_CONTINUATIO)
        {
            fprintf(pl,
                "    <attributum nomen=\"crlf\" genus=\"electio\">\n"
                "      <optio>true</optio>\n"
                "    </attributum>\n");
        }

        si (!_trivium_est(genus))
        {
            fprintf(pl,
                "    <liberum nomen=\"ante\" maximum=\"1\"/>\n"
                "    <liberum nomen=\"post\" maximum=\"1\"/>\n"
                "    <liberum nomen=\"scissura\"/>\n"
                "    <liberum nomen=\"expansio\"       maximum=\"1\"/>\n"
                "    <liberum nomen=\"pasta\"          maximum=\"1\"/>\n"
                "    <liberum nomen=\"stringificatio\" maximum=\"1\"/>\n"
                "    <liberum nomen=\"api\"            maximum=\"1\"/>\n");
        }
        fprintf(pl, "  </elementum>\n\n");
    }
}

/* ==================================================
 * Genera et loci: pars DERIVATA
 * ================================================== */

interior SilvaGenImpletio*
_impletio_quaerere (
                  Xar* impletiones,
     constans chorda*  genus,
     constans chorda*  locus)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(impletiones); i++)
    {
        SilvaGenImpletio* imp =
            (SilvaGenImpletio*)xar_obtinere(impletiones, i);

        si (   imp        == NIHIL || imp->genus == NIHIL
            || imp->locus == NIHIL)
        {
            perge;
        }
        si (   chorda_aequalis(*imp->genus, *genus)
            && chorda_aequalis(*imp->locus, *locus))
        {
            redde imp;
        }
    }
    redde NIHIL;
}

interior b32
_species_lista (s32 species)
{
    redde (b32)(   species == (s32)SILVA_LOCUS_LISTA_NODUS
                || species == (s32)SILVA_LOCUS_LISTA_TOKEN
                || species == (s32)SILVA_LOCUS_LISTA_MIXTA);
}

interior b32
_species_nodum_fert (s32 species)
{
    redde (b32)(   species == (s32)SILVA_LOCUS_NODUS
                || species == (s32)SILVA_LOCUS_LISTA_NODUS
                || species == (s32)SILVA_LOCUS_LISTA_MIXTA);
}

/* Vocabularium loci generis EXTRA: nulla derivatio possibilis
 * (slots= chorda cruda est, spec par. 4.2), ergo POLITIA - quam
 * porta corporis falsificat. */
interior vacuum
_extra_vocabularium (
                   FILE* pl,
       constans character* genus,
       constans character* locus,
                     Xar*  genera,
                     Xar*  genera_radicis)
{
    i32 i;

    si (strcmp(genus, "ambiguus") == ZEPHYRUM
        && strcmp(locus, "interpretationes") == ZEPHYRUM)
    {
        /* Fabrica ambigui QUAMLIBET reductionem involvere potest */
        per (i = ZEPHYRUM; i < xar_numerus(genera); i++)
        {
            SilvaGenGenusDef* def =
                (SilvaGenGenusDef*)xar_obtinere(genera, i);

            si (def == NIHIL || def->ex_extra)
            {
                perge;
            }
            fprintf(pl, "    <liberum nomen=\"");
            _ch(pl, def->titulus);
            fprintf(pl, "\"/>\n");
        }
        fprintf(pl, "    <liberum nomen=\"ambiguus\"/>\n");
        redde;
    }
    si (strcmp(genus, "conditionalis") == ZEPHYRUM
        && strcmp(locus, "rami") == ZEPHYRUM)
    {
        fprintf(pl,
            "    <liberum nomen=\"ramus-sumptus\"/>\n"
            "    <liberum nomen=\"ramus-omissus\"/>\n");
        redde;
    }
    si (strcmp(genus, "ramus-sumptus") == ZEPHYRUM
        && strcmp(locus, "contentum") == ZEPHYRUM)
    {
        per (i = ZEPHYRUM; i < xar_numerus(genera_radicis); i++)
        {
            chorda** t = (chorda**)xar_obtinere(genera_radicis, i);

            si (t == NIHIL || *t == NIHIL)
            {
                perge;
            }
            fprintf(pl, "    <liberum nomen=\"");
            _ch(pl, *t);
            fprintf(pl, "\"/>\n");
        }
        fprintf(pl,
            "    <liberum nomen=\"ambiguus\"/>\n"
            "    <liberum nomen=\"conditionalis\"/>\n");
        redde;
    }
    /* Reliqua (error/tokens, conditionalis/finis, directiva, cruda)
     * laminae lexematum crudae sunt */
    _lexemata_liberos(pl, "    ");
}

interior vacuum
_genera_scribere (
     FILE* pl,
     Xar*  genera,
     Xar*  impletiones,
     Xar*  genera_radicis)
{
    i32 g_i;

    fprintf(pl,
        "  <!-- =============== GENERA ET LOCI ===============\n"
        "       Pars DERIVATA: vocabularium ex clausura grammaticae\n"
        "       (silva_gen_impletiones_computare). Locus 'intra='\n"
        "       generi suo adstrictus est - nomina locorum LXII per\n"
        "       CLXXVI ingressus iterantur, ergo nomen solum nihil\n"
        "       dicit.\n"
        "       -->\n\n");

    per (g_i = ZEPHYRUM; g_i < xar_numerus(genera); g_i++)
    {
        SilvaGenGenusDef* def =
            (SilvaGenGenusDef*)xar_obtinere(genera, g_i);
        i32 k;

        si (def == NIHIL)
        {
            perge;
        }

        /* --- elementum generis: liberi eius LOCI sunt --- */
        fprintf(pl, "  <elementum nomen=\"");
        _ch(pl, def->titulus);
        fprintf(pl, "\">\n");
        _attributa_sedis(pl, "    ");
        per (k = ZEPHYRUM; k < xar_numerus(def->loci); k++)
        {
            SilvaGenLocusDef* locus =
                (SilvaGenLocusDef*)xar_obtinere(def->loci, k);

            si (locus == NIHIL)
            {
                perge;
            }
            /* Locus SEMEL aut nusquam: scriptor per numerum
             * locorum currit et NIHIL praetermittit (absentia
             * canonica), ergo minimum ZERO semper */
            fprintf(pl, "    <liberum nomen=\"");
            _ch(pl, locus->titulus);
            fprintf(pl, "\" maximum=\"1\"/>\n");
        }
        fprintf(pl, "  </elementum>\n\n");

        /* --- elementa locorum, generi adstricta --- */
        per (k = ZEPHYRUM; k < xar_numerus(def->loci); k++)
        {
            SilvaGenLocusDef* locus =
                (SilvaGenLocusDef*)xar_obtinere(def->loci, k);
            SilvaGenImpletio* imp;

            si (locus == NIHIL)
            {
                perge;
            }

            si (locus->species == (s32)SILVA_LOCUS_INDEX)
            {
                fprintf(pl, "  <elementum nomen=\"");
                _ch(pl, locus->titulus);
                fprintf(pl, "\" intra=\"");
                _ch(pl, def->titulus);
                fprintf(pl, "\" textus=\"numerus\"/>\n\n");
                perge;
            }

            fprintf(pl, "  <elementum nomen=\"");
            _ch(pl, locus->titulus);
            fprintf(pl, "\" intra=\"");
            _ch(pl, def->titulus);
            fprintf(pl, "\">\n");

            imp = _impletio_quaerere(impletiones, def->titulus,
                                     locus->titulus);
            si (imp == NIHIL)
            {
                character titulus_g[128];
                character titulus_l[128];
                i32       mg;
                i32       ml;

                mg = def->titulus->mensura   < (i32)magnitudo(titulus_g) - I
                   ? def->titulus->mensura   : (i32)magnitudo(titulus_g) - I;
                ml = locus->titulus->mensura < (i32)magnitudo(titulus_l) - I
                   ? locus->titulus->mensura : (i32)magnitudo(titulus_l) - I;
                memcpy(titulus_g, def->titulus->datum, (memoriae_index)mg);
                memcpy(titulus_l, locus->titulus->datum, (memoriae_index)ml);
                titulus_g[mg] = '\0';
                titulus_l[ml] = '\0';
                _extra_vocabularium(pl, titulus_g, titulus_l,
                                    genera, genera_radicis);
            }
            alioquin
            {
                i32 n;

                per (n = ZEPHYRUM; n < xar_numerus(imp->nodi); n++)
                {
                    chorda** t = (chorda**)xar_obtinere(imp->nodi, n);

                    si (t == NIHIL || *t == NIHIL)
                    {
                        perge;
                    }
                    fprintf(pl, "    <liberum nomen=\"");
                    _ch(pl, *t);
                    si (_species_lista(locus->species))
                    {
                        fprintf(pl, "\"/>\n");
                    }
                    alioquin
                    {
                        fprintf(pl, "\" maximum=\"1\"/>\n");
                    }
                }
                per (n = ZEPHYRUM; n < xar_numerus(imp->lexemata); n++)
                {
                    chorda** t =
                        (chorda**)xar_obtinere(imp->lexemata, n);

                    si (t == NIHIL || *t == NIHIL)
                    {
                        perge;
                    }
                    /* Terminale grammaticae -> tag lexematis:
                     * NOMINA_GENERUM idem verbum fert quod
                     * 'genus=' terminalis nominat, prefixo
                     * 'SILVA_LEX_' dempto */
                    fprintf(pl, "    <liberum nomen=\"lex-");
                    {
                        i32 c_i;

                        per (c_i = ZEPHYRUM; c_i < (*t)->mensura; c_i++)
                        {
                            character c = (character)(*t)->datum[c_i];

                            si (c >= 'A' && c <= 'Z')
                            {
                                c = (character)(c - 'A' + 'a');
                            }
                            alioquin si (c == '_')
                            {
                                c = '-';
                            }
                            fputc(c, pl);
                        }
                    }
                    si (_species_lista(locus->species))
                    {
                        fprintf(pl, "\"/>\n");
                    }
                    alioquin
                    {
                        fprintf(pl, "\" maximum=\"1\"/>\n");
                    }
                }
                /* AMBIGUUS ubicumque nodus sedere potest: fabrica
                 * GLR quamlibet reductionem involvere potest */
                si (_species_nodum_fert(locus->species))
                {
                    fprintf(pl, "    <liberum nomen=\"ambiguus\"/>\n");
                }
            }
            fprintf(pl, "  </elementum>\n\n");
        }
    }
}

/* ==================================================
 * SIGILLUM PROPRIUM (T6)
 *
 * CUR NON 'registrum-sigillum': illud (silva_arbor_sigillum) SOLAS
 * tabulas generum et locorum tegit. Productiones NON tegit. Ergo
 * 'si/consequens' emendare ut genus novum sententiae admittat
 * sigillum illud INTACTUM relinquit dum exemplar contenti omne
 * proiectum tacite falsum fit (spec par. 0.2). Canon proiectus
 * sigillum illud mutuari NEQUIT: suum poscit, super suos fontes.
 *
 * QUID TEGITUR: id quod emissor VERE legit, non quod spec
 * coniecit. Spec ORTHOGRAPHIAS nominaverat - emissor eas NON legit
 * (tabula lateris arboris est; emissor eam REPETIT, et probatio
 * repetitionem custodit). Sigillum quod fontem non-lectum tegeret
 * de re innoxia clamaret, et porta quae falso clamat neglegitur.
 *
 * DIVISIO OPERIS, consulto:
 *   sigillum + porta recentiae -> DATUM mutatum (grammatica,
 *     tabulae, nomina generum) artificium stalum relinquens
 *   probatio_silva_canon       -> CODEX mutatus (emissor ab
 *     arbore divergens)
 * Neutrum alterum tegit; simul rimam claudunt.
 * ================================================== */

interior vacuum
_campum (ChordaAedificator* m, constans chorda* c)
{
    /* Separator post CAMPUM QUEMQUE (mos silva_arbor_sigillum):
     * sine eo 'ab'+'c' et 'a'+'bc' idem sigillum darent */
    si (c != NIHIL)
    {
        chorda_aedificator_appendere_chorda(m, *c);
    }
    chorda_aedificator_appendere_character(m, '\n');
}

interior vacuum
_campum_literis (ChordaAedificator* m, constans character* s)
{
    si (s != NIHIL)
    {
        chorda_aedificator_appendere_literis(m, s);
    }
    chorda_aedificator_appendere_character(m, '\n');
}

interior chorda
_sigillum_computare (
    SilvaGenGrammatica* g,
                  Xar*  genera)
{
    ChordaAedificator* m;
    ChordaAedificator* exitus;
                chorda friandum;
                chorda vacua;
                   i32 friatum;
                   i32 i;
                   i32 k;

    vacua.mensura = ZEPHYRUM;
    vacua.datum   = NIHIL;

    m = chorda_aedificator_creare(g->piscina, 8192);
    si (m == NIHIL)
    {
        redde vacua;
    }

    /* Versio RATIONIS ipsius: si recipe sigilli mutetur, sigillum
     * mutari DEBET, aliter artificia duarum aetatum idem sigillum
     * ferrent */
    _campum_literis(m, "canon-sigillum-1");

    /* I. Symbolum initiale - vocabularium radicis inde fluit */
    {
        SilvaGenSymbolum* initium = (SilvaGenSymbolum*)xar_obtinere(
            g->symbola, (i32)g->initium_index);

        _campum(m, initium != NIHIL ? initium->titulus : NIHIL);
    }

    /* II. PRODUCTIONES cum annotationibus - rima quam par. 0.2
     * nominat. Dextrum symbolorum ET loci parallelae feruntur:
     * ambo exemplar contenti mutant. */
    per (i = ZEPHYRUM; i < xar_numerus(g->productiones); i++)
    {
        SilvaGenProductio* p = (SilvaGenProductio*)xar_obtinere(
            g->productiones, i);
        SilvaGenSymbolum*  s;

        si (p == NIHIL)
        {
            perge;
        }
        s = (SilvaGenSymbolum*)xar_obtinere(g->symbola,
                                            (i32)p->sinistrum);
        _campum(m, s != NIHIL ? s->titulus : NIHIL);
        _campum(m, p->genus);
        _campum(m, p->id);
        _campum(m, p->modus);

        per (k = ZEPHYRUM; k < xar_numerus(p->dextrum); k++)
        {
            s32*                atomus;
            SilvaGenLocusMappa* mappa;

            atomus = (s32*)xar_obtinere(p->dextrum, k);
            mappa  = (SilvaGenLocusMappa*)xar_obtinere(p->loci, k);
            si (atomus != NIHIL)
            {
                s = (SilvaGenSymbolum*)xar_obtinere(g->symbola,
                                                    (i32)*atomus);
                _campum(m, s != NIHIL ? s->titulus : NIHIL);
            }
            alioquin
            {
                _campum(m, NIHIL);
            }
            si (mappa != NIHIL)
            {
                _campum(m, mappa->titulus);
                chorda_aedificator_appendere_s32(m,
                    mappa->appendere ? I : ZEPHYRUM);
            }
            chorda_aedificator_appendere_character(m, '\n');
        }
        chorda_aedificator_appendere_literis(m, "|\n");
    }

    /* III. Genera extra - 'slots' chorda CRUDA est, ergo verbatim */
    per (i = ZEPHYRUM; i < xar_numerus(g->genera_extra); i++)
    {
        SilvaGenGenusExtra* e = (SilvaGenGenusExtra*)xar_obtinere(
            g->genera_extra, i);

        si (e == NIHIL)
        {
            perge;
        }
        _campum(m, e->titulus);
        _campum(m, e->loci_descriptio);
    }

    /* IV. Registrum generum et locorum cum SPECIEBUS - quod
     * 'registrum-sigillum' quoque tegit, hic retentum ut sigillum
     * hoc illius SUPERSET sit, non alternativum */
    per (i = ZEPHYRUM; i < xar_numerus(genera); i++)
    {
        SilvaGenGenusDef* d = (SilvaGenGenusDef*)xar_obtinere(genera, i);

        si (d == NIHIL)
        {
            perge;
        }
        _campum(m, d->titulus);
        per (k = ZEPHYRUM; k < xar_numerus(d->loci); k++)
        {
            SilvaGenLocusDef* l = (SilvaGenLocusDef*)xar_obtinere(
                d->loci, k);

            si (l == NIHIL)
            {
                perge;
            }
            _campum(m, l->titulus);
            chorda_aedificator_appendere_s32(m, l->species);
            chorda_aedificator_appendere_character(m, '\n');
        }
        chorda_aedificator_appendere_literis(m, "|\n");
    }

    /* V. NOMINA GENERUM lexematum - tags 'lex-*' inde manant, ergo
     * nomen mutatum canonem stalum facit */
    per (i = ZEPHYRUM; i < (i32)SILVA_LEX_NUMERUS_GENERUM; i++)
    {
        _campum_literis(m,
            silva_lexema_genus_nomen((SilvaLexemaGenus)i));
    }

    friandum = chorda_aedificator_spectare(m);
    friatum  = friatio_fnv1a_literis(
        (constans character*)friandum.datum, friandum.mensura);
    chorda_aedificator_destruere(m);

    /* Hexadecimale VIII characterum, ante-implitum - longitudo FIXA
     * quia sigilla oculo conferuntur (eadem ratio ac arbor) */
    exitus = chorda_aedificator_creare(g->piscina, IX);
    si (exitus == NIHIL)
    {
        redde vacua;
    }
    per (i = ZEPHYRUM; i < VIII; i++)
    {
        i32 gradus  = (VIII - I - i) * IV;
        i32 nibble  = (friatum >> gradus) & (i32)0xF;

        chorda_aedificator_appendere_character(exitus,
            HEX_CIFRAE[nibble]);
    }
    redde chorda_aedificator_finire(exitus);
}


chorda
silva_gen_canonem_sigillum (
    SilvaGenGrammatica* grammatica)
{
    chorda vacua;
    Xar*   genera;

    vacua.mensura = ZEPHYRUM;
    vacua.datum   = NIHIL;

    si (grammatica == NIHIL)
    {
        redde vacua;
    }
    genera = silva_gen_registrum_computare(grammatica);
    si (genera == NIHIL)
    {
        redde vacua;
    }
    redde _sigillum_computare(grammatica, genera);
}


/* ==================================================
 * Superficies
 * ================================================== */

b32
silva_gen_canonem_scribere (
    SilvaGenGrammatica* grammatica,
    constans character* dialectus,
    constans character* via,
    constans character* via_grammaticae)
{
    FILE* pl;
    Xar*  genera;
    Xar*  impletiones;
    Xar*  genera_radicis;

    si (   grammatica == NIHIL || dialectus == NIHIL
        || via        == NIHIL || via_grammaticae == NIHIL)
    {
        redde FALSUM;
    }

    genera = silva_gen_registrum_computare(grammatica);
    si (genera == NIHIL || xar_numerus(genera) == ZEPHYRUM)
    {
        fprintf(stderr, "canon: registrum generum vacuum\n");
        redde FALSUM;
    }
    impletiones = silva_gen_impletiones_computare(grammatica);
    si (impletiones == NIHIL)
    {
        fprintf(stderr, "canon: impletiones computari non potuerunt\n");
        redde FALSUM;
    }
    genera_radicis = silva_gen_genera_radicis_computare(grammatica);
    si (genera_radicis == NIHIL || xar_numerus(genera_radicis) == ZEPHYRUM)
    {
        /* CLAMAT, non tacet: radix vacua canonem gigneret qui
         * documentum omne reiceret - et causa lateret */
        fprintf(stderr,
            "canon: vocabularium radicis vacuum (symbolum initiale"
            " nullum genus gignit)\n");
        redde FALSUM;
    }

    pl = fopen(via, "w");
    si (pl == NIHIL)
    {
        fprintf(stderr, "canon: plagula aperiri non potuit: %s\n", via);
        redde FALSUM;
    }

    fprintf(pl,
        "<!--\n"
        "  %s.canon\n"
        "  GENERATUM - NE MANU EDITES.\n"
        "\n"
        "  Fons: %s\n"
        "  Regeneratio: ./silva/generare.sh\n"
        "  Recentia: ./tools/silva_canon.sh -probare\n"
        "\n"
        "  Canon documentorum <parsura> (arbor M2 par. 3): schema\n"
        "  quo documenta arboris iudicantur. Duae partes:\n"
        "\n"
        "    DERIVATA   - genera, loci, et QUAE genera quem locum\n"
        "                 implere possint. Ex grammatica computata,\n"
        "                 ergo cum motore putrescere non potest.\n"
        "    MANU SCRIPTA - involucrum (parsura, fontes, cauda,\n"
        "                 trivia, origines): forma SCRIPTORIS est,\n"
        "                 non grammaticae. Porta corporis eam\n"
        "                 falsificat.\n"
        "\n"
        "  Cardinalitas: locus quisque SEMEL aut nusquam (minimum\n"
        "  ZERO ordinarius est - absentia loci canonica est, e.g.\n"
        "  definitio-ansi locum tertium NIHIL relinquit).\n"
        "\n"
        "  SIGILLUM infra fontes DERIVATIONIS tegit (productiones\n"
        "  cum annotationibus, genera-extra, registrum, nomina\n"
        "  generum lexematum) - NON solas tabulas, quas\n"
        "  'registrum-sigillum' documenti tegit. Distinctio\n"
        "  necessaria: productio mutata exemplar contenti mutat\n"
        "  dum tabulae intactae manent.\n"
        "-->\n",
        dialectus, _basis(via_grammaticae));

    {
        chorda sigillum = _sigillum_computare(grammatica, genera);

        si (sigillum.mensura == ZEPHYRUM)
        {
            fprintf(stderr, "canon: sigillum computari non potuit\n");
            fclose(pl);
            redde FALSUM;
        }
        fprintf(pl, "<canon dialectus=\"%s\" versio=\"1\""
            " sigillum=\"%.*s\">\n\n",
            dialectus, (int)sigillum.mensura,
            (constans character*)sigillum.datum);
    }

    _involucrum_scribere(pl, genera_radicis);
    _lexemata_scribere(pl);
    _genera_scribere(pl, genera, impletiones, genera_radicis);

    /* --- claves --- */
    fprintf(pl,
        "  <!-- =============== CLAVES ===============\n"
        "       'index' fontis clavis est; quicumque fontem numero\n"
        "       nominat eam citat. Prima usura gravis citationis\n"
        "       extra naturam.\n"
        "       -->\n\n"
        "  <unicitas nomen=\"fontium-indices\" attributum=\"index\"\n"
        "    super=\"fons\"\n"
        "    nota=\"index bis datus tabulam fontium ambiguam facit\"/>\n\n"
        "  <citatio nomen=\"lexematis-fons\" attributum=\"f\"\n"
        "    ad=\"fons/index\" super=\"*\"\n"
        "    nota=\"'f=' cuiuslibet lexematis fontem tabulae\n"
        "          nominat; super='*' quia lexemata XCV sunt\"/>\n\n"
        "  <citatio nomen=\"definitionis-fons\" attributum=\"def-f\"\n"
        "    ad=\"fons/index\" super=\"expansio pasta stringificatio api\"\n"
        "    nota=\"sedes DEFINITIONIS macri - plagula alia quam\n"
        "          invocatio\"/>\n\n"
        "  <citatio nomen=\"regionis-fons\" attributum=\"regio-fons\"\n"
        "    ad=\"fons/index\"\n"
        "    super=\"regio-directiva regio-cruda regio-finis\"/>\n\n"
        "  <citatio nomen=\"principis-fons\" attributum=\"fons-princeps\"\n"
        "    ad=\"fons/index\" super=\"parsura\"/>\n\n");

    fprintf(pl, "</canon>\n");

    si (fclose(pl) != ZEPHYRUM)
    {
        fprintf(stderr, "canon: plagula claudi non potuit: %s\n", via);
        redde FALSUM;
    }
    redde VERUM;
}
