#pragma once
#include <iostream>
#include <vector>
//#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)     
//#include <boost/function.hpp>
//#include <boost/bind.hpp>
//#include <boost/lambda/construct.hpp>
//#include <boost/format.hpp>
//#else
//#include <boost/function.hpp>
//#include <boost/bind.hpp>
//#endif
#include <string>
#include <algorithm>
#include <sstream>

class LogS
{
	static double time;
public:
	static void ToLog(std::string text);
	static void addTime(double time2);
};


//
//  Copyright (c) by Peter Varsanyi <superfly@gwelican.eu>
//  Basic class headers for LoLSimulator
//

// 0b0001 DamageTaken & Basic stuffs
// 0b0010 Skill
// 0b0100 Buff

const int LOG_BUFF =  0x0100;
const int LOG_SKILL = 0x0010;
const int LOG_OTHER = 0x0001;

const int log_level = 0x1111;

//double time_log = 0;

// 
// typedef void_cb to store functions with void return type and zero parameter
//

class Base;
typedef boost::function<void ()> void_cb;
typedef boost::function<void (Base *src, Base *obj)> twoparam_cb;
//typedef std::tr1::function<void ()> void_cb;
//typedef std::tr1::function<void (Base *src, Base *obj)> twoparam_cb;


//
// Damage struct, stores physical, true and magical damage, all data is publicly available
// friend function takes care of std::cout << if you wanna dump it on screen
//
struct Damage {
	double TrueDamage,MagicDamage,PhysicalDamage;
	Damage() {
		TrueDamage = 0;
		MagicDamage = 0;
		PhysicalDamage = 0;
	}
	Damage(double TrueDamage, double MagicDamage, double PhysicalDamage) {
		this->TrueDamage = TrueDamage;
		this->MagicDamage = MagicDamage;
		this->PhysicalDamage = PhysicalDamage;
	}
	bool operator==(Damage dmg) {
		if (TrueDamage == dmg.TrueDamage && MagicDamage == dmg.MagicDamage && PhysicalDamage == dmg.PhysicalDamage) {
			return true;
		} else { return false; }
	}
	Damage operator+(Damage dmg) {
		return Damage(TrueDamage+dmg.TrueDamage,MagicDamage+dmg.MagicDamage,PhysicalDamage+dmg.PhysicalDamage);
	}
	friend std::ostream& operator <<(std::ostream& stream, Damage d) {
		stream << "Damage, " << "True:" << d.TrueDamage << ", Physical:" << d.PhysicalDamage << ", MagicDamage:" << d.MagicDamage;
		return stream;
	}
};

class Skill {
private:
	bool aoe;
	std::string Name;
	double Cooldown;
	double MaxCooldown;
	double AbilityPowerScale;
	double AttackDamageScale;
	double BonusAttackDamageScale;
	double TrueDamage;
	int phase;
	int level;
	std::string identifier;
	Damage baseDamage;
	twoparam_cb _cb;
public:
	Skill(void);
	~Skill(void);
	void reduceCooldown(double time);
	bool isAoe();
	void setAoe(bool aoe);
	void registerCallBack(twoparam_cb cb);
	void setMaxCooldown(double time);
	void setCooldown(double time);
	void setName(std::string Name);
	void setBaseDamage(Damage baseDamage);
	void setBonusAttackDamageScale(double BonusAttackDamageScale);
	
	void setIdentifier(std::string identifier);
	std::string getIdentifier();

	void setLevel(int level);
	int getLevel();

	void setPhase(int phase);
	int getPhase();
	Damage getDamage(Base *src, Base* target);
	double getMaxCooldown();
	double getCooldown();
	std::string getName();
};

class Summoner : public Skill {
public:
	Summoner(void);
	void onUse(void);
	~Summoner(void);
};




//
// Basic buff class, fixme
// increaseStack: increase the stack of the buff with 1 or num(if specified)
// decreaseStack: decrease the stack of the buff with 1 or num(if specified)
// getStack: returns an integer with the stack of the buff
//
class Buff
{
private:
	bool TimeBased;
	bool Unique;
	double duration;
	int hitcount;
	int stack;
	double AttackSpeed;
	std::string Name;
	double Shield;
public:
	std::string getName();
	bool isUnique();
	bool isTimeBased();
	double getShield();
	void setShield(double Shield);
	void setName(std::string Name);
	void increaseStack(int num);
	void increaseStack();
	void decreaseStack(int num);
	void decreaseStack();
	void setStack(int num);
	void setDuration(double duration);
	void setHitCount(int hitcount);
	void setAttackSpeed(double AttackSpeed);
	double getAttackSpeed();
	int getHitCount();
	double getDuration();
	int getStack();
	Buff(void);
	~Buff(void);
};


//
// Base class contains basic stats and some virtual functions, abstract class, you can't instatiate it
// vector<Buff> buffs to store buffs
// debug fixme
// takeDamage: the unit receives a Damage type damage from source, calculates the armor, and magicresist, also takes into account the source penetrations
// getDamage: returns the basic autoattackdamage of a base object
// applyBuff: puts a buff in buffs
// removeBuff: deletes a buff from buffs
// getNextSwing: returns the time remaining till the object autoattack
// unitDie: the unit dies, called when the object has less than 1 hp
// cdReduce: reduces the swingtimer with double type time parameter
// setName: sets the Name property in the object
// getName: returns the Name string property of the object
//

class Base
{
private:
	int debug;
	std::vector<Buff *> buffs;
	double Health,MaxHealth,Resource,MaxResource, 
		HealthRegen, ResourceRegen, 
		AttackDamage, BonusAttackDamage, AttackSpeed, AttackRange,
		BonusAbilityPower, 
		MagicPenetration, PercentMagicPenetration, ArmorPenetration, PercentArmorPenetration,
		Armor, MagicResist, 
		MovementSpeed, NextSwing;
	std::string Name;
	Base* target;
public:
	Base(void);
	~Base(void);
	virtual Base* getTarget();
	virtual void setTarget(Base* target);
	virtual void takeDamage(Base* source, Damage damage); 
	virtual Damage getDamage(); 
	virtual void applyBuff(Buff *buffname); 
	virtual void removeBuff(Buff *buffname); 
	virtual void cdReduce(double time); 
	virtual double getNextSwing(); 
	virtual void setNextSwing(double time);
	virtual void unitDie();
	virtual bool Alive();
	virtual bool isCreep() = 0;
	virtual std::vector<Skill *> getSkills() = 0;
	virtual Skill* getSkill(std::string identifier) = 0;
	virtual void replaceSkill(Skill *oldSkill, Skill *newSkill) = 0;
	std::vector<Buff*> getBuffs();
	// setters

	void setName(std::string Name);
	void setAttackDamage(double AttackDamage);
	void setAttackSpeed(double AttackSpeed);
	void setAttackRange(double AttackRange);

	void setMaxResource(double MaxResource);
	void setMaxHealth(double MaxHealth);

	void setHealth(double Health);
	void setResource(double Resource);

	void setHealthRegen(double HealthRegen);
	void setResourceRegen(double ResourceRegen);

	void setArmor(double Armor);
	void setMagicResist(double MagicResist);
	void setMovementSpeed(double MovementSpeed);

	void setMagicPenetration(double MagicPenetration);
	void setArmorPenetration(double ArmorPenetration);

	void setBonusAttackDamage(double BonusAttackDamage);
	// getters

	std::string getName();
	double getAttackDamage();
	double getBonusAttackDamage();
	double getAttackSpeed();
	double getAttackRange();

	double getMaxResource();
	double getMaxHealth();

	double getHealth();
	double getResource();

	double getHealthRegen();
	double getResourceRegen();

	double getArmor();
	double getMagicResist();
	double getMovementSpeed();

	double getArmorPenetration();
	double getMagicPenetration();

	double getBonusAbilityPower();
};



//
// Creep class for handling jungle creeps, extends base
//
// unitDie_cb void_cb type vector, stores callback function pointers to call when unitDie fires
//
// the following functions gonna call the appropiate callback functions when they fire:
// - takeDamage: calls the Base::takeDamage, passing the 2 argument Base type source, and Damage type damage
// - applyBuff: takes a Buff type buffname, calls Base::applyBuff
// - removeBuff: takes a Buff type buffname, calls Base::removeBuff
// - unitDie: unit dies, calls Base::unitDie
//
// registerUnitDieCallBack takes void_cb type func, stores it in unitDie_cb
//
class Creep : public Base
{
private:
	std::vector<void_cb> unitDie_cb;
public:
	Creep(void);
	~Creep(void);
	Creep(const Creep& theOther);

	void takeDamage(Base* source, Damage damage);
	void applyBuff(Buff *buffname);
	void removeBuff(Buff *buffname);
	void cdReduce(double time);
	void unitDie();
	void registerUnitDieCallBack(void_cb func);
	bool isCreep();
	std::vector<Skill *> getSkills();
	Skill* getSkill(std::string identifier);
	void replaceSkill(Skill *oldSkill, Skill *newSkill);
	
};

//
// Player class
//
// skills: contains Skill object 
// summoners: contains Summoner object
//
// takeDamage: calls the Base::takeDamage, passing the 2 argument Base type source, and Damage type damage
// applyBuff: takes a Buff type buffname, calls Base::applyBuff
// removeBuff: takes a Buff type buffname, calls Base::removeBuff
// cdReduce: takes double type time, and reduces the player skill cooldown, and then calls Base::cdReduce
// triggerPassive: pure virtual functions, implemented in hero classes, used in callbacks
//



class Player : public Base
{
	std::vector<Skill *> skills;
	std::vector<Summoner> summoners;
	std::vector<Creep *> targets;
public:
	Player(void);
	~Player(void);

	void takeDamage(Base* source, Damage Damage);
	void applyBuff(Buff *buffname);
	void removeBuff(Buff *buffname);
	void cdReduce(double time);
	virtual void triggerPassive() = 0;
	bool isCreep();
	std::vector<Skill *> getSkills();
	void addSkill(Skill *q);
	Skill* getSkill(std::string identifier);
	void replaceSkill(Skill *oldSkill, Skill *newSkill);

	void initCamp(std::string camp);
	std::vector<Creep *>& getTargets();

	
};


//
// First hero class
//
// triggerPassive: implemented from Player, used in callbacks
// returns void_cb pointer to a function in LeeSin::triggerPassive
//
class LeeSin : public Player 
{
public:
	LeeSin(void);
	~LeeSin(void);
	void triggerPassive();
	void triggerSkillQ(Base *src, Base *target);
	void triggerSkillW(Base *src, Base *target);
	void triggerSkillE(Base *src, Base *target);
	void_cb triggerPassiveCallback();
};
