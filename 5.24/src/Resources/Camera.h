#include <SDL_rect.h>

#ifndef CAMERA_H
#define CAMERA_H

#define FILE_CAMERA_SPEED "dcamera_speed"

class Entity;

class Camera {
public:
	Camera(int width, int height);

	void toggle();
	void move(int dir, float dis = 0);
	void update();
	void update(float x, float y);
	void center(Entity *entity);  // change camera focus
	void zoom(const float &amount);

	void rotate(double angle);
	void set_rotation(double angle);
	void scale(float factor);
	void set_scale(float factor);

	bool get_locked();

	float get_x() { return _x; }
	float get_y() { return _y; }

	float get_scale()	  { return _uniform_scale;	  }
	double get_rotation() { return _uniform_rotation; }

	SDL_Rect &get_rect() { return _rect; }
	void set_size(int width, int height);

	friend class Window;
	friend class Renderer;
private:
	const char *_FILE_PATH = "Data/system.txt";
	const float _SPEED = 500.0f;
	float _x, _y, _speed;
	bool _is_locked;
	Entity *_entity;  // entity to center camera on

	double _uniform_rotation;
	float _uniform_scale;

	SDL_Rect _rect;
};

#endif