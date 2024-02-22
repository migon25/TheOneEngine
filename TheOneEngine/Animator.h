#pragma once

#include "Component.h"

#include "Defs.h"

#include <map>
#include <string>

struct Channel
{
	std::string name;

	std::map<double, vec3> positionKeys;
	std::map<double, quat> rotationKeys;
	std::map<double, vec3> scaleKeys;

	bool HasPosKey() const;
	std::map<double, vec3>::const_iterator GetPrevPosKey(double currentKey) const;
	std::map<double, vec3>::const_iterator GetNextPosKey(double currentKey) const;

	bool HasRotKey() const;
	std::map<double, quat>::const_iterator GetPrevRotKey(double currentKey) const;
	std::map<double, quat>::const_iterator GetNextRotKey(double currentKey) const;

	bool HasScaleKey() const;
	std::map<double, vec3>::const_iterator GetPrevScaleKey(double currentKey) const;
	std::map<double, vec3>::const_iterator GetNextScaleKey(double currentKey) const;
};

class Animation
{
public:

	Animation();
	~Animation();

	float duration;
	float ticksPerSecond;
	bool loopable = true;

	std::map<std::string, Channel> channels;
};

class Animator : public Component
{
public:
	Animator(std::shared_ptr<GameObject> containerGO);
	
	json SaveComponent();
	void LoadComponent(const json& meshJSON);

private:

public:

};