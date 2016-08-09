
#ifndef __FLOATTRAITS_H__
#define __FLOATTRAITS_H__

#include <cmath>
#include <cfloat>


template<class FLOAT>
struct FloatTraits;


template<>
struct FloatTraits<float>
{
	static float epsilon;
};

template<>
struct FloatTraits<double>
{
	static double epsilon;
};


#endif //__FLOATTRAITS_H__