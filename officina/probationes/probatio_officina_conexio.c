/* probatio_officina_conexio.c - Conexio: nexor moduli (M2a)
 *
 * Duo moduli per fabricam: A definit $adiutor (functio) + $tabula
 * (datum cum relocationibus ad functionem ET ad datum externum cum
 * addendo) + $main; B definit $verba (datum). A refert stderr/errno
 * (cellae) + $ignotus (decipula); B refert $adiutor EXTERNUM
 * (identitas trans modulos). Praeterea: definitio duplex fractura,
 * via lectoris (textus -> modulus -> nexus). */

#include "officina_conexio.h"
#include "officina_medulla_textus.h"
#include "credo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

interior chorda
_plagulam_legere (
               Piscina* piscina,
    constans character* via_partis)
{
    character via[CCLVI];
    FILE* plagula;
    chorda fructus;
    long longitudo;
    constans character* radix = getenv("RHUBARB_RADIX");

    fructus.datum    = NIHIL;
    fructus.mensura  = ZEPHYRUM;
    sprintf(via, "%s/%s", (radix != NIHIL) ? radix : ".",
        via_partis);
    plagula = fopen(via, "rb");
    si (plagula == NIHIL)
    {
        redde fructus;
    }
    fseek(plagula, 0L, SEEK_END);
    longitudo = ftell(plagula);
    fseek(plagula, 0L, SEEK_SET);
    si (longitudo > 0L)
    {
        fructus.datum = (i8*)piscina_allocare(piscina,
            (memoriae_index)longitudo);
        si (   fructus.datum != NIHIL
            && fread(fructus.datum, I, (memoriae_index)longitudo,
                   plagula) == (memoriae_index)longitudo)
        {
            fructus.mensura = (i32)longitudo;
        }
        alioquin
        {
            fructus.datum = NIHIL;
        }
    }
    fclose(plagula);
    redde fructus;
}

interior chorda
_ch (
    constans character* literis)
{
    chorda c;
    unio { constans character* c; i8* m; } u;

    u.c        = literis;
    c.datum    = u.m;
    c.mensura  = (i32)strlen(literis);
    redde c;
}

interior MedullaInstructio
_instructio (
                 s32 op,
                 s32 typus,
                 s32 destinatio,
    MedullaOperandum a,
    MedullaOperandum b)
{
    MedullaInstructio instructio;

    memset(&instructio, ZEPHYRUM, magnitudo(MedullaInstructio));
    instructio.op              = op;
    instructio.typus           = typus;
    instructio.typus_secundus  = MEDULLA_TYPUS_NIHIL;
    instructio.destinatio      = destinatio;
    instructio.extra_index     = -I;
    instructio.extra_numerus   = ZEPHYRUM;
    instructio.a               = a;
    instructio.b               = b;
    instructio.c               = medulla_op_nihil();
    instructio.origo           = NIHIL;
    redde instructio;
}

/* functio minima terminata: $titulus () -> s32 { redde 0 } */
interior vacuum
_functionem_minimam (
        MedullaModulus* modulus,
    constans character* titulus)
{
    MedullaFunctio* functio = medulla_functionem_creare(modulus,
        _ch(titulus), MEDULLA_TYPUS_S32, FALSUM);
    s32 bloccus = medulla_bloccum_creare(functio, _ch("initium"));
    MedullaInstructio instructio = _instructio(MEDULLA_OP_REDDE,
        MEDULLA_TYPUS_NIHIL, -I, medulla_op_immediatum(ZEPHYRUM),
        medulla_op_nihil());

    medulla_emittere(functio, bloccus, &instructio);
}

s32
principale (vacuum)
{
    Piscina* piscina;
        b32  praeteritus;

    piscina = piscina_generare_dynamicum("probatio_conexio",
        16777216);
    si (!piscina)
    {
        imprimere("FRACTA: piscina\n");
        redde I;
    }
    credo_aperire(piscina);


    /* ========================================================
     * PROBARE: nexus duorum modulorum - totum iter
     * ======================================================== */

    {
                 Regio* regio    = regio_generare(piscina);
               Conexio* conexio  = conexio_creare(piscina, regio);
        MedullaModulus* modulus_a;
        MedullaModulus* modulus_b;
                   s32  sym_adiutor_a;
                   s32  sym_verba_a;
                   s32  sym_adiutor_b;
                    i8  addendum_octeti[8];

        imprimere("\n--- Probans nexum duorum modulorum ---\n");
        CREDO_NON_NIHIL (regio);
        CREDO_NON_NIHIL (conexio);

        /* modulus A */
        modulus_a = medulla_modulum_creare(piscina, _ch("a.c"));
        _functionem_minimam(modulus_a, "adiutor");
        _functionem_minimam(modulus_a, "main");
        sym_adiutor_a = medulla_symbolum_internare(modulus_a,
            _ch("adiutor"));
        sym_verba_a = medulla_symbolum_internare(modulus_a,
            _ch("verba"));
        medulla_symbolum_internare(modulus_a, _ch("stderr"));
        medulla_symbolum_internare(modulus_a, _ch("errno"));
        medulla_symbolum_internare(modulus_a, _ch("ignotus"));

        /* $tabula: [0] = &adiutor (addendum 0); [8] = &verba + 4 */
        {
            MedullaDatum* datum = medulla_datum_creare(modulus_a,
                _ch("tabula"), XVI, VIII);

            CREDO_NON_NIHIL (datum);
            memset(addendum_octeti, ZEPHYRUM, VIII);
            addendum_octeti[ZEPHYRUM] = IV;
            CREDO_VERUM (medulla_datum_scribere(datum, VIII,
                addendum_octeti, VIII));
            CREDO_VERUM (medulla_relocationem_addere(datum, ZEPHYRUM,
                sym_adiutor_a));
            CREDO_VERUM (medulla_relocationem_addere(datum, VIII,
                sym_verba_a));
        }

        /* modulus B: definit $verba, refert $adiutor externum */
        modulus_b = medulla_modulum_creare(piscina, _ch("b.c"));
        sym_adiutor_b = medulla_symbolum_internare(modulus_b,
            _ch("adiutor"));
        {
            MedullaDatum* datum = medulla_datum_creare(modulus_b,
                _ch("verba"), VIII, I);
            i8 salve[6];

            memcpy(salve, "salve", 6);
            CREDO_NON_NIHIL (datum);
            CREDO_VERUM (medulla_datum_scribere(datum, ZEPHYRUM,
                salve, VI));
        }

        /* addere + nectere */
        CREDO_VERUM (conexio_modulum_addere(conexio, modulus_a));
        CREDO_VERUM (conexio_modulum_addere(conexio, modulus_b));
        CREDO_VERUM (conexio_nectere(conexio));
        CREDO_AEQUALIS_I32 (conexio_numerus_modulorum(conexio), II);

        /* identitas trans modulos: adiutor A == adiutor B */
        CREDO_AEQUALIS_S32 (
            conexio_symbolum_globale(conexio, ZEPHYRUM, sym_adiutor_a),
            conexio_symbolum_globale(conexio, I, sym_adiutor_b));

        /* $main inventum, genus FUNCTIO (vectis M2a) */
        {
            s32 index = conexio_symbolum_quaerere(conexio,
                _ch("main"));
            constans ConexioSymbolum* symbolum;

            CREDO_VERUM (index >= ZEPHYRUM);
            symbolum = conexio_symbolum_obtinere(conexio, index);
            CREDO_AEQUALIS_S32 (symbolum->genus,
                CONEXIO_SYMBOLUM_FUNCTIO);
            CREDO_VERUM (regio_continet(regio, symbolum->sedes));
        }

        /* descriptor $adiutor: signum + functio recta */
        {
            vacuum* sedes = conexio_sedes_quaerere(conexio,
                _ch("adiutor"));
             constans ConexioDescriptor* descriptor = sedes;
            constans ConexioFunctioNexa* nexa;

            CREDO_NON_NIHIL (sedes);
            CREDO_VERUM (descriptor->signum
                == CONEXIO_SIGNUM_INTERPRETATUM);
            nexa = conexio_functionem_obtinere(conexio,
                descriptor->index);
            CREDO_NON_NIHIL (nexa);
            CREDO_VERUM (chorda_aequalis_literis(
                nexa->functio->titulus, "adiutor"));
            CREDO_AEQUALIS_S32 (nexa->modulus_index, ZEPHYRUM);
        }

        /* $verba collocatum, octeti copiati */
        {
            i8* sedes = conexio_sedes_quaerere(conexio, _ch("verba"));

            CREDO_NON_NIHIL (sedes);
            CREDO_VERUM (regio_continet(regio, sedes));
            CREDO_VERUM (memcmp(sedes, "salve", V) == ZEPHYRUM);
        }

        /* $tabula: relocationes ADDITIVE sarcitae */
        {
            i8* tabula = conexio_sedes_quaerere(conexio,
                _ch("tabula"));
            vacuum* adiutor_sedes = conexio_sedes_quaerere(conexio,
                _ch("adiutor"));
            i8* verba_sedes = conexio_sedes_quaerere(conexio,
                _ch("verba"));
            s64 locellus;

            CREDO_NON_NIHIL (tabula);
            memcpy(&locellus, tabula, VIII);
            CREDO_VERUM (locellus
                == (s64)(memoriae_index)adiutor_sedes);
            memcpy(&locellus, tabula + VIII, VIII);
            CREDO_VERUM (locellus
                == (s64)(memoriae_index)(verba_sedes + IV));
        }

        /* cellae: stderr ansa 2, errno 0 */
        {
            s64* cella = conexio_sedes_quaerere(conexio,
                _ch("stderr"));
            s32 index;

            CREDO_NON_NIHIL (cella);
            CREDO_VERUM (*cella == II);
            index = conexio_symbolum_quaerere(conexio, _ch("stderr"));
            CREDO_AEQUALIS_S32 (
                conexio_symbolum_obtinere(conexio, index)->genus,
                CONEXIO_SYMBOLUM_CELLA);

            cella = conexio_sedes_quaerere(conexio, _ch("errno"));
            CREDO_NON_NIHIL (cella);
            CREDO_VERUM (*cella == ZEPHYRUM);
            CREDO_AEQUALIS_I32 (conexio_numerus_cellarum(conexio),
                II);
        }

        /* decipula: numerata + nominata; descriptor signatus */
        {
            constans ConexioDescriptor* descriptor;

            CREDO_AEQUALIS_I32 (conexio_numerus_decipularum(conexio),
                I);
            CREDO_VERUM (chorda_aequalis_literis(
                *conexio_decipulam_obtinere(conexio, ZEPHYRUM),
                "ignotus"));
            descriptor = conexio_sedes_quaerere(conexio,
                _ch("ignotus"));
            CREDO_NON_NIHIL (descriptor);
            CREDO_VERUM (descriptor->signum
                == CONEXIO_SIGNUM_DECIPULA);
        }

        /* census */
        CREDO_AEQUALIS_I32 (conexio_numerus_functionum(conexio), II);
        CREDO_AEQUALIS_I32 (conexio_numerus_datorum(conexio), II);

        /* absens -> NIHIL / -I */
        CREDO_NIHIL (conexio_sedes_quaerere(conexio, _ch("absens")));
        CREDO_VERUM (conexio_symbolum_quaerere(conexio, _ch("absens"))
            == -I);

        /* post nectere: addere recusatur */
        CREDO_FALSUM (conexio_modulum_addere(conexio, modulus_a));

        regio_destruere(regio);
    }


    /* ========================================================
     * PROBARE: definitio duplex - fractura honesta
     * ======================================================== */

    {
                 Regio* regio    = regio_generare(piscina);
               Conexio* conexio  = conexio_creare(piscina, regio);
        MedullaModulus* modulus_a = medulla_modulum_creare(piscina,
            _ch("a.c"));
        MedullaModulus* modulus_b = medulla_modulum_creare(piscina,
            _ch("b.c"));

        imprimere("\n--- Probans definitionem duplicem ---\n");
        _functionem_minimam(modulus_a, "f");
        _functionem_minimam(modulus_b, "f");

        CREDO_VERUM (conexio_modulum_addere(conexio, modulus_a));
        CREDO_FALSUM (conexio_modulum_addere(conexio, modulus_b));
        CREDO_VERUM (conexio_querela(conexio)->mensura > ZEPHYRUM);

        regio_destruere(regio);
    }


    /* ========================================================
     * PROBARE: via lectoris - textus -> modulus -> nexus
     * ======================================================== */

    {
                     Regio* regio    = regio_generare(piscina);
                   Conexio* conexio  = conexio_creare(piscina, regio);
            MedullaModulus* modulus;
                       i32  linea_erroris;
        constans character* textus =
            "modulus \"summa.medulla\"\n"
            "\n"
            "functio $summa (s32 %n) -> s32\n"
            "@initium:\n"
            "    %s = movere.s32 0\n"
            "    %i = movere.s32 1\n"
            "    salire @proba\n"
            "@proba:\n"
            "    %c = maior.s32 %i, %n\n"
            "    ramus %c -> @finis | @corpus\n"
            "@corpus:\n"
            "    %s = addere.s32 %s, %i\n"
            "    %i = addere.s32 %i, 1\n"
            "    salire @proba\n"
            "@finis:\n"
            "    redde %s\n";

        imprimere("\n--- Probans viam lectoris ---\n");
        modulus = medulla_textum_legere(piscina, _ch(textus),
            &linea_erroris);
        CREDO_NON_NIHIL (modulus);
        CREDO_VERUM (conexio_modulum_addere(conexio, modulus));
        CREDO_VERUM (conexio_nectere(conexio));

        {
            constans ConexioDescriptor* descriptor =
                conexio_sedes_quaerere(conexio, _ch("summa"));

            CREDO_NON_NIHIL (descriptor);
            CREDO_VERUM (descriptor->signum
                == CONEXIO_SIGNUM_INTERPRETATUM);
        }
        CREDO_AEQUALIS_I32 (conexio_numerus_decipularum(conexio),
            ZEPHYRUM);

        regio_destruere(regio);
    }


    /* ========================================================
     * PROBARE: fixturae machinulae - onerantur + nectuntur
     * (exsecutio ad M2b; hic vectis M2a: ONERATUR)
     * ======================================================== */

    {
        constans character* fixturae[] = {
            "officina/probationes/fixa/machinula/arithmetica.medulla",
            "officina/probationes/fixa/machinula/bitalia.medulla",
            "officina/probationes/fixa/machinula/comparationes.medulla",
            "officina/probationes/fixa/machinula/conversiones.medulla",
            "officina/probationes/fixa/machinula/memoria.medulla",
            "officina/probationes/fixa/machinula/fluxus.medulla",
            "officina/probationes/fixa/machinula/vocationes.medulla",
            "officina/probationes/fixa/machinula/data.medulla",
            "officina/probationes/fixa/machinula/sistere.medulla"
        };
        i32 numerus_fixturarum = IX;
        i32 i;

        imprimere("\n--- Probans fixturas machinulae (oneratio) ---\n");
        per (i = ZEPHYRUM; i < numerus_fixturarum; i += I)
        {
            chorda textus = _plagulam_legere(piscina, fixturae[i]);
            MedullaModulus* modulus;
            i32 linea_erroris = ZEPHYRUM;
            Regio* regio;
            Conexio* conexio;

            CREDO_VERUM (textus.mensura > ZEPHYRUM);
            modulus = medulla_textum_legere(piscina, textus,
                &linea_erroris);
            si (modulus == NIHIL)
            {
                imprimere("FRACTA fixtura %s (linea %d)\n",
                    fixturae[i], (integer)linea_erroris);
            }
            CREDO_NON_NIHIL (modulus);
            si (modulus == NIHIL)
            {
                perge;
            }

            regio    = regio_generare(piscina);
            conexio  = conexio_creare(piscina, regio);
            CREDO_VERUM (conexio_modulum_addere(conexio, modulus));
            CREDO_VERUM (conexio_nectere(conexio));
            CREDO_NON_NIHIL (conexio_sedes_quaerere(conexio,
                _ch("main")));
            CREDO_AEQUALIS_I32 (
                conexio_numerus_decipularum(conexio), ZEPHYRUM);
            regio_destruere(regio);
        }
    }

    credo_imprimere_compendium();
    praeteritus = credo_omnia_praeterierunt();
    piscina_destruere(piscina);

    si (praeteritus)
    {
        redde ZEPHYRUM;
    }
    redde I;
}
