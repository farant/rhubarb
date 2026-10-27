/* probatio_sarcina.c - arca ZIP solum legenda (lector L1): directorium
 * centrale, introitus per nomen, arcae malae sine ruina. Arcae in
 * memoria per arcam_struere (scriptor probationis, flatura_deflare);
 * oraculum independens (zip Apple) in sectione 'oraculum' (Task 3). */
#include "latina.h"
#include "piscina.h"
#include "chorda.h"
#include "flatura.h"
#include "filum.h"
#include "sarcina.h"
#include "credo.h"
#include <string.h>

/* vexilla arcae probationis */
/* mensurae centrales 0xFFFFFFFF (signum ZIP64) */
#define ARCA_AMPLA          I
/* vexillum III: mensurae et CRC in capite locali 0, post data in
 * descriptore (0x08074b50) et in directorio */
#define ARCA_DESCRIPTOR     II
#define ARCA_CRC_MALUM      IV     /* CRC centrale mutatum */
#define ARCA_MODUS_IGNOTUS  VIII   /* modus XII (bzip2) in directorio */
#define ARCA_CRYPTA         XVI    /* vexillum 0 (cryptographia) */
#define ARCA_POSITIO_MALA   XXXII  /* caput locale ultra finem */
#define ARCA_MENSURA_MALA   LXIV   /* mensura compressa ultra finem */

hic_manens constans character GENUS_LIBRI[] = "application/epub+zip";

interior vacuum
binos_scribere (
      i8* b,
     i32* k,
     i32  v)
{
    b[(*k)++] = (i8)(v & 0xFFu);
    b[(*k)++] = (i8)((v >> VIII) & 0xFFu);
}

interior vacuum
quaternos_scribere (
      i8* b,
     i32* k,
     i32  v)
{
    binos_scribere(b, k, v & 0xFFFFu);
    binos_scribere(b, k, (v >> XVI) & 0xFFFFu);
}

/* arca ZIP in memoria: capita localia + data (DEFLATE si comprimere,
 * praeter directoria et "mimetype"), directorium centrale,
 * EOCD cum commento; vexilla ARCA_* */
interior chorda
arcam_struere (
                       Piscina* p,
    constans character* constans* nomina,
    constans character* constans* data,
                           i32  n,
                           b32  comprimere,
             constans character* commentum,
                           i32  vexilla)
{
                i8* b;
               i32  k;
               i32  i;
               i32  positiones[XVI];
               i32  modi[XVI];
               i32  crc[XVI];
               i32  compressae[XVI];
               i32  explicitae[XVI];
               i32  initium_centrale;
               i32  longitudo;
    FlaturaFructus  f;
       constans i8* fons;
               i32  longitudo_fontis;
            chorda  fructus;

    b = (i8*)piscina_allocare(p, CXXVIII * M);
    k = ZEPHYRUM;
    per (i = ZEPHYRUM; i < n; i++)
    {
        longitudo      = (i32)strlen(data[i]);
        positiones[i]  = k;
        explicitae[i]  = longitudo;
        crc[i] = flatura_crc32((constans i8*)data[i],
            longitudo);
        /* ut EPUB: "mimetype" et directoria semper reposita */
        modi[i]        = (   comprimere
                          && nomina[i][strlen(nomina[i]) - I] != '/'
                          && strcmp(nomina[i], "mimetype") != ZEPHYRUM)
                       ? VIII : ZEPHYRUM;
        fons              = (constans i8*)data[i];
        longitudo_fontis  = longitudo;
        si (modi[i] == VIII)
        {
            f = flatura_deflare(fons, longitudo,
                FLATURA_COMPRESSIO_ORDINARIA, p);
            fons              = f.datum;
            longitudo_fontis  = f.mensura;
        }
        compressae[i] = longitudo_fontis;
        quaternos_scribere(b, &k, 0x04034b50u);
        binos_scribere(b, &k, XX);
        binos_scribere(b, &k, (vexilla & ARCA_DESCRIPTOR) ? VIII
            : ZEPHYRUM);
        binos_scribere(b, &k, modi[i]);
        binos_scribere(b, &k, ZEPHYRUM);
        binos_scribere(b, &k, ZEPHYRUM);
        quaternos_scribere(b, &k, (vexilla & ARCA_DESCRIPTOR) ? ZEPHYRUM
            : crc[i]);
        quaternos_scribere(b, &k, (vexilla & ARCA_DESCRIPTOR) ? ZEPHYRUM
            : compressae[i]);
        quaternos_scribere(b, &k, (vexilla & ARCA_DESCRIPTOR) ? ZEPHYRUM
            : explicitae[i]);
        binos_scribere(b, &k, (i32)strlen(nomina[i]));
        binos_scribere(b, &k, ZEPHYRUM);
        memcpy(b + k, nomina[i], strlen(nomina[i]));
        k += (i32)strlen(nomina[i]);
        memcpy(b + k, fons, (size_t)longitudo_fontis);
        k += longitudo_fontis;
        si (vexilla & ARCA_DESCRIPTOR)
        {
            quaternos_scribere(b, &k, 0x08074b50u);
            quaternos_scribere(b, &k, crc[i]);
            quaternos_scribere(b, &k, compressae[i]);
            quaternos_scribere(b, &k, explicitae[i]);
        }
    }
    initium_centrale = k;
    per (i = ZEPHYRUM; i < n; i++)
    {
        quaternos_scribere(b, &k, 0x02014b50u);
        binos_scribere(b, &k, XX);
        binos_scribere(b, &k, XX);
        binos_scribere(b, &k, ((vexilla & ARCA_DESCRIPTOR) ? VIII
            : ZEPHYRUM) | ((vexilla & ARCA_CRYPTA) ? I : ZEPHYRUM));
        binos_scribere(b, &k, (vexilla & ARCA_MODUS_IGNOTUS) ? XII
            : modi[i]);
        binos_scribere(b, &k, ZEPHYRUM);
        binos_scribere(b, &k, ZEPHYRUM);
        quaternos_scribere(b, &k, (vexilla & ARCA_CRC_MALUM) ? crc[i]
            ^ I : crc[i]);
        quaternos_scribere(b, &k, (vexilla & ARCA_AMPLA) ? 0xFFFFFFFFu
            : (vexilla & ARCA_MENSURA_MALA) ? 0x7FFFFFF0u
            : compressae[i]);
        quaternos_scribere(b, &k, (vexilla & ARCA_AMPLA) ? 0xFFFFFFFFu
            : explicitae[i]);
        binos_scribere(b, &k, (i32)strlen(nomina[i]));
        binos_scribere(b, &k, ZEPHYRUM);
        binos_scribere(b, &k, ZEPHYRUM);
        binos_scribere(b, &k, ZEPHYRUM);
        binos_scribere(b, &k, ZEPHYRUM);
        quaternos_scribere(b, &k, ZEPHYRUM);
        quaternos_scribere(b, &k, (vexilla & ARCA_POSITIO_MALA)
            ? 0x7FFFFFF0u : positiones[i]);
        memcpy(b + k, nomina[i], strlen(nomina[i]));
        k += (i32)strlen(nomina[i]);
    }
    longitudo = commentum ? (i32)strlen(commentum) : ZEPHYRUM;
    quaternos_scribere(b, &k, 0x06054b50u);
    binos_scribere(b, &k, ZEPHYRUM);
    binos_scribere(b, &k, ZEPHYRUM);
    binos_scribere(b, &k, n);
    binos_scribere(b, &k, n);
    quaternos_scribere(b, &k, k - initium_centrale - IV);
    quaternos_scribere(b, &k, initium_centrale);
    binos_scribere(b, &k, longitudo);
    si (longitudo > ZEPHYRUM)
    {
        memcpy(b + k, commentum, (size_t)longitudo);
        k += longitudo;
    }
    fructus.datum    = b;
    fructus.mensura  = k;
    redde fructus;
}

hic_manens constans character* constans NOMINA_DUO[] = {
    "mimetype", "OEBPS/c1.xhtml"
};
hic_manens constans character* constans DATA_DUO[] = {
    GENUS_LIBRI, "<p>Cranford</p>"
};

/* arca mala: NIHIL et vitium non vacuum (numquam ruina) */
interior b32
recusata (
      chorda  arca,
     Piscina* p,
      chorda* vitium)
{
    vitium->datum    = NIHIL;
    vitium->mensura  = ZEPHYRUM;
    redde sarcina_aperire(arca, p, vitium) == NIHIL
        && vitium->mensura > ZEPHYRUM;
}

interior vacuum
_sectio_directorium (
    CredoContextus* c)
{
                       Piscina* p;
                       Sarcina* s;
     constans SarcinaIntroitus* e;
                        chorda  arca;
                        chorda  vitium;
                        chorda  mala;
    constans character* constans dir_nomina[] = { "OEBPS/",
        "OEBPS/c1.xhtml" };
    constans character* constans dir_data[] = { "", "<p>x</p>" };
                     character x[C];

    p = c->piscina;

    CREDO_GRADUS("duo introitus: repositus et DEFLATE");
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM, NIHIL,
        ZEPHYRUM);
    s = sarcina_aperire(arca, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_AEQUALIS_I32(sarcina_numerus(s), II);
    e = sarcina_introitus(s, ZEPHYRUM);
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_CHORDA_AEQUALIS_LITERIS(e->titulus, "mimetype");
    CREDO_AEQUALIS_I32(e->modus, ZEPHYRUM);
    CREDO_AEQUALIS_I32(e->mensura, XX);
    e = sarcina_invenire(s, chorda_ex_literis("OEBPS/c1.xhtml", p));
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_AEQUALIS_I32(e->modus, VIII);
    CREDO_AEQUALIS_I32(e->mensura, XV);
    CREDO_NIHIL(sarcina_invenire(s, chorda_ex_literis("absens", p)));
    CREDO_NIHIL(sarcina_introitus(s, II));

    CREDO_GRADUS("commentum in fine (EOCD non ultimi XXII octeti)");
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM,
        "commentum quadraginta octetorum longum!!", ZEPHYRUM);
    s = sarcina_aperire(arca, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_AEQUALIS_I32(sarcina_numerus(s), II);

    CREDO_GRADUS("introitus directorii");
    arca = arcam_struere(p, dir_nomina, dir_data, II, VERUM, NIHIL,
        ZEPHYRUM);
    s = sarcina_aperire(arca, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_VERUM(sarcina_introitus(s, ZEPHYRUM)->directorium);
    CREDO_FALSUM(sarcina_introitus(s, I)->directorium);

    CREDO_GRADUS("arcae malae: vitium, nulla ruina");
    mala.datum    = NIHIL;
    mala.mensura  = ZEPHYRUM;
    CREDO_VERUM(recusata(mala, p, &vitium));
    memset(x, 'x', magnitudo(x));
    mala.datum    = (i8*)x;
    mala.mensura  = C;
    CREDO_VERUM(recusata(mala, p, &vitium));
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM, NIHIL,
        ZEPHYRUM);
    mala.datum    = arca.datum;
    mala.mensura  = arca.mensura - X;
    CREDO_VERUM(recusata(mala, p, &vitium));
    /* EOCD: offset directorii (octeti XVI..XIX a signo) ultra finem */
    arca.datum[arca.mensura - VI]   = 0xFFu;
    arca.datum[arca.mensura - V]    = 0xFFu;
    arca.datum[arca.mensura - IV]   = 0xFFu;
    arca.datum[arca.mensura - III]  = 0x7Fu;
    CREDO_VERUM(recusata(arca, p, &vitium));

    CREDO_GRADUS("ZIP64 (mensurae 0xFFFFFFFF): recusatur, nominatur");
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM, NIHIL,
        ARCA_AMPLA);
    CREDO_VERUM(recusata(arca, p, &vitium));
    CREDO_VERUM(chorda_continet(vitium, chorda_ex_literis("ZIP64", p)));
}


/* data introitus: repositus, DEFLATE magnus, descriptor; vitia: CRC,
 * modus ignotus, cryptographia, fines mali */
interior b32
introitum_legere (
                 chorda  arca,
     constans character* titulus,
                Piscina* p,
                 chorda* exitus,
                 chorda* vitium)
{
                       Sarcina* s;
     constans SarcinaIntroitus* e;

    vitium->datum = NIHIL;
    vitium->mensura = ZEPHYRUM;
    s = sarcina_aperire(arca, p, vitium);
    e = s ? sarcina_invenire(s, chorda_ex_literis(titulus, p)) : NIHIL;
    redde e && sarcina_legere(s, e, p, exitus, vitium);
}

interior vacuum
_sectio_legere (
    CredoContextus* c)
{
                       Piscina* p;
                        chorda  arca;
                        chorda  exitus;
                        chorda  vitium;
                     character* magnus;
    constans character* constans magna_nomina[] = { "magnus.txt" };
            constans character* magna_data[I];
                           i32  i;

    p = c->piscina;

    CREDO_GRADUS("repositus: mimetype exactus");
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM, NIHIL,
        ZEPHYRUM);
    CREDO_VERUM(introitum_legere(arca, "mimetype", p, &exitus,
        &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(exitus, GENUS_LIBRI);
    CREDO_VERUM(introitum_legere(arca, "OEBPS/c1.xhtml", p, &exitus,
        &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(exitus, "<p>Cranford</p>");

    CREDO_GRADUS("DEFLATE: V M octeti redeunt");
    magnus = (character*)piscina_allocare(p, V * M + I);
    per (i = ZEPHYRUM; i < V * M; i++)
    {
        magnus[i] =
            "In the first place, Cranford is in possession of the "
            "Amazons. "[i % LX];
    }
    magnus[V * M]         = '\0';
    magna_data[ZEPHYRUM]  = magnus;
    arca = arcam_struere(p, magna_nomina, magna_data, I, VERUM, NIHIL,
        ZEPHYRUM);
    CREDO_VERUM(introitum_legere(arca, "magnus.txt", p, &exitus,
        &vitium));
    CREDO_AEQUALIS_I32(exitus.mensura, V * M);
    CREDO_VERUM(memcmp(exitus.datum, magnus, V * M) == ZEPHYRUM);

    CREDO_GRADUS("descriptor datorum: mensurae ex directorio");
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM, NIHIL,
        ARCA_DESCRIPTOR);
    CREDO_VERUM(introitum_legere(arca, "OEBPS/c1.xhtml", p, &exitus,
        &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(exitus, "<p>Cranford</p>");
    CREDO_VERUM(introitum_legere(arca, "mimetype", p, &exitus,
        &vitium));
    CREDO_CHORDA_AEQUALIS_LITERIS(exitus, GENUS_LIBRI);

    CREDO_GRADUS("CRC malum");
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM, NIHIL,
        ARCA_CRC_MALUM);
    CREDO_FALSUM(introitum_legere(arca, "OEBPS/c1.xhtml", p, &exitus,
        &vitium));
    CREDO_VERUM(chorda_continet(vitium, chorda_ex_literis("CRC", p)));

    CREDO_GRADUS("modus XII et cryptographia: recusantur, nominantur");
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM, NIHIL,
        ARCA_MODUS_IGNOTUS);
    CREDO_FALSUM(introitum_legere(arca, "OEBPS/c1.xhtml", p, &exitus,
        &vitium));
    CREDO_VERUM(chorda_continet(vitium, chorda_ex_literis("12", p)));
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM, NIHIL,
        ARCA_CRYPTA);
    CREDO_FALSUM(introitum_legere(arca, "OEBPS/c1.xhtml", p, &exitus,
        &vitium));
    CREDO_VERUM(chorda_continet(vitium, chorda_ex_literis("encrypted",
        p)));

    CREDO_GRADUS("fines mali: caput et mensura ultra finem");
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM, NIHIL,
        ARCA_POSITIO_MALA);
    CREDO_FALSUM(introitum_legere(arca, "OEBPS/c1.xhtml", p, &exitus,
        &vitium));
    CREDO_VERUM(vitium.mensura > ZEPHYRUM);
    arca = arcam_struere(p, NOMINA_DUO, DATA_DUO, II, VERUM, NIHIL,
        ARCA_MENSURA_MALA);
    CREDO_FALSUM(introitum_legere(arca, "OEBPS/c1.xhtml", p, &exitus,
        &vitium));
    CREDO_VERUM(vitium.mensura > ZEPHYRUM);
}


/* oraculum independens: probationes/fixa/sarcina/exemplum.zip ab
 * instrumento Apple factum (non a scriptore nostro), ex fons/:
 *   cd fons && zip -0 exemplum.zip mimetype
 *           && zip -r exemplum.zip OEBPS
 * SINE -X: zip extra campos scribit, LOCALES (XXVIII octeti) longiores
 * quam CENTRALES (XXIV) - initium datorum ex longitudine LOCALI */
interior vacuum
_sectio_oraculum (
    CredoContextus* c)
{
                       Piscina* p;
                       Sarcina* s;
     constans SarcinaIntroitus* e;
                        chorda  arca;
                        chorda  exitus;
                        chorda  vitium;

    p = c->piscina;
    arca = filum_legere_totum("probationes/fixa/sarcina/exemplum.zip",
        p);
    CREDO_NECESSE(arca.mensura > ZEPHYRUM);
    s = sarcina_aperire(arca, p, &vitium);
    CREDO_NECESSE_NON_NIHIL(s);
    CREDO_AEQUALIS_I32(sarcina_numerus(s), III);

    CREDO_GRADUS("mimetype repositus, idem ac fons");
    e = sarcina_invenire(s, chorda_ex_literis("mimetype", p));
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_AEQUALIS_I32(e->modus, ZEPHYRUM);
    CREDO_VERUM(sarcina_legere(s, e, p, &exitus, &vitium));
    CREDO_VERUM(chorda_aequalis(exitus, filum_legere_totum(
        "probationes/fixa/sarcina/fons/mimetype", p)));

    CREDO_GRADUS("OEBPS/ directorium");
    e = sarcina_invenire(s, chorda_ex_literis("OEBPS/", p));
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_VERUM(e->directorium);

    CREDO_GRADUS("c1.xhtml DEFLATE, idem ac fons");
    e = sarcina_invenire(s, chorda_ex_literis("OEBPS/c1.xhtml", p));
    CREDO_NECESSE_NON_NIHIL(e);
    CREDO_AEQUALIS_I32(e->modus, VIII);
    CREDO_VERUM(sarcina_legere(s, e, p, &exitus, &vitium));
    CREDO_VERUM(chorda_aequalis(exitus, filum_legere_totum(
        "probationes/fixa/sarcina/fons/OEBPS/c1.xhtml", p)));
}

hic_manens constans CredoSectio SECTIONES[] = {
    { "directorium", _sectio_directorium, NIHIL, NIHIL, NIHIL, NIHIL },
    { "legere", _sectio_legere, NIHIL, NIHIL, NIHIL, NIHIL },
    { "oraculum", _sectio_oraculum, NIHIL, NIHIL, NIHIL, NIHIL },
    { NIHIL, NIHIL, NIHIL, NIHIL, NIHIL, NIHIL }
};

s32
principale (vacuum)
{
    redde credo_suitam_currere("sarcina", SECTIONES);
}
