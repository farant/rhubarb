int numerus = 42;
static long longum = -7;
double ratio = 2.5;
float parvum = 1.5f;
int zeri;
int acies_plena[3] = {1, 2, 3};
int acies_partim[4] = {9};
struct Punctum { int x; int y; };
struct Punctum origo_p = {3, 4};
int* ad_numerum = &numerus;
int* ad_elementum = &acies_plena[1];
extern int externum_solum;
const char* salutatio = "salve\n";
char fixum[8] = "ave";

int addens(int k);
int (*tabula_functionum[2])(int) = { addens, 0 };

int addens(int k)
{
    static int comes = 5;
    comes += k;
    return comes + numerus;
}

long legens(int i)
{
    int locales[3] = {1, 2, 3};
    int mixta[3] = {1, i, 0};
    char salve_localis[8] = "io";
    return locales[i] + mixta[0] + salve_localis[0]
        + salutatio[1] + longum + tabula_functionum[0](i);
}
