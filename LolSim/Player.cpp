#include "LolSim.h"

Player::Player(void):Base()
{
}

void Player::replaceSkill(Skill *oldSkill, Skill* newSkill) {
	std::vector<Skill*>::iterator it;
	for(it = skills.begin(); it != skills.end();) {
		if ((*it)->getIdentifier() == newSkill->getIdentifier()) {
			delete (*it);
			it=skills.erase(it);
		} else { it++; }
	}
	skills.push_back(newSkill);
}

Skill* Player::getSkill(std::string identifier) {
	std::vector<Skill *>::iterator it;
	for(it = skills.begin(); it != skills.end(); it++) {
		if ((*it)->getIdentifier() == identifier) {
			return (*it);
		}
	}
	return NULL;
}

std::vector<Skill *> Player::getSkills() {
	return skills;
}

void Player::addSkill(Skill *q) {
	skills.push_back(q);
}

void Player::takeDamage(Base* source, Damage damage) {
	Base::takeDamage(source,damage);
}
void Player::applyBuff(Buff *buffname) {
	if (log_level & 0x0100) {
    std::stringstream s;
    s << "Applying player buff";
    LogS::ToLog(s.str());
	}
	Base::applyBuff(buffname); // calling Base applyBuff
}
void Player::removeBuff(Buff *buffname){
	if (log_level & 0x0010) {
    std::stringstream s;
		s << "Removing player buff";
    LogS::ToLog(s.str());
	}
	Base::removeBuff(buffname); // calling Base removeBuff
}
void Player::cdReduce(double time) {
  for(std::vector<Skill*>::iterator skillit = skills.begin(); skillit != skills.end(); skillit++) {
    Skill* skill = (*skillit);
//	for each(Skill *skill in skills) {
		skill->reduceCooldown(time);
	}
	Base::cdReduce(time);
}

bool Player::isCreep() {
	return false;
}

void Player::initCamp(std::string camp){
	if(camp == "blue")
	{
		targets.push_back(new Creep());
		targets[0]->setName("Bluegolem");
		targets[0]->setMaxHealth(1275+175);
		targets[0]->setAttackDamage(110);
		targets[0]->setArmor(24);
		targets[0]->setAttackSpeed(0.613);
		targets[0]->setMovementSpeed(200);

		targets.push_back(new Creep());
		targets[1]->setName("Lizard1");
		targets[1]->setMaxHealth(350);
		targets[1]->setAttackDamage(18);
		targets[1]->setArmor(8);
		targets[1]->setAttackSpeed(0.679);
		
		targets.push_back(new Creep());
		targets[2]->setName("Lizard2");
		targets[2]->setMaxHealth(350);
		targets[2]->setAttackDamage(18);
		targets[2]->setArmor(8);
		targets[2]->setAttackSpeed(0.679);
		return;
	}
	if(camp == "wolves"){
		targets.push_back(new Creep());
		targets[0]->setName("Giant Wolf");
		targets[0]->setMaxHealth(550);
		targets[0]->setAttackDamage(26);
		targets[0]->setArmor(9);
		targets[0]->setAttackSpeed(0.679);
		targets[0]->setMovementSpeed(510);

		targets.push_back(new Creep());
		targets[1]->setName("Wolf1");
		targets[1]->setMaxHealth(400);
		targets[1]->setAttackDamage(18);
		targets[1]->setArmor(6);
		targets[1]->setAttackSpeed(0.679);
		targets[1]->setMovementSpeed(510);

		targets.push_back(new Creep());
		targets[2]->setName("Wolf2");
		targets[2]->setMaxHealth(400);
		targets[2]->setAttackDamage(18);
		targets[2]->setArmor(6);
		targets[2]->setAttackSpeed(0.679);
		targets[2]->setMovementSpeed(510);
		return;
	}
	if(camp == "wraiths"){
		targets.push_back(new Creep());
		targets[0]->setName("Wraith");
		targets[0]->setMaxHealth(350);
		targets[0]->setAttackDamage(60);
		targets[0]->setArmor(35);
		targets[0]->setAttackSpeed(0.638);
		targets[0]->setMovementSpeed(330);

		targets.push_back(new Creep());
		targets[1]->setName("Lesser Wraith1");
		targets[1]->setMaxHealth(220);
		targets[1]->setAttackDamage(35);
		targets[1]->setArmor(30);
		targets[1]->setAttackSpeed(0.638);
		targets[1]->setMovementSpeed(330);

		targets.push_back(new Creep());
		targets[2]->setName("Lesser Wraith2");
		targets[2]->setMaxHealth(220);
		targets[2]->setAttackDamage(35);
		targets[2]->setArmor(30);
		targets[2]->setAttackSpeed(0.638);
		targets[2]->setMovementSpeed(330);
		return;
	}
	if(camp == "red"){
		targets.push_back(new Creep());
		targets[0]->setName("Lizard Elder");
		targets[0]->setMaxHealth(1275+175);
		targets[0]->setAttackDamage(110);
		targets[0]->setArmor(24);
		targets[0]->setAttackSpeed(0.625);
		targets[0]->setMovementSpeed(330);

		targets.push_back(new Creep());
		targets[1]->setName("Lizard1");
		targets[1]->setMaxHealth(350);
		targets[1]->setAttackDamage(18);
		targets[1]->setArmor(8);
		targets[1]->setAttackSpeed(0.679);
		
		targets.push_back(new Creep());
		targets[2]->setName("Lizard2");
		targets[2]->setMaxHealth(350);
		targets[2]->setAttackDamage(18);
		targets[2]->setArmor(8);
		targets[2]->setAttackSpeed(0.679);
		return;	
	}
	if(camp == "golems"){
		targets.push_back(new Creep());
		targets[0]->setName("Golem1");
		targets[0]->setMaxHealth(800);
		targets[0]->setAttackDamage(65);
		targets[0]->setArmor(12);
		targets[0]->setAttackSpeed(0.613);
		targets[0]->setMovementSpeed(200);

		targets.push_back(new Creep());
		targets[1]->setName("Golem2");
		targets[1]->setMaxHealth(800);
		targets[1]->setAttackDamage(65);
		targets[1]->setArmor(12);
		targets[1]->setAttackSpeed(0.613);
		targets[1]->setMovementSpeed(200);
		return;
	}
  std::stringstream s;
  s << "Wrong camp initialization.";
  LogS::ToLog(s.str());
}

std::vector<Creep *>& Player::getTargets(){
	return targets;
}

Player::~Player(void)
{
  skills.clear();
//  std::for_each(skills.begin(),skills.end(),boost::lambda::delete_ptr());
//	skills.clear();
//	buffs.clear();

}
