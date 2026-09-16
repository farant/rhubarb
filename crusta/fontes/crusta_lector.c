/* crusta_lector.c - Lector crustae: functio (modus, positio) -> lexema
 *
 * Vide crusta_lector.h. Scanners per modum; quisque lexema unum ex
 * cursore reddit (_facere) et cursorem progreditur. Octeti crudi
 * semper in lexemate manent; genus ex octeto LOGICO decernitur
 * (regio backtick profunditatis d retroversa exuit, vide _effugium).
 *
 * REGULA RETROVERSORUM (cursus n retroversorum, octetus 'post'
 * sequens, profunditas d): r = n, d vicibus: post in {$ `} -> r /= 2
 * (retroversum imparem post absorbet), aliter r = (r + 1) / 2 (par
 * exuitur, impar litterale manet). Deinde r LOGICA: par >= 2 =
 * effugia retroversorum (EFFUGIUM super cursum), impar = effugium
 * octeti post (EFFUGIUM super cursum + post), ZEPHYRUM = post nudus
 * (sigillum aut backtick, cursus in lexemate eius). Continuatio et
 * effugia in gemina: regula profunditatis ZEPHYRUM (casus anguli
 * intra backtick, nominati).
 *
 * VEXILLA CONTINUATIONIS (situs): titulus_exspectatur post sigillum
 * '$' (titulus '$$$x' aliter ambiguus), assignationis_gradus post
 * titulum/subscriptum assignationis. Pulsum unum supervivunt.
 *
 * '\r' octetus ordinarius est (bash idem facit: plagula CRLF verbum
 * '\r' fert); linea nova semper '\n' unum.
 */

#include "crusta_lector.h"
#include <string.h>


/* ==================================================
 * Octeti
 * ================================================== */

interior b32
_vacans (
    s32 c)
{
    redde c == ' ' || c == '\t';
}

interior b32
_littera (
    s32 c)
{
    redde (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

interior b32
_digitus (
    s32 c)
{
    redde c >= '0' && c <= '9';
}

interior b32
_nominis (
    s32 c)
{
    redde _littera(c) || _digitus(c);
}

interior b32
_digitus_sedecim (
    s32 c)
{
    redde _digitus(c) || (c >= 'a' && c <= 'f')
        || (c >= 'A' && c <= 'F');
}

/* metacharacter crustae: verbum terminat */
interior b32
_meta (
    s32 c)
{
    redde _vacans(c) || c == '\n' || c == '|' || c == '&' || c == ';'
        || c == '(' || c == ')' || c == '<' || c == '>';
}

/* parametri speciales post '$' */
interior b32
_specialis (
    s32 c)
{
    redde c == '@' || c == '*' || c == '#' || c == '?' || c == '-'
        || c == '$' || c == '!';
}


/* ==================================================
 * Lector: regiones, octeti, lexemata
 * ================================================== */

interior constans CrustaRegio*
_regio (
    constans CrustaLector* l)
{
    i32 n = xar_numerus(l->regiones);

    redde n > ZEPHYRUM
        ? (constans CrustaRegio*)xar_obtinere(l->regiones, n
            - I) : NIHIL;
}

interior s32
_finis (
    constans CrustaLector* l)
{
    constans CrustaRegio* r = _regio(l);

    redde r != NIHIL ? r->finis : (s32)l->mensura;
}

interior s32
_initium (
    constans CrustaLector* l)
{
    constans CrustaRegio* r = _regio(l);

    redde r != NIHIL ? r->initium : ZEPHYRUM;
}

interior i32
_gravis (
    constans CrustaLector* l)
{
    constans CrustaRegio* r = _regio(l);

    redde r != NIHIL ? r->gravis : ZEPHYRUM;
}

/* octetus ad i, aut -I extra regionem */
interior s32
_octetus (
    constans CrustaLector* l,
                      s32  i)
{
    si (i < ZEPHYRUM || i >= _finis(l))
    {
        redde (s32)-I;
    }
    redde (s32)(insignatus character)l->fons[i];
}

interior b32
_sequitur (
    constans CrustaLector* l,
                      s32  i,
       constans character* litterae)
{
    s32 n = (s32)strlen(litterae);

    si (i < ZEPHYRUM || i + n > _finis(l))
    {
        redde FALSUM;
    }
    redde memcmp(l->fons + i, litterae, (size_t)n) == ZEPHYRUM;
}

interior vacuum
_progredi (
    CrustaLector* l,
             s32  ad)
{
    s32 i;

    per (i = l->situs.cursor; i < ad; i++)
    {
        si (l->fons[i] == '\n')
        {
            l->situs.linea++;
            l->situs.linea_initium = i + I;
        }
    }
    l->situs.cursor = ad;
}

/* chorda.datum i8* est sed fons constans: unio (exemplar md). */
interior chorda
_segmentum (
    constans character* fons,
                   s32  ab,
                   s32  ad)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = fons + ab;
    c.datum    = u.m;
    c.mensura  = (i32)(ad - ab);
    redde c;
}

/* lexema [cursor, ad) generis dati; cauda profunditatem fert */
interior MateriaToken*
_facere (
    CrustaLector* l,
             s32  genus,
             s32  ad)
{
    MateriaToken* t;
     CrustaCauda* cauda;
              s32 ab = l->situs.cursor;

    t = materia_token_creare(l->piscina, &l->forma, genus,
        _segmentum(l->fons, ab, ad), ab, l->situs.linea,
        (i32)(ab - l->situs.linea_initium) + I, ZEPHYRUM);
    si (t == NIHIL)
    {
        redde NIHIL;
    }
    cauda = (CrustaCauda*)materia_token_cauda(t);
    si (cauda != NIHIL)
    {
        cauda->gravis = _gravis(l);
    }
    _progredi(l, ad);
    redde t;
}

/* numerus retroversorum consecutivorum ab i */
interior s32
_cursus_retro (
    constans CrustaLector* l,
                      s32  i)
{
    s32 n = ZEPHYRUM;

    dum (_octetus(l, i + n) == '\\')
    {
        n++;
    }
    redde n;
}

/* an i initium verbi sit: initium regionis, aut octetus prior
 * metacharacter, continuationibus ('\' + linea nova cursu impari)
 * retro saltatis */
interior b32
_ad_initium_verbi (
    constans CrustaLector* l,
                      s32  i)
{
    s32 initium = _initium(l);

    dum (VERUM)
    {
        si (i <= initium)
        {
            redde VERUM;
        }
        si (   l->fons[i - I]  == '\n' && i - II >= initium
            && l->fons[i - II] == '\\')
        {
            s32 j = i - II;
            s32 n = ZEPHYRUM;

            dum (j >= initium && l->fons[j] == '\\')
            {
                n++;
                j--;
            }
            si (n % II == I)
            {
                i -= II;
                perge;
            }
        }
        redde _meta((s32)(insignatus character)l->fons[i - I]);
    }
}

/* index finis cursus octetorum 'nudorum' (nec meta nec apex nec $ `
 * \) ab i */
interior s32
_cursus_nudus (
    constans CrustaLector* l,
                      s32  i)
{
    s32 c;

    dum ((c = _octetus(l, i)) >= ZEPHYRUM)
    {
        si (   _meta(c) || c == '\'' || c == '"' || c == '$' || c == '`'
            || c == '\\')
        {
            frange;
        }
        i++;
    }
    redde i;
}

interior b32
_in_tabula (
    constans character* constans* tabula,
    constans character*           datum,
                            s32   mensura)
{
    s32 i;

    per (i = ZEPHYRUM; tabula[i] != NIHIL; i++)
    {
        si (   (s32)strlen(tabula[i])                    == mensura
            && memcmp(tabula[i], datum, (size_t)mensura) == ZEPHYRUM)
        {
            redde VERUM;
        }
    }
    redde FALSUM;
}

/* subscriptum '[...]' ab i ('[' ipso): index post ']' parem,
    aut finis */
interior s32
_subscriptum_finis (
    constans CrustaLector* l,
                      s32  i)
{
    s32 profunditas = ZEPHYRUM;
    s32 c;

    dum ((c = _octetus(l, i)) >= ZEPHYRUM)
    {
        si (c == '[')
        {
            profunditas++;
        }
        alioquin si (c == ']')
        {
            profunditas--;
            si (profunditas == ZEPHYRUM)
            {
                redde i + I;
            }
        }
        i++;
    }
    redde i;
}


/* ==================================================
 * Partes verbi
 * ================================================== */

nomen structura {
    b32 gemina;             /* intra "...": effugia $ ` " \ linea */
    b32 heredoc;            /* corpus heredoc: ut gemina sine " */
    b32 metae_litterales;   /* intra ${...}: metacharacteres litterales
                             * sunt */
    b32 uncus_claudit;      /* '}' terminat */
    s32 divisor;            /* '/' aut ':' terminat; ZEPHYRUM nullus */
} Forma;

hic_manens constans Forma FORMA_IMPERII  = { FALSUM, FALSUM,
    FALSUM,
    FALSUM, ZEPHYRUM };
hic_manens constans Forma FORMA_GEMINAE  = { VERUM,  FALSUM,
    VERUM,
    FALSUM, ZEPHYRUM };
hic_manens constans Forma FORMA_HEREDOCI = { FALSUM, VERUM,
    VERUM,
    FALSUM, ZEPHYRUM };

interior MateriaToken*
_sigillum (
         CrustaLector* l,
       constans Forma* v,
                  s32  dollar);

/* cursus retroversorum ad cursorem: vide regulam in capite */
interior MateriaToken*
_effugium (
         CrustaLector* l,
       constans Forma* v)
{
    s32 cursor  = l->situs.cursor;
    s32 n       = _cursus_retro(l, cursor);
    s32 post    = _octetus(l, cursor + n);
    i32 gravis  = _gravis(l);
    s32 r;
    i32 k;
    b32 effugiendum;

    si (post < ZEPHYRUM)
    {
        redde _facere(l, (s32)CRUSTA_LEX_LITTERALIS, cursor + n);
    }
    si (post == '\n')
    {
        /* regula profunditatis ZEPHYRUM */
        si (n % II == I)
        {
            si (n > I)
            {
                redde _facere(l, (s32)CRUSTA_LEX_EFFUGIUM, cursor + n
                    - I);
            }
            si (   !v->gemina && !v->heredoc
                && _ad_initium_verbi(l, cursor))
            {
                redde _facere(l, (s32)CRUSTA_LEX_LAMINA, cursor + II);
            }
            redde _facere(l, (s32)CRUSTA_LEX_CONTINUATIO, cursor + II);
        }
        redde _facere(l, (s32)CRUSTA_LEX_EFFUGIUM, cursor + n);
    }

    r = n;
    per (k = ZEPHYRUM; k < gravis; k++)
    {
        r = (post == '$' || post == '`') ? r / II : (r + I) / II;
    }

    effugiendum = (!v->gemina && !v->heredoc)
               || post == '$' || post == '`' || post == '\\'
               || (v->gemina && post == '"');
    si (!effugiendum)
    {
        /* retroversa litteralia; paria in gemina effugia sunt */
        redde _facere(l, r >= II ? (s32)CRUSTA_LEX_EFFUGIUM
            : (s32)CRUSTA_LEX_LITTERALIS, cursor + n);
    }
    si (r % II == I)
    {
        redde _facere(l, (s32)CRUSTA_LEX_EFFUGIUM, cursor + n + I);
    }
    si (r == ZEPHYRUM)
    {
        si (post == '$')
        {
            redde _sigillum(l, v, cursor + n);
        }
        redde _facere(l, (s32)CRUSTA_LEX_GRAVIS, cursor + n + I);
    }
    redde _facere(l, (s32)CRUSTA_LEX_EFFUGIUM, cursor + n);
}

/* '$' ad 'dollar' (lexema ab cursore incipit: cursus retroversorum
 * exutus ante eum licet) */
interior MateriaToken*
_sigillum (
         CrustaLector* l,
       constans Forma* v,
                  s32  dollar)
{
    s32 c = _octetus(l, dollar + I);

    si (c == '(' && _octetus(l, dollar + II) == '(')
    {
        redde _facere(l, (s32)CRUSTA_LEX_ARITHMETICA_PARTIS_APERTURA,
            dollar + III);
    }
    si (c == '(')
    {
        redde _facere(l, (s32)CRUSTA_LEX_SUBSTITUTIO_APERTURA, dollar
            + II);
    }
    si (c == '{')
    {
        redde _facere(l, (s32)CRUSTA_LEX_EXPANSIO_APERTURA, dollar
            + II);
    }
    si (c == '\'' && !v->gemina && !v->heredoc)
    {
        s32 i = dollar + II;
        s32 d;

        dum ((d = _octetus(l, i)) >= ZEPHYRUM)
        {
            si (d == '\\')
            {
                i += II;
                perge;
            }
            si (d == '\'')
            {
                i++;
                frange;
            }
            i++;
        }
        si (i > _finis(l))
        {
            i = _finis(l);
        }
        redde _facere(l, (s32)CRUSTA_LEX_EFFUGIA, i);
    }
    si (c == '"' && !v->gemina && !v->heredoc)
    {
        redde _facere(l, (s32)CRUSTA_LEX_VERSA_APERTURA, dollar + II);
    }
    si (_littera(c) || _digitus(c) || _specialis(c))
    {
        l->situs.titulus_exspectatur = VERUM;
        redde _facere(l, (s32)CRUSTA_LEX_PARAMETRUM_SIGILLUM, dollar
            + I);
    }
    redde _facere(l, (s32)CRUSTA_LEX_LITTERALIS, dollar + I);
}

/* titulus post sigillum: nomen, digitus unus, aut specialis unus */
interior MateriaToken*
_titulus_parametri (
    CrustaLector* l)
{
    s32 i = l->situs.cursor;
    s32 c = _octetus(l, i);

    l->situs.titulus_exspectatur = FALSUM;
    si (_littera(c))
    {
        dum (_nominis(_octetus(l, i)))
        {
            i++;
        }
    }
    alioquin
    {
        i++;
    }
    redde _facere(l, (s32)CRUSTA_LEX_PARAMETRUM_TITULUS, i);
}

/* pars una verbi ad cursorem */
interior MateriaToken*
_pars (
         CrustaLector* l,
       constans Forma* v)
{
    s32 cursor  = l->situs.cursor;
    s32 c       = _octetus(l, cursor);
    s32 i;

    si (c == '\\')
    {
        redde _effugium(l, v);
    }
    si (c == '$')
    {
        redde _sigillum(l, v, cursor);
    }
    si (c == '`')
    {
        redde _facere(l, (s32)CRUSTA_LEX_GRAVIS, cursor + I);
    }
    si (c == '\'' && !v->gemina && !v->heredoc)
    {
        i = cursor + I;
        dum (_octetus(l, i) >= ZEPHYRUM && _octetus(l, i) != '\'')
        {
            i++;
        }
        si (_octetus(l, i) == '\'')
        {
            i++;
        }
        redde _facere(l, (s32)CRUSTA_LEX_SIMPLEX, i);
    }
    si (c == '"' && !v->heredoc)
    {
        redde _facere(l, v->gemina ? (s32)CRUSTA_LEX_GEMINA_CLAUSURA
            : (s32)CRUSTA_LEX_GEMINA_APERTURA, cursor + I);
    }

    /* cursus litteralis */
    i = cursor;
    dum ((c = _octetus(l, i)) >= ZEPHYRUM)
    {
        si (c == '$' || c == '`' || c == '\\')
        {
            frange;
        }
        si (v->heredoc)
        {
            si (c == '\n')
            {
                i++;
                frange;
            }
        }
        alioquin si (v->gemina)
        {
            si (c == '"')
            {
                frange;
            }
        }
        alioquin
        {
            si (c == '\'' || c == '"')
            {
                frange;
            }
            si (!v->metae_litterales && _meta(c))
            {
                frange;
            }
            si (v->uncus_claudit && c == '}')
            {
                frange;
            }
            si (v->divisor != ZEPHYRUM && c == v->divisor)
            {
                frange;
            }
        }
        i++;
    }
    si (i == cursor)
    {
        /* totalitas: octetus specialis non tractatus */
        i = cursor + I;
    }
    redde _facere(l, (s32)CRUSTA_LEX_LITTERALIS, i);
}


/* ==================================================
 * Modi imperii (INITIUM VERBA ASSIGNATIONES TABULATUM POST_TITULUM
 * IN_VERBIS EXEMPLAR)
 * ================================================== */

nomen structura {
    b32 reservata;        /* verba reservata (tabula tota) */
    b32 finis_electionis;       /* solum 'esac' reservatum (EXEMPLAR) */
    b32 assignationes;    /* nomen= agnoscitur */
    b32 subscripta_nuda;  /* [i]= sine nomine (TABULATUM) */
    b32 linea_terminat;   /* linea nova SEPARATOR_LINEAE */
    b32 arithmetica;      /* (( in positione */
} Ratio;

hic_manens constans Ratio RATIONES[] = {
    /* reservata esac   assign  subscr  linea   arith */
    { VERUM,  FALSUM, VERUM,  FALSUM, FALSUM, VERUM  },   /* INITIUM */
    { FALSUM, FALSUM, FALSUM, FALSUM, VERUM,  FALSUM },   /* VERBA */
    { FALSUM, FALSUM, VERUM,  VERUM,  VERUM,  FALSUM },
          /* ASSIGNATIONES */
    { FALSUM, FALSUM, FALSUM, VERUM,  FALSUM, FALSUM },
          /* TABULATUM */
    { VERUM,  FALSUM, FALSUM, FALSUM, FALSUM, VERUM  },
          /* POST_TITULUM */
    { FALSUM, FALSUM, FALSUM, FALSUM, VERUM,  FALSUM },
          /* IN_VERBIS */
    { FALSUM, VERUM,  FALSUM, FALSUM, FALSUM, FALSUM }    /* EXEMPLAR */
};

/* operatores redirectionis ordine longitudinis (praefixum longius
 * primum) */
hic_manens constans character* constans REDIRECTIONES_ORDINE[] = {
    "<<<", "<<-", "<<", "<&", "<>", "<", ">>", ">&", ">|", ">", NIHIL
};

interior MateriaToken*
_commentum (
    CrustaLector* l)
{
    s32 i = l->situs.cursor;

    dum (_octetus(l, i) >= ZEPHYRUM && _octetus(l, i) != '\n')
    {
        i++;
    }
    redde _facere(l, (s32)CRUSTA_LEX_COMMENTUM, i);
}

interior MateriaToken*
_spatium (
    CrustaLector* l)
{
    s32 i = l->situs.cursor;

    dum (_vacans(_octetus(l, i)))
    {
        i++;
    }
    redde _facere(l, (s32)CRUSTA_LEX_SPATIUM, i);
}

/* continuatio assignationis: post titulum '[' subscriptum, deinde '='
 * aut '+=' */
interior MateriaToken*
_assignationis_sequela (
    CrustaLector* l)
{
    s32 cursor  = l->situs.cursor;
    s32 c       = _octetus(l, cursor);

    si (l->situs.assignationis_gradus == I && c == '[')
    {
        l->situs.assignationis_gradus = II;
        redde _facere(l, (s32)CRUSTA_LEX_SUBSCRIPTUM,
            _subscriptum_finis(l, cursor));
    }
    l->situs.assignationis_gradus = ZEPHYRUM;
    si (c == '+' && _octetus(l, cursor + I) == '=')
    {
        redde _facere(l, (s32)CRUSTA_LEX_ASSIGNATIO_OPERATOR, cursor
            + II);
    }
    si (c == '=')
    {
        redde _facere(l, (s32)CRUSTA_LEX_ASSIGNATIO_OPERATOR, cursor
            + I);
    }
    redde NIHIL;   /* forma rupta: pars ordinaria sequitur */
}

/* an ab i 'nomen [sub] =' aut '+=' sequatur; j = finis nominis */
interior b32
_assignatio_sequitur (
    constans CrustaLector* l,
                      s32  i,
                      s32* nominis_finis)
{
    s32 j = i;
    s32 k;

    si (!_littera(_octetus(l, j)))
    {
        redde FALSUM;
    }
    dum (_nominis(_octetus(l, j)))
    {
        j++;
    }
    k = j;
    si (_octetus(l, k) == '[')
    {
        k = _subscriptum_finis(l, k);
    }
    si (   _octetus(l, k) == '=' || (_octetus(l, k) == '+'
        && _octetus(l, k + I) == '='))
    {
        *nominis_finis = j;
        redde VERUM;
    }
    redde FALSUM;
}

interior MateriaToken*
_imperium (
       CrustaLector* l,
     constans Ratio* ratio)
{
    s32 cursor  = l->situs.cursor;
    s32 c       = _octetus(l, cursor);
    b32 initium_verbi;
    s32 i;

    /* vexilla continuationis */
    si (l->situs.titulus_exspectatur)
    {
        redde _titulus_parametri(l);
    }
    si (l->situs.assignationis_gradus != ZEPHYRUM)
    {
        MateriaToken* t = _assignationis_sequela(l);

        si (t != NIHIL)
        {
            redde t;
        }
    }

    /* trivia */
    si (_vacans(c))
    {
        redde _spatium(l);
    }
    si (c == '\n')
    {
        redde _facere(l, ratio->linea_terminat
            ? (s32)CRUSTA_LEX_SEPARATOR_LINEAE : (s32)CRUSTA_LEX_LINEA,
            cursor + I);
    }
    initium_verbi = _ad_initium_verbi(l, cursor);
    si (c == '#' && initium_verbi)
    {
        redde _commentum(l);
    }
    si (c == '\\')
    {
        redde _effugium(l, &FORMA_IMPERII);
    }

    /* operatores */
    si (c == '&')
    {
        si (_sequitur(l, cursor, "&&"))
        {
            redde _facere(l, (s32)CRUSTA_LEX_ET, cursor + II);
        }
        si (_sequitur(l, cursor, "&>>"))
        {
            redde _facere(l, (s32)CRUSTA_LEX_REDIRECTIO_OPERATOR,
                cursor + III);
        }
        si (_sequitur(l, cursor, "&>"))
        {
            redde _facere(l, (s32)CRUSTA_LEX_REDIRECTIO_OPERATOR,
                cursor + II);
        }
        redde _facere(l, (s32)CRUSTA_LEX_SEPARATOR, cursor + I);
    }
    si (c == '|')
    {
        si (_sequitur(l, cursor, "||"))
        {
            redde _facere(l, (s32)CRUSTA_LEX_AUT, cursor + II);
        }
        si (_sequitur(l, cursor, "|&"))
        {
            redde _facere(l, (s32)CRUSTA_LEX_PIPA_ERRORIS, cursor + II);
        }
        redde _facere(l, (s32)CRUSTA_LEX_PIPA, cursor + I);
    }
    si (c == ';')
    {
        si (_sequitur(l, cursor, ";;&"))
        {
            redde _facere(l, (s32)CRUSTA_LEX_TERMINATOR_OPTIONIS,
                cursor + III);
        }
        si (_sequitur(l, cursor, ";;") || _sequitur(l, cursor, ";&"))
        {
            redde _facere(l, (s32)CRUSTA_LEX_TERMINATOR_OPTIONIS,
                cursor + II);
        }
        redde _facere(l, (s32)CRUSTA_LEX_SEPARATOR, cursor + I);
    }
    si (c == '(')
    {
        si (ratio->arithmetica && _octetus(l, cursor + I) == '(')
        {
            redde _facere(l, (s32)CRUSTA_LEX_ARITHMETICA_APERTURA,
                cursor + II);
        }
        redde _facere(l, (s32)CRUSTA_LEX_PARENTHESIS, cursor + I);
    }
    si (c == ')')
    {
        redde _facere(l, (s32)CRUSTA_LEX_PARENTHESIS_CLAUSURA, cursor
            + I);
    }
    si (c == '<' || c == '>')
    {
        si (_octetus(l, cursor + I) == '(')
        {
            redde _facere(l, (s32)CRUSTA_LEX_PROCESSUS_APERTURA,
                cursor + II);
        }
        per (i = ZEPHYRUM; REDIRECTIONES_ORDINE[i] != NIHIL; i++)
        {
            si (_sequitur(l, cursor, REDIRECTIONES_ORDINE[i]))
            {
                redde _facere(l, (s32)CRUSTA_LEX_REDIRECTIO_OPERATOR,
                    cursor + (s32)strlen(REDIRECTIONES_ORDINE[i]));
            }
        }
    }

    si (initium_verbi)
    {
        /* fd glutinatum: digiti aut {nomen} ante < aut > */
        si (_digitus(c))
        {
            i = cursor;
            dum (_digitus(_octetus(l, i)))
            {
                i++;
            }
            si (_octetus(l, i) == '<' || _octetus(l, i) == '>')
            {
                redde _facere(l, (s32)CRUSTA_LEX_REDIRECTIO_FD, i);
            }
        }
        si (c == '{' && _littera(_octetus(l, cursor + I)))
        {
            i = cursor + I;
            dum (_nominis(_octetus(l, i)))
            {
                i++;
            }
            si (   _octetus(l, i) == '}' && (_octetus(l, i + I) == '<'
                || _octetus(l, i + I) == '>'))
            {
                redde _facere(l, (s32)CRUSTA_LEX_REDIRECTIO_FD, i + I);
            }
        }

        /* verbum reservatum: cursus nudus totum verbum */
        si (ratio->reservata || ratio->finis_electionis)
        {
            i = _cursus_nudus(l, cursor);
            si (   i > cursor && (_octetus(l, i) < ZEPHYRUM
                || _meta(_octetus(l, i))))
            {
                b32 reservatum = ratio->finis_electionis
                    ? (i - cursor == (s32)IV
                        && _sequitur(l, cursor, "esac"))
                    : _in_tabula(l->dialectus->reservata, l->fons
                        + cursor,
                        i - cursor);

                si (reservatum)
                {
                    redde _facere(l, (s32)CRUSTA_LEX_RESERVATUM, i);
                }
            }
        }

        /* assignatio */
        si (ratio->assignationes && _assignatio_sequitur(l, cursor, &i))
        {
            l->situs.assignationis_gradus = I;
            redde _facere(l, (s32)CRUSTA_LEX_ASSIGNATIO_TITULUS, i);
        }
        si (ratio->subscripta_nuda && c == '[')
        {
            i = _subscriptum_finis(l, cursor);
            si (   _octetus(l, i) == '=' || (_octetus(l, i) == '+'
                && _octetus(l, i + I) == '='))
            {
                l->situs.assignationis_gradus = II;
                redde _facere(l, (s32)CRUSTA_LEX_SUBSCRIPTUM, i);
            }
        }
    }

    /* praefixum tildae: initio verbi, aut post '=' assignationis
     * (post ':' in valore NON agnoscitur - deviatio nominata: octeti
     * idem, pars litteralis) */
    si (c == '~' && (initium_verbi || _octetus(l, cursor - I) == '='))
    {
        i = cursor + I;
        dum (   _nominis(_octetus(l, i)) || _octetus(l, i) == '-'
             || _octetus(l, i) == '+' || _octetus(l, i) == '.')
        {
            i++;
        }
        redde _facere(l, (s32)CRUSTA_LEX_DOMUS, i);
    }

    redde _pars(l, &FORMA_IMPERII);
}


/* ==================================================
 * Expansio ${ }
 * ================================================== */

hic_manens constans character* constans EXPANSIONIS_SIGNA[] = {
    ":-", ":=", ":?", ":+", "##", "%%", "//", "/#", "/%", "^^", ",,",
    ":", "#", "%", "/", "^", ",", "@", "*", NIHIL
};

interior MateriaToken*
_expansio (
    CrustaLector* l)
{
    s32 cursor  = l->situs.cursor;
    s32 c       = _octetus(l, cursor);
    b32 initio = cursor >= II && l->fons[cursor - I] == '{'
              && l->fons[cursor - II] == '$';
    s32 i;

    si (initio && (c == '#' || c == '!'))
    {
        s32 d = _octetus(l, cursor + I);

        si (d >= ZEPHYRUM && d != '}' && (_nominis(d) || _specialis(d)))
        {
            redde _facere(l, (s32)CRUSTA_LEX_EXPANSIO_PRAEFIXUM, cursor
                + I);
        }
        redde _facere(l, (s32)CRUSTA_LEX_PARAMETRUM_TITULUS, cursor
            + I);
    }
    si (_littera(c))
    {
        i = cursor;
        dum (_nominis(_octetus(l, i)))
        {
            i++;
        }
        redde _facere(l, (s32)CRUSTA_LEX_PARAMETRUM_TITULUS, i);
    }
    si (_digitus(c))
    {
        i = cursor;
        dum (_digitus(_octetus(l, i)))
        {
            i++;
        }
        redde _facere(l, (s32)CRUSTA_LEX_PARAMETRUM_TITULUS, i);
    }
    si (initio && _specialis(c))
    {
        redde _facere(l, (s32)CRUSTA_LEX_PARAMETRUM_TITULUS, cursor
            + I);
    }
    si (c == '[')
    {
        redde _facere(l, (s32)CRUSTA_LEX_SUBSCRIPTUM,
            _subscriptum_finis(l, cursor));
    }
    si (c == '}')
    {
        redde _facere(l, (s32)CRUSTA_LEX_EXPANSIO_CLAUSURA, cursor + I);
    }
    per (i = ZEPHYRUM; EXPANSIONIS_SIGNA[i] != NIHIL; i++)
    {
        si (_sequitur(l, cursor, EXPANSIONIS_SIGNA[i]))
        {
            redde _facere(l, (s32)CRUSTA_LEX_EXPANSIO_OPERATOR,
                cursor + (s32)strlen(EXPANSIONIS_SIGNA[i]));
        }
    }
    /* totalitas: quidquid usque ad } aut octetum specialem */
    i = cursor;
    dum (   (c = _octetus(l, i)) >= ZEPHYRUM && c != '}' && c != '$'
         && c != '`' && c != '\\' && c != '\'' && c != '"')
    {
        i++;
    }
    si (i == cursor)
    {
        i = cursor + I;
    }
    redde _facere(l, (s32)CRUSTA_LEX_LITTERALIS, i);
}

interior MateriaToken*
_expansionis_verbum (
    CrustaLector* l,
             s32  divisor)
{
       Forma v = FORMA_IMPERII;
         s32 c = _octetus(l, l->situs.cursor);

    si (l->situs.titulus_exspectatur)
    {
        redde _titulus_parametri(l);
    }
    si (c == '}')
    {
        redde _facere(l, (s32)CRUSTA_LEX_EXPANSIO_CLAUSURA,
            l->situs.cursor + I);
    }
    si (divisor != ZEPHYRUM && c == divisor)
    {
        redde _facere(l, (s32)CRUSTA_LEX_EXPANSIO_OPERATOR,
            l->situs.cursor + I);
    }
    v.metae_litterales  = VERUM;
    v.uncus_claudit     = VERUM;
    v.divisor           = divisor;
    redde _pars(l, &v);
}


/* ==================================================
 * Arithmetica
 * ================================================== */

hic_manens constans character* constans ARITHMETICI_SIGNA[] = {
    "<<=", ">>=", "**", "++", "--", "&&", "||", "<=", ">=", "==", "!=",
    "+=", "-=", "*=", "/=", "%=", "&=", "^=", "|=", "<<", ">>",
    "+", "-", "*", "/", "%", "<", ">", "=", "!", "~", "&", "^", "|",
    "?", ":", ",", NIHIL
};

interior MateriaToken*
_arithmetica (
    CrustaLector* l,
             b32  summa)
{
    s32 cursor  = l->situs.cursor;
    s32 c       = _octetus(l, cursor);
    s32 i;

    si (l->situs.titulus_exspectatur)
    {
        redde _titulus_parametri(l);
    }
    si (_vacans(c))
    {
        redde _spatium(l);
    }
    si (c == '\n')
    {
        redde _facere(l, (s32)CRUSTA_LEX_LINEA, cursor + I);
    }
    si (c == ')')
    {
        si (summa && _octetus(l, cursor + I) == ')')
        {
            redde _facere(l, (s32)CRUSTA_LEX_ARITHMETICA_CLAUSURA,
                cursor + II);
        }
        redde _facere(l, (s32)CRUSTA_LEX_PARENTHESIS_CLAUSURA, cursor
            + I);
    }
    si (c == '(')
    {
        redde _facere(l, (s32)CRUSTA_LEX_PARENTHESIS, cursor + I);
    }
    si (c == ';')
    {
        redde _facere(l, (s32)CRUSTA_LEX_ARITHMETICA_SEPARATOR, cursor
            + I);
    }
    si (_digitus(c))
    {
        i = cursor;
        si (   c == '0' && (_octetus(l, i + I) == 'x'
            || _octetus(l, i + I) == 'X')
            && _digitus_sedecim(_octetus(l, i + II)))
        {
            i += II;
            dum (_digitus_sedecim(_octetus(l, i)))
            {
                i++;
            }
            redde _facere(l, (s32)CRUSTA_LEX_NUMERUS, i);
        }
        dum (_digitus(_octetus(l, i)))
        {
            i++;
        }
        si (   _octetus(l, i) == '#' && (_nominis(_octetus(l, i + I))
            || _octetus(l, i + I) == '@'))
        {
            i++;
            dum (_nominis(_octetus(l, i)) || _octetus(l, i) == '@')
            {
                i++;
            }
        }
        redde _facere(l, (s32)CRUSTA_LEX_NUMERUS, i);
    }
    si (_littera(c))
    {
        i = cursor;
        dum (_nominis(_octetus(l, i)))
        {
            i++;
        }
        redde _facere(l, (s32)CRUSTA_LEX_VARIABILIS, i);
    }
    si (   c == '[' && cursor > _initium(l)
        && _nominis((s32)(insignatus character)l->fons[cursor - I]))
    {
        redde _facere(l, (s32)CRUSTA_LEX_SUBSCRIPTUM,
            _subscriptum_finis(l, cursor));
    }
    si (c == '$' || c == '\\' || c == '`' || c == '\'' || c == '"')
    {
        redde _pars(l, &FORMA_IMPERII);
    }
    per (i = ZEPHYRUM; ARITHMETICI_SIGNA[i] != NIHIL; i++)
    {
        si (_sequitur(l, cursor, ARITHMETICI_SIGNA[i]))
        {
            redde _facere(l, (s32)CRUSTA_LEX_ARITHMETICA_OPERATOR,
                cursor + (s32)strlen(ARITHMETICI_SIGNA[i]));
        }
    }
    redde _facere(l, (s32)CRUSTA_LEX_LITTERALIS, cursor + I);
}


/* ==================================================
 * Iudicium [[ ]] et regula
 * ================================================== */

hic_manens constans character* constans IUDICII_SIGNA[] = {
    "==", "=", "!=", "=~", "<", ">", NIHIL
};

interior MateriaToken*
_iudicium (
    CrustaLector* l)
{
    s32 cursor  = l->situs.cursor;
    s32 c       = _octetus(l, cursor);
    b32 initium_verbi;
    s32 i;

    si (l->situs.titulus_exspectatur)
    {
        redde _titulus_parametri(l);
    }
    si (_vacans(c))
    {
        redde _spatium(l);
    }
    si (c == '\n')
    {
        redde _facere(l, (s32)CRUSTA_LEX_LINEA, cursor + I);
    }
    initium_verbi = _ad_initium_verbi(l, cursor);
    si (c == '#' && initium_verbi)
    {
        redde _commentum(l);
    }
    si (c == '(')
    {
        redde _facere(l, (s32)CRUSTA_LEX_PARENTHESIS, cursor + I);
    }
    si (c == ')')
    {
        redde _facere(l, (s32)CRUSTA_LEX_PARENTHESIS_CLAUSURA, cursor
            + I);
    }
    si (_sequitur(l, cursor, "&&") || _sequitur(l, cursor, "||"))
    {
        redde _facere(l, (s32)CRUSTA_LEX_IUDICIUM_OPERATOR, cursor
            + II);
    }
    si ((c == '<' || c == '>') && _octetus(l, cursor + I) == '(')
    {
        redde _facere(l, (s32)CRUSTA_LEX_PROCESSUS_APERTURA, cursor
            + II);
    }
    si (initium_verbi)
    {
        i = _cursus_nudus(l, cursor);
        si (   i > cursor && (_octetus(l, i) < ZEPHYRUM
            || _meta(_octetus(l, i))))
        {
            s32 mensura = i - cursor;

            si (mensura == II && _sequitur(l, cursor, "]]"))
            {
                redde _facere(l, (s32)CRUSTA_LEX_IUDICIUM_CLAUSURA, i);
            }
            si (   (mensura == I && c == '!')
                || _in_tabula(IUDICII_SIGNA, l->fons + cursor, mensura)
                || _in_tabula(l->dialectus->iudicii_praeposita,
                    l->fons + cursor, mensura)
                || _in_tabula(l->dialectus->iudicii_binaria,
                    l->fons + cursor, mensura))
            {
                redde _facere(l, (s32)CRUSTA_LEX_IUDICIUM_OPERATOR, i);
            }
        }
        /* < et > nudi ante ( iam tractati; nudi soli operatores */
        si (   (c == '<' || c == '>')
            && (_octetus(l, cursor + I) < ZEPHYRUM
            || _meta(_octetus(l, cursor + I))))
        {
            redde _facere(l, (s32)CRUSTA_LEX_IUDICIUM_OPERATOR, cursor
                + I);
        }
    }
    redde _pars(l, &FORMA_IMPERII);
}

/* an '$' ad i expansionem incipiat: $( ${ $nomen $digitus
 * $specialis */
interior b32
_expansionem_incipit (
    constans CrustaLector* l,
                      s32  i)
{
    s32 d = _octetus(l, i + I);

    redde d == '(' || d == '{' || _littera(d) || _digitus(d)
        || _specialis(d);
}

interior MateriaToken*
_regula (
    CrustaLector* l)
{
    s32 cursor       = l->situs.cursor;
    s32 c            = _octetus(l, cursor);
    s32 profunditas  = ZEPHYRUM;
    s32 i;

    si (l->situs.titulus_exspectatur)
    {
        redde _titulus_parametri(l);
    }
    si (_vacans(c))
    {
        redde _spatium(l);
    }
    si (c == '\n')
    {
        redde _facere(l, (s32)CRUSTA_LEX_LINEA, cursor + I);
    }
    si (   _sequitur(l, cursor, "]]")
        && (_octetus(l, cursor + II) < ZEPHYRUM
        || _meta(_octetus(l, cursor + II))))
    {
        redde _facere(l, (s32)CRUSTA_LEX_IUDICIUM_CLAUSURA, cursor
            + II);
    }
    si (_sequitur(l, cursor, "&&") || _sequitur(l, cursor, "||"))
    {
        redde _facere(l, (s32)CRUSTA_LEX_IUDICIUM_OPERATOR, cursor
            + II);
    }
    si (   c == '\'' || c == '"' || c == '\\' || c == '`'
        || (c == '$' && _expansionem_incipit(l, cursor)))
    {
        redde _pars(l, &FORMA_IMPERII);
    }
    i = cursor;
    dum ((c = _octetus(l, i)) >= ZEPHYRUM)
    {
        si (c == '(')
        {
            profunditas++;
        }
        alioquin si (c == ')' && profunditas > ZEPHYRUM)
        {
            profunditas--;
        }
        alioquin si (profunditas == ZEPHYRUM)
        {
            si (   _vacans(c) || c == '\n' || _sequitur(l, i, "]]")
                || _sequitur(l, i, "&&") || _sequitur(l, i, "||"))
            {
                frange;
            }
        }
        /* '$' in fine regulae litteralis (ancora), nisi expansionem
         * incipit */
        si (   c == '\'' || c == '"' || c == '\\' || c == '`'
            || (c == '$' && _expansionem_incipit(l, i)))
        {
            frange;
        }
        i++;
    }
    si (i == cursor)
    {
        i = cursor + I;
    }
    redde _facere(l, (s32)CRUSTA_LEX_REGULA, i);
}


/* ==================================================
 * Facies publica
 * ================================================== */

b32
crusta_lector_incipere (
                CrustaLector* lector,
                     Piscina* piscina,
          constans character* fons,
                         i32  mensura,
    constans CrustaDialectus* dialectus)
{
    memset(lector, ZEPHYRUM, magnitudo(*lector));
    lector->piscina = piscina;
    lector->fons = fons;
    lector->mensura = mensura;
    lector->dialectus = dialectus;
    lector->situs.linea = (i32)I;
    lector->delimitator_ab = (s32)-I;
    lector->delimitator_ad = (s32)-I;
    lector->forma.mensura_caudae = (i32)magnitudo(CrustaCauda);
    lector->regiones = xar_creare(piscina, (i32)magnitudo(CrustaRegio));
    lector->heredoca = xar_creare(piscina,
        (i32)magnitudo(CrustaHeredocPetitio));
    redde lector->regiones != NIHIL && lector->heredoca != NIHIL;
}

MateriaToken*
crusta_lector_proximum (
     CrustaLector* lector,
      CrustaModus  modus)
{
    s32 cursor = lector->situs.cursor;

    si (cursor >= _finis(lector))
    {
        redde _facere(lector, (s32)CRUSTA_LEX_FINIS, cursor);
    }
    commutatio (modus)
    {
        casus CRUSTA_MODUS_INITIUM:
        casus CRUSTA_MODUS_VERBA:
        casus CRUSTA_MODUS_ASSIGNATIONES:
        casus CRUSTA_MODUS_TABULATUM:
        casus CRUSTA_MODUS_POST_TITULUM:
        casus CRUSTA_MODUS_IN_VERBIS:
        casus CRUSTA_MODUS_EXEMPLAR:
            redde _imperium(lector, &RATIONES[(i32)modus]);
        casus CRUSTA_MODUS_GEMINA:
            si (lector->situs.titulus_exspectatur)
            {
                redde _titulus_parametri(lector);
            }
            redde _pars(lector, &FORMA_GEMINAE);
        casus CRUSTA_MODUS_EXPANSIO:
            redde _expansio(lector);
        casus CRUSTA_MODUS_EXPANSIO_VERBUM:
            redde _expansionis_verbum(lector, ZEPHYRUM);
        casus CRUSTA_MODUS_EXPANSIO_EXEMPLAR:
            redde _expansionis_verbum(lector, (s32)'/');
        casus CRUSTA_MODUS_EXPANSIO_SECTIO:
            redde _expansionis_verbum(lector, (s32)':');
        casus CRUSTA_MODUS_ARITHMETICA_SUMMA:
            redde _arithmetica(lector, VERUM);
        casus CRUSTA_MODUS_ARITHMETICA_INTRA:
            redde _arithmetica(lector, FALSUM);
        casus CRUSTA_MODUS_IUDICIUM:
            redde _iudicium(lector);
        casus CRUSTA_MODUS_REGULA:
            redde _regula(lector);
        casus CRUSTA_MODUS_HEREDOC:
            si (lector->situs.titulus_exspectatur)
            {
                redde _titulus_parametri(lector);
            }
            redde _pars(lector, &FORMA_HEREDOCI);
        casus CRUSTA_MODUS_HEREDOC_LITTERALE:
            redde _facere(lector, (s32)CRUSTA_LEX_LITTERALIS,
                _finis(lector));
        ordinarius:
            frange;
    }
    redde _facere(lector, (s32)CRUSTA_LEX_LITTERALIS, cursor + I);
}

CrustaSitus
crusta_lector_situs (
    constans CrustaLector* lector)
{
    redde lector->situs;
}

vacuum
crusta_lector_situm_reponere (
     CrustaLector* lector,
      CrustaSitus  situs)
{
    lector->situs = situs;
}

b32
crusta_lector_regionem_aperire (
    CrustaLector* lector,
             s32  finis,
             i32  gravis)
{
    CrustaRegio* r = (CrustaRegio*)xar_addere(lector->regiones);

    si (r == NIHIL)
    {
        redde FALSUM;
    }
    r->initium  = lector->situs.cursor;
    r->finis    = finis;
    r->gravis   = gravis;
    redde VERUM;
}

vacuum
crusta_lector_regionem_claudere (
    CrustaLector* lector)
{
    constans CrustaRegio* r = _regio(lector);

    si (r == NIHIL)
    {
        redde;
    }
    si (lector->situs.cursor < r->finis)
    {
        _progredi(lector, r->finis);
    }
    xar_removere_ultimum(lector->regiones);
}

s32
crusta_lector_gravem_quaerere (
    constans CrustaLector* lector,
                      s32  ab,
                      i32  gravis)
{
    s32 i = ab;
    s32 n;
    s32 r;
    i32 k;

    dum (_octetus(lector, i) >= ZEPHYRUM)
    {
        n = _cursus_retro(lector, i);
        si (_octetus(lector, i + n) == '`')
        {
            r = n;
            per (k = ZEPHYRUM; k + I < gravis; k++)
            {
                r = r / II;
            }
            si (r == ZEPHYRUM)
            {
                redde i;
            }
        }
        i += n + I;
    }
    redde (s32)-I;
}

constans CrustaCauda*
crusta_lector_cauda (
    constans MateriaToken* token)
{
    redde (constans CrustaCauda*)materia_token_cauda(token);
}

b32
crusta_lector_heredoc_petere (
    CrustaLector* lector,
          chorda  delimitator,
             b32  citatus,
             b32  tabulae)
{
    CrustaHeredocPetitio* p =
        (CrustaHeredocPetitio*)xar_addere(lector->heredoca);

    si (p == NIHIL)
    {
        redde FALSUM;
    }
    p->delimitator  = delimitator;
    p->citatus      = citatus;
    p->tabulae      = tabulae;
    redde VERUM;
}

b32
crusta_lector_heredoca_pendent (
    constans CrustaLector* lector)
{
    redde lector->heredoca_caput < xar_numerus(lector->heredoca);
}

b32
crusta_lector_heredoc_aperire (
            CrustaLector* lector,
    CrustaHeredocPetitio* exitus)
{
    constans CrustaHeredocPetitio* p;
                              s32  finis = _finis(lector);
                              s32  ls;
                              s32  le;
                              s32  cs;
                              s32  corporis_finis = finis;

    si (!crusta_lector_heredoca_pendent(lector))
    {
        redde FALSUM;
    }
    p = (constans CrustaHeredocPetitio*)xar_obtinere(lector->heredoca,
        lector->heredoca_caput);
    lector->heredoca_caput++;
    *exitus                 = *p;
    lector->delimitator_ab  = (s32)-I;
    lector->delimitator_ad  = (s32)-I;

    ls = lector->situs.cursor;
    dum (ls < finis)
    {
        le = ls;
        dum (le < finis && lector->fons[le] != '\n')
        {
            le++;
        }
        cs = ls;
        si (p->tabulae)
        {
            dum (cs < le && lector->fons[cs] == '\t')
            {
                cs++;
            }
        }
        si (   (i32)(le - cs) == p->delimitator.mensura
            && memcmp(lector->fons + cs, p->delimitator.datum,
                (size_t)p->delimitator.mensura) == ZEPHYRUM)
        {
            corporis_finis          = ls;
            lector->delimitator_ab  = ls;
            lector->delimitator_ad  = le;
            frange;
        }
        ls = le + I;
    }
    redde crusta_lector_regionem_aperire(lector, corporis_finis,
        _gravis(lector));
}

MateriaToken*
crusta_lector_heredoc_delimitator (
    CrustaLector* lector)
{
    s32 ad;

    crusta_lector_regionem_claudere(lector);
    si (lector->delimitator_ab < ZEPHYRUM)
    {
        redde NIHIL;
    }
    ad                      = lector->delimitator_ad;
    lector->delimitator_ab  = (s32)-I;
    lector->delimitator_ad  = (s32)-I;
    redde _facere(lector, (s32)CRUSTA_LEX_HEREDOC_DELIMITATOR, ad);
}

MateriaToken*
crusta_lector_heredoc_finis (
    CrustaLector* lector)
{
    s32 cursor = lector->situs.cursor;

    si (_octetus(lector, cursor) == '\n')
    {
        redde _facere(lector, (s32)CRUSTA_LEX_SEPARATOR_LINEAE, cursor
            + I);
    }
    redde NIHIL;
}
