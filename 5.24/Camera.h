#ifndef CAMERA_H
#define CAMERA_H

#define FILE_CAMERA_SPEED "dcamera_speed"

class Entity;

class Camera {
public:
	Camera();

	void toggle();
	void move(int dir, double dis = 0);
	void update();
	void update(double x, double y);
	void center(Entity *entity);  // change camera focus
	void zoom(const float &amount);

	void rotate(double angle);
	void set_rotation(double angle);
	void scale(float factor);
	void set_scale(float factor);

	bool get_locked();

	double get_x() { return _x; }
	double get_y() { return _y; }

	float get_scale()	  { return _uniform_scale;	  }
	double get_rotation() { return _uniform_rotation; }

	friend class Window;
	friend class Renderer;
private:
	const char *_FILE_PATH = "Data/system.txt";
	const double _SPEED = 500.0;
	double _x, _y, _speed;
	bool _is_locked;
	Entity *_entity;  // entity to center camera on

	double _uniform_rotation;
	float _uniform_scale;
};

#endif