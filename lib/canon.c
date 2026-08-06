/* canon.c - schema STML generale (gradus II)
 *
 * Vide include/canon.h pro doctrina et pro eo QUOD CANON NON
 * PRAESTAT consulto (citationes, hereditas, co-occurrentiae).
 */
#include "canon.h"
#include <string.h>

/* ==================================================
 * Structurae interiores
 * ================================================== */

nomen structura {
    chorda*  titulus;
    i32      minimum;
    /* SIGNATUM CONSULTO: -1 = infinitum. i32 INSIGNATUS est, ergo
     * '-1' in numerum immanem verteretur et 'maximum >= ZEPHYRUM'
     * semper verum esset - rami qui numquam currunt. Uncus examinis
     * id statim nominavit (codex: comparatio vana). */
    s32      maximum;   /* -1 = infinitum */
} CanonLiberum;

nomen structura {
    chorda*            titulus;
    CanonGenusValoris  genus;
    b32                necessarium;
    Xar*               optiones;   /* Xar de chorda* (electio) */
} CanonAttributum;

nomen structura {
    chorda*  titulus;
    /* intra= : definitio ad PARENTEM adstricta. NIHIL = globalis.
     * Necessarium quia idem nomen formas duas ferre potest:
     * aedilis <regula> intra <nexus> obiectum= poscit et
     * <vexillum> continet; intra <irregularia> caput= poscit et
     * <obiectum> continet. Declarationes GLOBALES (mos DTD) hoc
     * dicere non possunt; contextus (mos RELAX NG) potest.
     * INVENTUM dialecto SECUNDO describendo - natura eo caruit. */
    chorda*  intra;
    b32      radix;
    b32      textus_licet;
    /* TEXTUS ubi licet genus ferre potest (textus="numerus"):
     * textus non vacuus generi congruere debet. TEXTUS = sine
     * genere (textus="verum" vetus). */
    CanonGenusValoris  textus_genus;
    Xar*     attributa;  /* Xar de CanonAttributum */
    Xar*     liberi;     /* Xar de CanonLiberum */
} CanonElementum;

nomen structura {
    chorda*  titulus;
    chorda*  attributum;
    Xar*     super;      /* Xar de chorda* - genera elementorum */
} CanonUnicitas;

/* ==================================================
 * Prototypa
 * ================================================== */

interior CanonGenusValoris genus_legere(constans chorda* s);
interior b32 valor_congruit(constans chorda* v, CanonAttributum* a);
interior vacuum vitium_addere(Xar* vitia, CanonVitiumGenus genus,
    StmlNodus* nodus, chorda* elementum, chorda* detail,
    i32 numerus, i32 limes);
interior CanonElementum* elementum_quaerere(Canon* c,
    StmlNodus* n);
interior chorda clavis_scopi(Piscina* p, constans chorda* intra,
    constans chorda* titulus);
interior vacuum nodum_iudicare(Canon* c, StmlNodus* n, Xar* vitia,
    Piscina* piscina);
interior b32 album_solum(constans chorda* s);

/* ==================================================
 * Auxilia
 * ================================================== */

interior b32
album_solum(
    constans chorda* s)
{
    i32 i;

    per (i = ZEPHYRUM; i < s->mensura; i++)
    {
        character c;

        c = (character)s->datum[i];
        si (c != ' ' && c != '\t' && c != '\n' && c != '\r')
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

interior CanonGenusValoris
genus_legere(
    constans chorda* s)
{
    si (chorda_aequalis_literis(*s, "nomen"))   redde CANON_GENUS_NOMEN;
    si (chorda_aequalis_literis(*s, "numerus")) redde CANON_GENUS_NUMERUS;
    si (chorda_aequalis_literis(*s, "veritas")) redde CANON_GENUS_VERITAS;
    si (chorda_aequalis_literis(*s, "dies"))    redde CANON_GENUS_DIES;
    si (chorda_aequalis_literis(*s, "electio")) redde CANON_GENUS_ELECTIO;
    si (chorda_aequalis_literis(*s, "compositum"))
        redde CANON_GENUS_COMPOSITUM;

    redde CANON_GENUS_TEXTUS;
}

/* yyyy | yyyy-mm | yyyy-mm-dd */
interior b32
dies_bene_formata(
    constans chorda* d)
{
    i32 i;

    si (d->mensura != IV && d->mensura != VII && d->mensura != X)
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < d->mensura; i++)
    {
        character c;

        c = (character)d->datum[i];
        si (i == IV || i == VII)
        {
            si (c != '-')
            {
                redde FALSUM;
            }
        }
        alioquin si (c < '0' || c > '9')
        {
            redde FALSUM;
        }
    }
    si (d->mensura >= VII)
    {
        i32 m;

        m = (i32)(d->datum[V] - '0') * X + (i32)(d->datum[VI] - '0');
        si (m < I || m > XII)
        {
            redde FALSUM;
        }
    }
    si (d->mensura == X)
    {
        i32 dd;

        dd = (i32)(d->datum[VIII] - '0') * X +
             (i32)(d->datum[IX] - '0');
        si (dd < I || dd > XXXI)
        {
            redde FALSUM;
        }
    }

    redde VERUM;
}

interior b32
valor_congruit(
    constans chorda*  v,
    CanonAttributum*  a)
{
    i32 i;

    commutatio (a->genus)
    {
        casus CANON_GENUS_TEXTUS:
            redde VERUM;

        casus CANON_GENUS_NOMEN:
        casus CANON_GENUS_COMPOSITUM:
            si (v->mensura == ZEPHYRUM)
            {
                redde FALSUM;
            }
            per (i = ZEPHYRUM; i < v->mensura; i++)
            {
                character c;

                c = (character)v->datum[i];
                si (!((c >= 'a' && c <= 'z') ||
                      (c >= 'A' && c <= 'Z') ||
                      (c >= '0' && c <= '9') ||
                      c == '_' || c == '*' ||
                      (c == '-' &&
                       a->genus == CANON_GENUS_COMPOSITUM)))
                {
                    redde FALSUM;
                }
            }
            redde VERUM;

        casus CANON_GENUS_NUMERUS:
            si (v->mensura == ZEPHYRUM)
            {
                redde FALSUM;
            }
            per (i = ZEPHYRUM; i < v->mensura; i++)
            {
                character c;

                c = (character)v->datum[i];
                si (i == ZEPHYRUM && (c == '-' || c == '+'))
                {
                    perge;
                }
                si (c < '0' || c > '9')
                {
                    redde FALSUM;
                }
            }
            redde VERUM;

        casus CANON_GENUS_VERITAS:
            redde (b32)(chorda_aequalis_literis(*v, "verum") ||
                        chorda_aequalis_literis(*v, "falsum"));

        casus CANON_GENUS_DIES:
            redde dies_bene_formata(v);

        casus CANON_GENUS_ELECTIO:
            per (i = ZEPHYRUM; i < xar_numerus(a->optiones); i++)
            {
                chorda** o;

                o = (chorda**)xar_obtinere(a->optiones, i);
                si (chorda_aequalis(*v, **o))
                {
                    redde VERUM;
                }
            }
            redde FALSUM;

        ordinarius:
            redde VERUM;
    }
}

interior vacuum
vitium_addere(
    Xar*              vitia,
    CanonVitiumGenus  genus,
    StmlNodus*        nodus,
    chorda*           elementum,
    chorda*           detail,
    i32               numerus,
    i32               limes)
{
    CanonVitium* v;

    v = (CanonVitium*)xar_addere(vitia);
    si (!v)
    {
        redde;
    }

    v->genus     = genus;
    v->nodus     = nodus;
    v->elementum = elementum;
    v->detail    = detail;
    v->numerus   = numerus;
    v->limes     = limes;
}

/* clavis tabulae: "parens/nomen" pro definitione adstricta */
interior chorda
clavis_scopi(
    Piscina*          p,
    constans chorda*  intra,
    constans chorda*  titulus)
{
    chorda k;

    k.mensura = intra->mensura + I + titulus->mensura;
    k.datum   = (i8*)piscina_allocare(p, (memoriae_index)k.mensura);
    memcpy(k.datum, intra->datum, (memoriae_index)intra->mensura);
    k.datum[intra->mensura] = (i8)'/';
    memcpy(k.datum + intra->mensura + I, titulus->datum,
           (memoriae_index)titulus->mensura);

    redde k;
}

/* Definitionem ADSTRICTAM prius quaerere, deinde globalem:
 * specialius vincit. */
interior CanonElementum*
elementum_quaerere(
    Canon*      c,
    StmlNodus*  n)
{
    vacuum* valor;

    si (n->parens && n->parens->genus == STML_NODUS_ELEMENTUM &&
        n->parens->titulus)
    {
        PiscinaNotatio nota;
        chorda         k;
        b32            inventum;

        nota = piscina_notare(c->piscina);
        k = clavis_scopi(c->piscina, n->parens->titulus, n->titulus);
        inventum = tabula_dispersa_invenire(c->elementa, k, &valor);
        piscina_reficere(c->piscina, nota);
        si (inventum)
        {
            redde (CanonElementum*)valor;
        }
    }

    si (!tabula_dispersa_invenire(c->elementa, *n->titulus, &valor))
    {
        redde NIHIL;
    }

    redde (CanonElementum*)valor;
}

StmlNodus*
canon_infixum_invenire(
    StmlNodus* elementum_radix)
{
    i32 numerus;
    i32 i;

    si (!elementum_radix ||
        elementum_radix->genus != STML_NODUS_ELEMENTUM)
    {
        redde NIHIL;
    }

    numerus = stml_numerus_liberorum(elementum_radix);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;

        l = stml_liberum_ad_indicem(elementum_radix, i);
        si (!l || l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;   /* textus albus et commentaria non numerant */
        }
        si (l->titulus &&
            chorda_aequalis_literis(*l->titulus, "canon"))
        {
            redde l;
        }
        redde NIHIL;   /* liberum elementare PRIMUM solum */
    }

    redde NIHIL;
}

/* ==================================================
 * Lectio canonis
 * ================================================== */

Canon*
canon_legere(
    chorda                fons,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    chorda*               causa)
{
    StmlResultus r;

    r = stml_legere(fons, piscina, intern);
    si (!r.successus || !r.elementum_radix)
    {
        si (causa)
        {
            *causa = chorda_ex_literis("canon parsari nequit",
                                       piscina);
        }
        redde NIHIL;
    }

    redde canon_ex_nodo(r.elementum_radix, piscina, intern, causa);
}

Canon*
canon_ex_nodo(
    StmlNodus*            elementum,
    Piscina*              piscina,
    InternamentumChorda*  intern,
    chorda*               causa)
{
    Canon*  c;
    i32     numerus;
    i32     i;

    si (causa)
    {
        causa->datum   = NIHIL;
        causa->mensura = ZEPHYRUM;
    }

    si (!elementum ||
        elementum->genus != STML_NODUS_ELEMENTUM ||
        !elementum->titulus ||
        !chorda_aequalis_literis(*elementum->titulus, "canon"))
    {
        si (causa)
        {
            *causa = chorda_ex_literis("radix non est <canon>",
                                       piscina);
        }
        redde NIHIL;
    }

    c = (Canon*)piscina_allocare(piscina, magnitudo(Canon));
    c->piscina    = piscina;
    c->intern     = intern;
    c->dialectus  = stml_attributum_capere(elementum, "dialectus");
    c->versio     = stml_attributum_capere(elementum, "versio");
    c->elementa   = tabula_dispersa_creare_chorda(piscina, LXIV);
    c->unicitates = xar_creare(piscina, (i32)magnitudo(CanonUnicitas*));
    c->radix      = NIHIL;

    numerus = stml_numerus_liberorum(elementum);
    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* n;
        chorda*    titulus;

        n = stml_liberum_ad_indicem(elementum, i);
        si (!n || n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }

        titulus = stml_attributum_capere(n, "nomen");
        si (!titulus)
        {
            perge;
        }

        si (chorda_aequalis_literis(*n->titulus, "elementum"))
        {
            CanonElementum* e;
            chorda*         attr;
            i32             nl;
            i32             j;

            e = (CanonElementum*)piscina_allocare(piscina,
                magnitudo(CanonElementum));
            e->titulus      = titulus;
            e->attributa    = xar_creare(piscina,
                                (i32)magnitudo(CanonAttributum));
            e->liberi       = xar_creare(piscina,
                                (i32)magnitudo(CanonLiberum));
            e->intra = stml_attributum_capere(n, "intra");
            attr = stml_attributum_capere(n, "radix");
            e->radix = (b32)(attr &&
                chorda_aequalis_literis(*attr, "verum"));
            /* textus=: verum = licet sine genere; genus nominatum
             * (numerus/dies/...) = licet et congruere debet;
             * absens aut ignotus = vetitus. 'electio' pro textu
             * mechanismum optionum non habet - ut verum tractatur
             * (canon.canon eam ex electione sua excludit). */
            e->textus_licet = FALSUM;
            e->textus_genus = CANON_GENUS_TEXTUS;
            attr = stml_attributum_capere(n, "textus");
            si (attr)
            {
                si (chorda_aequalis_literis(*attr, "verum"))
                {
                    e->textus_licet = VERUM;
                }
                alioquin
                {
                    CanonGenusValoris g;

                    g = genus_legere(attr);
                    si (g == CANON_GENUS_ELECTIO)
                    {
                        e->textus_licet = VERUM;
                    }
                    alioquin si (g != CANON_GENUS_TEXTUS)
                    {
                        e->textus_licet = VERUM;
                        e->textus_genus = g;
                    }
                }
            }

            si (e->radix)
            {
                c->radix = titulus;
            }

            nl = stml_numerus_liberorum(n);
            per (j = ZEPHYRUM; j < nl; j++)
            {
                StmlNodus* l;
                chorda*    lt;

                l = stml_liberum_ad_indicem(n, j);
                si (!l || l->genus != STML_NODUS_ELEMENTUM)
                {
                    perge;
                }
                lt = stml_attributum_capere(l, "nomen");
                si (!lt)
                {
                    perge;
                }

                si (chorda_aequalis_literis(*l->titulus,
                                            "attributum"))
                {
                    CanonAttributum* a;
                    chorda*          g;
                    i32              no;
                    i32              k;

                    a = (CanonAttributum*)xar_addere(e->attributa);
                    a->titulus  = lt;
                    g = stml_attributum_capere(l, "genus");
                    a->genus = g ? genus_legere(g)
                                 : CANON_GENUS_TEXTUS;
                    g = stml_attributum_capere(l, "necessarium");
                    a->necessarium = (b32)(g &&
                        chorda_aequalis_literis(*g, "verum"));
                    a->optiones = xar_creare(piscina,
                                    (i32)magnitudo(chorda*));

                    no = stml_numerus_liberorum(l);
                    per (k = ZEPHYRUM; k < no; k++)
                    {
                        StmlNodus* o;
                        chorda**   locus;
                        chorda     t;

                        o = stml_liberum_ad_indicem(l, k);
                        si (!o ||
                            o->genus != STML_NODUS_ELEMENTUM ||
                            !chorda_aequalis_literis(*o->titulus,
                                                     "optio"))
                        {
                            perge;
                        }
                        t = chorda_praecidere(
                            stml_textus_internus(o, piscina));
                        locus = (chorda**)xar_addere(a->optiones);
                        *locus = chorda_internare(intern, t);
                    }
                }
                alioquin si (chorda_aequalis_literis(*l->titulus,
                                                     "liberum"))
                {
                    CanonLiberum* lb;
                    chorda*       m;

                    lb = (CanonLiberum*)xar_addere(e->liberi);
                    lb->titulus = lt;
                    lb->minimum = ZEPHYRUM;
                    lb->maximum = -I;

                    m = stml_attributum_capere(l, "minimum");
                    si (m)
                    {
                        i32 val;

                        si (chorda_ut_i32(*m, &val))
                        {
                            lb->minimum = val;
                        }
                    }
                    m = stml_attributum_capere(l, "maximum");
                    si (m)
                    {
                        i32 val;

                        si (chorda_ut_i32(*m, &val))
                        {
                            lb->maximum = (s32)val;
                        }
                    }
                }
            }

            si (e->intra)
            {
                tabula_dispersa_inserere(c->elementa,
                    clavis_scopi(piscina, e->intra, titulus), e);
            }
            alioquin
            {
                tabula_dispersa_inserere(c->elementa, *titulus, e);
            }
        }
        alioquin si (chorda_aequalis_literis(*n->titulus,
                                             "unicitas"))
        {
            CanonUnicitas*  u;
            CanonUnicitas** locus;
            chorda*         super;

            u = (CanonUnicitas*)piscina_allocare(piscina,
                magnitudo(CanonUnicitas));
            u->titulus    = titulus;
            u->attributum = stml_attributum_capere(n, "attributum");
            u->super      = xar_creare(piscina,
                                (i32)magnitudo(chorda*));

            super = stml_attributum_capere(n, "super");
            si (super)
            {
                chorda_fissio_fructus f;
                i32                   k;

                f = chorda_fissio(*super, ' ', piscina);
                per (k = ZEPHYRUM; k < f.numerus; k++)
                {
                    chorda   t;
                    chorda** locus_s;

                    t = chorda_praecidere(f.elementa[k]);
                    si (t.mensura == ZEPHYRUM)
                    {
                        perge;
                    }
                    locus_s = (chorda**)xar_addere(u->super);
                    *locus_s = chorda_internare(intern, t);
                }
            }

            locus = (CanonUnicitas**)xar_addere(c->unicitates);
            *locus = u;
        }
    }

    redde c;
}

/* ==================================================
 * Iudicium
 * ================================================== */

interior vacuum
nodum_iudicare(
    Canon*      c,
    StmlNodus*  n,
    Xar*        vitia,
    Piscina*    piscina)
{
    CanonElementum* e;
    StmlNodus*      infixus;
    i32             numerus;
    i32             i;

    si (!n || n->genus != STML_NODUS_ELEMENTUM || !n->titulus)
    {
        redde;
    }

    /* canon infixus (liberum primum radicis) contractus est, non
     * contentum: pro liberis, licentia, textu INVISIBILIS - alibi
     * eum canon_examen contra canonem canonum iudicat. Radix =
     * elementum sine parente elementari. */
    infixus = NIHIL;
    si (!n->parens || n->parens->genus != STML_NODUS_ELEMENTUM)
    {
        infixus = canon_infixum_invenire(n);
    }

    e = elementum_quaerere(c, n);
    si (!e)
    {
        vitium_addere(vitia, CANON_ELEMENTUM_IGNOTUM, n,
                      n->titulus, NIHIL, ZEPHYRUM, ZEPHYRUM);
        redde;   /* ignotum: liberi eius non iudicantur */
    }

    /* ---- attributa praesentia ---- */
    per (i = ZEPHYRUM; i < xar_numerus(n->attributa); i++)
    {
        StmlAttributum*  a;
        CanonAttributum* def;
        i32              j;

        a = (StmlAttributum*)xar_obtinere(n->attributa, i);
        def = NIHIL;
        per (j = ZEPHYRUM; j < xar_numerus(e->attributa); j++)
        {
            CanonAttributum* cand;

            cand = (CanonAttributum*)xar_obtinere(e->attributa, j);
            si (chorda_aequalis(*cand->titulus, *a->titulus))
            {
                def = cand;
                frange;
            }
        }

        si (!def)
        {
            vitium_addere(vitia, CANON_ATTRIBUTUM_IGNOTUM, n,
                          e->titulus, a->titulus, ZEPHYRUM,
                          ZEPHYRUM);
            perge;
        }
        si (!valor_congruit(a->valor, def))
        {
            vitium_addere(vitia, CANON_VALOR_MALUS, n, e->titulus,
                          a->titulus, ZEPHYRUM, ZEPHYRUM);
        }
    }

    /* ---- attributa necessaria ---- */
    per (i = ZEPHYRUM; i < xar_numerus(e->attributa); i++)
    {
        CanonAttributum* def;

        def = (CanonAttributum*)xar_obtinere(e->attributa, i);
        si (def->necessarium &&
            !stml_attributum_capere(n, chorda_ut_cstr(*def->titulus,
                                                      piscina)))
        {
            vitium_addere(vitia, CANON_ATTRIBUTUM_DEEST, n,
                          e->titulus, def->titulus, ZEPHYRUM,
                          ZEPHYRUM);
        }
    }

    /* ---- textus typatus: non vacuus generi congruere debet ---- */
    si (e->textus_licet && e->textus_genus != CANON_GENUS_TEXTUS)
    {
        chorda textus_totus;

        textus_totus = chorda_praecidere(
            stml_textus_normalizatus(n, piscina));
        si (textus_totus.mensura > ZEPHYRUM)
        {
            CanonAttributum tmp;

            tmp.titulus     = NIHIL;
            tmp.genus       = e->textus_genus;
            tmp.necessarium = FALSUM;
            tmp.optiones    = NIHIL;
            si (!valor_congruit(&textus_totus, &tmp))
            {
                chorda* d;

                d = (chorda*)piscina_allocare(piscina,
                                              magnitudo(chorda));
                *d = textus_totus;
                vitium_addere(vitia, CANON_TEXTUS_MALUS, n,
                              e->titulus, d, ZEPHYRUM, ZEPHYRUM);
            }
        }
    }

    /* ---- liberi: licentia et cardinalitas ---- */
    numerus = stml_numerus_liberorum(n);
    per (i = ZEPHYRUM; i < xar_numerus(e->liberi); i++)
    {
        CanonLiberum* lb;
        i32           computa;
        i32           j;

        lb = (CanonLiberum*)xar_obtinere(e->liberi, i);
        computa = ZEPHYRUM;
        per (j = ZEPHYRUM; j < numerus; j++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(n, j);
            si (l && l != infixus &&
                l->genus == STML_NODUS_ELEMENTUM &&
                chorda_aequalis(*l->titulus, *lb->titulus))
            {
                computa++;
            }
        }

        si (computa < lb->minimum)
        {
            vitium_addere(vitia, CANON_LIBERI_PAUCI, n, e->titulus,
                          lb->titulus, computa, lb->minimum);
        }
        si (lb->maximum >= ZEPHYRUM && computa > (i32)lb->maximum)
        {
            vitium_addere(vitia, CANON_LIBERI_MULTI, n, e->titulus,
                          lb->titulus, computa, (i32)lb->maximum);
        }
    }

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        StmlNodus* l;
        b32        licet;
        i32        j;

        l = stml_liberum_ad_indicem(n, i);
        si (!l || l == infixus)
        {
            perge;
        }

        si (l->genus == STML_NODUS_TEXTUS)
        {
            si (!e->textus_licet && l->valor &&
                !album_solum(l->valor))
            {
                vitium_addere(vitia, CANON_TEXTUS_ILLICITUS, l,
                              e->titulus, NIHIL, ZEPHYRUM,
                              ZEPHYRUM);
            }
            perge;
        }
        si (l->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }

        licet = FALSUM;
        per (j = ZEPHYRUM; j < xar_numerus(e->liberi); j++)
        {
            CanonLiberum* lb;

            lb = (CanonLiberum*)xar_obtinere(e->liberi, j);
            si (chorda_aequalis(*lb->titulus, *l->titulus))
            {
                licet = VERUM;
                frange;
            }
        }
        si (!licet)
        {
            vitium_addere(vitia, CANON_LIBERUM_ILLICITUM, l,
                          e->titulus, l->titulus, ZEPHYRUM,
                          ZEPHYRUM);
        }

        nodum_iudicare(c, l, vitia, piscina);
    }
}

Xar*
canon_iudicare(
    Canon*      canon,
    StmlNodus*  radix,
    Piscina*    piscina)
{
    Xar*       vitia;
    StmlNodus* elementum_radix;
    i32        i;

    si (!canon || !piscina)
    {
        redde NIHIL;
    }

    vitia = xar_creare(piscina, (i32)magnitudo(CanonVitium));
    si (!radix)
    {
        redde vitia;
    }

    /* nodum documenti aut elementum accipimus */
    elementum_radix = radix;
    si (radix->genus != STML_NODUS_ELEMENTUM)
    {
        i32 numerus;

        elementum_radix = NIHIL;
        numerus = stml_numerus_liberorum(radix);
        per (i = ZEPHYRUM; i < numerus; i++)
        {
            StmlNodus* l;

            l = stml_liberum_ad_indicem(radix, i);
            si (l && l->genus == STML_NODUS_ELEMENTUM)
            {
                elementum_radix = l;
                frange;
            }
        }
    }
    si (!elementum_radix)
    {
        redde vitia;
    }

    si (canon->radix &&
        !chorda_aequalis(*elementum_radix->titulus, *canon->radix))
    {
        vitium_addere(vitia, CANON_RADIX_MALA, elementum_radix,
                      elementum_radix->titulus, canon->radix,
                      ZEPHYRUM, ZEPHYRUM);
    }

    nodum_iudicare(canon, elementum_radix, vitia, piscina);

    /* ---- unicitates (subarbore infixi praetermissa) ---- */
    per (i = ZEPHYRUM; i < xar_numerus(canon->unicitates); i++)
    {
        CanonUnicitas*  u;
        TabulaDispersa* visa;
        Xar*            acervus;
        StmlNodus*      infixus;
        i32             j;

        infixus = canon_infixum_invenire(elementum_radix);

        u = *(CanonUnicitas**)xar_obtinere(canon->unicitates, i);
        si (!u->attributum)
        {
            perge;
        }

        visa = tabula_dispersa_creare_chorda(piscina, CXXVIII);
        acervus = xar_creare(piscina, (i32)magnitudo(StmlNodus*));
        {
            StmlNodus** locus;

            locus = (StmlNodus**)xar_addere(acervus);
            *locus = elementum_radix;
        }

        per (j = ZEPHYRUM; j < xar_numerus(acervus); j++)
        {
            StmlNodus* n;
            i32        numerus;
            i32        k;

            n = *(StmlNodus**)xar_obtinere(acervus, j);
            numerus = stml_numerus_liberorum(n);
            per (k = ZEPHYRUM; k < numerus; k++)
            {
                StmlNodus** locus;
                StmlNodus*  l;

                l = stml_liberum_ad_indicem(n, k);
                si (!l || l == infixus ||
                    l->genus != STML_NODUS_ELEMENTUM)
                {
                    perge;
                }
                locus = (StmlNodus**)xar_addere(acervus);
                *locus = l;
            }

            /* an hoc elementum sub unicitate cadat */
            {
                b32 sub;
                i32 m;

                sub = FALSUM;
                per (m = ZEPHYRUM; m < xar_numerus(u->super); m++)
                {
                    chorda** t;

                    t = (chorda**)xar_obtinere(u->super, m);
                    si (chorda_aequalis(*n->titulus, **t))
                    {
                        sub = VERUM;
                        frange;
                    }
                }
                si (sub)
                {
                    chorda* v;

                    v = stml_attributum_capere(n,
                        chorda_ut_cstr(*u->attributum, piscina));
                    si (v)
                    {
                        si (tabula_dispersa_continet(visa, *v))
                        {
                            vitium_addere(vitia, CANON_NOMEN_BIS,
                                n, n->titulus, v, ZEPHYRUM,
                                ZEPHYRUM);
                        }
                        alioquin
                        {
                            tabula_dispersa_inserere(visa, *v, n);
                        }
                    }
                }
            }
        }
    }

    redde vitia;
}

constans character*
canon_nuntius(
    CanonVitiumGenus genus)
{
    commutatio (genus)
    {
        casus CANON_ELEMENTUM_IGNOTUM:
            redde "elementum extra canonem";
        casus CANON_ATTRIBUTUM_IGNOTUM:
            redde "attributum extra canonem";
        casus CANON_ATTRIBUTUM_DEEST:
            redde "attributum necessarium deest";
        casus CANON_VALOR_MALUS:
            redde "valor generi attributi non congruit";
        casus CANON_LIBERUM_ILLICITUM:
            redde "liberum hic non licet";
        casus CANON_LIBERI_PAUCI:
            redde "liberi pauciores quam minimum";
        casus CANON_LIBERI_MULTI:
            redde "liberi plures quam maximum";
        casus CANON_TEXTUS_ILLICITUS:
            redde "textus ubi non licet";
        casus CANON_TEXTUS_MALUS:
            redde "textus generi elementi non congruit";
        casus CANON_NOMEN_BIS:
            redde "nomen bis in spatio unico";
        casus CANON_RADIX_MALA:
            redde "elementum radicis aliud quam canon poscit";
        ordinarius:
            redde "vitium ignotum";
    }
}

/* ==================================================
 * Registrum
 * ================================================== */

chorda
canon_registrum_quaerere_radice(
    chorda            catalogus,
    constans chorda*  radix,
    Piscina*          piscina)
{
    chorda                vacua;
    chorda_fissio_fructus lineae;
    i32                   i;

    vacua.datum   = NIHIL;
    vacua.mensura = ZEPHYRUM;

    si (!radix || radix->mensura == ZEPHYRUM)
    {
        redde vacua;
    }

    lineae = chorda_fissio(catalogus, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        chorda                linea;
        chorda_fissio_fructus campi;

        linea = chorda_praecidere(lineae.elementa[i]);
        si (linea.mensura == ZEPHYRUM ||
            (character)linea.datum[ZEPHYRUM] == '#')
        {
            perge;
        }

        campi = chorda_fissio(linea, '\t', piscina);
        si (campi.numerus < II)
        {
            perge;
        }
        {
            chorda clavis;

            clavis = chorda_praecidere(campi.elementa[ZEPHYRUM]);
            si (clavis.mensura >= III &&
                (character)clavis.datum[ZEPHYRUM] == '<' &&
                (character)clavis.datum[clavis.mensura - I] == '>')
            {
                chorda intus;

                intus = chorda_sectio(clavis, I,
                                      clavis.mensura - I);
                si (chorda_aequalis(intus, *radix))
                {
                    redde chorda_praecidere(campi.elementa[I]);
                }
            }
        }
    }

    redde vacua;
}

chorda
canon_registrum_quaerere(
    chorda               catalogus,
    constans character*  via_plagulae,
    Piscina*             piscina)
{
    chorda                vacua;
    chorda_fissio_fructus lineae;
    constans character*   punctum;
    i32                   i;

    vacua.datum   = NIHIL;
    vacua.mensura = ZEPHYRUM;

    si (!via_plagulae)
    {
        redde vacua;
    }
    punctum = strrchr(via_plagulae, '.');
    si (!punctum)
    {
        redde vacua;
    }

    lineae = chorda_fissio(catalogus, '\n', piscina);
    per (i = ZEPHYRUM; i < lineae.numerus; i++)
    {
        chorda                linea;
        chorda_fissio_fructus campi;

        linea = chorda_praecidere(lineae.elementa[i]);
        si (linea.mensura == ZEPHYRUM ||
            (character)linea.datum[ZEPHYRUM] == '#')
        {
            perge;
        }

        campi = chorda_fissio(linea, '\t', piscina);
        si (campi.numerus < II)
        {
            perge;
        }
        {
            chorda ext;
            chorda via;

            ext = chorda_praecidere(campi.elementa[ZEPHYRUM]);
            via = chorda_praecidere(campi.elementa[I]);
            si (chorda_aequalis_literis(ext, punctum))
            {
                redde via;
            }
        }
    }

    redde vacua;
}
