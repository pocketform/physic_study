#pragma once

#include <nclgl\Mesh.h>
#include <ncltech\Scene.h>
#include <ncltech\CommonUtils.h>
#include <ncltech\PhysicsEngine.h>
#include <ncltech\SceneManager.h>
#include <ncltech\NCLDebug.h>
//#include <ncltech\DistanceConstraint.h>
//#include <ncltech\CommonMeshes.h>
//#include <ncltech\CommonUtils.h>
//#include <ncltech\ObjectMesh.h>

#include<sstream>

class Planet : public Scene
{
public:
	Planet(const std::string& friendly_name) : Scene(friendly_name) {}
	virtual ~Planet() {}

	virtual void OnInitializeScene()	 override;
	virtual void OnUpdateScene(float dt) override;

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
};