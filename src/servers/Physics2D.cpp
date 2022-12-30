#include "servers/Physics2D.h"

Physics2D& Physics2D::ins(){
	static Physics2D instance;
	return instance;
}

