#pragma once

#include "types.h"

#include <limits>
#include "ray.h"

namespace {
	inline Vec3f Min3f(const Vec3f a, const Vec3f b)
	{
		return Vec3f(MIN(a.val[0], b.val[0]), MIN(a.val[1], b.val[1]), MIN(a.val[2], b.val[2]));
	}

	inline Vec3f Max3f(const Vec3f a, const Vec3f b)
	{
		return Vec3f(MAX(a.val[0], b.val[0]), MAX(a.val[1], b.val[1]), MAX(a.val[2], b.val[2]));
	}
}

/**
 * @brief Bounding Box class
 */
class CBoundingBox
{
public:
	CBoundingBox(void) {
		clear();
	}

	CBoundingBox(const CBoundingBox &box) {
		m_min = box.m_min;
		m_max = box.m_max;
	}

	~CBoundingBox(void)= default;
	
	/**
	 * @brief Resets the bounding box
	 * @details This function resets the member variables \b m_min and \b m_max
	 */
	void clear(void)
	{	
		m_min[0] = std::numeric_limits<float>::infinity();
		m_min[1] = std::numeric_limits<float>::infinity();
		m_min[2] = std::numeric_limits<float>::infinity();

		m_max[0] = -std::numeric_limits<float>::infinity();
		m_max[1] = -std::numeric_limits<float>::infinity();
		m_max[2] = -std::numeric_limits<float>::infinity();
	}
	
	/**
	 * @brief Extends the bounding box to contain point \b a
	 * @param a A point
	 */
	void extend(Vec3f a)
	{
		m_min = Min3f(m_min, a);
		m_max = Max3f(m_max, a);
	}
	
	/**
	 * @brief Extends the bounding box to contain bounding box \b box
	 * @param box The second bounding box
	 */
	void extend(const CBoundingBox& box)
	{
		extend(box.m_max);
		extend(box.m_min);
	}
	
	/**
	 * @brief Checks if the current bounding box overlaps with the argument bounding box \b box
	 * @param box The secind bounding box to be checked with
	 */
	bool overlaps(const CBoundingBox& box)
	{
		bool overlapX = m_min[0] <= box.m_max[0] && box.m_min[0] <= m_max[0];
		bool overlapY = m_min[1] <= box.m_max[1] && box.m_min[1] <= m_max[1];
		bool overlapZ = m_min[2] <= box.m_max[2] && box.m_min[2] <= m_max[2];
		return overlapX && overlapY && overlapZ;
	}
	
	/**
	 * @brief Clips the ray with the bounding box
	 * @param[in] ray The ray
	 * @param[in,out] t0 The distance from ray origin at which the ray enters the bounding box
	 * @param[in,out] t1 The distance from ray origin at which the ray leaves the bounding box
	 */
	void clip(const Ray& ray, float& t0, float& t1)
	{
			float txNear = (m_min[0] - ray.org[0]) / ray.dir[0];
			float tyNear = (m_min[1] - ray.org[1]) / ray.dir[1];
			float tzNear = (m_min[2] - ray.org[2]) / ray.dir[2];

			float txFar = (m_max[0] - ray.org[0]) / ray.dir[0];
			float tyFar = (m_max[1] - ray.org[1]) / ray.dir[1];
			float tzFar = (m_max[2] - ray.org[2]) / ray.dir[2];

			if (ray.dir[0] < 0) {
				std::swap(txNear, txFar);
			}

			if (ray.dir[1] < 0) {
				std::swap(tyNear, tyFar);
			}

			if (ray.dir[2] < 0) {
				std::swap(tzNear, tzFar);
			}

			float maxTNear = MAX(MAX(txNear, tyNear), tzNear);
			float minTFar = MIN(MIN(txFar, tyFar), tzFar);

			if (maxTNear < minTFar) {
				t0 = maxTNear;
				t1 = minTFar;
			}
	}
	
	
public:
	Vec3f m_min;	///< The minimal point defying the size of the bounding box
	Vec3f m_max;	///< The maximal point defying the size of the bounding box
};
