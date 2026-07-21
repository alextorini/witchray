#include "witch_animation.h"
#include "witch_components.h"

void switchAnimation(Animation *animation, AnimationIndex newState) {
    if (newState >= animation->set->count) {
        return;
    }

    if (animation->currentClip != newState) {
        animation->currentClip = newState;
        animation->currentFrame = animation->set->clips[newState].startFrame;
        animation->timer = 0.0f;
    }
}
