/* atomus.c - ordo memoriae (vide include/atomus.h)
 *
 * Plagula UNA domus quae __atomic tangit; postulata_atomica.h PRIMUM
 * (silva sectionem lexici ex inclusionibus huius plagulae ipsius
 * derivat). Compilatori sine __atomic hic clamat - numquam via
 * 'volatile' tacita (sonus spec X.7). Vide lib/circulus.worklog.md.
 */
#include "postulata_atomica.h"
#include "atomus.h"

vacuum
atomus_publicare (
    volatilis i32* locus,
              i32  valor)
{
    __atomic_store_n(locus, valor, __ATOMIC_RELEASE);
}

i32
atomus_legere (
    constans volatilis i32* locus)
{
    redde __atomic_load_n(locus, __ATOMIC_ACQUIRE);
}
