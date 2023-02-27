#pragma once
#include <iostream>
#include <stdlib.h>
#include <map>
#include "dataStruct.h"
namespace YB {
	template <typename T_Key,typename T_Value>
	bool keyInMap(std::map<T_Key,T_Value>&mp,T_Key key)
	{
		std::map<T_Key, T_Value>::iterator it = mp.find(key);
		return it != mp.end();
	}
	template<typename T>
	T InRang(T min, T max, T value)
	{
		if (value < min)
			return min;
		else if (value > max)
			return max;
		else
			return value;
	}
	
	inline int idNameInVector(const std::vector<showTableData>&Data, const QString &name)
	{
		for (int i = 0; i < Data.size(); i++)
		{
			if (Data.at(i).IdName == name)
			{
				return i;
			}
		}
		return -1;
	}

	bool nameInVector(std::vector<RollStruct>& vec, QString name, int& index);
	inline bool isExistKey(std::map<unsigned int, QStringList>& m, unsigned int key) {
		std::map<unsigned int, QStringList>::iterator iter;
		iter = m.find(key);
		if (iter != m.end()) return true;
		else return false;
	}
}