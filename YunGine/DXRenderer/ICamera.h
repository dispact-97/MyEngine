#pragma once
/// <summary>
/// 카메라가 여러개 필요한 이유?-> 여러시점으로 필요할 수도 있고 각각 다른것을 봐야할수도 있기때문에
/// 하지만 교수님은 사용하시지 않는 것 같다.
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