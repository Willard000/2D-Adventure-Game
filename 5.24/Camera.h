#ifndef CAMERA_H
#define CAMERA_H

#define FILE_CAMERA_SPEED "dcamera_speed"

class Entity;

class Camera {
public:
	Camera();

	void toggle();
	void move(int dir);
	void update();
	void update(double x, double y);
	void center(Entity *entity);  // change camera focus

	bool getLocked();

	friend class Window;
	friend class Renderer;
private:
	const char *_FILE_PATH = "Data/system.txt";
	const double _SPEED = 500.0;
	double _x, _y, _speed;
	bool _is_locked;
	Entity *_entity;  // entity to center camera on
};

#endif