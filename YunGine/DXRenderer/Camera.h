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
	DirectX::XMFLOAT3 GetPosition() const;	// ī�޶��� �������� �����´�.
	DirectX::XMVECTOR GetPositionXM() const;	// ������ ī�޶��� �������� float3��¥�� �迭�� ����
	void SetPosition(float x, float y, float z);	// ī�޶��� �������� ���Ѵ�.
	void SetPosition(const 	DirectX::XMFLOAT3& v);

	// Get camera basis vectors.
	DirectX::XMVECTOR GetRightXM() const;
	DirectX::XMFLOAT3 GetRight() const;
	DirectX::XMVECTOR GetUpXM() const;
	DirectX::XMFLOAT3 GetUp() const;
	DirectX::XMVECTOR GetLookXM() const;
	DirectX::XMFLOAT3 GetLook() const;

	// ���ν��� ������Ƽ�� �����´�.
	float GetNearZ() const;
	float GetFarZ() const;
	float GetAspect() const;
	float GetFovY() const;
	float GetFovX() const;

	// Set Frustum for Culling
	bool SetFrustum(const DirectX::XMFLOAT4X4 view, const DirectX::XMFLOAT4X4 proj);
	DirectX::BoundingFrustum& GetFrustum();
	DirectX::BoundingFrustum _BBFrustum;

	// �� ���� ��ǥ���� �ٰŸ� �� ���Ÿ� ��� ġ���� �����ɴϴ�.
	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	// ���ν��� ��ġ
	void SetLens(float fovY, float aspect, float zn, float zf);

	// LookAt �Ű� ������ ���� ī�޶� ������ �����մϴ�.
	void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR worldUp);
	void LookAt(const 	DirectX::XMFLOAT3& pos, const 	DirectX::XMFLOAT3& target, const 	DirectX::XMFLOAT3& up);

	// ��/�������� ����� �����ɴϴ�.
		DirectX::XMMATRIX View() const;
		DirectX::XMMATRIX Proj() const;
		DirectX::XMMATRIX ViewProj() const;

	// ����/�����̴� �Լ�
	void Strafe(float d);
	void Walk(float d);
	void WorldUpDown(float d);

	// ī�޶� ȸ�� �Լ�
	void RotateX(float angle);
	void RotateY(float angle);

	// ī�޶� ��ġ/������ ������ �� �� ��Ʈ������ �籸���ϱ� ���� ȣ���մϴ�.

	void UpdateViewMatrix();

private:
	// ���� ������ ������� ��ǥ�� ���� ī�޶� ��ǥ��.
		DirectX::XMFLOAT3 m_Position;
		DirectX::XMFLOAT3 m_Right;
		DirectX::XMFLOAT3 m_Up;
		DirectX::XMFLOAT3 m_Look;

	// ĳ�� ���ν��� ������Ƽ
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

