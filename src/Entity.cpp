#include "raylib.h"

#include <stdio.h>

#include "Entity.h"
#include "EnvironmentalObject.h"
#include "AnimationMgr.h"
#include "AudioMgr.h"
#include "BattyUtils.h"
#include "MathUtils.h"
#include "Constants.h"
#include "GameCamera.h"

Entity::Entity(const char* modelPath, float scale,
	bool spawn, bool isSphere, Vector3 pos)
	:
	m_scale(scale),
	m_spawned(spawn)
{
	if (modelPath != nullptr)
		GeneralEntity::Init(modelPath, isSphere, pos);
	else
	{
		m_isBillboard = true;
		m_sphereCollider = isSphere;
		SetPos(pos);
	}

	if (!m_isBillboard)
	{
		if (AnimationMgr::Instance().m_animations.count(modelPath) > 0)
		{
			m_anims = &AnimationMgr::Instance().m_animations.at(modelPath);
		}
		else
		{
			m_anims = nullptr;
		}
	}

	Init();
}

Entity::Entity(Mesh mesh, float scale,
	bool spawn, bool isSphere, Vector3 pos)
	:
	m_scale(scale),
	m_spawned(spawn)
{
	GeneralEntity::Init(mesh, isSphere, pos);

	Init();
}

void Entity::UpdateEntity(bool doNotMove, bool doNotAnimate)
{
	if (!m_spawned)
		return;

	if (!m_isBillboard)
	{
		if (!doNotAnimate)
			Animate(m_model, m_animFrameCounter);

		m_prevVisualRot = m_visualRot;
	}
	else
	{
		if (!doNotAnimate)
			Animate(m_animFrameCounter);

		if (DidMove())
		{
			Vector3 calcPos = GetPos() - m_cam->GetPosition();
			Vector2 pos = { calcPos.x, calcPos.z };
			Vector3 calcPrevPos = m_prevPos - m_cam->GetPosition();
			Vector2 prevPos = { calcPrevPos.x, calcPrevPos.z };
			float angle = Vector2Angle(prevPos, pos);
			if (fabs(angle) > 0.001f)
			{
				m_facingRight = Vector2Angle(prevPos, pos) > 0;
			}
		}
	}

	for (int i = 0; i < MAX_CUR_NOISES; i++)
	{
		if (m_curNoises[i].id == -1)
			continue;

		if (!IsSoundPlaying(*m_curNoises[i].pSound))
		{
			m_curNoises[i].id = -1;
			m_curNoises[i].pSound = nullptr;
		}
	}
}

void Entity::Draw()
{
	if (!m_spawned)
		return;

	if (!m_isBillboard)
	{
		DrawModel(GetModel(), Vector3Zero(), GetScale(), WHITE);

#ifdef DEBUG
		if (m_sphereCollider)
			DrawSphereWires(GetPos(), m_radius, 10, 10, GREEN);
		else
			DrawBoundingBox(GetBoundingBox(), GREEN);
#endif // DEBUG
	}
	else
	{
		Texture2D toDraw = m_billboardAnims[m_curAnim][m_animFrameCounter];
		Rectangle source = { m_facingRight ? 0.0f : -10.0f, 0.0f, (float)toDraw.width, (float)toDraw.height };
		Vector3 myPos = GetPos();
		myPos.y += ((float)toDraw.height) / 2.0f;
		Vector2 size = { m_scale * fabsf((float)source.width / source.height) * (m_facingRight ? 1.0f : -1.0f), m_scale };
		DrawBillboardRec(m_cam->GetCamera(), toDraw, source, myPos, size, WHITE);
	}
}

void Entity::SpawnEntity()
{
	m_dead = false;
	m_spawned = true;
}

void Entity::ResetEntity()
{
	m_curAnim			= -1;
	m_animFrameCounter	= 0;
	m_animFrameToStopAt	= -1;
	m_animLoop = false;

	m_pos				= Vector3Zero();
	m_prevPos			= Vector3Zero();

	m_velocity			= Vector3Zero();
	if (!m_isBillboard)
	{
		m_rot = QuaternionIdentity();
		m_visualRot = QuaternionIdentity();
		m_prevVisualRot = QuaternionIdentity();
	}

	if (!m_isBillboard)
		SetTransformAndBb(m_scale, m_anims != nullptr);

	m_dead				= false;
	m_spawned			= false;
}

void Entity::SetCamera(GameCamera* cam)
{
	m_cam = cam;
}

void Entity::SetBillboardAnim(const char* animPath, int id, int frames, int speed)
{
	constexpr int BUF_SZ = 256;
	char buf[BUF_SZ];
	memset(buf, 0, sizeof(char) * BUF_SZ);

#ifndef PLATFORM_WEB
	_ASSERT(frames < MAX_BILLBOARD_FRAMES);
#endif // !PLATFORM_WEB

	m_numBillboardFrames[id] = frames;
	m_billboardAnimSpeed[id] = speed;

	for (int i = 1; i <= frames; i++)
	{
		snprintf(buf, sizeof(char) * BUF_SZ, animPath, i);
		Image image = LoadImage(buf);
		Rectangle rec;
		rec.x = 112.0f;
		rec.y = 74.0f;
		rec.width = 170.0f - 112.0f;
		rec.height = 128.0f - 74.0f;
		image = ImageFromImage(image, rec);
		m_billboardAnims[id][i - 1] = LoadTextureFromImage(image);
	}
}

void Entity::SetCurAnim(int animNum)
{
	if (m_curAnim == animNum)
		return;

	m_curAnim = animNum;
	m_animFrameCounter = 0;
}

Vector3 Entity::GetCamPos() const
{
	return GetPos();
}

bool Entity::DidMove() const
{
	float magnitude = Vector3Distance(GetPos(), m_prevPos);
	bool didMove = magnitude > 0.3f;
	return didMove;
}

float Entity::GetScale() const
{
	return m_scale;
}

Vector3 Entity::GetForward() const
{
	return Vector3RotateByQuaternion(
		Vector3{ 0, 0, 1 },
		m_rot);
}

Vector3 Entity::GetLeft() const
{
	return Vector3RotateByQuaternion(
		Vector3{ 1, 0, 0 },
		m_rot);
}

Vector3 Entity::GetUp() const
{
	return Vector3RotateByQuaternion(
		Vector3{ 0, 1, 0 },
		m_rot);
}

Vector3 Entity::TransformPoint(Vector3 point) const
{
	auto mPos = MatrixTranslate(m_pos.x, m_pos.y, m_pos.z);
	auto mRot = QuaternionToMatrix(m_rot);
	auto matrix = MatrixMultiply(mRot, mPos);
	return Vector3Transform(point, matrix);
}

void Entity::RotateLocalEuler(Vector3 axis, float degrees, bool visuallyRot)
{
	auto radians = degrees * DEG2RAD;
	Quaternion toSet = QuaternionMultiply(
		m_rot,
		QuaternionFromAxisAngle(axis, radians));

	if (visuallyRot)
		SetAllRot(toSet);
	else
		m_rot = toSet;
}

void Entity::HitByProjectile()
{
}

bool Entity::isDead() const
{
	return m_dead;
}

bool Entity::isSpawned() const
{
	return m_spawned;
}

void Entity::makeNoise(int id, float pitch, float volume)
{
	for (int i = 0; i < m_MAX_NOISE_CANCEL_SETS; i++)
	{
		for (int j = 0; j < m_MAX_NOISE_CANCEL_SET_SZ; j++)
		{
			if (m_noiseCancelSets[i][j] != -1 && m_noiseCancelSets[i][j] == id)
			{
				for (int k = 0; k < m_MAX_NOISE_CANCEL_SET_SZ; k++)
				{
					for (int l = 0; l < MAX_CUR_NOISES; l++)
					{
						if (m_curNoises[l].id != -1 && m_curNoises[l].id == m_noiseCancelSets[i][k])
						{
							StopSound(*m_curNoises[l].pSound);
							m_curNoises[l].id = -1;
							m_curNoises[l].pSound = nullptr;
						}
					}
				}
			}
		}
	}

	const Sound& playedSound = AudioMgr::Instance().PlayNoise(id, pitch);
	
	bool placed = false;
	for (int i = 0; i < MAX_CUR_NOISES; i++)
	{
		if (m_curNoises[i].id == -1 && !placed)
		{
			m_curNoises[i].id = id;
			m_curNoises[i].pSound = &playedSound;
			placed = true;
		}
	}
#if PLATFORM_DESKTOP
	_ASSERT(placed);
#endif // PLATFORM_DESKTOP
}

void Entity::getCurNoises(int* ids, int& numNoises) const
{
	int myNumNoises = 0;
	for (int i = 0; i < MAX_CUR_NOISES; i++)
	{
		if (m_curNoises[i].id != -1)
		{
			ids[myNumNoises] = m_curNoises[i].id;
			myNumNoises++;
		}
	}
	numNoises = myNumNoises;
}

void Entity::SetUid(int uid)
{
	m_uid = uid;
}

void Entity::Init()
{
	memset(m_billboardAnims, 0, sizeof(Texture2D*) * MAX_BILLBOARD_ANIMS * MAX_BILLBOARD_FRAMES);
	memset(m_numBillboardFrames, 0, sizeof(int) * MAX_BILLBOARD_ANIMS);

	memset(m_noiseCancelSets, -1, sizeof(int) * m_MAX_NOISE_CANCEL_SETS * m_MAX_NOISE_CANCEL_SET_SZ);

	m_velocity = { 0.0f, 0.0f, 0.0f };

	SetTransformAndBb(m_scale, m_anims != nullptr);
}

void Entity::Die()
{
	m_dead = true;
}

void Entity::Animate(Model mdl, int& frame)
{
	if (m_curAnim >= 0 && m_anims != nullptr)
	{
		frame++;

		bool reset = false;

		if (
			frame >= m_anims->m_anims[m_curAnim].frameCount ||
			(m_animFrameToStopAt != -1 && frame >= m_animFrameToStopAt)
			)
		{
			frame = 0;
			if (!m_animLoop)
				reset = true;
		}

		// if it's time to reset and we don't want to reset, leave early
		if (reset && !m_resetToFirstAnimFrame)
		{
			m_curAnim = -1;
 			return;
		}

		UpdateModelAnimationBones(mdl, m_anims->m_anims[m_curAnim], frame);

		if (reset)
		{
			m_curAnim = -1;
		}
	}
}

void Entity::Animate(int& frame)
{
	m_ticksSinceLastFrame++;

	if (m_curAnim >= 0)
	{
		if (m_billboardAnimSpeed[m_curAnim] > m_ticksSinceLastFrame)
			return;
		else
			m_ticksSinceLastFrame = 0;

		frame++;

		bool reset = false;

		if (
			frame >= m_numBillboardFrames[m_curAnim] ||
			(m_animFrameToStopAt != -1 && frame >= m_animFrameToStopAt)
			)
		{
			frame = 0;
			if (!m_animLoop)
				reset = true;
		}

		// if it's time to reset and we don't want to reset, leave early
		if (reset && !m_resetToFirstAnimFrame)
		{
			m_curAnim = -1;
			return;
		}

		if (reset)
		{
			m_curAnim = -1;
		}
	}
}

void Entity::SetPos(Vector3 pos, bool setPrevPos)
{
#if PLATFORM_DESKTOP
	_ASSERT(!isnan(pos.x));
	_ASSERT(!isnan(pos.y));
	_ASSERT(!isnan(pos.z));
	_ASSERT(!isinf(pos.x));
	_ASSERT(!isinf(pos.y));
	_ASSERT(!isinf(pos.z));
#endif // PLATFORM_DESKTOP
	if (setPrevPos)
		m_prevPos = m_pos;
	m_pos = pos;

	SetTransformAndBb(m_scale, m_anims != nullptr);
}

void Entity::SetAllRot(Quaternion rot)
{
	m_visualRot = rot;
	m_rot = rot;

	SetTransformAndBb(m_scale, m_anims != nullptr);
}

RayCollision Entity::ResolveCollision(EnvironmentalObject& envObj)
{
	Ray collisionRay;
	Vector3 closestPoint = ClosestPointBox(GetPos(), envObj.GetBoundingBox());
	collisionRay.direction = Vector3Normalize(closestPoint - GetPos());

	RayCollision rc;

	int i = 1;
	do
	{
		printf("in level collision loop iter %d\n", i);
		collisionRay.position = GetPos();
		rc = envObj.GetRayCollision(collisionRay);

		float overlap;
		if (m_sphereCollider)
			overlap = fabs(m_radius - rc.distance);
		else
			// TODO BAD. I need to reduce this by the BB's dimensions
			overlap = rc.distance;
		Vector3 toMove = rc.normal * overlap;
		SetPos(GetPos() + toMove);
		i++;
		if (i > 100)
			break;
	}
	// at this point I should check that I'm no longer colliding with the obj. Then I could iteratively keep
	// moving until I'm not
	while (CollisionCheck(envObj.GetBoundingBox()));

	return rc;
}
