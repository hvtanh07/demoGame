﻿
#include <d3d9.h>

class Camera
{
protected:

	float _xCam;
	float _yCam;

	int _width;
	int _height;

	float vx;
	DWORD dt;

	float _boundaryLeft; // biên giới hạn bên trái 
	float _boundaryRight; // biên giới hạn bên phải, không bao gồm đoạn simon đi được ở cuối - SCREEN_WIDTH 

public:
	Camera(int w, int h);
	~Camera();
	void Update(DWORD dt);
	void SetPosition(float x, float y);
};

