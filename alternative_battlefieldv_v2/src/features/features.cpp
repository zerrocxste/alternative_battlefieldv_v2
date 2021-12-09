#include "../alternative.h"

namespace Features
{
	CFeatures::CFeatures() : m_pLocalClientSoldierEntity(0)
	{

	}

	CFeatures::~CFeatures()
	{
		this->m_vClientSoldierEntityList.clear();

		PatchDrawNameTagsAlwaysVisible(false);
		NoRecoil(false);
		IncreaseFireRate(false);
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

	void CFeatures::LockupRadarHackMutex()
	{
		this->m_mtxCollectData.lock();

		MainRadarHackWork();

		this->m_mtxCollectData.unlock();
	}

	void CFeatures::DrawEnemyInGameRadar()
	{
		LockupRadarHackMutex();
	}

	void CFeatures::ClientSoldierEntityListGrabber(DWORD_PTR ClientSoldierEntity)
	{
		if (!(std::find(this->m_vClientSoldierEntityList.begin(), 
			this->m_vClientSoldierEntityList.end(), 
			ClientSoldierEntity) 
			!= this->m_vClientSoldierEntityList.end())) 
		{
			this->m_mtxCollectData.lock();
			this->m_vClientSoldierEntityList.push_back(ClientSoldierEntity);
			this->m_mtxCollectData.unlock();
		}
	}

	void CFeatures::LocalpClientSoldierEntityGrabber(DWORD_PTR pClientSoldierEntity)
	{
		this->m_pLocalClientSoldierEntity = pClientSoldierEntity;
	}

	void CFeatures::PatchDrawNameTagsAlwaysVisible(bool bIsEnable)
	{
		static auto PatchAddressVisibleCheck = (memory_utils::pattern_scanner_module(memory_utils::get_base(), "\x45\x38\x00\x00\x00\x00\x00\x74\x00\x4D\x85", "xx?????x?xx") + 0x7);
		static auto PatchAddressWriteVaribles = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\x44\x88\x00\x00\x88\x5F", "xx??xx");
		static auto PatchAddressSmokeCheck = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\x75\x00\x48\x8B\x00\x00\x00\x00\x00\x4C\x8B\x00\x00\x00\x00\x00\x48\x8D", "x?xx?????xx?????xx");

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
		static auto PatchAddress = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\x75\x00\x49\x8B\x00\x00\x48\x81\xC6\x00\x00\x00\x00\x48\x89", "x?xx??xxx????xx");

		if (!PatchAddress)
		{
			Console::PrintLogTime(__FUNCTION__, "Sig not found");
			return;
		}

		if (bIsEnable)
		{
			memory_utils::patch_instruction(PatchAddress, "\x74", 1);
		}
		else
		{
			memory_utils::patch_instruction(PatchAddress, "\x75", 1);
		}
	}

	void CFeatures::NoRecoil(bool bIsEnable)
	{
		static auto WeaponRecoilPitchWriterInstruction = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\xF3\x41\x00\x00\x00\x00\x41\x0F\x00\x00\x76", "xx???xx????xx??x");
		static auto WeaponRecoilYawWriterInstruction = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x41\x00\x00\x00\x00\x41\x0F\x00\x00\x76", "xx???xx???xx????xx??x");

		if (!WeaponRecoilPitchWriterInstruction || !WeaponRecoilYawWriterInstruction)
		{
			Console::PrintLogTime(__FUNCTION__, "Sig not found");
			return;
		}

		if (bIsEnable)
		{
			memory_utils::fill_memory_region(WeaponRecoilPitchWriterInstruction, 0x90, 5);
			memory_utils::fill_memory_region(WeaponRecoilYawWriterInstruction, 0x90, 5);
		}
		else
		{
			BYTE bOriginalRecoilPitchInstruction[5] = { 0xf3, 0x0f, 0x11, 0x47, 0x7c };
			BYTE bOriginalRecoilYawInstruction[5] = { 0xf3, 0x0f, 0x11, 0x77, 0x78 };
			memory_utils::patch_instruction(WeaponRecoilPitchWriterInstruction, (const char*)&bOriginalRecoilPitchInstruction, 5);
			memory_utils::patch_instruction(WeaponRecoilYawWriterInstruction, (const char*)&bOriginalRecoilYawInstruction, 5);
		}
	}

	void CFeatures::IncreaseFireRate(bool bIsEnable, float flRate)
	{
		static auto WeaponFirerateWriterInstruction = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x85\xD2", "xx??????x????xx");

		if (!WeaponFirerateWriterInstruction)
			return;

		if (bIsEnable)
		{
			BYTE bFastfireratePatch[14] = { 0xc7, 0x87, 0xd4, 0x01, 0x00, 0x00, 0x0a, 0xd7, 0xa3, 0x3c, 0x90, 0x90, 0x90, 0x90 };
			memcpy(bFastfireratePatch + 0x6, &flRate, 4);
			memory_utils::patch_instruction(WeaponFirerateWriterInstruction - 0x6, (const char*)&bFastfireratePatch, 14);
		}
		else
		{
			BYTE bOriginalFirerateInstruction[14] = { 0x89, 0x87, 0x60, 0x01, 0x00, 0x00, 0xf3, 0x0f, 0x11, 0x87, 0xd4, 0x01, 0x00, 0x00 };
			memory_utils::patch_instruction(WeaponFirerateWriterInstruction - 0x6, (const char*)&bOriginalFirerateInstruction, 14);
		}
	}

	std::unique_ptr<CFeatures> pFeatures = std::make_unique<CFeatures>();
}