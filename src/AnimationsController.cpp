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
, m_ActiveAttack(0)
, m_IsAttacking(false)
, m_AtackStartTime(0)
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

void AnimationsController::AddAttack(const std::string& fileAttack, const std::string& fileAttackIdle, const mathgp::vector3& offset, float scale)
{
    AnimWithOffsetAndTwoStates anim;

    anim.Animation[0][MA_Left] = ResourceManager::instance().createSpriteFromSingleAnimationTexture(fileAttackIdle, 1, 8, ANIM_TIME);
    anim.Animation[0][MA_Left]->setScale(scale);
    anim.Animation[0][MA_Left]->setSortingYOffset(-offset.y() - 0.001f);

    anim.Animation[0][MA_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture(fileAttackIdle, 1, 8, ANIM_TIME);
    anim.Animation[0][MA_Right]->setScale(scale);
    anim.Animation[0][MA_Right]->setFlipX(true);
    anim.Animation[0][MA_Right]->setSortingYOffset(-offset.y() - 0.001f);

    anim.Animation[1][MA_Left] = ResourceManager::instance().createSpriteFromSingleAnimationTexture(fileAttack, 1, 8, ANIM_TIME);
    anim.Animation[1][MA_Left]->setScale(scale);
    anim.Animation[1][MA_Left]->setSortingYOffset(-offset.y() - 0.001f);

    anim.Animation[1][MA_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture(fileAttack, 1, 8, ANIM_TIME);
    anim.Animation[1][MA_Right]->setScale(scale);
    anim.Animation[1][MA_Right]->setFlipX(true);
    anim.Animation[1][MA_Right]->setSortingYOffset(-offset.y() - 0.001f);

    anim.Offsets[MA_Left] = offset;
    anim.Offsets[MA_Right] = offset;
    
    anim.Offsets[MA_Right].x() = m_MovementAnimations[MA_Right]->scaledFrameWidth() - anim.Animation[0][MA_Right]->scaledFrameWidth() - offset.x();

    m_Attacks.push_back(anim);
}

void AnimationsController::AddAttachment(const std::string& file, const mathgp::vector3& offset, float scale)
{
    SpritePtr spriteLeft = ResourceManager::instance().createSpriteFromSingleAnimationTexture(file, 1, 8, ANIM_TIME);
    SpritePtr spriteRight = ResourceManager::instance().createSpriteFromSingleAnimationTexture(file, 1, 8, ANIM_TIME);
    spriteLeft->setScale(scale);
    spriteLeft->setSortingYOffset(-offset.y() - 0.001f);
    spriteRight->setScale(scale);
    spriteRight->setFlipX(true);
    spriteRight->setSortingYOffset(-offset.y() - 0.001f);

    mathgp::vector3 flipedOffset = offset;
    flipedOffset.x() = m_MovementAnimations[MA_Right]->scaledFrameWidth() - spriteRight->scaledFrameWidth() - offset.x();

    m_Attachments[file] = { { offset, flipedOffset }, { spriteLeft, spriteRight } };
}

void AnimationsController::RemoveAttachment(const std::string& file)
{
    m_Attachments.erase(file);
}

void AnimationsController::SetMove(MovementAnim anim)
{
    if (m_IsDead)
    {
        return;
    }

    if (m_ActiveMovement == anim)
    {
        return;
    }

    if (m_IsTakingDamage)
    {
        int activeFrame = 0;
        if (m_ActiveMovement != MA_None)
        {
            activeFrame = m_Damage[m_ActiveMovement]->currentFrame();
            m_Damage[m_ActiveMovement]->stopRendering();
        }

        m_ActiveMovement = anim;

        m_Damage[m_ActiveMovement]->startRendering(activeFrame);

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
    if (m_IsTakingDamage)
    {
        m_Damage[m_ActiveMovement]->stopRendering();
    }
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

void AnimationsController::Attack(Uint32 attackIndex)
{
    if (attackIndex >= m_Attacks.size())
    {
        assert(false);
        return;
    }

    if (m_ActiveAttack >= 0 && m_ActiveAttack != attackIndex)
    {
        AnimWithOffsetAndTwoStates& oldAttack = m_Attacks[m_ActiveAttack];
        oldAttack.Animation[0][MA_Left]->stopRendering();
        oldAttack.Animation[0][MA_Right]->stopRendering();

        oldAttack.Animation[1][MA_Left]->stopRendering();
        oldAttack.Animation[1][MA_Right]->stopRendering();
    }

    AnimWithOffsetAndTwoStates& newAttack = m_Attacks[attackIndex];

    if (m_ActiveAttack != attackIndex)
    {
        newAttack.Animation[0][m_ActiveMovement]->stopRendering();
        newAttack.Animation[1][m_ActiveMovement]->startRendering(0);
    }
    
    newAttack.Animation[0][m_ActiveMovement]->stopRendering();

    m_IsAttacking = true;
    m_ActiveAttack = attackIndex;
    m_AtackStartTime = SDL_GetTicks();
}

void AnimationsController::update(const mathgp::vector3& position, const mathgp::vector3& camDir)
{
    updateAttack(position, camDir);

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

    if (m_ActiveMovement == MA_None)
    {
        return;
    }

    m_MovementAnimations[m_ActiveMovement]->update(position, camDir);
    updateAttachments(position, camDir);
}

void AnimationsController::updateAttack(const mathgp::vector3& position, const mathgp::vector3& camDir)
{
    if (m_ActiveAttack < 0 || m_ActiveAttack >= m_Attacks.size())
    {
        return;
    }

    AnimWithOffsetAndTwoStates& anim = m_Attacks[m_ActiveAttack];

    if (m_IsAttacking && (SDL_GetTicks() - m_AtackStartTime) >= ANIM_TIME)
    {
        m_IsAttacking = false;
        anim.Animation[1][MA_Left]->stopRendering();
        anim.Animation[1][MA_Right]->stopRendering();

        anim.Animation[0][m_ActiveMovement]->startRendering(0);
    }

    if (m_IsAttacking)
    {
        if (anim.Animation[1][m_ActiveMovement]->isRendering())
        {
            anim.Animation[1][m_ActiveMovement]->update(position + anim.Offsets[m_ActiveMovement], camDir);
        }
        else
        {
            int activeFrame = anim.Animation[1][!m_ActiveMovement]->currentFrame();
            anim.Animation[1][!m_ActiveMovement]->stopRendering();
            anim.Animation[1][m_ActiveMovement]->startRendering(activeFrame);
            anim.Animation[1][m_ActiveMovement]->update(position + anim.Offsets[m_ActiveMovement], camDir);
        }
    }
    else
    {
        if (anim.Animation[0][m_ActiveMovement]->isRendering())
        {
            anim.Animation[0][m_ActiveMovement]->update(position + anim.Offsets[m_ActiveMovement], camDir);
        }
        else
        {
            int activeFrame = anim.Animation[0][!m_ActiveMovement]->currentFrame();
            anim.Animation[0][!m_ActiveMovement]->stopRendering();
            anim.Animation[0][m_ActiveMovement]->startRendering(activeFrame);
            anim.Animation[0][m_ActiveMovement]->update(position + anim.Offsets[m_ActiveMovement], camDir);
        }
    }
}

void AnimationsController::updateAttachments(const mathgp::vector3& position, const mathgp::vector3& camDir)
{
    for (auto it = m_Attachments.begin(); it != m_Attachments.end(); ++it)
    {
        if (it->second.Animation[m_ActiveMovement]->isRendering())
        {
            it->second.Animation[m_ActiveMovement]->update(position + it->second.Offsets[m_ActiveMovement], camDir);
        }
        else
        {
            int activeFrame = it->second.Animation[!m_ActiveMovement]->currentFrame();
            it->second.Animation[!m_ActiveMovement]->stopRendering();
            it->second.Animation[m_ActiveMovement]->startRendering(activeFrame);
            it->second.Animation[m_ActiveMovement]->update(position + it->second.Offsets[m_ActiveMovement], camDir);
        }
    }
}
