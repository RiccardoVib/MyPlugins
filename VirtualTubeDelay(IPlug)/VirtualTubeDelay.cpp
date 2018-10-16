#include "VirtualTubeDelay.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmain"
#include "IPlug_include_in_plug_src.h"
#pragma clang diagnostic pop
#include "IControl.h"
#include "resource.h"
//#include "IGraphics.h"
//#include "TextEntry.h"
//#include <iostream>
#include "IAutoGUI.h"

const int kNumPrograms = 1;

enum EParams
{
  kGainLeft = 0,
  kGainRight,
  kLengthLeft,
  kLengthRight,
  kSize,
  kDryWet,
  
  kGainRefLeft,
  kGainRefRight,
  
  kLengthRefLeft,
  kLengthRefRight,
  kEnable,
 // kCaption,
 // kIKnobMultiControl_def,
 // kICaptionControl,
  
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kKnobFrames = 64,
  kButtonStates = 2,
  
  kIRBC_W = 24,  // width of bitmap
  kIRBC_H = 24,  // height of one of the bitmap images
  kIRBC_VN = 2,  // number of vertical buttons

  kIFaderControl_Horiz_L = 200,  // fader length
  kIFaderControl_Horiz_L_Ref = 200,
  
  //ITextControl
  kITC_X = 425,
  kITC_Y = 8,
  kITC_W = 100,
  kITC_H = 20,
  
  //ICaptionControl
  kICC_X = 384,
  kICC_Y = 202,
  kICC_W = 100,
  kICC_H = 20,
  
  
};

typedef struct {
  const char* name;
  const int x;
  const int y;
  const double defaultVal;
  const double minVal;
  const double maxVal;
  const double parameterStep;
} parameterProperties_struct;

const parameterProperties_struct parameterProperties[kNumParams] =
{
  {.name="GainLeft", .x=440, .y=20, .defaultVal=50.0, .minVal=0.0, .maxVal=700.0, .parameterStep = 0.01},
  {.name="GainRight", .x=520, .y=20, .defaultVal=50.0, .minVal=0.0, .maxVal=700.0, .parameterStep = 0.01},
  {.name="LengthLeft", .x=30, .y=30, .defaultVal=10.0, .minVal=2.0, .maxVal=30.0, .parameterStep = 0.01},
  {.name="LengthRight", .x=240, .y=30, .defaultVal=10.0, .minVal=2.0, .maxVal=30.0, .parameterStep = 0.01},
  {.name="Size", .x=520, .y=100, .defaultVal=0, .minVal=0, .maxVal=13},
  {.name="DryWet", .x=520, .y=220, .defaultVal=0., .minVal=0., .maxVal=100., .parameterStep = 0.01},
  {.name="GainRefLeft", .x=70, .y=220, .defaultVal=50., .minVal=0.0, .maxVal=2000. , .parameterStep = 0.01},
  {.name="GainRefRight", .x=280, .y=220, .defaultVal=50., .minVal=0.0, .maxVal=2000., .parameterStep = 0.01},
  {.name="LengthRefLeft", .x=30, .y=150, .defaultVal=1.0, .minVal=1.0, .maxVal=5.0, .parameterStep = 0.01},
  {.name="LengthRefRight", .x=240, .y=150, .defaultVal=1.0, .minVal=1.0, .maxVal=5.0, .parameterStep = 0.01},
  {.name="EnableRef", .x=455, .y=220, .defaultVal=false}//,
  //{.name="Caption", .x=384, .y=220}
};



VirtualTubeDelay::VirtualTubeDelay(IPlugInstanceInfo instanceInfo)
:	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mGainLeft(1.), mGainRight(1.), mLengthLeft(1.), mLengthRight(1.), mSize(1.), mDryWet(1.), mEnable(1.), leng_L(10.0), leng_R(10.0), delayMilli_L(28.985507246376812), delayMilli_R(28.985507246376812), delaySamples_L(1278.2608695652175), delaySamples_R(1278.2608695652175), lengRef_L(12.0), lengRef_R(12.0), delayMilliRef_L(34.78260869565217), delayMilliRef_R(34.78260869565217), delaySamplesRef_L(1533.913043478260), delaySamplesRef_R(1533.913043478260)

{
  TRACE;
  
  //IText text = IText(14, &COLOR_BLUE, "Helvetica");
  
  /*
  
  
  GetParam(kCaption)->InitDouble("kCaption", 50., 0., 1000.0, 0.01, "dB");
  
  
  
  //Attach ITextControl
  pGraphics->AttachControl(new ITextControl(this, IRECT(kITC_X, kITC_Y, (kITC_X + kITC_W), (kITC_Y + kITC_H)), &text, "Gain Left"));
  
  //Attach ICaptionControl
  pGraphics->AttachControl(new ICaptionControl(this, IRECT(kICC_X, kICC_Y, (kICC_X + kICC_W), (kICC_Y + kICC_H)), kCaption, &text));
  */
  
  
  CreateParams();
  CreateGraphics();
  CreatePresets();
  
  }

void VirtualTubeDelay::CreateParams() {
  for (int i = 0; i < kNumParams; i++) {
    IParam* param = GetParam(i);
    const parameterProperties_struct& properties = parameterProperties[i];
    switch (i) {
        // Enum Parameters:
      case kSize:
        // Int Parameters:
        param->InitInt(properties.name,
                       properties.defaultVal,
                       properties.minVal,
                       properties.maxVal);
        break;

      case kEnable:
        // Int Parameters:
        param->InitBool(properties.name,
                          properties.defaultVal);
        break;
     /* case kIKnobMultiControl_def:
        // Int Parameters:
        param->InitInt("IKnobMultiControl Default", 0, 1, 14, "a");
        break;
      case kICaptionControl:
        // Int Parameters:
        param->InitInt("ICaptionControl", 0, 1, 14, "aa");

        break;*/

        default:
        // Double Parameters:
        param->InitDouble(properties.name,
                          properties.defaultVal,
                          properties.minVal,
                          properties.maxVal,
                          properties.parameterStep);
        // For VST3:
        //param->SetDisplayText(mGainLeft, properties.name);
        break;
        
      //case kCaption:
        //param->InitDouble("kCaption", 50., 0., 1000.0, 0.01, "dB");
      }
  }
  
  GetParam(kGainLeft)->SetShape(1);
  GetParam(kGainRight)->SetShape(1);
  //GetParam(kLengthLeft)->SetShape(1);
  //GetParam(kLengthRight)->SetShape(1);
  GetParam(kSize)->SetShape(1);
  //GetParam(kEnable)->SetShape(1);
  GetParam(kGainRefLeft)->SetShape(1);
  GetParam(kGainRefRight)->SetShape(1);
  //GetParam(kLengthRefLeft)->SetShape(1);
  //GetParam(kLengthRefRight)->SetShape(1);
  
  
  for (int i = 0; i < kNumParams; i++) {
    OnParamChange(i);
  }
  
}

void VirtualTubeDelay::CreateGraphics() {
  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  pGraphics->AttachPanelBackground(&COLOR_WHITE);
  
  IText text(14, &COLOR_BLACK, "Verdana", IText::kStyleNormal, IText::kAlignNear, 0, IText::kQualityNonAntiAliased);
  
  //GenerateSliderGUI(pGraphics, this, &textProps, &COLOR_WHITE, &COLOR_BLACK);//, 60, 70);
  
  
  //Attach ITextControl
  pGraphics->AttachControl(new ITextControl(this, IRECT(440, 10, (442 + kITC_W), (10 + kITC_H)), &text, "Gain Left"));
  pGraphics->AttachControl(new ITextControl(this, IRECT(520, 10, (522 + kITC_W), (10 + kITC_H)), &text, "Gain Right"));
  pGraphics->AttachControl(new ITextControl(this, IRECT(40, 210, (40 + kITC_W), (210 + kITC_H)), &text, "Gain Left (Reflection)"));
  pGraphics->AttachControl(new ITextControl(this, IRECT(250, 210, (250 + kITC_W), (210 + kITC_H)), &text, "Gain Right (Reflection)"));
  pGraphics->AttachControl(new ITextControl(this, IRECT(10, 10, (10 + kITC_W), (10 + kITC_H)), &text, "Tube Length (Left) (2-30m)"));
  pGraphics->AttachControl(new ITextControl(this, IRECT(220, 10, (220 + kITC_W), (10 + kITC_H)), &text, "Tube Length (Right) (2-30m)"));
  pGraphics->AttachControl(new ITextControl(this, IRECT(10, 120, (10 + kITC_W), (120 + kITC_H)), &text, "Tube Length (Left Reflection) (1-5m)"));
  pGraphics->AttachControl(new ITextControl(this, IRECT(240, 120, (240 + kITC_W), (120 + kITC_H)), &text, "Tube Length (Right Reflection) (1-5m)"));
  pGraphics->AttachControl(new ITextControl(this, IRECT(525, 210, (525 + kITC_W), (210 + kITC_H)), &text, "Dry/Wet"));
  pGraphics->AttachControl(new ITextControl(this, IRECT(522, 95, (522 + kITC_W), (95 + kITC_H)), &text, "Tube Size"));
  pGraphics->AttachControl(new ITextControl(this, IRECT(420, 180, (420 + kITC_W), (180 + kITC_H)), &text, "Reflection on-off"));

  
  
  
  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);
  //Attach the horizontal IFaderControl
  IBitmap fader = pGraphics->LoadIBitmap(FADER_ID, FADER_FN);
  //Attach the vertical IRadioButtonsControl
  IBitmap bitmap = pGraphics->LoadIBitmap(BUTTON_ID, BUTTON_FN, 2);
  
  for (int i = 0; i < kNumParams; i++) {
    const parameterProperties_struct& properties = parameterProperties[i];
    IControl* control;
    //IBitmap* graphic;
    
    switch (i) {
        // Knobs:
      case kGainLeft:
        control = new IKnobMultiControl(this, properties.x, properties.y, i, &knob);
        break;
      case kGainRight:
        control = new IKnobMultiControl(this, properties.x, properties.y, i, &knob);
        break;
      case kGainRefLeft:
        control = new IKnobMultiControl(this, properties.x, properties.y, i, &knob);
        break;
      case kGainRefRight:
        control = new IKnobMultiControl(this, properties.x, properties.y, i, &knob);
        break;
      case kSize:
        control = new IKnobMultiControl(this, properties.x, properties.y, i, &knob);
        break;
      case kDryWet:
        control = new IKnobMultiControl(this, properties.x, properties.y, i, &knob);
        break;
      case kLengthLeft:
        control = new IFaderControl(this, properties.x, properties.y, kIFaderControl_Horiz_L_Ref, i, &fader, kHorizontal);
        break;
      case kLengthRight:
        control = new IFaderControl(this, properties.x, properties.y, kIFaderControl_Horiz_L_Ref, i, &fader, kHorizontal);
        break;
      case kLengthRefLeft:
        control = new IFaderControl(this, properties.x, properties.y, kIFaderControl_Horiz_L_Ref, i, &fader, kHorizontal);
        break;
      case kLengthRefRight:
        control = new IFaderControl(this, properties.x, properties.y, kIFaderControl_Horiz_L_Ref, i, &fader, kHorizontal);
        break;
      case kEnable:
        control = new IRadioButtonsControl(this, IRECT(properties.x, properties.y, properties.x + (kIRBC_W*kIRBC_VN), properties.y + (kIRBC_H*kIRBC_VN)), kEnable, kIRBC_VN, &bitmap);
        break;
 /*     case kIKnobMultiControl_def:
        control = new IKnobMultiControl(this, 384, 220, kIKnobMultiControl_def, &knob);
        break;
      case kICaptionControl:
        control = new ICaptionControl(this, IRECT(kICC_X, kICC_Y, (kICC_X + kICC_W), (kICC_Y + kICC_H)), kICaptionControl, &text);*/
    }

    pGraphics->AttachControl(control);
  }
  AttachGraphics(pGraphics);
}

VirtualTubeDelay::~VirtualTubeDelay() {}

void VirtualTubeDelay::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.
  
  double *leftInput = inputs[0];
  double *rightInput = inputs[1];
  double *leftOutput = outputs[0];
  double *rightOutput = outputs[1];
  
  if(mEnable == false){
  
    for (int s = 0; s < nFrames; ++s, ++leftInput, ++rightInput, ++leftOutput, ++rightOutput)
    {
      mFilter.process();
    
      bufIn_L[mFilter.i_0] = *leftInput;
    
      bufIn_R[mFilter.i_0] = *rightInput;
    
      bufOut_L[mFilter.i_0] = bufIn_L[mFilter.i_0] * mFilter.b0f_L + bufIn_L[mFilter.i_1] * mFilter.b1f_L + bufIn_L[mFilter.i_2] * mFilter.b2f_L + bufIn_L[mFilter.i_3] * mFilter.b3f_L + bufIn_L[mFilter.i_4] * mFilter.b4f_L + bufIn_L[mFilter.i_5] * mFilter.b5f_L + bufIn_L[mFilter.i_6] * mFilter.b6f_L - bufOut_L[mFilter.i_1] * mFilter.a1f_L - bufOut_L[mFilter.i_2] * mFilter.a2f_L - bufOut_L[mFilter.i_3] * mFilter.a3f_L - bufOut_L[mFilter.i_4] * mFilter.a4f_L - bufOut_L[mFilter.i_5] * mFilter.a5f_L - bufOut_L[mFilter.i_6] * mFilter.a6f_L;
    
      bufOut_R[mFilter.i_0] = bufIn_R[mFilter.i_0] * mFilter.b0f_R + bufIn_R[mFilter.i_1] * mFilter.b1f_R + bufIn_R[mFilter.i_2] * mFilter.b2f_R + bufIn_R[mFilter.i_3] * mFilter.b3f_R + bufIn_R[mFilter.i_4] * mFilter.b4f_R + bufIn_R[mFilter.i_5] * mFilter.b5f_R + bufIn_R[mFilter.i_6] * mFilter.b6f_R - bufOut_R[mFilter.i_1] * mFilter.a1f_R - bufOut_R[mFilter.i_2] * mFilter.a2f_R - bufOut_R[mFilter.i_3] * mFilter.a3f_R - bufOut_R[mFilter.i_4] * mFilter.a4f_R - bufOut_R[mFilter.i_5] * mFilter.a5f_R - bufOut_R[mFilter.i_6] * mFilter.a6f_R;
    
      if (mDryWet == 0) {
      
        *leftOutput = *leftInput;
        *rightOutput = *rightInput;
      
      }else{
      
        *leftOutput = *leftInput * (1.0 - mDryWet) + mDryWet * mGainLeft * mDelayLine.delayLineL(bufOut_L[mFilter.i_0]);
      
        *rightOutput = *rightInput * (1.0 - mDryWet) + mDryWet * mGainRight * mDelayLine.delayLineR(bufOut_R[mFilter.i_0]);
      }
    }
    
  }else{
  
    for (int s = 0; s < nFrames; ++s, ++leftInput, ++rightInput, ++leftOutput, ++rightOutput)
    {
  
      mFilter.process();
      
      bufIn_L[mFilter.i_0] = *leftInput;
      
      bufIn_R[mFilter.i_0] = *rightInput;
      
      bufOut_L[mFilter.i_0] = bufIn_L[mFilter.i_0] * mFilter.b0f_L + bufIn_L[mFilter.i_1] * mFilter.b1f_L + bufIn_L[mFilter.i_2] * mFilter.b2f_L + bufIn_L[mFilter.i_3] * mFilter.b3f_L + bufIn_L[mFilter.i_4] * mFilter.b4f_L + bufIn_L[mFilter.i_5] * mFilter.b5f_L + bufIn_L[mFilter.i_6] * mFilter.b6f_L - bufOut_L[mFilter.i_1] * mFilter.a1f_L - bufOut_L[mFilter.i_2] * mFilter.a2f_L - bufOut_L[mFilter.i_3] * mFilter.a3f_L - bufOut_L[mFilter.i_4] * mFilter.a4f_L - bufOut_L[mFilter.i_5] * mFilter.a5f_L - bufOut_L[mFilter.i_6] * mFilter.a6f_L;
      
      bufOut_R[mFilter.i_0] = bufIn_R[mFilter.i_0] * mFilter.b0f_R + bufIn_R[mFilter.i_1] * mFilter.b1f_R + bufIn_R[mFilter.i_2] * mFilter.b2f_R + bufIn_R[mFilter.i_3] * mFilter.b3f_R + bufIn_R[mFilter.i_4] * mFilter.b4f_R + bufIn_R[mFilter.i_5] * mFilter.b5f_R + bufIn_R[mFilter.i_6] * mFilter.b6f_R - bufOut_R[mFilter.i_1] * mFilter.a1f_R - bufOut_R[mFilter.i_2] * mFilter.a2f_R - bufOut_R[mFilter.i_3] * mFilter.a3f_R - bufOut_R[mFilter.i_4] * mFilter.a4f_R - bufOut_R[mFilter.i_5] * mFilter.a5f_R - bufOut_R[mFilter.i_6] * mFilter.a6f_R;
      
      bufIn_Ref_L[mFilter.i_0] = bufOut_L[mFilter.i_0];
      bufIn_Ref_R[mFilter.i_0] = bufOut_R[mFilter.i_0];
      
      bufOut_Ref_L[mFilter.i_0] = bufIn_Ref_L[mFilter.i_0] * mFilter.b0f_Ref_L + bufIn_Ref_L[mFilter.i_1] * mFilter.b1f_Ref_L + bufIn_Ref_L[mFilter.i_2] * mFilter.b2f_Ref_L + bufIn_Ref_L[mFilter.i_3] * mFilter.b3f_Ref_L + bufIn_Ref_L[mFilter.i_4] * mFilter.b4f_Ref_L + bufIn_Ref_L[mFilter.i_5] * mFilter.b5f_Ref_L + bufIn_Ref_L[mFilter.i_6] * mFilter.b6f_Ref_L - bufOut_Ref_L[mFilter.i_1] * mFilter.a1f_Ref_L - bufOut_Ref_L[mFilter.i_2] * mFilter.a2f_Ref_L - bufOut_Ref_L[mFilter.i_3] * mFilter.a3f_Ref_L - bufOut_Ref_L[mFilter.i_4] * mFilter.a4f_Ref_L - bufOut_Ref_L[mFilter.i_5] * mFilter.a5f_Ref_L - bufOut_Ref_L[mFilter.i_6] * mFilter.a6f_Ref_L;
      
      bufOut_Ref_R[mFilter.i_0] = bufIn_Ref_R[mFilter.i_0] * mFilter.b0f_Ref_R + bufIn_Ref_R[mFilter.i_1] * mFilter.b1f_Ref_R + bufIn_Ref_R[mFilter.i_2] * mFilter.b2f_Ref_R + bufIn_Ref_R[mFilter.i_3] * mFilter.b3f_Ref_R + bufIn_Ref_R[mFilter.i_4] * mFilter.b4f_Ref_R + bufIn_Ref_R[mFilter.i_5] * mFilter.b5f_Ref_R + bufIn_Ref_R[mFilter.i_6] * mFilter.b6f_Ref_R - bufOut_Ref_R[mFilter.i_1] * mFilter.a1f_Ref_R - bufOut_Ref_R[mFilter.i_2] * mFilter.a2f_Ref_R - bufOut_Ref_R[mFilter.i_3] * mFilter.a3f_Ref_R - bufOut_Ref_R[mFilter.i_4] * mFilter.a4f_Ref_R - bufOut_Ref_R[mFilter.i_5] * mFilter.a5f_Ref_R - bufOut_Ref_R[mFilter.i_6] * mFilter.a6f_Ref_R;
      
      bufIn_Fin_L[mFilter.j_0] = bufOut_L[mFilter.i_0];
      bufIn_Fin_R[mFilter.j_0] = bufOut_R[mFilter.i_0];

      
      bufOut_Fin_L[mFilter.j_0] = bufIn_Fin_L[mFilter.j_0] * mFilter.b_Ref[mSize][0] + bufIn_Fin_L[mFilter.j_1] * mFilter.b_Ref[mSize][1] + bufIn_Fin_L[mFilter.j_2] * mFilter.b_Ref[mSize][2] + bufIn_Fin_L[mFilter.j_3] * mFilter.b_Ref[mSize][3] - bufOut_Fin_L[mFilter.j_1] * mFilter.a_Ref[mSize][0] - bufOut_Fin_L[mFilter.j_2] * mFilter.a_Ref[mSize][1] - bufOut_Fin_L[mFilter.j_3] * mFilter.a_Ref[mSize][2];
      
      bufOut_Fin_R[mFilter.j_0] = bufIn_Fin_R[mFilter.j_0] * mFilter.b_Ref[mSize][0] + bufIn_Fin_R[mFilter.j_1] * mFilter.b_Ref[mSize][1] + bufIn_Fin_R[mFilter.j_2] * mFilter.b_Ref[mSize][2] + bufIn_Fin_R[mFilter.j_3] * mFilter.b_Ref[mSize][3] - bufOut_Fin_R[mFilter.j_1] * mFilter.a_Ref[mSize][0] - bufOut_Fin_R[mFilter.j_2] * mFilter.a_Ref[mSize][1] - bufOut_Fin_R[mFilter.j_3] * mFilter.a_Ref[mSize][2];
      
      *leftOutput = *leftInput * (1.0 - mDryWet) + mDryWet * (mGainLeft * mDelayLine.delayLineL(bufOut_L[mFilter.i_0]) + mGainRefLeft * mDelayLine.delayLine_Ref_L(bufOut_Fin_L[mFilter.j_0]));
      
      *rightOutput = *rightInput * (1.0 - mDryWet) + mDryWet * (mGainRight * mDelayLine.delayLineR(bufOut_R[mFilter.i_0]) + mGainRefRight * mDelayLine.delayLine_Ref_R(bufOut_Fin_R[mFilter.j_0]));
    
    }
  }
}

void VirtualTubeDelay::Reset()
{
  TRACE;
  IMutexLock lock(this);
  mFilter.setSampleRate(GetSampleRate());
  //mDelayLine.suspend();
}

void VirtualTubeDelay::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);
  
/*  if (paramIdx == kIKnobMultiControl_def)
  {
    if (GetGUI())
    {
      GetGUI()->SetParameterFromPlug(kICaptionControl, GetParam(paramIdx)->Int(), false);
    }
    InformHostOfParamChange(kICaptionControl, (GetParam(paramIdx)->Int())/14); //inform host of new normalized value
  }*/

  
  switch (paramIdx)
  {
    case kGainLeft:
      mGainLeft = GetParam(kGainLeft)->Value() / 100.;
      break;
      
    case kGainRight:
      mGainRight = GetParam(kGainRight)->Value() / 100.;
      break;

    case kLengthLeft:
      mLengthLeft = GetParam(kLengthLeft)->Value();
      
      leng_L = mFilter.setLengt(mLengthLeft);
      delayMilli_L = mFilter.setDelayMilliseconds(leng_L);
      delaySamples_L = mFilter.setDelaySamples(delayMilli_L);
      mDelayLine.setDelayL(delaySamples_L);
      mFilter.setValues(mLengthLeft, mSize);
      mFilter.getCalculatedCoefficients(0);
      // ref
      lengRef_L = leng_L + mFilter.setLengt(2*mLengthRefLeft);
      delayMilliRef_L = mFilter.setDelayMilliseconds(lengRef_L);
      delaySamplesRef_L = mFilter.setDelaySamples(delayMilliRef_L);
      mDelayLine.setDelay_Ref_L(delaySamplesRef_L);
      mFilter.setValues(2*mLengthRefLeft, mSize);
      mFilter.getCalculatedCoefficients_Ref(0);
      break;
    case kLengthRight:
      mLengthRight = GetParam(kLengthRight)->Value();
      
      leng_R = mFilter.setLengt(mLengthRight);
      delayMilli_R = mFilter.setDelayMilliseconds(leng_R);
      delaySamples_R = mFilter.setDelaySamples(delayMilli_R);
      mDelayLine.setDelayR(delaySamples_R);
      mFilter.setValues(mLengthRight, mSize);
      mFilter.getCalculatedCoefficients(1);
      // ref
      lengRef_R = leng_R + mFilter.setLengt(2*mLengthRefRight);
      delayMilliRef_R = mFilter.setDelayMilliseconds(lengRef_R);
      delaySamplesRef_R = mFilter.setDelaySamples(delayMilliRef_R);
      mDelayLine.setDelay_Ref_R(delaySamplesRef_R);
      mFilter.setValues(2*mLengthRefRight, mSize);
      mFilter.getCalculatedCoefficients_Ref(1);
      break;
    case kSize:
      mSize = GetParam(kSize)->Value();
      
      mFilter.setValues(mLengthLeft, mSize);
      mFilter.getCalculatedCoefficients(0);
      mFilter.setValues(mLengthRight, mSize);
      mFilter.getCalculatedCoefficients(1);
      
      mFilter.setValues(2*mLengthRefLeft, mSize);
      mFilter.getCalculatedCoefficients_Ref(0);
      mFilter.setValues(2*mLengthRefRight, mSize);
      mFilter.getCalculatedCoefficients_Ref(1);
      break;
    case kDryWet:
      mDryWet = GetParam(kDryWet)->Value() / 100.;
      break;
    case kEnable:
      mEnable = GetParam(kEnable)->Value();
      break;
    case kGainRefLeft:
      mGainRefLeft = GetParam(kGainRefLeft)->Value() / 100.;
      break;
    case kGainRefRight:
      mGainRefRight = GetParam(kGainRefRight)->Value() / 100.;
      break;
    case kLengthRefLeft:
      mLengthRefLeft = GetParam(kLengthRefLeft)->Value() / 100.;
      
      lengRef_L = leng_L + mFilter.setLengt(2*mLengthRefLeft);
      delayMilliRef_L = mFilter.setDelayMilliseconds(lengRef_L);
      delaySamplesRef_L = mFilter.setDelaySamples(delayMilliRef_L);
      mDelayLine.setDelay_Ref_L(delaySamplesRef_L);
      mFilter.setValues(2*mLengthRefLeft, mSize);
      mFilter.getCalculatedCoefficients_Ref(0);
      break;
    case kLengthRefRight:
      mLengthRefRight = GetParam(kLengthRefRight)->Value() / 100.;
      
      lengRef_R = leng_R + mFilter.setLengt(2*mLengthRefRight);
      delayMilliRef_R = mFilter.setDelayMilliseconds(lengRef_R);
      delaySamplesRef_R = mFilter.setDelaySamples(delayMilliRef_R);
      mDelayLine.setDelay_Ref_R(delaySamplesRef_R);
      mFilter.setValues(2*mLengthRefRight, mSize);
      mFilter.getCalculatedCoefficients_Ref(1);
      break;
   /* case kICaptionControl:
      break;
    case kIKnobMultiControl_def:
      break;*/
    default:
      break;
  }
  
  //GetParam(kCaption)->Set(77.);
}

void VirtualTubeDelay::CreatePresets() {
  MakeDefaultPreset((char *) "-", kNumPrograms);
}
