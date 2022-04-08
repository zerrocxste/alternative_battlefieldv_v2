#pragma once

class IError
{
private:
	char m_szWhat[255];
public:
	IError();
	~IError();

	const char* What();
protected:
	virtual void ShowError() = 0;
	void SetError(const char* szErrorText, ...);
};