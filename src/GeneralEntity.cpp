#include "GeneralEntity.h"
#include "BattyUtils.h"
#ifdef PLATFORM_DESKTOP
#include "crtdbg.h"
#endif // !PLATFORM_DESKTOP

void GeneralEntity::Init(const char* modelPath, bool sphereCollider, Vector3 pos)
{
	m_sphereCollider = sphereCollider;
	if (modelPath != nullptr)
		m_model = LoadModel(modelPath);

	m_pos = pos;
	SetTransformAndBb();
}

void GeneralEntity::Init(Mesh mesh, bool sphereCollider, Vector3 pos)
{
	m_sphereCollider = sphereCollider;
	m_model = LoadModelFromMesh(mesh);

	m_pos = pos;
	SetTransformAndBb();
}

GeneralEntity::~GeneralEntity()
{
	if (m_model.materialCount != 0)
		UnloadModel(m_model);
}

void GeneralEntity::Draw()
{
	DrawModel(m_model, Vector3Zero(), 1.0f, {127, 127, 127, 255});

#ifdef DEBUG
	if (m_sphereCollider)
		DrawSphereWires(m_pos, m_radius, 10, 10, GREEN);
	else
		DrawBoundingBox(m_bb, GREEN);
#endif
}

BoundingBox GeneralEntity::GetBoundingBox() const
{
	return m_bb;
}

bool GeneralEntity::CollisionCheck(BoundingBox bb)
{
	if (!m_sphereCollider)
		return CheckCollisionBoxes(m_bb, bb);
	else
		return CheckCollisionBoxSphere(bb, GetPos(), m_radius);
}

bool GeneralEntity::CollisionCheck(Vector3 pos, float radius)
{
	if (!m_sphereCollider)
		return CheckCollisionBoxSphere(m_bb, pos, radius);
	else
		return CheckCollisionSpheres(GetPos(), m_radius, pos, radius);
}

void GeneralEntity::SetAllRot(Quaternion rot)
{
	m_visualRot = rot;
	m_rot = rot;

	SetTransformAndBb();
}

Quaternion GeneralEntity::GetRot() const
{
	return m_rot;
}

Vector3 GeneralEntity::GetPos() const
{
	return m_pos;
}

void GeneralEntity::SetTransformAndBb(float scale, bool hasAnims)
{
	if (m_isBillboard)
		return;

	auto transform = MatrixTranslate(
		GetPos().x / scale,
		GetPos().y / scale,
		GetPos().z / scale
	);
	transform = MatrixMultiply(QuaternionToMatrix(m_visualRot), transform);
	m_model.transform = transform;

	m_bb = BattyGetModelBoundingBox(GetModel(), hasAnims);

	m_bb.min = Vector3Scale(m_bb.min, scale);
	m_bb.max = Vector3Scale(m_bb.max, scale);
}

void GeneralEntity::SetMaterialShader(Shader shader)
{
	for (int i = 0; i < m_model.materialCount; i++)
		m_model.materials[i].shader = shader;
}

Model GeneralEntity::GetModel() const
{
	return m_model;
}
