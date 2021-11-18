#include "../alternative.h"

namespace Features
{
	CFeatures::CFeatures()
	{

	}

	CFeatures::~CFeatures()
	{

	}

	void CFeatures::DrawEnemyInGameRadar()
	{
		this->m_mtxCollentData.lock();

		int iMyTeamIdx = INT_MAX;

		if (this->m_vClientSoldierEntityList.size())
		{
			DWORD_PTR LocalClientSoldierEntity = NULL;

			if (memory_utils::is_valid_ptr((void*)this->m_pLocalClientSoldierEntity))
			{
				LocalClientSoldierEntity = memory_utils::read_value<DWORD_PTR>({ this->m_pLocalClientSoldierEntity });

				if (memory_utils::is_valid_ptr((void*)LocalClientSoldierEntity))
				{
					auto HealthComponent = memory_utils::read_value<DWORD_PTR>({ LocalClientSoldierEntity, 0x2E8 });

					if (HealthComponent)
					{
						auto flHealth = memory_utils::read_value<float>({ HealthComponent, 0x20 });
						auto flMaxHealth = memory_utils::read_value<float>({ HealthComponent, 0x24 });

						if (flHealth > 0.1f && flHealth <= flMaxHealth)
						{
							iMyTeamIdx = memory_utils::read_value<int>({ LocalClientSoldierEntity, 0x234 });
						}
					}
				}
			}

			if (iMyTeamIdx != INT_MAX)
			{
				for (auto it = this->m_vClientSoldierEntityList.begin(); it < this->m_vClientSoldierEntityList.end(); it++)
				{
					auto ClientSoldierEntity = *it;

					if (ClientSoldierEntity == LocalClientSoldierEntity)
						continue;

					if (memory_utils::is_valid_ptr((void*)ClientSoldierEntity))
					{
						auto HealthComponent = memory_utils::read_value<DWORD_PTR>({ ClientSoldierEntity, 0x2E8 });

						if (!HealthComponent)
							continue;

						auto iTeamIdx = memory_utils::read_value<int>({ ClientSoldierEntity, 0x234 });

						if (iTeamIdx < 1 || iTeamIdx > 64) //firestorm have > 2 team
							goto erase;

						if (iMyTeamIdx == iTeamIdx)
							continue;

						auto flHealth = memory_utils::read_value<float>({ HealthComponent, 0x20 });
						auto flMaxHealth = memory_utils::read_value<float>({ HealthComponent, 0x24 });

						if (flMaxHealth > 150.f || flHealth < 0.1f || flHealth > flMaxHealth)
							continue;

						auto ClientSpottingTargetComponent = memory_utils::read_value<DWORD_PTR>({ ClientSoldierEntity, 0xAF8 });

						if (ClientSpottingTargetComponent)
						{
							memory_utils::write({ ClientSpottingTargetComponent, 0x58 }, 1);
						}
					}
					else
					{
					erase:
						this->m_vClientSoldierEntityList.erase(it);
					}
				}
			}
		}
		this->m_mtxCollentData.unlock();
	}

	void CFeatures::PatchDrawNameTagsAlwaysVisible(bool bIsEnable)
	{
		static auto PatchAddress = (memory_utils::pattern_scanner_module(memory_utils::get_base(), "\x45\x38\x00\x00\x00\x00\x00\x74\x00\x4D\x85", "xx?????x?xx") + 0x7);
		
		if (!PatchAddress)
		{
			Console::PrintLogTime(__FUNCTION__, "Sig not found");
			return;
		}

		if (bIsEnable)
			memory_utils::fill_memory_region(PatchAddress, 0xEB, 1);
		else
			memory_utils::fill_memory_region(PatchAddress, 0x74, 1);

	}

	void CFeatures::ClientSoldierEntityListGrabber(DWORD_PTR ClientSoldierEntity)
	{
		if (!(std::find(this->m_vClientSoldierEntityList.begin(), 
			this->m_vClientSoldierEntityList.end(), 
			ClientSoldierEntity) 
			!= this->m_vClientSoldierEntityList.end())) 
		{
			this->m_mtxCollentData.lock();
			this->m_vClientSoldierEntityList.push_back(ClientSoldierEntity);
			this->m_mtxCollentData.unlock();
		}
	}

	void CFeatures::LocalpClientSoldierEntityGrabber(DWORD_PTR pClientSoldierEntity)
	{
		this->m_pLocalClientSoldierEntity = pClientSoldierEntity;
	}

	void CFeatures::NoRecoil(bool bIsEnable)
	{
		static auto WeaponRecoilPitchWriterInstruction = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\xF3\x41\x00\x00\x00\x00\x41\x0F\x00\x00\x76", "xx???xx????xx??x");
		static auto WeaponRecoilYawWriterInstruction = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\xF3\x0F\x00\x00\x00\xF3\x41\x00\x00\x00\x00\x41\x0F\x00\x00\x76", "xx???xx???xx????xx??x");

		if (!WeaponRecoilPitchWriterInstruction || !WeaponRecoilYawWriterInstruction)
			return;

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

	void CFeatures::NoSpread(bool bIsEnable)
	{
		static auto WeaponSpreadWriterInstruction = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\xF3\x41\x00\x00\x00\x0F\x2F\x00\x00\x00\x00\x00\x76", "xx???xx???xx?????x");

		if (!WeaponSpreadWriterInstruction)
			return;

		if (bIsEnable)
		{
			BYTE bSpreadPatch[10] = { 0xc7, 0x47, 0x48, 0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x90 };
			memory_utils::patch_instruction(WeaponSpreadWriterInstruction, (const char*)&bSpreadPatch, 10);
		}
		else
		{
			BYTE bOriginalSpreadInstruction[10] = { 0xf3, 0x0f, 0x11, 0x47, 0x48, 0xf3, 0x41, 0x0f, 0x5c, 0xf8 };
			memory_utils::patch_instruction(WeaponSpreadWriterInstruction, (const char*)&bOriginalSpreadInstruction, 10);
		}
	}

	void CFeatures::IncreaseFireRate(bool bIsEnable, float flRate)
	{
		static auto WeaponFirerateWriterInstruction = memory_utils::pattern_scanner_module(memory_utils::get_base(), "\xF3\x0F\x00\x00\x00\x00\x00\x00\xE9\x00\x00\x00\x00\x85\xD2", "xx??????x????xx");

		if (!WeaponFirerateWriterInstruction)
			return;

		if (bIsEnable)
		{
			BYTE bSpreadPatch[14] = { 0xc7, 0x87, 0xd4, 0x01, 0x00, 0x00, 0x0a, 0xd7, 0xa3, 0x3c, 0x90, 0x90, 0x90, 0x90 };
			memcpy(bSpreadPatch + 0x6, &flRate, 4);
			memory_utils::patch_instruction(WeaponFirerateWriterInstruction - 0x6, (const char*)&bSpreadPatch, 14);
		}
		else
		{
			BYTE bOriginalFirerateInstruction[14] = { 0x89, 0x87, 0x60, 0x01, 0x00, 0x00, 0xf3, 0x0f, 0x11, 0x87, 0xd4, 0x01, 0x00, 0x00 };
			memory_utils::patch_instruction(WeaponFirerateWriterInstruction - 0x6, (const char*)&bOriginalFirerateInstruction, 14);
		}
	}

	std::unique_ptr<CFeatures> g_pFeatures = std::make_unique<CFeatures>();
}