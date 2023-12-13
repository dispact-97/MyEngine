#pragma once

class IDX11Render abstract
{
public:
	IDX11Render() {}
	virtual ~IDX11Render() {};	// 인터페이스 소멸자는 virtual로

	virtual long Initialize() abstract;

	virtual void Update(float deltaTime,float fps, float mspf) abstract;
	virtual void Render() abstract;
	virtual void BeginRender(float red, float green, float blue, float alpha) abstract;
	virtual void DrawObject() abstract;
	virtual void EndRender()  abstract;

	virtual void Finalize() abstract;
};

// dll외부에서 사용가능한 함수라고 알려주는 것
extern "C" __declspec(dllexport) IDX11Render * CreateRenderer();
extern "C" __declspec(dllexport) void DeleteRenderer(IDX11Render * renderer);

