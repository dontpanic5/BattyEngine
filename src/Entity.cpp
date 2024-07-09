#include "raylib.h"
#include "rlgl.h"
#include "config.h"

#include <stdio.h>

#include "Entity.h"
#include "AnimationMgr.h"
#include "AudioMgr.h"
#include "BattyUtils.h"
#include "MathUtils.h"
#include "Constants.h"

Entity::Entity(const char* modelPath, float scale,
	bool drawBounds, bool spawn, bool calcRotBb, Vector3 pos)
	:
#ifdef DEBUG
	m_drawBounds(drawBounds),
#else // DEBUG
	m_drawBounds(false),
#endif
	m_calcRotBb(calcRotBb),
	m_scale(scale),
	m_spawned(spawn)
{
	m_velocity = { 0.0f, 0.0f, 0.0f };

	m_model = LoadModel(modelPath);

	m_boneTransforms.resize(m_model.boneCount);

	//Texture2D texture = LoadTexture("resources/anim_vamp_bat/bat_tex.jpg");         // Load model texture and set material
	//SetMaterialTexture(&m_model.materials[0], MATERIAL_MAP_DIFFUSE, texture);     // Set model material map texture

	SetPos(pos);

	SetTransformAndBb();



	if (AnimationMgr::Instance().m_animations.count(modelPath) > 0)
	{
		m_anims = &AnimationMgr::Instance().m_animations.at(modelPath);
	}
	else
	{
		m_anims = nullptr;
	}
}

void Entity::UpdateEntity(bool doNotMove, bool doNotAnimate)
{
	if (!m_spawned)
		return;

	// TODO the if statement here needs to account for animations as well
	if (
		!Vector3Equals(m_prevPos, GetPos()) ||
		!QuaternionEquals(m_prevVisualRot, m_visualRot)
		)
	{
		SetTransformAndBb();
	}

	if (!doNotAnimate)
		Animate(m_model, m_animFrameCounter);

	m_prevPos		= GetPos();
	m_prevVisualRot	= m_visualRot;

	m_curNoiseTimer -= TICK;
	if (m_curNoiseTimer < 0.0)
	{
		m_curNoiseTimer = -1.0;
		m_curNoise = -1;
	}
}

void Entity::DrawEntity()
{
	if (!m_spawned)
		return;

	DrawModel(GetModel(), Vector3Zero(), GetScale(), WHITE);
	if (GetDrawBounds())
	{
		DrawBoundingBox(GetBoundingBox(), GREEN);
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

	SetTransformAndBb();

	m_velocity			= Vector3Zero();
	m_rot				= QuaternionIdentity();
	m_visualRot			= QuaternionIdentity();
	m_prevVisualRot		= QuaternionIdentity();

	m_dead				= false;
	m_spawned			= false;
}

void Entity::SetCurAnim(int animNum)
{
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

void Entity::RotateLocalEuler(Vector3 axis, float degrees)
{
	auto radians = degrees * DEG2RAD;
	m_rot = QuaternionMultiply(
		m_rot,
		QuaternionFromAxisAngle(axis, radians));
}

bool Entity::isDead() const
{
	return m_dead;
}

bool Entity::isSpawned() const
{
	return m_spawned;
}

void Entity::makeNoise(int id, bool overwrite)
{
	double length = -1.0;
	if (AudioMgr::Instance().PlayNoise(id, length, overwrite))
	{
		m_curNoise = id;
		m_curNoiseTimer = length;
	}
}

int Entity::getCurNoise() const
{
	return m_curNoise;
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

		UpdateModelAnimation(mdl, m_anims->m_anims[m_curAnim], frame);

		if (reset)
		{
			m_curAnim = -1;
		}
	}
}

void Entity::SetPos(Vector3 pos)
{
	m_pos = pos;
}

void Entity::SetTransformAndBb()
{
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

void Entity::SetupModel()
{
	for (int i = 0; i < m_model.meshCount; i++)
	{
		Mesh& mesh = m_model.meshes[i];

		mesh.tangents = (float*)MemAlloc(sizeof(float) * 4 * mesh.vertexCount);
		memcpy(mesh.tangents, mesh.boneWeights, sizeof(float) * 4 * mesh.vertexCount);

		if (mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT] != 0)
		{
			// Update existing vertex buffer
			rlUpdateVertexBuffer(mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT], mesh.tangents, mesh.vertexCount * 4 * sizeof(float), 0);
		}
		else
		{
			// Load a new tangent attributes buffer
			mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT] = rlLoadVertexBuffer(mesh.tangents, mesh.vertexCount * 4 * sizeof(float), false);
		}

		rlEnableVertexArray(mesh.vaoId);
		rlSetVertexAttribute(RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT, 4, RL_FLOAT, 0, 0, 0);
		rlEnableVertexAttribute(RL_DEFAULT_SHADER_ATTRIB_LOCATION_TANGENT);
		rlDisableVertexArray();

		mesh.texcoords2 = (float*)MemAlloc(sizeof(float) * 2 * mesh.vertexCount);
		for (int vert = 0; vert < mesh.vertexCount; vert++)
		{
			float u = mesh.boneIds[vert * 4 + 0] * 1000.0f + mesh.boneIds[vert * 4 + 1];
			float v = mesh.boneIds[vert * 4 + 2] * 1000.0f + mesh.boneIds[vert * 4 + 3];
			mesh.texcoords2[vert * 2 + 0] = u;
			mesh.texcoords2[vert * 2 + 1] = v;
		}

		if (mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2] != 0)
		{
			// Update existing vertex buffer
			rlUpdateVertexBuffer(mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2], mesh.texcoords2, mesh.vertexCount * 2 * sizeof(float), 0);
		}
		else
		{
			// Load a new tangent attributes buffer
			mesh.vboId[RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2] = rlLoadVertexBuffer(mesh.texcoords2, mesh.vertexCount * 2 * sizeof(float), false);
		}

		rlEnableVertexArray(mesh.vaoId);
		rlSetVertexAttribute(RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2, 2, RL_FLOAT, 0, 0, 0);
		rlEnableVertexAttribute(RL_DEFAULT_SHADER_ATTRIB_LOCATION_TEXCOORD2);
		rlDisableVertexArray();
	}
}
