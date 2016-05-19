#pragma once

#include <assimp/scene.h>

class BoneInfo {
public:

	BoneInfo();
	~BoneInfo();

	aiMatrix4x4 BoneOffset;
	aiMatrix4x4 FinalTransformation;
};