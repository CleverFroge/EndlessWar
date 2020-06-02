#include "FrogEngine.h"
#include "Tank.h"
using namespace FrogEngine;

class TankController : public Component
{

private:
	Tank* _tank;
	float _mouseSensitivity = 0.05;
public:
	TankController();
	~TankController();
	void Awake();
	void Update();
	void LateUpdate();
};