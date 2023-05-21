#ifndef _OBJCALLFUNCAAST_H
#define _OBJCALLFUNCAAST_H
#include "allinclude.h"
//example: a->func();
class ObjCallFuncAAST {
	string objName;
	string callFunc;
public:
	ObjCallFuncAAST() {};
	~ObjCallFuncAAST() {};
};
#endif