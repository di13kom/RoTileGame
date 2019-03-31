#ifndef COMMON_H
#define COMMON_H
#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip> // std::setw
#include <memory>
#include <set>
//#include <future>
#include <numeric> //iota
//#include <array>
//#include <thread>
#include <tuple>
//
#include <unordered_set>
#include <exception>

namespace Solver
{

	struct _Nd
	{
		_Nd* Parent;
		int hValue;
		int gValue;
		int fValue;
		char* Positions;
		_Nd** Child;
	};

	//
	struct compHashFunc
	{
		bool operator()(const char* s1, const char* s2) const
		{
			bool retVal;
			if (strcmp(s1, s2) == 0)
				retVal = true;
			else
				retVal = false;

			return retVal;
		}
	};
	struct hashFunc
	{
		inline size_t operator()(const char* s1) const
		{
			//		int vl = 0;
			//		size_t retVal;
			//		int len = strlen(s1);
			//
			//		for(int i=0;i<len;i++)
			//		{
			//			vl += ((int)s1[i])*std::pow(len,i);
			//		}
			//		retVal = std::hash<int>()(vl);
			//
			//		return retVal;
			size_t result = 0;
			const size_t prime = 31;
			int len = strlen(s1);
			for (size_t i = 0; i < len; ++i) {
				result = s1[i] + (result * prime);
			}
			return result;
			//size_t h = 5381;
			//int c;
			//while ((c = *s1++))
			//	h = ((h << 5) + h) + c;
			//return h;
		}
	};

	///-----------
	struct Comp1Func
	{
		bool operator()(const _Nd *s1, const _Nd *s2) const
		{
			return s1->fValue < s2->fValue;
		}
	};

	struct Comp2Func
	{
		bool operator()(const char *s1, const char *s2) const
		{
			return strcmp(s1, s2) < 0;
		}
	};

	class Fs
	{
		protected:
			int ElementsInRow, IterationCount;
			char* Solution;
			//std::set<const char*, Comp2Func> UsedList;//Checking list for used combinations
			std::unordered_set<char*, hashFunc, compHashFunc> UsedList;//Checking list for used combinations
			_Nd *Node;
			std::vector<char*> BackList;
			virtual int Rotate(char M, _Nd*, char) = 0;
		public:
			virtual std::vector<char*> FindSolution(char*) = 0;
			Fs(int);
			Fs() = default;
			virtual int GetIteration() = 0;
	};

	//void *PrgsBar(void *ptr);
}
#endif
