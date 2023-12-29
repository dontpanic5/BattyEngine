#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

/*****************************************************************
 * Some code taken from:
 * https://github.com/brihernandez/Ergo
 *****************************************************************/

#include "raylib.h"
#include "raymath.h"

#include "AnimationMgr.h"

class Entity
{
public:
	Entity(const char* modelPath, float scale,
		bool drawBounds, bool spawn, bool calcRotBb = false, Vector3 pos = Vector3Zero());

	// NOTE: makes the assumption that the child will resolve
	// all the movement and stuff before the parent method
	// is called.
	virtual void UpdateEntity(bool doNotMove = false, bool doNotAnimation = false);
	virtual void DrawEntity();

	virtual void SpawnEntity();
	virtual void ResetEntity();

	void		SetCurAnim(int animNum);

	Vector3		GetPos() const;
	virtual Vector3 GetCamPos() const;
	Quaternion	GetRot() const;
	float		GetScale() const;
	bool		GetDrawBounds() const;
	BoundingBox	GetBoundingBox() const;

	// can be overridden for entities that spin
	virtual Vector3 GetForward() const;
	Vector3 GetLeft() const;
	Vector3 GetUp() const;

	// can be overridden for entities that spin
	virtual Vector3 TransformPoint(Vector3 point) const;

	void RotateLocalEuler(Vector3 axis, float degrees);

	virtual bool collisionCheck(BoundingBox bb) = 0;

	virtual ~Entity() = default;

	bool isDead() const;
	bool isSpawned() const;

	void SetUid(int uid);

	virtual void UnloadEntity();

protected:
	virtual void Die();

	void Animate(Model mdl, int& frame);

	Model GetModel() const;

	void SetPos(Vector3 pos);

	void SetTransformAndBb();

	Model			m_model;
	bool			m_drawBounds;
	bool			m_calcRotBb				= false;
	int				m_curAnim				= -1;
	int				m_animFrameCounter		= 0;
	int				m_animFrameToStopAt		= -1;
	bool			m_animLoop				= false;
	bool			m_resetToFirstAnimFrame	= false;
	AnimWrapper*	m_anims;
	float			m_scale = 1.0f;

	BoundingBox		m_bb;

	Vector3			m_pos			= {0.0f, 0.0f, 0.0f};
	Vector3			m_prevPos		= {0.0f, 0.0f, 0.0f};
	Vector3			m_velocity		= Vector3Zero();
	Quaternion		m_rot			= QuaternionIdentity();
	Quaternion		m_visualRot		= QuaternionIdentity();
	Quaternion		m_prevVisualRot	= QuaternionIdentity();

	bool			m_dead			= false;

	bool			m_spawned		= false;

	int				m_uid				= -1;
};

#endif // !ENTITY_H_INCLUDED