#include "LolSim.h"
Skill::Skill(void)
{
	phase = 0;
	aoe = false;
	level = 0;
	Cooldown = 0;
	MaxCooldown = 0;
	BonusAttackDamageScale = 0;
	AttackDamageScale = 0;
	AbilityPowerScale = 0;
	TrueDamage = 0;
	baseDamage = Damage(0,0,0);
	_cb = NULL;
}

Skill::~Skill(void)
{
  //boost::lambda::delete_ptr(_cb);
//	delete _cb;
}

void Skill::setAoe(bool aoe) {
	this->aoe = aoe;
}
bool Skill::isAoe() {
	return aoe;
}

void Skill::setIdentifier(std::string identifier) {
	this->identifier = identifier;
}
std::string Skill::getIdentifier() {
	return identifier;
}
void Skill::setPhase(int phase) { this->phase = phase; }
int Skill::getPhase() {	return phase; }
int Skill::getLevel() { return level; }
void Skill::setLevel(int level) { this->level = level; }

void Skill::setBaseDamage(Damage baseDamage) {
	this->baseDamage = baseDamage;
}
void Skill::setBonusAttackDamageScale(double BonusAttackDamageScale) {
	this->BonusAttackDamageScale = BonusAttackDamageScale;
}

void Skill::registerCallBack(twoparam_cb cb) {
	_cb = cb;
}

double Skill::getMaxCooldown() {
	return MaxCooldown;
}

Damage Skill::getDamage(Base *src, Base *target) {
	Damage x = baseDamage+Damage(TrueDamage, src->getBonusAbilityPower()*AbilityPowerScale, src->getBonusAttackDamage()*BonusAttackDamageScale+src->getAttackDamage()*AttackDamageScale);
	if (log_level & 0x0010) {
    std::stringstream s;
    s << "##Calculating damage for " << Name << " " << x;
    LogS::ToLog(s.str());

	}
	if (_cb != NULL) {
		_cb(src,target);
	}
	return x;
}

void Skill::setCooldown(double time) {
	Cooldown = time;
}
void Skill::setName(std::string Name) {
	this->Name = Name;
}
std::string Skill::getName() {
	return Name;
}

double Skill::getCooldown() {
	return Cooldown;
}
void Skill::reduceCooldown(double time) {
	Cooldown-= time;
}
void Skill::setMaxCooldown(double time) {
	MaxCooldown = time;
}
