#include "../alternative.h"

FrostbiteGui::CFrostbiteGui::CFrostbiteGui() :
	m_bMenuNewPosStarted(false),
	m_iStartPosX(0),
	m_iStartPosY(0),
	m_flBGSizeX(0.f),
	m_flBGSizeY(0.f),
	m_iNewPosX(0),
	m_iNewPosY(0),
	m_iCurrentlyTabItemsSize(0),
	m_iCurrentlyItemsCount(0),
	m_pMenuCurrentlySelected(nullptr),
	m_flFontSize(0.f),
	m_bReturnIsPressed(false),
	m_bReturnIsDowned(false),
	m_bReturnIsReleased(false),
	m_bPressedGoToNextItem(false),
	m_bPressedGoToPrevItem(false),
	m_bIsSameLine(false),
	m_flLastSameLine(0.f),
	m_flMaxXLine(0.f),
	m_pszWindowTitle(nullptr)
{
	memset(this->m_szLastItemLabel, 0, sizeof(this->m_szLastItemLabel));
}

FrostbiteGui::CFrostbiteGui::~CFrostbiteGui()
{

}

void FrostbiteGui::CFrostbiteGui::MenuStartPos(const char* pszName, std::uint32_t x, std::uint32_t y, std::uint32_t* iCurrentlyTabHovered, float flFontSize, std::uint32_t iSizeX, std::uint32_t iSizeY)
{
	using namespace KeyHelper;

	this->m_bMenuNewPosStarted = true;
	this->m_iStartPosX = x;
	this->m_iStartPosY = y;
	this->m_flBGSizeX = iSizeX;
	this->m_flBGSizeY = iSizeY;
	this->m_iNewPosX = this->m_iStartPosX;
	this->m_iNewPosY = this->m_iStartPosY;
	this->m_iCurrentlyTabItemsSize = 0;
	this->m_iCurrentlyItemsCount = 0;
	this->m_pMenuCurrentlySelected = iCurrentlyTabHovered;
	this->m_flFontSize = flFontSize;
	this->m_bReturnIsPressed = IsKeyPressed(VK_RETURN);
	this->m_bReturnIsDowned = IsKeyDowned(VK_RETURN);
	this->m_bReturnIsReleased = IsKeyReleased(VK_RETURN);
	this->m_bIsSameLine = false;
	this->m_flLastSameLine = 0.f;
	this->m_flMaxXLine = 0.f;
	memset(this->m_szLastItemLabel, 0, sizeof(this->m_szLastItemLabel));
	this->m_pszWindowTitle = (char*)pszName;

	this->m_iNewPosX += CalcSpaceOffset(iWindowInnerSpacing);
	this->m_iNewPosY += CalcSpaceOffset(iWindowInnerSpacing + iTitlebarSizeSizeY);

	this->m_bPressedGoToPrevItem = IsKeyDowned(VK_UP);
	this->m_bPressedGoToNextItem = IsKeyDowned(VK_DOWN);
}

void FrostbiteGui::CFrostbiteGui::MenuEndPos(__int64 pUnk)
{
	using namespace KeyHelper;
	using namespace FrostbiteFunctions::Drawing;

	if (!this->m_bMenuNewPosStarted)
		return;

	if (this->m_bPressedGoToPrevItem && *this->m_pMenuCurrentlySelected != 0)
		*this->m_pMenuCurrentlySelected -= 1;

	if (this->m_bPressedGoToNextItem && *this->m_pMenuCurrentlySelected < this->m_iCurrentlyTabItemsSize - 1)
		*this->m_pMenuCurrentlySelected += 1;

	if (!this->m_flBGSizeX)
		this->m_flBGSizeX = this->m_flMaxXLine;

	if (!this->m_flBGSizeY)
		this->m_flBGSizeY = CalcSpaceOffset(iTitlebarSizeSizeY + iWindowInnerSpacing) + CalcSpaceOffset(this->m_iItemSpaceSize * this->m_iCurrentlyItemsCount) + CalcSpaceOffset(iWindowInnerSpacing);
		
	DrawEngineFilledRect(pUnk, this->m_iStartPosX, this->m_iStartPosY, 
		this->m_iStartPosX + this->m_flBGSizeX, this->m_iStartPosY + this->m_flBGSizeY, 
		Color::Grey());

	DrawEngineFilledRect(pUnk,
		this->m_iStartPosX, this->m_iStartPosY,
		this->m_iStartPosX + this->m_flBGSizeX, this->m_iStartPosY + CalcSpaceOffset(18),
		Color::White());

	DrawEngineText(pUnk, this->m_iStartPosX + CalcSpaceOffset(3), this->m_iStartPosY + CalcSpaceOffset(3), this->m_pszWindowTitle, Color::Black(), this->m_flFontSize);

	this->m_bMenuNewPosStarted = false;
	this->m_iStartPosX = this->m_iStartPosY = 0;
	this->m_flBGSizeX = this->m_flBGSizeY = 0.f;
	this->m_iNewPosX = this->m_iNewPosY = 0;
	this->m_iCurrentlyTabItemsSize = 0;
	this->m_iCurrentlyItemsCount = 0;
	this->m_pMenuCurrentlySelected = nullptr;
	this->m_flFontSize = 0.f;
	this->m_bReturnIsPressed = false;
	this->m_bReturnIsDowned = false;
	this->m_bReturnIsReleased = false;
	this->m_bPressedGoToNextItem = false;
	this->m_bPressedGoToPrevItem = false;
	this->m_bIsSameLine = false;
	this->m_flLastSameLine = 0.f;
	this->m_flMaxXLine = 0.f;
	memset(this->m_szLastItemLabel, 0, sizeof(this->m_szLastItemLabel));
	this->m_pszWindowTitle = nullptr;
}

bool FrostbiteGui::CFrostbiteGui::AddCheckbox(__int64 pUnk, const char* pszText, bool* pVarible)
{
	using namespace FrostbiteFunctions::Drawing;

	auto bClicked = false;

	if (!this->m_bMenuNewPosStarted || !pVarible || strlen(pszText) > iNameMaxSize)
		return bClicked;

	auto bCurrentItemHovered = this->m_iCurrentlyTabItemsSize == *this->m_pMenuCurrentlySelected;
	auto bCurrentItemClicked = bCurrentItemHovered && this->m_bReturnIsPressed;

	auto flLine = 0.f;
	if (this->m_bIsSameLine)
		flLine = this->m_flLastSameLine;

	memset(this->m_szLastItemLabel, 0, sizeof(this->m_szLastItemLabel));
	*pVarible ? strcpy(this->m_szLastItemLabel, "[x] ") : strcpy(this->m_szLastItemLabel, "[ ] ");
	strcpy(this->m_szLastItemLabel + 4, pszText);

	auto color = bCurrentItemClicked ? Color::Blue() : bCurrentItemHovered ? Color::Red() : Color::Green();
	
	DrawEngineText(pUnk, this->m_iNewPosX + flLine, this->m_iNewPosY + (CalcSpaceOffset(this->m_iItemSpaceSize) * this->m_iCurrentlyItemsCount), this->m_szLastItemLabel, color, this->m_flFontSize);

	this->m_iCurrentlyTabItemsSize++;
	this->m_iCurrentlyItemsCount++;

	if (!this->m_bIsSameLine)
		this->m_flLastSameLine = 0.f;
	this->m_bIsSameLine = false;

	auto flCurrentMaxXLine = CalcSpaceOffset(iWindowInnerSpacing) + flLine + CalcTextLength(this->m_szLastItemLabel) + CalcSpaceOffset(iWindowInnerSpacing);
	if (this->m_flMaxXLine < flCurrentMaxXLine)
		this->m_flMaxXLine = flCurrentMaxXLine;

	if (bCurrentItemHovered && 
		this->m_bReturnIsReleased)
	{
		bClicked = true;
		*pVarible = !*pVarible;
	}

	return bClicked;
}

void FrostbiteGui::CFrostbiteGui::AddText(__int64 pUnk, const char* pszText)
{
	using namespace FrostbiteFunctions::Drawing;

	if (!this->m_bMenuNewPosStarted)
		return;

	auto flLine = 0.f;
	if (this->m_bIsSameLine)
		flLine = this->m_flLastSameLine;

	memset(this->m_szLastItemLabel, 0, sizeof(this->m_szLastItemLabel));
	strcpy(this->m_szLastItemLabel, pszText);

	DrawEngineText(pUnk, this->m_iNewPosX + flLine, this->m_iNewPosY + (CalcSpaceOffset(this->m_iItemSpaceSize) * this->m_iCurrentlyItemsCount), this->m_szLastItemLabel, Color::Green(), this->m_flFontSize);

	this->m_iCurrentlyItemsCount++;

	auto flCurrentMaxXLine = CalcSpaceOffset(iWindowInnerSpacing) + flLine + CalcTextLength(this->m_szLastItemLabel) + CalcSpaceOffset(iWindowInnerSpacing);
	if (this->m_flMaxXLine < flCurrentMaxXLine)
		this->m_flMaxXLine = flCurrentMaxXLine;
}

float FrostbiteGui::CFrostbiteGui::CalcTextLength(char* szText)
{
	return strlen(szText) * CalcSpaceOffset(7);
}

float FrostbiteGui::CFrostbiteGui::CalcSpaceOffset(int iOfs)
{
	return iOfs * this->m_flFontSize;
}

void FrostbiteGui::CFrostbiteGui::SameLine() 
{
	this->m_bIsSameLine = true;
	this->m_iCurrentlyItemsCount--;
	this->m_flLastSameLine += CalcTextLength(this->m_szLastItemLabel) + CalcSpaceOffset(this->m_iItemSpaceSize);
}