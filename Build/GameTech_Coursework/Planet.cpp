#include "Planet.h"


void Planet::OnInitializeScene()
{
	num_ball           = 0;
	num_question_box   = 0;
	num_raptor         = 0;
	num_Score          = 0;
					   
	top01			   = 0;
	top02			   = 0;
	top03			   = 0;
	top04			   = 0;
	top05			   = 0;
	top06			   = 0;
	top07			   = 0;
	top08			   = 0;
	top09			   = 0;
	top10			   = 0;
					   
	score_chenged      = false;
	change_colour	   = false;

	c_Atmosphere	   = 0.1f;
	c_debug_state	   = 0;

	const int pyramid_stack_height = 7;

	SceneManager::Instance()->GetCamera()->SetPosition(Vector3(-3.f, 0.0f, 40.0f));
	SceneManager::Instance()->GetCamera()->SetPitch(0.f);

	//PhysicsEngine::Instance()->SetGravity(Vector3(0.0f, 0.0f, 0.0f));
	PhysicsEngine::Instance()->SetDampingFactor(1.0f);						//No Damping
	PhysicsEngine::Instance()->SetDebugDrawFlags(DEBUGDRAW_FLAGS_COLLISIONNORMALS | DEBUGDRAW_FLAGS_COLLISIONVOLUMES);

	my_drawFlag = PhysicsEngine::Instance()->GetDebugDrawFlags();

	my_drawFlag ^= DEBUGDRAW_FLAGS_COLLISIONVOLUMES;
	my_drawFlag ^= DEBUGDRAW_FLAGS_COLLISIONNORMALS;

	//Initialize Client Network
	if (m_Network.Initialize(0))
	{
		NCLDebug::Log("Network: Initialized!");

		//Attempt to connect to the server on localhost:1234
		m_pServerConnection = m_Network.ConnectPeer(127, 0, 0, 1, 1234);
		NCLDebug::Log("Network: Attempting to connect to server.");
	}

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

	{
		this->AddGameObject(CommonUtils::Build_atmosphere_SphereObject(
			"Atmosphere",
			Vector3(0.0f, 0.0f, 0.0f),
			16.0f,
			false,
			0.0f,
			true,
			false,
			Vector4(0.6f, 1.0f, 0.8f, 0.1f)));

		//Object* Atmosphere = this->FindGameObject("Atmosphere");
		//Atmosphere->Physics()->SetRestState(false);
	}//initialze the Atmosphere

	{

		this->AddGameObject(CommonUtils::Build_Quad_Object(
			"HorizontalGround_down",
			Vector3(0.0f, -34.0f, 0.0f),
			Vector3(10.0f, 10.0f, 1.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f)));

		Object* HorizontalGround_down = this->FindGameObject("HorizontalGround_down");
		HorizontalGround_down->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(90.0f,0.0f,0.0f));


		this->AddGameObject(CommonUtils::Build_Quad_Object(
			"HorizontalGround_up",
			Vector3(0.0f, -24.0f, 0.0f),
			Vector3(10.0f, 10.0f, 1.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f)));

		Object* HorizontalGround_up = this->FindGameObject("HorizontalGround_up");
		HorizontalGround_up->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(-90.0f, 0.0f, 0.0f));

		//HorizontalGround->SetLocalTransform(Matrix4::Rotation(45.0f, Vector3(0.0f, 1.0f, 0.0f)) * HorizontalGround->GetLocalTransform());

		this->AddGameObject(CommonUtils::Build_Quad_Object(
			"VerticalGround_fron",
			Vector3( 0.0f, -29.0f, -5.0f),
			Vector3(10.0f,  10.0f,  1.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f)));

		Object* VerticalGround_fron = this->FindGameObject("VerticalGround_fron");
		VerticalGround_fron->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(0.0f, 180.0f, 0.0f));


		this->AddGameObject(CommonUtils::Build_Quad_Object(
			"VerticalGround_right",
			Vector3( 5.0f, -29.0f, 0.0f),
			Vector3(10.0f,  10.0f, 1.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f)));

		Object* VerticalGround_right = this->FindGameObject("VerticalGround_right");
		VerticalGround_right->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(0.0f, 90.0f, 0.0f));

		this->AddGameObject(CommonUtils::Build_Quad_Object(
			"VerticalGround_left",
			Vector3( -5.0f, -29.0f, 0.0f),
			Vector3( 10.0f,  10.0f, 1.0f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f)));

		Object* VerticalGround_left = this->FindGameObject("VerticalGround_left");
		VerticalGround_left->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(0.0f, -90.0f, 0.0f));
		
		this->AddGameObject(CommonUtils::Build_Quad_Object(
			"VerticalGround_back",
			Vector3(0.0f, -29.0f, 5.0f),
			Vector3(10.0f, 10.0f, 0.1f),
			true,
			0.0f,
			true,
			false,
			Vector4(1.0f, 1.0f, 1.0f, 1.0f)));

		Object* VerticalGround_back = this->FindGameObject("VerticalGround_back");
		VerticalGround_back->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(0.0f, 0.0f, 0.0f));
	} //axis - aligned bounding box.

	this->AddGameObject(CommonUtils::Build_Quad_Object(
		"plane",
		Vector3(20.0f, -19.0f, -5.0f),
		Vector3(10.0f,  10.0f,  1.0f),
		true,
		0.0f,
		true,
		false,
		Vector4(1.0f, 1.0f, 1.0f, 1.0f)));

	this->AddGameObject(CommonUtils::BuildSphereObject(
		"this_ball",
		Vector3(-20.0f, -19.0f, -5.0f),
		5.0f,
		true,
		0.0f,
		true,
		false,
		Vector4(1.0f, 1.0f, 1.0f, 1.0f)));

	Object* plane = this->FindGameObject("plane");
	plane->Physics()->SetOrientation(Quaternion::EulerAnglesToQuaternion(0.0f, 180.0f, 0.0f));

	{
		ostringstream question_box;

		for (int y = 0; y < pyramid_stack_height; ++y)
		{
			for (int x = 0; x <= y; ++x)
			{
				question_box << "question_box" << num_question_box;

				this->AddGameObject(CommonUtils::Build_Box_CuboidObject(
					question_box.str().c_str(),
					Vector3((x - y * 0.5f) - 5.0f, 3.0f + ((-1.f) + float(pyramid_stack_height - 1 - y)), 20.f),
					Vector3(0.5f, 0.5f, 0.5f),
					true,
					0.1f,
					true,
					false,
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
					false,
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
			Vector3(-5.0f,  9.0f, 20.0f),
			Vector3( 1.0f, 1.0f,  1.0f),
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
		target->Physics()->SetIsTarget(true);
		target->Physics()->SetRestState(true);
		target->Physics()->SetInCourseWork(true);
		//target->Physics()->SetIsScore(true);
	}//inisialize target 

}

void Planet::OnUpdateScene(float dt)
{
	Scene::OnUpdateScene(dt);

	uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	Object* planet = this->FindGameObject("Planet");
	planet->Physics()->SetAngularVelocity(Vector3(0.0f, 0.1f, 0.0f));//let planet keep rooling
	Object* target = this->FindGameObject("target");
	target->Physics()->SetAngularVelocity(Vector3(0.0f, 0.1f, 0.0f));//let target keep rooling

	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "Push 'J' to throw a ball");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "Push 'Z' to change the state of Atmosphere");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "Push 'M' to control the state of debug mod");
	NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "Push 'N' to draw the colliction point");
	//NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "%d", c_debug_state);
	//NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "%d", 
	//	target->Physics()->GetDebugControl());

	Control_Atmosphere();

	Update_Score();

	Control_Debug();

	Ball_From_Camera();

	Update_Network(dt);
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
			0.5f,
			true,
			0.1f,
			true,
			false,
			Vector4(1.f, 1.f, 1.f, 1.f));
		this->AddGameObject(ballshoot);

		Object* ball = this->FindGameObject(ballshootName.str().c_str());
		ball->Physics()->SetRestState(false);
		ball->Physics()->SetInCourseWork(true);
		ball->Physics()->SetIsScore(true);
		ball->Physics()->SetLinearVelocity(viewDir * 35.f);

		num_ball++;
	}
}

void Planet::Control_Debug()
{
	//uint drawFlags = PhysicsEngine::Instance()->GetDebugDrawFlags();

	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_M))
	{
		c_debug_state++;

		if (c_debug_state >= 3)
		{
			c_debug_state = 0;
		}

		if (c_debug_state == 1)
		{
			my_drawFlag ^= DEBUGDRAW_FLAGS_COLLISIONVOLUMES;
		}

		if (c_debug_state == 0)
		{
			my_drawFlag ^= DEBUGDRAW_FLAGS_COLLISIONVOLUMES;
		}
	}



	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_N))
		my_drawFlag ^= DEBUGDRAW_FLAGS_COLLISIONNORMALS;

	//NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "     Draw Collision Volumes : %s (Press C to toggle)", (my_drawFlag & DEBUGDRAW_FLAGS_COLLISIONVOLUMES) ? "Enabled" : "Disabled");
	//NCLDebug::AddStatusEntry(Vector4(1.0f, 0.9f, 0.8f, 1.0f), "     Draw Collision Normals : %s (Press N to toggle)", (my_drawFlag & DEBUGDRAW_FLAGS_COLLISIONNORMALS) ? "Enabled" : "Disabled");

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

void Planet::Control_Atmosphere()
{
	Object* Atmosphere = this->FindGameObject("Atmosphere");
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Z))
	{
		change_colour = !change_colour;
	}

	if (change_colour == true || c_debug_state == 1)
	{
		if (c_Atmosphere >= 0.0f)
		{
			c_Atmosphere -= 0.01f;
		}
		Atmosphere->SetColour(Vector4(0.6f, 1.0f, 0.8f, c_Atmosphere));
	}
	else
	{
		if (c_debug_state == 2 || change_colour == false)
		{
			if (c_Atmosphere <= 0.1f)
			{
				c_Atmosphere += 0.01f;
			}
		}
		Atmosphere->SetColour(Vector4(0.6f, 1.0f, 0.8f, c_Atmosphere));
	}
}

//for Network
void Planet::OnCleanupScene()
{
	Scene::OnCleanupScene();
	//m_pObj = NULL; // Deleted in above function

				   //Send one final packet telling the server we are disconnecting
				   // - We are not waiting to resend this, so if it fails to arrive
				   //   the server will have to wait until we time out naturally
	enet_peer_disconnect_now(m_pServerConnection, 0);

	//Release network and all associated data/peer connections
	m_Network.Release();
	m_pServerConnection = NULL;
}

//for Network
void Planet::ProcessNetworkEvent(const ENetEvent& evnt)
{
	Object* target = this->FindGameObject("target");

	//if (this->can_send_information == true)
	//{
	//	char* text_data = "you get a score!";
	//	ENetPacket* packet = enet_packet_create(text_data, strlen(text_data) + 1, ENET_PACKET_FLAG_RELIABLE);
	//	enet_peer_send(m_pServerConnection, 0, packet);
	//}

	//if (this->score_chenged == true)
	//{
	//	ostringstream text_top01;
	//	text_top01 << "Top 01 is " << target->Physics()->GetScore();
	//	ENetPacket* packet = enet_packet_create(text_top01.str().c_str(), strlen(text_top01.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
	//	enet_peer_send(m_pServerConnection, 0, packet);


	//	//ostringstream text_top02;
	//	//text_top02 << "Top 02 is " << target->Physics()->GetScore();
	//	//ENetPacket* packet02 = enet_packet_create(text_top02.str().c_str(), strlen(text_top02.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
	//	//enet_peer_send(m_pServerConnection, 0, packet02);

	//}

	switch (evnt.type)
	{
		//New connection request or an existing peer accepted our connection request
		case ENET_EVENT_TYPE_CONNECT:
		{
			if (evnt.peer == m_pServerConnection)
			{
				NCLDebug::Log("Network: Successfully connected to server!");

				//Send a 'hello' packet
				char* text_data = "I'm Mr CourseWork";
				ENetPacket* packet = enet_packet_create(text_data, strlen(text_data) + 1, 0);
				enet_peer_send(m_pServerConnection, 0, packet);
			}
		}
		break;


		//Server has sent us a new packet
		case ENET_EVENT_TYPE_RECEIVE:
		{
			if (evnt.packet->dataLength == sizeof(Vector3))
			{
				//Vector3 pos;
				//memcpy(&pos, evnt.packet->data, sizeof(Vector3));
				//m_pObj->Physics()->SetPosition(pos);
			}
			else
			{
				NCLERROR("Recieved Invalid Network Packet!");
			}

		}
		break;

		//Server has disconnected
		case ENET_EVENT_TYPE_DISCONNECT:
		{
			NCLDebug::Log("Network: Server has disconnected!");
		}
		break;
	}
}

//for Network
void Planet::Update_Network(float dt)
{
	Object* planet = this->FindGameObject("Planet");
	Object* target = this->FindGameObject("target");

	if (target->Physics()->GetDoScore() == true)
	{
		can_send_information = true;

		if (target->Physics()->GetScore() > top01)
		{
			top01 = target->Physics()->GetScore();
			score_chenged = true;
		}
		else if (target->Physics()->GetScore() <= top01)
		{
			if (target->Physics()->GetScore() > top02)
			{
				top02 = target->Physics()->GetScore();
				score_chenged = true;
			}
			else if (target->Physics()->GetScore() <= top02)
			{
				if (target->Physics()->GetScore() > top03)
				{
					top03 = target->Physics()->GetScore();
					score_chenged = true;
				}
				else if (target->Physics()->GetScore() <= top03)
				{
					if (target->Physics()->GetScore() > top04)
					{
						top04 = target->Physics()->GetScore();
						score_chenged = true;
					}
					else if (target->Physics()->GetScore() <= top04)
					{
						if (target->Physics()->GetScore() > top05)
						{
							top05 = target->Physics()->GetScore();
							score_chenged = true;
						}
						else if (target->Physics()->GetScore() <= top05)
						{
							if (target->Physics()->GetScore() > top06)
							{
								top06 = target->Physics()->GetScore();
								score_chenged = true;
							}
							else if (target->Physics()->GetScore() <= top06)
							{
								if (target->Physics()->GetScore() > top07)
								{
									top07 = target->Physics()->GetScore();
									score_chenged = true;
								}
								else if (target->Physics()->GetScore() <= top07)
								{
									if (target->Physics()->GetScore() > top08)
									{
										top08 = target->Physics()->GetScore();
										score_chenged = true;
									}
									else if (target->Physics()->GetScore() <= top08)
									{
										if (target->Physics()->GetScore() > top09)
										{
											top09 = target->Physics()->GetScore();
											score_chenged = true;
										}
										else if (target->Physics()->GetScore() <= top09)
										{
											if (target->Physics()->GetScore() > top10)
											{
												top10 = target->Physics()->GetScore();
												score_chenged = true;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		can_send_information = false;
		score_chenged = false;
	}

	//Update Network
	auto callback = std::bind(
		&Planet::ProcessNetworkEvent,	      // Function to call
		this,								 // Associated class instance
		std::placeholders::_1);				// Where to place the first parameter
	m_Network.ServiceNetwork(dt, callback);

	if (this->can_send_information == true)
	{
		char* text_data = "you get a score!";
		ENetPacket* packet = enet_packet_create(text_data, strlen(text_data) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet);
	}

	if (this->score_chenged == true)
	{
		ostringstream text_top01;
		text_top01 << "Top 01 is " << top01;
		ENetPacket* packet01 = enet_packet_create(text_top01.str().c_str(), strlen(text_top01.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet01);


		ostringstream text_top02;
		text_top02 << "Top 02 is " << top02;
		ENetPacket* packet02 = enet_packet_create(text_top02.str().c_str(), strlen(text_top02.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet02);

		ostringstream text_top03;
		text_top03 << "Top 03 is " << top03;
		ENetPacket* packet03 = enet_packet_create(text_top03.str().c_str(), strlen(text_top03.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet03);

		ostringstream text_top04;
		text_top04 << "Top 04 is " << top04;
		ENetPacket* packet04 = enet_packet_create(text_top04.str().c_str(), strlen(text_top04.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet04);

		ostringstream text_top05;
		text_top05 << "Top 05 is " << top05;
		ENetPacket* packet05 = enet_packet_create(text_top05.str().c_str(), strlen(text_top05.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet05);

		ostringstream text_top06;
		text_top06 << "Top 06 is " << top06;
		ENetPacket* packet06 = enet_packet_create(text_top06.str().c_str(), strlen(text_top06.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet06);

		ostringstream text_top07;
		text_top07 << "Top 07 is " << top07;
		ENetPacket* packet07 = enet_packet_create(text_top07.str().c_str(), strlen(text_top07.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet07);

		ostringstream text_top08;
		text_top08 << "Top 08 is " << top08;
		ENetPacket* packet08 = enet_packet_create(text_top08.str().c_str(), strlen(text_top08.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet08);

		ostringstream text_top09;
		text_top09 << "Top 09 is " << top09;
		ENetPacket* packet09 = enet_packet_create(text_top09.str().c_str(), strlen(text_top09.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet09);

		ostringstream text_top10;
		text_top10 << "Top 10 is " << top10;
		ENetPacket* packet10 = enet_packet_create(text_top10.str().c_str(), strlen(text_top10.str().c_str()) + 1, ENET_PACKET_FLAG_RELIABLE);
		enet_peer_send(m_pServerConnection, 0, packet10);
	}

	//Add Debug Information to screen
	uint8_t ip1 = m_pServerConnection->address.host & 0xFF;
	uint8_t ip2 = (m_pServerConnection->address.host >> 8) & 0xFF;
	uint8_t ip3 = (m_pServerConnection->address.host >> 16) & 0xFF;
	uint8_t ip4 = (m_pServerConnection->address.host >> 24) & 0xFF;

	NCLDebug::DrawTextWs(planet->Physics()->GetPosition() + Vector3(0.f, 0.5f, 0.f), 14.f, TEXTALIGN_CENTRE, Vector4(),
		"Peer: %u.%u.%u.%u:%u", ip1, ip2, ip3, ip4, m_pServerConnection->address.port);

	Vector4 status_color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	NCLDebug::AddStatusEntry(status_color, "Network Traffic");
	NCLDebug::AddStatusEntry(status_color, "    Incoming: %5.2fKbps", m_Network.m_IncomingKb);
	NCLDebug::AddStatusEntry(status_color, "    Outgoing: %5.2fKbps", m_Network.m_OutgoingKb);
}
