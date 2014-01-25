//
// evoshooter
// Copyright (c) 2014 
// Borislav Stanimirov, Filip Chorbadzhiev, Nikolay Dimitrov
// Assen Kanev, Jem Kerim, Stefan Ivanov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//

#include "EvoShooter.pch.h"

#include "AnimationsController.h"
#include "ResourceManager.h"
#include "Sprite.h"

static Uint32 ANIM_TIME = 1000;

AnimationsController::AnimationsController()
: m_ActiveMovement(MA_None)
, m_IsTakingDamage(false)
, m_IsDead(false)
{

}

AnimationsController::~AnimationsController()
{
}

void AnimationsController::SetMovementAnimations(const std::string& fileMove, float scale)
{
    m_MovementAnimations[MA_Left] = ResourceManager::instance().createSpriteFromSingleAnimationTexture(fileMove, 1, 8, ANIM_TIME);
    m_MovementAnimations[MA_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture(fileMove, 1, 8, ANIM_TIME);

    m_MovementAnimations[MA_Left]->setScale(scale);
    m_MovementAnimations[MA_Right]->setScale(scale);
    m_MovementAnimations[MA_Right]->setFlipX(true);
}

void AnimationsController::SetDeath(const std::string& fileDeath, float scale)
{
    m_Death[MA_Left] = ResourceManager::instance().createSpriteFromSingleAnimationTexture(fileDeath, 1, 8, ANIM_TIME);
    m_Death[MA_Left]->setLoop(false);
    m_Death[MA_Left]->setScale(scale);

    m_Death[MA_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture(fileDeath, 1, 8, ANIM_TIME);
    m_Death[MA_Right]->setLoop(false);
    m_Death[MA_Right]->setScale(scale);
    m_Death[MA_Right]->setFlipX(true);
}

void AnimationsController::SetDamage(const std::string& fileDamage, float scale)
{
    m_Damage[MA_Left] = ResourceManager::instance().createSpriteFromSingleAnimationTexture(fileDamage, 1, 8, ANIM_TIME);
    m_Damage[MA_Left]->setScale(scale);

    m_Damage[MA_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture(fileDamage, 1, 8, ANIM_TIME);
    m_Damage[MA_Right]->setScale(scale);
    m_Damage[MA_Right]->setFlipX(true);
}

void AnimationsController::AddAttachment(const std::string& file, const mathgp::vector3& offset, float scale)
{
    SpritePtr spriteLeft = ResourceManager::instance().createSpriteFromSingleAnimationTexture(file, 1, 8, ANIM_TIME);
    SpritePtr spriteRight = ResourceManager::instance().createSpriteFromSingleAnimationTexture(file, 1, 8, ANIM_TIME);
    spriteLeft->setScale(scale);
    spriteRight->setScale(scale);
    spriteRight->setFlipX(true);

    m_Attachments[file] = { offset, { spriteLeft, spriteRight } };
}

void AnimationsController::RemoveAttachment(const std::string& file)
{
    m_Attachments.erase(file);
}

void AnimationsController::SetMove(MovementAnim anim)
{
    if (m_IsDead || m_IsTakingDamage)
    {
        return;
    }

    int activeFrame = 0;
    if (m_ActiveMovement != MA_None)
    {
        activeFrame = m_MovementAnimations[m_ActiveMovement]->currentFrame();
        m_MovementAnimations[m_ActiveMovement]->stopRendering();
    }

    m_ActiveMovement = anim;

    if (m_ActiveMovement != MA_None)
    {
        m_MovementAnimations[m_ActiveMovement]->startRendering(activeFrame);
    }
}

void AnimationsController::Die()
{
    if (m_ActiveMovement != MA_None)
    {
        m_MovementAnimations[m_ActiveMovement]->stopRendering();
    }

    if (m_ActiveMovement == MA_None)
    {
        m_ActiveMovement = MA_Left;
    }

    m_Death[m_ActiveMovement]->startRendering(0);

    m_IsDead = true;
    m_IsTakingDamage = false;
}

void AnimationsController::GetDamage()
{
    if (m_IsDead)
    {
        return;
    }

    int activeFrame = 0;
    if (m_ActiveMovement != MA_None)
    {
        activeFrame = m_MovementAnimations[m_ActiveMovement]->currentFrame();
        m_MovementAnimations[m_ActiveMovement]->stopRendering();
    }

    if (m_ActiveMovement == MA_None)
    {
        m_ActiveMovement = MA_Left;
    }

    m_Damage[m_ActiveMovement]->startRendering(activeFrame);

    m_DamageStartTime = SDL_GetTicks();
    m_IsTakingDamage = true;
}

void AnimationsController::update(const mathgp::vector3& position, const mathgp::vector3& camDir)
{
    if (m_IsDead)
    {
        m_Death[m_ActiveMovement]->update(position, camDir);
        updateAttachments(position, camDir);
        return;
    }

    if (m_IsTakingDamage)
    {
        Uint32 timeDiff = SDL_GetTicks() - m_DamageStartTime;

        if (timeDiff >= ANIM_TIME)
        {
            m_IsTakingDamage = false;
            int activeFrame = m_Damage[m_ActiveMovement]->currentFrame();
            m_Damage[m_ActiveMovement]->stopRendering();
            m_MovementAnimations[m_ActiveMovement]->startRendering(activeFrame);
        }
        else
        {
            m_Damage[m_ActiveMovement]->update(position, camDir);
            updateAttachments(position, camDir);
        }
    }

    m_MovementAnimations[m_ActiveMovement]->update(position, camDir);
    updateAttachments(position, camDir);
}

void AnimationsController::updateAttachments(const mathgp::vector3& position, const mathgp::vector3& camDir)
{
    for (auto it = m_Attachments.begin(); it != m_Attachments.end(); ++it)
    {
        if (it->second.Animation[m_ActiveMovement]->isRendering())
        {
            it->second.Animation[m_ActiveMovement]->update(position + it->second.Offset, camDir);
        }
        else
        {
            int activeFrame = it->second.Animation[!m_ActiveMovement]->currentFrame();
            it->second.Animation[!m_ActiveMovement]->stopRendering();
            it->second.Animation[m_ActiveMovement]->startRendering(activeFrame);
        }
    }
}
