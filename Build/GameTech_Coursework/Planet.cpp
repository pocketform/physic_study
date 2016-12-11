#include "Planet.h"


void Planet::OnInitializeScene()
{
	num_ball         = 0;
	num_question_box = 0;
	num_raptor       = 0;
	num_Score        = 0;
	const int pyramid_stack_height = 7;

	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(-3.f, 0.0f, 40.0f));
	SceneManager::Instance()->GetCamera()->SetPitch(0.f);

	//PhysicsEngine::Instance()->SetGravity(Vector3(0.0f, 0.0f, 0.0f));
	PhysicsEngine::Instance()->SetDampingFactor(1.0f);						//No Damping
	PhysicsEngine::Instance()->SetDebugDrawFlags(DEBUGDRAW_FLAGS_COLLISIONNORMALS | DEBUGDRAW_FLAGS_COLLISIONVOLUMES);

	my_drawFlag = PhysicsEngine::Instance()->GetDebugDrawFlags();

	my_drawFlag ^= DEBUGDRAW_FLAGS_COLLISIONVOLUMES;
	my_drawFlag ^= DEBUGDRAW_FLAGS_COLLISIONNORMALS;

	Scene::OnInitializeScene();

	{
		this->AddGameObject(CommonUtils::Build_Planet_SphereObject(
			"Planet",
			Vector3(0.0f, 0.0f, 0.0f),
			10.0f,
			true,
			0.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
		
		Object* planet = this->FindGameObject("Planet");
		planet->Physics()->SetRestState(false);
	}//initialze the planet

	this->AddGameObject(CommonUtils::BuildCuboidObject(
		"HorizontalGround",
		Vector3(0.0f, -15.0f, -20.0f),
		Vector3(10.0f, 1.0f, 10.0f),
		true,
		0.0f,
		true,
		false,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f)));

	this->AddGameObject(CommonUtils::BuildCuboidObject(
		"VerticalGround",
		Vector3(0.0f, -15.0f, -20.0f),
		Vector3(10.0f, 10.0f, 1.0f),
		true,
		0.0f,
		true,
		false,
		Vector4(0.2f, 0.5f, 1.0f, 1.0f)));
	
	{
		ostringstream question_box;

		for (int y = 0; y < pyramid_stack_height; ++y)
		{
			for (int x = 0; x <= y; ++x)
			{
				question_box << "question_box" << num_question_box;

				this->AddGameObject(CommonUtils::Build_Box_CuboidObject(
					question_box.str().c_str(),
					Vector3((x - y * 0.5f), 3.0f + ((-1.f) + float(pyramid_stack_height - 1 - y)), 15.f),
					Vector3(0.5f, 0.5f, 0.5f),
					true,
					0.1f,
					true,
					true,
					Vector4(1.0f, 1.0f, 1.0f, 1.0f)));
				
				Object* question_box_object = this->FindGameObject(question_box.str().c_str());
				question_box_object->Physics()->SetRestState(true);
				question_box_object->Physics()->SetInCourseWork(true);

				num_question_box++;
			}
		}
	}//inisialize question box

	{
		ostringstream ball;

		for (int y = 0; y < pyramid_stack_height; ++y)
		{
			for (int x = 0; x <= y; ++x)
			{
				ball << "my_ball" << num_question_box;

				this->AddGameObject(CommonUtils::BuildSphereObject(
					ball.str().c_str(),
					Vector3((x - y * 0.5f) * 0.7f +10.f, 3.0f + ((-1.f) + float(pyramid_stack_height - 1 - y)) * 0.7f, 20.f),
					0.3f,
					true,
					1.f,
					true,
					true,
					Vector4(1.0f, 1.0f, 1.0f, 1.0f)));

				Object* question_box_object = this->FindGameObject(ball.str().c_str());
				question_box_object->Physics()->SetRestState(true);
				question_box_object->Physics()->SetInCourseWork(true);

				num_question_box++;
			}
		}
	}//inisialize rest ball

	{
		this->AddGameObject(CommonUtils::Build_Raptor_Object(
			"raptor",
			Vector3(0.f,  9.0f, 15.f),
			Vector3(1.0f, 1.0f, 1.0f),
			true,
			1.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f)
			));

		Object* raptor = this->FindGameObject("raptor");
		raptor->Physics()->SetRestState(true);
		raptor->Physics()->SetInCourseWork(true);
	}//inisialize raptor 

	{
		this->AddGameObject(CommonUtils::Build_target_CuboidObject(
			"target",
			Vector3(0.f,11.4f,0.f),
			Vector3(1.5f, 1.5f, 0.3f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f)
			));

		Object* target = this->FindGameObject("target");
		target->Physics()->SetRestState(true);
		target->Physics()->SetInCourseWork(true);
		target->Physics()->SetCanScore(true);
	}//inisialize target 

}

void Planet::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	Object* planet = this->FindGameObject("Planet");
	planet->Physics()->SetAngularVelocity(Vector3(0.0f, 0.1f, 0.0f));//let planet keep rooling
	Object* target = this->FindGameObject("target");
	target->Physics()->SetAngularVelocity(Vector3(0.0f, 0.1f, 0.0f));

	Update_Score();

	DrawFlags();

	Ball_From_Camera();
}


//throwing a ball form camera
void Planet::Ball_From_Camera()
{
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_J))
	{
		Matrix4 viewMtx = SceneManager::Instance()->GetCamera()->BuildViewMatrix();
		Vector3 viewDir = - Vector3(viewMtx[2], viewMtx[6], viewMtx[10]);

		ostringstream ballshootName;
		ballshootName << "ball" << num_ball;

		Object* ballshoot = CommonUtils::Build_Ball_SphereObject(
			ballshootName.str().c_str(),
			SceneManager::Instance()->GetCamera()->GetPosition(),
			0.2f,
			true,
			0.1f,
			true,
			false,
			Vector4(1.f, 1.f, 1.f, 1.f));
		this->AddGameObject(ballshoot);

		Object* ball = this->FindGameObject(ballshootName.str().c_str());
		ball->Physics()->SetRestState(false);
		ball->Physics()->SetInCourseWork(true);
		ball->Physics()->SetCanScore(true);
		ball->Physics()->SetLinearVelocity(viewDir * 35.f);

		num_ball++;
	}
}

void Planet::DrawFlags()
{
	//uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_C))
		my_drawFlag ^= DEBUGDRAW_FLAGS_COLLISIONVOLUMES;

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_N))
		my_drawFlag ^= DEBUGDRAW_FLAGS_COLLISIONNORMALS;

	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "     Draw Collision Volumes : %s (Press C to toggle)", (my_drawFlag & DEBUGDRAW_FLAGS_COLLISIONVOLUMES) ? "Enabled" : "Disabled");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "     Draw Collision Normals : %s (Press N to toggle)", (my_drawFlag & DEBUGDRAW_FLAGS_COLLISIONNORMALS) ? "Enabled" : "Disabled");

	PhysicsEngine::Instance()->SetDebugDrawFlags(my_drawFlag);
}

void Planet::Update_Score()
{
	Object* target = this->FindGameObject("target");

	if (target->Physics()->GetDoScore() == true)
	{
		num_Score += target->Physics()->GetScore();
		NCLDebug::Log("You get %d scores", target->Physics()->GetScore());
	}
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "Your All Scores: %d", this->num_Score);
}
