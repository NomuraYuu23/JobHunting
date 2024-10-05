#include "ClothDemoObject.h"
#include "../../../Engine/3D/Model/ModelDraw.h"

void ClothDemoObject::Draw(BaseCamera& camera)
{

    ModelDraw::NormalObjectDesc desc;

    desc.model = model_.get();
    desc.material = material_.get();
    desc.camera = &camera;
    desc.worldTransform = &worldTransform_;

    ModelDraw::NormalObjectDraw(desc);

}
