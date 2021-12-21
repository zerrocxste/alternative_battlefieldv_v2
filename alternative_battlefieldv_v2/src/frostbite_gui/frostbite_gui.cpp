#include "../alternative.h"

CFrostbiteGui::CFrostbiteGui() : 
	m_bMenuNewPosStarted(false),
	m_iNewPosX(0), 
	m_iNewPosY(0),
	m_iCurrentlyTabItemsSize(0),
	m_piMenuCurrentlySelected(nullptr),
	m_flFontSize(0.f),
	m_bReturnIsPressed(false),
	m_bReturnIsDowned(false),
	m_bReturnIsReleased(false)
{

}

CFrostbiteGui::~CFrostbiteGui()
{

}

void CFrostbiteGui::MenuStartPos(__int64 pUnk, const char* pszName, int x, int y, int iSizeX, int iSizeY, std::uint32_t* iCurrentlyTabHovered, float flFontSize)
{
	using namespace KeyHelper;

	this->m_bMenuNewPosStarted = true;
	this->m_iNewPosX = x;
	this->m_iNewPosY = y;
	this->m_piMenuCurrentlySelected = iCurrentlyTabHovered;
	this->m_flFontSize = flFontSize;
	this->m_bReturnIsPressed = IsKeyPressed(VK_RETURN);
	this->m_bReturnIsDowned = IsKeyDowned(VK_RETURN);
	this->m_bReturnIsReleased = IsKeyReleased(VK_RETURN);

	FrostbiteDrawing::DrawEngineFilledRect(pUnk, this->m_iNewPosX, this->m_iNewPosY, this->m_iNewPosX + iSizeX, this->m_iNewPosY + iSizeY, Color::Grey());

	auto iTitleBarSize = 18.f * flFontSize;
	FrostbiteDrawing::DrawEngineFilledRect(pUnk, this->m_iNewPosX, this->m_iNewPosY, this->m_iNewPosX + iSizeX, this->m_iNewPosY + iTitleBarSize, Color::White());
	FrostbiteDrawing::DrawEngineText(pUnk, this->m_iNewPosX + 3, this->m_iNewPosY + 3, pszName, Color::Black(), this->m_flFontSize);

	this->m_iNewPosX += 10;
	this->m_iNewPosY += iTitleBarSize + 10;
}

void CFrostbiteGui::MenuEndPos()
{
	using namespace KeyHelper;

	if (IsKeyDowned(VK_UP) && *this->m_piMenuCurrentlySelected != 0)
		*this->m_piMenuCurrentlySelected -= 1;

	if (IsKeyDowned(VK_DOWN) && *this->m_piMenuCurrentlySelected < this->m_iCurrentlyTabItemsSize - 1)
		*this->m_piMenuCurrentlySelected += 1;

	this->m_bMenuNewPosStarted = false;
	this->m_iNewPosX = this->m_iNewPosY = 0;
	this->m_iCurrentlyTabItemsSize = 0;
	this->m_piMenuCurrentlySelected = nullptr;
	this->m_flFontSize = 0;
	this->m_bReturnIsDowned = false;
	this->m_bReturnIsPressed = false;
	this->m_bReturnIsReleased = false;
}

bool CFrostbiteGui::MenuAddTabCheckbox(__int64 pUnk, const char* pszTabName, bool* pVarible)
{
	constexpr auto iNameMaxSize = 64;

	auto bClicked = false;

	if (!this->m_bMenuNewPosStarted || strlen(pszTabName) > iNameMaxSize)
		return bClicked;

	auto bCurrentItemHovered = this->m_iCurrentlyTabItemsSize == *this->m_piMenuCurrentlySelected;
	auto bCurrentItemClicked = bCurrentItemHovered && this->m_bReturnIsPressed;

	auto color = Color::Green();
	if (bCurrentItemHovered)
		color = Color::Red();
	if (bCurrentItemClicked)
		color = Color::Blue();

	auto iItemSpace = 15 * this->m_flFontSize;

	char buf[3 + 1 + iNameMaxSize] = { 0 };
	*pVarible ? strcpy(buf, "[+] ") : strcpy(buf, "[-] ");
	strcpy(buf + 4, pszTabName);
	FrostbiteDrawing::DrawEngineText(pUnk, this->m_iNewPosX, this->m_iNewPosY + (iItemSpace * this->m_iCurrentlyTabItemsSize), buf, color, this->m_flFontSize);

	if (bCurrentItemHovered && this->m_bReturnIsReleased)
	{
		bClicked = true;
		*pVarible = !*pVarible;
	}

	this->m_iCurrentlyTabItemsSize++;

	return bClicked;
}