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
		
	}

	std::unique_ptr<CVars> pVars = std::make_unique<CVars>();
}