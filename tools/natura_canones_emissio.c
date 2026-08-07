/* tools/natura_canones_emissio.c - exemplar elementi -> litterae
 *
 * Duo emissores exemplar idem legunt:
 *   -inspicere  -> TSV, pro homine (linea una per membrum)
 *   -modulus    -> canon generatus, pro machina
 * Neuter exemplar mutat: quod natura_canones.c plicavit, hic
 * SOLUM scribitur.
 *
 * Vide project-specs/natura-canones-generati-spec.md.
 */
#include "natura_canones.h"
#include <string.h>

/* receptacula: nomen naturae cum praefixo, et praestitutum planatum.
 * Utrumque angustum CONSULTO - quod non capit RECUSATUR, non
 * truncatur, quia canon truncatus recte legitur et falsum dicit. */
#define NC_APPELLATIO_MAXIMA  CXXVIII
#define NC_VALOR_MAXIMUS      M

nomen enumeratio {
    NC_VALOR_BONUS  = I,
    NC_VALOR_QUOTA  = II,   /* '"' - STML valorem PRAECIDERET */
    NC_VALOR_MAIOR  = III,  /* receptaculum non capit */
    NC_VALOR_NULLUS = IV    /* octetus nullus - vide infra */
} NcValorStatus;

interior vacuum  _kebab_literas_scribere(FILE* f,
                                         constans character* s);
interior vacuum  _planum_scribere(FILE* f, constans chorda* t);
interior b32     _appellatio_emissa(character* ex, i32 tectum,
                                    constans character* praefixum,
                                    constans chorda* titulus);
interior NcValorStatus _valorem_planare(constans chorda* v,
                                        character* ex, i32 tectum,
                                        b32* planatum);
interior b32     _generi_congruit(constans character* genus_valoris,
                                  constans character* v,
                                  Xar* optiones);
interior b32     _litteras_alienas_habet(constans character* v);
interior vacuum  _textum_evasum_scribere(FILE* f,
                                         constans character* s);
interior b32     _appellatio_iam_emissa(Xar* membra, i32 usque,
                                        NcMembrumDiscrimen d,
                                        constans character* appellatio);
interior b32     _actio_iam_scripta(Xar* elementa, i32 usque_el,
                                    i32 usque_ac, constans chorda* a);
interior constans character* _textus_generis(
                                 constans character* genus_valoris);
interior b32     _elementum_scribere(FILE* f, NcElementum* el,
                                     i32* planata, i32* aliena,
                                     i32* omissa);
interior b32     _membrum_attributum_scribere(FILE* f,
                                              NcElementum* el,
                                              NcMembrum* m,
                                              constans character* ap,
                                              i32* planata,
                                              i32* aliena);
interior vacuum  _eventum_scribere(FILE* f, Xar* elementa);
interior vacuum  _citationes_scribere(FILE* f, Xar* elementa);

/* nomen naturae (snake) -> nomen canonis (kebab).
 * Bijectivum: genus 'nomen' naturae lineolam non fert. */
vacuum
_kebab_scribere(
    FILE*             f,
    constans chorda*  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t->mensura; i++)
    {
        character c;

        c = (character)t->datum[i];
        si (c == '_')
        {
            c = '-';
        }
        putc(c, f);
    }
}

/* idem pro literis C - praefixa ('status_') formam SNAKE in
 * exemplari servant, ergo kebab uno loco solo fit */
interior vacuum
_kebab_literas_scribere(
    FILE*                f,
    constans character*  s)
{
    i32 i;

    per (i = ZEPHYRUM; s[i] != '\0'; i++)
    {
        putc(s[i] == '_' ? '-' : s[i], f);
    }
}

/* chorda in unam lineam - valor <valor> prosa MULTILINEA esse
 * potest (normalizatio indentationem tollit, lineas servat), quae
 * TSV crudo emissa unum membrum in plures lineas frangeret.
 * Exemplar valorem VERUM tenet; sola haec species eum planat. */
interior vacuum
_planum_scribere(
    FILE*             f,
    constans chorda*  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t->mensura; i++)
    {
        character c;

        c = (character)t->datum[i];
        si (c == '\n' || c == '\r' || c == '\t')
        {
            c = ' ';
        }
        putc(c, f);
    }
}

/* exemplar in TSV - linea una per membrum:
 *   ENS  discrimen  nomen  genus  optiones  [=ordinarius]
 * Actio breviter: ENS  actio  nomen */
vacuum
_elementum_inspicere(
    FILE*         f,
    NcElementum*  el)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(el->membra); i++)
    {
        NcMembrum* m;
        i32        j;

        m = (NcMembrum*)xar_obtinere(el->membra, i);

        _kebab_scribere(f, el->ens->titulus);
        fputs(m->discrimen == NC_MEMBRUM_ATTRIBUTUM
              ? "\tattributum\t" : "\tliberum\t", f);
        si (m->praefixum)
        {
            _kebab_literas_scribere(f, m->praefixum);
        }
        _kebab_scribere(f, m->titulus);
        fprintf(f, "\t%s\t", m->genus_valoris);

        per (j = ZEPHYRUM;
             m->optiones && j < xar_numerus(m->optiones); j++)
        {
            chorda* o;

            o = *(chorda**)xar_obtinere(m->optiones, j);
            si (j > ZEPHYRUM)
            {
                putc('|', f);
            }
            _planum_scribere(f, o);
        }

        si (m->praestitutum)
        {
            fputs("\t=", f);
            _planum_scribere(f, m->praestitutum);
        }
        putc('\n', f);
    }

    per (i = ZEPHYRUM; i < xar_numerus(el->actiones); i++)
    {
        chorda* a;

        a = *(chorda**)xar_obtinere(el->actiones, i);
        _kebab_scribere(f, el->ens->titulus);
        fputs("\tactio\t", f);
        _kebab_scribere(f, a);
        putc('\n', f);
    }
}

/* ==================================================
 * Canon generatus
 * ================================================== */

/* nomen emissum in receptaculum: kebab(praefixum) + kebab(titulus).
 * Materiatur quia et scribendum et COMPARANDUM est - exemplar per
 * par (titulus, praefixum) deduplicat, canon per nomen emissum. */
interior b32
_appellatio_emissa(
    character*           ex,
    i32                  tectum,
    constans character*  praefixum,
    constans chorda*     titulus)
{
    i32 n;
    i32 i;

    n = ZEPHYRUM;
    si (praefixum)
    {
        per (i = ZEPHYRUM; praefixum[i] != '\0'; i++)
        {
            si (n + I >= tectum)
            {
                redde FALSUM;
            }
            ex[n++] = (praefixum[i] == '_') ? '-' : praefixum[i];
        }
    }
    per (i = ZEPHYRUM; i < titulus->mensura; i++)
    {
        character c;

        si (n + I >= tectum)
        {
            redde FALSUM;
        }
        c      = (character)titulus->datum[i];
        ex[n++] = (c == '_') ? '-' : c;
    }
    ex[n] = '\0';
    redde VERUM;
}

/* praestitutum in valorem attributi vertere.
 *
 * STML valores attributorum VERBATIM legit (lib/stml.c
 * _tok_legere_valor_attributi): nec spatium album normalizat nec
 * entitates solvit. Inde duo consilia:
 *
 *   QUOTA ('"') RECUSATUR - valorem in medio praecideret et
 *   reliquum in attributa nova retokenizaret. Corruptio certa;
 *   '&quot;' remedium non est, quia lector noster entitates in
 *   attributis non solvit et sex litteras redderet.
 *
 *   SPATIUM ALBUM PLANATUR - linea nova per STML superesset, sed
 *   XML 1.0 par. 3.3.3 normalizationem valorum poscit, unde lector
 *   conformis spatium legeret ubi noster lineam. Planamus ergo
 *   ANTE scriptionem, ut ambo idem legant. Praestitutum numquam
 *   reicit (canon id DICIT, non applicat), ergo damnum semanticum
 *   non est - sed numeratur, ne tacitum sit.
 */
interior NcValorStatus
_valorem_planare(
    constans chorda*  v,
    character*        ex,
    i32               tectum,
    b32*              planatum)
{
    i32 n;
    i32 i;
    i32 initium;
    i32 finis;

    *planatum = FALSUM;
    n         = ZEPHYRUM;

    per (i = ZEPHYRUM; i < v->mensura; i++)
    {
        character c;

        c = (character)v->datum[i];
        si (c == '"')
        {
            redde NC_VALOR_QUOTA;
        }
        /* chorda octetus nullos ferre POTEST (mensuram fert, non
         * terminatorem); receptaculum autem litterae C est, et
         * fprintf et strcmp ibi desinunt. Valor ergo TACITE
         * praecideretur - et comparatio generis prooemium solum
         * iudicaret. Unica lenitas quam speculum contra
         * valor_congruit habebat (quod chorda_aequalis per
         * mensuram comparat); clauditur ut RECUSATIO, quia
         * praecisio tacita vitium gravius est quam recusatio. */
        si (c == '\0')
        {
            redde NC_VALOR_NULLUS;
        }
        si (c == '\n' || c == '\r' || c == '\t')
        {
            c         = ' ';
            *planatum = VERUM;
        }
        si (n + I >= tectum)
        {
            redde NC_VALOR_MAIOR;
        }
        ex[n++] = c;
    }
    ex[n] = '\0';

    /* margines tondere: spatium ad initium aut finem valoris nihil
     * significat, et tonsum ambo lectores idem legunt */
    initium = ZEPHYRUM;
    finis   = n;
    dum (initium < finis && ex[initium] == ' ')
    {
        initium++;
    }
    dum (finis > initium && ex[finis - I] == ' ')
    {
        finis--;
    }
    si (initium > ZEPHYRUM || finis < n)
    {
        *planatum = VERUM;
        memmove(ex, ex + initium, (memoriae_index)(finis - initium));
    }
    ex[finis - initium] = '\0';

    redde NC_VALOR_BONUS;
}

/* an valor generi attributi congruat - SPECULUM lib/canon.c
 * valor_congruit. Canon ordinarius= generi repugnantem NON tolerat:
 * canon_legere NIHIL reddit ('ordinarius generi attributi non
 * congruit'), id est canon TOTUS onerari nequit. Ergo hic
 * deprehendendum est, dum locum nominare possumus. */
interior b32
_generi_congruit(
    constans character*  genus_valoris,
    constans character*  v,
    Xar*                 optiones)
{
    i32 i;

    si (strcmp(genus_valoris, "textus") == ZEPHYRUM)
    {
        redde VERUM;
    }

    si (strcmp(genus_valoris, "electio") == ZEPHYRUM)
    {
        per (i = ZEPHYRUM; optiones && i < xar_numerus(optiones); i++)
        {
            chorda*       o;
            character     planum[NC_VALOR_MAXIMUS];
            b32           mutatum;

            o = *(chorda**)xar_obtinere(optiones, i);
            si (_valorem_planare(o, planum, (i32)magnitudo(planum),
                                 &mutatum) != NC_VALOR_BONUS)
            {
                perge;
            }
            si (strcmp(planum, v) == ZEPHYRUM)
            {
                redde VERUM;
            }
        }
        redde FALSUM;
    }

    si (v[ZEPHYRUM] == '\0')
    {
        redde FALSUM;
    }

    si (strcmp(genus_valoris, "veritas") == ZEPHYRUM)
    {
        redde (b32)(strcmp(v, "verum")  == ZEPHYRUM ||
                    strcmp(v, "falsum") == ZEPHYRUM);
    }

    si (strcmp(genus_valoris, "numerus") == ZEPHYRUM)
    {
        per (i = ZEPHYRUM; v[i] != '\0'; i++)
        {
            si (i == ZEPHYRUM && (v[i] == '-' || v[i] == '+'))
            {
                perge;
            }
            si (v[i] < '0' || v[i] > '9')
            {
                redde FALSUM;
            }
        }
        redde VERUM;
    }

    si (strcmp(genus_valoris, "dies") == ZEPHYRUM)
    {
        i32 mensura;

        mensura = (i32)strlen(v);
        si (mensura != IV && mensura != VII && mensura != X)
        {
            redde FALSUM;
        }
        per (i = ZEPHYRUM; i < mensura; i++)
        {
            si (i == IV || i == VII)
            {
                si (v[i] != '-')
                {
                    redde FALSUM;
                }
            }
            alioquin si (v[i] < '0' || v[i] > '9')
            {
                redde FALSUM;
            }
        }
        si (mensura >= VII)
        {
            i32 m;

            m = (i32)(v[V] - '0') * X + (i32)(v[VI] - '0');
            si (m < I || m > XII)
            {
                redde FALSUM;
            }
        }
        si (mensura == X)
        {
            i32 d;

            d = (i32)(v[VIII] - '0') * X + (i32)(v[IX] - '0');
            si (d < I || d > XXXI)
            {
                redde FALSUM;
            }
        }
        redde VERUM;
    }

    /* nomen et compositum - lineola SOLI composito licet
     * (lib/canon.c valor_congruit, casus CANON_GENUS_NOMEN:
     * 'c == '-' && a->genus == CANON_GENUS_COMPOSITUM').
     *
     * Compositum hic olim non generabatur et ramus eum tacite ut
     * nomen iudicabat; monolithus id generat (relationes identitates
     * citant, quae lineolam ferunt), ergo speculum id nunc ferre
     * DEBET - aliter ordinarius licitus falso reiceretur. */
    per (i = ZEPHYRUM; v[i] != '\0'; i++)
    {
        si (!((v[i] >= 'a' && v[i] <= 'z') ||
              (v[i] >= 'A' && v[i] <= 'Z') ||
              (v[i] >= '0' && v[i] <= '9') ||
              v[i] == '_' || v[i] == '*' ||
              (v[i] == '-' &&
               strcmp(genus_valoris, "compositum") == ZEPHYRUM)))
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* '<' et '&' in valore attributi: lector noster eos VERBATIM legit
 * (rectum), lector XML conformis eos vetat. Emittimus ut sunt -
 * evasio ea corrumperet quod nostro lectori rectum est - sed
 * numeramus, ne tacita sit forma quam instrumentum alienum
 * aliter legeret. */
interior b32
_litteras_alienas_habet(
    constans character*  v)
{
    i32 i;

    per (i = ZEPHYRUM; v[i] != '\0'; i++)
    {
        si (v[i] == '<' || v[i] == '&')
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* textus ELEMENTI (optio) - hic evasio recta est, quia STML
 * entitates in contentu textuali SOLVIT (aliter quam in
 * attributis) */
interior vacuum
_textum_evasum_scribere(
    FILE*                f,
    constans character*  s)
{
    i32 i;

    per (i = ZEPHYRUM; s[i] != '\0'; i++)
    {
        si (s[i] == '<')
        {
            fputs("&lt;", f);
        }
        alioquin si (s[i] == '>')
        {
            fputs("&gt;", f);
        }
        alioquin si (s[i] == '&')
        {
            fputs("&amp;", f);
        }
        alioquin
        {
            putc(s[i], f);
        }
    }
}

/* an membrum PRIUS in eodem discrimine idem nomen EMISSUM ferat.
 *
 * Exemplar per par (titulus, praefixum) deduplicat; nomen emissum
 * ALIUD est. Machina 'X' praefixum 'status_' sumit et proprietas
 * 'status_X' nullum: paria distincta, nomen unum - et duo
 * <attributum nomen="status-X"> in uno elemento unicitatem canonis
 * frangunt. Hodie in corpore nusquam fit; porta tamen adest et
 * CLAMAT, quia collapsus tacitus vitium domus est. */
interior b32
_appellatio_iam_emissa(
    Xar*                 membra,
    i32                  usque,
    NcMembrumDiscrimen   d,
    constans character*  appellatio)
{
    character alia[NC_APPELLATIO_MAXIMA];
    i32       i;

    per (i = ZEPHYRUM; i < usque; i++)
    {
        NcMembrum* m;

        m = (NcMembrum*)xar_obtinere(membra, i);
        si (m->discrimen != d)
        {
            perge;
        }
        si (!_appellatio_emissa(alia, (i32)magnitudo(alia),
                                m->praefixum, m->titulus))
        {
            perge;
        }
        si (strcmp(alia, appellatio) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* actiones per elementa dispersae sunt; electio <eventum actio=>
 * eas UNIT. Deduplicatio hic sine piscina fit (copia parva) -
 * scriptor per omnia priora currit. */
interior b32
_actio_iam_scripta(
    Xar*              elementa,
    i32               usque_el,
    i32               usque_ac,
    constans chorda*  a)
{
    i32 i;

    per (i = ZEPHYRUM; i <= usque_el; i++)
    {
        NcElementum* el;
        i32          j;
        i32          tectum;

        el     = *(NcElementum**)xar_obtinere(elementa, i);
        tectum = (i == usque_el) ? usque_ac : xar_numerus(el->actiones);
        per (j = ZEPHYRUM; j < tectum; j++)
        {
            chorda* alia;

            alia = *(chorda**)xar_obtinere(el->actiones, j);
            si (chorda_aequalis(*alia, *a))
            {
                redde VERUM;
            }
        }
    }
    redde FALSUM;
}

/* genus valoris -> attributum textus= elementi (proprietas
 * multiplex: valor in CONTENTU stat, non in attributo).
 * 'electio' ad 'verum' cadit: canon.canon id consulto omittit
 * ('mechanismus optionum pro textu nondum existit'). */
interior constans character*
_textus_generis(
    constans character*  genus_valoris)
{
    si (strcmp(genus_valoris, "numerus") == ZEPHYRUM) redde "numerus";
    si (strcmp(genus_valoris, "veritas") == ZEPHYRUM) redde "veritas";
    si (strcmp(genus_valoris, "dies")    == ZEPHYRUM) redde "dies";
    si (strcmp(genus_valoris, "nomen")   == ZEPHYRUM) redde "nomen";
    redde "verum";
}

/* unum <attributum> cum optionibus et ordinario. Redde FALSUM si
 * praestitutum emitti NEQUIT - tunc canon totus recusandus est. */
interior b32
_membrum_attributum_scribere(
    FILE*                f,
    NcElementum*         el,
    NcMembrum*           m,
    constans character*  ap,
    i32*                 planata,
    i32*                 aliena)
{
    constans character* genus_valoris;
    character           valor[NC_VALOR_MAXIMUS];
    b32                 habet_valorem;
    b32                 planatum;
    i32                 i;

    genus_valoris = m->genus_valoris;

    /* electio SINE optionibus omnem valorem reiceret - attributum
     * quod scribi non potest porta non est sed laqueus. Ad textum
     * cadit (par. 3.4: canon limitat, non poscit), numquam tacite. */
    si (strcmp(genus_valoris, "electio") == ZEPHYRUM &&
        (!m->optiones || xar_numerus(m->optiones) == ZEPHYRUM))
    {
        fprintf(stderr,
            "natura_canones: <%.*s> '%s' electio SINE optionibus - "
            "textus adhibetur\n",
            (integer)el->ens->titulus->mensura,
            (constans character*)el->ens->titulus->datum, ap);
        genus_valoris = "textus";
    }

    habet_valorem = FALSUM;
    si (m->praestitutum)
    {
        NcValorStatus status;

        status = _valorem_planare(m->praestitutum, valor,
                                  (i32)magnitudo(valor), &planatum);
        si (status == NC_VALOR_QUOTA)
        {
            fprintf(stderr,
                "natura_canones: <%.*s> '%s' ordinarius QUOTAM fert - "
                "STML valorem praecideret, canon RECUSATUR\n",
                (integer)el->ens->titulus->mensura,
                (constans character*)el->ens->titulus->datum, ap);
            redde FALSUM;
        }
        si (status == NC_VALOR_MAIOR)
        {
            fprintf(stderr,
                "natura_canones: <%.*s> '%s' ordinarius maior quam "
                "%u - canon RECUSATUR\n",
                (integer)el->ens->titulus->mensura,
                (constans character*)el->ens->titulus->datum, ap,
                (i32)NC_VALOR_MAXIMUS);
            redde FALSUM;
        }
        si (status == NC_VALOR_NULLUS)
        {
            fprintf(stderr,
                "natura_canones: <%.*s> '%s' ordinarius octetum "
                "nullum fert - valor tacite praecideretur, canon "
                "RECUSATUR\n",
                (integer)el->ens->titulus->mensura,
                (constans character*)el->ens->titulus->datum, ap);
            redde FALSUM;
        }
        si (!_generi_congruit(genus_valoris, valor, m->optiones))
        {
            fprintf(stderr,
                "natura_canones: <%.*s> '%s' ordinarius '%s' generi "
                "'%s' NON congruit - canon se ipsum onerare non "
                "posset, RECUSATUR\n",
                (integer)el->ens->titulus->mensura,
                (constans character*)el->ens->titulus->datum, ap,
                valor, genus_valoris);
            redde FALSUM;
        }
        si (planatum)
        {
            (*planata)++;
        }
        si (_litteras_alienas_habet(valor))
        {
            (*aliena)++;
        }
        habet_valorem = VERUM;
    }

    fprintf(f, "    <attributum nomen=\"%s\" genus=\"%s\"", ap,
            genus_valoris);
    si (habet_valorem)
    {
        fprintf(f, " ordinarius=\"%s\"", valor);
    }

    si (strcmp(genus_valoris, "electio") != ZEPHYRUM)
    {
        fputs("/>\n", f);
        redde VERUM;
    }

    fputs(">\n", f);
    per (i = ZEPHYRUM; i < xar_numerus(m->optiones); i++)
    {
        chorda*   o;
        character planum[NC_VALOR_MAXIMUS];
        b32       mutatum;

        o = *(chorda**)xar_obtinere(m->optiones, i);
        si (_valorem_planare(o, planum, (i32)magnitudo(planum),
                             &mutatum) != NC_VALOR_BONUS)
        {
            fprintf(stderr,
                "natura_canones: <%.*s> '%s' optio emitti nequit - "
                "canon RECUSATUR\n",
                (integer)el->ens->titulus->mensura,
                (constans character*)el->ens->titulus->datum, ap);
            redde FALSUM;
        }
        si (mutatum)
        {
            (*planata)++;
        }
        fputs("      <optio>", f);
        _textum_evasum_scribere(f, planum);
        fputs("</optio>\n", f);
    }
    fputs("    </attributum>\n", f);
    redde VERUM;
}

/* unum genus (aut rem) cum definitionibus liberorum suorum.
 *
 * SCOPUS (par. 4.2): partes et membra multiplicia intra="<genus>"
 * declarantur, ne cum generibus homonymis pugnent - corpus
 * 'directorium', 'eventum', 'proiectio' et alia utroque modo
 * nominat. Eadem ratione historia intra genus, eventum intra
 * historiam: acta.genera partem 'eventum' habet, et sine scopo
 * definitiones duae idem nomen peterent. */
interior b32
_elementum_scribere(
    FILE*         f,
    NcElementum*  el,
    i32*          planata,
    i32*          aliena,
    i32*          omissa)
{
    character ap[NC_APPELLATIO_MAXIMA];
    b32       sanum;
    i32       i;

    sanum = VERUM;

    fputs("\n  <elementum nomen=\"", f);
    _kebab_scribere(f, el->ens->titulus);
    fputs("\">\n", f);

    /* Identitas: omne genus nomen suum ferre potest (par. 3.2).
     *
     * COMPOSITUM, non nomen (decretum Franis, bis emendatum): id
     * entis datum CONSUMENTIS est, non clavis naturae, ergo regula
     * par. 4.4 (nomina kebab, valores naturae snake) hic non regit -
     * auctor id eodem more scribat quo elementum quod implet.
     * Compositum superset strictum nominis est (nomen + lineola),
     * unde laxatio nihil constat et nihil prius licitum reicit;
     * spatium tamen adhuc reicitur, ergo identitas manet. */
    fputs("    <attributum nomen=\"nomen\" genus=\"compositum\"/>\n", f);

    per (i = ZEPHYRUM; i < xar_numerus(el->membra); i++)
    {
        NcMembrum* m;

        m = (NcMembrum*)xar_obtinere(el->membra, i);
        si (m->discrimen != NC_MEMBRUM_ATTRIBUTUM)
        {
            perge;
        }
        si (!_appellatio_emissa(ap, (i32)magnitudo(ap), m->praefixum,
                                m->titulus))
        {
            fprintf(stderr,
                "natura_canones: <%.*s> nomen attributi longius quam "
                "%u - canon RECUSATUR\n",
                (integer)el->ens->titulus->mensura,
                (constans character*)el->ens->titulus->datum,
                (i32)NC_APPELLATIO_MAXIMA);
            sanum = FALSUM;
            perge;
        }
        si (strcmp(ap, "nomen") == ZEPHYRUM ||
            _appellatio_iam_emissa(el->membra, i,
                                   NC_MEMBRUM_ATTRIBUTUM, ap))
        {
            fprintf(stderr,
                "natura_canones: <%.*s> attributum '%s' BIS emissum "
                "(paria exemplaris distincta, nomen unum) - alterum "
                "omittitur\n",
                (integer)el->ens->titulus->mensura,
                (constans character*)el->ens->titulus->datum, ap);
            perge;
        }
        si (!_membrum_attributum_scribere(f, el, m, ap, planata,
                                          aliena))
        {
            sanum = FALSUM;
        }
    }

    per (i = ZEPHYRUM; i < xar_numerus(el->membra); i++)
    {
        NcMembrum* m;

        m = (NcMembrum*)xar_obtinere(el->membra, i);
        si (m->discrimen != NC_MEMBRUM_LIBERUM)
        {
            perge;
        }
        si (!_appellatio_emissa(ap, (i32)magnitudo(ap), m->praefixum,
                                m->titulus))
        {
            fprintf(stderr,
                "natura_canones: <%.*s> nomen liberi longius quam "
                "%u - canon RECUSATUR\n",
                (integer)el->ens->titulus->mensura,
                (constans character*)el->ens->titulus->datum,
                (i32)NC_APPELLATIO_MAXIMA);
            sanum = FALSUM;
            perge;
        }
        si (strcmp(ap, "historia") == ZEPHYRUM ||
            _appellatio_iam_emissa(el->membra, i, NC_MEMBRUM_LIBERUM,
                                   ap))
        {
            fprintf(stderr,
                "natura_canones: <%.*s> liberum '%s' BIS emissum - "
                "alterum omittitur\n",
                (integer)el->ens->titulus->mensura,
                (constans character*)el->ens->titulus->datum, ap);
            perge;
        }

        /* minimum= NUSQUAM (par. 3.4): necessitas ontologica
         * obligatio documenti non est. maximum='1' partis limes
         * est, non postulatio - planta radices duas non habet. */
        si (m->origo == NC_ORIGO_PARS)
        {
            fprintf(f, "    <liberum nomen=\"%s\" maximum=\"1\"/>\n",
                    ap);
        }
        alioquin
        {
            fprintf(f, "    <liberum nomen=\"%s\"/>\n", ap);
        }

        si (m->praestitutum)
        {
            (*omissa)++;
        }
    }

    fputs("    <liberum nomen=\"historia\" maximum=\"1\"/>\n", f);
    fputs("  </elementum>\n", f);

    /* definitiones liberorum, ad hoc genus ADSTRICTAE */
    per (i = ZEPHYRUM; i < xar_numerus(el->membra); i++)
    {
        NcMembrum* m;

        m = (NcMembrum*)xar_obtinere(el->membra, i);
        si (m->discrimen != NC_MEMBRUM_LIBERUM)
        {
            perge;
        }
        si (!_appellatio_emissa(ap, (i32)magnitudo(ap), m->praefixum,
                                m->titulus) ||
            strcmp(ap, "historia") == ZEPHYRUM ||
            _appellatio_iam_emissa(el->membra, i, NC_MEMBRUM_LIBERUM,
                                   ap))
        {
            perge;
        }

        fprintf(f, "  <elementum nomen=\"%s\" intra=\"", ap);
        _kebab_scribere(f, el->ens->titulus);

        si (m->origo == NC_ORIGO_PARS)
        {
            fputs("\">\n", f);
            fputs("    <attributum nomen=\"nota\" genus=\"textus\"/>\n",
                  f);
            fputs("  </elementum>\n", f);
        }
        alioquin si (m->origo == NC_ORIGO_RELATIO)
        {
            /* par. 3.5: relatio trans plagulas oneratoris est;
             * canon nomen bene formatum SOLUM iudicat.
             *
             * Genus ex EXEMPLARI sumitur, non hic fixum: canon per
             * modulum 'nomen' dicit (valor clavis naturae est,
             * snake), monolithus 'compositum' (valor identitatem
             * documenti nominat, quae lineolam fert). Fixum hic
             * relictum monolithum sibi repugnantem faceret -
             * relatio simplex lineolam ferret, multiplex non. */
            fprintf(f, "\">\n    <attributum nomen=\"ad\" "
                       "genus=\"%s\"/>\n  </elementum>\n",
                    m->genus_valoris);
        }
        alioquin
        {
            /* proprietas multiplex: valor in CONTENTU stat */
            fprintf(f, "\" textus=\"%s\"/>\n",
                    _textus_generis(m->genus_valoris));
        }
    }

    fputs("  <elementum nomen=\"historia\" intra=\"", f);
    _kebab_scribere(f, el->ens->titulus);
    fputs("\">\n", f);
    fputs("    <liberum nomen=\"eventum\"/>\n", f);
    fputs("  </elementum>\n", f);

    redde sanum;
}

/* Elementum 'eventum' SEMEL, intra historiam.
 *
 * DEVIATIO A MANDATO, mensurata: mandatum electionem 'super
 * actionibus HUIUS elementi' poposcit. Fieri nequit - canon intra=
 * ad PARENTEM PROXIMUM solum ligat (lib/canon.c elementum_quaerere
 * 'n->parens->titulus'), et parens eventi semper 'historia' est,
 * quae per genera omnia communis est. Electio ergo actiones
 * MODULI unit: laxior quam per genus, sed vere limitans (mendum
 * scripturae capit) et numquam falso reiciens intra modulum.
 * Actio nulla in modulo => genus 'nomen', ne electio vacua omnem
 * valorem reiciat. */
interior vacuum
_eventum_scribere(
    FILE*  f,
    Xar*   elementa)
{
    i32 numerus;
    i32 i;

    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
    {
        NcElementum* el;

        el       = *(NcElementum**)xar_obtinere(elementa, i);
        numerus += xar_numerus(el->actiones);
    }

    fputs("\n  <elementum nomen=\"eventum\" intra=\"historia\" "
          "textus=\"verum\">\n", f);
    fputs("    <attributum nomen=\"quando\" genus=\"dies\"/>\n", f);

    si (numerus == ZEPHYRUM)
    {
        fputs("    <attributum nomen=\"actio\" genus=\"nomen\"/>\n", f);
    }
    alioquin
    {
        fputs("    <attributum nomen=\"actio\" genus=\"electio\">\n", f);
        per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
        {
            NcElementum* el;
            i32          j;

            el = *(NcElementum**)xar_obtinere(elementa, i);
            per (j = ZEPHYRUM; j < xar_numerus(el->actiones); j++)
            {
                chorda* a;

                a = *(chorda**)xar_obtinere(el->actiones, j);
                si (_actio_iam_scripta(elementa, i, j, a))
                {
                    perge;
                }
                /* par. 4.4: nomina KEBAB sunt, valores naturam
                 * suam servant - actio= clavis naturae est */
                fprintf(f, "      <optio>%.*s</optio>\n",
                        (integer)a->mensura,
                        (constans character*)a->datum);
            }
        }
        fputs("    </attributum>\n", f);
    }

    fputs("    <attributum nomen=\"certitudo\" genus=\"nomen\"/>\n", f);
    fputs("    <attributum nomen=\"fons\" genus=\"nomen\"/>\n", f);
    fputs("    <attributum nomen=\"nota\" genus=\"textus\"/>\n", f);
    fputs("  </elementum>\n", f);
}

/* Citationes - clavis-relatio INTRA documentum.
 *
 * Emissor hic NIHIL iudicat: membrum citatio_ad fert aut non fert.
 * Quae relatio citari possit natura_canones.c solus decernit (et
 * canon per modulum nullam ponit, unde haec functio ibi silet).
 *
 * super= ADSTRINGIT citationem ad elementum quod relationem vere
 * declarat: sine eo elementum alienum idem nomen attributi ferens
 * falso caperetur. Nomina generum per corpus unica sunt (CLXI
 * mensurata), ergo super= unum elementum exacte nominat.
 *
 * ad="<petitum>/nomen": clavis identitas est, id est attributum
 * 'nomen' generis peti - quod omne elementum fert. */
interior vacuum
_citationes_scribere(
    FILE*  f,
    Xar*   elementa)
{
    character ap[NC_APPELLATIO_MAXIMA];
    i32       numerus;
    i32       i;

    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
    {
        NcElementum* el;
        i32          j;

        el = *(NcElementum**)xar_obtinere(elementa, i);
        per (j = ZEPHYRUM; j < xar_numerus(el->membra); j++)
        {
            NcMembrum* m;

            m = (NcMembrum*)xar_obtinere(el->membra, j);
            si (!m->citatio_ad ||
                !_appellatio_emissa(ap, (i32)magnitudo(ap),
                                    m->praefixum, m->titulus))
            {
                perge;
            }

            si (numerus == ZEPHYRUM)
            {
                fputs("\n  <!-- CITATIONES: relatio quae folium "
                      "petit clavem documenti fit.\n"
                      "       Petitum cum posteris citari NEQUIT - "
                      "documentum subgenus\n"
                      "       licite nominat, et clavis generis id "
                      "non caperet. -->\n", f);
            }
            numerus++;

            fputs("  <citatio nomen=\"", f);
            _kebab_scribere(f, el->ens->titulus);
            fprintf(f, "-%s\" attributum=\"%s\" ad=\"", ap, ap);
            _kebab_scribere(f, m->citatio_ad);
            fputs("/nomen\" super=\"", f);
            _kebab_scribere(f, el->ens->titulus);
            fputs("\"/>\n", f);
        }
    }

    si (numerus > ZEPHYRUM)
    {
        fprintf(stderr, "natura_canones: citationes %u emissae\n",
                numerus);
    }
}

b32
_canonem_emittere(
    FILE*                f,
    Xar*                 elementa,
    constans character*  dialectus,
    constans character*  fons,
    constans character*  praefatio)
{
    b32 sanum;
    i32 planata;
    i32 aliena;
    i32 omissa;
    i32 i;

    sanum   = VERUM;
    planata = ZEPHYRUM;
    aliena  = ZEPHYRUM;
    omissa  = ZEPHYRUM;

    /* SIGNUM: 'GENERATUM' et 'NOLI MANU MUTARE' AMBO in linea II
     * stare debent. generata-custos.sh 'head -3' solum legit et
     * duas locutiones solas agnoscit; synonymum aut linea quarta
     * plagulam TACITE inermem relinqueret - quod iam semel momordit
     * (spec par. 3.3). Probatio ortus: editionem tempta, negari
     * vide. */
    fputs("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n", f);
    fprintf(f,
        "<!-- GENERATUM a natura_canones e %s - NOLI MANU MUTARE.\n",
        fons);
    fputs("     Regenera: ./tools/natura_canones.sh -->\n", f);

    /* praefatio POST signum: vide natura_canones.h - intra signum
     * posita locutionem 'NOLI MANU MUTARE' extra 'head -3' pelleret */
    si (praefatio)
    {
        fprintf(f, "<!--\n%s\n-->\n", praefatio);
    }

    fprintf(f, "<canon dialectus=\"%s\" versio=\"1\">\n\n", dialectus);

    /* 'individua' SOLA radix= fert */
    fputs("  <elementum nomen=\"individua\" radix=\"verum\">\n", f);
    per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
    {
        NcElementum* el;

        el = *(NcElementum**)xar_obtinere(elementa, i);
        fputs("    <liberum nomen=\"", f);
        _kebab_scribere(f, el->ens->titulus);
        fputs("\"/>\n", f);
    }
    fputs("  </elementum>\n", f);

    per (i = ZEPHYRUM; i < xar_numerus(elementa); i++)
    {
        NcElementum* el;

        el = *(NcElementum**)xar_obtinere(elementa, i);
        si (!_elementum_scribere(f, el, &planata, &aliena, &omissa))
        {
            sanum = FALSUM;
        }
    }

    _eventum_scribere(f, elementa);
    _citationes_scribere(f, elementa);
    fputs("\n</canon>\n", f);

    /* damna EXPRESSA: quod canon dicere non potest tacitum non
     * transit (mos _genera_ignota_nuntiare) */
    si (planata > ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_canones: valores %u planati (linea nova in "
            "attributo per XML superesse non potest)\n", planata);
    }
    si (aliena > ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_canones: valores %u litteras '<' aut '&' ferunt - "
            "STML eos verbatim legit, instrumentum XML alienum "
            "aliter legeret\n", aliena);
    }
    si (omissa > ZEPHYRUM)
    {
        fprintf(stderr,
            "natura_canones: praestituta %u omissa - canon "
            "ordinarius= in libero non fert, solum in attributo\n",
            omissa);
    }

    redde sanum;
}
