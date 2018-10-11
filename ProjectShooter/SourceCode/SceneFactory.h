#pragma once
#pragma warning( disable:4005 )

#include "Global.h"

#include "TitleScene.h"
#include "ActionScene.h"
#include "ClearScene.h"
#include "OverScene.h"

class SceneFactory
{
public:
	SceneFactory();
	~SceneFactory();

	BaseScene* Create(const enSwitchToNextScene enNextScene, const SceneNeedPointer ScenePointerGroup);

private:

};