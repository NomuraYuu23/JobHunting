#include "IObject.h"

ID3D12GraphicsCommandList* IObject::commandList_ = nullptr;

void IObject::StaticInitialize(ID3D12GraphicsCommandList* commandList)
{

	commandList_ = commandList;

}
