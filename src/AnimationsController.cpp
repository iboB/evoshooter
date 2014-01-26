//
// evoshooter
// Copyright (c) 2014 
// Borislav Stanimirov, Filip Chorbadzhiev, Nikolay Dimitrov
// Assen Kanev, Jem Kerim, Stefan Ivanov
//
//This game and all content in this file is licensed under  
//the Attribution-Noncommercial-Share Alike 3.0 version of the Creative Commons License.
//For reference the license is given below and can also be found at http://creativecommons.org/licenses/by-nc-sa/3.0/
//

#include "EvoShooter.pch.h"

#include "AnimationsController.h"
#include "ResourceManager.h"
#include "Sprite.h"

static Uint32 ANIM_TIME = 1000;

AnimationsController::AnimationsController()
: m_ActiveMovement(MA_Left)
, m_IsTakingDamage(false)
, m_IsDead(false)
, m_ActiveAttack(0)
, m_IsAttacking(false)
, m_AtackStartTime(0)
, m_IsReadyForDiscard(false)
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

void AnimationsController::AddAttack(const std::string& fileAttack, const std::string& fileAttackIdle, const mathgp::vector3& offset, float scale, bool wholeBodyAttack)
{
    AttackAnimation anim;
    anim.m_IsWholeBodyAttack = wholeBodyAttack;

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
        if (!m_IsAttacking || !m_Attacks[m_ActiveAttack].m_IsWholeBodyAttack)
        {
            m_MovementAnimations[m_ActiveMovement]->startRendering(activeFrame);
        }
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

    if (m_IsAttacking && m_Attacks[m_ActiveAttack].m_IsWholeBodyAttack)
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

    if (m_IsTakingDamage && m_Attacks[attackIndex].m_IsWholeBodyAttack)
    {
        return;
    }

    if (m_ActiveAttack >= 0 && m_ActiveAttack != attackIndex)
    {
        AttackAnimation& oldAttack = m_Attacks[m_ActiveAttack];
        oldAttack.Animation[0][MA_Left]->stopRendering();
        oldAttack.Animation[0][MA_Right]->stopRendering();

        oldAttack.Animation[1][MA_Left]->stopRendering();
        oldAttack.Animation[1][MA_Right]->stopRendering();
    }

    AttackAnimation& newAttack = m_Attacks[attackIndex];

    if (m_ActiveAttack != attackIndex)
    {
        newAttack.Animation[0][m_ActiveMovement]->stopRendering();
        newAttack.Animation[1][m_ActiveMovement]->startRendering(0);
    }
    
    newAttack.Animation[0][m_ActiveMovement]->stopRendering();

    m_IsAttacking = true;
    m_ActiveAttack = attackIndex;
    m_AtackStartTime = SDL_GetTicks();

    if (newAttack.m_IsWholeBodyAttack && !m_IsDead)
    {
        m_MovementAnimations[m_ActiveMovement]->stopRendering();
        m_Damage[m_ActiveMovement]->stopRendering();
    }
}

void AnimationsController::update(const mathgp::vector3& position, const mathgp::vector3& camDir)
{
    updateAttack(position, camDir);

    if (m_IsDead)
    {
        if (m_Death[m_ActiveMovement]->isDone())
        {
            m_IsReadyForDiscard = true;
        }
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

    AttackAnimation& anim = m_Attacks[m_ActiveAttack];

    if (m_IsAttacking && (SDL_GetTicks() - m_AtackStartTime) >= ANIM_TIME)
    {
        m_IsAttacking = false;
        anim.Animation[1][MA_Left]->stopRendering();
        anim.Animation[1][MA_Right]->stopRendering();

        if (anim.m_IsWholeBodyAttack)
        {
            MovementAnim m = m_ActiveMovement;
            m_ActiveMovement = MovementAnim(!m_ActiveMovement);
            SetMove(m);
        }
        else
        {
            anim.Animation[0][m_ActiveMovement]->startRendering(0);
        }
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
            if (!anim.m_IsWholeBodyAttack)
            {
                anim.Animation[0][m_ActiveMovement]->startRendering(activeFrame);
                anim.Animation[0][m_ActiveMovement]->update(position + anim.Offsets[m_ActiveMovement], camDir);
            }
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

PlayerAnimationsController::PlayerAnimationsController()
: m_ActiveWeapon(PWT_Sword)
, m_ActiveMovement(PM_Idle)
, m_IsDead(false)
{
    //////////////////// sword
    m_Animations[PWT_Sword].Move[PM_Idle] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_idle_sword_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Sword].Move[PM_Idle]->setScale(0.008f);
    m_Animations[PWT_Sword].Move[PM_Idle]->startRendering(0);
    m_Animations[PWT_Sword].Move[PM_LEft] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_run_sword_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Sword].Move[PM_LEft]->setScale(0.008f);
    m_Animations[PWT_Sword].Move[PM_LEft]->setFlipX(true);
    m_Animations[PWT_Sword].Move[PM_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_run_sword_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Sword].Move[PM_Right]->setScale(0.008f);

    m_Animations[PWT_Sword].Attack[PM_Idle] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_sword_attack_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Sword].Attack[PM_Idle]->setScale(0.008f);
    m_Animations[PWT_Sword].Attack[PM_LEft] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_run_swordattack_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Sword].Attack[PM_LEft]->setScale(0.008f);
    m_Animations[PWT_Sword].Attack[PM_LEft]->setFlipX(true);
    m_Animations[PWT_Sword].Attack[PM_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_run_swordattack_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Sword].Attack[PM_Right]->setScale(0.008f);

    m_Animations[PWT_Sword].Damage[PM_Idle] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_dmg_sword_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Sword].Damage[PM_Idle]->setScale(0.008f);
    m_Animations[PWT_Sword].Damage[PM_LEft] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_run_hitsword_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Sword].Damage[PM_LEft]->setScale(0.008f);
    m_Animations[PWT_Sword].Damage[PM_LEft]->setFlipX(true);
    m_Animations[PWT_Sword].Damage[PM_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_run_hitsword_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Sword].Damage[PM_Right]->setScale(0.008f);

    //////////////////// pistol
    m_Animations[PWT_Pistol].Move[PM_Idle] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_idlegun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Pistol].Move[PM_Idle]->setScale(0.008f);
    m_Animations[PWT_Pistol].Move[PM_LEft] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_rungun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Pistol].Move[PM_LEft]->setScale(0.008f);
    m_Animations[PWT_Pistol].Move[PM_LEft]->setFlipX(true);
    m_Animations[PWT_Pistol].Move[PM_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_rungun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Pistol].Move[PM_Right]->setScale(0.008f);

    m_Animations[PWT_Pistol].Attack[PM_Idle] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_shootguninplace_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Pistol].Attack[PM_Idle]->setScale(0.008f);
    m_Animations[PWT_Pistol].Attack[PM_LEft] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_shootgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Pistol].Attack[PM_LEft]->setScale(0.008f);
    m_Animations[PWT_Pistol].Attack[PM_LEft]->setFlipX(true);
    m_Animations[PWT_Pistol].Attack[PM_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_shootgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Pistol].Attack[PM_Right]->setScale(0.008f);
    
    m_Animations[PWT_Pistol].Damage[PM_Idle] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_hitgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Pistol].Damage[PM_Idle]->setScale(0.008f);
    m_Animations[PWT_Pistol].Damage[PM_LEft] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_runhitgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Pistol].Damage[PM_LEft]->setScale(0.008f);
    m_Animations[PWT_Pistol].Damage[PM_LEft]->setFlipX(true);
    m_Animations[PWT_Pistol].Damage[PM_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_runhitgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Pistol].Damage[PM_Right]->setScale(0.008f);
    
    //////////////////// shotgun
    m_Animations[PWT_Shotgun].Move[PM_Idle] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_idle_shotgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Shotgun].Move[PM_Idle]->setScale(0.008f);
    m_Animations[PWT_Shotgun].Move[PM_LEft] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_runshotgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Shotgun].Move[PM_LEft]->setScale(0.008f);
    m_Animations[PWT_Shotgun].Move[PM_LEft]->setFlipX(true);
    m_Animations[PWT_Shotgun].Move[PM_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_runshotgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Shotgun].Move[PM_Right]->setScale(0.008f);
    
    m_Animations[PWT_Shotgun].Attack[PM_Idle] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_shotgun_attack_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Shotgun].Attack[PM_Idle]->setScale(0.008f);
    m_Animations[PWT_Shotgun].Attack[PM_LEft] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_shootshotgunmoving_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Shotgun].Attack[PM_LEft]->setScale(0.008f);
    m_Animations[PWT_Shotgun].Attack[PM_LEft]->setFlipX(true);
    m_Animations[PWT_Shotgun].Attack[PM_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_shootshotgunmoving_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Shotgun].Attack[PM_Right]->setScale(0.008f);

    m_Animations[PWT_Shotgun].Damage[PM_Idle] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_dmg_shotgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Shotgun].Damage[PM_Idle]->setScale(0.008f);
    m_Animations[PWT_Shotgun].Damage[PM_LEft] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_runhitshotgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Shotgun].Damage[PM_LEft]->setScale(0.008f);
    m_Animations[PWT_Shotgun].Damage[PM_LEft]->setFlipX(true);
    m_Animations[PWT_Shotgun].Damage[PM_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_runhitshotgun_anim.png", 1, 8, ANIM_TIME);
    m_Animations[PWT_Shotgun].Damage[PM_Right]->setScale(0.008f);

    //die
    m_DieAnimation[PM_Idle] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_die_anim.png", 1, 8, ANIM_TIME);
    m_DieAnimation[PM_Idle]->setScale(0.008f);
    m_DieAnimation[PM_Idle]->setLoop(false);
    m_DieAnimation[PM_LEft] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_die_anim.png", 1, 8, ANIM_TIME);
    m_DieAnimation[PM_LEft]->setScale(0.008f);
    m_DieAnimation[PM_LEft]->setFlipX(true);
    m_DieAnimation[PM_LEft]->setLoop(false);
    m_DieAnimation[PM_Right] = ResourceManager::instance().createSpriteFromSingleAnimationTexture("sprites/player/MC_die_anim.png", 1, 8, ANIM_TIME);
    m_DieAnimation[PM_Right]->setScale(0.008f);
    m_DieAnimation[PM_Right]->setLoop(false);
}

PlayerAnimationsController::~PlayerAnimationsController()
{
    
}

void PlayerAnimationsController::SetWeapon(PlayerWeaponType weapon)
{
    if (m_IsDead)
    {
        return;
    }

    if (m_ActiveWeapon == weapon)
    {
        return;
    }

    int currentFrame = StopAll();

    m_ActiveWeapon = weapon;

    if (m_IsAttacking)
    {
        m_Animations[m_ActiveWeapon].Attack[m_ActiveMovement]->startRendering(currentFrame);
        return;
    }

    if (m_IsTakingDamage)
    {
        m_Animations[m_ActiveWeapon].Damage[m_ActiveMovement]->startRendering(currentFrame);
        return;
    }

    m_Animations[m_ActiveWeapon].Move[m_ActiveMovement]->startRendering(currentFrame);
}

void PlayerAnimationsController::SetMovement(PlayerMovement movement)
{
    if (m_IsDead)
    {
        return;
    }

    if (m_ActiveMovement == movement)
    {
        return;
    }

    int currentFrame = StopAll();

    m_ActiveMovement = movement;

    if (m_IsAttacking)
    {
        m_Animations[m_ActiveWeapon].Attack[m_ActiveMovement]->startRendering(currentFrame);
        return;
    }

    if (m_IsTakingDamage)
    {
        m_Animations[m_ActiveWeapon].Damage[m_ActiveMovement]->startRendering(currentFrame);
        return;
    }

    m_Animations[m_ActiveWeapon].Move[m_ActiveMovement]->startRendering(currentFrame);
}

void PlayerAnimationsController::Die()
{
    m_IsDead = true;

    StopAll();

    m_DieAnimation[m_ActiveMovement]->startRendering(0);
}

void PlayerAnimationsController::GetDamage()
{
    if (m_IsDead)
    {
        return;
    }

    if (m_IsTakingDamage)
    {
        return;
    }

    m_IsAttacking = false;

    int currentFrame = StopAll();

    m_Animations[m_ActiveWeapon].Damage[m_ActiveMovement]->startRendering(currentFrame);

    m_IsTakingDamage = true;

    m_DamageStartTime = SDL_GetTicks();
}

void PlayerAnimationsController::Attack()
{
    if (m_IsDead)
        return;
    
    if (m_IsAttacking)
        return;

    int currentFrame = StopAll();

    m_Animations[m_ActiveWeapon].Attack[m_ActiveMovement]->startRendering(currentFrame);

    m_AtackStartTime = SDL_GetTicks();

    m_IsAttacking = true;
}

void PlayerAnimationsController::update(const mathgp::vector3& position, const mathgp::vector3& camDir)
{
    if (m_IsDead)
    {
        if (m_DieAnimation[m_ActiveMovement]->isDone())
        {
            m_IsReadyForDiscard = true;
        }
        m_DieAnimation[m_ActiveMovement]->update(position, camDir);
        return;
    }

    if (m_IsAttacking)
    {
        if ((SDL_GetTicks() - m_AtackStartTime) >= ANIM_TIME)
        {
            m_IsAttacking = false;

            int currentFrame = StopAll();
            m_Animations[m_ActiveWeapon].Move[m_ActiveMovement]->startRendering(currentFrame);
            m_Animations[m_ActiveWeapon].Move[m_ActiveMovement]->update(position, camDir);
        }
        else
        {
            m_Animations[m_ActiveWeapon].Attack[m_ActiveMovement]->update(position, camDir);
        }

        return;
    }

    if (m_IsTakingDamage)
    {
        if((SDL_GetTicks() - m_DamageStartTime) >= ANIM_TIME)
        {
            m_IsTakingDamage = false;

            int currentFrame = StopAll();
            m_Animations[m_ActiveWeapon].Move[m_ActiveMovement]->startRendering(currentFrame);
            m_Animations[m_ActiveWeapon].Move[m_ActiveMovement]->update(position, camDir);
        }
        else
        {
            m_Animations[m_ActiveWeapon].Damage[m_ActiveMovement]->update(position, camDir);
        }
        return;
    }

    if (!m_Animations[m_ActiveWeapon].Move[m_ActiveMovement]->isRendering())
    {
        int currentFrame = StopAll();
        m_Animations[m_ActiveWeapon].Move[m_ActiveMovement]->startRendering(currentFrame);
    }

    m_Animations[m_ActiveWeapon].Move[m_ActiveMovement]->update(position, camDir);
}

int PlayerAnimationsController::StopAll()
{
    int activeFrame = 0;
    for (int i = 0; i < PWT_Count; ++i)
    {
        for (int j = 0; j < PM_Count; ++j)
        {
            if(m_Animations[i].Move[j]->isRendering())
            {
                activeFrame = m_Animations[i].Move[j]->currentFrame();
                m_Animations[i].Move[j]->stopRendering();
            }

            if (m_Animations[i].Attack[j]->isRendering())
            {
                activeFrame = m_Animations[i].Attack[j]->currentFrame();
                m_Animations[i].Attack[j]->stopRendering();
            }

            if (m_Animations[i].Damage[j]->isRendering())
            {
                activeFrame = m_Animations[i].Damage[j]->currentFrame();
                m_Animations[i].Damage[j]->stopRendering();
            }
        }
    }

    return activeFrame;
}

