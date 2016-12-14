#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\SceneManager.h>
#include <ncltech\NCLDebug.h>

#include <ncltech\NetworkBase.h>//for network

//#include <ncltech\DistanceConstraint.h>
//#include <ncltech\CommonMeshes.h>
//#include <ncltech\ObjectMesh.h>

#include<sstream>

class Planet : public Scene
{
public:
	Planet(const std::string& friendly_name) : Scene(friendly_name), m_pServerConnection(NULL){}
	virtual ~Planet() {}

	virtual void OnInitializeScene()	 override;
	virtual void OnUpdateScene(float dt) override;

	//-------< Net Work >-------//
	virtual void OnCleanupScene() override;
	void ProcessNetworkEvent(const ENetEvent& evnt);

	//-----< Make Scence more clearlly >------//
	void Ball_From_Camera();
	void DrawFlags();
	void Update_Score();
private:
	uint    my_drawFlag;

	Vector3 ball_position;

	float   ball_collitin_length;
	int     num_ball;
	int     num_question_box;
	int		num_raptor;

	int     num_Score;

	//-------< Net Work >-------//
	NetworkBase m_Network;
	ENetPeer*	m_pServerConnection;
	bool        can_send_information;
	bool		change_colour;

	float		c_Atmosphere;

	int			top01;
	int			top02;
	int			top03;
	int			top04;
	int			top05;
	int			top06;
	int			top07;
	int			top08;
	int			top09;
	int			top10;
	bool		score_chenged;
};