#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

/*****************************************************************
 * Some code taken from:
 * https://github.com/brihernandez/Ergo
 *****************************************************************/

#include "raylib.h"
#include "raymath.h"
#include "GeneralEntity.h"
#include "AnimationMgr.h"
class GameCamera;

constexpr int MAX_CUR_NOISES = 4;

struct NoiseTracker
{
	int id = -1;
	const Sound* pSound = nullptr;
};

class Entity : public GeneralEntity
{
public:
	Entity(const char* modelPath, float scale,
		bool spawn, bool isSphere = false, Vector3 pos = Vector3Zero());
	Entity(Mesh mesh, float scale,
		bool spawn, bool isSphere = false, Vector3 pos = Vector3Zero());
	virtual ~Entity() = default;

	// NOTE: makes the assumption that the child will resolve
	// all the movement and stuff before the parent method
	// is called.
	virtual void UpdateEntity(bool doNotMove = false, bool doNotAnimation = false);
	virtual void Draw() override;

	virtual void SpawnEntity();
	virtual void ResetEntity();

	void SetCamera(GameCamera* cam);

	void		SetBillboardAnim(const char* animPath, int id, int frames, int speed = 1);
	void		SetCurAnim(int animNum);

	virtual Vector3 GetCamPos() const;
	bool		DidMove() const;
	float		GetScale() const;

	// can be overridden for entities that spin
	virtual Vector3 GetForward() const;
	Vector3 GetLeft() const;
	Vector3 GetUp() const;

	// can be overridden for entities that spin
	virtual Vector3 TransformPoint(Vector3 point) const;

	void RotateLocalEuler(Vector3 axis, float degrees, bool visuallyRot = true);

	virtual void HitByProjectile();

	bool isDead() const;
	bool isSpawned() const;

	void makeNoise(int id, float pitch = 1.0f, float volume = 1.0f);
	void getCurNoises(int* ids, int& numNoises) const;


	void SetUid(int uid);

	virtual int GetTypeId() const = 0;

	int				m_uid = -1;

protected:
	void Init();

	virtual void Die();

	void Animate(Model mdl, int& frame);
	void Animate(int& frame);

	void SetPos(Vector3 pos);
	void SetAllRot(Quaternion rot) override;

	RayCollision ResolveCollision(EnvironmentalObject& envObj);

	static constexpr int MAX_BILLBOARD_ANIMS = 8;
	static constexpr int MAX_BILLBOARD_FRAMES = 64;
	Texture2D		m_billboardAnims[MAX_BILLBOARD_ANIMS][MAX_BILLBOARD_FRAMES];
	int				m_numBillboardFrames[MAX_BILLBOARD_ANIMS];
	int				m_billboardAnimSpeed[MAX_BILLBOARD_ANIMS];

	bool			m_facingRight			= true;

	GameCamera*		m_cam;

	int				m_curAnim				= -1;
	int				m_animFrameCounter		= 0;
	int				m_animFrameToStopAt		= -1;
	bool			m_animLoop				= false;
	bool			m_resetToFirstAnimFrame	= false;
	int				m_ticksSinceLastFrame	= 0;
	AnimWrapper*	m_anims					= nullptr;
	float			m_scale = 1.0f;

	Vector3			m_prevPos		= {0.0f, 0.0f, 0.0f};
	Vector3			m_velocity		= Vector3Zero();
	Quaternion		m_prevVisualRot	= QuaternionIdentity();

	bool			m_dead			= false;

	bool			m_spawned		= false;

	NoiseTracker	m_curNoises[MAX_CUR_NOISES];
	constexpr static int m_MAX_NOISE_CANCEL_SETS = 2;
	constexpr static int m_MAX_NOISE_CANCEL_SET_SZ = 8;
	int				m_noiseCancelSets[m_MAX_NOISE_CANCEL_SETS][m_MAX_NOISE_CANCEL_SET_SZ];
};

#endif // !ENTITY_H_INCLUDED