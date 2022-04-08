#include "../alternative.h"

IError::IError() 
{

}

IError::~IError() 
{

}

const char* IError::What()
{
	return !*this->m_szWhat ? "NO_ERROR" : this->m_szWhat;
};

void IError::SetError(const char* szErrorText, ...)
{
	va_list va_l;

	va_start(va_l, szErrorText);
	vsprintf_s(this->m_szWhat, szErrorText, va_l);
	va_end(va_l);
}