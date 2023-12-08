#pragma once
/// <summary>
/// ī�޶� ������ �ʿ��� ����?-> ������������ �ʿ��� ���� �ְ� ���� �ٸ����� �����Ҽ��� �ֱ⶧����
/// ������ �������� ����Ͻ��� �ʴ� �� ����.
/// </summary>

class ICamera abstract
{
public:
	// Strafe/Walk the camera a distance d.
	virtual void Strafe(float d) = 0;
	virtual void Walk(float d) = 0;
	virtual void WorldUpDown(float d) = 0;

	// Rotate the camera.
	virtual void Pitch(float angle) = 0;
	virtual void RotateY(float angle) = 0;

	// After modifying camera position/orientation, call to rebuild the view matrix.
	virtual void UpdateViewMatrix() = 0;

	// Get View/Proj matrices.
	///virtual XMMATRIX View() const = 0;
	///virtual XMMATRIX Proj() const = 0;
	///virtual XMMATRIX ViewProj() const = 0;

	// Set frustum.
	virtual void SetLens(float fovY, float aspect, float zn, float zf) = 0;
};