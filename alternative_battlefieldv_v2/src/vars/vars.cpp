#include "../alternative.h"

namespace Vars
{
	CVars::CVars()
	{
		memset(this, 0, sizeof(*this));

		LoadDefaultVars();
	}

	CVars::~CVars()
	{

	}

	void CVars::LoadDefaultVars()
	{
		this->m_HackVars.m_bRadarActive = true;
		//
	}

	std::unique_ptr<CVars> pVars = std::make_unique<CVars>();
}