#include "FrogEngine.h"
#include "Tank.h"
using namespace FrogEngine;

class TankController : public Component
{

private:
	Tank* _tank;
public:
	TankController() : Component{"TankController"}
	{
	}
	~TankController()
	{
	}
	void Awake()
	{
		_tank = (Tank*)_node;
	}
	void Update()
	{
		if (Input::GetKey(GLFW_MOUSE_BUTTON_LEFT))
		{
			_tank->Fire();
		}
		//��������ƶ�����
		_tank->RotateCannon(Input::GetMousePosDeltaX(), Input::GetMousePosDeltaY());
		//�����������
		float deltaTime = Time::GetDeltaTime();
		_tank->Move(Input::GetAxis(), deltaTime);
	}
	void LateUpdate()
	{
	}
	
private:
	
};