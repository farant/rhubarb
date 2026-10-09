/* scriba_componentia.c - componere scribae */

#include "scriba_componentia.h"
#include "dispositio.h"
#include "chorda.h"

#include <stdio.h>
#include <string.h>


/* ==================================================
 * Auxilia
 * ================================================== */

interior s32
attributum_s32 (
    constans InsulaRamus* ramus,
             InsulaGenus  genus,
      constans character* titulus,
                     s32  praestitutum)
{
    chorda* a;
       s32  v;

    a = insula_ramus_attributum(ramus, genus, titulus);
    si (a && chorda_ut_s32(*a, &v))
    {
        redde v;
    }
    redde praestitutum;
}

interior b32
attributum_est (
    constans InsulaRamus* ramus,
      constans character* titulus,
      constans character* valor)
{
    chorda* a;

    a = insula_ramus_attributum(ramus, INSULA_EPHEMERA, titulus);
    redde a ? chorda_aequalis_literis(*a, valor) : FALSUM;
}

interior Componens*
nodus (
                Piscina* p,
    InternamentumChorda* in,
     constans character* id,
                 Partes  partes,
                  Fines  f)
{
    Componens* c;

    c = componens_creare(p, in, id, partes);
    componens_ponere_fines(c, f);
    redde c;
}

interior Fines
fines (
    s32 x,
    s32 y,
    s32 w,
    s32 h)
{
    Fines f;

    f.x         = x;
    f.y         = y;
    f.latitudo  = w;
    f.altitudo  = h;
    redde f;
}

/* cellulae -> pixela; ora superficiem tangens ad superficiem (ut
 * pictor 013 B3) */
interior Fines
ad_pixela (
    Fines f,
      s32 columnae,
      s32 lineae,
      s32 latitudo,
      s32 altitudo,
      s32 cw,
      s32 ch)
{
    Fines p;

    p.x = f.x * cw;
    p.y = f.y * ch;
    p.latitudo  = (f.x + f.latitudo >= columnae) ? latitudo - p.x
                                                  : f.latitudo * cw;
    p.altitudo  = (f.y + f.altitudo >= lineae) ? altitudo - p.y
                                                : f.altitudo * ch;
    redde p;
}

/* volutio in cellulis: 0 si totum videtur; aliter cursor centratus,
 * ad [0, totum - visum] limitatus */
interior s32
volutio (
    s32 cursor,
    s32 totum,
    s32 visum)
{
    s32 v;

    si (visum <= ZEPHYRUM || totum <= visum)
    {
        redde ZEPHYRUM;
    }
    v = cursor - visum / II;
    si (v < ZEPHYRUM)
    {
        v = ZEPHYRUM;
    }
    si (v > totum - visum)
    {
        v = totum - visum;
    }
    redde v;
}


/* ==================================================
 * Componere
 * ================================================== */

/* <componens/> <purus/> */
/* indicium foci (vicus-latera): ramus focatus - sine Motu aut ramo
 * radicis (applicatio sola) semper; in vico spatium Motus = id rami */
interior b32
focatum_est (
    constans Motus* motus,
       InsulaRamus  ramus)
{
    redde !motus || chorda_vacua(ramus.id)
        || chorda_aequalis(motus->spatium, ramus.id);
}

Componens*
scriba_componere (
     InsulaRepositorium* repo,
         constans Motus* motus,
                Piscina* piscina,
    InternamentumChorda* intern,
                 vacuum* ctx)
{
           InsulaRamus  ramus;
      ScribaCompositio* cfg;
             Componens* radix;
             Componens* prospectus;
             Componens* pagina;
             Componens* status;
             Componens* paginae;
            Dispositio* dispositio;
       DispositioForma  forma;
                   s32  d_radix;
                   s32  d_prospectus;
                   s32  d_status;
                 Fines  fp;
                 Fines  fs;
                   s32  latitudo;
                   s32  altitudo;
                   s32  cw;
                   s32  ch;
                   s32  folium_x;
                   s32  folium_y;
                   s32  cl;
                   s32  cc;
                   s32  sl;
                   b32  visualis;
             character  titulus[XLVIII];
             character  index[XLVIII];
    constans character* modus;
                   s32  positio;
                   s32  numerus;
                   s32  ix;
                   s32  nx;
                   s32  limes;
                chorda* nuntius;
                chorda  textus_nuntii;
             Componens* nodus_nuntii;

    si (!repo || !piscina || !intern || !ctx)
    {
        redde NIHIL;
    }
    cfg = (ScribaCompositio*)ctx;
    /* R4: status scribae per ramum (sine eo radix repositorii) */
    ramus = cfg->ramus.repo ? cfg->ramus : insula_ramus_radix(repo);
    latitudo = attributum_s32(&ramus, INSULA_EPHEMERA,
        "superficies_latitudo", (s32)cfg->fenestra_latitudo);
    altitudo = attributum_s32(&ramus, INSULA_EPHEMERA,
        "superficies_altitudo", (s32)cfg->fenestra_altitudo);
    cw = (cfg->cellula_latitudo > ZEPHYRUM) ? (s32)cfg->cellula_latitudo
                                            : I;
    ch = (cfg->cellula_altitudo > ZEPHYRUM) ? (s32)cfg->cellula_altitudo
                                            : I;

    /* columna: prospectus (crescens, praecisus) super statum (fixa) */
    dispositio = dispositio_creare(piscina);
    dispositio_formam_initiare(&forma);
    forma.directio        = DISPOSITIO_COLUMNA;
    forma.latitudo.genus  = DISPOSITIO_CRESCENS;
    forma.altitudo.genus  = DISPOSITIO_CRESCENS;
    d_radix               = dispositio_addere(dispositio, -I, &forma);
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus = DISPOSITIO_CRESCENS;
    forma.altitudo.genus = DISPOSITIO_CRESCENS;
    forma.praecidere_x = VERUM;
    forma.praecidere_y = VERUM;
    d_prospectus = dispositio_addere(dispositio, d_radix, &forma);
    dispositio_formam_initiare(&forma);
    forma.latitudo.genus = DISPOSITIO_CRESCENS;
    forma.altitudo.genus = DISPOSITIO_FIXA;
    forma.altitudo.valor = (s32)cfg->status_lineae;
    d_status = dispositio_addere(dispositio, d_radix, &forma);
    dispositio_computare(dispositio, latitudo / cw, altitudo / ch,
        NIHIL,
                         NIHIL);
    fp = ad_pixela(dispositio_fines(dispositio, d_prospectus),
                   latitudo / cw, altitudo / ch, latitudo, altitudo, cw,
                   ch);
    fs = ad_pixela(dispositio_fines(dispositio, d_status), latitudo
        / cw,
                   altitudo / ch, latitudo, altitudo, cw, ch);

    folium_x = attributum_s32(&ramus, INSULA_DURABILIS, "latitudo", I);
    folium_y = attributum_s32(&ramus, INSULA_DURABILIS, "altitudo", I);
    cl = attributum_s32(&ramus, INSULA_EPHEMERA, "cursor_linea",
        ZEPHYRUM);
    cc = attributum_s32(&ramus, INSULA_EPHEMERA, "cursor_columna",
        ZEPHYRUM);
    sl = attributum_s32(&ramus, INSULA_EPHEMERA, "selectio_linea", -I);
    visualis = attributum_est(&ramus, "modus", "visualis");
    modus = attributum_est(&ramus, "modus", "inserere") ? "inserere"
          : visualis                                  ? "visualis"
          :                                             "normalis";

    radix = nodus(piscina, intern, "radix", PARTES_NULLUM,
                  fines(ZEPHYRUM, ZEPHYRUM, latitudo, altitudo));
    prospectus = nodus(piscina, intern, "prospectus", PARTES_PROSPECTUS,
                       fp);
    componens_ponere_sectio(prospectus, VERUM);
    /* indicium foci: figura mensae exemplar addit */
    si (focatum_est(motus, ramus))
    {
        componens_ponere_titulum(prospectus, "focatum");
    }
    /* volutio: folium cum margine (cellula utrimque) in cellulis */
    componens_ponere_transformatio(prospectus,
        -volutio(cc + I, folium_x + II, fp.latitudo / cw) * cw,
        -volutio(cl + I, folium_y + II, fp.altitudo / ch) * ch, I);

    pagina = nodus(piscina, intern, "pagina", PARTES_CAMPUS,
                   fines(cw, ch, folium_x * cw, folium_y * ch));
    componens_ponere_actio(pagina, "pagina.clavis");
    componens_ponere_focusabilis(pagina, VERUM);
    componens_ponere_praedicatum(pagina, PRAEDICATUM_PROPRIUS);
    componens_ponere_titulum(pagina, modus);
    pagina->numerus_punctorum = (visualis && sl >= ZEPHYRUM) ? II : I;
    pagina->puncta = (Punctum*)piscina_allocare(piscina,
        (memoriae_index)pagina->numerus_punctorum * magnitudo(Punctum));
    pagina->puncta[ZEPHYRUM].x = cc;
    pagina->puncta[ZEPHYRUM].y = cl;
    si (pagina->numerus_punctorum == II)
    {
        pagina->puncta[I].x = attributum_s32(&ramus, INSULA_EPHEMERA,
            "selectio_columna", ZEPHYRUM);
        pagina->puncta[I].y = sl;
    }

    status = nodus(piscina, intern, "status", PARTES_TITULUS, fs);
    sprintf(titulus, "%s %d:%d",
            modus[ZEPHYRUM] == 'i' ? "INSERERE"
            : modus[ZEPHYRUM] == 'v' ? "VISUALIS" : "NORMALIS",
            (integer)(cl + I), (integer)(cc + I));
    componens_ponere_titulum(status, titulus);
    /* index paginae (visus super librum, vicus-latera S2b): dextrorsum,
     * cellula ab ora; omittitur si textum status tangeret */
    limes   = fs.latitudo - cw;
    positio = attributum_s32(&ramus, INSULA_EPHEMERA, "pagina_positio",
        ZEPHYRUM);
    numerus = attributum_s32(&ramus, INSULA_EPHEMERA, "paginae_numerus",
        ZEPHYRUM);
    si (positio > ZEPHYRUM && numerus > ZEPHYRUM)
    {
        sprintf(index, "pagina %d/%d", (integer)positio,
            (integer)numerus);
        ix = fs.latitudo - (s32)(strlen(index) + I) * cw;
        si (ix >= II + (s32)(strlen(titulus) + II) * cw)
        {
            paginae = nodus(piscina, intern, "paginae", PARTES_INDEX,
                fines(ix, ZEPHYRUM, (s32)strlen(index) * cw,
                fs.altitudo));
            componens_ponere_titulum(paginae, index);
            componens_addere_liberum(status, paginae);
            limes = ix - cw;
        }
    }
    /* S3b-2: nuntius post modum et positionem (duabus cellulis), ante
     * indicem paginae (cellula una); praecisus ut capiat, omissus si
     * nulla cellula restat */
    nuntius = insula_ramus_attributum(&ramus, INSULA_EPHEMERA,
        "nuntius");
    nx = II + (s32)(strlen(titulus) + II) * cw;
    si (nuntius && nuntius->mensura > ZEPHYRUM && limes - nx >= cw)
    {
        textus_nuntii = *nuntius;
        si ((s32)textus_nuntii.mensura * cw > limes - nx)
        {
            textus_nuntii.mensura = (i32)((limes - nx) / cw);
        }
        nodus_nuntii = nodus(piscina, intern, "nuntius",
            PARTES_DIALOGUS,
            fines(nx, ZEPHYRUM, (s32)textus_nuntii.mensura * cw,
            fs.altitudo));
        componens_ponere_titulum(nodus_nuntii, chorda_ut_cstr(
            textus_nuntii, piscina));
        componens_addere_liberum(status, nodus_nuntii);
    }

    componens_addere_liberum(prospectus, pagina);
    componens_addere_liberum(radix, prospectus);
    componens_addere_liberum(radix, status);
    redde radix;
}
