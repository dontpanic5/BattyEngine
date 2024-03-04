/*******************************************************************************
 * This file taken from: https://github.com/brihernandez/Ergo
 *******************************************************************************/

#include "GameCamera.h"

#include <raymath.h>

#include "Entity.h"
#include "MathUtils.h"

GameCamera::GameCamera(bool isPerspective, float fieldOfView)
{
	Camera = Camera3D();
	Camera.position = Vector3{ 0, 10, -10 };
	Camera.target = Vector3{ 0, 0, 0 };
	Camera.up = Vector3{ 0, 1, 0 };

	Camera.fovy = fieldOfView;
	Camera.projection = isPerspective
		? CameraProjection::CAMERA_PERSPECTIVE
		: CameraProjection::CAMERA_ORTHOGRAPHIC;

	SmoothPosition = Vector3Zero();
	SmoothTarget = Vector3Zero();
	SmoothUp = Vector3Zero();
}

void GameCamera::FollowEntity(const Entity& entity, float deltaTime)
{
	Vector3 position;
	Vector3 target = entity.GetPos();
	Vector3 up = entity.GetUp();

	Vector3 shipForwards; 

	position = entity.TransformPoint({ 0.0f, 4.5f, -12.0f});
	shipForwards = Vector3Scale(entity.GetForward(), 50.0f);

	target = Vector3Add(entity.GetPos(), shipForwards);
	// put entity at the bottom of the view
	target = Vector3Add(target, {0.0f, 10.0f, 0.0f});

	MoveTo(position, target, up, deltaTime);
}

void GameCamera::CinematicWatchEntity(const Entity& entity, float deltaTime, bool immediate)
{
	Vector3 position;
	Vector3 target = entity.GetCamPos();

	auto mPos = MatrixTranslate(target.x, target.y, target.z);
	auto mRot = QuaternionToMatrix(m_cineCam);
	auto matrix = MatrixMultiply(mRot, mPos);
	position = Vector3Transform({ 0, 2.5, 10 }, matrix);


	if (!immediate)
		MoveTo(position, target, entity.GetUp(), deltaTime);
	else
		SetPosition(position, target, entity.GetUp());
}

void GameCamera::MoveTo(Vector3 position, Vector3 target, Vector3 up, float deltaTime)
{
	Camera.position = SmoothDamp(
		Camera.position, position,
		30, deltaTime);

	Camera.target = SmoothDamp(
		Camera.target, target,
		10, deltaTime);

	/*Camera.up = SmoothDamp(
		Camera.up, up,
		5, deltaTime);*/
}

void GameCamera::SetPosition(Vector3 position, Vector3 target, Vector3 up)
{
	Camera.position = position;
	Camera.target = target;
	Camera.up = up;

	SmoothPosition = position;
	SmoothTarget = target;
	SmoothUp = up;
}

void GameCamera::SetFoV(float fov)
{
	Camera.fovy = fov;
}

Vector3 GameCamera::GetPosition() const
{
	return Camera.position;
}

void GameCamera::Begin3DDrawing() const
{
	BeginMode3D(Camera);
}

void GameCamera::EndDrawing() const
{
	EndMode3D();
}
