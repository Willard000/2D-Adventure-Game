#ifndef CAMERA_H
#define CAMERA_H

struct Camera {
	double x = 0.0, y = 0.0, speed = 100.0;
	bool is_locked = true;
};

#endif