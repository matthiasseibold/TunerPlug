#ifndef __TUNERPLUG__
#define __TUNERPLUG__

#include "IPlug_include_in_plug_hdr.h"
#include "src/Gist.h"

#include "IGUIBRRectWT.h"

class TunerPlug : public IPlug
{
public:
  TunerPlug(IPlugInstanceInfo instanceInfo);
  ~TunerPlug();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
  Gist<double>* mGist;
  std::vector<double> mAudioFrame;
  IGUIBRRectWT * msgZone;
  IGUIBRRectWT * msgZone1;
  IGUIBRRectWT * msgZone2;
  std::string name[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" };
  int mText;
  int mText1;
};

#endif
