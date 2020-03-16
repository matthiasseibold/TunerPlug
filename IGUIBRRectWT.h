#include "IControl.h"

/*

 Text displayable on the plugin area

 */

class IGUIBRRectWT : public IPanelControl
{
protected:
	IRECT mWriteRECT;
	IRECT rcI;
	IColor mColorB, mColorI;
	int mRad;
	static const int ItxtBlgBrr = 64;  // max text size
	char Bffr[ItxtBlgBrr];

public:
	IGUIBRRectWT(IPlugBase* pPlug, IRECT pR, const IColor* pColorB, const IColor* pColorI, int cornerradius)
		: IPanelControl(pPlug, pR, pColorB), mColorB(*pColorB), mColorI(*pColorI), mRad(cornerradius)
	{
		rcI = pR.GetPadded(-1);
		mText = *new IText(16, &COLOR_BLACK, "Lucida Grande", IText::kStyleNormal, IText::kAlignNear,
			0, IText::kQualityAntiAliased);
		mWriteRECT = mRECT.GetHPadded(-6);

		for (int j = 0; j < ItxtBlgBrr; j++)
			Bffr[j] = 0;
	}

	~IGUIBRRectWT() {}

	bool Draw(IGraphics* pGraphics)
	{
		bool rz;
		rz = pGraphics->FillRoundRect(&mColorB, &mRECT, &mBlend, mRad, true);
		rz = rz && pGraphics->FillRoundRect(&mColorI, &rcI, &mBlend, mRad, true);
		rz = rz && pGraphics->DrawIText(&mText, Bffr, &mWriteRECT);
		return rz;
	}

	bool SetText(char * msg)
	{
		if (strncmp(Bffr, msg, ItxtBlgBrr))
		{
			strncpy(Bffr, msg, ItxtBlgBrr);
			Bffr[ItxtBlgBrr - 1] = 0;
			SetDirty(false);
		}
		return true;
	}
}; 