#pragma once
#include <DirectXMath.h>
#include <DirectXCollision.h>

class Camera final
{
public:
	Camera();
	~Camera();

public:
	// Get/Set world camera position.
	DirectX::XMFLOAT3 GetPosition() const;	// 카메라의 포지션을 가져온다.
	DirectX::XMVECTOR GetPositionXM() const;	// 가져온 카메라의 포지션을 float3개짜리 배열로 리턴
	void SetPosition(float x, float y, float z);	// 카메라의 포지션을 정한다.
	void SetPosition(const 	DirectX::XMFLOAT3& v);

	// Get camera basis vectors.
	DirectX::XMVECTOR GetRightXM() const;
	DirectX::XMFLOAT3 GetRight() const;
	DirectX::XMVECTOR GetUpXM() const;
	DirectX::XMFLOAT3 GetUp() const;
	DirectX::XMVECTOR GetLookXM() const;
	DirectX::XMFLOAT3 GetLook() const;

	// 프로스텀 프로퍼티를 가져온다.
	float GetNearZ() const;
	float GetFarZ() const;
	float GetAspect() const;
	float GetFovY() const;
	float GetFovX() const;

	// Set Frustum for Culling
	bool SetFrustum(const DirectX::XMFLOAT4X4 view, const DirectX::XMFLOAT4X4 proj);
	DirectX::BoundingFrustum& GetFrustum();
	DirectX::BoundingFrustum _BBFrustum;

	// 뷰 공간 좌표에서 근거리 및 원거리 평면 치수를 가져옵니다.
	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	// 프로스텀 설치
	void SetLens(float fovY, float aspect, float zn, float zf);

	// LookAt 매개 변수를 통해 카메라 공간을 정의합니다.
	void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	void LookAt(const 	DirectX::XMFLOAT3& pos, const 	DirectX::XMFLOAT3& target, const 	DirectX::XMFLOAT3& up);

	// 뷰/프로젝션 행렬을 가져옵니다.
		DirectX::XMMATRIX View() const;
		DirectX::XMMATRIX Proj() const;
		DirectX::XMMATRIX ViewProj() const;

	// 측면/움직이는 함수
	void Strafe(float d);
	void Walk(float d);
	void WorldUpDown(float d);

	// 카메라 회전 함수
	void RotateX(float angle);
	void RotateY(float angle);

	// 카메라 위치/방향을 수정한 후 뷰 매트릭스를 재구성하기 위해 호출합니다.

	void UpdateViewMatrix();

private:
	// 세계 공간에 상대적인 좌표를 가진 카메라 좌표계.
		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT3 m_Right;
		DirectX::XMFLOAT3 m_Up;
		DirectX::XMFLOAT3 m_Look;

	// 캐시 프로스텀 프로퍼티
		float m_NearZ;
		float m_FarZ;
		float m_Aspect;
		float m_FovY;
		float m_NearWindowHeight;
		float m_FarWindowHeight;

	// Cache View/Proj matrices.
	DirectX::XMFLOAT4X4 m_View;
	DirectX::XMFLOAT4X4 m_Proj;

};

