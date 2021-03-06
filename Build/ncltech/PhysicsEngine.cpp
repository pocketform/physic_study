#include "PhysicsEngine.h"
#include "Object.h"
#include "CollisionDetectionSAT.h"
#include "NCLDebug.h"
#include <nclgl\Window.h>
#include <omp.h>


void PhysicsEngine::SetDefaults()
{
	m_DebugDrawFlags = NULL;
	m_IsPaused = false;
	m_UpdateTimestep = 1.0f / 60.f;
	m_UpdateAccum = 0.0f;
	m_Gravity = Vector3(0.0f, -9.81f, 0.0f);
	m_DampingFactor = 0.999f;
}

PhysicsEngine::PhysicsEngine()
{
	SetDefaults();
}

PhysicsEngine::~PhysicsEngine()
{
	RemoveAllPhysicsObjects();
}

void PhysicsEngine::AddPhysicsObject(PhysicsObject* obj)
{
	m_PhysicsObjects.push_back(obj);
}

void PhysicsEngine::RemovePhysicsObject(PhysicsObject* obj)
{
	//Lookup the object in question
	auto found_loc = std::find(m_PhysicsObjects.begin(), m_PhysicsObjects.end(), obj);

	//If found, remove it from the list
	if (found_loc != m_PhysicsObjects.end())
	{
		m_PhysicsObjects.erase(found_loc);
	}
}

void PhysicsEngine::RemoveAllPhysicsObjects()
{
	//Delete and remove all constraints/collision manifolds
	for (Constraint* c : m_vpConstraints)
	{
		delete c;
	}
	m_vpConstraints.clear();

	for (Manifold* m : m_vpManifolds)
	{
		delete m;
	}
	m_vpManifolds.clear();


	//Delete and remove all physics objects
	// - we also need to inform the (possible) associated game-object
	//   that the physics object no longer exists
	for (PhysicsObject* obj : m_PhysicsObjects)
	{
		if (obj->m_pParent != NULL) obj->m_pParent->m_pPhysicsObject = NULL;
		delete obj;
	}
	m_PhysicsObjects.clear();
}


void PhysicsEngine::Update(float deltaTime)
{
	const int max_updates_per_frame = 5;

	if (!m_IsPaused)
	{
		m_UpdateAccum += deltaTime;
		for (int i = 0; (m_UpdateAccum >= m_UpdateTimestep) && i < max_updates_per_frame; ++i)
		{
			m_UpdateAccum -= m_UpdateTimestep;
			if (!m_IsPaused) UpdatePhysics(); //Additional check here incase physics was paused mid-update and the contents of the physics need to be displayed
		}

		if (m_UpdateAccum >= m_UpdateTimestep)
		{
			NCLERROR("Physics too slow to run in real time!");
			//Drop Time in the hope that it can continue to run in real-time
			m_UpdateAccum = 0.0f;
		}
	}


}


void PhysicsEngine::UpdatePhysics()
{
	if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_Z))
		m_atmosphere = !m_atmosphere;

	for (Manifold* m : m_vpManifolds)
	{
		delete m;
	}
	m_vpManifolds.clear();

	//Check for collisions
	BroadPhaseCollisions();
	NarrowPhaseCollisions();

	//Solve collision constraints
	SolveConstraints();

	//Update movement
	for (PhysicsObject* obj : m_PhysicsObjects)
	{
		UpdatePhysicsObject(obj);

		if (Window::GetKeyboard()->KeyTriggered(KEYBOARD_M))
		{
			int temp = obj->GetDebugControl();
			obj->SetDebugControl(temp + 1);
			//c_debug_state++;

			if (obj->GetDebugControl() >= 3)
			{
				obj->SetDebugControl(0);
			}

			if (obj->GetDebugControl() == 1)
			{
				Vector4 colour = obj->GetAssociatedObject()->GetColour();
				obj->GetAssociatedObject()->SetColour(Vector4(0.0f, 0.0f, 0.0f, 0.0f));
			}

			if (obj->GetDebugControl() == 2)
			{
				Vector4 colour = obj->GetAssociatedObject()->GetColour();
				obj->GetAssociatedObject()->SetColour(Vector4(1.0f, 1.0f, 1.0f, 1.0f));
			}
		}
	}
}


void PhysicsEngine::SolveConstraints()
{
	//Optional step to allow constraints to 
	// precompute values based off current velocities 
	// before they are updated in the main loop below.
	for (Manifold* m : m_vpManifolds)		m->PreSolverStep(m_UpdateTimestep);
	for (Constraint* c : m_vpConstraints)	c->PreSolverStep(m_UpdateTimestep);

	// Solve all Constraints and Collision Manifolds
	//for (Manifold* m : m_vpManifolds)		m->ApplyImpulse();
	//for (Constraint* c : m_vpConstraints)	c->ApplyImpulse();
	for (size_t i = 0; i < SOLVER_ITERATIONS; ++i)
	{
		for (Manifold * m : m_vpManifolds)
		{
			m->ApplyImpulse(/*factor*/);
		}

		for (Constraint * c : m_vpConstraints)
		{
			c->ApplyImpulse();
		}
	}
}


void PhysicsEngine::UpdatePhysicsObject(PhysicsObject* obj)
{



	//Vector3 obj_to_earth = Vector3(0.0f, 0.0f, 0.0f) - obj->GetPosition();
	float   distence_to_earth = (Vector3(0.0f, 0.0f, 0.0f) - obj->GetPosition()).Length();

	if (obj->m_CoursWork == true)
	{
		//gravity
		Vector3 g = obj->GetPosition() - Vector3(0, 0, 0)/*the position of planet*/;
		g.Normalise();
		m_Gravity = g * (-9.81f); //set the gracity of planet

		if (m_atmosphere == true)
		{
			if (distence_to_earth > 10 && distence_to_earth < 15)
			{
				m_DampingFactor = 0.8f;
				if (obj->IsBall() == true )
				{
					if (obj->GetColour() != 0.0f)
					{
						float colour = obj->GetColour() - 0.01f;
						obj->setcolour(colour);
						obj->GetAssociatedObject()->SetColour(Vector4(1.0f, obj->GetColour(), obj->GetColour(), 1.0f));
					}
				}
			}
			else 
			{
				m_DampingFactor = 1.0f;
			}
		}
		else
		{
			m_DampingFactor = 1.0f;
		}

		if (obj->m_InvMass > 0.0f)
		{
			obj->m_LinearVelocity += m_Gravity * m_UpdateTimestep;
		}

		if (obj->m_Rest_State == true)
		{
			obj->m_LinearVelocity  = Vector3(0.0f, 0.0f, 0.0f);
			obj->m_AngularVelocity = Vector3(0.0f, 0.0f, 0.0f);
		}

		//semi-implicit (linear displacement)
		////1. velocity in next time step
		//obj->m_LinearVelocity += obj->m_Force * obj->m_InvMass * m_UpdateTimestep;
		//obj->m_LinearVelocity = obj->m_LinearVelocity * m_DampingFactor;
		////2. position in next time step
		//obj->m_Position += obj->m_LinearVelocity * m_UpdateTimestep;
		//
		////semi-implicit (angular displacement)
		////1. velocity
		//obj->m_AngularVelocity += obj->m_InvInertia * obj->m_Torque * m_UpdateTimestep;
		//obj->m_AngularVelocity = obj->m_AngularVelocity * m_DampingFactor;
		////2. position
		//obj->m_Orientation = obj->m_Orientation + obj->m_Orientation * (obj->m_AngularVelocity * m_UpdateTimestep * 0.5f);
		//obj->m_Orientation.Normalise();

		//Midpoint Method
		Vector3 tempVelocity;
		tempVelocity = obj->m_LinearVelocity;
		//1. velocity in next time step
		obj->m_LinearVelocity += obj->m_Force * obj->m_InvMass * m_UpdateTimestep;
		obj->m_LinearVelocity = obj->m_LinearVelocity * m_DampingFactor;
		//2. position in next time step
		//obj->m_Position += obj->m_LinearVelocity * m_UpdateTimestep;
		obj->m_Position += (obj->m_LinearVelocity + tempVelocity) * m_UpdateTimestep *0.5;

		//Midpoint Method (angular displacement)
		tempVelocity = obj->m_AngularVelocity;
		//1. velocity
		obj->m_AngularVelocity += obj->m_InvInertia * obj->m_Torque * m_UpdateTimestep;
		obj->m_AngularVelocity = obj->m_AngularVelocity  * m_DampingFactor;
		//2. position
		obj->m_Orientation = obj->m_Orientation + obj->m_Orientation * ((obj->m_AngularVelocity + tempVelocity) * m_UpdateTimestep * 0.5f);
		//obj->m_Orientation = obj->m_Orientation + obj->m_Orientation * ((obj->m_AngularVelocity + tempVelocity) * m_UpdateTimestep * 0.5f);
		obj->m_Orientation.Normalise();

		obj->m_wsTransformInvalidated = true;


	}
	
	else
	{
		//gravity
		if (obj->m_InvMass > 0.0f)
		{
			obj->m_LinearVelocity += m_Gravity * m_UpdateTimestep;
		}

		if (obj->m_Rest_State == true)
		{
			obj->m_LinearVelocity  = Vector3(0.0f, 0.0f, 0.0f);
			obj->m_AngularVelocity = Vector3(0.0f, 0.0f, 0.0f);
		}

		//semi-implicit (linear displacement)
	////1. velocity in next time step
	//obj->m_LinearVelocity += obj->m_Force * obj->m_InvMass * m_UpdateTimestep;
	//obj->m_LinearVelocity = obj->m_LinearVelocity * m_DampingFactor;
	////2. position in next time step
	//obj->m_Position += obj->m_LinearVelocity * m_UpdateTimestep;
	//
	////semi-implicit (angular displacement)
	////1. velocity
	//obj->m_AngularVelocity += obj->m_InvInertia * obj->m_Torque * m_UpdateTimestep;
	//obj->m_AngularVelocity = obj->m_AngularVelocity * m_DampingFactor;
	////2. position
	//obj->m_Orientation = obj->m_Orientation + obj->m_Orientation * (obj->m_AngularVelocity * m_UpdateTimestep * 0.5f);
	//obj->m_Orientation.Normalise();

		//Midpoint Method
		Vector3 tempVelocity;
		tempVelocity = obj->m_LinearVelocity;
		//1. velocity in next time step
		obj->m_LinearVelocity += obj->m_Force * obj->m_InvMass * m_UpdateTimestep;
		obj->m_LinearVelocity = obj->m_LinearVelocity * m_DampingFactor;
		//2. position in next time step
		//obj->m_Position += obj->m_LinearVelocity * m_UpdateTimestep;
		obj->m_Position += (obj->m_LinearVelocity + tempVelocity) * m_UpdateTimestep *0.5;

		//Midpoint Method (angular displacement)
		tempVelocity = obj->m_AngularVelocity;
		//1. velocity
		obj->m_AngularVelocity += obj->m_InvInertia * obj->m_Torque * m_UpdateTimestep;
		obj->m_AngularVelocity = obj->m_AngularVelocity  * m_DampingFactor;
		//2. position
		obj->m_Orientation = obj->m_Orientation + obj->m_Orientation * ((obj->m_AngularVelocity + tempVelocity) * m_UpdateTimestep * 0.5f);
		//obj->m_Orientation = obj->m_Orientation + obj->m_Orientation * ((obj->m_AngularVelocity + tempVelocity) * m_UpdateTimestep * 0.5f);
		obj->m_Orientation.Normalise();

		obj->m_wsTransformInvalidated = true;
	}

}


void PhysicsEngine::BroadPhaseCollisions()
{
	m_BroadphaseCollisionPairs.clear();

	PhysicsObject *m_pObj1, *m_pObj2;
	//	The broadphase needs to build a list of all potentially colliding objects in the world,
	//	which then get accurately assesed in narrowphase. If this is too coarse then the system slows down with
	//	the complexity of narrowphase collision checking, if this is too fine then collisions may be missed.


	//	Brute force approach.
	//  - For every object A, assume it could collide with every other object.. 
	//    even if they are on the opposite sides of the world.
	if (m_PhysicsObjects.size() > 0)
	{
		for (size_t i = 0; i < m_PhysicsObjects.size() - 1; ++i)
		{
			for (size_t j = i + 1; j < m_PhysicsObjects.size(); ++j)
			{
				m_pObj1 = m_PhysicsObjects[i];
				m_pObj2 = m_PhysicsObjects[j];

				//Check they both atleast have collision shapes

				if (m_pObj1->GetCollisionShape() != NULL
					&& m_pObj2->GetCollisionShape() != NULL)
				{
					CollisionPair cp;
					cp.pObjectA = m_pObj1;
					cp.pObjectB = m_pObj2;
					m_BroadphaseCollisionPairs.push_back(cp);
				}
			}
		}
	}

	//reset the state of the collision and the state of doscore
	for (auto* obj : m_PhysicsObjects)
	{
		obj->m_isCollide = false;
		obj->m_DoScore	 = false;
		obj->m_Score     = 0;
	}
}


void PhysicsEngine::NarrowPhaseCollisions()
{
	if (m_BroadphaseCollisionPairs.size() > 0)
	{
		//Collision data to pass between detection and manifold generation stages.
		CollisionData colData;				

		//Collision Detection Algorithm to use
		CollisionDetectionSAT colDetect;	

		// Iterate over all possible collision pairs and perform accurate collision detection
		for (size_t i = 0; i < m_BroadphaseCollisionPairs.size(); ++i)
		{
			CollisionPair& cp = m_BroadphaseCollisionPairs[i];

			CollisionShape *shapeA = cp.pObjectA->GetCollisionShape();
			CollisionShape *shapeB = cp.pObjectB->GetCollisionShape();

			colDetect.BeginNewPair(
				cp.pObjectA,
				cp.pObjectB,
				cp.pObjectA->GetCollisionShape(),
				cp.pObjectB->GetCollisionShape());

			//--TUTORIAL 4 CODE--
			// Detects if the objects are colliding - Seperating Axis Theorem
			if (colDetect.AreColliding(&colData))
			{
				{
					cp.pObjectA->m_isCollide = true;//get the collision information
					cp.pObjectB->m_isCollide = true;//get the collision information
				}//changed by mo

				//Draw collision data to the window if requested
				// - Have to do this here as colData is only temporary. 
				if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_COLLISIONNORMALS)
				{
					NCLDebug::DrawPointNDT(colData._pointOnPlane, 0.1f, Vector4(0.5f, 0.5f, 1.0f, 1.0f));
					NCLDebug::DrawThickLineNDT(colData._pointOnPlane, colData._pointOnPlane - colData._normal * colData._penetration, 0.05f, Vector4(0.0f, 0.0f, 1.0f, 1.0f));
				}

				//Check to see if any of the objects have collision callbacks that dont want the objects to physically collide
				bool okA = cp.pObjectA->FireOnCollisionEvent(cp.pObjectA, cp.pObjectB);
				bool okB = cp.pObjectB->FireOnCollisionEvent(cp.pObjectB, cp.pObjectA);

				if (okA && okB)
				{
					{
						if ((cp.pObjectA->Get_Rest_State() == false) || (cp.pObjectB->Get_Rest_State() == false))
						{
							cp.pObjectA->m_Rest_State = false;
							cp.pObjectB->m_Rest_State = false;
						}// set tow object out of rest state

						if (((cp.pObjectA->IsBall() == true) && (cp.pObjectB->GetTarget() == true)) ||
							((cp.pObjectB->IsBall() == true) && (cp.pObjectA->GetTarget() == true)))
						{
							cp.pObjectA->m_DoScore = true;
							cp.pObjectB->m_DoScore = true;

							Vector3 position_A = cp.pObjectA->GetPosition();
							Vector3 position_B = cp.pObjectB->GetPosition();

							if (abs(position_A.Length() - position_B.Length()) <= 0.1f )
							{
								cp.pObjectA -> m_Score = 100;
								cp.pObjectB -> m_Score = 100;
							}
							else if (abs(position_A.Length() - position_B.Length()) <= 0.3f)
							{
								cp.pObjectA->m_Score = 50;
								cp.pObjectB->m_Score = 50;
							}
							else if (abs(position_A.Length() - position_B.Length()) <= 0.6f)
							{
								cp.pObjectA->m_Score = 30;
								cp.pObjectB->m_Score = 30;
							}
							else
							{
								cp.pObjectA->m_Score = 10;
								cp.pObjectB->m_Score = 10;
							}
						}

					}// changed by mo

					//-- TUTORIAL 5 CODE --

					// Build full collision manifold that will also handle the collision response between the two objects in the solver stage
					Manifold* manifold = new Manifold();
					manifold->Initiate(cp.pObjectA, cp.pObjectB);

					// Construct contact points that form the perimeter of the collision manifold
					colDetect.GenContactPoints(manifold);

					// Add to list of manifolds that need solving
					m_vpManifolds.push_back(manifold);
				}
			}
		}
	}
}


void PhysicsEngine::DebugRender()
{
	// Draw all collision manifolds
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_MANIFOLD)
	{
		for (Manifold* m : m_vpManifolds)
		{
			m->DebugDraw();
		}
	}

	// Draw all constraints
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_CONSTRAINT)
	{
		for (Constraint* c : m_vpConstraints)
		{
			c->DebugDraw();
		}
	}

	// Draw all associated collision shapes
	if (m_DebugDrawFlags & DEBUGDRAW_FLAGS_COLLISIONVOLUMES)
	{
		for (PhysicsObject* obj : m_PhysicsObjects)
		{
			if (obj->GetCollisionShape() != NULL)
			{
				obj->GetCollisionShape()->DebugDraw(obj);
			}
		}
	}
}