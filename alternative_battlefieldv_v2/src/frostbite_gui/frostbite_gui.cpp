#include "../alternative.h"

using namespace FrostbiteFunctions::Drawing;
using namespace KeyHelper;

FrostbiteGui::CFrostbiteGui::CFrostbiteGui()
{
	EraseWindowData();
}

FrostbiteGui::CFrostbiteGui::~CFrostbiteGui()
{

}

void FrostbiteGui::CFrostbiteGui::EraseWindowData()
{
	this->m_bMenuNewPosStarted = false;
	this->m_iStartPosX = this->m_iStartPosY = 0;
	this->m_flBGWidth = this->m_flBGHeight = 0.f;
	this->m_iNewPosX = this->m_iNewPosY = 0;
	this->m_iCurrentlyTabItemsSize = 0;
	this->m_iCurrentlyItemsCount = 0;
	this->m_pMenuCurrentlySelected = nullptr;
	this->m_flFontSize = 0.f;
	this->m_bReturnIsPressed = false;
	this->m_bReturnIsDowned = false;
	this->m_bReturnIsReleased = false;
	this->m_bIsSameLine = false;
	this->m_flLastSameLine = 0.f;
	this->m_flMaxWindowWidth = 0.f;
	memset(this->m_szLastItemLabel, 0, sizeof(this->m_szLastItemLabel));
	this->m_pszWindowTitle = nullptr;
}

void FrostbiteGui::CFrostbiteGui::NewWindow(const char* pszName, std::uint32_t x, std::uint32_t y, std::uint32_t* iCurrentlyTabHovered, float flFontSize, std::uint32_t iSizeX, std::uint32_t iSizeY)
{
	this->m_bMenuNewPosStarted = true;
	this->m_iStartPosX = x;
	this->m_iStartPosY = y;
	this->m_flBGWidth = iSizeX;
	this->m_flBGHeight = iSizeY;
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
	this->m_flMaxWindowWidth = 0.f;
	memset(this->m_szLastItemLabel, 0, sizeof(this->m_szLastItemLabel));
	this->m_pszWindowTitle = (char*)pszName;
	this->m_iNewPosX += CalcSpaceOffset(iWindowInnerSpacing);
	this->m_iNewPosY += CalcSpaceOffset(iWindowInnerSpacing + iTitlebarSizeHeight);
}

void FrostbiteGui::CFrostbiteGui::EndWindow(__int64 pUnk)
{
	if (!this->m_bMenuNewPosStarted)
		return;

	if (IsKeyDowned(VK_UP) && *this->m_pMenuCurrentlySelected != 0)
		*this->m_pMenuCurrentlySelected -= 1;

	if (IsKeyDowned(VK_DOWN) && *this->m_pMenuCurrentlySelected < this->m_iCurrentlyTabItemsSize - 1)
		*this->m_pMenuCurrentlySelected += 1;

	if (!this->m_flBGWidth)
		this->m_flBGWidth = this->m_flMaxWindowWidth;

	if (!this->m_flBGHeight)
		this->m_flBGHeight = CalcSpaceOffset(iTitlebarSizeHeight + iWindowInnerSpacing) + CalcSpaceOffset(this->m_iItemSpaceSize * this->m_iCurrentlyItemsCount) + CalcSpaceOffset(iWindowInnerSpacing);
		
	DrawEngineFilledRect(pUnk,
		this->m_iStartPosX, this->m_iStartPosY, 
		this->m_iStartPosX + this->m_flBGWidth, this->m_iStartPosY + this->m_flBGHeight,
		Color::Grey());

	DrawEngineFilledRect(pUnk,
		this->m_iStartPosX, this->m_iStartPosY,
		this->m_iStartPosX + this->m_flBGWidth, this->m_iStartPosY + CalcSpaceOffset(18),
		Color::White());

	DrawEngineText(pUnk, 
		this->m_iStartPosX + CalcSpaceOffset(3), this->m_iStartPosY + CalcSpaceOffset(3), 
		this->m_pszWindowTitle, 
		Color::Black(),
		this->m_flFontSize);

	EraseWindowData();
}

bool FrostbiteGui::CFrostbiteGui::AddCheckbox(__int64 pUnk, const char* pszText, bool* pVarible)
{
	auto bClicked = false;

	if (!this->m_bMenuNewPosStarted || !pVarible || strlen(pszText) > iNameMaxSize)
		return bClicked;

	auto bCurrentItemHovered = this->m_iCurrentlyTabItemsSize == *this->m_pMenuCurrentlySelected;
	auto bCurrentItemClicked = bCurrentItemHovered && this->m_bReturnIsPressed;

	auto flLine = GetActualWidthLine();

	memset(this->m_szLastItemLabel, 0, sizeof(this->m_szLastItemLabel));
	*pVarible ? strcpy(this->m_szLastItemLabel, "[x] ") : strcpy(this->m_szLastItemLabel, "[ ] ");
	strcpy(this->m_szLastItemLabel + 4, pszText);

	auto color = bCurrentItemClicked ? Color::Blue() : bCurrentItemHovered ? Color::Red() : Color::Green();
	
	DrawEngineText(pUnk, 
		this->m_iNewPosX + flLine, this->m_iNewPosY + (CalcSpaceOffset(this->m_iItemSpaceSize) * this->m_iCurrentlyItemsCount),
		this->m_szLastItemLabel, 
		color, 
		this->m_flFontSize);

	this->m_iCurrentlyTabItemsSize++;
	this->m_iCurrentlyItemsCount++;

	if (!this->m_bIsSameLine)
		this->m_flLastSameLine = 0.f;

	this->m_bIsSameLine = false;

	RecalcWindowWidth(flLine);

	if (bCurrentItemHovered && this->m_bReturnIsReleased)
	{
		bClicked = true;
		*pVarible = !*pVarible;
	}

	return bClicked;
}

void FrostbiteGui::CFrostbiteGui::AddText(__int64 pUnk, const char* pszText)
{
	if (!this->m_bMenuNewPosStarted)
		return;

	auto flLine = GetActualWidthLine();

	memset(this->m_szLastItemLabel, 0, sizeof(this->m_szLastItemLabel));
	strcpy(this->m_szLastItemLabel, pszText);

	DrawEngineText(pUnk, 
		this->m_iNewPosX + flLine, this->m_iNewPosY + (CalcSpaceOffset(this->m_iItemSpaceSize) * this->m_iCurrentlyItemsCount), 
		this->m_szLastItemLabel, 
		Color::Green(), 
		this->m_flFontSize);

	this->m_iCurrentlyItemsCount++;

	RecalcWindowWidth(flLine);
}

float FrostbiteGui::CFrostbiteGui::GetActualWidthLine()
{
	return this->m_bIsSameLine ? this->m_flLastSameLine : 0.f;
}

void FrostbiteGui::CFrostbiteGui::RecalcWindowWidth(float flCurrentWidthList)
{
	auto flCurrentMaxXLine = CalcSpaceOffset(iWindowInnerSpacing) + flCurrentWidthList + CalcTextLength(this->m_szLastItemLabel) + CalcSpaceOffset(iWindowInnerSpacing);

	if (this->m_flMaxWindowWidth < flCurrentMaxXLine)
		this->m_flMaxWindowWidth = flCurrentMaxXLine;
}

float FrostbiteGui::CFrostbiteGui::CalcTextLength(char* szText)
{
	return strlen(szText) * CalcSpaceOffset(7);
}

float FrostbiteGui::CFrostbiteGui::CalcSpaceOffset(int iValue)
{
	return iValue * this->m_flFontSize;
}

void FrostbiteGui::CFrostbiteGui::SameLine() 
{
	this->m_bIsSameLine = true;
	this->m_iCurrentlyItemsCount--;
	this->m_flLastSameLine += CalcTextLength(this->m_szLastItemLabel) + CalcSpaceOffset(this->m_iItemSpaceSize);
}