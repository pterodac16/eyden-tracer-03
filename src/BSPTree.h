#pragma once

#include "BSPNode.h"
#include "BoundingBox.h"

namespace {
	inline int MaxDim(Vec3f v)
	{
		return (v.val[0] > v.val[1]) ? ((v.val[0] > v.val[2]) ? 0 : 2) : ((v.val[1] > v.val[2]) ? 1 : 2);
	}
}

class BSPTree
{
public:
	/**
	 * @brief Constructor
	 * @param bounds The scene bounding box
	 * @param vpPrims The vector of pointers to the primitives in the scene
	 */
	BSPTree(CBoundingBox& bounds, const std::vector<std::shared_ptr<CPrim>>& vpPrims)
		: m_bounds(bounds)
		, m_maxDepth(20)
		, m_minTri(3)
		, m_root(nullptr)
	{
		m_root = BuildTree(bounds, vpPrims, 0);
	}
	
	/**
	 * @brief Builds the BSP tree
	 * @details This function builds the BSP tree recursively
	 * @param box The bounding box containing all the scene primitives
	 * @param vpPrims The vector of pointers to the primitives included in the bounding box \b box
	 * @param depth The distance from the root node of the tree
	 */
	std::shared_ptr<CBSPNode> BuildTree(const CBoundingBox& box, const std::vector<std::shared_ptr<CPrim>>& vpPrims, int depth)
	{
		if (depth > m_maxDepth || vpPrims.size() < m_minTri) {
			std::shared_ptr<CBSPNode> leafNode = std::make_shared<CBSPNode>(vpPrims);
			return leafNode;
		}
		
		Vec3f voxelDimensions(box.m_max[0] - box.m_min[0], box.m_max[1] - box.m_min[1], box.m_max[2] - box.m_min[2]);
		int splitDim = MaxDim(voxelDimensions);
		float splitVal = (box.m_max[splitDim] + box.m_min[splitDim]) / 2;
		CBoundingBox leftVox(box);
		CBoundingBox rightVox (box);
		leftVox.m_max[splitDim] = splitVal;
		rightVox.m_min[splitDim] = splitVal;
		std::vector<std::shared_ptr<CPrim>> leftPrims;
		std::vector<std::shared_ptr<CPrim>> rightPrims;

		for (auto prim : vpPrims) {
			if (prim->inVoxel(leftVox)) {
				leftPrims.push_back(prim);
			}
			else {
				rightPrims.push_back(prim);
			}
		}
		std::shared_ptr<CBSPNode> pLeft = BuildTree(leftVox, leftPrims, depth + 1);
		std::shared_ptr<CBSPNode> pRight = BuildTree(rightVox, rightPrims, depth + 1);

		return std::make_shared<CBSPNode>(splitVal, splitDim, pLeft, pRight);
	}

	/**
	 * @brief Checks whether the ray \b ray intersects a primitive.
	 * @details If ray \b ray intersects a primitive, the \b ray.t value will be updated
	 * @param[in,out] ray The ray
	 */
	bool Intersect(Ray& ray)
	{
		float t0 = 0;
		float t1 = 0;
		m_bounds.clip(ray, t0, t1);
		if (t0 == 0 && t1 == 0)
			return false;
		else {
			return m_root->traverse(ray, t0, t1);
		}
	}

	
private:
	CBoundingBox 				m_bounds;
	const int 	 				m_maxDepth;
	const int 	 				m_minTri;
	std::shared_ptr<CBSPNode> 	m_root;
};
	
