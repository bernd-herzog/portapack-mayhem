#pragma once

template<class cppCallType, class TargetClass>
struct infoHolder{
	TargetClass *tClass;
	cppCallType cppCallValue;
};

#define MakeWrapper(wapperName, functionArguments, functionCallArguments, targetClass, returnValue) \
struct wapperName{ \
	typedef returnValue (*cCall) functionArguments; \
	typedef returnValue (targetClass::*cppCall) functionArguments; \
	static infoHolder<wapperName::cppCall, targetClass> infoHolderInstance; \
	template<class cCallType, class cppCallType, class wapperNameType> \
	static cCallType getWrapperFunc(targetClass *tClass, cppCallType cppCallValue){ \
	wapperNameType::infoHolderInstance.tClass = tClass; \
	wapperNameType::infoHolderInstance.cppCallValue = cppCallValue; \
struct FunctionHolderClass{ \
	\
	static returnValue wrapperFunction functionArguments{ \
	return ((*wapperNameType::infoHolderInstance.tClass).*(wapperNameType::infoHolderInstance.cppCallValue)) functionCallArguments; \
} \
}; \
	return FunctionHolderClass::wrapperFunction; \
} \
}; infoHolder<wapperName::cppCall, targetClass> wapperName::infoHolderInstance;

#define GetWrapper(wapperName, func) (wapperName::getWrapperFunc<wapperName::cCall, wapperName::cppCall, wapperName>(this, func))
#define GetWrapperWithArgument(wapperName, func, obj) (wapperName::getWrapperFunc<wapperName::cCall, wapperName::cppCall, wapperName>(obj, func))
