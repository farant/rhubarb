/* sancti.c - bibliotheca sanctorum (include/sancti.h; project-specs/
 * sancti-plan.md par. II). Lectio: STML, canon infixus
 * (sancti_canon.c),
 * deinde regulae lectoris; corpus: ambulatio directorii, ordo viarum
 * (vitia et 'primus vincit' in ids duplicibus deterministica), ordo id,
 * iunctura sanctoralis. */
#include "sancti.h"
#include "sancti_canon.h"
#include "stml.h"
#include "canon.h"
#include "filum.h"
#include "iter_directoria.h"
#include "tabula_dispersa.h"
#include "chorda_aedificator.h"

#include <stdio.h>
#include <string.h>

#define SUFFIXUM ".sanctus"

structura SanctiCorpus {
               Xar* sancti;      /* Sanctus* ordine id */
    TabulaDispersa* per_id;      /* id -> Sanctus* */
               Xar* vitia;       /* SanctiVitium */
               Xar* iuncturae;   /* Iunctura */
};

/* linea sanctorale.tsv: celebratio -> ids (vacuum = sine sancto) */
nomen structura {
       s32  mensis;
       s32  dies;
    chorda  titulus;
       Xar* ids;                  /* chorda */
} Iunctura;

/* status lectionis plagulae unius */
nomen structura {
              chorda  via;
             Piscina* piscina;
                 Xar* vitia;
                 b32  fractum;
      TabulaDispersa* fontes;     /* id fontis -> nodus */
} Lector;

hic_manens constans s32 dies_mensium[XII] = {
    XXXI, XXIX, XXXI, XXX, XXXI, XXX, XXXI, XXXI, XXX, XXXI, XXX, XXXI
};


/* ==================================================
 * Auxilia
 * ================================================== */

interior chorda
_chorda_nulla (vacuum)
{
    chorda c;

    c.datum    = NIHIL;
    c.mensura  = ZEPHYRUM;
    redde c;
}

interior vacuum
_vitium_addere (
                   Xar* vitia,
                chorda  via,
                   i32  linea,
               Piscina* piscina,
    constans character* nuntius)
{
    SanctiVitium* v;

    si (vitia == NIHIL)
    {
        redde;
    }
    v = (SanctiVitium*)xar_addere(vitia);
    si (v == NIHIL)
    {
        redde;
    }
    v->via    = via;
    v->linea  = linea;
    v->nuntius  = chorda_transcribere(chorda_ex_literis(nuntius,
        piscina), piscina);
}

interior vacuum
_vitium (
                Lector* l,
             StmlNodus* nodus,
    constans character* nuntius)
{
    _vitium_addere(l->vitia, l->via, nodus ? nodus->linea : ZEPHYRUM,
        l->piscina, nuntius);
    l->fractum = VERUM;
}

interior b32
_titulus_est (
             StmlNodus* nodus,
    constans character* titulus)
{
    redde nodus && nodus->genus == STML_NODUS_ELEMENTUM
        && nodus->titulus
        && chorda_aequalis_literis(*nodus->titulus, titulus);
}

interior chorda
_attributum (
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* a;

    a = stml_attributum_capere(nodus, titulus);
    redde a ? *a : _chorda_nulla();
}

interior s32
_numerus (
             StmlNodus* nodus,
    constans character* titulus)
{
    chorda* a;
       s32  n;

    a = stml_attributum_capere(nodus, titulus);
    si (a == NIHIL || !chorda_ut_s32(*a, &n))
    {
        redde ZEPHYRUM;
    }
    redde n;
}

interior b32
_verum (
             StmlNodus* nodus,
    constans character* titulus)
{
    redde chorda_aequalis_literis(_attributum(nodus, titulus), "verum");
}

/* textus elementi ut prosa, praecisus, in piscina */
interior chorda
_textus (
    StmlNodus* nodus,
      Piscina* piscina)
{
    chorda fluxus;

    fluxus = stml_textus_fluxus(nodus, piscina);
    redde chorda_transcribere(chorda_praecidere(fluxus), piscina);
}

/* 'fontes' elementi: omnis id ad <fons> refert */
interior vacuum
_fontes_probare (
       Lector* l,
    StmlNodus* nodus)
{
                   chorda fontes;
    chorda_fissio_fructus partes;
                      i32 i;
                character nuntius[CCLVI];

    fontes = _attributum(nodus, "fontes");
    si (fontes.mensura == ZEPHYRUM)
    {
        redde;
    }
    partes = chorda_fissio(fontes, ' ', l->piscina);
    per (i = ZEPHYRUM; i < (i32)partes.numerus; i++)
    {
        si (partes.elementa[i].mensura == ZEPHYRUM)
        {
            perge;
        }
        si (!tabula_dispersa_continet(l->fontes, partes.elementa[i]))
        {
            sprintf(nuntius, "fontes: '%.*s' nullum <fons> in plagula "
                "(<%.*s>)", (integer)(partes.elementa[i].mensura > LXIV
                ? LXIV : partes.elementa[i].mensura),
                (constans character*)partes.elementa[i].datum,
                (integer)nodus->titulus->mensura,
                (constans character*)nodus->titulus->datum);
            _vitium(l, nodus, nuntius);
        }
    }
}

/* dies incertus ex elemento; mensis/dies probantur */
interior SanctiDies
_dies (
       Lector* l,
    StmlNodus* nodus)
{
    SanctiDies d;
     character nuntius[CXXVIII];

    memset(&d, ZEPHYRUM, magnitudo(SanctiDies));
    d.notus     = VERUM;
    d.annus     = _numerus(nodus, "annus");
    d.mensis    = _numerus(nodus, "mensis");
    d.dies      = _numerus(nodus, "dies");
    d.saeculum  = _numerus(nodus, "saeculum");
    d.circa     = _verum(nodus, "circa");
    d.julianum  = chorda_aequalis_literis(_attributum(nodus,
        "calendarium"), "julianum");
    d.locus     = _attributum(nodus, "locus");
    si (d.dies > ZEPHYRUM && d.mensis == ZEPHYRUM)
    {
        _vitium(l, nodus, "dies sine mense");
    }
    si (   d.mensis >= I && d.mensis <= XII
        && d.dies > dies_mensium[d.mensis - I])
    {
        sprintf(nuntius, "dies %d mensi %d non congruit",
            (integer)d.dies,
            (integer)d.mensis);
        _vitium(l, nodus, nuntius);
    }
    redde d;
}

interior SanctiCalendarium
_calendarium (
    chorda c)
{
    si (chorda_aequalis_literis(c, "usa"))
    {
        redde SANCTI_CALENDARIUM_USA;
    }
    si (chorda_aequalis_literis(c, "martyrologium"))
    {
        redde SANCTI_CALENDARIUM_MARTYROLOGIUM;
    }
    si (chorda_aequalis_literis(c, "generale-1962"))
    {
        redde SANCTI_CALENDARIUM_VETUS;
    }
    si (chorda_aequalis_literis(c, "alia"))
    {
        redde SANCTI_CALENDARIUM_ALIUD;
    }
    redde SANCTI_CALENDARIUM_GENERALE;
}

interior SanctiGradus
_gradus (
    chorda g)
{
    si (chorda_aequalis_literis(g, "sollemnitas"))
    {
        redde SANCTI_GRADUS_SOLLEMNITAS;
    }
    si (chorda_aequalis_literis(g, "festum"))
    {
        redde SANCTI_GRADUS_FESTUM;
    }
    si (chorda_aequalis_literis(g, "memoria"))
    {
        redde SANCTI_GRADUS_MEMORIA;
    }
    si (chorda_aequalis_literis(g, "memoria-ad-libitum"))
    {
        redde SANCTI_GRADUS_MEMORIA_AD_LIBITUM;
    }
    si (chorda_aequalis_literis(g, "commemoratio"))
    {
        redde SANCTI_GRADUS_COMMEMORATIO;
    }
    redde SANCTI_GRADUS_NULLUS;
}

interior SanctiStatus
_status (
    chorda s)
{
    si (chorda_aequalis_literis(s, "beatus"))
    {
        redde SANCTI_BEATUS;
    }
    si (chorda_aequalis_literis(s, "venerabilis"))
    {
        redde SANCTI_VENERABILIS;
    }
    si (chorda_aequalis_literis(s, "servus-dei"))
    {
        redde SANCTI_SERVUS_DEI;
    }
    redde SANCTI_SANCTUS;
}

interior SanctiCanonizatio
_modus (
    chorda m)
{
    si (chorda_aequalis_literis(m, "formalis"))
    {
        redde SANCTI_CANONIZATIO_FORMALIS;
    }
    si (chorda_aequalis_literis(m, "aequipollens"))
    {
        redde SANCTI_CANONIZATIO_AEQUIPOLLENS;
    }
    si (chorda_aequalis_literis(m, "ante-congregationem"))
    {
        redde SANCTI_CANONIZATIO_ANTE_CONGREGATIONEM;
    }
    redde SANCTI_CANONIZATIO_NULLA;
}

/* nomen plagulae sine directorio et suffixo */
interior chorda
_basis (
    chorda via)
{
    s32 i;
    i32 initium;
    i32 finis;

    initium = ZEPHYRUM;
    per (i = (s32)via.mensura - I; i >= ZEPHYRUM; i--)
    {
        si (via.datum[i] == '/')
        {
            initium = (i32)i + I;
            frange;
        }
    }
    finis = via.mensura;
    si (   finis - initium >= (i32)(magnitudo(SUFFIXUM) - I)
        && memcmp(via.datum + finis - (magnitudo(SUFFIXUM) - I),
        SUFFIXUM,
               magnitudo(SUFFIXUM) - I) == ZEPHYRUM)
    {
        finis -= (i32)(magnitudo(SUFFIXUM) - I);
    }
    redde chorda_sectio(via, initium, finis);
}


/* ==================================================
 * Lectio plagulae
 * ================================================== */

/* canon iudicat: vitia nominata; VERUM si sanum */
interior b32
_canonem_iudicare (
        Lector* l,
         Canon* canon,
     StmlNodus* radix)
{
        Xar* cv;
        i32  i;
  character  nuntius[CCCLXXXIV];

    cv = canon_iudicare(canon, radix, l->piscina);
    per (i = ZEPHYRUM; cv && i < xar_numerus(cv); i++)
    {
        CanonVitium* c = (CanonVitium*)xar_obtinere(cv, i);

        sprintf(nuntius, "%s%s%.*s%s%.*s", canon_nuntius(c->genus),
            c->elementum ? ": <" : "",
            c->elementum ? (integer)(c->elementum->mensura > CC ? CC
                                     : c->elementum->mensura) : 0,
            c->elementum
                ? (constans character*)c->elementum->datum : "",
            c->detail ? "> " : (c->elementum ? ">" : ""),
            c->detail ? (integer)(c->detail->mensura > CC ? CC
                                  : c->detail->mensura) : 0,
            c->detail ? (constans character*)c->detail->datum : "");
        _vitium(l, c->nodus, nuntius);
    }
    redde !l->fractum;
}

/* liberi elementares: fontes primum (ids), deinde campi */
interior vacuum
_campos_legere (
       Lector* l,
    StmlNodus* radix,
      Sanctus* s)
{
          i32 i;
          i32 j;
          i32 principalia;
    character nuntius[CXXVIII];

    principalia = ZEPHYRUM;
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(radix, i);

        si (_titulus_est(n, "fons"))
        {
            SanctiFons* f = (SanctiFons*)xar_addere(s->fontes);

            si (f == NIHIL)
            {
                redde;
            }
            f->id     = _attributum(n, "id");
            f->opus   = _attributum(n, "opus");
            f->locus  = _attributum(n, "locus");
            f->url    = _attributum(n, "url");
            f->excerpta = xar_creare(l->piscina,
                (i32)magnitudo(chorda));
            per (j = ZEPHYRUM; j < stml_numerus_liberorum(n); j++)
            {
                StmlNodus* e = stml_liberum_ad_indicem(n, j);

                si (_titulus_est(e, "excerptum") && f->excerpta)
                {
                    *(chorda*)xar_addere(f->excerpta) = _textus(e,
                        l->piscina);
                }
            }
            si (tabula_dispersa_continet(l->fontes, f->id))
            {
                sprintf(nuntius, "fons id duplex: '%.*s'",
                    (integer)(f->id.mensura
                        > LXIV ? LXIV : f->id.mensura),
                    (constans character*)f->id.datum);
                _vitium(l, n, nuntius);
            }
            (vacuum)tabula_dispersa_inserere(l->fontes, f->id, n);
        }
    }
    per (i = ZEPHYRUM; i < stml_numerus_liberorum(radix); i++)
    {
        StmlNodus* n = stml_liberum_ad_indicem(radix, i);

        si (n->genus != STML_NODUS_ELEMENTUM)
        {
            perge;
        }
        _fontes_probare(l, n);
        si (_titulus_est(n, "nomen"))
        {
            SanctiNomen* nm = (SanctiNomen*)xar_addere(s->nomina);

            si (nm == NIHIL)
            {
                redde;
            }
            nm->lingua     = _attributum(n, "lingua");
            nm->textus     = _textus(n, l->piscina);
            nm->primarium  = _verum(n, "principale");
            si (   nm->primarium
                && chorda_aequalis_literis(nm->lingua, "en"))
            {
                principalia++;
                s->titulus = nm->textus;
            }
            si (   nm->primarium && chorda_vacua(s->titulus_latinus)
                && chorda_aequalis_literis(nm->lingua, "la"))
            {
                s->titulus_latinus = nm->textus;
            }
        }
        alioquin si (_titulus_est(n, "ortus"))
        {
            s->ortus = _dies(l, n);
        }
        alioquin si (_titulus_est(n, "obitus"))
        {
            s->obitus = _dies(l, n);
            s->martyr = _verum(n, "martyrium");
        }
        alioquin si (_titulus_est(n, "canonizatio"))
        {
            s->canonizatio       = _modus(_attributum(n, "modus"));
            s->canonizatio_dies  = _dies(l, n);
            s->canonizatio_papa  = _attributum(n, "papa");
        }
        alioquin si (_titulus_est(n, "beatificatio"))
        {
            s->beatificatio       = _dies(l, n);
            s->beatificatio_papa  = _attributum(n, "papa");
        }
        alioquin si (_titulus_est(n, "festum"))
        {
             SanctiFestum* f = (SanctiFestum*)xar_addere(s->festa);
               SanctiDies  d;

            si (f == NIHIL)
            {
                redde;
            }
            d = _dies(l, n);
            f->calendarium = _calendarium(_attributum(n,
                "calendarium"));
            f->mensis  = d.mensis;
            f->dies    = d.dies;
            f->gradus  = _gradus(_attributum(n, "gradus"));
            f->nota    = _attributum(n, "nota");
        }
        alioquin si (_titulus_est(n, "munus"))
        {
            *(chorda*)xar_addere(s->munera) = _textus(n, l->piscina);
        }
        alioquin si (_titulus_est(n, "patrocinium"))
        {
            *(chorda*)xar_addere(s->patrocinia) = _textus(n,
                l->piscina);
        }
        alioquin si (_titulus_est(n, "socius"))
        {
            SanctiSocius* so = (SanctiSocius*)xar_addere(s->socii);

            si (so == NIHIL)
            {
                redde;
            }
            so->textus  = _textus(n, l->piscina);
            so->ad      = _attributum(n, "ad");
        }
        alioquin si (_titulus_est(n, "descriptio"))
        {
            per (j = ZEPHYRUM; j < stml_numerus_liberorum(n); j++)
            {
                StmlNodus* p = stml_liberum_ad_indicem(n, j);

                si (_titulus_est(p, "paragraphus"))
                {
                    _fontes_probare(l, p);
                    *(chorda*)xar_addere(s->paragraphi) = _textus(p,
                        l->piscina);
                }
            }
        }
    }
    si (principalia != I)
    {
        sprintf(nuntius, "nomen anglicum principale: %u (unum "
            "postulatur)", (insignatus integer)principalia);
        _vitium(l, radix, nuntius);
    }
    si (   s->ortus.notus && s->obitus.notus
        && s->ortus.annus != ZEPHYRUM && s->obitus.annus != ZEPHYRUM
        && s->ortus.annus > s->obitus.annus)
    {
        sprintf(nuntius, "ortus (%d) post obitum (%d)",
            (integer)s->ortus.annus, (integer)s->obitus.annus);
        _vitium(l, radix, nuntius);
    }
}

interior Sanctus*
_legere (
                 chorda  textus,
                 chorda  via,
                  Canon* canon,
                Piscina* piscina,
    InternamentumChorda* intern,
                    Xar* vitia)
{
     StmlResultus  r;
           Lector  l;
          Sanctus* s;
           chorda  basis;
        character  nuntius[CCLVI];

    memset(&l, ZEPHYRUM, magnitudo(Lector));
    l.via      = via;
    l.piscina  = piscina;
    l.vitia    = vitia;
    l.fontes   = tabula_dispersa_creare_chorda(piscina, XVI);
    si (textus.datum == NIHIL)
    {
        _vitium(&l, NIHIL, "plagula legi nequit");
        redde NIHIL;
    }
    r = stml_legere(textus, piscina, intern);
    si (!r.successus || r.elementum_radix == NIHIL)
    {
        sprintf(nuntius, "STML fractum: %.*s",
            (integer)(r.error.mensura > CC ? CC : r.error.mensura),
            r.error.datum ? (constans character*)r.error.datum : "");
        _vitium_addere(vitia, via, r.linea_erroris, piscina, nuntius);
        redde NIHIL;
    }
    si (canon == NIHIL)
    {
        _vitium(&l, NIHIL, "canon infixus fractus");
        redde NIHIL;
    }
    si (!_canonem_iudicare(&l, canon, r.radix))
    {
        redde NIHIL;
    }
    s = (Sanctus*)piscina_allocare(piscina, magnitudo(Sanctus));
    si (s == NIHIL || l.fontes == NIHIL)
    {
        redde NIHIL;
    }
    memset(s, ZEPHYRUM, magnitudo(Sanctus));
    s->id   = _attributum(r.elementum_radix, "id");
    s->via  = via;
    s->genus       =
        chorda_aequalis_literis(_attributum(r.elementum_radix,
        "genus"), "coetus") ? SANCTI_COETUS : SANCTI_PERSONA;
    s->status      = _status(_attributum(r.elementum_radix, "status"));
    s->nomina      = xar_creare(piscina, (i32)magnitudo(SanctiNomen));
    s->festa       = xar_creare(piscina, (i32)magnitudo(SanctiFestum));
    s->munera      = xar_creare(piscina, (i32)magnitudo(chorda));
    s->patrocinia  = xar_creare(piscina, (i32)magnitudo(chorda));
    s->socii       = xar_creare(piscina, (i32)magnitudo(SanctiSocius));
    s->paragraphi  = xar_creare(piscina, (i32)magnitudo(chorda));
    s->fontes      = xar_creare(piscina, (i32)magnitudo(SanctiFons));
    si (   !s->nomina || !s->festa || !s->munera || !s->patrocinia
        || !s->socii || !s->paragraphi || !s->fontes)
    {
        redde NIHIL;
    }
    basis = _basis(via);
    si (via.mensura > ZEPHYRUM && !chorda_aequalis(basis, s->id))
    {
        sprintf(nuntius, "id '%.*s' a nomine plagulae '%.*s' differt",
            (integer)(s->id.mensura > LXIV ? LXIV : s->id.mensura),
            (constans character*)s->id.datum,
            (integer)(basis.mensura > LXIV ? LXIV : basis.mensura),
            (constans character*)basis.datum);
        _vitium(&l, r.elementum_radix, nuntius);
    }
    _campos_legere(&l, r.elementum_radix, s);
    redde l.fractum ? NIHIL : s;
}

Sanctus*
sancti_legere (
                 chorda  textus,
                 chorda  via,
                Piscina* piscina,
    InternamentumChorda* intern,
                    Xar* vitia)
{
      Canon* canon;
     chorda  causa;

    causa  = _chorda_nulla();
    canon  = canon_legere(sancti_canon_textus(piscina), piscina, intern,
        &causa);
    redde _legere(textus, via, canon, piscina, intern, vitia);
}


/* ==================================================
 * Corpus
 * ================================================== */

nomen structura {
        Xar* viae;      /* chorda: plagulae .sanctus */
    Piscina* piscina;
} Collectio;

interior s32
_colligere (
                           chorda  via_plena,
    constans DirectoriumIntroitus* introitus,
                           vacuum* contextus)
{
    Collectio* c = (Collectio*)contextus;

    si (   introitus->genus == INTROITUS_FILUM
        && introitus->titulus.mensura > (i32)(magnitudo(SUFFIXUM) - I)
        && memcmp(introitus->titulus.datum + introitus->titulus.mensura
               - (magnitudo(SUFFIXUM) - I), SUFFIXUM,
               magnitudo(SUFFIXUM) - I) == ZEPHYRUM)
    {
        chorda* locus = (chorda*)xar_addere(c->viae);

        si (locus != NIHIL)
        {
            *locus = chorda_transcribere(via_plena, c->piscina);
        }
    }
    redde ZEPHYRUM;
}

interior s32
_chordas_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare(*(constans chorda*)a, *(constans chorda*)b);
}

interior s32
_sanctos_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    redde chorda_comparare((*(constans Sanctus* constans*)a)->id,
        (*(constans Sanctus* constans*)b)->id);
}

interior s32
_vitia_comparare (
    constans vacuum* a,
    constans vacuum* b)
{
    constans SanctiVitium* x = (constans SanctiVitium*)a;
    constans SanctiVitium* y = (constans SanctiVitium*)b;
                      s32  c;

    c = chorda_comparare(x->via, y->via);
    si (c != ZEPHYRUM)
    {
        redde c;
    }
    redde (x->linea > y->linea) - (x->linea < y->linea);
}

/* sanctorale.tsv: mensis TAB dies TAB titulus TAB ids */
interior vacuum
_iuncturas_legere (
            SanctiCorpus* corpus,
                  chorda  via,
                 Piscina* piscina)
{
                   chorda textus;
    chorda_fissio_fructus lineae;
                      i32 i;
                      i32 k;
                character nuntius[CCLVI];

    textus = filum_legere_totum(chorda_ut_cstr(via, piscina), piscina);
    si (textus.datum == NIHIL)
    {
        redde;
    }
    lineae = chorda_fissio(textus, '\n', piscina);
    per (i = ZEPHYRUM; i < (i32)lineae.numerus; i++)
    {
                       chorda  linea = lineae.elementa[i];
        chorda_fissio_fructus  campi;
        chorda_fissio_fructus  ids;
                     Iunctura* iu;

        si (linea.mensura == ZEPHYRUM || linea.datum[ZEPHYRUM] == '#')
        {
            perge;
        }
        campi  = chorda_fissio(linea, '\t', piscina);
        iu     = (Iunctura*)xar_addere(corpus->iuncturae);
        si (iu == NIHIL)
        {
            redde;
        }
        si (   campi.numerus != IV
            || !chorda_ut_s32(campi.elementa[ZEPHYRUM], &iu->mensis)
            || !chorda_ut_s32(campi.elementa[I], &iu->dies))
        {
            xar_truncare(corpus->iuncturae,
                xar_numerus(corpus->iuncturae) - I);
            _vitium_addere(corpus->vitia, via, i + I, piscina,
                "linea deformis (mensis TAB dies TAB titulus TAB ids)");
            perge;
        }
        iu->titulus  = campi.elementa[II];
        iu->ids      = xar_creare(piscina, (i32)magnitudo(chorda));
        si (iu->ids == NIHIL)
        {
            redde;
        }
        si (chorda_aequalis_literis(campi.elementa[III], "-"))
        {
            perge;
        }
        ids = chorda_fissio(campi.elementa[III], ' ', piscina);
        per (k = ZEPHYRUM; k < (i32)ids.numerus; k++)
        {
            si (ids.elementa[k].mensura == ZEPHYRUM)
            {
                perge;
            }
            si (!tabula_dispersa_continet(corpus->per_id,
                    ids.elementa[k]))
            {
                sprintf(nuntius, "id ignotum in iunctura: '%.*s'",
                    (integer)(ids.elementa[k].mensura > LXIV ? LXIV
                        : ids.elementa[k].mensura),
                    (constans character*)ids.elementa[k].datum);
                _vitium_addere(corpus->vitia, via, i + I, piscina,
                    nuntius);
                perge;
            }
            *(chorda*)xar_addere(iu->ids) = ids.elementa[k];
        }
    }
}

SanctiCorpus*
sancti_corpus_aperire (
     constans character* directorium,
                Piscina* piscina,
    InternamentumChorda* intern)
{
    SanctiCorpus* corpus;
       Collectio  collectio;
           Canon* canon;
          chorda  causa;
          chorda  iunctura;
             i32  i;
       character  nuntius[CCLVI];

    si (directorium == NIHIL || !directorium_existit(directorium))
    {
        redde NIHIL;
    }
    corpus = (SanctiCorpus*)piscina_allocare(piscina,
        magnitudo(SanctiCorpus));
    si (corpus == NIHIL)
    {
        redde NIHIL;
    }
    corpus->sancti = xar_creare(piscina, (i32)magnitudo(Sanctus*));
    corpus->per_id = tabula_dispersa_creare_chorda(piscina, CCLVI);
    corpus->vitia = xar_creare(piscina,
        (i32)magnitudo(SanctiVitium));
    corpus->iuncturae  = xar_creare(piscina, (i32)magnitudo(Iunctura));
    collectio.viae     = xar_creare(piscina, (i32)magnitudo(chorda));
    collectio.piscina  = piscina;
    si (   !corpus->sancti || !corpus->per_id || !corpus->vitia
        || !corpus->iuncturae || !collectio.viae
        || directorium_ambulare(directorium, NIHIL, _colligere,
               &collectio, piscina) != ZEPHYRUM)
    {
        redde NIHIL;
    }
    xar_ordinare(collectio.viae, _chordas_comparare);
    causa  = _chorda_nulla();
    canon  = canon_legere(sancti_canon_textus(piscina), piscina, intern,
        &causa);
    per (i = ZEPHYRUM; i < xar_numerus(collectio.viae); i++)
    {
         chorda  via = *(chorda*)xar_obtinere(collectio.viae, i);
        Sanctus* s;
         vacuum* prior;

        s = _legere(filum_legere_totum(chorda_ut_cstr(via, piscina),
            piscina), via, canon, piscina, intern, corpus->vitia);
        si (s == NIHIL)
        {
            perge;
        }
        si (tabula_dispersa_invenire(corpus->per_id, s->id, &prior))
        {
            sprintf(nuntius, "id duplex '%.*s' (prior: %.*s)",
                (integer)(s->id.mensura > LXIV ? LXIV : s->id.mensura),
                (constans character*)s->id.datum,
                (integer)(((Sanctus*)prior)->via.mensura > CXXVIII
                    ? CXXVIII : ((Sanctus*)prior)->via.mensura),
                (constans character*)((Sanctus*)prior)->via.datum);
            _vitium_addere(corpus->vitia, via, I, piscina, nuntius);
            perge;
        }
        (vacuum)tabula_dispersa_inserere(corpus->per_id, s->id, s);
        *(Sanctus**)xar_addere(corpus->sancti) = s;
    }
    xar_ordinare(corpus->sancti, _sanctos_comparare);
    iunctura = chorda_concatenare(chorda_ex_literis(directorium,
        piscina),
        chorda_ex_literis("/sanctorale.tsv", piscina), piscina);
    _iuncturas_legere(corpus, iunctura, piscina);
    xar_ordinare(corpus->vitia, _vitia_comparare);
    redde corpus;
}

constans Xar*
sancti_vitia (
    constans SanctiCorpus* corpus)
{
    redde corpus ? corpus->vitia : NIHIL;
}

i32
sancti_numerus (
    constans SanctiCorpus* corpus)
{
    redde corpus ? xar_numerus(corpus->sancti) : ZEPHYRUM;
}

constans Sanctus*
sancti_ad_indicem (
    constans SanctiCorpus* corpus,
                      i32  index)
{
    si (corpus == NIHIL || index >= xar_numerus(corpus->sancti))
    {
        redde NIHIL;
    }
    redde *(Sanctus**)xar_obtinere(corpus->sancti, index);
}

constans Sanctus*
sancti_invenire (
    constans SanctiCorpus* corpus,
                   chorda  id)
{
    vacuum* s;

    si (   corpus == NIHIL
        || !tabula_dispersa_invenire(corpus->per_id, id, &s))
    {
        redde NIHIL;
    }
    redde (constans Sanctus*)s;
}

Xar*
sancti_diei (
    constans SanctiCorpus* corpus,
        SanctiCalendarium  calendarium,
                      s32  mensis,
                      s32  dies,
                  Piscina* piscina)
{
    Xar* fructus;
    i32  i;
    i32  j;

    fructus = xar_creare(piscina, (i32)magnitudo(Sanctus*));
    per (i = ZEPHYRUM; corpus && fructus
         && i < xar_numerus(corpus->sancti); i++)
    {
        constans Sanctus* s = *(Sanctus**)xar_obtinere(corpus->sancti,
            i);

        per (j = ZEPHYRUM; j < xar_numerus(s->festa); j++)
        {
            constans SanctiFestum* f = (constans SanctiFestum*)
                xar_obtinere(s->festa, j);

            si (   f->calendarium == calendarium && f->mensis == mensis
                && f->dies        == dies)
            {
                *(constans Sanctus**)xar_addere(fructus) = s;
                frange;
            }
        }
    }
    redde fructus;
}

Xar*
sancti_quaerere (
    constans SanctiCorpus* corpus,
                   chorda  fragmentum,
                  Piscina* piscina)
{
       Xar* fructus;
    chorda  acus;
       i32  i;
       i32  j;

    fructus  = xar_creare(piscina, (i32)magnitudo(Sanctus*));
    acus     = chorda_minuscula(fragmentum, piscina);
    per (i = ZEPHYRUM; corpus && fructus
         && i < xar_numerus(corpus->sancti); i++)
    {
        constans Sanctus* s = *(Sanctus**)xar_obtinere(corpus->sancti,
            i);

        per (j = ZEPHYRUM; j < xar_numerus(s->nomina); j++)
        {
            constans SanctiNomen* n = (constans SanctiNomen*)
                xar_obtinere(s->nomina, j);

            si (chorda_continet(chorda_minuscula(n->textus, piscina),
                    acus))
            {
                *(constans Sanctus**)xar_addere(fructus) = s;
                frange;
            }
        }
    }
    redde fructus;
}

Xar*
sancti_celebrationis (
    constans SanctiCorpus* corpus,
                      s32  mensis,
                      s32  dies,
                   chorda  titulus,
                  Piscina* piscina)
{
    i32 i;
    i32 k;

    per (i = ZEPHYRUM; corpus
        && i < xar_numerus(corpus->iuncturae); i++)
    {
        constans Iunctura* iu = (constans Iunctura*)xar_obtinere(
            corpus->iuncturae, i);
                      Xar* fructus;

        si (   iu->mensis != mensis || iu->dies != dies
            || !chorda_aequalis(iu->titulus, titulus))
        {
            perge;
        }
        fructus = xar_creare(piscina, (i32)magnitudo(Sanctus*));
        per (k = ZEPHYRUM; fructus && k < xar_numerus(iu->ids); k++)
        {
            constans Sanctus* s = sancti_invenire(corpus,
                *(chorda*)xar_obtinere(iu->ids, k));

            si (s != NIHIL)
            {
                *(constans Sanctus**)xar_addere(fructus) = s;
            }
        }
        redde fructus;
    }
    redde NIHIL;
}

/* "c. " si circa; annus ante Christum "N BC" */
interior vacuum
_annum_appendere (
    ChordaAedificator* a,
                  s32  annus,
                  b32  circa)
{
    character numerus[XXIV];

    si (circa)
    {
        chorda_aedificator_appendere_literis(a, "c. ");
    }
    si (annus < ZEPHYRUM)
    {
        sprintf(numerus, "%d BC", (integer)-annus);
    }
    alioquin
    {
        sprintf(numerus, "%d", (integer)annus);
    }
    chorda_aedificator_appendere_literis(a, numerus);
}

/* suffixum ordinale anglicum: 1st 2nd 3rd 4th ... 11th 12th 13th
 * 21st */
interior constans character*
_ordinale (
    s32 n)
{
    si (n % C >= XI && n % C <= XIII)
    {
        redde "th";
    }
    commutatio (n % X)
    {
        casus I:
            redde "st";
        casus II:
            redde "nd";
        casus III:
            redde "rd";
        ordinarius:
            redde "th";
    }
}

chorda
sancti_aetas_textus (
    constans Sanctus* s,
             Piscina* piscina)
{
        ChordaAedificator* a;
                      b32  ortus;
                      b32  obitus;
      constans SanctiDies* d;
                character  numerus[XLVIII];

    a       = chorda_aedificator_creare(piscina, LXIV);
    ortus   = s->ortus.notus && s->ortus.annus != ZEPHYRUM;
    obitus  = s->obitus.notus && s->obitus.annus != ZEPHYRUM;
    si (a == NIHIL)
    {
        redde _chorda_nulla();
    }
    si (ortus && obitus)
    {
        _annum_appendere(a, s->ortus.annus, s->ortus.circa);
        chorda_aedificator_appendere_literis(a, "\342\200\223");
        _annum_appendere(a, s->obitus.annus, s->obitus.circa);
    }
    alioquin si (obitus)
    {
        chorda_aedificator_appendere_literis(a, "d. ");
        _annum_appendere(a, s->obitus.annus, s->obitus.circa);
    }
    alioquin si (ortus)
    {
        chorda_aedificator_appendere_literis(a, "b. ");
        _annum_appendere(a, s->ortus.annus, s->ortus.circa);
    }
    alioquin
    {
        d = (s->obitus.notus && s->obitus.saeculum != ZEPHYRUM)
            ? &s->obitus
            : (s->ortus.notus && s->ortus.saeculum != ZEPHYRUM)
            ? &s->ortus : NIHIL;
        si (d == NIHIL)
        {
            redde _chorda_nulla();
        }
        si (d->circa)
        {
            chorda_aedificator_appendere_literis(a, "c. ");
        }
        sprintf(numerus, "%d%s century%s",
            (integer)(d->saeculum
                < ZEPHYRUM ? -d->saeculum : d->saeculum),
            _ordinale(d->saeculum
                < ZEPHYRUM ? -d->saeculum : d->saeculum),
            d->saeculum < ZEPHYRUM ? " BC" : "");
        chorda_aedificator_appendere_literis(a, numerus);
    }
    redde chorda_aedificator_finire(a);
}

constans SanctiFestum*
sancti_festum_primum (
    constans Sanctus* s)
{
    hic_manens constans SanctiCalendarium ordo[III] = {
        SANCTI_CALENDARIUM_GENERALE, SANCTI_CALENDARIUM_USA,
        SANCTI_CALENDARIUM_MARTYROLOGIUM
    };
    i32 k;
    i32 j;

    per (k = ZEPHYRUM; k < III; k++)
    {
        per (j = ZEPHYRUM; s && j < xar_numerus(s->festa); j++)
        {
            constans SanctiFestum* f = (constans SanctiFestum*)
                xar_obtinere(s->festa, j);

            si (f->calendarium == ordo[k])
            {
                redde f;
            }
        }
    }
    redde NIHIL;
}
