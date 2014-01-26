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

enum EObjectType
{
    EBase_Object = 0,
    EBase_Character,
    EMonster_Character,
    EPlayer_Character,
    EBullet,
    EStatic,
    EObject_Type_Count
};

enum EAttackDamageType
{
    EMonsterDamage = 0, //full armor from all
    EPiercing, //bypasses armor
    EBlunt  //bypasses fatness
};

class Object
{
public:
	Object();
	Object(const mathgp::vector3& pos, float bc);
	virtual ~Object();

	mathgp::vector3 position() const;
	float boundingCircle();
	float x();
    void  x(float val);
	float y();
    void  y(float val);
	float r() const;
    bool collidesWith(std::shared_ptr<Object> otherGuy);
    float bb_w();
    float bb_h();
    float size() const { return m_size; }

    bool shouldRenderShadow() { return m_renderShadow;  }

    virtual void update(int dt){}

    unsigned int& id() { return m_id; }

    virtual void Move(const mathgp::vector3& position){}
    virtual void Die(){}
    virtual void GetDamage(){}
    virtual void Attack(Uint32 attackIndex){}
    virtual void OnHit(EAttackDamageType dmgType, int dmg);

    void type(EObjectType t) { m_type = t; }
    EObjectType type() const { return m_type; }
    mathgp::vector2 hitDetectionOffset() const { return m_hitDetectionOffset;  }
protected:

	mathgp::vector3 m_pos;
	float m_boundingCircle;
    mathgp::vector2 m_bb; //w,h of the bounding quad
    mathgp::vector2 m_hitDetectionOffset;
    bool m_renderShadow;
    float m_size;
    unsigned int m_id;

    EObjectType m_type;
};
