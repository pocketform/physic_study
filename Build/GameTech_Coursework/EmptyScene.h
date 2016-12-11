#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include<sstream>

class EmptyScene : public Scene
{
public:
	EmptyScene(const std::string& friendly_name) : Scene(friendly_name) {}
	virtual ~EmptyScene() {}

	virtual void OnInitializeScene()	 override
	{
		SceneManager::Instance()->GetCamera()->SetPosition(Vector3(-3.0f, 10.0f, 10.0f));
		SceneManager::Instance()->GetCamera()->SetPitch(-20.f);
		PhysicsEngine::Instance()->SetDebugDrawFlags(1.0f);

		Scene::OnInitializeScene();
		this->AddGameObject(CommonUtils::BuildCuboidObject(
			"Ground", 
			Vector3(0.0f, -1.0f, 0.0f), 
			Vector3(20.0f, 1.0f, 20.0f), 
			true, 
			0.0f, 
			true, 
			false, 
			Vector4(0.2f, 0.5f, 1.0f, 1.0f)));
	}

	int num = 0;

	virtual void OnUpdateScene(float dt) override
	{
		Scene::OnUpdateScene(dt);

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J))
		{
		Matrix4 viewMtx = SceneManager::Instance()->GetCamera()->BuildViewMatrix();
		Vector3 viewDir = -Vector3(viewMtx[2], viewMtx[6], viewMtx[10]);

			ostringstream ballshootName;
			ballshootName << "baller" << num;

			Object* ballshoot = CommonUtils::BuildSphereObject(
				ballshootName.str().c_str(),
				SceneManager::Instance()->GetCamera()->GetPosition(),
				0.3f,
				true,
				0.1f,
				true,
				false,
				Vector4(1.f, 0.f, 0.f, 1.f));
			this->AddGameObject(ballshoot);

		Object* baller=	this->FindGameObject(ballshootName.str().c_str());


		baller->Physics()->SetLinearVelocity(viewDir * 10.f);

		num++;

		}
	}
};