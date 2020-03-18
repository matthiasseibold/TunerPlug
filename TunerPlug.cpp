#include "TunerPlug.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

#include <cmath>
#include <iostream>
#include <sstream>

#define DEBUG

# ifdef DEBUG 

#include <Windows.h>

#define DBOUT( s )            \
{                             \
   std::ostringstream os_;    \
   os_ << s;                   \
   OutputDebugString( os_.str().c_str() );  \
}
#endif

const int kNumPrograms = 1;

enum EParams
{
  kGain = 0,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kGainX = 100,
  kGainY = 100,
  kKnobFrames = 60
};

TunerPlug::TunerPlug(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo)
{
  TRACE;

  	// background
	IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
	pGraphics->AttachPanelBackground(&COLOR_WHITE);

	// title box
	IRECT tmpRect(80, 50, 200, 80.);
	IText textProps(30, &COLOR_BLACK, "Arial", IText::kStyleNormal, IText::kAlignCenter, 0, IText::kQualityDefault);
	mText = pGraphics->AttachControl(new ITextControl(this, tmpRect, &textProps, "Tuner"));

	// text boxes
	IRECT tmprect(5, GUI_HEIGHT - 22, GUI_WIDTH - 5, GUI_HEIGHT - 5);
	pGraphics->AttachControl((msgZone = new IGUIBRRectWT(this, tmprect, &COLOR_WHITE, &COLOR_WHITE, 3)));
	msgZone->SetText("init_freq");

	IRECT tmprect1(5, GUI_HEIGHT - 50, GUI_WIDTH - 5, GUI_HEIGHT - 30);
	pGraphics->AttachControl((msgZone1 = new IGUIBRRectWT(this, tmprect1, &COLOR_WHITE, &COLOR_WHITE, 3)));
	msgZone1->SetText("init_pitch");

	IRECT tmprect2(5, GUI_HEIGHT - 78, GUI_WIDTH - 5, GUI_HEIGHT - 55);
	pGraphics->AttachControl((msgZone2 = new IGUIBRRectWT(this, tmprect2, &COLOR_WHITE, &COLOR_WHITE, 3)));
	msgZone2->SetText("init_cents");

	AttachGraphics(pGraphics);

	// initialize Gist 
	int frameSize = 4410;
	int sampleRate = GetSampleRate();
	mGist = new Gist<double>(frameSize, sampleRate);
	
	//MakePreset("preset 1", ... );
	MakeDefaultPreset((char *) "-", kNumPrograms);

}

TunerPlug::~TunerPlug() {}

void TunerPlug::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	// Mutex is already locked for us.
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];
    
	mAudioFrame.reserve(1100);

#ifdef DEBUG

	// generate sine wave for testing
	double amplitude = 1;
	double frequency = 31;
	double phase = 0.0;
	double time = 0.0;
	double deltaTime = 1 / GetSampleRate();

	for (int sample = 0; sample < nFrames; ++sample) {
		  float value = amplitude * sin(2 * M_PI * frequency * time + phase);
		  mAudioFrame.push_back(value);
		  time += deltaTime;
	}

#else
  
	// plugin output
	for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
	{
		*out1 = *in1;
		*out2 = *in2;
		mAudioFrame.push_back(0.5*(*in1 + *in2)); // make mono
	}

#endif

	if (mAudioFrame.size() >= 1100) {
		// set buffer size
		mGist->setAudioFrameSize(mAudioFrame.size());
	  
		// process frame
		mGist->processAudioFrame(mAudioFrame);

		// Pitch Estimation
		float pitch = mGist->pitch();
			  
		// Compute pitch from frequency
		int A4 = 440;
		double C0 = A4 * pow(2, -4.75);
		double h = round(12 * log2(pitch / C0));
		int octave = floor(h / 12);
		int n = fmod(h, 12);
		
		// compute offset in cents
		double freq_nearest_note = C0 * pow(2, h / 12);
		double freq_ratio = pitch / freq_nearest_note;
		int cents = round(1200 * log2(freq_ratio));
		
		// update GUI
		// offset
		std::stringstream strs;
		strs << cents;
		std::string temp_str = strs.str();
		char* cents_str = (char*)temp_str.c_str();
		char centsString[30];
		strcpy(centsString, "Offset: ");
		strcat(centsString, cents_str);
		strcat(centsString, " cents");
		
		// pitch
		std::string note_str = name[n];
		char noteString[30];
		const char * c = note_str.c_str();
		std::stringstream strs1;
		strs1 << octave;
		std::string temp_str1 = strs1.str();
		char* octave_str = (char*)temp_str1.c_str();
		strcpy(noteString, "Pitch: ");
		strcat(noteString, c);
		strcat(noteString, octave_str);
		
		// frequency
		char freqString[30];
		sprintf(freqString, "Frequency: %.2f Hz", pitch);
		
		// Write strings to GUI
		msgZone->SetText(freqString);
		msgZone1->SetText(noteString);
		msgZone2->SetText(centsString);

#ifdef DEBUG
		DBOUT("Pitch: " << pitch << std::endl); // log into VS console
#endif
	
		// clear the audio frame for next iteration
		mAudioFrame.clear();
	}
}

void TunerPlug::Reset()
{
  TRACE;
  IMutexLock lock(this);
}

void TunerPlug::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    default:
      break;
  }
}
