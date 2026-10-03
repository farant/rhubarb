/* probatio_thesaurus.c - thesaurus contentorum (fabrica plan 2 T3):
 * blobi per sigillum, actiones per clavem, generationes, purgatio */
#include "postulata_posix.h"
#include "latina.h"
#include "piscina.h"
#include "credo.h"
#include "chorda.h"
#include "filum.h"
#include "xar.h"
#include "sigillum.h"
#include "thesaurus.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#define RADIX "build/probatio_thesaurus"

/* identitas plagulae (inode): rename novum inode dat - scriptura
 * iterata sic videtur */
interior b32
_identitas (
         constans character* via,
          longus insignatus* identitas_out)
{
    structura stat informatio;

    si (stat(via, &informatio) != ZEPHYRUM)
    {
        redde FALSUM;
    }
    *identitas_out = (longus insignatus)informatio.st_ino;
    redde VERUM;
}

/* octetum unum blobi mutare (corruptio in disco) */
interior vacuum
_corrumpere (
     chorda  via,
    Piscina* piscina)
{
       chorda contentum;
    character terminata[DXII];

    memcpy(terminata, via.datum, via.mensura);
    terminata[via.mensura]  = '\0';
    contentum               = filum_legere_totum(terminata, piscina);
    si (contentum.mensura > ZEPHYRUM)
    {
        contentum.datum[ZEPHYRUM] = (i8)(contentum.datum[ZEPHYRUM] ^ I);
        (vacuum)filum_scribere(terminata, contentum);
    }
}

interior b32
_existit (
    chorda via)
{
    character terminata[DXII];

    memcpy(terminata, via.datum, via.mensura);
    terminata[via.mensura] = '\0';
    redde filum_existit(terminata);
}

#define CONCURSUS_ROTAE CC
#define CONCURSUS_MENSURA (LXIV * MXXIV)

/* buffer rotae r: octeti ex r et indice (rota quaeque blobum novum) */
interior vacuum
_buffer_implere (
    i8* buffer,
    i32 rota)
{
    i32 k;

    per (k = ZEPHYRUM; k < CONCURSUS_MENSURA; k++)
    {
        buffer[k] = (i8)((k * XXXI + rota * VII + k / CCLVI) & 0xFF);
    }
}

/* filius: omnes rotas ponit, quamque statim legit verificatione plena
 * (alter filius eundem blobum fortasse scribit) - exitus = lectiones
 * fractae */
interior s32
_concurrere (
    vacuum)
{
        Piscina* piscina;
      Thesaurus* thesaurus;
             i8* buffer;
         chorda  octeti;
         chorda  via;
       Sigillum  sigillum;
            i32  rota;
            s32  fracta;

    piscina    = piscina_generare_dynamicum("concursus", 1048576);
    thesaurus  = thesaurus_aperire(RADIX, piscina);
    buffer     = (i8*)piscina_allocare(piscina, CONCURSUS_MENSURA);
    si (thesaurus == NIHIL || buffer == NIHIL)
    {
        redde C;
    }
    thesaurus_verificationem_ponere(thesaurus, I);
    fracta = ZEPHYRUM;
    per (rota = ZEPHYRUM; rota < CONCURSUS_ROTAE; rota++)
    {
        _buffer_implere(buffer, rota);
        octeti.datum    = buffer;
        octeti.mensura  = CONCURSUS_MENSURA;
        si (   !thesaurus_ponere(thesaurus, octeti, &sigillum)
            || !thesaurus_via(thesaurus, &sigillum, piscina, &via))
        {
            fracta++;
        }
    }
    redde fracta;
}

/* clavis ficta ex literis (sigillum earum) */
interior Sigillum
_clavis (
    constans character* literae)
{
    redde sigillum_computare(literae, (memoriae_index)strlen(literae));
}

s32 principale (vacuum)
{
            Piscina* piscina;
          Thesaurus* thesaurus;
          Thesaurus* secundus;
           Sigillum  sigillum;
           Sigillum  iterum;
           Sigillum  expectatum;
           Sigillum  clavis_a;
           Sigillum  clavis_b;
           Sigillum  blobus_a;
           Sigillum  blobus_b;
           Sigillum  communis;
             chorda  via;
             chorda  via_a;
             chorda  via_b;
             chorda  via_communis;
             chorda  contentum;
                Xar* exitus;
                Xar* capta;
          character  terminata[DXII];
          character  hex[LXV];
          character  expectata[DXII];
  longus insignatus  identitas_ante;
  longus insignatus  identitas_post;
                i32  deleta;

    piscina = piscina_generare_dynamicum("probatio_thesaurus", 1048576);
    credo_aperire(piscina);
    (vacuum)filum_arborem_delere(RADIX);

    imprimere("\n--- I. aperire: radix creatur ---\n");
    thesaurus = thesaurus_aperire(RADIX, piscina);
    CREDO_NON_NIHIL(thesaurus);
    CREDO_VERUM(filum_directorium_existit(RADIX));

    imprimere("\n--- II. ponere / via: iter rotundum, forma viae ---\n");
    CREDO_VERUM(thesaurus_ponere(thesaurus,
        chorda_ex_literis("salve mundus\n", piscina), &sigillum));
    expectatum = sigillum_computare("salve mundus\n", XIII);
    CREDO_VERUM(sigillum_aequale(&sigillum, &expectatum));
    CREDO_VERUM(thesaurus_via(thesaurus, &sigillum, piscina, &via));
    sigillum_hex(&sigillum, hex);
    sprintf(expectata, "%s/blobi/%.2s/%s", RADIX, hex, hex + II);
    CREDO_VERUM(chorda_aequalis_literis(via, expectata));
    contentum = filum_legere_totum(expectata, piscina);
    CREDO_VERUM(chorda_aequalis_literis(contentum, "salve mundus\n"));

    imprimere("\n--- III. iterum ponere: idem blobus, non rescriptus ---\n");
    CREDO_VERUM(_identitas(expectata, &identitas_ante));
    CREDO_VERUM(thesaurus_ponere(thesaurus,
        chorda_ex_literis("salve mundus\n", piscina), &iterum));
    CREDO_VERUM(sigillum_aequale(&sigillum, &iterum));
    CREDO_VERUM(_identitas(expectata, &identitas_post));
    CREDO_VERUM(identitas_ante == identitas_post);

    imprimere("\n--- IV. actio: clavis -> II exitus; absens FALSUM ---\n");
    CREDO_VERUM(thesaurus_ponere(thesaurus,
        chorda_ex_literis("alpha", piscina), &blobus_a));
    CREDO_VERUM(thesaurus_ponere(thesaurus,
        chorda_ex_literis("beta", piscina), &blobus_b));
    exitus = xar_creare(piscina, (i32)magnitudo(Sigillum));
    *(Sigillum*)xar_addere(exitus) = blobus_a;
    *(Sigillum*)xar_addere(exitus) = blobus_b;
    clavis_a = _clavis("actio prima");
    CREDO_VERUM(thesaurus_actio_ponere(thesaurus, &clavis_a, exitus));
    capta = NIHIL;
    CREDO_VERUM(thesaurus_actio_capere(thesaurus, &clavis_a, piscina,
        &capta));
    CREDO_NON_NIHIL(capta);
    CREDO_AEQUALIS_I32(xar_numerus(capta), II);
    CREDO_VERUM(sigillum_aequale((Sigillum*)xar_obtinere(capta,
        ZEPHYRUM),
        &blobus_a));
    CREDO_VERUM(sigillum_aequale((Sigillum*)xar_obtinere(capta, I),
        &blobus_b));
    clavis_b = _clavis("actio nusquam");
    CREDO_FALSUM(thesaurus_actio_capere(thesaurus, &clavis_b, piscina,
        &capta));
    /* actio eadem iterum: non rescripta */
    sigillum_hex(&clavis_a, hex);
    sprintf(expectata, "%s/actiones/%.2s/%s", RADIX, hex, hex + II);
    CREDO_VERUM(_identitas(expectata, &identitas_ante));
    CREDO_VERUM(thesaurus_actio_ponere(thesaurus, &clavis_a, exitus));
    CREDO_VERUM(_identitas(expectata, &identitas_post));
    CREDO_VERUM(identitas_ante == identitas_post);

    imprimere("\n--- V. blobus corruptus: verificatio delet, FALSUM ---\n");
    CREDO_VERUM(thesaurus_via(thesaurus, &blobus_a, piscina, &via_a));
    _corrumpere(via_a, piscina);
    /* sine verificatione: corruptio non videtur (specimen) */
    thesaurus_verificationem_ponere(thesaurus, ZEPHYRUM);
    CREDO_VERUM(thesaurus_via(thesaurus, &blobus_a, piscina, &via));
    thesaurus_verificationem_ponere(thesaurus, I);
    CREDO_FALSUM(thesaurus_via(thesaurus, &blobus_a, piscina, &via));
    CREDO_FALSUM(_existit(via_a));
    /* sanus sub verificatione: VERUM */
    CREDO_VERUM(thesaurus_via(thesaurus, &blobus_b, piscina, &via_b));

    imprimere("\n--- VI. purgare: generationes ultimae servantur ---\n");
    /* GENERATIO = cursus ordinans (fabrica, porta), non processus:
     * THESAURUS_GENERATIO a tempore UTC incipit (ordo nominum = ordo
     * temporis). Sine ea: nullus index (processus solitarius cursus
     * veros e fenestra non expellit). */
    (vacuum)filum_arborem_delere(RADIX);
    unsetenv("THESAURUS_GENERATIO");
    thesaurus = thesaurus_aperire(RADIX, piscina);
    thesaurus_generationem_notare(thesaurus, &clavis_a);
    sprintf(expectata, "%s/generationes", RADIX);
    CREDO_FALSUM(filum_directorium_existit(expectata));
    setenv("THESAURUS_GENERATIO", "20260101T000000-prima", I);
    thesaurus = thesaurus_aperire(RADIX, piscina);
    CREDO_NON_NIHIL(thesaurus);
    /* generatio prima: clavis_a -> [blobus_a, communis] */
    CREDO_VERUM(thesaurus_ponere(thesaurus,
        chorda_ex_literis("alpha", piscina), &blobus_a));
    CREDO_VERUM(thesaurus_ponere(thesaurus,
        chorda_ex_literis("communis", piscina), &communis));
    exitus = xar_creare(piscina, (i32)magnitudo(Sigillum));
    *(Sigillum*)xar_addere(exitus) = blobus_a;
    *(Sigillum*)xar_addere(exitus) = communis;
    CREDO_VERUM(thesaurus_actio_ponere(thesaurus, &clavis_a, exitus));
    thesaurus_generationem_notare(thesaurus, &clavis_a);
    /* generatio secunda (cursus novus): clavis_b -> [blobus_b,
     * communis]; processus alter eiusdem generationis in indicem
     * eundem scribit */
    setenv("THESAURUS_GENERATIO", "20260102T000000-secunda", I);
    secundus = thesaurus_aperire(RADIX, piscina);
    CREDO_NON_NIHIL(secundus);
    CREDO_VERUM(thesaurus_ponere(secundus,
        chorda_ex_literis("beta", piscina), &blobus_b));
    exitus = xar_creare(piscina, (i32)magnitudo(Sigillum));
    *(Sigillum*)xar_addere(exitus) = blobus_b;
    *(Sigillum*)xar_addere(exitus) = communis;
    CREDO_VERUM(thesaurus_actio_ponere(secundus, &clavis_b, exitus));
    thesaurus_generationem_notare(secundus, &clavis_b);
    thesaurus_generationem_notare(thesaurus_aperire(RADIX, piscina),
        &clavis_b);
    sprintf(expectata, "%s/generationes/20260102T000000-secunda.lst",
        RADIX);
    CREDO_AEQUALIS_I32(filum_legere_totum(expectata, piscina).mensura,
        CXXX);
    unsetenv("THESAURUS_GENERATIO");
    thesaurus_verificationem_ponere(secundus, ZEPHYRUM);
    CREDO_VERUM(thesaurus_via(secundus, &blobus_a, piscina, &via_a));
    CREDO_VERUM(thesaurus_via(secundus, &blobus_b, piscina, &via_b));
    CREDO_VERUM(thesaurus_via(secundus, &communis, piscina,
        &via_communis));
    /* servare I: generatio secunda sola - actio prima, blobus_a et
     * index primus delentur (III); communis servatur */
    deleta = ZEPHYRUM;
    CREDO_VERUM(thesaurus_purgare(secundus, I, FALSUM, &deleta));
    CREDO_AEQUALIS_I32(deleta, III);
    CREDO_FALSUM(_existit(via_a));
    CREDO_VERUM(_existit(via_b));
    CREDO_VERUM(_existit(via_communis));
    CREDO_FALSUM(thesaurus_actio_capere(secundus, &clavis_a, piscina,
        &capta));
    CREDO_VERUM(thesaurus_actio_capere(secundus, &clavis_b, piscina,
        &capta));
    /* verificare: blobus servatus corruptus deletur */
    _corrumpere(via_communis, piscina);
    deleta = ZEPHYRUM;
    CREDO_VERUM(thesaurus_purgare(secundus, I, VERUM, &deleta));
    CREDO_AEQUALIS_I32(deleta, I);
    CREDO_FALSUM(_existit(via_communis));
    CREDO_VERUM(_existit(via_b));
    memcpy(terminata, via_b.datum, via_b.mensura);
    terminata[via_b.mensura] = '\0';
    CREDO_VERUM(chorda_aequalis_literis(
        filum_legere_totum(terminata, piscina), "beta"));

    imprimere("\n--- VII. concursus: II filii, CC rotae, LXIV KiB ---\n");
    (vacuum)filum_arborem_delere(RADIX);
    {
        pid_t  filii[II];
          i32  f;
          s32  status;
          s32  fracta_omnia;
           i8* buffer;
          i32  rota;
          i32  verificata;

        fflush(stdout);
        per (f = ZEPHYRUM; f < II; f++)
        {
            filii[f] = fork();
            si (filii[f] == ZEPHYRUM)
            {
                /* fracta <= CC (rotae): exitus integer */
                _exit((integer)_concurrere());
            }
        }
        fracta_omnia = ZEPHYRUM;
        per (f = ZEPHYRUM; f < II; f++)
        {
            status = ZEPHYRUM;
            (vacuum)waitpid(filii[f], &status, ZEPHYRUM);
            fracta_omnia += WIFEXITED(status) ? WEXITSTATUS(status)
                : C;
        }
        CREDO_AEQUALIS_S32(fracta_omnia, ZEPHYRUM);
        /* post concursum: omnis blobus integer */
        thesaurus = thesaurus_aperire(RADIX, piscina);
        thesaurus_verificationem_ponere(thesaurus, I);
        buffer      = (i8*)piscina_allocare(piscina, CONCURSUS_MENSURA);
        verificata  = ZEPHYRUM;
        per (rota = ZEPHYRUM; rota < CONCURSUS_ROTAE; rota++)
        {
            _buffer_implere(buffer, rota);
            sigillum = sigillum_computare(buffer, CONCURSUS_MENSURA);
            si (thesaurus_via(thesaurus, &sigillum, piscina, &via))
            {
                verificata++;
            }
        }
        CREDO_AEQUALIS_I32(verificata, CONCURSUS_ROTAE);
    }

    credo_imprimere_compendium();
    piscina_destruere(piscina);
    redde credo_omnia_praeterierunt() ? ZEPHYRUM : I;
}
