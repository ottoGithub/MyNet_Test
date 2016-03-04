#ifndef _CLASS_MANAGER_H_
#define _CLASS_MANAGER_H_
#include "TManager.h"

class CClass;
class ClassManager: public TManager<CClass>
{
public:
	void PrintClass();
	static ClassManager& GetSingleton()
	{
		static ClassManager m_instance;
		return m_instance;
	}
protected:
private:
	ClassManager(){}
	~ClassManager(){}
	ClassManager(const ClassManager&);
	ClassManager& operator=(const ClassManager&);
};

#define s_ClassManager ClassManager::GetSingleton()

#endif