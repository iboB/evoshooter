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
    void AddAttack(const std::string& fileAttack, const std::string& fileAttackIdle, const mathgp::vector3& offset, float scale, bool wholeBodyAttack);

    void AddAttachment(const std::string& file, const mathgp::vector3& offset, float scale);
    void RemoveAttachment(const std::string& file);
    
    void SetMove(MovementAnim anim);
    void Die();
    void GetDamage();
    void Attack(Uint32 attackIndex);

    void update(const mathgp::vector3& position, const mathgp::vector3& camDir);

    bool isReadyToDiscard() { return m_IsReadyForDiscard; }
private:
    void updateAttack(const mathgp::vector3& position, const mathgp::vector3& camDir);
    void updateAttachments(const mathgp::vector3& position, const mathgp::vector3& camDir);

    struct AnimWithOffset
    {
        mathgp::vector3 Offsets[MA_Count];
        SpritePtr Animation[MA_Count];
    };

    struct AttackAnimation
    {
        mathgp::vector3 Offsets[MA_Count];
        SpritePtr Animation[2][MA_Count];
        bool m_IsWholeBodyAttack;
    };

    typedef std::map<std::string, AnimWithOffset> Animations;
    typedef std::vector<AttackAnimation> AttackAnimations;

    SpritePtr m_MovementAnimations[MA_Count];
    SpritePtr m_Death[MA_Count];
    SpritePtr m_Damage[MA_Count];
    AttackAnimations m_Attacks;

    Animations m_Attachments;

    MovementAnim m_ActiveMovement;
    bool m_IsTakingDamage;
    bool m_IsDead;
    Uint32 m_DamageStartTime;

    Uint32 m_ActiveAttack;
    bool m_IsAttacking;
    int m_AtackStartTime;
    bool m_IsReadyForDiscard;
};

enum PlayerWeaponType
{
    PWT_Sword,
    PWT_Pistol,
    PWT_Shotgun,

    PWT_Count
};

enum PlayerMovement
{
    PM_Idle,
    PM_LEft,
    PM_Right,
    PM_Count
};

class PlayerAnimationsController
{
public:
    PlayerAnimationsController();
    ~PlayerAnimationsController();

    void SetWeapon(PlayerWeaponType weapon);
    void SetMovement(PlayerMovement movement);

    void Die();
    void GetDamage();
    void Attack();

    void update(const mathgp::vector3& position, const mathgp::vector3& camDir);

    bool isReadyToDiscard() { return m_IsReadyForDiscard; }
    bool isDead() { return m_IsDead; }

private:
    
    int StopAll();

    struct WeaponSet
    {
        SpritePtr Move[PM_Count];
        SpritePtr Attack[PM_Count];
        SpritePtr Damage[PM_Count];
    };

    WeaponSet m_Animations[PWT_Count];

    SpritePtr m_DieAnimation[PM_Count];

    PlayerWeaponType m_ActiveWeapon;
    PlayerMovement m_ActiveMovement;

    bool m_IsTakingDamage;
    bool m_IsDead;
    Uint32 m_DamageStartTime;

    bool m_IsAttacking;
    int m_AtackStartTime;
    bool m_IsReadyForDiscard;
};
