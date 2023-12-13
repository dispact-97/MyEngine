#pragma once

class IDX11Render abstract
{
public:
	IDX11Render() {}
	virtual ~IDX11Render() {};	// �������̽� �Ҹ��ڴ� virtual��

	virtual long Initialize() abstract;

	virtual void Update(float deltaTime,float fps, float mspf) abstract;
	virtual void Render() abstract;
	virtual void BeginRender(float red, float green, float blue, float alpha) abstract;
	virtual void DrawObject() abstract;
	virtual void EndRender()  abstract;

	virtual void Finalize() abstract;
};

// dll�ܺο��� ��밡���� �Լ���� �˷��ִ� ��
extern "C" __declspec(dllexport) IDX11Render * CreateRenderer();
extern "C" __declspec(dllexport) void DeleteRenderer(IDX11Render * renderer);

