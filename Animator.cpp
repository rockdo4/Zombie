#include "stdafx.h"
#include "Animator.h"

void Animator::Update(float dt)
{
	if (!isPlaying)
		return;

	accumTime += dt;
	if (accumTime < frameDuration)
		return;

	++currentFrame;
	accumTime = 0.f;

	if (currentFrame == totalFrame)
	{
		switch (currentClip->loopType)
		{
		case AnimationLoopTypes::Single:
			currentFrame = totalFrame - 1;
			break;
		case AnimationLoopTypes::Loop:
			currentFrame = 0;
			break;
		}
	}

	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::Play(AnimationClip* clip)
{
	isPlaying = true;

	currentClip = clip;
	currentFrame = 0;
	totalFrame = clip->frames.size();

	frameDuration = 1.f / clip->fps;

	accumTime = 0.f;
	SetFrame(currentClip->frames[currentFrame]);
}

void Animator::Stop()
{
	isPlaying = false;
}

void Animator::SetFrame(const AnimationFrame& frame)
{
	sprite->setTexture(TEXTURE_MGR.Get(frame.texId));
	sprite->setTextureRect(frame.texCoord);
}
