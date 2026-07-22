#include "witch_animation.h"
#include "witch_components.h"

void switchAnimation(Animation *animation, AnimationIndex newState) {
    if (newState >= ANIMATION_COUNT) {
        return;
    }

    animation->currentClip = newState;
    animation->currentFrame = animation->set->clips[newState].startFrame;
    animation->timer = 0.0f;
}
