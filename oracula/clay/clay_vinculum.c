/* clay_vinculum.c - vinculum C99 ad Clay (oraculum dispositionis).
 * Ratio in clay_vinculum.h. Compilatur RELAXATE (-std=c99), ut
 * vendicata; clay.h ex ../clay @ e6cc369. */
#define CLAY_IMPLEMENTATION
#include "clay.h"
#include "clay_vinculum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int errores = 0;
static int (*mensor_ductoris)(const char*, int) = NULL;

static Clay_Dimensions textum_metiri(Clay_StringSlice textus,
                                     Clay_TextElementConfig* forma,
                                     void* usus)
{
    Clay_Dimensions m;
    (void)forma;
    (void)usus;
    m.width = mensor_ductoris
        ? (float)mensor_ductoris(textus.chars, (int)textus.length)
        : (float)textus.length;
    m.height = 1.0f;
    return m;
}

void vinculum_mensorem_ponere(int (*mensor)(const char*, int))
{
    mensor_ductoris = mensor;
}

void vinculum_textum(const char* octeti, int mensura)
{
    Clay_String s;
    Clay_TextElementConfig forma;
    memset(&forma, 0, sizeof forma);
    forma.wrapMode = CLAY_TEXT_WRAP_NONE;
    s.isStaticallyAllocated = false;
    s.length = mensura;
    s.chars = octeti;
    Clay__OpenTextElement(s, forma);
}

static void errorem_tractare(Clay_ErrorData e)
{
    fprintf(stderr, "clay: %.*s\n", (int)e.errorText.length,
            e.errorText.chars);
    errores++;
}

static Clay_ElementId id_nodi(int index)
{
    return Clay__HashStringWithOffset(CLAY_STRING("nodus"),
                                      (uint32_t)index, 0);
}

static Clay_SizingAxis axis(int genus, float valor, float minimum,
                            float maximum)
{
    Clay_SizingAxis a;
    memset(&a, 0, sizeof a);
    switch (genus) {
    case VINCULUM_CRESCENS:
        a.type = CLAY__SIZING_TYPE_GROW;
        a.size.minMax.min = minimum;
        a.size.minMax.max = maximum;
        break;
    case VINCULUM_FIXA:
        a.type = CLAY__SIZING_TYPE_FIXED;
        a.size.minMax.min = valor;
        a.size.minMax.max = valor;
        break;
    case VINCULUM_PARS:
        a.type = CLAY__SIZING_TYPE_PERCENT;
        a.size.percent = valor;
        break;
    default:
        a.type = CLAY__SIZING_TYPE_FIT;
        a.size.minMax.min = minimum;
        a.size.minMax.max = maximum;
        break;
    }
    return a;
}

int vinculum_initiare(float latitudo, float altitudo)
{
    uint32_t mensura = Clay_MinMemorySize();
    void* memoria = malloc(mensura);
    Clay_Arena arena;
    Clay_Dimensions dimensiones;
    Clay_ErrorHandler tractator;

    if (!memoria) {
        return 0;
    }
    arena = Clay_CreateArenaWithCapacityAndMemory(mensura, memoria);
    dimensiones.width = latitudo;
    dimensiones.height = altitudo;
    tractator.errorHandlerFunction = errorem_tractare;
    tractator.userData = NULL;
    Clay_Initialize(arena, dimensiones, tractator);
    Clay_SetMeasureTextFunction(textum_metiri, NULL);
    return 1;
}

void vinculum_incipere(void)
{
    Clay_BeginLayout();
}

void vinculum_aperire(int index, const VinculumForma* f)
{
    Clay_ElementDeclaration d;
    memset(&d, 0, sizeof d);
    Clay__OpenElementWithId(id_nodi(index));
    d.layout.layoutDirection = f->directio ? CLAY_TOP_TO_BOTTOM
                                           : CLAY_LEFT_TO_RIGHT;
    d.layout.sizing.width = axis(f->genus_x, f->valor_x, f->minimum_x,
                                 f->maximum_x);
    d.layout.sizing.height = axis(f->genus_y, f->valor_y, f->minimum_y,
                                  f->maximum_y);
    d.layout.padding.left = (uint16_t)f->spatium_sinistrum;
    d.layout.padding.right = (uint16_t)f->spatium_dextrum;
    d.layout.padding.top = (uint16_t)f->spatium_superum;
    d.layout.padding.bottom = (uint16_t)f->spatium_inferum;
    d.layout.childGap = (uint16_t)f->intervallum;
    d.layout.childAlignment.x = f->allineatio_x == VINCULUM_MEDIUM
        ? CLAY_ALIGN_X_CENTER
        : (f->allineatio_x == VINCULUM_FINIS ? CLAY_ALIGN_X_RIGHT
                                             : CLAY_ALIGN_X_LEFT);
    d.layout.childAlignment.y = f->allineatio_y == VINCULUM_MEDIUM
        ? CLAY_ALIGN_Y_CENTER
        : (f->allineatio_y == VINCULUM_FINIS ? CLAY_ALIGN_Y_BOTTOM
                                             : CLAY_ALIGN_Y_TOP);
    d.clip.horizontal = f->praecidere_x ? true : false;
    d.clip.vertical = f->praecidere_y ? true : false;
    Clay__ConfigureOpenElement(d);
}

void vinculum_claudere(void)
{
    Clay__CloseElement();
}

int vinculum_finire(void)
{
    (void)Clay_EndLayout(0.0f);
    return errores == 0;
}

int vinculum_fines(int index, float* x, float* y, float* latitudo,
                   float* altitudo)
{
    Clay_ElementData e = Clay_GetElementData(id_nodi(index));
    if (!e.found) {
        return 0;
    }
    *x = e.boundingBox.x;
    *y = e.boundingBox.y;
    *latitudo = e.boundingBox.width;
    *altitudo = e.boundingBox.height;
    return 1;
}
