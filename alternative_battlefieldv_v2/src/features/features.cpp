#include "../alternative.h"

namespace Features
{
	static auto PatchAddressVisibleCheck = (memory_utils::pattern_scanner_module(memory_utils::get_base(), "\x45\x38\x00\x00\x00\x00\x00\x74\x00\x4D\x85", "xx?????x?xx") + 0x7);
	static auto PatchAddressWriteVaribles = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\x44\x88\x00\x00\x88\x5F", "xx??xx");
	static auto PatchAddressSmokeCheck = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\x75\x00\x48\x8B\x00\x00\x00\x00\x00\x4C\x8B\x00\x00\x00\x00\x00\x48\x8D", "x?xx?????xx?????xx");
	static auto PatchAddressExtendedNametag = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\x75\x00\x49\x8B\x00\x00\x48\x81\xC6\x00\x00\x00\x00\x48\x89", "x?xx??xxx????xx");
	static auto PatchAddressScopeReloading = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\x74\x00\x45\x89\x00\x00\x00\x00\x00\x44\x38", "x?xx?????xx");
	static auto PatchAddressWeaponPitch = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\xF3\x41\x00\x00\x00\x00\x41\x0F\x00\x00\x76", "xx???xx????xx??x");
	static auto PatchAddressWeaponYaw = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x41\x00\x00\x00\x00\x41\x0F\x00\x00\x76", "xx???xx???xx????xx??x");
	static auto PatchAddressFirerate = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x85\xD2", "xx??????x????xx");

	CFeatures::CFeatures() : 
		m_pLocalClientSoldierEntity(0), 
		m_pFrostbiteGui(std::make_unique<FrostbiteGui::CFrostbiteGui>())
	{
		InitializeCriticalSection(&this->m_csCollectData);
	}

	CFeatures::~CFeatures()
	{
		DeleteCriticalSection(&this->m_csCollectData);

		PatchDrawNameTagsAlwaysVisible(false);
		PatchNameTagDrawExtendedInfo(false);
		NoRecoil(false);
		IncreaseFireRate(false);
		PatchInScopeReloading(false);
	}

	void CFeatures::MainRadarHackWork()
	{
		if (!this->m_vClientSoldierEntityList.size())
			return;

		if (!memory_utils::is_valid_ptr((void*)this->m_pLocalClientSoldierEntity))
		{
			this->m_pLocalClientSoldierEntity = NULL;
			return;
		}

		auto LocalClientSoldierEntity = memory_utils::read_value<DWORD_PTR>({ this->m_pLocalClientSoldierEntity });

		if (!LocalClientSoldierEntity)
			return;

		auto LocalPlayerHealthComponent = memory_utils::read_value<DWORD_PTR>({ LocalClientSoldierEntity, 0x2E8 });

		if (!LocalPlayerHealthComponent)
			return;

		auto flLocalPlayerHealth = memory_utils::read_value<float>({ LocalPlayerHealthComponent, 0x20 });
		auto flLocalPlayerMaxHealth = memory_utils::read_value<float>({ LocalPlayerHealthComponent, 0x24 });

		if (flLocalPlayerHealth <= 0.1f || flLocalPlayerHealth > flLocalPlayerMaxHealth)
			return;

		auto iLocalPlayerTeamIdx = memory_utils::read_value<int>({ LocalClientSoldierEntity, 0x234 });

		for (auto it = this->m_vClientSoldierEntityList.begin(); it < this->m_vClientSoldierEntityList.end(); it++)
		{
			auto ClientSoldierEntity = *it;

			if (ClientSoldierEntity == LocalClientSoldierEntity)
			{
				this->m_vClientSoldierEntityList.erase(it);
				continue;
			}

			auto HealthComponent = memory_utils::read_value<DWORD_PTR>({ ClientSoldierEntity, 0x2E8 });

			if (!HealthComponent)
			{
				this->m_vClientSoldierEntityList.erase(it);
				continue;
			}

			auto iTeamIdx = memory_utils::read_value<int>({ ClientSoldierEntity, 0x234 });

			if (iTeamIdx < 1 || iTeamIdx > 64) //firestorm have > 2 team
			{
				this->m_vClientSoldierEntityList.erase(it);
				continue;
			}

			if (iLocalPlayerTeamIdx == iTeamIdx)
			{
				this->m_vClientSoldierEntityList.erase(it);
				continue;
			}

			auto flHealth = memory_utils::read_value<float>({ HealthComponent, 0x20 });
			auto flMaxHealth = memory_utils::read_value<float>({ HealthComponent, 0x24 });

			if (flMaxHealth > 150.f || flHealth < 0.1f || flHealth > flMaxHealth)
			{
				this->m_vClientSoldierEntityList.erase(it);
				continue;
			}

			auto ClientSpottingTargetComponent = memory_utils::read_value<DWORD_PTR>({ ClientSoldierEntity, 0xAF8 });

			if (!ClientSpottingTargetComponent)
			{
				this->m_vClientSoldierEntityList.erase(it);
				continue;
			}

			memory_utils::write({ ClientSpottingTargetComponent, 0x58 }, 1);
		}
	}

	void CFeatures::DrawEnemyInGameRadar()
	{
		EnterCriticalSection(&this->m_csCollectData);
		MainRadarHackWork();
		LeaveCriticalSection(&this->m_csCollectData);
	}

	void CFeatures::ClientSoldierEntityListGrabber(DWORD_PTR ClientSoldierEntity)
	{
		if (!(std::find(this->m_vClientSoldierEntityList.begin(), 
			this->m_vClientSoldierEntityList.end(), 
			ClientSoldierEntity) 
			!= this->m_vClientSoldierEntityList.end())) 
		{
			EnterCriticalSection(&this->m_csCollectData);
			this->m_vClientSoldierEntityList.push_back(ClientSoldierEntity);
			LeaveCriticalSection(&this->m_csCollectData);
		}
	}

	void CFeatures::LocalpClientSoldierEntityGrabber(DWORD_PTR pClientSoldierEntity)
	{
		this->m_pLocalClientSoldierEntity = pClientSoldierEntity;
	}

	void CFeatures::PatchDrawNameTagsAlwaysVisible(bool bIsEnable)
	{
		if (!PatchAddressVisibleCheck || !PatchAddressWriteVaribles || !PatchAddressSmokeCheck)
		{
			Console::PrintLogTime(__FUNCTION__, "Sig not found");
			return;
		}

		constexpr auto iSmokeCheckInstructionSize = 7;
		static BYTE bSmokeCheckPatch[iSmokeCheckInstructionSize] = { 0xC7, 0x47, *(BYTE*)(PatchAddressWriteVaribles + 0x3), 0x37, 0x13, 0x00, 0x00 };
		static BYTE* pbSmokeCheckCodeBackup = (BYTE*)memcpy(new BYTE[iSmokeCheckInstructionSize], (void*)PatchAddressWriteVaribles, iSmokeCheckInstructionSize);

		if (bIsEnable)
		{
			memory_utils::patch_instruction(PatchAddressVisibleCheck, "\xEB", 1);
			memory_utils::patch_instruction(PatchAddressWriteVaribles, (const char*)&bSmokeCheckPatch, iSmokeCheckInstructionSize);
			memory_utils::patch_instruction(PatchAddressSmokeCheck, "\xEB", 1);
		}
		else
		{
			memory_utils::patch_instruction(PatchAddressVisibleCheck, "\x74", 1);
			memory_utils::patch_instruction(PatchAddressWriteVaribles, (const char*)pbSmokeCheckCodeBackup, iSmokeCheckInstructionSize);
			memory_utils::patch_instruction(PatchAddressSmokeCheck, "\x75", 1);
		}
	}

	void CFeatures::PatchNameTagDrawExtendedInfo(bool bIsEnable)
	{
		if (!PatchAddressExtendedNametag)
		{
			Console::PrintLogTime(__FUNCTION__, "Sig not found");
			return;
		}

		if (bIsEnable)
		{
			memory_utils::patch_instruction(PatchAddressExtendedNametag, "\x74", 1);
		}
		else
		{
			memory_utils::patch_instruction(PatchAddressExtendedNametag, "\x75", 1);
		}
	}

	void CFeatures::NoRecoil(bool bIsEnable)
	{
		if (!PatchAddressWeaponPitch || !PatchAddressWeaponYaw)
		{
			Console::PrintLogTime(__FUNCTION__, "Sig not found");
			return;
		}

		if (bIsEnable)
		{
			memory_utils::fill_memory_region(PatchAddressWeaponPitch, 0x90, 5);
			memory_utils::fill_memory_region(PatchAddressWeaponYaw, 0x90, 5);
		}
		else
		{
			static BYTE bOriginalRecoilPitchInstruction[5] = { 0xf3, 0x0f, 0x11, 0x47, 0x7c };
			static BYTE bOriginalRecoilYawInstruction[5] = { 0xf3, 0x0f, 0x11, 0x77, 0x78 };
			memory_utils::patch_instruction(PatchAddressWeaponPitch, (const char*)&bOriginalRecoilPitchInstruction, 5);
			memory_utils::patch_instruction(PatchAddressWeaponYaw, (const char*)&bOriginalRecoilYawInstruction, 5);
		}
	}

	void CFeatures::IncreaseFireRate(bool bIsEnable, float flRate)
	{
		if (!PatchAddressFirerate)
			return;

		if (bIsEnable)
		{
			static BYTE bFastfireratePatch[14] = { 0xc7, 0x87, 0xd4, 0x01, 0x00, 0x00, 0x0a, 0xd7, 0xa3, 0x3c, 0x90, 0x90, 0x90, 0x90 };
			memcpy(bFastfireratePatch + 0x6, &flRate, 4);
			memory_utils::patch_instruction(PatchAddressFirerate - 0x6, (const char*)&bFastfireratePatch, 14);
		}
		else
		{
			static BYTE bOriginalFirerateInstruction[14] = { 0x89, 0x87, 0x60, 0x01, 0x00, 0x00, 0xf3, 0x0f, 0x11, 0x87, 0xd4, 0x01, 0x00, 0x00 };
			memory_utils::patch_instruction(PatchAddressFirerate - 0x6, (const char*)&bOriginalFirerateInstruction, 14);
		}
	}

	void CFeatures::PatchInScopeReloading(bool bIsEnable)
	{
		if (!PatchAddressScopeReloading)
			return;

		if (bIsEnable)
		{
			memory_utils::patch_instruction(PatchAddressScopeReloading, "\xEB", 1);
		}
		else
		{
			memory_utils::patch_instruction(PatchAddressScopeReloading, "\x74", 1);
		}
	}

	void CFeatures::DrawMenu(__int64 pUnk)
	{
		using namespace Vars;

		static std::uint32_t iCurrenlyItemHovered = 0;

		this->m_pFrostbiteGui->NewWindow("alternative hack | by zerroxste", 20, 50, &iCurrenlyItemHovered, 2.f);

		this->m_pFrostbiteGui->AddCheckbox(pUnk, "In game radar", &pVars->m_HackVars.m_bRadarActive);

		if (this->m_pFrostbiteGui->AddCheckbox(pUnk, "Nametags always visible", &pVars->m_HackVars.m_bNameTagsAlwaysVisible))
			PatchDrawNameTagsAlwaysVisible(pVars->m_HackVars.m_bNameTagsAlwaysVisible);

		if (this->m_pFrostbiteGui->AddCheckbox(pUnk, "Nametag extended info", &pVars->m_HackVars.m_bNameTagDrawExtendedInfo))
			PatchNameTagDrawExtendedInfo(pVars->m_HackVars.m_bNameTagDrawExtendedInfo);

		if (this->m_pFrostbiteGui->AddCheckbox(pUnk, "Norecoil (Ban risk)", &pVars->m_HackVars.m_bNoRecoil))
			NoRecoil(pVars->m_HackVars.m_bNoRecoil);

		if (this->m_pFrostbiteGui->AddCheckbox(pUnk, "High firerate (Ban risk)", &pVars->m_HackVars.m_bIncreaseFireRate))
			IncreaseFireRate(pVars->m_HackVars.m_bIncreaseFireRate);

		this->m_pFrostbiteGui->AddText(pUnk, "High ban risk when use Norecoil and High firerate at same time!");

		if (this->m_pFrostbiteGui->AddCheckbox(pUnk, "Reload in scope", &pVars->m_HackVars.m_bReloadInScope))
			PatchInScopeReloading(pVars->m_HackVars.m_bReloadInScope);

		this->m_pFrostbiteGui->EndWindow(pUnk);
	}

	void CFeatures::DrawScreen(__int64 pUnk)
	{
		if (KeyHelper::IsKeyReleased(VK_INSERT))
			FrostbiteFunctions::Input::SetBlockInput(Vars::pVars->m_MenuVars.m_bMenuOpened = !Vars::pVars->m_MenuVars.m_bMenuOpened);

		if (Vars::pVars->m_MenuVars.m_bMenuOpened)
		{
			auto pResolution = FrostbiteFunctions::Drawing::GetGameScreenResolution();
			char buf[64]{ 0 };
			sprintf(buf, "Width: %d Height: %d", pResolution[0], pResolution[1]);
			FrostbiteFunctions::Drawing::DrawEngineText(pUnk, 10, 10, buf, Color::Aqua(), 1.5f);

			DrawMenu(pUnk);
		}
	}

	std::unique_ptr<CFeatures> pFeatures = std::make_unique<CFeatures>();
}

