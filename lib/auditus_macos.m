/* auditus_macos.m - exitus soni per unitatem exitus CoreAudio (vide
 * include/auditus.h)
 *
 * API C purum (AudioToolbox / CoreAudio), sed capita systematis non
 * C89: ergo .m, ut energia et fenestra. Unitas 'DefaultOutput'
 * instrumentum ordinarium sequitur; forma nostra XLVIII milia f32
 * stereo intertexta - unitas ad formam instrumenti convertit.
 *
 * REDITUS (filum reale CoreAudio): iussa ex anulo in mixturam, mixtura
 * in tabulam systematis, nuntii in anulum, horologium publicatum. Nihil
 * allocat, nihil serat, nihil scribit in plagulas aut stderr. Vide
 * lib/auditus.worklog.md.
 */

/* capita systematis ANTE latina.h: macra eius ('character', numeri
 * Romani) capita Apple alioquin corrumpunt */
#include "postulata_macos.h"   /* PRIMUM: ante capita Apple */
#include <AudioToolbox/AudioToolbox.h>
#include <CoreAudio/CoreAudio.h>
#include <string.h>

#include "auditus.h"
#include "atomus.h"
#include "circulus.h"
#include "sonus.h"

/* status UNUS per processum (sicut fenestra) */
hic_manens b32                    auditus_apertum = FALSUM;
hic_manens AudioComponentInstance auditus_unitas;
hic_manens Mixtura*               auditus_mixtura = NIHIL;
hic_manens Circulus               auditus_iussa;
hic_manens Circulus               auditus_nuntii;
/* scriptor: filum soni; lector: principale */
hic_manens volatilis i32          auditus_momentum_xxxii = ZEPHYRUM;
/* principale solum: extensio circumvolutionum */
hic_manens i32                    auditus_momentum_ultimum = ZEPHYRUM;
hic_manens s64                    auditus_momentum_altum = ZEPHYRUM;
hic_manens i32                    auditus_latentia = ZEPHYRUM;


    /* ==================================================
     * REDITUS - in filo reali CoreAudio
     * ================================================== */

interior OSStatus
_reddere (
                         vacuum* referentia,
    AudioUnitRenderActionFlags*  vexilla,
          constans AudioTimeStamp* tempus,
                         UInt32  bus,
                         UInt32  quadra,
                AudioBufferList* tabulae)
{
     MixturaIussum iussum;
    MixturaNuntius nuntius;

    (vacuum)referentia;
    (vacuum)vexilla;
    (vacuum)tempus;
    (vacuum)bus;
    /* iussa ante agmen: momentum suum mixtura ipsa ad quadrum exactum
     * applicat */
    dum (circulus_legere(&auditus_iussa, &iussum))
    {
        (vacuum)mixtura_iubere(auditus_mixtura, &iussum);   /* plenum:
                                          * mixtura iussa_amissa numerat */
    }
    mixtura_reddere(auditus_mixtura, (f32*)tabulae->mBuffers[ZEPHYRUM].mData,
                    (i32)quadra);
    dum (mixtura_nuntium_legere(auditus_mixtura, &nuntius))
    {
        si (!circulus_scribere(&auditus_nuntii, &nuntius))
        {
            frange;   /* anulus plenus: nuntius in mixtura manet usque ad
                       * agmen proximum - non amittitur */
        }
    }
    atomus_publicare(&auditus_momentum_xxxii,
                     (i32)mixtura_momentum(auditus_mixtura));
    redde noErr;
}


    /* ==================================================
     * LATENTIA - quadra instrumenti in quadra nostra
     * ================================================== */

interior UInt32
_proprietas_instrumenti (
         AudioDeviceID instrumentum,
    AudioObjectPropertySelector selector)
{
    AudioObjectPropertyAddress inscriptio;
                        UInt32 valor = ZEPHYRUM;
                        UInt32 mensura = (UInt32)magnitudo(valor);

    inscriptio.mSelector = selector;
    inscriptio.mScope    = kAudioDevicePropertyScopeOutput;
    inscriptio.mElement  = kAudioObjectPropertyElementMain;
    si (AudioObjectGetPropertyData(instrumentum, &inscriptio, ZEPHYRUM, NIHIL,
                                   &mensura, &valor) != noErr)
    {
        redde ZEPHYRUM;
    }
    redde valor;
}

/* latentia + securitas + agmen instrumenti (quadra instrumenti) ad
 * frequentiam nostram scalata, plus latentia unitatis ipsius */
interior i32
_latentiam_computare (
    vacuum)
{
          AudioDeviceID instrumentum = kAudioObjectUnknown;
                 UInt32 mensura = (UInt32)magnitudo(instrumentum);
                Float64 frequentia = (Float64)SONUS_FREQUENTIA;
                Float64 unitatis = 0.0;
                 UInt32 m = (UInt32)magnitudo(unitatis);
                 UInt32 quadra;
    AudioObjectPropertyAddress inscriptio;
                 UInt32 mf = (UInt32)magnitudo(frequentia);

    si (AudioUnitGetProperty(auditus_unitas,
                             kAudioOutputUnitProperty_CurrentDevice,
                             kAudioUnitScope_Global, ZEPHYRUM, &instrumentum,
                             &mensura) != noErr)
    {
        redde ZEPHYRUM;
    }
    quadra = _proprietas_instrumenti(instrumentum,
                                     kAudioDevicePropertyLatency)
           + _proprietas_instrumenti(instrumentum,
                                     kAudioDevicePropertySafetyOffset)
           + _proprietas_instrumenti(instrumentum,
                                     kAudioDevicePropertyBufferFrameSize);
    inscriptio.mSelector = kAudioDevicePropertyNominalSampleRate;
    inscriptio.mScope    = kAudioObjectPropertyScopeGlobal;
    inscriptio.mElement  = kAudioObjectPropertyElementMain;
    si (AudioObjectGetPropertyData(instrumentum, &inscriptio, ZEPHYRUM, NIHIL,
                                   &mf, &frequentia) != noErr
        || frequentia <= 0.0)
    {
        frequentia = (Float64)SONUS_FREQUENTIA;
    }
    (vacuum)AudioUnitGetProperty(auditus_unitas, kAudioUnitProperty_Latency,
                                 kAudioUnitScope_Global, ZEPHYRUM, &unitatis,
                                 &m);
    redde (i32)((Float64)quadra * (Float64)SONUS_FREQUENTIA / frequentia
                + unitatis * (Float64)SONUS_FREQUENTIA + 0.5);
}


    /* ==================================================
     * INTERFACIES PUBLICA
     * ================================================== */

b32
auditus_aperire (
                Mixtura* mixtura,
    constans AuditusForma* forma,
                Piscina* piscina)
{
    AudioComponentDescription descriptio;
             AudioComponent  componens;
    AudioStreamBasicDescription formatio;
         AURenderCallbackStruct  reditus;
                         vacuum* memoria_iussorum;
                         vacuum* memoria_nuntiorum;

    si (auditus_apertum || mixtura == NIHIL || forma == NIHIL)
    {
        redde FALSUM;
    }
    memoria_iussorum = piscina_allocare(piscina,
        (memoriae_index)forma->iussa * magnitudo(MixturaIussum));
    memoria_nuntiorum = piscina_allocare(piscina,
        (memoriae_index)forma->nuntii * magnitudo(MixturaNuntius));
    si (   !circulus_parare(&auditus_iussa, memoria_iussorum, forma->iussa,
                            (i32)magnitudo(MixturaIussum))
        || !circulus_parare(&auditus_nuntii, memoria_nuntiorum, forma->nuntii,
                            (i32)magnitudo(MixturaNuntius)))
    {
        redde FALSUM;
    }

    memset(&descriptio, ZEPHYRUM, magnitudo(descriptio));
    descriptio.componentType         = kAudioUnitType_Output;
    descriptio.componentSubType      = kAudioUnitSubType_DefaultOutput;
    descriptio.componentManufacturer = kAudioUnitManufacturer_Apple;
    componens = AudioComponentFindNext(NIHIL, &descriptio);
    si (componens == NIHIL
        || AudioComponentInstanceNew(componens, &auditus_unitas) != noErr)
    {
        redde FALSUM;
    }

    memset(&formatio, ZEPHYRUM, magnitudo(formatio));
    formatio.mSampleRate       = (Float64)SONUS_FREQUENTIA;
    formatio.mFormatID         = kAudioFormatLinearPCM;
    formatio.mFormatFlags      = kAudioFormatFlagIsFloat
                               | kAudioFormatFlagIsPacked;   /* intertexta */
    formatio.mChannelsPerFrame = II;
    formatio.mBitsPerChannel   = XXXII;
    formatio.mFramesPerPacket  = I;
    formatio.mBytesPerFrame    = II * IV;
    formatio.mBytesPerPacket   = II * IV;
    reditus.inputProc       = _reddere;
    reditus.inputProcRefCon = NIHIL;

    auditus_mixtura          = mixtura;
    auditus_momentum_xxxii   = (i32)mixtura_momentum(mixtura);
    auditus_momentum_ultimum = auditus_momentum_xxxii;
    auditus_momentum_altum   = mixtura_momentum(mixtura)
                             - (s64)auditus_momentum_xxxii;
    si (   AudioUnitSetProperty(auditus_unitas, kAudioUnitProperty_StreamFormat,
                                kAudioUnitScope_Input, ZEPHYRUM, &formatio,
                                (UInt32)magnitudo(formatio)) != noErr
        || AudioUnitSetProperty(auditus_unitas,
                                kAudioUnitProperty_SetRenderCallback,
                                kAudioUnitScope_Input, ZEPHYRUM, &reditus,
                                (UInt32)magnitudo(reditus)) != noErr
        || AudioUnitInitialize(auditus_unitas) != noErr)
    {
        (vacuum)AudioComponentInstanceDispose(auditus_unitas);
        auditus_mixtura = NIHIL;
        redde FALSUM;
    }
    auditus_latentia = _latentiam_computare();
    si (AudioOutputUnitStart(auditus_unitas) != noErr)
    {
        (vacuum)AudioUnitUninitialize(auditus_unitas);
        (vacuum)AudioComponentInstanceDispose(auditus_unitas);
        auditus_mixtura = NIHIL;
        redde FALSUM;
    }
    auditus_apertum = VERUM;
    redde VERUM;
}

vacuum
auditus_claudere (
    vacuum)
{
    si (!auditus_apertum)
    {
        redde;
    }
    /* AudioOutputUnitStop synchronum est: reditus currens finitur
     * antequam redit - post hoc mixtura iterum vocantis */
    (vacuum)AudioOutputUnitStop(auditus_unitas);
    (vacuum)AudioUnitUninitialize(auditus_unitas);
    (vacuum)AudioComponentInstanceDispose(auditus_unitas);
    auditus_apertum  = FALSUM;
    auditus_mixtura  = NIHIL;
    auditus_latentia = ZEPHYRUM;
}

b32
auditus_iubere (
    constans MixturaIussum* iussum)
{
    si (!auditus_apertum)
    {
        redde FALSUM;
    }
    redde circulus_scribere(&auditus_iussa, iussum);
}

b32
auditus_nuntium_legere (
    MixturaNuntius* nuntius)
{
    si (!auditus_apertum)
    {
        redde FALSUM;
    }
    redde circulus_legere(&auditus_nuntii, nuntius);
}

s64
auditus_momentum (
    vacuum)
{
    i32 nunc = atomus_legere(&auditus_momentum_xxxii);

    /* circumvolutio XXXII bitorum (XXIV.VIII horae): pars alta crescit */
    si (nunc < auditus_momentum_ultimum)
    {
        auditus_momentum_altum += (s64)0x100000000LL;
    }
    auditus_momentum_ultimum = nunc;
    redde auditus_momentum_altum + (s64)nunc;
}

i32
auditus_mora (
    vacuum)
{
    redde auditus_apertum ? auditus_latentia : ZEPHYRUM;
}
