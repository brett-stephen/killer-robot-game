enum facing
{
    FACE_FORWARD,
    FACE_RIGHT,
    FACE_BACK,
    FACE_LEFT
};

enum alert_camera_views
{
	// Each of these descriptions are 
	// relative to the  robots center.
	FRONT_LEFT, // "F8"
	FRONT_RIGHT, // "F7"
	BACK_LEFT, // "F5"
	BACK_RIGHT, // "F6"
	FRONT_LEFT_FAR, // "F9"
	FRONT_RIGHT_FAR, // "F10"
	BACK_LEFT_FAR, // "F11"
	BACK_RIGHT_FAR, // "F12"
	DEFAULT // "No function key pressed"
};

int can_move_x();
int can_move_z();
void turn_right_if_possible();
void turn_left_if_possible();
void turn_around();
void move_forward();
void keyboard(unsigned char key, int x, int y);
void special_keyboard_up(int key, int x, int y);
void special_keyboard(int key, int x, int y);
float * get_camera_offset();
