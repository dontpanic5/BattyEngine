#ifndef GENERAL_ENTITY_H_INCLUDED
#define GENERAL_ENTITY_H_INCLUDED

#include "raylib.h"
#include "raymath.h"

class GeneralEntity
{
public:
	void Init(const char* modelPath, bool sphereCollider = false, Vector3 pos = Vector3Zero());
	void Init(Mesh mesh, bool sphereCollider = false, Vector3 pos = Vector3Zero());
	~GeneralEntity();

	virtual void Draw();

	BoundingBox GetBoundingBox() const;

	virtual bool CollisionCheck(BoundingBox bb);
	virtual bool CollisionCheck(Vector3 pos, float radius);

	Vector3 GetPos() const;
	Quaternion	GetRot() const;

	virtual void SetAllRot(Quaternion rot);

	virtual void SetMaterialShader(Shader shader);

protected:
	Model GetModel() const;

	void SetTransformAndBb(float scale = 1.0f, bool hasAnims = false);

	bool m_isBillboard = false;
	Model m_model;
	BoundingBox m_bb;
	float m_radius = 0.0f;
	bool m_sphereCollider = false;

	Vector3 m_pos = Vector3Zero();

	Quaternion		m_rot = QuaternionIdentity();
	Quaternion		m_visualRot = QuaternionIdentity();
};

#endif