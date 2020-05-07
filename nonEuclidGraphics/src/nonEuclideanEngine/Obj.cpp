
#include<core/Obj.h>

using namespace cgcore;



void Obj::Transform(matf3 S)
{
	mesh->Transform(center, S, R);
}
