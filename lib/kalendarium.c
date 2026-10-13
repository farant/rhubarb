/* kalendarium.c - calendarium, pagina mensis (include/kalendarium.h;
 * agenda-plan A2a). Scriptura status per portam rami, scriptore
 * calendarium.agere (vocantibus extra dispensatorem scriptor ponitur et
 * restituitur - aliter domini scripturam tacite recusant). */

#include "kalendarium.h"
#include "calendarium_liturgicum.h"
#include "color.h"
#include "mandatum.h"
#include "thema.h"

#include <stdio.h>
#include <string.h>

hic_manens constans character actio_calendarii[] = "calendarium.agere";

#define KALENDARIUM_TITULI_ALTITUDO  XVI
#define KALENDARIUM_LATITUDO         CCLXXX   /* superficies absens */
#define KALENDARIUM_ALTITUDO         CCLVI
#define KALENDARIUM_LITTERA          VI       /* latitudo glyphi */
#define KALENDARIUM_LINEA            VIII     /* altitudo glyphi */
#define KALENDARIUM_SIGNA_MAXIMA     IV
#define KALENDARIUM_TITULI_DIEI      XXVIII   /* lineae III + margo */

/* contextus figurarum (privatus; figurae_ctx): liturgia semel creata */
nomen structura {
    KalendariumMontatio* m;
  CalendariumLiturgicum* liturgia;
                Piscina* scheda;     /* A3: actio una (vacatur) */
} KalendariumFigurae;


/* ==================================================
 * Auxilia
 * ================================================== */

interior Dies
hodie (
    constans KalendariumMontatio* m)
{
    Dies d;

    si (m->hodie && m->hodie(m->hodie_ctx, &d) && fasti_dies_valida(d))
    {
        redde d;
    }
    redde fasti_dies(MMXXVI, I, I);
}

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_ramus_attributum(ramus, titulus);
    si (a && chorda_ut_s32(*a, &v) && v > ZEPHYRUM)
    {
        redde v;
    }
    redde praestitutum;
}

/* "YYYY-MM" -> annus, mensis; FALSUM si malum */
interior b32
mensem_legere (
    constans character* argumentum,
                   s32* annus,
                   s32* mensis)
{
    i32 i;

    si (   !argumentum || strlen(argumentum) != VII
        || argumentum[IV] != '-')
    {
        redde FALSUM;
    }
    per (i = ZEPHYRUM; i < VII; i++)
    {
        si (i != IV && (argumentum[i] < '0' || argumentum[i] > '9'))
        {
            redde FALSUM;
        }
    }
    *annus   = (s32)((argumentum[ZEPHYRUM] - '0') * M
             + (argumentum[I] - '0') * C + (argumentum[II] - '0') * X
             + (argumentum[III] - '0'));
    *mensis  = (s32)((argumentum[V] - '0') * X + (argumentum[VI]
        - '0'));
    redde *mensis >= I && *mensis <= XII;
}

/* dies argumenti: hodie si in eo mense, aliter dies I */
interior Dies
dies_argumenti (
    constans KalendariumMontatio* m,
                             s32  annus,
                             s32  mensis)
{
    Dies h;

    h = hodie(m);
    si (h.annus == annus && h.mensis == mensis)
    {
        redde h;
    }
    redde fasti_dies(annus, mensis, I);
}

/* par campi pro mutatore */
nomen structura {
    constans character* titulus;
    constans character* valor;
} ParCampi;

interior vacuum
campum_ponere (
              StmlNodus* radix,
                Piscina* p,
    InternamentumChorda* in,
                 vacuum* ctx)
{
    ParCampi* par;

    par = (ParCampi*)ctx;
    insula_attributum_ponere(radix, p, in, par->titulus, par->valor);
}

/* campum status scribere, scriptore calendarii (prior restituitur -
 * vocantes extra dispensatorem aliter a dominis tacite recusantur) */
interior b32
campum_scribere (
    KalendariumMontatio* m,
     constans character* titulus,
     constans character* valor)
{
    ParCampi par;
      chorda prior;
         b32 ok;

    par.titulus  = titulus;
    par.valor    = valor;
    prior        = m->ramus.repo->scriptor;
    insula_scriptorem_ponere(m->ramus.repo, chorda_ex_literis(
        actio_calendarii, m->piscina));
    ok = mutare_ramum(&m->ramus, campum_ponere, &par);
    insula_scriptorem_ponere(m->ramus.repo, prior);
    redde ok;
}

/* dies_electus scribere */
interior b32
diem_scribere (
    KalendariumMontatio* m,
                   Dies  dies)
{
    character iso[XVI];
       chorda textus;

    textus = fasti_formare_diem(dies, FASTI_FORMA_ISO, m->piscina);
    si (textus.mensura <= ZEPHYRUM || textus.mensura >= XVI)
    {
        redde FALSUM;
    }
    memcpy(iso, textus.datum, (size_t)textus.mensura);
    iso[textus.mensura] = '\0';
    redde campum_scribere(m, "dies_electus", iso);
}

interior b32
visus_diei (
    constans KalendariumMontatio* m)
{
    chorda* a;

    a = insula_ramus_attributum(&m->ramus, "visus");
    redde a && chorda_aequalis_literis(*a, "dies");
}

interior b32
dies_aequales (
    Dies a,
    Dies b)
{
    redde a.annus == b.annus && a.mensis == b.mensis
        && a.dies == b.dies;
}

/* ordines visus diei: res diei assignatae, deinde res eo die perfectae
 * sed alibi assignatae (Xar de constans AgendaRes*) */
interior Xar*
ordines_diei (
     constans KalendariumMontatio* m,
                             Dies  dies,
                          Piscina* piscina)
{
                   Xar* l;
                   Xar* perfectae;
    constans AgendaRes* r;
                   i32  i;

    si (!m->documentum)
    {
        redde xar_creare(piscina, (i32)magnitudo(constans AgendaRes*));
    }
    l          = agenda_res_diei(m->documentum, dies, piscina);
    perfectae  = agenda_perfectae_die(m->documentum, dies, piscina);
    per (i = ZEPHYRUM; l && perfectae
        && i < xar_numerus(perfectae); i++)
    {
        r = *(constans AgendaRes**)xar_obtinere(perfectae, i);
        si (!r->dies_posita || !dies_aequales(r->dies, dies))
        {
            *(constans AgendaRes**)xar_addere(l) = r;
        }
    }
    redde l;
}

interior i32
index_rei (
    Xar* l,
    s32  id)
{
    i32 i;

    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        si ((*(constans AgendaRes**)xar_obtinere(l, i))->id == id)
        {
            redde (i32)i;
        }
    }
    redde (i32)-I;
}

/* res electa: campus res_electa si in ordinibus, aliter prima; 0 si
 * nulla */
interior s32
res_electa (
    constans KalendariumMontatio* m,
                             Xar* l)
{
    chorda* a;
       s32  id;

    a = insula_ramus_attributum(&m->ramus, "res_electa");
    si (a && chorda_ut_s32(*a, &id) && index_rei(l, id) != (i32)-I)
    {
        redde id;
    }
    redde (l && xar_numerus(l) > ZEPHYRUM)
        ? (*(constans AgendaRes**)xar_obtinere(l, ZEPHYRUM))->id
        : ZEPHYRUM;
}

interior b32
electionem_scribere (
    KalendariumMontatio* m,
                    s32  id)
{
    character numerus[XVI];

    sprintf(numerus, "%d", (integer)id);
    redde campum_scribere(m, "res_electa", numerus);
}

/* post rem e ordinibus remotam: electio ad indicem eundem (aut
 * ultimum) ordinum novorum */
interior vacuum
electionem_reficere (
    KalendariumMontatio* m,
                   Dies  dies,
                    i32  index,
                Piscina* p)
{
    Xar* l;
    i32  n;

    l = ordines_diei(m, dies, p);
    n = l ? xar_numerus(l) : ZEPHYRUM;
    si (n > ZEPHYRUM)
    {
        (vacuum)electionem_scribere(m, (*(constans AgendaRes**)
            xar_obtinere(l, index < n ? index : n - I))->id);
    }
}


/* ==================================================
 * Actio
 * ================================================== */

/* visus mensis: claves (h/l/j/k/H/L/t, Enter) et ictus in cellula
 * (id = dies ISO) */
interior b32
agere_mensem (
    KalendariumMontatio* m,
              Componens* nodus,
       constans Eventus* ev)
{
    Dies d;

    si (ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        si (   nodus && fasti_parsare_iso(nodus->id, &d)
            && fasti_dies_valida(d))
        {
            redde diem_scribere(m, d);
        }
        redde FALSUM;
    }
    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    si (ev->datum.clavis.clavis == CLAVIS_REDITUS)
    {
        redde campum_scribere(m, "visus", "dies");
    }
    d = kalendarium_dies_electus(m);
    commutatio (ev->datum.clavis.producta)
    {
        casus 'h': d = fasti_addere_dies(d, -I);              frange;
        casus 'l': d = fasti_addere_dies(d, I);               frange;
        casus 'j': d = fasti_addere_dies(d, VII);             frange;
        casus 'k': d = fasti_addere_dies(d, -VII);            frange;
        casus 'H': d = fasti_addere_menses(d, -I);            frange;
        casus 'L': d = fasti_addere_menses(d, I);             frange;
        casus 't': d = hodie(m);                              frange;
        ordinarius: redde FALSUM;
    }
    redde diem_scribere(m, d);
}

/* visus diei: electio, perfectio, translatio, deletio, historia
 * agendae; ictus in ordine ("r<id>") eligit */
interior b32
agere_diem (
    KalendariumMontatio* m,
              Componens* nodus,
       constans Eventus* ev,
                Piscina* p)
{
                    Xar* l;
     constans AgendaRes* r;
                   Dies  d;
                    s32  id;
                    i32  index;

    si (ev->genus == EVENTUS_MUS_DEPRESSUS)
    {
        si (   nodus && nodus->id.mensura > I
            && nodus->id.datum[ZEPHYRUM] == 'r')
        {
            chorda numerus;

            numerus.datum    = nodus->id.datum + I;
            numerus.mensura  = nodus->id.mensura - I;
            si (chorda_ut_s32(numerus, &id))
            {
                redde electionem_scribere(m, id);
            }
        }
        redde FALSUM;
    }
    si (ev->genus != EVENTUS_CLAVIS_DEPRESSUS)
    {
        redde FALSUM;
    }
    si (ev->datum.clavis.clavis == CLAVIS_EFFUGIUM)
    {
        redde campum_scribere(m, "visus", "mensis");
    }
    si (   (ev->datum.clavis.modificantes & (i32)MOD_IMPERIUM)
        && ev->datum.clavis.runa == 'r')
    {
        (vacuum)agenda_reficere(m->documentum);
        redde VERUM;
    }
    d      = kalendarium_dies_electus(m);
    l      = ordines_diei(m, d, p);
    id     = res_electa(m, l);
    index  = index_rei(l, id);
    r      = (m->documentum && id > ZEPHYRUM)
           ? agenda_res(m->documentum, id) : NIHIL;
    commutatio (ev->datum.clavis.producta)
    {
        casus 'j':
        casus 'k':
            si (index != (i32)-I)
            {
                si (   ev->datum.clavis.producta == 'j'
                    && index + I < xar_numerus(l))
                {
                    index++;
                }
                alioquin si (   ev->datum.clavis.producta == 'k'
                             && index > ZEPHYRUM)
                {
                    index--;
                }
                redde electionem_scribere(m, (*(constans AgendaRes**)
                    xar_obtinere(l, index))->id);
            }
            redde VERUM;
        casus 'h': redde diem_scribere(m, fasti_addere_dies(d, -I));
        casus 'l': redde diem_scribere(m, fasti_addere_dies(d, I));
        casus 'u':
            (vacuum)agenda_revocare(m->documentum);
            redde VERUM;
        casus 'x':
            si (r)
            {
                (vacuum)(r->perfecta
                    ? agenda_perfectionem_tollere(m->documentum, id)
                    : agenda_perficere(m->documentum, id, hodie(m)));
            }
            redde VERUM;
        casus '<':
        casus '>':
            si (r)
            {
                Dies novus;

                novus = fasti_addere_dies(r->dies_posita ? r->dies : d,
                    ev->datum.clavis.producta == '>' ? I : -I);
                (vacuum)agenda_diem_ponere(m->documentum, id, &novus);
                electionem_reficere(m, d, (i32)index, p);
            }
            redde VERUM;
        casus 'X':
            si (r)
            {
                (vacuum)agenda_rem_delere(m->documentum, id);
                electionem_reficere(m, d, (i32)index, p);
            }
            redde VERUM;
        ordinarius:
            redde FALSUM;
    }
}

/* <actio/> calendarium.agere: visus mensis aut diei */
interior b32
agere (
     InsulaRepositorium* repo,
                  Motus* motus,
    constans Destinatio* des,
              Componens* nodus,
       constans Eventus* ev,
                 vacuum* ctx)
{
    KalendariumMontatio* m;
     KalendariumFigurae* privatum;

    (vacuum)repo;
    (vacuum)motus;
    (vacuum)des;
    m         = (KalendariumMontatio*)ctx;
    privatum  = (KalendariumFigurae*)m->figurae_ctx;
    si (!visus_diei(m))
    {
        redde agere_mensem(m, nodus, ev);
    }
    si (!privatum || !privatum->scheda)
    {
        redde FALSUM;
    }
    piscina_vacare(privatum->scheda);
    redde agere_diem(m, nodus, ev, privatum->scheda);
}


/* ==================================================
 * Figurae (A2b)
 * ================================================== */

hic_manens constans character* constans menses_anglice[XII] = {
    "January", "February", "March", "April", "May", "June", "July",
    "August", "September", "October", "November", "December"
};

interior ColorMandati
color_thematis (
    i32 signum)
{
    ColorMandati c;

    c.genus = COLOR_MANDATI_THEMA;
    c.valor = signum;
    redde c;
}

/* colores liturgici RGB, ordine ColorLiturgicus (albus aureus: in fundo
 * clarissimo videtur) */
hic_manens constans i32 colores_liturgici[VI][III] = {
    { CCXXX, CCV, CXX },     /* albus */
    { CXC, XL, XL },         /* ruber */
    { LX, CXL, LXX },        /* viridis */
    { CXX, LX, CL },         /* violaceus */
    { CCXXX, CXL, CLXX },    /* rosaceus */
    { XXX, XXX, XXX }        /* niger */
};

interior ColorMandati
color_liturgicus (
    ColorLiturgicus color)
{
    ColorMandati c;
             i32 k;

    k        = ((i32)color < VI) ? (i32)color : ZEPHYRUM;
    c.genus  = COLOR_MANDATI_RGBA;
    c.valor  = color_ad_pixelum(color_ex_rgba(
        (i8)colores_liturgici[k][ZEPHYRUM], (i8)colores_liturgici[k][I],
        (i8)colores_liturgici[k][II], (i8)CCLV));
    redde c;
}

/* textus ad latitudinem (pixela) praecisus */
interior chorda
praecidere (
    chorda textus,
       s32 latitudo)
{
    s32 n;

    n = latitudo / KALENDARIUM_LITTERA;
    si (n < ZEPHYRUM)
    {
        n = ZEPHYRUM;
    }
    si (textus.mensura > (i32)n)
    {
        textus.mensura = (i32)n;
    }
    redde textus;
}

interior vacuum
rectangulum (
         Mandata* md,
             s32  x,
             s32  y,
             s32  latitudo,
             s32  altitudo,
    ColorMandati  color,
             b32  impletum)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    mandata_rectangulum(md, f, color, impletum);
}

/* signa (apertae, perfectae) diei ex signis mensium praesentium */
nomen structura {
             s32 annus;
             s32 mensis;
    AgendaSignum signa[XXXI];
} SignaMensis;

interior constans AgendaSignum*
signum_diei (
    constans SignaMensis* menses,
                     i32  numerus,
                    Dies  dies)
{
    i32 i;

    per (i = ZEPHYRUM; i < numerus; i++)
    {
        si (   menses[i].annus  == dies.annus
            && menses[i].mensis == dies.mensis)
        {
            redde &menses[i].signa[dies.dies - I];
        }
    }
    redde NIHIL;
}

/* cellula una (in spatio tabulae; cellulae liberi tabulae in 0,0) */
interior vacuum
cellulam_pingere (
       KalendariumFigurae* contextus,
                  Mandata* md,
                    Fines  f,
                     Dies  dies,
                     Dies  electa,
                     Dies  hodierna,
    constans AgendaSignum* signum)
{
     InformatioDiei* info;
          character  numerus[IV];
             chorda  textus;
                b32  alienus;
                s32  x;
                i32  k;
                i32  apertae;
                i32  perfectae;

    alienus = dies.mensis != electa.mensis
        || dies.annus != electa.annus;
    info = NIHIL;
    si (contextus->liturgia)
    {
        info = calendarium_obtinere_diem(contextus->liturgia, dies,
            md->piscina);
    }
    /* numerus diei: hodie inversus, alieni obscuri */
    numerus[ZEPHYRUM]  = (character)('0' + dies.dies / X);
    numerus[I]         = (character)('0' + dies.dies % X);
    numerus[II]        = '\0';
    textus = chorda_ex_literis(dies.dies >= X ? numerus : numerus + I,
        md->piscina);
    si (dies_aequales(dies, hodierna))
    {
        rectangulum(md, f.x + III, f.y + I, (s32)textus.mensura
            * KALENDARIUM_LITTERA + II, KALENDARIUM_LINEA + I,
            color_thematis(COLOR_ACCENT_PRIMARY), VERUM);
        mandata_textus(md, f.x + IV, f.y + I, textus, ZEPHYRUM,
            color_thematis(COLOR_BACKGROUND));
    }
    alioquin
    {
        mandata_textus(md, f.x + IV, f.y + I, textus, ZEPHYRUM,
            color_thematis(alienus ? COLOR_TEXT_DIM : COLOR_TEXT));
    }
    /* color liturgicus: quadratum iuxta numerum (Franus: linea ad
     * marginem ut divisor legebatur) */
    si (info)
    {
        rectangulum(md, f.x + V + (s32)textus.mensura
            * KALENDARIUM_LITTERA + II, f.y + II, V, V,
            color_liturgicus(info->color_diei), VERUM);
    }
    /* festum: memoria aut maius; dominica nuda nihil (Franus: color et
     * columna dicunt - titulus praecisus "Dominica X" fallebat) */
    si (   info && info->celebratio_principalis
        && info->celebratio_principalis->gradus >= GRADUS_MEMORIA
        && info->celebratio_principalis->gradus != GRADUS_DOMINICA)
    {
        mandata_textus(md, f.x + IV, f.y + KALENDARIUM_LINEA + III,
            praecidere(info->celebratio_principalis->titulus_brevis,
            f.latitudo - VI), ZEPHYRUM, color_thematis(alienus
            ? COLOR_TEXT_DIM : COLOR_TEXT));
    }
    /* signa: plena = apertae, vacua = perfectae; IV summum, '+' */
    apertae    = signum ? signum->apertae : ZEPHYRUM;
    perfectae  = signum ? signum->perfectae : ZEPHYRUM;
    x          = f.x + IV;
    per (k = ZEPHYRUM; k < apertae + perfectae; k++)
    {
        si (k == KALENDARIUM_SIGNA_MAXIMA)
        {
            mandata_textus(md, x, f.y + f.altitudo - KALENDARIUM_LINEA
                - I, chorda_ex_literis("+", md->piscina), ZEPHYRUM,
                color_thematis(COLOR_TEXT_DIM));
            frange;
        }
        rectangulum(md, x, f.y + f.altitudo - V, III, III,
            color_thematis(k < apertae ? COLOR_ACCENT_PRIMARY
                                       : COLOR_SUCCESS), k < apertae);
        x += V;
    }
    si (dies_aequales(dies, electa))
    {
        rectangulum(md, f.x, f.y, f.latitudo, f.altitudo,
            color_thematis(COLOR_BORDER_ACTIVE), FALSUM);
    }
}

/* <figura/> <purus/> tabula: fundus et cellulae omnes (liberi
 * PARTES_ITEM quorum id = dies ISO); dies liturgici et signa semel per
 * quadrum */
interior vacuum
figura_tabulae (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
    KalendariumFigurae* contextus;
           SignaMensis  menses[III];
                   i32  numerus;
                   i32  i;
                  Dies  electa;
                  Dies  hodierna;
                  Dies  dies;
             Componens* l;

    (vacuum)thema;
    contextus = (KalendariumFigurae*)ctx;
    rectangulum(md, ZEPHYRUM, ZEPHYRUM, c->fines.latitudo,
        c->fines.altitudo, color_thematis(COLOR_BACKGROUND), VERUM);
    si (!contextus || !contextus->m)
    {
        redde;
    }
    electa    = kalendarium_dies_electus(contextus->m);
    hodierna  = hodie(contextus->m);
    /* signa mensium cellularum (III summum: prior, electus, sequens) */
    numerus = ZEPHYRUM;
    per (i = ZEPHYRUM; contextus->m->documentum
        && i < componens_numerus_liberorum(c); i++)
    {
        l = componens_liberum(c, i);
        si (   l->partes != PARTES_ITEM
            || !fasti_parsare_iso(l->id, &dies)
            || signum_diei(menses, numerus, dies) || numerus >= III)
        {
            perge;
        }
        menses[numerus].annus   = dies.annus;
        menses[numerus].mensis  = dies.mensis;
        si (agenda_signa_mensis(contextus->m->documentum, dies.annus,
                dies.mensis, menses[numerus].signa))
        {
            numerus++;
        }
    }
    per (i = ZEPHYRUM; i < componens_numerus_liberorum(c); i++)
    {
        l = componens_liberum(c, i);
        si (l->partes == PARTES_ITEM && fasti_parsare_iso(l->id, &dies))
        {
            cellulam_pingere(contextus, md, l->fines, dies, electa,
                hodierna,
                signum_diei(menses, numerus, dies));
        }
    }
}

/* <figura/> <purus/> titulus: "October 2026 · tempus", deinde litterae
 * dierum sub columnis */
interior vacuum
figura_tituli (
    constans Componens* c,
               Mandata* md,
                   i32  thema,
                vacuum* ctx)
{
                        character  litterae[VIII];
               KalendariumFigurae* contextus;
                   InformatioDiei* info;
                             Dies  electa;
                        character  titulus[LXIV];
                           chorda  textus;
                              s32  columna;
                              i32  j;

    (vacuum)thema;
    contextus = (KalendariumFigurae*)ctx;
    si (!contextus || !contextus->m)
    {
        redde;
    }
    electa = kalendarium_dies_electus(contextus->m);
    sprintf(titulus, "%s %d", menses_anglice[electa.mensis - I],
        (integer)electa.annus);
    textus  = chorda_ex_literis(titulus, md->piscina);
    info    = NIHIL;
    si (contextus->liturgia)
    {
        info = calendarium_obtinere_diem(contextus->liturgia, electa,
            md->piscina);
    }
    si (info)
    {
        textus = chorda_concatenare(textus, chorda_ex_literis(" - ",
            md->piscina), md->piscina);
        textus = chorda_concatenare(textus, calendarium_nomen_temporis(
            info->tempus_info.tempus, FALSUM, md->piscina),
            md->piscina);
    }
    mandata_textus(md, IV, ZEPHYRUM, praecidere(textus,
        c->fines.latitudo - VIII), ZEPHYRUM,
        color_thematis(COLOR_TEXT));
    columna = c->fines.latitudo / VII;
    strcpy(litterae, "SMTWTFS");
    per (j = ZEPHYRUM; j < VII; j++)
    {
        /* mandata textum per indicem servant: littera in memoriam
         * mandatorum transcribitur (acervus figurae post reditum
         * perit) */
        textus.datum    = (i8*)(litterae + j);
        textus.mensura  = I;
        textus          = chorda_transcribere(textus, md->piscina);
        mandata_textus(md, (s32)j * columna + (columna
            - KALENDARIUM_LITTERA) / II, KALENDARIUM_LINEA, textus,
            ZEPHYRUM, color_thematis(COLOR_TEXT_DIM));
    }
}

interior b32
figuras_registrare (
    KalendariumMontatio* m)
{
    KalendariumFigurae* contextus;

    contextus = (KalendariumFigurae*)piscina_allocare(m->piscina,
        magnitudo(KalendariumFigurae));
    si (!contextus)
    {
        redde FALSUM;
    }
    contextus->m         = m;
    contextus->liturgia  = calendarium_creare(m->piscina);
    contextus->scheda    = piscina_generare_dynamicum(
        "kalendarium_scheda", XVI * M);
    m->figurae_ctx       = contextus;
    redde figura_registrare(m->figurae, PARTES_TABULA, ZEPHYRUM,
               figura_tabulae, contextus)
        && figura_registrare(m->figurae, PARTES_TITULUS, ZEPHYRUM,
               figura_tituli, contextus);
}


/* ==================================================
 * Montatio
 * ================================================== */

b32
kalendarium_montare (
     KalendariumMontatio* m,
                 Piscina* piscina,
     InternamentumChorda* intern,
      InsulaRepositorium* repo,
      constans character* id,
      constans character* argumentum,
        AgendaDocumentum* documentum,
        KalendariumHodie  hodie_porta,
                  vacuum* hodie_ctx,
         StatusSystemata* systemata)
{
    Organum* organum;
        s32  annus;
        s32  mensis;

    si (!m || !piscina || !intern || !repo)
    {
        redde FALSUM;
    }
    si (argumentum && !mensem_legere(argumentum, &annus, &mensis))
    {
        redde FALSUM;
    }
    memset(m, ZEPHYRUM, magnitudo(KalendariumMontatio));
    m->piscina     = piscina;
    m->intern      = intern;
    m->documentum  = documentum;
    m->hodie       = hodie_porta;
    m->hodie_ctx   = hodie_ctx;
    m->ramus       = id ? insula_ramus(repo, "calendarium", id)
                        : insula_ramus_radix(repo);
    organum = organum_legere_genus(piscina, intern, "calendarium",
        "kalendarium");
    si (   !organum
        || !organum_ramum_parare(organum, &m->ramus, systemata,
               " focus=\"kalendarium\"", id ? id : "calendarium",
               piscina, intern))
    {
        redde FALSUM;
    }
    m->actiones  = actio_registrum_creare(piscina, intern);
    m->figurae   = figura_registrum_creare(piscina);
    si (   !m->actiones || !m->figurae
        || !actio_registrare(m->actiones, actio_calendarii, agere, m)
        || !figuras_registrare(m))
    {
        redde FALSUM;
    }
    si (argumentum)
    {
        redde diem_scribere(m, dies_argumenti(m, annus, mensis));
    }
    redde VERUM;
}

b32
kalendarium_argumentum_ponere (
    KalendariumMontatio* m,
     constans character* argumentum)
{
    s32 annus;
    s32 mensis;

    si (!m || !mensem_legere(argumentum, &annus, &mensis))
    {
        redde FALSUM;
    }
    redde diem_scribere(m, dies_argumenti(m, annus, mensis));
}

Dies
kalendarium_dies_electus (
    constans KalendariumMontatio* m)
{
     chorda* a;
       Dies  d;

    a = insula_ramus_attributum(&m->ramus, "dies_electus");
    si (a && fasti_parsare_iso(*a, &d) && fasti_dies_valida(d))
    {
        redde d;
    }
    redde hodie(m);
}


/* ==================================================
 * Componere
 * ================================================== */

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                    s32  x,
                    s32  y,
                    s32  latitudo,
                    s32  altitudo)
{
    Componens* c;
        Fines  f;

    c = componens_creare(p, in, id, partes);
    si (!c)
    {
        redde NIHIL;
    }
    f.x         = x;
    f.y         = y;
    f.latitudo  = latitudo;
    f.altitudo  = altitudo;
    componens_ponere_fines(c, f);
    redde c;
}

/* visus diei: titulus (lineae III) + ordo per rem ("r<id>"), altitudine
 * linea tituli et linea per notam */
interior vacuum
diem_componere (
    KalendariumMontatio* m,
              Componens* radix,
                Piscina* piscina,
    InternamentumChorda* intern,
                    s32  latitudo)
{
                    Xar* l;
     constans AgendaRes* r;
              Componens* ordo;
              character  id[XVI];
                    s32  y;
                    s32  altitudo;
                    i32  i;

    componens_addere_liberum(radix, nodus(piscina, intern, "titulus",
        PARTES_TITULUS, ZEPHYRUM, ZEPHYRUM, latitudo,
        KALENDARIUM_TITULI_DIEI));
    l = ordines_diei(m, kalendarium_dies_electus(m), piscina);
    y = KALENDARIUM_TITULI_DIEI;
    per (i = ZEPHYRUM; l && i < xar_numerus(l); i++)
    {
        r         = *(constans AgendaRes**)xar_obtinere(l, i);
        altitudo  = (s32)(I + xar_numerus(r->notae)) * KALENDARIUM_LINEA
                  + II;
        sprintf(id, "r%d", (integer)r->id);
        ordo = nodus(piscina, intern, id, PARTES_ITEM, ZEPHYRUM, y,
            latitudo, altitudo);
        si (ordo)
        {
            componens_ponere_actio(ordo, actio_calendarii);
            componens_addere_liberum(radix, ordo);
        }
        y += altitudo;
    }
}

/* <componens/> <purus/> */
Componens*
kalendarium_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
    KalendariumMontatio* m;
              Componens* radix;
              Componens* cellula;
                   Dies  electus;
                   Dies  initium;
                   Dies  dies;
                    s32  latitudo;
                    s32  altitudo;
                    s32  lat_cellulae;
                    s32  alt_cellulae;
                    i32  k;

    (vacuum)repo;
    (vacuum)motus;
    m = (KalendariumMontatio*)ctx;
    si (!m)
    {
        redde NIHIL;
    }
    latitudo  = attributum_s32(&m->ramus, "superficies_latitudo",
        KALENDARIUM_LATITUDO);
    altitudo  = attributum_s32(&m->ramus, "superficies_altitudo",
        KALENDARIUM_ALTITUDO);
    radix = nodus(piscina, intern, "kalendarium", PARTES_TABULA,
        ZEPHYRUM, ZEPHYRUM, latitudo, altitudo);
    si (!radix)
    {
        redde NIHIL;
    }
    componens_ponere_focusabilis(radix, VERUM);
    componens_ponere_actio(radix, actio_calendarii);
    si (visus_diei(m))
    {
        diem_componere(m, radix, piscina, intern, latitudo);
        redde radix;
    }
    componens_addere_liberum(radix, nodus(piscina, intern, "titulus",
        PARTES_TITULUS, ZEPHYRUM, ZEPHYRUM, latitudo,
        KALENDARIUM_TITULI_ALTITUDO));
    /* VI hebdomades x VII, dominica prima ante aut in die I */
    electus = kalendarium_dies_electus(m);
    initium = fasti_dies(electus.annus, electus.mensis, I);
    initium       = fasti_addere_dies(initium,
        -fasti_dies_hebdomadis(initium));
    lat_cellulae = latitudo / VII;
    alt_cellulae = (altitudo - KALENDARIUM_TITULI_ALTITUDO) / VI;
    per (k = ZEPHYRUM; k < XLII; k++)
    {
        dies     = fasti_addere_dies(initium, (s32)k);
        cellula  = nodus(piscina, intern, chorda_ut_cstr(
            fasti_formare_diem(dies, FASTI_FORMA_ISO, piscina),
            piscina), PARTES_ITEM,
            (s32)(k % VII) * lat_cellulae, KALENDARIUM_TITULI_ALTITUDO
            + (s32)(k / VII) * alt_cellulae, lat_cellulae,
            alt_cellulae);
        si (cellula)
        {
            componens_ponere_actio(cellula, actio_calendarii);
            componens_addere_liberum(radix, cellula);
        }
    }
    redde radix;
}
