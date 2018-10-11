#include "SceneFactory.h"

SceneFactory::SceneFactory()
{
}

SceneFactory::~SceneFactory()
{
}

BaseScene* SceneFactory::Create(const enSwitchToNextScene enNextScene, const SceneNeedPointer ScenePointerGroup)
{
	assert(enNextScene >= enSwitchToNextScene::Title);
	assert(enNextScene < enSwitchToNextScene::Nothing);

	switch (enNextScene)
	{
	case enSwitchToNextScene::Title:
		return new TitleScene(ScenePointerGroup);

		break;
	case enSwitchToNextScene::Action:
		return new ActionScene(ScenePointerGroup);

		break;
	case enSwitchToNextScene::Clear:
		return new ClearScene(ScenePointerGroup);

		break;
	case enSwitchToNextScene::Over:
		return new OverScene(ScenePointerGroup);

		break;
	}

	return nullptr;
}