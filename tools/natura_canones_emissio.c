/* tools/natura_canones_emissio.c - exemplar elementi -> litterae
 *
 * Duo emissores exemplar idem legunt:
 *   -inspicere  -> TSV, pro homine (linea una per membrum)
 *   -modulus    -> canon generatus, pro machina
 * Neuter exemplar mutat: quod natura_canones.c plicavit, hic
 * SOLUM scribitur.
 *
 * Vide project-specs/natura-canones-generati-spec.md.
 */
#include "natura_canones.h"
#include <string.h>

interior vacuum  _kebab_literas_scribere(FILE* f,
                                         constans character* s);
interior vacuum  _planum_scribere(FILE* f, constans chorda* t);

/* nomen naturae (snake) -> nomen canonis (kebab).
 * Bijectivum: genus 'nomen' naturae lineolam non fert. */
vacuum
_kebab_scribere(
    FILE*             f,
    constans chorda*  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t->mensura; i++)
    {
        character c;

        c = (character)t->datum[i];
        si (c == '_')
        {
            c = '-';
        }
        putc(c, f);
    }
}

/* idem pro literis C - praefixa ('status_') formam SNAKE in
 * exemplari servant, ergo kebab uno loco solo fit */
interior vacuum
_kebab_literas_scribere(
    FILE*                f,
    constans character*  s)
{
    i32 i;

    per (i = ZEPHYRUM; s[i] != '\0'; i++)
    {
        putc(s[i] == '_' ? '-' : s[i], f);
    }
}

/* chorda in unam lineam - valor <valor> prosa MULTILINEA esse
 * potest (normalizatio indentationem tollit, lineas servat), quae
 * TSV crudo emissa unum membrum in plures lineas frangeret.
 * Exemplar valorem VERUM tenet; sola haec species eum planat. */
interior vacuum
_planum_scribere(
    FILE*             f,
    constans chorda*  t)
{
    i32 i;

    per (i = ZEPHYRUM; i < t->mensura; i++)
    {
        character c;

        c = (character)t->datum[i];
        si (c == '\n' || c == '\r' || c == '\t')
        {
            c = ' ';
        }
        putc(c, f);
    }
}

/* exemplar in TSV - linea una per membrum:
 *   ENS  discrimen  nomen  genus  optiones  [=ordinarius]
 * Actio breviter: ENS  actio  nomen */
vacuum
_elementum_inspicere(
    FILE*         f,
    NcElementum*  el)
{
    i32 i;

    per (i = ZEPHYRUM; i < xar_numerus(el->membra); i++)
    {
        NcMembrum* m;
        i32        j;

        m = (NcMembrum*)xar_obtinere(el->membra, i);

        _kebab_scribere(f, el->ens->titulus);
        fputs(m->discrimen == NC_MEMBRUM_ATTRIBUTUM
              ? "\tattributum\t" : "\tliberum\t", f);
        si (m->praefixum)
        {
            _kebab_literas_scribere(f, m->praefixum);
        }
        _kebab_scribere(f, m->titulus);
        fprintf(f, "\t%s\t", m->genus_valoris);

        per (j = ZEPHYRUM;
             m->optiones && j < xar_numerus(m->optiones); j++)
        {
            chorda* o;

            o = *(chorda**)xar_obtinere(m->optiones, j);
            si (j > ZEPHYRUM)
            {
                putc('|', f);
            }
            _planum_scribere(f, o);
        }

        si (m->praestitutum)
        {
            fputs("\t=", f);
            _planum_scribere(f, m->praestitutum);
        }
        putc('\n', f);
    }

    per (i = ZEPHYRUM; i < xar_numerus(el->actiones); i++)
    {
        chorda* a;

        a = *(chorda**)xar_obtinere(el->actiones, i);
        _kebab_scribere(f, el->ens->titulus);
        fputs("\tactio\t", f);
        _kebab_scribere(f, a);
        putc('\n', f);
    }
}
