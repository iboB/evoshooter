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
#pragma once

class Sprite;
typedef std::shared_ptr<Sprite> SpritePtr;

enum MovementAnim
{
    MA_None = -1,
    MA_Left,
    MA_Right,

    MA_Count
};

class AnimationsController
{
public:
    AnimationsController();
    ~AnimationsController();
    
    void SetMovementAnimations(const std::string& fileMove, float scale);
    void SetDeath(const std::string& fileDeath, float scale);
    void SetDamage(const std::string& fileDamage, float scale);
    void AddAttachment(const std::string& file, const mathgp::vector3& offset, float scale);
    void RemoveAttachment(const std::string& file);
    
    void SetMove(MovementAnim anim);
    void Die();
    void GetDamage();

    void update(const mathgp::vector3& position, const mathgp::vector3& camDir);

private:
    void updateAttachments(const mathgp::vector3& position, const mathgp::vector3& camDir);

    struct AnimWithOffset
    {
        mathgp::vector3 Offset;
        SpritePtr Animation[MA_Count];
    };

    typedef std::map<std::string, AnimWithOffset> Animations;

    SpritePtr m_MovementAnimations[MA_Count];
    SpritePtr m_Death[MA_Count];
    SpritePtr m_Damage[MA_Count];

    Animations m_Attachments;

    MovementAnim m_ActiveMovement;
    bool m_IsTakingDamage;
    bool m_IsDead;
    Uint32 m_DamageStartTime;
};