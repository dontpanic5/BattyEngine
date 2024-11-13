#include "raylib.h"

#include <stdio.h>

#include "Entity.h"
#include "AnimationMgr.h"
#include "AudioMgr.h"
#include "BattyUtils.h"
#include "MathUtils.h"
#include "Constants.h"
#include "GameCamera.h"

Entity::Entity(const char* modelPath, float scale,
	bool drawBounds, bool spawn, Vector3 pos)
	:
#ifdef DEBUG
	m_drawBounds(drawBounds),
#else // DEBUG
	m_drawBounds(false),
#endif
	m_scale(scale),
	m_spawned(spawn)
{
	memset(m_billboardAnims, 0, sizeof(Texture2D*) * MAX_BILLBOARD_ANIMS * MAX_BILLBOARD_FRAMES);
	memset(m_numBillboardFrames, 0, sizeof(int) * MAX_BILLBOARD_ANIMS);

	m_velocity = { 0.0f, 0.0f, 0.0f };

	if (modelPath != nullptr)
		m_model = LoadModel(modelPath);
	else
		m_hasModel = false;

	//Texture2D texture = LoadTexture("resources/anim_vamp_bat/bat_tex.jpg");         // Load model texture and set material
	//SetMaterialTexture(&m_model.materials[0], MATERIAL_MAP_DIFFUSE, texture);     // Set model material map texture

	SetPos(pos);

	SetTransformAndBb();

	if (m_hasModel)
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
}

void Entity::UpdateEntity(bool doNotMove, bool doNotAnimate)
{
	if (!m_spawned)
		return;

	// TODO the if statement here needs to account for animations as well
	if (
		m_hasModel &&
		(!Vector3Equals(m_prevPos, GetPos()) ||
		!QuaternionEquals(m_prevVisualRot, m_visualRot))
		)
	{
		SetTransformAndBb();
	}

	if (m_hasModel)
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

void Entity::DrawEntity()
{
	if (!m_spawned)
		return;

	if (m_hasModel)
	{
		DrawModel(GetModel(), Vector3Zero(), GetScale(), WHITE);
		if (GetDrawBounds())
		{
			DrawBoundingBox(GetBoundingBox(), GREEN);
		}
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

	if (m_hasModel)
		SetTransformAndBb();

	m_velocity			= Vector3Zero();
	if (m_hasModel)
	{
		m_rot = QuaternionIdentity();
		m_visualRot = QuaternionIdentity();
		m_prevVisualRot = QuaternionIdentity();
	}

	m_dead				= false;
	m_spawned			= false;
}

void Entity::SetCamera(GameCamera* cam)
{
	m_cam = cam;
}

void Entity::SetBillboardAnim(const char* animPath, int id, int frames)
{
	constexpr int BUF_SZ = 256;
	char buf[BUF_SZ];
	memset(buf, 0, sizeof(char) * BUF_SZ);

	_ASSERT(frames < MAX_BILLBOARD_FRAMES);

	m_numBillboardFrames[id] = frames;

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

Model Entity::GetModel() const
{
	return m_model;
}

Vector3 Entity::GetPos() const
{
	return m_pos;
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

Quaternion Entity::GetRot() const
{
	return m_rot;
}

float Entity::GetScale() const
{
	return m_scale;
}

bool Entity::GetDrawBounds() const
{
	return m_drawBounds;
}

BoundingBox Entity::GetBoundingBox() const
{
	return m_bb;
}

void Entity::SetAllRot(Quaternion rot)
{
	m_visualRot	= rot;
	m_rot		= rot;
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

void Entity::makeNoise(int id, float pitch)
{
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

void Entity::SetMaterialShaders(Shader shader)
{
	for (int i = 0; i < m_model.materialCount; i++)
		m_model.materials[i].shader = shader;
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
	if (m_curAnim >= 0)
	{
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

void Entity::SetPos(Vector3 pos)
{
#if PLATFORM_DESKTOP
	_ASSERT(!isnan(pos.x));
#endif // PLATFORM_DESKTOP
	m_prevPos = m_pos;
	m_pos = pos;
}

void Entity::SetTransformAndBb()
{
	if (!m_hasModel)
		return;

	auto transform = MatrixTranslate(
		GetPos().x / GetScale(),
		GetPos().y / GetScale(),
		GetPos().z / GetScale()
	);
	transform = MatrixMultiply(QuaternionToMatrix(m_visualRot), transform);
	m_model.transform = transform;

	m_bb = BattyGetModelBoundingBox(GetModel());

	m_bb.min = Vector3Scale(m_bb.min, GetScale());
	m_bb.max = Vector3Scale(m_bb.max, GetScale());
}
