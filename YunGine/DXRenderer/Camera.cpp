#include <windows.h>
#include "Camera.h"

Camera::Camera()
	: m_Position(0.0f, 0.0f, 0.0f),	// 카메라의 위치
	m_Right(1.0f, 0.0f, 0.0f),		// 오른쪽으로 돌기 위한 변수
	m_Up(0.0f, 1.0f, 0.0f),			// 왼쪽으로 돌기 위한 변수
	m_Look(0.0f, 0.0f, 1.0f)		// 카메라가 보는 방향을 위한 변수
{

}

Camera::~Camera()
{

}

DirectX::XMFLOAT3 Camera::GetPosition() const
{
	return m_Position;
}

DirectX::XMVECTOR Camera::GetPositionXM() const
{
	return DirectX::XMLoadFloat3(&m_Position);
}

void Camera::SetPosition(float x, float y, float z)
{
	m_Position = DirectX::XMFLOAT3(x, y, z);
}

void Camera::SetPosition(const DirectX::XMFLOAT3& v)
{
	m_Position = v;
}

DirectX::XMVECTOR Camera::GetRightXM() const
{
	return DirectX::XMLoadFloat3(&m_Right);
}

DirectX::XMFLOAT3 Camera::GetRight() const
{
	return m_Right;
}

DirectX::XMVECTOR Camera::GetUpXM() const
{
	return DirectX::XMLoadFloat3(&m_Up);
}

DirectX::XMFLOAT3 Camera::GetUp() const
{
	return m_Up;
}

DirectX::XMVECTOR Camera::GetLookXM() const
{
	return DirectX::XMLoadFloat3(&m_Look);
}

DirectX::XMFLOAT3 Camera::GetLook() const
{
	return m_Look;
}

float Camera::GetNearZ() const
{
	return m_NearZ;
}

float Camera::GetFarZ() const
{
	return m_FarZ;
}

float Camera::GetAspect() const
{
	return m_Aspect;
}

float Camera::GetFovY() const
{
	return m_FovY;
}

float Camera::GetFovX() const
{
	#pragma warning(disable: 4244)
	float halfWidth = 0.5f * GetNearWindowWidth();
	return 2.0f * atan(halfWidth / m_NearZ);
}

bool Camera::SetFrustum(const DirectX::XMFLOAT4X4 view, const DirectX::XMFLOAT4X4 proj)
{
	DirectX::XMMATRIX viewMatrix = DirectX::XMLoadFloat4x4(&view);
	DirectX::XMMATRIX projMatrix = DirectX::XMLoadFloat4x4(&proj);
	DirectX::XMMATRIX tempMatrix = DirectX::XMMatrixMultiply(viewMatrix, projMatrix);

	DirectX::BoundingFrustum::CreateFromMatrix(_BBFrustum, tempMatrix);

	return true;
}

bool Camera::SetFrustum(const DirectX::XMMATRIX& view, const DirectX::XMMATRIX& proj)
{
	DirectX::XMMATRIX tempMatrix = DirectX::XMMatrixMultiply(view, proj);

	DirectX::BoundingFrustum::CreateFromMatrix(_BBFrustum, tempMatrix);

	return true;
}

DirectX::BoundingFrustum& Camera::GetFrustum()
{
	return _BBFrustum;
}

float Camera::GetNearWindowWidth() const
{
	return m_Aspect * m_NearWindowHeight;
}

float Camera::GetNearWindowHeight() const
{
	return m_NearWindowHeight;
}

float Camera::GetFarWindowWidth() const
{
	return m_Aspect * m_FarWindowHeight;
}

float Camera::GetFarWindowHeight() const
{
	return m_FarWindowHeight;
}

void Camera::SetLens(float fovY, float aspect, float zn, float zf)
{
	// cache properties
	m_FovY = fovY;
	m_Aspect = aspect;
	m_NearZ = zn;
	m_FarZ = zf;

	m_NearWindowHeight = 2.0f * m_NearZ * tanf(0.5f * m_FovY);
	m_FarWindowHeight = 2.0f * m_FarZ * tanf(0.5f * m_FovY);

	DirectX::XMMATRIX P = DirectX::XMMatrixPerspectiveFovLH(m_FovY, m_Aspect, m_NearZ, m_FarZ);
	XMStoreFloat4x4(&m_Proj, P);
}

void Camera::LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp)
{
	// 시선벡터와 월드업을 외적하면 right벡터가 나오고
	// 다시 그 right벡터와 시선벡터를 외적하면 내 up벡터가 나옴
	DirectX::XMVECTOR L = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(target, pos));
	DirectX::XMVECTOR R = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(worldUp, L));
	DirectX::XMVECTOR U = DirectX::XMVector3Cross(L, R);

	DirectX::XMStoreFloat3(&m_Position, pos);
	DirectX::XMStoreFloat3(&m_Look, L);
	DirectX::XMStoreFloat3(&m_Right, R);
	DirectX::XMStoreFloat3(&m_Up, U);
}

void Camera::LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target, const DirectX::XMFLOAT3& up)
{
	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&pos);
	DirectX::XMVECTOR T = DirectX::XMLoadFloat3(&target);
	DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&up);

	LookAt(P, T, U);
}

DirectX::XMMATRIX Camera::View() const
{
	return DirectX::XMLoadFloat4x4(&m_View);
}

DirectX::XMMATRIX Camera::Proj() const
{
	return DirectX::XMLoadFloat4x4(&m_Proj);
}

DirectX::XMMATRIX Camera::ViewProj() const
{
	return DirectX::XMLoadFloat4x4(&m_Proj);
}

void Camera::Strafe(float d)
{
	// mPosition += d*mRight
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
	DirectX::XMVECTOR r = DirectX::XMLoadFloat3(&m_Right);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_Position);
	DirectX::XMStoreFloat3(&m_Position, DirectX::XMVectorMultiplyAdd(s, r, p));
}

void Camera::Walk(float d)
{
	// mPosition += d*mLook
	DirectX::XMVECTOR s = DirectX::XMVectorReplicate(d);
	DirectX::XMVECTOR l = DirectX::XMLoadFloat3(&m_Look);
	DirectX::XMVECTOR p = DirectX::XMLoadFloat3(&m_Position);
	DirectX::XMStoreFloat3(&m_Position, DirectX::XMVectorMultiplyAdd(s, l, p));
}

void Camera::WorldUpDown(float d)
{
	// mPosition += d * mUp
	DirectX::XMVECTOR scale = DirectX::XMVectorReplicate(d);
	DirectX::XMVECTOR up = DirectX::XMLoadFloat3(&m_Up);
	DirectX::XMVECTOR position = DirectX::XMLoadFloat3(&m_Position);
	DirectX::XMStoreFloat3(&m_Position, DirectX::XMVectorMultiplyAdd(scale, up, position));
}

// X축 회전
void Camera::RotateX(float angle)
{
	// Rotate up and look vector about the right vector.

	DirectX::XMMATRIX R = DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&m_Right), angle);

	DirectX::XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	DirectX::XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));
}

void Camera::RotateY(float angle)
{
	// Rotate the basis vectors about the world y-axis.

	DirectX::XMMATRIX R = DirectX::XMMatrixRotationY(angle);

	DirectX::XMStoreFloat3(&m_Right, XMVector3TransformNormal(XMLoadFloat3(&m_Right), R));
	DirectX::XMStoreFloat3(&m_Up, XMVector3TransformNormal(XMLoadFloat3(&m_Up), R));
	DirectX::XMStoreFloat3(&m_Look, XMVector3TransformNormal(XMLoadFloat3(&m_Look), R));
}

void Camera::UpdateViewMatrix()
{
	DirectX::XMVECTOR U = DirectX::XMLoadFloat3(&m_Up);
	DirectX::XMVECTOR L = DirectX::XMLoadFloat3(&m_Look);
	DirectX::XMVECTOR P = DirectX::XMLoadFloat3(&m_Position);
	DirectX::XMVECTOR R = DirectX::XMLoadFloat3(&m_Right);

	// Keep camera's axes orthogonal to each other and of unit length.
	L = DirectX::XMVector3Normalize(L);
	U = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(L, R));

	// U, L already ortho-normal, so no need to normalize cross product.
	R = DirectX::XMVector3Cross(U, L);

	// Fill in the view matrix entries.
	float x = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, R));
	float y = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, U));
	float z = -DirectX::XMVectorGetX(DirectX::XMVector3Dot(P, L));

	DirectX::XMStoreFloat3(&m_Right, R);
	DirectX::XMStoreFloat3(&m_Up, U);
	DirectX::XMStoreFloat3(&m_Look, L);

	m_View(0, 0) = m_Right.x;
	m_View(1, 0) = m_Right.y;
	m_View(2, 0) = m_Right.z;
	m_View(3, 0) = x;
	 
	m_View(0, 1) = m_Up.x;
	m_View(1, 1) = m_Up.y;
	m_View(2, 1) = m_Up.z;
	m_View(3, 1) = y;
	 
	m_View(0, 2) = m_Look.x;
	m_View(1, 2) = m_Look.y;
	m_View(2, 2) = m_Look.z;
	m_View(3, 2) = z;
	 
	m_View(0, 3) = 0.0f;
	m_View(1, 3) = 0.0f;
	m_View(2, 3) = 0.0f;
	m_View(3, 3) = 1.0f;
}
