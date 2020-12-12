#include "Collision.hpp"
#include "ecs/ColliderComponent.hpp"

bool Collision::AABB(const SDL_Rect &recA, const SDL_Rect &recB)
{
	auto recA_left = recA.x;
	auto recA_right = recA.x + recA.w;
	auto recB_left = recB.x;
	auto recB_right = recB.x + recB.w;

	auto recA_top = recA.y;
	auto recA_bottom = recA.y + recA.h;
	auto recB_top = recB.y;
	auto recB_bottom = recB.y + recB.h;

	auto recA_width = recA_right - recA_left;
	auto recA_height = recA_bottom - recA_top;

	auto acceptableOverlap = 0.1f;

	recA_left += recA_width * acceptableOverlap;
	recA_right -= recA_width * acceptableOverlap;
	recA_top += recA_height * acceptableOverlap;
	recA_bottom -= recA_height * acceptableOverlap;

	auto recB_width = recB_right - recB_left;
	auto recB_height = recB_bottom - recB_top;

	recB_left += recB_width * acceptableOverlap;
	recB_right -= recB_width * acceptableOverlap;
	recB_top += recB_height * acceptableOverlap;
	recB_bottom -= recB_height * acceptableOverlap;

	if (recA_right >= recB_left && recB_right >= recA_left && recA_bottom >= recB_top && recB_bottom >= recA_top) {
		return true;
	}
	return false;
}

bool Collision::AABB(const ColliderComponent &colA, const ColliderComponent &colB)
{
	if (AABB(colA.collider, colB.collider)) {
		return true;
	}
	return false;
}