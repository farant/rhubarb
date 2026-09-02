/* probatio_silva_canon - canon c89 proiectus (T5)
 *
 * PORTA HUIUS GRADUS: canon generatus a canon.canon iudicatur -
 * schema schematum schema generatum iudicans. Non probatio
 * circularis: canon.canon manu scriptus est et quattuor vitia
 * scribentis claudit quae lector benignus taceret.
 *
 * DUAE AEQUALITATES ASSERTAE, non structuraliter praestitae.
 * Emissor arborem (motorem) attingere NON potest - generator
 * tabulas gignit quas motor consumit, ergo pendere ab eo
 * directionem aedificationis inverteret. Duas res ergo REPETIT:
 *
 *   I.  mangulationem tag ('lex-' + nomen minusculum)
 *   II. quae genera TEXTUM ferant (tabula ORTHOGRAPHIAE arboris)
 *
 * Repetitio tacita divergere posset. Hic per genera XCV curritur
 * et utraque cum fonte arboris comparatur, ergo divergentia
 * probationem NOMINE frangit. Hoc communicatione tacita MELIUS
 * est: fractura visibilis fit, non silens.
 *
 * NUMERI FIXI consulto: emendatio quae eos movet ANNUNTIANDA est.
 */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "internamentum.h"
#include "stml.h"
#include "canon.h"
#include "silva_arbor.h"
#include "silva_token.h"
#include "silva_generare.h"
#include "silva_canon_coquere.h"
#include "credo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_plagulam_legere (
               Piscina* piscina,
    constans character* via)
{
    FILE* pl;
    chorda fructus;
    signatus longus mensura;
    character* buffer;

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;

    pl = fopen(via, "rb");
    si (pl == NIHIL) redde fructus;
    si (fseek(pl, 0L, SEEK_END) != ZEPHYRUM)
    { fclose(pl); redde fructus;
    }
    mensura = ftell(pl);
    si (mensura < 0L)
    { fclose(pl); redde fructus;
    }
    rewind(pl);
    buffer = (character*)piscina_allocare(piscina,
        (memoriae_index)(mensura + 1L));
    si (buffer == NIHIL)
    { fclose(pl); redde fructus;
    }
    si (fread(buffer, I, (memoriae_index)mensura, pl)
        != (memoriae_index)mensura)
    {
        fclose(pl);
        redde fructus;
    }
    fclose(pl);
    buffer[mensura]  = '\0';
    fructus.datum    = (i8*)buffer;
    fructus.mensura  = (i32)mensura;
    redde fructus;
}

/* Elementum canonis per nomen (et intra=, si datum) invenire */
interior StmlNodus*
_elementum (
              StmlNodus* canon_radix,
     constans character* titulus,
     constans character* intra)
{
    i32 i;

    per (i = ZEPHYRUM; i < stml_numerus_liberorum(canon_radix); i++)
    {
        StmlNodus* l = stml_liberum_ad_indicem(canon_radix, i);
           chorda* n;
           chorda* in;

        si (   l          == NIHIL || l->genus != STML_NODUS_ELEMENTUM
            || l->titulus == NIHIL
            || !chorda_aequalis_literis(*l->titulus, "elementum"))
        {
            perge;
        }
        n = stml_attributum_capere(l, "nomen");
        si (n == NIHIL || !chorda_aequalis_literis(*n, titulus))
        {
            perge;
        }
        in = stml_attributum_capere(l, "intra");
        si (intra == NIHIL)
        {
            si (in == NIHIL) redde l;
            perge;
        }
        si (in != NIHIL && chorda_aequalis_literis(*in, intra))
        {
            redde l;
        }
    }
    redde NIHIL;
}

s32
principale (vacuum)
{
                 Piscina* piscina;
     InternamentumChorda* intern;
      constans character* radix_via;
               character  via_canonis[512];
               character  via_metacanonis[512];
                  chorda  fons_canonis;
                  chorda  fons_metacanonis;
            StmlResultus  res_canonis;
                   Canon* metacanon;
                  chorda  causa;
               StmlNodus* canon_radix;

    piscina = piscina_generare_dynamicum("canon_c89", LXIV * M * M);
    si (piscina == NIHIL)
    { imprimere("FRACTA: piscina\n"); redde I;
    }
    credo_aperire(piscina);
    intern = internamentum_creare(piscina);

    radix_via = getenv("RHUBARB_RADIX");
    si (radix_via == NIHIL) radix_via = "..";
    sprintf(via_canonis,     "%s/silva/c89.canon", radix_via);
    sprintf(via_metacanonis, "%s/canon.canon",     radix_via);

    imprimere("\n--- Probans canonem c89 proiectum ---\n");

    fons_canonis = _plagulam_legere(piscina, via_canonis);
    CREDO_NON_NIHIL (fons_canonis.datum);
    fons_metacanonis = _plagulam_legere(piscina, via_metacanonis);
    CREDO_NON_NIHIL (fons_metacanonis.datum);
    si (fons_canonis.datum == NIHIL || fons_metacanonis.datum == NIHIL)
    {
        imprimere("  (canon generatus abest: ./silva/generare.sh)\n");
        credo_imprimere_compendium();
        redde I;
    }


    /* ============================================================
     * I. CANON IPSE LEGITUR
     *
     * canon_legere iam porta est: regula quae attributum ignotum
     * nominat RECUSAT ab anno 2026-08-22 (T1). Canon proiectus
     * citationes quattuor et unicitatem fert - si quaelibet
     * attributum non declaratum nominaret, hic frangeretur.
     * ============================================================ */

    {
        Canon* canon_c89;

        causa.datum = NIHIL;
        causa.mensura = ZEPHYRUM;
        canon_c89 = canon_legere(fons_canonis, piscina, intern, &causa);
        CREDO_NON_NIHIL (canon_c89);
        si (canon_c89 == NIHIL && causa.datum != NIHIL)
        {
            imprimere("  CAUSA: %.*s\n", (integer)causa.mensura,
                (character*)causa.datum);
        }
    }


    /* ============================================================
     * II. CANON.CANON CANONEM PROIECTUM IUDICAT
     * ============================================================ */

    res_canonis = stml_legere(fons_canonis, piscina, intern);
    CREDO_VERUM (res_canonis.successus);
    si (!res_canonis.successus)
    {
        imprimere("  STML fractum: linea %d\n",
            (integer)res_canonis.linea_erroris);
        credo_imprimere_compendium();
        redde I;
    }
    canon_radix = res_canonis.elementum_radix;
    CREDO_NON_NIHIL (canon_radix);

    causa.datum = NIHIL;
    causa.mensura = ZEPHYRUM;
    metacanon = canon_legere(fons_metacanonis, piscina, intern, &causa);
    CREDO_NON_NIHIL (metacanon);
    si (metacanon != NIHIL && canon_radix != NIHIL)
    {
        Xar* vitia = canon_iudicare(metacanon, canon_radix, piscina);

        CREDO_NON_NIHIL (vitia);
        si (vitia != NIHIL)
        {
            i32 n = xar_numerus(vitia);
            i32 i;

            per (i = ZEPHYRUM; i < n && i < X; i++)
            {
                CanonVitium* v = (CanonVitium*)xar_obtinere(vitia, i);
                si (v == NIHIL) perge;

                imprimere("  VITIUM %s", canon_nuntius(v->genus));
                si (v->elementum != NIHIL)
                {
                    imprimere(": <%.*s>",
                        (integer)v->elementum->mensura,
                        (character*)v->elementum->datum);
                }
                si (v->detail != NIHIL)
                {
                    imprimere(" %.*s",
                        (integer)v->detail->mensura,
                        (character*)v->detail->datum);
                }
                imprimere("\n");
            }
            CREDO_AEQUALIS_I32 (n, ZEPHYRUM);
        }
    }


    /* ============================================================
     * III. AEQUALITAS I - MANGULATIO TAG
     *
     * Pro generibus XCV: tag quem SCRIPTOR gignit elementum in
     * canone habere DEBET. Si emissor mangulationem aliter faceret,
     * documenta scriptoris canonem numquam transirent - et causa
     * lateret. Hic clamat.
     * ============================================================ */

    {
        i32 inventa;
        i32 i;

        inventa = ZEPHYRUM;
        per (i = ZEPHYRUM; i < (i32)SILVA_LEX_NUMERUS_GENERUM; i++)
        {
            character  tag[SILVA_ARBOR_TAG_CAPACITAS];
            StmlNodus* elem;

            si (silva_arbor_lexema_tag((SilvaLexemaGenus)i, tag,
                    (i32)magnitudo(tag)) == ZEPHYRUM)
            {
                perge;
            }
            elem = _elementum(canon_radix, tag, NIHIL);
            si (elem == NIHIL)
            {
                imprimere("  TAG DEEST in canone: %s\n", tag);
                perge;
            }
            inventa++;
        }
        imprimere("  tags lexematum inventa: %d\n", (integer)inventa);
        CREDO_AEQUALIS_I32 (inventa, (i32)95);
    }


    /* ============================================================
     * IV. AEQUALITAS II - QUAE GENERA TEXTUM FERANT
     *
     * Regula arboris duabus semitis constat:
     *   silva_arbor_valor_portandus  (orthographia varia)
     *   MINUS trivia quae attributum PROPRIUM ferunt
     *     (SPATIA/TABULAE -> 'n='; NOVA_LINEA/CONTINUATIO -> 'crlf')
     *
     * Probatio regulam TOTAM hic dicit - non tabulam emissoris
     * repetit. Ergo si emissor et arbor divergant, ambo contra
     * hanc tertiam sententiam franguntur.
     * ============================================================ */

    {
        i32 textualia;
        i32 i;

        textualia = ZEPHYRUM;
        per (i = ZEPHYRUM; i < (i32)SILVA_LEX_NUMERUS_GENERUM; i++)
        {
            SilvaLexemaGenus  genus = (SilvaLexemaGenus)i;
                   character  tag[SILVA_ARBOR_TAG_CAPACITAS];
                   StmlNodus* elem;
                         b32  debet;
                         b32  habet;

            si (silva_arbor_lexema_tag(genus, tag, (i32)magnitudo(tag))
                == ZEPHYRUM)
            {
                perge;
            }
            debet = (b32)(silva_arbor_valor_portandus(genus)
                && genus != SILVA_LEX_SPATIA
                && genus != SILVA_LEX_TABULAE
                && genus != SILVA_LEX_NOVA_LINEA
                && genus != SILVA_LEX_CONTINUATIO);

            elem   = _elementum(canon_radix, tag, NIHIL);
            habet  = FALSUM;
            si (elem != NIHIL)
            {
                chorda* t = stml_attributum_capere(elem, "textus");
                habet = (b32)(t != NIHIL
                    && chorda_aequalis_literis(*t, "verum"));
            }
            si (debet != habet)
            {
                imprimere("  TEXTUS DISCREPAT: %s (arbor %d, canon %d)\n",
                    tag, (integer)debet, (integer)habet);
            }
            CREDO_VERUM (debet == habet);
            si (debet) textualia++;
        }
        imprimere("  genera textum ferentia: %d\n", (integer)textualia);
        CREDO_AEQUALIS_I32 (textualia, (i32)10);
    }


    /* ============================================================
     * V. FORMA CANONIS FIXA
     *
     * XCV lexemata + CLXXVI loci adstricti + LVIII genera +
     * XVII involucri = CCCXLVI. Numeri e mensura corporis veniunt
     * (spec par. 0.3), non e cursu primo.
     * ============================================================ */

    {
        i32 lexematum;
        i32 locorum;
        i32 liberorum;
        i32 i;

        lexematum  = ZEPHYRUM;
        locorum    = ZEPHYRUM;
        liberorum  = ZEPHYRUM;

        per (i = ZEPHYRUM; i < stml_numerus_liberorum(canon_radix); i++)
        {
            StmlNodus* l = stml_liberum_ad_indicem(canon_radix, i);
               chorda* n;

            si (   l == NIHIL || l->genus != STML_NODUS_ELEMENTUM
                || l->titulus == NIHIL
                || !chorda_aequalis_literis(*l->titulus, "elementum"))
            {
                perge;
            }
            n = stml_attributum_capere(l, "nomen");
            si (n == NIHIL) perge;

            si (stml_attributum_capere(l, "intra") != NIHIL)
            {
                locorum++;
            }
            alioquin si (   n->mensura > IV
                         && memcmp(n->datum, "lex-", IV) == ZEPHYRUM)
            {
                lexematum++;
            }
            alioquin
            {
                liberorum++;
            }
        }
        imprimere("  lexemata %d, loci adstricti %d, non-adstricta %d\n",
            (integer)lexematum, (integer)locorum, (integer)liberorum);
        CREDO_AEQUALIS_I32 (lexematum, (i32)95);
        CREDO_AEQUALIS_I32 (locorum,   (i32)176);
        /* LVIII genera + XVII involucri */
        CREDO_AEQUALIS_I32 (liberorum, (i32)75);
    }


    /* ============================================================
     * VI. COLLISIONES NOMINUM TRANS LAMINAS
     *
     * 'corpus' genus EST et locus EST; 'directiva' involucrum EST
     * et locus EST. Nomen solum dicere non potest quid sint - hoc
     * est prorsus cur loci 'intra=' ferunt. Utrumque asseritur:
     * definitio non-adstricta ADEST, et adstricta quoque.
     * ============================================================ */

    {
        CREDO_NON_NIHIL (_elementum(canon_radix, "corpus", NIHIL));
        CREDO_NON_NIHIL (_elementum(canon_radix, "corpus",
                                    "definitio-functionis"));
        CREDO_NON_NIHIL (_elementum(canon_radix, "directiva", NIHIL));
        CREDO_NON_NIHIL (_elementum(canon_radix, "directiva",
                                    "ramus-omissus"));
    }


    /* ============================================================
     * VII. CASUS QUI CONSILIUM DECREVIT
     *
     * definitio-functionis/declarator quinque genera admittit et
     * 'declarator-abstractus' NON. Species sola id dicere nequit -
     * ambo 'NODUS' sunt. ABSENTIA probatio vera est: praesentia
     * sola ex vocabulario nimis lato aeque sequeretur.
     * ============================================================ */

    {
        StmlNodus* locus;
              i32  i;
              b32  habet_titulum;
              b32  habet_abstractum;

        locus = _elementum(canon_radix, "declarator",
                           "definitio-functionis");
        CREDO_NON_NIHIL (locus);
        habet_titulum     = FALSUM;
        habet_abstractum  = FALSUM;
        si (locus != NIHIL)
        {
            per (i = ZEPHYRUM; i < stml_numerus_liberorum(locus); i++)
            {
                StmlNodus* l = stml_liberum_ad_indicem(locus, i);
                   chorda* n;

                si (   l == NIHIL || l->titulus == NIHIL
                    || !chorda_aequalis_literis(*l->titulus, "liberum"))
                {
                    perge;
                }
                n = stml_attributum_capere(l, "nomen");
                si (n == NIHIL) perge;
                si (chorda_aequalis_literis(*n, "declarator-titulus"))
                {
                    habet_titulum = VERUM;
                }
                si (chorda_aequalis_literis(*n,
                    "declarator-abstractus"))
                {
                    habet_abstractum = VERUM;
                }
            }
        }
        CREDO_VERUM  (habet_titulum);
        CREDO_FALSUM (habet_abstractum);
    }


    /* ============================================================
     * VIII. SIGILLUM PROPRIUM (T6) - SENSIBILITAS ASSERTA
     *
     * Spec par. 0.2: 'si/consequens' emendare ut genus aliud
     * admittat tabulas generum et locorum INTACTAS relinquit -
     * ergo 'registrum-sigillum' documenti immotum manet dum
     * exemplar contenti proiectum omnino mutatur. Ea est rima
     * propter quam hoc sigillum exsistit.
     *
     * Hic ea rima IPSA plantatur: grammatica in MEMORIA mutatur
     * (plagula intacta manet), et sigillum MUTARI debet. Sigillum
     * quem nemo frangi vidit sigillum est quem nemo scit operari -
     * eadem lex quae hunc gradum peperit.
     * ============================================================ */

    {
              character  via_grammaticae[512];
                 chorda  fons_grammaticae;
     SilvaGenGrammatica* g_sana;
                 chorda  sig_sana;

        sprintf(via_grammaticae, "%s/silva/grammatica/c89.stml",
                radix_via);
        fons_grammaticae = _plagulam_legere(piscina, via_grammaticae);
        CREDO_NON_NIHIL (fons_grammaticae.datum);

        si (fons_grammaticae.datum != NIHIL)
        {
            g_sana = silva_gen_grammaticam_legere(piscina, intern,
                (constans character*)fons_grammaticae.datum);
            CREDO_NON_NIHIL (g_sana);

            si (g_sana != NIHIL)
            {
                sig_sana = silva_gen_canonem_sigillum(g_sana);
                CREDO_AEQUALIS_I32 (sig_sana.mensura, (i32)8);

                /* sigillum canonis commissi cum computato congruere
                 * debet - aliter artificium rancidum est */
                {
                    chorda* attr = stml_attributum_capere(canon_radix,
                                                          "sigillum");
                    CREDO_NON_NIHIL (attr);
                    si (attr != NIHIL)
                    {
                        si (!chorda_aequalis(*attr, sig_sana))
                        {
                            imprimere("  SIGILLUM RANCIDUM: canon %.*s,"
                                " grammatica %.*s\n",
                                (integer)attr->mensura,
                                (character*)attr->datum,
                                (integer)sig_sana.mensura,
                                (character*)sig_sana.datum);
                        }
                        CREDO_VERUM (chorda_aequalis(*attr, sig_sana));
                    }
                }

                /* CULPA PLANTATA: productio mutata, tabulae immotae */
                {
                             character* copia;
                             character* sedes;
                    constans character* VETUS =
                        "sententia@consequens";
                    constans character* NOVUM =
                        "declaratio@consequens";

                    copia = (character*)piscina_allocare(piscina,
                        (memoriae_index)fons_grammaticae.mensura + I);
                    CREDO_NON_NIHIL (copia);
                    si (copia != NIHIL)
                    {
                        memcpy(copia, fons_grammaticae.datum,
                            (memoriae_index)fons_grammaticae.mensura);
                        copia[fons_grammaticae.mensura] = '\0';

                        sedes = strstr(copia, VETUS);
                        /* Si grammatica hoc verbum non iam fert,
                         * probatio TACERET - ergo id asseritur */
                        CREDO_NON_NIHIL (sedes);
                        si (sedes != NIHIL)
                        {
                            SilvaGenGrammatica* g_mutata;
                                        chorda  sig_mutata;

                            memcpy(sedes, NOVUM, strlen(NOVUM));
                            g_mutata = silva_gen_grammaticam_legere(
                                piscina, intern, copia);
                            CREDO_NON_NIHIL (g_mutata);
                            si (g_mutata != NIHIL)
                            {
                                sig_mutata =
                                    silva_gen_canonem_sigillum(g_mutata);
                                imprimere("  sigillum sanum %.*s ->"
                                    " mutatum %.*s\n",
                                    (integer)sig_sana.mensura,
                                    (character*)sig_sana.datum,
                                    (integer)sig_mutata.mensura,
                                    (character*)sig_mutata.datum);
                                CREDO_FALSUM (chorda_aequalis(sig_sana,
                                                              sig_mutata));
                            }
                        }
                    }
                }
            }
        }
    }

    credo_imprimere_compendium();
    si (credo_omnia_praeterierunt())
    {
        piscina_destruere(piscina);
        redde ZEPHYRUM;
    }
    piscina_destruere(piscina);
    redde I;
}
