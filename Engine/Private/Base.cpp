#include "Base.h"

Base::Base()
{
}

unsigned int Base::AddRef()
{	
	return ++m_iRefCnt;
}

unsigned int Base::Release()
{
	if (0 == m_iRefCnt)
	{
		Free();

		/* Áö¿ìÀÚ. */
		delete this;

		return 0;
	}
	else
		return m_iRefCnt--;
}

void Base::Free()
{
}