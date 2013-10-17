#include "LolSim.h"

LeeSin::LeeSin(void)
{ 
	this->setAttackDamage(55.8);
	this->setAttackSpeed(0.651);
	this->setAttackRange(125);

	this->setMaxResource(200);
	this->setResource(200);

	this->setMaxHealth(428);
	this->setHealth(428);

	this->setHealthRegen(6.25);
	this->setResourceRegen(50);

	this->setArmor(300);
	this->setMagicResist(30);
	this->setMovementSpeed(325);

	this->setBonusAttackDamage(60);

	Skill *q = new Skill();
	q->setName("Sonic Wave");
	q->setIdentifier("q");
	q->setPhase(1);
	q->setLevel(1);
	q->setMaxCooldown(11);
	q->setBonusAttackDamageScale(0.9);
	q->setBaseDamage(Damage(0,0,50));
	q->registerCallBack(boost::bind(&LeeSin::triggerSkillQ,*this,_1,_2));
	addSkill(q);

	Skill *w = new Skill();
	w->setName("Safeguard");
	w->setIdentifier("w");
	w->setPhase(1);
	w->setLevel(1);
	w->setMaxCooldown(8);
	w->setBaseDamage(Damage(0,0,0));
	w->registerCallBack(boost::bind(&LeeSin::triggerSkillW,*this,_1,_2));
	addSkill(w);

	Skill *e = new Skill();
	e->setIdentifier("e");
	e->setName("Tempest");
	e->setBaseDamage(Damage(0,60,0));
	e->setBonusAttackDamageScale(1);
	e->setPhase(1);
	e->setLevel(1);
	e->setAoe(true);
	e->setMaxCooldown(10);
	e->registerCallBack(boost::bind(&LeeSin::triggerSkillE,*this,_1,_2));
	addSkill(e);
}
void LeeSin::triggerSkillE(Base *src, Base *target) {
	return;
}
void LeeSin::triggerSkillW(Base *src, Base *target) {
	Buff *passive = new Buff();
	passive->setName("Flurry");
	passive->setDuration(3);
	passive->setHitCount(2);
	passive->setAttackSpeed(0.50);
	Buff *safeguard = new Buff();
	safeguard->setName("Safeguard");
	safeguard->setShield(40);
	safeguard->setDuration(5);
	safeguard->setShield(40);
	safeguard->setAttackSpeed(0);
	src->applyBuff(safeguard);
	src->applyBuff(passive);
	return;
}
void LeeSin::triggerSkillQ(Base *src, Base *target) {
	Buff *passive = new Buff();
	passive->setName("Flurry");
	passive->setDuration(3);
	passive->setHitCount(2);
	passive->setAttackSpeed(0.50);
	src->applyBuff(passive);
	if (log_level & LOG_BUFF) {
		std::stringstream s;
		s << "HA Q SKILL! Phase:" << src->getSkill("q")->getPhase();
		LogS::ToLog(s.str());
	}

	if (src->getSkill("q")->getPhase() == 2) { //phase 2 Resonating Strike
		double missingHealth = (target->getMaxHealth()-target->getHealth())*0.1;
		if (missingHealth > 400) missingHealth = 400; // max 400 against monsters
		Damage bonusDamage = Damage(0,missingHealth,0);
		if (log_level & 0x0010) {
      std::stringstream s;
      s << "Phase 2 bonus damage: " << missingHealth;
      LogS::ToLog(s.str());
		}
		target->takeDamage(src, bonusDamage);
	}

	if (src->getSkill("q")->getPhase() == 1) { // phase 1 Sonic Wave create phase 2 skill and replace it
		Skill *q = new Skill();
		q->setName("Sonic Wave");
		q->setIdentifier("q");
		q->setPhase(2);
		q->setLevel(1);
		q->setMaxCooldown(11);
		q->setCooldown(0.1);
		q->setBonusAttackDamageScale(0.9);
		q->setBaseDamage(Damage(0,0,50));
		q->registerCallBack(boost::bind(&LeeSin::triggerSkillQ,*this,_1,_2));
		src->replaceSkill(src->getSkill("q"),q);
	} else if (src->getSkill("q")->getPhase() == 2) { // phase 2 Resonating Strike create phase 1 skill and replace it
		Skill *q = new Skill();
		q->setName("Resonating strike");
		q->setIdentifier("q");
		q->setPhase(1);
		q->setLevel(1);
		q->setMaxCooldown(11);
		q->setCooldown(q->getMaxCooldown());
		q->setBonusAttackDamageScale(0.9);
		q->setBaseDamage(Damage(0,0,50));
		q->registerCallBack(boost::bind(&LeeSin::triggerSkillQ,*this,_1,_2));
		src->replaceSkill(src->getSkill("q"),q);
	}
	//return;
}

void LeeSin::triggerPassive() {
  std::stringstream s;
  s << "Haha KILL!";
  LogS::ToLog(s.str());
}
boost::function<void ()> LeeSin::triggerPassiveCallback() {
	return boost::bind(&LeeSin::triggerPassive,*this); // return the bound function of the current lee sin object
}

LeeSin::~LeeSin(void)
{
}
