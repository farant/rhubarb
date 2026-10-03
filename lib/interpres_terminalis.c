/* interpres_terminalis.c - Vide interpres_terminalis.h
 *
 * Tabulae legacy ex tessera_eventum.c (B1b) translatae in vocabularium
 * Eventus: claves nominatae (CSI A-D/H/F/Z, '~'-codices, SS3), mus
 * SGR/X10, regimina. Modificatores xterm (parametrum m): m-1 = bits
 * maiuscula I, alterum II, imperium IV, meta VIII (-> MOD_SUPER).
 */

#include "interpres_terminalis.h"
#include "utf8.h"
#include "claves_physicae.h"
#include <string.h>

#define MODIFICATOR_MAXIMUS CCLVI   /* ultra: invalidum (ingens) */


/* ==================================================
 * Auxilia
 * ================================================== */

interior i32
_modificantes_csi (
    s32 m)
{
    i32 fructus = ZEPHYRUM;
    s32 bits;

    si (m <= I || m > MODIFICATOR_MAXIMUS)
    {
        redde ZEPHYRUM;
    }
    bits = m - I;
    si (bits & I)
    { fructus |= MOD_SHIFT;
    }
    si (bits & II)
    { fructus |= MOD_ALT;
    }
    si (bits & IV)
    { fructus |= MOD_IMPERIUM;
    }
    si (bits & VIII)
    { fructus |= MOD_SUPER;
    }
    /* kitty: XVI hyper et XXXII meta sine pari in vocabulario */
    si (bits & LXIV)
    { fructus |= MOD_CAPS_LOCK;
    }
    si (bits & CXXVIII)
    { fructus |= MOD_NUM_LOCK;
    }
    redde fructus;
}

/* Clavis cum actione (B2b): SOLUTA -> genus LIBERATUS */
interior i32
_clavem_typo (
    EventusCauda* cauda,
             s64  tempus,
        clavis_t  clavis,
             s32  runa,
             i32  modificantes,
    EventusCodex  codex,
    EventusActio  actio,
       character  typus)
{
    Eventus e;

    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus = (actio == EVENTUS_ACTIO_SOLUTA)
        ? EVENTUS_CLAVIS_LIBERATUS : EVENTUS_CLAVIS_DEPRESSUS;
    e.tempus                     = tempus;
    e.datum.clavis.clavis        = clavis;
    e.datum.clavis.typus         = typus;
    e.datum.clavis.modificantes  = modificantes;
    e.datum.clavis.runa          = runa;
    e.datum.clavis.codex         = codex;
    e.datum.clavis.actio         = actio;
    redde eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
}

/* typus ex clave (nominatae: regimen eius, e.g. '\r', '\t') */
interior i32
_clavem_actio (
    EventusCauda* cauda,
             s64  tempus,
        clavis_t  clavis,
             s32  runa,
             i32  modificantes,
    EventusCodex  codex,
    EventusActio  actio)
{
    redde _clavem_typo(cauda, tempus, clavis, runa, modificantes, codex,
        actio, ((s32)clavis > ZEPHYRUM && (s32)clavis < CXXVIII)
                   ? (character)clavis : '\0');
}

interior i32
_clavem (
    EventusCauda* cauda,
             s64  tempus,
        clavis_t  clavis,
             s32  runa,
             i32  modificantes,
    EventusCodex  codex)
{
    redde _clavem_actio(cauda, tempus, clavis, runa, modificantes,
        codex,
        EVENTUS_ACTIO_PRESSA);
}

/* Runa imprimibilis -> clavis logica (litterae MAIUSCULAE ut fenestra;
 * runa litterarum minuscula - Shift nescitur, textus casum fert) */
interior i32
_runae_clavem (
    EventusCauda* cauda,
             s64  tempus,
             s32  r,
             i32  modificantes)
{
    clavis_t clavis  = CLAVIS_IGNOTA;
         s32 runa    = r;

    si (r >= 'a' && r <= 'z')
    {
        clavis  = (clavis_t)(r - 'a' + 'A');
    }
    alioquin si (r >= 'A' && r <= 'Z')
    {
        clavis  = (clavis_t)r;
        runa    = r - 'A' + 'a';
    }
    alioquin si (r >= 0x20 && r < 0x7F)
    {
        clavis = (clavis_t)r;
    }
    /* typus = character VERUS (ut fenestra characters[0]): 'A' ab
     * 'a' discernit etiam ubi textus deest (alterum) - B3a */
    redde _clavem_typo(cauda, tempus, clavis, runa, modificantes,
        EVENTUS_CODEX_IGNOTUS, EVENTUS_ACTIO_PRESSA,
        (r > ZEPHYRUM && r < CXXVIII) ? (character)r : '\0');
}

/* Octetus regiminis (C0, DEL) -> clavis. HONESTA: '\n' = Ctrl+J,
 * 0x08 = Ctrl+H (proiectio tesserae eas coniungit). */
interior i32
_regimen (
    EventusCauda* cauda,
             s64  tempus,
             i32  b,
             i32  modificantes)
{
    si (b == 0x0D)
    {
        redde _clavem(cauda, tempus, CLAVIS_REDITUS, ZEPHYRUM,
            modificantes, EVENTUS_CODEX_IGNOTUS);
    }
    si (b == 0x09)
    {
        redde _clavem(cauda, tempus, CLAVIS_TABULA, ZEPHYRUM,
            modificantes, EVENTUS_CODEX_IGNOTUS);
    }
    si (b == 0x7F)
    {
        redde _clavem(cauda, tempus, CLAVIS_RETRORSUM, ZEPHYRUM,
            modificantes, EVENTUS_CODEX_IGNOTUS);
    }
    si (b == ZEPHYRUM)
    {
        redde _clavem(cauda, tempus, CLAVIS_SPATIUM, (s32)' ',
            modificantes | MOD_IMPERIUM, EVENTUS_CODEX_IGNOTUS);
    }
    si (b >= I && b <= XXVI)
    {
        redde _clavem(cauda, tempus, (clavis_t)('A' + b - I),
            (s32)('a' + b - I), modificantes | MOD_IMPERIUM,
            EVENTUS_CODEX_IGNOTUS);
    }
    si (b >= 0x1C && b <= 0x1F)
    {
        redde _clavem(cauda, tempus, (clavis_t)(b | 0x40),
            (s32)(b | 0x40), modificantes | MOD_IMPERIUM,
            EVENTUS_CODEX_IGNOTUS);
    }
    redde ZEPHYRUM;
}

/* Clavis nominata ex finali (CSI aut SS3) */
interior i32
_finalem (
    EventusCauda* cauda,
             s64  tempus,
             i32  finale,
             i32  modificantes,
    EventusActio  actio)
{
    commutatio (finale)
    {
        casus 'A': redde _clavem_actio(cauda, tempus, CLAVIS_SURSUM,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_SURSUM,
                       actio);
        casus 'B': redde _clavem_actio(cauda, tempus, CLAVIS_DEORSUM,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_DEORSUM,
                       actio);
        casus 'C': redde _clavem_actio(cauda, tempus, CLAVIS_DEXTER,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_DEXTRA,
                       actio);
        casus 'D': redde _clavem_actio(cauda, tempus, CLAVIS_SINISTER,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_SAGITTA_SINISTRA,
                       actio);
        casus 'H': redde _clavem_actio(cauda, tempus, CLAVIS_DOMUS,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_DOMUS, actio);
        casus 'F': redde _clavem_actio(cauda, tempus, CLAVIS_FINIS,
                       ZEPHYRUM,
                       modificantes, EVENTUS_CODEX_FINIS, actio);
        casus 'Z': redde _clavem_actio(cauda, tempus, CLAVIS_TABULA,
                       ZEPHYRUM,
                       modificantes | MOD_SHIFT, EVENTUS_CODEX_TABULA,
                       actio);
        ordinarius:
            frange;
    }
    redde ZEPHYRUM;
}

/* F n (1..12) */
interior i32
_functionem (
    EventusCauda* cauda,
             s64  tempus,
             s32  n,
             i32  modificantes,
    EventusActio  actio)
{
    redde _clavem_actio(cauda, tempus, (clavis_t)((s32)CLAVIS_F1 + n
        - I),
        ZEPHYRUM, modificantes,
        (EventusCodex)((s32)EVENTUS_CODEX_FUNCTIONES + n - I), actio);
}

/* '~'-codices (xterm/vt220) */
interior i32
_clavem_tildae (
    EventusCauda* cauda,
             s64  tempus,
             s32  codex,
             i32  modificantes,
    EventusActio  actio)
{
    commutatio (codex)
    {
        casus I:
        casus VII:
            redde _clavem_actio(cauda, tempus, CLAVIS_DOMUS, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_DOMUS, actio);
        casus IV:
        casus VIII:
            redde _clavem_actio(cauda, tempus, CLAVIS_FINIS, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_FINIS, actio);
        casus II:
            redde _clavem_actio(cauda, tempus, CLAVIS_IGNOTA, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_INSERERE, actio);
        casus III:
            redde _clavem_actio(cauda, tempus, CLAVIS_DELERE, ZEPHYRUM,
                modificantes, EVENTUS_CODEX_DELERE, actio);
        casus V:
            redde _clavem_actio(cauda, tempus, CLAVIS_PAGINA_SURSUM,
                ZEPHYRUM,
                modificantes, EVENTUS_CODEX_PAGINA_SURSUM, actio);
        casus VI:
            redde _clavem_actio(cauda, tempus, CLAVIS_PAGINA_DEORSUM,
                ZEPHYRUM,
                modificantes, EVENTUS_CODEX_PAGINA_DEORSUM, actio);
        ordinarius:
            frange;
    }
    si (codex >= XI && codex <= XV)
    {
        redde _functionem(cauda, tempus, codex - X, modificantes,
            actio);
    }
    si (codex >= XVII && codex <= XXI)
    {
        redde _functionem(cauda, tempus, codex - XI, modificantes,
            actio);
    }
    si (codex == XXIII || codex == XXIV)
    {
        redde _functionem(cauda, tempus, codex - XII, modificantes,
            actio);
    }
    redde ZEPHYRUM;   /* ignota (200/201 glutinum: fons) */
}

/* Mus (SGR aut X10): b = codex bottonis (bits 0-1 botton, 4 maiuscula,
 * 8 alterum, 16 imperium, 32 motus, 64 rota); x, y cellulae 1-basatae
 * -> CENTRUM cellulae in pixelis nostris. Rota: gradus = cellula
 * altitudo; 64 sursum = dy +, 65 = dy -, 66 = dx +, 67 = dx -. */
interior i32
_murem (
    InterpresTerminalis* in,
           EventusCauda* cauda,
                    s64  tempus,
                    s32  b,
                    s32  x,
                    s32  y,
                    b32  solutio)
{
              Eventus e;
                  i32 modi   = ZEPHYRUM;
                  s32 basis  = b & III;
         mus_botton_t botton;

    si (b & IV)
    { modi |= MOD_SHIFT;
    }
    si (b & VIII)
    { modi |= MOD_ALT;
    }
    si (b & XVI)
    { modi |= MOD_IMPERIUM;
    }
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.tempus = tempus;
    /* centrum cellulae; memoratur pro depositione (B3b) */
    in->indicator_x = (x - I) * in->cellula_latitudo
                      + in->cellula_latitudo / II;
    in->indicator_y = (y - I) * in->cellula_altitudo
                      + in->cellula_altitudo / II;
    si (b & LXIV)
    {
        s32 g = in->cellula_altitudo;

        si (solutio || (b & XXXII))
        {
            /* rota solutionem non habet; motus + rota (96/97) tacite,
             * ut tessera (B3a) */
            redde ZEPHYRUM;
        }
        /* B3a: positio (centrum cellulae) et modificantes */
        e.datum.rotula.x             = in->indicator_x;
        e.datum.rotula.y             = in->indicator_y;
        e.datum.rotula.modificantes  = modi;
        e.genus                      = EVENTUS_MUS_ROTULA;
        e.datum.rotula.genus         = EVENTUS_ROTULA_GRADATA;
        e.datum.rotula.dy      = (basis == ZEPHYRUM) ? g
                               : (basis == I) ? -g : ZEPHYRUM;
        e.datum.rotula.dx      = (basis == II) ? g
                               : (basis == III) ? -g : ZEPHYRUM;
        e.datum.rotula.delta_x = (f32)e.datum.rotula.dx;
        e.datum.rotula.delta_y = (f32)e.datum.rotula.dy;
        redde eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
    }
    botton = (basis == ZEPHYRUM) ? MUS_SINISTER
           : (basis == I) ? MUS_MEDIUS
           : (basis == II) ? MUS_DEXTER : (mus_botton_t)ZEPHYRUM;
    e.datum.mus.x                = in->indicator_x;
    e.datum.mus.y                = in->indicator_y;
    e.datum.mus.botton           = botton;
    e.datum.mus.modificantes     = modi;
    e.datum.mus.indicator_genus  = EVENTUS_INDICATOR_MUS;
    e.datum.mus.pressio          = EVENTUS_PRESSIO_IGNOTA;
    si (b & XXXII)
    {
        e.genus = EVENTUS_MUS_MOTUS;
        redde eventus_caudae_motum_impellere(cauda, &e) ? I : ZEPHYRUM;
    }
    e.genus = (solutio || basis == III) ? EVENTUS_MUS_LIBERATUS
                                        : EVENTUS_MUS_DEPRESSUS;
    redde eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
}

/* Campi CSI (kitty): parametra per ';' in campos, ':' subcampos
 * dividit (separatores bitus i = ':' post parametrum i). */
#define CAMPI_MAXIMI III

nomen structura {
    s32 valor[CAMPI_MAXIMI][SERIES_PARAMETRA_MAXIMA];
    i32 numerus[CAMPI_MAXIMI];
} CampiCsi;

interior vacuum
_campos_legere (
    constans SeriesLexema* l,
                 CampiCsi* c)
{
    i32 k;
    i32 f = ZEPHYRUM;

    memset(c, ZEPHYRUM, magnitudo(CampiCsi));
    per (k = ZEPHYRUM; k < l->numerus_parametrorum; k++)
    {
        si (f < CAMPI_MAXIMI)
        {
            c->valor[f][c->numerus[f]] = l->parametra[k];
            c->numerus[f]++;
        }
        si (!(l->separatores & ((i32)I << k)))
        {
            f++;      /* ';' post parametrum k: campus novus */
        }
    }
}

interior s32
_campus (
    constans CampiCsi* c,
                  i32  campus,
                  i32  pars,
                  s32  si_abest)
{
    si (campus >= CAMPI_MAXIMI || pars >= c->numerus[campus])
    {
        redde si_abest;
    }
    /* campus vacuus (';;') = 0 = absens */
    redde (c->valor[campus][pars] == ZEPHYRUM)
        ? si_abest : c->valor[campus][pars];
}

/* genus kitty: 1 pressa, 2 iterata, 3 soluta */
interior EventusActio
_actio_kitty (
    s32 genus)
{
    si (genus == II)
    {
        redde EVENTUS_ACTIO_ITERATA;
    }
    si (genus == III)
    {
        redde EVENTUS_ACTIO_SOLUTA;
    }
    redde EVENTUS_ACTIO_PRESSA;
}

/* Series kitty prima: facultates discuntur (per observationem - fons
 * vexilla impulit, terminalis respondendo ea accepit) et eventus
 * FACULTATES ANTE clavem impellitur. */
interior i32
_kitty_discere (
    InterpresTerminalis* in,
                    s64  tempus,
           EventusCauda* cauda)
{
    Eventus e;

    si (in->kitty_visus)
    {
        redde ZEPHYRUM;
    }
    in->kitty_visus                  = VERUM;
    in->facultates.tabula_distincta  = VERUM;
    in->facultates.liberationes           =
        (b32)((in->kitty_vexilla & INTERPRES_KITTY_GENERA) != ZEPHYRUM);
    in->facultates.codex_physicus         =
        (b32)((in->kitty_vexilla & INTERPRES_KITTY_ALTERNAE)
            != ZEPHYRUM);
    /* B4: OMNES = claves imprimibiles ut CSI u, cum modificantibus */
    in->facultates.modificantes_textus    =
        (b32)((in->kitty_vexilla & INTERPRES_KITTY_OMNES) != ZEPHYRUM);
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus             = EVENTUS_FACULTATES;
    e.tempus            = tempus;
    e.datum.facultates  = in->facultates;
    redde eventus_caudae_impellere(cauda, &e) ? I : ZEPHYRUM;
}

/* Clavis 'CSI clavis[:maiuscula[:basis]] ;modi[:genus] ;textus u' */
interior i32
_kitty_clavem (
    InterpresTerminalis* in,
      constans CampiCsi* c,
                    i32  modi,
           EventusActio  actio,
                    s64  tempus,
           EventusCauda* cauda)
{
             s32 k       = _campus(c, ZEPHYRUM, ZEPHYRUM, ZEPHYRUM);
             s32 basis   = _campus(c, ZEPHYRUM, II, ZEPHYRUM);
    EventusCodex codex   = claves_codex_ex_kitty(k);
        clavis_t clavis  = CLAVIS_IGNOTA;
             s32 runa    = ZEPHYRUM;
             i32 n;
             i32 j;

    commutatio (k)
    {
        casus XXVII:  clavis = CLAVIS_EFFUGIUM;   frange;
        casus XIII:   clavis = CLAVIS_REDITUS;    frange;
        casus IX:     clavis = CLAVIS_TABULA;     frange;
        casus CXXVII: clavis = CLAVIS_RETRORSUM;  frange;
        casus 57358:  clavis = CLAVIS_CAPS_LOCK;  frange;
        casus 57360:  clavis = CLAVIS_NUM_LOCK;   frange;
        casus 57441:  clavis = CLAVIS_SINISTER_SHIFT;    frange;
        casus 57447:  clavis = CLAVIS_DEXTER_SHIFT;      frange;
        casus 57442:  clavis = CLAVIS_SINISTER_IMPERIUM; frange;
        casus 57448:  clavis = CLAVIS_DEXTER_IMPERIUM;   frange;
        casus 57443:  clavis = CLAVIS_SINISTER_ALT;      frange;
        casus 57449:  clavis = CLAVIS_DEXTER_ALT;        frange;
        casus 57444:  clavis = CLAVIS_SINISTER_SUPER;    frange;
        casus 57450:  clavis = CLAVIS_DEXTER_SUPER;      frange;
        casus 57409:  clavis = (clavis_t)'.'; runa = '.'; frange;
        casus 57410:  clavis = (clavis_t)'/'; runa = '/'; frange;
        casus 57411:  clavis = (clavis_t)'*'; runa = '*'; frange;
        casus 57412:  clavis = (clavis_t)'-'; runa = '-'; frange;
        casus 57413:  clavis = (clavis_t)'+'; runa = '+'; frange;
        casus 57414:  clavis = CLAVIS_REDITUS;              frange;
        casus 57415:  clavis = (clavis_t)'='; runa = '='; frange;
        casus 57416:  clavis = (clavis_t)','; runa = ','; frange;
        casus 57417:  clavis = CLAVIS_SINISTER;       frange;
        casus 57418:  clavis = CLAVIS_DEXTER;         frange;
        casus 57419:  clavis = CLAVIS_SURSUM;         frange;
        casus 57420:  clavis = CLAVIS_DEORSUM;        frange;
        casus 57421:  clavis = CLAVIS_PAGINA_SURSUM;  frange;
        casus 57422:  clavis = CLAVIS_PAGINA_DEORSUM; frange;
        casus 57423:  clavis = CLAVIS_DOMUS;          frange;
        casus 57424:  clavis = CLAVIS_FINIS;          frange;
        casus 57426:  clavis = CLAVIS_DELERE;         frange;
        ordinarius:
            si (k >= 57399 && k <= 57408)
            {
                /* tabula numerica 0-9 (codex nullus in vocabulario) */
                clavis  = (clavis_t)('0' + (k - 57399));
                runa    = (s32)('0' + (k - 57399));
            }
            alioquin si (k < 57344 || k > 63743)
            {
                /* runa (dispositionis currentis, sine maiuscula) */
                runa    = k;
                clavis  = (k >= 'a'
                    && k <= 'z') ? (clavis_t)(k - 'a' + 'A')
                        : (k >= 0x20 && k < 0x7F) ? (clavis_t)k
                        : CLAVIS_IGNOTA;
                codex   = (basis
                    != ZEPHYRUM) ? claves_codex_ex_littera(basis)
                        : (in->kitty_vexilla & INTERPRES_KITTY_ALTERNAE)
                            ? claves_codex_ex_littera(k)
                            : EVENTUS_CODEX_IGNOTUS;
            }
            frange;
    }
    /* typus = character verus: clavis maiuscula (campus 0 pars 1) sub
     * Shift, alioquin clavis ipsa (minuscula) - non clavis_t */
    {
        s32 verus = ((modi & MOD_SHIFT)
            && _campus(c, ZEPHYRUM, I, ZEPHYRUM))
            ? _campus(c, ZEPHYRUM, I, ZEPHYRUM) : runa;

        n = _clavem_typo(cauda, tempus, clavis, runa, modi, codex,
            actio,
            (verus >= 0x20 && verus < 0x7F) ? (character)verus : '\0');
    }
    /* textus associatus: solum si campus adest (vexillum TEXTUS) */
    si (actio != EVENTUS_ACTIO_SOLUTA && c->numerus[II] > ZEPHYRUM)
    {
         i8 octeti[SERIES_PARAMETRA_MAXIMA * IV];
        i32 m = ZEPHYRUM;

        per (j = ZEPHYRUM; j < c->numerus[II]; j++)
        {
            m += (i32)utf8_codere(c->valor[II][j], octeti + m);
        }
        si (m > ZEPHYRUM)
        {
            n += eventus_caudae_textum_impellere(cauda, tempus, octeti,
                m,
                EVENTUS_ORIGO_SCRIPTA) ? I : ZEPHYRUM;
        }
    }
    redde n;
}

interior b32
_sola_fuga (
    constans SeriesLexema* l)
{
    i32 k;

    si (l->crudum.mensura == ZEPHYRUM)
    {
        redde FALSUM;
    }
    per (k = ZEPHYRUM; k < l->crudum.mensura; k++)
    {
        si (l->crudum.datum[k] != (i8)0x1B)
        {
            redde FALSUM;
        }
    }
    redde VERUM;
}

/* Alterum pendens consumitur ab eventu proximo (clavis aut textus) */
interior i32
_alterum (
    InterpresTerminalis* in)
{
    i32 m = in->alterum_pendens ? MOD_ALT : ZEPHYRUM;

    in->alterum_pendens = FALSUM;
    redde m;
}


/* ==================================================
 * Publica
 * ================================================== */

vacuum
interpres_initiare (
    InterpresTerminalis* interpres,
                    s32  cellula_latitudo,
                    s32  cellula_altitudo)
{
    interpres->cellula_latitudo  = cellula_latitudo;
    interpres->cellula_altitudo  = cellula_altitudo;
    interpres->alterum_pendens   = FALSUM;
    interpres->kitty_vexilla     = ZEPHYRUM;
    interpres->kitty_visus       = FALSUM;
    interpres->indicator_x       = ZEPHYRUM;
    interpres->indicator_y       = ZEPHYRUM;
    /* legacy: quod terminalis sine kitty narrare potest */
    memset(&interpres->facultates, ZEPHYRUM,
        magnitudo(EventusFacultates));
    /* super FALSUM donec ?1003 declaratur (rivus_modos_intrare) */
    interpres->facultates.scriptura_copiae  = EVENTUS_FACULTAS_FORTASSE;
    /* depositio NULLA donec promotio declaratur (rivus, B3b) */
    interpres->facultates.depositio       = EVENTUS_DEPOSITIO_NULLA;
    interpres->facultates.gradus_rotulae  = cellula_altitudo;
}

i32
interpres_lexema (
         InterpresTerminalis* in,
       constans SeriesLexema* l,
                         b32  post_moram,
                         s64  tempus,
                EventusCauda* cauda)
{
    i32 n = ZEPHYRUM;
    i32 modi;

    commutatio (l->genus)
    {
        casus SERIES_IMPRIMERE:
        {
            constans i8* p      = l->textus.datum;
            constans i8* finis  = p + l->textus.mensura;

            dum (p < finis)
            {
                constans i8* initium  = p;
                        s32  r        = utf8_decodere(&p, finis);

                si (r < ZEPHYRUM)
                {
                    si (p == initium)
                    {
                        p++;    /* octetus invalidus abicitur */
                    }
                    perge;
                }
                modi  = _alterum(in);
                n     += _runae_clavem(cauda, tempus, r, modi);
                si (modi == ZEPHYRUM)
                {
                    n += eventus_caudae_textum_impellere(cauda, tempus,
                        initium, (i32)(p - initium),
                        EVENTUS_ORIGO_SCRIPTA) ? I : ZEPHYRUM;
                }
            }
            redde n;
        }

        casus SERIES_EXSEQUI:
            redde _regimen(cauda, tempus, ((i32)l->finale) & 0xFF,
                _alterum(in));

        casus SERIES_FUGA:
            si (!post_moram)
            {
                /* abrupta: ESC solus = alterum clavis proximae */
                si (_sola_fuga(l))
                {
                    in->alterum_pendens = VERUM;
                }
                redde ZEPHYRUM;
            }
            in->alterum_pendens = FALSUM;
            si (_sola_fuga(l))
            {
                i32 k;

                /* ESC (ESC) post moram: Effugium pro quoque */
                per (k = ZEPHYRUM; k < l->crudum.mensura; k++)
                {
                    n += _clavem(cauda, tempus, CLAVIS_EFFUGIUM,
                        ZEPHYRUM,
                        ZEPHYRUM, EVENTUS_CODEX_IGNOTUS);
                }
                redde n;
            }
            si (l->crudum.mensura == II)
            {
                /* 'ESC [' / 'ESC O' / 'ESC P' solum = alterum + x */
                redde _runae_clavem(cauda, tempus,
                    ((s32)l->crudum.datum[I]) & 0xFF, MOD_ALT);
            }
            redde ZEPHYRUM;   /* series dimidia abicitur */

        casus SERIES_ESC:
            in->alterum_pendens = FALSUM;
            si (   l->numerus_intermediorum > ZEPHYRUM
                || l->finale < 0x20 || l->finale > 0x7E)
            {
                redde ZEPHYRUM;
            }
            redde _runae_clavem(cauda, tempus, (s32)l->finale, MOD_ALT);

        casus SERIES_CSI:
        {
                CampiCsi c;
                     s32 p0;
            EventusActio actio;

            in->alterum_pendens = FALSUM;
            /* intermedia, privata praeter '<' (e.g. '?31u' responsum
             * vexillorum kitty, DA): ignota */
            si (   l->numerus_intermediorum > ZEPHYRUM
                || (l->privatum != ZEPHYRUM && l->privatum != '<'))
            {
                redde ZEPHYRUM;
            }
            p0 = (l->numerus_parametrorum >= I) ? l->parametra[0]
                                                : ZEPHYRUM;
            si (l->privatum == '<')
            {
                si (   (l->finale == 'M' || l->finale == 'm')
                    && l->numerus_parametrorum >= III)
                {
                    redde _murem(in, cauda, tempus, p0, l->parametra[I],
                        l->parametra[II], (b32)(l->finale == 'm'));
                }
                redde ZEPHYRUM;
            }
            si (   l->numerus_parametrorum == ZEPHYRUM
                && (l->finale == 'I' || l->finale == 'O'))
            {
                Eventus e;

                memset(&e, ZEPHYRUM, magnitudo(Eventus));
                e.genus   = (l->finale == 'I') ? EVENTUS_FOCUS
                                               : EVENTUS_DEFOCUS;
                e.tempus  = tempus;
                redde eventus_caudae_impellere(cauda,
                    &e) ? I : ZEPHYRUM;
            }
            _campos_legere(l, &c);
            modi   = _modificantes_csi(_campus(&c, I, ZEPHYRUM, I));
            actio  = _actio_kitty(_campus(&c, I, I, I));
            si (l->praefixum)
            {
                modi |= MOD_ALT;   /* ESC ESC [ A = alterum + sursum */
            }
            /* kitty: 'u', aut pars generis in forma legacy */
            si (l->finale == 'u' || c.numerus[I] >= II)
            {
                n = _kitty_discere(in, tempus, cauda);
            }
            si (l->finale == 'u')
            {
                redde n + _kitty_clavem(in, &c, modi, actio, tempus,
                    cauda);
            }
            si (   l->finale     == '~' && p0 == XXVII
                && c.numerus[II] >= I)
            {
                /* xterm modifyOtherKeys: CSI 27 ; m ; c ~ (codificator
                 * legacy Enter/Tab/Escape modificatos sic mittit,
                 * B6a) */
                s32 k = _campus(&c, II, ZEPHYRUM, ZEPHYRUM);

                commutatio (k)
                {
                    casus XIII:
                        redde n + _clavem(cauda, tempus, CLAVIS_REDITUS,
                            ZEPHYRUM, modi, EVENTUS_CODEX_IGNOTUS);
                    casus IX:
                        redde n + _clavem(cauda, tempus, CLAVIS_TABULA,
                            ZEPHYRUM, modi, EVENTUS_CODEX_IGNOTUS);
                    casus XXVII:
                        redde n + _clavem(cauda, tempus,
                            CLAVIS_EFFUGIUM,
                            ZEPHYRUM, modi, EVENTUS_CODEX_IGNOTUS);
                    casus CXXVII:
                        redde n + _clavem(cauda, tempus,
                            CLAVIS_RETRORSUM,
                            ZEPHYRUM, modi, EVENTUS_CODEX_IGNOTUS);
                    ordinarius:
                        redde n + ((k > ZEPHYRUM)
                            ? _runae_clavem(cauda, tempus, k, modi)
                            : ZEPHYRUM);
                }
            }
            si (l->finale == '~')
            {
                redde n + ((c.numerus[ZEPHYRUM] >= I)
                    ? _clavem_tildae(cauda, tempus, p0, modi, actio)
                    : ZEPHYRUM);
            }
            /* kitty F1 F2 F4: CSI P Q S (F3 = CSI 13~: CSI R = CPR) */
            si (   l->finale == 'P' || l->finale == 'Q'
                || l->finale == 'S')
            {
                redde n + _functionem(cauda, tempus,
                    (l->finale == 'S') ? IV : (s32)(l->finale - 'P')
                        + I,
                    modi, actio);
            }
            redde n + _finalem(cauda, tempus, (i32)l->finale, modi,
                actio);
        }

        casus SERIES_SS:
            in->alterum_pendens = FALSUM;
            si (l->introductor != 'O')
            {
                redde ZEPHYRUM;
            }
            modi = (l->numerus_parametrorum >= I)
                ? _modificantes_csi(l->parametra[0]) : ZEPHYRUM;
            si (l->praefixum)
            {
                modi |= MOD_ALT;
            }
            si (l->finale >= 'P' && l->finale <= 'S')
            {
                redde _functionem(cauda, tempus,
                    (s32)(l->finale - 'P') + I, modi,
                    EVENTUS_ACTIO_PRESSA);
            }
            redde _finalem(cauda, tempus, (i32)l->finale, modi,
                EVENTUS_ACTIO_PRESSA);

        ordinarius:
            /* OSC, DCS, APC (responsa), NIHIL */
            redde ZEPHYRUM;
    }
}

i32
interpres_x10 (
    InterpresTerminalis* interpres,
                    i32  cb,
                    i32  cx,
                    i32  cy,
                    s64  tempus,
           EventusCauda* cauda)
{
    s32 b = (s32)cb - XXXII;

    si (b < ZEPHYRUM || cx < XXXIII || cy < XXXIII)
    {
        redde ZEPHYRUM;   /* onus malum */
    }
    redde _murem(interpres, cauda, tempus, b, (s32)cx - XXXII,
        (s32)cy - XXXII, FALSUM);
}

i32
interpres_glutinum (
    InterpresTerminalis* interpres,
            constans i8* octeti,
                    i32  mensura,
                    s64  tempus,
           EventusCauda* cauda)
{
    (vacuum)interpres;
    redde eventus_caudae_textum_impellere(cauda, tempus, octeti,
        mensura,
        EVENTUS_ORIGO_GLUTINATA) ? I : ZEPHYRUM;
}

i32
interpres_depositio (
    InterpresTerminalis* interpres,
            constans i8* viae,
                    i32  mensura,
                    i32  numerus,
                    s64  tempus,
           EventusCauda* cauda)
{
    Eventus e;
    /* visus vocantis (constans): cauda copiat, non scribit */
    unio { constans i8* l; i8* m; } u;

    u.l = viae;
    memset(&e, ZEPHYRUM, magnitudo(Eventus));
    e.genus                         = EVENTUS_DEPOSITIO;
    e.tempus                        = tempus;
    e.datum.depositio.x             = interpres->indicator_x;
    e.datum.depositio.y             = interpres->indicator_y;
    e.datum.depositio.viae.datum    = u.m;
    e.datum.depositio.viae.mensura  = mensura;
    e.datum.depositio.numerus       = numerus;
    e.datum.depositio.promota       = VERUM;
    redde eventus_caudae_depositionem_impellere(cauda, &e) ? I
                                                           : ZEPHYRUM;
}
