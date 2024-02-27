#ifndef __SOURCE_H__
#define __SOURCE_H__
#pragma once

#include "Component.h"
#include "AudioObject.h"

class GameObject;

class Source : public Component, public AudioObject {
public:
	Source(std::shared_ptr<GameObject> containerGO);
	virtual ~Source();

	json SaveComponent();
	void LoadComponent(const json& transformJSON);

private:
	string path;
	int volume;
};

#endif // !__SOURCE_H__