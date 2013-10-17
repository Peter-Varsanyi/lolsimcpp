#include "LolSim.h"

Base::Base(void)
{
	NextSwing = 0;
	Health = 0;
	MaxHealth = 0;
	Resource = 0;
	MaxResource = 0; 
	HealthRegen = 0;
	ResourceRegen = 0; 
	AttackDamage = 0;
	BonusAttackDamage = 0;
	AttackSpeed = 0;
	AttackRange = 0;
	BonusAbilityPower = 0; 
	MagicPenetration = 0; 
	PercentMagicPenetration= 0;
	ArmorPenetration=0;
	PercentArmorPenetration=0;
	Armor=0;
	MagicResist=0;
	MovementSpeed = 0;
	NextSwing = 0;
	target = NULL;
}

Base* Base::getTarget() {
	return target;
}
bool Base::Alive() {
	if (Health>0) return true;
	return false;
}
void Base::setTarget(Base* target) {
	this->target = target;
}
bool Base::isCreep() {
	return true;
}
void Base::unitDie() {
	std::cerr << "Base object unitdie, not implemented" << std::endl;
}
Damage Base::getDamage() {
	return Damage(0,0,AttackDamage+BonusAttackDamage); // new damage, 0 true, 0 magic, ad+bonusad physical damage
}

double Base::getNextSwing() {
	return NextSwing;
}
double Base::getBonusAbilityPower() {
	return BonusAbilityPower;
}
void Base::setBonusAttackDamage(double BonusAttackDamage) {
	this->BonusAttackDamage = BonusAttackDamage;
}
void Base::setNextSwing(double time) {
	NextSwing = time;
}
void Base::setName(std::string Name) { this->Name = Name; }
std::string Base::getName() { return Name; }
void Base::cdReduce(double time) { // reduces the swing timer by time
	NextSwing -= time;
	for(std::vector<Buff*>::iterator buffit = buffs.begin(); buffit != buffs.end(); buffit++) {
    Buff *buff = (*buffit);
		buff->setDuration(buff->getDuration()-time);
	}
}
std::vector<Buff*> Base::getBuffs() {
	return buffs;
}
void Base::takeDamage(Base* source,Damage damage) {
	if (damage == Damage(0,0,0)) {
		return;
	}
	// 1. percentage armor reduction
	// 2. flat armor reduction
	// 3. flat amor penetration
	// 4. percentage armor penetration
	// 1
	// 2
	// 3

	double armor = Armor;
	if (armor >= source->getArmorPenetration()) {
		armor -= source->getArmorPenetration();
	} else {
		armor = 0;
	}
	// 4
	// 1. percentage magic resistance reduction
	// 2. flat magic resistance reduction
	// 3. flat magic resistance penetration
	// 4. percentage magic resistance penetration
	double magicresist = MagicResist;

	// 1
	// 2
	// 3
	if (magicresist >= source->getMagicResist()) {
		magicresist -= source->getMagicPenetration();
	} else {
		magicresist = 0;
	}
	// 4
	double phys_mult = 100/(100+armor);
	double magic_mult = 100/(100+magicresist);
	double dmg = damage.MagicDamage*magic_mult+damage.PhysicalDamage*phys_mult+damage.TrueDamage;
  for(std::vector<Buff*>::iterator buffit = buffs.begin(); buffit != buffs.end(); buffit++) {
    Buff *buff = (*buffit);
//	for each (Buff *buff in buffs) {
		if (buff->getShield() > 0) {
  		if (dmg > buff->getShield()) {
	  		dmg -= buff->getShield();
			if (log_level & 0x0001) {
          std::stringstream s;
          s << "Partial absorbning: " << buff->getShield();
          LogS::ToLog(s.str());
			}
  			buff->setShield(0);
	  	} else {
		  	buff->setShield(buff->getShield()-dmg);
			if (log_level & 0x0001) {
          std::stringstream s;
          s << "Absorbing: " << dmg;
          LogS::ToLog(s.str());
			}
  			dmg = 0;
	  	}
    }
	}
  Health -= dmg;
  if (log_level & LOG_OTHER) {
    std::stringstream s;
    s << getName() << " " << getHealth() << "/" << getMaxHealth() <<" damage taken: " << dmg << ", from: " << source->getName();
    LogS::ToLog(s.str());
  }
  }

  void Base::removeBuff(Buff *buffname) {
    std::vector<Buff*>::iterator it;

    for(it = buffs.begin(); it != buffs.end(); ) {
      if ((*it)->getStack() > 1) {
        (*it)->decreaseStack();
        it++;
      } else {
        AttackSpeed = AttackSpeed/= 1+(*it)->getAttackSpeed();
        if (log_level & 0x0100) {
          //std::stringstream s;
          //s << "Remove buff: " << (*it)->getName();
          //LogS::ToLog(s.str());
        }
        delete *it;
        it = buffs.erase(it); // it points to the new element already, so no increment
      }
    }
  }
  void Base::applyBuff(Buff *buffname) {
    std::vector<Buff*>::iterator it;
    bool found = false;

    for(it = buffs.begin(); it != buffs.end();it++) {
      if ((*it)->getName() == buffname->getName()) {
        found = true;
        break;
        //			skill = (*it);
      }
    }
    if (found == true && buffname->isUnique()) {
      if (log_level & 0x0100) {
        std::stringstream s;
        s << "Not Applying buff, refreshing" << AttackSpeed;
        LogS::ToLog(s.str());
      }
      (*it)->setDuration(buffname->getDuration());
    } else {
      buffs.push_back(buffname);		
      if (log_level & 0x0100) {
        std::stringstream s;
        s << "Pre attackspeed: " << AttackSpeed;
        LogS::ToLog(s.str());
      }
      AttackSpeed *= 1+buffname->getAttackSpeed();
      if (log_level & 0x0100) {
        std::stringstream s;
        s << "Applying buff" << buffname->getName() << " attackspeed:" << AttackSpeed;
        LogS::ToLog(s.str());
      }
    }
  }

  void Base::setAttackDamage(double AttackDamage) { this->AttackDamage = AttackDamage; }
  void Base::setAttackSpeed(double AttackSpeed) { this->AttackSpeed = AttackSpeed; }
  void Base::setAttackRange(double AttackRange) { this->AttackRange = AttackRange; }
  void Base::setMaxResource(double MaxResource) { this->MaxResource = MaxResource; }
  void Base::setResource(double Resource) { this->Resource = Resource; }
  void Base::setMaxHealth(double MaxHealth) {	this->MaxHealth = MaxHealth; this->Health = MaxHealth; }
  void Base::setHealth(double Health) { this->Health = Health; }
  void Base::setHealthRegen(double HealthRegen) { this->HealthRegen = HealthRegen;}
  void Base::setResourceRegen(double ResourceRegen) { this->ResourceRegen = ResourceRegen; }
  void Base::setArmor(double Armor) { this->Armor = Armor; }
  void Base::setMagicResist(double MagicResist) {	this->MagicResist = MagicResist; }
  void Base::setMovementSpeed(double MovementSpeed) { this->MovementSpeed = MovementSpeed; }
  void Base::setMagicPenetration(double MagicPenetration) { this->MagicPenetration = MagicPenetration; }
  void Base::setArmorPenetration(double ArmorPenetration) { this->ArmorPenetration = ArmorPenetration; }

  double Base::getAttackDamage() { return AttackDamage; }
  double Base::getBonusAttackDamage() { return BonusAttackDamage; }
  double Base::getAttackRange() { return AttackRange; }
  double Base::getAttackSpeed() { return AttackSpeed; }
  double Base::getMaxHealth() { return MaxHealth; }
  double Base::getMaxResource() { return MaxResource; }
  double Base::getHealth() { return Health; }
  double Base::getResource() { return Resource; }
  double Base::getResourceRegen() { return ResourceRegen; }
  double Base::getHealthRegen() { return HealthRegen; }
  double Base::getArmor() { return Armor; }
  double Base::getMagicResist() { return MagicResist; }
  double Base::getMovementSpeed() { return MovementSpeed; }
  double Base::getArmorPenetration() { return ArmorPenetration; }
  double Base::getMagicPenetration() { return MagicPenetration; }

  Base::~Base(void)
  {
    buffs.clear();
    delete this->target;
  }
