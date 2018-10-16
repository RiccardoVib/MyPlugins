#ifndef __VIRTUALTUBEDELAY__
#define __VIRTUALTUBEDELAY__

#include "IPlug_include_in_plug_hdr.h"
#include "Filter.hpp"
#include "DelayLine.hpp"
//#include "TextEntry.h"

static double bufOut_L[7];
static double bufIn_L[7];

static double bufOut_R[7];
static double bufIn_R[7];


static double bufOut_Ref_L[7];
static double bufIn_Ref_L[7];

static double bufOut_Ref_R[7];
static double bufIn_Ref_R[7];

static double bufOut_Fin_L[4];
static double bufIn_Fin_L[4];

static double bufOut_Fin_R[4];
static double bufIn_Fin_R[4];

class VirtualTubeDelay : public IPlug
{
public:
  VirtualTubeDelay(IPlugInstanceInfo instanceInfo);
  ~VirtualTubeDelay();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);
  
    
private:
  
  Filter mFilter;
  DelayLine mDelayLine;
  double mGainLeft;
  double mLengthLeft;
  
  double mGainRight;
  double mLengthRight;
  
  int mSize;
  double mDryWet;
  
  int mEnable;
  
  double mGainRefRight;
  double mLengthRefRight;

  double mGainRefLeft;
  double mLengthRefLeft;
  
  double mCaption;
  
  
  double leng_L;
  double delayMilli_L;
  double delaySamples_L;
  
  double leng_R;
  double delayMilli_R;
  double delaySamples_R;
  
  
  double lengRef_L;
  double delayMilliRef_L;
  double delaySamplesRef_L;
  
  double lengRef_R;
  double delayMilliRef_R;
  double delaySamplesRef_R;

  //int a;
  //TextEntry nameEntry;
  
  void CreateParams();
  void CreateGraphics();
  void CreatePresets();
  
};

#endif
