#include "LolSim.h"

Creep::Creep(void) {
}

Creep::Creep(const Creep& theOther){
	*this = theOther;
}

void Creep::takeDamage(Base* source, Damage damage) {
	Base::takeDamage(source, damage);
}

void Creep::replaceSkill(Skill *oldSkill, Skill *newSkill) {
}

std::vector<Skill *> Creep::getSkills() {
	std::vector<Skill*> vec;
	return vec;
}
Skill* Creep::getSkill(std::string identifier) {
	return NULL;
}

void Creep::registerUnitDieCallBack(void_cb a) {
	unitDie_cb.push_back(a); 
}
void Creep::applyBuff(Buff *buffname) {
  LogS::ToLog("Applying creep buff");
	Base::applyBuff(buffname); 
}
void Creep::removeBuff(Buff *buffname){
  LogS::ToLog("Removing creep buff");
	Base::removeBuff(buffname);
}
void Creep::cdReduce(double time) {
	Base::cdReduce(time);
}
bool Creep::isCreep() {
	return true;
}
void Creep::unitDie() {
  std::stringstream s;
  s << "Unit died: " << getName();
  LogS::ToLog(s.str());
	for(std::vector<void_cb>::iterator it = unitDie_cb.begin(); it != unitDie_cb.end(); it++) {
		(*it)();
	}
}
Creep::~Creep(void)
{
	unitDie_cb.clear();
}
