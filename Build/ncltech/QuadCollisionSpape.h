#pragma once

#include "CollisionShape.h"
#include "Hull.h"

class QuadCollisionShape : public CollisionShape
{
public:
	QuadCollisionShape();
	QuadCollisionShape(const Vector3& halfdims);
	virtual ~QuadCollisionShape();

	// Set Quad Dimensions
	void SetHalfWidth(float half_width) { m_QuadHalfDimensions.x = fabs(half_width); }
	void SetHalfHeight(float half_height) { m_QuadHalfDimensions.y = fabs(half_height); }
	void SetHalfDepth(float half_depth) { m_QuadHalfDimensions.z = fabs(half_depth); }

	// Get Quad Dimensions
	const Vector3& GetHalfDims() const { return m_QuadHalfDimensions; }
	float GetHalfWidth()	const { return m_QuadHalfDimensions.x; }
	float GetHalfHeight()	const { return m_QuadHalfDimensions.y; }
	float GetHalfDepth()	const { return m_QuadHalfDimensions.z; }

	// Debug Collision Shape
	virtual void DebugDraw(const PhysicsObject* currentObject) const override;


	// Build Inertia Matrix for rotational mass
	virtual Matrix3 BuildInverseInertia(float invMass) const override;


	// Generic Collision Detection Routines
	//  - Used in CollisionDetectionSAT to identify if two shapes overlap
	virtual void GetCollisionAxes(
		const PhysicsObject* currentObject,
		std::vector<Vector3>* out_axes) const override;

	virtual void GetEdges(
		const PhysicsObject* currentObject,
		std::vector<CollisionEdge>* out_edges) const override;

	virtual void GetMinMaxVertexOnAxis(
		const PhysicsObject* currentObject,
		const Vector3& axis,
		Vector3* out_min,
		Vector3* out_max) const override;

	virtual void GetIncidentReferencePolygon(
		const PhysicsObject* currentObject,
		const Vector3& axis,
		std::list<Vector3>* out_face,
		Vector3* out_normal,
		std::vector<Plane>* out_adjacent_planes) const override;



protected:
	//Constructs the static cube hull 
	static void ConstructCubeHull();

protected:
	Vector3				 m_QuadHalfDimensions;
	static Hull			 m_QuadHull;			//Static cube descriptor, as all cuboid instances will have the same underlying model format
};
